/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"

#include "fsl_sgtl5000.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_SAI I2S0
#define DEMO_I2C I2C0
#define DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_SAI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_DMA DMA0
#define EXAMPLE_DMAMUX DMAMUX
#define EXAMPLE_TX_CHANNEL (0U)
#define EXAMPLE_RX_CHANNEL (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMux0I2S0Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMux0I2S0Rx
#define SAI_UserTxIRQHandler I2S0_Tx_IRQHandler
#define SAI_UserRxIRQHandler I2S0_Rx_IRQHandler
#define DEMO_SAI_TX_IRQ I2S0_Tx_IRQn
#define DEMO_SAI_RX_IRQ I2S0_Rx_IRQn
#define DEMO_CODEC_I2C_INSTANCE 0

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
#if defined DEMO_SDCARD
#include "ff.h"
#include "diskio.h"
#include "fsl_sd.h"
/*!
 * @brief wait card insert function.
 */
static status_t sdcardWaitCardInsert(void);
static int SD_FatFsInit(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
sgtl_config_t sgtlConfig = {
    .i2cConfig    = {.codecI2CInstance = DEMO_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 60000000},
    .route        = kSGTL_RoutePlaybackandRecord,
    .slaveAddress = SGTL5000_I2C_ADDR,
    .bus          = kSGTL_BusI2S,
    .format       = {.mclk_HZ = 6144000U, .sampleRate = 16000U, .bitWidth = 16},
    .master_slave = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_SGTL5000, .codecDevConfig = &sgtlConfig};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaTxHandle                                = {0};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0};
edma_handle_t dmaRxHandle                                = {0};
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
extern codec_config_t boardCodecConfig;
volatile bool istxFinished     = false;
volatile bool isrxFinished     = false;
volatile uint32_t beginCount   = 0;
volatile uint32_t sendCount    = 0;
volatile uint32_t receiveCount = 0;
volatile bool sdcard           = false;
volatile uint32_t fullBlock    = 0;
volatile uint32_t emptyBlock   = BUFFER_NUM;
#if defined DEMO_SDCARD
/* static values for fatfs */
AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */
AT_NONCACHEABLE_SECTION(BYTE work[FF_MAX_SS]);
/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};
extern sd_card_t g_sd; /* sd card descriptor */
/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = {
    .powerOn          = BOARD_PowerOnSDCARD,
    .powerOnDelay_ms  = 500U,
    .powerOff         = BOARD_PowerOffSDCARD,
    .powerOffDelay_ms = 0U,
};
#endif
#endif
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
sai_master_clock_t mclkConfig = {
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    .mclkOutputEnable = true,
#if !(defined(FSL_FEATURE_SAI_HAS_NO_MCR_MICS) && (FSL_FEATURE_SAI_HAS_NO_MCR_MICS))
    .mclkSource = kSAI_MclkSourceSysclk,
#endif
#endif
};
#endif
sai_transceiver_t config;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}
static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sendCount++;
    emptyBlock++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        SAI_TransferTerminateSendEDMA(base, handle);
        sendCount = 0;
    }
}

static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    receiveCount++;
    fullBlock++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        SAI_TransferTerminateReceiveEDMA(base, handle);
        receiveCount = 0;
    }
}

#if defined DEMO_SDCARD
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
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
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

int SD_FatFsInit()
{
    /* If there is SDCard, Initialize SDcard and Fatfs */
    FRESULT error;

    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

    PRINTF("\r\nPlease insert a card into board.\r\n");

    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return -1;
    }

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif

#if FF_USE_MKFS
    PRINTF("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
    if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
    {
        PRINTF("Make file system failed.\r\n");
        return -1;
    }
#endif /* FF_USE_MKFS */

    PRINTF("\r\nCreate directory......\r\n");
    error = f_mkdir(_T("/record"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            return -1;
        }
    }

    return 0;
}
#endif /* DEMO_SDCARD */

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t dmaConfig = {0};
    char input              = '1';
    uint8_t userItem        = 1U;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    PRINTF("SAI Demo started!\n\r");

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);
    config.syncMode = kSAI_ModeSync;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

/* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
#endif
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);

    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_TX_IRQ);
    EnableIRQ(DEMO_SAI_RX_IRQ);

#if defined DEMO_SDCARD
    /* Init SDcard and FatFs */
    if (SD_FatFsInit() != 0)
    {
        PRINTF("SDCARD init failed !\r\n");
    }
#endif /* DEMO_SDCARD */

    PRINTF("\n\rPlease choose the option :\r\n");
    while (1)
    {
        PRINTF("\r%d. Record and playback at same time\r\n", userItem++);
        PRINTF("\r%d. Playback sine wave\r\n", userItem++);
#if defined DEMO_SDCARD
        PRINTF("\r%d. Record to SDcard, after record playback it\r\n", userItem++);
#endif /* DEMO_SDCARD */
#if defined DIG_MIC
        PRINTF("\r%d. Record using digital mic and playback at the same time\r\n", userItem++);
#endif
        PRINTF("\r%d. Quit\r\n", userItem);

        input = GETCHAR();
        PUTCHAR(input);
        PRINTF("\r\n");

        if (input == (userItem + 48U))
        {
            break;
        }

        switch (input)
        {
            case '1':
#if defined DIG_MIC
                /* Set the audio input source to AUX */
                DA7212_ChangeInput((da7212_handle_t *)((uint32_t)(codecHandle.codecDevHandle)), kDA7212_Input_AUX);
#endif
                RecordPlayback(DEMO_SAI, 30);
                break;
            case '2':
                PlaybackSine(DEMO_SAI, 250, 5);
                break;
#if defined DEMO_SDCARD
            case '3':
                RecordSDCard(DEMO_SAI, 5);
                break;
#endif
#if defined DIG_MIC
            case userItem - 1U + 48U:
                /* Set the audio input source to DMIC */
                DA7212_ChangeInput((da7212_handle_t *)((uint32_t)(codecHandle.codecDevHandle)), kDA7212_Input_MIC1_Dig);
                RecordPlayback(DEMO_SAI, 30);
                break;
#endif
            default:
                PRINTF("\rInvallid Input Parameter, please re-enter\r\n");
                break;
        }
        userItem = 1U;
    }

    CODEC_Deinit(&codecHandle);
    PRINTF("\n\r SAI demo finished!\n\r ");
    while (1)
    {
    }
}

void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void SAI_UserRxIRQHandler(void)
{
    SAI_RxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SAI_RxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void SAI_UserIRQHandler(void)
{
    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }

    if (SAI_RxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
    __DSB();
}
