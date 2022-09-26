
/**
 *  \file appl_rscs.c
 *
 *  This file contains the Runner's Speed & Cadence Sensor application.
  *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery
 *           - Device Information and
 *           - Runner's Speed & Cadence
 *         [NOTE]: Please see the associated GATT DB related files for more
 *         details of the record.
 *      c. The Running Speed and Cadence Service exposes:
 *           - Runner's Speed & Cadence Measurement
 *           - Sensor Location
 *           - Runner's Speed & Cadence Control Point
 *      d. Runner's Speed & Cadence Measurement can be configured to send
 *         Notifications.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Measurement Transfer by writing to the
 *             Characteristic Client Configuration of RSC Measurement.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_rsc_measurment_handler and
 *           - appl_rscs_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements
 *         periodically. This interval can be altered using the define
 *         APPL_RSC_MSURMNT_INTERVAL. In absence of a real sensor
 *         to provide speed & cadence readings, a static reading is being
 *         sent. Distance is incremented every 5 seconds. This value can
 *         be reset by the Collector device through the  Runner's Speed
 *         & Cadence Control Point. See appl_rsc_control_point_handler for more
 *         details.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined RSCS)

/* --------------------------------------------- Constants */
#define APPL_RSC_MSRMNT_LENGTH                              10U
#define APPL_RSC_MSRMNT_INTERVAL                            5U

/* Speed and Cadence Control Point Commands */
#define APPL_RSC_SET_CUMMULATIVE_VALUE_OPCODE               1U
#define APPL_RSC_START_SENSOR_CALIBRATION_OPCODE            2U
#define APPL_RSC_UPDATE_SENSOR_LOCATION_OPCODE              3U
#define APPL_RSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE   4U
#define APPL_RSC_RESPONSE_CODE_OPCODE                       16U

/* Speed and Cadence Control Point Response Codes */
#define APPL_RSC_SUCCESS                                    1U
#define APPL_RSC_OPCODE_NOT_SUPPORTED                       2U
#define APPL_RSC_INVALID_PARAMETER                          3U
#define APPL_RSC_OPERATION_FAILED                           4U

#define APPL_RSC_CUMMULATIVE_VALUE_LENGTH                   4U
#define APPL_RSC_RESPONSE_LENGTH                            3U

/* Supported Sensor Location Count */
#define APPL_RSC_SUPPORTED_SENSOR_LOC_COUNT                 4U

/* Application Error Codes */
#define APPL_RSC_PROCEDURE_ALREADY_IN_PROGRESS          (APPL_ERR_ID | 0x80U)
#define APPL_RSC_CCD_IMPROPERLY_CONFIGURED              (APPL_ERR_ID | 0x81U)

/* --------------------------------------------- External Global Variables */
extern UCHAR appl_hvc_flag;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Instantaneous speed of 7 m/s and and 100 rpm as Instantaneous Cadence */
static UCHAR rsc_obs_data[APPL_RSC_MSRMNT_LENGTH] =
{
    0x03U, 0x00U, 0x07U, 0x64U, 0x78U, 0x00U, 0xD4U, 0x30U, 0x00U, 0x00U
};
static UCHAR rsc_supported_sensor_location[] =
{
    0x02U, 0x03U, 0x07U, 0x08U
};


/* Timer related Globals */
static BT_timer_handle timer_handle;
static BT_timer_handle cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_batt_db_handle;
static GATT_DB_HANDLE  appl_rsc_sc_cp_db_handle;
static GATT_DB_HANDLE  appl_rsc_msrmt_db_handle;
/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_rsc_hndl;
static ATT_ATTR_HANDLE appl_rsc_sc_cp_hndl;
static ATT_ATTR_HANDLE appl_rsc_msrmt_handle;

/* Service data and states related Globals */
static UINT16          appl_msrmt_intrvl;
static UCHAR           rsc_count;
static UCHAR           appl_cntrl_point_cnfgd;
static UCHAR           appl_snsr_calib_error;
/* --------------------------------------------- Functions */

