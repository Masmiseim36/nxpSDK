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
 * This file provides definitions and function prototypes for Hello Sensor
 * device
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                Constants
 ******************************************************************************/

#define BLE_LTK_FILE "ble_ltk.dat"

#define COMMISSIONING_CHAR_WIFI_VAL_SIZE_B 150

#define HELLO_SENSOR_MAX_NUM_CLIENTS 1

/* Hello Sensor App Timer Timeout in seconds  */
#define HELLO_SENSOR_APP_TIMEOUT_IN_SECONDS 1

/* Hello Sensor App Fine Timer Timeout in milli seconds  */
#define HELLO_SENSOR_APP_FINE_TIMEOUT_IN_MS 1000

/* Hello Sensor Connection Idle  Timeout in milli seconds  */
#define HELLO_SENSOR_CONN_IDLE_TIMEOUT_IN_SECONDS 3

/******************************************************************************
 *                          Constants
 ******************************************************************************/
/* UUID value of the Commissioning Service */
#define UUID_COMMISSIONING_SERVICE \
    0x9a, 0x98, 0x1c, 0xc8, 0x4c, 0xa4, 0x02, 0x96, 0xe9, 0x11, 0x00, 0x2f, 0x30, 0x25, 0x9d, 0x3e

/* UUID value of the WiFi Characteristic */
#define UUID_COMMISSIONING_CHARACTERISTIC_WIFI \
    0x9a, 0x98, 0x1c, 0xc8, 0x4c, 0xa4, 0x02, 0x96, 0xe9, 0x11, 0x00, 0x2f, 0x32, 0x25, 0x9d, 0x3e

/******************************************************************************
 *                         Type Definitions
 ******************************************************************************/
typedef enum
{
    HANDLE_HSENS_GATT_SERVICE = 0x1,

    HANDLE_HSENS_GAP_SERVICE = 0x14,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL,

    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE = 0x40,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL,

    /* Commissioning Service */
    HANDLE_COMMISSIONING_SERVICE = 0x70,
    HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI,
    HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI_VAL,
} hello_sensor_db_tags;

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
/**
 * @brief Starts the ble server
 *
 * Set the ble device name and register the callback for the BT stack.
 * Create the event_group_bit which will be used to notify that wifi credentials were received.
 *
 * @params *dev_name Ble device name (pointer to a null-terminated string)
 */
void ble_server_start(char *dev_name);

/**
 * @brief Stops the ble server
 *
 * Disconnect any outstanding connections.
 * Deinit the BT stack.
 *
 */
void ble_server_stop(void);

/**
 * @brief Returns the wifi characteristic as a null-terminated string
 *
 * @params *val Allocated pointer where to copy the characteristic value
 */
void ble_server_get_char_wifi_str(char *val);
