
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

/* CPM control point opcode length */
#define SC_CP_OPCODE_LEN                                1U

/* Request length excludes opcode */
#define SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN              4U
#define SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN            1U
#define SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN     0U
#define SC_CP_SET_CRANK_LEN                             2U
#define SC_CP_REQUEST_CRANK_LEN                         0U
#define SC_CP_SET_CHAIN_LEN                             2U
#define SC_CP_REQUEST_CHAIN_LEN                         0U
#define SC_CP_SET_CHAIN_WEIGHT_LEN                      2U
#define SC_CP_REQUEST_CHAIN_WEIGHT_LEN                  0U
#define SC_CP_SET_SPAN_LEN                              2U
#define SC_CP_REQUEST_SPAN_LEN                          0U
#define SC_CP_START_OFFSET_COMPENSATION_LEN             0U
#define SC_CP_MASK_CPM_CHAR_CONTENT_LEN                 2U
#define SC_CP_REQ_SAMPLING_RATE_LEN                     0U
#define SC_CP_REQ_FACTORY_CALIBRATION_DATE_LEN          0U
#define SC_CP_START_ENHANCED_OFFSET_COMPENSATION_LEN    0U

/**
 *  For any of the current CPM Control Point procedure[CPMC Ver 1.0]
 *  the maximum request length is setting to
 *  5 Bytes = Opcode Len(1 Byte) + Set Cummulative Value Req Len(4 Bytes)
 */
#define SC_CP_MAX_REQ_LEN  (SC_CP_OPCODE_LEN + SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN)

/**
 * CMPC Control Point Response Code received from CPMS Indication
 */
#define APPL_CPMC_RESPONSE_CODE                        0x20U

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

UCHAR appl_cpmc_cp_set;
static BT_timer_handle appl_gatt_client_timer_handle;

static const UCHAR cpmc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
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
   37 - Enable CPM Broadcast\n\
   38 - Start Enhanced Offset Compensation\n\
   39 - Disable CPM Broadcast\n\
   --- Misc ---\n\
   40 - DIS Options\n\
   50 - BAS Options\n\
   60 - Initiate LE SMP Pairing/Bonding\n\
Your Option?\n";

/* ------------------------------- Functions Prototype */
void appl_gatt_client_timer_cb
     (
         /* IN */ void   * data_param,
         /* IN */ UINT16   datalen
     );
API_RESULT appl_cpmc_cp_send
           (
               ATT_HANDLE       * appl_gatt_client_hndl,
               ATT_ATTR_HANDLE    attr_handle,
               UCHAR              cp_opcode,
               UCHAR            * data,
               UINT16             data_len
           );

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
                            /* CONSOLE_OUT("Invalid...\n"); */
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
    UCHAR      cp_opcode;

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
            cp_opcode = SET_CUMULATIVE_VALUE_OPCODE;
            CONSOLE_OUT("Enter the Cumulative Value to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);

            set_cumulative_val = (UINT32)data;

            BT_PACK_LE_4_BYTE(&value_arr[0U], &set_cumulative_val);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_SET_CUMULATIVE_VALUE_REQ_LEN
                     );
            break;

        case 23:
            cp_opcode = UPDATE_SENSOR_LOCATION_OPCODE;
            CONSOLE_OUT("Enter the Sensor Location to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            value_arr[0U] = (UCHAR) data;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_UPDATE_SENSOR_LOCATION_REQ_LEN
                     );
            break;

        case 24:
            cp_opcode = REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQUEST_SUPPORTED_SENSOR_LOCATION_LEN
                     );
            break;

        case 25:
            cp_opcode = SET_CRANK_LENGTH_OPCODE;
            CONSOLE_OUT("Enter the Crank Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[0U]),&data);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_SET_CRANK_LEN
                     );
            break;

        case 26:
            cp_opcode = REQUEST_CRANK_LENGTH_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQUEST_CRANK_LEN
                     );
            break;

        case 27:
            cp_opcode = SET_CHAIN_LENGTH_OPCODE;
            CONSOLE_OUT("Enter the Chain Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[0U]),&data);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_SET_CHAIN_LEN
                     );
            break;

        case 28:
            cp_opcode = REQUEST_CHAIN_LENGTH_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQUEST_CHAIN_LEN
                     );
            break;

        case 29:
            cp_opcode = SET_CHAIN_WEIGHT_OPCODE;
            CONSOLE_OUT("Enter the Chain Weight to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[0U]),&data);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_SET_CHAIN_WEIGHT_LEN
                     );
            break;

        case 30:
            cp_opcode = REQUEST_CHAIN_WEIGHT_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQUEST_CHAIN_WEIGHT_LEN
                     );
            break;

        case 31:
            cp_opcode = SET_SPAN_LENGTH_OPCODE;
            CONSOLE_OUT("Enter the Span Length to be set[Hex]:\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[0U]),&data);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_SET_SPAN_LEN
                     );
            break;

        case 32:
            cp_opcode = REQUEST_SPAN_LENGTH_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQUEST_SPAN_LEN
                     );
            break;

        case 33:
            cp_opcode = START_OFFSET_COMPENSATION_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_START_OFFSET_COMPENSATION_LEN
                     );
            break;

        case 34:
            cp_opcode = MASK_CPM_CHAR_CONTENT_OPCODE;
            CONSOLE_OUT("Mask CPM Characteristics Content\n");
            CONSOLE_OUT("Please Enter a 2 Octect Value with appropriate masking\n");
            CONSOLE_IN("%x", &data);
            BT_PACK_LE_2_BYTE((&value_arr[0U]),&data);

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         &value_arr[0U],
                         SC_CP_MASK_CPM_CHAR_CONTENT_LEN
                     );
            break;

        case 35:
            cp_opcode = REQ_SAMPLING_RATE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQ_SAMPLING_RATE_LEN
                     );
            break;

        case 36:
            cp_opcode = REQ_FACTORY_CALIBRATION_DATE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_REQ_FACTORY_CALIBRATION_DATE_LEN
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

            /* Start Scanning to listen for Broadcast */
            /**
             * TODO: This needs to be an abstracted call.
             * Instead of restricting Legacy Scan command,
             * need to support either Legacy or Extended Scan.
             */
            (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_scan_enable(0x01U, 0x01U);
            break;

        case 38:
            cp_opcode = START_ENHANCED_OFFSET_COMPENSATION_OPCODE;

            retval = appl_cpmc_cp_send
                     (
                         &appl_gatt_client_handle,
                         cpms_cpm_cp_hdl,
                         cp_opcode,
                         NULL,
                         SC_CP_START_ENHANCED_OFFSET_COMPENSATION_LEN
                     );
            break;

        case 39:
            cli_cfg = GATT_SER_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cpms_cpm_serv_msrmnt_ccd_hdl,
                cfg_val,
                GATT_SER_CFG_VAL_LEN,
                0x01U
            );

            /* Stop Scanning to listen for Broadcast */
            /**
             * TODO: This needs to be an abstracted call.
             * Instead of restricting Legacy Scan command,
             * need to support either Legacy or Extended Scan.
             */
            (BT_IGNORE_RETURN_VALUE) BT_hci_le_set_scan_enable(0x00U, 0x01U);
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
                CONSOLE_OUT(
                "Pairing Initiation Successful!\n");
            }
            else
            {
                APPL_ERR(
                "Pairing Initiation Failed. Reason 0x%04X\n", retval);
            }
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
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

