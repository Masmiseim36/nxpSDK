/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_audio.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "audio_speaker.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USING_I2S)
#include "fsl_i2s.h"
#elif defined(USING_SAI)
#include "fsl_sai.h"
#endif

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI I2S2
#define EXAMPLE_CHANNEL (0)
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

#define AUDIO_DATA_WHOLE_BUFFER_LENGTH (32)
#define OVER_SAMPLE_RATE (384U)
/* USB clock source and frequency*/
#define USB_HS_PHY_CLK_SRC kCLOCK_Usbphy480M
#define USB_HS_PHY_CLK_FREQ 480000000U
#define USB_HS_CLK_SRC kCLOCK_Usb480M
#define USB_HS_CLK_FREQ 480000000U
#define BOARD_XTAL0_CLK_HZ 0
#define AUDIO_DATA_THRESHOLD (FS_ISO_OUT_ENDP_PACKET_SIZE * 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void BOARD_EnableSaiMclkOutput(bool enable);
void APPInit(void);
usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand, uint32_t *length, uint8_t **buffer);
void BOARD_InitHardware(void);
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
extern void BOARD_LPI2C_Init(uint32_t boardI2CClock, LPI2C_Type *LPI2CBase);
extern void BOARD_Codec_Init(LPI2C_Type *LPI2CBase);
#else
extern void BOARD_I2C_Init(uint32_t boardI2CClock, I2C_Type *I2CBase);
extern void BOARD_Codec_Init(I2C_Type *I2CBase);
#endif

extern void BOARD_SAI_TxInit(I2S_Type *SAIBase, uint32_t saiClockFreq);

extern void BOARD_SAI_TransferTxSetFormat(I2S_Type *SAIBase, uint32_t saiClockFreq);
#if defined(USING_I2S)
extern void BOARD_I2S_Enable(I2S_Type *SAIBase);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
const clock_audio_pll_config_t g_ccmConfigAudioPll = {
    .loopDivider = 30, .postDivider = 2, .numerator = 72, .denominator = 100};

USB_DATA_ALIGNMENT USB_RAM_ADDRESS_NONCACHEREG(
    uint8_t audioDataBuff[AUDIO_DATA_WHOLE_BUFFER_LENGTH][FS_ISO_OUT_ENDP_PACKET_SIZE * 2]);
uint8_t audioDataBuff[AUDIO_DATA_WHOLE_BUFFER_LENGTH][FS_ISO_OUT_ENDP_PACKET_SIZE * 2] = {{0}};
uint8_t audioBuffLength[AUDIO_DATA_WHOLE_BUFFER_LENGTH] = {0};
/*uint8_t audioFeedBackBuffer[3] = {0xF0U, 0x00U, 0x04U}; //noise comes every time the recv buffer exceeds the send
 * buffer */
USB_DATA_ALIGNMENT USB_RAM_ADDRESS_NONCACHEREG(uint8_t audioFeedBackBuffer[3]);
uint8_t audioFeedBackBuffer[3] = {0x00U, 0x00U, 0x04U};
volatile uint32_t startSai = 0;
volatile uint32_t audioTempBuffer;
uint16_t epPacketSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
volatile uint32_t sendCount = 0;
volatile uint32_t recvCount = 0;
extern uint8_t
    g_UsbDeviceInterface[USB_AUDIO_SPEAKER_INTERFACE_COUNT]; /* Default value of audio speaker device struct */
volatile int32_t prevSendCountInterval = ((AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2) * (FS_ISO_OUT_ENDP_PACKET_SIZE));
volatile uint32_t tdReadNumber = 0;
volatile uint32_t tdWriteNumber = 0;
volatile uint32_t dataSendCountInterval = 0;
volatile uint32_t dataSend = 0;
volatile bool isFinished = false;
volatile int32_t prevSendCount =
    -(AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2 * (FS_ISO_OUT_ENDP_PACKET_SIZE / (DEMO_SAI_BITWIDTH / 8U)));

