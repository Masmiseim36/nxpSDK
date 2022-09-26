
/**
 *  \file appl_pxm.c
 *
 *  This is a Proximity Monitor application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined PXM)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE pxm_ias_alert_level_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_level_hdl;
static ATT_ATTR_HANDLE pxm_ll_alert_level_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_level_ccc_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_cpf_hdl;

static const UCHAR pxm_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
   --- Link Loss Service ---\n\
   10 - Discover Link Loss Service\n\
   11 - Read Link Loss Service - Alert Level\n\
   12 - Set Link Loss Service - Alert Level - \"No Alert\"\n\
   13 - Set Link Loss Service - Alert Level - \"Mild Alert\"\n\
   14 - Set Link Loss Service - Alert Level - \"High Alert\"\n\n\
   --- Immediate Alert Service ---\n\
   20 - Discover Immediate Alert Service\n\
   21 - Immediate Alert Service - Alert Level - \"No Alert\"\n\
   22 - Immediate Alert Service - Alert Level - \"Mild Alert\"\n\
   23 - Immediate Alert Service - Alert Level - \"High Alert\"\n\n\
   --- Tx Power Service ---\n\
   30 - Discover Tx Power Service\n\
   31 - Read Tx Power Level\n\
   32 - Enable/Disable TX Power Service Notification\n\
   33 - Read Tx Power CPF Value\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void pxm_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i,j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_ALERT_LEVEL_CHARACTERISTIC:

                /**
                 *  Add a check to populate IAS or LL handle with
                 *  respect to characteristic property
                 */
                if (GATT_DB_WRITE_WITHOUT_RSP == characteristic->cproperty)
                {
                    /* To populate Immediate Alert Service handle*/
                    pxm_ias_alert_level_hdl = characteristic->value_handle;
                }
                else if (GATT_DB_WRITE == (characteristic->cproperty & GATT_DB_WRITE))
                {
                    /* To populate Link Loss Service handle*/
                    pxm_ll_alert_level_hdl = characteristic->value_handle;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                break;

            case GATT_TX_POWER_LEVEL_CHARACTERISTIC:
                pxm_tx_power_level_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_TX_POWER_LEVEL_CHARACTERISTIC ==
                    GATT_GET_CHAR_UUID(characteristic))
                {
                    switch (GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                    {
                    case GATT_CLIENT_CONFIG:
                        pxm_tx_power_level_ccc_hdl = characteristic->descriptor[j].handle;
                        break;

                    case GATT_FORMAT:
                        pxm_tx_power_cpf_hdl = characteristic->descriptor[j].handle;
                        break;

                    default:
                        /* CONSOLE_OUT("Invalid...\n"); */
                        break;
                    }
                }
            }
        }

        characteristic ++;
    }
}

void pxm_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen)
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

void pxm_parse_read_data (UCHAR * data, UINT16 datalen)
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

void pxm_profile_operations (void)
{
    int          choice, menu_choice;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;
    UCHAR        operation;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",pxm_client_menu);

        CONSOLE_IN ( "%d",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_LINK_LOSS_SERVICE;
            (BT_IGNORE_RETURN_VALUE) gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            uuid.uuid_16 = 0x0000U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_read
            (
                &appl_gatt_client_handle,
                pxm_ll_alert_level_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

        case 12:
            cfg_val[0U] = 0x00U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ll_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x01U
            );

            break;

        case 13:
            cfg_val[0U] = 0x01U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ll_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x01U
            );

            break;

        case 14:
            cfg_val[0U] = 0x02U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ll_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x01U
            );

            break;

         case 20:
            uuid.uuid_16 = GATT_IMMEDIATE_ALERT_SERVICE;
            (BT_IGNORE_RETURN_VALUE) gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

         case 21:
            cfg_val[0U] = 0x00U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ias_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
            );
            break;

         case 22:
            cfg_val[0U] = 0x01U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ias_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
            );
            break;

         case 23:
            cfg_val[0U] = 0x02U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_ias_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
            );
            break;

         case 30:
            uuid.uuid_16 = GATT_TX_POWER_SERVICE ;
            (BT_IGNORE_RETURN_VALUE) gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 31:
            uuid.uuid_16 = 0x0000U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_read
            (
                &appl_gatt_client_handle,
                pxm_tx_power_level_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 32:
            LOG_DEBUG ("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN ("%d", &choice);

            operation = (UCHAR)choice;
            cfg_val[0U] = operation;
            cfg_val[1U] = 0x00U;

            (BT_IGNORE_RETURN_VALUE) gatt_char_wr
            (
                &appl_gatt_client_handle,
                pxm_tx_power_level_ccc_hdl,
                cfg_val,
                0x0002U,
                0x01U
            );
            break;

        case 33:
            uuid.uuid_16 = 0x0000U;
            (BT_IGNORE_RETURN_VALUE) gatt_char_read
            (
                &appl_gatt_client_handle,
                pxm_tx_power_cpf_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
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

#endif /* (defined ATT && defined PXM) */
