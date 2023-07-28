/**
 *  \file appl_hap.c
 *
 *  \brief This file defines the Hearing Access/Aid Profile:
 *  Application Data Structures and Methods.
 *  This application is used to identify a hearing aid and optionally to
 *  control hearing aid presets.
 *  A hearing aid preset represents a configuration of the hearing aid signal
 *  processing parameters tailored to a specific listening situation.
 *  This application implements both HAP Client and Server Role.
 *  It shows how to discover a Hearing Aid, its features and also perform
 *  control point procedures on them.
 *  It also indicates how the remote Hearing Aid handles the requests from the
 *  Client device and responds to the same.
 *
 *  Roles: HAP Client: Device that control the Hearing Aid Presets.
 *         HAP Server: Device that receives CP procedures from HAP Client.
 *  GATT Requirement: HAP Client: GATT Client
 *                    HAP Server: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_hap.h"
#include "appl_utils.h"
#include "appl_ga_utils.h"

#include "appl_ga_ucc.h"
#include "appl_ga_ucs.h"
#include "appl_ga_bc_sd.h"
#include "appl_csip.h"
#include "appl_vcp.h"
#include "appl_micp.h"
#include "appl_ccp_ce.h"
#include "appl_ccp_se.h"

#ifdef GA_HAP

/* --------------------------------------------- Global Definitions */
/* HAP Menu Operations */
static const char appl_hap_appl_menu[] = " \n\
================ GA HAP MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Profile/Service Management ~ Init ============ \n\
    2. GA HAP Initialize \n\
    3. GA HAP Register \n\
\n\
================ Profile/Service Management ~ Deinit ============ \n\
    4. GA HAP Shutdown \n\
\n\
================ HAP Client MENU ================ \n\
================ Context Management ~ Setup ============ \n\
   10. GA HAP Setup \n\
\n\
================ Context Management ~ Release ============ \n\
   11. GA HAP Release/Close \n\
\n\
================ Profile Management (HAS) ~ Operations(Read) ============ \n\
   12. GA HAP Read - HA Features \n\
   13. GA HAP Read - Active Preset Index \n\
\n\
================ Profile Management (HAS) ~ Operations(Write) ============ \n\
   14. GA HAP Write - HA Control Point \n\
\n\
================ HAP - HAUC ================ \n\
   50. BAP Unicast Client Operations. \n\
   51. CCP Call Control Server. \n\
   52. CSIP Set Coordinator Operations. \n\
\n\
================ HAP - HARC ================ \n\
   60. VCP Volume Controller Operations. \n\
   61. MICP Microphone Controller Operations. \n\
   62. CSIP Set Coordinator Operations. \n\
\n\
================ HAP Server MENU ================ \n\
================ Misc Management ~ Get/Set ============ \n\
   20. Register Peer Device Address \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   25. GA HAP Notify Features \n\
   26. GA HAP Indicate Hearing Aid Preset Control Point \n\
   27. GA HAP Notify Active Preset Index \n\
\n\
================ HAP - HA ================ \n\
   40. BAP Unicast Server Operations. \n\
   41. VCP Volume Renderer Operations. \n\
   42. MICP Microphone Device Operations. \n\
   43. CCP Call Control Client. \n\
   44. CSIP Set Member Operations. \n\
   45. Broadcast Sink. \n\
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

#ifdef HAP_CLIENT

DECL_STATIC GA_ENDPOINT ga_hap_peer_bd_addr;

DECL_STATIC APPL_HAS_CHAR_INFO appl_hap_char_info[HAP_MAX_HAS_CONTEXTS];

HAP_HANDLE appl_hap_handle;

#ifdef HAP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_hap_has_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_hap_has_char_info_ptr;
DECL_STATIC  UINT16              appl_hap_has_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_hap_has_range;
DECL_STATIC  GA_CHAR_INFO      * appl_hap_has_char_info;
DECL_STATIC  HAP_HANDLE          appl_hap_has_handle_access;

#endif /* HAP_SUPPORT_CONTEXT_MANAGE */

#endif /* HAP_CLIENT */

#ifdef HAP_SERVER

DECL_STATIC APPL_HAS_CHAR_INFO appl_has_char_info[HAP_MAX_HAS_CONTEXTS];

DECL_STATIC GA_ENDPOINT ga_has_peer_bd_addr;

static BT_timer_handle appl_hap_ha_send_ntf_timer_handle;

typedef struct _APPL_CNTRL_POINT_PARAM
{
    GA_ENDPOINT     dev;
    UINT8           hap_cp_opcode;
    UINT8         * data;
    UINT8           datalen;
}APPL_CNTRL_POINT_PARAM;

/*
 * For HAP_CP_OPCODE_READ_PRESETS_REQ, data of 2Bytes
 * StartIndex (1B) + NumPresets (1B)
 * For others, data of 1Bytes
 * Preset Index (1B)
 */
UINT8  appl_hap_ha_data[2U];

#endif /* HAP_SERVER */

/*
 * When an operation includes the Index parameter and the Index parameter
 * does not correspond to a preset record exposed by the server, then the
 * server shall return an ATT_ERROR_RSP PDU to the ATT_WRITE_REQ PDU with the
 * error code set to Out of Range, as defined in Bluetooth Core Specification
 * Supplement, Version 10.
 * As per HAS Spec V1.0, Page: 16
 */
/*
 * As per BT_att_api.h
 * Out of Range error code is used when an attribute value is out of range as
 * defined by a profile or service specification.
 * #define ATT_OUT_OF_RANGE                         0xFF
 */
#define APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE          0xFF

/*
 * If a client writes either of the following opcodes to the Preset Control
 * Point characteristic:
 * - Read Presets Request
 * - Write Preset Name
 * before the server has sent all the notifications or indications triggered
 * by a write of the Read Presets Request opcode by the same client or by
 * another client, then the server shall reject the request by returning an
 * ATT_ERROR_RSP PDU to the ATT_WRITE_REQ PDU with the error code parameter
 * set to Procedure Already in Progress, as defined in Bluetooth Core
 * Specification Supplement, Version 10.
 * As per HAS Spec V1.0, Page: 17
 */
#define APPL_HAP_ATT_ERR_CODE_PROC_ALREADY_IN_PROG  0xFE

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;
/* Indicates what roles of the App are initialized */
extern APPL_GA_ROLE ga_role;

/* --------------------------------------------- APIs */
void appl_hap_server_init_HA(void);
void appl_hap_client_init_HAUC(void);
void appl_hap_client_init_HARC(void);

void appl_hap_server_init_HA(void)
{
    /* Requirements as per HAP Spec */
    /* HAP Role: HA: M support.
     * Table 2.1
     * CAP Role: Acceptor. M                   ------> 1
     * Table 2.2
     * BAP Unicast Server       M              ------> 2
     * VCP Volume Renderer      M
     * MICP Microphone Device   C.2
     * CCP Call Control Client  O
     * CSIP Set Member          C.1
     * HAS Server               M
     * C.1: M if the HA is capable of being part of a Binaural Hearing Aid Set;
     *      otherwise Excluded.
     * C.2: M if the HA supports the BAP Audio Source role; otherwise Excluded.
     *
     * Section 3.2
     * PL: LE 2M PHY                           ------> 3
     * To improve the performance of the mandatory LC3 codec at a
     * 10 ms frame duration over a transport interval of 7.5 ms, an HA shall
     * support the parameters in Table 3.1 when accepting the establishment of
     * a Connected Isochronous Stream (CIS) or synchronizing to a
     * Broadcast Isochronous Stream (BIS).
     *
     * Frame Duration:
     * 10 ms   M
     * 7.5 ms  O
     *
     * If the HA supports the BAP Audio Source role, and the HA has accepted
     * the establishment of a CIS configured with the parameters shown in
     * Table 3.1, then the HA shall support sending unsegmented
     * Service Data Units (SDUs) to prevent an undesirable increase of the
     * frame loss rate (i.e., the HA sends one SDU in a given PDU).
     *
     * Table 3.1
     * Flush Timeout (FT) (unicast)                     1
     * Pre-Transmission Offset (PTO) (broadcast)        0
     * Max_PDU_C_To_P (unicast),                        >= Maximum size of an SDU that contains one
     * Max_PDU_P_To_C (unicast)                         LC3 Media Packet (see Section 4.2 in BAP) with
     *                                                  codec frames at 10 ms frame duration plus
     *                                                  5 octets of framed ISOAL header
     * Max_PDU (broadcast)                              >= Maximum size of an SDU that contains one
     *                                                  LC3 Media Packet with codec frames at 10 ms
     *                                                  frame duration plus 5 octets of framed
     *                                                  ISOAL header
     * Framing                                          Framed
     * BN                                               1
     *
     * Table 3.2
     * HA shall support accepting the establishment of a CIS and
     * synchronizing to a BIS with the parameters
     * Flush Timeout (FT) (unicast)                     1
     * Pre-Transmission Offset (PTO) (broadcast)        0
     * Max_PDU_C_To_P (unicast),                        >= Maximum size of an SDU that contains one
     * Max_PDU_P_To_C (unicast)                         LC3 Media Packet with codec frames at 10 ms
     *                                                  frame duration
     * Max_PDU (broadcast)                              >= Maximum size of an SDU that contains one
     *                                                  LC3 Media Packet with codec frames at 10 ms
     *                                                  frame duration
     * Framing                                          Unframed
     * BN                                               1
     *
     * The maximum SDU size referred to in Table 3.1 and Table 3.2 of HAP SPec
     * is equal to the maximum size of the LC3 Media Packet that is supported
     * by the HA (see Section 4.2 in BAP), and is calculated as:
     * Supported_Max_Codec_Frames_Per_SDU * (octets 2-3 of Supported_Octets_Per_Codec_Frame).
     *
     * Section 3.5.2
     * If the HA is equipped with one or more batteries, the HA should
     * instantiate the Battery Service.
     *
     * Section 3.5.3
     * HA may instantiate one instance of the Immediate Alert Service (IAS)
     * Not Mandatory
     *
     * Section 3.6
     * If the HA is part of a Binaural Hearing Aid Set:
     * -> CSIP Set Member M
     * -> Coordinated Set Size characteristic M
     * -> Value of Coordinated Set Size characteristic: 0x02. M
     *
     * Section 3.7
     * BAP Audio Sink role    M
     * BAP Audio Source role  O
     *
     * If the HA supports the BAP Audio Source role, the HA shall support the
     * BAP Audio Source and BAP Audio Sink role concurrently and shall support
     * the establishment of a bidirectional CIS with a HAUC.
     *
     * HA is part of Binaural Hearing Aid Set:
     * Sink Audio Locations M
     * Front Left
     * Front Right
     * Front Right and Front Left
     * M to support atleast one of the above.
     *
     * Banded Hearing Aid in the HA role:
     * Sink Audio Locations M
     * Front Right and Front Left M
     *
     * As per BAP Spec, Table 3.5
     * Codec Specific Capabilities
     * 16_2 M
     * 24_2 M
     *
     * As per BAP Spec, Table 5.2
     * QoS Configuration settings
     * 16_2_1 16_2_2
     * 24_2_1 24_2_2
     *
     * Section 3.7
     * Supported Sink Contexts Types M
     * Conversational  M
     * Media           M
     * Live            M
     * In addition to the ‘Unspecified’ Context Type mandated by BAP.
     *
     * Section 3.7
     * The HA shall support a Presentation Delay range in the Codec Configured
     * state that includes the value of 20ms, in addition to the requirement
     * specified in Table 5.2 of BAP Spec.
     * By default, when M Conf Settings are chosen, the Presentation Delay
     * automatically defaults to 40ms.
     *
     * Section 3.8
     * BAP Broadcast Sink M
     *
     * As per BAP Spec, Table 3.11
     * Codec Configuration Setting
     * 16_2 M
     * 24_2 M
     *
     * Section 3.9
     * HA may instantiate one or more instances of Audio Input Control Service (AICS)
     * If the HA supports the Volume Balance feature (Section 3.1 of HAP Spec)
     * and the HA is part of a Binaural Hearing Aid Set, the HA shall
     * instantiate one instance of Volume Offset Control Service (VOCS).
     * If the HA supports the Volume Balance feature (Section 3.1 of HAP Spec)
     * and the HA is a Banded Hearing Aid, the HA shall instantiate
     * two instances of VOCS.
     */
    /* Register PACS */
    /* appl_ga_register_pacs(APPL_GA_HAP_ROLE_HA); */
    /* Register ASCS */
    /* appl_ga_ucs_register_ascs(APPL_GA_HAP_ROLE_HA); */
    /* Register CSIS */
}

