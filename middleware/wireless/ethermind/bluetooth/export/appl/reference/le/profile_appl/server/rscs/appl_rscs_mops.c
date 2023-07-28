/**
 *  \file appl_rscs_mops.c
 *
 *  This file contains the menu handling for RSCS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined RSCS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern APPL_HANDLE    appl_handle;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char rscs_options[] = "\n\
======== RSCS : M A I N   M E N U ========\n\
   0.  Exit\n\
   1.  Refresh\n\
\n\
  10.  Induce Calibration Error\n\
  11.  Reset Calibration Error\n\
\n\
  Your Option ?";

/* --------------------------------------------- Functions */
void appl_rscs_menu_handler (void)
{
    int choice;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", rscs_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\n\n");
                break;

            case 10:
                CONSOLE_OUT("\nInducing Calibration Error");
                appl_rscs_set_calib_error(BT_TRUE);
                break;

            case 11:
                CONSOLE_OUT("\nReset Calibration Error");
                appl_rscs_set_calib_error(BT_FALSE);
                break;

            default:
                CONSOLE_OUT("Invalid Choice\n");
                break;
        }
    }
}

#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined RSCS) */
