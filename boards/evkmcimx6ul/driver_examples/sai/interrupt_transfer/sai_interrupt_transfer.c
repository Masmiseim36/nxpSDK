/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "music.h"
#include "fsl_sai.h"
#include "fsl_debug_console.h"

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI I2S2
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_I2C I2C2
#define DEMO_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / ((CLOCK_GetDiv(kCLOCK_Sai2PreDiv) + 1U) * (CLOCK_GetDiv(kCLOCK_Sai2Div) + 1U)))
#define DEMO_I2C_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U))
#define DEMO_SAI_IRQ SAI2_IRQn

#define I2C_RELEASE_SDA_GPIO GPIO1
#define I2C_RELEASE_SDA_PIN 31U
#define I2C_RELEASE_SCL_GPIO GPIO1
#define I2C_RELEASE_SCL_PIN 30U
#define I2C_RELEASE_BUS_COUNT 100U
#define OVER_SAMPLE_RATE (384U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void BOARD_EnableSaiMclkOutput(bool enable);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const clock_audio_pll_config_t g_ccmConfigAudioPll = {
    .loopDivider = 30, .postDivider = 2, .numerator = 72, .denominator = 100};

sai_handle_t txHandle = {0};
static volatile bool isFinished = false;
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#elif defined (DEMO_CODEC_DA7212)
da7212_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK);
    }
}

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

    pin_config.direction = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    pin_config.interruptMode = kGPIO_NoIntmode;

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}
static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_config_t config;
    uint32_t mclkSourceClockHz = 0U;
    sai_transfer_format_t format;
    sai_transfer_t xfer;
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    uint32_t temp = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_EnableSaiMclkOutput(true);
    BOARD_InitDebugConsole();

    /* Enable PLL4 368.64M */
    CLOCK_InitAudioPll(&g_ccmConfigAudioPll);
    /* Set SAI source to PLL4*/
    CLOCK_SetMux(kCLOCK_Sai2Mux, 2);
    /* Set SAI pre-divider to 6 */
    CLOCK_SetDiv(kCLOCK_Sai2PreDiv, 5U);
    /* Set SAI divider to 10 */
    CLOCK_SetDiv(kCLOCK_Sai2Div, 9U);

    /* Install IRQ Handler */
    SystemInitIrqTable();

    PRINTF("SAI example started!\n\r");

    /*
     * config.masterSlave = kSAI_Master;
     * config.mclkSource = kSAI_MclkSourceSysclk;
     * config.protocol = kSAI_BusLeftJustified;
     * config.syncMode = kSAI_ModeAsync;
     * config.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&config);
    SAI_TxInit(DEMO_SAI, &config);

    /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    format.sampleRate_Hz = kSAI_SampleRate16KHz;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /* Configure Codec I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = DEMO_I2C_CLK_FREQ;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
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
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#endif

#if defined(DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#elif defined (DEMO_CODEC_DA7212)
    DA7212_Init(&codecHandle, NULL);
    DA7212_ConfigAudioFormat(&codecHandle, format.sampleRate_Hz, format.masterClockHz, format.bitWidth);
    DA7212_ChangeOutput(&codecHandle, kDA7212_Output_HP);
#else
    /* Use default settings for sgtl5000 */
    SGTL_Init(&codecHandle, NULL);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#endif
    SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, callback, NULL);
    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormat(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);

    /*  xfer structure */
    temp = (uint32_t)music;
    xfer.data = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
    /* Wait until finished */
    while (isFinished != true)
    {
    }

    PRINTF("\n\r SAI example finished!\n\r ");
    while (1)
    {
    }
}
