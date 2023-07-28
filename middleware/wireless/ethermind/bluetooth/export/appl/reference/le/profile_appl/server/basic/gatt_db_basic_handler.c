
/**
 *  \file gatt_db_basic_handler.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined BASIC)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT appl_basic_gatt_db_ntf_cb
           (
               UCHAR      gatt_db_event,
               API_RESULT event_result,
               UCHAR      * data_param,
               UINT16     data_len
           )
{
    API_RESULT     retval;
    ATT_VALUE      value;
    GATT_DB_HANDLE gatt_db_basic_db_hash_hndl;

    /**
     * Currently handling only GATT Hash calculated related event.
     */
    if ((GATT_DB_HASH_IND != gatt_db_event) || (API_SUCCESS != event_result))
    {
        return event_result;
    }

    /* Initialize */
    retval = API_SUCCESS;
    gatt_db_basic_db_hash_hndl.device_id  = DEVICE_HANDLE_INIT_VAL;
    gatt_db_basic_db_hash_hndl.service_id = (UCHAR)GATT_SER_BASIC_GATT_INST;
    gatt_db_basic_db_hash_hndl.char_id    = (UCHAR)GATT_CHAR_BASIC_DATABASE_HASH_INST;

    CONSOLE_OUT("Calculated HASH value is :\n");
    appl_dump_bytes(data_param, data_len);

    value.val = data_param;
    value.len = data_len;

    /* Dummy Device ID */
    gatt_db_basic_db_hash_hndl.device_id = 0x00U;

    /* Get DB Hash Char Val */
    retval = BT_gatt_db_set_char_val(&gatt_db_basic_db_hash_hndl, &value);

    CONSOLE_OUT( "Update of DB Hash Value returned 0x%04X\n", retval);

    return retval;
}

API_RESULT gatt_db_basic_gatt_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;

    retval = API_SUCCESS;

    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_BASIC_SER_CHNGD_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config, params->value.val);

        /**
         * Save the Service Changed CCCD value into locally maintained
         * array based on the Device Handle.
         */
        appl_basic_serv_chngd_cccd[handle->device_id] = config;

        /**
         * Have Service Changed Handler here
         * Could be specific to Basic
         */
    }

    if (GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST == handle->char_id)
    {
        switch(params->db_op)
        {
            case GATT_DB_CHAR_PEER_WRITE_REQ:
                if (APPL_BASIC_CSFC_LEN == params->value.len)
                {
                    /**
                     * Ignore RFU bits in CSFC Value
                     */
                    params->value.val[0U] &= APPL_BASIC_CSFC_MASK;

                    /**
                     * Save the CSFC value into locally maintained array
                     * based on the Device Handle.
                     */
                    appl_basic_csfc_value[handle->device_id] = params->value.val[0U];
                }
                break;

            case GATT_DB_CHAR_PEER_READ_REQ:
                {
                    ATT_HANDLE         * att_handle;
                    APPL_HANDLE          app_handle;
                    ATT_READ_RSP_PARAM   rsp_param;

                    /**
                     * Fetch the locally saved value of CSFC and
                     * send it as Read Response
                     */

                    retval = appl_get_handle_from_device_handle
                             (
                                 handle->device_id,
                                 &app_handle
                             );
                    if (API_SUCCESS == retval)
                    {
                        /* Get ATT Instance from Application Handle */
                        att_handle = &APPL_GET_ATT_INSTANCE(app_handle);

                        /* Access the application handle and send back read response */
                        rsp_param.val = &appl_basic_csfc_value[handle->device_id];
                        rsp_param.len = APPL_BASIC_CSFC_LEN;

                        /* Get ATT Handle */
                        if (NULL != att_handle)
                        {
                            retval = BT_att_read_rsp
                                     (
                                         att_handle,
                                         &rsp_param
                                     );
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }

                    /**
                     * Mark as Already Responded and ATT Server Layer not to
                     * send response.
                     */
                    retval = GATT_DB_ALREADY_RESPONDED;
                }
                break;

            default:
                break;
        }
    }


    if ((GATT_DB_CHAR_PEER_WRITE_REQ == params->db_op) &&
        (GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST == handle->char_id) &&
        (APPL_BASIC_CSFC_LEN == params->value.len))
    {
        /**
         * Ignore RFU bits in CSFC Value
         */
        params->value.val[0U] &= APPL_BASIC_CSFC_MASK;

        /**
         * Save the CSFC value into locally maintained array
         * based on the Device Handle.
         */
        appl_basic_csfc_value[handle->device_id] = params->value.val[0U];
    }

    return retval;
}

