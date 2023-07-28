/**
 *  \file appl_ga_ucc.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Unicast Client exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ga_ucc.h"
#include "appl_hci.h"
#include "appl_ga_utils_audio_src.h"
#include "appl_ga_utils_audio_snk.h"

#ifdef BAP_UCC

/* --------------------------------------------- Global Definitions */

static const char ucc_menu_options[] = " \n\
================ GA Unicast Client MENU ================ \n\
    0. Exit. \n\
    1. Refresh this Menu. \n\
\n\
================ Profile Management (ASCS) ~ Operations(BAP Procedures) ============ \n\
    2. ASE Discover. \n\
    3. ASE Codec Configuration. \n\
    4. ASE Setup CIG Parameters - HCI. \n\
    5. ASE QoS Configuration. \n\
    6. ASE Enable. \n\
    7. ASE Receiver Start Ready for Local ASE Sink. \n\
    8. ASE Update. \n\
    9. ASE Disable. \n\
   10. ASE Receiver Stop Ready for Local ASE Sink. \n\
   11. ASE Release. \n\
\n\
================ ASE Management ~ Display ============ \n\
   15. Display Remote ASEs Data. \n\
\n\
================ CIS Management ~ HCI ============ \n\
   16. ASE Create CIS. \n\
   17. ASE Disconnect CIS. \n\
   18. Remove CIG \n\
\n\
================ Device Management ~ Setting ============ \n\
   19. Display all Connected Devices. \n\
\n\
Your Option ?: ";

extern UINT8 uc_role;

/* Used to differentiate ongoing scan */
extern UINT8 is_scan_enabled;

/* Timer Handle for Automate Create CIS  */
DECL_STATIC BT_timer_handle  appl_ga_ucc_bap_create_cis_timer_handle;

/* --------------------------------------------- Data types /Structures */
typedef struct _REMOTE_ASCS_INFO
{
    ASCS_ASE_INFO  ascs;
    UINT8          conn_dev_index;
}REMOTE_ASCS_INFO;

REMOTE_ASCS_INFO remote_ascs[MAX_REM_ASCS_INFO];
/* --------------------------------------------- Macros */
#define GA_UCC_GET_LOCAL_ROLE_FOR_ASE(remote_role, local_role) \
        if (GA_ROLE_SOURCE == (remote_role)) \
            (local_role) = GA_ROLE_SINK; \
        else \
            (local_role) = GA_ROLE_SOURCE;

void appl_ga_ucc_update_transmitting_audio
     (
         UINT16 cis_handle,
         GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
         UINT8 qos_conf_set_id,
         UINT8 bps,
         UINT8 status
     );

void appl_ga_ucc_update_receiving_audio
     (
         UINT16 cis_handle,
         GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
         UINT8 bps,
         UINT8 status
     );

#define appl_ga_ucc_start_transmitting_audio(cis_handle, cs_conf, qos_conf_set_id, bps) \
        appl_ga_ucc_update_transmitting_audio(cis_handle, cs_conf, qos_conf_set_id, bps, GA_TRUE)

#define appl_ga_ucc_stop_transmitting_audio(cis_handle, cs_conf, qos_conf_set_id, bps) \
        appl_ga_ucc_update_transmitting_audio(cis_handle, cs_conf, qos_conf_set_id, bps, GA_FALSE)

#define appl_ga_ucc_start_receiving_audio(cis_handle, cs_conf, bps) \
        appl_ga_ucc_update_receiving_audio(cis_handle, cs_conf, bps, GA_TRUE)

#define appl_ga_ucc_stop_receiving_audio(cis_handle, cs_conf, bps) \
        appl_ga_ucc_update_receiving_audio(cis_handle, cs_conf, bps, GA_FALSE)

GA_RESULT appl_ga_ucc_get_common_params_for_asecp_operations
          (
              GA_CONTEXT  * context,
              UINT8       * conn_dev_index,
              GA_ENDPOINT * dev,
              UINT8       * ase_count,
              UINT8       * ascs_index
          );
void appl_ga_ucc_ascs_config_codec(void);
void appl_ga_ucc_ascs_config_qos(void);
void appl_ga_ucc_hci_set_cig_params(void);
void appl_ga_ucc_ascs_enable(void);
void appl_ga_ucc_receiver_start_ready(void);
void appl_ga_ucc_update_metadata(void);
void appl_ga_ucc_ascs_disable(void);
void appl_ga_ucc_receiver_stop_ready(void);
void appl_ga_ucc_ascs_release(void);
void appl_ga_ucc_display_ase_data(void);
void appl_ga_ucc_ase_create_cis(void);
void appl_ga_ucc_ase_disconnect_cis(void);
void appl_ga_ucc_remove_cig(void);

UINT8 appl_ga_ucc_get_ase_index_from_ase_id
      (
          UINT8 ase_id,
          UINT8 ascs_index
      );
GA_RESULT appl_ga_ucc_get_ascs_index_from_conn_dev_index
          (
              UINT8   conn_dev_index,
              UINT8 * ascs_index
          );

GA_RESULT appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
          (
              UINT16   cis_conn_handle,
              UINT8  * ascs_index,
              UINT8  * ase_count,
              UINT8  * ase_index
          );

GA_RESULT appl_ga_fetch_ascs_index_from_acl_conn_handle
          (
              UINT16   acl_conn_handle,
              UINT8  * conn_dev_index,
              UINT8  * ascs_index
          );

GA_RESULT appl_ga_ucc_fetch_free_ascs_db_index(UINT8 * ascs_index);

UINT8 appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming
      (
          UINT8 ascs_index,
          UINT8 ase_index
      );
UINT8 appl_ga_ucc_get_cig_cis_shared_ase_index
      (
          UINT8   ascs_index,
          UINT8   ase_index,
          UINT8 * shared_ase_index
      );
void appl_ga_ucc_bap_get_params_for_create_cis
     (
         /* OUT */ UINT8 * cis_count,
         /* OUT */ UINT8 * conn_dev_index,
         /* OUT */ UINT8 * ase_id
     );
void appl_ga_ucc_bap_update_cis_state
     (
         UINT8   cis_count,
         UINT8 * conn_dev_index,
         UINT8 * ase_id
     );
GA_RESULT appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis
          (
              UINT8  * cis_count,
              UINT16 * acl_handle_array,
              UINT16 * cis_handle_array
          );
GA_RESULT appl_ga_ucc_bap_create_cis
          (
              UINT8    cis_count,
              UINT16 * cis_handle,
              UINT16 * acl_handle
          );
void appl_ga_ucc_terminate_cis(UINT16 cis_conn_handle);

void appl_ga_ucc_remove_iso_cis(UINT8 ascs_index, UINT8 ase_index);

GA_RESULT appl_ga_ucc_remove_iso(UINT8 ascs_index, UINT8 ase_index);

void appl_ga_ucc_setup_cis_iso
     (
         UINT8       ascs_index,
         UINT8       ase_index,
         GA_ENDPOINT device,
         UINT8       ase_id
     );
void appl_ga_ucc_setup_iso(UINT8 ascs_index, UINT8 ase_index);

GA_RESULT appl_ga_ucc_is_cis_connect_allowed
          (
              UINT8 ascs_index,
              UINT8 ase_index
          );
GA_RESULT appl_ga_ucc_is_cis_disconnect_allowed
          (
              UINT8 ascs_index,
              UINT8 ase_index
          );

void appl_ga_ucc_handle_cis_established_event
     (
         UINT16 cis_conn_handle,
         UINT16 cis_conn_status
     );

void appl_ga_ucc_handle_iso_data_path_event
     (
         UINT16 cis_conn_handle,
         UCHAR  state,
         UCHAR  status
     );
void appl_ga_ucc_handle_set_cig_param_event(UCHAR* event_data, UINT8 status);

void appl_ga_ucc_handle_remove_cig_event
     (
         UCHAR status,
         UINT8 cig_id
     );

void appl_ga_ucc_update_cis_conn_state
     (
         UINT8   ascs_index,
         UINT8   ase_index,
         UINT8   conn,
         UINT16  cis_conn_status
     );

void appl_ga_ucc_handle_create_cis_event(UINT8 status);

void appl_ga_ucc_handle_disconnection_event
     (
         UINT8  conn_status,
         UINT16 conn_handle
     );

void appl_ga_ucc_bap_create_cis_timer_handler
     (
         void  * data,
         UINT16  datalen
     );

GA_RESULT appl_ga_ucc_bap_remove_remote_ascs_for_dev(UINT8 conn_dev_index);

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_ucc_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          )
{
    GA_RESULT  retval;
    UINT8      value_1;
    UINT16     value_2;
    UINT8      cig_id;
    UCHAR      num_report;
    UINT16     svc_uuid;
    UINT8      marker;
    UINT8      adtype;
    UINT8      adlength;
    UCHAR      status;
    UINT16     cis_conn_handle;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;
    value_1 = 0U;
    value_2 = 0U;
    cig_id = 0U;
    marker = 0U;
    status = 0U;
    cis_conn_handle = 0U;
    adtype = 0U;
    adlength = 0U;
    i = 0U;
    svc_uuid = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_hci_cb_handler\n");

    /* Switch on the Event Code */
    switch (event_code)
    {
    case HCI_LE_META_EVENT:
        hci_unpack_1_byte_param(&value_1, &event_data[marker]);
        marker++;
        switch (value_1)
        {
        case HCI_LE_CIS_ESTABLISHED_SUBEVENT:
            hci_unpack_1_byte_param(&status, &event_data[marker]);
            marker++;
            hci_unpack_2_byte_param(&cis_conn_handle, &event_data[marker]);
            marker += 2U;
            appl_ga_ucc_handle_cis_established_event
            (
                cis_conn_handle,
                status
            );
            break;

        case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT:
            /*
             * Deliver the HCI event parameters for all meta events skipping the
             * subevent code.
             */
            if (APPL_GA_SCAN_FOR_UCS == is_scan_enabled)
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

                                /*
                                 * Check if any one of the following Service UUID
                                 * is in the Service Data value:
                                 * - CAS
                                 * - VCS, VOCS, AICS
                                 * - MICS
                                 * - GTBS, TBS
                                 * - GMCS, MCS, OTS
                                 */
                                if ((GA_SERVICE_CAS == svc_uuid) ||
                                    (GA_SERVICE_VCS == svc_uuid) ||
                                    (GA_SERVICE_VOCS == svc_uuid) ||
                                    (GA_SERVICE_AICS == svc_uuid) ||
                                    (GA_SERVICE_MICS == svc_uuid) ||
                                    (GA_SERVICE_GMCS == svc_uuid) ||
                                    (GA_SERVICE_MCS == svc_uuid) ||
                                    (GA_SERVICE_UUID_OTS == svc_uuid) ||
                                    (GA_SERVICE_GTBS == svc_uuid) ||
                                    (GA_SERVICE_TBS == svc_uuid) ||
                                    (GA_SERVICE_TMAS == svc_uuid))
                                {
                                    /* UCC is scanning for UCS, Found UCS, Reset marker */
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
                            else if ((GA_ADTYPE_INCOMP_16BIT_UUID_LIST == adtype) ||
                                     (GA_ADTYPE_COMP_16BIT_UUID_LIST == adtype))
                            {
                                /*
                                 * Number of Service UUIDs present in the
                                 * Incomplete/Complete list.
                                 * svc_uuid_marker/2 = No of Service UUID
                                 */
                                UINT8 svc_uuid_marker;

                                svc_uuid_marker = 0U;
                                /*
                                 * ADData will have list of Service UUID.
                                 * Check for all Service UUIDs
                                 */
                                /*
                                 * (adlength - ad type(1U)) == svc_uuid_marker
                                 * means all the Service UUIDs are checked
                                 */
                                while ((adlength - 1U) != svc_uuid_marker)
                                {
                                    /* Extract the Value - Service UUID from the event data */
                                    GA_UNPACK_LE_2_BYTE(&svc_uuid, &event_data[marker]);
                                    marker += 2U;
                                    value_1 -= 2U;
                                    svc_uuid_marker += 2U;

                                    /*
                                     * Check if any one of the following Service ID
                                     * is in the Service Data value:
                                     * - CAS
                                     * - VCS, VOCS, AICS
                                     * - MICS
                                     * - GTBS, TBS
                                     * - GMCS, MCS, OTS
                                     */
                                    if ((GA_SERVICE_CAS == svc_uuid) ||
                                        (GA_SERVICE_VCS == svc_uuid) ||
                                        (GA_SERVICE_VOCS == svc_uuid) ||
                                        (GA_SERVICE_AICS == svc_uuid) ||
                                        (GA_SERVICE_MICS == svc_uuid) ||
                                        (GA_SERVICE_MCS == svc_uuid) ||
                                        (GA_SERVICE_UUID_OTS == svc_uuid) ||
                                        (GA_SERVICE_GTBS == svc_uuid) ||
                                        (GA_SERVICE_TBS == svc_uuid) ||
                                        (GA_SERVICE_TMAS == svc_uuid))

                                    {
                                        /* UCC is scanning for UCS, Found UCS, Reset marker */
                                        marker = 0U;

                                        break;
                                    }
                                    else
                                    {
                                        /* Service UUID not interested, Validate further */
                                        svc_uuid = 0x0000U;
                                    }
                                }

                                if (0U == marker)
                                {
                                    /* Interested Service UUID is found */
                                    break;
                                }
                                else
                                {
                                    /* Service UUID not interested, Validate further */
                                    svc_uuid = 0x0000U;
                                    /* marker and value_1 are already updated */
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
                    if ((GA_SERVICE_CAS == svc_uuid) ||
                        (GA_SERVICE_VCS == svc_uuid) ||
                        (GA_SERVICE_VOCS == svc_uuid) ||
                        (GA_SERVICE_AICS == svc_uuid) ||
                        (GA_SERVICE_MICS == svc_uuid) ||
                        (GA_SERVICE_MCS == svc_uuid) ||
                        (GA_SERVICE_UUID_OTS == svc_uuid) ||
                        (GA_SERVICE_GTBS == svc_uuid) ||
                        (GA_SERVICE_TBS == svc_uuid) ||
                        (GA_SERVICE_TMAS == svc_uuid))

                    {
                        /* Display the Device */
                        appl_ga_utils_parse_and_display_ea_report_subevent(event_data, event_datalen);

                        break;
                    }
                }
            }
            break;

        default:
            break;
        }
        break;

    case HCI_COMMAND_COMPLETE_EVENT:
        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, &event_data[marker]);
        marker++;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, &event_data[marker]);
        marker += 2U;

        /* Command Status */
        hci_unpack_1_byte_param(&status, &event_data[marker]);
        marker++;

        /* Command Return Parameters */
        if (event_datalen > 4U)
        {
            switch (value_2)
            {
            case HCI_LE_SET_HOST_FEATURE_OPCODE:
                break;

            case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE:
                hci_unpack_2_byte_param(&cis_conn_handle, &event_data[marker]);
                marker += 2U;
                appl_ga_ucc_handle_iso_data_path_event
                (
                    cis_conn_handle,
                    AUDIO_ISO_DATA_SETUP,
                    status
                );
                break;

            case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE:
                hci_unpack_2_byte_param(&cis_conn_handle, &event_data[marker]);
                marker += 2U;
                appl_ga_ucc_handle_iso_data_path_event
                (
                    cis_conn_handle,
                    AUDIO_ISO_DATA_REMOVE,
                    status
                );
                appl_ga_ucc_terminate_cis(cis_conn_handle);
                break;

            case HCI_LE_SET_CIG_PARAMETERS_OPCODE:
                appl_ga_ucc_handle_set_cig_param_event
                (
                    &event_data[marker],
                    status
                );
                break;

            case HCI_LE_REMOVE_CIG_OPCODE:
                /* Command Status */
                hci_unpack_1_byte_param(&status, &event_data[marker]);
                marker++;
                /* CIG ID */
                hci_unpack_1_byte_param(&cig_id, &event_data[marker]);
                marker++;
                appl_ga_ucc_handle_remove_cig_event(status, cig_id);
                break;

            default:
                break;
            }
        }
        break;

    case HCI_COMMAND_STATUS_EVENT:
        /* Status */
        hci_unpack_1_byte_param(&status, &event_data[marker]);
        marker++;

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, &event_data[marker]);
        marker++;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, &event_data[marker]);
        marker += 2U;

        switch (value_2)
        {
        case HCI_LE_CREATE_CIS_OPCODE:
            appl_ga_ucc_handle_create_cis_event(status);
            break;

        default:
            break;
        }
        break;

    case HCI_DISCONNECTION_COMPLETE_EVENT:
        /* Status */
        hci_unpack_1_byte_param(&status, &event_data[marker]);
        marker++;
        /* Connection Handle - CIS or ACL */
        hci_unpack_2_byte_param(&value_2, &event_data[marker]);
        marker += 2U;
        appl_ga_ucc_handle_disconnection_event(status, value_2);
        break;

    default:
        break;

    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_hci_cb_handler\n");
    return GA_SUCCESS;
}

