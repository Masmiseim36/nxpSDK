
/**
 *  \file BT_avrcp_api.h
 *
 *  \brief EtherMind AVRCP API Header File.
 *
 *  This Header File contains declaration of AVRCP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_AVRCP_API_
#define _H_BT_AVRCP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_avctp_api.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup avrcp_module AVRCP (Audio Video Remote Control Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Audio Video Remote Control Profile (AVRCP) module to the Application
 *  and other upper layers of the stack.
 */

/**
 * \defgroup avrcp_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup avrcp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 *  \name AVRCP Event Notification Callback Event Types
 *  Constant Definitions for AVRCP Notification Callback Event Types.
 */

/*@{*/

#define AVRCP_CONNECT_IND                               0x01U
#define AVRCP_CONNECT_CNF                               0x02U
#define AVRCP_DISCONNECT_IND                            0x03U
#define AVRCP_DISCONNECT_CNF                            0x04U
#define AVRCP_MESSAGE_IND                               0x05U
#define AVRCP_MESSAGE_CNF                               0x06U
#define AVRCP_MESSAGE_SEND_CNF                          0x07U

#ifdef AVRCP_1_3

#define AVRCP_METADATA_CMD                              0x10U
#define AVRCP_METADATA_RSP                              0x11U
#define AVRCP_METADATA_INTERIM_RSP                      0x12U

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4

#define AVRCP_BOW_CONNECT_IND                           0x20U
#define AVRCP_BOW_CONNECT_CNF                           0x21U
#define AVRCP_BOW_DISCONNECT_IND                        0x22U
#define AVRCP_BOW_DISCONNECT_CNF                        0x23U
#define AVRCP_BOW_MESSAGE_IND                           0x24U
#define AVRCP_BOW_MESSAGE_RSP                           0x25U

#endif /* AVRCP_1_4 */

/*@}*/


/**
 *  \name AVRCP_MAX_PACKET_SIZE.
 *  Constant Definitions for AVRCP_MAX_PACKET_SIZE.
 */

/*@{*/

#define AVRCP_MAX_PACKET_SIZE                           512U

/*@}*/

#ifdef AVRCP_1_3
/**
 *  \name BT SIG registered Company ID.
 *  Constant Definitions for BT SIG registered Company ID.
 */

/*@{*/

#define BT_SIG_REGISTERED_COMPANY_ID    0x001958U

/* #define BT_SIG_REG_COMPANY_ID BT_SIG_REGISTERED_COMPANY_ID */

/*@}*/


/**
 *  \name AVRCP Command Header Length
 *  Constant Definitioins for AVRCP COMMAND Header Length.
 */

/*@{*/
#define AVRCP_COMMAND_HDR_LEN                                   3U

/*@}*/

/**
 *  \name AVRCP Command Header Length
 *  Constant Definitioins for AVRCP COMMAND Header Length.
 */

/*@{*/
#define AVRCP_METADATA_PDU_HDR_LEN                              7U

/*@}*/

#endif /* AVRCP_1_3 */



/**
 *  \name AVRCP Command Opcode Types
 *  Constant Definitions for AVRCP Command Opcode Types.
 */

/*@{*/

#define AVRCP_OPCODE_UNIT_INFO                          0x30U
#define AVRCP_OPCODE_SUBUNIT_INFO                       0x31U
#define AVRCP_OPCODE_PASS_THROUGH                       0x7CU

#ifdef AVRCP_1_3

#define AVRCP_OPCODE_VENDOR_DEPENDENT                   0x00U

#endif /* AVRCP_1_3 */
/*@}*/

/**
 *  \name AVRCP SubUnit Types
 *  Constant Definitions for AVRCP SubUnit Types.
 */

/*@{*/

#define AVRCP_SUBUNIT_TYPE_UNIT                         0x1FU
#define AVRCP_SUBUNIT_TYPE_PANEL                        0x09U

/*@}*/

/**
 *  \name AVRCP Message Response Types
 *  Constant Definitions for AVRCP Message Response Types.
 */

/*@{*/

#define AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED             0x08U
#define AVRCP_RESPONSE_TYPE_ACCEPTED                    0x09U
#define AVRCP_RESPONSE_TYPE_REJECTED                    0x0AU
#define AVRCP_RESPONSE_TYPE_INTERIM                     0x0FU
#define AVRCP_RESPONSE_TYPE_STABLE                      0x0CU

#ifdef AVRCP_1_3
#define AVRCP_RESPONSE_TYPE_CHANGED                     0x0DU
#endif /* AVRCP_1_3 */

/*@}*/

/**
 *  \name AVRCP Message Command Types
 *  Constant Definitions for AVRCP Message Command Types.
 */

/*@{*/

#define AVRCP_COMMAND_TYPE_CONTROL                      0x00U
#define AVRCP_COMMAND_TYPE_STATUS                       0x01U

#ifdef AVRCP_1_3
#define AVRCP_COMMAND_TYPE_NOTIFY                       0x03U
#endif /* AVRCP_1_3 */

/*@}*/

/**
 *  \name AVRCP Opeartion ID for PASS THROUGH Commands
 *  Constant Definitions for AVRCP Opeartion ID for PASS THROUGH Commands.
 */

/*@{*/
#define AVRCP_OPERATION_ID_SELECT                       0x00U
#define AVRCP_OPERATION_ID_UP                           0x01U
#define AVRCP_OPERATION_ID_DOWN                         0x02U
#define AVRCP_OPERATION_ID_LEFT                         0x03U
#define AVRCP_OPERATION_ID_RIGHT                        0x04U
#define AVRCP_OPERATION_ID_RIGHT_UP                     0x05U
#define AVRCP_OPERATION_ID_RIGHT_DOWN                   0x06U
#define AVRCP_OPERATION_ID_LEFT_UP                      0x07U
#define AVRCP_OPERATION_ID_LEFT_DOWN                    0x08U
#define AVRCP_OPERATION_ID_ROOT_MENU                    0x09U
#define AVRCP_OPERATION_ID_SETUP_MENU                   0x0AU
#define AVRCP_OPERATION_ID_CONTENTS_MENU                0x0BU
#define AVRCP_OPERATION_ID_FAVORITE_MENU                0x0CU
#define AVRCP_OPERATION_ID_EXIT                         0x0DU
#define AVRCP_OPERATION_ID_0                            0x20U
#define AVRCP_OPERATION_ID_1                            0x21U
#define AVRCP_OPERATION_ID_2                            0x22U
#define AVRCP_OPERATION_ID_3                            0x23U
#define AVRCP_OPERATION_ID_4                            0x24U
#define AVRCP_OPERATION_ID_5                            0x25U
#define AVRCP_OPERATION_ID_6                            0x26U
#define AVRCP_OPERATION_ID_7                            0x27U
#define AVRCP_OPERATION_ID_8                            0x28U
#define AVRCP_OPERATION_ID_9                            0x29U
#define AVRCP_OPERATION_ID_DOT                          0x2AU
#define AVRCP_OPERATION_ID_ENTER                        0x2BU
#define AVRCP_OPERATION_ID_CLEAR                        0x2CU
#define AVRCP_OPERATION_ID_CHANNEL_UP                   0x30U
#define AVRCP_OPERATION_ID_CHANNEL_DOWN                 0x31U
#define AVRCP_OPERATION_ID_PREVIOUS_CHANNEL             0x32U
#define AVRCP_OPERATION_ID_SOUND_SELECT                 0x33U
#define AVRCP_OPERATION_ID_INPUT_SELECT                 0x34U
#define AVRCP_OPERATION_ID_DISPLAY_INFORMATION          0x35U
#define AVRCP_OPERATION_ID_HELP                         0x36U
#define AVRCP_OPERATION_ID_PAGE_UP                      0x37U
#define AVRCP_OPERATION_ID_PAGE_DOWN                    0x38U
#define AVRCP_OPERATION_ID_POWER                        0x40U
#define AVRCP_OPERATION_ID_VOLUME_UP                    0x41U
#define AVRCP_OPERATION_ID_VOLUME_DOWN                  0x42U
#define AVRCP_OPERATION_ID_MUTE                         0x43U
#define AVRCP_OPERATION_ID_PLAY                         0x44U
#define AVRCP_OPERATION_ID_STOP                         0x45U
#define AVRCP_OPERATION_ID_PAUSE                        0x46U
#define AVRCP_OPERATION_ID_RECORD                       0x47U
#define AVRCP_OPERATION_ID_REWIND                       0x48U
#define AVRCP_OPERATION_ID_FAST_FORWARD                 0x49U
#define AVRCP_OPERATION_ID_EJECT                        0x4AU
#define AVRCP_OPERATION_ID_FORWARD                      0x4BU
#define AVRCP_OPERATION_ID_BACKWARD                     0x4CU
#define AVRCP_OPERATION_ID_ANGLE                        0x50U
#define AVRCP_OPERATION_ID_SUBPICTURE                   0x51U
#define AVRCP_OPERATION_ID_F1                           0x71U
#define AVRCP_OPERATION_ID_F2                           0x72U
#define AVRCP_OPERATION_ID_F3                           0x73U
#define AVRCP_OPERATION_ID_F4                           0x74U
#define AVRCP_OPERATION_ID_F5                           0x75U
#define AVRCP_OPERATION_ID_VENDOR_UNIQUE                0x7EU

