/**
 *  \file appl_bps.c
 *
 *  This file contains the Sample Blood Pressure Monitor application.
 *  Sample applications detailed below:
 *      a. The Monitor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Monitor application has following service records:
 *           - GAP
 *           - GATT
 *           - Battery
 *           - Device Information and
 *           - Blood Pressure
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Blood Pressure Service exposes:
 *           - Blood Pressure Measurement
 *           - Intermediate Cuff Pressure
 *           - Blood Pressure Feature
 *      d. Blood Pressure Measurement and Intermediate Cuff Pressure can
 *         be configured to send Indications and Notifications respectively.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Measurement Transfer by writing to the
 *             Characteristic Client Configuration of Blood Pressure
 *             Measurement.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_bps_handler and
 *           - appl_bps_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send measurements every
 *         10s. This interval can be altered using the define
 *         APPL_BP_MEASUREMENT_INTERVAL. In absence of a real sensor to provide
 *         blood pressure readings, a static reading of 120-80-100 mmHg.
 *         Three intermediate cuff pressures of 130-95-80 mmHg, 126-90-85 mmHg
 *         and 123-85-90 mmHg are sent before the sending the Blood Pressure
 *         Measurement if collector has configured the device to send
 *         Intermediate Cuff Pressure values.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined BPS)
/* --------------------------------------------- Global Definitions */
/** Current Blood Pressure Measurement Time Interval */
#define APPL_BP_MEASUREMENT_INTERVAL                 5U
#define APPL_BLOOD_PRESSURE_MEASUREMENT_LENGTH       19U
#define APPL_IM_BLOOD_PRESSURE_MEASUREMENT_LENGTH    19U
#define APPL_IM_BLOOD_PRESSURE_COUNT                 3U

#define APPL_BPS_DEFAULT_ADV_SWITCH_BACK_INTERVAL    5U
/**
 * Complete Advertisement Data Length used by
 * Blood Pressure Advertisement.
 * This typically is the same value as the
 * "appl_gap_adv_data.datalen" which is present in the
 * BPS specific "appl_bps_gap_config_params.c" file.
 */
#define APPL_BPS_ADV_DATA_LEN                        19U


/* --------------------------------------------- External Global Variables */
/**
 * External global varible defined in "appl_bps_gap_config_params.c" file
 * which corresponds to the Blood Pressure related ADV data.
 */
extern APPL_GAP_ADV_DATA appl_gap_adv_data;

/** External global variable related to the Current Connected Peer Address */
extern BT_DEVICE_ADDR g_bd_addr;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* Timer related Globals */
static BT_timer_handle timer_handle;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_bps_db_handle;

/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_blood_pressure_hndl;
static ATT_ATTR_HANDLE appl_im_blood_pressure_hndl;

/* Service data and states related Globals */
static UCHAR           im_bpm_enabled = BT_FALSE;
static UCHAR           bpm_enabled;
UCHAR                  timer_started;
static UINT16          appl_msrmt_intrvl;

/**
 * Holding a constant Template of Blood Pressure ADV Data
 * To be used by application after disconnection if needed
 * by the application(to reset back any modified ADV Data)
 */
DECL_CONST UCHAR appl_default_adv_data[APPL_BPS_ADV_DATA_LEN] =
{
    /**
     *  Flags:
     *      0x01: LE Limited Discoverable Mode
     *      0x02: LE General Discoverable Mode
     *      0x04: BR/EDR Not Supported
     *      0x08: Simultaneous LE and BR/EDR to Same Device
     *            Capable (Controller)
     *      0x10: Simultaneous LE and BR/EDR to Same Device
     *            Capable (Host)
     */
    0x02U, 0x01U,
    (BT_AD_FLAGS_LE_GENERAL_DISC_MODE | BT_AD_FLAGS_LE_BR_EDR_HOST),

    /**
     *  Service UUID List:
     *      Battery Service (0x180F)
     *      DeviceInformation Service (0x180A)
     *      Blood Pressure Service (0x1810)
     */
    0x07U, 0x03U, 0x0FU, 0x18U, 0x0AU, 0x18U, 0x10U, 0x18U,

    /**
     *  Shortened Device Name: Mt-BPS
     */
    0x07U, 0x08U, 'M', 't', '-', 'B', 'P', 'S'
};

