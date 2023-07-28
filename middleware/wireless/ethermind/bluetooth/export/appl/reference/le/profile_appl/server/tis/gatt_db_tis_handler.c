
/**
 *  \file gatt_db_tis_handler.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

/**
 * TODO: Check if these GAPS, GATTS, BAS, DIS handlers need to be in a
 *       common location
 */

#if (defined ATT && defined TIS)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_tis_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;

    retval = API_SUCCESS;

    /**
     *  Note: Adding a generic handler for Battery Level Notification.
     *  Incase the profile does not support Battery Service. Please add a dummy
     *  GATT_CHAR_HTS_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_TIS_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_tis_cts_handler
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
        if (GATT_CHAR_TIS_CUR_TIME_INST == handle->char_id)
        {
            retval = appl_update_current_time (&params->value);
        }
        else if (GATT_CHAR_TIS_LCL_TIME_INFO_INST == handle->char_id)
        {
            retval = appl_update_local_time (&params->value);
        }
        else
        {
            /* Do Nothing */
        }
    break;

    case GATT_DB_CHAR_PEER_READ_REQ:
        if (GATT_CHAR_TIS_CUR_TIME_INST == handle->char_id)
        {
            retval = appl_cts_get_current_time (handle, &params->value);
        }
    break;

    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        if (GATT_CHAR_TIS_CUR_TIME_INST == handle->char_id)
        {
            BT_UNPACK_LE_2_BYTE (&config,params->value.val);
            appl_manage_trasnfer ((*handle),config);
        }
    break;

    default:
        CONSOLE_OUT(
        "No Specific Application Handling Required for Operation 0x%02X\n",
        params->db_op);
        break;
    }
    return retval;
}

API_RESULT gatt_db_tis_rtus_handler
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
        if (GATT_CHAR_TIS_TIME_UPDATE_CP_INST == handle->char_id)
        {
            retval = appl_rtus_config_time_update (handle, &params->value);
        }
        break;

    case GATT_DB_CHAR_PEER_READ_REQ:
        if (GATT_CHAR_TIS_TIME_UPDATE_STATE_INST == handle->char_id)
        {
            retval = appl_rtus_get_time_update_state (handle, &params->value);
        }
        break;

    default:
        CONSOLE_OUT(
        "No Specific Application Handling Required for Operation 0x%02X\n",
        params->db_op);
        break;
    }

    return retval;
}


API_RESULT gatt_db_tis_ndcs_handler
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
        if (GATT_CHAR_TIS_TIME_W_DST_INST == handle->char_id)
        {
            retval = appl_ndcs_get_current_time(handle, &params->value);
        }
        break;
    default:
        CONSOLE_OUT(
        "No Specific Application Handling Required for Operation 0x%02X\n",
        params->db_op);
        break;
    }

    return retval;
}


API_RESULT gatt_db_tis_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_TIS_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_TIS_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_TIS_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_tis_battery_level_handler(handle, params);
    }
    /* TIS Specific GATT Handlers */
    else if (GATT_SER_TIS_CUR_TIME_INST == handle->service_id)
    {
        retval = gatt_db_tis_cts_handler(handle, params);
    }
    /* RTUS Specific GATT Handlers */
    else if (GATT_SER_TIS_REF_TIME_UPDATE_INST == handle->service_id)
    {
        retval = gatt_db_tis_rtus_handler(handle, params);
    }
    /* NDCS Specific GATT Handlers */
    else if (GATT_SER_TIS_NDC_INST == handle->service_id)
    {
        retval = gatt_db_tis_ndcs_handler(handle, params);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined TIS) */

