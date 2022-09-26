
/**
 *  \file appl_fml.c
 *
 *  This is a Find Me Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined FML)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE fml_alert_level_hdl;


static const UCHAR fml_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
   --- Immediate Alert Service ---\n\
   20 - Discover Immediate Alert Service\n\
   21 - Immediate Alert Service - Alert Level - \"No Alert\"\n\
   22 - Immediate Alert Service - Alert Level - \"Mild Alert\"\n\
   23 - Immediate Alert Service - Alert Level - \"High Alert\"\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void fml_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_ALERT_LEVEL_CHARACTERISTIC:
                fml_alert_level_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        characteristic ++;
    }
}


void fml_profile_operations (void)
{
    int          choice, menu_choice;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",fml_client_menu);

        CONSOLE_IN ( "%d",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 20:
            uuid.uuid_16 = GATT_IMMEDIATE_ALERT_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;


        case 21:
            cfg_val[0U] = 0x00U;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                fml_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
            );

            break;

         case 22:
            cfg_val[0U] = 0x01U;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                fml_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
            );

            break;

        case 23:
            cfg_val[0U] = 0x02U;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                fml_alert_level_hdl,
                cfg_val,
                0x0001U,
                0x00U
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

#endif /* (defined ATT && defined FML) */

