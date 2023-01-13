
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
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup map_module MAP (Message Access Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Message Access Profile(MAP) module to the Application and other upper layers
 *  of the stack.
 */
/**
 * \defgroup map_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup map_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
#ifdef MAP_1_2
/* OBEX L2CAP PSM for local client entities */
#define MAP_OBEX_L2CAP_PSM                       0x6007U
#endif /* MAP_1_2 */

/* MAP Notification Callback Event types */
/**
 * @name MAP Notification Callback Event types
 *
 * Constant Definitions for MAP Notification Callback Event types
 */
/*@{*/
#ifdef MAP_MCE
#define MAP_MCE_CONNECT_CNF                      0x01U
#define MAP_MCE_DISCONNECT_CNF                   0x02U
#define MAP_MCE_TRANSPORT_CLOSE_CNF              0x03U
#define MAP_MCE_TRANSPORT_CLOSE_IND              0x04U

#define MAP_MCE_UPDATE_INBOX_CNF                 0x05U
#define MAP_MCE_SET_FOLDER_CNF                   0x06U
#define MAP_MCE_GET_FOLDER_LISTING_CNF           0x07U
#define MAP_MCE_GET_MESSAGES_LISTING_CNF         0x08U
#define MAP_MCE_GET_MESSAGE_CNF                  0x09U
#define MAP_MCE_SET_MESSAGE_STATUS_CNF           0x0AU
#define MAP_MCE_PUSH_MESSAGE_CNF                 0x0BU
#define MAP_MCE_SET_NTF_REGISTRATION_CNF         0x0CU

#ifdef MAP_1_2
#define MAP_MCE_GET_MASINSTINFO_CNF              0x0EU
#endif /* MAP_1_2 */

#ifdef MAP_1_3
#define MAP_MCE_GET_CONVERSATION_LIST_CNF        0x61U
#define MAP_MCE_SET_OWNER_STATUS_CNF             0x62U
#define MAP_MCE_GET_OWNER_STATUS_CNF             0x63U
#define MAP_MCE_SET_NOTIFICATION_FILTER_CNF      0x64U
#endif /* MAP_1_3 */

#define MAP_MCE_NS_CONNECT_IND                   0x10U
#define MAP_MCE_NS_DISCONNECT_IND                0x11U
#define MAP_MCE_NS_TRANSPORT_CLOSE_IND           0x12U
#define MAP_MCE_NS_TRANSPORT_CLOSE_CNF           0x13U
#define MAP_MCE_NS_EVENT_REPORT_IND              0x14U

/* To be deprecated */
#define MAP_MCE_ABORT_CNF                        MAP_ABORT_CNF
#define MAP_MCE_ABORT_IND                        MAP_ABORT_IND

#endif /* MAP_MCE */

#ifdef MAP_MSE
#define MAP_MSE_CONNECT_IND                      0x21U
#define MAP_MSE_DISCONNECT_IND                   0x22U
#define MAP_MSE_TRANSPORT_CLOSE_IND              0x23U
#define MAP_MSE_TRANSPORT_CLOSE_CNF              0x24U

#define MAP_MSE_UPDATE_INBOX_IND                 0x25U
#define MAP_MSE_SET_FOLDER_IND                   0x26U
#define MAP_MSE_GET_FOLDER_LISTING_IND           0x27U
#define MAP_MSE_GET_MESSAGES_LISTING_IND         0x28U
#define MAP_MSE_GET_MESSAGE_IND                  0x29U
#define MAP_MSE_SET_MESSAGE_STATUS_IND           0x2AU
#define MAP_MSE_PUSH_MESSAGE_IND                 0x2BU
#define MAP_MSE_SET_NTF_REGISTRATION_IND         0x2CU

#ifdef MAP_1_2
#define MAP_MSE_GET_MASINSTINFO_IND              0x2EU
#endif /* MAP_1_2 */

#ifdef MAP_1_3
#define MAP_MSE_GET_CONVERSATION_LIST_IND        0x81U
#define MAP_MSE_SET_OWNER_STATUS_IND             0x82U
#define MAP_MSE_GET_OWNER_STATUS_IND             0x83U
#define MAP_MSE_SET_NOTIFICATION_FILTER_IND      0x84U
#endif /* MAP_1_3 */

#define MAP_MSE_NS_CONNECT_CNF                   0x30U
#define MAP_MSE_NS_DISCONNECT_CNF                0x31U
#define MAP_MSE_NS_TRANSPORT_CLOSE_CNF           0x32U
#define MAP_MSE_NS_TRANSPORT_CLOSE_IND           0x33U
#define MAP_MSE_NS_SEND_EVENT_CNF                0x34U

/* To be deprecated */
#define MAP_MSE_ABORT_CNF                        MAP_ABORT_CNF
#define MAP_MSE_ABORT_IND                        MAP_ABORT_IND

#endif /* MAP_MSE */

/* Common Event codes */
#define MAP_ABORT_CNF                            0x40U
#define MAP_ABORT_IND                            0x50U
#define MAP_EVENT_INVALID                        0xFFU

/* Invalid entity indentifiers */
#define MAP_ENTITY_INVALID                       0xFFU
#define MAP_INVALID_OBEX_CONNECTION_ID           0xFFFFFFFFU
/*@}*/
/* MAP OBEX Response Codes */
/**
 * @name MAP OBEX Response Codes
 *
 * Constant Definitions for MAP OBEX Response Codes
 */
/*@{*/
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
/*@}*/
/* MAP Services OBEX Target header UUID values */
#define MAP_MAS_OBEX_TARGET_HDR_UUID \
        0xBB582B40420C11DBB0DE0800200C9A66U
#define MAP_MNS_OBEX_TARGET_HDR_UUID \
        0xBB582B41420C11DBB0DE0800200C9A66U

/* MAP Target Header Size */
#define MAP_TARGET_HDR_SIZE                      0x10U /* 16 */

/* MAP Application Parameters header Tag IDs */
/**
 * @name MAP Application Parameters header Tag IDs
 *
 * Constant Definitions for MAP Application Parameters header Tag IDs
 */
/*@{*/
#define MAP_TAG_ID_MAXLISTCOUNT                  0x01U  /* Length : 2 bytes */
#define MAP_TAG_ID_STARTOFFSET                   0x02U  /* Length : 2 bytes */
#define MAP_TAG_ID_FILTERMESSAGETYPE             0x03U  /* Length : 1 byte */
#define MAP_TAG_ID_FILTERPERIODBEGIN             0x04U  /* Length : variable */
#define MAP_TAG_ID_FILTERPERIODEND               0x05U  /* Length : variable */
#define MAP_TAG_ID_FILTERREADSTATUS              0x06U  /* Length : 1 byte */
#define MAP_TAG_ID_FILTERRECIPIENT               0x07U  /* Length : variable */
#define MAP_TAG_ID_FILTERORIGINATOR              0x08U  /* Length : variable */
#define MAP_TAG_ID_FILTERPRIORITY                0x09U  /* Length : 1 byte */
#define MAP_TAG_ID_ATTACHMENT                    0x0AU  /* Length : 1 byte */
#define MAP_TAG_ID_TRANSPARENT                   0x0BU  /* Length : 1 byte */
#define MAP_TAG_ID_RETRY                         0x0CU  /* Length : 1 byte */
#define MAP_TAG_ID_NEWMESSAGE                    0x0DU  /* Length : 1 byte */
#define MAP_TAG_ID_NOTIFICATIONSTATUS            0x0EU  /* Length : 1 byte */
#define MAP_TAG_ID_MASINSTANCEID                 0x0FU  /* Length : 1 byte */
#define MAP_TAG_ID_PARAMETERMASK                 0x10U  /* Length : 4 bytes */
#define MAP_TAG_ID_FOLDERLISTINGSIZE             0x11U  /* Length : 2 bytes */
#define MAP_TAG_ID_MESSAGESLISTINGSIZE           0x12U  /* Length : 2 bytes */
#define MAP_TAG_ID_SUBJECTLENGTH                 0x13U  /* Length : variable (max 255 bytes) */
#define MAP_TAG_ID_CHARSET                       0x14U  /* Length : 1 bytes */
#define MAP_TAG_ID_FRACTIONREQUEST               0x15U  /* Length : 1 bytes */
#define MAP_TAG_ID_FRACTIONDELIVER               0x16U  /* Length : 1 bytes */
#define MAP_TAG_ID_STATUSINDICATOR               0x17U  /* Length : 1 bytes */
#define MAP_TAG_ID_STATUSVALUE                   0x18U  /* Length : 1 bytes */
#define MAP_TAG_ID_MSETIME                       0x19U  /* Length : variable */

#ifdef MAP_1_3
#define MAP_TAG_ID_DATABASE_IDENTIFIER           0x1AU  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_CONV_LIST_VER_CNTR            0x1BU  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_PRESENCE_AVAILABILITY         0x1CU  /* Length : 1 bytes */
#define MAP_TAG_ID_PRESENCE_TEXT                 0x1DU  /* Length : variable */
#define MAP_TAG_ID_LAST_ACTIVITY                 0x1EU  /* Length : variable */
#define MAP_TAG_ID_FILTERLASTACTIVITYBEGIN       0x1FU  /* Length : variable */
#define MAP_TAG_ID_FILTERLASTACTIVITYEND         0x20U  /* Length : variable */

#define MAP_TAG_ID_CHAT_STATE                    0x21U  /* Length : 1 bytes */
#define MAP_TAG_ID_CONVERSATION_ID               0x22U  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_FOLDER_VER_CNTR               0x23U  /* Length : variable (max 32 bytes) */
#define MAP_TAG_ID_FILTER_MSG_HANDLE             0x24U  /* Length : variable (max 16 bytes) */
#define MAP_TAG_ID_NOTIFICATION_FILTER_MASK      0x25U  /* Length : 4 bytes */
#define MAP_TAG_ID_CONV_PARAMETER_MASK           0x26U  /* Length : 4 bytes */
#define MAP_TAG_ID_OWNER_UCI                     0x27U  /* Length : variable */
#define MAP_TAG_ID_EXTENTED_DATA                 0x28U  /* Length : variable */
#define MAP_TAG_ID_MAP_SUPPORTED_FEATURES        0x29U  /* Length : 4 bytes */
#endif /* MAP_1_3 */

#ifdef MAP_1_4
#define MAP_TAG_ID_MESSAGE_HANDLE                0x2AU  /* Length : 8 bytes */
#define MAP_TAG_ID_MODIFY_TEXT                   0x2BU  /* Length : 1 byte */
#endif /* MAP_1_4 */
/*@}*/
/* Maximum number of Application Header Tag IDs */
#if (defined MAP_1_4)
#define MAP_NUM_APPL_HEADERS                     0x2BU
#elif (defined MAP_1_3)
#define MAP_NUM_APPL_HEADERS                     0x29U
#else
#define MAP_NUM_APPL_HEADERS                     0x19U
#endif /* MAP_1_4 */

/* MAP Application parameters flag corresponding to each Tag */
/**
 * @name MAP Application Parameters header Tag IDs
 *
 * Constant Definitions for MAP Application Parameters header Tag IDs
 */
/*@{*/
#define MAP_FLAG_MAXLISTCOUNT                    0x00000001U
#define MAP_FLAG_STARTOFFSET                     0x00000002U
#define MAP_FLAG_FILTERMESSAGETYPE               0x00000004U
#define MAP_FLAG_FILTERPERIODBEGIN               0x00000008U
#define MAP_FLAG_FILTERPERIODEND                 0x00000010U
#define MAP_FLAG_FILTERREADSTATUS                0x00000020U
#define MAP_FLAG_FILTERRECIPIENT                 0x00000040U
#define MAP_FLAG_FILTERORIGINATOR                0x00000080U
#define MAP_FLAG_FILTERPRIORITY                  0x00000100U
#define MAP_FLAG_ATTACHMENT                      0x00000200U
#define MAP_FLAG_TRANSPARENT                     0x00000400U
#define MAP_FLAG_RETRY                           0x00000800U
#define MAP_FLAG_NEWMESSAGE                      0x00001000U
#define MAP_FLAG_NOTIFICATIONSTATUS              0x00002000U
#define MAP_FLAG_MASINSTANCEID                   0x00004000U
#define MAP_FLAG_PARAMETERMASK                   0x00008000U
#define MAP_FLAG_FOLDERLISTINGSIZE               0x00010000U
#define MAP_FLAG_MESSAGESLISTINGSIZE             0x00020000U
#define MAP_FLAG_SUBJECTLENGTH                   0x00040000U
#define MAP_FLAG_CHARSET                         0x00080000U
#define MAP_FLAG_FRACTIONREQUEST                 0x00100000U
#define MAP_FLAG_FRACTIONDELIVER                 0x00200000U
#define MAP_FLAG_STATUSINDICATOR                 0x00400000U
#define MAP_FLAG_STATUSVALUE                     0x00800000U
#define MAP_FLAG_MSETIME                         0x01000000U

#ifdef MAP_1_3
#define MAP_FLAG_DATABASE_IDENTIFIER             0x02000000U
#define MAP_FLAG_CONV_LIST_VER_CNTR              0x04000000U
#define MAP_FLAG_PRESENCE_AVAILABILITY           0x08000000U
#define MAP_FLAG_PRESENCE_TEXT                   0x10000000U
#define MAP_FLAG_LAST_ACTIVITY                   0x20000000U
#define MAP_FLAG_FILTERLASTACTIVITYBEGIN         0x40000000U
#define MAP_FLAG_FILTERLASTACTIVITYEND           0x80000000U

/* From 33rd application parameter */
#define MAP_FLAG_CHAT_STATE                      0x00000001U
#define MAP_FLAG_CONVERSATION_ID                 0x00000002U
#define MAP_FLAG_FOLDER_VER_CNTR                 0x00000004U
#define MAP_FLAG_FILTER_MSG_HANDLE               0x00000008U
#define MAP_FLAG_NOTIFICATION_FILTER_MASK        0x00000010U
#define MAP_FLAG_CONV_PARAMETER_MASK             0x00000020U
#define MAP_FLAG_OWNER_UCI                       0x00000040U
#define MAP_FLAG_EXTENDED_DATA                   0x00000080U
#define MAP_FLAG_MAP_SUPPORTED_FEATURES          0x00000100U
#endif /* MAP_1_3 */

#ifdef MAP_1_4
#define MAP_FLAG_MESSAGE_HANDLE                  0x00000200U
#define MAP_FLAG_MODIFY_TEXT                     0x00000400U
#endif /* MAP_1_4 */
/*@}*/
/* MAP SetPath parameter values */
/**
 * @name MAP SetPath parameter values
 *
 * Constant Definitions for MAP SetPath parameter values
 */
/*@{*/
#define MAP_SET_CHILD_FOLDER                     0x01U
#define MAP_SET_PARENT_FOLDER                    0x02U
#define MAP_SET_ROOT_FOLDER                      0x03U
/*@}*/
/**
 * MAPv1.3 Instant Messaging Availability Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
/**
 * @name MAPv1.3 Instant Messaging Availability Constants
 *
 * Constant Definitions for MAPv1.3 Instant Messaging Availability Constants
 */
/*@{*/
#define MAP_IM_PRESENCE_UNKNOWN                  0x00U
#define MAP_IM_PRESENCE_OFFLINE                  0x01U
#define MAP_IM_PRESENCE_ONLINE                   0x02U
#define MAP_IM_PRESENCE_AWAY                     0x03U
#define MAP_IM_PRESENCE_DO_NOT_DISTURB           0x04U
#define MAP_IM_PRESENCE_BUSY                     0x05U
#define MAP_IM_PRESENCE_IN_MEETING               0x06U
/*@}*/
/**
 * MAPv1.3 IM Chat State Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
/**
 * @name MAPv1.3 IM Chat State Constants
 *
 * Constant Definitions for MAPv1.3 IM Chat State Constants
 */
/*@{*/
#define MAP_IM_CHAT_STATE_UNKNOWN               0x00U
#define MAP_IM_CHAT_STATE_INACTIVE              0x01U
#define MAP_IM_CHAT_STATE_ACTIVE                0x02U
#define MAP_IM_CHAT_STATE_COMPOSING             0x03U
#define MAP_IM_CHAT_STATE_PAUSED_COMPOSING      0x04U
#define MAP_IM_CHAT_STATE_GONE                  0x05U
/*@}*/
/**
 * MAPv1.3 Message Extended Data Constants
 * Note: Refer the Assigned Numbers for MAP in SIG website.
 */
/**
 * @name MAPv1.3 Message Extended Data Constants
 *
 * Constant Definitions for MAPv1.3 Message Extended Data Constants
 */
/*@{*/
#define MAP_MSG_EXT_NUM_FB_LIKES                0x00U
#define MAP_MSG_EXT_NUM_TWITTER_FOLLOWERS       0x01U
#define MAP_MSG_EXT_NUM_TWITTER_RETWEETES       0x02U
#define MAP_MSG_EXT_NUM_GOOGLE_PLUS             0x03U
/*@}*/

/* Application Parameters Header Value Range Definitions */
/* ----------------------------------------------------- */
/* MaxListCount - 2 Byte value : 0x0000 to 0xFFFF */
/* StartOffset - 2 Byte value : 0x0000 to 0xFFFF */
/* FilterMessageType - 1 Byte Bit-Mask : When SET, type is filtered out */
#define MAP_FILTER_MSGTYPE_SMS_GSM               0x01U
#define MAP_FILTER_MSGTYPE_SMS_CDMA              0x02U
#define MAP_FILTER_MSGTYPE_EMAIL                 0x04U
#define MAP_FILTER_MSGTYPE_MMS                   0x08U
#define MAP_FILTER_MSGTYPE_IM                    0x10U

/* FilterPeriodBegin - Variable Length String */
/* FilterPeriodEnd - Variable Length String */
/* FilterReadStatus - 1 Byte Bit-Mask */
#define MAP_FILTER_READSTATUS_NO_FILTER          0x00U
#define MAP_FILTER_READSTATUS_UNREAD_ONLY        0x01U
#define MAP_FILTER_READSTATUS_READ_ONLY          0x02U

/* FilterRecipient - Variable Length UTF-8 Text */
/* FilterOriginator - Variable Length UTF-8 Text */
/* FilterPriority - 1 Byte Bit-Mask */
#define MAP_FILTER_PRIORITY_NO_FILTER            0x00U
#define MAP_FILTER_PRIORITY_HIGHPRIO_ONLY        0x01U
#define MAP_FILTER_PRIORITY_NONHIGHPRIO_ONLY     0x02U

/* Attachment - 1 Byte Bit-Mask */
#define MAP_ATTACHMENT_OFF                       0x00U
#define MAP_ATTACHMENT_ON                        0x01U

/* Transparent - 1 Byte Bit-Mask */
#define MAP_TRANSPARENT_OFF                      0x00U
#define MAP_TRANSPARENT_ON                       0x01U

/* Retry - 1 Byte Bit-Mask */
#define MAP_RETRY_OFF                            0x00U
#define MAP_RETRY_ON                             0x01U

/* NewMessage - 1 Byte Bit-Mask */
#define MAP_NEWMESSAGE_OFF                       0x00U
#define MAP_NEWMESSAGE_ON                        0x01U

/* NotificationStatus - 1 Byte Bit-Mask */
#define MAP_NOTIFICATIONSTATUS_OFF               0x00U
#define MAP_NOTIFICATIONSTATUS_ON                0x01U

/* MASInstanceID - 1 Byte Value : 0 to 255 */
/* Parameter Mask - 4 Byte Bit-Mask */
#define MAP_PARAM_MASK_SUBJECT                   0x00000001U
#define MAP_PARAM_MASK_DATETIME                  0x00000002U
#define MAP_PARAM_MASK_SENDER_NAME               0x00000004U
#define MAP_PARAM_MASK_SENDER_ADDRESSING         0x00000008U
#define MAP_PARAM_MASK_RECIPIENT_NAME            0x00000010U
#define MAP_PARAM_MASK_RECIPIENT_ADDRESSING      0x00000020U
#define MAP_PARAM_MASK_TYPE                      0x00000040U
#define MAP_PARAM_MASK_SIZE                      0x00000080U
#define MAP_PARAM_MASK_RECEPTION_STATUS          0x00000100U
#define MAP_PARAM_MASK_TEXT                      0x00000200U
#define MAP_PARAM_MASK_ATTACHMENT_SIZE           0x00000400U
#define MAP_PARAM_MASK_PRIORITY                  0x00000800U
#define MAP_PARAM_MASK_READ                      0x00001000U
#define MAP_PARAM_MASK_SENT                      0x00002000U
#define MAP_PARAM_MASK_PROTECTED                 0x00004000U
#define MAP_PARAM_MASK_REPLYTO_ADDRESSING        0x00008000U

/* FolderListingSize - 2 Byte value : 0x0000 to 0xFFFF */
/* MessagesListingSize - 2 Byte value : 0x0000 to 0xFFFF */
/* SubjectLength - 1 Byte Value : 0 to 255 */

/* Charset - 1 Byte Bit-Mask */
#define MAP_CHARSET_NATIVE                       0x00U
#define MAP_CHARSET_UTF8                         0x01U

/* FractionRequest - 1 Byte Bit-Mask */
#define MAP_FRACTIONREQUEST_FIRST                0x00U
#define MAP_FRACTIONREQUEST_NEXT                 0x01U

/* FractionDeliver - 1 Byte Bit-Mask */
#define MAP_FRACTIONDELIVER_MORE                 0x00U
#define MAP_FRACTIONDELIVER_LAST                 0x01U

/* StatusIndicator - 1 Byte Bit-Mask */
#define MAP_STATUSIND_READ_STATUS                0x00U
#define MAP_STATUSIND_DELETED_STATUS             0x01U

/* StatusValue - 1 Byte Bit-Mask */
#define MAP_STATUSVALUE_NO                       0x00U
#define MAP_STATUSVALUE_YES                      0x01U

/**
 * MAP NotificationFilterMask - 4Byte Bit-Mask
 */
#define MAP_NTF_FLTR_MASK_NEW_MSG                0x00000001U /* NewMessage */
#define MAP_NTF_FLTR_MASK_MSG_DELETED            0x00000002U /* MessageDeleted */
#define MAP_NTF_FLTR_MASK_MSG_SHIFT              0x00000004U /* MessageShift */
#define MAP_NTF_FLTR_MASK_SENDING_SUCCESS        0x00000008U /* SendingSuccess */
#define MAP_NTF_FLTR_MASK_SENDING_FAILURE        0x00000010U /* SendingFailure */
#define MAP_NTF_FLTR_MASK_DELIVERY_SUCCESS       0x00000020U /* DeliverySuccess */
#define MAP_NTF_FLTR_MASK_DELIVERY_FAILURE       0x00000040U /* DeliveryFailure */
#define MAP_NTF_FLTR_MASK_MEMORY_FULL            0x00000080U /* MemoryFull */
#define MAP_NTF_FLTR_MASK_MEMORY_AVAILABLE       0x00000100U /* MemoryAvailable */
#define MAP_NTF_FLTR_MASK_READ_STS_CHNGD         0x00000200U /* ReadStatusChanged */
#define MAP_NTF_FLTR_MASK_CONV_CHNGD             0x00000400U /* ConversationChanged */
#define MAP_NTF_FLTR_MASK_PTPNT_PRNCE_CHNGD      0x00000800U /* ParticipantPresenceChanged */
#define MAP_NTF_FLTR_MASK_PTPNT_CHT_ST_CHNGD     0x00001000U /* ParticipantChatStateChanged */
#define MAP_NTF_FLTR_MASK_MSG_EXTD_DATA_CHNGD    0x00002000U /* MessageExtendedDataChanged */
#define MAP_NTF_FLTR_MASK_MSG_REMOVED            0x00004000U /* MessageRemoved */

/**
 * MAP ConvParameterMask -4byte bit-mask
 */
#define MAP_CONV_PARAM_MASK_CONV_NAME            0x00000001U /* Conversation Name */
#define MAP_CONV_PARAM_MASK_CONV_L_ACTVTY        0x00000002U /* Conversation last_activity */
#define MAP_CONV_PARAM_MASK_CONV_RD_STS          0x00000004U /* Conversation read_status */
#define MAP_CONV_PARAM_MASK_CONV_VER_CNTR        0x00000008U /* Conversation version_counter */
#define MAP_CONV_PARAM_MASK_CONV_SUMMARY         0x00000010U /* Conversation summary */
#define MAP_CONV_PARAM_MASK_PARTICIPANTS         0x00000020U /* Participants */
#define MAP_CONV_PARAM_MASK_PTPNT_UCI            0x00000040U /* Participant UCI */
#define MAP_CONV_PARAM_MASK_PTPNT_DPLY_NAME      0x00000080U /* Participant display_name */
#define MAP_CONV_PARAM_MASK_PTPNT_CHT_STATE      0x00000100U /* Participant chat_state */
#define MAP_CONV_PARAM_MASK_PTPNT_L_ACTVTY       0x00000200U /* Participant last_activity */
#define MAP_CONV_PARAM_MASK_PTPNT_X_BT_UID       0x00000400U /* Participant x_bt_uid */
#define MAP_CONV_PARAM_MASK_PTPNT_NAME           0x00000800U /* Participant name */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_ALTY     0x00001000U /* Participant presence_availability */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_TEXT     0x00002000U /* Participant presence_text */
#define MAP_CONV_PARAM_MASK_PTPNT_PRNCE_PRTY     0x00004000U /* Participant priority */

/**
 * MAP SupportedFeatures -4byte bit-mask
 */
#define MAP_SUPP_FTRS_MASK_NTY_REG               0x00000001U /* Notification Registration Feature */
#define MAP_SUPP_FTRS_MASK_NTY                   0x00000002U /* Notification Feature */
#define MAP_SUPP_FTRS_MASK_BROWSING              0x00000004U /* Browsing Feature */
#define MAP_SUPP_FTRS_MASK_UPLOADING             0x00000008U /* Uploading Feature */
#define MAP_SUPP_FTRS_MASK_DELETE                0x00000010U /* Delete Feature */
#define MAP_SUPP_FTRS_MASK_INST_INFO             0x00000020U /* Instance Information Feature */
#define MAP_SUPP_FTRS_MASK_EXTED_EVNT_RPT_11     0x00000040U /* Extended Event Report 1.1 */
#define MAP_SUPP_FTRS_MASK_EVNT_RPT_12           0x00000080U /* Event Report Version 1.2 */
#define MAP_SUPP_FTRS_MASK_MSG_FRMT_11           0x00000100U /* Message Format Version 1.1 */
#define MAP_SUPP_FTRS_MASK_MSG_LIST_FRMT_11      0x00000200U /* Messages-Listing Format Version 1.1 */
#define MAP_SUPP_FTRS_MASK_PRST_MSG_HNDLS        0x00000400U /* Persistent Message Handles */
#define MAP_SUPP_FTRS_MASK_DATABASE_ID           0x00000800U /* Database Identifier */
#define MAP_SUPP_FTRS_MASK_FLDR_VER_CNTR         0x00001000U /* Folder Version Counter */
#define MAP_SUPP_FTRS_MASK_CONV_VER_CNTR         0x00002000U /* Conversation Version Counters */
#define MAP_SUPP_FTRS_MASK_PTPNT_PRNCE_CHNG_NTY  0x00004000U /* Participant Presence Change Notification */
#define MAP_SUPP_FTRS_MASK_PTPNT_CHT_ST_CHNG_NTY 0x00008000U /* Participant Chat State Change Notification */
#define MAP_SUPP_FTRS_MASK_PBAP_CNTCT_CRS_REF    0x00010000U /* PBAP Contact Cross Reference */
#define MAP_SUPP_FTRS_MASK_NTFY_FLTR             0x00020000U /* Notification Filtering */
#define MAP_SUPP_FTRS_MASK_UTC_TIMESTMP_FRMT     0x00040000U /* UTC offset timestamp format */
#define MAP_SUPP_FTRS_MASK_CONV_LISTING          0x00100000U /* Conversation Listing */
#define MAP_SUPP_FTRS_MASK_OWNER_STATUS          0x00200000U /* Owner Status */

/**
 * MAPv1.4 Message Forwand Constants
 */
#define MAP_FRWD_MODIFY_TEXT_REPLACE             0x00U
#define MAP_FRWD_MODIFY_TEXT_PREPEND             0x01U

/**
 * Max. Conversation id size
 */
#define MAX_CONVERSATION_ID_SIZE                (32U + 1U) /* NULL terminator included */

/**
 * Max. Presence Text
 */
#define MAX_PRESENCE_TEXT_SIZE                  (255U + 1U) /* NULL terminator included */

/**
 * Max Timestamp size
 */
#define MAX_MAP_TIMESTAMP_SIZE                  (20U + 1U) /* NULL terminator inlcude */
/** \} */
/* ----------------------------------------------------- */

/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup map_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** MAP Handle type */
typedef UINT8 MAP_HANDLE;

/** MAP Service Types */
typedef enum
{
    MAP_ACCESS_SERVICE = 0x01U,
    MAP_NTF_SERVICE = 0x02U

} MAP_SERVICE_TYPE;

/** MAP Entity Handle */
typedef struct _MAP_HANDLE_INFO
{
    /** Service Type */
    MAP_SERVICE_TYPE    service;

    /** Entity MAP Handle ID */
    MAP_HANDLE    entity_id;

} MAP_HANDLE_INFO;

/** MAP Header structure, to exchage data with application */
typedef struct _MAP_HEADER_STRUCT
{
    UCHAR   * value;
    UINT16    length;

} MAP_HEADER_STRUCT;

/** MAP Application Parameter Structure */
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


/** MAP Connection Structure */
typedef struct _MAP_CONNECT_STRUCT
{
    /** Peer bluetooth device address */
    UCHAR               * bd_addr;

    /** Maximum Receive Size */
    UINT16                max_recv_size;

    /** Peer MAP RFCOMM channel to connect */
    UINT8                 server_channel;

#ifdef MAP_1_2
    /** Remote MAP entitys OBEX L2CAP PSM */
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


/** MAP Request Structure */
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


/** MAP Response Structure */
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


/** MAP common headers */
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
/** \} */
/** \} */

/**
 * \defgroup map_appl_callback Application Callback
 * \{
 * Describes Application Callback defined by the module.
 */

/** MAP Event Notification Callback type */
typedef API_RESULT (* MAP_EVENT_NTF_CB)
                   (
                        MAP_HANDLE_INFO * map_handle,
                        UINT8             event_type,
                        UINT16            event_result,
                        MAP_HEADERS     * map_headers,
                        UINT16            num_headers
                   );
/** \} */

/* --------------------------------------------- Macros */

/**
 * \defgroup map_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */

/** To initialize a Header Structure */
#define MAP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0U;

#ifndef MAP_1_3
/** To set application parameter flag */
#define MAP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        *(flag) = *(flag) | (set_field)

/** To retrieve the field value (Set/Reset) */
#define MAP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        (*(flag) & (get_field))

/** Resets all the fields of the application parameter flag */
#define MAP_RESET_APPL_PARAM_FLAG(flag)            \
        *(flag) = 0x00000000U

/** To check if application parameter flag is zero */
#define MAP_APPL_PARAM_FLAG_IS_ZERO(flag) \
        (0U == (flag)[0U])

#else /* MAP_1_3 */
/** To set application parameter flag */
#define MAP_SET_APPL_PARAM_FLAG_EXT(flag, set_field, index)   \
        (flag)[(index)] = ((flag)[(index)]) | (set_field)

/** To retrieve the field value (Set/Reset) */
#define MAP_GET_APPL_PARAM_FLAG_EXT(flag, get_field, index)   \
        (((flag)[(index)]) & (get_field))

/** Resets all the fields of the application parameter flag */
#define MAP_RESET_APPL_PARAM_FLAG_EXT(flag_ptr, index)        \
        (flag_ptr)[(index)] = 0x00000000U

/** To check if application parameter flag is zero */
#define MAP_APPL_PARAM_FLAG_IS_ZERO(flag) \
        ((0U == (flag)[0U]) && (0U == (flag)[1U]))

/** Mapping for default macros */
#define MAP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        MAP_SET_APPL_PARAM_FLAG_EXT((flag), (set_field), 0U)

#define MAP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        MAP_GET_APPL_PARAM_FLAG_EXT((flag), (get_field), 0U)

#define MAP_RESET_APPL_PARAM_FLAG(flag)    \
        MAP_RESET_APPL_PARAM_FLAG_EXT((flag), 0U); \
        MAP_RESET_APPL_PARAM_FLAG_EXT((flag), 1U)
#endif /* MAP_1_3 */

/** Extract header structure value */
#define MAP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/** Extract application parameters */
#define MAP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(MAP_APPL_PARAMS));   \
        if(((from_params)->search_value.length != 0U) &&                    \
           ((from_params)->search_value.value != NULL))                     \
        {                                                                   \
            BT_mem_copy((search_value_out),                                 \
                   (from_params)->search_value.value,                       \
                   (from_params)->search_value.length);                     \
            (to_params)->search_value.value = (search_value_out);           \
            (to_params)->search_value.length =                              \
                                (from_params)->search_value.length;         \
        }
/** \} */

/* --------------------------------------------- API Declarations */

/**
 * \defgroup map_api API definitions
 * \{
 * Describes API definitions defined by the module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* MAP Message Client APIs */
#ifdef MAP_MCE
/**
 *  \brief To initialize MAP MCE module
 *
 *  \par Description:
 *       This API initializes the MAP MCE module. This function must be called
 *       (only once) before any other MAP MCE APIs are used.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mce_init (void);

/**
 *  \brief To shutdown MAP MCE module.
 *
 *  \par Description:
 *       This API shuts down the MAP MCE module if it is already initialized.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mce_shutdown (void);

/**
 *  \brief To start an instance of MAP MCE.
 *
 *  \par Description:
 *       This API starts a MAP MCE instance. On successful instantiation,
 *       this API sets and returns the MAP handle, which is an index to the MAP
 *       MCE entity started. This also registers the application callback
 *       function for the started instance.
 *
 *  \param [in] map_service
 *         MAP Service Type (Access/Notification) to be started.
 *
 *  \param [in] map_mce_cb
 *         The application Callback function pointer to be registered.
 *
 *  \param [out] map_handle
 *         Index to the Client MAS/MNS entity that is started.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note All further communication exchanged between the application and
 *        profile for the MCE instance will be identified using the assigned
 *        MAP handle.
 */
API_RESULT BT_map_mce_start
           (
               /* IN  */ MAP_SERVICE_TYPE    map_service,
               /* IN  */ MAP_EVENT_NTF_CB    map_mce_cb,
               /* OUT */ MAP_HANDLE        * map_handle
           );

/**
 *  \brief To stop an active MAP MCE instance.
 *
 *  \par Description:
 *       This API stops the MAP MCE instance corresponding to the specified handle.
 *
 *  \param [in] map_service
 *         MAP Service Type (Access/Notification) to be stopped.
 *
 *  \param [in] map_handle
 *         MAP Handle if the MAS/MNS instance that has to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 */
API_RESULT BT_map_mce_stop
           (
               /* IN */ MAP_SERVICE_TYPE    map_service,
               /* IN */ MAP_HANDLE        * map_handle
           );

/**
 *  \brief To establish the MAP Access Service Connection.
 *
 *  \par Description:
 *       This API establishes the OBEX Connection with the Remote MAP Server
 *       (MSE) for the Message Access Service (MAS). MAP MCE calls
 *       the registered application callback upon the completion of
 *       the connection procedure (when the MSE responds with the
 *       Connection Response) with the notification MAP_MCE_CONNECT_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance that has to be connected.
 *
 *  \param [in] map_connect_info
 *         Structure containing the Bluetooth Device Address of the MSE,
 *         the RFCOMM server channel of the MSE,the local maximum receive
 *         capacity (in Bytes) and L2CAP psm of MSE.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Before calling this API the application needs to ensure that the MAP
 *        session has been started by calling BT_map_mce_start().
 */
API_RESULT BT_map_mce_connect
           (
               /* IN */ MAP_HANDLE         * map_handle,
               /* IN */ MAP_CONNECT_STRUCT * map_connect_info
           );

/**
 *  \brief To disconnect the established MAP Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active MAP Access Service Connection with
 *       the Remote MAP MSE. MAP MCE calls the registered application callback
 *       upon completion of this request with the notification MAP_MCE_DISCONNECT_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note MAP MCE automatically initiates Transport Disconnection once the MAP
 *        disconnection completes and the application shall be notified with
 *        MAP_MCE_DISCONNECT_CNF and MAP_MCE_TRANSPORT_CLOSE_CNF for MAP
 *        disconnection and Transport disconnection completion respectively.
 */
API_RESULT BT_map_mce_disconnect
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/**
 *  \brief To disconnect the underlying transport of MAP Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the Transport Connection of the Access
 *       Service with the Remote MAP MSE. MAP MCE calls the registered
 *       application callback upon completion of this request with the
 *       notification MAP_MCE_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note The MAP Connection on this Transport will be closed automatically with
 *        the Transport Disconnection.
 */
API_RESULT BT_map_mce_transport_close
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/**
 *  \brief To request an update of Inbox to the MSE.
 *
 *  \par Description:
 *       This API requests the connected MSE to update its Inbox. Upon receiving
 *       a response from the Server, the application is notified using the
 *       registered event callback through the event MAP_MCE_UPDATE_INBOX_CNF
 *       along with the result of the operation.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE using BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_update_inbox
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/**
 *  \brief To set the current folder of the MSE
 *
 *  \par Description:
 *       This API sets the current folder at the connected MSE entity.
 *       This API makes use of OBEX API to send the setpath request.
 *       The setpath request shall be used for the following functions:
 *       i) Switch to Child Directory
 *      ii) Switch to Parent Directory
 *      ii) Switch to Root Directory
 *       The purpose of Setpath request is determined by the contents of
 *       map_set_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_SET_FOLDER_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing name of the folder to be set at the MSE and the
 *         setpath flag, indicating the folder to be set.
 *         This flag has to be set to one of the following values
 *         (defined in BT_map_api.h), corresponding to the requests:
 *      i)   MAP_SET_CHILD_FOLDER to switch to the Child Directory
 *      ii)  MAP_SET_PARENT_FOLDER to switch to the Previous Directory
 *      iii) MAP_SET_ROOT_FOLDER to Switch to the Root Directory
 *
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_set_folder
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

/**
 *  \brief To get the Message Listing from the current or given directory in the MSE
 *
 *  \par Description:
 *       This API retrieves the messages listing of the current folder set at
 *       the connected MSE entity or the specified child folder.
 *       The application shall include the folder name and may include the
 *       optional application parameters for the request using the
 *       map_get_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_MESSAGES_LISTING_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_get_info
 *         Structure containing folder name to retrieve the messages listing
 *         from and the application parameters if any, that may be included by
 *         the application. The following application parameters are supported
 *         as part of this request for this parameter.
 *         MaxListCount, ListStartOffset, SubjectLength, ParameterMask,
 *         FilterMessageType, FilterPeriodBegin, FilterPeriodEnd,
 *         FilterReadStatus, FilterRecipient, FilterOriginator, FilterPriority,
 *         ConversationID, FilterMessageHandle
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_get_folder_listing
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/**
 *  \brief To get the Message Listing from the current or given directory in the MSE
 *
 *  \par Description:
 *       This API retrieves the messages listing of the current folder set at
 *       the connected MSE entity or the specified child folder.
 *       The application shall include the folder name and may include the
 *       optional application parameters for the request using the map_get_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_MESSAGES_LISTING_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_get_info
 *         Structure containing folder name to retrieve the messages listing
 *         from and the application parameters if any, that may be included by
 *         the application. The following application parameters are supported
 *         as part of this request for this parameter.
 *         MaxListCount, ListStartOffset, SubjectLength, ParameterMask,
 *         FilterMessageType, FilterPeriodBegin, FilterPeriodEnd,
 *         FilterReadStatus, FilterRecipient, FilterOriginator, FilterPriority,
 *         ConversationID, FilterMessageHandle
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_get_messages_listing
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/**
 *  \brief To get message from server (MSE)
 *
 *  \par Description:
 *       This API retrieves the message with the given message handle from the
 *       connected MSE device. The application shall specify the message
 *       handle and application parameters through the map_get_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_MESSAGE_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_get_info
 *         Structure containing the message handle that is to be retrieved and
 *         the application parameters. The following are the mandatory and
 *         optional application parameters that are supported for the request
 *         through this parameter.
 *         Attachment (M), Charset (M), FractionRequest (O)
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_get_message
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_get_info
           );

/**
 *  \brief To set message status on server (MSE).
 *
 *  \par Description:
 *       This API sets the message status indicator value of a message with
 *       given handle in the MSE. The application shall specify the message
 *       handle and application parameters through the map_set_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_SET_MESSAGE_STATUS_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the message handle whose status indicator has to
 *         be updated and the application parameters that convey the indicator
 *         parameter to be updated and the value.
 *         The following are the mandatory application parameters that are
 *         supported for the request through this parameter.
 *         StatusIndicator, StatusValue, ExtendedData
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_set_message_status
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

/**
 *  \brief To push a message object to the server (MSE)
 *
 *  \par Description:
 *       This API pushes the given message object to the MSE. The application
 *       shall specify the name of folder to which the message has to be
 *       directed to at the MSE, the application parameters and the message body
 *       through the parameter map_set_info.
 *       The completion of this procedure is notified through
 *       MAP_MCE_PUSH_MESSAGE_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing name of the folder at MSE to which the message
 *         is directed along with the application parameters and message body.
 *         The following are the optional and mandatory application parameters
 *         that are supported for the request through this parameter-
 *         Transparent (O), Retry (O), Charset (M), ConversationID (O),
 *         MessageHandle(C1), Attachment(C2), Modify Text(C2)
 *         O: Optional
 *         M: Mandastory.
 *        C1: Optional if Message Forward is Supported by MSE & MCE.
 *        C2: Mandatory if C1 is present.
 *
 *  \param [in] more
 *         Indicates if application has remaining message still to be sent or
 *         message complete
 *         1 - Message part remaining
 *         0 - No more message part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_push_message
           (
               /* IN  */ MAP_HANDLE           * map_handle,
               /* IN  */ MAP_REQUEST_STRUCT   * map_set_info,
               /* IN  */ UCHAR                  more,
               /* OUT */ UINT16               * actual
           );

/**
 *  \brief To register for enabling or disabling notification with server (MSE)
 *
 *  \par Description:
 *       This API registers with the MSE to enable or disable the notification
 *       feature. The application shall include the NotificationStatus
 *       application parameter through the map_set_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_SET_NTF_REGISTRATION_CNF event along with the status of the
 *       procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the mandatory NotificationStatus application
 *         parameter
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_set_ntf_registration
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info
           );

#ifdef MAP_1_2
/**
 *  \brief To get MAS Instance information of given MASInstanceID from MSE.
 *
 *  \par Description:
 *       This API is used to retrieve user-readable information about the
 *       MAS instance provided by the MSE. The application shall specify the
 *       MASInstanceID application parameter through the map_get_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_MASINSTINFO_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance.
 *
 *  \param [in] map_get_info
 *         Structure containing the MASInstanceID application parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This operation is MAP v1.2 related feature. Enable MAP_1_2 feature flag
 *        in BT_features.h.
 *        Set the 'wait' parameter in map_get_info, if the remote MSE has to
 *        wait for subsequent Get requests after the first response. This shall
 *        be used only when the underlying transport for OBEX is over L2CAP.
 */
API_RESULT BT_map_mce_get_mas_instance_info
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );
#endif /* MAP_1_2 */

#ifdef MAP_1_3
/**
 *  \brief To retrieve Conversation-Listing object from the MSE.
 *
 *  \par Description:
 *       This API is used to retrieve Conversation-Listing objects from the MSE.
 *       The MCE shall not send this request if the 'Conversation listing' bit
 *       in the MapSupportedFeatures of the MSE is not set.
 *       The application can include the optional application parameter through
 *       the map_get_info parameter. List of application parameters for this
 *       request are: MaxListCount, ListStartOffset, FilterLastActivityBegin,
 *       FilterLastActivityEnd, FilterReadStatus, ConversationID, ConvParameterMask.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_CONVERSATION_LIST_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_get_info
 *         Structure containing the optional application parameters.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_get_conversation_listing
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );

/**
 *  \brief To set owner status on the MSE.
 *
 *  \par Description:
 *       This API is used to change the Presence, Chat State or Last Activity of
 *       the owner on the MSE. The MCE shall not send this request if the
 *       'Owner status' bit in the MapSupportedFeatures of MSE is not set.
 *       The application can include the optional application parameter through
 *       the map_set_info parameter. List of application parameters for this
 *       request are: PresenceAvailability, PresenceText, LastActivity, ChatState,
 *       ConversationID.
 *       The completion of this procedure is notified through
 *       MAP_MCE_SET_OWNER_STATUS_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the optional application parameters.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_set_owner_status
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_set_info
           );

/**
 *  \brief To get owner status on the MSE.
 *
 *  \par Description:
 *       This API is used to get the Presence, Chat State or Last Activity of
 *       the owner on the MSE.
 *       The application can include the optional application parameter through
 *       the map_get_info parameter. List of application parameters for this
 *       request are: ConversationID.
 *       The completion of this procedure is notified through
 *       MAP_MCE_GET_OWNER_STATUS_CNF event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the optional application parameter.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_get_owner_status
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_get_info
           );

/**
 *  \brief To set Message Notification and Notification Filtering feature with MSE.
 *
 *  \par Description:
 *       This API can be used to specify which notifications to receive from the
 *       MSE. If this request is not sent, then all events will be sent to the
 *       MCE when notification registration is ON. The application shall include
 *       the NotificationFilterMask application parameter through the
 *       map_set_info parameter.
 *       The completion of this procedure is notified through
 *       MAP_MCE_SET_NOTIFICATION_FILTER_CNF event along with the status of the
 *       procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the mandatory NotificationFilterMask application
 *         parameter
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the MAP Connection to a MAP MSE through BT_map_mce_connect()
 *        API before using this API.
 */
