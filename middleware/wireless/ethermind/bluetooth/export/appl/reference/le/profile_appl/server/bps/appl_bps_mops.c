/**
*  \file appl_bps_mops.c
*
*  This file contains the menu handling for BPS Profile.
*/

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_service_mops.h"

#if (defined ATT && defined BPS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char bps_options[] = "\n\
======== BPS : M A I N   M E N U ========\n\
   0. Exit \n\
   1. Refresh \n\
  10. Support Multi Bond and Target Address AD Type in ADV\n\
  11. Support Multi Bond and No Target Address AD Type in ADV\n\
  12. Support Single Bond and No Target Address AD Type in ADV\n\
  13. Support body_movement and Target Address AD Type in ADV\n\
  14. Support body_movement and No Target Address AD Type in ADV\n\
  15. Support No body_movement and No Target Address AD Type in ADV\n\
  16. Support cuff_fit_Detection and Target Address AD Type in ADV\n\
  17. Support cuff_fit_Detection and No Target Address AD Type in ADV\n\
  18. Support No cuff_fit_Detection and No Target Address AD Type in ADV\n\
  19. Support irregular_pulse and Target Address AD Type in ADV\n\
  20. Support irregular_pulse and No Target Address AD Type in ADV\n\
  21. Support No irregular_pulse and No Target Address AD Type in ADV\n\
  22. Support pulse_rate_range and Target Address AD Type in ADV\n\
  23. Support pulse_rate_range and No Target Address AD Type in ADV\n\
  24. Support No pulse_rate_range and No Target Address AD Type in ADV\n\
  25. Support measurment_position and Target Address AD Type in ADV\n\
  26. Support measurment_position and No Target Address AD Type in ADV\n\
  27. Support No measurment_position and No Target Address AD Type in ADV\n\
\n\
  Your Option ?\n";

/* --------------------------------------------- Functions */
void appl_bps_menu_handler(void)
{
    int choice;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", bps_options);
        CONSOLE_IN("%d", &choice);

        switch (choice)
        {
        case 0U:
            return;

        case 1U:
            CONSOLE_OUT("\nRefreshing ...\n\n");
            break;

        case 10U:
            appl_bps_update_multi_bond_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 11U:
            appl_bps_update_multi_bond_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 12U:
            appl_bps_update_multi_bond_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 13U:
            appl_bps_update_body_movement_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 14U:
            appl_bps_update_body_movement_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 15U:
            appl_bps_update_body_movement_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 16U:
            appl_bps_update_cuff_fit_Detection_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 17U:
            appl_bps_update_cuff_fit_Detection_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 18U:
            appl_bps_update_cuff_fit_Detection_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 19U:
            appl_bps_update_irregular_pulse_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 20U:
            appl_bps_update_irregular_pulse_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 21U:
            appl_bps_update_irregular_pulse_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 22U:
            appl_bps_update_pulse_rate_range_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 23U:
            appl_bps_update_pulse_rate_range_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 24U:
            appl_bps_update_pulse_rate_range_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 25U:
            appl_bps_update_measurment_position_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_TRUE);
            break;

        case 26U:
            appl_bps_update_measurment_position_flag(BT_TRUE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        case 27U:
            appl_bps_update_measurment_position_flag(BT_FALSE);
            appl_bps_update_target_addr_flag(BT_FALSE);
            break;

        default:
            CONSOLE_OUT("Invalid Menu Option Selected!\n");
            break;
        }
    }
}

#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined BPS) */
