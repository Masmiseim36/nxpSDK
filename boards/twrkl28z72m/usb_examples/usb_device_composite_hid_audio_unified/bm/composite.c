/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_hid.h"
#include "usb_device_audio.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif


#include "fsl_sgtl5000.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SGTL_ON_BOARD_OSCILLATOR_FREQUENCY (24576000)
#define BOARD_DEMO_SAI I2S0
#define BOARD_SAI_DEMO_I2C_BASEADDR LPI2C0
#define BOARD_DEMO_SAI_CLKSRC kCLOCK_ScgFircClk
#define BOARD_DEMO_I2C_CLKSRC kCLOCK_ScgFircClk
#define DEMO_SAI_IRQ I2S0_IRQn
#define SAI_UserIRQHandler I2S0_IRQHandler
#define SAI_IRQ_CHANNEL (0U)
#define EXAMPLE_DMA (DMA0)
#define EXAMPLE_DMAMUX DMAMUX0
#define EXAMPLE_TX_CHANNEL (0U)
#define EXAMPLE_RX_CHANNEL (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMux0I2S0Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMux0I2S0Rx
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircClk)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircClk)

#define I2C_RELEASE_SDA_PORT PORTC
#define I2C_RELEASE_SCL_PORT PORTC
#define I2C_RELEASE_SDA_GPIO GPIOC
#define I2C_RELEASE_SDA_PIN 9U
#define I2C_RELEASE_SCL_GPIO GPIOC
#define I2C_RELEASE_SCL_PIN 8U
#define I2C_RELEASE_BUS_COUNT 100U

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW3_NAME

#define AUDIO_DMA_EDMA_MODE (1U)
/*******************************************************************************
* Prototypes
******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
extern void Init_Board_Sai_Codec(void);
extern usb_status_t USB_DeviceHidKeyboardAction(void);
extern char *SW_GetName(void);
extern void USB_AudioCodecTask(void);
extern void USB_AudioSpeakerResetTask(void);
/*******************************************************************************
* Variables
******************************************************************************/
extern usb_device_composite_struct_t g_composite;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE];
extern uint8_t audioRecDataBuff[AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE];
extern sai_transfer_format_t audioFormat;
volatile bool g_ButtonPress = false;

sai_config_t saiTxConfig;
sai_config_t saiRxConfig;
sai_transfer_format_t audioFormat;

#if AUDIO_DMA_EDMA_MODE
sai_edma_handle_t txHandle = {0};
edma_handle_t dmaTxHandle = {0};
sai_edma_handle_t rxHandle = {0};
edma_handle_t dmaRxHandle = {0};
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[FS_ISO_OUT_ENDP_PACKET_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioRecDMATempBuff[FS_ISO_IN_ENDP_PACKET_SIZE];
#endif
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
/* Composite device structure. */
usb_device_composite_struct_t g_composite;
extern usb_device_class_struct_t g_UsbDeviceHidMouseClass;
extern usb_device_class_struct_t g_UsbDeviceAudioClassRecorder;
extern usb_device_class_struct_t g_UsbDeviceAudioClassSpeaker;
extern volatile bool g_ButtonPress;
extern usb_device_composite_struct_t *g_UsbDeviceComposite;
extern usb_device_composite_struct_t *g_deviceAudioComposite;

/* USB device class information */
static usb_device_class_config_struct_t g_CompositeClassConfig[3] = {
    {
        USB_DeviceHidKeyboardCallback, (class_handle_t)NULL, &g_UsbDeviceHidMouseClass,
    },
    {
        USB_DeviceAudioCompositeCallback, (class_handle_t)NULL, &g_UsbDeviceAudioClassRecorder,
    },
    {
        USB_DeviceAudioCompositeCallback, (class_handle_t)NULL, &g_UsbDeviceAudioClassSpeaker,
    }

};

/* USB device class configuration information */
static usb_device_class_config_list_struct_t g_UsbDeviceCompositeConfigList = {
    g_CompositeClassConfig, USB_DeviceCallback, 3,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void SGTL_USB_Audio_Init(void *I2CBase, void *i2cHandle);
extern void SGTL_Set_Playback_Mute(bool mute);
void SGTL_Config_Audio_Formats(uint32_t samplingRate);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    /* Change state of button. */
    g_ButtonPress = true;
}

void BOARD_USB_AUDIO_KEYBOARD_Init(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
    };

    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
}

char *SW_GetName(void)
{
    return BOARD_SW_NAME;
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
    port_pin_config_t i2c_pin_config = {0};
    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);
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

