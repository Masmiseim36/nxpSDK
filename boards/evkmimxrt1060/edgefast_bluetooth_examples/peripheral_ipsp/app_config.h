

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

#define CONFIG_BT_DEVICE_NAME "peripheral_ipsp"
#include "edgefast_bluetooth_config.h"
