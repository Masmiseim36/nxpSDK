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
 * BLE Vendor Specific Device
 *
 * Features demonstrated
 *  - GATT database and Device configuration initialization
 *  - Registration with LE stack for various events
 *  - Sending data to the client
 *  - Processing write requests from the client
 *
 * To demonstrate the app, work through the following steps.
 * 1. Plug the WICED eval board into your computer
 * 2. Build and download the application (to the WICED board)
 * 3. On application start the device acts as a GATT server and advertises itself as Hello Sensor
 * 4. Connect to GATT server using one of the LE clients (LEExplorer(android)) or (BLE Utility(Apple Store))
 * 5. Once connected the client can read Hello sensor characteristics
 * 6. Write the hello sensor characteristic configuration value from client
 * 7. on the console terminal you can see the value entered in the client
 * 8. To recieve notification the user has to register for notification after LE connection on the client
 * 9. To test notifications user can enter the command "notify" on the console
 * 10. Each time when user enters the notify command a new value of notification can be seen on the LE client.
 *
 * ENABLE_APP_POWERSAVE is defined to enable power-save related changes in the applications
 * For accurate current measurement, turn off LEDs on the board.
 *
 */

#include <string.h>
#include <stdlib.h>
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "wiced_bt_dev.h"
#include "fsl_debug_console.h"
#include "wiced_rtos.h"

#include "ble_server.h"
#include "cJSON.h"
#include "event_groups.h"
#include "platform_imxrt.h"

#include "sln_flash_mgmt.h"

/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define ERR_CMD_OK  0
#define ERR_UNKNOWN 1

#ifndef BLE_LTK_FILE
#define SIMULATE_NVRAM_WITH_ITC_RAM
#endif

/******************************************************************************
 *                           Function Prototypes
 ******************************************************************************/
static void hello_sensor_application_init(void);
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_status_handler(wiced_bt_gatt_connection_status_t *p_status);
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_up(wiced_bt_gatt_connection_status_t *p_status);
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_down(wiced_bt_gatt_connection_status_t *p_status);
static wiced_result_t hello_sensor_management_callback(wiced_bt_management_evt_t event,
                                                       wiced_bt_management_evt_data_t *p_event_data);
static wiced_bt_gatt_status_t hello_sensor_gatts_callback(wiced_bt_gatt_evt_t event,
                                                          wiced_bt_gatt_event_data_t *p_data);
static wiced_bt_gatt_status_t hello_sensor_gatt_server_read_request_handler(uint16_t conn_id,
                                                                            wiced_bt_gatt_read_t *p_read_data);
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_request_handler(uint16_t conn_id,
                                                                             wiced_bt_gatt_write_t *p_data);
static void hello_sensor_set_advertisement_data(void);
static void hello_sensor_set_scan_response_data(void);
static void hello_sensor_send_message(void);
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_and_execute_request_handler(
    uint16_t conn_id, wiced_bt_gatt_exec_flag_t exec_flag);
static void hello_sensor_advertisement_stopped(void);

wiced_result_t ble_hello_sensor_load_key_to_addr_resolution_db(void);
void ble_hello_sensor_save_link_key(wiced_bt_device_link_keys_t paired_device_keys);
void ble_hello_sensor_read_link_key(wiced_bt_device_link_keys_t *paired_device_keys);

extern wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];

int nvm_load_keys(wiced_bt_device_link_keys_t *keys, int key_len);
int nvm_save_keys(wiced_bt_device_link_keys_t *keys, int key_len);

/******************************************************************************
 *                                Structures
 ******************************************************************************/

typedef struct
{
    BD_ADDR remote_addr;          /* remote peer device address */
    uint32_t timer_count;         /* timer count */
    uint32_t fine_timer_count;    /* fine timer count */
    uint16_t conn_id;             /* connection ID referenced by the stack */
    uint16_t peer_mtu;            /* peer MTU */
    uint8_t flag_indication_sent; /* indicates waiting for confirmation */
    uint8_t flag_stay_connected;  /* stay connected or disconnect after all
                                   * messages are sent */
} hello_sensor_state_t;

typedef PACKED struct
{
    BD_ADDR bdaddr;                               /* BD address of the bonded host */
    uint16_t characteristic_client_configuration; /* Current value of the
                                                   * client configuration
                                                   * descriptor */
} host_info_t;

typedef struct
{
    uint16_t handle;
    uint16_t attr_len;
    void *p_attr;
} attribute_t;