API_RESULT BT_map_mce_set_notification_filter
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_REQUEST_STRUCT  * map_set_info
           );
#endif /* MAP_1_3 */

/* MAP Client Send continuing request if required for ongoing operation */
/**
 *  \brief To sends a request to the MAP Server.
 *
 *  \par Description:
 *       This API sends the request to the MAP Server with the corresponding MAP
 *       Event and the data given by the application contained in the map_headers.
 *       The application shall call this API to continue any operation initiated
 *       and to which the first notification has been received. Thus this API
 *       shall be called from the MAP_MCE Application Callback function with the
 *       event_type set to the current received Event.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] event_type
 *         Event Type that was received in the Application callback, to indicate
 *         that the request is the continuation of the operation for which it
 *         was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] map_headers
 *         Structure containing either the Connect Information in case of Connect
 *         operation or Get Request information, in case of Get operation or Put
 *         Request information in case of Put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of message still to be sent or
 *         message complete
 *         1 - Message part remaining
 *         0 - No more message part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API is used only in case of the operations with multiple Get or
 *        Put Requests.
 *        This API shall never be used to send other requests for which direct
 *        APIs are provided.
 *        The application shall make use of the actual parameter to track the
 *        offset of the object used for BODY header, which may have to be
 *        transmitted in subsequent chunks.
 *        If the current operation is Get, then set the 'wait' parameter in Get
 *        Request information, if the remote MSE has to wait for subsequent
 *        requests after responding for this request. This shall be used only
 *        when the underlying transport for OBEX is over L2CAP
 */
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
/**
 *  \brief To abort the ongoing MAP operation
 *
 *  \par Description:
 *       This API aborts the ongoing Get/Put operation that was initiated by the
 *       Client.
 *       The completion of the Abort operation is notified through
 *       MAP_MCE_ABORT_CNF notification event along with the status of the procedure.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance of which the operation is to be aborted.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mce_abort
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

#ifdef MAP_SUPPORT_NOTIFICATION
/* MAP Client Disconnect Notification Service Instance Transport */
/**
 *  \brief To disconnect the underlying transport of MAP Notification Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the Transport Connection of the Notification
 *       Service with the Remote MAP MSE. MAP MCE calls the registered
 *       application callback upon completion of this request with the
 *       notification MAP_MCE_NS_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note The MAP Notification Service Connection on this Transport will be
 *        closed automatically with the Transport Disconnection.
 */
