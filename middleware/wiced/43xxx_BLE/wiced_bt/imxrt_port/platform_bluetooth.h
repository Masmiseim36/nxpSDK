/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#include <stdint.h>

#define  WICED_BT_UART_DEFAULT_BAUD_RATE   115200
#define  WICED_BT_UART_DOWNLOAD_BAUD_RATE  115200
#define  WICED_BT_UART_FEATURED_BAUD_RATE  3000000 

typedef enum
{   
    WICED_BT_PIN_POWER,
    WICED_BT_PIN_RESET,
    WICED_BT_PIN_HOST_WAKE,
    WICED_BT_PIN_DEVICE_WAKE,
} wiced_bt_control_pin_t;

typedef enum
{
    PATCHRAM_DOWNLOAD_MODE_NO_MINIDRV_CMD,
    PATCHRAM_DOWNLOAD_MODE_MINIDRV_CMD,
} wiced_bt_patchram_download_mode_t;

typedef int platform_result_t;

typedef struct
{   
    uint32_t                          patchram_download_baud_rate;
    wiced_bt_patchram_download_mode_t patchram_download_mode;
    uint32_t                          featured_baud_rate;
    uint32_t                          default_baud_rate;
} platform_bluetooth_config_t;

#if 0

/**
 * UART data width
 */
typedef enum
{
    DATA_WIDTH_5BIT,    /**< 5 Bit data */
    DATA_WIDTH_6BIT,    /**< 6 Bit data */
    DATA_WIDTH_7BIT,    /**< 7 Bit data */
    DATA_WIDTH_8BIT,    /**< 8 Bit data */
    DATA_WIDTH_9BIT     /**< 9 Bit data */
} platform_uart_data_width_t;

/**
 * UART stop bits
 */
typedef enum
{
    STOP_BITS_1,        /**< 1 Stop bit  */
    STOP_BITS_2,        /**< 2 Stop bits */
} platform_uart_stop_bits_t;

/**
 * UART flow control
 */
typedef enum
{
    FLOW_CONTROL_DISABLED,      /**< No Flow Control  */
    FLOW_CONTROL_CTS,           /**< CTS Flow Control  */
    FLOW_CONTROL_RTS,           /**< RTS Flow Control  */
    FLOW_CONTROL_CTS_RTS        /**< CTS RTS Flow Control */
} platform_uart_flow_control_t;

/**
 * UART parity
 */
typedef enum
{
    NO_PARITY,      /**< No Parity   */
    ODD_PARITY,     /**< Odd Parity  */
    EVEN_PARITY,    /**< Even Parity */
} platform_uart_parity_t;

/**
 * UART configuration
 */
typedef struct
{
    uint32_t                     baud_rate;     /**< Baud rate */
    platform_uart_data_width_t   data_width;    /**< Data width */
    platform_uart_parity_t       parity;        /**< Parity */
    platform_uart_stop_bits_t    stop_bits;     /**< Number of stop bits */
    platform_uart_flow_control_t flow_control;
} platform_uart_config_t;
#endif

extern const platform_bluetooth_config_t wiced_bt_config;
