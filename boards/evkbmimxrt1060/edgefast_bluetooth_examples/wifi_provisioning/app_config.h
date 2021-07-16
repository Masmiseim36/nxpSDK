

#define USB_HOST_CONFIG_EHCI 2
#define LWIP_PROVIDE_ERRNO   1

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
/*#define K32W061_TRANSCEIVER*/

#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358)
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif
