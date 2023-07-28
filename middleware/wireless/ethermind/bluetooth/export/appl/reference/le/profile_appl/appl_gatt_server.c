
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

#include "appl_service.h"
#include "BT_status.h"

#ifdef APPL_MENU_OPS
#include "appl_service_mops.h"
#endif /* APPL_MENU_OPS */

#ifdef ATT

#ifdef APPL_LIMIT_LOGS
#ifdef APPL_TRC
#undef APPL_TRC
#define APPL_TRC(...)
#endif /* APPL_TRC */

#ifdef CONSOLE_OUT
#undef CONSOLE_OUT
#define CONSOLE_OUT(...)
#endif /* CONSOLE_OUT */
#endif /* APPL_LIMIT_LOGS */

/* --------------------------------------------- External Global Variables */
extern UCHAR g_appl_bond_del_flag;

#ifdef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
static APPL_EVENT_HANDLER_CB appl_event_hndlr_cb = NULL;
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Application MTU configuration. Initializing the default MTU here */
static UINT16 appl_gatt_mtu = APPL_ATT_MTU;

static const UCHAR *gatt_server_menu = (UCHAR *) \

"--------------------------------------------\n\
             GATT SERVER MENU\n\
--------------------------------------------\n\
  0 - Exit \n\
  1 - Refresh \n\
  2 - Grant/Deny Authorization\n\
  3 - Send Service Changed HVI\n\
  4 - Update Battery Level\n\
  5 - Send Multiple HVN sample\n\
 10 - Service Menu Operations\n\
100 - Enable/Disable Application Event Trace\n\
Your Option ? ";

#ifdef SMP_DATA_SIGNING
static UCHAR * s_data;
static UINT16 s_datalen;
static ATT_HANDLE appl_signed_write_server_handle;
#endif /* SMP_DATA_SIGNING */

#ifdef ATT_QUEUED_WRITE_SUPPORT
#define APPL_MAX_WRITELONG_SIZE        /* 128U */ 256U
typedef struct _APPL_WRITELONG_ELEMENT
{
    /* Value that is prepared */
    UCHAR value[APPL_MAX_WRITELONG_SIZE];

    /* Current length */
    UINT16 length;

    /* Attribute handle for the value */
    ATT_ATTR_HANDLE handle;

    /* Offset */
    UINT16 offset;

} APPL_WRITELONG_ELEMENT;

API_RESULT appl_att_prepare_write_cancel (ATT_HANDLE   *handle);
API_RESULT appl_att_prepare_write_execute (ATT_HANDLE   *handle);
API_RESULT appl_att_prepare_write_queue_init (void);

static APPL_WRITELONG_ELEMENT appl_att_write_queue[APPL_ATT_WRITE_QUEUE_SIZE];
static APPL_ATT_EXECUTE_WRITE_HANDLER appl_execute_write_handler[2U] =
                  {
                      appl_att_prepare_write_cancel,
                      appl_att_prepare_write_execute
                  };
#endif /* ATT_QUEUED_WRITE_SUPPORT */

#ifdef APPL_VALIDATE_ATT_PDU_LEN
static APPL_VALID_ATT_PDU_LEN appl_valid_att_pdu_len
                              [APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK] =
{
    {
        ATT_XCHNG_MTU_REQ,
        APPL_ATT_XCHNG_MTU_REQ_LEN,
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
#define APPL_ATT_TRANSPORT_ACCESS_APPL_ERR       0x90U

#ifdef APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION
/* Global to control enable disable of application event trace prints */
static UCHAR appl_gatt_server_evt_trc = BT_TRUE;
#endif /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */

#define APPL_ATT_MAX_MULT_HVN_COUNT     4

/* --------------------------------------------- Functions */
#ifdef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
API_RESULT appl_register_cb(APPL_EVENT_HANDLER_CB appl_pams_cb)
{
    API_RESULT retval;

    retval = API_FAILURE;

    if (NULL != appl_pams_cb)
    {
        appl_event_hndlr_cb = appl_pams_cb;

        retval = API_SUCCESS;
    }

    return retval;
}
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */

#ifdef APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION
void appl_set_gatt_server_evt_trc(UCHAR flag)
{
    appl_gatt_server_evt_trc =
        (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
}
#endif /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */

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

    index = 0U;

    do
    {
        APPL_TRC (
        "[APPL]: Initializing Queue Element 0x%02lX\n",index);

        APPL_ATT_PREPARE_QUEUE_INIT (index);
        index++;
    } while (index < APPL_ATT_WRITE_QUEUE_SIZE);

    return API_SUCCESS;
}

APPL_WRITELONG_ELEMENT* appl_alloc_writelong_element(ATT_ATTR_HANDLE handle, UINT16 *id)
{
    APPL_WRITELONG_ELEMENT *elt;
    UINT16 index, free_index;

    free_index = APPL_ATT_WRITE_QUEUE_SIZE;
    elt = NULL;
    *id = APPL_ATT_WRITE_QUEUE_SIZE;

    for (index = 0; index < APPL_ATT_WRITE_QUEUE_SIZE; index++)
    {
        if (handle == appl_att_write_queue[index].handle)
        {
            elt = &appl_att_write_queue[index];
            *id = index;
            break;
        }
        else if ((APPL_ATT_WRITE_QUEUE_SIZE == free_index) &&
                 (0x0000 == appl_att_write_queue[index].handle))
        {
            free_index = index;
        }
    }

    if (APPL_ATT_WRITE_QUEUE_SIZE == index)
    {
        if (APPL_ATT_WRITE_QUEUE_SIZE != free_index)
        {
            elt = &appl_att_write_queue[free_index];
            *id = free_index;
        }
    }

    return elt;
}

API_RESULT appl_att_prepare_write_enqueue (ATT_HANDLE_VALUE_OFFSET_PARAM * param)
{
    APPL_WRITELONG_ELEMENT *elt;
    API_RESULT retval;
    UINT16 index;

    retval = API_FAILURE;

    /* Search or allocate a WriteQueue element for the handle */
    elt = appl_alloc_writelong_element(param->handle_value.handle, &index);

    if (NULL == elt)
    {
        return retval;
    }

    APPL_TRC(
    "[APPL]: Queue index: 0x%02X Offset: 0x%04X\n", index, elt->length);

    /* Store the provided Offset value */
    elt->offset = param->offset;

    if (APPL_MAX_WRITELONG_SIZE > (elt->length + param->handle_value.value.len))
    {
        APPL_TRC(
        "[APPL]: Copy to Element\n");

        elt->handle = param->handle_value.handle;

        BT_mem_copy
        (
            &(elt->value[elt->length]),
            param->handle_value.value.val,
            param->handle_value.value.len
        );

        elt->length += param->handle_value.value.len;
        retval = API_SUCCESS;
    }
    else
    {
        APPL_TRC (
        "[APPL]: Cannot Enqueue value!\n");
    }

    return retval;
}

API_RESULT appl_att_prepare_write_cancel (ATT_HANDLE   * handle)
{
    API_RESULT retval;

    (BT_IGNORE_RETURN_VALUE) appl_att_prepare_write_queue_init ();

    /* Send response */
    retval = BT_att_send_execute_write_rsp (handle);

    APPL_TRC (
    "[APPL]: Sent Execute Write Response with result 0x%04X\n",retval);

    return retval;
}

API_RESULT appl_att_prepare_write_execute (ATT_HANDLE   * handle)
{
    UINT32                            index;
    API_RESULT                        retval;
    ATT_ERROR_RSP_PARAM               err_param;
    ATT_HANDLE_VALUE_PAIR             pair;
    UCHAR                             response;

    response = BT_TRUE;

    /* MISRA C - 2012 Rule 2.2 */
    /* retval = API_FAILURE; */
    err_param.op_code    = ATT_EXECUTE_WRITE_REQ;

    /* MISRA C - 2012 Rule 2.2 */
    /* err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND; */

    retval = API_FAILURE;
    index = 0U;

    do
    {
        if (0U != appl_att_write_queue[index].length)
        {
            APPL_TRC (
            "[APPL]: Executing Element 0x%02X Handle 0x%04X, Value Length 0x%04X\n",
            index, appl_att_write_queue[index].handle, appl_att_write_queue[index].length);

            pair.handle = appl_att_write_queue[index].handle;
            pair.value.val = appl_att_write_queue[index].value;
            pair.value.len = appl_att_write_queue[index].length;

            retval = BT_gatt_db_access_handle
                     (
                          &pair,
                          handle,
                          appl_att_write_queue[index].offset,
                          (GATT_DB_EXECUTE | GATT_DB_PEER_INITIATED)
                     );

            if (API_SUCCESS != retval)
            {
                if (GATT_DB_DONOT_RESPOND == retval)
                {
                    response = BT_FALSE;
                }
                else
                {
                    APPL_TRC(
                    "[APPL]: Failed to Execute write on handle 0x%04X, reason 0x%04X\n",
                    appl_att_write_queue[index].handle, retval);

                    break;
                }
            }
        }

        index++;

    } while (index < APPL_ATT_WRITE_QUEUE_SIZE);

    if (BT_TRUE == response)
    {
        /* TBD: What happens if Write on or more but not all attributes fails? */
        if (API_SUCCESS == retval)
        {
            retval = BT_att_send_execute_write_rsp
                     (
                          handle
                     );
            APPL_TRC (
            "[APPL]: Execute Write Response sent with result 0x%04X\n",retval);
        }
        else
        {
            err_param.handle = appl_att_write_queue[index].handle;
            if (GATT_DB_INVALID_OFFSET == retval)
            {
                err_param.error_code = ATT_INVALID_OFFSET;
            }
            else if (GATT_DB_INSUFFICIENT_BUFFER_LEN == retval)
            {
                err_param.error_code = ATT_INVALID_ATTRIBUTE_LEN;
            }
            else if (GATT_DB_INVALID_OPERATION == retval)
            {
                err_param.error_code = ATT_WRITE_NOT_PERMITTED;
            }
            else
            {
                err_param.error_code = (UCHAR)(0x00FFU & retval);
            }

            retval = BT_att_send_error_rsp
                        (
                            handle,
                            &err_param
                        );
            APPL_TRC (
            "[APPL]: Sent Error Response with result 0x%04X\n",retval);
        }
    }

    (BT_IGNORE_RETURN_VALUE) appl_att_prepare_write_queue_init ();

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
        APPL_ERR (
        "[APPL]:[*** ERR ***]: Failed to send error response, reason 0x%04X\n",
        retval);
    }
    else
    {
        APPL_ERR (
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

    list.count = 0U;

    err_param.op_code = ATT_FIND_BY_TYPE_VAL_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;
    err_param.handle = range->start_handle;

    APPL_TRC(
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
#ifdef APPL_LE_HANDLE_BT_ERRATA_4062
            if (list.count != 0U)
            {
                group_list[list.count-1U].end_handle = ATT_ATTR_HANDLE_END_RANGE;
            }
#endif /* APPL_LE_HANDLE_BT_ERRATA_4062 */
            break;
        }
    } while ((list.count < APPL_MAX_GROUP_TYPE_QUERIED) &&
            (range->end_handle >= range->start_handle));

    if (0U != list.count)
    {
        list.range = group_list;
        APPL_TRC(
        "[APPL]: Number of occurrences of UUID 0x%04X = 0x%04X\n",
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
        APPL_TRC (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    APPL_TRC(
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

    APPL_TRC(
    "[APPL]: >>> Handling Find Info request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);


    index = 0U;
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
            APPL_TRC (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> UUID Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);
#else /* ATT_SUPPORT_128_BIT_UUID */
            APPL_TRC (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> UUID Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);
#endif /* ATT_SUPPORT_128_BIT_UUID */
            index++;
        }

        if (GATT_DB_INCORRECT_UUID_FRMT == retval)
        {
            if (0U == index)
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
            APPL_TRC (
            "[APPL]:Search Complete, #Handle UUID List found = 0x%02lX\n",
            index);
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }while (index < APPL_MAX_HNDL_UUID_LIST_SIZE);

    if (0U != index)
    {
        param.handle_value_list.list_count = (UINT16)index;

        for (index = 0U; index < param.handle_value_list.list_count; index++)
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

        param.handle_value_list.list = &handle_uuid_list[0U];

        retval = BT_att_send_find_info_rsp (handle,&param);
        APPL_TRC (
        "[APPL]: Sent Find Info Response with result 0x%04X\n",retval);
    }
    else
    {
        retval = BT_att_send_error_rsp
                 (
                     handle,
                     &err_param
                 );
        APPL_TRC (
        "[APPL]: Sent Error Response with result 0x%04X\n",retval);
    }

    APPL_TRC(
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

    APPL_TRC(
    "[APPL]: >>> Handling Read By Type request, Range 0x%04X-0x%04X\n",
    range->start_handle,range->end_handle);

    index = 0U;
    len = 0U;
    err_param.handle = range->start_handle;
    err_param.op_code = ATT_READ_BY_TYPE_REQ;
    err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND;

    BT_mem_set(handle_value_list, 0x0, sizeof(handle_value_list));

    do
    {
        handle_value_list[index].value.len = (APPL_SERVER_BUFFER_SIZE - len);

        /**
         * The "Flags" Type to be used here is "Read By Type".
         * This will enable in identifying if a Characteristic is
         * attempted to be read using normal ATT Read/ReadLong procedure
         * through the Attribute Handle or if a Characteristic is
         * attempted to be read using the UUID in a given Range.
         */
        retval = BT_gatt_db_fetch_handle_value_pair
                 (
                     handle,
                     range,
                     uuid,
                     &handle_value_list[index],
                     (GATT_DB_READ_BY_TYPE | GATT_DB_PEER_INITIATED)
                 );

        if ((API_SUCCESS == retval) ||
           (GATT_DB_MORE_MATCHING_RESULT_FOUND == retval))
        {
            APPL_TRC (
            "[APPL]:[0x%04lX]: Handle 0x%04X -> Value Len 0x%04X\n",index,
            handle_value_list[index].handle,
            handle_value_list[index].value.len);

            if (0U == index)
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
            APPL_TRC (
            "[APPL]:Search Complete, #Handle UUID List found = 0x%02lX\n",
            index);
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }while (index < APPL_MAX_HNDL_VALUE_SIZE);

    if (0U != index)
    {
        param.handle_value_list = handle_value_list;
        param.count = (UINT16)index;

        retval = BT_att_read_by_type_rsp (handle,&param);
        APPL_TRC (
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
            (BT_IGNORE_RETURN_VALUE) appl_handle_security_error
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
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((GATT_DB_DELAYED_RESPONSE  != retval) &&
            (GATT_DB_DONOT_RESPOND     != retval) &&
            (GATT_DB_ALREADY_RESPONDED != retval))
        {
            retval = BT_att_send_error_rsp
                     (
                         handle,
                         &err_param
                     );
            APPL_TRC(
            "[APPL]: Sent Error Response with result 0x%04X\n", retval);
        }
    }

    APPL_TRC(
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

    list.count = 0U;

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
            if (0U == list.count)
            {
                list.length = value.len;
            }
            else if (value.len != list.length)
            {
                /* Mixed formatting encountered, stop the search */
                break;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
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
#ifdef APPL_LE_HANDLE_BT_ERRATA_4062
            if (list.count != 0U)
            {
                group_list[list.count-1U].range.end_handle =
                    ATT_ATTR_HANDLE_END_RANGE;
            }
#endif /* APPL_LE_HANDLE_BT_ERRATA_4062 */
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    } while (list.count < APPL_MAX_GROUP_TYPE_QUERIED);

    if (0U != list.count)
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
        APPL_TRC (
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

    index = 0U;

    BT_mem_set(&pair, 0x0, sizeof(pair));

    do
    {
        pair.handle = list->handle_list[index];
        retval  = BT_gatt_db_access_handle
                  (
                      &pair,
                      handle,
                      0U,
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
             APPL_TRC (
             "[APPL]: Successfully Sent Read Multiple Response, 0%04X of "
             "0x%04X values transferred!\n",rsp_param.actual_count,rsp_param.count);
         }
         else
         {
             APPL_ERR (
             "[APPL]:[*** ERR ***]: Failed to send read Multiple Response, "
             "Reason 0x%04X\n",retval);
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
            (BT_IGNORE_RETURN_VALUE) appl_handle_security_error
            (
                handle,
                retval
            );
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((GATT_DB_DELAYED_RESPONSE  != retval) &&
            (GATT_DB_DONOT_RESPOND     != retval) &&
            (GATT_DB_ALREADY_RESPONDED != retval))
        {
            /* Send Error Response */
            retval = BT_att_send_error_rsp
                        (
                            handle,
                            &err_param
                        );

            APPL_TRC (
            "[APPL]: Sent Error Response with result 0x%04X\n",retval);
        }
    }

    return retval;
}

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
API_RESULT appl_handle_read_multiple_var_len_request
           (
                /* IN */ ATT_HANDLE          * handle,
                /* IN */ ATT_HANDLE_LIST     * list
           )
{
    ATT_ERROR_RSP_PARAM            err_param;
    ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP_PARAM    rsp_param;
    ATT_HANDLE_VALUE_PAIR          pair;
    ATT_VALUE                      value[APPL_MAX_MULTIPLE_READ_COUNT];
    UINT32                         index;
    API_RESULT                     retval;

    index = 0;

    BT_mem_set(&pair, 0x0, sizeof(pair));

    do
    {
        pair.handle = list->handle_list[index];
        retval = BT_gatt_db_access_handle
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
    } while (index < list->list_count);


    if (API_SUCCESS == retval)
    {
        rsp_param.count = (UINT16)index;
        rsp_param.value = value;

         retval = BT_att_send_read_multiple_variable_length_rsp
                  (
                      handle,
                      &rsp_param
                  );

         if (API_SUCCESS == retval)
         {
             APPL_TRC (
             "[APPL]: Successfully Sent Read Multiple Variable Length Response, 0%04X of 0x%04X"
             "values transferred!", rsp_param.actual_count, rsp_param.count);
         }
         else
         {
             APPL_ERR (
             "[APPL]:[*** ERR ***]: Failed to send read Multiple Variable Length Response, "
             "Reason 0x%04X", retval);
         }
    }
    else
    {
        err_param.op_code = ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ;
        err_param.handle = list->handle_list[index];

        /* Initializing to this Value */
        /* MISRA C - 2012 Rule 2.2 */
        /* err_param.error_code = ATT_UNLIKELY_ERROR; */

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
            (BT_IGNORE_RETURN_VALUE)appl_handle_security_error
            (
                handle,
                retval
            );
        }
        else
        {
            err_param.error_code = (UCHAR)(0x00FF & retval);
        }

        if ((GATT_DB_DELAYED_RESPONSE  != retval) &&
            (GATT_DB_DONOT_RESPOND     != retval) &&
            (GATT_DB_ALREADY_RESPONDED != retval))
        {
            /* Send Error Response */
            retval = BT_att_send_error_rsp
            (
                handle,
                &err_param
            );

            APPL_TRC(
            "[APPL]: Sent Error Response with result 0x%04X\n",retval);
        }
    }

    return retval;
}
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

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
    UCHAR                    flags;

    err_param.handle = attr_handle;
    temp_offset = offset;

    BT_mem_set(&pair, 0x0, sizeof(pair));

    if (APPL_ATT_INVALID_OFFSET == offset)
    {
        err_param.op_code = ATT_READ_REQ;
        temp_offset = 0U;
    }
    else
    {
        err_param.op_code = ATT_READ_BLOB_REQ;
    }

    /* MISRA C - 2012 Rule 2.2 */
    /* err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND; */

    pair.value.len = APPL_SERVER_BUFFER_SIZE;
    pair.handle = attr_handle;

    /* Assign the access_type flag based on the Opcode */
    flags = (ATT_READ_REQ == err_param.op_code) ? GATT_DB_READ : GATT_DB_READ_BLOB;

    retval  = BT_gatt_db_access_handle
              (
                  &pair,
                  handle,
                  temp_offset,
                  (flags | direction)
              );

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

                APPL_TRC (
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

                APPL_TRC (
                "[APPL]:Read Blob Response sent with result 0x%04X\n",retval);
            }
        }
        else
        {
            APPL_TRC (
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
                (BT_IGNORE_RETURN_VALUE) appl_handle_security_error
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
                err_param.error_code = (UCHAR)(0x00FFU & retval);
            }

            if ((GATT_DB_DELAYED_RESPONSE  != retval) &&
                (GATT_DB_DONOT_RESPOND     != retval) &&
                (GATT_DB_ALREADY_RESPONDED != retval))
            {
                /* Send Error Response */
                retval = BT_att_send_error_rsp
                         (
                             handle,
                             &err_param
                         );
                APPL_TRC (
                "[APPL]: Sent Error Response with result 0x%04X\n",retval);
            }
        }
        else
        {
            APPL_ERR (
            "[APPL]: Failed to read local handle 0x%04X. Reason 0x%04X\n",
            attr_handle, retval);
        }
    }

    return API_SUCCESS;
}


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

    APPL_TRC (
    "[APPL] Write Request from %s for handle 0x%04X of Length %d, Offset %d\n",
    ((GATT_DB_PEER_INITIATED == (flags & GATT_DB_PEER_INITIATED)) ? "Peer" :
    "Local"), attr_handle, value->len, offset);

    appl_dump_bytes (value->val, value->len);

    err_param.handle = attr_handle;
    err_param.op_code = ATT_WRITE_REQ;

    /* MISRA C - 2012 Rule 2.2 */
    /* err_param.error_code = ATT_ATTRIBUTE_NOT_FOUND; */

    pair.value = *value;
    pair.handle = attr_handle;

    retval  = BT_gatt_db_access_handle
              (
                  &pair,
                  handle,
                  offset,
                  (flags)
              );

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

                APPL_TRC (
                "[APPL]: Prepare Response sent with result 0x%04X\n",retval);
            }
            else
#endif /* ATT_QUEUED_WRITE_SUPPORT */
            {
                retval = BT_att_write_rsp
                         (
                              handle
                         );

                APPL_TRC (
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
                (BT_IGNORE_RETURN_VALUE) appl_handle_security_error
                (
                    handle,
                    retval
                );
            }
            else if (GATT_DB_INSUFFICIENT_ENCRYPTION == retval)
            {
                err_param.error_code = ATT_INSUFFICIENT_ENCRYPTION;
            }
            else
            {
                /* This is for Profile specific errors */
                err_param.error_code = (UCHAR) retval;
            }

            if ((GATT_DB_DELAYED_RESPONSE  != retval) &&
                (GATT_DB_DONOT_RESPOND     != retval) &&
                (GATT_DB_ALREADY_RESPONDED != retval))
            {
                /* Send Error Response */
                retval = BT_att_send_error_rsp
                         (
                             handle,
                             &err_param
                         );

                APPL_TRC (
                "[APPL]: Sent Error Response with result 0x%04X\n",retval);
            }
        }
    }

    return retval;
}

void appl_parse_confirmation_data(ATT_HANDLE * handle, UINT16 evt_result)
{
    appl_service_parse_hv_cnf(handle, evt_result);
}

void appl_parse_ntf_tx_complete(ATT_HANDLE * handle, UCHAR * data, UINT16 datalen)
{
    appl_service_parse_ntf_tx_complete(handle, data, datalen);
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
#ifdef ATT_ON_BR_EDR_SUPPORT
    DEVICE_LINK_TYPE          link_type;
#endif /* ATT_ON_BR_EDR_SUPPORT */

#ifdef APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION
    if (BT_TRUE == appl_gatt_server_evt_trc)
#endif /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */
    {
        APPL_TRC(
        "[ATT]:[0x%02X]:[0x%02X]: Received event 0x%02X with result 0x%04X\n",
        handle->device_id, handle->att_id, att_event, event_result);
    }

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&value, 0, sizeof(ATT_VALUE));

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
     * Check if NULL data is received for event other than connection
     * or disconnection.
     */
    if ((NULL == event_data) &&
        ((ATT_CONNECTION_IND != att_event) &&
         (ATT_DISCONNECTION_IND != att_event)))
    {
        APPL_ERR("Received NULL event data\n");
        retval = API_FAILURE; /* return API_FAILURE; */

        /* Check for Event Result */
        if (ATT_RESPONSE_TIMED_OUT == event_result)
        {
            /**
             * For the server side, If Response timeout is received
             * for any of the server initiated procedures which is waiting
             * for the corresponding response from remote side, then
             * handle the same and feed it back to any upper layer applications
             * which needs to take any special action.
             */
            switch (att_event)
            {
            case ATT_HANDLE_VALUE_CNF:
                appl_parse_confirmation_data (handle, event_result);
                break;

            default:
                /* MISRA C-2012 Rule 16.4 */
                break;
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else
    {
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
            APPL_TRC(
            "Received ATT Connection Indication with result 0x%04X\n\n",
            event_result);

            if (API_SUCCESS == event_result)
            {
                if (sizeof(BT_DEVICE_ADDR) == event_datalen)
                {
                    APPL_TRC(
                    "Received ATT_CONNECTION_IND on LE for ATT Handle: \n"
                    "   -> Device_ID       : 0x%02X\n"
                    "   -> ATT_Instance_ID : 0x%02X\n\n",
                    handle->device_id, handle->att_id);
                }
                else if (0U == event_datalen)
                {
                    APPL_TRC(
                    "Received ATT_CONNECTION_IND on BR/EDR for ATT Handle: \n"
                    "   -> Device_ID       : 0x%02X\n"
                    "   -> ATT_Instance_ID : 0x%02X\n\n",
                    handle->device_id, handle->att_id);
                }
                else
                {
                    /**
                     * TODO:
                     * Handle EATT related Server Application connections here.
                     * Currently the Application GATT Client layer, does not
                     * pass the EATT Connection Indication to Server.
                     */
                }

                retval = appl_add_device(handle,&fsm_param.handle);

                if (API_SUCCESS == retval)
                {
#ifdef APPL_FSM_SUPPORT
                    /* Post Disconnection Event */
                    appl_fsm_post_event
                    (
                        ev_appl_transport_connected_ind,
                        (void *)(&fsm_param)
                    );

                    /* Post Transport Operating Indication */
                    /**
                     *  Please note that in case of BLE Transport Connect and Transport
                     *  Operating mean the same. But this may not be true in case of
                     *  BR/EDR or some-other transport for which this application could
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
            }
            break;

        case ATT_DISCONNECTION_IND:
            (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle (handle->device_id, &fsm_param.handle);

#ifdef APPL_FSM_SUPPORT
            appl_fsm_post_event
            (
                ev_appl_transport_suspend_ind,
                (void *)(&fsm_param)
            );
#else
            appl_service_disconnect(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */

            APPL_TRC(
            "\n\nReceived ATT_DISCONNECTION_IND with reason 0x%04X for ATT Handle:\n"
            "   -> Device_ID       : 0x%02X\n"
            "   -> ATT_Instance_ID : 0x%02X\n\n",
            event_result, handle->device_id, handle->att_id);

            (BT_IGNORE_RETURN_VALUE) BT_gatt_db_peer_session_shutdown_handler (handle);
            break;

        case ATT_XCHNG_MTU_REQ:
            if (ATT_INVALID_PARAMETER == event_result)
            {
                /* Error Response */
                /**
                 *  In case application does not support setting any value other
                 *  than default 23, an error response with request not supported
                 *  would be more appropriate. This should be sent irrespective
                 *  of what event result is.
                 *  But in case it does support the request, but MTU requested is
                 *  detected to be invalid, response code, 'Invalid PDU' seems to
                 *  be most fitting currently for Error Response.
                 */
                err_param.error_code = ATT_INVALID_PDU;
                err_param.handle = 0x0000U;
                err_param.op_code = ATT_XCHNG_MTU_REQ;
                retval = BT_att_send_error_rsp (handle,&err_param);
                APPL_ERR (
                "Invalid MTU Size received in Xchng MTU Req; "
                "Error Response sent with result %04X!\n",
                retval);
            }
            else if (API_SUCCESS == retval)
            {
#ifdef ATT_ON_ECBFC_SUPPORT
                ATT_ECBFC_INFO t_info;

                retval = BT_att_ecbfc_get_info(handle, &t_info);

                if (API_SUCCESS == retval)
                {
                    err_param.error_code = ATT_REQUEST_NOT_SUPPORTED;
                    err_param.handle = 0x0000;
                    err_param.op_code = ATT_XCHNG_MTU_REQ;
                    retval = BT_att_send_error_rsp (handle,&err_param);
                    APPL_ERR (
                    "Request Not Supported over EATT Channel!"
                    "Error Response sent with result %04X!\n",
                    retval);
                }
                else
#endif /* ATT_ON_ECBFC_SUPPORT */
                {
#ifdef ATT_ON_BR_EDR_SUPPORT
                    /* Fetch the Link Type and Send Error response if BREDR ATT */
                    retval = device_queue_get_link_type(&link_type, &handle->device_id);
                    if ((API_SUCCESS == retval) && (DQ_BR_LINK == link_type))
                    {
                        err_param.error_code = ATT_REQUEST_NOT_SUPPORTED;
                        err_param.handle = 0x0000;
                        err_param.op_code = ATT_XCHNG_MTU_REQ;
                        retval = BT_att_send_error_rsp (handle,&err_param);
                        APPL_ERR (
                        "Request Not Supported over EATT Channel!"
                        "Error Response sent with result %04X!\n",
                        retval);
                    }
                    else
#endif /* ATT_ON_BR_EDR_SUPPORT */
                    {
                        BT_UNPACK_LE_2_BYTE(&mtu, event_data);
                        APPL_TRC(
                        "Received Exchange MTU Request with MTU Size "
                        "= 0x%04X!\n", mtu);

                        xcnhg_rsp_param.mtu = appl_gatt_mtu;

                        retval = BT_att_send_mtu_xcnhg_rsp
                                 (
                                     handle,
                                     &xcnhg_rsp_param
                                 );

                        APPL_TRC(
                        "Sent Response with retval 0x%04X\n", retval);

                        if (API_SUCCESS == retval)
                        {
                            UINT16 t_mtu;

                            retval = BT_att_access_mtu
                                     (
                                         handle,
                                         &t_mtu
                                     );

                            if (API_SUCCESS == retval)
                            {
                                appl_service_mtu_updt_handler(handle, t_mtu);
                            }
                        }
                    }
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
            break;

        case ATT_FIND_INFO_REQ:
            APPL_TRC (
            "Received Find Info Request with result 0x%04X\n",event_result);

            if (API_SUCCESS == event_result)
            {
                offset= 0U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.start_handle,
                     &event_data[offset]
                );

                offset += 2U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.end_handle,
                     &event_data[offset]
                );

                if (0U != (ATT_CHECK_VALID_HANDLE_RANGE\
                (range.start_handle, range.end_handle)))
                {
                    err_param.error_code = ATT_INVALID_HANDLE;
                    send_err = BT_TRUE;
                }
                else
                {
                    (BT_IGNORE_RETURN_VALUE) appl_handle_find_info_request (handle,&range);
                }
            }
            break;

        case ATT_FIND_BY_TYPE_VAL_REQ:
            APPL_TRC (
            "Received Find By Type Value Request with result 0x%04X\n",
            event_result);

            if (API_SUCCESS == event_result)
            {
                offset= 0U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.start_handle,
                     &event_data[offset]
                );

                offset += 2U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.end_handle,
                     &event_data[offset]
                );

                if (0U != (ATT_CHECK_VALID_HANDLE_RANGE\
                (range.start_handle, range.end_handle)))
                {
                    err_param.error_code = ATT_INVALID_HANDLE;
                    send_err = BT_TRUE;
                    break;
                }

                offset += 2U;
                uuid.len = ATT_16_BIT_UUID_SIZE;
                uuid.val = &event_data[offset];

                offset += 2U;
                value.len = event_datalen - offset;
                value.val = &event_data[offset];

                if ((BT_FALSE != GATT_VERIFY_UUID_VALUE(uuid.val, GATT_PRIMARY_SERVICE_TYPE_OFFSET, ATT_16_BIT_UUID_SIZE)) ||\
                   (BT_FALSE != GATT_VERIFY_UUID_VALUE(uuid.val, GATT_SECONDARY_SERVICE_TYPE_OFFSET, ATT_16_BIT_UUID_SIZE)))
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
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }
                }

                if (BT_FALSE == send_err)
                {
                    (BT_IGNORE_RETURN_VALUE) appl_handle_find_by_type_val_request
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
                offset= 0U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.start_handle,
                     &event_data[offset]
                );

                offset += 2U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.end_handle,
                     &event_data[offset]
                );

                if (0U != (ATT_CHECK_VALID_HANDLE_RANGE\
                (range.start_handle, range.end_handle)))
                {
                    err_param.error_code = ATT_INVALID_HANDLE;
                    err_param.handle = range.start_handle;
                    send_err = BT_TRUE;
                    break;
                }

                offset += 2U;
                uuid.val = &event_data[offset];
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
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                if (BT_FALSE == send_err)
                {
                    (BT_IGNORE_RETURN_VALUE) appl_handle_read_by_type_request
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

                (BT_IGNORE_RETURN_VALUE) appl_handle_read_request
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
                    &event_data[2U]
                );

                (BT_IGNORE_RETURN_VALUE) appl_handle_read_request
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
            APPL_TRC (
            "Received Read By Multiple Request with result 0x%04X\n",
            event_result);
            list.list_count = 0U;
            do
            {
                BT_UNPACK_LE_2_BYTE
                (
                    &attr_handle_list[list.list_count],
                    (&event_data[(list.list_count * 2U)])
                );
                list.list_count++;
            }while (event_datalen > (list.list_count * 2U));

            list.handle_list = attr_handle_list;

            (BT_IGNORE_RETURN_VALUE) appl_handle_read_multiple_request (handle,&list);
        }
        break;
        case ATT_READ_BY_GROUP_REQ:
        {
            APPL_TRC (
            "Received Read By Group Type Request with result 0x%04X\n",
            event_result);
            if (API_SUCCESS == event_result)
            {
                offset= 0U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.start_handle,
                     &event_data[offset]
                );

                offset += 2U;

                BT_UNPACK_LE_2_BYTE
                (
                     &range.end_handle,
                     &event_data[offset]
                );

                if (0U != (ATT_CHECK_VALID_HANDLE_RANGE\
                (range.start_handle, range.end_handle)))
                {
                    err_param.error_code = ATT_INVALID_HANDLE;
                    send_err = BT_TRUE;
                    break;
                }

                offset += 2U;
                uuid.val = &event_data[offset];
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
                    (BT_IGNORE_RETURN_VALUE) appl_handle_read_by_group_type_request
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

                value.len = event_datalen - 2U;
                value.val = &event_data[2U];

                (BT_IGNORE_RETURN_VALUE) appl_handle_write_request
                (
                    handle,
                    attr_handle,
                    0U,
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

                value.len = event_datalen - 2U;
                value.val = &event_data[2U];

                (BT_IGNORE_RETURN_VALUE) appl_handle_write_request
                (
                    handle,
                    attr_handle,
                    0U,
                    &value,
                    ( GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP)
                );

                APPL_TRC (
                "[APPL] Received Write Command for Handle 0x%04X of Length %d\n",
                attr_handle, event_datalen - 2U);

                appl_dump_bytes (&event_data[2U], event_datalen - 2U);
            }
            break;
#ifdef ATT_QUEUED_WRITE_SUPPORT
        case ATT_PREPARE_WRITE_REQ:
            BT_UNPACK_LE_2_BYTE
            (
                &attr_handle,
                event_data
            );

            BT_UNPACK_LE_2_BYTE
            (
                &offset,
                &event_data[2U]
            );

            value.len = event_datalen - 4U;
            value.val = &event_data[4U];

            (BT_IGNORE_RETURN_VALUE) appl_handle_write_request
            (
                handle,
                attr_handle,
                offset,
                &value,
                (GATT_DB_PEER_INITIATED | GATT_DB_PREPARE)
            );
            break;

        case ATT_EXECUTE_WRITE_REQ:
            /* NULL Check for event_data already present in the top */
            APPL_TRC(
            "[APPL]: ATT Execute Write Param 0x%02X\n", event_data[0U]);
            if (0x02U > event_data[0U])
            {
                (BT_IGNORE_RETURN_VALUE)appl_execute_write_handler[event_data[0U]](handle);
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
            appl_parse_confirmation_data (handle, event_result);
            break;

        case ATT_HANDLE_VALUE_NTF_TX_COMPLETE:
            appl_parse_ntf_tx_complete (handle, event_data, event_datalen);
            break;

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
        case ATT_HANDLE_VALUE_MULTIPLE_NTF_TX_COMPLETE:
            CONSOLE_OUT("Received HVMN Tx Complete (Locally generated)\n");
            appl_dump_bytes(event_data, event_datalen);
            break;
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

        case ATT_SIGNED_WRITE_CMD:
            appl_dump_bytes(event_data, event_datalen);

#ifdef SMP_DATA_SIGNING
            {
                s_data = BT_alloc_mem (event_datalen + 1U);
                s_datalen = event_datalen + 1U;

                s_data[0U] = ATT_SIGNED_WRITE_CMD;
                BT_mem_copy (&s_data[1U], event_data, event_datalen);

                appl_signed_write_server_handle = *handle;

                appl_smp_verify_sign_data (s_data, s_datalen);
            }
#else /* SMP_DATA_SIGNING */
            appl_handle_unsupported_op_code (handle,att_event);
#endif /* SMP_DATA_SIGNING */

            break;

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
        case ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ:
        {
            APPL_TRC (
            "Received Read Multiple Variable Length Request with result 0x%04X\n",
            event_result);
            list.list_count = 0U;
            do
            {
                BT_UNPACK_LE_2_BYTE
                (
                    &attr_handle_list[list.list_count],
                    (event_data + (list.list_count * 2U))
                );
                list.list_count++;
            }while (event_datalen > (list.list_count * 2U));

            list.handle_list = attr_handle_list;

            (BT_IGNORE_RETURN_VALUE)appl_handle_read_multiple_var_len_request (handle,&list);
        }
        break;
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

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

            if (BT_FALSE != APPL_CHECK_IF_ATT_REQUEST(event_data[0U]))
            {
                (BT_IGNORE_RETURN_VALUE) appl_handle_unsupported_op_code (handle,event_data[0U]);
            }
            else
            {
                APPL_ERR (
                "[APPL]:[*** ERR ****]: Received Unknown PDU (Response Op code "
                "0x%02X?) from Client!\n",event_data[0U]);
                /**
                 * NOTE:
                 * In GATT PTS Testing in one of the Invalid behaviour testing
                 * the error that is needed to be sent is ATT_REQUEST_NOT_SUPPORTED
                 * and not ATT_INVALID_PDU.
                 */
                err_param.error_code = ATT_REQUEST_NOT_SUPPORTED;
                err_param.op_code = event_data[0U];
                err_param.handle = ATT_INVALID_ATTR_HANDLE_VAL;

                send_err = BT_TRUE;
            }
            break;
        default:
            /**
             * NOTE:
             * In GATT PTS Testing in one of the Invalid behaviour testing
             * the error is needed to be sent on unhandled PDU.
             * So, here we need to set:
             * send_err = BT_TRUE;
             */
            send_err = BT_TRUE;

             APPL_TRC(
             "[ATT]:[0x%02X]:[0x%02X]: Received Unhandled event 0x%02X with result 0x%04X\n",
             handle->device_id, handle->att_id, att_event, event_result);

            break;
        }

#ifdef APPL_HAVE_PROFILE_ATT_CB_SUPPORT
        if (NULL != appl_event_hndlr_cb)
        {
            (BT_IGNORE_RETURN_VALUE)appl_event_hndlr_cb
            (
                handle,
                att_event,
                event_result,
                event_data,
                event_datalen
            );
        }
#endif /* APPL_HAVE_PROFILE_ATT_CB_SUPPORT */
        }

        if (BT_FALSE != send_err)
        {
             retval = BT_att_send_error_rsp
                      (
                          handle,
                          &err_param
                      );
             APPL_ERR (
             "[APPL]: Error Response sent with result 0x%04X\n",retval);
        }

        /* Update ATT status to be free */
        BT_status_set_bit(STATUS_BIT_ATT_BUSY, STATUS_BIT_RESET);
    }

    return retval;
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

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&value, 0, sizeof(ATT_VALUE));

    if (API_SUCCESS == status)
    {
        /* Extract handle */
        BT_UNPACK_LE_2_BYTE
        (
            &attr_handle,
            (&s_data[1U])
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
        value.len = (s_datalen - (1U + 2U + sizeof(UINT32) + SMP_MAC_SIZE));
        value.val = &s_data[3U];

        /* Verify Signing Counter/Signature. Discard if invalid */
        if (0 != BT_mem_cmp(data, &s_data[s_datalen - datalen], datalen))
        {
            APPL_TRC("\n*** Signature Mismatch of Verified Data. Discarding... ***\n\n");
        }
        else
        {
            (BT_IGNORE_RETURN_VALUE)appl_handle_write_request
            (
                &appl_signed_write_server_handle,
                attr_handle,
                0U,
                &value,
                (GATT_DB_PEER_INITIATED | GATT_DB_SIGNED_WRITE)
            );
        }
    }
    else
    {
        /* Data signing verification procedure failed */
        /* Application Updates to Handle Encryption Key Size Error */
        /* Assigning API_FAILURE explicitly as the incoming status is only
         * UCHAR which is converted to API_RESULT in the calling function.
         */
        (BT_IGNORE_RETURN_VALUE) appl_handle_security_error
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

    for (index = 0U; index < APPL_MAX_VALID_ATT_PDU_FOR_LEN_CHK; index++)
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

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
API_RESULT appl_gatt_send_multi_handle_val_ntf_request(DEVICE_HANDLE dq_handle)
{
    ATT_HNDL_VAL_MULTIPLE_NTF_PARAM param;
    ATT_HANDLE_VALUE_PAIR           handle_value_list[APPL_ATT_MAX_MULT_HVN_COUNT];
    int     choice;
    UINT32  index;
    API_RESULT retval;
    APPL_HANDLE appl_handle;
    ATT_HANDLE  t_att_handle;

    BT_mem_set(handle_value_list, 0x0, sizeof(handle_value_list));
    appl_handle = 0U;

    /* Fetch the Application Handle */
    retval = appl_get_handle_from_device_handle(dq_handle, &appl_handle);

    CONSOLE_OUT ("Enter ATT Handle Device ID: [in HEX]");
    scanf ("%x", &choice);
    t_att_handle.device_id = (DEVICE_HANDLE)choice;
    CONSOLE_OUT ("Enter ATT Handle ATT Instance ID: [in HEX]");
    scanf ("%x", &choice);
    t_att_handle.att_id = (ATT_CON_ID)choice;

    APPL_TRC("Enter Number of ATT Handles (maximum %d): \n", APPL_ATT_MAX_MULT_HVN_COUNT);
    scanf("%d", &choice);

    if (choice > APPL_ATT_MAX_MULT_HVN_COUNT)
    {
        APPL_TRC(
        "Number of ATT Handles (%d) > APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT (%d). Returning error.\n",
        choice, APPL_ATT_MAX_MULT_HVN_COUNT);

        return API_FAILURE;
    }

    param.handle_value_list = handle_value_list;
    param.count = (UINT16)choice;

    APPL_TRC("Enter ATT Handles\n");

    for (index = 0U; index < param.count; index++)
    {
        APPL_TRC("ATT Handle [%d] (in HEX): \n", index);
        scanf("%x", &choice);

        handle_value_list[index].handle = (ATT_ATTR_HANDLE)choice;

        retval = BT_gatt_db_access_handle
                 (
                     &handle_value_list[index],
                     &APPL_GET_ATT_INSTANCE(appl_handle),
                     0U,
                     (GATT_DB_READ | GATT_DB_LOCALLY_INITIATED)
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC("Failed to read ATT value for Handle [0x%04X]: \n", handle_value_list[index].handle);

            break;
        }
    }

    retval = BT_att_send_hndl_val_multiple_ntf(&t_att_handle, &param);

    return retval;
}
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

API_RESULT appl_gatt_set_mtu(UINT16 mtu)
{
    /*
     * Set this as the Max MTU that the App has registered till now and
     * use this in ATT Exchange MTU Response.
     */
    if (appl_gatt_mtu < mtu)
    {
        appl_gatt_mtu = mtu;
    }
    else
    {
        /* Retain the highest MTU Configured so far */
    }

    return API_SUCCESS;
}

#ifdef APPL_LIMIT_LOGS
#ifdef APPL_TRC
#undef APPL_TRC
#define APPL_TRC printf
#endif /* APPL_TRC */

#ifdef CONSOLE_OUT
#undef CONSOLE_OUT
#define CONSOLE_OUT printf
#endif /* CONSOLE_OUT */
#endif /* APPL_LIMIT_LOGS */

void main_gatt_server_operations(void)
{
    unsigned int  choice, menu_choice;
    unsigned int  read_val;
    API_RESULT    retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT ("%s \n", gatt_server_menu);
        CONSOLE_OUT ("Enter you choice : ");
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 2:
            CONSOLE_OUT ("Selection Option to Grant or Deny User Level Authorization: \n");
            CONSOLE_OUT ("Enter 1. Grant, 0. Deny:\n");
            scanf ("%x", &choice);

            (choice == 1U) ?                                           \
            APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, BT_TRUE) : \
            APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, BT_FALSE);
            break;

        case 3:
            {
                BT_DEVICE_ADDR  device_addr;
                GATT_DB_HANDLE  db_handle;
                ATT_UUID        tmp_uuid;
                UCHAR           frmt;
                UCHAR           link_type;

                /* Populate GATT Service UUID */
                tmp_uuid.uuid_16 = GATT_GATT_SERVICE;
                frmt             = ATT_16_BIT_UUID_FORMAT;

                /**
                 * NOTE:
                 * The below code reference, always fetches the 1st instance
                 * instance of GATT_SERVICE_CHANGED_CHARACTERISTIC characteristic
                 * associated with the 1st instance of GATT_GATT_SERVICE that
                 * occur in the current active/registered GATT DB.
                 */
                /* Check for Service ID and Characteristic ID of GATT in current DB */
                retval = BT_gatt_db_get_sid_from_uuid
                         (
                             &tmp_uuid,
                             frmt,
                             0xFF,
                             BT_TRUE,
                             &db_handle.service_id
                         );

                if (API_SUCCESS == retval)
                {
                    /* Populate GATT Service Changed Characteristic UUID */
                    tmp_uuid.uuid_16 = GATT_SERVICE_CHANGED_CHARACTERISTIC;
                    frmt             = ATT_16_BIT_UUID_FORMAT;

                    retval = BT_gatt_db_get_cid_from_uuid
                             (
                                 &tmp_uuid,
                                 frmt,
                                 0xFF,
                                 db_handle.service_id,
                                 &db_handle.char_id
                             );

                    if (API_SUCCESS == retval)
                    {
                        CONSOLE_OUT(
                        "Found GATT Service Changed Char with SID:0x%04X, CID:0x%02X\n",
                        db_handle.service_id, db_handle.char_id);
                    }
                    else
                    {
                        CONSOLE_OUT(
                        "No Instance of GATT Service Changed Char found in Current GATT DB!\n");

                        break;
                    }
                }
                else
                {
                    CONSOLE_OUT(
                    "No Instance of GATT Service found in Current GATT DB!\n");

                    break;
                }

                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(device_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                scanf("%u", &read_val);
                device_addr.type = (UCHAR) read_val;

                CONSOLE_OUT("\n1 - BREDR Link \n2 - LE Link\n3 - Any Link");
                CONSOLE_OUT("Enter Link type : ");
                scanf("%u", &read_val);
                link_type = (UCHAR) read_val;
                /* TODO: Any specific validation for Link Type to be done here if needed. */

                /* Validate Link Type and Assign Accordingly */
                link_type = (DQ_BR_LINK == link_type) ? DQ_BR_LINK : (DQ_LE_LINK == link_type) ? DQ_LE_LINK : DQ_LINK_ANY;

                /* Fetch the Device Handle */
                retval = device_queue_search_remote_addr
                         (
                             &db_handle.device_id,
                             &device_addr,
                             link_type
                         );

                if (API_SUCCESS == retval)
                {
                    retval = appl_send_serv_changed_ind(&db_handle);

                    if (API_SUCCESS == retval)
                    {
                        CONSOLE_OUT("\nService Changed Indication sent successfully!\n");
                    }
                    else
                    {
                        CONSOLE_OUT("\nSend Service changed Indication Failed 0x%04X \n", retval);
                    }
                }
            }
            break;

        case 4:
            {
                BT_DEVICE_ADDR  device_addr;
                GATT_DB_HANDLE  db_handle;
                UCHAR           batt_level;
                ATT_UUID        tmp_uuid;
                UCHAR           frmt;
                UCHAR           link_type;

                /* Populate Battery Service UUID */
                tmp_uuid.uuid_16 = GATT_BATTERY_SERVICE;
                frmt             = ATT_16_BIT_UUID_FORMAT;

                /**
                 * NOTE:
                 * The below code reference, always fetches the 1st instance
                 * instance of GATT_BATTERY_LEVEL_CHARACTERISTIC characteristic
                 * associated with the 1st instance of GATT_BATTERY_SERVICE that
                 * occur in the current active/registered GATT DB.
                 */
                /* Check for Service ID of Battery Service in current DB as Primary Service */
                retval = BT_gatt_db_get_sid_from_uuid
                         (
                             &tmp_uuid,
                             frmt,
                             0xFF,
                             BT_TRUE, /* Primary Service */
                             &db_handle.service_id
                         );

                if (API_SUCCESS != retval)
                {
                    retval = BT_gatt_db_get_sid_from_uuid
                             (
                                 &tmp_uuid,
                                 frmt,
                                 0xFF, /* TO CHECK if this needs to be UINT16? */
                                 BT_FALSE, /* Secondary Service */
                                 &db_handle.service_id
                             );
                }
                else
                {
                    /* Empty */
                }

                if (API_SUCCESS == retval)
                {
                    /* Populate Battery Level Characteristic UUID */
                    tmp_uuid.uuid_16 = GATT_BATTERY_LEVEL_CHARACTERISTIC;
                    frmt             = ATT_16_BIT_UUID_FORMAT;

                    retval = BT_gatt_db_get_cid_from_uuid
                             (
                                 &tmp_uuid,
                                 frmt,
                                 0xFF, /* TO CHECK if this needs to be UINT16? */
                                 db_handle.service_id,
                                 &db_handle.char_id
                             );

                    if (API_SUCCESS == retval)
                    {
                        CONSOLE_OUT(
                        "Found Battery Level Char with SID:0x%04X, CID:0x%02X\n",
                        db_handle.service_id, db_handle.char_id);
                    }
                    else
                    {
                        CONSOLE_OUT(
                        "No Instance of Battery Level found in Current GATT DB!\n");

                        break;
                    }
                }
                else
                {
                    CONSOLE_OUT(
                    "No Instance of Battery Service found in Current GATT DB!\n");

                    break;
                }

                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(device_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                scanf("%u", &read_val);
                device_addr.type = (UCHAR) read_val;

                CONSOLE_OUT("\n1 - BREDR Link \n2 - LE Link\n3 - Any Link");
                CONSOLE_OUT("Enter Link type : ");
                scanf("%u", &read_val);
                link_type = (UCHAR) read_val;
                /* TODO: Any specific validation for Link Type to be done here if needed. */

                CONSOLE_OUT("\n Enter Desired Battery Level[range 0-100%%, in HEX]:\n");
                scanf("%u", &read_val);
                batt_level = (UCHAR) read_val;
                /* TODO: Any specific validation for Battery Level to be done here if needed. */

                /* Validate Link Type and Assign Accordingly */
                link_type = (DQ_BR_LINK == link_type) ? DQ_BR_LINK : (DQ_LE_LINK == link_type) ? DQ_LE_LINK : DQ_LINK_ANY;

                /* Fetch the Device Handle */
                retval = device_queue_search_remote_addr
                         (
                             &db_handle.device_id,
                             &device_addr,
                             link_type
                         );

                if (API_SUCCESS == retval)
                {
                    appl_update_battery_lvl(&db_handle, batt_level);
                }
                else
                {
                    CONSOLE_OUT("Update Battery Level Failed 0x%04X \n", retval);
                }
            }
            break;

        case 5:
#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
            {
                BT_DEVICE_ADDR device_addr;
                DEVICE_HANDLE  dq_handle;
                UCHAR          link_type;

                CONSOLE_OUT("Enter BD_ADDR : ");
                appl_get_bd_addr(device_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                scanf("%u", &read_val);
                device_addr.type = (UCHAR)read_val;

                CONSOLE_OUT("\n1 - BREDR Link \n2 - LE Link\n3 - Any Link");
                CONSOLE_OUT("Enter Link type : ");
                scanf("%u", &read_val);
                link_type = (UCHAR) read_val;

                /* Fetch the Device Handle */
                /* Validate Link Type and Assign Accordingly */
                link_type = (DQ_BR_LINK == link_type) ? DQ_BR_LINK : (DQ_LE_LINK == link_type) ? DQ_LE_LINK : DQ_LINK_ANY;

                /* Fetch the Device Handle */
                retval = device_queue_search_remote_addr
                         (
                             &dq_handle,
                             &device_addr,
                             link_type
                         );

                if (API_SUCCESS == retval)
                {
                    appl_gatt_send_multi_handle_val_ntf_request(dq_handle);
                }
            }
#else
            {
                CONSOLE_OUT(
                "ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT is not defined\n");
            }
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */
            break;

        case 10:
#ifdef APPL_MENU_OPS
            main_service_menu_operations();
#else /* APPL_MENU_OPS */
            CONSOLE_OUT("'APPL_MENU_OPS' not defined!\n");
#endif /* APPL_MENU_OPS */
            break;

        case 100:
#ifdef APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION
            {
                UCHAR flag;

                CONSOLE_OUT("\nEnable/Disable Application Event Trace\n");
                CONSOLE_OUT("1 - Enable Evt Prints\n");
                CONSOLE_OUT("0 - Disable Evt Prints\n");
                CONSOLE_OUT("Enter the desired value:\n");
                scanf("%u", &read_val);
                flag = (UCHAR) read_val;

                appl_set_gatt_server_evt_trc(flag);
            }
#else /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */
            CONSOLE_OUT("\n APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION is Disabled!\n");
#endif /* APPL_GATT_SERVER_HAVE_EVT_TRC_SELECTION */
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}
#endif /* ATT */
