/**
*  \file appl_ccp_ce.c
*
*  \brief This file is the Call Control Profile Client reference application
*/

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* --------------------------------------------- Header File Inclusion */
#include "appl_ccp_ce.h"
#include "appl_ccp_utils.h"

#ifdef CCP_CLIENT
/* --------------------------------------------- Global Definitions */

static const UCHAR ga_ccp_ce_appl_menu[] =
"\n\
================ GA CCP Client MENU Operations ================ \n\
    0. Exit \n\
    1. Refresh this Menu. \n\
\n\
================ Profile Management ~ Init ============ \n\
    2. CCP Init \n\
\n\
================ Profile Management ~ Deinit ============ \n\
    3. CCP Shutdown \n\
\n\
================ Context Management ~ Setup ============ \n\
    4. CCP Setup GTBS Context \n\
    5. CCP Discover TBS(s) with peer device \n\
    6. CCP Setup TBS Context \n\
\n\
================ Context Management ~ Release ============ \n\
    7. CCP Release/Close GTBS Context \n\
    8. CCP Release/Close  TBS Context \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
    9. Set TBS Handle \n\
\n\
================ Profile Management (GTBS & TBS) ~ Operations(Read) ============ \n\
   10. CCP Read Bearer Provider Name \n\
   11. CCP Read Bearer UCI \n\
   12. CCP Read Bearer Technology \n\
   13. CCP Read Bearer URI Schemes Supported List \n\
   14. CCP Read Bearer Signal Strength \n\
   15. CCP Read Bearer Signal Strength Reporting Interval \n\
   16. CCP Read Bearer List Current Calls \n\
   17. CCP Read Content Control ID \n\
   18. CCP Read Incoming Call Target Bearer URI \n\
   19. CCP Read Status Flags \n\
   20. CCP Read Call State \n\
   21. CCP Read Control Point Optional Opcodes \n\
   22. CCP Read Incoming Call \n\
   23. CCP Read Call Friendly Name \n\
\n\
================ Profile Management (GTBS & TBS) ~ Operations(Write) ============ \n\
   24. CCP Set Bearer Signal Reporting Interval \n\
\n\
   30. CCP Accept Incoming Call \n\
   31. CCP Terminate Call \n\
   32. CCP Make Local Hold \n\
   33. CCP Make Local Retrieve \n\
   34. CCP Originate \n\
   35. CCP Join \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get GTBS Context Info. \n\
   81. Set GTBS Context Info. \n\
\n\
   82. Get TBS Context Info. \n\
   83. Set TBS Context Info. \n\
\n\
   87. Free Saved GTBS Context Info. \n\
   88. Free Saved TBS Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure Notification \n\
\n\
Your Option? ";

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- External Global Variables */

DECL_STATIC GA_ENDPOINT ga_ccp_peer_bd_addr;

CCP_HANDLE  appl_ccp_gtbs_handle;
CCP_HANDLE  appl_ccp_tbs_handle;

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

DECL_STATIC  CCP_PARAMS appl_gtbs_params[APPL_GTBS_CE_ENITITES];
DECL_STATIC  CCP_PARAMS appl_tbs_params[APPL_TBS_CE_ENITITES];

DECL_STATIC  CCP_CALLER_FRIENDLY_NAME appl_gtbs_caller_friendly_name;
DECL_STATIC  CCP_BRR_LIST_CUR_CALL
    appl_gtbs_brr_cur_call_list[APPL_TBS_CE_BRR_CALL_LIST];
DECL_STATIC  CCP_CALLER_ID  appl_gtbs_incall_trgt_uri;
DECL_STATIC  CCP_CALLER_ID  appl_gtbs_incall_uri;
DECL_STATIC CCP_TERMIN_REASON appl_gtbs_term_reason;
DECL_STATIC  CCP_CALLER_FRIENDLY_NAME appl_tbs_caller_friendly_name;
DECL_STATIC  CCP_BRR_LIST_CUR_CALL
    appl_tbs_brr_cur_call_list[APPL_TBS_CE_BRR_CALL_LIST];
DECL_STATIC  CCP_CALLER_ID  appl_tbs_incall_trgt_uri;
DECL_STATIC  CCP_CALLER_ID  appl_tbs_incall_uri;
DECL_STATIC CCP_TERMIN_REASON appl_tbs_term_reason;

#ifdef CCP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the GTBS Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_ccp_gtbs_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_ccp_gtbs_char_info_ptr;
DECL_STATIC  UINT16              appl_ccp_gtbs_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_ccp_gtbs_range;
DECL_STATIC  GA_CHAR_INFO      * appl_ccp_gtbs_char_info;
DECL_STATIC  CCP_HANDLE          appl_ccp_gtbs_handle_access;

/* Variables to Save the TBS Context Information */
typedef struct _APPL_CCP_TBS_MANAGE_CTX_INFO
{
    GA_BRR_SVC_RANGE  * appl_ccp_tbs_range_ptr;
    GA_CHAR_INFO      * appl_ccp_tbs_char_info_ptr;
    UINT16              appl_ccp_tbs_char_count;
    GA_BRR_SVC_RANGE    appl_ccp_tbs_range;
    GA_CHAR_INFO      * appl_ccp_tbs_char_info;
    CCP_HANDLE          appl_ccp_tbs_handle_access;
}APPL_CCP_TBS_MANAGE_CTX_INFO;

DECL_STATIC APPL_CCP_TBS_MANAGE_CTX_INFO appl_ccp_tbs_manage_ctx_info[CCP_MAX_TBS_CONTEXTS];
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */

/* --------------------------------------------- Functions */
void main_ccp_ce_menu_operations(void)
{
    GA_RESULT   retval;
    CCP_HANDLE  appl_ccp_handle;
    UINT8       conn_dev_index;
    UINT8       i;
    int         choice, menu_choice;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ga_ccp_ce_appl_menu);
        CONSOLE_IN("%d", &choice);

        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            APPL_TRC("GA_ccp_ce_init...\n");
            retval = GA_ccp_ce_init(appl_ccp_ce_callback);
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);

            /* Init GTBS */
            GA_mem_set(&appl_gtbs_params[0U], 0U, sizeof(APPL_GTBS_CE_ENITITES));
            appl_gtbs_params[0U].brr_list_curr_list =
                &appl_gtbs_brr_cur_call_list[0U];
            appl_gtbs_params[0U].incmg_caller_trgt_id =
                &appl_gtbs_incall_trgt_uri;
            appl_gtbs_params[0U].call_state = &appl_gtbs_brr_cur_call_list[0U];
            appl_gtbs_params[0U].incmg_caller_info = &appl_gtbs_incall_uri;
            appl_gtbs_params[0U].frndly_name = &appl_gtbs_caller_friendly_name;
            appl_gtbs_params[0U].term_reason = &appl_gtbs_term_reason;

            /* Init TBS */
            GA_mem_set(&appl_tbs_params[0U], 0U, sizeof(APPL_TBS_CE_ENITITES));
            appl_tbs_params[0U].brr_list_curr_list =
                &appl_tbs_brr_cur_call_list[0U];
            appl_tbs_params[0U].incmg_caller_trgt_id =
                &appl_tbs_incall_trgt_uri;
            appl_tbs_params[0U].call_state = &appl_tbs_brr_cur_call_list[0U];
            appl_tbs_params[0U].incmg_caller_info = &appl_tbs_incall_uri;
            appl_tbs_params[0U].frndly_name = &appl_tbs_caller_friendly_name;
            appl_gtbs_params[0U].term_reason = &appl_tbs_term_reason;

            break;

        case 3:
            APPL_TRC("GA_ccp_ce_shutdown...\n");
            retval = GA_ccp_ce_shutdown();
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            break;

        case 4:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &ga_ccp_peer_bd_addr
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_ccp_peer_bd_addr.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                ga_ccp_peer_bd_addr.bd_type = (UINT8)choice;
            }

            APPL_TRC("GA_ccp_ce_setup_context...\n");
            retval = GA_ccp_ce_setup_context
                     (
                         &ga_ccp_peer_bd_addr,
                         &appl_ccp_gtbs_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_ccp_gtbs_handle:0x%04x\n", appl_ccp_gtbs_handle
                );
            }
            break;

        case 5:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &ga_ccp_peer_bd_addr
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_ccp_peer_bd_addr.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                ga_ccp_peer_bd_addr.bd_type = (UINT8)choice;
            }

            APPL_TRC("GA_ccp_ce_discover_tbs...\n");
            retval = GA_ccp_ce_discover_tbs
                     (
                         &ga_ccp_peer_bd_addr
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            break;

        case 6:
        {
            GA_BRR_SVC_INFO  svc;

            CONSOLE_OUT("Enter Start Handle(in Hex):\n");
            CONSOLE_IN("%x", &choice);
            svc.range.start = (UINT16)(choice);

            CONSOLE_OUT("Enter End Handle(in Hex):\n");
            CONSOLE_IN("%x", &choice);
            svc.range.end = (UINT16)(choice);

            APPL_TRC("GA_ccp_ce_setup_context...\n");
            retval = GA_ccp_ce_setup_tbs_context
                     (
                         &ga_ccp_peer_bd_addr,
                         &svc,
                         &appl_ccp_tbs_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("appl_ccp_tbs_handle:0x%04x\n", appl_ccp_tbs_handle);
            }
        }

            break;

        case 7:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_ccp_ce_release...\n");
                retval = GA_ccp_ce_release(appl_ccp_gtbs_handle);
                APPL_TRC("Retval - ");
                appl_ccp_display_ga_result(retval);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_ccp_ce_close...\n");
                retval = GA_ccp_ce_close(appl_ccp_gtbs_handle);
                APPL_TRC("Retval - ");
                appl_ccp_display_ga_result(retval);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 8:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_ccp_ce_release_tbs...\n");
                retval = GA_ccp_ce_release_tbs(appl_ccp_tbs_handle);
                APPL_TRC("Retval - ");
                appl_ccp_display_ga_result(retval);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_ccp_ce_close_tbs...\n");
                retval = GA_ccp_ce_close_tbs(appl_ccp_tbs_handle);
                APPL_TRC("Retval - ");
                appl_ccp_display_ga_result(retval);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 9:
            CONSOLE_OUT("Enter the TBS Handle: ");
            CONSOLE_IN("%d", &choice);
            if (CCP_MAX_TBS_CONTEXTS <= (UINT8)choice)
            {
                APPL_INF("Invalid handle !\n");
            }
            else
            {
                appl_ccp_tbs_handle = (UINT8)choice;
            }
            break;

        case 10:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_brr_provider_name...\n");
            retval = GA_ccp_ce_get_brr_provider_name
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 11:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_brr_uci...\n");
            retval = GA_ccp_ce_get_brr_uci
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 12:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_brr_technology...\n");
            retval = GA_ccp_ce_get_brr_technology
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 13:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_brr_uci_schemes_supported_list...\n");
            retval = GA_ccp_ce_get_brr_uci_schemes_supported_list
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 14:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_signal_strength...\n");
            retval = GA_ccp_ce_get_signal_strength
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 15:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_signal_reporting_interval...\n");
            retval = GA_ccp_ce_get_signal_reporting_interval
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 16:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);

            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_brr_list_cur_calls...\n");
            retval = GA_ccp_ce_get_brr_list_cur_calls
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 17:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);

            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_content_control_id...\n");
            retval = GA_ccp_ce_get_content_control_id
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 18:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);

            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_incall_trgt_brr_uri...\n");
            retval = GA_ccp_ce_get_incall_trgt_brr_uri
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 19:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_status_flag...\n");
            retval = GA_ccp_ce_get_status_flag
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;



        case 20:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_call_state...\n");
            retval = GA_ccp_ce_get_call_state
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 21:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_call_cp_optional_opcodes...\n");
            retval = GA_ccp_ce_get_call_cp_optional_opcodes
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 22:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_incoming_call...\n");
            retval = GA_ccp_ce_get_incoming_call
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 23:
        {
            CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            APPL_TRC("GA_ccp_ce_get_call_friendly_name...\n");
            retval = GA_ccp_ce_get_call_friendly_name
                     (
                         appl_ccp_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 24:
        {
            UCHAR  sig_reporting_intrvl;
            UCHAR  wt_req_type;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Select the below options\n");
            CONSOLE_OUT("0 -> Write Request\n");
            CONSOLE_OUT("1 -> Write Command\n");
            CONSOLE_IN("%d", &choice);
            wt_req_type = (UCHAR)choice;

            CONSOLE_OUT("Enter the Signal Reporting Interval(0-255)");
            CONSOLE_IN("%d", &choice);
            sig_reporting_intrvl = (UCHAR)choice;

            APPL_TRC("GA_ccp_ce_set_signal_reporting_interval...\n");
            retval = GA_ccp_ce_set_signal_reporting_interval
                     (
                         appl_ccp_handle,
                         wt_req_type,
                         &sig_reporting_intrvl,
                         sizeof(sig_reporting_intrvl)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
        break;

        case 30:
        {
            UCHAR  call_index;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Enter Call Index(in Hex):");
            CONSOLE_IN("%x", &choice);
            call_index = (UCHAR )choice;

            APPL_TRC("GA_ccp_ce_accept_call...\n");
            retval = GA_ccp_ce_accept_call
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &call_index,
                         sizeof(call_index)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 31:
        {
            UCHAR  call_index;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Enter Call Index(in Hex):");
            CONSOLE_IN("%x", &choice);
            call_index = (UCHAR)choice;

            APPL_TRC("GA_ccp_ce_terminate_call...\n");
            retval = GA_ccp_ce_terminate_call
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &call_index,
                         sizeof(call_index)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 32:
        {
            UCHAR  call_index;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Enter Call Index(in Hex):");
            CONSOLE_IN("%x", &choice);
            call_index = (UCHAR)choice;

            APPL_TRC("GA_ccp_ce_call_local_hold...\n");
            retval = GA_ccp_ce_call_local_hold
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &call_index,
                         sizeof(call_index)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 33:
        {
            UCHAR  call_index;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Enter Call Index(in Hex):");
            CONSOLE_IN("%x", &choice);
            call_index = (UCHAR)choice;

            APPL_TRC("GA_ccp_ce_call_local_retrieve...\n");
            retval = GA_ccp_ce_call_local_retrieve
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &call_index,
                         sizeof(call_index)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 34:
        {
            UCHAR  call_uri[MAX_TBS_URI_LEN];
            UCHAR  call_uri_len;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            EM_str_copy
            (
                &call_uri[0U],
                SAMPLE_URI
            );
            call_uri_len = (UCHAR )(EM_str_len(call_uri) + 1U);

            APPL_TRC("GA_ccp_ce_originate_call...\n");
            retval = GA_ccp_ce_originate_call
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &call_uri[0U],
                         call_uri_len
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 35:
        {
            UCHAR  join_calls_count;
            UCHAR  join_call_info[10U];

            i = 0U;

            CONSOLE_OUT("Select GTBS/TBS(0/1):");
            CONSOLE_IN("%x", &choice);
            if (0 == choice)
            {
                appl_ccp_handle = appl_ccp_gtbs_handle;
            }
            else
            {
                appl_ccp_handle = appl_ccp_tbs_handle;
            }

            CONSOLE_OUT("Enter no. of calls to join:");
            CONSOLE_IN("%d", &choice);
            join_calls_count = (UCHAR)choice;

            for (i = 0U; i < join_calls_count; i++)
            {
                CONSOLE_OUT("Enter 0x%02x th Call Index(in Hex):", i);
                CONSOLE_IN("%x", &choice);

                join_call_info[i] = (UCHAR)choice;
            }

            APPL_TRC("GA_ccp_ce_join_call...\n");
            retval = GA_ccp_ce_join_call
                     (
                         appl_ccp_handle,
                         GA_TRUE,
                         &join_call_info,
                         join_calls_count
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
        }
            break;

        case 80:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
        {
            appl_ccp_gtbs_handle_access = appl_ccp_gtbs_handle;

            APPL_TRC("GA_ccp_ce_get_context_info ...\n");
            retval = GA_ccp_ce_get_context_info
                     (
                         &ga_ccp_peer_bd_addr,
                         &appl_ccp_gtbs_handle_access,
                         &appl_ccp_gtbs_range_ptr,
                         &appl_ccp_gtbs_char_info_ptr,
                         &appl_ccp_gtbs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);

            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_ccp_gtbs_handle_access:0x%02x\n",
                    appl_ccp_gtbs_handle_access
                );
                APPL_TRC
                (
                    "appl_ccp_gtbs_char_count:0x%02x\n",
                    appl_ccp_gtbs_char_count
                );

                appl_ccp_gtbs_range = *appl_ccp_gtbs_range_ptr;

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_ccp_gtbs_char_info = GA_alloc_mem(appl_ccp_gtbs_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_ccp_gtbs_char_info)
                {
                    GA_mem_copy
                    (
                        appl_ccp_gtbs_char_info,
                        appl_ccp_gtbs_char_info_ptr,
                        appl_ccp_gtbs_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
        }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 81:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
        {
            if (NULL == appl_ccp_gtbs_char_info)
            {
                APPL_TRC("appl_ccp_gtbs_char_info is NULL \n");
                APPL_TRC("Call GA_ccp_ce_get_context_info\n");
                break;
            }

            appl_ccp_gtbs_handle_access = 0U;
            CCP_CE_SET_HANDLE_GTBS(appl_ccp_gtbs_handle_access);
            appl_ccp_gtbs_range_ptr = &appl_ccp_gtbs_range;
            appl_ccp_gtbs_char_info_ptr = appl_ccp_gtbs_char_info;

            APPL_TRC("GA_ccp_ce_set_context_info ...\n");
            retval = GA_ccp_ce_set_context_info
                     (
                         &ga_ccp_peer_bd_addr,
                         &appl_ccp_gtbs_handle_access,
                         &appl_ccp_gtbs_range_ptr,
                         &appl_ccp_gtbs_char_info_ptr,
                         &appl_ccp_gtbs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            if (GA_SUCCESS == retval)
            {
                appl_ccp_gtbs_handle = appl_ccp_gtbs_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_ccp_gtbs_char_info);
                appl_ccp_gtbs_char_info = NULL;
            }
        }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 82:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
        {
            CONSOLE_OUT("Enter the TBS Instance: ");
            CONSOLE_IN("%d", &choice);
            appl_ccp_handle = (UINT8)choice;
            if (CCP_MAX_TBS_CONTEXTS > appl_ccp_handle)
            {
                appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_handle_access =
                    appl_ccp_handle;

                APPL_TRC("GA_ccp_ce_get_context_info ...\n");
                retval = GA_ccp_ce_get_context_info
                         (
                             &ga_ccp_peer_bd_addr,
                             &appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_handle_access,
                             &appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_range_ptr,
                             &appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_info_ptr,
                             &appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_count
                         );
                APPL_TRC("Retval - ");
                appl_ccp_display_ga_result(retval);

                if (GA_SUCCESS == retval)
                {
                    APPL_TRC
                    (
                        "appl_ccp_tbs_handle_access:0x%02x\n",
                        appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_handle_access
                    );
                    APPL_TRC
                    (
                        "appl_ccp_tbs_char_count:0x%02x\n",
                        appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_count
                    );

                    appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_range =
                        *appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_range_ptr;

                    /**
                     *  Reference to allocate to save locally.
                     *  This can be written to persistent and retrieved
                     *  upon the next connection if bonded device
                     */
                    appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_info =
                        GA_alloc_mem
                        (
                            appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_count * sizeof(GA_CHAR_INFO)
                        );
                    if (NULL != appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_info)
                    {
                        GA_mem_copy
                        (
                            appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_info,
                            appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_info_ptr,
                            appl_ccp_tbs_manage_ctx_info[appl_ccp_handle].appl_ccp_tbs_char_count * sizeof(GA_CHAR_INFO)
                        );
                    }
                }
            }
            else
            {
                APPL_ERR("Invalid TBS Instance !\n");
            }
        }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 83:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
        {
            for (i = 0U; i < CCP_MAX_TBS_CONTEXTS; i++)
            {
                if (NULL == appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info)
                {
                    APPL_TRC("appl_ccp_tbs_char_info[%d] is NULL \n", i);
                    APPL_TRC("Call GA_ccp_ce_get_context_info\n");
                }
                else
                {
                    appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_handle_access = 0U;
                    appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_range_ptr =
                        &appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_range;
                    appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info_ptr =
                        appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info;

                    APPL_TRC("GA_ccp_ce_set_context_info ...\n");
                    retval = GA_ccp_ce_set_context_info
                             (
                                 &ga_ccp_peer_bd_addr,
                                 &appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_handle_access,
                                 &appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_range_ptr,
                                 &appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info_ptr,
                                 &appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_count
                             );
                    APPL_TRC("Retval - ");
                    appl_ccp_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        appl_ccp_tbs_handle = appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_handle_access;

                        /* Free the allocated memory */
                        if (NULL != appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info)
                        {
                            GA_free_mem(appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info);
                            appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info = NULL;
                        }
                    }
                }
            }
        }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 87:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_ccp_ce_set_context_info() is NOT called after
             * GA_ccp_ce_get_context_info().
             */

            /* Free the allocated memory for GTBS char. info. */
            if (NULL != appl_ccp_gtbs_char_info)
            {
                APPL_TRC("Freeing appl_ccp_gtbs_char_info ...\n");
                GA_free_mem(appl_ccp_gtbs_char_info);
                appl_ccp_gtbs_char_info = NULL;
            }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 88:
#ifdef CCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_ccp_ce_set_context_info() is NOT called after
             * GA_ccp_ce_get_context_info().
             */

            for (i = 0U; i < CCP_MAX_TBS_CONTEXTS; i++)
            {
                /* Free the allocated memory for TBS char. info. */
                if (NULL != appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info)
                {
                    APPL_TRC("Freeing appl_ccp_tbs_char_info ...\n");
                    GA_free_mem
                    (
                        appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info
                    );
                    appl_ccp_tbs_manage_ctx_info[i].appl_ccp_tbs_char_info = NULL;
                }
            }
#else /* CCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
        {
#ifdef CCP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_ccp_ce_update_ntf_configuration...\n");
            retval = GA_ccp_ce_update_ntf_configuration
                     (
                        (CCP_CHAR_CONFIG(CCP_CHAR_ID_BRR_PROVIDER_NAME) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_BRR_TECHNOLOGY) |
                         /* CCP_CHAR_CONFIG(CCP_CHAR_ID_BRR_URI_SCHMS_SUPPORT_LIST) | */
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_BRR_SIGSTRENGTH) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_BRR_LIST_CUR_CALLS) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_INCOMING_CALL_TARGET_BRR_URI) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_CALL_STATE) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_CALL_CP) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_TERMINATION_REASON) |
                         CCP_CHAR_CONFIG(CCP_CHAR_ID_INCOMING_CALL)
                         /* CCP_CHAR_CONFIG(CCP_CHAR_ID_CALL_FRIENDLY_NAME) */)
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
#else /* CCP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("CCP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* CCP_SUPPORT_CONFIG_SELECTION */
        }
        break;

        default:
            APPL_INF("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

GA_RESULT appl_ccp_ce_callback
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void        * event_data,
              /* IN */ UINT16        event_datalen
          )
{
    GA_RESULT         retval;
    UCHAR             srvc_handle;
    UCHAR             i;
    UINT16            offset;
    UCHAR             rx_datalen;
    UCHAR             j;
    GA_BRR_SVC_INFO * svc;

    CCP_EVENT_INFO  * appl_event_info;

    GA_IGNORE_UNUSED_PARAM(event_datalen);

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    if (NULL == event_data)
    {
        APPL_ERR("event_data is NULL !\n");
        return GA_FAILURE;
    }

    /* Initialize */
    retval = GA_SUCCESS;
    srvc_handle = 0U;
    offset = 0U;

    appl_event_info = (CCP_EVENT_INFO *)event_data;
    svc = (GA_BRR_SVC_INFO *)appl_event_info->data;

    GA_CB_ENTRY("CCP Client");

    APPL_TRC("[Profile]     : CCP (0x%4X)\n",GA_SERVICE_GTBS);
    APPL_TRC("[SECTION]     : CCP CLIENT EVENTS\n");

    switch (event_type)
    {
    case CCP_CE_SETUP_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_SETUP_CNF (0x%02X)\n",
            event_type
        );

        if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
        {
            srvc_handle = ((appl_event_info->srvc_handle) &
                           (~CCP_CE_GTBS_ENABLE));
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(GTBS)\n",
                appl_event_info->srvc_handle
            );
        }
        else
        {
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(TBS)\n",
                appl_event_info->srvc_handle
            );
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
        if(0U == appl_event_info->datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case CCP_CE_RELEASE_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RELEASE_CNF (0x%02X)\n",
            event_type
        );

        if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
        {
            srvc_handle = ((appl_event_info->srvc_handle) &
                           (~CCP_CE_GTBS_ENABLE));
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(GTBS)\n",
                appl_event_info->srvc_handle
            );
        }
        else
        {
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(TBS)\n",
                appl_event_info->srvc_handle
            );
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
        if(0U == appl_event_info->datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case CCP_CE_SETUP_TBS_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_SETUP_TBS_CNF (0x%02X)\n",
            event_type
        );

        if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
        {
            srvc_handle = ((appl_event_info->srvc_handle) &
                           (~CCP_CE_GTBS_ENABLE));
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(GTBS)\n",
                appl_event_info->srvc_handle
            );
        }
        else
        {
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(TBS)\n",
                appl_event_info->srvc_handle
            );
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
        if(0U == appl_event_info->datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case CCP_CE_RELEASE_TBS_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RELEASE_TBS_CNF (0x%02X)\n",
            event_type
        );

        if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
        {
            srvc_handle = ((appl_event_info->srvc_handle) &
                           (~CCP_CE_GTBS_ENABLE));
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(GTBS)\n", appl_event_info->srvc_handle
            );
        }
        else
        {
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(TBS)\n",
                appl_event_info->srvc_handle
            );
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
        if(0U == appl_event_info->datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case CCP_CE_DISCOVER_TBS_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_DISCOVER_TBS_CNF (0x%02X)\n",
            event_type
        );

        if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
        {
            srvc_handle = ((appl_event_info->srvc_handle) &
                           (~CCP_CE_GTBS_ENABLE));
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(GTBS)\n", appl_event_info->srvc_handle
            );
        }
        else
        {
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X(TBS)\n",
                appl_event_info->srvc_handle
            );
        }
        if((NULL != svc) && (GA_CONTINUE == event_status))
        {
            GA_FORMAT_DISPLAY();
            appl_ga_utils_display_svc_uuid(svc->uuid);
            GA_FORMAT_DISPLAY();
            appl_ga_utils_display_svc_range(svc->range);
        }
        else
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
            if(0U == appl_event_info->datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case CCP_CE_RD_BRR_PROVIDER_NAME_CNF:
    case CCP_CE_BRR_PROVIDER_NAME_NTF:
        if (CCP_CE_RD_BRR_PROVIDER_NAME_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_BRR_PROVIDER_NAME_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_BRR_PROVIDER_NAME_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_BRR_PROVIDER_NAME_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer Provider Name (0x%04X)\n",
                GA_CHAR_TBS_BRR_PROVIDER_NAME
            );
            GA_FORMAT_DISPLAY();

            /* TODO */
            if (APPL_GTBS_CE_ENITITES > srvc_handle)
            {
                GA_mem_copy
                (
                    appl_gtbs_params[srvc_handle].bearer_pname,
                    (UCHAR *)appl_event_info->data,
                    appl_event_info->datalen
                );
                appl_gtbs_params[srvc_handle].bearer_pname_len =
                    (UCHAR )appl_event_info->datalen;

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer Provider Name: %s\n",
                        (UCHAR )appl_event_info->datalen,
                        appl_gtbs_params[srvc_handle].bearer_pname
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type: 0x%02X !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_UCI_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RD_BRR_UCI_CNF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer UCI (0x%04X)\n",
                GA_CHAR_TBS_BRR_UCI
            );
            GA_FORMAT_DISPLAY();

            if (APPL_GTBS_CE_ENITITES > srvc_handle)
            {
                GA_mem_copy
                (
                    appl_gtbs_params[srvc_handle].bearer_uci,
                    (UCHAR *)appl_event_info->data,
                    appl_event_info->datalen
                );
                appl_gtbs_params[srvc_handle].bearer_uci_len =
                    (UCHAR )appl_event_info->datalen;

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer UCI: %s\n",
                        appl_event_info->datalen,
                        appl_gtbs_params[srvc_handle].bearer_uci
                    );
                }
            }
            else
            {
               APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_TECH_CNF:
    case CCP_CE_BRR_TECH_NTF:
        if (CCP_CE_RD_BRR_TECH_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_BRR_TECH_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_BRR_TECH_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_BRR_TECH_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer Technology (0x%04X)\n",
                GA_CHAR_TBS_BRR_TECHNOLOGY
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                appl_gtbs_params[srvc_handle].brr_tech =
                    ((UCHAR *)appl_event_info->data)[0U];

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer Technology: ",
                        appl_event_info->datalen
                    );
                    appl_ccp_utils_parse_and_display_ccp_brr_tech
                    (
                        appl_gtbs_params[srvc_handle].brr_tech
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF:
    case CCP_CE_BRR_URI_SCHMS_LIST_NTF:
        if (CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_BRR_URI_SCHMS_LIST_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer URI Schemes Supported List (0x%04X)\n",
                GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                GA_mem_copy
                (
                    appl_gtbs_params[srvc_handle].brr_uri_pfix_supp_list,
                    (UCHAR *)appl_event_info->data,
                    appl_event_info->datalen
                );
                appl_gtbs_params[srvc_handle].brr_uri_pfix_supp_list_len =
                    (UCHAR )appl_event_info->datalen;

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer URI Schemes Supported List: %s\n",
                        appl_event_info->datalen,
                        appl_gtbs_params[srvc_handle].brr_uri_pfix_supp_list
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_SIG_STRNTH_CNF:
    case CCP_CE_BRR_SIG_STRNTH_NTF:
        if (CCP_CE_RD_BRR_SIG_STRNTH_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_BRR_SIG_STRNTH_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_BRR_SIG_STRNTH_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_BRR_SIG_STRNTH_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer Signal Strength (0x%04X)\n",
                GA_CHAR_TBS_BRR_SIGSTRENGTH
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                appl_gtbs_params[srvc_handle].brr_sig_strength =
                    ((UCHAR *)appl_event_info->data)[0U];

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer Signal Strength: ",
                        appl_event_info->datalen
                    );
                    appl_ccp_utils_parse_and_display_ccp_brr_signal_strength
                    (
                        appl_gtbs_params[srvc_handle].brr_sig_strength
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer Signal Strength Reporting Interval (0x%04X)\n",
                GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                appl_gtbs_params[srvc_handle].brr_sig_strength_reptg_intval =
                    ((UCHAR *)appl_event_info->data)[0U];
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Bearer Signal Strength Reporting Interval: 0x%02X\n",
                        appl_event_info->datalen,
                        appl_gtbs_params[srvc_handle].brr_sig_strength_reptg_intval
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF:
    case CCP_CE_BRR_LIST_CUR_CALLS_NTF:
        if (CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_BRR_LIST_CUR_CALLS_NTF (0x%02X)\n",
                event_type
            );
        }

        offset = 0U;

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer List Current Calls (0x%04X)\n",
                GA_CHAR_TBS_BRR_LIST_CUR_CALLS
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                rx_datalen = (UCHAR )appl_event_info->datalen;

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if (0U == rx_datalen)
                {
                    APPL_TRC("Data: NULL\n");
                    APPL_TRC("====== No Call are present ====== \n");
                    break;
                }
                else
                {
                    for (i = 0U; i < APPL_TBS_CE_BRR_CALL_LIST; i++)
                    {
                        APPL_TRC("%u.  ", (i + 1U));
                        /* Item Length */
                        appl_gtbs_params[srvc_handle].brr_list_curr_list[i].list_item_length =
                            ((UCHAR *)appl_event_info->data)[offset];
                        APPL_TRC
                        (
                            "Len: 0x%02X,  List Item Length: 0x%02X\n",
                            (UCHAR )sizeof(appl_gtbs_params[srvc_handle].brr_list_curr_list[i].list_item_length),
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].list_item_length
                        );
                        offset++;

                        /* Call Index */
                        appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_index =
                            ((UCHAR *)appl_event_info->data)[offset];
                        APPL_TRC
                        (
                            "    Len: 0x%02X,  Call Index: 0x%02X\n",
                            (UCHAR )sizeof(appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_index),
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_index
                        );
                        offset++;

                        /* Call State */
                        appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_state =
                            ((UCHAR *)appl_event_info->data)[offset];
                        APPL_TRC
                        (
                            "    Len: 0x%02X,  Call State: ",
                            (UCHAR )sizeof(appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_state)
                        );
                        appl_ccp_utils_parse_and_display_ccp_call_state
                        (
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_state
                        );
                        offset++;

                        /* Call Flags */
                        appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_flags =
                            ((UCHAR *)appl_event_info->data)[offset];
                        APPL_TRC
                        (
                            "    Len: 0x%02X,  Call Flags: ",
                            (UCHAR )sizeof(appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_flags)
                        );
                        appl_ccp_utils_parse_and_display_ccp_call_flags
                        (
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].call_flags
                        );
                        offset++;

                        /* URI Length */
                        appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri_len =
                            (appl_gtbs_params[srvc_handle].brr_list_curr_list[i].list_item_length - 3U);

                        /* Call URI */
                        GA_mem_copy
                        (
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri,
                            &((UCHAR *)appl_event_info->data)[offset],
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri_len
                        );
                        APPL_TRC
                        (
                            "    Len: 0x%02X,  URI: ",
                            (UCHAR )appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri_len
                        );
                        for (j = 0U;
                            j < appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri_len;
                            j++)
                        {
                            APPL_TRC
                            (
                                "%c",
                                appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri[j]
                            );
                        }
                        APPL_TRC("\n\n");
                        offset +=
                            appl_gtbs_params[srvc_handle].brr_list_curr_list[i].uri_len;

                        if (offset >= rx_datalen)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_CNTNT_CTRL_ID_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RD_CNTNT_CTRL_ID_CNF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Content Control ID (0x%04X)\n",
                GA_CHAR_CONTENT_CONTROL_ID
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &appl_gtbs_params[srvc_handle].content_control_id,
                    (UCHAR *)appl_event_info->data
                );

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Content Control ID: 0x%02X\n",
                        appl_event_info->datalen,
                        appl_gtbs_params[srvc_handle].content_control_id
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_STATUS_FLAGS_CNF:
    case CCP_CE_STATUS_FLAGS_NTF:
        if (CCP_CE_RD_STATUS_FLAGS_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_STATUS_FLAGS_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_STATUS_FLAGS_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_STATUS_FLAGS_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Status Flags (0x%04X)\n",
                GA_CHAR_TBS_STATUS_FLAGS
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &appl_gtbs_params[srvc_handle].status_flags,
                    (UCHAR *)appl_event_info->data
                );

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Status Flags: ",
                        appl_event_info->datalen
                    );
                    appl_ccp_utils_parse_and_display_ccp_status_flags
                    (
                        appl_gtbs_params[srvc_handle].status_flags
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF:
    case CCP_CE_INCALL_TRGT_BRR_URI_NTF:
        if (CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_INCALL_TRGT_BRR_URI_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call State (0x%04X)\n",
                GA_CHAR_TBS_CALL_STATE
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    /* Call Index */
                    appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->call_indx =
                        ((UCHAR *)appl_event_info->data)[offset];
                    offset += 1U;
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Call Index: 0x%02X\n",
                        (UCHAR )sizeof(appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->call_indx),
                        appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->call_indx
                    );

                    /* URI */
                    appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len =
                        (UCHAR )(appl_event_info->datalen - 1U);
                    if (MAX_TBS_URI_LEN < appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len)
                    {
                        appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len =
                            MAX_TBS_URI_LEN;
                    }

                    GA_mem_copy
                    (
                        appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri,
                        &((UCHAR *)appl_event_info->data)[offset],
                        appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len
                    );

                    APPL_TRC
                    (
                        "Len: 0x%02X,  Incoming Call Target URI: ",
                        appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len
                    );
                    for (j = 0U;
                        j < appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri_len;
                        j++)
                    {
                        APPL_TRC
                        (
                            "%c",
                            appl_gtbs_params[srvc_handle].incmg_caller_trgt_id->uri[j]
                        );
                    }
                    APPL_TRC("\n");
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_CALL_STATE_CNF:
    case CCP_CE_CALL_STATE_NTF:
        if (CCP_CE_RD_CALL_STATE_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_CALL_STATE_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_CALL_STATE_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_CALL_STATE_NTF (0x%02X)\n",
                event_type
            );
        }

        offset = 0U;
        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call State (0x%04X)\n",
                GA_CHAR_TBS_CALL_STATE
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                rx_datalen = (UCHAR )appl_event_info->datalen;

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if (0U == rx_datalen)
                {
                    APPL_TRC("Data: NULL\n");
                    APPL_TRC("====== No Active Calls are present ======\n");
                    break;
                }

                for (i = 0U; i < APPL_TBS_CE_BRR_CALL_LIST; i++)
                {
                    APPL_TRC("%u.  ", (i + 1U));
                    appl_gtbs_params[srvc_handle].call_state[i].call_index =
                        ((UCHAR *)appl_event_info->data)[offset];
                    offset++;
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Call Index: 0x%02X\n",
                        (UCHAR)sizeof(appl_gtbs_params[srvc_handle].call_state[i].call_index),
                        appl_gtbs_params[srvc_handle].call_state[i].call_index
                    );

                    appl_gtbs_params[srvc_handle].call_state[i].call_state =
                        ((UCHAR *)appl_event_info->data)[offset];
                    offset++;
                    APPL_TRC
                    (
                        "    Len: 0x%02X,  State: ",
                        (UCHAR)sizeof(appl_gtbs_params[srvc_handle].call_state[i].call_state)
                    );
                    appl_ccp_utils_parse_and_display_ccp_call_state
                    (
                        appl_gtbs_params[srvc_handle].call_state[i].call_state
                    );

                    appl_gtbs_params[srvc_handle].call_state[i].call_flags =
                        ((UCHAR *)appl_event_info->data)[offset];
                    offset++;
                    APPL_TRC
                    (
                        "    Len: 0x%02X,  Call Flags: ",
                        (UCHAR)sizeof(appl_gtbs_params[srvc_handle].call_state[i].call_flags)
                    );
                    appl_ccp_utils_parse_and_display_ccp_call_flags
                    (
                        appl_gtbs_params[srvc_handle].call_state[i].call_flags
                    );
                    APPL_TRC("\n\n");

                    if (offset >= rx_datalen)
                    {
                        break;
                    }
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call Control Point Optional Opcodes (0x%04X)\n",
                GA_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &appl_gtbs_params[srvc_handle].opt_cp_opcodes,
                    (UCHAR *)appl_event_info->data
                );

                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Call Control Point Optional Opcodes: ",
                        (UCHAR )sizeof(appl_gtbs_params[srvc_handle].opt_cp_opcodes)
                    );
                    appl_ccp_utils_parse_and_display_ccp_cp_opt_opcodes
                    (
                        appl_gtbs_params[srvc_handle].opt_cp_opcodes
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_CALL_CP_NTF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_CALL_CP_NTF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call Control Point (0x%04X)\n",
                GA_CHAR_TBS_CALL_CP
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    appl_ccp_ce_parse_and_display_cp_ntf_data(appl_event_info);
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_TERMINATION_REASON_NTF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_TERMINATION_REASON_NTF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Termination Reason (0x%04X)\n",
                GA_CHAR_TBS_TERMINATION_REASON
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
                else
                {
                    appl_gtbs_params[srvc_handle].term_reason->call_indx =
                        ((UCHAR *)appl_event_info->data)[offset];
                    offset += 1U;
                    appl_gtbs_params[srvc_handle].term_reason->reason =
                        ((UCHAR *)appl_event_info->data)[offset];
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Call Index: 0x%02X\n",
                        (UCHAR )sizeof(appl_gtbs_params[srvc_handle].term_reason->call_indx),
                        appl_gtbs_params[srvc_handle].term_reason->call_indx
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Reason Code: 0x%02X\n",
                        (UCHAR )sizeof(appl_gtbs_params[srvc_handle].term_reason->reason),
                        appl_gtbs_params[srvc_handle].term_reason->reason
                    );
                    appl_ccp_ce_parse_and_display_termination_reason
                    (
                        appl_gtbs_params[srvc_handle].term_reason->reason
                    );
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_INCOMING_CALL_CNF:
    case CCP_CE_INCOMING_CALL_NTF:
        if (CCP_CE_RD_INCOMING_CALL_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_INCOMING_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_INCOMING_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_INCOMING_CALL_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Incoming Call (0x%04X)\n",
                GA_CHAR_TBS_INCOMING_CALL
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);

                if (0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                    break;
                }

                /* Call Index */
                appl_gtbs_params[srvc_handle].incmg_caller_info->call_indx =
                    ((UCHAR *)appl_event_info->data)[offset];
                offset += 1U;
                APPL_TRC
                (
                    "Len: 0x%02X,  Call Index: 0x%02X\n",
                    (UCHAR)sizeof(appl_gtbs_params[srvc_handle].incmg_caller_info->call_indx),
                    appl_gtbs_params[srvc_handle].incmg_caller_info->call_indx
                );

                if (1U < appl_event_info->datalen)
                {
                    /* URI */
                    appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len =
                        (UCHAR )(appl_event_info->datalen - 1U);
                    if (MAX_TBS_URI_LEN < appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len)
                    {
                        appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len =
                            MAX_TBS_URI_LEN;
                    }

                    GA_mem_copy
                    (
                        appl_gtbs_params[srvc_handle].incmg_caller_info->uri,
                        &((UCHAR *)appl_event_info->data)[offset],
                        appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len
                    );

                    APPL_TRC
                    (
                        "Len: 0x%02X,  URI: ",
                        appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len
                    );
                    for (j = 0U;
                        j < appl_gtbs_params[srvc_handle].incmg_caller_info->uri_len;
                        j++)
                    {
                        APPL_TRC
                        (
                            "%c",
                            appl_gtbs_params[srvc_handle].incmg_caller_info->uri[j]
                        );
                    }
                    APPL_TRC("\n");
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_RD_CALL_FRIENDLY_NAME_CNF:
    case CCP_CE_CALL_FRIENDLY_NAME_NTF:
        if (CCP_CE_RD_CALL_FRIENDLY_NAME_CNF == event_type)
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : CCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_RD_CALL_FRIENDLY_NAME_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_RD_CALL_FRIENDLY_NAME_CNF (0x%02X)\n",
                event_type
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_CALL_FRIENDLY_NAME_NTF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call Friendly Name (0x%04X)\n",
                GA_CHAR_TBS_CALL_FRIENDLY_NAME
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if (0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                    break;
                }
                /* Call Index */
                appl_gtbs_params[srvc_handle].frndly_name->call_indx =
                    ((UCHAR *)appl_event_info->data)[offset];
                offset += 1U;
                APPL_TRC
                (
                    "Len: 0x%02X,  Call Index: 0x%02X\n",
                    (UCHAR)sizeof(appl_gtbs_params[srvc_handle].frndly_name->call_indx),
                    appl_gtbs_params[srvc_handle].frndly_name->call_indx
                );

                /* Call Friendly Name */
                appl_gtbs_params[srvc_handle].frndly_name->name_len =
                    (UCHAR )(appl_event_info->datalen - 1U);
                if (MAX_TBS_URI_LEN < appl_gtbs_params[srvc_handle].frndly_name->name_len)
                {
                    appl_gtbs_params[srvc_handle].frndly_name->name_len =
                        MAX_TBS_CALLER_NAME_LEN;
                }

                GA_mem_copy
                (
                    appl_gtbs_params[srvc_handle].frndly_name->name,
                    &((UCHAR *)appl_event_info->data)[offset],
                    appl_gtbs_params[srvc_handle].frndly_name->name_len
                );

                APPL_TRC
                (
                    "Len: 0x%02X,  Friendly Name: ",
                    appl_gtbs_params[srvc_handle].frndly_name->name_len
                );
                for (j = 0U;
                    j < appl_gtbs_params[srvc_handle].frndly_name->name_len;
                    j++)
                {
                    APPL_TRC
                    (
                        "%c",
                        appl_gtbs_params[srvc_handle].frndly_name->name[j]
                    );
                }
                APPL_TRC("\n");
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF (0x%02X)\n",
            event_type
        );

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Bearer Signal Strength Reporting Interval (0x%04X)\n",
                GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    case CCP_CE_ACCEPT_CALL_CNF: /* Fall-Through */
    case CCP_CE_TERMINATE_CALL_CNF: /* Fall-Through */
    case CCP_CE_LOCAL_HOLD_CALL_CNF: /* Fall-Through */
    case CCP_CE_LOCAL_RETRIEVE_CALL_CNF: /* Fall-Through */
    case CCP_CE_ORGINATE_CALL_CNF: /* Fall-Through */
    case CCP_CE_JOIN_CALL_CNF:
        APPL_TRC("[SUB-SECTION] : CCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        if (CCP_CE_ACCEPT_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_ACCEPT_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else if (CCP_CE_TERMINATE_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_TERMINATE_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else if (CCP_CE_LOCAL_HOLD_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_LOCAL_HOLD_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else if (CCP_CE_LOCAL_RETRIEVE_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_LOCAL_RETRIEVE_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else if (CCP_CE_ORGINATE_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_ORGINATE_CALL_CNF (0x%02X)\n",
                event_type
            );
        }
        else if (CCP_CE_JOIN_CALL_CNF == event_type)
        {
            APPL_TRC
            (
                "[Event]       : CCP_CE_JOIN_CALL_CNF (0x%02X)\n",
                event_type
            );
        }

        if ((CCP_SVS_TYPE_GTBS == appl_event_info->srvc_type) ||
            (CCP_SVS_TYPE_TBS == appl_event_info->srvc_type))
        {
            if (CCP_CE_IS_HANDLE_GTBS(appl_event_info->srvc_handle))
            {
                srvc_handle = (appl_event_info->srvc_handle & 0x7FU);
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(GTBS)\n",
                    appl_event_info->srvc_handle
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Service Cntx]: 0x%02X(TBS)\n",
                    appl_event_info->srvc_handle
                );
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Call Control Point (0x%04X)\n",
                GA_CHAR_TBS_CALL_CP
            );
            GA_FORMAT_DISPLAY();

            if (APPL_TBS_CE_ENITITES > srvc_handle)
            {
                APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
                if(0U == appl_event_info->datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
            }
            else
            {
                APPL_ERR("Incorrect GTBS Handle: 0x%02X !\n", srvc_handle);
            }
        }
        else
        {
            APPL_ERR
            (
                "Incorrect TBS Service Type:0x%02x !\n",
                appl_event_info->srvc_type
            );
        }
        break;

    default:
        break;
    }

    GA_CB_EXIT();

    return retval;
}

