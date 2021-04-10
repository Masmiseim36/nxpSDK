
/**
 *  \file gatt_db_pl.c
 *
 *  This file contains source code for the platform specific portions of
 *  GATT Database.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* -------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "gatt_db.h"
#include "gatt_db_pl.h"

#ifdef RACP_SUPPORT
#include "racp_fsm_defines.h"
#include "racp_fsm.h"
#include "racp.h"
#endif /* RACP_SUPPORT */

#ifdef GATT_DB

#define GL_CONFIGURED_STATE                  0x07

#define GATT_GLS_MSRMT_BIT_SET_MASK          0x01
#define GATT_GLS_CNTXT_MSRMT_BIT_SET_MASK    0x02
#define GATT_GLS_RACP_BIT_SET_MASK           0x04
#define GATT_GLS_MSRMT_BIT_RESET_MASK        0xFE
#define GATT_GLS_CNTXT_MSRMT_BIT_RESET_MASK  0xFD
#define GATT_GLS_RACP_BIT_RESET_MASK         0xFB

/* CGM Related defines */
#define CGM_CONFIGURED_STATE                 0x03

#define GATT_CGMS_MSRMT_BIT_SET_MASK         0x01
#define GATT_CGMS_RACP_BIT_SET_MASK          0x02
#define GATT_CGMS_MSRMT_BIT_RESET_MASK       0xFE
#define GATT_CGMS_RACP_BIT_RESET_MASK        0xFD


/* POS Related defines */
#define PLX_CONFIGURED_STATE                   0x03

#define GATT_PLX_SPOT_CHK_MSRMT_BIT_SET_MASK   0x01
#define GATT_PLX_RACP_BIT_SET_MASK             0x02
#define GATT_PLX_SPOT_CHK_MSRMT_BIT_RESET_MASK 0xFE
#define GATT_PLX_RACP_BIT_RESET_MASK           0xFD

#ifndef GATT_DB_PROFILE_HANDLER
#define GATT_DB_PROFILE_HANDLER            NULL
#endif /* GATT_DB_PROFILE_HANDLER */

UCHAR cgm_flag;

#define GATT_HPS_CONFIG_URI_SET_MASK              0x01
#define GATT_HPS_CONFIG_HTTP_HEADER_SET_MASK      0x02
#define GATT_HPS_CONFIH_HTTP_ENTITY_BODY_SET_MASK 0x04

UCHAR hps_config_state;

/* -------------------------------------- Exported Global Variables */

API_RESULT gatt_db_service_change_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(params);

    return API_SUCCESS;
}

API_RESULT gatt_db_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
#if 0
    /*fix build warning: declared but never used.*/
    UINT16 config;
#endif

    retval = API_SUCCESS;

#if 0
    /**
     *  Note: Adding a generic handler for Battery Level Notification.
     *  Incase the profile does not support Battery Service. Please add a dummy
     *  GATT_CHAR_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle,config);
    }
#endif
    return retval;
}


#ifdef AIOS
API_RESULT gatt_db_aios_digital_io_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;
    UINT16 uuid_val;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_DIGITAL_OUTPUT_INST == handle->char_id)
        {
            retval = appl_digital_output_signal_handler (handle,&params->value);
        }
    case GATT_DB_CHAR_PEER_HLD_DESC_WRITE_REQ:
        if ((GATT_CHAR_DIGITAL_INPUT_INST == handle->char_id) || (GATT_CHAR_ANALOG_INPUT_INST == handle->char_id))
        {
            BT_UNPACK_LE_2_BYTE (&uuid_val,GATT_DB_GET_UUID(params->handle))
            retval = appl_analog_and_digital_input_signal_handler (handle, &params->value,uuid_val);
        }
        break;
    case GATT_DB_CHAR_PEER_HLD_DESC_READ_REQ:
        BT_UNPACK_LE_2_BYTE(&uuid_val, GATT_DB_GET_UUID(params->handle))
        retval = appl_aios_read_req_handler(handle, uuid_val);
        break;
    default:
        break;
    }
    return retval;

}
#endif /* AIOS */

