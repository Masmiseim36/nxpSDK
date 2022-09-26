
/**
 *  \file appl_service.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_utils.h"

#ifdef BT_LE

/* --------------------------------------------- External Global Variables */
#if ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL) || \
     (defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
extern UCHAR own_addr_type;
#endif /* ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL) || \
     (defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */

#if ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL))
extern UCHAR advertising_type;
extern UCHAR peer_addr_type;
extern UCHAR peer_addr[BT_BD_ADDR_SIZE];
extern APPL_GAP_ADV_DATA appl_gap_adv_data;
extern const APPL_GAP_ADV_PARAM appl_gap_adv_param[APPL_GAP_PROC_TYPES];
extern BT_DEVICE_ADDR g_bd_addr;
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
extern const APPL_GAP_SCAN_PARAM appl_gap_scan_param[APPL_GAP_PROC_TYPES];
extern const APPL_GAP_CONN_PARAM appl_gap_conn_param;
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* Timer related Globals */
static BT_timer_handle timer_handle;

#ifdef APPL_ENABLE_AUTHORIZATION
DECL_STATIC UINT16 appl_gatt_server_authorization_flag = BT_FALSE;
#endif /* APPL_ENABLE_AUTHORIZATION */

#ifdef APPL_SERVICE_CONFIG_GAP_STARTUP
/** Global to have Auto Start GAP Procedures in Profiles */
UCHAR appl_service_config_gap_auto_start = BT_TRUE;
#endif /* APPL_SERVICE_CONFIG_GAP_STARTUP */

/* --------------------------------------------- Functions */

void appl_service_set_gap_proc_state(UCHAR flag)
{
#ifdef APPL_SERVICE_CONFIG_GAP_STARTUP
    appl_service_config_gap_auto_start = flag;
#else /* APPL_SERVICE_CONFIG_GAP_STARTUP */
    BT_IGNORE_UNUSED_PARAM(flag);
#endif /* APPL_SERVICE_CONFIG_GAP_STARTUP */
}

UCHAR appl_service_get_gap_proc_state(void)
{
#ifdef APPL_SERVICE_CONFIG_GAP_STARTUP
    return appl_service_config_gap_auto_start;
#else /* APPL_SERVICE_CONFIG_GAP_STARTUP */
    return BT_TRUE;
#endif /* APPL_SERVICE_CONFIG_GAP_STARTUP */
}

void appl_service_init (void)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    BT_mem_set(peer_addr, 0x00, BT_BD_ADDR_SIZE);
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

    APPL_PROFILE_INIT();
}

void appl_service_bt_init(void)
{
    APPL_PROFILE_BT_INIT();
}

void appl_service_bt_deinit(void)
{
    APPL_PROFILE_BT_DEINIT();
}

void appl_service_connect (APPL_HANDLE * handle)
{
#ifdef ATT_ON_BR_EDR_SUPPORT
    API_RESULT          retval;
    DEVICE_HANDLE       dev_id;
    BT_DEVICE_ADDR      device_addr;
    DEVICE_LINK_TYPE    link_type;
    UINT16              connection_handle;
#endif /* ATT_ON_BR_EDR_SUPPORT */

    /**
     * Setting the Connection handle in Application Data Structure for
     * ATT over BREDR.
     */
#ifdef ATT_ON_BR_EDR_SUPPORT
    dev_id = APPL_GET_DEVICE_HANDLE(*handle);

    retval = device_queue_get_link_type
             (
                 &link_type,
                 &dev_id
             );

    retval = device_queue_get_remote_addr
             (
                 &dev_id,
                 &device_addr
             );

    if (API_SUCCESS == retval)
    {
        if (DQ_LE_LINK != link_type)
        {
            retval = BT_hci_get_connection_handle
                     (
                         &device_addr,
                         HCI_ACL_LINK,
                         &connection_handle
                     );

            if (API_SUCCESS == retval)
            {
                /*
                 * Setting the connection handle only if link type is BR link.
                 * For LE link, setting the connection will be done
                 * after receiving HCI_LE_CONNECTION_COMPLETE_SUBEVENT only.
                */
                (BT_IGNORE_RETURN_VALUE) appl_set_cxn_handle
                (
                     connection_handle,
                     dev_id,
                     handle
                 );
            }
        }
    }
#endif /* ATT_ON_BR_EDR_SUPPORT */

    APPL_PROFILE_CONNECT(handle);
}

void appl_service_disconnect (APPL_HANDLE * handle)
{
    BT_IGNORE_UNUSED_PARAM(handle);

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        /* Stop the timer on Disconnection */
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer(timer_handle);
    }

    APPL_PROFILE_DISCONNECT_HANDLER(handle);
}

