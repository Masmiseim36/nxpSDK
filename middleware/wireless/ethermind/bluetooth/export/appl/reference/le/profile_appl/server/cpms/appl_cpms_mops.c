/**
 *  \file appl_cpms_mops.c
 *
 *  This file contains the menu handling for CPMS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined CPMS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern UCHAR incorrect_calbn_position;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char cpms_options[] = "\n\
======== CPMS : M A I N   M E N U ========\n\
    0.  Exit\n\
    1.  Refresh\n\
\n\
   10.  Make CPM server set in incorrect position for offset compensation\n\
   11.  Register L2cap callback\n\
\n\
   Your Option ?\0";

/* --------------------------------------------- Functions */
void appl_cpms_menu_handler(void)
{
    int choice;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", cpms_options);
        CONSOLE_IN("%d", &choice);

        switch (choice)
        {
        case 0U:
            CONSOLE_OUT("\nExiting BlueLitE Menu Based Application ...\n\n");
            return;

        case 1U:
            CONSOLE_OUT("\nRefreshing ...\n\n");
            break;

        case 10U:
            CONSOLE_OUT("\nSetting CPM server in an incorrect position for offset compensation... \n");
            incorrect_calbn_position = BT_TRUE;
            break;

        case 11U:
            /** Register L2CAP */
            retval = BT_l2cap_register_le_event_cb(appl_cpms_l2cap_callback);
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
            break;
        }
    }
}
#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined CPMS) */

