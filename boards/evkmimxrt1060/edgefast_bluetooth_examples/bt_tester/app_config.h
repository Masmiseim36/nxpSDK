/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI (2U)
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define CONFIG_BT_PERIPHERAL  1
#define CONFIG_BT_CENTRAL     1
#define CONFIG_BT_BROADCASTER 1
#define CONFIG_BT_OBSERVER    1
#define CONFIG_BT_CONN        1
#define CONFIG_BT_MAX_CONN    1
/* #define CONFIG_BT_CONN_TX 1 EdgeFast Bluetooth doesn't support it yet*/
#define CONFIG_BT_PHY_UPDATE      1
#define CONFIG_BT_DATA_LEN_UPDATE 1

/*
 * Bluetooth buffer configuration
 */
#define CONFIG_BT_BUF_ACL_TX_SIZE           27
#define CONFIG_BT_BUF_ACL_TX_COUNT          3
#define CONFIG_BT_BUF_ACL_RX_SIZE           73
#define CONFIG_BT_BUF_ACL_RX_COUNT          3
#define CONFIG_BT_BUF_EVT_RX_SIZE           68
#define CONFIG_BT_BUF_EVT_RX_COUNT          3
#define CONFIG_BT_BUF_EVT_DISCARDABLE_SIZE  43
#define CONFIG_BT_BUF_EVT_DISCARDABLE_COUNT 20
#define CONFIG_BT_BUF_CMD_TX_SIZE           65
#define CONFIG_BT_BUF_CMD_TX_COUNT          2

#define CONFIG_BT_RPA                 1
#define CONFIG_BT_ASSERT              1
#define CONFIG_BT_ASSERT_VERBOSE      1
#define CONFIG_BT_DEBUG               1
#define CONFIG_BT_DEBUG_LOG           1
#define CONFIG_BT_HCI_HOST            1
#define CONFIG_BT_HCI_TX_STACK_SIZE   1024
#define CONFIG_BT_HCI_ECC_STACK_SIZE  1140
#define CONFIG_BT_HCI_TX_PRIO         7
#define CONFIG_BT_HCI_RESERVE         0
#define CONFIG_BT_RECV_BLOCKING       1
#define CONFIG_BT_RX_STACK_SIZE       2048
#define CONFIG_BT_RX_PRIO             8
#define CONFIG_BT_DRIVER_RX_HIGH_PRIO 6

#define CONFIG_BT_FILTER_ACCEPT_LIST         1
#define CONFIG_BT_LIM_ADV_TIMEOUT            30
#define CONFIG_BT_CONN_TX_MAX                3
#define CONFIG_BT_AUTO_PHY_UPDATE            1
#define CONFIG_BT_AUTO_DATA_LEN_UPDATE       1
#define CONFIG_BT_SMP                        1
#define CONFIG_BT_SIGNING                    1
#define CONFIG_BT_SMP_APP_PAIRING_ACCEPT     1
#define CONFIG_BT_SMP_ALLOW_UNAUTH_OVERWRITE 1
#define CONFIG_BT_BONDABLE                   1

/*
 * L2CAP Options
 */
#define CONFIG_BT_L2CAP_TX_BUF_COUNT    3
#define CONFIG_BT_L2CAP_TX_FRAG_COUNT   2
#define CONFIG_BT_L2CAP_TX_MTU          69
#define CONFIG_BT_L2CAP_DYNAMIC_CHANNEL 1
#define CONFIG_BT_L2CAP_ECRED           1

/*
 * ATT and GATT Options
 */
#define CONFIG_BT_ATT_ENFORCE_FLOW  1
#define CONFIG_BT_ATT_PREPARE_COUNT 8
/* #define CONFIG_BT_EATT 1 */
#define CONFIG_BT_EATT_MAX                       5
#define CONFIG_BT_EATT_SEC_LEVEL                 1
#define CONFIG_BT_GATT_AUTO_SEC_REQ              1
#define CONFIG_BT_GATT_SERVICE_CHANGED           1
#define CONFIG_BT_GATT_DYNAMIC_DB                1
#define CONFIG_BT_GATT_CACHING                   1
#define CONFIG_BT_GATT_CLIENT                    1
#define CONFIG_BT_GATT_READ_MULTIPLE             1
#define CONFIG_BT_GAP_AUTO_UPDATE_CONN_PARAMS    1
#define CONFIG_BT_GAP_PERIPHERAL_PREF_PARAMS     1
#define CONFIG_BT_PERIPHERAL_PREF_MIN_INT        24
#define CONFIG_BT_PERIPHERAL_PREF_MAX_INT        40
#define CONFIG_BT_PERIPHERAL_PREF_LATENCY        0
#define CONFIG_BT_PERIPHERAL_PREF_TIMEOUT        42
#define CONFIG_BT_DEVICE_NAME_GATT_WRITABLE      1
#define CONFIG_DEVICE_NAME_GATT_WRITABLE_ENCRYPT 1

#define CONFIG_BT_MAX_PAIRED                1
#define CONFIG_BT_CREATE_CONN_TIMEOUT       3
#define CONFIG_BT_CONN_PARAM_UPDATE_TIMEOUT 5000
#define CONFIG_BT_BACKGROUND_SCAN_INTERVAL  2048
#define CONFIG_BT_BACKGROUND_SCAN_WINDOW    18
#define CONFIG_BT_DEVICE_NAME_DYNAMIC       1
#define CONFIG_BT_DEVICE_NAME_MAX           32
#define CONFIG_BT_DEVICE_NAME               "Tester"
#define CONFIG_BT_DEVICE_APPEARANCE         0
#define CONFIG_BT_ID_MAX                    1
#define CONFIG_BT_ECC                       1
/* #define CONFIG_BT_TINYCRYPT_ECC 1 */
#define CONFIG_BT_HOST_CCM 1
#define CONFIG_BT_TESTING  1

/*
#define CONFIG_BT_PRIVACY                    1
#define CONFIG_BT_HOST_CRYPTO                1
#define CONFIG_BT_DEVICE_NAME                "BT_TESTER"
*/

#define DEBUG_CONSOLE_RX_ENABLE   0
#define BOARD_DEBUG_UART_BAUDRATE (9600U)
#define SDK_DEBUGCONSOLE          DEBUGCONSOLE_DISABLE

#define OSA_USED  1
#define BT_TESTER 1

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM510_USD
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_88W8987_BOARD_AW_CM358_USD
 * WIFI_IW416_BOARD_MURATA_1XK_USD
 * WIFI_88W8987_BOARD_MURATA_1ZM_USD
 *
 * If aw am510 uSD used, define marco WIFI_IW416_BOARD_AW_AM510_USD in following.
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw cm358 uSD used, define marco WIFI_88W8987_BOARD_AW_CM358_USD in following.
 * If Murata Type 1XK module used, define macro WIFI_IW416_BOARD_MURATA_1XK_USD in following.
 * If Murata Type 1ZM module used, define macro WIFI_88W8987_BOARD_MURATA_1ZM_USD in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_AW_AM510_USD
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_AW_AM457_USD*/
/*#define WIFI_88W8987_BOARD_AW_CM358_USD*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_USD*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_USD*/

#if (defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM457_USD) ||     \
     defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || \
     defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD))
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#include "edgefast_bluetooth_config.h"