void appl_hap_client_init_HAUC(void)
{
    /* Requirements as per HAP Spec */
    /* HAP Role: HAUC: M support.
     * Table 2.1
     * CAP Role: Initiator. M                  ------> 1
     * Table 2.2
     * BAP Unicast Client       M              ------> 2
     * CCP Call Control Server  O
     * CSIP Set Coordinator     M
     *
     * Section 4.1
     * PL: LE 2M PHY                           ------> 3
     * To provide interoperability with a device in the HA role that does not
     * support an LC3 codec configuration with a 7.5 ms frame duration,
     * a device in the HAUC role shall support establishing an isochronous
     * stream with the parameters listed in Table 4.1. These parameters make it
     * possible to avoid segmentation of SDUs and avoid an undesirable increase
     * of the frame loss rate.
     *
     * Table 4.1
     * Flush Timeout (FT)                               1
     * Max_PDU_C_To_P (unicast),                        >= Maximum size of an SDU that contains one
     * Max_PDU_P_To_C (unicast)                         LC3 Media Packet (see Section 4.2 in BAP) with
     *                                                  codec frames at 10 ms frame duration plus
     *                                                  5 octets of framed ISOAL header
     * Framing                                          Framed
     * BN                                               1
     *
     * The maximum SDU size referred to in Table 4.1 is equal to the maximum
     * size of the LC3 Media Packet (see Section 4.2 in BAP) that is supported
     * by the HAUC, and can be calculated as:
     * (Octets_Per_Codec_Frame * (number of bits set to a value of 0b1 in Audio_Channel_Allocation) * Codec_Frame_Blocks_Per_SDU).
     *
     * Section 4.3
     * BAP Audio Source role M
     *
     * As per BAP Spec, Table 3.11
     * Codec Configuration Settings
     * 16_2 M
     *
     * As per BAP Spec, Table 5.2
     * QoS Configuration settings
     * 16_2_1 16_2_2
     *
     * Section 4.3
     * The HAUC shall support establishing two unicast Audio Streams: one for
     * the Front Left Audio Location and one for the Front Right Audio Location
     * (i.e., a CIG with two unidirectional CISes). Each Audio Stream
     * transports one Audio Channel from the HAUC to each of the two HAs of a
     * Binaural Hearing Aid Set, or to a Banded Hearing Aid.
     *
     * Note: The HAUC can optionally support sending two Audio Channels in one
     * unicast Audio Stream as specified in BAP (Section 4).
     *
     * If the HAUC supports the CCP Call Control Server role, it shall support
     * at least one bidirectional CIS and the BAP Audio Sink role.
     * The HAUC should use a bidirectional CIS instead of two CISes in opposite
     * directions to optimize bandwidth usage and power consumption.
     */
}

void appl_hap_client_init_HARC(void)
{
    /* Requirements as per HAP Spec */
    /* HAP Role: HAUC: M support.
     * Table 2.1
     * CAP Role: Commander. M                  ------> 1
     * Table 2.2
     * VCP Volume Controller       M           ------> 2
     * MICP Microphone Controller  O
     * CSIP Set Coordinator        M
     * HAS Client                  M
     *
     * Section 5.1
     * PL: LE 2M PHY                           ------> 3
     */
}