/******************************************************************************
 *                                Variables Definitions
 ******************************************************************************/
static uint8_t hello_sensor_device_name[]      = "mcu-voice";
static uint8_t hello_sensor_appearance_name[2] = {BIT16_TO_8(APPEARANCE_GENERIC_TAG)};
static char hello_sensor_char_mfr_name_value[] = {
    'C', 'Y', 'P', 'R', 'E', 'S', 'S', 0,
};
static char hello_sensor_char_model_num_value[]    = {'1', '2', '3', '4', 0, 0, 0, 0};
static uint8_t hello_sensor_char_system_id_value[] = {0xbb, 0xb8, 0xa1, 0x80, 0x5f, 0x9f, 0x91, 0x71};

static struct
{
    char val[COMMISSIONING_CHAR_WIFI_VAL_SIZE_B];
    unsigned char val_len;
    char credentials_received;
    wiced_bt_gatt_status_t write_status;
} commissioning_char_wifi = {{0}, 0, 0, WICED_BT_GATT_INVALID_HANDLE};

static hello_sensor_state_t hello_sensor_state;
static host_info_t hello_sensor_hostinfo;
static wiced_bool_t is_connected = FALSE;

attribute_t ble_gatt_user_attributes[] = {
    {HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL, sizeof(hello_sensor_device_name), hello_sensor_device_name},
    {HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL, sizeof(hello_sensor_appearance_name),
     hello_sensor_appearance_name},
    {HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL, sizeof(hello_sensor_char_mfr_name_value),
     hello_sensor_char_mfr_name_value},
    {HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL, sizeof(hello_sensor_char_model_num_value),
     hello_sensor_char_model_num_value},
    {HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL, sizeof(hello_sensor_char_system_id_value),
     hello_sensor_char_system_id_value},
    {HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI_VAL, 0, commissioning_char_wifi.val},
};

EventGroupHandle_t ble_cred_received_event;
static wiced_bt_device_link_keys_t g_ble_dev_link_keys;
/******************************************************************************
 *                                GATT DATABASE
 ******************************************************************************/
/*
 * This is the GATT database for the Hello Sensor application.  It defines
 * services, characteristics and descriptors supported by the sensor.  Each
 * attribute in the database has a handle, (characteristic has two, one for
 * characteristic itself, another for the value).  The handles are used by
 * the peer to access attributes, and can be used locally by application for
 * example to retrieve data written by the peer.  Definition of characteristics
 * and descriptors has GATT Properties (read, write, notify...) but also has
 * permissions which identify if and how peer is allowed to read or write
 * into it.  All handles do not need to be sequential, but need to be in
 * ascending order.
 */
const uint8_t ble_gatt_database[] = {
    /* Declare mandatory GATT service */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_GATT_SERVICE, UUID_SERVCLASS_GATT_SERVER),

    /* Declare mandatory GAP service. Device Name and Appearance are mandatory
     * characteristics of GAP service                                        */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_GAP_SERVICE, UUID_SERVCLASS_GAP_SERVER),

    /* Declare mandatory GAP service characteristic: Dev Name */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME,
                          HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL,
                          GATT_UUID_GAP_DEVICE_NAME,
                          LEGATTDB_CHAR_PROP_READ,
                          LEGATTDB_PERM_READABLE),

    /* Declare mandatory GAP service characteristic: Appearance */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE,
                          HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,
                          GATT_UUID_GAP_ICON,
                          LEGATTDB_CHAR_PROP_READ,
                          LEGATTDB_PERM_READABLE),

    /* Declare Device info service */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE, UUID_SERVCLASS_DEVICE_INFO),

    /* Handle 0x4e: characteristic Manufacturer Name, handle 0x4f
     * characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME,
                          HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,
                          GATT_UUID_MANU_NAME,
                          LEGATTDB_CHAR_PROP_READ,
                          LEGATTDB_PERM_READABLE),

    /* Handle 0x50: characteristic Model Number, handle 0x51 characteristic
     * value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM,
                          HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL,
                          GATT_UUID_MODEL_NUMBER_STR,
                          LEGATTDB_CHAR_PROP_READ,
                          LEGATTDB_PERM_READABLE),

    /* Handle 0x52: characteristic System ID,handle 0x53 characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID,
                          HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL,
                          GATT_UUID_SYSTEM_ID,
                          LEGATTDB_CHAR_PROP_READ,
                          LEGATTDB_PERM_READABLE),

    /* Declare Commissioning Service (with 128 byte UUID) */
    PRIMARY_SERVICE_UUID128(HANDLE_COMMISSIONING_SERVICE, UUID_COMMISSIONING_SERVICE),

    /* Declare Wifi characteristic */
    CHARACTERISTIC_UUID128_WRITABLE(HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI,
                                    HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI_VAL,
                                    UUID_COMMISSIONING_CHARACTERISTIC_WIFI,
                                    LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_WRITE,
                                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_RELIABLE_WRITE),
};
/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
 
 /*
 * Some of the variables below are currently unused. This may change when print macro are enabled.
 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
void ble_server_start(char *dev_name)
{
    // wiced_core_init();

#ifdef BLE_LTK_FILE
    // Intialize file
    int32_t ret      = 0;
    uint8_t temp[]   = {0};
    uint32_t tempLen = 0;

    ret = SLN_FLASH_MGMT_Read(BLE_LTK_FILE, NULL, &tempLen);

    if (SLN_FLASH_MGMT_ENOENTRY2 == ret)
    {
        SLN_FLASH_MGMT_Save(BLE_LTK_FILE, temp, sizeof(wiced_bt_device_link_keys_t));
    }

#endif

    wiced_bt_cfg_settings.device_name = (uint8_t *)dev_name;
    /* Register call back and configuration with stack */
    wiced_bt_stack_init(hello_sensor_management_callback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
    WPRINT_BT_APP_INFO(("Hello Sensor Start\r\n"));

    ble_cred_received_event = xEventGroupCreate();
}