void appl_service_transport_disconnect (APPL_HANDLE * handle, UCHAR reason)
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(reason);

    APPL_TRANSPORT_DISCONNECT_HANDLER(handle, reason);
}

void appl_service_configure_adv
     (
         UCHAR   proc,
         UCHAR   adv_type,
         UCHAR   l_addr_type,
         UCHAR   p_addr_type,
         UCHAR * p_addr,
         UCHAR   filter_policy
     )
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    /* Set Advertising Data */
    (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_advertising_data(appl_gap_adv_data.datalen, appl_gap_adv_data.data);

    /* Update the Globals, if required */
    advertising_type = adv_type;
    own_addr_type = l_addr_type;
    peer_addr_type = p_addr_type;

    if (NULL != p_addr)
    {
        BT_COPY_BD_ADDR(peer_addr, p_addr);
    }

    /* Set Normal Advertising Parameters */
    (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_advertising_parameters
    (
        appl_gap_adv_param[proc].min_interval,
        appl_gap_adv_param[proc].max_interval,
        adv_type,
        l_addr_type,
        p_addr_type,
        p_addr,
        appl_gap_adv_param[proc].channel_map,
        filter_policy
    );
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */
}

void appl_service_enable_adv (UCHAR enable)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_advertising_enable(enable);
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */
}