void main_hap_menu_operations(void)
{
    int        choice;
    int        menu_choice;
    GA_RESULT  retval;
    UINT8      isClient;
    HAP_ROLE   role;
    UINT8      index;
    UINT8      marker;
    UINT8      conn_dev_index;
#ifdef HAP_CLIENT
    UCHAR             i;
    HAP_CE_WRITE_REQ  req;
    UINT8             size;
    UINT8             write_req_val[10U] = { 0U };
    UCHAR             name[APPL_HAP_MAX_PRESET_RECORD_NAME];
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER
    /* Notification Data Array used by Server */
    HAP_CHR_VAL  hap_ha_ntf_data;
#endif /* HAP_SERVER */

    /* Intialize */
    retval = GA_FAILURE;
    isClient = 0U;
    index = 0U;
    role = 0xFFU;
    conn_dev_index = 0U;
#ifdef HAP_CLIENT
    req.value.val = NULL;
    size = 0U;
    i = 0U;
#endif /* HAP_CLIENT */

    GA_LOOP_FOREVER()
    {
        choice = 1;
        CONSOLE_OUT("%s", appl_hap_appl_menu);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                APPL_TRC("GA_hap_init...\n");
                retval = GA_hap_init(appl_hap_cb);
#ifdef HAP_SERVER
                appl_hap_ha_initialize_records();
#endif /* HAP_SERVER */
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                break;

            case 3:
                CONSOLE_OUT("Input HAP Type, Client: 1, Server: 0\n");
                CONSOLE_IN("%x", &choice);
                isClient = (UINT8)choice;

                if (isClient)
                {
#ifdef HAP_CLIENT
                    CONSOLE_OUT("Input HAP Role\n");
                    CONSOLE_OUT("HA Unicast Client: 0x02, HA Remote Controller: 0x04\n");
                    CONSOLE_IN("%x", &choice);
                    role = (UINT8)choice;
                    APPL_TRC("GA_hap_register_role...\n");
                    retval = GA_hap_register_role(role, 0U);
                    if (GA_SUCCESS == retval)
                    {
                        if (HAP_ROLE_HAUC & role)
                        {
                            ga_role |= APPL_GA_HAP_ROLE_HAUC;
                            appl_hap_client_init_HAUC();
                            appl_ga_utils_set_adv_data_for_current_hap_role
                            (
                                APPL_GA_HAP_ROLE_HAUC,
                                0U
                            );
                        }
                        else if (HAP_ROLE_HARC & role)
                        {
                            ga_role |= APPL_GA_HAP_ROLE_HARC;
                            appl_hap_client_init_HARC();
                        }
                    }
#else /* HAP_CLIENT */
                    APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                }
                else
                {
#ifdef HAP_SERVER
                    role = HAP_ROLE_HA;
                    APPL_TRC("GA_hap_register_role...\n");
                    appl_has_char_info[0U].features = (HAP_HEARING_AID_TYPE_BINAURAL |
                    HAP_HEARING_AID_PRESET_SYNC_SUPP_YES |
                    HAP_HEARING_AID_INDEPENDENT_PRESETS_DIFF |
                    HAP_HEARING_AID_DYNAMIC_PRESETS_MAY_CHANGE |
                    HAP_HEARING_AID_WRITABLE_PRESETS_SUPP);
                    CONSOLE_OUT("Register HAP Preset Records\n");
                    CONSOLE_OUT("1. Use default 4 Preset Records\n");
                    CONSOLE_OUT("2. Create a new set of Preset Records\n");
                    CONSOLE_IN("%x", &choice);

                    if (0x01 == choice)
                    {
                        appl_hap_ha_register_preset_records();
                        /* By default, the active preset index will be the first record */
                        appl_has_char_info[0U].active_preset_index = appl_has_char_info[0U].preset_records[0U].index;
                    }
                    else
                    {
                        appl_hap_ha_create_preset_records();
                        CONSOLE_OUT("Set Active Preset Index\n");
                        CONSOLE_IN("%x", &choice);

                        appl_has_char_info[0U].active_preset_index = (UINT8)choice;

                    }
                    retval = GA_hap_register_role(role, appl_has_char_info[0U].features);
                    if (GA_SUCCESS == retval)
                    {
                        ga_role |= APPL_GA_HAP_ROLE_HA;
                        appl_hap_server_init_HA();
                        appl_ga_utils_set_adv_data_for_current_hap_role
                        (
                            APPL_GA_HAP_ROLE_HA,
                            appl_has_char_info[0U].features
                        );
                    }
#else /* HAP_SERVER */
                    APPL_ERR("Enable HAP_SERVER !");
#endif /* HAP_SERVER */
                }
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                break;

            case 4:
                APPL_TRC("GA_hap_shutdown...\n");
                retval = GA_hap_shutdown();
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                break;

            case 10:
#ifdef HAP_CLIENT
                if (APPL_GA_INIT_STATUS_GA == ga_init_status)
                {
                    conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                    /* Fetch Device from conn_dev_index */
                    retval = appl_ga_utils_get_conn_device
                             (
                                 conn_dev_index,
                                 &ga_hap_peer_bd_addr
                             );
                }
                else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
                {
                    CONSOLE_OUT("Enter Peer BD Address [In HEX]: ");
                    appl_get_bd_addr(ga_hap_peer_bd_addr.bd_addr);
                    CONSOLE_OUT("Enter endpoint type: ");
                    CONSOLE_IN("%d", &choice);
                    ga_hap_peer_bd_addr.bd_type = (UINT8)choice;
                }

                APPL_TRC("GA_hap_setup_context...\n");
                retval = GA_hap_setup_context
                         (
                             &ga_hap_peer_bd_addr,
                             &appl_hap_handle
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("appl_hap_handle: 0x%02x\n", appl_hap_handle);
                }
#else /* HAP_CLIENT */
                APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                break;

            case 11:
#ifdef HAP_CLIENT
            {
                CONSOLE_OUT("\n1. Release\n2. Close\n");
                CONSOLE_OUT("Enter your choice: ");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    APPL_TRC("GA_hap_release...\n");
                    retval = GA_hap_release(appl_hap_handle);
                    APPL_TRC("Retval - ");
                    appl_hap_parse_ga_result(retval);
                }
                else if (2 == choice)
                {
                    APPL_TRC("GA_hap_close...\n");
                    retval = GA_hap_close(appl_hap_handle);
                    APPL_TRC("Retval - ");
                    appl_hap_parse_ga_result(retval);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
            }
#else /* HAP_CLIENT */
                APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                break;

            case 12:
#ifdef HAP_CLIENT
                APPL_TRC("GA_hap_read_ha_features: \n");
                retval = GA_hap_read_ha_features(appl_hap_handle);
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
#else /* HAP_CLIENT */
                APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                break;

            case 13:
#ifdef HAP_CLIENT
                APPL_TRC("GA_hap_read_active_preset_index: \n");
                retval = GA_hap_read_active_preset_index(appl_hap_handle);
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
#else /* HAP_CLIENT */
                APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                break;

            case 14:
#ifdef HAP_CLIENT
                marker = 0U;
                CONSOLE_OUT("Input the Control Point Opcode [In HEX]:\n\n");
                CONSOLE_OUT("Read Presets Request [0x01]\n");
                CONSOLE_OUT("Write Preset Name [0x04]\n");
                CONSOLE_OUT("Set Active Preset [0x05]\n");
                CONSOLE_OUT("Set Next Preset [0x06]\n");
                CONSOLE_OUT("Set Previous Preset [0x07]\n");
                CONSOLE_OUT("Set Active Preset - Synchronized Locally [0x08]\n");
                CONSOLE_OUT("Set Next Preset - Synchronized Locally [0x09]\n");
                CONSOLE_OUT("Set Previous Preset - Synchronized Locally [0x0A]\n");
                CONSOLE_IN("%x", &choice);

                write_req_val[marker] = (UINT8)choice;

                if ((HAP_CP_OPCODE_SET_NEXT_PRESET == write_req_val[marker]) ||
                    (HAP_CP_OPCODE_SET_PREV_PRESET == write_req_val[marker]) ||
                    (HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY == write_req_val[marker]) ||
                    (HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY == write_req_val[marker]))
                {
                    /* Incrementing Opcode Byte */
                    marker++;
                    /* No more Inputs for this Opcode */
                }
                else if (HAP_CP_OPCODE_READ_PRESETS_REQ == write_req_val[marker])
                {
                    /* Incrementing Opcode Byte */
                    marker++;

                    CONSOLE_OUT("Enter Index [In HEX] [0x01 - 0xFF]:\n");
                    CONSOLE_IN("%x", &choice);
                    write_req_val[marker] = (UINT8)choice;

                    /* Incrementing Index Byte */
                    marker++;

                    CONSOLE_OUT("Enter NumPresets [In HEX] [0x01 - 0xFF]:\n");
                    CONSOLE_IN("%x", &choice);
                    write_req_val[marker] = (UINT8)choice;

                    /* Incrementing NumPresets Byte */
                    marker++;
                }
                else if ((HAP_CP_OPCODE_SET_ACTIVE_PRESET == write_req_val[marker]) ||
                         (HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY == write_req_val[marker]))
                {
                    /* Incrementing Opcode Byte */
                    marker++;

                    CONSOLE_OUT("Enter Index [In HEX] [0x01 - 0xFF]:\n");
                    CONSOLE_IN("%x", &choice);
                    write_req_val[marker] = (UINT8)choice;

                    /* Incrementing Index Byte */
                    marker++;
                }
                else if (HAP_CP_OPCODE_WRITE_PRESET_NAME == write_req_val[marker])
                {
                    /* Incrementing Opcode Byte */
                    marker++;

                    CONSOLE_OUT("Enter Preset Record Index [In HEX] [0x01 - 0xFF]:\n");
                    CONSOLE_IN("%x", &choice);
                    write_req_val[marker] = (UINT8)choice;

                    /* Incrementing Index Byte */
                    marker++;

                    CONSOLE_OUT("Enter size of Preset Record Name: ");
                    CONSOLE_IN("%d", &choice);
                    size = (UINT8)choice;

                    if (APPL_HAP_MAX_PRESET_RECORD_NAME < size)
                    {
                        APPL_INF("Size of the Name should not exceed 40 Chars\n");
                        size = (APPL_HAP_MAX_PRESET_RECORD_NAME - 1U);
                    }

                    CONSOLE_OUT("Enter Preset Record Name: ");
                    for (i = 0U; i < size; i++)
                    {
                        CONSOLE_IN(" %c", &name[i]);
                    }
                    name[i] = '\0';
                    size++;

                    GA_mem_copy(&write_req_val[marker], name, (size * sizeof(UINT8)));
                    /* Incrementing size bytes */
                    marker += size;
                }
                else
                {
                    APPL_INF("Invalid Opcode\n");
                }

                /* Once the Data is ready, Send CP */
                if (1U <= marker)
                {
                    req.value.val = &write_req_val[0U];
                    req.value.len = marker;
                    CONSOLE_OUT("1. Request \n2. Command\n");
                    CONSOLE_OUT("Enter Write Request Type\n");
                    CONSOLE_IN("%x", &choice);
                    if (1 == choice)
                    {
                        /* Request */
                        req.type = 0x00U;
                    }
                    else if (2 == choice)
                    {
                        /* Command */
                        req.type = 0x01U;
                    }

                    retval = GA_hap_write_ha_preset_cp(appl_hap_handle, &req);
                    APPL_TRC("Retval - ");
                    appl_hap_parse_ga_result(retval);
                }
#else /* HAP_CLIENT */
                APPL_ERR("Enable HAP_CLIENT !");
#endif /* HAP_CLIENT */
                break;

            case 20:
#ifdef HAP_SERVER
                if (APPL_GA_INIT_STATUS_GA == ga_init_status)
                {
                    conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                    /* Fetch Device from conn_dev_index */
                    retval = appl_ga_utils_get_conn_device
                            (
                                conn_dev_index,
                                &ga_has_peer_bd_addr
                            );
                }
                else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
                {
                    CONSOLE_OUT("Enter Peer BD Address [In HEX]: ");
                    appl_get_bd_addr(ga_has_peer_bd_addr.bd_addr);
                    CONSOLE_OUT("Enter endpoint type: ");
                    CONSOLE_IN("%d", &choice);
                    ga_has_peer_bd_addr.bd_type = (UINT8)choice;
                }
#else /* HAP_SERVER */
                APPL_ERR("Enable HAP_SERVER !");
#endif /* HAP_SERVER */
                break;

            case 25:
#ifdef HAP_SERVER
                APPL_TRC("Notify Features (Optional Property)\n");
                hap_ha_ntf_data.val = &appl_has_char_info[0U].features;
                hap_ha_ntf_data.len = sizeof(appl_has_char_info[0U].features);
                APPL_TRC("GA_hap_notify_features: \n");
                retval = GA_hap_notify_features
                         (
                             &ga_has_peer_bd_addr,
                             &hap_ha_ntf_data
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
#else /* HAP_SERVER */
                APPL_ERR("Enable HAP_SERVER !");
#endif /* HAP_SERVER */
                break;

            case 26:
#ifdef HAP_SERVER
                marker = 0U;
                CONSOLE_OUT("Input the Control Point Opcode [In HEX]:\n");
                CONSOLE_OUT(" - Preset Changed [0x03]\n");
                CONSOLE_IN("%x", &choice);
                if (HAP_CP_OPCODE_PRESET_CHANGED == choice)
                {
                    retval = appl_hap_send_preset_changed_ntf();
                    APPL_TRC("Retval - ");
                    appl_hap_parse_ga_result(retval);
                }
#else /* HAP_SERVER */
                APPL_ERR("Enable HAP_SERVER !");
#endif /* HAP_SERVER */
                break;

            case 27:
#ifdef HAP_SERVER
                APPL_TRC("Notify Active Preset Index\n");
                hap_ha_ntf_data.val = &appl_has_char_info[0U].active_preset_index;
                hap_ha_ntf_data.len = sizeof(appl_has_char_info[0U].active_preset_index);
                APPL_TRC("GA_hap_notify_active_preset_index: \n");
                retval = GA_hap_notify_active_preset_index
                         (
                             &ga_has_peer_bd_addr,
                             &hap_ha_ntf_data
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
#else /* HAP_SERVER */
                APPL_ERR("Enable HAP_SERVER !");
#endif /* HAP_SERVER */
                break;

            case 40:
                /*
                 * BAP Unicast Server role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                /*
                 * HA shall support BAP Audio Sink role and may support
                 * BAP Audio Source role.
                 * Refer: Spec HAP_v1.0, Page: 14, Section: 3.7
                 */
                main_ucs_operations();
                break;

            case 41:
                /*
                 * VCP Volume Renderer role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_vcp_rd_operations();
                break;

            case 42:
                /*
                 * MICP Microphone Device role
                 * C.2: Mandatory if the HA supports the BAP Audio Source role
                 *      (see Section 3.7); otherwise Excluded.
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_micp_dev_operations();
                break;

            case 43:
                /*
                 * CCP Call Control Client
                 * Optional
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                main_ccp_ce_menu_operations();
                break;

            case 44:
                /*
                 * CSIP Set Member role
                 * C.1: Mandatory if the HA is capable of being part of a
                 * Binaural Hearing Aid Set; otherwise Excluded.
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_csip_sm_operations();
                break;

            case 45:
                /*
                 * Broadcast Sink role
                 * Refer: Spec HAP_v1.0, Page 15, Section: 3.8
                 */
                main_sd_operations();
                break;

            case 50:
                /*
                 * BAP Unicast Client role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                /*
                 * HAUC shall support BAP Audio Source role.
                 * Refer: Spec HAP_v1.0, Page: 18, Section: 4.3
                 */
                main_ucc_operations();
                break;

            case 51:
                /*
                 * CCP Call Control Server
                 * Optional
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                main_ccp_se_menu_operations();
                break;

            case 52:
                /*
                 * CSIP Set Coordinator role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_csip_sc_operations();
                break;

            case 60:
                /*
                 * VCP Volume Controller role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_vcp_ct_operations();
                break;

            case 61:
                /*
                 * MICP Microphone Controller role
                 * Optional
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_micp_ct_operations();
                break;

            case 62:
                /*
                 * CSIP Set Coordinator role
                 * Mandatory
                 * Refer: Spec HAP_v1.0, Table No: 2.2
                 */
                appl_csip_sc_operations();
                break;

            case 80:
            {
#ifdef HAP_SUPPORT_CONTEXT_MANAGE
                appl_hap_has_handle_access = appl_hap_handle;

                APPL_TRC("GA_hap_get_context_info ...\n");
                retval = GA_hap_get_context_info
                         (
                             &ga_hap_peer_bd_addr,
                             &appl_hap_has_handle_access,
                             &appl_hap_has_range_ptr,
                             &appl_hap_has_char_info_ptr,
                             &appl_hap_has_char_count
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("appl_hap_has_char_count:0x%02x\n", appl_hap_has_char_count);

                    appl_hap_has_range = *appl_hap_has_range_ptr;

                    /**
                     *  Reference to allocate to save locally.
                     *  This can be written to persistent and retrieved
                     *  upon the next connection if bonded device
                     */
                    appl_hap_has_char_info = GA_alloc_mem(appl_hap_has_char_count * sizeof(GA_CHAR_INFO));
                    if (NULL != appl_hap_has_char_info)
                    {
                        GA_mem_copy
                        (
                            appl_hap_has_char_info,
                            appl_hap_has_char_info_ptr,
                            appl_hap_has_char_count * sizeof(GA_CHAR_INFO)
                        );
                    }
                }
#else /* HAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("HAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* HAP_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 81:
            {
#ifdef HAP_SUPPORT_CONTEXT_MANAGE
                if (NULL == appl_hap_has_char_info)
                {
                    APPL_ERR("appl_hap_has_char_info is NULL !\n");
                    APPL_TRC("Call GA_hap_get_context_info \n");
                    break;
                }

                appl_hap_has_char_info_ptr = appl_hap_has_char_info;
                appl_hap_has_range_ptr = &appl_hap_has_range;

                APPL_TRC("GA_hap_set_context_info ...\n");
                retval = GA_hap_set_context_info
                         (
                             &ga_hap_peer_bd_addr,
                             &appl_hap_has_handle_access,
                             &appl_hap_has_range_ptr,
                             &appl_hap_has_char_info_ptr,
                             &appl_hap_has_char_count
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    appl_hap_handle = appl_hap_has_handle_access;

                    /* Free the allocated memory */
                    GA_free_mem(appl_hap_has_char_info);
                    appl_hap_has_char_info = NULL;
                }
#else /* HAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("HAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* HAP_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 87:
#ifdef HAP_SUPPORT_CONTEXT_MANAGE
                /**
                 * This is to free up the allocated char. info. memory
                 * If incase GA_hap_set_context_info() is NOT called after
                 * GA_hap_get_context_info().
                 */

                /* Free the allocated memory for HAS char. info. */
                if (NULL != appl_hap_has_char_info)
                {
                    APPL_TRC("Freeing appl_hap_has_char_info ...\n");
                    GA_free_mem(appl_hap_has_char_info);
                    appl_hap_has_char_info = NULL;
                }
#else /* HAP_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("HAP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* HAP_SUPPORT_CONTEXT_MANAGE */
                break;

            case 90:
#ifdef HAP_CLIENT
#ifdef HAP_SUPPORT_CONFIG_SELECTION
                APPL_TRC("GA_hap_update_ntf_configuration...\n");
                retval = GA_hap_update_ntf_configuration
                         (
                             (/* HAS_CHAR_CONFIG(HAP_CHAR_ID_CHAR_HA_FEATURES) |
                             HAS_CHAR_CONFIG(HAP_CHAR_ID_HA_PRESET_CP) | */
                             HAS_CHAR_CONFIG(HAP_CHAR_ID_ACTIVE_PRESET_INDEX))
                         );
                APPL_TRC("Retval - ");
                appl_hap_parse_ga_result(retval);
#else /* HAP_SUPPORT_CONFIG_SELECTION */
                APPL_ERR("HAP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* HAP_SUPPORT_CONFIG_SELECTION */
#else /* HAP_CLIENT */
                APPL_TRC("Enable HAP_CLIENT!");
#endif /* HAP_CLIENT */
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

GA_RESULT appl_hap_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
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

    switch(evt_id)
    {
#ifdef HAP_CLIENT
        case HAP_SETUP_CNF:
            GA_CB_ENTRY("HAP Client");
            APPL_TRC("[Profile]     : HAP (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAP CLIENT-SETUP\n");
            APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_ce_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        case HAP_RELEASE_CNF:
            GA_CB_ENTRY("HAP Client");
            APPL_TRC("[Profile]     : HAP (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAP CLIENT-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_ce_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        case HAP_HA_GET_FEATURES_CNF: /* Fall - through */
        case HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF:
            GA_CB_ENTRY("HAP Client");
            APPL_TRC("[Profile]     : HAP (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAP CLIENT-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_ce_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        case HAP_HA_PRESET_CP_CNF:
            GA_CB_ENTRY("HAP Client");
            APPL_TRC("[Profile]     : HAP (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAP CLIENT-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_ce_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        case HAP_HA_FEATURES_NTF: /* Fall - through */
        case HAP_HA_PRESET_CP_NTF: /* Fall - through */
        case HAP_HA_GET_ACTIVE_PRESET_INDEX_NTF:
            GA_CB_ENTRY("HAP Client");
            APPL_TRC("[Profile]     : HAP (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAP CLIENT EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAP CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_ce_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER
        case HAP_HA_GET_FEATURES_IND: /* Fall - through */
        case HAP_HA_GET_ACTIVE_PRESET_INDEX_IND:
            GA_CB_ENTRY("HAP Server");
            APPL_TRC("[Profile]     : HAS (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAS SERVER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAS SERVER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_se_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

        case HAP_HA_PRESET_CP_IND:
            GA_CB_ENTRY("HAP Server");
            APPL_TRC("[Profile]     : HAS (0x%4X)\n", GA_SERVICE_HAS);
            APPL_TRC("[SECTION]     : HAS SERVER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : HAS SERVER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_hap_se_cb
                     (
                         device,
                         ctx,
                         evt_id,
                         evt_status,
                         evt_data
                     );
            break;

#endif /* HAP_SERVER */

        default:
            APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
            retval = GA_FAILURE;
    }

    GA_CB_EXIT();

    return retval;
}

void appl_hap_parse_features(HAP_FEATURES features)
{
    if (HAP_HEARING_AID_TYPE_MONAURAL ==
       (features & HAP_HEARING_AID_TYPE_MONAURAL))
    {
        APPL_TRC
        (
            "\t-Monaural Hearing Aid (0x%02X)\n",
            HAP_HEARING_AID_TYPE_MONAURAL
        );
    }
    else if (HAP_HEARING_AID_TYPE_BANDED ==
       (features & HAP_HEARING_AID_TYPE_BANDED))
    {
        APPL_TRC
        (
            "\t-Banded Hearing Aid (0x%02X)\n",
            HAP_HEARING_AID_TYPE_BANDED
        );
    }
    else if (HAP_HEARING_AID_TYPE_BANDED ==
       (features & HAP_HEARING_AID_TYPE_BANDED))
    {
        APPL_TRC
        (
            "\t-Binaural Hearing Aid (0x%02X)\n",
            HAP_HEARING_AID_TYPE_BINAURAL
        );
    }

    if (HAP_HEARING_AID_PRESET_SYNC_SUPP_NO ==
       (features & HAP_HEARING_AID_PRESET_SYNC_SUPP_NO))
    {
        APPL_TRC
        (
            "\t-Preset Synchronization is not supported (0x%02X)\n",
            HAP_HEARING_AID_PRESET_SYNC_SUPP_NO
        );
    }
    else if (HAP_HEARING_AID_PRESET_SYNC_SUPP_YES ==
       (features & HAP_HEARING_AID_PRESET_SYNC_SUPP_YES))
    {
        APPL_TRC
        (
            "\t-Preset Synchronization is supported (0x%02X)\n",
            HAP_HEARING_AID_PRESET_SYNC_SUPP_YES
        );
    }

    if (HAP_HEARING_AID_INDEPENDENT_PRESETS_SAME ==
       (features & HAP_HEARING_AID_INDEPENDENT_PRESETS_SAME))
    {
        APPL_TRC("\t-The list of preset records on this server is identical "
        "to the list of preset records in the other server of the coordinated "
        "set (0x%02X)\n", HAP_HEARING_AID_INDEPENDENT_PRESETS_SAME);
    }
    else if (HAP_HEARING_AID_INDEPENDENT_PRESETS_DIFF ==
       (features & HAP_HEARING_AID_INDEPENDENT_PRESETS_DIFF))
    {
        APPL_TRC("\t-The list of preset records on this server may be "
        "different from the list of preset records in the other server of the "
        "coordinated set (0x%02X)\n", HAP_HEARING_AID_INDEPENDENT_PRESETS_DIFF);
    }

    if (HAP_HEARING_AID_DYNAMIC_PRESETS_DOES_NOT_CHANGE ==
       (features & HAP_HEARING_AID_DYNAMIC_PRESETS_DOES_NOT_CHANGE))
    {
        APPL_TRC
        (
            "\t-The list of preset records does not change (0x%02X)\n",
            HAP_HEARING_AID_DYNAMIC_PRESETS_DOES_NOT_CHANGE
        );
    }
    else if (HAP_HEARING_AID_DYNAMIC_PRESETS_MAY_CHANGE ==
       (features & HAP_HEARING_AID_DYNAMIC_PRESETS_MAY_CHANGE))
    {
        APPL_TRC
        (
            "\t-The list of preset records may change (0x%02X)\n",
            HAP_HEARING_AID_DYNAMIC_PRESETS_MAY_CHANGE
        );
    }

    if (HAP_HEARING_AID_WRITABLE_PRESETS_NO_SUPP ==
       (features & HAP_HEARING_AID_WRITABLE_PRESETS_NO_SUPP))
    {
        APPL_TRC
        (
            "\t-The server does not support writable preset records (0x%02X)\n",
            HAP_HEARING_AID_WRITABLE_PRESETS_NO_SUPP
        );
    }
    else if (HAP_HEARING_AID_WRITABLE_PRESETS_SUPP ==
       (features & HAP_HEARING_AID_WRITABLE_PRESETS_SUPP))
    {
        APPL_TRC
        (
            "\t-The server supports writable preset records (0x%02X)\n",
            HAP_HEARING_AID_WRITABLE_PRESETS_SUPP
        );
    }

    return;
}

void appl_hap_fetch_preset_record_properties(UINT8* prop)
{
    int  choice;

    CONSOLE_OUT("Enter the Properties for the Preset Record:\n");
    CONSOLE_OUT("Property: Writable\n");
    CONSOLE_OUT("1. No: The name of the preset cannot be written by the client\n");
    CONSOLE_OUT("2. Yes: The name of the preset can be written by the client\n");

    CONSOLE_IN("%d", &choice);
    if (1 == choice)
    {
        /* Properties : The name of the preset cannot be written by the client */
        *prop |= HAP_PROP_NAME_OF_PRESET_WRITABLE_NO;
    }
    else if (2 == choice)
    {
        /* Properties : The name of the preset can be written by the client */
        *prop |= HAP_PROP_NAME_OF_PRESET_WRITABLE_YES;
    }

    CONSOLE_OUT("Property: isAvailable\n");
    CONSOLE_OUT("1. No: The preset is unavailable\n");
    CONSOLE_OUT("2. Yes: The preset is available\n");

    CONSOLE_IN("%d", &choice);
    if (1 == choice)
    {
        /* Properties : The preset is unavailable */
        *prop |= HAP_PROP_IS_AVAILABLE_NO;
    }
    else if (2 == choice)
    {
        /* Properties : The preset is available */
        *prop |= HAP_PROP_IS_AVAILABLE_YES;
    }

    return;
}

void appl_hap_parse_ga_result(GA_RESULT status)
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
        /* Application error code */
        case HAP_APPL_ERR_CODE_INVALID_OPCODE:
            APPL_ERR("Invalid Opcode !");
            break;

        case HAP_APPL_ERR_CODE_WRITE_NAME_NT_ALLOWED:
            APPL_ERR("Write Name Not Allowed !");
            break;

        case HAP_APPL_ERR_CODE_PRESET_SYNC_NT_SUPP:
            APPL_ERR("Preset Synchronization Not Supported !");
            break;

        case HAP_APPL_ERR_CODE_PRESET_OP_NOT_POSSIBLE:
            APPL_ERR("Preset Operation Not Possible !");
            break;

        case HAP_APPL_ERR_CODE_INVALID_PARAM_LENGTH:
            APPL_ERR("Invalid Parameter Length !");
            break;

        case APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE:
            APPL_ERR("Out of Range !");
            break;

        case APPL_HAP_ATT_ERR_CODE_PROC_ALREADY_IN_PROG:
            APPL_ERR("Procedure Already in Progress !");
            break;

        /* Implementation related error codes */
        case GA_HAP_INVALID_STATE:
            APPL_ERR("Invalid State !");
            break;

        case GA_HAP_INVALID_PARAMS:
            APPL_ERR("Invalid Params !");
            break;

        case GA_HAP_CNTX_ALLOC_FAILED:
            APPL_ERR("Context allocation failed !");
            break;

        case GA_HAP_IN_BUSY_STATE:
            APPL_ERR("In Busy State !");
            break;

        case GA_HAP_CNTX_NOT_FOUND:
            APPL_ERR("Context not found !");
            break;

        case GA_HAP_INVALID_HANDLE:
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

#ifdef HAP_CLIENT
GA_RESULT appl_hap_ce_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
          )
{
    GA_RESULT     retval;
    HAP_FEATURES  hap_features;

    GA_IGNORE_UNUSED_PARAM(ctx);
    GA_IGNORE_UNUSED_PARAM(device);

    /* Initialize */
    retval = GA_SUCCESS;
    hap_features = 0xFFU;

    APPL_TRC("[Event Status]: ");
    appl_hap_parse_ga_result(evt_status);

    switch(evt_id)
    {
        case HAP_SETUP_CNF:
            APPL_TRC("[Event]       : HAP_SETUP_CNF (0x%02X)\n", evt_id);
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();

                    if (0U != evt_data->len)
                    {
                        APPL_TRC("Data Length: 0x%02X\n", evt_data->len);
                    }
                    else
                    {
                        APPL_INF("Data Length: 0x00\n");
                    }
                    if (NULL != evt_data->data)
                    {
                        APPL_TRC("Data:\n");
                        hap_features = *((HAP_FEATURES *)evt_data->data);
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Hearing Aid Features: 0x%02X\n",
                            evt_data->len,
                            hap_features
                        );
                        appl_hap_char_info[evt_data->hap_handle].features = *((HAP_FEATURES*)(evt_data->data));
                        appl_hap_parse_features(appl_hap_char_info[evt_data->hap_handle].features);
                    }
                    else
                    {
                        APPL_INF("Data: NULL\n");
                    }
                }
            }
            break;

        case HAP_RELEASE_CNF:
            APPL_TRC("[Event]       : HAP_RELEASE_CNF (0x%02X)\n", evt_id);
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();

                    if(0U != evt_data->len)
                    {
                        APPL_INF("Data Length: 0x%02X\n", evt_data->len);
                    }
                    else
                    {
                        APPL_TRC("Data Length: 0x00\n");
                    }
                    if(NULL == evt_data->data)
                    {
                        APPL_TRC("Data: NULL\n");
                    }
                }
            }
            break;

        case HAP_HA_FEATURES_NTF: /* Fall - through */
        case HAP_HA_GET_FEATURES_CNF:
            if (HAP_HA_GET_FEATURES_CNF == evt_id)
            {
                APPL_TRC
                (
                    "[Event]       : HAP_HA_GET_FEATURES_CNF (0x%02X)\n",
                    evt_id
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : HAP_HA_FEATURES_NTF (0x%02X)\n",
                    evt_id
                );
            }
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Hearing Aid Features (0x%04X)\n",
                        GA_CHAR_HA_FEATURES
                    );
                    GA_FORMAT_DISPLAY();
                    appl_hap_char_info[evt_data->hap_handle].features = *((HAP_FEATURES *)(evt_data->data));
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
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Hearing Aid Features: 0x%02X\n",
                            evt_data->len,
                            appl_hap_char_info[evt_data->hap_handle].features
                        );
                        appl_hap_parse_features(appl_hap_char_info[evt_data->hap_handle].features);
                    }
                    else
                    {
                        APPL_INF("Data: NULL\n");
                    }
                }
            }
            break;

        case HAP_HA_GET_ACTIVE_PRESET_INDEX_NTF: /* Fall - through */
        case HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF:
            if (HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF == evt_id)
            {
                APPL_TRC
                (
                    "[Event]       : HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF (0x%02X)\n",
                    evt_id
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : HAP_HA_GET_ACTIVE_PRESET_INDEX_NTF (0x%02X)\n",
                    evt_id
                );
            }
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Active Preset Index (0x%04X)\n",
                        GA_CHAR_ACTIVE_PRESET_INDEX
                    );
                    GA_FORMAT_DISPLAY();
                    appl_hap_char_info[evt_data->hap_handle].active_preset_index = *((UINT8 *)(evt_data->data));
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
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Active Preset Index: 0x%02X\n",
                            evt_data->len, *(UINT8 *)(evt_data->data)
                        );
                    }
                    else
                    {
                        APPL_INF("Data: NULL\n");
                    }
                }
            }
            break;

        case HAP_HA_PRESET_CP_CNF:
            APPL_TRC
            (
                "[Event]       : HAP_HA_PRESET_CP_CNF (0x%02X)\n",
                evt_id
            );
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Hearing Aid Preset Control Point (0x%04X)\n",
                        GA_CHAR_HA_PRESET_CP
                    );
                    GA_FORMAT_DISPLAY();
                    if(0U != evt_data->len)
                    {
                        APPL_INF("Data Length: 0x%02X\n", evt_data->len);
                    }
                    else
                    {
                        APPL_TRC("Data Length: 0x00\n");
                    }
                    if(NULL == evt_data->data)
                    {
                        APPL_TRC("Data: NULL\n");
                    }
                }
            }
            break;

        case HAP_HA_PRESET_CP_NTF:
            APPL_TRC
            (
                "[Event]       : HAP_HA_PRESET_CP_NTF (0x%02X)\n",
                evt_id
            );
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Cntx]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Hearing Aid Preset Control Point (0x%04X)\n",
                        GA_CHAR_HA_PRESET_CP
                    );
                    GA_FORMAT_DISPLAY();
                    appl_hap_parse_hap_cp(evt_data->data, evt_data->len);
                }
            }
            break;

        default:
            APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
            retval = GA_FAILURE;
            break;
    }

    return retval;
}

