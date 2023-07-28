
/**
 *  \file appl_ccp_se.c
 *
 *  \brief This file is the Call Control Profile Server reference application
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ccp_se.h"
#include "appl_ccp_utils.h"

/* #include "db_gen.h" */

#ifdef CCP_SERVER
/* --------------------------------------------- Global Definitions */

static const UCHAR ga_ccp_se_appl_menu[] =
"\n\
================ GA CCP Server MENU Operations ================ \n\
    0. Exit \n\
    1. Refresh this Menu. \n\
\n\
================ Service Management ~ Init ============ \n\
    2. CCP Server Init \n\
\n\
================ Service Management ~ Deinit ============ \n\
    3. CCP Server Shutdown \n\
\n\
================ Service Management ~ Optional Service ============ \n\
    4. CCP Register TBS Entity \n\
\n\
================ Device Management ~ Setting ============ \n\
    7. Set Peer BD Address \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
    9. Set TBS Handle \n\
   10. Reset TBS. \n\
   11. Set Active TBS Service \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   12. Send Char Notify \n\
\n\
   13. Update Char Values \n\
\n\
   60. Setup Incomming call \n\
\n\
   71. Set Incomming call as Active \n\
   72. Reset all calls \n\
\n\
   73. Set Remotely Held \n\
   74. Set Locally and Remotely Held \n\
   77. Set Call Active \n\
   78. Set Call Alerting \n\
\n\
   79. Set Nwt Joint Support \n\
   80. Set Invalid URI CP write rsp \n\
\n\
   81. Set Char. change flag \n\
   82. Set Long Call State \n\
\n\
   90. Display GTBS curr. calls \n\
   91. Display TBS curr. calls \n\
\n\
================ Service Management ~ SDP Record ============ \n\
  300. Activate TBS SDP record \n\
  301. InActivate TBS SDP record \n\
\n\
Your Option? ";

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- External Global Variables */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
TBS_HANDLE   appl_ccp_se_gtbs_handle = 0U;
TBS_HANDLE   appl_ccp_se_tbs_handle = 0U;

DECL_STATIC  GA_ENDPOINT ga_ccp_se_peer_bd_addr;

DECL_STATIC  TBS_PARAMS appl_tbs_se_params[APPL_TBS_SE_ENITITES];
DECL_STATIC  TBS_CALLER_FRIENDLY_NAME appl_gtbs_se_caller_friendly_name;
DECL_STATIC  TBS_BRR_LIST_CUR_CALL
                 appl_gtbs_se_brr_cur_call_list[APPL_TBS_SE_BRR_CALL_LIST];
DECL_STATIC  TBS_CALLER_ID  appl_gtbs_se_incall_trgt_uri;
DECL_STATIC  TBS_CALLER_ID  appl_gtbs_se_incall_uri;
DECL_STATIC  TBS_TERMIN_REASON appl_gtbs_se_term_rsn;

DECL_STATIC  TBS_CALLER_FRIENDLY_NAME appl_tbs_se_caller_friendly_name;
DECL_STATIC  TBS_BRR_LIST_CUR_CALL
                 appl_tbs_se_brr_cur_call_list[APPL_TBS_SE_BRR_CALL_LIST];
DECL_STATIC  TBS_CALLER_ID  appl_tbs_se_incall_trgt_uri;
DECL_STATIC  TBS_CALLER_ID  appl_tbs_se_incall_uri;
DECL_STATIC  TBS_TERMIN_REASON appl_tbs_se_term_rsn;

DECL_STATIC  UCHAR appl_chr_data[64U];
DECL_STATIC  UINT16 appl_chr_datalen;

DECL_STATIC  UCHAR appl_active_service = 0U;
DECL_STATIC  UCHAR appl_nwt_join_support_flag = 1U;

DECL_STATIC  UCHAR appl_send_invalid_uri_rsp = 0U;

DECL_STATIC  UCHAR appl_char_changed_dur_rd_flag = 0U;

DECL_STATIC  UCHAR appl_set_long_call_state_flag = 0U;

/* Unique Call Id, allocated for each call */
DECL_STATIC  UCHAR g_appl_call_id = 1U;

/* Timer Handle to send GTBS/TBS Call State */
DECL_STATIC BT_timer_handle  appl_ccp_se_call_state_timer_handle;

/* Timer Handle to send GTBS/TBS Call Control Point */
DECL_STATIC BT_timer_handle  appl_ccp_se_call_cp_timer_handle;

/* Timer Handle to send GTBS/TBS Termination Reason */
DECL_STATIC BT_timer_handle  appl_ccp_se_terminate_reason_timer_handle;

/* --------------------------------------------- Functions */
void main_ccp_se_menu_operations(void)
{
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;
    UINT8      conn_dev_index;
    int        choice, input, menu_choice;
    UCHAR      call_index;
    UCHAR      i;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ga_ccp_se_appl_menu);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            APPL_TRC("GA_ccp_se_init...\n");
            retval = GA_ccp_se_init(appl_ccp_se_callback);
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);

            i = 0U;

            /* Init */
            GA_mem_set
            (
                &appl_tbs_se_params[0U],
                0U,
                sizeof(TBS_PARAMS)*APPL_TBS_SE_ENITITES
            );
            GA_mem_set
            (
                &appl_gtbs_se_brr_cur_call_list[0U],
                0U,
                sizeof(TBS_BRR_LIST_CUR_CALL)*APPL_TBS_SE_BRR_CALL_LIST
            );

            /* GTBS */
            appl_tbs_se_params[0U].brr_list_curr_list =
                &appl_gtbs_se_brr_cur_call_list[0U];
            appl_tbs_se_params[0U].incmg_caller_trgt_id =
                &appl_gtbs_se_incall_trgt_uri;
            appl_tbs_se_params[0U].call_state =
                &appl_gtbs_se_brr_cur_call_list[0U];
            appl_tbs_se_params[0U].incmg_caller_info =
                &appl_gtbs_se_incall_uri;
            appl_tbs_se_params[0U].frndly_name =
                &appl_gtbs_se_caller_friendly_name;
            appl_tbs_se_params[0U].term_reason =
                &appl_gtbs_se_term_rsn;
            appl_ccp_se_init_gtbs_info(&appl_tbs_se_params[0U]);
            i++;

            /* TBS-1 */
            /* Initialize only if APPL_TBS_SE_ENITITES > 1U (1 instance of TBS) */
            if (APPL_TBS_SE_ENITITES > i)
            {
                appl_tbs_se_params[i].brr_list_curr_list =
                    &appl_tbs_se_brr_cur_call_list[0U];
                appl_tbs_se_params[i].incmg_caller_trgt_id =
                    &appl_tbs_se_incall_trgt_uri;
                appl_tbs_se_params[i].call_state =
                    &appl_tbs_se_brr_cur_call_list[0U];
                appl_tbs_se_params[i].incmg_caller_info =
                    &appl_tbs_se_incall_uri;
                appl_tbs_se_params[i].frndly_name =
                    &appl_tbs_se_caller_friendly_name;
                appl_tbs_se_params[i].term_reason =
                    &appl_tbs_se_term_rsn;
                appl_ccp_se_init_tbs_info(&appl_tbs_se_params[1U], 0x01U);
            }
            i++;

            /* TBS-2 */
            /* Initialize only if APPL_TBS_SE_ENITITES > 2U (2 instance of TBS) */
            if (APPL_TBS_SE_ENITITES > i)
            {
                appl_tbs_se_params[i].brr_list_curr_list =
                    &appl_tbs_se_brr_cur_call_list[1U];
                appl_tbs_se_params[i].incmg_caller_trgt_id =
                    &appl_tbs_se_incall_trgt_uri;
                appl_tbs_se_params[i].call_state =
                    &appl_tbs_se_brr_cur_call_list[1U];
                appl_tbs_se_params[i].incmg_caller_info =
                    &appl_tbs_se_incall_uri;
                appl_tbs_se_params[i].frndly_name =
                    &appl_tbs_se_caller_friendly_name;
                appl_tbs_se_params[i].term_reason =
                    &appl_tbs_se_term_rsn;
                appl_ccp_se_init_tbs_info(&appl_tbs_se_params[2U], 0x02U);
            }

            appl_ccp_se_init_timer_handle();
            break;

        case 3:
            APPL_TRC("GA_ccp_se_shutdown...\n");
            retval = GA_ccp_se_shutdown();
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            break;

        case 4:
        {
            APPL_TRC("GA_ccp_se_register_tbs...\n");
            retval = GA_ccp_se_register_tbs
                     (
                         &appl_ccp_se_tbs_handle
                     );
            APPL_TRC("Retval - ");
            appl_ccp_display_ga_result(retval);
            APPL_TRC
            (
                "appl_ccp_se_tbs_handle: 0x%02x\n", appl_ccp_se_tbs_handle
            );
        }
            break;

        case 7:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &ga_ccp_se_peer_bd_addr
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_ccp_se_peer_bd_addr.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                ga_ccp_se_peer_bd_addr.bd_type = (UINT8)choice;
            }
            break;

        case 9:
            CONSOLE_OUT("Enter the TBS Handle: ");
            CONSOLE_IN("%d", &choice);
            if (CCP_MAX_TBS_ENTITIES <= (UINT8)choice)
            {
                APPL_INF("Invalid handle !\n");
            }
            else
            {
                appl_ccp_se_tbs_handle = (UINT8)choice;
            }
            break;

        case 10:
            /* Reset all call state values - GTBS and TBS */
            for (i = 0U; i < APPL_TBS_SE_ENITITES; i++)
            {
                appl_gtbs_reset_all_call_details(i);
            }

            /* Set the Default Values for mandatory GTBS and 2 TBS instances */
            appl_ccp_se_init_gtbs_info(&appl_tbs_se_params[0U]);
            if (APPL_TBS_SE_ENITITES > 1U)
            {
                appl_ccp_se_init_tbs_info(&appl_tbs_se_params[1U], 0x01U);
            }
            if (APPL_TBS_SE_ENITITES > 2U)
            {
                appl_ccp_se_init_tbs_info(&appl_tbs_se_params[2U], 0x02U);
            }

            /* Default Value */
            g_appl_call_id = 0x01U;
            appl_set_long_call_state_flag = 0U;
            appl_char_changed_dur_rd_flag = 0U;
            appl_send_invalid_uri_rsp = 0U;
            appl_nwt_join_support_flag = 1U;

            break;

        case 11:
            appl_set_active_tbs_service();
            break;

        case 12:
            appl_tbs_notify_char_value();
            break;

        case 13:
            appl_tbs_set_char_value();
            break;
#if 0
        case 50:
            CONSOLE_OUT("Enter the Call Index(0/1/2):");
            CONSOLE_IN("%d", &input);
            appl_tbs_notify_incall_details((UCHAR)input);
            break;