usb_audio_speaker_struct_t s_audioSpeaker = {
    .deviceHandle = NULL,
#if ((defined(USB_DEVICE_CONFIG_EHCI)) && (USB_DEVICE_CONFIG_EHCI > 0U))
    .currentMaxPacketSize = FS_ISO_OUT_ENDP_PACKET_SIZE,
#endif
    .currentStreamInterfaceAlternateSetting = 0U,
    .copyProtect = 0x01U,
    .curMute = 0x01U,
    .curVolume = {0x00U, 0x80U},
    .minVolume = {0x00U, 0x80U},
    .maxVolume = {0xFFU, 0x7FU},
    .resVolume = {0x01U, 0x00U},
    .curBass = 0x00U,
    .minBass = 0x80U,
    .maxBass = 0x7FU,
    .resBass = 0x01U,
    .curMid = 0x00U,
    .minMid = 0x80U,
    .maxMid = 0x7FU,
    .resMid = 0x01U,
    .curTreble = 0x01U,
    .minTreble = 0x80U,
    .maxTreble = 0x7FU,
    .resTreble = 0x01U,
    .curAutomaticGain = 0x01U,
    .curDelay = {0x00U, 0x40U},
    .minDelay = {0x00U, 0x00U},
    .maxDelay = {0xFFU, 0xFFU},
    .resDelay = {0x00U, 0x01U},
    .curLoudness = 0x01U,
    .curSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .minSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .maxSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .resSamplingFrequency = {0x00U, 0x00U, 0x01U},
#if USBCFG_AUDIO_CLASS_2_0
    .curSampleFrequency = 16000U, /* This should be changed to 48000 if sampling rate is 48k */
    .curClockValid = 1U,
    .controlRange = {1U, 16000U, 16000U, 0U},
#endif
    .speed = USB_SPEED_FULL,
    .attach = 0U,
};
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
#if defined(USING_I2S)
static inline void i2s_Send(I2S_Type *i2s, uint32_t data)
{
    i2s->FIFOWR = data;
}

static void I2S_WriteData(I2S_Type *base)
{
    uint32_t intstat;
    uint32_t data = 0;
    uint8_t j = 0;
    uint32_t temp = 0;

    intstat = base->FIFOINTSTAT; /* get FIFO pending interrupt status */
    if (intstat & I2S_FIFOINTSTAT_TXLVL_MASK)
    {
        if (startSai) // if there's valid data...
        {
            for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
            {
                temp = (uint32_t)(audioDataBuff[tdWriteNumber][dataSend]);
                data |= (temp << (8U * j));
                dataSend++;
            }

            if (dataSend >= audioBuffLength[tdWriteNumber])
            {
                dataSend = 0;
                dataSendCountInterval += audioBuffLength[tdWriteNumber];
                tdWriteNumber++;
                sendCount++;
                if (sendCount >= recvCount)
                {
                    startSai = 0;
                }
                if (tdWriteNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH)
                {
                    tdWriteNumber = 0;
                }
            }
        }
        else
        {
            data = 0; // set data to silence
        }
        i2s_Send(base, data); // send the data (or silence)
    }
}

void I2S_ErrorHandler(I2S_Type *base)
{
    uint32_t intstat;

    intstat = base->FIFOINTSTAT;              /* get FIFO pending interrupt status */
    if (intstat & I2S_FIFOINTSTAT_TXERR_MASK) /* process tx error */
    {
        /* Clear TX error interrupt flag */
        base->FIFOSTAT = I2S_FIFOSTAT_TXERR(1);
    }
}

void I2S_TxIRQHandler(void)
{
    I2S_ErrorHandler(DEMO_SAI); //	Accumulate TX errors then, clear status
    I2S_WriteData(DEMO_SAI);    // read data from I2S, write to USB
}
#elif defined(USING_SAI)
void SAI_TxIRQHandler(void)
{
    uint32_t data = 0;
    uint8_t j = 0;
    uint32_t temp = 0;

    /* Clear the FIFO error flag */
    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    }

    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOWarningFlag)
    {
        if (startSai)
        {
            for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
            {
                temp = (uint32_t)(audioDataBuff[tdWriteNumber][dataSend]);
                data |= (temp << (8U * j));
                dataSend++;
            }
            if (dataSend >= audioBuffLength[tdWriteNumber])
            {
                dataSend = 0;
                dataSendCountInterval += audioBuffLength[tdWriteNumber];
                tdWriteNumber++;
                sendCount++;
                if (sendCount >= recvCount)
                {
                    startSai = 0;
                }
                if (tdWriteNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH)
                {
                    tdWriteNumber = 0;
                }
            }
        }
        else
        {
            data = 0;
        }
        SAI_WriteData(DEMO_SAI, EXAMPLE_CHANNEL, data);
    }
}
#endif