#ifdef POS
API_RESULT gatt_db_pulse_ox_msrmt_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    static UCHAR     pos_config_state;
    API_RESULT       retval;
    UINT16           config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_PLX_SPOT_CHECK_MSRMNT_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                pos_config_state &= GATT_PLX_SPOT_CHK_MSRMT_BIT_RESET_MASK;
            }
            else
            {
                pos_config_state |= GATT_PLX_SPOT_CHK_MSRMT_BIT_SET_MASK;
                appl_manage_trasnfer ((*handle),config);
            }
        }
        else if(GATT_CHAR_RACP_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                pos_config_state &= GATT_PLX_RACP_BIT_RESET_MASK;
            }
            else
            {
                pos_config_state |= GATT_PLX_RACP_BIT_SET_MASK;
            }
        }
        else if (GATT_CHAR_PLX_CONTINUS_MSRMNT_INST == handle->char_id)
        {
            appl_manage_trasnfer ((*handle),config);
        }
        else
        {
            /* Not a valid request */
        }
    }
    break;

    case GATT_DB_CHAR_PEER_WRITE_REQ:
    {
        if (GATT_CHAR_RACP_INST == handle->char_id)
        {
            if (PLX_CONFIGURED_STATE  == pos_config_state)
            {
                retval = appl_pos_racp_write_handler (handle,&params->value);
            }
            else
            {
                retval = ATT_CCD_IMPROPERLY_CONFIGURED | APPL_ERR_ID ;
            }
        }
    }
    break;
    default:
        break;
    }
    return retval;
}
#endif /* POS */

#ifdef HID
API_RESULT gatt_db_hid_event_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;

    case GATT_DB_CHAR_PEER_WRITE_CMD:
    {
        retval = appl_manage_wwr (handle, params);
    }
    break;

    default:
        break;
    }
    return retval;
}
#endif /* HID */

#ifdef HTS
API_RESULT gatt_db_temperature_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_validate_msrmt_interval(handle,config);
    }
    break;
    default:
        break;
    }
    return retval;
}
#endif /* HTS */

#ifdef CSCS
API_RESULT gatt_db_cscs_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_SC_CNTRL_PNT_INST == handle->char_id)
        {
            appl_csc_cntrl_point_cnfg_handler (handle,config);
        }
        else
        {
           appl_manage_trasnfer ((*handle),config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_SC_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_csc_control_point_handler (handle,&params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* CSCS */

#ifdef RSCS
API_RESULT gatt_db_rscs_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_SC_CNTRL_PNT_INST == handle->char_id)
        {
            appl_rsc_cntrl_point_cnfg_handler (handle,config);
        }
        else
        {
           appl_manage_trasnfer ((*handle),config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_SC_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_rsc_control_point_handler (handle,&params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* RSCS */

#ifdef VSE1
API_RESULT gatt_db_vse1_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    default:
        break;
    }
    return retval;
}
#endif /* VSE1 */

#ifdef VSE2
API_RESULT gatt_db_vse2_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
    {
        appl_rx_data_handler ((*handle),&params->value);
    }
    break;
    default:
        break;
    }
    return API_SUCCESS;
}
#endif /* VSE2 */

#ifdef PASS
API_RESULT gatt_db_pass_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    if (GATT_CHAR_RING_CTRL_PNT_INST == handle->char_id)
    {
        if (GATT_DB_CHAR_PEER_WRITE_CMD == params->db_op)
        {
            appl_pass_ringer_control_point_write_handler (handle, &params->value);
        }
    }
    else if ((GATT_CHAR_ALRT_STATUS_INST == handle->char_id) ||
             (GATT_CHAR_RING_SET_INST == handle->char_id))
    {
        switch (params->db_op)
        {
            case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
                {
                    BT_UNPACK_LE_2_BYTE (&config,params->value.val);
                    appl_start_trasnfer ((*handle),config);
                }
                break;
            case GATT_DB_CHAR_PEER_READ_REQ:
                {
                    appl_pass_read_request_handler (handle);
                }
                break;
            default:
                break;
        }
    }
    return retval;
}
#endif /* PASS */

#ifdef HPS
void gatt_db_hps_config_state_reset(void)
{
    if (0 != hps_config_state)
    {
        hps_config_state = 0;
    }
}