#endif /* 0 */

        case 60:
        {
            UCHAR  index;

            /* MISRA C-2012 Rule 9.1 */
            index = 0U;

            if (0xFFU == appl_active_service)
            {
                CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
                CONSOLE_IN("%x", &choice);
                if (0 == choice)
                {
                    appl_srvs_handle = appl_ccp_se_gtbs_handle;
                }
                else
                {
                    appl_srvs_handle = appl_ccp_se_tbs_handle;
                }
            }
            else
            {
                appl_srvs_handle = appl_active_service;
            }

            retval = appl_tbs_setup_incall(appl_srvs_handle, &index);
            APPL_TRC("retval: 0x%04x\n", retval);
            APPL_TRC("call index in call list:0x%02x \n", index);

            if (GA_SUCCESS == retval)
            {
                appl_tbs_notify_incall_details(appl_srvs_handle, index);
            }

        }
            break;

        case 72:
            /* Reset all call state values - GTBS and TBS */
            for (i = 0U; i < APPL_TBS_SE_ENITITES; i++)
            {
                appl_gtbs_reset_all_call_details(i);
            }
            /* Reset call ID */
            g_appl_call_id = 1U;
            break;

        case 73:
            if (0xFFU == appl_active_service)
            {
                CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
                CONSOLE_IN("%x", &choice);
                if (0 == choice)
                {
                    appl_srvs_handle = appl_ccp_se_gtbs_handle;
                }
                else
                {
                    appl_srvs_handle = appl_ccp_se_tbs_handle;
                }
            }
            else
            {
                appl_srvs_handle = appl_active_service;
            }

            CONSOLE_OUT("Enter the Call ID: ");
            CONSOLE_IN("%d", &input);
            retval = appl_tbs_get_call_index_from_call_id
                     (
                         appl_srvs_handle,
                         (UCHAR)input,
                         &call_index
                     );
            if (GA_SUCCESS == retval)
            {
                appl_gtbs_set_remotely_held(appl_srvs_handle, call_index);
            }
            else
            {
                APPL_ERR("Invalid Call ID !\n");
            }
            break;

        case 74:
            if (0xFFU == appl_active_service)
            {
                CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
                CONSOLE_IN("%x", &choice);
                if (0 == choice)
                {
                    appl_srvs_handle = appl_ccp_se_gtbs_handle;
                }
                else
                {
                    appl_srvs_handle = appl_ccp_se_tbs_handle;
                }
            }
            else
            {
                appl_srvs_handle = appl_active_service;
            }

            CONSOLE_OUT("Enter the Call ID: ");
            CONSOLE_IN("%d", &input);
            retval = appl_tbs_get_call_index_from_call_id
                     (
                         appl_srvs_handle,
                         (UCHAR)input,
                         &call_index
                     );
            if (GA_SUCCESS == retval)
            {
                appl_gtbs_set_locally_and_remotely_held
                (
                    appl_srvs_handle,
                    call_index
                );
            }
            else
            {
                APPL_ERR("Invalid Call ID !\n");
            }
            break;

        case 77:
            if (0xFFU == appl_active_service)
            {
                CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
                CONSOLE_IN("%x", &choice);
                if (0 == choice)
                {
                    appl_srvs_handle = appl_ccp_se_gtbs_handle;
                }
                else
                {
                    appl_srvs_handle = appl_ccp_se_tbs_handle;
                }
            }
            else
            {
                appl_srvs_handle = appl_active_service;
            }

            CONSOLE_OUT("Enter the Call ID: ");
            CONSOLE_IN("%d", &input);
            retval = appl_tbs_get_call_index_from_call_id
                     (
                         appl_srvs_handle,
                         (UCHAR)input,
                         &call_index
                     );
            if (GA_SUCCESS == retval)
            {
                appl_gtbs_set_active(appl_srvs_handle, call_index);
            }
            else
            {
                APPL_ERR("Invalid Call ID !\n");
            }
            break;

        case 78:
            if (0xFFU == appl_active_service)
            {
                CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
                CONSOLE_IN("%x", &choice);
                if (0 == choice)
                {
                    appl_srvs_handle = appl_ccp_se_gtbs_handle;
                }
                else
                {
                    appl_srvs_handle = appl_ccp_se_tbs_handle;
                }
            }
            else
            {
                appl_srvs_handle = appl_active_service;
            }

            CONSOLE_OUT("Enter the Call ID: ");
            CONSOLE_IN("%d", &input);
            retval = appl_tbs_get_call_index_from_call_id
                     (
                         appl_srvs_handle,
                         (UCHAR)input,
                         &call_index
                     );
            if (GA_SUCCESS == retval)
            {
                appl_gtbs_set_call_alerting(appl_srvs_handle, call_index);
            }
            else
            {
                APPL_ERR("Invalid Call ID !\n");
            }
            break;

        case 79:
            CONSOLE_OUT("Enter Network Join Support:");
            CONSOLE_OUT("0 -> No\n");
            CONSOLE_OUT("1 -> Yes\n");
            CONSOLE_IN("%d", &input);

            appl_nwt_join_support_flag = (UCHAR)input;
            break;

        case 80:
            CONSOLE_OUT("Set Invalid URI Response:");
            CONSOLE_OUT("0 -> No\n");
            CONSOLE_OUT("1 -> Yes\n");
            CONSOLE_IN("%d", &input);

            appl_send_invalid_uri_rsp = (UCHAR)input;
            break;

        case 81:
            CONSOLE_OUT("Select Below Option\n");
            CONSOLE_OUT("1 -> Set\n");
            CONSOLE_OUT("0 -> Reset the Flag\n");
            CONSOLE_IN("%d", &input);

            appl_char_changed_dur_rd_flag = (UCHAR)input;
            break;

        case 82:
            CONSOLE_OUT("Select Below Option\n");
            CONSOLE_OUT("1 -> Sent long call state value\n");
            CONSOLE_OUT("0 -> Reset the Flag\n");
            CONSOLE_IN("%d", &input);

            appl_set_long_call_state_flag = (UCHAR)input;
            break;

        case 90:
            appl_gtbs_display_cur_call_state();
            break;

        case 91:
            appl_tbs_display_cur_call_state();
            break;

        case 300:
            appl_set_tbs_sdp_record_status(0x01U);
            break;

        case 301:
            appl_set_tbs_sdp_record_status(0x00U);
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

