/**
 *  \file appl_ga_bc_sd.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast source exposed by BAP library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga_bc_sd.h"
#include "appl_ga_utils_hci.h"
#include "appl_ga_utils_audio_snk.h"

#ifdef BAP_BROADCAST

/* --------------------------------------------- Global Definitions */

static const char bc_sd_sink_menu_options[] = " \n\
================ GA BC 'SD Collocated Sink' OR 'BC Sink' MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
================ BC SD Collocated BC Sink ============ \n\
================ Service Management ~ Init ============ \n\
    2. SD Init. \n\
\n\
================ Service Management ~ Deinit ============ \n\
    3. SD Shutdown. \n\
\n\
================ Service Management ~ Setup ============ \n\
    4. Add Broadcast Receive State Char Instance. \n\
\n\
================ Device Management ~ LE Advertise ============ \n\
    5. Start SD Advertising. \n\
    6. Stop SD Advertising. \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   10. Send Broadcast Receive State Notification. \n\
\n\
================ Service Management ~ PAST and Streaming Procedures ============ \n\
   15. Request Sync Info (PAST - Available, Automated). \n\
   16. Scan and Associate to Broadcast Source (PAST - Not Available). \n\
   17. Dissociate from Broadcast Source and Stop Scanning. \n\
   18. Enable Stream. \n\
   19. Disable Stream. \n\
\n\
================ BC Sink ============ \n\
   25. Scan Announcement. \n\
   26. Scan End. \n\
   27. Associate Source. \n\
   28. Dissociate Source. \n\
   29. Enable Stream. \n\
   30. Disable Stream. \n\
   31. Set Broadcast Code (OOB). \n\
\n\
Your Option ?: ";

#define APPL_GA_BC_SD_MAX_NTF_DATA_SIZE         100U
/* When a BC Sink, the count of maximum number of scanned remote BC Sources */
#define APPL_GA_BC_SD_SNK_REM_SRC_DB_COUNT      30U
/* --------------------------------------------- Data types /Structures */

/* LE Periodic Advertising Sync Transfer Received */
typedef struct _HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_META_SUBEVENT_STRUCT_1
{
    /* Status: 1 */
    UCHAR status;

    /* Connection_Handle: 2 */
    UINT16 connection_handle;

    /* Service_Data: 2 */
    UINT16 service_data;

    /* Sync_Handle: 2 */
    UINT16 sync_handle;

    /* Advertising_SID: 1 */
    UCHAR advertising_sid;

    /* Advertiser_Address_Type: 1 */
    UCHAR advertiser_address_type;

    /* Advertiser_Address: 6 */
    UCHAR* advertiser_address;

    /* Advertiser_PHY: 1 */
    UCHAR advertiser_phy;

    /* Periodic_Advertising_Interval: 1 */
    UINT16 periodic_advertising_interval;

    /* Advertiser_Clock_Accuracy: 1 */
    UCHAR advertiser_clock_accuracy;

} HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_META_SUBEVENT_STRUCT_1;

/* Scan Delegator related structures */
#ifdef BAP_BROADCAST_SCAN_DELEGATOR

typedef struct _APPL_BC_SD_RX_STATE_INST
{
    /*
     * This maintaining of status of Src Add is required since the DB wont know
     * how many Char instances are added.
     * Solution is during Char Add, Create a DB Entry by marking a variable.
     */
    UINT8 isSrcAdded;
    UINT8 db_index;
    BASS_BC_RX_STATE_INST inst;
    /*
     * When Add/Modify source CP Opcode is received, Subgroup and Bis_Sync
     * will indicate the SG and BIS_Index to which we can sync to. This can
     * be used during Enable (case 18) for syncing.
     */
    UINT8 subgroup;
    UINT32 bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
}APPL_GA_BC_SD_RX_STATE_INST;

typedef struct _appl_ga_bc_sd_bass_local_db
{
    /* Connection Device Index */
    UINT8 conn_dev_index;
    /* Scan Delegator - Adv parameters */
    GA_BRR_EXTENDED_ADV_PARAMS sd_eadv_params;
    /* DB - Scan Delegator Receive State Instance Registered */
    APPL_GA_BC_SD_RX_STATE_INST rx_state_inst[BASS_SE_MAX_BC_RX_STATE_ENTITIES];
    /* Count of number of Receive State Instances Registered */
    UINT8 rx_state_inst_count;
    /* Remote Scan Status */
    UINT8 scan_status;
    /* BC Source Endpoint Info - PAST Not Available */
    GA_ENDPOINT bc_src_dev;
}APPL_GA_BC_SD_BASS_LOCAL_DB;

/*
 * This is used during CP Add handling. It is made global since it's updated
 * at couple of places.
 * Eg: Receive CP Add, Sync to PA, Sync to BIS
 */
typedef struct _APPL_BC_SD_BASS_BC_RX_STATE
{
    /* RX State Instance */
    GA_BASS_BC_RX_STATE  bc_rx_state;
    /* BIS Sync */
    UINT32 bc_rx_state_bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    /* Metadata Length */
    UINT8 bc_rx_state_metadata_len[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    /* Metadata */
    UINT8 bc_rx_state_metadata[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN];
}APPL_BC_SD_BASS_BC_RX_STATE;

#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
/* --------------------------------------------- External Global Variables */

/* BC role: SD or BA or BC Src or BC Sink */
extern UINT8 bc_role;
/* Used to differentiate ongoing advertise */
extern UINT8 is_adv_enabled;
/* Used to differentiate ongoing scan: Scanning for Broadcast Source */
extern UINT8 is_scan_enabled;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* BC_Sink: Parameters are used for Sink operations */
BC_SINK_PARAMS appl_ga_bc_sink_params;

/* SD: Parameters are used for Sink operations */
BC_SINK_PARAMS appl_ga_bc_sd_params;

/* When a BC Sink/SD, Current count of BC Src scanned */
UINT8 rem_src_db_count;
/* When a BC Sink/SD, DB of scanned BC Src */
GA_ENDPOINT rem_src_db[APPL_GA_BC_SD_SNK_REM_SRC_DB_COUNT];

/* BC Sink: Timer Handle to Disable Broadcast */
DECL_STATIC BT_timer_handle  appl_ga_bc_sd_snk_disable_timer_handle;

#ifdef BAP_BROADCAST_SCAN_DELEGATOR
/*
 * SD: DB - Remote BA Info + EADV Params + Local PACS +
 * BASS Rx State Instance (Subgroup + BIS_Sync) + BC_Src address
 */
DECL_STATIC APPL_GA_BC_SD_BASS_LOCAL_DB appl_ga_bc_sd_bass_local_db;

/* SD: Timer Handle to send BASS Notifications */
DECL_STATIC BT_timer_handle  appl_ga_bc_sd_cp_ntf_timer_handle;

/*
 * SD: Timer Handle to verify if PAST is received from BA once PAST Available
 * is received as part of BASS Add/Modify CP.
 * If the PAST is not received within the timeout, we are sending Notification
 * to the BA with PA_Sync_State as No PAST(BASS_PA_SYNC_STATE_NOPAST).
 */
DECL_STATIC BT_timer_handle  appl_ga_bc_sd_past_rx_timer_handle;

/* Global Rx_State variable used for BASS operations */
APPL_BC_SD_BASS_BC_RX_STATE  appl_ga_bc_sd_bass_bc_rx_state;

/* SD: Data to be sent during notification */
UCHAR appl_ga_bc_sd_ntf_data[APPL_GA_BC_SD_MAX_NTF_DATA_SIZE];
/* SD: Notification data size */
UINT8 appl_ga_bc_sd_ntf_data_size;

#endif /* BAP_BROADCAST_SCAN_DELEGATOR */

#ifdef APPL_BAP_BROADCAST_SINK

/* BC Sink: Save the Broadcast Code for encrypted BIS */
UCHAR appl_ga_bc_snk_bc_code[BASS_BC_CODE_MAX_LEN];
UCHAR * appl_ga_bc_snk_bc_code_ptr = NULL;

#endif /* APPL_BAP_BROADCAST_SINK */

void appl_ga_bc_sd_snk_handle_setup_iso_data_path_event
     (
         UINT8 start_audio_lc3,
         UINT8 status,
         UINT8 sg_info_index,
         UINT8 bis_info_index
     );

void appl_ga_bc_sd_snk_handle_remove_iso_data_path_event
     (
         UINT8 status,
         UINT8 sg_info_index,
         UINT8 bis_info_index
     );

GA_RESULT appl_ga_bc_bass_sd_cb
          (
              /* IN */ GA_ENDPOINT            * device,
              /* IN */ GA_BASS_SE_RSP_CONTEXT * ctx,
              /* IN */ BASS_SE_EVT_ID           evt_id,
              /* IN */ UINT16                   evt_status,
              /* IN */ BASS_EVT               * evt_data
          );

void appl_ga_bc_sd_set_default_adv_params(void);

void appl_ga_bc_sd_setup_adv(UINT8 enable);

/* Return the number of bytes packed */
UINT8 appl_ga_bc_sd_pack_rx_state_ntf_data
      (
        /* IN */    GA_BASS_BC_RX_STATE_SRC_INFO rx_state_info,
        /* INOUT */ UCHAR * ntf_data
      );

GA_RESULT appl_ga_bc_sd_req_sync_info_past(void);

GA_RESULT appl_ga_bc_sd_parse_and_display_bc_rx_state
          (
              /* IN */ GA_BASS_BC_RX_STATE br_rx_state_info
          );

void appl_ga_bc_sd_parse_and_display_addr_type(/* IN */ UINT8 addr_type);

void appl_ga_bc_sd_parse_and_display_PA_Sync_State
     (
        /* IN */ UINT8 pa_sync_state
     );

void appl_ga_bc_sd_parse_and_display_BIG_encryption
     (
        /* IN */ UINT8 big_encryption
     );

void appl_ga_bc_sd_parse_and_display_PA_Sync(/* IN */ UINT8 pa_sync);

GA_RESULT appl_ga_bc_sd_parse_and_handle_bass_cp
          (
            /* IN */ UINT8       * bas_cp,
            /* IN */ UINT16        len,
            /* IN */ GA_ENDPOINT * device
          );

GA_RESULT appl_ga_bc_sd_is_src_add_allowed(/* IN */ UINT8 * id);

UINT16 appl_ga_bc_sd_parse_and_display_add_src_data
       (
           /* IN */    UINT8                  * bas_cp,
           /* INOUT */ GA_BASS_BAS_CP_ADD_SRC * add_src,
           /* INOUT */ UINT32                 * bis_sync,
           /* INOUT */ UINT8                  * metadata_len,
           /* INOUT */ UINT8                  * metadata
       );

GA_RESULT appl_ga_bc_sd_handle_add_src_cp
          (
              /* IN */ UINT8 * add_src_data,
              /* IN */ UINT16  len
          );

GA_RESULT appl_ga_bc_sd_copy_add_src_data_to_bc_rx_state
          (
              /* IN */ GA_BASS_BAS_CP_ADD_SRC  add_src,
              /* IN */ UINT8                   index
          );

GA_RESULT appl_ga_bc_sd_handle_modify_src_cp
          (
              /* IN */ UINT8  * modify_src_data,
              /* IN */ UINT16  len
          );

UINT16 appl_ga_bc_sd_parse_and_display_modify_src_data
       (
           /* IN */    UINT8                     * bas_cp,
           /* INOUT */ GA_BASS_BAS_CP_MODIFY_SRC * modify_src,
           /* INOUT */ UINT32                    * bis_sync,
           /* INOUT */ UINT8                     * metadata_len,
           /* INOUT */ UINT8                     * metadata
       );

GA_RESULT appl_ga_bc_sd_copy_modify_src_data_to_bc_rx_state
          (
              /* IN */ GA_BASS_BAS_CP_MODIFY_SRC modify_src,
              /* IN */ UINT8                     index,
              /* IN */ UINT8                   * notify_rx_state
          );

GA_RESULT appl_ga_bc_sd_handle_set_bc_code_cp
          (
            /* IN */ UINT8 * set_bc_code_data
          );

GA_RESULT appl_ga_bc_sd_handle_rem_src_cp(/* IN */ UINT8 source_id);

void appl_ga_bc_sd_cp_ntf_timer_handler
     (
        /* IN */ void * data,
        /* IN */ UINT16 datalen
     );

void appl_ga_bc_sd_past_rx_timer_handler
     (
        /* IN */ void * data,
        /* IN */ UINT16 datalen
     );

void appl_ga_bc_sd_update_conn_dev_index(GA_ENDPOINT * device);

GA_RESULT appl_ga_bc_sd_snk_setup_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          );
GA_RESULT appl_ga_bc_sd_snk_remove_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          );

void appl_ga_bc_sd_bass_display_ga_result(GA_RESULT status);

void appl_ga_bc_sd_snk_add_rem_src_to_db(UINT8* addr, UINT8 addr_type);
void appl_ga_bc_sd_snk_clear_rem_src_db(void);
UINT8 appl_ga_bc_sd_snk_is_rem_src_in_db(UINT8* addr, UINT8 addr_type);

GA_RESULT appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
          (
              UINT8   bis_index,
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          );
void appl_ga_bc_sd_extract_bis_indeces_from_bis_sync
     (
         UINT32  bis_sync,
         UINT8 * bis_indeces,
         UINT8 * bis_count
     );

void appl_ga_bc_sd_get_input_for_enable(void);

GA_RESULT appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
          (
              GA_ENDPOINT  device,
              UINT8      * bc_rx_state_inst,
              UINT8      * db_index
          );

GA_RESULT appl_ga_bc_sd_validate_for_duplicate_bis_index_in_subgroups
          (
              UINT8    num_subgroups,
              UINT32 * bis_sync
          );

GA_RESULT appl_ga_bc_sd_update_rx_state_and_notify
          (
              UINT8  db_index,
              UINT8  bc_rx_state_inst,
              UINT8  send_notification
          );
void appl_ga_bc_reset_on_disable(void);
void appl_ga_bc_sd_update_bis_sync_state_and_notify(UINT8 enable);

void appl_ga_bc_sd_snk_disable_timer_handler
     (
         void  * data,
         UINT16  datalen
     );

GA_RESULT appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle
          (
              UINT16   bis_conn_handle,
              UINT8  * sg_info_index,
              UINT8  * bis_info_index
          );

#ifdef BASS_DB
BASS_BC_RX_STATE_INST appl_ga_bc_sd_fetch_rx_state_inst_from_src_id
                      (
                          UINT8 src_id
                      );
#endif /* BASS_DB */
/* --------------------------------------------- APIs */

GA_RESULT appl_ga_bc_sd_snk_update_receiving_audio
          (
              UINT16                     bis_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8                      bps,
              UINT8                      status
          );

#define appl_ga_bc_sd_snk_start_receiving_audio(bis_handle, cs_conf, bps) \
        appl_ga_bc_sd_snk_update_receiving_audio(bis_handle, cs_conf, bps, GA_TRUE)

#define appl_ga_bc_sd_snk_stop_receiving_audio(bis_handle, cs_conf, bps) \
        appl_ga_bc_sd_snk_update_receiving_audio(bis_handle, cs_conf, bps, GA_FALSE)

GA_RESULT appl_ga_bc_sd_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          )
{
    GA_RESULT         retval;
    UINT8             value_1;
    UINT16            value_2;
    UINT8             status;
    UINT8             marker;
    UINT8             sub_event;
    GA_ENDPOINT       device;
    UINT16            conn_handle;
    UINT8             sg_info_index;
    UINT8             bis_info_index;
    UINT8             start_audio_lc3;
    BC_SINK_PARAMS  * snk_param;
    UINT8             db_index;
    UINT8             past_rx_timer_params[2U];
    BASS_BC_RX_STATE_INST  bc_rx_state_inst;


    /* Initialize */
    retval = GA_SUCCESS;
    value_1 = 0U;
    value_2 = 0U;
    status = 0U;
    marker = 0U;
    sub_event = 0U;
    conn_handle = 0U;
    sg_info_index = 0U;
    bis_info_index = 0U;
    start_audio_lc3 = GA_FALSE;
    db_index = 0U;

    if (!((IS_APPL_GA_BC_ROLE_SD(bc_role)) ||
        (IS_APPL_GA_BC_ROLE_BC_SINK(bc_role))))
    {
        return GA_SUCCESS;
    }

    APPL_DBG("[APPL][GA][BC_SD]: >> appl_ga_sd_ba_hci_cb_handler\n");

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else
    {
        snk_param = &appl_ga_bc_sink_params;
    }

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
                event_datalen -= 4U;

                switch (value_2)
                {
                /* TODO: Parse and display below events */
                case HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE:
                    APPL_TRC("HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE\n");
                    GA_UNPACK_LE_1_BYTE(&value_1, &event_data[marker]);
                    marker++;
                    event_datalen--;
                    APPL_TRC("Selected Tx Power: 0x%02X\n", value_1);
                    break;

                case HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE:
                    APPL_TRC("HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE\n");
                    break;

                case HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE:
                    APPL_TRC("HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE\n");
                    break;

                case HCI_LE_SET_HOST_FEATURE_OPCODE:
                    break;

                case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE:
                    hci_unpack_2_byte_param(&conn_handle, &event_data[marker]);
                    marker += 2U;
                    /* Fetch the SG Info Index and BIS Info Index from the BIS Handle */
                    if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle
                            (
                                conn_handle,
                                &sg_info_index,
                                &bis_info_index
                            ))
                    {
                        if (snk_param->bis_index_active_playback ==
                                snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index)
                        {
                            /*
                             * BIS index for the received BIS Handle is the
                             * Active Playback BIS index. Hence, create LC3
                             * module and start Audio_PL
                             */
                            start_audio_lc3 = GA_TRUE;
                        }
                        else
                        {
                            start_audio_lc3 = GA_FALSE;
                        }
                        appl_ga_bc_sd_snk_handle_setup_iso_data_path_event
                        (
                            start_audio_lc3,
                            status,
                            sg_info_index,
                            bis_info_index
                        );
                    }
                    break;

                case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE:
                    hci_unpack_2_byte_param(&conn_handle, &event_data[marker]);
                    marker += 2U;
                    /* Fetch the SG Info Index and BIS Info Index from the BIS Handle */
                    if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle
                            (
                                conn_handle,
                                &sg_info_index,
                                &bis_info_index
                            ))
                    {
                        appl_ga_bc_sd_snk_handle_remove_iso_data_path_event
                        (
                            status,
                            sg_info_index,
                            bis_info_index
                        );
                    }
                    break;

                case HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE:
                    APPL_TRC
                    (
                        "HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE\n"
                    );
                    /* Connection_Handle */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &conn_handle,
                        &event_data[marker]
                    );
                    marker += 1U;
                    /* Status */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &status,
                        &event_data[marker]
                    );
                    APPL_TRC("Connection_Handle: 0x%X\n", conn_handle);
                    APPL_TRC("Status: 0x%X\n", status);

                    if (0x00U == status)
                    {
                        /*
                         * Update the PA_Sync_State to
                         * BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST as the status
                         * is success.
                         */
                        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                            BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST;

                        /*
                         * Fetch the Rx_State_Instance for this source to update
                         * PA_Sync_State in BASS_DB.
                         */
                        if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                                          (
                                              appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                              &bc_rx_state_inst,
                                              &db_index
                                          ))
                        {
#ifdef BASS_DB
                            /* Update the Rx_State_Instance in BASS_DB */
                            APPL_TRC("BASS_DB_modify_source()...\n");
                            retval = BASS_DB_modify_source
                                     (
                                         db_index,
                                         &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
                                     );
                            APPL_TRC("Retval - ");
                            appl_ga_bc_sd_bass_display_ga_result(retval);
#endif /* BASS_DB */
                            past_rx_timer_params[0U] = bc_rx_state_inst;
                            past_rx_timer_params[1U] = db_index;

                            /* Send Notification */
                            /*
                             * NOTE:
                             * Timeout = 9sec
                             * (4sec Add_Source timeout + 5sec PAST receiving timeout)
                             */
                            BT_start_timer
                            (
                                &appl_ga_bc_sd_past_rx_timer_handle,
                                9U, /* Delay of 9 seconds */
                                appl_ga_bc_sd_past_rx_timer_handler,
                                &past_rx_timer_params[0U],
                                sizeof(BASS_BC_RX_STATE_INST) + sizeof(UINT8)
                            );
                        }
                    }
                    break;

                default:
                    break;
                }
            }
            break;

    case HCI_LE_META_EVENT:
        /* Extract the subevent code */
        GA_UNPACK_LE_1_BYTE(&sub_event, &event_data[marker]);
        marker++;
        event_datalen--;

        switch (sub_event)
        {
            case HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_SUBEVENT:
            {
                APPL_TRC
                (
                    "HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_SUBEVENT\n"
                );
                appl_dump_bytes(&event_data[marker], event_datalen);
                HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_META_SUBEVENT_STRUCT_1 le_periodic_advertising_sync_transfer_received_subevent;

                /* Status */
                GA_UNPACK_LE_1_BYTE
                (
                    &(le_periodic_advertising_sync_transfer_received_subevent.status),
                    &event_data[marker]
                );
                marker++;

                if (0x00U == le_periodic_advertising_sync_transfer_received_subevent.status)
                {
                    /* Connection_Handle */
                    GA_UNPACK_LE_2_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.connection_handle),
                        &event_data[marker]
                    );
                    marker += 2U;
                    /* Service_Data */
                    GA_UNPACK_LE_2_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.service_data),
                        &event_data[marker]
                    );
                    marker += 2U;
                    /* Sync_Handle */
                    GA_UNPACK_LE_2_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.sync_handle),
                        &event_data[marker]
                    );
                    marker += 2U;
                    /* SD: Copying Sync Handle for streaming */
                    appl_ga_bc_sd_params.bcast_synchandle =
                        le_periodic_advertising_sync_transfer_received_subevent.sync_handle;
                    /* Advertising_SID */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.advertising_sid),
                        &event_data[marker]
                    );
                    marker++;
                    /* Advertiser_Address_Type */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.advertiser_address_type),
                        &event_data[marker]
                    );
                    marker++;
                    device.bd_type =
                        le_periodic_advertising_sync_transfer_received_subevent.advertiser_address_type;
                    /* Advertiser_Address */
                    le_periodic_advertising_sync_transfer_received_subevent.advertiser_address =
                        &event_data[marker];
                    marker += 6U;
                    GA_COPY_BRR_DEV_ADDR
                    (
                        device.bd_addr,
                        le_periodic_advertising_sync_transfer_received_subevent.advertiser_address
                    );
                    /* Advertiser_PHY */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.advertiser_phy),
                        &event_data[marker]
                    );
                    marker++;
                    /* Periodic_Advertising_Interval */
                    GA_UNPACK_LE_2_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.periodic_advertising_interval),
                        &event_data[marker]
                    );
                    marker += 2U;
                    /* Advertiser_Clock_Accuracy */
                    GA_UNPACK_LE_1_BYTE
                    (
                        &(le_periodic_advertising_sync_transfer_received_subevent.advertiser_clock_accuracy),
                        &event_data[marker]
                    );
                    marker++;

                    /* Stop the PAST Rx Timer as we have received PAST from BA */
                    if (BT_TIMER_HANDLE_INIT_VAL != appl_ga_bc_sd_past_rx_timer_handle)
                    {
                        BT_stop_timer
                        (
                            appl_ga_bc_sd_past_rx_timer_handle
                        );
                    }

                    /* *event_substructure = le_periodic_advertising_sync_transfer_received_subevent; */
                    APPL_TRC("GA_broadcast_update_associate_info()...\n");
                    retval = GA_broadcast_update_associate_info
                             (
                                 le_periodic_advertising_sync_transfer_received_subevent.advertising_sid,
                                 &device,
                                 appl_ga_bc_sd_params.bcast_synchandle
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        /*
                         * Copy bc_src_dev info
                         * Used for verification during sending notification during
                         * GA_BC_SOURCE_STREAM_IND event
                         */
                        GA_COPY_BRR_DEV_ADDR
                        (
                            &appl_ga_bc_sd_bass_local_db.bc_src_dev.bd_addr,
                            &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr
                        );
                        appl_ga_bc_sd_bass_local_db.bc_src_dev.bd_type =
                            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr_type;
                    }
                }
                /*
                 * This event gets triggered in below cases:
                 * 1. PAST Available, Mode: 2 or 3(PA reports enabled)
                 * 2. PAST Not Available, Manual Trigger of Associate.
                 * In case 2, Update of pa_sync_state shall be handled in
                 * GA_BC_SOURCE_CONFIG_IND event.
                 * Case 1, Update of pa_sync_state shall be handled below.
                 */
                /*
                 * Fetch the Rx State Instance for this source before trigerring
                 * Notification stating the indication of encryption status.
                 */
                if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                                  (
                                      appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                      &bc_rx_state_inst,
                                      &db_index
                                  ))
                {
                    if (BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST ==
                        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state)
                    {
                        if (GA_SUCCESS == le_periodic_advertising_sync_transfer_received_subevent.status)
                        {
                            /*
                             * Update the rx_state_inst PA_Sync_State to
                             * Synchronized to PA (0x02) as SD is synced to BC Src PA.
                             */
                            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                                BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA;
                        }
                        else
                        {
                            /*
                             * Update the RX_State_Inst PA_Sync_State to
                             * BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA (0x03) as
                             * SD has failed to sync to Broadcast_Src PA.
                             */
                            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                                BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA;
                        }
                        /* Update and notify */
                        (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                                (
                                                    db_index,
                                                    bc_rx_state_inst,
                                                    GA_TRUE
                                                );
                    }
                }
                else
                {
                    APPL_ERR("Invalid Device !");
                }
            }
            break;

            default:
                /* Do Nothing */
                break;
        }

        default:
            /* Do Nothing */
            break;
    }

    APPL_DBG("[APPL][GA][BC_SD]: << appl_ga_sd_ba_hci_cb_handler\n");
    return GA_SUCCESS;
}

