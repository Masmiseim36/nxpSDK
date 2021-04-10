
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

#include "gatt_db.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_SERVICE" )
#include "iot_logging_setup.h"

#ifdef BT_LE

#ifdef AIOS
#include "appl_aios.h"
#endif /* AIOS */

#ifdef ANS
#include "appl_ans.h"
#endif /* ANS */

#ifdef BASIC
#include "appl_basic.h"
#endif /* BASIC */

#ifdef PTS
#include "appl_pts.h"
#endif /* PTS */

#ifdef BMS
#include "appl_bms.h"
#endif /* BMS */

#ifdef BPS
#include "appl_bps.h"
#endif /* BPS */

#ifdef CGMS
#include "appl_cgms.h"
#endif /* CGMS */

#ifdef CPMS
#include "appl_cpms.h"
#endif /* CPMS */

#ifdef CSCS
#include "appl_cscs.h"
#endif /* CSCS */

#ifdef CTS
#include "appl_cts.h"
#endif /* CTS */

#ifdef ESS
#include "appl_ess.h"
#endif /* ESS */

#ifdef FMT
#include "appl_fmt.h"
#endif /* FMT */

#ifdef FTMS
#include "appl_ftms.h"
#endif /* FTMS */

#ifdef GLS
#include "appl_gls.h"
#endif /* GLS */

#ifdef HID
#include "appl_hid.h"
#endif /* HID */

#ifdef HPS
#include "appl_hps.h"
#endif /* HPS */

#ifdef HRS
#include "appl_hrs.h"
#endif /* HRS */

#ifdef HTS
#include "appl_hts.h"
#endif /* HTS */

#ifdef IPS
#include "appl_ips.h"
#endif /* IPS */

#ifdef IPSPN
#include "appl_ipspn.h"
#endif /* IPSPN */

#ifdef LNS
#include "appl_lns.h"
#endif /* LNS */

#ifdef NDCS
#include "appl_ndcs.h"
#endif /* NDCS */

#ifdef OTS
#include "appl_ots.h"
#endif /* OTS */

#ifdef PASS
#include "appl_pass.h"
#endif /* PASS */

#ifdef POS
#include "appl_pos.h"
#endif /* POS */

#ifdef PXR
#include "appl_pxr.h"
#endif /* PXR */

#ifdef RCSP
#include "appl_rcsp.h"
#endif /* RCSP */

#ifdef RSCS
#include "appl_rscs.h"
#endif /* RSCS */

#ifdef RTUS
#include "appl_rtus.h"
#endif /* RTUS */

#ifdef SPS
#include "appl_sps.h"
#endif /* SPS */

#ifdef TDS
#include "appl_tds.h"
#endif /* TDS */

#ifdef TPMS
#include "appl_tpms.h"
#endif /* TPMS */

#ifdef VSE1
#include "appl_vse1.h"
#endif /* VSE1 */

#ifdef VSE2
#include "appl_vse2.h"
#endif /* VSE2 */

#ifdef WSS
#include "appl_wss.h"
#endif /* WSS */

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
#if 0
/* Battery Service Measurement Related Entries */
static GATT_DB_HANDLE appl_batt_db_handle =
{
    DEVICE_HANDLE_INIT_VAL,
    GATT_SER_BATTERY_INST,
    GATT_CHAR_BATTERY_LVL_INST
};
static ATT_ATTR_HANDLE appl_bat_lvl_hndl;

/* GATT Service Changed Related Entries */
static GATT_DB_HANDLE appl_serv_chg_db_handle =
{
    DEVICE_HANDLE_INIT_VAL,
    GATT_SER_GATT_INST,
    GATT_CHAR_SER_CHNGD_INST
};

/*fix build warning: declared but never used.*/
static ATT_ATTR_HANDLE appl_serv_chg_hndl;
#endif
static BT_timer_handle timer_handle;

#ifdef APPL_ENABLE_AUTHORIZATION
DECL_STATIC UINT16 appl_gatt_server_authorization_flag = BT_FALSE;
#endif /* APPL_ENABLE_AUTHORIZATION */

/* --------------------------------------------- Functions */

void appl_service_init (void)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    BT_mem_set(peer_addr, 0x00, BT_BD_ADDR_SIZE);
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

    //APPL_PROFILE_INIT();
}

void appl_service_connect (APPL_HANDLE * handle)
{
#if (defined APPL_GAP_CENTRAL)
    BT_hci_le_set_scan_enable(0x00, 0x00);
#endif /* (defined APPL_GAP_CENTRAL) */

    //APPL_PROFILE_CONNECT(handle);
}