API_RESULT appl_cpmc_cp_send
           (
               ATT_HANDLE      * appl_gatt_client_hndl,
               ATT_ATTR_HANDLE   attr_handle,
               UCHAR             cp_opcode,
               UCHAR           * data,
               UINT16            data_len
           )
{
    API_RESULT retval;
    UCHAR      value[SC_CP_MAX_REQ_LEN];
    UINT16     value_len;

    /* Initialize */
    retval    = API_FAILURE;
    value_len = 0;

    if (data_len <= SC_CP_MAX_REQ_LEN)
    {
        /* Extract opcode */
        value[0U] = cp_opcode;

        /* Update Value Len */
        value_len += SC_CP_OPCODE_LEN;

        /* Data length 0 indicates Control point with only Opcode */
        if (0 != data_len)
        {
            /* Extract remaining data */
            BT_mem_copy(&value[1U], &data[0U], data_len);

            value_len += data_len;
        }

        /**
         * Before receiving the indication for the control point
         * written from the sensor, if initiated another write request
         * then reject the write req
         */
        if (BT_FALSE == appl_cpmc_cp_set)
        {
            retval = gatt_char_wr
                     (
                         appl_gatt_client_hndl,
                         attr_handle,
                         value,
                         value_len,
                         0x01U
                    );

            if (API_FAILURE != retval)
            {
                appl_cpmc_cp_set = BT_TRUE;
            }
        }
        else
        {
            CONSOLE_OUT(
            "[CPMC]\nControl Point procedure is under Progress."
            " Cannot initiate another write request.\n");
        }
    }
    else
    {
        CONSOLE_OUT("Invalid parameter\n");
    }

    return retval;
}

