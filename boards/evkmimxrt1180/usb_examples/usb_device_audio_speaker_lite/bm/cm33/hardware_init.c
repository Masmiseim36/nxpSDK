/*
 * Copyright 2022 - 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "usb_device_audio.h"
#include "audio_speaker.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_sai.h"
#include "fsl_trdc.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_adapter_audio.h"
#include "fsl_codec_adapter.h"
/*${header:end}*/
/*${variable:start}*/
extern usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL * AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
HAL_AUDIO_HANDLE_DEFINE(audioTxHandle);
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
uint32_t masterClockHz = 0U;
codec_handle_t codecHandle;

hal_audio_dma_channel_mux_config_t dmaChannelSource = {
    .dmaChannelMuxConfig.dmaRequestSource = DEMO_SAI_TX_SOURCE,
};

edma_channel_config_t edmaChannelConfig = {
    .enableMasterIDReplication = true,
    .securityLevel             = kEDMA_ChannelSecurityLevelSecure,
    .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged,
};

hal_audio_dma_extra_config_t dmaExtraConfig = {
    .edmaExtraConfig.enableMasterIdReplication = true,
};

hal_audio_dma_config_t dmaTxConfig = {
    .instance            = DEMO_DMA_INDEX,
    .channel             = DEMO_DMA_TX_CHANNEL,
    .priority            = kHAL_AudioDmaChannelPriorityDefault,
    .dmaChannelMuxConfig = (void *)&dmaChannelSource,
    .dmaChannelConfig    = (void *)&edmaChannelConfig,
    .dmaExtraConfig      = (void *)&dmaExtraConfig,
};

hal_audio_ip_config_t ipTxConfig = {
    .sai.lineMask = 1U << 0U,
    .sai.syncMode = kHAL_AudioSaiModeAsync,
};

hal_audio_config_t audioTxConfig = {
    .dmaConfig         = &dmaTxConfig,
    .ipConfig          = &ipTxConfig,
    .instance          = DEMO_SAI_INSTANCE_INDEX,
    .srcClock_Hz       = DEMO_SAI_CLK_FREQ,
    .sampleRate_Hz     = (uint32_t)kHAL_AudioSampleRate48KHz,
    .masterSlave       = kHAL_AudioMaster,
    .bclkPolarity      = kHAL_AudioSampleOnRisingEdge,
    .frameSyncWidth    = kHAL_AudioFrameSyncWidthHalfFrame,
    .frameSyncPolarity = kHAL_AudioBeginAtFallingEdge,
    .dataFormat        = kHAL_AudioDataFormatI2sClassic,
    .fifoWatermark     = (uint8_t)(FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) - 1),
    .bitWidth          = (uint8_t)kHAL_AudioWordWidth16bits,
    .lineChannels      = kHAL_AudioStereo,
};

wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route =
        {
            .enableLoopBack            = false,
            .leftInputPGASource        = kWM8962_InputPGASourceInput1,
            .leftInputMixerSource      = kWM8962_InputMixerSourceInputPGA,
            .rightInputPGASource       = kWM8962_InputPGASourceInput3,
            .rightInputMixerSource     = kWM8962_InputMixerSourceInputPGA,
            .leftHeadphoneMixerSource  = kWM8962_OutputMixerDisabled,
            .leftHeadphonePGASource    = kWM8962_OutputPGASourceDAC,
            .rightHeadphoneMixerSource = kWM8962_OutputMixerDisabled,
            .rightHeadphonePGASource   = kWM8962_OutputPGASourceDAC,
        },
    .slaveAddress = WM8962_I2C_ADDR,
    .bus          = kWM8962_BusI2S,
    .format       = {.mclk_HZ    = 24576000U / 2,
               .sampleRate = kWM8962_AudioSampleRate48KHz,
               .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000)  / 2
 *                              = 393.216MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};
extern uint32_t USB_AudioSpeakerBufferSpaceUsed(void);
extern void USB_DeviceCalculateFeedback(void);
/*${variable:end}*/
/*${function:start}*/
void SEI_EAR_TRDC_EDMA3_ResetPermissions()
{
    uint8_t i, j;
    /* Set the master domain access configuration for eDMA3 */
    trdc_non_processor_domain_assignment_t edma3Assignment;
    (void)memset(&edma3Assignment, 0, sizeof(edma3Assignment));
    edma3Assignment.domainId = 0x7U;
    /* Use the bus master's privileged/user attribute directly */
    edma3Assignment.privilegeAttr = kTRDC_MasterPrivilege;
    /* Use the bus master's secure/nonsecure attribute directly */
    edma3Assignment.secureAttr = kTRDC_MasterSecure;
    /* Use the DID input as the domain indentifier */
    edma3Assignment.bypassDomainId = true;
    edma3Assignment.lock           = false;
    TRDC_SetNonProcessorDomainAssignment(TRDC1, kTRDC1_MasterDMA3, &edma3Assignment);

    /* Enable all access modes for MBC and MRC. */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC1, &hwConfig);

    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL |= BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL &= ~BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
}

