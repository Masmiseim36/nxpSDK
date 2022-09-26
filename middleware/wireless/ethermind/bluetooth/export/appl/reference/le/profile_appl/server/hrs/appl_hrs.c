
/**
 *  \file appl_hrs.c
 *
 *  This file contains the Heart Rate Sensor application.
  *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery
 *           - Device Information and
 *           - Heart Rate
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Heart Rate Service exposes:
 *           - Heart Rate Measurement
 *           - Sensor Location
 *           - Heart Rate Control Point
 *      d. Heart Rate Measurement can be configured to send Notifications.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Measurement Transfer by writing to the
 *             Characteristic Client Configuration of Heart Rate Measurement.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_hrs_handler and
 *           - appl_hrs_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements every
 *         1s. This interval can be altered using the define
 *         APPL_HEART_RATE_MSURMNT_INTERVAL. In absence of a real sensor
 *         to provide heart rate readings, a static reading of 86 bpm is being
 *         sent. Energy Expended is incremented every 5 seconds. This value can
 *         be reset reset by the Collector device through the Heart Rate
 *         Control Point. See appl_hr_control_point_handler for more details.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined BT_HRS)

/* --------------------------------------------- Constants */
#define APPL_HEART_RATE_MSRMNT_LENGTH          4U
#define APPL_HEART_RATE_MSURMNT_INTERVAL       1U
#define APPL_HEART_RATE_MSURMNT_COUNT          2U

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

static UCHAR hr_obs_data[] =
{
    /* Heart Rate Value 1 Byte with Energy Expended */
    0x08U, 0x56U, 0x00U, 0x00U,

    /* Heart Rate Value 2 Bytes with RR Interval and Sensor Contact */
    0x15U, 0x56U, 0x00U, 0x00U, 0x00U
};

static APPL_HRS_OBS_DATA_INFO hr_obs_data_info[APPL_HEART_RATE_MSURMNT_COUNT] =
{
    {
        /* Index */
        0x00U,

        /* Length*/
        0x04U
    },
    {
        /* Index */
        0x04U,

        /* Length */
        0x05U
    }
};

static UCHAR hr_count;
static GATT_DB_HANDLE appl_hrs_handle;
static BT_timer_handle timer_handle;
static ATT_ATTR_HANDLE appl_hr_hndl;

static GATT_DB_HANDLE  appl_hrs_msrmt_db_handle;
static GATT_DB_HANDLE  appl_body_sensor_loc_db_handle;
static GATT_DB_HANDLE  appl_hrs_cp_db_handle;
/* --------------------------------------------- Functions */

void appl_hrs_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_hrs_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_hrs_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_hrs_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_hrs_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_hrs_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_hrs_add_hrs();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the HRS GATT DB Signature */
    appl_hrs_db_sign = APPL_HRS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_hrs_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    /* Register the GATT DB */
    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[HRS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for Heart rate measurement */
    appl_hrs_msrmt_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hrs_msrmt_db_handle.service_id = (UCHAR)GATT_SER_HRS_HEART_RATE_INST;
    appl_hrs_msrmt_db_handle.char_id    = (UCHAR)GATT_CHAR_HRS_HR_MSRMNT;

    /* Populate the GATT DB HANDLE for Body sensor location */
    appl_body_sensor_loc_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_body_sensor_loc_db_handle.service_id = (UCHAR)GATT_SER_HRS_HEART_RATE_INST;
    appl_body_sensor_loc_db_handle.char_id    = (UCHAR)GATT_CHAR_HRS_BODY_SNSR_LOC_INST;

    /* Populate the GATT DB HANDLE for Heart rate control point */
    appl_hrs_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hrs_cp_db_handle.service_id = (UCHAR)GATT_SER_HRS_HEART_RATE_INST;
    appl_hrs_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_HRS_HR_CNTRL_PNT_INST;

#ifndef GATT_DB_DYNAMIC
    appl_hrs_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register HRS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_hrs_gatt_char_handler);

    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
}