void appl_ccp_ce_parse_and_display_termination_reason(UCHAR  termination_reason)
{
    switch(termination_reason)
    {
        case CCP_CALL_TER_RSN_URI_WRONG:
            APPL_TRC
            (
                "The URI value used to originate a call was formed improperly"
            );
            break;

        case CCP_CALL_TER_RSN_CALL_FAILED:
            APPL_TRC("The call failed");
            break;

        case CCP_CALL_TER_RSN_R_ENDED_CALL:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_SRV_ENDED_CALL:
            APPL_TRC("The call ended from the server");
            break;

        case CCP_CALL_TER_RSN_LINE_BUSY:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_NWT_CNGSTN:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_CLI_TER_CALL:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_CLI_NO_SERVICE:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_CLI_NO_ANSWER:
            APPL_TRC("The remote party ended the call");
            break;

        case CCP_CALL_TER_RSN_UNSPECIFIED:
            APPL_TRC("The remote party ended the call");
            break;

        default:
            APPL_INF("RFU");
    }
    APPL_TRC(" (0x%02X)\n", termination_reason);
}

void appl_ccp_ce_parse_and_display_cp_ntf_data(CCP_EVENT_INFO * appl_event_info)
{
    UCHAR   cp_opcode;
    UCHAR * event_data;

    if (NULL == appl_event_info)
    {
        APPL_ERR("appl_event_info is NULL !\n");
        return;
    }

    cp_opcode = ((UCHAR *)appl_event_info->data)[0U];
    event_data = (UCHAR *)appl_event_info->data;

    appl_event_info->datalen--;

    APPL_TRC
    (
        "Len: 0x%02X,  Opcode: ",
        (UCHAR)sizeof(cp_opcode)
    );
    appl_ccp_utils_parse_and_display_ccp_cp_opcode(cp_opcode);

    switch(cp_opcode)
    {
        case CCP_CP_OP_ACCEPT: /* Fall-Through */
        case CCP_CP_OP_TERMINATE: /* Fall-Through */
        case CCP_CP_OP_L_HLD: /* Fall-Through */
        case CCP_CP_OP_L_RTRV: /* Fall-Through */
        case CCP_CP_OP_ORIGINATE: /* Fall-Through */
        case CCP_CP_OP_JOIN:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Index: 0x%02X\n",
                (UCHAR)sizeof(event_data[1U]), event_data[1U]
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Result Code: ",
                (UCHAR)sizeof(event_data[2U])
            );
            appl_ccp_ce_parse_and_display_cp_result_code(event_data[2U]);
            break;

        default:
            APPL_ERR("Opcode Not Supported (0x%02X) !\n", cp_opcode);
    }
}

void appl_ccp_ce_parse_and_display_cp_result_code(UCHAR  result_code)
{
    switch(result_code)
    {
        case CCP_CP_NTF_RESULT_SUCCESS:
            APPL_TRC("Success");
            break;

        case CCP_CP_NTF_RESULT_OPCODE_NOT_SUPPORTED:
            APPL_TRC("Opcode Not Supported");
            break;

        case CCP_CP_NTF_RESULT_OP_NOT_POSSIBLE:
            APPL_TRC("Operation Not Possible");
            break;

        case CCP_CP_NTF_RESULT_INVALID_CALL_INDEX:
            APPL_TRC("Invalid Call Index");
            break;

        case CCP_CP_NTF_RESULT_STATE_MISMATCH:
            APPL_TRC("State Mismatch");
            break;

        case CCP_CP_NTF_RESULT_LACK_OF_RESOURCES:
            APPL_TRC("Lack of Resources");
            break;

        case CCP_CP_NTF_RESULT_INVALID_OUTGOING_URI:
            APPL_TRC("Invalid Outgoing URI");
            break;

        default:
            APPL_ERR("Invalid");
    }
    APPL_TRC(" (0x%02X)\n", result_code);
}

#endif /* CCP_CLIENT */
