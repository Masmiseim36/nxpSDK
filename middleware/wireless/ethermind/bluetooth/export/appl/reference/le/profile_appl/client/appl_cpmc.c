
/**
 *  \file appl_cpmc.c
 *
 *  This is a Cycling Power Meter Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"
#include "appl_smp.h"

#if (defined ATT && defined CPMC)
/* ----------------------------------------- Macro Defines */
#define SET_CUMULATIVE_VALUE_OPCODE                    1U
#define UPDATE_SENSOR_LOCATION_OPCODE                  2U
#define REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE       3U
#define SET_CRANK_LENGTH_OPCODE                        4U
#define REQUEST_CRANK_LENGTH_OPCODE                    5U
#define SET_CHAIN_LENGTH_OPCODE                        6U
#define REQUEST_CHAIN_LENGTH_OPCODE                    7U
#define SET_CHAIN_WEIGHT_OPCODE                        8U
#define REQUEST_CHAIN_WEIGHT_OPCODE                    9U
#define SET_SPAN_LENGTH_OPCODE                         10U
#define REQUEST_SPAN_LENGTH_OPCODE                     11U
#define START_OFFSET_COMPENSATION_OPCODE               12U
#define MASK_CPM_CHAR_CONTENT_OPCODE                   13U
#define REQ_SAMPLING_RATE                              14U
#define REQ_FACTORY_CALIBRATION_DATE                   15U
#define START_ENHANCED_OFFSET_COMPENSATION_OPCODE      16U

#define SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN             5U
#define SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN           2U
#define SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN    1U
#define SC_CP_SET_CRANK_LEN                            3U
#define SC_CP_REQUEST_CRANK_LEN                        1U
#define SC_CP_SET_CHAIN_LEN                            3U
#define SC_CP_REQUEST_CHAIN_LEN                        1U
#define SC_CP_SET_CHAIN_WEIGHT_LEN                     3U
#define SC_CP_REQUEST_CHAIN_WEIGHT_LEN                 1U
#define SC_CP_SET_SPAN_LEN                             3U
#define SC_CP_REQUEST_SPAN_LEN                         1U
#define SC_CP_START_OFFSET_COMPENSATION_LEN            1U
#define SC_CP_MASK_CPM_CHAR_CONTENT_LEN                3U
#define SC_CP_REQ_SAMPLING_RATE_LEN                    1U
#define SC_CP_REQ_FACTORY_CALIBRATION_DATE_LEN         1U
#define SC_CP_START_ENHANCED_OFFSET_COMPENSATION_LEN   1U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE cpms_cpm_msrmnt_ccd_hdl;
static ATT_ATTR_HANDLE cpms_cpm_serv_msrmnt_ccd_hdl;
static ATT_ATTR_HANDLE cpms_cpm_msrmnt_hdl;
static ATT_ATTR_HANDLE cpms_cpm_feature_hdl;
static ATT_ATTR_HANDLE cpms_cpm_sensr_loc_hdl;
static ATT_ATTR_HANDLE cpms_cpm_vector_ccd_hdl;
static ATT_ATTR_HANDLE cpms_cpm_cp_ccd_hdl;
static ATT_ATTR_HANDLE cpms_cpm_cp_hdl;
static ATT_ATTR_HANDLE cpms_cpm_vector_hdl;

