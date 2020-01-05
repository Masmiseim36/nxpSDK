/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_sss_ftr.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "fsl_debug_console.h"

#include "usb_device_descriptor.h"
#include "virtual_com.h"
#include <PlugAndTrust_Pkg_Ver.h>


/*VV-IMP
KSDK has recently add implementation for
USB_OTG1_IRQHandler
USB_OTG2_IRQHandler
USB0_IRQHandler
As the implemetation of those ISRs are already here and has to be used from this file,
we rename those ISRs using a define to its name and undefine here
*/
#undef USB_OTG1_IRQHandler
#undef USB_OTG2_IRQHandler
#undef USB0_IRQHandler


#if defined(LPC_55x)
#include "fsl_power.h"
#endif
#if defined(CPU_LPC54018)
#include "fsl_power.h"
#endif
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
extern uint8_t USB_EnterLowpowerMode(void);
#endif
#include "pin_mux.h"
#include <ax_reset.h>
#include <se_reset_config.h>

#include "sm_timer.h" //+A71CH
#include "vcom2i2c.h" //+A71CH
#include "nxLog_VCOM.h"

#if defined(IMX_RT)
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#endif
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

void BOARD_DbgConsole_Deinit(void);
void BOARD_DbgConsole_Init(void);
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
* Variables
******************************************************************************/
extern usb_device_endpoint_struct_t g_UsbDeviceCdcVcomDicEndpoints[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig;
/* Data structure of virtual com device */
usb_cdc_vcom_struct_t s_cdcVcom;

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {
    (STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU, (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {
    (COUNTRY_SETTING >> 0U) & 0x00FFU, (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo;
/* Data buffer for receiving and sending*/
#if defined(IMX_RT)
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
//USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currSendBuf[DATA_BUFF_SIZE];
#else
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[2048]; //+ A71CH I2C
// USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currSendBuf[DATA_BUFF_SIZE]; //- A71CH I2C

#endif
volatile static uint32_t s_recvSize = 0;
volatile static uint32_t s_sendSize = 0;

/* USB device class information */
static usb_device_class_config_struct_t s_cdcAcmConfig[1] = {{
    USB_DeviceCdcVcomCallback,
    0,
    &g_UsbDeviceCdcVcomConfig,
}};

/* USB device class configuration information */
static usb_device_class_config_list_struct_t s_cdcAcmConfigList = {
    &s_cdcAcmConfig[0],
    &USB_DeviceCallback,
    1,
};

#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
volatile static uint8_t s_waitForDataReceive = 0;
volatile static uint8_t s_comOpen = 0;
#endif
/*******************************************************************************
* Code
******************************************************************************/
#if defined(IMX_RT)
/* The function sets the cacheable memory to shareable, this suggestion is referred from chapter 2.2.1 Memory regions, types and attributes in Cortex-M7 Devices, Generic User Guide */
void BOARD_ConfigUSBMPU()
{
    /* Disable I cache and D cache */
    SCB_DisableICache();
    SCB_DisableDCache();

    /* Disable MPU */
    ARM_MPU_Disable();
    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable, SubRegionDisable, Size)
     * API in core_cm7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
     *      Use MACROS defined in core_cm7.h: ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner noncache
     *     1             1           0           0              Normal             shareable       outer and inner noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in core_cm7.h.
     */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

#endif
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle);
}
#endif

#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_cdcVcom.deviceHandle);
}
#endif

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_IRQHandler(void)
{
   USB_DeviceKhciIsrFunction(s_cdcVcom.deviceHandle);
   /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
   exception return operation might vector to incorrect interrupt */
   __DSB();
}
#endif

#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(s_cdcVcom.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0) {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    /* enable USB IP clock */
#if defined(CPU_LPC54018)
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFroHfFreq());
#else
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbfsSrcFro, CLOCK_GetFroHfFreq());
#endif
#if defined(FSL_FEATURE_USB_USB_RAM) && (FSL_FEATURE_USB_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USB_USB_RAM; i++) {
        ((uint8_t *)FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, 0U);
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++) {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif

#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++) {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */
#endif
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif
/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif
/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    uint32_t len;
    uint8_t *uartBitmap;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;
    usb_status_t error = kStatus_USB_Error;
    usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
    acmReqParam = (usb_device_cdc_acm_request_param_struct_t *)param;
    epCbParam = (usb_device_endpoint_callback_message_struct_t *)param;
    switch (event) {
    case kUSB_DeviceCdcEventSendResponse: {
        if ((epCbParam->length != 0) && (!(epCbParam->length % g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize))) {
            /* If the last packet is the size of endpoint, then send also zero-ended packet,
                 ** meaning that we want to inform the host that we do not have any additional
                 ** data, so it can flush the output.
                 */
            error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_BULK_IN_ENDPOINT, NULL, 0);
        }
        else if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions)) {
            if ((epCbParam->buffer != NULL) || ((epCbParam->buffer == NULL) && (epCbParam->length == 0))) {
                /* User: add your own code for send complete event */
                /* Schedule buffer for next receive event */
                error = USB_DeviceCdcAcmRecv(handle,
                    USB_CDC_VCOM_BULK_OUT_ENDPOINT,
                    s_currRecvBuf,
                    g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
                s_waitForDataReceive = 1;
                USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
#endif
            }
        }
        else {
        }
    } break;
    case kUSB_DeviceCdcEventRecvResponse: {
        if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions)) {
            s_recvSize = epCbParam->length;

#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
            s_waitForDataReceive = 0;
            USB0->INTEN |= USB_INTEN_SOFTOKEN_MASK;
#endif
            if (!s_recvSize) {
                /* Schedule buffer for next receive event */
                error = USB_DeviceCdcAcmRecv(handle,
                    USB_CDC_VCOM_BULK_OUT_ENDPOINT,
                    s_currRecvBuf,
                    g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
                s_waitForDataReceive = 1;
                USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
#endif
            }
        }
    } break;
    case kUSB_DeviceCdcEventSerialStateNotif:
        ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
        error = kStatus_USB_Success;
        break;
    case kUSB_DeviceCdcEventSendEncapsulatedCommand:
        break;
    case kUSB_DeviceCdcEventGetEncapsulatedResponse:
        break;
    case kUSB_DeviceCdcEventSetCommFeature:
        if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue) {
            if (1 == acmReqParam->isSetup) {
                *(acmReqParam->buffer) = s_abstractState;
            }
            else {
                *(acmReqParam->length) = 0;
            }
        }
        else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue) {
            if (1 == acmReqParam->isSetup) {
                *(acmReqParam->buffer) = s_countryCode;
            }
            else {
                *(acmReqParam->length) = 0;
            }
        }
        else {
        }
        error = kStatus_USB_Success;
        break;
    case kUSB_DeviceCdcEventGetCommFeature:
        if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue) {
            *(acmReqParam->buffer) = s_abstractState;
            *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
        }
        else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue) {
            *(acmReqParam->buffer) = s_countryCode;
            *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
        }
        else {
        }
        error = kStatus_USB_Success;
        break;
    case kUSB_DeviceCdcEventClearCommFeature:
        break;
    case kUSB_DeviceCdcEventGetLineCoding:
        *(acmReqParam->buffer) = s_lineCoding;
        *(acmReqParam->length) = LINE_CODING_SIZE;
        error = kStatus_USB_Success;
        break;
    case kUSB_DeviceCdcEventSetLineCoding: {
        if (1 == acmReqParam->isSetup) {
            *(acmReqParam->buffer) = s_lineCoding;
        }
        else {
            *(acmReqParam->length) = 0;
        }
    }
        error = kStatus_USB_Success;
        break;
    case kUSB_DeviceCdcEventSetControlLineState: {
        s_usbCdcAcmInfo.dteStatus = acmReqParam->setupValue;
        /* activate/deactivate Tx carrier */
        if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
            acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
        }
        else {
            acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
        }

        /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
        if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
            acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
        }
        /* Com port of terminal tool running on PC is closed now */
        else {
            acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
        }

        /* Indicates to DCE if DTE is present or not */
        acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

        /* Initialize the serial state buffer */
        acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
        acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
        acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
        acmInfo->serialStateBuf[3] = 0x00;
        acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
        acmInfo->serialStateBuf[5] = 0x00;
        acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
        acmInfo->serialStateBuf[7] = 0x00;
        /* Notify to host the line state */
        acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
        /* Lower byte of UART BITMAP */
        uartBitmap = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
        uartBitmap[0] = acmInfo->uartState & 0xFFu;
        uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
        len = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
        if (0 == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState) {
            error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
            if (kStatus_USB_Success != error) {
                LOG_E("kUSB_DeviceCdcEventSetControlLineState error!");
            }
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
        }

        /* Update status */
        if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
            /*  To do: CARRIER_ACTIVATED */
        }
        else {
            /* To do: CARRIER_DEACTIVATED */
        }
        if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
            /* DTE_ACTIVATED */
            if (1 == s_cdcVcom.attach) {
                s_cdcVcom.startTransactions = 1;
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
                s_waitForDataReceive = 1;
                USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
                s_comOpen = 1;
                LOG_I("USB_APP_CDC_DTE_ACTIVATED\r\n");
#endif
            }
        }
        else {
            /* DTE_DEACTIVATED */
            if (1 == s_cdcVcom.attach) {
                s_cdcVcom.startTransactions = 0;
            }
        }
    } break;
    case kUSB_DeviceCdcEventSendBreak:
        break;
    default:
        break;
    }

    return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t *temp16 = (uint16_t *)param;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event) {
    case kUSB_DeviceEventBusReset: {
        s_cdcVcom.attach = 0;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
        /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
        if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_cdcVcom.speed)) {
            USB_DeviceSetSpeed(handle, s_cdcVcom.speed);
        }
