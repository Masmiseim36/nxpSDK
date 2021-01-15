
/**
 *  \file BT_map_api.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_MAP_API_
#define _H_BT_MAP_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"


/* --------------------------------------------- Global Definitions */

#ifdef MAP_1_2
/* OBEX L2CAP PSM for local client entities */
#define MAP_OBEX_L2CAP_PSM                       0x8601
#endif /* MAP_1_2 */

/* MAP Notification Callback Event types */
#ifdef MAP_MCE
#define MAP_MCE_CONNECT_CNF                      0x01
#define MAP_MCE_DISCONNECT_CNF                   0x02
#define MAP_MCE_TRANSPORT_CLOSE_CNF              0x03
#define MAP_MCE_TRANSPORT_CLOSE_IND              0x04

#define MAP_MCE_UPDATE_INBOX_CNF                 0x05
#define MAP_MCE_SET_FOLDER_CNF                   0x06
#define MAP_MCE_GET_FOLDER_LISTING_CNF           0x07
#define MAP_MCE_GET_MESSAGES_LISTING_CNF         0x08
#define MAP_MCE_GET_MESSAGE_CNF                  0x09
#define MAP_MCE_SET_MESSAGE_STATUS_CNF           0x0A
#define MAP_MCE_PUSH_MESSAGE_CNF                 0x0B
#define MAP_MCE_SET_NTF_REGISTRATION_CNF         0x0C

#ifdef MAP_1_2
#define MAP_MCE_GET_MASINSTINFO_CNF              0x0E
#endif /* MAP_1_2 */

#ifdef MAP_1_3
#define MAP_MCE_GET_CONVERSATION_LIST_CNF        0x61
#define MAP_MCE_SET_OWNER_STATUS_CNF             0x62
#define MAP_MCE_GET_OWNER_STATUS_CNF             0x63
#define MAP_MCE_SET_NOTIFICATION_FILTER_CNF      0x64
#endif /* MAP_1_3 */

#define MAP_MCE_NS_CONNECT_IND                   0x10
#define MAP_MCE_NS_DISCONNECT_IND                0x11
#define MAP_MCE_NS_TRANSPORT_CLOSE_IND           0x12
#define MAP_MCE_NS_TRANSPORT_CLOSE_CNF           0x13
#define MAP_MCE_NS_EVENT_REPORT_IND              0x14

/* To be deprecated */
#define MAP_MCE_ABORT_CNF                        MAP_ABORT_CNF
#define MAP_MCE_ABORT_IND                        MAP_ABORT_IND

#endif /* MAP_MCE */

#ifdef MAP_MSE
#define MAP_MSE_CONNECT_IND                      0x21
#define MAP_MSE_DISCONNECT_IND                   0x22
#define MAP_MSE_TRANSPORT_CLOSE_IND              0x23
#define MAP_MSE_TRANSPORT_CLOSE_CNF              0x24

#define MAP_MSE_UPDATE_INBOX_IND                 0x25
#define MAP_MSE_SET_FOLDER_IND                   0x26
#define MAP_MSE_GET_FOLDER_LISTING_IND           0x27
#define MAP_MSE_GET_MESSAGES_LISTING_IND         0x28
#define MAP_MSE_GET_MESSAGE_IND                  0x29
#define MAP_MSE_SET_MESSAGE_STATUS_IND           0x2A
#define MAP_MSE_PUSH_MESSAGE_IND                 0x2B
#define MAP_MSE_SET_NTF_REGISTRATION_IND         0x2C

#ifdef MAP_1_2
#define MAP_MSE_GET_MASINSTINFO_IND              0x2E
#endif /* MAP_1_2 */

#ifdef MAP_1_3
#define MAP_MSE_GET_CONVERSATION_LIST_IND        0x81
#define MAP_MSE_SET_OWNER_STATUS_IND             0x82
#define MAP_MSE_GET_OWNER_STATUS_IND             0x83
#define MAP_MSE_SET_NOTIFICATION_FILTER_IND      0x84
#endif /* MAP_1_3 */

#define MAP_MSE_NS_CONNECT_CNF                   0x30
#define MAP_MSE_NS_DISCONNECT_CNF                0x31
#define MAP_MSE_NS_TRANSPORT_CLOSE_CNF           0x32
#define MAP_MSE_NS_TRANSPORT_CLOSE_IND           0x33
#define MAP_MSE_NS_SEND_EVENT_CNF                0x34

/* To be deprecated */
#define MAP_MSE_ABORT_CNF                        MAP_ABORT_CNF
#define MAP_MSE_ABORT_IND                        MAP_ABORT_IND

#endif /* MAP_MSE */

/* Common Event codes */
#define MAP_ABORT_CNF                            0x40
#define MAP_ABORT_IND                            0x50
#define MAP_EVENT_INVALID                        0xFF

/* Invalid entity indentifiers */
#define MAP_ENTITY_INVALID                       0xFF
#define MAP_INVALID_OBEX_CONNECTION_ID           0xFFFFFFFF

/* MAP OBEX Response Codes */
#define MAP_SUCCESS_RSP                          OBEX_SUCCESS_RSP
#define MAP_CONTINUE_RSP                         OBEX_CONTINUE_RSP

#define MAP_BAD_REQ_RSP                          OBEX_REQ_IS_BAD_RSP
#define MAP_NOT_IMPLEMENTED_RSP                  OBEX_NOT_IMP_RSP
#define MAP_UNAUTH_RSP                           OBEX_UNAUTHORIZED_RSP
#define MAP_PRECOND_FAILED_RSP                   OBEX_PRECOND_FAIL_RSP
#define MAP_NOT_FOUND_RSP                        OBEX_NOT_FOUND_RSP
#define MAP_NOT_ACCEPTABLE_RSP                   OBEX_NOT_ACCEPTABLE_RSP
#define MAP_NO_SERVICE_RSP                       OBEX_NO_SERVICE_RSP
#define MAP_FORBIDDEN_RSP                        OBEX_FORBIDDEN_RSP
#define MAP_SERVER_ERROR                         OBEX_SERVER_ERR_RSP

/* MAP Services OBEX Target header UUID values */
#define MAP_MAS_OBEX_TARGET_HDR_UUID \
        0xBB582B40420C11DBB0DE0800200C9A66
#define MAP_MNS_OBEX_TARGET_HDR_UUID \
        0xBB582B41420C11DBB0DE0800200C9A66

/* MAP Target Header Size */
#define MAP_TARGET_HDR_SIZE                      0x10 /* 16 */

/* MAP Application Parameters header Tag IDs */
#define MAP_TAG_ID_MAXLISTCOUNT                  0x01  /* Length : 2 bytes */
#define MAP_TAG_ID_STARTOFFSET                   0x02  /* Length : 2 bytes */
#define MAP_TAG_ID_FILTERMESSAGETYPE             0x03  /* Length : 1 byte */
#define MAP_TAG_ID_FILTERPERIODBEGIN             0x04  /* Length : variable */
#define MAP_TAG_ID_FILTERPERIODEND               0x05  /* Length : variable */
#define MAP_TAG_ID_FILTERREADSTATUS              0x06  /* Length : 1 byte */
#define MAP_TAG_ID_FILTERRECIPIENT               0x07  /* Length : variable */
#define MAP_TAG_ID_FILTERORIGINATOR              0x08  /* Length : variable */
#define MAP_TAG_ID_FILTERPRIORITY                0x09  /* Length : 1 byte */
#define MAP_TAG_ID_ATTACHMENT                    0x0A  /* Length : 1 byte */
#define MAP_TAG_ID_TRANSPARENT                   0x0B  /* Length : 1 byte */
#define MAP_TAG_ID_RETRY                         0x0C  /* Length : 1 byte */
#define MAP_TAG_ID_NEWMESSAGE                    0x0D  /* Length : 1 byte */
#define MAP_TAG_ID_NOTIFICATIONSTATUS            0x0E  /* Length : 1 byte */
#define MAP_TAG_ID_MASINSTANCEID                 0x0F  /* Length : 1 byte */
#define MAP_TAG_ID_PARAMETERMASK                 0x10  /* Length : 4 bytes */
#define MAP_TAG_ID_FOLDERLISTINGSIZE             0x11  /* Length : 2 bytes */
#define MAP_TAG_ID_MESSAGESLISTINGSIZE           0x12  /* Length : 2 bytes */
#define MAP_TAG_ID_SUBJECTLENGTH                 0x13  /* Length : variable (max 255 bytes) */
#define MAP_TAG_ID_CHARSET                       0x14  /* Length : 1 bytes */
#define MAP_TAG_ID_FRACTIONREQUEST               0x15  /* Length : 1 bytes */
#define MAP_TAG_ID_FRACTIONDELIVER               0x16  /* Length : 1 bytes */
#define MAP_TAG_ID_STATUSINDICATOR               0x17  /* Length : 1 bytes */
#define MAP_TAG_ID_STATUSVALUE                   0x18  /* Length : 1 bytes */
#define MAP_TAG_ID_MSETIME                       0x19  /* Length : variable */

#ifdef MAP_1_3
#define MAP_TAG_ID_DATABASE_IDENTIFIER           0x1A  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_CONV_LIST_VER_CNTR            0x1B  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_PRESENCE_AVAILABILITY         0x1C  /* Length : 1 bytes */
#define MAP_TAG_ID_PRESENCE_TEXT                 0x1D  /* Length : variable */
#define MAP_TAG_ID_LAST_ACTIVITY                 0x1E  /* Length : variable */
#define MAP_TAG_ID_FILTERLASTACTIVITYBEGIN       0x1F  /* Length : variable */
#define MAP_TAG_ID_FILTERLASTACTIVITYEND         0x20  /* Length : variable */

#define MAP_TAG_ID_CHAT_STATE                    0x21  /* Length : 1 bytes */
#define MAP_TAG_ID_CONVERSATION_ID               0x22  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_FOLDER_VER_CNTR               0x23  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_FILTER_MSG_HANDLE             0x24  /* Length : variable (max 16 bytes) */
#define MAP_TAG_ID_NOTIFICATION_FILTER_MASK      0x25  /* Length : 4 bytes */
#define MAP_TAG_ID_CONV_PARAMETER_MASK           0x26  /* Length : 4 bytes */
#define MAP_TAG_ID_OWNER_UCI                     0x27  /* Length : variable */
#define MAP_TAG_ID_EXTENTED_DATA                 0x28  /* Length : variable */
#define MAP_TAG_ID_MAP_SUPPORTED_FEATURES        0x29  /* Length : 4 bytes */
#endif /* MAP_1_3 */

#ifdef MAP_1_4
#define MAP_TAG_ID_MESSAGE_HANDLE                0x2A  /* Length : 8 bytes */
#define MAP_TAG_ID_MODIFY_TEXT                   0x2B  /* Length : 1 byte */
#endif /* MAP_1_4 */

/* Maximum number of Application Header Tag IDs */
#if (defined MAP_1_4)
#define MAP_NUM_APPL_HEADERS                     0x2B
#elif (defined MAP_1_3)
#define MAP_NUM_APPL_HEADERS                     0x29
#else
#define MAP_NUM_APPL_HEADERS                     0x19
#endif /* MAP_1_4 */

/* MAP Application parameters flag corresponding to each Tag */
#define MAP_FLAG_MAXLISTCOUNT                    0x00000001
#define MAP_FLAG_STARTOFFSET                     0x00000002
#define MAP_FLAG_FILTERMESSAGETYPE               0x00000004
#define MAP_FLAG_FILTERPERIODBEGIN               0x00000008
#define MAP_FLAG_FILTERPERIODEND                 0x00000010
#define MAP_FLAG_FILTERREADSTATUS                0x00000020
#define MAP_FLAG_FILTERRECIPIENT                 0x00000040
#define MAP_FLAG_FILTERORIGINATOR                0x00000080
#define MAP_FLAG_FILTERPRIORITY                  0x00000100
#define MAP_FLAG_ATTACHMENT                      0x00000200
#define MAP_FLAG_TRANSPARENT                     0x00000400
#define MAP_FLAG_RETRY                           0x00000800
#define MAP_FLAG_NEWMESSAGE                      0x00001000
#define MAP_FLAG_NOTIFICATIONSTATUS              0x00002000
#define MAP_FLAG_MASINSTANCEID                   0x00004000
#define MAP_FLAG_PARAMETERMASK                   0x00008000
#define MAP_FLAG_FOLDERLISTINGSIZE               0x00010000
#define MAP_FLAG_MESSAGESLISTINGSIZE             0x00020000
#define MAP_FLAG_SUBJECTLENGTH                   0x00040000
#define MAP_FLAG_CHARSET                         0x00080000
#define MAP_FLAG_FRACTIONREQUEST                 0x00100000
#define MAP_FLAG_FRACTIONDELIVER                 0x00200000
#define MAP_FLAG_STATUSINDICATOR                 0x00400000
#define MAP_FLAG_STATUSVALUE                     0x00800000
#define MAP_FLAG_MSETIME                         0x01000000

#ifdef MAP_1_3
#define MAP_FLAG_DATABASE_IDENTIFIER             0x02000000
#define MAP_FLAG_CONV_LIST_VER_CNTR              0x04000000
#define MAP_FLAG_PRESENCE_AVAILABILITY           0x08000000
#define MAP_FLAG_PRESENCE_TEXT                   0x10000000
#define MAP_FLAG_LAST_ACTIVITY                   0x20000000
#define MAP_FLAG_FILTERLASTACTIVITYBEGIN         0x40000000
#define MAP_FLAG_FILTERLASTACTIVITYEND           0x80000000

/* From 33rd application parameter */
#define MAP_FLAG_CHAT_STATE                      0x00000001
#define MAP_FLAG_CONVERSATION_ID                 0x00000002
#define MAP_FLAG_FOLDER_VER_CNTR                 0x00000004
#define MAP_FLAG_FILTER_MSG_HANDLE               0x00000008
#define MAP_FLAG_NOTIFICATION_FILTER_MASK        0x00000010
#define MAP_FLAG_CONV_PARAMETER_MASK             0x00000020
#define MAP_FLAG_OWNER_UCI                       0x00000040
#define MAP_FLAG_EXTENDED_DATA                   0x00000080
#define MAP_FLAG_MAP_SUPPORTED_FEATURES          0x00000100
#endif /* MAP_1_3 */

#ifdef MAP_1_4
#define MAP_FLAG_MESSAGE_HANDLE                  0x00000200
#define MAP_FLAG_MODIFY_TEXT                     0x00000400
#endif /* MAP_1_4 */

/* MAP SetPath parameter values */
#define MAP_SET_CHILD_FOLDER                     0x01
#define MAP_SET_PARENT_FOLDER                    0x02
#define MAP_SET_ROOT_FOLDER                      0x03

/**
 * MAPv1.3 Instant Messaging Availability Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
#define MAP_IM_PRESENCE_UNKNOWN                  0x00
#define MAP_IM_PRESENCE_OFFLINE                  0x01
#define MAP_IM_PRESENCE_ONLINE                   0x02
#define MAP_IM_PRESENCE_AWAY                     0x03
#define MAP_IM_PRESENCE_DO_NOT_DISTURB           0x04
#define MAP_IM_PRESENCE_BUSY                     0x05
#define MAP_IM_PRESENCE_IN_MEETING               0x06

/**
 * MAPv1.3 IM Chat State Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
#define MAP_IM_CHAT_STATE_UNKNOWN               0x00
#define MAP_IM_CHAT_STATE_INACTIVE              0x01
#define MAP_IM_CHAT_STATE_ACTIVE                0x02
#define MAP_IM_CHAT_STATE_COMPOSING             0x03
#define MAP_IM_CHAT_STATE_PAUSED_COMPOSING      0x04
#define MAP_IM_CHAT_STATE_GONE                  0x05

/**
 * MAPv1.3 Message Extended Data Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
#define MAP_MSG_EXT_NUM_FB_LIKES                0x00
#define MAP_MSG_EXT_NUM_TWITTER_FOLLOWERS       0x01
#define MAP_MSG_EXT_NUM_TWITTER_RETWEETES       0x02
#define MAP_MSG_EXT_NUM_GOOGLE_PLUS             0x03


/* Application Parameters Header Value Range Definitions */
/* ----------------------------------------------------- */
/* MaxListCount - 2 Byte value : 0x0000 to 0xFFFF */
/* StartOffset - 2 Byte value : 0x0000 to 0xFFFF */
/* FilterMessageType - 1 Byte Bit-Mask : When SET, type is filtered out */
#define MAP_FILTER_MSGTYPE_SMS_GSM               0x01
#define MAP_FILTER_MSGTYPE_SMS_CDMA              0x02
#define MAP_FILTER_MSGTYPE_EMAIL                 0x04
#define MAP_FILTER_MSGTYPE_MMS                   0x08
#define MAP_FILTER_MSGTYPE_IM                    0x10