void BOARD_Codec_Init()
{
    sgtl_config_t codecConfig;
    codecConfig.bus = kSGTL_BusI2S;
    codecConfig.master_slave = true;
    codecConfig.route = kSGTL_RoutePlaybackandRecord;
    
    boardCodecConfig.codecConfig = &codecConfig;
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, audioFormat.masterClockHz, audioFormat.sampleRate_Hz, audioFormat.bitWidth);
}

void BOARD_SetCodecMuteUnmute(bool mute)
{
    SGTL_SetMute(&codecHandle, kSGTL_ModuleDAC, mute);
}

void SAI_USB_Audio_TxInit(I2S_Type *SAIBase)
{
    SAI_TxGetDefaultConfig(&saiTxConfig);

    saiTxConfig.masterSlave = kSAI_Slave;
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    saiTxConfig.mclkOutputEnable = false;
#endif

    SAI_TxInit(SAIBase, &saiTxConfig);
}

void SAI_USB_Audio_RxInit(I2S_Type *SAIBase)
{
    SAI_RxGetDefaultConfig(&saiRxConfig);

#if 1
    saiRxConfig.masterSlave = kSAI_Slave;
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    saiRxConfig.mclkOutputEnable = false;
#endif
#endif

    SAI_RxInit(SAIBase, &saiRxConfig);
}

void SGTL_Config_Audio_Formats(uint32_t samplingRate)
{
    /* Configure the audio audioFormat */
    audioFormat.bitWidth = kSAI_WordWidth16bits;
    audioFormat.channel = 0U;
    audioFormat.sampleRate_Hz = samplingRate;

    audioFormat.masterClockHz = SGTL_ON_BOARD_OSCILLATOR_FREQUENCY;
    audioFormat.protocol = saiTxConfig.protocol;
    audioFormat.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    audioFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
}

void BOARD_USB_Audio_TxRxInit(uint32_t samplingRate)
{
    SAI_USB_Audio_TxInit(BOARD_DEMO_SAI);
    SAI_USB_Audio_RxInit(BOARD_DEMO_SAI);
    SGTL_Config_Audio_Formats(samplingRate);
}

#if AUDIO_DMA_EDMA_MODE
static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sai_transfer_t xfer = {0};
    if ((g_composite.audioUnified.audioSendTimes >= g_composite.audioUnified.usbRecvTimes) &&
        (g_composite.audioUnified.startPlayHalfFull == 1))
    {
        g_composite.audioUnified.startPlayHalfFull = 0;
        g_composite.audioUnified.speakerDetachOrNoInput = 1;
    }
    if (g_composite.audioUnified.startPlayHalfFull)
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data = audioPlayDataBuff + g_composite.audioUnified.tdWriteNumberPlay;
        g_composite.audioUnified.audioSendCount += FS_ISO_OUT_ENDP_PACKET_SIZE;
        g_composite.audioUnified.audioSendTimes++;
        g_composite.audioUnified.tdWriteNumberPlay += FS_ISO_OUT_ENDP_PACKET_SIZE;
        if (g_composite.audioUnified.tdWriteNumberPlay >=
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE)
        {
            g_composite.audioUnified.tdWriteNumberPlay = 0;
        }
    }
    else
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data = audioPlayDMATempBuff;
    }
    SAI_TransferSendEDMA(base, handle, &xfer);
}

static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sai_transfer_t xfer = {0};

    if (g_composite.audioUnified.startRec)
    {
        xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        xfer.data = audioRecDataBuff + g_composite.audioUnified.tdReadNumberRec;
        g_composite.audioUnified.tdReadNumberRec += FS_ISO_IN_ENDP_PACKET_SIZE;
        if (g_composite.audioUnified.tdReadNumberRec >=
            AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)
        {
            g_composite.audioUnified.tdReadNumberRec = 0;
        }
    }
    else
    {
        xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        xfer.data = audioRecDMATempBuff;
    }
    SAI_TransferReceiveEDMA(base, handle, &xfer);
}

void BOARD_DMA_EDMA_Config()
{
    edma_config_t dmaConfig = {0};
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);
}