#endif
    } break;
    case kUSB_DeviceEventSetConfiguration:
        if (param) {
            s_cdcVcom.attach = 1;
            s_cdcVcom.currentConfiguration = *temp8;
            if (USB_CDC_VCOM_CONFIGURE_INDEX == (*temp8)) {
                /* Schedule buffer for receive */
                USB_DeviceCdcAcmRecv(s_cdcVcom.cdcAcmHandle,
                    USB_CDC_VCOM_BULK_OUT_ENDPOINT,
                    s_currRecvBuf,
                    g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
            }
        }
        break;
    case kUSB_DeviceEventSetInterface:
        if (s_cdcVcom.attach) {
            uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
            uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
            if (interface < USB_CDC_VCOM_INTERFACE_COUNT) {
                s_cdcVcom.currentInterfaceAlternateSetting[interface] = alternateSetting;
            }
        }
        break;
    case kUSB_DeviceEventGetConfiguration:
        break;
    case kUSB_DeviceEventGetInterface:
        break;
    case kUSB_DeviceEventGetDeviceDescriptor:
        if (param) {
            error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
        }
        break;
    case kUSB_DeviceEventGetConfigurationDescriptor:
        if (param) {
            error =
                USB_DeviceGetConfigurationDescriptor(handle, (usb_device_get_configuration_descriptor_struct_t *)param);
        }
        break;
    case kUSB_DeviceEventGetStringDescriptor:
        if (param) {
            /* Get device string descriptor request */
            error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
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

    s_cdcVcom.speed = USB_SPEED_HIGH;
    s_cdcVcom.attach = 0;
    s_cdcVcom.cdcAcmHandle = (class_handle_t)NULL;
    s_cdcVcom.deviceHandle = NULL;

    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &s_cdcAcmConfigList, &s_cdcVcom.deviceHandle)) {
        LOG_E("USB device init failed");
    }
    else {
        LOG_I("Board is configured as virtual com port.");
        LOG_I("This makes the board as the bridge between PC and secure element");
#if defined(SCI2C)
        LOG_I("smCom : smComSCI2C");
#elif defined(T1oI2C)
        LOG_I("smCom : T=1 over I2C");
#else
        LOG_W("smCom : Unknown");
#endif

#if defined(SSS_HAVE_A71CH) && (SSS_HAVE_A71CH == 1)
        LOG_I("Secure Element : A71CH");
#elif defined(SSS_HAVE_A71CL) && (SSS_HAVE_A71CL == 1)
        LOG_I("Secure Element : A71CL");
#elif defined(SSS_HAVE_SE05X) && (SSS_HAVE_SE05X == 1)
        LOG_I("Secure Element : SE050");
#elif defined(SSS_HAVE_SE050_EAR_CH) && (SSS_HAVE_SE050_EAR_CH == 1)
        LOG_I("NXP Internal : SE050_CH");
#elif defined(SSS_HAVE_SE050_L) && (SSS_HAVE_SE050_L == 1)
        LOG_I("Secure Element : SE050_L");
#else
        LOG_W("Secure Element : Unknown");
#endif
        LOG_I("Package Version : " PLUGANDTRUST_PROD_NAME_VER_FULL);
        s_cdcVcom.cdcAcmHandle = s_cdcAcmConfigList.config->classHandle;
    }

    USB_DeviceIsrEnable();

    USB_DeviceRun(s_cdcVcom.deviceHandle);
}

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
void APPTask(void)
{
    state_vcom_read_write(&s_cdcVcom, &s_recvSize, s_currRecvBuf);
    state_vcom_2_i2c();
}

