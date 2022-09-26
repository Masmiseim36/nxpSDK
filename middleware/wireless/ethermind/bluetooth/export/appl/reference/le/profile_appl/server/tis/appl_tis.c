
/**
 *  \file appl_tis.c
 *
 *  This file contains the Time Server application which
 *  contains CTS, RTUS and NDCS services.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined TIS)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

static GATT_DB_HANDLE appl_cts_db_handle;

BT_timer_handle timer_handle;
static ATT_ATTR_HANDLE appl_cur_time_hndl;

/**
 * Current Time constitutes of the following:
 * # Date Time     [7 Octets]
 *   - Year        [2 Octets]
 *   - Month       [1 Octet]
 *   - Day         [1 Octet]
 *   - Hours       [1 Octet]
 *   - Minutes     [1 Octet]
 *   - Seconds     [1 Octet]
 * # Day of Week   [1 Octet]
 * # Fractions256  [1 Octet]
 * # Adjust Reason [1 Octet]
 *   - BIT0: Manual time update
 *   - BIT1: External reference time update
 *   - BIT2: Change of time zone
 *   - BIT3: Change of DST (daylight savings time)
 */
/*
 * Setting Adjust Reason as 0x0F
 * - Manual Time Update
 * - External reference time update
 * - Change of time zone
 * - Change of DST (daylight savings time)
 */
static UCHAR cur_time[APPL_CUR_TIME_LENGTH] =
             {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x0FU};

static UCHAR loc_time[APPL_LOC_TIME_LENGTH] = {0x33U, 0x02U};

static UCHAR ref_time[APPL_REF_TIME_LENGTH] = {0x04U, 0x32U, 0x32U, 0x14U};

UCHAR curr_time_set;
static UCHAR cur_time_count;

/* Using Adjust Reason as 0x01 (Manual Time Update) */
static UCHAR ndcs_time[APPL_NDCS_TIME_LENGTH] =
{
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
};
APPL_HANDLE    appl_handle;
UCHAR adj_time;

/*--------------------------------------------Global variables */

/* --------------------------------------------- Functions */
void appl_tis_set_gap_central_role(void)
{
    APPL_SET_GAP_CENTRAL_ROLE();

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    appl_service_enable_adv(0x00U);
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan (APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_tis_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_tis_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_cts();
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_rtus();
    (BT_IGNORE_RETURN_VALUE) appl_tis_add_ndcs();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the TIS GATT DB Signature */
    appl_tis_db_sign = APPL_TIS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_tis_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[TIS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for Current Time */
    appl_cts_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_cts_db_handle.service_id = (UCHAR)GATT_SER_TIS_CUR_TIME_INST;
    appl_cts_db_handle.char_id    = (UCHAR)GATT_CHAR_TIS_CUR_TIME_INST;

#ifndef GATT_DB_DYNAMIC
    appl_tis_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register TIS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_tis_gatt_char_handler);

#ifdef BT_DUAL_MODE
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_TIS_CUR_TIME_INST,
        DB_RECORD_CTS
    );
#endif

    timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    /**
     * Default GAP Role is Central for this Service.
     * But, during init time setting the role to Peripheral.
     * This needs to be updated from the TIS Menu after the
     * desired Peer Device Address to which this needs to scan and
     * initiate connection is Set.
     * User needs to select the role from the Menu.
     */
    /* NOTE: Printing the Above Text in Application */
    APPL_TRC(
    "\n=====================================================================\n"
    "                          NOTE to USER\n"
    "                          ----------\n");
    APPL_TRC(
    "Default GAP Role is GAP Central for this TIS Service. But, during\n"
    "init time setting the role to Peripheral. This needs to be updated\n"
    "from the TIS Menu after the desired Peer Device Address to which this\n"
    "service needs to scan and auto initiate connection is Set.\n"
    "\n        ** User needs to select the role from the TIS Menu **");
    APPL_TRC(
    "\n=====================================================================\n\n");
}

