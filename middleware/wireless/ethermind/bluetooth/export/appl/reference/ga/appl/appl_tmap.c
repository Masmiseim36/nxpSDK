/**
 *  \file appl_tmap.c
 *
 *  \brief This file defines the Telephony and Media Audio Profile:
 *  Application Data Structures and Methods.
 *  This application is used to enable a Generic Attribute Profile (GATT)
 *  client to setup TMAP profile roles.
 *
 *  The Call Gateway (CG): This role is defined for telephony or VoIP
 *  applications. The CG device has the connection to the call network
 *  infrastructure. Typical devices implementing the CG role include
 *  smartphones, laptops, tablets, and PCs.
 *  The Call Terminal (CT ): This role is defined for headset type devices in
 *  telephony or VoIP applications. Typical devices implementing the CT role
 *  include wireless headsets, speakers, and microphones that participate in
 *  conversational audio.
 *  The Unicast Media Sender (UMS): This role is defined for devices that send
 *  media audio content in one or more Unicast Audio Streams. Typical devices
 *  implementing the UMS role include smartphones, media players, TVs, laptops,
 *  tablets, and PCs.
 *  The Unicast Media Receiver (UMR): This role is defined for devices that
 *  receive media audio content from a source device in a Unicast Audio Stream.
 *  Typical devices implementing the UMR role include headphones, earbuds, and
 *  wireless speakers.
 *  The Broadcast Media Sender (BMS): This role is defined for devices that
 *  send media audio content to an unlimited number of receiving devices.
 *  Typical devices implementing the BMS role include smartphones, media
 *  players, TVs, laptops, tablets, and PCs.
 *  The Broadcast Media Receiver (BMR): This role is defined for devices that
 *  receive media audio content from a source device in a Broadcast Audio
 *  Stream. Typical devices implementing the BMR role include headphones,
 *  earbuds, and speakers. A smartphone may also support this role to receive
 *  Broadcast Audio Streams from a BMS.
 */

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* --------------------------------------------- Header File Inclusion */
#include "appl_tmap.h"
#include "appl_utils.h"
#include "appl_ga_utils.h"

#include "appl_ga_ucc.h"
#include "appl_ga_ucs.h"
#include "appl_ga_bc_ba.h"
#include "appl_ga_bc_sd.h"
#include "appl_vcp.h"
#include "appl_ccp_se.h"
#include "appl_ccp_ce.h"
#include "appl_mcp_se.h"

#ifdef GA_TMAP

/* --------------------------------------------- Global Definitions */

static const UCHAR ga_tmap_appl_menu[] = "\n\
================ GA TMAP MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Profile/Service Management ~ Init ============ \n\
    2. GA TMAP Initialize \n\
    3. GA TMAP Register \n\
\n\
================ Profile/Service Management ~ Deinit ============ \n\
    4. GA TMAP Shutdown \n\
\n\
================ GA TMAP Client MENU ================ \n\
================ Context Management ~ Setup ============ \n\
   10. Discover TMAS Role \n\
\n\
================ Context Management ~ Release ============ \n\
   11. Release/Close TMAS \n\
\n\
================ GA TMAP Server MENU ================ \n\
================ TMAP - CG ============ \n\
   20. Unicast Client. \n\
   21. VCP Volume Controller Operations. \n\
   22. CCP Call Control Server. \n\
\n\
================ TMAP - CT ============ \n\
   30. Unicast Server. \n\
   31. VCP Volume Renderer Operations. \n\
   32. CCP Call Control Client. \n\
\n\
================ TMAP - UMS ============ \n\
   40. Unicast Client - BAP Audio Source. \n\
   41. VCP Volume Controller Operations. \n\
   42. MCP Media Control Server. \n\
\n\
================ TMAP - UMR ============ \n\
   50. Unicast Server - BAP Audio Sink. \n\
   51. VCP Volume Renderer Operations. \n\
\n\
================ TMAP - BMS ============ \n\
   60. Broadcast Source. \n\
\n\
================ TMAP - BMR ============ \n\
   70. Broadcast Sink. \n\
   71. VCP Volume Renderer Operations. \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get Context Info. \n\
   81. Set Context Info. \n\
\n\
   87. Free Saved Context Info. \n\
\n\
Your Option? ";

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- External Global Variables */
DECL_STATIC GA_ENDPOINT ga_tmap_peer_bd_addr;

TMAP_HANDLE appl_tmap_handle;

#ifdef TMAP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_tmap_tmas_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_tmap_tmas_char_info_ptr;
DECL_STATIC  UINT16              appl_tmap_tmas_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_tmap_tmas_range;
DECL_STATIC  GA_CHAR_INFO      * appl_tmap_tmas_char_info;
DECL_STATIC  TMAP_HANDLE         appl_tmap_tmas_handle_access;
#endif /* TMAP_SUPPORT_CONTEXT_MANAGE */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;
/* Indicates what roles of the App are initialized */
extern APPL_GA_ROLE ga_role;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

