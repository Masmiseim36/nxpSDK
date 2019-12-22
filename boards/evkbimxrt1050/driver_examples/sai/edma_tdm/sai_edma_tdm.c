/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "fsl_cs42888.h"
#include "fsl_sd_disk.h"
#include "fsl_codec_common.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_codec_adapter.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#define DEMO_SAI SAI1
#define DEMO_SAI_IRQ SAI1_IRQn
#define SAI_TxIRQHandler SAI1_IRQHandler

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (1U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (15U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* I2C instance and clock */
#define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* DMA */
#define DMAMUX0 DMAMUX
#define EXAMPLE_DMA DMA0
#define EXAMPLE_CHANNEL (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx

#define DEMO_CODEC_RESET_GPIO GPIO1
#define DEMO_CODEC_RESET_GPIO_PIN 19
#define OVER_SAMPLE_RATE (384U)
#define BUFFER_SIZE (2048U)
#define BUFFER_NUM (4U)

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate48KHz)
/* demo audio master clock */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)
/* demo audio bitwidth */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits
#define DEMO_FRMAE_SYNC_LEN kSAI_FrameSyncLenOneBitClk
#define DEMO_SAI_CHANNEL kSAI_Channel0Mask
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BORAD_CodecReset(bool state);
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void SDCARD_DetectCallBack(bool isInserted, void *userData);
static status_t DEMO_MountFileSystem(void);
extern void BORAD_CodecReset(bool state);
static void DEMO_InitCS42888(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

cs42888_config_t cs42888Config = {
    .DACMode      = kCS42888_ModeSlave,
    .ADCMode      = kCS42888_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000U, .sampleRate = 48000U, .bitWidth = 24U},
    .bus          = kCS42888_BusTDM,
    .slaveAddress = CS42888_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42888, .codecDevConfig = &cs42888Config};

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaHandle                                  = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_NUM * BUFFER_SIZE], 4);
volatile bool isFinished      = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock  = BUFFER_NUM;
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
sai_master_clock_t mclkConfig = {
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    mclkOutputEnable = true,
#if !(defined(FSL_FEATURE_SAI_HAS_NO_MCR_MICS) && (FSL_FEATURE_SAI_HAS_NO_MCR_MICS))
    mclkSource = kSAI_MclkSourceSysclk,
#endif
#endif
};
#endif
/*! @brief Card descriptor. */
extern sd_card_t g_sd;
/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
    .cardInserted = SDCARD_DetectCallBack,
    .cardRemoved  = SDCARD_DetectCallBack,
};

/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = {
    .powerOn          = BOARD_PowerOnSDCARD,
    .powerOnDelay_ms  = 500U,
    .powerOff         = BOARD_PowerOffSDCARD,
    .powerOffDelay_ms = 0U,
};
#endif
/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
static const sdmmchost_card_switch_voltage_func_t s_sdCardVoltageSwitch = {
    .cardSignalLine1V8 = BOARD_USDHC_Switch_VoltageTo1V8,
    .cardSignalLine3V3 = BOARD_USDHC_Switch_VoltageTo3V3,
};
#endif
static uint32_t volatile s_writeIndex = 0U;
static uint32_t volatile s_readIndex  = 0U;
static uint32_t volatile s_emptyBlock = BUFFER_NUM;
static FATFS s_fileSystem; /* File system object */
static FIL s_fileObject;
static FILINFO s_fileInfo;
static volatile bool s_cardInserted      = false;
static volatile bool s_saiTransferFinish = false;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_USDHCClockConfiguration(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}

/*${function:start}*/
void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}