void ble_server_stop(void)
{
    if (is_connected)
    {
        wiced_bt_gatt_disconnect(hello_sensor_state.conn_id);
    }

    wiced_bt_stack_deinit();

    vEventGroupDelete(ble_cred_received_event);
}

/*
 * This function is executed in the BTM_ENABLED_EVT management callback.
 */
static void hello_sensor_application_init(void)
{
    wiced_bt_gatt_status_t gatt_status;
    wiced_result_t result;

    WPRINT_BT_APP_INFO(("hello_sensor_application_init\r\n"));

    /* Register with stack to receive GATT callback */
    gatt_status = wiced_bt_gatt_register(hello_sensor_gatts_callback);

    WPRINT_BT_APP_INFO(("\n wiced_bt_gatt_register: %d\r\n", gatt_status));

    /*  Tell stack to use our GATT database */
    gatt_status = wiced_bt_gatt_db_init(ble_gatt_database, sizeof(ble_gatt_database));

    WPRINT_BT_APP_INFO(("wiced_bt_gatt_db_init %d\r\n", gatt_status));

    /* Set the advertising parameters and make the device discoverable */
    hello_sensor_set_advertisement_data();
    hello_sensor_set_scan_response_data();

    /* Load the address resolution DB with the keys stored in the DCT */
    result = ble_hello_sensor_load_key_to_addr_resolution_db();

    result = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

    WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements %d\r\n", result));

    /*
     * Set flag_stay_connected to remain connected after all messages are sent
     * Reset flag to 0, to disconnect
     */
    hello_sensor_state.flag_stay_connected = 1;

    /* app_powersave_sleep() needs to be called here only in case of Embedded mode.
     * In case of host mode this will be called when BTM_LPM_STATE_LOW_POWER event is received from stack */
#ifdef USE_WICED_HCI
    app_powersave_sleep();
#endif
}

/*
 * Setup advertisement data with 16 byte UUID and device name
 */
static void hello_sensor_set_advertisement_data(void)
{
    wiced_result_t result;
    wiced_bt_ble_advert_elem_t adv_elem[3];
    uint8_t ble_advertisement_flag_value = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t num_elem                     = 0;

    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len         = 1;
    adv_elem[num_elem].p_data      = &ble_advertisement_flag_value;
    num_elem++;

    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len         = strlen((const char *)wiced_bt_cfg_settings.device_name);
    WPRINT_BT_APP_INFO(("wiced_bt_cfg_settings.device_name:%s\r\n", wiced_bt_cfg_settings.device_name));
    adv_elem[num_elem].p_data = (uint8_t *)wiced_bt_cfg_settings.device_name;

    num_elem++;

    result = wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);

    WPRINT_BT_APP_INFO(("wiced_bt_ble_set_advertisement_data %d\r\n", result));
}

