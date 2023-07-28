
/**
 *  \file appl_hts.c
 *
 *  This file contains the Sample Health Thermometer Sensor application.
 *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery
 *           - Device Information and
 *           - Health Thermometer
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Health Thermometer Service exposes:
 *           - Temperature Measurement
 *           - Intermediate Temperature Measurement
 *           - Temperature Type
 *      d. Temperature Measurement and Intermediate Temperature Measurement can
 *         be configured to send Indications and Notifications respectively.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Measurement Transfer by writing to the
 *             Characteristic Client Configuration of Temperature Measurement.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_temperature_measurment_handler and
 *           - appl_hts_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements every
 *         10s. This interval can be altered using the define
 *         APPL_TEMPERATURE_MEASUREMENT_INTERVAL. In absence of a real sensor
 *         to provide temperature readings, a static reading of 36.4 C.
 *         Three intermediate temperature measurements of 35.9 C, 36.1 C and
 *         36.3 C are sent before the sending the Temperature Measurement if
 *         collector has configured the device to send Intermediate Temperature
 *         values.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined HTS)

/* --------------------------------------------- Global Definitions */
/**
 * This Flag controls dumps and prints of HTS related Handle Value
 * Indication and Notification Complete Status.
 * This Flag is disabled by default.
 */
/* #define APPL_HTS_HAVE_DATA_DUMP */

/**
 * This Flag enables the application to initiate Link Disconnection
 * when the HTS measurement Notification count reaches a Threshold value.
 * This is typically needed in some of the compliance Testcase where
 * the compliance tester requires Server to disconnect the channel when
 * available measurements are transfered.
 *
 * This flag is disabled by default.
 */
/* #define APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE */
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* Timer related Globals */
static BT_timer_handle timer_handle;
static BT_timer_handle msrmt_intrvl_timer_handle;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_temp_db_handle;

/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_temperature_hndl;
static ATT_ATTR_HANDLE appl_im_temperature_hndl;
static ATT_ATTR_HANDLE appl_msrmt_intrvl_hndl;

/* Service data and states related Globals */
static UCHAR           im_temp_enabled;
static UINT16          appl_msrmt_intrvl;
static UINT16          appl_min_msrmt_intrvl;
static UINT16          appl_max_msrmt_intrvl;


/* Temperature Measurement Format Float. Value 36.4 corresponds to 0xFB378AC0 */
static UCHAR temp_obs_data[APPL_TEMPERATURE_MEASUREMENT_LENGTH] =
{
    /* Flag */
    0x02U,

    /* Temperature Measurement Value */
    0xC0U, 0x8AU, 0x37U, 0xFBU,

    /* Time Stamp */
    0xDEU, 0x07U, 0x04U, 0x1DU, 0x06U, 0x00U, 0x01U
};

/**
 * Temperature Measurement Format Float. Value(s)
 *  -  35.9 corresponds to 0xFB36C770
 *  -  36.1 corresponds to 0xFB371590
 *  -  36.3 corresponds to 0xFB3763B0
 */
static UCHAR intrm_temp_obs_data[APPL_IM_TEMPERATURE_MEASUREMENT_LENGTH * APPL_IM_TEMPERATURE_COUNT] =
{
    /* 35.9 */
    0x00U, 0x70U, 0xC7U, 0x36U, 0xFBU,

    /* 36.1 */
    0x00U, 0x90U, 0x15U, 0x37U, 0xFBU,

    /* 36.3 */
    0x00U, 0xB0U, 0x63U, 0x37U, 0xFBU
};

#ifdef APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE
static UCHAR appl_hts_msrmt_count;
#endif /* APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE */
/* --------------------------------------------- Functions */