/*@}*/

/**
 *  \name AVRCP State Flag Definitions
 *  Constant Definitions for AVRCP State Flag.
 */

/*
 *   7    6                        0
 * +----+----------------------------+
 * |    |      OPERATION ID          |
 * +----+----------------------------+
 *   |
 *   `-> State Flag [0 - Pressed, 1 - Released]
 */

/*@{*/

/** AVRCP State_Flag: Pressed */
#define AVRCP_STATE_FLAG_PRESSED                        0x00U

/** AVRCP State_Flag: Released */
#define AVRCP_STATE_FLAG_RELEASED                       0x80U

/*@}*/


/**
 *  \name AVRCP Passthrough Command Length defines
 *  Constant Definitions for AVRCP passthrough command lengths
 */

/*@{*/

/** AVRCP PASSTHROUGH Control Command length */
#define AVRCP_PASSTHROUGH_CT_CMD_LEN                    0x05U

/** AVRCP PASSTHROUGH Vendor Unique Control Command length */
#define AVRCP_PASSTHROUGH_VU_CT_CMD_LEN                 0x0AU

/*@}*/


/**
 *  \name AVRCP Macros for Updating Button pressed and released State Flag
 */

/*@{*/

/** Macro to set an Operation ID's State Flag as Pressed */
#define AVRCP_SET_STATE_FLAG_PRESSED(op_id)      \
        ((op_id) &= ~AVRCP_STATE_FLAG_RELEASED)

/** Macro to set an Operation ID's State Flag as Released */
#define AVRCP_SET_STATE_FLAG_RELEASED(op_id)     \
        ((op_id) |= AVRCP_STATE_FLAG_RELEASED)

/* Macro to Extract the operation ID */
#define AVRCP_GET_OPERATION_ID(op_id)            \
        ((op_id) & (~AVRCP_STATE_FLAG_RELEASED))

/**
 * Macro to Extract the State Flag
 * The Macro would result in 'AVRCP_STATE_FLAG_RELEASED' value if the
 * State Flag indicates 'Released',
 * else it would result in 'AVRCP_STATE_FLAG_PRESSED'
 */
#define AVRCP_GET_STATE_FLAG(op_id)              \
        ((op_id) & AVRCP_STATE_FLAG_RELEASED)
/*@}*/


#ifdef AVRCP_1_3
/**
 *  \name AVRCP PDU ID for VENDOR DEPENDENT Metadata Transfer PDUs
 *  Constant Definitions for VENDOR DEPENDENT Metadata Transfer PDU ID.
 */

/*@{*/

#define AVRCP_METADATA_PDU_ID_GET_CAPABILITY                    0x10U
#define AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR      0x11U
#define AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_VAL       0x12U
#define AVRCP_METADATA_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL    0x13U
#define AVRCP_METADATA_PDU_ID_SET_PLAYER_APP_SETTING_VAL        0x14U
#define AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT   0x15U
#define AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT    0x16U

#define AVRCP_METADATA_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET       0x17U
#define AVRCP_METADATA_PDU_ID_INFORM_BATTERY_STATUS             0x18U

#define AVRCP_METADATA_PDU_ID_GET_ELEMENT_ATTRIBUTE             0x20U

#define AVRCP_METADATA_PDU_ID_GET_PLAY_STATUS                   0x30U
#define AVRCP_METADATA_PDU_ID_REGISTER_NOTIFICATION             0x31U

#define AVRCP_METADATA_PDU_ID_REQUEST_CONTINUING_RESPONSE       0x40U
#define AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE         0x41U



/**
 * \name AVRCP Verndor Unique ID for Basic Group Navigation
 *
 */

#define AVRCP_GROUP_NAVIGATION_VENDOR_UNIQUE_ID_NEXT            0x00U
#define AVRCP_GROUP_NAVIGATION_VENDOR_UNIQUE_ID_PREV            0x01U


/* AVRCP 1.4 Specific PDU IDs */
#ifdef AVRCP_1_4

#define AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME                        0x50U

#define AVRCP_PDU_ID_SET_ADDRESSED_PLAYER                       0x60U

#define AVRCP_PDU_ID_SET_BROWSED_PLAYER                         0x70U
#define AVRCP_PDU_ID_GET_FOLDER_ITEMS                           0x71U
#define AVRCP_PDU_ID_CHANGE_PATH                                0x72U
#define AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES                        0x73U
#define AVRCP_PDU_ID_PLAY_ITEMS                                 0x74U

#define AVRCP_PDU_ID_SEARCH                                     0x80U

#define AVRCP_PDU_ID_ADD_TO_NOW_PLAYING                         0x90U

#define AVRCP_PDU_ID_GENERAL_REJECT                             0xA0U

#endif /* AVRCP_1_4 */

/* AVRCP 1.6 Specific PDU IDs */
#ifdef AVRCP_GET_NUM_OF_ITEMS
#define AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS                        0x75U
#endif /* AVRCP_GET_NUM_OF_ITEMS */

/*@}*/

/**
 *  \name AVRCP Capabilities
 *  Constant Definitions for AVRCP Capability.
 */

/*@{*/

#define AVRCP_CAP_ID_COMPANY_ID                                 0x02U
#define AVRCP_CAP_ID_EVENTS_SUPPORTED                           0x03U
/*@}*/

#ifdef AVRCP_1_4

/**
 *  \name Media Content Navigation.
 *  Constant Definitions for AVRCP Scopes.
 */

/*@{*/

#define AVRCP_SCOPE_MEDIA_PLAYER_LIST                           0x00U
#define AVRCP_SCOPE_VIRTUAL_FILESYSTEM                          0x01U
#define AVRCP_SCOPE_SEARCH                                      0x02U
#define AVRCP_SCOPE_NOW_PLAYING                                 0x03U
/*@}*/

/**
 *  \name Browsable Item Types.
 *  Constant Definitions for AVRCP Browsable Item Types.
 */

/*@{*/

#define AVRCP_ITEM_MEDIA_PLAYER                         0x01U
#define AVRCP_ITEM_FOLDER                               0x02U
#define AVRCP_ITEM_MEDIA_ELEMENT                        0x03U
/*@}*/

/**
 *  \name Major Player Types.
 *  Constant Definitions for AVRCP Major Player Types.
 */

/*@{*/

#define AVRCP_MP_MAJOR_PLAYER_TYPE_AUDIO                        0x01U
#define AVRCP_MP_MAJOR_PLAYER_TYPE_VIDEO                        0x02U
#define AVRCP_MP_MAJOR_PLAYER_TYPE_BROADCASTING_AUDIO           0x04U
#define AVRCP_MP_MAJOR_PLAYER_TYPE_BROADCASTING_VIDEO           0x08U
/*@}*/