/* FilterPeriodBegin - Variable Length String */
/* FilterPeriodEnd - Variable Length String */
/* FilterReadStatus - 1 Byte Bit-Mask */
#define MAP_FILTER_READSTATUS_NO_FILTER          0x00
#define MAP_FILTER_READSTATUS_UNREAD_ONLY        0x01
#define MAP_FILTER_READSTATUS_READ_ONLY          0x02

/* FilterRecipient - Variable Length UTF-8 Text */
/* FilterOriginator - Variable Length UTF-8 Text */
/* FilterPriority - 1 Byte Bit-Mask */
#define MAP_FILTER_PRIORITY_NO_FILTER            0x00
#define MAP_FILTER_PRIORITY_HIGHPRIO_ONLY        0x01
#define MAP_FILTER_PRIORITY_NONHIGHPRIO_ONLY     0x02

/* Attachment - 1 Byte Bit-Mask */
#define MAP_ATTACHMENT_OFF                       0x00
#define MAP_ATTACHMENT_ON                        0x01

/* Transparent - 1 Byte Bit-Mask */
#define MAP_TRANSPARENT_OFF                      0x00
#define MAP_TRANSPARENT_ON                       0x01

/* Retry - 1 Byte Bit-Mask */
#define MAP_RETRY_OFF                            0x00
#define MAP_RETRY_ON                             0x01

/* NewMessage - 1 Byte Bit-Mask */
#define MAP_NEWMESSAGE_OFF                       0x00
#define MAP_NEWMESSAGE_ON                        0x01

/* NotificationStatus - 1 Byte Bit-Mask */
#define MAP_NOTIFICATIONSTATUS_OFF               0x00
#define MAP_NOTIFICATIONSTATUS_ON                0x01

/* MASInstanceID - 1 Byte Value : 0 to 255 */
/* Parameter Mask - 4 Byte Bit-Mask */
#define MAP_PARAM_MASK_SUBJECT                   0x00000001
#define MAP_PARAM_MASK_DATETIME                  0x00000002
#define MAP_PARAM_MASK_SENDER_NAME               0x00000004
#define MAP_PARAM_MASK_SENDER_ADDRESSING         0x00000008
#define MAP_PARAM_MASK_RECIPIENT_NAME            0x00000010
#define MAP_PARAM_MASK_RECIPIENT_ADDRESSING      0x00000020
#define MAP_PARAM_MASK_TYPE                      0x00000040
#define MAP_PARAM_MASK_SIZE                      0x00000080
#define MAP_PARAM_MASK_RECEPTION_STATUS          0x00000100
#define MAP_PARAM_MASK_TEXT                      0x00000200
#define MAP_PARAM_MASK_ATTACHMENT_SIZE           0x00000400
#define MAP_PARAM_MASK_PRIORITY                  0x00000800
#define MAP_PARAM_MASK_READ                      0x00001000
#define MAP_PARAM_MASK_SENT                      0x00002000
#define MAP_PARAM_MASK_PROTECTED                 0x00004000
#define MAP_PARAM_MASK_REPLYTO_ADDRESSING        0x00008000

/* FolderListingSize - 2 Byte value : 0x0000 to 0xFFFF */
/* MessagesListingSize - 2 Byte value : 0x0000 to 0xFFFF */
/* SubjectLength - 1 Byte Value : 0 to 255 */

/* Charset - 1 Byte Bit-Mask */
#define MAP_CHARSET_NATIVE                       0x00
#define MAP_CHARSET_UTF8                         0x01

/* FractionRequest - 1 Byte Bit-Mask */
#define MAP_FRACTIONREQUEST_FIRST                0x00
#define MAP_FRACTIONREQUEST_NEXT                 0x01

/* FractionDeliver - 1 Byte Bit-Mask */
#define MAP_FRACTIONDELIVER_MORE                 0x00
#define MAP_FRACTIONDELIVER_LAST                 0x01

/* StatusIndicator - 1 Byte Bit-Mask */
#define MAP_STATUSIND_READ_STATUS                0x00
#define MAP_STATUSIND_DELETED_STATUS             0x01

/* StatusValue - 1 Byte Bit-Mask */
#define MAP_STATUSVALUE_NO                       0x00
#define MAP_STATUSVALUE_YES                      0x01

/**
 * MAP NotificationFilterMask - 4Byte Bit-Mask
 */
