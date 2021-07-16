
/**
 *  \file gatt.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_
#define _H_GATT_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_att_api.h"

#include "gatt_defines.h"


/* --------------------------------------------- Global Definitions */
#ifdef GATT_DEBUG
#define GATT_TRC(...)             BT_debug_trace(BT_MODULE_ID_GATT, __VA_ARGS__)
#define GATT_INF(...)             BT_debug_info (BT_MODULE_ID_GATT, __VA_ARGS__)
#define GATT_debug_dump_bytes(data, datalen) BT_debug_dump_bytes(BT_MODULE_ID_GATT, (data), (datalen))
#else /* GATT_DEBUG */
#define GATT_TRC                  BT_debug_null
#define GATT_INF                  BT_debug_null
#define GATT_debug_dump_bytes(data, datalen)
#endif /* GATT_DEBUG */

#ifndef GATT_NO_DEBUG
#define GATT_ERR(...)             BT_debug_error(BT_MODULE_ID_GATT, __VA_ARGS__)
#else /* GATT_NO_DEBUG */
#define GATT_ERR                  BT_debug_null
#endif /* GATT_NO_DEBUG */

/* GATT Configurable limits */
#define GATT_MAX_SUPPORTED_SERVERS      BT_MAX_REMOTE_DEVICES

#define GATT_NUM_CONTEXTS               BT_MAX_DEVICE_QUEUE_SIZE
#define GATT_INVALID_CONTEXT            0xFF
#define GATT_MAX_SERVICES               5
#define GATT_MAX_INC_SERVICES           2
#define GATT_MAX_CHARACTERISTICS        10
#define GATT_MAX_CHAR_DESCRIPTORS       10

#define GATT_PREPARE_WRITE_REQ_Q_SIZE   10

/* GATT Operation identifiers */
#define GATT_PS_DISCOVERY               0x0001
#define GATT_SS_DISCOVERY               0x0002
#define GATT_IS_DISCOVERY               0x0004
#define GATT_CHAR_DISCOVERY             0x0008
#define GATT_CHAR_DESC_DISCOVERY        0x0010
#define GATT_CHAR_READ                  0x0020
#define GATT_CHAR_READ_UUID             0x0040
#define GATT_CHAR_WRITE                 0x0080
#define GATT_CHAR_RELIABLE_WRITE        0x0100

/* GATT Function Event identifiers */
#define GATT_PS_DISCOVERY_RSP           0xF0
#define GATT_SS_DISCOVERY_RSP           0xF1
#define GATT_IS_DISCOVERY_RSP           0xF2
#define GATT_CHAR_DISCOVERY_RSP         0xF3
#define GATT_CHAR_DESC_DISCOVERY_RSP    0xF4
#define GATT_INVALID_EVENT              0xFF

/* GATT Handle Value Operations */
#define GATT_HV_CNF                     0x00
#define GATT_HV_NTF                     0x01
#define GATT_HV_IND                     0x02

#define GATT_CLI_CFG_VAL_LEN            0x02
#define GATT_SER_CFG_VAL_LEN            0x02

#ifndef ATT_SUPPORT_128_BIT_UUID
#define GATT_EXTRACT_UUID(uuid,val,type)\
        BT_UNPACK_LE_2_BYTE (uuid,val)
#else /* ATT_SUPPORT_128_BIT_UUID */
#define GATT_EXTRACT_UUID(uuid,val,type)\
        if (ATT_16_BIT_UUID_FORMAT == (type))\
        {\
            BT_UNPACK_LE_2_BYTE (&((uuid)->uuid_16),val)\
        }\
        else\
        {\
            BT_UNPACK_LE_N_BYTE ((uuid)->uuid_128.value,val,ATT_128_BIT_UUID_SIZE);\
        }
#endif /* ATT_SUPPORT_128_BIT_UUID */

#ifndef ATT_SUPPORT_128_BIT_UUID
#define GATT_CHECK_UUID(uuid,val,type)\
        (*(val) == *(uuid)? BT_TRUE:BT_FALSE)
#else /* ATT_SUPPORT_128_BIT_UUID */
#define GATT_CHECK_UUID(uuid,val,type)\
        ((type) == ATT_16_BIT_UUID_FORMAT ?\
        (0 == BT_mem_cmp((uuid),val,ATT_16_BIT_UUID_SIZE) ?\
        BT_TRUE:BT_FALSE):\
        (0 == BT_mem_cmp((uuid),val,ATT_128_BIT_UUID_SIZE) ?\
        BT_TRUE:BT_FALSE))
#endif /* ATT_SUPPORT_128_BIT_UUID */

/* --------------------------------------------- Structures/Data Types */