GA_RESULT appl_hap_parse_hap_cp(UCHAR *data, UINT16 len)
{
    GA_RESULT  retval;
    UINT8      opcode;
    UCHAR      i;
    UINT8      marker;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    marker = 0U;

    if ((NULL == data) || (0x00U == len))
    {
        return GA_FAILURE;
    }

    opcode = data[marker++];

    APPL_TRC("Data Length: 0x%02X\n", len);
    APPL_TRC("Data:\n");

    switch (opcode)
    {

    case HAP_CP_OPCODE_READ_PRESETS_REQ:
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Read Preset by Index Request (0x%02X)\n",
            (UCHAR)sizeof(opcode), opcode
        );
        APPL_TRC("Len: 0x%02X,  StartIndex: 0x%02X\n", (UCHAR)sizeof(UINT8), data[marker++]);
        APPL_TRC("Len: 0x%02X,  NumPresets: 0x%02X\n", (UCHAR)sizeof(UINT8), data[marker++]);
        appl_hap_parse_and_display_preset_record(&data[marker], (len - marker));
        break;

    case HAP_CP_OPCODE_READ_PRESETS_RSP:
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Read Preset Response (0x%02X)\n",
            (UCHAR)sizeof(opcode), opcode
        );

        APPL_TRC("Len: 0x%02X,  \n", (UCHAR)sizeof(UINT8));
        appl_hap_parse_and_display_is_last(data[marker++]);

        appl_hap_parse_and_display_preset_record(&data[marker], (len - marker));
        break;

    case HAP_CP_OPCODE_PRESET_CHANGED:
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Preset Changed (0x%02X)\n",
            (UCHAR)sizeof(opcode), opcode
        );

        APPL_TRC("Len: 0x%02X,  ChangeId: ", (UCHAR)sizeof(UINT8));
        appl_hap_parse_change_id(data[marker++]);

        APPL_TRC("Len: 0x%02X,  \n", (UCHAR)sizeof(UINT8));
        appl_hap_parse_and_display_is_last(data[marker++]);

        if (HAP_CP_CHANGE_ID_GENERIC_UPDATE == data[1U])
        {
            APPL_TRC("Len: 0x%02X,  PrevIndex: 0x%02X\n", (UCHAR)sizeof(UINT8), data[marker++]);
            appl_hap_parse_and_display_preset_record(&data[marker], (len - marker));
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Index: 0x%02X\n", (UCHAR)sizeof(UINT8), data[3U]);
        }
        break;

    case HAP_CP_OPCODE_WRITE_PRESET_NAME:
        APPL_TRC
        (
            "Len: 0x%02X,  Opcode: Preset Changed (0x%02X)\n",
            (UCHAR)sizeof(opcode), opcode
        );
        APPL_TRC("Len: 0x%02X,  Index: 0x%02X\n", (UCHAR)sizeof(UINT8), data[marker++]);
        appl_hap_parse_and_display_preset_record(&data[marker], (len - marker));
        break;

    case HAP_CP_OPCODE_SET_ACTIVE_PRESET: /* Fall-Through */
    case HAP_CP_OPCODE_SET_NEXT_PRESET: /* Fall-Through */
    case HAP_CP_OPCODE_SET_PREV_PRESET: /* Fall-Through */
    case HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY: /* Fall-Through */
    case HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY: /* Fall-Through */
    case HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY: /* Fall-Through */
    /* These opcodes will get Ntfs as part of Active Preset Index Char */
    /* TODO: Parsing? */
    break;

    default:
        retval = GA_FAILURE;
        break;
    }

    return retval;
}

