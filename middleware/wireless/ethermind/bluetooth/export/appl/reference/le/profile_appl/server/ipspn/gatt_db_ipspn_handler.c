
/**
 *  \file gatt_db_ipspn_handler.c
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
 * TODO:
 * Check if these GAPS, GATTS, BAS, DIS handlers need to be in a
 * common location
 */

#if (defined ATT && defined IPSPN)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_ipspn_battery_level_handler
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
     *  GATT_CHAR_IPSPN_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_IPSPN_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_ipspn_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_IPSPN_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_IPSPN_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_IPSPN_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_ipspn_battery_level_handler(handle, params);
    }
    /* IPSPN Specific GATT Handlers */
    else if (GATT_SER_IPSPN_IPSP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined IPSPN) */
