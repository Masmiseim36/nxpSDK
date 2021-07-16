
#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

/* Controller config
 * Supported controller list,
 * WIFI_BOARD_AW_AM457
 * WIFI_BOARD_AW_CM358
 *
 * If aw am457 uSD used, define marco WIFI_BOARD_AW_AM457 in following.
 * If aw cm358 uSD used, define marco WIFI_BOARD_AW_CM358 in following.
 */

#define WIFI_BOARD_AW_AM457
/*#define WIFI_BOARD_AW_CM358*/

#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358)
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

#if defined(WIFI_BOARD_AW_CM358)
#if defined(SDMMCHOST_OPERATION_VOLTAGE_3V3)
#elif defined(SDMMCHOST_OPERATION_VOLTAGE_1V8)
#undef SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#endif
#endif

#define CONFIG_BT_RFCOMM         1
#define CONFIG_BT_HFP_HF         1
#define CONFIG_BT_DEBUG          0
#define CONFIG_BT_DEBUG_HCI_CORE 0
#define CONFIG_BT_DEBUG_CONN     0
#define CONFIG_BT_DEBUG_GATT     0
#define CONFIG_BT_DEBUG_ATT      0
#define CONFIG_BT_DEBUG_SMP      0
#define CONFIG_BT_DEBUG_KEYS     0
#define CONFIG_BT_DEBUG_L2CAP    0
#define CONFIG_BT_DEBUG_SERVICE  0
#define CONFIG_BT_DEBUG_HFP_HF   0

#include "edgefast_bluetooth_config.h"
