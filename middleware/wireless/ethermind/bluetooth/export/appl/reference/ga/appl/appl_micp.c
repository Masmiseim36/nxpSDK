/**
 *  \file appl_micp.c
 *
 *  \brief This file defines the Microphone Control Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enable a Generic Attribute Profile (GATT)
 *  client to control and obtain the status of a microphone through the
 *  Microphone Control Service (MICS).
 *  This application also allows to perform the procedures used to control and
 *  obtain the status of instances of the Audio Input Control Service (AICS)
 *  if supported.
 *  Roles: Microphone Controller: Device that controls the microphone such as
 *                                a mobile phone.
 *         Microphone Device: Device that exposes controls for a microphone
 *                            such as a headset.
 *  GATT Requirement: Microphone Controller: GATT Client
 *                    Microphone Devic: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_micp.h"

#ifdef GA_MICP

/* --------------------------------------------- Global Definitions */

/* MICP Controller Menu Operations */
static const char ga_micp_options[] = " \n\
================ GA MICP Controller MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Context Management ~ Setup ============ \n\
    2. MICP Setup \n\
    3. MICP Get Capabilities \n\
    4. MICP Setup AICS Capability \n\
\n\
================ Context Management ~ Release ============ \n\
    5. MICP Release/Close MICS \n\
    6. MICP Release/Close AICS Capability \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
    7. Update AICS Handle \n\
\n\
================ Profile Management (MICS) ~ Operations(Read) ============ \n\
   10. MICS Get Mute \n\
================ Profile Management (MICS) ~ Operations(Write) ============ \n\
   11. MICS Set Mute \n\
\n\
================ Profile Management (AICS) ~ Operations(Read) ============ \n\
   30. MICS AICS Get Audio Input State \n\
   31. MICS AICS Get Gain Setting Properties \n\
   32. MICS AICS Get Audio Input Type \n\
   33. MICS AICS Get Audio Input Status \n\
   34. MICS AICS Get Audio Input Description \n\
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
   80. Get MICS Context Info. \n\
   81. Set MICS Context Info. \n\
\n\
   82. Get AICS Context Info. \n\
   83. Set AICS Context Info. \n\
\n\
   87. Free Saved MICS Context Info. \n\
   88. Free Saved AICS Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure MICS Notification \n\
   91. Configure AICS Notification \n\
\n\
Your Option ?: ";

/* MICP Device Menu Operations */
static const char ga_mcd_options[] = " \n\
================ GA MICP Device MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Service Management ~ Included Service ============ \n\
    6. Register AICS Service \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
    7. Update AICS Handle \n\
\n\
================ Device Management ~ Setting ============ \n\
   30. Set the Peer Address \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   50. MICS Notify Mute \n\
\n\
   51. AICS Notify Audio Input State \n\
   52. AICS Notify Audio_Input Status \n\
   53. AICS Notify Audio Input Description \n\
\n\
   60. MICS Set Mute Setting Value \n\
\n\
================ MICS Management ~ Display ============ \n\
   61. MICS Display Local Mute Setting Value \n\
\n\
================ Service Management ~ SDP Record ============ \n\
   70. Activate MICS SDP record \n\
   71. InActivate MICS SDP record \n\
\n\
Your Option ?: ";

#ifdef MICP_CONTROLLER

DECL_STATIC GA_ENDPOINT   ga_micp_device;
DECL_STATIC GA_MC_CONTEXT ga_micp_context;
DECL_STATIC GA_MC_CONTEXT ga_micp_aics_context;

DECL_STATIC MICS_CHAR_INFO appl_micp_cntrl_mute_val;

DECL_STATIC APPL_AICS_CHAR_INFO  appl_mc_aics_chr_info[MICP_MAX_AICS_CONTEXTS];
DECL_STATIC APPL_AICS_CHAR_INPUT_STATE_PARAMS
                appl_mc_aics_input_state[MICP_MAX_AICS_CONTEXTS];
DECL_STATIC APPL_AICS_CHAR_GAIN_SETTING_PARAMS
                appl_mc_aics_input_gain_setting[MICP_MAX_AICS_CONTEXTS];

#ifdef MICP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_micp_mics_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_micp_mics_char_info_ptr;
DECL_STATIC  UINT16              appl_micp_mics_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_micp_mics_range;
DECL_STATIC  GA_CHAR_INFO      * appl_micp_mics_char_info;
DECL_STATIC  GA_MC_CONTEXT       appl_micp_mics_handle_access;

DECL_STATIC  GA_BRR_SVC_RANGE  * appl_micp_aics_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_micp_aics_char_info_ptr;
DECL_STATIC  UINT16              appl_micp_aics_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_micp_aics_range;
DECL_STATIC  GA_CHAR_INFO      * appl_micp_aics_char_info;
DECL_STATIC  GA_CONTEXT          appl_micp_aics_handle_access;
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
#endif /* MICP_CONTROLLER */

#ifdef MICP_DEVICE

DECL_STATIC GA_ENDPOINT ga_mcd_device;

DECL_STATIC GA_CONTEXT  ga_mcd_aics_handle;

/* Timer Handle to send MICS Notifications */
DECL_STATIC BT_timer_handle  appl_mcd_mics_cp_wt_timer_handle;

/* Timer Handle to send AICS Notifications */
DECL_STATIC BT_timer_handle  appl_mcd_aics_cp_wt_timer_handle;

/* Timer Handle to send AICS Audio Input Description Notification */
DECL_STATIC BT_timer_handle  appl_mcd_aics_set_audio_inp_desc_timer_handle;

DECL_STATIC MICS_CHAR_INFO appl_micp_dev_mute_val;

DECL_STATIC UCHAR appl_mcd_audio_in_desptn_l[16U] = "MIC_L";
DECL_STATIC UCHAR appl_mcd_audio_in_desptn_r[16U] = "MIC_R";
DECL_STATIC APPL_AICS_CHAR_INFO  mcd_aics_chr_info[MICP_MAX_AICS_ENTITIES];
DECL_STATIC APPL_AICS_CHAR_INPUT_STATE_PARAMS
                mcd_aics_input_state[MICP_MAX_AICS_ENTITIES];
DECL_STATIC APPL_AICS_CHAR_GAIN_SETTING_PARAMS
                mcd_aics_input_gain_setting[MICP_MAX_AICS_ENTITIES];

DECL_STATIC  UCHAR appl_mcd_chr_data[64U];
DECL_STATIC  UCHAR appl_mcd_chr_datalen;

#endif /* MICP_DEVICE */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- APIs */

void appl_micp_display_ga_result(GA_RESULT status, UINT8 service);

void appl_micp_display_mics_appl_err_codes(GA_RESULT status);

/*** APIs - Microphone Controller ***/
void appl_micp_ct_set_default_values(void)
{
#ifdef MICP_CONTROLLER
    UINT8 i;

    for(i = 0U; i < MICP_MAX_AICS_CONTEXTS; i++)
    {
        /* MICP-AICS */
        appl_mc_aics_chr_info[i].input_state = &appl_mc_aics_input_state[i];
        appl_mc_aics_chr_info[i].gain_setting =
            &appl_mc_aics_input_gain_setting[i];
    }
#endif /* MICP_CONTROLLER */

    return;
}

