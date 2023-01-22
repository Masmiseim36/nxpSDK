
/**
 *  \file BT_features.h
 *
 *  \brief EtherMind Compilation Switches Configuration File.
 *
 *  This file lists all the Compilation Flags available in various
 *  EtherMind Bluetooth Stack modules.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_FEATURES_
#define _H_BT_FEATURES_

/* ----------------------------------------------------------------------- */
/* ========== Platform =================================================== */
/* ----------------------------------------------------------------------- */
/*
 *  FREERTOS
 *
 *  This flag is used when the EtherMind stack is being compiled on
 *  FreeRTOS platforms.
 *
 *  Dependency: None.
 */
#define FREERTOS

/*
 *  FREERTOS_WIN_EMULATION
 *
 *  This flag is used when the EtherMind stack is being compiled on
 *  Windows platforms and FreeRtos Emulation.
 *
 *  Dependency: None.
 */
/* #define FREERTOS_WIN_EMULATION */

/*
 *  FREERTOS_ENABLE_TRACE_UTILITY
 *
 *  This flag is used when the Enable the TRACE utility provided by FREERTOS
 *
 *  Dependency: None.
 */
/* #define FREERTOS_ENABLE_TRACE_UTILITY */

/*
 *  HAVE_CONDWAIT_WO_MUTEX
 *
 *  This flag is enabled if the OS does not require/support POSIX style
 *  atomic conditional wait.
 *
 *  Note: POSIX requires a Mutex along with Conditional Variable.
 *
 *  Dependency: None.
 */
/* #define HAVE_CONDWAIT_WO_MUTEX */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth Specification Flags ==================================== */
/* ----------------------------------------------------------------------- */
/*
 *  BT_1_2
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 1.2.
 *
 *  Currently, this flag only affects the L2CAP and HCI implementation.
 *  The appropriate source code must be chosen for compilation
 *  (the l2cap_1.2 & hci_1.2 source code directories).
 *
 *  The BT_1_2 flag is split into BT_HCI_1_2 and BT_L2CAP_1_2 so as to
 *  provide option for application to include Bluetooth v1.2 specific
 *  features for HCI & L2CAP individually.
 *
 *  If BT_1_2 is defined, then both BT_L2CAP_1_2 & HCI_1_2 gets defined
 *  automatically in BT_common.h
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 */
#define BT_L2CAP_1_2
#define BT_HCI_1_2

/*
 *  BT_2_0_EDR
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification v2.0+EDR.
 *
 *  Currently, this flag only affects the HCI implementation, source code
 *  for which is available under hci_1.2 directory.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 & BT_HCI_1_2 must be defined.
 */
#define BT_2_0_EDR

/*
 *  BT_2_1_EDR
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 2.1+EDR.
 *
 *  Currently, this flag only affects the L2CAP, HCI and SM implementation.
 *  The appropriate source code must be chosen for compilation
 *  (the l2cap_1.2 & hci_1.2 source code directories).
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  Both BT_1_2 & BT_2_0_EDR must be defined.
 */
#define BT_2_1_EDR

/*
 *  BT_CSA_1
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth Core Specification Addendum 1 (CSA1).
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  The BT_1_2 and BT_2_0_EDR must be defined.
 *  The BT_2_1_EDR may be defined.
 */
#define BT_CSA_1

/*
 *  BT_3_0
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 3.0.
 *
 *  Currently, this flag only affects the L2CAP and HCI implementation.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  BT_1_2, BT_2_0_EDR & BT_2_1_EDR must be defined.
 */
#define BT_3_0

/*
 *  BT_4_0
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 4.0.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR & BT_3_0 must be defined.
 */
#define BT_4_0

/*
 *  BT_4_1
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 4.1.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR, BT_3_0 & BT_4_0 must be defined.
 */
#define BT_4_1


/*
 *  BT_4_2
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 4.2.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR, BT_3_0, BT_4_0 & BT_4_1 must be defined.
 */
#define BT_4_2

/*
 *  BT_5_0
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 5.0.
 *
 *  Dependency
 *  ----------
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR, BT_3_0, BT_4_0, BT_4_1 & BT_4_2 must be defined.
 */
#define BT_5_0

/*
 *  BT_5_1
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 5.1.
 *
 *  Dependency
 *  ----------
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR, BT_3_0, BT_4_0, BT_4_1, BT_4_2 &
 *  BT_5_0 must be defined.
 */
#define BT_5_1

/*
 *  BT_5_2
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 5.2.
 *
 *  Dependency
 *  ----------
 *  BT_1_2, BT_2_0_EDR, BT_2_1_EDR, BT_3_0, BT_4_0, BT_4_1, BT_4_2,
 *  BT_5_0 & BT_5_1 must be defined.
 */
#define BT_5_2

/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v4.0 Feature Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef BT_4_0
#ifndef CFG_CLASSIC
/*
 *  BT_LE
 *
 *  This flag is used to enabling Low Energy support of stack
 *
 *  Dependency: BT_4_0 must be defined.
 */
#define BT_LE
#endif /* CFG_CLASSIC */

#ifdef CFG_BLE
/*
 *  BT_SINGLE_MODE
 *
 *  This flag is used to enabling Single Mode support of stack
 *
 *  Dependency: BT_4_0 must be defined.
 */
#define BT_SINGLE_MODE
#else /* CFG_BLE */

/*
 *  BT_DUAL_MODE
 *
 *  This flag is used to enabling Dual Mode support of stack
 *
 *  Dependency: BT_2_1_EDR and BT_4_0 must be defined.
 */
#define BT_DUAL_MODE
#endif /* CFG_BLE */

#if ((defined BT_SINGLE_MODE) && (defined BT_DUAL_MODE))
#error "Only one of BT_SINGLE_MODE, BT_DUAL_MODE shall be defined"
#endif /* ((defined BT_SINGLE_MODE) && (defined BT_DUAL_MODE)) */

/*
 * Only one of the following modes shall be defined
 * - BT_SINGLE_MODE
 * - BT_DUAL_MODE
 *
 * BT_DUAL_MODE is defined by default.
 *
 * If BT_SINGLE_MODE is defined, following feature flags
 * are typically disabled
 * - BT_2_1_EDR
 * - BT_CSA_1
 * - BT_3_0
 * - BT_DUAL_MODE
 */
#ifdef BT_SINGLE_MODE
#undef BT_2_1_EDR
#undef BT_CSA_1
#undef BT_3_0
#undef BT_DUAL_MODE
#endif /* BT_SINGLE_MODE */

/*
 * Enable Classic HCI and L2CAP for Dual Mode
 */
#ifdef BT_DUAL_MODE
#define BR_EDR_HCI
#define BR_EDR_L2CAP
#endif /* BT_DUAL_MODE */

#endif /* BT_4_0 */

/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v5.2 Feature Flags ==================================== */
/* ----------------------------------------------------------------------- */

#ifdef BT_5_2
/*
 *  BT_EATT
 *
 *  This flag is used to enable support for EATT
 *  feature of Bluetooth v5.2 Specification.
 *
 *  Dependency: BT_5_2 must be defined.
 */
#define BT_EATT
#endif /* BT_5_2 */

/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v4.1 Feature Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef BR_EDR_HCI
#ifdef BT_4_1
/*
 *  BT_BRSC
 *
 *  This flag is used to enable support for BR/EDR Secure Connections
 *  feature of Bluetooth Specification v4.1.
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define BT_BRSC
#endif /* BT_4_1 */

/*
 *  BT_BRSC_TEST
 *
 *  This flag is used to enable support for BR/EDR Secure Connections Test Mode
 *  Command.
 *
 *  Dependency: BT_BRSC must be defined.
 */
#ifdef BT_BRSC
#define BT_BRSC_TEST
#endif /* BT_BRSC */
#endif /* BR_EDR_HCI */

/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v3.0 Feature Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef BT_3_0

/*
 *  BT_UCD
 *
 *  This flag is used to enable support for Unicast Connectionless
 *  Data (UCD) feature of Bluetooth Specification v3.0.
 *
 *  Dependency: BT_3_0 must be defined.
 */
/* #define BT_UCD */

#endif /* BT_3_0 */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth Seattle Release Feature Flags ========================== */
/* ----------------------------------------------------------------------- */
#ifdef BT_CSA_1

/*
 *  BT_ENH_L2CAP
 *
 *  This flag enables the Enhanced L2CAP implementation.
 *
 *  Dependency: BT_CSA_1 must be defined.
 */
#define BT_ENH_L2CAP

#endif /* BT_CSA_1 */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v2.1+EDR Feature Flags ================================= */
/* ----------------------------------------------------------------------- */
#ifdef BT_2_1_EDR

/*
 *  BT_EIR
 *
 *  This flag is used to enabling support for Enhanced Inquiry Response (EIR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EIR

/*
 *  BT_EPR
 *
 *  This flag is used to enabling support for Encryption Pause/Resume (EPR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EPR

/*
 *  BT_EDR
 *
 *  This flag is used to enabling support for Enhanced Data Reporting (EDR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EDR

/*
 *  BT_LSTO
 *
 *  This flag is used to enabling support for Link Supervision Timeout (LSTO)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_LSTO

/*
 *  BT_2_1_QOS
 *  BT_PBF
 *
 *  This flag is used to enabling support for Packet Boundary Flag (PBF) and
 *  Enhanced Flush feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_2_1_QOS

#ifdef BT_2_1_QOS
#define BT_PBF
#define BT_ENHANCED_FLUSH
#endif /* BT_2_1_QOS */

/*
 *  BT_SSR
 *
 *  This flag is used to enabling support for Sinff Subrating (SSR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_SSR

/*
 *  BT_SSP
 *
 *  This flag is used to enabling support for Secure Simple Pairing (EIR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_SSP

/*
 *  BT_SSP_DEBUG
 *
 *  This flag is used to enable support for Simple Pairing Debug Mode
 *  Command, which can be used to configure the Controller to use a predefined
 *  DH private key for SSP to enable debug equipments monitor the data over
 *  an encrypted connection.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_DEBUG

#endif /* BT_2_1_EDR */

/*
 *  HCI_WRITE_LE_HOST_SUPPORT
 *
 *  This flag is used to enable support for Write_LE_Host_Support command.
 *
 *  Dependency: None.
 */
#define HCI_WRITE_LE_HOST_SUPPORT

/*
 *  HCI_READ_LE_HOST_SUPPORT
 *
 *  This flag is used to enable support for Read_LE_Host_Support command.
 *
 *  Dependency: None.
 */
#define HCI_READ_LE_HOST_SUPPORT

/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v2.1+EDR Secure Simple Pairing Feature Flags =========== */
/* ----------------------------------------------------------------------- */
/*
 *  CLASSIC_SEC_MANAGER
 *
 *  This flag is used to enabling BR/EDR Security Manager
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#ifdef BT_DUAL_MODE
#define CLASSIC_SEC_MANAGER
#endif /* BT_DUAL_MODE */

#ifdef BT_SSP

/*
 *  BT_SSP_JW
 *
 *  This flag is used for enabling support for Just Works Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
/* #define BT_SSP_JW */

/*
 *  BT_SSP_NC
 *
 *  This flag is used for enabling support for Numeric Comparison Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_NC

/*
 *  BT_SSP_UC
 *
 *  This flag is used for enabling support for User Confirmation Request or
 *  Response handling in HCI & SM.
 *
 *  Dependency: either BT_SSP_JW or BT_SSP_NC must be defined.
 */
#if defined BT_SSP_JW || defined BT_SSP_NC
#define BT_SSP_UC
#endif /* BT_SSP_JW || BT_SSP_NC */

#if defined BT_SSP_JW && defined BT_SSP_NC
#error "Please define either BT_SSP_JW or BT_SSP_NC, not both."
#endif /* BT_SSP_JW && BT_SSP_NC */

/*
 *  BT_SSP_OOB
 *
 *  This flag is used for enabling support for Out of Band Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_OOB

/*
 *  BT_SSP_PE
 *
 *  This flag is used for enabling support for Passkey Entry Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_PE

#endif /* BT_SSP */

#ifdef BT_DUAL_MODE
/*
 *  BTSIG_ERRATA_11838
 *
 *  This flag is used to enable the Bluetooth SIG security errata 11838
 *  at https://www.bluetooth.org/docman/handlers/DownloadDoc.ashx?doc_id=470741
 *  for the Knob Attack (https://knobattack.com/)
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BTSIG_ERRATA_11838
#endif /* BT_DUAL_MODE */

/* ----------------------------------------------------------------------- */
/* ==== Stack Architecture Flags ========================================= */
/* ----------------------------------------------------------------------- */
/*
 *  BT_ARCH_1_2
 *
 *  This flag is used when the EtherMind stack is being compiled for Bluetooth
 *  specification version 1.2 specific architectural changes.
 *
 *  Currently, this flag only affects the L2CAP and HCI implementation,
 *  and HCI-L2CAP internal interface.
 *
 *  If the source code of HCI and L2CAP from directories hci_1.2 and l2cap_1.2
 *  are used, then BT_ARCH_1_2 is mandatory and must be enabled.
 *
 *  If the source code of HCI and L2CAP from directories hci and l2cap are used,
 *  then this flag must not be enabled.
 *
 *  It should be noted that BT_ARCH_1_2 has nothing to do with Bluetooth
 *  Specification v1.2. It only enables architectural and interface change
 *  that is done in EtherMind HCI & L2CAP modules.
 *
 *  The hci_1.2 and l2cap_1.2 source codes are compliant to both Bluetooth
 *  specification version 1.1 and 1.2:
 *  - For Bluetooth v1.2 stack, both BT_ARCH_1_2 & BT_1_2 must be enabled.
 *  - For Bluetooth v1.1 stack, only BT_ARCH_1_2 must be enabled.
 *
 *  Dependency: None.
 */
#define BT_ARCH_1_2

/*
 *  This flag is used to perform the EtherMind Read Task's job using the
 *  EtherMind Write Task, and hence only one single task is created and used.
 *  For details on EtherMind Tasks and their usage/function, refer to
 *  EtherMind Stack Architecture Document, and EtherMind Programmer's Guide.
 *
 *  Dependency: None.
 */
/* #define BT_TRANSPORT_SINGLE_TASK */

/*
 *  BT_NO_STATIC_DATA_SIZE
 *
 *  This flag removes the usage of static buffers of size
 *  'BT_STATIC_DATA_SIZE' from the EtherMind stack.
 *
 *  Dependency: None.
 */
/* #define BT_NO_STATIC_DATA_SIZE */

/*
 *  BT_SUPPORT_CONTROLLER_INIT_SKIP
 *
 *  This flag enables support for API to enable/disable
 *  controller initialization during Bluetooth ON.
 *
 *  The default behaviour of stack will be to initialize the
 *  controller.
 *
 *  Dependency: None.
 */
#define BT_SUPPORT_CONTROLLER_INIT_SKIP

/*
 *  BT_SUPPORT_GET_STACK_INIT_STATE
 *
 *  This flag enables support for BT_bluetooth_get_stack_init_state API
 *  used to get initialization state of Stack.
 *
 *  Dependency: None.
 */
#define BT_SUPPORT_GET_STACK_INIT_STATE


/* ----------------------------------------------------------------------- */
/* ==== HCI Transport Flags ============================================== */
/* ----------------------------------------------------------------------- */
/*
 *  HCI_TX_RUN_TIME_SELECTION
 *
 *  This flag is used to enable runtime registration of any of the transports
 *  that are enabled below.
 *
 *  Dependency: None.
 */
/* #define HCI_TX_RUN_TIME_SELECTION */

/*
 *  BT_UART, BT_USB, BT_BCSP, BT_SOCKET, BT_SW_TXP
 *
 *  These flags are used to choose the HCI Transport module for communication
 *  with the Bluetooth hardware, in a hosted Stack.
 *
 *  For some platforms, these flags are defined in Makefiles & Workspaces.
 *  In such cases, these must not be enabled here.
 *
 *  Dependency: None.
 */

#if defined BT_PLATFORM
#elif defined BT_USB
#elif defined BT_BCSP
#elif defined BT_SOCKET
#elif defined BT_SW_TXP
#else
  #define BT_UART
#endif

/*
 *  HCI_LIBUSB_SUPPORT
 *
 *  This flag enables user space USB driver 'libusb' support.
 *
 *  Dependency: BT_USB
 */
#ifdef BT_USB
#define HCI_LIBUSB_SUPPORT
#endif /* BT_USB */

/*
 *  BT_CUSTOM_SERIAL_SETTINGS
 *
 *  This flag enables application to make use of hci_uart_set_serial_settings()
 *  API (defined in BT_api.h), to configure the COM/Serial Port (and, Baud
 *  Rate) on which the Bluetooth hardware is available or connected.
 *
 *  Dependency: The BT_UART must be defined.
 */
#define BT_CUSTOM_SERIAL_SETTINGS

/*
 *  HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
 *
 *  This flag enables application to instruct HCI-UART module to recombine
 *  fragments of a HCI Command or Data packet before the same is send over
 *  to the underlying Bluetooth Controller.
 *
 *  Dependency: The BT_UART must be defined.
 */
#ifdef BT_UART
#define HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
#endif /* BT_UART */

/*
 *  HT_DUMP_RX_BUF_ON_SYNC_LOSS
 *
 *  This flag enables dumping the entire transport receive buffer
 *  to help in analysing when synchronization loss is detected.
 *
 *  Dependency: None.
 */
#define HT_DUMP_RX_BUF_ON_SYNC_LOSS

/*
 *  HT_ENQUEUE_WITH_RETURN
 *
 *  This flag enables hci_transport_enqueue() function to return the
 *  number of bytes successfully enqueued into the HT buffer.
 *
 *  Dependency: None.
 */
#define HT_ENQUEUE_WITH_RETURN

/*
 *  HT_THREAD_SAFE_ENQUEUE
 *
 *  Using hci_transport_enqueue() interface, HCI transport driver provides
 *  HCI Events and Data packets from HCI Controller as byte stream to
 *  the EtherMind stack. Typically, hci_transport_enqueue() is called from
 *  a single thread/execution context.
 *
 *  If in some scenario, the hci_transport_enqueue() can be called from more
 *  than one thread/execution context, like in the case of 'libusb' based
 *  implementation, where the HCI Event and Data packets are received from two
 *  separate threads on USB 'Interrupt' and 'Bulk In' endpoints, thread safe
 *  version of hci_transport_enqueue() shall be enabled, using this feature
 *  flag.
 */
#define HT_THREAD_SAFE_ENQUEUE

/*
 *  HT_ENABLE_INVALID_RX_IND
 *
 *  If this flag is enabled and HCI error indication handler is registered
 *  by the application, using BT_hci_register_error_indication_callback(),
 *  application will be informed about the invalid octet reception over
 *  HCI transport, through HCI error indication callback.
 *
 *  In the corresponding HCI error indication callback, the last parameter
 *  will be result/error code HCI_RX_INVALID_PKT_TYPE. And the first parameter
 *  will be the received invalid packet type octet.
 *
 *  If the received packet type is processed by the application and wants
 *  the stack to ignore the reception of invalid packet type, the HCI error
 *  indication callback shall return API_SUCCESS. Any other return value
 *  will cause the HCI transport layer to consider this as synchronization loss.
 */