/**
 *  \name Player Sub Types.
 *  Constant Definitions for AVRCP Player Sub Types.
 */

/*@{*/

#define AVRCP_MP_PLAYER_SUBTYPE_AUDIO_BOOK                      0x00000001U
#define AVRCP_MP_PLAYER_SUBTYPE_PODCAST                         0x00000002U
/*@}*/

/**
 *  \name Media Player Play Status.
 *  AVRCP Media Player Play Status codes.
 */

/*@{*/

#define AVRCP_MP_PLAY_STATUS_STOPPED            0x00U
#define AVRCP_MP_PLAY_STATUS_PLAYING            0x01U
#define AVRCP_MP_PLAY_STATUS_PAUSED             0x02U
#define AVRCP_MP_PLAY_STATUS_FWD_SEEK           0x03U
#define AVRCP_MP_PLAY_STATUS_REV_SEEK           0x04U
#define AVRCP_MP_PLAY_STATUS_ERROR              0xffU
/*@}*/


/**
 *  \name File System File Type.
 *  Constatnt Definitions for AVRCP Folder Types.
 */

/*@{*/

#define AVRCP_VFS_FOLDER_TYPE_MIXED             0x00U
#define AVRCP_VFS_FOLDER_TYPE_TITLES            0x01U
#define AVRCP_VFS_FOLDER_TYPE_ALBUMS            0x02U
#define AVRCP_VFS_FOLDER_TYPE_ARTISTS           0x03U
#define AVRCP_VFS_FOLDER_TYPE_GENRES            0x04U
#define AVRCP_VFS_FOLDER_TYPE_PLAYLISTS         0x05U
#define AVRCP_VFS_FOLDER_TYPE_YEARS             0x06U

/* Folder is playable or not */
#define AVRCP_VFS_FOLDER_PLAYABLE               0x00U
#define AVRCP_VFS_FOLDER_NOT_PLAYABLE           0x01U
/*@}*/

/**
 *  \name Media Element Item Types.
 *  Constatnt Definitions for AVRCP Media Element Items Types.
 */

/*@{*/

#define AVRCP_MEDIA_ELEMENT_TYPE_AUDIO          0x00U
#define AVRCP_MEDIA_ELEMENT_TYPE_VIDEO          0x01U
/*@}*/

/**
 *  \name Path Change Directions.
 *  Constatnt Definitions for AVRCP Path Change Directions.
 */

/*@{*/

#define AVRCP_VFS_CHANGE_PATH_UP                0x00U
#define AVRCP_VFS_CHANGE_PATH_DOWN              0x01U
/*@}*/


#endif /* AVRCP_1_4 */

/**
 *  \name AVRCP Metadata PDU Packet Types.
 *  Constant Definitions for AVRCP Metadata PDU Packet Type.
 */

/*@{*/

#define AVRCP_METADATA_PACKET_TYPE_SINGLE      0x00U
#define AVRCP_METADATA_PACKET_TYPE_START       0x01U
#define AVRCP_METADATA_PACKET_TYPE_CONTINUE    0x02U
#define AVRCP_METADATA_PACKET_TYPE_END         0x03U
/*@}*/

/**
 *  \name AVRCP Metadata Media Attribute ID.
 *  Constant Definitions for AVRCP Metadata Media Attribute ID.
 */

/*@{*/

#define AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE           0x01U
#define AVRCP_MEDIA_ATTR_ID_ARTIST_NAME           0x02U
#define AVRCP_MEDIA_ATTR_ID_ALBUM_NAME            0x03U
#define AVRCP_MEDIA_ATTR_ID_MEDIA_NUMBER          0x04U
#define AVRCP_MEDIA_ATTR_ID_TOTAL_MEDIA_NUMBER    0x05U
#define AVRCP_MEDIA_ATTR_ID_GENRE                 0x06U
#define AVRCP_MEDIA_ATTR_ID_PLAYING_TIME          0x07U

/* AVRCP_1_6 related Attirbute ID */
#ifdef AVRCP_COVER_ART
#define AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART     0x08U
#endif /* AVRCP_COVER_ART */



/*@}*/

/**
 *  \name AVRCP Metadata Player Application Setting Attribute ID.
 *  Constant Definitions for AVRCP Metadata Player Application
 *           Setting Attribute ID.
 */

/*@{*/

#define AVRCP_PLAYER_APP_ATTR_ID_EQUALIZER    0x01U
#define AVRCP_PLAYER_APP_ATTR_ID_REPEAT       0x02U
#define AVRCP_PLAYER_APP_ATTR_ID_SHUFFLE      0x03U
#define AVRCP_PLAYER_APP_ATTR_ID_SCAN         0x04U

/*@}*/


/**
 *  \name AVRCP Metadata Player Application Setting Value ID.
 *  Constant Definitions for AVRCP Metadata Player Application
 *           Setting Value ID.
 */

/*@{*/

#define AVRCP_PLAYER_APP_VALUE_ID_EQUALIZER_OFF    0x01U
#define AVRCP_PLAYER_APP_VALUE_ID_EQUALIZER_ON     0x02U

#define AVRCP_PLAYER_APP_VALUE_ID_REPEAT_OFF       0x01U
#define AVRCP_PLAYER_APP_VALUE_ID_REPEAT_SINGLE    0x02U
#define AVRCP_PLAYER_APP_VALUE_ID_REPEAT_ALL       0x03U
#define AVRCP_PLAYER_APP_VALUE_ID_REPEAT_GROUP     0x04U

#define AVRCP_PLAYER_APP_VALUE_ID_SHUFFLE_OFF      0x01U
#define AVRCP_PLAYER_APP_VALUE_ID_SHUFFLE_ALL      0x02U
#define AVRCP_PLAYER_APP_VALUE_ID_SHUFFLE_GROUP    0x03U

#define AVRCP_PLAYER_APP_VALUE_ID_SCAN_OFF         0x01U
#define AVRCP_PLAYER_APP_VALUE_ID_SCAN_ALL         0x02U
#define AVRCP_PLAYER_APP_VALUE_ID_SCAN_GROUP       0x03U

/*@}*/


/**
 *  \name AVRCP Metadata Transfer Events.
 *  Constant Definitions for AVRCP Metadata Transfer Events.
 */

/*@{*/

#define AVRCP_EVENT_PLAYBACK_STATUS_CHANGED       0x01U
#define AVRCP_EVENT_TRACK_CHANGED                 0x02U
#define AVRCP_EVENT_TRACK_REACHED_END             0x03U
#define AVRCP_EVENT_TRACK_REACHED_START           0x04U
#define AVRCP_EVENT_PLAYBACK_POS_CHANGED          0x05U
#define AVRCP_EVENT_BATT_STATUS_CHANGED           0x06U
#define AVRCP_EVENT_SYSTEM_STATUS_CHANGED         0x07U
#define AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED    0x08U

#ifdef AVRCP_1_4

#define AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED   0x09U
#define AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED      0x0AU
#define AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED      0x0BU
#define AVRCP_EVENT_UIDS_CHANGED                  0x0CU
#define AVRCP_EVENT_VOLUME_CHANGED                0x0DU

/* The usage of following define is deprecated */
#define AVRCP_EVENT_NOW_PLAYING_CHANGED \
        AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED
#endif /* AVRCP_1_4 */
/*@}*/

/**
 *  \name AVRCP Metadata Battery Status Code.
 *  Constant Definitions for AVRCP Metadata Battery Status Code.
 */

/*@{*/

#define AVRCP_BATTERY_STATUS_NORMAL    0x00U
#define AVRCP_BATTERY_STATUS_WARNING   0x01U
#define AVRCP_BATTERY_STATUS_CRITICAL  0x02U
#define AVRCP_BATTERY_STATUS_EXTERNAL  0x03U
#define AVRCP_BATTERY_STATUS_FULL      0x04U