API_RESULT gatt_db_hps_handler
(
    GATT_DB_HANDLE    * handle,
    GATT_DB_PARAMS    * params
)
{
    API_RESULT retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE(&config, params->value.val);
        if (GATT_CHAR_HTTP_STATUS_CODE_INST == handle->char_id)
        {
            appl_hsc_cnfg_handler(config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        switch (handle->char_id)
        {
        case GATT_CHAR_HTTP_URI_INST:
            hps_config_state |= (GATT_HPS_CONFIG_URI_SET_MASK & 0xFF);
            break;
        case GATT_CHAR_HTTP_HEADERS_INST:
            hps_config_state |= (GATT_HPS_CONFIG_HTTP_HEADER_SET_MASK & 0xFF);
            break;
        case GATT_CHAR_HTTP_ENTITY_BODY_INST:
            hps_config_state |= (GATT_HPS_CONFIH_HTTP_ENTITY_BODY_SET_MASK & 0xFF);
            break;
        case GATT_CHAR_HTTP_CONTROL_POINT_INST:
            retval = appl_hps_cp_handler(handle, &params->value, hps_config_state);
            break;
        default:
            break;
        }
        break;
    case GATT_DB_CHAR_PEER_READ_REQ:
        if ((GATT_CHAR_HTTP_HEADERS_INST == handle->char_id) ||
            (GATT_CHAR_HTTP_ENTITY_BODY_INST == handle->char_id))
        {
            retval = GATT_DB_DELAYED_RESPONSE;
        }
        break;
    default:
        break;
    }

    return retval;
}
#endif /* HPS */

#ifdef HRS
API_RESULT gatt_db_hrs_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_HR_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_hr_control_point_handler (handle,&params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* HRS */

#ifdef GLS
API_RESULT gatt_db_glucose_meter_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    static UCHAR     gl_config_state;
    UINT16           config;
    API_RESULT       retval;
#ifdef RACP_SUPPORT
    RACP_EVENT_INFO  racp_event_info;
#endif /* RACP_SUPPORT */
    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_GL_MSRMNT_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                gl_config_state &= GATT_GLS_MSRMT_BIT_RESET_MASK;
            }
            else
            {
                gl_config_state |= GATT_GLS_MSRMT_BIT_SET_MASK;
            }
        }
        else if (GATT_CHAR_GL_CNTXT_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                gl_config_state &= GATT_GLS_CNTXT_MSRMT_BIT_RESET_MASK;
            }
            else
            {
                gl_config_state |= GATT_GLS_CNTXT_MSRMT_BIT_SET_MASK;
            }
        }
        else if (GATT_CHAR_GL_RACP_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                gl_config_state &= GATT_GLS_RACP_BIT_RESET_MASK;
            }
            else
            {
                gl_config_state |= GATT_GLS_RACP_BIT_SET_MASK;
            }
        }
        else
        {
            /* Not a valid request */
        }
#ifdef RACP_SUPPORT
        if (GL_CONFIGURED_STATE == gl_config_state)
        {
            racp_event_info.event_type = ev_racp_peer_config_complete;
        }
        else
        {
            racp_event_info.event_type = ev_racp_peer_config_reset;
        }
#endif /* RACP_SUPPORT */
        break;
    }
    case GATT_DB_CHAR_PEER_WRITE_REQ:

        /* Verify if security requirements are available with the link */
        retval = appl_smp_assert_security
                 (
                     &handle->device_id,
                     GATT_DB_SER_SECURITY_MODE1 | GATT_DB_SER_SECURITY_LEVEL1,
                     GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE
                 );

        /* Security requirements satisfied? */
        if (API_SUCCESS != retval)
        {
            retval = GATT_DB_INSUFFICIENT_SECURITY;
            return retval;
        }
        /* Security requirements satisfied, go ahead with data transfer */
#ifdef RACP_SUPPORT
        /* Update the RACP Event information */
        racp_update_event_info
        (
            racp_event_info,
            GL_DEVICE_OPCODE,
            RACP_PEER_INTIATED_REQ,
            params,
            handle
        )

        /* Parse the opcode and post appropriate event */
        retval = racp_opcode_parser
                 (
                     &racp_event_info
                 );
#else
        if (GL_CONFIGURED_STATE == gl_config_state)
        {
            retval = appl_gl_racp_write_handler (handle,&params->value);
        }
        else
        {
            retval = ATT_APPL_CCD_IMPROPERLY_CONFIGURED | APPL_ERR_ID ;
        }
