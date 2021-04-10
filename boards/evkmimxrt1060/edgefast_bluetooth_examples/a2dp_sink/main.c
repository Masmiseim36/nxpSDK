/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "app_a2dp_sink.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "fsl_adapter_uart.h"
#include "controller.h"
#include "host_msd_fatfs.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define DEMO_AUDIO_SAI       SAI1
#define DEMO_SAI_IRQ_TX      SAI1_IRQn
#define SAI_UserTxIRQHandler SAI1_IRQHandler
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)

/* DMA */
#define EXAMPLE_DMAMUX        DMAMUX
#define EXAMPLE_DMA           DMA0
#define EXAMPLE_DMA_IRQ       DMA0_DMA16_IRQn
#define EXAMPLE_TX_CHANNEL    (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx

#define AUDIO_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
/* demo audio sample frequency */
#define DEMO_AUDIO_SAMPLING_RATE (kSAI_SampleRate44100Hz)

uint32_t a2dp_snk_sf;
uint8_t a2dp_snk_nc;
static uint32_t currentFreq = 0;
static uint32_t currentBitWidth;
static uint32_t currentChannelNum;
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaTxHandle                                = {0};
static uint32_t masterClockHz                            = 0U;
codec_handle_t codecHandle;
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.mclk_HZ    = 11289600U,
               .sampleRate = kWM8960_AudioSampleRate44100Hz,
               .bitWidth   = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
/*setting for 44.1Khz*/
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 30,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 106,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};
/*setting for multiple of 8Khz,such as 48Khz/16Khz/32KHz*/
const clock_audio_pll_config_t audioPllConfig1 = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

extern void app_audio_streamer_task_signal(void);


/*******************************************************************************
 * Code
 ******************************************************************************/

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
void BOARD_Codec_Init()
{
    CODEC_Init(&codecHandle, &boardCodecConfig);
}

static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    app_audio_streamer_task_signal();
}

void BOARD_Audio_TxInit(uint32_t samplingRate)
{
    sai_transceiver_t config;
    SAI_Init(DEMO_AUDIO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_AUDIO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfigEDMA(DEMO_AUDIO_SAI, &txHandle, &config);
    masterClockHz = DEMO_SAI_CLK_FREQ;
    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, samplingRate, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL);
}
void BOARD_DMA_EDMA_Config()
{
    edma_config_t dmaConfig = {0};
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    NVIC_SetPriority((IRQn_Type)EXAMPLE_DMA_IRQ, AUDIO_INTERRUPT_PRIORITY);
    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
}

void BOARD_DMA_EDMA_Enable_Audio_Interrupts()
{
    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(DEMO_AUDIO_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_IRQ_TX);
}

/*Initialize audio interface and codec.*/
static void Init_Board_Audio(void)
{
    PRINTF("Init Audio SAI and CODEC\r\n");

    BOARD_DMA_EDMA_Config();
    BOARD_Audio_TxInit(DEMO_AUDIO_SAMPLING_RATE);
    BOARD_Codec_Init();
    BOARD_DMA_EDMA_Enable_Audio_Interrupts();
    currentFreq       = 44100;
    currentBitWidth   = 16;
    currentChannelNum = 2;
    (void)currentBitWidth; /*fix build warning: set but never used.*/
}

void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF flag */
    SAI_TxClearStatusFlags(DEMO_AUDIO_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(DEMO_AUDIO_SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

void BOARD_CodecSet(uint32_t sample_rate, uint8_t channels)
{
    a2dp_snk_sf = sample_rate;
    a2dp_snk_nc = channels;
}

void BOARD_CodecStart(void)
{
    if (a2dp_snk_sf == 0U)
    {
        return;
    }

    /*initialize hardware for the first time*/
    if ((0 == currentFreq))
    {
        CLOCK_DeinitAudioPll();
        CLOCK_InitAudioPll(&audioPllConfig);
        /*Clock setting for LPI2C*/
        CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

        /*Clock setting for SAI1*/
        CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

        /*Enable MCLK clock*/
        BOARD_EnableSaiMclkOutput(true);
        Init_Board_Audio();
    }

    if ((a2dp_snk_sf == currentFreq) && (a2dp_snk_nc == currentChannelNum))
    {
        /*nothing to do, default setting is ok*/
        PRINTF("current SAI and CODEC setting is ok\r\n");
    }
    else
    {
        SAI_TransferTerminateSendEDMA(DEMO_AUDIO_SAI, &txHandle);
        /*re-configure sai, codec, if current frequency is not multiple of new setting, audio pll should be
         * re-configure*/

        /*new setting is 44.1khz*/
        if (44100 == a2dp_snk_sf)
        {
            if (kSAI_SampleRate44100Hz == currentFreq)
            {
                SAI_TxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);
                SAI_RxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);

                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
                CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
            }
            else
            {
                PRINTF("switch to 44.1 \r\n");
                masterClockHz = 11289600U;
                /* set bit clock divider */
                SAI_TxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);
                SAI_RxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);

                CLOCK_DeinitAudioPll();
                CLOCK_InitAudioPll(&audioPllConfig);

                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
                CODEC_SetFormat(&codecHandle, masterClockHz, kCODEC_PlayChannelHeadphoneLeft, DEMO_AUDIO_BIT_WIDTH);
                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
            }
        }
        else
        {
            /*new setting is multiple of 8khz*/

            /*current frequency is multiple of 8khz*/
            if (0U == currentFreq % 8000)
            {
                SAI_TxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);
                SAI_RxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);
                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
                CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
            }
            else
            {
                /*current frequency is multiple of 44.1Khz and need change to mutiplu of 8K*/
                masterClockHz = 12288000;
                /* set bit clock divider */
                SAI_TxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);
                SAI_RxSetBitClockRate(DEMO_AUDIO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH, a2dp_snk_nc);

                CLOCK_DeinitAudioPll();
                CLOCK_InitAudioPll(&audioPllConfig1);

                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
                CODEC_SetFormat(&codecHandle, masterClockHz, kCODEC_PlayChannelHeadphoneLeft, DEMO_AUDIO_BIT_WIDTH);
                CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
            }
        }
    }
    CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
}

void BOARD_CodecStop(void)
{
    CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
    currentFreq = 0;
    PRINTF("eStop SAI and CODEC\r\n");
    BOARD_EnableSaiMclkOutput(false);
    SAI_Deinit(DEMO_AUDIO_SAI);
    DisableIRQ(SAI1_IRQn);
}

void BOARD_CodecStreamerPlay(uint8_t *data, uint32_t length)
{
    if ((data != NULL) && (length != 0U))
    {
        sai_transfer_t xfer = {0};
        xfer.dataSize       = length;
        xfer.data           = data;
        if (kStatus_Success != SAI_TransferSendEDMA(DEMO_AUDIO_SAI, &txHandle, &xfer))
        {
            PRINTF("prime fail\r\n");
        }
    }
}


int USB_HostGetConfiguration(usb_host_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->controllerId = kUSB_HostInstanceEhci0;
    return 0;
}

int USB_HostPhyGetConfiguration(usb_host_instance_t instance, usb_host_phy_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->D_CAL     = BOARD_USB_PHY_D_CAL;
    config->TXCAL45DM = BOARD_USB_PHY_TXCAL45DM;
    config->TXCAL45DP = BOARD_USB_PHY_TXCAL45DP;
    return 0;
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    SCB_DisableDCache();

    if (xTaskCreate(app_a2dp_sink_task, "app_a2dp_sink_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("a2dp task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