void appl_tis_bt_init(void)
{
    APPL_TRC("[TIS]: In appl_tis_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_tis_server_reinitialize();
}

void appl_tis_bt_deinit(void)
{
    APPL_TRC("[TIS]: In appl_tis_bt_deinit\n");

     /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle= BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_tis_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;

    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_cts_db_handle.device_id = (*dq_handle);

    /* If Current Time is configured, initiate transfer */
    BT_gatt_db_get_char_val_hndl (&appl_cts_db_handle,&appl_cur_time_hndl);
    BT_gatt_db_get_char_cli_cnfg(&appl_cts_db_handle,&value);
    BT_UNPACK_LE_2_BYTE (&cli_cnfg,value.val);

    APPL_TRC(
    "[TIS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    appl_manage_trasnfer(appl_cts_db_handle, cli_cnfg);
}


void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    API_RESULT     retval;
    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        /* Security requirements satisfied, go ahead with data transfer */
        appl_get_handle_from_device_handle (handle.device_id,&appl_handle);
        if (GATT_CLI_CNFG_NOTIFICATION == config)
        {
            /* Start Transfer Timer to Send measurement every time interval */
            if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
            {
                BT_stop_timer (timer_handle);
                timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }

            BT_start_timer
            (
                &timer_handle,
                APPL_CUR_TIME_MEASUREMENT_INTERVAL,
                appl_timer_expiry_handler,
                &appl_handle,
                sizeof (appl_handle)
            );

            APPL_TRC(
            "[TIS]: Current Time Timer %p Started!\n", timer_handle);
        }
        else if (GATT_CLI_CNFG_DEFAULT == config)
        {
            /* Turn Off Transfer Timer if On */
            if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
            {
                BT_stop_timer (timer_handle);
                timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }
        }
        else
        {
            /* Incorrect Configuration */
        }
    }
}

void appl_send_cur_time (APPL_HANDLE   * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    APPL_TRC(
    "[TIS]: Sending Current Time On Handle 0x%04X\n",
    appl_cur_time_hndl);

    /* Read Current Time */
    appl_get_local_time (cur_time, sizeof(cur_time));

    if (0U != adj_time)
    {
        cur_time[5U] += adj_time;
        adj_time = 20U;
    }

    /* Using Adjust Reason always as 0x00 */
    hndl_val_param.handle =  appl_cur_time_hndl;
    hndl_val_param.value.val = cur_time;
    hndl_val_param.value.len = APPL_CUR_TIME_LENGTH;

    retval = BT_att_send_hndl_val_ntf
             (
                 &APPL_GET_ATT_INSTANCE(*handle),
                 &hndl_val_param
              );
    cur_time_count++;
    if (API_SUCCESS != retval)
    {
        APPL_ERR(
        "[TIS]: **ERR** Failed to send measurement, reason 0x%04X",retval);
    }
}

API_RESULT appl_cts_get_current_time
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           )
{
    API_RESULT     retval;
    ATT_VALUE      updated_time;
    ATT_VALUE      local_time;
    ATT_VALUE      reference_time;

    BT_IGNORE_UNUSED_PARAM(value);

    retval = API_SUCCESS;

    if (GATT_CHAR_TIS_CUR_TIME_INST == handle->char_id)
    {
        if (curr_time_set != BT_TRUE)
        {
            /* Read Current Time */
            appl_get_local_time(cur_time, sizeof(cur_time));
        }

        /* Update Current Time in GATT DB */
        updated_time.val = cur_time;
        updated_time.len = sizeof(cur_time);

        BT_gatt_db_set_char_val
        (
            handle,
            &updated_time
        );
    }
    else if(GATT_CHAR_TIS_LCL_TIME_INFO_INST == handle->char_id)
    {
        local_time.val = loc_time;
        local_time.len = sizeof(loc_time);

        BT_gatt_db_set_char_val
        (
            handle,
            &local_time
        );
    }
    else if (GATT_CHAR_TIS_REF_TIME_INFO_INST == handle->char_id)
    {
        reference_time.val = ref_time;
        reference_time.len = sizeof(ref_time);

        BT_gatt_db_set_char_val
        (
            handle,
            &reference_time
        );
    }
    else
    {
        retval = GATT_DB_INVALID_OPERATION;
    }

    return retval;
}

void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    APPL_EVENT_PARAM    fsm_param;

    BT_IGNORE_UNUSED_PARAM(datalen);

    fsm_param.handle = (*((APPL_HANDLE *)data));

    /* Restart timer to simulate Current Time measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC(
        "[TIS]: Current Time Timer %p Timeout Occurred\n",
        timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &timer_handle,
        APPL_CUR_TIME_MEASUREMENT_INTERVAL,
        appl_timer_expiry_handler,
        &fsm_param.handle,
        sizeof (fsm_param.handle)
    );

    APPL_TRC(
    "[TIS]: Current Time Timer %p Started!\n",timer_handle);

#ifdef APPL_FSM_SUPPORT
    /* Post Measurement Indication to APPL FSM */
    fsm_post_event
    (
         APPL_FSM_ID,
         ev_appl_measurement_ind,
         &fsm_param
    );