void appl_tmap_server_init(TMAP_ROLE tmap_role);
void appl_tmap_server_init_CG(void);
void appl_tmap_server_init_CT(void);
void appl_tmap_server_init_UMS(void);
void appl_tmap_server_init_UMR(void);
void appl_tmap_server_init_BMS(void);
void appl_tmap_server_init_BMR(void);

void main_tmap_menu_operations(void)
{
    int        choice;
    int        menu_choice;
    GA_RESULT  retval;
    TMAP_ROLE  role;
    UINT8      isClient;
    UINT8      conn_dev_index;

    /* Initialize */
    retval = GA_FAILURE;
    role = 0xFFFFU;
    isClient = 0U;
    conn_dev_index = 0U;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ga_tmap_appl_menu);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                APPL_TRC("GA_tmap_init...\n");
                retval = GA_tmap_init(appl_tmap_cb);
                APPL_TRC("Retval - ");
                appl_tmap_parse_ga_result(retval);
                break;

            case 3:
                CONSOLE_OUT("Input TMAP Type, Client: 1, Server: 0\n");
                CONSOLE_IN("%x", &choice);
                isClient = (UINT8)choice;

                if (isClient)
                {
#ifdef TMAP_CLIENT
                    APPL_TRC("GA_tmap_register_role...\n");
                    retval = GA_tmap_register_role(isClient, role);
                    APPL_TRC("Retval - ");
                    appl_tmap_parse_ga_result(retval);
#else /* TMAP_CLIENT */
                    APPL_ERR("Enable TMAP_CLIENT !");
#endif /* TMAP_CLIENT */
                }

                if (!isClient)
                {
#ifdef TMAP_SERVER
                    /** TMAP Role - Call Gateway (CG) */
                    CONSOLE_OUT(" - Call Gateway(CG): 0x0001\n");
                    CONSOLE_OUT(" - Call Terminal(CT): 0x0002\n");
                    CONSOLE_OUT(" - Unicast Media Sender (UMS): 0x0004\n");
                    CONSOLE_OUT(" - Unicast Media Receiver (UMR): 0x0008\n");
                    CONSOLE_OUT(" - Broadcast Media Sender (BMS): 0x0010\n");
                    CONSOLE_OUT(" - Broadcast Media Receiver (BMR): 0x0020\n");
                    CONSOLE_OUT("Note: If this device supports more than one role, Compute BitMask OR and input\n");
                    CONSOLE_OUT("E.g., Call Gateway(CG):0x0001 and Call Terminal(CT):0x0002, Input 0x0003\n");
                    CONSOLE_IN("%x", &choice);
                    role = (UINT16)choice;

                    APPL_TRC("GA_tmap_register_role...\n");
                    retval = GA_tmap_register_role(isClient, role);
                    APPL_TRC("Retval - ");
                    appl_tmap_parse_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        appl_tmap_server_init(role);

                        /* Set the Adv Report as per TMAP */
                        appl_ga_utils_set_adv_data_for_current_tmap_role(ga_role);
                    }
#else /* TMAP_SERVER */
                    APPL_ERR("Enable TMAP_SERVER !");
#endif /* TMAP_SERVER */
                }
                break;

            case 4:
                APPL_TRC("GA_tmap_shutdown...\n");
                retval = GA_tmap_shutdown();
                APPL_TRC("Retval - ");
                appl_tmap_parse_ga_result(retval);
                break;

            case 10:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &ga_tmap_peer_bd_addr
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter Peer BD Address: ");
                appl_get_bd_addr(ga_tmap_peer_bd_addr.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                ga_tmap_peer_bd_addr.bd_type = (UINT8)choice;
            }

#ifdef TMAP_CLIENT
                APPL_TRC("GA_tmap_setup...\n");
                retval = GA_tmap_setup
                        (
                            &ga_tmap_peer_bd_addr,
                            &appl_tmap_handle
                        );
                APPL_TRC("Retval - ");
                appl_tmap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("appl_tmap_handle:0x%04x\n", appl_tmap_handle);
                }
#else /* TMAP_CLIENT */
                APPL_ERR("Enable TMAP_CLIENT !");
#endif /* TMAP_CLIENT */
                break;

            case 11:
