/**
 *  \file appl_basic_mops.c
 *
 *  This file contains the menu handling for BASIC Profile.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined BASIC)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static UCHAR basic_options[] =
"\n\
  0 - Exit\n\
  1 - Refresh\n\
  2 - Switch between Basic GATT DB Sets\n\
  3 - Generate Current Basic DB Hash Value\n\
  4 - Send Service Changed IND to all Clients\n\
  5 - Read Attribute Value Locally\n\
  6 - Reset Local Application Globals\n\
Your Option?\n";

/* --------------------------------------------- Functions */

void appl_basic_menu_handler (void)
{
    int        choice;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", basic_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
        case 0:
            CONSOLE_OUT ("\nExiting EtherMind Menu Based Application ... \n\n");
            return;

        case 1:
            CONSOLE_OUT ("\nRefreshing ...\r\n\n");
            break;

        case 2:
            /* Switch or Toggle the Database */
            appl_basic_switch_gatt_db();
            break;

        case 3:
            /* Calculate the DB Hash */
            appl_basic_calculate_db_hash();
            break;

        case 4:
            /* Send Service Changed IND to all connected clients */
            appl_display_all_connected_devices();
            appl_basic_send_serv_changed(DEVICE_HANDLE_INIT_VAL);
            break;

        case 5:
            {
                ATT_HANDLE_VALUE_PAIR t_hv_pair;
                UINT16                offset;
                UCHAR                 flags;
                APPL_HANDLE           app_hndl;

                /* Initialize the values */
                offset = 0;
                flags  = (GATT_DB_READ | GATT_DB_LOCALLY_INITIATED);

                appl_display_all_connected_devices();
                CONSOLE_OUT("Enter the Application handle[in HEX]:\n");
                CONSOLE_IN("%x", &choice);

                app_hndl = (APPL_HANDLE)choice;

                CONSOLE_OUT("Enter the Attribute handle to be read Locally[in HEX]:\n");
                CONSOLE_IN("%x", &choice);

                /* Take this as Input */
                t_hv_pair.handle = (ATT_ATTR_HANDLE)choice;

                CONSOLE_OUT("Reading Attr value for handle 0x%04X locally\n",
                t_hv_pair.handle);

                retval = BT_gatt_db_access_handle
                         (
                             &t_hv_pair,
                             &APPL_GET_ATT_INSTANCE(app_hndl),
                             offset,
                             flags
                         );

                if (API_SUCCESS == retval)
                {
                    CONSOLE_OUT("The Attribute Value Read Locally for Handle 0x%04X is:\n",
                    t_hv_pair.handle);

                    appl_dump_bytes(t_hv_pair.value.val, t_hv_pair.value.len);
                }
            }
            break;

        case 6:
            #ifdef GATT_DB_DYNAMIC
            appl_basic_dyn_db_init();
            #endif /* GATT_DB_DYNAMIC */
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
            break;
        }
    }
}
#endif /* APPL_MENU_OPS */
#endif /* (defined APPL_MENU_OPS && defined BASIC) */