void appl_rscs_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_rscs_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

    /* Initialize the Measurement Interval */
    appl_msrmt_intrvl = APPL_RSC_MSRMNT_INTERVAL;

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_rscs_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_rscs_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_rscs_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_rscs_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_rscs_add_rscs();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the CPMS GATT DB Signature */
    appl_rscs_db_sign = APPL_RSCS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_rscs_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[RSCS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */

    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

 /* Populate the GATT DB HANDLE for RSC Measurement and RSC Control Point */
    appl_rsc_sc_cp_db_handle.device_id   = DEVICE_HANDLE_INIT_VAL;
    appl_rsc_sc_cp_db_handle.service_id  = (UCHAR)GATT_SER_RSCS_RSC_INST;
    appl_rsc_sc_cp_db_handle.char_id     = (UCHAR)GATT_CHAR_RSCS_SC_CNTRL_PNT_INST;

    appl_rsc_msrmt_db_handle.device_id   = DEVICE_HANDLE_INIT_VAL;
    appl_rsc_msrmt_db_handle.service_id  = (UCHAR)GATT_SER_RSCS_RSC_INST;
    appl_rsc_msrmt_db_handle.char_id     = (UCHAR)GATT_CHAR_RSCS_RSC_MSRMNT_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for RSCS services if Dynamic GATT Database is not Used */
    appl_rscs_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register RSCS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_rscs_gatt_char_handler);

#ifdef BT_DUAL_MODE
    /* Update RSCS Service Range in SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_RSCS_RSC_INST,
        DB_RECORD_RSC
    );
#endif
}

void appl_rscs_bt_init(void)
{
    APPL_TRC("[RSCS]: In appl_rscs_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_rscs_server_reinitialize();
}

void appl_rscs_bt_deinit(void)
{
    APPL_TRC("[RSCS]: In appl_rscs_bt_deinit\n");

     /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle= BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        BT_stop_timer(timer_handle);
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_rscs_connect(DEVICE_HANDLE* dq_handle)
{
    ATT_VALUE         value;
    API_RESULT        retval;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_rsc_sc_cp_db_handle.device_id = (*dq_handle);
    appl_batt_db_handle.device_id      = (*dq_handle);
    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_rsc_sc_cp_db_handle,
                 &appl_rsc_sc_cp_hndl
             );

    retval = BT_gatt_db_get_char_cli_cnfg
             (
                 &appl_rsc_sc_cp_db_handle,
                 &value
             );
    BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    APPL_TRC(
    "[RSCS]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle), retval);

    appl_rsc_msrmt_db_handle.device_id = (*dq_handle);

    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_rsc_msrmt_db_handle,
                 &appl_rsc_hndl
             );
    retval = BT_gatt_db_get_char_cli_cnfg
             (
                 &appl_rsc_msrmt_db_handle,
                 &value
             );
    BT_UNPACK_LE_2_BYTE (&cli_cnfg,value.val);

    APPL_TRC (
    "[RSCS]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle),
    retval);

    appl_manage_trasnfer(appl_rsc_msrmt_db_handle,cli_cnfg);
}

void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    APPL_HANDLE    appl_handle;
    API_RESULT     retval;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        /* Security requirements satisfied, go ahead with data transfer */
        retval = appl_get_handle_from_device_handle(handle.device_id, &appl_handle);

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
                retval = BT_start_timer
                         (
                             &timer_handle,
                             APPL_RSC_MSRMNT_INTERVAL,
                             appl_timer_expiry_handler,
                             &appl_handle,
                             sizeof(appl_handle)
                         );
                APPL_TRC(
                    "[RSCS]: [%02X] Control Point Timer %p, result 0x%04X Started!\n",
                    appl_handle, timer_handle, retval);

                if (API_SUCCESS != retval)
                {
                    /**
                     *  Transfer could not be started, take appropriate action. One
                     *  option could be to indicate an application error for
                     *  configuration request
                     */
                }
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

void appl_send_rsc_measurement (APPL_HANDLE   * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    APPL_TRC(
    "[RSCS]: [0x%02X]:Sending RSC measurement On Handle 0x%04X\n",
    (*handle), appl_rsc_msrmt_handle);

    hndl_val_param.handle =  appl_rsc_hndl;
    hndl_val_param.value.val = rsc_obs_data;
    hndl_val_param.value.len = APPL_RSC_MSRMNT_LENGTH;

    retval = BT_att_send_hndl_val_ntf
             (
                 &APPL_GET_ATT_INSTANCE(*handle),
                 &hndl_val_param
              );
    if (2U == rsc_count)
    {
        if (0x07U == rsc_obs_data[0U])
        {
            rsc_obs_data[0U] = 0x03U;
        }
        else
        {
            rsc_obs_data[0U] = 0x07U;
        }
        if (0xFFU == rsc_obs_data[0x06U])
        {
            rsc_obs_data[0x07U]++;
        }
        rsc_obs_data[0x06U]++;
        rsc_count = 0U;
    }
    rsc_count++;
    if (API_SUCCESS != retval)
    {
        APPL_ERR (
        "[RSCS]: **ERR** Failed to send measurement, reason 0x%04X", retval);
    }
}