#ifdef TMAP_CLIENT
            {
                CONSOLE_OUT("\n1. Release\n2. Close\n");
                CONSOLE_OUT("Enter your choice: ");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    APPL_TRC("GA_tmap_release...\n");
                    retval = GA_tmap_release(appl_tmap_handle);
                    APPL_TRC("Retval - ");
                    appl_tmap_parse_ga_result(retval);
                }
                else if (2 == choice)
                {
                    APPL_TRC("GA_tmap_close...\n");
                    retval = GA_tmap_close(appl_tmap_handle);
                    APPL_TRC("Retval - ");
                    appl_tmap_parse_ga_result(retval);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
            }
#else /* TMAP_CLIENT */
                APPL_ERR("Enable TMAP_CLIENT !");
#endif /* TMAP_CLIENT */
                break;

            case 20: /* Fall - through */
            case 40:
                main_ucc_operations();
                break;

            case 21: /* Fall - through */
            case 41:
                appl_vcp_ct_operations();
                break;

            case 22:
                main_ccp_se_menu_operations();
                break;

            case 30: /* Fall - through */
            case 50:
                /*
                 * NOTE:
                 * For CT
                 * Mandatory to support at least one of these roles.
                 * BAP Audio Sink or BAP Audio Source.
                 */
                main_ucs_operations();
                break;

            case 31: /* Fall - through */
            case 51: /* Fall - through */
            case 71:
                /*
                 * NOTE:
                 * For CT
                 * Mandatory to support this role if the BAP Unicast Server is
                 * acting as an Audio Sink as defined in BAP, otherwise
                 * Excluded.
                 */
                appl_vcp_rd_operations();
                break;

            case 32:
                /*
                 * NOTE:
                 * For CT
                 * Optional to support this role.
                 * TMAP makes no change to the requirement as specified in CAP.
                 */
                main_ccp_ce_menu_operations();
                break;

            case 42:
                main_mcp_se_menu_operations();
                break;

            case 60:
                main_ba_operations();
                break;

            case 70:
                main_sd_operations();
                break;

            case 80:
            {
#ifdef TMAP_SUPPORT_CONTEXT_MANAGE
                appl_tmap_tmas_handle_access = appl_tmap_handle;

                APPL_TRC("GA_tmap_get_context_info ...\n");
                retval = GA_tmap_get_context_info
                         (
                             &ga_tmap_peer_bd_addr,
                             &appl_tmap_tmas_handle_access,
                             &appl_tmap_tmas_range_ptr,
                             &appl_tmap_tmas_char_info_ptr,
                             &appl_tmap_tmas_char_count
                         );
                APPL_TRC("Retval - ");
                appl_tmap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("appl_tmap_tmas_char_count:0x%02x\n", appl_tmap_tmas_char_count);

                    appl_tmap_tmas_range = *appl_tmap_tmas_range_ptr;

                    /**
                     *  Reference to allocate to save locally.
                     *  This can be written to persistent and retrieved
                     *  upon the next connection if bonded device
                     */
                    appl_tmap_tmas_char_info = GA_alloc_mem(appl_tmap_tmas_char_count * sizeof(GA_CHAR_INFO));
                    if (NULL != appl_tmap_tmas_char_info)
                    {
                        GA_mem_copy
                        (
                            appl_tmap_tmas_char_info,
                            appl_tmap_tmas_char_info_ptr,
                            appl_tmap_tmas_char_count * sizeof(GA_CHAR_INFO)
                        );
                    }
                }
#else /* TMAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("TMAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* TMAP_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 81:
            {
#ifdef TMAP_SUPPORT_CONTEXT_MANAGE
                if (NULL == appl_tmap_tmas_char_info)
                {
                    APPL_ERR("appl_tmap_tmas_char_info is NULL !\n");
                    APPL_TRC("Call GA_tmap_get_context_info \n");
                    break;
                }

                appl_tmap_tmas_char_info_ptr = appl_tmap_tmas_char_info;
                appl_tmap_tmas_range_ptr = &appl_tmap_tmas_range;

                APPL_TRC("GA_tmap_set_context_info ...\n");
                retval = GA_tmap_set_context_info
                         (
                             &ga_tmap_peer_bd_addr,
                             &appl_tmap_tmas_handle_access,
                             &appl_tmap_tmas_range_ptr,
                             &appl_tmap_tmas_char_info_ptr,
                             &appl_tmap_tmas_char_count
                         );
                APPL_TRC("Retval - ");
                appl_tmap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    appl_tmap_handle = appl_tmap_tmas_handle_access;

                    /* Free the allocated memory */
                    GA_free_mem(appl_tmap_tmas_char_info);
                    appl_tmap_tmas_char_info = NULL;
                }
