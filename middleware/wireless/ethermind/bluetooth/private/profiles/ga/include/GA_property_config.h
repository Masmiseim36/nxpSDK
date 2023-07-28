/**
 *  \file GA_property_config.h
 *
 *  \brief This file contains the various characteristics
 *         configurations for GA profiles as defined in respective
 *         profile specification.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_PROPERTY_CONFIG_
#define _H_GA_PROPERTY_CONFIG_

/* --------------------------------------------- Header File Inclusion */
#include "GA_bearer_api.h"

/* --------------------------------------------- Global Definitions */
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

#ifdef GA_BAP

/**
 * \name Char Property: PACS
 * \{
 * \brief This section lists the Char properties.
 * As defined in PACS Specification.
 */

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_PACS_SINK_PAC \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify, Write
 */
#define GA_CHAR_PROP_PACS_SINK_AUDIO_LOCATIONS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_PACS_SOURCE_PAC \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify, Write
 */
#define GA_CHAR_PROP_PACS_SOURCE_AUDIO_LOCATIONS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_PROPERTY)

/**
 * Mandatory Property - Read Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_PACS_AVAILABLE_AUDIO_CONTEXTS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_PACS_SUPPORTED_AUDIO_CONTEXTS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */

/**
 * \name Char Property: ASCS
 * \{
 * \brief This section lists the Char properties.
 * As defined in ASCS Specification.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_ASCS_SINK_ASE (GA_BRR_CHAR_READ_PROPERTY | \
                                    GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_ASCS_SOURCE_ASE (GA_BRR_CHAR_READ_PROPERTY | \
                                      GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write, WriteWithoutResponse, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_ASCS_ASE_CP (GA_BRR_CHAR_WRITE_PROPERTY | \
                                  GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY | \
                                  GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */

