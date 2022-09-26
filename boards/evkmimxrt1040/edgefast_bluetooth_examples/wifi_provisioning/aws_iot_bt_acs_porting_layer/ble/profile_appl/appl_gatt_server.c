
/**
 *  \file appl_gatt_server.c
 *
 *  This file contains the ATT client application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_gatt_server.h"
#include "appl_smp.h"

#include "BT_status.h"

#include "appl_service.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_GATT_SERVER" )
#include "iot_logging_setup.h"

#ifdef ATT

/* --------------------------------------------- External Global Variables */
extern UCHAR g_appl_bond_del_flag;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
DECL_STATIC UCHAR *gatt_server_menu = (UCHAR *) \
"--------------------------------------------\n\
             GATT SERVER MENU\n\
 --------------------------------------------\n\
 0 - Exit \n\
 1 - Refresh \n\
 \n\
 2 - Enable/Disable Authorization\n\
 \n\
 3 - Send Service Changed HVI\n\
 \n\
10 - Service Menu Operations\n\
Your Option ? ";

#ifdef SMP_DATA_SIGNING
static UCHAR * s_data;
static UINT16 s_datalen;
static ATT_HANDLE appl_signed_write_server_handle;
#endif /* SMP_DATA_SIGNING */

#ifdef ATT_QUEUED_WRITE_SUPPORT
API_RESULT appl_att_prepare_write_cancel (ATT_HANDLE   *handle);
API_RESULT appl_att_prepare_write_execute (ATT_HANDLE   *handle);
API_RESULT appl_att_prepare_write_queue_init (void);

static ATT_HANDLE_VALUE_OFFSET_PARAM appl_att_write_queue[APPL_ATT_WRITE_QUEUE_SIZE];
static UCHAR appl_att_write_queue_buffer[APPL_ATT_MAX_WRITE_BUFFER_SIZE];
static UCHAR appl_att_write_buffer_ptr;
static UCHAR appl_att_write_queue_index;
static APPL_ATT_EXECUTE_WRITE_HANDLER appl_execute_write_handler[2] =
                  {
                      appl_att_prepare_write_cancel,
                      appl_att_prepare_write_execute
                  };
#endif /* ATT_QUEUED_WRITE_SUPPORT */
UCHAR appl_att_buffer[APPL_SERVER_BUFFER_SIZE];
UCHAR appl_hvc_flag;

#ifdef APPL_VALIDATE_ATT_PDU_LEN
static APPL_VALID_ATT_PDU_LEN appl_valid_att_pdu_len
                              [APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK] =
{
    {
        ATT_XCHNG_MTU_REQ,
        APPL_ATT_XCNHG_MTU_REQ_LEN,
        APPL_ATT_INVALID_LEN
    },
    {
        ATT_FIND_INFO_REQ,
        APPL_ATT_FIND_INFO_REQ_LEN,
        APPL_ATT_INVALID_LEN
    },
    {
        ATT_READ_BY_TYPE_REQ,
        APPL_ATT_READ_BY_TYPE_REQ_LEN_1,
        APPL_ATT_READ_BY_TYPE_REQ_LEN_2
    },
    {
        ATT_READ_REQ,
        APPL_ATT_READ_REQ_LEN,
        APPL_ATT_INVALID_LEN
    },
    {
        ATT_READ_BLOB_REQ,
        APPL_ATT_READ_BLOB_REQ_LEN,
        APPL_ATT_INVALID_LEN
    },
    {
        ATT_READ_BY_GROUP_REQ,
        APPL_ATT_READ_BY_GROUP_REQ_LEN_1,
        APPL_ATT_READ_BY_GROUP_REQ_LEN_2
    },
    {
        ATT_EXECUTE_WRITE_REQ,
        APPL_ATT_EXECUTE_WRITE_REQ_LEN,
        APPL_ATT_INVALID_LEN

    }
};
#endif /* APPL_VALIDATE_ATT_PDU_LEN */

/* Application Error Code for Invalid Transport Access Error */
/* This can be any value between 0x80 - 0x9F according to the ATT Spec */
#define APPL_ATT_TRANSPORT_ACCESS_APPL_ERR       0x90

/* --------------------------------------------- Functions */
API_RESULT appl_handle_security_error
           (
               /* IN */ ATT_HANDLE  * handle,
               /* IN */ UINT16      sec_status
           )
{
    BT_IGNORE_UNUSED_PARAM(handle);

    if (
           (GATT_DB_INSUFFICIENT_ENC_KEY_SIZE == sec_status) ||
           (API_FAILURE == sec_status)
       )
    {
        /* Set the Bond to be erased on disconnect flag */
        g_appl_bond_del_flag = BT_TRUE;
    }

    return API_SUCCESS;
}

#ifdef ATT_QUEUED_WRITE_SUPPORT
API_RESULT appl_att_prepare_write_queue_init (void)
{
    UINT32 index;

    index = 0;

    do
    {
        IotLogDebug (
        "[APPL]: Initializing Queue Element 0x%02lX\n",index);

        APPL_ATT_PREPARE_QUEUE_INIT (index);
        index++;
    } while (index < APPL_ATT_WRITE_QUEUE_SIZE);

    appl_att_write_buffer_ptr = 0;
    appl_att_write_queue_index = 0;

    return API_SUCCESS;
}

API_RESULT appl_att_prepare_write_enqueue (ATT_HANDLE_VALUE_OFFSET_PARAM * param)
{
    API_RESULT retval;

    retval = API_FAILURE;

    IotLogDebug (
    "[APPL]: Queue index: 0x%02X Offset: 0x%04X Length: 0x%04X, Value "
    "Buffer Index 0x%02X",appl_att_write_queue_index,
    appl_att_write_queue[appl_att_write_queue_index].offset,
    appl_att_write_queue[appl_att_write_queue_index].handle_value.value.len,
    appl_att_write_buffer_ptr);

    if ((APPL_ATT_MAX_WRITE_BUFFER_SIZE > (appl_att_write_buffer_ptr + param->handle_value.value.len)) &&
       (APPL_ATT_WRITE_QUEUE_SIZE != appl_att_write_queue_index))
    {
        IotLogDebug (
        "[APPL]: Allocating Element 0x%02X, Buffer Index 0x%02X\n",
        appl_att_write_queue_index, appl_att_write_buffer_ptr);

        appl_att_write_queue[appl_att_write_queue_index] = (*param);
        appl_att_write_queue[appl_att_write_queue_index].handle_value.value.val\
            = &appl_att_write_queue_buffer[appl_att_write_buffer_ptr];

        BT_mem_copy
        (
            appl_att_write_queue[appl_att_write_queue_index].\
            handle_value.value.val,
            param->handle_value.value.val,
            param->handle_value.value.len
        );

        IotLogDebug (
        "[APPL]: Queued element Offset: 0x%04X, Length: 0x%04X\n",
        appl_att_write_queue[appl_att_write_queue_index].offset,
        appl_att_write_queue[appl_att_write_queue_index].handle_value.value.len);

        appl_att_write_queue_index++;
        appl_att_write_buffer_ptr += (UCHAR)param->handle_value.value.len;

        IotLogDebug (
        "[APPL]: Queue index: 0x%02X , Value Buffer Index 0x%02X\n",
        appl_att_write_queue_index,
        appl_att_write_buffer_ptr);

        retval = API_SUCCESS;
    }
    else
    {
        IotLogDebug (
        "[APPL]: Cannot Enqueue value!");
    }

    return retval;
}

