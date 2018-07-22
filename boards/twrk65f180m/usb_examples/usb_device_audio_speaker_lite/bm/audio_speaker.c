/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#include "fsl_sai.h"

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "fsl_sgtl5000.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI, I2C instance and clock */
#define DEMO_SAI I2S0
#define DEMO_I2C I2C0
#define DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DMAMUX0 DMAMUX
#define EXAMPLE_DMA DMA0
#define EXAMPLE_CHANNEL (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMux0I2S0Tx
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define AUDIO_DATA_WHOLE_BUFFER_LENGTH (32)
#define AUDIO_DATA_DISCARD_BUFFER_LENGTH (28)
#define DEMO_SAI_IRQ I2S0_Tx_IRQn
#define I2S_ClearFIFOError_IRQHandler I2S0_Tx_IRQHandler
#define SAI_TxIRQHandler I2S0_Tx_IRQHandler

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 18U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 19U
#define I2C_RELEASE_BUS_COUNT 100U
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcIrc48M
#define USB_FS_CLK_FREQ 48000000U
#define USB_HS_PHY_CLK_SRC kCLOCK_UsbPhySrcExt
#define USB_HS_PHY_CLK_FREQ BOARD_XTAL0_CLK_HZ
#define USB_HS_CLK_SRC kCLOCK_UsbSrcUnused
#define USB_HS_CLK_FREQ 0U
#define NO_AUDIO_DATA 0x0
#define AUDIO_DATA_THRESHOLD (FS_ISO_OUT_ENDP_PACKET_SIZE * 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void APPInit(void);
usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand, uint32_t *length, uint8_t **buffer);
void BOARD_InitHardware(void);
extern void BOARD_I2C_Init(void);
extern void BOARD_SAI_TxInit(void);
extern void BOARD_Codec_Init(void);
extern void BOARD_SAI_TransferTxSetFormat(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t audioDataBuff[AUDIO_DATA_WHOLE_BUFFER_LENGTH][FS_ISO_OUT_ENDP_PACKET_SIZE * 2];
uint8_t audioBuffLength[AUDIO_DATA_WHOLE_BUFFER_LENGTH] = {0};
/*uint8_t audioFeedBackBuffer[3] = {0xF0U, 0x00U, 0x04U}; //noise comes every time the recv buffer exceeds the send
 * buffer */
uint8_t audioFeedBackBuffer[3] = {0x00U, 0x00U, 0x04U};
uint8_t audioDataDiscardBuff[FS_ISO_OUT_ENDP_PACKET_SIZE];
volatile uint32_t dataSend = 0;
volatile uint32_t startSai = 0;
volatile uint32_t audioBufferIsFull = 0;
volatile uint32_t audioBufferIsEmpty = 0;
uint32_t audioTempBuffer;
uint16_t epPacketSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
sai_transfer_t xfer;
volatile uint32_t sendCount = 0;
volatile uint32_t recvCount = 0;
volatile uint32_t dataRead = 0;
volatile uint32_t dataReadTemp = 0;
volatile int32_t prevSendCount =
    -(AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2 * (FS_ISO_OUT_ENDP_PACKET_SIZE / (DEMO_SAI_BITWIDTH / 8U)));
volatile uint32_t tdReadNumber = 0;
volatile uint32_t tdWriteNumber = 0;
extern uint8_t
    g_UsbDeviceInterface[USB_AUDIO_SPEAKER_INTERFACE_COUNT]; /* Default value of audio speaker device struct */
extern sai_handle_t txHandle;

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
    .curSampleFrequency = 16000U,
    .curClockValid = 1U,
    .controlRange = {1U, 16000U, 16000U, 0U},
#endif
    .speed = USB_SPEED_FULL,
    .attach = 0U,
};
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
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
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
            if (tdReadNumber == tdWriteNumber)
            {
                startSai = 0;
            }
            for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
            {
                temp = (uint32_t)(audioDataBuff[tdWriteNumber][dataSend]);
                data |= (temp << (8U * j));
                dataSend++;
            }
            if (dataSend >= audioBuffLength[tdWriteNumber])
            {
                dataSend = 0;
                tdWriteNumber++;
                if (tdWriteNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH)
                {
                    tdWriteNumber = 0;
                }
            }
            sendCount++;
        }
        else
        {
            data = NO_AUDIO_DATA;
        }
        SAI_WriteData(DEMO_SAI, EXAMPLE_CHANNEL, data);
    }
}

