/**
 *  \file appl_ga_bc_ba.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast source exposed by BAP library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga_bc_ba.h"
#include "appl_ga_utils_audio_src.h"
#include "GA_pbp_api.h"

#ifdef BAP_BROADCAST

/* --------------------------------------------- Global Definitions */

static const char bc_ba_src_menu_options[] = " \n\
================ GA BC 'BA' OR 'BA Collocated BC Src' OR 'BC Src' MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
================ Profile Management ~ Init ============ \n\
    2. BA Init. \n\
\n\
================ Profile Management ~ Deinit ============ \n\
    3. BA Shutdown. \n\
\n\
================ BC BA ============ \n\
================ Device Management ~ LE Scan ============ \n\
   10. Scan for Scan Delegator \n\
   11. Stop Scan for Scan Delegator \n\
\n\
================ Device Management ~ LE Connection ============ \n\
   12. Connect to Scan Delegator \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
   13. Set SD index \n\
\n\
================ Context Management ~ Setup ============ \n\
   20. Setup BASS session with Peer Device \n\
\n\
================ Profile Management (BASS) ~ Operations(Read) ============ \n\
   21. Read all BASS Instances \n\
\n\
================ Profile Management (BASS) ~ Operations(Write) ============ \n\
   22. Add the Broadcast Source \n\
   23. Modify the Broadcast Source \n\
   24. Set Broadcast Code \n\
   25. Remove the Source \n\
\n\
================ Context Management ~ Release ============ \n\
   26. Release/Close BASS \n\
\n\
================ Profile Management (BASS) ~ BC BA & PAST Procedures ============ \n\
   30. Scan for Broadcast Source. \n\
   31. Stop Scan for Broadcast Source. \n\
   32. Associate to the Broadcast Source. \n\
   33. Send Sync Info to BA SD(PAST). \n\
   34. Dissociate from the Broadcast Source. \n\
\n\
================ BC BA Collocated BC Src ============ \n\
   35. Add Local Src Info to the BC Src DB \n\
   36. Send PA Set Info Transfer \n\
\n\
================ BC Src ============ \n\
   40. Allocate Session. \n\
   41. Free Session. \n\
   42. Configure Session. \n\
   43. Register Source EndPoint. \n\
   44. Setup Announcement. \n\
   45. End Announcement. \n\
   46. Start Broadcast. \n\
   47. Suspend Broadcast. \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get Context Info. \n\
   81. Set Context Info. \n\
\n\
   82. Free Saved Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure Notification \n\
\n\
Your Option ?: ";

/*
 * BA: This is the maximum number of Scan Delegators that can be saved in our
 * DB during EA Scan.
 */
#define APPL_BC_BA_MAX_SD_DEVICES     2U

/*
 * BA: This is the maximum number of Broadcast Sources discovered that can be
 * saved in our DB.
 */
#define APPL_BC_BA_MAX_BC_SRC_DEVICES 2U

/* To store rx_state info */
typedef struct _APPL_RX_BC_STATE
{
    GA_BASS_BC_RX_STATE   bc_rx_state;

    /*
     * The below arrays are referred by bis_sync_state, metadata_len and
     * metadata from the above bc_rx_state
     */
    UINT32 appl_bc_bass_bc_src_bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];

    UINT8 appl_bc_bass_bc_src_metadata_len[APPL_BC_MAX_NUM_OF_SUB_GROUPS];

    UINT8 appl_bc_bass_bc_src_metadata[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN];
}APPL_RX_BC_STATE;

typedef struct _APPL_BC_BASS_REMOTE_SD_DB
{
    /* Broadcast Assistant DB */
    /* Conn Device Index of Scan Delegator */
    UINT8                 conn_dev_index;

    /* BASS Handle */
    BASS_HANDLE           bass_ba_handle;
    /* BASS Broadcast Receive State Instances Discovered - ID */
    BASS_BC_RX_STATE_INST bc_rx_state_inst[BASS_CE_MAX_BC_RX_STATE_INSTANCES];
    /* BASS Broadcast Receive State Instance Read - Info  */
    APPL_RX_BC_STATE      bc_rx_state_db[BASS_CE_MAX_BC_RX_STATE_INSTANCES];
    /*
     * BASS Broadcast Receive State Instances Count.
     * This is received as part BASS_CE_SETUP_CNF event
     */
    UINT8                 bc_bass_rx_state_count;
    /* BASS Broadcast Receive State Instance in use */
    UINT8                 bc_bass_rx_state_in_use;
    /* BASS Broadcast Receive State Instance Read Chaining In progress */
    UINT8                 bc_bass_read_rx_state_chaining;
    /* Scan Status */
    UINT8                 scan_status;
}APPL_BC_BASS_REMOTE_SD_DB;

typedef struct _APPL_BC_BASS_BA_CP_INFO
{
    /* Index of the source_info, sent for CP operation */
    UINT8 index;
    /* CP Opcode */
    UINT8 cp_opcode;
}APPL_BC_BASS_BA_CP_INFO;

typedef struct _APPL_BC_BASS_BC_SRC_DB
{
    UINT16 sync_handle;
    GA_BASS_BAS_CP_ADD_SRC src;
    UINT8 src_length;
}APPL_BC_BASS_BC_SRC_DB;

/* BA: DB - Discovered Broadcast Scan Delegator - PACS */
DECL_STATIC APPL_BC_BASS_REMOTE_SD_DB appl_bc_bass_rem_sd_db[APPL_BC_BA_MAX_SD_DEVICES];
/*
 * BA: Holds the index and CP Opcode while doing CP operation. Used for copying
 * Source info after successful CP operation
 */
APPL_BC_BASS_BA_CP_INFO appl_bc_bass_ba_cp_info;
/* BA: Index of the Scan Delegator in use */
DECL_STATIC UINT8 appl_bc_rem_sd_in_use;
/* BA: Scan parameters */
DECL_STATIC GA_BRR_EXTENDED_SCAN_PARAMS appl_bc_ba_escan_params;

/* BC SRC: Timer Handle to suspend Broadcast */
DECL_STATIC BT_timer_handle  appl_ga_bc_src_suspend_timer_handle;

/* BA: This is global since once the Associate is complete,
 * GA_BC_SOURCE_CONFIG_IND/GA_BC_SOURCE_STREAM_IND keep flooding.
 * To avoid the same, use flags to indicate if its already printed once.
 */
BC_SINK_PARAMS appl_ga_ba_params;

/* BA: This gets updated at various events until Add operation is called,
 * hence global */
typedef struct _APPL_BASS_BA_BC_SRC_DB
{
    APPL_BC_BASS_BC_SRC_DB appl_bc_bass_bc_src;

    UINT32 appl_bc_bass_bc_src_bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];

    UINT8 appl_bc_bass_bc_src_metadata_len[APPL_BC_MAX_NUM_OF_SUB_GROUPS];

    UINT8 appl_bc_bass_bc_src_metadata[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN];
}APPL_BASS_BA_BC_SRC_DB;

APPL_BASS_BA_BC_SRC_DB appl_bc_bass_ba_bc_src[APPL_BC_BA_MAX_BC_SRC_DEVICES];

#ifdef BASS_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_bc_bass_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_bc_bass_char_info_ptr;
DECL_STATIC  UINT16              appl_bc_bass_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_bc_bass_range;
DECL_STATIC  GA_CHAR_INFO      * appl_bc_bass_char_info;
DECL_STATIC  BASS_HANDLE         appl_bc_bass_handle_access;
#endif /* BASS_SUPPORT_CONTEXT_MANAGE */

#ifdef BAP_BROADCAST

/* This is used to configure BASE structure,
 * used during Broadcast Source Setting
 */
typedef struct _BC_SRC_PARAMS
{
    /* Broadcast Session */
    UINT8 bc_ssn;

    /* Adv Set ID */
    UINT8 advset_handle;

    /* BIG Handle */
    UINT8 big_handle;

    /* Broadcast Packing */
    UINT8 bc_packing;

    /* Broadcast Encryption */
    UINT8 bc_encryption;

    /* Broadcast QOS Capability */
    GA_QOS_CONF bc_qos_conf;

    /* Broadcast Code */
    UCHAR bc_code[GA_BROADCAST_CODE_SIZE];

    /* Broadcast ID */
    UINT32 bc_id;

    /* Number of Subgroups */
    UINT8 bc_num_subgroups;

    /*
     * Subgroup Info: No. of BIS + Codec_info + Meta_info
     *                +
     *                Each BIS: BIS info (BIS_index + Codec_spec_conf)
     */
    APPL_BC_SUBGROUP bc_subgroup_info[APPL_BC_MAX_NUM_OF_SUB_GROUPS];

} BC_SRC_PARAMS;

DECL_STATIC BC_SRC_PARAMS appl_bc_src_params;
#endif /* BAP_BROADCAST */

#ifdef GA_PBP
/* Public Broadcast Audio Configuration Type: Standard Quality(SQ) */
#define APPL_BC_SRC_PBP_AUDIO_CONF_TYPE_SQ           PBP_BCAST_SQ_FEATURE_BIT
/* Public Broadcast Audio Configuration Type: High Quality(HQ) */
#define APPL_BC_SRC_PBP_AUDIO_CONF_TYPE_HQ           PBP_BCAST_HQ_FEATURE_BIT
/* Public Broadcast Audio Configuration Type: SQ or HQ */
UINT8 appl_bc_src_pbp_audio_conf_type;
#endif /* GA_PBP */

/* BC role: SD or BA or BC Src or BC Sink */
extern UINT8 bc_role;
/*
 * Used to differentiate ongoing scan: either Scanning for SD or Scanning for
 * UCS.
 */
extern UINT8 is_scan_enabled;
/* Indicates what roles of the App are initialized */
extern APPL_GA_ROLE ga_role;

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
void appl_ga_bc_ba_init(void);

GA_RESULT appl_ga_bc_src_fetch_first_valid_sg_bis_info_index
          (
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          );

GA_RESULT appl_ga_bc_src_fetch_next_valid_sg_bis_info_index
          (
              UINT8   curr_sg_info_index,
              UINT8   curr_bis_info_index,
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          );

GA_RESULT appl_ga_bc_ba_update_transmitting_audio
          (
              UINT16 bis_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8 qos_conf_set_id,
              UINT8 bps,
              UINT8 status
          );

#define appl_ga_bc_ba_start_transmitting_audio(bis_handle, cs_conf, qos_conf_set_id, bps) \
        appl_ga_bc_ba_update_transmitting_audio(bis_handle, cs_conf, qos_conf_set_id, bps, GA_TRUE)

#define appl_ga_bc_ba_stop_transmitting_audio(bis_handle, cs_conf, qos_conf_set_id, bps) \
        appl_ga_bc_ba_update_transmitting_audio(bis_handle, cs_conf, qos_conf_set_id, bps, GA_FALSE)

void appl_ga_bc_ba_handle_setup_iso_data_path_event
     (
         UINT8 start_audio_lc3,
         UINT8 status,
         UINT8 curr_sg_info_index,
         UINT8 curr_bis_info_index
     );

void appl_ga_bc_ba_handle_remove_iso_data_path_event
     (
         UINT8 status,
         UINT8 curr_sg_info_index,
         UINT8 curr_bis_info_index
     );

GA_RESULT appl_ga_bc_bass_ba_cb
          (
              /* IN */ GA_ENDPOINT    * device,
              /* IN */ UINT8            evt_id,
              /* IN */ UINT16           evt_status,
              /* IN */ BASS_EVT       * evt_data
          );

/* BA - Handle rx_state data when read_response or notification is received */
GA_RESULT appl_ga_bc_ba_handle_rx_state_data
          (
              GA_ENDPOINT  device,
              UINT8      * rx_state_evt_data,
              UINT8      * sd_index,
              UINT8      * rx_state_db_index
          );

/* BA - Get the rx_state_db index from Source_ID and SD_Index */
GA_RESULT appl_ga_bc_ba_get_bc_rx_state_db_index
          (
              UINT8   source_id,
              UINT8   sd_index,
              UINT8 * rx_state_db_index
          );

/* BA - Get free rx_state_db index to add new entry of rx_state */
GA_RESULT appl_ga_bc_ba_fetch_free_bc_rx_state_index
          (
              UINT8   sd_index,
              UINT8 * rx_state_db_index
          );

/* BA - Update rx_state data */
void appl_ga_bc_ba_update_rx_state_data
     (
         UINT8   sd_index,
         UINT8   rx_state_db_index,
         UINT8 * rx_state_evt_data
     );

GA_RESULT appl_ga_bc_ba_get_rem_sd_db_index(GA_ENDPOINT device, UINT8 * sd_index);

void appl_bc_set_default_scan_settings(void);

GA_RESULT appl_bc_scan_for_sd(void);

GA_RESULT appl_bc_stop_scan_for_sd(void);

GA_RESULT appl_bc_bass_ba_read_bc_rx_state(UINT8 sd_index);

GA_RESULT appl_bc_bass_ba_add_bc_src(UINT8 index);
GA_RESULT appl_bc_bass_ba_modify_bc_src(UINT8 sid);
GA_RESULT appl_bc_ba_bass_set_bc_code(UINT8 source_id);
GA_RESULT appl_bc_ba_bass_rem_bc_src(UINT8 source_id);

void appl_ga_bc_ba_get_input_for_modify_src_pa(UINT8 index);
void appl_ga_bc_ba_get_input_for_modify_src_bis(UINT8 index);
void appl_ga_bc_ba_get_input_for_modify_src_metadata(UINT8 index);
void appl_ga_bc_ba_parse_and_get_input_for_metadata
     (
         /* IN */    UINT8 * metadata,
         /* IN */    UINT8   metadata_len,
         /* INOUT */ UINT8 * new_metadata,
         /* INOUT */ UINT8 * new_metadata_len
     );

GA_RESULT appl_bc_ba_send_pa_sync_info(void);

#ifdef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
void appl_bc_src_bap_set_bc_params_using_default_values(APPL_GA_ROLE role);
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
void appl_bc_src_bap_set_bc_params_using_runtime_values(void);
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

GA_RESULT appl_bc_bass_ba_inform_scan_status(void);

GA_RESULT appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle
          (
              UINT16   bis_conn_handle,
              UINT8  * sg_info_index,
              UINT8  * bis_info_index
          );

/* BA - Fetch a free entry to save SD Device in DB */
GA_RESULT appl_bc_bass_ba_fetch_entry_in_db
          (
              UINT8 * rem_dev_index
          );

/* BA - Set the remote SD in use for all operations */
void appl_bc_bass_ba_set_rem_sd_in_use(void);

/* BA - Is SD already present in our BA DB */
GA_RESULT appl_bc_bass_ba_is_sd_not_added_in_db
          (
              GA_ENDPOINT sd_device
          );

GA_RESULT appl_bc_bass_ba_fetch_free_bc_src_index_in_db(UINT8 * index);

GA_RESULT appl_bc_bass_ba_fetch_bc_src_entry_present_in_db
          (
              UINT16   sync_handle,
              UINT8  * index
          );

GA_RESULT appl_bc_bass_ba_is_bc_src_entry_present_in_db
          (
              UINT8 * adv_addr,
              UINT8   adv_type,
              UINT8 * index
          );

void appl_bc_bass_ba_clear_bc_src_db(void);

void appl_bc_bass_ba_add_bc_src_info_into_db
     (
         UINT8 index,
         BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS bc_source_announcement,
         BAP_BROADCAST_ANNOUNCEMENT bc_annoucement
     );

GA_RESULT appl_bc_bass_ba_validate_add_bc_src(UINT8 index);

UINT8 appl_bc_bass_ba_is_pacs_compatible(UINT8 index);
GA_RESULT appl_bc_bass_ba_is_src_added_in_ba_db(UINT8 index);

GA_RESULT appl_bc_bass_ba_parse_and_display_bc_rx_state
          (
              GA_BASS_BC_RX_STATE br_rx_state_info
          );
void appl_bc_bass_ba_parse_and_display_addr_type(UINT8 addr_type);
void appl_bc_bass_ba_parse_and_display_bc_rx_state_bis_sync_state(UINT32 bis_sync);
void appl_bc_bass_ba_parse_and_display_PA_Sync_State(UINT8 pa_sync_state);
void appl_bc_bass_ba_parse_and_display_BIG_encryption(UINT8 big_encryption);
void appl_bc_src_bap_display_bc_settings(void);
GA_RESULT appl_ga_bc_ba_src_setup_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          );
GA_RESULT appl_ga_bc_ba_src_remove_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          );
void appl_ga_bc_ba_bass_display_ga_result(GA_RESULT status);

void appl_ga_bc_set_appearance_value(UINT16 * appearance);

void appl_ga_bc_src_suspend_timer_handler
     (
         void  * data,
         UINT16  datalen
     );

GA_RESULT appl_bc_ba_send_set_info_transfer(void);
void appl_bc_ba_add_local_bc_src_to_src_db(void);

