/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_audio.h"
#include "usb_audio_config.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "audio_generator.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include <stdio.h>
#include <stdlib.h>

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include "fsl_power.h"
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#include "fsl_sctimer.h"
#endif
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand, uint32_t *length, uint8_t **buffer);
extern void USB_AudioRecorderGetBuffer(uint8_t *buffer, uint32_t size);
#if defined(AUDIO_DATA_SOURCE_DMIC) && (AUDIO_DATA_SOURCE_DMIC > 0U)
extern void Board_DMIC_DMA_Init(void);
#endif
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
extern void SCTIMER_CaptureInit(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
static uint32_t eventCounterL = 0;
static uint32_t captureRegisterNumber;
static sctimer_config_t sctimerInfo;
#endif
extern usb_audio_generator_struct_t s_audioGenerator;
/* Audio data information */
extern uint8_t s_wavBuff[];

extern uint8_t g_UsbDeviceInterface[USB_AUDIO_GENERATOR_INTERFACE_COUNT];

extern usb_status_t USB_DeviceSetSpeed(uint8_t speed);

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_SetupOutBuffer[8];
/* Default value of audio generator device struct */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_audio_generator_struct_t s_audioGenerator = {
    .deviceHandle                           = NULL,
    .currentStreamInterfaceAlternateSetting = 0U,
    .copyProtect                            = 0x01U,
    .curMute                                = 0x01U,
    .curVolume                              = {0x00U, 0x80U},
    .minVolume                              = {0x00U, 0x80U},
    .maxVolume                              = {0xFFU, 0x7FU},
    .resVolume                              = {0x01U, 0x00U},
    .curBass                                = 0x00U,
    .minBass                                = 0x80U,
    .maxBass                                = 0x7FU,
    .resBass                                = 0x01U,
    .curMid                                 = 0x00U,
    .minMid                                 = 0x80U,
    .maxMid                                 = 0x7FU,
    .resMid                                 = 0x01U,
    .curTreble                              = 0x01U,
    .minTreble                              = 0x80U,
    .maxTreble                              = 0x7FU,
    .resTreble                              = 0x01U,
    .curAutomaticGain                       = 0x01U,
    .curDelay                               = {0x00U, 0x40U},
    .minDelay                               = {0x00U, 0x00U},
    .maxDelay                               = {0xFFU, 0xFFU},
    .resDelay                               = {0x00U, 0x01U},
    .curLoudness                            = 0x01U,
    .curSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .minSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .maxSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .resSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .speed                                  = USB_SPEED_FULL,
    .attach                                 = 0U,
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    .generatorIntervalCount = 0,
    .audioPllTicksPrev      = 0,
    .audioPllTicksDiff      = 0,
    .audioPllTicksEma       = AUDIO_PLL_USB1_SOF_INTERVAL_COUNT,
    .audioPllTickEmaFrac    = 0,
    .audioPllStep           = AUDIO_PLL_FRACTIONAL_CHANGE_STEP,
#endif
};

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
uint32_t pllIsChanged = 0;
void SCTIMER_SOF_TOGGLE_HANDLER()
{
    uint32_t currentSctCap = 0, pllCountPeriod = 0, pll_change = 0;
    static int32_t pllCount = 0, pllDiff = 0;
    static int32_t err, abs_err;
    if (SCTIMER_GetStatusFlags(SCT0) & (1 << eventCounterL))
    {
        /* Clear interrupt flag.*/
        SCTIMER_ClearStatusFlags(SCT0, (1 << eventCounterL));
    }

    if (s_audioGenerator.generatorIntervalCount != 100)
    {
        s_audioGenerator.generatorIntervalCount++;
        return;
    }
    s_audioGenerator.generatorIntervalCount = 1;
    currentSctCap                           = SCT0->CAP[0];
    pllCountPeriod                          = currentSctCap - s_audioGenerator.audioPllTicksPrev;
    s_audioGenerator.audioPllTicksPrev      = currentSctCap;
    pllCount                                = pllCountPeriod;
    if (s_audioGenerator.attach)
    {
        if (abs(pllCount - AUDIO_PLL_USB1_SOF_INTERVAL_COUNT) < (AUDIO_PLL_USB1_SOF_INTERVAL_COUNT >> 7))
        {
            pllDiff = pllCount - s_audioGenerator.audioPllTicksEma;
            s_audioGenerator.audioPllTickEmaFrac += (pllDiff % 8);
            s_audioGenerator.audioPllTicksEma += (pllDiff / 8) + s_audioGenerator.audioPllTickEmaFrac / 8;
            s_audioGenerator.audioPllTickEmaFrac = (s_audioGenerator.audioPllTickEmaFrac % 8);

            err     = s_audioGenerator.audioPllTicksEma - AUDIO_PLL_USB1_SOF_INTERVAL_COUNT;
            abs_err = abs(err);
            if (abs_err > s_audioGenerator.audioPllStep)
            {
                if (err > 0)
                {
                    s_audioGenerator.curAudioPllFrac -= abs_err / s_audioGenerator.audioPllStep;
                }
                else
                {
                    s_audioGenerator.curAudioPllFrac += abs_err / s_audioGenerator.audioPllStep;
                }
                pll_change = 1;
            }

            if (pll_change)
            {
                CLKCTL1->AUDIOPLL0NUM = s_audioGenerator.curAudioPllFrac;
                pllIsChanged          = 1;
            }
        }
    }
}

void SCTIMER_CaptureInit(void)
{
    INPUTMUX->SCT0_IN_SEL[eventCounterL] = 0xFU; /* 0xFU for USB1.*/
    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Switch to 16-bit mode */
    sctimerInfo.clockMode   = kSCTIMER_Input_ClockMode;
    sctimerInfo.clockSelect = kSCTIMER_Clock_On_Rise_Input_7;

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    if (SCTIMER_SetupCaptureAction(SCT0, kSCTIMER_Counter_L, &captureRegisterNumber, eventCounterL) == kStatus_Fail)
    {
        usb_echo("SCT Setup Capture failed!\r\n");
    }
    SCT0->EV[0].STATE = 0x1;
    SCT0->EV[0].CTRL  = (0x01 << 10) | (0x2 << 12);

    /* Enable interrupt flag for event associated with out 4, we use the interrupt to update dutycycle */
    SCTIMER_EnableInterrupts(SCT0, (1 << eventCounterL));

    /* Receive notification when event is triggered */
    SCTIMER_SetCallback(SCT0, SCTIMER_SOF_TOGGLE_HANDLER, eventCounterL);

    /* Enable at the NVIC */
    EnableIRQ(SCT0_IRQn);

    /* Start the L counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);
}
#endif

void USB_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_audioGenerator.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    /* enable USB IP clock */
    CLOCK_SetClkDiv(kCLOCK_DivPfc1Clk, 5);
    CLOCK_AttachClk(kXTALIN_CLK_to_USB_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivUsbHsFclk, 1);
    CLOCK_EnableUsbhsDeviceClock();
    RESET_PeripheralReset(kUSBHS_PHY_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_DEVICE_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_HOST_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_SRAM_RST_SHIFT_RSTn);
    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USBHS_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USBHS_SRAM);
    POWER_ApplyPD();

    CLOCK_EnableUsbhsPhyClock();

#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);

    /* the following code should run after phy initialization and should wait some microseconds to make sure utmi clock
     * valid */
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_UsbhsHost);
    /*  Wait until host_needclk de-asserts */
    while (SYSCTL0->USBCLKSTAT & SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_MASK)
    {
        __ASM("nop");
    }
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    USBHSH->PORTMODE |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_UsbhsHost);
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
}
#endif

