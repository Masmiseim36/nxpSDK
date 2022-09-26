/**
 *  \file appl_sps_mops.c
 *
 *  This file contains the menu handling for SPS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_tis.h"
#include "appl_tis_mops.h"

#if (defined ATT && defined TIS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern BT_timer_handle timer_handle;
extern UCHAR adj_time;
extern APPL_HANDLE appl_handle;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char tis_options[] = " \r\n\
======== TIS : M A I N   M E N U ======== \r\n\
    0.  Exit \n\
    1.  Refresh menu \n\n\
    2.  External Time Reference Update with Adjusting Time less than 1 min\n\
    3.  External Time Reference Update with Adjusting Time more than 1 min\n\
   11. Update Peer BD Address to Auto-Connect\n\
   12. Set GAP Central Role\n\
\n\
Your Option -> \0";

/* --------------------------------------------- Functions */
void appl_tis_menu_handler(void)
{
    int            choice;
    BT_DEVICE_ADDR bd_addr;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", tis_options);
        CONSOLE_IN("%d", &choice);

        switch (choice)
        {
        case 0:
            return;

        case 1:
            /* Refresh */
            break;

        case 2:
            /* Stop Notification if the Adjusting Time is less than 1 minute */
            if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
            {
                BT_stop_timer(timer_handle);
                timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }
            break;

        case 3:
            adj_time = 2U;
            if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
            {
                BT_stop_timer(timer_handle);
                timer_handle = BT_TIMER_HANDLE_INIT_VAL;
            }

            BT_start_timer
            (
                &timer_handle,
                APPL_CUR_TIME_MEASUREMENT_INTERVAL,
                appl_timer_expiry_handler,
                &appl_handle,
                sizeof (appl_handle)
            );
            break;

        case 11:
            CONSOLE_OUT("Enter the Peer BD Address to be updated and used...\n");
            CONSOLE_OUT("Enter BD_ADDR : ");
            appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            CONSOLE_OUT("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &choice);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR)choice;

            /* Update the Global Config Peer Address */
            appl_update_config_peer_addr(&bd_addr);
            break;

        case 12:
            CONSOLE_OUT("Setting GAP Central Role\n");
            appl_tis_set_gap_central_role();
            break;

        default:
            CONSOLE_OUT("\r\nInvalid Option\n");
            break;
        }
    }
}

#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined TIS) */