/* --------------------------------------------- APIs */
GA_RESULT appl_ga_bc_ba_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          )
{
    UCHAR      i;
    UINT8      value_1;
    UINT16     value_2;
    UINT8      status;
    UCHAR      num_report;
    UINT8      marker;
    UINT8      sub_event;
    UINT8      adtype;
    UINT8      adlength;
    UINT16     svc_uuid;
    UINT16     bis_conn_handle;
    UINT8      curr_sg_info_index;
    UINT8      curr_bis_info_index;
    UINT8      sg_info_index;
    UINT8      bis_info_index;
    UINT8      start_audio_lc3;
    GA_RESULT  retval;

    /* Initialize */
    i = 0U;
    value_1 = 0U;
    value_2 = 0U;
    status = 0U;
    num_report = 0U;
    marker = 0U;
    adtype = 0U;
    adlength = 0U;
    svc_uuid = 0U;
    bis_conn_handle = 0U;
    curr_sg_info_index = 0U;
    curr_bis_info_index = 0U;
    sg_info_index = 0U;
    bis_info_index = 0U;

    /* By default, Set data and len as 0 */
    sub_event = 0U;

    if (!((IS_APPL_GA_BC_ROLE_BA(bc_role)) ||
        (IS_APPL_GA_BC_ROLE_BC_SRC(bc_role)) ||
        (IS_APPL_GA_BC_ROLE_BA_PLUS_BC_SRC(bc_role))))
    {
        return GA_SUCCESS;
    }

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_ga_bc_ba_hci_cb_handler\n");

    /* Switch on the Event Code */
    switch (event_code)
    {
        case HCI_COMMAND_COMPLETE_EVENT:
            APPL_TRC("HCI_COMMAND_COMPLETE_EVENT\n");
            /* Number of Command Packets */
            GA_UNPACK_LE_1_BYTE(&value_1, &event_data[marker]);
            marker++;

            /* Command Opcode */
            GA_UNPACK_LE_2_BYTE(&value_2, &event_data[marker]);
            marker += 2U;

            /* Command Status */
            GA_UNPACK_LE_1_BYTE(&status, &event_data[marker]);
            marker++;

            /* Command Return Parameters */
            if (event_datalen >= 4U)
            {
                switch (value_2)
                {
                case HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE:
                    APPL_DBG("HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE\n");
                    break;

                case HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE:
                    APPL_DBG("HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE\n");
                    break;

                case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE:
                    hci_unpack_2_byte_param(&bis_conn_handle, &event_data[marker]);
                    marker += 2U;
                    if (0x00 == status)
                    {
                        if (GA_SUCCESS == appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle
                                          (
                                              bis_conn_handle,
                                              &curr_sg_info_index,
                                              &curr_bis_info_index
                                          ))
                        {
                            /* Get next Valid BIS Index */
                            retval = appl_ga_bc_src_fetch_next_valid_sg_bis_info_index
                                     (
                                         curr_sg_info_index,
                                         curr_bis_info_index,
                                         &sg_info_index,
                                         &bis_info_index
                                     );
                            if (GA_SUCCESS != retval)
                            {
                                /*
                                 * There are no more BIS for ISO Data Setup,
                                 * Trigger Start Audio Tx with valid QoS Conf ID
                                 * only for the First Valid BIS and for rest just
                                 * Update ISO Entry. No action on LC3 Encoder
                                 * Module and Audio PL Generator Module.
                                 */
                                start_audio_lc3 = GA_TRUE;
                            }
                            else
                            {
                                /* There are more BIS for Setup ISO Data Path */
                                start_audio_lc3 = GA_FALSE;
                            }
                            /* Update ISO Entry for all BISes, Start Tx for all BISes */
                            appl_ga_bc_ba_handle_setup_iso_data_path_event
                            (
                                start_audio_lc3,
                                status,
                                curr_sg_info_index,
                                curr_bis_info_index
                            );

                            if (GA_TRUE != start_audio_lc3)
                            {
                                /*
                                 * Trigger setup ISO data path for next valid
                                 * BIS index since we havent completed setup ISO
                                 * for all BISes.
                                 */
                                if (GA_FAILURE == appl_ga_bc_ba_src_setup_iso
                                                  (
                                                      sg_info_index,
                                                      bis_info_index
                                                  ))
                                {
                                    /*
                                     * Setup ISO failed ! Chain is broken
                                     * automatically !
                                     */
                                    APPL_TRC("Setup ISO Failed ! Chain broken !");
                                }
                                else
                                {
                                    /* Setup ISO is Ongoing, Wait */
                                }
                            }
                        }
                    }
                    break;

                case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE:
                    hci_unpack_2_byte_param(&bis_conn_handle, &event_data[marker]);
                    marker += 2U;
                    if (0x00 == status)
                    {
                        if (GA_SUCCESS == appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle
                                          (
                                              bis_conn_handle,
                                              &curr_sg_info_index,
                                              &curr_bis_info_index
                                          ))
                        {
                            /* Get next Valid BIS Index */
                            retval = appl_ga_bc_src_fetch_next_valid_sg_bis_info_index
                                     (
                                         curr_sg_info_index,
                                         curr_bis_info_index,
                                         &sg_info_index,
                                         &bis_info_index
                                     );
                            appl_ga_bc_ba_handle_remove_iso_data_path_event
                            (
                                status,
                                curr_sg_info_index,
                                curr_bis_info_index
                            );
                            if (GA_SUCCESS != retval)
                            {
                                /* There are no more BIS for ISO Data Removal. */
                            }
                            else
                            {
                                /* There are more BIS for Removal of ISO Data Path */
                                if (GA_FAILURE == appl_ga_bc_ba_src_remove_iso
                                                  (
                                                      sg_info_index,
                                                      bis_info_index
                                                  ))
                                {
                                    /*
                                     * Removal of ISO failed ! Chain is broken
                                     * automatically !
                                     */
                                    APPL_TRC("Removal of ISO Failed ! Chain broken !");
                                }
                                else
                                {
                                    /* Removal of ISO is Ongoing, Wait */
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
            }
            break;

        case HCI_LE_META_EVENT:
            marker = 0U;
            /* Extract the subevent code */
            GA_UNPACK_LE_1_BYTE(&sub_event, &event_data[marker]);
            marker++;

            switch (sub_event)
            {
            case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT:
                if (APPL_GA_SCAN_FOR_SD == is_scan_enabled)
                {
                    /* Num_Reports Length: 1 Octet */
                    num_report = event_data[marker];
                    marker++;

                    for (i = 0U; i < num_report; i++)
                    {
                        marker += 23U;

                        /* Data Length: 1 Octet */
                        value_1 = event_data[marker];
                        marker++;

                        while (0x00U < value_1)
                        {
                            /* Extract the Length from the event data */
                            GA_UNPACK_LE_1_BYTE(&adlength, &event_data[marker]);
                            marker++;
                            value_1 -= 1U;

                            /* TODO: Check Length for each param? */

                            if (0x000 != adlength)
                            {
                                /* Extract the Ad type from the event data */
                                GA_UNPACK_LE_1_BYTE(&adtype, &event_data[marker]);
                                marker++;
                                value_1 -= 1U;

                                if (GA_ADTYPE_SERVICE_DATA == adtype)
                                {
                                    /* Extract the Value - Service UUID from the event data */
                                    GA_UNPACK_LE_2_BYTE(&svc_uuid, &event_data[marker]);
                                    marker += 2U;
                                    value_1 -= 2U;

                                    /* Check if BASS Service ID is in the Service Data value */
                                    if (GA_SERVICE_BASS == svc_uuid)
                                    {
                                        /* BA is scanning for SD, Found SD, Reset marker */
                                        marker = 0U;

                                        break;
                                    }
                                    else
                                    {
                                        /* Service UUID not interested, Validate further */
                                        svc_uuid = 0x0000U;
                                        marker += (adlength - 3U);
                                        value_1 -= (adlength - 3U);
                                    }
                                }
                                else
                                {
                                    /* Interested AD Type - Not found yet! */
                                    marker += (adlength - 1U);
                                    value_1 -= (adlength - 1U);
                                }
                            }
                            else
                            {
                                /* AdLength is 0 */
                                break;
                            }
                        }
                        if (GA_SERVICE_BASS == svc_uuid)
                        {
                            /* Display the SD Device */
                            appl_ga_utils_parse_and_display_ea_report_subevent(event_data, event_datalen);

                            break;
                        }
                    }
                }
                break;

            default:
                /* Do Nothing */
                break;
            }
            break;

        default:
            /* Do Nothing */
            break;
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_ga_bc_ba_hci_cb_handler\n");
    return GA_SUCCESS;
}

void appl_ga_bc_ba_handle_setup_iso_data_path_event
     (
         UINT8 start_audio_lc3,
         UINT8 status,
         UINT8 curr_sg_info_index,
         UINT8 curr_bis_info_index
     )
{
    GA_RESULT  retval;
    UINT8      sg_info_index;
    UINT8      bis_info_index;
    UINT8      qos_conf_set_id;

    /* Initialize */
    retval = GA_SUCCESS;
    sg_info_index = 0U;
    bis_info_index = 0U;

    if (0x00 == status)
    {
        if (AUDIO_ISO_UPDATE_IN_PROGRESS ==
            appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].iso_state)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_src_update_entry
                              (
                                  appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].bis_handle,
                                  AUDIO_ISO_DATA_SETUP,
                                  status,
                                  AUDIO_IGNORE_VALUE
                              ))
            {
                APPL_TRC("[APPL][GA][%s]: ISO Data Path Setup Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
                /* Update the ISO state */
                appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].iso_state =
                    AUDIO_ISO_SETUP_COMPLETE;

                if (GA_TRUE == start_audio_lc3)
                {
                    /*
                     * We have finished ISO Entry Setup for all BISes in all SG.
                     * Now create LC3 encoder module and move it to streaming.
                     */
                    /* QoS Conf Set ID of the BIG is taken here */
                    qos_conf_set_id = appl_bc_src_params.bc_subgroup_info[0U].qos_conf_set_id;
                    for (sg_info_index = 0U; sg_info_index < appl_bc_src_params.bc_num_subgroups; sg_info_index++)
                    {
                        for (bis_info_index = 0U; bis_info_index < appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
                        {
                            /* We are Audio Source, Start Transmitting Audio data */
                            if (GA_SUCCESS == appl_ga_bc_ba_start_transmitting_audio
                                              (
                                                  appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                                  appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                                  qos_conf_set_id,
                                                  AUDIO_NUM_OF_BITS_PER_SAMPLE
                                              ))
                            {
                                appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                                    GA_TRUE;
                                /*
                                 * appl_ga_bc_ba_start_transmitting_audio
                                 * Create LC3 encoder module and Setup and
                                 * Start Audio PL module here. Since for
                                 * broadcast we have multiple BISes in BIG
                                 * and all move to streaming at once,
                                 * appl_ga_bc_ba_start_transmitting_audio
                                 * is called with qos_conf_set_id as 0xFF
                                 * for rest of the BISes to indicate update of
                                 * ISO and Streaming state only.
                                 */
                                qos_conf_set_id = 0xFFU;
                            }
                            else
                            {
                                APPL_ERR
                                (
                                    "[APPL][GA][%s]: Failed to create LC3 Encoder or "
                                    "start Audio_pl !\n", BA_GET_ROLE_STRING(bc_role)
                                );
                                if (0xFFU != qos_conf_set_id)
                                {
                                    /*
                                     * The LC3 create and Audio PL Setup and
                                     * Start is called only once. Hence if it
                                     * fails for the first time, there is no
                                     * point in updating of ISO and Streaming
                                     * state for rest.
                                     */
                                    retval = GA_FAILURE;
                                    break;
                                }
                            }
                        }
                        if (GA_FAILURE == retval)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: ISO Data Path Setup Status: Update entry failed !\n", BA_GET_ROLE_STRING(bc_role));
            }
        }
    }
    else
    {
        APPL_ERR("[APPL][GA][%s]: ISO Data Path Setup Status: Failed !\n", BA_GET_ROLE_STRING(bc_role));
    }

    return;
}

void appl_ga_bc_ba_handle_remove_iso_data_path_event
     (
         UINT8 status,
         UINT8 curr_sg_info_index,
         UINT8 curr_bis_info_index
     )
{
    if (0x00 == status)
    {
        if (AUDIO_ISO_UPDATE_IN_PROGRESS ==
            appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].iso_state)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_src_remove_entry
                              (
                                  appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].bis_handle
                              ))
            {
                APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
                /* Update the ISO state */
                appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bis_info[curr_bis_info_index].iso_state =
                    AUDIO_ISO_IDLE;
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n", BA_GET_ROLE_STRING(bc_role));
            }
        }
    }
    else
    {
        APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Failed !\n", BA_GET_ROLE_STRING(bc_role));
    }

    return;
}

GA_RESULT appl_ga_bc_bass_ba_cb
          (
              /* IN */ GA_ENDPOINT    * device,
              /* IN */ UINT8            evt_id,
              /* IN */ UINT16           evt_status,
              /* IN */ BASS_EVT       * evt_data
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      sd_index;
    UINT8      rx_state_db_index;

    /* Initialize */
    retval = GA_SUCCESS;
    index = 0U;
    sd_index = 0U;
    rx_state_db_index = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_ga_bc_bass_ba_cb\n");

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    if (NULL == evt_data)
    {
        APPL_ERR("ga_data is NULL !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("BC BASS Broadcast Assistant");

    switch (evt_id)
    {
    case BASS_CE_SETUP_CNF:
        APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
        APPL_TRC("[SECTION]     : BASS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : BASS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_bc_ba_bass_display_ga_result(evt_status);
        APPL_TRC
        (
            "[Event]       : BASS_CE_SETUP_CNF (0x%02X)\n",
            evt_id
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", evt_data->bass_handle);
        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);

            UCHAR* data = (UCHAR *)evt_data->data;

            if (evt_data->len != 0U)
            {
                APPL_TRC("Data:\n");
                APPL_TRC("Len: 0x%02X,  ID: ", evt_data->len);
                for (index = 0U; index < evt_data->len; index++)
                {
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_inst[index] =
                        data[index];
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_bass_rx_state_count++;
                    APPL_TRC("0x%02X ", data[index]);
                }
                APPL_TRC("\n");
            }
            else
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        else
        {
            APPL_ERR("Context Setup Complete Failed !\n");
        }
        break;

    case BASS_CE_RELEASE_CNF:
        APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
        APPL_TRC("[SECTION]     : BASS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : BASS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_bc_ba_bass_display_ga_result(evt_status);
        APPL_TRC
        (
            "[Event]       : BASS_CE_RELEASE_CNF (0x%02X)\n",
            evt_id
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", evt_data->bass_handle);
        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);

            if (evt_data->len == 0U)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case BASS_CE_READ_BC_RX_STATE_CNF:
        APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
        APPL_TRC("[SECTION]     : BASS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : BASS CLIENT-READ\n");
        APPL_TRC("[TYPE]        : READ RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_bc_ba_bass_display_ga_result(evt_status);
        APPL_TRC
        (
            "[Event]       : BASS_CE_READ_BC_RX_STATE_CNF (0x%02X)\n",
            evt_id
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", evt_data->bass_handle);
        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Broadcast Receive State (0x%04X)\n",
                GA_CHAR_BASS_BCAST_RECEIVE_STATE
            );
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Broadcast Receive State Instance: 0x%02X\n",
                evt_data->rx_state_inst
            );

            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);
            if ((evt_data->len != 0U) && (NULL != evt_data->data))
            {
                APPL_TRC("Data:\n");
                /*
                 * Updating the rx_state_data and fetching SD device index and
                 * rx_state_db_index for which read response is received
                 */
                if (GA_SUCCESS == appl_ga_bc_ba_handle_rx_state_data
                                  (
                                      *device,
                                      (UINT8*)evt_data->data,
                                      &sd_index,
                                      &rx_state_db_index
                                  ))
                {
                    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info_len =
                        evt_data->len;
                    appl_bc_bass_ba_parse_and_display_bc_rx_state
                    (
                        appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state
                    );

                }
                if (GA_TRUE == appl_bc_bass_rem_sd_db[sd_index].bc_bass_read_rx_state_chaining)
                {
                    /* Continue until all the instances discovered are read */
                    appl_bc_bass_rem_sd_db[sd_index].bc_bass_rx_state_in_use++;
                    if (appl_bc_bass_rem_sd_db[sd_index].bc_bass_rx_state_in_use
                        < (appl_bc_bass_rem_sd_db[sd_index].bc_bass_rx_state_count))
                    {
                        retval = appl_bc_bass_ba_read_bc_rx_state(sd_index);
                    }
                    else
                    {
                        appl_bc_bass_rem_sd_db[sd_index].bc_bass_read_rx_state_chaining = GA_FALSE;
                        APPL_TRC("Read all instances finished\n");
                    }
                }
            }
        }
        break;

    case BASS_CE_BAS_CP_CNF:
        APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
        APPL_TRC("[SECTION]     : BASS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : BASS CLIENT-WRITE\n");
        APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_bc_ba_bass_display_ga_result(evt_status);
        APPL_TRC
        (
            "[Event]       : BASS_CE_BAS_CP_CNF (0x%02X)\n",
            evt_id
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", evt_data->bass_handle);
        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Broadcast Audio Scan Control Point (0x%04X)\n",
                GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP
            );
            GA_FORMAT_DISPLAY();

            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);
            if (0U == evt_data->len)
            {
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case BASS_CE_BC_RX_STATE_IND:
        APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
        APPL_TRC("[SECTION]     : BASS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : BASS CLIENT-NOTIFICATION\n");
        APPL_TRC("[TYPE]        : NOTIFICATION\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_bc_ba_bass_display_ga_result(evt_status);
        APPL_TRC
        (
            "[Event]       : BASS_CE_BC_RX_STATE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC("[Service Cntx]: 0x%02X\n", evt_data->bass_handle);
        if (GA_SUCCESS == evt_status)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Broadcast Receive State (0x%04X)\n",
                GA_CHAR_BASS_BCAST_RECEIVE_STATE
            );
            GA_FORMAT_DISPLAY();

            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);
            APPL_TRC("Data: ");
            if (0U == evt_data->len)
            {
                APPL_TRC("NULL\n");
            }
            else
            {
                /*
                 * Updating the rx_state_data and fetching SD device index and
                 * rx_state_db_index for which notification is received
                 */
                if (GA_SUCCESS == appl_ga_bc_ba_handle_rx_state_data
                                  (
                                      *device,
                                      (UINT8*)evt_data->data,
                                      &sd_index,
                                      &rx_state_db_index
                                  ))
                {
                    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info_len =
                        evt_data->len;
                    appl_bc_bass_ba_parse_and_display_bc_rx_state
                    (
                        appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state
                    );
                }

                /*
                 * Check for Big Encryption Field value:
                 * 0x01- Broadcast_Code required
                 */
                if (BASS_BIG_ENCRYPTION_BC_CODE_REQUIRED ==
                    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.big_encryption)
                {
                    /* Perform set Broadcast Code Control Point operation */
                    APPL_TRC
                    (
                        "[APPL][GA][BC_BA]: SD requesting for Broadcast Code for"
                        "Decryption. Source_ID: 0x%02X. Use Menu Option 24\n",
                        appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.id
                    );
                }
            }
        }
        break;

    default:
        APPL_ERR("Unknown Event ID: 0x%02X !\n", evt_id);
        break;
    }

    GA_CB_EXIT();

    APPL_DBG("[APPL][GA][BC_BA]: << appl_ga_bc_bass_ba_cb\n");

    return retval;
}

GA_RESULT appl_ga_bc_ba_bc_sink_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          )
{
    GA_RESULT retval;
    BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS bc_source_announcement = { 0 };
    BAP_BROADCAST_ANNOUNCEMENT bc_annoucement = { 0 };
    BAP_BC_ASSOCIATE_PARAMS bc_associate = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_1 bc_basic_announcement_1 = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_2 bc_basic_announcement_2 = { 0 };
    BAP_BC_SOURCE_CONFIG_PARAMS bc_source_config = { 0 };
    BAP_BC_SOURCE_STREAM_PARAMS bc_source_stream = { 0 };
    BAP_BC_DISSOCIATE_PARAMS bc_dissociate = { 0 };
    UINT8 i;
    UINT32 delay;
    UINT8 n_sg, sg_info_index;
    UINT8 n_bis, num_bis;
    UINT8 parsed_len;
    UINT8 marker;

    GA_LC3_CODEC_SPECIFIC_CONF rem_cs_conf = { 0 };

    UINT8  prev_metadata_len;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    delay = 0U;
    n_sg = 0U;
    sg_info_index = 0U;
    num_bis = 0U;
    parsed_len = 0U;
    prev_metadata_len = 0U;
    marker = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_ga_bc_ba_bc_sink_handler\n");

    switch (ga_event)
    {
    case GA_BC_SCAN_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SCAN_ANNOUNCEMENT_CNF (0x%02X)\n",
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

    case GA_BC_SOURCE_ANNOUNCEMENT_IND:
    {
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params
            (
                GA_BC_SOURCE_ANNOUNCEMENT_IND,
                ga_data,
                &bc_source_announcement
            ))
        {
            if (GA_SUCCESS == GA_broadcast_decode_broadcast_announcement
                              (
                                  bc_source_announcement.data,
                                  bc_source_announcement.datalen,
                                  &bc_annoucement
                              ))
            {
                /* Validating if the scan is triggered by BA or SD */
                if ((GA_TRUE == appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status) &&
                    (GA_SUCCESS != appl_bc_bass_ba_is_bc_src_entry_present_in_db
                                   (
                                       bc_source_announcement.address,
                                       bc_source_announcement.address_type,
                                       &i
                                   )))
                {
                    /*
                     * BA has triggered PA Scan and this is a new src to be added,
                     * Go ahead, and trigger BASS CP Add Source.
                     * If the Src is already added, Skip BASS CP Add Source.
                     */
                    i = 0U;
                    /* This is a new entry, Add this BC Src */
                    if (GA_SUCCESS == appl_bc_bass_ba_fetch_free_bc_src_index_in_db(&i))
                    {
                        /* There is a free index, Add it here */
                        appl_bc_bass_ba_add_bc_src_info_into_db
                        (
                            i,
                            bc_source_announcement,
                            bc_annoucement
                        );

                        /* Display */
                        GA_CB_ENTRY("BC CAP Server");
                        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
                        APPL_TRC
                        (
                            "[Event]       : GA_BC_SOURCE_ANNOUNCEMENT_IND (0x%02X)\n",
                            ga_event
                        );
                        APPL_TRC("\n");
                        GA_FORMAT_DISPLAY();

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Event Type: 0x%04X\n",
                            (UCHAR)sizeof(UINT16), bc_source_announcement.event_type
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Address Type: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_source_announcement.address_type
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Address: %02X %02X %02X %02X %02X %02X\n",
                            GA_BD_ADDR_SIZE,
                            bc_source_announcement.address[0],
                            bc_source_announcement.address[1],
                            bc_source_announcement.address[2],
                            bc_source_announcement.address[3],
                            bc_source_announcement.address[4],
                            bc_source_announcement.address[5]
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Primary Phy: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_source_announcement.primary_phy
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Secondary Phy: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_source_announcement.secondary_phy
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  SID: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_source_announcement.advertising_sid
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Tx Power: %d\n",
                            (UCHAR)sizeof(INT8), bc_source_announcement.tx_power
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  RSSI: %d\n",
                            (UCHAR)sizeof(INT8), bc_source_announcement.rssi
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  PA Interval: 0x%04X\n",
                            (UCHAR)sizeof(UINT16),
                            bc_source_announcement.periodic_adv_interval
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Direct Address Type: 0x%02X\n",
                            (UCHAR)sizeof(UINT8),
                            bc_source_announcement.direct_address_type
                        );
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Direct Address: %02X %02X %02X %02X %02X %02X\n",
                            GA_BD_ADDR_SIZE,
                            bc_source_announcement.direct_address[0],
                            bc_source_announcement.direct_address[1],
                            bc_source_announcement.direct_address[2],
                            bc_source_announcement.direct_address[3],
                            bc_source_announcement.direct_address[4],
                            bc_source_announcement.direct_address[5]
                        );
                        APPL_TRC("Data Length: 0x%02X\n", bc_source_announcement.datalen);

                        APPL_TRC("Data:\n");
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Broadcast_ID: 0x%06X\n",
                            BAP_BC_ID_LEN, bc_annoucement.broadcast_id
                        );
                        appl_ga_utils_parse_and_display_adv_data
                        (
                            bc_source_announcement.data,
                            bc_source_announcement.datalen
                        );

                        APPL_TRC("Source added in Index: %d", i);
                    }
                    else
                    {
                        /* Reached Max */
                    }
                }
                else
                {
                    /* Do not display since it is already added */
                }
            }
        }
    }
    break;

    case GA_BC_SCAN_END_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SCAN_END_CNF (0x%02X)\n",
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

    case GA_BC_ASSOCIATE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_ASSOCIATE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_ASSOCIATE_CNF,
                              ga_data,
                              &bc_associate
                          ))
        {
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Error: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_associate.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_associate.sync_handle
            );
            appl_ga_ba_params.bcast_synchandle =
                bc_associate.sync_handle;
            APPL_TRC
            (
                "Len: 0x%02X,  SID: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_associate.advertising_sid
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Advertiser Address Type: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_addr_type
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Advertiser Address: %02X %02X %02X %02X %02X %02X\n",
                GA_BD_ADDR_SIZE,
                bc_associate.advertiser_addr[0],
                bc_associate.advertiser_addr[1],
                bc_associate.advertiser_addr[2],
                bc_associate.advertiser_addr[3],
                bc_associate.advertiser_addr[4],
                bc_associate.advertiser_addr[5]
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Adv Phy: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PA Interval: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_associate.periodic_adv_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Adv Clock Accuracy: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_clock_accuracy
            );

            if (0x00U == bc_associate.error_code)
            {
                /* Validate if the BA triggered Associate */
                if ((GA_TRUE == appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status) &&
                    GA_SUCCESS == appl_bc_bass_ba_is_bc_src_entry_present_in_db
                                  (
                                      bc_associate.advertiser_addr,
                                      bc_associate.advertiser_addr_type,
                                      &i
                                  ))
                {
                    /*
                     * BA: During scan for BC Src, the info of the BC Src needs
                     * to be updated
                     */
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.pa_interval =
                        bc_associate.periodic_adv_interval;

                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.sync_handle =
                        bc_associate.sync_handle;
                }
            }
        }
        break;

    case GA_BC_SOURCE_CONFIG_IND:
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_SOURCE_CONFIG_IND,
                              ga_data,
                              &bc_source_config
                          ))
        {
            if (appl_ga_ba_params.config_synchandle ==
                appl_ga_ba_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }
            GA_CB_ENTRY("BC CAP Server");
            appl_ga_ba_params.config_synchandle =
                appl_ga_ba_params.bcast_synchandle;

            appl_ga_utils_bc_display_sink_console_header(device, ga_status);
            APPL_TRC
            (
                "[Event]       : GA_BC_SOURCE_CONFIG_IND (0x%02X)\n",
                ga_event
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();

            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_config.sync_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Tx Power: %d\n",
                (UCHAR)sizeof(INT8), bc_source_config.tx_power
            );
            APPL_TRC
            (
                "Len: 0x%02X,  RSSI: %d\n",
                (UCHAR)sizeof(INT8), bc_source_config.rssi
            );
            APPL_TRC
            (
                "Len: 0x%02X,  CTE Type: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_config.cte_type
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Data Status: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_config.data_status
            );
            APPL_TRC("Data Length: 0x%02X\n", bc_source_config.datalen);
            APPL_TRC("Data:\n");

            if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_info
                              (
                                  bc_source_config.data,
                                  &delay,
                                  &n_sg,
                                  &parsed_len
                              ))
            {
                APPL_TRC
                (
                    "Len: 0x03,  Presentation_Delay: 0x%04X\n",
                    delay
                );
                APPL_TRC
                (
                    "Len: 0x%02X,  Num_Subgroups: 0x%02X\n",
                    (UCHAR)sizeof(n_sg), n_sg
                );

                /* Presentation_Delay + Num_Subgroups is parsed */
                bc_source_config.data += parsed_len;

                if (GA_SUCCESS == appl_bc_bass_ba_fetch_bc_src_entry_present_in_db
                                  (
                                      bc_source_config.sync_handle,
                                      &i
                                  ))
                {
                    while (sg_info_index < n_sg)
                    {
                        parsed_len = 0U;
                        if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_1
                                          (
                                              bc_source_config.data,
                                              &bc_basic_announcement_1,
                                              &n_bis,
                                              &parsed_len
                                          ))
                        {
                            APPL_TRC("Subgroups[%d]:\n", (sg_info_index + 1U));
                            APPL_TRC
                            (
                                "Len: 0x%02X,  Num_BIS: 0x%02X\n",
                                (UCHAR)sizeof(n_bis), n_bis
                            );
                            appl_ga_utils_bap_display_codec_id(bc_basic_announcement_1.codec);

                            APPL_TRC
                            (
                                "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
                                (UCHAR)sizeof(UINT8), bc_basic_announcement_1.codec.ie_len
                            );

                            APPL_TRC
                            (
                                "Len: 0x%02X,  Codec_Specific_Configuration:\n",
                                bc_basic_announcement_1.codec.ie_len
                            );

                            appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
                            (
                                bc_basic_announcement_1.codec.ie,
                                bc_basic_announcement_1.codec.ie_len,
                                &rem_cs_conf
                            );

                            APPL_TRC
                            (
                                "Len: 0x%02X,  Metadata_Length: 0x%02X\n",
                                (UCHAR)sizeof(UINT8), bc_basic_announcement_1.meta.length
                            );
                            /* Copy data to the Local array */
                            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[sg_info_index] = bc_basic_announcement_1.meta.length;

                            if (0x00U != bc_basic_announcement_1.meta.length)
                            {
                                APPL_TRC
                                (
                                    "Len: 0x%02X,  Metadata:\n",
                                    bc_basic_announcement_1.meta.length
                                );

                                appl_ga_utils_bap_parse_and_display_metadata_ltv
                                (
                                    bc_basic_announcement_1.meta.data,
                                    bc_basic_announcement_1.meta.length,
                                    NULL,
                                    NULL
                                );
                                /* Copy metadata to the Local array */
                                BT_UNPACK_LE_N_BYTE
                                (
                                    &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata[prev_metadata_len],
                                    &bc_basic_announcement_1.meta.data[0U],
                                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[sg_info_index]
                                );
                            }
                            else
                            {
                                APPL_TRC
                                (
                                    "Len: 0x%02X,  Metadata: NULL\n",
                                    bc_basic_announcement_1.meta.length
                                );
                            }
                            prev_metadata_len += appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[sg_info_index];

                            bc_source_config.data += parsed_len;

                            /* Initializing to 0 before setting the required bit */
                            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_bis_sync[sg_info_index] = 0U;

                            /* Initializing number of bis to 0 for each subgroup */
                            num_bis = 0U;

                            while (num_bis < n_bis)
                            {
                                parsed_len = 0U;
                                GA_mem_set
                                (
                                    &bc_basic_announcement_2,
                                    0x00,
                                    sizeof(BAP_BASIC_ANNOUNCEMENT_LEVEL_2)
                                );

                                if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_2
                                                  (
                                                      bc_source_config.data,
                                                      &bc_basic_announcement_2,
                                                      &parsed_len
                                                  ))
                                {
                                    num_bis++;
                                    APPL_TRC
                                    (
                                        "Len: 0x%02X,  BIS_index: 0x%02X\n",
                                        (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream_index
                                    );
                                    /*
                                     * BIS_Sync  : 4-octet bitfield. Bit 0-30 = BIS_index[1-31]
                                     * 0x00000000: 0b0 = Do not synchronize to BIS_index[x]
                                     * 0xxxxxxxxx: 0b1 = Synchronize to BIS_index[x]
                                     * 0xFFFFFFFF: No Preference
                                     */
                                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_bis_sync[sg_info_index] |=
                                        (0x00000001U << (bc_basic_announcement_2.stream_index - 1U));

                                    APPL_TRC
                                    (
                                        "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
                                        (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream.length
                                    );

                                    APPL_TRC
                                    (
                                        "Len: 0x%02X,  Codec_Specific_Configuration:\n",
                                        bc_basic_announcement_2.stream.length
                                    );

                                    appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
                                    (
                                        bc_basic_announcement_2.stream.value,
                                        bc_basic_announcement_2.stream.length,
                                        &rem_cs_conf
                                    );

                                    bc_source_config.data += parsed_len;
                                }
                            }
                            /* Increment the SubGroups Parsed */
                            sg_info_index++;
                        }
                    }
                    /*
                     * By default, give BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL.
                     * This will be taken as input during Add Source Control
                     * Point
                     */
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.pa_sync =
                        BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL;
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.num_subgroups = sg_info_index;
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.bis_sync =
                        &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_bis_sync[0U];
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.metadata_len =
                        &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[0U];
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.metadata =
                        &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata[0U];
                }
            }
        }
        break;

    case GA_BC_SOURCE_STREAM_IND:
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_SOURCE_STREAM_IND,
                              ga_data,
                              &bc_source_stream
                          ))
        {
            if (appl_ga_ba_params.stream_synchandle ==
                appl_ga_ba_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }

            GA_CB_ENTRY("BC CAP Server");
            appl_ga_ba_params.stream_synchandle =
                appl_ga_ba_params.bcast_synchandle;

            appl_ga_utils_bc_display_sink_console_header(device, ga_status);
            APPL_TRC
            (
                "[Event]       : GA_BC_SOURCE_STREAM_IND (0x%02X)\n",
                ga_event
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();

            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.sync_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Num BIS: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.num_bis
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.iso_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  SDU Interval: 0x%08X\n",
                (UCHAR)sizeof(UINT32), bc_source_stream.sdu_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max SDU: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.max_sdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Phy: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Framing: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.framing
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Encryption: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.encryption
            );
        }
        break;

    case GA_BC_DISSOCIATE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISSOCIATE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_DISSOCIATE_CNF,
                              ga_data,
                              &bc_dissociate
                          ))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_dissociate.sync_handle
            );

            appl_ga_ba_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_ba_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_ba_params.stream_synchandle = BC_INVALID_HANDLE;
        }
        break;

    case GA_BC_DISSOCIATE_IND:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISSOCIATE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_DISSOCIATE_IND,
                              ga_data,
                              &bc_dissociate
                          ))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_dissociate.sync_handle
            );

            appl_ga_ba_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_ba_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_ba_params.stream_synchandle = BC_INVALID_HANDLE;
        }
        break;

    default:
        APPL_ERR("Unknown Event ID: 0x%02X !\n", ga_event);
        break;
    }

    /* GA_CB_EXIT(); */

    APPL_DBG("[APPL][GA][BC_BA]: << appl_ga_bc_ba_bc_sink_handler\n");

    return retval;
}