#endif /* RACP_SUPPORT */
        break;
    default:
        break;
    }

#ifdef RACP_FSM_SUPPORT
    retval = racp_fsm_post_event
             (
                 RACP_FSM_ID,
                 racp_event_info.event_type,
                 &racp_event_info
             );
#endif /* RACP_FSM_SUPPORT */
    return retval;
}
#endif /* GLS */

#ifdef PXR
API_RESULT gatt_db_pxr_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT   retval;

    retval = API_SUCCESS;

    switch(params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
    case GATT_DB_CHAR_PEER_WRITE_CMD:
        retval = appl_pxr_set_alert_level (handle, &params->value);
        break;

    case GATT_DB_CHAR_PEER_READ_REQ:
        if (GATT_CHAR_TX_POWER_INST == handle->char_id)
        {
            retval = appl_pxr_get_tx_power_level (handle, &params->value);
        }
        break;

    default:
        break;
    }

    return retval;
}
#endif /* PXR */

#ifdef FMT
API_RESULT gatt_db_fmt_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT   retval;

    retval = API_SUCCESS;

    switch(params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        retval = appl_fmt_set_alert_level (handle, &params->value);
        break;

    default:
        break;
    }

    return retval;
}
#endif /* FMT */

#ifdef BPS
API_RESULT gatt_db_bps_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;
    retval = API_SUCCESS;
    switch (params->db_op)
    {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            BT_UNPACK_LE_2_BYTE (&config,params->value.val);
            appl_manage_trasnfer ((*handle),config);
        }
        break;
        default:
            break;
    }
    return retval;
}
#endif /* BPS */

#ifdef CTS
API_RESULT gatt_db_cts_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_CUR_TIME_INST == handle->char_id)
        {
            retval = appl_update_current_time (&params->value);
        }
        else if (GATT_CHAR_LCL_TIME_INFO_INST == handle->char_id)
        {
            retval = appl_update_local_time (&params->value);
        }
        else
        {
            /* Do Nothing */
        }
    break;

    case GATT_DB_CHAR_PEER_WRITE_CMD:
    {
        retval = appl_rtus_config_time_update (handle, &params->value);
    }
    break;

    case GATT_DB_CHAR_PEER_READ_REQ:
    if (GATT_CHAR_TIME_UPDATE_CP_INST == handle->char_id)
    {
        retval = appl_rtus_get_time_update_state (handle, &params->value);
    }
    else
    {
        retval = appl_cts_get_current_time (handle, &params->value);
    }
    break;
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    default:
        break;
    }
    return retval;
}
#endif /* CTS */

#ifdef RTUS
API_RESULT gatt_db_rtus_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_CMD:
    {
        retval = appl_rtus_config_time_update (handle, &params->value);
    }
    break;

    case GATT_DB_CHAR_PEER_READ_REQ:
        retval = appl_rtus_get_time_update_state (handle, &params->value);
        break;

    default:
        break;
    }

    return retval;
}
#endif /* RTUS */

#ifdef NDCS
API_RESULT gatt_db_ndcs_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_READ_REQ:
        retval = appl_ndcs_get_current_time (handle, &params->value);
        break;

    default:
        break;
    }
    return retval;
}
#endif /* NDCS */

#ifdef ANS
API_RESULT gatt_db_ans_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;
    UCHAR      command_id;
    UCHAR      category_id;

    retval = API_SUCCESS;

    if (GATT_CHAR_ALRT_NTF_CP_INST == handle->char_id)
    {
        switch (params->db_op)
        {
            case GATT_DB_CHAR_PEER_WRITE_REQ:
                BT_UNPACK_LE_1_BYTE (&command_id,params->value.val);
                BT_UNPACK_LE_1_BYTE (&category_id,params->value.val+1);
                retval = appl_alrt_ntf_cntrl_point((*handle), command_id, category_id);
                break;

            default:
                break;
        }
    }
    else if ((GATT_CHAR_NEW_ALRT_INST == handle->char_id) ||
          (GATT_UNREAD_ALRT_STATUS_INST == handle->char_id))
    {
        switch (params->db_op)
        {
            case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
                BT_UNPACK_LE_2_BYTE (&config,params->value.val);
                appl_manage_trasnfer ((*handle), config);
                break;

            default:
                break;
        }
    }
    return retval;
}
#endif /* ANS */

