/*
 *  Copyright 2022 - 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_audio_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* 
 * macro configuring flash adapter size 
 */
#define FLASH_ADAPTER_SIZE 0x10000

/* 
 * macro for USB debug port
 */
#define USE_USB_DEBUG_PORT   0U
#define USB_HOST_CONFIG_EHCI (2U)
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define BOARD_DEBUG_USBCDC_INSTANCE kSerialManager_UsbControllerEhci1
#define SERIAL_PORT_USB_CDC_USB0_IRQ_HANDLER_DISABLE
#define SERIAL_PORT_USB_CDC_USBHS_IRQ_HANDLER_DISABLE

/*
 * Macro CONFIG_BT_PRIVACY should be enabled for below cases:
 * GAP/CONN/DCON/BV-04-C
 */
/* #define CONFIG_BT_PRIVACY                 0 */
/* #define CONFIG_BT_SMP_ENFORCE_MITM        0 */
/* #define CONFIG_BT_EXT_ADV                 1  */
/* #define CONFIG_BT_TINYCRYPT_ECC           1 */
#define CONFIG_BT_TESTING 1

/* 
 * Macros for BR/EDR
 */
#if (defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U))
/* redefine CONFIG_BT_PAGE_TIMEOUT */
#undef CONFIG_BT_PAGE_TIMEOUT
#define CONFIG_BT_PAGE_TIMEOUT 0xFFFF

/* 
 * Macros for A2DP
 */
#if (defined(CONFIG_BT_A2DP) && ((CONFIG_BT_A2DP) > 0U))
/* In a2dp auto-pts test, only 1 of CONFIG_BT_A2DP_SINK and CONFIG_BT_A2DP_SOURCE can be enabled. 
 * CONFIG_BT_A2DP_SINK enables a2dp sink role and it's the defaulted role. 
 * CONFIG_BT_A2DP_SOURCE enables a2dp source role. */
#undef CONFIG_BT_A2DP_SINK
#define CONFIG_BT_A2DP_SINK             1
#undef CONFIG_BT_A2DP_SOURCE
#define CONFIG_BT_A2DP_SOURCE           0

/* macro to enable a2dp delay report */
#undef CONFIG_BT_A2DP_DR_SERVICE
#define CONFIG_BT_A2DP_DR_SERVICE       0
#endif /* CONFIG_BT_A2DP > 0U */

/*
 * Macros for AVRCP
 */
#if (defined(CONFIG_BT_AVRCP) && ((CONFIG_BT_AVRCP) > 0U))
/* AVRCP Controller */
#define CONFIG_BT_AVRCP_CT  			     1
/* AVRCP Target */
#define CONFIG_BT_AVRCP_TG  		    	 0

#define CONFIG_BT_AVRCP_SEND_PACKET_MAX_SIZE 588

#define CONFIG_BT_AVRCP_BROWSING             1

#define CONFIG_BT_AVRCP_COVER_ART           1
#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))
#define CONFIG_BT_AVRCP_COVER_ART_RESPONDER 0
#define CONFIG_BT_AVRCP_COVER_ART_INITIATOR 1
#endif /* CONFIG_BT_AVRCP_COVER_ART > 0U */
#endif /* CONFIG_BT_AVRCP > 0U */

/*
 * Macros for BR/EDR L2CAP
 */
#if 1
/* CONFIG_BT_UCD is used by unicast connectionless 
 * When this macro is enabled, libethermind_bt_core.a should be regenerated with BT_UCD enabled. */
#define CONFIG_BT_UCD   0
#if (defined(CONFIG_BT_UCD) && (1U == CONFIG_BT_UCD))
#define BT_UCD
#endif

/* For BREDR, define defaulted CONFIG_BT_L2CAP_ECRED as 0, default L2CAP mode is basic mode */
#if (defined(CONFIG_BT_L2CAP_ECRED) && (1U == CONFIG_BT_L2CAP_ECRED))
#undef CONFIG_BT_L2CAP_ECRED
#define CONFIG_BT_L2CAP_ECRED 0
#undef CONFIG_BT_EATT
#define CONFIG_BT_EATT 0
#endif /* CONFIG_BT_L2CAP_ECRED */