#define MAP_NTF_FLTR_MASK_NEW_MSG                0x00000001 /* NewMessage */
#define MAP_NTF_FLTR_MASK_MSG_DELETED            0x00000002 /* MessageDeleted */
#define MAP_NTF_FLTR_MASK_MSG_SHIFT              0x00000004 /* MessageShift */
#define MAP_NTF_FLTR_MASK_SENDING_SUCCESS        0x00000008 /* SendingSuccess */
#define MAP_NTF_FLTR_MASK_SENDING_FAILURE        0x00000010 /* SendingFailure */
#define MAP_NTF_FLTR_MASK_DELIVERY_SUCCESS       0x00000020 /* DeliverySuccess */
#define MAP_NTF_FLTR_MASK_DELIVERY_FAILURE       0x00000040 /* DeliveryFailure */
#define MAP_NTF_FLTR_MASK_MEMORY_FULL            0x00000080 /* MemoryFull */
#define MAP_NTF_FLTR_MASK_MEMORY_AVAILABLE       0x00000100 /* MemoryAvailable */
#define MAP_NTF_FLTR_MASK_READ_STS_CHNGD         0x00000200 /* ReadStatusChanged */
#define MAP_NTF_FLTR_MASK_CONV_CHNGD             0x00000400 /* ConversationChanged */
#define MAP_NTF_FLTR_MASK_PTPNT_PRNCE_CHNGD      0x00000800 /* ParticipantPresenceChanged */
#define MAP_NTF_FLTR_MASK_PTPNT_CHT_ST_CHNGD     0x00001000 /* ParticipantChatStateChanged */
#define MAP_NTF_FLTR_MASK_MSG_EXTD_DATA_CHNGD    0x00002000 /* MessageExtendedDataChanged */
#define MAP_NTF_FLTR_MASK_MSG_REMOVED            0x00004000 /* MessageRemoved */

/**
 * MAP ConvParameterMask -4byte bit-mask
 */
#define MAP_CONV_PARAM_MASK_CONV_NAME            0x00000001 /* Conversation Name */
#define MAP_CONV_PARAM_MASK_CONV_L_ACTVTY        0x00000002 /* Conversation last_activity */
#define MAP_CONV_PARAM_MASK_CONV_RD_STS          0x00000004 /* Conversation read_status */
#define MAP_CONV_PARAM_MASK_CONV_VER_CNTR        0x00000008 /* Conversation version_counter */
#define MAP_CONV_PARAM_MASK_CONV_SUMMARY         0x00000010 /* Conversation summary */
#define MAP_CONV_PARAM_MASK_PARTICIPANTS         0x00000020 /* Participants */
#define MAP_CONV_PARAM_MASK_PTPNT_UCI            0x00000040 /* Participant UCI */
#define MAP_CONV_PARAM_MASK_PTPNT_DPLY_NAME      0x00000080 /* Participant display_name */
#define MAP_CONV_PARAM_MASK_PTPNT_CHT_STATE      0x00000100 /* Participant chat_state */
#define MAP_CONV_PARAM_MASK_PTPNT_L_ACTVTY       0x00000200 /* Participant last_activity */
#define MAP_CONV_PARAM_MASK_PTPNT_X_BT_UID       0x00000400 /* Participant x_bt_uid */
#define MAP_CONV_PARAM_MASK_PTPNT_NAME           0x00000800 /* Participant name */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_ALTY     0x00001000 /* Participant presence_availability */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_TEXT     0x00002000 /* Participant presence_text */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_PRTY     0x00004000 /* Participant priority */

/**
 * MAP SupportedFeatures -4byte bit-mask
 */
#define MAP_SUPP_FTRS_MASK_NTY_REG               0x00000001 /* Notification Registration Feature */
#define MAP_SUPP_FTRS_MASK_NTY                   0x00000002 /* Notification Feature */
#define MAP_SUPP_FTRS_MASK_BROWSING              0x00000004 /* Browsing Feature */
#define MAP_SUPP_FTRS_MASK_UPLOADING             0x00000008 /* Uploading Feature */
#define MAP_SUPP_FTRS_MASK_DELETE                0x00000010 /* Delete Feature */
#define MAP_SUPP_FTRS_MASK_INST_INFO             0x00000020 /* Instance Information Feature */
#define MAP_SUPP_FTRS_MASK_EXTED_EVNT_RPT_11     0x00000040 /* Extended Event Report 1.1 */
#define MAP_SUPP_FTRS_MASK_EVNT_RPT_12           0x00000080 /* Event Report Version 1.2 */
#define MAP_SUPP_FTRS_MASK_MSG_FRMT_11           0x00000100 /* Message Format Version 1.1 */
#define MAP_SUPP_FTRS_MASK_MSG_LIST_FRMT_11      0x00000200 /* Messages-Listing Format Version 1.1 */
#define MAP_SUPP_FTRS_MASK_PRST_MSG_HNDLS        0x00000400 /* Persistent Message Handles */
#define MAP_SUPP_FTRS_MASK_DATABASE_ID           0x00000800 /* Database Identifier */
#define MAP_SUPP_FTRS_MASK_FLDR_VER_CNTR         0x00001000 /* Folder Version Counter */
#define MAP_SUPP_FTRS_MASK_CONV_VER_CNTR         0x00002000 /* Conversation Version Counters */
#define MAP_SUPP_FTRS_MASK_PTPNT_PRNCE_CHNG_NTY  0x00004000 /* Participant Presence Change Notification */
#define MAP_SUPP_FTRS_MASK_PTPNT_CHT_ST_CHNG_NTY 0x00008000 /* Participant Chat State Change Notification */
#define MAP_SUPP_FTRS_MASK_PBAP_CNTCT_CRS_REF    0x00010000 /* PBAP Contact Cross Reference */
#define MAP_SUPP_FTRS_MASK_NTFY_FLTR             0x00020000 /* Notification Filtering */
#define MAP_SUPP_FTRS_MASK_UTC_TIMESTMP_FRMT     0x00040000 /* UTC offset timestamp format */
#define MAP_SUPP_FTRS_MASK_CONV_LISTING          0x00100000 /* Conversation Listing */
#define MAP_SUPP_FTRS_MASK_OWNER_STATUS          0x00200000 /* Owner Status */