void appl_hts_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_hts_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

    im_temp_enabled        = BT_FALSE;

    /* Initialize the Measurement Interval */
    appl_msrmt_intrvl      = APPL_TEMPERATURE_MEASUREMENT_INTERVAL;

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_hts_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_hts_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_hts_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_hts_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_hts_add_hts();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the HTS GATT DB Signature */
    appl_hts_db_sign = APPL_HTS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_hts_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    /* Register the GATT DB */
    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[HTS APPL]: GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for Temperature */
    appl_temp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_temp_db_handle.service_id = (UCHAR)GATT_SER_HTS_HEALTH_THERM_INST;
    appl_temp_db_handle.char_id    = (UCHAR)GATT_CHAR_HTS_TEMP_MSRMNT_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for HTS services if Dynamic GATT Database is not Used */
    appl_hts_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register HTS GATT DB Handler with PL Extension */
    (BT_IGNORE_RETURN_VALUE) gatt_db_init_pl(gatt_db_hts_gatt_char_handler);
}

void appl_hts_bt_init(void)
{
    APPL_TRC("[HTS]: In appl_hts_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_hts_server_reinitialize();
}

void appl_hts_bt_deinit(void)
{
    APPL_TRC("[HTS]: In appl_hts_bt_deinit\n");

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);

        APPL_TRC (
        "[HTS]: Temperature Measurement Timer %p Stopped\n", timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    /* Start timer to simulate Measurement Interval Indication */
    if (BT_TIMER_HANDLE_INIT_VAL != msrmt_intrvl_timer_handle)
    {
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer (msrmt_intrvl_timer_handle);

        APPL_TRC (
        "[HTS]: Measurement Interval Timer Stopped%p\n",
        msrmt_intrvl_timer_handle);

        msrmt_intrvl_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_hts_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;
    API_RESULT        retval;

    cli_cnfg = 0U;

    appl_temp_db_handle.device_id = (*dq_handle);

    /* If Temperature measurement is configured, initiate transfer */
    appl_temp_db_handle.char_id   = (UCHAR)GATT_CHAR_HTS_TEMP_MSRMNT_INST;

    BT_gatt_db_get_char_val_hndl
    (
        &appl_temp_db_handle,
        &appl_temperature_hndl
    );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_temp_db_handle,&value);
    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }
    APPL_TRC (
    "[HTS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_temp_db_handle,cli_cnfg);
    }

    /* Check if intermediate measurement is configured by the client */
    appl_temp_db_handle.char_id = (UCHAR)GATT_CHAR_HTS_INTRM_TEMP_INST;
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_temp_db_handle,&value);
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
        im_temp_enabled = BT_TRUE;
    }

    BT_gatt_db_get_char_val_hndl
    (
        &appl_temp_db_handle,
        &appl_im_temperature_hndl
    );

   /* Get Valid Range for Measurement Interval characteristic */
    appl_temp_db_handle.char_id = (UCHAR)GATT_CHAR_HTS_MSRMT_INTERVAL_INST;

    retval = BT_gatt_db_get_hld_char_desc_val
             (
                 &appl_temp_db_handle,
                 &value,
                 GATT_VALID_RANGE
             );
    if (API_SUCCESS == retval)
    {
        BT_UNPACK_LE_2_BYTE(&appl_min_msrmt_intrvl, &value.val[0U]);
        BT_UNPACK_LE_2_BYTE(&appl_max_msrmt_intrvl, &value.val[2U]);
    }

    (BT_IGNORE_RETURN_VALUE) BT_gatt_db_get_char_cli_cnfg(&appl_temp_db_handle,&value);
    appl_temp_db_handle.char_id = (UCHAR)GATT_CHAR_HTS_MSRMT_INTERVAL_INST;
    BT_gatt_db_get_char_val_hndl
    (
        &appl_temp_db_handle,
        &appl_msrmt_intrvl_hndl
    );
}