void appl_ga_ucc_handle_remove_cig_event
     (
         UCHAR status,
         UINT8 cig_id
     )
{
    UINT8  ascs_index;
    UINT8  ase_index;

    /* Initialize */
    ascs_index = 0U;
    ase_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_handle_remove_cig_event\n");

    if (0x00U == status)
    {
        /*
         * Match the CIG ID of all the ASEs and initialize the status of
         * CIG and CIS to CIG_STATE_IDLE and CIS_STATE_IDLE, CIS handle, ISO State
         * and CIG and CIS ID
         */
        for (ascs_index = 0U; ascs_index < MAX_REM_ASCS_INFO; ascs_index++)
        {
            for (ase_index = 0U; ase_index < remote_ascs[ascs_index].ascs.ase_count; ase_index++)
            {
                if (cig_id == remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id =
                        INVALID_CIG_CIS_ID;
                    remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cis_id =
                        INVALID_CIG_CIS_ID;
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                        CIG_STATE_IDLE;
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                        CIS_STATE_IDLE;
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle =
                        INVALID_CIS_HANDLE;
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                        AUDIO_ISO_IDLE;
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_handle_remove_cig_event\n");
    return;
}

void appl_ga_ucc_handle_iso_data_path_event
     (
         UINT16 cis_conn_handle,
         UCHAR  state,
         UCHAR  status
     )
{
    UINT8      ase_index_list[MAX_UC_ASE] = { 0U };
    UINT8      ase_index;
    UINT8      ascs_index;
    UINT8      ase_count;
    UINT8      index;
    UINT8      local_role;

    /* Initialize */
    ase_index = 0U;
    ascs_index = 0U;
    ase_count = 0U;
    index = 0U;
    local_role = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_handle_iso_data_path_event\n");

    /* Fetch the ASCS Index and ASE Index List based on the CIS Conn Handle */
    if (GA_SUCCESS == appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
                      (
                          cis_conn_handle,
                          &ascs_index,
                          &ase_count,
                          ase_index_list
                      ))
    {
        for (index = 0U; index < ase_count; index++)
        {
            ase_index = ase_index_list[index];

            if ((AUDIO_ISO_DATA_SETUP == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS ==
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state))
            {
                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].role,
                    local_role
                );
                if (GA_ROLE_SOURCE == local_role)
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_src_update_entry
                                      (
                                          cis_conn_handle,
                                          AUDIO_ISO_DATA_SETUP,
                                          status,
                                          AUDIO_IGNORE_VALUE
                                      ))
                    {
                        APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Complete\n", UC_GET_ROLE_STRING(local_role));
                        /*
                         * Until ASE state of an ASE is moved to enabling,
                         * setup iso API will not get call and iso state of an ASE
                         * will not change
                         */
                        remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                            AUDIO_ISO_SETUP_COMPLETE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Update entry failed !\n", UC_GET_ROLE_STRING(local_role));
                    }
                }
                else
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_snk_update_entry
                                      (
                                          cis_conn_handle,
                                          AUDIO_ISO_DATA_SETUP,
                                          status,
                                          AUDIO_IGNORE_VALUE
                                      ))
                    {
                        APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Complete\n", UC_GET_ROLE_STRING(local_role));
                        /*
                         * Until ASE state of an ASE is moved to enabling,
                         * setup iso API will not get call and iso state of an ASE
                         * will not change
                         */
                        remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                            AUDIO_ISO_SETUP_COMPLETE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Update entry failed !\n", UC_GET_ROLE_STRING(local_role));
                    }
                }
            }
            else if ((AUDIO_ISO_DATA_REMOVE == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS ==
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state))
            {
                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].role,
                    local_role
                );
                if (GA_ROLE_SOURCE == local_role)
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_src_update_entry
                                      (
                                          cis_conn_handle,
                                          AUDIO_ISO_DATA_REMOVE,
                                          status,
                                          AUDIO_IGNORE_VALUE
                                      ))
                    {
                        APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Complete\n", UC_GET_ROLE_STRING(local_role));
                        remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                            AUDIO_ISO_IDLE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][UCCs][%s]: ISO Data Path Removal Status Update entry failed !\n", UC_GET_ROLE_STRING(local_role));
                    }
                }
                else
                {
                    if (GA_SUCCESS == appl_ga_utils_audio_snk_update_entry
                                      (
                                          cis_conn_handle,
                                          AUDIO_ISO_DATA_REMOVE,
                                          status,
                                          AUDIO_IGNORE_VALUE
                                      ))
                    {
                        APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Complete\n", UC_GET_ROLE_STRING(local_role));
                        remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                            AUDIO_ISO_IDLE;
                    }
                    else
                    {
                        APPL_ERR("[APPL][GA][UCCs][%s]: ISO Data Path Removal Status Update entry failed !\n", UC_GET_ROLE_STRING(local_role));
                    }
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_handle_iso_data_path_event\n");
    return;
}