GA_RESULT appl_ccp_se_callback
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void        * event_data,
              /* IN */ UINT16        event_datalen
          )
{
    GA_RESULT         retval;

    CCP_EVENT_INFO  * appl_event_info;
    UCHAR             srvc_handle;
    TBS_PARAMS      * tbs_data;

    UCHAR             send_tbs_rsp_flag;
    UCHAR             rsp_event;
    UINT16            rsp_status;
    CCP_RSP_INF       tbs_rsp_info;
    GA_BRR_SVC_INFO * svc;

    GA_IGNORE_UNUSED_PARAM(event_datalen);

    /* Initialize */
    retval = GA_SUCCESS;

    if ((NULL == event_data) || (NULL == device))
    {
        APPL_ERR("Invalid parameters !\n");
        return GA_FAILURE;
    }

    appl_event_info = (CCP_EVENT_INFO *)event_data;
    svc = (GA_BRR_SVC_INFO *)appl_event_info->data;

    srvc_handle = appl_event_info->srvc_handle;

    if (APPL_TBS_SE_ENITITES > srvc_handle)
    {
        tbs_data = &appl_tbs_se_params[srvc_handle];
    }
    else
    {
        APPL_ERR("srvc_handle is OUT of range !\n");
        return GA_FAILURE;
    }

    /* Init */
    send_tbs_rsp_flag = 0x00U;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;

    GA_CB_ENTRY("CCP Server");

    if (CCP_SVS_TYPE_GTBS == srvc_handle)
    {
        APPL_TRC("[Profile]     : CCP-GTBS (0x%4X)\n", GA_SERVICE_GTBS);
    }
    else
    {
        APPL_TRC("[Profile]     : CCP-TBS (0x%4X)\n", GA_SERVICE_TBS);
    }

    APPL_TRC("[SECTION]     : CCP SERVER EVENTS\n");

    switch (event_type)
    {
    case CCP_SE_SET_BRR_SIG_STRTH_RPT_INTVAL_IND:
        APPL_TRC("[SUB-SECTION] : CCP SERVER-WRITE\n");
        if (GA_TRUE == appl_event_info->chr->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_SE_SET_BRR_SIG_STRTH_RPT_INTVAL_IND (0x%02X)\n",
            event_type
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            srvc_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Bearer Signal Strength Reporting Interval (0x%04X)\n",
            GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL
        );
        GA_FORMAT_DISPLAY();

        if ((NULL != appl_event_info) && (0U != appl_event_info->datalen))
        {
            retval = GA_SUCCESS;
            tbs_data->brr_sig_strength_reptg_intval =
                ((UCHAR *)appl_event_info->data)[0U];

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("RECEIVED DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", appl_event_info->datalen);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer Signal Strength Reporting Interval: 0x%02X\n",
                (UCHAR )appl_event_info->datalen,
                tbs_data->brr_sig_strength_reptg_intval
            );
        }

        if (GA_TRUE == appl_event_info->chr->to_rsp)
        {
            /* Fill the response Info. */
            send_tbs_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_WT;
            tbs_rsp_info.chr = appl_event_info->chr;
            tbs_rsp_info.data = NULL;
            tbs_rsp_info.datalen = 0U;

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n",tbs_rsp_info.datalen);
            if (0U == tbs_rsp_info.datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case CCP_SE_CP_WT_IND:
        APPL_TRC("[SUB-SECTION] : CCP SERVER-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_SE_CP_WT_IND (0x%02X)\n",
            event_type
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            srvc_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Call Control Point (0x%04X)\n",
            GA_CHAR_TBS_CALL_CP
        );
        GA_FORMAT_DISPLAY();
        if (GA_TRUE != appl_event_info->chr->to_rsp)
        {
            /**
             * Handling the CP write and sending the required notifications
             * from CP write context.
             */
            retval = appl_ccp_se_handle_cp_write
                     (
                         device,
                         appl_event_info
                     );
        }
        else
        {
            /**
             * First send the response and then handle the CP write & send
             * required notifications from the application context.
             */

            /* Fill the response Info. */
            send_tbs_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_WT;
            tbs_rsp_info.chr = appl_event_info->chr;
            tbs_rsp_info.data = NULL;
            tbs_rsp_info.datalen = 0U;
        }
        break;

    case CCP_SE_RD_IND:
        APPL_TRC("[SUB-SECTION] : CCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ccp_display_ga_result(event_status);
        APPL_TRC
        (
            "[Event]       : CCP_SE_RD_IND (0x%02X)\n",
            event_type
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            srvc_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        appl_ccp_utils_parse_and_display_char_uuid(appl_event_info->chr->uuid);
        GA_FORMAT_DISPLAY();

        UINT16  rd_offset;

        if ((GA_CONFIG_LIMITS(CCP_MAX_TBS_ENTITIES) <= appl_event_info->srvc_handle) ||
            (NULL == appl_event_info))
        {
            retval = GA_FAILURE;
            break;
        }

        rd_offset = 0U;
        if ((NULL != appl_event_info->data) && (0U != appl_event_info->datalen))
        {
            /* For Read Blob */
            GA_UNPACK_LE_2_BYTE(&rd_offset, (UCHAR*)appl_event_info->data);
            /* APPL_TRC("--------- Rd_offset:0x%04x--------- \n", rd_offset); */
        }

        appl_event_info->data = &appl_chr_data[0U];

        retval = appl_ccp_se_pack_tbs_chr_value
                 (
                     appl_event_info->chr->uuid,
                     tbs_data,
                     (UCHAR *)appl_event_info->data,
                     &appl_event_info->datalen
                 );
        if (GA_SUCCESS != retval)
        {
            /* TODO */
            appl_event_info->datalen = 0U;
        }

        /* Fill the response Info. */
        send_tbs_rsp_flag = 1U;
        tbs_rsp_info.chr = appl_event_info->chr;
        rsp_event = GA_RSP_TYPE_RD;

        if (0U != appl_char_changed_dur_rd_flag)
        {
            rsp_status = TBS_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG;
            tbs_rsp_info.data = NULL;
            tbs_rsp_info.datalen = 0U;
            appl_event_info->datalen = 0U;

            appl_char_changed_dur_rd_flag = 0U;
        }
        else
        {
            /* TODO: Remove this below hardcoded data formation for responding
             * to Read Blob request
             * Done during PTS test.
             */
            if (1U == appl_set_long_call_state_flag)
            {
                appl_set_large_call_state_info
                (
                    appl_event_info->data,
                    &appl_event_info->datalen
                );
            }

            tbs_rsp_info.data = ((UCHAR *)appl_event_info->data + rd_offset);
            tbs_rsp_info.datalen = (UCHAR)(appl_event_info->datalen - rd_offset);
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", tbs_rsp_info.datalen);
        if (0U == tbs_rsp_info.datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        else
        {
            appl_ccp_se_display_response_data(&tbs_rsp_info, rd_offset);
        }
        break;

    default:
        break;
    }

    if (0U != send_tbs_rsp_flag)
    {
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA CCP RESPONSE\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("API   : GA_ccp_se_send_rsp\n");
        retval = GA_ccp_se_send_rsp
                 (
                     device,
                     rsp_event,
                     rsp_status,
                     &tbs_rsp_info,
                     sizeof(CCP_RSP_INF)
                 );
        APPL_TRC("Retval: ");
        appl_ccp_display_ga_result(retval);
    }

    GA_CB_EXIT();

    return retval;
}

GA_RESULT appl_ccp_se_pack_tbs_chr_value
          (
              /* IN  */  UINT16       chr_uuid,
              /* IN  */  TBS_PARAMS * tbs_params,
              /* OUT */  UCHAR      * chr_val,
              /* OUT */  UINT16     * chr_val_len
          )
{
    GA_RESULT  retval;
    UCHAR      offset;
    UCHAR      i;
    UCHAR    * pdu;

    if ((NULL == chr_val) ||
        (NULL == chr_val_len)||
        (NULL == tbs_params))
    {
        APPL_ERR("Invalid parameters !\n");
        return GA_FAILURE;
    }

    /* Initialize */
    retval = GA_SUCCESS;
    offset = 0U;
    pdu = chr_val;

    switch (chr_uuid)
    {
    case GA_CHAR_TBS_BRR_PROVIDER_NAME:
        GA_mem_copy
        (
            &pdu[offset],
            &tbs_params->bearer_pname[0U],
            tbs_params->bearer_pname_len
        );
        offset += tbs_params->bearer_pname_len;

        break;

    case GA_CHAR_TBS_BRR_UCI:
        GA_mem_copy
        (
            &pdu[offset],
            &tbs_params->bearer_uci[0U],
            tbs_params->bearer_uci_len
        );
        offset += tbs_params->bearer_uci_len;
        break;

    case GA_CHAR_TBS_BRR_TECHNOLOGY:
        pdu[offset] = tbs_params->brr_tech;
        offset += 1U;
        break;

    case GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST:
        GA_mem_copy
        (
            &pdu[offset],
            &tbs_params->brr_uri_pfix_supp_list[0U],
            tbs_params->brr_uri_pfix_supp_list_len
        );
        offset += tbs_params->brr_uri_pfix_supp_list_len;
        break;

    case GA_CHAR_TBS_BRR_SIGSTRENGTH:
        pdu[offset] = tbs_params->brr_sig_strength;
        offset += 1U;
        break;

    case GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL:
        pdu[offset] = tbs_params->brr_sig_strength_reptg_intval;
        offset += 1U;
        break;

    case GA_CHAR_TBS_BRR_LIST_CUR_CALLS:
        for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
        {
            if (CCP_CALL_STATE_LR_HELD >= tbs_params->brr_list_curr_list[i].call_state)
            {
                /* Total Item Length */
                pdu[offset] = (3U + tbs_params->brr_list_curr_list[i].uri_len);
                offset += 1U;

                /* Call Index */
                pdu[offset] = tbs_params->brr_list_curr_list[i].call_index;
                offset += 1U;

                /* Call State */
                pdu[offset] = tbs_params->brr_list_curr_list[i].call_state;
                offset += 1U;

                /* Call Flags */
                pdu[offset] = tbs_params->brr_list_curr_list[i].call_flags;
                offset += 1U;

                /* URI */
                GA_mem_copy
                (
                    &pdu[offset],
                    &tbs_params->brr_list_curr_list[i].uri[0U],
                    tbs_params->brr_list_curr_list[i].uri_len
                );
                offset += tbs_params->brr_list_curr_list[i].uri_len;
            }
        }
        break;

    case GA_CHAR_CONTENT_CONTROL_ID:
        /* Content Control ID */
        pdu[offset] = tbs_params->content_control_id;
        offset += 1U;
        break;

    case GA_CHAR_TBS_STATUS_FLAGS:
        /* Satus Flags */
        GA_PACK_LE_2_BYTE_VAL
        (
            &pdu[offset],
            tbs_params->status_flags
        );
        offset += 2U;
        break;

    case GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI:
        /* Call Index */
        pdu[offset] = tbs_params->incmg_caller_trgt_id->call_indx;
        offset += 1U;

        /* URI */
        if ((MAX_TBS_URI_LEN >= tbs_params->incmg_caller_trgt_id->uri_len) &&
            (0U != tbs_params->incmg_caller_trgt_id->uri_len))
        {
            GA_mem_copy
            (
                &pdu[offset],
                &tbs_params->incmg_caller_trgt_id->uri[0U],
                tbs_params->incmg_caller_trgt_id->uri_len
            );
            offset += tbs_params->incmg_caller_trgt_id->uri_len;
        }
        break;

    case GA_CHAR_TBS_CALL_STATE:
        for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
        {
            if (CCP_CALL_STATE_LR_HELD >= tbs_params->call_state[i].call_state)
            {
                /* Call Index */
                pdu[offset] = tbs_params->call_state[i].call_index;
                offset += 1U;

                /* Call State */
                pdu[offset] = tbs_params->call_state[i].call_state;
                offset += 1U;

                /* Call Flags */
                pdu[offset] = tbs_params->call_state[i].call_flags;
                offset += 1U;
            }
        }
        break;

    case GA_CHAR_TBS_CALL_CP:
        break;

    case GA_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES:
        GA_PACK_LE_2_BYTE_VAL
        (
            &pdu[offset],
            tbs_params->opt_cp_opcodes
        );
        offset += 2U;
        break;

    case GA_CHAR_TBS_TERMINATION_REASON:
        pdu[offset] = tbs_params->term_reason->call_indx;
        offset += 1U;

        pdu[offset] = tbs_params->term_reason->reason;
        offset += 1U;
        break;

    case GA_CHAR_TBS_INCOMING_CALL:
        /* Call Index */
        pdu[offset] = tbs_params->incmg_caller_info->call_indx;
        offset += 1U;

        /* URI */
        if ((MAX_TBS_URI_LEN >= tbs_params->incmg_caller_info->uri_len) &&
            (0U != tbs_params->incmg_caller_info->uri_len))
        {
            GA_mem_copy
            (
                &pdu[offset],
                &tbs_params->incmg_caller_info->uri[0U],
                tbs_params->incmg_caller_info->uri_len
            );

            offset += tbs_params->incmg_caller_info->uri_len;
        }
        break;

    case GA_CHAR_TBS_CALL_FRIENDLY_NAME:
        /* Call Index */
        pdu[offset] = tbs_params->frndly_name->call_indx;
        offset += 1U;

        /* Friendly Name */
        if ((MAX_TBS_CALLER_NAME_LEN >= tbs_params->frndly_name->name_len) &&
            (0U != tbs_params->frndly_name->name_len))
        {
            GA_mem_copy
            (
                &pdu[offset],
                &tbs_params->frndly_name->name[0U],
                tbs_params->frndly_name->name_len
            );

            offset += tbs_params->frndly_name->name_len;
        }
        break;

    default:

        retval = GA_FAILURE;
        break;
    }

    (*chr_val_len) = offset;

    return retval;
}

GA_RESULT  appl_ccp_se_handle_cp_write
           (
               /* IN */ GA_ENDPOINT    * device,
               /* IN */ CCP_EVENT_INFO * appl_event_info
           )
{
    GA_RESULT retval;

    UCHAR   * cp_wt_data;
    UCHAR     cp_wt_datalen;
    UCHAR     cp_op;
    UCHAR     cp_result;
    UCHAR     i;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;

    if ((NULL == appl_event_info) ||
        (NULL == appl_event_info->data)||
        (2U > appl_event_info->datalen))
    {
        APPL_ERR("Invalid parameters !\n");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    cp_wt_data = (UCHAR *)appl_event_info->data;
    cp_wt_datalen = (UCHAR )appl_event_info->datalen;
    cp_op = cp_wt_data[0U];

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("RECEIVED DATA\n");
    GA_FORMAT_DISPLAY();

    APPL_TRC("Data Length: 0x%02X\n", cp_wt_datalen);
    APPL_TRC("Data:\n");

    APPL_TRC
    (
        "Len: 0x%02X,  Opcode: ",
        (UCHAR)sizeof(cp_op)
    );
    appl_ccp_utils_parse_and_display_ccp_cp_opcode(cp_op);

    switch (cp_op)
    {
        case TBS_CP_OP_ACCEPT:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Index: 0x%02X\n",
                cp_wt_datalen - 1U,
                *(cp_wt_data + 1U)
            );
            appl_tbs_handle_accept
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen-1U)
            );
            break;

        case TBS_CP_OP_TERMINATE:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Index: 0x%02X\n",
                cp_wt_datalen - 1U,
                *(cp_wt_data + 1U)
            );
            appl_tbs_handle_terminate
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen - 1U)
            );
            break;

    case TBS_CP_OP_L_HLD:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Index: 0x%02X\n",
                cp_wt_datalen - 1U,
                *(cp_wt_data + 1U)
            );
            appl_tbs_handle_local_hold
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen - 1U)
            );
            break;

    case TBS_CP_OP_L_RTRV:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Index: 0x%02X\n",
                cp_wt_datalen - 1U,
                *(cp_wt_data + 1U)
            );
            appl_tbs_handle_local_retrieve
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen - 1U)
            );
            break;

    case TBS_CP_OP_ORIGINATE:
            APPL_TRC
            (
                "Len: 0x%02X,  URI: ",
                cp_wt_datalen - 1U
            );
            /* Data is printed in the API */
            appl_tbs_handle_local_originate
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen - 1U)
            );
            break;

    case TBS_CP_OP_JOIN:
            APPL_TRC
            (
                "Len: 0x%02X,  List of Call Indexes: ",
                cp_wt_datalen - 1U
            );
            for (i = 0U; i < (cp_wt_datalen - 1U); i++)
            {
                APPL_TRC("0x%02X, ", (cp_wt_data[i + 1U]));
            }
            APPL_TRC("\n");
            appl_tbs_handle_local_join
            (
                device,
                appl_active_service,
                appl_active_service,
                (cp_wt_data + 1U),
                (cp_wt_datalen - 1U)
            );
            break;

        default:

            cp_result = TBS_CP_NTF_RESULT_OPCODE_NOT_SUPPORTED;

            APPL_ERR
            (
                "Len: 0x%02X,  Opcode: Opcode Not Supported (0x%02X) !\n",
                (UCHAR )sizeof(cp_op),
                cp_op
            );
            appl_dump_bytes(appl_event_info->data, appl_event_info->datalen);

            ccp_se_ntf_info.cp_ntf_info.cp_opcode = cp_op;
            ccp_se_ntf_info.cp_ntf_info.call_index = 0x00U;
            ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
            ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

            /*
             * Notification should be send to the device from which server has
             * received the CP write request.
             */
            /* Copy service handle */
            ccp_se_ntf_info.tbs_handle = appl_active_service;
            /* Copy Char UUID*/
            ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
            /* Copy service instance */
            ccp_se_ntf_info.srvc_inst = appl_active_service;
            /* Copy device info */
            GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

            BT_start_timer
            (
                &appl_ccp_se_call_cp_timer_handle,
                1U,
                appl_ccp_se_call_cp_timer_handler,
                &ccp_se_ntf_info,
                sizeof(ccp_se_ntf_info)
            );

            retval = GA_FAILURE;
            break;
    }

    return retval;
}

void appl_ccp_se_init_gtbs_info(/* IN */ TBS_PARAMS * appl_gtbs_params)
{
    UCHAR i;

    APPL_TRC(">> appl_ccp_se_init_gtbs_info\n");
    if (NULL == appl_gtbs_params)
    {
        APPL_ERR("Invalide parameters !\n");
        return;
    }

    /* Bearer Name */
    EM_str_copy
    (
        appl_gtbs_params->bearer_pname,
        APPL_TBS_BRR_AIRTEL
    );
    appl_gtbs_params->bearer_pname_len =
        (UCHAR)EM_str_len(APPL_TBS_BRR_AIRTEL) + 1U;

    /* UCI */
    EM_str_copy
    (
        appl_gtbs_params->bearer_uci,
        TBS_UCI_ID_WTSAP
    );
    appl_gtbs_params->bearer_uci_len = (UCHAR)EM_str_len(TBS_UCI_ID_WTSAP) + 1U;

    /* Technology */
    appl_gtbs_params->brr_tech = TBS_BRR_TECH_LTE;

    /* Bearer URI List */
    EM_str_copy
    (
        appl_gtbs_params->brr_uri_pfix_supp_list,
        TBS_URI_ID_TEL
    );
    appl_gtbs_params->brr_uri_pfix_supp_list_len =
        (UCHAR)EM_str_len(TBS_URI_ID_TEL) + 1U;

    /* Signal Strength */
    appl_gtbs_params->brr_sig_strength = TBS_BRR_SIG_UNAVAILABLE;

    /* Signal reporting interval */
    appl_gtbs_params->brr_sig_strength_reptg_intval = 0U;

    /* */
    GA_mem_set
    (
        appl_gtbs_params->brr_list_curr_list,
        0U,
        sizeof(TBS_BRR_LIST_CUR_CALL)*APPL_TBS_SE_BRR_CALL_LIST
    );

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        appl_gtbs_params->brr_list_curr_list[i].call_state =
            TBS_CALL_STATE_INVALID;
    }

    /* TODO */
    appl_gtbs_params->content_control_id = 0x01U;

    /* Status Flags */
    appl_gtbs_params->status_flags = 0x0000U;

    /* Incoming Caller Target URI */
    appl_gtbs_params->incmg_caller_trgt_id->call_indx = 0U;
    EM_str_copy
    (
        appl_gtbs_params->incmg_caller_trgt_id->uri,
        APPL_TBS_TRGT_BRR_URI
    );
    appl_gtbs_params->incmg_caller_trgt_id->uri_len =
        (UCHAR)EM_str_len(APPL_TBS_TRGT_BRR_URI) + 1U;

    /* Optional */
    appl_gtbs_params->opt_cp_opcodes = 0x0003U;

    /* Incoming Call Info. */
    appl_gtbs_params->incmg_caller_info->call_indx = 0x00U;
    appl_gtbs_params->incmg_caller_info->uri_len = 0x00U;

    appl_gtbs_params->frndly_name->call_indx = 0x00U;