void state_vcom_usbLowPower()
{
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
    if ((s_waitForDataReceive)) {
        if (s_comOpen == 1) {
            /* Wait for all the packets been sent during opening the com port. Otherwise these packets may
                 * wake up the system.
                 */
            LOG_I("Waiting to enter lowpower ...\r\n");
            for (uint32_t i = 0U; i < 16000000U; ++i) {
                __ASM("NOP"); /* delay */
            }

            s_comOpen = 0;
        }
        LOG_I("Enter lowpower\r\n");
        BOARD_DbgConsole_Deinit();
        USB0->INTEN &= ~USB_INTEN_TOKDNEEN_MASK;
        USB_EnterLowpowerMode();

        s_waitForDataReceive = 0;
        USB0->INTEN |= USB_INTEN_TOKDNEEN_MASK;
        BOARD_DbgConsole_Init();
        LOG_I("Exit  lowpower\r\n");
    }
#endif
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
#if defined(FRDM_KW41Z) || defined(FRDM_K64F)
    BOARD_BootClockRUN();
#endif
#if defined(FRDM_K82F)
    BOARD_BootClockHSRUN();
#endif

#ifdef FREEDOM
    BOARD_InitPins();
    BOARD_InitDebugConsole();

    LED_BLUE_INIT(1);
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);
#endif

#if defined(IMX_RT)

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#endif
#ifdef CPU_LPC54018
    /* Init board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_USART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

    /* reset USB0 and USB1 device */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    BOARD_InitBootPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole(); /* Initialize the debug console as CDC virtual com. */
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_Usbh1);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    /* enable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
#endif
#endif //CPU_LPC54018
#ifdef FRDM_K64F
    axReset_HostConfigure();
    axReset_PowerUp();
#endif
#ifdef LPC_55x
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM8 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB0 Phy */
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY); /*< Turn on USB1 Phy */

    /* reset the IP to make sure it's in reset state. */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);

#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
#endif
    axReset_HostConfigure();
    axReset_PowerUp();
#endif
    sm_initSleep();

    APPInit();

    while (1) {
        APPTask();

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(s_cdcVcom.deviceHandle);
#endif
    }
}