/* Initialize the structure information for sai. */
void Init_Board_Sai_Codec(void)
{
    uint32_t saiMasterClockHz;
    usb_echo("Init Audio SAI and CODEC\r\n");

/* Init SAI module */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    saiMasterClockHz = OVER_SAMPLE_RATE * kSAI_SampleRate16KHz;
#else
    saiMasterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    BOARD_SAI_TxInit(DEMO_SAI, saiMasterClockHz);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    BOARD_LPI2C_Init(DEMO_I2C_CLK_FREQ, DEMO_I2C);
#else
    BOARD_I2C_Init(DEMO_I2C_CLK_FREQ, DEMO_I2C);
#endif

    BOARD_Codec_Init(DEMO_I2C);

#if defined(USING_I2S)
    BOARD_I2S_Enable(DEMO_SAI);
#elif defined(USING_SAI)
    BOARD_SAI_TransferTxSetFormat(DEMO_SAI, DEMO_SAI_CLK_FREQ);
    /*  Enable interrupt */
    EnableIRQ(DEMO_SAI_IRQ);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
    SAI_TxEnable(DEMO_SAI, true);
#endif
}

usb_status_t USB_DeviceAudioGetControlTerminal(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint32_t audioCommand = 0U;

    switch (setup->bRequest)
    {
        /* Copy Protect Control only supports the CUR attribute!*/
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioGetCurAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_MUTE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_AUTOMATIC_GAIN_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_BOOST_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_BASS_BOOST_CONTROL;
            break;
        case USB_DEVICE_AUDIO_LOUDNESS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_LOUDNESS_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);

    return error;
}

usb_status_t USB_DeviceAudioGetMinAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioGetMaxAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioGetResAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);

    return error;
}

usb_status_t USB_DeviceAudioSetCurAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;

    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_MUTE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_AUTOMATIC_GAIN_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_BOOST_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_BASS_BOOST_CONTROL;
            break;
        case USB_DEVICE_AUDIO_LOUDNESS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_LOUDNESS_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioSetMinAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);

    return error;
}

usb_status_t USB_DeviceAudioSetMaxAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);

    return error;
}

usb_status_t USB_DeviceAudioSetResAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioGetFeatureUnit(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            error = USB_DeviceAudioGetCurAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_REQUEST:
            error = USB_DeviceAudioGetMinAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_REQUEST:
            error = USB_DeviceAudioGetMaxAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_REQUEST:
            error = USB_DeviceAudioGetResAudioFeatureUnit(handle, setup, length, buffer);
            break;
        default:
            break;
    }
    return error;
}

usb_status_t USB_DeviceAudioSetFeatureUnit(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:
            error = USB_DeviceAudioSetCurAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_REQUEST:
            error = USB_DeviceAudioSetMinAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_REQUEST:
            error = USB_DeviceAudioSetMaxAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_REQUEST:
            error = USB_DeviceAudioSetResAudioFeatureUnit(handle, setup, length, buffer);
            break;
        default:
            break;
    }
    return error;
}