/* USB device audio ISO OUT endpoint callback */
usb_status_t USB_DeviceAudioIsoOut(usb_device_handle deviceHandle,
                                   usb_device_endpoint_callback_message_struct_t *event,
                                   void *arg)
{
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)event;
    if ((s_audioGenerator.attach) &&
        (ep_cb_param->length ==
         ((USB_SPEED_HIGH == s_audioGenerator.speed) ? HS_ISO_IN_ENDP_PACKET_SIZE : FS_ISO_IN_ENDP_PACKET_SIZE)))
    {
        USB_AudioRecorderGetBuffer(s_wavBuff, (USB_SPEED_HIGH == s_audioGenerator.speed) ? HS_ISO_IN_ENDP_PACKET_SIZE :
                                                                                           FS_ISO_IN_ENDP_PACKET_SIZE);
        return USB_DeviceSendRequest(
            deviceHandle, USB_AUDIO_STREAM_ENDPOINT, s_wavBuff,
            (USB_SPEED_HIGH == s_audioGenerator.speed) ? HS_ISO_IN_ENDP_PACKET_SIZE : FS_ISO_IN_ENDP_PACKET_SIZE);
    }

    return kStatus_USB_Error;
}

usb_status_t USB_DeviceAudioGetControlTerminal(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error    = kStatus_USB_InvalidRequest;
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;

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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;

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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;

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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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

    return error;
}