API_RESULT BT_map_mce_ns_transport_close
           (
               /* IN */ MAP_HANDLE   * map_handle
           );

/* MAP Client Send Response for last received request on Notification Channel */
/**
 *  \brief To send Response to the latest Request received over Notification
 *         channel from the MAP MSE.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the MAP MSE.
 *       Response to Connect, SendEvent etc. shall be sent through this API.
 *       The application shall call this API with the Event Type of the latest
 *       Notification received. This indicates the response is for the latest
 *       indication received.
 *       MAP MCE frames the OBEX specific packets based on the response and
 *       associated response_data and send it to the MAP MSE.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance.
 *
 *  \param [in] event_type
 *         Event Type that was received in the Application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX Response Code.
 *
 *  \param [in] map_headers
 *         Structure containing either the Connect Information, in case of
 *         Connect operation or the Response data, in case of Get operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note If the current operation is Get, then set the 'wait' parameter in Get
 *        Response information, if the remote MCE has to send Get request again
 *        after receiving current Get response. This shall be used only when the
 *        underlying transport for OBEX is over L2CAP.
 */
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
/**
 *  \brief To initialize MAP MSE module.
 *
 *  \par Description:
 *       This API initializes the MAP MSE module. This function must be called
 *       (only once) before any other MAP MSE APIs are used.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API can be called again to re-initialize MAP MSE module, after a successful call to BT_map_mse_shutdown().
 */