#if 1
    /* Call Friendly Name */
    EM_str_copy
    (
        appl_gtbs_params->frndly_name->name,
        APPL_TBS_FRNDLY_NAME
    );
    appl_gtbs_params->frndly_name->name_len =
        (UCHAR)EM_str_len(APPL_TBS_FRNDLY_NAME) + 1U;
#else
    appl_tbs_se_params[0U].frndly_name->name_len = 0x00U;
#endif /* 0 */

    APPL_TRC("<< appl_ccp_se_init_gtbs_info\n");
}

void appl_ccp_se_init_tbs_info(/* IN */ TBS_PARAMS * appl_tbs_params, UCHAR indx)
{
    UCHAR i;

    APPL_TRC(">> appl_ccp_se_init_tbs_info\n");
    if (NULL == appl_tbs_params)
    {
        APPL_ERR("appl_tbs_params is NULL !\n");
        return;
    }

    if (0x01U == indx)
    {
        /* Bearer Name */
        EM_str_copy
        (
            appl_tbs_params->bearer_pname,
            APPL_TBS_BRR_BSNL
        );
        appl_tbs_params->bearer_pname_len =
            (UCHAR)EM_str_len(APPL_TBS_BRR_BSNL) + 1U;
    }
    else if (0x02U == indx)
    {
        /* Bearer Name */
        EM_str_copy
        (
            appl_tbs_params->bearer_pname,
            APPL_TBS_BRR_JIO
        );
        appl_tbs_params->bearer_pname_len =
            (UCHAR)EM_str_len(APPL_TBS_BRR_JIO) + 1U;
    }
    else
    {
        /* should not reach here */
        return;
    }

    /* UCI */
    EM_str_copy
    (
        appl_tbs_params->bearer_uci,
        TBS_UCI_ID_WTSAP
    );
    appl_tbs_params->bearer_uci_len =
        (UCHAR)EM_str_len(TBS_UCI_ID_WTSAP) + 1U;

    /* Technology */
    appl_tbs_params->brr_tech = TBS_BRR_TECH_LTE;

    /* Bearer URI List */
    EM_str_copy
    (
        appl_tbs_params->brr_uri_pfix_supp_list,
        TBS_URI_ID_TEL
    );
    appl_tbs_params->brr_uri_pfix_supp_list_len =
        (UCHAR)EM_str_len(TBS_URI_ID_TEL) + 1U;

    /* Signal Strength */
    appl_tbs_params->brr_sig_strength = TBS_BRR_SIG_UNAVAILABLE;

    /* Signal reporting interval */
    appl_tbs_params->brr_sig_strength_reptg_intval = 0U;

    /* */
    GA_mem_set
    (
        appl_tbs_params->brr_list_curr_list,
        0U,
        sizeof(TBS_BRR_LIST_CUR_CALL)*APPL_TBS_SE_BRR_CALL_LIST
    );

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        appl_tbs_params->brr_list_curr_list[i].call_state =
            TBS_CALL_STATE_INVALID;
    }

    /* TODO */
    appl_tbs_params->content_control_id = 0x02U;

    /* Status Flags */
    appl_tbs_params->status_flags = 0x0000U;

    /* Incoming Caller Target URI */
    appl_tbs_params->incmg_caller_trgt_id->call_indx = 0U;
    EM_str_copy
    (
        appl_tbs_params->incmg_caller_trgt_id->uri,
        APPL_TBS_TRGT_BRR_URI
    );
    appl_tbs_params->incmg_caller_trgt_id->uri_len =
        (UCHAR)EM_str_len(APPL_TBS_TRGT_BRR_URI) + 1U;

    /* Optional */
    appl_tbs_params->opt_cp_opcodes = 0x0003U;

    /* Incoming Call Info. */
    appl_tbs_params->incmg_caller_info->call_indx = 0x00U;
    appl_tbs_params->incmg_caller_info->uri_len = 0x00U;

    appl_tbs_params->frndly_name->call_indx = 0x00U;

#if 1
    /* Call Friendly Name */
    EM_str_copy
    (
        appl_tbs_params->frndly_name->name,
        APPL_TBS_FRNDLY_NAME
    );
    appl_tbs_params->frndly_name->name_len =
        (UCHAR)EM_str_len(APPL_TBS_FRNDLY_NAME) + 1U;
#else
    appl_tbs_se_params[0U].frndly_name->name_len = 0x00U;
#endif /* 0 */

    APPL_TRC(">> appl_ccp_se_init_tbs_info\n");
}

void appl_ccp_se_init_timer_handle(void)
{
    appl_ccp_se_call_state_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    appl_ccp_se_call_cp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    appl_ccp_se_terminate_reason_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
}

void appl_tbs_notify_incall_details(UCHAR tbs_srvc_index, UCHAR index)
{
    int         read_val;
    GA_RESULT   retval;
    UCHAR       appl_srvs_handle;


    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &read_val);
        if (0U == read_val)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    /* Packet Call State */
    retval = appl_ccp_se_pack_tbs_chr_value
             (
                GA_CHAR_TBS_CALL_STATE,
                &appl_tbs_se_params[appl_srvs_handle],
                &appl_chr_data[0U],
                &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_call_state...\n");
    retval = GA_ccp_se_ntf_call_state
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );
    appl_chr_data[0U] =
        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_index;

    APPL_TRC("GA_ccp_se_ntf_incoming_call_trgt_brr_uri...\n");
    retval = GA_ccp_se_ntf_incoming_call_trgt_brr_uri
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_INCOMING_CALL,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );
    appl_chr_data[0U] =
        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_index;

    APPL_TRC("GA_ccp_se_ntf_incoming_call...\n");
    retval = GA_ccp_se_ntf_incoming_call
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_CALL_FRIENDLY_NAME,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );
    appl_chr_data[0U] =
        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_index;

    APPL_TRC("GA_ccp_se_ntf_call_friendly_name...\n");
    retval = GA_ccp_se_ntf_call_friendly_name
             (
                         &ga_ccp_se_peer_bd_addr,
                         appl_srvs_handle,
                         &appl_chr_data[0U],
                         appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);

    return;
}


void appl_tbs_display_cur_call_state(void)
{
    UCHAR i, j;

    /* Initialize */
    i = 0U;
    j = 0U;

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        APPL_TRC
        (
            "Call Index: 0x%02X\n",
            appl_tbs_se_params[1U].brr_list_curr_list[i].call_index
        );
        APPL_TRC
        (
            "State: "
        );
        appl_ccp_utils_parse_and_display_ccp_call_state
        (
            appl_tbs_se_params[1U].brr_list_curr_list[i].call_state
        );
        APPL_TRC
        (
            "Call Flags: 0x%02X\n",
            appl_tbs_se_params[1U].brr_list_curr_list[i].call_flags
        );

        APPL_TRC("URI:");
        for (j = 0U; j < appl_tbs_se_params[1U].brr_list_curr_list[i].uri_len; j++)
        {
            APPL_TRC
            (
                "%c",
                appl_tbs_se_params[1U].brr_list_curr_list[i].uri[j]
            );
        }
        APPL_TRC("\n");
    }
}

void appl_gtbs_display_cur_call_state(void)
{
    UCHAR i, j;

    /* Initialize */
    i = 0U;
    j = 0U;

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        APPL_TRC
        (
            "Call Index: 0x%02X\n",
            appl_tbs_se_params[0U].brr_list_curr_list[i].call_index
        );
        APPL_TRC
        (
            "State: "
        );
        appl_ccp_utils_parse_and_display_ccp_call_state
        (
            appl_tbs_se_params[0U].brr_list_curr_list[i].call_state
        );
        APPL_TRC
        (
            "Call Flags: "
        );
        appl_ccp_utils_parse_and_display_ccp_call_flags
        (
            appl_tbs_se_params[0U].brr_list_curr_list[i].call_flags
        );

        APPL_TRC("URI:");
        for (j = 0U; j < appl_tbs_se_params[0U].brr_list_curr_list[i].uri_len; j++)
        {
            APPL_TRC("%c", appl_tbs_se_params[0U].brr_list_curr_list[i].uri[j]);
        }
        APPL_TRC("\n");
    }
}

void appl_gtbs_reset_all_call_details(UCHAR  tbs_indx)
{
    UCHAR i;

    if (APPL_TBS_SE_ENITITES <= tbs_indx)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    /* Initialize */
    i = 0U;

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        appl_tbs_se_params[tbs_indx].call_state[i].call_state =
            TBS_CALL_STATE_INVALID;
        appl_tbs_se_params[tbs_indx].call_state[i].call_flags = 0U;
    }
}

void appl_gtbs_set_remotely_held(UCHAR  srvc, UCHAR  call_index)
{
    if (APPL_TBS_SE_ENITITES <= srvc)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    if (APPL_TBS_SE_BRR_CALL_LIST <= call_index)
    {
        APPL_ERR("Invalid Call Index !\n");
        return;
    }

    appl_tbs_se_params[srvc].call_state[call_index].call_state =
        TBS_CALL_STATE_R_HELD;
    appl_tbs_se_params[srvc].call_state[call_index].call_flags = 0U;
}

void appl_gtbs_set_locally_and_remotely_held(UCHAR  srvc, UCHAR  call_index)
{
    if (APPL_TBS_SE_ENITITES <= srvc)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    if (APPL_TBS_SE_BRR_CALL_LIST <= call_index)
    {
        APPL_ERR("Invalid Call Index !\n");
        return;
    }

    appl_tbs_se_params[srvc].call_state[call_index].call_state =
        TBS_CALL_STATE_LR_HELD;
    appl_tbs_se_params[srvc].call_state[call_index].call_flags = 0U;
}

void appl_gtbs_set_active(UCHAR  srvc, UCHAR  call_index)
{
    if (APPL_TBS_SE_ENITITES <= srvc)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    if (APPL_TBS_SE_BRR_CALL_LIST <= call_index)
    {
        APPL_ERR("Invalid Call Index !\n");
        return;
    }

    appl_tbs_se_params[srvc].call_state[call_index].call_state =
        TBS_CALL_STATE_ACTIVE;
    appl_tbs_se_params[srvc].call_state[call_index].call_flags = 0U;
}

void appl_gtbs_set_locate_held(UCHAR  srvc, UCHAR  call_index)
{
    if (APPL_TBS_SE_ENITITES <= srvc)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    if (APPL_TBS_SE_BRR_CALL_LIST <= call_index)
    {
        APPL_ERR("Invalid Call Index !\n");
        return;
    }

    appl_tbs_se_params[srvc].call_state[call_index].call_state =
        TBS_CALL_STATE_L_HELD;
    appl_tbs_se_params[srvc].call_state[call_index].call_flags = 0U;
}

void appl_gtbs_set_call_alerting(UCHAR  srvc, UCHAR  call_index)
{
    if (APPL_TBS_SE_ENITITES <= srvc)
    {
        APPL_ERR("Invalid Service Index !\n");
        return;
    }

    if (APPL_TBS_SE_BRR_CALL_LIST <= call_index)
    {
        APPL_ERR("Invalid Call Index !\n");
        return;
    }

    appl_tbs_se_params[srvc].call_state[call_index].call_state =
        TBS_CALL_STATE_ALERTING;
}


void appl_set_active_tbs_service(void)
{
    int read_val;

    CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
    CONSOLE_IN("%x", &read_val);
    if (0U == read_val)
    {
        appl_active_service = appl_ccp_se_gtbs_handle;
    }
    else
    {
        appl_active_service = appl_ccp_se_tbs_handle;
    }
}