/*
 * Setup scan response data with 16 byte UUID and device name
 */
static void hello_sensor_set_scan_response_data(void)
{
    wiced_result_t result;
    wiced_bt_ble_advert_elem_t adv_elem[1];
    uint8_t num_elem                         = 0;
    uint8_t hello_service_uuid[LEN_UUID_128] = {UUID_COMMISSIONING_SERVICE};

    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE;
    adv_elem[num_elem].len         = LEN_UUID_128;
    adv_elem[num_elem].p_data      = hello_service_uuid;
    num_elem++;

    result = wiced_bt_ble_set_raw_scan_response_data(num_elem, adv_elem);

    WPRINT_BT_APP_INFO(("wiced_bt_ble_set_scan_response_data %d\r\n", result));
}

/*
 * This function is invoked when advertisements stop.  If we are configured to stay connected,
 * disconnection was caused by the peer, start low advertisements, so that peer can connect
 * when it wakes up
 */
static void hello_sensor_advertisement_stopped(void)
{
    wiced_result_t result;

    if (hello_sensor_state.flag_stay_connected && !hello_sensor_state.conn_id)
    {
        result = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL);
        WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements: %d\r\n", result));
    }
    else
    {
        WPRINT_BT_APP_INFO(("ADV stop\n"));
    }
}

/*
 * hello_sensor bt/ble link management callback
 */
static wiced_result_t hello_sensor_management_callback(wiced_bt_management_evt_t event,
                                                       wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t result = WICED_BT_SUCCESS;
    wiced_bt_dev_ble_pairing_info_t *p_info;
    wiced_bt_ble_advert_mode_t *p_mode;
    // wiced_bool_t                   is_warmboot; // = WICED_DEEP_SLEEP_IS_WARMBOOT( );

    WPRINT_BT_APP_INFO(("hello_sensor_management_callback: %x\r\n", event));

    switch (event)
    {
        /* Bluetooth  stack enabled */
        case BTM_ENABLED_EVT:
            hello_sensor_application_init();

#ifdef ENABLE_APP_POWERSAVE
            /* Break early as initializing command console and hence STDIO_UART increases the power numbers */
            break;
#endif
            break;

        case BTM_DISABLED_EVT:
            break;

        case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
            p_event_data->pairing_io_capabilities_ble_request.local_io_cap = BTM_IO_CAPABILITIES_NONE;
            p_event_data->pairing_io_capabilities_ble_request.oob_data     = BTM_OOB_NONE;
            p_event_data->pairing_io_capabilities_ble_request.auth_req = BTM_LE_AUTH_REQ_BOND | BTM_LE_AUTH_REQ_MITM;
            p_event_data->pairing_io_capabilities_ble_request.max_key_size = 0x10;
            p_event_data->pairing_io_capabilities_ble_request.init_keys    = BTM_LE_KEY_PENC | BTM_LE_KEY_PID;
            p_event_data->pairing_io_capabilities_ble_request.resp_keys    = BTM_LE_KEY_PENC | BTM_LE_KEY_PID;
            break;

        case BTM_PAIRING_COMPLETE_EVT:
            p_info = &p_event_data->pairing_complete.pairing_complete_info.ble;
            WPRINT_BT_APP_INFO(("Pairing Complete: %d\r\n", p_info->reason));
            nvm_save_keys(&g_ble_dev_link_keys, sizeof(wiced_bt_device_link_keys_t));
            break;

        case BTM_SECURITY_REQUEST_EVT:
            WPRINT_BT_APP_INFO(("BTM Security Event\r\n"));
            wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
            break;

        case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        {
            WPRINT_APP_INFO(("Key Upd\r\n"));
            /******
            WPRINT_APP_INFO((" Paired Device Link Keys Update Event\n"));
            WPRINT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                        p_event_data->paired_device_link_keys_request.bd_addr[0],
                        p_event_data->paired_device_link_keys_request.bd_addr[1],
                        p_event_data->paired_device_link_keys_request.bd_addr[2],
                        p_event_data->paired_device_link_keys_request.bd_addr[3],
                        p_event_data->paired_device_link_keys_request.bd_addr[4],
                        p_event_data->paired_device_link_keys_request.bd_addr[5]));
            ********/
            ble_hello_sensor_save_link_key(p_event_data->paired_device_link_keys_request);
            break;
        }

        case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        {
            WPRINT_APP_INFO(("Paired Device Link Keys Request Event\r\n"));
            ble_hello_sensor_read_link_key(&p_event_data->paired_device_link_keys_request);
            WPRINT_APP_INFO(("Key retrieval success\r\n"));

            break;
        }

        case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
            p_mode = &p_event_data->ble_advert_state_changed;
            WPRINT_BT_APP_INFO(("Advertisement State Change: %d\r\n", *p_mode));
            if (*p_mode == BTM_BLE_ADVERT_OFF)
            {
                hello_sensor_advertisement_stopped();
            }
            break;

        case BTM_LPM_STATE_LOW_POWER:

#ifdef ENABLE_APP_POWERSAVE
            app_powersave_sleep();
#endif
            break;

        default:
            break;
    }

    return result;
}

