
/**
 *  \file appl_hci_api.c
 *
 *  Source File for HCI Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hci_le.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_HCI_LE" )
#include "iot_logging_setup.h"

#ifdef BT_LE

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
#ifdef BT_5_0
#define MAX_NUM_OF_ANTENNA_ELEMENTS         0xFF
#define MAX_EXTENDED_ADVERTISING_DATA_SIZE  0xFC
#define MAX_SCAN_RSP_DATA_SIZE              0xFC
#define APPL_MAX_LE_SCAN_TYPE               0x02
#define APPL_MAX_LE_SCAN_INTERVAL           0x04
#define APPL_MAX_LE_SCAN_WINDOW             0x04
#define APPL_MAX_OWN_TYPE                   0x02
#define APPL_MAX_PEER_ADDR_TYPE             0x02
#define APPL_MAX_PEER_ADDR                  0x12
#define APPL_MAX_SCANNING_FILTER_POLICY     0x02
#define APPL_MAX_NO_OF_SETS                 0x3F
#define APPL_USE_LEGACY_ADV_FIELD_IS_SET    0x04

/* Maximum number of Initiating PHYs */
#define APPL_MAX_NUM_OF_INIT_PHYS           0x03
/* Maximum number of Scanning PHYs */
#define APPL_MAX_NUM_OF_SCAN_PHYS           0x02
#endif /* BT_5_0 */