void appl_tbs_handle_accept
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT retval;
    UCHAR     call_index;
    UCHAR     i;
    UCHAR     cp_result;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    /* Call Id */
    call_index = cp_data[0U];

    /* Get index from the Call Id */
    retval = appl_tbs_get_call_index_from_call_id
             (
                 srvc_index,
                 call_index,
                 &i
             );
    if (GA_SUCCESS == retval)
    {
        if (TBS_CALL_STATE_INCOMING ==
            appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_ACTIVE;

            cp_result = TBS_CP_NTF_RESULT_SUCCESS;
        }
        else
        {
            cp_result = TBS_CP_NTF_RESULT_STATE_MISMATCH;
        }
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_INVALID_CALL_INDEX;
    }

    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );
    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = CCP_CP_OP_ACCEPT;
    if (CCP_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = call_index;
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );
}

void appl_tbs_handle_terminate
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT retval;
    UCHAR call_index;
    UCHAR i;
    UCHAR cp_result;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_TRC("Invalid parameters !\n");
        return;
    }

    call_index = cp_data[0U];

    /* Get index from the Call Id */
    retval = appl_tbs_get_call_index_from_call_id
             (
                 srvc_index,
                 call_index,
                 &i
             );
    if (GA_SUCCESS == retval)
    {
        if (TBS_CALL_STATE_LR_HELD >=
            appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_INVALID;

            cp_result = TBS_CP_NTF_RESULT_SUCCESS;
        }
        else if (TBS_CALL_STATE_INVALID ==
            appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            cp_result = TBS_CP_NTF_RESULT_INVALID_CALL_INDEX;
        }
        else
        {
            cp_result = TBS_CP_NTF_RESULT_STATE_MISMATCH;
        }
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_INVALID_CALL_INDEX;
    }

    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        appl_tbs_se_params[srvc_index].term_reason->call_indx = call_index;
        appl_tbs_se_params[srvc_index].term_reason->reason =
            TBS_CALL_TER_RSN_CLI_TER_CALL;

        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_TERMINATION_REASON;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );

        /* Update Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;

        BT_start_timer
        (
            &appl_ccp_se_terminate_reason_timer_handle,
            1U,
            appl_ccp_se_terminate_reason_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );
    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = TBS_CP_OP_TERMINATE;
    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = call_index;
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );
}

void appl_tbs_handle_local_hold
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT  retval;
    UCHAR      call_index;
    UCHAR      i;
    UCHAR      cp_result;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    call_index = cp_data[0U];

    /* Get index from the Call Id */
    retval = appl_tbs_get_call_index_from_call_id
             (
                 srvc_index,
                 call_index,
                 &i
             );
    if (GA_SUCCESS == retval)
    {
        if ((TBS_CALL_STATE_INCOMING ==
             appl_tbs_se_params[srvc_index].call_state[i].call_state) ||
            (TBS_CALL_STATE_ACTIVE ==
                appl_tbs_se_params[srvc_index].call_state[i].call_state))
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_L_HELD;
            cp_result = TBS_CP_NTF_RESULT_SUCCESS;
        }
        else if (TBS_CALL_STATE_R_HELD ==
                 appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_LR_HELD;
            cp_result = TBS_CP_NTF_RESULT_SUCCESS;
        }
        else
        {
            cp_result = TBS_CP_NTF_RESULT_STATE_MISMATCH;
        }
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_INVALID_CALL_INDEX;
    }


    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );

    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = CCP_CP_OP_L_HLD;
    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = call_index;
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );
}

void appl_tbs_handle_local_retrieve
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT  retval;
    UCHAR      call_index;
    UCHAR      i;
    UCHAR      cp_result;
    UCHAR      k;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    call_index = cp_data[0U];

    /* Get index from the Call Id */
    retval = appl_tbs_get_call_index_from_call_id
             (
                 srvc_index,
                 call_index,
                 &i
             );
    if (GA_SUCCESS == retval)
    {
        /* Default */
        cp_result = TBS_CP_NTF_RESULT_SUCCESS;

        if (TBS_CALL_STATE_L_HELD ==
            appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_ACTIVE;
        }
        else if (TBS_CALL_STATE_LR_HELD ==
                 appl_tbs_se_params[srvc_index].call_state[i].call_state)
        {
            appl_tbs_se_params[srvc_index].call_state[i].call_state =
                TBS_CALL_STATE_R_HELD;
        }
        else
        {
            cp_result = TBS_CP_NTF_RESULT_STATE_MISMATCH;
        }
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_INVALID_CALL_INDEX;
    }


    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        for (k = 0U; k < APPL_TBS_SE_BRR_CALL_LIST; k++)
        {
            if (i != k)
            {
                if (TBS_CALL_STATE_LR_HELD ==
                    appl_tbs_se_params[srvc_index].call_state[k].call_state)
                {
                    appl_tbs_se_params[srvc_index].call_state[k].call_state =
                        TBS_CALL_STATE_R_HELD;
                }
                else if (TBS_CALL_STATE_ACTIVE ==
                         appl_tbs_se_params[srvc_index].call_state[k].call_state)
                {
                    appl_tbs_se_params[srvc_index].call_state[k].call_state =
                        TBS_CALL_STATE_L_HELD;
                }
                else if (TBS_CALL_STATE_R_HELD ==
                         appl_tbs_se_params[srvc_index].call_state[k].call_state)
                {
                    appl_tbs_se_params[srvc_index].call_state[k].call_state =
                        TBS_CALL_STATE_LR_HELD;
                }
            }
        }

        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );
    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = CCP_CP_OP_L_RTRV;
    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = call_index;
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );
}

void appl_tbs_handle_local_join
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT  retval;
    UCHAR      call_index;
    UCHAR      index;
    UCHAR      i;
    UCHAR      cp_result;
    UCHAR      k;
    UCHAR      cp_ids[APPL_TBS_SE_BRR_CALL_LIST];
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    GA_mem_set(&cp_ids, 0U, sizeof(UCHAR)*APPL_TBS_SE_BRR_CALL_LIST);

    cp_result = TBS_CP_NTF_RESULT_SUCCESS;
    if (APPL_TBS_SE_BRR_CALL_LIST > cp_data_len)
    {
        if ((cp_data_len < 2U)||
            (0U == appl_nwt_join_support_flag))
        {
            cp_result = TBS_CP_NTF_RESULT_OP_NOT_POSSIBLE;
        }
        else
        {
            for (k = 0U; k < cp_data_len; k++)
            {
                /* Call Id */
                call_index = cp_data[k];

                /* Get index from the Call Id */
                retval = appl_tbs_get_call_index_from_call_id
                         (
                             srvc_index,
                             call_index,
                             &index
                         );
                if (GA_SUCCESS != retval)
                {
                    cp_result = TBS_CP_NTF_RESULT_OP_NOT_POSSIBLE;
                    break;
                }

                if (TBS_CALL_STATE_INCOMING ==
                    appl_tbs_se_params[srvc_index].call_state[index].call_state)
                {
                    cp_result = TBS_CP_NTF_RESULT_OP_NOT_POSSIBLE;
                    break;
                }
            }
        }

        if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
        {
            GA_mem_copy(&cp_ids, cp_data, cp_data_len);

            /* Change calls state which are part of Join CP */
            for (k = 0U; k < cp_data_len; k++)
            {
                /* Get index from the Call Id */
                retval = appl_tbs_get_call_index_from_call_id
                         (
                             srvc_index,
                             cp_data[k],
                             &index
                         );
                if (GA_SUCCESS != retval)
                {
                    cp_result = TBS_CP_NTF_RESULT_OP_NOT_POSSIBLE;
                    break;
                }

                for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
                {
                    if (appl_tbs_se_params[srvc_index].call_state[i].call_index ==
                        cp_data[k])
                    {
                        cp_ids[i] = 0x01U;

                        switch (appl_tbs_se_params[srvc_index].call_state[index].call_state)
                        {
                        case TBS_CALL_STATE_L_HELD:
                            appl_tbs_se_params[srvc_index].call_state[index].call_state =
                                TBS_CALL_STATE_ACTIVE;
                            break;

                        case TBS_CALL_STATE_LR_HELD:
                            appl_tbs_se_params[srvc_index].call_state[index].call_state =
                                TBS_CALL_STATE_R_HELD;
                            break;

                        default:
                            /* No change in the state */
                            ;
                        }/* switch */
                    }
                } /* for */
            } /* for */

            /* Change calls state which are part of Join CP */
            for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
            {
                if ((0U == cp_ids[i]) &&
                    (TBS_CALL_STATE_ACTIVE ==
                     appl_tbs_se_params[srvc_index].call_state[i].call_state))
                {
                    appl_tbs_se_params[srvc_index].call_state[i].call_state =
                        TBS_CALL_STATE_L_HELD;
                }
            }
        } /* if */
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_LACK_OF_RESOURCES;
    }

    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );
    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = TBS_CP_OP_JOIN;
    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = cp_data[0U];
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );

}