void appl_service_disconnect (APPL_HANDLE * handle)
{
	if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
	{
		/* Stop the timer on Disconnection */
		BT_stop_timer(timer_handle);
	}

    //APPL_PROFILE_DISCONNECT_HANDLER(handle);
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
    BT_hci_le_set_advertising_data(appl_gap_adv_data.datalen, appl_gap_adv_data.data);

    /* Update the globals, if required */
    advertising_type = adv_type;
    own_addr_type = l_addr_type;
    peer_addr_type = p_addr_type;

    if (NULL != p_addr)
    {
        BT_COPY_BD_ADDR(peer_addr, p_addr);
    }

    /* Set Normal Advertising Parameters */
    BT_hci_le_set_advertising_parameters
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
    BT_hci_le_set_advertising_enable(enable);
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
    BT_hci_le_set_scan_parameters
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
    BT_hci_le_set_scan_enable(enable, 0x00);
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_service_initiate_conn
     (
         UCHAR   l_addr_type,
         UCHAR   p_addr_type,
         UCHAR * p_addr,
         UCHAR   filter_policy,
         UCHAR   use_config
     )
{
#if (defined APPL_GAP_CENTRAL)
    if (BT_TRUE == use_config)
    {
        if ((APPL_CONFIG_PEER_DEVICE_ADDR_TYPE() != p_addr_type) ||
            (0 != BT_mem_cmp(APPL_CONFIG_PEER_DEVICE_ADDR(),\
            p_addr, BT_BD_ADDR_SIZE)))
        {
            return;
        }
    }

    BT_hci_le_create_connection
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
#endif /* (defined APPL_GAP_CENTRAL) */
}

#ifdef GATT_DB
/* Generic Handler to update Battery level */
void appl_update_battery_lvl(DEVICE_HANDLE  * dq_handle)
{
#if 0
/*fix build warning: declared but never used.*/
    API_RESULT   retval;
#endif
    ATT_VALUE    value;
    UCHAR        data;

    data = APPL_BATTERY_PERCENT;
    value.val = &data;
    value.len = 1;
    (void)value;    /*fix build warning: set but never used.*/
#if 0
    appl_batt_db_handle.device_id = (*dq_handle);

    /* Get Battery Level Handle */
    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_batt_db_handle,
                 &appl_bat_lvl_hndl
             );

    retval = BT_gatt_db_set_char_val(&appl_batt_db_handle, &value);

    if (API_FAILURE != retval)
    {
        IotLogDebug(
        "Successfully updated Battery Level, reason 0x%04X\n",
        retval);
    }
    else
    {
        IotLogDebug(
        "[ERR]: Failed to update Battery Level, reason 0x%04X\n",
        retval);
    }
#endif
}

API_RESULT appl_battery_level_handler(GATT_DB_HANDLE * handle, UINT16 config)
{
    API_RESULT    retval;
    APPL_HANDLE   appl_handle;

    retval = API_SUCCESS;

    /* If Notification is Enabled then start Notification */
    if (GATT_CLI_CNFG_NOTIFICATION == config)
    {
        appl_get_handle_from_device_handle(handle->device_id, &appl_handle);

        /* Start timer to simulate Battery Level Notification */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            IotLogDebug("[APPL]: Timeout Occurred: %p\n", timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }

        retval = BT_start_timer
                 (
                     &timer_handle,
                     1,
                     appl_batt_timer_expiry_handler,
                     &appl_handle,
                     sizeof (appl_handle)
                 );

        IotLogDebug(
        "[APPL]: Started Timer with result 0x%04X, timer handle %p\n",
        retval, timer_handle);
    }

    return retval;
}

void appl_batt_timer_expiry_handler(void *data, UINT16 datalen)
{
    API_RESULT    retval;
#if 0
    /*fix build warning: declared but never used.*/
    ATT_VALUE     value;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
#endif
    APPL_HANDLE   appl_handle;

    retval = API_SUCCESS;
    (void)retval;    /*fix build warning: set but never used.*/
    timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    appl_handle = (*((APPL_HANDLE *)data));

    if (HCI_INVALID_CONNECTION_HANDLE == APPL_GET_CONNECTION_HANDLE(appl_handle))
    {
        /* Return if there is no Connection Instance */
        return;
    }
#if 0
    BT_gatt_db_get_char_val(&appl_batt_db_handle, &value);

    hndl_val_param.handle = appl_bat_lvl_hndl;
    hndl_val_param.value.val = value.val;
    hndl_val_param.value.len = 1;

    retval = BT_att_send_hndl_val_ntf
             (
                 &APPL_GET_ATT_INSTANCE(appl_handle),
                 &hndl_val_param
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug(
        "[** ERR **]: Failed to send Battery Level, reason 0x%04X\n",
        retval);
    }

    retval = BT_start_timer
             (
                &timer_handle,
                APPL_BATT_NTF_TIMEOUT,
                appl_batt_timer_expiry_handler,
                &appl_handle,
                sizeof (appl_handle)
             );

    IotLogDebug(
    "[APPL]: Started Timer with result 0x%04X, timer handle %p\n",
    retval, timer_handle);
#endif
}

