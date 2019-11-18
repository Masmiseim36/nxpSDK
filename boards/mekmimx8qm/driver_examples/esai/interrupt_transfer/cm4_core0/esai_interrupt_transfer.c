/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_esai.h"
#include "fsl_lpi2c.h"
#include "fsl_cs42888.h"
#include "pin_mux.h"
#include "fsl_irqsteer.h"
#include "main/imx8qm_pads.h"
#include "svc/pad/pad_api.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C (CM4_1__LPI2C)
#define I2C_SOURCE_CLOCK_FREQ CLOCK_GetIpFreq(kCLOCK_M4_1_Lpi2c)
#define DEMO_ESAI AUDIO__ESAI0
#define ESAI_SOURCE_CLOCK_FREQ (24576000)
#define AUDIO_IRQHandler IRQSTEER_6_IRQHandler
#define CODEC_CS42888 (1)

#define CODEC_RST_GPIO LSIO__GPIO4 /* SC_P_QSPI1A_DATA1, LSIO.GPIO4.IO25 */
#define CODEC_RST_PIN 25
#define OVER_SAMPLE_RATE (256U)
#define SAMPLE_RATE (kESAI_SampleRate48KHz)
#define BUFFER_SIZE (1024)
#define BUFFER_NUM (4)
#define PLAY_COUNT (5000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_CodecReset(bool state);
void BOARD_Delay(uint32_t ms);
void BOARD_CS42888_I2C_Init(void *handle);
status_t BOARD_CS42888_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_CS42888_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
static void txCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData);
static void rxCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
esai_handle_t txHandle = {0};
esai_handle_t rxHandle = {0};
static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
cs42888_handle_t codecHandle = {0}; /* Codec handler */
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif
volatile bool istxFinished             = false;
volatile bool isrxFinished             = false;
volatile uint32_t beginCount           = 0;
volatile uint32_t sendCount            = 0;
volatile uint32_t receiveCount         = 0;
static cs42888_config_t cs42888_config = {
    .DACMode = kCS42888_ModeSlave,
    .ADCMode = kCS42888_ModeSlave,
    .bus     = kCS42888_BusI2S,
    .reset   = BOARD_CodecReset,
};
codec_config_t codecConfig = {.I2C_SendFunc    = BOARD_CS42888_I2C_Send,
                              .I2C_ReceiveFunc = BOARD_CS42888_I2C_Receive,
                              .delayMs         = BOARD_Delay,
                              .op.Init         = CS42888_Init,
                              .op.Deinit       = CS42888_Deinit,
                              .op.SetFormat    = CS42888_ConfigDataFormat,
                              .codecConfig     = &cs42888_config};