void appl_tbs_handle_local_originate
     (
         GA_ENDPOINT * device,
         TBS_HANDLE  tbs_handle,
         UCHAR       srvc_index,
         UCHAR     * cp_data,
         UCHAR       cp_data_len
     )
{
    GA_RESULT  retval;
    UCHAR      index;
    UCHAR      i;
    UCHAR      j;
    UCHAR      cp_result;
    APPL_CCP_SE_NTF_INFO ccp_se_ntf_info;

    /* Param Check */
    if ((NULL == cp_data) ||
        (0U == cp_data_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    /* Initialize */
    i = 0U;
    j = 0U;

    retval = appl_get_free_call_index(srvc_index, &index);
    if (GA_SUCCESS == retval)
    {
        /* When the service instantiates, the Call_Index shall be a number from
         * 1 to 255. With each call, the Call_Index should increment by 1. When
         * the Call_Index reaches 255, it should roll over to 1. The Call_Index
         * of 0 is reserved. The Call_Index shall remain unique for the life of
         * the call. */
        if ((0x00U == g_appl_call_id) || (0xFFU == g_appl_call_id))
        {
            g_appl_call_id = 0x01U;
        }

        appl_tbs_se_params[srvc_index].call_state[index].call_index =
            g_appl_call_id;
        g_appl_call_id += 1U;

        appl_tbs_se_params[srvc_index].call_state[index].call_state =
            TBS_CALL_STATE_DIALING;

        if (cp_data_len > MAX_TBS_URI_LEN)
        {
            cp_data_len = MAX_TBS_URI_LEN;
        }

        GA_mem_copy
        (
            appl_tbs_se_params[srvc_index].call_state[index].uri,
            cp_data,
            cp_data_len
        );
        appl_tbs_se_params[srvc_index].call_state[index].uri_len = cp_data_len;

        /* Printing received data on console */
        for (j = 0U;
            j < appl_tbs_se_params[srvc_index].call_state[index].uri_len;
            j++)
        {
            APPL_TRC
            (
                "%c",
                appl_tbs_se_params[srvc_index].call_state[index].uri[j]
            );
        }
        APPL_TRC("\n");

        for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
        {
            if (i != index)
            {
                if (TBS_CALL_STATE_ACTIVE ==
                    appl_tbs_se_params[srvc_index].call_state[index].call_state)
                {
                    appl_tbs_se_params[srvc_index].call_state[index].call_state =
                        TBS_CALL_STATE_L_HELD;
                }
            }
        }

        cp_result = TBS_CP_NTF_RESULT_SUCCESS;
    }
    else
    {
        cp_result = TBS_CP_NTF_RESULT_LACK_OF_RESOURCES;
    }

    /* For PTS Test */
    if (0x00U != appl_send_invalid_uri_rsp)
    {
        cp_result = TBS_CP_NTF_RESULT_INVALID_OUTGOING_URI;
    }

    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy service handle */
        ccp_se_ntf_info.tbs_handle = tbs_handle;
        /* Copy Char UUID*/
        ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_STATE;
        /* Copy service instance */
        ccp_se_ntf_info.srvc_inst = srvc_index;
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

        BT_start_timer
        (
            &appl_ccp_se_call_state_timer_handle,
            1U,
            appl_ccp_se_call_state_timer_handler,
            &ccp_se_ntf_info,
            sizeof(ccp_se_ntf_info)
        );
    }

    ccp_se_ntf_info.cp_ntf_info.cp_opcode = TBS_CP_OP_ORIGINATE;
    if (TBS_CP_NTF_RESULT_SUCCESS == cp_result)
    {
        ccp_se_ntf_info.cp_ntf_info.call_index =
            appl_tbs_se_params[srvc_index].call_state[index].call_index;
    }
    else
    {
        ccp_se_ntf_info.cp_ntf_info.call_index = 0U;
    }
    ccp_se_ntf_info.cp_ntf_info.result_code = cp_result;
    ccp_se_ntf_info.cp_ntf_info.datalen = 3U;

    /*
     * Notification should be send to the device from which server has
     * received the CP write request.
     */
    /* Copy service handle */
    ccp_se_ntf_info.tbs_handle = tbs_handle;
    /* Copy Char UUID*/
    ccp_se_ntf_info.char_uuid = GA_CHAR_TBS_CALL_CP;
    /* Copy service instance */
    ccp_se_ntf_info.srvc_inst = srvc_index;
    /* Copy device info */
    GA_COPY_BRR_DEVICE(&ccp_se_ntf_info.device, device);

    BT_start_timer
    (
        &appl_ccp_se_call_cp_timer_handle,
        1U,
        appl_ccp_se_call_cp_timer_handler,
        &ccp_se_ntf_info,
        sizeof(ccp_se_ntf_info)
    );

}

GA_RESULT appl_get_free_call_index
          (
              /* IN */ UCHAR   tbs_indx,
              /* IN */ UCHAR * index
          )
{
    GA_RESULT  retval;
    UCHAR      i;

    /* Initialize */
    retval = GA_FAILURE;

    if (APPL_TBS_SE_ENITITES <= tbs_indx)
    {
        APPL_ERR("Invalid Service Index !\n");
        return retval;
    }

    if (NULL == index)
    {
        APPL_ERR("Index is NULL !\n");
        return retval;
    }

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        if (TBS_CALL_STATE_INVALID ==
            appl_tbs_se_params[tbs_indx].call_state[i].call_state)
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

GA_RESULT appl_tbs_setup_incall(UCHAR  tbs_srvc_index, UCHAR * index_in_list)
{
    GA_RESULT retval;
    UCHAR     index;
    UCHAR     i;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;
    i = 0U;

    if ((APPL_TBS_SE_ENITITES <= tbs_srvc_index) ||
        (NULL == index_in_list))
    {
        APPL_ERR("Invalid parameters !\n");
        return retval;
    }

    retval = appl_get_free_call_index
             (
                 tbs_srvc_index,
                 &index
             );
    if (GA_SUCCESS == retval)
    {
        /* When the service instantiates, the Call_Index shall be a number from
         * 1 to 255. With each call, the Call_Index should increment by 1. When
         * the Call_Index reaches 255, it should roll over to 1. The Call_Index
         * of 0 is reserved. The Call_Index shall remain unique for the life of
         * the call. */
        if ((0x00U == g_appl_call_id) || (0xFFU == g_appl_call_id))
        {
            g_appl_call_id = 0x01U;
        }

        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_index =
            g_appl_call_id;
        g_appl_call_id += 1U;

        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_state =
            TBS_CALL_STATE_INCOMING;
        appl_tbs_se_params[tbs_srvc_index].call_state[index].call_flags = 0U;
        EM_str_copy
        (
            &appl_tbs_se_params[tbs_srvc_index].call_state[index].uri[0U],
            APPL_TBS_CALLER_URI
        );
        appl_tbs_se_params[tbs_srvc_index].call_state[index].uri_len =
             (UCHAR )(EM_str_len(appl_tbs_se_params[tbs_srvc_index].call_state[index].uri) + 1U);

        for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
        {
            if (i != index)
            {
                if (TBS_CALL_STATE_ACTIVE ==
                    appl_tbs_se_params[tbs_srvc_index].call_state[i].call_state)
                {
                    appl_tbs_se_params[tbs_srvc_index].call_state[i].call_state =
                        TBS_CALL_STATE_L_HELD;
                }
                else if (TBS_CALL_STATE_R_HELD ==
                    appl_tbs_se_params[tbs_srvc_index].call_state[i].call_state)
                {
                    appl_tbs_se_params[tbs_srvc_index].call_state[i].call_state =
                        TBS_CALL_STATE_LR_HELD;
                }
            }
        }
    }

    *index_in_list = index;

    return retval;
}

GA_RESULT appl_tbs_get_call_index_from_call_id
          (
              /* IN */ UCHAR   tbs_srvc_index,
              /* IN */ UCHAR   call_id,
              /* IN */ UCHAR * index
          )
{
    GA_RESULT retval;
    UCHAR     i;

    if (NULL == index)
    {
        APPL_ERR("Index is NULL !\n");
        return GA_FAILURE;
    }

    /* Initialize */
    retval = GA_FAILURE;

    for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
    {
        if (call_id ==
            appl_tbs_se_params[tbs_srvc_index].call_state[i].call_index)
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

/* This is for GTBS/SR/SPN/BV-09-C */
void appl_set_large_call_state_info
     (
         UCHAR  * call_state_info,
         UINT16 * call_state_info_len
     )
{
    UCHAR i;
    UCHAR offset;

    if ((NULL == call_state_info) ||
        (NULL == call_state_info_len))
    {
        APPL_ERR("Invalid parameters !\n");
        return;
    }

    offset = 0U;

    for (i = 0U; i < 8U; i++)
    {
        call_state_info[offset] = (i + 1U);
        offset += 1U;

        if (0U == i)
        {
            call_state_info[offset] = 0x03U;
        }
        else
        {
            call_state_info[offset] = 0x04U;
        }
        offset += 1U;

        call_state_info[offset] = 0U;
        offset += 1U;
    }

    *call_state_info_len = offset;
}

/* =================== */

void appl_tbs_set_bearer_name(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer Provider Name(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Bearer Provider Name\n");
        CONSOLE_IN("%s", appl_tbs_se_params[srvc_index].bearer_pname);
        appl_tbs_se_params[srvc_index].bearer_pname_len =
            (UCHAR)EM_str_len(appl_tbs_se_params[srvc_index].bearer_pname) + 1U;
    }
}

void appl_tbs_set_bearer_uci(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer UCI Name(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Bearer UCI\n");
        CONSOLE_IN("%s", appl_tbs_se_params[srvc_index].bearer_uci);
        appl_tbs_se_params[srvc_index].bearer_uci_len =
            (UCHAR)EM_str_len(appl_tbs_se_params[srvc_index].bearer_uci) + 1U;
    }
}

void appl_tbs_set_bearer_technology(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer Technology(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");

            return;
        }

        CONSOLE_OUT("Select Bearer Technology(in Hex)\n");
        CONSOLE_OUT("\t1 -> TBS_BRR_TECH_3G\n");
        CONSOLE_OUT("\t2 -> TBS_BRR_TECH_4G\n");
        CONSOLE_OUT("\t3 -> TBS_BRR_TECH_LTE\n");
        CONSOLE_OUT("\t4 -> TBS_BRR_TECH_WIFI\n");
        CONSOLE_OUT("\t5 -> TBS_BRR_TECH_5G\n");
        CONSOLE_OUT("\t6 -> TBS_BRR_TECH_GSM\n");
        CONSOLE_OUT("\t7 -> TBS_BRR_TECH_CDMA\n");
        CONSOLE_OUT("\t8 -> TBS_BRR_TECH_2G\n");
        CONSOLE_OUT("\t9 -> TBS_BRR_TECH_WCDMA\n");
        CONSOLE_OUT("\ta -> TBS_BRR_TECH_IP\n");

        CONSOLE_IN("%x", &input);

        if (0x0AU < (UINT8)input)
        {
            APPL_INF("Invalid Option\n");
            return;
        }
        appl_tbs_se_params[srvc_index].brr_tech = (UCHAR)input;
    }
}

void appl_tbs_set_bearer_supp_uri_schemes(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer URI Schemes(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Select URI Schemes\n");
        CONSOLE_OUT("\t1 -> mailto\n");
        CONSOLE_OUT("\t2 -> tel\n");
        CONSOLE_OUT("\t3 -> tel,mailto\n");
        CONSOLE_OUT("\t4 -> tel,mailto,news,chrome,bitcoin,aim\n");
        CONSOLE_OUT("\t5 -> chrome,mailto,tel,news,bitcoin,aim\n");

        CONSOLE_IN("%d", &input);

        if (1 == input)
        {
            EM_str_copy
            (
                &appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list[0U],
                TBS_URI_ID_MAILTO
            );

            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len =
                (UCHAR)EM_str_len(TBS_URI_ID_MAILTO) + 1U;
        }
        else if (2 == input)
        {
            EM_str_copy
            (
                &appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list[0U],
                TBS_URI_ID_TEL
            );

            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len =
                (UCHAR)EM_str_len(TBS_URI_ID_TEL) + 1U;
        }
        else if (3U == input)
        {
            EM_str_copy
            (
                &appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list[0U],
                TBS_URI_ID_TEL_MAILTO
            );

            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len =
                (UCHAR)EM_str_len(TBS_URI_ID_TEL_MAILTO) + 1U;
        }
        else if (4U == input)
        {
            EM_str_copy
            (
                &appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list[0U],
                TBS_URI_ID_LONG_VALUE_V1
            );

            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len =
                (UCHAR)EM_str_len(TBS_URI_ID_LONG_VALUE_V1) + 1U;
        }
        else if (5U == input)
        {
            EM_str_copy
            (
                &appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list[0U],
                TBS_URI_ID_LONG_VALUE_V2
            );

            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len =
                (UCHAR)EM_str_len(TBS_URI_ID_LONG_VALUE_V2) + 1U;
        }
        else
        {
            appl_tbs_se_params[srvc_index].brr_uri_pfix_supp_list_len = 0U;
            return;
        }
    }
}

void appl_tbs_set_bearer_signal_strength(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer Signal Strength(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }
        CONSOLE_OUT("Enter Bearer Signal(in Hex)\n");
        CONSOLE_OUT("0x0 - 0x64 -> Valid Signal Strength\n");
        CONSOLE_OUT("0xFF -> Signal Strength Unavailable\n");
        CONSOLE_IN("%x", &input);

        if (0xFF < input)
        {
            APPL_INF("Invalid Option\n");
            return;
        }
        appl_tbs_se_params[srvc_index].brr_sig_strength = (UCHAR)input;
    }
}

void appl_tbs_set_bearer_signal_strength_reporting_interval(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer Signal Strength Reporting Interval(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }
        CONSOLE_OUT("Enter Bearer Signal Strenght Report in(< 255)\n");
        CONSOLE_IN("%d", &input);

        appl_tbs_se_params[srvc_index].brr_sig_strength_reptg_intval =
            (UCHAR)input;
    }
}

void appl_tbs_set_bearer_current_call_list(void)
{
    int    input;
    UCHAR  srvc_index;
    UCHAR  call_index;

#if 0
    CONSOLE_OUT("Set Bearer Current Call List (0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter the Call Index(0, or , 1)\n");
        CONSOLE_IN("%d", &input);
        call_index = (UCHAR)input;

        appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_index =
            (call_index + 1U);

        CONSOLE_OUT("Enter Call State\n");
        CONSOLE_OUT("0 -> Incoming\n");
        CONSOLE_OUT("1 -> Dialing\n");
        CONSOLE_OUT("2 -> Alerting\n");
        CONSOLE_OUT("3 -> Active\n");
        CONSOLE_OUT("4 -> Local Held\n");
        CONSOLE_OUT("5 -> Remote Held \n");
        CONSOLE_OUT("6 -> Local & Remote Held\n");
        CONSOLE_IN("%d", &input);
        appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_state =
            (UCHAR)input;

        CONSOLE_OUT("Enter Call Flags\n");
        appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags = 0U;
        CONSOLE_OUT("0 -> Incoming\n");
        CONSOLE_OUT("1 -> Outgoing\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Incoming */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags &= 0xF7U;
        }
        else
        {
            /* Outgoing */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags |= 0x01U;
        }

        CONSOLE_OUT("0 -> Not WithHeld By Server\n");
        CONSOLE_OUT("1 -> WithHeld By Server\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Not WithHeld */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags &= 0xFDU;
        }
        else
        {
            /* WithHeld */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags |= 0x02U;
        }

        CONSOLE_OUT("0 -> Not WithHeld By Network\n");
        CONSOLE_OUT("1 -> WithHeld By Network\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Not WithHeld */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags &= 0xFBU;
        }
        else
        {
            /* WithHeld */
            appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].call_flags |= 0x04U;
        }

        CONSOLE_OUT("Enter Caller URI\n");
        CONSOLE_IN("%s", &appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].uri[0U]);

        appl_tbs_se_params[srvc_index].brr_list_curr_list->uri_len =
            (UCHAR)EM_str_len(&appl_tbs_se_params[srvc_index].brr_list_curr_list[call_index].uri) + 1U;
    }
}