void BOARD_InitHardware(void)
{
    uint32_t result = 0U;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Workaround to make SAI1 CLK Root output 12MHz*/
    CLOCK_InitAudioPll(&audioPllConfig);
    /*
     * Check ELE FW status
     */
    do
    {
        /*Wait TR empty*/
        while ((MU_RT_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
            ;
        /* Send Get FW Status command(0xc5), message size 0x01 */
        MU_RT_S3MUA->TR[0] = 0x17c50106;
        /*Wait RR Full*/
        while ((MU_RT_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
            ;
        (void)MU_RT_S3MUA->RR[0];
        /*Wait RR Full*/
        while ((MU_RT_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
            ;
        /* Get response code, only procedd when code is 0xD6 which is S400_SUCCESS_IND. */
        result = MU_RT_S3MUA->RR[1];
        /*Wait RR Full*/
        while ((MU_RT_S3MUA->RSR & MU_RSR_RF2_MASK) == 0)
            ;
        (void)MU_RT_S3MUA->RR[2];
    } while (result != 0xD6);

    /*
     * Send Release TRDC command
     */
    do
    {
        /*Wait TR empty*/
        while ((MU_RT_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
            ;
        /* Send release RDC command(0xc4), message size 2 */
        MU_RT_S3MUA->TR[0] = 0x17c40206;
        /*Wait TR empty*/
        while ((MU_RT_S3MUA->TSR & MU_TSR_TE1_MASK) == 0)
            ;
        /* Release TRDC A(TRDC1, 0x74) to the RTD core(cm33, 0x1) */
        MU_RT_S3MUA->TR[1] = 0x7401;
        /*Wait RR Full*/
        while ((MU_RT_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
            ;
        (void)MU_RT_S3MUA->RR[0];
        /*Wait RR Full*/
        while ((MU_RT_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
            ;
        result = MU_RT_S3MUA->RR[1];
    } while (result != 0xD6);

    /*
     * TRDC and Related Settings
     */
    SEI_EAR_TRDC_EDMA3_ResetPermissions();

    /*Clock setting for LPI2C*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0102, 0);

    /*Clock setting for SAI1*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, 2);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 32);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);
}

void BOARD_Codec_Init()
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, 50U) !=
        kStatus_Success)
    {
        assert(false);
    }
}
#if (defined(FSL_FEATURE_SOC_DMA4_COUNT) && (FSL_FEATURE_SOC_DMA4_COUNT > 0U))
AT_QUICKACCESS_SECTION_CODE(static void txCallback(hal_audio_handle_t handle,
                                                   hal_audio_status_t completionStatus,
                                                   void *callbackParam))
#else
static void txCallback(hal_audio_handle_t handle, hal_audio_status_t completionStatus, void *callbackParam)
#endif
{
    uint32_t audioSpeakerPreReadDataCount = 0U;
    uint32_t preAudioSendCount            = 0U;
    hal_audio_transfer_t xfer             = {0};

    if ((USB_AudioSpeakerBufferSpaceUsed() < (g_UsbDeviceAudioSpeaker.audioPlayTransferSize)) &&
        (g_UsbDeviceAudioSpeaker.startPlayFlag == 1U))
    {
        g_UsbDeviceAudioSpeaker.startPlayFlag          = 0;
        g_UsbDeviceAudioSpeaker.speakerDetachOrNoInput = 1;
    }
    if (0U != g_UsbDeviceAudioSpeaker.startPlayFlag)
    {
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
        USB_DeviceCalculateFeedback();
#endif
        xfer.dataSize     = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        xfer.data         = audioPlayDataBuff + g_UsbDeviceAudioSpeaker.tdReadNumberPlay;
        preAudioSendCount = g_UsbDeviceAudioSpeaker.audioSendCount[0];
        g_UsbDeviceAudioSpeaker.audioSendCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (preAudioSendCount > g_UsbDeviceAudioSpeaker.audioSendCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSendCount[1] += 1U;
        }
        g_UsbDeviceAudioSpeaker.audioSendTimes++;
        g_UsbDeviceAudioSpeaker.tdReadNumberPlay += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay >= g_UsbDeviceAudioSpeaker.audioPlayBufferSize)
        {
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay = 0;
        }
        audioSpeakerPreReadDataCount = g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0];
        g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (audioSpeakerPreReadDataCount > g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[1] += 1U;
        }
    }
    else
    {
        if (0U != g_UsbDeviceAudioSpeaker.audioPlayTransferSize)
        {
            xfer.dataSize = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        }
        else
        {
            xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
        }
        xfer.data = audioPlayDMATempBuff;
    }
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}

void AUDIO_DMA_EDMA_Start()
{
    usb_echo("Init Audio SAI and CODEC\r\n");
    hal_audio_transfer_t xfer = {0};
    memset(audioPlayDMATempBuff, 0, AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME);
    xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
    xfer.data     = audioPlayDMATempBuff;
    HAL_AudioTxInstallCallback((hal_audio_handle_t)&audioTxHandle[0], txCallback, NULL);
    HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audioTxHandle[0], &xfer);
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}

void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceAudioSpeaker.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    usbClockFreq = 24000000;
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, usbClockFreq);
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
/*${function:end}*/