/* Service Information */
typedef struct gatt_service_param
{
    /* Service UUID */
    ATT_UUID uuid;

#ifdef ATT_128_BIT_UUID_FORMAT
    /* Service UUID Type */
    UCHAR uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

    /* Service Range */
    ATT_HANDLE_RANGE range;

} GATT_SERVICE_PARAM;

/* Included Service Information */
typedef struct gatt_inc_service_param
{
    /* Service Declaration Handle */
    ATT_ATTR_HANDLE handle;

    /* Service Information */
    GATT_SERVICE_PARAM service;

} GATT_INC_SERVICE_PARAM;

/* Characteristic Descriptor Information */
typedef struct gatt_char_desc_param
{
    /* Descriptor handle */
    ATT_ATTR_HANDLE handle;

    /* Descriptor UUID */
    ATT_UUID uuid;

#ifdef ATT_128_BIT_UUID_FORMAT
    /* Descriptor UUID Type */
    UCHAR uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

} GATT_CHAR_DESC_PARAM;

/* Characteristic Information */
typedef struct gatt_characteristic_param
{
    /* Characteristic Range */
    ATT_HANDLE_RANGE range;

    /* Characteristic Property */
    UCHAR cproperty;

    /* Characteristic Value Handle */
    ATT_ATTR_HANDLE value_handle;

    /* Characteristic UUID */
    ATT_UUID uuid;

#ifdef ATT_128_BIT_UUID_FORMAT
    /* Characteristic UUID Type */
    UCHAR uuid_type;
#endif /* ATT_128_BIT_UUID_FORMAT */

    /* Characteristic desciptor index*/
    UCHAR desc_index;

    /* Characteristics Value Offset */
    UINT16 val_offset;

    /* Characteristic Value Length */
    UINT16 val_length;

    /* Characteristic descriptor array */
    GATT_CHAR_DESC_PARAM descriptor[GATT_MAX_CHAR_DESCRIPTORS];

} GATT_CHARACTERISTIC_PARAM;

typedef struct gatt_prepare_write_req_param
{
    /* GATT Prepare write request array */
    ATT_PREPARE_WRITE_REQ_PARAM req_param[GATT_PREPARE_WRITE_REQ_Q_SIZE];

    /* GATT prepare write request count */
    UINT16                      count;

} GATT_PREPARE_WRITE_REQ_PARAM;

typedef struct gatt_context
{
    /* ATT Handle */
    ATT_HANDLE att_handle;

    /* Service Array */
    GATT_SERVICE_PARAM service[GATT_MAX_SERVICES];

    /* Included Service Array */
    GATT_INC_SERVICE_PARAM inc_service[GATT_MAX_INC_SERVICES];

    /* Characteristic Array */
    GATT_CHARACTERISTIC_PARAM characteristic[GATT_MAX_CHARACTERISTICS];

    /* Service/Characteristic Index used during discovery */
    UINT16 attr_index;

    /* Temporary parse index */
    UINT16 index;

    /* UUID for which service/characteristic is being searched */
    ATT_UUID uuid;

    /* ATT UUID Format */
    UCHAR    frmt;

    /* Handle requested for procedure */
    UINT16 handle;

    /* Service End handle of procedure */
    UINT16 se_handle;

    /* Cuurent procedure in progress */
    UINT16 proc;

} GATT_CONTEXT;

/* GATT Application Callback */
typedef API_RESULT (* GATT_APP_CB)
                   (
                       ATT_HANDLE   * att_handle,
                       UCHAR          event_type,
                       API_RESULT     event_result,
                       void         * event_data,
                       UINT16         event_datalen
                   );

/* --------------------------------------------- Macros */
/* Discover Primary Services */
#define gatt_discover_ps(atthdl, uuid, frmt) \
        gatt_discover_service((atthdl), (uuid), (frmt), GATT_PS_DISCOVERY)

/* Discover Secondary Services */
#define gatt_discover_ss(atthdl, uuid, frmt) \
        gatt_discover_service((atthdl), (uuid), (frmt), GATT_SS_DISCOVERY)

/* Send handle-value notification */
#define gatt_send_hv_ntf(atthdl, hdl, val, len) \
        gatt_char_hv_action((atthdl), GATT_HV_NTF, (hdl), (val), (len));

/* Send handle-value indication */
#define gatt_send_hv_ind(atthdl, hdl, val, len) \
        gatt_char_hv_action((atthdl), GATT_HV_IND, (hdl), (val), (len));

/* Send handle-value confirmation */
#define gatt_send_hv_cnf(atthdl) \
        gatt_char_hv_action((atthdl), GATT_HV_CNF, 0x0000, NULL, 0x0000);