/* For BREDR, set default security level as 2.
 * For L2CAP/LE/CID/BV-04-C, support BREDR and LE with the same spsm, disable this macro. */
#define CONFIG_BT_PSM_SC 1

/*
 * Enable CONFIG_BT_L2CAP_IFRAME_SUPPORT for L2CAP Enhanced Retransmission Mode and Streaming Mode
 */
#undef CONFIG_BT_L2CAP_IFRAME_SUPPORT
#define CONFIG_BT_L2CAP_IFRAME_SUPPORT 0

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && ((CONFIG_BT_L2CAP_IFRAME_SUPPORT) > 0U))
/*
 * Select L2CAP mode by macro CONFIG_BT_L2CAP_MODE_SELECT, the available values are:
 * 0: L2CAP_MODE_BASIC
 * 1: L2CAP_MODE_RTM
 * 2: L2CAP_MODE_FC
 * 3: L2CAP_MODE_ERTM
 * 4: L2CAP_MODE_SM
 */
#define CONFIG_BT_L2CAP_MODE_SELECT 3
/*
 * Force to use L2CAP Enhanced Retransmission Mode or Streming Mode if CONFIG_BT_L2CAP_MODE_OPTINAL is 0
 */
#define CONFIG_BT_L2CAP_MODE_OPTINAL 1

/*
 * No FCS option in L2CAP configuration packet
 */
#define CONFIG_BT_L2CAP_NO_FCS 0

#if 0
/*
 * Support extended flow spec 
 * Enabled this code for L2CAP/LSC/ auto-pts cases
 */
#define L2CAP_EXTENDED_FLOW_SPEC
#endif

/*
 * Force to use L2CAP extended flow spec with Guaranteed type
 */
#define CONFIG_BT_L2CAP_EXT_FLOW_SPEC 0
#endif /* CONFIG_BT_L2CAP_IFRAME_SUPPORT */
#endif /* Macros for BR/EDR L2CAP */

#endif /* CONFIG_BT_CLASSIC */

/* #define CONFIG_BT_CONN_TX 1 EdgeFast Bluetooth doesn't support it yet*/

/*
 * Bluetooth buffer configuration
 */

#define CONFIG_BT_ASSERT               1
#define CONFIG_BT_ASSERT_VERBOSE       1
#define CONFIG_BT_RECV_BLOCKING        1
#define CONFIG_BT_DRIVER_RX_HIGH_PRIO  6

/*
 * Macro CONFIG_BT_SMP_SC_ONLY should be enabled for below cases:
 * GAP/SEC/SEM/BI-22-C
 * GAP/SEC/SEM/BI-10-C
 * GAP/SEC/SEM/BV-25-C
 * GAP/SEC/SEM/BV-30-C
 */
/* #define CONFIG_BT_SMP_SC_ONLY              1 */

/*
 * ATT and GATT Options
 */
#define CONFIG_BT_ATT_ENFORCE_FLOW     1
#define CONFIG_BT_GATT_AUTO_SEC_REQ    1

#define CONFIG_BT_CONN_PARAM_RETRY_COUNT   3
#define CONFIG_BT_CONN_PARAM_RETRY_TIMEOUT 5000

/*
 * LE Audio Options
 */
#if (defined(CONFIG_BT_AUDIO) && ((CONFIG_BT_AUDIO) > 0U))
/* For le audio, redefine some macros different from non le audio */
#if (defined(CONFIG_BT_MAX_CONN) && (2U != CONFIG_BT_MAX_CONN))
#undef CONFIG_BT_MAX_CONN
#define CONFIG_BT_MAX_CONN 2
#endif

#if (defined(CONFIG_BT_L2CAP_TX_MTU) && (247U != CONFIG_BT_L2CAP_TX_MTU))
#undef CONFIG_BT_L2CAP_TX_MTU
#define CONFIG_BT_L2CAP_TX_MTU 247
#endif