void appl_ga_bc_sd_snk_handle_setup_iso_data_path_event
     (
         UINT8 start_audio_lc3,
         UINT8 status,
         UINT8 sg_info_index,
         UINT8 bis_info_index
     )
{
    BC_SINK_PARAMS * snk_param;

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    if (0x00U == status)
    {
        if (AUDIO_ISO_UPDATE_IN_PROGRESS == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_snk_update_entry
                              (
                                  snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                  AUDIO_ISO_DATA_SETUP,
                                  status,
                                  AUDIO_IGNORE_VALUE
                              ))
            {
                APPL_TRC
                (
                    "[APPL][GA][%s]: ISO Data Path Setup Status: Complete\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
                /* Update the ISO state */
                snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_SETUP_COMPLETE;

                if (GA_TRUE == start_audio_lc3)
                {
                    /* We are Audio Sink, Start Receiving Audio data */
                    if (GA_SUCCESS == appl_ga_bc_sd_snk_start_receiving_audio
                                      (
                                          snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_TRUE;
                    }
                }
            }
            /*
             * TODO: Check
             * As the event status is 0x00, printing that
             * update_entry failed, because setup failed is printed when
             * status is not 0x00.
             */
            else
            {
                APPL_ERR
                (
                    "[APPL][GA][%s]: ISO Data Path Setup Status: Update entry failed !\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
            }
        }
    }
    else
    {
        APPL_ERR
        (
            "[APPL][GA][%s]: ISO Data Path Setup Status: Failed !\n",
            SD_GET_ROLE_STRING(bc_role)
        );
    }

    return;
}

void appl_ga_bc_sd_snk_handle_remove_iso_data_path_event
     (
         UINT8 status,
         UINT8 sg_info_index,
         UINT8 bis_info_index
     )
{
    BC_SINK_PARAMS * snk_param;

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    if (0x00U == status)
    {
        if (AUDIO_ISO_SETUP_COMPLETE == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_snk_remove_entry
                              (
                                  snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                              ))
            {
                APPL_TRC
                (
                    "[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
                /* Update the ISO state */
                snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_IDLE;
            }
            /*
             * TODO: Check
             * As the event status is 0x00, printing that
             * remove_entry failed, because setup failed is printed when
             * status is not 0x00.
             */
            else
            {
                APPL_ERR
                (
                    "[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
            }
        }
    }
    else
    {
        APPL_ERR
        (
            "[APPL][GA][%s]: ISO Data Path Removal Status: Failed !\n",
            SD_GET_ROLE_STRING(bc_role)
        );
    }

    return;
}

GA_RESULT appl_ga_bc_bass_sd_cb
          (
              /* IN */ GA_ENDPOINT            * device,
              /* IN */ GA_BASS_SE_RSP_CONTEXT * ctx,
              /* IN */ BASS_SE_EVT_ID           evt_id,
              /* IN */ UINT16                   evt_status,
              /* IN */ BASS_EVT               * evt_data
          )
{
    APPL_DBG("[APPL][GA][BC_SD]: >> appl_ga_bc_bass_sd_cb\n");

    GA_RESULT  retval;
    UINT8      send_rsp;
    void     * rsp;
    UINT16     rsp_len;
    UCHAR      i;

#ifdef BASS_DB
    GA_BASS_BC_RX_STATE rx_state = { 0 };
#endif /* BASS_DB */

    /* Initialize */
    send_rsp = GA_FALSE;
    retval = GA_SUCCESS;
    rsp = NULL;
    rsp_len = 0U;
    i = 0U;

    GA_CB_ENTRY("BC BASS Scan Delegator");

    if ((NULL != device) && (NULL != evt_data))
    {
        send_rsp = GA_TRUE;

        switch (evt_id)
        {
        case BASS_SE_READ_BC_RX_STATE_IND:
            appl_ga_bc_sd_update_conn_dev_index(device);
            APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
            APPL_TRC("[SECTION]     : BASS SERVER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : BASS SERVER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_ga_bc_sd_bass_display_ga_result(evt_status);
            APPL_TRC
            (
                "[Event]       : BASS_SE_READ_BC_RX_STATE_IND (0x%02X)\n",
                evt_id
            );
            APPL_TRC("[Service Inst]: 0x%02X\n", evt_data->bass_handle);

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

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            for (i = 0U; i < GA_CONFIG_LIMITS(BASS_SE_MAX_BC_RX_STATE_ENTITIES); i++)
            {
                if (appl_ga_bc_sd_bass_local_db.rx_state_inst[i].inst == evt_data->rx_state_inst)
                {
                    retval = GA_SUCCESS;
                    break;
                }
            }
            if ((GA_SUCCESS != retval) || (BASS_SE_MAX_BC_RX_STATE_ENTITIES == i))
            {
                APPL_ERR("Could not find the Rx State Instance !\n");
            }
            else
            {
                if (GA_TRUE == appl_ga_bc_sd_bass_local_db.rx_state_inst[i].isSrcAdded)
                {
#ifdef BASS_DB
                    if (GA_SUCCESS == BASS_DB_fetch_rx_state_instance
                                      (
                                          appl_ga_bc_sd_bass_local_db.rx_state_inst[i].db_index,
                                          &rx_state
                                      ))
                    {
                        APPL_TRC("Data Length: 0x%02X\n", rx_state.info_len);
                        APPL_TRC("Data:\n");
                        retval = appl_ga_bc_sd_parse_and_display_bc_rx_state
                                 (
                                     rx_state
                                 );
                        /* Initializing appl_ga_bc_sd_ntf_data_size */
                        appl_ga_bc_sd_ntf_data_size = 0U;
                        /* Pack the data for response and also get the rsp data size */
                        appl_ga_bc_sd_ntf_data_size = appl_ga_bc_sd_pack_rx_state_ntf_data
                                                      (
                                                          rx_state.info,
                                                          &appl_ga_bc_sd_ntf_data[0U]
                                                      );
                        /* Fill the response */
                        rsp = appl_ga_bc_sd_ntf_data;
                        rsp_len = appl_ga_bc_sd_ntf_data_size;
                        appl_dump_bytes((UCHAR*)rsp, rsp_len);
                    }
                    else
                    {
                        APPL_ERR("Could not get Broadcast Receive State !\n");
                        retval = GA_FAILURE;
                    }
#else /* BASS_DB */
                    APPL_ERR("BASS_DB is undefined !\n");
#endif /* BASS_DB */
                }
                else
                {
                    APPL_TRC("Broadcast Receive State Instance is empty\n");
                }
            }
            break;

        case BASS_SE_WRITE_BAS_CP_IND:
            appl_ga_bc_sd_update_conn_dev_index(device);
            APPL_TRC("[Profile]     : BASS (0x%04X)\n", GA_SERVICE_BASS);
            APPL_TRC("[SECTION]     : BASS SERVER EVENTS\n");
            APPL_TRC("[SUB-SECTION] : BASS SERVER-WRITE\n");
            if ((NULL != ctx) && (!ctx->to_rsp))
            {
                APPL_TRC("[TYPE]        : WRITE COMMAND\n");
                send_rsp = GA_FALSE;
            }
            else
            {
                APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            }
            appl_ga_utils_display_bdaddr_and_type(device);
            APPL_TRC("[Event Status]: ");
            appl_ga_bc_sd_bass_display_ga_result(evt_status);
            APPL_TRC
            (
                "[Event]       : BASS_SE_WRITE_BAS_CP_IND (0x%02X)\n",
                evt_id
            );
            APPL_TRC("[Service Inst]: 0x%02X\n", evt_data->bass_handle);

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Broadcast Audio Scan Control Point (0x%04X)\n",
                GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP
            );
            GA_FORMAT_DISPLAY();

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("RECEIVED DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", evt_data->len);

            /* Parse the BAS CP */
            if ((NULL != evt_data->data) && (0U != evt_data->len))
            {
                APPL_TRC("Data:\n");
                retval = appl_ga_bc_sd_parse_and_handle_bass_cp
                         (
                             evt_data->data,
                             evt_data->len,
                             device
                         );
            }
            else
            {
                APPL_TRC("Data: NULL\n");
                retval = GA_FAILURE;
                APPL_ERR("BAS CP Param is NULL !\n");
            }
            if ((NULL != ctx) && (ctx->to_rsp))
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
            break;

        default:
            APPL_ERR("Unknown Event ID: 0x%02X !\n", evt_id);
            send_rsp = GA_FALSE;
            break;
        }
    }
    else
    {
        retval = GA_FAILURE;
        APPL_ERR("device is NULL !\n");
    }

    if (GA_TRUE == send_rsp)
    {
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING BC BASS RESPONSE\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("API   : GA_bass_se_send_rsp\n");
        retval = GA_bass_se_send_rsp
                 (
                     device,
                     ctx,
                     evt_id,
                     retval,
                     rsp,
                     rsp_len
                 );
        APPL_TRC("Retval - ");
        appl_ga_bc_sd_bass_display_ga_result(retval);
    }

    GA_CB_EXIT();

    APPL_DBG("[APPL][GA][BC_SD]: << appl_ga_bc_bass_sd_cb\n");

    return retval;
}

GA_RESULT appl_ga_bc_sd_sink_cb_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          )
{
    APPL_DBG("[APPL][GA][BC_SD]: >> appl_ga_bc_sd_sink_cb_handler\n");

    GA_RESULT              retval;
    UINT8                  i;
    UINT8                  j;
    UINT32                 delay;
    UINT8                  n_sg;
    UINT8                  n_bis;
    UINT8                  parsed_len;
    UINT32                 value_4;
    BASS_BC_RX_STATE_INST  bc_rx_state_inst;
    UINT8                  sg_info_index;
    UINT8                  bis_info_index;
    UINT8                  db_index;
    UINT8                  is_notify;

    BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS  bc_source_announcement = { 0 };
    BAP_BROADCAST_ANNOUNCEMENT         bc_annoucement = { 0 };
    BAP_BC_ASSOCIATE_PARAMS            bc_associate = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_1     bc_basic_announcement_1 = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_2     bc_basic_announcement_2 = { 0 };
    BAP_BC_SOURCE_CONFIG_PARAMS        bc_source_config = { 0 };
    BAP_BC_SOURCE_STREAM_PARAMS        bc_source_stream = { 0 };
    BAP_BC_DISSOCIATE_PARAMS           bc_dissociate = { 0 };
    BAP_BC_ENABLE_PARAMS               bc_enable = { 0 };
    BAP_BC_DISABLE_PARAMS              bc_disable = { 0 };

    GA_LC3_CODEC_SPECIFIC_CONF rem_cs_conf = { 0 };

    UINT8  prev_metadata_len;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    j = 0U;
    delay = 0U;
    n_sg = 0U;
    sg_info_index = 0U;
    bis_info_index = 0U;
    parsed_len = 0U;
    prev_metadata_len = 0U;
    value_4 = 0U;
    bc_rx_state_inst = 0U;
    db_index = 0U;
    is_notify = GA_FALSE;

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
                /* Do not allow printing of the events multiple times */
                if (GA_FALSE == appl_ga_bc_sd_snk_is_rem_src_in_db
                                (
                                    bc_source_announcement.address,
                                    bc_source_announcement.address_type
                                ))
                {
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
                        bc_source_announcement.address[0U],
                        bc_source_announcement.address[1U],
                        bc_source_announcement.address[2U],
                        bc_source_announcement.address[3U],
                        bc_source_announcement.address[4U],
                        bc_source_announcement.address[5U]
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
                        bc_source_announcement.direct_address[0U],
                        bc_source_announcement.direct_address[1U],
                        bc_source_announcement.direct_address[2U],
                        bc_source_announcement.direct_address[3U],
                        bc_source_announcement.direct_address[4U],
                        bc_source_announcement.direct_address[5U]
                    );
                    APPL_TRC("Data Length: 0x%02X\n", bc_source_announcement.datalen);

                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Broadcast_ID: 0x%06X\n",
                        BASS_BC_ID_LEN, bc_annoucement.broadcast_id
                    );
                    appl_ga_utils_parse_and_display_adv_data
                    (
                        bc_source_announcement.data,
                        bc_source_announcement.datalen
                    );
                    appl_ga_bc_sd_snk_add_rem_src_to_db
                    (
                        bc_source_announcement.address,
                        bc_source_announcement.address_type
                    );

                    /* TODO: Move the below procedure to an API */
                    /*
                     * Below procedure is for SD.
                     * When BASS CP Add Source is called with PAST_Not_Available
                     * and when SD has to sync to the Broadcast_Src by itself
                     */
                    /* Check if SD has triggered scanning. */
                    if (GA_TRUE == appl_ga_bc_sd_bass_local_db.scan_status)
                    {
                        GA_ENDPOINT bc_src_device;
                        /* If the Addr_Type is random, then compare the
                         * SID of the Broadcast_Src with the data from
                         * Add Source operation on SD.
                         */
                        if ((BASS_ADDR_TYPE_RANDOM == appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr_type)
                            &&
                            (BASS_ADDR_TYPE_RANDOM == bc_source_announcement.address_type))
                        {
                            /* If the SID matches, then check for Broadcast_ID. */
                            if (bc_source_announcement.advertising_sid ==
                            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.adv_sid)
                            {
                                /*
                                 * Compare the Broadcast_ID of the
                                 * Broadcast_Src with the data from Add Source
                                 * operation on SD.
                                 */

                                /*
                                 * If the Broadcast_ID matches, then sync
                                 * to the Broadcast_Src.
                                 */
                                UINT32 bc_id;

                                GA_UNPACK_LE_3_BYTE
                                (
                                    &bc_id,
                                    &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.broadcast_ID[0U]
                                );

                                if (bc_annoucement.broadcast_id == bc_id)
                                {
                                    bc_src_device.bd_type =
                                        bc_source_announcement.address_type;
                                    for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
                                    {
                                        bc_src_device.bd_addr[i] =
                                            bc_source_announcement.address[i];
                                    }
                                    APPL_TRC("Syncing to the broadcast_src\n");
                                }
                                else
                                {
                                    /* Device not interested in! Skip! */
                                    break;
                                }
                            }
                            else
                            {
                                /* Device not interested in! Skip! */
                                break;
                            }
                        }
                        else
                        {
                            /*
                             * If the Addr_Type is public, then check for SID,
                             * Broadcast_ID and Address.
                             */
                            if (GA_COMPARE_BRR_DEV_ADDR
                                (
                                    &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr[0U],
                                    &bc_source_announcement.address[0U]
                                )
                                &&
                                GA_COMPARE_BRR_DEV_ADDR_TYPE
                                (
                                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr_type,
                                    bc_source_announcement.address_type
                                ))
                            {
                                /* If the SID matches, then check for Broadcast_ID. */
                                if (bc_source_announcement.advertising_sid ==
                                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.adv_sid)
                                {
                                    UINT32 bc_id;

                                    GA_UNPACK_LE_3_BYTE
                                    (
                                        &bc_id,
                                        &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.broadcast_ID[0U]
                                    );

                                    if (bc_annoucement.broadcast_id == bc_id)
                                    {
                                        /*
                                         * If the Broadcast_ID matches, then sync
                                         * to the Broadcast_Src.
                                         */
                                        bc_src_device.bd_type =
                                            bc_source_announcement.address_type;
                                        for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
                                        {
                                            bc_src_device.bd_addr[i] =
                                                bc_source_announcement.address[i];
                                        }
                                        APPL_TRC("Syncing to the broadcast_src\n");
                                    }
                                    else
                                    {
                                        /* Device not interested in! Skip! */
                                        break;
                                    }
                                }
                                else
                                {
                                    /* Device not interested in! Skip! */
                                    break;
                                }
                            }
                            else
                            {
                                /* Device not interested in! Skip! */
                                break;
                            }
                        }
                        /*
                         * Copy bc_src_dev info.
                         * Used for verification during sending notification in
                         * GA_BC_SOURCE_STREAM_IND event
                         */
                        GA_COPY_BRR_DEVICE
                        (
                            &appl_ga_bc_sd_bass_local_db.bc_src_dev,
                            &bc_src_device
                        );
                        APPL_TRC("GA_broadcast_associate...\n");
                        retval = GA_broadcast_associate
                                 (
                                     bc_source_announcement.advertising_sid,
                                     &bc_src_device
                                 );
                        APPL_TRC("Retval - ");
                        appl_ga_bc_sd_bass_display_ga_result(retval);
                    }
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
            /* For testing. TODO: check what to pass for stream enable. */
            appl_ga_bc_sd_params.bcast_synchandle =
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
                bc_associate.advertiser_addr[0U],
                bc_associate.advertiser_addr[1U],
                bc_associate.advertiser_addr[2U],
                bc_associate.advertiser_addr[3U],
                bc_associate.advertiser_addr[4U],
                bc_associate.advertiser_addr[5U]
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
                if (GA_TRUE == appl_ga_bc_sd_bass_local_db.scan_status)
                {
                    /* SD: During PAST Not available, SD would have triggered
                     * PAST by itself, once associated Stop Scan.
                     */
                    if (APPL_GA_SCAN_FOR_BC_SRC == is_scan_enabled)
                    {
                        APPL_TRC("Release Broadcast Sink...\n");
                        retval = GA_broadcast_scan_end();
                        APPL_TRC("Retval - ");
                        appl_ga_bc_sd_bass_display_ga_result(retval);
                        if (GA_SUCCESS == retval)
                        {
                            is_scan_enabled = APPL_GA_SCAN_INVALID;
                            appl_ga_bc_sd_bass_local_db.scan_status = GA_FALSE;
                        }
                    }
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
            if (appl_ga_bc_sd_params.config_synchandle
                == appl_ga_bc_sd_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }
            GA_CB_ENTRY("BC CAP Server");
            appl_ga_bc_sd_params.config_synchandle =
                appl_ga_bc_sd_params.bcast_synchandle;

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
                appl_ga_bc_sd_params.bc_num_subgroups = n_sg;

                /* Presentation_Delay + Num_Subgroups is parsed */
                bc_source_config.data += parsed_len;

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
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bc_nstream =
                            n_bis;
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
                        GA_mem_copy
                        (
                            &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bc_basic_announcement_1.codec,
                            &bc_basic_announcement_1.codec,
                            sizeof(GA_CODEC_INFO)
                        );
                        GA_mem_copy
                        (
                            &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].cs_conf,
                            &rem_cs_conf,
                            sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                        );

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Metadata_Length: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_basic_announcement_1.meta.length
                        );

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
                        }
                        else
                        {
                            APPL_TRC
                            (
                                "Len: 0x%02X,  Metadata: NULL\n",
                                bc_basic_announcement_1.meta.length
                            );
                        }
                        GA_mem_copy
                        (
                            &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bc_basic_announcement_1.meta,
                            &bc_basic_announcement_1.meta,
                            sizeof(GA_METADATA)
                        );

                        bc_source_config.data += parsed_len;

                        /* Initializing number of bis to 0 for each subgroup */
                        bis_info_index = 0U;

                        while (bis_info_index < n_bis)
                        {
                            parsed_len = 0U;
                            GA_mem_set
                            (
                                &bc_basic_announcement_2,
                                0x00U,
                                sizeof(BAP_BASIC_ANNOUNCEMENT_LEVEL_2)
                            );

                            if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_2
                                              (
                                                  bc_source_config.data,
                                                  &bc_basic_announcement_2,
                                                  &parsed_len
                                              ))
                            {
                                APPL_TRC
                                (
                                    "Len: 0x%02X,  BIS_index: 0x%02X\n",
                                    (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream_index
                                );
                                appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index =
                                    bc_basic_announcement_2.stream_index;

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
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_ltv,
                                    &bc_basic_announcement_2.stream,
                                    sizeof(GA_CODEC_IE)
                                );
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf,
                                    &rem_cs_conf,
                                    sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                                );
                                /* Create the combine cs conf of SG and BIS */
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                    &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].cs_conf,
                                    sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                                );
                                appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis.aca =
                                    rem_cs_conf.aca;

                                bis_info_index++;
                                bc_source_config.data += parsed_len;
                            }
                        }
                        /* Increment the SubGroups Parsed */
                        sg_info_index++;
                    }
                }
            }
            /*
             * This event gets triggered in below cases:
             * 1. PAST Available, Mode: 2 or 3(PA reports enabled)
             * 2. PAST Not Available, Manual Trigger of Associate.
             * In case 1, Update of pa_sync_state shall be handled in
             * HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_SUBEVENT.
             * Case 2, Update of pa_sync_state shall be handled below.
             */
            /*
             * Fetch the Rx State Instance for this source before trigerring
             * Notification stating the indication of encryption status.
             */
            if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                              (
                                  appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                  &bc_rx_state_inst,
                                  &db_index
                              ))
            {
                if (BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA !=
                     appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state)
                {
                    if (GA_SUCCESS == ga_status)
                    {
                        /*
                         * Update the rx_state_inst PA_Sync_State to
                         * Synchronized to PA (0x02) as SD is synced to BC Src PA.
                         */
                        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                            BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA;
                    }
                    else
                    {
                        /*
                         * Update the RX_State_Inst PA_Sync_State to
                         * BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA (0x03) as
                         * SD has failed to sync to Broadcast_Src PA.
                         */
                        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                            BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA;
                    }
                    /* Update and notify */
                    (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                            (
                                                db_index,
                                                bc_rx_state_inst,
                                                GA_TRUE
                                            );
                }
            }
            else
            {
                APPL_ERR("Invalid Device !");
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
            if (appl_ga_bc_sd_params.stream_synchandle
                == appl_ga_bc_sd_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }

            GA_CB_ENTRY("BC CAP Server");
            appl_ga_bc_sd_params.stream_synchandle =
                appl_ga_bc_sd_params.bcast_synchandle;

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
            appl_ga_bc_sd_params.encryption_state =
            bc_source_stream.encryption;
            /* Check for encrypted/unencrypted BIS */
            if (GA_TRUE == appl_ga_bc_sd_params.encryption_state)
            {
                if (BASS_BIG_ENCRYPTION_NOT_ENCRYPTED ==
                 appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.big_encryption)
                {
                    /*
                     * Setting Big_Encryption to BASS_BIG_ENCRYPTION_BC_CODE_REQUIRED
                     * for encrypted BIS.
                     */
                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.big_encryption =
                        BASS_BIG_ENCRYPTION_BC_CODE_REQUIRED;
                }

                /*
                 * Fetch the Rx_State_Instance for this source before trigerring
                 * Notification indicating encryption status.
                 */
                if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                                  (
                                      appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                      &bc_rx_state_inst,
                                      &db_index
                                  ))
                {
                    /* Update and notify */
                    (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                            (
                                                db_index,
                                                bc_rx_state_inst,
                                                GA_TRUE
                                            );
                }
                else
                {
                    APPL_ERR("Invalid Device !");
                }
            }
            else
            {
                /*
                 * If the Encryption State is Not encrypted, Reset the State to
                 * Not encrypted in case if it was set to Decrypting before.
                 */
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.big_encryption =
                    BASS_BIG_ENCRYPTION_NOT_ENCRYPTED;
            }
        }
        break;

    case GA_BC_ENABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_ENABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_ENABLE_CNF, ga_data, &bc_enable))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Error Code: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.error_code),
                bc_enable.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.big_handle),
                bc_enable.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Transport Latency BIG: 0x%08X\n",
                (UCHAR)sizeof(bc_enable.transport_latency_big),
                bc_enable.transport_latency_big
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.nse), bc_enable.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.bn), bc_enable.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.pto), bc_enable.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.irc), bc_enable.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.max_pdu), bc_enable.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.iso_interval), bc_enable.iso_interval
            );

            APPL_TRC
            (
                "Len: 0x%02X,  Number of BISes: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.num_bis), bc_enable.num_bis
            );
            for (i = 0U; i < bc_enable.num_bis; i++)
            {
                APPL_TRC("BIS[%d]:\n", (i+1U));
                APPL_TRC
                (
                    "\tLen: 0x%02X,  BIS Conn Handle: 0x%04X\n",
                    (UCHAR)sizeof(bc_enable.connection_handle[i]),
                    bc_enable.connection_handle[i]
                );
            }

            if (0x00U == bc_enable.error_code)
            {
                if (GA_TRUE == appl_ga_bc_sd_params.encryption_state)
                {
                    /*
                     * Update the State to Decrypting if the encryption_state
                     * is True
                     */
                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.big_encryption =
                        BASS_BIG_ENCRYPTION_DECRYPTING;
                }

                /*
                 * Fetch the SG Index and BIS Info Index to update the BIS Conn
                 * Handle.
                 */
                for (i = 0U; i < appl_ga_bc_sd_params.bis_count_enable; i++)
                {
                    if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                            (
                                appl_ga_bc_sd_params.bis_index_enable[i],
                                &sg_info_index,
                                &bis_info_index
                            ))
                    {
                        /*
                         * Save the BIS Handle to the respective BIS Index which has
                         * been used in Enable.
                         */
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                            bc_enable.connection_handle[j];
                        j++;
                    }
                    else
                    {
                        APPL_ERR
                        (
                            "BIS Index %d is not present in any Subgroup\n",
                            appl_ga_bc_sd_params.bis_index_enable[i]
                        );
                    }
                }

                if (0U < bc_enable.num_bis)
                {
                    appl_ga_bc_sd_params.enable_synchandle =
                        appl_ga_bc_sd_params.bcast_synchandle;

#if 0
                    /* Broadcast Sink Dissociate: Disable PA Scan, Stop rx PA reports */
                    APPL_TRC("Dissociate Broadcast Sink...\n");
                    retval = GA_broadcast_dissociate
                             (
                                 appl_ga_bc_sd_params.bcast_synchandle
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);

                    /*  Broadcast Sink Release: Disable EA Scan */
                    APPL_TRC("Release Broadcast Sink...\n");
                    retval = GA_broadcast_scan_end();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
#endif /* 0 */

                    if (1U < appl_ga_bc_sd_params.bis_count_enable)
                    {
                        /*
                         * This is a case where we are trying to Sync to more than 1
                         * BIS at a time, Not supported here.
                         */
                        APPL_TRC("Not yet Handled\n!");

                        /*
                         * TODO: See is there is a practical case for this,
                         * If yes, then add Code to Setup ISO Data path in a chain
                         * and also start receiving Audio for a single BIS
                         * i.e. Begin LC3 Decoder Module + Audio PL Module only
                         * once while ISO Data Path can be set for all and the
                         * audio playback can be allowed for one and for the rest
                         * of the BISes, the audio can be decoded and dumped into
                         * a file.
                         */
                    }
                    else
                    {
                        /*
                         * Fetch the first valid SG Index and BIS Index to Setup ISO
                         * Data by referring to first BIS index in the bis_index_enable[]
                         */
                        if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                            (
                                appl_ga_bc_sd_params.bis_index_enable[0U],
                                &sg_info_index,
                                &bis_info_index
                            ))
                        {
                            /* Setup the ISO Data Path for the BIS */
                            if (GA_FAILURE == appl_ga_bc_sd_snk_setup_iso
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
            /*
             * Update BIS_Sync_State for RX_State and Notify RX State
             * Instance
             */
             appl_ga_bc_sd_update_bis_sync_state_and_notify(GA_TRUE);
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

            appl_ga_bc_sd_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sd_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sd_params.stream_synchandle = BC_INVALID_HANDLE;

            /*
             * Fetch the Rx State Instance for this source before trigerring
             * Notification stating the indication of encryption status.
             */
            if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                              (
                                  appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                  &bc_rx_state_inst,
                                  &db_index
                              ))
            {
                /*
                 * Update the rx_state_inst PA_Sync_State to
                 * Not synchronized to PA (0x00) as SD has stopped sync to BC Src PA.
                 */
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                    BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED;

                /* Update and notify */
                (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                        (
                                            db_index,
                                            bc_rx_state_inst,
                                            GA_TRUE
                                        );
            }
            else
            {
                APPL_ERR("Invalid Device !");
            }
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

            appl_ga_bc_sd_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sd_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sd_params.stream_synchandle = BC_INVALID_HANDLE;

            /*
             * Fetch the Rx State Instance for this source before trigerring
             * Notification stating the indication of encryption status.
             */
            if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                              (
                                  appl_ga_bc_sd_bass_local_db.bc_src_dev,
                                  &bc_rx_state_inst,
                                  &db_index
                              ))
            {
                /*
                 * Update the rx_state_inst PA_Sync_State to
                 * Not synchronized to PA (0x00) as SD has lost sync to BC Src PA.
                 */
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                    BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED;

                /* Update and notify */
                (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                        (
                                            db_index,
                                            bc_rx_state_inst,
                                            GA_TRUE
                                        );
            }
            else
            {
                APPL_ERR("Invalid Device !");
            }
        }
        break;

    case GA_BC_DISABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_CNF, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );

                appl_ga_bc_sd_params.config_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sd_params.stream_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sd_params.enable_synchandle = BC_INVALID_HANDLE;
            }

            /*
             * Fetch the first valid SG Index and BIS Index to Setup ISO
             * Data by referring to first BIS index in the bis_index_enable[]
             */
            if (GA_SUCCESS ==
                appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                (
                    appl_ga_bc_sd_params.bis_index_enable[0U],
                    &sg_info_index,
                    &bis_info_index
                ))
            {
                /*
                 * As BIG is already terminated when this event is generated,
                 * update the entry for ISO Removal if any Remove ISO Entry Failed.
                 */
                if (AUDIO_ISO_IDLE !=
                    appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state)
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_snk_remove_entry
                                      (
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                                      ))
                    {
                        APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                            SD_GET_ROLE_STRING(bc_role));
                        /* Update the ISO state */
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                            AUDIO_ISO_IDLE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed or Already Removed!\n",
                            SD_GET_ROLE_STRING(bc_role));
                    }
                }

                if (appl_ga_bc_sd_params.bis_index_active_playback ==
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index)
                {
                    /* We are Audio Sink, Stop Receiving Audio data */
                    if (GA_SUCCESS == appl_ga_bc_sd_snk_stop_receiving_audio
                                      (
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                    }
                }
            }
        }
        /*
         * Reset the Handles, BIS(s) Info and Update BC Rx State Instance and
         * notify.
         */
        appl_ga_bc_reset_on_disable();
        break;

    case GA_BC_DISABLE_IND:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_IND, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );
                APPL_TRC
                (
                    "Len: 0x%02X,  Reason: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.reason
                );

                appl_ga_bc_sd_params.config_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sd_params.stream_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sd_params.enable_synchandle = BC_INVALID_HANDLE;

                /* Check for Reason HC_CONNECTION_TREMINATED_DUE_TO_MIC_FAILURE */
                if (HC_CONNECTION_TREMINATED_DUE_TO_MIC_FAILURE == bc_disable.reason)
                {
                    /*
                     * HCI_LE_BIG_SYNC_LOST_SUBEVENT is received due to
                     * incorrect Broadcast Code.
                     */
                    /*
                     * Setting of Big Encryption to Bad Code
                     * (incorrect encryption key)
                     */
                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.big_encryption =
                        BASS_BIG_ENCRYPTION_BAD_CODE;
                    /*
                     * Fill Bad Code with the received incorrect broadcast code
                     * from BA.
                     */
                    GA_mem_copy
                    (
                        &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bad_code[0U],
                        &appl_ga_bc_sd_params.bc_code[0U],
                        BASS_BC_CODE_MAX_LEN
                    );

                    /*
                     * Cancel the previous timer since it would have set the
                     * value to BASS_BIG_ENCRYPTION_DECRYPTING and Notification
                     * will be sent wrongly.
                     */
                    if (BT_TIMER_HANDLE_INIT_VAL != appl_ga_bc_sd_cp_ntf_timer_handle)
                    {
                        BT_stop_timer
                        (
                            appl_ga_bc_sd_cp_ntf_timer_handle
                        );
                    }
                }
                else
                {
                    /* No action required */
                }
            }
            else
            {
                /* No action required */
            }

            /*
             * Fetch the first valid SG Index and BIS Index to Setup ISO
             * Data by referring to first BIS index in the bis_index_enable[]
             */
            if (GA_SUCCESS ==
                appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                (
                    appl_ga_bc_sd_params.bis_index_enable[0U],
                    &sg_info_index,
                    &bis_info_index
                ))
            {
                /*
                 * As BIG is already terminated when this event is generated,
                 * update the entry for ISO Removal.
                 */
                if (GA_SUCCESS == appl_ga_utils_audio_snk_remove_entry
                                  (
                                      appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                                  ))
                {
                    APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                        SD_GET_ROLE_STRING(bc_role));
                    /* Update the ISO state */
                    appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                        AUDIO_ISO_IDLE;
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n",
                        SD_GET_ROLE_STRING(bc_role));
                }

                if (appl_ga_bc_sd_params.bis_index_active_playback ==
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index)
                {
                    /* We are Audio Sink, Stop Receiving Audio data */
                    if (GA_SUCCESS == appl_ga_bc_sd_snk_stop_receiving_audio
                                      (
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                    }
                }
            }
            /*
             * Reset the Handles, BIS(s) Info and Update BC Rx State Instance and
             * notify.
             */
            appl_ga_bc_reset_on_disable();
        }
        break;

    default:
        APPL_ERR("Unknown Event ID: 0x%02X !\n", ga_event);
        break;
    }

    /* GA_CB_EXIT(); */

    APPL_DBG("[APPL][GA][BC_SD]: << appl_ga_bc_sd_sink_cb_handler\n");

    return retval;
}

