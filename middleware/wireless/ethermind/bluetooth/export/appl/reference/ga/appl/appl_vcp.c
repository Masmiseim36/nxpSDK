/**
 *  \file appl_vcp.c
 *
 *  \brief This file defines the Volume Control Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enables a device to control the audio volume
 *  of a device that exposes the Volume Control Service and to obtain the
 *  volume status of that device.
 *  This application also allows to perform the procedures used to control
 *  and obtain the status of the Volume Offset Control Service (VOCS)
 *  and Audio Input Control Service (AICS) if supported.
 *  Roles: VCP Controller: Device that controls the volume and related state
 *                         of this audio.
 *         VCP Renderer: Device that receives one or more audio inputs and
 *         exhibits control over an audio output device.
 *  GATT Requirement: VCP Controller: GATT Client
 *                    VCP Renderer: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_vcp.h"

#ifdef GA_VCP

/* --------------------------------------------- Global Definitions */
/* VCP Controller Menu Operations */
static const char appl_vcp_ct_options[] = " \n\
================ GA VCP Controller MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Context Management ~ Setup ============ \n\
    2. VCP Setup \n\
    3. VCP Get Capabilities \n\
    4. VCP Setup VOCS Capability \n\
    5. VCP Setup AICS Capability \n\
\n\
================ Context Management ~ Release ============ \n\
    6. VCP Release/Close VCS \n\
    7. VCP Release/Close VOCS \n\
    8. VCP Release/Close AICS \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
    9. Set VOCS/AICS Handle \n\
\n\
================ Profile Management (VCS) ~ Operations(Read) ============ \n\
   10. VCS Get Volume State \n\
   11. VCS Get Volume Flags \n\
\n\
================ Profile Management (VCS) ~ Operations(Write) ============ \n\
   12. VCS Send Relative Volume Down \n\
   13. VCS Send Relative Volume Up \n\
   14. VCS Send Unmute & Relative Volume Down \n\
   15. VCS Send Unmute & Relative Volume Up \n\
   16. VCS Send Set Absolute Volume \n\
   17. VCS Send Unmute \n\
   18. VCS Send Mute \n\
\n\
================ Profile Management (VOCS) ~ Operations(Read) ============ \n\
   20. VOCS Get Volume Offset \n\
   21. VOCS Get Audio Location \n\
   22. VOCS Get Output Description \n\
\n\
================ Profile Management (VOCS) ~ Operations(Write) ============ \n\
   23. VOCS Set Volume Offset \n\
   24. VOCS Set Audio Location \n\
   25. VOCS Set Output Description \n\
\n\
================ Profile Management (AICS) ~ Operations(Read) ============ \n\
   30. AICS Get Audio Input State \n\
   31. AICS Get Gain Setting Properties \n\
   32. AICS Get Audio Input Type \n\
   33. AICS Get Audio Input Status \n\
   34. AICS Get Audio Input Description \n\
\n\
================ Profile Management (AICS) ~ Operations(Write) ============ \n\
   35. AICS Set Audio Input Description \n\
\n\
   36. AICS Set Gain Setting \n\
   37. AICS Unmute \n\
   38. AICS Mute \n\
   39. AICS Set Manual Gain Mode \n\
   40. AICS Set Automatic Gain Mode \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get VCS Context Info. \n\
   81. Set VCS Context Info. \n\
\n\
   82. Get VOCS Context Info. \n\
   83. Set VOCS Context Info. \n\
\n\
   84. Get AICS Context Info. \n\
   85. Set AICS Context Info. \n\
\n\
   87. Free Saved VCS Context Info. \n\
   88. Free Saved VOCS Context Info. \n\
   89. Free Saved AICS Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure VCS Notification \n\
   91. Configure VOCS Notification \n\
   92. Configure AICS Notification \n\
\n\
Your Option ?: ";

/* VCP Renderer Menu Operations */
static const char appl_vcp_rd_options[] = " \n\
================ GA VCP Renderer MENU ================ \n\
   0. Exit \n\
   1. Refresh this Menu \n\
\n\
================ Device Management ~ Setting ============ \n\
   7. Register Peer Device Address \n\
\n\
================ Service Management ~ Included Service ============ \n\
   8. Register VOSC/AICS Service \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
   9. Set VOCS/AICS Handle \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
  50. VCS Notify Volume State \n\
  51. VCS Notify Volume Flag \n\
\n\
  52. VOCS Notify Volume Offset State\n\
  53. VOCS Notify Audio Location \n\
  54. VOCS Notify Audio Output Description \n\
\n\
  55. AICS Notify Audio Input State \n\
  56. AICS Notify Audio_Input Status \n\
  57. AICS Notify Audio Input Description \n\
\n\
  58. AICS Update Gain Mode \n\
  59. AICS Update Mute Settings \n\
  60. AICS Update Audio Input Description \n\
  61. AICS Set Gain Settings \n\
\n\
  62. VCS Update Volume Setting \n\
  63. VCS Update Mute Setting \n\
  64. VCS Update Volume Flag \n\
  65. VCS Update Change Counter \n\
\n\
  66. VOCS Update Audio Location \n\
  67. VOCS Update Output Description \n\
\n\
================ Service Management ~ SDP Record ============ \n\
  70. Activate VCS SDP record \n\
  71. InActivate VCS SDP record \n\
\n\
  72. Activate VOCS SDP record \n\
  73. InActivate VOCS SDP record \n\
\n\
  74. Activate AICS SDP record \n\
  75. InActivate AICS SDP record \n\
\n\
Your Option ?: ";

#ifdef VCP_CONTROLLER

/* Peer Device to connect VCP Profile to */
DECL_STATIC GA_ENDPOINT  appl_vcp_peer_rd_dev;
DECL_STATIC VCP_HANDLE   appl_vcp_ct_hndl;
DECL_STATIC GA_CONTEXT   appl_vcp_ct_vocs_ctx;
DECL_STATIC GA_CONTEXT   appl_vcp_ct_aics_ctx;

/* Controller Volume state Info. */
DECL_STATIC APPL_VCP_VCS_CHAR_VOL_STATE_PARAMS  appl_vcp_cntrl_vcs_vol_state;
DECL_STATIC APPL_VCP_VCS_CHAR_INFO              appl_vcp_cntrl_vcs_vol_val;

DECL_STATIC APPL_VCP_VOCS_CHAR_INFO             appl_vc_vocs_char_info[VCP_MAX_VOCS_CONTEXTS];
DECL_STATIC APPL_VCP_VOCS_CHAR_OFFSET_STATE_PARAMS
            appl_vc_vocs_offset_state_info[VCP_MAX_VOCS_CONTEXTS];

DECL_STATIC APPL_AICS_CHAR_INFO  appl_aics_chr_info[VCP_MAX_AICS_CONTEXTS];
DECL_STATIC APPL_AICS_CHAR_INPUT_STATE_PARAMS
            appl_aics_input_state[VCP_MAX_AICS_CONTEXTS];
DECL_STATIC APPL_AICS_CHAR_GAIN_SETTING_PARAMS
            appl_aics_input_gain_setting[VCP_MAX_AICS_CONTEXTS];

#ifdef VCP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_vcp_vcs_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_vcs_char_info_ptr;
DECL_STATIC  UINT16              appl_vcp_vcs_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_vcp_vcs_range;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_vcs_char_info;
DECL_STATIC  VCP_HANDLE          appl_vcp_vcs_handle_access;

DECL_STATIC  GA_BRR_SVC_RANGE  * appl_vcp_vocs_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_vocs_char_info_ptr;
DECL_STATIC  UINT16              appl_vcp_vocs_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_vcp_vocs_range;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_vocs_char_info;
DECL_STATIC  GA_CONTEXT          appl_vcp_vocs_handle_access;

DECL_STATIC  GA_BRR_SVC_RANGE  * appl_vcp_aics_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_aics_char_info_ptr;
DECL_STATIC  UINT16              appl_vcp_aics_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_vcp_aics_range;
DECL_STATIC  GA_CHAR_INFO      * appl_vcp_aics_char_info;
DECL_STATIC  GA_CONTEXT          appl_vcp_aics_handle_access;
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
#endif /* VCP_CONTROLLER */

#ifdef VCP_RENDERER

/* Renderer Volume state Info. */
DECL_STATIC APPL_VCP_VCS_CHAR_INFO              appl_vcp_rndr_vcs_vol_val;

DECL_STATIC UCHAR appl_ntf_volume_flag = 0U;

/* For sending notification */
DECL_STATIC VCP_EVENT_INFO  appl_vcr_ntf_info;
DECL_STATIC UCHAR           appl_vcr_ntf_data[64U];
DECL_STATIC UCHAR           appl_vcr_ntf_datalen;

/* VOCS Service Entities */
/* Audio Output */
DECL_STATIC UCHAR appl_audio_op_desptn[] = "Speaker_L";
DECL_STATIC UCHAR appl_audio_op_desptn1[] = "Speaker_R";
DECL_STATIC APPL_VCP_VOCS_CHAR_INFO
            appl_vc_rndr_vocs_char_info[VCP_MAX_VOCS_ENTITIES];
DECL_STATIC APPL_VCP_VOCS_CHAR_OFFSET_STATE_PARAMS
            appl_vc_rndr_vocs_offset_state_info[VCP_MAX_VOCS_ENTITIES];

/* Audio Input */
DECL_STATIC UCHAR appl_audio_in_desptn[] = "BLE_AUDIO";
DECL_STATIC APPL_AICS_CHAR_INFO  aics_chr_info[VCP_MAX_AICS_ENTITIES];
DECL_STATIC APPL_AICS_CHAR_INPUT_STATE_PARAMS
            aics_input_state[VCP_MAX_AICS_ENTITIES];
DECL_STATIC APPL_AICS_CHAR_GAIN_SETTING_PARAMS
            aics_input_gain_setting[VCP_MAX_AICS_ENTITIES];

/* VOCS Handle */
DECL_STATIC UCHAR  vcr_vocs_handle;
DECL_STATIC UCHAR  vcr_vocs_out_description_cnt;

/* AICS Handle */
DECL_STATIC UCHAR  vcr_aics_handle;
DECL_STATIC UCHAR  vcr_aics_out_description_cnt;

/* Timer Handle to send VCS Notifications */
DECL_STATIC BT_timer_handle  appl_vcs_cp_wt_timer_handle;

/* Timer Handle to send VOCS Notifications */
DECL_STATIC BT_timer_handle  appl_vocs_cp_wt_timer_handle;

/* Timer Handle to send VOCS Audio location Notification */
DECL_STATIC BT_timer_handle  appl_vocs_set_audio_loc_timer_handle;

/* Timer Handle to send VOCS Audio Output Description Notification */
DECL_STATIC BT_timer_handle  appl_vocs_set_audio_out_desc_timer_handle;

/* Timer Handle to send AICS Notifications */
DECL_STATIC BT_timer_handle  appl_aics_cp_wt_timer_handle;

/* Timer Handle to send AICS Audio Input Description Notification */
DECL_STATIC BT_timer_handle  appl_aics_set_audio_inp_desc_timer_handle;

DECL_STATIC UCHAR  appl_vcp_rndr_chr_data[64U];
DECL_STATIC UCHAR  appl_vcp_rndr_chr_datalen;

#endif /* VCP_RENDERER */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- APIs */

/* Parser for printing error codes */
void appl_vcp_display_ga_result(GA_RESULT status, UINT8 service);
/* Parser for printing VCS application error codes */
void appl_vcp_display_vcs_appl_err_codes(GA_RESULT status);
/* Parser for printing VOCS application error codes */
void appl_vcp_display_vocs_appl_err_codes(GA_RESULT status);

/*** APIs - VCP Controller ***/

#ifdef VCP_CONTROLLER
void appl_vcp_ct_set_default_values(void)
{
    UINT8 i;

    /* VCS */
    appl_vcp_cntrl_vcs_vol_val.vol_state = appl_vcp_cntrl_vcs_vol_state;

    /* VOCS */
    for(i = 0U; i < VCP_MAX_VOCS_CONTEXTS; i++)
    {
        appl_vc_vocs_char_info[i].offset_state =
            &appl_vc_vocs_offset_state_info[i];
    }

    /* AICS */
    for(i = 0U; i < VCP_MAX_AICS_CONTEXTS; i++)
    {
        appl_aics_chr_info[i].input_state = &appl_aics_input_state[i];
        appl_aics_chr_info[i].gain_setting = &appl_aics_input_gain_setting[i];
    }

    return;
}

