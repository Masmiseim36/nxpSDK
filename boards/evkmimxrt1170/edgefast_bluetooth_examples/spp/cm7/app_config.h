
#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define FLASH_ADAPTER_SIZE 0x4000

/* Controller config
 * Supported controller list,
 * WIFI_BOARD_AW_AM457
 * WIFI_BOARD_AW_CM358
 *
 * If aw am457 used, define marco WIFI_BOARD_AW_AM457 in following.
 * If aw cm358ma used, define marco WIFI_BOARD_AW_CM358 in following.
 */

#define WIFI_BOARD_AW_AM457
/*#define WIFI_BOARD_AW_CM358*/

#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358)
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

#define CONFIG_BT_RFCOMM 1

#include "edgefast_bluetooth_config.h"
