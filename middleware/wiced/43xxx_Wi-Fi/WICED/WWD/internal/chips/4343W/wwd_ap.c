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
 *  Provides an APSTA functionality specific to the 43362A2
 */

#include <string.h>
#include "wwd_rtos.h"
#include "wwd_events.h"
#include "wwd_assert.h"
#include "wwd_management.h"
#include "wwd_wifi.h"
#include "network/wwd_buffer_interface.h"
#include "internal/wwd_sdpcm.h"
#include "internal/wwd_internal.h"
#include "internal/wwd_ap.h"
#include "platform_toolchain.h"
#include "internal/wwd_ap_common.h"


/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 * @cond               Constants
 ******************************************************/

#define WLC_EVENT_MSG_LINK      (0x01)
#define RATE_SETTING_11_MBPS    (11000000 / 500000)

/* Soft AP Options */
#define WICED_DEFAULT_SOFT_AP_DTIM_PERIOD       (1)

/** @endcond */

/* HT/AMPDU specific define */
#define AMPDU_AP_DEFAULT_BA_WSIZE   2   /* AP default BA window size */
#define AMPDU_STA_DEFAULT_BA_WSIZE  8   /* STA default BA window size */
#define AMPDU_STA_DEFAULT_MPDU      4   /* STA default num MPDU per AMPDU */

#define WEP40_KEY_LENGTH                     5
#define WEP104_KEY_LENGTH                    13
#define FORMATTED_ASCII_WEP40_KEY_LENGTH     28 /* For 5  bytes key */
#define FORMATTED_ASCII_WEP104_KEY_LENGTH    60 /* For 13 bytes key */

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/


/******************************************************
 *               Variables Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

/** Sets the chip specific AMPDU parameters for AP and STA
 *  For SDK 3.0, and beyond, each chip will need it's own function for setting AMPDU parameters.
 */

wwd_result_t wwd_wifi_set_ampdu_parameters( void )
{
    return wwd_wifi_set_ampdu_parameters_common( WWD_STA_INTERFACE, AMPDU_STA_DEFAULT_BA_WSIZE, AMPDU_STA_DEFAULT_MPDU, AMPDU_RX_FACTOR_8K );
}

/** Sets the chip specific AMPDU parameters for AP and STA
 *  For SDK 3.0, and beyond, each chip will need it's own function for setting AMPDU parameters.
 */
wwd_result_t wwd_wifi_set_block_ack_window_size( wwd_interface_t interface )
{
    return wwd_wifi_set_block_ack_window_size_common( interface, AMPDU_AP_DEFAULT_BA_WSIZE, AMPDU_STA_DEFAULT_BA_WSIZE );
}