void appl_hap_parse_change_id(UINT8 changeId)
{
    switch(changeId)
    {
    case HAP_CP_CHANGE_ID_GENERIC_UPDATE:
        APPL_TRC("Generic Update (0x%02X)\n", changeId);
        break;

    case HAP_CP_CHANGE_ID_PRESET_RECORD_DELETED:
        APPL_TRC("Preset Record Deleted (0x%02X)\n", changeId);
        break;

    case HAP_CP_CHANGE_ID_PRESET_RECORD_AVAILABLE:
        APPL_TRC("Preset Record Available (0x%02X)\n", changeId);
        break;

    case HAP_CP_CHANGE_ID_PRESET_RECORD_UNAVAILABLE:
        APPL_TRC("Preset Record Unavailable (0x%02X)\n", changeId);
        break;

    default:
        APPL_ERR("UnHandled ChangeId (0x%02X) !\n", changeId);
        break;
    }

    return;
}

void appl_hap_parse_and_display_preset_record(UCHAR * data, UINT16 len)
{
    UINT8 marker;
    UINT8 i;

    i = 0U;
    marker = 0U;

    APPL_TRC("Preset Record:\n");
    APPL_TRC("\tIndex: 0x%02X\n", data[marker++]);
    appl_hap_parse_and_display_preset_record_properties(data[marker]);
    /* Incrementing the properties Byte */
    marker++;
    APPL_TRC("\tName: ");

    /* TODO: Validate Length, since Spec does not mandate \0 */
    for (i = marker; i < (len - 2U); i++)
    {
        APPL_TRC("%c ", data[i]);
    }

    APPL_TRC("\n");

    return;
}
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER

void appl_hap_ha_initialize_records(void)
{
    UCHAR  i;

    i = 0U;

    /* Initialize the preset records */
    for (i = 0U; i < APPL_HAP_MAX_SUPP_PRESET_RECORDS; i++)
    {
        GA_mem_set
        (
            &appl_has_char_info[0U],
            0x00U,
            sizeof(APPL_HAS_CHAR_INFO)
        );
    }

    return;
}

void appl_hap_ha_register_preset_records(void)
{
    appl_has_char_info[0U].preset_records[0U].index = 1U;
    appl_has_char_info[0U].preset_records[0U].properties = HAP_PROP_NAME_OF_PRESET_WRITABLE_NO | HAP_PROP_IS_AVAILABLE_NO;
    appl_has_char_info[0U].preset_records[0U].name_len = 5U;
    /* Length is 4B + 1B for '\0' */
    GA_mem_copy
    (
        &appl_has_char_info[0U].preset_records[0U].name,
        "PR-1",
        ((appl_has_char_info[0U].preset_records[0U].name_len - 1U) * sizeof(UCHAR))
    );
    appl_has_char_info[0U].preset_records[0U].name[(appl_has_char_info[0U].preset_records[0U].name_len - 1U)] = '\0';
    appl_has_char_info[0U].num_of_preset_records++;

    appl_has_char_info[0U].preset_records[1U].index = 2U;
    appl_has_char_info[0U].preset_records[1U].properties = HAP_PROP_NAME_OF_PRESET_WRITABLE_YES | HAP_PROP_IS_AVAILABLE_YES;
    appl_has_char_info[0U].preset_records[1U].name_len = 5U;
    /* Length is 4B + 1B for '\0' */
    GA_mem_copy
    (
        &appl_has_char_info[0U].preset_records[1U].name,
        "PR-2",
        ((appl_has_char_info[0U].preset_records[1U].name_len - 1U) * sizeof(UCHAR))
    );
    appl_has_char_info[0U].preset_records[1U].name[(appl_has_char_info[0U].preset_records[1U].name_len - 1U)] = '\0';
    appl_has_char_info[0U].num_of_preset_records++;

    appl_has_char_info[0U].preset_records[2U].index = 3U;
    appl_has_char_info[0U].preset_records[2U].properties = HAP_PROP_NAME_OF_PRESET_WRITABLE_YES | HAP_PROP_IS_AVAILABLE_NO;
    appl_has_char_info[0U].preset_records[2U].name_len = 5U;
    /* Length is 4B + 1B for '\0' */
    GA_mem_copy
    (
        &appl_has_char_info[0U].preset_records[2U].name,
        "PR-3",
        ((appl_has_char_info[0U].preset_records[2U].name_len - 1U) * sizeof(UCHAR))
    );
    appl_has_char_info[0U].preset_records[2U].name[(appl_has_char_info[0U].preset_records[2U].name_len - 1U)] = '\0';
    appl_has_char_info[0U].num_of_preset_records++;

    appl_has_char_info[0U].preset_records[3U].index = 4U;
    appl_has_char_info[0U].preset_records[3U].properties = HAP_PROP_NAME_OF_PRESET_WRITABLE_NO | HAP_PROP_IS_AVAILABLE_YES;
    appl_has_char_info[0U].preset_records[3U].name_len = 5U;
    /* Length is 4B + 1B for '\0' */
    GA_mem_copy
    (
        &appl_has_char_info[0U].preset_records[3U].name,
        "PR-4",
        ((appl_has_char_info[0U].preset_records[3U].name_len - 1U) * sizeof(UCHAR))
    );
    appl_has_char_info[0U].preset_records[3U].name[(appl_has_char_info[0U].preset_records[3U].name_len - 1U)] = '\0';
    appl_has_char_info[0U].num_of_preset_records++;

    return;
}

void appl_hap_ha_create_preset_records(void)
{
    int    choice;
    UINT8  i;
    UINT8  j;
    UINT8  num_of_rec;
    UINT8  size;
    UINT8  properties;
    UCHAR  name[APPL_HAP_MAX_PRESET_RECORD_NAME];

    /* Initialize */
    choice = 0;
    i = 0U;
    j = 0U;
    size = 0U;

    CONSOLE_OUT
    (
        "Input number of Preset Records >=1 and <= %d\n",
        APPL_HAP_MAX_PRESET_RECORD_NAME
    );
    CONSOLE_IN("%d", &choice);
    num_of_rec = (UINT8)choice;

    if (APPL_HAP_MAX_SUPP_PRESET_RECORDS < num_of_rec)
    {
        num_of_rec = APPL_HAP_MAX_SUPP_PRESET_RECORDS;
    }

    for (i = 0U; i < num_of_rec; i++)
    {
        properties = 0U;
        CONSOLE_OUT("Input Preset Index\n");
        CONSOLE_OUT
        (
            "NOTE: Index should be > %d and < %d\n",
            HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN,
            HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX
        );
        CONSOLE_OUT("NOTE: Index value should always be in ascending order\n");
        CONSOLE_IN("%d", &choice);

        appl_has_char_info[0U].preset_records[i].index = (UINT8)choice;

        CONSOLE_OUT("Enter size of Preset Record Name: ");
        CONSOLE_IN("%d", &choice);
        size = (UINT8)choice;

        if (APPL_HAP_MAX_PRESET_RECORD_NAME < size)
        {
            APPL_INF("Size of the Name should not exceed 40 Chars\n");
            size = (APPL_HAP_MAX_PRESET_RECORD_NAME - 1U);
        }
        CONSOLE_OUT("Enter Preset Record Name: ");
        for (j = 0U; j < size; j++)
        {
            CONSOLE_IN(" %c", &name[j]);
        }
        name[j] = '\0';
        size++;

        appl_hap_fetch_preset_record_properties(&properties);

        appl_has_char_info[0U].preset_records[i].properties = properties;
        appl_has_char_info[0U].preset_records[i].name_len = size;
        GA_mem_copy
        (
            &appl_has_char_info[0U].preset_records[i].name,
            &name[0U],
            (appl_has_char_info[0U].preset_records[i].name_len * sizeof(UCHAR))
        );
        appl_has_char_info[0U].num_of_preset_records++;
    }

    return;
}

GA_RESULT appl_hap_se_cb
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ void        * ctx,
              /* IN */ HAP_EVT_ID    evt_id,
              /* IN */ UINT16        evt_status,
              /* IN */ HAP_EVT     * evt_data
          )
{
    UCHAR        rsp_event;
    UINT16       rsp_status;
    HAP_CHR_VAL  rsp_data;
    GA_RESULT    retval;

    /* Initialize */
    retval = GA_FAILURE;
    rsp_status = GA_SUCCESS;
    rsp_event = GA_RSP_TYPE_INVALID;

    if ((NULL == evt_data) ||
        (HAP_MAX_HAS_CONTEXTS <= evt_data->hap_handle))
    {
        rsp_status = GA_FAILURE;
        return rsp_status;
    }

    APPL_TRC("[Event Status]: ");
    appl_hap_parse_ga_result(evt_status);

    switch(evt_id)
    {
        case HAP_HA_GET_FEATURES_IND:
            APPL_TRC("[Event]       : HAP_HA_GET_FEATURES_IND (0x%02X)\n", evt_id);
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Inst]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Hearing Aid Features (0x%04X)\n",
                        GA_CHAR_HA_FEATURES
                    );
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("FORMING RESPONSE DATA\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(HAP_FEATURES));
                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Hearing Aid Features: 0x%02X\n",
                        (UCHAR)sizeof(HAP_FEATURES),
                        appl_has_char_info[evt_data->hap_handle].features
                    );
                    appl_hap_parse_features(appl_has_char_info[evt_data->hap_handle].features);

                    /* Fill the response Info. */
                    rsp_event = evt_id;
                    rsp_data.val = &appl_has_char_info[evt_data->hap_handle].features;
                    rsp_data.len = sizeof(HAP_FEATURES);
                }
            }
            break;

        case HAP_HA_GET_ACTIVE_PRESET_INDEX_IND:
            APPL_TRC("[Event]       : HAP_HA_GET_ACTIVE_PRESET_INDEX_IND (0x%02X)\n", evt_id);
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Inst]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Active Preset Index (0x%04X)\n",
                        GA_CHAR_ACTIVE_PRESET_INDEX
                    );
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("FORMING RESPONSE DATA\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(UINT8));
                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Active Preset Index: 0x%02X\n",
                        (UCHAR)sizeof(UINT8),
                        appl_has_char_info[evt_data->hap_handle].active_preset_index
                    );

                    /* Fill the response Info. */
                    rsp_event = evt_id;
                    rsp_data.val = &appl_has_char_info[evt_data->hap_handle].active_preset_index;
                    rsp_data.len = sizeof(UINT8);
                }
            }
            break;

        case HAP_HA_PRESET_CP_IND:
            APPL_TRC("[Event]       : HAP_HA_PRESET_CP_IND (0x%02X)\n", evt_id);
            if (GA_SUCCESS == evt_status)
            {
                if (NULL != evt_data)
                {
                    APPL_TRC
                    (
                        "[Service Inst]: 0x%02X\n",
                        evt_data->hap_handle
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC
                    (
                        "Char UUID: Hearing Aid Preset Control Point (0x%04X)\n",
                        GA_CHAR_HA_PRESET_CP
                    );
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("RECEIVED DATA\n");
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
                        /* Decode the CP Write and Fetch the response Status */
                        rsp_status = appl_hap_ha_preset_cp_handler
                                     (
                                         device,
                                         evt_id,
                                         evt_data
                                     );
                    }
                    else
                    {
                        APPL_INF("Data: NULL\n");
                    }
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("FORMING RESPONSE DATA\n");
                    GA_FORMAT_DISPLAY();
                    APPL_TRC("Data Length: 0x00\n");
                    APPL_TRC("Data: NULL\n");

                    /* Fill the response Info. */
                    rsp_event = evt_id;
                    rsp_data.val = NULL;
                    rsp_data.len = 0U;
                }
            }
            break;

        default:
            APPL_ERR("Unknown Event: 0x%02X !\n", evt_id);
            rsp_status = GA_FAILURE;
    }

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("SENDING GA HAP RESPONSE\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("GA_hap_send_rsp\n");
    retval = GA_hap_send_rsp
             (
                 device,
                 ctx,
                 rsp_event,
                 rsp_status,
                 &rsp_data
             );
    APPL_TRC("Retval: ");
    appl_hap_parse_ga_result(retval);

    return retval;
}

