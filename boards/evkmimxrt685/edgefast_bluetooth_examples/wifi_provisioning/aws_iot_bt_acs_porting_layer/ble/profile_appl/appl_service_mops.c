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

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_SERVICE_MOPS" )
#include "iot_logging_setup.h"

#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const UCHAR *profile_menu = (UCHAR *) \
 "\n--------------------------------------------\n\
               BLE PROFILE MENU\n\
   --------------------------------------------\n\
   0 - Exit\n\
   1 - Refresh\n\
   10 - SPS Menu Operations. \n\
   11 - GLS Menu Operations. \n\
   12 - IPS Menu Operations. \n\
   13 - ESS Menu Operations. \n\
   14 - WSS Menu Operations. \n\
   15 - CGMS Menu Operations.\n\
   16 - CPMS Menu Operations.\n\
   17 - AIOS Menu Operations.\n\
   18 - CTS Menu Operations. \n\
   19 - ANS Menu Operations. \n\
   20 - PASS Menu Operations.\n\
   21 - POS Menu Operations. \n\
   22 - RCS Menu Operations. \n\
   23 - FTMS Menu Operations.\n\
Your Option ? \0";

/* --------------------------------------------- Functions */

void main_service_menu_operations(void)
{
    unsigned int  choice;

    while (1)
    {
        IotLogInfo("%s \n", profile_menu);
        IotLogInfo("Enter you choice : ");
        CONSOLE_IN("%d", &choice);

        switch (choice)
        {
        case 0:
            return;

        case 1:
            break;

        case 10:
#ifdef SPS
            appl_sps_menu_handler();
#else /* SPS */
            IotLogDebug("*** Enable 'SPS' Compilation Flag\n");
#endif /* SPS */
            break;

        case 11:
#ifdef GLS
            appl_gls_menu_handler();
#else /* GLS */
            IotLogDebug("*** Enable 'GLS' Compilation Flag\n");
#endif /* GLS */
            break;

        case 12:
#ifdef IPS
            appl_ips_menu_handler();
#else /* ips */
            IotLogDebug("*** Enable 'IPS' Compilation Flag\n");
#endif /* IPS */
            break;

        case 13:
#ifdef ESS
            appl_ess_menu_handler();
#else /* ESS */
            IotLogDebug("*** Enable 'ESS' Compilation Flag\n");
#endif /* ESS */
            break;

        case 14:
#ifdef WSS
            appl_wss_menu_handler();
#else /* WSS */
            IotLogDebug("*** Enable 'WSS' Compilation Flag\n");
#endif /* WSS */
            break;

        case 15:
#ifdef CGMS
            appl_cgms_menu_handler();
#else /* CGMS */
            IotLogDebug("*** Enable 'CGMS' Compilation Flag\n");
#endif /* CGMS */
            break;

        case 16:
#ifdef CPMS
            appl_cpms_menu_handler();
#else /* CPMS */
            IotLogDebug("*** Enable 'CPMS' Compilation Flag\n");
#endif /* CPMS */
            break;

        case 17:
#ifdef AIOS
            appl_aios_menu_handler();
#else /* AIOS */
            IotLogDebug("*** Enable 'AIOS' Compilation Flag\n");
#endif /* AIOS */
            break;

        case 18:
#ifdef CTS
            appl_cts_menu_handler();
#else /* CTS */
            IotLogDebug("*** Enable 'CTS' Compilation Flag\n");
#endif /* CTS */
            break;

        case 19:
#ifdef ANS
            appl_ans_menu_handler();
#else /* ANS */
            IotLogDebug("*** Enable 'ANS' Compilation Flag\n");
#endif /* ANS */
            break;

        case 20:
#ifdef PASS
            appl_pass_menu_handler();
#else /* PASS */
            IotLogDebug("*** Enable 'PASS' Compilation Flag\n");
#endif /* PASS */
            break;

        case 21:
#ifdef POS
            appl_pos_menu_handler();
#else /* POS */
            IotLogDebug("*** Enable 'POS' Compilation Flag\n");
#endif /* POS */
            break;

        case 22:
#ifdef RCSP
            appl_rcsp_menu_handler();
#else /* RCSP */
            IotLogDebug("*** Enable 'RCSP' Compilation Flag\n");
#endif /* RCSP */
            break;
        case 23:
#ifdef FTMS
            appl_ftms_menu_handler();
#else /* FTMS */
            IotLogDebug("*** Enable 'FTMS' Compilation Flag\n");
#endif /* FTMS */
            break;

        default:
            break;
        }
    }
}

#endif /* APPL_MENU_OPS */

