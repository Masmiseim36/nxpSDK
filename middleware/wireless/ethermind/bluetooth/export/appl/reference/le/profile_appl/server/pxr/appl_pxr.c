
/**
 *  \file appl_pxr.c
 *
 *  This file contains the Proximity Reporter application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined PXR)
/* --------------------------------------------- Global Definitions */
/**
 * This Flag controls dumps and prints of PXR related Handle Value
 * Notification Complete Status.
 * This Flag is disabled by default.
 */
/* #define APPL_PXR_HAVE_DATA_DUMP */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */


/* --------------------------------------------- Static Global Variables */
static UCHAR           appl_px_configured;
static UCHAR           appl_px_read_tx_power_pending;
static UCHAR           appl_px_tx_power_level;

static BT_timer_handle timer_handle;

static GATT_DB_HANDLE  appl_lls_db_handle;
static GATT_DB_HANDLE  appl_tps_db_handle;

static ATT_ATTR_HANDLE appl_tps_tx_pw_lvl_hndl;

/* --------------------------------------------- Functions */

void appl_pxr_reinit
     (
          DEVICE_HANDLE * handle,
          UCHAR         disconnect_reason
     )
{
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
        appl_service_configure_scan (APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */

    /* Stop transfer simulate TX Power measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);

        APPL_TRC (
        "[PXR]: TX Power Level Measurement Timer %p Stopped\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_FALSE != appl_px_configured)
    {
        /**
         * Alert for Link Loss only when the Disconnection Reason Received is:
         * 1. Disconnection due to Connection timeout
         * 2. Disconnection due to Remote Side PowerOff
         *
         * LLS_v1.0.1.pdf, Section 4.1 Disconnection Behaviour:
         * "When this service is instantiated in a device and the connection is
         *  lost without any prior warning, the device shall start alerting to
         *  the current link loss alert level. However, if the connection is
         *  terminated using a link layer procedure, the device shall not alert
         *  and shall ignore the current link loss alert level."
         */
        if (((UCHAR)HC_CONNECTION_TIMEOUT             == disconnect_reason) ||
            ((UCHAR)HC_OTHER_END_TERMINATED_POWER_OFF == disconnect_reason))
        {
            appl_pxr_alert_user(handle, APPL_PXR_LINK_LOSS);
        }
    }
}

void appl_pxr_init (void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_pxr_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_ias();
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_lls();
    (BT_IGNORE_RETURN_VALUE) appl_pxr_add_tps();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the PXR GATT DB Signature */
    appl_pxr_db_sign = APPL_PXR_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_pxr_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[PXR]: GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for Link Loss */
    appl_lls_db_handle.device_id   = DEVICE_HANDLE_INIT_VAL;
    appl_lls_db_handle.service_id  = (UCHAR)GATT_SER_PXR_LINK_LOSS_INST;
    appl_lls_db_handle.char_id     = (UCHAR)GATT_CHAR_PXR_LL_ALRT_LVL_INST;

    /* Populate the GATT DB HANDLE for Tx Power Level */
    appl_tps_db_handle.device_id   = DEVICE_HANDLE_INIT_VAL;
    appl_tps_db_handle.service_id  = (UCHAR)GATT_SER_PXR_TX_POWER_LVL_INST;
    appl_tps_db_handle.char_id     = (UCHAR)GATT_CHAR_PXR_TX_POWER_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for PXR services if Dynamic GATT Database is not Used */
    appl_pxr_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register PXR GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_pxr_gatt_char_handler);
}

void appl_pxr_bt_init(void)
{
    APPL_TRC("[PXR]: In appl_pxr_bt_init\n");

    /* Initialize */
    appl_px_configured            = BT_FALSE;
    appl_px_read_tx_power_pending = BT_FALSE;

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    /* Invoke Reinitialize */
    /**
     * Since this is invoked from BT Init i.e. BT ON Complete,
     * and as appl_px_configured is initialized to BT_FALSE prior to this
     * invoking the pxr reinit function just to start the action specified
     * by the GAP role. No Alert is possible at this time. So the parameters of
     *  - Device Handle reference is NULL (dont-care)
     *  - Disconnect reason is 0x00 (dont-care)
     */
    appl_pxr_reinit(NULL, 0x00);
}