void appl_ga_ucc_handle_set_cig_param_event(UCHAR * event_data, UINT8 status)
{
    UINT8   cig_id;
    UINT8   cis_count;
    UINT16  cis_conn_handle;
    UINT8   index;
    UINT8   j;
    UINT8   k;
    UINT8   ase_index_using_same_cis;

    /* Initialize */
    cig_id = 0U;
    cis_count = 0U;
    cis_conn_handle = 0U;
    index = 0U;
    j = 0U;
    k = 0U;
    ase_index_using_same_cis = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_handle_set_cig_param_event\n");

    if (0x00U == status)
    {
        /* Extract the CIG_ID */
        hci_unpack_1_byte_param(&cig_id, event_data);
        APPL_TRC("CIG ID: %d\n", cig_id);
        event_data++;

        /* Extract the CIS_Count */
        hci_unpack_1_byte_param(&cis_count, event_data);
        APPL_TRC("CIS Count: %d\n", cis_count);
        event_data++;

        for (index = 0U; index < cis_count; index++)
        {
            /* Extract the CIS_Connection_Handle */
            hci_unpack_2_byte_param(&cis_conn_handle, event_data);
            APPL_TRC("CIS Conn Handle: 0x%04X\n", cis_conn_handle);

            for (j = 0U; j < MAX_REM_ASCS_INFO; j++)
            {
                for (k = 0U; k < remote_ascs[j].ascs.ase_count; k++)
                {
                    if ((cig_id == remote_ascs[j].ascs.ase[k].qos_conf.channel.cig_id)
                        &&
                        (CIG_STATE_WAITING == remote_ascs[j].ascs.ase[k].cig_cis_info.cig_state)
                        &&
                        (CIS_STATE_IDLE == remote_ascs[j].ascs.ase[k].cig_cis_info.cis_state))
                    {
                        remote_ascs[j].ascs.ase[k].cig_cis_info.cig_state = CIG_STATE_CREATED;
                        remote_ascs[j].ascs.ase[k].cig_cis_info.cis_state = CIS_STATE_INITIALIZED;
                        remote_ascs[j].ascs.ase[k].cig_cis_info.handle = cis_conn_handle;

                        /* Validate if there are any other ASE waiting and is bidirectional */
                        if (GA_TRUE == appl_ga_ucc_get_cig_cis_shared_ase_index
                                       (
                                           j, k, &ase_index_using_same_cis
                                       ))
                        {
                            remote_ascs[j].ascs.ase[ase_index_using_same_cis].cig_cis_info.cig_state =
                             CIG_STATE_CREATED;
                            remote_ascs[j].ascs.ase[ase_index_using_same_cis].cig_cis_info.cis_state =
                             CIS_STATE_INITIALIZED;
                            remote_ascs[j].ascs.ase[ase_index_using_same_cis].cig_cis_info.handle =
                             cis_conn_handle;
                        }

                        /* Move to the next CIS */
                        event_data += 2;
                        break;
                    }
                }
                if (remote_ascs[j].ascs.ase_count != k)
                {
                    /*
                     * Skip if CIS is already assigned to the current Device,
                     * Do not run through other devices for the same CIS.
                     */
                    break;
                }
            }

            /* Skip if not found in entire list */
            if (MAX_REM_ASCS_INFO == j)
            {
                /* This case should never be hit */
                APPL_DBG("Unexpected CIS !\n");

                /* Move to the next CIS */
                event_data += 2;
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_handle_set_cig_param_event\n");
    return;
}

void appl_ga_ucc_handle_create_cis_event(UINT8 status)
{
    UINT8 ascs_index;
    UINT8 ase_index;

    if (0x00U != status)
    {
        /*
         * Fetch the details of the ASE for which we have triggered Create
         * CIS.
         */
        for (ascs_index = 0U; ascs_index < MAX_REM_ASCS_INFO; ascs_index++)
        {
            for (ase_index = 0U; ase_index < remote_ascs[ascs_index].ascs.ase_count; ase_index++)
            {
                if (CIS_STATE_WAITING ==
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle =
                        INVALID_CIS_HANDLE;
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                        CIS_STATE_IDLE;
                }
            }
        }
    }

    return;
}

void appl_ga_ucc_handle_disconnection_event
     (
         UINT8  conn_status,
         UINT16 conn_handle
     )
{
    UINT8 ascs_index;
    UINT8 ase_count;
    UINT8 ase_index[MAX_UC_ASE];
    UINT8 index;
    UINT8 conn_dev_index;
    UINT8 local_role;

    ascs_index = 0U;
    ase_count = 0U;
    index = 0U;
    conn_dev_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_handle_disconnection_event\n");

    if (GA_SUCCESS == conn_status)
    {
        /*
         * Validate if it's a cis_conn_handle.
         * For Valid cis_conn_handle, fetch ASCS index and all ase_index
         * with same cis_conn_handle. There can be more than 1 ASE having the
         * the same CIS especially Bi-Directional.
         */
        if (GA_SUCCESS == appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
                          (
                              conn_handle,
                              &ascs_index,
                              &ase_count,
                              ase_index
                          ))
        {
            for (index = 0U; index < ase_count; index++)
            {
                /* Update CIS conn state */
                appl_ga_ucc_update_cis_conn_state
                (
                    ascs_index,
                    ase_index[index],
                    GA_FALSE,
                    conn_status
                );

                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index[index]].role,
                    local_role
                );
                /*
                 * Since disconnection is already done, we cannot do ISO
                 * Removal, So update the Audio PL and LC3 to stop.
                 */
                if (GA_ROLE_SOURCE == local_role)
                {
                    appl_ga_ucc_stop_transmitting_audio
                    (
                        conn_handle,
                        remote_ascs[ascs_index].ascs.ase[ase_index[index]].cs_conf,
                        remote_ascs[ascs_index].ascs.ase[ase_index[index]].qos_conf_set_id,
                        AUDIO_NUM_OF_BITS_PER_SAMPLE
                    );
                }
                else
                {
                    appl_ga_ucc_stop_receiving_audio
                    (
                        conn_handle,
                        remote_ascs[ascs_index].ascs.ase[ase_index[index]].cs_conf,
                        AUDIO_NUM_OF_BITS_PER_SAMPLE
                    );
                }

                /* Check if the release is initiated locally for this ASE */
                if (GA_TRUE ==
                    remote_ascs[ascs_index].ascs.ase[ase_index[index]].is_release_initiated)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index[index]].is_release_initiated =
                        GA_FALSE;
                }
            }
        }
        /*
         * It is not a CIS Handle, Validate if it is a ACL Handle
         * For Valid acl_conn_handle, fetch conn_dev_index and ascs_index
         * with same conn_dev_index
         */
        else if (GA_SUCCESS == appl_ga_fetch_ascs_index_from_acl_conn_handle
                               (
                                   conn_handle,
                                   &conn_dev_index,
                                   &ascs_index
                               ))
        {
            /**
             * Free the remote ASCS instance
             * - Commented as this has the count used during Get/Set context validation
             * - TODO: Have clean ways to Get/Set application data as required
             */
            /* GA_mem_set(&remote_ascs[ascs_index], 0U, sizeof(REMOTE_ASCS_INFO)); */

            /*
             * TODO: If we dont get CIS Disconnect and get ACL disconnect, Handle
             * appl_ga_ucc_stop_transmitting_audio or appl_ga_ucc_stop_receiving_audio
             * like above?
             */
        }
        else
        {
            APPL_DBG
            (
                "[APPL][GA][UCC]: No registered ASEs connected with "
                "this handle 0x%04x", conn_handle
            );
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_handle_disconnection_event\n");
    return;
}

void appl_ga_ucc_handle_cis_established_event
     (
         UINT16 cis_conn_handle,
         UINT16 cis_conn_status
     )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      ascs_index;
    UINT8      ase_count;
    UINT8      ase_index[MAX_UC_ASE];

    ascs_index = 0U;
    ase_count = 0U;
    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_handle_cis_established_event\n");

    /* Fetch ascs_index and ase_index, from cis conn handle */
    retval = appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
             (
                 cis_conn_handle,
                 &ascs_index,
                 &ase_count,
                 ase_index
             );

    if (GA_SUCCESS == retval)
    {
        /* Setup ISO for each ASE */
        for (index = 0U; index < ase_count; index++)
        {
            /* Update CIS conn state */
            appl_ga_ucc_update_cis_conn_state
            (
                ascs_index,
                ase_index[index],
                GA_TRUE,
                cis_conn_status
            );

            /*
             * Need not check the cis_conn_status, since
             * appl_ga_ucc_update_cis_conn_state would have updated the
             * cis_conn_status, which will get validated inside this
             * function to proceed or not.
             */
            appl_ga_ucc_setup_iso
            (
                ascs_index,
                ase_index[index]
            );
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_handle_cis_established_event\n");
    return;
}

GA_RESULT appl_ga_ucc_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT      retval;
    UINT8          ascs_index;
    UINT8          ase_index;
    UINT8          conn_dev_index;
    GA_ASE_INFO  * ase;
    UINT8          local_role;

    ascs_index = 0U;
    conn_dev_index = 0U;
    retval = GA_FAILURE;
    ase_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_cb_handler\n");

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    if ((NULL == ga_data) || (0U == ga_datalen))
    {
        APPL_ERR("ga_data is NULL !\n");
        return GA_FAILURE;
    }

    /* Based on the device, Fetch the conn_dev_index */
    retval = appl_ga_utils_get_conn_device_index(*device, &conn_dev_index);

    if (GA_SUCCESS == retval)
    {
        /* Based on the conn_dev_index, Fetch the ASCS Index */
        retval = appl_ga_ucc_get_ascs_index_from_conn_dev_index
                 (
                     conn_dev_index,
                     &ascs_index
                 );
        if (GA_SUCCESS != retval)
        {
            if (GA_SEP_DISCOVER_CNF == ga_event)
            {
                /* It's a new device. Add it in the DB */
                if (GA_SUCCESS !=
                        appl_ga_ucc_fetch_free_ascs_db_index(&ascs_index))
                {
                    APPL_DBG("[APPL][GA][UCC]: ASCS DB is full !\n");
                    return GA_FAILURE;
                }
                else
                {
                    /* Update the conn_dev_index into ASCS DB */
                    remote_ascs[ascs_index].conn_dev_index = conn_dev_index;

                    ase = (GA_ASE_INFO *)ga_data;
                }
            }
            else
            {
                APPL_DBG
                (
                    "[APPL][GA][UCC]: ASCS data for this device is not present !\n"
                );
                return GA_FAILURE;
            }
        }
        else
        {
            ase = (GA_ASE_INFO *)ga_data;
            /* Fetch ASE index based on ASCS_Index and ASE_ID */
            ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                        (
                            ascs_index,
                            ase->data[0U]
                        );
        }
    }
    else
    {
        APPL_DBG("[APPL][GA][UCC]: Device is not present !\n");
        return GA_FAILURE;
    }

    switch (ga_event)
    {
    case GA_SEP_DISCOVER_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_DISCOVER_CNF (0x%02X)\n",
            ga_event
        );

        APPL_TRC("\n");
        if (GA_ROLE_SINK & ase->ase_role)
        {
            appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_SINK_ASE);
        }
        else if (GA_ROLE_SOURCE & ase->ase_role)
        {
            appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_SOURCE_ASE);
        }
        if ((GA_SUCCESS == ga_status) || (GA_CONTINUE == ga_status))
        {
            APPL_TRC("SEP Context ID - 0x%02X\n", ase->sep_id);
            appl_ga_utils_bap_display_role(ase->ase_role);
            APPL_TRC("Data Length: 0x%02X\n", ase->datalen);
            APPL_TRC("Data:\n");
            appl_ga_utils_ucc_ascs_parse_and_display_ase_data
            (
                ase->data,
                ase->datalen
            );

            remote_ascs[ascs_index].ascs.ase[remote_ascs[ascs_index].ascs.ase_count].role =
                ase->ase_role;

            remote_ascs[ascs_index].ascs.ase[remote_ascs[ascs_index].ascs.ase_count].id =
                ase->data[0U];

            remote_ascs[ascs_index].ascs.ase[remote_ascs[ascs_index].ascs.ase_count].state =
                ase->data[1U];
            remote_ascs[ascs_index].ascs.ase_count++;

            /* TODO: If State is Codec Configured, QoS Configured */
        }
        break;

    case GA_SEP_CONFIGURE_NTF: /* Fall - through */
    case GA_SEP_CONFIGURE_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : ");
        if (GA_SEP_CONFIGURE_CNF == ga_event)
        {
            APPL_TRC("GA_SEP_CONFIGURE_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_SEP_CONFIGURE_NTF (0x%02X)\n", ga_event);
        }

        if (GA_SUCCESS == ga_status &&
            (ase_index < remote_ascs[ascs_index].ascs.ase_count))
        {
            /* Update the ASE state */
            remote_ascs[ascs_index].ascs.ase[ase_index].state =
                ase->data[1U];
            if (GA_SEP_CONFIGURE_NTF == ga_event)
            {
                /*
                 * TODO: Save the codec specific configuration for ASE by
                 * extracting CS Conf from LTV
                 */
            }
        }
        else if (GA_SUCCESS != ga_status &&
                 (ase_index < remote_ascs[ascs_index].ascs.ase_count))
        {
            /* We might have set this during Config Codec CP */
            GA_mem_set
            (
                &remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                0U,
                sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
            );
        }
        else
        {
            APPL_ERR("Unknown ASE ID !\n");
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_SETUP_NTF: /* Fall - through */
    case GA_SEP_SETUP_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : ");
        if (GA_SEP_SETUP_CNF == ga_event)
        {
            APPL_TRC("GA_SEP_SETUP_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_SEP_SETUP_NTF (0x%02X)\n", ga_event);
        }

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }

            if (GA_SEP_SETUP_NTF == ga_event)
            {
                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].role,
                    local_role
                );
                if (GA_ROLE_SOURCE == local_role)
                {
                    /* We are Audio Source, Stop Transmitting Audio data */
                    appl_ga_ucc_stop_transmitting_audio
                    (
                        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                        remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                        remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id,
                        AUDIO_NUM_OF_BITS_PER_SAMPLE
                    );
                }
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_START_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_START_CNF (0x%02X)\n",
            ga_event
        );

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);

        /*
         * State is in Enabling, Trigger CIS and ISO Setup Chain, Move the CIS
         * State to Waiting
         */
        appl_ga_ucc_setup_cis_iso
        (
            ascs_index,
            ase_index,
            *device,
            ase->data[0U]
        );

        /*
         * We will receive GA_SEP_START_CNF event for each ASE_ID
         * separately. So, calling create CIS for each CIS connection Handle
         * separately will result in calling back to back Create CIS without waiting
         * for Command Complete Event for HCI_LE_CREATE_CIS_OPCODE.
         * and HCI_LE_CIS_ESTABLISHED_SUBEVENT event.
         * As per Core Spec V5.3, Page 2566, LE Create CIS command:
         * If the Host issues this command before all the
         * HCI_LE_CIS_Established events from the previous use of the command
         * have been generated, the Controller shall return the error code
         * Command Disallowed (0x0C).
         */
        /*
         * A timer is started and BT_hci_le_create_cis() is called after
         * all the ASE have been moved to Enabling State(CIS State - Waiting)
         * as part of GA_SEP_START_CNF.
         */
        if (BT_TIMER_HANDLE_INIT_VAL == appl_ga_ucc_bap_create_cis_timer_handle)
        {
            BT_start_timer
            (
                &appl_ga_ucc_bap_create_cis_timer_handle,
                2U,
                appl_ga_ucc_bap_create_cis_timer_handler,
                NULL,
                0U
            );
        }
        break;

    case GA_SEP_RX_START_READY_NTF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : GA_SEP_RX_START_READY_NTF (0x%02X)\n", ga_event);
        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                if ((GA_ASE_STATE_STREAMING == ase->data[1U]) &&
                    (GA_ASE_STATE_ENABLING == remote_ascs[ascs_index].ascs.ase[ase_index].state))
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].state =
                        ase->data[1U];
                    GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                    (
                        remote_ascs[ascs_index].ascs.ase[ase_index].role,
                        local_role
                    );
                    if (GA_ROLE_SOURCE == local_role)
                    {
                        /* We are Audio Source, Start Transmitting Audio data */
                        appl_ga_ucc_start_transmitting_audio
                        (
                            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                            remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                            remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id,
                            AUDIO_NUM_OF_BITS_PER_SAMPLE
                        );
                    }
                    else
                    {
                        /*
                         * This shall never occur since Rx Start Ready is
                         * initiated on Sink ASE.
                         */
                    }
                }
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }
        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_RX_START_READY_CNF: /* Fall - through */
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : GA_SEP_RX_START_READY_CNF (0x%02X)\n", ga_event);
        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                if ((GA_ASE_STATE_STREAMING == ase->data[1U]) &&
                    (GA_ASE_STATE_ENABLING == remote_ascs[ascs_index].ascs.ase[ase_index].state))
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].state =
                        ase->data[1U];
                    GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                    (
                        remote_ascs[ascs_index].ascs.ase[ase_index].role,
                        local_role
                    );
                    if (GA_ROLE_SINK == local_role)
                    {
                        /* We are Audio Sink, Start Receiving Audio data */
                        appl_ga_ucc_start_receiving_audio
                        (
                            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                            remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                            AUDIO_NUM_OF_BITS_PER_SAMPLE
                        );
                    }
                    else
                    {
                        /*
                         * This shall never occur since Rx Start Ready is
                         * initiated on Sink ASE.
                         */
                    }
                }
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }
        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_RX_STOP_READY_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_STOP_READY_CNF (0x%02X)\n",
            ga_event
        );

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            GA_UCC_GET_LOCAL_ROLE_FOR_ASE
            (
                remote_ascs[ascs_index].ascs.ase[ase_index].role,
                local_role
            );
            if (GA_ROLE_SINK == local_role)
            {
                /* We are Audio Sink, Stop Receiving Audio data */
                appl_ga_ucc_stop_receiving_audio
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                    remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                    AUDIO_NUM_OF_BITS_PER_SAMPLE
                );
            }
        }
        break;

    case GA_SEP_UPDATE_NTF: /* Fall - through */
    case GA_SEP_UPDATE_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        if (GA_SEP_UPDATE_NTF == ga_event)
        {
            APPL_TRC("[Event]       : GA_SEP_UPDATE_NTF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("[Event]       : GA_SEP_UPDATE_CNF (0x%02X)\n", ga_event);
        }
        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Don't do anything */
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }
        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_SUSPEND_NTF: /* Fall - through */
    case GA_SEP_SUSPEND_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : ");
        if (GA_SEP_SUSPEND_CNF == ga_event)
        {
            APPL_TRC("GA_SEP_SUSPEND_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_SEP_SUSPEND_NTF (0x%02X)\n", ga_event);
        }

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_ASE_STATE_QOS_CONF ==
                remote_ascs[ascs_index].ascs.ase[ase_index].state)
            {
                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].role,
                    local_role
                );
                if (GA_ROLE_SOURCE == local_role)
                {
                    /* We are Audio Source, Stop Transmitting Audio data */
                    appl_ga_ucc_stop_transmitting_audio
                    (
                        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                        remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                        remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id,
                        AUDIO_NUM_OF_BITS_PER_SAMPLE
                    );
                }
            }
            else if (GA_ASE_STATE_DISABLING ==
                remote_ascs[ascs_index].ascs.ase[ase_index].state)
            {
                GA_UCC_GET_LOCAL_ROLE_FOR_ASE
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].role,
                    local_role
                );
                if (GA_ROLE_SINK == local_role)
                {
                    /*
                     * Wait to trigger Receiver Stop Ready Operation and handle
                     * the Stop in the GA_SEP_RX_STOP_READY_CNF event.
                     *
                     * As per BAP Spec: BAP_v1.0.1: Page 99
                     * 5.6.5.1 Receiver Stop Ready
                     * The Unicast Client is ready to stop consuming audio data
                     * transmitted for that ASE by the Unicast Server.
                     * The Unicast Server in the Audio Source role should not
                     * stop transmitting audio data for a Source ASE in the
                     * Disabling state until the Unicast Server transitions the
                     * ASE to the QoS Configured state.
                     */
                }
            }
        }
        break;

    case GA_SEP_RELEASE_NTF: /* Fall - through */
    case GA_SEP_RELEASE_CNF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : ");
        if (GA_SEP_RELEASE_CNF == ga_event)
        {
            APPL_TRC("GA_SEP_RELEASE_CNF (0x%02X)\n", ga_event);
        }
        else
        {
            APPL_TRC("GA_SEP_RELEASE_NTF (0x%02X)\n", ga_event);
        }

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if ((GA_SUCCESS == ga_status) )
        {
            if (GA_SEP_RELEASE_CNF == ga_event)
            {
                /*
                 * If the Unicast Client initiates the Release operation for
                 * an ASE, the Unicast Client and Unicast Server should remove any
                 * internal audio data paths that have been set up for the ASE
                 * as defined in Section 5.6.6.1
                 */
                /*
                 * The Unicast Client shall terminate any CIS established for
                 * that ASE by following the Connected Isochronous Stream Terminate
                 * procedure defined in Volume 3, Part C, Section 9.3.15 in
                 * Core Spec, when the Unicast Client has determined that the ASE
                 * is in the Releasing state.
                 */

            }
            else
            {
                /*
                 * The Unicast Server shall terminate a CIS established by
                 * the Unicast Client for an ASE if the Unicast Server has
                 * autonomously initiated the Release operation for that ASE by
                 * following the Connected Isochronous Stream Terminate procedure
                 * defined in Volume 3, Part C, Section 9.3.15 in Core Spec.
                 */
                /*
                 * The Unicast Client and Unicast Server should remove any
                 * internal audio data paths that have been set up for the ASE that
                 * is in the Releasing state, as defined in Section 5.6.6.1,
                 * before terminating the CIS.
                 */
            }
            /* Remove ISO and CIS */
            appl_ga_ucc_remove_iso_cis(ascs_index, ase_index);
            GA_UCC_GET_LOCAL_ROLE_FOR_ASE
            (
                remote_ascs[ascs_index].ascs.ase[ase_index].role,
                local_role
            );
            if (GA_ROLE_SOURCE == local_role)
            {
                /* We are Source, Stop Transmitting Audio Data */
                appl_ga_ucc_stop_transmitting_audio
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                    remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                    remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id,
                    AUDIO_NUM_OF_BITS_PER_SAMPLE
                );
            }
            else
            {
                /* We are Sink, Stop Receiving Audio data */
                appl_ga_ucc_stop_receiving_audio
                (
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                    remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                    AUDIO_NUM_OF_BITS_PER_SAMPLE
                );
            }
        }
        break;

    case GA_SEP_IDLE_NTF:
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : GA_SEP_IDLE_NTF (0x%02X)\n", ga_event);

        if (GA_SUCCESS == ga_status)
        {
            /* Check if it is a valid ASE */
            if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
            {
                /* Update the ASE state */
                remote_ascs[ascs_index].ascs.ase[ase_index].state =
                    ase->data[1U];
            }
            else
            {
                APPL_ERR("Unknown ASE ID !\n");
            }
        }

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    default:
        break;
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_cb_handler\n");
    return GA_SUCCESS;
}