API_RESULT BT_map_mse_init (void);

/**
 *  \brief To shutdown MAP MSE module.
 *
 *  \par Description:
 *       This API shuts down the MAP MSE.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API shall be called only when there are no active/started
 *        instances of the MSE. In case of active MSE instances, they should be
 *        stopped by calling BT_map_mse_stop() API.
 */
API_RESULT BT_map_mse_shutdown (void);

/**
 *  \brief To start a MAP MSE instance.
 *
 *  \par Description:
 *       This API starts a MAP MSE instance. On the successful instantiation,
 *       this API returns a MAP handle, which is an index to the MAP MSE entity.
 *       This also registers the application callback function for the started
 *       MSE instance.
 *
 *  \param [in] map_service
 *         MAP Service Type (Access/Notification) to be started.
 *
 *  \param [in] map_mse_cb
 *         The application Callback function pointer to be registered.
 *
 *  \param [out] map_handle
 *         Index to the Server MAS/MNS Entity that is started.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mse_start
           (
               /* IN  */ MAP_SERVICE_TYPE    map_service,
               /* IN  */ MAP_EVENT_NTF_CB    map_mse_cb,
               /* OUT */ MAP_HANDLE        * map_handle
           );

/**
 *  \brief To stop an active MAP MSE instance.
 *
 *  \par Description:
 *       This API stops the MAP MSE instance corresponding to the specified handle.
 *
 *  \param [in] map_service
 *         MAP Service Type (Access/Notification) to be stopped.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS/MNS instance that has to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 */
