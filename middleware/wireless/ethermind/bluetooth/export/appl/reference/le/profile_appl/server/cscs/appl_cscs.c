
/**
 *  \file appl_cscs.c
 *
 *  This file contains the Cycling Speed & Cadence Sensor application.
 *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery
 *           - Device Information and
 *           - Cycling Speed & Cadence
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Cycling Speed and Cadence Service exposes:
 *           - Cycling Speed & Cadence Measurement
 *           - Sensor Location
 *           - Cycling Speed & Cadence Control Point
 *      d. Cycling Speed & Cadence Measurement can be configured to send
 *         Notifications.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Measurement Transfer by writing to the
 *             Characteristic Client Configuration of CSC Measurement.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_cscs_handler and
 *           - appl_cscs_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements
 *         periodically. This interval can be altered using the define
 *         APPL_CSC_MSRMNT_INTERVAL. In absence of a real sensor
 *         to provide speed & cadence readings, a static reading is being
 *         sent. Distance is incremented every 5 seconds. This value can
 *         be reset by the Collector device through the Cycling Speed
 *         & Cadence Control Point. See appl_csc_control_point_handler for more
 *         details.
 */

 /*
  *  Copyright (C) 2013. Mindtree Ltd.
  *  All rights reserved.
  */

  /* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined CSCS)

/* --------------------------------------------- Constants */
#define APPL_CSC_MSRMNT_LENGTH                              11U
#define APPL_CSC_MSRMNT_INTERVAL                            5U

/* Speed and Cadence Control Point Commands */
#define APPL_CSC_SET_CUMMULATIVE_VALUE_OPCODE               1U
#define APPL_CSC_START_SENSOR_CALIBRATION_OPCODE            2U
#define APPL_CSC_UPDATE_SENSOR_LOCATION_OPCODE              3U
#define APPL_CSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE   4U
#define APPL_CSC_RESPONSE_CODE_OPCODE                       16U

/* Speed and Cadence Control Point Response Codes */
#define APPL_CSC_SUCCESS                                    1U
#define APPL_CSC_OPCODE_NOT_SUPPORTED                       2U
#define APPL_CSC_INVALID_PARAMETER                          3U
#define APPL_CSC_OPERATION_FAILED                           4U

#define APPL_CSC_CUMMULATIVE_VALUE_LENGTH                   4U
#define APPL_CSC_RESPONSE_LENGTH                            3U

/* Supported Sensor Location Count */
#define APPL_CSC_SUPPORTED_SENSOR_LOC_COUNT                 3U

/* Application Error Codes */
#define APPL_CSC_PROCEDURE_ALREADY_IN_PROGRESS          (APPL_ERR_ID | 0x80U)
#define APPL_CSC_CCD_IMPROPERLY_CONFIGURED              (APPL_ERR_ID | 0x81U)

/* --------------------------------------------- External Global Variables */
extern UCHAR appl_hvc_flag;
/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/*static UCHAR csc_obs_data[APPL_CSC_MSRMNT_LENGTH] = {0x0E, 0x56, 0x00, 0x00};*/
static UCHAR csc_obs_data[APPL_CSC_MSRMNT_LENGTH] =
{
    0x03U, 0x01U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U, 0x01U, 0x00U, 0x01U, 0x00U
};
static UCHAR csc_supported_sensor_location[APPL_CSC_SUPPORTED_SENSOR_LOC_COUNT] =
{
    0x04U, 0x05U, 0x06U
};

/* Timer related Globals */
static BT_timer_handle timer_handle;
static BT_timer_handle cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_batt_db_handle;
static GATT_DB_HANDLE  appl_cscs_hndl;
static GATT_DB_HANDLE  appl_csc_sc_cp_db_handle;

/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_csc_hndl;
static ATT_ATTR_HANDLE appl_csc_sc_cp_hndl;

