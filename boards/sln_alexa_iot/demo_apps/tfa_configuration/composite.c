/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"
#include "usb_device_audio.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "stdio.h"
#include "stdlib.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#endif
#include "fsl_codec_adapter.h"
#include "fsl_tfa9xxx.h"
#include "tfa_config_TFA9894N2A1_48kHz_PUI.h"
#ifndef FSL_TFA9XXX_HEADER_VERSION
#error "Cannot find a valid config header for tfa9xxx driver."
#elif FSL_TFA9XXX_HEADER_VERSION != FSL_TFA9XXX_DRIVER_VERSION
#warning "Version mismatch between tfa9xxx config header and tfa9xxx driver."
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (256U)

/* I2C instance and clock */
#define DEMO_I2C LPI2C1
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                       \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* DMA */
#define EXAMPLE_DMAMUX        DMAMUX
#define EXAMPLE_DMA           DMA0
#define EXAMPLE_TX_CHANNEL    (0U)
#define EXAMPLE_RX_CHANNEL    (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMuxSai1Rx


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
extern void Init_Board_Sai(void);

extern void USB_AudioCodecTask(void);
extern void USB_DeviceAudioSpeakerStatusReset(void);
extern void USB_DeviceAudioRecorderStatusReset(void);
extern usb_status_t USB_DeviceAudioRecorderSetInterface(usb_device_handle handle,
                                                        uint8_t interface,
                                                        uint8_t alternateSetting);
extern usb_status_t USB_DeviceAudioSpeakerSetInterface(usb_device_handle handle,
                                                       uint8_t interface,
                                                       uint8_t alternateSetting);
extern void USB_AudioSpeakerResetTask(void);
extern usb_status_t USB_DeviceHidGenericInit(usb_device_composite_struct_t *deviceComposite);
extern usb_status_t USB_DeviceHidGenericClassRequest(usb_device_handle handle,
                                                     usb_setup_struct_t *setup,
                                                     uint32_t *length,
                                                     uint8_t **buffer);
extern usb_status_t USB_DeviceHidGenericSetConfigure(usb_device_handle handle, uint8_t configure);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern usb_device_composite_struct_t g_composite;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE];
extern uint8_t audioRecDataBuff[AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE];
extern sai_transfer_format_t audioFormat;

sai_config_t saiTxConfig;
sai_config_t saiRxConfig;
sai_transfer_format_t audioFormat;
sai_edma_handle_t txHandle = {0};
edma_handle_t dmaTxHandle  = {0};
sai_edma_handle_t rxHandle = {0};
edma_handle_t dmaRxHandle  = {0};
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[FS_ISO_OUT_ENDP_PACKET_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioRecDMATempBuff[FS_ISO_IN_ENDP_PACKET_SIZE];

codec_handle_t codecHandle;
tfa9xxx_config_t tfa9xxxConfig = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 10000000U},
    .slaveAddress = TFA9XXX_I2C_ADDR_0,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 0,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfig};

// global volatile counter used for sleep/wait
volatile uint32_t g_systickCounter;
/* Composite device structure. */
usb_device_composite_struct_t g_composite;
extern usb_device_composite_struct_t *g_UsbDeviceComposite;
extern usb_device_composite_struct_t *g_deviceComposite;
extern uint8_t g_UsbDeviceInterface[USB_COMPOSITE_INTERFACE_COUNT];
extern codec_config_t boardCodecConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
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

extern void TFA9XXX_Config_Audio_Formats(uint32_t samplingRate);

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

/* delay n milli-seconds */
void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

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
    status_t rc;
    rc = CODEC_Init(&codecHandle, &boardCodecConfig);
    if (rc != kStatus_Success)
        usb_echo("Codec init failed!\n");
}

void BOARD_SetCodecMuteUnmute(bool mute)
{
    status_t rc;
    rc = CODEC_SetMute(&codecHandle, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, mute);
    if (rc != kStatus_Success)
        usb_echo("Codec set mute/unmute failed!\n");
}

void BOARD_SetCodecVolume(uint32_t volume)
{
    status_t rc;
    rc = CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, volume);
    if (rc != kStatus_Success)
        usb_echo("Codec set volume failed!\n");
}

void SAI_USB_Audio_TxInit(I2S_Type *SAIBase)
{
    SAI_TxGetDefaultConfig(&saiTxConfig);

    SAI_TxInit(SAIBase, &saiTxConfig);
}

void SAI_USB_Audio_RxInit(I2S_Type *SAIBase)
{
    SAI_RxGetDefaultConfig(&saiRxConfig);

    SAI_RxInit(SAIBase, &saiRxConfig);
}