#else /* TMAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("TMAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* TMAP_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 87:
#ifdef TMAP_SUPPORT_CONTEXT_MANAGE
                /**
                 * This is to free up the allocated char. info. memory
                 * If incase GA_tmap_set_context_info() is NOT called after
                 * GA_tmap_get_context_info().
                 */

                /* Free the allocated memory for TMAS char. info. */
                if (NULL != appl_tmap_tmas_char_info)
                {
                    APPL_TRC("Freeing appl_tmap_tmas_char_info ...\n");
                    GA_free_mem(appl_tmap_tmas_char_info);
                    appl_tmap_tmas_char_info = NULL;
                }
#else /* TMAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("TMAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* TMAP_SUPPORT_CONTEXT_MANAGE */
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

void appl_tmap_server_init(TMAP_ROLE tmap_role)
{
    if (TMAP_ROLE_CG & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_CG;
        appl_tmap_server_init_CG();
    }
    if (TMAP_ROLE_CT & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_CT;
        appl_tmap_server_init_CT();
    }
    if (TMAP_ROLE_UMS & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_UMS;
        appl_tmap_server_init_UMS();
    }
    if (TMAP_ROLE_UMR & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_UMR;
        appl_tmap_server_init_UMR();
    }
    if (TMAP_ROLE_BMS & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_BMS;
        appl_tmap_server_init_BMS();
    }
    if (TMAP_ROLE_BMR & tmap_role)
    {
        ga_role |= APPL_GA_TMAP_ROLE_BMR;
        appl_tmap_server_init_BMR();
    }
}

void appl_tmap_server_init_CG(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: CG: M support.
     * Table 3.2
     * CAP Role: Initiator. M                  ------> 1
     *           Commander: M
     * Table 3.3
     * BAP Unicast Client: BAP Audio Source.   ------> 2
     *                     BAP Audio Sink.
     * Table 3.4
     * VCP: Controller                         ------> 3
     * Table 3.5
     * CCP: Server                             ------> 4
     * Table 3.6
     * LL: LE 2M PHY                           ------> 5
     * Table 3.10
     * Sink Audio Locations   M                ------> 6
     * Source Audio Locations M
     *
     * Config Codec:
     * ------------
     * As per TMAP Spec: Table 3.12
     * Codec Configuration Setting:
     * 32_2: M                                 ------> 7
     *
     * As per BAP Spec: Table 3.11
     * Unicast Client - Audio Source:
     * 16_2: M                                 ------> 8
     * Unicast Client - Audio Sink:
     * 16_2: 24_2: M                           ------> 9
     *
     * As per TMAP Spec: Table 3.14
     * ACA:
     * Front Left: O
     * Front Right: O
     * Front Right and Front Left: Mandatory if Audio Configuration 5 in
     * Table 3.16 is supported, otherwise Excluded.
     *
     *
     * Config QoS:
     * ----------
     * For each 16 kHz or 32 kHz Codec Configuration setting in Table 3.12 or
     * Table 3.11 in BAP that the CG supports, the CG shall support the
     * corresponding QoS Configuration settings for low latency audio data
     * from Table 5.2 in BAP.
     *
     * As per BAP Spec: Unicast Client
     * 16_2_1: 16_2: M                         ------> 10
     * All others 8_2_1 to 48_6_2 are O if they are supported in Table 3.11
     * Except for 16_2, all others are O.
     *
     * As per CAP Spec:
     * BAP Unicast Client - GAP Central
     * VCP Volume Controller - GAP Central
     * CCP Call Control Server - GAP Central
     * CSIP Set Coordinator - GAP Central
     * GAP Role: Central                       ------> 11
     *
     * As per CAP Spec, Table 3.1
     * CSIP Set Coordinator: Mandatory if BAP Unicast Client is supported,
     * otherwise Excluded.                     ------> 12
     *
     * As per TMAP Spec: Table 3.16
     * Audio Configuration:
     * 3: 7(ii): 8(ii): 8(i): M
     * 5: 11(i): 11(ii): O
     *
     * To help enable a consistent user experience when a CG is communicating
     * with a single CT, when the CG has a single channel (e.g., mono voice) of
     * audio data to send to a single CT that is not a member of a coordinated
     * set, and that supports both the Front Left and Front Right Audio
     * Locations, (i.e., at least two Sink ASEs comprising Label D or Label F
     * as defined in Table 3.16), and does not support an Audio_Channel_Counts
     * value greater than 1, then the CG shall attempt to configure the CT
     * using Label D or using Label F. If the CT is successfully configured
     * using Label D or Label F (i.e., the Audio_Channel_Allocation is
     * configured for the Front Left Audio Location on one Sink ASE and the
     * Audio_Channel_Allocation is configured for the Front Right Sink Audio
     * Location on the other Sink ASE), then the CG shall transmit the single
     * channel of audio data to each configured ASE in the CT.
     *
     * If a CG that does not support Label F discovers a CT exposing a set of
     * ASE characteristics corresponding to Label F, then the CG shall be able
     * to configure the CT using Label D. If a CG that does not support Label G
     * discovers a pair of CTs exposing a set of ASE characteristics
     * corresponding to Label G, then the CG shall be able to configure the
     * pair of CTs using Label C.
     *
     * A CG device supporting an Audio Configuration in Table 3.16 shall
     * support concurrent streams on the multiple ASEs involved in the
     * supported Audio Configuration using the CG device’s supported codec
     * configuration setting in Table 3.12 and the CG device’s supported QoS
     * configuration settings in Section 3.5.1.4.2.
     */

    /* TODO: Take input if TBS is required. */
    /* BAP UCC - Nothing here, since it is a Client */

    /* VCP - Nothing here, since it is a Client */

    /* CCP - Nothing here, Register happens as part of a case in CCP menu */

    /* CSIP - Nothing here, since it is a Client */
}