/**
 * MAPv1.4 Message Forwand Constants
 */
#define MAP_FRWD_MODIFY_TEXT_REPLACE             0x00
#define MAP_FRWD_MODIFY_TEXT_PREPEND             0x01

/**
 * Max. Conversation id size
 */
#define MAX_CONVERSATION_ID_SIZE                (32 + 1) /* NULL terminator included */

/**
 * Max. Presence Text
 */
#define MAX_PRESENCE_TEXT_SIZE                  (255 + 1) /* NULL terminator included */

/**
 * Max Timestamp size
 */
#define MAX_MAP_TIMESTAMP_SIZE                  (20 + 1) /* NULL terminator inlcude */

/* ----------------------------------------------------- */

/* --------------------------------------------- Structures/Data Types */
/* MAP Handle type */
typedef UINT8 MAP_HANDLE;

/* MAP Service Types */
typedef enum
{
    MAP_ACCESS_SERVICE = 0x01,
    MAP_NTF_SERVICE = 0x02

} MAP_SERVICE_TYPE;

/* MAP Entity Handle */
typedef struct _MAP_HANDLE_INFO
{
    /* Service Type */
    MAP_SERVICE_TYPE    service;

    /* Entity MAP Handle ID */
    MAP_HANDLE    entity_id;

} MAP_HANDLE_INFO;

/* MAP Header structure, to exchage data with application */
typedef struct _MAP_HEADER_STRUCT
{
    UCHAR   * value;
    UINT16    length;

} MAP_HEADER_STRUCT;

/* MAP Application Parameter Structure */
typedef struct _MAP_APPL_PARAMS
{
    UINT32                 parameter_mask;

    UINT16                 max_list_count;
    UINT16                 start_offset;
    UINT16                 folder_listing_size;
    UINT16                 messages_listing_size;

    /* TODO: remove hardcode value  2 */
    UINT32                 appl_param_flag[2];
    /**
     * 32 Bit Flag if MAP version < 1.3
     * 64 Bit Flag if MAP version >= 1.3
     *
     * Usage of appl_param_flag[0]:
     *
     * BIT 0 - Max List Count
     *     1 - Start Offset
     *     2 - Filter Message Type
     *     3 - Filter Period Begin
     *     4 - Filter Period End
     *     5 - Filter Read Status
     *     6 - Filter Recipient
     *     7 - Filter Originator
     *     8 - Filter Priority
     *     9 - Attachment
     *    10 - Transparent
     *    11 - Retry
     *    12 - New Message
     *    13 - Notification Status
     *    14 - MAS Instance ID
     *    15 - Parameter Mask
     *    16 - Folder Listing Size
     *    17 - Messages Listing Size
     *    18 - Subject Length
     *    19 - Charset
     *    20 - Fraction Request
     *    21 - Fraction Deliver
     *    22 - Status Indicator
     *    23 - Status Value
     *    24 - MSE Time
     *    25 - Database Identifier
     *    26 - Conversation Listing Version Counter
     *    27 - Presence Availability
     *    28 - Presence Text
     *    29 - Last Activity
     *    30 - FilterLastActivityBegin
     *    31 - FilterLastActivityEnd
     *
     * Usage of appl_param_flag[1]:
     *
     * BIT 0 - Chat State
     *     1 - Coverstaion ID
     *     2 - Folder Version Counter
     *     3 - Filter Message Handle
     *     4 - Notification Filter Mask
     *     5 - ConvParameterMask
     *     6 - Owner UCI
     *     7 - Extended Data
     *     8 - MapSupportedFeatures
     */

    MAP_HEADER_STRUCT     filter_period_begin;
    MAP_HEADER_STRUCT     filter_period_end;
    MAP_HEADER_STRUCT     filter_recipient;
    MAP_HEADER_STRUCT     filter_originator;
    MAP_HEADER_STRUCT     mse_time;

    UINT8                 filter_message_type;
    UINT8                 filter_read_status;
    UINT8                 filter_priority;
    UINT8                 attachment;
    UINT8                 transparent;
    UINT8                 retry;
    UINT8                 new_message;
    UINT8                 notification_status;
    UINT8                 mas_instance_id;
    UINT8                 subject_length;
    UINT8                 charset;
    UINT8                 fraction_request;
    UINT8                 fraction_deliver;
    UINT8                 status_indicator;
    UINT8                 status_value;

#ifdef MAP_1_3
    MAP_HEADER_STRUCT     database_identifier;
    MAP_HEADER_STRUCT     conv_listing_ver_cntr;
    MAP_HEADER_STRUCT     presence_text;
    MAP_HEADER_STRUCT     last_activity;
    MAP_HEADER_STRUCT     filter_last_activity_begin;
    MAP_HEADER_STRUCT     filter_last_activity_end;
    MAP_HEADER_STRUCT     conversation_id;
    MAP_HEADER_STRUCT     folder_ver_cntr;
    MAP_HEADER_STRUCT     filter_msg_handle;
    MAP_HEADER_STRUCT     owner_uci;
    MAP_HEADER_STRUCT     extended_data;

    UINT8                 presence_availability;
    UINT8                 chat_state;
    UINT32                ntf_filter_mask;
    UINT32                conv_parameter_mask;
    UINT32                supported_features;
#endif /* MAP_1_3 */

#ifdef MAP_1_4
    UCHAR                 modify_text;
    MAP_HEADER_STRUCT     msg_handle;
#endif /* MAP_1_4 */

} MAP_APPL_PARAMS;