API_RESULT appl_att_prepare_write_cancel (ATT_HANDLE   * handle)
{
    API_RESULT retval;

    appl_att_prepare_write_queue_init ();

    /* Send response */
    retval = BT_att_send_execute_write_rsp (handle);

    IotLogDebug (
    "[APPL]: Sent Execute Write Response with result 0x%04X",retval);

    return retval;
}

API_RESULT appl_att_prepare_write_execute (ATT_HANDLE   * handle)
{
    UINT32                            index;
    API_RESULT                        retval;
    ATT_ERROR_RSP_PARAM               err_param;

    retval = API_FAILURE;
    err_param.op_code    = ATT_EXECUTE_WRITE_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;

    index = 0;

    IotLogDebug (
    "[APPL]: Number of elements to be executed = 0x%02X",
    appl_att_write_queue_index);

    do
    {
        IotLogDebug (
        "[APPL]: Executing Element 0x%02lX Handle 0x%04X, Value Length 0x%04X\n",
        index,appl_att_write_queue[index].handle_value.handle,
        appl_att_write_queue[index].handle_value.value.len);

#ifdef HPS
        appl_hps_write_char_value
        (
            &appl_att_write_queue[index].handle_value,
            appl_att_write_queue[index].offset
        );
#endif /* HPS */

        retval = BT_gatt_db_access_handle
                 (
                      &appl_att_write_queue[index].handle_value,
                      handle,
                      appl_att_write_queue[index].offset,
                      (GATT_DB_WRITE | GATT_DB_PEER_INITIATED)
                 );

        if (API_SUCCESS != retval)
        {
            IotLogDebug (
            "[APPL]: Failed to Execute write on handle 0x%04X, reason 0x%04X\n",
            appl_att_write_queue[index].handle_value.handle, retval);
            break;
        }
        else
        {
#ifdef OTS
            /* Passing requested Complete Execute writes with offset to Higer Layer */
            appl_ots_update_obj_metadata_val
            (
                handle,
                appl_att_write_queue[index].handle_value.handle,
                &appl_att_write_queue[index].handle_value.value,
                appl_att_write_queue[index].offset
            );
#endif /* OTS */
        }
        index++;
    }while (index < appl_att_write_queue_index);

    /* TBD: What happens if Write on or more but not all attributes fails? */
    if (0 != index && API_SUCCESS == retval)
    {
        retval = BT_att_send_execute_write_rsp
                 (
                      handle
                 );
        IotLogDebug (
        "[APPL]: Execute Write Response sent with result 0x%04X\n",retval);
    }
    else
    {
        err_param.handle = appl_att_write_queue[index].handle_value.handle;
        if (GATT_DB_INVALID_OFFSET == retval)
        {
            err_param.error_code = ATT_INVALID_OFFSET;
        }
        else if (GATT_DB_INSUFFICIENT_BUFFER_LEN == retval)
        {
            err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
        }
        else
        {
            err_param.error_code = (UCHAR)(0x00FF & retval);
        }
        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        IotLogDebug (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    appl_att_prepare_write_queue_init ();

    return retval;
}
#endif /* ATT_QUEUED_WRITE_SUPPORT */

API_RESULT appl_handle_unsupported_op_code (ATT_HANDLE * handle,UCHAR op_code)
{
    ATT_ERROR_RSP_PARAM               err_param;
    API_RESULT                        retval;

    err_param.handle = ATT_INVALID_ATTR_HANDLE_VAL;
    err_param.op_code = op_code;
    err_param.error_code = ATT_REQUEST_NOT_SUPPORTED;

    retval = BT_att_send_error_rsp (handle,&err_param);

    if (API_SUCCESS != retval)
    {
        IotLogError (
        "[APPL]:[*** ERR ***]: Failed to send error response, reason 0x%04X\n",
        retval);
    }
    else
    {
        IotLogError (
        "[APPL]:Sent Error Response for unsupported Op Code\n");
    }

    return retval;
}


API_RESULT appl_handle_find_by_type_val_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_RANGE    * range,
                /* IN */ ATT_VALUE           * value,
                /* IN */ ATT_VALUE           * uuid
           )
{
    ATT_ERROR_RSP_PARAM               err_param;
    ATT_FIND_BY_TYPE_VAL_RSP_PARAM    list;
    ATT_HANDLE_RANGE                  group_list[APPL_MAX_GROUP_TYPE_QUERIED];
    API_RESULT                        retval;

    list.count = 0;

    err_param.op_code = ATT_FIND_BY_TYPE_VAL_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
    err_param.handle = range->start_handle;

    IotLogDebug(
    "[APPL]: >>> Handling Find By Type Value request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);

    do
    {
        retval = BT_gatt_db_get_range_by_type_val
                 (
                     handle,
                     range,
                     uuid,
                     value,
                     &group_list[list.count]
                 );

        if ((API_SUCCESS == retval) ||
           (GATT_DB_MORE_MATCHING_RESULT_FOUND == retval))
        {
            list.count++;
        }
        if (GATT_DB_MORE_MATCHING_RESULT_FOUND != retval)
        {
            /**
             * This is an invalid sentence in the specification. An errata has
             * been raised and adopted
             * BT_Errata_ID:  4062
             */
#if 0
            if (list.count != 0)
            {
                group_list[list.count-1].end_handle = ATT_ATTR_HANDLE_END_RANGE;
            }
#endif /* 0 */
            break;
        }
    } while ((list.count < APPL_MAX_GROUP_TYPE_QUERIED) &&
            (range->end_handle >= range->start_handle));

    if (0 != list.count)
    {
        list.range = group_list;
        IotLogDebug(
        "[APPL]: Number of occurences of UUID 0x%04X = 0x%04X\n",
        *uuid->val,list.count);

        retval = BT_att_send_find_by_type_val_rsp
                 (
                     handle,
                     &list
                 );
    }
    else
    {
        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        IotLogDebug (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    IotLogDebug(
    "[APPL]: <<< Handling Find By Type Value request, Range 0x%04X-0x%04X,"
    " with result 0x%04X\n",range->start_handle,range->end_handle,retval);

    return retval;
}


API_RESULT appl_handle_find_info_request
           (
                /* IN */ ATT_HANDLE         * handle,
                /* IN */ ATT_HANDLE_RANGE   * range
           )
{
    ATT_FIND_INFO_RSP_PARAM    param;
    ATT_ERROR_RSP_PARAM        err_param;
    ATT_HANDLE_VALUE_PAIR      handle_value_list[APPL_MAX_HNDL_UUID_LIST_SIZE];
    ATT_HANDLE_UUID_PAIR       handle_uuid_list[APPL_MAX_HNDL_UUID_LIST_SIZE];
    UINT32                     index;
    API_RESULT                 retval;

    param.uuid_format = ATT_16_BIT_UUID_FORMAT;

    IotLogDebug(
    "[APPL]: >>> Handling Find Info request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);


    index = 0;
    err_param.handle = range->start_handle;
    err_param.op_code = ATT_FIND_INFO_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
    do
    {
        if (ATT_16_BIT_UUID_FORMAT == param.uuid_format)
        {
            handle_value_list[index].value.len = ATT_16_BIT_UUID_SIZE;
        }

        retval = BT_gatt_db_get_handle_uuid_pair
                 (
                     handle,
                     range,
                     &handle_value_list[index]
                 );

        if ((API_SUCCESS == retval) ||
           (GATT_DB_MORE_MATCHING_RESULT_FOUND == retval))
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            IotLogDebug (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> UUID Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);
#else /* ATT_SUPPORT_128_BIT_UUID */
            IotLogDebug (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> UUID Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);
#endif /* ATT_SUPPORT_128_BIT_UUID */
            index++;
        }

        if (GATT_DB_INCORRECT_UUID_FRMT == retval)
        {
            if (0 == index)
            {
                /* Search again, change the UUID format requested */
                param.uuid_format = ATT_128_BIT_UUID_FORMAT;
                handle_value_list[index].value.len = ATT_128_BIT_UUID_SIZE;
            }
            else
            {
                /* Else, pack no more, as mixed UUID format detected */
                break;
            }
        }
        else if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
        {
            continue;
        }
        else if (GATT_DB_MORE_MATCHING_RESULT_FOUND != retval)
        {
            /* Search complete */
            IotLogDebug (
            "[APPL]:Search Complete, #Handle UUID List found = 0x%02lX\n",
            index);
            break;
        }
    }while (index < APPL_MAX_HNDL_UUID_LIST_SIZE);

    if (0 != index)
    {
        param.handle_value_list.list_count = (UINT16)index;

        for (index = 0; index < param.handle_value_list.list_count; index++)
        {
            handle_uuid_list[index].handle = handle_value_list[index].handle;
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (handle_value_list[index].value.len == ATT_16_BIT_UUID_SIZE)
            {
                BT_UNPACK_LE_2_BYTE
                (
                    &handle_uuid_list[index].uuid.uuid_16,
                    handle_value_list[index].value.val
                );
            }
            else
            {
                BT_UNPACK_LE_N_BYTE
                (
                    &handle_uuid_list[index].uuid.uuid_128,
                    handle_value_list[index].value.val,
                    ATT_128_BIT_UUID_SIZE
                );
            }
#else
            BT_UNPACK_LE_2_BYTE
            (
                &handle_uuid_list[index].uuid,
                handle_value_list[index].value.val
            );
#endif /* ATT_SUPPORT_128_BIT_UUID */
        }

        param.handle_value_list.list = &handle_uuid_list[0];

        retval = BT_att_send_find_info_rsp (handle,&param);
        IotLogDebug (
        "[APPL]: Sent Find Info Response with result 0x%04X\n",retval);
    }
    else
    {
        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        IotLogDebug (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    IotLogDebug(
    "[APPL]: <<< Handling Find Info request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);

    return retval;
}

API_RESULT appl_handle_read_by_type_request
           (
               ATT_HANDLE          * handle,
               ATT_HANDLE_RANGE    * range,
               ATT_VALUE           * uuid
           )
{
    ATT_READ_BY_TYPE_RSP_PARAM    param;
    ATT_ERROR_RSP_PARAM           err_param;
    ATT_HANDLE_VALUE_PAIR         handle_value_list[APPL_MAX_HNDL_VALUE_SIZE];
    UINT32                        index;
    API_RESULT                    retval;
    UINT16                        len;

    IotLogDebug(
    "[APPL]: >>> Handling Read By Type request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);

    index = 0;
    len = 0;
    err_param.handle = range->start_handle;
    err_param.op_code = ATT_READ_BY_TYPE_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
    do
    {
        handle_value_list[index].value.len = (APPL_SERVER_BUFFER_SIZE - len);

        retval = BT_gatt_db_get_handle_value_pair
                 (
                     handle,
                     range,
                     uuid,
                     &handle_value_list[index]
                 );

        if ((API_SUCCESS == retval) ||
           (GATT_DB_MORE_MATCHING_RESULT_FOUND == retval))
        {
            IotLogDebug (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> Value Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);

#if 0
            /**
             * Commenting out, unnecessary checks will should ideally not be true.
             */
            if (handle_value_list[index].handle > range->end_handle)
            {
                break;
            }
#endif /* 0 */
            if (0 == index)
            {
                len = handle_value_list[index].value.len;
                index++;
            }
            else if (len == handle_value_list[index].value.len)
            {
                index++;
            }
            else
            {
                /* Record not of same length, skip */
                break;
            }
        }

        if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
        {
            continue;
        }
        else if (GATT_DB_MORE_MATCHING_RESULT_FOUND != retval)
        {
            /* Search complete */
            IotLogDebug (
            "[APPL]:Search Complete, #Handle UUID List found = 0x%02lX\n",
            index);
            break;
        }
    }while (index < APPL_MAX_HNDL_VALUE_SIZE);

    if (0 != index)
    {
        param.handle_value_list = handle_value_list;
        param.count = (UINT16)index;

        retval = BT_att_read_by_type_rsp (handle,&param);
        IotLogDebug (
        "[APPL]: Sent Read By Type Response with result 0x%04X\n",retval);
    }
    else
    {
        if (GATT_DB_INVALID_OPERATION == retval)
        {
            err_param.handle = handle_value_list[index].handle;
            err_param.error_code = ATT_READ_NOT_PERMITTED;
        }
        else if (GATT_DB_INSUFFICIENT_SECURITY == retval)
        {
            err_param.handle = handle_value_list[index].handle;
            err_param.error_code = ATT_INSUFFICIENT_AUTHENTICATION;
        }
        else if (GATT_DB_INSUFFICIENT_ENC_KEY_SIZE == retval)
        {
            err_param.handle = handle_value_list[index].handle;
            err_param.error_code = ATT_INSUFFICIENT_ENC_KEY_SIZE;

            /* Application Updates to Handle Encryption Key Size Error */
            appl_handle_security_error
            (
                handle,
                retval
            );
        }
        else if (GATT_DB_INSUFFICIENT_ENCRYPTION == retval)
        {
            err_param.handle = handle_value_list[index].handle;
            err_param.error_code = ATT_INSUFFICIENT_ENCRYPTION;
        }
        if (GATT_DB_DELAYED_RESPONSE != retval)
        {
            retval = BT_att_send_error_rsp
            (
                handle,
                &err_param
            );
            IotLogDebug(
                "[APPL]: Sent Error Response with result 0x%04X\n", retval);
        }
    }

    IotLogDebug(
    "[APPL]: <<< Handling Read By Type request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);

    return retval;
}

API_RESULT appl_handle_read_by_group_type_request
           (
               ATT_HANDLE          * handle,
               ATT_HANDLE_RANGE    * range,
               ATT_VALUE           * uuid
           )
{
    ATT_ERROR_RSP_PARAM            err_param;
    ATT_GROUP_ATTR_DATA_LIST       list;
    ATT_GROUP_ATTR_DATA_ELEMENT    group_list[APPL_MAX_GROUP_TYPE_QUERIED];
    ATT_VALUE                      value;
    API_RESULT                     retval;

    list.count = 0;

    err_param.handle = range->start_handle;
    err_param.op_code = ATT_READ_BY_GROUP_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
#ifdef ATT_SUPPORT_128_BIT_UUID
    value.len = ATT_128_BIT_UUID_SIZE;
#else /* ATT_SUPPORT_128_BIT_UUID */
    value.len = ATT_16_BIT_UUID_SIZE;
#endif /* ATT_SUPPORT_128_BIT_UUID */
    do
    {
        group_list[list.count].range = *range;
        /**
         *  Value length, for the first time is initialized to 128 Bit UUID
         *  Size, thereafter, should be left to what is the first Group value
         *  fetched as in the same response mixed format UUIDs cannot be packed
         */
        retval = BT_gatt_db_get_group_range_val_pair
                 (
                     handle,
                     range,
                     uuid,
                     &group_list[list.count].range,
                     &value
                 );

        if ((API_SUCCESS == retval) ||
           (GATT_DB_MORE_MATCHING_RESULT_FOUND == retval))
        {
            if (0 == list.count)
            {
                list.length = value.len;
            }
            else if (value.len != list.length)
            {
                /* Mixed formatting encountered, stop the search */
                break;
            }

            group_list[list.count].attr_value = value.val;
            list.count++;
        }

        if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
        {
            continue;
        }
        else if (GATT_DB_MORE_MATCHING_RESULT_FOUND != retval)
        {
            /**
             * Spec mandates group end handle of last found group element to be
             * set to ATT_ATTR_END_RANGE to indicate 'Search Complete, however
             * no such mandate specified for Read By Group Type.
             */
#if 0
            if (list.count != 0)
            {
                group_list[list.count-1].range.end_handle =
                    ATT_ATTR_HANDLE_END_RANGE;
            }
#endif /* 0 */
            break;
        }
    } while (list.count < APPL_MAX_GROUP_TYPE_QUERIED);

    if (0 != list.count)
    {
        list.list = group_list;

        retval = BT_att_read_by_group_rsp
                 (
                     handle,
                     &list
                 );
    }
    else
    {
        if (GATT_DB_UNSUPPORTED_GROUP_TYPE == retval)
        {
            err_param.error_code = ATT_UNSUPPORTED_GROUP_TYPE;
        }

        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        IotLogDebug (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    return retval;
}

API_RESULT appl_handle_read_multiple_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_LIST     * list
           )
{
    ATT_ERROR_RSP_PARAM            err_param;
    ATT_READ_MULTIPLE_RSP_PARAM    rsp_param;
    ATT_HANDLE_VALUE_PAIR          pair;
    ATT_VALUE                      value[APPL_MAX_MULTIPLE_READ_COUNT];
    UINT32                         index;
    API_RESULT                     retval;

    index = 0;

    do
    {
        pair.handle = list->handle_list[index];
        retval  = BT_gatt_db_access_handle
                  (
                      &pair,
                      handle,
                      0,
                      (GATT_DB_READ | GATT_DB_PEER_INITIATED)
                  );

        if (API_SUCCESS == retval)
        {
            value[index].val = pair.value.val;
            value[index].len = pair.value.len;
            index++;
        }
        else
        {
            break;
        }
    }while(index < list->list_count);

    if (API_SUCCESS == retval)
    {
        rsp_param.count = (UINT16)index;
        rsp_param.value = value;

         retval = BT_att_read_multiple_rsp
                  (
                      handle,
                      &rsp_param
                  );

         if (API_SUCCESS == retval)
         {
             IotLogDebug (
             "[APPL]: Successfully Sent Read Multiple Response, 0%04X of 0x%04X"
             "values transferred!",rsp_param.actual_count,rsp_param.count);
         }
         else
         {
             IotLogError (
             "[APPL]:[*** ERR ***]: Failed to send read Multiple Response, "
             "Reason 0x%04X",retval);
         }
    }
    else
    {
        err_param.op_code = ATT_READ_MULTIPLE_REQ;
        err_param.handle = list->handle_list[index];

        if (GATT_DB_INVALID_OPERATION == retval)
        {
            err_param.error_code = ATT_READ_NOT_PERMITTED;
        }
        else if (GATT_DB_INVALID_ATTR_HANDLE == retval)
        {
            err_param.error_code = ATT_INVALID_HANDLE;
        }
        else if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
        {
            err_param.error_code = APPL_ATT_TRANSPORT_ACCESS_APPL_ERR;
        }
        else if (GATT_DB_INSUFFICIENT_SECURITY == retval)
        {
            err_param.error_code = ATT_INSUFFICIENT_AUTHENTICATION;
        }
        else if (GATT_DB_INSUFFICIENT_ENC_KEY_SIZE == retval)
        {
            err_param.error_code = ATT_INSUFFICIENT_ENC_KEY_SIZE;

            /* Application Updates to Handle Encryption Key Size Error */
            appl_handle_security_error
            (
                handle,
                retval
            );
        }

        /* Send Error Response */
        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        IotLogDebug (
        "[APPL]: Sent Error Response with result 0x%04X",retval);

    }

    return retval;
}

extern int prvBTGattReadRequest(uint16_t usConnId, uint32_t ulTransId, uint16_t handle, uint16_t offset, uint8_t request_code, uint8_t response_code);
API_RESULT appl_handle_read_request
           (
               /* IN */ ATT_HANDLE    * handle,
               /* IN */ UINT16        attr_handle,
               /* IN */ UINT16        offset,
               /* IN */ UCHAR         direction
           )
{
    ATT_ERROR_RSP_PARAM      err_param;
    ATT_READ_RSP_PARAM       * rsp_param;
    ATT_HANDLE_VALUE_PAIR    pair;
    API_RESULT               retval;
    UINT16                   temp_offset;
    UINT16                   cxn_handle;
    int                      ret;

    err_param.handle = attr_handle;
    temp_offset = offset;

    if (APPL_ATT_INVALID_OFFSET == offset)
    {
        err_param.op_code = ATT_READ_REQ;
        temp_offset = 0;
    }
    else
    {
        err_param.op_code = ATT_READ_BLOB_REQ;
    }
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;

    pair.value.len = APPL_SERVER_BUFFER_SIZE;
    pair.handle = attr_handle;

    retval = appl_get_cxn_handle_from_device_handle(handle->device_id, &cxn_handle);

    if ((API_SUCCESS == retval) && (GATT_DB_PEER_INITIATED == direction))
    {
        uint8_t         request_code;
        uint8_t         response_code;
        request_code = err_param.op_code;
        if (APPL_ATT_INVALID_OFFSET == offset)
        {
            response_code = ATT_READ_RSP;
        }
        else
        {
            response_code = ATT_READ_BLOB_RSP;
        }
        ret = prvBTGattReadRequest(cxn_handle, ( uint32_t ) handle, attr_handle, temp_offset, request_code, response_code);

        if (0 != ret)
        {
            retval = ATT_UNLIKELY_ERROR;
        }
        else
        {
            return API_SUCCESS;
        }
    }
    else
    {
        retval  = BT_gatt_db_access_handle
                (
                    &pair,
                    handle,
                    temp_offset,
                    (GATT_DB_READ | direction)
                );
    }

    if (API_SUCCESS == retval)
    {
        if (GATT_DB_PEER_INITIATED == direction)
        {
            rsp_param = &pair.value;

            if (APPL_ATT_INVALID_OFFSET == offset)
            {
                /* Read Request */
                retval = BT_att_read_rsp
                         (
                              handle,
                              rsp_param
                         );

                IotLogDebug (
                "[APPL]:Read Response sent with result 0x%04X\n",retval);
            }
            else
            {
                /* Read Blob Response */
                retval = BT_att_read_blob_rsp
                         (
                              handle,
                              rsp_param
                         );

                IotLogDebug (
                "[APPL]:Read Blob Response sent with result 0x%04X\n",retval);
            }
        }
        else
        {
            IotLogDebug (
            "[APPL] Value of local handle 0x%04X\n", attr_handle);
            appl_dump_bytes (pair.value.val, pair.value.len);
        }
    }
    else
    {
        if (GATT_DB_PEER_INITIATED == direction)
        {
            if (GATT_DB_INVALID_OPERATION == retval)
            {
                err_param.error_code = ATT_READ_NOT_PERMITTED;
            }
            else if (GATT_DB_INVALID_ATTR_HANDLE == retval)
            {
                err_param.error_code = ATT_INVALID_HANDLE;
            }
            else if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
            {
                err_param.error_code = APPL_ATT_TRANSPORT_ACCESS_APPL_ERR;
            }
            else if (GATT_DB_INSUFFICIENT_SECURITY == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_AUTHENTICATION;
            }
            else if (GATT_DB_INSUFFICIENT_ENC_KEY_SIZE == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_ENC_KEY_SIZE;

                /* Application Updates to Handle Encryption Key Size Error */
                appl_handle_security_error
                (
                    handle,
                    retval
                );
            }
            else if (GATT_DB_INSUFFICIENT_ENCRYPTION == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_ENCRYPTION;
            }
            else if (GATT_DB_HANDLE_NOT_FOUND == retval)
            {
                err_param.error_code = ATT_INVALID_HANDLE;
            }
            else
            {
                err_param.error_code = (UCHAR)(0x00FF & retval);
            }

            if (GATT_DB_DELAYED_RESPONSE != retval)
            {
                /* Send Error Response */
                retval = BT_att_send_error_rsp
                         (
                             handle,
                             &err_param
                         );
                IotLogDebug (
                "[APPL]: Sent Error Response with result 0x%04X",retval);
            }
        }
        else
        {
            IotLogError (
            "[APPL]: Failed to read local handle 0x%04X. Reason 0x%04X\n",
            attr_handle, retval);
        }
    }

    return API_SUCCESS;
}

extern int prvBTGattWriteRequest(uint16_t usConnId, uint32_t ulTransId, uint16_t handle, uint8_t *buffer, uint32_t length, uint16_t offset, uint8_t request_code, uint8_t response_code);
/*
 * direction: GATT_DB_PEER_INITIATED or GATT_DB_LOCALLY_INITIATED
 */
API_RESULT appl_handle_write_request
           (
               /* IN */ ATT_HANDLE    * handle,
               /* IN */ UINT16        attr_handle,
               /* IN */ UINT16        offset,
               /* IN */ ATT_VALUE     * value,
               /* IN */ UCHAR        flags
           )
{
    ATT_ERROR_RSP_PARAM            err_param;
    ATT_HANDLE_VALUE_PAIR          pair;
#ifdef ATT_QUEUED_WRITE_SUPPORT
    ATT_PREPARE_WRITE_RSP_PARAM    prepare_write_rsp;
#endif /* ATT_QUEUED_WRITE_SUPPORT */
    API_RESULT                     retval;
    UINT16                         cxn_handle;
    int                            ret;

    IotLogDebug (
    "[APPL] Write Request from %s for handle 0x%04X of Length %d\n",
    ((GATT_DB_PEER_INITIATED == (flags & GATT_DB_PEER_INITIATED)) ? "Peer" :
    "Local"), attr_handle, value->len);

    appl_dump_bytes (value->val, value->len);

    err_param.handle = attr_handle;
    err_param.op_code = ATT_WRITE_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;

    pair.value = *value;
    pair.handle = attr_handle;

    retval = appl_get_cxn_handle_from_device_handle(handle->device_id, &cxn_handle);

    if ((API_SUCCESS == retval) && (GATT_DB_PEER_INITIATED == (flags & GATT_DB_PEER_INITIATED)) &&
        ((GATT_DB_WRITE_WITHOUT_RSP | GATT_DB_WRITE) & flags))
    {
        uint8_t         request_code;
        uint8_t         response_code;
        if (GATT_DB_WRITE_WITHOUT_RSP & flags)
        {
            request_code = ATT_WRITE_CMD;
            response_code = 0;
        }
        else
        {
            request_code = ATT_WRITE_REQ;
            response_code = ATT_WRITE_RSP;
        }
        ret = prvBTGattWriteRequest(cxn_handle, ( uint32_t )handle, attr_handle, value->val, value->len, offset, request_code, response_code);

        if (0 != ret)
        {
            retval = ATT_UNLIKELY_ERROR;
        }
        else
        {
            return API_SUCCESS;
        }
    }
    else
    {

        retval  = BT_gatt_db_access_handle
                (
                    &pair,
                    handle,
                    offset, /* Offset is zero */
                    (flags)
                );
    }

    if ( GATT_DB_WRITE_WITHOUT_RSP != (GATT_DB_WRITE_WITHOUT_RSP & flags))
    {
        if (API_SUCCESS == retval)
        {
#ifdef ATT_QUEUED_WRITE_SUPPORT
            if (GATT_DB_PREPARE == (flags & GATT_DB_PREPARE))
            {
                err_param.op_code = ATT_PREPARE_WRITE_REQ;
                prepare_write_rsp.offset = offset;
                prepare_write_rsp.handle_value.handle = attr_handle;
                prepare_write_rsp.handle_value.value = (*value);
                retval = appl_att_prepare_write_enqueue (&prepare_write_rsp);
                if (API_SUCCESS == retval)
                {
                    retval = BT_att_send_prepare_write_rsp
                             (
                                  handle,
                                  &prepare_write_rsp
                             );
                }
                else
                {
                    err_param.handle = attr_handle;
                    err_param.error_code = ATT_PREPARE_WRITE_QUEUE_FULL;

                    retval = BT_att_send_error_rsp
                             (
                                 handle,
                                 &err_param
                             );
                }

                IotLogDebug (
                "[APPL]: Prepare Response sent with result 0x%04X",retval);
            }
            else
#endif /* ATT_QUEUED_WRITE_SUPPORT */
            {
                retval = BT_att_write_rsp
                         (
                              handle
                         );

                IotLogDebug (
                "[APPL]: Write Response sent with result 0x%04X\n",retval);
            }

        }
        else
        {
#ifdef ATT_QUEUED_WRITE_SUPPORT
            if (GATT_DB_PREPARE == (flags & GATT_DB_PREPARE))
            {
                err_param.op_code = ATT_PREPARE_WRITE_REQ;
            }
#endif /* ATT_QUEUED_WRITE_SUPPORT */

            if (GATT_DB_INVALID_OPERATION == retval)
            {
                err_param.error_code = ATT_WRITE_NOT_PERMITTED;
            }
            else if (GATT_DB_INSUFFICIENT_BUFFER_LEN == retval)
            {
                err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
            }
            else if ((GATT_DB_INVALID_ATTR_HANDLE == retval) ||
                (GATT_DB_HANDLE_NOT_FOUND == retval))
            {
                err_param.error_code = ATT_INVALID_HANDLE;
            }
            else if (GATT_DB_INVALID_TRANSPORT_ACCESS == retval)
            {
                err_param.error_code = APPL_ATT_TRANSPORT_ACCESS_APPL_ERR;
            }
            else if (GATT_DB_INVALID_OFFSET == retval)
            {
                err_param.error_code = ATT_INVALID_OFFSET;
            }
            else if (GATT_DB_INSUFFICIENT_SECURITY == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_AUTHENTICATION;
            }
            else if (GATT_DB_INSUFFICIENT_ENC_KEY_SIZE == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_ENC_KEY_SIZE;

                /* Application Updates to Handle Encryption Key Size Error */
                appl_handle_security_error
                (
                    handle,
                    retval
                );
            }
            else if (GATT_DB_INSUFFICIENT_ENCRYPTION == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_ENCRYPTION;
            }
            else if (GATT_DB_HANDLE_NOT_FOUND == retval)
            {
                err_param.error_code = ATT_INVALID_HANDLE;
            }
            else
            {
                /* This is for Profile specific errors */
                err_param.error_code = (UCHAR) retval;
            }

            if (GATT_DB_DELAYED_RESPONSE != retval)
            {
                /* Send Error Response */
                retval = BT_att_send_error_rsp
                         (
                             handle,
                             &err_param
                         );

                IotLogDebug (
                "[APPL]: Sent Error Response with result 0x%04X",retval);
            }
        }
    }

    return retval;
}

API_RESULT appl_att_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           )
{
    ATT_ERROR_RSP_PARAM       err_param;
    ATT_XCHG_MTU_RSP_PARAM    xcnhg_rsp_param;
    ATT_HANDLE_RANGE          range;
    ATT_HANDLE_LIST           list;
    ATT_VALUE                 uuid;
    UINT16                    attr_handle_list[APPL_MAX_MULTIPLE_READ_COUNT];
    ATT_ATTR_HANDLE           attr_handle;
    ATT_VALUE                 value;
    APPL_EVENT_PARAM          fsm_param;
    UINT16                    offset;
    UINT16                    mtu;
    API_RESULT                retval;
    UCHAR                     send_err;
#ifdef HPS
    ATT_HANDLE_VALUE_PAIR     pair;
#endif /* HPS */

    IotLogDebug
    ("[ATT]:[0x%02X]: Received event 0x%02X with result 0x%04X\n",
    handle->device_id, att_event, event_result);

    retval = API_SUCCESS;
    err_param.handle = ATT_INVALID_ATTR_HANDLE_VAL;
    err_param.op_code = att_event;
    err_param.error_code = ATT_REQUEST_NOT_SUPPORTED;
    send_err = BT_FALSE;
    APPL_EVENT_PARAM_INIT(&fsm_param);

    /* Update ATT status to be busy */
    BT_status_set_bit (STATUS_BIT_ATT_BUSY, STATUS_BIT_SET);

#ifdef APPL_VALIDATE_ATT_PDU_LEN
    /* Validate ATT PDU Request length */
    if (BT_FALSE != APPL_VALID_ATT_EVENT_FOR_LEN_CHK(att_event))
    {
        retval = appl_validate_att_pdu_req_len(att_event,event_datalen);

        if (API_SUCCESS != retval)
        {
            err_param.error_code = ATT_INVALID_PDU;
            retval = BT_att_send_error_rsp (handle,&err_param);
            return retval;
        }
    }
#endif /* APPL_VALIDATE_ATT_PDU_LEN */

    /**
    * APPL_ENABLE_AUTHORIZATION flag is enabled in this application while running
    * PTS testing related to AUTHORIZATION testcases as this embedded application
    * does not have an MMI or GIOP mappings.
    * APPL_ENABLE_AUTHORIZATION needs to be enabled in Workspace
    */
#ifdef APPL_ENABLE_AUTHORIZATION
    retval = appl_check_attr_dev_authorization
             (
                 handle,
                 att_event,
                 event_result,
                 event_data,
                 event_datalen
             );

    if (API_SUCCESS == retval)
#endif /* APPL_ENABLE_AUTHORIZATION */

    {
    switch (att_event)
    {
    case ATT_CONNECTION_IND:
        if (API_SUCCESS == event_result)
        {
            appl_add_device(handle,&fsm_param.handle);

            extern void prvGattConnect(uint16_t connectHandle, ATT_HANDLE * handle);    /*fix build warning: function declared implicitly.*/
            if (fsm_param.handle < BT_MAX_REMOTE_DEVICES)
            {
                prvGattConnect(APPL_GET_CONNECTION_HANDLE(fsm_param.handle) , handle);
            }

#ifdef APPL_FSM_SUPPORT
            /* Post Disconnection Event */
            appl_fsm_post_event
            (
                ev_appl_transport_connected_ind,
                (void *)(&fsm_param)
            );

            /* Post Transport Operating Inciation */
            /**
             *  Please note that in case of BLE Transport Connect and Transport
             *  Operating mean the same. But this may not be true in case of
             *  BR/EDR or someother transport for which this application could
             *  be used.
             */
            appl_fsm_post_event
            (
                ev_appl_transport_operating_ind,
                (void *)(&fsm_param)
            );
#else
            appl_service_connect(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */
        }
        break;
    case ATT_DISCONNECTION_IND:
        appl_get_handle_from_device_handle (handle->device_id,&fsm_param.handle);

#ifdef APPL_FSM_SUPPORT
        appl_fsm_post_event
        (
            ev_appl_transport_suspend_ind,
            (void *)(&fsm_param)
        );
#else
        appl_service_disconnect(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */

        IotLogDebug(
        "Received ATT Disconnection Indication with result 0x%04X",
        event_result);
        BT_gatt_db_peer_session_shutdown_handler (handle);
        break;

    case ATT_XCHNG_MTU_REQ:
        if (ATT_INVALID_PARAMETER == event_result)
        {
            /* Error Response */
            /**
             *  In case application does not support setting any value other
             *  than default 23, an error respone with request not supported
             *  would be more appropriate. This should be sent irrespective
             *  of what event result is.
             *  But in case it does support the request, but MTU requested is
             *  detected to be invalid, response code, 'Invalid PDU' seems to
             *  be most fitting currently for Error Response.
             */
            err_param.error_code = ATT_INVALID_PDU;
            err_param.handle = 0x0000;
            err_param.op_code = ATT_XCHNG_MTU_REQ;
            retval = BT_att_send_error_rsp (handle,&err_param);
            IotLogError (
            "Invalid MTU Size received in Xchng MTU Req; "
            "Error Response sent with result %04X!\n",
            retval);
        }
        else if (API_SUCCESS == retval)
        {
            BT_UNPACK_LE_2_BYTE(&mtu, event_data);
            IotLogDebug (
            "Received Exchange MTU Request with MTU Size "
            "= 0x%04X!\n", mtu);

            extern uint16_t prvGattMqttExchangeReq(ATT_HANDLE * handle, uint16_t mtu);    /*fix build warning: function declared implicitly.*/
            xcnhg_rsp_param.mtu = prvGattMqttExchangeReq(handle, mtu);;

            retval = BT_att_send_mtu_xcnhg_rsp
                     (
                         handle,
                         &xcnhg_rsp_param
                     );

            IotLogDebug (
            "Sent Response with retval 0x%04X\n", retval);
        }
        break;

    case ATT_FIND_INFO_REQ:
        IotLogDebug (
        "Received Find Info Request with result 0x%04X\n",event_result);

        if (API_SUCCESS == event_result)
        {
            offset= 0;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.start_handle,
                 event_data+offset
            );

            offset += 2;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.end_handle,
                 event_data+offset
            );

            if (ATT_CHECK_VALID_HANDLE_RANGE\
            (range.start_handle, range.end_handle))
            {
                err_param.error_code = ATT_INVALID_HANDLE;
                send_err = BT_TRUE;
            }
            else
            {
                appl_handle_find_info_request (handle,&range);
            }
        }
        break;

    case ATT_FIND_BY_TYPE_VAL_REQ:
        IotLogDebug (
        "Received Find By Type Value Request with result 0x%04X\n",
        event_result);

        if (API_SUCCESS == event_result)
        {
            offset= 0;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.start_handle,
                 event_data+offset
            );

            offset += 2;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.end_handle,
                 event_data+offset
            );

            if (ATT_CHECK_VALID_HANDLE_RANGE\
            (range.start_handle, range.end_handle))
            {
                err_param.error_code = ATT_INVALID_HANDLE;
                send_err = BT_TRUE;
                break;
            }

            offset += 2;
            uuid.len = ATT_16_BIT_UUID_SIZE;
            uuid.val = event_data+offset;

            offset += 2;
            value.len = event_datalen - offset;
            value.val = event_data+offset;

            if ((BT_FALSE != GATT_VERIFY_UUID_VALUE(uuid.val, GATT_PRIMARY_SERVICE_TYPE_OFFSET, ATT_16_BIT_UUID_SIZE)) ||\
               (BT_FALSE != GATT_VERIFY_UUID_VALUE(uuid.val, GATT_PRIMARY_SERVICE_TYPE_OFFSET, ATT_16_BIT_UUID_SIZE)))
            {
                /**
                 * For Service Type, only permissible values are 16 bit UUID or
                 * 128 bit UUID. Hence the following checks
                 */
                if (ATT_128_BIT_UUID_SIZE == value.len)
                {
                    retval =  BT_gatt_db_get_16_bit_uuid(&value, &value);
                    if (retval != API_SUCCESS)
                    {
#ifndef GATT_DB_SUPPORT_128_BIT_UUID
                        err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
                        send_err = BT_TRUE;
#endif /* GATT_DB_SUPPORT_128_BIT_UUID */
                    }
                }
                else if (ATT_16_BIT_UUID_SIZE != value.len)
                {
                    /* Malformed PDU */
                    err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
                    send_err = BT_TRUE;
                }
            }

            if (BT_FALSE == send_err)
            {
                appl_handle_find_by_type_val_request
                (
                    handle,
                    &range,
                    &value,
                    &uuid
                );
            }
        }
        break;
    case ATT_READ_BY_TYPE_REQ:
        if (API_SUCCESS == event_result)
        {
            offset= 0;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.start_handle,
                 event_data+offset
            );

            offset += 2;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.end_handle,
                 event_data+offset
            );

            if (ATT_CHECK_VALID_HANDLE_RANGE\
            (range.start_handle, range.end_handle))
            {
                err_param.error_code = ATT_INVALID_HANDLE;
                err_param.handle = range.start_handle;
                send_err = BT_TRUE;
                break;
            }

            offset += 2;
            uuid.val = event_data+offset;
            uuid.len = event_datalen - offset;

            if (ATT_128_BIT_UUID_SIZE == uuid.len)
            {
                retval = BT_gatt_db_get_16_bit_uuid(&uuid, &uuid);
                if (API_SUCCESS != retval)
                {
#ifndef GATT_DB_SUPPORT_128_BIT_UUID
                    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
                    send_err = BT_TRUE;
#endif /* GATT_DB_SUPPORT_128_BIT_UUID */
                }
            }
            else if (ATT_16_BIT_UUID_SIZE != (event_datalen - offset))
            {
                /* Incorrect PDU */
                err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
                send_err = BT_TRUE;
                retval = API_FAILURE;
            }

            if (BT_FALSE == send_err)
            {
                appl_handle_read_by_type_request
                (
                    handle,
                    &range,
                    &uuid
                );
            }
        }
        break;
    case ATT_READ_REQ:
        if (API_SUCCESS == event_result)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle,
                event_data
            );

            appl_handle_read_request
            (
                 handle,
                 attr_handle,
                 APPL_ATT_INVALID_OFFSET,
                 GATT_DB_PEER_INITIATED
            );
        }
        break;
    case ATT_READ_BLOB_REQ:
        if (API_SUCCESS == event_result)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle,
                event_data
            );

            BT_UNPACK_LE_2_BYTE
            (
                &offset,
                event_data+2
            );
            appl_handle_read_request
            (
                 handle,
                 attr_handle,
                 offset,
                 GATT_DB_PEER_INITIATED
            );
        }
        break;
    case ATT_READ_MULTIPLE_REQ:
    {
        IotLogDebug (
        "Received Read By Multiple Request with result 0x%04X\n",
        event_result);
        list.list_count = 0;
        do
        {
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle_list[list.list_count],
                (event_data + (list.list_count * 2))
            );
            list.list_count++;
        }while (event_datalen > (list.list_count * 2));

        list.handle_list = attr_handle_list;

        appl_handle_read_multiple_request (handle,&list);
    }
    break;
    case ATT_READ_BY_GROUP_REQ:
    {
        IotLogDebug (
        "Received Read By Group Type Request with result 0x%04X\n",
        event_result);
        if (API_SUCCESS == event_result)
        {
            offset= 0;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.start_handle,
                 event_data+offset
            );

            offset += 2;

            BT_UNPACK_LE_2_BYTE
            (
                 &range.end_handle,
                 event_data+offset
            );

            if (ATT_CHECK_VALID_HANDLE_RANGE\
            (range.start_handle, range.end_handle))
            {
                err_param.error_code = ATT_INVALID_HANDLE;
                send_err = BT_TRUE;
                break;
            }

            offset += 2;
            uuid.val = event_data+offset;
            uuid.len = event_datalen - offset;

            if (ATT_128_BIT_UUID_SIZE == uuid.len)
            {
                retval = BT_gatt_db_get_16_bit_uuid(&uuid, &uuid);
                /**
                 *  Note, 128-bit UUID Group Types are not supported by the
                 *  Data Base, hence Queries with Non BT Space 128 bit will
                 *  be responded with error
                 */
                if (API_SUCCESS != retval)
                {
                    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
                    send_err = BT_TRUE;
                }
            }
            else if (ATT_16_BIT_UUID_SIZE != (event_datalen - offset))
            {
                /* Incorrect PDU */
                err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
                send_err = BT_TRUE;
            }
            else
            {
                /* Do Nothing */
            }

            if (API_SUCCESS == retval)
            {
                appl_handle_read_by_group_type_request
                (
                    handle,
                    &range,
                    &uuid
                );
            }
        }
    }
    break;
    case ATT_WRITE_REQ:
        if (API_SUCCESS == event_result)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle,
                event_data
            );

            value.len = event_datalen - 2;
            value.val = appl_att_buffer;
            value.actual_len = 0U;
            BT_mem_copy
            (
                value.val,
                event_data+2,
                value.len
            );
#ifdef HPS
            pair.handle = attr_handle;
            pair.value = value;

            appl_hps_write_char_value(&pair, 0);
#endif /* HPS */
            appl_handle_write_request
            (
                handle,
                attr_handle,
                0,
                &value,
                (GATT_DB_PEER_INITIATED | GATT_DB_WRITE)
            );
        }
        break;

    case ATT_WRITE_CMD:
        if (API_SUCCESS == event_result)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle,
                event_data
            );

            value.len = event_datalen - 2;
            value.val = appl_att_buffer;
            value.actual_len = 0U;
            BT_mem_copy
            (
                value.val,
                event_data+2,
                value.len
            );

            appl_handle_write_request
            (
                handle,
                attr_handle,
                0,
                &value,
                ( GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP)
            );

            IotLogDebug (
            "[APPL] Received Write Command for Handle 0x%04X of Length %d\n",
            attr_handle, event_datalen - 2);

            appl_dump_bytes (event_data + 2, event_datalen - 2);
        }
        break;
