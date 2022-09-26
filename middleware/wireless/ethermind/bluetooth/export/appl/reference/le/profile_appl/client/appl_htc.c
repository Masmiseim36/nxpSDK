
/**
 *  \file appl_htc.c
 *
 *  This is a Health Thermometer Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"
#include "appl_smp.h"

#if (defined ATT && defined HTC)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE     appl_gatt_client_handle;
extern BT_DEVICE_ADDR g_bd_addr;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
DECL_STATIC ATT_ATTR_HANDLE hts_temp_msrmt_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_temp_msrmt_ccc_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_intr_temp_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_intr_temp_ccc_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_temp_type_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_msrmt_intrvl_ccc_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_msrmt_intrvl_hdl;
DECL_STATIC ATT_ATTR_HANDLE hts_msrmt_intrvl_vrd_hdl;

static const UCHAR htc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
   10 - Discover Health Thermometer Service\n\
   11 - Configure Temperature Measurement for Indication\n\
   12 - Configure Intermediate Temperature for Notification\n\
   13 - Configure Measurement Interval for Indication\n\
   14 - Read Temperature Type\n\
   15 - Read Measurement Interval\n\
   16 - Update Measurement Interval\n\
   17 - Read Measurement Interval - Valid Range Descriptor\n\
   18 - Stop Temperature Measurement Indication\n\
   19 - Stop Intermediate Temperature Notification\n\
   20 - Stop Measurement Interval Indication\n\n\
   21 - DIS Options\n\n\
   22 - Bond on LE transport\n\n\
Your Option?\n\
";

/* ------------------------------- Functions */
void htc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_TEMPERATURE_MSMNT_CHARACTERISTIC:
                hts_temp_msrmt_hdl = characteristic->value_handle;
                break;

            case GATT_INTERMEDIATE_TEMP_CHARACTERISTIC:
                hts_intr_temp_hdl = characteristic->value_handle;
                break;

            case GATT_TEMPERATURE_TYPE_CHARACTERISTIC:
                hts_temp_type_hdl = characteristic->value_handle;
                break;

            case GATT_MSMNT_INTERVAL_CHARATACTERISTIC:
                hts_msrmt_intrvl_hdl = characteristic->value_handle;
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
                    case GATT_TEMPERATURE_MSMNT_CHARACTERISTIC:
                        hts_temp_msrmt_ccc_hdl = characteristic->descriptor[j].handle;
                        break;

                    case GATT_INTERMEDIATE_TEMP_CHARACTERISTIC:
                        hts_intr_temp_ccc_hdl = characteristic->descriptor[j].handle;
                        break;

                    case GATT_MSMNT_INTERVAL_CHARATACTERISTIC:
                        hts_msrmt_intrvl_ccc_hdl= characteristic->descriptor[j].handle;
                        break;

                    default:
                        /* CONSOLE_OUT("Invalid...\n"); */
                        break;
                    }
                }
                else if (GATT_VALID_RANGE ==
                         GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    hts_msrmt_intrvl_vrd_hdl = characteristic->descriptor[j].handle;
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
    BT_IGNORE_UNUSED_PARAM(hts_temp_msrmt_hdl);
    BT_IGNORE_UNUSED_PARAM(hts_intr_temp_hdl);
}

void htc_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

   /**
    * Empty Function.
    *
    * This is the place-holder for any particular Profile/Service
    * Specific Data Parsing to be done if the Application UI desires.
    */
}

void htc_parse_indication_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

   /**
    * Empty Function.
    *
    * This is the place-holder for any particular Profile/Service
    * Specific Data Parsing to be done if the Application UI desires.
    */
}

void htc_parse_read_data (UCHAR * data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

   /**
    * Empty Function.
    *
    * This is the place-holder for any particular Profile/Service
    * Specific Data Parsing to be done if the Application UI desires.
    */
}

void htc_profile_operations (void)
{
    UINT32     choice, menu_choice;
    UINT16     cli_cfg;
    UINT16     msrmt_intrvl;
    UCHAR      cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID   uuid;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",htc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_HEALTH_THERMOMETER_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_temp_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_intr_temp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_msrmt_intrvl_ccc_hdl,
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
                hts_temp_type_hdl,
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
                hts_msrmt_intrvl_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 16:
            LOG_DEBUG ("Please enter the new measurement interval[Dec]: ");
            CONSOLE_IN ("%u", &choice);
            msrmt_intrvl = (UINT16)choice;
            BT_PACK_LE_2_BYTE(cfg_val, &msrmt_intrvl);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_msrmt_intrvl_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 17:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hts_msrmt_intrvl_vrd_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 18:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_temp_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 19:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_intr_temp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 20:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hts_msrmt_intrvl_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 21:
#ifdef DIC
        dic_profile_operations ();
#endif /* DIC */
            break;

        case 22:
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

#endif /* (defined ATT && defined HTC) */