/* MAP Connection Structure */
typedef struct _MAP_CONNECT_STRUCT
{
    /* Peer bluetooth device address */
    UCHAR               * bd_addr;

    /* Maximum Receive Size */
    UINT16                max_recv_size;

    /* Peer MAP RFCOMM channel to connect */
    UINT8                 server_channel;

#ifdef MAP_1_2
    /* Remote MAP entitys OBEX L2CAP PSM */
    UINT16                psm;
#endif /* MAP_1_2 */

#ifdef MAP_1_3
    /**
     * Should be included in connect request if peer
     * advertises its support in the SDP rsp.
     */
    UINT32                map_supported_features;
#endif /* MAP_1_3 */

} MAP_CONNECT_STRUCT;


/* MAP Request Structure */
typedef struct _MAP_REQUEST_STRUCT
{
    /**
     * Name/Handle of Message
     */
    MAP_HEADER_STRUCT   * name;

    /**
     * Message contents
     */
    MAP_HEADER_STRUCT   * body;

    /**
     * Application Parameters
     */
    MAP_APPL_PARAMS     * appl_params;

    /**
     * Set path flag
     */
    UCHAR                 setpath_flag;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                 wait;
} MAP_REQUEST_STRUCT;


/* MAP Response Structure */
typedef struct _MAP_RESPONSE_STRUCT
{
    /**
     * Name/Handle of Message
     */
    MAP_HEADER_STRUCT  * name;

    /**
     * Message contents
     */
    MAP_HEADER_STRUCT  * body;

    /**
     * Application Parameters
     */
    MAP_APPL_PARAMS    * appl_params;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                wait;
} MAP_RESPONSE_STRUCT;


/* MAP common headers */
typedef union _MAP_HEADERS
{
    /**
     * Used to send the request
     */
    MAP_REQUEST_STRUCT   * map_req_info;

    /**
     * Used to send the response
     */
    MAP_RESPONSE_STRUCT  * map_resp_info;

    /**
     * Used for Connect request
     */
    MAP_CONNECT_STRUCT   * map_connect_info;
} MAP_HEADERS;

/* MAP Event Notification Callback type */
typedef API_RESULT (* MAP_EVENT_NTF_CB)
                   (
                        MAP_HANDLE_INFO * map_handle,
                        UINT8             event_type,
                        UINT16            event_result,
                        MAP_HEADERS     * map_headers,
                        UINT16            num_headers
                   );

/* --------------------------------------------- Macros */
/* To initialize a Header Structure */
#define MAP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0;

#ifndef MAP_1_3
/* To set application parameter flag */
#define MAP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        *(flag) = *(flag) | (set_field)

/* To retrieve the field value (Set/Reset) */
#define MAP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        (*(flag) & (get_field))

/* Resets all the fields of the application parameter flag */
#define MAP_RESET_APPL_PARAM_FLAG(flag)            \
        *(flag) = 0x00000000

#else /* MAP_1_3 */
/* To set application parameter flag */
#define MAP_SET_APPL_PARAM_FLAG_EXT(flag, set_field, index)   \
        *((UINT32 *)((flag) + (index))) = *((UINT32 *)((flag) + (index))) | (set_field)

/* To retrieve the field value (Set/Reset) */
#define MAP_GET_APPL_PARAM_FLAG_EXT(flag, get_field, index)   \
        (*((UINT32 *)((flag) + (index))) & (get_field))

/* Resets all the fields of the application parameter flag */
#define MAP_RESET_APPL_PARAM_FLAG_EXT(flag_ptr, index)        \
        (flag_ptr)[(index)] = 0x00000000

/* Mapping for default macros */
#define MAP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        MAP_SET_APPL_PARAM_FLAG_EXT((flag), (set_field), 0)

#define MAP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        MAP_GET_APPL_PARAM_FLAG_EXT((flag), (get_field), 0)

#define MAP_RESET_APPL_PARAM_FLAG(flag)    \
        MAP_RESET_APPL_PARAM_FLAG_EXT((flag), 0); \
        MAP_RESET_APPL_PARAM_FLAG_EXT((flag), 1)
#endif /* MAP_1_3 */

/* Extract header structure value */
#define MAP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/* Extract application parameters */
#define MAP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(MAP_APPL_PARAMS));   \
        if((from_params)->search_value.length != 0 &&                       \
           (from_params)->search_value.value != NULL)                       \
        {                                                                   \
            BT_mem_copy((search_value_out),                                 \
                   (from_params)->search_value.value,                       \
                   (from_params)->search_value.length);                     \
            (to_params)->search_value.value = (search_value_out);           \
            (to_params)->search_value.length =                              \
                                (from_params)->search_value.length;         \
        }


/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/* MAP Message Client APIs */
#ifdef MAP_MCE
/* MAP Client Initialization */
API_RESULT BT_map_mce_init (void);

/* MAP Client Shutdown */
API_RESULT BT_map_mce_shutdown (void);

/* MAP Client Start Instance */
API_RESULT BT_map_mce_start
           (
               /* IN  */ MAP_SERVICE_TYPE    map_service,
               /* IN  */ MAP_EVENT_NTF_CB    map_mce_cb,
               /* OUT */ MAP_HANDLE        * map_handle
           );

