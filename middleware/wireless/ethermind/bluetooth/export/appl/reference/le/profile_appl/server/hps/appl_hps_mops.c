/**
 *  \file appl_hps_mops.c
 *
 *  This file contains the menu handling for HPS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined HPS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern APPL_HANDLE appl_handle;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char hps_options[] = "\n\
======== HPS : M A I N   M E N U ========\n\
   0. Exit \n\
   1. Refresh \n\
  10. Set Network Availability \n\
  11. Update Peer BD Address to Auto-Connect\n\
  12. Set GAP Central Role\n\
\n\
  Your Option ?\n";

/* --------------------------------------------- Functions */
void appl_hps_menu_handler (void)
{
    int choice;
    UCHAR flag;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", hps_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\n\n");
                break;

            case 10:
                CONSOLE_OUT("Set Network Availability State \n0.No Network \n1.Network Available\n");
                CONSOLE_IN("%d", &choice);

                flag = (UCHAR)choice;

                appl_hps_set_network_availability(flag);
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
                appl_hps_set_gap_central_role();
                break;

            default:
                CONSOLE_OUT("Invalid Menu Option Selected!\n");
                break;
        }
    }
}

#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined HPS) */