GA_RESULT appl_ga_bc_sink_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          )
{
#ifdef BAP_BROADCAST_SINK

    GA_RESULT  retval;
    UINT8      i;
    UINT8      j;
    UINT32     bc_id;
    UINT32     delay;
    UINT8      n_sg;
    UINT8      n_bis;
    UINT8      parsed_len;
    UINT8      sg_info_index;
    UINT8      bis_info_index;

    BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS  bc_source_announcement = { 0 };
    BAP_BROADCAST_ANNOUNCEMENT         bc_annoucement = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_1     bc_basic_announcement_1 = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_2     bc_basic_announcement_2 = { 0 };
    BAP_BC_SOURCE_CONFIG_PARAMS        bc_source_config = { 0 };
    BAP_BC_SOURCE_STREAM_PARAMS        bc_source_stream = { 0 };
    BAP_BC_ASSOCIATE_PARAMS            bc_associate = { 0 };
    BAP_BC_DISSOCIATE_PARAMS           bc_dissociate = { 0 };
    BAP_BC_ENABLE_PARAMS               bc_enable = { 0 };
    BAP_BC_DISABLE_PARAMS              bc_disable = { 0 };

    GA_LC3_CODEC_SPECIFIC_CONF rem_cs_conf = { 0 };

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    j = 0U;
    bc_id = 0U;
    delay = 0U;
    n_sg = 0U;
    sg_info_index = 0U;
    bis_info_index = 0U;
    n_bis = 0U;
    parsed_len = 0U;

    APPL_DBG("[APPL][GA][BC_SNK]: >> appl_ga_bc_sink_handler\n");

    /* GA_CB_ENTRY("BCast Sink"); */

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
                /* Do not allow printing of the events multiple times */
                if (GA_FALSE == appl_ga_bc_sd_snk_is_rem_src_in_db
                                (
                                    bc_source_announcement.address,
                                    bc_source_announcement.address_type
                                ))
                {
                    /* Device has newly come in, Display and Add in the DB */
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
                        bc_source_announcement.address[0U],
                        bc_source_announcement.address[1U],
                        bc_source_announcement.address[2U],
                        bc_source_announcement.address[3U],
                        bc_source_announcement.address[4U],
                        bc_source_announcement.address[5U]
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
                        bc_source_announcement.direct_address[0U],
                        bc_source_announcement.direct_address[1U],
                        bc_source_announcement.direct_address[2U],
                        bc_source_announcement.direct_address[3U],
                        bc_source_announcement.direct_address[4U],
                        bc_source_announcement.direct_address[5U]
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

                    appl_ga_bc_sd_snk_add_rem_src_to_db
                    (
                        bc_source_announcement.address,
                        bc_source_announcement.address_type
                    );
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
        if (GA_SUCCESS == ga_status)
        {
            /*
             * Assign the role to NONE here, as the role is checked while
             * redirecting the callback API.
             */
            bc_role = BC_ROLE_NONE;
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
            appl_ga_bc_sink_params.bcast_synchandle =
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
                bc_associate.advertiser_addr[0U],
                bc_associate.advertiser_addr[1U],
                bc_associate.advertiser_addr[2U],
                bc_associate.advertiser_addr[3U],
                bc_associate.advertiser_addr[4U],
                bc_associate.advertiser_addr[5U]
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
            if (appl_ga_bc_sink_params.config_synchandle
                == appl_ga_bc_sink_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }
            GA_CB_ENTRY("BC CAP Server");
            appl_ga_bc_sink_params.config_synchandle =
                appl_ga_bc_sink_params.bcast_synchandle;

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
                appl_ga_bc_sink_params.bc_num_subgroups = n_sg;

                /* Presentation_Delay + Num_Subgroups is parsed */
                bc_source_config.data += parsed_len;

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
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bc_nstream =
                            n_bis;
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
                        GA_mem_copy
                        (
                            &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bc_basic_announcement_1.codec,
                            &bc_basic_announcement_1.codec,
                            sizeof(GA_CODEC_INFO)
                        );
                        GA_mem_copy
                        (
                            &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].cs_conf,
                            &rem_cs_conf,
                            sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                        );

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Metadata_Length: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_basic_announcement_1.meta.length
                        );

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
                        }
                        else
                        {
                            APPL_TRC
                            (
                                "Len: 0x%02X,  Metadata: NULL\n",
                                bc_basic_announcement_1.meta.length
                            );
                        }
                        GA_mem_copy
                        (
                            &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bc_basic_announcement_1.meta,
                            &bc_basic_announcement_1.meta,
                            sizeof(GA_METADATA)
                        );

                        bc_source_config.data += parsed_len;

                        /* Initializing number of bis to 0 for each subgroup */
                        bis_info_index = 0U;

                        while (bis_info_index < n_bis)
                        {
                            parsed_len = 0U;
                            GA_mem_set
                            (
                                &bc_basic_announcement_2,
                                0x00U,
                                sizeof(BAP_BASIC_ANNOUNCEMENT_LEVEL_2)
                            );

                            if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_2
                                              (
                                                  bc_source_config.data,
                                                  &bc_basic_announcement_2,
                                                  &parsed_len
                                              ))
                            {
                                APPL_TRC
                                (
                                    "Len: 0x%02X,  BIS_index: 0x%02X\n",
                                    (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream_index
                                );
                                appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index =
                                    bc_basic_announcement_2.stream_index;

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
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_ltv,
                                    &bc_basic_announcement_2.stream,
                                    sizeof(GA_CODEC_IE)
                                );
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf,
                                    &rem_cs_conf,
                                    sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                                );
                                /* Create the combine cs conf of SG and BIS */
                                GA_mem_copy
                                (
                                    &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                    &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].cs_conf,
                                    sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                                );
                                appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis.aca =
                                    rem_cs_conf.aca;

                                bis_info_index++;
                                bc_source_config.data += parsed_len;
                            }
                        }
                        /* Increment the SubGroups Parsed */
                        sg_info_index++;
                    }
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
            if (appl_ga_bc_sink_params.stream_synchandle
                == appl_ga_bc_sink_params.bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }

            GA_CB_ENTRY("BC CAP Server");
            appl_ga_bc_sink_params.stream_synchandle =
                appl_ga_bc_sink_params.bcast_synchandle;

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
            appl_ga_bc_sink_params.encryption_state = bc_source_stream.encryption;
            /*
             * TODO: Validate if we already have the broadcast code via OOB.
             * If yes, broadcast code will be user input in case 29.
             */
        }
        break;

    case GA_BC_ENABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_ENABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_ENABLE_CNF, ga_data, &bc_enable))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Error Code: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.error_code),
                bc_enable.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.big_handle),
                bc_enable.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Transport Latency BIG: 0x%08X\n",
                (UCHAR)sizeof(bc_enable.transport_latency_big),
                bc_enable.transport_latency_big
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.nse), bc_enable.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.bn), bc_enable.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.pto), bc_enable.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.irc), bc_enable.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.max_pdu), bc_enable.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.iso_interval), bc_enable.iso_interval
            );

            APPL_TRC
            (
                "Len: 0x%02X,  Number of BISes: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.num_bis), bc_enable.num_bis
            );

            for (i = 0U; i < bc_enable.num_bis; i++)
            {
                APPL_TRC("BIS[%d]:\n", (i+1U));
                APPL_TRC
                (
                    "\tLen: 0x%02X,  BIS Conn Handle: 0x%04X\n",
                    (UCHAR)sizeof(bc_enable.connection_handle[i]),
                    bc_enable.connection_handle[i]
                );
            }

            if (0x00U == bc_enable.error_code)
            {
                /*
                 * Fetch the SG Index and BIS Info Index to update the BIS Conn
                 * Handle.
                 */
                for (i = 0U; i < appl_ga_bc_sink_params.bis_count_enable; i++)
                {
                    if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                            (
                                appl_ga_bc_sink_params.bis_index_enable[i],
                                &sg_info_index,
                                &bis_info_index
                            ))
                    {
                        /*
                         * Save the BIS Handle to the respective BIS Index which has
                         * been used in Enable.
                         */
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                            bc_enable.connection_handle[j];
                        j++;
                    }
                    else
                    {
                        APPL_ERR
                        (
                            "BIS Index %d is not present in any Subgroup\n",
                            appl_ga_bc_sink_params.bis_index_enable[i]
                        );
                    }
                }

                if (0U < bc_enable.num_bis)
                {
                    appl_ga_bc_sink_params.enable_synchandle =
                        appl_ga_bc_sink_params.bcast_synchandle;

#if 0
                    /*
                     * Application Restrcition:
                     * If we do Scan end here, it will reset the bc_role to None.
                     * The callback for BC events and HCI events routed from
                     * appl_ga.c and appl_ga_utils.c
                     * further steps, otherwise event callback will not get hit.
                     *
                     * According to the state machine we should not do Release
                     * (State change to Idle) when we have triggered enable
                     * (Current State: Streaming).
                     * As per BAP Spec, Page 111, Fig: 6.3
                     *
                     * Eg: Start_scan -> Associate -> Enable -> Scan End -> Disable
                     * In this case, Disable_CNF event will not get hit and
                     * removal of iso and stop_receiving audio will not be performed.
                     * So, ISO, LC3 and Audio_pl are still active, but not receiving
                     * any audio streams due to BIG terminate.
                     */
                    /* Broadcast Sink Dissociate: Disable PA Scan, Stop rx PA reports */
                    APPL_TRC("GA_broadcast_dissociate\n");
                    retval = GA_broadcast_dissociate
                             (
                                 appl_ga_bc_sink_params.bcast_synchandle
                             );
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);

                    /*  Broadcast Sink Release: Disable EA Scan */
                    APPL_TRC("GA_broadcast_scan_end\n");
                    retval = GA_broadcast_scan_end();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