/* Blood Pressure Measurement Format SFLOAT */
static UCHAR bp_obs_data[APPL_BLOOD_PRESSURE_MEASUREMENT_LENGTH] =
{
    /**
     *  1st Octet         - Flag
     *  2nd & 3rd Octet   - Systolic  - 120 mmHg corresponds to 0x0078
     *  4th & 5th Octet   - Diastolic -  80 mmHg corresponds to 0x0050
     *  6th & 7th Octet   - MAP       - 100 mmHg corresponds to 0x0064
     *  8th - 14th Octet  - Time Stamp
     *  15th & 16th Octet - Pulse Rate
     *  17th Octet        - User Id
     *  18th & 19th Octet - Measurement Status
     */
    0x1EU, 0x78U, 0x00U, 0x50U, 0x00U, 0x64U, 0x00U,
    0xDEU, 0x07U, 0x04U, 0x1DU, 0x06U, 0x00U, 0x1EU,
    0x50U, 0x00U, 0x00U, 0x00U, 0x00U
};

/* Intermediate Cuff Pressure Format SFLOAT */
static UCHAR intrm_bp_obs_data\
[APPL_IM_BLOOD_PRESSURE_MEASUREMENT_LENGTH * APPL_IM_BLOOD_PRESSURE_COUNT] =
{
    /**
     *  1st Octet         - Flag
     *  2nd & 3rd Octet   - Systolic  - 130 mmHg corresponds to 0x0082
     *  4th & 5th Octet   - Diastolic -  95 mmHg corresponds to 0x005F
     *  6th & 7th Octet   - MAP       -  80 mmHg corresponds to 0x0050
     *  8th - 14th Octet  - Time Stamp
     *  15th & 16th Octet - Pulse Rate
     *  17th Octet        - User Id
     *  18th & 19th Octet - Measurement Status
     */
    0x1EU, 0x82U, 0x00U, 0x5FU, 0x00U, 0x50U, 0x00U,
    0xDEU, 0x07U, 0x04U, 0x1DU, 0x06U, 0x00U, 0x01U,
    0x50U, 0x00U, 0x00U, 0x00U, 0x00U,

    /**
     *  1st Octet         - Flag
     *  2nd & 3rd Octet   - Systolic  - 126 mmHg corresponds to 0x007E
     *  4th & 5th Octet   - Diastolic -  90 mmHg corresponds to 0x005A
     *  6th & 7th Octet   - MAP       -  85 mmHg corresponds to 0x0055
     *  8th - 14th Octet  - Time Stamp
     *  15th & 16th Octet - Pulse Rate
     *  17th Octet        - User Id
     *  18th & 19th Octet - Measurement Status
     */
    0x1EU, 0x7EU, 0x00U, 0x5AU, 0x00U, 0x55U, 0x00U,
    0xDEU, 0x07U, 0x04U, 0x1DU, 0x06U, 0x00U, 0x0AU,
    0x50U, 0x00U, 0x00U, 0x00U, 0x00U,

    /**
     *  1st Octet         - Flag
     *  2nd & 3rd Octet   - Systolic  - 123 mmHg corresponds to 0x007B
     *  4th & 5th Octet   - Diastolic -  85 mmHg corresponds to 0x0055
     *  6th & 7th Octet   - MAP       -  90 mmHg corresponds to 0x005A
     *  8th - 14th Octet  - Time Stamp
     *  15th & 16th Octet - Pulse Rate
     *  17th Octet        - User Id
     *  18th & 19th Octet - Measurement Status
     */
    0x1EU, 0x7BU, 0x00U, 0x55U, 0x00U, 0x5AU, 0x00U,
    0xDEU, 0x07U, 0x04U, 0x1DU, 0x06U, 0x00U, 0x12U,
    0x50U, 0x00U, 0x00U, 0x00U, 0x00U

};