#ifdef CGMS
API_RESULT gatt_db_cgms_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    static UCHAR cgm_config_state;
    UINT16       config;
    API_RESULT   retval;
    UCHAR racp_event;
#ifdef RACP_SUPPORT
    RACP_EVENT_INFO racp_event_info;
#endif /* RACP_SUPPORT */
    retval = API_SUCCESS;
    racp_event =  BT_TRUE;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_CGM_MSRMT_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                cgm_config_state &= GATT_CGMS_MSRMT_BIT_RESET_MASK;
                cgm_flag = BT_FALSE;
            }
            else
            {
                cgm_config_state |= GATT_CGMS_MSRMT_BIT_SET_MASK;
                cgm_flag = BT_TRUE;
            }
        }
        else if (GATT_CHAR_CGM_RACP_INST == handle->char_id)
        {
            if (GATT_CLI_CNFG_DEFAULT == config)
            {
                cgm_config_state &= GATT_CGMS_RACP_BIT_RESET_MASK;
            }
            else
            {
                cgm_config_state |= GATT_CGMS_RACP_BIT_SET_MASK;
            }
        }
        else if (GATT_CHAR_CGM_CTRL_PNT_INST == handle->char_id)
        {
            appl_cgm_cntrl_point_cnfg_handler (handle,config);
        }
        else if (GATT_CHAR_CGM_ASCP_INST == handle->char_id)
        {
            appl_cgm_ascp_cnfg_handler (handle,config);
        }
        else
        {
            /* Invalid Configuration */
        }
#ifdef RACP_SUPPORT
        if (CGM_CONFIGURED_STATE == cgm_config_state)
        {
            racp_event_info.event_type = ev_racp_peer_config_complete;
        }
        else
        {
            racp_event_info.event_type = ev_racp_peer_config_reset;
        }
#endif /* RACP_SUPPORT */

        break;
    }
    case GATT_DB_CHAR_PEER_WRITE_REQ:

        switch (handle->char_id)
        {
        case GATT_CHAR_CGM_SSN_STRT_TIME_INST:
            racp_event = BT_FALSE;
            retval = appl_cgm_set_ssn_start_time
                     (
                         handle,
                         &params->value
                     );
            break;
        case GATT_CHAR_CGM_ASCP_INST:
            racp_event = BT_FALSE;
            retval = appl_ascp_handler
                     (
                         handle,
                         &params->value
                     );
            break;
        case GATT_CHAR_CGM_CTRL_PNT_INST:
            racp_event = BT_FALSE;
            retval = appl_cgm_cp_handler
                     (
                         handle,
                         &params->value
                     );
            break;
        case GATT_CHAR_CGM_RACP_INST:
#ifdef RACP_SUPPORT
            /* Update the RACP Event information */
            racp_update_event_info
            (
                racp_event_info,
                CGM_DEVICE_OPCODE,
                RACP_PEER_INTIATED_REQ,
                params,
                handle
            )

            /* Parse the opcode and post appropriate event */
            retval = racp_opcode_parser
                     (
                         &racp_event_info
                     );
#endif /* RACP_SUPPORT */
            break;
        default:
            racp_event = BT_FALSE;
            break;
        }
        break;
    default:
        racp_event = BT_FALSE;
        break;
    }
#ifdef RACP_FSM_SUPPORT
    if (BT_FALSE != racp_event)
    {
        retval = racp_fsm_post_event
                 (
                     RACP_FSM_ID,
                     racp_event_info.event_type,
                     &racp_event_info
                 );
    }
#endif /* RACP_SUPPORT */
    return retval;
}
#endif /* CGMS */

#ifdef CPMS
API_RESULT gatt_db_cpms_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;
    UINT16     config;

    retval = API_SUCCESS;


    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_CPM_CNTRL_POINT_INST == handle->char_id)
        {
            appl_cpm_cntrl_point_cnfg_handler (handle,config);
        }
        else
        {
           appl_manage_trasnfer ((*handle),config);

           if ((GATT_CHAR_CPM_VECTOR_INST == handle->char_id) &&
               (GATT_CLI_CNFG_NOTIFICATION == config))
           {
               retval = GATT_DB_DELAYED_RESPONSE;
           }
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_CPM_CNTRL_POINT_INST == handle->char_id)
        {
            retval = appl_cpm_control_point_handler (handle,&params->value);
        }
        break;
    case GATT_DB_CHAR_PEER_SER_CNFG_WRITE_REQ:
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
#ifdef APPL_GAP_BROADCASTER_SUPPORT
        appl_cpm_manage_brodcast (config);