static const UCHAR cpmc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
\n\
   --- CPM Service ---\n\
   10 - Discover CPM Service\n\
   11 - Read CPM Measurement CCD\n\
   12 - Configure CPM Measurement for Notification\n\
   13 - Disable CPM Measurement Notification\n\
   14 - Read CPM Feature\n\
   15 - Read CPM Sensor Location\n\
   16 - Read CP Vector CCD\n\
   17 - Configure CP Vector for Notification\n\
   18 - Disable CP Vector Notification\n\
   --- Control Point Operations ---\n\
   19 - Read CPM Control Point CCD\n\
   20 - Configure CPM Control Point for Indication\n\
   21 - Disable CPM Control Point Indication\n\
   22 - Set Cumulative Value\n\
   23 - Update Sensor Location\n\
   24 - Request Supported Sensor Locations\n\
   25 - Set Crank Length\n\
   26 - Request Crank Length\n\
   27 - Set Chain Length\n\
   28 - Request Chain Length\n\
   29 - Set Chain Weight\n\
   30 - Request Chain Weight\n\
   31 - Set Span Length\n\
   32 - Request Span Length\n\
   33 - Start Offset Compensation\n\
   34 - Mask Cycling Power Measurement\n\
   35 - Request Sampling Rate\n\
   36 - Request Factory Calibration Date\n\
   37 - Option to Enable Broadcast\n\
   38 - Start Enhanced Offset Compensation\n\
   --- Misc ---\n\
   40 - DIS Options\n\
   50 - BAS Options\n\
   60 - Initiate LE SMP Pairing/Bonding\n\
\
Your Option?\n\
";

/* ------------------------------- Functions */

void cpmc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_CPM_MSRMT_CHARACTERISTIC:
                cpms_cpm_msrmnt_hdl = characteristic->value_handle;
                break;

            case GATT_CPM_VECTOR_CHARACTERISTIC:
                cpms_cpm_vector_hdl = characteristic->value_handle;
                break;

            case GATT_CPM_FEATURE_CHARACTERISTIC:
                cpms_cpm_feature_hdl = characteristic->value_handle;
                break;

            case GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC:
                cpms_cpm_sensr_loc_hdl = characteristic->value_handle;
                break;

            case GATT_CPM_CNTRL_PNT_CHARACTERISTIC:
                cpms_cpm_cp_hdl = characteristic->value_handle;
                break;

            default:
                /* LOG_DEBUG("Invalid...\n"); */
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
                        case GATT_CPM_MSRMT_CHARACTERISTIC:
                            cpms_cpm_msrmnt_ccd_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_CPM_VECTOR_CHARACTERISTIC:
                            cpms_cpm_vector_ccd_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_CPM_CNTRL_PNT_CHARACTERISTIC:
                            cpms_cpm_cp_ccd_hdl = characteristic->descriptor[j].handle;
                            break;

                        default:
                            /* LOG_DEBUG("Invalid...\n"); */
                            break;
                    }

                }
                else if (GATT_SERVER_CONFIG ==
                    GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    switch (GATT_GET_CHAR_UUID(characteristic))
                    {
                        case GATT_CPM_MSRMT_CHARACTERISTIC:
                            cpms_cpm_serv_msrmnt_ccd_hdl = characteristic->descriptor[j].handle;
                            break;

                        default:
                            /* CONSOLE_OUT("Invalid...\n"); */
                            break;
                    }
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
        }

        characteristic ++;
    }

    /**
     * The below stored characteristic value handles are currently
     * not used, but these variables are retained for reference.
     */
    BT_IGNORE_UNUSED_PARAM(cpms_cpm_msrmnt_hdl);
    BT_IGNORE_UNUSED_PARAM(cpms_cpm_vector_hdl);
}