API_RESULT BT_map_mse_stop
           (
               /* IN */ MAP_SERVICE_TYPE    map_service,
               /* IN */ MAP_HANDLE        * map_handle
           );

/**
 *  \brief To disconnect the underlying transport of MAP Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the Transport Connection of the Access
 *       Service with the Remote MAP MCE. MAP MSE calls the registered
 *       application callback upon completion of this request with the
 *       notification MAP_MSE_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note The MAP Access Service Connection on this Transport will be closed
 *        automatically with the Transport Disconnection.
 */
API_RESULT BT_map_mse_transport_close
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/**
 *  \brief To send Response to the latest Request received from the MAP MCE.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the MAP
 *       MCE. Response to Connect, GetFolderListing, GetMessagesListing,
 *       GetMessage etc. shall be sent through this API.
 *       The application shall call this API with the Event Type of the latest
 *       Notification received. This indicates the response is for the latest
 *       indication received.
 *       MAP MSE frames the OBEX specific packets based on the response and
 *       associated response data and send it to the MAP MCE.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MAS instance.
 *
 *  \param [in] event_type
 *         Event Type that was received in the Application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX Response Code.
 *
 *  \param [in] map_headers
 *         Structure containing either the Connect Information, in case of
 *         Connect operation or the Response data, in case of Get operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mse_send_response
           (
                /* IN */ MAP_HANDLE           * map_handle,
                /* IN */ UINT8                  event_type,
                /* IN */ UCHAR                  event_result,
                /* IN */ MAP_HEADERS          * map_headers
           );

