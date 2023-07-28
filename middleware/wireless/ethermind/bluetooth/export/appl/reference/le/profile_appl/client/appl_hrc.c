
/**
 *  \file appl_hrc.c
 *
 *  This is a Heart Rate Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined HRC)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
DECL_STATIC ATT_ATTR_HANDLE hrs_msrmt_hdl;
DECL_STATIC ATT_ATTR_HANDLE hrs_msrmt_ccc_hdl;
DECL_STATIC ATT_ATTR_HANDLE hrs_sensor_loc_hdl;
DECL_STATIC ATT_ATTR_HANDLE hrs_cntrl_pnt_hdl;

static const UCHAR hrc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
   --- Heart Rate Service ---\n\
   10 - Discover Heart Rate Service\n\
   11 - Configure Heart Measurement for Notification\n\
   12 - Read Sensor Location\n\
   13 - Reset Energy Expended\n\
   14 - Stop Heart Rate Measurement Notification\n\n\
   --- Device Information Service ---\n\
   15 - DIC Options\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void hrc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i, j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_HR_MSRMT_CHARACTERISTIC:
                hrs_msrmt_hdl = characteristic->value_handle;
                break;

            case GATT_BODY_SENSOR_LOC_CHARACTERISTIC:
                hrs_sensor_loc_hdl = characteristic->value_handle;
                break;

            case GATT_HR_CNTRL_POINT:
                hrs_cntrl_pnt_hdl = characteristic->value_handle;
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
                    case GATT_HR_MSRMT_CHARACTERISTIC:
                        hrs_msrmt_ccc_hdl = characteristic->descriptor[j].handle;
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
    BT_IGNORE_UNUSED_PARAM(hrs_msrmt_hdl);
}


void hrc_profile_operations (void)
{
    UINT32       choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",hrc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_HEART_RATE_SERVICE;
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
                hrs_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hrs_sensor_loc_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            break;

        case 13:
            cfg_val[0U] = 0x01U;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hrs_cntrl_pnt_hdl,
                cfg_val,
                0x0001U,
                0x01U
            );

            break;

        case 14:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hrs_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 15:
#ifdef DIC
            dic_profile_operations ();
#endif /* DIC */
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

#endif /* (defined ATT && defined HRC) */