/* Initialize the structure information for sai. */
void Init_Board_Sai_Codec(void)
{
    usb_echo("Init Audio SAI and CODEC\r\n");

    /* Init SAI module */
    BOARD_SAI_TxInit();

    BOARD_I2C_Init();

    BOARD_Codec_Init();

    BOARD_SAI_TransferTxSetFormat();

    /*  Enable interrupt */
    EnableIRQ(DEMO_SAI_IRQ);
    SAI_TxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
    SAI_TxEnable(BOARD_DEMO_SAI, true);
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

/*!
 * @brief USB Audio data discard algorithm.
 *
 * This function discard the USB Audio data .
 * On the most soc, the USB SOF is about 0.99999, that means every 100000 SOF
 * will send 1 more audio data, and after more than 40 seconds it will send
 * 64 bytes audio data. Since the SAI frame sampling rate is accurate 16k, so
 * the USB will receive more data than the SAI sends. After more than
 * 40 * AUDIO_DATA_DISCARD_BUFFER_LENGTH seconds the audioDataBuff is full and
 * the USB stops receives data to audioDataBuff but to audioDataDiscardBuff and
 * it will discard AUDIO_DATA_DISCARD_BUFFER_LENGTH audio data every
 * 40 * AUDIO_DATA_DISCARD_BUFFER_LENGTH seconds. We can change AUDIO_DATA_WHOLE_BUFFER_LENGTH
 * and the AUDIO_DATA_DISCARD_BUFFER_LENGTH to alternate the time and length to
 * discard.
 *
 */
void USB_SplitFeedbackData(uint32_t feedbackData)
{
    audioFeedBackBuffer[0] = (uint8_t)feedbackData;
    audioFeedBackBuffer[1] = (uint8_t)(feedbackData >> 8U);
    audioFeedBackBuffer[2] = (uint8_t)(feedbackData >> 16U);
}

int32_t USB_CalculateSendDiff(void)
{
    static int32_t intervalSendDiff = 0;
    if (startSai)
    {
        intervalSendDiff = sendCount - prevSendCount;
        prevSendCount = sendCount;
    }
    else
    {
        intervalSendDiff = 1000 * (FS_ISO_OUT_ENDP_PACKET_SIZE / (DEMO_SAI_BITWIDTH / 8U));
        ;
    }
    return intervalSendDiff;
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
    sendRecvDiff += USB_CalculateSendDiff() * (DEMO_SAI_BITWIDTH / 8U) - dataReadCountInterval;

    if (sendRecvDiff == 0)
    {
        threshold = AUDIO_DATA_THRESHOLD;
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

/* USB device audio ISO IN endpoint callback */
usb_status_t USB_DeviceAudioIsoOut(usb_device_handle deviceHandle,
                                   usb_device_endpoint_callback_message_struct_t *event,
                                   void *arg)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)event;
    static uint32_t tempTdRead = 0;

    if ((s_audioSpeaker.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
    {
        recvCount++;
        if (recvCount % 129600000 == 0)
        {
            sendCount = 0;
            prevSendCount = 0;
        }
        if (startSai == 0)
        {
            if (tdReadNumber < tdWriteNumber)
            {
                tempTdRead = tdReadNumber + AUDIO_DATA_WHOLE_BUFFER_LENGTH;
                if (tempTdRead - tdWriteNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2)
                {
                    startSai = 1;
                }
            }
            else
            {
                if (tdReadNumber - tdWriteNumber >= AUDIO_DATA_WHOLE_BUFFER_LENGTH / 2)
                {
                    startSai = 1;
                }
            }
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
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
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
                            epPacketSize = HS_ISO_OUT_ENDP_PACKET_SIZE;
                        }
                        else
                        {
                            epPacketSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
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
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(s_audioSpeaker.deviceHandle);
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

    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t khciIrq[] = USB_IRQS;
    irqNo = khciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

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
    NVIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNo);

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
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

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
#endif
    }
}