#endif /* APPL_GAP_BROADCASTER_SUPPORT */
        break;
    default:
        break;
    }
    return retval;
}
#endif /* CPMS */

#ifdef OTS
API_RESULT gatt_db_ots_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;
    UINT16     config;

    retval = API_SUCCESS;

    if (GATT_SER_OTS_INST != handle->service_id)
    {
        retval = API_FAILURE;
    }
    else
    {
    switch (params->db_op)
    {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            BT_UNPACK_LE_2_BYTE (&config,params->value.val);
            if (GATT_CHAR_OACP_INST == handle->char_id)
            {
                appl_oacp_cnfg_handler (handle,config);
            }
            else if (GATT_CHAR_OLCP_INST == handle->char_id)
            {
                appl_olcp_cnfg_handler (handle,config);
            }
            else if (GATT_CHAR_OBJ_CHNGD_INST == handle->char_id)
            {
                appl_ots_obj_changed_handler(handle, config);
            }
            else
            {
                /* Do Nothing */
            }
        }
            break;
        case GATT_DB_CHAR_PEER_WRITE_REQ:
            if (GATT_CHAR_OACP_INST == handle->char_id)
            {
                retval = appl_oacp_handler (handle,&params->value);
            }
            else if (GATT_CHAR_OLCP_INST == handle->char_id)
            {
                retval = appl_olcp_handler (handle, &params->value);
            }
            else if ((GATT_CHAR_OLF_INST_1 == handle->char_id) ||
                (GATT_CHAR_OLF_INST_2 == handle->char_id) ||
                (GATT_CHAR_OLF_INST_3 == handle->char_id))
            {
                retval = appl_obj_list_filter_handler (handle, params);
            }
            else
            {
                retval = appl_obj_metadata_handler(handle, params);
            }
            break;
        case GATT_DB_CHAR_PEER_READ_REQ:
            if ((GATT_CHAR_OLF_INST_1 == handle->char_id) ||
                (GATT_CHAR_OLF_INST_2 == handle->char_id) ||
                (GATT_CHAR_OLF_INST_3 == handle->char_id))
            {
                /* retval = appl_obj_list_filter_handler (handle, params); */
            }
            break;
        default:
            break;
    }
    }

    return retval;
}
#endif /* OTS */


#ifdef LNS
API_RESULT gatt_db_lns_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;
    UINT16     config;

    retval = API_SUCCESS;


    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_LN_CNTRL_POINT_INST == handle->char_id)
        {
            appl_ln_cntrl_point_cnfg_handler (handle,config);
        }
        else
        {
           appl_manage_trasnfer ((*handle),config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_LN_CNTRL_POINT_INST == handle->char_id)
        {
            retval = appl_ln_control_point_handler (handle,&params->value);
        }
        break;
    case GATT_DB_CHAR_PEER_READ_REQ:
        if (GATT_CHAR_POSTN_QLTY_INST == handle->char_id)
        {
            appl_mask_pos_quality_content();
        }
        break;
    default:
        break;
    }

    return retval;
}
#endif /* LNS */

#ifdef SPS
API_RESULT gatt_db_sps_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 config;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_scan_refresh_cnfg_handler (handle,config);
    }
    break;
    default:
        break;
    }
    return API_SUCCESS;
}
#endif /* SPS */