void BORAD_CodecReset(bool state)
{
    if (state)
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
static void SDCARD_DetectCallBack(bool isInserted, void *userData)
{
    s_cardInserted = isInserted;
}
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
    }
    else
    {
        s_saiTransferFinish = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    edma_config_t dmaConfig = {0};
    sai_transceiver_t config;
    UINT oneTimeRW = 0U;
    UINT bytesRead;
    FRESULT error;
    uint32_t leftWAVData = 0U;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

    BOARD_USDHCClockConfiguration();

    GPIO_PinInit(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, &gpio_config);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

    PRINTF("\r\nSAI edma TDM example started.\n\r");

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_CHANNEL);
#endif

    if (DEMO_MountFileSystem() != kStatus_Success)
    {
        PRINTF("Mount file system failed, make sure card is formatted.\r\n");
        return -1;
    }

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle);

    /* TDM mode configurations */
    SAI_GetTDMConfig(&config, DEMO_FRMAE_SYNC_LEN, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL, DEMO_SAI_CHANNEL);
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ,
#endif
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

    /* CS42888 initialization */
    DEMO_InitCS42888();

    /* The 8_TDM.wav file process flow:
     * 1.Full fill the transfer buffer firstly, it is important to make sure the audio data is transferred continuously.
     * 2.Send one buffer block through SAI when there is at least one buffer block is full.
     * 3.read one buffer block from sdcard when there is at least one buffer block is empty and transfer done.
     * with step2/step3 repeat, audio data will be sent out continuously.
     * */
    if (s_fileInfo.fsize > BUFFER_SIZE * BUFFER_NUM)
    {
        oneTimeRW = BUFFER_SIZE * BUFFER_NUM;
    }
    else
    {
        oneTimeRW = s_fileInfo.fsize;
    }
    error = f_read(&s_fileObject, s_buffer, oneTimeRW, &bytesRead);
    if ((error) || (bytesRead != oneTimeRW))
    {
        PRINTF("Read file failed.\r\n");
        return -1;
    }
    s_emptyBlock -= BUFFER_NUM;
    s_readIndex += BUFFER_NUM;
    leftWAVData = s_fileInfo.fsize - oneTimeRW;
    oneTimeRW   = BUFFER_SIZE;

    PRINTF("\r\nStart play 8_TDM.wav file.\n\r");

    while (leftWAVData > BUFFER_SIZE)
    {
        /* wait at least one buffer block is full */
        if (s_emptyBlock < BUFFER_NUM)
        {
            if (s_writeIndex >= BUFFER_NUM)
            {
                s_writeIndex = 0U;
            }

            /* xfer structure */
            xfer.data     = (uint8_t *)((uint32_t)s_buffer + BUFFER_SIZE * s_writeIndex);
            xfer.dataSize = BUFFER_SIZE;
            /* Wait for available queue. */
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                s_writeIndex++;
                s_emptyBlock++;
            }
        }
        /* wait at least one buffer block is empty and transfer done. */
        if ((s_emptyBlock) && (s_saiTransferFinish))
        {
            s_saiTransferFinish = false;

            if (s_readIndex >= BUFFER_NUM)
            {
                s_readIndex = 0U;
            }

            error = f_read(&s_fileObject, (uint8_t *)((uint32_t)s_buffer + s_readIndex * BUFFER_SIZE), BUFFER_SIZE,
                           &bytesRead);
            if ((error) || (bytesRead != BUFFER_SIZE))
            {
                PRINTF("Read file failed.\r\n");
                return -1;
            }

            s_readIndex++;
            s_emptyBlock--;
            leftWAVData -= BUFFER_SIZE;
        }
    }
    f_close(&s_fileObject);
    /* Once transfer finish, disable SAI instance. */
    SAI_TransferAbortSendEDMA(DEMO_SAI, &txHandle);
    SAI_Deinit(DEMO_SAI);
    PRINTF("\r\nSAI TDM EDMA example finished.\n\r ");
    while (1)
    {
    }
}

#if defined(SAI_ErrorIRQHandler)
void SAI_ErrorIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif

static status_t sdcardWaitCardInsert(void)
{
    /* Save host information. */
    g_sd.host.base           = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif
#if defined DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
    g_sd.usrParam.cardVoltage = &s_sdCardVoltageSwitch;
#endif
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);

    PRINTF(
        "\r\nPlease insert a SDCARD into board, make sure the sdcard is format to FAT32 format and put the 8_TDM.wav "
        "file into the sdcard.\r\n");

    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static status_t DEMO_MountFileSystem(void)
{
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    FRESULT error;

    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return kStatus_Fail;
    }

    if (f_mount(&s_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return kStatus_Fail;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return kStatus_Fail;
    }
#endif

    error = f_open(&s_fileObject, _T("/8_TDM.wav"), FA_READ);
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("8_TDM.wav File exists.\r\n");
        }
        else
        {
            PRINTF("8_TDM file not exist.\r\n");
            return kStatus_Fail;
        }
    }

    error = f_stat(_T("/8_TDM.wav"), &s_fileInfo);
    if (error != FR_OK)
    {
        PRINTF("Get file status failed\r\n");
        return kStatus_Fail;
    }

    PRINTF("\r\n8_TDM.wav File is available\r\n");

    return kStatus_Success;
}

static void DEMO_InitCS42888(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
    }

    PRINTF("\r\nCS42888 codec Init Done.\r\n");
}
