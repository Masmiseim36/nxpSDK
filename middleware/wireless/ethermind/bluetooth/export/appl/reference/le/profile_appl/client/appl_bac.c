
/**
 *  \file appl_bac.c
 *
 *  This is a Battery Service Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"


#if (defined ATT && defined BAC)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE bat_lvl_hdl;
static ATT_ATTR_HANDLE bat_lvl_ccd_hdl;

static const UCHAR bac_client_menu[] = "\n\
    0 - Exit\n\
    1 - Refresh\n\
   --- Battery Service ---\n\
   20 - Discover Battery Service/Characteristics\n\
   21 - Read Battery Level\n\
   22 - Read Battery Level CCD \n\
   23 - Configure Battery Level for Notification \n\
   24 - Disable Battery Level Notification \n\
Your Option ?\n\
";

/* ------------------------------- Functions */

void bac_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_BATTERY_LEVEL_CHARACTERISTIC:
                bat_lvl_hdl = characteristic->value_handle;
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
                        case GATT_BATTERY_LEVEL_CHARACTERISTIC:
                            bat_lvl_ccd_hdl = characteristic->descriptor[j].handle;
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

void bac_profile_operations (void)
{
    UINT32   choice, menu_choice;
    UINT16   cli_cfg;
    UCHAR    val_arr[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID uuid;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT ("%s \n", bac_client_menu);
        CONSOLE_OUT ("Enter you choice : ");
        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 20:
            uuid.uuid_16 = GATT_BATTERY_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 21:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                bat_lvl_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 22:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                bat_lvl_ccd_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 23:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(val_arr, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bat_lvl_ccd_hdl,
                val_arr,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 24:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(val_arr, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bat_lvl_ccd_hdl,
                val_arr,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
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

#endif /* (defined ATT && defined BAC) */
