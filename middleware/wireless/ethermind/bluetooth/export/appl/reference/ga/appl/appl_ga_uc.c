/**
 *  \file appl_ga_uc.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Unicast exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga_uc.h"

#include "appl_ga_ucc.h"
#include "appl_ga_ucs.h"

#ifdef GA_BAP

/* --------------------------------------------- Global Definitions */

/* Unicast Role. Used in routing the Unicast callbacks */
UINT8 uc_role = 0U;

static const char uc_menu_options[] = " \n\
================ GA Unicast MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
    2. Unicast Client. \n\
    3. Unicast Server. \n\
\n\
Your Option ?: ";

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_uc_client_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA][UC]: >> appl_ga_uc_client_cb_handler\n");

    retval = appl_ga_ucc_cb_handler
             (
                 device,
                 ga_event,
                 ga_status,
                 ga_data,
                 ga_datalen
             );

    APPL_DBG("[APPL][GA][UC]: << appl_ga_uc_client_cb_handler\n");

    return retval;
}

GA_RESULT appl_ga_uc_server_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA][UC]: >> appl_ga_uc_server_cb_handler\n");

    retval = appl_ga_ucs_cb_handler
             (
                 device,
                 ga_event,
                 ga_status,
                 ga_data,
                 ga_datalen
             );
    APPL_DBG("[APPL][GA][UC]: << appl_ga_uc_server_cb_handler\n");

    return retval;
}

void appl_ga_uc_init(UINT8 unicast_role)
{
    APPL_DBG("[APPL][GA][UC]: >> appl_ga_uc_init\n");

    if (APPL_GA_UNICAST_CLIENT == (APPL_GA_UNICAST_CLIENT & unicast_role))
    {
        /* Init unicast client */
        appl_ga_ucc_init();
    }

    if (APPL_GA_UNICAST_SERVER == (APPL_GA_UNICAST_SERVER & unicast_role))
    {
        /* Init unicast server */
        appl_ga_ucs_init();
    }

    APPL_DBG("[APPL][GA][UC]: << appl_ga_uc_init\n");
    return;
}

void main_unicast_operations(void)
{
    int choice;
    UINT8 menu_choice;

    choice = 0;
    menu_choice = 0U;

    APPL_DBG("[APPL][GA][UC]: >> main_unicast_operations\n");

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", uc_menu_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = (UINT8)choice;

        switch(menu_choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                main_ucc_operations();
                break;

            case 3:
                main_ucs_operations();
                break;

            default:
                APPL_TRC("Invalid Choice\n");
        }
        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    APPL_DBG("[APPL][GA][UC]: << main_unicast_operations\n");
    return;
}

#endif /* GA_BAP */