void appl_ga_ucc_init(void)
{
    UINT8  ascs_index;
    UINT8  ase_index;

    ascs_index = 0U;
    ase_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_init\n");

    /* Initialize all fields to 0U */
    for (ascs_index = 0U; ascs_index < MAX_REM_ASCS_INFO; ascs_index++)
    {
        GA_mem_set
        (
            &remote_ascs[ascs_index],
            0U,
            (sizeof(REMOTE_ASCS_INFO))
        );
        for (ase_index = 0U; ase_index < MAX_UC_ASE; ase_index++)
        {
            /*
             * Initialize the CIG, CIS, ISO, ASE state,
             * CIS ID and CIS conn_handle
             */
            remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id =
                INVALID_CIG_CIS_ID;
            remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cis_id =
                INVALID_CIG_CIS_ID;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                CIG_STATE_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                CIS_STATE_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle =
                INVALID_CIS_HANDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                AUDIO_ISO_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].state =
                GA_ASE_STATE_IDLE;
        }
        remote_ascs[ascs_index].conn_dev_index = INVALID_CONN_DEV_INDEX;
    }

    appl_ga_ucc_bap_create_cis_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_init\n");
    return;
}

GA_RESULT appl_ga_ucc_bap_remove_remote_ascs_for_dev(UINT8 conn_dev_index)
{
    GA_RESULT  retval;
    UINT8      ascs_index;
    UINT8      ase_index;

    /* Initialize */
    retval = GA_FAILURE;
    ascs_index = 0U;
    ase_index = 0U;

    /* Get the index for remote_pacs */
    if (GA_SUCCESS ==
            appl_ga_ucc_get_ascs_index_from_conn_dev_index
            (
                conn_dev_index,
                &ascs_index
            ))
    {
        /* Remove ASCS data */
        GA_mem_set
        (
            &remote_ascs[ascs_index],
            0U,
            (sizeof(REMOTE_ASCS_INFO))
        );
        for (ase_index = 0U; ase_index < MAX_UC_ASE; ase_index++)
        {
            /*
             * Initialize the CIG, CIS, ISO, ASE state,
             * CIS ID and CIS conn_handle
             */
            remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id =
                INVALID_CIG_CIS_ID;
            remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cis_id =
                INVALID_CIG_CIS_ID;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                CIG_STATE_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                CIS_STATE_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle =
                INVALID_CIS_HANDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                AUDIO_ISO_IDLE;
            remote_ascs[ascs_index].ascs.ase[ase_index].state =
                GA_ASE_STATE_IDLE;
        }
        remote_ascs[ascs_index].conn_dev_index = INVALID_CONN_DEV_INDEX;
        retval = GA_SUCCESS;
    }

    return retval;
}