GA_RESULT appl_hap_ha_preset_cp_handler
          (
              GA_ENDPOINT * device,
              HAP_EVT_ID    evt_id,
              HAP_EVT     * evt_data
          )
{
    GA_RESULT   retval;
    UCHAR       cp_op_code;
    UCHAR     * data;
    UCHAR       datalen;
    INT8        i;
    INT8        j;

    UCHAR   has_send_ntf;
    UCHAR   rsp_event;
    UINT16  rsp_status;
    UINT8   marker;
    UINT8   index;

    APPL_CNTRL_POINT_PARAM  hap_ntf_param;

    /* Initialize */
    index = 0U;
    i = 0;
    j = 0;
    has_send_ntf = GA_TRUE;
    retval = GA_SUCCESS;
    marker = 0U;

    GA_IGNORE_UNUSED_PARAM(evt_id);

    if((NULL == evt_data) ||
       (NULL == evt_data->data) ||
       (0U == evt_data->len))
    {
        APPL_ERR("Invalid Parameters !\n");
        return GA_FAILURE;
    }

    if (HAP_MAX_HAS_CONTEXTS <= evt_data->hap_handle)
    {
        APPL_ERR("Invalid Handle !\n");
        return GA_FAILURE;
    }

    data = evt_data->data;
    datalen = (UINT8)evt_data->len;

    /* Init */
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;

    if (1U > datalen)
    {
        APPL_ERR("\n==== Invalid CP Write Length ======== !\n");
        return HAP_APPL_ERR_CODE_INVALID_PARAM_LENGTH;
    }

    /* Fetch the CP Opcode */
    cp_op_code = data[marker];
    /* Incrementing the Opcode Byte */
    marker++;

    if ((HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY < cp_op_code) ||
        (HAP_CP_OPCODE_READ_PRESETS_RSP == cp_op_code) ||
        (HAP_CP_OPCODE_PRESET_CHANGED == cp_op_code))
    {
        /* CP Opcodes that are to be sent by Server if initiated by Client */
        APPL_ERR("\n==== Invalid CP OPCODE ======== !\n");
        return HAP_APPL_ERR_CODE_INVALID_OPCODE;
    }

    if ((HAP_CP_OPCODE_READ_PRESETS_REQ == cp_op_code) ||
        (HAP_CP_OPCODE_WRITE_PRESET_NAME == cp_op_code) ||
        (HAP_CP_OPCODE_SET_ACTIVE_PRESET == cp_op_code) ||
        (HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY == cp_op_code))
    {
        /* Fetch the Index */
        index = data[marker];
        /* Incrementing the Index */
        marker++;
        if ((HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN > index) ||
            (HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX < index))
        {
            APPL_ERR("Index Out of Range !\n");
            return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
        }

        /*
         * When an operation includes the Index parameter and the Index parameter
         * does not correspond to a preset record exposed by the server, then the
         * server shall return an ATT_ERROR_RSP PDU to the ATT_WRITE_REQ PDU with the
         * error code set to Out of Range, as defined in Bluetooth Core Specification
         * Supplement, Version 10.
         * As per HAS Spec V1.0, Page: 16
         */
        for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if (appl_has_char_info[0U].preset_records[i].index == index)
            {
                break;
            }
        }
        if (appl_has_char_info[0U].num_of_preset_records == i)
        {
            APPL_ERR("Index Out of Range !\n");
            return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
        }
    }

    /* Set to Send Ntf */
    GA_COPY_BRR_DEVICE(&hap_ntf_param.dev, device);
    hap_ntf_param.hap_cp_opcode = cp_op_code;

    APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(cp_op_code));
    if (HAP_CP_OPCODE_READ_PRESETS_REQ == cp_op_code)
    {
        APPL_TRC("Read Presets Request (0x%02X)\n", cp_op_code);

        /* Copying into global array */
        appl_hap_ha_data[0U] = index;
        APPL_TRC
        (
            "Len: 0x%02X,  StartIndex: 0x%02X\n",
            (UCHAR)sizeof(UINT8), appl_hap_ha_data[0U]
        );

        appl_hap_ha_data[1U] = data[marker];
        APPL_TRC
        (
            "Len: 0x%02X,  NumPresets: 0x%02X\n",
            (UCHAR)sizeof(UINT8), appl_hap_ha_data[1U]
        );

        /*
         * If the client writes a Read Presets Request in which:
         * - the StartIndex value is beyond the range of preset records on
         *   the Server, or
         * - the StartIndex is set to 0x00, or
         * - the NumPresets is set to 0x00, or
         * - there are no presets,
         * the server shall reject the request by returning an
         * ATT_ERROR_RSP PDU to the ATT_WRITE_REQ PDU with the error code
         * parameter set to Out of Range, as defined in
         * Bluetooth Core Specification Supplement, Version 10
         * As per HAS Spec V1.0, Page: 17
         */
        /*
         * All the cases are already checked above, except
         * "the NumPresets is set to 0x00" case
         */
        if (0U == appl_hap_ha_data[1U])
        {
            APPL_ERR("Index Out of Range !\n");
            return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
        }

        hap_ntf_param.data = &appl_hap_ha_data[0U];
        /*
         * datalen = 3U -> Opcode(1B) + StartIndex(1B) + NumPresets(1B)
         * hap_ntf_param.datalen will have: 2B(StartIndex(1B) + NumPresets(1B))
         */
        hap_ntf_param.datalen = datalen - 1U;
    }
    else if (HAP_CP_OPCODE_WRITE_PRESET_NAME == cp_op_code)
    {
        APPL_TRC("Write Preset Name (0x%02X)\n", cp_op_code);
        APPL_TRC
        (
            "Len: 0x%02X,  Index: 0x%02X\n",
            (UCHAR)sizeof(UINT8), index
        );

        /*
         * If the Name parameter is not present, or the Name parameter is
         * longer than 40 octets, then the server shall respond with an
         * ATT_ERROR_RSP PDU with the Error Code parameter set to
         * Invalid Parameters Length.
         */
        if ((0U == (datalen - 2U)) || (40U < (datalen - 2U)))
        {
            return HAP_APPL_ERR_CODE_INVALID_PARAM_LENGTH;
        }

        for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if (appl_has_char_info[0U].preset_records[i].index == index)
            {
                if (appl_has_char_info[0U].preset_records[i].properties &
                    HAP_PROP_NAME_OF_PRESET_WRITABLE_YES)
                {
                    /*
                     * datalen = Opcode(1B) + Preset Index(1B) + Name Length
                     * hap_ntf_param.datalen will have: Name Length
                     * If Name Length is 5U, the:
                     * datalen = 7U
                     * hap_ntf_param.datalen = 7U - 2U = 5U
                     */
                    GA_mem_copy
                    (
                        &appl_has_char_info[0U].preset_records[i].name,
                        &data[marker],
                        ((datalen - 2U) * sizeof(UCHAR))
                    );
                    appl_has_char_info[0U].preset_records[i].name_len = datalen - 2U;
                    /* Copying into global array */
                    appl_hap_ha_data[0U] = index;
                    hap_ntf_param.data = &appl_hap_ha_data[0U];
                    hap_ntf_param.datalen = (UCHAR)sizeof(UINT8);
                }
                else
                {
                    APPL_ERR("Write Name Not Allowed !\n");
                    return HAP_APPL_ERR_CODE_WRITE_NAME_NT_ALLOWED;
                }
                break;
            }
        }
    }
    else if ((HAP_CP_OPCODE_SET_ACTIVE_PRESET == cp_op_code) ||
             (HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY == cp_op_code))
    {
        if (HAP_CP_OPCODE_SET_ACTIVE_PRESET == cp_op_code)
        {
            APPL_TRC("Set Active Preset (0x%02X)\n", cp_op_code);
        }
        else
        {
            APPL_TRC("Set Active Preset - Synchronized Locally (0x%02X)\n", cp_op_code);
        }
        for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if(data[1U] == appl_has_char_info[0U].preset_records[i].index)
            {
                if ((HAP_PROP_IS_AVAILABLE_YES & appl_has_char_info[0U].preset_records[i].properties))
                {
                    break;
                }
                else
                {
                    return HAP_APPL_ERR_CODE_PRESET_OP_NOT_POSSIBLE;
                }
            }
        }
        if (appl_has_char_info[0U].num_of_preset_records == i)
        {
            return HAP_APPL_ERR_CODE_PRESET_OP_NOT_POSSIBLE;
        }
        APPL_TRC
        (
            "Len: 0x%02X,  Index: 0x%02X\n",
            (datalen - 1U), data[1U]
        );
        if (appl_has_char_info[0U].active_preset_index == (UINT8)data[1U])
        {
            has_send_ntf = GA_FALSE;
        }
        appl_has_char_info[0U].active_preset_index = (UINT8)data[1U];
        if (HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY == cp_op_code)
        {
            if (HAP_HEARING_AID_PRESET_SYNC_SUPP_NO & appl_has_char_info[0U].features)
            {
                return HAP_APPL_ERR_CODE_PRESET_SYNC_NT_SUPP;
            }
            if (GA_TRUE == has_send_ntf)
            {
                APPL_TRC
                (
                    "Server relays the change to the other server that is part of the Binaural Hearing Aid Set\n"
                );
            }
        }
        APPL_TRC("\n");
        /* Copying into global array */
        appl_hap_ha_data[0U] = appl_has_char_info[0U].active_preset_index;
        hap_ntf_param.data = &appl_hap_ha_data[0U];
        /*
         * datalen = 2U -> Opcode(1B) + Preset Index(1B)
         * hap_ntf_param.datalen will have: 1B (Preset Index(1B))
         */
        hap_ntf_param.datalen = datalen - 1U;
    }
    else if ((HAP_CP_OPCODE_SET_NEXT_PRESET == cp_op_code) ||
             (HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY == cp_op_code))
    {
        if (HAP_CP_OPCODE_SET_NEXT_PRESET == cp_op_code)
        {
            APPL_TRC("Set Next Preset (0x%02X)\n", cp_op_code);
        }
        else
        {
            APPL_TRC("Set Next Preset - Synchronized Locally (0x%02X)\n", cp_op_code);
        }
        /* Setting to default value */
        has_send_ntf = GA_FALSE;
        for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if(appl_has_char_info[0U].active_preset_index ==
               appl_has_char_info[0U].preset_records[i].index)
            {
                for (j = (i+1); j < appl_has_char_info[0U].num_of_preset_records; j++)
                {
                    if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[j].index)
                        &&
                        (HAP_PROP_IS_AVAILABLE_YES & appl_has_char_info[0U].preset_records[j].properties))
                    {
                        has_send_ntf = GA_TRUE;
                        appl_has_char_info[0U].active_preset_index = appl_has_char_info[0U].preset_records[j].index;
                        break;
                    }
                    else
                    {
                        has_send_ntf = GA_FALSE;
                    }
                }
                if (GA_TRUE != has_send_ntf)
                {
                    for (j = 0U; j < i; j++)
                    {
                        if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[j].index)
                            &&
                            (HAP_PROP_IS_AVAILABLE_YES & appl_has_char_info[0U].preset_records[j].properties))
                        {
                            has_send_ntf = GA_TRUE;
                            appl_has_char_info[0U].active_preset_index = appl_has_char_info[0U].preset_records[j].index;
                            break;
                        }
                    }
                }
                break;
            }
        }
        if (HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY == cp_op_code)
        {
            if (HAP_HEARING_AID_PRESET_SYNC_SUPP_NO & appl_has_char_info[0U].features)
            {
                return HAP_APPL_ERR_CODE_PRESET_SYNC_NT_SUPP;
            }
            if (GA_TRUE == has_send_ntf)
            {
                APPL_TRC
                (
                    "Server relays the change to the other server that is part of the Binaural Hearing Aid Set\n"
                );
            }
        }
        APPL_TRC("\n");
        /* Copying into global array */
        appl_hap_ha_data[0U] = appl_has_char_info[0U].active_preset_index;
        hap_ntf_param.data = &appl_hap_ha_data[0U];
        hap_ntf_param.datalen = (UCHAR)sizeof(UINT8);
    }
    else if ((HAP_CP_OPCODE_SET_PREV_PRESET == cp_op_code) ||
             (HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY == cp_op_code))
    {
        if (HAP_CP_OPCODE_SET_PREV_PRESET == cp_op_code)
        {
            APPL_TRC("Set Previous Preset (0x%02X)\n", cp_op_code);
        }
        else
        {
            APPL_TRC("Set Previous Preset - Synchronized Locally (0x%02X)\n", cp_op_code);
        }
        /* Setting to default value */
        has_send_ntf = GA_FALSE;
        for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if(appl_has_char_info[0U].active_preset_index ==
               appl_has_char_info[0U].preset_records[i].index)
            {
                for (j = (i-1); j >= 0; j--)
                {
                    if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[j].index)
                        &&
                        (HAP_PROP_IS_AVAILABLE_YES & appl_has_char_info[0U].preset_records[j].properties))
                    {
                        has_send_ntf = GA_TRUE;
                        appl_has_char_info[0U].active_preset_index = appl_has_char_info[0U].preset_records[j].index;
                        break;
                    }
                    else
                    {
                        has_send_ntf = GA_FALSE;
                    }
                }
                if (GA_TRUE != has_send_ntf)
                {
                    for (j = (appl_has_char_info[0U].num_of_preset_records - 1U); j >= (i+1); j--)
                    {
                        if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[j].index)
                            &&
                            (HAP_PROP_IS_AVAILABLE_YES & appl_has_char_info[0U].preset_records[j].properties))
                        {
                            has_send_ntf = GA_TRUE;
                            appl_has_char_info[0U].active_preset_index = appl_has_char_info[0U].preset_records[j].index;
                            break;
                        }
                        else
                        {
                            has_send_ntf = GA_FALSE;
                        }
                    }
                }
                break;
            }
        }
        if (HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY == cp_op_code)
        {
            if (HAP_HEARING_AID_PRESET_SYNC_SUPP_NO & appl_has_char_info[0U].features)
            {
                return HAP_APPL_ERR_CODE_PRESET_SYNC_NT_SUPP;
            }
            if (GA_TRUE == has_send_ntf)
            {
                APPL_TRC
                (
                    "Server relays the change to the other server that is part of the Binaural Hearing Aid Set\n"
                );
            }
        }
        APPL_TRC("\n");
        /* Copying into global array */
        appl_hap_ha_data[0U] = appl_has_char_info[0U].active_preset_index;
        hap_ntf_param.data = &appl_hap_ha_data[0U];
        hap_ntf_param.datalen = (UCHAR)sizeof(UINT8);
    }
    else
    {
        retval = GA_FAILURE;

        /* TODO */
        APPL_ERR("??? !\n");
    }

    if (GA_TRUE == has_send_ntf)
    {
        BT_start_timer
        (
            &appl_hap_ha_send_ntf_timer_handle,
            1U,
            appl_hap_ha_send_ntf_timer_expiry_handler,
            &hap_ntf_param,
            sizeof(hap_ntf_param)
        );
    }

    return retval;
}

