/**
 *  \file appl_ga_bc.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga_bc.h"

#include "appl_ga_bc_ba.h"
#include "appl_ga_bc_sd.h"

#ifdef BAP_BROADCAST

/* --------------------------------------------- Global Definitions */

/** Broadcast Role. Used in routing the Broadcast callbacks */
UINT8 bc_role = BC_ROLE_NONE;

static const char bc_menu_options[] = " \n\
================ GA Broadcast MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
    2. Broadcast BA - 'BA' OR 'BA Collocated BC Src' OR 'BC Src' Operations. \n\
    3. Broadcast SD - 'SD Collocated Sink' OR 'BC Sink' Operations. \n\
\n\
Your Option ?: ";

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_bc_src_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_SUCCESS;

    APPL_DBG(">> appl_ga_bc_src_cb_handler\n");

    if ((IS_APPL_GA_BC_ROLE_BC_SRC(bc_role)) ||
        (IS_APPL_GA_BC_ROLE_BA_PLUS_BC_SRC(bc_role)))
    {
        retval = appl_ga_bc_src_handler
                 (
                     device,
                     ga_event,
                     ga_status,
                     ga_data,
                     ga_datalen
                 );
    }
    else
    {
        /* No action */
    }

    APPL_DBG("<< appl_ga_bc_src_cb_handler\n");

    return retval;
}

GA_RESULT appl_ga_bc_sink_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_SUCCESS;

    APPL_DBG(">> appl_ga_bc_sink_cb_handler\n");

    if (IS_APPL_GA_BC_ROLE_BA(bc_role))
    {
        retval = appl_ga_bc_ba_bc_sink_handler
                 (
                     device,
                     ga_event,
                     ga_status,
                     ga_data,
                     ga_datalen
                 );
    }
    else if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        retval = appl_ga_bc_sd_sink_cb_handler
                 (
                     device,
                     ga_event,
                     ga_status,
                     ga_data,
                     ga_datalen
                 );
    }
    else if (IS_APPL_GA_BC_ROLE_BC_SINK(bc_role))
    {
        retval = appl_ga_bc_sink_handler
                 (
                     device,
                     ga_event,
                     ga_status,
                     ga_data,
                     ga_datalen
                 );
    }

    APPL_DBG("<< appl_ga_bc_sink_cb_handler\n");

    return retval;
}

void appl_ga_bc_init(void)
{
    appl_ga_bc_sd_init();
    appl_ga_bc_snk_init();
    appl_ga_bc_ba_snk_init();

    return;
}

void main_broadcast_operations(void)
{
    int choice;
    UINT8 menu_choice;

    menu_choice = 0U;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", bc_menu_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = (UINT8)choice;

        switch(menu_choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                main_ba_operations();
                break;

            case 3:
                main_sd_operations();
                break;

            default:
                APPL_ERR("Invalid Choice\n");
        }
        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

#endif /* BAP_BROADCAST */