/* #define HT_ENABLE_INVALID_RX_IND */

/*
 *  HCI_CHECK_TARNSPORT_READINESS
 *
 *  This flag when enabled, exposes HCI interface to set the readiness of the
 *  underlying transport in case any initialization of transport is required
 *  during stack startup. For example, when BCSP is used as transport, before
 *  Bluetooth ON procedure, the transport link establishment procedure should
 *  take place. This interface can be used to notify the HCI layer when the link
 *  establishment procedure starts and completes.
 *
 *  Dependency: BT_BCSP
 */
#ifdef BT_BCSP
#define HCI_CHECK_TARNSPORT_READINESS
#endif /* BT_BCSP */

/*
 *  HT_ENABLE_PARSE_UTIL
 *
 *  This flag when enabled, exposes interfaces to parse partially received
 *  HCI packets from the transport. This helps in finding out the remaining
 *  lengths to be read from the transport, before a complete packet
 *  can be enqueued to the host stack.
 *  This is required for systems where the transport driver blocks to read
 *  a specified number of octets. This is not to be enabled by default,
 *  as EtherMind HCI transport layer is capable of identifying partial
 *  and complete HCI packets for further processing.
 */
#define HT_ENABLE_PARSE_UTIL

/* ----------------------------------------------------------------------- */
/* ==== Stack Feature Flags ============================================== */
/* ----------------------------------------------------------------------- */
/*
 *  BT_SUPPORT_STACK_VERSION_INFO
 *
 *  This flag enables EtherMind BT_get_version_number() API.
 *
 *  This API is used to retrieve the current stack build version
 *  information
 *
 *  Dependency: None.
 */
#define BT_SUPPORT_STACK_VERSION_INFO

/*
 *  BT_DEBUG_ASSERT
 *
 *  This flag enables EtherMind BT_assert() macro, for evaluating truth
 *  value of an expression. For details, refer to BT_assert.h for the
 *  platform.
 *
 *  The BT_assert() macro is used for debugging during development, and
 *  can be excluded in builds for the final product.
 *
 *  Dependency: None.
 */
#define BT_DEBUG_ASSERT

/*
 *  BT_STATUS
 *
 *  This flag enables EtherMind Status Flag APIs for reporting various
 *  Status, as described in BT_status_api.h. The EtherMind Status APIs
 *  are designed and to be used for debugging purposes during development
 *  and can be excluded in builds for final products.
 *
 *  Dependency: None.
 */
#define BT_STATUS

/*
 *  BT_DISABLE_MUTEX
 *
 *  This flag disables the use of mutex and conditional variables from all
 *  stack modules (protocols and profiles).
 *  This should be defined in EtherMind builds where synchronization
 *  primitives are not required (such as, single task build of EtherMind).
 *
 *  Dependency: None.
 */
/* #define BT_DISABLE_MUTEX */

/*
 *  BT_HAVE_STATIC_DECL
 *
 *  This flag enables provision for declaring functions and/or globals in
 *  a file as 'static'.
 */
#define BT_HAVE_STATIC_DECL

#ifdef BT_HAVE_STATIC_DECL
#define EM_HAVE_STATIC_DECL
#endif /* BT_HAVE_STATIC_DECL */

/*
 *  BT_HAVE_CONST_DECL
 *
 *  This flag enables provision for declaring globals in a file as 'const'.
 */
#define BT_HAVE_CONST_DECL

#ifdef BT_HAVE_CONST_DECL
#define EM_HAVE_CONST_DECL
#endif /* BT_HAVE_CONST_DECL */

/*
 *  BT_HAVE_REENTRANT_DECL
 *
 *  This flag enables provision for declaring re-entrant functions.
 *  At present used for callback function pointers for 8051 build.
 */
/* #define BT_HAVE_REENTRANT_DECL */

/*
 *  VAR_ARG_IN_MACRO_NOT_SUPPORTED
 *
 *  This flag should be enabled if the compiler tool-chain does not support
 *  variable argument in macro.
 */
/* #define VAR_ARG_IN_MACRO_NOT_SUPPORTED */

/*
 *  BT_STORAGE
 *
 *  This flag enables support of the Storage Module
 *
 *  Dependency: BT_LE
 */
#define BT_STORAGE

/*
 *  STORAGE_PERSISTENT_SUPPORT
 *
 *  This flag enables support of persistent memory storage
 *
 *  Dependency: BT_STORAGE
 */
#define STORAGE_PERSISTENT_SUPPORT
#if (defined STORAGE_PERSISTENT_SUPPORT && !defined BT_STORAGE)
#error "Dependency Error for STORAGE_PERSISTENT_SUPPORT"
#endif /* (defined STORAGE_PERSISTENT_SUPPORT && !defined BT_STORAGE) */

/*
 *  STORAGE_RETENTION_SUPPORT
 *
 *  This flag enables support of retention memory storage
 *
 *  Dependency: BT_STORAGE
 */
/* #define STORAGE_RETENTION_SUPPORT */
#if (defined STORAGE_RETENTION_SUPPORT && !defined BT_STORAGE)
#error "Dependency Error for STORAGE_RETENTION_SUPPORT"
#endif /* (defined STORAGE_RETENTION_SUPPORT && !defined BT_STORAGE) */

/*
 *  STORAGE_HAVE_EVENT_MASK
 *
 *  This flag enables support for masking specific Storage Write
 *  Operations.
 *  This is useful to ensure storage write is done only during shutdown or
 *  during pairing/link key update etc.
 *  This can also be changed runtime, to avoid storage write during audio streaming etc.
 *
 *  Dependency: BT_STORAGE
 */
#define STORAGE_HAVE_EVENT_MASK
#if (defined STORAGE_HAVE_EVENT_MASK && !defined BT_STORAGE)
#error "Dependency Error for STORAGE_HAVE_EVENT_MASK"
#endif /* (defined STORAGE_HAVE_EVENT_MASK && !defined BT_STORAGE) */

/*
 *  STORAGE_HAVE_WRITE_CHECK
 *
 *  This flag enables support to block storage write access during
 *  any time and again sync the updates at a later point of time.
 *
 *  Dependency: BT_STORAGE
 */
#define STORAGE_HAVE_WRITE_CHECK
#if (defined STORAGE_HAVE_WRITE_CHECK && !defined BT_STORAGE)
#error "Dependency Error for STORAGE_HAVE_WRITE_CHECK"
#endif /* (defined STORAGE_HAVE_WRITE_CHECK && !defined BT_STORAGE) */

/*
 *  STORAGE_CHECK_CONSISTENCY_ON_RESTORE
 *
 *  This flag enables support for checking consistency of data structures of various
 *  modules, which are restored following storage load operation.
 *  For example, if device queue has some stray elements (due to inconsistent power cycle
 *  of the system etc.), which are not present in the security databases, those can be cleaned.
 *
 *  Dependency: BT_STORAGE
 */
/* #define STORAGE_CHECK_CONSISTENCY_ON_RESTORE */
#if (defined STORAGE_CHECK_CONSISTENCY_ON_RESTORE && !defined BT_STORAGE)
#error "Dependency Error for STORAGE_CHECK_CONSISTENCY_ON_RESTORE"
#endif /* (defined STORAGE_CHECK_CONSISTENCY_ON_RESTORE && !defined BT_STORAGE) */

/*
 *  BT_SNOOP
 *
 *  This flag enables logging HCI traffic in btsnoop format.
 */
#define BT_SNOOP

/*
 *  BT_SUPPORT_ERR_IND_CALLBACK
 *
 *  This flag enables common error indication callback accross modules.
 */
#define BT_SUPPORT_ERR_IND_CALLBACK

/*
 *  BT_SECURITY_VU_VALIDATION
 *
 *  This flag enables hooks to simulate security vulnerabilities.
 */
#define BT_SECURITY_VU_VALIDATION

/* ----------------------------------------------------------------------- */
/* ==== Generic Access Profile Specific Flags ============================ */
/* ----------------------------------------------------------------------- */
#ifdef BT_LE
/*
 *  GAP_CENTRAL
 *
 *  This flag enables GAP Central Role.
 *
 *  A device that supports the Central role initiates the establishment of
 *  a physical connection. A device operating in the Central role will be
 *  in the Master role in the Link Layer Connection State.
 *
 *  Dependency: Only one of the roles Central, Peripheral, Broadcaster and
 *  Observer shall be defined.
 */
#define GAP_CENTRAL

/*
 *  GAP_PERIPHERAL
 *
 *  This flag enables GAP Peripheral Role.
 *
 *  Any device that accepts the establishment of an LE physical link using
 *  any of the connection establishment procedures is referred to as being
 *  in the Peripheral role. A device operating in the Peripheral role will
 *  be in the Slave role in the Link Layer Connection State.
 *
 *  Dependency: Only one of the roles Central, Peripheral, Broadcaster and
 *  Observer shall be defined.
 */
#define GAP_PERIPHERAL

/*
 *  GAP_BROADCASTER
 *
 *  This flag enables GAP Broadcaster Role.
 *
 *  A device operating in the Broadcaster role is a device that sends
 *  non-connectable advertising events.
 *
 *  Dependency: Only one of the roles Central, Peripheral, Broadcaster and
 *  Observer shall be defined.
 */
#define GAP_BROADCASTER

/*
 *  GAP_OBSERVER
 *
 *  This flag enables GAP Observer Role.
 *
 *  A device operating in the Observer role is a device that receives
 *  advertising events.
 *
 *  Dependency: Only one of the roles Central, Peripheral, Broadcaster and
 *  Observer shall be defined.
 */
#define GAP_OBSERVER
#endif /* BT_LE */

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Protocols =================== */
/* ----------------------------------------------------------------------- */
/*
 *  These flags enable initialization and shutdown of various Stack protocol
 *  modules, above L2CAP, and should be defined if any one of them is included
 *  in the build.
 *
 *  Dependency: None.
 */
#ifndef BT_SINGLE_MODE
#define SDP
#define RFCOMM
#define BNEP
#define AVDTP
#define AVCTP
#define OBEX
#define MCAP
#endif /* BT_SINGLE_MODE */

#if (defined GAP_CENTRAL || defined GAP_PERIPHERAL)
#define SMP
#define ATT
#endif /* (defined GAP_CENTRAL || defined GAP_PERIPHERAL) */

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Profiles ==================== */
/* ----------------------------------------------------------------------- */
/*
 *  Module inclusion flags for various EtherMind Profiles, and should be
 *  defined according to the modules included in the build.
 *
 *  Dependency: None.
 */
#ifndef BT_SINGLE_MODE
#define SPP
#define OPP
#define FTP
/* #define HSP_UNIT */
/* #define HSP_AG */
#define HFP_UNIT
#define HFP_AG
/* #define DUNP_DT */
/* #define DUNP_GW */
#define BIP
#define BIP_INITIATOR
#define BIP_RESPONDER
#define BPP
#define BPP_SENDER
#define BPP_PRINTER
#define PAN
#define HID_HOST
#define HID_DEVICE
#define A2DP
#define A2DP_SOURCE
#define A2DP_SINK
#define AVRCP
#define AVRCP_TG
#define AVRCP_CT
/* #define HCRP */
#define DID_CLIENT
#define DID_SERVER
#define SAP_SERVER
#define SAP_CLIENT
#define PBAP_PCE
#define PBAP_PSE
#define HDP
#define MAP_MCE
#define MAP_MSE
/* #define SYNCP */
/* #define SYNCP_SERVER */
/* #define SYNCP_CLIENT */
/* #define GNSS_SERVER */
/* #define GNSS_CLIENT */
#define CTN_CCE
#define CTN_CSE
#endif /* BT_SINGLE_MODE */


/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Supporting Modules ========== */
/* ----------------------------------------------------------------------- */
/*
 *  BT_FSM
 *
 *  FSM Module.
 *
 *  Use this feature flag for modules like OBEX, ATT, SMP etc. which requires
 *  FSM module.
 *
 *  Dependency: None.
 */
#define BT_FSM

/*
 *  BT_HAVE_SHUTDOWN
 *
 *  enables stack shutdown API and protocol/profile interfaces.
 *
 *  Use this feature flag to enable the shutdown APIs in the protocol
 *  and profile modules.
 *
 *  Dependency: None.
 */
/* #define BT_HAVE_SHUTDOWN */

#ifdef BT_LE
/*
 *  BT_RACP
 *
 *  Record Access Control Point Module.
 *
 *  Use this feature flag for GATT based profiles/services which requires
 *  Record Access Control Point.
 *
 *  Dependency: None.
 */
#define BT_RACP
#endif /* BT_LE */

/* ----------------------------------------------------------------------- */
/* ==== Device Queue Module Specific Flags =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  DQ_NO_NULL_PARAM_CHECK
 *
 *  Disable null check for Device Queue APIs.
 *
 *  Use of this feature flag is recommended for final release/production
 *  versions of applications for optimized code size.
 *
 *  Dependency: None.
 */
/* #define DQ_NO_NULL_PARAM_CHECK */


/* ----------------------------------------------------------------------- */
/* ==== Write Task Module Specific Flags ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  WT_SERVICE_Q_ON_PRIORITY
 *
 *  If this feature flag is defined, Write Task after servicing
 *  each enqueued element from a specific priority queue, will check
 *  if there is any enqueued element in higher priority queue to be processed.
 *
 *  Default behaviour is to process all the enqueued elements in a specific
 *  queue, before processing elements from another priority queue.
 *
 *  Dependency: None.
 */
#define WT_SERVICE_Q_ON_PRIORITY

/*
 *  WT_HAVE_USER_MEDIA_QUEUE
 *
 *  If this feature flag is defined, Write Task exposes support for an
 *  additional media queue, and this would be used by the AVDTP layer
 *  during media write scheduling over the Write Task. This could be
 *  enabled when the application supports other data intensive profiles
 *  like SPP, MAP, PBAP transmission along with A2DP Source streaming.
 *
 *  Dependency: None.
 */
#define WT_HAVE_USER_MEDIA_QUEUE

/* ----------------------------------------------------------------------- */
/* ==== HCI Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/*
 *  HCI_VENDOR_SPECIFIC_COMMANDS
 *
 *  This flag enables HCI module to provide applications the ability to send
 *  vendor-specific HCI commands.
 *
 *  Specifically, the BT_hci_vendor_specific_command() API is available only
 *  if this flag is defined.
 *
 *  Dependency: None.
 */
#define HCI_VENDOR_SPECIFIC_COMMANDS

#ifdef BR_EDR_HCI
/*
 *  HCI_NO_ADD_SCO_CONNECTION
 *
 *  This flag disables support for older HCI Add SCO Connection API & Command
 *  handling in HCI. The newer HCI Setup Synchronous Connection API & Command
 *  should be used instead.
 *
 *  Dependency: None.
 */
/* #define HCI_NO_ADD_SCO_CONNECTION */

/*
 *  HCI_NO_ESCO_AUTO_ACCEPT
 *
 *  By default, HCI accepts the eSCO connection request from peer.
 *  This flag disables this behaviour and allows the application to accept or
 *  reject the eSCO connection using BT_hci_esco_accept_connection_request()
 *  or BT_hci_esco_reject_connection_request() APIs.
 *
 *  Dependency: None.
 */
/* #define HCI_NO_ESCO_AUTO_ACCEPT */
#endif /* BR_EDR_HCI */

/*
 *  HCI_HOST_CONTROLLER_FLOW_ON
 *
 *  This flag enables HCI module to switch on HCI Host Controller to Host Flow
 *  Control feature offered by the Bluetooth device.
 *
 *  If switched on, this feature will enable HCI to send HCI Host Number of
 *  Completed Packets Command to the Bluetooth Hardware on reception of every
 *  ACL packet.
 *
 *  Dependency: None.
 *
 *  NOTE: This feature is not fully supported.
 */
/* #define HCI_HOST_CONTROLLER_FLOW_ON */

/*
 *  HCI_TESTING_COMMANDS
 *
 *  This flag enables HCI Testing Commands APIs:
 *  - BT_hci_read_loopback_mode()
 *  - BT_hci_write_loopback_mode()
 *  - BT_hci_enable_device_under_test_mode()
 *
 *  Dependency: None.
 */
#define HCI_TESTING_COMMANDS

#ifdef BR_EDR_HCI
/*
 *  HCI_SUPPORT_STORED_LINK_KEY_COMMANDS
 *
 *  This flag enables following HCI Command APIs:
 *  - BT_hci_read_stored_link_key()
 *  - BT_hci_write_stored_link_key()
 *  - BT_hci_delete_stored_link_key()
 *
 *  Dependency: None.
 */
/* #define HCI_SUPPORT_STORED_LINK_KEY_COMMANDS */

/*
 *  HCI_SCO
 *
 *  This flag enables HCI SCO data packet handling.
 *
 *  Dependency: None.
 */
/* #define HCI_SCO */

/*
 *  HCI_ENH_SCO
 *
 *  This flag enables HCI Enhanced SCO connection & data packet handling.
 *
 *  Dependency: BT_4_1.
 */
#ifdef BT_4_1
#define HCI_ENH_SCO
#endif /* BT_4_1 */
#endif /* BR_EDR_HCI */


/*
 *  HCI_SUPPORT_LE_CONN_FAILURE_EVENT_WITH_NO_ADDR
 *
 *  This flag enables support for bluetooth controller which does not
 *  provide peer bluetooth device address in case of failure with
 *  LE Connection complete or LE Enhanced Connection complete event.
 *
 *  Dependency: None.
 */
#define HCI_SUPPORT_LE_CONN_FAILURE_EVENT_WITH_NO_ADDR

#ifdef BT_5_2
#ifdef BT_LE
/*
 *  HCI_ISO_DATA
 *
 *  This flag enables HCI ISO data packet handling.
 *
 *  Dependency: BT_5_2 must be defined.
 */
#define HCI_ISO_DATA

/*
 *  HCI_SUPPORT_ISO_WRITE_PDU
 *
 *  This flag enables appliation to write framed ISO PDUs.
 *  The stack does not do any packet framing with the controllers
 *  supported ISO buffer size, and is expected to be done by the
 *  application and sent as correct fragments.
 *
 *  Dependency: BT_5_2 must be defined.
 */
#define HCI_SUPPORT_ISO_WRITE_PDU
#endif /* BT_LE */
#endif /* BT_5_2 */

/*
 *  HCI_HAVE_INIT_COMMAND_MASK
 *
 *  This flag enables BT_hci_set_init_command_mask() API.
 *
 *  Using the BT_hci_set_init_command_mask() API, application
 *  can control the HCI commands sent during BT_bluetooth_on()
 *
 *  Dependency: None.
 */
