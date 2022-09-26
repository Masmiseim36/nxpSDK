
/**
 *  \file appl_att_client.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_ATT_CLIENT_
#define _H_APPL_ATT_CLIENT_
/* --------------------------------------------- Header File Inclusion */
#include "BT_api.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */
typedef struct
{
    /* Indicates the UUID for which service is being seacrhed */
    UINT16 uuid;

    /**
     * Indicates the end handle that was provided in the search, handle 0x0000
     * indicates no service discovery is in progress
     */
    UINT16 handle;

    /**
     * Points to the service being discovered, this is useful only when
     * Characteristics are being discovered and refers to the last service
     * discovered in during discovery of Primary or Secondary Services Procedure
     */
    UINT16  service_index;

    /**
     * Ongoing configuration being requested on the connection
     */
    UINT16  config_property;
}APPL_SERVICE_DISCOVERY_STATE;

/**
 * Abstracts handle value information for 16 bit values, this is used to store
 * information related to service and characteristic information and was not
 * designed for generic purposes
 */
typedef struct
{
    /* Handle at which the information is stored */
    ATT_HANDLE_RANGE    range;

    /* Value at the handle */
    UINT16              value;
}APPL_GATT_SERVICE_DESC;

typedef struct
{
    UINT16 uuid;

    UCHAR * desc;
}APPL_GATT_UUID_DESC;

typedef struct
{
    UCHAR s_index;
    UCHAR c_property;
    UINT16 value_handle;
    UINT16 uuid;
    UINT16 client_config;
    UINT16 client_config_handle;
}APPL_GATT_CHARACTERISTIC_DESCRIPTOR;

typedef struct
{
    /* Handle at which the Characteristic is defined */
    UINT16 handle;

    /* Characteristic Format Descriptor */
    APPL_GATT_CHARACTERISTIC_DESCRIPTOR  desc;
}APPL_CHARACTERISTIC_INFORMATION;

typedef struct
{
    UINT16 char_start_range;

    UINT16 char_end_range;

    APPL_GATT_SERVICE_DESC    service_info;
}APPL_SERVICE_INFORMATION;


typedef struct
{
    /* Contains information of current Service Discovery Proecudure */
    APPL_SERVICE_DISCOVERY_STATE    state;

    /* Primary Service Information */
    UINT16    pri_start_range;

    UINT16    pri_end_range;

    /* Secondary Service Information */
    UINT16    sec_start_range;

    UINT16    sec_end_range;

}APPL_PEER_SERVER_INFO;

typedef struct
{
    UINT16 uuid;

    UCHAR  * desc_str;
}APPL_UUID_DESC;

/* --------------------------------------------- Macros */

#define APPL_MAX_CHARACTERISTICS          50
#define APPL_MAX_SERVICES                 20
#define GATT_PRIMARY_SERVICE_UUID         0x2800
#define GATT_SECONDARY_SERVICE_UUID       0x2801
#define GATT_CHARACTERISTIC_UUID          0x2803
#define GATT_CLIENT_CNFG_UUID             0x2902


/* GATT Services */
#define GATT_HDI_SERVICE_UUID             0x180A
#define GATT_THERM_SERVICE_UUID           0x1809
#define GATT_HR_SERVICE_UUID              0xF000
#define GATT_BPM_SERVICE_UUID             0xA000

/* GATT Characteristics */
#define GATT_TEMP_CHAR_UUID               0x2A1C
#define GATT_TEMP_TYPE_CHAR_UUID          0x2A1D
#define GATT_INTER_TEMP_CHAR_UUID         0x2A1E
#define GATT_TEMP_INTERVAL_CHAR_UUID      0x2A21

#define GATT_HR_CHAR_UUID                 0xA105
#define GATT_HR_SENSOR_LOC_CHAR_UUID      0xA106
#define GATT_HR_COMMAND_UUID              0xA107

#define GATT_BPM_CHAR_UUID                0xA100
#define GATT_INTER_BPM_UUID               0xA101

#define GATT_DIS_SYSTEM_ID_CHAR_UUID      0x2A23
#define GATT_DIS_MODEL_NUM_CHAR_UUID      0x2A24
#define GATT_DIS_SERIAL_NUM_CHAR_UUID     0x2A25
#define GATT_DIS_FW_VER_CHAR_UUID         0x2A26
#define GATT_DIS_HW_VER_CHAR_UUID         0x2A27
#define GATT_DIS_SW_VER_CHAR_UUID         0x2A28
#define GATT_DIS_MAN_NAME_CHAR_UUID       0x2A29
#define GATT_DIS_REG_CERT_CHAR_UUID       0x2A2A