volatile uint32_t g_systickCounter;
/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_CS42888_I2C_Init(void *handle)
{
    uint32_t i2cSourceClock = I2C_SOURCE_CLOCK_FREQ;

    lpi2c_master_config_t i2cConfig = {0};

    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(EXAMPLE_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(EXAMPLE_I2C, (lpi2c_master_handle_t *)handle, NULL, NULL);
}

status_t BOARD_CS42888_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(EXAMPLE_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_CS42888_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(EXAMPLE_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_CodecReset(bool state)
{
    GPIO_PinWrite(CODEC_RST_GPIO, CODEC_RST_PIN, state);
}

extern volatile uint32_t g_systickCounter;

void BOARD_Delay(uint32_t ms)
{
    g_systickCounter = ms;
    while (g_systickCounter != 0U)
    {
    }
}

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}
static void txCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    sendCount++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        ESAI_TransferAbortSend(base, handle);
        sendCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((sendCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferSendNonBlocking(base, handle, &xfer);
    }
}

static void rxCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    receiveCount++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        ESAI_TransferAbortReceive(base, handle);
        receiveCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((receiveCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferReceiveNonBlocking(base, handle, &xfer);
    }
}

int main(void)
{
    esai_config_t config;
    uint32_t hclkSourceClockHz = 0U;
    esai_transfer_t txfer;
    esai_transfer_t rxfer;
    esai_format_t format;

    uint32_t freq     = ESAI_SOURCE_CLOCK_FREQ;
    uint32_t mst_freq = 1228800000;
    sc_ipc_t ipcHandle;
    gpio_pin_config_t pin_config = {kGPIO_DigitalOutput, 1U, kGPIO_NoIntmode};

    ipcHandle = BOARD_InitRpc();

    BOARD_InitPins(ipcHandle);
    BOARD_BootClockRUN();
    BOARD_I2C_ConfigurePins(ipcHandle);
    BOARD_ESAI_ConfigurePins(ipcHandle);
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power the I2C module */
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_M4_1_I2C, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable lpi2c");
    }
    /* Set LPI2C clock */
    if (CLOCK_SetIpFreq(kCLOCK_M4_1_Lpi2c, SC_133MHZ) == 0)
    {
        PRINTF("Error: Failed to set LPI2C frequency\r\n");
    }

    /* Power on ESAI and clocks */
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_ESAI_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable ESAI0\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0\r\n");
    }
    if (sc_pm_set_clock_rate(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_CLK_PLL, &mst_freq) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set Audio PLL 0 master frequency\r\n");
    }
    if (sc_pm_set_clock_rate(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC0, &freq) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set Audio PLL 0 DIV frequency\r\n");
    }
    if (sc_pm_clock_enable(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC0, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0 DIV clock\r\n");
    }
    if (sc_pm_set_clock_rate(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC1, &freq) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set Audio PLL 0 frequency\r\n");
    }
    if (sc_pm_clock_enable(ipcHandle, SC_R_AUDIO_PLL_0, SC_PM_CLK_MISC1, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable Audio PLL0 REC clock\r\n");
    }

    /* Configure ACM(Audio Clock Mux), set ESAI master clock source to audio pll0 div */
    uint32_t *mux = (uint32_t *)0x59E60000; /* Configure ACM_ESAI0_MCLK_CTL register */
    *mux          = 0;

    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }

    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, AUDIO_ESAI0_INT_IRQn);

    /* Power on GPIO. */
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_GPIO_4, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on GPIO4\r\n");
    }
    /* The AUD_RST_B comes from BB_PER_RST_B AND logic with BB_AUDIN_RST_B.*/
    GPIO_PinInit(CODEC_RST_GPIO, CODEC_RST_PIN, &pin_config);
    NVIC_EnableIRQ(SysTick_IRQn);
    /* system tick configured as a 1ms timer */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    PRINTF("\r\nESAI Interrupt example started! \n\r");

    BOARD_CS42888_I2C_Init(&i2cHandle);
    /* Init codec */
    if (CODEC_Init(&codecHandle, &codecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        return -1;
    }

    if (CODEC_SetFormat(&codecHandle, SAMPLE_RATE * 256U, SAMPLE_RATE, 16) != kStatus_Success)
    {
        PRINTF("CODEC_SetFormat failed!\r\n");
        return -1;
    }

    /* Initialize ESAI module */
    ESAI_GetDefaultConfig(&config);
#if defined CODEC_CS42888
    /* CS42888 have seperate clock source for Tx and Rx, shall not use sync mode */
    config.syncMode = kESAI_ModeAsync;
#endif
    ESAI_Init(DEMO_ESAI, &config);

    /* Configure the audio format */
    format.slotType      = kESAI_SlotLen32WordLen16;
    format.sampleRate_Hz = SAMPLE_RATE;
    format.sectionMap    = 0x1;

    ESAI_TransferTxCreateHandle(DEMO_ESAI, &txHandle, txCallback, NULL);
    ESAI_TransferRxCreateHandle(DEMO_ESAI, &rxHandle, rxCallback, NULL);

    hclkSourceClockHz = ESAI_SOURCE_CLOCK_FREQ;
#if defined ESAI_TX_CHANNEL
    format.sectionMap = (1U << ESAI_TX_CHANNEL);
#endif
    ESAI_TransferTxSetFormat(DEMO_ESAI, &txHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);
#if defined ESAI_RX_CHANNEL
    format.sectionMap = (1U << ESAI_RX_CHANNEL);
#else
    format.sectionMap = 1U;
#endif
    ESAI_TransferRxSetFormat(DEMO_ESAI, &rxHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);

    /*  xfer structure */
    rxfer.data     = audioBuff;
    rxfer.dataSize = BUFFER_SIZE;
    txfer.data     = audioBuff;
    txfer.dataSize = BUFFER_SIZE;

    /* Set the time to record and playback */
    beginCount = PLAY_COUNT;

    ESAI_TransferReceiveNonBlocking(DEMO_ESAI, &rxHandle, &rxfer);
    ESAI_TransferSendNonBlocking(DEMO_ESAI, &txHandle, &txfer);

    /* Waiting for transfer finished */
    while ((isrxFinished == false) || (istxFinished == false))
    {
    }

    ESAI_TransferAbortReceive(DEMO_ESAI, &rxHandle);
    ESAI_TransferAbortSend(DEMO_ESAI, &txHandle);
    PRINTF("\r\nESAI Interrupt example Succeed! \n\r");

    while (1)
    {
    }
}

void AUDIO_IRQHandler(void)
{
    /* Handle Rx operation */
    if ((ESAI_GetStatusFlag(DEMO_ESAI) & kESAI_ReceiveFIFOFullFlag) && (DEMO_ESAI->RCR & kESAI_TransmitInterruptEnable))
    {
        ESAI_TransferRxHandleIRQ(DEMO_ESAI, &rxHandle);
    }

    /* Handle Tx operation */
    if ((ESAI_GetStatusFlag(DEMO_ESAI) & kESAI_TransmitFIFOEmptyFlag) &&
        (DEMO_ESAI->TCR & kESAI_TransmitInterruptEnable))
    {
        ESAI_TransferTxHandleIRQ(DEMO_ESAI, &txHandle);
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