UCHAR appl_bps_feature_value[] = { 0x00U, 0x00U };

/* Variables used to know the current Target Address and Multi Bond Support */
DECL_STATIC UCHAR target_addr_set;
DECL_STATIC UCHAR appl_bps_multi_bond_set;
DECL_STATIC UCHAR appl_bps_update_body_movement_set;
DECL_STATIC UCHAR appl_bps_update_cuff_fit_Detection_set;
DECL_STATIC UCHAR appl_bps_update_irregular_pulse_set;
DECL_STATIC UCHAR appl_bps_update_pulse_rate_range_set;
DECL_STATIC UCHAR appl_bps_update_measurment_position_set;

/* --------------------------------------------- Functions */
void appl_bps_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_bps_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

    im_bpm_enabled          = BT_FALSE;
    bpm_enabled             = BT_FALSE;
    target_addr_set         = BT_FALSE;
    appl_bps_multi_bond_set = BT_TRUE;
    appl_bps_update_body_movement_set       = BT_FALSE;
    appl_bps_update_cuff_fit_Detection_set  = BT_FALSE;
    appl_bps_update_irregular_pulse_set     = BT_FALSE;
    appl_bps_update_pulse_rate_range_set    = BT_FALSE;
    appl_bps_update_measurment_position_set = BT_FALSE;

    /* Initialize the Measurement Interval */
    appl_msrmt_intrvl = APPL_BP_MEASUREMENT_INTERVAL;

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_bps_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_bps_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_bps_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_bps_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_bps_add_bps();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the BPS GATT DB Signature */
    appl_bps_db_sign = APPL_BPS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_bps_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[BPS]: GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for BP Measurement */
    appl_bps_db_handle.device_id   = DEVICE_HANDLE_INIT_VAL;
    appl_bps_db_handle.service_id  = (UCHAR)GATT_SER_BPS_BP_INST;
    appl_bps_db_handle.char_id     = (UCHAR)GATT_CHAR_BPS_BP_MSRMNT_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for BPS services if Dynamic GATT Database is not Used */
    appl_bps_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register BPS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_bps_gatt_char_handler);
}

