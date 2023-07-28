/**
 *  \file appl_pass_mops.c
 *
 *  This file contains the menu handling for PASS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined PASS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern UCHAR    appl_notification_state;
extern BT_timer_handle ringer_setting_timer_handle;
extern BT_timer_handle alert_status_timer_handle;
extern GATT_DB_HANDLE  appl_alert_status_db_hndl;
extern GATT_DB_HANDLE  appl_ringer_settings_db_hndl;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char pass_menu[] = "\n\
========= PASS : M A I N   M E N U ========= \n\
    0. Exit \n\
    1. Refresh \n\
    3. Change Application Notification State \n\
    4. Set Alert Status \n\
    5. Set Ringer Settings \n\
    6. Send Alert Status Notification \n\
    7. Send Ringer Settings Notification \n\
   11. Update Peer BD Address to Auto-Connect\n\
   12. Set GAP Central Role\n\
Your Option ?\n";

static const char alrt_status_menu[] = "\n\
 0 - No Alerts \n\
 1 - Set Ringer State not active \n\
 2 - Set Ringer State active \n\
 3 - Set Vibrate State not active \n\
 4 - Set Vibrate State active \n\
 5 - Set Display Alert Status not active \n\
 6 - Set Display Alert Status active \n";

/* --------------------------------------------- Functions */
void appl_pass_menu_handler (void)
{
    UCHAR val;
    ATT_VALUE att_value;
    int choice;
    int temp;

    att_value.len = 1U;
    att_value.val = &val;

    BT_LOOP_FOREVER()
    {
        printf("\n");
        printf("%s", pass_menu);
        scanf("%d", &choice);

        switch(choice)
        {
        case 0: /* exit */
            return;

        case 1: /* Refresh */
            break;

        case 3:
            /* Set Application Notification State */
            CONSOLE_OUT (" 1 - Auto Send Notification\n");
            CONSOLE_OUT (" 2 - Menu Based Notification\n");
            CONSOLE_IN ("%d", &temp);
            /* Handle the Sub Menu for Alert Status */
            switch (temp)
            {
                case 1:
                    appl_notification_state = APPL_AUTO_SEND_NOTIFICATION;
                    break;

                case 2:
                    appl_notification_state = APPL_MENU_SEND_NOTIFICATION;
                    if (BT_TIMER_HANDLE_INIT_VAL != ringer_setting_timer_handle)
                    {
                        BT_stop_timer (ringer_setting_timer_handle);
                        APPL_TRC (
                        "[APPL]: Stopping Timer %p\n", ringer_setting_timer_handle);
                        ringer_setting_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }
                    if (BT_TIMER_HANDLE_INIT_VAL != alert_status_timer_handle)
                    {
                        BT_stop_timer (alert_status_timer_handle);
                        APPL_TRC (
                        "[APPL]: Stopping Timer %p\n", alert_status_timer_handle);
                        alert_status_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                    }

                    break;

                default:
                    CONSOLE_OUT("\nInvalid Option\n");
                    break;
            }
            break;

        case 4:
            /* Set Alert Status */
            CONSOLE_OUT(alrt_status_menu);
            CONSOLE_IN ("%d", &temp);
            /* Handle the Sub Menu for Alert Status */
            switch (temp)
            {
                case 0:
                     {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_RINGER_STATE_BIT);
                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_VIBRATOR_STATE_BIT);
                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_DISP_ALRT_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 1:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_RINGER_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 2:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_set_bit_value(*(att_value.val),ALRT_STATUS_RINGER_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 3:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_VIBRATOR_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 4:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_set_bit_value(*(att_value.val),ALRT_STATUS_VIBRATOR_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 5:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_clear_bit_value(*(att_value.val),ALRT_STATUS_DISP_ALRT_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                case 6:
                    {
                       BT_gatt_db_get_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );

                       appl_set_bit_value(*(att_value.val),ALRT_STATUS_DISP_ALRT_STATE_BIT);

                       BT_gatt_db_set_char_val
                       (
                           &appl_alert_status_db_hndl,
                           &att_value
                       );
                    }
                    break;
                default:
                    CONSOLE_OUT("\nInvalid Option\n");
                    break;
            }
            break;

        case 5:
            /* Set Ringer Settings */
            CONSOLE_OUT(" 1 - Set Ringer Silent\n");
            CONSOLE_OUT(" 2 - Set Ringer Normal\n");
            CONSOLE_IN ("%d", &temp);

            /* Handle the Sub Menu for Ringer Settings */
            switch (temp)
            {
                case 1:
                    {
                       *(att_value.val) = RINGER_SETTING_RINGER_SILENT;

                       BT_gatt_db_set_char_val
                       (
                           &appl_ringer_settings_db_hndl,
                           &att_value
                       );
                    }
                    break;

                case 2:
                    {
                       *(att_value.val) = RINGER_SETTING_RINGER_NORMAL;

                       BT_gatt_db_set_char_val
                       (
                           &appl_ringer_settings_db_hndl,
                           &att_value
                       );
                    }
                    break;

                default:
                   CONSOLE_OUT("\nInvalid Option\n");
                   break;
            }
            break;

        case 6:
            /* Send Alert Status notifications */
            appl_send_alert_status_ntf();
            break;

        case 7:
            /* Send Ringer Settings notifications */
            appl_send_ringer_settings_ntf();
            break;

        case 11:
        {
            BT_DEVICE_ADDR bd_addr;

            CONSOLE_OUT("Enter the Peer BD Address to be updated and used...\n");
            CONSOLE_OUT("Enter BD_ADDR : ");
            appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            CONSOLE_OUT("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &choice);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR)choice;

            /* Update the Global Config Peer Address */
            appl_update_config_peer_addr(&bd_addr);
        }
        break;

        case 12:
            CONSOLE_OUT("Setting GAP Central Role\n");
            appl_pass_set_gap_central_role();
            break;

        default:
            CONSOLE_OUT("\nInvalid Option\n");
            break;
        }
    }
}
#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined PASS) */