/*
 * Check if client has registered for notification/indication
 * and send message if appropriate
 */
 #if 0
static void hello_sensor_send_message(void)
{
    WPRINT_BT_APP_INFO(("%s: Client's Characteristic configuration:%d\r\n", __func__,
                        hello_sensor_hostinfo.characteristic_client_configuration));

    /* If client has not registered for indication or notification, no action */
    if (hello_sensor_hostinfo.characteristic_client_configuration == 0)
    {
        return;
    }
}
#endif

/*
 * Find attribute description by handle
 */
static attribute_t *hello_sensor_get_attribute(uint16_t handle)
{
    int i;
    for (i = 0; i < sizeof(ble_gatt_user_attributes) / sizeof(ble_gatt_user_attributes[0]); i++)
    {
        if (ble_gatt_user_attributes[i].handle == handle)
        {
            return (&ble_gatt_user_attributes[i]);
        }
    }
    WPRINT_BT_APP_INFO(("attribute not found:%x\r\n", handle));
    return NULL;
}

/*
 * Process Read request or command from peer device
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_read_request_handler(uint16_t conn_id,
                                                                            wiced_bt_gatt_read_t *p_read_data)
{
    attribute_t *puAttribute;
    int attr_len_to_copy;

    if ((puAttribute = hello_sensor_get_attribute(p_read_data->handle)) == NULL)
    {
        WPRINT_BT_APP_INFO(("read_hndlr attr not found hdl:%x\r\n", p_read_data->handle));
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    attr_len_to_copy = puAttribute->attr_len;

    WPRINT_BT_APP_INFO(("read_hndlr conn_id:%d hdl:%x offset:%d len:%d\r\n", conn_id, p_read_data->handle,
                        p_read_data->offset, attr_len_to_copy));

    if (p_read_data->offset >= puAttribute->attr_len)
    {
        attr_len_to_copy = 0;
    }

    if (attr_len_to_copy != 0)
    {
        uint8_t *from;
        int to_copy = attr_len_to_copy - p_read_data->offset;

        if (to_copy > *p_read_data->p_val_len)
        {
            to_copy = *p_read_data->p_val_len;
        }

        from                    = ((uint8_t *)puAttribute->p_attr) + p_read_data->offset;
        *p_read_data->p_val_len = to_copy;

        memcpy(p_read_data->p_val, from, to_copy);
    }

    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process write request or write command from peer device
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_request_handler(uint16_t conn_id,
                                                                             wiced_bt_gatt_write_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_SUCCESS;

    WPRINT_BT_APP_INFO(("write_handler: conn_id:%d hdl:0x%x prep:%d offset:%d len:%d\r\n ", conn_id, p_data->handle,
                        p_data->is_prep, p_data->offset, p_data->val_len));

    switch (p_data->handle)
    {
        default:
            result = WICED_BT_GATT_INVALID_HANDLE;
            break;
    }
    return result;
}

/*
 * Prep-Write Procedure
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_prep_write_request_handler(uint16_t conn_id,
                                                                                  wiced_bt_gatt_write_t *p_write_req)
{
    wiced_bt_gatt_status_t result        = WICED_BT_GATT_SUCCESS;
    attribute_t *commissioning_attribute = NULL;
    unsigned char write_req_len          = 0;

    WPRINT_BT_APP_INFO(("prep_write_handler: conn_id:%d hdl:0x%x prep:%d offset:%d len:%d\r\n ", conn_id,
                        p_write_req->handle, p_write_req->is_prep, p_write_req->offset, p_write_req->val_len));

    switch (p_write_req->handle)
    {
        case HANDLE_COMMISSIONING_SERVICE_CHAR_WIFI_VAL:
            commissioning_char_wifi.write_status = WICED_BT_GATT_SUCCESS;

            commissioning_attribute = hello_sensor_get_attribute(p_write_req->handle);
            if (NULL == commissioning_attribute)
            {
                WPRINT_BT_APP_INFO(("prep_write_hndlr attr not found hdl:%x\r\n", p_write_req->handle));

                commissioning_char_wifi.write_status = WICED_BT_GATT_INVALID_HANDLE;
                result                               = WICED_BT_GATT_INVALID_HANDLE;
                break;
            }

            write_req_len = p_write_req->offset + p_write_req->val_len;
            if (sizeof(commissioning_char_wifi.val) < write_req_len)
            {
                WPRINT_BT_APP_INFO(("prep_write_hndlr attr not long enough hdl:%x (%d < %d)\r\n", p_write_req->handle,
                                    WICED_BT_GATT_INVALID_ATTR_LEN, write_req_len));

                commissioning_char_wifi.write_status = WICED_BT_GATT_INVALID_ATTR_LEN;
                result                               = WICED_BT_GATT_INVALID_ATTR_LEN;
                break;
            }

            memcpy(&commissioning_char_wifi.val[p_write_req->offset], p_write_req->p_val, p_write_req->val_len);
            commissioning_char_wifi.val_len   = write_req_len;
            commissioning_attribute->attr_len = write_req_len;

            commissioning_char_wifi.credentials_received = 1;
            break;

        default:
            result = WICED_BT_GATT_INVALID_HANDLE;
            break;
    }
    return result;
}

/*
 * Write Execute Procedure
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_and_execute_request_handler(
    uint16_t conn_id, wiced_bt_gatt_exec_flag_t exec_flag)
{
    WPRINT_BT_APP_INFO(("write exec: flag:%d\r\n", exec_flag));

    if (commissioning_char_wifi.credentials_received)
    {
        /* Credentials successfully received */
        commissioning_char_wifi.credentials_received = 0;

        /* If valid credentials received set the event bit */
        if ((commissioning_char_wifi.write_status == WICED_BT_GATT_SUCCESS) && (commissioning_char_wifi.val_len > 0))
        {
            xEventGroupSetBits(ble_cred_received_event, 1);
        }

        return commissioning_char_wifi.write_status;
    }

    return WICED_BT_GATT_INVALID_HANDLE;
}