#ifdef ATT_QUEUED_WRITE_SUPPORT
    case ATT_PREPARE_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE
        (
            &attr_handle,
            event_data
        );

        BT_UNPACK_LE_2_BYTE
        (
            &offset,
            event_data+2
        );

        value.len = event_datalen - 4;
        value.val = appl_att_buffer;
        value.actual_len = 0U;
        BT_mem_copy
        (
            value.val,
            event_data+4,
            value.len
        );

        appl_handle_write_request
        (
            handle,
            attr_handle,
            offset,
            &value,
            ( GATT_DB_PEER_INITIATED | GATT_DB_UPDATE | GATT_DB_PREPARE)
        );
    }
    break;
    case ATT_EXECUTE_WRITE_REQ:
        if (NULL != event_data)
        {
            IotLogDebug  (
            "[APPL]: ATT Execute Write Param 0x%02X",event_data[0]);

            if (0x02 > event_data[0])
            {
                appl_execute_write_handler[event_data[0]](handle);
            }
        }
        else
        {
            /* TBD: Verify appropriate code for this */
            err_param.error_code = ATT_INVALID_PDU;
            send_err = BT_TRUE;
        }
        break;
#else /* ATT_QUEUED_WRITE_SUPPORT */
    case ATT_PREPARE_WRITE_REQ: /* Fall through */
    case ATT_EXECUTE_WRITE_REQ: /* Fall through */
        appl_handle_unsupported_op_code (handle,att_event);
        break;