#define HCI_HAVE_INIT_COMMAND_MASK


#ifdef BT_3_0
/*
 *  HCI_FLOW_CONTROL_COMMANDS
 *
 *  This flag enables HCI Flow Control Commands APIs:
 *  - BT_hci_read_flow_control_mode()
 *  - BT_hci_write_flow_control_mode()
 *
 *  Dependency: BT_3_0 must be defined.
 */
/* #define HCI_FLOW_CONTROL_COMMANDS */
#endif /* BT_3_0 */

/*
 *  HCI_NO_NULL_PARAM_CHECK
 *
 *  This flag disables 'null' parameter check in HCI APIs
 *
 *  When application using HCI APIs are validated and
 *  it is ensured that none of the API call path is using
 *  an unexpected 'null' parameter, this flag can be enabled
 *  to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define HCI_NO_NULL_PARAM_CHECK */

/*
 *  HCI_NO_RANGE_CHECK
 *
 *  This flag disables the check in HCI APIs to verify if the parameter
 *  values are within specification defined valid range
 *
 *  When application using HCI APIs are validated and
 *  it is ensured that none of the API call path is using
 *  parameters with invalid value, this flag can be enabled
 *  to reduce the code size.
 *
 *  Otherwise also this flag can be enabled, if the overall system can
 *  depend on the HCI parameter validation performed
 *  by the underlying HCI controller.
 *
 *  Dependency: None.
 */
/* #define HCI_NO_RANGE_CHECK */

/*
 *  HCI_BYPASS_BH_ENQUEUE
 *
 *  This flag disables the use of common_api_bh in the command transmit path.
 *  By default this flag should be disabled.
 *
 *  Dependency: None.
 */
/* #define HCI_BYPASS_BH_ENQUEUE */

/*
 *  HCI_HANDLE_CONNECTION_FAILURE_IN_CONNECTED_STATE
 *
 *  This flag enables hack to drop connection failure from the same peer
 *  which is previously disconnected due to connection timeout but the
 *  local device has not indicated the disconnection.
 *
 *  This is observed with the certain controllers and this check
 *  is a hack to drop the connection event as long as no
 *  disconnection is received.
 *
 *  Dependancy: None
 */
#define HCI_HANDLE_CONNECTION_FAILURE_IN_CONNECTED_STATE

/*
 *  HCI_CHECK_EVENT_CODE_IN_RANGE
 *
 *  This flag enables the block to validate the event codes
 *  received from the controller.
 *
 *  Dependancy: None
 */
#define HCI_CHECK_EVENT_CODE_IN_RANGE

/*
 *  HAVE_HCI_COMMAND_PARAMS_INIT
 *
 *  This flag enables the initialization of HCI_COMMAND_PARAMS structure,
 *  to suppress MISRA C-2012 Rule 9.1 and Coverity UNINIT warnings.
 *
 *  Dependency: None
 */
#define HAVE_HCI_COMMAND_PARAMS_INIT

/*
 * Feature flags for HCI Commands defined for BLE Single Mode
 *
 * Note:
 * ## Commands primarily required for Master(M) and Slave(S) are marked
 * ## Remaining commands are applicable for both roles
 */
/* Reset Command */
#define HCI_RESET_SUPPORT

/* Read Local Version Information Command */
#define HCI_READ_LOCAL_VERSION_INFORMATION_SUPPORT

/* Read Local Supported Features Command */
#define HCI_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT

/* Read BD_ADDR Command */
#define HCI_READ_BD_ADDR_SUPPORT

/* Set Event Mask Command */
#define HCI_SET_EVENT_MASK_SUPPORT

/* Disconnect Command */
#define HCI_DISCONNECT_SUPPORT

/* Read Remote Version Information Command */
#define HCI_READ_REMOTE_VERSION_INFORMATION_SUPPORT

/* Read Transmit Power Level Command */
#define HCI_READ_TRANSMIT_POWER_LEVEL_SUPPORT

/* Read RSSI Command */
#define HCI_READ_RSSI_SUPPORT

/* Read Buffer Size Command */
#define HCI_READ_BUFFER_SIZE_SUPPORT

/* Host Buffer Size Command */
#define HCI_HOST_BUFFER_SIZE_SUPPORT

/* Set Controller To Host Flow Control Command */
#define HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_SUPPORT

/* Host Number Of Completed Packets Command */
/* Read Local Supported Commands Command */
#define HCI_READ_LOCAL_SUPPORTED_COMMANDS_SUPPORT

#ifdef BT_LE
/* LE Read Buffer Size Command */
#define HCI_LE_READ_BUFFER_SIZE_SUPPORT

/* LE Read Local Supported Features Command */
#define HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT

/* LE Read Supported States Command */
#define HCI_LE_READ_SUPPORTED_STATES_SUPPORT

/* LE Add Device To White List Command */
#define HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT

/* LE Clear White List Command */
#define HCI_LE_CLEAR_WHITE_LIST_SUPPORT

/* LE Read White List Size Command */
#define HCI_LE_READ_WHITE_LIST_SIZE_SUPPORT

/* LE Remove Device From White List Command */
#define HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT

/* LE Set Event Mask Command */
#define HCI_LE_SET_EVENT_MASK_SUPPORT

/* LE Test End Command */
#define HCI_LE_TEST_END_SUPPORT

/* LE Set Advertise Enable Command (S) */
#define HCI_LE_SET_ADVERTISING_ENABLE_SUPPORT

/* LE Set Advertising Data Command (S) */
#define HCI_LE_SET_ADVERTISING_DATA_SUPPORT

/* LE Set Advertising Parameters Command (S) */
#define HCI_LE_SET_ADVERTISING_PARAMETERS_SUPPORT

/* LE Set Random Address Command */
#define HCI_LE_SET_RANDOM_ADDRESS_SUPPORT

/* LE Read Advertising Channel TX Power Command */
#define HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_SUPPORT

/* LE Transmitter Test Command */
#define HCI_LE_TRANSMITTER_TEST_COMMAND_SUPPORT

/* LE Set Scan Enable Command (M) */
#define HCI_LE_SET_SCAN_ENABLE_SUPPORT

/* LE Set Scan Parameters Command (M) */
#define HCI_LE_SET_SCAN_PARAMETERS_SUPPORT

/* LE Receiver Test Command */
#define HCI_LE_RECEIVER_TEST_COMMAND_SUPPORT

/* LE Set Scan Response Data Command (S) */
#define HCI_LE_SET_SCAN_RESPONSE_DATA_SUPPORT

/* LE Create Connection Cancel Command (M) */
#define HCI_LE_CREATE_CONNECTION_CANCEL_SUPPORT

/* LE Create Connection Command (M) */
#define HCI_LE_CREATE_CONNECTION_SUPPORT

/* LE Read Remote Used Features Command */
#define HCI_LE_READ_REMOTE_USED_FEATURES_SUPPORT

/* LE Connection Update Command (M) */
#define HCI_LE_CONNECTION_UPDATE_SUPPORT

/* LE Set Host Channel Classification Command (M) */
#define HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_SUPPORT

/* LE Read Channel Map Command */
#define HCI_LE_READ_CHANNEL_MAP_SUPPORT

/* LE Encrypt Command */
#define HCI_LE_ENCRYPT_SUPPORT

/* LE Long Term Key Requested Reply Command */
#define HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_SUPPORT

/* LE Long Term Key Requested Negative Reply Command */
#define HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_SUPPORT

/* LE Rand Command */
#define HCI_LE_RAND_SUPPORT

/* LE Start Encryption Command (M) */
#define HCI_LE_START_ENCRYPTION_SUPPORT

/* Get LE Device Role */
#define HCI_SUPPORT_GET_DEVICE_ROLE

/*
 *  HCI_HAVE_LE_LONG_TERM_KEY_REQ_SUBEVENT
 *
 *  HCI LE event, HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT is handled internally
 *  by SMP and not given to the HCI application.
 *  This flag enables passing HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT to
 *  the HCI application, if required for specific handling.
 *
 *  By default, this macro is not defined.
 *
 *  Dependency: None.
 */
/* #define HCI_HAVE_LE_LONG_TERM_KEY_REQ_SUBEVENT */

#endif /* BT_LE */

#ifdef BR_EDR_HCI
#ifdef BT_1_2
/* Read LMP Handle */
/* #define HCI_READ_LMP_HANDLE_SUPPORT */
#endif /* BT_1_2 */

#ifdef BT_3_0
/* Read Data Block Size */
/* #define HCI_READ_DATA_BLOCK_SIZE_SUPPORT  */
#endif /* BT_3_0 */

#ifdef BT_4_1
/* Remote OOB Extended Data Request Reply */
/* #define HCI_REMOTE_OOB_EXTENDED_DATA_REQUEST_REPLY_SUPPORT */

/* Set MWS Channel Parameters */
/* #define HCI_SET_MWS_CHANNEL_PARAMETERS_SUPPORT */

/* Set External Frame Configuration */
/* #define HCI_SET_EXTERNAL_FRAME_CONFIGURATION_SUPPORT */

/* Set MWS Signalling */
/* #define HCI_SET_MWS_SIGNALING_SUPPORT */

/* Set MWS Transport Layer */
/* #define HCI_SET_MWS_TRANSPORT_LAYER_SUPPORT */

/* Set MWS_PATTERN Configuration */
/* #define HCI_SET_MWS_PATTERN_CONFIGURATION_SUPPORT */

/* Set MWS Scan Frequency Table */
/* #define HCI_SET_MWS_SCAN_FREQUENCY_TABLE_SUPPORT */

/* Read Extended Page Timeout */
/* #define HCI_READ_EXTENDED_PAGE_TIMEOUT_SUPPORT */

/* Write Extended Page Timeout */
/* #define HCI_WRITE_EXTENDED_PAGE_TIMEOUT_SUPPORT */

/* Read Extended Inquiry Length */
/* #define HCI_READ_EXTENDED_INQUIRY_LENGTH_SUPPORT */

/* Write Extended Inquiry Length */
/* #define HCI_WRITE_EXTENDED_INQUIRY_LENGTH_SUPPORT */

/* Read Local OOB Extended Data */
/* #define HCI_READ_LOCAL_OOB_EXTENDED_DATA_SUPPORT */

/* Get MWS Transport Layer Configuration */
/* #define HCI_GET_MWS_TRANSPORT_LAYER_CONFIGURATION_SUPPORT */

/* Set Triggered Clock Capture */
/* #define HCI_SET_TRIGGERED_CLOCK_CAPTURE_SUPPORT */
#endif /* BT_4_1 */

#ifdef BT_5_1
/* Read Local Simple Pairing Options */
/* #define HCI_READ_LOCAL_SIMPLE_PAIRING_OPTIONS_SUPPORT */
#endif /* BT_5_1 */

#ifdef BT_5_2
/* Set Ecosystem Base Interval */
#define HCI_SET_ECOSYSTEM_BASE_INTERVAL_SUPPORT

/* Configure Data Path */
#define HCI_CONFIGURE_DATA_PATH_SUPPORT

/* Read Local Supported Codecs V2 */
#define HCI_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT

/* Read Local Supported Codec Capabilities */
#define HCI_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_SUPPORT

/* Read Local Supported Controller Delay */
#define HCI_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_SUPPORT
#endif /* BT_5_2 */
#endif /* BR_EDR_HCI */

/*
 *  HCI_DONT_CHECK_LOCAL_ADDR_FOR_CONNECT
 *
 *  This flag is to be enabled if connection to remote device with same
 *  BD Address as local device is to be allowed. Default behaviour
 *  of the stack is to disallow such behaviour.
 *
 *  Dependency: None.
 */
/* #define HCI_DONT_CHECK_LOCAL_ADDR_FOR_CONNECT */

/* ----------------------------------------------------------------------- */
/* ==== SM Module Specific Flags ========================================= */
/* ----------------------------------------------------------------------- */
#ifdef CLASSIC_SEC_MANAGER
/*
 *  SM_HAVE_MODE_2
 *
 *  This flag is needed if Security Manager (SM) module is required to provide
 *  Bluetooth Security Mode 2 operations.
 *
 *  Dependency: None.
 */
#define SM_HAVE_MODE_2

/*
 *  SM_STORAGE
 *
 *  This flag enables Security Manager (SM) module to store configuration data
 *  (such as, the Security Mode, default PIN Code, default Authorization Flag
 *  etc.) in persistent storage device, if one is available and configured.
 *  The SM attempts to store the configuration data at the time when SM is
 *  shutdown from the context of BT_bluetooth_off().
 *
 *  On next initialization, during BT_bluetooth_on(), SM reads the stored
 *  configuration data from the persistent storage media.
 *
 *  Dependency: STORAGE_PERSISTENT_SUPPORT
 */
#define SM_STORAGE
#if (defined SM_STORAGE && !defined STORAGE_PERSISTENT_SUPPORT)
#error "Dependency Error for SM_STORAGE"
#endif /* (defined SM_STORAGE && !defined STORAGE_PERSISTENT_SUPPORT) */

/*
 *  If SSP is defined, then SM_HAVE_MODE_2 must be enabled.
 */
#if defined BT_SSP && !defined SM_HAVE_MODE_2
#error "SSP needs SM_HAVE_MODE_2 to be enabled!"
#endif /* defined BT_SSP && !defined SM_HAVE_MODE_2 */

/*
 *  SM_IO_CAP_DYNAMIC
 *
 *  This flag enables APIs that allow dynamic configuration of device's
 *  Input/Output Capability, for the purpose of Secure Simple Pairing.
 *  For most platform/devices, this configuration should be commented
 *  as IO Capability is not expected to change over time.
 *
 *  Dependency: BT_SSP must be defined.
 */
#ifdef BT_SSP
#define SM_IO_CAP_DYNAMIC
#endif /* BT_SSP */

/*
 *  SM_AUTHREQ_DYNAMIC
 *
 *  This flag enables APIs that allow dynamic configuration of device's
 *  MITM and Bonding requirement, for the purpose of Secure Simple Pairing.
 *  For most platform/devices, this configuration should be commented
 *  as these would be derived from the Service security requirements.
 *
 *  Dependency: BT_SSP must be defined.
 */
#ifdef BT_SSP
#define SM_AUTHREQ_DYNAMIC
#endif /* BT_SSP */

/*
 *  SM_FORCE_CLEAR_ENTITY
 *
 *  This flag enables SM to free the entity and its device queue reference on disconnection
 *  of a device irrespective of bonded or non-bonded
 *
 *  Dependency: None
 */
/* #define SM_FORCE_CLEAR_ENTITY */

/*
 *  SM_CLEAR_UNBONDED_ENTITY
 *
 *  This flag enables SM to free the entity and its device queue reference on disconnection
 *  of an non-bonded device. This flag does not have any effect if the
 *  SM_FORCE_CLEAR_ENTITY is defined.
 *
 *  Dependency: None
 */
/* #define SM_CLEAR_UNBONDED_ENTITY */

/*
 *  SM_NOSYNC_PSSTORE_ON_DELETION
 *
 *  This flag disables the SM from updating the PSStore upon device deletion.
 *
 *  Dependency: None
 */
/* #define SM_NOSYNC_PSSTORE_ON_DELETION */
#endif /* CLASSIC_SEC_MANAGER */

#ifdef BT_4_1
#ifdef BT_LE
/*
 *  HCI_LOW_DUTY_CYCLE_DIRECTED_ADVERTISING
 *
 *  This flag enables configuration of Low Duty Cycle Directed Advertising
 *  mode through BT_hci_le_set_advertising_parameters() API.
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define HCI_LOW_DUTY_CYCLE_DIRECTED_ADVERTISING

/*
 * HCI_LE_PING_SUPPORT
 *
 * This flag enables LE Ping Support.
 * Dependency: BT_4_1 must be defined.
 */
#define HCI_LE_PING_SUPPORT
#endif /* BT_LE */

#ifdef HCI_LE_PING_SUPPORT
/*
 *  HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT

/*
 *  HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
#endif /* HCI_LE_PING_SUPPORT*/

/*
 *  HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT

/*
 *  HCI_SET_EVENT_MASK_PAGE_2_SUPPORT
 *
 *  Dependency: BT_4_1 must be defined.
 */
#define HCI_SET_EVENT_MASK_PAGE_2_SUPPORT

/*
 *  HCI_TRUNCATED_PAGE_SUPPORT
 *
 *  This flag is to support Truncated Page command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_TRUNCATED_PAGE_SUPPORT */

/*
 *  HCI_TRUNCATED_PAGE_CANCEL_SUPPORT
 *
 *  This flag is to support Truncated Page Cancel command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_TRUNCATED_PAGE_CANCEL_SUPPORT */

/*
 *  HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_SUPPORT
 *
 *  This flag is to support Set Connectionless Slave Broadcast command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_SUPPORT */

/*
 *  HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_SUPPORT
 *
 *  This flag is to support Set Connectionless Slave Broadcast Receive command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_SUPPORT */

/*
 *  HCI_START_SYNCHRONIZATION_TRAIN_SUPPORT
 *
 *  This flag is to support Start Synchronization Train command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_START_SYNCHRONIZATION_TRAIN_SUPPORT */

/*
 *  HCI_RECEIVE_SYNCHRONIZATION_TRAIN_SUPPORT
 *
 *  This flag is to support Receive Synchronization Train command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_RECEIVE_SYNCHRONIZATION_TRAIN_SUPPORT */

/*
 *  HCI_SET_RESERVED_LT_ADDR_SUPPORT
 *
 *  This flag is to support Set Reserved LT_ADDR command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_SET_RESERVED_LT_ADDR_SUPPORT */

/*
 *  HCI_DELETE_RESERVED_LT_ADDR_SUPPORT
 *
 *  This flag is to support Delete Reserved LT_ADDR command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_DELETE_RESERVED_LT_ADDR_SUPPORT */

/*
 *  HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_DATA_SUPPORT
 *
 *  This flag is to support Set Connectionless Slave Broadcast Data command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_DATA_SUPPORT */

/*
 *  HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT
 *
 *  This flag is to support Read Synchronization Train Parameters command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT */

/*
 *  HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT
 *
 *  This flag is to support Write Synchronization Train Parameters command.
 *
 *  Dependency: BT_4_1 must be defined.
 */
/* #define HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT */

#endif /* BT_4_1 */

#ifdef BT_4_2
#ifdef BT_LE
/*
 * HCI_LE_DATA_LENGTH_EXT_SUPPORT
 *
 * This flag enables LE Data Length Extension Support.
 * Dependency: BT_4_2 must be defined.
 */
#define HCI_LE_DATA_LENGTH_EXT_SUPPORT

/*
 * HCI_LE_SECURE_CONNECTIONS_SUPPORT
 *
 * This flag enables LE Secure Connections Support.
 * Dependency: BT_4_2 must be defined.
 */
#define HCI_LE_SECURE_CONNECTIONS_SUPPORT

/*
 * HCI_LE_PRIVACY_1_2_SUPPORT
 *
 * This flag enables LE Privacy 1.2 Support.
 * Dependency: BT_4_2 must be defined.
 */
