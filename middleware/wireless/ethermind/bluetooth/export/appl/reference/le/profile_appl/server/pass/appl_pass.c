
/**
 *  \file appl_pass.c
 *
 *  This file contains the Sample Phone Alert Status Sensor application.
 *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Central
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery and
 *           - Phone Alert Status
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Phone Alert Status Service exposes:
 *           - Alert Status
 *           - Ringer Settings
 *           - Ringer Control Point
 *      d. Alert Status and Ringer Settings can be configured to send
 *         Notifications.
 *      e. appl_start_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Alert Status Notifications Transfer by
 *             writing to the Characteristic Client Configuration of Alert
 *             Status.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_start_transfer, is therefore called from:
 *           - gatt_db_pass_handler and
 *           - appl_pass_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements every
 *         10s. This interval can be altered using the define
 *         APPL_ALERT_STATUS_INTERVAL and APPL_RINGER_SETTING_INTERVAL.
 *         In absence of a real sensor to provide alert status and ringer
 *         settings readings, appl_update_alert_status and
 *         appl_update_ringer_setting is used to simulate value change of
 *         alert status and ringer setting characteristic respectively.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined PASS)

/* --------------------------------------------- Global Definitions */
/* GATT DB Handle related Globals, to be used in mops file */
GATT_DB_HANDLE  appl_alert_status_db_hndl;
GATT_DB_HANDLE  appl_ringer_settings_db_hndl;
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Timer related Globals */
BT_timer_handle alert_status_timer_handle;
BT_timer_handle ringer_setting_timer_handle;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_ringer_cntrl_point_db_hndl;

/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_alert_status_hndl;
static ATT_ATTR_HANDLE appl_ringer_settings_hndl;
/* Service data and states related Globals */
static ATT_ATTR_HANDLE appl_ringer_control_point_hndl;
static UCHAR           alert_status_enabled;
static UCHAR           ringer_setting_enabled;
/**
 *  This configuration flag controls whether the notifications
 *  are sent automatically or from the Menu
 */
UCHAR           appl_notification_state;
/* --------------------------------------------- Functions */

void appl_pass_set_gap_central_role(void)
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

void appl_pass_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_pass_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval      = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

    alert_status_timer_handle   = BT_TIMER_HANDLE_INIT_VAL;
    ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    alert_status_enabled        = BT_FALSE;
    ringer_setting_enabled      = BT_FALSE;
    appl_notification_state     = APPL_AUTO_SEND_NOTIFICATION;

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_pass_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_pass_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_pass_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_pass_add_pass();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the PASS GATT DB Signature */
    appl_pass_db_sign = APPL_PASS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_pass_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[PASS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for PASS Alert Status, Ringer Setting, Control Point */
    appl_alert_status_db_hndl.device_id        = DEVICE_HANDLE_INIT_VAL;
    appl_alert_status_db_hndl.service_id       = (UCHAR)GATT_SER_PASS_PHN_ALRT_STATUS_INST;
    appl_alert_status_db_hndl.char_id          = (UCHAR)GATT_CHAR_PASS_ALRT_STATUS_INST;

    appl_ringer_settings_db_hndl.device_id     = DEVICE_HANDLE_INIT_VAL;
    appl_ringer_settings_db_hndl.service_id    = (UCHAR)GATT_SER_PASS_PHN_ALRT_STATUS_INST;
    appl_ringer_settings_db_hndl.char_id       = (UCHAR)GATT_CHAR_PASS_RING_SET_INST;

    appl_ringer_cntrl_point_db_hndl.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_ringer_cntrl_point_db_hndl.service_id = (UCHAR)GATT_SER_PASS_PHN_ALRT_STATUS_INST;
    appl_ringer_cntrl_point_db_hndl.char_id    = (UCHAR)GATT_CHAR_PASS_RING_CTRL_PNT_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for PASS services if Dynamic GATT Database is not Used */
    appl_pass_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register PASS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_pass_gatt_char_handler);

    /**
     * Default GAP Role is Central for this Service.
     * But, during init time setting the role to Peripheral.
     * This needs to be updated from the HPS Menu after the
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
    "Default GAP Role is GAP Central for this PASS Service. But, during\n"
    "init time setting the role to Peripheral. This needs to be updated\n"
    "from the PASS Menu after the desired Peer Device Address to which this\n"
    "service needs to scan and auto initiate connection is Set.\n"
    "\n        ** User needs to select the role from the PASS Menu **");
    APPL_TRC(
    "\n=====================================================================\n\n");
}

void appl_pass_bt_init(void)
{
    APPL_TRC("[PASS]: In appl_pass_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_pass_server_reinitialize();
}

void appl_pass_bt_deinit(void)
{
    APPL_TRC("[PASS]: In appl_pass_bt_deinit\n");

     /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
    {
        BT_stop_timer (alert_status_timer_handle);
        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
    {
        BT_stop_timer(ringer_setting_timer_handle);
        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_pass_connect(DEVICE_HANDLE* dq_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;
    API_RESULT        retval;

    cli_cnfg = 0U;

    appl_alert_status_db_hndl.device_id = (*dq_handle);
    appl_ringer_settings_db_hndl.device_id = (*dq_handle);

    /* If Alert Status is configured, initiate transfer */
    appl_alert_status_db_hndl.char_id = (UCHAR)GATT_CHAR_PASS_ALRT_STATUS_INST;

    BT_gatt_db_get_char_val_hndl
    (
        &appl_alert_status_db_hndl,
        &appl_alert_status_hndl
    );

    retval = BT_gatt_db_get_char_cli_cnfg
    (
        &appl_alert_status_db_hndl,
        &value
    );
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC(
        "[PASS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
        cli_cnfg, (*dq_handle));

    if (0U != cli_cnfg)
    {
        appl_start_trasnfer(appl_alert_status_db_hndl, cli_cnfg);
    }

    /* Check if Ringer Setting is configured by the client */
    appl_ringer_settings_db_hndl.char_id = (UCHAR)GATT_CHAR_PASS_RING_SET_INST;

    BT_gatt_db_get_char_val_hndl
    (
        &appl_ringer_settings_db_hndl,
        &appl_ringer_settings_hndl
    );

    retval = BT_gatt_db_get_char_cli_cnfg
             (
                 &appl_ringer_settings_db_hndl,
                 &value
             );
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    if (0U != cli_cnfg)
    {
        appl_start_trasnfer(appl_ringer_settings_db_hndl, cli_cnfg);
    }
}