#define APPL_INVALID_CHARACTERISTICS_INDEX   0xFFFF
#define APPL_INVALID_SERVICE_INDEX           0xFFFF
#define APPL_INVLAID_CHAR_CONFIGURATION      0xFFFF
#define APPL_INVALID_UUID                    0x0000

#define APPL_GATT_CLIENT_CH_NOTIFICATION     0x0001
#define APPL_GATT_CLIENT_CH_INDICATION       0x0002

#define APPL_GATT_CHARACTERISTICS_INDICATION   0x20

#define APPL_CHECK_IF_CH_SUPPORTS_INDICATIONS(id)\
        APPL_IS_PROPERTY_IS_SUPPROTED(id,APPL_GATT_CHARACTERISTICS_INDICATION)

#define APPL_IS_PROPERTY_IS_SUPPROTED(id,prpty)\
        (((prpty) == (characteristic_array[(id)].desc.c_property & \
        (prpty)))? BT_TRUE : BT_FALSE)

#define appl_discover_primary_services(range)\
        appl_att_read_by_group_type(range,GATT_PRIMARY_SERVICE_UUID)

#define appl_discover_characteristic(range)\
        appl_att_read_by_type(range,GATT_CHARACTERISTIC_UUID)

#define appl_discover_secondary_services(range)\
        appl_att_read_by_group_type(range,GATT_SECONDARY_SERVICE_UUID)

#define appl_discover_client_ch_configuration(range)\
        appl_att_read_by_type(range,GATT_CLIENT_CNFG_UUID)

#define appl_configure_temperature_characteristics(prpty)\
        appl_configure_characteristic(GATT_TEMP_CHARACTERISTIC_UUID,prpty)

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/**
 *  \fn main_att_operations
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \return void 
 */
void main_att_client_operations(void);

void main_gatt_client_operations(void);

/**
 *  \fn att_cb
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param event_type
 *  \param event_result
 *  \param bd_addr
 *  \param event_data
 *  \param event_datalen
 *
 *  \return void 
 */
API_RESULT att_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           );

void appl_att_initiate_service_discovery (void);

void appl_att_read_by_type
     (
         /* IN */ ATT_HANDLE_RANGE * range,
         /* IN */ UINT16 uuid
     );

void appl_att_read_by_group_type
     (
         /* IN */ ATT_HANDLE_RANGE * range,
         /* IN */ UINT16 uuid
     );

void appl_handle_read_by_type_response
     (
         /* IN */ UINT16   data_elem_size,
         /* IN */ UCHAR   * data,
         /* IN */ UINT16  data_len
     );

void appl_handle_read_by_group_response
     (
         UINT16 data_elem_size,
         UCHAR * data,
         UINT16 data_len
     );

void appl_handle_disconnection_indication(void);

void appl_handle_error_response
     (
         /* IN */ UCHAR response_code
     );

void appl_handle_find_info_response (UCHAR * list, UINT16 length, UCHAR type);

void appl_handle_primary_service_search_complete (UINT16 handle);

void appl_handle_secondary_service_search_complete (UINT16 handle);

void appl_handle_characteristics_search_complete(UINT16 handle);

void appl_display_peer_service_record(void);

void appl_display_service_info
     (
         /* IN */ UINT16   start_range,
         /* IN */ UINT16   end_range
     );

void appl_print_characteristic_information
     (
         /* IN */ UINT16 start_range,
         /* IN */ UINT16 end_range
     );

void appl_configure_characteristic
    (
        /* IN */ UINT16 ch_uuid,
        /* IN */ UINT16 ch_property
    );

void appl_write_attribute_value(UINT16 handle, UCHAR *value, UINT16 length);

void appl_attr_read_value(UINT16 handle);

void appl_att_read_blob_request (ATT_READ_BLOB_REQ_PARAM    * param);

void appl_att_read_multiple_request (ATT_HANDLE_LIST * list);

void appl_att_prepare_write_request (ATT_PREPARE_WRITE_REQ_PARAM * param);

void appl_att_execute_write_request (ATT_EXECUTE_WRITE_REQ_PARAM   * param);

void appl_handle_read_response (UCHAR * data, UINT16 datalen);

void appl_handle_read_blob_response (UCHAR * data, UINT16 datalen);

UCHAR * appl_uuid_desc (UINT16 UUID);

void appl_weight_scale_collector_operations (void);

UCHAR* appl_display_uuid_label(UINT16 uuid);

void appl_thermometer_options (void);

void appl_heart_rate_options (void);

void appl_bpm_options (void);

void appl_hid_host_options (void);

void appl_puid_client_options (void);

void appl_read_characteristic (UINT16 c_index);

API_RESULT appl_discover_characteristic_descriptor
           (
               UINT16 start_handle,
               UINT16 end_handle
           );

#endif /* _H_APPL_ATT_CLIENT_ */