void cpmc_parse_indication_data
     (
         UINT16   handle,
         UCHAR  * data,
         UINT16   datalen
     )
{
    UCHAR    rsp_code_opcode;

    BT_IGNORE_UNUSED_PARAM(datalen);

    if (cpms_cpm_cp_hdl == handle)
    {
        if (NULL != data)
        {
            /* Extract Response Code Opcode from the received indication */
            rsp_code_opcode = data[0U];

            if ((BT_FALSE != appl_cpmc_cp_set) &&
                (APPL_CPMC_RESPONSE_CODE == rsp_code_opcode))
            {
                /* Stop the timer */
                if (BT_TIMER_HANDLE_INIT_VAL != appl_gatt_client_timer_handle)
                {
                    BT_stop_timer(appl_gatt_client_timer_handle);
                    appl_gatt_client_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                /* Reset */
                appl_cpmc_cp_set = BT_FALSE;

                CONSOLE_OUT(
                "\n[CPMC]: Control point procedure status is Success\n");
            }
        }
    }
    else
    {
        CONSOLE_OUT("Invalid Handle\n");
    }
}

void cpmc_notify_write_rsp()
{
    API_RESULT retval;

    /* Initialize */
    retval = API_SUCCESS;

    /**
     * After receiving write response for writing control point to server
     * will start the timer, with the value set to the ATT transaction timeout
     *  >> If within ATT transaction timeout received the indication
     *     from the server then procedure is consider as success
     *  >> If within ATT transaction timeout not received the indication
     *     from the server then procedure is consider as ATT Transaction Timeout
     */

    if (BT_FALSE != appl_cpmc_cp_set)
    {
        /* Start the timer with the value set to the ATT transaction timeout */
        retval = BT_start_timer
                 (
                     &appl_gatt_client_timer_handle,
                     ATT_RESPONSE_TIMEOUT,
                     appl_gatt_client_timer_cb,
                     NULL,
                     0U
                 );

        if (API_SUCCESS != retval)
        {
            CONSOLE_OUT("[CPMC]: Timer Start Failed with 0x%04X\n", retval);
        }
        else
        {
            /* CONSOLE_OUT("Started Timer\n"); */
        }
    }
}

void appl_gatt_client_timer_cb
     (
         /* IN */ void   * data_param,
         /* IN */ UINT16   datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    BT_IGNORE_UNUSED_PARAM(data_param);

    /* If the timer expires, then the procedure shall be considered to have failed.*/

    /* Reset global variable */
    appl_cpmc_cp_set = BT_FALSE;

    CONSOLE_OUT(
    "[CPMC]: Control point procedure status is 0x%04X\n",
    ATT_RESPONSE_TIMED_OUT);
}

void cpmc_notify_gatt_disconn(void)
{
    CONSOLE_OUT("\n[CPMC]: ACL disconnected with CPM Sensor\n");

    /* Reset global variable */
    appl_cpmc_cp_set = BT_FALSE;
}

void cpmc_notify_gatt_conn(void)
{
    /* Place Holder for connection related handling */
    CONSOLE_OUT("\n[CPMC]: ACL Connected with CPM Sensor\n");
}

void cpmc_notify_adv_report
     (
         UCHAR          adv_type,
         BT_DEVICE_ADDR * bd,
         UCHAR          * data,
         UINT16         data_len
     )
{
    int    i;
    UINT32 count;
    UCHAR  ad_element_len, ad_type, index;
    UINT16 service_uuid, flag, inst_power;

    /* Initialize */
    index = 0U;
    count = 0U;

    if (NULL != data)
    {
        /* CPM Broadcast Data is Non-Connectable Only */
        if (HCI_ADV_NONCONN_IND == adv_type)
        {
            for (i = 0U; i < data_len; i++)
            {
                ad_element_len = data[index];
                index++;

                if (1 <= ad_element_len)
                {
                    ad_type = data[index];
                    index++;

                    /* Search for Service Data 16bit UUID ad type in adv data */
                    if (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS == ad_type)
                    {
                        /* Extract Service UUID */
                        BT_UNPACK_LE_2_BYTE(&service_uuid, &data[index]);
                        index += 2;

                        /**
                         * CPM Broadcast ADV Service Data consists of
                         * 1. AD Type                            : 1 Byte
                         * 2. Service UUID corresponding to Data : 2 Bytes
                         * 3. Flags Field                        : 2 Bytes
                         * 4. Instantenous Power                 : 2 Bytes
                         * 5. Additional Field (Optional depending on Flags)
                         * Total Mandatory Field is 7Bytes
                         */
                        if ((GATT_CPM_SERVICE == service_uuid) &&
                            (7 <= ad_element_len))
                        {
                            CONSOLE_OUT(
                            "\n[CPMC]: Received BROADCAST Data from "
                            BT_DEVICE_ADDR_FRMT_SPECIFIER "\n",
                            BT_DEVICE_ADDR_PRINT_STR(bd));

                            BT_UNPACK_LE_2_BYTE(&flag, &data[index]);
                            CONSOLE_OUT("Flag               : [0x%04X]\n",
                            flag);
                            index += 2;

                            BT_UNPACK_LE_2_BYTE(&inst_power, &data[index]);
                            CONSOLE_OUT("Instantanoues Power: [%d(0x%04X)]W\n",
                            inst_power, inst_power);
                            index += 2;

                            /* Print Additional Fields */
                            if (7 < ad_element_len)
                            {
                                CONSOLE_OUT(
                                "Other fields(not Decoding, only Dumping):\n");
                                appl_dump_bytes(&data[index], (ad_element_len - 7));
                            }
                        }
                        break;
                    }
                }

                /* Point to next AD Element */
                index += (ad_element_len - 1U);

                /* adjust the adv data length */
                data_len = data_len - (ad_element_len + 1U);
            }
        }
    }
    else
    {
        /* CONSOLE_OUT("Adv report doesn't have an adv data into it\n"); */
    }

    BT_IGNORE_UNUSED_PARAM(bd);
}

#endif /* (defined ATT && defined CPMC) */