GA_RESULT appl_ga_bc_src_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          )
{
    GA_RESULT retval;

    UINT8 i;
    UINT8 sg_info_index;
    UINT8 bis_info_index;
    UINT8 qos_conf_set_id;
    BAP_BC_START_PARAMS bc_start = { 0 };
    BAP_BC_SUSPEND_PARAMS bc_suspend = { 0 };

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    sg_info_index = 0U;
    bis_info_index = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_ga_bc_src_handler\n");

    switch (ga_event)
    {
    case GA_BC_SETUP_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SETUP_ANNOUNCEMENT_CNF (0x%02X)\n",
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

    case GA_BC_END_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_END_ANNOUNCEMENT_CNF (0x%02X)\n",
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

    case GA_BC_START_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_START_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_START_CNF, ga_data, &bc_start))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Error Code: 0x%02X\n",
                (UCHAR)sizeof(bc_start.error_code),
                bc_start.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_start.big_handle),
                bc_start.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Sync Delay: 0x%08X\n",
                (UCHAR)sizeof(bc_start.big_sync_delay),
                bc_start.big_sync_delay
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Transport Latency BIG: 0x%08X\n",
                (UCHAR)sizeof(bc_start.transport_latency_big),
                bc_start.transport_latency_big
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Phy: 0x%02X\n",
                (UCHAR)sizeof(bc_start.phy), bc_start.phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(bc_start.nse), bc_start.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(bc_start.bn), bc_start.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(bc_start.pto), bc_start.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(bc_start.irc), bc_start.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(bc_start.max_pdu), bc_start.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(bc_start.iso_interval), bc_start.iso_interval
            );

            APPL_TRC
            (
                "Len: 0x%02X,  Number of BISes: 0x%02X\n",
                (UCHAR)sizeof(bc_start.num_bis), bc_start.num_bis
            );

            if (0x00U == bc_start.error_code)
            {
                /*
                 * TODO: Extract the BIS from the BIG handle if we support
                 * multiple BIG Sessions.
                 */
                appl_bc_src_params.big_handle = bc_start.big_handle;
                /*
                 * During Start API, we might have triggered for 'n' number of BIS
                 * to sync to, However based on the controller, we might have got
                 * BIS Connection Handles for all 'n' or less than 'n'.
                 * Use them as in sequence received.
                 *
                 * Eg: BIS Index: 1, 2, 3, BIS Count: 3
                 *     BIS Conn Handle Count: 2, BIS Conn Handles: 1, 2
                 *     BIS Index: 1, BIS Conn Handle: 1
                 *     BIS Index: 2, BIS Conn Handle: 2
                 */
                for (i = 0U; i < bc_start.num_bis; i++)
                {
                    APPL_TRC("BIS[%d]:\n", (i+1U));
                    APPL_TRC
                    (
                        "\tLen: 0x%02X,  BIS Conn Handle: 0x%04X\n",
                        (UCHAR)sizeof(bc_start.connection_handle[i]),
                        bc_start.connection_handle[i]
                    );

                    /* Copy BIS connection handle in the BASE structure */
                    if ((appl_bc_src_params.bc_num_subgroups > sg_info_index) &&
                        (appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream > bis_info_index))
                    {
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                            bc_start.connection_handle[i];
                        /* Move to the Next BIS Index */
                        bis_info_index++;

                        /*
                         * Check if current BIS has reached total no of BIS in
                         * the current SG.
                         * If Yes, Reset the value of bis_info_index for next
                         * iteration and sg_info_index to the next SG.
                         * If No, set the bis_info_index to the next BIS.
                         */
                        if (appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream == bis_info_index)
                        {
                            bis_info_index = 0U;
                            sg_info_index++;
                        }
                        else
                        {
                            /* Continue on the same SG for rest of the BISes */
                        }
                    }
                }

                if (0U < bc_start.num_bis)
                {
#ifdef APPL_END_ANNOUNCEMENT_ON_STREAMING
                    /* Broadcast release: BIG is created, Now disable Advertising */
                    APPL_TRC("GA_broadcast_end_announcement\n");
                    retval = GA_broadcast_end_announcement
                             (
                                 appl_bcast_ssn
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
#endif /* APPL_END_ANNOUNCEMENT_ON_STREAMING */

                    /* Fetch the first Valid BIS Index */
                    if (GA_SUCCESS ==
                        appl_ga_bc_src_fetch_first_valid_sg_bis_info_index
                        (
                            &sg_info_index,
                            &bis_info_index
                        ))
                    {
                        if (GA_FAILURE == appl_ga_bc_ba_src_setup_iso
                                          (
                                              sg_info_index,
                                              bis_info_index
                                          ))
                        {
                            /* First Setup ISO itself failed, break */
                            APPL_TRC("Setup ISO Failed !");
                        }
                        else
                        {
                            /* Setup ISO is Ongoing, Wait */
                        }
                    }
                }
            }
        }
        break;

    case GA_BC_SUSPEND_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SUSPEND_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_SUSPEND_CNF, ga_data, &bc_suspend))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.big_handle), bc_suspend.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Reason: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.reason), bc_suspend.reason
            );

            /*
             * TODO: If we support more than 1 BIG, fetch the right BIG Session
             * from the BIG handle received.
             */

            /*
             * As BIG is already terminated when this event is generated,
             * Removal of ISO is already taken care before calling the Suspend
             * API. Hence Just call appl_ga_bc_ba_stop_transmitting_audio() to
             * Delete LC3 Encoder Module and Stop Audio PL Generator Module.
             */
            qos_conf_set_id = appl_bc_src_params.bc_subgroup_info[0U].qos_conf_set_id;
            for (sg_info_index = 0U; sg_info_index < appl_bc_src_params.bc_num_subgroups; sg_info_index++)
            {
                for (bis_info_index = 0U; bis_info_index < appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
                {
                    if (AUDIO_ISO_IDLE !=
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state)
                    {
                        /* If any ISO entry is not removed yet, remove here. */
                        if (GA_SUCCESS == appl_ga_utils_audio_src_remove_entry
                        (
                            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                        ))
                        {
                            APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
                            /* Update the ISO state */
                            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                                AUDIO_ISO_IDLE;
                        }
                        else
                        {
                            APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed or Already Removed!\n",
                                BA_GET_ROLE_STRING(bc_role));
                        }
                    }
                    /* We are Audio Source, Stop Transmitting Audio data */
                    if (GA_SUCCESS == appl_ga_bc_ba_stop_transmitting_audio
                                      (
                                          appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          qos_conf_set_id,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                        /* Reset the BIS Handle to Invalid */
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                            BC_INVALID_HANDLE;
                        /*
                         * appl_ga_bc_ba_stop_transmitting_audio
                         * Delete LC3 encoder module and Setup and
                         * Stop Audio PL module here. Since for
                         * broadcast we have multiple BISes in BIG
                         * and all move to streaming at once,
                         * appl_ga_bc_ba_stop_transmitting_audio
                         * is called with qos_conf_set_id as 0xFF
                         * for rest of the BISes to indicate update of
                         * ISO and Streaming state only.
                         */
                        qos_conf_set_id = 0xFFU;
                    }
                    else
                    {
                        APPL_DBG
                        (
                            "[APPL][GA][%s]: Failed to delete LC3 Encoder or"
                            "start Audio_pl\n", BA_GET_ROLE_STRING(bc_role)
                        );
                        if (0xFFU != qos_conf_set_id)
                        {
                            /*
                             * The LC3 delete and Audio PL Stop and
                             * is called only once. Hence if it
                             * fails for the first time, there is no
                             * point in updating of ISO and Streaming
                             * state for rest.
                             */
                            retval = GA_FAILURE;
                            break;
                        }
                    }
                }
                if (GA_FAILURE == retval)
                {
                    break;
                }
            }
            /* Reset the BIG Handle to 0xFF */
            appl_bc_src_params.big_handle = 0xFFU;
        }
        break;

    case GA_BC_SUSPEND_IND:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SUSPEND_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_SUSPEND_IND, ga_data, &bc_suspend))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.big_handle), bc_suspend.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Reason: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.reason), bc_suspend.reason
            );

            /*
             * TODO: If we support more than 1 BIG, fetch the right BIG Session
             * from the BIG handle received.
             */

            /*
             * As BIG is already terminated when this event is generated,
             * ISO data path can't be removed now since the Controller will not
             * have reference to BIS Handles as BIG itself is removed.
             * Hence call appl_ga_utils_audio_src_remove_entry() to remove ISO
             * entry and appl_ga_bc_ba_stop_transmitting_audio() to
             * Delete LC3 Encoder Module and Stop Audio PL Generator Module.
             */
            qos_conf_set_id = appl_bc_src_params.bc_subgroup_info[0U].qos_conf_set_id;
            for (sg_info_index = 0U; sg_info_index < appl_bc_src_params.bc_num_subgroups; sg_info_index++)
            {
                for (bis_info_index = 0U; bis_info_index < appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_src_remove_entry
                                      (
                                          appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                                      ))
                    {
                        APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
                        /* Update the ISO state */
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                            AUDIO_ISO_IDLE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n", BA_GET_ROLE_STRING(bc_role));
                    }
                    /* We are Audio Source, Stop Transmitting Audio data */
                    if (GA_SUCCESS == appl_ga_bc_ba_stop_transmitting_audio
                                      (
                                          appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          qos_conf_set_id,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                        /* Reset the BIS Handle to Invalid */
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                            BC_INVALID_HANDLE;
                        /*
                         * appl_ga_bc_ba_stop_transmitting_audio
                         * Delete LC3 encoder module and Setup and
                         * Stop Audio PL module here. Since for
                         * broadcast we have multiple BISes in BIG
                         * and all move to streaming at once,
                         * appl_ga_bc_ba_stop_transmitting_audio
                         * is called with qos_conf_set_id as 0xFF
                         * for rest of the BISes to indicate update of
                         * ISO and Streaming state only.
                         */
                        qos_conf_set_id = 0xFFU;
                    }
                    else
                    {
                        APPL_DBG
                        (
                            "[APPL][GA][%s]: Failed to delete LC3 Encoder or"
                            "start Audio_pl\n", BA_GET_ROLE_STRING(bc_role)
                        );
                        if (0xFFU != qos_conf_set_id)
                        {
                            /*
                             * The LC3 delete and Audio PL Stop and
                             * is called only once. Hence if it
                             * fails for the first time, there is no
                             * point in updating of ISO and Streaming
                             * state for rest.
                             */
                            retval = GA_FAILURE;
                            break;
                        }
                    }
                    if (GA_FAILURE == retval)
                    {
                        break;
                    }
                }
            }
            /* Reset the BIG Handle to 0xFF */
            appl_bc_src_params.big_handle = 0xFFU;
        }
        break;

    default:
        APPL_ERR("Received UnHandled Event !\n");
        break;
    }

    GA_CB_EXIT();

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_src_handler\n");
    return retval;
}

void appl_ga_bc_ba_snk_init(void)
{
    appl_ga_ba_params.bcast_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_ba_params.config_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_ba_params.stream_synchandle =
        BC_INVALID_HANDLE;

    return;
}