void appl_tbs_set_bearer_content_control_id(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Bearer Content Control ID(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Bearer Content Control Id(in Hex)\n");
        CONSOLE_IN("%x", &input);

        appl_tbs_se_params[srvc_index].content_control_id = (UCHAR)input;
    }
}

void appl_tbs_set_incomming_caller_target_info(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Incoming Caller Target Info(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Call Index \n");
        CONSOLE_IN("%d", &input);

        appl_tbs_se_params[srvc_index].incmg_caller_trgt_id->call_indx =
            (UCHAR)input;

        CONSOLE_OUT("Enter Caller Target URI\n");
        CONSOLE_IN("%s", &appl_tbs_se_params[srvc_index].incmg_caller_trgt_id->uri[0U]);

        appl_tbs_se_params[srvc_index].incmg_caller_trgt_id->uri_len =
            (UCHAR)EM_str_len(appl_tbs_se_params[srvc_index].incmg_caller_trgt_id->uri) + 1U;
    }
}

void appl_tbs_set_status_flags(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Status Flags(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 8 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }
        CONSOLE_OUT("Enter Status Flags(in Hex)\n");
        CONSOLE_IN("%x", &input);

        appl_tbs_se_params[srvc_index].status_flags = (UINT16)input;
    }

}

void appl_tbs_set_call_state(void)
{
    int    input;
    UCHAR  srvc_index;
    UCHAR  call_index;

#if 0
    CONSOLE_OUT("Set Call State (0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter the Call Index\n");
        CONSOLE_IN("%d", &input);
        call_index = (UCHAR)input;

        CONSOLE_OUT("Enter Call State\n");
        CONSOLE_OUT("0 -> Incoming\n");
        CONSOLE_OUT("1 -> Dialing\n");
        CONSOLE_OUT("2 -> Alerting\n");
        CONSOLE_OUT("3 -> Active\n");
        CONSOLE_OUT("4 -> Local Held\n");
        CONSOLE_OUT("5 -> Remote Held \n");
        CONSOLE_OUT("6 -> Local & Remote Held\n");
        CONSOLE_OUT("> 6 Invalid State\n");
        CONSOLE_IN("%d", &input);
        if (input > 6)
        {
            input = TBS_CALL_STATE_INVALID;
        }

        if (0U == appl_tbs_se_params[srvc_index].call_state[call_index].call_index)
        {
            /* If the call_index is invalid, then assign new index */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_index =
                g_appl_call_id++;
        }

        appl_tbs_se_params[srvc_index].call_state[call_index].call_state =
            (UCHAR)input;

        CONSOLE_OUT("Enter Call Flags\n");
        appl_tbs_se_params[srvc_index].call_state[call_index].call_flags = 0U;
        CONSOLE_OUT("0 -> Incoming\n");
        CONSOLE_OUT("1 -> Outgoing\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Incoming */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags &= 0xF7U;
        }
        else
        {
            /* Outgoing */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags |= 0x01U;
        }

        CONSOLE_OUT("0 -> Not WithHeld By Server\n");
        CONSOLE_OUT("1 -> WithHeld By Server\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Not WithHeld */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags &= 0xFDU;
        }
        else
        {
            /* WithHeld */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags |= 0x02U;
        }

        CONSOLE_OUT("0 -> Not WithHeld By Network\n");
        CONSOLE_OUT("1 -> WithHeld By Network\n");
        CONSOLE_IN("%d", &input);
        if (0 == input)
        {
            /* Not WithHeld */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags &= 0xFBU;
        }
        else
        {
            /* WithHeld */
            appl_tbs_se_params[srvc_index].call_state[call_index].call_flags |= 0x04U;
        }
    }
}

void appl_tbs_set_optional_cp_opcodes(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Optional CP Opcodes (0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Optional CP Opcodes(in Hex)\n");
        CONSOLE_IN("%x", &input);

        appl_tbs_se_params[srvc_index].opt_cp_opcodes = (UINT16)input;
    }
}

void appl_tbs_set_incoming_caller_info(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Incoming Caller Info(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Call Index \n");
        CONSOLE_IN("%d", &input);

        appl_tbs_se_params[srvc_index].incmg_caller_info->call_indx =
            (UCHAR)input;

        CONSOLE_OUT("Enter Caller URI\n");
        CONSOLE_IN
        (
            "%s",
            &appl_tbs_se_params[srvc_index].incmg_caller_info->uri[0U]
        );

        appl_tbs_se_params[srvc_index].incmg_caller_info->uri_len =
            (UCHAR)EM_str_len(appl_tbs_se_params[srvc_index].incmg_caller_info->uri) + 1U;
    }
}

void appl_tbs_set_caller_friendly_name(void)
{
    int    input;
    UCHAR  srvc_index;

#if 0
    CONSOLE_OUT("Set Caller Friendly Name(0/1):");
    CONSOLE_IN("%d", &input);
    if (0 != input)
#endif /* 0 */
    {
        CONSOLE_OUT("Enter the TBS Server Index \n");
        CONSOLE_IN("%d", &input);
        srvc_index = (UCHAR)input;

        if (APPL_TBS_SE_ENITITES <= srvc_index)
        {
            APPL_ERR("Selected Invalid Service Index !\n");
            APPL_ERR("Please Try again !\n");
            return;
        }

        CONSOLE_OUT("Enter Call Index \n");
        CONSOLE_IN("%d", &input);

        appl_tbs_se_params[srvc_index].frndly_name->call_indx = (UCHAR)input;

        CONSOLE_OUT("Enter Caller Name\n");
        CONSOLE_IN("%s", &appl_tbs_se_params[srvc_index].frndly_name->name[0U]);

        appl_tbs_se_params[srvc_index].frndly_name->name_len =
            (UCHAR)EM_str_len(appl_tbs_se_params[srvc_index].frndly_name->name) + 1U;
    }
}

void appl_tbs_set_char_value(void)
{
    int  rd_val;

    CONSOLE_OUT("Select below options\n");
    CONSOLE_OUT(" 1 -> Set Bearer Provider Name \n");
    CONSOLE_OUT(" 2 -> Set Bearer UCI \n");
    CONSOLE_OUT(" 3 -> Set Bearer Technology \n");
    CONSOLE_OUT(" 4 -> Set Bearer URI Schemes Supported \n");
    CONSOLE_OUT(" 5 -> Set Bearer Signal Strength \n");
    CONSOLE_OUT(" 6 -> Set Bearer Signal Reporting Interval \n");
    CONSOLE_OUT(" 7 -> Set Bearer Current Calls List \n");
    CONSOLE_OUT(" 8 -> Set Content Control ID \n");
    CONSOLE_OUT(" 9 -> Set Incoming Caller Target Info. \n");
    CONSOLE_OUT("10 -> Set Status Flags \n");
    CONSOLE_OUT("11 -> Set Call State \n");
    CONSOLE_OUT("12 -> Set Control Point Optional Opcodes \n");
    CONSOLE_OUT("13 -> Set Incoming Caller Info. \n");
    CONSOLE_OUT("14 -> Set Call Friendly Name \n");
    CONSOLE_IN("%d", &rd_val);

    switch (rd_val)
    {
    case 1:
        appl_tbs_set_bearer_name();
        break;

    case 2:
        appl_tbs_set_bearer_uci();
        break;

    case 3:
        appl_tbs_set_bearer_technology();
        break;

    case 4:
        appl_tbs_set_bearer_supp_uri_schemes();
        break;

    case 5:
        appl_tbs_set_bearer_signal_strength();
        break;

    case 6:
        appl_tbs_set_bearer_signal_strength_reporting_interval();
        break;

    case 7:
        appl_tbs_set_bearer_current_call_list();
        break;

    case 8:
        appl_tbs_set_bearer_content_control_id();
        break;

    case 9:
        appl_tbs_set_incomming_caller_target_info();
        break;

    case 10:
        appl_tbs_set_status_flags();
        break;

    case 11:
        appl_tbs_set_call_state();
        break;

    case 12:
        appl_tbs_set_optional_cp_opcodes();
        break;

    case 13:
        appl_tbs_set_incoming_caller_info();
        break;

    case 14:
        appl_tbs_set_caller_friendly_name();
        break;

    default:
        APPL_INF("Invalid Choice\n");
    }
}

void appl_tbs_notify_bearer_provider_name(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_BRR_PROVIDER_NAME,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_provider_name...\n");
    retval = GA_ccp_se_ntf_brr_provider_name
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_bearer_technology(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_BRR_TECHNOLOGY,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_technology...\n");
    retval = GA_ccp_se_ntf_brr_technology
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);

    return;
}