void appl_pxr_bt_deinit(void)
{
    APPL_TRC("[PXR]: In appl_pxr_bt_deinit invoked\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

API_RESULT appl_pxr_set_alert_level
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           )
{
    API_RESULT   retval;

    retval = APPL_PXR_INVALID_ALRT_VALUE;

    if (GATT_CHAR_PXR_LL_ALRT_LVL_INST == handle->char_id)
    {
        /**
         *  Permissible Alert Levels are:
         *     0 - No Alert
         *     1 - Mild Alert
         *     2 - High Alert
         */
         if (BT_FALSE != APPL_PXR_VALID_ALERT_LEVEL(value->val[0U]))
         {
             appl_px_configured = BT_TRUE;
             retval = API_SUCCESS;
         }
    }
    else if (GATT_CHAR_PXR_IM_ALRT_LVL_INST == handle->char_id)
    {
        APPL_TRC(
        "\n=====================================================================\n");
        APPL_TRC (
        "                          User Alert\n");
        APPL_TRC (
        "                          ----------\n");
        APPL_TRC (
        "[Proximity]: %s Alert => IMMEDIATE ALERT",appl_pxr_get_alert_str(value->val[0U]));
        APPL_TRC(
        "\n=====================================================================\n\n\n");
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

void appl_pxr_alert_user
     (
          DEVICE_HANDLE * handle,
          UCHAR         alert_reason
     )
{
#ifndef DONT_USE_STANDARD_IO
    API_RESULT        retval;
#endif /* DONT_USE_STANDARD_IO */
    UCHAR             ll_alert_level;
    ATT_VALUE         value;

    appl_lls_db_handle.device_id = (*handle);

#ifndef DONT_USE_STANDARD_IO
    retval =
#endif /* DONT_USE_STANDARD_IO */
    BT_gatt_db_get_char_val(&appl_lls_db_handle, &value);

    /**
     * Extract the Link Loss Alert Level Characteristic
     * Value into local variable.
     */
    BT_UNPACK_LE_1_BYTE(&ll_alert_level, value.val);

#ifndef DONT_USE_STANDARD_IO
    APPL_TRC (
    "[PXR]: Fetched Alert (0x%02X) for Device 0x%02X for Link Loss Service "
    "with result 0x%04X\n",ll_alert_level,(*handle),retval);
#endif /* DONT_USE_STANDARD_IO */

    if (APPL_PXR_LINK_LOSS == alert_reason)
    {
        APPL_TRC(
        "\n=====================================================================\n");
        APPL_TRC (
        "                          User Alert\n");
        APPL_TRC (
        "                          ----------\n");
        APPL_TRC (
        "[Proximity]: %s Alert => LINK LOST",appl_pxr_get_alert_str(ll_alert_level));

        APPL_TRC(
        "\n=====================================================================\n\n\n");
    }
}

char * appl_pxr_get_alert_str (UCHAR  alert_level)
{
    char * ret_val;
    static const char * alert_str_1 = "NO";
    static const char * alert_str_2 = "MILD";
    static const char * alert_str_3 = "HIGH";

    /* Init */
    ret_val = NULL;

    switch (alert_level)
    {
    case APPL_PXR_NO_ALERT:
        ret_val = (char *)alert_str_1; /* return "NO"; */
        break;
    case APPL_PXR_MILD_ALERT:
        ret_val = (char *)alert_str_2; /* return "MILD"; */
        break;
    case APPL_PXR_HIGH_ALERT:
        ret_val = (char *)alert_str_3; /* return "HIGH"; */
        break;
    default:
        CONSOLE_OUT("Invalid Level: 0x%02X\n", alert_level);
        break;
    }

    return ret_val;
}

API_RESULT appl_pxr_get_tx_power_level
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           )
{
    API_RESULT     retval, api_return;
    APPL_HANDLE    appl_handle;

    BT_IGNORE_UNUSED_PARAM(value);

    appl_get_handle_from_device_handle (handle->device_id, &appl_handle);

    /* Invoke the TX Power Level Read from HCI */
    api_return = appl_pxr_fetch_tx_power_level(appl_handle);

    if (API_SUCCESS == api_return)
    {
        /**
         * Set the TX Power Read Flag only when invoking this HCI
         * command for a "Read" Operation.
         * This HCI Command for TX Power Level Read is also invoked
         * when Notifications are configured.
         */
        appl_px_read_tx_power_pending = BT_TRUE;

        /**
         * Concerned data gathering is still under process,
         * mark the packet as to be responded later so as to
         * avoid sending response from the below GATT
         * server application layer.
         */
        retval = GATT_DB_DELAYED_RESPONSE;
    }
    else
    {
        APPL_TRC(
        "[PXR]: Failed to Read Current Tx Power Level");

        retval = (ATT_UNLIKELY_ERROR | ATT_ERR_ID);
    }

    return retval;
}

API_RESULT appl_pxr_get_tx_power_level_complete
           (
                UCHAR     status,
                UINT16    conn_handle,
                UCHAR     tx_power_level
           )
{
    ATT_HANDLE         * att_handle;
    APPL_HANDLE          app_handle;
    ATT_READ_RSP_PARAM   rsp_param;
    ATT_ERROR_RSP_PARAM  err_param;
    API_RESULT           retval;

    retval = API_SUCCESS;

    /* Update the TX Power Level into the Global Variable */
    if  (0x00U == status)
    {
        /* Save Current Tx Power Level */
        appl_px_tx_power_level = tx_power_level;
    }

    if (BT_FALSE != appl_px_read_tx_power_pending)
    {
        att_handle = NULL;

        /* Get Application Handle from connection Handle */
        retval = appl_get_handle_from_cxn_handle(conn_handle, &app_handle);

        if (API_SUCCESS == retval)
        {
            /* Get ATT Instance from Application Handle */
            att_handle = &APPL_GET_ATT_INSTANCE(app_handle);

            if  (0x00U == status)
            {
                /* Access the application handle and send back read response */
                rsp_param.val = &appl_px_tx_power_level;
                rsp_param.len = APPL_PX_TX_POWER_LEVEL_LENGTH;

                /* Get ATT Handle */
                if (NULL != att_handle)
                {
                    retval = BT_att_read_rsp
                             (
                                 att_handle,
                                 &rsp_param
                             );
                }
            }
            else
            {
                /* Failed to read Tx Power Level. Send Error */
                /* Not remembering if it was a read blob request */
                err_param.handle     = appl_tps_tx_pw_lvl_hndl;
                err_param.op_code    = ATT_READ_REQ;
                err_param.error_code = ATT_UNLIKELY_ERROR;

                retval = BT_att_send_error_rsp
                         (
                             att_handle,
                             &err_param
                         );

                APPL_TRC (
                "[PXR]: Sent Error Response with result 0x%04X",retval);
            }
        }

        /* Reset Read Tx Power Pending flag */
        appl_px_read_tx_power_pending = BT_FALSE;
    }

    return retval;
}

void appl_pxr_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;
    API_RESULT        retval;

    cli_cnfg = 0U;

    appl_lls_db_handle.device_id  = (*dq_handle);
    appl_tps_db_handle.device_id  = (*dq_handle);

    /* Fetch TPS Power Level Attribute Handle */
    BT_gatt_db_get_char_val_hndl
    (
        &appl_tps_db_handle,
        &appl_tps_tx_pw_lvl_hndl
    );

    /* If TPS Power Level is Configured, then send measurements */
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_tps_db_handle,&value);
    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC (
    "[PXR]: Fetched TX Power Level CCCD (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_tps_db_handle, cli_cnfg);
    }
}