void main_ucc_operations(void)
{
    GA_RESULT   retval;
    int         choice;
    UINT8       menu_choice;
    GA_CONTEXT  context;
    UINT8       conn_dev_index;

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    menu_choice = 0U;
    conn_dev_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> main_ucc_operations\n");

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", ucc_menu_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = (UINT8)choice;

        switch(menu_choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                /* Fetch GA context and Device_index. 2nd param Device can be
                 * NULL
                 */
                appl_ga_utils_get_conn_device_in_use
                (
                    &conn_dev_index,
                    NULL,
                    &context
                );

                /* Remove reference to old records */
                retval = appl_ga_ucc_bap_remove_remote_ascs_for_dev
                         (
                             conn_dev_index
                         );

                APPL_TRC("GA_sep_discover\n");
                retval = GA_sep_discover(&context);
                APPL_TRC("Retval - ");
                appl_ga_utils_display_ga_result(retval);
                break;

            case 3:
                appl_ga_ucc_ascs_config_codec();
                break;

            case 4:
                /*
                 * When configuring a CIS, the Unicast Client should
                 * (is recommended that) use QoS parameters that correspond to
                 * parameters exposed by the Unicast Server for an ASE in the
                 * Codec Configured state.
                 */
                appl_ga_ucc_hci_set_cig_params();
                break;

            case 5:
                /*
                 * The Unicast Client shall request QoS parameters that the
                 * Unicast Client's Bluetooth controller has accepted during
                 * CIS configuration.
                 */
                appl_ga_ucc_ascs_config_qos();
                break;

            case 6:
                appl_ga_ucc_ascs_enable();
                break;

            case 7:
                appl_ga_ucc_receiver_start_ready();
                break;

            case 8:
                appl_ga_ucc_update_metadata();
                break;

            case 9:
                appl_ga_ucc_ascs_disable();
                break;

            case 10:
                appl_ga_ucc_receiver_stop_ready();
                break;

            case 11:
                appl_ga_ucc_ascs_release();
                break;

            case 15:
                appl_ga_ucc_display_ase_data();
                break;

            case 16:
                appl_ga_ucc_ase_create_cis();
                break;

            case 17:
                appl_ga_ucc_ase_disconnect_cis();
                break;

            case 18:
                appl_ga_ucc_remove_cig();
                break;

            case 19:
                appl_ga_utils_display_all_conn_device();
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

    APPL_DBG("[APPL][GA][UCC]: << main_ucc_operations\n");
    return;
}

GA_RESULT appl_ga_ucc_get_common_params_for_asecp_operations
          (
              GA_CONTEXT  * context,
              UINT8       * conn_dev_index,
              GA_ENDPOINT * dev,
              UINT8       * ase_count,
              UINT8       * ascs_index
          )
{
    GA_RESULT  retval;

    retval = GA_SUCCESS;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_get_common_params_for_asecp_operations\n");

    /* Fetch conn_dev_index in use */
    appl_ga_utils_get_conn_device_in_use
    (
        conn_dev_index,
        dev,
        context
    );

    if (NULL != ase_count)
    {
        *ase_count = appl_ga_utils_bap_get_ase_count();
    }

    if (NULL != ascs_index)
    {
        /* Fetch ASCS index based on conn_dev_index */
        retval = appl_ga_ucc_get_ascs_index_from_conn_dev_index
                 (
                     *conn_dev_index,
                     ascs_index
                 );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_get_common_params_for_asecp_operations\n");
    return retval;
}

void appl_ga_ucc_ascs_config_codec(void)
{
    GA_RESULT                   retval;
    int                         choice;
    UINT8                       index;
    UINT8                       id;
    UINT8                       ascs_index;
    UINT8                       ase_index;
    /* Connected Device Index */
    UINT8                       conn_dev_index;
    /* CS Configuration for the ASE */
    GA_LC3_CODEC_SPECIFIC_CONF  cs_conf[MAX_UC_ASE] = { 0 };
    /* Context for this Device */
    GA_CONTEXT                  context;
    /* Information of ASEs for Configure Codec Op */
    UINT8                       ase_count;
    UINT8                       ase_id[MAX_UC_ASE] = { 0U };
    UINT8                       ase_role[MAX_UC_ASE] = { 0U };
    GA_CODEC_INFO               codec_info[MAX_UC_ASE] = { 0 };
    UINT8                       target_phy[MAX_UC_ASE] = { 0U };
    UINT8                       target_latency[MAX_UC_ASE] = { 0U };

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    id = 0U;
    context = 0U;
    index = 0U;
    ase_count = 0U;
    conn_dev_index = 0U;
    ascs_index = 0U;
    ase_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ascs_config_codec\n");

    /* Fetch context, conn_dev_index and ase_count */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    if (GA_SUCCESS == retval)
    {
        for (index = 0U; index < ase_count; index++)
        {
            ase_id[index] = appl_ga_utils_bap_get_ase_id();
            ase_role[index] = appl_ga_utils_bap_get_role();

            /* TODO: Remove later, Derive based on Set ID */
            target_phy[index] = 2U;
            target_latency[index] = 2U;

            /*
             * The Server would have sent preferred values during PACS
             * capability. When initiating the Config Codec operation
             * for an ASE the Unicast Client shall only submit codec
             * configuration parameters that are supported by the Unicast Server.
             * Display the PACS here, So that the user chooses one of
             * the Set supported by the remote PACS.
             */
            appl_ga_utils_bap_display_remote_pacs(conn_dev_index);

            /* Display CS Conf Set */
            /* Input the CS Conf Set ID */

            /* TMAP Role: UMS, Shall Select as per reqirements */
            id = appl_ga_utils_bap_get_cs_conf_id();

            /*
             * Prepare the CS Conf based on Set ID, Ignore the other
             * field.
             */
            if (GA_SUCCESS ==
                appl_ga_utils_bap_uc_prepare_csc_and_qos_cap_based_on_set_id
                (
                    id,
                    &cs_conf[index],
                    NULL,
                    NULL
                ))
            {
                /*
                 * If one of the Set is chosen as defined in BAP Spec,
                 * the fields are mandated as below as per Spec.
                 */
                appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
                (
                    &codec_info[index]
                );

                /* Prepare the Codec Spec Configuration LTVs for LC3 */
                appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
                (
                    &cs_conf[index],
                    codec_info[index].ie,
                    &codec_info[index].ie_len
                );

                /* TODO: Add Phy and Latency based on the Set ID */
            }
            else
            {
                /*
                 * Chosen a set not defined in BAP Spec, Shall be
                 * vendor specific
                 */
                appl_ga_utils_bap_get_codec_id_for_vendor_spec(&codec_info[index]);

                /* Fetch the Codec Spec Configuration LTVs for Vendor Specific */
                appl_ga_utils_bap_get_vendor_spec_codec_spec_conf_ltv
                (
                    codec_info[index].ie,
                    &codec_info[index].ie_len
                );

                /* TODO: Add Phy and Latency based on the Vendor Specific Conf */
            }
        }

        APPL_TRC("GA_sep_configure\n");
        retval = GA_sep_configure
                 (
                     &context,
                     ase_count,
                     ase_id,
                     ase_role,
                     codec_info,
                     target_phy,
                     target_latency
                 );
        APPL_TRC("Retval - ");
        appl_ga_utils_display_ga_result(retval);

        if (GA_SUCCESS == retval)
        {
            /* Save the codec specific configuration setting for ASE */
            for (index = 0U; index < ase_count; index++)
            {
                ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                            (
                                ascs_index,
                                ase_id[index]
                            );
                if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
                {
                    APPL_ERR("Unknown ASE ID !\n");
                }
                else
                {
                    GA_mem_copy
                    (
                        &remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf,
                        &cs_conf[index],
                        sizeof(GA_LC3_CODEC_SPECIFIC_CONF)
                    );
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ascs_config_codec\n");
    return;
}

void appl_ga_ucc_hci_set_cig_params(void)
{
    int          choice;
    GA_RESULT    retval;
    GA_QOS_CONF  qos_conf = { 0 };
    UINT8        cis_count;
    UINT8        qos_conf_set_id;
    UINT8        index;
    UINT8        ascs_index;
    UINT8        ase_index;

    /* Common Params for all CISes per CIG */
    UINT8   sca;
    UINT8   packing;

    /* CIS Specific Param */
    UINT8   conn_dev_index;
    UINT8   direction;
    UINT8   cis_id[MAX_UC_ASE] = { 0U };
    UINT16  max_sdu_m_to_s[MAX_UC_ASE] = { 0U };
    UINT16  max_sdu_s_to_m[MAX_UC_ASE] = { 0U };
    UINT8   phy_m_to_s[MAX_UC_ASE] = { 0U };
    UINT8   phy_s_to_m[MAX_UC_ASE] = { 0U };
    UINT8   retxn_m_to_s[MAX_UC_ASE] = { 0U };
    UINT8   retxn_s_to_m[MAX_UC_ASE] = { 0U };

    /* Associate CIS to the ASE */
    UINT8 peer_ase_id[MAX_UC_ASE] = { 0U };

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    cis_count = 0U;
    qos_conf_set_id = 0U;
    index = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    sca = 0U;
    packing = 0U;
    conn_dev_index = 0U;
    direction = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_hci_set_cig_params\n");

    APPL_TRC("Since SDU Interval, SCA, Packing, Framing, Max Transport "
        "latency are to be common for a CIG, Ensure the selection of "
        " Set ID results in common values for below\n");

    APPL_TRC("For simplicity, Configuring all CISes of the CIG with the same "
             "set\n");

    APPL_TRC("Call this separately if CIS of different Conf need to be "
              "configured\n");

    qos_conf.channel.cig_id = appl_ga_utils_bap_get_cig_id();

    cis_count = appl_ga_utils_bap_get_input_cis_count();

    /*
     * Since SDU Interval, SCA, Packing, Framing, Max Transport latency are
     * are to common for a CIG,
     * Ensure that the CISes have the same values for these parameters.
     */

    /* Fetch the SCA */
    sca = appl_ga_utils_bap_get_sca();

    /* Fetch the Packing */
    packing = appl_ga_utils_bap_get_packing();

    /*
     * The Unicast Client shall request a Presentation_Delay(see Section 7)
     * parameter value for an ASE that lies within the Unicast Server’s
     * supported range for that ASE, defined by Presentation_Delay_Min and
     * Presentation_Delay_Max and the Unicast Client should request a
     * Presentation_Delay parameter value that lies within the Unicast
     * Server’s preferred range for that ASE, defined by
     * Preferred_Presentation_Delay_Min and Preferred_Presentation_Delay_Max(see Table 4.3 in[4]).
     */

    /*
     * The Unicast Client shall not request Unframed ISOAL PDUs if the Unicast
     * Server has indicated no support for Unframed ISOAL PDUs, otherwise the
     * Unicast Client may request Unframed ISOAL PDUs.
     */
    /*
     * The Unicast Client shall request a Max_Transport_Latency parameter value
     * for an ASE that is no greater than the Max_Transport_Latency field value
     * exposed by the Unicast Server for that ASE.
     */

    /* For now, Ignore the Pref Values received during Codec Config State */

    /* Display the CS Conf Set ID which was set during Config Codec */
    APPL_TRC("Ensure that the Set ID input here is the same as used during "
        "Config Codec\n");

    /* Fetch the QoS Set ID */
    qos_conf_set_id = appl_ga_utils_bap_uc_get_qos_cs_cap_set_id();

    appl_ga_utils_bap_uc_prepare_csc_and_qos_cap_based_on_set_id
    (
        qos_conf_set_id,
        NULL,
        NULL,
        &qos_conf
    );

    for (index = 0U; index < cis_count; index++)
    {
        APPL_TRC("Enter the Device to which CIS needs to be created:\n");

        /*
         * Assuming that the User inputs the ASEs in the order of Device ID
         * Since, when the Event is received, the Event doesnt indicate the
         * CIS ID as a parameter for the CIS Connection Handle.
         */

        /*
         * The Unicast Client should use QoS parameters that correspond to
         * parameters exposed by the Unicast Server for an ASE in the Codec
         * Configured state.
         */

        conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();

        /* Fetch ASCS index based on conn_dev_index */
        retval = appl_ga_ucc_get_ascs_index_from_conn_dev_index
                 (
                     conn_dev_index,
                     &ascs_index
                 );

        if (GA_SUCCESS == retval)
        {
            cis_id[index] = appl_ga_utils_bap_get_cis_id();
            qos_conf.channel.cis_id = cis_id[index];

            /* Input if this needs to be a Unidirectional/Bidirectional */
            direction = appl_ga_utils_bap_get_data_flow_direction();

            if (ASE_UNIDIRECTIONAL == direction)
            {
                /* Associate single ASE with CIS and QoS Conf */
                peer_ase_id[index] = appl_ga_utils_bap_get_ase_id();

                /* Fetch ASE index based on ase_id */
                ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                            (
                                ascs_index,
                                peer_ase_id[index]
                            );

                /* Saving the QOS Conf set id for this ASE */
                remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id =
                    qos_conf_set_id;

                /*
                 * When tested for 2 CIS with max_sdu_m_to_s and max_sdu_s_to_m
                 * set to the same value, CIS establishment happens only for the
                 * first one and CIS Request Subevent will not be triggered for
                 * the second CIS on the Server.
                 * Issue is because max_sdu_s_to_m was set to max_sdu_m_to_s.
                 * For Local Source Role(Client), we need to set the value of
                 * the max_sdu_s_to_m to 0.
                 * For Local Source Role(Server), we need to set the value of
                 * the max_sdu_m_to_s to 0.
                 * However, the m_to_s/s_to_m param values shall be set to the
                 * same as s_to_m/m_to_s.
                 * Conclusion: m_to_s and s_to_m has to be set to 0.
                 * 1. Unidirectional 1 CIS(In a single Create CIS)
                 * 2. 2 CIS(In a single/multi Create CIS) the other.
                 */
                if (GA_ROLE_SINK ==
                    remote_ascs[ascs_index].ascs.ase[ase_index].role)
                {
                    max_sdu_m_to_s[index] = qos_conf.max_sdu;
                    max_sdu_s_to_m[index] = 0U;
                }
                else
                {
                    max_sdu_s_to_m[index] = qos_conf.max_sdu;
                    max_sdu_m_to_s[index] = 0U;
                }
                phy_m_to_s[index] = qos_conf.phy;
                phy_s_to_m[index] = qos_conf.phy;
                retxn_m_to_s[index] = qos_conf.retxn;
                retxn_s_to_m[index] = qos_conf.retxn;

                /* Check if it is a valid ASE */
                if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                        CIG_STATE_WAITING;
                    GA_mem_copy
                    (
                        &remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf,
                        &qos_conf,
                        sizeof(GA_QOS_CONF)
                    );
                }
            }
            else
            {
                /* Associate the ASE with this QoS Conf */
                peer_ase_id[index] = appl_ga_utils_bap_get_ase_id();

                /*
                 * Bidirectional CIS refers to ASE 1(Source) and ASE 2(Sink)
                 * using same CIG and CIS.
                 */

                /* Fetch ASE index based on ase_id */
                ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                            (
                                ascs_index,
                                peer_ase_id[index]
                            );

                /* Saving the QOS Conf set id for this ASE */
                remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id =
                    qos_conf_set_id;

                if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                        CIG_STATE_WAITING;
                    GA_mem_copy
                    (
                        &remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf,
                        &qos_conf,
                        sizeof(GA_QOS_CONF)
                    );
                }

                /* Associate the ASE with this QoS Conf */
                peer_ase_id[index] = appl_ga_utils_bap_get_ase_id();

                /* Fetch ASE index based on ase_id */
                ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                            (
                                ascs_index,
                                peer_ase_id[index]
                            );

                /* Saving the QOS Conf set id for this ASE */
                remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf_set_id =
                    qos_conf_set_id;

                if (ase_index < remote_ascs[ascs_index].ascs.ase_count)
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cig_state =
                        CIG_STATE_WAITING;
                    GA_mem_copy
                    (
                        &remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf,
                        &qos_conf,
                        sizeof(GA_QOS_CONF)
                    );
                }

                max_sdu_m_to_s[index] = qos_conf.max_sdu;
                max_sdu_s_to_m[index] = qos_conf.max_sdu;
                phy_m_to_s[index] = qos_conf.phy;
                phy_s_to_m[index] = qos_conf.phy;
                retxn_m_to_s[index] = qos_conf.retxn;
                retxn_s_to_m[index] = qos_conf.retxn;
            }
        }
    }

    APPL_TRC("Set CIG Params...\n");
    /* Refer 7.8.97 LE Set CIG Parameters command in Core Spec */
    retval = BT_hci_le_set_cig_parameters
             (
                 qos_conf.channel.cig_id,
                 qos_conf.sdu_interval,
                 qos_conf.sdu_interval,
                 sca,
                 packing,
                 qos_conf.framing,
                 qos_conf.max_transport_latency,
                 qos_conf.max_transport_latency,
                 cis_count,
                 cis_id,
                 max_sdu_m_to_s,
                 max_sdu_s_to_m,
                 phy_m_to_s,
                 phy_s_to_m,
                 retxn_m_to_s,
                 retxn_s_to_m
             );
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_hci_set_cig_params\n");
    return;
}

void appl_ga_ucc_ascs_config_qos(void)
{
    GA_RESULT    retval;
    /* Total ASE count */
    UINT8        ase_count;
    UINT8        index;
    UINT8        ascs_index;
    UINT8        ase_index;
    UINT8        ase_id_input;
    UINT8        ase_count_valid;
    UINT8        conn_dev_index;
    /* Required context for Config QOS */
    GA_CONTEXT   context;
    /* ASE Data */
    UINT8        ase_id[MAX_UC_ASE] = {0U};
    GA_QOS_CONF  qos_conf[MAX_UC_ASE] = { 0 };

    /* Initialize */
    retval = GA_FAILURE;
    ase_count = 0U;
    index = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    ase_id_input = 0U;
    ase_count_valid = 0U;
    conn_dev_index = 0U;
    context = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ascs_config_qos\n");

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            ase_id[ase_count_valid] = ase_id_input;
            ase_count_valid++;

            GA_mem_copy
            (
                &qos_conf[index],
                &remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf,
                sizeof(GA_QOS_CONF)
            );
        }
    }

    APPL_TRC("GA_sep_setup\n");
    retval = GA_sep_setup(&context, ase_count_valid, ase_id, qos_conf);
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ascs_config_qos\n");
    return;
}

void appl_ga_ucc_ascs_enable(void)
{
    GA_RESULT        retval;
    UINT8            index;
    UINT8            j;
    PACS_CAP_INFO    pacs = { 0 };
    UINT8            ascs_index;
    UINT8            ase_index;
    UINT8            ase_id_input;
    UINT8            ase_count_valid;
    UINT8            conn_dev_index;
    GA_ENDPOINT      device;
    UINT8            local_role;
    /* Required context for ASE Enable */
    GA_CONTEXT       context;
    /* Total ASE count */
    UINT8            ase_count;
    /* ASE Data */
    UINT8            ase_id[MAX_UC_ASE] = { 0U };
    GA_CONTENT_INFO  content_info[MAX_UC_ASE] = { 0U };
    UINT8            ccid[MAX_UC_ASE] = { 0U };

    /* Initialize */
    retval = GA_FAILURE;
    ase_count = 0U;
    index = 0U;
    j = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    ase_id_input = 0U;
    ase_count_valid = 0U;
    conn_dev_index = 0U;
    context = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ascs_enable\n");

    /* Fetch context, conn_dev_index, device, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 &device,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
            /* Skip this ASE ID */
        }
        else
        {
            /*
             * Validate if our Audio PL and LC3 Module allows this
             * Configuration.
             */
            GA_UCC_GET_LOCAL_ROLE_FOR_ASE
            (
                remote_ascs[ascs_index].ascs.ase[ase_index].role,
                local_role
            );
            /*
             * Validate below:
             * Src:
             * If LC3 is already configured and streaming,
             * Validate the Stream Configurations, Channel Count and
             * Audio Channel Allocation to be the same set or a subset of the
             * existing set.
             * If yes, Allow else do not.
             * Snk:
             * If LC3 is already configured and streaming,
             * Do not allow.
             */

            if ((GA_ROLE_SOURCE == local_role) &&
                GA_SUCCESS != appl_ga_utils_audio_src_audio_lc3_enc_allowed
                              (
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf
                              ))
            {
                /* Skip this ASE ID */
            }
            else if ((GA_ROLE_SINK == local_role) &&
                 GA_SUCCESS != appl_ga_utils_audio_snk_audio_lc3_dec_allowed
                               (
                                   remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf
                               ))
            {
                /* Skip this ASE ID */
            }
            else
            {
                ase_id[ase_count_valid] = ase_id_input;
                ase_count_valid++;

                /* Fetch Remote PACS from device */
                retval = appl_ga_utils_bap_fetch_remote_pacs_based_on_dev
                         (
                             device,
                             &pacs
                         );
                if (GA_SUCCESS == retval)
                {
                    /* Display PACS available and supported context */
                    appl_ga_utils_bap_display_avail_and_supp_pacs_ctx
                    (
                        remote_ascs[ascs_index].ascs.ase[ase_index].role,
                        pacs.record
                    );
                }
                /*
                    * Unicast Client may write any LTV structures for the Metadata
                    * parameter
                    */
                    /*
                    * While initiating the Enable operation for an ASE, the Unicast
                    * Client may include the Streaming_Audio_Contexts LTV structure
                    * in the Metadata parameter for that ASE
                    * Refer BAP Spec V_1.0 Page- 89
                    */
                    /* Fetch the streaming audio context value */
                content_info[index].contexts =
                    appl_ga_utils_bap_fetch_streaming_audio_ctx();

                content_info[index].num_ccid = appl_ga_utils_bap_get_ccid_count();

                for (j = 0U; j < content_info[index].num_ccid; j++)
                {
                    ccid[j] = appl_ga_utils_bap_get_ccid();
                }
                content_info[index].ccid = ccid;
            }
        }
    }

    APPL_TRC("GA_sep_start\n");
    retval = GA_sep_start(&context, ase_count_valid, ase_id, content_info);
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ascs_enable\n");
    return;
}

void appl_ga_ucc_receiver_start_ready(void)
{
    GA_RESULT   retval;
    UINT8       index;
    UINT8       conn_dev_index;
    UINT8       ascs_index;
    UINT8       ase_id_input;
    UINT8       ase_index;
    GA_CONTEXT  context;
    UINT8       ase_count;
    UINT8       ase_count_valid;
    UINT8       ase_id[MAX_UC_ASE] = { 0U };
    UINT8       cis_count;
    UINT16      acl_handle;
    UINT16      cis_handle[16U] = { 0U };

    /* Initialize */
    retval = 0U;
    index = 0U;
    conn_dev_index = 0U;
    ascs_index = 0U;
    ase_id_input = 0U;
    ase_index = 0U;
    context = 0U;
    ase_count = 0U;
    cis_count = 0U;
    acl_handle = 0U;
    ase_count_valid = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_receiver_start_ready\n");

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            ase_id[ase_count_valid] = ase_id_input;
            ase_count_valid++;
        }
        /*
         * With the role as sink, check if CIS is established. If not,
         * then establish CIS.
         */
        if (CIS_STATE_CONNECTED ==
            remote_ascs[ascs_index].ascs.ase[ase_id[index]].cig_cis_info.cis_state)
        {
            retval = appl_ga_utils_get_conn_handle_from_index
                     (
                         conn_dev_index,
                         &acl_handle
                     );

            cis_count++;
            cis_handle[index] =
                remote_ascs[ascs_index].ascs.ase[ase_id[index]].cig_cis_info.handle;
        }
    }

    APPL_TRC("GA_sep_receiver_start_ready\n");
    retval = GA_sep_receiver_start_ready
             (
                 &context,
                 ase_count_valid,
                 &ase_id[0U]
             );
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_receiver_start_ready\n");
    return;
}