#endif /* ATT_QUEUED_WRITE_SUPPORT */

    case ATT_HANDLE_VALUE_CNF:
        appl_hvc_flag = BT_FALSE;
        break;

    case ATT_HANDLE_VALUE_NTF_TX_COMPLETE:
        IotLogInfo("Received HVN Tx Complete (Locally generated)\n");
        appl_dump_bytes(event_data, event_datalen);
        break;

    case ATT_SIGNED_WRITE_CMD:
        appl_dump_bytes(event_data, event_datalen);

#ifdef SMP_DATA_SIGNING
        {
            s_data = BT_alloc_mem (event_datalen + 1);
            s_datalen = event_datalen + 1;

            s_data[0] = ATT_SIGNED_WRITE_CMD;
            BT_mem_copy (&s_data[1], event_data, event_datalen);

            appl_signed_write_server_handle = *handle;

            appl_smp_verify_sign_data (s_data, s_datalen);
        }
#else /* SMP_DATA_SIGNING */
        appl_handle_unsupported_op_code (handle,att_event);
#endif /* SMP_DATA_SIGNING */

        break;

    case ATT_UNKNOWN_PDU_IND:
        /**
         * Check if request, then send an error response. Else, drop.
         * Assumption here is that future Client Initiated Request and server
         * response will follow the current odd & even pattern.
         * Please note that currently handle value confirmation is also an odd
         * value, which then belongs to a request category, so any such future
         * client initiated confirmations will be responded with error, which
         * may be considered a violation as application would be responding to
         * a response.
         */
        if (BT_FALSE != APPL_CHECK_IF_ATT_REQUEST(event_data[0]))
        {
            appl_handle_unsupported_op_code (handle,event_data[0]);
        }
        else
        {
            IotLogError (
            "[APPL]:[*** ERR ****]: Received Unknown PDU (Response Op code "
            "0x%02X?) from Client!\n",event_data[0]);
            err_param.error_code = ATT_INVALID_PDU;
            err_param.op_code = event_data[0];
            err_param.handle = ATT_INVALID_ATTR_HANDLE_VAL;

            send_err = BT_TRUE;
        }
        break;
    default:
        break;
    }
    }

    if (BT_FALSE != send_err)
    {
         retval = BT_att_send_error_rsp
                  (
                      handle,
                      &err_param
                  );
         IotLogError (
         "[APPL]: Error Response sent with result 0x%04X\n",retval);
    }

    /* Update ATT status to be free */
    BT_status_set_bit (STATUS_BIT_ATT_BUSY, STATUS_BIT_RESET);

    return API_SUCCESS;
}