void appl_tmap_server_init_CT(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: CG: M support.
     * Table 3.2
     * CAP Role: Acceptor. M                   ------> 1
     *           Commander: O
     * Table 3.3
     * BAP Unicast Server: BAP Audio Source.   ------> 2
     *                     BAP Audio Sink.
     * M to support at least one of these roles
     * Table 3.4
     * VCP: Renderer                           ------> 3
     * M to support this role if the BAP Unicast Server is acting as an
     * Audio Sink as defined in BAP, otherwise Excluded.
     * Table 3.6
     * LL: LE 2M PHY                           ------> 4
     *
     * As per TMAP Spec, Table 3.9
     * Codec Capability Settings:              ------> 5
     * 16_1 M
     * 32_1 M
     * 32_2 M
     *
     * As per BAP Spec, Table 3.5
     * Codec Specific Capabilities             ------> 6
     * Audio Source:
     * 16_2 M
     * 16_2_1 16_2_2
     * Audio Sink:
     * 16_2 M
     * 24_2 M
     * 16_2_1 16_2_2
     * 24_2_1 24_2_2
     *
     * A CT device that supports the BAP Audio Sink role shall include at least
     * one Sink Audio Stream Endpoint (ASE) characteristic as defined in ASCS.
     * A CT device that supports the BAP Audio Source role shall include at
     * least one Source ASE characteristic. A CT capable of bidirectional audio
     * shall include at least one Sink ASE characteristic and one Source ASE
     * characteristic.
     *
     * As per TMAP Spec: Table 3.16
     * Audio Configuration:
     * 8(i): C.1
     * 5: 11(i): C.2
     * C.1: Mandatory if both the BAP Audio Sink and BAP Audio Source roles are
     *      supported and the CT device can present two or more audio outputs
     *      to the end user, otherwise Excluded.
     * C.2: Optional if both the BAP Audio Sink and BAP Audio Source roles are
     *      supported and the CT device can present two or more audio outputs
     *      to the end user, otherwise Excluded.
     *
     * If an Initiator device configures a CT device with an Audio
     * Configuration in Table 3.16 that the CT device supports, then the CT
     * device shall support concurrent streams on the multiple ASEs involved in
     * the supported Audio Configuration using the CT device’s supported codec
     * capability settings in Table 3.8 and the CT device’s supported QoS
     * configuration settings in Section 3.5.1.4.2.
     *
     * As per CAP Spec:
     * BAP Unicast Server - GAP Peripheral
     * VCP Volume Renderer - GAP Peripheral
     * GAP Role: Peripheral                    ------> 7
     *
     * As per CAP Spec, Table 3.1
     * CSIP Set Member: Mandatory if part of a Coordinated Set,
     * otherwise Excluded.                     ------> 8
     */

    /* PACS */
    /* appl_ga_register_pacs(APPL_GA_TMAP_ROLE_CT); */

    /* ASCS */
    /* appl_ga_ucs_register_ascs(APPL_GA_TMAP_ROLE_CT); */
}