void appl_hap_ha_send_ntf_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     )
{
    GA_RESULT                 retval;
    APPL_CNTRL_POINT_PARAM  * cntrl_params;
    HAP_CHR_VAL               value;
    UCHAR                     hap_cp_ntf_data[64U];
    UCHAR                     i;
    UCHAR                     len;
    UINT8                     marker;
    UINT8                     index;
    UINT8                     num_presets;

    /* Initialize */
    marker = 0U;
    i = 0U;
    len = 0U;
    retval = GA_FAILURE;
    index = 0U;
    num_presets = 0U;

    GA_IGNORE_UNUSED_PARAM(datalen);

    GA_mem_set(hap_cp_ntf_data, 0x00, (64U * sizeof(UCHAR)));
    cntrl_params = (APPL_CNTRL_POINT_PARAM *)cntrl_point_rsp;

    if (BT_TIMER_HANDLE_INIT_VAL != appl_hap_ha_send_ntf_timer_handle)
    {
        appl_hap_ha_send_ntf_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("SENDING GA HAP NOTIFICATION\n");
    GA_FORMAT_DISPLAY();

    switch(cntrl_params->hap_cp_opcode)
    {
        case HAP_CP_OPCODE_READ_PRESETS_REQ:
            /* Fetch Start Preset Index from data */
            index = cntrl_params->data[0U];
            /* Fetch Number of Presets from data */
            num_presets = cntrl_params->data[1U];

            for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
            {
                if (0U == num_presets)
                {
                    /* Already notified allowed num of preset records */
                    break;
                }

                /* Begin from the StartIndex requested */
                if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[i].index) &&
                    (index <= appl_has_char_info[0U].preset_records[i].index))
                {
                    marker = 0U;
                    len = 0U;
                    GA_mem_set(&hap_cp_ntf_data[marker], 0x00, (64U * sizeof(UCHAR)));

                    hap_cp_ntf_data[marker] = HAP_CP_OPCODE_READ_PRESETS_RSP;
                    /* Incrementing the Opcode Byte */
                    marker++;
                    if (appl_has_char_info[0U].num_of_preset_records == (i + 1U))
                    {
                        /* This is the Last Preset Record in the list */
                        hap_cp_ntf_data[marker] = HAP_CP_IS_LAST_YES;
                        /* Incrementing the isLast Byte */
                        marker++;
                    }
                    else
                    {
                        /* This is still not the Last Record in the list */
                        hap_cp_ntf_data[marker] = HAP_CP_IS_LAST_NO;
                        /* Incrementing the isLast Byte */
                        marker++;
                    }

                    appl_has_create_preset_records_array
                    (
                        i, &hap_cp_ntf_data[marker], &len
                    );

                    value.val = &hap_cp_ntf_data[0U];
                    /* (Opcode + isLast) + (Preset Record - Index + Properties + Name Length) */
                    value.len = marker + len;

                    /* Send Notification for each Preset Record */
                    APPL_TRC("GA_hap_notify_ha_preset_cp\n");
                    retval = GA_hap_notify_ha_preset_cp
                             (
                                 &cntrl_params->dev,
                                 &value
                             );
                    APPL_TRC("Retval: ");
                    appl_hap_parse_ga_result(retval);

                    /* Decrementing the total num of notified preset record */
                    num_presets--;
                }
            }
            break;

        case HAP_CP_OPCODE_WRITE_PRESET_NAME:
            hap_cp_ntf_data[marker++] = HAP_CP_OPCODE_PRESET_CHANGED;
            /* ChangeId - Generic Update */
            hap_cp_ntf_data[marker++] = 0x00U;
            /* This is the only Record to be sent */
            hap_cp_ntf_data[marker++] = HAP_CP_IS_LAST_YES;

            /* Fetch Preset Index from data */
            index = cntrl_params->data[0U];
            for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
            {
                if (appl_has_char_info[0U].preset_records[i].index == index)
                {
                    break;
                }
            }

            /*
             * Availability of Preset Record is already checked before
             * performing CP operation
             */
            if (APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[i].index)
            {
                if (i == 0U)
                {
                    /* If referring to the first Preset Record, then PrevIndex = 0x00
                     * appl_has_char_info[0U].preset_records[i]
                     * Referring to the first Preset Record
                     */
                    hap_cp_ntf_data[marker++] = 0x00U;
                }
                else
                {
                    hap_cp_ntf_data[marker++] = appl_has_char_info[0U].preset_records[i - 1U].index;
                }

                appl_has_create_preset_records_array
                (
                    i, &hap_cp_ntf_data[marker], &len
                );

                value.val = &hap_cp_ntf_data[0U];
                /* (Opcode + ChangeID + isLast + PrevIndex) + (Preset Record - Index + Properties + Name Length) */
                value.len = marker + len;

                APPL_TRC("GA_hap_notify_ha_preset_cp\n");
                retval = GA_hap_notify_ha_preset_cp
                         (
                             &cntrl_params->dev,
                             &value
                         );
                APPL_TRC("Retval: ");
                appl_hap_parse_ga_result(retval);
            }
            break;

        case HAP_CP_OPCODE_SET_ACTIVE_PRESET: /* Fall - through */
        case HAP_CP_OPCODE_SET_NEXT_PRESET: /* Fall - through */
        case HAP_CP_OPCODE_SET_PREV_PRESET: /* Fall - through */
        case HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY: /* Fall - through */
        case HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY: /* Fall - through */
        case HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY:
            for (i = 0U; i < appl_has_char_info[0U].num_of_preset_records; i++)
            {
                index = *(cntrl_params->data);
                if(index == appl_has_char_info[0U].preset_records[i].index)
                {
                    break;
                }
            }

            /* Update the Active Preset Index */
            hap_cp_ntf_data[marker++] = appl_has_char_info[0U].active_preset_index;

            value.val = &hap_cp_ntf_data[0U];
            value.len = sizeof(UINT8);

            APPL_TRC("GA_hap_notify_active_preset_index\n");
            retval = GA_hap_notify_active_preset_index
                     (
                         &cntrl_params->dev,
                         &value
                     );
            APPL_TRC("Retval: ");
            appl_hap_parse_ga_result(retval);
            break;

        default:
            APPL_ERR("UnHandled CP Opcode: 0x%02X !\n", cntrl_params->hap_cp_opcode);
            break;
    }

    return;
}

