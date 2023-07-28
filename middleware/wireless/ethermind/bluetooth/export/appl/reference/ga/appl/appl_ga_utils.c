/**
 *  \file appl_ga_utils.c
 *
 *  Reference application for GA profile utils
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_ga_utils.h"
#include "GA_audio_config.h"
#include "appl_hci_le.h"
#include "appl_ga_utils_hci.h"
#include "GA_tmap_api.h"
#include "GA_pbp_api.h"

#ifdef BT_GAM

/* --------------------------------------------- Global Definitions */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
UINT8 ga_init_status = APPL_GA_INIT_STATUS_NONE;

APPL_GA_ROLE ga_role = APPL_GA_ROLE_NONE;

/** Advertising data maximum length */
#define APPL_GAP_ADVDATA_LEN                60U

/** Advertising Data type */
typedef struct _APPL_GA_GAP_ADV_DATA
{
    /** Data */
    UCHAR data[APPL_GAP_ADVDATA_LEN];

    /** Data Length */
    UCHAR datalen;

} APPL_GA_GAP_ADV_DATA;

/*
 * As per BASS spec:
 * The BASS UUID should be included in the Service Data AD Type in advertising
 * data transported using connectable extended advertising packets transmitted
 * by servers supporting this specification. Higher-layer specifications may
 * define additional service data to be included in the Service Data AD Type
 * used in advertising data.
 */
APPL_GA_GAP_ADV_DATA appl_ga_gap_adv_data =
{
    {
        /**
         *  Flags:
         *      0x01: LE Limited Discoverable Mode
         *      0x02: LE General Discoverable Mode
         *      0x04: BR/EDR Not Supported
         *      0x08: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Controller)
         *      0x10: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Host)
         */
        0x02U, GA_ADTYPE_FLAGS,
        (BT_AD_FLAGS_LE_GENERAL_DISC_MODE | BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED),

        /**
         *  CAP Announcement
         *  Service Data 16 bit UUID:
         *  ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U) = 0x04U
         *  ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         *  ADData:   Size: 2U, Value: Common Audio Service UUID: 0x1853 (GA_SERVICE_CAS)
         *                  1U         Announcement Type: 0x00U (CAS_GENERAL_ANNOUNCEMENT)
         */
        0x04U, GA_ADTYPE_SERVICE_DATA, 0x53U, 0x18U, 0x00U,

        /**
         *  BAP Announcement
         *  Service Data 16 bit UUID:
         *  ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U + 4U + 1U + XU)
         *  ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         *  ADData:   Size: 2U, Value: Audio Stream Control Service UUID: 0x184E (GA_SERVICE_ASCS)
         *                  1U,        Announcement Type: 0x00U (BAP_ASCS_GENERAL_ANNOUNCEMENT)
         *                  4U,        Available Audio Contexts:
         *                  1U,        Metadata_Length
         *                  X          Metadata
         */
        0x04U, GA_ADTYPE_SERVICE_DATA, 0x4EU, 0x18U, 0x00U,

        /**
         *  Service Data 16 bit UUID:
         *  ADLength: Size: 0x01, Value: ADType Len + ADValue Len = 1 + (2 + )
         *  ADType:   Size: 0x01, Value: 0x16 (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         *  ADData:   Size: 2U + Varies, Value: 0x184F (GA_SERVICE_BASS)
         */
        0x03U, GA_ADTYPE_SERVICE_DATA, 0x4FU, 0x18U,

        /**
         *  Service UUID List:
         *  ADLength: Size: 0x01, Value: ADType Len + ADValue Len = 1U + (2U + 2U) = 5U
         *  ADType:   Size: 0x01, Value: 0x03 (HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS)
         *  ADData: Size: 2U + 2U = 4U
         *      BASS (0x184F) (GA_SERVICE_BASS)
         *      PACS (0x1850) (GA_SERVICE_PACS)
         */
        0x05U, GA_ADTYPE_COMP_16BIT_UUID_LIST, 0x4FU, 0x18U, 0x50U, 0x18U,

        /**
         *  Shortened Device Name: Mt-GA
         */
        0x06U, GA_ADTYPE_SHORTENED_LOCAL_NAME, 'M', 't', '-', 'G', 'A',

        /* MISRA C-2012 Rule 9.3 */
        0x00U
    },

    /** Advertising Data length */
    30U
};

APPL_GA_GAP_ADV_DATA appl_ga_gap_adv_data_tmap =
{
    {
        /**
         *  Flags:
         *      0x01: LE Limited Discoverable Mode
         *      0x02: LE General Discoverable Mode
         *      0x04: BR/EDR Not Supported
         *      0x08: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Controller)
         *      0x10: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Host)
         */
        0x02U, GA_ADTYPE_FLAGS,
        (BT_AD_FLAGS_LE_GENERAL_DISC_MODE | BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED),

        /**
         *  TMAP Announcement
         *  Service Data 16 bit UUID:
         *  ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 2U) = 0x05U
         *  ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         *  ADData:   Size: 2U, Value: Telephony and Media Service UUID: 0x1855 (GA_SERVICE_TMAS)
         *                  2U, Value: TMAP Role: 0x00U (Bytes 12-13)
         *                             NOTE: This needs to be updated by calling appl_ga_utils_set_adv_data_for_current_tmap_role
         */
        0x05U, GA_ADTYPE_SERVICE_DATA, 0x55U, 0x18U, 0x00U, 0x00U,

        /**
         *  Shortened Device Name: Mt-GA-TMAP
         */
        0x0BU, GA_ADTYPE_SHORTENED_LOCAL_NAME, 'M', 't', '-', 'G', 'A', '-', 'T', 'M', 'A', 'P',

        /* MISRA C-2012 Rule 9.3 */
        0x00U
    },

    /** Advertising Data length */
    21U
};

APPL_GA_GAP_ADV_DATA appl_ga_gap_adv_data_hap =
{
    {
        /**
         *  Flags:
         *      0x01: LE Limited Discoverable Mode
         *      0x02: LE General Discoverable Mode
         *      0x04: BR/EDR Not Supported
         *      0x08: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Controller)
         *      0x10: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Host)
         */
        0x02U, GA_ADTYPE_FLAGS,
        (BT_AD_FLAGS_LE_GENERAL_DISC_MODE | BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED),

        /**
         *  Shortened Device Name: Mt-GA-HAP
         */
        0x0AU, GA_ADTYPE_SHORTENED_LOCAL_NAME, 'M', 't', '-', 'G', 'A', '-', 'H', 'A', 'P',

        /* MISRA C-2012 Rule 9.3 */
        0x00U
    },

    /** Advertising Data length */
    14U
};

REMOTE_PACS_CAP_INFO  remote_pacs[MAX_REM_PACS_CAP_INFO];
PACS_CAP_INFO         local_pacs;
CONN_DEVICES          conn_dev[MAX_CONN_DEVICES];
UINT8                 conn_dev_in_use;

/* Used to differentiate ongoing scan */
UINT8 is_scan_enabled = APPL_GA_SCAN_INVALID;

/* Used to differentiate ongoing advertise */
UINT8 is_adv_enabled = APPL_GA_ADV_INVALID;

void appl_ga_utils_display_adtype(UINT8 adtype);

void appl_ga_utils_display_announcement_type(UINT8 announcement_type);

void appl_ga_utils_display_adtype_flags(UINT8 flags);
void appl_ga_utils_display_adtype_flags_data(UCHAR* adv_data, UINT8 adlength);
void appl_ga_utils_display_adtype_comp_and_incomp_16b_uuid_data(UCHAR* adv_data, UINT8 adlength);
void appl_ga_utils_display_adtype_short_and_comp_local_name_data(UCHAR* adv_data, UINT8 adlength);
void appl_ga_utils_display_adtype_svc_data(UCHAR* adv_data, UINT8 adlength);
void appl_ga_utils_display_adtype_rsi(UCHAR* adv_data, UINT8 adlength);
void appl_ga_utils_display_adtype_appearance(UCHAR * adv_data, UINT8 adlength);
void appl_ga_utils_display_appearance_type(UINT16 appearance_type);
void appl_ga_utils_display_adtype_bc_name(UCHAR * adv_data, UINT8 adlength);
void appl_ga_utils_display_spaces(UINT8 start, UINT8 end);
void appl_ga_utils_tmap_display_role(UINT16 role);
void appl_ga_utils_display_adtype_svc_data_pbp_features(UINT8 pbp_features);
/* --------------------------------------------- Functions */

void appl_ga_utils_add_has_info_to_hap_adv_data(void);

void appl_ga_utils_fetch_adv_data_for_current_role
     (
         APPL_GA_GAP_ADV_DATA      * eadv_data,
         UINT8                       own_addr_type
     );

void appl_ga_utils_dev_manager_init(void)
{
    UINT8 i;

    /* Initialize */
    i = 0U;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        GA_INIT_ENDPOINT(&conn_dev[i].device);
        conn_dev[i].conn_handle = APPL_INVALID_CONN_HANDLE;
    }
    conn_dev_in_use = 0U;
    APPL_TRC("Default settings done for device manager\n");

    return;
}

void appl_ga_utils_pacs_init(void)
{
    UINT8 i;

    /* Initialize */
    i = 0U;

    for (i = 0U; i < MAX_REM_PACS_CAP_INFO; i++)
    {
        GA_mem_set
        (
            &remote_pacs[i].pacs,
            0x00U,
            sizeof(PACS_CAP_INFO)
        );
        remote_pacs[i].conn_dev_index = INVALID_CONN_DEV_INDEX;
    }

    GA_mem_set
    (
        &local_pacs,
        0x00U,
        sizeof(PACS_CAP_INFO)
    );

    return;
}

void appl_ga_utils_display_bdaddr_and_type(GA_ENDPOINT * device)
{
    UINT8 i;

    if (NULL != device)
    {
        APPL_TRC
        (
            "[BD ADDRESS]  : "
        );

        for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
        {
            APPL_TRC("0x%02X ", device->bd_addr[i]);
        }
        APPL_TRC("\n");
        APPL_TRC("[BD_ADDR TYPE]: 0x%02X\n", device->bd_type);
    }

    return;
}

GA_RESULT appl_ga_utils_display_ga_result(GA_RESULT result)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_SUCCESS;

    switch (result)
    {
    case GA_SUCCESS:
        APPL_TRC("GA_SUCCESS (0x%04X)\n", result);
        break;

    case GA_FAILURE:
        APPL_TRC("GA_FAILURE (0x%04X)\n", result);
        break;

    case GA_CONTINUE:
        APPL_TRC("GA_CONTINUE (0x%04X)\n", result);
        break;

    case GA_EVT_CONSUMED:
        APPL_TRC("GA_EVT_CONSUMED (0x%04X)\n", result);
        break;

    default:
        retval = GA_FAILURE;
        break;
    }
    return retval;
}

void appl_ga_utils_display_svc_uuid(UINT16 svc_uuid)
{
    switch (svc_uuid)
    {
    case GA_SERVICE_AICS:
        APPL_TRC
        (
            "Service UUID: Audio Input Control Service (0x%04X)\n",
            GA_SERVICE_AICS
        );
        break;
    case GA_SERVICE_VCS:
        APPL_TRC
        (
            "Service UUID: Volume Control Service (0x%04X)\n",
            GA_SERVICE_VCS
        );
        break;
    case GA_SERVICE_VOCS:
        APPL_TRC
        (
            "Service UUID: Volume Offset Control Service (0x%04X)\n",
            GA_SERVICE_VOCS
        );
        break;
    case GA_SERVICE_MICS:
        APPL_TRC
        (
            "Service UUID: Microphone Control Service (0x%04X)\n",
            GA_SERVICE_MICS
        );
        break;
    case GA_SERVICE_ASCS:
        APPL_TRC
        (
            "Service UUID: Audio Stream Control Service (0x%04X)\n",
            GA_SERVICE_ASCS
        );
        break;
    case GA_SERVICE_BASS:
        APPL_TRC
        (
            "Service UUID: Broadcast Audio Scan Service (0x%04X)\n",
            GA_SERVICE_BASS
        );
        break;
    case GA_SERVICE_PACS:
        APPL_TRC
        (
            "Service UUID: Published Audio Capabilities Service (0x%04X)\n",
            GA_SERVICE_PACS
        );
        break;
    case GA_SERVICE_BAAS:
        APPL_TRC
        (
            "Service UUID: Basic Audio Announcement Service (0x%04X)\n",
            GA_SERVICE_BAAS
        );
        break;
    case GA_SERVICE_BCAS:
        APPL_TRC
        (
            "Service UUID: Broadcast Audio Announcement Service (0x%04X)\n",
            GA_SERVICE_BCAS
        );
        break;
    case GA_SERVICE_PBAS:
        APPL_TRC
        (
            "Service UUID: Public Broadcast Announcement Service (0x%04X)\n",
            GA_SERVICE_PBAS
        );
        break;
    case GA_SERVICE_CSIS:
        APPL_TRC
        (
            "Service UUID: Coordinated Set Identification Service (0x%04X)\n",
            GA_SERVICE_CSIS
        );
        break;
    case GA_SERVICE_UUID_OTS:
        APPL_TRC
        (
            "Service UUID: Object Transfer Service (0x%04X)\n",
            GA_SERVICE_UUID_OTS
        );
        break;
    case GA_SERVICE_MCS:
        APPL_TRC
        (
            "Service UUID: Media Control Service (0x%04X)\n",
            GA_SERVICE_MCS
        );
        break;
    case GA_SERVICE_GMCS:
        APPL_TRC
        (
            "Service UUID: Generic Media Control Service (0x%04X)\n",
            GA_SERVICE_GMCS
        );
        break;
    case GA_SERVICE_CAS:
        APPL_TRC
        (
            "Service UUID: Common Audio Service (0x%04X)\n",
            GA_SERVICE_CAS
        );
        break;
    case GA_SERVICE_TMAS:
        APPL_TRC
        (
            "Service UUID: Telephony and Media Service (0x%04X)\n",
            GA_SERVICE_TMAS
        );
        break;
    case GA_SERVICE_TBS:
        APPL_TRC
        (
            "Service UUID: Telephone Bearer Service (0x%04X)\n",
            GA_SERVICE_TBS
        );
        break;
    case GA_SERVICE_GTBS:
        APPL_TRC
        (
            "Service UUID: Generic Telephone Bearer Service (0x%04X)\n",
            GA_SERVICE_GTBS
        );
        break;
    case GA_SERVICE_HAS:
        APPL_TRC
        (
            "Service UUID: Hearing Access Service (0x%04X)\n",
            GA_SERVICE_HAS
        );
        break;
    default:
        APPL_TRC("UnHandled Service UUID (0x%04X)\n", svc_uuid);
        break;
    }
    return;
}

/* Parser for printing Service Info - Start Range & End Range */
void appl_ga_utils_display_svc_range
     (
         GA_BRR_SVC_RANGE range
     )
{
    APPL_TRC("Data Length: 0x04\n");
    APPL_TRC("Data:\n");

    APPL_TRC("Len = 0x02,  Start Handle: (0x%04X)\n", range.start);
    APPL_TRC("Len = 0x02,  End Handle: (0x%04X)\n", range.end);

    return;
}

