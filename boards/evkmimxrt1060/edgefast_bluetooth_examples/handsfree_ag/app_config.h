
#define USB_HOST_CONFIG_EHCI 2

/* Controller config
 * Supported controller list,
 * WIFI_BOARD_AW_AM457
 *
 * If aw am457 used, define marco WIFI_BOARD_AW_AM457 in following.
 */

#define WIFI_BOARD_AW_AM457

#if defined(WIFI_BOARD_AW_AM457)
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

#define CONFIG_BT_DEBUG          0
#define CONFIG_BT_DEBUG_HCI_CORE 0
#define CONFIG_BT_DEBUG_CONN     0
#define CONFIG_BT_DEBUG_GATT     0
#define CONFIG_BT_DEBUG_ATT      0
#define CONFIG_BT_DEBUG_SMP      0
#define CONFIG_BT_DEBUG_KEYS     0
#define CONFIG_BT_DEBUG_L2CAP    0
#define CONFIG_BT_DEBUG_SERVICE  0
#define CONFIG_BT_DEBUG_HFP_AG   0

#include "edgefast_bluetooth_config.h"