/* Handler to send Service Changed Indications */
API_RESULT appl_send_serv_changed_ind
           (
               DEVICE_HANDLE dq_handle
           )
{
#if 0
    /*fix build warning: declared but never used.*/
    ATT_HNDL_VAL_IND_PARAM hvi_param;
    UCHAR                  serv_change_val[4];
    ATT_ATTR_HANDLE        t_handle;
#endif
    APPL_HANDLE            appl_handle;
    API_RESULT             retval;

    /* Fetch the Application Handle */
    retval = appl_get_handle_from_device_handle (dq_handle,&appl_handle);
#if 0
    if (API_SUCCESS == retval)
    {
        /* Populate the Service Changed GATT Handle */
        appl_serv_chg_db_handle.device_id  = dq_handle;

        /* Get Service Changed Value Handle */
        BT_gatt_db_get_char_val_hndl
        (
            &appl_serv_chg_db_handle,
            &appl_serv_chg_hndl
        );

        t_handle = 0x0001;
        BT_PACK_LE_2_BYTE(serv_change_val, &t_handle);
        t_handle = 0xFFFF;
        BT_PACK_LE_2_BYTE(serv_change_val+2, &t_handle);

        hvi_param.handle = appl_serv_chg_hndl;
        hvi_param.value.val = serv_change_val;
        hvi_param.value.len = 4;

        retval = BT_att_send_hndl_val_ind
                    (
                        &APPL_GET_ATT_INSTANCE(appl_handle),
                        &hvi_param
                    );
    }
#endif
    return retval;
}

void appl_get_local_time(UCHAR *buf, UINT16 buf_len)
{
    BT_LOCAL_TIME tmp_time;
    UCHAR         marker;

    if (9 <= buf_len)
    {
        marker = 0;
        BT_get_local_time(&tmp_time);

        /* Year */
        BT_PACK_LE_2_BYTE(&buf[marker], &tmp_time.dyear);
        marker += 2;

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
        buf[marker++] = (UCHAR)tmp_time.tmsec;
    }
    else
    {
        /* Not populating any Value here as the required Buffer is less */
        IotLogError(
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
        BT_BD_ADDR_SIZE + 1;

    /* Add Target Address AD Type. Public/Random */
    if (BT_BD_PUBLIC_ADDRESS_TYPE == g_bd_addr.type)
    {
        appl_gap_adv_data.data[offset + 1] =
            HCI_AD_TYPE_PUBLIC_TARGET_ADDRESS;
    }
    else
    {
        appl_gap_adv_data.data[offset + 1] =
            HCI_AD_TYPE_RANDOM_TARGET_ADDRESS;
    }

    /* Add Target Address */
    BT_COPY_BD_ADDR
    (
        appl_gap_adv_data.data + offset + 2,
        g_bd_addr.addr
    );

    /* Update the actual length of advertising data */
    appl_gap_adv_data.datalen = offset + BT_BD_ADDR_SIZE + 2;
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
        return API_FAILURE;
    }

    /* TODO: Update */
    service_info->start_handle = gatt_service_g[gatt_service_instance].start_handle;
    service_info->end_handle = gatt_service_g[gatt_service_instance].end_handle;

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

    retval = API_SUCCESS;
    status_flag = BT_FALSE;

    if (event_datalen < 2)
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
            if ((event_datalen >= 4) && (att_event == ATT_READ_BY_TYPE_REQ ))
            {
                UINT16 start_hndl;
                UINT16 end_hndl;

                /* Extract Start and End Handles */
                BT_UNPACK_LE_2_BYTE(&start_hndl, event_data);
                BT_UNPACK_LE_2_BYTE(&end_hndl, event_data + 2);

                if (0xFFFF == end_hndl)
                {
                    /* Do Nothing */
                    retval = API_FAILURE;
                }
            }

            if ((event_datalen >= 4) && (att_event == ATT_READ_MULTIPLE_REQ ))
            {
                UCHAR hndl_count;

                hndl_count = 0x00;
                do
                {
                    BT_UNPACK_LE_2_BYTE
                    (
                        &attr_handle,
                        (event_data+ (hndl_count * 2))
                    );

                    hndl_count++;

                    retval = (BT_TRUE != GATT_CHK_ATTR_HNDL_AUTHORIZATION(attr_handle)) ?
                             API_FAILURE : API_SUCCESS;

                }while (event_datalen > (hndl_count * 2));
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
                    /* check if the applicaton authorization flag is set */
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
                            retval = API_FAILURE;
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

#endif /* BT_LE */