void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    API_RESULT     retval;
    APPL_HANDLE    appl_handle;

    /* Init */
    retval = API_SUCCESS;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle (handle.device_id,&appl_handle);

        /* Security requirements satisfied, go ahead with data transfer */
        if (GATT_CHAR_HTS_TEMP_MSRMNT_INST == handle.char_id)
        {
            if (GATT_CLI_CNFG_INDICATION == config)
            {
                /* Start timer to simulate Temperature measurements */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);

                    APPL_TRC (
                    "[HTS]: Temperature Measurement Timer %p Stopped\n",
                    timer_handle);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                (BT_IGNORE_RETURN_VALUE) BT_start_timer
                (
                    &timer_handle,
                    appl_msrmt_intrvl,
                    appl_timer_expiry_handler,
                    &appl_handle,
                    sizeof (appl_handle)
                );

                APPL_TRC (
                "[HTS]: Temperature Measurement Timer %p Started\n",
                timer_handle);
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                /* Stop transfer simulate Temperature measurements */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);

                    APPL_TRC (
                    "[HTS]: Temperature Measurement Timer %p Stopped\n",
                    timer_handle);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else if (GATT_CHAR_HTS_INTRM_TEMP_INST == handle.char_id)
        {
            if (GATT_CLI_CNFG_NOTIFICATION == config)
            {
                im_temp_enabled = BT_TRUE;
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                im_temp_enabled = BT_FALSE;
            }
            else
            {
                APPL_TRC ("[*** ERR ***]:Not the right transfer type!\n");
            }
        }
        else if (GATT_CHAR_HTS_MSRMT_INTERVAL_INST == handle.char_id)
        {
            if (GATT_CLI_CNFG_INDICATION == config)
            {
                /* Start timer to simulate Measurement Interval Indication */
                if (BT_TIMER_HANDLE_INIT_VAL != msrmt_intrvl_timer_handle)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_stop_timer (msrmt_intrvl_timer_handle);

                    APPL_TRC (
                    "[HTS]: Measurement Interval Timer Stopped%p\n",
                    msrmt_intrvl_timer_handle);

                    msrmt_intrvl_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                (BT_IGNORE_RETURN_VALUE) BT_start_timer
                (
                    &msrmt_intrvl_timer_handle,
                    APPL_MSRMT_INTRVL_DATA_INTERVAL,
                    appl_msrmt_intrvl_timer_expiry_handler,
                    &appl_handle,
                    sizeof (appl_handle)
                );

                APPL_TRC (
                "[HTS]: Measurement Interval Timer %p Started\n",
                msrmt_intrvl_timer_handle);
            }
        }
        else
        {
            APPL_TRC ("[*** ERR ***]:Not the right transfer type!\n");
        }
    }
}

