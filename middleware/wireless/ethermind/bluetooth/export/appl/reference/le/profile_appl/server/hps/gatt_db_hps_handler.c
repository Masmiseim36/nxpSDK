
/**
 *  \file gatt_db_hps_handler.c
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

#if (defined ATT && defined HPS)
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
UCHAR hps_config_state;

/* --------------------------------------------- Static Global Variables */
/* HPS Characteristic Configuration related Mask values */
#define GATT_HPS_CONFIG_URI_SET_MASK                0x01U
#define GATT_HPS_CONFIG_HTTP_HEADER_SET_MASK        0x02U
#define GATT_HPS_CONFIH_HTTP_ENTITY_BODY_SET_MASK   0x04U

/* --------------------------------------------- Functions */

void gatt_db_hps_config_state_reset(void)
{
    if (0U != hps_config_state)
    {
        hps_config_state = 0U;
    }
}

API_RESULT gatt_db_hps_battery_level_handler
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
     *  GATT_CHAR_HPS_BATTERY_LVL_INST set to 0xFF in corresponding gatt_db.h to
     *  resolve the build issue.
     */
    if ((GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ == params->db_op) &&
       (GATT_CHAR_HPS_BATTERY_LVL_INST == handle->char_id))
    {
        BT_UNPACK_LE_2_BYTE (&config,params->value.val);
        retval = appl_battery_level_handler (handle, config);
    }

    return retval;
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
        if (GATT_CHAR_HPS_HTTP_STATUS_CODE_INST == handle->char_id)
        {
            appl_hsc_cnfg_handler(config);
        }
    }
    break;

    case GATT_DB_CHAR_PEER_PREPARE_WRITE_REQ:
        /* This is queued at the GATT Server Application layer */
        break;

    /* Fall Through */
    case GATT_DB_CHAR_PEER_WRITE_REQ:
    case GATT_DB_CHAR_PEER_EXECUTE_WRITE_REQ:
    {
        if (GATT_CHAR_HPS_HTTP_URI_INST == handle->char_id)
        {
            hps_config_state |= (GATT_HPS_CONFIG_URI_SET_MASK & 0xFFU);

            retval = appl_hps_write_char_value(params->handle, &params->value, params->offset);
        }
        else if (GATT_CHAR_HPS_HTTP_HEADERS_INST == handle->char_id)
        {
            hps_config_state |= (GATT_HPS_CONFIG_HTTP_HEADER_SET_MASK & 0xFFU);

            retval = appl_hps_write_char_value(params->handle, &params->value, params->offset);
        }
        else if (GATT_CHAR_HPS_HTTP_ENTITY_BODY_INST == handle->char_id)
        {
            hps_config_state |= (GATT_HPS_CONFIH_HTTP_ENTITY_BODY_SET_MASK & 0xFFU);

            retval = appl_hps_write_char_value(params->handle, &params->value, params->offset);
        }
        else if (GATT_CHAR_HPS_HTTP_CONTROL_POINT_INST == handle->char_id)
        {
            retval = appl_hps_cp_handler(handle, &params->value, hps_config_state);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    break;

    /* Fall through */
    case GATT_DB_CHAR_PEER_READ_REQ:
    case GATT_DB_CHAR_PEER_READ_BLOB_REQ:
        if ((GATT_CHAR_HPS_HTTP_HEADERS_INST == handle->char_id) ||
            (GATT_CHAR_HPS_HTTP_ENTITY_BODY_INST == handle->char_id))
        {
            retval = appl_hps_read_char_value
                     (
                         handle,
                         params->handle,
                         params->offset,
                         (GATT_DB_CHAR_PEER_READ_BLOB_REQ == params->db_op) ? BT_TRUE : BT_FALSE
                     );
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

API_RESULT gatt_db_hps_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* GAP Service Specific GATT Handlers */
    if (GATT_SER_HPS_GAP_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* GATT Service Specific GATT Handlers */
    else if (GATT_SER_HPS_GATT_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* Battery Service Specific GATT Handlers */
    else if (GATT_SER_HPS_BATTERY_INST == handle->service_id)
    {
        retval = gatt_db_hps_battery_level_handler(handle, params);
    }
    /* Device Info Service Specific GATT Handlers */
    else if (GATT_SER_HPS_DEV_INFO_INST == handle->service_id)
    {
        retval = API_SUCCESS;
    }
    /* HPS Specific GATT Handlers */
    else if (GATT_SER_HPS_HTTP_PROXY_INST == handle->service_id)
    {
        retval = gatt_db_hps_handler(handle, params);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

#endif /* (defined ATT && defined HPS) */