/*
 * Process MTU request from the peer
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_mtu_request_handler(uint16_t conn_id, uint16_t mtu)
{
    WPRINT_BT_APP_INFO(("req_mtu: %d\r\n", mtu));
    return WICED_BT_GATT_SUCCESS;
}

static wiced_bt_gatt_status_t hello_sensor_gatt_server_confirmation_handler(uint16_t conn_id, uint16_t handle)
{
    WPRINT_BT_APP_INFO(("hello_sensor_indication_confirmation, conn %d hdl %d\r\n", conn_id, handle));
    if (!hello_sensor_state.flag_indication_sent)
    {
        WPRINT_BT_APP_INFO(("Hello: Wrong Confirmation!\r\n"));
        return WICED_BT_GATT_SUCCESS;
    }
    hello_sensor_state.flag_indication_sent = 0;
    // hello_sensor_send_message();
    return WICED_BT_GATT_SUCCESS;
}

/* This function is invoked when connection is established */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_up(wiced_bt_gatt_connection_status_t *p_status)
{
    wiced_result_t result;

    WPRINT_BT_APP_INFO(("hello_sensor_conn_up  id:%d\r\n:", p_status->conn_id));

    /* Update the connection handler.  Save address of the connected device. */
    hello_sensor_state.conn_id = p_status->conn_id;
    memcpy(hello_sensor_state.remote_addr, p_status->bd_addr, sizeof(BD_ADDR));

    /* Stop advertising */
    result = wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);

    WPRINT_BT_APP_INFO(("Stopping Advertisements%d\r\n", result));

