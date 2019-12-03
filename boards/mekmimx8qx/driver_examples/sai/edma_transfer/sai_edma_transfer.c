/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "music.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "main/imx8qx_pads.h"
#include "svc/pad/pad_api.h"
#include "fsl_gpio.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI ADMA__SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_I2C CM4__LPI2C
#define DEMO_SAI_CLK_FREQ (24576000U)
#define DEMO_I2C_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_M4_0_Lpi2c)
#define CODEC_CYCLE (30000000)
/* DAM used by SAI */
#define EXAMPLE_DMA ADMA__EDMA0
#define EXAMPLE_CHANNEL 15U

#define OVER_SAMPLE_RATE (384U)
#define BUFFER_SIZE (1600U)
#define BUFFER_NUM (2U)

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bitwidth */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaHandle                                  = {0};
codec_handle_t codecHandle                               = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t buffer[BUFFER_NUM * BUFFER_SIZE], 4);
volatile bool isFinished      = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock  = BUFFER_NUM;
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
/*******************************************************************************
 * Code
 ******************************************************************************/
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
    }
    else
    {
        finishIndex++;
        emptyBlock++;
        /* Judge whether the music array is completely transfered. */
        if (MUSIC_LEN / BUFFER_SIZE == finishIndex)
        {
            isFinished = true;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    edma_config_t dmaConfig = {0};
    uint32_t cpy_index = 0U, tx_index = 0U;
    uint32_t delayCycle = 500000U;
    sai_transceiver_t config;

    sc_ipc_t ipc;
    uint32_t freq;
    /* set mclk 24.576M */
    uint32_t misc_src_rate = 24576000U;
    /* set pll clock,to get accurate mclk freq,it is recommended to set this integer multiple of misc_src_rate,but
     * should in [650MHz-1300MHz] */
    uint32_t pll_src_rate = 1228800000U;

    ipc = BOARD_InitRpc();
    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_I2C_ConfigurePins(ipc);

    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    /* Power on LPI2C. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_M4_0_I2C, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on LPI2C\r\n");
    }
    /* Set LPI2C clock */
    freq = CLOCK_SetIpFreq(kCLOCK_M4_0_Lpi2c, SC_24MHZ);
    if (freq == 0)
    {
        PRINTF("Error: Failed to set LPI2C frequency\r\n");
    }

    /* Power on SAI. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_SAI_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on SAI\r\n");
    }

    /* Power on MCLKOUT0, AUDIO PLL0 clocks */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_AUDIO_PLL_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0\r\n");
    }

    /* Set SC_PM_CLK_PLL clock freq and enable */
    if (sc_pm_set_clock_rate(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_PLL, &pll_src_rate) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set SC_PM_CLK_PLL clock rate\r\n");
    }
    if (sc_pm_clock_enable(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_PLL, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable SC_PM_CLK_PLL clock\r\n");
    }

    /* Set AUDIO PLL0 DIV clock freq and enable */
    if (sc_pm_set_clock_rate(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC0, &misc_src_rate) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set Audio PLL0 DIV clock rate\r\n");
    }
    if (sc_pm_clock_enable(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC0, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0 DIV clock\r\n");
    }

    /* Set AUDIO PLL0 REC clock freq and enable */
    if (sc_pm_set_clock_rate(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC1, &misc_src_rate) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set Audio PLL0 REC clock rate\r\n");
    }
    if (sc_pm_clock_enable(ipc, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC1, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0 REC clock\r\n");
    }

    /* Power on MCLK_OUT clock */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_MCLK_OUT_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MCLKOUT0\r\n");
    }

    /* Power on Local EDMA for M4 Core0. */
    sc_pm_set_resource_power_mode(ipc, SC_R_DMA_0_CH14, SC_PM_PW_MODE_ON);
    sc_pm_set_resource_power_mode(ipc, SC_R_DMA_0_CH15, SC_PM_PW_MODE_ON);

    /* Power on IRQSteer . */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTR\r\n");
    }

    /* Enable interrupt in irqsteer */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, ADMA_SAI1_DMA_INT_IRQn);
    BOARD_Codec_I2C_Init();

    PRINTF("SAI example started!\n\r");

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

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
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
    CODEC_SetFormat(&codecHandle, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH);

#if defined(DEMO_CODEC_WM8524)
    wm8524_config_t codecConfig = {0};
    codecConfig.busPinNum       = CODEC_BUS_PIN_NUM;
    codecConfig.busPin          = CODEC_BUS_PIN;
    codecConfig.mutePin         = CODEC_MUTE_PIN;
    codecConfig.mutePinNum      = CODEC_MUTE_PIN_NUM;
    codecConfig.protocol        = kWM8524_ProtocolI2S;
    WM8524_Init(&codecHandle, &codecConfig);
#endif

/* If need to handle audio error, enable sai interrupt */
#if defined(DEMO_SAI_IRQ)
    EnableIRQ(DEMO_SAI_IRQ);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
#endif

#if defined(CODEC_CYCLE)
    delayCycle = CODEC_CYCLE;
#endif
    while (delayCycle)
    {
        __ASM("nop");
        delayCycle--;
    }

    /* Waiting until finished. */
    while (!isFinished)
    {
        if ((emptyBlock > 0U) && (cpy_index < MUSIC_LEN / BUFFER_SIZE))
        {
            /* Fill in the buffers. */
            memcpy((uint8_t *)&buffer[BUFFER_SIZE * (cpy_index % BUFFER_NUM)],
                   (uint8_t *)&music[cpy_index * BUFFER_SIZE], sizeof(uint8_t) * BUFFER_SIZE);
            emptyBlock--;
            cpy_index++;
        }
        if (emptyBlock < BUFFER_NUM)
        {
            /*  xfer structure */
            xfer.data     = (uint8_t *)&buffer[BUFFER_SIZE * (tx_index % BUFFER_NUM)];
            xfer.dataSize = BUFFER_SIZE;
            /* Wait for available queue. */
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
        }
    }

    /* Once transfer finish, disable SAI instance. */
    SAI_TransferAbortSendEDMA(DEMO_SAI, &txHandle);
    SAI_Deinit(DEMO_SAI);
    PRINTF("\n\r SAI EDMA example finished!\n\r ");
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