void appl_ga_ucc_update_metadata(void)
{
    GA_RESULT        retval;
    UINT8            j;
    UINT8            conn_dev_index;
    GA_CONTEXT       context;
    UINT8            ase_count;
    UINT8            ascs_index;
    UINT8            index;
    UINT8            ase_id[MAX_UC_ASE] = { 0U };
    UINT8            ase_id_input;
    UINT8            ase_index;
    UINT8            ase_count_valid;
    GA_CONTENT_INFO  content_info[MAX_UC_ASE];
    UINT8            ccid[MAX_UC_ASE] = { 0U };
    /* To be used for taking metadata inputs */

    /* Initialize */
    j = 0U;
    conn_dev_index = 0U;
    context = 0U;
    ase_count = 0U;
    ascs_index = 0U;
    index = 0U;
    ase_id_input = 0U;
    ase_index = 0U;
    ase_count_valid = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_update_metadata\n");

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            ase_id[ase_count_valid] = ase_id_input;
            ase_count_valid++;
            /* TODO */
#if 0
            appl_ga_utils_bap_get_input_and_create_metadata_ltv
            (
                &meta.data[0U],
                &meta.length
            );
#endif /* 0 */
            /*
             * Unicast Client may write any LTV structures for the Metadata
             * parameter.
             */
            /*
             * While initiating the Update Metadata operation for an ASE, the
             * Unicast Client may include the Streaming_Audio_Contexts LTV
             * structure in the Metadata parameter for that ASE.
             * Refer BAP Spec V_1.0 Page- 95
             */
            /* Fetch the streaming audio context value */
            content_info[index].contexts =
                appl_ga_utils_bap_fetch_streaming_audio_ctx();

            content_info[index].num_ccid = appl_ga_utils_bap_get_ccid_count();

            for (j = 0U; j < content_info[index].num_ccid; j++)
            {
                ccid[j] = appl_ga_utils_bap_get_ccid();
            }
            content_info[index].ccid = ccid;
        }
    }

    APPL_TRC("GA_sep_update\n");
    retval = GA_sep_update
             (
                 &context,
                 ase_count_valid,
                 &ase_id[0U],
                 &content_info[0U]
             );
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_update_metadata\n");
    return;
}

void appl_ga_ucc_ascs_disable(void)
{
    GA_RESULT   retval;
    UINT8       index;
    UINT8       conn_dev_index;
    UINT8       ase_id_input;
    UINT8       ase_count_valid;
    UINT8       ascs_index;
    UINT8       ase_index;
    /* Required context for Receiver Start Ready */
    GA_CONTEXT  context;
    UINT8       ase_count;
    UINT8       ase_id[MAX_UC_ASE] = { 0 };

    /* Initialize */
    retval = GA_FAILURE;
    ase_count = 0U;
    index = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    ase_id_input = 0U;
    ase_count_valid = 0U;
    conn_dev_index = 0U;
    context = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ascs_disable\n");

    /*
     * Disable operation is valid for an ASE only if the value of the
     * ASE_State field is 0x03 (Enabling) or 0x04 (Streaming).
     */

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            ase_id[ase_count_valid] = ase_id_input;
            ase_count_valid++;
        }
    }

    APPL_TRC("GA_sep_suspend\n");
    retval = GA_sep_suspend(&context, ase_count_valid, ase_id);
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ascs_disable\n");
    return;
}

void appl_ga_ucc_receiver_stop_ready(void)
{
    GA_RESULT   retval;
    UINT8       index;
    UINT8       conn_dev_index;
    UINT8       ase_id_input;
    UINT8       ase_count_valid;
    UINT8       ascs_index;
    UINT8       ase_index;
    /* Required context for Receiver Start Ready */
    GA_CONTEXT  context;
    UINT8       ase_count;
    UINT8       ase_id[MAX_UC_ASE] = { 0 };

    /* Initialize */
    retval = GA_FAILURE;
    ase_count = 0U;
    index = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    ase_id_input = 0U;
    ase_count_valid = 0U;
    conn_dev_index = 0U;
    context = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_receiver_stop_ready\n");

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            ase_id[ase_count_valid] = ase_id_input;
            ase_count_valid++;
        }
    }

    APPL_TRC("GA_sep_receiver_stop_ready\n");
    retval = GA_sep_receiver_stop_ready(&context, ase_count_valid, ase_id);
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_receiver_stop_ready\n");
    return;
}