#ifdef SMP_DATA_SIGNING
void appl_gatt_signing_verification_complete
     (
         API_RESULT   status,
         UCHAR      * data,
         UINT16       datalen
     )
{
    ATT_ATTR_HANDLE attr_handle;
    ATT_VALUE value;

    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

    if (API_SUCCESS == status)
    {
        /* Extract handle */
        BT_UNPACK_LE_2_BYTE
        (
            &attr_handle,
            (s_data + 1)
        );

        /* Extract value */
        /**
         * ATT SIGNED WRITE DATA comprises of
         *    1. ATT_SIGNED_WRITE_CMD OPCODE    (1 byte)
         *    2. Attribute Handle               (2 bytes)
         *    3. Attribute Value                (N bytes)
         *    4. Signed write Counter           (4 bytes)
         *    5. MAC                            (8 bytes)
         *    thus Attribute Value len(N) = Total Len - (1 + 2 + 4 + 8)
         */
        value.len = (s_datalen - (1 + 2 + sizeof(UINT32) + SMP_MAC_SIZE));
        value.val = appl_att_buffer;
        value.actual_len = 0U;

        BT_mem_copy
        (
            value.val,
            (s_data + 3),
            value.len
        );

        appl_handle_write_request
        (
            &appl_signed_write_server_handle,
            attr_handle,
            0,
            &value,
            (GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP)
        );
    }
    else
    {
        /* Data signing verification procedure failed */
        /* Application Updates to Handle Encryption Key Size Error */
        /* Assigning API_FAILURE explicitly as the incoming status is only
         * UCHAR which is converted to API_RESULT in the calling function.
         */
        appl_handle_security_error
        (
            &appl_signed_write_server_handle,
            API_FAILURE
        );
    }

    if (NULL != s_data)
    {
        BT_free_mem (s_data);
        s_data = NULL;
    }
}
#endif /* SMP_DATA_SIGNING */


