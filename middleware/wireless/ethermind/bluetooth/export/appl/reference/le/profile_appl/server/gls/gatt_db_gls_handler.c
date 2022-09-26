
/**
 *  \file gatt_db_gls_handler.c
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

#if (defined ATT && defined GLS)
/* -------------------------------------------- Constants */
#define GL_CONFIGURED_STATE                  0x07U

#define GATT_GLS_MSRMT_BIT_SET_MASK          0x01U
#define GATT_GLS_CNTXT_MSRMT_BIT_SET_MASK    0x02U
#define GATT_GLS_RACP_BIT_SET_MASK           0x04U
#define GATT_GLS_MSRMT_BIT_RESET_MASK        0xFEU
#define GATT_GLS_CNTXT_MSRMT_BIT_RESET_MASK  0xFDU
#define GATT_GLS_RACP_BIT_RESET_MASK         0xFBU
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT gatt_db_gls_battery_level_handler
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
     *  GATT_CHAR_GLS_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_GLS_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
}

API_RESULT gatt_db_gls_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    UINT16     retval;
    static UCHAR     gl_config_state;
    UINT16           config;
    retval = API_SUCCESS;

    switch (params->db_op)
    {
    case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
    {
        BT_UNPACK_LE_2_BYTE(&config, params->value.val);
        if (GATT_CHAR_GLS_GL_MSRMNT_INST == handle->char_id)
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
        else if (GATT_CHAR_GLS_GL_CNTXT_INST == handle->char_id)
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
        else if (GATT_CHAR_GLS_GL_RACP_INST == handle->char_id)
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
        break;
    }
    case GATT_DB_CHAR_PEER_WRITE_REQ:
        /**
         * Security requirements checked prior by GATT DB internal layers,
         * go ahead with data transfer.
         */
        if (GL_CONFIGURED_STATE == gl_config_state)
        {
            retval = appl_gls_racp_write_handler(&params->value);
        }
        else
        {
            retval = ATT_APPL_CCD_IMPROPERLY_CONFIGURED | APPL_ERR_ID;
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

API_RESULT gatt_db_gls_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_GLS_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_GLS_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_GLS_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_gls_battery_level_handler(handle, params);
    }
    /* Device Info Service Specific GATT Handlers */
    else if (GATT_SER_GLS_DEV_INFO_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GLS Specific GATT Handlers */
    else if (GATT_SER_GLS_GLUCOSE_METER_INST == handle->service_id)
    {
        retval = gatt_db_gls_handler(handle, params);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined GLS) */
