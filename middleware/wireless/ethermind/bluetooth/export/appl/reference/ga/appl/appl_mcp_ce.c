/**
 *  \file appl_mcp_ce.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of MCP - Client role interfaces exposed by MCP Client library.
 *  MCC discovers all the instances of GMCS and MCS present on a particular
 *  peer device. Based on the Service Instance and the Context provided by MCP
 *  Client module, the procedures of MCC on a particular GMCS or MCS instance
 *  can be carried out.
 */

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* --------------------------------------------- Header File Inclusion */
#include "appl_mcp_ce.h"
#include "appl_mcp_utils.h"

#ifdef MCP_CLIENT

/* --------------------------------------------- Global Definitions */
/* Maximum MCS Instances supported by MCC */
#define MCS_MAX_INST 5U

/* Unused MCS Instance */
#define MCS_INST_FREE 0xFU

static const UCHAR ga_mcp_ce_appl_menu[] =
"\n\
================ GA MCP Client MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Profile Management ~ Init ============ \n\
    5. GA MCP CE Init \n\
\n\
================ Profile Management ~ Deinit ============ \n\
    6. GA MCP CE Shutdown \n\
\n\
================ Context Management ~ Setup ============ \n\
   11. Setup GMCS session with Peer Device \n\
   12. Discover MCS instances on remote device \n\
   13. Setup MCS session with Peer Device \n\
   14. Setup OTS session with Peer Device \n\
   15. Connect OTS Transport Channel \n\
   16. Disconnect OTS Transport Channel \n\
   17. Config all Char \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
   18. Set the Instance \n\
\n\
================ Context Management ~ Release ============ \n\
   19. Release GMCS And/Or MCS \n\
\n\
================ Profile Management (GMCS & MCS) ~ Operations(Read) ============ \n\
   20. Read Media Player Name \n\
   21. Set Media Player Icon Obj Reference \n\
   22. Get Media Player Icon Obj Attribute - Size \n\
   23. Read Media Player Icon \n\
   24. Read Media Player Icon URL \n\
   25. Read Track Title \n\
   26. Read Track Duration \n\
   27. Read Track Position \n\
   28. Read Playback Speed \n\
   29. Read Seeking Speed \n\
   30. Set Current Track Seg Obj Reference \n\
   31. Get Current Track Seg Obj Attribute - Size \n\
   32. Read Current Track Seg Obj \n\
   33. Set Current Track Obj Reference \n\
   34. Get Current Track Obj Attribute - Size \n\
   35. Read Current Track Obj \n\
   36. Set Next Track Obj Reference \n\
   37. Get Next Track Obj Attribute - Size \n\
   38. Read Next Track Obj Obj \n\
   39. Set Parent Group Obj Reference \n\
   40. Get Parent Group Obj Attribute - Size \n\
   41. Read Parent Group Obj \n\
   42. Set Current Group Obj Reference \n\
   43. Get Current Group Obj Attribute - Size \n\
   44. Read Current Group Obj \n\
   45. Read Playing Order \n\
   46. Read Playing Orders Supported \n\
   47. Read Media State \n\
   48. Read MCP Opcode Supported \n\
   49. Set Search Results Obj Reference \n\
   50. Get Search Results Obj Attribute - Size \n\
   51. Read Search Results Obj \n\
   52. Read Content Control ID \n\
\n\
================ Profile Management (GMCS & MCS) ~ Operations(Write) ============ \n\
   60. Write Track Pos \n\
   61. Write Playback Speed \n\
   62. Write Curr Track Obj ID \n\
   63. Write Next Track Obj ID \n\
   64. Write Curr Group Obj ID \n\
   65. Write Playing Order \n\
   66. Write MCP \n\
   67. Write SCP \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get GMCS Context Info. \n\
   81. Set GMCS Context Info. \n\
\n\
   82. Get MCS Context Info. \n\
   83. Set MCS Context Info. \n\
\n\
   87. Free Saved GMCS Context Info. \n\
   88. Free Saved MCS Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure Notification \n\
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
DECL_STATIC GA_ENDPOINT appl_ga_mcc_device;

DECL_STATIC MCP_HANDLE appl_mcp_gmcs_handle;
DECL_STATIC MCP_HANDLE appl_mcp_mcs_handle[MCS_MAX_INST];
DECL_STATIC MCP_HANDLE appl_mcp_handle_in_use;

GA_MCP_CE_OBJ_ID obj_id;
UINT32 curr_size;
UINT32 alloc_size;

#ifdef MCP_SUPPORT_CONTEXT_MANAGE
DECL_STATIC MCP_HANDLE appl_mcp_gmcs_ctx_handle;
DECL_STATIC MCP_HANDLE appl_mcp_mcs_ctx_handle;

/* Variables to Save the GMCS Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE *  appl_mcp_gmcs_range_ptr;
DECL_STATIC  GA_CHAR_INFO *      appl_mcp_gmcs_char_info_ptr;
DECL_STATIC  UINT16              appl_mcp_gmcs_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_mcp_gmcs_range;
DECL_STATIC  GA_CHAR_INFO *      appl_mcp_gmcs_char_info;
DECL_STATIC  MCP_HANDLE          appl_mcp_gmcs_handle_access;

/* Variables to Save the MCS Context Information */
typedef struct APPL_MCP_MCS_MANAGE_CTX_INFO
{
    GA_BRR_SVC_RANGE *  appl_mcp_mcs_range_ptr;
    GA_CHAR_INFO *      appl_mcp_mcs_char_info_ptr;
    UINT16              appl_mcp_mcs_char_count;
    GA_BRR_SVC_RANGE    appl_mcp_mcs_range;
    GA_CHAR_INFO *      appl_mcp_mcs_char_info;
    MCP_HANDLE          appl_mcp_mcs_handle_access;
}APPL_MCP_MCS_MANAGE_CTX_INFO;

DECL_STATIC APPL_MCP_MCS_MANAGE_CTX_INFO appl_mcp_mcs_manage_ctx_info[MCP_MAX_MCS_CONTEXTS];
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */

/* --------------------------------------------- Functions */
GA_RESULT appl_mcp_ce_read_sess_inst_for_op(void );
void appl_find_empty_mcs_pos(UINT8 * index);
void appl_mcp_ce_utils_parse_and_display_mcp_opc(UINT8 req_opc);
void appl_mcp_ce_utils_parse_and_display_mcp_res_code(UINT8 res_code);
void appl_mcp_ce_utils_parse_and_display_handle(MCP_HANDLE handle);
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
void appl_mcp_ce_utils_parse_and_display_attr_id_and_value
    (
        GA_MCP_CE_OBJ_ATTR * attr
    );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

