/**
 *  \file appl_service_mops.c
 *
 *  This file contains the Sample Profile Menu Operations.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service_mops.h"

#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
DECL_STATIC DECL_CONST UCHAR *profile_menu = (UCHAR *) \
"\n--------------------------------------------\n\
               BLE PROFILE MENU\n\
--------------------------------------------\n\
   0 - Exit\n\
   1 - Refresh\n\
  10 - BASIC Menu Operations \n\
  11 - GLS Menu Operations. \n\
  16 - CPMS Menu Operations.\n\
  18 - TIS Menu Operations. \n\
  20 - PASS Menu Operations.\n\
  24 - HPS Menu Operations.\n\
  25 - RSCS Menu Operations.\n\
  26 - BPS Menu Operation.\n\
\n\
 100 - PTS Menu Operations\n\
Your Option ? \0";

/* --------------------------------------------- Functions */

void main_service_menu_operations(void)
{
    unsigned int  choice, menu_choice;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s\n", profile_menu);
        CONSOLE_OUT("Enter you choice : ");
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
#ifdef BASIC
            appl_basic_menu_handler();
#else /* BASIC */
            CONSOLE_OUT("*** Enable 'BASIC' Compilation Flag\n");
#endif /* BASIC */
            break;

        case 11:
#ifdef GLS
            appl_gls_menu_handler();
#else /* GLS */
            CONSOLE_OUT("*** Enable 'GLS' Compilation Flag\n");
#endif /* GLS */
            break;

        case 16:
#ifdef CPMS
            appl_cpms_menu_handler();
#else /* CPMS */
            CONSOLE_OUT("*** Enable 'CPMS' Compilation Flag\n");
#endif /* CPMS */
            break;

        case 18:
#ifdef TIS
            appl_tis_menu_handler();
#else /* TIS */
            CONSOLE_OUT("*** Enable 'TIS' Compilation Flag\n");
#endif /* TIS */
            break;

        case 20:
#ifdef PASS
            appl_pass_menu_handler();
#else /* PASS */
            CONSOLE_OUT("*** Enable 'PASS' Compilation Flag\n");
#endif /* PASS */
            break;

        case 24:
#ifdef HPS
            appl_hps_menu_handler();
#else /* HPS */
            CONSOLE_OUT("*** Enable 'HPS' Compilation Flag\n");
#endif /* HPS */
            break;

        case 25:
#ifdef RSCS
            appl_rscs_menu_handler();
#else /* RSCS */
            CONSOLE_OUT("*** Enable 'RCSP' Compilation Flag\n");
#endif /* RSCS */
            break;

        case 26:
#ifdef BPS
            appl_bps_menu_handler();
#else /* BPS */
            CONSOLE_OUT("*** Enable 'BPS' Compilation Flag\n");
#endif /* BPS */
            break;

        case 100:
#ifdef PTS
            appl_pts_menu_handler();
#else /* PTS */
            CONSOLE_OUT("*** Enable 'PTS' Compilation Flag\n");
#endif /* PTS */
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
            break;
        }

        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

#endif /* APPL_MENU_OPS */