#if 0
/* When initiating Codec Config by server for 3 ASEs one by one, MTU buffers
   were freed too slow. The bt_bap_stream_ops.configured callback comes earlier. */
CONFIG_BT_L2CAP_TX_BUF_COUNT=4
#endif

/* 
 * Broadcast Source
 */
#undef CONFIG_BT_ISO_TX_BUF_COUNT
#define CONFIG_BT_ISO_TX_BUF_COUNT                 4

/* 
 * IAS
 */
#define CONFIG_BT_IAS        1
#define CONFIG_BT_IAS_CLIENT 1
#endif /* CONFIG_BT_AUDIO > 0U */

#define DEBUG_CONSOLE_RX_ENABLE   0
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#define SDK_DEBUGCONSOLE          DEBUGCONSOLE_DISABLE

#define OSA_USED  1
#define BT_TESTER 1

#define CONFIG_BT_HOST_USB_2_IRQ_DISABLE

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_88W8987_BOARD_MURATA_1ZM_M2
 * WIFI_IW612_BOARD_RD_USD
 * WIFI_IW416_BOARD_AW_AM510_USD
 * WIFI_IW612_BOARD_MURATA_2EL_M2
 *
 * If Murata Type 1XK module(EAR00385 M2 only) used, define macro WIFI_IW416_BOARD_MURATA_1XK_M2 in following.
 * If Murata Type 1ZM module(EAR00364 M2 only) used , define macro WIFI_88W8987_BOARD_MURATA_1ZM_M2 in following.
 * If IW612 uSD module(usd only) used , define macro WIFI_IW612_BOARD_RD_USD in following.
 * If AW AM510 uSD module(usd only) used , define macro WIFI_IW416_BOARD_AW_AM510_USD in following.
 * If Murata Type 2EL module(Rev-A1 M2 only) used , define macro WIFI_IW612_BOARD_MURATA_2EL_M2 in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW612_BOARD_MURATA_2EL_M2
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_MURATA_1XK_M2*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_M2*/
/*#define WIFI_IW612_BOARD_RD_USD*/
/*#define WIFI_IW416_BOARD_AW_AM510_USD*/
/*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

#if (defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
     defined(WIFI_IW612_BOARD_RD_USD) || defined(WIFI_IW416_BOARD_AW_AM510_USD) ||           \
     defined(WIFI_IW612_BOARD_MURATA_2EL_M2))
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

/*
 * AUTO-PTS CASE CONFIG
 * If the following cases want to be pass, please enable corresponding macros.
 */
/* GAP/SEC/SEM/BV-56-C
   GAP/SEC/SEM/BV-62-C
   GAP/SEC/SEM/BV-63-C
   GAP/SEC/SEM/BV-64-C
   GAP/SEC/SEM/BV-65-C
   GAP/SEC/SEM/BV-66-C
   GAP/SEC/SEM/BV-67-C */
#define CONFIG_AUTO_PTS_DEFALUT_MIN_SEC_L2_ENABLE 0

/*
 * AUTO-PTS CASE CONFIG
 * If the following cases want to be pass, please enable corresponding macros.
 */
/*
GAP/SEC/SEM/BI-17-C
GAP/SEC/SEM/BI-18-C
GAP/SEC/SEM/BI-19-C
GAP/SEC/SEM/BI-04-C
GAP/SEC/SEM/BI-08-C
GAP/SEC/SEM/BI-31-C
*/
/*
#define CONFIG_BT_SMP_MIN_ENC_KEY_SIZE 16
*/

#if (defined(CONFIG_BT_SMP_SC_ONLY) && (CONFIG_BT_SMP_SC_ONLY > 0))
#undef CONFIG_BT_SMP_MIN_ENC_KEY_SIZE
#define CONFIG_BT_SMP_MIN_ENC_KEY_SIZE 16
#else
#undef CONFIG_BT_SMP_MIN_ENC_KEY_SIZE
#define CONFIG_BT_SMP_MIN_ENC_KEY_SIZE 7
#endif /* CONFIG_BT_SMP_SC_ONLY */

/* #define CONFIG_BT_EAD 1 */
/* #define CONFIG_BT_TESTING 1 */