API_RESULT gatt_db_basic_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;

    retval = API_SUCCESS;

    /**
     *  Note: Adding a generic handler for Battery Level Notification.
     *  In case the profile does not support Battery Service. Please add dummy
     *  GATT_CHAR_BASIC_BATTERY_LVL_INST set to 0xFF in corresponding
     *  gatt_db.h to resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_BASIC_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_basic_gatt_cache_check
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT  retval;
    UCHAR       robust_cache;

    retval = API_SUCCESS;

    /* Get the GATT DB Robust Cache Value for this Device ID */
    robust_cache = (appl_basic_csfc_value[handle->device_id] &
                    APPL_BASIC_CSFC_ROBUST_CACHE_MASK);

    /**
     * If the existing Value of the Robust caching for this Device
     * handle is Set. It cannot be Reset again by the Client.
     */
    if ((GATT_DB_CHAR_PEER_WRITE_REQ           == params->db_op) &&
        (GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST == handle->char_id))
    {
        if ((BT_TRUE  == robust_cache) &&
            (BT_FALSE == params->value.val[0]))
        {
            return ATT_PARAM_VAL_NOT_ALLOWED;
        }
    }

    /**
     * Check if Robust Cache for this Device Handle is enabled
     * and the incoming value length is as required.
     */
    if (BT_TRUE == robust_cache)
    {
        /**
         * Set the DB State if Discovery is initiated from Peer
         * or if DB Hash is tried to be read.
         */
        if ((0xF5                               == params->db_op) ||
           ((GATT_DB_CHAR_PEER_READ_BY_TYPE_REQ == params->db_op) &&
            (GATT_CHAR_BASIC_DATABASE_HASH_INST == handle->char_id)))
        {
            /** Set the Device Handle as Change Aware */
            /** TODO: shift to a macro for better readability */
            appl_basic_change_aware_state[handle->device_id] = BT_TRUE;
        }
        /** Check the change aware state */
        /** TODO: shift to a macro for better readability */
        else if (BT_TRUE != appl_basic_change_aware_state[handle->device_id])
        {
            /* Check if the incoming operation is anything other than
             * ATT Commands which do not require any responses.
             */
            if ((GATT_DB_CHAR_PEER_WRITE_CMD        != params->db_op) &&
                (GATT_DB_CHAR_PEER_SIGNED_WRITE_CMD != params->db_op))
            {
                /* Incoming Request, Increment the counter for this Device */
                appl_basic_request_counter[handle->device_id] += 1;

                if (0x02 <= appl_basic_request_counter[handle->device_id])
                {
                    /**
                     * If already DB Out of Sync Error has been sent.
                     * Then mark the device as change aware.
                     */
                    appl_basic_change_aware_state[handle->device_id] = BT_TRUE;
                }
                else
                {
                    return ATT_DB_OUT_OF_SYNC;
                }
            }
            else
            {
                /* Ignore the Write Command or Signed Write Command */
                return API_FAILURE;
            }
        }
        else
        {
        }
    }
    /**
     * Robust Caching is Reset and
     * Peer is not Change Aware.
     * Possible that device is a fresh device.
     * Mark the Peer as Change aware if Discovery is being
     * attempted.
     * "dp_op" 0xF5 corresponds to Discovery.
     */
    else if ((BT_FALSE == robust_cache) &&
             (BT_TRUE != appl_basic_change_aware_state[handle->device_id]))
    {
        if (0xF5 == params->db_op)
        {
            appl_basic_change_aware_state[handle->device_id] = BT_TRUE;
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

API_RESULT gatt_db_basic_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    if ((NULL == handle) || (NULL == params))
    {
        return API_SUCCESS;
    }

#ifdef GATT_DB_ENABLE_CHAR_HANDLER_CONSOLE_TRACE
    APPL_TRC("GATT DB BASIC: Char Handler invoked for\n");
    APPL_TRC(">> Device   ID: 0x%02X\n", handle->device_id);
    APPL_TRC(">> Service  ID: 0x%02X\n", handle->service_id);
    APPL_TRC(">> Char     ID: 0x%02X\n", handle->char_id);
    APPL_TRC(">> Attr Handle: 0x%04X\n", params->handle);
    APPL_TRC(">> Operation  : 0x%02X\n", params->db_op);
#endif /* GATT_DB_ENABLE_CHAR_HANDLER_CONSOLE_TRACE */

    /**
     * Check for Caching related states before invoking
     * dedicated handlers for other discovery?
     */
    retval = gatt_db_basic_gatt_cache_check(handle, params);

    if (API_SUCCESS == retval)
    {
        /* GAP Service Specific GATT Handlers */
        if (GATT_SER_BASIC_GAP_INST == handle->service_id)
        {
            retval = API_SUCCESS;
        }
        /* GATT Service Specific GATT Handlers */
        else if (GATT_SER_BASIC_GATT_INST == handle->service_id)
        {
            retval = gatt_db_basic_gatt_handler(handle, params);
        }
        /* Battery Service Specific GATT Handlers */
        else if (GATT_SER_BASIC_BATTERY_INST == handle->service_id)
        {
            retval = gatt_db_basic_battery_level_handler(handle, params);
        }
        /* Device Info Service Specific GATT Handlers */
        else if (GATT_SER_BASIC_DEV_INFO_INST == handle->service_id)
        {
            retval = API_SUCCESS;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined BASIC) */