GA_RESULT appl_hap_send_preset_changed_ntf(void)
{
    GA_RESULT    retval;
    int          choice;
    int          menu_choice;
    int          j;
    UINT8        marker;
    UINT8        size;
    UINT8        sub_choice;
    UINT8        change_ID;
    UCHAR        name[APPL_HAP_MAX_PRESET_RECORD_NAME];
    UINT8        i;
    UCHAR        len;
    UINT8        index;
    UINT8        properties;
    HAP_CHR_VAL  value;
    UCHAR        hap_cp_ntf_data[64U] = { 0U };

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0U;
    marker = 0U;
    size = 0U;
    i = 0;
    j = 0;
    index = 0U;
    properties = 0U;
    len = 0U;

    /* Server sending Auto Notification */
    hap_cp_ntf_data[marker] = HAP_CP_OPCODE_PRESET_CHANGED;
    /* Incrementing the Opcode Byte which is already updated before the call */
    marker++;

    CONSOLE_OUT("Input ChangeID [In HEX]:\n");
    CONSOLE_OUT(" - Generic Update [0x00]\n");
    CONSOLE_OUT(" - Preset Record Deleted [0x01]\n");
    CONSOLE_OUT(" - Preset Record Available [0x02]\n");
    CONSOLE_OUT(" - Preset Record Unavailable [0x03]\n");
    CONSOLE_IN("%x", &choice);
    menu_choice = choice;

    change_ID = (UINT8)choice;
    hap_cp_ntf_data[marker] = change_ID;
    /* Incrementing the ChangeID Byte */
    marker++;

    if (HAP_CP_CHANGE_ID_GENERIC_UPDATE == change_ID)
    {
        hap_cp_ntf_data[marker] = HAP_CP_IS_LAST_YES;
        /* Incrementing the isLast Byte */
        marker++;

        CONSOLE_OUT("Input Generic Update Option:\n");
        CONSOLE_OUT("  1. Add new Preset Record\n");
        CONSOLE_OUT("  2. Change Preset Record Name\n");
        CONSOLE_OUT("  3. Both 1 and 2\n");
        CONSOLE_IN("%d", &choice);
        sub_choice = (UINT8)choice;
        if ((1U > sub_choice) || (3U < sub_choice))
        {
            APPL_INF("Invalid choice\n");
            return retval;
        }
        CONSOLE_OUT("Enter size of Preset Record Name: ");
        CONSOLE_IN("%d", &choice);
        size = (UINT8)choice;

        if (APPL_HAP_MAX_PRESET_RECORD_NAME < size)
        {
            APPL_INF("Size of the Name should not exceed 40 Chars\n");
            size = (APPL_HAP_MAX_PRESET_RECORD_NAME - 1U);
        }
        CONSOLE_OUT("Enter Preset Record Name: ");
        for (i = 0; i < size; i++)
        {
            CONSOLE_IN(" %c", &name[i]);
        }
        name[i] = '\0';
        size++;

        if (1U == sub_choice)
        {
            /* A preset record has been added to the list of preset records. */
            if(APPL_HAP_MAX_SUPP_PRESET_RECORDS == appl_has_char_info[0U].num_of_preset_records)
            {
                APPL_INF
                (
                    "Reached maximum supported records, %d\n",
                    APPL_HAP_MAX_SUPP_PRESET_RECORDS
                );
            }
            else
            {
                for (i = 0; i < APPL_HAP_MAX_SUPP_PRESET_RECORDS; i++)
                {
                    if (APPL_HAP_PRESET_RECORD_INDEX_INVALID == appl_has_char_info[0U].preset_records[i].index)
                    {
                        break;
                    }
                }
                appl_has_char_info[0U].preset_records[i].index = i + 1;

                appl_hap_fetch_preset_record_properties(&properties);

                appl_has_char_info[0U].preset_records[i].properties
                    = properties;

                GA_mem_set
                (
                    &appl_has_char_info[0U].preset_records[i].name,
                    0x00U,
                    (APPL_HAP_MAX_PRESET_RECORD_NAME * sizeof(UCHAR))
                );
                /* Set the name of the preset record */
                GA_mem_copy
                (
                    &appl_has_char_info[0U].preset_records[i].name,
                    &name,
                    (size * sizeof(UCHAR))
                );
                /* Increment the num of preset records */
                appl_has_char_info[0U].num_of_preset_records++;
                appl_has_char_info[0U].preset_records[i].name_len = size;
                if (0 == i)
                {
                    hap_cp_ntf_data[marker] = 0x00U;
                }
                else
                {
                    /*
                     * The field PrevIndex shall contain the Index field of
                     * the previous preset record in the list of preset records
                     */
                    hap_cp_ntf_data[marker] = appl_has_char_info[0U].preset_records[i - 1].index;
                }
                /* Incrementing the PrevIndex Byte */
                marker++;

                /* Form the notification data */
                appl_has_create_preset_records_array
                (
                    i, &hap_cp_ntf_data[marker], &len
                );

                value.val = &hap_cp_ntf_data[0U];
                /* (Opcode + ChangeId + isLast + PrevIndex) + (Preset Record - Index + Properties + Name Length) */
                value.len = marker + len;
                APPL_TRC("GA_hap_notify_ha_preset_cp...\n");
                retval = GA_hap_notify_ha_preset_cp
                         (
                             &ga_has_peer_bd_addr,
                             &value
                         );
                APPL_TRC("Retval: ");
                appl_hap_parse_ga_result(retval);
            }
        }
        else if (2U == sub_choice)
        {
            /* The name of a preset record has changed. */
            CONSOLE_OUT("Enter Preset Record Index [In HEX] [0x01 - 0xFF]:\n");
            CONSOLE_IN("%x", &choice);
            index = (UINT8)choice;

            if ((HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN > index) ||
                (HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX < index))
            {
                APPL_ERR("Index Out of Range !\n");
                return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
            }
            /* Search for the preset record */
            for (i = 0; i < appl_has_char_info[0U].num_of_preset_records; i++)
            {
                /*
                 * Validate the preset record index(should not be invalid) and
                 * check if the preset record is found
                 */
                if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[i].index) &&
                    (appl_has_char_info[0U].preset_records[i].index == index))
                {
                    /* Check the preset record properties if it is writable or not */
                    if (appl_has_char_info[0U].preset_records[i].properties &
                        HAP_PROP_NAME_OF_PRESET_WRITABLE_YES)
                    {
                        GA_mem_set
                        (
                            &appl_has_char_info[0U].preset_records[i].name,
                            0x00U,
                            (APPL_HAP_MAX_PRESET_RECORD_NAME * sizeof(UCHAR))
                        );
                        /* Set the name of the preset record */
                        GA_mem_copy
                        (
                            &appl_has_char_info[0U].preset_records[i].name,
                            &name,
                            (size * sizeof(UCHAR))
                        );
                        appl_has_char_info[0U].preset_records[i].name_len = size;
                    }
                    else
                    {
                        APPL_ERR("Write Name Not Allowed !\n");
                        return HAP_APPL_ERR_CODE_WRITE_NAME_NT_ALLOWED;
                    }
                    /* Update Previous Index */
                    if ((i == 0) || (1U == appl_has_char_info[0U].num_of_preset_records))
                    {
                        /*
                         * If the preset record is at the top of the list
                         * (i.e., it is the preset record with the lowest value
                         * of Index field), then the PrevIndex field shall be
                         * set to 0x00.
                         */
                        hap_cp_ntf_data[marker] = 0x00;
                    }
                    else
                    {
                        /*
                         * The field PrevIndex shall contain the Index field of
                         * the previous preset record in the list of preset
                         * records
                         */
                        for (j = (i - 1); j >= 0; j--)
                        {
                            if (APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[j].index)
                            {
                                hap_cp_ntf_data[marker] = appl_has_char_info[0U].preset_records[j].index;
                                break;
                            }
                        }
                        if (j < 0)
                        {
                            hap_cp_ntf_data[marker] = 0x00U;
                        }
                    }
                    /* Incrementing the PrevIndex Byte */
                    marker++;

                    /* Form the notification data */
                    appl_has_create_preset_records_array
                    (
                        i, &hap_cp_ntf_data[marker], &len
                    );

                    value.val = &hap_cp_ntf_data[0U];
                    /* (Opcode + ChangeId + isLast + PrevIndex) + (Preset Record - Index + Properties + Name Length) */
                    value.len = marker + len;

                    APPL_TRC("GA_hap_notify_ha_preset_cp...\n");
                    retval = GA_hap_notify_ha_preset_cp
                             (
                                 &ga_has_peer_bd_addr,
                                 &value
                             );
                    APPL_TRC("Retval: ");
                    appl_hap_parse_ga_result(retval);
                    break;
                }
            }
        }
        else if (3U == sub_choice)
        {
            /* TODO */
        }
    }
    else if (HAP_CP_CHANGE_ID_PRESET_RECORD_DELETED == change_ID)
    {
        hap_cp_ntf_data[marker] = HAP_CP_IS_LAST_YES;
        /* Incrementing the isLast Byte */
        marker++;

        CONSOLE_OUT("Enter Preset Record Index [In HEX] [0x01 - 0xFF]:\n");
        CONSOLE_IN("%x", &choice);
        index = (UINT8)choice;

        /* Validate the index value input */
        if ((HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN > index) ||
            (HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX < index))
        {
            APPL_ERR("Index Out of Range !\n");
            return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
        }

        /*
         * Validate the preset record index(should not be invalid) and
         * check if the preset record is found
         */
        for (i = 0; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[i].index) &&
                (appl_has_char_info[0U].preset_records[i].index == index))
            {
                break;
            }
        }
        if (appl_has_char_info[0U].num_of_preset_records == i)
        {
            APPL_ERR("Could not find the preset record !\n");
            return retval;
        }
        else
        {
            APPL_TRC("Deleting the record...\n");

            if (appl_has_char_info[0U].active_preset_index == appl_has_char_info[0U].preset_records[i].index)
            {
                appl_has_char_info[0U].active_preset_index = APPL_HAP_PRESET_RECORD_INDEX_INVALID;
            }

            GA_mem_set
            (
                &appl_has_char_info[0U].preset_records[i],
                0x00U,
                sizeof(appl_has_char_info[0U].preset_records[i])
            );
            /* Decrement the num of preset records */
            appl_has_char_info[0U].num_of_preset_records--;

            hap_cp_ntf_data[marker] = index;
            /* Incrementing the Index Byte */
            marker++;

            value.val = &hap_cp_ntf_data[0U];
            value.len = marker;
            APPL_TRC("GA_hap_notify_ha_preset_cp...\n");
            retval = GA_hap_notify_ha_preset_cp
                     (
                         &ga_has_peer_bd_addr,
                         &value
                     );
            APPL_TRC("Retval: ");
            appl_hap_parse_ga_result(retval);
            retval = GA_SUCCESS;
        }
    }
    else if ((HAP_CP_CHANGE_ID_PRESET_RECORD_AVAILABLE == change_ID) ||
        (HAP_CP_CHANGE_ID_PRESET_RECORD_UNAVAILABLE == change_ID))
    {
        hap_cp_ntf_data[marker] = HAP_CP_IS_LAST_YES;
        /* Incrementing the isLast Byte */
        marker++;

        /* The preset record has become available/unavailable */
        CONSOLE_OUT("Enter Preset Record Index [In HEX] [0x01 - 0xFF]:\n");
        CONSOLE_IN("%x", &choice);
        index = (UINT8)choice;

        if ((HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN > index) ||
            (HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX < index))
        {
            APPL_ERR("Index Out of Range !\n");
            return APPL_HAP_ATT_ERR_CODE_OUT_OF_RANGE;
        }
        for (i = 0; i < appl_has_char_info[0U].num_of_preset_records; i++)
        {
            if ((APPL_HAP_PRESET_RECORD_INDEX_INVALID != appl_has_char_info[0U].preset_records[i].index) &&
                (appl_has_char_info[0U].preset_records[i].index == index))
            {
                break;
            }
        }
        if (appl_has_char_info[0U].num_of_preset_records == i)
        {
            APPL_ERR("Could not find the preset record !\n");
            return HAP_APPL_ERR_CODE_PRESET_OP_NOT_POSSIBLE;
        }
        else
        {
            hap_cp_ntf_data[marker] = index;
            /* Incrementing Index Byte */
            marker++;
            if (HAP_CP_CHANGE_ID_PRESET_RECORD_AVAILABLE == change_ID)
            {
                /* First Reset the Current Property - Bit 1 */
                appl_has_char_info[0U].preset_records[i].properties &=
                    (~(APPL_HAP_PRESET_RECORD_IS_AVAILABLE_BIT_LOC));
                /* Set the Bit - 1 to Available Yes (1) */
                appl_has_char_info[0U].preset_records[i].properties |=
                    HAP_PROP_IS_AVAILABLE_YES;
            }
            else if (HAP_CP_CHANGE_ID_PRESET_RECORD_UNAVAILABLE == change_ID)
            {
                /* First Reset the Current Property - Bit 1 */
                appl_has_char_info[0U].preset_records[i].properties &=
                    (~(APPL_HAP_PRESET_RECORD_IS_AVAILABLE_BIT_LOC));
                /* Set the Bit - 1 to Available No (0) */
                appl_has_char_info[0U].preset_records[i].properties |=
                    HAP_PROP_IS_AVAILABLE_NO;
            }

            value.val = &hap_cp_ntf_data[0U];
            value.len = marker;
            APPL_TRC("GA_hap_notify_ha_preset_cp...\n");
            retval = GA_hap_notify_ha_preset_cp
                     (
                         &ga_has_peer_bd_addr,
                         &value
                     );
            APPL_TRC("Retval: ");
            appl_hap_parse_ga_result(retval);
        }
    }
    else
    {
        APPL_INF("Invalid ChangeID\n");
    }

    if (0 == menu_choice)
    {
        /* return */
        return retval;
    }

    return retval;
}

void appl_has_create_preset_records_array(UCHAR i, UCHAR * data, UCHAR *len)
{
    /* Add Preset Record: Index */
    data[(*len)++] = appl_has_char_info[0U].preset_records[i].index;
    /* Add Preset Record: Properties */
    data[(*len)++] = appl_has_char_info[0U].preset_records[i].properties;
    /* Add Preset Record: Name */
    BT_PACK_LE_N_BYTE
    (
        &data[(*len)++],
        &appl_has_char_info[0U].preset_records[i].name,
        appl_has_char_info[0U].preset_records[i].name_len
    );

    *len = *len + appl_has_char_info[0U].preset_records[i].name_len;

    return;
}

#endif /* HAP_SERVER */

void appl_hap_parse_and_display_is_last(UCHAR is_last)
{
    switch(is_last)
    {
        case HAP_CP_IS_LAST_YES:
            APPL_TRC("isLast: Yes (0x%02X)\n", is_last);
            break;

        case HAP_CP_IS_LAST_NO:
            APPL_TRC("isLast: No (0x%02X)\n", is_last);
            break;

        default:
            /* Should not reach here */
            APPL_ERR("Invalid Value (0x%02X) !\n", is_last);
    }

    return;
}

void appl_hap_parse_and_display_preset_record_properties
     (
         UCHAR preset_record_properties
     )
{
    APPL_TRC("\tProperties: ");

    if (0U == preset_record_properties)
    {
        APPL_TRC("Not Writable and Not Available (0x%02X)\n", preset_record_properties);
    }
    else if (1U == preset_record_properties)
    {
        APPL_TRC("Writable and Not Available (0x%02X)\n", preset_record_properties);
    }
    else if (2U == preset_record_properties)
    {
        APPL_TRC("Not Writable and Available (0x%02X)\n", preset_record_properties);
    }
    else if (3U == preset_record_properties)
    {
        APPL_TRC("Writable and Available (0x%02X)\n", preset_record_properties);
    }
    else
    {
        /* Should not reach here */
        APPL_ERR("Unknown property (0x%02X) !\n", preset_record_properties);
    }
    return;
}

#endif /* GA_HAP */