usb_status_t USB_DeviceAudioSetControlTerminal(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint32_t audioCommand = 0U;

    switch (setup->bRequest)
    {
        /* Copy Protect Control only supports the CUR attribute!*/
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:

            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioSetRequestInterface(usb_device_handle handle,
                                                usb_setup_struct_t *setup,
                                                uint32_t *length,
                                                uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t entity_id = (uint8_t)(setup->wIndex >> 0x08);

    if (USB_AUDIO_CONTROL_OUTPUT_TERMINAL_ID == entity_id)
    {
        error = USB_DeviceAudioSetControlTerminal(handle, setup, length, buffer);
    }
    else if (USB_AUDIO_CONTROL_FEATURE_UNIT_ID == entity_id)
    {
        error = USB_DeviceAudioSetFeatureUnit(handle, setup, length, buffer);
    }
    else
    {
    }

    return error;
}

usb_status_t USB_DeviceAudioGetRequestInterface(usb_device_handle handle,
                                                usb_setup_struct_t *setup,
                                                uint32_t *length,
                                                uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t entity_id = (uint8_t)(setup->wIndex >> 0x08);
    if (USB_AUDIO_CONTROL_INPUT_TERMINAL_ID == entity_id)
    {
        error = USB_DeviceAudioGetControlTerminal(handle, setup, length, buffer);
    }
    else if (USB_AUDIO_CONTROL_FEATURE_UNIT_ID == entity_id)
    {
        error = USB_DeviceAudioGetFeatureUnit(handle, setup, length, buffer);
    }
    else
    {
    }
    return error;
}

usb_status_t USB_DeviceAudioSetRequestEndpoint(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;

    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL;
                    break;
                case USB_DEVICE_AUDIO_PITCH_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_CUR_PITCH_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceAudioGetRequestEndpoint(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand = 0U;
    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL;

                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);
    return error;
}

usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
#if USBCFG_AUDIO_CLASS_2_0
    /* Handle the audio class specific request. */
    uint8_t entityId = (uint8_t)(setup->wIndex >> 0x08);
    uint32_t audioCommand = 0;

    switch (entityId)
    {
        case USB_AUDIO_CONTROL_OUTPUT_TERMINAL_ID:
            break;
        case USB_AUDIO_CONTROL_FEATURE_UNIT_ID:
            break;
        case USB_AUDIO_CONTROL_CLOCK_SOURCE_UNIT_ID:
            if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_IN))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_CS_SAM_FREQ_CONTROL:
                        if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_CUR)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL;
                        }
                        else if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_RANGE)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL;
                        }
                        else
                        {
                        }
                        break;
                    case USB_DEVICE_AUDIO_CS_CLOCK_VALID_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL;
                        break;
                    default:
                        break;
                }
            }
            else if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_OUT))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_CS_SAM_FREQ_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL;
                        break;
                    case USB_DEVICE_AUDIO_CS_CLOCK_VALID_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL;
                        break;
                    default:
                        break;
                }
            }
            else
            {
            }
            break;
        default:
            break;
    }

    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer);