void appl_ga_utils_display_char_uuid(UINT16 char_uuid)
{
    switch (char_uuid)
    {
    case GA_CHAR_VCS_VOLUME_STATE:
        APPL_TRC("Char UUID: Volume State (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_VCS_VOLUME_FLAGS:
        APPL_TRC("Char UUID: Volume Flags (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_VOCS_OFFSET_STATE:
        APPL_TRC("Char UUID: Volume Offset State (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_VOCS_AUDIO_LOCATION:
        APPL_TRC("Char UUID: Audio Location (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_VOCS_AUDIO_OUTPUT_DESC:
        APPL_TRC("Char UUID: Audio Output Description (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_AICS_INPUT_STATE:
        APPL_TRC("Char UUID: Audio Input State (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_AICS_GAIN_SETTING:
        APPL_TRC("Char UUID: Gain Setting Properties (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_AICS_INPUT_TYPE:
        APPL_TRC("Char UUID: Audio Input Type (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_AICS_INPUT_STATUS:
        APPL_TRC("Char UUID: Audio Input Status (0x%04X)\n", char_uuid);
        break;

    case GA_CHAR_AICS_AUDIO_INPUT_DESC:
        APPL_TRC("Char UUID: Audio Input Description (0x%04X)\n", char_uuid);
        break;

    default:
        APPL_TRC("UnHandled Characteristic UUID (0x%04X)\n", char_uuid);
        break;
    }

    return;
}

void appl_ga_utils_display_mute(UINT8 mute)
{
#if (defined GA_VCP)
    if (VCS_MUTE_NO == mute)
#elif (defined GA_MICP)
    if (MICS_CHR_VAL_NOT_MUTED == mute)
#else
    /* This wont hit when VCP and MICP are undefined */
#endif /* GA_VCP/GA_MICP */
    {
        APPL_TRC("Not Muted (0x%02X)\n", mute);
    }

#if (defined GA_VCP)
    else if (VCS_MUTE_YES == mute)
#elif (defined GA_MICP)
    else if (MICS_CHR_VAL_MUTED == mute)
#else
    /* This wont hit when VCP and MICP are undefined */
#endif /* GA_VCP/GA_MICP */
    {
        APPL_TRC("Muted (0x%02X)\n", mute);
    }

#if (defined GA_VCP)
    /* AICS could be a part of VCP even if MICP is undefiend */
    else if (AICS_DISABLED == mute)
#elif (defined GA_MICP)
    else if (MICS_CHR_VAL_DISABLED == mute)
#else
    /* This wont hit when VCP and MICP are undefined */
#endif /* GA_VCP/GA_MICP */
    {
        APPL_TRC("Disabled (0x%02X)\n", mute);
    }
    else
    {
        APPL_TRC("Invalid Mute Value\n)");
    }

    return;
}

void appl_ga_utils_display_audio_loc(UINT32 audio_loc)
{
    switch(audio_loc)
    {
    case GA_AUDIO_LOCATION_NOT_ALLOWED:
        APPL_TRC("Not Allowed (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FL:
        APPL_TRC("Front Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FR:
        APPL_TRC("Front Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FC:
        APPL_TRC("Front Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_LOW_FREQ_EFF_1:
        APPL_TRC("Low Frequency Effects 1 (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BL:
        APPL_TRC("Back Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BR:
        APPL_TRC("Back Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FLC:
        APPL_TRC("Front Left of Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FRC:
        APPL_TRC("Front Right of Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BC:
        APPL_TRC("Back Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_LOW_FREQ_EFF_2:
        APPL_TRC("Low Frequency Effects 2 (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_SL:
        APPL_TRC("Side Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_SR:
        APPL_TRC("Side Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TFL:
        APPL_TRC("Top Front Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TFR:
        APPL_TRC("Top Front Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TFC:
        APPL_TRC("Top Front Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TC:
        APPL_TRC("Top Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TBL:
        APPL_TRC("Top Back Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TBR:
        APPL_TRC("Top Back Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TSL:
        APPL_TRC("Top Side Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TSR:
        APPL_TRC("Top Side Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_TBC:
        APPL_TRC("Top Back Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BFC:
        APPL_TRC("Bottom Front Center (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BFL:
        APPL_TRC("Bottom Front Left (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_BFR:
        APPL_TRC("Bottom Front Right (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FLW:
        APPL_TRC("Front Left Wide (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_FRW:
        APPL_TRC("Front Right Wide (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_LS:
        APPL_TRC("Left Surround (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_RS:
        APPL_TRC("Right Surround (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_RFU_1:
        APPL_TRC("RFU 1 (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_RFU_2:
        APPL_TRC("RFU 2 (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_RFU_3:
        APPL_TRC("RFU 3 (0x%08X)\n", audio_loc);
        break;

    case GA_AUDIO_LOCATION_RFU_4:
        APPL_TRC("RFU 4 (0x%08X)\n", audio_loc);
        break;

    default:
        APPL_TRC
        (
            "UnHandled Audio Location Value (0x%08X)\n",
            audio_loc
        );
        break;
    }

    return;
}

void appl_ga_utils_bap_display_target_latency(UINT8 target_latency)
{
    if (GA_TARGET_LATENCY_LOW == target_latency)
    {
        APPL_TRC("Target low latency");
    }
    else if (GA_TARGET_LATENCY_BALANCED_AND_RELAIBILITY == target_latency)
    {
        APPL_TRC("Target balanced latency and reliability");
    }
    else if (GA_TARGET_LATENCY_HIGH_RELIABILITY == target_latency)
    {
        APPL_TRC("Target high reliability");
    }
    else
    {
        APPL_TRC("Unknown");
    }
    APPL_TRC(" (0x%02X)\n", target_latency);

    return;
}

void appl_ga_utils_bap_display_target_latency_for_input(void)
{
    CONSOLE_OUT("Target low latency:                      0x01 \n");
    CONSOLE_OUT("Target balanced latency and reliability: 0x02 \n");
    CONSOLE_OUT("Target high reliability:                 0x03 \n");

    return;
}

void appl_ga_utils_bap_display_target_phy_for_input(void)
{
    CONSOLE_OUT("No Preference:              0x00 \n");
    CONSOLE_OUT("LE 1M PHY:                  0x01 \n");
    CONSOLE_OUT("LE 2M PHY:                  0x02 \n");
    CONSOLE_OUT("LE Coded PHY:               0x03 \n");

    CONSOLE_OUT("TMAP Role: UMS, M: LE 2M PHY\n");

    return;
}

void appl_ga_utils_bap_display_pref_phy_for_input(void)
{
    CONSOLE_OUT("No Preference:              0x00 \n");
    CONSOLE_OUT("LE 1M PHY preferred:        0x01 \n");
    CONSOLE_OUT("LE 2M PHY preferred:        0x02 \n");
    CONSOLE_OUT("LE Coded PHY preferred:     0x04 \n");

    CONSOLE_OUT("TMAP Role: UMS, M: LE 2M PHY\n");
    CONSOLE_OUT("TMAP Role: UMR, M: LE 2M PHY\n");
    CONSOLE_OUT("TMAP Role: BMS, M: LE 2M PHY\n");
    CONSOLE_OUT("TMAP Role: BMR, M: LE 2M PHY\n");

    return;
}

void appl_ga_utils_bap_display_audio_locations_for_input(void)
{
    CONSOLE_OUT("Not Allowed:                 0x00000000 \n");
    CONSOLE_OUT("Front Left:                  0x00000001 \n");
    CONSOLE_OUT("Front Right:                 0x00000002 \n");
    CONSOLE_OUT("Front Center:                0x00000004 \n");
    CONSOLE_OUT("Low Frequency Effects 1:     0x00000008 \n");
    CONSOLE_OUT("Back Left:                   0x00000010 \n");
    CONSOLE_OUT("Back Right:                  0x00000020 \n");
    CONSOLE_OUT("Front Left of Center:        0x00000040 \n");
    CONSOLE_OUT("Front Right of Center:       0x00000080 \n");
    CONSOLE_OUT("Back Center:                 0x00000100 \n");
    CONSOLE_OUT("Low Frequency Effects 2:     0x00000200 \n");
    CONSOLE_OUT("Side Left:                   0x00000400 \n");
    CONSOLE_OUT("Side Right:                  0x00000800 \n");
    CONSOLE_OUT("Top Front Left:              0x00001000 \n");
    CONSOLE_OUT("Top Front Right:             0x00002000 \n");
    CONSOLE_OUT("Top Front Center:            0x00004000 \n");
    CONSOLE_OUT("Top Center:                  0x00008000 \n");
    CONSOLE_OUT("Top Back Left:               0x00010000 \n");
    CONSOLE_OUT("Top Back Right:              0x00020000 \n");
    CONSOLE_OUT("Top Side Left:               0x00040000 \n");
    CONSOLE_OUT("Top Side Right:              0x00080000 \n");
    CONSOLE_OUT("Top Back Center:             0x00100000 \n");
    CONSOLE_OUT("Bottom Front Center:         0x00200000 \n");
    CONSOLE_OUT("Bottom Front Left:           0x00400000 \n");
    CONSOLE_OUT("Bottom Front Right:          0x00800000 \n");
    CONSOLE_OUT("Front Left Wide:             0x01000000 \n");
    CONSOLE_OUT("Front Right Wide:            0x02000000 \n");
    CONSOLE_OUT("Left Surround:               0x04000000 \n");
    CONSOLE_OUT("Right Surround:              0x08000000 \n");
    CONSOLE_OUT("RFU 1:                       0x10000000 \n");
    CONSOLE_OUT("RFU 2:                       0x20000000 \n");
    CONSOLE_OUT("RFU 3:                       0x40000000 \n");
    CONSOLE_OUT("RFU 4:                       0x80000000 \n");

    CONSOLE_OUT("TMAP Role: UMS, M: Front Left, Front Right\n");
    CONSOLE_OUT("TMAP Role: UMR, M: Front Left, Front Right, Front Right and Front Left\n");
    CONSOLE_OUT("TMAP Role: BMS, M: Front Left, Front Right\n");
    CONSOLE_OUT("TMAP Role: BMR, M: Front Left or Front Right or Front Right and Front Left\n");

    return;
}

void appl_ga_utils_bap_display_ctx_type_for_input(void)
{
    CONSOLE_OUT("Prohibited:           0x0000 \n");
    CONSOLE_OUT("Unspecified:          0x0001 \n");
    CONSOLE_OUT("Conversational:       0x0002 \n");
    CONSOLE_OUT("Media:                0x0004 \n");
    CONSOLE_OUT("Game:                 0x0008 \n");
    CONSOLE_OUT("Instructional:        0x0010 \n");
    CONSOLE_OUT("Voice assistants:     0x0020 \n");
    CONSOLE_OUT("Live:                 0x0040 \n");
    CONSOLE_OUT("Sound effects:        0x0080 \n");
    CONSOLE_OUT("Notifications:        0x0100 \n");
    CONSOLE_OUT("Ringtone:             0x0200 \n");
    CONSOLE_OUT("Alerts:               0x0400 \n");
    CONSOLE_OUT("Emergency alarm:      0x0800 \n");

    return;
}

GA_RESULT appl_ga_utils_bap_prepare_cs_cap_based_on_set_id
          (
              UINT8 csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CAP * csc
          )
{
    GA_RESULT retval;
    UINT16 min_supported_opcf;
    UINT16 max_supported_opcf;
    int choice;

    if (NULL == csc)
    {
        APPL_ERR("Invalid parameter !\n");
        return GA_FAILURE;
    }

    /* Initialize */
    retval = GA_SUCCESS;
    min_supported_opcf = 0x0000U;
    max_supported_opcf = 0x0000U;
    choice = 0x0000;

    /*
     * A single PAC Record can have multiple SF, FD hence combine them,
     * Also, ensure to update min opcf to the minimum of all values
     * and max to maximum of all values.
     */
    switch (csc_set_id)
    {
    case 1:
        csc->supported_sf |= GA_CODEC_CAP_8_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_8_1_FD;
        min_supported_opcf = GA_8_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_8_1_1_MAXSDUSIZE_BYTES;
        break;

    case 2:
        csc->supported_sf |= GA_CODEC_CAP_8_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_8_2_FD;
        min_supported_opcf = GA_8_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_8_2_1_MAXSDUSIZE_BYTES;
        break;

    case 3:
        csc->supported_sf |= GA_CODEC_CAP_16_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_16_1_FD;
        min_supported_opcf = GA_16_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_16_1_1_MAXSDUSIZE_BYTES;
        break;

    case 4:
        csc->supported_sf |= GA_CODEC_CAP_16_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_16_2_FD;
        min_supported_opcf = GA_16_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_16_2_1_MAXSDUSIZE_BYTES;
        break;

    case 5:
        csc->supported_sf |= GA_CODEC_CAP_24_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_24_1_FD;
        min_supported_opcf = GA_24_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_24_1_1_MAXSDUSIZE_BYTES;
        break;

    case 6:
        csc->supported_sf |= GA_CODEC_CAP_24_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_24_2_FD;
        min_supported_opcf = GA_24_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_24_2_1_MAXSDUSIZE_BYTES;
        break;

    case 7:
        csc->supported_sf |= GA_CODEC_CAP_32_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_32_1_FD;
        min_supported_opcf = GA_32_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_32_1_1_MAXSDUSIZE_BYTES;
        break;

    case 8:
        csc->supported_sf |= GA_CODEC_CAP_32_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_32_2_FD;
        min_supported_opcf = GA_32_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_32_2_1_MAXSDUSIZE_BYTES;
        break;

    case 9:
        csc->supported_sf |= GA_CODEC_CAP_441_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_441_1_FD;
        min_supported_opcf = GA_441_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_441_1_1_MAXSDUSIZE_BYTES;
        break;

    case 10:
        csc->supported_sf |= GA_CODEC_CAP_441_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_441_2_FD;
        min_supported_opcf = GA_441_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_441_2_1_MAXSDUSIZE_BYTES;
        break;

    case 11:
        csc->supported_sf |= GA_CODEC_CAP_48_1_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_1_FD;
        min_supported_opcf = GA_48_1_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_1_1_MAXSDUSIZE_BYTES;
        break;

    case 12:
        csc->supported_sf |= GA_CODEC_CAP_48_2_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_2_FD;
        min_supported_opcf = GA_48_2_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_2_1_MAXSDUSIZE_BYTES;
        break;

    case 13:
        csc->supported_sf |= GA_CODEC_CAP_48_3_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_3_FD;
        min_supported_opcf = GA_48_3_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_3_1_MAXSDUSIZE_BYTES;
        break;

    case 14:
        csc->supported_sf |= GA_CODEC_CAP_48_4_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_4_FD;
        min_supported_opcf = GA_48_4_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_4_1_MAXSDUSIZE_BYTES;
        break;

    case 15:
        csc->supported_sf |= GA_CODEC_CAP_48_5_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_5_FD;
        min_supported_opcf = GA_48_5_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_5_1_MAXSDUSIZE_BYTES;
        break;

    case 16:
        csc->supported_sf |= GA_CODEC_CAP_48_6_SF;
        csc->supported_fd |= GA_CODEC_CAP_48_6_FD;
        min_supported_opcf = GA_48_6_1_MAXSDUSIZE_BYTES;
        max_supported_opcf = GA_48_6_1_MAXSDUSIZE_BYTES;
        break;

    default:
        APPL_ERR("Unknown Set !\n");
        retval = GA_FAILURE;
        break;
    }

    if (GA_SUCCESS == retval)
    {
        /* TODO: Add reference of Spec */

        /*
         * If the new value is Less/Greater than the Min/<ax already set,
         * Set the new value as Min/Max.
         */
        if (min_supported_opcf < (csc->supported_opcf & 0xFFFFU))
        {
            csc->supported_opcf |= min_supported_opcf;
        }
        if (max_supported_opcf > ((csc->supported_opcf & 0xFFFF0000U) >> 16U))
        {
            csc->supported_opcf |= (max_supported_opcf << 16U);
        }
    }

    return retval;
}

void appl_ga_utils_bap_display_cs_cap_set_for_input(void)
{
    /* Refer: Page 25 of BAP spec version 1.0.1, Table: 3.5 */
    CONSOLE_OUT("Codec Specific Capabilities: \n");
    CONSOLE_OUT
    (
        "Set ID.    Set Name:    Codec ID    Supp Sampling Freq    "
        "Supp Frame Dur    Supp Octets per codec frame    "
        "Bitrate\n"
    );
    CONSOLE_OUT("   1.       Set  8_1  :     LC3              8kHz              7.5ms                    26                 27.734kbps\n");
    CONSOLE_OUT("   2.       Set  8_2  :     LC3              8kHz               10ms                    30                     24kbps\n");
    CONSOLE_OUT("   3.       Set  16_1 :     LC3             16kHz              7.5ms                    30                     32kbps\n");
    CONSOLE_OUT("   4.       Set  16_2 :     LC3             16kHz               10ms                    40                     32kbps\n");
    CONSOLE_OUT("   5.       Set  24_1 :     LC3             24kHz              7.5ms                    45                     48kbps\n");
    CONSOLE_OUT("   6.       Set  24_2 :     LC3             24kHz               10ms                    60                     48kbps\n");
    CONSOLE_OUT("   7.       Set  32_1 :     LC3             32kHz              7.5ms                    60                     64kbps\n");
    CONSOLE_OUT("   8.       Set  32_2 :     LC3             32kHz               10ms                    80                     64kbps\n");
    CONSOLE_OUT("   9.       Set 441_1 :     LC3           44.1kHz              7.5ms                    97                  95.06kbps\n");
    CONSOLE_OUT("  10.       Set 441_2 :     LC3           44.1kHz               10ms                   130                  95.55kbps\n");
    CONSOLE_OUT("  11.       Set  48_1 :     LC3             48kHz              7.5ms                    75                     80kbps\n");
    CONSOLE_OUT("  12.       Set  48_2 :     LC3             48kHz               10ms                   100                     80kbps\n");
    CONSOLE_OUT("  13.       Set  48_3 :     LC3             48kHz              7.5ms                    90                     96kbps\n");
    CONSOLE_OUT("  14.       Set  48_4 :     LC3             48kHz               10ms                   120                     96kbps\n");
    CONSOLE_OUT("  15.       Set  48_5 :     LC3             48kHz              7.5ms                   117                  124.8kbps\n");
    CONSOLE_OUT("  16.       Set  48_6 :     LC3             48kHz               10ms                   155                    124kbps\n");

    CONSOLE_OUT("TMAP Role: UMR, M: 16_2, 24_2, 48_1, 48_2, 48_3, 48_4, 48_5, 48_6\n");
    CONSOLE_OUT("TMAP Role: BMR, M: 16_2, 24_2, 48_1, 48_2, 48_3, 48_4, 48_5, 48_6\n");

    return;
}

void appl_ga_utils_bap_display_cs_conf_set_for_input(void)
{
    /* Refer: Page 33 of BAP spec version 1.0.1, Table: 3.11 */
    CONSOLE_OUT("Codec Specific Configuration: \n");
    CONSOLE_OUT
    (
        "Set ID.    Set Name:    Codec ID    Supp Sampling Freq    "
        "Supp Frame Dur    Supp Octets per codec frame    "
        "Bitrate\n"
    );
    CONSOLE_OUT("   1.       Set  8_1  :     LC3              8kHz              7.5ms                    26                 27.734kbps\n");
    CONSOLE_OUT("   2.       Set  8_2  :     LC3              8kHz               10ms                    30                     24kbps\n");
    CONSOLE_OUT("   3.       Set  16_1 :     LC3             16kHz              7.5ms                    30                     32kbps\n");
    CONSOLE_OUT("   4.       Set  16_2 :     LC3             16kHz               10ms                    40                     32kbps\n");
    CONSOLE_OUT("   5.       Set  24_1 :     LC3             24kHz              7.5ms                    45                     48kbps\n");
    CONSOLE_OUT("   6.       Set  24_2 :     LC3             24kHz               10ms                    60                     48kbps\n");
    CONSOLE_OUT("   7.       Set  32_1 :     LC3             32kHz              7.5ms                    60                     64kbps\n");
    CONSOLE_OUT("   8.       Set  32_2 :     LC3             32kHz               10ms                    80                     64kbps\n");
    CONSOLE_OUT("   9.       Set 441_1 :     LC3           44.1kHz              7.5ms                    97                  95.06kbps\n");
    CONSOLE_OUT("  10.       Set 441_2 :     LC3           44.1kHz               10ms                   130                  95.55kbps\n");
    CONSOLE_OUT("  11.       Set  48_1 :     LC3             48kHz              7.5ms                    75                     80kbps\n");
    CONSOLE_OUT("  12.       Set  48_2 :     LC3             48kHz               10ms                   100                     80kbps\n");
    CONSOLE_OUT("  13.       Set  48_3 :     LC3             48kHz              7.5ms                    90                     96kbps\n");
    CONSOLE_OUT("  14.       Set  48_4 :     LC3             48kHz               10ms                   120                     96kbps\n");
    CONSOLE_OUT("  15.       Set  48_5 :     LC3             48kHz              7.5ms                   117                  124.8kbps\n");
    CONSOLE_OUT("  16.       Set  48_6 :     LC3             48kHz               10ms                   155                    124kbps\n");

    CONSOLE_OUT("TMAP Role: UMS, M: 48_2, 48_4 OR 48_6\n");

    return;
}

void appl_ga_utils_bap_uc_display_qos_cs_cap_set_for_input(void)
{
    /* Refer: Page 85 of BAP spec version 1.0, Table: 5.2 */
    APPL_TRC("QoS Codec Capability/Configuration Setting\n");
    APPL_TRC("NOTE: Ensure to enter Set ID corresponding to Records added "
             "in PACS.\n");
    APPL_TRC
    (
        "Set ID.    Set Name:    Codec Cap/Conf Setting    SDU Interval    "
        "Framing    Max SDU Size    Retransmission Num    "
        "Max Transport Latency    Presentation Delay\n"
    );

    APPL_TRC("QoS Capability/Conf settings for low latency audio data:\n");
    APPL_TRC("   1.      Set  8_1_1  :          8_1                7500us        unframed         26                 2                     8ms                40000us\n");
    APPL_TRC("   2.      Set  8_2_1  :          8_2               10000us        unframed         30                 2                    10ms                40000us\n");
    APPL_TRC("   3.      Set  16_1_1 :         16_1                7500us        unframed         30                 2                     8ms                40000us\n");
    APPL_TRC("   4.      Set  16_2_1 :         16_2               10000us        unframed         40                 2                    10ms                40000us\n");
    APPL_TRC("   5.      Set  24_1_1 :         24_1                7500us        unframed         45                 2                     8ms                40000us\n");
    APPL_TRC("   6.      Set  24_2_1 :         24_2               10000us        unframed         60                 2                    10ms                40000us\n");
    APPL_TRC("   7.      Set  32_1_1 :         32_1                7500us        unframed         60                 2                     8ms                40000us\n");
    APPL_TRC("   8.      Set  32_2_1 :         32_2               10000us        unframed         80                 2                    10ms                40000us\n");
    APPL_TRC("   9.      Set 441_1_1 :        441_1                7500us          framed         97                 5                    24ms                40000us\n");
    APPL_TRC("  10.      Set 441_2_1 :        441_2               10000us          framed        130                 5                    31ms                40000us\n");
    APPL_TRC("  11.      Set  48_1_1 :         48_1                7500us        unframed         75                 5                    15ms                40000us\n");
    APPL_TRC("  12.      Set  48_2_1 :         48_2               10000us        unframed        100                 5                    20ms                40000us\n");
    APPL_TRC("  13.      Set  48_3_1 :         48_3                7500us        unframed         90                 5                    15ms                40000us\n");
    APPL_TRC("  14.      Set  48_4_1 :         48_4               10000us        unframed        120                 5                    20ms                40000us\n");
    APPL_TRC("  15.      Set  48_5_1 :         48_5                7500us        unframed        117                 5                    15ms                40000us\n");
    APPL_TRC("  16.      Set  48_6_1 :         48_6               10000us        unframed        155                 5                    20ms                40000us\n");

    APPL_TRC("QoS Capability/Conf settings for high-reliability audio data:\n");
    APPL_TRC("  17.      Set  8_1_2  :          8_1                7500us        unframed         26                13                    75ms                40000us\n");
    APPL_TRC("  18.      Set  8_2_2  :          8_2               10000us        unframed         30                13                    95ms                40000us\n");
    APPL_TRC("  19.      Set  16_1_2 :         16_1                7500us        unframed         30                13                    75ms                40000us\n");
    APPL_TRC("  20.      Set  16_2_2 :         16_2               10000us        unframed         40                13                    95ms                40000us\n");
    APPL_TRC("  21.      Set  24_1_2 :         24_1                7500us        unframed         45                13                    75ms                40000us\n");
    APPL_TRC("  22.      Set  24_2_2 :         24_2               10000us        unframed         60                13                    95ms                40000us\n");
    APPL_TRC("  23.      Set  32_1_2 :         32_1                7500us        unframed         60                13                    75ms                40000us\n");
    APPL_TRC("  24.      Set  32_2_2 :         32_2               10000us        unframed         80                13                    95ms                40000us\n");
    APPL_TRC("  25.      Set 441_1_2 :        441_1                7500us          framed         97                13                    80ms                40000us\n");
    APPL_TRC("  26.      Set 441_2_2 :        441_2               10000us          framed        130                13                    85ms                40000us\n");
    APPL_TRC("  27.      Set  48_1_2 :         48_1                7500us        unframed         75                13                    75ms                40000us\n");
    APPL_TRC("  28.      Set  48_2_2 :         48_2               10000us        unframed        100                13                    95ms                40000us\n");
    APPL_TRC("  29.      Set  48_3_2 :         48_3                7500us        unframed         90                13                    75ms                40000us\n");
    APPL_TRC("  30.      Set  48_4_2 :         48_4               10000us        unframed        120                13                   100ms                40000us\n");
    APPL_TRC("  31.      Set  48_5_2 :         48_5                7500us        unframed        117                13                    75ms                40000us\n");
    APPL_TRC("  32.      Set  48_6_2 :         48_6               10000us        unframed        155                13                   100ms                40000us\n");

    CONSOLE_OUT("TMAP Role: UMS, M: Low Latency: Set 16_2_1, 48_2_1, 48_4_1, 48_6_1 \
                                    High Reliability: Set 48_2_2, 48_4_2, 48_6_2 \n");
    CONSOLE_OUT("TMAP Role: UMR, M: Low Latency: Set 16_2_1, 24_2_1, 48_1_1, 48_2_1, 48_3_1, 48_4_1, 48_5_1, 48_6_1 \
                                    High Reliability: Set 16_2_2, 24_2_2, 48_1_2, 48_2_2, 48_3_2, 48_4_2, 48_5_2, 48_6_2 \n");
    CONSOLE_OUT("TMAP Role: BMR, M: Low Latency: Set 16_2_1, 24_2_1, 48_1_1, 48_2_1, 48_3_1, 48_4_1, 48_5_1, 48_6_1 \
                                    High Reliability: Set 16_2_2, 24_2_2, 48_1_2, 48_2_2, 48_3_2, 48_4_2, 48_5_2, 48_6_2 \n");

    return;
}

void appl_ga_utils_bap_bc_display_qos_cs_cap_set_for_input(void)
{
    /* Refer: Page 111 of BAP spec version 1.0, Table: 6.4 */
    APPL_TRC
    (
        "Set ID.    Set Name:    Codec Cap/Conf Setting    SDU Interval    "
        "Framing    Max SDU Size    Retransmission Num    "
        "Max Transport Latency    Presentation Delay\n"
    );

    APPL_TRC("Broadcast Audio Stream configuration settings for low latency audio data:\n");
    APPL_TRC("   1.      Set  8_1_1  :          8_1                7500us        unframed         26                 2                     8ms                40000us\n");
    APPL_TRC("   2.      Set  8_2_1  :          8_2               10000us        unframed         30                 2                    10ms                40000us\n");
    APPL_TRC("   3.      Set  16_1_1 :         16_1                7500us        unframed         30                 2                     8ms                40000us\n");
    APPL_TRC("   4.      Set  16_2_1 :         16_2               10000us        unframed         40                 2                    10ms                40000us\n");
    APPL_TRC("   5.      Set  24_1_1 :         24_1                7500us        unframed         45                 2                     8ms                40000us\n");
    APPL_TRC("   6.      Set  24_2_1 :         24_2               10000us        unframed         60                 2                    10ms                40000us\n");
    APPL_TRC("   7.      Set  32_1_1 :         32_1                7500us        unframed         60                 2                     8ms                40000us\n");
    APPL_TRC("   8.      Set  32_2_1 :         32_2               10000us        unframed         80                 2                    10ms                40000us\n");
    APPL_TRC("   9.      Set 441_1_1 :        441_1                7500us          framed         97                 4                    24ms                40000us\n");
    APPL_TRC("  10.      Set 441_2_1 :        441_2               10000us          framed        130                 4                    31ms                40000us\n");
    APPL_TRC("  11.      Set  48_1_1 :         48_1                7500us        unframed         75                 4                    15ms                40000us\n");
    APPL_TRC("  12.      Set  48_2_1 :         48_2               10000us        unframed        100                 4                    20ms                40000us\n");
    APPL_TRC("  13.      Set  48_3_1 :         48_3                7500us        unframed         90                 4                    15ms                40000us\n");
    APPL_TRC("  14.      Set  48_4_1 :         48_4               10000us        unframed        120                 4                    20ms                40000us\n");
    APPL_TRC("  15.      Set  48_5_1 :         48_5                7500us        unframed        117                 4                    15ms                40000us\n");
    APPL_TRC("  16.      Set  48_6_1 :         48_6               10000us        unframed        155                 4                    20ms                40000us\n");

    APPL_TRC("Broadcast Audio Stream configuration settings for high-reliability audio data:\n");
    APPL_TRC("  17.      Set  8_1_2  :          8_1                7500us        unframed         26                 4                    45ms                40000us\n");
    APPL_TRC("  18.      Set  8_2_2  :          8_2               10000us        unframed         30                 4                    60ms                40000us\n");
    APPL_TRC("  19.      Set  16_1_2 :         16_1                7500us        unframed         30                 4                    45ms                40000us\n");
    APPL_TRC("  20.      Set  16_2_2 :         16_2               10000us        unframed         40                 4                    60ms                40000us\n");
    APPL_TRC("  21.      Set  24_1_2 :         24_1                7500us        unframed         45                 4                    45ms                40000us\n");
    APPL_TRC("  22.      Set  24_2_2 :         24_2               10000us        unframed         60                 4                    60ms                40000us\n");
    APPL_TRC("  23.      Set  32_1_2 :         32_1                7500us        unframed         60                 4                    45ms                40000us\n");
    APPL_TRC("  24.      Set  32_2_2 :         32_2               10000us        unframed         80                 4                    60ms                40000us\n");
    APPL_TRC("  25.      Set 441_1_2 :        441_1                7500us          framed         97                 4                    54ms                40000us\n");
    APPL_TRC("  26.      Set 441_2_2 :        441_2               10000us          framed        130                 4                    60ms                40000us\n");
    APPL_TRC("  27.      Set  48_1_2 :         48_1                7500us        unframed         75                 4                    50ms                40000us\n");
    APPL_TRC("  28.      Set  48_2_2 :         48_2               10000us        unframed        100                 4                    65ms                40000us\n");
    APPL_TRC("  29.      Set  48_3_2 :         48_3                7500us        unframed         90                 4                    50ms                40000us\n");
    APPL_TRC("  30.      Set  48_4_2 :         48_4               10000us        unframed        120                 4                    65ms                40000us\n");
    APPL_TRC("  31.      Set  48_5_2 :         48_5                7500us        unframed        117                 4                    50ms                40000us\n");
    APPL_TRC("  32.      Set  48_6_2 :         48_6               10000us        unframed        155                 4                    65ms                40000us\n");

    CONSOLE_OUT("TMAP Role: BMS, M: Low Latency: Set 16_2_1, 48_1_1, 48_2_1, 48_3_1/48_5_1, 48_4_1/48_6_1 \
                                    High Reliability: Set 16_2_2, 48_1_2, 48_2_2, 48_3_2/48_5_2, 48_4_2/48_6_2 \n");
    CONSOLE_OUT("Broadcast Profile Type: PBP, PBP Role: PBS \
                                              Standard Quality: 16_2_1, 24_2_1, 16_2_2, 24_2_2 \
                                              High Quality: 48_1_1, 48_2_1, 48_3_1, 48_4_1, 48_5_1, 48_6_1, 48_1_2, 48_2_2, 48_3_2, 48_4_2, 48_5_2, 48_6_2\n");

    return;
}

GA_RESULT appl_ga_utils_bap_uc_prepare_csc_and_qos_cap_based_on_set_id
          (
              UINT8                        csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CONF * csc,
              GA_QOS_CAP                 * qos_cap,
              GA_QOS_CONF                * qos_conf
          )
{
    GA_RESULT retval;
    int       choice;

    /* Initialize */
    retval = GA_SUCCESS;
    choice = 0x00000;

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    /* Set PHY Preference */
    if (NULL != qos_cap)
    {
        qos_cap->pref_phy = appl_ga_utils_bap_get_pref_phy();
    }
    if(NULL != qos_conf)
    {
        qos_conf->phy = appl_ga_utils_bap_get_pref_phy();
    }
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

    /* For any TMAP Role, TMAP Mandates 2M Phy */
    /* Set PHY Preference */
    if (NULL != qos_cap)
    {
        qos_cap->pref_phy = GA_PREF_PHY_LE_2M_PHY;
    }
    if (NULL != qos_conf)
    {
        qos_conf->phy = GA_PREF_PHY_LE_2M_PHY;
    }
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

    if (NULL != csc)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        /* Set Max Codec Frames Per SDU */
        csc->mcfpSDU = appl_ga_utils_bap_get_max_codec_frames_per_sdu();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* Set Codec Frame Blocks Per SDU */
        /*
         * The absence of the Codec_Frame_Blocks_Per_SDU LTV structure shall be
         * interpreted as equivalent to a Codec_Frame_Blocks_Per_SDU value of 0x01.
         */
        csc->mcfpSDU = 0x01U;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    }

    if (NULL != qos_cap)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        /* TODO: Input Narrower Range Presentation Delay Min and Max */
        qos_cap->pref_presentation_delay_min =
            appl_ga_utils_bap_get_pref_presentation_delay();
        qos_cap->pref_presentation_delay_max =
            appl_ga_utils_bap_get_pref_presentation_delay();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        qos_cap->pref_presentation_delay_min =
            GA_PRESENTATION_DELAY_NO_PREF; /* No Preference */
        qos_cap->pref_presentation_delay_max =
            GA_PRESENTATION_DELAY_NO_PREF; /* No Preference */
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    }

    if (NULL != csc)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        /* Set Audio Channel Allocation */
        csc->aca = appl_ga_utils_bap_get_audio_channel_alloc();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

        /*
         * TMAP Role: UMR, FL, FR, FL + FR
         * TMAP Role: UMS, FL, FR
         */
        /* Set Audio Channel Allocation */
        csc->aca = GA_AUDIO_LOCATION_FL;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    }

    switch (csc_set_id)
    {
        case 1:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_8_1_SF;
                csc->fd = GA_CODEC_CONF_8_1_FD;
                csc->opcf = GA_CODEC_CONF_8_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_8_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_8_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_8_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_8_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_8_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_8_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_8_1_1_FRAMING;
                qos_conf->max_sdu = GA_8_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_8_1_1_RTN;
                qos_conf->max_transport_latency = GA_8_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_8_1_1_PRESENTDELAY_uS;
            }
            break;

        case 2:
            if (csc)
            {
                csc->sf = GA_CODEC_CONF_8_2_SF;
                csc->fd = GA_CODEC_CONF_8_2_FD;
                csc->opcf = GA_CODEC_CONF_8_2_OF;
            }
            if (qos_cap)
            {
                qos_cap->pref_framing = GA_8_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_8_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_8_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_8_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_8_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_8_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_8_2_1_FRAMING;
                qos_conf->max_sdu = GA_8_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_8_2_1_RTN;
                qos_conf->max_transport_latency = GA_8_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_8_2_1_PRESENTDELAY_uS;
            }
            break;

        case 3:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_16_1_SF;
                csc->fd = GA_CODEC_CONF_16_1_FD;
                csc->opcf = GA_CODEC_CONF_16_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_16_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_16_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_16_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_16_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_16_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_16_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_16_1_1_FRAMING;
                qos_conf->max_sdu = GA_16_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_16_2_1_RTN;
                qos_conf->max_transport_latency = GA_16_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_16_1_1_PRESENTDELAY_uS;
            }
            break;

        case 4:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_16_2_SF;
                csc->fd = GA_CODEC_CONF_16_2_FD;
                csc->opcf = GA_CODEC_CONF_16_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_16_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_16_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_16_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_16_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_16_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_16_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_16_2_1_FRAMING;
                qos_conf->max_sdu = GA_16_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_16_2_1_RTN;
                qos_conf->max_transport_latency = GA_16_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_16_2_1_PRESENTDELAY_uS;
            }
            break;

        case 5:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_24_1_SF;
                csc->fd = GA_CODEC_CONF_24_1_FD;
                csc->opcf = GA_CODEC_CONF_24_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_24_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_24_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_24_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_24_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_24_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_24_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_24_1_1_FRAMING;
                qos_conf->max_sdu = GA_24_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_24_1_1_RTN;
                qos_conf->max_transport_latency = GA_24_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_24_1_1_PRESENTDELAY_uS;
            }
            break;

        case 6:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_24_2_SF;
                csc->fd = GA_CODEC_CONF_24_2_FD;
                csc->opcf = GA_CODEC_CONF_24_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_24_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_24_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_24_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_24_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_24_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_24_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_24_2_1_FRAMING;
                qos_conf->max_sdu = GA_24_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_24_2_1_RTN;
                qos_conf->max_transport_latency = GA_24_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_24_2_1_PRESENTDELAY_uS;
            }
            break;

        case 7:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_32_1_SF;
                csc->fd = GA_CODEC_CONF_32_1_FD;
                csc->opcf = GA_CODEC_CONF_32_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_32_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_32_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_32_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_32_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_32_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_32_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_32_1_1_FRAMING;
                qos_conf->max_sdu = GA_32_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_32_1_1_RTN;
                qos_conf->max_transport_latency = GA_32_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_32_1_1_PRESENTDELAY_uS;
            }
            break;

        case 8:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_32_2_SF;
                csc->fd = GA_CODEC_CONF_32_2_FD;
                csc->opcf = GA_CODEC_CONF_32_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_32_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_32_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_32_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_32_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_32_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_32_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_32_2_1_FRAMING;
                qos_conf->max_sdu = GA_32_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_32_2_1_RTN;
                qos_conf->max_transport_latency = GA_32_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_32_2_1_PRESENTDELAY_uS;
            }
            break;

        case 9:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_441_1_SF;
                csc->fd = GA_CODEC_CONF_441_1_FD;
                csc->opcf = GA_CODEC_CONF_441_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_441_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_441_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_441_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_441_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_441_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_441_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_441_1_1_FRAMING;
                qos_conf->max_sdu = GA_441_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_441_1_1_RTN;
                qos_conf->max_transport_latency = GA_441_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_441_1_1_PRESENTDELAY_uS;
            }
            break;

        case 10:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_441_2_SF;
                csc->fd = GA_CODEC_CONF_441_2_FD;
                csc->opcf = GA_CODEC_CONF_441_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_441_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_441_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_441_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_441_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_441_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_441_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_441_2_1_FRAMING;
                qos_conf->max_sdu = GA_441_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_441_2_1_RTN;
                qos_conf->max_transport_latency = GA_441_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_441_2_1_PRESENTDELAY_uS;
            }
            break;

        case 11:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_1_SF;
                csc->fd = GA_CODEC_CONF_48_1_FD;
                csc->opcf = GA_CODEC_CONF_48_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_1_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_1_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_1_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_1_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_1_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_1_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_1_1_FRAMING;
                qos_conf->max_sdu = GA_48_1_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_1_1_RTN;
                qos_conf->max_transport_latency = GA_48_1_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_1_1_PRESENTDELAY_uS;
            }
            break;

        case 12:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_2_SF;
                csc->fd = GA_CODEC_CONF_48_2_FD;
                csc->opcf = GA_CODEC_CONF_48_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_2_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_2_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_2_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_2_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_2_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_2_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_2_1_FRAMING;
                qos_conf->max_sdu = GA_48_2_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_2_1_RTN;
                qos_conf->max_transport_latency = GA_48_2_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_2_1_PRESENTDELAY_uS;
            }
            break;

        case 13:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_3_SF;
                csc->fd = GA_CODEC_CONF_48_3_FD;
                csc->opcf = GA_CODEC_CONF_48_3_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_3_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_3_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_3_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_3_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_3_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_3_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_3_1_FRAMING;
                qos_conf->max_sdu = GA_48_3_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_3_1_RTN;
                qos_conf->max_transport_latency = GA_48_3_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_3_1_PRESENTDELAY_uS;
            }
            break;

        case 14:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_4_SF;
                csc->fd = GA_CODEC_CONF_48_4_FD;
                csc->opcf = GA_CODEC_CONF_48_4_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_4_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_4_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_4_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_4_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_4_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_4_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_4_1_FRAMING;
                qos_conf->max_sdu = GA_48_4_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_4_1_RTN;
                qos_conf->max_transport_latency = GA_48_4_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_4_1_PRESENTDELAY_uS;
            }
            break;

        case 15:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_5_SF;
                csc->fd = GA_CODEC_CONF_48_5_FD;
                csc->opcf = GA_CODEC_CONF_48_5_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_5_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_5_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_5_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_5_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_5_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_5_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_5_1_FRAMING;
                qos_conf->max_sdu = GA_48_5_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_5_1_RTN;
                qos_conf->max_transport_latency = GA_48_5_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_5_1_PRESENTDELAY_uS;
            }
            break;

        case 16:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_6_SF;
                csc->fd = GA_CODEC_CONF_48_6_FD;
                csc->opcf = GA_CODEC_CONF_48_6_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_6_1_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_6_1_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_6_1_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_6_1_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_6_1_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_6_1_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_6_1_FRAMING;
                qos_conf->max_sdu = GA_48_6_1_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_6_1_RTN;
                qos_conf->max_transport_latency = GA_48_6_1_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_6_1_PRESENTDELAY_uS;
            }
            break;

        case 17:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_8_1_SF;
                csc->fd = GA_CODEC_CONF_8_1_FD;
                csc->opcf = GA_CODEC_CONF_8_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_8_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_8_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_8_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_8_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_8_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_8_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_8_1_2_FRAMING;
                qos_conf->max_sdu = GA_8_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_8_1_2_RTN;
                qos_conf->max_transport_latency = GA_8_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_8_1_2_PRESENTDELAY_uS;
            }
            break;

        case 18:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_8_2_SF;
                csc->fd = GA_CODEC_CONF_8_2_FD;
                csc->opcf = GA_CODEC_CONF_8_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_8_2_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_8_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_8_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_8_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_8_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_8_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_8_2_2_FRAMING;
                qos_conf->max_sdu = GA_8_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_8_2_2_RTN;
                qos_conf->max_transport_latency = GA_8_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_8_2_2_PRESENTDELAY_uS;
            }
            break;

        case 19:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_16_1_SF;
                csc->fd = GA_CODEC_CONF_16_1_FD;
                csc->opcf = GA_CODEC_CONF_16_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_16_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_16_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_16_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_16_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_16_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_16_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_16_1_2_FRAMING;
                qos_conf->max_sdu = GA_16_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_16_1_2_RTN;
                qos_conf->max_transport_latency = GA_16_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_16_1_2_PRESENTDELAY_uS;
            }
            break;

        case 20:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_16_2_SF;
                csc->fd = GA_CODEC_CONF_16_2_FD;
                csc->opcf = GA_CODEC_CONF_16_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_16_2_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_16_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_16_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_16_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_16_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_16_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_16_2_2_FRAMING;
                qos_conf->max_sdu = GA_16_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_16_2_2_RTN;
                qos_conf->max_transport_latency = GA_16_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_16_2_2_PRESENTDELAY_uS;
            }
            break;

        case 21:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_24_1_SF;
                csc->fd = GA_CODEC_CONF_24_1_FD;
                csc->opcf = GA_CODEC_CONF_24_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_24_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_24_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_24_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_24_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_24_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_24_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_24_1_2_FRAMING;
                qos_conf->max_sdu = GA_24_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_24_1_2_RTN;
                qos_conf->max_transport_latency = GA_24_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_24_1_2_PRESENTDELAY_uS;
            }
            break;

        case 22:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_24_2_SF;
                csc->fd = GA_CODEC_CONF_24_2_FD;
                csc->opcf = GA_CODEC_CONF_24_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_24_2_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_24_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_24_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_24_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_24_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_24_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_24_2_2_FRAMING;
                qos_conf->max_sdu = GA_24_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_24_2_2_RTN;
                qos_conf->max_transport_latency = GA_24_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_24_2_2_PRESENTDELAY_uS;
            }
            break;

        case 23:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_32_1_SF;
                csc->fd = GA_CODEC_CONF_32_1_FD;
                csc->opcf = GA_CODEC_CONF_32_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_32_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_32_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_32_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_32_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_32_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_cap)
            {
                qos_conf->sdu_interval = GA_32_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_32_1_2_FRAMING;
                qos_conf->max_sdu = GA_32_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_32_1_2_RTN;
                qos_conf->max_transport_latency = GA_32_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_32_1_2_PRESENTDELAY_uS;
            }
            break;

        case 24:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_32_2_SF;
                csc->fd = GA_CODEC_CONF_32_2_FD;
                csc->opcf = GA_CODEC_CONF_32_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_32_2_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_32_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_32_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_32_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_32_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_32_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_32_2_2_FRAMING;
                qos_conf->max_sdu = GA_32_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_32_2_2_RTN;
                qos_conf->max_transport_latency = GA_32_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_32_2_2_PRESENTDELAY_uS;
            }
            break;

        case 25:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_441_1_SF;
                csc->fd = GA_CODEC_CONF_441_1_FD;
                csc->opcf = GA_CODEC_CONF_441_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_441_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_441_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_441_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_441_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_441_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_441_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_441_1_2_FRAMING;
                qos_conf->max_sdu = GA_441_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_441_1_2_RTN;
                qos_conf->max_transport_latency = GA_441_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_441_1_2_PRESENTDELAY_uS;
            }
            break;

        case 26:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_441_2_SF;
                csc->fd = GA_CODEC_CONF_441_2_FD;
                csc->opcf = GA_CODEC_CONF_441_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_441_2_2_FRAMING;;
                qos_cap->pref_max_transport_latency = GA_441_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_441_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_441_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_441_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_441_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_441_2_2_FRAMING;
                qos_conf->max_sdu = GA_441_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_441_2_2_RTN;
                qos_conf->max_transport_latency = GA_441_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_441_2_2_PRESENTDELAY_uS;
            }
            break;

        case 27:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_1_SF;
                csc->fd = GA_CODEC_CONF_48_1_FD;
                csc->opcf = GA_CODEC_CONF_48_1_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_1_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_1_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_1_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_1_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_1_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_1_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_1_2_FRAMING;
                qos_conf->max_sdu = GA_48_1_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_1_2_RTN;
                qos_conf->max_transport_latency = GA_48_1_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_1_2_PRESENTDELAY_uS;
            }
            break;

        case 28:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_2_SF;
                csc->fd = GA_CODEC_CONF_48_2_FD;
                csc->opcf = GA_CODEC_CONF_48_2_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_2_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_2_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_2_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_2_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_2_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_2_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_2_2_FRAMING;
                qos_conf->max_sdu = GA_48_2_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_2_2_RTN;
                qos_conf->max_transport_latency = GA_48_2_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_2_2_PRESENTDELAY_uS;
            }
            break;

        case 29:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_3_SF;
                csc->fd = GA_CODEC_CONF_48_3_FD;
                csc->opcf = GA_CODEC_CONF_48_3_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_3_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_3_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_3_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_3_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_3_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_3_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_3_2_FRAMING;
                qos_conf->max_sdu = GA_48_3_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_3_2_RTN;
                qos_conf->max_transport_latency = GA_48_3_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_3_2_PRESENTDELAY_uS;
            }
            break;

        case 30:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_4_SF;
                csc->fd = GA_CODEC_CONF_48_4_FD;
                csc->opcf = GA_CODEC_CONF_48_4_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_4_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_4_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_4_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_4_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_4_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_4_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_4_2_FRAMING;
                qos_conf->max_sdu = GA_48_4_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_4_2_RTN;
                qos_conf->max_transport_latency = GA_48_4_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_4_2_PRESENTDELAY_uS;
            }
            break;

        case 31:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_5_SF;
                csc->fd = GA_CODEC_CONF_48_5_FD;
                csc->opcf = GA_CODEC_CONF_48_5_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_5_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_5_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_5_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_5_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_5_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_5_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_5_2_FRAMING;
                qos_conf->max_sdu = GA_48_5_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_5_2_RTN;
                qos_conf->max_transport_latency = GA_48_5_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_5_2_PRESENTDELAY_uS;
            }
            break;

        case 32:
            if (NULL != csc)
            {
                csc->sf = GA_CODEC_CONF_48_6_SF;
                csc->fd = GA_CODEC_CONF_48_6_FD;
                csc->opcf = GA_CODEC_CONF_48_6_OF;
            }
            if (NULL != qos_cap)
            {
                qos_cap->pref_framing = GA_48_6_2_FRAMING;
                qos_cap->pref_max_transport_latency = GA_48_6_2_MAXTLATENCY_mS;
                qos_cap->pref_retxn = GA_48_6_2_RTN;
                qos_cap->supp_presentation_delay_min = GA_48_6_2_PRESENTDELAY_uS;
                qos_cap->supp_presentation_delay_max = GA_48_6_2_PRESENTDELAY_uS;
            }
            if (NULL != qos_conf)
            {
                qos_conf->sdu_interval = GA_48_6_2_SDUINTERVAL_uS;
                qos_conf->framing = GA_48_6_2_FRAMING;
                qos_conf->max_sdu = GA_48_6_2_MAXSDUSIZE_BYTES;
                qos_conf->retxn = GA_48_6_2_RTN;
                qos_conf->max_transport_latency = GA_48_6_2_MAXTLATENCY_mS;
                qos_conf->presentation_delay = GA_48_6_2_PRESENTDELAY_uS;
            }
            break;

        default:
            APPL_ERR("Unknown Set !\n");
            retval = GA_FAILURE;
            break;
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_bc_prepare_csc_and_qos_conf_based_on_set_id
          (
              UINT8                        csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CONF * csc,
              GA_QOS_CONF                * qos_conf
          )
{
    GA_RESULT retval;

    if ((NULL == csc) || (NULL == qos_conf))
    {
        APPL_ERR("Invalid parameters !\n");
        return GA_FAILURE;
    }

    /* Initialize */
    retval = GA_SUCCESS;

    switch (csc_set_id)
    {
        case 1:
            csc->sf = GA_CODEC_CONF_8_1_SF;
            csc->fd = GA_CODEC_CONF_8_1_FD;
            csc->opcf = GA_CODEC_CONF_8_1_OF;

            qos_conf->sdu_interval = GA_BC_8_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_8_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_8_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_8_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_8_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_8_1_1_PRESENTDELAY_uS;
            break;

        case 2:
            csc->sf = GA_CODEC_CONF_8_2_SF;
            csc->fd = GA_CODEC_CONF_8_2_FD;
            csc->opcf = GA_CODEC_CONF_8_2_OF;

            qos_conf->sdu_interval = GA_BC_8_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_8_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_8_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_8_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_8_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_8_2_1_PRESENTDELAY_uS;
            break;

        case 3:
            csc->sf = GA_CODEC_CONF_16_1_SF;
            csc->fd = GA_CODEC_CONF_16_1_FD;
            csc->opcf = GA_CODEC_CONF_16_1_OF;

            qos_conf->sdu_interval = GA_BC_16_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_16_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_16_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_16_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_16_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_16_1_1_PRESENTDELAY_uS;
            break;

        case 4:
            csc->sf = GA_CODEC_CONF_16_2_SF;
            csc->fd = GA_CODEC_CONF_16_2_FD;
            csc->opcf = GA_CODEC_CONF_16_2_OF;

            qos_conf->sdu_interval = GA_BC_16_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_16_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_16_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_16_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_16_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_16_2_1_PRESENTDELAY_uS;
            break;

        case 5:
            csc->sf = GA_CODEC_CONF_24_1_SF;
            csc->fd = GA_CODEC_CONF_24_1_FD;
            csc->opcf = GA_CODEC_CONF_24_1_OF;

            qos_conf->sdu_interval = GA_BC_24_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_24_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_24_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_24_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_24_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_24_1_1_PRESENTDELAY_uS;
            break;

        case 6:
            csc->sf = GA_CODEC_CONF_24_2_SF;
            csc->fd = GA_CODEC_CONF_24_2_FD;
            csc->opcf = GA_CODEC_CONF_24_2_OF;

            qos_conf->sdu_interval = GA_BC_24_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_24_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_24_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_24_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_24_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_24_2_1_PRESENTDELAY_uS;
            break;

        case 7:
            csc->sf = GA_CODEC_CONF_32_1_SF;
            csc->fd = GA_CODEC_CONF_32_1_FD;
            csc->opcf = GA_CODEC_CONF_32_1_OF;

            qos_conf->sdu_interval = GA_BC_32_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_32_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_32_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_32_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_32_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_32_1_1_PRESENTDELAY_uS;
            break;

        case 8:
            csc->sf = GA_CODEC_CONF_32_2_SF;
            csc->fd = GA_CODEC_CONF_32_2_FD;
            csc->opcf = GA_CODEC_CONF_32_2_OF;

            qos_conf->sdu_interval = GA_BC_32_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_32_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_32_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_32_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_32_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_32_2_1_PRESENTDELAY_uS;
            break;

        case 9:
            csc->sf = GA_CODEC_CONF_441_1_SF;
            csc->fd = GA_CODEC_CONF_441_1_FD;
            csc->opcf = GA_CODEC_CONF_441_1_OF;

            qos_conf->sdu_interval = GA_BC_441_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_441_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_441_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_441_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_441_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_441_1_1_PRESENTDELAY_uS;
            break;

        case 10:
            csc->sf = GA_CODEC_CONF_441_2_SF;
            csc->fd = GA_CODEC_CONF_441_2_FD;
            csc->opcf = GA_CODEC_CONF_441_2_OF;

            qos_conf->sdu_interval = GA_BC_441_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_441_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_441_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_441_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_441_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_441_2_1_PRESENTDELAY_uS;
            break;

        case 11:
            csc->sf = GA_CODEC_CONF_48_1_SF;
            csc->fd = GA_CODEC_CONF_48_1_FD;
            csc->opcf = GA_CODEC_CONF_48_1_OF;

            qos_conf->sdu_interval = GA_BC_48_1_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_1_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_1_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_1_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_1_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_1_1_PRESENTDELAY_uS;
            break;

        case 12:
            csc->sf = GA_CODEC_CONF_48_2_SF;
            csc->fd = GA_CODEC_CONF_48_2_FD;
            csc->opcf = GA_CODEC_CONF_48_2_OF;

            qos_conf->sdu_interval = GA_BC_48_2_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_2_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_2_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_2_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_2_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_2_1_PRESENTDELAY_uS;
            break;

        case 13:
            csc->sf = GA_CODEC_CONF_48_3_SF;
            csc->fd = GA_CODEC_CONF_48_3_FD;
            csc->opcf = GA_CODEC_CONF_48_3_OF;

            qos_conf->sdu_interval = GA_BC_48_3_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_3_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_3_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_3_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_3_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_3_1_PRESENTDELAY_uS;
            break;

        case 14:
            csc->sf = GA_CODEC_CONF_48_4_SF;
            csc->fd = GA_CODEC_CONF_48_4_FD;
            csc->opcf = GA_CODEC_CONF_48_4_OF;

            qos_conf->sdu_interval = GA_BC_48_4_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_4_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_4_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_4_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_4_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_4_1_PRESENTDELAY_uS;
            break;

        case 15:
            csc->sf = GA_CODEC_CONF_48_5_SF;
            csc->fd = GA_CODEC_CONF_48_5_FD;
            csc->opcf = GA_CODEC_CONF_48_5_OF;

            qos_conf->sdu_interval = GA_BC_48_5_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_5_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_5_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_5_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_5_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_5_1_PRESENTDELAY_uS;
            break;

        case 16:
            csc->sf = GA_CODEC_CONF_48_6_SF;
            csc->fd = GA_CODEC_CONF_48_6_FD;
            csc->opcf = GA_CODEC_CONF_48_6_OF;

            qos_conf->sdu_interval = GA_BC_48_6_1_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_6_1_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_6_1_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_6_1_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_6_1_RTN;
            qos_conf->presentation_delay = GA_BC_48_6_1_PRESENTDELAY_uS;
            break;

        case 17:
            csc->sf = GA_CODEC_CONF_8_1_SF;
            csc->fd = GA_CODEC_CONF_8_1_FD;
            csc->opcf = GA_CODEC_CONF_8_1_OF;

            qos_conf->sdu_interval = GA_BC_8_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_8_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_8_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_8_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_8_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_8_1_2_PRESENTDELAY_uS;
            break;

        case 18:
            csc->sf = GA_CODEC_CONF_8_2_SF;
            csc->fd = GA_CODEC_CONF_8_2_FD;
            csc->opcf = GA_CODEC_CONF_8_2_OF;

            qos_conf->sdu_interval = GA_BC_8_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_8_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_8_2_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_8_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_8_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_8_2_2_PRESENTDELAY_uS;
            break;

        case 19:
            csc->sf = GA_CODEC_CONF_16_1_SF;
            csc->fd = GA_CODEC_CONF_16_1_FD;
            csc->opcf = GA_CODEC_CONF_16_1_OF;

            qos_conf->sdu_interval = GA_BC_16_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_16_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_16_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_16_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_16_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_16_1_2_PRESENTDELAY_uS;
            break;

        case 20:
            csc->sf = GA_CODEC_CONF_16_2_SF;
            csc->fd = GA_CODEC_CONF_16_2_FD;
            csc->opcf = GA_CODEC_CONF_16_2_OF;

            qos_conf->sdu_interval = GA_BC_16_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_16_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_16_2_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_16_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_16_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_16_2_2_PRESENTDELAY_uS;
            break;

        case 21:
            csc->sf = GA_CODEC_CONF_24_1_SF;
            csc->fd = GA_CODEC_CONF_24_1_FD;
            csc->opcf = GA_CODEC_CONF_24_1_OF;

            qos_conf->sdu_interval = GA_BC_24_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_24_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_24_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_24_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_24_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_24_1_2_PRESENTDELAY_uS;
            break;

        case 22:
            csc->sf = GA_CODEC_CONF_24_2_SF;
            csc->fd = GA_CODEC_CONF_24_2_FD;
            csc->opcf = GA_CODEC_CONF_24_2_OF;

            qos_conf->sdu_interval = GA_BC_24_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_24_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_24_2_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_24_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_24_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_24_2_2_PRESENTDELAY_uS;
            break;

        case 23:
            csc->sf = GA_CODEC_CONF_32_1_SF;
            csc->fd = GA_CODEC_CONF_32_1_FD;
            csc->opcf = GA_CODEC_CONF_32_1_OF;

            qos_conf->sdu_interval = GA_BC_32_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_32_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_32_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_32_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_32_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_32_1_2_PRESENTDELAY_uS;
            break;

        case 24:
            csc->sf = GA_CODEC_CONF_32_2_SF;
            csc->fd = GA_CODEC_CONF_32_2_FD;
            csc->opcf = GA_CODEC_CONF_32_2_OF;

            qos_conf->sdu_interval = GA_BC_32_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_32_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_32_2_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_32_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_32_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_32_2_2_PRESENTDELAY_uS;
            break;

        case 25:
            csc->sf = GA_CODEC_CONF_441_1_SF;
            csc->fd = GA_CODEC_CONF_441_1_FD;
            csc->opcf = GA_CODEC_CONF_441_1_OF;

            qos_conf->sdu_interval = GA_BC_441_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_441_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_441_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_441_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_441_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_441_1_2_PRESENTDELAY_uS;
            break;

        case 26:
            csc->sf = GA_CODEC_CONF_441_2_SF;
            csc->fd = GA_CODEC_CONF_441_2_FD;
            csc->opcf = GA_CODEC_CONF_441_2_OF;

            qos_conf->sdu_interval = GA_BC_441_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_441_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_441_2_2_FRAMING;;
            qos_conf->max_transport_latency = GA_BC_441_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_441_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_441_2_2_PRESENTDELAY_uS;
            break;

        case 27:
            csc->sf = GA_CODEC_CONF_48_1_SF;
            csc->fd = GA_CODEC_CONF_48_1_FD;
            csc->opcf = GA_CODEC_CONF_48_1_OF;

            qos_conf->sdu_interval = GA_BC_48_1_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_1_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_1_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_1_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_1_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_1_2_PRESENTDELAY_uS;
            break;

        case 28:
            csc->sf = GA_CODEC_CONF_48_2_SF;
            csc->fd = GA_CODEC_CONF_48_2_FD;
            csc->opcf = GA_CODEC_CONF_48_2_OF;

            qos_conf->sdu_interval = GA_BC_48_2_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_2_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_2_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_2_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_2_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_2_2_PRESENTDELAY_uS;
            break;

        case 29:
            csc->sf = GA_CODEC_CONF_48_3_SF;
            csc->fd = GA_CODEC_CONF_48_3_FD;
            csc->opcf = GA_CODEC_CONF_48_3_OF;

            qos_conf->sdu_interval = GA_BC_48_3_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_3_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_3_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_3_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_3_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_3_2_PRESENTDELAY_uS;
            break;

        case 30:
            csc->sf = GA_CODEC_CONF_48_4_SF;
            csc->fd = GA_CODEC_CONF_48_4_FD;
            csc->opcf = GA_CODEC_CONF_48_4_OF;

            qos_conf->sdu_interval = GA_BC_48_4_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_4_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_4_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_4_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_4_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_4_2_PRESENTDELAY_uS;
            break;

        case 31:
            csc->sf = GA_CODEC_CONF_48_5_SF;
            csc->fd = GA_CODEC_CONF_48_5_FD;
            csc->opcf = GA_CODEC_CONF_48_5_OF;

            qos_conf->sdu_interval = GA_BC_48_5_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_5_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_5_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_5_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_5_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_5_2_PRESENTDELAY_uS;
            break;

        case 32:
            csc->sf = GA_CODEC_CONF_48_6_SF;
            csc->fd = GA_CODEC_CONF_48_6_FD;
            csc->opcf = GA_CODEC_CONF_48_6_OF;

            qos_conf->sdu_interval = GA_BC_48_6_2_SDUINTERVAL_uS;
            qos_conf->max_sdu = GA_BC_48_6_2_MAXSDUSIZE_BYTES;
            qos_conf->framing = GA_BC_48_6_2_FRAMING;
            qos_conf->max_transport_latency = GA_BC_48_6_2_MAXTLATENCY_mS;
            qos_conf->retxn = GA_BC_48_6_2_RTN;
            qos_conf->presentation_delay = GA_BC_48_6_2_PRESENTDELAY_uS;
            break;

        default:
            APPL_ERR("Unknown Set !\n");
            retval = GA_FAILURE;
            break;
    }

    return retval;
}

void appl_ga_utils_bap_display_framing(UINT8 framing)
{
    switch(framing)
    {
        case GA_FRAMING_UNFRAMED:
            APPL_TRC("Unframed");
            break;

        case GA_FRAMING_FRAMED:
            APPL_TRC("Framed");
            break;

        default:
            APPL_TRC("RFU");
    }
    APPL_TRC(" (0x%02X)\n", framing);

    return;
}

void appl_ga_utils_bap_display_target_phy(UINT8 target_phy)
{
    if (GA_TARGET_PHY_LE_1M_PHY == target_phy)
    {
        APPL_TRC("\n\t\t- LE 1M PHY\n");
    }
    else if (GA_TARGET_PHY_LE_2M_PHY == target_phy)
    {
        APPL_TRC("\n\t\t- LE 2M PHY\n");
    }
    else if (GA_TARGET_PHY_LE_CODED_PHY == target_phy)
    {
        APPL_TRC("\n\t\t- LE Coded PHY\n");
    }
    else
    {
        APPL_TRC("\n\t\t- Unknown\n");
    }

    return;
}

void appl_ga_utils_bap_display_pref_phy(UINT8 pref_phy)
{
    if (GA_PREF_PHY_NO_PREFERENCE & pref_phy)
    {
        APPL_TRC("\n\t\t- No Preference\n");
    }
    if (GA_PREF_PHY_LE_1M_PHY & pref_phy)
    {
        APPL_TRC("\n\t\t- LE 1M PHY preferred\n");
    }
    if (GA_PREF_PHY_LE_2M_PHY & pref_phy)
    {
        APPL_TRC("\n\t\t- LE 2M PHY preferred\n");
    }
    if (GA_PREF_PHY_LE_CODED_PHY & pref_phy)
    {
        APPL_TRC("\n\t\t- LE Coded PHY preferred\n");
    }

    return;
}

void appl_ga_utils_bap_display_audio_channel_count_for_input(void)
{
    CONSOLE_OUT("Channel Count 1: 0x01 \n");
    CONSOLE_OUT("Channel Count 2: 0x02 \n");
    CONSOLE_OUT("Channel Count 3: 0x04 \n");
    CONSOLE_OUT("Channel Count 4: 0x08 \n");
    CONSOLE_OUT("Channel Count 5: 0x10 \n");
    CONSOLE_OUT("Channel Count 6: 0x20 \n");
    CONSOLE_OUT("Channel Count 7: 0x40 \n");
    CONSOLE_OUT("Channel Count 8: 0x80 \n");

    return;
}

UINT8 appl_ga_utils_bap_get_cs_cap_set_id(void)
{
    int choice;

    CONSOLE_OUT("Enter CS Cap Set ID: \n");
    appl_ga_utils_bap_display_cs_cap_set_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_cs_conf_id(void)
{
    int choice;

    CONSOLE_OUT("Enter CS Conf Set ID: \n");
    appl_ga_utils_bap_display_cs_conf_set_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_uc_get_qos_cs_cap_set_id(void)
{
    int choice;

    CONSOLE_OUT("Enter QoS Codec Capability/Configuration Setting Set ID:\n");
    appl_ga_utils_bap_uc_display_qos_cs_cap_set_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_create_lc3_codec_spec_cap_ltv
     (
         /* IN */   GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap,
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     )
{
    UINT8 marker;

    if (NULL == codec_specific_cap)
    {
        APPL_ERR("Invalid parameter !\n");
        return;
    }

    marker = 0U;

    /* Pack the Supported Sampling Frequency */
    if (0U != codec_specific_cap->supported_sf)
    {
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SSF_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SSF_TYPE;
        GA_PACK_LE_2_BYTE_VAL
        (
            &ltvarray[marker],
            codec_specific_cap->supported_sf
        );
        marker += 2U;
    }

    if (0U != codec_specific_cap->supported_fd)
    {
        /* Pack the Supported Frame Duration */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SFD_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SFD_TYPE;
        GA_PACK_LE_1_BYTE_VAL
        (
            &ltvarray[marker],
            codec_specific_cap->supported_fd
        );
        marker += 1U;
    }

    if (0U != codec_specific_cap->supported_acc)
    {
        /* Pack the Supported Audio Channel Counts */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SACC_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SACC_TYPE;
        GA_PACK_LE_1_BYTE_VAL
        (
            &ltvarray[marker],
            codec_specific_cap->supported_acc
        );
        marker += 1U;
    }

    if (0U != codec_specific_cap->supported_opcf)
    {
        /* Pack the Supported Octets per Codec Frame */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SOPCF_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SOPCF_TYPE;
        GA_PACK_LE_2_BYTE_VAL
        (
            &ltvarray[marker],
            (codec_specific_cap->supported_opcf & 0xFFFFU)
        );
        marker += 2U;
        GA_PACK_LE_2_BYTE_VAL
        (
            &ltvarray[marker],
            ((codec_specific_cap->supported_opcf & 0xFFFF0000U) >> 16U)
        );
        marker += 2U;
    }

    if (0U != codec_specific_cap->supported_mcfpSDU)
    {
        /* Pack the Supported Maximum Codec Frames per SDU */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_TYPE;
        GA_PACK_LE_1_BYTE_VAL
        (
            &ltvarray[marker],
            codec_specific_cap->supported_mcfpSDU
        );
        marker += 1U;
    }

    *ltvarray_len = marker;

    return;
}

void appl_ga_utils_bap_get_vendor_spec_codec_spec_cap_ltv
     (
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     )
{
    /* TODO: Implement for vendor specific */
    GA_IGNORE_UNUSED_PARAM(ltvarray);
    GA_IGNORE_UNUSED_PARAM(ltvarray_len);

    return;
}

void appl_ga_utils_bap_get_vendor_spec_codec_spec_conf_ltv
     (
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     )
{
    /* TODO: Implement for vendor specific */
    GA_IGNORE_UNUSED_PARAM(ltvarray);
    GA_IGNORE_UNUSED_PARAM(ltvarray_len);

    return;
}

void appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
     (
         /* IN */  GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf,
         /* OUT */ UCHAR                      * ltvarray,
         /* OUT */ UINT8                      * ltvarray_len
     )
{
    UINT8  marker;

    if (NULL == codec_spec_conf)
    {
        APPL_ERR("Invalid parameter !\n");
        return;
    }

    marker = 0U;

    if (0U != codec_spec_conf->sf)
    {
        /* Mandatory: Pack the Sampling Frequency */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_SF_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_SF_TYPE;
        GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_spec_conf->sf);
        marker += 1U;
    }

    /*
     * For 7.5ms, FD value will be 0U
     * For 10ms, FD value will be 1U
     */
    if (0xFFU != codec_spec_conf->fd)
    {
        /* Mandatory: Pack the Frame Duration */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_FD_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_FD_TYPE;
        GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_spec_conf->fd);
        marker += 1U;
    }

    if (GA_AUDIO_LOCATION_RFU_1 != codec_spec_conf->aca)
    {
        /* Optional: Pack the Audio Channel Allocation */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_ACA_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_ACA_TYPE;
        GA_PACK_LE_4_BYTE_VAL(&ltvarray[marker], codec_spec_conf->aca);
        marker += 4U;
    }

    if (0U != codec_spec_conf->opcf)
    {
        /* Mandatory: Pack the Octets per Codec Frame */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_OPCF_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_OPCF_TYPE;
        GA_PACK_LE_2_BYTE_VAL(&ltvarray[marker], codec_spec_conf->opcf);
        marker += 2U;
    }

    if (0U != codec_spec_conf->mcfpSDU)
    {
        /* Optional: Pack the Maximum Codec Frames per SDU */
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_CFBPSDU_LEN;
        ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_CFBPSDU_TYPE;
        GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_spec_conf->mcfpSDU);
        marker += 1U;
    }

    *ltvarray_len = marker;

    return;
}

void appl_ga_utils_bap_display_ctx(UINT16 contexts)
{
    if (contexts & GA_CONTEXT_TYPE_PROHIBITED)
    {
        APPL_TRC("\t- Prohibited (0x%04X)\n", GA_CONTEXT_TYPE_PROHIBITED);
    }
    if (contexts & GA_CONTEXT_TYPE_UNSPECIFIED)
    {
        APPL_TRC("\t- Unspecified (0x%04X)\n", GA_CONTEXT_TYPE_UNSPECIFIED);
    }
    if (contexts & GA_CONTEXT_TYPE_CONVERSATIONAL)
    {
        APPL_TRC
        (
            "\t- Conversational (0x%04X)\n",
            GA_CONTEXT_TYPE_CONVERSATIONAL
        );
    }
    if (contexts & GA_CONTEXT_TYPE_MEDIA)
    {
        APPL_TRC("\t- Media (0x%04X)\n", GA_CONTEXT_TYPE_MEDIA);
    }
    if (contexts & GA_CONTEXT_TYPE_GAME)
    {
        APPL_TRC("\t- Game (0x%04X)\n", GA_CONTEXT_TYPE_GAME);
    }
    if (contexts & GA_CONTEXT_TYPE_INSTRUCTIONAL)
    {
        APPL_TRC
        (
            "\t- Instructional (0x%04X)\n",
            GA_CONTEXT_TYPE_INSTRUCTIONAL
        );
    }
    if (contexts & GA_CONTEXT_TYPE_VOICE_ASSISTANTS)
    {
        APPL_TRC
        (
            "\t- Voice Assistants (0x%04X)\n",
            GA_CONTEXT_TYPE_VOICE_ASSISTANTS
        );
    }
    if (contexts & GA_CONTEXT_TYPE_LIVE)
    {
        APPL_TRC("\t- Live (0x%04X)\n", GA_CONTEXT_TYPE_LIVE);
    }
    if (contexts & GA_CONTEXT_TYPE_SOUND_EFFECTS)
    {
        APPL_TRC
        (
            "\t- Sound effects (0x%04X)\n",
            GA_CONTEXT_TYPE_SOUND_EFFECTS
        );
    }
    if (contexts & GA_CONTEXT_TYPE_NOTIFICATIONS)
    {
        APPL_TRC("\t- Notification (0x%04X)\n", GA_CONTEXT_TYPE_NOTIFICATIONS);
    }
    if (contexts & GA_CONTEXT_TYPE_RINGTONE)
    {
        APPL_TRC("\t- Ringtone (0x%04X)\n", GA_CONTEXT_TYPE_RINGTONE);
    }
    if (contexts & GA_CONTEXT_TYPE_ALERTS)
    {
        APPL_TRC("\t- Alerts (0x%04X)\n", GA_CONTEXT_TYPE_ALERTS);
    }
    if (contexts & GA_CONTEXT_TYPE_EMERGENCY_ALARM)
    {
        APPL_TRC
        (
            "\t- Emergency alarm (0x%04X)\n",
            GA_CONTEXT_TYPE_EMERGENCY_ALARM
        );
    }
    if (GA_CONTEXT_TYPE_PROHIBITED == contexts)
    {
        APPL_TRC("\t- If ASE is Src/Sink, Server is not available to "
        "transmit/receive audio for any Context Type value.(0x%04X)\n",
        GA_CONTEXT_TYPE_PROHIBITED);
    }

    return;
}

void appl_ga_utils_bap_display_audio_locations(UINT32 audio_locations)
{
    if (audio_locations & GA_AUDIO_LOCATION_NOT_ALLOWED)
    {
        APPL_TRC("\t\t- Not Allowed (0x%X)\n", GA_AUDIO_LOCATION_NOT_ALLOWED);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FL)
    {
        APPL_TRC("\t\t- Front Left (0x%X)\n", GA_AUDIO_LOCATION_FL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FR)
    {
        APPL_TRC("\t\t- Front Right (0x%X)\n", GA_AUDIO_LOCATION_FR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FC)
    {
        APPL_TRC("\t\t- Front Center (0x%X)\n", GA_AUDIO_LOCATION_FC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_LOW_FREQ_EFF_1)
    {
        APPL_TRC
        (
            "\t\t- Low Frequency Effects 1 (0x%X)\n",
            GA_AUDIO_LOCATION_LOW_FREQ_EFF_1
        );
    }
    if (audio_locations & GA_AUDIO_LOCATION_BL)
    {
        APPL_TRC("\t\t- Back Left (0x%X)\n", GA_AUDIO_LOCATION_BL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_BR)
    {
        APPL_TRC("\t\t- Back Right (0x%X)\n", GA_AUDIO_LOCATION_BR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FLC)
    {
        APPL_TRC("\t\t- Front Left of Center (0x%X)\n", GA_AUDIO_LOCATION_FLC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FRC)
    {
        APPL_TRC
        (
            "\t\t- Front Right of Center (0x%X)\n",
            GA_AUDIO_LOCATION_FRC
        );
    }
    if (audio_locations & GA_AUDIO_LOCATION_BC)
    {
        APPL_TRC("\t\t- Back Center (0x%X)\n", GA_AUDIO_LOCATION_BC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_LOW_FREQ_EFF_2)
    {
        APPL_TRC
        (
            "\t\t- Low Frequency Effects 2 (0x%X)\n",
            GA_AUDIO_LOCATION_LOW_FREQ_EFF_2
        );
    }
    if (audio_locations & GA_AUDIO_LOCATION_SL)
    {
        APPL_TRC("\t\t- Side Left (0x%X)\n", GA_AUDIO_LOCATION_SL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_SR)
    {
        APPL_TRC("\t\t- Side Right (0x%X)\n", GA_AUDIO_LOCATION_SR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TFL)
    {
        APPL_TRC("\t\t- Top Front Left (0x%X)\n", GA_AUDIO_LOCATION_TFL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TFR)
    {
        APPL_TRC("\t\t- Top Front Right (0x%X)\n", GA_AUDIO_LOCATION_TFR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TFC)
    {
        APPL_TRC("\t\t- Top Front Center (0x%X)\n", GA_AUDIO_LOCATION_TFC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TC)
    {
        APPL_TRC("\t\t- Top Center (0x%X)\n",GA_AUDIO_LOCATION_TC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TBL)
    {
        APPL_TRC("\t\t- Top Back Left (0x%X)\n", GA_AUDIO_LOCATION_TBL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TBR)
    {
        APPL_TRC("\t\t- Top Back Right (0x%X)\n", GA_AUDIO_LOCATION_TBR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TSL)
    {
        APPL_TRC("\t\t- Top Side Left (0x%X)\n", GA_AUDIO_LOCATION_TSL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TSR)
    {
        APPL_TRC("\t\t- Top Side Right (0x%X)\n", GA_AUDIO_LOCATION_TSR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_TBC)
    {
        APPL_TRC("\t\t- Top Back Center (0x%X)\n", GA_AUDIO_LOCATION_TBC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_BFC)
    {
        APPL_TRC("\t\t- Bottom Front Center (0x%X)\n", GA_AUDIO_LOCATION_BFC);
    }
    if (audio_locations & GA_AUDIO_LOCATION_BFL)
    {
        APPL_TRC("\t\t- Bottom Front Left (0x%X)\n", GA_AUDIO_LOCATION_BFL);
    }
    if (audio_locations & GA_AUDIO_LOCATION_BFR)
    {
        APPL_TRC("\t\t- Bottom Front Right (0x%X)\n", GA_AUDIO_LOCATION_BFR);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FLW)
    {
        APPL_TRC("\t\t- Front Left Wide (0x%X)\n", GA_AUDIO_LOCATION_FLW);
    }
    if (audio_locations & GA_AUDIO_LOCATION_FRW)
    {
        APPL_TRC("\t\t- Front Right Wide (0x%X)\n", GA_AUDIO_LOCATION_FRW);
    }
    if (audio_locations & GA_AUDIO_LOCATION_LS)
    {
        APPL_TRC("\t\t- Left Surround (0x%X)\n", GA_AUDIO_LOCATION_LS);
    }
    if (audio_locations & GA_AUDIO_LOCATION_RS)
    {
        APPL_TRC("\t\t- Right Surround (0x%X)\n", GA_AUDIO_LOCATION_RS);
    }
    if (audio_locations & GA_AUDIO_LOCATION_RFU_1)
    {
        APPL_TRC("\t\t- RFU 1 (0x%X)\n", GA_AUDIO_LOCATION_RFU_1);
    }
    if (audio_locations & GA_AUDIO_LOCATION_RFU_2)
    {
        APPL_TRC("\t\t- RFU 2 (0x%X)\n", GA_AUDIO_LOCATION_RFU_2);
    }
    if (audio_locations & GA_AUDIO_LOCATION_RFU_3)
    {
        APPL_TRC("\t\t- RFU 3 (0x%X)\n", GA_AUDIO_LOCATION_RFU_3);
    }
    if (audio_locations & GA_AUDIO_LOCATION_RFU_4)
    {
        APPL_TRC("\t\t- RFU 4 (0x%X)\n", GA_AUDIO_LOCATION_RFU_4);
    }

    return;
}

void appl_ga_utils_bap_display_language_codes_for_input(void)
{
    CONSOLE_OUT("1. eng (English) \n");
    CONSOLE_OUT("2. deu (German) \n");
    CONSOLE_OUT("3. spa (Spanish) \n");
    CONSOLE_OUT("4. cmn (Mandarin Chinese) \n");
    CONSOLE_OUT("5. ita (Italian) \n");
    CONSOLE_OUT("For now, only these are shown as options, "
                "However, there are many!\n");
    CONSOLE_OUT("https://en.wikipedia.org/wiki/Wikipedia:WikiProject_Languages/List_of_ISO_639-3_language_codes_(2019)\n");

    return;
}

void appl_ga_utils_set_language_code(UINT32 * lang_code, UINT8 choice)
{
    switch(choice)
    {
    case 1:
        EM_str_copy(lang_code, "eng");
        break;

    case 2:
        EM_str_copy(lang_code, "deu");
        break;

    case 3:
        EM_str_copy(lang_code, "spa");
        break;

    case 4:
        EM_str_copy(lang_code, "cmn");
        break;

    case 5:
        EM_str_copy(lang_code, "ita");
        break;

    default:
        APPL_ERR("Invalid option !");
        break;
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_metadata_ltv
     (
         /* IN */    UCHAR  * ltvarray,
         /* IN */    UINT8    ltvarray_len,
         /* INOUT */ UINT16 * pref_audio_ctx,
         /* INOUT */ UINT16 * streaming_audio_ctx
     )
{
    UINT8 marker;
    UINT8 len;
    UINT8 tag;
    UINT16 value_2b;
    UINT32 value_4b;
    UCHAR i;

    marker = 0U;
    value_2b = 0U;
    value_4b = 0U;
    i = 0U;

    if (NULL == ltvarray)
    {
        APPL_TRC("Parameters NULL !\n");
        return;
    }

    if (0U == ltvarray_len)
    {
        APPL_TRC("Metadata LTV Length = 0 !\n");
        return;
    }

    while (marker < ltvarray_len)
    {
        /* Extract the Length and the Tag */
        len = ltvarray[marker];
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Length: 0x%02X\n",
            (UCHAR)sizeof(len), len
        );
        marker++;

        tag = ltvarray[marker];
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Type: 0x%02X\n",
            (UCHAR)sizeof(tag), tag
        );
        marker++;

        APPL_TRC("\t\t- Len: 0x%02X,  Value: ", (len - (UCHAR)sizeof(tag)));

        switch (tag)
        {
            case GA_LTV_METADATA_PREF_AC_TYPE:
                if (GA_LTV_METADATA_PREF_AC_LEN == len)
                {
                    GA_UNPACK_LE_2_BYTE(&value_2b, &ltvarray[marker]);
                    marker += 2U;
                    APPL_TRC("Preferred Audio Contexts: ");
                    appl_ga_utils_bap_display_ctx(value_2b);
                    if (NULL != pref_audio_ctx)
                    {
                        *pref_audio_ctx = value_2b;
                    }
                }
                else
                {
                    APPL_ERR
                    (
                        "Preferred Audio Contexts Length Wrong: 0x%02X !\n",
                        len
                    );
                }
                break;

            case GA_LTV_METADATA_STREAMING_AC_TYPE:
                if (GA_LTV_METADATA_STREAMING_AC_LEN == len)
                {
                    GA_UNPACK_LE_2_BYTE(&value_2b, &ltvarray[marker]);
                    marker += 2U;
                    APPL_TRC("Streaming Audio Contexts: ");
                    appl_ga_utils_bap_display_ctx(value_2b);
                    if (NULL != streaming_audio_ctx)
                    {
                        *streaming_audio_ctx = value_2b;
                    }
                }
                else
                {
                    APPL_ERR
                    (
                        "Streaming Audio Contexts Length Wrong: 0x%02X !\n",
                        len
                    );
                }
                break;

            case GA_LTV_METADATA_PGM_INFO_TYPE:
                APPL_TRC("Program Info: ");
                for (i = 0U; i < (len - 1U); i++)
                {
                    APPL_TRC("%c", ltvarray[marker + i]);
                }
                APPL_TRC("\n");
                marker += (len - 1U);
                break;

            case GA_LTV_METADATA_LANGUAGE_TYPE:
                if (GA_LTV_METADATA_LANGUAGE_LEN == len)
                {
                    GA_UNPACK_LE_3_BYTE(&value_4b, &ltvarray[marker]);
                    APPL_TRC("Language: ");
                    for (i = 0U; i < (len - 1U); i++)
                    {
                        APPL_TRC("%c", ltvarray[marker + i]);
                    }
                    APPL_TRC("\n");
                    marker += 3U;
                }
                else
                {
                    APPL_ERR("Language Length Wrong: 0x%02X !\n", len);
                }
                break;

            case GA_LTV_METADATA_CCID_LIST_TYPE:
                APPL_TRC("CCID List: ");
                for (i = 0U; i < (len - 1U); i++)
                {
                    APPL_TRC("0x%X ", ltvarray[marker + i]);
                }
                APPL_TRC("\n");
                marker += (len - 1U);
                break;

            case GA_LTV_METADATA_PARENTAL_RATING_TYPE:
                if (GA_LTV_METADATA_PARENTAL_RATING_LEN == len)
                {
                    APPL_TRC("Parental Rating: 0x%06X\n", ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("Parental Rating Length Wrong: 0x%02X !\n", len);
                }
                break;

            case GA_LTV_METADATA_PROGRAM_INFO_URL_TYPE:
                APPL_TRC("Program Info URI: ");
                for (i = 0U; i < (len - 1U); i++)
                {
                    APPL_TRC("%c", ltvarray[marker + i]);
                }
                APPL_TRC("\n");
                marker += (len - 1U);
                break;

            case GA_LTV_METADATA_EM_TYPE:
                APPL_TRC("\n\tExtended Metadata:\n");
                GA_UNPACK_LE_2_BYTE(&value_2b, &ltvarray[marker]);
                marker += 2U;
                APPL_TRC("\n\tExtended Metadata Type: 0x%04X\n", value_2b);
                len -= 2U;
                appl_dump_bytes(&ltvarray[marker], (len - 1U));
                marker += (len - 1U);
                break;

            case GA_LTV_METADATA_VS_TYPE:
                APPL_TRC("\n\tVendor Specific:\n");
                GA_UNPACK_LE_2_BYTE(&value_2b, &ltvarray[marker]);
                marker += 2U;
                APPL_TRC("\n\tCompany_ID: 0x%04X\n", value_2b);
                len -= 2U;
                appl_dump_bytes(&ltvarray[marker], (len - 1U));
                marker += (len - 1U);
                break;

            default:
                APPL_ERR("\t\tUnknown Metadata Type 0x%02X !\n", tag);
                marker += (len - 1U);
                break;
        }
        APPL_TRC("\n");
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_codec_spec_cap_ltv
     (
         /* IN */   UCHAR * ltvarray,
         /* IN */   UINT8   ltvarray_len,
         /* OUT */  GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap
     )
{
    UINT8  marker;
    UINT8  len;
    UINT8  type;
    UINT16 value_2b;
    UINT32 value_4b;

    marker = 0U;

    if ((0U == ltvarray_len) ||
        (NULL == ltvarray))
    {
        APPL_ERR("Invalid Parameters !\n");
        return;
    }

    while (marker < ltvarray_len)
    {
        /* Extract the Length and the type */
        len = ltvarray[marker];
        marker++;
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Length: 0x%02X\n",
            (UCHAR)sizeof(len), len
        );

        type = ltvarray[marker];
        marker++;
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Type: 0x%02X\n",
            (UCHAR)sizeof(type), type
        );

        APPL_TRC("\t\t- Len: 0x%02X,  Value: ", (len - (UCHAR)sizeof(type)));

        switch (type)
        {
            case GA_LTV_CODEC_SPEC_CAP_SSF_TYPE:
                if (GA_LTV_CODEC_SPEC_CAP_SSF_LEN == len)
                {
                    if (NULL != codec_specific_cap)
                    {
                        GA_UNPACK_LE_2_BYTE
                        (
                            &codec_specific_cap->supported_sf,
                            &ltvarray[marker]
                        );
                    }
                    GA_UNPACK_LE_2_BYTE
                    (
                        &value_2b,
                        &ltvarray[marker]
                    );

                    APPL_TRC("Supported Sampling Frequencies: ");
                    appl_ga_utils_bap_display_ssf(value_2b);
                    marker +=2U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                }
                break;

            case GA_LTV_CODEC_SPEC_CAP_SFD_TYPE:
                if (GA_LTV_CODEC_SPEC_CAP_SFD_LEN == len)
                {
                    if (NULL != codec_specific_cap)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_specific_cap->supported_fd,
                            &ltvarray[marker]
                        );
                    }
                    APPL_TRC("Supported Frame Durations: ");
                    appl_ga_utils_bap_display_sfd(ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                }
                break;

            case GA_LTV_CODEC_SPEC_CAP_SACC_TYPE:
                if (GA_LTV_CODEC_SPEC_CAP_SACC_LEN == len)
                {
                    if (NULL != codec_specific_cap)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_specific_cap->supported_acc,
                            &ltvarray[marker]
                        );
                    }

                    APPL_TRC("Supported Audio Channel Counts: ");
                    appl_ga_utils_bap_display_sacc(ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                }
                break;

            case GA_LTV_CODEC_SPEC_CAP_SOPCF_TYPE:
                if (GA_LTV_CODEC_SPEC_CAP_SOPCF_LEN == len)
                {
                    if (NULL != codec_specific_cap)
                    {
                        GA_UNPACK_LE_4_BYTE
                        (
                            &codec_specific_cap->supported_opcf,
                            &ltvarray[marker]
                        );
                    }
                    GA_UNPACK_LE_4_BYTE
                    (
                        &value_4b,
                        &ltvarray[marker]
                    );

                    APPL_TRC
                    (
                        "Supported Octets per Codec Frame: Min(0x%04X), "
                        "Max(0x%04X)\n",
                        (value_4b & 0xFFFFU),
                        ((value_4b & 0xFFFF0000U) >> 16U)
                    );
                    marker += 4U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                }
                break;

            case GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_TYPE:
                if (GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_LEN == len)
                {
                    if (NULL != codec_specific_cap)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_specific_cap->supported_mcfpSDU,
                            &ltvarray[marker]
                        );
                    }

                    APPL_TRC("Supported Max Codec Frames Per SDU: 0x%02X\n",
                    ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length: 0x%02X !\n", type);
                }
                break;

            default:
                APPL_ERR("\tUnknown type !\n");
                marker += (len - 1U);
                break;
        }
        APPL_TRC("\n");
    }

    return;
}

void appl_ga_utils_bap_display_ssf(UINT16 ssf)
{
    APPL_TRC("0x%04X\n", ssf);

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_8000 & ssf)
    {
        APPL_TRC("\t\t\t\t8000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_11025 & ssf)
    {
        APPL_TRC("\t\t\t\t11025 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_16000 & ssf)
    {
        APPL_TRC("\t\t\t\t16000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_22050 & ssf)
    {
        APPL_TRC("\t\t\t\t22050 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_24000 & ssf)
    {
        APPL_TRC("\t\t\t\t24000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_32000 & ssf)
    {
        APPL_TRC("\t\t\t\t32000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_44100 & ssf)
    {
        APPL_TRC("\t\t\t\t44100 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000 & ssf)
    {
        APPL_TRC("\t\t\t\t48000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_88200 & ssf)
    {
        APPL_TRC("\t\t\t\t88200 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_96000 & ssf)
    {
        APPL_TRC("\t\t\t\t96000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_176400 & ssf)
    {
        APPL_TRC("\t\t\t\t176400 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_192000 & ssf)
    {
        APPL_TRC("\t\t\t\t192000 Hz\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SSF_VAL_384000 & ssf)
    {
        APPL_TRC("\t\t\t\t384000 Hz\n");
    }

    return;
}

void appl_ga_utils_bap_display_sfd(UINT8 sfd)
{
    APPL_TRC("0x%02X\n", sfd);
    if (GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS & sfd)
    {
        APPL_TRC("\t\t\t\t7.5 ms\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS & sfd)
    {
        APPL_TRC("\t\t\t\t10 ms\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS_PREF & sfd)
    {
        APPL_TRC("\t\t\t\t7.5 ms preferred\n");
    }

    if (GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS_PREF & sfd)
    {
        APPL_TRC("\t\t\t\t10 ms preferred\n");
    }

    return;
}

void appl_ga_utils_bap_display_sacc(UINT8 sacc)
{
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_1)
    {
        APPL_TRC("1");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_2)
    {
        APPL_TRC("2");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_3)
    {
        APPL_TRC("3");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_4)
    {
        APPL_TRC("4");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_5)
    {
        APPL_TRC("5");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_6)
    {
        APPL_TRC("6");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_7)
    {
        APPL_TRC("7");
    }
    if (sacc & GA_LTV_CODEC_SPEC_CAP_SACC_VAL_8)
    {
        APPL_TRC("8");
    }
    APPL_TRC(" (0x%02X)\n", sacc);

    return;
}

void appl_ga_utils_bap_parse_and_fill_codec_id
     (
         /* IN */   UCHAR         * array,
         /* IN */   UINT8           array_len,
         /* OUT */  GA_CODEC_INFO * codec_id
     )
{
    UINT8  marker;

    marker = 0U;

    if ((NULL == array) || (5U > array_len))
    {
        APPL_ERR("Invalid parameter !\n");
        return;
    }

    if (NULL != codec_id)
    {
        GA_UNPACK_LE_1_BYTE(&codec_id->coding_format, &array[marker]);
        marker += 1U;

        GA_UNPACK_LE_2_BYTE(&codec_id->company_id, &array[marker]);
        marker += 2U;

        GA_UNPACK_LE_2_BYTE(&codec_id->vendor_codec_id, &array[marker]);
        marker += 2U;

        appl_ga_utils_bap_display_codec_id(*codec_id);
    }

    return;
}

void appl_ga_utils_bap_display_codec_id(GA_CODEC_INFO codec_id)
{
    APPL_TRC
    (
        "Len: 0x%02X,  Codec ID:\n",
        (UCHAR)(sizeof(UINT8) + sizeof(UINT16) + sizeof(UINT16))
    );

    APPL_TRC
    (
        "\t\t- Len: 0x%02X,  Coding Format: ",
        (UCHAR)sizeof(codec_id.coding_format)
    );
    appl_ga_utils_bap_display_coding_format(codec_id.coding_format);

    APPL_TRC
    (
        "\t\t- Len: 0x%02X,  Company ID: (0x%04X)\n",
        (UCHAR)sizeof(codec_id.company_id), codec_id.company_id
    );

    APPL_TRC
    (
        "\t\t- Len: 0x%02X,  Vendor Specific Codec ID: (0x%04X)\n",
        (UCHAR)sizeof(codec_id.vendor_codec_id), codec_id.vendor_codec_id
    );

    return;
}

void appl_ga_utils_bap_display_coding_format(UINT8 coding_format)
{
    if (GA_AUDIO_CODING_LC3 == coding_format)
    {
        APPL_TRC("LC3 (0x%02X),\n", coding_format);
    }
    else if (GA_AUDIO_CODING_VENDOR_SPECIFIC == coding_format)
    {
        APPL_TRC("Vendor Specific (0x%02X),\n", coding_format);
    }
    else if (GA_AUDIO_CODING_INVALID == coding_format)
    {
        APPL_TRC("Invalid (0x%02X),\n", coding_format);
    }
    else
    {
        APPL_ERR("Unknown (0x%02X) !,\n", coding_format);
    }

    return;
}

void appl_ga_utils_bap_display_cap_type(UINT8 type)
{
    if (GA_CAP_TYPE_CODECS == type)
    {
        APPL_TRC("Capability Type: Codecs (0x%02X)\n", type);
    }
    else if (GA_CAP_TYPE_LOCATIONS == type)
    {
        APPL_TRC("Capability Type: Audio Locations (0x%02X)\n", type);
    }
    else if (GA_CAP_TYPE_AVAIL_CONTEXTS == type)
    {
        APPL_TRC("Capability Type: Available Contexts (0x%02X)\n", type);
    }
    else if (GA_CAP_TYPE_SUPP_CONTEXTS == type)
    {
        APPL_TRC("Capability Type: Supported Contexts (0x%02X)\n", type);
    }
    else
    {
        APPL_TRC("Capability Type: Unknown (0x%02X)\n", type);
    }

    return;
}

void appl_ga_utils_bap_display_role(UINT8 role)
{
    APPL_TRC("Role: ");
    if (GA_ROLE_SINK == role)
    {
        APPL_TRC("Sink(0x%02X)\n", role);
    }
    else if (GA_ROLE_SOURCE == role)
    {
        APPL_TRC("Source(0x%02X)\n", role);
    }
    else if ((GA_ROLE_SINK & role) && (GA_ROLE_SOURCE & role))
    {
        APPL_TRC
        (
            "Source(0x%02X) and Sink(0x%02X) - (0x%02X)\n",
            GA_ROLE_SOURCE, GA_ROLE_SINK, role
        );
    }
    else
    {
        APPL_TRC("Ignore(0x%02X)\n", role);
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
     (
         /* IN */   UCHAR                      * ltvarray,
         /* IN */   UINT8                        ltvarray_len,
         /* OUT */  GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf
     )
{
    UINT8  marker;
    UINT8  len;
    UINT8  type;
    UINT16 value_2b;
    UINT32 value_4b;

    marker = 0U;

    if ((0U == ltvarray_len) ||
        (NULL == ltvarray))
    {
        APPL_ERR("Invalid Parameters !\n");
        return;
    }

    while (marker < ltvarray_len)
    {
        /* Extract the Length and the type */
        len = ltvarray[marker];
        marker++;
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Length: 0x%02X\n",
            (UCHAR)sizeof(len), len
        );

        type = ltvarray[marker];
        marker++;
        APPL_TRC
        (
            "\t\t- Len: 0x%02X,  Type: 0x%02X\n",
            (UCHAR)sizeof(type), type
        );

        APPL_TRC("\t\t- Len: 0x%02X,  Value: ", (len - (UCHAR)sizeof(type)));

        switch (type)
        {
            case GA_LTV_CODEC_SPEC_CONF_SF_TYPE:
                if (GA_LTV_CODEC_SPEC_CONF_SF_LEN == len)
                {
                    if (NULL != codec_spec_conf)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_spec_conf->sf,
                            &ltvarray[marker]
                        );
                    }

                    APPL_TRC("Sampling Frequencies: ");
                    appl_ga_utils_bap_display_sf(ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                    /* Skip the Wrong Length Bytes to move to next */
                    marker += (len - 1U);
                }
                break;

            case GA_LTV_CODEC_SPEC_CONF_FD_TYPE:
                if (GA_LTV_CODEC_SPEC_CONF_FD_LEN == len)
                {
                    if (NULL != codec_spec_conf)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_spec_conf->fd,
                            &ltvarray[marker]
                        );
                    }
                    APPL_TRC("Frame Durations: ");
                    appl_ga_utils_bap_display_fd(ltvarray[marker]);
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                    /* Skip the Wrong Length Bytes to move to next */
                    marker += (len - 1U);
                }
                break;

            case GA_LTV_CODEC_SPEC_CONF_ACA_TYPE:
                if (GA_LTV_CODEC_SPEC_CONF_ACA_LEN == len)
                {
                    if (NULL != codec_spec_conf)
                    {
                        GA_UNPACK_LE_4_BYTE
                        (
                            &codec_spec_conf->aca,
                            &ltvarray[marker]
                        );
                    }
                    GA_UNPACK_LE_4_BYTE
                    (
                        &value_4b,
                        &ltvarray[marker]
                    );

                    APPL_TRC("Audio Channel Allocation: \n");
                    appl_ga_utils_bap_display_audio_locations(value_4b);
                    marker += 4U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                    /* Skip the Wrong Length Bytes to move to next */
                    marker += (len - 1U);
                }
                break;

            case GA_LTV_CODEC_SPEC_CONF_OPCF_TYPE:
                if (GA_LTV_CODEC_SPEC_CONF_OPCF_LEN == len)
                {
                    if (NULL != codec_spec_conf)
                    {
                        GA_UNPACK_LE_2_BYTE
                        (
                            &codec_spec_conf->opcf,
                            &ltvarray[marker]
                        );
                    }
                    GA_UNPACK_LE_2_BYTE
                    (
                        &value_2b,
                        &ltvarray[marker]
                    );

                    APPL_TRC("Octets per Codec Frame: 0x%04X\n", value_2b);
                    marker += 2U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length !\n");
                    /* Skip the Wrong Length Bytes to move to next */
                    marker += (len - 1U);
                }
                break;

            case GA_LTV_CODEC_SPEC_CONF_CFBPSDU_TYPE:
                if (GA_LTV_CODEC_SPEC_CONF_CFBPSDU_LEN == len)
                {
                    if (NULL != codec_spec_conf)
                    {
                        GA_UNPACK_LE_1_BYTE
                        (
                            &codec_spec_conf->mcfpSDU,
                            &ltvarray[marker]
                        );
                    }

                    APPL_TRC
                    (
                        "Codec Frame Blocks Per SDU: 0x%02X\n",
                        ltvarray[marker]
                    );
                    marker += 1U;
                }
                else
                {
                    APPL_ERR("\tUnexpected Length: 0x%02X !\n", type);
                    /* Skip the Wrong Length Bytes to move to next */
                    marker += (len - 1U);
                }
                break;

            default:
                APPL_ERR("\tUnknown type !\n");
                marker += (len - 1U);
                break;
        }
        APPL_TRC("\n");
    }

    return;
}

void appl_ga_utils_bap_display_fd(UINT8 fd)
{
    switch(fd)
    {
        case GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS:
            APPL_TRC("7.5 ms");
            break;

        case GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS:
            APPL_TRC("10 ms");
            break;

        default:
            APPL_TRC("RFU");
    }
    APPL_TRC(" (0x%02X)\n", fd);

    return;
}

void appl_ga_utils_bap_display_sf(UINT8 sf)
{
    switch(sf)
    {
        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_8000:
            APPL_TRC("8000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_11025:
            APPL_TRC("11025 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_16000:
            APPL_TRC("16000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_22050:
            APPL_TRC("22050 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_24000:
            APPL_TRC("24000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_32000:
            APPL_TRC("32000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_44100:
            APPL_TRC("44100 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000:
            APPL_TRC("48000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_88200:
            APPL_TRC("88200 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_96000:
            APPL_TRC("96000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_176400:
            APPL_TRC("176400 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_192000:
            APPL_TRC("192000 Hz");
            break;

        case GA_LTV_CODEC_SPEC_CONF_SF_VAL_384000:
            APPL_TRC("384000 Hz");
            break;

        default:
            APPL_TRC("Unsupported !");
    }

    APPL_TRC(" (0x%04X)\n", sf);

    return;
}

void appl_ga_utils_bap_parse_and_display_qos_cnf(GA_QOS_CONF qos_conf)
{
    APPL_TRC
    (
        "SDU Interval: 0x%06X(%dus)\n",
        qos_conf.sdu_interval,
        qos_conf.sdu_interval
    );
    APPL_TRC("Framing: ");
    appl_ga_utils_bap_display_framing
    (
        qos_conf.framing
    );
    APPL_TRC("PHY Preference: ");
    appl_ga_utils_bap_display_pref_phy
    (
        qos_conf.phy
    );
    APPL_TRC("MAX SDU: 0x%04X\n", qos_conf.max_sdu);
    APPL_TRC
    (
        "Retransmission Number: 0x%02X\n",
        qos_conf.retxn
    );
    APPL_TRC
    (
        "Max Transport Latency: 0x%04X(%dms)\n",
        qos_conf.max_transport_latency,
        qos_conf.max_transport_latency
    );
    APPL_TRC
    (
        "Presentation Delay: 0x%06X(%dus)\n",
        qos_conf.presentation_delay,
        qos_conf.presentation_delay
    );

    return;
}

UINT8 appl_ga_utils_bap_compare_codec_spec_conf
      (
          GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf_1,
          GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf_2
      )
{
    UINT8 confMatch;

    /* Initialize */
    confMatch = GA_TRUE;

    /* Match only the M fields of Codec Specific Conf */
    if (codec_spec_conf_1->sf != codec_spec_conf_2->sf)
    {
        confMatch = GA_FALSE;
    }

    if ((GA_TRUE == confMatch) &&
        (codec_spec_conf_1->fd != codec_spec_conf_2->fd))
    {
        confMatch = GA_FALSE;
    }

    if ((GA_TRUE == confMatch) &&
        (codec_spec_conf_1->opcf != codec_spec_conf_2->opcf))
    {
        confMatch = GA_FALSE;
    }

    return confMatch;
}

UINT32 appl_ga_utils_bap_fetch_sf(UINT8 sf)
{
    UINT32 sf_value;

    /* Initialize */
    sf_value = 0U;

    switch (sf)
    {
    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_8000:
        sf_value = 8000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_11025:
        sf_value = 11025U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_16000:
        sf_value = 16000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_22050:
        sf_value = 22050U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_24000:
        sf_value = 24000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_32000:
        sf_value = 32000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_44100:
        sf_value = 44100U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000:
        sf_value = 48000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_88200:
        sf_value = 88200U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_96000:
        sf_value = 96000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_176400:
        sf_value = 176400U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_192000:
        sf_value = 192000U;
        break;

    case GA_LTV_CODEC_SPEC_CONF_SF_VAL_384000:
        sf_value = 384000U;
        break;

    default:
        break;
    }

    return (sf_value);
}

UINT32 appl_ga_utils_bap_fetch_bitrate_from_qos_conf_set_id
       (
           UINT8 qos_conf_set_id
       )
{
    UINT32  bitrate;

    bitrate = 0U;

    /*
     * Note: The Bitrate as per BAP Spec,
     * Codec-Specific Configuration for Unicast and
     * Broadcast (Table 3.11 and/or Table 3.12) is defined in kbps.
     * However there are some values which are decimal.
     * Hence to avoid the decimal part, we have them
     * defined in bps by * by 1000(k).
     * Eg: For Set 8_1, Bitrate: 27.734 Kbps.
     * Bitrate in Bps: (27.734 * 1000) = 27734 Bps which is
     * GA_CODEC_CONF_8_1_BITRATE_IN_BPS.
     */
    switch(qos_conf_set_id)
    {
    /* As per Table 5.2:,
     * the QoS Configuration settings for low latency audio data and QoS
     * Configuration settings for high-reliability audio data refers to the
     * same Codec Capability / Configuration Setting(Table 3.5 and Table 3.11).
     * Eg: Set Name: 8_1_1(low latency audio data) and
     * 8_1_2(high-reliability audio data) refers to same 8_1 in
     * Codec Capability / Configuration Setting(Table 3.5 and Table 3.11).
     * Case 1: 8_1_1
     * Case 17: 8_1_2
     */
    case 1: /* Fall - through */
    case 17:
        bitrate = GA_CODEC_CONF_8_1_BITRATE_IN_BPS;
        break;

    case 2: /* Fall - through */
    case 18:
        bitrate = GA_CODEC_CONF_8_2_BITRATE_IN_BPS;
        break;

    case 3: /* Fall - through */
    case 19:
        bitrate = GA_CODEC_CONF_16_1_BITRATE_IN_BPS;
        break;

    case 4: /* Fall - through */
    case 20:
        bitrate = GA_CODEC_CONF_16_2_BITRATE_IN_BPS;
        break;

    case 5: /* Fall - through */
    case 21:
        bitrate = GA_CODEC_CONF_24_1_BITRATE_IN_BPS;
        break;

    case 6: /* Fall - through */
    case 22:
        bitrate = GA_CODEC_CONF_24_2_BITRATE_IN_BPS;
        break;

    case 7: /* Fall - through */
    case 23:
        bitrate = GA_CODEC_CONF_32_1_BITRATE_IN_BPS;
        break;

    case 8: /* Fall - through */
    case 24:
        bitrate = GA_CODEC_CONF_32_2_BITRATE_IN_BPS;
        break;

    case 9: /* Fall - through */
    case 25:
        bitrate = GA_CODEC_CONF_441_1_BITRATE_IN_BPS;
        break;

    case 10: /* Fall - through */
    case 26:
        bitrate = GA_CODEC_CONF_441_2_BITRATE_IN_BPS;
        break;

    case 11: /* Fall - through */
    case 27:
        bitrate = GA_CODEC_CONF_48_1_BITRATE_IN_BPS;
        break;

    case 12: /* Fall - through */
    case 28:
        bitrate = GA_CODEC_CONF_48_2_BITRATE_IN_BPS;
        break;

    case 13: /* Fall - through */
    case 29:
        bitrate = GA_CODEC_CONF_48_3_BITRATE_IN_BPS;
        break;

    case 14: /* Fall - through */
    case 30:
        bitrate = GA_CODEC_CONF_48_4_BITRATE_IN_BPS;
        break;

    case 15: /* Fall - through */
    case 31:
        bitrate = GA_CODEC_CONF_48_5_BITRATE_IN_BPS;
        break;

    case 16: /* Fall - through */
    case 32:
        bitrate = GA_CODEC_CONF_48_6_BITRATE_IN_BPS;
        break;

    default:
        break;
    }

    return bitrate;
}

UINT32 appl_ga_utils_bap_get_audio_channel_alloc(void)
{
    int choice;

    CONSOLE_OUT("Enter Audio Locations (0x): \n");

    appl_ga_utils_bap_display_audio_locations_for_input();
    CONSOLE_OUT
    (
        "Note: If this PACS/ASE/BIG needs to transmit more than 1 channel "
        "LC3 Frame, Compute BitMask OR and input\n"
    );
    CONSOLE_OUT("Front Left(0x00000001) and Side Left(0x00000400): 401\n");
    CONSOLE_OUT
    (
        "Note: For PACS each Location added here will mandate that there"
        "shall be atleast one ASE registered representing this Location \n"
        "OR \n"
        "There shall be a single ASE which supports all the Audio Location \n"
    );
    CONSOLE_OUT
    (
        "Note: For ASE each Location added here will mandate that the PACS has"
        "registered this Audio Location\n"
    );
    CONSOLE_OUT
    (
        "BIS will not transmit more than 1 channel LC3 Frame."
        "Do not compute BitMask OR during input\n"
    );
    CONSOLE_IN("%x", &choice);

    return ((UINT32)choice);
}

UINT8 appl_ga_utils_bap_get_target_phy(void)
{
    int choice;

    CONSOLE_OUT("Enter Target PHY:\n");
    appl_ga_utils_bap_display_target_phy_for_input();
    CONSOLE_IN("%x", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_pref_phy(void)
{
    int choice;

    CONSOLE_OUT("Enter PHY Preference:\n");
    appl_ga_utils_bap_display_pref_phy_for_input();
    CONSOLE_OUT
    (
        "Note: The server may set multiple bits in the Preferred_PHY bitfield,"
        "Compute BitMask OR and input\n"
    );
    CONSOLE_OUT
    (
        "LE 1M PHY preferred(0x01) and LE Coded PHY preferred(0x04): 0x05\n"
    );
    CONSOLE_IN("%x", &choice);

    return ((UINT8)choice);
}

UINT16 appl_ga_utils_bap_get_supp_ctx(void)
{
    UINT16 supp_cntx;

    supp_cntx = 0U;

    CONSOLE_OUT("Enter Supported Contexts (0x): \n");
    supp_cntx = appl_ga_utils_bap_get_ctx_type();

    return supp_cntx;
}

UINT16 appl_ga_utils_bap_get_available_ctx(void)
{
    UINT16 avail_cntx;

    avail_cntx = 0U;

    CONSOLE_OUT("Enter Available Contexts (0x): \n");
    avail_cntx = appl_ga_utils_bap_get_ctx_type();

    return avail_cntx;
}

UINT16 appl_ga_utils_bap_get_streaming_audio_ctx(void)
{
    UINT16 streaming_audio_ctx;

    streaming_audio_ctx = 0U;

    CONSOLE_OUT("Enter Streaming Audio Contexts (0x): \n");
    streaming_audio_ctx = appl_ga_utils_bap_get_ctx_type();

    return streaming_audio_ctx;
}

UINT16 appl_ga_utils_bap_fetch_streaming_audio_ctx(void)
{
    UINT16  context_type;
    UINT8   is_req;
    int     choice;

    choice = 0;
    context_type = 0U;
    is_req = GA_FALSE;

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Streaming Audio Contexts required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_TRUE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_req = (UINT8)choice;

    if (GA_TRUE == is_req)
    {
        /* Fetch streaming audio context */
        context_type = appl_ga_utils_bap_get_streaming_audio_ctx();
    }

    return context_type;
}

UINT16 appl_ga_utils_bap_get_ctx_type(void)
{
    int choice;

    appl_ga_utils_bap_display_ctx_type_for_input();
    CONSOLE_OUT
    (
        "Note: If the device suports more than one Context, "
        "Compute BitMask OR and input.\n"
        "Ensure that the Context added as Available is also "
        "added as Supported.\n"
    );
    CONSOLE_OUT("Media(0x0004) and Live(0x0040): 0x0044\n");
    CONSOLE_IN("%x", &choice);

    return ((UINT16)choice);
}

UINT8 appl_ga_utils_bap_get_parental_rating_type(void)
{
    int choice;

    CONSOLE_OUT("NOTE: Bits 0 - 3 Value representing the parental rating, "
                "where values other than 0 and 1 recommend for listeners "
                "of age Y years, Y = value + 3 years\n");
    CONSOLE_OUT("Eg.  0x05 = recommended for listeners of 8 years or older\n");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

int appl_ga_utils_bap_get_language_code(void)
{
    int choice;

    CONSOLE_OUT("Enter Language code:\n");
    appl_ga_utils_bap_display_language_codes_for_input();
    CONSOLE_OUT("NOTE: For now, only these are shown as options, "
                "However, there are many!\n");
    CONSOLE_OUT("https://en.wikipedia.org/wiki/Wikipedia:WikiProject_Languages/List_of_ISO_639-3_language_codes_(2019)\n");
    CONSOLE_IN("%d", &choice);

    return choice;
}

void appl_ga_utils_bap_get_input_and_create_metadata_ltv
     (
         /* OUT */ UCHAR                      * ltvarray,
         /* OUT */ UINT8                      * ltvarray_len
     )
{
    UINT8   marker;
    UINT32  value_4b;
    int     choice;
    UINT8   is_this_req;
    UINT8   value_2b;
    UINT16  context_type;
    UINT8   data_len;
    UINT8   index;
    UCHAR   data[15U] = "EtherMind";

    marker = 0U;
    value_4b = 0U;
    value_2b = 0U;
    is_this_req = 0U;
    context_type = 0U;
    data_len = 0U;
    index = 0U;

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    GA_mem_set(data, 0U, 15U);
    data_len = 0U;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Preferred Audio Contexts required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;

    if (GA_TRUE == is_this_req)
    {
        ltvarray[marker++] = GA_LTV_METADATA_PREF_AC_LEN;
        ltvarray[marker++] = GA_LTV_METADATA_PREF_AC_TYPE;
        context_type = appl_ga_utils_bap_get_supp_ctx();
        GA_PACK_LE_2_BYTE_VAL
        (
            &ltvarray[marker],
            context_type
        );
        marker += 2U;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Streaming Audio Contexts required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_TRUE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;

    if (GA_TRUE == is_this_req)
    {
        ltvarray[marker++] = GA_LTV_METADATA_STREAMING_AC_LEN;
        ltvarray[marker++] = GA_LTV_METADATA_STREAMING_AC_TYPE;
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        context_type = appl_ga_utils_bap_get_streaming_audio_ctx();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        context_type = GA_CONTEXT_TYPE_MEDIA;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        GA_PACK_LE_2_BYTE_VAL
        (
            &ltvarray[marker],
            context_type
        );
        marker += 2U;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Program Info required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_TRUE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;
    if (GA_TRUE == is_this_req)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        appl_ga_utils_bap_get_program_info(data, &data_len);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* Using data initialized duirng declaration */
        data_len = 9U;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /*
         * Program_Info Length - Varies GA_LTV_METADATA_PGM_INFO_LEN
         * 1Byte for Type + Remaining for Program_Info
         */
        ltvarray[marker++] = data_len + 1U;
        ltvarray[marker++] = GA_LTV_METADATA_PGM_INFO_TYPE;
        BT_PACK_LE_N_BYTE
        (
            &ltvarray[marker],
            &data[0U],
            data_len
        );
        marker += data_len;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Language code required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_TRUE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;

    if (GA_TRUE == is_this_req)
    {
        ltvarray[marker++] = GA_LTV_METADATA_LANGUAGE_LEN;
        ltvarray[marker++] = GA_LTV_METADATA_LANGUAGE_TYPE;
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        choice = appl_ga_utils_bap_get_language_code();
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        choice = 1U; /* eng */
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        appl_ga_utils_set_language_code(&value_4b, (UINT8 )choice);
        GA_PACK_LE_3_BYTE_VAL
        (
            &ltvarray[marker],
            value_4b
        );
        marker += 3U;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is CCID List required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;
    if (GA_TRUE == is_this_req)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        data_len = appl_ga_utils_bap_get_ccid_count();
        for (index = 0U; index < data_len; index++)
        {
            data[index] = appl_ga_utils_bap_get_ccid();
        }
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* By default, assign 1 CCID with the value 1 for the CCID_List */
        data_len = 1U;
        data[0U] = 1U;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* CCID List Length - Varies GA_LTV_METADATA_CCID_LIST_LEN */
        /* data_len(Varies) -> CCID Length + 1U(Fixed) -> Type Length */
        ltvarray[marker++] = data_len + 1U;
        ltvarray[marker++] = GA_LTV_METADATA_CCID_LIST_TYPE;
        BT_PACK_LE_N_BYTE
        (
            &ltvarray[marker],
            &data[0U],
            data_len
        );
        marker += data_len;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Parental Rating Type required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;

    if (GA_TRUE == is_this_req)
    {
        ltvarray[marker++] = GA_LTV_METADATA_PARENTAL_RATING_LEN;
        ltvarray[marker++] = GA_LTV_METADATA_PARENTAL_RATING_TYPE;
        value_2b = appl_ga_utils_bap_get_parental_rating_type();
        ltvarray[marker++] = value_2b;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Program Info URL required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;
    if (GA_TRUE == is_this_req)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        appl_ga_utils_bap_get_program_info_url(data, &data_len);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* TODO: Fetch default values */
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* Program_Info_URL Length - Varies GA_LTV_METADATA_PGM_INFO_URL_LEN */
        /* data_len(Varies) -> Program_Info_URL Length + 1U(Fixed) -> Type Length */
        ltvarray[marker++] = data_len + 1U;
        ltvarray[marker++] = GA_LTV_METADATA_PROGRAM_INFO_URL_TYPE;
        BT_PACK_LE_N_BYTE
        (
            &ltvarray[marker],
            &data[0U],
            data_len
        );
        marker += data_len;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Extended Metadata required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;
    if (GA_TRUE == is_this_req)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        appl_ga_utils_bap_get_extended_metadata(data, &data_len);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* TODO: Fetch default values */
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* Extended_Metadata Length - Varies GA_LTV_METADATA_EM_LEN */
        /* data_len(Varies) -> Extended_Metadata Length + 1U(Fixed) -> Type Length */
        ltvarray[marker++] = data_len + 1U;
        ltvarray[marker++] = GA_LTV_METADATA_EM_TYPE;
        BT_PACK_LE_N_BYTE
        (
            &ltvarray[marker],
            &data[0U],
            data_len
        );
        marker += data_len;
    }

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    CONSOLE_OUT
    (
        "Is Vendor Specific required?\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    choice = GA_FALSE;
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    is_this_req = (UINT8)choice;
    if (GA_TRUE == is_this_req)
    {
#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
        appl_ga_utils_bap_get_vendor_specific(data, &data_len);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* TODO: Fetch default values */
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
        /* Vendor_Specific Length - Varies GA_LTV_METADATA_VS_LEN */
        /* data_len(Varies) -> Vendor_Specific Length Length + 1U(Fixed) -> Type Length */
        ltvarray[marker++] = data_len + 1U;
        ltvarray[marker++] = GA_LTV_METADATA_VS_TYPE;
        BT_PACK_LE_N_BYTE
        (
            &ltvarray[marker],
            &data[0U],
            data_len
        );
        marker += data_len;
    }

    *ltvarray_len = marker;

    return;
}

void appl_ga_utils_bap_get_vendor_specific(UCHAR * data, UINT8 * datalen)
{
    int    choice;
    UINT8  marker;

    /* Initialize */
    marker = 0U;

    CONSOLE_OUT("Enter Vendor Specific datalength:\n");
    CONSOLE_OUT("NOTE: Max length for Vendor Specific is 15\n");
    CONSOLE_IN("%d", &choice);
    *datalen = (UINT8)choice;

    appl_ga_utils_bap_display_vendor_specific();
    CONSOLE_OUT("Enter Company_ID: ");
    CONSOLE_IN("%d", &choice);
    GA_PACK_LE_2_BYTE_VAL(&data[marker], (UINT16)choice);
    marker += 2U;
    CONSOLE_OUT("Enter Vendor Specific value:\n");

    /* marker shall start from 2U, Append it to already added data[marker] */
    for (; marker < *datalen; marker++)
    {
        CONSOLE_IN("%d", &choice);
        data[marker] = (UINT8)choice;
    }

    return;
}

void appl_ga_utils_bap_display_vendor_specific(void)
{
    CONSOLE_OUT
    (
        "NOTE: Octet 0 - 1 = Company_ID\n"
        "      Octet 2 - 254 = Vendor-Specific Metadata\n"
    );

    return;
}

void appl_ga_utils_bap_get_extended_metadata(UCHAR * data, UINT8 * datalen)
{
    int    choice;
    UINT8  marker;

    /* Initialize */
    marker = 0U;

    CONSOLE_OUT("Enter Extended Metadata datalength:\n");
    CONSOLE_OUT("NOTE: Max length for Extended Metadata is 15\n");
    appl_ga_utils_bap_display_extended_metadata();
    CONSOLE_IN("%d", &choice);
    *datalen = (UINT8)choice;

    appl_ga_utils_bap_display_extended_metadata();
    CONSOLE_OUT("Enter Extended Metadata Type: ");
    CONSOLE_IN("%d", &choice);
    GA_PACK_LE_2_BYTE_VAL(&data[marker], (UINT16)choice);
    marker += 2U;
    CONSOLE_OUT("Enter Extended Metadata value:\n");

    /* marker shall start from 2U, Append it to already added data[marker] */
    for (; marker < *datalen; marker++)
    {
        CONSOLE_IN("%d", &choice);
        data[marker] = (UINT8)choice;
    }

    return;
}

void appl_ga_utils_bap_display_extended_metadata(void)
{
    CONSOLE_OUT
    (
        "NOTE: Octet 0 - 1 = Extended Metadata Type\n"
        "      Octet 2 - 254. = Extended Metadata\n"
    );

    return;
}

void appl_ga_utils_bap_get_program_info_url(UCHAR * data, UINT8 * datalen)
{
    int    choice;
    UCHAR  char_input;

    GA_IGNORE_UNUSED_PARAM(data);

    CONSOLE_OUT("Enter Program Info URL datalength:\n");
    CONSOLE_OUT("NOTE: Max length for Program Info URL is 15\n");
    CONSOLE_IN("%d", &choice);
    *datalen = (UINT8)choice;

    CONSOLE_OUT("Enter Program Info value:\n");
    appl_ga_utils_bap_display_program_info_url();

    /* TODO */
    return;
}

void appl_ga_utils_bap_display_program_info_url(void)
{
    CONSOLE_OUT
    (
        "NOTE: A UTF-8 formatted URL link used to present more "
        "information about Program_Info\n"
    );

    return;
}

void appl_ga_utils_bap_get_program_info(UCHAR * data, UINT8 * datalen)
{
    int    choice;
    UINT8  marker;

    CONSOLE_OUT("Enter Program Info datalength :\n");
    CONSOLE_OUT("NOTE: Max length for Program Info is 15\n");
    CONSOLE_IN("%d", &choice);
    *datalen = (UINT8)choice;

    CONSOLE_OUT("Enter Program Info value:\n");
    appl_ga_utils_bap_display_program_info();
    CONSOLE_IN("%s", data);

    return;
}

void appl_ga_utils_bap_display_program_info(void)
{
    CONSOLE_OUT
    (
        "NOTE: Program Info is a title and/or summary of "
        "Audio Stream content: UTF-8 format\n"
    );

    return;
}

void appl_ga_utils_bap_display_packing(UINT8 packing)
{
    if (0x00U == packing)
    {
        APPL_TRC("Sequential");
    }
    else if (0x01U == packing)
    {
        APPL_TRC("Interleaved");
    }
    else
    {
        APPL_TRC("Unknown Packing value");
    }
    APPL_TRC(" (0x%02X)\n", packing);

    return;
}

UINT8 appl_ga_utils_bap_get_codec_frame_block_per_sdu(void)
{
    int choice;

    CONSOLE_OUT("Enter Codec Frame Blocks Per SDU:\n");
    CONSOLE_OUT
    (
        "NOTE: The number of blocks in an LC3 Media Packet Payload field is "
        "defined by the Codec_Frame_Blocks_Per_SDU\n"
    );
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_bc_get_qos_cs_cap_set_id(void)
{
    int choice;

    CONSOLE_OUT("Enter Broadcast Audio Stream configuration Set ID:\n");
    appl_ga_utils_bap_bc_display_qos_cs_cap_set_for_input();
    CONSOLE_OUT
    (
        "This is the mandatory Broadcast Audio Stream configuration support "
        "settings defined for the BC Source (encoding and transmission "
        "of the BC Audio Stream) and the BC Sink (reception and decoding "
        "of the BC Audio Stream).\n"
    );
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
     (
         /* OUT */ GA_CODEC_INFO * codec_info
     )
{
    codec_info->coding_format = GA_AUDIO_CODING_LC3;
    codec_info->company_id = 0x0000U;
    codec_info->vendor_codec_id = 0x0000U;

    GA_mem_set
    (
        &codec_info->ie[0U],
        0U,
        GA_CODEC_IE_LEN
    );

    return;
}

void appl_ga_utils_bap_get_codec_id_for_vendor_spec
     (
         /* OUT */ GA_CODEC_INFO * codec_info
     )
{
    int  choice;

    codec_info->coding_format = GA_AUDIO_CODING_VENDOR_SPECIFIC;
    /*
     * Company ID and Vendor ID: Valid Octects.
     * Refer Bluetooth Assigned Numbers for Company ID.
     */
    CONSOLE_OUT("Enter Company ID(Hex): ");
    CONSOLE_IN("%d", &choice);
    codec_info->company_id = (UINT16)choice;

    CONSOLE_OUT("Enter Vendor Specific Codec ID(Hex): ");
    CONSOLE_IN("%d", &choice);
    codec_info->vendor_codec_id = (UINT16)choice;

    GA_mem_set
    (
        &codec_info->ie[0U],
        0U,
        GA_CODEC_IE_LEN
    );

    return;
}

UINT8 appl_ga_utils_bap_get_audio_channel_count(void)
{
    int  choice;

    choice = 0;

    CONSOLE_OUT("Enter Supported Audio Channel Count (0x):\n");
    appl_ga_utils_bap_display_audio_channel_count_for_input();

    CONSOLE_OUT
    (
        "Note: If the Server supports > 1 channel count, "
        "Compute BitMask OR and input\n"
    );
    CONSOLE_OUT("Channel Count: 1(0x01) and Channel Count: 2(0x02): 0x03\n");
    CONSOLE_OUT
    (
        "Note: PACS shall be registered with the 'Channel Count' number "
        "of Audio Locations\n"
    );
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_max_codec_frames_per_sdu(void)
{
    int    choice;

    choice = 0;

    CONSOLE_OUT("Enter Max Codec Frames Per SDU (0x):\n");

    CONSOLE_OUT
    (
        "NOTE: The absence of the Supported_Max_Codec_Frames_Per_SDU LTV "
        "structure shall be interpreted as equivalent to a "
        "Supported_Max_Codec_Frames_Per_SDU value of 1 codec frame per "
        "Audio Channel per SDU maximum\n"
    );

    CONSOLE_OUT
    (
        "Is Max Codec Frames Per SDU required:\n"
        "0. No 1. Yes\n"
    );
    CONSOLE_IN("%d", &choice);

    if (GA_TRUE == (UINT8)choice)
    {
        CONSOLE_OUT("Enter the value: ");
        CONSOLE_IN("%d", &choice);
    }

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_pref_presentation_delay(void)
{
    int choice;

    CONSOLE_OUT("Enter Pref Presentation Delay Min/Max in us (0x):\n");

    CONSOLE_OUT
    (
        "NOTE: Pref Presentaion Delay Min:"
               "If nonzero, shall be >= Presentation_Delay_Min.\n"
        "       Pref Presentaion Delay Max:"
               "If nonzero, shall be <= Presentation_Delay_Max.\n"
        "A value of 0x000000 indicates no preference.\n"
    );

    CONSOLE_OUT("TMAP Role: BMR, M: 20ms to 40ms\n");

    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_choice_for_update_notify(void)
{
    int choice;

    CONSOLE_OUT("Enter Role: 1 - Update, 2 - Notify, 3 - Update and Notify:\n");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_role(void)
{
    int choice;

    CONSOLE_OUT("Enter Role: 1 - Sink, 2 - Source:\n");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_pacs_cap_count(void)
{
    int choice;

    CONSOLE_OUT("Enter Capabilities Count: ");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_pacs_cap_set_count(void)
{
    int choice;

    CONSOLE_OUT("Enter Set Count: ");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_ase_count(void)
{
    int choice;

    CONSOLE_OUT("Enter ASE Count: ");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_ccid_count(void)
{
    int choice;

    CONSOLE_OUT("Enter CCID Count:\n");
    CONSOLE_OUT("NOTE: Max Limit for CCID count is 15\n");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_ccid(void)
{
    int choice;

    CONSOLE_OUT("Enter CCID:\n");
    appl_ga_utils_display_ccid();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_display_ccid(void)
{
    CONSOLE_OUT
    (
        "The CCID_List LTV structure is used as part of the information "
        "supplied by the Initiator in the Metadata field associated with the "
        "Audio Streams during the Setting Context Type and CCID values "
        "Metadata step of the Unicast/Broadcast Audio Start procedure "
        "(see Section 7.3.1.2.6/Section 7.3.1.3.2) or the Updating "
        "Context Type and CCID Value Metadata of the Unicast/Broadcast Audio"
        " Update procedure (see Section 7.3.1.3.2/Section 7.3.1.6.1).\n"
        "Refer CAP Spec V 1.0\n"
    );

    return;
}

void appl_ga_utils_bap_display_adv_set_identifier(void)
{
    CONSOLE_OUT("Advertising_Handle: 0x00 - 0xEF\n");
    return;
}

UINT8 appl_ga_utils_bap_get_adv_set_identifier(void)
{
    int choice;

    CONSOLE_OUT("Enter Advertising_Handle:\n");
    appl_ga_utils_bap_display_adv_set_identifier();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_display_packing_for_input(void)
{
    CONSOLE_OUT("Packing: Sequential - 0x00\n");
    CONSOLE_OUT("Packing: Interleaved - 0x01\n");

    return;
}

UINT8 appl_ga_utils_bap_get_packing(void)
{
    int choice;

    CONSOLE_OUT("Enter Packing:\n");
    appl_ga_utils_bap_display_packing_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_display_sca_for_input(void)
{
    CONSOLE_OUT("251 ppm to 500 ppm: 0x00 \n");
    CONSOLE_OUT("151 ppm to 250 ppm: 0x01 \n");
    CONSOLE_OUT("101 ppm to 150 ppm: 0x02 \n");
    CONSOLE_OUT("76 ppm to 100 ppm : 0x03 \n");
    CONSOLE_OUT("51 ppm to 75 ppm  : 0x04 \n");
    CONSOLE_OUT("31 ppm to 50 ppm  : 0x05 \n");
    CONSOLE_OUT("21 ppm to 30 ppm  : 0x06 \n");
    CONSOLE_OUT("0 ppm to 20 ppm   : 0x07 \n");

    return;
}

UINT8 appl_ga_utils_bap_get_sca(void)
{
    int choice;

    CONSOLE_OUT("Enter SCA:\n");
    appl_ga_utils_bap_display_sca_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

/*
 * Appearance Category: 0x022
 *            Value Range: 0x0880 to 0x08BF
 *            Name: Audio Source
 * As per Appearance Values spec in Assigned Numbers
 */
void appl_ga_utils_bap_display_audio_src_appearance_value_for_input(void)
{
    CONSOLE_OUT("Category(bits 15 to 6)      Subcatgeory(bits 5 to 0)      Value      Name\n");
    CONSOLE_OUT("     0x022                         0x00                   0x0880     Generic Audio Source\n");
    CONSOLE_OUT("     0x022                         0x01                   0x0881     Microphone\n");
    CONSOLE_OUT("     0x022                         0x02                   0x0882     Alarm\n");
    CONSOLE_OUT("     0x022                         0x03                   0x0883     Bell\n");
    CONSOLE_OUT("     0x022                         0x04                   0x0884     Horn\n");
    CONSOLE_OUT("     0x022                         0x05                   0x0885     Broadcasting Device\n");
    CONSOLE_OUT("     0x022                         0x06                   0x0886     Service Desk\n");
    CONSOLE_OUT("     0x022                         0x07                   0x0887     Kiosk\n");
    CONSOLE_OUT("     0x022                         0x08                   0x0888     Broadcasting Room\n");
    CONSOLE_OUT("     0x022                         0x09                   0x0889     Auditorium\n");

    return;
}

UINT16 appl_ga_utils_bap_get_audio_src_appearance_value(void)
{
    int choice;

    CONSOLE_OUT("Enter Audio Source Appearance Value as per \"Value\" field [IN HEX]:\n");
    appl_ga_utils_bap_display_audio_src_appearance_value_for_input();
    CONSOLE_IN("%x", &choice);

    return ((UINT16)choice);
}

#ifdef GA_PBP
void appl_ga_utils_bap_display_pbp_audio_conf_type_for_input(void)
{
    CONSOLE_OUT("1. Standard Quality\n");
    CONSOLE_OUT("2. High Quality\n");

    return;
}

UINT8 appl_ga_utils_bap_get_pbp_audio_conf_type(void)
{
    int choice;

    /*
     * Standard Quality Public Broadcast Audio in this specification means
     * broadcast Audio Streams configured with a broadcast Audio Stream
     * configuration setting defined as Mandatory for a Broadcast Sink in
     * Table 6.4 in BAP spec.
     * 16_2_1, 16_2_2
     * 24_2_1, 24_2_2
     * High Quality Public Broadcast Audio: Table 4.2 in PBP Spec
     * 48_1_1, 48_1_2
     * 48_2_1, 48_2_2
     * 48_3_1, 48_3_2
     * 48_4_1, 48_4_2
     * 48_5_1, 48_5_2
     * 48_6_1, 48_6_2
     */
    CONSOLE_OUT("Enter Public Broadcast Audio Configuration Type:\n");
    appl_ga_utils_bap_display_pbp_audio_conf_type_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}
#endif /* GA_PBP */

GA_RESULT appl_ga_register_pacs(APPL_GA_ROLE role)
{
    GA_RESULT retval;

    /* Intitialize */
    retval = GA_FAILURE;

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    GA_IGNORE_UNUSED_PARAM(role);
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

#ifdef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
    if (APPL_GA_TMAP_ROLE_UMR ==
            (APPL_GA_TMAP_ROLE_UMR & role))
    {
        retval = appl_ga_utils_bap_register_pacs_using_default_values
                 (
                     role, GA_ROLE_SINK
                 );
    }
    else
    {
        /*
         * Registering PACS for both Source and Sink role
         * Comment out the PACS registering of Role when not required.
         * Eg: Scan Delegator, PACS registeration for Source role
         * can be excluded.
         */
        retval = appl_ga_utils_bap_register_pacs_using_default_values
                 (
                     role, GA_ROLE_SINK
                 );
        retval = appl_ga_utils_bap_register_pacs_using_default_values
                 (
                     role, GA_ROLE_SOURCE
                 );
    }
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    retval = appl_ga_utils_bap_register_pacs_using_runtime_values();
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
    if (GA_SUCCESS != retval)
    {
        APPL_DBG("Failed to register PACS, 0x%04X !", retval);
    }

    return retval;
}

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
GA_RESULT appl_ga_utils_bap_register_pacs_using_runtime_values(void)
{
    GA_RESULT      retval;
    int            choice;
    UINT8          index;
    UINT8          j;

    /* PACS: Role */
    UINT8          role;
    GA_AUDIO_CAP * cap;
    /* Total Capability Count Present */
    UINT8          cap_count;
    /* Total Capability Count added during this trial */
    UINT8          count;
    UINT8          reg_cap_id[MAX_AUDIO_CAP];

    /* PACS: Sink PAC/Source PAC */
    UINT8          csc_set_count;
    UINT8          csc_set_id;
    GA_LC3_CODEC_SPECIFIC_CAP csc = { 0 };

    /* PACS: Locations, Avaliable and Supported Audio Contexts */
    UINT16         context_type;
    UINT32         audio_loc;

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    role = 0U;
    index = 0U;
    j = 0U;
    cap_count = 0U;
    count = 0U;
    csc_set_count = 0U;
    csc_set_id = 0U;
    context_type = 0U;
    audio_loc = 0U;

    /* Input the Role for the PACS */
    role = appl_ga_utils_bap_get_role();

    /*
     * This refers to total SINK/SOURCE PACS Char Count,
     * Number of Capabilty Count equals Number of Source/Sink Char Instance.
     */
    count = appl_ga_utils_bap_get_pacs_cap_count();

    /* Fetch the previously added Count, Continue from there */
    if (GA_ROLE_SOURCE == role)
    {
        cap_count = local_pacs.src_cap_count;
        cap = &local_pacs.src_cap[cap_count].cap;
    }
    else if (GA_ROLE_SINK == role)
    {
        cap_count = local_pacs.snk_cap_count;
        cap = &local_pacs.snk_cap[cap_count].cap;
    }
    else
    {
        APPL_INF("Invalid GA Role: 0x%02X\n", role);
        return retval;
    }

    for (index = 0U; index < count; index++)
    {
        /* Fetch the Number of Sets for this single PAC Record */
        csc_set_count = appl_ga_utils_bap_get_pacs_cap_set_count();

        /* This implies adding of Multiple Sets in a single PACS record */
        for (j = 0U; j < csc_set_count; j++)
        {
            /*
             * Setting of Audio Channel Count: Optional field in Codec
             * Spec Cap.
             */
            csc.supported_acc |= appl_ga_utils_bap_get_audio_channel_count();

            /*
             * Setting of Max Codec Frames Per SDU: Optional field in Codec
             * Spec Cap.
             */
            csc.supported_mcfpSDU =
                appl_ga_utils_bap_get_max_codec_frames_per_sdu();

            /* Fetch the Codec Specfic Capability Set ID */
            csc_set_id = appl_ga_utils_bap_get_cs_cap_set_id();

            /* Prepare Codec Specific Capability */
            if (GA_SUCCESS == appl_ga_utils_bap_prepare_cs_cap_based_on_set_id
                              (
                                  csc_set_id, &csc
                              ))
            {
                /*
                 * If one of the Set is chosen as defined in BAP Spec, the
                 * fields are mandated as below as per Spec.
                 * Coding Format: LC3 [As per BAP]
                 * Company ID and Vendor ID: 0x0000U [As per PACS]
                 */
                appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
                (
                    &cap->codec
                );

                /* Prepare the Codec Spec Capability LTVs for LC3 */
                appl_ga_utils_bap_create_lc3_codec_spec_cap_ltv
                (
                    &csc,
                    cap->codec.ie,
                    &cap->codec.ie_len
                );

                /* Update this to be used later for comparison */
                if (GA_ROLE_SOURCE == role)
                {
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_sf |=
                        csc.supported_sf;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_fd |=
                        csc.supported_fd;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_acc |=
                        csc.supported_acc;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_opcf =
                        csc.supported_opcf;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_mcfpSDU =
                        csc.supported_mcfpSDU;
                }
                else if (GA_ROLE_SINK == role)
                {
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_sf |=
                        csc.supported_sf;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_fd |=
                        csc.supported_fd;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_acc |=
                        csc.supported_acc;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_opcf =
                        csc.supported_opcf;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_mcfpSDU =
                        csc.supported_mcfpSDU;
                }
            }
            else
            {
                /*
                 * Chosen a set not defined in BAP Spec, Shall be vendor
                 * specific.
                 */
                appl_ga_utils_bap_get_codec_id_for_vendor_spec(&cap->codec);

                /* Fetch the Codec Spec Capability LTVs for Vendor Specific */
                appl_ga_utils_bap_get_vendor_spec_codec_spec_cap_ltv
                (
                    cap->codec.ie,
                    &cap->codec.ie_len
                );
            }
        }

        /* Increment the Record Count */
        cap_count++;
        cap++;
    }

    /*
     * Point to the First Capability Record added during this trial.
     * cap_count - Total Capability Record count.
     * index - Total Capability Record count added during this trial.
     */
    if (GA_ROLE_SOURCE == role)
    {
        cap = &local_pacs.src_cap[cap_count - index];
    }
    else if (GA_ROLE_SINK == role)
    {
        cap = &local_pacs.snk_cap[cap_count - index];
    }

    APPL_TRC("GA_register_audio_capabilities: \n");
    retval = GA_register_audio_capabilities
             (
                 role,
                 count,
                 cap,
                 reg_cap_id
             );
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    if (GA_SUCCESS == retval)
    {
        for (j = 0U; j < count; j++)
        {
            if (GA_ROLE_SINK == role)
            {
                /* Adding Sink Capability */
                local_pacs.snk_cap[cap_count - index].cap_record_id = reg_cap_id[j];
                APPL_TRC("Registered Capability Record ID: %d\n", reg_cap_id[j]);
                index++;
            }
            else
            {
                /* Adding Source Capability */
                local_pacs.src_cap[cap_count - index].cap_record_id = reg_cap_id[j];
                APPL_TRC("Registered Capability Record ID: %d\n", reg_cap_id[j]);
                index++;
            }
        }

        if (GA_ROLE_SINK == role)
        {
            /* Adding Sink Capability */
            context_type = appl_ga_utils_bap_get_supp_ctx();
            local_pacs.record.supp_sink_contexts |= context_type;

            context_type = appl_ga_utils_bap_get_available_ctx();
            local_pacs.record.avail_sink_contexts |= context_type;

            audio_loc = appl_ga_utils_bap_get_audio_channel_alloc();
            local_pacs.record.sink_audio_locations |= audio_loc;

            /*
             * When exposing support for the LC3 codec in a PAC record,
             * the Preferred_Audio_Context LTV structure defined in Bluetooth
             * Assigned Numbers may be present in the Metadata field.
             * The absence of the Preferred_Audio_Contexts LTV structure in a
             * PAC record means no preference for audio data Context Types are
             * defined in that PAC record.
             */
            local_pacs.snk_cap[local_pacs.snk_cap_count].cap.pref_audio_contexts =
                local_pacs.record.avail_sink_contexts;

            /* Update the cap count of local PACS. */
            local_pacs.snk_cap_count = cap_count;

            APPL_TRC("GA_register_audio_contexts_and_locations: \n");
            retval = GA_register_audio_contexts_and_locations
                     (
                         role,
                         local_pacs.record.avail_sink_contexts,
                         local_pacs.record.supp_sink_contexts,
                         local_pacs.record.sink_audio_locations
                     );
            APPL_TRC("Retval - ");
            appl_ga_utils_display_ga_result(retval);
        }
        else if (GA_ROLE_SOURCE == role)
        {
            /* Adding Source Capability */
            context_type = appl_ga_utils_bap_get_supp_ctx();
            local_pacs.record.supp_src_contexts |= context_type;

            context_type = appl_ga_utils_bap_get_available_ctx();
            local_pacs.record.avail_src_contexts |= context_type;

            audio_loc = appl_ga_utils_bap_get_audio_channel_alloc();
            local_pacs.record.src_audio_locations |= audio_loc;

            /*
             * When exposing support for the LC3 codec in a PAC record,
             * the Preferred_Audio_Context LTV structure defined in Bluetooth
             * Assigned Numbers may be present in the Metadata field.
             * The absence of the Preferred_Audio_Contexts LTV structure in a
             * PAC record means no preference for audio data Context Types are
             * defined in that PAC record.
             */
            local_pacs.src_cap[local_pacs.src_cap_count].cap.pref_audio_contexts =
                local_pacs.record.avail_src_contexts;

            /* Update the cap count of local PACS. */
            local_pacs.src_cap_count = cap_count;

            APPL_TRC("GA_register_audio_contexts_and_locations: \n");
            retval = GA_register_audio_contexts_and_locations
                     (
                         role,
                         local_pacs.record.avail_src_contexts,
                         local_pacs.record.supp_src_contexts,
                         local_pacs.record.src_audio_locations
                     );
            APPL_TRC("Retval - ");
            appl_ga_utils_display_ga_result(retval);
        }
    }
    else
    {
        appl_ga_utils_bap_remove_pacs_cap();
    }

    appl_ga_utils_bap_display_local_pacs();

    return retval;
}
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
GA_RESULT appl_ga_utils_bap_register_pacs_using_default_values
          (
              /* IN */  APPL_GA_ROLE  role,
              /* IN  */ UINT8         cap_role
          )
{
    GA_RESULT      retval;
    int            choice;
    UINT8          index;
    UINT8          j;

    GA_AUDIO_CAP * cap;
    /* Total Capability Count Present */
    UINT8          cap_count;
    /* Total Capability Count added during this trial */
    UINT8          count;
    UINT8          reg_cap_id[MAX_AUDIO_CAP];

    /* PACS: Sink PAC/Source PAC */
    UINT8          csc_set_count;
    UINT8          csc_set_id;
    GA_LC3_CODEC_SPECIFIC_CAP csc = { 0 };

    /* PACS: Locations, Avaliable and Supported Audio Contexts */
    UINT16         context_type;
    UINT32         audio_loc;

    GA_IGNORE_UNUSED_PARAM(role);

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    index = 0U;
    j = 0U;
    cap_count = 0U;
    count = 0U;
    csc_set_count = 0U;
    csc_set_id = 0U;
    context_type = 0U;
    audio_loc = 0U;

    /*
     * This refers to total SINK/SOURCE PACS Char Count,
     * Number of Capabilty Count equals Number of Source/Sink Char Instance.
     */
    count = 1U;

    /* Fetch the previously added Count, Continue from there */
    if (GA_ROLE_SOURCE == cap_role)
    {
        cap_count = local_pacs.src_cap_count;
        cap = &local_pacs.src_cap[cap_count].cap;
    }
    else if (GA_ROLE_SINK == cap_role)
    {
        cap_count = local_pacs.snk_cap_count;
        cap = &local_pacs.snk_cap[cap_count].cap;
    }
    else
    {
        APPL_INF("Invalid GA Role: 0x%02X\n", cap_role);
        return retval;
    }

    for (index = 0U; index < count; index++)
    {
        /* Set the Number of Sets for this single PAC Record */
        csc_set_count = 1U;

        /* This implies adding of Multiple Sets in a single PACS record */
        for (j = 0U; j < csc_set_count; j++)
        {
            /*
             * Setting of Audio Channel Count: Optional field in Codec
             * Spec Cap
             */
            /*
             * The absence of the Supported_Audio_Channel_Counts LTV structure
             * shall be interpreted as equivalent to a
             * Supported_Audio_Channel_Counts value of
             * 0x01 (one Audio Channel supported)
             */
            csc.supported_acc = 0x01U;

            /*
             * Setting of Max Codec Frames Per SDU: Optional field in Codec
             * Spec Cap
             */
            /*
             * The absence of the Supported_Max_Codec_Frames_Per_SDU LTV
             * structure shall be interpreted as equivalent to a
             * Supported_Max_Codec_Frames_Per_SDU value of 1 codec frame per
             * Audio Channel per SDU maximum
             */
            csc.supported_mcfpSDU = 0x01U;

            /* Set the Codec Specfic Capability Set ID */
            /* TMAP Role: UMR, Shall be one of
             * 11: 48_1,
             * 12: 48_2,
             * 13: 48_3,
             * 14: 48_4,
             * 15: 48_5,
             * 16: 48_6
             *
             * 4: 16_2,
             * 6: 24_2,
             *
             */

            /*
             * Using 12, 48_2 Set ID,
             * Refer appl_ga_utils_bap_get_cs_cap_set_id().
             */
            csc_set_id = 12U;

            /* Fetch the CSC Set ID and prepare */
            if (GA_SUCCESS ==
                appl_ga_utils_bap_prepare_cs_cap_based_on_set_id
                (
                    csc_set_id, &csc
                ))
            {
                /*
                 * If one of the Set is chosen as defined in BAP Spec, the
                 * fields are mandated as below as per Spec.
                 * Coding Format: LC3 [As per BAP]
                 * Company ID and Vendor ID: 0x0000U [As per PACS]
                 */
                appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
                (
                    &cap->codec
                );

                /* Prepare the Codec Spec Capability LTVs for LC3 */
                appl_ga_utils_bap_create_lc3_codec_spec_cap_ltv
                (
                    &csc,
                    cap->codec.ie,
                    &cap->codec.ie_len
                );

                /* Update this to be used later for comparison */
                if (GA_ROLE_SOURCE == cap_role)
                {
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_sf |=
                        csc.supported_sf;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_fd |=
                        csc.supported_fd;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_acc |=
                        csc.supported_acc;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_opcf =
                        csc.supported_opcf;
                    local_pacs.src_cap[cap_count].lc3_cs_cap.supported_mcfpSDU =
                        csc.supported_mcfpSDU;
                }
                else if (GA_ROLE_SINK == cap_role)
                {
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_sf |=
                        csc.supported_sf;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_fd |=
                        csc.supported_fd;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_acc |=
                        csc.supported_acc;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_opcf =
                        csc.supported_opcf;
                    local_pacs.snk_cap[cap_count].lc3_cs_cap.supported_mcfpSDU =
                        csc.supported_mcfpSDU;
                }
            }
            else
            {
                /*
                 * Chosen a set not defined in BAP Spec, Shall be vendor
                 * specific.
                 */
                appl_ga_utils_bap_get_codec_id_for_vendor_spec(&cap->codec);

                /* Fetch the Codec Spec Capability LTVs for Vendor Specific */
                appl_ga_utils_bap_get_vendor_spec_codec_spec_cap_ltv
                (
                    cap->codec.ie,
                    &cap->codec.ie_len
                );
            }
        }

        /* Increment the Record Count */
        cap_count++;
        cap++;
    }

    /*
     * Point to the First Capability Record added during this trial.
     * cap_count - Total Capability Record count.
     * index - Total Capability Record count added during this trial.
     */
    if (GA_ROLE_SOURCE == cap_role)
    {
        cap = &local_pacs.src_cap[cap_count - index].cap;
    }
    else if (GA_ROLE_SINK == cap_role)
    {
        cap = &local_pacs.snk_cap[cap_count - index].cap;
    }

    APPL_TRC("GA_register_audio_capabilities: \n");
    retval = GA_register_audio_capabilities
             (
                 cap_role,
                 count,
                 cap,
                 reg_cap_id
             );
    APPL_TRC("Retval - ");
    appl_ga_utils_display_ga_result(retval);

    if (GA_SUCCESS == retval)
    {
        for (j = 0U; j < count; j++)
        {
            if (GA_ROLE_SINK == cap_role)
            {
                /* Adding Sink Capability */
                local_pacs.snk_cap[cap_count - index].cap_record_id = reg_cap_id[j];
                APPL_TRC("Registered Capability Record ID: %d\n", reg_cap_id[j]);
                index++;
            }
            else
            {
                /* Adding Source Capability */
                local_pacs.src_cap[cap_count - index].cap_record_id = reg_cap_id[j];
                APPL_TRC("Registered Capability Record ID: %d\n", reg_cap_id[j]);
                index++;
            }
        }

        if (GA_ROLE_SINK == cap_role)
        {
            /* Update the cap count of local PACS. */
            local_pacs.snk_cap_count = cap_count;

            /* Use appl_ga_utils_bap_get_supp_ctx() for runtime */
            /* Set Supported Contexts */

            /*
             * As per BAP Spec, 3.5.2.1 Audio data Context Type requirements
             * If the Unicast Server supports the Audio Sink role, the Unicast
             * Server shall support the Context Type value defined as
             * ‘unspecified’ in the Supported_Sink_Contexts field of the
             * Supported Audio Contexts characteristic.
             */

            /*
             * As per HAP Spec, 3.7 BAP Unicast Server role requirements
             * The HA should set to 0b1 at least the bits representing
             * ‘Conversational’, ‘Media’, and ‘Live’ Context Types in the
             * Supported_Sink_Contexts field of the Supported Audio Contexts
             * characteristic in Published Audio Capabilities Service (PACS),
             * in addition to the ‘Unspecified’ Context Type mandated by BAP.
             */
            context_type = GA_CONTEXT_TYPE_UNSPECIFIED | GA_CONTEXT_TYPE_MEDIA;
            local_pacs.record.supp_sink_contexts |= context_type;

            /* Use appl_ga_utils_bap_get_available_ctx() for runtime */
            /* Set Available Contexts */
            context_type = GA_CONTEXT_TYPE_MEDIA;
            local_pacs.record.avail_sink_contexts |= context_type;

            /*
             * As per HAP Spec, 3.7 BAP Unicast Server role requirements
             * If the HA is part of a Binaural Hearing Aid Set, the HA shall
             * set at least one of the Front Left bit or the Front Right bit
             * to a value of 0b1 in the Sink Audio Locations.
             */

            /* Use appl_ga_utils_bap_get_audio_channel_alloc() for runtime */
            /* Set Audio Locations */
            audio_loc = GA_AUDIO_LOCATION_FL;
            local_pacs.record.sink_audio_locations |= audio_loc;

            /*
             * When exposing support for the LC3 codec in a PAC record,
             * the Preferred_Audio_Context LTV structure defined in Bluetooth
             * Assigned Numbers may be present in the Metadata field.
             * The absence of the Preferred_Audio_Contexts LTV structure in a
             * PAC record means no preference for audio data Context Types are
             * defined in that PAC record.
             */
            local_pacs.snk_cap[local_pacs.snk_cap_count].cap.pref_audio_contexts =
                local_pacs.record.avail_sink_contexts;

            APPL_TRC("GA_register_audio_contexts_and_locations: \n");
            retval = GA_register_audio_contexts_and_locations
                     (
                        cap_role,
                        local_pacs.record.avail_sink_contexts,
                        local_pacs.record.supp_sink_contexts,
                        local_pacs.record.sink_audio_locations
                     );
            APPL_TRC("Retval - ");
            appl_ga_utils_display_ga_result(retval);
        }
        else if (GA_ROLE_SOURCE == cap_role)
        {
            /* Update the cap count of local PACS. */
            local_pacs.src_cap_count = cap_count;

            /* Use appl_ga_utils_bap_get_supp_ctx() for runtime */
            /* Set Supported Contexts */
            /*
             * As per BAP Spec, 3.5.2.1 Audio data Context Type requirements
             * If the Unicast Server supports the Audio Source role, the Unicast
             * Server shall support the Context Type value defined as
             * ‘unspecified’ in the Supported_Sink_Contexts field of the
             * Supported Audio Contexts characteristic.
             */
            context_type = GA_CONTEXT_TYPE_UNSPECIFIED | GA_CONTEXT_TYPE_MEDIA;
            local_pacs.record.supp_src_contexts |= context_type;

            /* Use appl_ga_utils_bap_get_available_ctx() for runtime */
            /* Set Available Contexts */
            context_type = GA_CONTEXT_TYPE_MEDIA;
            local_pacs.record.avail_src_contexts |= context_type;

            /* Use appl_ga_utils_bap_get_audio_channel_alloc() for runtime */
            /* Set Audio Locations */
            audio_loc = GA_AUDIO_LOCATION_FL;
            local_pacs.record.src_audio_locations |= audio_loc;

            /*
             * When exposing support for the LC3 codec in a PAC record,
             * the Preferred_Audio_Context LTV structure defined in Bluetooth
             * Assigned Numbers may be present in the Metadata field.
             * The absence of the Preferred_Audio_Contexts LTV structure in a
             * PAC record means no preference for audio data Context Types are
             * defined in that PAC record.
             */
            local_pacs.src_cap[local_pacs.src_cap_count].cap.pref_audio_contexts =
                local_pacs.record.avail_src_contexts;

            APPL_TRC("GA_register_audio_contexts_and_locations: \n");
            retval = GA_register_audio_contexts_and_locations
                     (
                        cap_role,
                        local_pacs.record.avail_src_contexts,
                        local_pacs.record.supp_src_contexts,
                        local_pacs.record.src_audio_locations
                     );
            APPL_TRC("Retval - ");
            appl_ga_utils_display_ga_result(retval);
        }
    }
    else
    {
        appl_ga_utils_bap_remove_pacs_cap();
    }

    appl_ga_utils_bap_display_local_pacs();

    return retval;
}
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */

UINT8 appl_ga_utils_bap_get_cap_record_id(void)
{
    int  choice;

    CONSOLE_OUT("Enter Capability Record ID:\n");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_get_cap(UINT8 role, GA_AUDIO_CAP *cap, UINT8 *record_id)
{
    GA_RESULT      retval;
    int            choice;
    UINT8          index;
    UINT8          j;

    /* Total Capability Count Present */
    UINT8          cap_count;
    /* Total Capability Count added during this trial */
    UINT8          count;

    /* PACS: Sink PAC/Source PAC */
    UINT8          csc_set_count;
    UINT8          csc_set_id;
    GA_LC3_CODEC_SPECIFIC_CAP csc = { 0 };

    /* Initialize */
    retval = GA_FAILURE;
    choice = 0;
    index = 0U;
    j = 0U;
    cap_count = 0U;
    count = 0U;
    csc_set_count = 0U;
    csc_set_id = 0U;

    /* Fetch the Record ID */
    *record_id = appl_ga_utils_bap_get_cap_record_id();

    if (GA_ROLE_SINK == role)
    {
        for (index = 0U; index < MAX_SNK_CODEC_CAP; index++)
        {
            if (*record_id == local_pacs.snk_cap[index].cap_record_id)
            {
                /* This is the PAC Record Instance we are interested in changing */
                GA_mem_set
                (
                    &local_pacs.snk_cap[index].cap,
                    0x00U,
                    sizeof(GA_AUDIO_CAP)
                );
                GA_mem_set
                (
                    &local_pacs.snk_cap[index].lc3_cs_cap,
                    0x00U,
                    sizeof(GA_LC3_CODEC_SPECIFIC_CAP)
                );
                break;
            }
        }
    }
    else if (GA_ROLE_SOURCE == role)
    {
        for (index = 0U; index < MAX_SRC_CODEC_CAP; index++)
        {
            if (*record_id == local_pacs.src_cap[index].cap_record_id)
            {
                /* This is the PAC Record Instance we are interested in changing */
                GA_mem_set
                (
                    &local_pacs.src_cap[index].cap,
                    0x00U,
                    sizeof(GA_AUDIO_CAP)
                );
                GA_mem_set
                (
                    &local_pacs.src_cap[index].lc3_cs_cap,
                    0x00U,
                    sizeof(GA_LC3_CODEC_SPECIFIC_CAP)
                );
                break;
            }
        }
    }

    /* Set the Number of Sets for this single PAC Record */
    csc_set_count = 1U;

    /* This implies adding of Multiple Sets in a single PACS record */
    for (j = 0U; j < csc_set_count; j++)
    {
        /*
         * Setting of Audio Channel Count: Optional field in Codec
         * Spec Cap
         */
        /*
         * The absence of the Supported_Audio_Channel_Counts LTV structure
         * shall be interpreted as equivalent to a
         * Supported_Audio_Channel_Counts value of
         * 0x01 (one Audio Channel supported)
         */
        csc.supported_acc = 0x01U;

        /*
         * Setting of Max Codec Frames Per SDU: Optional field in Codec
         * Spec Cap
         */
        /*
         * The absence of the Supported_Max_Codec_Frames_Per_SDU LTV
         * structure shall be interpreted as equivalent to a
         * Supported_Max_Codec_Frames_Per_SDU value of 1 codec frame per
         * Audio Channel per SDU maximum
         */
        csc.supported_mcfpSDU = 0x01U;

        /* Set the Codec Specfic Capability Set ID */
        /* TMAP Role: UMR, Shall be one of
         * 11: 48_1,
         * 12: 48_2,
         * 13: 48_3,
         * 14: 48_4,
         * 15: 48_5,
         * 16: 48_6
         *
         * 4: 16_2,
         * 6: 24_2,
         *
         */

        /*
         * Using 4, 16_2 Set ID,
         * Refer appl_ga_utils_bap_get_cs_cap_set_id().
         */
        csc_set_id = 4U;

        /* Fetch the CSC Set ID and prepare */
        if (GA_SUCCESS ==
            appl_ga_utils_bap_prepare_cs_cap_based_on_set_id
            (
                csc_set_id, &csc
            ))
        {
            /*
             * If one of the Set is chosen as defined in BAP Spec, the
             * fields are mandated as below as per Spec.
             * Coding Format: LC3 [As per BAP]
             * Company ID and Vendor ID: 0x0000U [As per PACS]
             */
            appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
            (
                &cap->codec
            );

            /* Prepare the Codec Spec Capability LTVs for LC3 */
            appl_ga_utils_bap_create_lc3_codec_spec_cap_ltv
            (
                &csc,
                cap->codec.ie,
                &cap->codec.ie_len
            );

            /* Update this to be used later for comparison */
            if (GA_ROLE_SOURCE == role)
            {
                local_pacs.src_cap[index].lc3_cs_cap.supported_sf |=
                    csc.supported_sf;
                local_pacs.src_cap[index].lc3_cs_cap.supported_fd |=
                    csc.supported_fd;
                local_pacs.src_cap[index].lc3_cs_cap.supported_acc |=
                    csc.supported_acc;
                local_pacs.src_cap[index].lc3_cs_cap.supported_opcf =
                    csc.supported_opcf;
                local_pacs.src_cap[index].lc3_cs_cap.supported_mcfpSDU =
                    csc.supported_mcfpSDU;
            }
            else if (GA_ROLE_SINK == role)
            {
                local_pacs.snk_cap[index].lc3_cs_cap.supported_sf |=
                    csc.supported_sf;
                local_pacs.snk_cap[index].lc3_cs_cap.supported_fd |=
                    csc.supported_fd;
                local_pacs.snk_cap[index].lc3_cs_cap.supported_acc |=
                    csc.supported_acc;
                local_pacs.snk_cap[index].lc3_cs_cap.supported_opcf =
                    csc.supported_opcf;
                local_pacs.snk_cap[index].lc3_cs_cap.supported_mcfpSDU =
                    csc.supported_mcfpSDU;
            }
        }
        else
        {
            /*
             * Chosen a set not defined in BAP Spec, Shall be vendor
             * specific.
             */
            appl_ga_utils_bap_get_codec_id_for_vendor_spec(&cap->codec);

            /* Fetch the Codec Spec Capability LTVs for Vendor Specific */
            appl_ga_utils_bap_get_vendor_spec_codec_spec_cap_ltv
            (
                cap->codec.ie,
                &cap->codec.ie_len
            );
        }
    }

    /*
     * Point to the First Capability Record added during this trial.
     * cap_count - Total Capability Record count.
     * index - Total Capability Record count added during this trial.
     */
    if (GA_ROLE_SOURCE == role)
    {
        GA_mem_copy
        (
            &local_pacs.src_cap[index].cap,
            cap,
            sizeof(GA_AUDIO_CAP)
        );
    }
    else if (GA_ROLE_SINK == role)
    {
        GA_mem_copy
        (
            &local_pacs.snk_cap[index].cap,
            cap,
            sizeof(GA_AUDIO_CAP)
        );
    }

    return;
}

void appl_ga_utils_bap_remove_pacs_cap(void)
{
    /* TODO: Implement */

    return;
}

void appl_ga_utils_bap_display_local_pacs(void)
{
    /* TODO: Implement */

    /* Display Sink Related */

    /* Display Source Related */

    return;
}

void appl_ga_utils_bap_display_remote_pacs(UINT8 conn_dev_index)
{
    GA_IGNORE_UNUSED_PARAM(conn_dev_index);
    /* TODO: Implement */

    /* Display Sink Related */

    /* Display Source Related */

    return;
}

GA_RESULT appl_ga_utils_add_conn_device_as_a_initiator(GA_ENDPOINT device)
{
    UINT8  index;
    GA_ENDPOINT dev;
    GA_RESULT retval;

    index = 0U;
    retval = GA_SUCCESS;

    for (index = 0U; index < MAX_CONN_DEVICES; index++)
    {
        if (GA_SUCCESS == appl_ga_utils_get_conn_device(index, &dev))
        {
            if (GA_COMPARE_ENDPOINT(&device, &dev))
            {
                /* Device is already added */
                retval = GA_FAILURE;
                break;
            }
        }
    }

    /* It is a new entry, Add it here */
    if ((GA_SUCCESS == retval) &&
        (GA_SUCCESS == appl_ga_utils_get_free_conn_device(&index)))
    {
        GA_mem_copy(&conn_dev[index].device, &device, sizeof(GA_ENDPOINT));
        conn_dev[index].isInitiator = GA_TRUE;
    }
    else
    {
        APPL_TRC("Maximum Device list reached !\n");
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_utils_handle_conn_device
          (
              GA_ENDPOINT  device,
              GA_RESULT    status,
              UINT16       handle
          )
{
    UINT8  index;
    GA_RESULT retval;

    index = 0U;
    retval = GA_SUCCESS;

    if ((GA_SUCCESS == appl_ga_utils_get_conn_device_index(device, &index)) &&
        (GA_TRUE == conn_dev[index].isInitiator))
    {
        if (GA_SUCCESS != status)
        {
            /* If Status is Failure, Remove the entry for Client Role */
            appl_ga_utils_remove_conn_device(index);
        }
        else
        {
            /* If Status is Success, Update connection handle */
            conn_dev[index].conn_handle = handle;
        }
    }
    else
    {
        /* It is a new entry, Add it here, it must be a Server */
        if (GA_SUCCESS == appl_ga_utils_get_free_conn_device(&index))
        {
            GA_mem_copy(&conn_dev[index].device, &device, sizeof(GA_ENDPOINT));
            conn_dev[index].isInitiator = GA_FALSE;
            conn_dev[index].conn_handle = handle;
        }
        else
        {
            APPL_DBG("Maximum Simultaneous Connections reached !\n");
            retval = GA_FAILURE;
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_remove_conn_device(UINT8 index)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_SUCCESS;

    if (index >= MAX_CONN_DEVICES)
    {
        APPL_ERR("Invalid Index !\n");
        retval = GA_FAILURE;
    }
    else
    {
        if (GA_FALSE ==
            GA_BRR_DEV_ADDR_IS_NON_ZERO(conn_dev[index].device.bd_addr))
        {
            APPL_ERR("Could not remove the device from Database !\n");
            retval = GA_FAILURE;
        }
        else
        {
            APPL_INF("Removing Device from Database\n");
            GA_mem_set(&conn_dev[index], 0x00U, sizeof(CONN_DEVICES));
            conn_dev[index].conn_handle = APPL_INVALID_CONN_HANDLE;
        }
    }

    return retval;
}

void appl_ga_utils_display_all_conn_device(void)
{
    UINT8  index;
    UINT8  is_present;

    /* Initialize */
    is_present = GA_FALSE;

    /* Display Connected Devices, Role with Index */
    for (index = 0U; index < MAX_CONN_DEVICES; index++)
    {
        if ((GA_BRR_DEV_ADDR_IS_NON_ZERO(conn_dev[index].device.bd_addr)) &&
            (APPL_INVALID_CONN_HANDLE != conn_dev[index].conn_handle))
        {
            APPL_TRC("Device: \n");
            appl_ga_utils_display_bdaddr_and_type(&conn_dev[index].device);
            APPL_TRC("Connection Handle: 0x%04X\n", conn_dev[index].conn_handle);
            APPL_TRC("Initiator: 0x%02X\n", conn_dev[index].isInitiator);
            APPL_TRC("Index: 0x%02X\n\n", index + 1U);
            is_present = GA_TRUE;
        }
    }
    if (GA_FALSE == is_present)
    {
        APPL_TRC("No device is registered yet\n");
    }

    return;
}

GA_RESULT appl_ga_utils_set_conn_device_in_use(UINT8 index)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (index >= MAX_CONN_DEVICES)
    {
        APPL_ERR("Invalid Index !\n");
    }
    else
    {
        if (GA_FALSE ==
            GA_BRR_DEV_ADDR_IS_NON_ZERO(conn_dev[index].device.bd_addr))
        {
            APPL_ERR("Device not added into the Database !\n");
        }
        else
        {
            /*
             * While taking conn_dev_index as input from the user,
             * Already Decremented during taking input.
             */
            conn_dev_in_use = index;
            retval = GA_SUCCESS;
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_get_free_conn_device(UINT8 * index)
{
    UINT8  i;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        /* Validate if device is Empty */
        if ((!GA_IS_NON_ZERO_ENDPOINT_ADDR(conn_dev[i].device.bd_addr)) &&
            (APPL_INVALID_CONN_HANDLE == conn_dev[i].conn_handle))
        {
            *index = i;
            return GA_SUCCESS;
        }
    }

    return GA_FAILURE;
}

void appl_ga_utils_get_conn_device_in_use
     (
         UINT8       * index,
         GA_ENDPOINT * device,
         GA_CONTEXT  * context
     )
{
    if (NULL != context)
    {
        *context = conn_dev[conn_dev_in_use].context;
    }
    if (NULL != index)
    {
        *index = conn_dev_in_use;
    }
    if (NULL != device)
    {
        GA_mem_copy
        (
            device,
            &conn_dev[conn_dev_in_use].device,
            sizeof(GA_ENDPOINT)
        );
    }

    return;
}

GA_RESULT appl_ga_utils_get_conn_handle_from_index
          (
              UINT8    conn_dev_index,
              UINT16 * acl_handle
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    /* Validate Index for invalid value */
    if (MAX_CONN_DEVICES <= conn_dev_index)
    {
        APPL_ERR("Invalid Index !\n");
        return retval;
    }

    /* Check if info is filled for that particular index */
    if (GA_IS_NON_ZERO_ENDPOINT_ADDR(conn_dev[conn_dev_index].device.bd_addr))
    {
        *acl_handle = conn_dev[conn_dev_index].conn_handle;
        retval = GA_SUCCESS;
    }
    else
    {
        APPL_ERR("Device Info not filled in the index provided !\n");
        return retval;
    }

    return retval;
}

void appl_ga_utils_update_context_of_conn_dev
     (
         GA_CONTEXT  context,
         UINT8       index
     )
{
    conn_dev[index].context = context;

    return;
}

GA_RESULT appl_ga_utils_get_conn_device(UINT8 index, GA_ENDPOINT * device)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (index >= MAX_CONN_DEVICES)
    {
        APPL_ERR("Invalid Index !\n");
    }
    else
    {
        if (GA_TRUE ==
            GA_BRR_DEV_ADDR_IS_NON_ZERO(conn_dev[index].device.bd_addr))
        {
            /* Index and device at this index are validated, copy the device_info */
            GA_mem_copy(device, &conn_dev[index].device, sizeof(GA_ENDPOINT));
            retval = GA_SUCCESS;
        }
        else
        {
            /* Entry is empty */
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_get_conn_device_index(GA_ENDPOINT device, UINT8 * index)
{
    GA_RESULT retval;
    UINT8 i;

    retval = GA_FAILURE;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        /* Compare Device */
        if (GA_COMPARE_ENDPOINT(&device, &conn_dev[i].device))
        {
            retval = GA_SUCCESS;
            *index = i;
            break;
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_get_dev_index_from_handle
          (
              UINT16  conn_handle,
              UINT8 * index
          )
{
    GA_RESULT retval;
    UINT8 i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        /* Compare Device */
        if (conn_handle == conn_dev[i].conn_handle)
        {
            retval = GA_SUCCESS;
            *index = i;
            break;
        }
    }
    return retval;
}

GA_RESULT appl_ga_utils_get_dev_from_handle
          (
              UINT16        conn_handle,
              GA_ENDPOINT * device
          )
{
    GA_RESULT retval;
    UINT8 i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        /* Compare Device */
        if (conn_handle == conn_dev[i].conn_handle)
        {
            retval = GA_SUCCESS;
            GA_mem_copy
            (
                device,
                &conn_dev[i].device, sizeof(GA_ENDPOINT)
            );
            break;
        }
    }
    return retval;
}

GA_RESULT appl_ga_utils_get_dev_handle_from_device
          (
            GA_ENDPOINT   device,
            UINT16      * conn_handle
          )
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    for (i = 0U; i < MAX_CONN_DEVICES; i++)
    {
        /* Compare Device */
        if (GA_COMPARE_ENDPOINT(&device, &conn_dev[i].device))
        {
            retval = GA_SUCCESS;
            *conn_handle = conn_dev[i].conn_handle;
            break;
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_add_remote_pacs_for_dev
          (
              GA_ENDPOINT     * device,
              GA_CAP_ENDPOINT * capab
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      conn_dev_index;
    UINT8      type;
    UINT8      record_id;

    /* Initialize */
    retval = GA_SUCCESS;
    index = 0U;
    conn_dev_index = 0U;

    if (APPL_GA_INIT_STATUS_GA == ga_init_status)
    {
        /* Fetch conn_dev_index based on device */
        retval = appl_ga_utils_get_conn_device_index(*device, &conn_dev_index);
    }

    if (GA_SUCCESS == retval)
    {
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            if (GA_SUCCESS ==
                    appl_ga_utils_bap_get_remote_pacs_index(conn_dev_index, &index))
            {
                /* Already present, update PACS data */
            }
            else if (GA_SUCCESS ==
                         appl_ga_utils_bap_fetch_remote_free_entry_for_pacs(&index))
            {
                /* Update the conn_dev_index */
                remote_pacs[index].conn_dev_index = conn_dev_index;
            }
            else
            {
                APPL_DBG("No free entry for remote PACS !\n");
                retval = GA_FAILURE;
            }
        }

        if (GA_SUCCESS == retval && NULL != capab)
        {
            /* Derive the Capability Type: LSB */
            type = capab->type & 0x0F;

            /* Add the details */
            if (GA_CAP_TYPE_CODECS == type)
            {
                /* Derive the Capability Record ID: MSB */
                record_id = capab->type >> 4U;

                /* Display the Capability Record ID */
                APPL_TRC("Capability Record ID: %d", record_id);

                appl_ga_utils_bap_parse_and_display_audio_cap_codecs
                (
                    index,
                    capab->role,
                    capab->data,
                    capab->datalen
                );
            }
            else if (GA_CAP_TYPE_LOCATIONS == type)
            {
                appl_ga_utils_bap_parse_and_display_audio_cap_locations
                (
                    index,
                    capab->role,
                    capab->data,
                    capab->datalen
                );
            }
            else if (GA_CAP_TYPE_AVAIL_CONTEXTS == type)
            {
                appl_ga_utils_bap_parse_and_display_audio_cap_avail_ctx
                (
                    index,
                    capab->role,
                    capab->data,
                    capab->datalen
                );
            }
            else if (GA_CAP_TYPE_SUPP_CONTEXTS == type)
            {
                appl_ga_utils_bap_parse_and_display_audio_cap_supp_ctx
                (
                    index,
                    capab->role,
                    capab->data,
                    capab->datalen
                );
            }
            else
            {
                APPL_ERR("Unknown Capability Type !\n");
                retval = GA_FAILURE;
            }
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_get_remote_pacs_index
          (
              UINT8   conn_dev_index,
              UINT8 * index
          )
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    for (i = 0U; i < MAX_REM_PACS_CAP_INFO; i++)
    {
        if (conn_dev_index == remote_pacs[i].conn_dev_index)
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_remove_remote_pacs_for_dev(UINT8 conn_dev_index)
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    /* Get the index for remote_pacs */
    if (GA_SUCCESS ==
        appl_ga_utils_bap_get_remote_pacs_index(conn_dev_index, &index))
    {
        /* Remove PACS data */
        GA_mem_set
        (
            &remote_pacs[index].pacs,
            0U,
            (sizeof(PACS_CAP_INFO))
        );
        remote_pacs[index].conn_dev_index = INVALID_CONN_DEV_INDEX;
        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_fetch_remote_pacs_based_on_dev
          (
              GA_ENDPOINT     device,
              PACS_CAP_INFO * pacs
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      conn_dev_index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;
    conn_dev_index = 0U;

    retval = appl_ga_utils_get_conn_device_index(device, &conn_dev_index);

    /* Compare and fetch the index for remote_pacs based on conn_dev_index */
    /* Get the index for remote_pacs based on conn_dev_index */
    if (GA_SUCCESS ==
        appl_ga_utils_bap_get_remote_pacs_index(conn_dev_index, &index))
    {
        if (NULL != pacs)
        {
            GA_mem_copy
            (
                pacs,
                &remote_pacs[index].pacs,
                sizeof(PACS_CAP_INFO)
            );
        }
        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT appl_ga_utils_bap_fetch_remote_free_entry_for_pacs(UINT8 * index)
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    for (i = 0U; i < MAX_REM_PACS_CAP_INFO; i++)
    {
        /* Check for remote PACS free entry */
        if (INVALID_CONN_DEV_INDEX == remote_pacs[i].conn_dev_index)
        {
            /* Set the index of free entry */
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

void appl_ga_utils_bap_display_avail_and_supp_pacs_ctx
     (
         UINT8                role,
         GA_PACS_AUDIO_RECORD pacs_audio_record
     )
{
    if (GA_ROLE_SINK == role)
    {
        APPL_TRC
        (
            "Supported Sink Contexts: 0x%02X\n",
            pacs_audio_record.supp_sink_contexts
        );
        appl_ga_utils_bap_display_ctx
        (
            pacs_audio_record.supp_sink_contexts
        );
        APPL_TRC
        (
            "Available Sink Contexts: 0x%02X\n",
            pacs_audio_record.avail_sink_contexts
        );
        appl_ga_utils_bap_display_ctx
        (
            pacs_audio_record.avail_sink_contexts
        );
    }
    else if (GA_ROLE_SOURCE == role)
    {
        APPL_TRC
        (
            "Supported Source Contexts: 0x%02X\n",
            pacs_audio_record.supp_src_contexts
        );
        appl_ga_utils_bap_display_ctx
        (
            pacs_audio_record.supp_src_contexts
        );
        APPL_TRC
        (
            "Available Source Contexts: 0x%02X\n",
            pacs_audio_record.avail_src_contexts
        );
        appl_ga_utils_bap_display_ctx
        (
            pacs_audio_record.avail_src_contexts
        );
    }

    return;
}

void appl_ga_utils_bap_fetch_local_pacs(PACS_CAP_INFO * pacs)
{
    GA_mem_copy
    (
        pacs,
        &local_pacs,
        sizeof(PACS_CAP_INFO)
    );

    return;
}

void appl_ga_utils_bap_parse_and_display_audio_cap_codecs
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     )
{
    GA_AUDIO_CAP * audio_cap;
    UINT8          cap_count;
    UINT8          num_of_pacs_records;
    UINT8          marker;
    UINT8          metadata_len;
    UINT8          i;
    GA_AUDIO_CAP   snk_cap[MAX_SNK_CODEC_CAP] = {0};
    GA_AUDIO_CAP   src_cap[MAX_SRC_CODEC_CAP] = {0};

    GA_LC3_CODEC_SPECIFIC_CAP rem_cs_cap;

    audio_cap = NULL;

    if (NULL == cap)
    {
        APPL_ERR("cap is NULL !\n");
        return;
    }

    cap_count = 0U;

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();

    marker = 0U;
    num_of_pacs_records = cap[marker];

    if (GA_ROLE_SINK & role)
    {
        APPL_TRC
        (
            "Char UUID: Sink PAC (0x%04X)\n",
            GA_CHAR_PACS_SINK_PAC
        );
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            cap_count = remote_pacs[index].pacs.snk_cap_count;
            audio_cap = &remote_pacs[index].pacs.snk_cap[cap_count].cap;
        }
        else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
        {
            audio_cap = &snk_cap[cap_count];
        }
    }
    else if (GA_ROLE_SOURCE & role)
    {
        APPL_TRC
        (
            "Char UUID: Source PAC (0x%04X)\n",
            GA_CHAR_PACS_SOURCE_PAC
        );
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            cap_count = remote_pacs[index].pacs.src_cap_count;
            audio_cap = &remote_pacs[index].pacs.src_cap[cap_count].cap;
        }
        else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
        {
            audio_cap = &src_cap[cap_count];
        }
    }
    GA_FORMAT_DISPLAY();

    appl_ga_utils_bap_display_role(role);
    appl_ga_utils_bap_display_cap_type(GA_CAP_TYPE_CODECS);

    APPL_TRC("Data Length: 0x%02X\n", cap_len);
    APPL_TRC("Data:\n");
    APPL_TRC
    (
        "Len: 0x%02X,  Num of PAC Records: 0x%02X\n",
        (UCHAR)sizeof(UINT8), num_of_pacs_records
    );
    /* Increment for num_of_pacs_records */
    marker++;

    for (i = 0U; i < num_of_pacs_records; i++)
    {
        /* Codec Info is parsed, filled and printed */
        appl_ga_utils_bap_parse_and_fill_codec_id
        (
            &cap[marker],
            (sizeof(UINT8) + sizeof(UINT16) + sizeof(UINT16)),
            &audio_cap->codec
        );
        /* Size of Codec_ID from the spec is 5 octets */
        marker += (sizeof(UINT8) + sizeof(UINT16) + sizeof(UINT16));

        APPL_TRC
        (
            "Len: 0x%02X,  Codec Specific Capabilities Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8), cap[marker]
        );
        audio_cap->codec.ie_len = cap[marker];
        marker++;

        APPL_TRC
        (
            "Len: 0x%02X,  Codec Specific Capabilities:\n",
            audio_cap->codec.ie_len
        );
        GA_mem_copy
        (
            audio_cap->codec.ie,
            &cap[marker],
            audio_cap->codec.ie_len
        );

        appl_ga_utils_bap_parse_and_display_codec_spec_cap_ltv
        (
            audio_cap->codec.ie,
            audio_cap->codec.ie_len,
            &rem_cs_cap
        );
        marker += audio_cap->codec.ie_len;

        metadata_len = cap[marker];
        marker++;

        APPL_TRC
        (
            "Len: 0x%02X,  Metadata Length: 0x%02X\n",
            (UCHAR)sizeof(UINT8), metadata_len
        );
        if (0x00U != metadata_len)
        {
            APPL_TRC("Len: 0x%02X,  Metadata:\n", metadata_len);

            appl_ga_utils_bap_parse_and_display_metadata_ltv
            (
                &cap[marker],
                metadata_len,
                NULL,
                NULL
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Metadata: NULL\n", metadata_len);
        }
        marker += metadata_len;

        /* TODO: Add a comment. Parsing issue */
        marker--;

        APPL_TRC("Codec Specific Capabilities PAC Record Index: 0x%02X\n",
            cap_count);

        cap_count++;
        if (GA_ROLE_SOURCE == role)
        {
            cap++;
        }
        else if (GA_ROLE_SINK == role)
        {
            cap++;
        }
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_audio_cap_locations
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     )
{
    /*
     * This is used to reference the sink/source audio location based
     * on the role
     */
    UINT32  location;

    if (NULL == cap)
    {
        APPL_ERR("cap is NULL !\n");
        return;
    }

    location = 0U;

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();

    if (GA_ROLE_SINK & role)
    {
        APPL_TRC
        (
            "Char UUID: Sink Audio Locations (0x%04X)\n",
            GA_CHAR_PACS_SINK_AUDIO_LOCATIONS
        );
    }
    else if (GA_ROLE_SOURCE & role)
    {
        APPL_TRC
        (
            "Char UUID: Source Audio Locations (0x%04X)\n",
            GA_CHAR_PACS_SOURCE_AUDIO_LOCATIONS
        );
    }
    GA_FORMAT_DISPLAY();

    appl_ga_utils_bap_display_role(role);
    appl_ga_utils_bap_display_cap_type(GA_CAP_TYPE_LOCATIONS);

    APPL_TRC("Data Length: 0x%02X\n", cap_len);
    APPL_TRC("Data:\n");
    if (sizeof(UINT32) == cap_len)
    {
        GA_UNPACK_LE_4_BYTE
        (
            &location,
            cap
        );

        APPL_TRC("Len: 0x%02X,  Audio Locations: 0x%08X\n", cap_len, location);
        appl_ga_utils_bap_display_audio_locations(location);

        if (GA_ROLE_SINK & role)
        {
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                remote_pacs[index].pacs.record.sink_audio_locations = location;
            }
        }
        else if (GA_ROLE_SOURCE & role)
        {
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                remote_pacs[index].pacs.record.src_audio_locations = location;
            }
        }
    }
    else
    {
        APPL_ERR("Unexpected Length !\n");
        appl_dump_bytes(cap, cap_len);
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_audio_cap_avail_ctx
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     )
{
    UINT16  ctx;

    if (NULL == cap)
    {
        APPL_ERR("cap is NULL !\n");
        return;
    }

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC
    (
        "Char UUID: Available Contexts (0x%04X)\n",
        GA_CHAR_PACS_AVAILABLE_AUDIO_CONTEXTS
    );

    GA_FORMAT_DISPLAY();

    appl_ga_utils_bap_display_role(role);
    appl_ga_utils_bap_display_cap_type(GA_CAP_TYPE_AVAIL_CONTEXTS);

    APPL_TRC("Data Length: 0x%02X\n", cap_len);
    APPL_TRC("Data:\n");

    if ((sizeof(UINT16) + sizeof(UINT16)) == cap_len)
    {
        GA_UNPACK_LE_2_BYTE
        (
            &ctx,
            cap
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Available Sink Contexts: 0x%02X\n",
            (UCHAR)sizeof(ctx), ctx
        );
        appl_ga_utils_bap_display_ctx(ctx);
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            remote_pacs[index].pacs.record.avail_sink_contexts = ctx;
        }

        GA_UNPACK_LE_2_BYTE
        (
            &ctx,
            cap + 2U
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Available Source Contexts:0x%02X\n ",
            (UCHAR)sizeof(ctx), ctx
        );
        appl_ga_utils_bap_display_ctx(ctx);
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            remote_pacs[index].pacs.record.avail_src_contexts = ctx;
        }
    }
    else
    {
        APPL_ERR("Unexpected Length !\n");
        appl_dump_bytes(cap, cap_len);
    }

    return;
}

void appl_ga_utils_bap_parse_and_display_audio_cap_supp_ctx
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     )
{
    UINT16  ctx;

    ctx = 0U;

    if (NULL == cap)
    {
        APPL_ERR("cap is NULL !\n");
        return;
    }
    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC
    (
        "Char UUID: Supported Contexts (0x%04X)\n",
        GA_CHAR_PACS_SUPPORTED_AUDIO_CONTEXTS
    );
    GA_FORMAT_DISPLAY();

    appl_ga_utils_bap_display_role(role);
    appl_ga_utils_bap_display_cap_type(GA_CAP_TYPE_SUPP_CONTEXTS);

    APPL_TRC("Data Length: 0x%02X\n", cap_len);
    APPL_TRC("Data:\n");

    if ((sizeof(UINT16) + sizeof(UINT16)) == cap_len)
    {
        GA_UNPACK_LE_2_BYTE
        (
            &ctx,
            cap
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Supported Sink Contexts: 0x%02X\n",
            (UCHAR)sizeof(ctx), ctx
        );
        appl_ga_utils_bap_display_ctx(ctx);
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            remote_pacs[index].pacs.record.supp_sink_contexts = ctx;
        }

        GA_UNPACK_LE_2_BYTE
        (
            &ctx,
            (cap + 2U)
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Supported Source Contexts: 0x%02X\n",
            (UCHAR)sizeof(ctx), ctx
        );
        appl_ga_utils_bap_display_ctx(ctx);
        if (APPL_GA_INIT_STATUS_GA == ga_init_status)
        {
            remote_pacs[index].pacs.record.supp_src_contexts = ctx;
        }
    }
    else
    {
        APPL_ERR("Unexpected Length !\n");
        appl_dump_bytes(cap, cap_len);
    }

    return;
}

void appl_ga_utils_bap_display_ase_id_for_input(void)
{
    /* TODO: Display from ASE_DISCOVER_CNF event */

    return;
}

UINT8 appl_ga_utils_bap_get_ase_id(void)
{
    int  choice;

    CONSOLE_OUT("Enter ASE ID:\n");
    appl_ga_utils_bap_display_ase_id_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_display_cis_id_for_input(void)
{
    APPL_TRC("CIS_ID: 0x00 - 0xEF\n");

    return;
}

UINT8 appl_ga_utils_bap_get_cis_id(void)
{
    int  choice;

    CONSOLE_OUT("Enter CIS ID:\n");
    appl_ga_utils_bap_display_cis_id_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_bap_display_cig_id_for_input(void)
{
    APPL_TRC("CIG_ID: 0x00 - 0xEF\n");

    return;
}

UINT8 appl_ga_utils_bap_get_cig_id(void)
{
    int  choice;

    CONSOLE_OUT("Enter CIG ID:\n");
    appl_ga_utils_bap_display_cig_id_for_input();
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

void appl_ga_utils_connect(GA_ENDPOINT device, UCHAR mode)
{
    /*
     * Create Extended Connection with remote
     * (Mode - 0x00(Connection), 0x01(Extended Connection))
     */
    appl_hci_le_connect((BT_DEVICE_ADDR *)&device, mode);

    return;
}

GA_RESULT appl_ga_utils_disconnect
          (
              UINT16  connection_handle,
              UCHAR   reason
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_SUCCESS;

    retval = BT_hci_disconnect
             (
                 connection_handle,
                 reason
             );

    return retval;
}

UINT8 appl_ga_utils_bap_get_data_flow_direction(void)
{
    int  choice;
    UINT8 direction;

    CONSOLE_OUT("Enter Direction:\n");
    appl_ga_utils_bap_display_data_flow_direction_for_input();
    CONSOLE_IN("%d", &choice);

    direction = (UINT8)choice;
    if (ASE_DIRECTION_INVALID == direction)
    {
        APPL_ERR("Invalid Input for ASE Direction !\n");
    }
    return direction;
}

void appl_ga_utils_bap_display_data_flow_direction_for_input(void)
{
    APPL_TRC("1. Unidirectional\n");
    APPL_TRC("2. Bidirectional\n");

    return;
}

UINT8 appl_ga_utils_bap_get_input_cis_count(void)
{
    int choice;

    CONSOLE_OUT("Enter CIS Count: ");
    CONSOLE_IN("%d", &choice);

    return ((UINT8)choice);
}

UINT8 appl_ga_utils_bap_get_input_conn_dev_index(void)
{
    int choice;

    appl_ga_utils_display_all_conn_device();

    CONSOLE_OUT("Enter the Conn device index: ");
    CONSOLE_IN("%d", &choice);
    /* Decrementing input by as conn_dev_index will start from 0 */
    choice -= 1;

    return ((UINT8)choice);
}

void appl_ga_utils_bap_parse_and_display_ase_state(UCHAR ase_state)
{
    switch (ase_state)
    {
    case SEP_STATE_IDLE:
        APPL_TRC("Idle");
        break;

    case SEP_STATE_CODEC_CONFIGURED:
        APPL_TRC("Codec Configured");
        break;

    case SEP_STATE_QOS_CONFIGURED:
        APPL_TRC("QoS Configured");
        break;

    case SEP_STATE_ENABLING:
        APPL_TRC("Enabling");
        break;

    case SEP_STATE_STREAMING:
        APPL_TRC("Streaming");
        break;

    case SEP_STATE_DISABLING:
        APPL_TRC("Disabling");
        break;

    case SEP_STATE_RELEASING:
        APPL_TRC("Releasing");
        break;

    default:
        APPL_TRC("RFU");
        break;
    }
    APPL_TRC(" (0x%02X)\n", ase_state);

    return;
}

void appl_ga_utils_bap_display_ase_data(UINT8 conn_dev_index, ASE ase)
{
    APPL_TRC
    (
        "Connection Device Index: 0x%02X (%d)\n",
        conn_dev_index, conn_dev_index
    );
    APPL_TRC("ASE ID: 0x%02X\n", ase.id);
    appl_ga_utils_bap_display_role(ase.role);
    APPL_TRC("ASE State: ");
    appl_ga_utils_bap_parse_and_display_ase_state(ase.state);

    return;
}

void appl_ga_utils_bap_display_local_ase_db(ASE ase)
{
    UINT8  index;

    index = 0U;

    appl_ga_utils_bap_display_role(ase.role);
    APPL_TRC("ASE ID: 0x%02X\n", ase.id);
    APPL_TRC("ASE State: ");
    appl_ga_utils_bap_parse_and_display_ase_state(ase.state);
    for (index = 0U; index < MAX_UCS_SIMULTANEOUS_ASE; index++)
    {
        APPL_TRC
        (
            "Connection Device Index[%d]: 0x%02X (%d)\n",
            (index + 1U), ase.conn_dev_index[index], ase.conn_dev_index[index]
        );
    }
    appl_ga_utils_bap_parse_and_display_cig_state(ase.cig_cis_info.cig_state);
    appl_ga_utils_bap_parse_and_display_cis_state(ase.cig_cis_info.cis_state);
    APPL_TRC("CIS Connection Handle: 0x%04X\n", ase.cig_cis_info.handle);
    appl_ga_utils_bap_parse_and_display_iso_state(ase.iso_state);
    APPL_TRC("CIG ID: 0x%02X\n", ase.qos_conf.channel.cig_id);
    APPL_TRC("CIS ID: 0x%02X\n", ase.qos_conf.channel.cis_id);
    appl_ga_utils_bap_parse_and_display_qos_cnf(ase.qos_conf);

    return;
}

void appl_ga_utils_bap_parse_and_display_cig_state(UINT8 cig_state)
{
    APPL_TRC("CIG State: ");
    if (CIG_STATE_IDLE == cig_state)
    {
        APPL_TRC("Idle");
    }
    else if (CIG_STATE_WAITING == cig_state)
    {
        APPL_TRC("Waiting");
    }
    else if (CIG_STATE_CREATED == cig_state)
    {
        APPL_TRC("Created");
    }
    else
    {
        APPL_TRC("Undefined CIG State");
    }
    APPL_TRC(" (0x%02X)\n", cig_state);

    return;
}

void appl_ga_utils_bap_parse_and_display_cis_state(UINT8 cis_state)
{
    APPL_TRC("CIS State: ");
    if (CIS_STATE_IDLE == cis_state)
    {
        APPL_TRC("Idle");
    }
    else if (CIS_STATE_INITIALIZED == cis_state)
    {
        APPL_TRC("Initialized");
    }
    else if (CIS_STATE_WAITING == cis_state)
    {
        APPL_TRC("Waiting");
    }
    else if (CIS_STATE_CONNECTED == cis_state)
    {
        APPL_TRC("Connected");
    }
    else
    {
        APPL_TRC("Undefined CIS State");
    }
    APPL_TRC(" (0x%02X)\n", cis_state);

    return;
}

void appl_ga_utils_bap_parse_and_display_iso_state(UINT8 iso_state)
{
    APPL_TRC("ISO State: ");
    if (AUDIO_ISO_IDLE == iso_state)
    {
        APPL_TRC("Idle");
    }
    else if (AUDIO_ISO_UPDATE_IN_PROGRESS == iso_state)
    {
        APPL_TRC("ISO Update in progress");
    }
    else if (AUDIO_ISO_SETUP_COMPLETE == iso_state)
    {
        APPL_TRC("ISO Setup Complete");
    }
    else
    {
        APPL_TRC("Undefined ISO State");
    }
    APPL_TRC(" (0x%02X)\n", iso_state);

    return;
}

UINT16 appl_ga_utils_get_fd_in_us(UINT8 fd_in_ms)
{
    /* Frame duration in MicroSeconds */
    UINT16 fd_in_us;

    /* Initialize */
    fd_in_us = 0U;

    switch (fd_in_ms)
    {
        /* Extract the Frame Duration in value */
        case GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS:
            fd_in_us = 7500U;
            break;

        case GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS:
            fd_in_us = 10000U;
            break;

        default:
            fd_in_us = 0U;
            break;
    }

    return fd_in_us;
}

void appl_ga_utils_pacs_display_char_uuid(UINT16 char_uuid)
{
    GA_FORMAT_DISPLAY();
    APPL_TRC("Char UUID: ");
    switch(char_uuid)
    {
        case GA_CHAR_PACS_SINK_AUDIO_LOCATIONS:
            APPL_TRC("Sink Audio Locations (0x%04X)\n", GA_CHAR_PACS_SINK_AUDIO_LOCATIONS);
            break;

        case GA_CHAR_PACS_SOURCE_AUDIO_LOCATIONS:
            APPL_TRC("Source Audio Locations (0x%04X)\n", GA_CHAR_PACS_SOURCE_AUDIO_LOCATIONS);
            break;

        default:
            /* Should not reach here */
            break;
    }
    GA_FORMAT_DISPLAY();

    return;
}

void appl_ga_utils_ascs_display_char_uuid(UINT16 char_uuid)
{
    GA_FORMAT_DISPLAY();
    APPL_TRC("Char UUID: ");
    switch(char_uuid)
    {
        case GA_CHAR_ASCS_SINK_ASE:
            APPL_TRC("Sink ASE (0x%04X)\n", GA_CHAR_ASCS_SINK_ASE);
            break;

        case GA_CHAR_ASCS_SOURCE_ASE:
            APPL_TRC("Source ASE (0x%04X)\n", GA_CHAR_ASCS_SOURCE_ASE);
            break;

        case GA_CHAR_ASCS_ASE_CP:
            APPL_TRC("ASE Control Point (0x%04X)\n", GA_CHAR_ASCS_ASE_CP);
            break;

        default:
            /* Should not reach here */
            break;
    }
    GA_FORMAT_DISPLAY();

    return;
}

UINT8 appl_ga_utils_fetch_sacc_from_aca(UINT32 aca)
{
    UINT8  sacc;
    UINT8  index;

    sacc = 0U;
    index = 0U;

    /* ACA : 4-octet bitfield of Audio Location values */
    for (index = 0U; index < 32U; index++)
    {
        if (aca & (1U << index))
        {
            sacc++;
        }
    }

    return sacc;
}

void appl_ga_utils_fetch_priority_from_aca
     (
         UINT32 aca,
         APPL_GA_UTILS_AUDIO_ACA_INFO * aca_info
     )
{
    UINT8  sacc;
    UINT8  index;

    sacc = 0U;
    index = 0U;

    /* ACA : 4-octet bitfield of Audio Location values */
    if (GA_AUDIO_LOCATION_FL == (aca & GA_AUDIO_LOCATION_FL))
    {
        aca_info->aca_lp[index].location = GA_AUDIO_LOCATION_FL;
        aca_info->aca_lp[index].position = index + 1U;
        index++;
    }
    if (GA_AUDIO_LOCATION_FR == (aca & GA_AUDIO_LOCATION_FR))
    {
        aca_info->aca_lp[index].location = GA_AUDIO_LOCATION_FR;
        aca_info->aca_lp[index].position = index + 1U;
        index++;
    }
    /* TODO */
    return;
}

UINT8 appl_ga_utils_fetch_qos_conf_set_id_from_qos_conf(GA_QOS_CONF qos_conf)
{
    UINT8  qos_conf_set_id;

    qos_conf_set_id = 0U;
    /*
     * High Reliability and Low Latency differ only in Retransmission
     * number and Max Transport Latency.
     * Using Retransmission number.
     */
    switch(qos_conf.max_sdu)
    {
        case GA_8_1_1_MAXSDUSIZE_BYTES:
            if (GA_8_1_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 1U;
            }
            else if (GA_8_1_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 17U;
            }
            break;

        case GA_8_2_1_MAXSDUSIZE_BYTES:
            /*
             * QOS Configuration Low Latency Set 8_2_1 & 16_1_1 has same
             * Retransmission Number, but they differ in SDU Interval
             * Similarly for QOS Configuration High Reliability
             * Set 8_2_2 & 16_1_2
             */
            if (GA_8_2_1_SDUINTERVAL_uS == qos_conf.sdu_interval)
            {
                if (GA_8_2_1_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 2U;
                }
                else if (GA_8_2_2_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 18U;
                }
            }
            else if (GA_16_1_1_SDUINTERVAL_uS == qos_conf.sdu_interval)
            {
                if (GA_16_1_1_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 3U;
                }
                else if (GA_16_1_2_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 19U;
                }
            }
            break;

        case GA_16_2_1_MAXSDUSIZE_BYTES:
            if (GA_16_2_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 4U;
            }
            else if (GA_16_2_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 20U;
            }
            break;

        case GA_24_1_1_MAXSDUSIZE_BYTES:
            if (GA_24_1_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 5U;
            }
            else if (GA_24_1_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 21U;
            }
            break;

        case GA_24_2_1_MAXSDUSIZE_BYTES:
            /*
             * QOS Configuration Low Latency Set 24_2_1 & 32_1_1 has same
             * Retransmission Number, but they differ in SDU Interval
             * Similarly for QOS Configuration High Reliability
             * Set 24_2_2 & 32_1_2
             */
            if (GA_24_2_1_SDUINTERVAL_uS == qos_conf.sdu_interval)
            {
                if (GA_24_2_1_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 6U;
                }
                else if (GA_24_2_2_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 22U;
                }
            }
            else if (GA_32_1_1_SDUINTERVAL_uS == qos_conf.sdu_interval)
            {
                if (GA_32_1_1_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 7U;
                }
                else if (GA_32_1_2_RTN == qos_conf.retxn)
                {
                    qos_conf_set_id = 23U;
                }
            }
            break;

        case GA_32_2_1_MAXSDUSIZE_BYTES:
            if (GA_32_2_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 8U;
            }
            else if (GA_32_2_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 24U;
            }
            break;

        case GA_441_1_1_MAXSDUSIZE_BYTES:
            if (GA_441_1_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 9U;
            }
            else if (GA_441_1_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 25U;
            }
            break;

        case GA_441_2_1_MAXSDUSIZE_BYTES:
            if (GA_441_2_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 10U;
            }
            else if (GA_441_2_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 26U;
            }
            break;

        case GA_48_1_1_MAXSDUSIZE_BYTES:
            if (GA_48_1_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 11U;
            }
            else if (GA_48_1_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 27U;
            }
            break;

        case GA_48_2_1_MAXSDUSIZE_BYTES:
            if (GA_48_2_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 12U;
            }
            else if (GA_48_2_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 28U;
            }
            break;

        case GA_48_3_1_MAXSDUSIZE_BYTES:
            if (GA_48_3_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 13U;
            }
            else if (GA_48_3_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 29U;
            }
            break;

        case GA_48_4_1_MAXSDUSIZE_BYTES:
            if (GA_48_4_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 14U;
            }
            else if (GA_48_4_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 30U;
            }
            break;

        case GA_48_5_1_MAXSDUSIZE_BYTES:
            if (GA_48_5_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 15U;
            }
            else if (GA_48_5_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 31U;
            }
            break;

        case GA_48_6_1_MAXSDUSIZE_BYTES:
            if (GA_48_6_1_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 16U;
            }
            else if (GA_48_6_2_RTN == qos_conf.retxn)
            {
                qos_conf_set_id = 32U;
            }
            break;

        default:
            break;
    }

    return qos_conf_set_id;
}

GA_RESULT appl_ga_utils_enable_legacy_advertise(UINT8 advertising_enable)
{
    GA_RESULT   retval;

    retval = GA_FAILURE;

    if (advertising_enable)
    {
        if (APPL_GA_ADV_INVALID == is_adv_enabled)
        {
            retval = BT_hci_le_set_advertising_enable
                     (
                         advertising_enable
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
        else
        {
            /* Adv already in progress */
        }
    }
    else
    {
        if (APPL_GA_ADV_INVALID != is_adv_enabled)
        {
            retval = BT_hci_le_set_advertising_enable
                     (
                         advertising_enable
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
        else
        {
            /* Adv not in Progress */
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_enable_extended_advertise(UINT8 enable)
{
    int       choice;
    UINT8     handle;
    UCHAR     random_addr[6U];
    GA_RESULT retval;
    GA_BRR_EXTENDED_ADV_PARAMS eadv_params;
    APPL_GA_GAP_ADV_DATA eadv_data;

    /* Initialize */
    handle = 1U;
    retval = GA_FAILURE;

    /* MISRA C-2012 RULE 9.1 | Coverity UNINIT  */
    GA_mem_set(&eadv_params, 0, sizeof(GA_BRR_EXTENDED_ADV_PARAMS));

    if (enable)
    {
        if (APPL_GA_ADV_INVALID == is_adv_enabled)
        {
            /* Make the properties as 1, to set it as scannable advertisement */
            eadv_params.adv_event_properties = 0x01U;
            eadv_params.adv_filter_policy = 0x00U;
            eadv_params.adv_tx_power = 0x00U;
            eadv_params.duration = 0x00U;
            eadv_params.frag_pref = 0x01U;
            eadv_params.max_ext_adv_evts = 0x00U;
            GA_mem_set(&eadv_params.peer_addr[0U], 0U, GA_BD_ADDR_SIZE);
            eadv_params.peer_addr_type = 0x00U;
            eadv_params.pri_adv_channel_map = 0x07U;
            eadv_params.pri_adv_interval_max = 0x00000020U;
            eadv_params.pri_adv_interval_min = 0x00000020U;
            eadv_params.pri_adv_phy = 0x01U;
            eadv_params.scan_req_nfy_enable = 0x00U;
            eadv_params.sec_adv_max_skip = 0x00U;
            eadv_params.sec_adv_phy = 0x01U;
            CONSOLE_OUT("Enter the Own Address Type\n");
            CONSOLE_OUT("1. Public Address\n2. Random Address\n");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                eadv_params.own_addr_type = 0x00U;
            }
            else if (2 == choice)
            {
                eadv_params.own_addr_type = 0x01U;
                CONSOLE_OUT("Enter the BD Address for Random Type: ");
                appl_get_bd_addr(random_addr);
            }

            APPL_TRC("BT_hci_le_set_extended_advertising_parameters\n");
            retval = BT_hci_le_set_extended_advertising_parameters
                     (
                         handle,
                         eadv_params.adv_event_properties,
                         eadv_params.pri_adv_interval_min,
                         eadv_params.pri_adv_interval_max,
                         eadv_params.pri_adv_channel_map,
                         eadv_params.own_addr_type,
                         eadv_params.peer_addr_type,
                         eadv_params.peer_addr,
                         eadv_params.adv_filter_policy,
                         eadv_params.adv_tx_power,
                         eadv_params.pri_adv_phy,
                         eadv_params.sec_adv_max_skip,
                         eadv_params.sec_adv_phy,
                         (handle & 0x0F), /* Derive SID from Handle for 4 bit validity */
                         eadv_params.scan_req_nfy_enable
                     );
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }

            appl_ga_utils_fetch_adv_data_for_current_role
            (
                &eadv_data,
                eadv_params.own_addr_type
            );

            APPL_TRC("BT_hci_le_set_extended_advertising_data\n");
            retval = BT_hci_le_set_extended_advertising_data
                     (
                         handle,
                         0x03, /* TODO: Assuming single packet. Update to handle fragmentation */
                         eadv_params.frag_pref,
                         eadv_data.datalen,
                         eadv_data.data
                     );
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }

            /* Set Random Address if the user choice is random address type */
            if (2 == choice)
            {
                APPL_TRC("BT_hci_le_set_adv_set_random_address\n");
                retval = BT_hci_le_set_adv_set_random_address
                         (
                             handle,
                             random_addr
                         );
                APPL_TRC("Retval - ");
                if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
                {
                    APPL_TRC("0x%04X\n", retval);
                }
            }

            APPL_TRC("BT_hci_le_set_extended_advertising_enable\n");
            retval = BT_hci_le_set_extended_advertising_enable
                     (
                         0x01U,
                         0x01U,
                         &handle,
                         &eadv_params.duration,
                         &eadv_params.max_ext_adv_evts
                     );
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
        }
        else
        {
            /* Adv already in progress */
        }
    }
    else
    {
        if (APPL_GA_ADV_INVALID != is_adv_enabled)
        {
            /* Disable */
            APPL_TRC("BT_hci_le_set_extended_advertising_enable\n");
            retval = BT_hci_le_set_extended_advertising_enable
                     (
                         0x00U,
                         0x01U,
                         &handle,
                         &eadv_params.duration,
                         &eadv_params.max_ext_adv_evts
                     );
            APPL_TRC("Retval - ");
            if (GA_SUCCESS != appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }
        }
        else
        {
            /* Adv not in Progress */
        }
    }

    return retval;
}

void appl_ga_utils_set_adv_data_for_current_tmap_role(APPL_GA_ROLE tmap_role)
{
    UINT8 marker;
    PACS_CAP_INFO local_pacs_info;

    /* Initialize */
    marker = 0U;

    /* Bytes 7-8 refers to TMAP Role. Update the role */
    marker = 7U;
    GA_PACK_LE_2_BYTE_VAL(&appl_ga_gap_adv_data_tmap.data[marker], tmap_role);
    /*
     * Fetch the current appl_ga_gap_adv_data_tmap.datalen and append
     * from the last byte.
     * Append the variable length parameters.
     * Update the appl_ga_gap_adv_data_tmap.datalen.
     */
    marker = appl_ga_gap_adv_data_tmap.datalen;
    if (APPL_GA_TMAP_ROLE_CG & tmap_role)
    {
        /**
         * CCP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + 2U = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Generic Telephone Bearer Service UUID: 0x184C (GA_SERVICE_GTBS)
         * 0x03U, 0x16U, 0x4CU, 0x18U
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_GTBS
        );
        marker += 2U;
    }
    if (APPL_GA_TMAP_ROLE_UMS & tmap_role)
    {
        /**
         * MCP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U) = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Generic Media Control Service UUID: 0x1849 (GA_SERVICE_GMCS)
         * 0x03U, 0x16U, 0x49U, 0x18U
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_GMCS
        );
        marker += 2U;
    }
    /*
     * Since all the below TMAP Roles include VCP in their adv data, the VCP
     * data info is packed only once.
     */
    if ((APPL_GA_TMAP_ROLE_CT & tmap_role) || (APPL_GA_TMAP_ROLE_UMR & tmap_role) ||
        (APPL_GA_TMAP_ROLE_BMR & tmap_role))
    {
        /**
         * VCP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U) = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Volume Control Service UUID: 0x1844 (GA_SERVICE_VCS)
         * 0x03U, 0x16U, 0x44U, 0x18U
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_VCS
        );
        marker += 2U;
    }
    /*
     * Since all the below TMAP Roles include BAP in their adv data, the BAP
     * data info is packed only once
     */
    if ((APPL_GA_TMAP_ROLE_CT & tmap_role) || (APPL_GA_TMAP_ROLE_UMR & tmap_role))
    {
        /**
         * BAP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U + 4U + 1U) = 0x09U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Audio Stream Control Service UUID: 0x184E (GA_SERVICE_ASCS)
         *                 1U         Announcement Type: 0x00U (BAP_ASCS_GENERAL_ANNOUNCEMENT)
         *                            Announcement Type: 0x01U (BAP_ASCS_TARGETED_ANNOUNCEMENT)
         *                 4U         Available Audio Contexts: For Sink and Source PAC.
         *                 1U         Metadata Length: 0U (TODO: Update metadata if metadata_length is not zero)
         *                 Varies     Metadata: NULL (TODO: Update metadata if metadata_length is not zero)
         * 0x04U, 0x16U, 0x4EU, 0x18U, 0x00
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x09U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_ASCS
        );
        marker += 2U;
        /* Announcement Type */
        appl_ga_gap_adv_data_tmap.data[marker] = BAP_ASCS_GENERAL_ANNOUNCEMENT;
        marker++;
        /* Fetching local PACS info to pack available audio contexts */
        appl_ga_utils_bap_fetch_local_pacs(&local_pacs_info);
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            local_pacs_info.record.avail_sink_contexts
        );
        marker += 2U;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            local_pacs_info.record.avail_src_contexts
        );
        marker += 2U;
        appl_ga_gap_adv_data_tmap.data[marker] = 0x00U;
        marker++;
        /* TODO: Append Metadata Info if the metadata_length is not zero */
    }
    /*
     * Since all the below TMAP Roles include CAP in their adv data, the CAP
     * data info is packed only once
     */
    if ((APPL_GA_TMAP_ROLE_CG & tmap_role) ||
        (APPL_GA_TMAP_ROLE_UMS & tmap_role) ||
        (APPL_GA_TMAP_ROLE_BMR & tmap_role))
    {
        /**
         *  CAP Announcement
         *  Service Data 16 bit UUID:
         *  ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U) = 0x04U
         *  ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         *  ADData:   Size: 2U, Value: Common Audio Service UUID: 0x1853 (GA_SERVICE_CAS)
         *                  1U         Announcement Type: 0x00U (CAS_GENERAL_ANNOUNCEMENT)
         *                             Announcement Type: 0x01U (CAS_TARGETED_ANNOUNCEMENT)
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x04U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_CAS
        );
        marker += 2U;
        /* Announcement Type */
        appl_ga_gap_adv_data_tmap.data[marker] = CAS_GENERAL_ANNOUNCEMENT;
        marker++;
    }
    if (APPL_GA_TMAP_ROLE_BMR & tmap_role)
    {
        /*
         * As per BAP Spec:
         * Service UUID AD data type(as defined in Part A, Section 1.1 in[3])
         * containing the ASCS[4] UUID if ASCS is supported over LE, and /or
         * the BASS[6] UUID if BASS is supported over LE, and the PACS[5] UUID
         * if PACS is supported over LE If the Peripheral is in a GAP
         * Discoverable mode.
         * BMR: Sink + SD Server [BASS]
         */
        appl_ga_gap_adv_data_tmap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_tmap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_tmap.data[marker],
            GA_SERVICE_BASS
        );
        marker += 2U;
    }
    /* Update the adv datalen from the marker */
    appl_ga_gap_adv_data_tmap.datalen = marker;

    return;
}

void appl_ga_utils_set_adv_data_for_current_hap_role
     (
         APPL_GA_ROLE  hap_role,
         UINT8         hap_features
     )
{
    UINT8 marker;
    PACS_CAP_INFO local_pacs_info;
    GA_RESULT retval;

    /*
     * Used for RSI AD_Type when HAP features has Hearing Aid Type - Binaural
     *
     * NOTE: Set the same SIRK value while registering CSIP
     */
    UCHAR csip_sirk_value[16U] = {0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU};
    UCHAR rsi[6U] = { 0x00U };

    /* Initialize */
    marker = 0U;

    /*
     * Steps followed
     * Fetch the current appl_ga_gap_adv_data_hap.datalen and append from the last byte.
     * Append the variable length parameters.
     * Update the appl_ga_gap_adv_data_hap.datalen.
     */
    marker = appl_ga_gap_adv_data_hap.datalen;

    if (APPL_GA_HAP_ROLE_HA & hap_role)
    {
        /**
         * CAP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U) = 0x04U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Common Audio Service UUID: 0x1853 (GA_SERVICE_CAS)
         *                 1U         Announcement Type: 0x00U (CAS_GENERAL_ANNOUNCEMENT)
         *                            Announcement Type: 0x01U (CAS_TARGETED_ANNOUNCEMENT)
         *
         * 0x04U, 0x16U, 0x53U, 0x18U, 0x00U,
         */
        appl_ga_gap_adv_data_hap.data[marker] = 0x04U;
        marker++;
        appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            GA_SERVICE_CAS
        );
        marker += 2U;
        appl_ga_gap_adv_data_hap.data[marker] = 0x00U;
        marker++;

        /**
         * VCP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U) = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Volume Control Service UUID: 0x1844 (GA_SERVICE_VCS)
         * 0x03U, 0x16U, 0x44U, 0x18U
         */
        appl_ga_gap_adv_data_hap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            GA_SERVICE_VCS
        );
        marker += 2U;

        /**
         * MICP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U) = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Microphone Control Service UUID: 0x184D (GA_SERVICE_MICS)
         * 0x03U, 0x16U, 0x4DU, 0x18U
         */
        appl_ga_gap_adv_data_hap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            GA_SERVICE_MICS
        );
        marker += 2U;

        /*
         * Binaural Hearing Aid - HAP Features Bits 0-1.
         * Append RSI AD_Type and the info.
         */
        if (0x00U == (hap_features & 0x03U))
        {
            APPL_TRC("Generating RSI...\n");
            retval = BT_smp_create_resolvable_pvt_addr(csip_sirk_value);
            APPL_TRC("Retval - ");
            if (GA_FAILURE == appl_ga_utils_display_ga_result(retval))
            {
                APPL_TRC("0x%04X\n", retval);
            }

            CONSOLE_OUT("Enter the rsi to append to the adv data: ");
            appl_input_octets(rsi, 6U);

            /**
             * CSIP Announcement
             * Service Data 16 bit UUID:
             * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (6U) = 0x07U
             * ADType:   Size: 1U, Value: 0x2EU (Resolvable Set Identifier)
             * ADData:   Size: 6U, Value: RSI
             * 0x07U, 0x2EU, 6 Bytes Data for RSI
             */
            appl_ga_gap_adv_data_hap.data[marker] = 0x07U;
            marker++;

            appl_ga_gap_adv_data_hap.data[marker] = 0x2EU;
            marker++;

            GA_mem_copy
            (
                &appl_ga_gap_adv_data_hap.data[marker],
                &rsi[0U],
                6U
            );
            marker += 6U;
        }

       /**
         * BAP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + (2U + 1U + 4U + 1U) = 0x09U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Audio Stream Control Service UUID: 0x184E (GA_SERVICE_ASCS)
         *                 1U         Announcement Type: 0x00U (BAP_ASCS_GENERAL_ANNOUNCEMENT)
         *                            Announcement Type: 0x01U (BAP_ASCS_TARGETED_ANNOUNCEMENT)
         *                 4U         Available Audio Contexts: For Sink and Source PAC.
         *                 1U         Metadata Length: 0U (TODO: Update metadata if metadata_length is not zero)
         *                 Varies     Metadata: NULL (TODO: Update metadata if metadata_length is not zero)
         * 0x04U, 0x16U, 0x4EU, 0x18U, 0x00
         */
        appl_ga_gap_adv_data_hap.data[marker] = 0x09U;
        marker++;
        appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            GA_SERVICE_ASCS
        );
        marker += 2U;
        appl_ga_gap_adv_data_hap.data[marker] = 0x00U;
        marker++;
        /* Fetching local PACS info to pack available audio contexts */
        appl_ga_utils_bap_fetch_local_pacs(&local_pacs_info);
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            local_pacs_info.record.avail_sink_contexts
        );
        marker += 2U;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            local_pacs_info.record.avail_src_contexts
        );
        marker += 2U;
        appl_ga_gap_adv_data_hap.data[marker] = 0x00U;
        marker++;
        /* TODO: Append Metadata Info if the metadata_length is not zero */
    }
    else if (APPL_GA_HAP_ROLE_HAUC & hap_role)
    {
        /**
         * CCP Announcement
         * Service Data 16 bit UUID:
         * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + 2U = 0x03U
         * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
         * ADData:   Size: 2U, Value: Generic Telephone Bearer Service UUID: 0x184C (GA_SERVICE_GTBS)
         * 0x03U, 0x16U, 0x4CU, 0x18U
         */
        appl_ga_gap_adv_data_hap.data[marker] = 0x03U;
        marker++;
        appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
        marker++;
        GA_PACK_LE_2_BYTE_VAL
        (
            &appl_ga_gap_adv_data_hap.data[marker],
            GA_SERVICE_GTBS
        );
        marker += 2U;
    }
    /* Update the adv datalen from the marker */
    appl_ga_gap_adv_data_hap.datalen = marker;

    return;
}

void appl_ga_utils_add_has_info_to_hap_adv_data(void)
{
    UINT8 marker;

    marker = appl_ga_gap_adv_data_hap.datalen;

    /**
     * HAP Announcement
     * Service Data 16 bit UUID:
     * ADLength: Size: 1U, Value: ADType Len + ADValue Len = 1U + 2U = 0x03U
     * ADType:   Size: 1U, Value: 0x16U (HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS)
     * ADData:   Size: 2U, Value: Hearing Access Service UUID: 0x1854U (GA_SERVICE_HAS)
     * 0x03U, 0x16U, 0x54U, 0x18U
     *
     * NOTE: Not adding appearance type AD_Type as it is optional, to enhance UI
     */
    appl_ga_gap_adv_data_hap.data[marker] = 0x03U;
    marker++;
    appl_ga_gap_adv_data_hap.data[marker] = GA_ADTYPE_SERVICE_DATA;
    marker++;
    GA_PACK_LE_2_BYTE_VAL
    (
        &appl_ga_gap_adv_data_hap.data[marker],
        GA_SERVICE_HAS
    );
    marker += 2U;

    appl_ga_gap_adv_data_hap.datalen = marker;

    return;
}

void appl_ga_utils_fetch_adv_data_for_current_role
     (
         APPL_GA_GAP_ADV_DATA      * eadv_data,
         UINT8                       own_addr_type
     )
{
    UINT8 marker;

    /* Initialize */
    marker = 0U;

    /* GA Role can have more than 1 role (e.g., TMAP can have CG and UMR) */
    if ((APPL_GA_TMAP_ROLE_CG & ga_role) || (APPL_GA_TMAP_ROLE_CT & ga_role) ||
        (APPL_GA_TMAP_ROLE_UMS & ga_role) || (APPL_GA_TMAP_ROLE_UMR & ga_role) ||
        (APPL_GA_TMAP_ROLE_BMS & ga_role) || (APPL_GA_TMAP_ROLE_BMR & ga_role))
    {
        GA_mem_copy
        (
            eadv_data->data,
            appl_ga_gap_adv_data_tmap.data,
            appl_ga_gap_adv_data_tmap.datalen
        );
        eadv_data->datalen = appl_ga_gap_adv_data_tmap.datalen;
    }
    else if ((APPL_GA_HAP_ROLE_HA & ga_role) || (APPL_GA_HAP_ROLE_HAUC & ga_role) ||
             (APPL_GA_HAP_ROLE_HARC & ga_role))
    {
        /*
         * If we are advertising with public address,
         * include HAS service UUID in the adv_data. Otherwise dont.
         */
        if ((APPL_GA_HAP_ROLE_HA & ga_role) && (0x01U == own_addr_type))
        {
            appl_ga_utils_add_has_info_to_hap_adv_data();
        }
        GA_mem_copy
        (
            eadv_data->data,
            appl_ga_gap_adv_data_hap.data,
            appl_ga_gap_adv_data_hap.datalen
        );
        eadv_data->datalen = appl_ga_gap_adv_data_hap.datalen;
    }
    else
    {
        GA_mem_copy
        (
            eadv_data->data,
            appl_ga_gap_adv_data.data,
            appl_ga_gap_adv_data.datalen
        );
        eadv_data->datalen = appl_ga_gap_adv_data.datalen;
    }

    return;
}

GA_RESULT appl_ga_utils_enable_legacy_scan(UINT8 scan_enable)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

    if (scan_enable)
    {
        if (APPL_GA_SCAN_INVALID == is_scan_enabled)
        {
            retval = BT_hci_le_set_scan_enable
                     (
                         scan_enable,
                         0x01U /* Duplicate Filtering disabled */
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
        else
        {
            /* Scan already in progress */
        }
    }
    else
    {
        if (APPL_GA_SCAN_INVALID != is_scan_enabled)
        {
            retval = BT_hci_le_set_scan_enable
                     (
                         scan_enable,
                         0x01U /* Duplicate Filtering disabled */
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
        else
        {
            /* Scan not in Progress */
        }
    }

    return retval;
}

GA_RESULT appl_ga_utils_enable_extended_scan(GA_BRR_EXTENDED_SCAN_PARAMS escan_params)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

    if (APPL_GA_SCAN_INVALID == is_scan_enabled)
    {
        /* Reset all the devices in DB */
        appl_ga_utils_hci_clear_ea_db();

        retval = BT_hci_le_set_extended_scan_parameters
                 (
                     escan_params.own_addr_type,
                     escan_params.scan_filter_policy,
                     escan_params.scan_phy,
                     &escan_params.scan_type,
                     &escan_params.scan_interval,
                     &escan_params.scan_window
                 );

        if (GA_SUCCESS == retval)
        {
            retval = BT_hci_le_set_extended_scan_enable
                     (
                         0x01,
                         escan_params.filter_duplicates,
                         escan_params.duration,
                         escan_params.period
                     );
        }
    }
    else
    {
        /* Scan already in Progress */
    }

    return retval;
}

GA_RESULT appl_ga_utils_disable_extended_scan(GA_BRR_EXTENDED_SCAN_PARAMS escan_params)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

    if (APPL_GA_SCAN_INVALID != is_scan_enabled)
    {
        retval = BT_hci_le_set_extended_scan_enable
                 (
                     0x00,
                     escan_params.filter_duplicates,
                     escan_params.duration,
                     escan_params.period
                 );
    }
    else
    {
        /* Scan not in Progress */
    }

    return retval;
}

void appl_ga_utils_ucs_ascs_display_common_event_info
     (
        GA_ENDPOINT * device,
        UINT16        ga_status
     )
{
    APPL_DBG("[APPL][GA][UCS]: >> appl_ga_utils_ucs_ascs_display_common_event_info\n");

    APPL_TRC("[Profile]     : ASCS (0x%04X)\n", GA_SERVICE_ASCS);
    APPL_TRC("[SECTION]     : ASCS SERVER EVENTS\n");
    APPL_TRC("[SUB-SECTION] : ASCS SERVER-WRITE\n");
    APPL_TRC("[TYPE]        : WRITE REQUEST\n");
    appl_ga_utils_display_bdaddr_and_type(device);
    APPL_TRC("[Event Status]: ");
    appl_ga_utils_display_ga_result(ga_status);

    APPL_DBG("[APPL][GA][UCS]: << appl_ga_utils_ucs_ascs_display_common_event_info\n");

    return;
}

void appl_ga_utils_ucc_ascs_display_common_event_info
     (
        GA_ENDPOINT * device,
        UINT8         ga_event,
        UINT16        ga_status
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_display_common_event_info\n");

    APPL_TRC("[Profile]     : ASCS (0x%04X)\n", GA_SERVICE_ASCS);
    APPL_TRC("[SECTION]     : ASCS CLIENT EVENTS\n");
    appl_ga_utils_ucc_ascs_display_event_type(ga_event);
    appl_ga_utils_display_bdaddr_and_type(device);
    APPL_TRC("[Event Status]: ");
    appl_ga_utils_display_ga_result(ga_status);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_display_common_event_info\n");

    return;
}

void appl_ga_utils_ucc_ascs_display_event_type(UINT8 ga_event)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_display_event_type\n");

    switch(ga_event)
    {
        case GA_SEP_DISCOVER_CNF:
            APPL_TRC("[SUB-SECTION] : ASCS CLIENT-SETUP\n");
            APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            break;

        case GA_SEP_CONFIGURE_NTF: /* Fall - through */
        case GA_SEP_SETUP_NTF: /* Fall - through */
        case GA_SEP_RX_START_READY_NTF: /* Fall - through */
        case GA_SEP_UPDATE_NTF: /* Fall - through */
        case GA_SEP_SUSPEND_NTF: /* Fall - through */
        case GA_SEP_RELEASE_NTF: /* Fall - through */
        case GA_SEP_IDLE_NTF:
            APPL_TRC("[SUB-SECTION] : ASCS CLIENT-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            break;

        case GA_SEP_CONFIGURE_CNF: /* Fall - through */
        case GA_SEP_SETUP_CNF: /* Fall - through */
        case GA_SEP_START_CNF: /* Fall - through */
        case GA_SEP_RX_START_READY_CNF: /* Fall - through */
        case GA_SEP_RX_STOP_READY_CNF: /* Fall - through */
        case GA_SEP_UPDATE_CNF: /* Fall - through */
        case GA_SEP_SUSPEND_CNF: /* Fall - through */
        case GA_SEP_RELEASE_CNF:
            APPL_TRC("[SUB-SECTION] : ASCS CLIENT-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            break;

        default:
            /* Should not reach here */
            break;
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_display_event_type\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_ase_data(UCHAR * data, UINT16 datalen)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_ase_data\n");

    if (NULL == data)
    {
        APPL_ERR("data is NULL !\n");
        return;
    }

    APPL_TRC
    (
        "Length: 0x%02X,  ASE ID: 0x%02X\n",
        (UCHAR)sizeof(data[0]), data[0U]
    );
    APPL_TRC("Length: 0x%02X,  ASE State: ", (UCHAR)sizeof(data[1U]));
    appl_ga_utils_bap_parse_and_display_ase_state(data[1U]);
    APPL_TRC
    (
        "Length: 0x%02X,  Additional Parameters: ",
        (datalen - 2U)
    );

    if ((SEP_STATE_IDLE == data[1U]) || (SEP_STATE_RELEASING == data[1U]))
    {
        APPL_TRC("NULL\n");
    }
    else if (SEP_STATE_CODEC_CONFIGURED == data[1U])
    {
        APPL_TRC("\n");
        appl_ga_utils_ucc_ascs_parse_and_display_ase_params_codec_cnf
        (
            (data + 2U),
            (datalen - 2U),
            0U
        );
    }
    else if (SEP_STATE_QOS_CONFIGURED == data[1U])
    {
        APPL_TRC("\n");
        appl_ga_utils_ucc_ascs_parse_and_display_ase_params_qos_cnf
        (
            (data + 2U),
            (datalen - 2U)
        );
    }
    else if ((SEP_STATE_ENABLING == data[1U]) ||
             (SEP_STATE_STREAMING == data[1U]) ||
             (SEP_STATE_DISABLING == data[1U]))
    {
        APPL_TRC("\n");
        appl_ga_utils_ucc_ascs_parse_and_display_ase_params_enabling_streaming_disabling
        (
            (data + 2U),
            (datalen - 2U)
        );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_ase_data\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_enabling_streaming_disabling
     (
         UCHAR * data,
         UINT16  datalen
     )
{
    UCHAR marker;
    UCHAR metadata_len;

    /* Initialize */
    marker = 0U;
    metadata_len = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_ase_params_enabling_streaming_disabling\n");

    if (data == NULL)
    {
        APPL_ERR("data is NULL !\n");
        return;
    }

    /* Display of Additional_ASE Parameters */
    /* Refer ASCS_v1.0, Page 22, Table 4.5 */

    /* Minimum M fields Length */
    if (3U > datalen)
    {
        APPL_ERR("Data Length Mismatch !\n");
    }

    APPL_TRC
    (
        "\tLen: 0x%02X,  CIG ID: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  CIS ID: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    metadata_len = data[marker];
    APPL_TRC("\tLen: 0x%02X,  Metadata Length: 0x%02X\n", metadata_len, metadata_len);
    marker += 1U;

    if (0x00U != metadata_len)
    {
        APPL_TRC("\tLen: 0x%02X,  Metadata:\n", metadata_len);

        appl_ga_utils_bap_parse_and_display_metadata_ltv
        (
            &data[marker],
            metadata_len,
            NULL,
            NULL
        );
    }
    else
    {
        APPL_TRC("Len: 0x%02X,  Metadata: NULL\n", metadata_len);
    }
    marker += metadata_len;

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_ase_params_enabling_streaming_disabling\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_qos_cnf
     (
         UCHAR * data,
         UINT16  datalen
     )
{
    UINT8   marker;
    UINT8   codec_spec_cnf_len;
    UINT16  value_2b;
    UINT32  value_4b;

    /* Initialiaze */
    marker = 0U;
    codec_spec_cnf_len = 0U;
    value_2b = 0U;
    value_4b = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_ase_params_qos_cnf\n");

    if (NULL == data)
    {
        APPL_ERR("data is NULL !\n");
        return;
    }

    /* Display of Additional_ASE Parameters */
    /* Refer ASCS_v1.0, Page 20, Table 4.4 */

    /* Minimum M fields Length */
    if (15U != datalen)
    {
        APPL_ERR("Data Length Mismatch !\n");
    }

    APPL_TRC
    (
        "\tLen: 0x%02X,  CIG ID: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  CIS ID: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  SDU Interval: 0x%06X ",
        (UCHAR)sizeof(data[marker]), value_4b
    );
    if ((0x0000FFU > value_4b) || (0x0FFFFFU < value_4b))
    {
        APPL_TRC("(RFU)");
    }
    APPL_TRC("\n");
    marker += 3U;

    APPL_TRC("\tLen: 0x%02X,  Framing: ", (UCHAR)sizeof(data[marker]));
    appl_ga_utils_bap_display_framing(data[marker]);
    marker += 1U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  PHY: 0x%02X",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    appl_ga_utils_bap_display_pref_phy(data[marker]);
    marker += 1U;

    GA_UNPACK_LE_2_BYTE(&value_2b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  MAX SDU: 0x%06X\n",
        (UCHAR)sizeof(data[marker]), value_2b
    );
    marker += 2U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  Retransmission Number: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    GA_UNPACK_LE_2_BYTE(&value_2b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Max Transport Latency: 0x%04X(%dms)\n",
        (UCHAR)sizeof(value_2b), value_2b, value_2b
    );
    marker += 2U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Presentation Delay: 0x%06X(%dus)\n",
        (UCHAR)sizeof(UINT8) + (UCHAR)sizeof(UINT16), value_4b, value_4b
    );
    marker += 3U;

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_ase_params_qos_cnf\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_codec_cnf
     (
         UCHAR * data,
         UINT16  datalen,
         UINT8   capability
     )
{
    UINT8          marker;
    UINT8          codec_spec_cnf_len;
    UINT16         value_2b;
    UINT32         value_4b;
    GA_CODEC_INFO  codec_id = { 0 };

    /* Initialiaze */
    marker = 0U;
    codec_spec_cnf_len = 0U;
    value_2b = 0U;
    value_4b = 0U;

    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_ase_params_codec_cnf\n");

    if (NULL == data)
    {
        APPL_ERR("data is NULL !\n");
        return;
    }

    /* Display of Additional_ASE Parameters */
    /* Refer ASCS_v1.0, Page 18, Table 4.3 */

    /* Minimum M fields Length */
    if (23U > datalen)
    {
        APPL_ERR("Data Length Mismatch !\n");
    }

    APPL_TRC("\tLen: 0x%02X,  Framing: ", (UCHAR)sizeof(data[marker]));
    appl_ga_utils_ucc_ascs_parse_and_display_isoal_framing(data[marker]);
    marker += 1U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  Preferred PHY: 0x%02X",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    appl_ga_utils_bap_display_pref_phy(data[marker]);
    marker += 1U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  Preferred Retransmission Number: 0x%02X\n",
        (UCHAR)sizeof(data[marker]), data[marker]
    );
    marker += 1U;

    GA_UNPACK_LE_2_BYTE(&value_2b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Max Transport Latency: 0x%04X(%dms)\n",
        (UCHAR)sizeof(value_2b), value_2b, value_2b
    );
    marker += 2U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Presentation Delay Min: 0x%06X(%dus)\n",
        (UCHAR)sizeof(UINT8) + (UCHAR)sizeof(UINT16), value_4b, value_4b
    );
    marker += 3U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Presentation Delay Max: 0x%06X(%dus)\n",
        (UCHAR)sizeof(UINT8) + (UCHAR)sizeof(UINT16), value_4b, value_4b
    );
    marker += 3U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Preferred Presentation Delay Min: ",
        (UCHAR)sizeof(UINT8) + (UCHAR)sizeof(UINT16)
    );
    if (0U == value_4b)
    {
        APPL_TRC("No preference(0x%06X)\n", value_4b);
    }
    else
    {
        APPL_TRC("0x%06X\n", value_4b);
    }
    marker += 3U;

    GA_UNPACK_LE_3_BYTE(&value_4b, &data[marker]);
    APPL_TRC
    (
        "\tLen: 0x%02X,  Preferred Presentation Delay Max: ",
        (UCHAR)sizeof(UINT8) + (UCHAR)sizeof(UINT16)
    );
    if (0U == value_4b)
    {
        APPL_TRC("No preference(0x%06X)\n", value_4b);
    }
    else
    {
        APPL_TRC("0x%06X\n", value_4b);
    }
    marker += 3U;

    APPL_TRC
    (
        "\tLen: 0x%02X,  Codec ID:\n",
        (UCHAR)(sizeof(UINT8) + sizeof(UINT16) + sizeof(UINT16))
    );
    appl_ga_utils_bap_parse_and_fill_codec_id
    (
        &data[marker],
        (sizeof(UINT8) + sizeof(UINT16) + sizeof(UINT16)),
        &codec_id
    );
    /* Size of Codec ID is 5 bytes */
    marker += 5U;

    if (1U == capability)
    {
        APPL_TRC
        (
            "\tLen: 0x%02X,  Codec Specific Capability Length: 0x%02X\n",
            (UCHAR)sizeof(data[marker]), data[marker]
        );
        GA_UNPACK_LE_1_BYTE(&codec_spec_cnf_len, &data[marker]);
        marker += 1U;

        APPL_TRC
        (
            "\tLen: 0x%02X,  Codec Specific Capability:\n",
            codec_spec_cnf_len
        );
        appl_ga_utils_bap_parse_and_display_codec_spec_cap_ltv
        (
            &data[marker],
            codec_spec_cnf_len,
            NULL
        );
    }
    else
    {
        APPL_TRC
        (
            "\tLen: 0x%02X,  Codec Specific Configuration Length: 0x%02X\n",
            (UCHAR)sizeof(data[marker]), data[marker]
        );
        GA_UNPACK_LE_1_BYTE(&codec_spec_cnf_len, &data[marker]);
        marker += 1U;

        APPL_TRC
        (
            "\tLen: 0x%02X,  Codec Specific Configuration:\n",
            codec_spec_cnf_len
        );
        appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
        (
            &data[marker],
            codec_spec_cnf_len,
            NULL
        );
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_ase_params_codec_cnf\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_isoal_framing(UINT8 framing)
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_isoal_framing\n");

    switch(framing)
    {
        case GA_FRAMING_UNFRAMED_ISOAL_PDU_SUPP:
            APPL_TRC("Unframed ISOAL PDUs supported");
            break;

        case GA_FRAMING_UNFRAMED_ISOAL_PDU_NOT_SUPP:
            APPL_TRC("Unframed ISOAL PDUs not supported");
            break;

        default:
            APPL_TRC("RFU");
    }
    APPL_TRC(" (0x%02X)\n", framing);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_isoal_framing\n");
    return;
}

void appl_ga_utils_ucc_ascs_display_cp_event_data
     (
         UINT16        event_status,
         GA_ASE_INFO * ase
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_display_cp_event_data\n");

    APPL_TRC("\n");
    appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);
    if (GA_SUCCESS == event_status)
    {
        APPL_TRC("SEP Context ID - 0x%02X\n", ase->sep_id);
        APPL_TRC("Data Length: 0x%02X\n", ase->datalen);
        APPL_TRC("Data:\n");
        appl_ga_utils_ucc_ascs_parse_and_display_ase_data
        (
            ase->data,
            ase->datalen
        );
    }
    else
    {
        APPL_TRC("SEP Context ID - 0x%02X\n", ase->sep_id);
        APPL_TRC("Data Length: 0x%02X\n", ase->datalen);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Length: 0x%02X,  ASE ID: 0x%02X\n",
            (UCHAR)sizeof(ase->data[0U]), ase->data[0U]
        );
        APPL_TRC
        (
            "Length: 0x%02X,  ASE Response:",
            (UCHAR)sizeof(ase->data[1U])
        );
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_rsp_rsn_code(ase->data[1U], ase->data[2U]);
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_display_cp_event_data\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_asecp_rsp_rsn_code
     (
        UINT8 asecp_rsp_code,
        UINT8 asecp_rsn_code
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_asecp_rsp_rsn_code\n");

    switch (asecp_rsp_code)
    {
    case GA_SEP_RSP_SUCCESS:
        APPL_TRC(" Success (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_UNSUPP_OPCODE:
        APPL_TRC(" Unsupported Opcode (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_INVALID_LENGTH:
        APPL_TRC(" Invalid Length (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_INVALID_ASEID:
        APPL_TRC(" Invalid ASE_ID (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_INVALID_ASE_SM_TRANSITION:
        APPL_TRC
        (
            " Invalid ASE State Machine Transition (0x%02X)\n",
            asecp_rsp_code
        );
        break;

    case GA_SEP_RSP_INVALID_ASE_DIRECTION:
        APPL_TRC(" Invalid ASE direction (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_UNSUPP_AUDIO_CAPABILITIES:
        APPL_TRC
        (
            " Unsupported  Audio Capabilities (0x%02X)\n",
            asecp_rsp_code
        );
        break;

    case GA_SEP_RSP_UNSUPP_CONFIG_PARAM_VAL:
        APPL_TRC
        (
            " Unsupported Configuration Parameter Value (0x%02X)\n",
            asecp_rsp_code
        );
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_REJECTED_CONFIG_PARAM_VAL:
        APPL_TRC
        (
            " Rejected Configuration Parameter Value (0x%02X)\n",
            asecp_rsp_code
        );
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_INVALID_CONFIG_PARAM_VAL:
        APPL_TRC
        (
            " Invalid Configuration Parameter Value (0x%02X)\n",
            asecp_rsp_code
        );
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_UNSUPP_METADATA:
        APPL_TRC(" Unsupported Metadata (0x%02X)\n", asecp_rsp_code);
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_REJECTED_METADATA:
        APPL_TRC(" Rejected Metadata (0x%02X)\n", asecp_rsp_code);
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_INVALID_METADATA:
        APPL_TRC(" Invalid Metadata (0x%02X)\n", asecp_rsp_code);
        appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason
        (
            asecp_rsn_code
        );
        break;

    case GA_SEP_RSP_INSUFFICIENT_RESOURCES:
        APPL_TRC(" Insufficient Resources (0x%02X)\n", asecp_rsp_code);
        break;

    case GA_SEP_RSP_UNSPECIFIED_ERROR:
        APPL_TRC(" Unspecified Error (0x%02X)\n", asecp_rsp_code);
        break;

    default:
        APPL_TRC(" Unknown Response Code (0x%02X)\n", asecp_rsp_code);
        break;
    }

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_asecp_rsp_rsn_code\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason
     (
        UINT8 asecp_rsn_code
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason\n");

    APPL_TRC
    (
        "Length: 0x%02X,  ASE Reason:",
        (UCHAR)sizeof(asecp_rsn_code)
    );

    switch (asecp_rsn_code)
    {
    case GA_SEP_RSP_RSN_NONE:
        APPL_TRC(" None");
        break;

    case GA_SEP_RSP_RSN_CODEC_ID:
        APPL_TRC(" Codec_ID");
        break;

    case GA_SEP_RSP_RSN_CODEC_SPECIFIC_CONFIG:
        APPL_TRC(" Codec_Specific_Configuration");
        break;

    case GA_SEP_RSP_RSN_SDU_INTERVAL:
        APPL_TRC(" SDU_Interval");
        break;

    case GA_SEP_RSP_RSN_FRAMING:
        APPL_TRC(" Framing");
        break;

    case GA_SEP_RSP_RSN_PHY:
        APPL_TRC(" PHY");
        break;

    case GA_SEP_RSP_RSN_MAX_SDU_SIZE:
        APPL_TRC(" Maximum_SDU_Size");
        break;

    case GA_SEP_RSP_RSN_RETRANSMISSION_NUMBER:
        APPL_TRC(" Retransmission_Number");
        break;

    case GA_SEP_RSP_RSN_MAX_TRANSPORT_LATENCY:
        APPL_TRC(" Max_Transport_Latency");
        break;

    case GA_SEP_RSP_RSN_PRESENTATION_DELAY:
        APPL_TRC(" Presentation_Delay");
        break;

    case GA_SEP_RSP_RSN_INVALID_ASE_CIS_MAPPING:
        APPL_TRC(" Invalid_ASE_CIS_Mapping");
        break;

    default:
        APPL_TRC(" Unknown Response Reason Code");
        break;
    }

    APPL_TRC(" (0x%02X)\n", asecp_rsn_code);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason\n");
    return;
}

void appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason
     (
         UINT8 asecp_rsn_code
     )
{
    APPL_DBG("[APPL][GA][UCC]: >> appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason\n");

    APPL_TRC
    (
        "Length: 0x%02X,  ASE Reason:",
        (UCHAR)sizeof(asecp_rsn_code)
    );

    switch (asecp_rsn_code)
    {
    case GA_LTV_METADATA_PREF_AC_TYPE:
        APPL_TRC(" Preferred Audio Contexts");
        break;
    case GA_LTV_METADATA_STREAMING_AC_TYPE:
        APPL_TRC(" Streaming Audio Contexts");
        break;
    case GA_LTV_METADATA_PGM_INFO_TYPE:
        APPL_TRC(" Program Info");
        break;
    case GA_LTV_METADATA_LANGUAGE_TYPE:
        APPL_TRC(" Language");
        break;
    case GA_LTV_METADATA_CCID_LIST_TYPE:
        APPL_TRC(" CCID List");
        break;
    case GA_LTV_METADATA_PARENTAL_RATING_TYPE:
        APPL_TRC(" Parental Rating");
        break;
    case GA_LTV_METADATA_PROGRAM_INFO_URL_TYPE:
        APPL_TRC(" Program Info URL");
        break;
    case GA_LTV_METADATA_EM_TYPE:
        APPL_TRC(" Extended Metadata");
        break;
    case GA_LTV_METADATA_VS_TYPE:
        APPL_TRC(" Vendor Specific");
        break;

    default:
        APPL_TRC(" Unknown Response Reason Code");
        break;
    }

    APPL_TRC(" (0x%02X)\n", asecp_rsn_code);

    APPL_DBG("[APPL][GA][UCC]: << appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason\n");
    return;
}

void appl_ga_utils_bc_display_src_console_header
     (
         GA_ENDPOINT * device,
         UINT16        ga_status
     )
{
    APPL_TRC("[Profile]     : CAP \n");
    APPL_TRC("[SECTION]     : BCAS SOURCE EVENTS\n");
    APPL_TRC("[SUB-SECTION] : BCAS SOURCE-SETUP\n");
    APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
    appl_ga_utils_display_bdaddr_and_type(device);
    APPL_TRC("[Event Status]: ");
    appl_ga_utils_display_ga_result(ga_status);

    return;
}

void appl_ga_utils_bc_display_sink_console_header
     (
         GA_ENDPOINT * device,
         UINT16        ga_status
     )
{
    APPL_TRC("[Profile]     : CAP \n");
    APPL_TRC("[SECTION]     : BCAS SINK EVENTS\n");
    APPL_TRC("[SUB-SECTION] : BCAS SINK-SETUP\n");
    APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
    appl_ga_utils_display_bdaddr_and_type(device);
    APPL_TRC("[Event Status]: ");
    appl_ga_utils_display_ga_result(ga_status);

    return;
}

void appl_ga_utils_parse_and_display_ea_report_subevent
     (
         UCHAR * ga_event_data,
         UINT8   ga_event_datalen
     )
{
    UCHAR     num_report;
    UINT32    count;
    UINT8     marker;

    marker = 0U;

    APPL_TRC("[APPL][GA][HCI]: HCI_LE_META_EVENT\n");
    APPL_TRC("[APPL][GA][HCI]: HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT\n");

    if ((NULL == ga_event_data) || (0U == ga_event_datalen))
    {
        APPL_ERR("ga_event_data is NULL or ga_event_datalen is 0U !");
        return;
    }

    /* HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT is ga_event_data[0U] */
    marker++;

    num_report = ga_event_data[marker];
    APPL_TRC("\t- num_reports = 0x%02X\n", ga_event_data[marker]);
    marker++;

    /* Print the reports */
    for (count = 0; count < num_report; count++)
    {
        appl_ga_utils_parse_and_display_ea_report_subevent_data
        (
            &ga_event_data[marker],
            (ga_event_datalen - marker)
        );
    }

    return;
}

void appl_ga_utils_parse_and_display_ea_report_subevent_data
     (
         UCHAR * ga_event_data,
         UINT8   ga_event_datalen
     )
{
    UINT8     marker;
    UINT8     value_1;
    UINT16    value_2;
    UCHAR     i;
    GA_ENDPOINT  device;

    marker = 0U;
    value_1 = 0U;
    value_2 = 0U;
    i = 0U;

    if ((NULL == ga_event_data) || (0U == ga_event_datalen))
    {
        APPL_ERR("ga_event_data is NULL or ga_event_datalen is 0U !");
        return;
    }

    /* Event Type: 2 Octet */
    GA_UNPACK_LE_2_BYTE
    (
        &value_2,
        &ga_event_data[marker]
    );
    marker += 2U;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Event type: 0x%04X\n", value_2);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Address Type: 1 Octet */
    value_1 = ga_event_data[marker];
    device.bd_type = value_1;
    marker++;
    APPL_TRC("\t- Address type: 0x%04X\n", value_1);

    /* Address: 6 Octet */
    APPL_TRC("\t- Address: ");
    for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
    {
        APPL_TRC("0x%02X ", ga_event_data[marker]);
        device.bd_addr[i] = ga_event_data[marker];
        marker++;
    }
    APPL_TRC("\n");

    /* Primary PHY: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Primary PHY: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Secondary PHY: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Secondary PHY: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Advertising SID: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
    APPL_TRC("\t- Advertising SID: 0x%04X\n", value_1);

    /* Tx Power: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Tx Power: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* RSSI: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- RSSI: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Periodic Advertising Interval: 2 Octet */
    GA_UNPACK_LE_2_BYTE
    (
        &value_2,
        &ga_event_data[marker]
    );
    marker += 2U;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Periodic Advertising Interval: 0x%04X\n", value_2);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Direct Address Type: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Direct Address type: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Direct Address: 6 Octet */
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Direct Address:");
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */
    for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
    {
#if APPL_GA_PARSE_EA_IN_DETAIL
        APPL_TRC("0x%02X ", ga_event_data[marker]);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */
        marker++;
    }
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\n");
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Data Length: 1 Octet */
    value_1 = ga_event_data[marker];
    marker++;
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Data Length: 0x%04X\n", value_1);
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    /* Data: Data Length Octet */
#if APPL_GA_PARSE_EA_IN_DETAIL
    APPL_TRC("\t- Data:\n");
#endif /* APPL_GA_PARSE_EA_IN_DETAIL */

    appl_ga_utils_parse_and_display_adv_data
    (
        &ga_event_data[marker],
        (ga_event_datalen - marker)
    );

    return;
}

void appl_ga_utils_parse_and_display_adv_data
     (
         UCHAR * adv_data,
         UINT8   adv_datalen
     )
{
    UINT8   marker;
    UINT8   value_1;
    UINT8   adlength;
    UINT8   adtype;

    if ((NULL == adv_data) || (0U == adv_datalen))
    {
        APPL_ERR("adv_data is NULL or adv_datalen is 0U !");
        return;
    }

    /* Initialize */
    marker = 0U;
    value_1 = 0U;
    adlength = 0U;
    adtype = 0U;

    value_1 = adv_datalen;

    GA_FORMAT_ADV_DISPLAY();

    while (0x00U < value_1)
    {
        /* Extract the Length from the event data */
        GA_UNPACK_LE_1_BYTE(&adlength, &adv_data[marker]);
        marker++;
        value_1 -= 1U;
        APPL_TRC("0x%04X", adlength);
        appl_ga_utils_display_spaces(6U, GA_EA_ADV_FORMAT_LENGTH_POS);

        /* TODO: Check Length for each param? */

        /* Extract the Ad type from the event data */
        GA_UNPACK_LE_1_BYTE(&adtype, &adv_data[marker]);
        marker++;
        value_1 -= 1U;
        appl_ga_utils_display_adtype(adtype);

        if (GA_ADTYPE_FLAGS == adtype)
        {
            appl_ga_utils_display_adtype_flags_data(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if ((GA_ADTYPE_COMP_16BIT_UUID_LIST == adtype) ||
                 (GA_ADTYPE_INCOMP_16BIT_UUID_LIST == adtype))
        {
            appl_ga_utils_display_adtype_comp_and_incomp_16b_uuid_data(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if ((GA_ADTYPE_SHORTENED_LOCAL_NAME == adtype) ||
                 (GA_ADTYPE_COMPLETE_LOCAL_NAME == adtype))
        {
            appl_ga_utils_display_adtype_short_and_comp_local_name_data(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if (GA_ADTYPE_SERVICE_DATA == adtype)
        {
            appl_ga_utils_display_adtype_svc_data(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if (GA_ADTYPE_RSI == adtype)
        {
            appl_ga_utils_display_adtype_rsi(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if (GA_ADTYPE_APPEARANCE == adtype)
        {
            appl_ga_utils_display_adtype_appearance(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else if (GA_ADTYPE_BROADCAST_NAME == adtype)
        {
            appl_ga_utils_display_adtype_bc_name(&adv_data[marker], adlength);
            /* Skip the already parsed adtype and the adv_data */
            value_1 -= (adlength - 1U);
            /* Skip the already parsed adtype and the adv_data */
            marker += (adlength - 1U);
        }
        else
        {
            /* TODO: Parse the data */
            appl_dump_bytes(&adv_data[marker], (adlength - 1U));

            /* Interested AD Type - Not found yet! */
            marker += (adlength - 1);
            value_1 -= (adlength - 1);
        }
    }

    return;
}

void appl_ga_utils_display_adtype(UINT8 adtype)
{
    switch(adtype)
    {
        case GA_ADTYPE_FLAGS:
            APPL_TRC("Flags (0x%04X)", adtype);
            appl_ga_utils_display_spaces(14U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_INCOMP_16BIT_UUID_LIST:
            APPL_TRC("Incomplete 16-bit UUIDs (0x%04X)", adtype);
            appl_ga_utils_display_spaces(32U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_COMP_16BIT_UUID_LIST:
            APPL_TRC("Complete 16-bit UUIDs (0x%04X)", adtype);
            appl_ga_utils_display_spaces(30U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_SHORTENED_LOCAL_NAME:
            APPL_TRC("Shortened Local Name (0x%04X)", adtype);
            appl_ga_utils_display_spaces(29U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_COMPLETE_LOCAL_NAME:
            APPL_TRC("Complete Local Name (0x%04X)", adtype);
            appl_ga_utils_display_spaces(28U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_SERVICE_DATA:
            APPL_TRC("Service Data (0x%04X)", adtype);
            appl_ga_utils_display_spaces(21U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_RSI:
            APPL_TRC("Resolvable Set Identifier (0x%04X)", adtype);
            appl_ga_utils_display_spaces(34U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_APPEARANCE:
            APPL_TRC("Appearance (0x%04X)", adtype);
            appl_ga_utils_display_spaces(19U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        case GA_ADTYPE_BROADCAST_NAME:
            APPL_TRC("Broadcast_Name (0x%04X)", adtype);
            appl_ga_utils_display_spaces(23U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;

        default:
            APPL_TRC("UnHandled AD Type (0x%04X)", adtype);
            appl_ga_utils_display_spaces(26U, GA_EA_ADV_FORMAT_TYPE_POS);
            break;
    }

    return;
}

void appl_ga_utils_display_adtype_flags(UINT8 flags)
{
    if (BT_AD_FLAGS_LE_LIMITED_DISC_MODE & flags)
    {
        APPL_TRC("LE Limited Discoverable Mode (0x%02X) - ", BT_AD_FLAGS_LE_LIMITED_DISC_MODE);
    }
    if (BT_AD_FLAGS_LE_GENERAL_DISC_MODE & flags)
    {
        APPL_TRC("LE General Discoverable Mode (0x%02X) - ", BT_AD_FLAGS_LE_GENERAL_DISC_MODE);
    }
    if (BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED & flags)
    {
        APPL_TRC("BR/EDR Not Supported (0x%02X) - ", BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED);
    }
    if (BT_AD_FLAGS_LE_BR_EDR_CONTROLLER & flags)
    {
        APPL_TRC("Simultaneous LE and BR/EDR to Same Device Capable (Controller) (0x%02X) - ", BT_AD_FLAGS_LE_BR_EDR_CONTROLLER);
    }
    if (BT_AD_FLAGS_LE_BR_EDR_HOST & flags)
    {
        APPL_TRC("Simultaneous LE and BR/EDR to Same Device Capable (Host) (0x%02X) - ", BT_AD_FLAGS_LE_BR_EDR_HOST);
    }
    APPL_TRC("(0x%02X)\n", flags);

    return;
}

void appl_ga_utils_display_adtype_flags_data(UCHAR * adv_data, UINT8 adlength)
{
    UINT8  marker;
    UINT8  parsedlen;

    /* Initialize */
    marker = 0U;
    parsedlen = 0U;

    appl_ga_utils_display_adtype_flags(adv_data[marker]);
    marker++;
    /* Skip the already Parsed ADType(1B) and Flags(1B) */
    parsedlen = 2U;

    if (parsedlen < adlength)
    {
        /* TODO: Parse the additional data */
#if 1
        appl_dump_bytes(&adv_data[marker], (adlength - parsedlen));
#endif /* 0 */
    }

    return;
}

void appl_ga_utils_display_adtype_comp_and_incomp_16b_uuid_data(UCHAR * adv_data, UINT8 adlength)
{
    UINT16  svc_uuid;
    UINT8   marker;

    /* Initialize */
    marker = 0U;
    svc_uuid = 0U;

    while (marker < (adlength - 1U))
    {
        /* Extract the Value - Service UUID from the event data */
        GA_UNPACK_LE_2_BYTE(&svc_uuid, &adv_data[marker]);
        marker += 2U;
        appl_ga_utils_display_svc_uuid(svc_uuid);
        appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
    }
    APPL_TRC("\n");

    return;
}

void appl_ga_utils_display_adtype_short_and_comp_local_name_data(UCHAR * adv_data, UINT8 adlength)
{
    UCHAR i;

    for(i = 0U; i < (adlength - 1U); i++)
    {
        APPL_TRC("%c", adv_data[i]);
    }
    APPL_TRC("\n");

    return;
}

void appl_ga_utils_display_announcement_type(UINT8 announcement_type)
{
    appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
    APPL_TRC("Announcement Type: ");
    if (CAS_GENERAL_ANNOUNCEMENT == announcement_type)
    {
        APPL_TRC("General");
    }
    else if (CAS_TARGETED_ANNOUNCEMENT == announcement_type)
    {
        APPL_TRC("Targeted");
    }
    APPL_TRC("(0x%02X)\n", announcement_type);

    return;
}

void appl_ga_utils_display_adtype_svc_data(UCHAR * adv_data, UINT8 adlength)
{
    UINT16  svc_uuid;
    UINT16  tmap_role;
    UINT16  audio_ctx;
    UINT8   metadata_len;
    UINT8   pbp_features;
    UINT8   marker;
    UINT8   parsedlen;
    UINT32  broadcast_id;
    UCHAR   i;

    /* Initialize */
    svc_uuid = 0U;
    parsedlen = 0U;
    marker = 0U;
    tmap_role = 0U;
    metadata_len = 0U;
    pbp_features = 0U;

    /* Extract the Value - Service UUID from the event data */
    GA_UNPACK_LE_2_BYTE(&svc_uuid, &adv_data[marker]);
    marker += 2U;
#if 0
    APPL_TRC("\t\t- AD Value: ");
#endif /* 0 */
    appl_ga_utils_display_svc_uuid(svc_uuid);
    /* Skip the already Parsed ADType(1B) and Svc UUID(2B) */
    parsedlen = 3U;

    if (parsedlen < adlength)
    {
        if ((GA_SERVICE_CAS == svc_uuid) || (GA_SERVICE_ASCS == svc_uuid))
        {
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Additional Service Data:\n");
            appl_ga_utils_display_announcement_type(adv_data[marker]);
            marker++;
            parsedlen++;
        }
        if ((GA_SERVICE_ASCS == svc_uuid) && (parsedlen < adlength))
        {
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Additional Service Data:\n");
            /* Available Sink Audio Contexts */
            GA_UNPACK_LE_2_BYTE(&audio_ctx, &adv_data[marker]);
            marker += 2U;
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Available Audio Contexts (Sink): ");
            APPL_TRC("0x%04X\n", audio_ctx);
            /* Available Source Audio Contexts */
            GA_UNPACK_LE_2_BYTE(&audio_ctx, &adv_data[marker]);
            marker += 2U;
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Available Audio Contexts (Source): ");
            APPL_TRC("0x%04X\n", audio_ctx);
            metadata_len = adv_data[marker];
            marker++;
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Metadata Length: 0x%02X\n", metadata_len);
            if (0U != metadata_len)
            {
                appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
                APPL_TRC("Metadata: ");
                for (i = 0U; i < metadata_len; i++)
                {
                    APPL_TRC("%02X ", adv_data[marker++]);
                }
                APPL_TRC("\n");
            }
        }
        if (GA_SERVICE_TMAS == svc_uuid)
        {
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Additional Service Data:\n");
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            GA_UNPACK_LE_2_BYTE(&tmap_role, &adv_data[marker]);
            appl_ga_utils_tmap_display_role(tmap_role);
        }
        if (GA_SERVICE_BCAS == svc_uuid)
        {
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Additional Service Data:\n");
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            GA_UNPACK_LE_3_BYTE(&broadcast_id, &adv_data[marker]);
            marker += 3U;
            APPL_TRC("Broadcast_ID: 0x%06X\n", broadcast_id);
        }
        if (GA_SERVICE_PBAS == svc_uuid)
        {
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Additional Service Data:\n");
            /* PBAP Features(1B) */
            pbp_features = adv_data[marker];
            marker++;
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            appl_ga_utils_display_adtype_svc_data_pbp_features(pbp_features);
            /* Metadata Length (1B) */
            metadata_len = adv_data[marker];
            marker++;
            appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
            APPL_TRC("Metadata Length: 0x%02X\n", metadata_len);
            if (0U != metadata_len)
            {
                appl_ga_utils_display_spaces(0U, GA_EA_ADV_FORMAT_VALUE_POS);
                APPL_TRC("Metadata: ");
                for (i = 0U; i < metadata_len; i++)
                {
                    /* TODO: Parse and display metadata ? */
                    APPL_TRC("%02X ", adv_data[marker++]);
                }
                APPL_TRC("\n");
                marker += metadata_len;
            }
        }
    }

    return;
}

void appl_ga_utils_display_adtype_rsi(UCHAR * adv_data, UINT8 adlength)
{
    UINT8  marker;
    UCHAR  rsi[6U];
    UINT8  parsedlen;
    UCHAR  i;

    /* Initialize */
    marker = 0U;
    parsedlen = 0U;

    BT_UNPACK_LE_N_BYTE
    (
        &rsi[0U],
        &adv_data[marker],
        6U
    );
    for (i = 0U; i < 6U; i++)
    {
        APPL_TRC("0x%02X ", rsi[i]);
    }
    APPL_TRC("\n");
    /* Skip the already Parsed ADType(1B) and RSI(6B) */
    parsedlen = 7U;

    if (parsedlen < adlength)
    {
        /* TODO: Parse the data */
        appl_dump_bytes(&adv_data[marker], (adlength - parsedlen));
    }

    return;
}

void appl_ga_utils_display_adtype_appearance(UCHAR * adv_data, UINT8 adlength)
{
    UINT16 appearance_type;

    /* Initialize */
    appearance_type = 0U;

    if ((NULL == adv_data) || (0U == adlength))
    {
        return;
    }

    GA_UNPACK_LE_2_BYTE(&appearance_type, &adv_data[0U]);

    appl_ga_utils_display_appearance_type(appearance_type);

    return;
}

void appl_ga_utils_display_appearance_type(UINT16 appearance_type)
{
    APPL_TRC("Appearance Value: ");

    switch(appearance_type)
    {
        case GA_APPEARANCE_TYPE_GA_SOURCE:
            APPL_TRC("Generic Audio Source");
            break;

        case GA_APPEARANCE_TYPE_MICROPHONE:
            APPL_TRC("Microphone");
            break;

        case GA_APPEARANCE_TYPE_ALARM:
            APPL_TRC("Alarm");
            break;

        case GA_APPEARANCE_TYPE_BELL:
            APPL_TRC("Bell");
            break;

        case GA_APPEARANCE_TYPE_HORN:
            APPL_TRC("Horn");
            break;

        case GA_APPEARANCE_TYPE_BC_DEV:
            APPL_TRC("Broadcasting Device");
            break;

        case GA_APPEARANCE_TYPE_SVC_DESK:
            APPL_TRC("Service Desk");
            break;

        case GA_APPEARANCE_TYPE_KIOSK:
            APPL_TRC("Kiosk");
            break;

        case GA_APPEARANCE_TYPE_BC_ROOM:
            APPL_TRC("Broadcasting Room");
            break;

        case GA_APPEARANCE_TYPE_AUDITORIUM:
            APPL_TRC("Auditorium");
            break;

        default:
            APPL_TRC("Unhandled Type");
            break;
    }

    APPL_TRC(" (0x%04X)\n", appearance_type);

    return;
}

void appl_ga_utils_display_adtype_bc_name(UCHAR * adv_data, UINT8 adlength)
{
    UCHAR i;

    /* Initialize */
    i = 0U;

    APPL_TRC("Broadcast Name: ");
    for (i = 0U; i < (adlength - 1U); i++)
    {
        APPL_TRC("%c", adv_data[i]);
    }
    APPL_TRC("\n");

    return;
}

void appl_ga_utils_display_spaces(UINT8 start, UINT8 end)
{
    for ( ; start < end; start++)
    {
        APPL_TRC(" ");
    }

    return;
}

void appl_ga_utils_tmap_display_role(UINT16 tmap_role)
{
    APPL_TRC("TMAP Role: ");

    if (tmap_role & TMAP_ROLE_CG)
    {
        APPL_TRC("Call Gateway (CG) (0x%04X) - ", TMAP_ROLE_CG);
    }
    if (tmap_role & TMAP_ROLE_CT)
    {
        APPL_TRC("Call Terminal (CT) (0x%04X) - ", TMAP_ROLE_CT);
    }
    if (tmap_role & TMAP_ROLE_UMS)
    {
        APPL_TRC("Unicast Media Sender (UMS) (0x%04X) - ", TMAP_ROLE_UMS);
    }
    if (tmap_role & TMAP_ROLE_UMR)
    {
        APPL_TRC("Unicast Media Receiver (UMR) (0x%04X) - ", TMAP_ROLE_UMR);
    }
    if (tmap_role & TMAP_ROLE_BMS)
    {
        APPL_TRC("Broadcast Media Sender (BMS) (0x%04X) - ", TMAP_ROLE_BMS);
    }
    if (tmap_role & TMAP_ROLE_BMR)
    {
        APPL_TRC("Broadcast Media Receiver (BMR) (0x%04X) - ", TMAP_ROLE_BMR);
    }
    APPL_TRC("(0x%04X)\n", tmap_role);
    return;
}

void appl_ga_utils_display_adtype_svc_data_pbp_features(UINT8 pbp_features)
{
    APPL_TRC("Public Broadcast Announcement features: ");

    APPL_TRC("Encryption: ");
    if ((PBP_BCAST_ENCRYPTION_ON << PBP_BCAST_ENCRYPTION_FEATURE_BIT) & pbp_features)
    {
        APPL_TRC
        (
            "Broadcast Streams are encrypted and require a Broadcast_Code (0x%04X) - ",
            ((PBP_BCAST_ENCRYPTION_ON << PBP_BCAST_ENCRYPTION_FEATURE_BIT) & pbp_features)
        );
    }
    else
    {
        APPL_TRC
        (
            "Broadcast Streams are not encrypted (0x%04X) - ",
            ((PBP_BCAST_ENCRYPTION_ON << PBP_BCAST_ENCRYPTION_FEATURE_BIT) & pbp_features)
        );
    }

    APPL_TRC("Standard Quality Public Broadcast Audio :");
    if ((PBP_BCAST_SQ_ON << PBP_BCAST_SQ_FEATURE_BIT) & pbp_features)
    {
        APPL_TRC
        (
            "Audio configuration present (0x%04X) - ",
            ((PBP_BCAST_SQ_ON << PBP_BCAST_SQ_FEATURE_BIT) & pbp_features)
        );
    }
    else
    {
        APPL_TRC
        (
            "Audio configuration not present (0x%04X) - ",
            ((PBP_BCAST_SQ_ON << PBP_BCAST_SQ_FEATURE_BIT) & pbp_features)
        );
    }

    APPL_TRC("High Quality Public Broadcast Audio: ");
    if ((PBP_BCAST_HQ_ON << PBP_BCAST_HQ_FEATURE_BIT) & pbp_features)
    {
        APPL_TRC
        (
            "Audio configuration present (0x%04X) - ",
            ((PBP_BCAST_HQ_ON << PBP_BCAST_HQ_FEATURE_BIT) & pbp_features)
        );
    }
    else
    {
        APPL_TRC
        (
            "Audio configuration not present (0x%04X) - ",
            ((PBP_BCAST_HQ_ON << PBP_BCAST_HQ_FEATURE_BIT) & pbp_features)
        );
    }

    APPL_TRC("(0x%04X)\n", pbp_features);
    return;
}

void appl_ga_utils_bap_fetch_update_local_pacs_audio_locations
     (
         UINT8 role,
         UINT32 locations
     )
{
    return;
}

#endif /* BT_GAM */