#ifdef MAP_SUPPORT_NOTIFICATION
/**
 *  \brief To establish the MAP Notification Service Connection.
 *
 *  \par Description:
 *       This API establishes the OBEX Connection with the Remote MAP Client
 *       (MCE) for the Message Notification Service (MNS). MAP MSE calls the
 *       registered application callback upon the completion of the connection
 *       procedure (when the MCE responds with the Connection Response)
 *       with the notification MAP_MSE_NS_CONNECT_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance that has to be connected.
 *
 *  \param [in] map_connect_info
 *         Structure containingthe Bluetooth Device Address of the MCE,
 *         the RFCOMM server channel of Notification Service at the MCE ,
 *         the local maximum receive capacity (in Bytes) and L2CAP psm of
 *         Notification Service at the MCE.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Before calling this API the application needs to ensure that an active
 *        MAS session is established with the MCE. This API will return failure
 *        otherwise.
 */
API_RESULT BT_map_mse_ns_connect
           (
               /* IN */ MAP_HANDLE          * map_handle,
               /* IN */ MAP_CONNECT_STRUCT  * map_connect_info
           );

/**
 *  \brief To disconnect the Established MAP Notification Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active MAP Notification Service
 *       Connection with the Remote MAP MCE. MAP MSE calls the registered
 *       application callback upon completion of this request with the
 *       notification MAP_MSE_NS_DISCONNECT_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note MAP MSE automatically initiates Transport Disconnection once the MAP
 *        disconnection completes and the application shall be notified with
 *        MAP_MSE_NS_DISCONNECT_CNF and MAP_MSE_NS_TRANSPORT_CLOSE_CNF for
 *        MAP disconnection and Transport disconnection completion of the
 *        Notification Service respectively.
 */
