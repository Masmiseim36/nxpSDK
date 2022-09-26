
/**
 *  \file gatt_db_rscs_handler.c
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

#if (defined ATT && defined RSCS)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_rscs_battery_level_handler
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
     *  GATT_CHAR_RSCS_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_RSCS_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

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
        BT_UNPACK_LE_2_BYTE(&config, params->value.val);
        if (GATT_CHAR_RSCS_SC_CNTRL_PNT_INST == handle->char_id)
        {
            appl_rsc_cntrl_point_cnfg_handler(handle, config);
        }
        else
        {
            appl_manage_trasnfer((*handle), config);
        }
    }
    break;
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        if (GATT_CHAR_RSCS_SC_CNTRL_PNT_INST == handle->char_id)
        {
            retval = appl_rsc_control_point_handler(handle, &params->value);
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

API_RESULT gatt_db_rscs_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_RSCS_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_RSCS_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_RSCS_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_rscs_battery_level_handler(handle, params);
    }
    /* Device Info Service Specific GATT Handlers */
    else if (GATT_SER_RSCS_DEV_INFO_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* RSCS Specific GATT Handlers */
    else if (GATT_SER_RSCS_RSC_INST == handle->service_id)
    {
        retval = gatt_db_rscs_handler(handle, params);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined RSCS) */