#ifdef APPL_VALIDATE_ATT_PDU_LEN
API_RESULT appl_validate_att_pdu_req_len
           (
               UCHAR     att_event,
               UINT16    event_datalen
           )
{
    API_RESULT    retval;
    UINT32        index;

    retval = API_FAILURE;

    for (index = 0; index < APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK; index++)
    {
        if (appl_valid_att_pdu_len[index].att_event == att_event)
        {

            if ((appl_valid_att_pdu_len[index].len1 == event_datalen) ||
                ((appl_valid_att_pdu_len[index].len2 == event_datalen) &&
                (appl_valid_att_pdu_len[index].len2 != APPL_ATT_INVALID_LEN)))
            {
                retval = API_SUCCESS;
            }
            else
            {
                break;
            }
        }
    }
    return retval;
}
#endif /* APPL_VALIDATE_ATT_PDU_LEN */

void main_gatt_server_operations(void)
{
    unsigned int  choice;
    unsigned int  read;
    API_RESULT    retval;

    BT_LOOP_FOREVER()
    {
        IotLogInfo ("%s \n", gatt_server_menu);
        (void)gatt_server_menu;    /*fix build warning: set but never used.*/
        IotLogInfo ("Enter you choice : ");
        CONSOLE_IN ("%d", &choice);

        switch(choice)
        {
        case 0:
            return;

        case 1:
            break;

        case 2:
            IotLogInfo ("Selection Option to Enable Disable User Level Authorization: \n");
            IotLogInfo ("Enter 1. Enable, 0. Disable:\n");
            CONSOLE_IN ("%x", &choice);

            (choice == 1) ?                                           \
            APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, BT_TRUE) : \
            APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, BT_FALSE);
            break;

        case 3:
            {
                BT_DEVICE_ADDR         device_addr;
                DEVICE_HANDLE          dq_handle;

                IotLogInfo("Enter BD_ADDR : ");
                appl_get_bd_addr(device_addr.addr);

                IotLogInfo("\n0 - Public\n1 - Random\n");
                IotLogInfo("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read);
                device_addr.type = (UCHAR) read;

                /* Fetch the Device Handle */
                retval = device_queue_search_le_remote_addr
                         (
                             &dq_handle,
                             &device_addr
                         );

                if (API_SUCCESS == retval)
                {
                    retval = appl_send_serv_changed_ind(dq_handle);

                    if (API_SUCCESS == retval)
                    {
                        IotLogInfo("Send Service changed Indication: \n");
                    }
                    else
                    {
                        IotLogInfo("Send Service changed Indication Failed 0x%04X \n", retval);
                    }
                }
            }
            break;

        case 10:
#ifdef APPL_MENU_OPS
            main_service_menu_operations();
#else /* APPL_MENU_OPS */
            IotLogInfo("'APPL_MENU_OPS' not defined!\n");
#endif /* APPL_MENU_OPS */
        default:
            break;
        }
    }
}
#endif /* ATT */

