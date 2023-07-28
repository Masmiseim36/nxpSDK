/**
 *  \file appl_ga.c
 *
 *  Reference application for GA Core interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga.h"

#include "appl_vcp.h"
#include "appl_micp.h"
#include "appl_csip.h"

#include "appl_ga_uc.h"
#include "appl_ga_bc.h"

#include "appl_mcp_ce.h"
#include "appl_mcp_se.h"

#include "appl_ccp_ce.h"
#include "appl_ccp_se.h"

#include "appl_tmap.h"
#include "appl_hap.h"

#include "appl_ga_utils_hci.h"

#include "appl_ga_utils_audio_src.h"
#include "appl_ga_utils_audio_snk.h"

#ifdef BT_GAM

/* Used to differentiate ongoing scan */
extern UINT8 is_scan_enabled;

/* Used to differentiate ongoing advertise */
extern UINT8 is_adv_enabled;

static const char ga_menu_options[] = " \n\
================ GA Main MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
================ Profile Management ~ Init ============ \n\
    2. GA Init. \n\
    3. GA Setup. \n\
\n\
================ Profile Management ~ Deinit ============ \n\
    4. GA Shutdown. \n\
\n\
================ GA PACS SERVER MENU ================ \n\
================ Service Management ~ Setup ============ \n\
    5. GA Register - PACS. \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
    6. Update/Notify PACS - Audio Capability. \n\
    7. Update/Notify PACS - Available Audio Contexts. \n\
    8. Update/Notify PACS - Supported Audio Contexts. \n\
    9. Update/Notify PACS - Audio Locations. \n\
\n\
================ GA PACS CLIENT MENU ================ \n\
================ Context Management ~ Setup ============ \n\
   10. Setup PACS - Audio Role Discovery. \n\
\n\
================ Context Management ~ Release ============ \n\
   11. Release/Close PACS. \n\
\n\
================ Profile Management (PACS) ~ Operations(Read) ============ \n\
   12. Get PACS Capabilities - Audio Capability, Location, Supp/Avail Contexts \n\
\n\
================ Device Management ~ LE Advertise ============ \n\
   20. Enable/Disable LE Advertising - Legacy. [CAP - Acceptor, GAP - Peripheral] \n\
   21. Enable/Disable LE Advertising - Extended. [CAP - Acceptor, GAP - Peripheral] \n\
\n\
================ Device Management ~ LE Scan ============ \n\
   22. Enable/Disable LE Scan - Legacy. [CAP - Initiator, GAP - Central] \n\
   23. Enable/Disable LE Scan - Extended. [CAP - Initiator GAP - Central]  \n\
\n\
================ Device Management ~ LE Connection ============ \n\
   25. LE Create Connection. [CAP - Initiator GAP - Central] \n\
   26. LE Extended Create Connection. [CAP - Initiator GAP - Central]  \n\
\n\
================ Device Management ~ Authentication ============ \n\
   27. LE Legacy Authenticate and Encrypt. \n\
   28. LE LESC Authenticate and Encrypt. \n\
\n\
================ Device Management ~ Disconnection ============ \n\
   29. Disconnection. \n\
\n\
================ Device Management ~ Setting ============ \n\
   30. Display all Connected Devices. \n\
   31. Set Connected Device Index. \n\
   32. Remove Device from Device Manager. \n\
\n\
================ GA Config Audio Dump MENU ================ \n\
   35. Configure Sink Audio Dump. \n\
   36. Configure Source Audio Dump. \n\
\n\
================ GA Sub MENU ================ \n\
   40. Unicast Operations. \n\
   45. Broadcast Operations. \n\
\n\
================ Capture and Rendering Control MENU ================ \n\
   50. VCP Volume Controller Operations. \n\
   51. VCP Volume Renderer Operations. \n\
\n\
   55. MICP Microphone Controller Operations. \n\
   56. MICP Microphone Device Operations. \n\
\n\
================ GA CSIP MENU ================ \n\
   60. CSIP Set Coordinator Operations. \n\
   61. CSIP Set Member Operations. \n\
\n\
================ Content Control MENU ================ \n\
   65. MCP Media Control Client. \n\
   66. MCP Media Control Server. \n\
\n\
   70. CCP Call Control Client. \n\
   71. CCP Call Control Server. \n\
\n\
================ Device Application MENU ============= \n\
   75. GA TMAP Operations. \n\
   76. GA HAP Operations. \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get Context Info. \n\
   81. Set Context Info. \n\
\n\
   87. Free Saved Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure Notification \n\
\n\
Your Option ?: ";