#endif /* 0 */

                    if (1U < appl_ga_bc_sink_params.bis_count_enable)
                    {
                        /*
                         * This is a case where we are trying to Sync to more than 1
                         * BIS at a time, Not supported here.
                         */
                        APPL_TRC("Not yet Handled\n!");

                        /*
                         * TODO: See is there is a practical case for this,
                         * If yes, then add Code to Setup ISO Data path in a chain
                         * and also start rececing Audio for a single BIS
                         * i.e. Begin LC3 Decoder Module + Audio PL Module only
                         * once while ISO Data Path can be set for all and the
                         * audio playback can be allowed for one and for the rest
                         * of the BISes, the audio can be decoded and dumped into
                         * a file.
                         */
                    }
                    else
                    {
                        /*
                         * Fetch the first valid SG Index and BIS Index to Setup ISO
                         * Data Path.
                         */
                        if (GA_SUCCESS ==
                            appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                            (
                                appl_ga_bc_sink_params.bis_index_enable[0U],
                                &sg_info_index,
                                &bis_info_index
                            ))
                        {
                            /* Setup the ISO Data Path for the BIS */
                            if (GA_FAILURE == appl_ga_bc_sd_snk_setup_iso
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

            appl_ga_bc_sink_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sink_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sink_params.stream_synchandle = BC_INVALID_HANDLE;
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

            appl_ga_bc_sink_params.bcast_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sink_params.config_synchandle = BC_INVALID_HANDLE;
            appl_ga_bc_sink_params.stream_synchandle = BC_INVALID_HANDLE;
        }
        break;

    case GA_BC_DISABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_CNF, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );

                appl_ga_bc_sink_params.config_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sink_params.stream_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sink_params.enable_synchandle = BC_INVALID_HANDLE;
            }

            /*
             * Fetch the first valid SG Index and BIS Index to Setup ISO
             * Data by referring to first BIS index in the bis_index_enable[]
             */
            if (GA_SUCCESS ==
                appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                (
                    appl_ga_bc_sink_params.bis_index_enable[0U],
                    &sg_info_index,
                    &bis_info_index
                ))
            {
                /*
                 * As BIG is already terminated when this event is generated,
                 * update the entry for ISO Removal if any Remove ISO Entry Failed.
                 */
                if (AUDIO_ISO_IDLE !=
                    appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state)
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_snk_remove_entry
                                      (
                                          appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                                      ))
                    {
                        APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                            SD_GET_ROLE_STRING(bc_role));
                        /* Update the ISO state */
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                            AUDIO_ISO_IDLE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n",
                            SD_GET_ROLE_STRING(bc_role));
                    }
                }

                if (appl_ga_bc_sink_params.bis_index_active_playback ==
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index)
                {
                    /* We are Audio Sink, Stop Receiving Audio data */
                    if (GA_SUCCESS == appl_ga_bc_sd_snk_stop_receiving_audio
                                      (
                                          appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                    }
                }
            }

            /* Reset the Handles, BIS(s) Info */
            appl_ga_bc_reset_on_disable();
        }
        break;

    case GA_BC_DISABLE_IND:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_IND, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );
                APPL_TRC
                (
                    "Len: 0x%02X,  Reason: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.reason
                );

                appl_ga_bc_sink_params.config_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sink_params.stream_synchandle = BC_INVALID_HANDLE;
                appl_ga_bc_sink_params.enable_synchandle = BC_INVALID_HANDLE;
            }

            /*
             * Fetch the first valid SG Index and BIS Index to Setup ISO
             * Data by referring to first BIS index in the bis_index_enable[].
             */

             /*
              * TODO: In case where > 1 BIS is enabled and ISO is setup for all,
              * Call this in a loop and remove ISO for all.
              */
            if (GA_SUCCESS ==
                appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                (
                    appl_ga_bc_sink_params.bis_index_enable[0U],
                    &sg_info_index,
                    &bis_info_index
                ))
            {
                /*
                 * As BIG is already terminated when this event is generated,
                 * update the entry for ISO Removal.
                 */
                if (GA_SUCCESS == appl_ga_utils_audio_snk_remove_entry
                                  (
                                      appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                                  ))
                {
                    APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                        SD_GET_ROLE_STRING(bc_role));
                    /* Update the ISO state */
                    appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                        AUDIO_ISO_IDLE;
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: ISO Data Path Removal Status: Remove entry failed !\n",
                        SD_GET_ROLE_STRING(bc_role));
                }

                if (appl_ga_bc_sink_params.bis_index_active_playback ==
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index)
                {
                    /* We are Audio Sink, Stop Receiving Audio data */
                    if (GA_SUCCESS == appl_ga_bc_sd_snk_stop_receiving_audio
                                      (
                                          appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                                          appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis,
                                          AUDIO_NUM_OF_BITS_PER_SAMPLE
                                      ))
                    {
                        appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                            GA_FALSE;
                    }
                }
            }

            /* Reset the Handles, BIS(s) Info */
            appl_ga_bc_reset_on_disable();
        }
        break;

    default:
        APPL_TRC("Received UnHandled Event\n");
        break;
    }

    /* GA_CB_EXIT(); */

    APPL_DBG("[APPL][GA][BC_SNK]: << appl_ga_bc_sink_handler\n");

    return retval;

#endif /* BAP_BROADCAST_SINK */
}