void appl_start_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    API_RESULT   retval;
    APPL_HANDLE    appl_handle;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        /* Security requirements satisfied, go ahead with data transfer */
        if ((GATT_CHAR_PASS_ALRT_STATUS_INST == handle.char_id) ||
            (GATT_CHAR_PASS_RING_SET_INST == handle.char_id))
        {
            appl_get_handle_from_device_handle (handle.device_id, &appl_handle);

            if (GATT_CLI_CNFG_NOTIFICATION == config)
            {
                if (GATT_CHAR_PASS_ALRT_STATUS_INST == handle.char_id)
                {
                    /* Start timer to simulate Status Change */
                    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
                    {
                        BT_stop_timer (alert_status_timer_handle);

                        APPL_TRC (
                        "[PASS]: Alert/Ringer Setting Timer %p Stopped!\n",
                        alert_status_timer_handle);

                        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    alert_status_enabled = BT_TRUE;

                    BT_start_timer
                    (
                        &alert_status_timer_handle,
                        APPL_ALERT_STATUS_INTERVAL,
                        appl_alert_status_timer_expiry_handler,
                        &appl_handle,
                        sizeof (APPL_HANDLE)
                    );
                    APPL_TRC (
                    "[PASS]: Alert Setting Timer %p Started!\n",
                        alert_status_timer_handle);
                }
                else if (GATT_CHAR_PASS_RING_SET_INST == handle.char_id)
                {
                    /* Start Transfer Timer to Send measurement every time interval */
                    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
                    {
                        BT_stop_timer (ringer_setting_timer_handle);
                        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    ringer_setting_enabled = BT_TRUE;

                    BT_start_timer
                    (
                        &ringer_setting_timer_handle,
                        APPL_RINGER_SETTING_INTERVAL,
                        appl_ringer_setting_timer_expiry_handler,
                        &appl_handle,
                        sizeof (appl_handle)
                    );

                    APPL_TRC (
                    "[PASS]: Ringer Setting Timer %p Started!\n",
                    ringer_setting_timer_handle);
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                if (GATT_CHAR_PASS_ALRT_STATUS_INST == handle.char_id)
                {
                    /* Stop transfer simulate Status Change */
                    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
                    {
                        BT_stop_timer (alert_status_timer_handle);

                        APPL_TRC (
                        "[PASS]: Alert Setting Timer %p Stopped!\n",
                        alert_status_timer_handle);

                        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    alert_status_enabled = BT_FALSE;
                }
                else if (GATT_CHAR_PASS_RING_SET_INST == handle.char_id)
                {
                    /* Stop transfer simulate Status Change */
                    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
                    {
                        BT_stop_timer (ringer_setting_timer_handle);

                        APPL_TRC (
                        "[PASS]: Ringer Setting Timer %p Stopped!\n",
                        ringer_setting_timer_handle);

                        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    ringer_setting_enabled = BT_FALSE;
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
        }
    }
}

void appl_send_phone_alert_notifications (APPL_HANDLE  * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 alert_status_value;
    UCHAR                 ringer_settings_value;

    if (APPL_AUTO_SEND_NOTIFICATION == appl_notification_state)
    {
        hndl_val_param.value.len = 1U;

        /** Send Notifications for Alert Status Change */
        if (BT_FALSE != alert_status_enabled)
        {
            hndl_val_param.handle =  appl_alert_status_hndl;
            hndl_val_param.value.val = &alert_status_value;

            retval = BT_gatt_db_get_char_val
                     (
                         &appl_alert_status_db_hndl,
                         &hndl_val_param.value
                     );

            appl_update_alert_status (hndl_val_param.value.val);

            retval = BT_gatt_db_set_char_val
                     (
                         &appl_alert_status_db_hndl,
                         &hndl_val_param.value
                     );

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                      );
        }
        /** Send Notifications for Ringer Settings Change */
        if (BT_FALSE != ringer_setting_enabled)
        {
            hndl_val_param.handle =  appl_ringer_settings_hndl;
            hndl_val_param.value.val = &ringer_settings_value;

            retval = BT_gatt_db_get_char_val
                     (
                         &appl_ringer_settings_db_hndl,
                         &hndl_val_param.value
                     );

            appl_update_ringer_setting (hndl_val_param.value.val);

            retval = BT_gatt_db_set_char_val
                     (
                         &appl_ringer_settings_db_hndl,
                         &hndl_val_param.value
                     );

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                      );
        }
    }
}

API_RESULT appl_pass_read_request_handler
           (
               GATT_DB_HANDLE    * handle
           )
{
    ATT_VALUE    att_value;
    API_RESULT   retval;
    APPL_HANDLE  appl_handle;
    UCHAR        value;

    retval        = API_SUCCESS;
    att_value.len = 1U;
    att_value.val = &value;

    if (GATT_CHAR_PASS_ALRT_STATUS_INST == handle->char_id)
    {
        retval = BT_gatt_db_get_char_val
                 (
                     &appl_alert_status_db_hndl,
                     &att_value
                 );
    }
    else if (GATT_CHAR_PASS_RING_SET_INST == handle->char_id)
    {
        retval = BT_gatt_db_get_char_val
                 (
                     &appl_ringer_settings_db_hndl,
                     &att_value
                 );
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    if (API_SUCCESS == retval)
    {
        /* Read Request */
        appl_get_handle_from_device_handle (handle->device_id,&appl_handle);

        retval = BT_att_read_rsp
                 (
                      &APPL_GET_ATT_INSTANCE(appl_handle),
                      &att_value
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR (
            "[PASS]: **ERR** Failed to send Read Response, result 0x%04X!\n",
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
    else
    {
        APPL_ERR (
        "[PASS]: **ERR** Failed to fetch Characterisitic Value for Handle 0x%04X,"
        " result 0x%04X!\n", handle->char_id, retval);
    }

    return retval;
}

void appl_pass_ringer_control_point_write_handler
     (
        GATT_DB_HANDLE    * handle,
        ATT_VALUE         * value
     )
{
    APPL_HANDLE  appl_handle;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 alert_status_value;
    UCHAR                 ringer_settings_value;

    hndl_val_param.value.len = 1U;
    appl_get_handle_from_device_handle (handle->device_id,&appl_handle);

    switch (*(value->val))
    {
        case APPL_PASS_RINGER_CNTRL_PNT_SILENT_MODE:
            APPL_TRC("[PASS]: Set Silent Mode - Ringer Silent\n");
            /** Send Notifications for Ringer Settings Change */
            if (BT_FALSE != ringer_setting_enabled)
            {
                hndl_val_param.handle =  appl_ringer_settings_hndl;
                hndl_val_param.value.val = &ringer_settings_value;

                (*hndl_val_param.value.val) = RINGER_SETTING_RINGER_SILENT;

                retval = BT_gatt_db_set_char_val
                         (
                             &appl_ringer_settings_db_hndl,
                             &hndl_val_param.value
                         );

                retval = BT_att_send_hndl_val_ntf
                         (
                             &APPL_GET_ATT_INSTANCE(appl_handle),
                             &hndl_val_param
                          );
            }
            break;
        case APPL_PASS_RINGER_CNTRL_PNT_MUTE_ONCE:
            APPL_TRC ("[PASS]: Mute Once \n");
            /** Send Notifications for Alert Status Change */
            if (BT_FALSE != alert_status_enabled)
            {
                hndl_val_param.handle =  appl_alert_status_hndl;
                hndl_val_param.value.val = &alert_status_value;

                retval = BT_gatt_db_get_char_val
                         (
                             &appl_alert_status_db_hndl,
                             &hndl_val_param.value
                         );

                appl_clear_bit_value(alert_status_value,ALRT_STATUS_RINGER_STATE_BIT);

                retval = BT_gatt_db_set_char_val
                         (
                             &appl_alert_status_db_hndl,
                             &hndl_val_param.value
                         );

                retval = BT_att_send_hndl_val_ntf
                         (
                             &APPL_GET_ATT_INSTANCE(appl_handle),
                             &hndl_val_param
                          );
            }
            break;
        case APPL_PASS_RINGER_CNTRL_PNT_CANCEL_SILENT_MODE:
            APPL_TRC ("[PASS]: Cancel Silent Mode \n Ringer Normal\n");
            /** Send Notifications for Ringer Settings Change */
            if (BT_FALSE != ringer_setting_enabled)
            {
                hndl_val_param.handle =  appl_ringer_settings_hndl;
                hndl_val_param.value.val = &ringer_settings_value;

                (*hndl_val_param.value.val) = RINGER_SETTING_RINGER_NORMAL;

                retval = BT_gatt_db_set_char_val
                         (
                             &appl_ringer_settings_db_hndl,
                             &hndl_val_param.value
                         );

                retval = BT_att_send_hndl_val_ntf
                         (
                             &APPL_GET_ATT_INSTANCE(appl_handle),
                             &hndl_val_param
                          );
            }
            break;
        default:
            APPL_TRC ("[PASS]: Unknown Ringer Control Command \n");
            if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
            {
                BT_stop_timer (ringer_setting_timer_handle);

                APPL_TRC (
                "[PASS]: Ringer Setting Timer %p Stopped!\n",
                ringer_setting_timer_handle);

                ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }
            if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
            {
                BT_stop_timer (alert_status_timer_handle);

                APPL_TRC (
                "[PASS]: Alert Setting Timer %p Stopped!\n",
                alert_status_timer_handle);

                alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }
            break;
    }
}

void appl_alert_status_timer_expiry_handler (void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    APPL_EVENT_PARAM   fsm_param;

    fsm_param.handle = *((APPL_HANDLE *)data);

    /* Restart timer to simulate Temperature measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
    {
        /**
         * Console Prints are disabled for timeouts as this will hinder
         * with the user experience when the Notifications for Alert Status
         * or Ringer Status are to be triggered from the Menu.
         */
        /* APPL_TRC (
        "[PASS]: Timeout Occurred: %p\n", alert_status_timer_handle); */

        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &alert_status_timer_handle,
        APPL_ALERT_STATUS_INTERVAL,
        appl_alert_status_timer_expiry_handler,
        &fsm_param.handle,
        sizeof(fsm_param.handle)
    );

    /**
     * Console Prints are disabled for timeouts as this will hinder
     * with the user experience when the Notifications for Alert Status
     * or Ringer Status are to be triggered from the Menu.
     */
    /* APPL_TRC (
    "[PASS]: Started Timer %p\n", alert_status_timer_handle); */

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

void appl_ringer_setting_timer_expiry_handler (void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    APPL_EVENT_PARAM   fsm_param;
    fsm_param.handle = (*((APPL_HANDLE *)data));

    /* Restart timer to simulate Temperature measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
    {
        /**
         * Console Prints are disabled for timeouts as this will hinder
         * with the user experience when the Notifications for Alert Status
         * or Ringer Status are to be triggered from the Menu.
         */
        /* APPL_TRC (
        "[PASS]: Timeout Occurred: %p\n", ringer_setting_timer_handle); */

        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &ringer_setting_timer_handle,
        APPL_RINGER_SETTING_INTERVAL,
        appl_ringer_setting_timer_expiry_handler,
        &fsm_param.handle,
        sizeof(fsm_param.handle)
    );

    /**
     * Console Prints are disabled for timeouts as this will hinder
     * with the user experience when the Notifications for Alert Status
     * or Ringer Status are to be triggered from the Menu.
     */
    /* APPL_TRC (
    "[PASS]: Started Timer %p\n", ringer_setting_timer_handle); */
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

void appl_pass_server_reinitialize (void)
{
    /* Stop Timer for Phone Alert Status Change */
    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
    {
        BT_stop_timer (alert_status_timer_handle);
        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
    /* Stop Timer for Phone Alert Status Change */
    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
    {
        BT_stop_timer (ringer_setting_timer_handle);
        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

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

void appl_update_alert_status (UCHAR * val)
{
    /** Toggle ALRT_STATUS_RINGER_STATE_BIT */
    if (appl_get_bit_value(*val,ALRT_STATUS_RINGER_STATE_BIT))
    {
        appl_clear_bit_value(*val,ALRT_STATUS_RINGER_STATE_BIT);
    }
    else
    {
        appl_set_bit_value(*val,ALRT_STATUS_RINGER_STATE_BIT);
    }

    /** Toggle ALRT_STATUS_VIBRATOR_STATE_BIT */
    if (appl_get_bit_value(*val,ALRT_STATUS_VIBRATOR_STATE_BIT))
    {
        appl_clear_bit_value(*val,ALRT_STATUS_VIBRATOR_STATE_BIT);
    }
    else
    {
        appl_set_bit_value(*val,ALRT_STATUS_VIBRATOR_STATE_BIT);
    }

    /** Toggle ALRT_STATUS_DISP_ALRT_STATE_BIT */
}

void appl_update_ringer_setting (UCHAR * val)
{
    if (RINGER_SETTING_RINGER_SILENT == (*val))
    {
        (*val) = RINGER_SETTING_RINGER_NORMAL;
    }
    else
    {
        (*val) = RINGER_SETTING_RINGER_SILENT;
    }
}

void appl_send_alert_status_ntf (void)
{
    APPL_HANDLE           appl_handle;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 alert_status_value;

    if (APPL_MENU_SEND_NOTIFICATION == appl_notification_state)
    {
        appl_get_handle_from_device_handle
        (appl_alert_status_db_hndl.device_id,&appl_handle);

        hndl_val_param.value.len = 1U;

        /** Send Notifications for Alert Status Change */
        if (BT_FALSE != alert_status_enabled)
        {
            hndl_val_param.handle =  appl_alert_status_hndl;
            hndl_val_param.value.val = &alert_status_value;

            retval = BT_gatt_db_get_char_val
                     (
                         &appl_alert_status_db_hndl,
                         &hndl_val_param.value
                     );

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(appl_handle),
                         &hndl_val_param
                      );
        }
    }
}

void appl_send_ringer_settings_ntf (void)
{
    APPL_HANDLE           appl_handle;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 ringer_settings_value;

    if (APPL_MENU_SEND_NOTIFICATION == appl_notification_state)
    {
        appl_get_handle_from_device_handle
        (appl_ringer_settings_db_hndl.device_id,&appl_handle);

        hndl_val_param.value.len = 1U;

        /** Send Notifications for Ringer Settings Change */
        if (BT_FALSE != ringer_setting_enabled)
        {
            hndl_val_param.handle =  appl_ringer_settings_hndl;
            hndl_val_param.value.val = &ringer_settings_value;

            retval = BT_gatt_db_get_char_val
                     (
                         &appl_ringer_settings_db_hndl,
                         &hndl_val_param.value
                     );

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(appl_handle),
                         &hndl_val_param
                      );
        }
    }
}

void appl_pass_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    CONSOLE_OUT("\n [PASS] IND Completed for Appl Handle 0x%02X with result"
    "0x%04X\n", (*handle), evt_result);
}

void appl_pass_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
#ifdef APPL_PASS_HAVE_DATA_DUMP
    /**
     * Console Prints are disabled for NTF Complete as this will hinder
     * with the user experience when the Notifications for Alert Status
     * or Ringer Status are to be triggered from the Menu.
     * The flag APPL_PASS_HAVE_DATA_DUMP is disbled by default.
     */
    CONSOLE_OUT("\n [PASS] NTF Sent for Appl Handle 0x%02X", *handle);
    appl_dump_bytes(event_data, datalen);
#else /* APPL_PASS_HAVE_DATA_DUMP */
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);
#endif /* APPL_PASS_HAVE_DATA_DUMP */
}

void appl_pass_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    CONSOLE_OUT("\n [PASS] Updated MTU is %d for Appl Handle 0x%02X", mtu, *handle);
}

#endif /* (defined ATT && defined PASS) */