#define HCI_LE_PRIVACY_1_2_SUPPORT

#ifdef HCI_LE_DATA_LENGTH_EXT_SUPPORT
/* LE Set Data Length command support */
#define HCI_LE_SET_DATA_LENGTH_SUPPORT

/* LE Read suggested Default Data Length */
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT

/* LE Write Suggested Degault Data Length */
#define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT

/* LE Read Maximum Data Length */
#define HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT
#endif /* HCI_LE_DATA_LENGTH_EXT_SUPPORT */

#ifdef HCI_LE_SECURE_CONNECTIONS_SUPPORT
/* LE Read Local P256 Public Key */
#define HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT

/* LE Generate DH Key */
#define HCI_LE_GENERATE_DHKEY_SUPPORT
#endif /* HCI_LE_SECURE_CONNECTIONS_SUPPORT */

#ifdef HCI_LE_PRIVACY_1_2_SUPPORT
/* LE Add Device to Resolving List command */
#define HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT

/* LE Remove Device From Resolving List */
#define HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT

/* LE Clear Resolving List */
#define HCI_LE_CLR_RESOLVING_LIST_SUPPORT

/* LE Read Resolving List Size */
#define HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT

/* LE Read Peer Resolvable Address */
#define HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT

/* LE Read Local Resolvable Address */
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT

/* LE Set Address Resolution Enable */
#define HCI_LE_SET_ADDR_RESOLUTION_ENABLE_SUPPORT

/* LE Set Resolvable Private Address Timeout */
#define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT
#endif /* HCI_LE_PRIVACY_1_2_SUPPORT */
#endif /* BT_LE */
#endif /* BT_4_2 */

#ifdef BT_5_0
#ifdef BT_LE
/*
 * HCI_LE_2MBPS_PHY_SUPPORT
 *
 * This flag enables LE 2Mbps PHY Support.
 * Dependency: BT_5_0 must be defined.
 */
#define HCI_LE_2MBPS_PHY_SUPPORT

#ifdef HCI_LE_2MBPS_PHY_SUPPORT

/* LE Read PHY */
#define HCI_LE_READ_PHY_SUPPORT

/* LE Set Host Preferred PHY */
#define HCI_LE_SET_DEFAULT_PHY_SUPPORT

/* LE Set PHY */
#define HCI_LE_SET_PHY_SUPPORT

/* LE Enhanced Receiver Test */
#define HCI_LE_ENHANCED_RECEIVER_TEST_SUPPORT

/* LE Enhanced Transmitter Test */
#define HCI_LE_ENHANCED_TRANSMITTER_TEST_SUPPORT

#endif /* HCI_LE_2MBPS_PHY_SUPPORT */

/*
 *  HCI_LE_ADVERTISING_EXTENSION_SUPPORT
 *
 *  This flag enables HCI LE Advertising Extension support
 *  Dependency:
 */
#define HCI_LE_ADVERTISING_EXTENSION_SUPPORT

#ifdef HCI_LE_ADVERTISING_EXTENSION_SUPPORT
/* LE Set Extended Advertising Parameters */
#define HCI_LE_SET_EXTENDED_ADV_PARAMS_SUPPORT

/* LE Set Periodic Advertising Parameters*/
#define HCI_LE_SET_PERIODIC_ADV_PARAMS_SUPPORT

/* LE Set Extended Advertising Data */
#define HCI_LE_SET_EXTENDED_ADVERTISING_DATA_SUPPORT

/* LE Set Extended Scan Response Data */
#define HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_SUPPORT

/* LE Set Extended Advertise Enable */
#define HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_SUPPORT

/* LE Read Maximum Advertising Data Length */
#define HCI_LE_READ_MAX_ADV_DATA_LENGTH_SUPPORT

/* LE Read Num Supported Advertising Sets */
#define HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_SUPPORT

/* LE Read Transmit Power */
#define HCI_LE_READ_TRANSMIT_POWER_SUPPORT

/* LE Remove Advertising Set */
#define HCI_LE_REMOVE_ADVERTISING_SET_SUPPORT

/* LE Clear Advertising Sets */
#define HCI_LE_CLEAR_ADVERTISING_SETS_SUPPORT

/* LE Set Extended Scan Parameters */
#define HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_SUPPORT

/* LE Set Extended Scan Enable */
#define HCI_LE_SET_EXTENDED_SCAN_ENABLE_SUPPORT

/* LE Periodic Advertising Receive */
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_SUPPORT

/* LE Add Device To Periodic Advertiser List */
#define HCI_LE_ADD_DEV_TO_PERIODIC_ADVSER_LIST_SUPPORT

/* LE Remove Device From Periodic Advertiser List */
#define HCI_LE_RM_DEV_FROM_PERIODIC_ADVSER_LIST_SUPPORT

/* LE Clear Periodic Advertiser List */
#define HCI_LE_CLEAR_PERIODIC_ADVSER_LIST_SUPPORT

/* LE Read Periodic Advertiser List Size */
#define HCI_LE_READ_PERIODIC_ADVSER_LIST_SIZE_SUPPORT

/* LE Set Scan Request Notification Enable */
#define HCI_LE_SET_SCAN_REQUEST_NTF_ENABLE_SUPPORT

/* LE Read RF Path Compensation */
#define HCI_LE_READ_RF_PATH_COMPENSATION_SUPPORT

/* LE Write RF Path Compensation */
#define HCI_LE_WRITE_RF_PATH_COMPENSATION_SUPPORT

/* LE Set Advertising Random Address */
#define HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_SUPPORT

/* LE Set Periodic Advertising Data */
#define HCI_LE_SET_PERIODIC_ADVERTISING_DATA_SUPPORT

/* LE Set Periodic Advertising Enable */
#define HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_SUPPORT

/* LE Periodic Advertising Receive Cancel */
#define HCI_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_SUPPORT

/* LE Periodic Advertising Receive End */
#define HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_SUPPORT

/* LE Extended Create Connection */
#define HCI_LE_EXTENDED_CREATE_CONNECTION_SUPPORT

/**
 * HCI LE Extended ADV Set Local Address Support
 *
 * This flag lets stack to maintain the own address mapping
 * for each ADV Sets and maps the same with Device Queue
 * on Connection of each ADV Handle.
 */
#define HCI_LE_EXT_ADV_SETS_LOCAL_ADDR_SUPPORT

#endif /* HCI_LE_ADVERTISING_EXTENSION_SUPPORT */

/* LE Set Privacy Mode */
#define HCI_LE_SET_PRIVACY_MODE_SUPPORT
#endif /* BT_LE */
#endif /* BT_5_0 */

#ifdef BT_5_1
#ifdef BT_LE
/* LE Receiver Test v3 */
#define HCI_LE_RECEIVER_TEST_V3_SUPPORT

/* LE Transmitter Test v3 */
#define HCI_LE_TRANSMITTER_TEST_V3_SUPPORT

/* LE Set Connectionless CTE Transmit Parameters */
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_SUPPORT

/* LE Set Connectionless CTE Transmit Enable */
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_SUPPORT

/* LE Set Connectionless IQ Sampling Enable */
#define HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_SUPPORT

/* LE Set Connection CTE Receive Parameters */
#define HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_SUPPORT

/* LE Set Connection CTE Transmit Parameters */
#define HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_SUPPORT

/* LE Set Connection CTE Request Enable */
#define HCI_LE_SET_CONNECTION_CTE_REQUEST_ENABLE_SUPPORT

/* LE Set Connection CTE Response Enable */
#define HCI_LE_SET_CONNECTION_CTE_RESPONSE_ENABLE_SUPPORT

/* LE Read Antenna Information */
#define HCI_LE_READ_ANTENNA_INFORMATION_SUPPORT

/* LE Set Periodic Advertising Receive Enable */
#define HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_SUPPORT

/* LE Periodic Advertising Sync Transfer */
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_SUPPORT

/* LE Periodic Advertising Set Info Transfer */
#define HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER_SUPPORT

/* LE Set Periodic Advertising Sync Transfer Parameters */
#define HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT

/* LE Set Default Periodic Advertising Sync Transfer Parameters */
#define HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT

/* LE Generate DHKey v2 */
#define HCI_LE_GENERATE_DHKEY_V2_SUPPORT

/* LE Modify Sleep Clock Accuracy */
#define HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_SUPPORT
#endif /* BT_LE */
#endif /* BT_5_1 */


#ifdef BT_5_2
#ifdef BT_LE
/* LE Read Buffer Size (v2) Command */
#define HCI_LE_READ_BUFFER_SIZE_V2_SUPPORT

/* LE Read ISO TX Sync */
#define HCI_LE_READ_ISO_TX_SYNC_SUPPORT

/*
 *  HCI_LE_CIG_CIS_SUPPORT
 *
 *  This flag enables HCI LE CIG and CIS handling.
 *  Dependency: HCI_ISO_DATA
 */
#ifdef HCI_ISO_DATA
#define HCI_LE_CIG_CIS_SUPPORT
#endif /* HCI_ISO_DATA */

#ifdef HCI_LE_CIG_CIS_SUPPORT

/*
 *  HCI_LE_HAVE_ACL_TO_CIS_MAPPING
 *
 *  This flag shall be enabled, if the underlying controller does not give
 *  CIS Disconnection Events, when the underlying ACL is disconnected.
 *
 *  By default, this flag shall be disabled.
 *
 *  Dependency: HCI_LE_CIG_CIS_SUPPORT
 */
/* #define HCI_LE_HAVE_ACL_TO_CIS_MAPPING */

/* LE Set CIG Parameters */
#define HCI_LE_SET_CIG_PARAMETERS_SUPPORT

/* LE Set CIG Parameters Test */
#define HCI_LE_SET_CIG_PARAMETERS_TEST_SUPPORT

/* LE Create CIS */
#define HCI_LE_CREATE_CIS_SUPPORT

/* LE Remove CIG */
#define HCI_LE_REMOVE_CIG_SUPPORT

/* LE Accept CIS Request */
#define HCI_LE_ACCEPT_CIS_REQUEST_SUPPORT

/* LE Reject CIS Request */
#define HCI_LE_REJECT_CIS_REQUEST_SUPPORT
#endif  /* HCI_LE_CIG_CIS_SUPPORT */

/*
 *  HCI_LE_BIG_BIS_SUPPORT
 *
 *  This flag enables HCI LE BIG and BIS handling.
 *  Dependency: HCI_ISO_DATA.
 */
#ifdef HCI_ISO_DATA
#define HCI_LE_BIG_BIS_SUPPORT
#endif /* HCI_ISO_DATA */

#ifdef HCI_LE_BIG_BIS_SUPPORT
/* LE Create BIG */
#define HCI_LE_CREATE_BIG_SUPPORT

/* LE Create BIG Test */
#define HCI_LE_CREATE_BIG_TEST_SUPPORT

/* LE Terminate BIG */
#define HCI_LE_TERMINATE_BIG_SUPPORT

/* LE BIG Create Sync */
#define HCI_LE_BIG_CREATE_SYNC_SUPPORT

/* LE BIG Terminate Sync */
#define HCI_LE_BIG_TERMINATE_SYNC_SUPPORT
#endif /* HCI_LE_BIG_BIS_SUPPORT */

#if (defined HCI_ISO_DATA && !defined HCI_LE_CIG_CIS_SUPPORT && !defined HCI_LE_BIG_BIS_SUPPORT)
#error "Dependency: HCI_ISO_DATA defined but CIS or BIS Support Undefined"
#endif /* (defined HCI_ISO_DATA && !defined HCI_LE_CIG_CIS_SUPPORT && !defined HCI_LE_BIG_BIS_SUPPORT) */

/* LE Request Peer SCA */
#define HCI_LE_REQUEST_PEER_SCA_SUPPORT

/* LE Setup ISO Data Path */
#define HCI_LE_SETUP_ISO_DATA_PATH_SUPPORT

/* LE Remove ISO Data Path */
#define HCI_LE_REMOVE_ISO_DATA_PATH_SUPPORT

/* LE ISO Transmit Test */
#define HCI_LE_ISO_TRANSMIT_TEST_SUPPORT

/* LE ISO Receive Test */
#define HCI_LE_ISO_RECEIVE_TEST_SUPPORT

/* LE ISO Read Test Counters */
#define HCI_LE_ISO_READ_TEST_COUNTERS_SUPPORT

/* LE ISO Test End */
#define HCI_LE_ISO_TEST_END_SUPPORT

/* LE Set Host Feature */
#define HCI_LE_SET_HOST_FEATURE_SUPPORT

/* LE Read ISO Link Quality */
#define HCI_LE_READ_ISO_LINK_QUALITY_SUPPORT

/* LE Enhanced Read Transmit Power Level */
#define HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_SUPPORT

/* LE Read Remote Transmit Power Level */
#define HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_SUPPORT

/* LE Set Path Loss Reporting Parameters */
#define HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_SUPPORT

/* LE Set Path Loss Reporting Enable */
#define HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_SUPPORT

/* LE Set Transmit Power Reporting Enable */
#define HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_SUPPORT
#endif /* BT_LE */
#endif /* BT_5_2 */



/* ----------------------------------------------------------------------- */
/* ==== L2CAP Module Specific Flags ====================================== */
/* ----------------------------------------------------------------------- */
/*
 * L2CAP
 *
 * L2CAP module specific flag
 */
#define L2CAP

#ifdef L2CAP
#ifdef BR_EDR_L2CAP
/*
 *  L2CAP_HAVE_GROUP_SUPPORT
 *
 *  This flag enables the L2CAP Group, and Connectionless (Broadcast) data
 *  transmission and reception in L2CAP module.
 *
 *  Dependency: None.
 */
/* #define L2CAP_HAVE_GROUP_SUPPORT */

/*
 *  L2CAP_HAVE_PING_INFO_SUPPORT
 *
 *  This flag enables support for sending the "L2CAP_EchoREQ" and
 *  "L2CAP_GetInfoREQ" commands its peer in a remote Bluetooth device.
 *  Specifically, this flag enables the l2ca_ping_req() and l2ca_getinfo_req()
 *  APIs, and the corresponding callback registration and handling.
 *  Refer the EtherMind Stack API Document, Part II, for details.
 *
 *  Dependency: None.
 */
#define L2CAP_HAVE_PING_INFO_SUPPORT

/*
 *  L2CAP_EXTENDED_FLOW_SPEC
 *
 *  This flag enables support for Extended Flow Specification Option.
 *  By default this flag should be disabled.
 *
 *  Dependency: BT_ENH_L2CAP.
 */
/* #define L2CAP_EXTENDED_FLOW_SPEC */
#endif /* BR_EDR_L2CAP */

/*
 *  L2CAP_DATA_TX_NO_WRITE_TASK
 *
 *  If this flag is defined then L2CAP avoids using Write Task for
 *  transmission of its PDUs on explicitly created Write Task's context.
 *  The data transmission is performed as and when the data are queued
 *  in its transmission queue.
 *
 *  On the other hand, if this flag is not defined, L2CAP queues a Bottom Half
 *  function (BH) to Write Task, to trigger the data transmission, as and when
 *  data are queued in its transmission queue. Actual transmission of data is
 *  done when the Write Task calls the queued BH.
 *
 *  [Refer EtherMind Programmer's Guide, for relevant details on the Write
 *  Task and the concept of Bottom Half function]
 *
 *  Dependency: This flag is available only for L2CAP Bluetooth specification
 *  v1.2 implementation. Hence, the BT_ARCH_1_2 must be defined.
 *
 *  Note: This flag is disabled by default. Enable only if the implications of
 *  this are completely understood. With this flag enabled, it is possible that
 *  the HCI Command and L2CAP data packets get interleaved while sending to
 *  the HCI transport layer.
 */
/* #define L2CAP_DATA_TX_NO_WRITE_TASK */

#ifdef BR_EDR_L2CAP
/*
 *  L2CAP_TX_QUEUE_FLOW
 *
 *  This flag enables L2CAP provides additional APIs and processing to
 *  application so that it is possible to know and control buffer occupancy
 *  of the L2CAP Transmission Queue.
 *
 *  Dependency: This flag is available only for L2CAP Bluetooth specification
 *  v1.2 implementation. Hence, the BT_ARCH_1_2 must be defined.
 */
#define L2CAP_TX_QUEUE_FLOW

/*
 *  L2CAP_TX_QUEUE_FLOW_ON_CALLBACK
 *
 *  This flag enables L2CAP to indicate Tx Flow ON callback to the PSM registered
 *  handler, when the local Tx queue becomes available for Tx by upper layers.
 *
 *  Dependency: None.
 */
#define L2CAP_TX_QUEUE_FLOW_ON_CALLBACK

/*
 *  L2CAP_TX_COMPLETE_CALLBACK
 *
 *  This flag enables L2CAP transmission complete callback.
 *
 *  Dependency: None
 */
/* #define L2CAP_TX_COMPLETE_CALLBACK */

/*
 *  L2CAP_CHANNEL_DATAWRITE_CALLBACK
 *
 *  This flag enables L2CAP transmission complete callback
 *  on every channel.
 *
 *  Dependency: None
 */
/* #define L2CAP_CHANNEL_DATAWRITE_CALLBACK */
#endif /* BR_EDR_L2CAP */

/*
 *  L2CAP_HAVE_GET_REMOTE_CID_SUPPORT
 *
 *  This flag enables L2CAP Get Remote CID support.
 *  This protects l2cap_get_remote_cid() API. Currently required for AVDTP Assitive Mode.
 *
 *  Dependency: None
 */
#define L2CAP_HAVE_GET_REMOTE_CID_SUPPORT

/*
 *  BT_L2CAP_FIXED_CHANNEL
 *
 *  This flag enables L2CAP Fixed Channel Support
 *
 *  Dependency: BT_3_0 or BT_4_0.
 */
#if ((defined BT_3_0) || (defined BT_4_0))
#define BT_L2CAP_FIXED_CHANNEL
#endif /* (defined BT_3_0) || (defined BT_4_0) */

/*
 *  BT_L2CAP_FIXED_CHANNEL_TX_COMPLETE
 *
 *  This flag enables L2CAP Fixed Channel Tx Complete Event
 *  through callback.
 *
 *  Dependency: BT_L2CAP_FIXED_CHANNEL.
 */
#ifdef BT_L2CAP_FIXED_CHANNEL
#define BT_L2CAP_FIXED_CHANNEL_TX_COMPLETE
#endif /* BT_L2CAP_FIXED_CHANNEL */

/*
 *  L2CAP_LE_SLAVE
 *
 *  This flag enables L2CAP Connection Parameter Update Request Command
 *
 *  Dependency: GAP_PERIPHERAL.
 */
#ifdef GAP_PERIPHERAL
#define L2CAP_LE_SLAVE
#endif /* GAP_PERIPHERAL */