void appl_micp_ct_operations(void)
{
    GA_RESULT  retval;
    int        choice;
    int        menu_choice;
    int        input;
    UINT8      conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(input);

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", ga_micp_options);
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
                             &ga_micp_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                APPL_TRC("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_micp_device.bd_addr);
                APPL_TRC("Enter endpoint type: ");
                scanf("%d", &choice);
                ga_micp_device.bd_type = (UINT8)choice;
            }
#else
            BT_COPY_BD_ADDR(ga_micp_device.bd_addr, g_pts_bd_addr);
            ga_micp_device.bd_type = BT_BD_PUBLIC_ADDRESS_TYPE;
#endif

            APPL_TRC("Setup MICP \n");
            retval = GA_mc_setup(&ga_micp_device, &ga_micp_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            APPL_TRC("MICP Context: %d\n", ga_micp_context);
            break;

        case 3:
            APPL_TRC("GA_mc_get_capabilities: \n");
            retval = GA_mc_get_capabilities(&ga_micp_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            break;

        case 4:
        {
            GA_BRR_SVC_INFO svc;

            svc.uuid = GA_SERVICE_AICS;

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
            svc.range.start = 0x52U;
            svc.range.end = 0x61U;
#else
            /* PTS DB*/
            svc.range.start = 0xA0U;
            svc.range.end = 0xAFU;
#endif
#endif /* 0 */

            APPL_TRC("AICS Start Handle:0x%04x\n", svc.range.start);
            APPL_TRC("AICS End Handle:0x%04x\n", svc.range.end);

            APPL_TRC("GA_mc_set_capability: \n");
            retval = GA_mc_set_capability
                     (
                         &ga_micp_context,
                         &svc,
                         &ga_micp_aics_context
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
            break;

        case 5:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_mc_release: \n");
                retval = GA_mc_release(&ga_micp_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_mc_close: \n");
                retval = GA_mc_close(&ga_micp_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 6:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_mc_release_capability: \n");
                retval = GA_mc_release_capability(&ga_micp_aics_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_mc_close_capability: \n");
                retval = GA_mc_close_capability(&ga_micp_aics_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 7:
            CONSOLE_OUT("Enter the AICS Handle: ");
            CONSOLE_IN("%d", &choice);
            if (MICP_MAX_AICS_CONTEXTS <= (UINT8)choice)
            {
                APPL_INF("Invalid handle !\n");
            }
            else
            {
                ga_micp_aics_context = (UINT8)choice;
            }
            break;

        case 10:
            APPL_TRC("GA_mc_mics_get_mute: \n");
            retval = GA_mc_mics_get_mute(ga_micp_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            break;

        case 11:
        {
            CONSOLE_OUT("Select your choice\n");
            CONSOLE_OUT("0 -> Not Mute\n");
            CONSOLE_OUT("1 -> Mute\n");
            CONSOLE_OUT("2 -> Disable Mute\n");
            CONSOLE_IN("%x", &choice);

            if (0U == (UINT8)choice)
            {
                APPL_TRC("GA_mc_mics_set_unmute: \n");
                retval = GA_mc_mics_set_unmute(ga_micp_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            }
            else if (1U == (UINT8)choice)
            {
                APPL_TRC("GA_mc_mics_set_mute: \n");
                retval = GA_mc_mics_set_mute(ga_micp_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            }
            else if (2U == (UINT8)choice)
            {
                APPL_TRC("GA_mc_mics_set_mute: \n");
                retval = GA_mc_mics_set_mute_disable(ga_micp_context);
                APPL_TRC("Retval - ");
                appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            }
            else
            {
                APPL_INF("Invalid Choice\n");
            }
        }
            break;

        case 30:
            APPL_TRC("GA_mc_aics_get_input_state: \n");
            retval = GA_mc_aics_get_input_state(ga_micp_aics_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 31:
            APPL_TRC("GA_mc_aics_get_gain_setting_proprty: \n");
            retval = GA_mc_aics_get_gain_setting_proprty(ga_micp_aics_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 32:
            APPL_TRC("GA_mc_aics_get_input_type: \n");
            retval = GA_mc_aics_get_input_type(ga_micp_aics_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 33:
            APPL_TRC("GA_mc_aics_get_input_status: \n");
            retval = GA_mc_aics_get_input_status(ga_micp_aics_context);
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 34:
            APPL_TRC("GA_mc_aics_get_audio_input_description: \n");
            retval = GA_mc_aics_get_audio_input_description
                     (
                         ga_micp_aics_context
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 35:
        {
            UCHAR   aid[16U];
            UCHAR   aidl;

            EM_str_copy
            (
                aid,
                APPL_MICS_AICS_INPUT_DESC
            );

            aidl = (UCHAR )EM_str_len(aid) + 1U;

            APPL_TRC("GA_mc_aics_set_audio_input_description: \n");
            retval = GA_mc_aics_set_audio_input_description
                    (
                        ga_micp_aics_context,
                        aid,
                        aidl
                    );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 36:
        {
            UCHAR appl_audio_input[2U];

            CONSOLE_OUT("Enter Gain Setting(-128 to 127): ");
            CONSOLE_IN("%d", &input);

            appl_audio_input[1U] = (CHAR)input;
            appl_audio_input[0U] =
                appl_mc_aics_chr_info[ga_micp_aics_context].input_state->change_counter;

            APPL_TRC("GA_mc_aics_set_gain_setting: \n");
            retval = GA_mc_aics_set_gain_setting
                     (
                         ga_micp_aics_context,
                         &appl_audio_input[0U],
                         2U
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 37:
        {
            UCHAR appl_audio_input[2U];

            appl_audio_input[0U] = 0x02U;

            APPL_TRC("GA_mc_aics_unmute: \n");
            retval = GA_mc_aics_unmute
                     (
                         ga_micp_aics_context,
                         &appl_mc_aics_chr_info[ga_micp_aics_context].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 38:
        {
            UCHAR appl_audio_input[2U];

            appl_audio_input[0U] = 0x02U;

            APPL_TRC("GA_mc_aics_mute: \n");
            retval = GA_mc_aics_mute
                     (
                         ga_micp_aics_context,
                         &appl_mc_aics_chr_info[ga_micp_aics_context].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 39:
        {
            UCHAR appl_audio_input[2U];

            appl_audio_input[0U] = 0x03U;

            APPL_TRC("GA_mc_aics_set_manual_gain_mode: \n");
            retval = GA_mc_aics_set_manual_gain_mode
                     (
                         ga_micp_aics_context,
                         &appl_mc_aics_chr_info[ga_micp_aics_context].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 40:
        {
            UCHAR appl_audio_input[2U];

            appl_audio_input[0U] = 0x04U;

            APPL_TRC("GA_mc_aics_set_auto_gain_mode: \n");
            retval = GA_mc_aics_set_auto_gain_mode
                     (
                         ga_micp_aics_context,
                         &appl_mc_aics_chr_info[ga_micp_aics_context].input_state->change_counter,
                         1U
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
        }
        break;

        case 80:
        {
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            appl_micp_mics_handle_access = ga_micp_context;

            APPL_TRC("GA_mc_get_context_info ...\n");
            retval = GA_mc_get_context_info
                     (
                         &ga_micp_device,
                         &appl_micp_mics_handle_access,
                         &appl_micp_mics_range_ptr,
                         &appl_micp_mics_char_info_ptr,
                         &appl_micp_mics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_micp_mics_char_count:0x%02x\n",
                    appl_micp_mics_char_count
                );

                appl_micp_mics_range = *appl_micp_mics_range_ptr;

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_micp_mics_char_info =
                    GA_alloc_mem(appl_micp_mics_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_micp_mics_char_info)
                {
                    GA_mem_copy
                    (
                        appl_micp_mics_char_info,
                        appl_micp_mics_char_info_ptr,
                        appl_micp_mics_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 81:
        {
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_micp_mics_char_info)
            {
                APPL_TRC("appl_micp_mics_char_info is NULL \n");
                APPL_TRC("Call GA_mc_get_mics_context_info\n");
                break;
            }

            appl_micp_mics_char_info_ptr = appl_micp_mics_char_info;
            appl_micp_mics_range_ptr = &appl_micp_mics_range;

            APPL_TRC("GA_mc_set_context_info ...\n");
            retval = GA_mc_set_context_info
                     (
                         &ga_micp_device,
                         &appl_micp_mics_handle_access,
                         &appl_micp_mics_range_ptr,
                         &appl_micp_mics_char_info_ptr,
                         &appl_micp_mics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            if (GA_SUCCESS == retval)
            {
                ga_micp_context = appl_micp_mics_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_micp_mics_char_info);
                appl_micp_mics_char_info = NULL;
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 82:
        {
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            appl_micp_mics_handle_access = ga_micp_context;
            appl_micp_aics_handle_access = ga_micp_aics_context;

            APPL_TRC("GA_mc_get_aics_context_info ...\n");
            retval = GA_mc_get_aics_context_info
                     (
                         &ga_micp_device,
                         &appl_micp_aics_handle_access,
                         &appl_micp_aics_range_ptr,
                         &appl_micp_aics_char_info_ptr,
                         &appl_micp_aics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);

            if (GA_SUCCESS == retval)
            {
                APPL_TRC
                (
                    "appl_vcp_aics_char_count:0x%02x\n",
                    appl_micp_aics_char_count
                );

                appl_micp_aics_range = *appl_micp_aics_range_ptr;
                appl_micp_aics_char_info =
                    GA_alloc_mem(appl_micp_aics_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_micp_aics_char_info)
                {
                    GA_mem_copy
                    (
                        appl_micp_aics_char_info,
                        appl_micp_aics_char_info_ptr,
                        appl_micp_aics_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 83:
        {
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_micp_aics_char_info)
            {
                printf("appl_micp_aics_char_info is NULL \n");
                printf("Call GA_mc_get_aics_context_info\n");
                break;
            }

            appl_micp_aics_char_info_ptr = appl_micp_aics_char_info;
            appl_micp_aics_range_ptr = &appl_micp_aics_range;

            APPL_TRC("GA_mc_set_aics_context_info ...\n");
            retval = GA_mc_set_aics_context_info
                     (
                         &ga_micp_device,
                         &appl_micp_aics_handle_access,
                         &appl_micp_aics_range_ptr,
                         &appl_micp_aics_char_info_ptr,
                         &appl_micp_aics_char_count
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            if (GA_SUCCESS == retval)
            {
                ga_micp_aics_context = appl_micp_aics_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_micp_aics_char_info);
                appl_micp_aics_char_info = NULL;
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 87:
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_mc_set_context_info() is NOT called after
             * GA_mc_get_context_info().
             */

            /* Free the allocated memory for MICS char. info. */
            if (NULL != appl_micp_mics_char_info)
            {
                APPL_TRC("Freeing appl_micp_mics_char_info ...\n");
                GA_free_mem(appl_micp_mics_char_info);
                appl_micp_mics_char_info = NULL;
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 88:
#ifdef MICP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_mc_set_aics_context_info() is NOT called after
             * GA_mc_get_aics_context_info().
             */

            /* Free the allocated memory for AICS char. info. */
            if (NULL != appl_micp_aics_char_info)
            {
                APPL_TRC("Freeing appl_micp_aics_char_info ...\n");
                GA_free_mem(appl_micp_aics_char_info);
                appl_micp_aics_char_info = NULL;
            }
#else /* MICP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("MICP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
        {
#ifdef MICP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_mc_update_ntf_configuration...\n");
            retval = GA_mc_update_ntf_configuration
                     (
                         MICP_SERVICE_TYPE_MICS,
                         0U /* (MICS_CHAR_CONFIG(MICS_CHAR_ID_MUTE)) */
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
#else /* MICP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("MICP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* MICP_SUPPORT_CONFIG_SELECTION */
        }
        break;

        case 91:
        {
#ifdef MICP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_mc_update_ntf_configuration...\n");
            retval = GA_mc_update_ntf_configuration
                     (
                         MICP_SERVICE_TYPE_AICS,
                        (AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_STATE)
                        | AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_STATUS)
                        /* | AICS_CHAR_CONFIG(AICS_CHAR_ID_AUDIO_INPUT_DESCRIPTION) */)
                     );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
#else /* MICP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("MICP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* MICP_SUPPORT_CONFIG_SELECTION */
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

GA_RESULT appl_micp_ct_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT      retval;
    GA_ENDPOINT  * device;
    UCHAR          rsp_event;
    UINT16         rsp_status;
    UCHAR          i;

    MICP_EVENT_INFO     * mc_event_info;
    UCHAR                 mc_srvc_inst;
    UCHAR               * mc_event_data;
    UCHAR                 mc_event_datalen;

    BT_IGNORE_UNUSED_PARAM(ga_datalen);

    /* Initialize */
    retval = GA_SUCCESS;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;
    device = NULL;
    i = 0U;

    mc_event_info = (MICP_EVENT_INFO *)ga_data;
    mc_event_data = mc_event_info->data;
    mc_event_datalen = mc_event_info->datalen;
    mc_srvc_inst = mc_event_info->srv_inst;

    APPL_TRC("[Event Status]: ");
    appl_micp_display_ga_result(ga_status, mc_event_info->srvc_type);

    switch (ga_event)
    {
        case GA_MC_AICS_GET_INPUT_STATE_CNF: /* Fall - through */
        case GA_MC_AICS_INPUT_STATE_NTF:
        {
            if(GA_MC_AICS_GET_INPUT_STATE_CNF == ga_event)
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_GET_INPUT_STATE_CNF (0x%02X)\n",
                    ga_event
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_INPUT_STATE_NTF (0x%02X)\n",
                    ga_event
                );
            }

            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Audio Input State (0x%04X)\n",
                GA_CHAR_AICS_INPUT_STATE
            );
            GA_FORMAT_DISPLAY();

            if (mc_srvc_inst < MICP_MAX_AICS_CONTEXTS)
            {
                if (4U == mc_event_datalen)
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].input_state->gain_setting =
                        mc_event_data[0U];
                    appl_mc_aics_chr_info[mc_srvc_inst].input_state->mute =
                        mc_event_data[1U];
                    appl_mc_aics_chr_info[mc_srvc_inst].input_state->gain_mode =
                        mc_event_data[2U];
                    appl_mc_aics_chr_info[mc_srvc_inst].input_state->change_counter =
                        mc_event_data[3U];

                    APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                    APPL_TRC("Data:\n");

                    appl_aics_parse_and_display_char_value
                    (
                        GA_CHAR_AICS_INPUT_STATE,
                        mc_event_data,
                        mc_event_datalen
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

        case GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF:
        {
            APPL_TRC
            (
                "[Event]       : GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF (0x%02X)\n",
                ga_event
            );

            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Gain Setting Properties (0x%04X)\n",
                GA_CHAR_AICS_GAIN_SETTING
            );
            GA_FORMAT_DISPLAY();

            if (mc_srvc_inst < MICP_MAX_AICS_CONTEXTS)
            {
                if (3U == mc_event_datalen)
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].gain_setting->gain_setting_units =
                        mc_event_data[0U];
                    appl_mc_aics_chr_info[mc_srvc_inst].gain_setting->gain_settting_min =
                        mc_event_data[1U];
                    appl_mc_aics_chr_info[mc_srvc_inst].gain_setting->gain_setting_max =
                        mc_event_data[2U];

                    APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                    APPL_TRC("Data:\n");

                    appl_aics_parse_and_display_char_value
                    (
                        GA_CHAR_AICS_GAIN_SETTING,
                        mc_event_data,
                        mc_event_datalen
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

        case GA_MC_AICS_GET_INPUT_TYPE_CNF:
        {
            APPL_TRC
            (
                "[Event]       : GA_MC_AICS_GET_INPUT_TYPE_CNF (0x%02X)\n",
                ga_event
            );

            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Audio Input Type (0x%04X)\n",
                GA_CHAR_AICS_INPUT_TYPE
            );
            GA_FORMAT_DISPLAY();

            if (mc_srvc_inst < MICP_MAX_AICS_CONTEXTS)
            {
                if (1U == mc_event_datalen)
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].input_type = mc_event_data[0U];

                    APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                    APPL_TRC("Data:\n");

                    appl_aics_parse_and_display_char_value
                    (
                        GA_CHAR_AICS_INPUT_TYPE,
                        mc_event_data,
                        mc_event_datalen
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

        case GA_MC_AICS_GET_INPUT_STATUS_CNF: /* Fall - through */
        case GA_MC_AICS_INPUT_STATUS_NTF:
        {
            if(GA_MC_AICS_GET_INPUT_STATUS_CNF == ga_event)
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_GET_INPUT_STATUS_CNF (0x%02X)\n",
                    ga_event
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_INPUT_STATUS_NTF (0x%02X)\n",
                    ga_event
                );
            }

            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Audio Input Status (0x%04X)\n",
                GA_CHAR_AICS_INPUT_STATUS
            );
            GA_FORMAT_DISPLAY();

            if (mc_srvc_inst < MICP_MAX_AICS_CONTEXTS)
            {
                if (1U == mc_event_datalen)
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].input_status = mc_event_data[0U];

                    APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                    APPL_TRC("Data:\n");

                    appl_aics_parse_and_display_char_value
                    (
                        GA_CHAR_AICS_INPUT_STATUS,
                        mc_event_data,
                        mc_event_datalen
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

        case GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF: /* Fall - through */
        case GA_MC_AICS_AUDIO_INPUT_DESC_NTF:
        {
            if(GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF == ga_event)
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF (0x%02X)\n",
                    ga_event
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_AUDIO_INPUT_DESC_NTF (0x%02X)\n",
                    ga_event
                );
            }

            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Audio Input Description (0x%04X)\n",
                GA_CHAR_AICS_AUDIO_INPUT_DESC
            );
            GA_FORMAT_DISPLAY();

            if (mc_srvc_inst < MICP_MAX_AICS_CONTEXTS)
            {
                if (mc_event_datalen > APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN)
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].audio_in_desc_len =
                        APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN;
                }
                else
                {
                    appl_mc_aics_chr_info[mc_srvc_inst].audio_in_desc_len =
                        mc_event_datalen;
                }

                GA_mem_copy
                (
                    &appl_mc_aics_chr_info[mc_srvc_inst].audio_in_desc[0U],
                    &mc_event_data[0U],
                    appl_mc_aics_chr_info[mc_srvc_inst].audio_in_desc_len
                );

                APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                APPL_TRC("Data:\n");

                appl_aics_parse_and_display_char_value
                (
                    GA_CHAR_AICS_AUDIO_INPUT_DESC,
                    mc_event_data,
                    mc_event_datalen
                );
            }
            else
            {
                /* Should not reach here */
                APPL_ERR("Invalid Service Instance !\n");
            }
        }
            break;

        case GA_MC_SETUP_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_SETUP_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        case GA_MC_RELEASE_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_RELEASE_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        case GA_MC_RELEASE_CAPABILITY_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_RELEASE_CAPABILITY_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        case GA_MC_GET_CAPABILITIES_CNF:
        {
            APPL_TRC
            (
                "[Event]       : GA_MC_GET_CAPABILITIES_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            GA_BRR_SVC_INFO * svc;
            svc = (GA_BRR_SVC_INFO *)mc_event_data;
            if(NULL != svc)
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
                APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
                if(0U == mc_event_datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
            }
        }
            break;

        case GA_MC_GET_MUTE_CNF: /* Fall - through */
        case GA_MC_MUTE_NTF:
        {
            if(GA_MC_MUTE_NTF == ga_event)
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_MUTE_NTF (0x%02X)\n",
                    ga_event
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_GET_MUTE_CNF (0x%02X)\n",
                    ga_event
                );
            }
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: MUTE (0x%04X)\n",
                GA_CHAR_MICS_MUTE
            );
            if (1U == mc_event_datalen)
            {
                appl_micp_cntrl_mute_val.mute = mc_event_data[0U];
            }
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Mute: ",
                (UCHAR)sizeof(appl_micp_cntrl_mute_val.mute)
            );
            appl_ga_utils_display_mute(appl_micp_cntrl_mute_val.mute);
        }
            break;

        case GA_MC_SET_MUTE_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_SET_MUTE_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: MUTE (0x%04X)\n",
                GA_CHAR_MICS_MUTE
            );
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        case GA_MC_SET_CAPABILITIES_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_SET_CAPABILITIES_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        case GA_MC_AICS_CP_WT_CNF:
            APPL_TRC
            (
                "[Event]       : GA_MC_AICS_CP_WT_CNF (0x%02X)\n",
                ga_event
            );
            APPL_TRC
            (
                "[Service Cntx]: 0x%02X\n",
                mc_srvc_inst
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Audio Input Control Point (0x%04X)\n",
                GA_CHAR_AICS_AUDIO_INPUT_CP
            );
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", mc_event_datalen);
            if(0U == mc_event_datalen)
            {
                APPL_TRC("Data: NULL\n");
            }
            break;

        default:
            APPL_ERR("UnHandled Event (0x%02X) !\n", ga_event);
            break;
    }

    return retval;
}

/*** APIs - Microphone Device ***/

GA_RESULT appl_micp_dev_reg_opt_service(void)
{
    GA_RESULT retval;
    UCHAR handle;

    retval = GA_mc_register_aics
             (
                &handle
             );
    if (GA_SUCCESS == retval)
    {
        APPL_TRC("AICS Handle: 0x%02X\n", handle);

        /* By default, Update the Handle to the latest registered */
        ga_mcd_aics_handle = handle;
    }
    return retval;
}

void appl_micp_dev_set_default_values(void)
{
#ifdef MICP_DEVICE
    UINT8 i;

    i = 0U;

    /* Not Muted */
    appl_micp_dev_mute_val.mute = MICS_CHR_VAL_NOT_MUTED;

    /* MICP-AICS */
    mcd_aics_chr_info[0U].input_type = GA_AUDIO_INPUT_TYPE_MICROPHONE;
    mcd_aics_chr_info[1U].input_type = GA_AUDIO_INPUT_TYPE_ANALOG;
    for (i = 0U; i < MICP_MAX_AICS_ENTITIES; i++)
    {
        mcd_aics_chr_info[i].input_state = &mcd_aics_input_state[i];
        mcd_aics_chr_info[i].gain_setting = &mcd_aics_input_gain_setting[i];
        mcd_aics_chr_info[i].input_status = 0x01U;

        mcd_aics_input_state[i].gain_setting = 0x01U;
        mcd_aics_input_state[i].mute = AICS_NOT_MUTED;
        mcd_aics_input_state[i].gain_mode =
            /* AICS_GM_MANUAL */ AICS_GM_AUTOMATIC_ONLY;
        mcd_aics_input_state[i].change_counter = 254U;

        mcd_aics_input_gain_setting[i].gain_setting_units = 0x01U;
        mcd_aics_input_gain_setting[i].gain_settting_min = AICS_GAIN_SETTING_MIN;
        mcd_aics_input_gain_setting[i].gain_setting_max = AICS_GAIN_SETTING_MAX;

        EM_str_copy
        (
            mcd_aics_chr_info[i].audio_in_desc,
            appl_mcd_audio_in_desptn_l
        );
        mcd_aics_chr_info[i].audio_in_desc_len =
            (UCHAR )EM_str_len(appl_mcd_audio_in_desptn_l) + 1U;
    }

    appl_mcd_mics_cp_wt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcd_aics_cp_wt_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcd_aics_set_audio_inp_desc_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

#endif /* MICP_DEVICE */

    return;
}

void appl_micp_dev_operations(void)
{
    GA_RESULT  retval;
    int        choice;
    int        menu_choice;
    int        input;
    UINT8      conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(input);

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", ga_mcd_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 6:
            /* Register MICP-AICS */
            APPL_TRC("GA_mc_register_aics \n");
            retval = appl_micp_dev_reg_opt_service();
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 7:
            CONSOLE_OUT("Enter the AICS Handle: ");
            CONSOLE_IN("%d", &choice);
            if (MICP_MAX_AICS_ENTITIES <= (UINT8)choice)
            {
                APPL_INF("Invalid handle !\n");
            }
            else
            {
                ga_mcd_aics_handle = (UINT8)choice;
            }
            break;
        case 30:

#if 1
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &ga_mcd_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_mcd_device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                ga_mcd_device.bd_type = (UINT8)choice;
            }
#else
            APPL_TRC("Updating the Peer BD Address\n");
            BT_COPY_BD_ADDR(ga_mcd_device.bd_addr, g_pts_bd_addr);
            ga_mcd_device.bd_type = BT_BD_PUBLIC_ADDRESS_TYPE;
#endif /* 0 */
            break;

        /* ================ MICP Device Options ================= */
        case 50:
            APPL_TRC("GA_mcd_notify_mics_mute_status \n");
            retval = GA_mcd_notify_mics_mute_status
                     (
                        &ga_mcd_device,
                        &appl_micp_dev_mute_val.mute,
                        sizeof(appl_micp_dev_mute_val.mute)
                      );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
            break;

        case 51:
            mcd_aics_chr_info[ga_mcd_aics_handle].input_state->change_counter += 1U;

            APPL_TRC
            (
                "\t======== AICS Input State Changed, Sending Notification ====\n"
            );
            APPL_TRC("Change_Counter: 0x%02x\n",
            mcd_aics_chr_info[ga_mcd_aics_handle].input_state->change_counter);

            retval = appl_micp_dev_pack_aics_chr_value
                     (
                         GA_CHAR_AICS_INPUT_STATE,
                         &mcd_aics_chr_info[ga_mcd_aics_handle],
                         appl_mcd_chr_data,
                         &appl_mcd_chr_datalen
                     );

            APPL_TRC("GA_mcd_notify_aics_audio_input_state \n");
            retval = GA_mcd_notify_aics_audio_input_state
                    (
                        &ga_mcd_device,
                        ga_mcd_aics_handle,
                        &appl_mcd_chr_data[0U],
                        appl_mcd_chr_datalen
                    );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 52:
            retval = appl_micp_dev_pack_aics_chr_value
                     (
                         GA_CHAR_AICS_INPUT_STATUS,
                         &mcd_aics_chr_info[ga_mcd_aics_handle],
                         appl_mcd_chr_data,
                         &appl_mcd_chr_datalen
                     );

            APPL_TRC("GA_mcd_notify_aics_audio_input_status \n");
            retval = GA_mcd_notify_aics_audio_input_status
                    (
                        &ga_mcd_device,
                        ga_mcd_aics_handle,
                        &appl_mcd_chr_data[0U],
                        appl_mcd_chr_datalen
                    );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 53:
            retval = appl_micp_dev_pack_aics_chr_value
                     (
                         GA_CHAR_AICS_AUDIO_INPUT_DESC,
                         &mcd_aics_chr_info[ga_mcd_aics_handle],
                         appl_mcd_chr_data,
                         &appl_mcd_chr_datalen
                     );

            APPL_TRC("GA_mcd_notify_aics_audio_input_desc \n");
            retval = GA_mcd_notify_aics_audio_input_desc
                    (
                        &ga_mcd_device,
                        ga_mcd_aics_handle,
                        &appl_mcd_chr_data[0U],
                        appl_mcd_chr_datalen
                    );
            APPL_TRC("Retval - ");
            appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
            break;

        case 60:
            CONSOLE_OUT("Select your choice\n");
            CONSOLE_OUT("0 -> Not Mute\n");
            CONSOLE_OUT("1 -> Mute\n");
            CONSOLE_OUT("2 -> Disable Mute\n");
            CONSOLE_IN("%d", &choice);
            appl_micp_dev_mute_val.mute = (UCHAR)choice;
            break;

        case 61:
            APPL_TRC("\t-----------------------\n");
            APPL_TRC("\tMute Value: 0x%02X\n", appl_micp_dev_mute_val.mute);
            APPL_TRC("\t-----------------------\n");
            break;

        case 70:
            appl_micp_dev_set_aics_sdp_record_status(0x01U);
            break;

        case 71:
            appl_micp_dev_set_aics_sdp_record_status(0x00U);
            break;

        default:
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

void appl_micp_dev_set_aics_sdp_record_status(UCHAR flag)
{
#if 0
    API_RESULT retval;
    UINT32 record_handle;

    /* Get the MICS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_MICS, 0U, &record_handle);

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

GA_RESULT appl_micp_dev_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT           retval;
    MICP_EVENT_INFO   * micp_event_info;
    UCHAR               rsp_event;
    UINT16              rsp_status;
    MICP_RSP_INF        micp_rsp_info;
    UCHAR               send_micp_rsp_flag;
    UCHAR               i;

    UCHAR               micp_srvc_inst;
    UCHAR             * micp_event_data;
    UCHAR               micp_event_datalen;
    APPL_MICP_NTF_INFO  mics_aics_ntf_info;
    UINT8               aics_char_val_change_flag;

    GA_IGNORE_UNUSED_PARAM(ga_datalen);

    /* Initialize */
    retval = GA_SUCCESS;
    send_micp_rsp_flag = 0U;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;
    i=0U;
    aics_char_val_change_flag = GA_FALSE;

    micp_event_info = (MICP_EVENT_INFO *)ga_data;
    micp_event_data = micp_event_info->data;
    micp_event_datalen = micp_event_info->datalen;
    micp_srvc_inst = micp_event_info->srv_inst;

    APPL_TRC("[Event Status]: ");
    appl_micp_display_ga_result(ga_status, micp_event_info->srvc_type);

    switch (ga_event)
    {
        case GA_MC_SET_MUTE_IND:
        {
            APPL_TRC("[Event]       : GA_MC_SET_MUTE_IND (0x%02X)\n", ga_event);
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Mute (0x%04X)\n",
                GA_CHAR_MICS_MUTE
            );
            GA_FORMAT_DISPLAY();

            if (0U != micp_event_datalen)
            {
                /**
                 * Check if Local Mute is not disabled, but peer is attempting
                 * set as disabled
                 */
                if (((MICS_CHR_VAL_DISABLED != appl_micp_dev_mute_val.mute) &&
                    (MICS_CHR_VAL_DISABLED == micp_event_data[0U])) ||
                    ((MICS_CHR_VAL_DISABLED < micp_event_data[0U])))
                {
                    /* TODO */
                    rsp_status = MICS_ERR_CODE_NOT_ALLOWED;
                }
                else if ((MICS_CHR_VAL_DISABLED == appl_micp_dev_mute_val.mute) &&
                    (MICS_CHR_VAL_DISABLED > micp_event_data[0U]))
                {
                    rsp_status = MICS_ERR_CODE_MUTE_DISABLED;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                if (GA_SUCCESS == rsp_status)
                {
                    appl_micp_dev_mute_val.mute = micp_event_data[0U];
                    GA_COPY_BRR_DEVICE(&mics_aics_ntf_info.device, device);

                    /* Send the Notification */
                    BT_start_timer
                    (
                        &appl_mcd_mics_cp_wt_timer_handle,
                        1U,
                        appl_micp_mics_dev_cp_write_timer_handler,
                        &mics_aics_ntf_info,
                        sizeof(mics_aics_ntf_info)
                    );
                }

                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("RECEIVED DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x%02X\n", micp_event_datalen);
                if (NULL != micp_event_info)
                {
                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Mute: ",
                        (UCHAR)sizeof(appl_micp_dev_mute_val.mute)
                    );
                    appl_ga_utils_display_mute(appl_micp_dev_mute_val.mute);
                }

            }
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x00\n");
            APPL_TRC("Data: NULL\n");

            /* Fill the response Info. */
            send_micp_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_WT;
            device = micp_event_info->device;
            micp_rsp_info.chr = micp_event_info->chr;
            micp_rsp_info.data = NULL;
            micp_rsp_info.datalen = 0x00U;

        }
            break;

        case GA_MC_AICS_CP_WT_IND:
        {
            {
                APPL_TRC
                (
                    "[Event]       : GA_MC_AICS_CP_WT_IND (0x%02X)\n",
                    ga_event
                );
                APPL_TRC
                (
                    "[Service Inst]: 0x%02X\n",
                    micp_srvc_inst
                );

                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC
                (
                    "Char UUID: Audio Input Control Point (0x%04X)\n",
                    GA_CHAR_AICS_AUDIO_INPUT_CP
                );
                GA_FORMAT_DISPLAY();


                if (NULL != micp_event_info)
                {
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("RECEIVED DATA\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("Data Length: 0x%02X\n", micp_event_datalen);
                    APPL_TRC("Data:\n");
                }
                rsp_status = appl_micp_dev_aics_cp_write_handler(micp_event_info);
            }
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x00\n");
            APPL_TRC("Data: NULL\n");

            /* Fill the response Info. */
            send_micp_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_WT;
            device = micp_event_info->device;
            micp_rsp_info.chr = micp_event_info->chr;
            micp_rsp_info.data = NULL;
            micp_rsp_info.datalen = 0x00U;

        }
            break;

        case GA_MC_RD_IND:
        {
            {
                APPL_TRC("[Event]       : GA_MC_RD_IND (0x%02X)\n", ga_event);
                APPL_TRC
                (
                    "[Service Inst]: 0x%02X\n",
                    micp_srvc_inst
                );
                APPL_TRC("\n");

                if (GA_CHAR_MICS_MUTE == micp_event_info->chr->uuid)
                {
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Mute (0x%04X)\n",
                        GA_CHAR_MICS_MUTE
                    );
                    GA_FORMAT_DISPLAY();
                    appl_mcd_chr_data[0U] = appl_micp_dev_mute_val.mute;
                    appl_mcd_chr_datalen = 1U;
                }
                else
                {
                    retval = GA_FAILURE;
                    APPL_TRC("Invalid Char UUID\n");
                }
            }
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", appl_mcd_chr_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Mute: ", appl_mcd_chr_datalen);
            appl_ga_utils_display_mute(appl_mcd_chr_data[0U]);

            /* Fill the response Info. */
            send_micp_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_RD;
            device = micp_event_info->device;
            micp_rsp_info.chr = micp_event_info->chr;
            micp_rsp_info.data = appl_mcd_chr_data;
            micp_rsp_info.datalen = appl_mcd_chr_datalen;

        }
            break;

        case GA_MC_AICS_RD_IND:
        {
            {
                APPL_TRC("[Event]       : GA_MC_AICS_RD_IND (0x%02X)\n", ga_event);
                APPL_TRC
                (
                    "[Service Inst]: 0x%02X\n",
                    micp_srvc_inst
                );

                APPL_TRC("\n");

                if (MICP_MAX_AICS_ENTITIES <= micp_srvc_inst)
                {
                    /* Should Not reach here */
                    retval = GA_FAILURE;
                    break;
                }
                GA_FORMAT_DISPLAY();
                appl_ga_utils_display_char_uuid(micp_event_info->chr->uuid);
                GA_FORMAT_DISPLAY();

                retval = appl_micp_dev_pack_aics_chr_value
                        (
                            micp_event_info->chr->uuid,
                            &mcd_aics_chr_info[micp_srvc_inst],
                            &appl_mcd_chr_data[0U],
                            &appl_mcd_chr_datalen
                        );
            }
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", micp_event_info->datalen);
            APPL_TRC("Data: \n");
            appl_aics_parse_and_display_char_value
            (
                micp_event_info->chr->uuid,
                appl_mcd_chr_data,
                appl_mcd_chr_datalen
            );

            /* Fill the response Info. */
            send_micp_rsp_flag = 1U;
            rsp_event = GA_RSP_TYPE_RD;
            device = micp_event_info->device;
            micp_rsp_info.chr = micp_event_info->chr;
            micp_rsp_info.data = appl_mcd_chr_data;
            micp_rsp_info.datalen = appl_mcd_chr_datalen;

        }
            break;

        case GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND:
        {
            APPL_TRC
            (
                "[Event]       : GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND (0x%02X)\n",
                ga_event
            );

            APPL_TRC
            (
                "[Service Inst]: 0x%02X\n",
                micp_srvc_inst
            );
            APPL_TRC("\n");

            micp_event_info = (MICP_EVENT_INFO *)ga_data;

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
            APPL_TRC("Data Length: 0x%02X\n", micp_event_info->datalen);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Audio Input Description: ",
                micp_event_info->datalen
            );

            for (i = 0U; i < micp_event_info->datalen; i++)
            {
                APPL_TRC("%c", micp_event_info->data[i]);
            }
            APPL_TRC("\n");

            if (micp_srvc_inst < MICP_MAX_AICS_ENTITIES)
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
                if ((mcd_aics_chr_info[micp_srvc_inst].audio_in_desc_len != micp_event_datalen) ||
                    (0U != GA_mem_cmp
                           (
                               &mcd_aics_chr_info[micp_srvc_inst].audio_in_desc[0U],
                               &micp_event_info->data[0U],
                               micp_event_datalen
                           )))
                {
                    if (micp_event_datalen > APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN)
                    {
                        mcd_aics_chr_info[micp_srvc_inst].audio_in_desc_len =
                            APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN;
                    }
                    else
                    {
                        mcd_aics_chr_info[micp_srvc_inst].audio_in_desc_len = micp_event_datalen;
                    }

                    GA_mem_copy
                    (
                        &mcd_aics_chr_info[micp_srvc_inst].audio_in_desc[0U],
                        &micp_event_data[0U],
                        mcd_aics_chr_info[micp_srvc_inst].audio_in_desc_len
                    );
                }
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

            if (GA_FALSE != aics_char_val_change_flag)
            {
                /*
                 * Notification should be send to the device from which server has
                 * received the CP write request.
                 */
                /* Copy device info */
                GA_COPY_BRR_DEVICE(&mics_aics_ntf_info.device, micp_event_info->device);
                /* Copy service instance */
                mics_aics_ntf_info.srvc_inst = micp_srvc_inst;

                BT_start_timer
                (
                    &appl_mcd_aics_set_audio_inp_desc_timer_handle,
                    1U,
                    appl_micp_aics_rd_set_audio_inp_desc_timer_handler,
                    &mics_aics_ntf_info,
                    sizeof(mics_aics_ntf_info)
                );
            }
        }
            break;

        default:
            APPL_ERR("UnHandled Event (0x%02X) !\n", ga_event);
            break;
    }

    if (0U != send_micp_rsp_flag)
    {
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MICP RESPONSE\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("API   : GA_mc_send_response\n");
        retval = GA_mc_send_rsp
                 (
                     device,
                     rsp_event,
                     rsp_status,
                     &micp_rsp_info,
                     sizeof(MICP_RSP_INF)
                 );
        APPL_TRC("Retval - ");
        appl_micp_display_ga_result(retval, micp_event_info->srvc_type);
    }

    return retval;
}

GA_RESULT appl_micp_dev_aics_cp_write_handler
          (
              MICP_EVENT_INFO * mcd_event_info
          )
{
    GA_RESULT  retval;
    UCHAR      cp_op_code;
    UCHAR    * data;
    UCHAR      datalen;
    UCHAR      aics_change_counter_flag;
    UCHAR      srv_inst;

    APPL_MICP_NTF_INFO aics_ntf_info;

    if ((NULL == mcd_event_info) ||
        (NULL == mcd_event_info->data) ||
        (0U == mcd_event_info->datalen))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    if (MICP_MAX_AICS_CONTEXTS < mcd_event_info->srv_inst)
    {
        APPL_ERR("Invalid AICS Service Instance !\n");
        return GA_FAILURE;
    }

    retval = GA_SUCCESS;
    data = mcd_event_info->data;
    datalen = mcd_event_info->datalen;

    if (1U > datalen)
    {
        APPL_ERR("Invalid CP Write Length !\n");
        return GA_FAILURE;
    }

    cp_op_code = data[0U];

    if ((AICS_CP_OP_SET_AUTO_GAIN_MODE < cp_op_code) ||
        (0U == cp_op_code))
    {
        APPL_ERR("Invalid CP OPCODE (0x%02X) !\n", cp_op_code);
        return AICS_ERR_CODE_OPCODE_NOT_SUPPORTED;
    }

    srv_inst = mcd_event_info->srv_inst;

    data += 1U;
    datalen -= 1U;

    if (1U > datalen)
    {
        APPL_ERR("Invalid CP Write Length !\n");
        return GA_FAILURE;
    }

    if (data[0U] != mcd_aics_chr_info[srv_inst].input_state->change_counter)
    {
        APPL_ERR("Invalid CHANGE COUNTER !\n");
        APPL_TRC("Rx. Change_cnt: 0x%02x, IUT Change Counter:0x%02x\n",
            data[0U], mcd_aics_chr_info[srv_inst].input_state->change_counter);
        return AICS_ERR_CODE_INVALID_CHNG_CNTR;
    }

    aics_change_counter_flag = 0U;

    if (AICS_CP_OP_SET_GAIN_SETTING == cp_op_code)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Set Gain Setting (0x%02X)\n",
            (UCHAR)sizeof(cp_op_code), cp_op_code
        );

        if (0x02U != datalen)
        {
            APPL_ERR("Invalid CP Write Length !\n");
            return GA_FAILURE;
        }

        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Gain_Setting: 0x%02X\n",
            (UCHAR)sizeof(data[1U]), data[1U]
        );

        if (((CHAR)data[1U] < mcd_aics_chr_info[srv_inst].gain_setting->gain_settting_min) ||
            ((CHAR)data[1U] > mcd_aics_chr_info[srv_inst].gain_setting->gain_setting_max))
        {
            return AICS_ERR_CODE_VALUE_OUT_OF_RANGE;
        }
        else if (mcd_aics_chr_info[srv_inst].input_state->gain_setting != (CHAR)data[1U])
        {
            mcd_aics_chr_info[srv_inst].input_state->gain_setting = (CHAR)data[1U];
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_UNMUTE == cp_op_code)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Unmute (0x%02X)\n",
            (UCHAR)sizeof(cp_op_code), cp_op_code
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if (AICS_DISABLED == mcd_aics_chr_info[srv_inst].input_state->mute)
        {
            return AICS_ERR_CODE_MUTE_DISABLED;
        }
        else if (AICS_MUTED == mcd_aics_chr_info[srv_inst].input_state->mute)
        {
            mcd_aics_chr_info[srv_inst].input_state->mute = AICS_NOT_MUTED;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_MUTE == cp_op_code)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Mute (0x%02X)\n",
            (UCHAR)sizeof(cp_op_code), cp_op_code
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if (AICS_DISABLED == mcd_aics_chr_info[srv_inst].input_state->mute)
        {
            return AICS_ERR_CODE_MUTE_DISABLED;
        }
        else if (AICS_NOT_MUTED == mcd_aics_chr_info[srv_inst].input_state->mute)
        {
            mcd_aics_chr_info[srv_inst].input_state->mute = AICS_MUTED;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_SET_MANUAL_GAIN_MODE == cp_op_code)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Set Manual Gain Mode (0x%02X)\n",
            (UCHAR)sizeof(cp_op_code), cp_op_code
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if ((AICS_GM_MANUAL_ONLY == mcd_aics_chr_info[srv_inst].input_state->gain_mode) ||
            (AICS_GM_AUTOMATIC_ONLY == mcd_aics_chr_info[srv_inst].input_state->gain_mode))
        {
            return AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED;
        }
        else if (AICS_GM_AUTO == mcd_aics_chr_info[srv_inst].input_state->gain_mode)
        {
            mcd_aics_chr_info[srv_inst].input_state->gain_mode = AICS_GM_MANUAL;
            aics_change_counter_flag = 1U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
    else if (AICS_CP_OP_SET_AUTO_GAIN_MODE == cp_op_code)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Set Automatic Gain Mode (0x%02X)\n",
            (UCHAR)sizeof(cp_op_code), cp_op_code
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Change_Counter: 0x%02X\n",
            (UCHAR)sizeof(data[0U]), data[0U]
        );

        if ((AICS_GM_MANUAL_ONLY == mcd_aics_chr_info[srv_inst].input_state->gain_mode) ||
            (AICS_GM_AUTOMATIC_ONLY == mcd_aics_chr_info[srv_inst].input_state->gain_mode))
        {
            return AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED;
        }
        else if (AICS_GM_MANUAL == mcd_aics_chr_info[srv_inst].input_state->gain_mode)
        {
            mcd_aics_chr_info[srv_inst].input_state->gain_mode = AICS_GM_AUTO;
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
        APPL_TRC("SENDING GA MICP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("API   : GA_mcd_notify_aics_audio_input_state\n");
        */

        /*
         * Update change counter here as CP response will be sent before
         * notification
         */
        mcd_aics_chr_info[srv_inst].input_state->change_counter += 1U;

        /*
         * Notification should be send to the device from which server has
         * received the CP write request.
         */
        /* Copy device info */
        GA_COPY_BRR_DEVICE(&aics_ntf_info.device, mcd_event_info->device);
        /* Copy service instance */
        aics_ntf_info.srvc_inst = srv_inst;

        BT_start_timer
        (
            &appl_mcd_aics_cp_wt_timer_handle,
            1U,
            appl_micp_aics_dev_cp_write_timer_handler,
            &aics_ntf_info,
            sizeof(aics_ntf_info)
        );
    }

    return retval;
}

void appl_micp_mics_dev_cp_write_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT            retval;
    APPL_MICP_NTF_INFO * mics_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    if (NULL == data)
    {
        APPL_ERR("data is NULL !\n");
    }
    else if (0U == datalen)
    {
        APPL_ERR("datalen is zero !\n");
    }
    else
    {
        mics_ntf_info = (APPL_MICP_NTF_INFO *)data;

        APPL_TRC
        (
            "\t======== MICS Mute Changed, Sending Notification ====\n"
        );

        APPL_TRC("GA_mcd_notify_mics_mute_status \n");
        retval = GA_mcd_notify_mics_mute_status
                 (
                     &(mics_ntf_info->device),
                     &appl_micp_dev_mute_val.mute,
                     sizeof(appl_micp_dev_mute_val.mute)
                 );
        APPL_TRC("Retval - ");
        appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_MICS);
    }

    return;
}

void appl_micp_aics_dev_cp_write_timer_handler(void * data, UINT16 datalen)
{
    GA_RESULT   retval;
    APPL_MICP_NTF_INFO * aics_ntf_info;

    /* Initialize */
    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected AICS Service Instance !\n");
        return;
    }

    aics_ntf_info = (APPL_MICP_NTF_INFO *)data;

    if (MICP_MAX_AICS_ENTITIES <= aics_ntf_info->srvc_inst)
    {
        /* Should not reach here */
        APPL_ERR("Invalid AICS Handle !\n");
        return;
    }

    APPL_TRC
    (
        "\t======== AICS Input State Changed, Sending Notification ====\n"
    );
    APPL_TRC("Change_Counter: 0x%02x\n",
    mcd_aics_chr_info[aics_ntf_info->srvc_inst].input_state->change_counter);

    retval = appl_micp_dev_pack_aics_chr_value
             (
                 GA_CHAR_AICS_INPUT_STATE,
                 &mcd_aics_chr_info[aics_ntf_info->srvc_inst],
                 appl_mcd_chr_data,
                 &appl_mcd_chr_datalen
             );

    APPL_TRC("GA_mcd_notify_aics_audio_input_state \n");
    retval = GA_mcd_notify_aics_audio_input_state
             (
                 &(aics_ntf_info->device),
                 aics_ntf_info->srvc_inst,
                 &appl_mcd_chr_data[0U],
                 appl_mcd_chr_datalen
             );
    APPL_TRC("Retval - ");
    appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);

    return;
}

void appl_micp_aics_rd_set_audio_inp_desc_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    APPL_MICP_NTF_INFO * aics_ntf_info;

    /* Initialize */
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(datalen);

    if (NULL == data)
    {
        APPL_ERR("Data is NULL !\nExpected AICS Service Instance !\n");
    }
    else
    {
        aics_ntf_info = (APPL_MICP_NTF_INFO *)data;

        if (MICP_MAX_AICS_ENTITIES <= aics_ntf_info->srvc_inst)
        {
            /* Should not reach here */
            APPL_ERR("Invalid AICS Handle !\n");
            return;
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MICP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();

        retval = appl_micp_dev_pack_aics_chr_value
                 (
                     GA_CHAR_AICS_AUDIO_INPUT_DESC,
                     &mcd_aics_chr_info[aics_ntf_info->srvc_inst],
                     &appl_mcd_chr_data[0U],
                     &appl_mcd_chr_datalen
                 );

        APPL_TRC("GA_mcd_notify_aics_audio_input_desc \n");
        retval = GA_mcd_notify_aics_audio_input_desc
                 (
                     &aics_ntf_info->device,
                     aics_ntf_info->srvc_inst,
                     &appl_mcd_chr_data[0U],
                     appl_mcd_chr_datalen
                 );
        APPL_TRC("Retval - ");
        appl_micp_display_ga_result(retval, MICP_SERVICE_TYPE_AICS);
    }

    return;
}

GA_RESULT appl_micp_dev_pack_aics_chr_value
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

    if ((NULL == chr_val) ||
        (NULL == chr_val_len) ||
        (NULL == aics_params))
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
    case GA_CHAR_AICS_INPUT_STATE:
        pdu[offset] = aics_params->input_state->gain_setting;
        offset += 1U;

        pdu[offset] = aics_params->input_state->mute;
        offset += 1U;

        pdu[offset] = aics_params->input_state->gain_mode;
        offset += 1U;

        pdu[offset] = aics_params->input_state->change_counter;
        offset += 1U;
        break;

    case GA_CHAR_AICS_GAIN_SETTING:
        pdu[offset] = aics_params->gain_setting->gain_setting_units;
        offset += 1U;

        pdu[offset] = aics_params->gain_setting->gain_settting_min;
        offset += 1U;

        pdu[offset] = aics_params->gain_setting->gain_setting_max;
        offset += 1U;
        break;

    case GA_CHAR_AICS_INPUT_TYPE:
        pdu[offset] = aics_params->input_type;
        offset += 1U;
        break;

    case GA_CHAR_AICS_INPUT_STATUS:
        pdu[offset] = aics_params->input_status;
        offset += 1U;
        break;

    case GA_CHAR_AICS_AUDIO_INPUT_DESC:
        GA_mem_copy
        (
            &pdu[offset],
            aics_params->audio_in_desc,
            aics_params->audio_in_desc_len
        );
        offset += aics_params->audio_in_desc_len;
        break;

    default:
        /* Should not reach here */
        APPL_ERR("Invalid char UUID (0x%02X) !\n", chr_uuid);
        retval = GA_FAILURE;
        break;
    }

    (*chr_val_len) = offset;

    return retval;
}

GA_RESULT appl_micp_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT  retval;

    /*Initialize*/
    retval = GA_SUCCESS;

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    if (NULL == ga_data)
    {
        APPL_ERR("ga_data is NULL !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("MICP Server");

    switch(ga_event)
    {
        case GA_MC_SET_MUTE_IND:
            APPL_TRC("[Profile]     : MICS (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICS DEVICE EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICS DEVICE-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_micp_dev_callback
                    (
                        device,
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_AICS_CP_WT_IND: /* Fall - through */
        case GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND:
            APPL_TRC("[Profile]     : MICS-AICS (0x%4X)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICS-AICS DEVICE EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICS-AICS DEVICE-WRITE\n");
            if(GA_MC_AICS_SET_AUDIO_INPUT_DESC_IND == ga_event)
            {
                APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            }
            else
            {
                APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            }
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_micp_dev_callback
                    (
                        device,
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_RD_IND:
            APPL_TRC("[Profile]     : MICS (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICS DEVICE EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICS DEVICE-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_micp_dev_callback
                    (
                        device,
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_AICS_RD_IND:
            APPL_TRC("[Profile]     : MICS-AICS (0x%4X)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICS-AICS DEVICE EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICS-AICS DEVICE-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_micp_dev_callback
                    (
                        device,
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        default:
            APPL_ERR("UnHandled MICP Event (0x%02X) !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

GA_RESULT appl_micp_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT  retval;

    /*Initialize*/
    retval = GA_SUCCESS;

    if (NULL == ga_device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    if (NULL == ga_data)
    {
        APPL_ERR("ga_data is NULL !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("MICP Client");

    switch(ga_event)
    {
        case GA_MC_SETUP_CNF:
            APPL_TRC("[Profile]     : MICP (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP CONTROLLER-SETUP\n");
            APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_RELEASE_CNF:
            APPL_TRC("[Profile]     : MICP (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP CONTROLLER-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_RELEASE_CAPABILITY_CNF:
            APPL_TRC("[Profile]     : MICP-AICS (0x%4x)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICP-AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP-AICS CONTROLLER-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_GET_CAPABILITIES_CNF: /* Fall - through */
        case GA_MC_SET_CAPABILITIES_CNF:
            APPL_TRC("[Profile]     : MICP-AICS (0x%4x)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICP-AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP-AICS CONTROLLER-SETUP\n");
            if(GA_MC_GET_CAPABILITIES_CNF == ga_event)
            {
                APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
            }
            else
            {
                APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            }
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_GET_MUTE_CNF:
            APPL_TRC("[Profile]     : MICP (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP CONTROLLER-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_MUTE_NTF:
            APPL_TRC("[Profile]     : MICP (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP CONTROLLER-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_SET_MUTE_CNF:
            APPL_TRC("[Profile]     : MICP (0x%4X)\n",GA_SERVICE_MICS);
            APPL_TRC("[SECTION]     : MICP CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP CONTROLLER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_AICS_CP_WT_CNF:
            APPL_TRC("[Profile]     : MICP-AICS (0x%4x)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICP-AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP-AICS CONTROLLER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_AICS_GET_INPUT_STATE_CNF: /* Fall - through */
        case GA_MC_AICS_GET_GAIN_SETTING_PROPRTY_CNF: /* Fall - through */
        case GA_MC_AICS_GET_INPUT_TYPE_CNF: /* Fall - through */
        case GA_MC_AICS_GET_INPUT_STATUS_CNF: /* Fall - through */
        case GA_MC_AICS_GET_AUDIO_INPUT_DESC_CNF:
            APPL_TRC("[Profile]     : MICP-AICS (0x%4x)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICP-AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP-AICS CONTROLLER-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        case GA_MC_AICS_INPUT_STATE_NTF: /* Fall - through */
        case GA_MC_AICS_INPUT_STATUS_NTF:  /* Fall - through */
        case GA_MC_AICS_AUDIO_INPUT_DESC_NTF:
            APPL_TRC("[Profile]     : MICP-AICS (0x%4x)\n",GA_SERVICE_AICS);
            APPL_TRC("[SECTION]     : MICP-AICS CONTROLLER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : MICP-AICS CONTROLLER-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_micp_ct_callback
                    (
                        ga_event,
                        ga_status,
                        ga_data,
                        ga_datalen
                    );
            break;

        default:
            APPL_ERR("UnHandled MICP Event (0x%02X) !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

void appl_micp_display_ga_result(GA_RESULT status, UINT8 service)
{
    GA_RESULT  retval;

    retval = GA_SUCCESS;

    /* checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(status);

    if (GA_FAILURE == retval)
    {
        retval = GA_SUCCESS;
        switch (status)
        {
        /* Implementation related error codes */
        case GA_MICP_INVALID_STATE:
            APPL_TRC("Invalid State!");
            break;

        case GA_MICP_INVALID_PARAMS:
            APPL_TRC("Invalid Params!");
            break;

        case GA_MICP_CNTX_ALLOC_FAILED:
            APPL_TRC("Context allocation failed!");
            break;

        case GA_MICP_IN_BUSY_STATE:
            APPL_TRC("In Busy State!");
            break;

        case GA_MICP_INVALID_HANDLE:
            APPL_TRC("Invalid Handle!");
            break;

        case GA_MICP_OPERATION_NOT_SUPPORTED_BY_PEER_DEVICE:
            APPL_TRC("Operation not supported by Peer Device!");
            break;

        case GA_MICP_SERVICE_ID_NOT_FOUND:
            APPL_TRC("Service ID not found!");
            break;

        case GA_MICP_INVALID_SERVICE_TYPE:
            APPL_TRC("Invalid Service Type!");
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
            if (MICP_SERVICE_TYPE_MICS == service)
            {
                appl_micp_display_mics_appl_err_codes(status);
            }
            else if (MICP_SERVICE_TYPE_AICS == service)
            {
                appl_aics_display_appl_err_codes(status);
            }
            else
            {
                APPL_TRC("Invalid Service: 0x%04X\n", service);
            }
        }
    }
    return;
}

void appl_micp_display_mics_appl_err_codes(GA_RESULT status)
{
    switch (status)
    {
    /* Application error code */
    case MICS_ERR_CODE_MUTE_DISABLED:
        APPL_TRC("Mute Disabled!");
        break;

    case MICS_ERR_CODE_VALUE_OUT_OF_RANGE:
        APPL_TRC("Value Out of Range!");
        break;

    default:
        APPL_TRC("Invalid Code:");
        break;
    }

    APPL_TRC(" (0x%04X)\n", status);
    return;
}

#endif /* GA_MICP */