API_RESULT appl_validate_msrmt_interval (GATT_DB_HANDLE * handle, UINT16 data)
{
    APPL_HANDLE appl_handle;
    API_RESULT  retval;
    UINT16      prev_msrmt_val, new_msrmt_val, cli_cnfg;
    ATT_VALUE   value;

    retval = API_SUCCESS;

    /* Store previous and current values of Mesurement Interval */
    prev_msrmt_val = appl_msrmt_intrvl;
    new_msrmt_val  = data;

    if (0x0000U == data)
    {
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);
            APPL_TRC (
            "[HTS]: Measurement Interval set to %d! Stopped Timer %p\n",
            data, timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
    else if (!(data >= appl_min_msrmt_intrvl && data <= appl_max_msrmt_intrvl))
    {
        retval = APPL_OUT_OF_RANGE_ERROR; /* return APPL_OUT_OF_RANGE_ERROR; */
    }
    else
    {
        APPL_TRC (
        "[HTS]: Measurement Interval Change to be in-effect after Tx \n"
        "of any pending/Ongoing Temperature Measurements!\n");
    }

    if (API_SUCCESS == retval)
    {
        /**
         * NOTE:
         * If the Measurement Interval is set to 0 by the Collector.
         * Then in the subsequent connection this value will be updated to default.
         * In the current connection it will remain 0, until Collector
         * writes a valid value again.
         */
        appl_msrmt_intrvl = data;

        APPL_TRC (
        "[HTS]: Measurement Interval is Set to %d seconds\n",
        appl_msrmt_intrvl);

        /*
         * If Measurement interval is a non-zero value:
         *
         * Check if the previous value of Measurement value was 0
         * And if the current value is non-zero.
         * Check if the CCCD is Enabled for HTS Measurement.
         * Check if timer is not started.
         * The start the timer with new Measurement Interval.
         */
        if ((0U == prev_msrmt_val) && (0U != new_msrmt_val))
        {
            /* Fetch the Application Handle from Device Handle */
            (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle
            (
                handle->device_id,
                &appl_handle
            );

            /* Populate the GATT DB HANDLE for Temperature */
            appl_temp_db_handle.device_id  = handle->device_id;
            appl_temp_db_handle.service_id = (UCHAR)GATT_SER_HTS_HEALTH_THERM_INST;
            appl_temp_db_handle.char_id    = (UCHAR)GATT_CHAR_HTS_TEMP_MSRMNT_INST;

            /* Get the CCCD Value of HTS Measurement */
            retval = BT_gatt_db_get_char_cli_cnfg
                     (
                         &appl_temp_db_handle,
                         &value
                     );

            if (retval != API_SUCCESS)
            {
                cli_cnfg = 0x0000U;
            }
            else
            {
                BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
            }

            APPL_TRC (
            "[HTS]: Fetched Temp Measurement CCCD (0x%04X) for Device (0x%02X)\n",
            cli_cnfg, handle->device_id);

            /**
             * Restart timer if it was previously stopped
             * to simulate Temperature measurements
             */
            if ((BT_TIMER_HANDLE_INIT_VAL == timer_handle) &&
                (GATT_CLI_CNFG_INDICATION == cli_cnfg))
            {
                retval = BT_start_timer
                         (
                             &timer_handle,
                             appl_msrmt_intrvl,
                             appl_timer_expiry_handler,
                             &appl_handle,
                             sizeof(appl_handle)
                         );

                APPL_TRC (
                "[HTS]: Temperature Measurement Timer %p Started, retval 0x%04X\n",
                timer_handle, retval);
            }
            else
            {
                APPL_ERR (
                "[HTS]: CCCD not configured or Measurements already ongoing!\n");
            }
        }
    }

    return retval;
}

void appl_msrmt_intrvl_timer_expiry_handler (void *data_param, UINT16 datalen)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    UCHAR temp_msrmt_int_buff[2U];
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(datalen);

    /* Stop timer */
    if (BT_TIMER_HANDLE_INIT_VAL != msrmt_intrvl_timer_handle)
    {
        APPL_TRC (
        "[HTS]: Measurement Interval Timer %p Expired!\n",
        msrmt_intrvl_timer_handle);

        msrmt_intrvl_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    hndl_val_param.handle     = appl_msrmt_intrvl_hndl;
    hndl_val_param.value.val  = temp_msrmt_int_buff;
    BT_PACK_LE_2_BYTE(hndl_val_param.value.val,&appl_msrmt_intrvl);
    hndl_val_param.value.len  = APPL_MSRMT_INTRVL_DATA_LENGTH;

    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(*((APPL_HANDLE *)data_param)),
                 &hndl_val_param
              );
    if (API_SUCCESS != retval)
    {
        APPL_TRC ("[** ERR **]: Failed to send Measurement Interval,"
        "reason 0x%04X\n", retval);
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
        "[HTS]: Temperature Measurement Timer %p Expired!\n", timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

#ifdef APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE
    appl_hts_msrmt_count++;

    if (3 == appl_hts_msrmt_count)
    {
        retval = BT_hci_disconnect
                 (
                     APPL_GET_CONNECTION_HANDLE(fsm_param.handle),
                     0x13U
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR(
            "Failed to Disconnect Connection Handle 0x%04X with DevId 0x%02X\n",
            APPL_GET_CONNECTION_HANDLE(fsm_param.handle),
            APPL_GET_DEVICE_HANDLE(fsm_param.handle));
        }
        else
        {
            APPL_TRC(
            "Disconnected Connection Handle 0x%04X with DevId 0x%02X\n",
            APPL_GET_CONNECTION_HANDLE(fsm_param.handle),
            APPL_GET_DEVICE_HANDLE(fsm_param.handle));
        }
    }
    else
#endif /* APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE */
    {
        retval = BT_start_timer
                 (
                     &timer_handle,
                     appl_msrmt_intrvl,
                     appl_timer_expiry_handler,
                     &fsm_param.handle,
                     sizeof(fsm_param.handle)
                 );

        APPL_TRC(
        "[HTS]: Temperature Measurement Timer %p Started, retval 0x%04X\n",
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
}


void appl_hts_server_reinitialize(void)
{
    ATT_VALUE         value;

    /* MISRA C - 2012 Rule 9.1 */
    BT_mem_set(&value, 0,sizeof(ATT_VALUE));

    appl_temp_db_handle.char_id = (UCHAR)GATT_CHAR_HTS_MSRMT_INTERVAL_INST;

    /* Get Temperature Measurement Interval from DB */
    (BT_IGNORE_RETURN_VALUE) BT_gatt_db_get_char_val(&appl_temp_db_handle, &value);

    if ((0x02U == value.len) &&
        (NULL != value.val))
    {
        BT_UNPACK_LE_2_BYTE(&appl_msrmt_intrvl, value.val);

        if (0U == appl_msrmt_intrvl)
        {
            /**
             * If the value was reset in the previous connection,
             * switch to default.
             */
            appl_msrmt_intrvl = APPL_TEMPERATURE_MEASUREMENT_INTERVAL;
        }
    }
    else
    {
       /* set default value */
       appl_msrmt_intrvl = APPL_TEMPERATURE_MEASUREMENT_INTERVAL;
    }

    APPL_TRC (
    "[HTS]: Current HTS Temperature Measurement Interval is %d\n",
    appl_msrmt_intrvl);

    /* Stop Timer for Temperature measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
    if (BT_TIMER_HANDLE_INIT_VAL != msrmt_intrvl_timer_handle)
    {
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer(msrmt_intrvl_timer_handle);
        msrmt_intrvl_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

#ifdef APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE
    /* Set the Measurement Transfer Count */
    appl_hts_msrmt_count = 0;
#endif /* APPL_HTS_DISCONN_ON_MSRMT_TX_COMPLETE */

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

void appl_send_temperature_measurement (APPL_HANDLE    * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 im_index;

    /**
     *  Transfer intermediate measurements before transferring the
     *  stable measurement
     */
    im_index = 0U;
    if (BT_FALSE != im_temp_enabled)
    {
        do
        {
            hndl_val_param.handle =  appl_im_temperature_hndl;
            hndl_val_param.value.val = (&intrm_temp_obs_data[(5U * im_index)]);
            hndl_val_param.value.len = APPL_IM_TEMPERATURE_MEASUREMENT_LENGTH;
            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                      );
            im_index++;
        }while (im_index < APPL_IM_TEMPERATURE_COUNT);
    }
    hndl_val_param.handle =  appl_temperature_hndl;
    hndl_val_param.value.val = temp_obs_data;
    hndl_val_param.value.len = APPL_TEMPERATURE_MEASUREMENT_LENGTH;

    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(*handle),
                 &hndl_val_param
              );
    if (API_SUCCESS != retval)
    {
        APPL_TRC ("[** ERR **]: Failed to send measurement, reason 0x%04X",
        retval);

        /* Stop any previously invoked timers */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer (timer_handle);

            APPL_TRC (
            "[HTS]: Stopping Temperature Measurement Timer %p\n", timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
}

void appl_hts_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(evt_result);

#ifdef APPL_HTS_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[HTS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
#endif /* APPL_HTS_HAVE_DATA_DUMP */
}

void appl_hts_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_HTS_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[HTS]: NTF Sent for Appl Handle 0x%02X", *handle);

    appl_dump_bytes(event_data, datalen);
#endif /* APPL_HTS_HAVE_DATA_DUMP */
}

void appl_hts_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[HTS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* (defined ATT && defined HTS) */