void appl_tmap_server_init_UMS(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: UMS: M support.
     * Table 3.2
     * CAP Role: Initiator + Commander.        ------> 1
     * Table 3.3
     * BAP Unicast Client: BAP Audio Source.   ------> 2
     * Table 3.4
     * VCP: Controller                         ------> 3
     * Table 3.5
     * MCP: Server                             ------> 4
     * Table 3.6
     * LL: LE 2M PHY                           ------> 5
     *
     * Config Codec:
     * ------------
     * As per TMAP Spec: Table 3.13
     * Local Role: Source, Remote Role: Sink.
     * Codec Configuration Setting:
     * 48_2: M                                 ------> 6
     * 48_4 or 48_6: One is M amongst the 2.   ------> 7
     *
     * As per BAP Spec: Unicast Client - Audio Source
     * 48_1_1: 48_1
     * 48_3_1: 48_3
     * 48_5_1: 48_5
     * 48_1_2: 48_1
     * 48_3_2: 48_3
     * 48_5_2: 48_5
     * C.1: Optional if the Codec Configuration Setting is supported in Table 3.11, otherwise Excluded.
     *
     * As per BAP Spec: Table 3.11:
     * 48_1: O
     * 48_3: O
     * 48_5: O
     * The above are not M.
     *
     * Since the above are not M, we dont have to support 48_1, 48_3, 48_5 as
     * M.
     *
     * As per TMAP Spec: Table 3.14
     * ACA:
     * Front Left: M
     * Front Right: M
     * Front Right and Front Left: Mandatory if Audio Configuration 4 in
     * Table 4.1 in BAP [4] is supported, otherwise Excluded.
     *
     *
     * Config QoS:
     * ----------
     * For each 48 kHz or 44.1 kHz Codec Configuration setting in Table 3.13 or
     * Table 3.11 in BAP [4] that the UMS supports, the UMS shall support the
     * corresponding low latency and high reliability QoS Configuration
     * settings from Table 5.2 in BAP.
     *
     * As per BAP Spec: Unicast Client - Audio Source
     * 16_2_1: 16_2: M                           ------>8
     * All others 8_2_1 to 48_6_2 are O if they are supported in Table 3.11
     * Except for 16_2, all others are O.
     *
     * As per CAP Spec:
     * BAP Unicast Client - GAP Central
     * VCP Volume Controller - GAP Central
     * MCP Server - GAP Central
     * CSIP Set Coordinator - GAP Central
     * GAP Role: Central                         ------>9
     *
     * As per CAP Spec, Table 3.1
     * CSIP Set Coordinator: Mandatory if BAP Unicast Client is supported,
     * otherwise Excluded.                     ------> 10
     *
     * To help enable a consistent user experience when a UMS is communicating
     * with a single UMR, when the UMS has a single channel (e.g., mono music)
     * of audio data to send to a single UMR that is not a member of a
     * coordinated set, and that supports both the Front Left and Front Right
     * Audio Locations as defined in Table 3.11, (i.e., at least two Sink ASEs
     * comprising Audio Configuration 6 (i) in Table 4.1 in BAP [4]), and does
     * not support an Audio_Channel_Counts value greater than 1, then the UMS
     * shall attempt to configure the UMR using Audio Configuration 6(i).
     * If the UMR is successfully configured using Audio Configuration 6(i)
     * (i.e., the Audio_Channel_Allocation is configured for the Front Left
     * Audio Location on one Sink ASE and the Audio_Channel_Allocation is
     * configured for the Front Right Sink Audio Location on the other Sink
     * ASE), then the UMS shall transmit the single channel of audio data to
     * each configured ASE in the UMR.
     */

    /* Registering below M requirements */

    /* BAP UCC - Nothing here, since it is a Client */

    /* VCP - Nothing here, since it is a Client */

    /* MCP - Nothing here, Register happens as part of a case in MCP menu */

    /* CSIP - Nothing here, since it is a Client */

}

void appl_tmap_server_init_UMR(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: UMR: M support.
     * Table 3.2
     * CAP Role: Acceptor. M                   ------> 1
     *           Commander: O
     * Table 3.3
     * BAP Unicast Server: BAP Audio Sink.     ------> 2
     * Table 3.4
     * VCP: Renderer                           ------> 3
     * Table 3.5
     * LL: LE 2M PHY                           ------> 4
     *
     * As per TMAP Spec, Table 3.9
     * Codec Capability Settings:              ------> 5
     * 48_1 M
     * 48_2 M
     * 48_3 M
     * 48_4 M
     * 48_5 M
     * 48_6 M
     *
     * As per BAP Spec, Table 3.5
     * Codec Specific Capabilities             ------> 6
     * 16_2 M
     * 24_2 M
     *
     * 16_2_1 16_2_2
     * 24_2_1 24_2_2
     *
     * As per TMAP Spec, Table 3.11
     * Sink Audio Locations M                  ------> 7
     * Front Left: C.1
     * Front Right: C.1
     * Front Right and Front Left: C.1
     * M to support atleast one of the above.
     *
     * A UMR that supports Sink Audio Locations (bit 1 and bit 0) = 0b11 and
     * exposes at least 2 Sink ASE characteristics shall support, through
     * configuration of two Sink ASEs, receiving and decoding audio data with
     * left and right Audio Channels each transmitted in a separate Unicast
     * Audio Stream where both Unicast Audio Streams are synchronized in a
     * single CIG.
     *
     * If the UMR is successfully configured using Audio Configuration 6(i)
     * (i.e., the Audio_Channel_Allocation is configured for the Front Left
     * Audio Location on one Sink ASE and the Audio_Channel_Allocation is
     * configured for the Front Right Sink Audio Location on the other Sink
     * ASE), then the UMS shall transmit the single channel of audio data
     * to each configured ASE in the UMR.
     *
     * As per CAP Spec:
     * BAP Unicast Server - GAP Peripheral
     * VCP Volume Renderer - GAP Peripheral
     * CSIP Set Coordinator - GAP Central
     * GAP Role: Peripheral                    ------> 8
     *
     * As per CAP Spec, Table 3.1
     * CSIP Set Member: Mandatory if part of a Coordinated Set,
     * otherwise Excluded.                     ------> 9
     */

    /* PACS */
    appl_ga_register_pacs(APPL_GA_TMAP_ROLE_UMR);

    /* ASCS */
    appl_ga_ucs_register_ascs(APPL_GA_TMAP_ROLE_UMR);
}