/*
 *  L2CAP_LE_MASTER
 *
 *  This flag enables L2CAP Connection Parameter Update Response
 *
 *  Dependency: GAP_CENTRAL.
 */
#ifdef GAP_CENTRAL
#define L2CAP_LE_MASTER
#endif /* GAP_CENTRAL */

/*
 *  L2CAP_NO_PARAM_CHECK
 *
 *  This flag disables the check in L2CAP APIs to verify the parameter memory
 *  and if the parameter values are within specification defined valid range
 *
 *  When application using L2CAP APIs are validated and
 *  it is ensured that none of the API call path is using
 *  parameters with invalid value, this flag can be enabled
 *  to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define L2CAP_NO_PARAM_CHECK */

#if ((defined BT_4_1) && (defined BT_LE))
/*
 *  L2CAP_SUPPORT_CBFC_MODE
 *
 *  This flag enables the L2CAP Credit Based Flow Control mode defined in
 *  Bluetooth Specification 4.1
 *
 *  Dependency: BT_4_1
 */
#define L2CAP_SUPPORT_CBFC_MODE

/*
 *  L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
 *
 *  This flag enables the application with access to the Local/Remote
 *  channel identifier in the connection context
 *
 *  Dependency: L2CAP_SUPPORT_CBFC_MODE
 */
#define L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
#endif /* BT_4_1 && BT_LE */

/*
 *  L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
 *
 *  For L2CAP CBFC, this flag enables the Upper Layer(UL) to define Low Water Mark
 *  for receive credits in the L2CAP_PSM_CBFC structure, while registering
 *  L2CAP LE_PSM.
 *
 *  In CBFC mode, L2CAP will inform application/UL about receive credits
 *  reached low water mark once credits is less than or equal to
 *  the Low Water Mark defined by the application/UL.
 *
 *  Dependency: L2CAP_SUPPORT_CBFC_MODE
 */
#ifdef L2CAP_SUPPORT_CBFC_MODE
#define L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
#endif /* L2CAP_SUPPORT_CBFC_MODE */

/*
 *  L2CAP_ALLOC_FOR_CBFC_SDU
 *
 *  For L2CAP CBFC, this flag enables allocation of memory to frame
 *  complete L2CAP SDU, which includes Information Payload, along with the
 *  associated L2CAP Header in the l2ca_channel_data_write() interface.
 *
 *  See the description of 'l2ca_channel_data_write()', for more details.
 *
 *  Dependency: L2CAP_SUPPORT_CBFC_MODE
 */
#ifdef L2CAP_SUPPORT_CBFC_MODE
#define L2CAP_ALLOC_FOR_CBFC_SDU
#endif /* L2CAP_SUPPORT_CBFC_MODE */

/*
 *  L2CAP_SUPPORT_ECBFC_MODE
 *
 *  This flag enables the L2CAP Enhanced Credit Based Flow Control mode defined in
 *  Bluetooth Specification v5.2
 *
 *  Dependency: BT_EATT
 */
#ifdef BT_EATT
#define L2CAP_SUPPORT_ECBFC_MODE
#endif /* BT_EATT */


/*
 *  L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
 *
 *  For L2CAP ECBFC, this flag enables the Upper Layer(UL) to define Low Water Mark
 *  for receive credits in the L2CAP_PSM_ECBFC structure, while registering
 *  L2CAP LE_PSM.
 *
 *  In ECBFC mode, L2CAP will inform application/UL about receive credits
 *  reached low water mark once credits is less than or equal to
 *  the Low Water Mark defined by the application/UL.
 *
 *  Dependency: L2CAP_SUPPORT_ECBFC_MODE
 */
#ifdef L2CAP_SUPPORT_ECBFC_MODE
#define L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/*
 *  L2CAP_ALLOC_FOR_ECBFC_SDU
 *
 *  For L2CAP ECBFC, this flag enables allocation of memory to frame
 *  complete L2CAP SDU, which includes Information Payload, along with the
 *  associated L2CAP Header in the l2ca_channel_data_write() interface.
 *
 *  See the description of 'l2ca_channel_data_write()', for more details.
 *
 *  Dependency: L2CAP_SUPPORT_ECBFC_MODE
 */
#ifdef L2CAP_SUPPORT_ECBFC_MODE
#define L2CAP_ALLOC_FOR_ECBFC_SDU
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

#endif /* L2CAP */

/* ----------------------------------------------------------------------- */
/* ==== SDP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#ifdef SDP
/*
 *  SDP_SERVER
 *
 *  This flag enables the SDP "Server" functionalities. If this flag is
 *  defined, then upper layer profiles, and applications, can register their
 *  "Service Record", and activate/deactivate the same whenever required.
 *  Also, other remote SDP "Client" Bluetooth devices will be able to search
 *  and find the services (profiles) available on the local device,
 *  and their attributes and parameters.
 *
 *  Dependency: The SDP Database module specific files must be included
 *  in the build.
 */
#define SDP_SERVER

/*
 *  SDP_CLIENT
 *
 *  This flag enables the SDP "Client" functionalities. If this flag is
 *  defined, then upper layer profiles, and applications, will be able to
 *  query for services, and their attributes and parameters, on other remote
 *  Bluetooth devices having SDP "Server" functionalities.
 *
 *  All the SDP APIs, which is described in EtherMind Stack API, Part II,
 *  is available only if this flag is defined.
 *
 *  Dependency: None.
 */
#define SDP_CLIENT

/*
 *  SDP_HAVE_HANDLE_WITH_CB
 *
 *  If defined, this flag enables an extra parameter in SDP Callback to
 *  application to send the SDP_HANDLE parameter. This helps in identifying
 *  the remote Bluetooth device (using BD_ADDR) when multiple SDP requests
 *  are initiated to multiple Bluetooth devices at the same time.
 *
 *  Dependency: None
 */
/* #define SDP_HAVE_HANDLE_WITH_CB */

/*
 *  SDP_CLIENT_HAVE_PARTIAL_SSA_SUPPORT
 *
 *  This flag is used to control the behaviour of SDP Client on receiving
 *  Service Search Attribute (SSA) Response.
 *
 *  If this flag is defined, SDP Client will pass the response received
 *  from the peer (even if it is a partial response) to the application.
 *
 *  After receiving the partial response, application can choose to
 *  continue with the same SSA request by invoking the SDP SSA API again
 *  or can call some other SDP APIs like Close or Service Search
 *  or Attribute Search.
 *
 *  If this flag is not defined, SDP Client will take care of the partial
 *  response received from the peer and try to pack the entire response
 *  received through multiple SSA responses into the buffer provided by
 *  the application through SDP SSA request API.
 *
 *  Note: By default this flag should be disabled.
 *
 *  Dependency: SDP_CLIENT must be defined.
 */
/* #define SDP_CLIENT_HAVE_PARTIAL_SSA_SUPPORT */

#ifdef SDP_SERVER
/*
 *  SDP_SERVER_HAVE_CB_IND_SUPPORT
 *
 *  If application wants to get notified when a peer device tries to
 *  perform SDP query with the local device and when the peer initiated
 *  SDP operation is complete, this flag shall be enabled and application
 *  shall register a callback with SDP Server using the SDP Server API
 *  "BT_sdp_server_register_ind_cb()".
 *
 *  If this flag is defined, and the application has registered callback
 *  using the above mentioned API, SDP Server will indicate following
 *  events to application:
 *  - SDP_SERVER_CONNECT_IND   :
 *    When a peer device establishes L2CAP connection
 *    with local SDP Server
 *
 *  - SDP_SERVER_DISCONNECT_IND:
 *    When the already existing L2CAP connection
 *    with local SDP server is dropped
 *
 *  Note: With both the above event indications, the SDP Server Callback
 *  will also provide the 'event_data' parameter which will contain
 *  the Bluetooth device address of the peer device and the 'event_datalen'
 *  parameter which will be the length of the Bluetooth device address
 *  of the peer device (i.e. 6 octet).
 *
 *  Dependency: SDP_SERVER must be defined.
 */
#define SDP_SERVER_HAVE_CB_IND_SUPPORT

/*
 *  SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT
 *
 *  This flag shall be defined to create host stack library without
 *  any dependency on the SDP database (i.e., library and SDP Database
 *  can be built separately).
 *
 *  Dependency: SDP_SERVER must be defined.
 */
#define SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT

/*
 *  SDP_SERVER_HAVE_NO_MAX_REC_DEPENDENCY
 *
 *  This flag shall be defined to have no dependency on the maximum service
 *  record present in the SDP database, while responding to Service Search
 *  or Service Search Attribute requests.
 *  Keeping this a compile time feature flag, as for some memory constraint
 *  system, still having the maximum record handle in BT_limits.h
 *  'SDP_MAX_REC_HANDLES' and usage of the same during the handling of SDP
 *  requests might be useful. When this feature flag is defined, SDP does one
 *  additional dynamic memory allocation, which can be avoided (if required)
 *  in such memory constrained systems, by not defining this feature flag
 *  and have the limitation of not getting all the service record related
 *  information on the peer device.
 *
 *  Dependency: SDP_SERVER must be defined.
 */
#define SDP_SERVER_HAVE_NO_MAX_REC_DEPENDENCY

/*
 * SDP_SERVER_ACCESS_RFCOMM_CH
 *
 * This flag enables to access the RFCOMM channel in the SDP record.
 * Enable this flag to add support updating the RFCOMM chnnel in the SDP record.
 *
 * Dependency: SDP_SERVER must be defined.
 *
 * Default: Enable
 */
#define SDP_SERVER_ACCESS_RFCOMM_CH

/*
 * SDP_DYNAMIC_DB
 *
 * This flag enables APIs to support dynamic creation of the SDP database.
 * The static database shall not be included in the build with this flag
 * defined.
 *
 * Dependency: SDP_SERVER must be defined.
 *
 * Default: Disable
 */
/* #define SDP_DYNAMIC_DB */
#endif /* SDP_SERVER */
#endif /* SDP */

/* ----------------------------------------------------------------------- */
/* ==== RFCOMM Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef RFCOMM

/*
 *  RFCOMM_ENABLE_SUPPORT_APIS
 *
 *  This compilation flag enables additional RFCOMM APIs for opening and
 *  closing an RFCOMM Session and DLC, as defined in rfcomm_support.h.
 *  These APIs are not defined in BT_rfcomm_api.h. Hence, these APIs are not
 *  directly available for applications to use. Currently, these APIs are only
 *  used in various RFCOMM test cases (such as, UPFs).
 *
 *  Dependency: None.
 */
/* #define RFCOMM_ENABLE_SUPPORT_APIS */

/*
 *  RFCOMM_CREDIT_TX_USE_BH
 *
 *  This flag enables implementation of RFCOMM Credit Transmission related
 *  API function to make use of Write Task (and, Bottom Halves) for carrying
 *  out core work of sending and updating Credit information.
 *
 *  Dependency: None.
 */
#define RFCOMM_CREDIT_TX_USE_BH

/*
 *  RFCOMM_ENABLE_FC_FROM_APP
 *
 *  This flag enables control for RFCOMM Credit Based Flow Control from
 *  application.
 *
 *  Dependency: None.
 */
#define RFCOMM_ENABLE_FC_FROM_APP

/*
 *  RFCOMM_USE_L2CAP_ERTM
 *
 *  This flag enables RFCOMM to use L2CAP's Enhanced Retransmission Mode,
 *  instead of default Basic Mode.
 *
 *  Dependency: BT_ENH_L2CAP must be defined.
 */
#ifdef BT_ENH_L2CAP
/* #define RFCOMM_USE_L2CAP_ERTM */
#endif /* BT_ENH_L2CAP */

/*
 *  RFCOMM_USE_L2CAP_ERTM_RUNTIME_CONFIG
 *
 *  This flag enables RFCOMM to use L2CAP's Enhanced Retransmission Mode, and allow using ERTM mode
 *  instead of default Basic Mode through runtime selection.
 *  See BT_rfcomm_set_l2cap_operation_mode() API.
 *
 *  Dependency: RFCOMM_USE_L2CAP_ERTM must be defined.
 */
#ifdef RFCOMM_USE_L2CAP_ERTM
#define RFCOMM_USE_L2CAP_ERTM_RUNTIME_CONFIG
#endif /* RFCOMM_USE_L2CAP_ERTM */

/*
 *  RFCOMM_ENABLE_USER_CXT_IN_HANDLE
 *
 *  This flag enables RFCOMM Application to send context data using
 *  RFCOMM Handle when invoking RFCOMM APIs
 *
 *  Dependency: None
 */
#define RFCOMM_ENABLE_USER_CXT_IN_HANDLE

/*
 *  RFCOMM_FLUSHABLE_PACKET
 *
 *  This flag enables setting packets flushable over RFCOMM connection.
 *
 *  Dependency: BT_PBF
 */
#ifdef BT_PBF
/* #define RFCOMM_FLUSHABLE_PACKET */
#endif /* BT_PBF */

#endif /* RFCOMM */

/* ----------------------------------------------------------------------- */
/* ==== BNEP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
#ifdef BNEP
/*
 *  BNEP_WRITE_NO_WRITE_TASK
 *
 *  This flag disables use of Write Task while writing data over BNEP.
 *  This should be disabled by default.
 *
 *  Dependency: None.
 */
/* #define BNEP_WRITE_NO_WRITE_TASK */
#endif /* BNEP */

/* ---------------------------------------------------------------------- */
/* ==== AVDTP Module Specific Flag ====================================== */
/* ---------------------------------------------------------------------- */
#ifdef AVDTP
/*
 *  AVDTP_HAVE_CONTENT_PROTECTION
 *
 *  This flag is used to enable the content protection feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_CONTENT_PROTECTION

/*
 *  AVDTP_HAVE_REPORTING_SERVICE
 *
 *  This flag is used to enable the reporting service feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_REPORTING_SERVICE

/*
 *  AVDTP_HAVE_RECOVERY_SERVICE
 *
 *  This flag is used to enable the recovery service feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_RECOVERY_SERVICE

/*
 *  AVDTP_HAVE_ROHC_SERVICE
 *
 *  This flag is used to enable the robust header compression service feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_ROHC_SERVICE

/*
 *  AVDTP_HAVE_MULTIPLEXING
 *
 *  This flag is used to enable the multiplexing feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_MULTIPLEXING

/*
 *  AVDTP_ASSISTIVE_MODE
 *
 *  This flag is used to enable interfaces to share media channel parameters
 *  to the application. Useful when stack used with TI DualMode controller that
 *  supports Assistive A2DP to decode and play-out media.
 *
 *  Dependency: None.
 */
/* #define AVDTP_ASSISTIVE_MODE */

/*
 *  AVDTP_HAVE_GET_ALL_CAPABILITIES
 *
 *  This flag enables AVDTP Get All Capability Feature of AVDTP 1.3 in the
 *  EtherMind AVDTP module.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_GET_ALL_CAPABILITIES

/*
 *  AVDTP_HAVE_DELAY_REPORTING
 *
 *  This flag enables AVDTP Delay Reporting Feature of AVDTP 1.3 in the
 *  EtherMind AVDTP module.
 *
 *  Dependency: AVDTP_HAVE_GET_ALL_CAPABILITIES.
 */
#define AVDTP_HAVE_DELAY_REPORTING

/*
 *  AVDTP_HAVE_GENERAL_REJECT
 *
 *  This flag enables General Reject Feature of AVDTP 1.3 in the
 *  EtherMind AVDTP module.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_GENERAL_REJECT

/*
 *  HAVE_AVDTP_TEST_MODE
 *
 *  This flag enables AVDTP Test Mode in EtherMind AVDTP module. Enable this flag
 *  to validate a few AVDTP -ve test cases with PTS.
 *
 *  By default it shall be disabled.
 *
 *  Dependency: None.
 */
/* #define HAVE_AVDTP_TEST_MODE */

/*
 *  HAVE_AVDTP_SET_GET_SIG_CH_OUTMTU
 *
 *  This flag enables to get/set the Out MTU of a given AVDTP signalling channel.
 *
 *  Typically this is used to set the outmtu to a smaller value to enable sending
 *  AVDTP signalling messages in fragments, required for some of the qualification
 *  test cases
 *
 *  By default it shall be disabled.
 *
 *  Dependency: None.
 */
#define HAVE_AVDTP_SET_GET_SIG_CH_OUTMTU

/*
 *  HAVE_AVDTP_SEND_FRAGMENTED_SIG_MSG
 *
 *  This flag enables to send the AVDTP signalling messages in fragments
 *  when message length is greater than the Out MTU of Signalling Channel.
 *
 *  By default it shall be enabled.
 *
 *  Dependency: None.
 */
#define HAVE_AVDTP_SEND_FRAGMENTED_SIG_MSG

/*
 *  HAVE_AVDTP_MEDIA_PKT_FLUSHABLE
 *
 *  This flag is used to set the AVDTP Media packets as Flushable.
 *
 *  Dependency: None.
 */
#define HAVE_AVDTP_MEDIA_PKT_FLUSHABLE
#endif /* AVDTP */

/* ----------------------------------------------------------------------- */
/* ==== AVCTP Module Specific Flags ====================================== */
/* ----------------------------------------------------------------------- */
#ifdef AVCTP
/*
 *  AVCTP_1_3
 *
 *  This flag enable the AVCTP 1.3 specific features in the
 *  EtherMind AVCTP module.
 *
 *  AVCTP 1.4 Changes includes Editorial aspects. The Errata 733 and 2689
 *  refer to editorial changes i.e. Appending of text and Typo Correction
 *  in the specification.
 */
#define AVCTP_1_3

/*
 *  AVCTP_CHECK_BROWSING_CH_MTU
 *
 *  AVCTP Browsing Channel defines 335 as the minimum MTU size.
 *  This flag is used to enable validation of the AVCTP Browsing Channel MTU
 *  size received from the peer device.
 *
 *  Dependency: AVCTP_1_3
 */
#ifdef AVCTP_1_3
/* #define AVCTP_CHECK_BROWSING_CH_MTU */
#endif /* AVCTP_1_3 */

/*
 *  AVCTP_CONNECT_NO_WRITE_TASK
 *
 *  If this flag is defined then AVCTP Connection Request avoids using
 *  Write Task for processing of the connection request and transmission
 *  of corresponding PDUs.
 *
 *  This flag helps in handling the collision scenario, where local device
 *  tries to initiate AVRCP connection, when the peer device has already started
 *  the underlying AVCTP transport connection but the AVCTP connection is not
 *  complete, so the local AVRCP module is unaware of the on-going AVCTP connection.
 *
 * By default, this flag shall be enabled.
 */
#define AVCTP_CONNECT_NO_WRITE_TASK

/*
 *  HAVE_AVRCP_MSG_COPY
 *
 *  This flag enables allocation of buffer and keeping a copy of AVRCP message
 *  in AVCTP Layer.
 *  When this flag is enabled, instead of freeing the AVRCP message buffer on
 *  receiving AVRCP_MESSAGE_SEND_CNF event, application shall free
 *  the allocated buffer for AVRCP message immediately on returning from
 *  the called AVRCP APIs.
 *
 *  EtherMind AVCTP module.
 */