/* GATT Application Callback */
#define GATT_APP_CB_NTF(h, et, er, ed, edl) \
        if (NULL != gatt_app_cb) \
        { \
            gatt_app_cb ((h), (et), (er), (ed), (edl)); \
        }

#ifdef ATT_SUPPORT_128_BIT_UUID
#define GATT_GET_CHAR_UUID(c) (c)->uuid.uuid_16
#else
#define GATT_GET_CHAR_UUID(c) (c)->uuid
#endif /* ATT_SUPPORT_128_BIT_UUID */

#ifdef ATT_SUPPORT_128_BIT_UUID
#define GATT_GET_CLI_CHAR_DESC_UUID(c,i) (c)->descriptor[i].uuid.uuid_16
#else
#define GATT_GET_CLI_CHAR_DESC_UUID(c,i) (c)->descriptor[i].uuid
#endif /* ATT_SUPPORT_128_BIT_UUID */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
API_RESULT gatt_init (GATT_APP_CB cb);
API_RESULT gatt_xchg_mtu (ATT_HANDLE * att_handle, UINT16 mtu, UCHAR rsp);
API_RESULT gatt_discover_service (ATT_HANDLE * att_handle, ATT_UUID uuid, UCHAR frmt, UINT16 type);
API_RESULT gatt_discover_is (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh);
API_RESULT gatt_discover_char (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh, UINT16 uuid, UCHAR desc);
API_RESULT gatt_discover_char_desc (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh);
API_RESULT gatt_char_read (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh, ATT_UUID uuid, UCHAR frmt);
API_RESULT gatt_char_read_long (ATT_HANDLE * att_handle, UINT16 hdl, UINT16 offset);
API_RESULT gatt_char_read_multiple (ATT_HANDLE * att_handle, ATT_READ_MULTIPLE_REQ_PARAM * param);
API_RESULT gatt_char_wr (ATT_HANDLE * att_handle, UINT16 hdl, UCHAR * value, UINT16 length, UCHAR rsp);
API_RESULT gatt_char_wr_long (ATT_HANDLE * att_handle, ATT_PREPARE_WRITE_REQ_PARAM * prepare_write_param);
API_RESULT gatt_char_reliable_wr (ATT_HANDLE * att_handle, GATT_PREPARE_WRITE_REQ_PARAM * req_param);
API_RESULT gatt_char_hv_action
           (
               ATT_HANDLE * att_handle,
               UCHAR action,
               UINT16 hdl,
               UCHAR * value,
               UINT16 length
           );


/* --------------------------------------------- Internal Declarations */
API_RESULT gatt_search_context (ATT_HANDLE * att_handle, UCHAR * id);
API_RESULT gatt_alloc_context (ATT_HANDLE * att_handle, UCHAR * id);
API_RESULT gatt_free_context (ATT_HANDLE * att_handle);
void gatt_handle_service_search_complete
     (
         /* IN */ GATT_CONTEXT * context
     );
void gatt_handle_included_service_search_complete
     (
         /* IN */ GATT_CONTEXT * context
     );
void gatt_handle_characteristics_search_complete
     (
          /* IN */ GATT_CONTEXT * context
     );
void gatt_handle_char_desc_search_complete
     (
          /* IN */ GATT_CONTEXT * context
     );
API_RESULT gatt_handle_find_by_type_value_response
           (
               ATT_HANDLE     * att_handle,
               UCHAR          * data,
               UINT16         datalen
           );
API_RESULT gatt_handle_read_blob_response
           (
               ATT_HANDLE   * att_handle,
               UCHAR          * data,
               UINT16         datalen
           );
API_RESULT gatt_handle_read_by_group_response
           (
               ATT_HANDLE   * att_handle,
               UINT16         data_elem_size,
               UCHAR          * data,
               UINT16         data_len
           );
API_RESULT gatt_handle_find_info_response
           (
               ATT_HANDLE  * att_handle,
               UINT16        uuid_type,
               UCHAR         * data,
               UINT16        data_len
           );
API_RESULT gatt_handle_read_by_type_response
           (
               ATT_HANDLE  * att_handle,
               UINT16        data_elem_size,
               UCHAR         * data,
               UINT16        data_len
           );
API_RESULT gatt_handle_prepare_write_response
           (
               ATT_HANDLE  * att_handle,
               UCHAR         * data,
               UINT16        data_len
           );
API_RESULT gatt_handle_error_response
           (
               /* IN */ ATT_HANDLE   * att_handle,
               /* IN */ UCHAR          response_code
           );
API_RESULT gatt_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           );
#endif /* _H_GATT_ */