#endif /* GA_BAP */

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_bass_module
 * \{
 */

/**
 * \addtogroup ga_bass_defines
 * \{
 */

/**
 * \addtogroup ga_bass_se_module_def
 * \{
 */

/**
 * \addtogroup ga_bass_se_constants
 * \{
 */

#ifdef GA_BAP

/**
 * \name Char Property: BASS
 * \{
 * This section lists the Char properties.
 * As defined in BASS Specification.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_BASS_BCAST_AUDIO_SCAN_CP (GA_BRR_CHAR_WRITE_PROPERTY | \
                                            GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_BASS_BCAST_RX_STATE (GA_BRR_CHAR_READ_PROPERTY | \
                                       GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */

#endif /* GA_BAP */

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_vcp_module
 * \{
 */

/**
 * \addtogroup ga_vcp_defines
 * \{
 */

/**
 * \defgroup ga_vcp_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GA VCP Layer.
 */

#ifdef GA_VCP

/**
 * \name Char Property: VCS
 * \{
 * \brief This section lists the Char properties.
 * As defined in VCS Specification.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_VOLUME_STATE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_VOLUME_CP GA_BRR_CHAR_WRITE_PROPERTY

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_VCS_VOLUME_FLAGS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */

/**
 * \name Char Property: VOCS
 * \{
 * \brief This section lists the Char properties.
 * As defined in VOCS Specification.
 * C.1: Mandatory to support Notify if Write Without Response
 * is supported, otherwise Optional.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VOCS_OFFSET_STATE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify[C.1] \n
 * Optional Property - WriteWithoutResponse, Notify
 */
#define GA_CHAR_PROP_VOCS_AUDIO_LOCATION \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/**
 * Mandatory Property - Write \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VOCS_OFFSET_CP GA_BRR_CHAR_WRITE_PROPERTY

/**
 * Mandatory Property - Read, Notify[C.1] \n
 * Optional Property - WriteWithoutResponse, Notify
 */
#define GA_CHAR_PROP_VOCS_AUDIO_OUTPUT_DESC \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/** \} */

#endif /* GA_VCP */

#ifdef GA_VCP

/**
 * \name Char Property: VCS-AICS
 * \{
 * \brief This section lists the Char properties.
 * As defined in AICS Specification.
 * C.1: Mandatory to support Notify if Write Without Response is supported,
 * otherwise Optional.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_AICS_INPUT_STATE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_AICS_GAIN_SETTING GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_AICS_INPUT_TYPE GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_AICS_INPUT_STATUS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_VCS_AICS_AUDIO_INPUT_CP GA_BRR_CHAR_WRITE_PROPERTY

/**
 * Mandatory Property - Read, Notify[C.1] \n
 * Optional Property - WriteWithoutResponse, Notify
 */
#define GA_CHAR_PROP_VCS_AICS_AUDIO_INPUT_DESC \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/** \} */

#endif /* GA_VCP */

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \addtogroup ga_micp_defines
 * \{
 */

/**
 * \defgroup ga_micp_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GA MICP Layer.
 */

#ifdef GA_MICP

/**
 * \name Char Property: MICS-AICS
 * \{
 * \brief This section lists the Char properties.
 * As defined in AICS Specification.
 * C.1: Mandatory to support Notify if Write Without Response is supported,
 * otherwise Optional.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_AICS_INPUT_STATE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_AICS_GAIN_SETTING GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_AICS_INPUT_TYPE GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_AICS_INPUT_STATUS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_AICS_AUDIO_INPUT_CP GA_BRR_CHAR_WRITE_PROPERTY

/**
 * Mandatory Property - Read, Notify[C.1] \n
 * Optional Property - WriteWithoutResponse, Notify
 */
#define GA_CHAR_PROP_MICS_AICS_AUDIO_INPUT_DESC \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/** \} */

#endif /* GA_MICP */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

/**
 * \addtogroup bt_ga_profiles
 * \{
 */

/**
 * \addtogroup bt_ga_content_control
 * \{
 */

/**
 * \addtogroup bt_ga_mcp
 * \{
 */

/**
 * \addtogroup ga_mcp_defines
 * \{
 */

/**
 * \addtogroup ga_mcp_se_module_def
 * \{
 */

/**
 * \addtogroup ga_mcp_se_constants
 * \{
 */

#ifdef GA_MCP

/**
 * \name Char Property: MCS
 * \{
 * \brief This section lists the Char properties.
 * As defined in MCS Specification.
 * C.4: Optional for MCS, Mandatory for GMCS
 */

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify[C.4]
 */
#define GA_CHAR_PROP_MCS_MEDIA_PLAYER_NAME \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_MEDIA_PLAYER_ICON_OBJ_ID GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_MEDIA_PLAYER_ICON_URL GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_TRACK_CHANGED GA_BRR_CHAR_NOTIFY_PROPERTY

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_TRACK_TITLE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_TRACK_DURATION \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_TRACK_POS \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_PLAYBACK_SPEED \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_SEEKING_SPEED \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_CURR_TRACK_SEG_OBJ_ID GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_CURR_TRACK_OBJ_ID \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_NEXT_TRACK_OBJ_ID \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_PARENT_GROUP_OBJ_ID \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_CURR_GROUP_OBJ_ID \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, WriteWithoutResponse \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_PLAYING_ORDER \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_WRITE_PROPERTY \
        | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_PLAYING_ORDERS_SUPP GA_BRR_CHAR_READ_PROPERTY

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_MEDIA_STATE \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write, WriteWithoutResponse, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_MEDIA_CONTROL_POINT \
        (GA_BRR_CHAR_WRITE_PROPERTY | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_MCS_CP_OPC_SUPP \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write, WriteWithoutResponse, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_SEARCH_CONTROL_POINT \
        (GA_BRR_CHAR_WRITE_PROPERTY | GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY \
        | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_SEARCH_RES_OBJ_ID \
        (GA_BRR_CHAR_READ_PROPERTY | GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MCS_CONTENT_CONTROL_ID GA_BRR_CHAR_READ_PROPERTY

/** \} */

#endif /* GA_MCP */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */
/** \} */

/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \addtogroup ga_csip_defines
 * \{
 */

/**
 * \defgroup ga_csip_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GA CSIP Layer.
 */

#ifdef GA_CSIP

/**
 * \name Char Property: CSIS
 * \{
 * \brief This section lists the Char properties.
 * As defined in CSIS Specification.
 */

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_CSIS_SIRK (GA_BRR_CHAR_READ_PROPERTY | \
                                GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_CSIS_SIZE (GA_BRR_CHAR_READ_PROPERTY | \
                                GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_CSIS_LOCK (GA_BRR_CHAR_READ_PROPERTY | \
                                GA_BRR_CHAR_WRITE_PROPERTY| \
                                GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_CSIS_RANK (GA_BRR_CHAR_READ_PROPERTY)

/** \} */

#endif /* GA_CSIP */

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \addtogroup ga_micp_defines
 * \{
 */

/**
 * \addtogroup ga_micp_constants
 * \{
 */

#ifdef GA_MICP

/**
 * \name Char Property: MICS
 * \{
 * \brief This section lists the Char properties.
 * As defined in MICS Specification.
 */

/**
 * Mandatory Property - Read, Write, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_MICS_MUTE (GA_BRR_CHAR_READ_PROPERTY | \
                                GA_BRR_CHAR_WRITE_PROPERTY | \
                                GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */

#endif /* GA_MICP */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

/**
 * \addtogroup bt_ga_profiles
 * \{
 */

/**
 * \addtogroup bt_ga_content_control
 * \{
 */

/**
 * \addtogroup bt_ga_ccp
 * \{
 */

/**
 * \addtogroup ga_ccp_defines
 * \{
 */

/**
 * \addtogroup ga_ccp_se_module_def
 * \{
 */

/**
 * \addtogroup ga_ccp_se_constants
 * \{
 */

#ifdef GA_CCP

/**
 * \name Char Property: TBS
 * \{
 * \brief This section lists the Char properties.
 * As defined in TBS Specification.
 */

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_PROVIDER_NAME (GA_BRR_CHAR_READ_PROPERTY | \
                                            GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_UCI (GA_BRR_CHAR_READ_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_TECHNOLOGY (GA_BRR_CHAR_READ_PROPERTY | \
                                         GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_TBS_BRR_URI_SCHMS_SUPPORT_LIST \
                                        (GA_BRR_CHAR_READ_PROPERTY | \
                                        GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_SIGSTRENGTH \
                                        (GA_BRR_CHAR_READ_PROPERTY | \
                                         GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Write, Write Without Response \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL \
                                    (GA_BRR_CHAR_READ_PROPERTY | \
                                     GA_BRR_CHAR_WRITE_PROPERTY | \
                                     GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_BRR_LIST_CUR_CALLS (GA_BRR_CHAR_READ_PROPERTY | \
                                             GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_STATUS_FLAGS (GA_BRR_CHAR_READ_PROPERTY | \
                                       GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_INCOMING_CALL_TARGET_BRR_URI \
                                            (GA_BRR_CHAR_READ_PROPERTY | \
                                             GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_CALL_STATE (GA_BRR_CHAR_READ_PROPERTY | \
                                     GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Write, Notify, Write Without Response \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_CALL_CP (GA_BRR_CHAR_WRITE_PROPERTY | \
                                  GA_BRR_CHAR_NOTIFY_PROPERTY | \
                                  GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_CALL_CP_OPTIONAL_OPCODES (GA_BRR_CHAR_READ_PROPERTY)

/**
 * Mandatory Property - Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_TERMINATION_REASON (GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_INCOMING_CALL (GA_BRR_CHAR_READ_PROPERTY | \
                                        GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_CALL_FRIENDLY_NAME (GA_BRR_CHAR_READ_PROPERTY | \
                                             GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TBS_CONTENT_CONTROL_ID GA_BRR_CHAR_READ_PROPERTY

/** \} */

#endif /* GA_CCP */

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup bt_ga_appl_profiles
 * \{
 */

/**
 * \addtogroup hap_module
 * \{
 */

/**
 * \addtogroup hap_defines
 * \{
 */

/**
 * \addtogroup hap_constants
 * \{
 */

#ifdef GA_HAP


/**
 * \name Char Property: HAS
 * \{
 * \brief This section lists the Char properties.
 * As defined in HAS Specification.
 */

/**
 * Mandatory Property - Read \n
 * Optional Property - Notify
 */
#define GA_CHAR_PROP_HA_FEATURES (GA_BRR_CHAR_READ_PROPERTY | \
                                  GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Indicate, Notify[If EATT is supported] \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_HA_PRESET_CP (GA_BRR_CHAR_WRITE_PROPERTY | \
                                   GA_BRR_CHAR_INDICATE_PROPERTY | \
                                   GA_BRR_CHAR_NOTIFY_PROPERTY)

/**
 * Mandatory Property - Read, Notify \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_ACTIVE_PRESET_INDEX (GA_BRR_CHAR_READ_PROPERTY | \
                                          GA_BRR_CHAR_NOTIFY_PROPERTY)

/** \} */
/** \} */
/** \} */
/** \} */

#endif /* GA_HAP */

/**
 * \addtogroup tmap_module
 * \{
 */

/**
 * \addtogroup tmap_defines
 * \{
 */

/**
 * \addtogroup tmap_constants
 * \{
 */

#ifdef GA_TMAP


/**
 * \name Char Property: TMAS
 * \{
 * \brief This section lists the Char properties.
 * As defined in TMAP Specification.
 */

/**
 * Mandatory Property - Read \n
 * Optional Property - None
 */
#define GA_CHAR_PROP_TMAP_ROLE GA_BRR_CHAR_READ_PROPERTY

/** \} */

#endif /* GA_TMAP */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

#endif /* _H_GA_PROPERTY_CONFIG_ */