void appl_hrs_bt_init(void)
{
    APPL_TRC("[HRS]: In appl_hrs_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_hrs_server_reinitialize ();

}

void appl_hrs_bt_deinit(void)
{
    APPL_TRC("[HRS]: In appl_hts_bt_deinit invoked\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle= BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_hrs_connect(APPL_HANDLE  * appl_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;
    API_RESULT        retval;

    cli_cnfg = 0U;

    appl_hrs_handle.device_id  = APPL_GET_DEVICE_HANDLE((*appl_handle));
    /* If Heart Rate measurement is configured, initiate transfer */
    appl_hrs_handle.char_id    = (UCHAR)GATT_CHAR_HRS_HR_MSRMNT;
    appl_hrs_handle.service_id = (UCHAR)GATT_SER_HRS_HEART_RATE_INST;

    BT_gatt_db_get_char_val_hndl (&appl_hrs_handle,&appl_hr_hndl);
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_hrs_handle,&value);
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC (
    "[HRS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, APPL_GET_DEVICE_HANDLE((*appl_handle)));

    appl_manage_trasnfer(appl_hrs_handle, cli_cnfg);
}

void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    APPL_HANDLE appl_handle;
    API_RESULT  retval;

    /* Init */
    retval = API_SUCCESS;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        /* Security requirements satisfied, go ahead with data transfer */
        retval = appl_get_handle_from_device_handle(handle.device_id,&appl_handle);

        if (API_SUCCESS == retval)
        {
            if (GATT_CLI_CNFG_NOTIFICATION == config)
            {
                /* Start Transfer Timer to Send measurement every second */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    BT_stop_timer(timer_handle);
                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                BT_start_timer
                (
                    &timer_handle,
                    APPL_HEART_RATE_MSURMNT_INTERVAL,
                    appl_timer_expiry_handler,
                    &appl_handle,
                    sizeof(appl_handle)
                );

                APPL_TRC(
                "[HRS]: Heart rate Measurement Timer %p Started!\n",
                timer_handle);
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                /* Turn Off Transfer Timer if On */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    BT_stop_timer(timer_handle);
                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }
            }
            else
            {
                /* Incorrect Configuration */
            }
        }
    }
}

void appl_send_heart_rate_measurement (APPL_HANDLE   * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UINT16                indx;

    retval = API_FAILURE;

    APPL_TRC (
    "[HRS]: [0x%02X]:Sending measurement On Handle 0x%04X\n",
    (*handle), appl_hr_hndl);

    for (indx = 0U; indx < APPL_HEART_RATE_MSURMNT_COUNT; indx++)
    {
        hndl_val_param.handle =  appl_hr_hndl;
        hndl_val_param.value.val = &hr_obs_data[hr_obs_data_info[indx].index];
        hndl_val_param.value.len = hr_obs_data_info[indx].length;

        if ((1U == indx) && (1U == hr_count))
        {
            hr_obs_data[hr_obs_data_info[indx].index] = 0x17U;
        }

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );

        /* Simulate an incremental value */
        hr_obs_data[hr_obs_data_info[indx].index + 1U] ++;
    }

    if (5U == hr_count)
    {
        if (0xFFU == hr_obs_data[0x02U])
        {
            hr_obs_data[0x03U]++;
        }
        hr_obs_data[0x02U]++;
        hr_obs_data[hr_obs_data_info[1U].index] = 0x15U;
        hr_count = 0U;
    }

    hr_count++;

    if (API_SUCCESS != retval)
    {
        APPL_ERR (
        "[HRS]: **ERR** Failed to send measurement, reason 0x%04X",
        retval);
    }
}

void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    APPL_EVENT_PARAM    fsm_param;
    fsm_param.handle = (*((APPL_HANDLE *)data));

    BT_IGNORE_UNUSED_PARAM(datalen);

    /* Restart timer to simulate Heart Rate measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC (
        "[HRS]: Heart Rate Measurement Timeout occurred: %p\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &timer_handle,
        APPL_HEART_RATE_MSURMNT_INTERVAL,
        appl_timer_expiry_handler,
        &fsm_param.handle,
        sizeof (fsm_param.handle)
    );

    APPL_TRC (
    "[HRS]: Heart Rate Measurement Timer %p Started\n",
    timer_handle);

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

void appl_hrs_server_reinitialize (void)
{
    /* Turn Off Transfer Timer if On.  */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    hr_count = 0U;

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    /* Configure and Enable Advertising */
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U,
                                   0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    /* Configure and Enable Scanning */
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U,
                                    0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

API_RESULT appl_hr_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           )
{
    API_RESULT retval;

    retval = APPL_HR_CNTRL_PNT_NOT_SUPPORTED;

    BT_IGNORE_UNUSED_PARAM(handle);

    if ((1U == value->len) && (0x01U == value->val[0U]))
    {
        hr_obs_data[2U] = 0x00U;
        hr_obs_data[3U] = 0x00U;
        hr_count = 0U;
        retval = API_SUCCESS;
    }

    return retval;
}

void appl_hrs_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    APPL_TRC("\n[HRS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_hrs_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_HRS_HAVE_DATA_DUMP
    APPL_TRC("\n[HRS]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_HRS_HAVE_DATA_DUMP */
}

void appl_hrs_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[HRS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* (defined ATT && defined BT_HRS) */