void main_mcp_ce_menu_operations(void)
{
    GA_RESULT            retval;
    int                  choice, input, menu_choice;
    UINT8                mcs_ntf_flag;
    GA_MCP_CE_WRITE_REQ  req;
    UINT8                i;

    UINT8 write_req_val[10U] = { 0U };
    UINT8 index = 0U;
    UINT8 len = 0U;

    GA_BRR_SVC_INFO  mcs_svc_info;
    UINT8            mcs_handle_index;
    UINT8            conn_dev_index;

    /* MISRA C - 2012 Rule 9.1 */
    mcs_handle_index = 0U;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ga_mcp_ce_appl_menu);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 5:
            APPL_TRC("GA_mcp_ce_init...\n");
            retval = GA_mcp_ce_init(appl_mcp_ce_cb);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 6:
            APPL_TRC("GA_mcp_ce_shutdown...\n");
            retval = GA_mcp_ce_shutdown();
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 11:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &appl_ga_mcc_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(appl_ga_mcc_device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &input);
                appl_ga_mcc_device.bd_type = (UINT8)input;
            }

            APPL_TRC("GA_mcp_ce_setup_context: \n");
            retval = GA_mcp_ce_setup_context
                     (
                         &appl_ga_mcc_device,
                         &appl_mcp_gmcs_handle
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            APPL_TRC("MCC Remote GMCS Handle: 0x%04X\n", appl_mcp_gmcs_handle);
            break;

        case 12:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &appl_ga_mcc_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(appl_ga_mcc_device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &input);
                appl_ga_mcc_device.bd_type = (UINT8)input;
            }

            APPL_TRC("GA_mcp_ce_discover_mcs: \n");
            retval = GA_mcp_ce_discover_mcs(&appl_ga_mcc_device);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_MCS
            );
            break;

        case 13:
            APPL_TRC("GA_mcp_ce_setup_mcs_context: \n");
            mcs_svc_info.uuid = GA_SERVICE_MCS;
            CONSOLE_OUT("Enter the Start Handle[HEX] : ");
            CONSOLE_IN("%x", &choice);
            mcs_svc_info.range.start = (UINT16)choice;
            CONSOLE_OUT("Enter the End Handle[HEX] : ");
            CONSOLE_IN("%x", &choice);
            mcs_svc_info.range.end = (UINT16)choice;

            appl_find_empty_mcs_pos(&index);
            if (MCS_MAX_INST != index)
            {
                retval = GA_mcp_ce_setup_mcs_context
                         (
                             &appl_ga_mcc_device,
                             &mcs_svc_info,
                             &mcs_handle_index
                         );

                appl_mcp_mcs_handle[index] = mcs_handle_index;

                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_MCS
                );
            }
            else
            {
                APPL_INF("Maximum MCS Instances reached\n");
            }
            break;

        case 14:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            APPL_TRC("Set the instance using Option 18 if not set yet\n");
            APPL_TRC("Setup OTS Session\n");
            retval = GA_mcp_ce_setup_object_ctrl_channel
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 15:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            APPL_TRC("Set the instance using Option 18 if not set yet\n");
            APPL_TRC("Setup OTS Session\n");
            retval = GA_mcp_ce_setup_object_data_channel
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 16:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            APPL_TRC("Set the instance using Option 18 if not set yet\n");
            APPL_TRC("Setup OTS Session\n");
            retval = GA_mcp_ce_release_object_data_channel
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 17:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_configure_context
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 18:
            appl_mcp_ce_read_sess_inst_for_op();
            break;

        case 19:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("Release GMCS And/Or MCS\n");
                APPL_TRC("GA_mcp_ce_release...\n");
                retval = GA_mcp_ce_release(appl_mcp_handle_in_use);
                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_GMCS
                );
            }
            else if (2 == choice)
            {
                APPL_TRC("Close context GMCS And/Or MCS\n");
                APPL_TRC("GA_mcp_ce_close...\n");
                retval = GA_mcp_ce_close(appl_mcp_handle_in_use);
                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_GMCS
                );
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;
#if 0
        case 70:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_media_player_name
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 71:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_track_changed
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 72:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_track_title
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 73:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_track_duration
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 74:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_track_pos
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 75:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_playback_speed
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 76:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_seeking_speed
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 77:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;
            retval = GA_mcp_ce_config_ntf_curr_track_obj_id
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 78:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;
            retval = GA_mcp_ce_config_ntf_next_track_obj_id
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 79:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;
            retval = GA_mcp_ce_config_ntf_parent_grp_obj_id
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 80:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;
            retval = GA_mcp_ce_config_ntf_curr_group_obj_id
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 81:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_playing_order
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 82:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_media_state
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 83:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_mcp
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 84:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_mcp_opc_supp
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 85:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;

            retval = GA_mcp_ce_config_ntf_scp
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 86:
            CONSOLE_OUT
            (
                "Configuration Notificaton : [Disable:0] [Enable: 1]\n"
            );
            CONSOLE_IN("%d", &input);
            mcs_ntf_flag = (UINT8)input;
            retval = GA_mcp_ce_config_ntf_search_results_obj_id
                     (
                         appl_mcp_handle_in_use,
                         mcs_ntf_flag
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;
#endif /* 0 */
        case 20:
            retval = GA_mcp_ce_read_media_player_name(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 21:
            retval = GA_mcp_ce_set_media_player_icon_obj
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 22:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_media_player_icon_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 23:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_media_player_icon_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 24:
            retval = GA_mcp_ce_read_media_player_icon_URL
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 25:
            retval = GA_mcp_ce_read_track_title(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 26:
            retval = GA_mcp_ce_read_track_duration(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 27:
            retval = GA_mcp_ce_read_track_pos(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 28:
            retval = GA_mcp_ce_read_playback_speed(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 29:
            retval = GA_mcp_ce_read_seeking_speed(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 30:
            retval = GA_mcp_ce_set_curr_track_seg_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 31:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_track_seg_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 32:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_track_seg_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 33:
            retval = GA_mcp_ce_set_curr_track_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 34:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_track_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 35:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_track_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 36:
            retval = GA_mcp_ce_set_next_track_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 37:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_next_track_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 38:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_next_track_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 39:
            retval = GA_mcp_ce_set_parent_grp_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 40:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_parent_grp_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 41:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_parent_grp_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 42:
            retval = GA_mcp_ce_set_curr_grp_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 43:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_grp_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 44:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_curr_grp_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 45:
            retval = GA_mcp_ce_read_playing_order(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 46:
            retval = GA_mcp_ce_read_playing_orders_supp
                     (
                         appl_mcp_handle_in_use
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 47:
            retval = GA_mcp_ce_read_media_state(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 48:
            retval = GA_mcp_ce_read_mcp_opc_supp(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 49:
            retval = GA_mcp_ce_set_search_res_obj(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 50:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_search_res_obj_attr
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         MCP_CE_OBJ_ATTR_ID_SIZE
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 51:
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
            retval = GA_mcp_ce_read_search_res_obj
                     (
                         appl_mcp_handle_in_use,
                         &obj_id,
                         0U,
                         curr_size
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_UUID_OTS
            );
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
            APPL_ERR("MCP_SUPPORT_OBJECT_TRANSFER is not defined !\n");
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
            break;

        case 52:
            retval = GA_mcp_ce_read_content_control_ID(appl_mcp_handle_in_use);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 60:
            CONSOLE_OUT("Enter Track Position[32-bit Signed Integer]\n");
            for (index = 0U; index < 4U; index++)
            {
                CONSOLE_IN("%d", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = 4U;

            CONSOLE_OUT("Enter the Write Type: [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;

            retval = GA_mcp_ce_write_track_pos(appl_mcp_handle_in_use, &req);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 61:
            CONSOLE_OUT("Enter Playback Speed[8-bit Integer]\n");
            for (index = 0U; index < 1U; index++)
            {
                CONSOLE_IN("%d", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = 1U;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;

            retval = GA_mcp_ce_write_playback_speed
                     (
                         appl_mcp_handle_in_use,
                         &req
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 62:
            CONSOLE_OUT("Enter Obj ID[48-bit Unsigned Integer] in Hex:\n");
            for (index = 0U; index < OBJ_ID_LEN; index++)
            {
                CONSOLE_IN("%x", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = OBJ_ID_LEN;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;
            retval = GA_mcp_ce_write_curr_track_obj_ID
                     (
                         appl_mcp_handle_in_use,
                         &req
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 63:
            CONSOLE_OUT("Enter Obj ID[48-bit Unsigned Integer] in Hex:\n");
            for (index = 0U; index < OBJ_ID_LEN; index++)
            {
                CONSOLE_IN("%x", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = OBJ_ID_LEN;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;
            retval = GA_mcp_ce_write_next_track_obj_ID
                     (
                         appl_mcp_handle_in_use,
                         &req
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 64:
            CONSOLE_OUT("Enter Obj ID[48-bit Unsigned Integer] in Hex:\n");
            for (index = 0U; index < OBJ_ID_LEN; index++)
            {
                CONSOLE_IN("%x", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = OBJ_ID_LEN;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;
            retval = GA_mcp_ce_write_curr_group_obj_ID
                     (
                         appl_mcp_handle_in_use,
                         &req
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 65:
            CONSOLE_OUT("Enter Playing Order\n");
            for (index = 0U; index < 1U; index++)
            {
                CONSOLE_IN("%d", &input);
                write_req_val[index] = (UINT8)input;
            }
            req.req_data.val = &write_req_val[0U];
            req.req_data.len = 1U;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;

            retval = GA_mcp_ce_write_playing_order
                     (
                         appl_mcp_handle_in_use,
                         &req
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 66:
            CONSOLE_OUT("Enter MCP Command Opcode in Hex:\n");
            CONSOLE_OUT("[Play: 1, Pause: 2, Fast Rewind: 3, Fast Forward: 4, Stop: 5\n");
            CONSOLE_OUT("Move Relative: 0x10, Prev Seg: 0x20, Next Seg: 0x21, First Seg: 0x22\n");
            CONSOLE_OUT("Last Seg: 0x23, Goto Seg: 0x24, Prev Track: 0x30, Next Track: 0x31\n");
            CONSOLE_OUT("First Track:0x32, Last Track:0x33, Goto Track:0x34, Prev Group: 0x40\n");
            CONSOLE_OUT("Next Group: 0x41, First Group: 0x42, Last Group: 0x43, Goto Group: 0x44]\n");

            for (index = 0U; index < 1U; index++)
            {
                CONSOLE_IN("%x", &input);
                write_req_val[index] = (UINT8)input;
            }

            if ((0x10U == write_req_val[0U]) || (0x24U == write_req_val[0U]) ||
                (0x34U == write_req_val[0U]) || (0x44U == write_req_val[0U]))
            {
                CONSOLE_OUT
                (
                    "Enter the Offset/Track(n)/Segment(n): 32bit signed integer in Hex\n"
                );
                for (index = 1U; index < 5U; index++)
                {
                    CONSOLE_IN("%x", &input);
                    write_req_val[index] = (UINT8)input;
                }

            }

            req.req_data.val = &write_req_val[0U];
            req.req_data.len = index;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;

            retval = GA_mcp_ce_write_MCP(appl_mcp_handle_in_use, &req);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 67:

            CONSOLE_OUT("Enter SCP Type:\n");
            CONSOLE_OUT("[Track Name: 1, Artist Name: 2, Album Name: 3\n");
            CONSOLE_OUT("Group Name: 4, Earliest Year: 5, Latest Year: 6\n");
            CONSOLE_OUT("Genre: 7, Only Tracks: 8, Only Groups: 9]\n");
            CONSOLE_IN("%d", &input);
            write_req_val[1U] = (UINT8)input;

            /* SCP Type : Only Tracks or Only Groups */
            if ((write_req_val[1U] == 0x08U) || (write_req_val[1U] == 0x09U))
            {
                /* First Byte is Length, No Parameters */
                write_req_val[0U] = 0x01U;
            }
            else
            {
                CONSOLE_OUT("Enter the SCP Parameter Length\n");
                CONSOLE_IN("%d", &input);
                len = (UINT8)input;

                CONSOLE_OUT("Enter SCP Data[Hex]\n");
                for (index = 2U; index < (len + 2U); index++)
                {
                    CONSOLE_IN("%x", &input);
                    write_req_val[index] = (UINT8)input;
                }

                /* First Byte is Length - 1B, Type: 1B + Len of Data : len Bytes */
                write_req_val[0U] = len;
                write_req_val[0U] = write_req_val[0U] + 1U;
            }

            req.req_data.val = &write_req_val[0U];
            req.req_data.len = write_req_val[0U] + 1U;

            CONSOLE_OUT("Enter the Write Type:  [Write Req: 0], [WWR: 1]\n");
            CONSOLE_IN("%d", &input);
            req.write_type = (UINT8)input;

            retval = GA_mcp_ce_write_SCP(appl_mcp_handle_in_use, &req);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 80:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            appl_mcp_gmcs_ctx_handle = 0x00U;
            MCP_CE_SET_HANDLE_GMCS(appl_mcp_gmcs_ctx_handle);

            APPL_TRC("GA_mcp_ce_get_context_info ...\n");
            retval = GA_mcp_ce_get_context_info
                     (
                         &appl_ga_mcc_device,
                         &appl_mcp_gmcs_ctx_handle,
                         &appl_mcp_gmcs_range_ptr,
                         &appl_mcp_gmcs_char_info_ptr,
                         &appl_mcp_gmcs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );

            if (GA_SUCCESS == retval)
            {
                APPL_TRC("appl_mcp_gmcs_char_count:0x%02x\n", appl_mcp_gmcs_char_count);

                appl_mcp_gmcs_range = *appl_mcp_gmcs_range_ptr;

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_mcp_gmcs_char_info = GA_alloc_mem(appl_mcp_gmcs_char_count * sizeof(GA_CHAR_INFO));

                if (NULL != appl_mcp_gmcs_char_info)
                {
                    GA_mem_copy
                    (
                        appl_mcp_gmcs_char_info,
                        appl_mcp_gmcs_char_info_ptr,
                        appl_mcp_gmcs_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 81:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            appl_mcp_gmcs_handle_access = 0x00U;

            MCP_CE_SET_HANDLE_GMCS(appl_mcp_gmcs_handle_access);
            appl_mcp_gmcs_range_ptr = &appl_mcp_gmcs_range;
            appl_mcp_gmcs_char_info_ptr = appl_mcp_gmcs_char_info;

            if (NULL == appl_mcp_gmcs_char_info)
            {
                APPL_ERR("appl_mcp_gmcs_char_info is NULL !\n");
                APPL_TRC("Call GA_mcp_ce_get_context_info\n");
                break;
            }
            APPL_TRC("GA_mcp_ce_set_context_info ...\n");
            retval = GA_mcp_ce_set_context_info
                     (
                         &appl_ga_mcc_device,
                         &appl_mcp_gmcs_handle_access,
                         &appl_mcp_gmcs_range_ptr,
                         &appl_mcp_gmcs_char_info_ptr,
                         &appl_mcp_gmcs_char_count
                     );

            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            APPL_TRC("Handle for context allocated - 0x%02X\n", appl_mcp_gmcs_handle_access);

            if (GA_SUCCESS == retval)
            {
                appl_mcp_gmcs_ctx_handle = appl_mcp_gmcs_handle_access;

                /* Free the allocated memory */
                if (NULL != appl_mcp_gmcs_char_info)
                {
                    GA_free_mem(appl_mcp_gmcs_char_info);
                    appl_mcp_gmcs_char_info = NULL;
                }
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 82:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            CONSOLE_OUT("Enter the MCS Instance: ");
            CONSOLE_IN("%d", &choice);
            appl_mcp_mcs_ctx_handle = (UINT8)choice;
            if (MCP_MAX_MCS_CONTEXTS > appl_mcp_mcs_ctx_handle)
            {
                appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_handle_access =
                    appl_mcp_mcs_ctx_handle;
                APPL_TRC("GA_mcp_ce_get_context_info ...\n");
                retval = GA_mcp_ce_get_context_info
                         (
                             &appl_ga_mcc_device,
                             &appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_handle_access,
                             &appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_range_ptr,
                             &appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_info_ptr,
                             &appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_count
                         );
                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_MCS
                );

                if (GA_SUCCESS == retval)
                {
                    APPL_TRC
                    (
                        "appl_mcp_mcs_char_count:0x%02x\n",
                        appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_count
                    );

                    appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_range =
                        *appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_range_ptr;

                    /**
                     *  Reference to allocate to save locally.
                     *  This can be written to persistent and retrieved
                     *  upon the next connection if bonded device
                     */
                    appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_info =
                        GA_alloc_mem
                        (
                            appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_count * sizeof(GA_CHAR_INFO)
                        );

                    if (NULL != appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_info)
                    {
                        GA_mem_copy
                        (
                            appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_info,
                            appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_info_ptr,
                            appl_mcp_mcs_manage_ctx_info[appl_mcp_mcs_ctx_handle].appl_mcp_mcs_char_count * sizeof(GA_CHAR_INFO)
                        );
                    }
                }
            }
            else
            {
                APPL_ERR("Invalid MCS Instance !\n");
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 83:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            for (i = 0U; i < MCP_MAX_MCS_CONTEXTS; i++)
            {
                if (NULL == appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info)
                {
                    APPL_ERR("appl_mcp_mcs_char_info[%d] is NULL !\n", i);
                    APPL_TRC("Call GA_mcp_ce_get_context_info\n");
                }
                else
                {
                    appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_handle_access = 0U;
                    appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_range_ptr =
                        &appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_range;
                    appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info_ptr =
                        appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info;

                    APPL_TRC("GA_mcp_ce_set_context_info ...\n");
                    retval = GA_mcp_ce_set_context_info
                             (
                                 &appl_ga_mcc_device,
                                 &appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_handle_access,
                                 &appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_range_ptr,
                                 &appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info_ptr,
                                 &appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_count
                             );

                    APPL_TRC("Retval - ");
                    appl_mcp_utils_display_ga_result
                    (
                        retval,
                        GA_SERVICE_MCS
                    );
                    APPL_TRC
                    (
                        "Handle for context allocated - 0x%02X\n",
                        appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_handle_access
                    );

                    if (GA_SUCCESS == retval)
                    {
                        appl_mcp_mcs_ctx_handle =
                            appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_handle_access;

                        /* Free the allocated memory */
                        if (NULL != appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info)
                        {
                            GA_free_mem
                            (
                                appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info
                            );
                            appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info = NULL;
                        }
                    }
                }
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 87:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_mcp_ce_set_context_info() is NOT called
             * after GA_mcp_ce_get_context_info().
             */

             /* Free the allocated memory for GMCS char. info. */
            if (NULL != appl_mcp_gmcs_char_info)
            {
                APPL_TRC("Freeing appl_mcp_gmcs_char_info ...\n");
                GA_free_mem(appl_mcp_gmcs_char_info);
                appl_mcp_gmcs_char_info = NULL;
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 88:
#ifdef MCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_mcp_ce_set_context_info() is NOT called
             * after GA_mcp_ce_get_context_info().
             */

            for (i = 0U; i < MCP_MAX_MCS_CONTEXTS; i++)
            {
                /* Free the allocated memory for MCS char. info. */
                if (NULL != appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info)
                {
                    APPL_TRC("Freeing appl_mcp_mcs_char_info ...\n");
                    GA_free_mem
                    (
                        appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info
                    );
                    appl_mcp_mcs_manage_ctx_info[i].appl_mcp_mcs_char_info = NULL;
                }
            }
#else /* MCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
        {
#ifdef MCP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_mcp_ce_update_ntf_configuration...\n");
            retval = GA_mcp_ce_update_ntf_configuration
                     (
                        (MCP_CHAR_CONFIG(MCP_CHAR_ID_MEDIA_PLAYER_NAME) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_TRACK_CHANGED) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_TRACK_TITLE) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_TRACK_DURATION) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_TRACK_POS) |
                         /* MCP_CHAR_CONFIG(MCP_CHAR_ID_PLAYBACK_SPEED) | */
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_SEEKING_SPEED) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_CURR_TRACK_OBJ_ID) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_NEXT_TRACK_OBJ_ID) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_PARENT_GROUP_OBJ_ID) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_CURR_GROUP_OBJ_ID) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_PLAYING_ORDER) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_MEDIA_STATE) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_MEDIA_CONTROL_POINT) |
                         /* MCP_CHAR_CONFIG(MCP_CHAR_ID_CP_OPC_SUPP) | */
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_SEARCH_RES_OBJ_ID) |
                         MCP_CHAR_CONFIG(MCP_CHAR_ID_SEARCH_CONTROL_POINT))
                    );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
#else /* MCP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("MCP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* MCP_SUPPORT_CONFIG_SELECTION */
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

GA_RESULT appl_mcp_ce_read_sess_inst_for_op(void )
{
    int input = 0;

    /* Reset */
    appl_mcp_handle_in_use = 0U;

    CONSOLE_OUT("Session Instance Type: [MCS:0] [GMCS: 1]\n");
    CONSOLE_IN("%d", &input);
    if (0x01U == (UINT8)input)
    {
        appl_mcp_handle_in_use |= MCP_CE_GMCS_ENABLE;
    }

    CONSOLE_OUT("Session Instance ID: \n");
    CONSOLE_IN("%d", &input);
    appl_mcp_handle_in_use |= (UINT8)input;

    return GA_SUCCESS;
}

GA_RESULT appl_mcp_ce_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ MCP_CE_EVT_ID   evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ MCP_CE_EVT    * evt_data
          )
{
    GA_RESULT            retval;
    GA_BRR_SVC_INFO   * mcs_svc_info;
    UCHAR                index;
    UCHAR             * mcp_data;
    GA_MCP_CE_OBJ_ID  * object_id;
    MCP_PARAM_MCP_RSP * mcp_cp_ntf;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    GA_MCP_CE_MP_ICON_OBJ   * mcp_mp_icon_obj;
    GA_MCP_CE_TRACK_SEG_OBJ * mcp_track_segment_obj_id;
    GA_MCP_CE_TRACK_OBJ     * mcp_track_obj;
    GA_MCP_CE_GRP_OBJ       * mcp_grp_obj;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    /* Initialize */
    index = 0U;
    retval = GA_SUCCESS;

    APPL_TRC("[MCC_APPL] >> appl_mcp_ce_cb\n");

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("MCP Client");

    if (MCP_CE_IS_HANDLE_GMCS(evt_data->mcp_handle))
    {
        APPL_TRC("[Profile]     : MCP (0x%04X)\n", GA_SERVICE_GMCS);
    }
    else
    {
        APPL_TRC("[Profile]     : MCP (0x%04X)\n", GA_SERVICE_MCS);
    }
    APPL_TRC("[SECTION]     : MCP CLIENT EVENTS\n");

    switch (evt_id)
    {
    case MCP_CE_SETUP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC("[Event]       : MCP_CE_SETUP_CNF (0x%02X)\n", evt_id);
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;
    case MCP_CE_DISC_MCS_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        if (NULL != evt_data->data)
        {
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                ((GA_BRR_SVC_INFO*)evt_data->data)->uuid
            );
        }
        else
        {
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
        }
        APPL_TRC("[Event]       : MCP_CE_DISC_MCS_CNF (0x%02X)\n", evt_id);
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        /* MCS service discovery ongoing */
        if (GA_CONTINUE == evt_status)
        {
            mcs_svc_info = (GA_BRR_SVC_INFO*)evt_data->data;
            GA_FORMAT_DISPLAY();
            appl_ga_utils_display_svc_uuid(mcs_svc_info->uuid);
            GA_FORMAT_DISPLAY();
            appl_ga_utils_display_svc_range(mcs_svc_info->range);
        }
        else if (GA_FAILURE == evt_status)
        {
            APPL_ERR("MCS Service Discovery Result: Failed !\n");
        }
        else
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            if (0U == evt_data->len)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_CE_CONTROL_CHANNEL_SETUP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CONTROL_CHANNEL_SETUP_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_DATA_CHANNEL_UP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_DATA_CHANNEL_UP_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_DATA_CHANNEL_DOWN_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_DATA_CHANNEL_DOWN_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
    case MCP_CE_RELEASE_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_RELEASE_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;
    case MCP_CE_RELEASE_MCS_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_RELEASE_MCS_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;
    case MCP_CE_ENABLE_ALL_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_ENABLE_ALL_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_DISABLE_ALL_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_DISABLE_ALL_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Name (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_NAME
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_CHANGED_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_CHANGED_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Changed (0x%04X)\n",
            GA_CHAR_MCS_TRACK_CHANGED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_TITLE_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_TITLE_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Title (0x%04X)\n",
            GA_CHAR_MCS_TRACK_TITLE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_DUR_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_DUR_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Duration (0x%04X)\n",
            GA_CHAR_MCS_TRACK_DURATION
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_POS_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_POS_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Position (0x%04X)\n",
            GA_CHAR_MCS_TRACK_POS
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_PLAYBACK_SPEED_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_PLAYBACK_SPEED_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playback Speed (0x%04X)\n",
            GA_CHAR_MCS_PLAYBACK_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_SEEKING_SPEED_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SEEKING_SPEED_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Seeking Speed (0x%04X)\n",
            GA_CHAR_MCS_SEEKING_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Parent Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_PARENT_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_PLAYING_ORDER_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_PLAYING_ORDER_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Order (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDER
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MEDIA_STATE_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MEDIA_STATE_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media State (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_STATE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MCP_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MCP_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MCP_OPC_SUPP_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MCP_OPC_SUPP_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point Opcodes Supported (0x%04X)\n",
            GA_CHAR_MCS_CP_OPC_SUPP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_SCP_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SCP_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Control Point (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Results Object ID (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_RES_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MEDIA_PLAYER_NAME_NTF: /* Fall - through */
    case MCP_CE_READ_MEDIA_PLAYER_NAME_CNF:
        if (MCP_CE_READ_MEDIA_PLAYER_NAME_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_MEDIA_PLAYER_NAME_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_MEDIA_PLAYER_NAME_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Name (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_NAME
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_data = (UCHAR*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Media Player Name: ", evt_data->len);
            for (index = 0U; index < evt_data->len; index++)
            {
                APPL_TRC("%c", mcp_data[index]);
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_TRACK_CHANGED_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_CHANGED_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Changed (0x%04X)\n",
            GA_CHAR_MCS_TRACK_CHANGED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_TITLE_NTF: /* Fall - through */
    case MCP_CE_READ_TRACK_TITLE_CNF:
        if (MCP_CE_READ_TRACK_TITLE_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_TRACK_TITLE_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_TRACK_TITLE_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Title (0x%04X)\n",
            GA_CHAR_MCS_TRACK_TITLE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (MCP_DEFAULT_UTF8_STR_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Data: No current track or Track title unavailable (NULL)\n"
            );
        }
        else
        {
            mcp_data = (UCHAR*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Track Title: ", evt_data->len);
            for (index = 0U; index < evt_data->len; index++)
            {
                APPL_TRC("%c", mcp_data[index]);
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_TRACK_DUR_NTF: /* Fall - through */
    case MCP_CE_READ_TRACK_DUR_CNF:
        if (MCP_CE_READ_TRACK_DUR_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_TRACK_DUR_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_TRACK_DUR_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Duration (0x%04X)\n",
            GA_CHAR_MCS_TRACK_DURATION
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            if (MCP_DEFAULT_TRACK_VALUE == *((INT32*)evt_data->data))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Duration: Media player has no current track or the duration of the current track is unknown (0x%08X)\n",
                    (UCHAR)sizeof(INT32),
                    *((INT32*)evt_data->data)
                );
            }
            else
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Duration: 0x%08X\n",
                    (UCHAR)sizeof(INT32),
                    *((INT32*)evt_data->data)
                );
            }
        }
        break;

    case MCP_CE_TRACK_POS_NTF: /* Fall - through */
    case MCP_CE_READ_TRACK_POS_CNF:
        if (MCP_CE_READ_TRACK_POS_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_TRACK_POS_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_TRACK_POS_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Position (0x%04X)\n",
            GA_CHAR_MCS_TRACK_POS
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            if (MCP_DEFAULT_TRACK_VALUE == *((INT32*)evt_data->data))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Position: Unavailable (0x%08X)\n",
                    (UCHAR)sizeof(INT32),
                    *((INT32*)evt_data->data)
                );
            }
            else
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Position: 0x%08X\n",
                    (UCHAR)sizeof(INT32),
                    *((INT32*)evt_data->data)
                );
            }
        }
        break;

    case MCP_CE_PLAYBACK_SPEED_NTF: /* Fall - through */
    case MCP_CE_READ_PLAYBACK_SPEED_CNF:
        if (MCP_CE_READ_PLAYBACK_SPEED_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_PLAYBACK_SPEED_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_PLAYBACK_SPEED_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playback Speed (0x%04X)\n",
            GA_CHAR_MCS_PLAYBACK_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Playback Speed: 0x%02X\n",
                (UCHAR)sizeof(INT8),
                *((INT8*)evt_data->data)
            );
        }
        break;

    case MCP_CE_SEEKING_SPEED_NTF: /* Fall - through */
    case MCP_CE_READ_SEEKING_SPEED_CNF:
        if (MCP_CE_READ_SEEKING_SPEED_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_SEEKING_SPEED_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_SEEKING_SPEED_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Seeking Speed (0x%04X)\n",
            GA_CHAR_MCS_SEEKING_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            if (MCP_DEFAULT_SEEKING_SPEED == *((INT8*)evt_data->data))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Seeking Speed: Media player is not seeking (0x%02X)\n",
                    (UCHAR)sizeof(INT8),
                    *((INT8*)evt_data->data)
                );
            }
            else
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Seeking Speed: 0x%02X\n",
                    (UCHAR)sizeof(INT8),
                    *((INT8*)evt_data->data)
                );
            }
        }
        break;

    case MCP_CE_CURR_TRACK_OBJ_ID_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_TRACK_OBJ_ID_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Current Track Object ID: Media player has no current track (NULL)\n",
                evt_data->len
            );
        }
        else
        {
            object_id = (GA_MCP_CE_OBJ_ID*)evt_data->data;
            APPL_TRC("Len: 0x%02X,  Current Track Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    object_id->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_NEXT_TRACK_OBJ_ID_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_NEXT_TRACK_OBJ_ID_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Next Track Object ID: Media player has no next track (NULL)\n",
                evt_data->len
            );
        }
        else
        {
            object_id = (GA_MCP_CE_OBJ_ID*)evt_data->data;
            APPL_TRC("Len: 0x%02X,  Next Track Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    object_id->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_PARENT_GROUP_OBJ_ID_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_PARENT_GROUP_OBJ_ID_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Parent Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_PARENT_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Parent Group Object ID: Media player has not created a parent group or a current group (NULL)\n",
                evt_data->len
            );
        }
        else
        {
            object_id = (GA_MCP_CE_OBJ_ID*)evt_data->data;
            APPL_TRC("Len: 0x%02X,  Parent Group Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    object_id->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_CURR_GROUP_OBJ_ID_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_GROUP_OBJ_ID_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Current Group Object ID: Media player has not created a current group (NULL)\n",
                evt_data->len
            );
        }
        else
        {
            object_id = (GA_MCP_CE_OBJ_ID*)evt_data->data;
            APPL_TRC("Len: 0x%02X,  Current Group Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    object_id->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_PLAYING_ORDER_NTF: /* Fall - through */
    case MCP_CE_READ_PLAYING_ORDER_CNF:
        if (MCP_CE_READ_PLAYING_ORDER_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_PLAYING_ORDER_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_PLAYING_ORDER_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Order (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDER
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Playing Order: ", evt_data->len);
        appl_mcp_utils_parse_and_display_playing_order(*((UINT8*)evt_data->data));
        break;

    case MCP_CE_MEDIA_STATE_NTF: /* Fall - through */
    case MCP_CE_READ_MEDIA_STATE_CNF:
        if (MCP_CE_READ_MEDIA_STATE_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_MEDIA_STATE_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_MEDIA_STATE_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media State (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_STATE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Media State: ", evt_data->len);
        appl_mcp_utils_parse_and_display_media_state(*((UINT8*)evt_data->data));
        break;

    case MCP_CE_MCP_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MCP_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_cp_ntf = (MCP_PARAM_MCP_RSP*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Requested Opcode: ",
                (UCHAR)sizeof(MCP_CP_REQ_OPC)
            );
            appl_mcp_ce_utils_parse_and_display_mcp_opc(mcp_cp_ntf->req_opc);
            APPL_TRC
            (
                "Len: 0x%02X,  Result Code: ",
                (UCHAR)sizeof(MCP_CP_RES_CODE)
            );
            appl_mcp_ce_utils_parse_and_display_mcp_res_code(mcp_cp_ntf->result);
        }
        break;

    case MCP_CE_MCP_OPC_SUPP_NTF: /* Fall - through */
    case MCP_CE_READ_MCP_OPC_SUPP_CNF:
        if (MCP_CE_READ_MCP_OPC_SUPP_CNF == evt_id)
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_READ_MCP_OPC_SUPP_CNF (0x%02X)\n",
                evt_id
            );
        }
        else
        {
            APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_mcp_utils_display_ga_result
            (
                evt_status,
                GA_SERVICE_GMCS
            );
            APPL_TRC
            (
                "[Event]       : MCP_CE_MCP_OPC_SUPP_NTF (0x%02X)\n",
                evt_id
            );
        }
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point Opcodes Supported (0x%04X)\n",
            GA_CHAR_MCS_CP_OPC_SUPP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Media Control Point Opcodes Supported: 0x%08X\n",
            evt_data->len,
            *((UINT32*)evt_data->data)
        );
        appl_mcp_utils_parse_and_display_mcp_opc_supp(*((UINT32*)evt_data->data));
        break;

    case MCP_CE_SCP_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SCP_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Control Point (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Result Code: \n", evt_data->len);
            if (MCP_SCP_RES_CODE_SUCCESS == *((UINT8*)evt_data->data))
            {
                APPL_TRC
                (
                    "Search request was accepted; search has started (0x%02X)\n",
                    MCP_SCP_RES_CODE_SUCCESS
                );
            }
            else if (MCP_SCP_RES_CODE_FAILURE == *((UINT8*)evt_data->data))
            {
                APPL_TRC
                (
                    "Search request was invalid; no search started (0x%02X)\n",
                    MCP_SCP_RES_CODE_FAILURE
                );
            }
        }
        break;

    case MCP_CE_SEARCH_RES_OBJ_ID_NTF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SEARCH_RES_OBJ_ID_NTF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Results Object ID (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_RES_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == evt_data->len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Search Results Object ID: There are no search results (NULL)\n",
                evt_data->len
            );
        }
        else
        {
            object_id = (GA_MCP_CE_OBJ_ID*)evt_data->data;
            APPL_TRC("Len: 0x%02X,  Search Results Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    object_id->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_CE_READ_MEDIA_PLAYER_ICON_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_MEDIA_PLAYER_ICON_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Icon Object ID (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_ICON_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_mp_icon_obj = (GA_MCP_CE_MP_ICON_OBJ*)evt_data->data;
            mcp_data = (UCHAR*)mcp_mp_icon_obj->icon_png_data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object: ",
                mcp_mp_icon_obj->total_obj_len
            );
            for (index = 0U; index < mcp_mp_icon_obj->total_obj_len; index++)
            {
                APPL_TRC("%c", mcp_data[index]);
            }
            APPL_TRC("\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Length: 0x%04X\n",
                (UCHAR)sizeof(UINT16),
                mcp_mp_icon_obj->total_obj_len
            );
        }
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    case MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Icon URL (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_ICON_URL
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        mcp_data = (UCHAR*)evt_data->data;
        APPL_TRC("Len: 0x%02X,  Media Player Icon URL: ", evt_data->len);
        for (index = 0U; index < evt_data->len; index++)
        {
            APPL_TRC("%c", mcp_data[index]);
        }
        APPL_TRC("\n");
        break;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Segments Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_SEG_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_track_segment_obj_id = (GA_MCP_CE_TRACK_SEG_OBJ*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Segment Name Length: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                mcp_track_segment_obj_id->seg_name_len
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Segment Name: ",
                mcp_track_segment_obj_id->seg_name_len
            );
            for (index = 0U; index < mcp_track_segment_obj_id->seg_name_len; index++)
            {
                APPL_TRC("%c", mcp_track_segment_obj_id->seg_name[index]);
            }
            APPL_TRC("\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Segment Position: 0x%08X\n",
                (UCHAR)sizeof(INT32),
                mcp_track_segment_obj_id->seg_pos
            );
        }
        break;

    case MCP_CE_READ_CURR_TRACK_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_CURR_TRACK_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_track_obj = (GA_MCP_CE_TRACK_OBJ*)evt_data->data;
            mcp_data = (UCHAR*)mcp_track_obj->IDV32_data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object: ",
                mcp_track_obj->total_obj_len
            );
            for (index = 0U; index < mcp_track_obj->total_obj_len; index++)
            {
                APPL_TRC("%c", mcp_data[index]);
            }
            APPL_TRC("\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Length: 0x%04X\n",
                (UCHAR)sizeof(UINT16),
                mcp_track_obj->total_obj_len
            );
        }
        break;

    case MCP_CE_READ_NEXT_TRACK_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_NEXT_TRACK_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_track_obj = (GA_MCP_CE_TRACK_OBJ*)evt_data->data;
            mcp_data = (UCHAR*)mcp_track_obj->IDV32_data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object: ",
                mcp_track_obj->total_obj_len
            );
            for (index = 0U; index < mcp_track_obj->total_obj_len; index++)
            {
                APPL_TRC("%c", mcp_data[index]);
            }
            APPL_TRC("\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Length: 0x%04X\n",
                (UCHAR)sizeof(UINT16),
                mcp_track_obj->total_obj_len
            );
        }
        break;

    case MCP_CE_READ_PARENT_GROUP_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_PARENT_GROUP_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Parent Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_PARENT_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_grp_obj = (GA_MCP_CE_GRP_OBJ*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Type: ",
                (UCHAR)sizeof(UINT8)
            );
            if (MCP_SE_GRP_OBJ_TYPE_TRACK == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Track (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            else if (MCP_SE_GRP_OBJ_TYPE_GROUP == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Group (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", MCP_OBJ_ID_LEN);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    mcp_grp_obj->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_READ_CURR_GROUP_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_CURR_GROUP_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_grp_obj = (GA_MCP_CE_GRP_OBJ*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Type: ",
                (UCHAR)sizeof(UINT8)
            );
            if (MCP_SE_GRP_OBJ_TYPE_TRACK == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Track (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            else if (MCP_SE_GRP_OBJ_TYPE_GROUP == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Group (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", MCP_OBJ_ID_LEN);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    mcp_grp_obj->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    case MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Orders Supported (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDERS_SUPP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Playing Orders Supported: 0x%04X\n",
                evt_data->len,
                *((UINT16*)evt_data->data)
            );
            appl_mcp_utils_parse_and_display_playing_orders_supp(*((UINT16*)evt_data->data));
        }
        break;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_CE_READ_SEARCH_RES_OBJ_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_SEARCH_RES_OBJ_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Results Object ID (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_RES_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            mcp_grp_obj = (GA_MCP_CE_GRP_OBJ*)evt_data->data;
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Object Type: ",
                (UCHAR)sizeof(UINT8)
            );
            if (MCP_SE_GRP_OBJ_TYPE_TRACK == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Track (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            else if (MCP_SE_GRP_OBJ_TYPE_GROUP == mcp_grp_obj->obj_type)
            {
                APPL_TRC("Group (0x%02X)\n", mcp_grp_obj->obj_type);
            }
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", MCP_OBJ_ID_LEN);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    mcp_grp_obj->obj[index]
                );
            }
            APPL_TRC("\n");
        }
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    case MCP_CE_READ_CONTENT_CONTROL_ID_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_READ_CONTENT_CONTROL_ID_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Content Control ID (0x%04X)\n",
            GA_CHAR_CONTENT_CONTROL_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_INF("Data: NULL\n");
        }
        else
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Content Control ID: 0x%02X\n",
                evt_data->len,
                *((UINT8*)evt_data->data)
            );
        }
        break;

    case MCP_CE_TRACK_POS_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_POS_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Position (0x%04X)\n",
            GA_CHAR_MCS_TRACK_POS
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_TRACK_PLAYBACK_SPEED_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_TRACK_PLAYBACK_SPEED_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playback Speed (0x%04X)\n",
            GA_CHAR_MCS_PLAYBACK_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_CURR_TRACK_OBJ_ID_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_TRACK_OBJ_ID_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_NEXT_TRACK_OBJ_ID_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_NEXT_TRACK_OBJ_ID_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_CURR_GRP_OBJ_ID_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_CURR_GRP_OBJ_ID_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_PLAYING_ORDER_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_PLAYING_ORDER_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Order (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDER
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_MCP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_MCP_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_SCP_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SCP_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Control Point (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_CE_SET_OBJ_REF_CNF:
        APPL_TRC("[SUB-SECTION] : MCP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_SET_OBJ_REF_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        if (GA_CONTINUE == evt_status)
        {
            GA_mem_copy(&obj_id.obj[0U], evt_data->data, (evt_data->len  * sizeof(UINT8)));
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", evt_data->len);
            for (index = 0U; index < evt_data->len; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    obj_id.obj[index]
                );
            }
            APPL_TRC("\n");
        }
        else if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            if (0U == evt_data->len)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        else
        {
            APPL_ERR("Current Object reference set failed !\n");
        }
        break;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_CE_GET_OBJ_ATTR_CNF:
    {
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_GET_OBJ_ATTR_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        GA_MCP_CE_OBJ_ATTR * attr = (GA_MCP_CE_OBJ_ATTR *)evt_data->data;

        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC
            (
                "Len: 0x%02X,  Attribute ID: ",
                (UCHAR)sizeof(MCP_CE_OBJ_ATTR_ID)
            );
            appl_mcp_ce_utils_parse_and_display_attr_id_and_value(attr);
        }
    }
        break;

    case MCP_CE_OBJ_DATA_RX_READY_CNF:
        APPL_TRC("[SUB-SECTION] : MCP - OTP CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_CE_OBJ_DATA_RX_READY_CNF (0x%02X)\n",
            evt_id
        );
        appl_mcp_ce_utils_parse_and_display_handle(evt_data->mcp_handle);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        if (0U == evt_data->len)
        {
            APPL_TRC("Data: NULL\n");
        }

        /* if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("Object contents read request success\n");
        }
        else
        {
            APPL_ERR("Object contents read request failed !\n");
        } */
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    default:
        APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
        break;
    }

    GA_CB_EXIT();

    APPL_TRC("\n[MCC_APPL] << appl_mcp_ce_cb\n\n");

    return retval;
}

void appl_find_empty_mcs_pos(UINT8 * index)
{
    UINT8 i;

    for (i = 0U; i < MCS_MAX_INST; i++)
    {
        if (MCS_INST_FREE == appl_mcp_mcs_handle[i])
        {
            *index = i;
            break;
        }
    }

    return;
}

void appl_mcp_ce_utils_parse_and_display_mcp_opc(UINT8 req_opc)
{
    switch(req_opc)
    {
    case MCP_CP_OPCODE_PLAY:
        APPL_TRC("Play (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_PAUSE:
        APPL_TRC("Pause (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_FAST_RWD:
        APPL_TRC("Fast Rewind (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_FAST_FWD:
        APPL_TRC("Fast Forward (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_STOP:
        APPL_TRC("Stop (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_MOVE_RELATIVE:
        APPL_TRC("Move Relative (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_PREV_SEG:
        APPL_TRC("Previous Segment (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_NEXT_SEG:
        APPL_TRC("Next Segment (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_FIRST_SEG:
        APPL_TRC("First Segment (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_LAST_SEG:
        APPL_TRC("Last Segment (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_GOTO_SEG:
        APPL_TRC("Goto Segment (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_PREV_TRACK:
        APPL_TRC("Previous Track (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_NEXT_TRACK:
        APPL_TRC("Next Track (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_FIRST_TRACK:
        APPL_TRC("First Track (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_LAST_TRACK:
        APPL_TRC("Last Track (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_GOTO_TRACK:
        APPL_TRC("Goto Track (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_PREV_GROUP:
        APPL_TRC("Previous Group (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_NEXT_GROUP:
        APPL_TRC("Next Group (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_FIRST_GROUP:
        APPL_TRC("First Group (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_LAST_GROUP:
        APPL_TRC("Last Group (0x%02X)\n", req_opc);
        break;

    case MCP_CP_OPCODE_GOTO_GROUP:
        APPL_TRC("Goto Group (0x%02X)\n", req_opc);
        break;

    default:
        APPL_ERR("UnHandled Opcode (0x%02X) !\n", req_opc);
        break;
    }

    return;
}

void appl_mcp_ce_utils_parse_and_display_mcp_res_code(UINT8 res_code)
{
    switch(res_code)
    {
    case MCP_CP_RES_CODE_SUCCESS:
        APPL_TRC("Success (0x%02X)\n", res_code);
        break;

    case MCP_CP_RES_CODE_OPCODE_NOT_SUPP:
        APPL_ERR("Opcode Not Supported (0x%02X) !\n", res_code);
        break;

    case MCP_CP_RES_CODE_MP_INACTIVE:
        APPL_ERR("Media Player Inactive (0x%02X) !\n", res_code);
        break;

    case MCP_CP_RES_CODE_CMD_CANNOT_BE_COMPLETED:
        APPL_ERR("Command Cannot Be Completed (0x%02X) !\n", res_code);
        break;

    default:
        APPL_ERR("UnHandled Result Code (0x%02X) !\n", res_code);
        break;
    }

    return;
}

void appl_mcp_ce_utils_parse_and_display_handle(MCP_HANDLE handle)
{
    if (MCP_CE_IS_HANDLE_GMCS(handle))
    {
        APPL_TRC
        (
            "[Service Cntx]: GMCS Instance: 0x%02x\n",
            (handle & 0x7F)
        );
    }
    else
    {
        APPL_TRC
        (
            "[Service Cntx]: MCS Instance: 0x%02x\n",
            handle
        );
    }
}

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
void appl_mcp_ce_utils_parse_and_display_attr_id_and_value(GA_MCP_CE_OBJ_ATTR * attr)
{
    UCHAR   index;
    UINT32  oacp_feature;
    UINT32  olcp_feature;
    UINT16  data1;

    /* Initialize */
    oacp_feature = 0U;
    olcp_feature = 0U;
    data1 = 0U;

    switch(attr->attr_id)
    {
    case MCP_CE_OBJ_ATTR_ID_FEATURE_NAME:
        APPL_TRC("Feature Name (0x%02X)\n", attr->attr_id);
        GA_UNPACK_LE_4_BYTE(&oacp_feature, &attr->data[0U]);
        APPL_TRC
        (
            "Len: 0x%02X,  OACP Features: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            oacp_feature
        );
        appl_mcp_par_and_display_oacp_feature(oacp_feature);
        GA_UNPACK_LE_4_BYTE(&olcp_feature, &attr->data[4U]);
        APPL_TRC
        (
            "Len: 0x%02X,  OLCP Features: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            olcp_feature
        );
        appl_mcp_par_and_display_olcp_feature(olcp_feature);
        break;

    case MCP_CE_OBJ_ATTR_ID_NAME:
        APPL_TRC("Name (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            APPL_TRC("Len: 0x%02X,  Object Name: ", attr->len);
            for (index = 0U; index < attr->len; index++)
            {
                APPL_TRC("%c", attr->data[index]);
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_TYPE:
        APPL_TRC("Type (0x%02X)\n", attr->attr_id);
        if ((NULL != attr->data) && (2U == attr->len))
        {
            APPL_TRC("Len: 0x%02X,  Object Type: ", attr->len);
            appl_mcp_utils_parse_and_display_object_type(attr->data);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_SIZE:
        APPL_TRC("Size (0x%02X)\n", attr->attr_id);
        curr_size = 0U;
        alloc_size = 0U;
        GA_UNPACK_LE_4_BYTE(&curr_size, &attr->data[0U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Current Size: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            curr_size
        );
        GA_UNPACK_LE_4_BYTE(&alloc_size, &attr->data[4U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Allocated Size: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            alloc_size
        );
        break;

    case MCP_CE_OBJ_ATTR_ID_FIRST_CREATED:
        APPL_TRC("First Created (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            APPL_TRC("Len: 0x%02X,  Object First-Created:\n", attr->len);
            GA_UNPACK_LE_2_BYTE(&data1, &attr->data[0U]);
            APPL_TRC("\t-Year: %d (0x%04X)\n", data1, data1);
            APPL_TRC("\t-Month: %d (0x%02X)\n", attr->data[2U], attr->data[2U]);
            APPL_TRC("\t-Day: %d (0x%02X)\n", attr->data[3U], attr->data[3U]);
            APPL_TRC("\t-Hours: %d (0x%02X)\n", attr->data[4U], attr->data[4U]);
            APPL_TRC("\t-Minutes: %d (0x%02X)\n", attr->data[5U], attr->data[5U]);
            APPL_TRC("\t-Seconds: %d (0x%02X)\n", attr->data[6U], attr->data[6U]);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_LAST_MOD:
        APPL_TRC("Last Modified (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            APPL_TRC("Len: 0x%02X,  Object Last-Modified:\n", attr->len);
            GA_UNPACK_LE_2_BYTE(&data1, &attr->data[0U]);
            APPL_TRC("\t-Year: %d (0x%04X)\n", data1, data1);
            APPL_TRC("\t-Month: %d (0x%02X)\n", attr->data[2U], attr->data[2U]);
            APPL_TRC("\t-Day: %d (0x%02X)\n", attr->data[3U], attr->data[3U]);
            APPL_TRC("\t-Hours: %d (0x%02X)\n", attr->data[4U], attr->data[4U]);
            APPL_TRC("\t-Minutes: %d (0x%02X)\n", attr->data[5U], attr->data[5U]);
            APPL_TRC("\t-Seconds: %d (0x%02X)\n", attr->data[6U], attr->data[6U]);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_OBJ_ID:
        APPL_TRC("Object ID (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", attr->len);
            for (index = 0U; index < attr->len; index++)
            {
                APPL_TRC("%02X", attr->data[index]);
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_PROP:
        APPL_TRC("Property (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            appl_mcp_utils_parse_and_display_obj_prop(attr->data, attr->len);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_OACP:
        APPL_TRC("OACP (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            appl_mcp_utils_parse_and_display_oacp(attr->data);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_OLCP:
        APPL_TRC("OLCP (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            appl_mcp_utils_parse_and_display_olcp(attr->data, attr->len);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_LIST_FILTER:
        APPL_TRC("List Filter (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            appl_mcp_utils_parse_and_display_list_filter(attr->data, attr->len);
        }
        break;

    case MCP_CE_OBJ_ATTR_ID_CHGD:
        APPL_TRC("Changed (0x%02X)\n", attr->attr_id);
        if (NULL != attr->data)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Flags: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                attr->data[0U]
            );
            appl_mcp_utils_parse_and_display_obj_chgd_flag_bit(attr->data[0U]);
            APPL_TRC("Len: 0x%02X,  Object ID: 0x", (attr->len - 1U));
            for (index = 1U; index < attr->len; index++)
            {
                APPL_TRC("%02X", attr->data[index]);
            }
            APPL_TRC("\n");
        }
        break;

    default:
        APPL_ERR("UnHandled Attribute ID (0x%02X) !\n", attr->attr_id);
        break;
    }

    return;
}
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

#endif /* MCP_CLIENT */
