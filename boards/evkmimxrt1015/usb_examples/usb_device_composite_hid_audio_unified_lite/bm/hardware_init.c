/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "composite.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_adapter_audio.h"
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${variable:start}*/
extern usb_device_composite_struct_t g_composite;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL * AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
extern uint8_t audioRecDataBuff[AUDIO_RECORDER_DATA_WHOLE_BUFFER_COUNT_NORMAL * FS_ISO_IN_ENDP_PACKET_SIZE];
volatile bool g_ButtonPress = false;
HAL_AUDIO_HANDLE_DEFINE(audioTxHandle);
hal_audio_config_t audioTxConfig;
hal_audio_dma_config_t dmaTxConfig;
HAL_AUDIO_HANDLE_DEFINE(audioRxHandle);
hal_audio_config_t audioRxConfig;
hal_audio_dma_config_t dmaRxConfig;
hal_audio_ip_config_t ipTxConfig;
hal_audio_ip_config_t ipRxConfig;
hal_audio_dma_mux_config_t dmaMuxTxConfig;
hal_audio_dma_mux_config_t dmaMuxRxConfig;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioRecDMATempBuff[FS_ISO_IN_ENDP_PACKET_SIZE];
uint32_t masterClockHz = 0U;
codec_handle_t codecHandle;

wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .leftInputSource  = kWM8960_InputDifferentialMicInput3,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.mclk_HZ    = 12288000U,
               .sampleRate = kWM8960_AudioSampleRate48KHz,
               .bitWidth   = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};

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
extern void WM8960_USB_Audio_Init(void *I2CBase, void *i2cHandle);
extern void WM8960_Config_Audio_Formats(uint32_t samplingRate);
extern uint32_t USB_AudioSpeakerBufferSpaceUsed(void);
extern void USB_DeviceCalculateFeedback(void);
/*${variable:end}*/
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

void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_USB_AUDIO_KEYBOARD_Init(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };

    /* Init input switch GPIO. */
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
}

char *SW_GetName(void)
{
    return BOARD_SW_NAME;
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

    BOARD_USB_AUDIO_KEYBOARD_Init();

    dmaMuxTxConfig.dmaMuxConfig.dmaMuxInstance   = DEMO_DMAMUX_INDEX;
    dmaMuxTxConfig.dmaMuxConfig.dmaRequestSource = DEMO_SAI_TX_SOURCE;
    dmaTxConfig.dmaMuxConfig                     = &dmaMuxTxConfig;
    dmaTxConfig.instance                         = DEMO_DMA_INDEX;
    dmaTxConfig.channel                          = DEMO_DMA_TX_CHANNEL;
    dmaTxConfig.priority                         = kHAL_AudioDmaChannelPriorityDefault;
    dmaTxConfig.dmaChannelMuxConfig              = NULL;
    ipTxConfig.sai.lineMask                      = 1U << 0U;
    ipTxConfig.sai.syncMode                      = kHAL_AudioSaiModeAsync;
    audioTxConfig.dmaConfig                      = &dmaTxConfig;
    audioTxConfig.ipConfig                       = &ipTxConfig;
    audioTxConfig.instance                       = DEMO_SAI_INSTANCE_INDEX;
    audioTxConfig.srcClock_Hz                    = DEMO_SAI_CLK_FREQ;
    audioTxConfig.sampleRate_Hz                  = (uint32_t)kHAL_AudioSampleRate48KHz;
    audioTxConfig.masterSlave                    = kHAL_AudioMaster;
    audioTxConfig.bclkPolarity                   = kHAL_AudioSampleOnRisingEdge;
    audioTxConfig.frameSyncWidth                 = kHAL_AudioFrameSyncWidthHalfFrame;
    audioTxConfig.frameSyncPolarity              = kHAL_AudioBeginAtFallingEdge;
    audioTxConfig.dataFormat                     = kHAL_AudioDataFormatI2sClassic;
    audioTxConfig.fifoWatermark                  = (uint8_t)(FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) - 1);
    audioTxConfig.bitWidth                       = (uint8_t)kHAL_AudioWordWidth16bits;
    audioTxConfig.lineChannels                   = kHAL_AudioStereo;

    dmaMuxRxConfig.dmaMuxConfig.dmaMuxInstance   = DEMO_DMAMUX_INDEX;
    dmaMuxRxConfig.dmaMuxConfig.dmaRequestSource = DEMO_SAI_RX_SOURCE;
    dmaRxConfig.dmaMuxConfig                     = &dmaMuxRxConfig;
    dmaRxConfig.instance                         = DEMO_DMA_INDEX;
    dmaRxConfig.channel                          = DEMO_DMA_RX_CHANNEL;
    dmaRxConfig.priority                         = kHAL_AudioDmaChannelPriorityDefault;
    dmaRxConfig.dmaChannelMuxConfig              = NULL;
    ipRxConfig.sai.lineMask                      = 1U << 0U;
    ipRxConfig.sai.syncMode                      = kHAL_AudioSaiModeSync;
    audioRxConfig.dmaConfig                      = &dmaRxConfig;
    audioRxConfig.ipConfig                       = &ipRxConfig;
    audioRxConfig.instance                       = DEMO_SAI_INSTANCE_INDEX;
    audioRxConfig.srcClock_Hz                    = DEMO_SAI_CLK_FREQ;
    audioRxConfig.sampleRate_Hz                  = (uint32_t)kHAL_AudioSampleRate48KHz;
    audioRxConfig.masterSlave                    = kHAL_AudioMaster;
    audioRxConfig.bclkPolarity                   = kHAL_AudioSampleOnRisingEdge;
    audioRxConfig.frameSyncWidth                 = kHAL_AudioFrameSyncWidthHalfFrame;
    audioRxConfig.frameSyncPolarity              = kHAL_AudioBeginAtFallingEdge;
    audioRxConfig.dataFormat                     = kHAL_AudioDataFormatI2sClassic;
    audioRxConfig.fifoWatermark                  = (uint8_t)((uint32_t)FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2U);
    audioRxConfig.bitWidth                       = (uint8_t)kHAL_AudioWordWidth16bits;
    audioRxConfig.lineChannels                   = kHAL_AudioStereo;
}