#if 1
    // Initiating the security. If call to the Bond returns success, device is bonded, and we just need to setup
    // encryption
    if ((result = wiced_bt_dev_sec_bond(p_status->bd_addr, p_status->addr_type, BT_TRANSPORT_LE, 0, NULL)) ==
        WICED_BT_SUCCESS)
    {
        WPRINT_APP_INFO(("wifi_introducer starting encryption\r\n"));
        wiced_bt_dev_set_encryption(p_status->bd_addr, BT_TRANSPORT_LE, NULL);
    }
    else
    {
        // WICED_BT_TRACE( "wiced_bt_dev_sec_bond %d \n", result );
        /* Updating the bd address in the  host info in NVRAM  */
        memcpy(hello_sensor_hostinfo.bdaddr, p_status->bd_addr, sizeof(BD_ADDR));
    }
#endif

    memcpy(hello_sensor_hostinfo.bdaddr, p_status->bd_addr, sizeof(BD_ADDR));
    hello_sensor_hostinfo.characteristic_client_configuration = 0;

    return WICED_BT_GATT_SUCCESS;
}

/*
 * This function is invoked when connection is lost
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_down(wiced_bt_gatt_connection_status_t *p_status)
{
    wiced_result_t result;

    WPRINT_BT_APP_INFO(("connection_down  conn_id:%d reason:%d\r\n", p_status->conn_id, p_status->reason));

    /* Resetting the device info */
    memset(hello_sensor_state.remote_addr, 0, 6);
    hello_sensor_state.conn_id = 0;

    /*
     * If we are configured to stay connected, disconnection was
     * caused by the peer, start low advertisements, so that peer
     * can connect when it wakes up
     */
    if (hello_sensor_state.flag_stay_connected)
    {
        result = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL);
        WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements %d\r\n", result));
    }

    /* app_powersave_sleep() needs to be called here only in case of Embedded mode.
     * In case of host mode this will be called when BTM_LPM_STATE_LOW_POWER event is received from stack */
#ifdef USE_WICED_HCI
    app_powersave_sleep();
#endif

    return WICED_BT_SUCCESS;
}

/*
 * Connection up/down event
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_status_handler(wiced_bt_gatt_connection_status_t *p_status)
{
    is_connected = p_status->connected;
    if (p_status->connected)
    {
        return hello_sensor_gatts_connection_up(p_status);
    }

    return hello_sensor_gatts_connection_down(p_status);
}

/*
 * Process GATT request from the peer
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_request_handler(wiced_bt_gatt_attribute_request_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    WPRINT_BT_APP_INFO(
        ("hello_sensor_gatt_server_request_handler. conn %d, type %d\r\n", p_data->conn_id, p_data->request_type));

    switch (p_data->request_type)
    {
        case GATTS_REQ_TYPE_READ:
            result = hello_sensor_gatt_server_read_request_handler(p_data->conn_id, &(p_data->data.read_req));
            break;

        case GATTS_REQ_TYPE_WRITE:
            result = hello_sensor_gatt_server_write_request_handler(p_data->conn_id, &(p_data->data.write_req));
            break;

        case GATTS_REQ_TYPE_PREP_WRITE:
            result = hello_sensor_gatt_server_prep_write_request_handler(p_data->conn_id, &(p_data->data.write_req));
            break;

        case GATTS_REQ_TYPE_WRITE_EXEC:
            result =
                hello_sensor_gatt_server_write_and_execute_request_handler(p_data->conn_id, p_data->data.exec_write);
            break;

        case GATTS_REQ_TYPE_MTU:
            result = hello_sensor_gatt_server_mtu_request_handler(p_data->conn_id, p_data->data.mtu);
            break;

        case GATTS_REQ_TYPE_CONF:
            result = hello_sensor_gatt_server_confirmation_handler(p_data->conn_id, p_data->data.handle);
            break;

        default:
            break;
    }
    return result;
}

/*
 * Callback for various GATT events.  As this application performs only as a GATT server, some of
 * the events are ommitted.
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    switch (event)
    {
        case GATT_CONNECTION_STATUS_EVT:
            result = hello_sensor_gatts_connection_status_handler(&p_data->connection_status);
            break;

        case GATT_ATTRIBUTE_REQUEST_EVT:
            result = hello_sensor_gatt_server_request_handler(&p_data->attribute_request);
            break;

        default:
            break;
    }

    return result;
}

#if DEBUG_POWERSAVE
static void application_sleep_event_handler(wiced_sleep_event_type_t event, int handler)
{
    switch (event)
    {
        case WICED_SLEEP_EVENT_ENTER:
            WPRINT_BT_APP_INFO(("ble_hello_sensor : App Entering  Low power mode \r\n"));
            break;

        case WICED_SLEEP_EVENT_CANCEL:
            WPRINT_BT_APP_INFO(("ble_hello_sensor : App Cancelling Low power mode \r\n"));
            break;

        case WICED_SLEEP_EVENT_LEAVE:
            WPRINT_BT_APP_INFO(("ble_hello_sensor : App Leaving Low power mode \r\n"));
            break;

        default:
            break;
    }
}

/* Call back , which is called before entering or after leaving sleep state */
WICED_SLEEP_EVENT_HANDLER(application_sleep_event_handler_1)
{
    application_sleep_event_handler(event, 0);
}