#ifdef WSS
API_RESULT gatt_db_wss_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        /**
         * Not checking for security w.r.t peer device here.
         * That should be depended on the Service Level Security
         * configured for WSS. Current Seucity Level requirement is
         * NONE.
         */
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_manage_trasnfer ((*handle),config);
    }
    break;
    /* Fall through */
    case GATT_DB_CHAR_PEER_READ_REQ:
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_USER_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_user_control_point_handler (handle,&params->value);
        }
        else
        {
            retval = appl_validate_user_data_access (handle,&params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* WSS */

#ifdef ESS
API_RESULT gatt_db_ess_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;
    UINT16     uuid;

    retval = API_SUCCESS;

    BT_UNPACK_LE_2_BYTE(&uuid,GATT_DB_GET_UUID(params->handle));

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        appl_ess_ccd_config_handlr ((*handle),config);
    }
    break;
    case GATT_DB_CHAR_PEER_HLD_DESC_WRITE_REQ:
    {
        if (GATT_ES_TRIGGER_SETTING_DESCRIPTOR == uuid)
        {
            retval = appl_ess_set_trgr_desc(params);
        }
        else if (GATT_ES_CONFIGURATION_DESCRIPTOR == uuid)
        {
            retval = appl_ess_set_config_desc(params);
        }
        else if (GATT_VALID_RANGE_DESCRIPTOR == uuid)
        {

        }
        else
        {
            /* Do Nothing */
        }
    }
    break;
    case GATT_DB_CHAR_PEER_USR_DESC_WRITE_REQ:
        break;
    default:
        break;
    }
    return retval;
}
#endif /* ESS */

#ifdef BMS
API_RESULT gatt_db_bms_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;


    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_BMCP_INST == handle->char_id)
        {
            retval = appl_bms_control_point_handler (handle,&params->value);
        }
        break;
    default:
        break;
    }

    return retval;
}
#endif /* BMS */

#ifdef IPS
API_RESULT gatt_db_ips_handler
           (
                GATT_DB_HANDLE    * handle,
                GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        retval = appl_ips_write_chars_handler(handle, &params->value);
    default:
        break;
    }
    return retval;

}
#endif /* IPS */


#ifdef TDS
API_RESULT gatt_db_tds_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_TDS_CONTROL_POINT_INST == handle->char_id)
        {
            appl_tds_cntrl_point_cnfg_handler (handle,config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_TDS_CONTROL_POINT_INST == handle->char_id)
        {
            retval = appl_tds_control_point_handler (handle,&params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* TDS */

#ifdef FTMS
API_RESULT gatt_db_ftms_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    UINT16     config;

    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        if (GATT_CHAR_CONTROL_POINT_INST == handle->char_id)
        {
            appl_ftm_cntrl_point_cnfg_handler (handle,config);
        }
        else
        {
            appl_manage_trasnfer ((*handle),config);
        }

    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_CONTROL_POINT_INST == handle->char_id)
        {
            retval = appl_ftm_control_point_handler (handle,&params->value);
        }
        else if (GATT_CHAR_USER_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_user_control_point_handler(handle, &params->value);
        }
        break;
    default:
        break;
    }
    return retval;
}
#endif /* FTMS */

API_RESULT gatt_char_handler (GATT_DB_HANDLE * handle,GATT_DB_PARAMS * param)
{
    API_RESULT retval = API_SUCCESS;
#if 0
    /*fix build warning: declared but never used.*/
    uint8_t         request_code;
    uint8_t         response_code;
#endif
    /**
     *  For calling common handlers before calling dedicated profile/service
     *  handler
     */
#if 0
    if (GATT_CHAR_BATTERY_LVL_INST == handle->char_id)
    {
        return gatt_db_battery_level_handler(handle,param);
    }
#endif

#if 0
    /* handle the upper layer services request */
    switch (params->db_op)
    {
        case GATT_DB_CHAR_PEER_WRITE_REQ:
        case GATT_DB_CHAR_PEER_WRITE_CMD:
            if (GATT_DB_CHAR_PEER_WRITE_CMD == params->db_op)
            {
                request_code = ATT_WRITE_CMD;
                response_code = 0;
            }
            else
            {
                request_code = ATT_WRITE_REQ;
                response_code = ATT_WRITE_RSP;
            }

            retval = appl_get_cxn_handle_from_device_handle(handle->device_id, &cxn_handle);
            if (API_SUCCESS == retval)
            {
                int ret = prvBTGattWriteRequest(cxn_handle, params->handle, params->value.val, params->value.len, request_code, response_code);
                if (0 != ret)
                {
                    retval = API_FAILURE
                }
            }
            break;
        default:
            break;
    }
#endif
    return retval;
}

#endif /* GATT_DB */