void appl_ga_ucc_ascs_release(void)
{
    GA_RESULT   retval;
    UINT8       index;
    UINT8       conn_dev_index;
    UINT8       ase_id_input;
    UINT8       ase_count_valid;
    UINT8       ascs_index;
    UINT8       ase_index;
    /* Required context for Release */
    GA_CONTEXT  context;
    UINT8       ase_count;
    UINT8       ase_index_valid[MAX_UC_ASE] = { 0 };
    UINT8       ase_id[MAX_UC_ASE] = { 0 };

    /* Initialize */
    retval = GA_FAILURE;
    ase_count = 0U;
    index = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    ase_id_input = 0U;
    ase_count_valid = 0U;
    conn_dev_index = 0U;
    context = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ascs_release\n");

    /*
     * Release operation is valid for an ASE only if the value of the ASE_State
     * field is 0x01 (Codec Configured), 0x02 (QoS Configured),
     * 0x03 (Enabling), 0x04 (Streaming), or 0x05 (Disabling).
     */

    /* Fetch context, conn_dev_index, ase_count and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 &context,
                 &conn_dev_index,
                 NULL,
                 &ase_count,
                 &ascs_index
             );

    for (index = 0U; index < ase_count; index++)
    {
        ase_id_input = appl_ga_utils_bap_get_ase_id();

        ase_index = appl_ga_ucc_get_ase_index_from_ase_id
                    (
                        ascs_index,
                        ase_id_input
                    );
        if (ase_index >= remote_ascs[ascs_index].ascs.ase_count)
        {
            APPL_ERR("Unknown ASE ID !\n");
        }
        else
        {
            /*
             * The Unicast Client should not initiate the Release operation for
             * an ASE that is in the Enabling state or the Streaming state if
             * the CIS_ID and CIG_ID parameters are identical to other ASEs
             * exposed by the Unicast Server that are in the Enabling state
             * or the Streaming state.
             * The Unicast Client should instead disable the ASE.
             */
            if (GA_FALSE == appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming
                            (
                                ascs_index,
                                ase_index
                            ))
            {
                ase_id[ase_count_valid] = ase_id_input;
                ase_index_valid[ase_count_valid] = ase_index;
                ase_count_valid++;
            }
            else
            {
                APPL_ERR("Release Operation is not allowed ! Disable is allowed !\n");
            }
        }
    }

    APPL_TRC("GA_sep_release\n");
    retval = GA_sep_release(&context, ase_count_valid, ase_id);
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    if (GA_SUCCESS == retval)
    {
        /* Release operation initiated by client */
        for (index = 0U; index < ase_count_valid; index++)
        {
            remote_ascs[ascs_index].ascs.ase[ase_index_valid[index]].is_release_initiated =
                GA_TRUE;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ascs_release\n");
    return;
}

void appl_ga_ucc_display_ase_data(void)
{
    GA_RESULT  retval;
    UINT8      conn_dev_index;
    UINT8      index;
    UINT8      ascs_index;

    conn_dev_index = 0U;
    index = 0U;
    ascs_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_display_ase_data\n");

    /* Fetch conn_dev_index and ascs_index */
    retval = appl_ga_ucc_get_common_params_for_asecp_operations
             (
                 NULL,
                 &conn_dev_index,
                 NULL,
                 NULL,
                 &ascs_index
             );

    /* Fetch ascs_index from conn_dev_index */
    if (GA_SUCCESS == retval)
    {
        for (index = 0U; index < remote_ascs[ascs_index].ascs.ase_count; index++)
        {
            APPL_TRC("ASE Data [%d]:\n", (index + 1U));
            /* Display ASE data */
            appl_ga_utils_bap_display_ase_data
            (
                conn_dev_index,
                remote_ascs[ascs_index].ascs.ase[index]
            );
        }
    }
    else
    {
        APPL_DBG
        (
            "[APPL][GA][UCC]: ASCS data for this device is not present !\n"
        );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_display_ase_data\n");
    return;
}

void appl_ga_ucc_ase_create_cis(void)
{
    /* No of CIS given by user as input */
    UINT8      num_of_cis;
    UINT8      conn_dev_index[MAX_UCS_SIMULTANEOUS_ASE] = { 0 };
    UINT8      ase_id[MAX_UC_ASE] = { 0 };
    UINT16     acl_handle_array[16U] = { INVALID_CIS_HANDLE };
    UINT16     cis_handle_array[16U] = { INVALID_CIS_HANDLE };
    /* Total CIS count which are currently in Waiting State */
    UINT8      cis_count;
    GA_RESULT  retval;

    /* Initialize */
    num_of_cis = 0U;
    cis_count = 0U;
    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ase_create_cis\n");

    /* Fetch the params for create CIS */
    appl_ga_ucc_bap_get_params_for_create_cis
    (
        &num_of_cis,
        conn_dev_index,
        ase_id
    );

    /* Update CIS State */
    appl_ga_ucc_bap_update_cis_state
    (
        num_of_cis,
        conn_dev_index,
        ase_id
    );

    /* Prepare the CIS and ACL Handle array */
    retval = appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis
             (
                 &cis_count,
                 acl_handle_array,
                 cis_handle_array
             );

    /* Trigger CIS Connection */
    if ((GA_SUCCESS == retval) && (0U != cis_count))
    {
        (GA_IGNORE_RETURN_VALUE)appl_ga_ucc_bap_create_cis
                                (
                                    cis_count,
                                    cis_handle_array,
                                    acl_handle_array
                                );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ase_create_cis\n");
    return;
}

void appl_ga_ucc_ase_disconnect_cis(void)
{
    UINT8   ase_id;
    UINT8   ascs_index;
    UINT8   ase_index;
    UINT8   conn_dev_index;
    UINT16  cis_handle;
    UCHAR   reason;
    int     input;

    /* Initialize */
    ase_id = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    conn_dev_index = 0U;
    cis_handle = INVALID_CIS_HANDLE;
    reason = 0U;
    input = 0;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_ase_disconnect_cis\n");

    /* Get conn_dev_index and ascs_index */
    if (GA_SUCCESS == appl_ga_ucc_get_common_params_for_asecp_operations
                      (
                          NULL,
                          &conn_dev_index,
                          NULL,
                          NULL,
                          &ascs_index
                      ))
    {
        ase_id = appl_ga_utils_bap_get_ase_id();

        /* Fetch ase_index from ase_id and ascs_index */
        ase_index = appl_ga_ucc_get_ase_index_from_ase_id(ascs_index, ase_id);
        /* Check if disconnection is allowed or not */
        if (GA_SUCCESS == appl_ga_ucc_is_cis_disconnect_allowed
                          (
                              ascs_index,
                              ase_index
                          ))
        {
            /* Check if CIS state is valid */
            if (CIS_STATE_CONNECTED ==
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
            {
                /* Update cis_handle to be disconnected */
                cis_handle =
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle;
            }

            if (INVALID_CIS_HANDLE != cis_handle)
            {
                /* Disconnect CIS */
                CONSOLE_OUT("Enter reason for disconnection: ");
                CONSOLE_IN("%d", &input);
                reason = (UCHAR)input;
                if (GA_SUCCESS == appl_ga_utils_disconnect
                                  (
                                      cis_handle,
                                      reason
                                  ))
                {
                    APPL_TRC("CIS Disconnection successful\n");
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_ase_disconnect_cis\n");
    return;
}

void appl_ga_ucc_remove_cig(void)
{
    UINT8  cig_id;

    /* Initialize */
    cig_id = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_remove_cig\n");

    /*
     * This command shall also remove the isochronous data paths that are
     * associated with the Connection_Handles of the CISes, which is equivalent
     * to issuing the HCI_LE_Remove_ISO_Data_Path command.
     */

    /* Take CIG_ID as input */
    cig_id = appl_ga_utils_bap_get_cig_id();

    /* Remove CIG */
    if (GA_SUCCESS == BT_hci_le_remove_cig(cig_id))
    {
        APPL_TRC("CIG removed successfully\n");
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_remove_cig\n");
    return;
}

UINT8 appl_ga_ucc_get_ase_index_from_ase_id
      (
          UINT8 ascs_index,
          UINT8 ase_id
      )
{
    UINT8  index;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_get_ase_index_from_ase_id\n");

    for (index = 0U; index < remote_ascs[ascs_index].ascs.ase_count; index++)
    {
        if (ase_id == remote_ascs[ascs_index].ascs.ase[index].id)
        {
            break;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_get_ase_index_from_ase_id\n");

    return index;
}

GA_RESULT appl_ga_ucc_get_ascs_index_from_conn_dev_index
          (
              UINT8   conn_dev_index,
              UINT8 * ascs_index
          )
{
    GA_RESULT  retval;
    UINT8      index;

    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_get_ascs_index_from_conn_dev_index\n");

    /* Based on the conn_dev_index, Fetch the Index */
    for (index = 0U; index < MAX_REM_ASCS_INFO; index++)
    {
        if (conn_dev_index == remote_ascs[index].conn_dev_index)
        {
            *ascs_index = index;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_get_ascs_index_from_conn_dev_index\n");

    return retval;
}

GA_RESULT appl_ga_fetch_ascs_index_from_acl_conn_handle
          (
              UINT16   acl_conn_handle,
              UINT8  * conn_dev_index,
              UINT8  * ascs_index
          )
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_fetch_ascs_index_from_acl_conn_handle\n");

    /* Fetch conn_dev_index from acl_conn_handle */
    if (GA_SUCCESS == appl_ga_utils_get_dev_index_from_handle
                      (
                          acl_conn_handle,
                          conn_dev_index
                      ))
    {
        /* Fetch ascs_index from conn_dev_index */
        retval = appl_ga_ucc_get_ascs_index_from_conn_dev_index
                 (
                     *conn_dev_index,
                     ascs_index
                 );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_fetch_ascs_index_from_acl_conn_handle\n");
    return retval;
}

GA_RESULT appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
          (
              UINT16   cis_conn_handle,
              UINT8  * ascs_index,
              UINT8  * ase_count,
              UINT8  * ase_index
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      i;

    retval = GA_FAILURE;
    index = 0U;
    i = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle\n");

    if ((NULL == ase_index) || (NULL == ase_count))
    {
        return GA_FAILURE;
    }

    for (i = 0U; i < MAX_REM_ASCS_INFO; i++)
    {
        for (index = 0U; index < remote_ascs[i].ascs.ase_count; index++)
        {
            if (cis_conn_handle ==
                remote_ascs[i].ascs.ase[index].cig_cis_info.handle)
            {
                ase_index[*ase_count] = index;
                *ase_count += 1U;
                retval = GA_SUCCESS;
            }
        }
        if (GA_SUCCESS == retval)
        {
            *ascs_index = i;
            break;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle\n");

    return retval;
}

GA_RESULT appl_ga_ucc_fetch_free_ascs_db_index(UINT8 * ascs_index)
{
    GA_RESULT  retval;
    UINT8      index;

    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_fetch_free_ascs_db_index\n");

    for (index = 0U; index < MAX_REM_ASCS_INFO; index++)
    {
        if (INVALID_CONN_DEV_INDEX == remote_ascs[index].conn_dev_index)
        {
            *ascs_index = index;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_fetch_free_ascs_db_index\n");
    return retval;
}

void appl_ga_ucc_setup_cis_iso
     (
         UINT8       ascs_index,
         UINT8       ase_index,
         GA_ENDPOINT device,
         UINT8       ase_id
     )
{
    UINT8  conn_dev_index;

    conn_dev_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_setup_cis_iso\n");

    if (GA_SUCCESS == appl_ga_ucc_is_cis_connect_allowed(ascs_index, ase_index))
    {
        /* Fetch conn_dev_index from device */
        appl_ga_utils_get_conn_device_index(device, &conn_dev_index);
        /* Update CIS State and Trigger CIS Connection */
        /*
         * As we will receive GA_SEP_START_CNF event for each ASE_ID
         * separately. Hence CIS count will be 1.
         * If we get different ASE_ID for same device with same CIS connection
         * handle, then triggering CIS connection for that ASE won't be
         * required.
         */
        appl_ga_ucc_bap_update_cis_state
        (
            1U,
            &conn_dev_index,
            &ase_id
        );
    }
    else
    {
        if (CIS_STATE_CONNECTED == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
        {
            /* CIS is already connected, Setup ISO */
            appl_ga_ucc_setup_iso(ascs_index, ase_index);
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_setup_cis_iso\n");
    return;
}

void appl_ga_ucc_remove_iso_cis(UINT8 ascs_index, UINT8 ase_index)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_remove_iso_cis\n");

    /* Remove ISO */
    if (GA_SUCCESS != appl_ga_ucc_remove_iso(ascs_index, ase_index))
    {
        /*
         * ISO Data path removal is not allowed.
         * Check if termonate CIS is allowed or not
         */
        if (GA_SUCCESS == appl_ga_ucc_is_cis_disconnect_allowed
                          (
                              ascs_index,
                              ase_index
                          ))
        {
            /* Disconnect CIS */
            if (GA_SUCCESS == appl_ga_utils_disconnect
                              (
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                                  0x13U
                              ))
            {
                APPL_TRC("CIS Disconnection successful\n");
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_remove_iso_cis\n");
    return;
}

GA_RESULT appl_ga_ucc_is_cis_connect_allowed(UINT8 ascs_index, UINT8 ase_index)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_is_cis_connect_allowed\n");

    if (((GA_ASE_STATE_QOS_CONF == remote_ascs[ascs_index].ascs.ase[ase_index].state)
        ||
        (GA_ASE_STATE_ENABLING == remote_ascs[ascs_index].ascs.ase[ase_index].state))
        &&
        (CIS_STATE_INITIALIZED == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state))
    {
        return GA_SUCCESS;
    }

    APPL_DBG("CIS Connect Not Allowed, ASE State: (0x%02X) CIS State: (0x%02X) !",
        remote_ascs[ascs_index].ascs.ase[ase_index].state,
        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_is_cis_connect_allowed\n");
    return GA_FAILURE;
}

GA_RESULT appl_ga_ucc_is_cis_disconnect_allowed(UINT8 ascs_index, UINT8 ase_index)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_is_cis_disconnect_allowed\n");
    /*
     * Sequence: Client triggered Release
     * 1. Client triggers Release, ASE State is Releasing.
     * 2. Remove ISO Data Path.
     * 3. Receive the Remove ISO Data Path Complete Event.
     * 4. Trigger CIS Disconnect.
     * 5. Receive CIS Disconnect Complete Event.
     * 6. Receive Release Complete.
     *
     * Sequence: Server triggered Release
     * 1. Server triggers Release, ASE State is Releasing.
     * 2. Remove ISO Data Path.
     * 3. Receive the Remove ISO Data Path Complete Event.
     * 4. Receive CIS Disconnect Complete Event.
     * 6. Receive Release Complete.
     */
    if ((GA_TRUE == remote_ascs[ascs_index].ascs.ase[ase_index].is_release_initiated)
        &&
        (GA_ASE_STATE_RELEASING == remote_ascs[ascs_index].ascs.ase[ase_index].state)
        &&
        (CIS_STATE_CONNECTED == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state))
    {
        /*
         * Validate if there is no other ASE using the same CIS - Bidirectional
         * which is Enabling/Streaming state.
         */
        if (GA_FALSE == appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming(ascs_index, ase_index))
        {
            return GA_SUCCESS;
        }
    }

    APPL_DBG("CIS Disconnect Not Allowed, ASE State: (0x%02X) CIS State: (0x%02X) !",
        remote_ascs[ascs_index].ascs.ase[ase_index].state,
        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_is_cis_disconnect_allowed\n");
    return GA_FAILURE;
}

UINT8 appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming
      (
          UINT8 ascs_index,
          UINT8 ase_index
      )
{
    UINT8 index;

    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming\n");

    for (index = 0U; index < MAX_UC_ASE; index++)
    {
        if (((GA_ASE_STATE_ENABLING ==
            remote_ascs[ascs_index].ascs.ase[index].state)
            ||
            (GA_ASE_STATE_STREAMING ==
                remote_ascs[ascs_index].ascs.ase[index].state))
            &&
            (CIS_STATE_CONNECTED ==
                remote_ascs[ascs_index].ascs.ase[index].cig_cis_info.cis_state)
            &&
            (remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id ==
                remote_ascs[ascs_index].ascs.ase[index].qos_conf.channel.cig_id)
            &&
            (remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cis_id ==
                remote_ascs[ascs_index].ascs.ase[index].qos_conf.channel.cis_id)
            &&
            (ase_index != index))
        {
            /*
             * For the given device, CIG and CIS is shared by more than 1 ASE,
             * Also in Enabling/Streaming state.
             */
            APPL_DBG("ASE_Index 0x%02X and 0x%02X: CIG and CIS(0x%02X) is "
                     "shared and is in Enabling/Streaming \n",
                      remote_ascs[ascs_index].ascs.ase[ase_index].id,
                      remote_ascs[ascs_index].ascs.ase[index].id,
                      remote_ascs[ascs_index].ascs.ase[index].qos_conf.channel.cis_id
                     );
            return GA_TRUE;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_is_cig_cis_shared_and_enabling_streaming\n");
    return GA_FALSE;
}

UINT8 appl_ga_ucc_get_cig_cis_shared_ase_index
      (
          UINT8   ascs_index,
          UINT8   ase_index,
          UINT8 * ase_index_using_same_cis
      )
{
    UINT8 index;

    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_get_cig_cis_shared_ase_index\n");

    for (index = 0U; index < MAX_UC_ASE; index++)
    {
        /* Skip the ase_index given and compae with the other ASEs */
        if ((ase_index != index)
            &&
            (remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cig_id ==
                remote_ascs[ascs_index].ascs.ase[index].qos_conf.channel.cig_id)
            &&
            (remote_ascs[ascs_index].ascs.ase[ase_index].qos_conf.channel.cis_id ==
                remote_ascs[ascs_index].ascs.ase[index].qos_conf.channel.cis_id))
        {
            /*
             * For the given device, CIG and CIS is shared by more than 1 ASE.
             * Bidirectional CIS.
             */
            *ase_index_using_same_cis = index;
            return GA_TRUE;
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_get_cig_cis_shared_ase_index\n");
    return GA_FALSE;
}

void appl_ga_ucc_bap_get_params_for_create_cis
     (
         /* OUT */ UINT8 * cis_count,
         /* OUT */ UINT8 * conn_dev_index,
         /* OUT */ UINT8 * ase_id
     )
{
    UINT8  i;

    i = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_bap_get_params_for_create_cis\n");

    *cis_count = appl_ga_utils_bap_get_input_cis_count();

    for (i = 0U; i < *cis_count; i++)
    {
        conn_dev_index[i] = appl_ga_utils_bap_get_input_conn_dev_index();
        ase_id[i] = appl_ga_utils_bap_get_ase_id();
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_bap_get_params_for_create_cis\n");
    return;
}

void appl_ga_ucc_bap_update_cis_state
     (
         UINT8   cis_count,
         UINT8 * conn_dev_index,
         UINT8 * ase_id
     )
{
    UINT8   i;
    UINT8   ascs_index;
    UINT8   ase_index;
    UINT16  cis_handle;

    /* Initialize */
    i = 0U;
    ascs_index = 0U;
    ase_index = 0U;
    cis_handle = INVALID_CIS_HANDLE;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_bap_update_cis_state\n");

    APPL_TRC("Creating CIS\n");
    for (i = 0U; i < cis_count; i++)
    {
        for (ascs_index = 0U; ascs_index < MAX_REM_ASCS_INFO; ascs_index++)
        {
            for (ase_index = 0U; ase_index < remote_ascs[ascs_index].ascs.ase_count; ase_index++)
            {
                if ((conn_dev_index[i] == remote_ascs[ascs_index].conn_dev_index) &&
                    (ase_id[i] == remote_ascs[ascs_index].ascs.ase[ase_index].id) &&
                    (CIS_STATE_INITIALIZED == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state))
                {
                    cis_handle =
                        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle;
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                        CIS_STATE_WAITING;
                    break;
                }
            }
            if (INVALID_CIS_HANDLE != cis_handle)
            {
                break;
            }
        }

        if ((MAX_REM_ASCS_INFO == ascs_index) ||
            (INVALID_CIS_HANDLE == cis_handle))
        {
            APPL_ERR
            (
                "ASE_ID 0x%02X is not present for this device or CIS "
                "connect not allowed  !\n",
                ase_id[i]
            );
        }
        else
        {
            /* Update the other ASEs CIS State */
            for (ase_index = 0U; ase_index < remote_ascs[ascs_index].ascs.ase_count; ase_index++)
            {
                if ((cis_handle == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle)
                && (CIS_STATE_INITIALIZED == remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state))
                {
                    remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                        CIS_STATE_WAITING;
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_bap_update_cis_state\n");

    return;
}

GA_RESULT appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis
          (
              UINT8  * cis_count,
              UINT16 * acl_handle_array,
              UINT16 * cis_handle_array
          )
{
    GA_RESULT  retval;
    UINT8      ascs_index;
    UINT8      ase_index;
    UINT16     acl_handle;
    UINT8      cis_handle_index;

    /* Initialize */
    retval = GA_FAILURE;
    ascs_index = 0U;
    ase_index = 0U;
    acl_handle = INVALID_CIS_HANDLE;
    cis_handle_index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis\n");

    if ((NULL == acl_handle_array) || (NULL == cis_handle_array))
    {
        return GA_FAILURE;
    }

    for (ascs_index = 0U; ascs_index < MAX_REM_ASCS_INFO; ascs_index++)
    {
        /* Get the ACL handle */
        retval = appl_ga_utils_get_conn_handle_from_index
                 (
                     remote_ascs[ascs_index].conn_dev_index,
                     &acl_handle
                 );

        if (GA_SUCCESS != retval)
        {
            APPL_TRC("Invalid Device Index !\n");
        }
        else
        {
            for (ase_index = 0U; ase_index < remote_ascs[ascs_index].ascs.ase_count; ase_index++)
            {
                if (CIS_STATE_WAITING ==
                        remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
                {
                    /*
                     * Multiple ASEs can have same CIS Handle. So check for
                     * same CIS Handle before adding into the array
                     */
                    for (cis_handle_index = 0U; cis_handle_index < *cis_count; cis_handle_index++)
                    {
                        /* This is checked to avoid multiple entries of the same CIS */
                        if (remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle ==
                                cis_handle_array[cis_handle_index])
                        {
                            break;
                        }
                    }
                    if (*cis_count == cis_handle_index)
                    {
                        /* It means CIS Handle is not yet added. Add it */
                        cis_handle_array[*cis_count] =
                            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle;
                        acl_handle_array[*cis_count] = acl_handle;
                        *cis_count = *cis_count + 1U;
                    }
                }
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis\n");

    return GA_SUCCESS;
}

GA_RESULT appl_ga_ucc_bap_create_cis
          (
              UINT8    cis_count,
              UINT16 * cis_handle,
              UINT16 * acl_handle
          )
{
    GA_RESULT retval;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_bap_create_cis\n");

    retval = BT_hci_le_create_cis
             (
                 cis_count,
                 &cis_handle[0U],
                 &acl_handle[0U]
             );

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_bap_create_cis\n");
    return retval;
}

void appl_ga_ucc_bap_create_cis_timer_handler
     (
         void  * data,
         UINT16  datalen
     )
{
    UINT16     acl_handle_array[16U] = { INVALID_CIS_HANDLE };
    UINT16     cis_handle_array[16U] = { INVALID_CIS_HANDLE };
    UINT8      cis_count;
    GA_RESULT  retval;

    /* Initialize */
    cis_count = 0U;
    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(data);
    GA_IGNORE_UNUSED_PARAM(datalen);

    retval = appl_ga_ucc_bap_prepare_cis_and_acl_handle_for_create_cis
             (
                 &cis_count,
                 acl_handle_array,
                 cis_handle_array
             );

    if ((GA_SUCCESS == retval) && (0U != cis_count))
    {
        (GA_IGNORE_RETURN_VALUE)appl_ga_ucc_bap_create_cis
                                (
                                    cis_count,
                                    cis_handle_array,
                                    acl_handle_array
                                );
    }

    /* Reset the timer handle to NULL */
    appl_ga_ucc_bap_create_cis_timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    return;
}

void appl_ga_ucc_update_cis_conn_state
     (
         UINT8   ascs_index,
         UINT8   ase_index,
         UINT8   conn,
         UINT16  cis_conn_status
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_update_cis_conn_state\n");

    if (conn)
    {
        if (GA_SUCCESS == cis_conn_status)
        {
            if (CIS_STATE_WAITING ==
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
            {
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                    CIS_STATE_CONNECTED;
            }
        }
        else
        {
            if (CIS_STATE_WAITING ==
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
            {
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                    CIS_STATE_INITIALIZED;
            }
        }
    }
    else
    {
        if (GA_SUCCESS == cis_conn_status)
        {
            if (CIS_STATE_CONNECTED ==
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
            {
                remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state =
                    CIS_STATE_INITIALIZED;
            }
        }
        else
        {
            /* This case will never hit */
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_update_cis_conn_state\n");
    return;
}

void appl_ga_ucc_terminate_cis(UINT16 cis_conn_handle)
{
    UINT8  ascs_index;
    UINT8  ase_index[MAX_UC_ASE] = { 0U };
    UINT8  ase_count;
    UINT8  index;

    /* Initialize */
    ascs_index = 0U;
    ase_count = 0U;
    index = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_terminate_cis\n");

    /*
     * Fetch the ASCS Index and ASE Index List based on
     * the CIS Conn Handle
     */
    if (GA_SUCCESS == appl_ga_ucc_get_ascs_index_ase_index_from_cis_conn_handle
                      (
                          cis_conn_handle,
                          &ascs_index,
                          &ase_count,
                          ase_index
                      ))
    {
        for (index = 0U; index < ase_count; index++)
        {
            if (GA_SUCCESS == appl_ga_ucc_is_cis_disconnect_allowed
                              (
                                  ascs_index,
                                  ase_index[index]
                              ))
            {
                /* Disconnect CIS */
                if (GA_SUCCESS == appl_ga_utils_disconnect
                                  (
                                      cis_conn_handle,
                                      0x13U
                                  ))
                {
                    APPL_TRC("CIS Disconnection successful\n");
                }
                break;
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_terminate_cis\n");
    return;
}

void appl_ga_ucc_setup_iso(UINT8 ascs_index, UINT8 ase_index)
{
    GA_RESULT  retval;
    UINT8      local_role;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_setup_iso\n");

    if ((GA_ASE_STATE_ENABLING ==
        remote_ascs[ascs_index].ascs.ase[ase_index].state)
        &&
        (CIS_STATE_CONNECTED ==
            remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.cis_state)
        &&
        (AUDIO_ISO_IDLE ==
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state))
    {
        GA_UCC_GET_LOCAL_ROLE_FOR_ASE
        (
            remote_ascs[ascs_index].ascs.ase[ase_index].role,
            local_role
        );
        if (GA_ROLE_SOURCE == local_role)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_src_set_entry
                              (
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf
                              ))
            {
                retval = appl_ga_utils_audio_src_le_setup_iso_data_path
                         (
                             remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle
                         );

                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Already Complete\n", UC_GET_ROLE_STRING(local_role));
                    /* ISO Data Path is already set - If multiple ASEs has same CIS */
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                        AUDIO_ISO_SETUP_COMPLETE;
                }
                else if (GA_CONTINUE == retval)
                {
                    APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: In-progress\n", UC_GET_ROLE_STRING(local_role));
                    /* ISO Data Path is Ongoing */
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                        AUDIO_ISO_UPDATE_IN_PROGRESS;
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Failed !\n", UC_GET_ROLE_STRING(local_role));
                }
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Set entry failed !\n", UC_GET_ROLE_STRING(local_role));
            }
        }
        else
        {
            if (GA_SUCCESS == appl_ga_utils_audio_snk_set_entry
                              (
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle,
                                  remote_ascs[ascs_index].ascs.ase[ase_index].cs_conf
                              ))
            {
                retval = appl_ga_utils_audio_snk_le_setup_iso_data_path
                         (
                             remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle
                         );

                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Already Complete\n", UC_GET_ROLE_STRING(local_role));
                    /* ISO Data Path is already set - If multiple ASEs has same CIS */
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                        AUDIO_ISO_SETUP_COMPLETE;
                }
                else if (GA_CONTINUE == retval)
                {
                    APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: In-progress\n", UC_GET_ROLE_STRING(local_role));
                    /* ISO Data Path is Ongoing */
                    remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                        AUDIO_ISO_UPDATE_IN_PROGRESS;
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Failed !\n", UC_GET_ROLE_STRING(local_role));
                }
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Setup Status: Set entry failed !\n", UC_GET_ROLE_STRING(local_role));
            }
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_setup_iso\n");
    return;
}

GA_RESULT appl_ga_ucc_remove_iso(UINT8 ascs_index, UINT8 ase_index)
{
    GA_RESULT  retval;
    UINT8      local_role;

    retval = GA_FAILURE;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_ucc_remove_iso\n");

    /* Check for ASE State and ISO State */
    if ((GA_ASE_STATE_RELEASING ==
        remote_ascs[ascs_index].ascs.ase[ase_index].state)
        &&
        (AUDIO_ISO_SETUP_COMPLETE ==
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state))
    {
        /* Get local role */
        GA_UCC_GET_LOCAL_ROLE_FOR_ASE
        (
            remote_ascs[ascs_index].ascs.ase[ase_index].role,
            local_role
        );
        if (GA_ROLE_SOURCE == local_role)
        {
            /* Remove ISO Data Data Path */
            retval = appl_ga_utils_audio_src_le_remove_iso_data_path
                     (
                         remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle
                     );
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Complete\n", UC_GET_ROLE_STRING(local_role));
            }
            else if (GA_CONTINUE == retval)
            {
                APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: In-progress\n", UC_GET_ROLE_STRING(local_role));
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Failed !\n", UC_GET_ROLE_STRING(local_role));
            }
        }
        else
        {
            /* Remove ISO Data Data Path */
            retval = appl_ga_utils_audio_snk_le_remove_iso_data_path
                     (
                         remote_ascs[ascs_index].ascs.ase[ase_index].cig_cis_info.handle
                     );
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Complete\n", UC_GET_ROLE_STRING(local_role));
            }
            else if (GA_CONTINUE == retval)
            {
                APPL_TRC("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: In-progress\n", UC_GET_ROLE_STRING(local_role));
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][%s]: ISO Data Path Removal Status: Failed !\n", UC_GET_ROLE_STRING(local_role));
            }
        }

        if (GA_SUCCESS == retval)
        {
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state = AUDIO_ISO_IDLE;
        }
        else if (GA_CONTINUE == retval)
        {
            remote_ascs[ascs_index].ascs.ase[ase_index].iso_state =
                AUDIO_ISO_UPDATE_IN_PROGRESS;
        }
        else
        {
            /* State mismatch */
        }
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_ucc_remove_iso\n");
    return retval;
}

void appl_ga_ucc_update_transmitting_audio
     (
         UINT16 cis_handle,
         GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
         UINT8 qos_conf_set_id,
         UINT8 bps,
         UINT8 status
     )
{
#ifdef AUDIO_SRC_ISO_SUPPORT
    if (GA_SUCCESS == appl_ga_utils_audio_src_update_entry
                      (
                          cis_handle,
                          AUDIO_IGNORE_VALUE,
                          AUDIO_IGNORE_VALUE,
                          status
                      ))
    {
#ifdef AUDIO_SRC_LC3_SUPPORT
        if (GA_TRUE == status)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_src_create_lc3_encoder
                              (
                                  cs_conf,
                                  qos_conf_set_id,
                                  bps
                              ))
            {
                APPL_TRC("[APPL][GA][UCC][SRC]: LC3 Encoder Setup Status: Created\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SRC]: LC3 Encoder Setup Status: Could not create !\n");
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_src_is_lc3_encoder_running())
            {
                if (GA_SUCCESS == appl_ga_utils_audio_src_delete_lc3_encoder())
                {
                    APPL_TRC("[APPL][GA][UCC][SRC]: LC3 Encoder Setup Status: Deleted\n");
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][SRC]: LC3 Encoder Setup Status: Could not delete !\n");
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][UCC][SRC]: LC3 Encoder Setup Status: Already Deleted\n");
            }
        }
#else /* AUDIO_SRC_LC3_SUPPORT */
        APPL_ERR("AUDIO_SRC_LC3_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_LC3_SUPPORT */
#ifdef AUDIO_SRC_PL_SUPPORT
        if (GA_TRUE == status)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_src_setup_generator_pl
                              (
                                  cs_conf,
                                  bps,
                                  AUDIO_SRC_PL_BUFFER_SIZE
                              ))
            {
                APPL_TRC("[APPL][GA][UCC][SRC]: Audio PL Generator Setup Status: Success\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SRC]: Audio PL Generator Setup Status: Failed !\n");
            }
            if (GA_SUCCESS == appl_ga_utils_audio_src_start_generator_pl())
            {
                APPL_TRC("[APPL][GA][UCC][SRC]: Audio PL Generator Start Process Status: Success\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SRC]: Audio PL Generator Start Process Status: Failed !\n");
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_src_is_generator_pl_running())
            {
                if (GA_SUCCESS == appl_ga_utils_audio_src_stop_generator_pl())
                {
                    APPL_TRC("[APPL][GA][UCC][SRC]: Audio PL Generator Stop Process Status: Success\n");
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][SRC]: Audio PL Generator Stop Process Status: Failed !\n");
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][UCC][SRC]: Audio PL Generator Status: Already Stopped\n");
            }
        }
#else /* AUDIO_SRC_PL_SUPPORT */
        APPL_ERR("AUDIO_SRC_PL_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_PL_SUPPORT */
    }
    else
    {
        APPL_ERR("[APPL][GA][UCC][SRC]: ISO Data Path Setup Status: Update entry failed !\n");
    }
#else /* AUDIO_SRC_ISO_SUPPORT */
APPL_ERR("AUDIO_SRC_ISO_SUPPORT is not enabled !\n");
#endif /* AUDIO_SRC_ISO_SUPPORT */

    return;
}

void appl_ga_ucc_update_receiving_audio
     (
         UINT16 cis_handle,
         GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
         UINT8 bps,
         UINT8 status
     )
{
#ifdef AUDIO_SNK_ISO_SUPPORT
    if (GA_SUCCESS == appl_ga_utils_audio_snk_update_entry
                      (
                          cis_handle,
                          AUDIO_IGNORE_VALUE,
                          AUDIO_IGNORE_VALUE,
                          status
                      ))
    {
#ifdef AUDIO_SNK_LC3_SUPPORT
        if (GA_TRUE == status)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_snk_create_lc3_decoder
                              (
                                  cs_conf,
                                  bps
                              ))
            {
                APPL_TRC("[APPL][GA][UCC][SNK]: LC3 Decoder Setup Status: Created\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SNK]: LC3 Decoder Setup Status: Could not create !\n");
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_snk_is_lc3_decoder_running())
            {
                if (GA_SUCCESS == appl_ga_utils_audio_snk_delete_lc3_decoder())
                {
                    APPL_TRC("[APPL][GA][UCC][SNK]: LC3 Decoder Setup Status: Deleted\n");
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][SNK]: LC3 Decoder Setup Status: Could not delete !\n");
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][UCC][SNK]: LC3 Decoder Setup Status: Already Deleted\n");
            }
        }
#else /* AUDIO_SNK_LC3_SUPPORT */
        APPL_ERR("AUDIO_SNK_LC3_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_LC3_SUPPORT */
#ifdef AUDIO_SNK_PL_SUPPORT
        if (GA_TRUE == status)
        {
            if (GA_SUCCESS == appl_ga_utils_audio_snk_setup_playback_pl
                              (
                                  cs_conf,
                                  bps,
                                  AUDIO_SNK_PL_BUFFER_SIZE
                              ))
            {
                APPL_TRC("[APPL][GA][UCC][SNK]: Audio PL Playback Setup Status: Success\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SNK]: Audio PL Playback Setup Status: Failed !\n");
            }
            if (GA_SUCCESS == appl_ga_utils_audio_snk_start_playback_pl())
            {
                APPL_TRC("[APPL][GA][UCC][SNK]: Audio PL Playback Start Process Status: Success\n");
            }
            else
            {
                APPL_ERR("[APPL][GA][UCC][SNK]: Audio PL Playback Start Process Status: Failed !\n");
            }
        }
        else if (GA_FALSE == status)
        {
            if (GA_TRUE == appl_ga_utils_audio_snk_is_playback_pl_running())
            {
                if (GA_SUCCESS == appl_ga_utils_audio_snk_stop_playback_pl())
                {
                    APPL_TRC("[APPL][GA][UCC][SNK]: Audio PL Playback Stop Process Status: Success\n");
                }
                else
                {
                    APPL_ERR("[APPL][GA][UCC][SNK]: Audio PL Playback Stop Process Status: Failed !\n");
                }
            }
            else
            {
                APPL_TRC("[APPL][GA][UCC][SNK]: Audio PL Playback Status: Already Stopped\n");
            }
        }
#else /* AUDIO_SNK_PL_SUPPORT */
        APPL_ERR("AUDIO_SNK_PL_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_PL_SUPPORT */
    }
    else
    {
        APPL_ERR("[APPL][GA][UCC][SNK]: ISO Data Path Setup Status: Update entry failed !\n");
    }
#else /* AUDIO_SNK_ISO_SUPPORT */
APPL_ERR("AUDIO_SNK_ISO_SUPPORT is not enabled !\n");
#endif /* AUDIO_SNK_ISO_SUPPORT */

    return;
}

#endif /* BAP_UCC */