/*@}*/

/**
 *  \name AVRCP Metadata System Status Code.
 *  Constant Definitions for AVRCP System Status Code.
 */

/*@{*/

#define AVRCP_SYSTEM_STATUS_POWER_ON   0x00U
#define AVRCP_SYSTEM_STATUS_POWER_OFF  0x01U
#define AVRCP_SYSTEM_STATUS_UNPLUGED   0x02U

/*@}*/


/**
 *  \name AVRCP Metadata Error Status Code.
 *  Constant Definitions for AVRCP Metadata Error Status Code.
 */

/*@{*/

#define AVRCP_METADATA_ERROR_INVALID_COMMAND        0x00U
#define AVRCP_METADATA_ERROR_INVALID_PARAMETER      0x01U
#define AVRCP_METADATA_ERROR_PARAMETER_NOT_FOUND    0x02U
#define AVRCP_METADATA_ERROR_INTERNAL               0x03U
#define AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL   0x04U
#define AVRCP_METADATA_ERROR_UID_CHANGED            0x05U

/* Deprecated in the current version of the specification */
/* #define AVRCP_METADATA_ERROR_UNKNOWN             0x06 */

/*@}*/

/**
 *  \name AVRCP Browsing Error Status Code.
 *  Constant Definitions for AVRCP Browsing Error Status Code.
 */

/*@{*/

#define AVRCP_BOW_ERROR_INVALID_DIRECTION           0x07U
#define AVRCP_BOW_ERROR_NOT_A_DIRECTORY             0x08U
#define AVRCP_BOW_ERROR_DOES_NOT_EXIST              0x09U
#define AVRCP_BOW_ERROR_INVALID_SCOPE               0x0AU
#define AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS         0x0BU
#define AVRCP_BOW_ERROR_DIRECTORY_NOT_HANDLED       0x0CU
#define AVRCP_BOW_ERROR_MEDIA_IN_USE                0x0DU
#define AVRCP_BOW_ERROR_NOW_PLAYING_LIST_FULL       0x0EU
#define AVRCP_BOW_ERROR_SEARCH_NOT_SUPPORTED        0x0FU
#define AVRCP_BOW_ERROR_SEARCH_IN_PROGRESS          0x10U
#define AVRCP_BOW_ERROR_INVALID_PLAYER_ID           0x11U
#define AVRCP_BOW_ERROR_PLAYER_NOT_BROWSABLE        0x12U
#define AVRCP_BOW_ERROR_PLAYER_NOT_ADDRESSED        0x13U
#define AVRCP_BOW_ERROR_NO_VALID_SEARCH_RESULTS     0x14U
#define AVRCP_BOW_ERROR_NO_AVAILABLE_PLAYERS        0x15U
#define AVRCP_BOW_ERROR_ADDR_PLAYER_CHANGED         0x16U
/*@}*/
#endif /* AVRCP_1_3 */
/** \} */

/* ----------------------------------------- Structures/Data Types */
/**
 * \defgroup avrcp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *  \typedef AVRCP_HANDLE
 *  \brief   AVRCP Handle.
 *
 *  This data type represents the abstract handle to refer while initiating
 *  a procedure with AVRCP. Except utility APIs, all AVRCP APIs require an
 *  AVRCP Handle to be provided.
 *
 *  \note
 *
 *  \sa
 *  AVRCP_EVENT_NTF_CB
 */