void appl_tmap_server_init_BMS(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: BMS: M support.
     * Table 3.2
     * CAP Role: Initiator. M                   ------> 1
     *           Commander: O
     * Table 3.3
     * BAP Broadcast Source.                    ------> 2
     * Table 3.4
     * LL: LE 2M PHY                            ------> 3
     *
     * As per BAP Spec, Table 3.17
     * Codec Configuration Setting for the BMS role
     * 48_1                                     ------> 4
     * 48_2
     * 48_3 or 48_5
     * 48_4 or 48_6
     *
     * As per BAP Spec, Table 3.12
     * 16_2
     *
     * As per BAP Spec, Table 3.18
     * Audio Channel Allocations M                  ------> 7
     * Front Left: M
     * Front Right: M
     * Front Right and Front Left: O
     *
     * Codec Configuration Setting:
     * For each 48 kHz or 44.1 kHz Codec Configuration setting in Table 3.17 or
     * Table 3.12 in BAP [4] that the BMS supports, the BMS shall support the
     * corresponding low latency and high reliability broadcast Audio Stream
     * configuration settings from Table 6.4 in BAP.
     *
     * As per BAP Spec, Table 6.4                   ------> 8
     * 16_2_1
     * 16_2_2
     *
     */

    /* Broadcast Settings */
    appl_ga_bc_set_src_setting(APPL_GA_TMAP_ROLE_BMS);
}

void appl_tmap_server_init_BMR(void)
{
    /* Requirements as per TMAP Spec */
    /* TMAP Role: BMR: M support.
     * Table 3.2
     * CAP Role: Acceptor. M                   ------> 1
     * Table 3.3
     * BAP Broadcast Sink.                     ------> 2
     * VCP: Renderer                           ------> 3
     * Table 3.5
     * LL: LE 2M PHY                           ------> 4
     *
     * As per BAP Spec, Table 3.17
     * Codec Capability Settings:              ------> 5
     * 48_1 M
     * 48_2 M
     * 48_3 M
     * 48_4 M
     * 48_5 M
     * 48_6 M
     *
     * As per BAP Spec, Table 3.5
     * Codec Specific Capabilities             ------> 6
     * 16_2 M
     * 24_2 M
     *
     * As per BAP Spec, Table 3.19
     * Audio Location Value: Sink              ------> 7
     * Front Left OR
     * Front Right OR
     * Front Right and Front Left.
     *
     * For each 48 kHz or 44.1 kHz Codec Capability setting in Table 3.17 or
     * Table 3.17 in BAP [4] that the BMR supports, the BMR shall support the
     * corresponding low latency and high reliability broadcast Audio Stream
     * configuration settings from Table 6.4 in BAP.
     *
     * Codec Capability Setting:
     * As per BAP Spec, Table and Table 3.17   ------> 8
     * 16_2_1
     * 16_2_2
     * 24_2_1
     * 24_2_2
     *
     * The BMR shall support rendering broadcast Audio Streams configured with
     * a Presentation_Delay of 20 ms to 40 ms.
     * By default, when M Conf Settings are chosen, the Presentation Delay
     * automatically defaults to 40ms.         ------> 9
     */
}

