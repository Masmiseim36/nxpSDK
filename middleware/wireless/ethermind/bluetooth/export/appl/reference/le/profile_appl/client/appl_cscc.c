
/**
 *  \file appl_cscc.c
 *
 *  This is a Cycling Speed & Cadence Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined CSCC)
/* ----------------------------------------- Macro Defines */
#define SET_CUMMULATIVE_VALUE_OPCODE                   1U
#define START_SENSOR_CALIBRATION_OPCODE                2U
#define UPDATE_SENSOR_LOCATION_OPCODE                  3U
#define REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE       4U

#define SC_CP_SET_CUMMULATIVE_VALUE_REQ_LEN            5U
#define SC_CP_START_SENSOR_CALIBRATION_REQ_LEN         1U
#define SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN           2U
#define SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN    1U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
DECL_STATIC ATT_ATTR_HANDLE cscs_csc_msrmnt_hdl;
DECL_STATIC ATT_ATTR_HANDLE cscs_csc_msrmnt_ccc_hdl;
DECL_STATIC ATT_ATTR_HANDLE cscs_csc_feature_hdl;
DECL_STATIC ATT_ATTR_HANDLE cscs_snsr_loc_hdl;
DECL_STATIC ATT_ATTR_HANDLE cscs_sc_cp_hdl;
DECL_STATIC ATT_ATTR_HANDLE cscs_sc_cp_ccc_hdl;

static const UCHAR cscc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
   --- Cycling Speed and Cadence Service ---\n\
   10 - Discover Cycling Speed and Cadence Service\n\
   11 - Read CSC Measurement Configuration\n\
   12 - Configure CSC Measurement for Notification\n\
   13 - Stop CSC Measurement Notification\n\
   14 - Read CSC Feature\n\
   15 - Read Sensor Location\n\
   16 - Read SC Control Point Configuration\n\
   17 - Configure SC Control Point for Indication\n\
   18 - Stop SC Control Point Indication\n\
   19 - Set Cumulative Value through Control Point\n\
   20 - Start Sensor Calibration through Control Point\n\
   21 - Update Sensor Location through Control Point\n\
   22 - Request Supported Sensor Location through Control Point\n\n\
   --- Device Information Service ---\n\
   23 - DIC Options\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void cscc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i, j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_CSC_MSRMT_CHARACTERISTIC:
                cscs_csc_msrmnt_hdl = characteristic->value_handle;
                break;

            case GATT_CSC_FEATURE_CHARACTERISTIC:
                cscs_csc_feature_hdl = characteristic->value_handle;
                break;

            case GATT_SENSOR_LOCATION_CHARACTERISTIC:
                cscs_snsr_loc_hdl = characteristic->value_handle;
                break;

            case GATT_SC_CONTROL_POINT_CHARACTERISTIC:
                cscs_sc_cp_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_CLIENT_CONFIG ==
                    GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    switch (GATT_GET_CHAR_UUID(characteristic))
                    {
                        case GATT_CSC_MSRMT_CHARACTERISTIC:
                            cscs_csc_msrmnt_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_SC_CONTROL_POINT_CHARACTERISTIC:
                            cscs_sc_cp_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        default:
                            /* CONSOLE_OUT("Invalid...\n"); */
                            break;
                    }

                    break;
                }
            }
        }

        characteristic ++;
    }

    /**
     * The below stored characteristic value handles are currently
     * not used, but these variables are retained for reference.
     */
    BT_IGNORE_UNUSED_PARAM(cscs_csc_msrmnt_hdl);
}


void cscc_profile_operations (void)
{
    UINT32       choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    UCHAR        value_arr[5U];
    int          data;
    UINT16       index;
    ATT_UUID     uuid;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",cscc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_CYCLING_SPEED_AND_CADENCE_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cscs_csc_msrmnt_ccc_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_csc_msrmnt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_csc_msrmnt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 14:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cscs_csc_feature_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 15:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cscs_snsr_loc_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 16:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_ccc_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 17:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 18:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 19:
            value_arr[0U] = SET_CUMMULATIVE_VALUE_OPCODE;
            LOG_DEBUG("Enter the Cumulative Value to be set[Hex]:\n");

            for (index = 1U; index < SC_CP_SET_CUMMULATIVE_VALUE_REQ_LEN; index++)
            {
                CONSOLE_IN("%x", &data);
                value_arr[index] = (UCHAR)data;
            }

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_hdl,
                value_arr,
                SC_CP_SET_CUMMULATIVE_VALUE_REQ_LEN,
                0x01U
            );
            break;

        case 20:
            value_arr[0U] = START_SENSOR_CALIBRATION_OPCODE;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_hdl,
                value_arr,
                SC_CP_START_SENSOR_CALIBRATION_REQ_LEN,
                0x01U
            );
            break;

        case 21:
            value_arr[0U] = UPDATE_SENSOR_LOCATION_OPCODE;
            LOG_DEBUG("Enter the Sensor Location to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            value_arr[1U] = (UCHAR) data;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_hdl,
                value_arr,
                SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN,
                0x01U
            );
            break;

        case 22:
            value_arr[0U] = REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cscs_sc_cp_hdl,
                value_arr,
                SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN,
                0x01U
            );
            break;

        case 23:
#ifdef DIC
            dic_profile_operations ();
#endif /* DIC */
            break;

        default:
            LOG_DEBUG("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined CSCC) */