typedef struct _AVRCP_HANDLE
{
    /* AVRCP Profile Instance */
    UCHAR profile_inst;

    /* Remote Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} AVRCP_HANDLE;
/** \} */
/** \} */

/**
 * \defgroup avrcp_appl_callback Application Callback
 * \{
 */

/**
 *  \typedef AVRCP_EVENT_NTF_CB
 *  \brief   AVRCP Event Notification Callback
 *
 *  This data type represents the AVRCP Event Notification Callback, which
 *  AVRCP uses for the following purposes:
 *  \li    To confirm result of an operation initiated by the application.
 *  \li    To inform the application of asynchronous events.
 *
 *  \param [in] event_type
 *         This parameter defines the AVRCP event being reported. The valid
 *         values for this parameter are defined in "AVRCP Event Notification
 *         Callback Event Types" section.
 *  \param [in] event_result
 *         If the event being reported is a result of an earlier trigger (an
 *         AVRCP API) by the application, this parameter holds the result of
 *         the procedure. On successful completion of the procedure this
 *         parameter will have a value of API_SUCCESS, else it will
 *         contain an Error Code describing the reason for failure.
 *         For asynchronous event indications (e.g. AVRCP_CONNECT_IND), this
 *         parameter holds the possible reason due to which the event is being
 *         reported.
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE describing the
 *         application responsible for this event.
 *  \param [in] event_data
 *         Depending upon the event_type, this parameters holds any data
 *         being given to the AVRCP application. Description of any such
 *         data will be provided whenever appropriate in the description
 *         of AVRCP APIs that follows.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data .
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVRCP Error Code describing the reason of failure.
 *
 *  \par AVRCP Event Notification Callback
 *
 *  \sa
 *  AVRCP_HANDLE, AVRCP Event Notification Callback Event Types
 */
typedef API_RESULT (* AVRCP_EVENT_NTF_CB)
                   (
                       UCHAR          event_type,
                       UINT16         event_result,
                       AVRCP_HANDLE * avrcp_handle,
                       void  *        event_data,
                       UINT16         event_datalen
                   );

/** \} */

/**
 * \cond ignore_this
 * \{
 */

/**
 *  This section describes the use of AVRCP_EVENT_NTF_CB for AVRCP event
 *  notifications, that is, when called with the following event
 *  types:
 *      \li \ref Section_1 (AVRCP_CONNECT_IND)
 *      \li \ref Section_2 (AVRCP_CONNECT_CNF)
 *      \li \ref Section_3 (AVRCP_DISCONNECT_IND)
 *      \li \ref Section_4 (AVRCP_DISCONNECT_CNF)
 *      \li \ref Section_5 (AVRCP_MESSAGE_IND)
 *      \li \ref Section_6 (AVRCP_MESSAGE_CNF)
 *
 *  The sections below describe how AVRCP Event Notfication is called for
 *  events and also gives a guidelines on how they should be handled.
 *
 *
 *  \section Section_1 AVRCP Connect Indication
 *  The AVRCP will call the Event Notification Callback for this event
 *  whenever it receives the AVRCP Connect Request from a remote device.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_CONNECT_IND.
 *  \param [in] event_result
 *         This parameter will always be set to  API_SUCCESS.
 *
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter  will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is set to Zero.
 *
 *
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *
 *  \section Section_2 AVRCP Connect Confirmation
 *  The AVRCP will call the Event Notification Callback for this event when
 *  AVRCP Control Channel is established succssefullty on request of
 *  application.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_CONNECT_CNF.
 *  \param [in] event_result
 *         On successful completion of the procedure this parameter will have
 *         a value of API_SUCCESS, else it will contain an AVRCP Error Code
 *         describing the reason for failure.
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter is set to NULL.
 *  \param [in] event_datalen
 *         This parameter  is set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *
 *  \section Section_3 AVRCP Disconnect Indication
 *  The AVRCP will call the Event Notification Callback for this event when
 *  AVRCP Control Channel is disconnected by a remote device.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_DISCONNECT_IND.
 *  \param [in] event_result
 *         This parameter will always be set to  API_SUCCESS.
 *
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *
 *
 *  \section Section_4 AVRCP Disconnect Confirmation
 *  The AVRCP will call the Event Notification Callback for this event when
 *  AVRCP Control Channel is disconnected succssefullty on request of
 *  application using disconnect API.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_DISCONNECT_CNF.
 *  \param [in] event_result
 *         On successful completion of the procedure this parameter will have
 *         a value of API_SUCCESS, else it will contain an AVRCP Error Code
 *         describing the reason for failure.
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter  will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is  set to Zero.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *
 *
 *  \section Section_5 AVRCP Message Recieved Indication
 *  The AVRCP will call the Event Notification Callback for this event when
 *  AVRCP recieves AVRCP control command message packet.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_MESSAGE_IND.
 *  \param [in] event_result
 *         This parameter will always be set to  API_SUCCESS.
 *         If wrong packet received, AVRCP validates the packet and discards
 *         For AVRCP control response packet with invalid profile ID bit is
 *         set, this will have the error code.
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter is the AVRCP control message packet.
 *  \param [in] event_datalen
 *         This parameter is ths length of AVRCP control command message
 *         packet.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *
 *  \section Section_6 AVRCP Message Recieved Conformation
 *  The AVRCP will call the Event Notification Callback for this event when
 *  AVRCP recieves AVRCP control response message packetin intiator side.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVRCP_MESSAGE_CNF.
 *  \param [in] event_result
 *         This parameter will always be set to  API_SUCCESS.
 *         But for AVRCP control response packet with invalid profile ID
 *         bit is set, this will have the AVRCP error code. If wrong
 *         packets are received, AVRCP validates the packet and simply
 *         discards.
 *  \param [in] avrcp_handle
 *         This parameter is a pointer to AVRCP_HANDLE
 *  \param [in] event_data
 *         This parameter is the AVRCP control response message packet.
 *  \param [in] event_datalen
 *         This parameter is ths length of AVRCP control response message
 *         packet.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 */

/** \endcond */

/* ----------------------------------------- Macros */

/**
 *  \defgroup avrcp_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */

/**
 *  This utility macro initializes an AVRCP Handle.
 *
 *  \param [in] hdl
 *         The AVRCP Handle to be initialized.
 *
 *  \hideinitializer
 */
#define AVRCP_INIT_HANDLE(hdl)        \
        ((hdl).profile_inst) = 0xFFU; \
        BT_mem_set ((hdl).bd_addr, 0x00, BT_BD_ADDR_SIZE)


/**
 *  This utility macro sets BD_ADDR in an AVRCP Handle.
 *
 *  \param [in] hdl
 *         The AVRCP Handle
 *  \param [in] bd
 *         The BD_ADDR
 *
 *  \hideinitializer
 */
#define AVRCP_SET_HANDLE_BD_ADDR(hdl, bd) \
        BT_COPY_BD_ADDR((hdl).bd_addr, (bd))


/**
 *  This utility macro resets BD_ADDR in an AVRCP Handle.
 *
 *  \param [in] hdl
 *         The AVRCP Handle
 *
 *  \hideinitializer
 */
#define AVRCP_RESET_HANDLE_BD_ADDR(hdl) \
        BT_mem_set ((hdl).bd_addr, 0x00, BT_BD_ADDR_SIZE)


/**
 *  This utility macro gets BD_ADDR from an AVRCP Handle.
 *
 *  \param [in] hdl
 *         The AVRCP Handle
 *  \param [in] bd
 *         The BD_ADDR
 *
 *  \hideinitializer
 */
#define AVRCP_GET_HANDLE_BD_ADDR(hdl, bd) \
        BT_COPY_BD_ADDR((bd), (hdl).bd_addr)


/**
 *  This utility macro updates the AVRCP Control Command Header
 *  for PASS THROUGH Opcode.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [in] ct
 *         The Command Type of AVRCP Control Command Packet.
 *  \param [in] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *  \param [in] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *  \param [in] op_id
 *         The Operation ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_PASSTHROUGH_HDR(buffer, ct, su_type, su_id, op_id) \
        ((buffer)[0U]) = (ct);                                          \
        ((buffer)[1U]) = (((su_type) << 3U) | (su_id));                 \
        ((buffer)[2U]) = (AVRCP_OPCODE_PASS_THROUGH);                   \
        ((buffer)[3U]) = (op_id)


/**
 * This Utility macro forms the PASSTHROUGH VENDOR UNIQUE Control Packet
 *
 * \param [in] buffer
 *        The AVRCP VENDOR UNIQUE Control Command/Response Packet.
 *  \param [in] ct
 *         The Command Type of AVRCP Control Command Packet.
 *  \param [in] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *  \param [in] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *  \param [in] s_f
 *         The State flag of AVRCP Control Command Packet.
 *  \param [in] op_id
 *         The Vendor Unique operation ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_PASSTHROUGH_VU_PKT(buffer, ct, su_type, su_id, s_f, op_id) \
        ((buffer)[0U]) = (ct);                                                  \
        ((buffer)[1U]) = (((su_type) << 3U) | (su_id));                         \
        ((buffer)[2U]) = (AVRCP_OPCODE_PASS_THROUGH);                           \
        ((buffer)[3U]) = (((s_f) << 7U) | AVRCP_OPERATION_ID_VENDOR_UNIQUE);    \
        ((buffer)[4U]) = (0x05U);                                               \
        ((buffer)[5U]) = (UCHAR)((BT_SIG_REGISTERED_COMPANY_ID) >> 16U);        \
        ((buffer)[6U]) = (UCHAR)((BT_SIG_REGISTERED_COMPANY_ID) >> 8U);         \
        ((buffer)[7U]) = (UCHAR)(BT_SIG_REGISTERED_COMPANY_ID);                 \
        ((buffer)[8U]) = (UCHAR)((op_id) >> 8U);                                \
        ((buffer)[9U]) = (UCHAR)(op_id);


/**
 *  This utility macro updates the AVRCP Control Command Header
 *  for SUB UNIT INFO Opcode.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [in] ct
 *         The Command Type of AVRCP Control Command Packet.
 *  \param [in] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *  \param [in] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_SUBUNIT_HDR(buffer, ct, su_type, su_id) \
        ((buffer)[0U]) = (ct);                               \
        ((buffer)[1U]) = (((su_type) << 3U) | (su_id));      \
        ((buffer)[2U]) = (AVRCP_OPCODE_SUBUNIT_INFO)


/**
 *  This utility macro updates the AVRCP Control Command Header
 *  for UNIT INFO Opcode.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [in] ct
 *         The Command Type of AVRCP Control Command Packet.
 *  \param [in] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *  \param [in] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_UNIT_HDR(buffer, ct, su_type, su_id)    \
        ((buffer)[0U]) = (ct);                               \
        ((buffer)[1U]) = (((su_type) << 3U) | (su_id));      \
        ((buffer)[2U]) = (AVRCP_OPCODE_UNIT_INFO)


/**
 *  This utility macro updates the Command Type for the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [in] ctype
 *         The Commad Type of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_COMMAND_TYPE(buffer, ctype)             \
        ((buffer)[0U]) = (ctype)


/**
 *  This utility macro updates the Response Type from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [in] rtype
 *         The Response Type of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_RESPONSE_TYPE(buffer, rtype)             \
        ((buffer)[0U]) = (rtype)


/**
 *  This utility macro extarcts the Command Type from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] ctype
 *         The Command Type of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_COMMAND_TYPE(buffer, ctype)            \
        (ctype) = ((buffer)[1U] & 0x0FU)


/**
 *  This utility macro extarcts the Response Type from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] rtype
 *         The Response Type of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_RESPONSE_TYPE(buffer, rtype)           \
        (rtype) = ((buffer)[1U] & 0x0FU)


/**
 *  This utility macro extarcts the Sub Unit Type from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_SUBUNIT_TYPE(buffer, su_type)          \
        (su_type) = (((buffer)[2U]) >> 3U)


/**
 *  This utility macro extarcts the Sub Unit ID from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_SUBUNIT_ID(buffer, su_id)              \
        (su_id) = ((buffer)[2U] & 0x07U)


/**
 *  This utility macro extarcts the Opcode from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] opcode
 *         The Opcode of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_OPCODE(buffer, opcode)                 \
        (opcode) = ((buffer)[3U])


/**
 *  This utility macro extarcts the Operation ID from the AVRCP Control
 *  Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] opcode
 *         The Operation ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_OPERATION_ID(buffer, op_id)            \
        (op_id) = ((buffer)[4U] & 0x7FU)


/**
 *  This utility macro extracts the Operation ID and State_Flag
 *  from the AVRCP Control Command Header.
 *
 *  \param [in] buffer
 *         The AVRCP Control Command Packet.
 *  \param [out] op_id
 *         The Operation ID of AVRCP Control Command Packet.
 *  \param [out] state_flag
 *         The State Flag AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_MSG_OPERATION_INFO(buffer, op_id, state_flag)   \
        (op_id)      = (((buffer)[4U]) & (0x7FU));                \
        (state_flag) = (((buffer)[4U]) & (0x80U))



/**
 *  \name AVRCP 1.3 Adaptation Layer Utility Macros
 *  Defining AVRCP 1.3 Adaptation Layer Utility Macros
 */

#ifdef AVRCP_1_3

/**
 * This Utility macro updates the VENDOR DEPENDENT HDR for AVRCP metadata
 *
 * \param [in] buffer
 *        The AVRCP metadata Command/Response Packet.
 *  \param [in] ct
 *         The Command Type of AVRCP Control Command Packet.
 *  \param [in] su_type
 *         The Sub Unit Type of AVRCP Control Command Packet.
 *  \param [in] su_id
 *         The Sub Unit ID of AVRCP Control Command Packet.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_VD_METADATA_HDR(buffer, ct, su_type, su_id)         \
        ((buffer)[0U]) = (ct);                                           \
        ((buffer)[1U]) = (((su_type) << 3U) | (su_id));                  \
        ((buffer)[2U]) = (AVRCP_OPCODE_VENDOR_DEPENDENT);                \
        ((buffer)[3U]) = (UCHAR)((BT_SIG_REGISTERED_COMPANY_ID) >> 16U); \
        ((buffer)[4U]) = (UCHAR)((BT_SIG_REGISTERED_COMPANY_ID) >> 8U);  \
        ((buffer)[5U]) = (UCHAR)(BT_SIG_REGISTERED_COMPANY_ID);

/**
 * This Utility macro updates the metadata info for the
 * VENDOR DEPENDENT command/response.
 *
 * \param [in] buffer
 *        The AVRCP metadata Command/Response Packet.
 * \param [in] pdu_id
 *        The PDU ID of the AVRCP metadata PDU.
 * \param [in] packet_type
 *        The packet type of the AVRCP metadata PDU.
 * \param [in] param_len
 *        The parameter length of the AVRCP metadata parameter PDU.
 *
 *  \hideinitializer
 */
#define AVRCP_UPDATE_METADATA_INFO(buffer, pdu_id, packet_type, param_len)  \
        ((buffer)[6U]) = (pdu_id);                                          \
        ((buffer)[7U]) = (packet_type);                                     \
        ((buffer)[8U]) = (UCHAR)(((UINT16)(param_len)) >> 8U);              \
        ((buffer)[9U]) = (UCHAR)((UINT16)(param_len));


/**
 * This Utility macro packs a 2 byte metadata parameter.
 *
 * \param [in] buffer
 *        The buffer where the metadata parameter has to be packed.
 * \param [in] metadata
 *        The metadata parameter that has to be packed.
 *
 */
#define avrcp_pack_2_byte_metadata_param(buffer, metadata)      \
        ((buffer)[0U]) = (UCHAR)(*((UINT16*)(metadata)) >> 8U); \
        ((buffer)[1U]) = (UCHAR)(*((UINT16*)(metadata)));


/**
 * This Utility macro packs a 3 byte metadata parameter.
 *
 * \param [in] buffer
 *        The buffer where the metadata parameter has to be packed.
 * \param [in] metadata
 *        The metadata parameter that has to be packed.
 *
 */
#define avrcp_pack_3_byte_metadata_param(buffer, metadata)       \
        ((buffer)[0U]) = (UCHAR)(*((UINT32*)(metadata)) >> 16U); \
        ((buffer)[1U]) = (UCHAR)(*((UINT32*)(metadata)) >> 8U);  \
        ((buffer)[2U]) = (UCHAR)(*((UINT32*)(metadata)));


/**
 * This Utility macro packs a 4 byte metadata parameter.
 *
 * \param [in] buffer
 *        The buffer where the metadata parameter has to be packed.
 * \param [in] metadata
 *        The metadata parameter that has to be packed.
 *
 */
#define avrcp_pack_4_byte_metadata_param(buffer, metadata)       \
        ((buffer)[0U]) = (UCHAR)(*((UINT32*)(metadata)) >> 24U); \
        ((buffer)[1U]) = (UCHAR)(*((UINT32*)(metadata)) >> 16U); \
        ((buffer)[2U]) = (UCHAR)(*((UINT32*)(metadata)) >> 8U);  \
        ((buffer)[3U]) = (UCHAR)(*((UINT32*)(metadata)));

/**
 * This Utility macro extracts the PDU ID from AVRCP VENDOR DEPENDENT
 * Command/Response.
 *
 * \param [in] buffer
 *        The AVRCP metadata Command/Response Packet.
 * \param [out] pdu_id
 *        The PDU ID of the AVRCP metadata PDU.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_METADATA_PDU_ID(buffer, pdu_id)  \
        *((UCHAR *)(pdu_id)) = ((buffer)[7U]);

/**
 * This Utility macro extracts the Packet Type from AVRCP VENDOR DEPENDENT
 * Command/Response.
 *
 * \param [in] buffer
 *        The AVRCP metadata Command/Response Packet.
 * \param [out] packet_type
 *        The Packet_type of the AVRCP metadata PDU.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_METADATA_PACKET_TYPE(buffer, packet_type)  \
        *((UCHAR *)(packet_type)) = ((buffer)[8U] & 0x03U);


/**
 * This Utility macro extracts the Parameter Length from AVRCP VENDOR DEPENDENT
 * Command/Response.
 *
 * \param [in] buffer
 *        The AVRCP metadata Command/Response Packet.
 * \param [out] param_len
 *        The Parameter Length of the AVRCP metadata PDU.
 *
 *  \hideinitializer
 */
#define AVRCP_GET_METADATA_PARAM_LEN(buffer, param_len)             \
        *((UINT16 *)(param_len))  = ((buffer)[9U]);                 \
        *((UINT16 *)(param_len))  = *((UINT16 *)(param_len)) << 8U; \
        *((UINT16 *)(param_len)) |= ((buffer)[10U]);


/**
 * This Utility macro extracts 2 Byte metadata parameter
 *
 * \param [in] buffer
 *        The buffer from which metadata parameter has to be extracted.
 * \param [out] metadata
 *        The metadata to be extracted.
 *
 *  \hideinitializer
 */
#define avrcp_extract_2_byte_metadata_param(buffer, metadata)     \
        *((UINT16 *)(metadata))  = ((buffer)[0U]);                \
        *((UINT16 *)(metadata))  = *((UINT16 *)(metadata)) << 8U; \
        *((UINT16 *)(metadata)) |= ((buffer)[1U]);


/**
 * This Utility macro extracts 3 Byte metadata parameter
 *
 * \param [in] buffer
 *        The buffer from which metadata parameter has to be extracted.
 * \param [out] metadata
 *        The metadata to be extracted.
 *
 *  \hideinitializer
 */
#define avrcp_extract_3_byte_metadata_param(buffer, metadata)     \
        *((UINT32 *)(metadata))  = ((buffer)[0U]);                \
        *((UINT32 *)(metadata))  = *((UINT32 *)(metadata)) << 8U; \
        *((UINT32 *)(metadata)) |= ((buffer)[1U]);                \
        *((UINT32 *)(metadata))  = *((UINT32 *)(metadata)) << 8U; \
        *((UINT32 *)(metadata)) |= ((buffer)[2U]);

/**
 * This Utility macro extracts 4 Byte metadata parameter.
 *
 * \param [in] buffer
 *        The buffer from which metadata parameter has to be extracted.
 * \param [out] metadata
 *        The metadata to be extracted.
 *
 *  \hideinitializer
 */
#define avrcp_extract_4_byte_metadata_param(buffer, metadata)     \
        *((UINT32 *)(metadata))  = ((buffer)[0U]);                \
        *((UINT32 *)(metadata))  = *((UINT32 *)(metadata)) << 8U; \
        *((UINT32 *)(metadata)) |= ((buffer)[1U]);                \
        *((UINT32 *)(metadata))  = *((UINT32 *)(metadata)) << 8U; \
        *((UINT32 *)(metadata)) |= ((buffer)[2U]);                \
        *((UINT32 *)(metadata))  = *((UINT32 *)(metadata)) << 8U; \
        *((UINT32 *)(metadata)) |= ((buffer)[3U]);


#ifdef AVRCP_1_4
/**
 * This utility macros extracts the UID persistant bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_UID_PERSISTENT(buffer) \
        (((buffer)[8U])&(0x04U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_UID_PERSISTENT(buffer) \
        (((buffer)[8U])|=(0x04U))

/**
 * This utility macros extracts the now playing  bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_NOW_PLAYING(buffer) \
        (((buffer)[8U])&(0x02U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_NOW_PLAYING(buffer) \
        (((buffer)[8U])|=(0x02U))

#define AVRCP_MP_GET_FEATURE_BIT_ONLY_SEARCHABLE_WHEN_ADDR(buffer) \
        (((buffer)[8U])&(0x01U))

#define AVRCP_MP_SET_FEATURE_BIT_ONLY_SEARCHABLE_WHEN_ADDR(buffer) \
        (((buffer)[8U])|=(0x01U))

#define AVRCP_MP_GET_FEATURE_BIT_ONLY_BROWSABLE_WHEN_ADDR(buffer) \
        (((buffer)[7U])&(0x80U))

#define AVRCP_MP_SET_FEATURE_BIT_ONLY_BROWSABLE_WHEN_ADDR(buffer) \
        (((buffer)[7U])|=(0x80U))
/**
 * This utility macros extracts the unique UID over FS bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_UUID(buffer) \
        (((buffer)[7U])&(0x40U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_UUID(buffer) \
        (((buffer)[7U])|=(0x40U))

/**
 * This utility macros extracts the add to now playing bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_A2NP(buffer) \
        (((buffer)[7U])&(0x20U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_A2NP(buffer) \
        (((buffer)[7U])|=(0x20U))

/**
 * This utility macros extracts the searching bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_SEARCHING(buffer) \
        (((buffer)[7U])&(0x10U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_SEARCHING(buffer) \
        (((buffer)[7U])|=(0x10U))

/**
 * This utility macros extracts the browsing bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_BROWSING(buffer) \
        (((buffer)[7U])&(0x08U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_BROWSING(buffer) \
        (((buffer)[7U])|=(0x08U))

/**
 * This utility macros extracts the avrcp 1.4 bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_AVRCP_1_4(buffer) \
        (((buffer)[7U])&(0x04U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_AVRCP_1_4(buffer) \
        (((buffer)[7U])&(0x04U))

#define AVRCP_MP_GET_FEATURE_BIT_VENDOR_UNIQUE(buffer) \
        (((buffer)[7U])&(0x01U))

#define AVRCP_MP_SET_FEATURE_BIT_VENDOR_UNIQUE(buffer) \
        (((buffer)[7U])|=(0x01U))

/**
 * This utility macros extracts the backward bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_BACKWARD(buffer) \
        (((buffer)[6U])&(0x01U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_BACKWARD(buffer) \
        (((buffer)[6U])|=(0x01U))

/**
 * This utility macros extracts the forward bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_FORWARD(buffer) \
        (((buffer)[5U])&(0x80U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_FORWARD(buffer) \
        (((buffer)[5U])|=(0x80U))

/**
 * This utility macros extracts the eject bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_EJECT(buffer) \
        (((buffer)[5U])&(0x40U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_EJECT(buffer) \
        (((buffer)[5U])|=(0x40U))

/**
 * This utility macros extracts the fast forward bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_FAST_FWD(buffer) \
        (((buffer)[5U])&(0x20U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_FAST_FWD(buffer) \
        (((buffer)[5U])|=(0x20U))

/**
 * This utility macros extracts the rewind bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_REW(buffer) \
        (((buffer)[5U])&(0x10U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_REW(buffer) \
        (((buffer)[5U])|=(0x10U))

/**
 * This utility macros extracts the pause bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_PAUSE(buffer) \
        (((buffer)[5U])&(0x04U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_PAUSE(buffer) \
        (((buffer)[5U])|=(0x04U))

/**
 * This utility macros extracts the stop bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_STOP(buffer) \
        (((buffer)[5U])&(0x02U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_STOP(buffer) \
        (((buffer)[5U])|=(0x02U))

/**
 * This utility macros extracts the play bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_PLAY(buffer) \
        (((buffer)[5U])&(0x01U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_PLAY(buffer) \
        (((buffer)[5U])|=(0x01U))


/**
 * This utility macros extracts the mute bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_MUTE(buffer) \
        (((buffer)[4U])&(0x80U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_MUTE(buffer) \
        (((buffer)[4U])|=(0x80U))

/**
 * This utility macros extracts the volume down bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_VOL_DOWN(buffer) \
        (((buffer)[4U])&(0x40U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_VOL_DOWN(buffer) \
        (((buffer)[4U])|=(0x40U))

/**
 * This utility macros extracts the volume up  bit
 * in the Feature Bit Mask.
 *
 * \param [in] buffer
 *        The buffer from which the feature bit is extracted
 */
#define AVRCP_MP_GET_FEATURE_BIT_VOL_UP(buffer) \
        (((buffer)[4U])&(0x20U))

/**
 * \param [out] buffer
 *        The buffer in which the fearture bit is set
 */
#define AVRCP_MP_SET_FEATURE_BIT_VOL_UP(buffer) \
        (((buffer)[4U])|=(0x20U))

#endif /* AVRCP_1_4 */

#endif /* AVRCP_1_3 */

/** \} */
/** \} */

/* ----------------------------------------- APIs */

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup avrcp_api API Definitions
 * \{
 * Describes API definitions of this module.
 */
/**
 *  \name AVRCP APIs - Intitialization and Shutdown
 */

/*@{*/

/**
 *  \brief To initialize AVRCP module.
 *
 *  \par Description:
 *  This API is used to initialize the AVRCP module. It initializes
 *  the AVRCP global data structures and synchronization primitives
 *  used in this module.
 *  AVRCP module.
 *
 *  \param None
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - AVCTP Error Code describing the reason of failure.
 *
 *  \note
 *  This function must be called only once as a part of initialization
 *  routine of the complete system.
 *
 *  \sa
 *  BT_avrcp_shutdown
 */
API_RESULT BT_avrcp_init ( void );


/**
 *  \brief To shutdown AVRCP module.
 *
 *  \par Description:
 *  This API is used to terminate the AVRCP module. It disconnects all
 *  AVRCP controls connections if present, resets all AVRCP Profile Instances,
 *  dereigsters AVRCP module from  AVCTP .
 *
 *  \param None
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \note
 *
 *  \sa
 *  BT_avrcp_init
 */
API_RESULT BT_avrcp_shutdown ( void );

/*@}*/


/**
 *  \name AVRCP APIs - Start and Stop of AVRCP profile instance
 */

/*@{*/

/**
 *  \brief To start an instance of AVRCP Profile.
 *
 *  \par Description:
 *  This API starts an instance of AVRCP Profile. On the successful
 *  instantiation, this API returns a AVRCP Handle which is an index
 *  to the AVRCP Profile Entity. This also registers the callback function
 *  that AVRCP needs to call when any event occurs for this AVRCP Profile
 *  Instance.
 *
 *  \param AVRCP Handle (OUT)
 *         Denotes the AVRCP Profile instance for newly started AVRCP instance.
 *  \param [in] ntf_cb
 *         Callback function which will be called for all
 *         Event Notifications for this Profile Enity.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \note
 *  The AVRCP Handle shall be used hereafter by the application to initiate
 *  any procedure (API).
 *
 *  \sa
 *  BT_avrcp_stop
 */
API_RESULT BT_avrcp_start
           (
               /* OUT */  AVRCP_HANDLE *      handle,
               /* IN */   AVRCP_EVENT_NTF_CB  ntf_cb
           );


/**
 *  \brief To stop an instance of AVRCP Profile.
 *
 *  \par Description:
 *   This API stops the instance of AVRCP profile.
 *   It disconnects all AVRCP control connections
 *   if present and stops the running timers..
 *
 *  \param AVRCP Handle (IN)
 *           Profile Instance representing this Profile.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \note
 *
 *  \sa
 *  BT_avrcp_start
 */
API_RESULT BT_avrcp_stop
           (
               /* IN */ AVRCP_HANDLE * handle
           );

/*@}*/


/**
 *  \name AVRCP APIs - Connection Management
 */

/*@{*/

/**
 *
 *  \brief To Establish a control connection with a remote device.
 *
 *  \par Description:
 *  This API is used to establish a control connection with a remote
 *  AVRCP device.
 *
 *  \param [in] handle
 *         Denotes profile instance representing this profile.
 *  \param [in] bd_addr
 *         Bluetooth Device Address of the remote device.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  All connections are created by the application by using this API.
 *
 *  \sa
 *  BT_avrcp_disconnect_req, AVRCP_HANDLE, AVRCP_EVENT_NTF_CB
 */
API_RESULT BT_avrcp_channel_connect_req
           (
               /* IN */  AVRCP_HANDLE * handle,
               /* IN */  UCHAR *        bd_addr,
               /* IN */  UCHAR          channel_type
           );

/**
 *  \brief To disconnect a control connection with a remote device.
 *
 *  \par Description:
 *  This API is used to disconnect a control connection with a remote
 *  AVRCP device.
 *
 *  \param [in] handle
 *         Denotes profile instance representing this profile.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  \sa
 *  BT_avrcp_connect_req, AVRCP_HANDLE, AVRCP_EVENT_NTF_CB
 */
API_RESULT BT_avrcp_channel_disconnect_req
           (
               /* IN */ AVRCP_HANDLE * handle,
               /* IN */ UCHAR          channel_type
           );

/*@}*/

/**
 *  \name AVRCP APIs - Control Message Exchange Management
 */

/*@{*/

/**
 *  \brief To send AVRCP control command messages.
 *
 *  \par Description:
 *  This API is used to send AVRCP control command messages over an AVRCP
 *  control connection to the remote AVRCP device.
 *
 *  \param [in] handle
 *         The AVRCP Handle, Profile Instance representing this Profile.
 *  \param [in] data
 *         Buffer containing AVRCP Control Message.
 *  \param [in] datalen
 *         AVRCP Control Message length.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \remark
 *  The application should provide a complete AVRCP Control Message Packet for
 *  transmission, which includes the following:
 *  - Space for L2CAP Header - 4 Bytes. This will be filled by L2CAP module.
 *  - Space for AVCTP Control Message Packet Header - 3 Bytes.
 *    This will be filled by AVRCP and AVCTP module.
 *  - Space for AVRCP Control Message Packet Header - 3 Bytes.
 *    This must be filled by application.
 *  - AVRCP Control Message Packet Payload.
 *
 *  \par
 *  The memory for AVRCP Control Message Packet should always be dynamically
 *  allocated, and it will be freed by the EtherMind Stack, upon successful
 *  transmission.
 *
 *  \remarks
 *  This API should be called to send AVRCP Control Commands only after the
 *  AVRCP module has given the indication for connected state.
 *  The connected state is set only when the AVRCP notifies application
 *  with AVRCP_CONNECT_CNF or AVRCP_CONNECT_IND events.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  \sa
 *  AVRCP_HANDLE, AVRCP_EVENT_NTF_CB
 */
API_RESULT BT_avrcp_send_cmd
           (
               /* IN */  AVRCP_HANDLE * handle,
               /* IN */  UCHAR  *       data,
               /* IN */  UINT16         datalen,
               /* IN */  UCHAR          channel_type
           );


/**
 *  \brief To send AVRCP control response messages.
 *
 *  \par Description:
 *  This API is used to send AVRCP control response messages over an AVRCP
 *  control connection with a remote AVRCP device.
 *
 *  \param [in] handle
 *         The AVRCP Handle, Profile Instance representing this Profile.
 *  \param [in] data
 *         Buffer containing AVRCP Control Response Message.
 *  \param [in] datalen
 *         AVRCP Control Message command length.
 *  \param [in] channel_type
 *         Channel Type - Command or Browsing.
 *
 *  \return API_RESULT
 *  - API_SUCCESS on successful initiation of the procedure.
 *  - Error Code describing the reason of failure.
 *
 *  \remark
 *  The application should provide a complete AVRCP Control Message Packet for
 *  transmission, which includes the following:
 *  - Space for L2CAP Header - 4 Bytes. This will be filled by L2CAP module.
 *  - Space for AVCTP Control Message Packet Header - 3 Bytes.
 *    This will be filled by AVRCP and AVCTP module.
 *  - Space for AVRCP Control Message Packet Header - 3 Bytes.
 *    This must be filled by application.
 *  - AVRCP Control Message Packet Payload.
 *
 *  \par
 *  The memory for AVRCP Control Message Packet should always be dynamically
 *  allocated, and it will be freed by the EtherMind Stack, upon successful
 *  transmission.
 *
 *  \remarks
 *  This API should be called to send AVRCP Control Resposne only after the
 *  AVRCP module has given the indication for connected state.
 *  The connected state is set only when the AVRCP notifies application
 *  with AVRCP_CONNECT_CNF or AVRCP_CONNECT_IND events.
 *
 *  \note
 *  Below is the example of how to use this API:
 *
 *  \sa
 *  AVRCP_HANDLE
 */
API_RESULT BT_avrcp_send_rsp
           (
               /* IN */  AVRCP_HANDLE * handle,
               /* IN */  UCHAR  *       data,
               /* IN */  UINT16         datalen,
               /* IN */  UCHAR          channel_type
           );

/*@}*/

/**
 *  \name AVRCP APIs - Utility APIs
 */

/*@{*/


/**
 *  \brief To set InMTU for AVRCP Control Channel.
 *
 *  \par Description:
 *  This API enables application to set the In MTU value for
 *  AVRCP Control Channel. This value will be indicated to the
 *  peer device during connection establishment procedure.
 *
 *  \param [in] in_mtu
 *         Value of In MTU in octects.
 *
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 */
API_RESULT BT_avrcp_set_mtu
           (
               /* IN */  UINT16 in_mtu
           );

/*@}*/


/**
 *   Abstraction to AVRCP 1.0 /1.3 APIs for backward compatibility
 */
#define BT_avrcp_connect_req(handle, bd_addr) \
        BT_avrcp_channel_connect_req          \
        (                                     \
            (handle),                         \
            (bd_addr),                        \
            AVCTP_CHANNEL_TYPE_COMMAND        \
        )

#define BT_avrcp_disconnect_req(handle)       \
        BT_avrcp_channel_disconnect_req       \
        (                                     \
            (handle),                         \
            AVCTP_CHANNEL_TYPE_COMMAND        \
        )

#define BT_avrcp_send_control_cmd(handle, data, datalen) \
        BT_avrcp_send_cmd                                \
        (                                                \
            (handle),                                    \
            (data),                                      \
            (datalen),                                   \
            AVCTP_CHANNEL_TYPE_COMMAND                   \
        )

#define BT_avrcp_send_control_rsp(handle, data, datalen) \
        BT_avrcp_send_rsp                                \
        (                                                \
            (handle),                                    \
            (data),                                      \
            (datalen),                                   \
            AVCTP_CHANNEL_TYPE_COMMAND                   \
        )

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_AVRCP_API_ */