void appl_bps_bt_init(void)
{
    APPL_TRC("[BPS]: In appl_bps_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_bps_server_reinitialize();
}

void appl_bps_bt_deinit(void)
{
    APPL_TRC("[BPS]: In appl_bps_bt_deinit\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_bps_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;
    API_RESULT        retval;

    cli_cnfg = 0U;
    /* Set the Public or random target address in the adv data */
    if (BT_TRUE == target_addr_set)
    {
        appl_bps_set_target_address_in_adv_data(APPL_BPS_ADV_DATA_LEN);
    }

    appl_bps_db_handle.device_id  = (*dq_handle);
    appl_bps_db_handle.char_id = (UCHAR)GATT_CHAR_BPS_BP_MSRMNT_INST;

    BT_gatt_db_get_char_val_hndl
    (
        &appl_bps_db_handle,
        &appl_blood_pressure_hndl
    );
    retval = BT_gatt_db_get_char_cli_cnfg (&appl_bps_db_handle,&value);

    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC (
    "[BPS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_bps_db_handle,cli_cnfg);
    }

    /* Check if intermediate cuff pressure is configured by the client */
    appl_bps_db_handle.char_id = (UCHAR)GATT_CHAR_BPS_INTRM_CUFF_PRSR_INST;
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_bps_db_handle,&value);
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE (&cli_cnfg,value.val);
    }
    if (0U != cli_cnfg)
    {
        im_bpm_enabled = BT_TRUE;
    }
    BT_gatt_db_get_char_val_hndl
    (
        &appl_bps_db_handle,
        &appl_im_blood_pressure_hndl
    );

    appl_bps_db_handle.char_id = (UCHAR)GATT_CHAR_BPS_BP_FEATURE_INST;

    retval = BT_gatt_db_get_char_val
             (
                 &appl_bps_db_handle,
                 &value
             );

    if (BT_TRUE == appl_bps_multi_bond_set)
    {
        /* Set Multiple Bond Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0020U);
         LOG_DEBUG("Muti_Bond Enable \n");
    }
    else
    {
        /* Reset Multiple Bond Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFDFU);
        LOG_DEBUG("Muti_Bond Disable \n");
    }

    if (BT_TRUE == appl_bps_update_body_movement_set)
    {
        /* Set body_movement Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0001U);
        LOG_DEBUG("body_movement Enable \n");
    }
    else
    {
        /* Reset body_movement Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFFEU);
        LOG_DEBUG("body_movement Disable \n");
    }

    if (BT_TRUE == appl_bps_update_cuff_fit_Detection_set )
    {
        /* Set cuff_fit_Detection Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0002U);
        LOG_DEBUG("cuff_fit_Detection Enable \n");
    }
    else
    {
        /* Reset cuff_fit_Detection  Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFFDU);
        LOG_DEBUG("cuff_fit_Detection Disable \n");
    }

    if (BT_TRUE == appl_bps_update_irregular_pulse_set)
    {
        /* Set irregular_pulse Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0004U);
        LOG_DEBUG("irregular_pulse Enable \n");
    }
    else
    {
        /* Reset irregular_pulse Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFFBU);
        LOG_DEBUG("irregular_pulse Disable \n");
    }

    if (BT_TRUE == appl_bps_update_pulse_rate_range_set)
    {
        /* Set pulse_rate_range Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0008U);
        LOG_DEBUG("pulse_rate_range Enable \n");
    }
    else
    {
        /* Reset pulse_rate_range Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFF7U);
        LOG_DEBUG("pulse_rate_range Disable \n");
    }

    if (BT_TRUE == appl_bps_update_measurment_position_set)
    {
        /* Set measurment_position Support bit in Blood pressure feature char */
        *value.val = ((*value.val) | 0x0010U);
        LOG_DEBUG("measurment_position Enable \n");
    }
    else
    {
        /* Reset measurment_position Support bit in Blood pressure feature char */
        *value.val = ((*value.val) & 0xFFEFU);
        LOG_DEBUG("measurment_position Disable \n");
    }

    retval = BT_gatt_db_set_char_val
             (
                &appl_bps_db_handle,
                &value
             );
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
        /* Security requirements satisfied, go ahead with data transfer */
        if (GATT_CHAR_BPS_BP_MSRMNT_INST == handle.char_id)
        {
            appl_get_handle_from_device_handle (handle.device_id,&appl_handle);

            if (GATT_CLI_CNFG_INDICATION == config)
            {
                /* Set the Flag for BP Measurements requested to True */
                bpm_enabled = BT_TRUE;

                if (timer_started != BT_TRUE)
                {
                    appl_bps_start_timer(&appl_handle, sizeof(appl_handle));
                }
            }
            else if (GATT_CLI_CNFG_DEFAULT == config)
            {
                bpm_enabled = BT_FALSE;

                if ((BT_FALSE == bpm_enabled) && (BT_FALSE == im_bpm_enabled))
                {
                   /* Stop transfer simulate Blood Pressure measurements
                    * only if both BPM and IM_CUFF are disabled
                    */
                    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                    {
                        BT_stop_timer (timer_handle);

                        APPL_TRC (
                        "[BPS]: Stopped Blood Pressure Measurement Timer %p\n",
                        timer_handle);

                        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    timer_started = BT_FALSE;
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else
        {
            appl_get_handle_from_device_handle(handle.device_id, &appl_handle);

            if (GATT_CHAR_BPS_INTRM_CUFF_PRSR_INST == handle.char_id)
            {
                if (GATT_CLI_CNFG_NOTIFICATION == config)
                {
                    im_bpm_enabled = BT_TRUE;

                    if (timer_started != BT_TRUE)
                    {
                        appl_bps_start_timer(&appl_handle, sizeof(appl_handle));
                    }
                }
                else if (GATT_CLI_CNFG_DEFAULT == config)
                {
                    im_bpm_enabled = BT_FALSE;

                    if ((BT_FALSE == bpm_enabled) &&
                        (BT_FALSE == im_bpm_enabled))
                    {
                        /* Stop transfer simulate Blood Pressure measurements
                         * only if both BPM and IM_CUFF are disabled
                         */
                        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                        {
                            BT_stop_timer (timer_handle);

                            APPL_ERR(
                            "[BPS]: **ERR** Blood Pressure Measurement Timer"
                            " %p Stopped\n", timer_handle);

                            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                        }

                        timer_started = BT_FALSE;
                    }
                }
                else
                {
                    APPL_ERR("[BPS]: **ERR** Not the right transfer type!");
                }
            }
            else
            {
                APPL_ERR("[BPS]: **ERR** Not the right transfer type!");
            }
        }
    }
}


void appl_bps_start_timer(APPL_HANDLE * hndl, UINT16 hndl_len)
{
    API_RESULT         retval = API_FAILURE;
    timer_started = BT_TRUE;

    /* Start timer to simulate Blood Pressure measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC( "[BPS]: Blood Pressure Measurement Timeout Occurred: %p\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_BP_MEASUREMENT_INTERVAL,
                 appl_timer_expiry_handler,
                 (void *) hndl,
                 hndl_len
             );

    APPL_TRC(
    "[BPS]: Blood Pressure Measurement Timer %p Started, result 0x%04X!\n",
    timer_handle, retval);
}

void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    APPL_EVENT_PARAM   fsm_param;

    /* Setting Timer Start Flag to False */
    timer_started = BT_FALSE;

    fsm_param.handle = (*((APPL_HANDLE *)data));

    appl_bps_start_timer(&fsm_param.handle, sizeof(fsm_param.handle));

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


void appl_bps_server_reinitialize (void)
{
    /* Stop Timer for Blood Pressure measurements */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    timer_started = BT_FALSE;

    if (BT_FALSE != target_addr_set)
    {
        target_addr_set = BT_FALSE;

        /**
         * Start Timer to set the default advertising data after
         * "APPL_BPS_DEFAULT_ADV_SWITCH_BACK_INTERVAL" seconds
         */
        BT_start_timer
        (
            &timer_handle,
            APPL_BPS_DEFAULT_ADV_SWITCH_BACK_INTERVAL,
            appl_bps_timer_expiry_hndlr,
            NULL,
            0U
        );

        APPL_TRC(
        "[APPL]: Started Timer %p\n", timer_handle);
    }

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv
        (
            APPL_GAP_PROC_NORMAL,
            HCI_ADV_IND,
            0x00U,
            0x00U,
            NULL,
            0x00U
        );

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

void appl_bps_timer_expiry_hndlr(void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

    /* Stop timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC(
        "[APPL]: Timeout Occurred: %p\n", timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    /* Reset to Default ADV Data */
    appl_reset_adv_data_to_default_value();
}

void appl_send_blood_pressure_measurement (APPL_HANDLE    * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 im_index;

    /**
     *  Transfer intermediate measurements before transferring the
     *  stable measurement
     */
    im_index = 0U;
    if (BT_FALSE != im_bpm_enabled)
    {
        do
        {
            hndl_val_param.handle =  appl_im_blood_pressure_hndl;
            hndl_val_param.value.val = \
                (&intrm_bp_obs_data[(APPL_BLOOD_PRESSURE_MEASUREMENT_LENGTH * im_index)]);
            hndl_val_param.value.len = APPL_BLOOD_PRESSURE_MEASUREMENT_LENGTH;
            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                     );
            im_index++;
        }while (im_index < APPL_IM_BLOOD_PRESSURE_COUNT);
    }

    if (BT_FALSE != bpm_enabled)
    {
        hndl_val_param.handle =  appl_blood_pressure_hndl;
        hndl_val_param.value.val = bp_obs_data;
        hndl_val_param.value.len = APPL_BLOOD_PRESSURE_MEASUREMENT_LENGTH;

        retval = BT_att_send_hndl_val_ind
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );

        if (API_SUCCESS != retval)
        {
            APPL_ERR(
            "[BPS]: **ERR** Failed to send measurement, reason 0x%04X\n",
            retval);
        }
    }
}

void appl_bps_handle_ind_complete
     (
         APPL_HANDLE* handle,
         UINT16      evt_result
     )
{
    CONSOLE_OUT(
    "\n[BPS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_bps_handle_ntf_complete
     (
         APPL_HANDLE* handle,
         UCHAR* event_data,
         UINT16      datalen
     )
{
    CONSOLE_OUT("\n[BPS]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
}

void appl_bps_handle_mtu_update_complete
     (
         APPL_HANDLE* handle,
         UINT16      mtu
     )
{
    CONSOLE_OUT("\n[BPS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

void appl_bps_set_target_address_in_adv_data(UCHAR offset)
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
        appl_gap_adv_data.data + offset + 2U,
        g_bd_addr.addr
    );

    /* Update the actual length of advertising data */
    appl_gap_adv_data.datalen = offset + BT_BD_ADDR_SIZE + 2U;
#endif /* ((defined APPL_GAP_BROADCASTER) || (defined APPL_GAP_PERIPHERAL)) */
}

void appl_reset_adv_data_to_default_value(void)
{
    /* Copy the Default Template ADV Data for next usage */
    BT_mem_copy
    (
        appl_gap_adv_data.data,
        appl_default_adv_data,
        sizeof(appl_default_adv_data)
    );

    appl_gap_adv_data.datalen = sizeof(appl_default_adv_data);
}

void appl_bps_update_multi_bond_flag(UCHAR flag)
{
    /* Set the Multi Bond Usage Flag */
    appl_bps_multi_bond_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
    LOG_DEBUG("appl_bps_multi_bond_set = %d \n",appl_bps_multi_bond_set);
}

void appl_bps_update_target_addr_flag(UCHAR flag)
{
    /* Set the Target Address Usage value */
    target_addr_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
    LOG_DEBUG("target_addr_set = %d\n",target_addr_set);
}
void appl_bps_update_body_movement_flag(UCHAR flag)
{
	appl_bps_update_body_movement_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
	LOG_DEBUG("appl_bps_update_body_movement_set = %d \n",appl_bps_update_body_movement_set);
}

void appl_bps_update_cuff_fit_Detection_flag(UCHAR flag)
{
	appl_bps_update_cuff_fit_Detection_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
	LOG_DEBUG("appl_bps_update_cuff_fit_Detection_set = %d \n",appl_bps_update_cuff_fit_Detection_set);
}

void appl_bps_update_irregular_pulse_flag(UCHAR flag)
{
	appl_bps_update_irregular_pulse_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
	LOG_DEBUG("appl_bps_update_irregular_pulse_set = %d \n",appl_bps_update_irregular_pulse_set);
}

void appl_bps_update_pulse_rate_range_flag(UCHAR flag)
{
	appl_bps_update_pulse_rate_range_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
	LOG_DEBUG("appl_bps_update_pulse_rate_range_set = %d \n",appl_bps_update_pulse_rate_range_set);
}

void appl_bps_update_measurment_position_flag(UCHAR flag)
{
	appl_bps_update_measurment_position_set = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;
	LOG_DEBUG("appl_bps_update_measurment_position_set = %d \n",appl_bps_update_measurment_position_set);
}

#endif /* BPS */