void appl_service_configure_scan
     (
         UCHAR proc,
         UCHAR scan_type,
         UCHAR l_addr_type,
         UCHAR filter_policy
     )
{
#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    /* Set Scan Parameters */
    (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_scan_parameters
    (
        scan_type,
        appl_gap_scan_param[proc].scan_interval,
        appl_gap_scan_param[proc].scan_window,
        l_addr_type,
        filter_policy
    );
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_service_enable_scan (UCHAR enable)
{
#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_scan_enable(enable, 0x01U);
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

API_RESULT appl_service_initiate_conn
           (
               UCHAR   l_addr_type,
               UCHAR   p_addr_type,
               UCHAR * p_addr,
               UCHAR   filter_policy,
               UCHAR   use_config
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

#if (defined APPL_GAP_CENTRAL)
    if (BT_TRUE == use_config)
    {
        if ((APPL_CONFIG_PEER_DEVICE_ADDR_TYPE() != p_addr_type) ||
            (0 != BT_mem_cmp(APPL_CONFIG_PEER_DEVICE_ADDR(),\
            p_addr, BT_BD_ADDR_SIZE)))
        {
            retval = API_FAILURE;
        }
    }

    if (API_SUCCESS == retval)
    {
        /**
         * Disable the Scan Prior to Initiating the Connection.
         */
        appl_service_enable_scan(0U);

        retval = BT_hci_le_create_connection
                 (
                     appl_gap_conn_param.scan_interval,
                     appl_gap_conn_param.scan_window,
                     filter_policy,
                     p_addr_type,
                     p_addr,
                     l_addr_type,
                     appl_gap_conn_param.conn_interval_min,
                     appl_gap_conn_param.conn_interval_max,
                     appl_gap_conn_param.conn_latency,
                     appl_gap_conn_param.supervision_timeout,
                     appl_gap_conn_param.max_ce_length,
                     appl_gap_conn_param.min_ce_length
                 );
    }
#endif /* (defined APPL_GAP_CENTRAL) */

    return retval;
}

#ifdef GATT_DB

/* Generic Handler to update Battery level */
void appl_update_battery_lvl(GATT_DB_HANDLE  * batt_handle, UCHAR batt_val)
{
    API_RESULT      retval;
    ATT_VALUE       value;

    value.val = &batt_val;
    value.len = 1U;

    /* Set the Battery Value */
    retval = BT_gatt_db_set_char_val(batt_handle, &value);

    if (API_FAILURE != retval)
    {
        APPL_TRC(
        "Successfully updated Battery Level, reason 0x%04X\n",
        retval);
    }
    else
    {
        APPL_TRC(
        "[ERR]: Failed to update Battery Level, reason 0x%04X\n",
        retval);
    }
}

API_RESULT appl_battery_level_handler(GATT_DB_HANDLE * handle, UINT16 config)
{
    API_RESULT    retval;
    APPL_HANDLE   appl_handle;

    retval = API_SUCCESS;

    /* If Notification is Enabled then start Notification */
    if (GATT_CLI_CNFG_NOTIFICATION == config)
    {
        (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle(handle->device_id, &appl_handle);

        /* Start timer to simulate Battery Level Notification */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            APPL_TRC("[APPL]: Battery Timeout Occurred: %p\n", timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }

        retval = BT_start_timer
                 (
                     &timer_handle,
                     1U,
                     appl_batt_timer_expiry_handler,
                     handle,
                     sizeof (GATT_DB_HANDLE)
                 );

        APPL_TRC(
        "[APPL]: Started Battery Timer with result 0x%04X, timer handle %p\n",
        retval, timer_handle);
    }
    else if (GATT_CLI_CNFG_DEFAULT == config)
    {
        /* Stop simulated transfer of Battery Level Notifications */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);

            APPL_TRC (
            "[APPL]: Stopped Battery Timer %p\n", timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
    else
    {
        /* empty */
    }

    return retval;
}

void appl_batt_timer_expiry_handler(void * data, UINT16 datalen)
{
    API_RESULT            retval;
    ATT_VALUE             value;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    GATT_DB_HANDLE        * db_hndl;
    APPL_HANDLE           appl_handle;
    ATT_ATTR_HANDLE       appl_bat_lvl_hndl;

    BT_IGNORE_UNUSED_PARAM(datalen);

    retval = API_FAILURE;
    timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    appl_handle = 0U;

    /* Fetch the GATT DB Handle */
    db_hndl = ((GATT_DB_HANDLE *)data);

    /* Fetch the application handle */
    (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle(db_hndl->device_id, &appl_handle);

    if (HCI_INVALID_CONNECTION_HANDLE != APPL_GET_CONNECTION_HANDLE(appl_handle))
    {
        if (GET_GATT_CHARACTERISTIC_COUNT() > db_hndl->char_id)
        {
            /* Get Battery Level Attribute Handle */
            retval = BT_gatt_db_get_char_val_hndl
                     (
                         db_hndl,
                         &appl_bat_lvl_hndl
                     );
        }

        if (API_SUCCESS == retval)
        {
            BT_mem_set(&value, 0x00, sizeof(ATT_VALUE));
            retval = BT_gatt_db_get_char_val(db_hndl, &value);

            if (API_SUCCESS == retval)
            {
                hndl_val_param.handle    = appl_bat_lvl_hndl;
                hndl_val_param.value.val = value.val;
                hndl_val_param.value.len = 1U;

                retval = BT_att_send_hndl_val_ntf
                         (
                             &APPL_GET_ATT_INSTANCE(appl_handle),
                             &hndl_val_param
                         );
            }
        }

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[** ERR **]: Failed to send Battery Level, reason 0x%04X\n",
            retval);
        }
        else
        {
            retval = BT_start_timer
                     (
                        &timer_handle,
                        APPL_BATT_NTF_TIMEOUT,
                        appl_batt_timer_expiry_handler,
                        db_hndl,
                        sizeof (GATT_DB_HANDLE)
                     );

            APPL_TRC(
            "[APPL]: Started Timer with result 0x%04X, timer handle %p\n",
            retval, timer_handle);
        }
    }

    return;
}

/* Handler to send Service Changed Indications */
API_RESULT appl_send_serv_changed_ind(GATT_DB_HANDLE * serv_change_handle)
{
    ATT_HNDL_VAL_IND_PARAM hvi_param;
    APPL_HANDLE            appl_handle;
    UCHAR                  serv_change_val[4U];
    API_RESULT             retval;
    ATT_ATTR_HANDLE        t_handle;
    ATT_ATTR_HANDLE        appl_serv_chg_hndl;

    /* Fetch the Application Handle */
    retval = appl_get_handle_from_device_handle
             (
                 serv_change_handle->device_id,
                 &appl_handle
             );

    if (API_SUCCESS == retval)
    {
        /* Get Service Changed Value Handle */
        BT_gatt_db_get_char_val_hndl
        (
            serv_change_handle,
            &appl_serv_chg_hndl
        );

        t_handle = 0x0001U;
        BT_PACK_LE_2_BYTE(serv_change_val, &t_handle);
        t_handle = 0xFFFFU;
        BT_PACK_LE_2_BYTE(serv_change_val+2U, &t_handle);

        hvi_param.handle = appl_serv_chg_hndl;
        hvi_param.value.val = serv_change_val;
        hvi_param.value.len = 4U;

        retval = BT_att_send_hndl_val_ind
                    (
                        &APPL_GET_ATT_INSTANCE(appl_handle),
                        &hvi_param
                    );
    }

    return retval;
}

void appl_get_local_time(UCHAR *buf, UINT16 buf_len)
{
    BT_LOCAL_TIME tmp_time;
    UCHAR         marker;

    if (9U <= buf_len)
    {
        marker = 0U;
        BT_get_local_time(&tmp_time);

        /* Year */
        BT_PACK_LE_2_BYTE(&buf[marker], &tmp_time.dyear);
        marker += 2U;

        /* Month */
        buf[marker++] = (UCHAR)tmp_time.dmonth;

        /* Date */
        buf[marker++] = (UCHAR)tmp_time.dday;

        /* Hour */
        buf[marker++] = (UCHAR)tmp_time.thour;

        /* Minute */
        buf[marker++] = (UCHAR)tmp_time.tmin;

        /* Second */
        buf[marker++] = (UCHAR)tmp_time.tsec;

        /* Day of week */
        buf[marker++] = (UCHAR)tmp_time.dwday;

        /* Fraction256 */ /* TODO: Check this value */
        buf[marker] = (UCHAR)tmp_time.tmsec;
    }
    else
    {
        /* Not populating any Value here as the required Buffer is less */
        APPL_ERR(
        "[APPL]: **ERR**: Insufficient Buffer provided for Get Local Time!\n");
    }

    return;
}

/*
*  Add Target Address at the end of pre-configured
*  advertising data associated with specific profile/service.
*
*  \param offset:
*         End index of the pre-configured advertising data.
*         Target Adress AD Type to be added from this offset.
*
*  \note:
*  This function does not check if adding Target Address AD type
*  exceeds specification defined advertising data length.
*/
void appl_set_target_address_in_adv_data(UCHAR offset)
{
#if ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL))
    /* Add Target Address AD Type Length */
    appl_gap_adv_data.data[offset] =
        BT_BD_ADDR_SIZE + 1U;

    /* Add Target Address AD Type. Public/Random */
    if (BT_BD_PUBLIC_ADDRESS_TYPE == g_bd_addr.type)
    {
        appl_gap_adv_data.data[offset + 1U] =
            HCI_AD_TYPE_PUBLIC_TARGET_ADDRESS;
    }
    else
    {
        appl_gap_adv_data.data[offset + 1U] =
            HCI_AD_TYPE_RANDOM_TARGET_ADDRESS;
    }

    /* Add Target Address */
    BT_COPY_BD_ADDR
    (
        &appl_gap_adv_data.data[offset + 2U],
        g_bd_addr.addr
    );

    /* Update the actual length of advertising data */
    appl_gap_adv_data.datalen = offset + BT_BD_ADDR_SIZE + 2U;
#endif /* ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL)) */
}

API_RESULT appl_gatt_db_find_service_info_from_inst
           (
                UCHAR           gatt_service_instance,
                GATT_DB_SERVICE * service_info
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* Validate the incoming service instance */
    if (GET_GATT_SERVICE_COUNT() <= gatt_service_instance)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* TODO: Update */
        service_info->start_handle = gatt_service_g[gatt_service_instance].start_handle;
        service_info->end_handle = gatt_service_g[gatt_service_instance].end_handle;
    }

    return retval;
}


#ifdef APPL_ENABLE_AUTHORIZATION
UCHAR appl_get_gatt_server_authorization_state(DEVICE_HANDLE dq_handle)
{
    /**
     * Currently not using the incoming Device ID.
     * TODO: Have Authorization based on Peer Device Instantce.
     */
    BT_IGNORE_UNUSED_PARAM(dq_handle);

    /* Return the Authorization Flag value */
    return (UCHAR)appl_gatt_server_authorization_flag;
}

void  appl_set_gatt_server_authorization_state
      (
          DEVICE_HANDLE dq_handle,
          UCHAR         state
      )
{
    /**
     * Currently not using the incoming Device ID.
     * TODO: Have Authorization based on Peer Device Instantce.
     */
    BT_IGNORE_UNUSED_PARAM(dq_handle);

    /* Set the incoming State to the Authorization Flag */
    appl_gatt_server_authorization_flag = state;
}

API_RESULT appl_check_attr_dev_authorization
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           )
{
    ATT_ATTR_HANDLE  attr_handle;
    API_RESULT       retval;
    UCHAR            status_flag;

    BT_IGNORE_UNUSED_PARAM(event_result);

    retval = API_SUCCESS;
    status_flag = BT_FALSE;

    if (event_datalen < 2U)
    {
        /* Return Failure as length is less */
        retval = API_FAILURE;
    }

    if (API_SUCCESS == retval)
    {
        /* Extract the Attribute Handle being requested */
        BT_UNPACK_LE_2_BYTE
        (
            &attr_handle,
            event_data
        );

        if (
               ((att_event != ATT_CONNECTION_IND) &&
               (att_event != ATT_DISCONNECTION_IND)) &&
               ((att_event == ATT_READ_REQ ) ||
               (att_event == ATT_READ_BLOB_REQ) ||
               (att_event == ATT_READ_MULTIPLE_REQ ) ||
               (att_event == ATT_WRITE_REQ ) ||
               (att_event == ATT_WRITE_CMD ) ||
               (att_event == ATT_SIGNED_WRITE_CMD ) ||
               (att_event == ATT_PREPARE_WRITE_REQ ) ||
               (att_event == ATT_READ_BY_TYPE_REQ ) ||
               (att_event == ATT_READ_BY_GROUP_REQ ) ||
               (att_event == ATT_EXECUTE_WRITE_REQ ))
           )
        {
            if ((event_datalen >= 4U) && (att_event == ATT_READ_BY_TYPE_REQ ))
            {
                UINT16 start_hndl;
                UINT16 end_hndl;

                /* Extract Start and End Handles */
                BT_UNPACK_LE_2_BYTE(&start_hndl, event_data);
                BT_UNPACK_LE_2_BYTE(&end_hndl, &event_data[2U]);

                if (0xFFFFU == end_hndl)
                {
                    /* Do Nothing */
                    retval = API_FAILURE;
                }
            }

            if ((event_datalen >= 4U) && (att_event == ATT_READ_MULTIPLE_REQ ))
            {
                UCHAR hndl_count;

                hndl_count = 0x00U;
                do
                {
                    BT_UNPACK_LE_2_BYTE
                    (
                        &attr_handle,
                        (&event_data[(hndl_count * 2U)])
                    );

                    hndl_count++;

                    retval = (BT_TRUE != GATT_CHK_ATTR_HNDL_AUTHORIZATION(attr_handle)) ?
                             API_FAILURE : API_SUCCESS;

                }while (event_datalen > (hndl_count * 2U));
            }

            if ((GATT_DB_MAX_ATTRIBUTES) <= attr_handle)
            {
                /* Do Nothing */
                retval = API_FAILURE;
            }

            if (API_SUCCESS == retval)
            {
                /* Check the Authorization for the handle based on type of event */
                if (BT_TRUE == GATT_CHK_ATTR_HNDL_AUTHORIZATION(attr_handle))
                {
                    /* check if the application authorization flag is set */
                    if (BT_TRUE != APPL_GET_AUTHORIZATION(handle->device_id))
                    {
                        /* Send Authorization error */
                        retval = appl_handl_authorization_err
                                 (
                                     handle,
                                     att_event,
                                     attr_handle
                                 );

                        if (API_SUCCESS == retval)
                        {
                            /*
                             * Assign Failure retval to avoid further
                             * processing in parent function
                             */
                            status_flag = BT_TRUE;

                            /* MISRA C - 2012 Rule 2.2 */
                            /* retval = API_FAILURE; */
                        }
                    }
                }
            }
        }
    }

    /* Return based on the value of the Status Flag set above */
    return (status_flag == BT_TRUE) ? API_FAILURE : API_SUCCESS;
}

API_RESULT appl_handl_authorization_err
           (
               ATT_HANDLE       * handle,
               UCHAR            event_code,
               ATT_ATTR_HANDLE  attr_handle
           )
{
    ATT_ERROR_RSP_PARAM err_param;
    API_RESULT          retval;

    err_param.op_code = event_code;
    err_param.handle  = attr_handle;

    err_param.error_code = ATT_INSUFFICIENT_AUTHORIZATION;

    retval = BT_att_send_error_rsp
             (
                 handle,
                 &err_param
             );

    return retval;
}
#endif /* APPL_ENABLE_AUTHORIZATION */
#endif /* GATT_DB */

void appl_service_parse_hv_cnf (ATT_HANDLE * handle, UINT16 evt_result)
{
    APPL_HANDLE appl_hndl;
    API_RESULT  retval;

    retval = appl_get_handle_from_device_handle(handle->device_id, &appl_hndl);

    APPL_PROFILE_HV_IND_COMPLETE_HANDLER(&appl_hndl, evt_result);

    BT_IGNORE_UNUSED_PARAM(evt_result);
    BT_IGNORE_UNUSED_PARAM(retval);
}

void appl_service_parse_ntf_tx_complete(ATT_HANDLE * handle, UCHAR * data, UINT16 datalen)
{
    APPL_HANDLE appl_hndl;
    API_RESULT  retval;

    retval = appl_get_handle_from_device_handle(handle->device_id, &appl_hndl);

    APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(&appl_hndl, data, datalen);

    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);
    BT_IGNORE_UNUSED_PARAM(appl_hndl);
    BT_IGNORE_UNUSED_PARAM(retval);
}

void appl_service_mtu_updt_handler(ATT_HANDLE * handle, UINT16 mtu)
{
    APPL_HANDLE appl_hndl;
    API_RESULT  retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    appl_hndl = 0U;

    retval = appl_get_handle_from_device_handle(handle->device_id, &appl_hndl);

    /* Save the Negotiated MTU to Global */
    APPL_SET_MTU(appl_hndl, mtu);

    APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(&appl_hndl, mtu);

    BT_IGNORE_UNUSED_PARAM(retval);
}

#endif /* BT_LE */