int appl_vcp_ct_operations(void)
{
    GA_RESULT  retval;
    int        choice;
    int        menu_choice;
    int        input;
    UINT16     contexts;
    UINT8      conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(contexts);
    GA_IGNORE_UNUSED_PARAM(input);

    GA_LOOP_FOREVER()
    {
        APPL_TRC("\n");
        CONSOLE_OUT("%s", appl_vcp_ct_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 2:

#if 1
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &appl_vcp_peer_rd_dev
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(appl_vcp_peer_rd_dev.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                appl_vcp_peer_rd_dev.bd_type = (UINT8)choice;
            }
#else
            BT_COPY_BD_ADDR(appl_vcp_peer_rd_dev.bd_addr, g_pts_bd_addr);
            appl_vcp_peer_rd_dev.bd_type = BT_BD_PUBLIC_ADDRESS_TYPE;
#endif

            APPL_TRC("Setup VCP \n");
            retval = GA_vc_setup(&appl_vcp_peer_rd_dev, &appl_vcp_ct_hndl);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            APPL_TRC("VCP Context: %d\n", appl_vcp_ct_hndl);
            break;

        case 3:
            APPL_TRC("GA_vc_get_capabilities: \n");
            retval = GA_vc_get_capabilities(&appl_vcp_ct_hndl);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 4:
        {
            GA_BRR_SVC_INFO svc;

            svc.uuid = GA_SERVICE_VOCS;

           /* Temp */
#if 1
            CONSOLE_OUT("Enter start handle:\n");
            CONSOLE_IN("%x", &choice);
            svc.range.start = (UINT16)(choice);

            CONSOLE_OUT("Enter end handle:\n");
            CONSOLE_IN("%x", &choice);
            svc.range.end = (UINT16)(choice);
#else
#if 0
            svc.range.start = 0x31;
            svc.range.end = 0x3c;
#endif /* 0 */

            /* PTS */
            svc.range.start = 0xA0;
            svc.range.end = 0xAB;

            APPL_TRC("Start Handle: 0x%04x \n", svc.range.start);
            APPL_TRC("Start Handle: 0x%04x \n", svc.range.end);
#endif /* 0 */

            APPL_TRC("GA_vc_set_capability: \n");
            retval = GA_vc_set_capability
                     (
                         &appl_vcp_ct_hndl,
                         &svc,
                         &appl_vcp_ct_vocs_ctx
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 5:
        {
            GA_BRR_SVC_INFO svc;

            svc.uuid = GA_SERVICE_AICS;

#if 1
            CONSOLE_OUT("Enter start handle:\n");
            CONSOLE_IN("%x", &choice);
            svc.range.start = (UINT16)(choice);

            CONSOLE_OUT("Enter end handle:\n");
            CONSOLE_IN("%x", &choice);
            svc.range.end = (UINT16)(choice);
#else

#if 0
            svc.range.start = 0x3D;
            svc.range.end = 0x4C;
#endif /* 0 */

            /* PTS GATT db */
            svc.range.start = 0xB0;
            svc.range.end = 0xBF;

            APPL_TRC("Start Handle: 0x%04x \n", svc.range.start);
            APPL_TRC("Start Handle: 0x%04x \n", svc.range.end);
#endif /* 0 */

            APPL_TRC("GA_vc_set_capability: \n");
            retval = GA_vc_set_capability
                     (
                         &appl_vcp_ct_hndl,
                         &svc,
                         &appl_vcp_ct_aics_ctx
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 6:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_vc_release: \n");
                retval = GA_vc_release(&appl_vcp_ct_hndl);
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_vc_close: \n");
                retval = GA_vc_close(&appl_vcp_ct_hndl);
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
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
                APPL_TRC("GA_vc_release_capability: \n");
                retval = GA_vc_release_capability
                         (
                             VCP_SERVICE_TYPE_VOCS,
                             &appl_vcp_ct_vocs_ctx
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_vc_close_capability: \n");
                retval = GA_vc_close_capability
                         (
                             VCP_SERVICE_TYPE_VOCS,
                             &appl_vcp_ct_vocs_ctx
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
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
                APPL_TRC("GA_vc_release_capability: \n");
                retval = GA_vc_release_capability
                         (
                             VCP_SERVICE_TYPE_AICS,
                             &appl_vcp_ct_aics_ctx
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_vc_close_capability: \n");
                retval = GA_vc_close_capability
                         (
                             VCP_SERVICE_TYPE_AICS,
                             &appl_vcp_ct_aics_ctx
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 9:
            CONSOLE_OUT("Select the service to Set the current handle\n");
            CONSOLE_OUT("1. VOCS\n2. AICS\n");
            CONSOLE_IN("%d", &choice);
            if (1U == (UINT8)choice)
            {
                CONSOLE_OUT("Enter VOCS Handle :");
                CONSOLE_IN("%d", &choice);

                if (VCP_MAX_VOCS_CONTEXTS <= (UINT8)choice)
                {
                    APPL_INF("Invalid Handle !\n");
                }
                else
                {
                    appl_vcp_ct_vocs_ctx = (UCHAR)choice;
                }
            }
            else if(2U == (UINT8)choice)
            {
                CONSOLE_OUT("Enter AICS Handle :");
                CONSOLE_IN("%d", &choice);

                if (VCP_MAX_AICS_CONTEXTS <= (UINT8)choice)
                {
                    APPL_INF("Invalid Handle !\n");
                }
                else
                {
                    appl_vcp_ct_aics_ctx = (UCHAR)choice;
                }
            }
            else
            {
                /* Do nothing */
            }
            break;

        case 10:
            APPL_TRC("GA_vc_get_volume_state: \n");
            retval = GA_vc_get_volume_state(appl_vcp_ct_hndl);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 11:
            APPL_TRC("GA_vc_get_volume_flags: \n");
            retval = GA_vc_get_volume_flags(appl_vcp_ct_hndl);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 12:
            APPL_TRC("GA_vc_set_rel_vol_down: \n");
            retval = GA_vc_set_rel_vol_down
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 13:
            APPL_TRC("GA_vc_set_rel_vol_up: \n");
            retval = GA_vc_set_rel_vol_up
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 14:
            APPL_TRC("GA_vc_set_unmute_rel_vol_down: \n");
            retval = GA_vc_set_unmute_rel_vol_down
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 15:
            APPL_TRC("GA_vc_set_unmute_rel_vol_up: \n");
            retval = GA_vc_set_unmute_rel_vol_up
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 16:
        {
            UCHAR appl_set_volume[2U];

            appl_set_volume[0U] =
                appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter;

            CONSOLE_OUT("Enter Absolute Volume: ");
            CONSOLE_IN("%d", &input);

            appl_set_volume[1U] = (UINT8)input;

            APPL_TRC("GA_vc_set_abs_volume: \n");
            retval = GA_vc_set_abs_volume(appl_vcp_ct_hndl, appl_set_volume, 2U);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
        }
            break;

        case 17:
            APPL_TRC("GA_vc_set_unmute: \n");
            retval = GA_vc_set_unmute
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 18:
            APPL_TRC("GA_vc_set_mute: \n");
            retval = GA_vc_set_mute
                     (
                         appl_vcp_ct_hndl,
                         &appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 20:
            APPL_TRC("GA_vc_vocs_get_volume_offset_state: \n");
            retval = GA_vc_vocs_get_volume_offset_state(appl_vcp_ct_vocs_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            break;

        case 21:
            APPL_TRC("GA_vc_vocs_get_audio_location: \n");
            retval = GA_vc_vocs_get_audio_location(appl_vcp_ct_vocs_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            break;

        case 22:
            APPL_TRC("GA_vc_vocs_get_audio_out_description: \n");
            retval = GA_vc_vocs_get_audio_out_description(appl_vcp_ct_vocs_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            break;

        case 23:
        {
            UCHAR appl_set_volume[4U];

            appl_set_volume[0U] =
                appl_vc_vocs_char_info[appl_vcp_ct_vocs_ctx].offset_state->change_counter;

            CONSOLE_OUT("Enter Volume Offset Value: ");
            CONSOLE_IN("%d", &input);

            appl_vc_vocs_char_info[appl_vcp_ct_vocs_ctx].offset_state->vol_offset =
                (UINT16)input;

            GA_PACK_LE_2_BYTE_VAL
            (
                &appl_set_volume[1U],
                appl_vc_vocs_char_info[appl_vcp_ct_vocs_ctx].offset_state->vol_offset
            );

            APPL_TRC("GA_vc_vocs_set_volume_offset: \n");
            retval = GA_vc_vocs_set_volume_offset
                     (
                         appl_vcp_ct_vocs_ctx,
                         &appl_set_volume[0U],
                         3U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 24:
        {
            UINT32  appl_audio_location;
            appl_audio_location = appl_ga_utils_bap_get_audio_channel_alloc();

            retval = GA_vc_vocs_set_audio_location
                     (
                         appl_vcp_ct_vocs_ctx,
                         appl_audio_location
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 25:
        {
            UCHAR   aod[16U];
            UCHAR   aodl;

            EM_str_copy
            (
                aod,
                APPL_VCS_VOCS_OUTPUT_DESC
            );
            aodl = (UCHAR )EM_str_len(aod) + 1U;

            APPL_TRC("GA_vc_vocs_set_audio_output_description: \n");
            retval = GA_vc_vocs_set_audio_output_description
                     (
                         appl_vcp_ct_vocs_ctx,
                         aod,
                         aodl
                    );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 30:
            APPL_TRC("GA_vc_aics_get_input_state: \n");
            retval = GA_vc_aics_get_input_state(appl_vcp_ct_aics_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 31:
            APPL_TRC("GA_vc_aics_get_gain_setting_proprty: \n");
            retval = GA_vc_aics_get_gain_setting_proprty(appl_vcp_ct_aics_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 32:
            APPL_TRC("GA_vc_aics_get_input_type: \n");
            retval = GA_vc_aics_get_input_type(appl_vcp_ct_aics_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 33:
            APPL_TRC("GA_vc_aics_get_input_status: \n");
            retval = GA_vc_aics_get_input_status(appl_vcp_ct_aics_ctx);
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 34:
            APPL_TRC("GA_vc_aics_get_audio_input_description: \n");
            retval = GA_vc_aics_get_audio_input_description
                     (
                         appl_vcp_ct_aics_ctx
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 35:
        {
            UCHAR   aid[16U];
            UCHAR   aidl;

            EM_str_copy
            (
                aid,
                APPL_VCS_AICS_INPUT_DESC
            );

            aidl = (UCHAR )EM_str_len(aid) + 1U;

            APPL_TRC("GA_vc_aics_set_audio_input_description: \n");
            retval = GA_vc_aics_set_audio_input_description
                     (
                         appl_vcp_ct_aics_ctx,
                         aid,
                         aidl
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 36:
        {
            UCHAR appl_audio_input[2U];

            CONSOLE_OUT("Enter Gain Setting(-128 to 127): ");
            CONSOLE_IN("%d", &input);

            appl_audio_input[1U] = (CHAR)input;
            appl_audio_input[0U] =
                appl_aics_chr_info[appl_vcp_ct_aics_ctx].input_state->change_counter;

            APPL_TRC("GA_vc_aics_set_gain_setting: \n");
            retval = GA_vc_aics_set_gain_setting
                     (
                         appl_vcp_ct_aics_ctx,
                         &appl_audio_input[0U],
                         2U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 37:
        {
            APPL_TRC("GA_vc_aics_unmute: \n");
            retval = GA_vc_aics_unmute
                     (
                         appl_vcp_ct_aics_ctx,
                         &appl_aics_chr_info[appl_vcp_ct_aics_ctx].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 38:
        {
            APPL_TRC("GA_vc_aics_mute: \n");
            retval = GA_vc_aics_mute
                     (
                         appl_vcp_ct_aics_ctx,
                         &appl_aics_chr_info[appl_vcp_ct_aics_ctx].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 39:
        {
            APPL_TRC("GA_vc_aics_set_manual_gain_mode: \n");
            retval = GA_vc_aics_set_manual_gain_mode
                     (
                         appl_vcp_ct_aics_ctx,
                         &appl_aics_chr_info[appl_vcp_ct_aics_ctx].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 40:
        {
            APPL_TRC("GA_vc_aics_set_auto_gain_mode: \n");
            retval = GA_vc_aics_set_auto_gain_mode
                     (
                         appl_vcp_ct_aics_ctx,
                         &appl_aics_chr_info[appl_vcp_ct_aics_ctx].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
        }
            break;

        case 80:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            appl_vcp_vcs_handle_access = appl_vcp_ct_hndl;
            APPL_TRC("GA_vc_get_context_info ...\n");
            retval = GA_vc_get_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_vcs_handle_access,
                         &appl_vcp_vcs_range_ptr,
                         &appl_vcp_vcs_char_info_ptr,
                         &appl_vcp_vcs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_vcp_vcs_char_count:0x%02x\n",
                    appl_vcp_vcs_char_count
                );

                appl_vcp_vcs_range = *appl_vcp_vcs_range_ptr;

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_vcp_vcs_char_info =
                    GA_alloc_mem(appl_vcp_vcs_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_vcp_vcs_char_info)
                {
                    GA_mem_copy
                    (
                        appl_vcp_vcs_char_info,
                        appl_vcp_vcs_char_info_ptr,
                        appl_vcp_vcs_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 81:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_vcp_vcs_char_info)
            {
                APPL_TRC("appl_vcp_vcs_char_info is NULL \n");
                APPL_TRC("Call GA_vc_get_vcs_context_info\n");
                break;
            }

            appl_vcp_vcs_char_info_ptr = appl_vcp_vcs_char_info;
            appl_vcp_vcs_range_ptr = &appl_vcp_vcs_range;

            APPL_TRC("GA_vc_set_context_info ...\n");
            retval = GA_vc_set_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_vcs_handle_access,
                         &appl_vcp_vcs_range_ptr,
                         &appl_vcp_vcs_char_info_ptr,
                         &appl_vcp_vcs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            if (GA_SUCCESS == retval)
            {
                appl_vcp_ct_hndl = appl_vcp_vcs_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_vcp_vcs_char_info);
                appl_vcp_vcs_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 82:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            appl_vcp_vcs_handle_access = appl_vcp_ct_hndl;
            appl_vcp_vocs_handle_access = appl_vcp_ct_vocs_ctx;

            APPL_TRC("GA_vc_get_vocs_context_info ...\n");
            retval = GA_vc_get_vocs_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_vocs_handle_access,
                         &appl_vcp_vocs_range_ptr,
                         &appl_vcp_vocs_char_info_ptr,
                         &appl_vcp_vocs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);

            if (GA_SUCCESS == retval)
            {
                APPL_TRC("appl_vcp_vocs_char_count:0x%02x\n", appl_vcp_vocs_char_count);

                appl_vcp_vocs_range = *appl_vcp_vocs_range_ptr;
                appl_vcp_vocs_char_info =
                    GA_alloc_mem(appl_vcp_vocs_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_vcp_vocs_char_info)
                {
                    GA_mem_copy
                    (
                        appl_vcp_vocs_char_info,
                        appl_vcp_vocs_char_info_ptr,
                        appl_vcp_vocs_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 83:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_vcp_vocs_char_info)
            {
                APPL_TRC("appl_vcp_vocs_char_info is NULL \n");
                APPL_TRC("Call GA_vc_get_vocs_context_info\n");
                break;
            }

            appl_vcp_vocs_char_info_ptr = appl_vcp_vocs_char_info;
            appl_vcp_vocs_range_ptr = &appl_vcp_vocs_range;

            APPL_TRC("GA_vc_set_vocs_context_info ...\n");
            retval = GA_vc_set_vocs_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_vocs_handle_access,
                         &appl_vcp_vocs_range_ptr,
                         &appl_vcp_vocs_char_info_ptr,
                         &appl_vcp_vocs_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            if (GA_SUCCESS == retval)
            {
                appl_vcp_ct_vocs_ctx = appl_vcp_vocs_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_vcp_vocs_char_info);
                appl_vcp_vocs_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 84:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            appl_vcp_vcs_handle_access = appl_vcp_ct_hndl;
            appl_vcp_aics_handle_access = appl_vcp_ct_aics_ctx;

            APPL_TRC("GA_vc_get_aics_context_info ...\n");
            retval = GA_vc_get_aics_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_aics_handle_access,
                         &appl_vcp_aics_range_ptr,
                         &appl_vcp_aics_char_info_ptr,
                         &appl_vcp_aics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);

            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_vcp_aics_char_count:0x%02x\n",
                    appl_vcp_aics_char_count
                );

                appl_vcp_aics_range = *appl_vcp_aics_range_ptr;
                appl_vcp_aics_char_info =
                    GA_alloc_mem(appl_vcp_aics_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_vcp_aics_char_info)
                {
                    GA_mem_copy
                    (
                        appl_vcp_aics_char_info,
                        appl_vcp_aics_char_info_ptr,
                        appl_vcp_aics_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 85:
        {
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_vcp_aics_char_info)
            {
                APPL_TRC("appl_vcp_aics_char_info is NULL \n");
                APPL_TRC("Call GA_vc_get_aics_context_info\n");
                break;
            }

            appl_vcp_aics_char_info_ptr = appl_vcp_aics_char_info;
            appl_vcp_aics_range_ptr = &appl_vcp_aics_range;

            APPL_TRC("GA_vc_set_aics_context_info ...\n");
            retval = GA_vc_set_aics_context_info
                     (
                         &appl_vcp_peer_rd_dev,
                         &appl_vcp_aics_handle_access,
                         &appl_vcp_aics_range_ptr,
                         &appl_vcp_aics_char_info_ptr,
                         &appl_vcp_aics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            if (GA_SUCCESS == retval)
            {
                appl_vcp_ct_aics_ctx = appl_vcp_aics_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_vcp_aics_char_info);
                appl_vcp_aics_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 87:
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_vc_set_context_info() is NOT called after
             * GA_vc_get_context_info().
             */

            /* Free the allocated memory for VCS char. info. */
            if (NULL != appl_vcp_vcs_char_info)
            {
                APPL_TRC("Freeing appl_vcp_vcs_char_info ...\n");
                GA_free_mem(appl_vcp_vcs_char_info);
                appl_vcp_vcs_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 88:
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_vc_set_vocs_context_info() is NOT called after
             * GA_vc_get_vocs_context_info().
             */

            /* Free the allocated memory for VOCS char. info. */
            if (NULL != appl_vcp_vocs_char_info)
            {
                APPL_TRC("Freeing appl_vcp_vocs_char_info ...\n");
                GA_free_mem(appl_vcp_vocs_char_info);
                appl_vcp_vocs_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 89:
#ifdef VCP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_vc_set_aics_context_info() is NOT called after
             * GA_vc_get_aics_context_info().
             */

            /* Free the allocated memory for AICS char. info. */
            if (NULL != appl_vcp_aics_char_info)
            {
                APPL_TRC("Freeing appl_vcp_aics_char_info ...\n");
                GA_free_mem(appl_vcp_aics_char_info);
                appl_vcp_aics_char_info = NULL;
            }
#else /* VCP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("VCP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
        {
#ifdef VCP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_vc_update_ntf_configuration...\n");
            retval = GA_vc_update_ntf_configuration
                     (
                         VCP_SERVICE_TYPE_VCS,
                        (VCS_CHAR_CONFIG(VCS_CHAR_ID_VOLUME_STATE)
                         /* |VCS_CHAR_CONFIG(VCS_CHAR_ID_VOLUME_FLAGS)*/)
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
#else /* VCP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("VCP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* VCP_SUPPORT_CONFIG_SELECTION */
        }
        break;

        case 91:
        {
#ifdef VCP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_vc_update_ntf_configuration...\n");
            retval = GA_vc_update_ntf_configuration
                     (
                         VCP_SERVICE_TYPE_VOCS,
                        (VOCS_CHAR_CONFIG(VOCS_CHAR_ID_VOLUME_OFFSET_STATE)
                         | VOCS_CHAR_CONFIG(VOCS_CHAR_ID_AUDIO_LOCATION)
                         /* | VOCS_CHAR_CONFIG(VOCS_CHAR_ID_VOLUME_DESCRIPTION) */)
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
#else /* VCP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("VCP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* VCP_SUPPORT_CONFIG_SELECTION */
        }
        break;

        case 92:
        {
#ifdef VCP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_vc_update_ntf_configuration...\n");
            retval = GA_vc_update_ntf_configuration
                     (
                         VCP_SERVICE_TYPE_AICS,
                        (AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_STATE)
                         | AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_STATUS)
                         /* | AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_DESCRIPTION) */)
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
#else /* VCP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("VCP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* VCP_SUPPORT_CONFIG_SELECTION */
        }
        break;

        default:
            APPL_INF("Invalid Option \n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

GA_RESULT appl_vcp_ct_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT          retval;
    GA_ENDPOINT      * device;
    UCHAR              i;
    UCHAR            * ga_char_val;
    GA_BRR_SVC_INFO  * svc;
    VCP_EVENT_INFO   * vcp_event_info;

    UCHAR              vcp_srvc_inst;
    UCHAR            * vcp_event_data;
    UCHAR              vcp_event_datalen;

    /* Initialize */
    retval = GA_SUCCESS;
    device = NULL;
    i = 0U;
    ga_char_val = NULL;
    svc = NULL;

    vcp_event_info = (VCP_EVENT_INFO *)ga_data;
    vcp_event_data = vcp_event_info->data;
    vcp_event_datalen = vcp_event_info->datalen;
    vcp_srvc_inst = vcp_event_info->srvc_inst;

    APPL_TRC("[Event Status]: ");
    appl_vcp_display_ga_result(ga_status, vcp_event_info->srvc_type);

    GA_IGNORE_UNUSED_PARAM(device);
    GA_IGNORE_UNUSED_PARAM(i);
    GA_IGNORE_UNUSED_PARAM(ga_char_val);
    GA_IGNORE_UNUSED_PARAM(ga_datalen);

    switch (ga_event)
    {
    case GA_VC_SETUP_CNF:
        APPL_TRC("[Event]       : GA_VC_SETUP_CNF (0x%02X)\n", ga_event);
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_GET_VOLUME_STATE_CNF: /* Fall - through */
    case GA_VC_VOLUME_STATE_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_GET_VOLUME_STATE_CNF == ga_event)
        {
            APPL_TRC("GA_VC_GET_VOLUME_STATE_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_VOLUME_STATE_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Char UUID: Volume State (0x%04X)\n", GA_CHAR_VCS_VOLUME_STATE);
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (3U == vcp_event_datalen)
        {
            appl_vcp_cntrl_vcs_vol_val.vol_state.volume_setting =
                vcp_event_data[0U];
            appl_vcp_cntrl_vcs_vol_val.vol_state.mute = vcp_event_data[1U];
            appl_vcp_cntrl_vcs_vol_val.vol_state.change_counter =
                vcp_event_data[2U];

            appl_vcp_parse_and_display_vcs_char_value
            (
                GA_CHAR_VCS_VOLUME_STATE,
                vcp_event_data
            );
        }
        break;

    case GA_VC_GET_VOLUME_FLAGS_CNF: /* Fall - through */
    case GA_VC_VOLUME_FLAGS_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_GET_VOLUME_FLAGS_CNF == ga_event)
        {
            APPL_TRC("GA_VC_GET_VOLUME_FLAGS_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_VOLUME_FLAGS_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Char UUID: Volume Flags (0x%04X)\n", GA_CHAR_VCS_VOLUME_FLAGS);
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (1U == vcp_event_datalen)
        {
            appl_vcp_cntrl_vcs_vol_val.volume_flags = vcp_event_data[0U];

            appl_vcp_parse_and_display_vcs_char_value
            (
                GA_CHAR_VCS_VOLUME_FLAGS,
                vcp_event_data
            );
        }
        break;

    case GA_VC_CP_WT_CNF:
        APPL_TRC("[Event]       : GA_VC_CP_WT_CNF (0x%02X)\n", ga_event);
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Volume Control Point (0x%04X)\n",
            GA_CHAR_VCS_VOLUME_CP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_GET_CAPABILITIES_CNF:
        APPL_TRC
        (
            "[Event]       : GA_VC_GET_CAPABILITIES_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        svc = (GA_BRR_SVC_INFO *)vcp_event_data;

        if (NULL != svc)
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
            APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
            if (0U == vcp_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case GA_VC_SET_CAPABILITY_CNF:
        APPL_TRC
        (
            "[Event]       : GA_VC_SET_CAPABILITY_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_VOCS_GET_OFFSET_STATE_CNF: /* Fall - through */
    case GA_VC_VOCS_OFFSET_STATE_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_VOCS_GET_OFFSET_STATE_CNF == ga_event)
        {
            APPL_TRC("GA_VC_VOCS_GET_OFFSET_STATE_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_VOCS_OFFSET_STATE_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Volume Offset State (0x%04X)\n",
            GA_CHAR_VOCS_OFFSET_STATE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_VOCS_CONTEXTS)
        {
            if (3U == vcp_event_datalen)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &appl_vc_vocs_char_info[vcp_srvc_inst].offset_state->vol_offset,
                    &vcp_event_data[0U]
                );

                appl_vc_vocs_char_info[vcp_srvc_inst].offset_state->change_counter =
                    vcp_event_data[2U];
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_vcp_parse_and_display_vocs_char_value
        (
            GA_CHAR_VOCS_OFFSET_STATE,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_VOCS_GET_AUDIO_LOCATION_CNF: /* Fall - through */
    case GA_VC_VOCS_AUDIO_LOCATION_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_VOCS_GET_AUDIO_LOCATION_CNF == ga_event)
        {
            APPL_TRC("GA_VC_VOCS_GET_AUDIO_LOCATION_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_VOCS_AUDIO_LOCATION_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Location (0x%04X)\n",
            GA_CHAR_VOCS_AUDIO_LOCATION
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_VOCS_CONTEXTS)
        {
            if (4U == vcp_event_datalen)
            {
                GA_UNPACK_LE_4_BYTE
                (
                    &appl_vc_vocs_char_info[vcp_srvc_inst].audio_loc,
                    &vcp_event_data[0U]
                );
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_vcp_parse_and_display_vocs_char_value
        (
            GA_CHAR_VOCS_AUDIO_LOCATION,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF: /* Fall - through */
    case GA_VC_VOCS_AUDIO_OUT_DESC_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF == ga_event)
        {
            APPL_TRC("GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_VOCS_AUDIO_OUT_DESC_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Output Description (0x%04X)\n",
            GA_CHAR_VOCS_AUDIO_OUTPUT_DESC
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_VOCS_CONTEXTS)
        {
            if (APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN < vcp_event_datalen)
            {
                appl_vc_vocs_char_info[vcp_srvc_inst].audio_out_desc_len =
                    APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN;
            }
            else
            {
                appl_vc_vocs_char_info[vcp_srvc_inst].audio_out_desc_len =
                    (UCHAR)vcp_event_datalen;
            }

            GA_mem_copy
            (
                &appl_vc_vocs_char_info[vcp_srvc_inst].audio_out_desc[0U],
                &vcp_event_data[0U],
                appl_vc_vocs_char_info[vcp_srvc_inst].audio_out_desc_len
            );
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_vcp_parse_and_display_vocs_char_value
        (
            GA_CHAR_VOCS_AUDIO_OUTPUT_DESC,
            vcp_event_data,
            vcp_event_datalen
        );
        break;

    case GA_VC_RELEASE_CAPABILITY_CNF:
        APPL_TRC
        (
            "[Event]       : GA_VC_RELEASE_CAPABILITY_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_RELEASE_CNF:
        APPL_TRC("[Event]       : GA_VC_RELEASE_CNF (0x%02X)\n", ga_event);
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_AICS_GET_INPUT_STATE_CNF: /* Fall - through */
    case GA_VC_AICS_INPUT_STATE_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_AICS_GET_INPUT_STATE_CNF == ga_event)
        {
            APPL_TRC("GA_VC_AICS_GET_INPUT_STATE_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_AICS_INPUT_STATE_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input State (0x%04X)\n",
            GA_CHAR_AICS_INPUT_STATE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_AICS_CONTEXTS)
        {
            if (4U == vcp_event_datalen)
            {
                appl_aics_chr_info[vcp_srvc_inst].input_state->gain_setting =
                    vcp_event_data[0U];
                appl_aics_chr_info[vcp_srvc_inst].input_state->mute =
                    vcp_event_data[1U];
                appl_aics_chr_info[vcp_srvc_inst].input_state->gain_mode =
                    vcp_event_data[2U];
                appl_aics_chr_info[vcp_srvc_inst].input_state->change_counter =
                    vcp_event_data[3U];

            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_aics_parse_and_display_char_value
        (
            GA_CHAR_AICS_INPUT_STATE,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF:
        APPL_TRC
        (
            "[Event]       : GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Gain Setting Properties (0x%04X)\n",
            GA_CHAR_AICS_GAIN_SETTING
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_AICS_CONTEXTS)
        {
            if (3U == vcp_event_datalen)
            {
                appl_aics_chr_info[vcp_srvc_inst].gain_setting->gain_setting_units =
                    vcp_event_data[0U];
                appl_aics_chr_info[vcp_srvc_inst].gain_setting->gain_settting_min =
                    vcp_event_data[1U];
                appl_aics_chr_info[vcp_srvc_inst].gain_setting->gain_setting_max =
                    vcp_event_data[2U];
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_aics_parse_and_display_char_value
        (
            GA_CHAR_AICS_GAIN_SETTING,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_AICS_GET_INPUT_TYPE_CNF:
        APPL_TRC
        (
            "[Event]       : GA_VC_AICS_GET_INPUT_TYPE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Type (0x%04X)\n",
            GA_CHAR_AICS_INPUT_TYPE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_AICS_CONTEXTS)
        {
            if (1U == vcp_event_datalen)
            {
                appl_aics_chr_info[vcp_srvc_inst].input_type = vcp_event_data[0U];
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_aics_parse_and_display_char_value
        (
            GA_CHAR_AICS_INPUT_TYPE,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_AICS_GET_INPUT_STATUS_CNF: /* Fall - through */
    case GA_VC_AICS_INPUT_STATUS_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_AICS_GET_INPUT_STATUS_CNF == ga_event)
        {
            APPL_TRC("GA_VC_AICS_GET_INPUT_STATUS_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_VC_AICS_INPUT_STATUS_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Status (0x%04X)\n",
            GA_CHAR_AICS_INPUT_STATUS
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_AICS_CONTEXTS)
        {
            if (1U == vcp_event_datalen)
            {
                appl_aics_chr_info[vcp_srvc_inst].input_status =
                    vcp_event_data[0U];
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_aics_parse_and_display_char_value
        (
            GA_CHAR_AICS_INPUT_STATUS,
            vcp_event_data,
            vcp_event_datalen
        );

        break;

    case GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF: /* Fall - through */
    case GA_VC_AICS_AUDIO_INPUT_DESC_NTF:
        APPL_TRC("[Event]       : ");
        if (GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF == ga_event)
        {
            APPL_TRC
            (
                "GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF (0x%02X)\n",
                ga_event
            );
        }
        else
        {
            APPL_TRC("GA_VC_AICS_AUDIO_INPUT_DESC_NTF (0x%02X)\n", ga_event);
        }
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Description (0x%04X)\n",
            GA_CHAR_AICS_AUDIO_INPUT_DESC
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        APPL_TRC("Data:\n");

        if (vcp_srvc_inst < VCP_MAX_AICS_CONTEXTS)
        {
            if (APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN < vcp_event_datalen)
            {
                appl_aics_chr_info[vcp_srvc_inst].audio_in_desc_len =
                    APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN;
            }
            else
            {
                appl_aics_chr_info[vcp_srvc_inst].audio_in_desc_len =
                    vcp_event_datalen;
            }

            GA_mem_copy
            (
                &appl_aics_chr_info[vcp_srvc_inst].audio_in_desc[0U],
                &vcp_event_data[0U],
                appl_aics_chr_info[vcp_srvc_inst].audio_in_desc_len
            );
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }

        appl_aics_parse_and_display_char_value
        (
            GA_CHAR_AICS_AUDIO_INPUT_DESC,
            vcp_event_data,
            vcp_event_datalen
        );
        break;

    case GA_VC_VOCS_CP_WT_CNF:
        APPL_TRC("[Event]       : GA_VC_VOCS_CP_WT_CNF (0x%02X)\n", ga_event);
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Volume Offset Control Point (0x%04X)\n",
            GA_CHAR_VOCS_OFFSET_CP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_VC_AICS_CP_WT_CNF:
        APPL_TRC("[Event]       : GA_VC_AICS_CP_WT_CNF (0x%02X)\n", ga_event);
        APPL_TRC("[Service Cntx]: 0x%02X\n", vcp_srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Control Point (0x%04X)\n",
            GA_CHAR_AICS_AUDIO_INPUT_CP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_datalen);
        if (0U == vcp_event_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    default:
        APPL_ERR("UnHandled Event: (0x%02X) !\n", ga_event);
        break;
    }

    return retval;
}
#endif /* VCP_CONTROLLER */

/*** APIs - VCP Renderer ***/

#ifdef VCP_RENDERER
GA_RESULT appl_vcp_rd_reg_opt_service(UCHAR srvc_type)
{
    GA_RESULT  retval;
    GA_VR_HANDLE handle;

    /* Initialize */
    retval = GA_FAILURE;

    if (VCP_SERVICE_TYPE_VOCS == srvc_type)
    {
        retval = GA_vc_register(VCP_SERVICE_TYPE_VOCS, &handle);
        if (GA_SUCCESS == retval)
        {
            APPL_TRC("VCP_VOCS Handle: 0x%02X\n", handle);

            /* By default, Update the Handle to the latest registered */
            vcr_vocs_handle = handle;
        }
    }
    else if (VCP_SERVICE_TYPE_AICS == srvc_type)
    {
        retval = GA_vc_register(VCP_SERVICE_TYPE_AICS, &handle);
        if (GA_SUCCESS == retval)
        {
            APPL_TRC("VCP_AICS Handle: 0x%02X\n", handle);

            /* By default, Update the Handle to the latest registered */
            vcr_aics_handle = handle;
        }
    }
    else
    {
        APPL_ERR("Received Invalid Service Type !\n");
    }
    return retval;
}

void appl_vcp_rd_set_default_values(void)
{
    UINT8 i;

    i = 0U;

    /* Setting of VCS Default values */
    appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting = 0x04U;
    appl_vcp_rndr_vcs_vol_val.vol_state.mute = VCS_MUTE_YES;

    appl_vcp_rndr_vcs_vol_val.vol_state.change_counter = 254U;

    appl_vcp_rndr_vcs_vol_val.volume_flags = VCS_VOLUME_SETTING_PERSIST_NO;

    /* Setting of VOCS Default values */
    /* 1st Instance */
    appl_vc_rndr_vocs_offset_state_info[i].vol_offset = 0x0020;

    appl_vc_rndr_vocs_offset_state_info[i].change_counter = 254U;

    /* TODO: Check the spec. */
    appl_vc_rndr_vocs_char_info[i].audio_loc = GA_AUDIO_LOCATION_FC;

    appl_vc_rndr_vocs_char_info[i].audio_out_desc_len =
        (UCHAR )EM_str_len(appl_audio_op_desptn) + 1U;

    EM_str_copy
    (
        appl_vc_rndr_vocs_char_info[i].audio_out_desc,
        appl_audio_op_desptn
    );

    /* 2nd Instance */
    if (++i < VCP_MAX_VOCS_ENTITIES)
    {
        appl_vc_rndr_vocs_offset_state_info[i].vol_offset = 0x0030;

        appl_vc_rndr_vocs_offset_state_info[i].change_counter = VOCS_CHNG_CNTR_MAX;

        /* TODO: Check the spec. */
        appl_vc_rndr_vocs_char_info[i].audio_loc = GA_AUDIO_LOCATION_LOW_FREQ_EFF_1;

        EM_str_copy
        (
            appl_vc_rndr_vocs_char_info[i].audio_out_desc,
            appl_audio_op_desptn1
        );

        appl_vc_rndr_vocs_char_info[i].audio_out_desc_len =
            (UCHAR )GA_str_len(appl_audio_op_desptn1) + 1U;
    }

    /* VOCS: Offset State */
    for (i = 0U; i < VCP_MAX_VOCS_ENTITIES; i++)
    {
        appl_vc_rndr_vocs_char_info[i].offset_state =
            &appl_vc_rndr_vocs_offset_state_info[i];

    }

    /* Setting of AICS Default values */
    for (i = 0U; i < VCP_MAX_AICS_ENTITIES; i++)
    {
        aics_chr_info[i].input_state = &aics_input_state[i];
        aics_chr_info[i].gain_setting = &aics_input_gain_setting[i];
        aics_chr_info[i].input_type = GA_AUDIO_INPUT_TYPE_BLUETOOTH;
        aics_chr_info[i].input_status = AICS_IN_STATUS_ACTIVE;

        aics_input_state[i].gain_setting = 0x01;
        aics_input_state[i].mute = AICS_NOT_MUTED;
        aics_input_state[i].gain_mode = /* AICS_GM_MANUAL */ AICS_GM_AUTOMATIC_ONLY;
        aics_input_state[i].change_counter = 254U;

        aics_input_gain_setting[i].gain_setting_units = 0x01U;
        aics_input_gain_setting[i].gain_settting_min = AICS_GAIN_SETTING_MIN;
        aics_input_gain_setting[i].gain_setting_max = AICS_GAIN_SETTING_MAX;

        EM_str_copy
        (
            aics_chr_info[i].audio_in_desc,
            appl_audio_in_desptn
        );
        aics_chr_info[i].audio_in_desc_len =
            (UCHAR )EM_str_len(appl_audio_in_desptn) + 1U;
    }

    /* Timer Handles for sending Notifications */
    appl_vcs_cp_wt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_vocs_cp_wt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_vocs_set_audio_loc_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_vocs_set_audio_out_desc_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_aics_cp_wt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_aics_set_audio_inp_desc_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void appl_vcp_rd_cp_write_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * vcs_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\n");
    }
    else
    {
        vcs_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_VCS_VOLUME_STATE;

        retval = appl_vcp_rd_pack_vcs_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &appl_vcp_rndr_vcs_vol_val,
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_notify_vcs_volume_state\n");
        retval = GA_vc_notify_vcs_volume_state
                 (
                     &(vcs_ntf_info->device),
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);

        if (0U != appl_ntf_volume_flag)
        {
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VCS_VOLUME_FLAGS;
            retval = appl_vcp_rd_pack_vcs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vcp_rndr_vcs_vol_val,
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            if (GA_FAILURE == retval)
            {
                APPL_ERR("Cannot pack VCS Characteristic Value !\n");
            }
            else
            {
                APPL_TRC("GA_vc_notify_vcs_volume_flag\n");
                retval = GA_vc_notify_vcs_volume_flag
                         (
                             &(vcs_ntf_info->device),
                             appl_vcr_ntf_info.data,
                             appl_vcr_ntf_info.datalen
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            }
        }
    }

    return;
}

void appl_vcp_vocs_rd_cp_write_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * vocs_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected VOCS Service Instance !\n");
    }
    else
    {
        vocs_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (VCP_MAX_VOCS_ENTITIES <= vocs_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid VOCS Handle !\n");
            return;
        }

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_OFFSET_STATE;
        retval = appl_vcp_rd_pack_vocs_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &appl_vc_rndr_vocs_char_info[vocs_ntf_info->srvc_inst],
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_notify_vocs_volume_offset\n");
        retval = GA_vc_notify_vocs_volume_offset
                 (
                     &(vocs_ntf_info->device),
                     vocs_ntf_info->srvc_inst,
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
    }

    return;
}

void appl_vcp_vocs_rd_set_audio_loc_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * vocs_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected VOCS Service Instance !\n");
    }
    else
    {
        vocs_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (VCP_MAX_VOCS_ENTITIES <= vocs_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid VOCS Handle !\n");
            return;
        }

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_AUDIO_LOCATION;
        retval = appl_vcp_rd_pack_vocs_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &appl_vc_rndr_vocs_char_info[vocs_ntf_info->srvc_inst],
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_notify_vocs_audio_location\n");
        retval = GA_vc_notify_vocs_audio_location
                 (
                     &(vocs_ntf_info->device),
                     vocs_ntf_info->srvc_inst,
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
    }

    return;
}

void appl_vcp_vocs_rd_set_audio_out_desc_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * vocs_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected VOCS Service Instance !\n");
    }
    else
    {
        vocs_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (VCP_MAX_VOCS_ENTITIES <= vocs_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid VOCS Handle !\n");
            return;
        }

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_AUDIO_OUTPUT_DESC;
        retval = appl_vcp_rd_pack_vocs_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &appl_vc_rndr_vocs_char_info[vocs_ntf_info->srvc_inst],
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_notify_vocs_audio_out_desc\n");
        retval = GA_vc_notify_vocs_audio_out_desc
                 (
                     &(vocs_ntf_info->device),
                     vocs_ntf_info->srvc_inst,
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
    }

    return;
}

void appl_vcp_aics_rd_cp_write_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * aics_ntf_info;

    /* Initialize */
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected AICS Service Instance !\n");
    }
    else
    {
        aics_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (VCP_MAX_AICS_ENTITIES <= aics_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid AICS Handle !\n");
            return;
        }

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        APPL_TRC
        (
            "Change_Counter: 0x%02x\n",
            aics_chr_info[0U].input_state->change_counter
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_AICS_INPUT_STATE;
        retval = appl_vcp_rd_pack_aics_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &aics_chr_info[aics_ntf_info->srvc_inst],
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("GA_vc_notify_aics_audio_input_state \n");
        retval = GA_vc_notify_aics_audio_input_state
                 (
                     &(aics_ntf_info->device),
                     aics_ntf_info->srvc_inst,
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
    }

    return;
}

void appl_vcp_aics_rd_set_audio_inp_desc_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_VCP_NTF_INFO * aics_ntf_info;

    /* Initialize */
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected AICS Service Instance !\n");
    }
    else
    {
        aics_ntf_info = (APPL_VCP_NTF_INFO *)data;

        if (VCP_MAX_AICS_ENTITIES <= aics_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid AICS Handle !\n");
            return;
        }

        if (NULL == appl_vcr_ntf_info.chr)
        {
            APPL_ERR("appl_vcr_ntf_info.chr is NULL !\n");
            return;
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();

        appl_vcr_ntf_info.chr->uuid = GA_CHAR_AICS_AUDIO_INPUT_DESC;
        retval = appl_vcp_rd_pack_aics_chr_value
                 (
                     appl_vcr_ntf_info.chr->uuid,
                     &aics_chr_info[aics_ntf_info->srvc_inst],
                     appl_vcr_ntf_info.data,
                     &appl_vcr_ntf_info.datalen
                 );

        APPL_TRC("GA_vc_notify_aics_audio_input_desc \n");
        retval = GA_vc_notify_aics_audio_input_desc
                 (
                     &(aics_ntf_info->device),
                     aics_ntf_info->srvc_inst,
                     appl_vcr_ntf_info.data,
                     appl_vcr_ntf_info.datalen
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
    }

    return;
}

int appl_vcp_rd_operations(void)
{
    GA_RESULT           retval;
    int                 choice;
    int                 menu_choice;
    int                 input;
    UINT16              contexts;
    GA_BRR_CHR_CONTEXT  chr_ctx;
    UINT8               conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(contexts);
    GA_IGNORE_UNUSED_PARAM(input);

    /* Initialize */
    appl_vcr_ntf_datalen = 0U;
    appl_vcr_ntf_info.data = &appl_vcr_ntf_data[0U];
    appl_vcr_ntf_info.datalen = appl_vcr_ntf_datalen;
    appl_vcr_ntf_info.chr = &chr_ctx;

    GA_LOOP_FOREVER()
    {
        APPL_TRC("\n");
        CONSOLE_OUT("%s", appl_vcp_rd_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 7:
#if 1
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &appl_vcp_peer_rd_dev
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(appl_vcp_peer_rd_dev.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                appl_vcp_peer_rd_dev.bd_type = (UINT8)choice;
            }
#else
            BT_COPY_BD_ADDR(appl_vcp_peer_rd_dev.bd_addr, g_pts_bd_addr);
            appl_vcp_peer_rd_dev.bd_type = BT_BD_PUBLIC_ADDRESS_TYPE;
#endif
            break;

        case 8:
            CONSOLE_OUT("Register Included Service\n");
            CONSOLE_OUT("1. VOCS\n2. AICS\n");
            CONSOLE_IN("%d", &choice);
            if (1U == (UINT8)choice)
            {
                /* Register VOCS */
                APPL_TRC("GA_vc_register. for VOCS \n");
                retval = appl_vcp_rd_reg_opt_service
                         (
                             VCP_SERVICE_TYPE_VOCS
                         );
                APPL_TRC("Retval - 0x%04X\n", retval);
            }
            else if (2U == (UINT8)choice)
            {
                /* Register VCP-AICS */
                APPL_TRC("GA_vc_register..for AICS \n");
                retval = appl_vcp_rd_reg_opt_service
                         (
                             VCP_SERVICE_TYPE_AICS
                         );
                APPL_TRC("Retval - 0x%04X\n", retval);
            }
            else
            {
                /* Do nothing */
            }
            break;

        case 9:
            CONSOLE_OUT("Select the service to Set the current handle\n");
            CONSOLE_OUT("1. VOCS\n2. AICS\n");
            CONSOLE_IN("%d", &choice);
            if (1U == (UINT8)choice)
            {
                CONSOLE_OUT("Enter VOCS Handle :");
                CONSOLE_IN("%d", &choice);

                if (VCP_MAX_VOCS_ENTITIES <= (UINT8)choice)
                {
                    APPL_INF("Invalid Handle !\n");
                }
                else
                {
                    vcr_vocs_handle = (UCHAR)choice;
                }
            }
            else if(2U == (UINT8)choice)
            {
                CONSOLE_OUT("Enter AICS Handle :");
                CONSOLE_IN("%d", &choice);

                if (VCP_MAX_AICS_ENTITIES <= (UINT8)choice)
                {
                    APPL_INF("Invalid Handle !\n");
                }
                else
                {
                    vcr_aics_handle = (UCHAR)choice;
                }
            }
            else
            {
                /* Do nothing */
            }
            break;

        /* ================ Renderer Options ================= */
        case 50:
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VCS_VOLUME_STATE;
            retval = appl_vcp_rd_pack_vcs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vcp_rndr_vcs_vol_val,
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_vcs_volume_state \n");
            retval = GA_vc_notify_vcs_volume_state
                     (
                         &appl_vcp_peer_rd_dev,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 51:
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VCS_VOLUME_FLAGS;
            retval = appl_vcp_rd_pack_vcs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vcp_rndr_vcs_vol_val,
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_vcs_volume_flag \n");
            retval = GA_vc_notify_vcs_volume_flag
                     (
                         &appl_vcp_peer_rd_dev,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VCS);
            break;

        case 52:
        {
            if (VCP_MAX_VOCS_ENTITIES <= vcr_vocs_handle)
            {
                APPL_INF("Invalid Choice\n");
                break;
            }

            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_OFFSET_STATE;
            retval = appl_vcp_rd_pack_vocs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vc_rndr_vocs_char_info[vcr_vocs_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_vocs_volume_offset \n");
            retval = GA_vc_notify_vocs_volume_offset
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_vocs_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 53:
        {
            if (VCP_MAX_VOCS_ENTITIES <= vcr_vocs_handle)
            {
                APPL_INF("Invalid Choice\n");
                break;
            }

#if 0
            CONSOLE_OUT("Enter the audio location to be updated:\n");
            CONSOLE_IN("%x", &choice);
            appl_vc_rndr_vocs_char_info[0U].audio_loc = (UINT32)choice;
#endif /* 0 */
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_AUDIO_LOCATION;
            retval = appl_vcp_rd_pack_vocs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vc_rndr_vocs_char_info[vcr_vocs_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_vocs_audio_location \n");
            retval = GA_vc_notify_vocs_audio_location
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_vocs_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 54:
        {

            if (VCP_MAX_VOCS_ENTITIES <= vcr_vocs_handle)
            {
                APPL_INF("Invalid Choice\n");
                break;
            }

#if 0
            {
                CONSOLE_OUT("Enter Audio Out Desc:\n");
                CONSOLE_IN("%s", appl_audio_op_desptn);

                GA_mem_copy
                (
                    appl_vc_rndr_vocs_char_info[0U].audio_out_desc,
                    appl_audio_op_desptn,
                    EM_str_len(appl_audio_op_desptn)
                );

                appl_vc_rndr_vocs_char_info[0U].audio_out_desc_len =
                    (UCHAR )EM_str_len(appl_audio_op_desptn);
            }
#endif /* 0 */

            appl_vcr_ntf_info.chr->uuid = GA_CHAR_VOCS_AUDIO_OUTPUT_DESC;
            retval = appl_vcp_rd_pack_vocs_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &appl_vc_rndr_vocs_char_info[vcr_vocs_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_vocs_audio_out_desc \n");
            retval = GA_vc_notify_vocs_audio_out_desc
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_vocs_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
        }
            break;

        case 55:
            aics_chr_info[0U].input_state->change_counter += 1U;

            APPL_TRC
            (
                "\t======== AICS Input State Changed, Sending Notification ====\n"
            );
            APPL_TRC("Change_Counter: 0x%02x\n",
            aics_chr_info[0U].input_state->change_counter);

            appl_vcr_ntf_info.chr->uuid = GA_CHAR_AICS_INPUT_STATE;
            retval = appl_vcp_rd_pack_aics_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &aics_chr_info[vcr_aics_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_aics_audio_input_state \n");
            retval = GA_vc_notify_aics_audio_input_state
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_aics_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 56:
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_AICS_INPUT_STATUS;
            retval = appl_vcp_rd_pack_aics_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &aics_chr_info[vcr_aics_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_aics_audio_input_status \n");
            retval = GA_vc_notify_aics_audio_input_status
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_aics_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;

        case 57:
#if 0
            {
                CONSOLE_OUT("Enter Audio In Desc:\n");
                CONSOLE_IN("%s", appl_audio_in_desptn);

                EM_str_copy
                (
                    aics_chr_info[0U].audio_in_desc,
                    appl_audio_in_desptn
                );
                aics_chr_info[0U].audio_in_desc_len =
                    (UCHAR )EM_str_len(appl_audio_in_desptn);
            }
#endif /* 0 */
            appl_vcr_ntf_info.chr->uuid = GA_CHAR_AICS_AUDIO_INPUT_DESC;
            retval = appl_vcp_rd_pack_aics_chr_value
                     (
                         appl_vcr_ntf_info.chr->uuid,
                         &aics_chr_info[vcr_aics_handle],
                         appl_vcr_ntf_info.data,
                         &appl_vcr_ntf_info.datalen
                     );

            APPL_TRC("GA_vc_notify_aics_audio_input_desc \n");
            retval = GA_vc_notify_aics_audio_input_desc
                     (
                         &appl_vcp_peer_rd_dev,
                         vcr_aics_handle,
                         appl_vcr_ntf_info.data,
                         appl_vcr_ntf_info.datalen
                     );
            APPL_TRC("Retval - ");
            appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_AICS);
            break;


        /* ==================== Local Characterstic Updates =================== */
        case 58:
            CONSOLE_OUT
            (
                "Update Gain Mode \n0. Manual Only\n1. Auto Only\n"
                "2. Manual\n3. Auto\nEnter Gain Mode:"
            );
            CONSOLE_IN("%x", &choice);

            aics_input_state[vcr_aics_handle].gain_mode = (UCHAR)(choice);
            break;

        case 59:
            CONSOLE_OUT
            (
                "Update Mute Settings to \n 0.UNMUTE\n1. MUTE\n"
                "2. DISABLED\nEnter Value:"
            );
            CONSOLE_IN("%x", &choice);
            aics_input_state[vcr_aics_handle].mute = (UCHAR)(choice);
            break;

        case 60:
            EM_str_copy
            (
                &aics_chr_info[0U].audio_in_desc[0U],
                APPL_VCS_AICS_INPUT_DESC
            );
            aics_chr_info[0U].audio_in_desc_len =
                (UCHAR )EM_str_len(APPL_VCS_AICS_INPUT_DESC) + 1U;

            /**
             * Changing Last Char. of the APPL_VCS_AICS_INPUT_DESC.
             * This is to set different value for each option #60 selection.
             */
            aics_chr_info[vcr_aics_handle].audio_in_desc[aics_chr_info[0U].audio_in_desc_len - 2U] +=
                vcr_aics_out_description_cnt;
            vcr_aics_out_description_cnt += 1U;

            break;

        case 61:
        {
            CONSOLE_OUT("Enter AICS Gain Setting Min.(>= -128):");
            CONSOLE_IN("%d", &input);

            aics_input_gain_setting[0U].gain_settting_min = (CHAR )input;

            CONSOLE_OUT("Enter AICS Gain Setting Max. (<= 127):");
            CONSOLE_IN("%d", &input);

            aics_input_gain_setting[0U].gain_setting_max = (CHAR)input;
        }
            break;

        case 62:
            CONSOLE_OUT("Enter Volume Setting Value:\n");
            CONSOLE_IN("%d", &choice);
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting = (UCHAR)(choice);
            break;

        case 63:
            CONSOLE_OUT
            (
                "Update Mute Settings to \n0. UNMUTE\n1. MUTE\n\nEnter Value:"
            );
            CONSOLE_IN("%x", &choice);
            appl_vcp_rndr_vcs_vol_val.vol_state.mute =
                (0U == (UCHAR)choice) ? VCS_MUTE_NO : VCS_MUTE_YES;
            break;

        case 64:
            CONSOLE_OUT("Enter Volume Flags Value:\n");
            CONSOLE_IN("%x", &choice);
            appl_vcp_rndr_vcs_vol_val.volume_flags =
                (0U == (UCHAR)choice) ? 0x00U : 0x01U;
            break;

        case 65:
            CONSOLE_OUT("Enter Change Counter Value:\n");
            CONSOLE_IN("%d", &choice);
            appl_vcp_rndr_vcs_vol_val.vol_state.change_counter = (UCHAR)(choice);
            break;

        case 66:
            appl_vc_rndr_vocs_char_info[vcr_vocs_handle].audio_loc =
                appl_ga_utils_bap_get_audio_channel_alloc();
            break;

        case 67:
            EM_str_copy
            (
                appl_vc_rndr_vocs_char_info[vcr_vocs_handle].audio_out_desc,
                APPL_VCS_VOCS_OUTPUT_DESC
            );
            appl_vc_rndr_vocs_char_info[vcr_vocs_handle].audio_out_desc_len =
                (UCHAR)EM_str_len(APPL_VCS_VOCS_OUTPUT_DESC) + 1U;

            /**
             * Changing Last Char. of the APPL_VCS_VOCS_OUTPUT_DESC.
             * This is to set different value for each option #66 selection.
             */
            appl_vc_rndr_vocs_char_info[vcr_vocs_handle].audio_out_desc[appl_vc_rndr_vocs_char_info[vcr_vocs_handle].audio_out_desc_len - 2U] +=
                vcr_vocs_out_description_cnt;
            vcr_vocs_out_description_cnt += 1U;

            break;

        case 70:
            appl_vcp_rd_set_vcs_sdp_record_status(0x01U);
            break;

        case 71:
            appl_vcp_rd_set_vcs_sdp_record_status(0x00U);
            break;

        case 72:
            appl_vcp_rd_set_vocs_sdp_record_status(0x01U);
            break;

        case 73:
            appl_vcp_rd_set_vocs_sdp_record_status(0x00U);
            break;

        case 74:
            appl_vcp_rd_set_aics_sdp_record_status(0x01U);
            break;

        case 75:
            appl_vcp_rd_set_aics_sdp_record_status(0x00U);
            break;

        default:
            APPL_INF("Invalid Option \n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

void appl_vcp_rd_set_vcs_sdp_record_status(UCHAR flag)
{
#if 0
    GA_RESULT  retval;
    UINT32      record_handle;

    /* Get the VCS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_VCS, 0U, &record_handle);

    if (0U != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }

#else /* 0 */
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}

void appl_vcp_rd_set_vocs_sdp_record_status(UCHAR flag)
{
#if 0
    GA_RESULT  retval;
    UINT32      record_handle;

    /* Get the VOCS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_VOCS, 0U, &record_handle);

    if (0U != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }

#else /* 0 */
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}

void appl_vcp_rd_set_aics_sdp_record_status(UCHAR flag)
{
#if 0
    GA_RESULT  retval;
    UINT32      record_handle;

    /* Get the AICS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_AICS, 0U, &record_handle);

    if (0U != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }

#else /* 0 */
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}

GA_RESULT appl_vcp_rd_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT         retval;
    VCP_EVENT_INFO  * vcp_event_info;
    UCHAR             send_vcp_rsp_flag;
    UCHAR             rsp_event;
    UINT16            rsp_status;
    VCP_RSP_INF       vcp_rsp_info;
    UCHAR             i;
    UCHAR             vcp_srvc_inst;
    UCHAR             vocs_char_val_change_flag;
    UCHAR             aics_char_val_change_flag;

    APPL_VCP_NTF_INFO  vocs_aics_ntf_info;

    GA_IGNORE_UNUSED_PARAM(ga_datalen);

    /* Initialize */
    retval = GA_SUCCESS;
    send_vcp_rsp_flag = 0U;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;
    vcp_srvc_inst = 0U;
    i = 0U;
    vocs_char_val_change_flag = 0U;
    aics_char_val_change_flag = 0U;

    APPL_TRC("[Event Status]: ");
    appl_vcp_display_ga_result
    (
        ga_status,
        ((VCP_EVENT_INFO *)ga_data)->srvc_type
    );

    switch (ga_event)
    {
    case GA_VC_CP_WT_IND:
    {
        APPL_TRC("[Event]       : GA_VC_CP_WT_IND (0x%02X)\n", ga_event);
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Volume Control Point (0x%04X)\n",
            GA_CHAR_VCS_VOLUME_CP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        /* Should be 0 */
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        retval = appl_vcp_rd_vcs_cp_write_handler
                 (
                     vcp_event_info
                 );

        /* Response Status */
        rsp_status = retval;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        rsp_event = GA_RSP_TYPE_WT;
        device = vcp_event_info->device;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = NULL;
        vcp_rsp_info.datalen = 0U;
    }
    break;

    case GA_VC_VOCS_CP_WT_IND:
    {
        APPL_TRC("[Event]       : GA_VC_VOCS_CP_WT_IND (0x%02X)\n", ga_event);
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Volume Offset Control Point (0x%04X)\n",
            GA_CHAR_VOCS_OFFSET_CP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        retval = appl_vcp_rd_vocs_cp_write_handler
                 (
                     vcp_event_info
                 );

        /* Set the Respone Status */
        rsp_status = retval;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        rsp_event = GA_RSP_TYPE_WT;
        device = vcp_event_info->device;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = NULL;
        vcp_rsp_info.datalen = 0U;
    }
    break;

    case GA_VC_AICS_CP_WT_IND:
    {
        APPL_TRC("[Event]       : GA_VC_AICS_CP_WT_IND (0x%02X)\n", ga_event);
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Control Point (0x%04X)\n",
            GA_CHAR_AICS_AUDIO_INPUT_CP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        retval = appl_vcp_rd_aics_cp_write_handler
                 (
                     vcp_event_info
                 );

        /* Response Status */
        rsp_status = retval;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        rsp_event = GA_RSP_TYPE_WT;
        device = vcp_event_info->device;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = NULL;
        vcp_rsp_info.datalen = 0U;

    }
    break;

    case GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND:
    {
        APPL_TRC
        (
            "[Event]       : GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND (0x%02X)\n",
            ga_event
        );
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Input Description (0x%04X)\n",
            GA_CHAR_AICS_AUDIO_INPUT_DESC
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Audio Input Description: ",
            vcp_event_info->datalen
        );
        for (i = 0U; i < vcp_event_info->datalen; i++)
        {
            APPL_TRC("%c", vcp_event_info->data[i]);
        }
        APPL_TRC("\n");

        /* Update the Char. Value. */
        vcp_srvc_inst = vcp_event_info->srvc_inst;
        if (vcp_srvc_inst < VCP_MAX_AICS_ENTITIES)
        {
            /*
             * Check if the received value length is same as the current audio
             * Input description value length.
             * If No, update it with received value and update
             *         value change flag.
             * If Yes, compare the received value with the current value.
             *         If the value is different, then update it with received
             *         value and update value change flag, else no action.
             */
            if ((aics_chr_info[vcp_srvc_inst].audio_in_desc_len != vcp_event_info->datalen) ||
                (0U != GA_mem_cmp
                       (
                           &aics_chr_info[vcp_srvc_inst].audio_in_desc[0U],
                           &vcp_event_info->data[0U],
                           vcp_event_info->datalen
                       )))
            {
                if (vcp_event_info->datalen > APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN)
                {
                    aics_chr_info[vcp_srvc_inst].audio_in_desc_len =
                        APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN;
                }
                else
                {
                    aics_chr_info[vcp_srvc_inst].audio_in_desc_len =
                        vcp_event_info->datalen;
                }

                GA_mem_copy
                (
                    &aics_chr_info[vcp_srvc_inst].audio_in_desc[0U],
                    &vcp_event_info->data[0U],
                    aics_chr_info[vcp_srvc_inst].audio_in_desc_len
                );

                /*
                 * Updating the flag to notify the updated
                 * audio input description value
                 */
                aics_char_val_change_flag = 1U;
            }
            else
            {
                /* No action required */
            }

            if (0U != aics_char_val_change_flag)
            {
                /*
                 * Notification should be send to the device from which server has
                 * received the CP write request.
                 */
                /* Copy device info */
                GA_COPY_BRR_DEVICE(&vocs_aics_ntf_info.device, vcp_event_info->device);
                /* Copy service instance */
                vocs_aics_ntf_info.srvc_inst = vcp_srvc_inst;

                BT_start_timer
                (
                    &appl_aics_set_audio_inp_desc_timer_handle,
                    1U,
                    appl_vcp_aics_rd_set_audio_inp_desc_timer_handler,
                    &vocs_aics_ntf_info,
                    sizeof(vocs_aics_ntf_info)
                );
            }
        }
    }
    break;

    case GA_VC_RD_IND:
    {
        APPL_TRC("[Event]       : GA_VC_RD_IND (0x%02X)\n", ga_event);

        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        appl_ga_utils_display_char_uuid(vcp_event_info->chr->uuid);
        GA_FORMAT_DISPLAY();

        retval = appl_vcp_rd_pack_vcs_chr_value
                 (
                     vcp_event_info->chr->uuid,
                     &appl_vcp_rndr_vcs_vol_val,
                     &appl_vcp_rndr_chr_data[0U],
                     &appl_vcp_rndr_chr_datalen
                 );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        appl_vcp_parse_and_display_vcs_char_value
        (
            vcp_event_info->chr->uuid,
            &appl_vcp_rndr_chr_data[0U]
        );

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        rsp_event =  GA_RSP_TYPE_RD;
        device = vcp_event_info->device;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = &appl_vcp_rndr_chr_data[0U];
        vcp_rsp_info.datalen = appl_vcp_rndr_chr_datalen;
    }
    break;

    case GA_VC_VOCS_RD_IND:
    {
        APPL_TRC("[Event]       : GA_VC_VOCS_RD_IND (0x%02X)\n", ga_event);

        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        appl_ga_utils_display_char_uuid(vcp_event_info->chr->uuid);
        GA_FORMAT_DISPLAY();

        if (VCP_MAX_VOCS_ENTITIES > vcp_event_info->srvc_inst)
        {
            retval = appl_vcp_rd_pack_vocs_chr_value
                     (
                         vcp_event_info->chr->uuid,
                         &appl_vc_rndr_vocs_char_info[vcp_event_info->srvc_inst],
                         &appl_vcp_rndr_chr_data[0U],
                         &appl_vcp_rndr_chr_datalen
                     );
        }
        else
        {
            appl_vcp_rndr_chr_datalen = 0U;
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");
        appl_vcp_parse_and_display_vocs_char_value
        (
            vcp_event_info->chr->uuid,
            &appl_vcp_rndr_chr_data[0U],
            appl_vcp_rndr_chr_datalen
        );

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        rsp_event = GA_RSP_TYPE_RD;
        device = vcp_event_info->device;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = &appl_vcp_rndr_chr_data[0U];
        vcp_rsp_info.datalen = appl_vcp_rndr_chr_datalen;
    }
    break;

    case GA_VC_VOCS_SET_AUDIO_LOCATION_IND:
    {
        APPL_TRC
        (
            "[Event]       : GA_VC_VOCS_SET_AUDIO_LOCATION_IND (0x%02X)\n",
            ga_event
        );
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Location (0x%04X)\n",
            GA_CHAR_VOCS_AUDIO_LOCATION
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");

        vcp_srvc_inst = vcp_event_info->srvc_inst;
        if (VCP_MAX_VOCS_ENTITIES > vcp_srvc_inst)
        {
            if (4U == vcp_event_info->datalen)
            {
                UINT32 audio_loc = 0U;

                GA_UNPACK_LE_4_BYTE(&audio_loc, &vcp_event_info->data[0U]);

                if (0xFFFFFFFFU != audio_loc)
                {
                    /*
                     * Check if the received value is same as the current
                     * Audio Location value.
                     * If Yes, no action required.
                     * If No, update the field with received value and update
                     *        value change flag.
                     */
                    if (appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_loc != audio_loc)
                    {
                        appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_loc =
                            audio_loc;
                        /*
                         * Updating the flag to notify the updated
                         * audio location value
                         */
                        vocs_char_val_change_flag = 1U;
                    }
                    else
                    {
                        /* No action required */
                    }

                    APPL_TRC
                    (
                        "Len: 0x%02X,  Audio Location: ",
                        (UCHAR)sizeof(appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_loc)
                    );
                    appl_ga_utils_display_audio_loc
                    (
                        appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_loc
                    );

                    if (0U != vocs_char_val_change_flag)
                    {
                        /*
                         * Notification should be send to the device from which server has
                         * received the CP write request.
                         */
                        /* Copy device info */
                        GA_COPY_BRR_DEVICE(&vocs_aics_ntf_info.device, vcp_event_info->device);
                        /* Copy service instance */
                        vocs_aics_ntf_info.srvc_inst = vcp_srvc_inst;

                        BT_start_timer
                        (
                            &appl_vocs_set_audio_loc_timer_handle,
                            1U,
                            appl_vcp_vocs_rd_set_audio_loc_timer_handler,
                            &vocs_aics_ntf_info,
                            sizeof(vocs_aics_ntf_info)
                        );
                    }
                }
                else
                {
                    APPL_ERR("Invalid Audio Location, Ignoring.. !\n");
                }
            }
            else
            {
                APPL_ERR("Invalid Audio Location value size !\n");
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }
    }
    break;

    case GA_VC_VOCS_SET_AUDIO_OUT_DESC_IND:
    {
        APPL_TRC
        (
            "[Event]       : GA_VC_VOCS_SET_AUDIO_OUT_DESC_IND (0x%02X)\n",
            ga_event
        );
        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Audio Output Description (0x%04X)\n",
            GA_CHAR_VOCS_AUDIO_OUTPUT_DESC
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");

        vcp_srvc_inst = vcp_event_info->srvc_inst;
        if (VCP_MAX_VOCS_ENTITIES > vcp_srvc_inst)
        {
            /*
             * Check if the received value length is same as the current audio
             * output description value length.
             * If No, update it with received value and update
             *         value change flag.
             * If Yes, compare the received value with the current value.
             *         If the value is different, then update it with received
             *         value and update value change flag, else no action.
             */
            if ((appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc_len !=
                     vcp_event_info->datalen) ||
                (0U != GA_mem_cmp
                       (
                           &appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc[0U],
                           &vcp_event_info->data[0U],
                           vcp_event_info->datalen
                       )))
            {
                if (APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN < vcp_event_info->datalen)
                {
                    appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc_len = \
                        APPL_VCP_MAX_VOCS_CHAR_AUDIO_OUTPUT_DESC_LEN;
                }
                else
                {
                    appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc_len = \
                        (UCHAR)vcp_event_info->datalen;
                }

                GA_mem_copy
                (
                    &appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc[0U],
                    &vcp_event_info->data[0U],
                    appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc_len
                );

                /*
                 * Updating the flag to notify the updated
                 * audio location value
                 */
                vocs_char_val_change_flag = 1U;
            }
            else
            {
                /* No action required */
            }

            APPL_TRC
            (
                "Len: 0x%02X,  Audio Output Description: ",
                vcp_event_info->datalen
            );
            for (i = 0U; i < appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc_len; i++)
            {
                APPL_TRC("%c", appl_vc_rndr_vocs_char_info[vcp_srvc_inst].audio_out_desc[i]);
            }
            APPL_TRC("\n");

            if (0U != vocs_char_val_change_flag)
            {
                /*
                 * Notification should be send to the device from which server has
                 * received the CP write request.
                 */
                /* Copy device info */
                GA_COPY_BRR_DEVICE(&vocs_aics_ntf_info.device, vcp_event_info->device);
                /* Copy service instance */
                vocs_aics_ntf_info.srvc_inst = vcp_srvc_inst;

                BT_start_timer
                (
                    &appl_vocs_set_audio_out_desc_timer_handle,
                    1U,
                    appl_vcp_vocs_rd_set_audio_out_desc_timer_handler,
                    &vocs_aics_ntf_info,
                    sizeof(vocs_aics_ntf_info)
                );
            }
        }
        else
        {
            /* Should not reach here */
            APPL_ERR("Invalid Service Instance !\n");
        }
    }
    break;

    case GA_VC_AICS_RD_IND:
    {
        APPL_TRC("[Event]       : GA_VC_AICS_RD_IND (0x%02X)\n", ga_event);

        vcp_event_info = (VCP_EVENT_INFO *)ga_data;
        APPL_TRC("[Service Inst]: 0x%02X\n", vcp_event_info->srvc_inst);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        appl_ga_utils_display_char_uuid(vcp_event_info->chr->uuid);
        GA_FORMAT_DISPLAY();

        vcp_event_info->data = &appl_vcp_rndr_chr_data[0U];
        vcp_event_info->datalen = 0U;

        if (vcp_event_info->srvc_inst < VCP_MAX_AICS_ENTITIES)
        {
            retval = appl_vcp_rd_pack_aics_chr_value
                     (
                         vcp_event_info->chr->uuid,
                         &aics_chr_info[vcp_event_info->srvc_inst],
                         &appl_vcp_rndr_chr_data[0U],
                         &appl_vcp_rndr_chr_datalen
                     );
        }
        else
        {
            appl_vcp_rndr_chr_datalen = 0U;
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", vcp_event_info->datalen);
        APPL_TRC("Data:\n");

        appl_aics_parse_and_display_char_value
        (
            vcp_event_info->chr->uuid,
            &appl_vcp_rndr_chr_data[0U],
            appl_vcp_rndr_chr_datalen
        );

        /* Fill the response Info. */
        send_vcp_rsp_flag = 1U;
        device = vcp_event_info->device;
        rsp_event = GA_RSP_TYPE_RD;
        vcp_rsp_info.chr = vcp_event_info->chr;
        vcp_rsp_info.data = &appl_vcp_rndr_chr_data[0U];
        vcp_rsp_info.datalen = appl_vcp_rndr_chr_datalen;
    }
    break;

    default:
        APPL_ERR("UnHandled Event: (0x%02X) !\n", ga_event);
        break;
    }

    if (0U != send_vcp_rsp_flag)
    {
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP RESPONSE\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_send_rsp\n");
        retval = GA_vc_send_rsp
                 (
                     device,
                     rsp_event,
                     rsp_status,
                     &vcp_rsp_info,
                     sizeof(VCP_RSP_INF)
                 );
        APPL_TRC("Retval - ");
        appl_vcp_display_ga_result
        (
            retval,
            ((VCP_EVENT_INFO *)ga_data)->srvc_type
        );
    }

    return retval;
}

GA_RESULT appl_vcp_rd_vcs_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          )
{
    GA_RESULT   retval;
    UCHAR       cp_op_code;
    UCHAR     * data;
    UCHAR       datalen;
    UCHAR       vcs_change_counter_flag;
    UCHAR       vcs_cp_param_len;

    APPL_VCP_NTF_INFO vcs_ntf_info;

    if ((NULL == vcp_event_info) ||
        (NULL == vcp_event_info->device) ||
        (NULL == vcp_event_info->data) ||
        (0U == vcp_event_info->datalen))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    if (0x00U != vcp_event_info->srvc_inst)
    {
        APPL_ERR("Invalid VCS Service Instance !\n");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    data = vcp_event_info->data;
    datalen = vcp_event_info->datalen;

    appl_ntf_volume_flag = 0U;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    cp_op_code = data[0U];

    if (VCS_CP_OP_MUTE < cp_op_code)
    {
        APPL_ERR("\n==== Invalid CP OPCODE ======== !\n");
        return VCS_ERR_CODE_OPCODE_NOT_SUPPORTED;
    }
    else if (data[1U] != appl_vcp_rndr_vcs_vol_val.vol_state.change_counter)
    {
        APPL_ERR("\n==== Invalid CHANGE COUNTER ======== !\n");
        return VCS_ERR_CODE_INVALID_CHNG_CNTR;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    data += 1U;
    datalen -= 1U;
    vcs_change_counter_flag = 0U;
    vcs_cp_param_len = 1U;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    vcs_change_counter_flag = 0U;
    APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(cp_op_code));
    if (VCS_CP_OP_RELATIVE_VOL_DOWN == cp_op_code)
    {
        APPL_TRC("Relative Volume Down (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
             vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        /* Update the Volume setting */
        if (APPL_VCP_RD_VOL_STEP_SIZE <= appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting -=
                APPL_VCP_RD_VOL_STEP_SIZE;
            vcs_change_counter_flag = 1U;
        }
        else if (0U != appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting =
                VCS_VOLUME_SETTING_MIN;
            vcs_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (VCS_CP_OP_RELATIVE_VOL_UP == cp_op_code)
    {
        APPL_TRC("Relative Volume Up (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        if ((VCS_VOLUME_SETTING_MAX - APPL_VCP_RD_VOL_STEP_SIZE) >= (appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting))
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting +=
                APPL_VCP_RD_VOL_STEP_SIZE;
            vcs_change_counter_flag = 1U;
        }
        else if (VCS_VOLUME_SETTING_MAX != appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting =
                VCS_VOLUME_SETTING_MAX;
            vcs_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((1U == vcs_change_counter_flag) &&
            (0U == appl_vcp_rndr_vcs_vol_val.volume_flags))
        {
            appl_vcp_rndr_vcs_vol_val.volume_flags = 0x01U;
            appl_ntf_volume_flag = 1U;
        }
    }
    else if (VCS_CP_OP_UNMUTE_RELATIVE_VOL_DOWN == cp_op_code)
    {
        APPL_TRC("Unmute/Relative Volume Down (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        if (VCS_MUTE_YES == appl_vcp_rndr_vcs_vol_val.vol_state.mute)
        {
            vcs_change_counter_flag = 1U;
            appl_vcp_rndr_vcs_vol_val.vol_state.mute = VCS_MUTE_NO;
        }

        /* Update the Volume setting */
        if (APPL_VCP_RD_VOL_STEP_SIZE <= appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting -=
                APPL_VCP_RD_VOL_STEP_SIZE;
            vcs_change_counter_flag = 1U;
        }
        else if (VCS_VOLUME_SETTING_MIN >= appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting = \
                VCS_VOLUME_SETTING_MIN;
            vcs_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (VCS_CP_OP_UNMUTE_RELATIVE_VOL_UP == cp_op_code)
    {
        APPL_TRC("Unmute/Relative Volume Up (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        if (VCS_MUTE_YES == appl_vcp_rndr_vcs_vol_val.vol_state.mute)
        {
            vcs_change_counter_flag = 1U;
            appl_vcp_rndr_vcs_vol_val.vol_state.mute = VCS_MUTE_NO;
        }

        if ((VCS_VOLUME_SETTING_MAX - APPL_VCP_RD_VOL_STEP_SIZE) >= (appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting))
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting +=
                APPL_VCP_RD_VOL_STEP_SIZE;
            vcs_change_counter_flag = 1U;
        }
        else if (VCS_VOLUME_SETTING_MAX != appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting)
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting =
                VCS_VOLUME_SETTING_MAX;
            vcs_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (VCS_CP_OP_SET_ABSOLUE_VOL == cp_op_code)
    {
        APPL_TRC("Set Absolute Volume (0x%02X)\n", cp_op_code);

        if (2U != datalen)
        {
            /* TODO */
            return GA_FAILURE;
        }

        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Volume_Setting: 0x%02X\n",
            (UCHAR)sizeof(appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting),
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n", data[1U]);

        if (appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting != data[1U])
        {
            appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting = data[1U];
            vcs_change_counter_flag = 1U;
        }
    }
    else if (VCS_CP_OP_UNMUTE == cp_op_code)
    {
        APPL_TRC("Unmute (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        if (VCS_MUTE_YES == appl_vcp_rndr_vcs_vol_val.vol_state.mute)
        {
            vcs_change_counter_flag = 1U;
            appl_vcp_rndr_vcs_vol_val.vol_state.mute = VCS_MUTE_NO;
        }
    }
    else if (VCS_CP_OP_MUTE == cp_op_code)
    {
        APPL_TRC("Mute (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vcs_cp_param_len, data[0U]
        );
        APPL_TRC("\n");
        APPL_TRC("Volume Setting: 0x%02X\n",
        appl_vcp_rndr_vcs_vol_val.vol_state.volume_setting);

        if (VCS_MUTE_NO == appl_vcp_rndr_vcs_vol_val.vol_state.mute)
        {
            vcs_change_counter_flag = 1U;
            appl_vcp_rndr_vcs_vol_val.vol_state.mute = VCS_MUTE_YES;
        }
    }
    else
    {
        retval = GA_FAILURE;

        /* TODO */
        APPL_ERR("??? !\n");
    }

    if (0U != vcs_change_counter_flag)
    {
        appl_vcp_rndr_vcs_vol_val.vol_state.change_counter += 1U;

        if ((VCS_CP_OP_UNMUTE > cp_op_code) &&
            (1U == vcs_change_counter_flag) &&
            (0U == appl_vcp_rndr_vcs_vol_val.volume_flags))
        {
            /* Send Volume Flag Notification */
            appl_vcp_rndr_vcs_vol_val.volume_flags = 0x01U;
            appl_ntf_volume_flag = 1U;
        }

        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&vcs_ntf_info.device, vcp_event_info->device);

        BT_start_timer
        (
            &appl_vcs_cp_wt_timer_handle,
            1U,
            appl_vcp_rd_cp_write_timer_handler,
            &vcs_ntf_info,
            sizeof(vcs_ntf_info)
        );
    }

    return retval;
}

GA_RESULT appl_vcp_rd_vocs_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          )
{
    GA_RESULT   retval;
    UCHAR       cp_op_code;
    UCHAR     * data;
    UCHAR       datalen;
    INT16       volume_offset;
    UCHAR       vocs_cp_param_len;
    UCHAR       srvc_inst;

    APPL_VCP_NTF_INFO vocs_ntf_info;

    if ((NULL == vcp_event_info) ||
        (NULL == vcp_event_info->device) ||
        (NULL == vcp_event_info->data) ||
        (0U == vcp_event_info->datalen))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    if (VCP_MAX_VOCS_ENTITIES <= vcp_event_info->srvc_inst)
    {
        APPL_ERR("Invalid VOCS Service Instance !\n");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    data = vcp_event_info->data;
    datalen = vcp_event_info->datalen;
    srvc_inst = vcp_event_info->srvc_inst;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    cp_op_code = data[0U];

    if (VOCS_CP_OP_SET_VOLUME_OFFSET != cp_op_code)
    {
        APPL_ERR("\n==== Invalid CP OPCODE:0x%02x ======== !\n", cp_op_code);
        return VOCS_ERR_CODE_OPCODE_NOT_SUPPORTED;
    }
    else if (data[1U] != appl_vc_rndr_vocs_char_info[srvc_inst].offset_state->change_counter)
    {
        APPL_ERR("\n==== Invalid CHANGE COUNTER:0x%02x ======== !\n", data[1U]);
        return VOCS_ERR_CODE_INVALID_CHNG_CNTR;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    data += 1U;
    datalen -= 1U;
    vocs_cp_param_len = 1U;

    if (3U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    volume_offset = 0U;

    APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(cp_op_code));
    if (VOCS_CP_OP_SET_VOLUME_OFFSET == cp_op_code)
    {
        APPL_TRC("Set Volume Offset (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            vocs_cp_param_len, data[0U]
        );
        GA_UNPACK_LE_2_BYTE(&volume_offset, &data[1U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Volume_Offset: 0x%04X (%d)\n",
            (UCHAR)sizeof(volume_offset),
            volume_offset,
            volume_offset
        );

        if ((255 < volume_offset) || (-255 > volume_offset))
        {
            APPL_ERR("Invalid Volume Offset:%4d !\n", volume_offset);
            return VOCS_ERR_CODE_VALUE_OUT_OF_RANGE;
        }

        if (appl_vc_rndr_vocs_char_info[srvc_inst].offset_state->vol_offset != volume_offset)
        {
            appl_vc_rndr_vocs_char_info[srvc_inst].offset_state->vol_offset =
                volume_offset;
            appl_vc_rndr_vocs_char_info[srvc_inst].offset_state->change_counter += 1U;

            /*
             * Notification should be send to the device from which server has
             * received the CP write request.
             */
            /* Copy device info */
            GA_COPY_BRR_DEVICE(&vocs_ntf_info.device, vcp_event_info->device);
            /* Copy service instance */
            vocs_ntf_info.srvc_inst = srvc_inst;

            BT_start_timer
            (
                &appl_vocs_cp_wt_timer_handle,
                1U,
                appl_vcp_vocs_rd_cp_write_timer_handler,
                &vocs_ntf_info,
                sizeof(vocs_ntf_info)
            );

#if 0
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("SENDING GA VCP NOTIFICATION\n");
                GA_FORMAT_DISPLAY();
                BT_COPY_BD_ADDR(appl_vcp_peer_rd_dev.bd_addr, g_pts_bd_addr);
                appl_vcp_peer_rd_dev.bd_type = BT_BD_PUBLIC_ADDRESS_TYPE;

                APPL_TRC("GA_vc_notify_vocs_volume_offset\n");
                retval = GA_vc_notify_vocs_volume_offset
                         (
                             &appl_vcp_peer_rd_dev,
                             0x00,
                             &appl_vc_rndr_vocs_char_info,
                             sizeof(appl_vc_rndr_vocs_char_info)
                         );
                APPL_TRC("Retval - ");
                appl_vcp_display_ga_result(retval, VCP_SERVICE_TYPE_VOCS);
            }
#endif /* 0 */
        }
    }
    else
    {
        retval = GA_FAILURE;

        /* TODO */
        APPL_ERR("??? !\n");
    }

    return retval;

}

GA_RESULT appl_vcp_rd_aics_cp_write_handler
          (
              VCP_EVENT_INFO * vcp_event_info
          )
{
    GA_RESULT   retval;
    UCHAR       cp_op_code;
    UCHAR     * data;
    UCHAR       datalen;
    UCHAR       aics_change_counter_flag;
    UCHAR       aics_cp_param_len;
    UCHAR       srvc_inst;

    APPL_VCP_NTF_INFO aics_ntf_info;

    if ((NULL == vcp_event_info) ||
        (NULL == vcp_event_info->device) ||
        (NULL == vcp_event_info->data) ||
        (0U == vcp_event_info->datalen))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    if (VCP_MAX_AICS_ENTITIES <= vcp_event_info->srvc_inst)
    {
        APPL_ERR("Invalid AICS Service Instance !\n");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    data = vcp_event_info->data;
    datalen = vcp_event_info->datalen;
    srvc_inst = vcp_event_info->srvc_inst;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    cp_op_code = data[0U];

    if ((AICS_CP_OP_SET_AUTO_GAIN_MODE < cp_op_code) ||
        (0U == cp_op_code))
    {
        APPL_ERR("\n==== Invalid CP OPCODE ======== !\n");
        return AICS_ERR_CODE_OPCODE_NOT_SUPPORTED;
    }

    data += 1U;
    datalen -= 1U;
    aics_cp_param_len = 1U;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return GA_FAILURE;
    }

    if (data[0U] != aics_chr_info[srvc_inst].input_state->change_counter)
    {
        APPL_ERR("\n==== Invalid CHANGE COUNTER ======== !\n");
        APPL_TRC("Rx. Change_cnt: 0x%02x, IUT Change Counter:0x%02x\n",
            data[0U], aics_chr_info[srvc_inst].input_state->change_counter);
        return AICS_ERR_CODE_INVALID_CHNG_CNTR;
    }

    aics_change_counter_flag = 0U;
    APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(cp_op_code));
    if (AICS_CP_OP_SET_GAIN_SETTING == cp_op_code)
    {
        APPL_TRC("Set Gain Setting (0x%02X)\n", cp_op_code);

        if (0x02U != datalen)
        {
            APPL_ERR("Invalid CP Write Length !\n");
            return GA_FAILURE;
        }
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            aics_cp_param_len, data[0U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Gain_Setting: 0x%02X\n",
            aics_cp_param_len, data[1U]
        );

        if (((CHAR)data[1U] < aics_chr_info[srvc_inst].gain_setting->gain_settting_min) ||
            ((CHAR)data[1U] > aics_chr_info[srvc_inst].gain_setting->gain_setting_max))
        {
            return AICS_ERR_CODE_VALUE_OUT_OF_RANGE;
        }
        else if (aics_chr_info[srvc_inst].input_state->gain_setting != (CHAR)data[1U])
        {
            aics_chr_info[srvc_inst].input_state->gain_setting = (CHAR)data[1U];
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_UNMUTE == cp_op_code)
    {
        APPL_TRC("Unmute (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            aics_cp_param_len, data[0U]
        );

        if (AICS_DISABLED == aics_chr_info[srvc_inst].input_state->mute)
        {
            return AICS_ERR_CODE_MUTE_DISABLED;
        }
        else if (AICS_MUTED == aics_chr_info[srvc_inst].input_state->mute)
        {
            aics_chr_info[srvc_inst].input_state->mute = AICS_NOT_MUTED;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_MUTE == cp_op_code)
    {
        APPL_TRC("Mute (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            aics_cp_param_len, data[0U]
        );

        if (AICS_DISABLED == aics_chr_info[srvc_inst].input_state->mute)
        {
            return AICS_ERR_CODE_MUTE_DISABLED;
        }
        else if (AICS_NOT_MUTED == aics_chr_info[srvc_inst].input_state->mute)
        {
            aics_chr_info[srvc_inst].input_state->mute = AICS_MUTED;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_SET_MANUAL_GAIN_MODE == cp_op_code)
    {
        APPL_TRC("Set Manual Gain Mode (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if ((AICS_GM_MANUAL_ONLY == aics_chr_info[srvc_inst].input_state->gain_mode) ||
            (AICS_GM_AUTOMATIC_ONLY == aics_chr_info[srvc_inst].input_state->gain_mode))
        {
            return AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED;
        }
        else if (AICS_GM_AUTO == aics_chr_info[srvc_inst].input_state->gain_mode)
        {
            aics_chr_info[0U].input_state->gain_mode = AICS_GM_MANUAL;
            aics_change_counter_flag = 1U;
        }
        else
        {
          /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_SET_AUTO_GAIN_MODE == cp_op_code)
    {
        APPL_TRC("Set Automatic Gain Mode (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if ((AICS_GM_MANUAL_ONLY == aics_chr_info[srvc_inst].input_state->gain_mode) ||
            (AICS_GM_AUTOMATIC_ONLY == aics_chr_info[srvc_inst].input_state->gain_mode))
        {
            return AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED;
        }
        else if (AICS_GM_MANUAL == aics_chr_info[srvc_inst].input_state->gain_mode)
        {
            aics_chr_info[0U].input_state->gain_mode = AICS_GM_AUTO;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else
    {
        retval = GA_FAILURE;

        /* TODO */
        APPL_ERR("??? !\n");
    }

    if (0U != aics_change_counter_flag)
    {
        /*
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA VCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_vc_notify_aics_audio_input_state\n");
        */

        /* Incrementing the change counter */
        aics_chr_info[0U].input_state->change_counter += 1U;

        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&aics_ntf_info.device, vcp_event_info->device);
        /* Copy service instance */
        aics_ntf_info.srvc_inst = srvc_inst;

        BT_start_timer
        (
            &appl_aics_cp_wt_timer_handle,
            1U,
            appl_vcp_aics_rd_cp_write_timer_handler,
            &aics_ntf_info,
            sizeof (aics_ntf_info)
        );
    }

    return retval;
}

GA_RESULT appl_vcp_rd_pack_vcs_chr_value
          (
              /* IN  */  UINT16                   chr_uuid,
              /* IN  */  APPL_VCP_VCS_CHAR_INFO * vcs_params,
              /* OUT */  UCHAR                  * chr_val,
              /* OUT */  UCHAR                  * chr_val_len
          )
{
    GA_RESULT  retval;
    UCHAR      offset;
    UCHAR    * pdu;

    /* Param Check */
    if ((NULL == chr_val) ||
        (NULL == chr_val_len) ||
        (NULL == vcs_params))
    {
        APPL_ERR("Invalid Parameters !\n ");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    offset = 0U;
    pdu = chr_val;
    offset = 0U;

    switch (chr_uuid)
    {
    case GA_CHAR_VCS_VOLUME_STATE:

        pdu[offset] = vcs_params->vol_state.volume_setting;
        offset++;

        pdu[offset] = vcs_params->vol_state.mute;
        offset++;

        pdu[offset] = vcs_params->vol_state.change_counter;
        offset++;
        break;

    case GA_CHAR_VCS_VOLUME_FLAGS:

        pdu[offset] = vcs_params->volume_flags;
        offset++;

        break;

     default:
        /* Should not reach here */
        APPL_ERR("Invalid char UUID: 0x%04X !\n", chr_uuid);
        retval = GA_FAILURE;
        break;
    }

    *chr_val_len = offset;

    return retval;
}

GA_RESULT appl_vcp_rd_pack_vocs_chr_value
          (
              /* IN  */  UINT16                    chr_uuid,
              /* IN  */  APPL_VCP_VOCS_CHAR_INFO * vocs_params,
              /* OUT */  UCHAR                   * chr_val,
              /* OUT */  UCHAR                   * chr_val_len
          )
{
    GA_RESULT  retval;
    UCHAR      offset;

    UCHAR    * pdu;

    /* Param Check */
    if ((NULL == chr_val) ||
        (NULL == chr_val_len) ||
        (NULL == vocs_params))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    /* Init */
    retval = GA_SUCCESS;
    offset = 0U;
    pdu = chr_val;
    offset = 0U;

    switch (chr_uuid)
    {
    case GA_CHAR_VOCS_OFFSET_STATE:
        GA_PACK_LE_2_BYTE_VAL
        (
            &pdu[offset],
            vocs_params->offset_state->vol_offset
        );
        offset += 2U;

        pdu[offset] = vocs_params->offset_state->change_counter;
        offset++;
        break;

    case GA_CHAR_VOCS_AUDIO_LOCATION:
        GA_PACK_LE_4_BYTE_VAL(&pdu[offset], vocs_params->audio_loc);
        offset += 4U;
        break;

    case GA_CHAR_VOCS_AUDIO_OUTPUT_DESC:

        GA_mem_copy
        (
            &pdu[offset],
            vocs_params->audio_out_desc,
            vocs_params->audio_out_desc_len
        );
        offset += vocs_params->audio_out_desc_len;
        break;

     default:
        /* Should not reach here */
        APPL_ERR("Invalid char UUID: 0x%04X !\n", chr_uuid);
        retval = GA_FAILURE;
        break;
    }

    *chr_val_len = offset;

    return retval;
}

GA_RESULT appl_vcp_rd_pack_aics_chr_value
          (
              /* IN  */  UINT16                chr_uuid,
              /* IN  */  APPL_AICS_CHAR_INFO * aics_params,
              /* OUT */  UCHAR               * chr_val,
              /* OUT */  UCHAR               * chr_val_len
          )
{
    GA_RESULT  retval;
    UCHAR      offset;

    UCHAR    * pdu;

    /* APPL_TRC(">> appl_vcp_rd_pack_aics_chr_value\n "); */
    /* Param Check */
    if ((NULL == chr_val) ||
        (NULL == chr_val_len) ||
        (NULL == aics_params))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    /* Init */
    retval = GA_SUCCESS;
    offset = 0U;
    pdu = chr_val;
    offset = 0U;

    /* APPL_TRC("Char UUID:0x%04x\n", chr_uuid); */
    switch (chr_uuid)
    {
    case GA_CHAR_AICS_INPUT_STATE:
        /* APPL_TRC("GA_CHAR_AICS_INPUT_STATE\n"); */
        pdu[offset] = aics_params->input_state->gain_setting;
        offset += 1U;

        pdu[offset] = aics_params->input_state->mute;
        offset += 1U;

        pdu[offset] = aics_params->input_state->gain_mode;
        offset += 1U;

        pdu[offset] = aics_params->input_state->change_counter;

        /*
        APPL_TRC("Change_Counter: 0x%02x\n",
        aics_params->input_state->change_counter);
        */
        offset += 1U;
        break;

    case GA_CHAR_AICS_GAIN_SETTING:
        /* APPL_TRC("GA_CHAR_AICS_GAIN_SETTING\n"); */
        pdu[offset] = aics_params->gain_setting->gain_setting_units;
        offset += 1U;

        pdu[offset] = aics_params->gain_setting->gain_settting_min;
        offset += 1U;

        pdu[offset] = aics_params->gain_setting->gain_setting_max;
        offset += 1U;
        break;

    case GA_CHAR_AICS_INPUT_TYPE:
        /* APPL_TRC("GA_CHAR_AICS_INPUT_TYPE\n"); */
        pdu[offset] = aics_params->input_type;
        offset += 1U;
        break;

    case GA_CHAR_AICS_INPUT_STATUS:

        /* APPL_TRC("GA_CHAR_AICS_INPUT_STATUS\n"); */
        pdu[offset] = aics_params->input_status;
        offset += 1U;
        break;

    case GA_CHAR_AICS_AUDIO_INPUT_DESC:
        /* APPL_TRC("GA_CHAR_AICS_AUDIO_INPUT_DESC\n"); */
        GA_mem_copy
        (
            &pdu[offset],
            aics_params->audio_in_desc,
            aics_params->audio_in_desc_len
        );
        offset += aics_params->audio_in_desc_len;
        break;

    default:
        APPL_ERR("Invalid char UUID: 0x%04X !\n", chr_uuid);
        /* Should not reach here */
        retval = GA_FAILURE;
        break;
    }

    /* APPL_TRC("<< appl_vcp_rd_pack_aics_chr_value\n "); */

    (*chr_val_len) = offset;

    return retval;
}
#endif /* VCP_RENDERER */

/*** Common APIs - VCP Controller and Renderer ***/

/* VCP Server Callback */
GA_RESULT appl_vcp_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT  retval;

    /* Initialize */
    retval = GA_SUCCESS;

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("VCP Server");

    switch(ga_event)
    {
        case GA_VC_CP_WT_IND:
            APPL_TRC("[Profile]     : VCS (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS RENDERER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOCS_SET_AUDIO_LOCATION_IND: /* Fall - through */
        case GA_VC_VOCS_SET_AUDIO_OUT_DESC_IND: /* Fall - through */
        case GA_VC_VOCS_CP_WT_IND:
            APPL_TRC("[Profile]     : VCS - VOCS (0x%04X)\n", GA_SERVICE_VOCS);
            APPL_TRC("[SECTION]     : VCS - VOCS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS - VOCS RENDERER-WRITE\n");
            if (GA_VC_VOCS_CP_WT_IND == ga_event)
            {
                APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            }
            else
            {
                APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            }
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_AICS_CP_WT_IND: /* Fall - through */
        case GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND:
            APPL_TRC("[Profile]     : VCS - AICS (0x%04X)\n", GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : VCS - AICS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS - AICS RENDERER-WRITE\n");
            if (GA_VC_AICS_SET_AUDIO_INPUT_DESC_IND == ga_event)
            {
                APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            }
            else
            {
                APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            }
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_RD_IND:
            APPL_TRC("[Profile]     : VCS (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS RENDERER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOCS_RD_IND:
            APPL_TRC("[Profile]     : VCS - VOCS (0x%04X)\n", GA_SERVICE_VOCS);
            APPL_TRC("[SECTION]     : VCS - VOCS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS - VOCS RENDERER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_AICS_RD_IND:
            APPL_TRC("[Profile]     : VCS - AICS (0x%04X)\n", GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : VCS - AICS RENDERER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCS - AICS RENDERER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_vcp_rd_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        default:
            APPL_ERR("UnHandled VCP Event: 0x%02X !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

/* VCP Client Callback */
GA_RESULT appl_vcp_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT          retval;
    GA_BRR_SVC_INFO  * svc;

    /* Initialize */
    retval = GA_SUCCESS;
    svc = NULL;

    if (NULL == ga_device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("VCP Client");

    switch(ga_event)
    {
        case GA_VC_SETUP_CNF:
            APPL_TRC("[Profile]     : VCP (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP CONTROLLER-SETUP\n");
            APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_GET_VOLUME_STATE_CNF: /* Fall - through */
        case GA_VC_GET_VOLUME_FLAGS_CNF:
            APPL_TRC("[Profile]     : VCP (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP CONTROLLER-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOLUME_STATE_NTF: /* Fall - through */
        case GA_VC_VOLUME_FLAGS_NTF:
            APPL_TRC("[Profile]     : VCP (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP CONTROLLER-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_CP_WT_CNF:
            APPL_TRC("[Profile]     : VCP (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP CONTROLLER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_GET_CAPABILITIES_CNF:
            {
                svc = (GA_BRR_SVC_INFO *)(((VCP_EVENT_INFO *)ga_data)->data);
                if ((NULL != svc) && (GA_SERVICE_VOCS == svc->uuid))
                {
                    APPL_TRC
                    (
                        "[Profile]     : VCP - VOCS (0x%04X)\n",
                        GA_SERVICE_VOCS
                    );
                    APPL_TRC("[SECTION]     : VCP - VOCS CONTROLLER EVENTS\n");
                    APPL_TRC("[SUB-SECTION] : VCP - VOCS CONTROLLER-SETUP\n");
                }
                else if ((NULL != svc) && (GA_SERVICE_AICS == svc->uuid))
                {
                    APPL_TRC
                    (
                        "[Profile]     : VCP - AICS (0x%04X)\n",
                        GA_SERVICE_AICS
                    );
                    APPL_TRC("[SECTION]     : VCP - AICS CONTROLLER EVENTS\n");
                    APPL_TRC("[SUB-SECTION] : VCP - AICS CONTROLLER-SETUP\n");
                }
                else
                {
                    APPL_TRC
                    (
                        "[Profile]     : VCP - VOCS (0x%04X) / AICS (0x%04X)\n",
                        GA_SERVICE_VOCS,
                        GA_SERVICE_AICS
                    );
                    APPL_TRC
                    (
                        "[SECTION]     : VCP - VOCS / AICS CONTROLLER EVENTS\n"
                    );
                    APPL_TRC
                    (
                        "[SUB-SECTION] : VCP - VOCS / AICS CONTROLLER-SETUP\n"
                    );
                }
                APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
                appl_ga_utils_display_bdaddr_and_type(ga_device);
                retval = appl_vcp_ct_callback
                         (
                             ga_event,
                             ga_status,
                             ga_data,
                             ga_datalen
                         );
            }
            break;

        case GA_VC_SET_CAPABILITY_CNF:
            APPL_TRC
            (
                "[Profile]     : VCP - VOCS (0x%04X) / AICS (0x%04X)\n",
                GA_SERVICE_VOCS,
                GA_SERVICE_AICS
            );
            APPL_TRC("[SECTION]     : VCP - VOCS / AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - VOCS / AICS CONTROLLER-SETUP\n");
            APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOCS_GET_OFFSET_STATE_CNF: /* Fall - through */
        case GA_VC_VOCS_GET_AUDIO_LOCATION_CNF: /* Fall - through */
        case GA_VC_VOCS_GET_AUDIO_OUT_DESC_CNF:
            APPL_TRC("[Profile]     : VCP - VOCS (0x%04X)\n", GA_SERVICE_VOCS);
            APPL_TRC("[SECTION]     : VCP - VOCS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - VOCS CONTROLLER-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOCS_OFFSET_STATE_NTF: /* Fall - through */
        case GA_VC_VOCS_AUDIO_LOCATION_NTF: /* Fall - through */
        case GA_VC_VOCS_AUDIO_OUT_DESC_NTF:
            APPL_TRC("[Profile]     : VCP - VOCS (0x%04X)\n", GA_SERVICE_VOCS);
            APPL_TRC("[SECTION]     : VCP - VOCS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - VOCS CONTROLLER-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_RELEASE_CAPABILITY_CNF:
            APPL_TRC
            (
                "[Profile]     : VCP - VOCS (0x%04X) / AICS (0x%04X)\n",
                GA_SERVICE_VOCS,
                GA_SERVICE_AICS
            );
            APPL_TRC("[SECTION]     : VCP - VOCS / AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - VOCS / AICS CONTROLLER-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_RELEASE_CNF:
            APPL_TRC("[Profile]     : VCP (0x%04X)\n", GA_SERVICE_VCS);
            APPL_TRC("[SECTION]     : VCP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP CONTROLLER-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_AICS_GET_INPUT_STATE_CNF: /* Fall - through */
        case GA_VC_AICS_GET_GAIN_SETTING_PROPRTY_CNF: /* Fall - through */
        case GA_VC_AICS_GET_INPUT_TYPE_CNF: /* Fall - through */
        case GA_VC_AICS_GET_INPUT_STATUS_CNF: /* Fall - through */
        case GA_VC_AICS_GET_AUDIO_INPUT_DESC_CNF:
            APPL_TRC("[Profile]     : VCP - AICS (0x%04X)\n", GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : VCP - AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - AICS CONTROLLER-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_AICS_INPUT_STATE_NTF: /* Fall - through */
        case GA_VC_AICS_INPUT_STATUS_NTF:  /* Fall - through */
        case GA_VC_AICS_AUDIO_INPUT_DESC_NTF:
            APPL_TRC("[Profile]     : VCP - AICS (0x%04X)\n", GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : VCP - AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - AICS CONTROLLER-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_VOCS_CP_WT_CNF:
            APPL_TRC("[Profile]     : VCP - VOCS (0x%04X)\n", GA_SERVICE_VOCS);
            APPL_TRC("[SECTION]     : VCP - VOCS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - VOCS CONTROLLER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_VC_AICS_CP_WT_CNF:
            APPL_TRC("[Profile]     : VCP - AICS (0x%04X)\n", GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : VCP - AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : VCP - AICS CONTROLLER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_vcp_ct_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        default:
            APPL_ERR("UnHandled VCP Event: 0x%02X !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

void appl_vcp_parse_and_display_vcs_char_value
     (
         UINT16   char_uuid,
         UCHAR  * vcs_data
     )
{
    switch (char_uuid)
    {
    case GA_CHAR_VCS_VOLUME_STATE:
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Volume_Setting: 0x%02X\n",
                (UCHAR)sizeof(vcs_data[0U]), vcs_data[0U]
            );
            APPL_TRC("Len: 0x%02X,  Mute: ", (UCHAR)sizeof(vcs_data[1U]));
            appl_ga_utils_display_mute(vcs_data[1U]);
            APPL_TRC
            (
                "Len: 0x%02X,  Change_Counter: 0x%02X\n",
                (UCHAR)sizeof(vcs_data[2U]), vcs_data[2U]
            );
        }
        break;

    case GA_CHAR_VCS_VOLUME_FLAGS:
        APPL_TRC
        (
            "Len: 0x%02X,  Volume_Setting_Persisted: ",
            (UCHAR)sizeof(vcs_data[0U])
        );
        if (VCS_VOLUME_SETTING_PERSIST_NO == (UINT8)vcs_data[0U])
        {
            APPL_TRC("Reset Volume Setting (0x%02X)\n", vcs_data[0U]);
        }
        else if (VCS_VOLUME_SETTING_PERSIST_YES == (UINT8)vcs_data[0U])
        {
            APPL_TRC("User Set Volume Setting (0x%02X)\n", vcs_data[0U]);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        break;

    default:
        APPL_ERR("UnHandled Char UUID from VCS: 0x%04X !\n", char_uuid);
        break;
    }
    return;
}

void appl_vcp_parse_and_display_vocs_char_value
     (
         UINT16   char_uuid,
         UCHAR  * vocs_data,
         UINT16   datalen
     )
{
    UINT32  audio_loc;
    INT16   vol_offset;
    UCHAR   change_counter;
    UCHAR   i;

    /* Initialize */
    i = 0U;

    switch(char_uuid)
    {
        case GA_CHAR_VOCS_OFFSET_STATE:
            GA_UNPACK_LE_2_BYTE
            (
                &vol_offset,
                &vocs_data[0U]
            );
            change_counter = vocs_data[2U];

            APPL_TRC
            (
                "Len: 0x%02X,  Volume_Offset: 0x%04X\n",
                (UCHAR)sizeof(vol_offset),
                vol_offset
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Change_Counter: 0x%02X\n",
                (UCHAR)sizeof(change_counter), change_counter
            );
            break;

        case GA_CHAR_VOCS_AUDIO_LOCATION:
            GA_UNPACK_LE_4_BYTE
            (
                &audio_loc,
                &vocs_data[0U]
            );

            APPL_TRC
            (
                "Len: 0x%02X,  Audio Location: ",
                (UCHAR)sizeof(audio_loc)
            );
            appl_ga_utils_display_audio_loc(audio_loc);
            break;

        case GA_CHAR_VOCS_AUDIO_OUTPUT_DESC:
                APPL_TRC("Len: 0x%02X,  Audio Output Description: ", datalen);
                for (i = 0U; i < datalen; i++)
                {
                    APPL_TRC("%c", vocs_data[i]);
                }
                APPL_TRC("\n");
            break;

        default:
            APPL_ERR("UnHandled Char UUID from VOCS: 0x%04X !\n", char_uuid);
            break;
    }
    return;
}

void appl_vcp_display_ga_result(GA_RESULT status, UINT8 service)
{
    GA_RESULT  retval;

    retval = GA_SUCCESS;

    /* Checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(status);

    if (GA_FAILURE == retval)
    {
        retval = GA_SUCCESS;
        switch (status)
        {
        /* Implementation related error codes */
        case GA_VCP_INVALID_STATE:
            APPL_ERR("Invalid State !");
            break;

        case GA_VCP_INVALID_PARAMS:
            APPL_ERR("Invalid Params !");
            break;

        case GA_VCP_CNTX_ALLOC_FAILED:
            APPL_ERR("Context allocation failed !");
            break;

        case GA_VCP_IN_BUSY_STATE:
            APPL_ERR("In Busy State !");
            break;

        case GA_VCP_INVALID_HANDLE:
            APPL_ERR("Invalid Handle !");
            break;

        case GA_VCP_OPERATION_NOT_SUPPORTED_BY_REMOTE_DEVICE:
            APPL_ERR("Operation not supported by Remote Device !");
            break;

        case GA_VCP_SERVICE_ID_NOT_FOUND:
            APPL_ERR("Service ID not found !");
            break;

        case GA_VCP_INVALID_SERVICE_TYPE:
            APPL_ERR("Invalid Service Type !");
            break;

        default:
            retval = GA_FAILURE;
            break;
        }

        if (GA_SUCCESS == retval)
        {
            APPL_TRC(" (0x%04X)\n", status);
        }
        else
        {
            if (VCP_SERVICE_TYPE_VCS == service)
            {
                appl_vcp_display_vcs_appl_err_codes(status);
            }
            else if (VCP_SERVICE_TYPE_VOCS == service)
            {
                appl_vcp_display_vocs_appl_err_codes(status);
            }
            else if (VCP_SERVICE_TYPE_AICS == service)
            {
                appl_aics_display_appl_err_codes(status);
            }
            else
            {
                APPL_ERR("Invalid Service: 0x%04X !\n", service);
            }
        }
    }
    return;
}

void appl_vcp_display_vcs_appl_err_codes(GA_RESULT status)
{
    switch (status)
    {
    /* Application error code */
    case VCS_ERR_CODE_INVALID_CHNG_CNTR:
        APPL_ERR("Invalid Change Counter !");
        break;

    case VCS_ERR_CODE_OPCODE_NOT_SUPPORTED:
        APPL_ERR("Opcode Not Supported !");
        break;

    default:
        APPL_ERR("Invalid Code !");
        break;
    }

    APPL_TRC(" (0x%04X)\n", status);
    return;
}

void appl_vcp_display_vocs_appl_err_codes(GA_RESULT status)
{
    switch (status)
    {
    /* Application error code */
    case VOCS_ERR_CODE_INVALID_CHNG_CNTR:
        APPL_ERR("Invalid Change Counter !");
        break;

    case VOCS_ERR_CODE_OPCODE_NOT_SUPPORTED:
        APPL_ERR("Opcode Not Supported !");
        break;

    case VOCS_ERR_CODE_VALUE_OUT_OF_RANGE:
        APPL_ERR("Value Out of Range !");
        break;

    default:
        APPL_ERR("Invalid Code !");
        break;
    }

    APPL_TRC(" (0x%04X)\n", status);
    return;
}

#endif /* GA_VCP */