void TFA9XXX_Config_Audio_Formats(uint32_t samplingRate)
{
    /* Configure the audio audioFormat */
    audioFormat.bitWidth      = kSAI_WordWidth16bits;
    audioFormat.channel       = 0U;
    audioFormat.sampleRate_Hz = samplingRate;

    audioFormat.protocol           = saiTxConfig.protocol;
    audioFormat.stereo             = kSAI_Stereo;
    audioFormat.isFrameSyncCompact = true;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    audioFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
}

void BOARD_USB_Audio_TxRxInit(uint32_t samplingRate)
{
    SAI_USB_Audio_TxInit(BOARD_AMP_SAI);
    SAI_USB_Audio_RxInit(BOARD_AMP_SAI);
    TFA9XXX_Config_Audio_Formats(samplingRate);
}

static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sai_transfer_t xfer = {0};
    if ((g_composite.audioUnified.audioSendTimes >= g_composite.audioUnified.usbRecvTimes) &&
        (g_composite.audioUnified.startPlayHalfFull == 1))
    {
        g_composite.audioUnified.startPlayHalfFull      = 0;
        g_composite.audioUnified.speakerDetachOrNoInput = 1;
    }
    if (g_composite.audioUnified.startPlayHalfFull)
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data     = audioPlayDataBuff + g_composite.audioUnified.tdWriteNumberPlay;
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
        xfer.data     = audioPlayDMATempBuff;
    }
    SAI_TransferSendEDMA(base, handle, &xfer);
}

static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sai_transfer_t xfer = {0};

    if (g_composite.audioUnified.startRec)
    {
        xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        xfer.data     = audioRecDataBuff + g_composite.audioUnified.tdReadNumberRec;
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
        xfer.data     = audioRecDMATempBuff;
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

    DMAMUX_Init(DMAMUX);
    DMAMUX_SetSource(DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)kDmaRequestMuxSai3Tx);
    DMAMUX_EnableChannel(DMAMUX, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)kDmaRequestMuxSai3Rx);
    DMAMUX_EnableChannel(DMAMUX, EXAMPLE_RX_CHANNEL);
}

