
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

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

typedef struct
{
    UCHAR     att_event;

    UINT16    len1;

    UINT16    len2;
}APPL_VALID_ATT_PDU_LEN;

typedef API_RESULT (*APPL_ATT_EXECUTE_WRITE_HANDLER)(ATT_HANDLE   *handle);

#define APPL_ATT_WRITE_QUEUE_SIZE          10
#define APPL_ATT_MAX_WRITE_BUFFER_SIZE     255
#define APPL_ATT_MTU                       ATT_DEFAULT_MTU

#define APPL_SERVER_BUFFER_SIZE            256

#define APPL_MAX_GROUP_TYPE_QUERIED        5
#define APPL_MAX_HNDL_UUID_LIST_SIZE       5
#define APPL_MAX_HNDL_LIST_SIZE            11
#define APPL_MAX_HNDL_VALUE_SIZE           5

/* ATT PDU Request Length */
#define APPL_ATT_XCNHG_MTU_REQ_LEN                    2
#define APPL_ATT_FIND_INFO_REQ_LEN                    4
#define APPL_ATT_READ_BY_TYPE_REQ_LEN_1               6
#define APPL_ATT_READ_BY_TYPE_REQ_LEN_2               20
#define APPL_ATT_READ_REQ_LEN                         2
#define APPL_ATT_READ_BLOB_REQ_LEN                    4
#define APPL_ATT_READ_BY_GROUP_REQ_LEN_1              6
#define APPL_ATT_READ_BY_GROUP_REQ_LEN_2              20
#define APPL_ATT_EXECUTE_WRITE_REQ_LEN                1

#define APPL_ATT_INVALID_LEN                          0xFF
#define APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK            7

/** Application Defined error */
#define APPL_ATT_INVALID_OFFSET                0xFFFF
#ifndef ATT_INVALID_ATTR_HANDLE_VAL
#define ATT_INVALID_ATTR_HANDLE_VAL            0x0000
#endif /* ATT_INVALID_ATTR_HANDLE_VAL */

#define APPL_MAX_MULTIPLE_READ_COUNT           11

#define APPL_CHECK_IF_ATT_REQUEST(type)\
        ((((((type) > 0x13) && ((type) < 0x16)) ||\
        (((type) > 0x1E) && (((type) != ATT_SIGNED_WRITE_CMD) &&\
        ((type) != ATT_WRITE_CMD)))) || (0x01 == ((type) & 0x01)))? BT_FALSE : BT_TRUE)

#define APPL_VALID_ATT_EVENT_FOR_LEN_CHK(type)\
        (((0x02 == (type))||(0x04 == (type)) ||\
        ((type) >= 0x08 && (type) <= 0x0C) ||\
        (0x10 == (type)) || (0x18 == (type)))? BT_TRUE : BT_FALSE)

#define APPL_ATT_PREPARE_QUEUE_INIT(i)\
        appl_att_write_queue[(i)].handle_value.handle = 0;\
        appl_att_write_queue[(i)].offset = 0xFFFF;\
        appl_att_write_queue[(i)].handle_value.value.len = 0;\
        appl_att_write_queue[(i)].handle_value.value.val = NULL;

void main_gatt_server_operations(void);

API_RESULT appl_handle_unsupported_op_code (ATT_HANDLE *handle,UCHAR op_code);
API_RESULT appl_validate_att_pdu_req_len
           (
               UCHAR     att_event,
               UINT16    event_datalen
           );
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
#endif /* _H_APPL_GATT_SERVER_ */