void appl_ga_bc_snk_init(void)
{
    UINT8  sg_info_index;
    UINT8  bis_info_index;

    sg_info_index = 0U;
    bis_info_index = 0U;

    /* Initialize Parameters for Broadcast Sink */
    appl_ga_bc_sink_params.bcast_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sink_params.config_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sink_params.enable_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sink_params.stream_synchandle =
        BC_INVALID_HANDLE;

    appl_ga_bc_sink_params.bc_num_subgroups = 0U;
    for (sg_info_index = 0U; sg_info_index < APPL_BC_MAX_NUM_OF_SUB_GROUPS; sg_info_index++)
    {
        GA_mem_set
        (
            &appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index],
            0x00U,
            sizeof(APPL_BC_SUBGROUP)
        );
        for (bis_info_index = 0U; bis_info_index < APPL_BC_MAX_BIS_PER_SUB_GROUP; bis_info_index++)
        {
            appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                BC_INVALID_HANDLE;
            appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_IDLE;
            appl_ga_bc_sink_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                GA_FALSE;
        }
    }

    appl_ga_bc_sink_params.bis_count_enable = 0U;
    GA_mem_set
    (
        &appl_ga_bc_sink_params.bis_index_enable,
        0x00U,
        sizeof(appl_ga_bc_sink_params.bis_index_enable)
    );
    appl_ga_bc_sink_params.bis_index_active_playback = 0U;
    appl_ga_bc_sink_params.encryption_state = GA_FALSE;


    appl_ga_bc_sd_snk_disable_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void appl_ga_bc_sd_init(void)
{
    UINT8  sg_info_index;
    UINT8  bis_info_index;

    sg_info_index = 0U;
    bis_info_index = 0U;

    /* Initialize Parameters for Scan Delegator */
    appl_ga_bc_sd_params.bcast_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sd_params.config_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sd_params.enable_synchandle =
        BC_INVALID_HANDLE;
    appl_ga_bc_sd_params.stream_synchandle =
        BC_INVALID_HANDLE;

    appl_ga_bc_sd_params.bc_num_subgroups = 0U;
    for (sg_info_index = 0U; sg_info_index < APPL_BC_MAX_NUM_OF_SUB_GROUPS; sg_info_index++)
    {
        GA_mem_set
        (
            &appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index],
            0x00U,
            sizeof(APPL_BC_SUBGROUP)
        );
        for (bis_info_index = 0U; bis_info_index < APPL_BC_MAX_BIS_PER_SUB_GROUP; bis_info_index++)
        {
            appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                BC_INVALID_HANDLE;
            appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_IDLE;
            appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status =
                GA_FALSE;
        }
    }

    appl_ga_bc_sd_params.bis_count_enable = 0U;
    GA_mem_set
    (
        &appl_ga_bc_sd_params.bis_index_enable,
        0x00U,
        sizeof(appl_ga_bc_sd_params.bis_index_enable)
    );
    appl_ga_bc_sd_params.bis_index_active_playback = 0U;
    appl_ga_bc_sd_params.encryption_state = GA_FALSE;

    appl_ga_bc_sd_snk_disable_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void main_sd_operations(void)
{
    int          choice;
    UINT8        menu_choice;
    GA_RESULT    retval;
    GA_ENDPOINT  device;
    UINT8        sid;
    UINT8        i;
    UINT8        bc_rx_state_inst;
    UINT8        db_index;
    UINT8        bis_count, bis_index[2U];
    UCHAR        bc_code[BASS_BC_CODE_MAX_LEN];
    UCHAR      * bc_code_ptr;
    UINT8        sg_info_index;
    UINT8        bis_info_index;

#ifdef BASS_DB
    GA_BASS_BC_RX_STATE rx_state = { 0 };
#endif /* BASS_DB */

    /* Initialize */
    menu_choice = 0U;
    retval = GA_SUCCESS;
    bc_code_ptr = NULL;
    bc_rx_state_inst = 0U;

    GA_LOOP_FOREVER()
    {
        APPL_TRC("\n");
        CONSOLE_OUT("%s", bc_sd_sink_menu_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = (UINT8)choice;

        switch(menu_choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                if (BC_ROLE_NONE != bc_role)
                {
                    APPL_TRC("A role is already defined for the device. "
                    "Please deinit and try again\n");
                }
                else
                {
                    APPL_TRC("GA_bass_se_init...\n");
                    retval = GA_bass_se_init(appl_ga_bc_bass_sd_cb);
                    if (GA_SUCCESS == retval)
                    {
                        /* Reset the value in the global variable */
                        GA_mem_set
                        (
                            &appl_ga_bc_sd_bass_bc_rx_state,
                            0U,
                            sizeof(APPL_BC_SD_BASS_BC_RX_STATE)
                        );

                        /* Assign the role as SD after initialization */
                        bc_role = BC_ROLE_SD;
                        /* Initialize timer handle for notification */
                        appl_ga_bc_sd_cp_ntf_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                        /* Initialize timer handle for PAST receive verification */
                        appl_ga_bc_sd_past_rx_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                        appl_ga_bc_sd_set_default_adv_params();
#ifdef BASS_DB
                        BASS_DB_init();
#else /* BASS_DB */
                        APPL_ERR("BASS_DB is undefined !\n");
#endif /* BASS_DB */
                        appl_ga_bc_sd_init();
                        /*
                         * During SD init, by default 1 bc_rx_state instance is
                         * added and hence update it in our Local DB and update
                         * the count rx_state_inst_count.
                         */
                        appl_ga_bc_sd_bass_local_db.rx_state_inst[appl_ga_bc_sd_bass_local_db.rx_state_inst_count].inst =
                            0U;
                        appl_ga_bc_sd_bass_local_db.rx_state_inst[appl_ga_bc_sd_bass_local_db.rx_state_inst_count].isSrcAdded
                            = GA_FALSE;
                        appl_ga_bc_sd_bass_local_db.rx_state_inst_count = 1U;
                    }
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
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
                    APPL_TRC("GA_bass_se_shutdown...\n");
                    retval = GA_bass_se_shutdown();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        /* Assign the role as NONE after initialization */
                        bc_role = BC_ROLE_NONE;
                    }
                }
                break;

            case 4:
#ifdef BAP_BROADCAST_SCAN_DELEGATOR
                APPL_TRC("GA_bass_se_add_bc_rx_state_inst...\n");
                retval = GA_bass_se_add_bc_rx_state_inst
                         (
                             &appl_ga_bc_sd_bass_local_db.rx_state_inst[appl_ga_bc_sd_bass_local_db.rx_state_inst_count].inst
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("GA_bass_se_add_bc_rx_state_inst Instance: %d \n",
                        appl_ga_bc_sd_bass_local_db.rx_state_inst[appl_ga_bc_sd_bass_local_db.rx_state_inst_count].inst);
                    appl_ga_bc_sd_bass_local_db.rx_state_inst[appl_ga_bc_sd_bass_local_db.rx_state_inst_count].isSrcAdded
                        = GA_FALSE;
                    appl_ga_bc_sd_bass_local_db.rx_state_inst_count++;
                }
#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
                break;

            case 5:
#ifdef BAP_BROADCAST_SCAN_DELEGATOR
                APPL_TRC("appl_ga_bc_sd_setup_adv...\n");
                appl_ga_bc_sd_setup_adv(GA_TRUE);
#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
                break;

            case 6:
#ifdef BAP_BROADCAST_SCAN_DELEGATOR
                if (APPL_GA_ADV_SD == is_adv_enabled)
                {
                    APPL_TRC("appl_ga_bc_sd_setup_adv...\n");
                    appl_ga_bc_sd_setup_adv(GA_FALSE);
                }
                else
                {
                    /* TODO */
                }
#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
                break;

            case 10:
#ifdef BAP_BROADCAST_SCAN_DELEGATOR
                CONSOLE_OUT("Input Broadcast Recieve State Instance: ");
                CONSOLE_IN("%d", &choice);
                bc_rx_state_inst = (UINT8)choice;
                if (GA_SUCCESS == appl_ga_utils_get_conn_device
                                  (
                                      appl_ga_bc_sd_bass_local_db.conn_dev_index,
                                      &device
                                  ))
                {
#ifdef BASS_DB
                    /* Fetch the rx_state info from given bc_rx_state_inst. */
                    for (i = 0U; i < appl_ga_bc_sd_bass_local_db.rx_state_inst_count; i++)
                    {
                        /*
                         * Fetch the rx_state from BASS db only if instance is
                         * same as given bc_rx_state_inst and source is added
                         * for the instance.
                         */
                        if ((bc_rx_state_inst == appl_ga_bc_sd_bass_local_db.rx_state_inst[i].inst) &&
                            (GA_TRUE == appl_ga_bc_sd_bass_local_db.rx_state_inst[i].isSrcAdded))
                        {
                            retval = BASS_DB_fetch_rx_state_instance
                                     (
                                         appl_ga_bc_sd_bass_local_db.rx_state_inst[i].db_index,
                                         &rx_state
                                     );
                            break;
                        }
                    }

                    /* Check if the fetched bc_rx_state info is proper */
                    if ((appl_ga_bc_sd_bass_local_db.rx_state_inst_count != i) &&
                        (GA_SUCCESS == retval))
                    {
                        /* Pack the data for notification and also get the ntf_data size */
                        appl_ga_bc_sd_ntf_data_size = appl_ga_bc_sd_pack_rx_state_ntf_data
                                                      (
                                                          rx_state.info,
                                                          &appl_ga_bc_sd_ntf_data[0U]
                                                      );
                        APPL_TRC("GA_bass_se_notify_bc_rx_state()...\n");
                        retval = GA_bass_se_notify_bc_rx_state
                                 (
                                     &device,
                                     appl_ga_bc_sd_bass_local_db.rx_state_inst[i].inst,
                                     appl_ga_bc_sd_ntf_data,
                                     appl_ga_bc_sd_ntf_data_size
                                 );
                        APPL_TRC("Retval - ");
                        appl_ga_bc_sd_bass_display_ga_result(retval);
                    }
                    else
                    {
                        APPL_ERR("Given instance is incorrect or Source is not added for the instance !");
                    }
#else /* BASS_DB */
                    APPL_ERR("BASS_DB is undefined !\n");
                    /*
                     * TODO: How to send Ntf if BASS_DB is not present? Local
                     * Copy in the App
                     */
#endif /* BASS_DB */
                }
                else
                {
                    APPL_ERR("Invalid Connection Device Index !\n");
                }
#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
                break;

            case 15:
                appl_ga_bc_sd_req_sync_info_past();
                break;

            case 16:
#ifdef BAP_BROADCAST_SCAN_DELEGATOR
                if (APPL_GA_SCAN_INVALID == is_scan_enabled)
                {
                    APPL_TRC("SD - Scan for Broadcast Source...\n");
                    retval = GA_broadcast_scan_announcement();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        is_scan_enabled = APPL_GA_SCAN_FOR_BC_SRC;
                        appl_ga_bc_sd_bass_local_db.scan_status = GA_TRUE;
                        /* Reset the DB when fresh scan begins */
                        appl_ga_bc_sd_snk_clear_rem_src_db();
                    }
                }
#endif /* BAP_BROADCAST_SCAN_DELEGATOR */
                break;

            case 17:
                /*
                 * TODO: Handle stop scan when GA_BC_DISSOCIATE_CNF event
                 * is received for SD role.
                 */
                APPL_TRC("SD - Dissociate Broadcast Sink...\n");
                retval = GA_broadcast_dissociate
                         (
                             appl_ga_bc_sd_params.bcast_synchandle
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                break;

            case 18:
                if (GA_TRUE == appl_ga_bc_sd_params.encryption_state)
                {
                    for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
                    {
                        if (0U != appl_ga_bc_sd_params.bc_code[i])
                        {
                            /* It is a valid BC Code */
                            break;
                        }
                    }
                    if (BASS_BC_CODE_MAX_LEN == i)
                    {
                        /*
                         * BIS is encrypted, we have not received BC Code via CP
                         * set BC Code. It must be a OOB method of fetching the
                         * BC Code. Hence take the input here.
                         */
                        CONSOLE_OUT("Enter Broadcast_Code [IN HEX]: ");
                        for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
                        {
                            CONSOLE_IN("%x", &choice);
                            bc_code[i] = (UCHAR)choice;
                        }
                        bc_code_ptr = &bc_code[0U];
                    }
                    else
                    {
                        bc_code_ptr = &appl_ga_bc_sd_params.bc_code[0U];
                    }
                }
                else
                {
                    /*
                     * Reset the BC Code Pointer if it is referencing previously
                     * to a valid value.
                     */
                    bc_code_ptr = NULL;
                }

                /*
                 * Extract/Take input of BIS_Indeces and BIS_Count for
                 * Enable operation
                 */
                appl_ga_bc_sd_get_input_for_enable();

                /* Validate if LC3 Decoding is allowed by passing the
                 * CS Conf of the first valid SG Index, BIS Index.
                 */
                if (GA_SUCCESS ==
                    appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                    (
                        appl_ga_bc_sd_params.bis_index_enable[0U],
                        &sg_info_index,
                        &bis_info_index
                    ))
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_snk_audio_lc3_dec_allowed
                                      (
                                          appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis
                                      ))
                    {
                        if (1U < appl_ga_bc_sd_params.bis_count_enable)
                        {
                            /*
                             * TODO: Although we can test till syncing to multiple
                             * BISes, Support for Setup ISO for multiple BISes and
                             * saving of audio data for BISes other than active
                             * playback BIS is not in place yet!
                             */

                             /*
                             * This is a case where we are enabling multiple BIS to
                             * streaming, However we can playback only a single BIS at
                             * once, Hence select the BIS which needs playback, rest
                             * of the BISes audio dump will be saved in files.
                             */
                            CONSOLE_OUT("Enter the BIS which needs active playback\n");
                            CONSOLE_IN("%d", &choice);
                            appl_ga_bc_sd_params.bis_index_active_playback = (UINT8)choice;
                        }
                        else
                        {
                            /* There is only one BIS_Index for enable. Use the same. */
                            appl_ga_bc_sd_params.bis_index_active_playback =
                                appl_ga_bc_sd_params.bis_index_enable[0U];
                        }

                        if (NULL != bc_code_ptr)
                        {
                            APPL_TRC("Broadcast Code for Encrypted Stream: ");
                            for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
                            {
                                APPL_TRC("%X ", bc_code_ptr[i]);
                            }
                            APPL_TRC("\n");
                        }
                        /* TODO: Above shd not be called before, Check again */
                        APPL_TRC("SD - Enable Broadcast Sink...\n");
                        retval = GA_broadcast_enable
                                 (
                                     appl_ga_bc_sd_params.bcast_synchandle,
                                     bc_code_ptr,
                                     appl_ga_bc_sd_params.bis_count_enable,
                                     appl_ga_bc_sd_params.bis_index_enable
                                 );
                        APPL_TRC("Retval - ");
                        appl_ga_bc_sd_bass_display_ga_result(retval);
                    }
                    else
                    {
                        /* TODO: Display Log */
                    }
                }
                else
                {
                    APPL_ERR("Unable to fetch SG_Index and/or BIS_Index !\n");
                }
                break;

            case 19:
                APPL_TRC("GA_broadcast_disable\n");

                /*
                 * This will terminate the BIG, hence the ISO Data Path needs
                 * to be removed before calling this API itself.
                 * So the sequence is:
                 * 1. Begin a timer to call Disable API.
                 * 2. Trigger Remove ISO Data Path.
                 * 3. ISO Data Path Removal Handler:
                 *    Stop Audio PL Generator and LC3 Decoder Module.
                 * 4. Timer times out and this API is called as part of the
                 *    timer callback.
                 *
                 */
                if (GA_SUCCESS ==
                    appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                    (
                        appl_ga_bc_sd_params.bis_index_enable[0U],
                        &sg_info_index,
                        &bis_info_index
                    ))
                {
                    if (GA_FAILURE == appl_ga_bc_sd_snk_remove_iso
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

                BT_start_timer
                (
                    &appl_ga_bc_sd_snk_disable_timer_handle,
                    3U,
                    appl_ga_bc_sd_snk_disable_timer_handler,
                    NULL,
                    0U
                );
                break;

            case 25:
                APPL_TRC("Setup Broadcast Sink...\n");
                if ((APPL_GA_SCAN_INVALID == is_scan_enabled) &&
                    (BC_ROLE_NONE == bc_role))
                {
                    /* Assign the role as BC_Sink if the current role is NONE */
                    bc_role = BC_ROLE_BC_SNK;
                    appl_ga_bc_snk_init();

                    retval = GA_broadcast_scan_announcement();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);

                    /* Reset the DB when fresh scan begins */
                    if (GA_SUCCESS == retval)
                    {
                        is_scan_enabled = APPL_GA_SCAN_FOR_BC_SRC;
                        appl_ga_bc_sd_snk_clear_rem_src_db();
                    }
                }
                else
                {
                    /* Do not change the role here */
                }
                break;

            case 26:
                if (APPL_GA_SCAN_FOR_BC_SRC == is_scan_enabled)
                {
                    APPL_TRC("Release Broadcast Sink...\n");
                    retval = GA_broadcast_scan_end();
                    APPL_TRC("Retval - ");
                    appl_ga_bc_sd_bass_display_ga_result(retval);
                    if (GA_SUCCESS == retval)
                    {
                        is_scan_enabled = APPL_GA_SCAN_INVALID;
                        /* Do not reset the role here, wait for the event */
                    }
                }
                else
                {
                    /* TODO */
                }
                break;

            case 27:
                CONSOLE_OUT("Enter SID: ");
                CONSOLE_IN("%d", &choice);
                sid = (UINT8)choice;

                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                device.bd_type = (UINT8)choice;

                APPL_TRC("Associate Broadcast Sink...\n");
                retval = GA_broadcast_associate
                         (
                             sid,
                             &device
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                break;

            case 28:
                APPL_TRC("Dissociate Broadcast Sink...\n");
                retval = GA_broadcast_dissociate
                         (
                             appl_ga_bc_sink_params.bcast_synchandle
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                break;

            case 29:
                if (GA_TRUE != appl_ga_bc_sink_params.encryption_state)
                {
                    /*
                     * Reset the BC Code Pointer if it is referencing previously
                     * to a valid value.
                     */
                    appl_ga_bc_snk_bc_code_ptr = NULL;
                }
                /* Reset the BIS_Count and BIS_Indeces before taking input. */
                appl_ga_bc_sink_params.bis_count_enable = 0U;
                GA_mem_set
                (
                    &appl_ga_bc_sink_params.bis_index_enable[0U],
                    0U,
                    (APPL_BC_MAX_BIS_PER_SUB_GROUP * APPL_BC_MAX_NUM_OF_SUB_GROUPS)
                );

                /*
                 * Since the bcast_synchandle corresponds to a single BIG, the
                 * CS Conf on all SGs and BISes will remain same except for
                 * the ACA. Hence the LC3 Decoder or the Audio PL module should
                 * not have any propblem if multiple BISes are enabled.
                 */
                CONSOLE_OUT("Enter Num_BIS: ");
                CONSOLE_IN("%d", &choice);
                bis_count = (UCHAR)choice;

                appl_ga_bc_sink_params.bis_count_enable = bis_count;
                CONSOLE_OUT("Enter BIS_Index [IN HEX]: ");
                for (i = 0U; i < bis_count; i++)
                {
                    CONSOLE_IN("%x", &choice);
                    bis_index[i] = (UCHAR)choice;
                    appl_ga_bc_sink_params.bis_index_enable[i] =
                        bis_index[i];
                }

                if (1U < bis_count)
                {
                    /*
                     * This is a case where we are enabling multiple BIS to
                     * streaming, However we can playback only a single BIS at
                     * once, Hence select the BIS which needs playback, rest
                     * of the BISes audio dump will be saved in files.
                     */
                    CONSOLE_OUT("Enter the BIS which needs active playback\n");
                    CONSOLE_IN("%d", &choice);
                    appl_ga_bc_sink_params.bis_index_active_playback = (UINT8)choice;
                }
                else
                {
                    appl_ga_bc_sink_params.bis_index_active_playback = (UINT8)choice;
                }

                APPL_TRC("Enable Broadcast Sink...\n");
                retval = GA_broadcast_enable
                         (
                             appl_ga_bc_sink_params.bcast_synchandle,
                             appl_ga_bc_snk_bc_code_ptr,
                             appl_ga_bc_sink_params.bis_count_enable,
                             &appl_ga_bc_sink_params.bis_index_enable[0U]
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                break;

            case 30:
                APPL_TRC("Disable Broadcast Sink...\n");

                /*
                 * This will terminate the BIG, hence the ISO Data Path needs
                 * to be removed before calling this API itself.
                 * So the sequence is:
                 * 1. Begin a timer to call Disable API.
                 * 2. Trigger Remove ISO Data Path.
                 * 3. ISO Data Path Removal Handler:
                 *    Stop Audio PL Generator and LC3 Decoder Module.
                 * 4. Timer times out and this API is called as part of the
                 *    timer callback.
                 *
                 */
                /*
                 * Fetch the first valid SG Index and BIS Index to Setup ISO
                 * Data by referring to first BIS index in the bis_index_enable[]
                 */
                if (GA_SUCCESS ==
                    appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
                    (
                        appl_ga_bc_sink_params.bis_index_enable[0U],
                        &sg_info_index,
                        &bis_info_index
                    ))
                {
                    if (GA_FAILURE == appl_ga_bc_sd_snk_remove_iso
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

                BT_start_timer
                (
                    &appl_ga_bc_sd_snk_disable_timer_handle,
                    3U,
                    appl_ga_bc_sd_snk_disable_timer_handler,
                    NULL,
                    0U
                );
                break;

            case 31:
                /* This is simulation of OOB to use the BC Code */
                if (GA_TRUE == appl_ga_bc_sink_params.encryption_state)
                {
                    CONSOLE_OUT("Enter Broadcast_Code [IN HEX]: ");
                    for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
                    {
                        CONSOLE_IN("%x", &choice);
                        appl_ga_bc_snk_bc_code[i] = (UCHAR)choice;
                    }
                    APPL_TRC("Broadcast_Code: ");
                    for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
                    {
                        APPL_TRC("0%x ", appl_ga_bc_snk_bc_code[i]);
                    }
                    APPL_TRC("\n");
                    appl_ga_bc_snk_bc_code_ptr = &appl_ga_bc_snk_bc_code[0U];
                }
                break;

            default:
                APPL_TRC("Invalid choice\n");
        }
        if (0U == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

void appl_ga_bc_sd_set_default_adv_params(void)
{
    /* Initialize default broadcast adv parameters */
    /* Make the properties as 1, to set it as scannable advertisement */
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.adv_event_properties = 0x01U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.adv_filter_policy = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.adv_tx_power = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.duration = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.frag_pref = 0x01U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.max_ext_adv_evts = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.own_addr_type = 0x00U;
    GA_mem_set(&appl_ga_bc_sd_bass_local_db.sd_eadv_params.peer_addr[0U], 0U, GA_BD_ADDR_SIZE);
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.peer_addr_type = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.pri_adv_channel_map = 0x07U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.pri_adv_interval_max = 0x00000020U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.pri_adv_interval_min = 0x00000020U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.pri_adv_phy = 0x01U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.scan_req_nfy_enable = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.sec_adv_max_skip = 0x00U;
    appl_ga_bc_sd_bass_local_db.sd_eadv_params.sec_adv_phy = 0x01U;

    return;
}

void appl_ga_bc_sd_setup_adv(UINT8 enable)
{
    GA_RESULT  retval;

    if (enable)
    {
        /* Reusing enable to indicate enable extended advertising */
        retval = appl_ga_utils_enable_extended_advertise(enable);
        if (GA_SUCCESS == retval)
        {
            is_adv_enabled = APPL_GA_ADV_SD;
        }
        else
        {
            APPL_ERR("Extended Advertise Already initiated by Other Module !\n");
        }
    }
    else
    {
        /* Reusing enable to indicate disable extended advertising */
        retval = appl_ga_utils_enable_extended_advertise(enable);
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

    return;
}

UINT8 appl_ga_bc_sd_pack_rx_state_ntf_data
      (
        /* IN */    GA_BASS_BC_RX_STATE_SRC_INFO rx_state_info,
        /* INOUT */ UCHAR * ntf_data
      )
{
    UINT8 marker;
    UCHAR i;
    UCHAR j;
    UINT8 prev_metadata_len;

    marker = 0U;
    i = 0U;
    j = 0U;
    prev_metadata_len = 0U;

    /*
     * Set the array to zero values to avoid sending any previously packed
     * data while sending notifications.
     */
    GA_mem_set
    (
        ntf_data,
        0U,
        APPL_GA_BC_SD_MAX_NTF_DATA_SIZE
    );

    ntf_data[marker++] = rx_state_info.id;
    ntf_data[marker++] = rx_state_info.addr_type;
    BT_PACK_LE_N_BYTE
    (
        &ntf_data[marker],
        &rx_state_info.addr[0U],
        GA_BD_ADDR_SIZE
    );
    marker += GA_BD_ADDR_SIZE;
    ntf_data[marker++] = rx_state_info.adv_sid;
    for (i = 0U; i < 3U ;i++)
    {
        ntf_data[marker++] = rx_state_info.broadcast_ID[i];
    }
    ntf_data[marker++] = rx_state_info.pa_sync_state;
    ntf_data[marker++] = rx_state_info.big_encryption;
    if (BASS_BIG_ENCRYPTION_BAD_CODE == rx_state_info.big_encryption)
    {
        BT_PACK_BE_N_BYTE
        (
            &ntf_data[marker],
            &rx_state_info.bad_code,
            BASS_BC_CODE_MAX_LEN
        );
        marker += BASS_BC_CODE_MAX_LEN;
    }
    ntf_data[marker++] = rx_state_info.num_subgroups;

    for (i = 0U; i < rx_state_info.num_subgroups; i++)
    {
        GA_PACK_LE_4_BYTE_VAL
        (
            &ntf_data[marker],
            rx_state_info.bis_sync_state[i]
        );
        /* Increment 4Byte for bis_sync_state */
        marker += 4U;
        ntf_data[marker++] = rx_state_info.metadata_len[i];
        for (j = prev_metadata_len; j < (prev_metadata_len + rx_state_info.metadata_len[i]); j++)
        {
            ntf_data[marker++] = rx_state_info.metadata[j];
        }
        prev_metadata_len += rx_state_info.metadata_len[i];
    }

    return marker;
}

GA_RESULT appl_ga_bc_sd_req_sync_info_past(void)
{
    GA_RESULT  retval;
    UINT8      mode;
    UINT16     skip;
    UINT16     sync_timeout;
    UINT8      cte_type;
    UINT16     acl_conn_handle;

    /* Initialize*/
    retval = GA_FAILURE;
    acl_conn_handle = 0U;

    /*
     * An HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
     * sent to the Host. HCI_LE_Periodic_Advertising_Report events will be
     * disabled.
     */
    mode = 0x02U;

    /*
     * The number of periodic advertising packets that can be skipped after a
     * successful receive.
     */
    skip = 0x00U;

    /*
     * Synchronization timeout for the periodic advertising train.
     * Range: 0x000A to 0x4000
     * Time = N*10 ms
     * Time Range: 100 ms to 163.84 s
     */
    /* Maximum */
    sync_timeout = 0xC80U;

    /* Constant Tone Extension */
    cte_type = 0x00U;

    if (GA_SUCCESS == appl_ga_utils_get_conn_handle_from_index
                      (
                          appl_ga_bc_sd_bass_local_db.conn_dev_index,
                          &acl_conn_handle
                      ))
    {
        APPL_TRC("Connection Handle: 0x%04X\n", acl_conn_handle);
        retval = BT_hci_le_set_periodic_advertising_sync_transfer_parameters
                 (
                     acl_conn_handle,
                     mode,
                     skip,
                     sync_timeout,
                     cte_type
                 );
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_parse_and_display_bc_rx_state
          (
              /* IN */ GA_BASS_BC_RX_STATE br_rx_state_info
          )
{
    GA_RESULT  retval;
    UCHAR      i;
    UINT8      prev_metadata_len;

    prev_metadata_len = 0U;
    retval = GA_FAILURE;

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
        appl_ga_bc_sd_parse_and_display_addr_type
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
        appl_ga_bc_sd_parse_and_display_PA_Sync_State
        (
            br_rx_state_info.info.pa_sync_state
        );
        appl_ga_bc_sd_parse_and_display_BIG_encryption
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
            APPL_TRC
            (
                "\tLen: 0x%02X,  BIS_Sync State: ",
                (UCHAR)sizeof(br_rx_state_info.info.bis_sync_state[i])
            );
            if (BASS_BIS_SYNC_STATE_FAILED_TO_SYNC_TO_BIG == br_rx_state_info.info.bis_sync_state[i])
            {
                APPL_TRC("Failed to sync to BIG (0x%08X)\n", br_rx_state_info.info.bis_sync_state[i]);
            }
            else
            {
                APPL_TRC
                (
                    "0x%08X\n",
                    br_rx_state_info.info.bis_sync_state[i]
                );
            }
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
    return retval;
}

void appl_ga_bc_sd_parse_and_display_addr_type(/* IN */ UINT8 addr_type)
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

void appl_ga_bc_sd_parse_and_display_PA_Sync_State
     (
        /* IN */ UINT8 pa_sync_state
     )
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

void appl_ga_bc_sd_parse_and_display_BIG_encryption
     (
        /* IN */ UINT8 big_encryption
     )
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

void appl_ga_bc_sd_parse_and_display_PA_Sync(/* IN */ UINT8 pa_sync)
{
    APPL_TRC("Len: 0x%02X,  PA_Sync: ", (UCHAR)sizeof(pa_sync));
    switch (pa_sync)
    {
    case BASS_PA_SYNC_DO_NOT_SYNC_TO_PA:
        APPL_TRC("Do not synchronize to PA (0x%02X)\n", pa_sync);
        break;
    case BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL:
        APPL_TRC("Synchronize to PA - PAST available (0x%02X)\n", pa_sync);
        break;
    case BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL:
        APPL_TRC("Synchronize to PA - PAST not available (0x%02X)\n", pa_sync);
        break;
    default:
        APPL_ERR("Unknown PA_Sync value (0x%02X) !\n", pa_sync);
        break;
    }

    return;
}

GA_RESULT appl_ga_bc_sd_parse_and_handle_bass_cp
          (
            /* IN */ UINT8       * bass_cp,
            /* IN */ UINT16        len,
            /* IN */ GA_ENDPOINT * device
          )
{
    GA_RESULT  retval;
    UINT8      opcode;
    UINT8      marker;
    UINT8      index;
    UINT8      size;

    GA_IGNORE_UNUSED_PARAM(device);

    /* Initialize */
    retval = GA_SUCCESS;
    marker = 0U;
    index = 0U;
    size = 0U;

    /* Assign the Opcode and increment the Marker by 1U */
    opcode = bass_cp[marker];
    marker++;
    /* Excluding the Length 1B of the Opcode */
    len -= 1U;

    APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(opcode));
    /*
     * BASS Spec V1.0, 3.1.1.1
     *
     * If the server detects the total length of the opcode and parameter
     * values, written by a client to be less than or greater than the
     * total length of the opcode and all fixed length parameter values
     * plus the length of any variable length parameter values for that
     * operation, the server shall respond with an ATT Error Response
     * and shall set the Error Code parameter to Write Request Rejected.
     */

    switch (opcode)
    {
    case BASS_CP_OPCODE_REMOTE_SCAN_STOPPED:
        APPL_TRC("Remote Scan Stopped operation (0x%02X)\n", opcode);
        /* This CP opcode should not receive any data other than opcode. */
        if (0U != len)
        {
            APPL_ERR
            (
                "[GA][BASS]: BASS_CP_OPCODE_REMOTE_SCAN_STOPPED Invalid Param or Length !\n"
            );
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        break;

    case BASS_CP_OPCODE_REMOTE_SCAN_STARTED:
        APPL_TRC("Remote Scan Started operation (0x%02X)\n", opcode);
        /* This CP opcode should not receive any data other than opcode. */
        if (0U != len)
        {
            APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_REMOTE_SCAN_STARTED Invalid Param or Length !\n");
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        break;

    case BASS_CP_OPCODE_ADD_SOURCE:
        APPL_TRC("Add Source operation (0x%02X)\n", opcode);
        /*
         * Excluding opcode, the minimum data size for this CP operation is
         * 15Bytes
         */
        if (15U > len)
        {
            APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_ADD_SOURCE Invalid Param or Length !\n");
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        else
        {
            retval = appl_ga_bc_sd_handle_add_src_cp(&bass_cp[marker], len);
        }
        break;

    case BASS_CP_OPCODE_MODIFY_SOURCE:
        APPL_TRC("Modify Source operation (0x%02X)\n", opcode);
        /*
         * Excluding opcode, the minimum data size for this CP operation is
         * 5Bytes
         */
        if (5U > len)
        {
            APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_MODIFY_SOURCE Invalid Param or Length !\n");
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        else
        {
            retval = appl_ga_bc_sd_handle_modify_src_cp(&bass_cp[marker], len);
        }
        break;

    case BASS_CP_OPCODE_SET_BC_CODE:
        APPL_TRC("Set Broadcast_Code operation (0x%02X)\n", opcode);
        /*
         * Excluding opcode, the minimum data size for this CP operation is
         * 17Bytes
         */
        if (17U != len)
        {
            APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_SET_BC_CODE Invalid Param or Length !\n");
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        else
        {
            retval = appl_ga_bc_sd_handle_set_bc_code_cp(&bass_cp[marker]);
        }
        break;

    case BASS_CP_OPCODE_REMOVE_SOURCE:
        APPL_TRC("Remove Source operation (0x%02X)\n", opcode);
        /*
         * Excluding opcode, the minimum data size for this CP operation is
         * 1Byte (source_ID)
         */
        if (1U != len)
        {
            APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_REMOVE_SOURCE Invalid Param or Length !\n");
            retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
        }
        else
        {
            retval = appl_ga_bc_sd_handle_rem_src_cp(bass_cp[marker]);
        }
        break;

    default:
        APPL_ERR("Unknown Opcode: 0x%02X !\n", opcode);
        retval = BASS_APPL_ERR_CODE_OPCODE_NOT_SUPP;
        break;
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_handle_add_src_cp
          (
              /* IN */ UINT8 * add_src_data,
              /* IN */ UINT16  len
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      db_index;
    UINT16     bytes_parsed;

    GA_BASS_BAS_CP_ADD_SRC  add_src;
    UINT32 bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    UINT8  metadata_len[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    UINT8  metadata[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN];

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;
    db_index = 0U;
    bytes_parsed = 0U;

    /*
     * Validate if we have an added free BR Receive State
     * Char instance.
     * - If yes use the free index entry.
     * - Else,
     *          Remove any DB Entry of App s choice.
     *          Clear the contents of that entry if any in the App
     *          space.
     */
    if (GA_SUCCESS == appl_ga_bc_sd_is_src_add_allowed(&index))
    {
        /* Use this unique index as Source_ID */
    }
    else
    {
#ifdef BASS_DB
        /* For now, Delete 0th Entry, Remove DB Entry */
        if (GA_SUCCESS == BASS_DB_remove_source(0U))
        {
            index = 0U;
            appl_ga_bc_sd_bass_local_db.rx_state_inst[index].isSrcAdded = GA_FALSE;
        }
        else
        {
            return GA_FAILURE;
        }
#else /* BASS_DB */
        /* For now, Delete 0th Entry */
        appl_ga_bc_sd_bass_local_db.rx_state_inst[0U].isSrcAdded = GA_FALSE;
#endif /* BASS_DB */
    }

    bytes_parsed = appl_ga_bc_sd_parse_and_display_add_src_data
                   (
                       add_src_data, /* IN */
                       &add_src, /* INOUT */
                       bis_sync,  /* INOUT */
                       metadata_len,  /* INOUT */
                       metadata  /* INOUT */
                   );

    /*
     * Check if bytes parsed is not equal to the length received.
     * If not, then send write_req_rejected error response
     */
    if (len != bytes_parsed)
    {
        APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_ADD_SOURCE Invalid Param or Length !\n");
        /*
         * BASS Spec V1.0, 3.1.1.1
         *
         * If the server detects the total length of the opcode and parameter
         * values, written by a client to be less than or greater than the
         * total length of the opcode and all fixed length parameter values
         * plus the length of any variable length parameter values for that
         * operation, the server shall respond with an ATT Error Response
         * and shall set the Error Code parameter to Write Request Rejected.
         */
        retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
    }
    else
    {
        /*
         * Give the reference to the global arrays in the structure to the
         * bc_rx_state
         */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bis_sync_state =
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_bis_sync;
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len =
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata_len;
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata =
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata;

        /* Copy add_src data to bc_rx_state and perform required operation */
        retval = appl_ga_bc_sd_copy_add_src_data_to_bc_rx_state
                 (
                     add_src,
                     index
                 );

        if (GA_SUCCESS == retval)
        {
            retval = appl_ga_bc_sd_parse_and_display_bc_rx_state
                     (
                         appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
                     );

#ifdef BASS_DB
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("BASS_DB_add_source()...\n");
                retval = BASS_DB_add_source
                         (
                             &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state, &db_index
                         );
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
            }
#endif /* BASS_DB */

            if (GA_SUCCESS == retval)
            {
                APPL_TRC("Source_ID of the added source: 0x%04X\n", index);
                /* Updatig the BASS DB index */
                appl_ga_bc_sd_bass_local_db.rx_state_inst[index].db_index = db_index;
                appl_ga_bc_sd_bass_local_db.rx_state_inst[index].isSrcAdded = GA_TRUE;
            }

            if (BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL == add_src.pa_sync)
            {
                GA_COPY_BRR_DEV_ADDR
                (
                    appl_ga_bc_sd_bass_local_db.bc_src_dev.bd_addr,
                    add_src.adv_addr
                );
                GA_COPY_BRR_DEV_TYPE
                (
                    appl_ga_bc_sd_bass_local_db.bc_src_dev.bd_type,
                    add_src.adv_addr_type
                )
                /* Automated process to prepare Controller to receive PAST */
                /*
                 * If GA_SUCCESS, then the PA_Sync_State will get updated as
                 * SYNC_INFO_REQUEST in the event Handling, Else the default
                 * value is retained.
                 */
                (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_req_sync_info_past();
            }

            /*
             * Send notification only when PA_Sync is
             * BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL, as manual operation is
             * required for BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL.
             */
            if (BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL != add_src.pa_sync)
            {
                /* Send Notification */
                BT_start_timer
                (
                    &appl_ga_bc_sd_cp_ntf_timer_handle,
                    4U, /* Delay of 4 seconds */
                    appl_ga_bc_sd_cp_ntf_timer_handler,
                    &appl_ga_bc_sd_bass_local_db.rx_state_inst[index].inst,
                    sizeof(BASS_BC_RX_STATE_INST)
                );
            }
        }
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_is_src_add_allowed(/* IN */ UINT8 * id)
{
    UINT8 index;
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    for (index = 0U; index < BASS_SE_MAX_BC_RX_STATE_ENTITIES; index++)
    {
        if (!appl_ga_bc_sd_bass_local_db.rx_state_inst[index].isSrcAdded)
        {
            retval = GA_SUCCESS;
            *id = index;
            break;
        }
    }

    return retval;
}

UINT16 appl_ga_bc_sd_parse_and_display_add_src_data
       (
           /* IN */    UINT8                  * bas_cp,
           /* INOUT */ GA_BASS_BAS_CP_ADD_SRC * add_src,
           /* INOUT */ UINT32                 * bis_sync,
           /* INOUT */ UINT8                  * metadata_len,
           /* INOUT */ UINT8                  * metadata
       )
{
    UINT8  prev_metadata_len;
    UINT8  marker;
    UCHAR  i;
    UINT32 value_4;

    marker = 0U;
    prev_metadata_len = 0U;
    value_4 = 0U;

    add_src->adv_addr_type = bas_cp[marker];
    marker++;
    APPL_TRC
    (
        "Len: 0x%02X,  Advertiser_Address_Type: ",
        (UCHAR)sizeof(UINT8)
    );
    appl_ga_bc_sd_parse_and_display_addr_type(add_src->adv_addr_type);

    BT_COPY_BD_ADDR
    (
        add_src->adv_addr,
        &bas_cp[marker]
    );
    marker += GA_BD_ADDR_SIZE;
    APPL_TRC
    (
        "Len: 0x%02X,  Advertiser_Address: %02X %02X %02X %02X %02X %02X\n",
        GA_BD_ADDR_SIZE,
        add_src->adv_addr[0U], add_src->adv_addr[1U], add_src->adv_addr[2U],
        add_src->adv_addr[3U], add_src->adv_addr[4U], add_src->adv_addr[5U]
    );

    add_src->adv_SID = bas_cp[marker];
    marker++;
    APPL_TRC
    (
        "Len: 0x%02X,  Advertising_SID: 0x%02X\n",
        (UCHAR)sizeof(UINT8),
        add_src->adv_SID
    );

    GA_mem_copy
    (
        &add_src->broadcast_ID[0U],
        &bas_cp[marker],
        BASS_BC_ID_LEN
    );
    marker += BASS_BC_ID_LEN;

    GA_UNPACK_LE_3_BYTE
    (
        &value_4,
        &add_src->broadcast_ID[0U]
    );
    APPL_TRC
    (
        "Len: 0x%02X,  Broadcast_ID: 0x%06X\n",
        BASS_BC_ID_LEN,
        value_4
    );

    add_src->pa_sync = bas_cp[marker];
    marker ++;
    appl_ga_bc_sd_parse_and_display_PA_Sync(add_src->pa_sync);

    GA_UNPACK_LE_2_BYTE
    (
        &add_src->pa_interval,
        &bas_cp[marker]
    );
    marker += 2U;
    APPL_TRC
    (
        "Len: 0x%02X,  PA_Interval: 0x%04X\n",
        (UCHAR)sizeof(UINT16),
        add_src->pa_interval
    );

    add_src->num_subgroups = bas_cp[marker];
    marker++;
    APPL_TRC
    (
        "Len: 0x%02X,  Num_Subgroups: 0x%02X\n",
        (UCHAR)sizeof(UINT8),
        add_src->num_subgroups
    );

    for (i = 0U; i < add_src->num_subgroups; i++)
    {
        APPL_TRC("Subgroup Data[%d]:\n", i);
        /* Copy data to the Local array */
        GA_UNPACK_LE_4_BYTE
        (
            &bis_sync[i],
            &bas_cp[marker]
        );
        /* Increment marker by 4B */
        marker += 4U;
        APPL_TRC
        (
            "\tLen: 0x%02X,  BIS_Sync: ",
            (UCHAR)sizeof(UINT32)
        );
        if (BASS_BIS_SYNC_NO_PREFERENCE == bis_sync[i])
        {
            APPL_TRC("No Preference (0x%08X)\n", bis_sync[i]);
        }
        else
        {
            APPL_TRC("0x%08X\n", bis_sync[i]);
        }
        /* Copy data to the Local array */
        metadata_len[i] = bas_cp[marker];
        /* Increment marker by 1B */
        marker++;
        APPL_TRC
        (
            "\tLen: 0x%02X,  Metadata_Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8),
            metadata_len[i]
        );
        /* Copy metadata to the Local array */
        BT_UNPACK_LE_N_BYTE
        (
            &metadata[prev_metadata_len],
            &bas_cp[marker],
            metadata_len[i]
        );
        /* Increment marker by metadata_len[i] for metadata_len */
        marker += metadata_len[i];
        APPL_TRC
        (
            "\tLen: 0x%02X,  Metadata:\n",
            metadata_len[i]
        );
        /* TODO: Parse Metadata API */
        appl_ga_utils_bap_parse_and_display_metadata_ltv
        (
            &metadata[prev_metadata_len],
            metadata_len[i],
            NULL,
            NULL
        );
        prev_metadata_len += metadata_len[i];
    }
    /*
     * Make the pointers point to the base address of their
     * respective arrays.
     */
    add_src->bis_sync = &bis_sync[0U];
    add_src->metadata_len = &metadata_len[0U];
    add_src->metadata = &metadata[0U];

    /* Return number of bytes parsed */
    return marker;
}

GA_RESULT appl_ga_bc_sd_copy_add_src_data_to_bc_rx_state
          (
              /* IN */ GA_BASS_BAS_CP_ADD_SRC add_src,
              /* IN */ UINT8 index
          )
{
    GA_RESULT retval;
    UINT8     size;
    UCHAR     i;
    UINT8     marker;

    /* Initialize */
    retval = GA_FAILURE;
    size = 0U;
    i = 0U;
    marker = 0U;

    if ((BASS_ADDR_TYPE_PUBLIC != add_src.adv_addr_type) &&
        (BASS_ADDR_TYPE_RANDOM != add_src.adv_addr_type))
    {
        APPL_ERR("RFU Address Type !\n");
    }
    else if (0x0FU < add_src.adv_SID)
    {
        APPL_ERR("RFU Adv SID !\n");
    }
    else if (BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL < add_src.pa_sync)
    {
        APPL_ERR("RFU PA Sync !\n");
    }
    else if (GA_FAILURE == appl_ga_bc_sd_validate_for_duplicate_bis_index_in_subgroups
                           (
                               add_src.num_subgroups,
                               add_src.bis_sync
                           ))
    {
        APPL_ERR("Invalid BIS Sync !\n");
    }
    else
    {

        /* Copy the contents of Add Source to Broadcast Receive State */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.id = index;

        /* Assign as written by the client */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr_type = add_src.adv_addr_type;

        /* Assign as written by the client */
        for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
        {
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.addr[i] = add_src.adv_addr[i];
        }

        /* Assign as written by the client */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.adv_sid = add_src.adv_SID;

        /* Assign as written by the client */
        for (i = 0U; i < BASS_BC_ID_LEN; i++)
        {
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.broadcast_ID[i] = add_src.broadcast_ID[i];
        }

        switch(add_src.pa_sync)
        {
            case BASS_PA_SYNC_DO_NOT_SYNC_TO_PA:
                /*
                 * If the PA_Sync parameter value written by the client is set
                 * to a value of 0x00 (Do not synchronize to PA), the server
                 * shall write a value of 0x00 (Not synchronized to PA) to the
                 * PA_Sync_State field of the selected Broadcast Receive State
                 * characteristic and the server shall not attempt to
                 * synchronize to the PA.
                 */
                break;

            case BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL:
                /*
                 * If the PA_Sync parameter value written by the client is set
                 * to a value of 0x01 (Synchronize to PA – PAST available), or
                 * 0x02 (Synchronize to PA – PAST not available), the server
                 * shall either write a value of 0x01 (SyncInfo Request) to the
                 * PA_Sync_State field of the selected Broadcast Receive State
                 * characteristic to request a client to transfer SyncInfo data
                 * to the server or the server shall attempt to synchronize
                 * with the PA by using the Periodic Advertising
                 * Synchronization Establishment procedure.
                 */
                /*
                 * PA_Sync_State field is updated to
                 * BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST while handling
                 * HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE
                 * event if the status is success.
                 */
                break;

            case BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL:
                /*
                 * - If the Addr_type is 1, then scan for broadcast source and
                 *   compare SID, Broadcast_ID, and Address. If they match, then
                 *   sync to that broadcast source
                 * - If the Addr_type is 0, then scan for broadcast source and
                 *   sync to that broadcast source
                 */
                break;

            default:
                /* Should not reach here */
                break;
        }
        /*
         * Default PA_Sync_State is made to BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED.
         * PA_Sync_State is updated in the operations triggered based on PA_Sync
         * field received.
         */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
         BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED;

        /* Assign as written by the client */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups =
         add_src.num_subgroups;

        /*
         * TODO: If BIG_Encryption field is set to Bad_Code, then 16-octet
         * will be added for Bad_Code field value.
         */
        size += 15U;

        /*
         * Copy the subgroup value. This is used for updating BIS_Sync_State
         * when GA_BC_ENABLE_CNF event is received.
         */
        appl_ga_bc_sd_bass_local_db.rx_state_inst[index].subgroup =
         add_src.num_subgroups;

        if (0U != appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups)
        {
            /*
             * Copy BIS_Sync to the global rx_state_inst. This is used while
             * syncing to the BIS (BC Enable Operation). When GA_BC_ENABLE_CNF
             * event is received, then BIS_Sync_State is updated.
             */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_local_db.rx_state_inst[index].bis_sync[0U],
                &add_src.bis_sync[0U],
                APPL_BC_MAX_NUM_OF_SUB_GROUPS
            );

            /* Assign as written by the client */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len[0U],
                &add_src.metadata_len[0U],
                APPL_BC_MAX_NUM_OF_SUB_GROUPS
            );
            /* Assign as written by the client */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata[0U],
                &add_src.metadata[0U],
                (APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN)
            );
        }

        /* size of all BIS_Sync State field */
        size += (sizeof(UINT32) * appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups);
        /* size of all Metadata_Length field */
        size += (sizeof(UINT8) * appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups);
        /* size of all Metadata field */
        for (i = 0U; i < appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups; i++)
        {
            size += appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len[i];
        }

        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info_len = size;

        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_handle_modify_src_cp
          (
              /* IN */ UINT8  * modify_src_data,
              /* IN */ UINT16  len
          )
{
    GA_RESULT  retval;
    UINT8      size;
    UINT16     bytes_parsed;
    UINT8      notify_rx_state;
    BASS_BC_RX_STATE_INST rx_state_inst;

    GA_BASS_BAS_CP_MODIFY_SRC  modify_src;
    UINT32 bis_sync[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    UINT8  metadata_len[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    UINT8  metadata[APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN];

    /* Initialize */
    retval = GA_FAILURE;
    rx_state_inst = 0U;
    size = 0U;
    bytes_parsed = 0U;
    notify_rx_state = GA_TRUE;

    bytes_parsed = appl_ga_bc_sd_parse_and_display_modify_src_data
                   (
                       modify_src_data, /* IN */
                       &modify_src, /* INOUT */
                       bis_sync, /* INOUT */
                       metadata_len, /* INOUT */
                       metadata /* INOUT */
                   );

    if (len != bytes_parsed)
    {
        APPL_ERR("[GA][BASS]: BASS_CP_OPCODE_MODIFY_SOURCE Invalid Param or Length !\n");
        /*
         * BASS Spec V1.0, 3.1.1.1
         *
         * If the server detects the total length of the opcode and parameter
         * values, written by a client to be less than or greater than the
         * total length of the opcode and all fixed length parameter values
         * plus the length of any variable length parameter values for that
         * operation, the server shall respond with an ATT Error Response
         * and shall set the Error Code parameter to Write Request Rejected.
         */
        retval = GA_BRR_ERR_WRITE_REQ_REJECTED;
    }
    else
    {
#ifdef BASS_DB
        /* Fetch the rx_state based on the Source_ID provided by the client. */
        rx_state_inst = appl_ga_bc_sd_fetch_rx_state_inst_from_src_id
                        (
                            modify_src.src_ID
                        );
        if (appl_ga_bc_sd_bass_local_db.rx_state_inst_count == rx_state_inst)
        {
            APPL_ERR("Invalid Source ID !\n");
            retval = BASS_APPL_ERR_CODE_INVALID_SRC_ID;
        }
        else
#endif /* BASS_DB */
        {
            /*
             * Give the reference to the global arrays in the structure to the
             * bc_rx_state
             */
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bis_sync_state =
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_bis_sync;
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len =
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata_len;
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata =
                appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata;

            /* Copy modify_src data to bc_rx_state and perform required operation */
            if (GA_SUCCESS == appl_ga_bc_sd_copy_modify_src_data_to_bc_rx_state
                              (
                                  modify_src,
                                  rx_state_inst,
                                  &notify_rx_state
                              ))
            {
                APPL_TRC("\n\nReceive State Instance Data:\n");
                retval = appl_ga_bc_sd_parse_and_display_bc_rx_state
                         (
                             appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
                         );

                if (GA_SUCCESS == retval)
                {
                    /*
                     * Notification is not sent when dissociate/disable has to
                     * be triggered manually
                     */
                    if (GA_SUCCESS == appl_ga_bc_sd_update_rx_state_and_notify
                                      (
                                          appl_ga_bc_sd_bass_local_db.rx_state_inst[rx_state_inst].db_index,
                                          rx_state_inst,
                                          notify_rx_state
                                      ))
                    {
                        APPL_TRC("Source_ID of the modified source: 0x%04X\n",
                            modify_src.src_ID);
                    }
                }
            }
        }
    }

    return retval;
}

UINT16 appl_ga_bc_sd_parse_and_display_modify_src_data
       (
           /* IN */    UINT8                     * bas_cp,
           /* INOUT */ GA_BASS_BAS_CP_MODIFY_SRC * modify_src,
           /* INOUT */ UINT32                    * bis_sync,
           /* INOUT */ UINT8                     * metadata_len,
           /* INOUT */ UINT8                     * metadata
       )
{
    UINT8  prev_metadata_len;
    UINT8  marker;
    UCHAR  i;
    UINT32 value_4;

    marker = 0U;
    prev_metadata_len = 0U;
    value_4 = 0U;

    modify_src->src_ID = bas_cp[marker];
    marker++;
    APPL_TRC
    (
        "Len: 0x%02X,  Source_ID: 0x%02X\n",
        (UCHAR)sizeof(UINT8), modify_src->src_ID
    );

    modify_src->pa_sync = bas_cp[marker];
    marker++;
    appl_ga_bc_sd_parse_and_display_PA_Sync(modify_src->pa_sync);

    GA_UNPACK_LE_2_BYTE
    (
        &modify_src->pa_interval,
        &bas_cp[marker]
    );
    marker += 2U;
    APPL_TRC
    (
        "Len: 0x%02X,  PA_Interval: 0x%04X\n",
        (UCHAR)sizeof(UINT16),
        modify_src->pa_interval
    );

    modify_src->num_subgroups = bas_cp[marker];
    marker++;
    APPL_TRC
    (
        "Len: 0x%02X,  Num_Subgroups: 0x%02X\n",
        (UCHAR)sizeof(UINT8),
        modify_src->num_subgroups
    );

    for (i = 0U; i < modify_src->num_subgroups; i++)
    {
        APPL_TRC("Subgroup Data[%d]:\n", i);
        /* Copy data to the Local array */
        GA_UNPACK_LE_4_BYTE
        (
            &bis_sync[i],
            &bas_cp[marker]
        );
        /* Increment marker by 4B */
        marker += 4U;
        APPL_TRC
        (
            "\tLen: 0x%02X,  BIS_Sync: ",
            (UCHAR)sizeof(UINT32)
        );
        if (BASS_BIS_SYNC_NO_PREFERENCE == bis_sync[i])
        {
            APPL_TRC("No Preference (0x%08X)\n", bis_sync[i]);
        }
        else
        {
            APPL_TRC("0x%08X\n", bis_sync[i]);
        }
        /* Copy data to the Local array */
        metadata_len[i] = bas_cp[marker];
        /* Increment marker by 1B */
        marker++;
        APPL_TRC
        (
            "\tLen: 0x%02X,  Metadata_Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8),
            metadata_len[i]
        );
        /* Copy metadata to the Local array */
        BT_UNPACK_LE_N_BYTE
        (
            &metadata[prev_metadata_len],
            &bas_cp[marker],
            metadata_len[i]
        );
        /* Increment marker by metadata_len[i] for metadata_len */
        marker += metadata_len[i];
        APPL_TRC
        (
            "\tLen: 0x%02X,  Metadata:\n",
            metadata_len[i]
        );
        /* TODO: Parse Metadata API */
        appl_ga_utils_bap_parse_and_display_metadata_ltv
        (
            &metadata[prev_metadata_len],
            metadata_len[i],
            NULL,
            NULL
        );
        prev_metadata_len += metadata_len[i];
    }
    /*
     * Make the pointers point to the base address of their
     * respective arrays.
     */
    modify_src->bis_sync = &bis_sync[0U];
    modify_src->metadata_len = &metadata_len[0U];
    modify_src->metadata = &metadata[0U];

    /* Return number of bytes parsed */
    return marker;
}

GA_RESULT appl_ga_bc_sd_copy_modify_src_data_to_bc_rx_state
          (
              /* IN */ GA_BASS_BAS_CP_MODIFY_SRC modify_src,
              /* IN */ UINT8                     index,
              /* IN */ UINT8                   * notify_rx_state
          )
{
    UCHAR     i;
    UINT8     marker;
    GA_RESULT retval;
    UINT8     is_dissociate;
    UINT8     is_enable_required;
    UINT8     is_disable_required;

    i = 0U;
    marker = 0U;
    retval = GA_FAILURE;
    is_dissociate = GA_FALSE;
    is_enable_required = GA_FALSE;
    is_disable_required = GA_FALSE;

    /* Reference pointers to arrays */
    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bis_sync_state =
     &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_bis_sync[0U];
    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len =
     &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata_len[0U];
    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata =
     &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_metadata[0U];

    if (BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL < modify_src.pa_sync)
    {
        APPL_ERR("RFU PA Sync !\n");
    }
    else if (GA_FAILURE == appl_ga_bc_sd_validate_for_duplicate_bis_index_in_subgroups
                           (
                               modify_src.num_subgroups,
                               modify_src.bis_sync
                           ))
    {
        APPL_ERR("Invalid BIS Sync !\n");
    }
    else
    {
        /* Copy the contents of Modify Source to Broadcast Receive State */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.id = modify_src.src_ID;

        switch(modify_src.pa_sync)
        {
            case BASS_PA_SYNC_DO_NOT_SYNC_TO_PA:
                /*
                 * If the PA_Sync parameter value written by the client is set
                 * to a value of 0x00 (Do not synchronize to PA), the server
                 * shall write a value of 0x00 (Not synchronized to PA) to the
                 * PA_Sync_State field of the selected Broadcast Receive State
                 * characteristic and the server shall not attempt to
                 * synchronize to the PA.
                 */
                /*
                 * PA_Sync_State value is updated in GA_BC_DISSOCIATE_CNF
                 * event and notified.
                 */
                /* If the SD is synchronized to a PA, then dissociate */
                if (appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state ==
                 BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA)
                {
                    is_dissociate = GA_TRUE;
                }
                break;

            case BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL:
                /*
                 * If the PA_Sync parameter value written by the client is set
                 * to a value of 0x01 (Synchronize to PA – PAST available), or
                 * 0x02 (Synchronize to PA – PAST not available), the server
                 * shall either write a value of 0x01 (SyncInfo Request) to the
                 * PA_Sync_State field of the selected Broadcast Receive State
                 * characteristic to request a client to transfer SyncInfo data
                 * to the server or the server shall attempt to synchronize
                 * with the PA by using the Periodic Advertising
                 * Synchronization Establishment procedure.
                 */

                /*
                 * If the SD is synchronized to a PA and if PA_Sync value is
                 * BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL, then do not
                 */
                if (appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state !=
                 BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA)
                {
                    /* Automated process to prepare Controller to receive PAST */
                    /*
                     * If GA_SUCCESS, then the PA_Sync_State will get updated as
                     * SYNC_INFO_REQUEST in the event Handling, Else the default
                     * value is retained.
                     */
                    (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_req_sync_info_past();
                }
                break;

            case BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL:
                /*
                 * - If the Addr_type is 1, then scan for broadcast source and
                 *   compare SID, Broadcast_ID, and Address. If they match, then
                 *   sync to that broadcast source
                 * - If the Addr_type is 0, then scan for broadcast source and
                 *   sync to that broadcast source
                 */
                if (appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state !=
                 BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA)
                {
                    /*
                     * If the PA_Sync_State is not
                     * BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA, Set the
                     * PA_Sync_State to BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED
                     */
                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
                     BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED;
                }
                break;

            default:
                /* Should not reach here */
                break;
        }

        /* Assign as written by the client */
        appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups =
         modify_src.num_subgroups;

        /* Update the subgroup value */
        appl_ga_bc_sd_bass_local_db.rx_state_inst[index].subgroup =
         modify_src.num_subgroups;

        if (0U != appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups)
        {
            /*
             * Update BIS_Sync to the global rx_state_inst. This is used while
             * syncing to the BIS (case 18). When GA_BC_ENABLE_CNF
             * event is received, then BIS_Sync_State is updated.
             */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_local_db.rx_state_inst[index].bis_sync[0U],
                &modify_src.bis_sync[0U],
                APPL_BC_MAX_NUM_OF_SUB_GROUPS
            );

            for (i = 0U; i < appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups; i++)
            {
                /*
                 * Check if the BIS_Sync Parameter of any of the subgroup has
                 * changed.
                 */
                if (appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bis_sync_state[i] !=
                 modify_src.bis_sync[i])
                {
                    if (0U == modify_src.bis_sync[i])
                    {
                        APPL_TRC("Trigger disable manually\n");
                        is_disable_required = GA_TRUE;
                    }
                    else
                    {
                        APPL_TRC("Trgger enable manually\n");
                        is_enable_required = GA_TRUE;
                    }
                }
            }

            /* Assign as written by the client */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata_len[0U],
                &modify_src.metadata_len[0U],
                APPL_BC_MAX_NUM_OF_SUB_GROUPS
            );
            /* Assign as written by the client */
            GA_mem_copy
            (
                &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.metadata[0U],
                &modify_src.metadata[0U],
                (APPL_BC_MAX_NUM_OF_SUB_GROUPS * GA_METADATA_LEN)
            );
        }

        /* Dont send notification if any manual operation is to be performed */
        if ((GA_TRUE == is_enable_required) || (GA_TRUE == is_disable_required))
        {
            *notify_rx_state = GA_FALSE;
        }

        if (GA_TRUE == is_dissociate)
        {
            *notify_rx_state = GA_FALSE;
            APPL_TRC("Trigger Dissociate manually\n");
        }

        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_handle_set_bc_code_cp
          (
            /* IN */ UINT8 * set_bc_code_data
          )
{
    GA_RESULT retval;
    BASS_BC_RX_STATE_INST rx_state_inst;
    UINT8 i;
    UINT8 marker;
    UINT8 source_id;

    retval = GA_SUCCESS;
    i = 0U;
    rx_state_inst = 0U;
    marker = 0U;
    source_id = 0U;

    source_id = set_bc_code_data[marker++];

    APPL_TRC
    (
        "Len: 0x%02X,  Source_ID: 0x%02X\n",
        (UCHAR)sizeof(source_id), source_id
    );

    APPL_TRC
    (
        "Len: 0x%02X,  Broadcast_Code: ",
        BASS_BC_CODE_MAX_LEN
    );
    for (i = 0U; i < BASS_BC_CODE_MAX_LEN; i++)
    {
        APPL_TRC("%02X ", set_bc_code_data[marker++]);
    }
    APPL_TRC("\n");

#ifdef BASS_DB
    /* Fetch the rx_state based on the Source_ID provided by the client. */
    rx_state_inst = appl_ga_bc_sd_fetch_rx_state_inst_from_src_id
                    (
                        source_id
                    );
    if (appl_ga_bc_sd_bass_local_db.rx_state_inst_count == rx_state_inst)
    {
        APPL_ERR("Invalid Source ID !\n");
        retval = BASS_APPL_ERR_CODE_INVALID_SRC_ID;
    }
    else
    {
        /*
         * Server shall use the provided Broadcast_Code value to attempt
         * to decrypt an encrypted BIS.
         */
        /* Copy the broadcast code to rx_state and proceed */
        GA_mem_copy
        (
            &appl_ga_bc_sd_params.bc_code[0U],
            &set_bc_code_data[1U],
            BASS_BC_CODE_MAX_LEN
        );
        /*
         * Do not update BIG_Encryption or send notification here, as we have
         * not triggered Enable yet. BIG_Encryption is updated as part of
         * GA_BC_ENABLE_CNF (Decrypting)/DISABLE_IND (BAD_Code) event.
         */
    }
#else /* BASS_DB */
    if (GA_TRUE == appl_ga_bc_sd_bass_local_db.rx_state_inst[source_id].isSrcAdded)
    {
        /* Copy the broadcast code to rx_state and proceed */
        GA_mem_copy
        (
            &appl_ga_bc_sd_params.bc_code[0U],
            &set_bc_code_data[1U],
            BASS_BC_CODE_MAX_LEN
        );
    }
    APPL_ERR("BASS_DB is undefined !\n");
#endif /* BASS_DB */

    return retval;
}

GA_RESULT appl_ga_bc_sd_handle_rem_src_cp(/* IN */ UINT8 source_id)
{
    GA_RESULT retval;
    UINT8     sg_index;
    BASS_BC_RX_STATE_INST rx_state_inst;

    retval = GA_SUCCESS;
    sg_index = 0U;
    rx_state_inst = 0U;

    APPL_TRC
    (
        "Len: 0x%02X,  Source_ID: 0x%04X\n",
        (UCHAR)sizeof(source_id), source_id
    );

#ifdef BASS_DB
    /*
     * Fetch the rx_state based on the Source_ID provided by the client here
     * and remove the complete entry.
     */
    rx_state_inst = appl_ga_bc_sd_fetch_rx_state_inst_from_src_id
                    (
                        source_id
                    );
    if (appl_ga_bc_sd_bass_local_db.rx_state_inst_count == rx_state_inst)
    {
        APPL_ERR("Invalid Source ID !\n");
        retval = BASS_APPL_ERR_CODE_INVALID_SRC_ID;
    }
    else
    {
        /*
         * Validate if the server is synchronized to the PA,
         * If Sync - Do not accept Remove operation.
         */
        if (BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA !=
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state)
        {
            for (sg_index = 0U; sg_index < appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups; sg_index++)
            {
                /*
                 * Validate if the server is synchronized to any BIS,
                 * If Sync - Do not accept Remove operation.
                 */
                if (BASS_BIS_SYNC_STATE_NOT_SYNCED_TO_BIS !=
                    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.bis_sync_state[sg_index])
                {
                    break;
                }
            }
            if (sg_index == appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups)
            {
                /*
                 * We have run through PA and all BISes, none of them
                 * are in Sync State, Hence accept Remove Operation
                 * OR
                 * The number of Subgroups is 0.
                 */
                APPL_TRC("BASS_DB_remove_source()...\n");
                retval = BASS_DB_remove_source(source_id);
                APPL_TRC("Retval - ");
                appl_ga_bc_sd_bass_display_ga_result(retval);
                if (GA_SUCCESS == retval)
                {
                    /*
                     * If BASS_DB remove source procedure is success, then
                     * update the local DB for that rx_state_inst
                     */
                    GA_mem_set
                    (
                        &appl_ga_bc_sd_bass_local_db.rx_state_inst[source_id],
                        0U,
                        sizeof(APPL_GA_BC_SD_RX_STATE_INST)
                    );

                    /* Reset the value in the global variable */
                    GA_mem_set
                    (
                        &appl_ga_bc_sd_bass_bc_rx_state,
                        0U,
                        sizeof(APPL_BC_SD_BASS_BC_RX_STATE)
                    );
                }

                /* Send Notification */
                BT_start_timer
                (
                    &appl_ga_bc_sd_cp_ntf_timer_handle,
                    1U, /* Delay of 1 seconds */
                    appl_ga_bc_sd_cp_ntf_timer_handler,
                    &appl_ga_bc_sd_bass_local_db.rx_state_inst[rx_state_inst].inst,
                    sizeof(BASS_BC_RX_STATE_INST)
                );
            }
            else
            {
                APPL_ERR("SD is synced to BIS(s) !\n");
            }
        }
        else
        {
            /*
             * The server shall accept the Remove Source Opcode if the source_ID
             * is valid.
             * The server shall not accept a Remove Source operation for a
             * Source_ID value that matches the Source_ID written by the client
             * in the Remove Source operation if the server is synchronized to
             * the PA and/or any BIS
             */
            APPL_ERR("SD is synced to PA !\n");
        }
    }
#else /* BASS_DB */
    appl_ga_bc_sd_bass_local_db.rx_state_inst[source_id].isSrcAdded = GA_FALSE;
#endif /* BASS_DB */
    return retval;
}

void appl_ga_bc_sd_cp_ntf_timer_handler
     (
        /* IN */ void * data,
        /* IN */ UINT16 datalen
     )
{
    GA_RESULT              retval;
    GA_ENDPOINT            device;
    UINT8                  i;
    BASS_BC_RX_STATE_INST  bc_rx_state_inst;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    bc_rx_state_inst = 0U;

    if ((NULL != data) && (0U != datalen))
    {
        bc_rx_state_inst = *((BASS_BC_RX_STATE_INST *)data);

        if (GA_SUCCESS == appl_ga_utils_get_conn_device
                          (
                              appl_ga_bc_sd_bass_local_db.conn_dev_index,
                              &device
                          ))
        {
            /* Pack the data for notification and also get the ntf_data size */
            if (0U != appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info_len)
            {
                appl_ga_bc_sd_ntf_data_size = appl_ga_bc_sd_pack_rx_state_ntf_data
                                              (
                                                  appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info,
                                                  &appl_ga_bc_sd_ntf_data[0U]
                                              );

                APPL_TRC
                (
                    "Device Address Type: 0x%02X\n",
                    device.bd_type
                );
                APPL_TRC("Device Address: ");
                for(i = 0U; i < GA_BD_ADDR_SIZE; i++)
                {
                    APPL_TRC("%02X ", device.bd_addr[i]);
                }
                APPL_TRC("\n");
            }
            else
            {
                /*
                 * If the Remove Source is executed successfully, then send
                 * empty data.
                 */
                appl_ga_bc_sd_ntf_data_size = 0U;
            }

            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("SENDING GA BASS NOTIFICATION\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("GA_bass_se_notify_bc_rx_state()...\n");
            retval = GA_bass_se_notify_bc_rx_state
                     (
                         &device,
                         bc_rx_state_inst,
                         appl_ga_bc_sd_ntf_data,
                         appl_ga_bc_sd_ntf_data_size
                     );
            APPL_TRC("Retval - ");
            appl_ga_bc_sd_bass_display_ga_result(retval);
        }
        else
        {
            APPL_ERR("Invalid Connection Device Index !\n");
        }
    }
    else
    {
        APPL_ERR("Invalid BC_RX_State Instance !\n");
    }

    /* Reset the timer handle to NULL */
    appl_ga_bc_sd_cp_ntf_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}
void appl_ga_bc_sd_past_rx_timer_handler
     (
        /* IN */ void * data,
        /* IN */ UINT16 datalen
     )
{
    UINT8 db_index;
    BASS_BC_RX_STATE_INST  bc_rx_state_inst;
    UINT8 * recvd_data;
    GA_RESULT retval;
    GA_ENDPOINT device;
    UINT8 i;

    if ((NULL == data) || (0U == datalen))
    {
        /* Reset the timer handle to NULL */
        appl_ga_bc_sd_past_rx_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        return;
    }

    recvd_data = (UINT8*)data;

    bc_rx_state_inst = (BASS_BC_RX_STATE_INST)recvd_data[0U];

    db_index = (UINT8)recvd_data[1U];

    /*
     * Update the rx_state_inst PA_Sync_State to
     * BASS_PA_SYNC_STATE_NOPAST as SD has not received PAST from BA.
     */
    appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.pa_sync_state =
        BASS_PA_SYNC_STATE_NOPAST;
#ifdef BASS_DB
    /* Update the Rx_State_Instance in BASS_DB */
    APPL_TRC("BASS_DB_modify_source()...\n");
    retval = BASS_DB_modify_source
             (
                 db_index,
                 &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_sd_bass_display_ga_result(retval);
#endif /* BASS_DB */
    /* Send Notification */
    if (GA_SUCCESS == appl_ga_utils_get_conn_device
                      (
                          appl_ga_bc_sd_bass_local_db.conn_dev_index,
                          &device
                      ))
    {
        /* Pack the data for notification and also get the ntf_data size */
        if (0U != appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info_len)
        {
            appl_ga_bc_sd_ntf_data_size = appl_ga_bc_sd_pack_rx_state_ntf_data
                                          (
                                              appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info,
                                              &appl_ga_bc_sd_ntf_data[0U]
                                          );
            APPL_TRC
            (
                "Device Address Type: 0x%02X\n",
                device.bd_type
            );
            APPL_TRC("Device Address: ");
            for(i = 0U; i < GA_BD_ADDR_SIZE; i++)
            {
                APPL_TRC("%02X ", device.bd_addr[i]);
            }
            APPL_TRC("\n");
        }
        else
        {
            /* No action */
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA BASS NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_bass_se_notify_bc_rx_state()...\n");
        retval = GA_bass_se_notify_bc_rx_state
                 (
                     &device,
                     bc_rx_state_inst,
                     appl_ga_bc_sd_ntf_data,
                     appl_ga_bc_sd_ntf_data_size
                 );
        APPL_TRC("Retval - ");
        appl_ga_bc_sd_bass_display_ga_result(retval);
    }
    else
    {
        APPL_ERR("Invalid Connection Device Index !\n");
    }

    /* Reset the timer handle to NULL */
    appl_ga_bc_sd_past_rx_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void appl_ga_bc_sd_update_conn_dev_index(GA_ENDPOINT * device)
{
    UINT8 conn_dev_index;

    /* Initialize */
    conn_dev_index = 0U;

    APPL_DBG("[APPL][GA][BC_SD]: >> appl_ga_bc_sd_update_conn_dev_index\n");

    /* Fetch conn_dev_index from device */
    if (GA_SUCCESS == appl_ga_utils_get_conn_device_index
                      (
                          *device,
                          &conn_dev_index
                      ))
    {
        /* Compare conn_dev_index */
        if (conn_dev_index != appl_ga_bc_sd_bass_local_db.conn_dev_index)
        {
            /* Update Connection Device Index if they don't match */
            appl_ga_bc_sd_bass_local_db.conn_dev_index = conn_dev_index;
        }
        else
        {
            /* Do Nothing */
        }
    }

    APPL_DBG("[APPL][GA][BC_SD]: << appl_ga_bc_sd_update_conn_dev_index\n");
    return;
}

GA_RESULT appl_ga_bc_sd_snk_setup_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          )
{
    GA_RESULT         retval;
    BC_SINK_PARAMS  * snk_param;

    retval = GA_FAILURE;
    snk_param = NULL;

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else if (IS_APPL_GA_BC_ROLE_BC_SINK(bc_role))
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    if ((GA_FALSE == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status) &&
        (AUDIO_ISO_IDLE == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state))
    {
        if (GA_SUCCESS == appl_ga_utils_audio_snk_set_entry
                          (
                              snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle,
                              snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].cs_conf_sg_bis
                          ))
        {
            retval = appl_ga_utils_audio_snk_le_setup_iso_data_path
                     (
                         snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                     );

            if (GA_SUCCESS == retval)
            {
                /* ISO Data Path is already set */
                APPL_TRC
                (
                    "[APPL][GA][%s]: ISO Data Path Setup Status: Already Complete\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
                snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_SETUP_COMPLETE;
            }
            else if (GA_CONTINUE == retval)
            {
                /* ISO Data Path is Ongoing */
                APPL_TRC
                (
                    "[APPL][GA][%s]: ISO Data Path Setup Status: In-progress\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
                snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                    AUDIO_ISO_UPDATE_IN_PROGRESS;
            }
            else
            {
                APPL_ERR
                (
                    "[APPL][GA][%s]: ISO Data Path Setup Status: Failed !\n",
                    SD_GET_ROLE_STRING(bc_role)
                );
            }
        }
        else
        {
            APPL_ERR
            (
                "[APPL][GA][%s]: ISO Data Path Setup Status: Set entry failed !\n",
                SD_GET_ROLE_STRING(bc_role)
            );
        }
    }
    else
    {
        APPL_DBG("[APPL][GA][%s]: Invalid state\n", SD_GET_ROLE_STRING(bc_role));
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_snk_remove_iso
          (
              UINT8 sg_info_index,
              UINT8 bis_info_index
          )
{
    GA_RESULT         retval;
    BC_SINK_PARAMS  * snk_param;

    /* Initialize */
    retval = GA_FAILURE;
    snk_param = NULL;

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else if (IS_APPL_GA_BC_ROLE_BC_SINK(bc_role))
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    if ((GA_TRUE == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].streaming_status) &&
        (AUDIO_ISO_SETUP_COMPLETE == snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state))
    {
        retval = appl_ga_utils_audio_snk_le_remove_iso_data_path
                 (
                     snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle
                 );

        if (GA_SUCCESS == retval)
        {
            APPL_TRC
            (
                "[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n",
                SD_GET_ROLE_STRING(bc_role)
            );
            snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_IDLE;
        }
        else if (GA_CONTINUE == retval)
        {
            snk_param->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].iso_state =
                AUDIO_ISO_UPDATE_IN_PROGRESS;
            APPL_TRC
            (
                "[APPL][GA][%s]: ISO Data Path Removal Status: In-progress\n",
                SD_GET_ROLE_STRING(bc_role)
            );
        }
        else
        {
            /* State mismatch */
            APPL_ERR
            (
                "[APPL][GA][%s]: ISO Data Path Removal Status: Failed !\n",
                SD_GET_ROLE_STRING(bc_role)
            );
        }
    }
    else
    {
        APPL_DBG
        (
            "[APPL][GA][%s]: Already in Streaming state\n",
            SD_GET_ROLE_STRING(bc_role)
        );
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_snk_update_receiving_audio
          (
              UINT16                     bis_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8                      bps,
              UINT8                      status
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

#ifdef AUDIO_SNK_ISO_SUPPORT
    /* As we have already removed the entry, update entry is not required */
    if (GA_FALSE == status)
    {
        retval = GA_SUCCESS;
        /*
         * TODO: Check if needs to be printed. We are not triggering remove_iso,
         * instead its removed along with remove_BIG
         */
#if 0
        APPL_TRC("[APPL][GA][%s]: ISO Data Path Removal Status: Complete\n", SD_GET_ROLE_STRING(bc_role));
#endif /* 0 */
    }
    else
    {
        retval = appl_ga_utils_audio_snk_update_entry
                 (
                     bis_handle,
                     AUDIO_IGNORE_VALUE,
                     AUDIO_IGNORE_VALUE,
                     status
                 );
    }
    if (GA_SUCCESS == retval)
    {
#ifdef AUDIO_SNK_LC3_SUPPORT
        if (GA_TRUE == status)
        {
            retval = appl_ga_utils_audio_snk_create_lc3_decoder
                     (
                         cs_conf,
                         bps
                     );
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("[APPL][GA][%s]: LC3 Decoder Setup Status: Created\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: LC3 Decoder Setup Status: Could not create !\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_snk_is_lc3_decoder_running())
            {
                retval = appl_ga_utils_audio_snk_delete_lc3_decoder();
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][%s]: LC3 Decoder Setup Status: Deleted\n",
                        SD_GET_ROLE_STRING(bc_role));
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: LC3 Decoder Setup Status: Could not delete !\n",
                        SD_GET_ROLE_STRING(bc_role));
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][%s]: LC3 Decoder Setup Status: Already Deleted\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
        }
#else /* AUDIO_SNK_LC3_SUPPORT */
        APPL_ERR("AUDIO_SNK_LC3_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_LC3_SUPPORT */
#ifdef AUDIO_SNK_PL_SUPPORT
        if (GA_TRUE == status)
        {
            retval = appl_ga_utils_audio_snk_setup_playback_pl
                     (
                         cs_conf,
                         bps,
                         AUDIO_SNK_PL_BUFFER_SIZE
                     );
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("[APPL][GA][%s]: Audio PL Playback Setup Status: Success\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: Audio PL Playback Setup Status: Failed !\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
            retval = appl_ga_utils_audio_snk_start_playback_pl();
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("[APPL][GA][%s]: Audio PL Playback Start Process Status: Success\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
            else
            {
                APPL_ERR("[APPL][GA][%s]: Audio PL Playback Start Process Status: Failed !\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_snk_is_playback_pl_running())
            {
                if (GA_SUCCESS == appl_ga_utils_audio_snk_stop_playback_pl())
                {
                    APPL_TRC("[APPL][GA][%s]: Audio PL Playback Stop Process Status: Success\n",
                        SD_GET_ROLE_STRING(bc_role));
                }
                else
                {
                    APPL_ERR("[APPL][GA][%s]: Audio PL Playback Stop Process Status: Failed !\n",
                        SD_GET_ROLE_STRING(bc_role));
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][%s]: Audio PL Playback Status: Already Stopped\n",
                    SD_GET_ROLE_STRING(bc_role));
            }
        }
#else /* AUDIO_SNK_PL_SUPPORT */
        APPL_ERR("AUDIO_SNK_PL_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_PL_SUPPORT */
    }
    else
    {
        APPL_ERR("[APPL][GA][%s]: ISO Data Path Setup Status: Update entry failed !\n",
            SD_GET_ROLE_STRING(bc_role));
    }
#else /* AUDIO_SNK_ISO_SUPPORT */
APPL_ERR("AUDIO_SNK_ISO_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_ISO_SUPPORT */

    return retval;
}

void appl_ga_bc_sd_bass_display_ga_result(GA_RESULT status)
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
        }
    }
    return;
}

void appl_ga_bc_sd_snk_add_rem_src_to_db(UINT8 *addr, UINT8 addr_type)
{
    UINT8 index;

    if (rem_src_db_count < APPL_GA_BC_SD_SNK_REM_SRC_DB_COUNT)
    {
        for (index = 0U; index < GA_BD_ADDR_SIZE; index++)
        {
            rem_src_db[rem_src_db_count].bd_addr[index] = *(addr + index);
        }
        rem_src_db[rem_src_db_count].bd_type = addr_type;
        rem_src_db_count++;
    }

    return;
}

void appl_ga_bc_sd_snk_clear_rem_src_db(void)
{
    UINT8 index;

    for (index = 0U; index < rem_src_db_count; index++)
    {
        GA_INIT_ENDPOINT(&rem_src_db[index]);
    }
    rem_src_db_count = 0U;
}

UINT8 appl_ga_bc_sd_snk_is_rem_src_in_db(UINT8* addr, UINT8 addr_type)
{
    UINT8 index;

    if ((rem_src_db_count != 0U)
        &&
        (rem_src_db_count < APPL_GA_BC_SD_SNK_REM_SRC_DB_COUNT))
    {
        for (index = 0U; index < rem_src_db_count; index++)
        {
            if (GA_COMPARE_ENDPOINT_ADDR
               (
                   &addr[0U],
                   &rem_src_db[index].bd_addr[0U]
               ))
            {
                if (GA_COMPARE_ENDPOINT_TYPE(addr_type, rem_src_db[index].bd_type))
                {
                    return GA_TRUE;
                }
            }
        }
    }

    return GA_FALSE;
}

GA_RESULT appl_ga_bc_sd_snk_fetch_sg_bis_info_index_from_bis_index
          (
              UINT8   bis_index,
              UINT8 * sg_info_index,
              UINT8 * bis_info_index
          )
{
    GA_RESULT  retval;
    UINT8      i;
    UINT8      j;
    BC_SINK_PARAMS * snk_param;

    retval = GA_FAILURE;
    i = 0U;
    j = 0U;
    snk_param = NULL;

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else if (IS_APPL_GA_BC_ROLE_BC_SINK(bc_role))
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    for (i = 0U; i < snk_param->bc_num_subgroups; i++)
    {
        for (j = 0U; j < snk_param->bc_subgroup_info[i].bc_nstream; j++)
        {
            if (bis_index ==
                    snk_param->bc_subgroup_info[i].bis_info[j].bis_index)
            {
                *sg_info_index = i;
                *bis_info_index = j;
                retval = GA_SUCCESS;
                break;
            }
        }
    }

    return retval;
}

void appl_ga_bc_sd_extract_bis_indeces_from_bis_sync
     (
         UINT32  bis_sync,
         UINT8 * bis_indeces,
         UINT8 * bis_count
     )
{
    UINT8 i;

    /*
     * Example:
     * bis_sync = 10110 (Bis_indeces set = 2, 3, 5).
     *
     * To check if the index is set, do bitwise AND. To go to the next index,
     * divide bis_sync by 10.
     *
     * bis_sync = 10110, Bis_Index_1 = (bis_sync & 1U) = 0U -> NOT SET
     * bis_sync = 1011, Bis_Index_2 = (bis_sync & 1U) = 1U -> SET
     * bis_sync = 101, Bis_Index_3 = (bis_sync & 1U) = 1U -> SET
     * bis_sync = 10, Bis_Index_4 = (bis_sync & 1U) = 0U -> NOT SET
     * bis_sync = 1, Bis_Index_5 = (bis_sync & 1U) = 1U -> SET
     */

    /* Run for maximum number of BIS */
    for (i = 0U; i < 31U; i++)
    {
        /* If the BIS is set, then update the index and count */
        if (bis_sync & 0x01U)
        {
            bis_indeces[*bis_count] = (i+1U);
            *bis_count += 1U;
        }
        /* Go to the next index */
        bis_sync = (bis_sync/10U);

        /* TODO: Temporily 31. Update with Macro */
        /* If BIS_Sync is 0 or BIS_Count is 31 (MAX), then break */
        if ((0U == bis_sync) || (31U == *bis_count))
        {
            break;
        }
    }

    return;
}

void appl_ga_bc_sd_get_input_for_enable(void)
{
    UINT8 sg_index;
    BASS_BC_RX_STATE_INST bc_rx_state_inst;
    int choice;
    UINT8 i;

    /*
     * Fetch the RX_State_Inst to check the BIS_indeces
     * received during Add/Modify Source for the Subroups
     */
    if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                      (
                          appl_ga_bc_sd_bass_local_db.bc_src_dev,
                          &bc_rx_state_inst,
                          NULL
                      ))
    {
        APPL_TRC("Refer to GA_BC_SOURCE_CONFIG_IND for SG and BIS Index\n");

        /* Reset the BIS_Count and BIS_Indeces before updating */
        appl_ga_bc_sd_params.bis_count_enable = 0U;
        GA_mem_set
        (
            &appl_ga_bc_sd_params.bis_index_enable[0U],
            0U,
            (APPL_BC_MAX_BIS_PER_SUB_GROUP * APPL_BC_MAX_NUM_OF_SUB_GROUPS)
        );

        for (sg_index = 0U; sg_index < appl_ga_bc_sd_bass_local_db.rx_state_inst[bc_rx_state_inst].subgroup; sg_index++)
        {
            UINT8 sg_bis_count = 0U;
            if (BASS_BIS_SYNC_NO_PREFERENCE == appl_ga_bc_sd_bass_local_db.rx_state_inst[bc_rx_state_inst].bis_sync[sg_index])
            {
                /*
                 * Validate the BIS_Sync received is
                 * BASS_BIS_SYNC_NO_PREFERENCE, take input
                 * from the user. Update the Bis_sync global variable
                 * here since this will be used during
                 * GA_BC_ENABLE_CNF event for updating
                 * bc_rx_state_inst BIS_Sync_State.
                 */
                /* Take Num_of_BIS for the Subgroup */
                CONSOLE_OUT("Enter Num_BIS for Subgroup[%d]: ", sg_index);
                CONSOLE_IN("%d", &choice);
                sg_bis_count = (UCHAR)choice;

                CONSOLE_OUT("Enter BIS_Indeces for Subgroup[%d]:\n", sg_index);
                for (i = 0U; i < sg_bis_count; i++)
                {
                    CONSOLE_IN("%d", &choice);

                    /* Copy the BIS_Index to the structure */
                    appl_ga_bc_sd_params.bis_index_enable[appl_ga_bc_sd_params.bis_count_enable] =
                        (UCHAR)choice;

                    /* Increment BIS_Count */
                    appl_ga_bc_sd_params.bis_count_enable++;
                }
            }
            else
            {
                /*
                 * Extract BIS_Indeces from the Bis_Sync
                 * and pass it to bis_index for enable
                 */
                appl_ga_bc_sd_extract_bis_indeces_from_bis_sync
                (
                    appl_ga_bc_sd_bass_local_db.rx_state_inst[bc_rx_state_inst].bis_sync[sg_index],
                    &appl_ga_bc_sd_params.bis_index_enable[appl_ga_bc_sd_params.bis_count_enable],
                    &sg_bis_count
                );
                /* Update total BIS Count. */
                appl_ga_bc_sd_params.bis_count_enable += sg_bis_count;
            }
        }
    }

    return;
}

void appl_ga_bc_sd_update_bis_sync_state_and_notify(UINT8 enable)
{
    GA_RESULT retval;
    UINT8  db_index;
    UINT8  bis_enable_count;
    UINT8  sg_info_index;
    UINT8  bis_info_index;
    BASS_BC_RX_STATE_INST  bc_rx_state_inst;

    /* Initialize */
    retval = GA_FAILURE;

    /*
     * Set the bits of bis_sync_state in bc_rx_state to 0, if GA_BC_DISABLE_CNF/
     * GA_BC_DISABLE_IND event is received.
     *
     * - For each subgroup, for each BIS the server attempts to synchronize
     *   to and fails, the server shall set the BIS_index value of that BIS to
     *   0b0 in the BIS_Sync_State field of the Broadcast Receive State
     *   characteristic.
     * - If the server fails to synchronize to the BIG, the server shall write
     *   a value of 0xFFFFFFFF (Failed to synchronize to BIG) to the
     *   BIS_Sync_State field of the selected Broadcast Receive State
     *   characteristic.
     *  - For each subgroup, if the server has synchronized to a BIS and then
     *    loses synchronization to that BIS, the server shall set the BIS_index
     *    value of that BIS to 0b0 in the BIS_Sync_State field of the selected
     *    Broadcast Receive State characteristic.
     */

    /* GA_BC_ENABLE_CNF event. Update BIS_Sync_State of all Subgroup BIS(s) */
    if (GA_TRUE == enable)
    {
        for (bis_enable_count = 0U; bis_enable_count < appl_ga_bc_sd_params.bis_count_enable; bis_enable_count++)
        {
            /* For each subgroup */
            for (sg_info_index = 0U; sg_info_index < appl_ga_bc_sd_params.bc_num_subgroups; sg_info_index++)
            {
                /* For each BIS(s) */
                for (bis_info_index = 0U; bis_info_index < appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
                {
                    /*
                     * Check if the user given input of BIS_Index for
                     * enable is present in the PA. If present, then
                     * update the BIS_Sync_State for the BIS.
                     */
                    if (appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_index & appl_ga_bc_sd_params.bis_index_enable[bis_enable_count])
                    {
                        BASS_BIS_SYNC_STATE_UPDATE_SYNC_TO_BIS_INDEX
                        (
                            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_bis_sync[sg_info_index],
                            appl_ga_bc_sd_params.bis_index_enable[bis_enable_count]
                        );
                        /*
                         * After updating the BIS_Sync_State for
                         * the BIS, go to next user given input BIS.
                         */
                         break;
                    }
                }
                /*
                 * After updating the BIS_Sync_State for
                 * the BIS, go to next user given input BIS.
                 */
                if (bis_info_index < appl_ga_bc_sd_params.bc_subgroup_info[sg_info_index].bc_nstream)
                {
                    break;
                }
            }
        }
    }
    else
    {
        /*
         * GA_BC_DISABLE_CNF/GA_BC_DISABLE_IND event. Reset BIS_Sync_State
         * of all Subgroup BIS(s)
         */

        /* Reset the BIS_Sync_State for all the Subgroups of the RX_State */
        for (sg_info_index = 0U; sg_info_index < appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.num_subgroups; sg_info_index++)
        {
            appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state_bis_sync[sg_info_index] = 0U;
        }
    }

    /*
     * Fetch the RX_State_Inst and DB_Index before updating in BASS_DB and
     * sending notification.
     */
    if (GA_SUCCESS == appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
                      (
                          appl_ga_bc_sd_bass_local_db.bc_src_dev,
                          &bc_rx_state_inst,
                          &db_index
                      ))
    {
        (GA_IGNORE_RETURN_VALUE)appl_ga_bc_sd_update_rx_state_and_notify
                                (
                                    db_index,
                                    bc_rx_state_inst,
                                    GA_TRUE
                                );
    }
    else
    {
        APPL_ERR("Invalid Device !");
    }

    return;
}

GA_RESULT appl_ga_bc_sd_fetch_rx_state_inst_db_index_from_dev
          (
              GA_ENDPOINT  device,
              UINT8      * bc_rx_state_inst,
              UINT8      * db_index
          )
{
    UINT8      i;
    GA_RESULT  retval;

#ifdef BASS_DB
    GA_BASS_BC_RX_STATE  rx_state = {0};
#endif /* BASS_DB */

    i = 0U;
    retval = GA_FAILURE;

    for (i = 0U; i < appl_ga_bc_sd_bass_local_db.rx_state_inst_count; i++)
    {
#ifdef BASS_DB
        /* Check if source is added for the instance */
        if (GA_TRUE == appl_ga_bc_sd_bass_local_db.rx_state_inst[i].isSrcAdded)
        {
            /* Fetch the Rx State based on the DB index. */
            retval = BASS_DB_fetch_rx_state_instance
                     (
                         appl_ga_bc_sd_bass_local_db.rx_state_inst[i].db_index,
                         &rx_state
                     );
            /*
             * Check if the fetched Rx State info is proper and
             * Advertiser Address of the Broadcast Source in RX State is
             * same as received device's BD Address and Addr Type.
             */
            if ((GA_SUCCESS == retval) &&
                (GA_TRUE == GA_COMPARE_BRR_DEV_ADDR
                            (
                                device.bd_addr,
                                rx_state.info.addr
                            )) &&
                (GA_TRUE == GA_COMPARE_BRR_DEV_ADDR_TYPE
                            (
                                device.bd_type,
                                rx_state.info.addr_type
                            )))
            {
                /* Get the rx_state_inst */
                if (NULL != bc_rx_state_inst)
                {
                    *bc_rx_state_inst =
                        appl_ga_bc_sd_bass_local_db.rx_state_inst[i].inst;
                }
                /* Get the db_index */
                if (NULL != db_index)
                {
                    *db_index =
                        appl_ga_bc_sd_bass_local_db.rx_state_inst[i].db_index;
                }
                break;
            }
        }
#else /* BASS_DB */
        APPL_ERR("BASS_DB is undefined !\n");
#endif /* BASS_DB */
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_validate_for_duplicate_bis_index_in_subgroups
          (
              UINT8    num_subgroups,
              UINT32 * bis_sync
          )
{
    GA_RESULT  retval;
    UINT8      i;
    UINT32     bis_sync_ref;

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;
    /* Copy the bis_sync value of the 1st subgroup */
    bis_sync_ref = bis_sync[0U];

    /*
     * 1st subgroup BIS_sync is already copied to bis_sync_ref.
     *
     * From 2nd subgroup, until the last, check if same bit is set in atleast
     * 2 subgroups.
     *
     * E.g., bis_sync_ref = 3 (0011), bis_sync[1] = 4 (0100) -> VALID
     *       bis_sync_ref = 3 (0011), bis_sync[2] = 2 (0010) -> INVALID
     * Then bis_sync[2] has bit 2 set which was already set for a previous
     * subgroup bis.
     */
    for (i = 1U; i < num_subgroups; i++)
    {
        if (0U != (bis_sync_ref & bis_sync[i]))
        {
            /*
             * If any of the previous bits for bis_sync are set in bis_sync[i],
             * then return GA_FAILURE
             */
            retval = GA_FAILURE;
            break;
        }
        else
        {
            /* If any of the previous bits for bis_sync are not set in bis_sync[i],
             * then OR the bis_sync[i] to bis_sync_ref to validate for the next
             * subgroups.
             */
            bis_sync_ref = (bis_sync_ref | bis_sync[i]);
        }
    }

    return retval;
}

GA_RESULT appl_ga_bc_sd_update_rx_state_and_notify
          (
              UINT8  db_index,
              UINT8  bc_rx_state_inst,
              UINT8  send_notification
          )
{
    GA_RESULT retval;

#ifdef BASS_DB
    /* Update the Rx_State_Instance in BASS_DB */
    APPL_TRC("BASS_DB_modify_source()...\n");
    retval = BASS_DB_modify_source
             (
                 db_index,
                 &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_sd_bass_display_ga_result(retval);
#endif /* BASS_DB */

    if (GA_TRUE == send_notification)
    {
        /* Send Notification */
        BT_start_timer
        (
            &appl_ga_bc_sd_cp_ntf_timer_handle,
            1U, /* Delay of 1 second */
            appl_ga_bc_sd_cp_ntf_timer_handler,
            &bc_rx_state_inst,
            sizeof(BASS_BC_RX_STATE_INST)
        );
    }
    return retval;
}

void appl_ga_bc_sd_snk_disable_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    GA_RESULT  retval;
    BC_SINK_PARAMS* snk_param;

    /* Initialize */
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(data);

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    GA_IGNORE_UNUSED_PARAM(datalen);

    /* TODO: Pass the Session as Data */

    APPL_TRC ("GA_broadcast_disable\n");
    retval = GA_broadcast_disable
             (
                 snk_param->enable_synchandle
             );
    APPL_TRC("Retval - ");
    appl_ga_bc_sd_bass_display_ga_result(retval);

    /* Reset the timer handle to NULL */
    appl_ga_bc_sd_snk_disable_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void appl_ga_bc_reset_on_disable(void)
{
    UINT8  sg_info_index;
    UINT8  bis_info_index;
    BC_SINK_PARAMS * sd_snk_ptr = NULL;

    /* Check role and get the structure reference for SD/BC_Sink */
    if (BC_ROLE_SD & bc_role)
    {
        sd_snk_ptr = &appl_ga_bc_sd_params;
    }
    else if (BC_ROLE_BC_SNK & bc_role)
    {
        sd_snk_ptr = &appl_ga_bc_sink_params;
    }
    else
    {
        APPL_ERR("Invalid State !\n");
        return;
    }

    /* Reset BIS_Handle for all BIS(s) to BC_INVALID_HANDLE */
    for (sg_info_index = 0U; sg_info_index < sd_snk_ptr->bc_num_subgroups; sg_info_index++)
    {
        for (bis_info_index = 0U; bis_info_index < sd_snk_ptr->bc_subgroup_info[sg_info_index].bc_nstream; bis_info_index++)
        {
            sd_snk_ptr->bc_subgroup_info[sg_info_index].bis_info[bis_info_index].bis_handle =
                BC_INVALID_HANDLE;
        }
    }

    /* Reset the BIS_Indeces in Params */
    GA_mem_set
    (
        &sd_snk_ptr->bis_index_enable[0U],
        0U,
        sd_snk_ptr->bis_count_enable
    );

    /* Reset the BIS_Count in SD Params */
    sd_snk_ptr->bis_count_enable = 0U;

    if (BC_ROLE_SD & bc_role)
    {
        appl_ga_bc_sd_update_bis_sync_state_and_notify(GA_FALSE);
    }

    return;
}

GA_RESULT appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle
          (
              UINT16   bis_conn_handle,
              UINT8  * sg_info_index,
              UINT8  * bis_info_index
          )
{
    GA_RESULT         retval;
    UINT8             sg_info_index_local;
    UINT8             bis_info_index_local;
    BC_SINK_PARAMS  * snk_param;

    /* Initialize */
    retval = GA_FAILURE;
    sg_info_index_local = 0U;
    bis_info_index_local = 0U;

    APPL_DBG
    (
        "[APPL][GA][%s]: >> appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle\n",
        SD_GET_ROLE_STRING(bc_role)
    );

    if (IS_APPL_GA_BC_ROLE_SD(bc_role))
    {
        snk_param = &appl_ga_bc_sd_params;
    }
    else
    {
        snk_param = &appl_ga_bc_sink_params;
    }

    if ((NULL == sg_info_index) || (NULL == bis_info_index))
    {
        return GA_FAILURE;
    }

    for (sg_info_index_local = 0U; sg_info_index_local < snk_param->bc_num_subgroups; sg_info_index_local++)
    {
        for (bis_info_index_local = 0U; bis_info_index_local < snk_param->bc_subgroup_info[sg_info_index_local].bc_nstream; bis_info_index_local++)
        {
            if (bis_conn_handle ==
                snk_param->bc_subgroup_info[sg_info_index_local].bis_info[bis_info_index_local].bis_handle)
            {
                *sg_info_index = sg_info_index_local;
                *bis_info_index = bis_info_index_local;
                retval = GA_SUCCESS;
                break;
            }
        }
    }

    APPL_DBG
    (
        "[APPL][GA][%s]: << appl_ga_bc_sd_snk_get_sg_index_bis_info_index_from_bis_conn_handle\n",
        SD_GET_ROLE_STRING(bc_role)
    );

    return retval;
}

#ifdef BASS_DB
BASS_BC_RX_STATE_INST appl_ga_bc_sd_fetch_rx_state_inst_from_src_id
                      (
                          UINT8 src_id
                      )
{
    BASS_BC_RX_STATE_INST  rx_state_inst;
    GA_RESULT              retval;

    /* Initialize */
    retval = GA_FAILURE;

    for (rx_state_inst = 0U; rx_state_inst < appl_ga_bc_sd_bass_local_db.rx_state_inst_count; rx_state_inst++)
    {
        if (GA_TRUE == appl_ga_bc_sd_bass_local_db.rx_state_inst[rx_state_inst].isSrcAdded)
        {
            retval = BASS_DB_fetch_rx_state_instance
                     (
                         appl_ga_bc_sd_bass_local_db.rx_state_inst[rx_state_inst].db_index,
                         &appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state
                     );

            if ((GA_SUCCESS == retval) &&
                (src_id == appl_ga_bc_sd_bass_bc_rx_state.bc_rx_state.info.id))
            {
                /*
                 * DB Index is
                 * appl_ga_bc_sd_bass_local_db.rx_state_inst[rx_state_inst].db_index
                 */
                break;
            }
        }
    }

    return rx_state_inst;
}
#endif /* BASS_DB */
#endif /* BAP_BROADCAST */
