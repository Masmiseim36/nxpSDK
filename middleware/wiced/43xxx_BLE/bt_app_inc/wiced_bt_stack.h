/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
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

/** @file
 *
 * Bluetooth Management (BTM) Application Programming Interface
 *
 * The BTM consists of several management entities:
 *      1. Device Control - controls the local device
 *      2. Device Discovery - manages inquiries, discover database
 *      3. ACL Channels - manages ACL connections (BR/EDR and LE)
 *      4. SCO Channels - manages SCO connections
 *      5. Security - manages all security functionality
 *      6. Power Management - manages park, sniff, hold, etc.
 *
 * @defgroup wicedbt      Bluetooth
 *
 * WICED Bluetooth Framework Functions
 */

#pragma once

#include "wiced_bt_cfg.h"

/******************************************************
 *               Function Declarations
 ******************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/**
 * Framework Management Functions
 *
 * @addtogroup  wicedbt_Framework   Framework
 * @ingroup     wicedbt
 *
 *
 * Bluetooth Management (BTM) Application Programming Interface
 *
 * The BTM consists of several management entities:
 *      1. Device Control - controls the local device
 *      2. Device Discovery - manages inquiries, discover database
 *      3. ACL Channels - manages ACL connections (BR/EDR and LE)
 *      4. SCO Channels - manages SCO connections
 *      5. Security - manages all security functionality
 *      6. Power Management - manages park, sniff, hold, etc.
 *
 * @{
 */
/****************************************************************************/

/**
 * Function         wiced_bt_stack_init
 *
 *                  Initialize the Bluetooth controller and stack; register
 *                  callback for Bluetooth event notification.
 *
 * @param[in] p_bt_management_cback     : Callback for receiving Bluetooth management events
 * @param[in] p_bt_cfg_settings         : Bluetooth stack configuration
 * @param[in] wiced_bt_cfg_buf_pools    : Buffer pool configuration
 *
 * @return    WICED_BT_SUCCESS : on success;
 *            WICED_BT_ERROR   : if an error occurred
 */
wiced_result_t wiced_bt_stack_init(wiced_bt_management_cback_t *p_bt_management_cback,
                                   const wiced_bt_cfg_settings_t     *p_bt_cfg_settings,
                                   const wiced_bt_cfg_buf_pool_t     wiced_bt_cfg_buf_pools[WICED_BT_CFG_NUM_BUF_POOLS]);

/**
 * Function         wiced_bt_stack_deinit
 *
 *                  This function blocks until all de-initialisation procedures are complete.
 *                  It is recommended that the application disconnect any outstanding connections prior to invoking this function.
 *
 * @return    WICED_BT_SUCCESS : on success;
 *            WICED_BT_ERROR   : if an error occurred
 */
wiced_result_t wiced_bt_stack_deinit( void );

/**@} wicedbt_Framework */


#ifdef __cplusplus
}
#endif