/* ----------------------------------------- Functions */
/* Send HCI_LE_SET_EVENT_MASK */
void appl_hci_le_set_event_mask( void )
{
    API_RESULT retval;
    UCHAR   le_event_mask[8];
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter le_event_mask : \n");
    for(index = 0; index<8; index++)
    {
        scanf("%X",&read);
        le_event_mask[index] = (UCHAR) read;
    }
    retval = BT_hci_le_set_event_mask
                  (
                      le_event_mask
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_BUFFER_SIZE */
void appl_hci_le_read_buffer_size( void )
{
    API_RESULT retval;
    retval = BT_hci_le_read_buffer_size();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_LOCAL_SUPPORTED_FEATURES */
void appl_hci_le_read_local_supported_features( void )
{
    API_RESULT retval;
    retval = BT_hci_le_read_local_supported_features();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_RANDOM_ADDRESS */
void appl_hci_le_set_random_address( void )
{
    API_RESULT retval;
    UCHAR   random_address[6];
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter random_address : \n");
    for(index = 0; index<6; index++)
    {
        scanf("%X",&read);
        random_address[index] = (UCHAR) read;
    }
    retval = BT_hci_le_set_random_address
                  (
                      random_address
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_ADVERTISING_PARAMETERS */
void appl_hci_le_set_advertising_parameters( void )
{
    API_RESULT retval;
    UCHAR   advertising_channel_map;
    UCHAR   advertising_filter_policy;
    UCHAR   advertising_type;
    UCHAR   direct_addr[BT_BD_ADDR_SIZE];
    UCHAR   direct_addr_type;
    UCHAR   own_addr_type;
    UINT16   advertising_interval_max;
    UINT16   advertising_interval_min;
    UINT32   index;
    unsigned int read;

    BT_mem_set(direct_addr, 0, BT_BD_ADDR_SIZE);
    direct_addr_type = 0;

    IotLogDebug("Enter advertising_interval_min : \n");
    scanf("%X",&read);
    advertising_interval_min = (UINT16) read;
    IotLogDebug("Enter advertising_interval_max : \n");
    scanf("%X",&read);
    advertising_interval_max = (UINT16) read;
    IotLogDebug("Enter advertising_type : \n");
    scanf("%X",&read);
    advertising_type = (UCHAR) read;
    IotLogDebug("Enter own_addr_type : \n");
    scanf("%X",&read);
    own_addr_type = (UCHAR) read;

    IotLogDebug("Enter direct_addr_type : \n");
    scanf("%X",&read);
    direct_addr_type = (UCHAR) read;
    IotLogDebug("Enter direct_addr : \n");
    for(index = 0; index<6; index++)
    {
        scanf("%X",&read);
        direct_addr[index] = (UCHAR) read;
    }

    IotLogDebug("Enter advertising_channel_map : \n");
    scanf("%X",&read);
    advertising_channel_map = (UCHAR) read;
    IotLogDebug("Enter advertising_filter_policy : \n");
    scanf("%X",&read);
    advertising_filter_policy = (UCHAR) read;
    retval = BT_hci_le_set_advertising_parameters
                  (
                       advertising_interval_min,
                       advertising_interval_max,
                       advertising_type,
                       own_addr_type,
                       direct_addr_type,
                       direct_addr,
                       advertising_channel_map,
                       advertising_filter_policy
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER */
void appl_hci_le_read_advertising_channel_tx_power( void )
{
    API_RESULT retval;
    retval = BT_hci_le_read_advertising_channel_tx_power();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_ADVERTISING_DATA */
void appl_hci_le_set_advertising_data( void )
{
    API_RESULT retval;
    UCHAR   advertising_data[31];
    UCHAR   advertising_data_len;
    UINT32   index;
    unsigned int read;

    BT_mem_set(advertising_data, 0, 31);

    IotLogDebug("Enter advertising_data_len : \n");
    scanf("%X",&read);
    advertising_data_len = (UCHAR) read;
    IotLogDebug("Enter advertising_data : \n");
    for(index = 0; index<advertising_data_len; index++)
    {
        scanf("%X",&read);
        advertising_data[index] = (UCHAR) read;
    }
    retval = BT_hci_le_set_advertising_data
                  (
                      advertising_data_len,
                      advertising_data
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_SCAN_RESPONSE_DATA */
void appl_hci_le_set_scan_response_data( void )
{
    API_RESULT retval;
    UCHAR   scan_data[31];
    UCHAR   scan_data_len;
    UINT32   index;
    unsigned int read;

    BT_mem_set(scan_data, 0, 31);

    IotLogDebug("Enter scan_data_len : \n");
    scanf("%X",&read);
    scan_data_len = (UCHAR) read;
    IotLogDebug("Enter scan_data : \n");
    for(index = 0; index<scan_data_len; index++)
    {
        scanf("%X",&read);
        scan_data[index] = (UCHAR) read;
    }
    retval = BT_hci_le_set_scan_response_data
             (
                 scan_data_len,
                 scan_data
             );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_ADVERTISING_ENABLE */
void appl_hci_le_set_advertising_enable( void )
{
    API_RESULT retval;
    UCHAR   advertising_enable;
    unsigned int read;

    IotLogDebug("Enter advertising_enable : \n");
    scanf("%X",&read);
    advertising_enable = (UCHAR) read;
    retval = BT_hci_le_set_advertising_enable
                  (
                       advertising_enable
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_SCAN_PARAMETERS */
void appl_hci_le_set_scan_parameters( void )
{
    API_RESULT retval;
    UCHAR   le_scan_type;
    UCHAR   own_addr_type;
    UCHAR   scan_filter_policy;
    UINT16   le_scan_interval;
    UINT16   le_scan_window;
    unsigned int read;

    IotLogDebug("Enter le_scan_type : \n");
    scanf("%X",&read);
    le_scan_type = (UCHAR) read;
    IotLogDebug("Enter le_scan_interval : \n");
    scanf("%X",&read);
    le_scan_interval = (UINT16) read;
    IotLogDebug("Enter le_scan_window : \n");
    scanf("%X",&read);
    le_scan_window = (UINT16) read;
    IotLogDebug("Enter own_addr_type : \n");
    scanf("%X",&read);
    own_addr_type = (UCHAR) read;
    IotLogDebug("Enter scan_filter_policy : \n");
    scanf("%X",&read);
    scan_filter_policy = (UCHAR) read;
    retval = BT_hci_le_set_scan_parameters
                  (
                       le_scan_type,
                       le_scan_interval,
                       le_scan_window,
                       own_addr_type,
                       scan_filter_policy
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_SCAN_ENABLE */
void appl_hci_le_set_scan_enable( void )
{
    API_RESULT retval;
    UCHAR   filter_dups;
    UCHAR   le_scan_enable;
    unsigned int read;

    IotLogDebug("Enter le_scan_enable : \n");
    scanf("%X",&read);
    le_scan_enable = (UCHAR) read;
    IotLogDebug("Enter filter_dups : \n");
    scanf("%X",&read);
    filter_dups = (UCHAR) read;
    retval = BT_hci_le_set_scan_enable
                  (
                       le_scan_enable,
                       filter_dups
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_CREATE_CONNECTION */
void appl_hci_le_create_connection( void )
{
    API_RESULT retval;
    UCHAR   initiator_filter_policy;
    UCHAR   own_address_type;
    UCHAR   peer_address[6];
    UCHAR   peer_address_type;
    UINT16   conn_interval_max;
    UINT16   conn_interval_min;
    UINT16   conn_latency;
    UINT16   le_scan_interval;
    UINT16   le_scan_window;
    UINT16   maximum_ce_length;
    UINT16   minimum_ce_length;
    UINT16   supervision_timeout;
    UINT32   index;
    unsigned int read;

    IotLogDebug("Set Default: Yes (1), No (0)\n");
    scanf("%X",&read);

    if (1 == read)
    {
        le_scan_interval = (UINT16) 0x0040;
        le_scan_window = (UINT16) 0x0040;
        initiator_filter_policy = (UCHAR) 0x00;
        peer_address_type = (UCHAR) 0x00;
        IotLogInfo("Enter peer_address : \n");
        for(index = 0; index<6; index++)
        {
            CONSOLE_IN("%X",&read);
            peer_address[index] = (UCHAR) read;
        }
        IotLogInfo("\n0 - Public\n1 - Random\n");
        IotLogInfo("Enter Peer Address Type : \n");
        CONSOLE_IN("%u", &read);
        peer_address_type = (UCHAR) read;
        IotLogInfo("\n0 - Public\n1 - Random\n");
        IotLogInfo("Enter Own Address Type : \n");
        CONSOLE_IN("%u", &read);
        own_address_type = (UCHAR) read;
        conn_interval_min = (UINT16) 0x0006;
        conn_interval_max = (UINT16) 0x0006;
        conn_latency = (UINT16) 0x0000;
        supervision_timeout = (UINT16) 0x03BB;
        maximum_ce_length = (UINT16) 0x0000;
        minimum_ce_length = (UINT16) 0x0000;
    }
    else
    {
        IotLogDebug("Enter le_scan_interval : \n");
        scanf("%X",&read);
        le_scan_interval = (UINT16) read;
        IotLogDebug("Enter le_scan_window : \n");
        scanf("%X",&read);
        le_scan_window = (UINT16) read;
        IotLogDebug("Enter initiator_filter_policy : \n");
        scanf("%X",&read);
        initiator_filter_policy = (UCHAR) read;
        IotLogDebug("Enter peer_address_type : \n");
        scanf("%X",&read);
        peer_address_type = (UCHAR) read;
        IotLogDebug("Enter peer_address : \n");
        for(index = 0; index<6; index++)
        {
            scanf("%X",&read);
            peer_address[index] = (UCHAR) read;
        }
        IotLogDebug("Enter own_address_type : \n");
        scanf("%X",&read);
        own_address_type = (UCHAR) read;
        IotLogDebug("Enter conn_interval_min : \n");
        scanf("%X",&read);
        conn_interval_min = (UINT16) read;
        IotLogDebug("Enter conn_interval_max : \n");
        scanf("%X",&read);
        conn_interval_max = (UINT16) read;
        IotLogDebug("Enter conn_latency : \n");
        scanf("%X",&read);
        conn_latency = (UINT16) read;
        IotLogDebug("Enter supervision_timeout : \n");
        scanf("%X",&read);
        supervision_timeout = (UINT16) read;
        IotLogDebug("Enter maximum_ce_length : \n");
        scanf("%X",&read);
        maximum_ce_length = (UINT16) read;
        IotLogDebug("Enter minimum_ce_length : \n");
        scanf("%X",&read);
        minimum_ce_length = (UINT16) read;
    }

    retval = BT_hci_le_create_connection
              (
                   le_scan_interval,
                   le_scan_window,
                   initiator_filter_policy,
                   peer_address_type,
                   peer_address,
                   own_address_type,
                   conn_interval_min,
                   conn_interval_max,
                   conn_latency,
                   supervision_timeout,
                   maximum_ce_length,
                   minimum_ce_length
              );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_CREATE_CONNECTION_CANCEL */
void appl_hci_le_create_connection_cancel( void )
{
    API_RESULT retval;
    retval = BT_hci_le_create_connection_cancel();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_WHITE_LIST_SIZE */
void appl_hci_le_read_white_list_size( void )
{
    API_RESULT retval;
    retval = BT_hci_le_read_white_list_size();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_CLEAR_WHITE_LIST */
void appl_hci_le_clear_white_list( void )
{
    API_RESULT retval;
    retval = BT_hci_le_clear_white_list();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_ADD_DEVICE_TO_WHITE_LIST */
void appl_hci_le_add_device_to_white_list( void )
{
    API_RESULT retval;
    UCHAR   addr[6];
    UCHAR   addr_type;
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter addr_type : \n");
    scanf("%X",&read);
    addr_type = (UCHAR) read;
    IotLogDebug("Enter addr : \n");
    for(index = 0; index<6; index++)
    {
        scanf("%X",&read);
        addr[index] = (UCHAR) read;
    }
    retval = BT_hci_le_add_device_to_white_list
                  (
                       addr_type,
                      addr
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST */
void appl_hci_le_remove_device_from_white_list( void )
{
    API_RESULT retval;
    UCHAR   addr[6];
    UCHAR   addr_type;
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter addr_type : \n");
    scanf("%X",&read);
    addr_type = (UCHAR) read;
    IotLogDebug("Enter addr : \n");
    for(index = 0; index<6; index++)
    {
        scanf("%X",&read);
        addr[index] = (UCHAR) read;
    }
    retval = BT_hci_le_remove_device_from_white_list
                  (
                       addr_type,
                      addr
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_CONNECTION_UPDATE */
void appl_hci_le_connection_update( void )
{
    API_RESULT retval;
    UINT16   conn_interval_max;
    UINT16   conn_interval_min;
    UINT16   conn_latency;
    UINT16   connection_handle;
    UINT16   maximum_ce_length;
    UINT16   minimum_ce_length;
    UINT16   supervision_timeout;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    IotLogDebug("Enter conn_interval_min : \n");
    scanf("%X",&read);
    conn_interval_min = (UINT16) read;
    IotLogDebug("Enter conn_interval_max : \n");
    scanf("%X",&read);
    conn_interval_max = (UINT16) read;
    IotLogDebug("Enter conn_latency : \n");
    scanf("%X",&read);
    conn_latency = (UINT16) read;
    IotLogDebug("Enter supervision_timeout : \n");
    scanf("%X",&read);
    supervision_timeout = (UINT16) read;
    IotLogDebug("Enter minimum_ce_length : \n");
    scanf("%X",&read);
    minimum_ce_length = (UINT16) read;
    IotLogDebug("Enter maximum_ce_length : \n");
    scanf("%X",&read);
    maximum_ce_length = (UINT16) read;
    retval = BT_hci_le_connection_update
                  (
                       connection_handle,
                       conn_interval_min,
                       conn_interval_max,
                       conn_latency,
                       supervision_timeout,
                       minimum_ce_length,
                       maximum_ce_length
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION */
void appl_hci_le_set_host_channel_classification( void )
{
    API_RESULT retval;
    UCHAR   le_channel_map[6];
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter le_channel_map : \n");
    for(index = 0; index<6; index++)
    {
        scanf("%X",&read);
        le_channel_map[index] = (UCHAR) read;
    }
    retval = BT_hci_le_set_host_channel_classification
                  (
                      le_channel_map
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_CHANNEL_MAP */
void appl_hci_le_read_channel_map( void )
{
    API_RESULT retval;
    UINT16   connection_handle;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    retval = BT_hci_le_read_channel_map
             (
                   connection_handle
             );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_REMOTE_USED_FEATURES */
void appl_hci_le_read_remote_used_features( void )
{
    API_RESULT retval;
    UINT16   connection_handle;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    retval = BT_hci_le_read_remote_used_features
                  (
                       connection_handle
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_ENCRYPT */
void appl_hci_le_encrypt( void )
{
    API_RESULT retval;
    UCHAR   key[16];
    UCHAR   plain_text[16];
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter key : \n");
    for(index = 0; index<16; index++)
    {
        scanf("%X",&read);
        key[index] = (UCHAR) read;
    }
    IotLogDebug("Enter plain_text : \n");
    for(index = 0; index<16; index++)
    {
        scanf("%X",&read);
        plain_text[index] = (UCHAR) read;
    }
    retval = BT_hci_le_encrypt
                  (
                      key,
                      plain_text,
                      NULL
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_RAND */
void appl_hci_le_rand( void )
{
    API_RESULT retval;
    retval = BT_hci_le_rand();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_START_ENCRYPTION */
void appl_hci_le_start_encryption( void )
{
    API_RESULT retval;
    UCHAR   long_term_key[16];
    UCHAR   random_number[8];
    UINT16   conn_handle;
    UINT16   encrypted_diversifier;
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter conn_handle : \n");
    scanf("%X",&read);
    conn_handle = (UINT16) read;
    IotLogDebug("Enter random_number : \n");
    for(index = 0; index<8; index++)
    {
        scanf("%X",&read);
        random_number[index] = (UCHAR) read;
    }
    IotLogDebug("Enter encrypted_diversifier : \n");
    scanf("%X",&read);
    encrypted_diversifier = (UINT16) read;
    IotLogDebug("Enter long_term_key : \n");
    for(index = 0; index<16; index++)
    {
        scanf("%X",&read);
        long_term_key[index] = (UCHAR) read;
    }
    retval = BT_hci_le_start_encryption
                  (
                       conn_handle,
                      random_number,
                       encrypted_diversifier,
                      long_term_key
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY */
void appl_hci_le_long_term_key_requested_reply( void )
{
    API_RESULT retval;
    UCHAR   long_term_key[16];
    UINT16   connection_handle;
    UINT32   index;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    IotLogDebug("Enter long_term_key : \n");
    for(index = 0; index<16; index++)
    {
        scanf("%X",&read);
        long_term_key[index] = (UCHAR) read;
    }
    retval = BT_hci_le_long_term_key_requested_reply
                  (
                       connection_handle,
                      long_term_key
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY */
void appl_hci_le_long_term_key_requested_negative_reply( void )
{
    API_RESULT retval;
    UINT16   connection_handle;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    retval = BT_hci_le_long_term_key_requested_negative_reply
                  (
                       connection_handle
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_READ_SUPPORTED_STATES */
void appl_hci_le_read_supported_states( void )
{
    API_RESULT retval;
    retval = BT_hci_le_read_supported_states();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}


/* Send HCI_READ_TRANSMIT_POWER_LEVEL */
void appl_hci_read_transmit_power_level(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR      power_type;
    UINT16     handle;

    IotLogDebug("Read Transmit Power Leve. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("[Transmit Power Level: 0x00->Current, 0x01->Maximum]");
    IotLogDebug("Power Type (in Hex) = "); fflush(stdout);

    scanf("%x", &read);
    power_type = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Reat Transmit Power Level ...\n"); fflush(stdout);
    retval = BT_hci_read_transmit_power_level (handle, power_type);
    if ( retval != API_SUCCESS )
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
    }
}

/* Send HCI_LE_RECEIVER_TEST_COMMAND */
void appl_hci_le_receiver_test_command( void )
{
    API_RESULT retval;
    UCHAR   rx_frequency;
    unsigned int read;

    IotLogDebug("Enter rx_frequency : \n");
    scanf("%X",&read);
    rx_frequency = (UCHAR) read;
    retval = BT_hci_le_receiver_test_command
                  (
                       rx_frequency
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_TRANSMITTER_TEST_COMMAND */
void appl_hci_le_transmitter_test_command( void )
{
#ifdef HCI_TESTING_COMMANDS
    API_RESULT retval;
    UCHAR   packet_payload;
    UCHAR   rx_frequency;
    UCHAR   test_data_length;
    unsigned int read;

    IotLogDebug("Enter rx_frequency : \n");
    scanf("%X",&read);
    rx_frequency = (UCHAR) read;
    IotLogDebug("Enter test_data_length : \n");
    scanf("%X",&read);
    test_data_length = (UCHAR) read;
    IotLogDebug("Enter packet_payload : \n");
    scanf("%X",&read);
    packet_payload = (UCHAR) read;
    retval = BT_hci_le_transmitter_test_command
                  (
                       rx_frequency,
                       test_data_length,
                       packet_payload
                  );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else /* HCI_TESTING_COMMANDS */
    IotLogDebug("Defien HCI_TESTING_COMMANDS for testing commands.\n");
#endif /* HCI_TESTING_COMMANDS */
}

/* Send HCI_LE_TEST_END */
void appl_hci_le_test_end( void )
{
#ifdef HCI_TESTING_COMMANDS
    API_RESULT retval;
    retval = BT_hci_le_test_end();

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else /* HCI_TESTING_COMMANDS */
    IotLogDebug("Defien HCI_TESTING_COMMANDS for testing commands.\n");
#endif /* HCI_TESTING_COMMANDS */
}

#ifdef HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
/* Remote Connection Parameter Request Reply */
void appl_hci_le_remote_conn_param_req_reply(void)
{
    API_RESULT retval;
    UINT16   conn_interval_max;
    UINT16   conn_interval_min;
    UINT16   conn_latency;
    UINT16   connection_handle;
    UINT16   maximum_ce_length;
    UINT16   minimum_ce_length;
    UINT16   supervision_timeout;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;
    IotLogDebug("Enter conn_interval_min : \n");
    scanf("%X",&read);
    conn_interval_min = (UINT16) read;
    IotLogDebug("Enter conn_interval_max : \n");
    scanf("%X",&read);
    conn_interval_max = (UINT16) read;
    IotLogDebug("Enter conn_latency : \n");
    scanf("%X",&read);
    conn_latency = (UINT16) read;
    IotLogDebug("Enter supervision_timeout : \n");
    scanf("%X",&read);
    supervision_timeout = (UINT16) read;
    IotLogDebug("Enter minimum_ce_length : \n");
    scanf("%X",&read);
    minimum_ce_length = (UINT16) read;
    IotLogDebug("Enter maximum_ce_length : \n");
    scanf("%X",&read);
    maximum_ce_length = (UINT16) read;
    retval = BT_hci_le_remote_conn_param_req_reply
             (
                  connection_handle,
                  conn_interval_min,
                  conn_interval_max,
                  conn_latency,
                  supervision_timeout,
                  minimum_ce_length,
                  maximum_ce_length
             );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Remote Connection Parameter Request Negative Reply */
void appl_hci_le_remote_conn_param_req_negtive_reply(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR      reason;
    UINT16     handle;

    IotLogDebug("Remote Connection Parameter Request Negative Reply. Enter Required Information ...\n");

    IotLogDebug("LE Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("Reason for Reject (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    reason = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Remote Connection Parameter Request Negative Reply ...\n"); fflush(stdout);
    retval = BT_hci_le_remote_conn_param_req_negtive_reply ( handle, reason );
    if ( retval != API_SUCCESS )
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Remote Connection Parameter Request Negative Reply Complete.\n");
    }
}
#endif /* HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT */

/* Read Authenticated Payload Timeout */
#ifdef HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
void appl_hci_read_authenticated_payload_timeout(void)
{
    API_RESULT retval;
    UINT16   connection_handle;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X", &read);
    connection_handle = (UINT16)read;
    retval = BT_hci_read_authenticated_payload_timeout
        (
        connection_handle
        );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}
#endif /* HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

/* Write Authenticated Payload Timeout */
#ifdef HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
void appl_hci_write_authenticated_payload_timeout(void)
{
    API_RESULT retval;
    UINT16   connection_handle, authenticated_payload_timeout;
    unsigned int read;

    IotLogDebug("Enter connection_handle : \n");
    scanf("%X", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter Autheticated Payload Timeout : \n");
    scanf("%X", &read);
    authenticated_payload_timeout = (UINT16)read;

    retval = BT_hci_write_authenticated_payload_timeout
        (
        connection_handle,
        authenticated_payload_timeout
        );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}
#endif /* HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

void appl_le_send_data(void)
{
    API_RESULT retval;
    BT_DEVICE_ADDR   bd_addr;
    DEVICE_HANDLE    handle;
    UCHAR   data[100];
    unsigned int read;
    UINT16  len;
    UINT16  lcid;
    UINT32  index;

    IotLogDebug("Enter Bluetooth address : \n");
    for(index = 0; index<BT_BD_ADDR_SIZE; index++)
    {
        scanf("%X",&read);
        BT_BD_ADDR(&bd_addr)[index] = (UCHAR) read;
    }

    IotLogDebug("Enter bd_addr_type ");
    scanf("%X", &read);
    BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read;

    IotLogDebug("On which lcid? ");
    scanf("%X",&read);
    lcid = (UINT16) read;

    IotLogDebug("Enter data length : ");
    scanf("%X",&read);
    len = (UINT16) read;

    IotLogDebug("Enter data : \n");
    for(index = 0; index<len; index++)
    {
        scanf("%X",&read);
        data[index + L2CAP_HDR_LEN] = (UCHAR) read;
    }
    len += L2CAP_HDR_LEN;

    retval = device_queue_search_le_remote_addr (&handle,&bd_addr);

    retval = l2cap_fixed_channel_data_write
             (
                  &handle,
                  lcid,
                  data,
                  len
             );

    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* BT 4.2 related HCI LE Commands */
#ifdef BT_4_2

void appl_hci_le_set_data_length(void)
{
#ifdef HCI_LE_SET_DATA_LENGTH_SUPPORT
    UINT16 connection_handle;
    UINT16 tx_octets;
    UINT16 tx_time;
    unsigned int read;
    API_RESULT retval;

    IotLogDebug("Enter connection_handle(in Hex): \n");
    scanf("%X",&read);
    connection_handle = (UINT16) read;

    IotLogDebug("Enter tx octets(in Hex): \n");
    scanf("%X",&read);
    tx_octets = (UINT16) read;

    IotLogDebug("Enter tx time(in Hex): \n");
    scanf("%X",&read);
    tx_time = (UINT16) read;

    retval = BT_hci_le_set_data_length
             (
                connection_handle,
                tx_octets,
                tx_time
             );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_SET_DATA_LENGTH_SUPPORT feature flag\n");
#endif /* HCI_LE_SET_DATA_LENGTH_SUPPORT */

    return;
}

void appl_hci_le_read_suggested_default_data_length(void)
{
#ifdef HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT
    API_RESULT retval;

    retval = BT_hci_le_read_suggested_default_data_length();
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT feature flag\n");
#endif /* HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT */

    return;
}

void appl_hci_le_write_suggested_default_data_length(void)
{
#ifdef HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT
    API_RESULT retval;
    UINT16 tx_octets;
    UINT16 tx_time;
    unsigned int read;

    IotLogDebug("Enter tx octets(in Hex): \n");
    scanf("%X",&read);
    tx_octets = (UINT16) read;

    IotLogDebug("Enter tx time(in Hex): \n");
    scanf("%X",&read);
    tx_time = (UINT16) read;

    retval = BT_hci_le_write_suggested_default_data_length
            (
                tx_octets,
                tx_time
            );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT feature flag\n");
#endif /* HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT */

    return;
}

void appl_hci_le_read_local_p256_public_key(void)
{
#ifdef HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT
    API_RESULT retval;

    retval = BT_hci_le_read_local_p256_public_key();
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT is not defined\n");
#endif /* HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT */

    return;
}

void appl_hci_le_generate_dhkey(void)
{
#ifdef HCI_LE_GENERATE_DHKEY_SUPPORT
    API_RESULT retval;
    UCHAR remote_public_key[64];
    UCHAR index;
    unsigned int read;

    IotLogDebug("Enter remote public key: \n");
    for(index = 0; index < 64; index++)
    {
        scanf("%X",&read);
        remote_public_key[index] = (UCHAR) read;
    }

    retval = BT_hci_le_generate_dhkey(remote_public_key);
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("HCI_LE_GENERATE_DHKEY_SUPPORT is not defined\n");
#endif /* HCI_LE_GENERATE_DHKEY_SUPPORT */

    return;
}

void appl_hci_le_add_device_to_resolving_list(void)
{
#ifdef HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT
    API_RESULT retval;
    UCHAR peer_id_addr_type;
    UCHAR peer_id_addr[6];
    UCHAR peer_irk[16];
    UCHAR local_irk[16];
    UCHAR index;
    unsigned int read;

    IotLogDebug("Enter peer addr type: \n");
    scanf("%d",&read);
    peer_id_addr_type = (UCHAR )read;

    IotLogDebug("Enter peer identity addr: \n");
    for(index = 0; index < 6; index++)
    {
        scanf("%X",&read);
        peer_id_addr[index] = (UCHAR) read;
    }

    IotLogDebug("Enter peer IRK: \n");
    for(index = 0; index < 16; index++)
    {
        scanf("%X",&read);
        peer_irk[index] = (UCHAR) read;
    }

    IotLogDebug("Enter local IRK: \n");
    for(index = 0; index < 16; index++)
    {
        scanf("%X",&read);
        local_irk[index] = (UCHAR) read;
    }

    retval = BT_hci_le_add_device_to_resolving_list
                (
                    peer_id_addr_type,
                    peer_id_addr,
                    peer_irk,
                    local_irk
                );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT feature flag\n");
#endif /* HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT */

    return;
}

void appl_hci_le_remove_device_from_resolving_list(void)
{
#ifdef HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT
    API_RESULT retval;
    UCHAR peer_id_addr_type;
    UCHAR peer_id_addr[6];
    UCHAR index;
    unsigned int read;

    IotLogDebug("Enter peer addr type: \n");
    scanf("%d",&read);
    peer_id_addr_type = (UCHAR )read;

    IotLogDebug("Enter peer identity addr: \n");
    for(index = 0; index < 6; index++)
    {
        scanf("%X",&read);
        peer_id_addr[index] = (UCHAR) read;
    }

    retval = BT_hci_le_remove_device_from_resolving_list
                (
                    peer_id_addr_type,
                    peer_id_addr
                );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT feature flag\n");
#endif /* HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT */

    return;
}

void appl_hci_le_clear_resolving_list(void)
{
#ifdef HCI_LE_CLR_RESOLVING_LIST_SUPPORT
    API_RESULT retval;

    retval = BT_hci_le_clear_resolving_list();
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_CLR_RESOLVING_LIST_SUPPORT feature flag\n");
#endif /* HCI_LE_CLR_RESOLVING_LIST_SUPPORT */

    return;
}

void appl_hci_le_read_resolving_list_size(void)
{
#ifdef HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT
    API_RESULT retval;

    retval = BT_hci_le_read_resolving_list_size();
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Defined HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT feature flag\n");
#endif /* HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT */

    return;
}

void appl_hci_le_read_peer_resolvable_address(void)
{
#ifdef HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT
    API_RESULT retval;
    UCHAR peer_id_addr_type;
    UCHAR peer_id_addr[6];
    UCHAR index;
    unsigned int read;

    IotLogDebug("Enter peer addr type: \n");
    scanf("%d",&read);
    peer_id_addr_type = (UCHAR )read;

    IotLogDebug("Enter peer identity addr: \n");
    for(index = 0; index < 6; index++)
    {
        scanf("%X",&read);
        peer_id_addr[index] = (UCHAR) read;
    }

    retval = BT_hci_le_read_peer_resolvable_address
                (
                    peer_id_addr_type,
                    peer_id_addr
                );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT feature flag\n");
#endif /* HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT */

    return;
}

void appl_hci_le_read_local_resolvable_address(void)
{
#ifdef HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT
    API_RESULT retval;
    UCHAR peer_id_addr_type;
    UCHAR peer_id_addr[6];
    UCHAR index;
    unsigned int read;

    IotLogDebug("Enter peer addr type: \n");
    scanf("%d",&read);
    peer_id_addr_type = (UCHAR )read;

    IotLogDebug("Enter peer identity addr: \n");
    for(index = 0; index < 6; index++)
    {
        scanf("%X",&read);
        peer_id_addr[index] = (UCHAR) read;
    }

    retval = BT_hci_le_read_local_resolvable_address
                (
                    peer_id_addr_type,
                    peer_id_addr
                );
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT feature flag\n");
#endif /* HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT */

    return;
}

void appl_hci_le_set_address_resolution_enable(void)
{
#ifdef HCI_LE_SET_ADDR_RESOLUTION_ENABLE_SUPPORT
    API_RESULT retval;
    UCHAR  addr_resolution_enable;
    unsigned int read;

    IotLogDebug("Enter addr resolution enable: \n");
    IotLogDebug("    0 -> Disable\n");
    IotLogDebug("    1 -> Enable\n");
    scanf("%X",&read);
    addr_resolution_enable = (UCHAR) read;

    retval = BT_hci_le_set_address_resolution_enable(addr_resolution_enable);
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#endif /* HCI_LE_SET_ADDR_RESOLUTION_ENABLE_SUPPORT */

    return;
}

void appl_hci_le_set_resolvable_private_address_timeout(void)
{
#ifdef HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT
    UINT16  rpa_timeout;
    API_RESULT retval;
    unsigned int read;

    IotLogDebug("Enter RPA timeout(in Hex): \n");
    scanf("%X", &read);
    rpa_timeout = (UINT16) read;

    retval = BT_hci_le_set_resolvable_private_address_timeout (rpa_timeout);
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT feature flag\n");
#endif /* HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT */

    return;
}

void appl_hci_le_read_maximum_data_length(void)
{
#ifdef HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT
    API_RESULT retval;

    retval = BT_hci_le_read_maximum_data_length();
    if(API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
#else
    IotLogDebug("Define HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT feature flag\n");
#endif /* HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT */
    return;
}
#endif /* BT_4_2 */

#ifdef BT_5_0
/* Send HCI_LE_READ_PHY */
void appl_hci_le_read_phy(void)
{
    API_RESULT    retval;
    unsigned int  read;
    UINT16        handle;

    IotLogDebug("ACL Connection Handle (in Hex) = \n");
    scanf("%x", &read);
    handle = (UINT16)read;

    retval = BT_hci_read_phy(handle);

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_DEFAULT_PHY */
void appl_hci_le_set_default_phy(void)
{
    API_RESULT   retval;
    UCHAR        allphy;
    UCHAR        txphy;
    UCHAR        rxphy;
    unsigned int read;

    IotLogDebug("ALL Phy (in Hex) = \n");
    scanf("%x", &read);
    allphy = (UCHAR)read;

    IotLogDebug("TX Phy (in Hex) = \n");
    scanf("%x", &read);
    txphy = (UCHAR)read;

    IotLogDebug("RX Phy (in Hex) = \n");
    scanf("%x", &read);
    rxphy = (UCHAR)read;

    retval = BT_hci_le_set_default_phy(allphy, txphy, rxphy);

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

/* Send HCI_LE_SET_PHY */
void appl_hci_le_set_phy(void)
{
    API_RESULT   retval;
    UINT16       handle;
    UCHAR        allphy;
    UCHAR        txphy;
    UCHAR        rxphy;
    UINT16       phy_options;
    unsigned int read;

    IotLogDebug("Connection handle (in Hex) = \n");
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("ALL Phy (in Hex) = \n");
    scanf("%x", &read);
    allphy = (UCHAR)read;

    IotLogDebug("TX Phy (in Hex) = \n");
    scanf("%x", &read);
    txphy = (UCHAR)read;

    IotLogDebug("RX Phy (in Hex) = \n");
    scanf("%x", &read);
    rxphy = (UCHAR)read;

    IotLogDebug("PHY_options (in Hex) = \n");
    scanf("%x", &read);
    phy_options = (UINT16)read;

    retval = BT_hci_le_set_phy
             (
                 handle,
                 allphy,
                 txphy,
                 rxphy,
                 phy_options
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_extended_advertising_params(void)
{
    int     read;
    UCHAR   adv_handle;
    UINT32  pri_adv_intval_min;
    UINT32  pri_adv_intval_max;
    UCHAR   channel_map;
    UCHAR   own_addr_type;
    UINT32  index;
    UCHAR   peer_addr_type;
    UCHAR   peer_addr[BT_BD_ADDR_SIZE];
    UCHAR   adv_filter_policy;
    CHAR    adv_tx_power;
    UCHAR   pri_adv_phy;
    UCHAR   sec_adv_max_skip;
    UCHAR   sec_adv_phy;
    UCHAR   adv_sid;
    UCHAR   scan_rsp_ntf_enable;
    UCHAR   adv_event_prty;
    API_RESULT retval;

    IotLogDebug("Enter adv_handle : \n");
    scanf("%x", &read);
    adv_handle = (UCHAR)read;

    IotLogDebug("Enter adv_event_prty: \n");
    scanf("%x", &read);
    adv_event_prty = (UCHAR)read;

    IotLogDebug("Enter Primary_Advertising_Interval_Min: \n");
    scanf("%x", &read);
    pri_adv_intval_min = (UINT32)read;

    IotLogDebug("Enter Primary_Advertising_Interval_Max : \n");
    scanf("%x", &read);
    pri_adv_intval_max = (UINT32)read;

    IotLogDebug("Enter channel_map : \n");
    scanf("%x", &read);
    channel_map = (UCHAR)read;

    IotLogDebug("Enter own_addr_type : \n");
    scanf("%x", &read);
    own_addr_type = (UCHAR)read;

    IotLogDebug("Enter peer address type : \n");
    scanf("%x", &read);
    peer_addr_type = (UCHAR)read;

    IotLogDebug("Enter peer address : \n");
    for (index = 0; index < 6; index++)
    {
        scanf("%x", &read);
        peer_addr[index] = (UCHAR)read;
    }

    IotLogDebug("Enter advertising_filter_policy : \n");
    scanf("%x", &read);
    adv_filter_policy = (UCHAR)read;

    IotLogDebug("Enter adv_tx_power [in Decimal] : \n");
    scanf("%d", &read);
    adv_tx_power = (CHAR)read;

    IotLogDebug("Enter Primary_Advertising_PHY : \n");
    scanf("%x", &read);
    pri_adv_phy = (UCHAR)read;

    IotLogDebug("Enter sec_adv_max_skip : \n");
    scanf("%x", &read);
    sec_adv_max_skip = (UCHAR)read;

    IotLogDebug("Enter Secondary_Advertising_PHY : \n");
    scanf("%x", &read);
    sec_adv_phy = (UCHAR)read;

    IotLogDebug("Enter Advertising_SID [In HEX] : \n");
    scanf("%x", &read);
    adv_sid = (UCHAR)read;

    IotLogDebug("Enter Scan_Request_Notification_Enable: \n");
    scanf("%x", &read);
    scan_rsp_ntf_enable = (UCHAR)read;

    retval = BT_hci_le_set_extended_advertising_parameters
             (
                 adv_handle,
                 adv_event_prty,
                 pri_adv_intval_min,
                 pri_adv_intval_max,
                 channel_map,
                 own_addr_type,
                 peer_addr_type,
                 peer_addr,
                 adv_filter_policy,
                 adv_tx_power,
                 pri_adv_phy,
                 sec_adv_max_skip,
                 sec_adv_phy,
                 adv_sid,
                 scan_rsp_ntf_enable
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_periodic_adv_params(void)
{
    int     read;
    UCHAR   periodic_adv_hndl;
    UINT16  periodic_adv_intval_min;
    UINT16  periodic_adv_intval_max;
    UINT16  periodic_adv_prtys;
    API_RESULT retval;

    IotLogDebug("Enter advertising Handle : \n");
    scanf("%x", &read);
    periodic_adv_hndl = (UCHAR)read;

    IotLogDebug("Enter Periodic Avertising interval Minimum : \n");
    scanf("%x", &read);
    periodic_adv_intval_min = (UINT16)read;

    IotLogDebug("Enter Periodic Avertising interval Maximum : \n");
    scanf("%x", &read);
    periodic_adv_intval_max = (UINT16)read;

    IotLogDebug("Enter Periodic Advertising Properties : \n");
    scanf("%x", &read);
    periodic_adv_prtys = (UINT16)read;

    retval = BT_hci_le_set_periodic_advertising_parameters
             (
                 periodic_adv_hndl,
                 periodic_adv_intval_min,
                 periodic_adv_intval_max,
                 periodic_adv_prtys
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_extended_advertising_data(void)
{
    unsigned int  read;
    UCHAR   adv_handle;
    UCHAR   optns;
    UINT32  index;
    UCHAR   frgment_pref;
    UCHAR   adv_data_len;
    UCHAR   data[MAX_EXTENDED_ADVERTISING_DATA_SIZE];

    API_RESULT retval;

    IotLogDebug("Enter Advertising Handle : \n");
    scanf("%x", &read);
    adv_handle = (UCHAR)read;

    IotLogDebug("Enter Operations: \n");
    scanf("%x", &read);
    optns = (UCHAR)read;

    IotLogDebug("Fragment_Preference: \n");
    scanf("%x", &read);
    frgment_pref = (UCHAR)read;

    IotLogDebug("Enter Extended Advertertising data length in[DECIMAL]: \n");
    scanf("%d", &read);
    adv_data_len = (UCHAR)read;

    /* Initialize the Advertising Response Data field */
    BT_mem_set
    (
        &data[0],
        0x00,
        MAX_EXTENDED_ADVERTISING_DATA_SIZE
    );

    IotLogDebug("Enter Extended Advertertising data: \n");
    for (index = 0; index < adv_data_len; index++)
    {
        scanf("%x", &read);
        data[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_extended_advertising_data
             (
                 adv_handle,
                 optns,
                 frgment_pref,
                 adv_data_len,
                 data
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_extended_scan_response_data(void)
{
    unsigned int  read;
    UCHAR   adv_handle;
    UCHAR   optns;
    UCHAR   frag_pref;
    UINT32  index;
    UCHAR   scan_rsp_data_len;
    UCHAR   scan_rsp_data[MAX_SCAN_RSP_DATA_SIZE];
    API_RESULT retval;

    IotLogDebug("Enter advertising_handle: \n");
    scanf("%x", &read);
    adv_handle = (UCHAR)read;

    IotLogDebug("Enter Operations: \n");
    scanf("%x", &read);
    optns = (UCHAR)read;

    IotLogDebug("Enter fragment Preference: \n");
    scanf("%x", &read);
    frag_pref = (UCHAR)read;

    IotLogDebug("Enter Scan Response data length [in Decimal]: \n");
    scanf("%d", &read);
    scan_rsp_data_len = (UCHAR)read;

    IotLogDebug("Enter Extended Scan Response data: \n");
    for (index = 0; index < scan_rsp_data_len; index++)
    {
        scanf("%x", &read);
        scan_rsp_data[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_extended_scan_response_data
             (
                 adv_handle,
                 optns,
                 frag_pref,
                 scan_rsp_data_len,
                 scan_rsp_data
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_extended_advertise_enable(void)
{
    unsigned int  read;
    UCHAR   enable;
    UCHAR   no_of_sets;
    UCHAR   adv_handle[APPL_MAX_NO_OF_SETS];
    UINT16  duration[APPL_MAX_NO_OF_SETS];
    UCHAR   max_ext_adv_events[APPL_MAX_NO_OF_SETS];
    UCHAR   i;
    API_RESULT retval;

    IotLogDebug("Enter enable : \n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    IotLogDebug("Enter number of sets : \n");
    scanf("%x", &read);
    no_of_sets = (UCHAR)read;

    for (i = 0; i < no_of_sets; i++)
    {
        IotLogDebug("Enter advertising handle for %dth\n", i);
        scanf("%x", &read);
        adv_handle[i] = (UCHAR)read;

        IotLogDebug("Enter duration for %dth\n", i);
        scanf("%x", &read);
        duration[i] = (UINT16)read;

        IotLogDebug("Enter Max Extended Advertising Event for %dth\n", i);
        scanf("%x", &read);
        max_ext_adv_events[i] = (UCHAR)read;
    }

    retval = BT_hci_le_set_extended_advertising_enable
             (
                 enable,
                 no_of_sets,
                 adv_handle,
                 duration,
                 max_ext_adv_events
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_read_max_advertising_data_length(void)
{
    IotLogDebug("\nReading the maximum advertising data length\n\n");
    BT_hci_le_read_maximum_advertising_data_length();
}

void appl_hci_le_read_num_supported_advertising_sets(void)
{
    IotLogDebug("\nReading the number of supported advertising sets\n\n");
    BT_hci_le_read_num_supported_advertising_sets();
}

void appl_hci_le_read_transmit_power(void)
{
    IotLogDebug("\nReading the advertising transmit power\n\n");
    BT_hci_le_read_transmit_power();
}

void appl_hci_le_remove_advertising_set(void)
{
    unsigned int  read;
    UCHAR   adv_handle;

    IotLogDebug("Enter advertising Handle : \n");
    scanf("%x", &read);
    adv_handle = (UCHAR)read;

    IotLogDebug("\nRemoving the advertising set\n\n");
    BT_hci_le_remove_advertising_set(adv_handle);
}

void appl_hci_le_clear_advertising_sets(void)
{
    IotLogDebug("\nClearing the advertising sets\n\n");
    BT_hci_le_clear_advertising_sets();
}

void appl_hci_le_set_extended_scan_parameters(void)
{
    unsigned int  read;
    UCHAR   own_addr_type;
    UCHAR   scanning_filr_policy;
    UCHAR   scan_phy;

    UCHAR   scan_type[APPL_MAX_NUM_OF_SCAN_PHYS];
    UINT16  scan_intval[APPL_MAX_NUM_OF_SCAN_PHYS];
    UINT16  scan_window[APPL_MAX_NUM_OF_SCAN_PHYS];
    API_RESULT retval;
    UCHAR   index;

    IotLogDebug("Enter own_address_type: \n");
    scanf("%x", &read);
    own_addr_type = (UCHAR)read;

    IotLogDebug("Enter scanning_filter_policy: \n");
    scanf("%x", &read);
    scanning_filr_policy = (UCHAR)read;

    IotLogDebug("Enter scanning_phy : \n");
    IotLogDebug("0x01 for LE 1M PHY\n");
    IotLogDebug("0x04 for LE Coded PHY\n");
    IotLogDebug("0x05 for both 1M and LE Coded PHY\n");
    scanf("%x", &read);
    scan_phy = (UCHAR)read;

    index = 0;
    if (HCI_LE_SET_EXT_SCAN_PARAM_1M_PHY_MASK & scan_phy)
    {
        IotLogDebug("For LE 1M PHY: \n");

        IotLogDebug("Enter le_scan_type: \n");
        scanf("%x", &read);
        scan_type[index] = (UCHAR)read;

        IotLogDebug("Enter le_scan_interval: \n");
        scanf("%x", &read);
        scan_intval[index] = (UINT16)read;

        IotLogDebug("Enter le_scan_window: \n");
        scanf("%x", &read);
        scan_window[index] = (UINT16)read;

        index += 1;
    }

    if (HCI_LE_SET_EXT_SCAN_PARAM_CODED_PHY_MASK & scan_phy)
    {
        IotLogDebug("For LE Coded PHY: \n");

        IotLogDebug("Enter le_scan_type: \n");
        scanf("%x", &read);
        scan_type[index] = (UCHAR)read;

        IotLogDebug("Enter le_scan_interval: \n");
        scanf("%x", &read);
        scan_intval[index] = (UINT16)read;

        IotLogDebug("Enter le_scan_window: \n");
        scanf("%x", &read);
        scan_window[index] = (UINT16)read;
    }

    retval = BT_hci_le_set_extended_scan_parameters
             (
                 own_addr_type,
                 scanning_filr_policy,
                 scan_phy,
                 scan_type,
                 scan_intval,
                 scan_window
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_extended_scan_enable(void)
{
    UINT32  read;
    UCHAR   enable;
    UCHAR   fter_duplicates;
    UINT16  durn;
    UINT16  period;
    API_RESULT retval;

    IotLogDebug("Enter enable : \n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    IotLogDebug("Enter filter_duplicates : \n");
    scanf("%x", &read);
    fter_duplicates = (UCHAR)read;

    IotLogDebug("Enter duration : \n");
    scanf("%x", &read);
    durn = (UINT16)read;

    IotLogDebug("Enter Period : \n");
    scanf("%x", &read);
    period = (UINT16)read;

    retval = BT_hci_le_set_extended_scan_enable
             (
                 enable,
                 fter_duplicates,
                 durn,
                 period
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_periodic_adv_create_sync(void)
{
    unsigned int  read;
    UCHAR   filtr_policy;
    UCHAR   adv_sid;
    UCHAR   addr_type;
    UINT32  index;
    UCHAR   addr[BT_BD_ADDR_SIZE];
    UINT16  skip;
    UINT16  sync_timeout;
    UCHAR   unused;
    API_RESULT retval;

    IotLogDebug("Enter filter_policy: \n");
    scanf("%x", &read);
    filtr_policy = (UCHAR)read;

    IotLogDebug("Enter Advertising_SID: \n");
    scanf("%x", &read);
    adv_sid = (UCHAR)read;

    IotLogDebug("Enter Peer address_type: \n");
    scanf("%x", &read);
    addr_type = (UCHAR)read;

    IotLogDebug("Enter Peer address : \n");
    for (index = 0; index<6; index++)
    {
        scanf("%x", &read);
        addr[index] = (UCHAR)read;
    }

    IotLogDebug("Enter skip: \n");
    scanf("%x", &read);
    skip = (UINT16)read;

    IotLogDebug("Enter sync_timeout: \n");
    scanf("%x", &read);
    sync_timeout = (UINT16)read;

    unused = 0x00;

    retval = BT_hci_le_periodic_adv_create_sync
             (
                 filtr_policy,
                 adv_sid,
                 addr_type,
                 addr,
                 skip,
                 sync_timeout,
                 unused
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_add_dev_to_periodic_advertiser_list(void)
{
    unsigned int  read;
    UCHAR   addr_type;
    UINT32  index;
    UCHAR   addr[BT_BD_ADDR_SIZE];
    UCHAR   adv_sid;
    API_RESULT retval;

    IotLogDebug("Enter Advertiser_Address_Type: \n");
    scanf("%x", &read);
    addr_type = (UCHAR)read;

    IotLogDebug("Enter Advertiser_Address : \n");
    for (index = 0; index<6; index++)
    {
        scanf("%x", &read);
        addr[index] = (UCHAR)read;
    }

    IotLogDebug("Enter Advertising_SID: \n");
    scanf("%x", &read);
    adv_sid = (UCHAR)read;

    retval = BT_hci_le_add_device_to_periodic_advertiser_list
             (
                 addr_type,
                 addr,
                 adv_sid
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_rm_dev_from_periodic_advertiser_list(void)
{
    unsigned int  read;
    UCHAR   addr_type;
    UINT32  index;
    UCHAR   addr[BT_BD_ADDR_SIZE];
    UCHAR   adv_sid;
    API_RESULT retval;

    IotLogDebug("Enter Advertiser_Address_Type: \n");
    scanf("%x", &read);
    addr_type = (UCHAR)read;

    IotLogDebug("Enter Advertiser_Address : \n");
    for (index = 0; index<6; index++)
    {
        scanf("%x", &read);
        addr[index] = (UCHAR)read;
    }

    IotLogDebug("Enter Advertising_SID: \n");
    scanf("%x", &read);
    adv_sid = (UCHAR)read;

    retval = BT_hci_le_remove_device_from_periodic_advertiser_list
             (
                 addr_type,
                 addr,
                 adv_sid
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

}

void appl_hci_le_clear_periodic_advertiser_list(void)
{
    IotLogDebug("\nClearning the periodic advertising list\n\n");
    BT_hci_le_clear_periodic_advertiser_list();
}

void appl_hci_le_read_periodic_advertiser_list_size(void)
{
    IotLogDebug("\nReading the periodic advertising list size\n\n");
    BT_hci_le_read_periodic_advertiser_list_size();
}

void appl_hci_le_write_rf_path_compensation(void)
{
    unsigned int  read;
    UINT16        rf_tx_path_compn_val;
    UINT16        rf_rx_path_compn_val;
    API_RESULT    retval;

    IotLogDebug("Enter rf_tx_path_compn_val : \n");
    scanf("%x", &read);
    rf_tx_path_compn_val = (UINT16)read;

    IotLogDebug("Enter rf_rx_path_compn_val : \n");
    scanf("%x", &read);
    rf_rx_path_compn_val = (UINT16)read;

    retval = BT_hci_le_write_rf_path_compensation
             (
                 rf_tx_path_compn_val,
                 rf_rx_path_compn_val
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_read_rf_path_compensation(void)
{
    IotLogDebug("\nReading the RF Path Compensation\n");
    BT_hci_le_read_rf_path_compensation();
}

void appl_hci_le_set_advertising_random_address(void)
{
    unsigned int  read;
    UCHAR   adv_handle;
    UINT32  index;
    UCHAR   random_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    IotLogDebug("Enter advertising_handle in[HEX]: \n");
    scanf("%x", &read);
    adv_handle = (UCHAR)read;

    IotLogDebug("Enter random_address in[HEX]: \n");
    for (index = 0; index < 6; index++)
    {
        scanf("%x", &read);
        random_addr[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_adv_set_random_address
             (
                 adv_handle,
                 random_addr
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_periodic_advertising_data(void)
{
    unsigned int  read;
    UCHAR   adv_hndl;
    UCHAR   oprtn;
    UCHAR   adv_data_len;
    UCHAR   adv_data[MAX_EXTENDED_ADVERTISING_DATA_SIZE];
    UINT32  index;
    API_RESULT retval;

    IotLogDebug("Enter Advertising handle in[HEX]: \n");
    scanf("%x", &read);
    adv_hndl = (UCHAR)read;

    IotLogDebug("Enter Operation in[HEX]: \n");
    scanf("%x", &read);
    oprtn = (UCHAR)read;

    IotLogDebug("Enter adv_data_len in[DECIMAL]: \n");
    scanf("%d", &read);
    adv_data_len = (UCHAR)read;

    IotLogDebug("Enter periodic advertising data in[HEX]: \n");
    for (index = 0; index < adv_data_len; index++)
    {
        scanf("%x", &read);
        adv_data[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_periodic_advertising_data
             (
                 adv_hndl,
                 oprtn,
                 adv_data_len,
                 adv_data
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_periodic_advertising_enable(void)
{
    unsigned int  read;
    UCHAR         adv_hndl;
    UCHAR         enable;
    API_RESULT    retval;

    IotLogDebug("Enter Enable : \n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    IotLogDebug("Enter Advertising_Handle: \n");
    scanf("%x", &read);
    adv_hndl = (UCHAR)read;

    retval = BT_hci_le_set_periodic_advertising_enable
             (
                 enable,
                 adv_hndl
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_periodic_advertising_create_sync_cancel(void)
{
    IotLogDebug("\nSending Periodic Advertising Receive Cancel Command\n");
    BT_hci_le_periodic_advertising_create_sync_cancel();
}

void appl_hci_le_periodic_adv_terminate_sync(void)
{
    unsigned int  read;
    UINT16        sync_hndl;
    API_RESULT    retval;

    IotLogDebug("Enter Sync_Handle : \n");
    scanf("%x", &read);
    sync_hndl = (UINT16)read;

    retval = BT_hci_le_periodic_adv_terminate_sync
    (
        sync_hndl
    );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_extended_create_connection(void)
{
    unsigned int  read;
    UINT32  i;

    UCHAR   init_filr_policy;
    UCHAR   own_addr_type;
    UCHAR   peer_addr_type;
    UCHAR   peer_addr[BT_BD_ADDR_SIZE];
    UCHAR   init_phy;

    UINT16  scan_intval[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  scan_window[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  conn_interval_min[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  conn_interval_max[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  conn_latency[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  supn_timeout[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  min_ce_len[APPL_MAX_NUM_OF_INIT_PHYS];
    UINT16  max_ce_len[APPL_MAX_NUM_OF_INIT_PHYS];
    API_RESULT retval;

    IotLogDebug("Enter initiator_filter_policy: \n");
    scanf("%x", &read);
    init_filr_policy = (UCHAR)read;

    IotLogDebug("Enter own_address_type: \n");
    scanf("%x", &read);
    own_addr_type = (UCHAR)read;

    IotLogDebug("Enter peer address type: \n");
    scanf("%x", &read);
    peer_addr_type = (UCHAR)read;

    IotLogDebug("Enter peer_address: \n");
    for (i = 0; i<6; i++)
    {
        scanf("%x", &read);
        peer_addr[i] = (UCHAR)read;
    }

    /**
    * Initiating Phy values are
    * 0x01: controller needs to scan the connectable extn adv packets coming
    * on primary channel using 1M phy and use 1M connection parameter for
    * connection
    *
    * 0x03: controller needs to scan the connectable extn adv packets coming
    * on primary channel using only 1M phy and use either 2M or 1M Phy
    * connection parameters for connection
    *
    * 0x04: controller needs to scan the connectable extn adv packets coming
    * on primary channel using LE coded phy and use LE coded connection parameters
    * connection
    *
    * 0x05: controller needs to scan the connectable extn adv packets coming
    * on primary channel using either 1M phy or LE coded phy and use either 1M or
    * LE coded connection parameters for connection
    *
    * 0x06: controller needs to scan the connectable extn adv packets coming
    * on primary channel using only LE coded phy and use either 2M or LE coded
    * connection parameters for connection
    *
    * 0x07: controller needs to scan the connectable extn adv packets coming
    * on primary channel using either 1M phy or LE coded phy's and use either 1M
    * or 2M Phy or LE coded connection parameters for connection
    *
    * Did not added '0x02 for 2M PHY' option for Initiating
    * phy parameter because 0x02 value for initiating phy parameter in
    * HCI_le_extended_create_connection command indicates to the controller
    * that, host is provided only the connection parameters for 2M PHY
    * controller will not scan for any adv packets for hci connection
    */

    IotLogDebug("Enter Initiating PHY's : \n");
    IotLogDebug("0x01 for LE 1M PHY\n");
    IotLogDebug("0x03 for both 2M and 1M PHY\n");
    IotLogDebug("0x04 for LE Coded PHY\n");
    IotLogDebug("0x05 for both 1M and LE Coded PHY\n");
    IotLogDebug("0x06 for both 2M and LE Coded PHY\n");
    IotLogDebug("0x07 for 1M and 2M and LE Coded PHY\n");
    scanf("%x", &read);
    init_phy = (UCHAR)read;

    i = 0;
    if (HCI_LE_SET_EXT_SCAN_PARAM_1M_PHY_MASK & init_phy)
    {
        IotLogDebug("For LE 1M PHY: \n");
        IotLogDebug("Enter scan_interval\n");
        scanf("%x", &read);
        scan_intval[i] = (UINT16)read;

        IotLogDebug("Enter scan_window\n");
        scanf("%x", &read);
        scan_window[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_min: \n");
        scanf("%x", &read);
        conn_interval_min[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_max: \n");
        scanf("%x", &read);
        conn_interval_max[i] = (UINT16)read;

        IotLogDebug("Enter conn_latency: \n");
        scanf("%x", &read);
        conn_latency[i] = (UINT16)read;

        IotLogDebug("Enter supn_timeout: \n");
        scanf("%x", &read);
        supn_timeout[i] = (UINT16)read;

        IotLogDebug("Enter min_ce_len: \n");
        scanf("%x", &read);
        min_ce_len[i] = (UINT16)read;

        IotLogDebug("Enter max_ce_len: \n");
        scanf("%x", &read);
        max_ce_len[i] = (UINT16)read;

        i += 1;
    }

    if (HCI_LE_SET_EXT_SCAN_PARAM_2M_PHY_MASK & init_phy)
    {
        IotLogDebug("For LE 2M PHY: \n");
        IotLogDebug("Enter scan_interval\n");
        scanf("%x", &read);
        scan_intval[i] = (UINT16)read;

        IotLogDebug("Enter scan_window\n");
        scanf("%x", &read);
        scan_window[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_min: \n");
        scanf("%x", &read);
        conn_interval_min[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_max: \n");
        scanf("%x", &read);
        conn_interval_max[i] = (UINT16)read;

        IotLogDebug("Enter conn_latency: \n");
        scanf("%x", &read);
        conn_latency[i] = (UINT16)read;

        IotLogDebug("Enter supn_timeout: \n");
        scanf("%x", &read);
        supn_timeout[i] = (UINT16)read;

        IotLogDebug("Enter min_ce_len: \n");
        scanf("%x", &read);
        min_ce_len[i] = (UINT16)read;

        IotLogDebug("Enter max_ce_len: \n");
        scanf("%x", &read);
        max_ce_len[i] = (UINT16)read;

        i += 1;
    }

    if (HCI_LE_SET_EXT_SCAN_PARAM_CODED_PHY_MASK & init_phy)
    {
        IotLogDebug("For LE Coded PHY: \n");
        IotLogDebug("Enter scan_interval\n");
        scanf("%x", &read);
        scan_intval[i] = (UINT16)read;

        IotLogDebug("Enter scan_window\n");
        scanf("%x", &read);
        scan_window[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_min: \n");
        scanf("%x", &read);
        conn_interval_min[i] = (UINT16)read;

        IotLogDebug("Enter conn_interval_max: \n");
        scanf("%x", &read);
        conn_interval_max[i] = (UINT16)read;

        IotLogDebug("Enter conn_latency: \n");
        scanf("%x", &read);
        conn_latency[i] = (UINT16)read;

        IotLogDebug("Enter supn_timeout: \n");
        scanf("%x", &read);
        supn_timeout[i] = (UINT16)read;

        IotLogDebug("Enter min_ce_len: \n");
        scanf("%x", &read);
        min_ce_len[i] = (UINT16)read;

        IotLogDebug("Enter max_ce_len: \n");
        scanf("%x", &read);
        max_ce_len[i] = (UINT16)read;
    }

    retval = BT_hci_le_extended_create_connection
             (
                 init_filr_policy,
                 own_addr_type,
                 peer_addr_type,
                 peer_addr,
                 init_phy,
                 scan_intval,
                 scan_window,
                 conn_interval_min,
                 conn_interval_max,
                 conn_latency,
                 supn_timeout,
                 min_ce_len,
                 max_ce_len
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_privacy_mode(void)
{
    unsigned int read;
    UINT32 index;
    UCHAR  peer_identity_addr_type;
    UCHAR  peer_identity_addr[BT_BD_ADDR_SIZE];
    UCHAR  privacy_mode;
    API_RESULT retval;

    IotLogDebug("Enter peer_identity_address_type : \n");
    scanf("%x", &read);
    peer_identity_addr_type = (UCHAR)read;

    IotLogDebug("Enter peer_identity_address : \n");
    for (index = 0; index < 6; index++)
    {
        scanf("%x", &read);
        peer_identity_addr[index] = (UCHAR)read;
    }

    IotLogDebug("Enter privacy_mode : \n");
    scanf("%x", &read);
    privacy_mode = (UCHAR)read;

    retval = BT_hci_le_set_privacy_mode
             (
                 peer_identity_addr_type,
                 peer_identity_addr,
                 privacy_mode
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}
#endif /* BT_5_0 */

#ifdef BT_5_1
void appl_hci_le_receiver_test_v3(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  rx_channel;
    UCHAR  phy;
    UCHAR  modulation_index;
    UCHAR  expected_cte_length;
    UCHAR  extension_cte_type;
    UCHAR  slot_duration;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter rx_channel :\n");
    scanf("%x", &read);
    rx_channel = (UCHAR)read;

    IotLogDebug("Enter phy :\n");
    scanf("%x", &read);
    phy = (UCHAR)read;

    IotLogDebug("Enter modulation_index :\n");
    scanf("%x", &read);
    modulation_index = (UCHAR)read;

    IotLogDebug("Enter expected_cte_length :\n");
    scanf("%x", &read);
    expected_cte_length = (UCHAR)read;

    IotLogDebug("Enter extension_cte_type :\n");
    scanf("%x", &read);
    extension_cte_type = (UCHAR)read;

    IotLogDebug("Enter slot_duration :\n");
    scanf("%x", &read);
    slot_duration = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_receiver_test_v3
             (
                 rx_channel,
                 phy,
                 modulation_index,
                 expected_cte_length,
                 extension_cte_type,
                 slot_duration,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_transmitter_test_v3(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  tx_channel;
    UCHAR  length_of_test_data;
    UCHAR  packet_payload;
    UCHAR  phy;
    UCHAR  cte_length;
    UCHAR  cte_type;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter tx_channel :\n");
    scanf("%x", &read);
    tx_channel = (UCHAR)read;

    IotLogDebug("Enter length_of_test_data :\n");
    scanf("%x", &read);
    length_of_test_data = (UCHAR)read;

    IotLogDebug("Enter packet_payload :\n");
    scanf("%x", &read);
    packet_payload = (UCHAR)read;

    IotLogDebug("Enter phy :\n");
    scanf("%x", &read);
    phy = (UCHAR)read;

    IotLogDebug("Enter cte_length :\n");
    scanf("%x", &read);
    cte_length = (UCHAR)read;

    IotLogDebug("Enter cte_type :\n");
    scanf("%x", &read);
    cte_type = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_transmitter_test_v3
             (
                 tx_channel,
                 length_of_test_data,
                 packet_payload,
                 phy,
                 cte_length,
                 cte_type,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_set_connectionless_cte_transmit_parameters(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  advertising_handle;
    UCHAR  cte_length;
    UCHAR  cte_type;
    UCHAR  cte_count;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter advertising_handle :\n");
    scanf("%x", &read);
    advertising_handle = (UCHAR)read;

    IotLogDebug("Enter cte_length :\n");
    scanf("%x", &read);
    cte_length = (UCHAR)read;

    IotLogDebug("Enter cte_type :\n");
    scanf("%x", &read);
    cte_type = (UCHAR)read;

    IotLogDebug("Enter cte_count :\n");
    scanf("%x", &read);
    cte_count = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_connectionless_cte_transmit_parameters
             (
                 advertising_handle,
                 cte_length,
                 cte_type,
                 cte_count,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_set_connectionless_cte_transmit_enable(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  advertising_handle;
    UCHAR  cte_enable;

    IotLogDebug("Enter advertising_handle :\n");
    scanf("%x", &read);
    advertising_handle = (UCHAR)read;

    IotLogDebug("Enter cte_enable :\n");
    scanf("%x", &read);
    cte_enable = (UCHAR)read;

    retval = BT_hci_le_set_connectionless_cte_transmit_enable
             (
                 advertising_handle,
                 cte_enable
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_connectionless_iq_sampling_enable(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 sync_handle;
    UCHAR  sampling_enable;
    UCHAR  slot_duration;
    UCHAR  max_sampled_ctes;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter sync_handle :\n");
    scanf("%x", &read);
    sync_handle = (UINT16)read;

    IotLogDebug("Enter sampling_enable :\n");
    scanf("%x", &read);
    sampling_enable = (UCHAR)read;

    IotLogDebug("Enter slot_duration :\n");
    scanf("%x", &read);
    slot_duration = (UCHAR)read;

    IotLogDebug("Enter max_sampled_ctes :\n");
    scanf("%x", &read);
    max_sampled_ctes = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_connectionless_iq_sampling_enable
             (
                 sync_handle,
                 sampling_enable,
                 slot_duration,
                 max_sampled_ctes,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_set_connection_cte_receive_parameters(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR  sampling_enable;
    UCHAR  slot_duration;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter sampling_enable :\n");
    scanf("%x", &read);
    sampling_enable = (UCHAR)read;

    IotLogDebug("Enter slot_duration :\n");
    scanf("%x", &read);
    slot_duration = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_connection_cte_receive_parameters
             (
                 connection_handle,
                 sampling_enable,
                 slot_duration,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_set_connection_cte_transmit_parameters(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR  cte_types;
    UCHAR  length_of_switching_pattern;
    UCHAR * antenna_ids;
    UINT32 index;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter cte_types :\n");
    scanf("%x", &read);
    cte_types = (UCHAR)read;

    IotLogDebug("Enter length_of_switching_pattern :\n");
    scanf("%x", &read);
    length_of_switching_pattern = (UCHAR)read;

    /* Allocate memory for antenna_ids */
    antenna_ids = (UCHAR *)BT_alloc_mem(sizeof(UCHAR) * length_of_switching_pattern);
    if (NULL == antenna_ids)
    {
        IotLogDebug("Memory allocation for antenna_ids Failed. Returning\n");
        return;
    }

    IotLogDebug("Enter antenna_ids :\n");
    for(index = 0; index < length_of_switching_pattern; index++)
    {
        scanf("%x", &read);
        antenna_ids[index] = (UCHAR)read;
    }

    retval = BT_hci_le_set_connection_cte_transmit_parameters
             (
                 connection_handle,
                 cte_types,
                 length_of_switching_pattern,
                 antenna_ids
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(antenna_ids);
}

void appl_hci_le_set_connection_cte_request_enable(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR  enable;
    UINT16 cte_request_interval;
    UCHAR  requested_cte_length;
    UCHAR  requested_cte_type;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter enable :\n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    IotLogDebug("Enter cte_request_interval :\n");
    scanf("%x", &read);
    cte_request_interval = (UINT16)read;

    IotLogDebug("Enter requested_cte_length :\n");
    scanf("%x", &read);
    requested_cte_length = (UCHAR)read;

    IotLogDebug("Enter requested_cte_type :\n");
    scanf("%x", &read);
    requested_cte_type = (UCHAR)read;

    retval = BT_hci_le_set_connection_cte_request_enable
             (
                 connection_handle,
                 enable,
                 cte_request_interval,
                 requested_cte_length,
                 requested_cte_type
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_connection_cte_response_enable(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR  enable;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter enable :\n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    retval = BT_hci_le_set_connection_cte_response_enable
             (
                 connection_handle,
                 enable
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_read_antenna_information(void)
{
    API_RESULT retval;

    retval = BT_hci_le_read_antenna_information();

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_periodic_advertising_receive_enable(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 sync_handle;
    UCHAR  enable;

    IotLogDebug("Enter sync_handle :\n");
    scanf("%x", &read);
    sync_handle = (UINT16)read;

    IotLogDebug("Enter enable :\n");
    scanf("%x", &read);
    enable = (UCHAR)read;

    retval = BT_hci_le_set_periodic_advertising_receive_enable
             (
                 sync_handle,
                 enable
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_periodic_advertising_sync_transfer(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UINT16 service_data;
    UINT16 sync_handle;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter service_data :\n");
    scanf("%x", &read);
    service_data = (UINT16)read;

    IotLogDebug("Enter sync_handle :\n");
    scanf("%x", &read);
    sync_handle = (UINT16)read;

    retval = BT_hci_le_periodic_advertising_sync_transfer
             (
                 connection_handle,
                 service_data,
                 sync_handle
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_periodic_advertising_set_info_transfer(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UINT16 service_data;
    UCHAR  advertising_handle;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter service_data :\n");
    scanf("%x", &read);
    service_data = (UINT16)read;

    IotLogDebug("Enter advertising_handle :\n");
    scanf("%x", &read);
    advertising_handle = (UCHAR)read;

    retval = BT_hci_le_periodic_advertising_set_info_transfer
             (
                 connection_handle,
                 service_data,
                 advertising_handle
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_periodic_advertising_sync_transfer_parameters(void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR  mode;
    UINT16 skip;
    UINT16 sync_timeout;
    UCHAR  cte_type;

    IotLogDebug("Enter connection_handle :\n");
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Enter mode :\n");
    scanf("%x", &read);
    mode = (UCHAR)read;

    IotLogDebug("Enter skip :\n");
    scanf("%x", &read);
    skip = (UINT16)read;

    IotLogDebug("Enter sync_timeout :\n");
    scanf("%x", &read);
    sync_timeout = (UINT16)read;

    IotLogDebug("Enter cte_type :\n");
    scanf("%x", &read);
    cte_type = (UCHAR)read;

    retval = BT_hci_le_set_periodic_advertising_sync_transfer_parameters
             (
                 connection_handle,
                 mode,
                 skip,
                 sync_timeout,
                 cte_type
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_set_default_periodic_advertising_sync_transfer_parameters(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  mode;
    UINT16 skip;
    UINT16 sync_timeout;
    UCHAR  cte_type;

    IotLogDebug("Enter mode :\n");
    scanf("%x", &read);
    mode = (UCHAR)read;

    IotLogDebug("Enter skip :\n");
    scanf("%x", &read);
    skip = (UINT16)read;

    IotLogDebug("Enter sync_timeout :\n");
    scanf("%x", &read);
    sync_timeout = (UINT16)read;

    IotLogDebug("Enter cte_type :\n");
    scanf("%x", &read);
    cte_type = (UCHAR)read;

    retval = BT_hci_le_set_default_periodic_advertising_sync_transfer_parameters
             (
                 mode,
                 skip,
                 sync_timeout,
                 cte_type
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

void appl_hci_le_generate_dhkey_v2(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR * remote_p_256_public_key;
    UCHAR  key_type;
    UINT32 index;

    IotLogDebug("Enter remote_p_256_public_key :\n");
    /* Allocate memory for remote_p_256_public_key */
    remote_p_256_public_key = (UCHAR *)BT_alloc_mem(64);
    if (NULL == remote_p_256_public_key)
    {
        IotLogDebug("Memory allocation for remote_p_256_public_key Failed. Returning\n");
        return;
    }

    for(index = 0; index < 64; index++)
    {
        scanf("%x", &read);
        remote_p_256_public_key[index] = (UCHAR)read;
    }

    IotLogDebug("Enter key_type :\n");
    scanf("%x", &read);
    key_type = (UCHAR)read;

    retval = BT_hci_le_generate_dhkey_v2
             (
                 remote_p_256_public_key,
                 key_type
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }

    BT_free_mem(remote_p_256_public_key);
}

void appl_hci_le_modify_sleep_clock_accuracy(void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR  action;

    IotLogDebug("Enter action :\n");
    scanf("%x", &read);
    action = (UCHAR)read;

    retval = BT_hci_le_modify_sleep_clock_accuracy
             (
                 action
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        IotLogDebug("API returned success...\n");
    }
}

#endif /* BT_5_1 */

#endif /* BT_LE */
