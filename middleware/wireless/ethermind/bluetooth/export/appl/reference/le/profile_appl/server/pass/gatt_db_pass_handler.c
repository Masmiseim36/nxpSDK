
/**
 *  \file gatt_db_pass_handler.c
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

#if (defined ATT && defined PASS)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_pass_battery_level_handler
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
     *  GATT_CHAR_PASS_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_PASS_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_pass_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16 retval;
    UINT16 config;

    retval = API_SUCCESS;

    if (GATT_CHAR_PASS_RING_CTRL_PNT_INST == handle->char_id)
    {
        if (GATT_DB_CHAR_PEER_WRITE_CMD == params->db_op)
        {
            appl_pass_ringer_control_point_write_handler(handle, &params->value);
        }
    }
    else if ((GATT_CHAR_PASS_ALRT_STATUS_INST == handle->char_id) ||
        (GATT_CHAR_PASS_RING_SET_INST == handle->char_id))
    {
        switch (params->db_op)
        {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            BT_UNPACK_LE_2_BYTE(&config, params->value.val);
            appl_start_trasnfer((*handle), config);
        }
        break;
        case GATT_DB_CHAR_PEER_READ_REQ:
        {
            retval = appl_pass_read_request_handler(handle);
        }
        break;
        default:
            CONSOLE_OUT(
            "No Specific Application Handling Required for Operation 0x%02X\n",
            params->db_op);
            break;
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

API_RESULT gatt_db_pass_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_PASS_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_PASS_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_PASS_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_pass_battery_level_handler(handle, params);
    }
    /* PASS Specific GATT Handlers */
    else if (GATT_SER_PASS_PHN_ALRT_STATUS_INST == handle->service_id)
    {
        retval = gatt_db_pass_handler(handle, params);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined PASS) */
