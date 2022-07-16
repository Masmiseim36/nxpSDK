/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - USB Example Implementation
 */

#ifndef _USB_DEVICE_CONFIG_H_
#define _USB_DEVICE_CONFIG_H_

/* getr processor feature options */
#include "fsl_device_registers.h"

/*******************************************************************************
* Definitions
******************************************************************************/
/*!
 * @addtogroup usb_device_configuration
 * @{
 */

/*!
 * @name Hardware instance define
 * @{
 */

/* Board specific exceptions */

#ifdef FRDM_K66F
/* K66F has both KHCI and EHCI, the FRDM-K66F has the EHCI connected */
#undef FSL_FEATURE_USB_KHCI_COUNT
#endif

/* Determine USB configuration automatically from the processor-specifc feature options */

#if defined(FSL_FEATURE_USB_KHCI_COUNT) && FSL_FEATURE_USB_KHCI_COUNT>0

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB0_IRQHandler

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USB_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerKhci0

    /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (1U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

#elif defined(FSL_FEATURE_USBHS_EHCI_COUNT) && FSL_FEATURE_USBHS_EHCI_COUNT>0

    /*! brief i.MXRT USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USBHS_IRQHandler /* this is redefined to USB_OTG1_IRQHandler on some platforms */

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USBHS_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerEhci0

    /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (0U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (1U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

#elif defined(CPU_LPC54628J512ET180) || defined(LPC51U68_SERIES) || defined(LPC54018_SERIES) || \
      defined(LPC54608_SERIES) || defined(LPC54618_SERIES) || \
      defined(LPC54S018_SERIES) || defined(LPC54S018M_SERIES) || \
      defined(LPC54114_cm4_SERIES) || defined(LPC54114_cm0plus_SERIES)

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB0_IRQHandler

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USB_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0

    /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (0U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (1U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

#elif defined(CPU_LPC55S69JBD100_cm33_core0) || defined(CPU_LPC55S28JBD100) || defined(CPU_LPC55S36JBD100) || \
      defined(CPU_LPC55S16JBD100)

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB0_IRQHandler

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USB_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0

    /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (0U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (1U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

#elif defined(MC56F83789_SERIES) || defined(MC56F83689_SERIES)

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB_IRQHandler

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerKhci0

    /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (1U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

#elif defined(MIMXRT595S_cm33_SERIES)

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB0_IRQHandler

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USBHSD_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0

        /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (0U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (1U)

#elif defined(MIMXRT685S_cm33_SERIES)

    /*! brief USB interrupt vector name */
    #define USB_MCU_INT_HANDLER USB_IRQHandler

    /*! brief USB interrupt vectors indexes */
    #define USB_IRQS_LIST USBHSD_IRQS

    /*! brief Controller Identificator */
    #define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0

        /*! brief Controller Peripheral Index */
    #define USB_CONTROLLER_IX 0

    /*! @brief KHCI instance count */
    #define USB_DEVICE_CONFIG_KHCI (0U)

    /*! @brief EHCI instance count */
    #define USB_DEVICE_CONFIG_EHCI (0U)

    /*! @brief LPC USB IP3511 FS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

    /*! @brief LPC USB IP3511 HS instance count */
    #define USB_DEVICE_CONFIG_LPCIP3511HS (1U)

#else
#error Add the new MCU here!
#endif


/*! @brief Device instance count, the sum of KHCI and EHCI instance counts*/
#define USB_DEVICE_CONFIG_NUM \
    (USB_DEVICE_CONFIG_KHCI + USB_DEVICE_CONFIG_EHCI + USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS)

/* @} */


/*!
 * @name class instance define
 * @{
 */

/*! @brief HID instance count */
#define USB_DEVICE_CONFIG_HID (0U)

/*! @brief CDC ACM instance count */
#define USB_DEVICE_CONFIG_CDC_ACM (1U)

/*! @brief MSC instance count */
#define USB_DEVICE_CONFIG_MSC (0U)

/*! @brief Audio instance count */
#define USB_DEVICE_CONFIG_AUDIO (0U)

/*! @brief PHDC instance count */
#define USB_DEVICE_CONFIG_PHDC (0U)

/*! @brief Video instance count */
#define USB_DEVICE_CONFIG_VIDEO (0U)

/*! @brief CCID instance count */
#define USB_DEVICE_CONFIG_CCID (0U)

/*! @brief Printer instance count */
#define USB_DEVICE_CONFIG_PRINTER (0U)

/*! @brief DFU instance count */
#define USB_DEVICE_CONFIG_DFU (0U)

/* @} */

/*! @brief Whether device is self power. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_SELF_POWER (1U)

/*! @brief How many endpoints are supported in the stack. */
#define USB_DEVICE_CONFIG_ENDPOINTS (4U)

/*! @brief Whether the device task is enabled. */
#define USB_DEVICE_CONFIG_USE_TASK (0U)

/*! @brief How many the notification message are supported when the device task is enabled. */
#define USB_DEVICE_CONFIG_MAX_MESSAGES (8U)

/*! @brief Whether test mode enabled. */
#define USB_DEVICE_CONFIG_USB20_TEST_MODE (0U)

/*! @brief Whether device CV test is enabled. */
#define USB_DEVICE_CONFIG_CV_TEST (0U)

/*! @brief Whether device compliance test is enabled. If the macro is enabled,
    the test mode and CV test macroes will be set.*/
#define USB_DEVICE_CONFIG_COMPLIANCE_TEST (0U)

#if ((defined(USB_DEVICE_CONFIG_COMPLIANCE_TEST)) && (USB_DEVICE_CONFIG_COMPLIANCE_TEST > 0U))

/*! @brief Undefine the macro USB_DEVICE_CONFIG_USB20_TEST_MODE. */
#undef USB_DEVICE_CONFIG_USB20_TEST_MODE
/*! @brief Undefine the macro USB_DEVICE_CONFIG_CV_TEST. */
#undef USB_DEVICE_CONFIG_CV_TEST

/*! @brief enable the test mode. */
#define USB_DEVICE_CONFIG_USB20_TEST_MODE (1U)

/*! @brief enable the CV test */
#define USB_DEVICE_CONFIG_CV_TEST (1U)

#endif

#if ((defined(USB_DEVICE_CONFIG_KHCI)) && (USB_DEVICE_CONFIG_KHCI > 0U))

/*! @brief The MAX buffer length for the KHCI DMA workaround.*/
#define USB_DEVICE_CONFIG_KHCI_DMA_ALIGN_BUFFER_LENGTH (64U)
#endif

#if ((defined(USB_DEVICE_CONFIG_EHCI)) && (USB_DEVICE_CONFIG_EHCI > 0U))
/*! @brief How many the DTD are supported. */
#define USB_DEVICE_CONFIG_EHCI_MAX_DTD (16U)

/*! @brief Whether the EHCI ID pin detect feature enabled. */
#define USB_DEVICE_CONFIG_EHCI_ID_PIN_DETECT (0U)
#endif

/*! @brief Whether the keep alive feature enabled. */
#define USB_DEVICE_CONFIG_KEEP_ALIVE_MODE (0U)

/*! @brief Whether the transfer buffer is cache-enabled or not. */
#ifndef USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE
#define USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE (0U)
#endif
/*! @brief Whether the low power mode is enabled or not. */
#define USB_DEVICE_CONFIG_LOW_POWER_MODE (0U)

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
/*! @brief Whether device remote wakeup supported. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_REMOTE_WAKEUP (0U)

/*! @brief Whether LPM is supported. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_LPM_L1 (0U)
#else
/*! @brief The device remote wakeup is unsupported. */
#define USB_DEVICE_CONFIG_REMOTE_WAKEUP (0U)
#endif

/*! @brief Whether the device detached feature is enabled or not. */
#define USB_DEVICE_CONFIG_DETACH_ENABLE (0U)

/*! @brief Whether handle the USB bus error. */
#define USB_DEVICE_CONFIG_ERROR_HANDLING (0U)

/* @} */

#endif /* _USB_DEVICE_CONFIG_H_ */