usb_status_t USB_DeviceAudioSetRequestInterface(usb_device_handle handle,
                                                usb_setup_struct_t *setup,
                                                uint32_t *length,
                                                uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t entity_id  = (uint8_t)(setup->wIndex >> 0x08);

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
    uint8_t entity_id  = (uint8_t)(setup->wIndex >> 0x08);
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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;

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
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
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

    return error;
}

usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand, uint32_t *length, uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_Success;
    uint16_t volume;

    switch (audioCommand)
    {
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL:
            *buffer = &s_audioGenerator.curMute;
            *length = sizeof(s_audioGenerator.curMute);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL:
            *buffer = s_audioGenerator.curVolume;
            *length = sizeof(s_audioGenerator.curVolume);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL:
            *buffer = &s_audioGenerator.curBass;
            *length = sizeof(s_audioGenerator.curBass);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL:
            *buffer = &s_audioGenerator.curMid;
            *length = sizeof(s_audioGenerator.curMid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL:
            *buffer = &s_audioGenerator.curTreble;
            *length = sizeof(s_audioGenerator.curTreble);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL:
            *buffer = &s_audioGenerator.curAutomaticGain;
            *length = sizeof(s_audioGenerator.curAutomaticGain);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL:
            *buffer = s_audioGenerator.curDelay;
            *length = sizeof(s_audioGenerator.curDelay);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioGenerator.curSamplingFrequency;
            *length = sizeof(s_audioGenerator.curSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL:
            *buffer = s_audioGenerator.minVolume;
            *length = sizeof(s_audioGenerator.minVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL:
            *buffer = &s_audioGenerator.minBass;
            *length = sizeof(s_audioGenerator.minBass);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL:
            *buffer = &s_audioGenerator.minMid;
            *length = sizeof(s_audioGenerator.minMid);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL:
            *buffer = &s_audioGenerator.minTreble;
            *length = sizeof(s_audioGenerator.minTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL:
            *buffer = s_audioGenerator.minDelay;
            *length = sizeof(s_audioGenerator.minDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioGenerator.minSamplingFrequency;
            *length = sizeof(s_audioGenerator.minSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL:
            *buffer = s_audioGenerator.maxVolume;
            *length = sizeof(s_audioGenerator.maxVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL:
            *buffer = &s_audioGenerator.maxBass;
            *length = sizeof(s_audioGenerator.maxBass);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL:
            *buffer = &s_audioGenerator.maxMid;
            *length = sizeof(s_audioGenerator.maxMid);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL:
            *buffer = &s_audioGenerator.maxTreble;
            *length = sizeof(s_audioGenerator.maxTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL:
            *buffer = s_audioGenerator.maxDelay;
            *length = sizeof(s_audioGenerator.maxDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioGenerator.maxSamplingFrequency;
            *length = sizeof(s_audioGenerator.maxSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL:
            *buffer = s_audioGenerator.resVolume;
            *length = sizeof(s_audioGenerator.resVolume);
            break;
        case USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL:
            *buffer = &s_audioGenerator.resBass;
            *length = sizeof(s_audioGenerator.resBass);
            break;
        case USB_DEVICE_AUDIO_GET_RES_MID_CONTROL:
            *buffer = &s_audioGenerator.resMid;
            *length = sizeof(s_audioGenerator.resMid);
            break;
        case USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL:
            *buffer = &s_audioGenerator.resTreble;
            *length = sizeof(s_audioGenerator.resTreble);
            break;
        case USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL:
            *buffer = s_audioGenerator.resDelay;
            *length = sizeof(s_audioGenerator.resDelay);
            break;
        case USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL:
            *buffer = s_audioGenerator.resSamplingFrequency;
            *length = sizeof(s_audioGenerator.resSamplingFrequency);
            break;

        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL:
            s_audioGenerator.curVolume[0] = **(buffer);
            s_audioGenerator.curVolume[1] = *((*buffer) + 1);
            volume                        = (uint16_t)((uint16_t)s_audioGenerator.curVolume[1] << 8U);
            volume |= (uint8_t)(s_audioGenerator.curVolume[0]);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL:
            s_audioGenerator.curMute = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL:
            s_audioGenerator.curBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL:
            s_audioGenerator.curMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL:
            s_audioGenerator.curTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL:
            s_audioGenerator.curAutomaticGain = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL:
            s_audioGenerator.curDelay[0] = **(buffer);
            s_audioGenerator.curDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL:
            s_audioGenerator.curSamplingFrequency[0] = **(buffer);
            s_audioGenerator.curSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL:
            s_audioGenerator.minVolume[0] = **(buffer);
            s_audioGenerator.minVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL:
            s_audioGenerator.minBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL:
            s_audioGenerator.minMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL:
            s_audioGenerator.minTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL:
            s_audioGenerator.minDelay[0] = **(buffer);
            s_audioGenerator.minDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL:
            s_audioGenerator.minSamplingFrequency[0] = **(buffer);
            s_audioGenerator.minSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL:
            s_audioGenerator.maxVolume[0] = **(buffer);
            s_audioGenerator.maxVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL:
            s_audioGenerator.maxBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL:
            s_audioGenerator.maxMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL:
            s_audioGenerator.maxTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL:
            s_audioGenerator.maxDelay[0] = **(buffer);
            s_audioGenerator.maxDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL:
            s_audioGenerator.maxSamplingFrequency[0] = **(buffer);
            s_audioGenerator.maxSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL:
            s_audioGenerator.resVolume[0] = **(buffer);
            s_audioGenerator.resVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL:
            s_audioGenerator.resBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_MID_CONTROL:
            s_audioGenerator.resMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL:
            s_audioGenerator.resTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL:
            s_audioGenerator.resDelay[0] = **(buffer);
            s_audioGenerator.resDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL:
            s_audioGenerator.resSamplingFrequency[0] = **(buffer);
            s_audioGenerator.resSamplingFrequency[1] = *((*buffer) + 1);
            break;
        default:
            error = kStatus_USB_InvalidRequest;
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
    static uint32_t audioGeneratorSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&audioGeneratorSetup;
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
    if ((NULL == buffer) || ((*length) > sizeof(s_SetupOutBuffer)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = s_SetupOutBuffer;
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
 * @param handle           The USB device handle.
 * @param event            The USB device event type.
 * @param param            The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;
    uint8_t count      = 0U;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* The device BUS reset signal detected */
            for (count = 0U; count < USB_AUDIO_GENERATOR_INTERFACE_COUNT; count++)
            {
                s_audioGenerator.currentInterfaceAlternateSetting[count] = 0U;
            }
            s_audioGenerator.attach               = 0U;
            s_audioGenerator.currentConfiguration = 0U;
            USB_DeviceControlPipeInit(s_audioGenerator.deviceHandle);
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success ==
                USB_DeviceGetStatus(s_audioGenerator.deviceHandle, kUSB_DeviceStatusSpeed, &s_audioGenerator.speed))
            {
                USB_DeviceSetSpeed(s_audioGenerator.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_audioGenerator.attach               = 0U;
                s_audioGenerator.currentConfiguration = 0U;
            }
            else if (USB_AUDIO_GENERATOR_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                s_audioGenerator.attach               = 1U;
                s_audioGenerator.currentConfiguration = *temp8;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (s_audioGenerator.attach)
            {
                uint8_t interface        = (*temp8) & 0xFFU;
                uint8_t alternateSetting = g_UsbDeviceInterface[interface];

                if (s_audioGenerator.currentStreamInterfaceAlternateSetting != alternateSetting)
                {
                    if (s_audioGenerator.currentStreamInterfaceAlternateSetting)
                    {
                        USB_DeviceDeinitEndpoint(
                            s_audioGenerator.deviceHandle,
                            USB_AUDIO_STREAM_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT));
                    }
                    else
                    {
                        usb_device_endpoint_init_struct_t epInitStruct;
                        usb_device_endpoint_callback_struct_t epCallback;

                        epCallback.callbackFn    = USB_DeviceAudioIsoOut;
                        epCallback.callbackParam = handle;

                        epInitStruct.zlt          = 0U;
                        epInitStruct.transferType = USB_ENDPOINT_ISOCHRONOUS;
                        epInitStruct.endpointAddress =
                            USB_AUDIO_STREAM_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                        if (USB_SPEED_HIGH == s_audioGenerator.speed)
                        {
                            epInitStruct.maxPacketSize = HS_ISO_IN_ENDP_PACKET_SIZE;
                            epInitStruct.interval      = HS_ISO_IN_ENDP_INTERVAL;
                        }
                        else
                        {
                            epInitStruct.maxPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE;
                            epInitStruct.interval      = FS_ISO_IN_ENDP_INTERVAL;
                        }

                        USB_DeviceInitEndpoint(s_audioGenerator.deviceHandle, &epInitStruct, &epCallback);
                        USB_AudioRecorderGetBuffer(s_wavBuff, (USB_SPEED_HIGH == s_audioGenerator.speed) ?
                                                                  HS_ISO_IN_ENDP_PACKET_SIZE :
                                                                  FS_ISO_IN_ENDP_PACKET_SIZE);
                        error = USB_DeviceSendRequest(
                            s_audioGenerator.deviceHandle, USB_AUDIO_STREAM_ENDPOINT, s_wavBuff,
                            (USB_SPEED_HIGH == s_audioGenerator.speed) ? HS_ISO_IN_ENDP_PACKET_SIZE :
                                                                         FS_ISO_IN_ENDP_PACKET_SIZE);
                    }
                    s_audioGenerator.currentStreamInterfaceAlternateSetting = alternateSetting;
                }
            }
            break;

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

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    SCTIMER_CaptureInit();
#endif

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &s_audioGenerator.deviceHandle))
    {
        usb_echo("USB device failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device audio generator demo\r\n");
    }

    USB_DeviceIsrEnable();

    USB_DeviceRun(s_audioGenerator.deviceHandle);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach main clock to I3C */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 20);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 32);

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    /* attach AUDIO PLL clock to SCTimer input7. */
    CLOCK_AttachClk(kAUDIO_PLL_to_SCT_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 1);

    s_audioGenerator.curAudioPllFrac = CLKCTL1->AUDIOPLL0NUM;
#endif

#if defined(AUDIO_DATA_SOURCE_DMIC) && (AUDIO_DATA_SOURCE_DMIC > 0U)
    Board_DMIC_DMA_Init();
#endif

    APPInit();

    while (1)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(s_audioGenerator.deviceHandle);
#endif
    }
}
