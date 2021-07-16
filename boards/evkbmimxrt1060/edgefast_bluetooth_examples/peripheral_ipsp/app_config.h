

#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

/* Controller config
 * Supported controller list,
 * WIFI_BOARD_AW_AM457
 * WIFI_BOARD_AW_CM358
 * K32W061_TRANSCEIVER
 *
 * If aw am457 uSD used, define marco WIFI_BOARD_AW_AM457 in following.
 * If aw cm358 uSD used, define marco WIFI_BOARD_AW_CM358 in following.
 * If transceiver k32w061 is used, the macro K32W061_TRANSCEIVER should be defined
 */

#define WIFI_BOARD_AW_AM457
/*#define WIFI_BOARD_AW_CM358*/
/*#define K32W061_TRANSCEIVER*/

#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358) || defined(K32W061_TRANSCEIVER)
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define CONFIG_BT_PERIPHERAL  1
#define CONFIG_BT_DEVICE_NAME "peripheral_ipsp"
#include "edgefast_bluetooth_config.h"