/* #define HAVE_AVRCP_MSG_COPY */

/*
 *  AVCTP_SUPPORT_MULTIPLE_CHANNELS
 *
 *  This flag enables the stack to accept and create multiple AVCTP channels
 *  for the Control/Browsing PSM with a same remote device.
 *
 *  This flag is disabled by default.
 */
/* #define AVCTP_SUPPORT_MULTIPLE_CHANNELS */
#endif /* AVCTP */

/* ----------------------------------------------------------------------- */
/* ==== OBEX Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
#ifdef OBEX

/*
 *  OBEX_SERVER
 *
 *  This flag enables the OBEX "Server" functionalities.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_SERVER

/*
 *  OBEX_CLIENT
 *
 *  This flag enables the OBEX "Client" functionalities.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_CLIENT

/*
 *  OBEX_FRAGMENTED_HDR_SUPPORT
 *
 *  Some peer implementation sends OBEX packets in fragments of size 1 bytes.
 *  Default EtherMind OBEX implementation assumes that the first OBEX packet
 *  will be of size at least of length 3 bytes (which will include the OBEX
 *  packet length).
 *
 *  Since IrDA specification or the Bluetooth adaptation of it does not impose
 *  such restriction, it is possible to receive an entire OBEX packet in
 *  multiple fragments of different sizes. It is also possible to receive parts
 *  of more than one OBEX packet in single underlying transport packet
 *  (RFCOMM/L2CAP).
 *
 *  This flag enables handling of fragmented OBEX header.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_FRAGMENTED_HDR_SUPPORT

/*
 *  OBEX_OVER_L2CAP
 *
 *  This flag enables handling of L2CAP transport support for OBEX.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_OVER_L2CAP

/*
 *  OBEX_ENH_SUPPORT
 *
 *  This flag enables enhanced OBEX commands support
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_ENH_SUPPORT

/*
 *  OBEX_ASCII_TO_FROM_UNICODE
 *
 *  This flag enables Encode/Decode of ASCII string to/from Unicode format in the OBEX Layer.
 *  and OBEX Layer assumes, the upper profiles/applications processes the Unicode text in the ASCII format.
 *
 *  When this flag is enabled, then the behaviour is as following
 *
 *  In the OBEX Tx. path:
 *    - ASCII text will be encoded to Unicode format & sent.
 *  In the OBEX Rx. path:
 *    - Unicode text will be decoded to ASCII format & pass it on to profiles/applications.
 *
 *  Example:
 *  As we see in the OBEX Spec. v1.4, Section #2.1
 *  " For Unicode text, the length field (immediately following the header ID) includes the
 *    2 bytes of the null terminator (0x00, 0x00). Therefore the length of the string Jumar
 *    would be 12 bytes; 5 visible characters plus the null terminator, each two bytes in length"
 *
 *  So, in the Tx. path, upper profiles/applications are expected to send 'Jumar' as 5 byte + 1(null) char.
 *  and OBEX layer encode this 6 byte text to 12 byte text by converting each 1 byte char to 2 byte char(by appending
 *  0x00  to each character). In receive path, OBER layer decode this 12 byte text to 6 byte text by converting 2 byte char
 *  to 1 byte char(by removing MSB of each 2 byte char).
 *
 *  Note:
 *  1. Enable this flag, when the format of the string text is in ASCII format.
 *  2. If the format of string is not known in advance, then disable this flag &
 *     application has to take care of Encoding/Decoding to/from Unicode format.
 *
 *  By default, this flag will be enabled.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_ASCII_TO_FROM_UNICODE

/*
 *  OBEX_SUPPORT_RELIABLE_SSN
 *
 *  This flag enables support for Reliable Sessions over OBEX
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
/* #define OBEX_SUPPORT_RELIABLE_SSN */

/*
 *  OBEX_AL
 *
 *  This flag enables the OBEX Adaptation Layer interfaces for use by the few
 *  profiles that need them
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 *
 *  NOTE: This layer will be deprecated once the new OBEX interfaces have been
 *        put to use by all the profiles
 */
/* #define OBEX_AL */

/*
 *  OBEX_ENABLE_CONNECT_ID_HDR_CHECK
 *
 *  If defined, this flag enables OBEX_HDR_CONNECTION_ID check in the OBEX based
 *  Profiles.
 *
 *  Note:
 *  1. According to GOEP v2.1 specification, the OBEX Connection Identifier header is
 *     mandatory in the OBEX requests, if the  Target header is used when establishing
 *     the OBEX connection. This header should  be used in the first request of the GET
 *     operation only.
 *  2. Also, According to OBEX v4.0, Connection Id header When in use, it must be the
 *     first header in the request.
 *
 *   But few older implementations are not sending the Connection Identifier header in the
 *   OBEX GET/PUT request. So optionally, we can disable 'Connection Identifier' Header check
 *   in the profile implementation.
 *
 *   By default, this will be disabled.
 *
 *   Dependency: NONE
 */
/* #define OBEX_ENABLE_CONNECT_ID_HDR_CHECK */

/* Check at-least one among RFCOMM and OBEX_OVER_L2CAP flag is defined */
#if (!((defined RFCOMM) || (defined OBEX_OVER_L2CAP)))
#error "Atleast one of RFCOMM, OBEX_OVER_L2CAP shall be defined"
#endif /* (!((defined RFCOMM) || (defined OBEX_OVER_L2CAP))) */

#endif /* OBEX */

/* ----------------------------------------------------------------------- */
/* ==== MCAP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
#ifdef MCAP

/*
 *  MCAP_STANDARD_COMMANDS
 *
 *  This flag enables support for Standard commands in MCAP.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_STANDARD_COMMANDS

/*
 *  MCAP_CLOCK_SYNC
 *
 *  This flag enables support for Clock Synchronization in MCAP.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_CLOCK_SYNC

/*
 *  MCAP_STORAGE
 *
 *  This flag enables MCAP to store configuration data for established
 *  MCAP Data Links (MDLs), so that the configuration information could
 *  be re-used during reconnection.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_STORAGE

/*
 * MCAP_BQB_TEST
 *
 * This flag is enabled for qualification purposes, need special handling for
 * specific tests which may normally not be desired.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_BQB_TEST

/*
 *  MCAP_TEST
 *
 *  This flag enables MCAP routines that are used for test purposes.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_TEST

#endif /* MCAP */


/* ----------------------------------------------------------------------- */
/* ==== HFP Unit Module Specific Flags =================================== */
/* ----------------------------------------------------------------------- */
#ifdef HFP_UNIT
/*
 *  HFP_UNIT_1_5
 *
 *  This flag enables HFP-1.5 features and API functions for HFP Unit module.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_1_5

/*
 *  HFP_UNIT_1_6
 *
 *  This flag enables HFP-1.6 features and API functions for HFP Unit module.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_1_6

/*
 *  HFP_UNIT_1_7
 *
 *  This flag enables HFP-1.7 features and API functions for HFP Unit module.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_1_7

/*
 *  HFP_UNIT_1_8
 *
 *  This flag enables HFP-1.8 features and API functions for HFP Unit module.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_1_8

/*
 *  HFP_UNIT_HAVE_RFCOMM_CB_SUPPORT
 *
 *  This flag enables RFCOMM Callback support in HFP Unit module.
 *  If this flag is enabled, application can to register RFCOMM Callback
 *  using BT_hfp_unit_register_rfcomm_cb() API.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_HAVE_RFCOMM_CB_SUPPORT
#endif /* HFP_UNIT */

/* ----------------------------------------------------------------------- */
/* ==== HFP AG Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
#ifdef HFP_AG
/*
 *  HFP_AG_1_5
 *
 *  This flag enables handling of HFP-1.5 commands in HFP-AG application.
 *
 *  Dependency: None.
 */
#define HFP_AG_1_5

/*
 *  HFP_AG_1_6
 *
 *  This flag enables handling of HFP-1.6 commands in HFP-AG application.
 *
 *  Dependency: None.
 */
#define HFP_AG_1_6

/*
 *  HFP_AG_1_7
 *
 *  This flag enables handling of HFP-1.7 commands in HFP-AG application.
 *
 *  Dependency: None.
 */
#define HFP_AG_1_7

/*
 *  HFP_AG_1_8
 *
 *  This flag enables handling of HFP-1.8 commands in HFP-AG application.
 *
 *  Dependency: None.
 */
#define HFP_AG_1_8
#endif /* HFP_AG */

/* ----------------------------------------------------------------------- */
/* ==== A2DP Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */

#ifdef A2DP

#ifdef A2DP_SOURCE
/*
 *  A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
 *
 *  This flag enables support for Multiple SBC Media Frames Write from the application.
 *
 *  Note:
 *  - When this flag is enabled, EtherMind A2DP source module sends the given SBC frames
 *    immediately to AVDTP without waiting till maximum possible media packet length is
 *    formed. This feature is useful when EtherMind A2DP source is used to stream to
 *    multiple A2DP sink devices.
 *
 *  - If A2DP source application streams the encoded SBC frames with each media frame of same
 *    size (say 'N') and the calculated 'Maximum Payload Length' (say 'M'), then the
 *    application can send 'M'/'N' media frames at maximum using BT_a2dp_media_write() API.
 *
 *  Dependency: A2DP_SOURCE.
 */
/* #define A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */
#endif /* A2DP_SOURCE */

/*
 *  A2DP_MPEG_1_2
 *
 *  This flag enables A2DP MPEG-1,2 Codec Support.
 *
 *  Dependency: A2DP.
 */
#define A2DP_MPEG_1_2

/*
 *  A2DP_MPEG_2_4
 *
 *  This flag enables A2DP MPEG-2,4 Codec Support.
 *
 *  Dependency: A2DP.
 */
#define A2DP_MPEG_2_4

/*
 *  A2DP_VENDOR_CODEC
 *
 *  This flag enables A2DP Vendor Specific Codec Support.
 *
 *  Dependency: A2DP.
 */
#define A2DP_VENDOR_CODEC

/*
 *  A2DP_1_3
 *
 *  This flag enables A2DP 1.3 specific features in the
 *  EtherMind A2DP module.
 *
 *  Dependency: AVDTP_HAVE_DELAY_REPORTING
 */
#define A2DP_1_3

#endif /* A2DP */


/* ----------------------------------------------------------------------- */
/* ==== AVRCP Module Specific Flags ====================================== */
/* ----------------------------------------------------------------------- */
#ifdef AVRCP
/*
 *  AVRCP_1_3
 *
 *  This flag enable the AVRCP 1.3 specific features in the
 *  EtherMind AVRCP module.
 */
#define AVRCP_1_3

/*
 *  AVRCP_1_4
 *
 *  This flag enable the AVRCP 1.4 specific features in the
 *  EtherMind AVRCP module.
 */
#define AVRCP_1_4

/*
 *  AVRCP_1_6
 *  This flag enables the AVRCP 1.6 specific new features
 *   - Number of Items
 *   - Cover Art
 *  in the EtherMind AVRCP module.
 */
#define AVRCP_1_6

#ifdef AVRCP_1_6
/*
 *  AVRCP_GET_NUM_OF_ITEMS
 *  This flag enables the GetNumberOfItems Feature of AVRCP 1.6 specific features in the
 *  EtherMind AVRCP module.
 */
#define AVRCP_GET_NUM_OF_ITEMS

/*
 *  AVRCP_COVER_ART
 *  This flag enables the CoverArt Feature of AVRCP 1.6 specific features in the
 *  EtherMind AVRCP module.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define AVRCP_COVER_ART

#ifdef AVRCP_COVER_ART
/*
 *  AVRCP_COVER_ART_INITIATOR
 *  This flag enables the CoverArt Initiator in the EtherMind AVRCP module.
 *
 *  Dependency: AVRCP_COVER_ART
 */
#define AVRCP_COVER_ART_INITIATOR

/*
 *  AVRCP_COVER_ART_RESPONDER
 *  This flag enables the CoverArt Responder in the EtherMind AVRCP module.
 *
 *  Dependency: AVRCP_COVER_ART
 */
#define AVRCP_COVER_ART_RESPONDER

#endif /* AVRCP_COVER_ART */

#endif /* AVRCP_1_6 */

#endif /* AVRCP */

/* ----------------------------------------------------------------------- */
/* ==== OPP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#ifdef OPP
/*
 *  OPP_EXTENDED_CALLBACK
 *
 *  If defined, this flag enables extra OBEX Header parameters in OPP Callback
 *  to the application. Application can then parse the received OBEX headers
 *  from the peer and extract information, which is not readily available
 *  from OPP implementation.
 *
 *  Dependency: None
 */
/* #define OPP_EXTENDED_CALLBACK */

/*
 *  OPP_1_2
 *
 *  If defined, this flag enables OPP 1.2 features Support.
 *  This flag is common for both OPP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define OPP_1_2

/*
 *  OPP_SUPPORT_ANY_OBJECT_TYPE_PULL
 *
 *  If defined, this flag enables OPP client to request
 *  pull for any object other than business card. This is disabled
 *  by default as it is not a part of specification.
 *
 *  Dependency: OPP
 */
/* #define OPP_SUPPORT_ANY_OBJECT_TYPE_PULL */
#endif /* OPP */

/* ----------------------------------------------------------------------- */
/* ==== FTP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#ifdef FTP
/*
 *  FTP_EXTENDED_CALLBACK
 *
 *  If defined, this flag enables extra OBEX Header parameters in FTP Callback
 *  to the application. Application can then parse the received OBEX headers
 *  from the peer and extract information, which is not readily available
 *  from FTP implementation.
 *
 *  Dependency: None
 */
/* #define FTP_EXTENDED_CALLBACK */

/*
 *  FTP_1_3
 *
 *  If defined, this flag enables FTP 1.3 features Support.
 *  This flag is common for both FTP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP and OBEX_ENH_SUPPORT
 */
#define FTP_1_3
#endif /* FTP */

/* ----------------------------------------------------------------------- */
/* ==== CTN Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#if ((defined CTN_CCE) || (defined CTN_CSE))
/*
 *  CTN_SUPPORT_NOTIFICATION
 *
 *  If defined, this flag enables CTN Notification Service Support.
 *  This flag is common for both CTN Client and Server implementation.
 *
 *  Dependency: None
 */
#define CTN_SUPPORT_NOTIFICATION

/*
 *  HAVE_CTN_SYNC_INSTANCE
 *
 *  If defined, this flag enables CTN SyncInstacce Support.
 *  Refer to Generic PIM Profile(GPP) for more details.
 *  This flag is common for both CTN Client and Server implementation.
 *
 *  Dependency: None
 */
/* #define HAVE_CTN_SYNC_INSTANCE */
#endif /* ((defined CTN_CCE) || (defined CTN_CSE)) */

/* ----------------------------------------------------------------------- */
/* ==== MAP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#if ((defined MAP_MCE) || (defined MAP_MSE))
/*
 *  MAP_SUPPORT_NOTIFICATION
 *
 *  If defined, this flag enables MAP Notification Service Support.
 *  This flag is common for both MAP Client and Server implementation.
 *
 *  Dependency: None
 */
#define MAP_SUPPORT_NOTIFICATION

/*
 *  MAP_1_2
 *
 *  If defined, this flag enables MAP 1.2 features Support.
 *  This flag is common for both MAP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define MAP_1_2

/*
 *  MAP_1_3
 *
 *  If defined, this flag enables MAP 1.3 features Support.
 *  This flag is common for both MAP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define MAP_1_3

/*
 *  MAP_1_4
 *
 *  If defined, this flag enables MAP 1.4 features Support.
 *  This flag is common for both MAP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define MAP_1_4

/*
 *  MAP_1_4_2
 *
 *  If defined, this flag enables MAP 1.4.2 features Support.
 *  This flag is common for both MAP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define MAP_1_4_2
#endif /* ((defined MAP_MCE) || (defined MAP_MSE)) */

/* ----------------------------------------------------------------------- */
/* ==== PBAP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
#if ((defined PBAP_PCE) || (defined PBAP_PSE))
/*
 *  PBAP_1_2
 *
 *  If defined, this flag enables PBAP 1.2 features Support.
 *  This flag is common for both PBAP Client and Server implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define PBAP_1_2
#endif /* ((defined PBAP_PCE) || (defined PBAP_PSE)) */

/* ----------------------------------------------------------------------- */
/* ==== BIP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
#ifdef BIP
/*
 *  BIP_1_2
 *
 *  If defined, this flag enables BIP 1.2 of BIP
 *  This flag is common for both BIP Initiator and BIP Responder implementation.
 *
 *  Dependency: OBEX_OVER_L2CAP
 */
#define BIP_1_2
#endif /* BIP */

/* ----------------------------------------------------------------------- */
/* ==== PAN Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#ifdef PAN
/*
 *  PAN_ROLE_PANU
 *
 *  PAN Role Define: PANU
 *
 *  Dependency: PAN
 */
#define PAN_ROLE_PANU

/*
 *  PAN_ROLE_NAP
 *
 *  PAN Role Define: NAP
 *
 *  Dependency: PAN
 */
#define PAN_ROLE_NAP

/*
 *  PAN_ROLE_GN
 *
 *  PAN Role Define: GN
 *
 *  Dependency: PAN
 */
#define PAN_ROLE_GN
#endif /* PAN */

/* ----------------------------------------------------------------------- */
/* ==== HID Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
#if ((defined HID_HOST) || (defined HID_DEVICE))
/*
 *  HID_1_1
 *
 *  This flag enables HIDv1.1 features and API functions HID module.
 *
 *  Dependency: HID
 */
#define HID_1_1

/*
 *  HID_1_0
 *
 *  This flag enables HIDv1.0 features and API functions HID module.
 *
 *  Dependency: HID
 */
/* #define HID_1_0 */

#ifdef HID_1_0
/*
 *  HID_SUPPORT_GET_IDLE
 *
 *  This flag enables HID message type GET_IDLE (defined only for HIDv1.0).
 *
 *  Dependency: HID_1_0
 */
#define HID_SUPPORT_GET_IDLE

/*
 *  HID_SUPPORT_SET_IDLE
 *
 *  This flag enables HID message type SET_IDLE (defined only for HIDv1.0).
 *
 *  Dependency: HID_1_0
 */
#define HID_SUPPORT_SET_IDLE

/*
 *  HID_SUPPORT_DATC
 *
 *  This flag enables HID message type DATC (defined only for HIDv1.0).
 *  This feature is associated with HID segmentation and reassembly
 *  functionality, which is deprecated in HIDv1.1.
 *
 *  Dependency: HID_1_0
 */
#define HID_SUPPORT_DATC

#ifdef HID_SUPPORT_DATC
/*
 *  HID_DEVICE_LONG_OUT_REPORT
 *
 *  This flag enables handling of received long out report on HID Device.
 *
 *  Dependency: HID_SUPPORT_DATC
 */