#ifdef BAP_SUPPORT_CONTEXT_MANAGE
DECL_STATIC GA_BRR_SVC_RANGE bap_svc_range[BAP_SERVICE_COUNT];
DECL_STATIC GA_CHAR_INFO* bap_char_info;
DECL_STATIC GA_BRR_SVC_RANGE* bap_svc_range_ptr;
DECL_STATIC GA_CHAR_INFO* bap_char_info_ptr;
DECL_STATIC UINT16 bap_total_char_count;
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* Session role selected for GA during Init */
DECL_STATIC UCHAR appl_ga_role;

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_cap_uc_client_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT          retval;
    UINT8              role;
    GA_CAP_ENDPOINT  * capab;

    GA_CB_ENTRY("Client");

    /* Initialize */
    retval = GA_SUCCESS;
    role = 0U;
    APPL_DBG("[APPL][GA]: >> appl_ga_cap_uc_client_callback\n");

    switch (ga_event)
    {
    case GA_SETUP_CNF:
        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        APPL_TRC
        (
            "[Event]       : GA_SETUP_CNF (0x%02X)\n",
            ga_event
        );
        if (GA_SUCCESS == ga_status)
        {
            role = *((UINT8*)ga_data);
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Supported ", (UCHAR)sizeof(UINT8));
            appl_ga_utils_bap_display_role(role);
        }
        break;

    case GA_RELEASE_CNF:
        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        APPL_TRC
        (
            "[Event]       : GA_RELEASE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_CAPABILITIES_CHANGE_NTF: /* Fall-through */
    case GA_GET_CAPABILITIES_CNF:
        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        if (GA_GET_CAPABILITIES_CNF == ga_event)
        {
            APPL_TRC
            (
                "[Event]       : GA_GET_CAPABILITIES_CNF (0x%02X)\n",
                ga_event
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : GA_CAPABILITIES_CHANGE_NTF (0x%02X)\n",
                ga_event
            );
        }

        capab = (GA_CAP_ENDPOINT *)ga_data;
        /*
         * Add Remote PACS data: Context can be passed as NULL here since it is
         * already added during GA_Setup() .
         */
        retval = appl_ga_utils_bap_add_remote_pacs_for_dev
                 (
                     device,
                     capab
                 );
        break;

    default:
        retval = appl_ga_uc_client_cb_handler
                 (
                     device,
                     ga_event,
                     ga_status,
                     ga_data,
                     ga_datalen
                 );
        break;
    }

    GA_CB_EXIT();

    APPL_DBG("[APPL][GA]: << appl_ga_cap_uc_client_callback\n");

    return retval;
}

GA_RESULT appl_ga_cap_uc_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA]: >> appl_ga_cap_uc_server_callback\n");

    retval = appl_ga_uc_server_cb_handler
             (
                 device,
                 ga_event,
                 ga_status,
                 ga_data,
                 ga_datalen
             );

    APPL_DBG("[APPL][GA]: << appl_ga_cap_uc_server_callback\n");

    return retval;
}

GA_RESULT appl_ga_cap_bc_src_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA]: >> appl_ga_cap_bc_src_callback\n");

    retval = appl_ga_bc_src_cb_handler
             (
                 device,
                 ga_event,
                 ga_status,
                 ga_data,
                 ga_datalen
             );

    APPL_DBG("[APPL][GA]: << appl_ga_cap_bc_src_callback\n");

    return retval;
}

GA_RESULT appl_ga_cap_bc_sink_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA]: >> appl_ga_cap_bc_sink_callback\n");

    retval = appl_ga_bc_sink_cb_handler
             (
                 device,
                 ga_event,
                 ga_status,
                 ga_data,
                 ga_datalen
             );

    APPL_DBG("[APPL][GA]: << appl_ga_cap_bc_sink_callback\n");

    return retval;
}

int main_ga_operations(void)
{
    GA_RESULT    retval;
    int          choice;
    UINT8        menu_choice;
    GA_ENDPOINT  device, *pdevice;
    UINT8        index;
    GA_CONTEXT   context;
#ifdef GA_HAVE_DYNAMIC_CONFIG
    GA_CONFIG    appl_ga_config;
#endif /* GA_HAVE_DYNAMIC_CONFIG */
    GA_BRR_EXTENDED_SCAN_PARAMS escan_params;

    UINT16 ctx, *pctx;
    UINT32 loc, *ploc;
    UINT8 role;

#ifdef APPL_CS_MEMBER
    GA_SET_MEMBER member;
    UCHAR appl_cs_sirk[CSIP_SIRK_LEN] = APPL_CS_SIRK;
#endif /* APPL_CS_MEMBER */

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;
    pctx = NULL;
    ploc = NULL;
    pdevice = NULL;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", ga_menu_options);
        CONSOLE_IN("%d", &choice);

        menu_choice = (UINT8)choice;

        switch (choice)
        {
        case 0:
            break; /* return 0; */

        case 1:
            break;

        case 2:
            if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                APPL_ERR("GA is already initialized with Reference application - Legacy !\n");
            }
            else if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                APPL_ERR("GA is already initialized with Reference application - New !\n");
            }
            else
            {
                CONSOLE_OUT("Enter the Role (1-Client, 2-Server, 3-Both): ");
                CONSOLE_IN("%d", &choice);

                /* Save the GA role chosen */
                appl_ga_role = (UCHAR)choice;

                /* Register with the bearer interface */
                ga_brr_register();

                retval = appl_ga_utils_hci_register_callback();
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("HCI Callback Registered\n");
                }
                else
                {
                    APPL_ERR
                    (
                        "FAILED to register HCI Callback !!! Error code = %04X\n",
                        retval
                    );
                }

#ifdef GA_HAVE_DYNAMIC_CONFIG
                APPL_TRC("GA_configure: \n");
                GA_INIT_CONFIG(appl_ga_config);
                GA_configure(&appl_ga_config);