void BOARD_Create_Audio_DMA_EDMA_Handle()
{
    SAI_TransferTxCreateHandleEDMA(BOARD_AMP_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(BOARD_AMP_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);
}

void BOARD_DMA_EDMA_Set_AudioFormat()
{
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz          = BOARD_AMP_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatEDMA(BOARD_AMP_SAI, &txHandle, &audioFormat, mclkSourceClockHz,
                                OVER_SAMPLE_RATE * audioFormat.sampleRate_Hz);
    SAI_TransferRxSetFormatEDMA(BOARD_AMP_SAI, &rxHandle, &audioFormat, mclkSourceClockHz,
                                OVER_SAMPLE_RATE * audioFormat.sampleRate_Hz);
}

void BOARD_DMA_EDMA_Enable_Audio_Interrupts()
{
    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(BOARD_AMP_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(BOARD_AMP_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(BOARD_AMP_SAI_TX_IRQ);
    EnableIRQ(BOARD_AMP_SAI_RX_IRQ);
}

void BOARD_DMA_EDMA_Start()
{
    sai_transfer_t xfer = {0};
    memset(audioPlayDMATempBuff, 0, FS_ISO_OUT_ENDP_PACKET_SIZE);
    memset(audioRecDMATempBuff, 0, FS_ISO_IN_ENDP_PACKET_SIZE);
    xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
    xfer.data     = audioPlayDMATempBuff;
    SAI_TransferSendEDMA(BOARD_AMP_SAI, &txHandle, &xfer);
    xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
    xfer.data     = audioRecDMATempBuff;
    SAI_TransferReceiveEDMA(BOARD_AMP_SAI, &rxHandle, &xfer);
}

void BOARD_Transfer_Mode_Config()
{
    BOARD_DMA_EDMA_Config();
    BOARD_Create_Audio_DMA_EDMA_Handle();
    BOARD_DMA_EDMA_Set_AudioFormat();
    BOARD_DMA_EDMA_Enable_Audio_Interrupts();
    BOARD_DMA_EDMA_Start();
}

void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF flag */
    SAI_TxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(BOARD_AMP_SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

void SAI_UserRxIRQHandler(void)
{
    SAI_RxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);
    SAI_RxSoftwareReset(BOARD_AMP_SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

void SAI_UserIRQHandler(void)
{
    if (BOARD_AMP_SAI->TCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }

    if (BOARD_AMP_SAI->RCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_composite.deviceHandle);
}

void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_composite.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
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
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            /* Initialize the control IN and OUT pipes */
            USB_DeviceControlPipeInit(handle);
            g_composite.attach               = 0U;
            g_composite.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(handle, kUSB_DeviceStatusSpeed, &g_composite.speed))
            {
                USB_DeviceSetSpeed(g_composite.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                g_composite.attach               = 0U;
                g_composite.currentConfiguration = 0U;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                g_composite.attach               = 1U;
                g_composite.currentConfiguration = *temp8;
                USB_DeviceAudioUnifiedSetConfigure(handle, *temp8);
                USB_DeviceHidGenericSetConfigure(handle, *temp8);
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
                uint8_t interface        = (uint8_t)(*temp8);
                uint8_t alternateSetting = (uint8_t)g_UsbDeviceInterface[interface];
                if (USB_AUDIO_RECORDER_STREAM_INTERFACE_INDEX == interface)
                {
                    if (g_composite.audioUnified.currentInterfaceAlternateSetting[interface] != alternateSetting)
                    {
                        if (g_composite.audioUnified.currentInterfaceAlternateSetting[interface] != alternateSetting)
                        {
                            if (g_composite.audioUnified.currentInterfaceAlternateSetting[interface])
                            {
                                USB_DeviceDeinitEndpoint(
                                    g_composite.deviceHandle,
                                    USB_AUDIO_RECORDER_STREAM_ENDPOINT |
                                        (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
                            }
                            else
                            {
                                USB_DeviceAudioRecorderStatusReset();
                                USB_DeviceAudioRecorderSetInterface(handle, interface, alternateSetting);
                            }
                            g_composite.audioUnified.currentInterfaceAlternateSetting[interface] = alternateSetting;
                        }
                    }
                }
                else if (USB_AUDIO_SPEAKER_STREAM_INTERFACE_INDEX == interface)
                {
                    if (g_composite.audioUnified.currentInterfaceAlternateSetting[interface] != alternateSetting)
                    {
                        if (g_composite.audioUnified.currentInterfaceAlternateSetting[interface])
                        {
                            USB_DeviceDeinitEndpoint(g_composite.deviceHandle,
                                                     USB_AUDIO_SPEAKER_STREAM_ENDPOINT |
                                                         (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
                        }
                        else
                        {
                            USB_DeviceAudioSpeakerStatusReset();
                            USB_DeviceAudioSpeakerSetInterface(handle, interface, alternateSetting);
                        }
                        g_composite.audioUnified.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    }
                }
                error = kStatus_USB_Success;
            }
            break;
        default:
            break;
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
    static uint32_t compositeSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&compositeSetup;
    return kStatus_USB_Success;
}

/*!
 * @brief Get the vendor request data buffer.
 *
 * This function gets the data buffer for vendor request.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetVendorReceiveBuffer(usb_device_handle handle,
                                              usb_setup_struct_t *setup,
                                              uint32_t *length,
                                              uint8_t **buffer)
{
    return kStatus_USB_Error;
}

/*!
 * @brief Audio vendor specific callback function.
 *
 * This function handles the CDC vendor specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceProcessVendorRequest(usb_device_handle handle,
                                            usb_setup_struct_t *setup,
                                            uint32_t *length,
                                            uint8_t **buffer)
{
    return kStatus_USB_InvalidRequest;
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
    usb_status_t error = kStatus_USB_InvalidRequest;
    error              = USB_DeviceAudioUnifiedConfigureEndpointStatus(handle, ep, status);
    error              = USB_DeviceHidConfigureEndpointStatus(handle, ep, status);

    return error;
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
 * @brief Audio class specific callback function.
 *
 * This function handles the Audio class specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if ((setup->bmRequestType & USB_REQUEST_TYPE_RECIPIENT_MASK) != USB_REQUEST_TYPE_RECIPIENT_INTERFACE)
    {
        return USB_DeviceAudioUnifiedClassRequestEndpRecipient(handle, setup, length, buffer);
    }
    else
    {
        if (USB_AUDIO_CONTROL_INTERFACE_INDEX == (setup->wIndex & 0xFFU))
        {
            return USB_DeviceAudioUnifiedClassRequestIntfRecipient(handle, setup, length, buffer);
        }
        else if (USB_HID_GENERIC_INTERFACE_INDEX == (setup->wIndex & 0xFFU))
        {
            return USB_DeviceHidGenericClassRequest(handle, setup, length, buffer);
        }
        else
        {
        }
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

    g_composite.speed        = USB_SPEED_FULL;
    g_composite.attach       = 0U;
    g_composite.deviceHandle = NULL;

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_composite.deviceHandle))
    {
        usb_echo("TFA control demo init failed\r\n");
        return;
    }
    else
    {
        usb_echo("TFA control demo started\r\n");
        USB_DeviceAudioUnifiedInit(&g_composite);
        USB_DeviceHidGenericInit(&g_composite);
    }

    Init_Board_Sai();

    USB_DeviceIsrEnable();

    USB_DeviceRun(g_composite.deviceHandle);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI3*/
    CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);
    // System Tick Configuration, generate 1ms interrupt
    SysTick_Config(SystemCoreClock / 1000U);

    APPInit();

    while (1)
    {
        USB_AudioCodecTask();

        USB_AudioSpeakerResetTask();

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_composite.deviceHandle);
#endif
    }
}