void appl_ga_bc_ba_src_init(void)
{
    UINT8  sg_info_index;
    UINT8  bis_info_index;

    sg_info_index = 0U;
    bis_info_index = 0U;

    GA_mem_set
    (
        &appl_bc_src_params,
        0x00U,
        sizeof(BC_SRC_PARAMS)
    );
    appl_bc_src_params.big_handle = 0xFFU;

    for (sg_info_index = 0U; sg_info_index < APPL_BC_MAX_NUM_OF_SUB_GROUPS; sg_info_index++)
    {
        for (bis_info_index = 0U; bis_info_index < APPL_BC_MAX_BIS_PER_SUB_GROUP; bis_info_index++)
        {
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                BC_INVALID_HANDLE;
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_IDLE;
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                GA_FALSE;
        }
    }
    appl_ga_bc_src_suspend_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void main_ba_operations(void)
{
    int        choice;
    UINT8      menu_choice;
    GA_RESULT  retval;
    GA_ENDPOINT dev;
    UINT8 sid;
    UINT8 index;
    UINT8 j;
    GA_CODEC_IE cs_conf_ltv[APPL_BC_MAX_BIS_PER_SUB_GROUP];
    GA_ENDPOINT device;
    UCHAR * bc_code_ptr;
    UINT8 sg_info_index;
    UINT8 bis_info_index;

    /* Initialize */
    menu_choice = 0U;
    index = 0U;
    j = 0U;
    retval = GA_SUCCESS;
    bc_code_ptr = NULL;
    sg_info_index = 0U;
    bis_info_index = 0U;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", bc_ba_src_menu_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = (UINT8)choice;

        switch(menu_choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                if (BC_ROLE_SD == bc_role)
                {
                    APPL_TRC("A role is already assigned for the device. "
                    "Please deinit and try again\n");
                }
                else
                {
                    APPL_TRC("GA_bass_ce_init\n");
                    retval = GA_bass_ce_init(appl_ga_bc_bass_ba_cb);
                    if (GA_SUCCESS == retval)
                    {
                        /* Assign the role as BA after initialization */
                        bc_role |= BC_ROLE_BA;
                        /* Setting of default Scan parameters */
                        appl_bc_set_default_scan_settings();
                        /* Initialize BC_Sink parameters required for BA */
                        appl_ga_bc_ba_snk_init();
                        appl_ga_bc_ba_init();
                    }
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                break;

            case 3:
                if (BC_ROLE_NONE == bc_role)
                {
                    APPL_TRC("No role has been assigned for the device. "
                    "Please init and try again\n");
                }
                else
                {
                    APPL_TRC("GA_bass_ce_shutdown\n");
                    retval = GA_bass_ce_shutdown();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        /* Assign the role as NONE after initialization */
                        bc_role = BC_ROLE_NONE;
                    }
                }
                break;

            case 10:
                retval = appl_bc_scan_for_sd();
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 11:
                if (APPL_GA_SCAN_FOR_SD == is_scan_enabled)
                {
                    retval = appl_bc_stop_scan_for_sd();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                else
                {
                    /* TODO */
                }
                break;

            case 12:
                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                device.bd_type = (UINT8)choice;
                retval = appl_ga_utils_add_conn_device_as_a_initiator(device);
                if (GA_SUCCESS == retval)
                {
                    appl_ga_utils_connect
                    (
                        device,
                        GA_CONNECTION_MODE_EXTENDED
                    );
                    if (GA_SUCCESS == appl_bc_bass_ba_is_sd_not_added_in_db
                                      (
                                          device
                                      ))
                    {
                        /* Fetch an empty entry in the DB */
                        if (GA_SUCCESS == appl_bc_bass_ba_fetch_entry_in_db(&index))
                        {
                            appl_bc_rem_sd_in_use = index;
                            /*
                             * Free SD Index is found. Get the connection
                             * device index from the device to update in the DB
                             */
                            if (GA_SUCCESS == appl_ga_utils_get_conn_device_index(device, &index))
                            {
                                appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index =
                                    index;
                            }
                            else
                            {
                                /* Reset the conn_dev_index */
                                appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index =
                                    INVALID_CONN_DEV_INDEX;
                            }
                        }
                    }
                }
                else
                {
                    APPL_ERR("Device already registered !\n");
                }
                break;

            case 13:
                appl_bc_bass_ba_set_rem_sd_in_use();
                break;

            case 20:
                /* Fetch device from conn_dev_index */
                appl_ga_utils_get_conn_device
                (
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                    &device
                );
                APPL_TRC("GA_bass_ce_setup_context\n");
                retval = GA_bass_ce_setup_context
                        (
                            &device,
                            &appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle
                        );

                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS != retval)
                {
                    /* Reset the BASS Handle, if it is a Failure */
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle =
                        0xFFU;
                }
                else
                {
                    APPL_TRC
                    (
                        "BASS Handle: 0x%04X\n",
                        appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle
                    );
                }
                break;

            case 21:
                APPL_TRC("appl_bc_bass_ba_read_bc_rx_state: \n");
                appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_bass_rx_state_in_use = 0U;
                for (index = 0U; index < appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_bass_rx_state_count; index++)
                {
                    GA_mem_set
                    (
                        &appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[index].bc_rx_state,
                        0U,
                        sizeof(GA_BASS_BC_RX_STATE)
                    );
                }
                retval = appl_bc_bass_ba_read_bc_rx_state(appl_bc_rem_sd_in_use);

                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS != retval)
                {
                    APPL_ERR("Failed to read Broadcast Receive State !\n");
                }
                break;

            case 22:
                CONSOLE_OUT("Input index of the source: ");
                CONSOLE_IN("%d", &choice);
                index = (UINT8)choice;
                APPL_TRC("Add the Broadcast Source...\n");
                APPL_TRC("appl_bc_bass_ba_add_bc_src...\n");
                retval = appl_bc_bass_ba_add_bc_src(index);
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 23:
                APPL_TRC("Modify Source CP Operation\n");
                CONSOLE_OUT("Enter the Source_ID of the source to modify source\n");
                CONSOLE_IN("%d", &choice);
                sid = (UINT8)choice;
                APPL_TRC("appl_bc_bass_ba_modify_bc_src...\n");
                retval = appl_bc_bass_ba_modify_bc_src(sid);
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 24:
                APPL_TRC("Set Broadcast_Code CP Operation\n");
                CONSOLE_OUT("Enter the Source_ID of the source to send Broadcast_Code\n");
                CONSOLE_IN("%d", &choice);
                sid = (UINT8)choice;
                retval = appl_bc_ba_bass_set_bc_code(sid);
                break;

            case 25:
                APPL_TRC("Remove Source CP Operation\n");
                CONSOLE_OUT("Enter the Source_ID of the source to be removed\n");
                CONSOLE_IN("%d", &choice);
                sid = (UINT8)choice;
                retval = appl_bc_ba_bass_rem_bc_src(sid);
                break;

            case 26:
            {
                CONSOLE_OUT("\n1. Release\n2. Close\n");
                CONSOLE_OUT("Enter your choice: ");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    APPL_TRC("GA_bass_ce_release\n");
                    retval = GA_bass_ce_release
                            (
                                appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle
                            );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                else if (2 == choice)
                {
                    APPL_TRC("GA_bass_ce_close\n");
                    retval = GA_bass_ce_close
                            (
                                appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle
                            );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
            }
                break;

            case 30:
                if (BC_ROLE_NONE == bc_role)
                {
                    APPL_ERR("No role is assigned for the device !\n");
                    break;
                }
                else if (IS_APPL_GA_BC_ROLE_BC_SRC(bc_role))
                {
                    APPL_ERR("Invalid Role for operation. Current role is"
                    "only Broadcast Source !\n");
                    break;
                }
                if (APPL_GA_SCAN_INVALID == is_scan_enabled)
                {
                    APPL_TRC("GA_broadcast_scan_announcement\n");
                    retval = GA_broadcast_scan_announcement();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        is_scan_enabled = APPL_GA_SCAN_FOR_BC_SRC;
                        appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status
                            = GA_TRUE;
                        /* Reset the DB when scan starts */
                        appl_bc_bass_ba_clear_bc_src_db();

                        APPL_TRC("appl_bc_bass_ba_inform_scan_status...\n");
                        retval = appl_bc_bass_ba_inform_scan_status();
                        APPL_TRC("Retval - ");
                        appl_ga_bc_ba_bass_display_ga_result(retval);
                    }
                }
                break;

            case 31:
                if (BC_ROLE_NONE == bc_role)
                {
                    APPL_ERR("No role is assigned for the device !\n");
                    break;
                }
                else if (IS_APPL_GA_BC_ROLE_BC_SRC(bc_role))
                {
                    APPL_ERR("Invalid Role for operation. Current role is"
                    "only Broadcast Source !\n");
                    break;
                }
                APPL_TRC("Stop Scan for Broadcast Source...\n");
                if ((APPL_GA_SCAN_FOR_BC_SRC == is_scan_enabled) &&
                    (GA_TRUE == appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status))
                {
                    APPL_TRC("GA_broadcast_scan_end\n");
                    retval = GA_broadcast_scan_end();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                if (GA_SUCCESS == retval)
                {
                    is_scan_enabled = APPL_GA_SCAN_INVALID;
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status
                        = GA_FALSE;

                    APPL_TRC("appl_bc_bass_ba_inform_scan_status...\n");
                    retval = appl_bc_bass_ba_inform_scan_status();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                break;

            case 32:
                if (BC_ROLE_NONE == bc_role)
                {
                    APPL_ERR("No role is assigned for the device !\n");
                    break;
                }
                else if (IS_APPL_GA_BC_ROLE_BC_SRC(bc_role))
                {
                    APPL_ERR("Invalid Role for operation. Current role is"
                    "only Broadcast Source !\n");
                    break;
                }
                APPL_TRC("Associate to the Broadcast Source...\n");
                CONSOLE_OUT("Enter SID: ");
                CONSOLE_IN("%d", &choice);
                sid = (UINT8)choice;

                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(dev.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                dev.bd_type = (UINT8)choice;

                APPL_TRC("GA_broadcast_associate\n");
                retval = GA_broadcast_associate
                         (
                             sid,
                             &dev
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 33:
                APPL_TRC("appl_bc_ba_send_pa_sync_info...\n");
                retval = appl_bc_ba_send_pa_sync_info();
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 34:
                if (BC_ROLE_NONE == bc_role)
                {
                    APPL_ERR("No role is assigned for the device !\n");
                    break;
                }
                else if (IS_APPL_GA_BC_ROLE_BC_SRC(bc_role))
                {
                    APPL_ERR("Invalid Role for operation. Current role is"
                    "only Broadcast Source !\n");
                    break;
                }
                APPL_TRC("Dissociate from the Broadcast Source...\n");
                APPL_TRC("GA_broadcast_dissociate\n");
                retval = GA_broadcast_dissociate
                         (
                             appl_ga_ba_params.bcast_synchandle
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 35:
                appl_bc_ba_add_local_bc_src_to_src_db();
                break;

            case 36:
                APPL_TRC("appl_bc_ba_send_set_info_transfer...\n");
                retval = appl_bc_ba_send_set_info_transfer();
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 40:
                /* Set Broadcast settings */
                if (APPL_GA_ROLE_NONE == ga_role)
                {
                    appl_ga_bc_set_src_setting(APPL_GA_ROLE_NONE);
                }
                APPL_TRC ("GA_broadcast_alloc_session\n");
                retval = GA_broadcast_alloc_session
                         (
                             &appl_bc_src_params.bc_ssn
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    if (BC_ROLE_NONE == bc_role)
                    {
                        /* Assign the role as BC_src if the current role is NONE */
                        bc_role = BC_ROLE_BC_SRC;
                    }
                    else if (BC_ROLE_BA == bc_role)
                    {
                        /* Assign the role as BA_with_BC_src if the current role is BA */
                        bc_role |= BC_ROLE_BC_SRC;
                    }
                }
                break;

            case 41:
                APPL_TRC ("GA_broadcast_free_session\n");
                retval = GA_broadcast_free_session
                         (
                             appl_bc_src_params.bc_ssn
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    bc_role = BC_ROLE_NONE;
                }
                break;

            case 42:
                if (GA_TRUE == appl_bc_src_params.bc_encryption)
                {
                    bc_code_ptr = &appl_bc_src_params.bc_code[0U];
                }
                else
                {
                    /*
                     * By Default bc_code is initialize to zero.
                     * bc_code_ptr will be passed as NULL in this case.
                     */
                    bc_code_ptr = NULL;
                }
                APPL_TRC ("GA_broadcast_configure_session\n");
                retval = GA_broadcast_configure_session
                         (
                             appl_bc_src_params.bc_ssn,
                             appl_bc_src_params.bc_qos_conf.phy,
                             appl_bc_src_params.bc_packing,
                             bc_code_ptr
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 43:
                /*
                 * Each BIS will have its own CS Conf, Create a 1D array of CS
                 * Conf LTVs of all BISes.
                 */
                for (index = 0U; index < appl_bc_src_params.bc_num_subgroups; index++)
                {
                    APPL_TRC ("GA_broadcast_register_sep\n");
                    for (j = 0U; j < appl_bc_src_params.bc_subgroup_info[index].bc_nstream; j++)
                    {
                        GA_mem_copy
                        (
                            &cs_conf_ltv[j],
                            &appl_bc_src_params.bc_subgroup_info[index].bis_info[j].cs_conf_ltv,
                            sizeof(GA_CODEC_IE)
                        );
                    }
                    retval = GA_broadcast_register_sep
                             (
                                 appl_bc_src_params.bc_ssn,
                                 &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec,
                                 &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta,
                                 appl_bc_src_params.bc_subgroup_info[index].bc_nstream,
                                 &cs_conf_ltv[0U],
                                 &appl_bc_src_params.bc_subgroup_info[index].bc_sep
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                break;

            case 44:
            {
                UCHAR type;
#ifdef GA_PBP
                UINT16 appearance_val;
                /*
                 * adata: Max of 32B + 1B + 1B for Name.
                 *        Fixed: 2B + 1B + 1B for Appearance.
                 */
                UCHAR sdata[32], adata[38];
                UINT16 sdatalen, adatalen, alen;

                alen = 0U;
                sdatalen = 0U;
                adatalen = 0U;

                /* Form the service data */
                if (APPL_BC_SRC_PBP_AUDIO_CONF_TYPE_SQ == appl_bc_src_pbp_audio_conf_type)
                {
                    /*
                     * As per PBP Spec, A PBS should include the Program_Info
                     * length-type-value (LTV) structure metadata (defined in
                     * Bluetooth Assigned Numbers [2]) to help users determine
                     * which broadcast Audio Stream to select when populating
                     * the BASE
                     */
                    PBP_ENCODE_BCAST_ADV_SERVICE_DATA_WITH_METADATA
                    (
                        sdata,
                        sdatalen,
                        appl_bc_src_params.bc_encryption,
                        PBP_BCAST_SQ_ON,
                        PBP_BCAST_HQ_OFF,
                        appl_bc_src_params.bc_subgroup_info[0U].bc_basic_announcement_1.meta.length,
                        &appl_bc_src_params.bc_subgroup_info[0U].bc_basic_announcement_1.meta.data
                    );
                }
                else if (APPL_BC_SRC_PBP_AUDIO_CONF_TYPE_HQ == appl_bc_src_pbp_audio_conf_type)
                {
                    /*
                     * As per PBP Spec, A PBS should include the Program_Info
                     * length-type-value (LTV) structure metadata (defined in
                     * Bluetooth Assigned Numbers [2]) to help users determine
                     * which broadcast Audio Stream to select when populating
                     * the BASE
                     */
                    PBP_ENCODE_BCAST_ADV_SERVICE_DATA_WITH_METADATA
                    (
                        sdata,
                        sdatalen,
                        appl_bc_src_params.bc_encryption,
                        PBP_BCAST_SQ_OFF,
                        PBP_BCAST_HQ_ON,
                        appl_bc_src_params.bc_subgroup_info[0U].bc_basic_announcement_1.meta.length,
                        &appl_bc_src_params.bc_subgroup_info[0U].bc_basic_announcement_1.meta.data
                    );
                }

                /*
                 * Form the additional Adv data - Name
                 * As per Spec, Length: Min 4, Max 32.
                 */
                PBP_ENCODE_BCAST_ADV_BROADCAST_NAME(&adata[0U], alen, "Gate-1", 6U);
                adatalen = alen;

                appl_ga_bc_set_appearance_value(&appearance_val);

                /*
                 * Form the additional Adv data - Appearance
                 * Category Sub-category
                 * 10 bits     6 bits
                 * (bits 15-6) (bits 5-0)
                 * 0x022: Audio Source
                 * 0x00: Generic
                 * 0x0880: Generic Audio Source
                 */
                PBP_ENCODE_BCAST_ADV_APPEARANCE(&adata[adatalen], alen, appearance_val);
                adatalen += alen;

                appl_bc_src_params.advset_handle = 1U;

                APPL_TRC("Registering Advset to Session...\n");
                retval = GA_broadcast_register_advset_info
                         (
                             appl_bc_src_params.bc_ssn,
                             1U,
                             sdata,
                             sdatalen,
                             adata,
                             adatalen
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);

                type = GA_BCAST_SIG_PROFILE_PUBLIC_BROADCAST;
#else /* GA_PBP */
                APPL_TRC("GA_broadcast_register_advset\n");
                retval = GA_broadcast_register_advset
                         (
                             appl_bc_src_params.bc_ssn,
                             1U
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);

                type = GA_BCAST_SIG_PROFILE_NONE;
#endif /* GA_PBP */

                APPL_TRC ("GA_broadcast_setup_announcement\n");
                retval = GA_broadcast_setup_announcement
                         (
                             appl_bc_src_params.bc_ssn,
                             appl_bc_src_params.bc_id,
                             0x00,
                             appl_bc_src_params.bc_qos_conf.presentation_delay
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
            }
                break;

            case 45:
                APPL_TRC ("GA_broadcast_end_announcement\n");
                retval = GA_broadcast_end_announcement
                         (
                             appl_bc_src_params.bc_ssn
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                break;

            case 46:
                /*
                 * Start is done for the complete BIG, Just validate the CC of
                 * this SG, BIS is supported by LC3 module.
                 * Since we have not moved any BIS to streaming yet, the only
                 * condition that gets validated will be the CC.
                 */
                for (sg_info_index = 0U; sg_info_index < appl_bc_src_params.bc_num_subgroups; sg_info_index++)
                {
                    for (bis_info_index = 0U; bis_info_index < appl_bc_src_params.bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
                    {
                        /*
                         * As per BAP spec Sampling Frequency, Frame Duration
                         * and Octets per Codec Frame field is specified as
                         * mandated and will be common per SG.
                         * Hence copy the cs conf of SG into combined cs conf
                         * of SG and BIS.
                         */
                        appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis =
                                appl_bc_src_params.bc_subgroup_info[sg_info_index].cs_conf;
                        if (GA_AUDIO_LOCATION_RFU_1 == appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf.aca)
                        {
                            /*
                             * This indicates that the Num of Bis in this SG is
                             * 1 and the BAP Spec specifies either the SG or
                             * the BIS can have the ACA.
                             * Hence, Now fetch the ACA from the SG.
                             */
                        }
                        else
                        {
                            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis.aca =
                                appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf.aca;
                        }

                        if (GA_SUCCESS != appl_ga_utils_audio_src_audio_lc3_enc_allowed
                                          (
                                              appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis
                                          ))
                        {
                            retval = GA_FAILURE;
                            break;
                        }
                        else
                        {
                            retval = GA_SUCCESS;
                        }
                    }
                    if (GA_FAILURE == retval)
                    {
                        break;
                    }
                }

                if (GA_SUCCESS == retval)
                {
                    APPL_TRC ("GA_broadcast_start\n");
                    retval = GA_broadcast_start
                             (
                                 appl_bc_src_params.bc_ssn,
                                 appl_bc_src_params.bc_qos_conf.sdu_interval,
                                 appl_bc_src_params.bc_qos_conf.max_sdu,
                                 appl_bc_src_params.bc_qos_conf.max_transport_latency,
                                 appl_bc_src_params.bc_qos_conf.retxn,
                                 appl_bc_src_params.bc_qos_conf.framing
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_ba_bass_display_ga_result(retval);
                }
                break;

            case 47:
                /*
                 * Chain removal of ISO Data Paths of all BISes before
                 * triggering Suspend since on suspend API, the reference to
                 * BISes Handles are already removed by the Controller and
                 * trigger to Removal of ISO Fails.
                 */
                /* Fetch the first Valid BIS Index */
                if (GA_SUCCESS ==
                    appl_ga_bc_src_fetch_first_valid_sg_bis_info_index
                    (
                        &sg_info_index,
                        &bis_info_index
                    ))
                {
                    if (GA_FAILURE == appl_ga_bc_ba_src_remove_iso
                                      (
                                          sg_info_index,
                                          bis_info_index
                                      ))
                    {
                        /* First Removal of ISO itself failed, break */
                        APPL_TRC("Removal of ISO Failed !");
                    }
                    else
                    {

                        /* Removal of ISO is Ongoing, Wait */
                    }
                }

                /*
                 * Here, once the Removal of ISO for all BISes in all SGs are
                 * complete, Trigger Suspend API to termiante the BIG.
                 * To achieve this, Use a timer.
                 * On confirmation of BIG Terminate (GA_BC_SUSPEND_CNF),
                 * trigger call appl_ga_bc_ba_stop_transmitting_audio() to
                 * remove LC3 Encoder and Audio PL Generator
                 * Module.
                 */
                BT_start_timer
                (
                    &appl_ga_bc_src_suspend_timer_handle,
                    3U,
                    appl_ga_bc_src_suspend_timer_handler,
                    NULL,
                    0U
                );
                break;

            case 80:
            {
#ifdef BASS_SUPPORT_CONTEXT_MANAGE
                /* Fetch device from conn_dev_index */
                appl_ga_utils_get_conn_device
                (
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                    &device
                );

                appl_bc_bass_handle_access =
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle;

                APPL_TRC("GA_bass_ce_get_context_info ...\n");
                retval = GA_bass_ce_get_context_info
                         (
                             &device,
                             &appl_bc_bass_handle_access,
                             &appl_bc_bass_range_ptr,
                             &appl_bc_bass_char_info_ptr,
                             &appl_bc_bass_char_count
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("appl_bc_bass_char_count:0x%02x\n", appl_bc_bass_char_count);

                    appl_bc_bass_range = *appl_bc_bass_range_ptr;

                    /**
                     *  Reference to allocate to save locally.
                     *  This can be written to persistent and retrieved
                     *  upon the next connection if bonded device
                     */
                    appl_bc_bass_char_info = GA_alloc_mem(appl_bc_bass_char_count * sizeof(GA_CHAR_INFO));
                    if (NULL != appl_bc_bass_char_info)
                    {
                        GA_mem_copy
                        (
                            appl_bc_bass_char_info,
                            appl_bc_bass_char_info_ptr,
                            appl_bc_bass_char_count * sizeof(GA_CHAR_INFO)
                        );
                    }
                }
#else /* BASS_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("BASS_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BASS_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 81:
            {
#ifdef BASS_SUPPORT_CONTEXT_MANAGE
                /* Fetch device from conn_dev_index */
                appl_ga_utils_get_conn_device
                (
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                    &device
                );

                if (NULL == appl_bc_bass_char_info)
                {
                    APPL_ERR("appl_bc_bass_char_info is NULL !\n");
                    APPL_TRC("Call GA_bass_ce_get_context_info \n");
                    break;
                }

                appl_bc_bass_char_info_ptr = appl_bc_bass_char_info;
                appl_bc_bass_range_ptr = &appl_bc_bass_range;

                APPL_TRC("GA_bass_ce_set_context_info ...\n");
                retval = GA_bass_ce_set_context_info
                         (
                             &device,
                             &appl_bc_bass_handle_access,
                             &appl_bc_bass_range_ptr,
                             &appl_bc_bass_char_info_ptr,
                             &appl_bc_bass_char_count
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle =
                        appl_bc_bass_handle_access;

                    /* Free the allocated memory */
                    GA_free_mem(appl_bc_bass_char_info);
                    appl_bc_bass_char_info = NULL;
                }
#else /* BASS_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("BASS_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BASS_SUPPORT_CONTEXT_MANAGE */
            }
            break;

            case 87:
#ifdef BASS_SUPPORT_CONTEXT_MANAGE
                /**
                 * This is to free up the allocated char. info. memory
                 * If incase GA_bass_ce_set_context_info() is NOT called after
                 * GA_bass_ce_get_context_info().
                 */

                /* Free the allocated memory for TMAS char. info. */
                if (NULL != appl_bc_bass_char_info)
                {
                    APPL_TRC("Freeing appl_bc_bass_char_info ...\n");
                    GA_free_mem(appl_bc_bass_char_info);
                    appl_bc_bass_char_info = NULL;
                }
#else /* BASS_SUPPORT_CONTEXT_MANAGE */
                APPL_ERR("BASS_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* BASS_SUPPORT_CONTEXT_MANAGE */
                break;

            case 90:
#ifdef BASS_SUPPORT_CONFIG_SELECTION
                APPL_TRC("GA_bass_ce_update_ntf_configuration...\n");
                retval = GA_bass_ce_update_ntf_configuration
                         (
                            0U/* BASS_CHAR_CONFIG(BASS_CHAR_ID_RX_STATE) */
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_ba_bass_display_ga_result(retval);
#else /* BASS_SUPPORT_CONFIG_SELECTION */
                CONSOLE_OUT("BASS_SUPPORT_CONFIG_SELECTION NOT defined\n");
#endif /* BASS_SUPPORT_CONFIG_SELECTION */
                break;

            default:
                APPL_TRC("Invalid Choice\n");
        }
        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

GA_RESULT appl_ga_bc_ba_handle_rx_state_data
          (
              GA_ENDPOINT  device,
              UINT8      * rx_state_evt_data,
              UINT8      * sd_index,
              UINT8      * rx_state_db_index
          )
{
    GA_RESULT  retval;
    UINT8      marker;

    retval = GA_FAILURE;
    marker = 0U;

    if ((NULL == sd_index) && (NULL == rx_state_db_index))
    {
        return retval;
    }

    if (GA_SUCCESS == appl_ga_bc_ba_get_rem_sd_db_index(device, sd_index))
    {
        /*
         * Check if this rx_state is already present in the DB, by passing
         * sd_index and source_id
         */
        if (GA_SUCCESS == appl_ga_bc_ba_get_bc_rx_state_db_index
                          (
                              rx_state_evt_data[marker],
                              *sd_index,
                              rx_state_db_index
                          ))
        {
            /* If the rx_state is already present, then update its data */
            marker++;
            appl_ga_bc_ba_update_rx_state_data
            (
                *sd_index,
                *rx_state_db_index,
                &rx_state_evt_data[marker]
            );
            retval = GA_SUCCESS;
        }
        /*
         * If the rx_state is not present in DB, then add it to DB by fetching
         * free index
         */
        else if (GA_SUCCESS == appl_ga_bc_ba_fetch_free_bc_rx_state_index
                               (
                                   *sd_index,
                                   rx_state_db_index
                               ))
        {
            /*
             * If a free rx_state_db_index is present, then copy the data.
             */
            appl_bc_bass_rem_sd_db[*sd_index].bc_rx_state_db[*rx_state_db_index].bc_rx_state.info.id =
                rx_state_evt_data[marker];
            marker++;
            appl_ga_bc_ba_update_rx_state_data
            (
                *sd_index,
                *rx_state_db_index,
                &rx_state_evt_data[marker]
            );
            retval = GA_SUCCESS;
        }
        else
        {
            /*
             * The rx_state is not present in DB and also, there is no free
             * index. Inform the user.
             */
            APPL_ERR("No free index preent for rx_state !\n");
        }
    }
    else
    {
        APPL_ERR("SD not present in DB !\n");
    }
    return retval;
}

GA_RESULT appl_ga_bc_ba_get_bc_rx_state_db_index
          (
              UINT8   source_id,
              UINT8   sd_index,
              UINT8 * rx_state_db_index
          )
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    for (index = 0U; index < BASS_CE_MAX_BC_RX_STATE_INSTANCES; index++)
    {
        if (source_id ==
            appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[index].bc_rx_state.info.id)
        {
            *rx_state_db_index = index;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

GA_RESULT appl_ga_bc_ba_fetch_free_bc_rx_state_index
          (
              UINT8   sd_index,
              UINT8 * rx_state_db_index
          )
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    for (index = 0U; index < BASS_CE_MAX_BC_RX_STATE_INSTANCES; index++)
    {
        /* Check if the Source_ID of the rx_state is invalid */
        if (APPL_BASS_INVALID_SRC_ID ==
            appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[index].bc_rx_state.info.id)
        {
            *rx_state_db_index = index;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

void appl_ga_bc_ba_update_rx_state_data
     (
         UINT8   sd_index,
         UINT8   rx_state_db_index,
         UINT8 * rx_state_evt_data
     )
{
    UINT8      marker;
    UINT8      sg_info_index;

    /* Initialize */
    marker = 0U;
    sg_info_index = 0U;

    /* Update Source_Address_Type */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.addr_type =
        rx_state_evt_data[marker];
    marker++;
    /* Update Source_Address */
    GA_COPY_BRR_DEV_ADDR
    (
        appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.addr,
        &rx_state_evt_data[marker]
    );
    marker += GA_BD_ADDR_SIZE;
    /* Update Source_Adv_SID */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.adv_sid =
        rx_state_evt_data[marker];
    marker++;
    /* Update Broadcast_ID */
    GA_mem_copy
    (
        appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.broadcast_ID,
        &rx_state_evt_data[marker],
        BASS_BC_ID_LEN
    );
    marker += BASS_BC_ID_LEN;
    /* Update PA_Sync_State */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.pa_sync_state =
        rx_state_evt_data[marker];
        marker++;
    /* Update BIG_Encryption */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.big_encryption =
        rx_state_evt_data[marker];
        marker++;
    /* Check for Bad_Code */
    if (BASS_BIG_ENCRYPTION_BAD_CODE == rx_state_evt_data[marker])
    {
        /* Update Bad_Code */
        GA_mem_copy
        (
            &appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.bad_code[0U],
            &rx_state_evt_data[marker],
            BASS_BC_CODE_MAX_LEN
        );
        marker += BASS_BC_CODE_MAX_LEN;
    }
    /* Update Num_Subgroups */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.num_subgroups =
        rx_state_evt_data[marker];
    marker++;
    /* Give reference to BIS_Sync State */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.bis_sync_state =
        &appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].appl_bc_bass_bc_src_bis_sync[0U];

    /* Give reference to Metadata_Length */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata_len =
        &appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].appl_bc_bass_bc_src_metadata_len[0U];

    /* Give reference to Metadata pointers */
    appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata =
        &appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].appl_bc_bass_bc_src_metadata[0U];

    for (sg_info_index = 0U; sg_info_index < appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.num_subgroups; sg_info_index++)
    {
        /* Update BIS_Sync State */
        GA_mem_copy
        (
            appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.bis_sync_state,
            &rx_state_evt_data[marker],
            sizeof(UINT32)
        );
        marker += 4U;

        /* Update Metadata_Length */
        *(appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata_len) =
            rx_state_evt_data[marker];
        marker++;

        /* Update Metadata */
        GA_mem_copy
        (
            appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata,
            &rx_state_evt_data[marker],
            *(appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata_len)
        );
        marker = marker + (*(appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.metadata_len));
    }

    return;
}

GA_RESULT appl_ga_bc_ba_get_rem_sd_db_index(GA_ENDPOINT device, UINT8 * sd_index)
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      conn_dev_index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;
    conn_dev_index = 0U;

    /* Get conn_dev_index from the device */
    if (GA_SUCCESS == appl_ga_utils_get_conn_device_index
                      (
                          device,
                          &conn_dev_index
                      ))
    {
        /* Check if the conn_dev_index is present in SD_DB */
        for (index = 0U; index < APPL_BC_BA_MAX_SD_DEVICES; index++)
        {
            if (appl_bc_bass_rem_sd_db[index].conn_dev_index == conn_dev_index)
            {
                *sd_index = index;
                retval = GA_SUCCESS;
                break;
            }
        }
    }

    return retval;
}

void appl_bc_set_default_scan_settings(void)
{
    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_set_default_scan_settings\n");

    /* Initialize default broadcast scan parameters */
    appl_bc_ba_escan_params.duration = 0x00;
    appl_bc_ba_escan_params.filter_duplicates = 0x01;
    appl_bc_ba_escan_params.own_addr_type = 0x00;
    appl_bc_ba_escan_params.period = 0x00;
    appl_bc_ba_escan_params.scan_filter_policy = 0x00;
    appl_bc_ba_escan_params.scan_interval = 0x00A0;
    appl_bc_ba_escan_params.scan_phy = 0x01;
    appl_bc_ba_escan_params.scan_type = 0x00;
    appl_bc_ba_escan_params.scan_window = 0x00A0;

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_set_default_scan_settings\n");
    return;
}

GA_RESULT appl_bc_scan_for_sd(void)
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_scan_for_sd\n");

    retval = appl_ga_utils_enable_extended_scan(appl_bc_ba_escan_params);
    if (GA_SUCCESS == retval)
    {
        is_scan_enabled = APPL_GA_SCAN_FOR_SD;
    }
    else
    {
        APPL_ERR("Failed to set Extended Scan Params Or Scan Already initiated"
                 " by Other Module !\n");
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_scan_for_sd\n");
    return retval;
}

GA_RESULT appl_bc_stop_scan_for_sd(void)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_stop_scan_for_sd\n");

    retval = appl_ga_utils_disable_extended_scan(appl_bc_ba_escan_params);
    if (GA_SUCCESS == retval)
    {
        is_scan_enabled = APPL_GA_SCAN_INVALID;
    }
    else
    {
        APPL_ERR("Failed to Disable Extended Scan Or Scan Already Stopped"
            " by Other Module !\n");
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_stop_scan_for_sd\n");
    return retval;
}

GA_RESULT appl_bc_bass_ba_read_bc_rx_state(UINT8 sd_index)
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_read_bc_rx_state\n");

    retval = GA_bass_ce_read_bc_rx_state
             (
                 appl_bc_bass_rem_sd_db[sd_index].bass_ba_handle,
                 appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_inst[appl_bc_bass_rem_sd_db[sd_index].bc_bass_rx_state_in_use]
             );
    if (GA_SUCCESS == retval)
    {
        appl_bc_bass_rem_sd_db[sd_index].bc_bass_read_rx_state_chaining = GA_TRUE;
        APPL_TRC("Broadcast Receive State char value Read is Successful\n");
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_read_bc_rx_state\n");
    return retval;
}

GA_RESULT appl_bc_bass_ba_add_bc_src(UINT8 index)
{
    /* By default uses Write Request */
    GA_BASS_CE_WRITE_REQ req = { 0 };
    GA_RESULT retval;
    UINT8 write_pdu[100U];
    UINT8 marker;
    UINT8 size;
    UCHAR i;
    UINT8 prev_metadata_len;
    int   choice;

    retval = GA_FAILURE;
    marker = 0U;
    size = 0U;
    i = 0U;
    prev_metadata_len = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_add_bc_src\n");

    CONSOLE_OUT("1. Do not synchronize to PA\n"
                "2. Synchronize to PA - PAST available\n"
                "3. Synchronize to PA - PAST not available\n");
    CONSOLE_IN("%d", &choice);
    if (1U == (UINT8)choice)
    {
    APPL_TRC("Do not synchronize to PA\n");
    appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
        BASS_PA_SYNC_DO_NOT_SYNC_TO_PA;
    }
    else if (2U == (UINT8)choice)
    {
        APPL_TRC("Synchronize to PA - PAST available\n");
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
            BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL;
    }
    else if (3U == (UINT8)choice)
    {
        APPL_TRC("Synchronize to PA - PAST not available\n");
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
            BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL;
    }
    else
    {
        APPL_ERR("Invalid choice !\n");
        return retval;
    }

    /* First Byte as opcode */
    write_pdu[marker] = BASS_CP_OPCODE_ADD_SOURCE;
    marker++;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr_type;
    /* Increment marker by 1B for adv_addr_type */
    marker++;
    size++;

    GA_COPY_BRR_DEV_ADDR
    (
        &write_pdu[marker],
        &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr[0U]
    );
    /* Increment marker by 6B for adv_addr */
    marker += 6U;
    size += 6U;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_SID;
    /* Increment marker by 1B for adv_SID */
    marker++;
    size++;

    GA_mem_copy
    (
        &write_pdu[marker],
        &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.broadcast_ID[0U],
        BASS_BC_ID_LEN
    );
    /* Increment marker by 3B for adv_SID */
    marker += 3U;
    size += 3U;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync;
    /* Increment marker by 1B for pa_sync */
    marker++;
    size++;

    GA_mem_copy
    (
        &write_pdu[marker],
        &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_interval,
        2U
    );
    /* Increment marker by 2B for pa_interval */
    marker += 2U;
    size += 2U;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups;
    /* Increment marker by 1B for num_subgroups */
    marker++;
    size++;

    for (i = 0U; i < appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups; i++)
    {
        GA_mem_copy
        (
            &write_pdu[marker],
            &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.bis_sync[i],
            4U
        );
        /* Increment marker by 1B for bis_sync */
        marker += 4U;
        size += 4U;

        write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
        /* Increment marker by 1B for metadata_len */
        marker++;
        size++;

        GA_mem_copy
        (
            &write_pdu[marker],
            &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata[prev_metadata_len],
            appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i]
        );
        /* Increment marker by metadata_len for metadata */
        marker += appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
        size += appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
        prev_metadata_len += appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
    }

    /* Copy the address to the structure data */
    appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src_length = size;

    /* Update the req_data.val from write_pdu, to be sent */
    req.req_data.val = &write_pdu[0U];
    /*
     * Total data length will be 1B for opcode and remaining for CP
     * operation data
     */
    req.req_data.len = 1U + appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src_length;
    /* Write request */
    req.write_type = 0U;

    APPL_TRC("GA_bass_ce_request_to_add_source\n");
    retval = GA_bass_ce_request_to_add_source
             (
                 appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle,
                 &req
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_add_bc_src\n");
    return retval;
}

GA_RESULT appl_bc_bass_ba_modify_bc_src(UINT8 source_id)
{
    /* By default uses Write Request */
    GA_BASS_CE_WRITE_REQ req = { 0 };
    UINT8 write_pdu[100U];
    int choice;
    UINT8 is_req;
    UINT8 marker;
    UINT8 size;
    UCHAR i;
    UINT8 index;
    UINT8 prev_metadata_len;
    GA_RESULT retval;

    /* Initialize */
    is_req = 0U;
    marker = 0U;
    size = 0U;
    i = 0U;
    index = 0U;
    prev_metadata_len = 0U;
    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_modify_bc_src\n");

    if (GA_SUCCESS != appl_ga_bc_ba_get_bc_rx_state_db_index
                      (
                          source_id,
                          appl_bc_rem_sd_in_use,
                          &index
                      ))
    {
        APPL_ERR("Invalid Source_ID !\n");
        return retval;
    }

    CONSOLE_OUT
    (
        "Update Synchronize/Stop Synchronization to PA ?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
    is_req = (UINT8)choice;
    if (GA_TRUE == is_req)
    {
        appl_ga_bc_ba_get_input_for_modify_src_pa(index);
    }

    CONSOLE_OUT
    (
        "Update Synchronize/Stop Synchronization to BIS(s) ?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
    is_req = (UINT8)choice;
    if (GA_TRUE == is_req)
    {
        appl_ga_bc_ba_get_input_for_modify_src_bis(index);
    }

    CONSOLE_OUT
    (
        "Modify Metadata ?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
    is_req = (UINT8)choice;
    if (GA_TRUE == is_req)
    {
        appl_ga_bc_ba_get_input_for_modify_src_metadata(index);
    }

    /* First Byte as opcode */
    write_pdu[marker] = BASS_CP_OPCODE_MODIFY_SOURCE;
    marker++;

    write_pdu[marker] = appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[index].bc_rx_state.info.id;
    /* Increment marker by 1B for id */
    marker++;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync;
    /* Increment marker by 1B for pa_sync */
    marker++;

    GA_mem_copy
    (
        &write_pdu[marker],
        &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_interval,
        2U
    );
    /* Increment marker by 2B for pa_interval */
    marker += 2U;

    write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups;
    /* Increment marker by 1B for num_subgroups */
    marker++;

    for (i = 0U; i < appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups; i++)
    {
        GA_mem_copy
        (
            &write_pdu[marker],
            &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.bis_sync[i],
            4U
        );
        /* Increment marker by 1B for bis_sync */
        marker += 4U;

        write_pdu[marker] = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
        /* Increment marker by 1B for metadata_len */
        marker++;

        GA_mem_copy
        (
            &write_pdu[marker],
            &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata[prev_metadata_len],
            appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i]
        );
        /* Increment marker by metadata_len for metadata */
        marker += appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
        prev_metadata_len += appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i];
    }

    /* Update the req_data.val from write_pdu, to be sent */
    req.req_data.val = &write_pdu[0U];
    /*
     * Total data length will be 1B for opcode and remaining for CP
     * operation data.
     */
    req.req_data.len = marker;
    /* Write request */
    req.write_type = 0U;

    APPL_TRC("GA_bass_ce_request_to_modify_source\n");
    retval = GA_bass_ce_request_to_modify_source
             (
                 appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle,
                 &req
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_modify_bc_src\n");
    return retval;
}

GA_RESULT appl_bc_ba_bass_set_bc_code(UINT8 source_id)
{
    GA_RESULT retval;
    GA_BASS_CE_WRITE_REQ req = { 0 };
    /* Opcode(1B) + Source_ID(1B) + BC Code(16B) */
    UINT8 write_pdu[18U];
    UINT8 marker;
    UINT8 i;
    int choice;
    UINT8 wt_req_type;

    retval = GA_FAILURE;
    i = 0U;
    marker = 0U;
    wt_req_type = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_bass_set_bc_code\n");

    write_pdu[marker++] = BASS_CP_OPCODE_SET_BC_CODE;
    write_pdu[marker++] = source_id;
    /*
     * TODO: Validate if we already have the broadcast code via OOB.
     * If yes, broadcast code will be user input here.
     */
    CONSOLE_OUT("Enter Broadcast_Code [IN HEX]: ");
    for (i = 0; i < BASS_BC_CODE_MAX_LEN; i++)
    {
        CONSOLE_IN("%x", &choice);
        write_pdu[marker++] = (UCHAR)choice;
    }

    CONSOLE_OUT("Select the below options\n");
    CONSOLE_OUT("0 -> Write Request\n");
    CONSOLE_OUT("1 -> Write Command\n");
    CONSOLE_IN("%d", &choice);
    wt_req_type = (UINT8)choice;

    /* Update the req_data.val from write_pdu, to be sent */
    req.req_data.val = &write_pdu[0U];
    req.req_data.len = marker;
    req.write_type = wt_req_type;

    APPL_TRC("GA_bass_ce_set_bc_code\n");
    retval = GA_bass_ce_set_bc_code
             (
                 appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle,
                 &req
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_bass_set_bc_code\n");
    return retval;
}

GA_RESULT appl_bc_ba_bass_rem_bc_src(UINT8 source_id)
{
    GA_RESULT retval;
    GA_BASS_CE_WRITE_REQ req = { 0 };
    /* Opcode(1B) + Source_ID(1B) */
    UINT8 write_pdu[2U];
    UINT8 marker;
    int choice;
    UINT8 wt_req_type;

    retval = GA_FAILURE;
    marker = 0U;
    wt_req_type = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_bass_rem_bc_src\n");

    write_pdu[marker++] = BASS_CP_OPCODE_REMOVE_SOURCE;
    write_pdu[marker++] = source_id;

    CONSOLE_OUT("Select the below options\n");
    CONSOLE_OUT("0 -> Write Request\n");
    CONSOLE_OUT("1 -> Write Command\n");
    CONSOLE_IN("%d", &choice);
    wt_req_type = (UINT8)choice;

    /* Update the req_data.val from write_pdu, to be sent */
    req.req_data.val = &write_pdu[0U];
    req.req_data.len = marker;
    req.write_type = wt_req_type;

    APPL_TRC("GA_bass_ce_request_to_remove_source\n");
    retval = GA_bass_ce_request_to_remove_source
             (
                 appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle,
                 &req
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_bass_rem_bc_src\n");
    return retval;
}

void appl_ga_bc_ba_get_input_for_modify_src_pa(UINT8 index)
{
    int choice;

    /*
     * PA_Sync Value:
     * 0x00 = Do not synchronize to PA
     * 0x01 = Synchronize to PA - PAST available
     * 0x02 = Synchronize to PA - PAST not available
     * All other values: RFU
     */
    CONSOLE_OUT("1. Do not synchronize to PA\n"
                "2. Synchronize to PA - PAST available\n"
                "3. Synchronize to PA - PAST not available\n");
    CONSOLE_OUT("Enter your choice: ");
    CONSOLE_IN("%d", &choice);

    /*
     * If Sync to PA is chosen, Fetch the Input for PAST Status
     * (Available or not available).
     */
    if (1U == (UINT8)choice)
    {
        APPL_TRC("Do not synchronize to PA\n");
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
            BASS_PA_SYNC_DO_NOT_SYNC_TO_PA;
    }
    if (2U == (UINT8)choice)
    {
        APPL_TRC("Synchronize to PA - PAST available\n");
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
            BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL;
    }
    else if (3U == (UINT8)choice)
    {
        APPL_TRC("Synchronize to PA - PAST not available\n");
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.pa_sync =
            BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL;
    }
    else
    {
        APPL_ERR("Invalid choice !\n");
    }

    return;
}

void appl_ga_bc_ba_get_input_for_modify_src_bis(UINT8 index)
{
    int    choice;
    UINT32 bis_sync;
    UCHAR  i;
    UCHAR  j;
    UINT8  bis_info_index;
    UINT8  num_bis;

    /* Initialize */
    i = 0U;
    j = 0U;
    bis_info_index = 0U;
    num_bis = 0U;

    for (i = 0U; i < appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups; i++)
    {
        bis_sync = appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.bis_sync[i];

        APPL_TRC
        (
            "Subgroup[%d], BIS: 0x%04X\n",
            i,
            appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.bis_sync[i]
        );

        /*
         * BIS_Sync parameter for the [ith] subgroup in the BIG
         * 4-octet bitfield. Bit 0-30 = BIS_index[1-31]
         * 0x00000000: 0b0 = Do not synchronize to BIS_index[x]
         * 0xxxxxxxxx: 0b1 = Synchronize to BIS_index[x]
         * 0xFFFFFFFF: No preference
         * Shall not exist if Num_Subgroups = 0
         */

        /* Take the number of BIS(s) to synchronize to. */
        CONSOLE_OUT("Enter the number of BIS(s) to sync: ");
        CONSOLE_IN("%d", &choice);
        num_bis = (UINT8)choice;

        /* Take input for the BIS index(s) to synchronize to. */
        for (j = 0U; j < num_bis; j++)
        {
            CONSOLE_OUT("Enter the BIS index: ");
            CONSOLE_IN("%d", &choice);
            bis_info_index = (UINT8)choice;
            BASS_BIS_SYNC_STATE_UPDATE_SYNC_TO_BIS_INDEX(bis_sync, bis_info_index);
        }

        /* Take the number of BIS(s) to stop synchronization. */
        CONSOLE_OUT("Enter the number of BIS(s) to stop sync: ");
        CONSOLE_IN("%d", &choice);
        num_bis = (UINT8)choice;

        /* Take input for the BIS index(s) to stop synchronization. */
        for (j = 0U; j < num_bis; j++)
        {
            CONSOLE_OUT("Enter the BIS index: ");
            CONSOLE_IN("%d", &choice);
            bis_info_index = (UINT8)choice;
            BASS_BIS_SYNC_STATE_UPDATE_NOT_SYNC_TO_BIS_INDEX(bis_sync, bis_info_index);
        }
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.bis_sync[i] = bis_sync;
    }

    return;
}
void appl_ga_bc_ba_get_input_for_modify_src_metadata(UINT8 index)
{
    UCHAR i;
    UINT8 orig_metadata_copy[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN] = { 0U };
    /* This is the total length of the Metadata of all SGs */
    UINT8 total_metadata_parsed_len;
    GA_METADATA new_metadata;

    /* Initialize */
    i = 0U;
    total_metadata_parsed_len = 0U;

    /*
     * Steps followed
     * - Copy all the metadata contents of all the subgroup of the source into
     *   a orig_metadata_copy.
     *
     * - orig_metadata_copy will have all the original contents of all the subgroups.
     *   This is used when no action is required for a particular metadata_type.
     *
     * - For each subgroup, for each metadata type, ask the user if it needs to
     *   to removed, updated or no action required, and take the input/copy data
     *   into new_metadata.
     *     - If remove is selected, then do not copy the contents of that
     *       metadata_type into the new_metadata.
     *     - If update is selected, then take fresh input for that metadata
     *       type, and copy the contents to new_metadata.
     *     - If no action is required, then copy the original contents into
     *       new_metadata.
     *
     *
     * E.g., Metadata LTV: | 1 -  0x03, 0x01, 0x04, 0x00 |
     *                     | 2 - 0x02, 0x01, 0x01 |
     *                     | 3 - 0x06, 0x03, 0x01, 0x02, 0x03, 0x04, 0x05 |
     *
     * 1 - No action required, 2 - Remove, 3 - Update
     *
     * New Metadata LTV: | 1 -  0x03, 0x01, 0x04, 0x00 |
     *                   | 3 - 0x03, 0x03, 0x01, 0x02 |
     *
     *
     * - After copying all the contents into new_metadata, copy it to the
     *   global metadata array, update the metadata_length and proceed for
     *   the next subgroup.
     */

    /* Copy all metadata contents of all subgroups into a backup buffer. */
    GA_mem_copy
    (
        &orig_metadata_copy[0U],
        &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src_metadata[0U],
        (APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN)
    );

    /* For each subgroup. */
    for (i = 0U; i < appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.num_subgroups; i++)
    {
        /* Display subgroup metadata. */
        APPL_TRC("Subgroup[%d], Metadata: ", i);
        if (0U == appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i])
        {
            APPL_TRC("Empty\n");
        }
        else
        {
            APPL_TRC("\n");
            appl_ga_utils_bap_parse_and_display_metadata_ltv
            (
                &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata[total_metadata_parsed_len],
                appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i],
                NULL,
                NULL
            );
        }

        /* Reset the metadata input variable. */
        GA_mem_set
        (
            &new_metadata.data[0U],
            0U,
            GA_METADATA_LEN
        );
        new_metadata.length = 0U;

        /* Take input to modify the metadata. */
        appl_ga_bc_ba_parse_and_get_input_for_metadata
        (
            &orig_metadata_copy[0U],
            appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i],
            &new_metadata.data[0U],
            &new_metadata.length
        );

        /* Copy the contents from new_metadata to the metadata. */
        GA_mem_copy
        (
            &appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata[total_metadata_parsed_len],
            &new_metadata.data[0U],
            new_metadata.length
        );
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.metadata_len[i] = new_metadata.length;
        total_metadata_parsed_len += new_metadata.length;
    }

    return;
}

void appl_ga_bc_ba_parse_and_get_input_for_metadata
     (
         /* IN */    UINT8 * metadata,
         /* IN */    UINT8   metadata_len,
         /* INOUT */ UINT8 * new_metadata,
         /* INOUT */ UINT8 * new_metadata_len
     )
{
    int choice;
    UINT8 marker;
    UINT8 new_metadata_marker;
    UINT8 len;
    UINT8 type;
    UINT16 value_2b_input;
    UINT32 value_4b_input;
    UINT8 data[15U];
    UINT8 datalen;
    UCHAR i;

    /* Initialize */
    marker = 0U;
    new_metadata_marker = 0U;
    len = 0U;
    type = 0U;
    value_2b_input = 0U;
    datalen = 0U;
    i = 0U;

    /* TODO: Optimize */

    while (marker < metadata_len)
    {
        GA_mem_set(data, 0U, 15U);
        datalen = 0U;

        /* Extract the Length */
        len = metadata[marker];
        CONSOLE_OUT("Metadata Length: 0x%02X\n", len);
        marker++;

        /* Extract the Type */
        type = metadata[marker];
        CONSOLE_OUT("Metadata Type: 0x%02X\n", type);
        marker++;

        switch (type)
        {
            case GA_LTV_METADATA_PREF_AC_TYPE:
                CONSOLE_OUT("Preferred_Audio_Contexts\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */
                }
                else if ((2 == choice) || (3 == choice))
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;

                    if (2 == choice)
                    {
                        /* Get the new input for this metadata type */
                        value_2b_input = appl_ga_utils_bap_get_supp_ctx();
                    }
                    else
                    {
                        GA_UNPACK_LE_2_BYTE
                        (
                            &value_2b_input,
                            &metadata[new_metadata_marker]
                        );
                    }
                    GA_PACK_LE_2_BYTE_VAL
                    (
                        &new_metadata[new_metadata_marker],
                        value_2b_input
                    );
                    /* Update the Length of the Value */
                    new_metadata_marker += (GA_LTV_METADATA_PREF_AC_LEN - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                    /* It wont be copied */
                }
                /* Increment the marker to go to next LTV */
                marker += (GA_LTV_METADATA_PREF_AC_LEN - 1U);
                break;

            case GA_LTV_METADATA_STREAMING_AC_TYPE:
                CONSOLE_OUT("Streaming_Audio_Contexts\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */
                }
                else if ((2 == choice) || (3 == choice))
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;

                    if (2 == choice)
                    {
                        /* Get the new input for this metadata type */
                        value_2b_input = appl_ga_utils_bap_get_streaming_audio_ctx();
                    }
                    else
                    {
                        GA_UNPACK_LE_2_BYTE
                        (
                            &value_2b_input,
                            &metadata[new_metadata_marker]
                        );
                    }
                    GA_PACK_LE_2_BYTE_VAL
                    (
                        &new_metadata[new_metadata_marker],
                        value_2b_input
                    );
                    /* Update the Length of the Value */
                    new_metadata_marker += (GA_LTV_METADATA_PREF_AC_LEN - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                    /* It wont be copied */
                }
                /* Increment the marker to go to next LTV */
                marker += (GA_LTV_METADATA_STREAMING_AC_LEN - 1U);
                break;

            case GA_LTV_METADATA_PGM_INFO_TYPE:
                CONSOLE_OUT("Program_Info\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */
                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;

                    /* Get the new input for this metadata type */
                    appl_ga_utils_bap_get_program_info(data, &datalen);
                    /* Update length for this LTV */
                    new_metadata[new_metadata_marker - 2U] = datalen + 1U;
                    /* Copy the new metadata value to new_metadata */
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &data[0U],
                        datalen
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += datalen;
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the metadata_input_data.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (len - 1U)
                    );
                    /* Update the input_data_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_LANGUAGE_TYPE:
                CONSOLE_OUT("Language\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    choice = appl_ga_utils_bap_get_language_code();
                    appl_ga_utils_set_language_code(&value_4b_input, (UINT8 )choice);
                    GA_PACK_LE_3_BYTE_VAL
                    (
                        &new_metadata[new_metadata_marker],
                        value_4b_input
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (GA_LTV_METADATA_LANGUAGE_LEN - 1U);
                    /* Update the marker */
                    marker += (GA_LTV_METADATA_LANGUAGE_LEN - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (GA_LTV_METADATA_LANGUAGE_LEN - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_CCID_LIST_TYPE:
                CONSOLE_OUT("CCID_List\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    datalen = appl_ga_utils_bap_get_ccid_count();
                    /* Update the new length for this metadata_type. */
                    new_metadata[new_metadata_marker - 2U] = datalen + 1U;
                    for (i = 0U; i < datalen; i++)
                    {
                        /* Copy new data and also update the new_metadata_marker */
                        new_metadata[new_metadata_marker++] =
                            appl_ga_utils_bap_get_ccid();
                    }
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (len - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_PARENTAL_RATING_TYPE:
                CONSOLE_OUT("Parental_Rating\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    value_2b_input = appl_ga_utils_bap_get_parental_rating_type();
                    GA_PACK_LE_2_BYTE_VAL
                    (
                        &new_metadata[new_metadata_marker],
                        value_2b_input
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (GA_LTV_METADATA_PARENTAL_RATING_LEN - 1U);
                    /* Update the marker */
                    marker += (GA_LTV_METADATA_PARENTAL_RATING_LEN - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (GA_LTV_METADATA_PARENTAL_RATING_LEN - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_PROGRAM_INFO_URL_TYPE:
                CONSOLE_OUT("Program_Info_URI\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    appl_ga_utils_bap_get_program_info_url(data, &datalen);
                    /* Update length for this LTV */
                    new_metadata[new_metadata_marker - 2U] = datalen + 1U;
                    /* Copy the new metadata value to new_metadata */
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &data[0U],
                        datalen
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += datalen;
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (len - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_EM_TYPE:
                CONSOLE_OUT("Extended_Metadata\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    appl_ga_utils_bap_get_extended_metadata(data, &datalen);
                    /* Update length for this LTV */
                    new_metadata[new_metadata_marker - 2U] = datalen + 1U;
                    /* Copy the new metadata value to new_metadata */
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &data[0U],
                        datalen
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += datalen;
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (len - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            case GA_LTV_METADATA_VS_TYPE:
                CONSOLE_OUT("Vendor_Specific\n");
                CONSOLE_OUT("1. Remove\n2. Update\n3. No action\n");
                CONSOLE_IN("%d", &choice);
                if (1 == choice)
                {
                    /* Do not copy the metadata LTV */

                }
                else if (2 == choice)
                {
                    /*
                     * Update Length and Type, take new input and copy the
                     * contents to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    /* Get the new input for this metadata type */
                    appl_ga_utils_bap_get_vendor_specific(data, &datalen);
                    /* Update length for this LTV */
                    new_metadata[new_metadata_marker - 2U] = datalen + 1U;
                    /* Copy the new metadata value to new_metadata */
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &data[0U],
                        datalen
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += datalen;
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else if (3 == choice)
                {
                    /*
                     * Update Length and Type, copy the original contents of this
                     * type to the new_metadata.
                     */
                    new_metadata[new_metadata_marker] = len;
                    new_metadata_marker++;
                    new_metadata[new_metadata_marker] = type;
                    new_metadata_marker++;
                    GA_mem_copy
                    (
                        &new_metadata[new_metadata_marker],
                        &metadata[marker],
                        (len - 1U)
                    );
                    /* Update the new_metadata_marker */
                    new_metadata_marker += (len - 1U);
                    /* Update the marker */
                    marker += (len - 1U);
                }
                else
                {
                    APPL_ERR("Invalid choice !\n");
                }
                break;

            default:
                APPL_ERR("Unhandled Case !\n");
                /*
                 * Update Length and Type, copy the original contents of this
                 * type to the input variable
                 */
                new_metadata[new_metadata_marker] = len;
                new_metadata_marker++;
                new_metadata[new_metadata_marker] = type;
                new_metadata_marker++;
                GA_mem_copy
                (
                    &new_metadata[new_metadata_marker],
                    &metadata[marker],
                    (len - 1U)
                );
                /* Update the new_metadata_marker */
                new_metadata_marker += (len - 1U);
                /* Update the marker */
                marker += (len - 1U);
                break;
        }

    }
    *new_metadata_len = new_metadata_marker;

    return;
}

GA_RESULT appl_bc_ba_send_pa_sync_info(void)
{
    GA_RESULT retval;
    UINT16 svc_data;
    UINT16 acl_handle;

    /* Initialize */
    retval = GA_FAILURE;
    acl_handle = 0U;
    svc_data = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_ba_send_pa_sync_info\n");

    /*
     * The HCI_LE_Periodic_Advertising_Sync_Transfer command is used to
     * instruct the Controller to send synchronization information about the periodic
     * advertising train identified by the Sync_Handle parameter to a connected
     * device.
     */
    /* TODO: This needs to be updated based on AdvA */

    /*
     * Bit 0: AdvA in PAST matches AdvA in ADV_EXT_IND
     * 0b0 = Yes, 0b1 = No/Dont know
     * Bit 1: AdvA in PAST matches Source_Address
     * 0b0 = Yes, 0b1 = No/Dont know
     */
    svc_data = 0x03;

    /* TODO: Use the Source_ID received from Server on BR Rx State */

    /* Source_ID */
    svc_data |= 0x00;

    /* Fetch ACL Connection Handle from conn_dev_index */
    if (GA_SUCCESS == appl_ga_utils_get_conn_handle_from_index
                      (
                          appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                          &acl_handle
                      ))
    {
        retval = BT_hci_le_periodic_advertising_sync_transfer
                 (
                     acl_handle,
                     svc_data,
                     appl_ga_ba_params.bcast_synchandle /* TODO: What if there are multiple */
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC("FAILED !!! Error code = %04X\n", retval);
        }
        else
        {
            APPL_TRC("API returned success...\n");
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_send_pa_sync_info\n");
    return retval;
}

GA_RESULT appl_bc_ba_send_set_info_transfer(void)
{
    GA_RESULT retval;
    UINT16 svc_data;
    UINT16 acl_handle;
    UINT8 i;
    int choice;

    /* Initialize */
    retval = GA_FAILURE;
    acl_handle = 0U;
    svc_data = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_ba_send_set_info_transfer\n");

    /* Fetch ACL Connection Handle from conn_dev_index */
    if (GA_SUCCESS == appl_ga_utils_get_conn_handle_from_index
                      (
                          appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                          &acl_handle
                      ))
    {
        /* Take input for Local Source Index added in DB */
        CONSOLE_OUT("Input index of the source: ");
        CONSOLE_IN("%d", &choice);
        i = (UINT8)choice;

        retval = BT_hci_le_periodic_advertising_set_info_transfer
                 (
                     acl_handle,
                     svc_data,
                     appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.adv_SID
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC("FAILED !!! Error code = %04X\n", retval);
        }
        else
        {
            APPL_TRC("API returned success...\n");
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_ba_send_set_info_transfer\n");
    return retval;
}

void appl_bc_ba_add_local_bc_src_to_src_db(void)
{
    UINT8 i, j, k, index;
    BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS bc_source_announcement = { 0 };
    BAP_BROADCAST_ANNOUNCEMENT bc_annoucement = { 0 };
    UINT8 num_of_streams;
    UINT8 addr[GA_BD_ADDR_SIZE];

    j = 0U;
    /* BIS_Index */
    k = 1;

    /*
     * TODO: From App, if we have set Random addr, Save it here.
     */
    if(GA_SUCCESS != BT_hci_get_local_bd_addr(&addr[0U]))
    {
        return;
    }

    bc_source_announcement.address_type = BASS_ADDR_TYPE_PUBLIC;
    bc_source_announcement.advertising_sid = appl_bc_src_params.advset_handle;
    bc_source_announcement.address = &addr[0U];

    if (GA_SUCCESS != appl_bc_bass_ba_is_bc_src_entry_present_in_db
                      (
                          bc_source_announcement.address,
                          bc_source_announcement.address_type,
                          &i
                      ))
    {
        i = 0U;
        /* This is a new entry, Add this BC Src */
        if (GA_SUCCESS == appl_bc_bass_ba_fetch_free_bc_src_index_in_db(&i))
        {
            APPL_TRC("Source added in Index: %d", i);

            /* There is a free index, Add it here */
            appl_bc_bass_ba_add_bc_src_info_into_db
            (
                i,
                bc_source_announcement,
                bc_annoucement
            );

            /* This value is ideally set in ga_brr_pl.c, Using the same */
            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.pa_interval =
                0x0320;

            GA_PACK_LE_3_BYTE_VAL
            (
                &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.broadcast_ID[0U],
                appl_bc_src_params.bc_id
            );

            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.num_subgroups =
                appl_bc_src_params.bc_num_subgroups;

            for (index = 0u; index < appl_bc_src_params.bc_num_subgroups; index++)
            {
                appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[index] =
                    appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length;

                GA_mem_copy
                (
                    &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata[j],
                    &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.data[0U],
                    appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
                );

                j += appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length;

                num_of_streams = 0U;
                while (num_of_streams < appl_bc_src_params.bc_subgroup_info[index].bc_nstream)
                {
                    /*
                     * BIS_Sync  : 4-octet bitfield. Bit 0-30 = BIS_index[1-31]
                     * 0x00000000: 0b0 = Do not synchronize to BIS_index[x]
                     * 0xxxxxxxxx: 0b1 = Synchronize to BIS_index[x]
                     * 0xFFFFFFFF: No Preference
                     */

                    /*
                     * GA Core provides BIS Index from k starting as 1,
                     * Hence use k from 1.
                     */
                    appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_bis_sync[index] |=
                        (0x00000001U << (k - 1U));
                    k++;
                    num_of_streams++;
                }
            }

            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.metadata_len =
                &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata_len[0U];

            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.metadata =
                &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_metadata[0U];

            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.bis_sync =
                &appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src_bis_sync[0U];
        }
    }
}

#ifdef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
void appl_bc_src_bap_set_bc_params_using_default_values(APPL_GA_ROLE role)
{
    UINT8                      qos_conf_set_id;
    int                        choice;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;
    UINT8                      index;
    UINT8                      i;
    GA_CODEC_INFO              codec_info = {0};

    GA_IGNORE_UNUSED_PARAM(role);

    /* Initialize */
    qos_conf_set_id = 0U;
    index = 0U;
    i = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_bc_src_bap_set_bc_params_using_default_values\n");

    /*
     * Recommendation for a preferred arrangement of multiple BIS subevents.
     * 0 - Sequential, 1 - Interleaved
     */
    appl_bc_src_params.bc_packing = 0x00; /* Sequential */

    /*
     * For each BIG, the Broadcast Source shall generate a Broadcast_ID
     * according to the requirements for random number generation
     */
    appl_bc_src_params.bc_id = 0xDEADBEU;

#ifdef GA_PBP
    /* 0 = PBP_BCAST_ENCRYPTION_OFF, 1 = PBP_BCAST_ENCRYPTION_ON */
#endif /* GA_PBP */
    appl_bc_src_params.bc_encryption = GA_FALSE;

    /* Broadcast Code parameter shall be represented as a 128-bit value */
    for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
    {
        /*
         * Update it to a 0x00 if appl_bc_src_params.bc_encryption
         * is set to GA_FALSE.
         */
        appl_bc_src_params.bc_code[index] = 0x00U;
    }

    /* Reusing the same value */
    index = 0U;

    CONSOLE_OUT("Enter no. of Subgroup: ");
    CONSOLE_IN("%d", &choice);
    if (1 > choice)
    {
        choice = 1U;
        APPL_INF
        (
            "There should be at least one subgroup. Num_Subgroups is set to 1\n"
        );
    }
    appl_bc_src_params.bc_num_subgroups = (UINT8)choice;

    /* Setting Broadcast Audio Stream Configuration Set Id */
    /*
     * TMAP_ROLE: BMS
     * Shall choose one of the below:
     *
     * 4:  16_2_1
     * 20: 16_2_2
     *
     * 11: 48_1_1
     * 27: 48_1_2
     * 12: 48_2_1
     * 28: 48_2_2
     * 13: 48_3_1 or 15: 48_5_1
     * 29: 48_3_2 or 31: 48_5_2
     * 14: 48_4_1 or 16: 48_6_1
     * 30: 48_4_2 or 32: 48_6_2
     *
     */
    /*
     * For now, 48_2_1 is selected,
     * Refer appl_ga_utils_bap_bc_get_qos_cs_cap_set_id()
     */
    choice = 12U; /* TODO: Low Latency or High - reliability ? */
    qos_conf_set_id = (UINT8)choice;

    /* Set PHY Preference */
    /* For any TMAP Role, TMAP Mandates 2M Phy */
    appl_bc_src_params.bc_qos_conf.phy = GA_PREF_PHY_LE_2M_PHY;

    /* Set Codec Frame Blocks Per SDU */
    /*
     * The absence of the Codec_Frame_Blocks_Per_SDU LTV structure shall be
     * interpreted as equivalent to a Codec_Frame_Blocks_Per_SDU value of 0x01.
     */
    cs_conf.mcfpSDU = 0x01U;

#if 0
    /*
     * ACA in LTV shall be inculded either by SG or BIS only if no of BIS
     * for that SG is equal to 1.
     * In this case, ACA LTV is added at the BIS level(Level 3)
     */
    /* TMAP Role: BMS, FL, FR */
    /* Set Audio Channel Allocation for BIG */
    cs_conf.aca = GA_AUDIO_LOCATION_FL;
#endif /* 0 */
    /*
     * Set Audio Channel Allocation to RFU 1 to show the absence of
     * ACA field in cs conf since at SG Level (Level 2), it is not M,
     * before calling appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv() to
     * skip this field. It will be included in BIS only.
     */
    cs_conf.aca = GA_AUDIO_LOCATION_RFU_1;

    /* Fetch the CSC Set ID and prepare qos cap */
    if (GA_SUCCESS ==
        appl_ga_utils_bap_bc_prepare_csc_and_qos_conf_based_on_set_id
        (
            qos_conf_set_id, &cs_conf, &appl_bc_src_params.bc_qos_conf
        ))
    {
        /*
         * If one of the Set is chosen as defined in BAP Spec,
         * the fields are mandated as below as per Spec.
         */
        appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id(&codec_info);

        /* Prepare the Codec Spec Configuration LTVs for LC3 */
        appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
        (
            &cs_conf,
            codec_info.ie,
            &codec_info.ie_len
        );
        appl_bc_src_params.bc_subgroup_info[index].qos_conf_set_id =
            qos_conf_set_id;
        appl_bc_src_params.bc_subgroup_info[index].cs_conf = cs_conf;
    }
    else
    {
        /*
         * Chosen a set not defined in BAP Spec, Shall be
         * vendor specific
         */
        appl_ga_utils_bap_get_codec_id_for_vendor_spec(&codec_info);

        /* Fetch the Codec Spec Configuration LTVs for Vendor Specific */
        appl_ga_utils_bap_get_vendor_spec_codec_spec_conf_ltv
        (
            codec_info.ie,
            &codec_info.ie_len
        );
    }

    /* TODO: Getting set to different value inside prepare QOS API */
    /* appl_bc_max_latency = 0x0005; */
    /* TODO: Getting set to different value inside prepare QOS API */
    /* appl_bc_rtn = 0x00; */

    /* TODO: Add the Metadata for the BIG and BIS */
    for (index = 0U; index < appl_bc_src_params.bc_num_subgroups; index++)
    {
        appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec = codec_info;
        GA_mem_set
        (
            &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta,
            0,
            sizeof(GA_METADATA)
        );

        appl_ga_utils_bap_get_input_and_create_metadata_ltv
        (
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.data,
            &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
        );

        CONSOLE_OUT("Enter no. of BIS: ");
        CONSOLE_IN("%d", &choice);
        if (1 > choice)
        {
            choice = 1U;
            APPL_INF
            (
                "There should be at least one BIS per subgroup. Num_BIS is set to 1\n"
            );
        }
        appl_bc_src_params.bc_subgroup_info[index].bc_nstream = (UINT8)choice;

        for (i = 0U; i < appl_bc_src_params.bc_subgroup_info[index].bc_nstream; i++)
        {
            GA_mem_set
            (
                &cs_conf,
                0U,
                sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
            );
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length = 0U;

            /*
             * Set Frame Duration for BIS to 0xFFU to show the absence of
             * FD field in cs conf before calling
             * appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv().
             * The FD is not M field in BIS level, hence skip this field.
             *
             */
            cs_conf.fd = 0xFFU;

            /* Set Audio Channel Allocation for BIS */
            cs_conf.aca = appl_ga_utils_bap_get_audio_channel_alloc();
            /* Create Codec Spec Conf */
            appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
            (
                &cs_conf,
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.value,
                &appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length
            );
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf =
                cs_conf;
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].qos_conf_set_id =
                qos_conf_set_id;
        }
    }

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_bc_src_bap_set_bc_params_using_default_values\n");
    return;
}
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
void appl_bc_src_bap_set_bc_params_using_runtime_values(void)
{
    UINT8                      qos_conf_set_id;
    int                        choice;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;
    UINT8                      index;
    UINT8                      i;
    GA_CODEC_INFO              codec_info = {0};

    qos_conf_set_id = 0U;
    index = 0U;
    i = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_bc_src_bap_set_bc_params_using_runtime_values\n");

    /* Setting of Packing - Sequential or Interleaved */
    appl_bc_src_params.bc_packing = appl_ga_utils_bap_get_packing();

    /*
     * For each BIG, the Broadcast Source shall generate a Broadcast_ID
     * according to the requirements for random number generation
     */
    appl_bc_src_params.bc_id = 0xDEADBEU;

#ifdef GA_PBP
    /* 0 = PBP_BCAST_ENCRYPTION_OFF, 1 = PBP_BCAST_ENCRYPTION_ON */
#endif /* GA_PBP */

    CONSOLE_OUT("Select your choice:\n1. UnEncrypted Stream\n2. Encrypted Stream\n");
    CONSOLE_IN("%d", &choice);
    if (1 == choice)
    {
        appl_bc_src_params.bc_encryption = GA_FALSE;
    }
    else if (2 == choice)
    {
        appl_bc_src_params.bc_encryption = GA_TRUE;
    }
    else
    {
        /* No action required */
    }

    if (GA_FALSE == appl_bc_src_params.bc_encryption)
    {
        APPL_TRC("Configuring unencrypted broadcast\n");
        /* Broadcast Code parameter shall be represented as a 128-bit value */
        for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
        {
            appl_bc_src_params.bc_code[index] = 0x00U;
        }
    }
    else
    {
        APPL_TRC("Configuring encrypted broadcast\n");
        CONSOLE_OUT("Enter the BC Code for encryption\n");
        /* Broadcast Code parameter shall be represented as a 128-bit value */
        for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
        {
            CONSOLE_IN("%x", &choice);
            appl_bc_src_params.bc_code[index] = (UINT8 )choice;
        }
    }

    /* Reusing the same value */
    index = 0U;

    CONSOLE_OUT("Enter no. of Subgroup: ");
    CONSOLE_IN("%d", &choice);
    if (1 > choice)
    {
        choice = 1U;
        APPL_INF
        (
            "There should be at least one subgroup. Num_Subgroups is set to 1\n"
        );
    }
    appl_bc_src_params.bc_num_subgroups = (UINT8)choice;

    /* Setting Broadcast Audio Stream Configuration Set Id */
    qos_conf_set_id = appl_ga_utils_bap_bc_get_qos_cs_cap_set_id();

    /* Set PHY Preference */
    appl_bc_src_params.bc_qos_conf.phy = appl_ga_utils_bap_get_pref_phy();

    /* Set Codec Frame Blocks Per SDU */
    cs_conf.mcfpSDU = appl_ga_utils_bap_get_codec_frame_block_per_sdu();

#if 0
    /*
     * ACA in LTV shall be inculded either by SG or BIS only if no of BIS
     * for that SG is equal to 1.
     * In this case, ACA LTV is added at the BIS level(Level 3)
     */
    /* Set Audio Channel Allocation */
    cs_conf.aca = appl_ga_utils_bap_get_audio_channel_alloc();
#endif /* 0 */
    /*
     * Set Audio Channel Allocation to RFU 1 to show the absence of
     * ACA field in cs conf
     */
    cs_conf.aca = GA_AUDIO_LOCATION_RFU_1;

    /* Fetch the CSC Set ID and prepare qos cap */
    if (GA_SUCCESS ==
        appl_ga_utils_bap_bc_prepare_csc_and_qos_conf_based_on_set_id
        (
            qos_conf_set_id, &cs_conf, &appl_bc_src_params.bc_qos_conf
        ))
    {
        /*
         * If one of the Set is chosen as defined in BAP Spec,
         * the fields are mandated as below as per Spec.
         */
        appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id(&codec_info);

        /* Prepare the Codec Spec Configuration LTVs for LC3 */
        appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
        (
            &cs_conf,
            codec_info.ie,
            &codec_info.ie_len
        );
        appl_bc_src_params.bc_subgroup_info[index].qos_conf_set_id =
            qos_conf_set_id;
        appl_bc_src_params.bc_subgroup_info[index].cs_conf = cs_conf;
    }
    else
    {
        /*
         * Chosen a set not defined in BAP Spec, Shall be
         * vendor specific
         */
        appl_ga_utils_bap_get_codec_id_for_vendor_spec(&codec_info);

        /* Fetch the Codec Spec Configuration LTVs for Vendor Specific */
        appl_ga_utils_bap_get_vendor_spec_codec_spec_conf_ltv
        (
            codec_info.ie,
            &codec_info.ie_len
        );
    }

    /* TODO: Getting set to different value inside prepare QOS API */
    /* appl_bc_max_latency = 0x0005; */
    /* TODO: Getting set to different value inside prepare QOS API */
    /* appl_bc_rtn = 0x00; */

    for (index = 0U; index < appl_bc_src_params.bc_num_subgroups; index++)
    {
        appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec = codec_info;
        GA_mem_set
        (
            &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta,
            0,
            sizeof(GA_METADATA)
        );

        appl_ga_utils_bap_get_input_and_create_metadata_ltv
        (
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.data,
            &appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
        );

        CONSOLE_OUT("Enter no. of BIS: ");
        CONSOLE_IN("%d", &choice);
        if (1 > choice)
        {
            choice = 1U;
            APPL_INF
            (
                "There should be at least one BIS per subgroup. Num_BIS is set to 1\n"
            );
        }
        appl_bc_src_params.bc_subgroup_info[index].bc_nstream = (UINT8)choice;

        for (i = 0U; i < appl_bc_src_params.bc_subgroup_info[index].bc_nstream; i++)
        {
            GA_mem_set
            (
                &cs_conf,
                0U,
                sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
            );
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length = 0U;

            /*
             * Set Frame Duration for BIS to 0xFFU to show the absence of
             * FD field in cs conf
             */
            cs_conf.fd = 0xFFU;

            /* Set Audio Channel Allocation for BIS */
            cs_conf.aca = appl_ga_utils_bap_get_audio_channel_alloc();
            /* Create Codec Spec Conf */
            appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
            (
                &cs_conf,
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.value,
                &appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length
            );
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf =
                cs_conf;
            appl_bc_src_params.bc_subgroup_info[index].bis_info[i].qos_conf_set_id =
                qos_conf_set_id;
        }
    }

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_bc_src_bap_set_bc_params_using_runtime_values\n");
    return;
}
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

void appl_ga_bc_set_appearance_value(UINT16 * appearance)
{
    /* Fetch Appearance Value */
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    *appearance = appl_ga_utils_bap_get_audio_src_appearance_value();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    *appearance = 0x0880;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

    return;
}

void appl_ga_bc_set_src_setting(APPL_GA_ROLE role)
{
    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_set_src_setting\n");

#ifdef GA_PBP
    GA_IGNORE_UNUSED_PARAM(role);
#endif /* GA_PBP */

    /* Initialize BASE structure */
    appl_ga_bc_ba_src_init();

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
#ifdef GA_PBP
    /* Take input for SQ or HQ, by default, use HQ */
    appl_bc_src_pbp_audio_conf_type = appl_ga_utils_bap_get_pbp_audio_conf_type();
#endif /* GA_PBP */

    appl_bc_src_bap_set_bc_params_using_runtime_values();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
#ifdef GA_PBP
    appl_bc_src_pbp_audio_conf_type = 2U; /* High Quality */
#endif /* GA_PBP */
    appl_bc_src_bap_set_bc_params_using_default_values(role);
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

    /* Display the Broadcast Audio Stream Configuration */
    appl_bc_src_bap_display_bc_settings();

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_set_src_setting\n");
    return;
}

GA_RESULT appl_bc_bass_ba_inform_scan_status(void)
{
    /* By default uses Write Request */
    GA_BASS_CE_WRITE_REQ req = { 0 };
    UINT8 write_pdu[5U];
    GA_RESULT retval;
    UINT8 marker;

    retval = GA_FAILURE;
    marker = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_inform_scan_status\n");

    /* First Byte as opcode */
    if (GA_TRUE == appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].scan_status)
    {
        write_pdu[marker] = BASS_CP_OPCODE_REMOTE_SCAN_STARTED;
        marker++;
    }
    else
    {
        write_pdu[marker] = BASS_CP_OPCODE_REMOTE_SCAN_STOPPED;
        marker++;
    }

    req.req_data.val = &write_pdu[0U];
    req.req_data.len = marker;
    /* Write request */
    req.write_type = 0U;

    APPL_TRC("GA_bass_ce_inform_remote_scan_status\n");
    retval = GA_bass_ce_inform_remote_scan_status
             (
                 appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bass_ba_handle,
                 &req
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_inform_scan_status\n");
    return retval;
}

GA_RESULT appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle
          (
              UINT16   bis_conn_handle,
              UINT8  * sg_info_index,
              UINT8  * bis_info_index
          )
{
    GA_RESULT  retval;
    UINT8      sg_info_index_local;
    UINT8      bis_info_index_local;

    /* Initialize */
    retval = GA_FAILURE;
    sg_info_index_local = 0U;
    bis_info_index_local = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle\n");

    if ((NULL == sg_info_index) || (NULL == bis_info_index))
    {
        return GA_FAILURE;
    }

    for (sg_info_index_local = 0U; sg_info_index_local < APPL_BC_MAX_NUM_OF_SUB_GROUPS; sg_info_index_local++)
    {
        for (bis_info_index_local = 0U; bis_info_index_local < appl_bc_src_params.bc_subgroup_info[sg_info_index_local].bc_nstream; bis_info_index_local++)
        {
            if (bis_conn_handle ==
                appl_bc_src_params.bc_subgroup_info[sg_info_index_local].bis_info[bis_info_index_local].bis_handle)
            {
                *sg_info_index = sg_info_index_local;
                *bis_info_index = bis_info_index_local;
                retval = GA_SUCCESS;
                break;
            }
        }
    }

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_ba_get_sg_index_bis_info_index_from_bis_conn_handle\n");

    return retval;
}

GA_RESULT appl_bc_bass_ba_fetch_entry_in_db
          (
              UINT8 * rem_dev_index
          )
{
    UINT8 index;
    GA_RESULT retval;
    GA_ENDPOINT device;

    retval = GA_SUCCESS;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_fetch_entry_in_db\n");

    for (index = 0; index < APPL_BC_BA_MAX_SD_DEVICES; index++)
    {
        /* Fetch device from conn_dev_index */
        if (GA_SUCCESS == appl_ga_utils_get_conn_device
                          (
                              appl_bc_bass_rem_sd_db[index].conn_dev_index,
                              &device
                          ))
        {
            /* Do nothing as the SD is already present in this Index */
        }
        else
        {
            /* This SD index is free in the DB */
            *rem_dev_index = index;
            break;
        }
    }

    if (APPL_BC_BA_MAX_SD_DEVICES == index)
    {
        retval = GA_FAILURE;
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_fetch_entry_in_db\n");
    return retval;
}

void appl_bc_bass_ba_set_rem_sd_in_use(void)
{
    UINT8        index;
    GA_ENDPOINT  device;
    int          input;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_set_rem_sd_in_use\n");

    /* Display all the SD devices that BA is connected to */
    for (index = 0U; index < APPL_BC_BA_MAX_SD_DEVICES; index++)
    {
        if (INVALID_CONN_DEV_INDEX != appl_bc_bass_rem_sd_db[index].conn_dev_index)
        {
            if (GA_SUCCESS == appl_ga_utils_get_conn_device
                              (
                                  appl_bc_bass_rem_sd_db[index].conn_dev_index,
                                  &device
                              ))
            {
                if (GA_IS_NON_ZERO_ENDPOINT_ADDR(device.bd_addr))
                {
                    appl_ga_utils_display_bdaddr_and_type(&device);
                    APPL_TRC("SD Index: %02X\n\n", index);
                }
            }
        }
    }

    CONSOLE_OUT("Enter the SD Index to set: ");
    CONSOLE_IN("%d", &input);

    appl_bc_rem_sd_in_use = (UINT8)input;

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_set_rem_sd_in_use\n");

    return;
}

GA_RESULT appl_bc_bass_ba_is_sd_not_added_in_db
          (
              GA_ENDPOINT sd_device
          )
{
    UINT8 index;
    GA_RESULT retval;
    GA_ENDPOINT device;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_is_sd_not_added_in_db\n");

    for (index = 0; index < APPL_BC_BA_MAX_SD_DEVICES; index++)
    {
        /* Fetch device from conn_dev_index */
        if (GA_SUCCESS == appl_ga_utils_get_conn_device
                          (
                              appl_bc_bass_rem_sd_db[index].conn_dev_index,
                              &device
                          ))
        {
            if ((GA_IS_NON_ZERO_ENDPOINT_ADDR(device.bd_addr))
                &&
                (GA_TRUE == GA_COMPARE_ENDPOINT
                            (
                                &device,
                                &sd_device
                            )))
            {
                break;
            }
        }
    }

    if (APPL_BC_BA_MAX_SD_DEVICES == index)
    {
        retval = GA_SUCCESS;
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_is_sd_not_added_in_db\n");
    return retval;
}

GA_RESULT appl_bc_bass_ba_fetch_free_bc_src_index_in_db(UINT8 * index)
{
    GA_RESULT retval;
    UINT8 i;

    /* Initialize */
    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_fetch_free_bc_src_index_in_db\n");

    for (i = 0U; i < APPL_BC_BA_MAX_BC_SRC_DEVICES; i++)
    {
        if (!GA_BRR_DEV_ADDR_IS_NON_ZERO
             (
                 appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.adv_addr
             ))
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_fetch_free_bc_src_index_in_db\n");
    return retval;
}

GA_RESULT appl_bc_bass_ba_fetch_bc_src_entry_present_in_db
          (
              UINT16  sync_handle,
              UINT8 * index
          )
{
    UINT8 i;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_fetch_bc_src_entry_present_in_db\n");

    for (i = 0U; i < APPL_BC_BA_MAX_BC_SRC_DEVICES; i++)
    {
        if (appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.sync_handle == sync_handle)
        {
            *index = i;
            return GA_SUCCESS;
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_fetch_bc_src_entry_present_in_db\n");
    return GA_FAILURE;
}

GA_RESULT appl_bc_bass_ba_is_bc_src_entry_present_in_db
          (
              UINT8 * adv_addr,
              UINT8   adv_type,
              UINT8 * index
          )
{
    UINT8 i;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_is_bc_src_entry_present_in_db\n");

    for (i = 0U; i < APPL_BC_BA_MAX_BC_SRC_DEVICES; i++)
    {
        if ((GA_TRUE == GA_COMPARE_BRR_DEV_ADDR
                        (
                            adv_addr,
                            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.adv_addr
                        ))
            &&
            (GA_TRUE == GA_COMPARE_BRR_DEV_ADDR_TYPE
                        (
                            adv_type,
                            appl_bc_bass_ba_bc_src[i].appl_bc_bass_bc_src.src.adv_addr_type
                        )))
        {
            *index = i;
            return GA_SUCCESS;
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_is_bc_src_entry_present_in_db\n");
    return GA_FAILURE;
}

void appl_bc_bass_ba_clear_bc_src_db(void)
{
    UINT8 index;

    for (index = 0U; index < APPL_BC_BA_MAX_BC_SRC_DEVICES; index++)
    {
        GA_mem_set
        (
            &appl_bc_bass_ba_bc_src[index],
            0U,
            sizeof(APPL_BASS_BA_BC_SRC_DB)
        );
    }
}

void appl_bc_bass_ba_add_bc_src_info_into_db
     (
         UINT8 index,
         BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS bc_source_announcement,
         BAP_BROADCAST_ANNOUNCEMENT bc_annoucement
     )
{
    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_add_bc_src_info_into_db\n");

    GA_COPY_BRR_DEV_ADDR
    (
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr,
        bc_source_announcement.address
    );

    GA_COPY_BRR_DEV_TYPE
    (
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr_type,
        bc_source_announcement.address_type
    );

    appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_SID =
        bc_source_announcement.advertising_sid;

    GA_mem_copy
    (
        appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.broadcast_ID,
        &bc_annoucement.broadcast_id,
        GA_BC_ID_LEN
    );

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_add_bc_src_info_into_db\n");

    return;
}

GA_RESULT appl_bc_bass_ba_validate_add_bc_src(UINT8 index)
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_validate_add_bc_src\n");

    if (GA_SUCCESS != appl_bc_bass_ba_is_src_added_in_ba_db(index))
    {
        APPL_ERR("There is already an instance for this Broadcast Source !\n");
        retval = GA_FAILURE;
    }
    else
    {
        if (GA_SUCCESS != appl_bc_bass_ba_is_pacs_compatible(index))
        {
            APPL_ERR("Broadcast Sink collocated with Scan Delegator is not capable"
            " of decoding broadcast Audio Stream !\n");
            retval = GA_FAILURE;
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_validate_add_bc_src\n");
    return retval;
}

UINT8 appl_bc_bass_ba_is_pacs_compatible(UINT8 src_index)
{
    UINT8 index, n_sg;
    GA_LC3_CODEC_SPECIFIC_CAP   cs_cap = {0};
    UINT8                       capabilitySupp;
    GA_ENDPOINT                 device;
    PACS_CAP_INFO               rem_pacs_db = {0};

    capabilitySupp = GA_FALSE;
    n_sg = appl_bc_bass_ba_bc_src[src_index].appl_bc_bass_bc_src.src.num_subgroups;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_is_pacs_compatible\n");

    /* Fetch device from conn_dev_index */
    if (GA_SUCCESS == appl_ga_utils_get_conn_device
                      (
                          appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].conn_dev_index,
                          &device
                      ))
    {
        /* Fetch PACS of SD */
        if (GA_SUCCESS == appl_ga_utils_bap_fetch_remote_pacs_based_on_dev
                          (
                              device,
                              &rem_pacs_db
                          ))
        {
            /* TODO: Fetch the Combined CS Conf of the BIS of a SG of the BC Src */

            /* TODO: Validate PACS of SD with the Codec Spec Conf */
            for (index = 0U; index < rem_pacs_db.snk_cap_count; index++)
            {
                capabilitySupp = GA_TRUE;

                appl_ga_utils_bap_parse_and_display_codec_spec_cap_ltv
                (
                    rem_pacs_db.snk_cap[index].cap.codec.ie,
                    rem_pacs_db.snk_cap[index].cap.codec.ie_len,
                    &cs_cap
                );

                /*
                 * As per BAP Spec, Compare only CC and the ACA.
                 * The Broadcast Assistant may determine whether a Broadcast
                 * Sink collocated with the Scan Delegator is capable of
                 * rendering a broadcast Audio Stream to a specified location
                 * by performing the procedure in Section 6.5.1
                 * or by receiving notifications of the Sink Audio Locations
                 * characteristic and comparing the value to the
                 * Audio_Channel_Allocation (see Section 4.3.3) LTV structure,
                 * if present, in the BASE transmitted by the Broadcast Source.
                 */
            }
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_is_pacs_compatible\n");
    return capabilitySupp;
}

GA_RESULT appl_bc_bass_ba_is_src_added_in_ba_db(UINT8 index)
{
    GA_RESULT retval;
    UINT8     i;
    UINT8     j;

    retval = GA_SUCCESS;
    i = 0U;
    j = 0U;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_is_src_added_in_ba_db\n");

    for (i = 0U; i < appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_bass_rx_state_count; i++)
    {
        if ((appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr_type ==
             appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[i].bc_rx_state.info.addr_type) &&
            (GA_COMPARE_ENDPOINT_ADDR(appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_addr, appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[i].bc_rx_state.info.addr) &&
            (appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.adv_SID == appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[i].bc_rx_state.info.adv_sid)))
        {
            for (j = 0U; j < BASS_BC_ID_LEN; i++)
            {
                if (appl_bc_bass_ba_bc_src[index].appl_bc_bass_bc_src.src.broadcast_ID[j] !=
                        appl_bc_bass_rem_sd_db[appl_bc_rem_sd_in_use].bc_rx_state_db[i].bc_rx_state.info.broadcast_ID[j])
                {
                    retval = GA_FAILURE;
                    break;
                }
            }
            if ((GA_SUCCESS == retval) && (BASS_BC_ID_LEN == j))
            {
                break;
            }
        }
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_is_src_added_in_ba_db\n");
    return retval;
}

void appl_bc_bass_ba_parse_and_display_bc_rx_state_bis_sync_state(UINT32 bis_sync)
{
    UINT8 index;

    if (BASS_BIS_SYNC_STATE_FAILED_TO_SYNC_TO_BIG == bis_sync)
    {
        APPL_TRC("Failed to sync to BIG\n");
    }
    else
    {
        APPL_TRC("bis_index: \n");
        for (index = 0U; index < 32; index++)
        {
            if ((bis_sync >> index) & 1U)
            {
                /* BIS_Index Synced */
                APPL_TRC("%d\n", index + 1U);
            }
        }
    }

    return;
}

GA_RESULT appl_bc_bass_ba_parse_and_display_bc_rx_state
          (
              GA_BASS_BC_RX_STATE br_rx_state_info
          )
{
    GA_RESULT  retval;
    UCHAR      i;
    UINT8      prev_metadata_len;

    prev_metadata_len = 0U;
    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_BA]: >> appl_bc_bass_ba_parse_and_display_bc_rx_state\n");

    if (0U != br_rx_state_info.info_len)
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Source_ID: 0x%02X\n",
            (UCHAR)sizeof(br_rx_state_info.info.id),
            br_rx_state_info.info.id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Source_Address_Type: ",
            (UCHAR)sizeof(br_rx_state_info.info.addr_type)
        );
        appl_bc_bass_ba_parse_and_display_addr_type
        (
            br_rx_state_info.info.addr_type
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Source_Address: %02X %02X %02X %02X %02X %02X\n",
            GA_BD_ADDR_SIZE,
            br_rx_state_info.info.addr[0U],
            br_rx_state_info.info.addr[1U],
            br_rx_state_info.info.addr[2U],
            br_rx_state_info.info.addr[3U],
            br_rx_state_info.info.addr[4U],
            br_rx_state_info.info.addr[5U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Source_Adv_SID: 0x%02X\n",
            (UCHAR)sizeof(br_rx_state_info.info.adv_sid),
            br_rx_state_info.info.adv_sid
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Broadcast_ID: %02X %02X %02X\n",
            BASS_BC_ID_LEN,
            br_rx_state_info.info.broadcast_ID[0U],
            br_rx_state_info.info.broadcast_ID[1U],
            br_rx_state_info.info.broadcast_ID[2U]
        );
        appl_bc_bass_ba_parse_and_display_PA_Sync_State
        (
            br_rx_state_info.info.pa_sync_state
        );
        appl_bc_bass_ba_parse_and_display_BIG_encryption
        (
            br_rx_state_info.info.big_encryption
        );
        if (BASS_BIG_ENCRYPTION_BAD_CODE == br_rx_state_info.info.big_encryption)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Bad_Code:\n",
                BASS_BC_CODE_MAX_LEN
            );
            /*
             * Bad_Code shall be set to the value of the incorrect 16-octet
             * Broadcast_Code that fails to decrypt the BIG
             */
            appl_dump_bytes(br_rx_state_info.info.bad_code, BASS_BC_CODE_MAX_LEN);
        }
        APPL_TRC
        (
            "Len: 0x%02X,  Num_Subgroups: 0x%02X\n",
            (UCHAR)sizeof(br_rx_state_info.info.num_subgroups),
            br_rx_state_info.info.num_subgroups
        );
        for (i = 0U; i < br_rx_state_info.info.num_subgroups; i++)
        {
            APPL_TRC("Subgroup Data[%d]:\n", i);
            appl_bc_bass_ba_parse_and_display_bc_rx_state_bis_sync_state
            (
                br_rx_state_info.info.bis_sync_state[i]
            );
            APPL_TRC
            (
                "\tLen: 0x%02X,  BIS_Sync: ",
                (UCHAR)sizeof(br_rx_state_info.info.bis_sync_state[i])
            );
            APPL_TRC
            (
                "0x%08X\n",
                br_rx_state_info.info.bis_sync_state[i]
            );
            APPL_TRC
            (
                "\tLen: 0x%02X,  Metadata_Length: ",
                (UCHAR)sizeof(br_rx_state_info.info.metadata_len[i])
            );
            APPL_TRC
            (
                "0x%02X\n",
                br_rx_state_info.info.metadata_len[i]
            );
            APPL_TRC
            (
                "\tLen: 0x%02X,  Metadata:\n",
                br_rx_state_info.info.metadata_len[i]
            );
            appl_ga_utils_bap_parse_and_display_metadata_ltv
            (
                &br_rx_state_info.info.metadata[prev_metadata_len],
                br_rx_state_info.info.metadata_len[i],
                NULL,
                NULL
            );
            prev_metadata_len += br_rx_state_info.info.metadata_len[i];
        }
        retval = GA_SUCCESS;
    }

    APPL_DBG("[APPL][GA][BC_BA]: << appl_bc_bass_ba_parse_and_display_bc_rx_state\n");
    return retval;
}

void appl_bc_bass_ba_parse_and_display_addr_type(UINT8 addr_type)
{
    if (BASS_ADDR_TYPE_PUBLIC == addr_type)
    {
        APPL_TRC
        (
            "Public Device Address or Public Identity Address (0x%02X)\n",
            addr_type
        );
    }
    else if (BASS_ADDR_TYPE_RANDOM == addr_type)
    {
        APPL_TRC
        (
            "Random Device Address or Random (static) Identity Address (0x%02X)\n",
            addr_type
        );
    }
    else
    {
        APPL_ERR
        (
            "Unknown Address type (0x%02X) !\n",
            addr_type
        );
    }

    return;
}

void appl_bc_bass_ba_parse_and_display_PA_Sync_State(UINT8 pa_sync_state)
{
    APPL_TRC("Len: 0x%02X,  PA_Sync_State: ", (UCHAR)sizeof(pa_sync_state));
    switch (pa_sync_state)
    {
    case BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED:
        APPL_TRC("Not synchronized to PA (0x%02X)\n", pa_sync_state);
        break;
    case BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST:
        APPL_TRC("SyncInfo Request (0x%02X)\n", pa_sync_state);
        break;
    case BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA:
        APPL_TRC("Synchronized to PA (0x%02X)\n", pa_sync_state);
        break;
    case BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA:
        APPL_TRC("Failed to synchronize to PA (0x%02X)\n", pa_sync_state);
        break;
    case BASS_PA_SYNC_STATE_NOPAST:
        APPL_TRC("No PAST (0x%02X)\n", pa_sync_state);
        break;
    default:
        APPL_ERR("Unknown PA_Sync_State value (0x%02X) !\n", pa_sync_state);
        break;
    }

    return;
}

void appl_bc_bass_ba_parse_and_display_BIG_encryption(UINT8 big_encryption)
{
    APPL_TRC("Len: 0x%02X,  BIG_Encryption: ", (UCHAR)sizeof(big_encryption));
    switch (big_encryption)
    {
    case BASS_BIG_ENCRYPTION_NOT_ENCRYPTED:
        APPL_TRC("Not encrypted (0x%02X)\n", big_encryption);
        break;
    case BASS_BIG_ENCRYPTION_BC_CODE_REQUIRED:
        APPL_TRC("Broadcast_Code required (0x%02X)\n", big_encryption);
        break;
    case BASS_BIG_ENCRYPTION_DECRYPTING:
        APPL_TRC("Decrypting (0x%02X)\n", big_encryption);
        break;
    case BASS_BIG_ENCRYPTION_BAD_CODE:
        APPL_TRC
        (
            "Bad_Code (incorrect encryption key (0x%02X)\n",
            big_encryption
        );
        break;
    default:
        APPL_ERR("Unknown BIG_Encryption value (0x%02X) !\n", big_encryption);
        break;
    }

    return;
}

/* Revisit */
void appl_bc_src_bap_display_bc_settings(void)
{
    UINT8 index;
    UINT8 i;

    /* Intialize */
    index = 0U;
    i = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_bc_src_bap_display_bc_settings\n");

    APPL_TRC("**************Broadcast Audio Stream Configuration**************\n");
    APPL_TRC("Packing: ");
    appl_ga_utils_bap_display_packing
    (
        appl_bc_src_params.bc_packing
    );
    APPL_TRC("Encryption: 0x%02X\n", appl_bc_src_params.bc_encryption);
    /* Display QOS configuration */
    appl_ga_utils_bap_parse_and_display_qos_cnf
    (
        appl_bc_src_params.bc_qos_conf
    );

    APPL_TRC("Broadcast Code: ");
    for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
    {
        APPL_TRC("%02X ", appl_bc_src_params.bc_code[index]);
    }
    APPL_TRC("\n");
    APPL_TRC("Broadcast ID: 0x%06X\n", appl_bc_src_params.bc_id);
    APPL_TRC
    (
        "Number of Subgroups: 0x%02X\n",
        appl_bc_src_params.bc_num_subgroups
    );

    /* Display All Subgroup Info */
    for (index = 0U; index < appl_bc_src_params.bc_num_subgroups; index++)
    {
        APPL_TRC("Subgroup Info [%d]:\n", (index + 1U));
        APPL_TRC
        (
            "No. of BIS: 0x%02X\n",
            appl_bc_src_params.bc_subgroup_info[index].bc_nstream
        );
        appl_ga_utils_bap_display_codec_id
        (
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8),
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec.ie_len
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Codec_Specific_Configuration:\n",
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec.ie_len
        );

        appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
        (
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec.ie,
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.codec.ie_len,
            NULL
        );

        APPL_TRC
        (
            "Len: 0x%02X,  Metadata_Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8),
            appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
        );
        if (0x00U != appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Metadata:\n",
                appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
            );

            appl_ga_utils_bap_parse_and_display_metadata_ltv
            (
                appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.data,
                appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length,
                NULL,
                NULL
            );
        }
        else
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Metadata: NULL\n",
                appl_bc_src_params.bc_subgroup_info[index].bc_basic_announcement_1.meta.length
            );
        }

        /* Display All BIS Info */
        for (i = 0U; i < appl_bc_src_params.bc_subgroup_info[index].bc_nstream; i++)
        {
            APPL_TRC("BIS Info [%d]:\n", (i + 1U));
            APPL_TRC
            (
                "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Codec_Specific_Configuration:\n",
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length
            );
            appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
            (
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.value,
                appl_bc_src_params.bc_subgroup_info[index].bis_info[i].cs_conf_ltv.length,
                NULL
            );
        }
    }
    APPL_TRC("****************************************************************\n");

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_bc_src_bap_display_bc_settings\n");
    return;
}

GA_RESULT appl_ga_bc_ba_src_remove_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          )
{
    GA_RESULT  retval;

    /* Initialize */
    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_ga_bc_ba_src_remove_iso\n");

    /* Check for ISO State */
    if ((AUDIO_ISO_SETUP_COMPLETE ==
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state) &&
        (GA_TRUE ==
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status))
    {
        /* Remove ISO Data Data Path */
        retval = appl_ga_utils_audio_src_le_remove_iso_data_path
                 (
                     appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                 );

        if (GA_SUCCESS == retval)
        {
            APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_IDLE;
        }
        else if (GA_CONTINUE == retval)
        {
            APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: In-progress\n", BA_GET_ROLE_STRING(bc_role));
            appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_UPDATE_IN_PROGRESS;
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Failed !\n", BA_GET_ROLE_STRING(bc_role));
        }
    }

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_ba_src_remove_iso\n");
    return retval;
}

void appl_ga_bc_ba_init(void)
{
    UINT8  sd_index;
    UINT8  rx_state_db_index;

    /* Initialize */
    sd_index = 0U;
    rx_state_db_index = 0U;

    for (sd_index = 0U; sd_index < APPL_BC_BA_MAX_SD_DEVICES; sd_index++)
    {
        for (rx_state_db_index = 0U; rx_state_db_index < BASS_CE_MAX_BC_RX_STATE_INSTANCES; rx_state_db_index++)
        {
            appl_bc_bass_rem_sd_db[sd_index].bc_rx_state_db[rx_state_db_index].bc_rx_state.info.id =
                APPL_BASS_INVALID_SRC_ID;

            appl_bc_bass_rem_sd_db[sd_index].conn_dev_index =
                INVALID_CONN_DEV_INDEX;
        }
    }

    return;
}

GA_RESULT appl_ga_bc_src_fetch_first_valid_sg_bis_info_index
          (
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    if ((appl_bc_src_params.bc_num_subgroups > 0U) &&
        (appl_bc_src_params.bc_subgroup_info[0U].bc_nstream > 0U))
    {
        /* TODO: How to validate if this BIS index is a valid one? */
        *bis_info_index = 0U;
        *sg_info_index = 0U;
        retval = GA_SUCCESS;
    }
    else
    {
        if (GA_SUCCESS == appl_ga_bc_src_fetch_next_valid_sg_bis_info_index
                          (
                              0U, /* Currently, Try for other BISes in first SG */
                              0U, /* Search for next valid BIS, since 0th is invalid */
                              sg_info_index,
                              bis_info_index
                          ))
        {
            /* No Action, this is a valid SG Index and BIS Info Index */
            retval = GA_SUCCESS;
        }
        else
        {
            /* We do not have any SGs or any BISes in any of the SGs */
        }
    }

    return retval;
}

GA_RESULT appl_ga_bc_src_fetch_next_valid_sg_bis_info_index
          (
              UINT8   curr_sg_info_index,
              UINT8   curr_bis_info_index,
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    /* Move to the Next BIS Index */
    curr_bis_info_index++;

    /*
     * Check if current BIS has reached total no of BIS in the
     * current SG.
     * If Yes, Reset the value of bis_info_index for next iteration and
     * sg_info_index to the next SG.
     * If No, this is a valid BIS, Send this.
     */
    if (appl_bc_src_params.bc_subgroup_info[curr_sg_info_index].bc_nstream
        == curr_bis_info_index)
    {
        *bis_info_index = 0U;
        curr_sg_info_index++;
        *sg_info_index = curr_sg_info_index;
    }
    else
    {
        /* TODO: How to validate if this BIS index is a valid one? */
        *bis_info_index = curr_bis_info_index;
        *sg_info_index = curr_sg_info_index;
    }

    if ((appl_bc_src_params.bc_num_subgroups > *sg_info_index) &&
        (appl_bc_src_params.bc_subgroup_info[*sg_info_index].bc_nstream > *bis_info_index))
    {
        retval = GA_SUCCESS;
    }
    else
    {
        /* End of List, We do not have anymore BISes in any of the SGs */
    }

    return retval;
}

GA_RESULT appl_ga_bc_ba_update_transmitting_audio
          (
              UINT16 bis_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8 qos_conf_set_id,
              UINT8 bps,
              UINT8 status
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

#ifdef AUDIO_SRC_ISO_SUPPORT
    /* As we have already removed the entry, update entry is not required */
    if (GA_FALSE == status)
    {
        retval = GA_SUCCESS;
        /*
         * TODO: Check if needs to be printed. We are not triggering remove_iso,
         * instead its removed along with remove_BIG
         */
#if 0
        APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", BA_GET_ROLE_STRING(bc_role));
#endif /* 0 */
    }
    else
    {
        retval = appl_ga_utils_audio_src_update_entry
                 (
                     bis_handle,
                     AUDIO_IGNORE_VALUE,
                     AUDIO_IGNORE_VALUE,
                     status
                 );
    }
    if (GA_SUCCESS == retval)
    {
#ifdef AUDIO_SRC_LC3_SUPPORT
        /*
         * As a Source, Even if we are supporting more than 1 BISes
         * streaming simultaneously, Create LC3 Encoder Module only once
         * and Start Audio PL to generate only once, since we are
         * streaming same audio across all BISes based on the CC.
         * If any BIS has a diff CC, same data will be mixed based on the
         * CC and sent across.
         *
         * This function will be called multipe times for multiple BISes to
         * setup ISO data path and update ISO entry.
         * When qos_conf_set_id is set to a valid value other than 0xFF,
         * it means this is the first BIS and hence create LC3 Encoder and
         * start Audio Playback module.
         */
        if (GA_TRUE == status)
        {
            if (0xFFU != qos_conf_set_id)
            {
                retval = appl_ga_utils_audio_src_create_lc3_encoder
                         (
                             cs_conf,
                             qos_conf_set_id,
                             bps
                         );
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][%s]: LC3 Encoder Setup Status: Created\n",
                     BA_GET_ROLE_STRING(bc_role));
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: LC3 Encoder Setup Status: Could not create !\n",
                     BA_GET_ROLE_STRING(bc_role));
                }
            }
        }
        else if (GA_FALSE == status)
        {
            if (0xFFU != qos_conf_set_id)
            {
                if (GA_TRUE == appl_ga_utils_audio_src_is_lc3_encoder_running())
                {
                    retval = appl_ga_utils_audio_src_delete_lc3_encoder();
                    if (GA_SUCCESS == retval)
                    {
                        APPL_TRC("[APPL][GA][%s]: LC3 Encoder Setup Status: Deleted\n",
                            BA_GET_ROLE_STRING(bc_role));
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][%s]: LC3 Encoder Setup Status: Could not delete !\n",
                            BA_GET_ROLE_STRING(bc_role));
                    }
                }
                else
                {
                    APPL_TRC("[APPL][GA][%s]: LC3 Encoder Setup Status: Already Deleted\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
            }
        }
#else /* AUDIO_SRC_LC3_SUPPORT */
        APPL_ERR("AUDIO_SRC_LC3_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_LC3_SUPPORT */
#ifdef AUDIO_SRC_PL_SUPPORT
        if (GA_TRUE == status)
        {
            if (0xFFU != qos_conf_set_id)
            {
                retval = appl_ga_utils_audio_src_setup_generator_pl
                         (
                             cs_conf,
                             bps,
                             AUDIO_SRC_PL_BUFFER_SIZE
                         );
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][%s]: Audio PL Generator Setup Status: Success\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: Audio PL Generator Setup Status: Failed !\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
                retval = appl_ga_utils_audio_src_start_generator_pl();
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][%s]: Audio PL Generator Start Process Status: Success\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: Audio PL Generator Start Process Status: Failed !\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
            }
        }
        else if (GA_FALSE == status)
        {
            if (0xFFU != qos_conf_set_id)
            {
                if (GA_TRUE == appl_ga_utils_audio_src_is_generator_pl_running())
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_src_stop_generator_pl())
                    {
                        APPL_TRC("[APPL][GA][%s]: Audio PL Generator Stop Process Status: Success\n",
                            BA_GET_ROLE_STRING(bc_role));
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][%s]: Audio PL Generator Stop Process Status: Failed !\n",
                            BA_GET_ROLE_STRING(bc_role));
                    }
                }
                else
                {
                    APPL_TRC("[APPL][GA][%s]: Audio PL Playback Status: Already Stopped\n",
                        BA_GET_ROLE_STRING(bc_role));
                }
            }
        }
#else /* AUDIO_SRC_PL_SUPPORT */
        APPL_ERR("AUDIO_SRC_PL_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_PL_SUPPORT */
    }
    else
    {
        APPL_ERR("[APPL][GA][%s]: ISO Data Path Setup Status: Update entry failed !\n",
            BA_GET_ROLE_STRING(bc_role));
    }
#else /* AUDIO_SRC_ISO_SUPPORT */
APPL_ERR("AUDIO_SRC_ISO_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_ISO_SUPPORT */
    return retval;
}

void appl_ga_bc_ba_bass_display_ga_result(GA_RESULT status)
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
        case GA_BASS_INVALID_STATE:
            APPL_ERR("Invalid State !");
            break;

        case GA_BASS_INVALID_PARAMS:
            APPL_ERR("Invalid Params !");
            break;

        case GA_BASS_CNTX_ALLOC_FAILED:
            APPL_ERR("Context allocation failed !");
            break;

        case GA_BASS_IN_BUSY_STATE:
            APPL_ERR("In Busy State !");
            break;

        case GA_BASS_CNTX_NOT_FOUND:
            APPL_ERR("Context not found !");
            break;

        case GA_BASS_INVALID_HANDLE:
            APPL_ERR("Invalid Handle !");
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
            switch (status)
            {
                /* Application error code */
            case BASS_APPL_ERR_CODE_OPCODE_NOT_SUPP:
                APPL_ERR("Opcode Not Supported !");
                break;

            case BASS_APPL_ERR_CODE_INVALID_SRC_ID:
                APPL_ERR("Invalid Source_ID !");
                break;

            default:
                APPL_ERR("Invalid Code !");
                break;
            }
            APPL_TRC(" (0x%04X)\n", status);
        }
    }
    return;
}

GA_RESULT appl_ga_bc_ba_src_setup_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_ga_bc_ba_src_setup_iso\n");

    if ((GA_FALSE == appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status) &&
        (AUDIO_ISO_IDLE == appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state))
    {
        if (GA_SUCCESS != appl_ga_utils_audio_src_set_entry
                          (
                              appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                              appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis
                          ))
        {
            APPL_ERR
            (
                "[APPL][GA][%s]: ISO Data Path Setup Status: Set entry failed for SG: %d, BIS: %d !\n",
                BA_GET_ROLE_STRING(bc_role),
                sg_info_index,
                bis_info_index
            );
        }
        else
        {
            /* Setup ISO data path */
            retval = appl_ga_utils_audio_src_le_setup_iso_data_path
                     (
                         appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                     );
            if (GA_SUCCESS == retval)
            {
                /* This case is never expected */
                APPL_TRC("[APPL][GA][%s]: ISO Data Path Setup Status: Already Complete\n", BA_GET_ROLE_STRING(bc_role));
                appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_SETUP_COMPLETE;
            }
            else if (GA_CONTINUE == retval)
            {
                /* ISO Data Path is Ongoing */
                APPL_TRC("[APPL][GA][%s]: ISO Data Path Setup Status: In-progress\n", BA_GET_ROLE_STRING(bc_role));
                appl_bc_src_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_UPDATE_IN_PROGRESS;
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: ISO Data Path Setup Status: Failed !\n", BA_GET_ROLE_STRING(bc_role));
            }
        }
    }

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_ba_src_setup_iso\n");

    return retval;
}

void appl_ga_bc_src_suspend_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;

    /* Initialize */
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(data);
    GA_IGNORE_UNUSED_PARAM(datalen);

    /* TODO: Pass the Session as Data */

    APPL_TRC ("GA_broadcast_suspend\n");
    retval = GA_broadcast_suspend
             (
                 appl_bc_src_params.bc_ssn
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_ba_bass_display_ga_result(retval);

    /* Reset the timer handle to NULL */
    appl_ga_bc_src_suspend_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

#endif /* BAP_BROADCAST */