#define HID_DEVICE_LONG_OUT_REPORT
#endif /* HID_SUPPORT_DATC */
#endif /* HID_1_0 */
#endif /* ((defined HID_HOST) || (defined HID_DEVICE)) */

/* ----------------------------------------------------------------------- */
/* ==== EtherMind LITE Stack Specific Flags ============================== */
/* ----------------------------------------------------------------------- */
/*
 *  This flag is used to choose the LITE version of EtherMind Stack.
 *  At present, this flag affects HCI, L2CAP, SDP and RFCOMM Modules.
 *  This flag helps in removing unnecessary APIs and related functions
 *  for specific application (such as, Mono Headset Build).
 *
 *  Dependency: None.
 */
/* #define BT_LITE */

/* If BT_LITE is defined, choose features from below */
#ifdef BT_LITE

/* Common LITE Feature Flags */
#define BT_NO_BLUETOOTH_OFF
#define TIMER_NO_RESTART_TIMER

/* HCI LITE Feature Flags */
#define HCI_LITE
#define HCI_NO_SYNC_LOST_IND

/* SM LITE Feature Flags */
#define SM_LITE

/* L2CAP LITE Feature Flags */
#define L2CAP_NO_UNREGISTER_PSM

/* SDP LITE Feature Flags */
#ifdef SDP
#define SDP_NO_SS_REQ
#define SDP_NO_SA_REQ
#define SDP_NO_GET_RECORD_HANDLE
#define SDP_NO_GET_PTR_2_NEXT_ATTR_LIST
#define SDP_NO_GET_SERVICE_RECORD_HANDLE
#define SDP_NO_GET_SERVICE_CLASS_ID_LIST
#define SDP_NO_GET_SERVICE_RECORD_STATE
#define SDP_NO_GET_SERVICE_ID
#define SDP_NO_GET_BROWSE_GROUP_LIST
#define SDP_NO_GET_LANG_BASE_ATTR_ID_LIST
#define SDP_NO_GET_SERVICE_INFO_TIME_TO_LIVE
#define SDP_NO_GET_SERVICE_AVAILABILITY
#define SDP_NO_GET_BT_PROFILE_DESC_LIST
#define SDP_NO_GET_DOCUMENTATION_URL
#define SDP_NO_GET_CLIENT_EXECUTABLE_URL
#define SDP_NO_GET_ICON_URL
#define SDP_NO_GET_SERVICE_NAME
#define SDP_NO_GET_SERVICE_DESCRIPTION
#define SDP_NO_GET_PROVIDER_NAME
#define SDP_NO_GET_VERSION_NUMBER_LIST
#define SDP_NO_GET_SERVICE_DATABASE_STATE
#define SDP_NO_GET_GROUP_ID
#define SDP_NO_GET_ADDITIONAL_CHANNEL
#define SDP_NO_GET_PSM
#define SDP_NO_GET_ADDITIONAL_PSM

#define SDP_NO_GET_UINT32_ATTRIBUTE_VALUE
#define SDP_NO_GET_UUID_ATTRIBUTE_VALUE
#define SDP_NO_GET_STRING_ATTRIBUTE_VALUE
#define SDP_NO_GET_INT_ATTRIBUTE_VALUE

/* SDP Databse LITE Feature Flags */
#define DB_NO_INACTIVATE_RECORD
#endif /* SDP */

/* RFCOMM LITE Feature Flags */
#ifdef RFCOMM
#define RFCOMM_NO_QUERY_STATE
#define RFCOMM_NO_SEND_PN
#define RFCOMM_NO_SEND_RPN
#define RFCOMM_NO_SEND_MSC
#define RFCOMM_NO_SEND_RLS
#define RFCOMM_NO_SEND_TEST
#define RFCOMM_NO_SEND_FC
#define RFCOMM_NO_GET_LOCAL_RPN
#define RFCOMM_NO_SET_LOCAL_RPN
#define RFCOMM_NO_GET_LOCAL_PN
#define RFCOMM_NO_GET_LOCAL_MSC
#endif /* RFCOMM */

/* HFP Unit LITE Feature Flags */
#define HFP_UNIT_NO_STOP

/* HSP Unit LITE Feature Flags */
#define HSP_UNIT_NO_STOP

#endif /* BT_LITE */

/* ----------------------------------------------------------------------- */
/* ==== LE Security Manager Protocol Specific Flags ====================== */
/* ----------------------------------------------------------------------- */
#ifdef SMP

/*
 *  SMP_STORAGE
 *
 *  This flag enables Security Manager Protocol(SMP) module to store
 *  device bonding information and configuration data (such as, Long Term Key,
 *  Identity Resolving Key, Signature Key, default configuration etc.) in
 *  persistent storage device. SMP writes on to the persistent storage when
 *  on any update on the device database or configuration.
 *
 *  On next initialization, during BT_bluetooth_on(), SMP reads the stored
 *  configuration data from the persistent storage media.
 *
 *  Dependency: STORAGE_PERSISTENT_SUPPORT
 */
#define SMP_STORAGE
#if (defined SMP_STORAGE && !defined STORAGE_PERSISTENT_SUPPORT)
#error "Dependency Error for SMP_STORAGE"
#endif /* (defined SMP_STORAGE && !defined STORAGE_PERSISTENT_SUPPORT) */

/*
 *  SMP_MASTER
 *
 *  This flag enables SMP module master role functionalities.
 *
 *  Dependency: GAP_CENTRAL
 */
#ifdef GAP_CENTRAL
#define SMP_MASTER
#endif /* GAP_CENTRAL */

/*
 *  SMP_SLAVE
 *
 *  This flag enables SMP module slave role functionalities.
 *
 *  Dependency: GAP_PERIPHERAL
 */
#ifdef GAP_PERIPHERAL
#define SMP_SLAVE
#endif /* GAP_PERIPHERAL */

/*
 *  SMP_HAVE_ENCRYPT_API
 *
 *  This flag enables BT_smp_encrypt API that can be used by application
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_HAVE_ENCRYPT_API */

/*
 *  SMP_IGNORE_REAUTHENTICATION
 *
 *  This flag ignores authentication request on an encrypted link
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_IGNORE_REAUTHENTICATION */

/*
 *  SMP_NO_STORED_KEYS
 *
 *  This flag disables the Authentication API to check for bonded/authentication
 *  status of peer device. Also peer's shared key information is not stored in
 *  persistent storage. This API instead enables BT_smp_encrypt API that
 *  can be used by application
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_NO_STORED_KEYS */

#ifdef SMP_NO_STORED_KEYS
#define SMP_HAVE_ENCRYPT_API
#endif /* SMP_NO_STORED_KEYS */

/*
 *  SMP_NO_PARAM_CHECK
 *
 *  This flag disables the check in SMP APIs to verify the parameter memory
 *  and if the parameter values are within specification defined valid range
 *
 *  When application using SMP APIs are validated and
 *  it is ensured that none of the API call path is using
 *  parameters with invalid value, this flag can be enabled
 *  to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define SMP_NO_PARAM_CHECK */

/*
 *  SMP_DST_ENCRYPT_INFO
 *
 *  This flag enables SMP module encryption information distribution
 *  functionalities.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_DST_ENCRYPTION_INFO

/*
 *  SMP_DST_IRK
 *
 *  This flag enables SMP module Identity information distribution
 *  functionalities.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_DST_IDENTITY_INFO

/*
 *  SMP_DST_SRK
 *
 *  This flag enables SMP module SRK distribution functionalities.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_DST_SRK

/*
 *  SMP_HNDL_ENCRYPT_INFO
 *
 *  This flag enables SMP module to handle remote Signature Key information.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_HNDL_ENCRYPTION_INFO

/*
 *  SMP_HNDL_IDENTITY
 *
 *  This flag enables SMP module to handle remote Identity Information.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_HNDL_IDENTITY_INFO

/*
 *  SMP_HNDL_SRK
 *
 *  This flag enables SMP module to handle remote SRK and also enable to the
 *  signature related APIs to send signed data.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_HNDL_SRK

/*
 *  SMP_SUPPORT_UTIL_APIS
 *
 *  This flag enables SMP Utility APIs.
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_SUPPORT_UTIL_APIS

/*
 *  SMP_CHECK_ENCRYPTION_KEY_DIST_FOR_BONDING
 *
 *  This flag enables verification of received peer Encryption Information
 *  before initiating encryption procedure for bonded device
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_CHECK_ENCRYPTION_KEY_DIST_FOR_BONDING

/*
 *  SMP_FORCE_CLEAR_ENTITY
 *
 *  This flag enables SMP to free the entity and its device queue reference on disconnection
 *  of a device irrespective of bonded or non-bonded
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_FORCE_CLEAR_ENTITY */

/*
 *  SMP_CLEAR_UNBONDED_ENTITY
 *
 *  This flag enables SMP to free the entity and its device queue reference on disconnection
 *  of an non-bonded device. This flag does not have any effect if the
 *  SMP_FORCE_CLEAR_ENTITY is defined.
 *
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_CLEAR_UNBONDED_ENTITY

/*
 *  SMP_HAVE_OOB_SUPPORT
 *
 *  This flag enables SMP to support OOB data for authentication
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_HAVE_OOB_SUPPORT

/*
 *  SMP_DATA_SIGNING
 *
 *  This flag enables SMP data signing feature
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_DATA_SIGNING

/*
 *  SMP_LESC
 *
 *  This flag governs the Bluetooth 4.2 SMP Secure connections Feature for Host
 *
 *  Dependency: BT_LE_4_2 && SMP
 */
#define SMP_LESC

/*
 *  SMP_HAVE_TBX_PL_ENC
 *
 *  This flag enables the use of platform specific Encrypt function
 *
 *  Dependency:
 */
/* #define SMP_HAVE_TBX_PL_ENC */

/*
 *  SMP_HAVE_TBX_CMD_WT_BH
 *
 *  This flag enables the use of SMP toolbox specific WriteTask BottomHalf function
 *
 *  Dependency: BT_LE && SMP && SMP_HAVE_TBX_PL_ENC
 */
#if (defined SMP_HAVE_TBX_PL_ENC) || (defined SMP_HAVE_TBX_PL_ECDH)
#define SMP_HAVE_TBX_CMD_WT_BH
#endif /* SMP_HAVE_TBX_PL_ENC || SMP_HAVE_TBX_PL_ECDH */

#ifdef SMP_LESC
/*
 *  SMP_HAVE_TBX_PL_ECDH
 *
 *  This flag enables the use of platform specific
 *  Elliptic Curve Diffe-Hellman Functions.
 *
 *  Dependency:
 */
/* #define SMP_HAVE_TBX_PL_ECDH */

/*
 *  SMP_TBX_TEST_LESC_FUNCTIONS
 *
 *  This flag enables to test the SMP LESC tool box functions
 *
 *  Dependency: SMP_LESC
 */
/* #define SMP_TBX_TEST_LESC_FUNCTIONS */

#if (defined CLASSIC_SEC_MANAGER && defined SMP && defined SMP_LESC)
/*
 *  SMP_LESC_CROSS_TXP_KEY_GEN
 *
 *  This flag enables Cross Transport Key Derivation SMP LESC PL functions
 *
 *  Dependency: CLASSIC_SEC_MANAGER, SMP, SMP_LESC
 */
#define SMP_LESC_CROSS_TXP_KEY_GEN
#endif /* (defined CLASSIC_SEC_MANAGER && defined SMP && defined SMP_LESC) */

/*
 *  SMP_HAVE_REMOTE_PUBLIC_KEY_VALIDATION
 *
 *  This flag enables Remote Public Key Reflection Validation,
 *  introduced as part of SIG Spec Errata 10734.
 *
 *  Dependency: SMP_LESC
 */
#define SMP_HAVE_REMOTE_PUBLIC_KEY_VALIDATION

/*
 *  SMP_HAVE_RPK_X_COORD_VALIDATION
 *
 *  This flag enables Remote Public Key Validation only on the
 *  X Coordinate of the Public Key.
 *
 *  Dependency: SMP_LESC
 */
#define SMP_HAVE_RPK_X_COORD_VALIDATION


/*
 *  SMP_HAVE_REMOTE_CONFIRMVAL_VALIDATION
 *
 *  This flag enables ConfirmValue reflection attack
 *  validation.
 *
 *  Dependency: SMP_LESC
 */
#define SMP_HAVE_REMOTE_CONFIRMVAL_VALIDATION
#endif /* SMP_LESC */

/*
 *  AES_CMAC_UTIL
 *
 *  This flag enables the AES-CMAC utility module
 *
 *  Dependency: BT_LE && SMP && (SMP_DATA_SIGNING || SMP_LESC)
 */
#if (defined SMP_DATA_SIGNING) || (defined SMP_LESC)
#define AES_CMAC_UTIL
#endif /* SMP_DATA_SIGNING || SMP_LESC */

/*
 *  SMP_AUTH_REQ_RFU_TESTING
 *
 *  This flag enables testing SMP with RFU bits of Auth Req parameters set.
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_AUTH_REQ_RFU_TESTING */

/*
 *  SMP_LESC_TESTER_BEHAVIOUR
 *
 *  This flag enables Lower Tester Flexibility in SMP module.
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_LESC_TESTER_BEHAVIOUR */

/*
 *  HOST_RESOLVE_PVT_ADDR
 *
 *  This flag should be enabled if support for Private Address Resolution
 *  of a peer connected device is required in the host stack
 *
 *  Dependency: BT_LE && SMP
 */
#define HOST_RESOLVE_PVT_ADDR

/*
 *  HOST_PURGE_ON_DB_FULL
 *
 *  This flag should be enabled if the stack requires to purge
 *  the internal database when it becomes full for connections
 *
 *  Dependency: BT_LE && SMP
 */
/* #define HOST_PURGE_ON_DB_FULL */

/*
 *  SMP_HANDLE_PEER_KEY_XCHG_SEQUENCE_VIOLATION
 *
 *  This flag enables the SMP module to send pairing failure when peer
 *  device violates the Key Exchange sequence defined in the BT SMP
 *  specification.
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_HANDLE_PEER_KEY_XCHG_SEQUENCE_VIOLATION */

/*
 *  SMP_ENABLE_BLURTOOTH_VU_UPDATE
 *
 *  This flag enables the validation update to handle the
 *  Blurtooth Vulnerability with respect to Cross Transport
 *  Key Distribution
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_ENABLE_BLURTOOTH_VU_UPDATE

/*
 *  SMP_SAVE_REMOTE_IOCAP
 *
 *  This flag enables the stack to save the remote IO capabilities
 *  and also enables the application interfaces to access the same
 *
 *  Dependency: BT_LE && SMP
 */
#define SMP_SAVE_REMOTE_IOCAP

/*
 *  SMP_DISABLE_AUTHRSP_CALLBACK
 *
 *  This flag disables the SMP Authentication Response callback
 *  to the application when in Master role.
 *
 *  Dependency: BT_LE && SMP
 */
/* #define SMP_DISABLE_AUTHRSP_CALLBACK */
#endif /* SMP */


/* ----------------------------------------------------------------------- */
/* ==== ATT Protocol Related Flags ======================================= */
/* ----------------------------------------------------------------------- */
#ifdef ATT
/*
 *  ATT_CLIENT
 *  This flag is defined to enable support for ATT Client Specific
 *  Functionality, which by specification is optional to support.
 *
 *  Dependency: ATT
 *
 */
#define ATT_CLIENT

/*
 *  ATT_ON_BR_EDR_SUPPORT
 *  This flag is defined to enable support for ATT over BR/EDR
 *  transport.
 *
 *  Dependency: ATT
 *
 */
#ifndef BT_SINGLE_MODE
#define ATT_ON_BR_EDR_SUPPORT
#endif /* BT_SINGLE_MODE */

/*
 *  ATT_ON_ECBFC_SUPPORT
 *  This flag is defined to enable support for ATT over L2CAP
 *  Enhanced Credit Based Flow Control mode connection.
 *
 *  Dependency: ATT, BT_5_2 and BT_EATT
 *
 */
#ifdef BT_EATT
#define ATT_ON_ECBFC_SUPPORT
#endif /* BT_EATT */

/*
 *  ATT_NO_PARAM_CHECK
 *
 *  This flag disables the check in ATT APIs to verify the parameter memory
 *  and if the parameter values are within specification defined valid range
 *
 *  When application using ATT APIs are validated and it is ensured that none
 *  of the API call path is using parameters with invalid value, this flag can
 *  be enabled to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define ATT_NO_PARAM_CHECK */

/**
 *  ATT_DEFAULT_CONFIGURATION
 *
 *  Used to define the default configuration of ATT for the application.
 *
 *  Dependency: None.
 */
#define ATT_DEFAULT_CONFIGURATION

/**
 *  ATT_MTU_EXCHANGE_SUPPORT
 *
 *  Enables Support for MTU Exchange Procedure.
 *
 *  Dependency: None.
 */
#define ATT_MTU_EXCHANGE_SUPPORT

/**
 *  ATT_MTU_UPDATE
 *
 *  Enables support to update MTU other than the default MTU size.
 *  Please note that this feature flag is not the same as
 *  ATT_MTU_EXCHANGE_SUPPORT. For servers processing a peer request to update
 *  MTU is mandatory, however, it may respond with default MTU size and would
 *  never have to update the MTU. Hence this flag is only needed for
 *  applications that need an update of the MTU.
 *  Also note that, ATT_MTU_EXCHANGE_SUPPORT should be enabled as well to
 *  ensure exchange of MTU and hence the update can occur.
 *
 *  Dependency:  ATT_MTU_EXCHANGE_SUPPORT
 */
#define ATT_MTU_UPDATE

/**
 *  ATT_FIND_INFO_SUPPORT
 *
 *  Enables Support for Find Information Procedure.
 *
 *  Dependency: None.
 */
#define ATT_FIND_INFO_SUPPORT

/**
 *  ATT_FIND_BY_TYPE_VALUE_SUPPORT
 *
 *  Enables Support for Find by Type Procedure.
 *
 *  Dependency: None.
 */
#define ATT_FIND_BY_TYPE_VALUE_SUPPORT

/**
 *  ATT_READ_BY_TYPE_SUPPORT
 *
 *  Enables Support for Read by Type Procedure.
 *
 *  Dependency: None.
 */
#define ATT_READ_BY_TYPE_SUPPORT

/**
 *  ATT_READ_REQUEST_SUPPORT
 *
 *  Enables Support for Read Procedure.
 *
 *  Dependency: None.
 */
#define ATT_READ_REQUEST_SUPPORT

/**
 *  ATT_READ_BLOB_SUPPORT
 *
 *  Enables Support for Read Blob Procedure.
 *
 *  Dependency: None.
 */
#define ATT_READ_BLOB_SUPPORT

/**
 *  ATT_READ_MULTIPLE_SUPPORT
 *
 *  Enables Support for Read Multiple Procedure.
 *
 *  Dependency: None.
 */
#define ATT_READ_MULTIPLE_SUPPORT

