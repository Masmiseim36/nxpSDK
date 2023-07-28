
/**
 *  \file GA_assigned_numbers.h
 *
 *  \brief This file contains all the bluetooth assigned number
 *  definitions for the GA module
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_ASSIGNED_NUMBERS_
#define _H_GA_ASSIGNED_NUMBERS_


/* --------------------------------------------- Header File Inclusion */
/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \addtogroup ga_bap_constants
 * \{
 */

/* --------------------------------------------- Global Definitions */

/**
 * \name GA Capturing/Rendering Control Service UUID
 * \{
 * \brief GA Capturing and Rendering Control Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 * \sa ga_bap_constants
 */

/** Audio Input Control Service */
#define GA_SERVICE_AICS                                 0x1843U
/** Volume Control Service */
#define GA_SERVICE_VCS                                  0x1844U
/** Volume Offset Control Service */
#define GA_SERVICE_VOCS                                 0x1845U
/** Microphone Control Service */
#define GA_SERVICE_MICS                                 0x184DU

/** \} */

/**
 * \name GA Audio Stream Control Service UUID
 * \{
 * \brief GA Audio Control Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 * \sa ga_bap_constants
 */

/** Audio Stream Control Service */
#define GA_SERVICE_ASCS                                 0x184EU
/** Broadcast Audio Scan Service */
#define GA_SERVICE_BASS                                 0x184FU
/** Published Audio Capabilities Service */
#define GA_SERVICE_PACS                                 0x1850U
/** Basic Audio Announcement Service */
#define GA_SERVICE_BAAS                                 0x1851U
/** Broadcast Audio Announcement Service */
#define GA_SERVICE_BCAS                                 0x1852U
/** Public Broadcast Announcement Service */
#define GA_SERVICE_PBAS                                 0x1856U

/** \} */

/**
 * \name GA Coordination Control Service UUID
 * \{
 * \brief GA Coordination Control Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 * \sa ga_bap_constants
 */

/** Coordinated Set Identification Service */
#define GA_SERVICE_CSIS                                 0x1846U

/** \} */

/**
 * \name GA Content Control Service UUID
 * \{
 * \brief GA Content Control Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 * GMCS and MCS use the same char UUID.
 * \sa ga_bap_constants
 *
 * GTBS and TBS uses the same char UUID.
 * \sa ga_bap_constants
 *
 * GMCS, MCS, GTBS, TBS use below Char:
 * \sa ga_bap_constants
 */

/** Media Control Service */
#define GA_SERVICE_MCS                                  0x1848U
/** Generic Media Control Service */
#define GA_SERVICE_GMCS                                 0x1849U
/** Telephone Bearer Service: O */
#define GA_SERVICE_TBS                                  0x184BU
/** Telephone Bearer Service: M */
#define GA_SERVICE_GTBS                                 0x184CU

/** \} */

/**
 * \name GA Application Profile Service UUID
 * \{
 * \brief GA Application Profile Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 * \sa ga_bap_constants
 */

/** Hearing Access Service */
#define GA_SERVICE_HAS                                  0x1854U
/** Telephony and Media Service */
#define GA_SERVICE_TMAS                                 0x1855U

/** \} */

/**
 * \name GA Common Audio Service UUID
 * \{
 * \brief GA Common Audio Service UUIDs
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 * \sa ga_bap_constants
 */

/** Common Audio Service */
#define GA_SERVICE_CAS                                  0x1853U

/** \} */

/**
 * \name GA Object Transfer Service UUID
 * \{
 * \brief GA Object Transfer Service UUIDs
 * \sa ga_bap_constants
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 */

/** Object Transfer Service */
#define GA_SERVICE_UUID_OTS                             0x1825U

/** \} */

/**
 * \name GA Capturing/Rendering Control Char UUID: AICS
 * \{
 * \brief Char UUID for AICS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M : Mandatory \n
 * O : Optional \n
 * X : Excluded \n
 * C.1: Mandatory to support Notify if Write Without Response is supported,
 * otherwise Optional.
 */

 /** Audio Input State: M */
#define GA_CHAR_AICS_INPUT_STATE                        0x2B77U
/** Gain Setting Properties: M */
#define GA_CHAR_AICS_GAIN_SETTING                       0x2B78U
/** Audio Input Type: M */
#define GA_CHAR_AICS_INPUT_TYPE                         0x2B79U
/** Audio Input Status: M */
#define GA_CHAR_AICS_INPUT_STATUS                       0x2B7AU
/** Audio Input Control Point: M */
#define GA_CHAR_AICS_AUDIO_INPUT_CP                     0x2B7BU
/** Audio Input Description: M */
#define GA_CHAR_AICS_AUDIO_INPUT_DESC                   0x2B7CU

/** \} */

/**
 * \name GA Capturing/Rendering Control Char UUID: VCS
 * \{
 * \brief Char UUID for VCS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M : Mandatory \n
 * O : Optional \n
 * X : Excluded
 */

 /** Volume State: M */
#define GA_CHAR_VCS_VOLUME_STATE                        0x2B7DU
/** Volume Control Point: M */
#define GA_CHAR_VCS_VOLUME_CP                           0x2B7EU
/** Volume Flags: M */
#define GA_CHAR_VCS_VOLUME_FLAGS                        0x2B7FU

/** \} */

/**
 * \name GA Capturing/Rendering Control Char UUID: VOCS
 * \{
 * \brief Char UUID for VOCS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M : Mandatory \n
 * O : Optional \n
 * X : Excluded \n
 * C.1: Mandatory to support Notify if Write Without Response is supported,
 * otherwise Optional.
 */

 /** Volume Offset State: M */
#define GA_CHAR_VOCS_OFFSET_STATE                       0x2B80U
/** Audio Location: M */
#define GA_CHAR_VOCS_AUDIO_LOCATION                     0x2B81U
/** Volume Offset Control Point: M */
#define GA_CHAR_VOCS_OFFSET_CP                          0x2B82U
/** Audio Output Description: M */
#define GA_CHAR_VOCS_AUDIO_OUTPUT_DESC                  0x2B83U

/** \} */

/**
 * \name GA Coordination Control Char UUID: CSIS
 * \{
 * \brief Char UUID for CSIS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory \n
 * O: Optional \n
 * X: Excluded \n
 * C.1: Mandatory if Set Member Lock is supported, otherwise Optional.
 */

/** Set Identity Resolving Key: M */
#define GA_CHAR_CSIS_SIRK                               0x2B84U
/** Coordinated Set Size: O */
#define GA_CHAR_CSIS_SIZE                               0x2B85U
/** Set Member Lock: O */
#define GA_CHAR_CSIS_LOCK                               0x2B86U
/** Set Member Rank: C.1 */
#define GA_CHAR_CSIS_RANK                               0x2B87U
/** \} */

/**
 * \name GA Content Control Char UUID: MCS and TBS
 * \{
 */

/**
 * GMCS and MCS use the same char UUID.
 * \sa ga_bap_constants
 *
 * GTBS and TBS use the same char UUID.
 * \sa ga_bap_constants
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 */
#define GA_CHAR_CONTENT_CONTROL_ID                      0x2BBAU

/** \} */

/**
 * \name GA Content Control Char UUID: MCS
 * \{
 * \brief Char UUID for MCS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M : Mandatory \n
 * O : Optional \n
 * X : Excluded \n
 * C.1: Mandatory if the Current Track Object ID characteristic is supported,
 * otherwise Excluded. \n
 * C.2: Mandatory if the Media Control Point characteristic is supported,
 * otherwise Excluded. \n
 * C.3: Mandatory if the Search Results Object ID characteristic is supported,
 * otherwise Excluded. \n
 * C.4: Optional for MCS, Mandatory for GMCS.
 */

/** Media Player Name: M */
#define GA_CHAR_MCS_MEDIA_PLAYER_NAME                   0x2B93U
/** Media Player Icon Object ID: O */
#define GA_CHAR_MCS_MEDIA_PLAYER_ICON_OBJ_ID            0x2B94U
/** Media Player Icon URL: O */
#define GA_CHAR_MCS_MEDIA_PLAYER_ICON_URL               0x2B95U
/** Track Changed: M */
#define GA_CHAR_MCS_TRACK_CHANGED                       0x2B96U
/** Track Title: M */
#define GA_CHAR_MCS_TRACK_TITLE                         0x2B97U
/** Track Duration: M */
#define GA_CHAR_MCS_TRACK_DURATION                      0x2B98U
/** Track Position: M */
#define GA_CHAR_MCS_TRACK_POS                           0x2B99U
/** Playback Speed: O */
#define GA_CHAR_MCS_PLAYBACK_SPEED                      0x2B9AU
/** Seeking Speed: O */
#define GA_CHAR_MCS_SEEKING_SPEED                       0x2B9BU
/** Current Track Segments Object ID: C.1 */
#define GA_CHAR_MCS_CURR_TRACK_SEG_OBJ_ID               0x2B9CU
/** Current Track Object ID: O */
#define GA_CHAR_MCS_CURR_TRACK_OBJ_ID                   0x2B9DU
/** Next Track Object ID: C.1 */
#define GA_CHAR_MCS_NEXT_TRACK_OBJ_ID                   0x2B9EU
/** Parent Group Object ID: C.1 */
#define GA_CHAR_MCS_PARENT_GROUP_OBJ_ID                 0x2B9FU
/** Current Group Object ID: C.1 */
#define GA_CHAR_MCS_CURR_GROUP_OBJ_ID                   0x2BA0U
/** Playing Order: O */
#define GA_CHAR_MCS_PLAYING_ORDER                       0x2BA1U
/** Playing Orders Supported: O */
#define GA_CHAR_MCS_PLAYING_ORDERS_SUPP                 0x2BA2U
/** Media State: M */
#define GA_CHAR_MCS_MEDIA_STATE                         0x2BA3U
/** Media Control Point: O */
#define GA_CHAR_MCS_MEDIA_CONTROL_POINT                 0x2BA4U
/** Media Control Point Opcodes Supported: C.2 */
#define GA_CHAR_MCS_CP_OPC_SUPP                         0x2BA5U
/** Search Results Object ID: O */
#define GA_CHAR_MCS_SEARCH_RES_OBJ_ID                   0x2BA6U
/** Search Control Point: C.3 */
#define GA_CHAR_MCS_SEARCH_CONTROL_POINT                0x2BA7U
/** Content Control ID: M */
#define GA_CHAR_MCS_CONTENT_CONTROL_ID                  GA_CHAR_CONTENT_CONTROL_ID

/** \} */

/**
 * \name GA Content Control Char UUID: TBS
 * \{
 * \brief Char UUID for TBS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory \n
 * O: Optional \n
 * X: Excluded \n
 * C.1:Mandatory if Bearer Signal Strength is supported, otherwise Excluded.
 */

/** Bearer Provider Name: M */
#define GA_CHAR_TBS_BRR_PROVIDER_NAME                   0x2BB3U
/** Bearer Uniform Caller Identifier (UCI): M */
#define GA_CHAR_TBS_BRR_UCI                             0x2BB4U
/** Bearer Technology: M */
#define GA_CHAR_TBS_BRR_TECHNOLOGY                      0x2BB5U
/** Bearer URI Schemes Supported List: M */
#define GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST          0x2BB6U
/** Bearer Signal Strength: O */
#define GA_CHAR_TBS_BRR_SIGSTRENGTH                     0x2BB7U
/** Bearer Signal Strength Reporting Interval: C.1 */
#define GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL  0x2BB8U
/** Bearer List Current Calls: M */
#define GA_CHAR_TBS_BRR_LIST_CUR_CALLS                  0x2BB9U
/** Status Flags: M */
#define GA_CHAR_TBS_STATUS_FLAGS                        0x2BBBU
/** Incoming Call Target Bearer URI: O */
#define GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI        0x2BBCU
/** Call State: M */
#define GA_CHAR_TBS_CALL_STATE                          0x2BBDU
/** Call Control Point: M */
#define GA_CHAR_TBS_CALL_CP                             0x2BBEU
/** Call Control Point Optional Opcodes: M */
#define GA_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES            0x2BBFU
/** Termination Reason: M */
#define GA_CHAR_TBS_TERMINATION_REASON                  0x2BC0U
/** Incoming Call: M */
#define GA_CHAR_TBS_INCOMING_CALL                       0x2BC1U
/** Call Friendly Name: O */
#define GA_CHAR_TBS_CALL_FRIENDLY_NAME                  0x2BC2U
/** Content Control ID: M */
#define GA_CHAR_TBS_CONTENT_CONTROL_ID                  GA_CHAR_CONTENT_CONTROL_ID

/** \} */

/**
 * \name GA Capturing/Rendering Control Char UUID: MICS
 * \{
 * \brief Char UUID for MICS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory \n
 * O: Optional \n
 * X: Excluded \n
 *
 * This service may include zero or more instances of AICS
 */

/** Mute: M */
#define GA_CHAR_MICS_MUTE                               0x2BC3U

/** \} */

/**
 * \name GA Audio Stream Control Char UUID: ASCS
 * \{
 * \brief Char UUID for ASCS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory \n
 * O: Optional \n
 * X: Excluded \n
 * C.1: Mandatory to support at least one of the Sink PAC characteristic or
 * Source PAC characteristic.
 */

/** Sink ASE: C.1 */
#define GA_CHAR_ASCS_SINK_ASE                           0x2BC4U
/** Source ASE: C.1 */
#define GA_CHAR_ASCS_SOURCE_ASE                         0X2BC5U
/** ASE Control Point: M */
#define GA_CHAR_ASCS_ASE_CP                             0x2BC6U
/** \} */

/**
 * \name GA Audio Stream Control Char UUID: BASS
 * \{
 * \brief Char UUID for BASS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory
 */

/** Broadcast Audio Scan Control Point: M */
#define GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP                0x2BC7U
/** Broadcast Receive State: M */
#define GA_CHAR_BASS_BCAST_RECEIVE_STATE                0x2BC8U
/** \} */

/**
 * \name GA Audio Stream Control Char UUID: PACS
 * \{
 * \brief Char UUID for PACS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * M: Mandatory \n
 * O: Optional \n
 * X: Excluded \n
 * C.1: Mandatory to support at least one of the Sink PAC characteristic or
 * Source PAC characteristic. \n
 * C.2: Optional to support if the Sink PAC characteristic is supported,
 * otherwise Excluded. \n
 * C.3: Optional to support if the Source PAC characteristic is supported,
 * otherwise Excluded.
 */

/** Sink PAC: C.1 */
#define GA_CHAR_PACS_SINK_PAC                           0x2BC9U
/** Sink Audio Locations: C.2 */
#define GA_CHAR_PACS_SINK_AUDIO_LOCATIONS               0x2BCAU
/** Source PAC: C.1 */
#define GA_CHAR_PACS_SOURCE_PAC                         0x2BCBU
/** Source Audio Locations: C.3 */
#define GA_CHAR_PACS_SOURCE_AUDIO_LOCATIONS             0x2BCCU
/** Available Audio Contexts: M */
#define GA_CHAR_PACS_AVAILABLE_AUDIO_CONTEXTS           0x2BCDU
/** Supported Audio Contexts: M */
#define GA_CHAR_PACS_SUPPORTED_AUDIO_CONTEXTS           0x2BCEU

/** \} */

/**
 * \name GA OTS Char UUID
 * \{
 * \brief Char UUID for OTS Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * C.1: Mandatory if the Server is capable of storing more than one object;
 * excluded otherwise. \n
 * C.2: Optional if the Server is capable of storing more than one object;
 * excluded otherwise. \n
 * C.3: Mandatory if the OACP Create op code is supported;
 * optional otherwise. \n
 * C.4: Mandatory if the Server does not have access to a real time clock;
 * excluded otherwise.
 */

/** OTS Feature: M */
#define GA_CHAR_OTS_CHAR_OTS_FEATURE                    0x2ABDU
/** Object Name: M */
#define GA_CHAR_OTS_CHAR_OBJ_NAME                       0x2ABEU
/** Object Type: M */
#define GA_CHAR_OTS_CHAR_OBJ_TYPE                       0x2ABFU
/** Object Size: M */
#define GA_CHAR_OTS_CHAR_OBJ_SIZE                       0x2AC0U
/** Object First-Created: O */
#define GA_CHAR_OTS_CHAR_FIRST_CREATED                  0x2AC1U
/** Object Last-Modified: O */
#define GA_CHAR_OTS_CHAR_LAST_MOD                       0x2AC2U
/** Object ID: C.1 */
#define GA_CHAR_OTS_CHAR_OBJ_ID                         0x2AC3U
/** Object Properties: M */
#define GA_CHAR_OTS_CHAR_OBJ_PROP                       0x2AC4U
/** Object Action Control Point: M */
#define GA_CHAR_OTS_CHAR_OACP                           0x2AC5U
/** Object List Control Point: C.1 */
#define GA_CHAR_OTS_CHAR_OLCP                           0x2AC6U
/** Object List Filter: C.2 */
#define GA_CHAR_OTS_CHAR_OBJ_LIST_FILTER                0x2AC7U
/** Object Changed: O */
#define GA_CHAR_OTS_CHAR_OBJ_CHG                        0x2AC8U

/** \} */

/**
 * \name GA OTS Object Type UUID
 * \{
 * \brief UUID for Object Type.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 */

/** Media Player Icon Object Type */
#define GA_OTS_OBJ_TYPE_MPLAYER_ICON                    0x2BA9U
/** Track Segments Object Type */
#define GA_OTS_OBJ_TYPE_TRACK_SEGMENTS                  0x2BAAU
/** Track Object Type */
#define GA_OTS_OBJ_TYPE_TRACK                           0x2BABU
/** Group Object Type */
#define GA_OTS_OBJ_TYPE_GROUP                           0x2BACU

/** \} */

/**
 * \name GA HAP Char UUID
 * \{
 * \brief Char UUID for HAP Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 *
 * C.1: Mandatory if Hearing Aid Preset Control Point is supported by the
 *      server; otherwise Excluded. \n
 */

/** Hearing Aid Features: M */
#define GA_CHAR_HA_FEATURES                             0x2BDAU
/** Hearing Aid Preset Control Point: O */
#define GA_CHAR_HA_PRESET_CP                            0x2BDBU
/** Active Preset Index: C.1 */
#define GA_CHAR_ACTIVE_PRESET_INDEX                     0x2BDCU

/** \} */

/**
 * \name GA TMAP Char UUID
 * \{
 * \brief Char UUID for TMAP Service.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document. \n
 */

/** TMAP Role: M */
#define GA_CHAR_TMAP_ROLE                               0x2B51U

/** \} */

/* TODO: Move to GA_Api.h or ??? */
/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_CONTENT_CONTROL_ID                 GA_BRR_CHAR_READ_PROPERTY


/**
 * \name GA MCS Object Type UUID
 * \{
 * \brief UUID for Object Type.
 * As defined in Assigned Numbers - 16-bit UUID Numbers Document.
 */

#define GA_OBJTYPE_MCS_MPLAYER_ICON                     0x2BA9U
#define GA_OBJTYPE_MCS_TRACK_SEGMENTS                   0x2BAAU
#define GA_OBJTYPE_MCS_TRACK                            0x2BABU
#define GA_OBJTYPE_MCS_GROUP                            0x2BACU

/** \} */

/**
 * \name GA Audio Location Definitions
 * \{
 * \brief GA Audio Location Definitions
 * Values(BitMap) \n
 * As defined in Assigned Numbers - Generic Audio Document.
 */

/** Not Allowed */
#define GA_AUDIO_LOCATION_NOT_ALLOWED                   (0x00000000U << 0U)
/** Front Left */
#define GA_AUDIO_LOCATION_FL                            (0x00000001U << 0U)
/** Front Right */
#define GA_AUDIO_LOCATION_FR                            (0x00000001U << 1U)
/** Front Center */
#define GA_AUDIO_LOCATION_FC                            (0x00000001U << 2U)
/** Low Frequency Effects 1 */
#define GA_AUDIO_LOCATION_LOW_FREQ_EFF_1                (0x00000001U << 3U)
/** Back Left */
#define GA_AUDIO_LOCATION_BL                            (0x00000001U << 4U)
/** Back Right */
#define GA_AUDIO_LOCATION_BR                            (0x00000001U << 5U)
/** Front Left of Center */
#define GA_AUDIO_LOCATION_FLC                           (0x00000001U << 6U)
/** Front Right of Center */
#define GA_AUDIO_LOCATION_FRC                           (0x00000001U << 7U)
/** Back Center */
#define GA_AUDIO_LOCATION_BC                            (0x00000001U << 8U)
/** Low Frequency Effects 2 */
#define GA_AUDIO_LOCATION_LOW_FREQ_EFF_2                (0x00000001U << 9U)
/** Side Left */
#define GA_AUDIO_LOCATION_SL                            (0x00000001U << 10U)
/** Side Right */
#define GA_AUDIO_LOCATION_SR                            (0x00000001U << 11U)
/** Top Front Left */
#define GA_AUDIO_LOCATION_TFL                           (0x00000001U << 12U)
/** Top Front Right */
#define GA_AUDIO_LOCATION_TFR                           (0x00000001U << 13U)
/** Top Front Center */
#define GA_AUDIO_LOCATION_TFC                           (0x00000001U << 14U)
/** Top Center */
#define GA_AUDIO_LOCATION_TC                            (0x00000001U << 15U)
/** Top Back Left */
#define GA_AUDIO_LOCATION_TBL                           (0x00000001U << 16U)
/** Top Back Right */
#define GA_AUDIO_LOCATION_TBR                           (0x00000001U << 17U)
/** Top Side Left */
#define GA_AUDIO_LOCATION_TSL                           (0x00000001U << 18U)
/** Top Side Right */
#define GA_AUDIO_LOCATION_TSR                           (0x00000001U << 19U)
/** Top Back Center */
#define GA_AUDIO_LOCATION_TBC                           (0x00000001U << 20U)
/** Bottom Front Center */
#define GA_AUDIO_LOCATION_BFC                           (0x00000001U << 21U)
/** Bottom Front Left */
#define GA_AUDIO_LOCATION_BFL                           (0x00000001U << 22U)
/** Bottom Front Right */
#define GA_AUDIO_LOCATION_BFR                           (0x00000001U << 23U)
/** Front Left Wide */
#define GA_AUDIO_LOCATION_FLW                           (0x00000001U << 24U)
/** Front Right Wide */
#define GA_AUDIO_LOCATION_FRW                           (0x00000001U << 25U)
/** Left Surround */
#define GA_AUDIO_LOCATION_LS                            (0x00000001U << 26U)
/** Right Surround */
#define GA_AUDIO_LOCATION_RS                            (0x00000001U << 27U)
/** RFU 1 */
#define GA_AUDIO_LOCATION_RFU_1                         (0x00000001U << 28U)
/** RFU 2 */
#define GA_AUDIO_LOCATION_RFU_2                         (0x00000001U << 29U)
/** RFU 3 */
#define GA_AUDIO_LOCATION_RFU_3                         (0x00000001U << 30U)
/** RFU 4 */
#define GA_AUDIO_LOCATION_RFU_4                         (0x00000001U << 31U)

/** \} */

/**
 * \name GA Audio Input Type Definitions
 * \{
 */

/** Unspecified input */
#define GA_AUDIO_INPUT_TYPE_UNSPECIFIED                 0x00U
/** Bluetooth audio stream */
#define GA_AUDIO_INPUT_TYPE_BLUETOOTH                   0x01U
/** Microphone */
#define GA_AUDIO_INPUT_TYPE_MICROPHONE                  0x02U
/** Analog Interface */
#define GA_AUDIO_INPUT_TYPE_ANALOG                      0x03U
/** Digital Interface */
#define GA_AUDIO_INPUT_TYPE_DIGITAL                     0x04U
/** AM/FM/XM/etc. */
#define GA_AUDIO_INPUT_TYPE_RADIO                       0x05U
/** Streaming audio source */
#define GA_AUDIO_INPUT_TYPE_STREAMING                   0x06U

/** \} */

/**
 * \name GA Context Type
 * \{
 * \brief GA Context Type
 * Values(BitMap)
 */

/** Prohibited */
#define GA_CONTEXT_TYPE_PROHIBITED                      (0x0000U << 0U)
/** Unspecified - bit 0 */
#define GA_CONTEXT_TYPE_UNSPECIFIED                     (0x0001U << 0U)
/**
 * Conversation between humans, for example, in telephony or video calls,
 * including traditional cellular as well as VoIP and Push-to-Talk - bit 1
 */
#define GA_CONTEXT_TYPE_CONVERSATIONAL                  (0x0001U << 1U)
/**
 * Media, for example, music playback, radio, podcast
 * or movie soundtrack, or tv audio - bit 2
 */
#define GA_CONTEXT_TYPE_MEDIA                           (0x0001U << 2U)
/**
 * Audio associated with video gaming, for example gaming media;
 * gaming effects; music and in-game voice chat between participants;
 * or a mix of all the above - bit 3
 */
#define GA_CONTEXT_TYPE_GAME                            (0x0001U << 3U)
/**
 * Instructional audio, for example, in navigation,
 * announcements, or user guidance - bit 4
 */
#define GA_CONTEXT_TYPE_INSTRUCTIONAL                   (0x0001U << 4U)
/**
 * Man-machine communication, for example, with voice recognition
 * or virtual assistants - bit 5
 */
#define GA_CONTEXT_TYPE_VOICE_ASSISTANTS                (0x0001U << 5U)
/**
 * Live audio, for example, from a microphone where audio is perceived
 * both through a direct acoustic path and through an LE Audio Stream - bit 6
 */
#define GA_CONTEXT_TYPE_LIVE                            (0x0001U << 6U)
/**
 * Sound effects including keyboard and touch feedback; menu and user
 * interface sounds; and other system sounds - bit 7
 */
#define GA_CONTEXT_TYPE_SOUND_EFFECTS                   (0x0001U << 7U)
/**
 * Notification and reminder sounds; attention-seeking audio, for example,
 * in beeps signaling the arrival of a message - bit 8
 */
#define GA_CONTEXT_TYPE_NOTIFICATIONS                   (0x0001U << 8U)
/**
 * Alerts the user to an incoming call, for example, an incoming
 * telephony or video call, including traditional cellular as well
 * as VoIP and Push-to-Talk - bit 9
 */
#define GA_CONTEXT_TYPE_RINGTONE                        (0x0001U << 9U)
/**
 * Alarms and timers; immediate alerts, for example,
 * in a critical battery alarm, timer expiry or alarm clock,
 * toaster, cooker, kettle, microwave, etc. - bit 10
 */
#define GA_CONTEXT_TYPE_ALERTS                          (0x0001U << 10U)
/**
 * Emergency alarm Emergency sounds, for example,
 * fire alarms or other urgent alerts - bit 11
 */
#define GA_CONTEXT_TYPE_EMERGENCY_ALARM                 (0x0001U << 11U)

/** \} */

/**
 * \name GA LTV Length
 * \{
 * \brief Length : Size(Octets): 1
 * Length includes size of LTV Type(1B) plus size of LTV Value(nB).
 */

/**
 * LTV Codec Specific Capabilities:
 * Supported_Sampling_Frequencies Length
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_LEN                   0x03U
/** LTV Codec Specific Capabilities: Supported_Frame_Durations Length */
#define GA_LTV_CODEC_SPEC_CAP_SFD_LEN                   0x02U
/** LTV Codec Specific Capabilities: Supported_Audio_Channel_Counts Length */
#define GA_LTV_CODEC_SPEC_CAP_SACC_LEN                  0x02U
/** LTV Codec Specific Capabilities: Supported_Octets_Per_Codec_Frame Length */
#define GA_LTV_CODEC_SPEC_CAP_SOPCF_LEN                 0x05U
/**
 * LTV Codec Specific Capabilities:
 * Supported Max Codec Frames Per SDU Length
 */
#define GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_LEN              0x02U
/** LTV Codec Specific Configuration: Sampling_Frequency Length */
#define GA_LTV_CODEC_SPEC_CONF_SF_LEN                   0x02U
/** LTV Codec Specific Configuration: Frame_Duration Length */
#define GA_LTV_CODEC_SPEC_CONF_FD_LEN                   0x02U
/** LTV Codec Specific Configuration: Audio_Channel_Allocation Length */
#define GA_LTV_CODEC_SPEC_CONF_ACA_LEN                  0x05U
/** LTV Codec Specific Configuration: Octets_Per_Codec_Frame Length */
#define GA_LTV_CODEC_SPEC_CONF_OPCF_LEN                 0x03U
/** LTV Codec Specific Configuration: Codec_Frame_Blocks_Per_SDU Length */
#define GA_LTV_CODEC_SPEC_CONF_CFBPSDU_LEN              0x02U
/** LTV Metadata: Preferred_Audio_Contexts Length */
#define GA_LTV_METADATA_PREF_AC_LEN                     0x03U
/** LTV Metadata: Streaming_Audio_Contexts Length */
#define GA_LTV_METADATA_STREAMING_AC_LEN                0x03U
/* LTV Metadata: Program_Info Length - Varies */
/* #define GA_LTV_METADATA_PGM_INFO_LEN */
/** LTV Metadata: Language Length */
#define GA_LTV_METADATA_LANGUAGE_LEN                    0x04U
/* LTV Metadata: CCID List Length - Varies */
/* #define GA_LTV_METADATA_CCID_LIST_LEN */
/** LTV Metadata: Parental_Rating Length */
#define GA_LTV_METADATA_PARENTAL_RATING_LEN             0x02U
/* LTV Metadata: Program_Info_URI Length - Varies */
/* #define GA_LTV_METADATA_PGM_INFO_URI_LEN */
/* LTV Metadata: Extended_Metadata Length - Varies */
/* #define GA_LTV_METADATA_EM_LEN */
/* LTV Metadata: Vendor_Specific Length - Varies */
/* #define GA_LTV_METADATA_VS_LEN */

/** \} */

/**
 * \name GA LTV Type
 * \{
 * \brief Type : Size(Octets): 1
 */

/** LTV Codec Specific Capabilities: Supported_Sampling_Frequencies Type */
#define GA_LTV_CODEC_SPEC_CAP_SSF_TYPE                  0x01U
/** LTV Codec Specific Capabilities: Supported_Frame_Durations Type */
#define GA_LTV_CODEC_SPEC_CAP_SFD_TYPE                  0x02U
/** LTV Codec Specific Capabilities: Supported_Audio_Channel_Counts Type */
#define GA_LTV_CODEC_SPEC_CAP_SACC_TYPE                 0x03U
/** LTV Codec Specific Capabilities: Supported_Octets_Per_Codec_Frame Type */
#define GA_LTV_CODEC_SPEC_CAP_SOPCF_TYPE                0x04U
/** LTV Codec Specific Capabilities: Supported_Max_Codec_Frames_Per_SDU Type */
#define GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_TYPE             0x05U

/** LTV Codec Specific Configuration: Sampling_Frequency Type */
#define GA_LTV_CODEC_SPEC_CONF_SF_TYPE                  0x01U
/** LTV Codec Specific Configuration: Frame_Duration Type */
#define GA_LTV_CODEC_SPEC_CONF_FD_TYPE                  0x02U
/** LTV Codec Specific Configuration: Audio_Channel_Allocation Type */
#define GA_LTV_CODEC_SPEC_CONF_ACA_TYPE                 0x03U
/** LTV Codec Specific Configuration: Octets_Per_Codec_Frame Type */
#define GA_LTV_CODEC_SPEC_CONF_OPCF_TYPE                0x04U
/** LTV Codec Specific Configuration: Codec_Frame_Blocks_Per_SDU Type */
#define GA_LTV_CODEC_SPEC_CONF_CFBPSDU_TYPE             0x05U

/** LTV Metadata: Preferred_Audio_Contexts Type */
#define GA_LTV_METADATA_PREF_AC_TYPE                    0x01U
/** LTV Metadata: Streaming_Audio_Contexts Type */
#define GA_LTV_METADATA_STREAMING_AC_TYPE               0x02U
/** LTV Metadata: Program_Info Type */
#define GA_LTV_METADATA_PGM_INFO_TYPE                   0x03U
/** LTV Metadata: Language Type */
#define GA_LTV_METADATA_LANGUAGE_TYPE                   0x04U
/** LTV Metadata: CCID_List Type */
#define GA_LTV_METADATA_CCID_LIST_TYPE                  0x05U
/** LTV Metadata: Parental_Rating Type */
#define GA_LTV_METADATA_PARENTAL_RATING_TYPE            0x06U
/** LTV Metadata: Program_Info_URL Type */
#define GA_LTV_METADATA_PROGRAM_INFO_URL_TYPE           0x07U
/** LTV Metadata: Extended_Metadata Type */
#define GA_LTV_METADATA_EM_TYPE                         0xFEU
/** LTV Metadata: Vendor_Specific Type */
#define GA_LTV_METADATA_VS_TYPE                         0xFFU

/** \} */

/**
 * \defgroup ga_ltv_val GA LTV Value
 * \{
 * Value
 */

/**
 * \name GA LTV Codec_Specific_Capabilities Supported_Sampling_Frequencies Value
 * \{
 * \brief Value : Size(Octets): 2 \n
 * Values(Bitmask)
 * Bitfield of supported sampling frequencies \n
 * 0b1 = supported, 0b0 = not supported
 */

/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 8000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_8000              (0x0001U << 0U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 11025Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_11025             (0x0001U << 1U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 16000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_16000             (0x0001U << 2U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 22050Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_22050             (0x0001U << 3U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 24000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_24000             (0x0001U << 4U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 32000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_32000             (0x0001U << 5U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 44100Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_44100             (0x0001U << 6U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 48000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000             (0x0001U << 7U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 88200Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_88200             (0x0001U << 8U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 96000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_96000             (0x0001U << 9U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 176400Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_176400            (0x0001U << 10U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 192000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_192000            (0x0001U << 11U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Sampling_Frequencies Value : 384000Hz
 */
#define GA_LTV_CODEC_SPEC_CAP_SSF_VAL_384000            (0x0001U << 12U)

/** \} */

/**
 * \name GA LTV Codec_Specific_Capabilities Supported_Frame_Durations Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 * Values(Bitmask) \n
 * 0b1 = supported, 0b0 = not supported
 */

/**
 * LTV Codec_Specific_Capabilities: Supported_Frame_Durations Value
 * 7.5 ms frame duration.
 */
#define GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS            (0x01U << 0U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Frame_Durations Value
 * 10 ms frame duration.
 */
#define GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS             (0x01U << 1U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Frame_Durations Value
 * 7.5 ms preferred. Valid only when 7.5 ms is supported and 10 ms
 * is supported. Shall not be set to 0b1 if bit 5 is set to 0b1.
 */
#define GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS_PREF       (0x01U << 4U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Frame_Durations Value
 * 10 ms preferred. Valid only when 7.5 ms is supported and 10 ms is
 * supported. Shall not be set to 0b1 if bit 4 is set to 0b1.
 */
#define GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS_PREF        (0x01U << 5U)

/** \} */

/**
 * \name GA LTV Codec_Specific_Capabilities Supported_Audio_Channel_Counts Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 * Values(Bitmask)
 */

/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 1
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_1                (0x01U << 0U)
/**
 * LTV Codec_Specific_Capabilities:
 * Supported_Audio_Channel_Counts Value Channel count: 2
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_2                (0x01U << 1U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 3
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_3                (0x01U << 2U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 4
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_4                (0x01U << 3U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 5
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_5                (0x01U << 4U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 6
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_6                (0x01U << 5U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 7
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_7                (0x01U << 6U)
/**
 * LTV Codec_Specific_Capabilities: Supported_Audio_Channel_Counts Value
 * Channel count: 8
 */
#define GA_LTV_CODEC_SPEC_CAP_SACC_VAL_8                (0x01U << 7U)

/** \} */

/**
 * \name GA LTV Codec_Specific_Capabilities Supported_Octets_Per_Codec_Frame Value
 * \{
 * \brief Value : Size(Octets): 4 \n
 */

/*
 * LTV Codec_Specific_Capabilities: Supported_Octets_Per_Codec_Frame Value
 * Octet 0-1: Minimum number of octets supported per codec frame
 * Octet 2-3: Maximum number of octets supported per codec frame
 */
/* #define GA_LTV_CODEC_SPEC_CAP_SOPCF_VAL */

/** \} */

/**
 * \name GA LTV Codec_Specific_Capabilities Supported_Max_Codec_Frames_Per_SDU Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 */

/*
 * LTV Codec_Specific_Capabilities: Supported_Max_Codec_Frames_Per_SDU Value
 * Maximum number of codec frames per SDU supported by this device
 */
/* #define GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_VAL */

/** \} */

/**
 * \name GA LTV Codec_Specific_Configuration Sampling_Frequencies Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 * Selected codec sampling frequency
 */

/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 8000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_8000              0x01U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 11025Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_11025             0x02U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 16000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_16000             0x03U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 22050Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_22050             0x04U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 24000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_24000             0x05U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 32000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_32000             0x06U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 44100Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_44100             0x07U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 48000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000             0x08U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 88200Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_88200             0x09U
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 96000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_96000             0x0AU
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 176400Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_176400            0x0BU
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 192000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_192000            0x0CU
/**
 * LTV Codec_Specific_Configuration:
 * Sampling_Frequencies Value : 384000Hz
 */
#define GA_LTV_CODEC_SPEC_CONF_SF_VAL_384000            0x0DU

/** \} */

/**
 * \name GA LTV Codec_Specific_Configuration Frame_Duration Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 */

/**
 * LTV Codec_Specific_Configuration: Frame_Duration Value
 * 7.5ms codec frames
 */
#define GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS            0x00U

/**
 * LTV Codec_Specific_Configuration: Frame_Duration Value
 * 10ms codec frames
 */
#define GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS             0x01U

/** \} */

/**
 * \name GA LTV Codec_Specific_Configuration Audio_Channel_Allocation Value
 * \{
 * \brief Value : Size(Octets): 4 \n
 */

/*
 * LTV Codec_Specific_Configuration: Audio_Channel_Allocation Value
 * 4-octet bitfield of Audio Location values
 * \ref ga_bap_constants
 */
/* #define GA_LTV_CODEC_SPEC_CONF_ACA_VAL */

/** \} */

/**
 * \name GA LTV Codec_Specific_Configuration Octets_Per_Codec_Frame Value
 * \{
 * \brief Value : Size(Octets): 2 \n
 */

/*
 * LTV Codec_Specific_Configuration: Octets_Per_Codec_Frame Value
 * Number of octets used per codec frame
 */
/* #define GA_LTV_CODEC_SPEC_CONF_OPCF_VAL */

/** \} */

/**
 * \name GA LTV Codec_Specific_Configuration Codec_Frame_Blocks_Per_SDU Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 */

/*
 * LTV Codec_Specific_Configuration: Codec_Frame_Blocks_Per_SDU Value
 * Number of blocks of codec frames per SDU
 */
/* #define GA_LTV_CODEC_SPEC_CONF_CFBPSDU_VAL */

/** \} */

/**
 * \name GA LTV Metadata Preferred_Audio_Contexts Value
 * \{
 * \brief Value : Size(Octets): 2 \n
 */

/*
 * LTV Metadata: Preferred_Audio_Contexts Value
 * Bitfield of Context Type values. \n
 * See Context Type values defined in \ref ga_bap_constants. \n
 * 0b0 = Context Type is not a preferred use case for this
 * codec configuration. \n
 * 0b1 = Context Type is a preferred use case for this codec configuration.
 */
/* #define GA_LTV_METADATA_PREF_AC_VAL */

/** \} */

/**
 * \name GA LTV Metadata Streaming_Audio_Contexts Value
 * \{
 * \brief Value : Size(Octets): 2 \n
 */

/*
 * LTV Metadata: Streaming_Audio_Contexts Value
 * Bitfield of Context Type values. \n
 * See Context Type values defined in \ref ga_bap_constants. \n
 * 0b0 = Context Type is not an intended use case for the Audio Stream. \n
 * 0b1 = Context Type is an intended use case for the Audio Stream.
 */
/* #define GA_LTV_METADATA_STREAM_AC_VAL */

/** \} */

/**
 * \name GA LTV Metadata Program_Info Value
 * \{
 * \brief Value : Size(Octets): Varies \n
 */

/*
 * LTV Metadata: Program_Info Value
 * Title and/or summary of Audio Stream content: UTF-8 format
 */
/* #define GA_LTV_METADATA_PGM_INFO_VAL */

/** \} */

/**
 * \name GA LTV Metadata Language Value
 * \{
 * \brief Value : Size(Octets): 3 \n
 * 3-byte, lower case language code as defined in ISO 639-3
 */

/*
 * LTV Metadata: Language Value
 */
/* #define GA_LTV_METADATA_LANGUAGE_VAL */

/** \} */

/**
 * \name GA LTV Metadata CCID_List Value
 * \{
 * \brief Value : Size(Octets): Varies \n
 */

/*
 * LTV Metadata: CCID_List Value
 * Array of CCID values
 */
/* #define GA_LTV_METADATA_CCID_LIST_VAL */

/** \} */

/**
 * \name GA LTV Metadata Parental_Rating Value
 * \{
 * \brief Value : Size(Octets): 1 \n
 */

/*
 * LTV Metadata: Parental_Rating Value
 */
/* #define GA_LTV_METADATA_PARENTAL_RATING_VAL */

/** \} */

/**
 * \name GA LTV Metadata Program_Info_URI Value
 * \{
 * \brief Value : Size(Octets): Varies \n
 * A UTF-8 formatted URL link used to present more information about
 * Program_Info.
 */

/*
 * LTV Metadata: Program_Info_URI Value
 */
/* #define GA_LTV_METADATA_PGM_INFO_URI_VAL */

/** \} */

/**
 * \name GA LTV Metadata Extended_Metadata Value
 * \{
 * \brief Value : Size(Octets): Varies \n
 */

/*
 * LTV Metadata: Extended_Metadata Value
 * Octet 0-1 = Extended Metadata Type \n
 * Octet 2-254 = Extended Metadata
 */
/* #define GA_LTV_METADATA_EM_VAL */

/** \} */

/**
 * \name GA LTV Metadata Vendor_Specific Value
 * \{
 * \brief Value : Size(Octets): Varies \n
 */

/*
 * LTV Metadata: Vendor_Specific Value
 * Octet 0-1 = Company_ID
 * Company ID values are defined in Bluetooth Assigned Numbers. \n
 * Octet 2-254 = Vendor-Specific Metadata
 */
/* #define GA_LTV_METADATA_VS_VAL */

/** \} */
/** \} */

/**
 * \name GA Coding Format Type
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in Assigned Numbers - Host Controller Interface.
 */

 /** GA Audio Coding Format - LC3 */
#define GA_AUDIO_CODING_LC3              0x06U

/** GA Audio Coding Format - Vendor Specific */
#define GA_AUDIO_CODING_VENDOR_SPECIFIC  0xFFU

/** GA Audio Coding Format - Invalid */
#define GA_AUDIO_CODING_INVALID          0xFEU

/** \} */

/**
 * \name GA Framing
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in ASCS Specification.
 */

/** Unframed ISOAL PDUs supported */
#define GA_FRAMING_UNFRAMED_ISOAL_PDU_SUPP      0x00U

/** Unframed ISOAL PDUs not supported */
#define GA_FRAMING_UNFRAMED_ISOAL_PDU_NOT_SUPP  0x01U

/** \} */

/**
 * \name GA Framing Config QOS
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in ASCS Specification.
 */

/** Unframed */
#define GA_FRAMING_UNFRAMED                     0x00U

/** Framed */
#define GA_FRAMING_FRAMED                       0x01U

/** \} */

/**
 * \name GA Target Latency
 * \{
 * \brief Type : Size(Octets): 1
 * Defined as in ASCS Specification.
 */

/** Target low latency */
#define GA_TARGET_LATENCY_LOW                       0x01U

/** Target balanced latency and reliability */
#define GA_TARGET_LATENCY_BALANCED_AND_RELAIBILITY  0x02U

/** Target high reliability */
#define GA_TARGET_LATENCY_HIGH_RELIABILITY          0x03U

/** \} */

/**
 * \name GA Target PHY
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in ASCS Specification.
 */

/** LE 1M PHY */
#define GA_TARGET_PHY_LE_1M_PHY      0x01U

/** LE 2M PHY */
#define GA_TARGET_PHY_LE_2M_PHY      0x02U

/** LE Coded PHY */
#define GA_TARGET_PHY_LE_CODED_PHY   0x03U

/** \} */

/**
 * \name GA Preferred PHY
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in ASCS Specification. Formatted as a bitfield.
 */

/** No Preference */
#define GA_PREF_PHY_NO_PREFERENCE  (0x00U << 0U)

/** LE 1M PHY preferred */
#define GA_PREF_PHY_LE_1M_PHY      (0x01U << 0U)

/** LE 2M PHY preferred */
#define GA_PREF_PHY_LE_2M_PHY      (0x01U << 1U)

/** LE Coded PHY preferred */
#define GA_PREF_PHY_LE_CODED_PHY   (0x01U << 2U)

/** \} */

/**
 * \name GA Preferred Retransmission Number
 * \{
 * \brief Type : Size(Octets): 1 \n
 * Defined as in ASCS Specification.
 */

/** Retransmission Number Min */
#define GA_PREF_RETXN_NUM_MIN  0x00U

/** Retransmission Number Max */
#define GA_PREF_RETXN_NUM_MAX  0xFFU

/** \} */

/**
 * \name GA Max Transport Latency
 * \{
 * \brief Type : Size(Octets): 2 \n
 * Defined as in ASCS Specification. \n
 * Units: ms
 */

/** Maximum Transport Latency Min */
#define GA_MAX_TRANS_LATENCY_MIN  0x0005U

/** Maximum Transport Latency Max */
#define GA_MAX_TRANS_LATENCY_MAX  0x0FA0U

/** \} */

/**
 * \name GA Max Presentation Delay
 * \{
 * \brief Type : Size(Octets): 3 \n
 * Defined as in ASCS Specification. \n
 * Units: micro seconds \n
 * If non-zero, shall be >= Presentation Delay Min \n
 * If non-zero, shall be <= Presentation Delay Max
 */

/** Presentation Delay Minimum No Preference */
#define GA_PRESENTATION_DELAY_NO_PREF  0x000000U

/** \} */

/**
 * \name GA SDU Interval
 * \{
 * \brief Type : Size(Octets): 3 \n
 * Defined as in ASCS Specification.
 */

/** SDU Interval Min */
#define GA_SDU_INTERVAL_MIN  0x0000FFU

/** SDU Interval Max */
#define GA_SDU_INTERVAL_MAX  0x0FFFFFU

/** \} */

/**
 * \name GA Max SDU
 * \{
 * \brief Type : Size(Octets): 2 \n
 * Defined as in ASCS Specification.
 */

/** Max SDU Min */
#define GA_MAX_SDU_MIN  0x0000U

/** Max SDU Max */
#define GA_MAX_SDU_MAX  0x0FFFU

/** \} */

/**
 * \name GA Advertising types
 * \{
 * \brief GA AD Type values defined to use in BLE Advertising Data
 * As defined in Assigned Numbers - Generic Access Profile Document.
 * \sa ga_cap_constants
 */

/** Advertisement Data Type: Flags */
#define GA_ADTYPE_FLAGS                   0x01U

/** Advertisement Data Type: Incomplete List of 16-bit Service Class UUIDs */
#define GA_ADTYPE_INCOMP_16BIT_UUID_LIST  0x02U

/** Advertisement Data Type: Complete List of 16-bit Service Class UUIDs */
#define GA_ADTYPE_COMP_16BIT_UUID_LIST    0x03U

/** Advertisement Data Type: Shortened Local Name */
#define GA_ADTYPE_SHORTENED_LOCAL_NAME    0x08U

/** Advertisement Data Type: Complete Local Name */
#define GA_ADTYPE_COMPLETE_LOCAL_NAME     0x09U

/* AD Type - Service Data */
#define GA_ADTYPE_SERVICE_DATA            0x16U

/* AD Type - Appearance */
#define GA_ADTYPE_APPEARANCE              0x19U

/* AD Type - BIGInfo */
#define GA_ADTYPE_BIGINFO                 0x2CU

/* AD Type - Broadcast Code */
#define GA_ADTYPE_BROADCAST_CODE          0x2DU

/* AD Type - Resolvable Set Identifier */
#define GA_ADTYPE_RSI                     0x2EU

/* AD Type - Broadcast Name */
#define GA_ADTYPE_BROADCAST_NAME          0x30U

/** \} */

/**
 * \name GA Appearance types
 * \{
 * \brief GA Appearance Type values defined to use in BLE Advertising Data
 * As defined in Assigned Numbers - Appearances Document.
 * https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf
 * \sa ga_cap_constants
 */
/* Appearance Types - Generic Audio Source */
#define GA_APPEARANCE_TYPE_GA_SOURCE      0x0880U

/* Appearance Types - Microphone */
#define GA_APPEARANCE_TYPE_MICROPHONE     0x0881U

/* Appearance Types - Alarm */
#define GA_APPEARANCE_TYPE_ALARM          0x0882U

/* Appearance Types - Bell */
#define GA_APPEARANCE_TYPE_BELL           0x0883U

/* Appearance Types - Horn */
#define GA_APPEARANCE_TYPE_HORN           0x0884U

/* Appearance Types - Broadcasting Device */
#define GA_APPEARANCE_TYPE_BC_DEV         0x0885U

/* Appearance Types - Service Desk */
#define GA_APPEARANCE_TYPE_SVC_DESK       0x0886U

/* Appearance Types - Kiosk */
#define GA_APPEARANCE_TYPE_KIOSK          0x0887U

/* Appearance Types - Broadcasting Room */
#define GA_APPEARANCE_TYPE_BC_ROOM        0x0888U

/* Appearance Types - Auditorium */
#define GA_APPEARANCE_TYPE_AUDITORIUM     0x0889U

/** \} */
/** \} */

/**
 * \addtogroup ga_bap_structures
 * \{
 */

/** Codec_Specific_Capabilities: Supported_Sampling_Frequencies */
typedef struct _GA_LTV_CODEC_SPEC_CAP_SSF
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CAP_SSF_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CAP_SSF_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT16 value;
}GA_LTV_CODEC_SPEC_CAP_SSF;

/** Codec_Specific_Capabilities: Supported_Frame_Durations */
typedef struct _GA_LTV_CODEC_SPEC_CAP_SFD
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CAP_SFD_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CAP_SFD_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CAP_SFD;

/** Codec_Specific_Capabilities: Supported_Audio_Channel_Counts */
typedef struct _GA_LTV_CODEC_SPEC_CAP_SACC
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CAP_SACC_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CAP_SACC_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CAP_SACC;

/** Codec_Specific_Capabilities: Supported_Octets_Per_Codec_Frame */
typedef struct _GA_LTV_CODEC_SPEC_CAP_SOPCF
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CAP_SOPCF_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CAP_SOPCF_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT32 value;
}GA_LTV_CODEC_SPEC_CAP_SOPCF;

/** Codec_Specific_Capabilities: Supported_Max_Codec_Frames_Per_SDU */
typedef struct _GA_LTV_CODEC_SPEC_CAP_SMCFPSDU
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CAP_SMCFPSDU;

/** Codec_Specific_Configuration: Sampling_Frequency */
typedef struct _GA_LTV_CODEC_SPEC_CONF_SF
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CONF_SF_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CONF_SF_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CONF_SF;

/** Codec_Specific_Configuration: Frame_Duration */
typedef struct _GA_LTV_CODEC_SPEC_CONF_FD
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CONF_FD_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CONF_FD_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CONF_FD;

/** Codec_Specific_Configuration: Audio_Channel_Allocation */
typedef struct _GA_LTV_CODEC_SPEC_CONF_ACA
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CONF_ACA_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CONF_ACA_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT32 value;
}GA_LTV_CODEC_SPEC_CONF_ACA;

/** Codec_Specific_Configuration: Octets_Per_Codec_Frame */
typedef struct _GA_LTV_CODEC_SPEC_CONF_OPCF
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CONF_OPCF_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CONF_OPCF_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT16 value;
}GA_LTV_CODEC_SPEC_CONF_OPCF;

/** Codec_Specific_Configuration: Codec_Frame_Blocks_Per_SDU */
typedef struct _GA_LTV_CODEC_SPEC_CONF_CFBPSDU
{
    /** Length: \ref GA_LTV_CODEC_SPEC_CONF_CFBPSDU_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_CODEC_SPEC_CONF_CFBPSDU_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8 value;
}GA_LTV_CODEC_SPEC_CONF_CFBPSDU;

/** Metadata: Preferred_Audio_Contexts */
typedef struct _GA_LTV_METADATA_PREF_AC
{
    /** Length: \ref GA_LTV_METADATA_PREF_AC_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_PREF_AC_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT16 value;
}GA_LTV_METADATA_PREF_AC;

/** Metadata: Streaming_Audio_Contexts */
typedef struct _GA_LTV_METADATA_STREAM_AC
{
    /** Length: \ref GA_LTV_METADATA_STREAMING_AC_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_STREAMING_AC_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT16 value;
}GA_LTV_METADATA_STREAM_AC;

/** Metadata: Program_Info */
typedef struct _GA_LTV_METADATA_PGM_INFO
{
    /** Length: \ref GA_LTV_METADATA_PGM_INFO_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_PGM_INFO_TYPE */
    UINT8 type;

    /** Value: Title and/or summary of Audio Stream content: UTF-8 format */
    UCHAR* value;
}GA_LTV_METADATA_PGM_INFO;

/** Metadata: Extended_Metadata */
typedef struct _GA_LTV_METADATA_EM
{
    /** Length: \ref GA_LTV_METADATA_EM_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_EM_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8* value;
}GA_LTV_METADATA_EM;

/** Metadata: Vendor_Specific */
typedef struct _GA_LTV_METADATA_VS
{
    /** Length: \ref GA_LTV_METADATA_VS_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_VS_TYPE */
    UINT8 type;

    /** Value: \ref ga_ltv_val */
    UINT8* value;
}GA_LTV_METADATA_VS;

/**
 * Metadata: CCID_List
 * Defined as in CAP Specification.
 */
typedef struct _GA_LTV_CCID_LIST
{
    /** Length: \ref GA_LTV_METADATA_CCID_LIST_LEN */
    UINT8 len;

    /** Type: \ref GA_LTV_METADATA_CCID_LIST_TYPE */
    UINT8 type;

    /**
     * Value: List for N CCID values referring to N instances of Content
     * Control services.
     */
    UINT8* value;
}GA_LTV_CCID_LIST;

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

#endif /* _H_GA_ASSIGNED_NUMBERS_ */