void BOARD_Codec_Init()
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
}

void BOARD_SetCodecMuteUnmute(bool mute)
{
    if (CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, mute) !=
        kStatus_Success)
    {
        assert(false);
    }
}

static void txCallback(hal_audio_handle_t handle, hal_audio_status_t completionStatus, void *callbackParam)
{
    uint32_t audioSpeakerPreReadDataCount = 0U;
    uint32_t preAudioSendCount            = 0U;
    hal_audio_transfer_t xfer             = {0};
    if ((USB_AudioSpeakerBufferSpaceUsed() < (g_composite.audioUnified.audioPlayTransferSize)) &&
        (g_composite.audioUnified.startPlayFlag == 1U))
    {
        g_composite.audioUnified.startPlayFlag          = 0;
        g_composite.audioUnified.speakerDetachOrNoInput = 1;
    }
    if (0U != g_composite.audioUnified.startPlayFlag)
    {
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
        USB_DeviceCalculateFeedback();
#endif
        xfer.dataSize     = g_composite.audioUnified.audioPlayTransferSize;
        xfer.data         = audioPlayDataBuff + g_composite.audioUnified.tdReadNumberPlay;
        preAudioSendCount = g_composite.audioUnified.audioSendCount[0];
        g_composite.audioUnified.audioSendCount[0] += g_composite.audioUnified.audioPlayTransferSize;
        if (preAudioSendCount > g_composite.audioUnified.audioSendCount[0])
        {
            g_composite.audioUnified.audioSendCount[1] += 1U;
        }
        g_composite.audioUnified.audioSendTimes++;
        g_composite.audioUnified.tdReadNumberPlay += g_composite.audioUnified.audioPlayTransferSize;
        if (g_composite.audioUnified.tdReadNumberPlay >= g_composite.audioUnified.audioPlayBufferSize)
        {
            g_composite.audioUnified.tdReadNumberPlay = 0;
        }
        audioSpeakerPreReadDataCount = g_composite.audioUnified.audioSpeakerReadDataCount[0];
        g_composite.audioUnified.audioSpeakerReadDataCount[0] += g_composite.audioUnified.audioPlayTransferSize;
        if (audioSpeakerPreReadDataCount > g_composite.audioUnified.audioSpeakerReadDataCount[0])
        {
            g_composite.audioUnified.audioSpeakerReadDataCount[1] += 1U;
        }
    }
    else
    {
        if (0U != g_composite.audioUnified.audioPlayTransferSize)
        {
            xfer.dataSize = g_composite.audioUnified.audioPlayTransferSize;
        }
        else
        {
            xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
        }
        xfer.data = audioPlayDMATempBuff;
    }
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}

static void rxCallback(hal_audio_handle_t handle, hal_audio_status_t completionStatus, void *callbackParam)
{
    hal_audio_transfer_t xfer = {0};

    if (g_composite.audioUnified.startRec)
    {
        xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        xfer.data     = audioRecDataBuff + g_composite.audioUnified.tdWriteNumberRec;
        g_composite.audioUnified.tdWriteNumberRec += FS_ISO_IN_ENDP_PACKET_SIZE;
        if (g_composite.audioUnified.tdWriteNumberRec >=
            AUDIO_RECORDER_DATA_WHOLE_BUFFER_COUNT_NORMAL * FS_ISO_IN_ENDP_PACKET_SIZE)
        {
            g_composite.audioUnified.tdWriteNumberRec = 0;
        }
    }
    else
    {
        xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        xfer.data     = audioRecDMATempBuff;
    }
    HAL_AudioTransferReceiveNonBlocking(handle, &xfer);
}

void AUDIO_DMA_EDMA_Start()
{
    usb_echo("Init Audio SAI and CODEC\r\n");
    hal_audio_transfer_t xfer = {0};
    memset(audioPlayDMATempBuff, 0, AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME);
    memset(audioRecDMATempBuff, 0, FS_ISO_IN_ENDP_PACKET_SIZE);
    xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
    xfer.data     = audioPlayDMATempBuff;
    HAL_AudioTxInstallCallback((hal_audio_handle_t)&audioTxHandle[0], txCallback, NULL);
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
    xfer.dataSize = FS_ISO_IN_ENDP_PACKET_SIZE;
    xfer.data     = audioRecDMATempBuff;
    HAL_AudioRxInstallCallback((hal_audio_handle_t)&audioRxHandle[0], rxCallback, NULL);
    HAL_AudioTransferReceiveNonBlocking((hal_audio_handle_t)&audioRxHandle[0], &xfer);
}

void USB_OTG1_IRQHandler(void)
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

    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
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
/*${function:end}*/