API_RESULT BT_map_mse_ns_disconnect
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/**
 *  \brief To disconnect the underlying transport of MAP Notification Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the Transport Connection of the Notification
 *       Service with the Remote MAP MCE. MAP MSE calls the registered application
 *       callback upon completion of this request with the notification
 *       MAP_MSE_NS_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note The MAP Connection on this Transport will be closed automatically with
 *        the Transport Disconnection.
 */
API_RESULT BT_map_mse_ns_transport_close
           (
               /* IN */ MAP_HANDLE  * map_handle
           );

/**
 *  \brief To send a Notification Event to the MNS Server in the MAP Client.
 *
 *  \par Description:
 *       This API sends a Notification event over Notification channel from the
 *       MNS server to the MAP Client. The application shall call this API with
 *       the event report body and the application parameter 'MASInstanceID' to
 *       refer the MAS Instance in Client to which the notification is intended.
 *
 *  \param [in] map_handle
 *         MAP Handle of the MNS instance.
 *
 *  \param [in] map_set_info
 *         Structure containing the notification event report content and the
 *         MASInstanceID application parameter
 *
 *  \param [in] more
 *         Indicates if application has remaining of message still to be sent or message complete
 *         1 - Message part remaining
 *         0 - No more message part remaining
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_map_mse_ns_send_event
           (
               /* IN */ MAP_HANDLE           * map_handle,
               /* IN */ MAP_REQUEST_STRUCT   * map_set_info,
               /* IN */ UCHAR                  more
           );

/**
 *  \brief To sends a request to the MAP Notification Service Server.
 *
 *  \par Description:
 *      This API sends the request to the MAP Notification Server with the
 *      corresponding MAP Event and the data given by the application contained
 *      in the map_headers.  The application shall call this API to continue
 *      any operation initiated and to which the first notification has been
 *      received. Thus this API shall be called from the MAP_MSE Application
 *      Callback function with the event_type set to the current received Event.
 *
 * \param [in] map_handle
 *        MAP Handle of the MNS instance.
 *
 * \param [in] event_type
 *        Event Type that was received in the Application callback, to indicate
 *        that the request is the continuation of the operation for which it was notified.
 *
 * \param [in] event_result
 *        This is the status of the request.
 *
 * \param [in] map_headers
 *        Structure containing either Get Request information, in case of Get
 *        operation or Put Request information in case of Put operation.
 *
 * \param [in] Indicates if application has remaining of message still to be
 *        sent or message complete
 *        1 - Message part remaining
 *        0 - No more message part remaining
 *
 * \param [out] actual
 *        Contains the actual number of bytes of BODY header, if any included
 *        in the request, that is packed for transmission.
 *
 * \return
 *      API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 * \note This API is used only in case of the operations with multiple Get or
 *       Put Requests.
 *       This API shall never be used to send other requests for which direct
 *       APIs are provided.
 *       The application shall make use of the actual parameter to track the
 *       offset of the object used for BODY header, which may have to be
 *       transmitted in subsequent chunks.
 *       If the current operation is Get, then set the 'wait' parameter in Get
 *       Request information, if the remote MCE has to wait for subsequent
 *       requests after responding for this request. This shall be used only
 *       when the underlying transport for OBEX is over L2CAP
 */
API_RESULT BT_map_mse_ns_send_request
           (
               /* IN  */ MAP_HANDLE   * map_handle,
               /* IN  */ UINT8          event_type,
               /* IN  */ UINT16         event_result,
               /* IN  */ MAP_HEADERS  * map_headers,
               /* IN  */ UCHAR          more,
               /* OUT */ UINT16       * actual
           );

#endif /* MAP_SUPPORT_NOTIFICATION */

#endif /* MAP_MSE */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_MAP_API_ */