void appl_tbs_notify_bearer_supp_uri_schemes(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_uri_schms_supp_list...\n");
    retval = GA_ccp_se_ntf_brr_uri_schms_supp_list
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_bearer_signal_strength(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_BRR_SIGSTRENGTH,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_sig_strength...\n");
    retval = GA_ccp_se_ntf_brr_sig_strength
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_curr_call_list(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_BRR_LIST_CUR_CALLS,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_cur_call_list...\n");
    retval = GA_ccp_se_ntf_brr_cur_call_list
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_status_flags(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_STATUS_FLAGS,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_brr_status_flags..\n");
    retval = GA_ccp_se_ntf_brr_status_flags
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_incoming_call_target_bearer_uri(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_incoming_call_trgt_brr_uri...\n");
    retval = GA_ccp_se_ntf_incoming_call_trgt_brr_uri
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_call_state(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_CALL_STATE,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    if (1U == appl_set_long_call_state_flag)
    {
        appl_set_large_call_state_info(&appl_chr_data[0U], &appl_chr_datalen);
    }

    APPL_TRC("GA_ccp_se_ntf_call_state...\n");
    retval = GA_ccp_se_ntf_call_state
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_cp(void)
{
    int        choice;
    UCHAR      appl_srvs_handle;
    GA_RESULT  retval;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    appl_chr_data[0U] = TBS_CALL_STATE_INCOMING;
    appl_chr_data[1U] = 0x01U;
    appl_chr_data[2U] = 0x00U;
    appl_chr_datalen = 3U;

    APPL_TRC("GA_ccp_se_ntf_call_cp...\n");
    retval = GA_ccp_se_ntf_call_cp
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_termination_reason(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
                (
                    GA_CHAR_TBS_TERMINATION_REASON,
                    &appl_tbs_se_params[appl_srvs_handle],
                    &appl_chr_data[0U],
                    &appl_chr_datalen
                );

    APPL_TRC("GA_ccp_se_ntf_terminate_reason...\n");
    retval = GA_ccp_se_ntf_terminate_reason
                (
                    &ga_ccp_se_peer_bd_addr,
                    appl_srvs_handle,
                    &appl_chr_data[0U],
                    appl_chr_datalen
                );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_incoming_call(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_INCOMING_CALL,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_incoming_call...\n");
    retval = GA_ccp_se_ntf_incoming_call
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_call_friendly_name(void)
{
    int        choice;
    GA_RESULT  retval;
    UCHAR      appl_srvs_handle;

    if (0xFFU == appl_active_service)
    {
        CONSOLE_OUT("Select GTBS/TBS(0/1):\n");
        CONSOLE_IN("%x", &choice);
        if (0 == choice)
        {
            appl_srvs_handle = appl_ccp_se_gtbs_handle;
        }
        else
        {
            appl_srvs_handle = appl_ccp_se_tbs_handle;
        }
    }
    else
    {
        appl_srvs_handle = appl_active_service;
    }

    retval = appl_ccp_se_pack_tbs_chr_value
             (
                 GA_CHAR_TBS_CALL_FRIENDLY_NAME,
                 &appl_tbs_se_params[appl_srvs_handle],
                 &appl_chr_data[0U],
                 &appl_chr_datalen
             );

    APPL_TRC("GA_ccp_se_ntf_call_friendly_name...\n");
    retval = GA_ccp_se_ntf_call_friendly_name
             (
                 &ga_ccp_se_peer_bd_addr,
                 appl_srvs_handle,
                 &appl_chr_data[0U],
                 appl_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_ccp_display_ga_result(retval);
}

void appl_tbs_notify_char_value(void)
{
    int  rd_val;

    CONSOLE_OUT("Select below options\n");
    CONSOLE_OUT("10 -> CCP Notify Bearer Provider Name\n");
    CONSOLE_OUT("11 -> CCP Notify Bearer Technology\n");
    CONSOLE_OUT("12 -> CCP Notify Bearer URI Schemes Supported\n");
    CONSOLE_OUT("13 -> CCP Notify Bearer Signal Strength\n");
    CONSOLE_OUT("14 -> CCP Notify Bearer Current Calls List\n");
    CONSOLE_OUT("15 -> CCP Notify Bearer Status Flags\n");
    CONSOLE_OUT("16 -> CCP Notify Bearer Incall Target Brr URI\n");
    CONSOLE_OUT("17 -> CCP Notify Bearer Call State\n");
    CONSOLE_OUT("18 -> CCP Notify Bearer Call Control Point\n");
    CONSOLE_OUT("19 -> CCP Notify Terminate Reason\n");
    CONSOLE_OUT("20 -> CCP Notify Incoming Call\n");
    CONSOLE_OUT("21 -> CCP Notify Call Friendly Name\n");
    CONSOLE_IN("%d", &rd_val);

    switch (rd_val)
    {
    case 10:
        appl_tbs_notify_bearer_provider_name();
        break;

    case 11:
        appl_tbs_notify_bearer_technology();
        break;

    case 12:
        appl_tbs_notify_bearer_supp_uri_schemes();
        break;

    case 13:
        appl_tbs_notify_bearer_signal_strength();
        break;

    case 14:
        appl_tbs_notify_curr_call_list();
        break;

    case 15:
        appl_tbs_notify_status_flags();
        break;

    case 16:
        appl_tbs_notify_incoming_call_target_bearer_uri();
        break;

    case 17:
        appl_tbs_notify_call_state();
        break;

    case 18:
        appl_tbs_notify_cp();
        break;

    case 19:
        appl_tbs_notify_termination_reason();
        break;

    case 20:
        appl_tbs_notify_incoming_call();
        break;

    case 21:
        appl_tbs_notify_call_friendly_name();
        break;

    default:
        APPL_INF("Invalid Choice");
        break;
    }
}

void appl_set_tbs_sdp_record_status(UCHAR flag)
{
#if 0
    API_RESULT retval;
    UINT32 record_handle;

    /* Get the TBS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_TBS, 0U, &record_handle);

    if (0U != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }
#else
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}

void appl_ccp_se_display_response_data(CCP_RSP_INF * tbs_rsp_info, UINT16 offset_rcvd)
{
    UINT16 char_uuid;
    UCHAR  i;
    UCHAR  j;
    UCHAR rx_datalen;
    UINT16 offset;
    UCHAR item_len;

    if (NULL == tbs_rsp_info)
    {
        APPL_ERR("tbs_rsp_info is NULL !\n");
        return;
    }

    /* Initialize */
    i = 0U;
    j = 0U;
    rx_datalen = 0U;
    offset = 0U;

    char_uuid = tbs_rsp_info->chr->uuid;

    switch(char_uuid)
    {
        case GA_CHAR_TBS_BRR_PROVIDER_NAME:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer Provider Name: %s\n",
                tbs_rsp_info->datalen,
                tbs_rsp_info->data
            );
            break;

        case GA_CHAR_TBS_BRR_UCI:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer UCI: %s\n",
                tbs_rsp_info->datalen,
                tbs_rsp_info->data
            );
            break;

        case GA_CHAR_TBS_BRR_TECHNOLOGY:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer Technology: ",
                tbs_rsp_info->datalen
            );
            appl_ccp_utils_parse_and_display_ccp_brr_tech
            (
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer URI Schemes Supported List: %s\n",
                tbs_rsp_info->datalen,
                tbs_rsp_info->data
            );
            break;

        case GA_CHAR_TBS_BRR_SIGSTRENGTH:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer Signal Strength: ",
                tbs_rsp_info->datalen
            );
            appl_ccp_utils_parse_and_display_ccp_brr_signal_strength
            (
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL:
            APPL_TRC
            (
                "Len: 0x%02X,  Bearer Signal Strength Reporting Interval: 0x%02X\n",
                tbs_rsp_info->datalen,
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_BRR_LIST_CUR_CALLS:
            rx_datalen = tbs_rsp_info->datalen;
            for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
            {
                APPL_TRC("%u.  ", (i + 1U));
                item_len = (tbs_rsp_info->data)[offset];
                APPL_TRC
                (
                    "Len: 0x%02X,  List Item Length: 0x%02X\n",
                    (UCHAR)sizeof(item_len),
                    item_len
                );
                offset++;
                APPL_TRC
                (
                    "    Len: 0x%02X,  Call Index: 0x%02X\n",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset]),
                    (tbs_rsp_info->data)[offset]
                );
                offset++;
                APPL_TRC
                (
                    "    Len: 0x%02X,  Call State: ",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset])
                );
                appl_ccp_utils_parse_and_display_ccp_call_state
                (
                    (tbs_rsp_info->data)[offset]
                );
                offset++;
                APPL_TRC
                (
                    "    Len: 0x%02X,  Call Flags: ",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset])
                );
                appl_ccp_utils_parse_and_display_ccp_call_flags
                (
                    (tbs_rsp_info->data)[offset]
                );
                offset++;
                APPL_TRC("    Len: 0x%02X,  URI: ", (item_len - 3U));
                for (j = 0U;
                    j < (item_len - 3U);
                    j++)
                {
                    APPL_TRC
                    (
                        "%c",
                        (tbs_rsp_info->data)[offset + j]
                    );
                }
                offset += (item_len - 3U);
                if (offset >= rx_datalen)
                {
                    break;
                }
                APPL_TRC("\n");
            }
            break;

        case GA_CHAR_CONTENT_CONTROL_ID:
            APPL_TRC
            (
                "Len: 0x%02X,  Content Control ID: 0x%02X\n",
                tbs_rsp_info->datalen,
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_STATUS_FLAGS:
            APPL_TRC
            (
                "Len: 0x%02X,  Status Flags: ",
                tbs_rsp_info->datalen
            );
            appl_ccp_utils_parse_and_display_ccp_status_flags
            (
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI:
            if (0U == offset_rcvd)
            {
                APPL_TRC
                (
                    "Len: 0x01,  Call Index: 0x%02X\n",
                    ((tbs_rsp_info->data)[offset])
                );
                offset++;
            }
            APPL_TRC
            (
                "Len: 0x%02X,  Incoming Call Target URI: ",
                ((tbs_rsp_info->datalen) - offset)
            );
            for (j = 0U;
                j < ((tbs_rsp_info->datalen) - offset);
                j++)
            {
                APPL_TRC
                (
                    "%c",
                    ((tbs_rsp_info->data)[j + offset])
                );
            }
            APPL_TRC("\n");
            break;

        case GA_CHAR_TBS_CALL_STATE:
            rx_datalen = tbs_rsp_info->datalen;
            for (i = 0U; i < APPL_TBS_SE_BRR_CALL_LIST; i++)
            {
                APPL_TRC("%u.  ", (i + 1U));
                APPL_TRC
                (
                    "Len: 0x%02X,  Call Index: 0x%02X\n",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset]),
                    ((tbs_rsp_info->data)[offset])
                );
                offset++;

                APPL_TRC
                (
                    "    Len: 0x%02X,  State: 0x%02X\n",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset]),
                    ((tbs_rsp_info->data)[offset])
                );

                offset++;
                APPL_TRC
                (
                    "    Len: 0x%02X,  Call Flags: 0x%02X\n\n",
                    (UCHAR)sizeof((tbs_rsp_info->data)[offset]),
                    ((tbs_rsp_info->data)[offset])
                );
                offset++;

                if (offset >= rx_datalen)
                {
                    break;
                }
            }
            break;

        case GA_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES:
            APPL_TRC
            (
                "Len: 0x%02X,  Call Control Point Optional Opcodes: ",
                tbs_rsp_info->datalen
            );
            appl_ccp_utils_parse_and_display_ccp_cp_opt_opcodes
            (
                *(tbs_rsp_info->data)
            );
            break;

        case GA_CHAR_TBS_INCOMING_CALL:
            if (0U == offset_rcvd)
            {
                APPL_TRC
                (
                    "Len: 0x01,  Call Index: 0x%02X\n",
                    ((tbs_rsp_info->data)[offset])
                );
                offset++;
            }

            if (1U < tbs_rsp_info->datalen)
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  URI: ",
                    tbs_rsp_info->datalen - offset
                );
                for (j = 0U;
                    j < tbs_rsp_info->datalen - offset;
                    j++)
                {
                    APPL_TRC
                    (
                        "%c",
                        ((tbs_rsp_info->data)[j + offset])
                    );
                }
                APPL_TRC("\n");
            }
            break;

        case GA_CHAR_TBS_CALL_FRIENDLY_NAME:
            if (0U == offset_rcvd)
            {
                APPL_TRC
                (
                    "Len: 0x01,  Call Index: 0x%02X\n",
                    ((tbs_rsp_info->data)[0U])
                );
                offset++;
            }
            APPL_TRC
            (
                "Len: 0x%02X,  Friendly Name: ",
                tbs_rsp_info->datalen - offset
            );
            for (j = 0U;
                j < tbs_rsp_info->datalen - offset;
                j++)
            {
                APPL_TRC
                (
                    "%c",
                    ((tbs_rsp_info->data)[j + offset])
                );
            }
            APPL_TRC("\n");
            break;

        default:
            APPL_ERR("Invalid UUID in data !\n");
    }
}

void appl_ccp_se_call_state_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT retval;
    APPL_CCP_SE_NTF_INFO * ccp_se_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\n");
    }
    else
    {
        ccp_se_ntf_info = (APPL_CCP_SE_NTF_INFO *)data;

        if (CCP_MAX_TBS_ENTITIES <= ccp_se_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid Handle !\n");
            return;
        }

        retval = appl_ccp_se_pack_tbs_chr_value
                 (
                     ccp_se_ntf_info->char_uuid,
                     &appl_tbs_se_params[ccp_se_ntf_info->srvc_inst],
                     &appl_chr_data[0U],
                     &appl_chr_datalen
                 );

        APPL_TRC("GA_ccp_se_ntf_call_state \n");
        retval = GA_ccp_se_ntf_call_state
                 (
                     &(ccp_se_ntf_info->device),
                     ccp_se_ntf_info->tbs_handle,
                     &appl_chr_data[0U],
                     appl_chr_datalen
                 );
        APPL_TRC("Retval - ");
        appl_ccp_display_ga_result(retval);
    }
    return;
}

void appl_ccp_se_call_cp_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT retval;
    APPL_CCP_SE_NTF_INFO * ccp_se_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\n");
    }
    else
    {
        ccp_se_ntf_info = (APPL_CCP_SE_NTF_INFO *)data;

        if (CCP_MAX_TBS_ENTITIES <= ccp_se_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid Handle !\n");
        }

        /* Pack notification data */
        /* Pack Opcode */
        appl_chr_data[0U] = ccp_se_ntf_info->cp_ntf_info.cp_opcode;
        /* Pack Call Index */
        appl_chr_data[1U] = ccp_se_ntf_info->cp_ntf_info.call_index;
        /* Pack Result Code */
        appl_chr_data[2U] = ccp_se_ntf_info->cp_ntf_info.result_code;
        /* Pack Datalen */
        appl_chr_datalen = ccp_se_ntf_info->cp_ntf_info.datalen;

        APPL_TRC("GA_ccp_se_ntf_call_cp...\n");
        retval = GA_ccp_se_ntf_call_cp
                 (
                     &(ccp_se_ntf_info->device),
                     ccp_se_ntf_info->srvc_inst,
                     &appl_chr_data[0U],
                     appl_chr_datalen
                 );
        APPL_TRC("Retval - ");
        appl_ccp_display_ga_result(retval);
    }
    return;
}

void appl_ccp_se_terminate_reason_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT retval;
    APPL_CCP_SE_NTF_INFO * ccp_se_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\n");
    }
    else
    {
        ccp_se_ntf_info = (APPL_CCP_SE_NTF_INFO *)data;

        if (CCP_MAX_TBS_ENTITIES <= ccp_se_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid Handle !\n");
            return;
        }

        retval = appl_ccp_se_pack_tbs_chr_value
                 (
                     GA_CHAR_TBS_TERMINATION_REASON,
                     &appl_tbs_se_params[ccp_se_ntf_info->srvc_inst],
                     &appl_chr_data[0U],
                     &appl_chr_datalen
                 );
        APPL_TRC("GA_ccp_se_ntf_terminate_reason...\n");
        retval = GA_ccp_se_ntf_terminate_reason
                 (
                     &(ccp_se_ntf_info->device),
                     ccp_se_ntf_info->tbs_handle,
                     &appl_chr_data[0U],
                     appl_chr_datalen
                 );
        APPL_TRC("Retval - ");
        appl_ccp_display_ga_result(retval);
    }
}

#endif /* CCP_SERVER */
