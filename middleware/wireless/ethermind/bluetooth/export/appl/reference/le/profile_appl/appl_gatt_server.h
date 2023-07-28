
/**
 *  \file appl_gatt_server.h
 *
 *  ATT Server Application Header File
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GATT_SERVER_
#define _H_APPL_GATT_SERVER_

/* --------------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

/* --------------------------------------------- Global Definitions */
/* Flag that control enable disable of application event trace prints */
#define APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION

#ifndef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
#define APPL_HAVE_PROFILE_ATT_CB_SUPPORT
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */

/* --------------------------------------------- Structures/Data Types */
typedef struct
{
    UCHAR     att_event;

    UINT16    len1;

    UINT16    len2;
}APPL_VALID_ATT_PDU_LEN;

typedef API_RESULT (*APPL_ATT_EXECUTE_WRITE_HANDLER)(ATT_HANDLE   *handle);

#ifdef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
typedef API_RESULT(*APPL_EVENT_HANDLER_CB)
        (
            ATT_HANDLE    * handle,
            UCHAR           att_event,
            API_RESULT      event_result,
            UCHAR         * event_data,
            UINT16          event_datalen
        )DECL_REENTRANT;
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */

/* --------------------------------------------- Macros */
#define APPL_ATT_WRITE_QUEUE_SIZE         50U
#define APPL_ATT_MAX_WRITE_BUFFER_SIZE    127U /* 255 */
#define APPL_ATT_MTU                      ATT_DEFAULT_MTU

#define APPL_SERVER_BUFFER_SIZE           32U /* 256 */

#define APPL_MAX_GROUP_TYPE_QUERIED       5U
#define APPL_MAX_HNDL_UUID_LIST_SIZE      5U
#define APPL_MAX_HNDL_LIST_SIZE           11U
#define APPL_MAX_HNDL_VALUE_SIZE          5U

/* ATT PDU Request Length */
#define APPL_ATT_XCHNG_MTU_REQ_LEN                    2U
#define APPL_ATT_FIND_INFO_REQ_LEN                    4U
#define APPL_ATT_READ_BY_TYPE_REQ_LEN_1               6U
#define APPL_ATT_READ_BY_TYPE_REQ_LEN_2               20U
#define APPL_ATT_READ_REQ_LEN                         2U
#define APPL_ATT_READ_BLOB_REQ_LEN                    4U
#define APPL_ATT_READ_BY_GROUP_REQ_LEN_1              6U
#define APPL_ATT_READ_BY_GROUP_REQ_LEN_2              20U
#define APPL_ATT_EXECUTE_WRITE_REQ_LEN                1U

#define APPL_ATT_INVALID_LEN                          0xFFU
#define APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK            7

/** Application Defined error */
#define APPL_ATT_INVALID_OFFSET                       0xFFFFU

#define APPL_MAX_MULTIPLE_READ_COUNT                  11U

#define APPL_CHECK_IF_ATT_REQUEST(type)\
        ((((((type) > 0x13) && ((type) < 0x16)) ||\
        (((type) > 0x1E) && (((type) != ATT_SIGNED_WRITE_CMD) &&\
        ((type) != ATT_WRITE_CMD)))) || (0x01 == ((type) & 0x01)))? BT_FALSE : BT_TRUE)

#define APPL_VALID_ATT_EVENT_FOR_LEN_CHK(type)\
        (((0x02 == (type))||(0x04 == (type)) ||\
        ((type) >= 0x08 && (type) <= 0x0C) ||\
        (0x10 == (type)) || (0x18 == (type)))? BT_TRUE : BT_FALSE)

#define APPL_ATT_PREPARE_QUEUE_INIT(i)\
        appl_att_write_queue[(i)].handle = 0x0000U;\
        appl_att_write_queue[(i)].length = 0U;\
        BT_mem_set(appl_att_write_queue[(i)].value, 0, sizeof(APPL_MAX_WRITELONG_SIZE));

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

void main_gatt_server_operations(void);

#ifdef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
API_RESULT appl_register_cb(APPL_EVENT_HANDLER_CB appl_pams_cb);
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */

API_RESULT appl_handle_unsupported_op_code (ATT_HANDLE *handle,UCHAR op_code);

#ifdef APPL_VALIDATE_ATT_PDU_LEN
API_RESULT appl_validate_att_pdu_req_len
           (
               UCHAR     att_event,
               UINT16    event_datalen
           );
#endif /* APPL_VALIDATE_ATT_PDU_LEN */

API_RESULT appl_att_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           );

API_RESULT appl_att_prepare_write_enqueue (ATT_HANDLE_VALUE_OFFSET_PARAM * param);
API_RESULT appl_handle_find_by_type_val_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_RANGE    * range,
                /* IN */ ATT_VALUE           * value,
                /* IN */ ATT_VALUE           * uuid
           );
API_RESULT appl_handle_find_info_request
           (
                /* IN */ ATT_HANDLE         * handle,
                /* IN */ ATT_HANDLE_RANGE   * range
           );
API_RESULT appl_handle_read_by_type_request
           (
               ATT_HANDLE          * handle,
               ATT_HANDLE_RANGE    * range,
               ATT_VALUE           * uuid
           );
API_RESULT appl_handle_read_by_group_type_request
           (
               ATT_HANDLE          * handle,
               ATT_HANDLE_RANGE    * range,
               ATT_VALUE           * uuid
           );

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
API_RESULT appl_handle_read_multiple_var_len_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_LIST     * list
           );
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

API_RESULT appl_handle_read_multiple_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_LIST     * list
           );
API_RESULT appl_handle_read_request
           (
               /* IN */ ATT_HANDLE    * handle,
               /* IN */ UINT16        attr_handle,
               /* IN */ UINT16        offset,
               /* IN */ UCHAR         direction
           );
API_RESULT appl_handle_write_request
           (
               /* IN */ ATT_HANDLE    * handle,
               /* IN */ UINT16        attr_handle,
               /* IN */ UINT16        offset,
               /* IN */ ATT_VALUE     * value,
               /* IN */ UCHAR        flags
           );
API_RESULT appl_handle_security_error
           (
               /* IN */ ATT_HANDLE  * handle,
               /* IN */ UINT16      sec_status
           );
void appl_gatt_signing_verification_complete
     (
         API_RESULT status,
         UCHAR * data,
         UINT16 datalen
     );
void appl_parse_confirmation_data(ATT_HANDLE * handle, UINT16 evt_result);
void appl_parse_ntf_tx_complete(ATT_HANDLE * handle, UCHAR * data, UINT16 datalen);

API_RESULT appl_gatt_set_mtu(UINT16 mtu);

#ifdef APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION
void appl_set_gatt_server_evt_trc(UCHAR flag);
#endif /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
API_RESULT appl_gatt_send_multi_handle_val_ntf_request(DEVICE_HANDLE dq_handle);
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

#endif /* _H_APPL_GATT_SERVER_ */