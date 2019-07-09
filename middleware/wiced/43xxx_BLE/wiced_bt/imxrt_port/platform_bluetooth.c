#include "platform_bluetooth.h"

/* BT chip specific configuration information.
 * Change default_baud_rate only by changing 
 * macro WICED_BT_UART_DEFAULT_BAUD_RATE, as this macro is used in 
 * platform_imxrt.c also. 
 */
const platform_bluetooth_config_t wiced_bt_config =
{
    .patchram_download_mode      = PATCHRAM_DOWNLOAD_MODE_MINIDRV_CMD,
    .patchram_download_baud_rate = WICED_BT_UART_DOWNLOAD_BAUD_RATE,
    .featured_baud_rate          = WICED_BT_UART_FEATURED_BAUD_RATE,
    .default_baud_rate           = WICED_BT_UART_DEFAULT_BAUD_RATE 
};

#if 0
/* Bluetooth UART configuration. Used by libraries/bluetooth/internal/bus/UART/bt_bus.c */
const platform_uart_config_t wiced_bt_uart_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_CTS_RTS,
};
#endif