void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    APPL_EVENT_PARAM    fsm_param;

    fsm_param.handle = (*((APPL_HANDLE *)data));


    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC("[RSCS]: RSC Measurement %p Timeout Occurred\n", timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &timer_handle,
        APPL_RSC_MSRMNT_INTERVAL,
        appl_timer_expiry_handler,
        &fsm_param.handle,
        sizeof (fsm_param.handle)
    );

    APPL_TRC(
    "[RSCS]: [0x%02X] RSC Measurement Timer %p Started!\n",
    fsm_param.handle, timer_handle);

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

void appl_rsc_stop_timer (APPL_HANDLE   * appl_handle)
{
    BT_IGNORE_UNUSED_PARAM(appl_handle);
    /* Turn Off Transfer Timer if On.  */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        BT_stop_timer (cntrl_point_handle);
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_rsc_cntrl_point_cnfg_handler (GATT_DB_HANDLE  *handle, UINT16 config)
{
    BT_IGNORE_UNUSED_PARAM(handle);
    if (GATT_CLI_CNFG_INDICATION == config)
    {
        appl_cntrl_point_cnfgd = BT_TRUE;
    }
    else if (GATT_CLI_CNFG_DEFAULT == config)
    {
        appl_cntrl_point_cnfgd = BT_FALSE;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}

void appl_rscs_server_reinitialize (void)
{
    /* Turn Off Transfer Timer if On.  */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        BT_stop_timer (cntrl_point_handle);
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
    rsc_count = 0U;
    appl_cntrl_point_cnfgd = BT_FALSE;
    appl_snsr_calib_error = BT_FALSE;
    appl_hvc_flag = BT_FALSE;

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U,
                                   0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U,
                                    0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_rscs_cntrl_point_timer_handle (void * data, UINT16 datalen)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    UCHAR                 rsp[7U];
    API_RESULT            retval;

    BT_IGNORE_UNUSED_PARAM(datalen);

    rsp[0U] = APPL_RSC_RESPONSE_CODE_OPCODE;
    rsp[1U] = ((APPL_RCSC_CNTRL_POINT_RSP_PARAM *)data)->op_code;
    rsp[2U] = ((APPL_RCSC_CNTRL_POINT_RSP_PARAM *)data)->rsp_code;

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (APPL_RSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE == rsp[1U])
    {
        hndl_val_param.value.len = APPL_RSC_RESPONSE_LENGTH +
            APPL_RSC_SUPPORTED_SENSOR_LOC_COUNT;
        BT_mem_copy
        (
            &rsp[3U],
            &rsc_supported_sensor_location,
            APPL_RSC_SUPPORTED_SENSOR_LOC_COUNT
        );
    }
    else
    {
        hndl_val_param.value.len = APPL_RSC_RESPONSE_LENGTH;
    }

    hndl_val_param.handle =  appl_rsc_sc_cp_hndl;
    hndl_val_param.value.val = rsp;

    /* Send Indication */
    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(((APPL_RCSC_CNTRL_POINT_RSP_PARAM *)data)->appl_handle),
                 &hndl_val_param
              );

    if (API_FAILURE != retval)
    {
        appl_hvc_flag = BT_TRUE;
    }
}

API_RESULT appl_rsc_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           )
{
    API_RESULT retval;
    APPL_RCSC_CNTRL_POINT_RSP_PARAM   rsp_param;

    appl_get_handle_from_device_handle
    (
        handle->device_id,
        &rsp_param.appl_handle
    );
    retval = API_SUCCESS;

    rsp_param.op_code = value->val[0U];

    if (BT_FALSE == appl_cntrl_point_cnfgd)
    {
        retval = APPL_RSC_CCD_IMPROPERLY_CONFIGURED;
    }
    else if (BT_TRUE != appl_hvc_flag)
    {
        if ((value->len >= 1U) && (value->val[0U] < 0x05U))
        {
            switch (value->val[0U])
            {
            case APPL_RSC_SET_CUMMULATIVE_VALUE_OPCODE:
                if (value->len >= (APPL_RSC_CUMMULATIVE_VALUE_LENGTH + 1U))
                {
                    BT_mem_copy
                    (
                        &rsc_obs_data[6U],
                        &(value->val[1U]),
                        APPL_RSC_CUMMULATIVE_VALUE_LENGTH
                    );
                    rsc_count = 0U;
                    rsp_param.rsp_code = APPL_RSC_SUCCESS;
                }
                else
                {
                    rsp_param.rsp_code = APPL_RSC_INVALID_PARAMETER;
                }
                retval = API_SUCCESS;
                break;
            case APPL_RSC_START_SENSOR_CALIBRATION_OPCODE:
                if ((1U == value->len) && (BT_FALSE == appl_snsr_calib_error))
                {
                    rsp_param.rsp_code = APPL_RSC_SUCCESS;
                }
                else if (1U == appl_snsr_calib_error)
                {
                    rsp_param.rsp_code = APPL_RSC_OPERATION_FAILED;
                    appl_snsr_calib_error = BT_FALSE;
                }
                else
                {
                    rsp_param.rsp_code = APPL_RSC_INVALID_PARAMETER;
                }
                break;
            case APPL_RSC_UPDATE_SENSOR_LOCATION_OPCODE:
                {
                    if (2U == value->len)
                    {
                        GATT_DB_HANDLE   db_handle;
                        ATT_VALUE        att_value;
                        UCHAR            count;
                        count = 0U;
                        rsp_param.rsp_code = APPL_RSC_INVALID_PARAMETER;

                        while (count < APPL_RSC_SUPPORTED_SENSOR_LOC_COUNT)
                        {
                            if (value->val[1U] == rsc_supported_sensor_location[count])
                            {
                                att_value.len = 1U;
                                att_value.val = value->val+1U;

                                db_handle = (*handle);
                                db_handle.char_id = (UCHAR)GATT_CHAR_RSCS_SNSR_LOC_INST;
                                BT_gatt_db_set_char_val (&db_handle,&att_value);
                                rsp_param.rsp_code = APPL_RSC_SUCCESS;
                                break;
                            }
                            count ++;
                        }
                    }
                    else
                    {
                        rsp_param.rsp_code = APPL_RSC_INVALID_PARAMETER;
                    }
                }
                break;
            case APPL_RSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE:
                if (1U == value->len)
                {
                    rsp_param.rsp_code = APPL_RSC_SUCCESS;
                }
                else
                {
                    rsp_param.rsp_code = APPL_RSC_INVALID_PARAMETER;
                }
                break;
            default:
                retval = API_SUCCESS;
                rsp_param.rsp_code = APPL_RSC_OPCODE_NOT_SUPPORTED;
                break;
            }
        }
        else
        {
            rsp_param.rsp_code = APPL_RSC_OPCODE_NOT_SUPPORTED;
        }

        if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
        {
            BT_stop_timer (cntrl_point_handle);
            cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
        }

        retval = BT_start_timer
                 (
                     &cntrl_point_handle,
                     1U,
                     appl_rscs_cntrl_point_timer_handle,
                     &rsp_param,
                     sizeof (rsp_param)
                 );

        APPL_TRC(
        "[RSCS]: Control Point Timer %p with result 0x%04X Started!\n",
        cntrl_point_handle, retval);
    }
    else
    {
        retval = APPL_RSC_PROCEDURE_ALREADY_IN_PROGRESS;
    }
    return retval;
}

void appl_rscs_set_calib_error (UCHAR flag)
{
    /* Set the Calibration Error Value */
    appl_snsr_calib_error = (0U == flag) ? BT_FALSE : BT_TRUE;
}

void appl_rscs_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    CONSOLE_OUT(
    "\n [RSCS] IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);

    if (ATT_RESPONSE_TIMED_OUT == evt_result)
    {
        /**
         * If a Handle Value Confirmation has not been sent for an Indication
         * by the remote device, Suspend all Transactions.
         */
        appl_rsc_stop_timer (handle);
    }
}

void appl_rscs_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    CONSOLE_OUT("\n [RSCS] NTF Sent for Appl Handle 0x%02X", *handle);
    appl_dump_bytes(event_data, datalen);
}

void appl_rscs_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    CONSOLE_OUT("\n [RSCS] Updated MTU is %d for Appl Handle 0x%02X", mtu, *handle);
}

#endif /* (defined ATT && defined RSCS) */