#endif

void ble_hello_sensor_save_link_key(wiced_bt_device_link_keys_t paired_device_keys)
{
    // WPRINT_BT_APP_INFO( ("ble_hello_sensor_save_link_key\n"));
    memcpy(&g_ble_dev_link_keys, &paired_device_keys, sizeof(wiced_bt_device_link_keys_t));
}

void ble_hello_sensor_read_link_key(wiced_bt_device_link_keys_t *paired_device_keys)
{
    // WPRINT_APP_INFO( ("ble_hello_sensor_read_link_keys\n"));
    memcpy(&paired_device_keys->key_data, &g_ble_dev_link_keys.key_data, sizeof(paired_device_keys->key_data));
}

wiced_result_t ble_hello_sensor_load_key_to_addr_resolution_db(void)
{
    int32_t ret = 0;

    // Copy from persistant storage to global keys structure first
    ret = nvm_load_keys(&g_ble_dev_link_keys, sizeof(wiced_bt_device_link_keys_t));

    if (ret)
    {
        WPRINT_APP_INFO(("Error loading pairing key, %d\r\n", ret));
    }

    return wiced_bt_dev_add_device_to_address_resolution_db(&g_ble_dev_link_keys);
}

#ifdef SIMULATE_NVRAM_WITH_ITC_RAM
/*
 *  Non Volatile Memory (NVM) routines for testing.
 *  These test NVM routes uses unsed SRAM_ITC RAM as NVM.
 *  On plain reset (not power reset) the contents of SRAM_ITC are retained.
 *  So after resetting the board also connection with 'paired Android mobile'
 *  should work.
 */
#define SRAM_ITC_BUF ((void *)0x10000)

int nvm_load_keys(wiced_bt_device_link_keys_t *keys, int key_len)
{
    WPRINT_BT_APP_INFO(("#LK\n"));
    memcpy(keys, SRAM_ITC_BUF, key_len);
    return 0;
}

int nvm_save_keys(wiced_bt_device_link_keys_t *keys, int key_len)
{
    WPRINT_BT_APP_INFO(("#SK\n"));
    memcpy(SRAM_ITC_BUF, keys, key_len);
    return 0;
}
#else
int nvm_load_keys(wiced_bt_device_link_keys_t *keys, int key_len)
{
    int32_t ret = 0;

    ret = SLN_FLASH_MGMT_Read(BLE_LTK_FILE, (uint8_t *)keys, (uint32_t *)&key_len);

    return ret;
}

int nvm_save_keys(wiced_bt_device_link_keys_t *keys, int key_len)
{
    int32_t ret = 0;

    uint8_t *temp = NULL;

    temp = (uint8_t *)pvPortMalloc(key_len);

    if (NULL == temp)
    {
        ret = 1;
    }

    if (0 == ret)
    {
        ret = SLN_FLASH_MGMT_Read(BLE_LTK_FILE, temp, (uint32_t *)&key_len);
    }

    if (0 == ret)
    {
        if (memcmp(temp, (uint8_t *)keys, key_len))
        {
            // Flash MGMT Save will walk up the pages in the sector
            // Garbage collection on restart will wipe if we have too many
            // Pairing operation is not expected to occur many times before a restart
            ret = SLN_FLASH_MGMT_Save(BLE_LTK_FILE, (uint8_t *)keys, key_len);
        }
    }

    if (NULL != temp)
    {
        memset(temp, 0, key_len);
        vPortFree(temp);
        temp = NULL;
    }

    return ret;
}
#endif

void ble_server_get_char_wifi_str(char *val)
{
    memcpy(val, commissioning_char_wifi.val, commissioning_char_wifi.val_len);
    val[commissioning_char_wifi.val_len] = '\0';
}

#pragma GCC diagnostic pop
#endif /* GNUC pragma for Unused Variables */
