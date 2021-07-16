
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

#define CONFIG_BT_A2DP      1
#define CONFIG_BT_A2DP_SINK 1

#include "edgefast_bluetooth_config.h"