#endif /* GA_HAVE_DYNAMIC_CONFIG */

                APPL_TRC("GA_init\n");
                retval = GA_init();
                APPL_TRC("Retval - ");
                if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
                {
                    APPL_TRC("0x%04X\n", retval);
                }

                if (GA_SUCCESS == retval)
                {
                    ga_init_status = APPL_GA_INIT_STATUS_GA;

                    /* Initialize Device Manager */
                    appl_ga_utils_dev_manager_init();

                    /* Initialize ISO Module - Source */
                    appl_ga_utils_audio_src_init();
                    /* Initialize ISO Module - Sink */
                    appl_ga_utils_audio_snk_init();

                    if (1U == appl_ga_role)
                    {
                        /* Client only Initialization */
                        retval = GA_uc_client_init(appl_ga_cap_uc_client_callback);
                        APPL_TRC("GA_uc_client_init. Retval - 0x%04X\n", retval);

                        /* Initialize Unicast App Client Module */
                        appl_ga_uc_init(APPL_GA_UNICAST_CLIENT);

                        appl_ga_utils_pacs_init();
#ifdef APPL_VCP_CONTROLLER
                        retval = GA_vc_client_init(appl_vcp_client_callback);
                        APPL_TRC("GA_vc_client_init. Retval - 0x%04X\n", retval);
                        appl_vcp_ct_set_default_values();
#endif /* APPL_VCP_CONTROLLER */
#ifdef APPL_MICP_CONTROLLER
                        retval = GA_mc_client_init(appl_micp_client_callback);
                        APPL_TRC("GA_mc_client_init. Retval - 0x%04X\n", retval);
                        appl_micp_ct_set_default_values();
#endif /* APPL_MICP_CONTROLLER */
#ifdef APPL_BAP_BROADCAST_SOURCE
                        retval = GA_bc_source_init(appl_ga_cap_bc_src_callback);
                        APPL_TRC("GA_bc_source_init. Retval - 0x%04X\n", retval);
#endif /* APPL_BAP_BROADCAST_SOURCE */
#ifdef APPL_GA_CSIP
                        retval = GA_cs_client_init(appl_csip_client_callback);
                        APPL_TRC("GA_cs_client_init. Retval - 0x%04X\n", retval);
#endif /* APPL_GA_CSIP */

                        /* Prepare the CAP role and CS Member if required */
                        retval = GA_prepare((CAP_ROLE_INITIATOR | CAP_ROLE_COMMANDER), NULL);
                        APPL_TRC("GA_prepare. Retval - 0x%04X\n", retval);
                    }
                    else if (2U == appl_ga_role)
                    {
                        /* Server only Initialization */
                        retval = GA_uc_server_init(appl_ga_cap_uc_server_callback);
                        APPL_TRC("GA_uc_server_init. Retval - 0x%04X\n", retval);

                        /* Initialize Unicast App Server Module */
                        appl_ga_uc_init(APPL_GA_UNICAST_SERVER);
#ifdef APPL_VCP_RENDERER
                        retval = GA_vc_server_init(appl_vcp_server_callback);
                        APPL_TRC("GA_vc_server_init. Retval - 0x%04X\n", retval);
                        appl_vcp_rd_set_default_values();
#endif /* APPL_VCP_RENDERER */
#ifdef APPL_MICP_DEVICE
                        retval = GA_mc_server_init(appl_micp_server_callback);
                        APPL_TRC("GA_mc_server_init. Retval - 0x%04X\n", retval);
                        appl_micp_dev_set_default_values();
#endif /* APPL_MICP_DEVICE */
#ifdef APPL_BAP_BROADCAST_SINK
                        retval = GA_bc_sink_init(appl_ga_cap_bc_sink_callback);
                        APPL_TRC("GA_bc_sink_init. Retval - 0x%04X\n", retval);
                        appl_ga_bc_init();
#endif /* APPL_BAP_BROADCAST_SINK */
                        retval = GA_cs_server_init(appl_csip_server_callback);
                        APPL_TRC("GA_cs_server_init. Retval - 0x%04X\n", retval);

                        /* Prepare the CAP role and CS Member if required */
                        retval = GA_prepare(CAP_ROLE_ACCEPTOR, NULL);
                        APPL_TRC("GA_prepare. Retval - 0x%04X\n", retval);
                    }
                    else if (3U == appl_ga_role)
                    {
                        /* Initialize Unicast core */
                        (GA_IGNORE_RETURN_VALUE)GA_uc_client_init(appl_ga_cap_uc_client_callback);
                        (GA_IGNORE_RETURN_VALUE)GA_uc_server_init(appl_ga_cap_uc_server_callback);

                        /* Initialize Unicast App Client+Server Module */
                        appl_ga_uc_init(APPL_GA_UNICAST_CLIENT | APPL_GA_UNICAST_SERVER);

                        /* Initialize PACS Module - Client */
                        appl_ga_utils_pacs_init();

                        /* Initialize Broadcast core */
#ifdef APPL_BAP_BROADCAST_SOURCE
                        (GA_IGNORE_RETURN_VALUE)GA_bc_source_init(appl_ga_cap_bc_src_callback);
#endif /* APPL_BAP_BROADCAST_SOURCE */
#ifdef APPL_BAP_BROADCAST_SINK
                        (GA_IGNORE_RETURN_VALUE)GA_bc_sink_init(appl_ga_cap_bc_sink_callback);
#endif /* APPL_BAP_BROADCAST_SINK */

                        /* Initialize Broadcast Module */
                        appl_ga_bc_init();

                        /* Initialize Audio PL Module */
                        /* Initialize LC3 Module */

                    /* Initialize Volume Control core */
#ifdef APPL_VCP_CONTROLLER
                        (GA_IGNORE_RETURN_VALUE)GA_vc_client_init(appl_vcp_client_callback);
#endif /* APPL_VCP_CONTROLLER */
#ifdef APPL_VCP_RENDERER
                        (GA_IGNORE_RETURN_VALUE)GA_vc_server_init(appl_vcp_server_callback);
#endif /* APPL_VCP_RENDERER */

                        /* Initialize VCP Controller */
                        appl_vcp_ct_set_default_values();
                        /* Initialize VCP Renderer */
                        appl_vcp_rd_set_default_values();

                        /* Initialize Microphone Control core */
#ifdef APPL_MICP_CONTROLLER
                        (GA_IGNORE_RETURN_VALUE)GA_mc_client_init(appl_micp_client_callback);
#endif /* APPL_MICP_CONTROLLER */
#ifdef APPL_MICP_DEVICE
                        (GA_IGNORE_RETURN_VALUE)GA_mc_server_init(appl_micp_server_callback);
#endif /* APPL_MICP_DEVICE */

                        /* Initialize MICP Device */
                        appl_micp_dev_set_default_values();
                        /* Initialize MICP Controller */
                        appl_micp_ct_set_default_values();

                        /* Initialize Coordinated Set core */
#ifdef APPL_GA_CSIP
                        (GA_IGNORE_RETURN_VALUE)GA_cs_client_init(appl_csip_client_callback);
                        (GA_IGNORE_RETURN_VALUE)GA_cs_server_init(appl_csip_server_callback);

#ifndef APPL_CS_MEMBER
                        /* Prepare the CAP role and CS Member if required */
                        (GA_IGNORE_RETURN_VALUE) GA_prepare
                                                 (
                                                     (CAP_ROLE_INITIATOR | CAP_ROLE_ACCEPTOR | CAP_ROLE_COMMANDER),
                                                     NULL
                                                 );
#else /* APPL_CS_MEMBER */
                        /* Set the SIRK Type and Value */
                        member.sirk[0] = APPL_CS_SIRK_TYPE;
                        GA_mem_copy(&member.sirk[1], appl_cs_sirk, CSIP_SIRK_LEN);

                        /* Get the Set Size */
                        CONSOLE_OUT("Enter the Coordinated Set Size: ");
                        CONSOLE_IN("%d", &choice);
                        member.size = (UINT8)choice;

                        /* Get the Member Rank */
                        CONSOLE_OUT("Enter the Coordinated Set Rank for this member: ");
                        CONSOLE_IN("%d", &choice);
                        member.rank = (UINT8)choice;

                        /* Prepare the CAP role and CS Member if required */
                        (GA_IGNORE_RETURN_VALUE) GA_prepare
                                                 (
                                                     (CAP_ROLE_INITIATOR | CAP_ROLE_ACCEPTOR | CAP_ROLE_COMMANDER),
                                                     &member
                                                 );
#endif /* APPL_CS_MEMBER */
#endif /* APPL_GA_CSIP */
                    }

                    /*
                     * Initialize Host Features - Connected Isochronous Stream
                     * (Host Support)
                     */
                    appl_ga_utils_hci_set_host_feature(0x20U, 0x01U);
                }
            }
            break;

        case 3:
#ifdef GATT_DB_DYNAMIC
        {
            UINT32 appl_ga_signature = 0x1234FFFFU;
            retval = ga_brr_dyn_gatt_db_init_pl(&appl_ga_signature);
            APPL_TRC("GA Setup - ");
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
        }
#endif /* GATT_DB_DYNAMIC */
            break;

        case 4:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                if (1U == appl_ga_role)
                {
                    /* De-Initialize Unicast core */
                    retval = GA_uc_client_shutdown();
                    APPL_TRC("GA_uc_client_shutdown. Retval - 0x%04X\n", retval);

#ifdef APPL_BAP_BROADCAST_SOURCE
                    retval = GA_bc_source_shutdown();
                    APPL_TRC("GA_bc_source_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_BAP_BROADCAST_SOURCE */

#ifdef APPL_VCP_CONTROLLER
                    retval = GA_vc_client_shutdown();
                    APPL_TRC("GA_vc_client_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_VCP_CONTROLLER */

#ifdef APPL_MICP_CONTROLLER
                    retval = GA_mc_client_shutdown();
                    APPL_TRC("GA_mc_client_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_MICP_CONTROLLER */

#ifdef APPL_GA_CSIP
                    retval = GA_cs_client_shutdown();
                    APPL_TRC("GA_cs_client_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_GA_CSIP */
                }
                else if (2U == appl_ga_role)
                {
                    /* De-Initialize Unicast core */
                    retval = GA_uc_server_shutdown();
                    APPL_TRC("GA_uc_server_shutdown. Retval - 0x%04X\n", retval);

#ifdef APPL_BAP_BROADCAST_SINK
                    retval = GA_bc_sink_shutdown();
                    APPL_TRC("GA_bc_sink_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_BAP_BROADCAST_SINK */

#ifdef APPL_VCP_RENDERER
                    retval = GA_vc_server_shutdown();
                    APPL_TRC("GA_vc_server_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_VCP_RENDERER */

#ifdef APPL_MICP_DEVICE
                    retval = GA_mc_server_shutdown();
                    APPL_TRC("GA_mc_server_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_MICP_DEVICE */

#ifdef APPL_GA_CSIP
                    retval = GA_cs_server_shutdown();
                    APPL_TRC("GA_cs_server_shutdown. Retval - 0x%04X\n", retval);
#endif /* APPL_GA_CSIP */
                }
                else if (3U == appl_ga_role)
                {
                    /* De-Initialize Unicast core */
                    (GA_IGNORE_RETURN_VALUE)GA_uc_client_shutdown();
                    (GA_IGNORE_RETURN_VALUE)GA_uc_server_shutdown();

#ifdef APPL_BAP_BROADCAST_SOURCE
                    (GA_IGNORE_RETURN_VALUE)GA_bc_source_shutdown();
#endif /* APPL_BAP_BROADCAST_SOURCE */
#ifdef APPL_BAP_BROADCAST_SINK
                    (GA_IGNORE_RETURN_VALUE)GA_bc_sink_shutdown();
#endif /* APPL_BAP_BROADCAST_SINK */

#ifdef APPL_VCP_CONTROLLER
                    (GA_IGNORE_RETURN_VALUE)GA_vc_client_shutdown();
#endif /* APPL_VCP_CONTROLLER */
#ifdef APPL_VCP_RENDERER
                    (GA_IGNORE_RETURN_VALUE)GA_vc_server_shutdown();
#endif /* APPL_VCP_RENDERER */

#ifdef APPL_MICP_CONTROLLER
                    (GA_IGNORE_RETURN_VALUE)GA_mc_client_shutdown();
#endif /* APPL_MICP_CONTROLLER */
#ifdef APPL_MICP_DEVICE
                    (GA_IGNORE_RETURN_VALUE)GA_mc_server_shutdown();
#endif /* APPL_MICP_DEVICE */

#ifdef APPL_GA_CSIP
                    (GA_IGNORE_RETURN_VALUE)GA_cs_client_shutdown();
                    (GA_IGNORE_RETURN_VALUE)GA_cs_server_shutdown();
#endif /* APPL_GA_CSIP */
                }

                APPL_TRC("GA Shutdown...\n");
                retval = GA_shutdown();
                APPL_TRC("Retval - ");
                if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
                {
                    APPL_TRC("0x%04X\n", retval);
                }
                if (GA_SUCCESS == retval)
                {
                    ga_init_status = APPL_GA_INIT_STATUS_NONE;
                    /* Fetch the connected device index and info */
                    appl_ga_utils_get_conn_device_in_use
                    (
                        &index,
                        NULL,
                        NULL
                    );
                    /* Reset the context in device manager */
                    appl_ga_utils_update_context_of_conn_dev(0U, index);

                    retval = appl_ga_utils_hci_unregister_callback();
                    if (GA_SUCCESS == retval)
                    {
                        APPL_TRC("HCI Callback UnRegistered\n");
                    }
                    else
                    {
                        APPL_ERR
                        (
                            "FAILED to unregister HCI Callback !!! Error code = %04X\n",
                            retval
                        );
                    }
                }

                /* De-Initialize ISO Module */
                /* De-Initialize Audio PL Module */
                /* De-Initialize LC3 Module */
            }
            else
            {
                APPL_ERR("Invalid status for GA_shutdown() %d!\n",
                ga_init_status);
            }
            break;

        case 5:
            retval = appl_ga_register_pacs(APPL_GA_ROLE_NONE);
            break;

        case 6:
            break;

        case 7:
            pctx = NULL;
            pdevice = NULL;
            role = appl_ga_utils_bap_get_role();

            /* Take Input if we want to Update/Notify/Both */
            choice = appl_ga_utils_bap_get_choice_for_update_notify();
            if ((0x01 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                ctx = appl_ga_utils_bap_get_available_ctx();
                pctx = &ctx;
            }
            if ((0x02 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                /* Fetch the connected device index and info */
                appl_ga_utils_get_conn_device_in_use
                (
                    &index,
                    &device,
                    NULL
                );
                pdevice = &device;
            }

            if (GA_ROLE_SOURCE == role)
            {
                retval = GA_update_source_available_audio_contexts(pdevice, pctx);
            }
            else if (GA_ROLE_SINK == role)
            {
                retval = GA_update_sink_available_audio_contexts(pdevice, pctx);
            }
            else
            {
                APPL_ERR("Invalid Role - 0x%02X\n", role);
            }

            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 8:
            pctx = NULL;
            pdevice = NULL;
            role = appl_ga_utils_bap_get_role();

            /* Take Input if we want to Update/Notify/Both */
            choice = appl_ga_utils_bap_get_choice_for_update_notify();
            if ((0x01 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                ctx = appl_ga_utils_bap_get_supp_ctx();
                pctx = &ctx;
            }
            if ((0x02 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                /* Fetch the connected device index and info */
                appl_ga_utils_get_conn_device_in_use
                (
                    &index,
                    &device,
                    NULL
                );
                pdevice = &device;
            }

            if (GA_ROLE_SOURCE == role)
            {
                retval = GA_update_source_supported_audio_contexts(pdevice, pctx);
            }
            else if (GA_ROLE_SINK == role)
            {
                retval = GA_update_sink_supported_audio_contexts(pdevice, pctx);
            }
            else
            {
                APPL_ERR("Invalid Role - 0x%02X\n", role);
            }

            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 9:
            ploc = NULL;
            pdevice = NULL;
            role = appl_ga_utils_bap_get_role();

            /* Take Input if we want to Update/Notify/Both */
            choice = appl_ga_utils_bap_get_choice_for_update_notify();
            if ((0x01 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                loc = appl_ga_utils_bap_get_audio_channel_alloc();
                ploc = &loc;
            }
            if ((0x02 == (UINT8)choice) ||
                (0x03 == (UINT8)choice))
            {
                /* Fetch the connected device index and info */
                appl_ga_utils_get_conn_device_in_use
                (
                    &index,
                    &device,
                    NULL
                );
                pdevice = &device;
            }

            if (GA_ROLE_SOURCE == role)
            {
                retval = GA_update_source_audio_locations(pdevice, ploc);
            }
            else if (GA_ROLE_SINK == role)
            {
                retval = GA_update_sink_audio_locations(pdevice, ploc);
            }
            else
            {
                APPL_ERR("Invalid Role - 0x%02X\n", role);
            }

            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 10:
            /* Fetch the connected device index and info */
            appl_ga_utils_get_conn_device_in_use
            (
                &index,
                &device,
                NULL
            );

            /* Remove reference to old records */
            retval = appl_ga_utils_bap_remove_remote_pacs_for_dev(index);

            APPL_TRC("Setup Endpoint\n");
            APPL_TRC("Setup PACS: Audio Role Discovery - PACS Discovery\n");
            retval = GA_setup(&device, &context);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            APPL_TRC("Context: %d\n", context);

            if (GA_SUCCESS == retval)
            {
                /* Set the context in device manager */
                appl_ga_utils_update_context_of_conn_dev
                (
                    context,
                    index
                );
            }
            break;

        case 11:
        {
            /* Fetch the connected device index, context and info */
            appl_ga_utils_get_conn_device_in_use
            (
                &index,
                &device,
                &context
            );
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_release\n");
                retval = GA_release(&context);
                APPL_TRC("Retval - ");
                if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
                {
                    APPL_TRC("0x%04X\n", retval);
                }
                if (GA_SUCCESS == retval)
                {
                    /* Reset the context in device manager */
                    appl_ga_utils_update_context_of_conn_dev(0U, index);
                }
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_close\n");
                retval = GA_close(&context);
                APPL_TRC("Retval - ");
                if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
                {
                    APPL_TRC("0x%04X\n", retval);
                }
                if (GA_SUCCESS == retval)
                {
                    /* Reset the context in device manager */
                    appl_ga_utils_update_context_of_conn_dev(0U, index);
                }
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 12:
            /* Fetch the connected device index, context and info */
            appl_ga_utils_get_conn_device_in_use
            (
                &index,
                &device,
                &context
            );

            role = appl_ga_utils_bap_get_role();
            APPL_TRC
            (
                "Get PACS Capabilities - Read Audio Capability, Location, "
                "Supp/Avail Contexts\n"
            );
            retval = GA_get_capabilities(&context, role);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 20:
            CONSOLE_OUT("1. Enable 2. Disable\n");
            CONSOLE_IN("%d", &choice);
            index = (UINT8)choice;
            if (1U == index)
            {
                /* Reusing index to indicate enable */
                retval = appl_ga_utils_enable_legacy_advertise(GA_TRUE);
                if (GA_SUCCESS == retval)
                {
                    is_adv_enabled = APPL_GA_ADV_UCS;
                }
                else
                {
                    APPL_ERR("Legacy Advertise Already initiated by Other Module !\n");
                }
            }
            else if (2U == index)
            {
                /* Reusing index to indicate disable */
                retval = appl_ga_utils_enable_legacy_advertise(GA_FALSE);
                if (GA_SUCCESS == retval)
                {
                    is_adv_enabled = APPL_GA_ADV_INVALID;
                }
                else
                {
                    APPL_ERR("Failed to Disable Legacy Advertise Or Advertise Already Stopped"
                        " by Other Module !\n");
                }
            }
            else
            {

            }
            break;

        case 21:
            CONSOLE_OUT("1. Enable 2. Disable\n");
            CONSOLE_IN("%d", &choice);
            index = (UINT8)choice;
            if (1U == index)
            {
                /* Reusing index to indicate enable */
                retval = appl_ga_utils_enable_extended_advertise(GA_TRUE);
                if (GA_SUCCESS == retval)
                {
                    is_adv_enabled = APPL_GA_ADV_UCS;
                }
                else
                {
                    APPL_ERR("Extended Advertise Already initiated by Other Module !\n");
                }
            }
            else if (2U == index)
            {
                /* Reusing index to indicate disable */
                retval = appl_ga_utils_enable_extended_advertise(GA_FALSE);
                if (GA_SUCCESS == retval)
                {
                    is_adv_enabled = APPL_GA_ADV_INVALID;
                }
                else
                {
                    APPL_ERR("Failed to Disable Extended Advertise Or Advertise Already Stopped"
                        " by Other Module !\n");
                }
            }
            else
            {

            }
            break;

        case 22:
            CONSOLE_OUT("1. Enable 2. Disable\n");
            CONSOLE_IN("%d", &choice);
            index = (UINT8)choice;
            if (0x01U == index)
            {
                retval = appl_ga_utils_enable_legacy_scan(GA_TRUE);
                if (GA_SUCCESS == retval)
                {
                    is_scan_enabled = APPL_GA_SCAN_FOR_UCS;
                }
                else
                {
                    APPL_ERR("Failed to set Legacy Scan Params Or Scan Already initiated"
                        " by Other Module !\n");
                }
            }
            else
            {
                retval = appl_ga_utils_enable_legacy_scan(GA_FALSE);
                if (GA_SUCCESS == retval)
                {
                    is_scan_enabled = APPL_GA_SCAN_INVALID;
                }
                else
                {
                    APPL_ERR("Failed to Disable Extended Scan Or Scan Already Stopped"
                        " by Other Module !\n");
                }
            }
            break;

        case 23:
            CONSOLE_OUT("1. Enable 2. Disable\n");
            CONSOLE_IN("%d", &choice);
            index = (UINT8)choice;
            escan_params.duration = 0x00;
            escan_params.filter_duplicates = 0x01;
            escan_params.own_addr_type = 0x00;
            escan_params.period = 0x00;
            escan_params.scan_filter_policy = 0x00;
            escan_params.scan_interval = 0x00A0;
            escan_params.scan_phy = 0x01;
            escan_params.scan_type = 0x00;
            escan_params.scan_window = 0x00A0;
            if (0x01U == index)
            {
                retval = appl_ga_utils_enable_extended_scan(escan_params);
                if (GA_SUCCESS == retval)
                {
                    is_scan_enabled = APPL_GA_SCAN_FOR_UCS;
                }
                else
                {
                    APPL_ERR("Failed to set Extended Scan Params Or Scan Already initiated"
                        " by Other Module !\n");
                }
            }
            else
            {
                retval = appl_ga_utils_disable_extended_scan(escan_params);
                if (GA_SUCCESS == retval)
                {
                    is_scan_enabled = APPL_GA_SCAN_INVALID;
                }
                else
                {
                    APPL_ERR("Failed to Disable Extended Scan Or Scan Already Stopped"
                        " by Other Module !\n");
                }
            }
            break;

        case 25:
            CONSOLE_OUT("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            CONSOLE_OUT("Enter endpoint type: ");
            CONSOLE_IN("%d", &choice);
            device.bd_type = (UINT8)choice;
            retval = appl_ga_utils_add_conn_device_as_a_initiator(device);
            if (GA_SUCCESS == retval)
            {
                appl_ga_utils_connect(device, GA_CONNECTION_MODE_NORMAL);
            }
            else
            {
                APPL_ERR("Device already registered !\n");
            }
            break;

        case 26:
            CONSOLE_OUT("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            CONSOLE_OUT("Enter endpoint type: ");
            CONSOLE_IN("%d", &choice);
            device.bd_type = (UINT8)choice;
            retval = appl_ga_utils_add_conn_device_as_a_initiator(device);
            if (GA_SUCCESS == retval)
            {
                appl_ga_utils_connect(device, GA_CONNECTION_MODE_EXTENDED);
            }
            else
            {
                APPL_ERR("Device already registered !\n");
            }
            break;

        case 27:
            retval = appl_ga_authenticate(SMP_LEGACY_MODE);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 28:
            retval = appl_ga_authenticate(SMP_LESC_MODE);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 29:
            retval = appl_ga_disconnect();
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 30:
            appl_ga_utils_display_all_conn_device();
            break;

        case 31:
            index = appl_ga_utils_bap_get_input_conn_dev_index();
            retval = appl_ga_utils_set_conn_device_in_use(index);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 32:
            index = appl_ga_utils_bap_get_input_conn_dev_index();
            retval = appl_ga_utils_remove_conn_device(index);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 35:
            CONSOLE_OUT("Configure Sink Audio Dump:\n");
            CONSOLE_OUT("1. Enable\n2. Disable\n");
            CONSOLE_IN("%d", &choice);
            retval = appl_ga_utils_audio_snk_config_audio_dump_files((UINT8)choice);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 36:
            CONSOLE_OUT("Configure Source Audio Dump:\n");
            CONSOLE_OUT("1. Enable\n2. Disable\n");
            CONSOLE_IN("%d", &choice);
            retval = appl_ga_utils_audio_src_config_audio_dump_files((UINT8)choice);
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
            break;

        case 40:
            main_unicast_operations();
            break;

        case 45:
            main_broadcast_operations();
            break;

        case 50:
            appl_vcp_ct_operations();
            break;

        case 51:
            appl_vcp_rd_operations();
            break;

        case 55:
            appl_micp_ct_operations();
            break;

        case 56:
            appl_micp_dev_operations();
            break;

        case 60:
            appl_csip_sc_operations();
            break;

        case 61:
            appl_csip_sm_operations();
            break;

        case 65:
            main_mcp_ce_menu_operations();
            break;

        case 66:
            main_mcp_se_menu_operations();
            break;

        case 70:
            main_ccp_ce_menu_operations();
            break;

        case 71:
            main_ccp_se_menu_operations();
            break;

        case 75:
            main_tmap_menu_operations();
            break;

        case 76:
            main_hap_menu_operations();
            break;

        case 80:
#ifdef BAP_SUPPORT_CONTEXT_MANAGE
            /* Fetch the connected device index, context and info */
            appl_ga_utils_get_conn_device_in_use
            (
                &index,
                &device,
                &context
            );

            retval = GA_get_context_info
                     (
                         &device,
                         &context,
                         &bap_svc_range_ptr,
                         &bap_char_info_ptr,
                         &bap_total_char_count
                     );
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }

            if (GA_SUCCESS == retval)
            {
                GA_mem_copy
                (
                    bap_svc_range,
                    bap_svc_range_ptr,
                    (BAP_SERVICE_COUNT * sizeof(GA_BRR_SVC_RANGE))
                );

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device.
                 */
                bap_char_info = GA_alloc_mem(bap_total_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != bap_char_info)
                {
                    GA_mem_copy(bap_char_info, bap_char_info_ptr, (bap_total_char_count * sizeof(GA_CHAR_INFO)));
                }
            }
#else /* BAP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("BAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 81:
#ifdef BAP_SUPPORT_CONTEXT_MANAGE
            /* Fetch the connected device index, context and info */
            appl_ga_utils_get_conn_device_in_use
            (
                &index,
                &device,
                &context
            );

            bap_svc_range_ptr = bap_svc_range;
            bap_char_info_ptr = bap_char_info;

            retval = GA_set_context_info
                     (
                         &device,
                         &context,
                         &bap_svc_range_ptr,
                         &bap_char_info_ptr,
                         &bap_total_char_count
                     );
            APPL_TRC("Retval - ");
            appl_ga_utils_display_ga_result(retval);

            if (GA_SUCCESS == retval)
            {
                /* Set the context in device manager */
                appl_ga_utils_update_context_of_conn_dev(context, index);
                if (NULL != bap_char_info)
                {
                    GA_free_mem(bap_char_info);
                    bap_char_info = NULL;
                }
            }
#else /* BAP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("BAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 87:
#ifdef BAP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_set_context_info() is NOT called after
             * GA_get_context_info().
             */

            /* Free the allocated memory for PACS + ASCS char. info. */
            if (NULL != bap_char_info)
            {
                APPL_TRC("Freeing bap_char_info ...\n");
                GA_free_mem(bap_char_info);
                bap_char_info = NULL;
            }
#else /* BAP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("BAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
#ifdef BAP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_update_ntf_configuration...\n");
            retval = GA_update_ntf_configuration
                     (
                         (/* BAP_CHAR_CONFIG(BAP_CHAR_ID_SOURCE_PAC) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SOURCE_AUDIO_LOCATIONS) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SINK_PAC) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SINK_AUDIO_LOCATIONS) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_AVAILABLE_AUDIO_CONTEXTS) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SUPPORTED_AUDIO_CONTEXTS) | */
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_CP) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_SOURCE) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_SINK))
                     );
            APPL_TRC("Retval - 0x%04X\n", retval);
#else /* BAP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("BAP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* BAP_SUPPORT_CONFIG_SELECTION */
            break;

        default:
            APPL_INF("Invalid Choice\n");
            break;

        }
        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

GA_RESULT appl_ga_authenticate(UCHAR security_mode)
{
    GA_RESULT      retval;
    SMP_AUTH_INFO  auth;
    SMP_BD_HANDLE  bd_handle;
    GA_ENDPOINT    device;
    UINT8          dev_index;

    /* Initialize */
    retval = GA_FAILURE;
    /* Security Mode - SMP_LEGACY_MODE or SMP_LESC_MODE */
    auth.pair_mode = security_mode;
    auth.security = SMP_SEC_LEVEL_1;
    auth.bonding = BT_TRUE;
    auth.ekey_size = 16U;
    dev_index = 0U;

    /* Fetch Conn Device Index */
    dev_index = appl_ga_utils_bap_get_input_conn_dev_index();

    /* Fetch device from conn_dev_index */
    appl_ga_utils_get_conn_device(dev_index, &device);

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
    auth.transport = SMP_LINK_LE;
    auth.xtx_info = SMP_XTX_DISABLE;
    BT_smp_get_link_handle
    (
        (BT_DEVICE_ADDR *)&device,
        &bd_handle,
        auth.transport
    );
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
    BT_smp_get_bd_handle
    (
        (BT_DEVICE_ADDR *)&device,
        &bd_handle
    );
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

    retval = BT_smp_authenticate
             (
                 &bd_handle,
                 &auth
             );

    return retval;
}

GA_RESULT appl_ga_disconnect(void)
{
    UINT32     read;
    GA_RESULT  retval;
    UCHAR      reason;
    UINT16     handle;

    /* Initialize */
    retval = GA_FAILURE;
    reason = 0U;
    handle = 0U;
    read = 0U;

    CONSOLE_OUT("ACL/SCO Connection Handle (in Hex) = ");
    CONSOLE_IN("%x", &read);
    handle = (UINT16)read;
    CONSOLE_OUT("Reason for Disconnection (in Hex) = ");
    CONSOLE_IN("%x", &read);
    reason = (UCHAR)read;
    APPL_TRC("\nStarting ACL/SCO Disconnection... ");
    retval = appl_ga_utils_disconnect(handle, reason);

    return retval;
}
#endif /* BT_GAM */