/* Service data and states related Globals */
static UINT16          appl_msrmt_intrvl;
static UCHAR           appl_cntrl_point_cnfgd = BT_FALSE;
static UCHAR           csc_count;
static UCHAR           csc_reverse;
/* --------------------------------------------- Functions */

void appl_cscs_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_cscs_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

    /* Initialize the Measurement Interval */
    appl_msrmt_intrvl = APPL_CSC_MSRMNT_INTERVAL;

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_cscs_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_cscs_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_cscs_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_cscs_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_cscs_add_cscs();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the CSCS GATT DB Signature */
    appl_cscs_db_sign = APPL_CSCS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_cscs_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[CSCS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for CSC Measurement and CSC Control Point */

    appl_cscs_hndl.device_id            = DEVICE_HANDLE_INIT_VAL;
    appl_cscs_hndl.service_id           = (UCHAR)GATT_SER_CSCS_CSC_INST;
    appl_cscs_hndl.char_id              = (UCHAR)GATT_CHAR_CSCS_CSC_MSRMNT_INST;

    appl_csc_sc_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_csc_sc_cp_db_handle.service_id = (UCHAR)GATT_SER_CSCS_CSC_INST;
    appl_csc_sc_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_CSCS_SC_CNTRL_PNT_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for CSCS services if Dynamic GATT Database is not Used */
    appl_cscs_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register CSCS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_cscs_gatt_char_handler);

#ifdef BT_DUAL_MODE
    /* Update CSCS Service Range in SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_CSCS_CSC_INST,
        DB_RECORD_CSC
    );
#endif
}

void appl_cscs_bt_init(void)
{
    APPL_TRC("[CSCS]: In appl_cscs_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_cscs_server_reinitialize();
}

void appl_cscs_bt_deinit(void)
{
    APPL_TRC("[CSCS]: In appl_cscs_bt_deinit\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}


void appl_cscs_connect(DEVICE_HANDLE* dq_handle)
{
    ATT_VALUE         value;
    API_RESULT        retval;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_csc_sc_cp_db_handle.device_id = (*dq_handle);
    appl_batt_db_handle.device_id      = (*dq_handle);

    /* If SC Control Point is configured? */
    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_csc_sc_cp_db_handle,
                 &appl_csc_sc_cp_hndl
             );

    /* If CSC Measurement is configured, initiate transfer */
    appl_cscs_hndl.service_id = (UCHAR)GATT_SER_CSCS_CSC_INST;
    appl_cscs_hndl.device_id  = (*dq_handle);
    appl_cscs_hndl.char_id    = (UCHAR)GATT_CHAR_CSCS_CSC_MSRMNT_INST;
    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_cscs_hndl,
                 &appl_csc_hndl
             );

    retval = BT_gatt_db_get_char_cli_cnfg(&appl_cscs_hndl, &value);

    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC(
    "[CSCS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_cscs_hndl, cli_cnfg);
    }
}

void appl_manage_trasnfer(GATT_DB_HANDLE handle, UINT16 config)
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
                             APPL_CSC_MSRMNT_INTERVAL,
                             appl_timer_expiry_handler,
                             &appl_handle,
                             sizeof(appl_handle)
                         );

                APPL_TRC(
                "[CSCS]: CSC Measurement Timer %p Started, result 0x%04X!\n",
                timer_handle, retval);

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