/**
 *  ATT_READ_BY_GROUP_TYPE_SUPPORT
 *
 *  Enables Support for Read by Group Type Procedure.
 *
 *  Dependency: None.
 */
#define ATT_READ_BY_GROUP_TYPE_SUPPORT

/**
 *  ATT_WRITE_REQUEST_SUPPORT
 *
 *  Enables Support for Write Request Procedure.
 *
 *  Dependency: None.
 */
#define ATT_WRITE_REQUEST_SUPPORT

/**
 *  ATT_WRITE_COMMAND_SUPPORT
 *
 *  Enables Support Write Command Procedure.
 *
 *  Dependency: None.
 */
#define ATT_WRITE_COMMAND_SUPPORT

/**
 *  ATT_TX_COMPLETE_EVENT_SUPPORT
 *
 *  Enables support for indicating completion of following procedures,
 *  which are not acknowledged by the peer ATT
 *  - Write Command
 *  - Signed Write Command
 *  - Handle Value Notification
 *
 *  Transmission Complete event is generated once the corresponding
 *  ATT PDU is sent to the peer device (to the Bluetooth Controller).
 *
 *  Dependency: ATT_WRITE_COMMAND_SUPPORT, ATT_SIGNED_WRITE_SUPPORT,
 *              ATT_HNDL_VAL_NOTIFICATION_SUPPORT
 */
#define ATT_TX_COMPLETE_EVENT_SUPPORT

/**
 *  ATT_SIGNED_WRITE_SUPPORT
 *
 *  Enables Support for Signed Write Procedure.
 *
 *  Dependency: None.
 */
#define ATT_SIGNED_WRITE_SUPPORT

/**
 *  ATT_QUEUED_WRITE_SUPPORT
 *
 *  Enables Support for Confirmed Write Procedure = Prepare Write and Execute.
 *
 *  Dependency: None.
 */
#define ATT_QUEUED_WRITE_SUPPORT

#ifdef BT_EATT
/**
 *  ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
 *
 *  Enables Support for Read Multiple Variable Length Procedure.
 *
 *  Dependency: BT_EATT
 */
#define ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
#endif /* BT_EATT */

/**
 *  ATT_HNDL_VAL_NOTIFICATION_SUPPORT
 *
 *  Enables Support for Handle Value Notification Procedure.
 *
 *  Dependency: None.
 */
#define ATT_HNDL_VAL_NOTIFICATION_SUPPORT

/**
 *  ATT_HNDL_VAL_INDICATION_SUPPORT
 *
 *  Enables Support for Handle Value Indication Procedure.
 *
 *  Dependency: None.
 */
#define ATT_HNDL_VAL_INDICATION_SUPPORT

#ifdef BT_EATT
/**
 *  ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
 *
 *  Enables Support for Handle Value Multiple Notifcation Procedure.
 *
 *  Dependency: BT_EATT
 */
#define ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
#endif /* BT_EATT */

/**
 *  ATT_SUPPORT_128_BIT_UUID
 *
 *  Enables Support for 128-bit UUIDs. This feature flags only when ATT as a
 *  Server has UUIDs that are/cannot be maintained as 16-bit ones or if as a
 *  Client Requests will use 128-bit values instead of 16-bit ones. This feature
 *  flag should not be enabled if Server only wishes to support accepting
 *  requests from Client with 128-bit UUID values.
 *
 *  Dependency: None.
 */
#define ATT_SUPPORT_128_BIT_UUID

/**
 *  ATT_NOTIFY_UNEXPECTED_PDU
 *
 *  Enables application notification of remote PDU received in unexpected
 *  ATT state.
 *
 *  Dependency: None.
 */
#define ATT_NOTIFY_UNEXPECTED_PDU

/* ----------------------------------------------------------------------- */
/* ==== GATT Protocol Related Flags ====================================== */
/* ----------------------------------------------------------------------- */
/**
 *  GATT_DB_NO_NULL_PARAM_CHECK
 *
 *  Disables Null parameter check for GATT DB APIs.
 *
 *  Use of this feature flag is recommended for final release/production
 *  versions of applications for optimized code size.
 *
 *  Dependency: None.
 */
/* #define GATT_DB_NO_NULL_PARAM_CHECK */

/**
 *  GATT_DB_HAVE_STATIC_CONFIGURATION
 *
 *  Enables the GATT Database to be static, with fixed number of services,
 *  characteristics etc.
 *  If this flag is not defined, the GATT Database can be defined and extended
 *  without requiring to rebuild the core GATT Database Engine.
 *
 *  For the host stack library build, this flag should not be defined.
 *
 *  Dependency: None.
 */
/* #define GATT_DB_HAVE_STATIC_CONFIGURATION */

/*
 *  GATT_DB_HAVE_REGISTERATION_SUPPORT
 *
 *  This flag shall be defined to enable registration of GATT database
 *  with GATT DB module.
 *
 *  Dependency: None.
 */
#define GATT_DB_HAVE_REGISTERATION_SUPPORT

/*
 *  GATT_DB_HAVE_DB_SIGNATURE
 *
 *  This flag enables the support for Registered GATT Databases to have a
 *  uniquely distinguishable Database Signatures \ref GATT_DB_SIGNATURE.
 *
 *  Dependency: GATT_DB_HAVE_REGISTERATION_SUPPORT.
 */
#define GATT_DB_HAVE_DB_SIGNATURE
#if ((defined GATT_DB_HAVE_DB_SIGNATURE) && !(defined GATT_DB_HAVE_REGISTERATION_SUPPORT))
#error "Please Enable GATT_DB_HAVE_REGISTERATION_SUPPORT if you need GATT_DB_HAVE_DB_SIGNATURE"
#endif /* GATT_DB_HAVE_DB_SIGNATURE && ! GATT_DB_HAVE_REGISTERATION_SUPPORT */

/*
 *  GATT_DB_DYNAMIC
 *
 *  This flag shall be defined to enable dynamic creation of
 *  GATT Databases using exposed APIs.
 *
 *  Dependency: GATT_DB_HAVE_REGISTERATION_SUPPORT.
 */
#define GATT_DB_DYNAMIC

#if ((defined GATT_DB_DYNAMIC) && !(defined GATT_DB_HAVE_REGISTERATION_SUPPORT))
#error "Please Enable GATT_DB_HAVE_REGISTERATION_SUPPORT if you need GATT_DB_DYNAMIC"
#endif /* GATT_DB_DYNAMIC && ! GATT_DB_HAVE_REGISTERATION_SUPPORT */

/*
 *  BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
 *
 *  This flag enables dynamic allocation of global data structures.
 *
 *  If this flag is defined, the dynamic configuration of the data structures
 *  (like number of services supported etc.) shall be configured using
 *  GATT_DB_DYNAMIC_CONFIG data structure and BT_gatt_db_init() API.
 *
 *  Dependency: GATT_DB_DYNAMIC.
 */
#ifdef GATT_DB_DYNAMIC
#define BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
#endif /* GATT_DB_DYNAMIC */

/**
 *  GATT_DB_SUPPORT_128_BIT_UUID
 *
 *  Enables support for 128 bit UUIDs in GATT Data Base.
 *
 *  Dependency: ATT_SUPPORT_128_BIT_UUID
 */
#define GATT_DB_SUPPORT_128_BIT_UUID

#if ((defined GATT_DB_SUPPORT_128_BIT_UUID) && !(defined ATT_SUPPORT_128_BIT_UUID))
#error "Please Enable ATT_SUPPORT_128_BIT_UUID if you need GATT_DB_SUPPORT_128_BIT_UUID"
#endif /* GATT_DB_SUPPORT_128_BIT_UUID && ! ATT_SUPPORT_128_BIT_UUID */

/**
 *  GATT_DB_CHAR_DESC_DATA
 *
 *  Enables support for Profile/Higher Layer defined Descriptors.
 *
 *  Dependency: None.
 */
#define GATT_DB_CHAR_DESC_DATA

/**
 *  GATT_DB_HASH_SUPPORT
 *
 *  Enables support for GATT DB Hash generation.
 *
 *  Dependency: None.
 */
#define GATT_DB_HASH_SUPPORT

/**
 *  GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT
 *
 *  Enables support for "offset" parameter in the \ref GATT_DB_PARAMS
 *  structure.
 *
 *  The "offset" is required to inform the upper layer about the Offset
 *  received in the Prepare-Write/Execute-Write, Read-Long/Read-Blob etc.
 *
 *  Dependency: None.
 */
#define GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT

/**
 *  GATT_DB_VALIDATE_TRANSPORT_ACCESS
 *
 *  Enables transport validation support for GATT Service access .
 *
 *  Dependency: BT_DUAL_MODE.
 */
#ifdef BT_DUAL_MODE
#define GATT_DB_VALIDATE_TRANSPORT_ACCESS
#endif /* BT_DUAL_MODE */
#endif /* ATT */

/* ----------------------------------------------------------------------- */
/* ==== JPL Related Flags ====================================== */
/* ----------------------------------------------------------------------- */
/**
 *  JPL_USE_APP_MEMORY
 *
 *  Enables application to register memory for JPL buffers.
 *
 *  Dependency: None.
 */
#define JPL_USE_APP_MEMORY

/* ----------------------------------------------------------------------- */
/* ==== Module Profiling Flags =========================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Profiling related definitions for all the layers (protocol and profiles)
 *  shall be defined in this section.
 */

/*
 *  BT_ENABLE_SPY
 *
 *  If defined, this flag enables module profiling.
 *
 *  Dependency: None
 */
/* #define BT_ENABLE_SPY */

#ifdef BT_ENABLE_SPY
#define HT_ENABLE_SPY
#define WT_ENABLE_SPY
#define L2CAP_ENABLE_SPY
#endif /* BT_ENABLE_SPY */


/* ----------------------------------------------------------------------- */
/* ==== Debug Specification Flags ======================================== */
/* ----------------------------------------------------------------------- */
/*
 *  Debug definitions for all the layers (protocol and profiles) should be
 *  defined in this section.
 */

/*
 *  Global Debug Disable Flag : BT_DISABLE_ALL_DEBUG
 *  If this flag is defined, debug logging will be disabled.
 *  If this flag is not defined, then the debug logging will be controlled
 *  by other debug flags as described below.
 */
#define BT_DISABLE_ALL_DEBUG

/*
 *  Flag to control debug logging at run time.
 *  Debug level can be set using BT_set_debug_level() API.
 *  In addition there are interfaces to enable/disable module specific flag at run time.
 */
#define BT_ENABLE_DISABLE_RUNTIME_DEBUG

/*
 * BT_DEBUG_DONT_LOG_FILE_PATH
 *
 * By default, debug log prints the file name with the path.
 * This flag enables logging only the file name, without the path.
 */
#define BT_DEBUG_DONT_LOG_FILE_PATH

/*
 *  BT_LOG_TIMESTAMP
 *
 *  If defined, this flag enables logging timestamp along
 *  with the debug prints.
 *
 *  Dependency: None
 */
#define BT_LOG_TIMESTAMP

/*
 * By Default SPP_MAX_ENTITY will be 2. 1 SPP instance is created having SERIALPORT_UUID with default server_channel number 2.
 * and 1 SPP_VS instance having CustomUUID_0 or default SERIALPORT_UUID on server_channel number 3.
 *
 * With SPP_ENABLE_MAX_CONN_RANGE enabled 20 SPP instances are created having SERIALPORT_UUID with 20 different server_channels
 * and 1 SPP_VS instance created having CustomUUID_0 or with default SERIALPORT_UUID on another server_channel.
 */
#define SPP_ENABLE_MAX_CONN_RANGE

/*
 * By Default SPP_MAX_ENTITY will be 2. 1 SPP instance is created having SERIALPORT_UUID with default server_channel number 2.
 * and 1 SPP_VS instance having CustomUUID_0 or default SERIALPORT_UUID on server_channel number 3.
 *
 * With SPP_ENABLE_MAX_CONN_RANGE enabled 20 SPP instances are created having SERIALPORT_UUID with 20 different server_channels
 * and 1 SPP_VS instance created having CustomUUID_0 or with default SERIALPORT_UUID on another server_channel.
 */
#define SPP_ENABLE_MAX_CONN_RANGE

/*
 * By default, the Error Logs of all the layers are enabled.
 * To disable error logging of a module, define <module>_NO_DEBUG flag.
 * Example: Define HCI_NO_DEBUG to disable error logging of HCI layer.
 *
 * By default, the Trace, Information, Data and other Logs
 * of all the layers are disabled.
 * To enable debug logging of a module, define <module>_DEBUG flag.
 * Example: Define HCI_DEBUG to enable debug logging (Trace and Information)
 * of HCI layer.
 */

/* Protocol Modules */
#define COMMON_NO_DEBUG
/* #define COMMON_DEBUG */

#define  AVCTP_NO_DEBUG
/* #define  AVCTP_DEBUG */

#define  AVDTP_NO_DEBUG
/* #define  AVDTP_DEBUG */
/* #define  AVDTP_DATA_DEBUG */

/* #define  BCSP_NO_DEBUG */
/* #define  BCSP_DEBUG */
/* #define  BCSP_LINK_DEBUG */
/* #define  BCSP_DATAGRAM_DEBUG */
/* #define  BCSP_SEQUENCE_DEBUG */
/* #define  BCSP_MUX_DEBUG */
/* #define  BCSP_INTEGRITY_DEBUG */
/* #define  BCSP_SLIP_DEBUG */

/* #define  BNEP_NO_DEBUG */
/* #define  BNEP_DEBUG */
/* #define  BNEP_DATA_DEBUG */

#define  DB_NO_DEBUG
/* #define  DB_DEBUG */

#define  HCI_NO_DEBUG
/* #define  HCI_DEBUG */
/* #define  HCI_DDEBUG */

#define  HT_NO_DEBUG
/* #define  HT_DEBUG */
/* #define  HT_INTR_DEBUG */

#define DQ_NO_DEBUG
/* #define DQ_DEBUG */

/* #define  MCAP_NO_DEBUG */
/* #define  MCAP_DEBUG */

#define  OBEX_NO_DEBUG
/* #define  OBEX_DEBUG */

#define  L2CAP_NO_DEBUG
/* #define  L2CAP_DEBUG */
/* #define  L2CAP_LP_DEBUG */

#define  RFCOMM_NO_DEBUG
/* #define  RFCOMM_DEBUG */

#define  SDP_NO_DEBUG
/* #define  SDP_DEBUG */

#define  SEC_NO_DEBUG
/* #define  SEC_DEBUG */

#define  WT_NO_DEBUG
/* #define  WT_DEBUG */

#define ATT_NO_DEBUG
/* #define ATT_DEBUG */

#define SMP_NO_DEBUG
/* #define SMP_DEBUG */

#ifdef SMP_DEBUG
#define SMP_DATA_DEBUG
#endif /* SMP_DEBUG */

#define GATT_DB_NO_DEBUG
/* #define GATT_DB_DEBUG */

#define FSM_NO_DEBUG
/* #define FSM_DEBUG */

/* Profile Modules */
#define  A2DP_NO_DEBUG
/* #define  A2DP_DEBUG */
/* #define  A2DP_DATA_DEBUG */

#define  AVRCP_NO_DEBUG
/* #define  AVRCP_DEBUG */

#define  AVRCP_CA_NO_DEBUG
/* #define  AVRCP_CA_DEBUG */

/* #define  BIP_NO_DEBUG */
/* #define  BIP_DEBUG */

/* #define  BPP_NO_DEBUG */
/* #define  BPP_DEBUG */

/* #define  CTN_NO_DEBUG */
/* #define  CTN_DEBUG */

/* #define  DUNP_GW_NO_DEBUG */
/* #define  DUNP_GW_DEBUG */
/* #define  DUNP_GW_DATA_DEBUG */

/* #define  DUNP_DT_NO_DEBUG */
/* #define  DUNP_DT_DEBUG */
/* #define  DUNP_DT_DATA_DEBUG */

/* #define  FTP_NO_DEBUG */
/* #define  FTP_DEBUG */

/* #define  HCRP_NO_DEBUG */
/* #define  HCRP_DEBUG */

#define SPP_NO_DEBUG
/* #define SPP_DEBUG */

/* #define  HFP_AG_NO_DEBUG */
/* #define  HFP_AG_DEBUG */
/* #define  HFP_AG_DATA_DEBUG */

#define  HFP_UNIT_NO_DEBUG
/* #define  HFP_UNIT_DEBUG */

/* #define  HID_HOST_NO_DEBUG */
/* #define  HID_HOST_DEBUG */

/* #define  HID_DEV_NO_DEBUG */
/* #define  HID_DEV_DEBUG */

/* #define  HSP_AG_NO_DEBUG */
/* #define  HSP_AG_DEBUG */
/* #define  HSP_AG_DATA_DEBUG */

#define  HSP_UNIT_NO_DEBUG
/* #define  HSP_UNIT_DEBUG */

/* #define SAP_NO_DEBUG */
/* #define SAP_DEBUG */

/* #define GNSS_CLI_NO_DEBUG */
/* #define GNSS_CLI_DEBUG */

/* #define GNSS_SER_NO_DEBUG */
/* #define GNSS_SER_DEBUG */

/* #define  OPP_NO_DEBUG */
/* #define  OPP_DEBUG */

/* #define  PAN_NO_DEBUG */
/* #define  PAN_DEBUG */
/* #define  PAN_DATA_DEBUG */

/* #define  PBAP_NO_DEBUG */
/* #define  PBAP_DEBUG */

/* #define  SYNCP_NO_DEBUG */
/* #define  SYNCP_DEBUG */
/* #define  SYNCP_DATA_DEBUG */

/* #define  UDIP_NO_DEBUG */
/* #define  UDIP_DEBUG */
/* #define  UDIP_DATA_DEBUG */

/* #define  MCAP_NO_DEBUG */
/* #define  MCAP_DEBUG */

/* #define  MAP_NO_DEBUG */
/* #define  MAP_DEBUG */

/* #define JPL_NO_DEBUG */
/* #define JPL_DEBUG */

/* #define BT_FOPS_NO_DEBUG */
/* #define BT_FOPS_DEBUG */

/* #define BT_TIMER_NO_DEBUG */
/* #define BT_TIMER_DEBUG */

/* #define NIFACE_PL_NO_DEBUG */
/* #define NIFACE_PL_DEBUG */

/* #define VCOM_NO_DEBUG */
/* #define VCOM_DEBUG */

/* #define A2DP_PL_NO_DEBUG */
/* #define A2DP_PL_DEBUG */

/* #define CTN_PL_NO_DEBUG */
/* #define CTN_PL_DEBUG */

/* #define MAP_PL_NO_DEBUG */
/* #define MAP_PL_DEBUG */

/* #define SNOOP_NO_DEBUG */
/* #define SNOOP_DEBUG */

#define HCI_UART_NO_DEBUG
/* #define HCI_UART_DEBUG */

#endif /* _H_BT_FEATURES_ */

