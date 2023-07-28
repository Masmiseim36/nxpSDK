
/**
 *  \file appl_pasp.c
 *
 *  This is a Phone Alert Status Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined PASP)
/* ----------------------------------------- Macro Defines */
#define SET_SILENT_MODE_CMD           1U
#define MUTE_ONCE_CMD                 2U
#define CANCEL_SILENT_MODE_CMD        3U

#define APPL_PAS_CP_REQ_LEN           1U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE pass_ringer_cntrl_pnt_hdl;
static ATT_ATTR_HANDLE pass_alert_status_ccc_hdl;
static ATT_ATTR_HANDLE pass_ringer_setting_ccc_hdl;
static ATT_ATTR_HANDLE pass_alert_status_hdl;
static ATT_ATTR_HANDLE pass_ringer_setting_hdl;

static const UCHAR pasp_client_menu[] =
"\n\
    0.  Exit\n\
    1.  Refresh\n\
       === Phone Alert Status Service ===\n\
   10.  Discover Phone Alert Status Service\n\
        -- Alert Status --\n\
   11.  Configure Alert Status CCC\n\
   12.  Stop Alert Status CCC\n\
   13.  Read Alert Status Characteristic\n\
        -- Ringer Setting --\n\
   14.  Configure Ringer Setting CCC\n\
   15.  Stop Ringer Setting CCC\n\
   16.  Read Ringer Setting Characteristic\n\
        -- Ringer Control Point --\n\
   17.  Set Silent Mode Command\n\
   18.  Cancel Silent Mode Command\n\
   19.  Mute Once Command\n\
   Your Option?\n";

/* ------------------------------- Functions */

void pasp_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_ALERT_STATUS_CHARACTERISTIC:
                pass_alert_status_hdl = characteristic->value_handle;
                break;

            case GATT_RINGER_SETTING_CHARACTERISTIC:
                pass_ringer_setting_hdl = characteristic->value_handle;
                break;

            case GATT_RINGER_CONTROL_POINT_CHARACTERISTIC:
                pass_ringer_cntrl_pnt_hdl = characteristic->value_handle;
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
                    case GATT_ALERT_STATUS_CHARACTERISTIC:
                        pass_alert_status_ccc_hdl = characteristic->descriptor[j].handle;
                        break;

                    case GATT_RINGER_SETTING_CHARACTERISTIC:
                        pass_ringer_setting_ccc_hdl = characteristic->descriptor[j].handle;
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
}


void pasp_profile_operations (void)
{
    UINT32       choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;
    UCHAR        value[APPL_PAS_CP_REQ_LEN + 1U];

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",pasp_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_PHONE_ALERT_STATUS_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_alert_status_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_alert_status_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                pass_alert_status_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 14:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_ringer_setting_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 15:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_ringer_setting_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 16:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                pass_ringer_setting_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 17:
            value[0U] = SET_SILENT_MODE_CMD;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_ringer_cntrl_pnt_hdl,
                value,
                APPL_PAS_CP_REQ_LEN,
                0x00U
            );
            break;

        case 18:
            value[0U] = CANCEL_SILENT_MODE_CMD;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_ringer_cntrl_pnt_hdl,
                value,
                APPL_PAS_CP_REQ_LEN,
                0x00U
            );
            break;

        case 19:
            value[0U] = MUTE_ONCE_CMD;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                pass_ringer_cntrl_pnt_hdl,
                value,
                APPL_PAS_CP_REQ_LEN,
                0x00U
            );
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

#endif /* (defined ATT && defined PASP) */