void appl_send_csc_measurement(APPL_HANDLE* handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    APPL_TRC (
    "[CSCS]: [0x%02X]:Sending CSC measurement On Handle 0x%04X\n",
    (*handle), appl_csc_hndl);

    hndl_val_param.handle = appl_csc_hndl;
    hndl_val_param.value.val = csc_obs_data;
    hndl_val_param.value.len = APPL_CSC_MSRMNT_LENGTH;

    retval = BT_att_send_hndl_val_ntf
             (
                 &APPL_GET_ATT_INSTANCE(*handle),
                 &hndl_val_param
             );

    if (2U == csc_count)
    {
        if (BT_TRUE != csc_reverse)
        {
            /* Simulate Cumulative Wheel Revolutions */
            if (0xFFU == csc_obs_data[0x01U])
            {
                csc_obs_data[0x02U]++;
            }
            csc_obs_data[0x01U]++;

            /* Simulate Last Wheel Event Time */
            if (0x40U == csc_obs_data[0x05U])
            {
                csc_obs_data[0x05U] = 0U;
            }
            csc_obs_data[0x05U]++;
        }
        else
        {
            csc_obs_data[0x01U]--;
            if (0x00U == csc_obs_data[0x01U])
            {
                csc_reverse = BT_FALSE;
            }
        }
        csc_count = 0U;
    }

    csc_count++;

    if (API_SUCCESS != retval)
    {
        APPL_ERR(
        "[CSCS]: **ERR**: Failed to send measurement, reason 0x%04X",
        retval);
    }
}


