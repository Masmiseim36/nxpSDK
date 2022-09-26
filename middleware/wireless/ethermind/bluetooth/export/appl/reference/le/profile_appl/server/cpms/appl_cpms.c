
/**
 *  \file appl_cpms.c
 *
 *  This file contains the Sample Profile application..
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"


#if (defined ATT && defined CPMS)

/* --------------------------------------------- External Global Variables */
extern UCHAR appl_hvc_flag;

#ifdef APPL_GAP_BROADCASTER_SUPPORT
extern const APPL_GAP_ADV_PARAM appl_gap_adv_param[APPL_GAP_PROC_TYPES];
extern BT_DEVICE_ADDR g_bd_addr;
#endif /* APPL_GAP_BROADCASTER_SUPPORT */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
UCHAR cpm_msrmt_data_default[APPL_CPM_MSRMNT_LENGTH];

UCHAR cpm_msrmt_set_cumu_data[4U];

UCHAR cpm_broadcast_data[APPL_CPM_BROADCAST_DATA_LENGTH] =
{
    /* Flags */
    0x02U, 0x01U, 0x04U,

    /* Advertising Interval */
    0x03U, 0x1AU, 0x02U, 0x00U,

    /* Service Data */
    0x07U, 0x16U, 0x18U, 0x18U, 0x00U, 0x00U, 0x32U, 0x00U,
};

static UCHAR cpm_msrmt_data[APPL_CPM_MSRMNT_LENGTH] =
{
    /* Flag */
    0xFFU, 0x1FU,

    /* Instantaneous Power */
    0x32U, 0x00U,

    /* Pedal Power Balance Present */
    0x00U,

    /* Accumulated Torque */
    0x00U, 0x00U,

    /* Wheel Revolution Data */
    /* Cumulative Wheel Revolutions */
    0x01U, 0x00U, 0x00U, 0x00U,

    /* Last Wheel Event Time */
    0x01U, 0x00U,

    /* Crank Revolution Data */
    /* Cumulative Crank Revolutions */
    0x00U, 0x00U,

    /* Last Crank Event Time */
    0x00U, 0x00U,

    /* Extreme Force Magnitudes */
    /* Maximum Force Magnitude */
    0x00U, 0x00U,

    /*  Minimum Force Magnitude */
    0x00U, 0x00U,

    /* Extreme Angles */
    0x12U, 0x34U, 0x56U,

    /* Top Dead Spot Angle */
    0x00U, 0x00U,

    /* Bottom Dead Spot Angle */
    0x00U, 0x00U,

    /* Accumulated Energy */
    0x00U, 0x00U
};

static UCHAR cpm_vector_data[APPL_CPM_VECTOR_DATA_LENGTH] =
{
    /* Flag */
    0x1FU,

    /* Crank Revolution Data */
    /* Cumulative Crank Revolutions */
    0x01U, 0x00U,

    /* Last Crank Event Time */
    0x01U, 0x00U,

    /* First Crank Measurement Angle  */
    0x00U, 0x00U,

    /* Instantaneous Force Magnitude Array */
    0x00U, 0x00U,

    /* Instantaneous Torque Magnitude Array */
    0x00U, 0x01U
};

static UCHAR cpm_supported_sensor_location[APPL_CPM_SUPPORTED_SENSOR_LOC_COUNT] =
{
    0x04U, 0x05U, 0x06U
};

static GATT_DB_HANDLE  appl_cpm_msrmt_db_handle;
static GATT_DB_HANDLE  appl_cpm_vector_db_handle;
static GATT_DB_HANDLE  appl_cpm_cp_db_handle;

static ATT_ATTR_HANDLE    appl_cpm_msrmt_hndl;
static ATT_ATTR_HANDLE    appl_cpm_vector_hndl;
static ATT_ATTR_HANDLE    appl_cpm_cp_hndl;
static UCHAR              appl_cpm_msrmt_cnfgd;
static UCHAR              appl_cpm_vector_cnfgd;
static UCHAR              appl_cpm_cp_cnfgd;
static BT_timer_handle    appl_cpm_msrmt_timer_hndl;
static BT_timer_handle    appl_cpm_vector_timer_hndl;
static BT_timer_handle    cntrl_pnt_timer_handle;
static UCHAR              appl_cpm_count;
static UCHAR              appl_cpm_count_comp_val;
static UCHAR              appl_cpm_msrmt_obs_data_len;

static UINT16             crank_len;
static UINT16             chain_len;
static UINT16             chain_weight;
static UINT16             span_len;
static UCHAR              sampling_rate;
static UCHAR              start_off_comp;
static UINT16             company_id = 0x006AU;
static UCHAR              no_of_manufr_data = 0x0aU;
static UCHAR              data[5U];
static UCHAR              cps_reverse;
UCHAR                     cmd_status;
UCHAR                     calib_date[10U] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
UCHAR                     incorrect_calbn_position;
UCHAR                     appl_broadcast_data_set;