void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    API_RESULT     retval;
    APPL_HANDLE    appl_handle;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        appl_get_handle_from_device_handle(handle.device_id, &appl_handle);

        /* Invoke the TX Power Level Read from HCI */
        retval = appl_pxr_fetch_tx_power_level(appl_handle);

        /* Security requirements satisfied, go ahead with data transfer */
        if (GATT_CHAR_PXR_TX_POWER_INST == handle.char_id)
        {
            if (GATT_CLI_CNFG_NOTIFICATION == config)
            {
                /* Start timer to simulate Temperature measurements */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    BT_stop_timer(timer_handle);

                    APPL_TRC(
                        "[PXR]: TX Power Level Measurement Timer %p Stopped\n",
                        timer_handle);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                BT_start_timer
                (
                    &timer_handle,
                    APPL_PXR_TPS_MSRMT_TIMEOUT,
                    appl_timer_expiry_handler,
                    &appl_handle,
                    sizeof(appl_handle)
                );

                APPL_TRC(
                    "[PXR]: TX Power Level Measurement Timer %p Started\n",
                    timer_handle);
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                /* Stop transfer simulate Temperature measurements */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    BT_stop_timer(timer_handle);

                    APPL_TRC(
                        "[PXR]: TX Power Level Measurement Timer %p Stopped\n",
                        timer_handle);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else
        {
            APPL_TRC("[*** ERR ***]:Not the right transfer type!\n");
        }
    }
}