void BOARD_Create_Audio_DMA_EDMA_Handle()
{
    SAI_TransferTxCreateHandleEDMA(BOARD_DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(BOARD_DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);
}

void BOARD_DMA_EDMA_Set_AudioFormat()
{
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz = CLOCK_GetFreq(BOARD_DEMO_SAI_CLKSRC);
    SAI_TransferTxSetFormatEDMA(BOARD_DEMO_SAI, &txHandle, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
    SAI_TransferRxSetFormatEDMA(BOARD_DEMO_SAI, &rxHandle, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
}

void BOARD_DMA_EDMA_Enable_Audio_Interrupts()
{
    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_IRQ);
}

void BOARD_DMA_EDMA_Start()
{
    sai_transfer_t xfer = {0};
    memset(audioPlayDMATempBuff, 0, FS_ISO_OUT_ENDP_PACKET_SIZE);
    memset(audioRecDMATempBuff, 0, FS_ISO_IN_ENDP_PACKET_SIZE);
    xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
    xfer.data = audioPlayDMATempBuff;
    SAI_TransferSendEDMA(BOARD_DEMO_SAI, &txHandle, &xfer);
    xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
    xfer.data = audioRecDMATempBuff;
    SAI_TransferReceiveEDMA(BOARD_DEMO_SAI, &rxHandle, &xfer);
}

void BOARD_Transfer_Mode_Config()
{
    BOARD_DMA_EDMA_Config();
    BOARD_Create_Audio_DMA_EDMA_Handle();
    BOARD_DMA_EDMA_Set_AudioFormat();
    BOARD_DMA_EDMA_Enable_Audio_Interrupts();
    BOARD_DMA_EDMA_Start();
}

void SAI_UserIRQHandler(void)
{
    if (SAI_TxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(BOARD_DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_TxSoftwareReset(BOARD_DEMO_SAI, kSAI_ResetTypeFIFO);
        /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
            exception return operation might vector to incorrect interrupt */
        __DSB();
    }
    if (SAI_RxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_RxClearStatusFlags(BOARD_DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_RxSoftwareReset(BOARD_DEMO_SAI, kSAI_ResetTypeFIFO);
        /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
            exception return operation might vector to incorrect interrupt */
        __DSB();
    }
}
#endif

#if AUDIO_INTERRUPT_IRQ_MODE
void BOARD_Interrupt_Set_AudioFormat()
{
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz = CLOCK_GetFreq(BOARD_DEMO_SAI_CLKSRC);

    SAI_TxSetFormat(BOARD_DEMO_SAI, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
    SAI_RxSetFormat(BOARD_DEMO_SAI, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
}

void BOARD_Enable_Audio_Interrupts()
{
    NVIC_SetPriority((IRQn_Type)DEMO_SAI_IRQ, USB_DEVICE_INTERRUPT_PRIORITY - 1);
    /*  Enable interrupt */
    EnableIRQ(DEMO_SAI_IRQ);
    SAI_TxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
    SAI_TxEnable(BOARD_DEMO_SAI, true);
    SAI_RxEnable(BOARD_DEMO_SAI, true);
}

void SAI_UserIRQHandler(void)
{
    uint32_t data_tx = 0, data_rx = 0;
    uint8_t i, j = 0;
    uint32_t temp = 0;

    /* Clear the FIFO error flag */
    if (SAI_TxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(BOARD_DEMO_SAI, kSAI_FIFOErrorFlag);
    }

    /* Clear the FIFO error flag */
    if (SAI_RxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_RxClearStatusFlags(BOARD_DEMO_SAI, kSAI_FIFOErrorFlag);
    }

    if (SAI_TxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOWarningFlag)
    {
        for (i = 0; i < FSL_FEATURE_SAI_FIFO_COUNT; i++)
        {
            data_tx = 0;
            if (g_composite.audioUnified.startPlayHalfFull)
            {
                for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
                {
                    temp = (uint32_t)(audioPlayDataBuff[g_composite.audioUnified.tdWriteNumberPlay]);
                    data_tx |= (temp << (8U * j));
                    g_composite.audioUnified.tdWriteNumberPlay++;
                    g_composite.audioUnified.audioSendCount++;
                }
                if (g_composite.audioUnified.tdWriteNumberPlay >=
                    AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE)
                {
                    g_composite.audioUnified.tdWriteNumberPlay = 0;
                }
            }
            else if (g_composite.audioUnified.startPlay)
            {
                for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
                {
                    g_composite.audioUnified.audioSendCount++;
                }
                data_tx = 0;
            }
            else
            {
                data_tx = 0;
            }
            SAI_WriteData(BOARD_DEMO_SAI, SAI_IRQ_CHANNEL, data_tx);
        }
    }
    if (SAI_RxGetStatusFlag(BOARD_DEMO_SAI) & kSAI_FIFOWarningFlag)
    {
        data_rx = 0;
        for (i = 0; i < FSL_FEATURE_SAI_FIFO_COUNT; i++)
        {
            data_rx = SAI_ReadData(BOARD_DEMO_SAI, SAI_IRQ_CHANNEL);
            if (g_composite.audioUnified.startRec)
            {
                for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
                {
                    audioRecDataBuff[g_composite.audioUnified.tdReadNumberRec] = (data_rx >> (8U * j));
                    g_composite.audioUnified.tdReadNumberRec++;
                    g_composite.audioUnified.audioRecvCount++;
                }
                if (g_composite.audioUnified.tdReadNumberRec >=
                    AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)
                {
                    g_composite.audioUnified.tdReadNumberRec = 0;
                }
            }
        }
    }
}
#endif

void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_composite.deviceHandle);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

void USB_DeviceClockInit(void)
{

    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_IpSrcFircAsync, CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv1Clk));
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

}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

/* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceKhciTaskFunction(deviceHandle);
}
#endif

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
    uint16_t *temp16 = (uint16_t *)param;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            g_composite.attach = 0U;
            g_composite.currentConfiguration = 0U;
            error = kStatus_USB_Success;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_composite.speed))
            {
                USB_DeviceSetSpeed(handle, g_composite.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                g_composite.attach = 0U;
                g_composite.currentConfiguration = 0U;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                g_composite.attach = 1U;
                g_composite.currentConfiguration = *temp8;
                USB_DeviceAudioCompositeSetConfigure(g_composite.audioUnified.audioSpeakerHandle, *temp8);
                USB_DeviceAudioCompositeSetConfigure(g_composite.audioUnified.audioRecorderHandle, *temp8);
                USB_DeviceHidKeyboardSetConfigure(g_composite.hidKeyboard.hidHandle, *temp8);
                error = kStatus_USB_Success;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:

            if (g_composite.attach)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (USB_AUDIO_RECORDER_STREAM_INTERFACE_INDEX == interface)
                {
                    USB_DeviceAudioRecorderSetInterface(g_composite.audioUnified.audioRecorderHandle, interface,
                                                        alternateSetting);
                }
                else if (USB_AUDIO_SPEAKER_STREAM_INTERFACE_INDEX == interface)
                {
                    USB_DeviceAudioSpeakerSetInterface(g_composite.audioUnified.audioSpeakerHandle, interface,
                                                       alternateSetting);
                }
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = g_composite.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_DEVICE_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | g_composite.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetHidReportDescriptor:
            if (param)
            {
                error =
                    USB_DeviceGetHidReportDescriptor(handle, (usb_device_get_hid_report_descriptor_struct_t *)param);
            }
            break;
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
        case kUSB_DeviceEventGetDeviceQualifierDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceQualifierDescriptor(
                    handle, (usb_device_get_device_qualifier_descriptor_struct_t *)param);
            }
            break;
#endif
        default:
            break;
    }

    return error;
}

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void APPInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    g_composite.speed = USB_SPEED_FULL;
    g_composite.attach = 0U;
    g_composite.audioUnified.audioSpeakerHandle = (class_handle_t)NULL;
    g_composite.audioUnified.audioRecorderHandle = (class_handle_t)NULL;
    g_composite.hidKeyboard.hidHandle = (class_handle_t)NULL;
    g_composite.deviceHandle = NULL;

    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceCompositeConfigList, &g_composite.deviceHandle))
    {
        usb_echo("USB device composite demo init failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device composite demo\r\n");
        usb_echo("Please Press  switch(%s) to mute/unmute device audio speaker.\r\n", SW_GetName());

        g_composite.hidKeyboard.hidHandle = g_UsbDeviceCompositeConfigList.config[0].classHandle;
        g_composite.audioUnified.audioRecorderHandle = g_UsbDeviceCompositeConfigList.config[1].classHandle;
        g_composite.audioUnified.audioSpeakerHandle = g_UsbDeviceCompositeConfigList.config[2].classHandle;

        USB_DeviceAudioCompositeInit(&g_composite);
        USB_DeviceHidKeyboardInit(&g_composite);
    }

    Init_Board_Sai_Codec();

    USB_DeviceIsrEnable();

    USB_DeviceRun(g_composite.deviceHandle);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    /* Choose clock source for LPI2C */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
    /* Choose clock source for SAI */
    CLOCK_SetIpSrc(kCLOCK_Sai0, kCLOCK_IpSrcFircAsync);

    APPInit();

    while (1)
    {
        USB_DeviceHidKeyboardAction();

        USB_AudioCodecTask();

        USB_AudioSpeakerResetTask();

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_composite.deviceHandle);
#endif
    }
}