#else

    switch (setup->bmRequestType)
    {
        case USB_DEVICE_AUDIO_SET_REQUEST_INTERFACE:
            error = USB_DeviceAudioSetRequestInterface(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_REQUEST_INTERFACE:
            error = USB_DeviceAudioGetRequestInterface(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_REQUEST_ENDPOINT:
            error = USB_DeviceAudioSetRequestEndpoint(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_REQUEST_ENDPOINT:
            error = USB_DeviceAudioGetRequestEndpoint(handle, setup, length, buffer);
            break;
        default:
            break;
    }
#endif

    return error;
}

usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand, uint32_t *length, uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_Success;
    static uint16_t volume = 0;

    switch (audioCommand)
    {
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL:
            *buffer = &s_audioSpeaker.curMute;
            *length = sizeof(s_audioSpeaker.curMute);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL:
            *buffer = s_audioSpeaker.curVolume;
            *length = sizeof(s_audioSpeaker.curVolume);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL:
            *buffer = &s_audioSpeaker.curBass;
            *length = sizeof(s_audioSpeaker.curBass);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL:
            *buffer = &s_audioSpeaker.curMid;
            *length = sizeof(s_audioSpeaker.curMid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL:
            *buffer = &s_audioSpeaker.curTreble;
            *length = sizeof(s_audioSpeaker.curTreble);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL:
            *buffer = &s_audioSpeaker.curAutomaticGain;
            *length = sizeof(s_audioSpeaker.curAutomaticGain);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL:
            *buffer = s_audioSpeaker.curDelay;
            *length = sizeof(s_audioSpeaker.curDelay);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioSpeaker.curSamplingFrequency;
            *length = sizeof(s_audioSpeaker.curSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL:
            *buffer = s_audioSpeaker.minVolume;
            *length = sizeof(s_audioSpeaker.minVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL:
            *buffer = &s_audioSpeaker.minBass;
            *length = sizeof(s_audioSpeaker.minBass);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL:
            *buffer = &s_audioSpeaker.minMid;
            *length = sizeof(s_audioSpeaker.minMid);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL:
            *buffer = &s_audioSpeaker.minTreble;
            *length = sizeof(s_audioSpeaker.minTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL:
            *buffer = s_audioSpeaker.minDelay;
            *length = sizeof(s_audioSpeaker.minDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioSpeaker.minSamplingFrequency;
            *length = sizeof(s_audioSpeaker.minSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL:
            *buffer = s_audioSpeaker.maxVolume;
            *length = sizeof(s_audioSpeaker.maxVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL:
            *buffer = &s_audioSpeaker.maxBass;
            *length = sizeof(s_audioSpeaker.maxBass);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL:
            *buffer = &s_audioSpeaker.maxMid;
            *length = sizeof(s_audioSpeaker.maxMid);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL:
            *buffer = &s_audioSpeaker.maxTreble;
            *length = sizeof(s_audioSpeaker.maxTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL:
            *buffer = s_audioSpeaker.maxDelay;
            *length = sizeof(s_audioSpeaker.maxDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioSpeaker.maxSamplingFrequency;
            *length = sizeof(s_audioSpeaker.maxSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL:
            *buffer = s_audioSpeaker.resVolume;
            *length = sizeof(s_audioSpeaker.resVolume);
            break;
        case USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL:
            *buffer = &s_audioSpeaker.resBass;
            *length = sizeof(s_audioSpeaker.resBass);
            break;
        case USB_DEVICE_AUDIO_GET_RES_MID_CONTROL:
            *buffer = &s_audioSpeaker.resMid;
            *length = sizeof(s_audioSpeaker.resMid);
            break;
        case USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL:
            *buffer = &s_audioSpeaker.resTreble;
            *length = sizeof(s_audioSpeaker.resTreble);
            break;
        case USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL:
            *buffer = s_audioSpeaker.resDelay;
            *length = sizeof(s_audioSpeaker.resDelay);
            break;
        case USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioSpeaker.resSamplingFrequency;
            *length = sizeof(s_audioSpeaker.resSamplingFrequency);
            break;
#if USBCFG_AUDIO_CLASS_2_0
        case USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL:
            *buffer = (uint8_t *)&s_audioSpeaker.curSampleFrequency;
            *length = sizeof(s_audioSpeaker.curSampleFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL:
            *buffer = (uint8_t *)&s_audioSpeaker.controlRange;
            *length = sizeof(s_audioSpeaker.controlRange);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL:
            *buffer = &s_audioSpeaker.curClockValid;
            *length = sizeof(s_audioSpeaker.curClockValid);
            break;
#endif

        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL:
            s_audioSpeaker.curVolume[0] = **(buffer);
            s_audioSpeaker.curVolume[1] = **(buffer + 1);
            volume = (uint16_t)((uint16_t)s_audioSpeaker.curVolume[1] << 8U);
            volume |= (uint8_t)(s_audioSpeaker.curVolume[0]);
            /* If needs print information while adjusting the volume, please enable the following sentence. */
            /* usb_echo("Set Cur Volume : %x\r\n", volume); */
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL:
            s_audioSpeaker.curMute = **(buffer);
            /* If needs print information while adjusting the volume, please enable the following sentence. */
            /* usb_echo("Set Cur Mute : %x\r\n", s_audioSpeaker.curMute); */
            break;
        case USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL:
            s_audioSpeaker.curBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL:
            s_audioSpeaker.curMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL:
            s_audioSpeaker.curTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL:
            s_audioSpeaker.curAutomaticGain = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL:
            s_audioSpeaker.curDelay[0] = **(buffer);
            s_audioSpeaker.curDelay[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL:
            s_audioSpeaker.curSamplingFrequency[0] = **(buffer);
            s_audioSpeaker.curSamplingFrequency[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL:
            s_audioSpeaker.minVolume[0] = **(buffer);
            s_audioSpeaker.minVolume[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL:
            s_audioSpeaker.minBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL:
            s_audioSpeaker.minMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL:
            s_audioSpeaker.minTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL:
            s_audioSpeaker.minDelay[0] = **(buffer);
            s_audioSpeaker.minDelay[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL:
            s_audioSpeaker.minSamplingFrequency[0] = **(buffer);
            s_audioSpeaker.minSamplingFrequency[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL:
            s_audioSpeaker.maxVolume[0] = **(buffer);
            s_audioSpeaker.maxVolume[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL:
            s_audioSpeaker.maxBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL:
            s_audioSpeaker.maxMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL:
            s_audioSpeaker.maxTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL:
            s_audioSpeaker.maxDelay[0] = **(buffer);
            s_audioSpeaker.maxDelay[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL:
            s_audioSpeaker.maxSamplingFrequency[0] = **(buffer);
            s_audioSpeaker.maxSamplingFrequency[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL:
            s_audioSpeaker.resVolume[0] = **(buffer);
            s_audioSpeaker.resVolume[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL:
            s_audioSpeaker.resBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_MID_CONTROL:
            s_audioSpeaker.resMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL:
            s_audioSpeaker.resTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL:
            s_audioSpeaker.resDelay[0] = **(buffer);
            s_audioSpeaker.resDelay[1] = **(buffer + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL:
            s_audioSpeaker.resSamplingFrequency[0] = **(buffer);
            s_audioSpeaker.resSamplingFrequency[1] = **(buffer + 1);
            break;
#if USBCFG_AUDIO_CLASS_2_0
        case USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL:
            s_audioSpeaker.curSampleFrequency = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL:
            s_audioSpeaker.curClockValid = **(buffer);
            break;

#endif
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

void USB_SplitFeedbackData(uint32_t feedbackData)
{
    audioFeedBackBuffer[0] = (uint8_t)feedbackData;
    audioFeedBackBuffer[1] = (uint8_t)(feedbackData >> 8U);
    audioFeedBackBuffer[2] = (uint8_t)(feedbackData >> 16U);
}

void USB_AudioDataMatch(uint32_t length)
{
    static uint32_t intervalRecvCount = 0;
    static int32_t sendRecvDiff = 0;
    static int32_t dataReadCountInterval = 0;
    static int32_t threshold = AUDIO_DATA_THRESHOLD;
    static uint32_t feedBackData = 0x40000U;

    intervalRecvCount++;
    dataReadCountInterval += length;
    if (intervalRecvCount != 1000)
    {
        return;
    }
    intervalRecvCount = 0;
    if (((dataSendCountInterval + prevSendCountInterval) > dataReadCountInterval + 3200) ||
        ((dataSendCountInterval + prevSendCountInterval) < dataReadCountInterval - 3200))
    {
        dataSendCountInterval = dataReadCountInterval;
    }

    sendRecvDiff += dataSendCountInterval - dataReadCountInterval + prevSendCountInterval;
    dataSendCountInterval = 0;
    prevSendCountInterval = 0;
    if ((sendRecvDiff >= (-FS_ISO_OUT_ENDP_PACKET_SIZE)) && (sendRecvDiff <= (FS_ISO_OUT_ENDP_PACKET_SIZE)))
    {
        threshold = AUDIO_DATA_THRESHOLD;
        feedBackData = 0x40000U;
        USB_SplitFeedbackData(feedBackData);
    }
    if (sendRecvDiff <= -threshold)
    {
        threshold += AUDIO_DATA_THRESHOLD;
        feedBackData -= 0x40U;
        USB_SplitFeedbackData(feedBackData);
    }
    if (sendRecvDiff >= threshold)
    {
        threshold += AUDIO_DATA_THRESHOLD;
        feedBackData += 0x40U;
        USB_SplitFeedbackData(feedBackData);
    }
    dataReadCountInterval = 0;
}

usb_status_t USB_DeviceAudioIsoIn(usb_device_handle deviceHandle,
                                  usb_device_endpoint_callback_message_struct_t *event,
                                  void *arg)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)event;

    if ((s_audioSpeaker.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
    {
        error = USB_DeviceSendRequest(deviceHandle, USB_AUDIO_FEEDBACK_ENDPOINT, audioFeedBackBuffer,
                                      FS_ISO_FEEDBACK_ENDP_PACKET_SIZE);
    }
    return error;
}

/* USB device audio ISO IN endpoint callback */
usb_status_t USB_DeviceAudioIsoOut(usb_device_handle deviceHandle,
                                   usb_device_endpoint_callback_message_struct_t *event,
                                   void *arg)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)event;

    if ((s_audioSpeaker.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
    {
        recvCount++;
        if (((recvCount - sendCount) > AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2) && (startSai == 0))
        {
            startSai = 1;
        }

        audioBuffLength[tdReadNumber] = ep_cb_param->length;
        tdReadNumber++;
        if (tdReadNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH)
        {
            tdReadNumber = 0;
        }

        /* request next data to the current buffer */
        error = USB_DeviceRecvRequest(deviceHandle, USB_AUDIO_STREAM_ENDPOINT, &audioDataBuff[tdReadNumber][0],
                                      FS_ISO_OUT_ENDP_PACKET_SIZE * 2);
        USB_AudioDataMatch(ep_cb_param->length);
    }
    return error;
}

/*!
 * @brief Get the setup packet buffer.
 *
 * This function provides the buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setupBuffer The pointer to the address of setup packet buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static uint32_t audioSpeakerSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&audioSpeakerSetup;
    return kStatus_USB_Success;
}

/*!
 * @brief Get the setup packet data buffer.
 *
 * This function gets the data buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    static uint8_t setupOut[8];
    if ((NULL == buffer) || ((*length) > sizeof(setupOut)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = setupOut;
    return kStatus_USB_Success;
}

/*!
 * @brief Configure remote wakeup feature.
 *
 * This function configures the remote wakeup feature.
 *
 * @param handle The USB device handle.
 * @param enable 1: enable, 0: disable.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if ((USB_AUDIO_STREAM_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_AUDIO_CONTROL_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_AUDIO_STREAM_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_AUDIO_CONTROL_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle		  The USB device handle.
 * @param event 		  The USB device event type.
 * @param param 		  The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            s_audioSpeaker.attach = 0U;
            error = kStatus_USB_Success;
            USB_DeviceControlPipeInit(s_audioSpeaker.deviceHandle);
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success ==
                USB_DeviceGetStatus(s_audioSpeaker.deviceHandle, kUSB_DeviceStatusSpeed, &s_audioSpeaker.speed))
            {
                USB_DeviceSetSpeed(s_audioSpeaker.speed);
            }

            if (USB_SPEED_HIGH == s_audioSpeaker.speed)
            {
                s_audioSpeaker.currentMaxPacketSize = HS_ISO_OUT_ENDP_PACKET_SIZE;
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_AUDIO_SPEAKER_CONFIGURE_INDEX == (*temp8))
            {
                s_audioSpeaker.attach = 1U;
                s_audioSpeaker.currentStreamInterfaceAlternateSetting = 0U;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (s_audioSpeaker.attach)
            {
                uint8_t interface = (*temp8) & 0xFF;
                uint8_t alternateSetting = g_UsbDeviceInterface[interface];

                if (s_audioSpeaker.currentStreamInterfaceAlternateSetting != alternateSetting)
                {
                    if (s_audioSpeaker.currentStreamInterfaceAlternateSetting)
                    {
                        USB_DeviceDeinitEndpoint(s_audioSpeaker.deviceHandle,
                                                 USB_AUDIO_STREAM_ENDPOINT | (USB_OUT << 7U));
                    }

                    else
                    {
                        usb_device_endpoint_init_struct_t epInitStruct;
                        usb_device_endpoint_callback_struct_t endpointCallback;

                        endpointCallback.callbackFn = USB_DeviceAudioIsoOut;
                        endpointCallback.callbackParam = handle;

                        epInitStruct.zlt = 0U;
                        epInitStruct.transferType = USB_ENDPOINT_ISOCHRONOUS;
                        epInitStruct.endpointAddress =
                            USB_AUDIO_STREAM_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                        if (USB_SPEED_HIGH == s_audioSpeaker.speed)
                        {
                            epInitStruct.maxPacketSize = 2 * HS_ISO_OUT_ENDP_PACKET_SIZE;
                        }
                        else
                        {
                            epInitStruct.maxPacketSize = 2 * FS_ISO_OUT_ENDP_PACKET_SIZE;
                        }

                        USB_DeviceInitEndpoint(s_audioSpeaker.deviceHandle, &epInitStruct, &endpointCallback);

                        if (s_audioSpeaker.speed == USB_SPEED_HIGH)
                        {
                            epPacketSize = 2 * HS_ISO_OUT_ENDP_PACKET_SIZE;
                        }
                        else
                        {
                            epPacketSize = 2 * FS_ISO_OUT_ENDP_PACKET_SIZE;
                        }

                        error = USB_DeviceRecvRequest(
                            s_audioSpeaker.deviceHandle, USB_AUDIO_STREAM_ENDPOINT, &audioDataBuff[0][0],
                            ((USB_SPEED_HIGH == s_audioSpeaker.speed) ? HS_ISO_OUT_ENDP_PACKET_SIZE * 2 :
                                                                        FS_ISO_OUT_ENDP_PACKET_SIZE * 2));

                        endpointCallback.callbackFn = USB_DeviceAudioIsoIn;
                        endpointCallback.callbackParam = handle;

                        epInitStruct.zlt = 0U;
                        epInitStruct.transferType = USB_ENDPOINT_ISOCHRONOUS;
                        epInitStruct.endpointAddress =
                            USB_AUDIO_FEEDBACK_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                        if (USB_SPEED_HIGH == s_audioSpeaker.speed)
                        {
                            epInitStruct.maxPacketSize = HS_ISO_FEEDBACK_ENDP_PACKET_SIZE;
                        }
                        else
                        {
                            epInitStruct.maxPacketSize = FS_ISO_FEEDBACK_ENDP_PACKET_SIZE;
                        }

                        USB_DeviceInitEndpoint(s_audioSpeaker.deviceHandle, &epInitStruct, &endpointCallback);

                        error = USB_DeviceSendRequest(
                            s_audioSpeaker.deviceHandle, USB_AUDIO_FEEDBACK_ENDPOINT, audioFeedBackBuffer,
                            ((USB_SPEED_HIGH == s_audioSpeaker.speed) ? HS_ISO_FEEDBACK_ENDP_PACKET_SIZE :
                                                                        FS_ISO_FEEDBACK_ENDP_PACKET_SIZE));
                    }
                    s_audioSpeaker.currentStreamInterfaceAlternateSetting = alternateSetting;
                }
            }
            break;
        default:
            break;
    }

    return error;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_audioSpeaker.deviceHandle);
}
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
#if defined(FSL_FEATURE_USBHS_EHCI_COUNT) && (FSL_FEATURE_USBHS_EHCI_COUNT > 1U)
void USB1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_audioSpeaker.deviceHandle);
}
#endif
#endif
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(s_audioSpeaker.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_audioSpeaker.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_audioSpeaker.deviceHandle);
}
#endif

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void APPInit(void)
{
    uint8_t irqNo;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t ehciIrq[] = USBHS_IRQS;
    irqNo = ehciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL, BOARD_USB_PHY_TXCAL45DP, BOARD_USB_PHY_TXCAL45DM,
    };

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs1Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
#else
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t khciIrq[] = USB_IRQS;
    irqNo = khciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNo = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNo = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

#if (((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)) || \
     ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)))
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

    Init_Board_Sai_Codec();

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &s_audioSpeaker.deviceHandle))
    {
        usb_echo("USB device failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device audio speaker demo\r\n");
    }

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNo);

    USB_DeviceRun(s_audioSpeaker.deviceHandle);
}

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
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
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(SAI2_IRQn, (system_irq_handler_t)SAI_TxIRQHandler, NULL);
    SystemInstallIrqHandler(USB_OTG1_IRQn, (system_irq_handler_t)(uint32_t)USBHS_IRQHandler, NULL);

    APPInit();

    while (1)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(s_audioSpeaker.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(s_audioSpeaker.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(s_audioSpeaker.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
        USB_DeviceLpcIp3511TaskFunction(s_audioSpeaker.deviceHandle);
#endif
#endif
    }
}