void appl_timer_expiry_handler (void *data_param, UINT16 datalen)
{
    APPL_EVENT_PARAM   fsm_param;
    API_RESULT         retval = API_FAILURE;

    BT_IGNORE_UNUSED_PARAM(datalen);

    fsm_param.handle = (*((APPL_HANDLE *)data_param));

    /* Restart timer to simulate Temperature measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC (
        "[PXR]: TX Power Level Measurement Timer %p Expired!\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    /* Invoke the TX Power Level Read from HCI */
    retval = appl_pxr_fetch_tx_power_level(fsm_param.handle);

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_PXR_TPS_MSRMT_TIMEOUT,
                 appl_timer_expiry_handler,
                 &fsm_param.handle,
                 sizeof(fsm_param.handle)
             );

    APPL_TRC (
    "[PXR]: TX Power Level Measurement Timer %p Started, retval 0x%04X\n",
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

void appl_send_tx_power_ntf (APPL_HANDLE    * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    /**
     *  Transfer intermediate measurements before transferring the
     *  stable measurement
     */
    hndl_val_param.handle    = appl_tps_tx_pw_lvl_hndl;
    hndl_val_param.value.val = &appl_px_tx_power_level;
    hndl_val_param.value.len = APPL_PX_TX_POWER_LEVEL_LENGTH;

    retval = BT_att_send_hndl_val_ntf
             (
                 &APPL_GET_ATT_INSTANCE(*handle),
                 &hndl_val_param
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC (
        "[** ERR **]: Failed to send measurement, reason 0x%04X", retval);
    }
}

API_RESULT appl_pxr_fetch_tx_power_level(APPL_HANDLE handle)
{
    API_RESULT retval;

    APPL_TRC (
    "[PXR]: Fetching TX Power Level for Appl Handle 0x%02X with "
    "Conn handle 0x%04X\n", handle, APPL_GET_CONNECTION_HANDLE(handle));

    retval = BT_hci_read_transmit_power_level
             (
                 APPL_GET_CONNECTION_HANDLE(handle),
                 HCI_READ_CUR_TX_POWER_LEVEL
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC (
        "[** ERR **]: Failed to Fetch Power Level, reason 0x%04X", retval);
    }

    return retval;
}

void appl_pxr_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_PXR_HAVE_DATA_DUMP
    CONSOLE_OUT("\n[PXR]: NTF Sent for Appl Handle 0x%02X", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_PXR_HAVE_DATA_DUMP */
}

void appl_pxr_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[PXR]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* (defined ATT && defined PXR) */