void appl_timer_expiry_handler(void* data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    APPL_EVENT_PARAM    fsm_param;
    API_RESULT         retval = API_FAILURE;

    fsm_param.handle = (*((APPL_HANDLE*)data));

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC(
        "[CSCS]: [0x%02X]: CSC Measurement Timeout %p Expired!\n",
        fsm_param.handle, timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
    (
        &timer_handle,
        APPL_CSC_MSRMNT_INTERVAL,
        appl_timer_expiry_handler,
        &fsm_param.handle,
        sizeof(fsm_param.handle)
    );

    APPL_TRC(
    "[CSCS]: CSC Measurement Timer %p Started, retval 0x%04X\n",
    timer_handle, retval);

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

void appl_csc_cntrl_point_cnfg_handler(GATT_DB_HANDLE* handle, UINT16 config)
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

void appl_cscs_server_reinitialize(void)
{
    /* Turn Off Transfer Timer if On.  */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        BT_stop_timer(cntrl_point_handle);
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    csc_count              = 0U;
    appl_cntrl_point_cnfgd = BT_FALSE;
    appl_hvc_flag          = BT_FALSE;

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

void appl_cscs_cntrl_point_timer_handle(void* data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    /* Send Indication */
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    UCHAR                 rsp[6U];
    API_RESULT            retval;

    rsp[0U] = APPL_CSC_RESPONSE_CODE_OPCODE;
    rsp[1U] = ((APPL_CSCS_CNTRL_POINT_RSP_PARAM*)data)->op_code;
    rsp[2U] = ((APPL_CSCS_CNTRL_POINT_RSP_PARAM*)data)->rsp_code;

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
    {
        cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (APPL_CSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE == rsp[1U])
    {
        hndl_val_param.value.len = APPL_CSC_RESPONSE_LENGTH +
            APPL_CSC_SUPPORTED_SENSOR_LOC_COUNT;

        BT_mem_copy
        (
            &rsp[3U],
            &csc_supported_sensor_location,
            APPL_CSC_SUPPORTED_SENSOR_LOC_COUNT
        );
    }
    else
    {
        hndl_val_param.value.len = APPL_CSC_RESPONSE_LENGTH;
    }

    hndl_val_param.handle = appl_csc_sc_cp_hndl;
    hndl_val_param.value.val = rsp;

    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(((APPL_CSCS_CNTRL_POINT_RSP_PARAM*)data)->appl_handle),
                 &hndl_val_param
             );

    if (API_FAILURE != retval)
    {
        appl_hvc_flag = BT_TRUE;
    }
}

API_RESULT appl_csc_control_point_handler
(
    GATT_DB_HANDLE* handle,
    ATT_VALUE* value
)
{
    API_RESULT retval;
    APPL_CSCS_CNTRL_POINT_RSP_PARAM   rsp_param;

    appl_get_handle_from_device_handle
    (
        handle->device_id,
        &rsp_param.appl_handle
    );

    retval = API_SUCCESS;

    rsp_param.op_code = value->val[0U];

    if (BT_FALSE == appl_cntrl_point_cnfgd)
    {
        retval = APPL_CSC_CCD_IMPROPERLY_CONFIGURED;
    }
    else if (BT_TRUE != appl_hvc_flag)
    {
        if ((value->len >= 1U) && (value->val[0U] < 0x05U))
        {
            switch (value->val[0U])
            {
            case APPL_CSC_SET_CUMMULATIVE_VALUE_OPCODE:
                if (value->len >= (APPL_CSC_CUMMULATIVE_VALUE_LENGTH + 1U))
                {
                    BT_mem_copy
                    (
                        &csc_obs_data[1U],
                        &(value->val[1U]),
                        APPL_CSC_CUMMULATIVE_VALUE_LENGTH
                    );
                    csc_count = 0U;
                    if (0x05U == value->val[1U])
                    {
                        csc_reverse = BT_TRUE;
                    }
                    rsp_param.rsp_code = APPL_CSC_SUCCESS;
                }
                else
                {
                    rsp_param.rsp_code = APPL_CSC_INVALID_PARAMETER;
                }
                retval = API_SUCCESS;
                break;
            case APPL_CSC_UPDATE_SENSOR_LOCATION_OPCODE:
            {
                if (2U == value->len)
                {
                    GATT_DB_HANDLE   db_handle;
                    ATT_VALUE        att_value;
                    UCHAR            count;
                    count = 0U;
                    rsp_param.rsp_code = APPL_CSC_INVALID_PARAMETER;

                    while (count < APPL_CSC_SUPPORTED_SENSOR_LOC_COUNT)
                    {
                        if (value->val[1U] == csc_supported_sensor_location[count])
                        {
                            att_value.len = 1U;
                            att_value.val = value->val + 1U;

                            db_handle = (*handle);
                            db_handle.char_id = (UCHAR)GATT_CHAR_CSCS_SNSR_LOC_INST;
                            BT_gatt_db_set_char_val(&db_handle, &att_value);
                            rsp_param.rsp_code = APPL_CSC_SUCCESS;
                            break;
                        }
                        count++;
                    }
                }
                else
                {
                    rsp_param.rsp_code = APPL_CSC_INVALID_PARAMETER;
                }
            }
            break;
            case APPL_CSC_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE:
                if (1U == value->len)
                {
                    rsp_param.rsp_code = APPL_CSC_SUCCESS;
                }
                else
                {
                    rsp_param.rsp_code = APPL_CSC_INVALID_PARAMETER;
                }
                break;
            default:
                retval = API_SUCCESS;
                rsp_param.rsp_code = APPL_CSC_OPCODE_NOT_SUPPORTED;
                break;
            }
        }
        else
        {
            rsp_param.rsp_code = APPL_CSC_OPCODE_NOT_SUPPORTED;
        }

        BT_start_timer
        (
            &cntrl_point_handle,
            1U,
            appl_cscs_cntrl_point_timer_handle,
            &rsp_param,
            sizeof(rsp_param)
        );
    }
    else
    {
        retval = APPL_CSC_PROCEDURE_ALREADY_IN_PROGRESS;
    }

    return retval;
}

void appl_cscs_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    CONSOLE_OUT(
    "\n [CSCS] IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);

    if (ATT_RESPONSE_TIMED_OUT == evt_result)
    {
        /**
         * If a Handle Value Confirmation has not been sent for an Indication
         * by the remote device, Suspend all Transactions.
         */
        /* Suspend all Transfer related timers */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            BT_stop_timer(timer_handle);
            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }

        if (BT_TIMER_HANDLE_INIT_VAL != cntrl_point_handle)
        {
            BT_stop_timer(cntrl_point_handle);
            cntrl_point_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
}

void appl_cscs_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    CONSOLE_OUT("\n [CSCS] NTF Sent for Appl Handle 0x%02X", *handle);
    appl_dump_bytes(event_data, datalen);
}

void appl_cscs_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    CONSOLE_OUT("\n [CSCS] Updated MTU is %d for Appl Handle 0x%02X", mtu, *handle);
}

#endif /* (defined ATT && defined CSCS) */