#else /* APPL_FSM_SUPPORT */
    APPL_SEND_MEASUREMENT(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */
}

void appl_tis_server_reinitialize (void)
{
    /* Turn Off Transfer Timer if On.  */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TRUE == curr_time_set)
    {
        /*
         * Re-Setting Adjust Reason as 0x0F
         * - Manual Time Update
         * - External reference time update
         * - Change of time zone
         * - Change of DST (daylight savings time)
         *
         * NOTE:
         * Check if the adjust needs to be retained to the value
         * that it was set previously by the Client based on use-case.
         * For some of the PTS testcases, the expectation from the PTS (v 7.6.2)
         * is have all the valid BITs set in the Adjust reason.
         */
        cur_time[APPL_CUR_TIME_LENGTH - 1U] = 0x0FU;
    }

    cur_time_count = 0U;
    curr_time_set = BT_FALSE;

    adj_time = 0U;

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))

    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))

    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

API_RESULT appl_update_current_time (ATT_VALUE    * value)
{
    UINT16 data;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Year */
    BT_UNPACK_LE_2_BYTE(&data, value->val);

    if (!((APPL_YEAR_MAX >= data) && \
        (APPL_YEAR_MIN <= data))||(APPL_YEAR_NOT_KNOWN == data))
    {
        APPL_ERR("[TIS]: **ERR** Illegal Year\n");
        retval =  (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID);  /* return  (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
    }

    if (API_SUCCESS == retval)
    {
        /* Month */
        data = (UCHAR)value->val[2U];

        if (!(APPL_MONTH_MAX >= data) && \
            (APPL_MONTH_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Month\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Date */
        data = (UCHAR)value->val[3U];

        if (!(APPL_DAY_MAX >= data) && \
            (APPL_DAY_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** APPL Day Check FAILED\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Hours */
        data = (UCHAR) value->val[4U];

        if (!(APPL_HOURS_MAX >= data) && \
            (APPL_HOURS_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Hours\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Minutes */
        data = (UCHAR)value->val[5U];

        if (!(APPL_MINUTES_MAX >= data) && \
            (APPL_MINUTES_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Minutes\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }


    if (API_SUCCESS == retval)
    {
        /* Seconds */
        data = (UCHAR)value->val[6U];

        if (!(APPL_SECONDS_MAX >= data) && \
            (APPL_SECONDS_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Seconds\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID);  /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Day of the week */
        data = (UCHAR)value->val[7U];

        if (!(APPL_DAY_OF_WEEK_MAX >= data) && \
            (APPL_DAY_OF_WEEK_MIN <= data))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Day of week\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Fraction256 value is currently ignored */

        /* Adjust Reason */
        data = (UCHAR)value->val[9U];

        /**
         * Check the bits in 2nd nibble(bit4 to bit7) of Adjust Reason field is set
         * by the peer device
         */

        if (0U != (data & 0xF0U))
        {
            APPL_ERR("[TIS]: **ERR** Illegal Adjust Reason\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID);  /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
        BT_mem_copy(cur_time, &value->val[0U], value->len);

        curr_time_set = BT_TRUE;
    }

    return retval;
}

API_RESULT appl_update_local_time (ATT_VALUE    * value)
{
    INT8    data;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Time-zone */
    data = value->val[0U];
    BT_PACK_LE_1_BYTE(&loc_time[0U], &value->val[0U]);

    if (!((APPL_TIMEZONE_MAX >= data) && \
        (APPL_TIMEZONE_MIN <= data)) || \
        (APPL_TIMEZONE_NOT_KNOWN == data))
    {
        APPL_ERR("[TIS]: **ERR** APPL Time-zone Illegal\n");
        retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
    }

    if (API_SUCCESS == retval)
    {
        /* DST Offset */
        data = (UCHAR)value->val[1U];
        BT_PACK_LE_1_BYTE(&loc_time[1U], &value->val[1U]);

        if (!((APPL_DSTOFFSET_MAX >= data) && \
            (APPL_DSTOFFSET_MIN <= data)) || \
            (APPL_DSTOFFSET_NOT_KNOWN == data))

        {
            APPL_ERR("[TIS]: **ERR** APPL DST Illegal\n");
            retval = (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); /* return (APPL_CUR_TIME_ILLEGAL_WRITE | APPL_ERR_ID); */
        }
        else
        {
            BT_mem_copy(loc_time, &value->val[0U], value->len);
        }
    }

    return retval;
}

API_RESULT appl_rtus_config_time_update
           (
               GATT_DB_HANDLE    * handle,
               ATT_VALUE         * value
           )
{
    UCHAR update_cp_value;

    /* Check Characteristic ID */
    if (GATT_CHAR_TIS_TIME_UPDATE_CP_INST == handle->char_id)
    {
        /* TODO: Have appropriate define for the CP value length */
        if (sizeof(UCHAR) == value->len)
        {
            update_cp_value = value->val[0U];

            switch (update_cp_value)
            {
                case 0x01U:
                    APPL_TRC("Get Reference Update\n");
                    break;

                case 0x02U:
                    APPL_TRC("Cancel Reference Update\n");
                    break;

                default:
                    APPL_TRC("Reserved value for Ref Update CP 0x%02X\n",
                    update_cp_value);
                    break;
            }
        }
    }

    return API_SUCCESS;
}

API_RESULT appl_rtus_get_time_update_state
           (
               GATT_DB_HANDLE    * handle,
               ATT_VALUE         * value
           )
{
    ATT_READ_RSP_PARAM rd_rsp_param;
    APPL_HANDLE        t_appl_handle;
    UCHAR              time_update_state[2U];
    API_RESULT         retval;

    BT_IGNORE_UNUSED_PARAM(value);

    retval = API_SUCCESS;
    appl_get_handle_from_device_handle (handle->device_id, &t_appl_handle);

    /* Check Characteristic ID */
    if (GATT_CHAR_TIS_TIME_UPDATE_STATE_INST == handle->char_id)
    {
        /**
         * Returning fixed value.
         *
         * Current State - Idle (0x00)
         * Result - Successful (0x00)
         */
        time_update_state[0U] = 0x00U;
        time_update_state[1U] = 0x00U;

        rd_rsp_param.val = time_update_state;
        rd_rsp_param.len = sizeof(time_update_state);
        rd_rsp_param.actual_len = sizeof(time_update_state);

        retval = BT_att_read_rsp
                 (
                     &APPL_GET_ATT_INSTANCE(t_appl_handle),
                     &rd_rsp_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[TIS]: **ERR** Failed to send Time Update State, reason 0x%04X",
            retval);
        }
        else
        {
            /**
             * Response is already initiated above,
             * so avoid sending response from the below GATT
             * server application layer.
             */
            retval = GATT_DB_ALREADY_RESPONDED;
        }
    }

    return retval;
}

API_RESULT appl_ndcs_get_current_time
           (
                GATT_DB_HANDLE * handle,
                ATT_VALUE      * value
           )
{
    API_RESULT     retval;
    ATT_VALUE      updated_time;

    BT_IGNORE_UNUSED_PARAM(value);

    if (GATT_CHAR_TIS_TIME_W_DST_INST == handle->char_id)
    {
        /*
         * Read Current Time returns Day of Week and Fraction256 fields
         * which are not required for NDCS.
         *
         * NDCS has DST offset field.
         * In this sample example, it is set as Standard Time.
         */

        appl_get_local_time (ndcs_time, sizeof(ndcs_time));

        ndcs_time[APPL_NDCS_DST_INDEX] = APPL_NDCS_DEFAULT_DST_OFFSET;

        /* Update Current Time in GATT DB */
        updated_time.val = ndcs_time;
        updated_time.len = sizeof(ndcs_time) - 1U;

        BT_gatt_db_set_char_val
        (
            handle,
            &updated_time
        );

        /* Return Success */
        retval = API_SUCCESS;
    }
    else
    {
        retval = GATT_DB_INVALID_OPERATION;
    }

    return retval;
}


void appl_tis_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    APPL_TRC("\n[TIS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_tis_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_TIS_HAVE_DATA_DUMP
    APPL_TRC("\n[TIS]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_TIS_HAVE_DATA_DUMP */
}

void appl_tis_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[TIS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* (defined ATT && defined TIS) */