/* --------------------------------------------- Functions */

void appl_cpms_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_cpms_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_cpms_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_cpms_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_cpms_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_cpms_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_cpms_add_cpms();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the CPMS GATT DB Signature */
    appl_cpms_db_sign = APPL_CPMS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_cpms_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
        "[CPMS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for CPM Measurement */
    appl_cpm_msrmt_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_cpm_msrmt_db_handle.service_id = (UCHAR)GATT_SER_CPMS_CPM_INST;
    appl_cpm_msrmt_db_handle.char_id    = (UCHAR)GATT_CHAR_CPMS_CPM_MSRMT_INST;

    /* Populate the GATT DB HANDLE for CPM Vector Measurement */
    appl_cpm_vector_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_cpm_vector_db_handle.service_id = (UCHAR)GATT_SER_CPMS_CPM_INST;
    appl_cpm_vector_db_handle.char_id    = (UCHAR)GATT_CHAR_CPMS_CPM_VECTOR_INST;

    /* Populate the GATT DB HANDLE for CPM Mea */
    appl_cpm_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_cpm_cp_db_handle.service_id = (UCHAR)GATT_SER_CPMS_CPM_INST;
    appl_cpm_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_CPMS_CPM_CNTRL_POINT_INST;

#ifndef GATT_DB_DYNAMIC
    appl_cpms_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register CPMS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_cpms_gatt_char_handler);

    appl_cpm_count_comp_val = 0U;
    appl_broadcast_data_set = BT_FALSE;

#ifdef BT_DUAL_MODE
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_CPMS_CPM_INST,
        DB_RECORD_CPS
    );
#endif

    /* Copy default cycling power measurement data */
    BT_mem_copy
    (
        cpm_msrmt_data_default,
        cpm_msrmt_data,
        APPL_CPM_MSRMNT_LENGTH
    );

    incorrect_calbn_position = BT_FALSE;

}