/* MAP Client Stop Instance */
API_RESULT BT_map_mce_stop
           (
               /* IN */ MAP_SERVICE_TYPE    map_service,
               /* IN */ MAP_HANDLE        * map_handle
           );

/* MAP Client Connect on Instance */
API_RESULT BT_map_mce_connect
           (
               /* IN */ MAP_HANDLE         * map_handle,
               /* IN */ MAP_CONNECT_STRUCT * map_connect_info
           );

/* MAP Client Disconnect Instance */
API_RESULT BT_map_mce_disconnect
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/* MAP Client Disconnect Instance Transport */
API_RESULT BT_map_mce_transport_close
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/* MAP Client update Server inbox */
API_RESULT BT_map_mce_update_inbox
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/* MAP Client set server folder */
API_RESULT BT_map_mce_set_folder
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

/* MAP Client get server folder listing */
API_RESULT BT_map_mce_get_folder_listing
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/* MAP Client get server messages listing */
API_RESULT BT_map_mce_get_messages_listing
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/* MAP Client get message from server */
API_RESULT BT_map_mce_get_message
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/* MAP Client set message status on server */
API_RESULT BT_map_mce_set_message_status
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

/* MAP Client push message to server */
API_RESULT BT_map_mce_push_message
           (
               /* IN  */ MAP_HANDLE           * map_handle,
               /* IN  */ MAP_REQUEST_STRUCT   * map_set_info,
               /* IN  */ UCHAR                  more,
               /* OUT */ UINT16               * actual
           );

/* MAP Client set notification registration on server */
API_RESULT BT_map_mce_set_ntf_registration
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

#ifdef MAP_1_2
API_RESULT BT_map_mce_get_mas_instance_info
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );
#endif /* MAP_1_2 */

#ifdef MAP_1_3
API_RESULT BT_map_mce_get_conversation_listing
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );

API_RESULT BT_map_mce_set_owner_status
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_set_info
           );

API_RESULT BT_map_mce_get_owner_status
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );

API_RESULT BT_map_mce_set_notification_filter
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_set_info
           );
#endif /* MAP_1_3 */

/* MAP Client Send continuing request if required for ongoing operation */
API_RESULT BT_map_mce_send_request
           (
               /* IN  */ MAP_HANDLE   * map_handle,
               /* IN  */ UINT8          event_type,
               /* IN  */ UINT16         event_result,
               /* IN  */ MAP_HEADERS  * map_headers,
               /* IN  */ UCHAR          more,
               /* OUT */ UINT16       * actual
           );

/* MAP Client abort ongoing transaction */
API_RESULT BT_map_mce_abort
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

#ifdef MAP_SUPPORT_NOTIFICATION
/* MAP Client Disconnect Notification Service Instance Transport */
API_RESULT BT_map_mce_ns_transport_close
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/* MAP Client Send Response for last received request on Notification Channel */
API_RESULT BT_map_mce_ns_send_response
           (
                /* IN */ MAP_HANDLE    * map_handle,
                /* IN */ UINT8           event_type,
                /* IN */ UCHAR           response,
                /* IN */ MAP_HEADERS   * map_headers
           );
#endif /* MAP_SUPPORT_NOTIFICATION */

#endif /* MAP_MCE */

/* MAP Message Server APIs */
#ifdef MAP_MSE
/* MAP Server Initialization */
API_RESULT BT_map_mse_init (void);

/* MAP Server Shutdown */
API_RESULT BT_map_mse_shutdown (void);

/* MAP Server Start Instance */
API_RESULT BT_map_mse_start
           (
               /* IN  */ MAP_SERVICE_TYPE    map_service,
               /* IN  */ MAP_EVENT_NTF_CB    map_mse_cb,
               /* OUT */ MAP_HANDLE        * map_handle
           );

/* MAP Server Stop Instance */
API_RESULT BT_map_mse_stop
           (
               /* IN */ MAP_SERVICE_TYPE    map_service,
               /* IN */ MAP_HANDLE        * map_handle
           );

/* MAP Server Disconnect Instance Transport */
API_RESULT BT_map_mse_transport_close
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/* MAP Server Send Response for last received MAS request */
API_RESULT BT_map_mse_send_response
           (
                /* IN */ MAP_HANDLE           * map_handle,
                /* IN */ UINT8                  event_type,
                /* IN */ UCHAR                  response,
                /* IN */ MAP_HEADERS          * map_headers
           );

#ifdef MAP_SUPPORT_NOTIFICATION
/* MAP Server Connect for Notification Service */
API_RESULT BT_map_mse_ns_connect
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_CONNECT_STRUCT  * map_connect_info
           );

/* MAP MNS Disconnect Instance */
API_RESULT BT_map_mse_ns_disconnect
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/* MAP MNS Disconnect Instance Transport */
API_RESULT BT_map_mse_ns_transport_close
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/* MAP Server message notification event */
API_RESULT BT_map_mse_ns_send_event
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info,
               /* IN */ UCHAR                  more
           );

API_RESULT BT_map_mse_ns_send_request
           (
               /* IN  */ MAP_HANDLE   * map_handle,
               /* IN  */ UINT8          event_type,
               /* IN  */ UINT16         event_result,
               /* IN  */ MAP_HEADERS  * MAP_headers,
               /* IN  */ UCHAR          more,
               /* OUT */ UINT16       * actual
           );

#endif /* MAP_SUPPORT_NOTIFICATION */

#endif /* MAP_MSE */

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_MAP_API_ */