GA_RESULT appl_tmap_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ TMAP_EVT_ID     evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ TMAP_EVT      * evt_data
          )
{
    GA_RESULT  retval;

    retval = GA_SUCCESS;

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("TMAP Client");

    switch(evt_id)
    {
        case TMAP_SETUP_CNF:
            APPL_TRC("[Profile]     : TMAP (0x%4X)\n", GA_SERVICE_TMAS);
            APPL_TRC("[SECTION]     : TMAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : TMAP CLIENT-DISCOVER\n");
            APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_tmap_ce_cb
                     (
                         device,
                         evt_id,
                         evt_status,
                         evt_data
                     );

            break;

        case TMAP_RELEASE_CNF:
            APPL_TRC("[Profile]     : TMAP (0x%4X)\n", GA_SERVICE_TMAS);
            APPL_TRC("[SECTION]     : TMAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : TMAP CLIENT-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_tmap_ce_cb
                     (
                         device,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        default:
            APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
            retval = GA_FAILURE;
    }

    GA_CB_EXIT();

    return retval;
}

GA_RESULT appl_tmap_ce_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ TMAP_EVT_ID     evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ TMAP_EVT      * evt_data
          )
{
    GA_RESULT   retval;
    TMAP_ROLE * tmap_role;

    GA_IGNORE_UNUSED_PARAM(device);

    /* Initialize */
    retval = GA_SUCCESS;

    APPL_TRC("[Event Status]: ");
    appl_tmap_parse_ga_result(evt_status);

    switch(evt_id)
    {
        case TMAP_SETUP_CNF:
            APPL_TRC
            (
                "[Event]       : TMAP_SETUP_CNF (0x%02X)\n",
                evt_id
            );
            if (GA_SUCCESS == evt_status)
            {
                if(evt_data != NULL)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->tmap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();

                    if(0U != evt_data->len)
                    {
                        APPL_TRC("Data Length: 0x%02X\n", evt_data->len);
                    }
                    else
                    {
                        APPL_INF("Data Length: 0x00\n");
                    }
                    if(NULL != evt_data->data)
                    {
                        APPL_TRC("Data:\n");
                        /* appl_dump_bytes(evt_data->data, evt_data->len); */
                        tmap_role = evt_data->data;
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Role(s): 0x%04X\n",
                            evt_data->len,
                            *tmap_role
                        );
                        appl_tmap_parse_role(*tmap_role);
                    }
                    else
                    {
                        APPL_INF("Data: NULL\n");
                    }
                }
            }
            break;

        case TMAP_RELEASE_CNF:
            APPL_TRC
            (
                "[Event]       : TMAP_RELEASE_CNF (0x%02X)\n",
                evt_id
            );
            break;

        default:
            APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
            retval = GA_FAILURE;
            break;
    }

    return retval;
}

#ifdef TMAP_CLIENT
void appl_tmap_parse_role(TMAP_ROLE role)
{
    if (role & TMAP_ROLE_CG)
    {
        APPL_TRC("\t-Call Gateway (CG) (0x%04X)\n", TMAP_ROLE_CG);
    }
    if (role & TMAP_ROLE_CT)
    {
        APPL_TRC("\t-Call Terminal (CT) (0x%04X)\n", TMAP_ROLE_CT);
    }
    if (role & TMAP_ROLE_UMS)
    {
        APPL_TRC("\t-Unicast Media Sender (UMS) (0x%04X)\n", TMAP_ROLE_UMS);
    }
    if (role & TMAP_ROLE_UMR)
    {
        APPL_TRC("\t-Unicast Media Receiver (UMR) (0x%04X)\n", TMAP_ROLE_UMR);
    }
    if (role & TMAP_ROLE_BMS)
    {
        APPL_TRC("\t-Broadcast Media Sender (BMS) (0x%04X)\n", TMAP_ROLE_BMS);
    }
    if (role & TMAP_ROLE_BMR)
    {
        APPL_TRC("\t-Broadcast Media Receiver (BMR) (0x%04X)\n", TMAP_ROLE_BMR);
    }
    return;
}

#endif /* TMAP_CLIENT */

void appl_tmap_parse_ga_result(GA_RESULT status)
{
    GA_RESULT  retval;

    retval = GA_SUCCESS;

    /* Checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(status);

    if (GA_FAILURE == retval)
    {
        /*
         * Status could be one of the App or implementation Error Codes which
         * are received from the Server as part of Response to Read/Write
         * Requests from Client.
         */
        switch (status)
        {
        /* Implementation related error codes */
        case GA_TMAP_INVALID_STATE:
            APPL_ERR("Invalid State !");
            break;

        case GA_TMAP_INVALID_PARAMS:
            APPL_ERR("Invalid Params !");
            break;

        case GA_TMAP_CNTX_ALLOC_FAILED:
            APPL_ERR("Context allocation failed !");
            break;

        case GA_TMAP_IN_BUSY_STATE:
            APPL_ERR("In Busy State !");
            break;

        case GA_TMAP_CNTX_NOT_FOUND:
            APPL_ERR("Context not found !");
            break;

        case GA_TMAP_INVALID_HANDLE:
            APPL_ERR("Invalid Handle !");
            break;

        default:
            APPL_ERR("Invalid Code !");
            break;
        }
        APPL_TRC(" (0x%04X)\n", status);
    }
    return;
}

#endif /* GA_TMAP */