void appl_cpms_bt_init(void)
{
    APPL_TRC("[CPMS]: In appl_cpms_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_cpms_server_reinitialize();
}

void appl_cmps_bt_deinit(void)
{
    APPL_TRC("[CPMS]: In appl_cpms_bt_deinit\n");

     /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
    {
        BT_stop_timer (appl_cpm_msrmt_timer_hndl);
        appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_vector_timer_hndl)
    {
        BT_stop_timer(appl_cpm_vector_timer_hndl);
        appl_cpm_vector_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_pnt_timer_handle)
    {
        BT_stop_timer(cntrl_pnt_timer_handle);
        cntrl_pnt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_cpms_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    API_RESULT        retval;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;


    /** Register L2CAP */
    retval = BT_l2cap_register_le_event_cb(appl_cpms_l2cap_callback);

    appl_cpm_msrmt_db_handle.device_id = (*dq_handle);

    BT_gatt_db_get_char_val_hndl
    (
        &appl_cpm_msrmt_db_handle,
        &appl_cpm_msrmt_hndl
    );
    BT_gatt_db_get_char_cli_cnfg(&appl_cpm_msrmt_db_handle,&value);

    BT_UNPACK_LE_2_BYTE (&cli_cnfg,value.val);

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_cpm_msrmt_db_handle,cli_cnfg);
    }

    appl_cpm_vector_db_handle.device_id = (*dq_handle);

    BT_gatt_db_get_char_val_hndl
    (
        &appl_cpm_vector_db_handle,
        &appl_cpm_vector_hndl
    );

    appl_cpm_cp_db_handle.device_id = (*dq_handle);

    BT_gatt_db_get_char_val_hndl
    (
        &appl_cpm_cp_db_handle,
        &appl_cpm_cp_hndl
    );
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
        appl_get_handle_from_device_handle(handle.device_id,&appl_handle);

        if (GATT_CLI_CNFG_NOTIFICATION == config)
        {
            if (GATT_CHAR_CPMS_CPM_MSRMT_INST == handle.char_id)
            {
                /* Start Transfer Timer to Send measurement every second */
                if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
                {
                    BT_stop_timer (appl_cpm_msrmt_timer_hndl);
                    appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
                }

                appl_cpm_msrmt_cnfgd = BT_TRUE;

                retval = BT_start_timer
                         (
                             &appl_cpm_msrmt_timer_hndl,
                             APPL_CPM_MSRMT_INTERVAL,
                             appl_timer_expiry_handler,
                             &appl_handle,
                             sizeof (appl_handle)
                         );
                APPL_TRC (
                "[CPMS]: CPM Measuremet Timer %p Started, result 0x%04X!\n",
                appl_cpm_msrmt_timer_hndl, retval);
            }
            else if (GATT_CHAR_CPMS_CPM_VECTOR_INST == handle.char_id)
            {
                /* Send L2CAP connection parameter update request */
                retval = BT_l2cap_le_connection_param_update_request
                         (
                             &handle.device_id,
                             0x000AU,
                             0x000AU,
                             0x0000U,
                             0x03BBU
                         );
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else if (GATT_CLI_CNFG_DEFAULT == config)
        {
            /* Turn Off Transfer Timer if On */
            if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
            {
                BT_stop_timer (appl_cpm_msrmt_timer_hndl);
                appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
            }
            appl_cpm_msrmt_cnfgd = BT_FALSE;
        }
        else
        {
            /* Incorrect Configuration */
        }
    }
}

void appl_cpm_vector_timer_expiry_handler (void * t_data, UINT16 t_datalen)
{
    APPL_EVENT_PARAM   fsm_param;

    BT_IGNORE_UNUSED_PARAM(t_datalen);

    fsm_param.handle = (*((APPL_HANDLE *)t_data));
    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_vector_timer_hndl)
    {
        APPL_TRC (
        "[CPMS]: CPM Measurement Timeout Occurred: %p\n",
            appl_cpm_vector_timer_hndl);
        appl_cpm_vector_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    BT_start_timer
    (
        &appl_cpm_vector_timer_hndl,
        APPL_CPM_MSRMT_INTERVAL,
        appl_cpm_vector_timer_expiry_handler,
        &fsm_param.handle,
        sizeof(fsm_param.handle)
    );

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

void appl_timer_expiry_handler (void * t_data, UINT16 t_datalen)
{
    APPL_EVENT_PARAM   fsm_param;
    API_RESULT retval = API_SUCCESS;

    BT_IGNORE_UNUSED_PARAM(t_datalen);

    fsm_param.handle = (*((APPL_HANDLE *)t_data));
    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
    {
        APPL_TRC (
        "[CPMS]: CPM Measurement Timeout Occurred: %p\n",
        appl_cpm_msrmt_timer_hndl);

        appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
             (
                 &appl_cpm_msrmt_timer_hndl,
                 APPL_CPM_MSRMT_INTERVAL,
                 appl_timer_expiry_handler,
                 &fsm_param.handle,
                 sizeof(fsm_param.handle)
             );

    APPL_TRC(
    "[CPMS]: CPM Measurement Timer %p Started, result 0x%04X!\n",
    appl_cpm_msrmt_timer_hndl, retval);

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

void appl_cpms_server_reinitialize (void)
{
    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
    {
        BT_stop_timer (appl_cpm_msrmt_timer_hndl);
        appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_vector_timer_hndl)
    {
        BT_stop_timer (appl_cpm_vector_timer_hndl);
        appl_cpm_vector_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_pnt_timer_handle)
    {
        BT_stop_timer (cntrl_pnt_timer_handle);
        cntrl_pnt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    appl_cpm_msrmt_obs_data_len = APPL_CPM_MSRMNT_LENGTH;
    appl_cpm_count = 0U;
    appl_cpm_count_comp_val = 0U;
    appl_hvc_flag = BT_FALSE;

    /* Restore the cycling power measurement data if masked */
    if (0xFFU != cpm_msrmt_data[0U])
    {
        BT_mem_copy
        (
            cpm_msrmt_data,
            cpm_msrmt_data_default,
            APPL_CPM_MSRMNT_LENGTH
        );
    }

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
#ifdef APPL_GAP_BROADCASTER_SUPPORT
    if (BT_TRUE == appl_broadcast_data_set)
    {
        /* Stop broadcasting the CPS data */
        appl_service_enable_adv(0x00U);

        /** Set the adv data without the CPM data and
         *  Set advertising type as Connectable Undirected.
         *  Because during broadcasting the CPM data IUT will
         *  be sending Non Connectable advertising adv packets.
         */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, g_bd_addr.type, g_bd_addr.addr, 0x00U);

        /* Enable advertising */
        appl_service_enable_adv(0x01U);
    }
    else
#endif /* APPL_GAP_BROADCASTER_SUPPORT */
    {
        if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
        {
            /* Configure and Enable Advertising */
            appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
            appl_service_enable_adv(0x01U);
        }
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

void appl_send_cpms_measurement (APPL_HANDLE    * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UINT16 appl_cpm_inst_force_magn_arry;

    retval = API_SUCCESS;

    if (BT_FALSE != appl_cpm_msrmt_cnfgd)
    {
        APPL_TRC (
        "[CPMS]: [0x%02X]:Sending measurement On Handle 0x%04X\n",
        (*handle), appl_cpm_msrmt_hndl);

        hndl_val_param.handle = appl_cpm_msrmt_hndl;

        if (APPL_CPM_MSRMNT_LENGTH ==
            appl_cpm_msrmt_obs_data_len)
        {
            cpm_msrmt_data[0U] &= 0x3FU;
            cpm_msrmt_data[1U] &= 0x00U;
            hndl_val_param.value.val = cpm_msrmt_data;
            hndl_val_param.value.len = 17U;

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                     );

            if (API_SUCCESS == retval)
            {
                cpm_msrmt_data[13U] = 0x40U;
                cpm_msrmt_data[14U] = 0x1FU;
                cpm_msrmt_data[15U] = 0x32U;
                cpm_msrmt_data[16U] = 0x00U;
                hndl_val_param.value.val = &cpm_msrmt_data[13U];
                hndl_val_param.value.len = 17U;

                retval = BT_att_send_hndl_val_ntf
                         (
                             &APPL_GET_ATT_INSTANCE(*handle),
                             &hndl_val_param
                         );

                if ((API_SUCCESS == retval) && (BT_TRUE != cps_reverse))
                {
                    BT_mem_copy
                    (
                        cpm_msrmt_data,
                        cpm_msrmt_data_default,
                        APPL_CPM_MSRMNT_LENGTH
                    );
                }
            }
        }
        else
        {
            hndl_val_param.handle =  appl_cpm_msrmt_hndl;
            hndl_val_param.value.val = cpm_msrmt_data;
            hndl_val_param.value.len = appl_cpm_msrmt_obs_data_len;

            retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                 );
        }
    }
    else if (BT_FALSE != appl_cpm_vector_cnfgd)
    {
        APPL_TRC (
        "[CPMS]: [0x%02X]:Sending measurement On Handle 0x%04X\n",
        (*handle),appl_cpm_msrmt_hndl);

        /**
         * Send Cycling power vector by excluding Instantaneous Torque
         * Magnitude Array value
         */
        cpm_vector_data[0U] = 0x07U;
        hndl_val_param.handle =  appl_cpm_vector_hndl;
        hndl_val_param.value.val = cpm_vector_data;
        hndl_val_param.value.len = (APPL_CPM_VECTOR_DATA_LENGTH - 2U);

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                 );

        /* Copy the Instantaneous Force Magnitude Array value to the local varibale */
        BT_UNPACK_LE_2_BYTE(&appl_cpm_inst_force_magn_arry, &cpm_vector_data[7U]);

        if (API_SUCCESS == retval)
        {
            /**
             * Send Cycling power vector by excluding Instantaneous Force
             * Magnitude Array value
             */
            hndl_val_param.handle = appl_cpm_vector_hndl;
            cpm_vector_data[0U] = 0x1BU;
            cpm_vector_data[7U] = cpm_vector_data[9U];
            cpm_vector_data[8U] = cpm_vector_data[10U];
            hndl_val_param.value.val = cpm_vector_data;
            hndl_val_param.value.len = (APPL_CPM_VECTOR_DATA_LENGTH - 2U);

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(*handle),
                         &hndl_val_param
                     );

            /* Reset the Force Magnitude Array value */
            BT_PACK_LE_2_BYTE(&cpm_vector_data[7U], &appl_cpm_inst_force_magn_arry);
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }


    if (appl_cpm_count_comp_val == appl_cpm_count)
    {
        if(BT_TRUE == cps_reverse)
        {
            cpm_msrmt_data[0x07U]--;
            if (0xFFU == cpm_msrmt_data[0x07U])
            {
                cps_reverse = BT_FALSE;
                BT_mem_copy
                (
                    &cpm_msrmt_data[7U],
                    &cpm_msrmt_set_cumu_data[0U],
                    4U
                );
                cps_reverse = BT_TRUE;
            }
        }
        appl_cpm_count_comp_val++;
    }
    else if (2U == appl_cpm_count)
    {
        /* Simulate Cumulative Wheel and Crank Revolutions */
        if (0xFFU == cpm_msrmt_data[0x04U] && 0xFFU == cpm_vector_data[0x01U])
        {
            cpm_msrmt_data[0x05U]++;
            cpm_vector_data[0x02U]++;
        }
        cpm_msrmt_data[0x04U]++;
        cpm_vector_data[0x01U]++;

        /* Simulate Last Wheel and Crank Event Time */
        if (0x40U == cpm_msrmt_data[0x08U])
        {
            cpm_msrmt_data[0x05U] = 0U;
            cpm_vector_data[0x03U] = 0U;
        }
        cpm_msrmt_data[0x08U]++;
        cpm_vector_data[0x03U]++;

        appl_cpm_count = 0U;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    appl_cpm_count++;
    if (API_SUCCESS != retval)
    {
        APPL_ERR (
        "[CPMS]: **ERR** Failed to send measurement, reason 0x%04X",
        retval);
    }
}

void appl_cpm_cntrl_point_timer_handle (void * t_data, UINT16 t_datalen)
{
    /* Send Indication */
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    BT_IGNORE_UNUSED_PARAM(t_datalen);

    hndl_val_param.handle = appl_cpm_cp_hndl;
    hndl_val_param.value.val = ((APPL_CPM_CNTRL_POINT_RSP_PARAM *)t_data)->rsp_param.val;
    hndl_val_param.value.len = ((APPL_CPM_CNTRL_POINT_RSP_PARAM *)t_data)->rsp_param.len;

    if (APPL_CPM_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE ==
        hndl_val_param.value.val[1U])
    {
        hndl_val_param.value.len = 3U +
            APPL_CPM_SUPPORTED_SENSOR_LOC_COUNT;
        BT_mem_copy
        (
            (hndl_val_param.value.val + 3U),
            &cpm_supported_sensor_location,
            APPL_CPM_SUPPORTED_SENSOR_LOC_COUNT
        );
    }

    if (BT_TIMER_HANDLE_INIT_VAL != cntrl_pnt_timer_handle)
    {
        cntrl_pnt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(((APPL_CPM_CNTRL_POINT_RSP_PARAM *)t_data)->appl_handle),
                 &hndl_val_param
              );

    if (API_FAILURE != retval)
    {
        appl_hvc_flag = BT_TRUE;
    }
}

void appl_cpm_cntrl_point_cnfg_handler (GATT_DB_HANDLE  * handle, UINT16 config)
{
    BT_IGNORE_UNUSED_PARAM(handle);

    if (GATT_CLI_CNFG_INDICATION == config)
    {
        appl_cpm_cp_cnfgd = BT_TRUE;
    }
    else if (GATT_CLI_CNFG_DEFAULT == config)
    {
        appl_cpm_cp_cnfgd = BT_FALSE;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}

API_RESULT appl_cpm_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           )
{
    API_RESULT                        retval;
    APPL_CPM_CNTRL_POINT_RSP_PARAM    param;
    UINT16     temp_data;

    retval = API_SUCCESS;
    param.rsp_param.val = &data[0];
    param.rsp_param.len = 3;

    appl_get_handle_from_device_handle
    (
        handle->device_id,
        &param.appl_handle
    );

    if (BT_FALSE == appl_cpm_cp_cnfgd)
    {
        retval = ATT_CCD_IMPROPERLY_CONFIGURED | APPL_ERR_ID;
    }
    else if (BT_TRUE != appl_hvc_flag)
    {
        data[0] = APPL_CPM_RESPONSE_CODE;
        data[1] = value->val[0];
        data[2] = APPL_CPM_SUCCESS;

        if (BT_TRUE == APPL_CPM_SUPPORTED_OPCODE(value->val[0]))
        {
            switch(value->val[0])
            {
            case APPL_CPM_SET_CUMMULATIVE_VALUE_OPCODE:
                if (value->len >= (APPL_CPM_CUMMULATIVE_VALUE_LENGTH + 1))
                {
                    BT_mem_copy
                    (
                        cpm_msrmt_data+7,
                        &(value->val[1]),
                        APPL_CPM_CUMMULATIVE_VALUE_LENGTH
                    );
                    appl_cpm_count = 0;
                    if (0x05 == value->val[1])
                    {
                        cps_reverse = BT_TRUE;
                    }
                    data[2] = APPL_CPM_SUCCESS;
                    BT_mem_copy
                    (
                        &cpm_msrmt_set_cumu_data[0],
                        &cpm_msrmt_data[7],
                        4
                    );
                }
                else
                {
                    data[2] = APPL_CPM_INVALID_PARAMETER;
                }
                break;
            case APPL_CPM_UPDATE_SENSOR_LOCATION_OPCODE:
                {
                    if (value->len == 2)
                    {
                        GATT_DB_HANDLE   db_handle;
                        ATT_VALUE        att_value;
                        UCHAR            count;
                        count = 0;
                        data[2] = APPL_CPM_INVALID_PARAMETER;

                        while (count < APPL_CPM_SUPPORTED_SENSOR_LOC_COUNT)
                        {
                            if (value->val[1] == cpm_supported_sensor_location[count])
                            {
                                att_value.len = 1;
                                att_value.val = value->val+1;

                                db_handle = (*handle);
                                db_handle.char_id = (UCHAR) GATT_CHAR_CPMS_SNSR_LOC_INST;
                                BT_gatt_db_set_char_val (&db_handle,&att_value);
                                data[2] = APPL_CPM_SUCCESS;
                                break;
                            }
                            count ++;
                        }
                    }
                    else
                    {
                        data[2] = APPL_CPM_INVALID_PARAMETER;
                    }
                }
                break;
            case APPL_CPM_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE:
                if (value->len != 1)
                {
                    data[2] = APPL_CPM_INVALID_PARAMETER;
                }
                break;
            case APPL_CPM_SET_CRANK_LENGTH_OPCODE:
                APPL_CPM_VALIDATE_AND_SET_CRANK_LEN_PARAM
                (
                    &value->val[1],
                    crank_len,
                    (value->len - 1),
                    data[2]
                );
                break;
            case APPL_CPM_REQ_CRANK_LENGTH_OPCODE:
                BT_PACK_LE_2_BYTE(&data[3],&crank_len);
                param.rsp_param.len = 5;
                break;
            case APPL_CPM_SET_CHAIN_LENGTH_OPCODE:
                APPL_CPM_VALIDATE_AND_SET_CHAIN_LEN_PARAM
                (
                    &value->val[1],
                    chain_len,
                    (value->len - 1),
                    data[2]
                );
                break;
            case APPL_CPM_REQ_CHAIN_LENGTH_OPCODE:
                BT_PACK_LE_2_BYTE(&data[3],&chain_len);
                param.rsp_param.len = 5;
                break;
            case APPL_CPM_SET_CHAIN_WEIGHT_OPCODE:
                APPL_CPM_VALIDATE_AND_SET_CHAIN_WEIGHT_PARAM
                (
                    &value->val[1],
                    chain_weight,
                    (value->len - 1),
                    data[2]
                );
                break;
            case APPL_CPM_REQ_CHAIN_WEIGHT_OPCODE:
                BT_PACK_LE_2_BYTE(&data[3],&chain_weight);
                param.rsp_param.len = 5;
                break;
            case APPL_CPM_SET_SPAN_LENGTH_OPCODE:
                APPL_CPM_VALIDATE_AND_SET_SPAN_LEN_PARAM
                (
                    &value->val[1],
                    span_len,
                    (value->len - 1),
                    data[2]
                )
                break;
            case APPL_CPM_REQ_SPAN_LENGTH_OPCODE:
                BT_PACK_LE_2_BYTE(&data[3],&span_len);
                param.rsp_param.len = 5;
                break;

            case APPL_CPM_REQ_FACTRY_CALIB_DATE_OPCODE:
                BT_mem_copy(&data[3], &calib_date[0], 7);
                param.rsp_param.len = 10;
                break;

            case APPL_CPM_REQ_SAMPLING_RATE_OPCODE:
                BT_PACK_LE_1_BYTE(&data[3],&sampling_rate);
                param.rsp_param.len = 4;
                break;

            case APPL_CPM_STRT_OFFSET_COMPNSTN_OPCODE:
                if (1 == value->len)
                {
                    BT_PACK_LE_2_BYTE(&data[3],&start_off_comp);
                    param.rsp_param.len = 5;
                }
                else
                {
                    data[2] = APPL_CPM_INVALID_PARAMETER;
                }
                break;

            case APPL_MASK_CPM_CHAR_CONTENT_OPCODE:
                if (value->len == 3)
                {
                    if (value->val[2] != 0x01)
                    {
                        data[2] = APPL_CPM_INVALID_PARAMETER;
                    }
                    else
                    {
                        for (temp_data = 0;
                         temp_data < APPL_NUM_OF_CPM_MSRMT_MASK_FIELD;
                         temp_data++)
                        {
                            if (0x01 == (value->val[1] & 0x01) || (value->val[2] & 0x01))
                            {
                                switch (temp_data)
                                {
                                case 0:
                                    if (0x01 == (cpm_msrmt_data[0] & 0x01))
                                    {
                                        cpm_msrmt_data[0] &= 0xFC;
                                        appl_cpm_msrmt_obs_data_len -= 1;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[4],
                                            &cpm_msrmt_data[5],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;
                                case 1:
                                    if (0x04 == (cpm_msrmt_data[0] & 0x04))
                                    {
                                        cpm_msrmt_data[0] &= 0xF3;
                                        appl_cpm_msrmt_obs_data_len -= 2;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[5],
                                            &cpm_msrmt_data[7],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 2:
                                    if (0x10 == (cpm_msrmt_data[0] & 0x10))
                                    {
                                        cpm_msrmt_data[0] &= 0xEF;
                                        appl_cpm_msrmt_obs_data_len -= 6;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[7],
                                            &cpm_msrmt_data[13],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 3:
                                    if (0x20 == (cpm_msrmt_data[0] & 0x20))
                                    {
                                        cpm_msrmt_data[0] &= 0xDF;
                                        appl_cpm_msrmt_obs_data_len -= 4;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[13],
                                            &cpm_msrmt_data[17],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 4:
                                    if (0x40 == (cpm_msrmt_data[0] & 0x40))
                                    {
                                        cpm_msrmt_data[0] &= 0x3F;
                                        appl_cpm_msrmt_obs_data_len -= 4;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[17],
                                            &cpm_msrmt_data[21],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                 case 5:
                                    if (0x01 == (cpm_msrmt_data[1] & 0x01))
                                    {
                                        cpm_msrmt_data[1] &= 0x1E;
                                        appl_cpm_msrmt_obs_data_len -= 3;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[21],
                                            &cpm_msrmt_data[24],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 6:
                                    if (0x02 == (cpm_msrmt_data[1] & 0x02))
                                    {
                                        cpm_msrmt_data[1] &= 0x1D;
                                        appl_cpm_msrmt_obs_data_len -= 2;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[24],
                                            &cpm_msrmt_data[26],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 7:
                                    if (0x04 == (cpm_msrmt_data[1] & 0x04))
                                    {
                                        cpm_msrmt_data[1] &= 0x1B;
                                        appl_cpm_msrmt_obs_data_len -= 2;
                                        BT_mem_copy
                                        (
                                            &cpm_msrmt_data[26],
                                            &cpm_msrmt_data[28],
                                            appl_cpm_msrmt_obs_data_len
                                        );
                                    }
                                    break;

                                case 8:
                                    if (0x08 == (cpm_msrmt_data[1] & 0x08))
                                    {
                                        cpm_msrmt_data[1] &= 0x07;
                                        appl_cpm_msrmt_obs_data_len -= 2;
                                    }
                                    break;

                                default:
                                    /* CONSOLE_OUT("Invalid...\n"); */
                                    break;
                                }
                            }
                            value->val[1] = value->val[1] >> 1;
                        }
                    }
                }
                else
                {
                    data[2] = APPL_CPM_INVALID_PARAMETER;
                }
                break;
            case APPL_CPM_STRT_ENHCED_OFFSET_COMPNSTN_OPCODE:
                if (incorrect_calbn_position == BT_TRUE)
                {
                    data[2] = APPL_CPM_OPERATION_FAILED;
                    data[3] = APPL_CPM_INCORRECT_CALBN_POSITION;
                    param.rsp_param.len = 4;
                }
                else
                {
                    BT_PACK_LE_2_BYTE(&data[3], &start_off_comp);
                    BT_PACK_LE_2_BYTE(&data[5], &company_id);
                    BT_PACK_LE_1_BYTE(&data[7], &no_of_manufr_data);
                    param.rsp_param.len = 8;
                }
                break;

            default:
                data[2] = APPL_CPM_OPCODE_NOT_SUPPORTED;
                break;
            }

        }
        else
        {
            data[2] = APPL_CPM_OPCODE_NOT_SUPPORTED;
        }

        BT_start_timer
        (
             &cntrl_pnt_timer_handle,
             1,
             appl_cpm_cntrl_point_timer_handle,
             &param,
             sizeof (param)
         );
    }
    else
    {
        retval = ATT_PROCEDURE_ALREADY_IN_PROGRESS | APPL_ERR_ID;
    }

    return retval;
}

void appl_cpm_manage_brodcast (UINT16 config)
{
#ifdef APPL_GAP_BROADCASTER_SUPPORT
    if (GATT_SER_CNFG_BROADCAST == config)
    {
        appl_broadcast_data_set = BT_TRUE;

        /* Set the adv data to include CPMS broadcast data */
        BT_hci_le_set_advertising_data
        (
            APPL_CPM_BROADCAST_DATA_LENGTH,
            cpm_broadcast_data
        );

        /** The Advertising_Interval_Min and Advertising_Interval_Max shall
         *  not be set to less than 0x00A0 if adv type is undirected non connectable.
         */
        BT_hci_le_set_advertising_parameters
        (
            appl_gap_adv_param[APPL_GAP_PROC_SLOW].min_interval,
            appl_gap_adv_param[APPL_GAP_PROC_SLOW].max_interval,
            HCI_ADV_NONCONN_IND,
            0x00U,
            g_bd_addr.type,
            g_bd_addr.addr,
            appl_gap_adv_param[APPL_GAP_PROC_SLOW].channel_map,
            0x00U
        );

        /* Enable advertising */
        appl_service_enable_adv(0x01U);
    }
    else
    {
        /* Disable advertising when SCCD is disabled */
        appl_broadcast_data_set = BT_FALSE;

        /* Disable Advertising */
        appl_service_enable_adv(0x00U);

        /* Set the adv type as Connectable undirected */
        BT_hci_le_set_advertising_parameters
        (
            appl_gap_adv_param[APPL_GAP_PROC_NORMAL].min_interval,
            appl_gap_adv_param[APPL_GAP_PROC_NORMAL].max_interval,
            HCI_ADV_IND,
            0x00U,
            g_bd_addr.type,
            g_bd_addr.addr,
            appl_gap_adv_param[APPL_GAP_PROC_SLOW].channel_map,
            0x00U
        );
    }
#else /* APPL_GAP_BROADCASTER_SUPPORT */
    CONSOLE_OUT("\n[CPMS]: APPL_GAP_BROADCASTER_SUPPORT not Enabled!\n");
#endif /* APPL_GAP_BROADCASTER_SUPPORT */
}

void appl_cpms_l2cap_callback
     (
         DEVICE_HANDLE * handle,
         UCHAR           event_type,
         UCHAR         * event_data,
         UINT16          event_datalen
     )
{
    UINT16 length;
    UINT16 result;
    UCHAR   * bd_addr;
    UCHAR     bd_addr_type;
    BT_DEVICE_ADDR t_peer_bd_addr;

    BT_IGNORE_UNUSED_PARAM(event_datalen);

    device_queue_get_remote_addr (handle, &t_peer_bd_addr);
    bd_addr = BT_BD_ADDR(&t_peer_bd_addr);
    bd_addr_type = BT_BD_ADDR_TYPE (&t_peer_bd_addr);

    if (L2CAP_CONNECTION_UPDATE_RESPONSE_EVENT == event_type)
    {
        appl_unpack_2_byte_param(length, &event_data[0U]);
        appl_unpack_2_byte_param(result, &event_data[2U]);

#ifdef APPL_MENU_OPS
        CONSOLE_OUT("[CPMS]: L2CAP Connection Update Response. Result: 0x%04X\n",
            result);
#endif /* APPL_MENU_OPS */

        APPL_TRC (
        "Received : L2CAP_CONNECTION_UPDATE\n");
        APPL_TRC (
        "\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",bd_addr[0U],
        bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        APPL_TRC (
        "\tbd_addr_type : %02X\n", bd_addr_type);
        APPL_TRC (
        "\tLength       : %04X\n", length);
        APPL_TRC (
        "\tResult       : %04X\n", result);

        appl_manage_cpm_vector_msrmnt(handle, result);
    }
}

void appl_manage_cpm_vector_msrmnt (DEVICE_HANDLE * handle, UINT16 result)
{
    API_RESULT             retval;
    APPL_HANDLE            appl_handle;
    ATT_ERROR_RSP_PARAM    err_param;
    ATT_VALUE              value;
    UINT16                 cli_cnfg;

    appl_get_handle_from_device_handle(*handle,&appl_handle);

    /* If success send write response and start timer for sending notifications */
    if (0U == result)
    {
        value.val = data;
        cli_cnfg = GATT_CLI_CNFG_NOTIFICATION;
        BT_PACK_LE_2_BYTE(value.val, &cli_cnfg);
        value.len = 2U;

        /* Set CCCD for CPM Vector*/
        retval = gatt_db_access_val_by_db_handle
                 (
                     &appl_cpm_vector_db_handle,
                     &value,
                     GATT_CLIENT_CONFIG,
                     GATT_DB_UPDATE
                 );

        /* Start Transfer Timer to Send measurement every second */
        if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
        {
            BT_stop_timer (appl_cpm_msrmt_timer_hndl);
            appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
        }

        appl_cpm_vector_cnfgd = BT_TRUE;

        retval = BT_start_timer
                 (
                     &appl_cpm_vector_timer_hndl,
                     APPL_CPM_VECTOR_INTERVAL,
                     appl_timer_expiry_handler,
                     &appl_handle,
                     sizeof (appl_handle)
                 );

        APPL_TRC (
        "[CPMS]: CPM Vector Interval Timer %p Started, result 0x%04X!\n",
        appl_cpm_vector_timer_hndl, retval);

        /* Send Write Response */
        retval = BT_att_write_rsp
                 (
                     &APPL_GET_ATT_INSTANCE(appl_handle)
                 );
    }
    /* Else send error response with reason set to Application error code */
    else
    {
        err_param.error_code = APPL_CPM_INAPPROPRIATE_CONNECTION_PARAMETERS;
        /* CCCD handle = Value handle + 1 */
        err_param.handle = appl_cpm_vector_hndl + 1U;
        err_param.op_code = ATT_WRITE_REQ;

        retval = BT_att_send_error_rsp
                 (
                     &APPL_GET_ATT_INSTANCE(appl_handle),
                     &err_param
                 );
    }
}

void appl_cpms_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    APPL_TRC("\n[CPMS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);

    if (ATT_RESPONSE_TIMED_OUT == evt_result)
    {
        /**
         * If a Handle Value Confirmation has not been sent for an Indication
         * by the remote device, Suspend all Transactions.
         */
        /* Suspend all Transfer related timers */
        if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_msrmt_timer_hndl)
        {
            BT_stop_timer (appl_cpm_msrmt_timer_hndl);
            appl_cpm_msrmt_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
        }

        if (BT_TIMER_HANDLE_INIT_VAL != appl_cpm_vector_timer_hndl)
        {
            BT_stop_timer (appl_cpm_vector_timer_hndl);
            appl_cpm_vector_timer_hndl = BT_TIMER_HANDLE_INIT_VAL;
        }

        if (BT_TIMER_HANDLE_INIT_VAL != cntrl_pnt_timer_handle)
        {
            BT_stop_timer (cntrl_pnt_timer_handle);
            cntrl_pnt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
}

void appl_cpms_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_CPMS_HAVE_DATA_DUMP
    APPL_TRC("\n[CPMS]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_CPMS_HAVE_DATA_DUMP */
}

void appl_cpms_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[CPMS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}
#endif /* (defined ATT && defined CPMS) */