void cpmc_profile_operations (void)
{
    int        choice, menu_choice;
    UINT16     cli_cfg;
    UCHAR      cfg_val[GATT_CLI_CFG_VAL_LEN];
    UCHAR      value_arr[SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN];
    int        data;
    ATT_UUID   uuid;
    UINT32     set_cumulative_val;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",cpmc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_CPM_SERVICE;
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
                cpms_cpm_msrmnt_ccd_hdl,
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
                cpms_cpm_msrmnt_ccd_hdl,
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
                cpms_cpm_msrmnt_ccd_hdl,
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
                cpms_cpm_feature_hdl,
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
                cpms_cpm_sensr_loc_hdl,
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
                cpms_cpm_vector_ccd_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 17:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_vector_ccd_hdl,
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
                cpms_cpm_vector_ccd_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 19:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_ccd_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 20:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_ccd_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 21:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_ccd_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 22:
            value_arr[0U] = SET_CUMULATIVE_VALUE_OPCODE;
            LOG_DEBUG("Enter the Cumulative Value to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);

            set_cumulative_val = (UINT32)data;

            BT_PACK_LE_4_BYTE(&value_arr[1U], &set_cumulative_val);

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN,
                0x01U
            );
            break;

        case 23:
            value_arr[0U] = UPDATE_SENSOR_LOCATION_OPCODE;
            LOG_DEBUG("Enter the Sensor Location to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            value_arr[1U] = (UCHAR) data;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN,
                0x01U
            );
            break;

        case 24:
            value_arr[0U] = REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN,
                0x01U
            );
            break;

        case 25:
            value_arr[0U] = SET_CRANK_LENGTH_OPCODE;
            LOG_DEBUG("Enter the Crank Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[1U]),&data);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_SET_CRANK_LEN,
                0x01U
            );
            break;

        case 26:
            value_arr[0U] = REQUEST_CRANK_LENGTH_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQUEST_CRANK_LEN,
                0x01U
            );
            break;

        case 27:
            value_arr[0U] = SET_CHAIN_LENGTH_OPCODE;
            LOG_DEBUG("Enter the Chain Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[1U]),&data);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_SET_CHAIN_LEN,
                0x01U
            );
            break;

        case 28:
            value_arr[0U] = REQUEST_CHAIN_LENGTH_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQUEST_CHAIN_LEN,
                0x01U
            );
            break;

        case 29:
            value_arr[0U] = SET_CHAIN_WEIGHT_OPCODE;
            LOG_DEBUG("Enter the Chain Weight to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[1U]),&data);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_SET_CHAIN_WEIGHT_LEN,
                0x01U
            );
            break;

        case 30:
            value_arr[0U] = REQUEST_CHAIN_WEIGHT_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQUEST_CHAIN_WEIGHT_LEN,
                0x01U
            );
            break;

        case 31:
            value_arr[0U] = SET_SPAN_LENGTH_OPCODE;
            LOG_DEBUG("Enter the Span Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[1U]),&data);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_SET_SPAN_LEN,
                0x01U
            );
            break;

        case 32:
            value_arr[0U] = REQUEST_SPAN_LENGTH_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQUEST_SPAN_LEN,
                0x01U
            );
            break;

        case 33:
            value_arr[0U] = START_OFFSET_COMPENSATION_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_START_OFFSET_COMPENSATION_LEN,
                0x01U
            );
            break;

        case 34:
            value_arr[0U] = MASK_CPM_CHAR_CONTENT_OPCODE;
            LOG_DEBUG("Mask CPM Characteristics Content\n");
            LOG_DEBUG("Please Enter a 2 Octect Value with appropriate masking\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[1U]),&data);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_MASK_CPM_CHAR_CONTENT_LEN,
                0x01U
            );
            break;

        case 35:
            value_arr[0U] = REQ_SAMPLING_RATE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQ_SAMPLING_RATE_LEN,
                0x01U
            );
            break;

        case 36:
            value_arr[0U] = REQ_FACTORY_CALIBRATION_DATE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_REQ_FACTORY_CALIBRATION_DATE_LEN,
                0x01U
            );
            break;

        case 37:
            cli_cfg = GATT_SER_CNFG_BROADCAST;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_serv_msrmnt_ccd_hdl,
                cfg_val,
                GATT_SER_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 38:
            value_arr[0U] = START_ENHANCED_OFFSET_COMPENSATION_OPCODE;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_cp_hdl,
                value_arr,
                SC_CP_START_ENHANCED_OFFSET_COMPENSATION_LEN,
                0x01U
            );
            break;

        case 40:
#ifdef DIC
            dic_profile_operations ();
#endif /* DIC*/
        break;

        case 50:
#ifdef BAC
            bac_profile_operations ();
#endif /* BAC */
        break;

        case 60:
            retval = appl_smp_initiate_pairing();
            if (API_SUCCESS == retval)
            {
                APPL_TRC(
                "Pairing Initiation Successful!\n");
            }
            else
            {
                APPL_ERR(
                "Pairing Initiation Failed. Reason 0x%04X\n", retval);
            }
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
#endif /* (defined ATT && defined CPMC) */
