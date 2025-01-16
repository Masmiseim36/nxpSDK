/** @file
 *  @brief Bluetooth MAP types.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_MAP_TYPES_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_MAP_TYPES_H_

#include <bluetooth/obex.h>

/**
 * @brief Message Access Profile (MAP)
 * @defgroup bt_map Message Access Profile (MAP)
 * @ingroup bluetooth
 * @{
 */

/* MAP version */
#define BT_MAP_VERSION_1_1 (0x0101U)
#define BT_MAP_VERSION_1_2 (0x0102U)
#define BT_MAP_VERSION_1_3 (0x0103U)
#define BT_MAP_VERSION_1_4 (0x0104U)

/* Message Handle size */
#define BT_MAP_MSG_HANDLE_SIZE (8U * 4U + 2U) /* 34U - unicode for uint64_t with null terminated */
/* Conversation ID size */
#define BT_MAP_CONVO_ID_SIZE (16U * 2U + 1U) /* 33U - string for 128-bit value with null terminated */
/* Database ID size */
#define BT_MAP_DATABASE_ID_SIZE (16U * 2U + 1U) /* 33U - string for 128-bit value with null terminated */
/* Database ID size */
#define BT_MAP_VER_CNT_SIZE (16U * 2U + 1U) /* 33U - string for 128-bit value with null terminated */

/* MAP v1.1 supported features(mandatory) */
#define BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11 (0x0000001FU)
/* MAP v1.2 supported features(mandatory) */
#define BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V12 (0x0000007FU)
/* MAP v1.3 supported features(mandatory) */
#define BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V13 (0x000601FFU)
/* MAP v1.4 supported features(mandatory) */
#define BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V14 (0x000601FFU)

/**
 *  @name MAP OBEX Headers
 *
  * Constant Definitions for MAP OBEX Headers
 */
#define BT_MAP_HDR_NAME             (BT_OBEX_HDR_NAME)
#define BT_MAP_HDR_TYPE             (BT_OBEX_HDR_TYPE)
#define BT_MAP_HDR_TARGET           (BT_OBEX_HDR_TARGET)
#define BT_MAP_HDR_BODY             (BT_OBEX_HDR_BODY)
#define BT_MAP_HDR_END_OF_BODY      (BT_OBEX_HDR_END_OF_BODY)
#define BT_MAP_HDR_CONNECTION_ID    (BT_OBEX_HDR_CONNECTION_ID)
#define BT_MAP_HDR_APP_PARAM        (BT_OBEX_HDR_APP_PARAM)

/**
 * @name MAP OBEX Response Codes
 *
 * Constant Definitions for MAP OBEX Response Codes
 */
#define BT_MAP_RSP_CONTINUE           (BT_OBEX_RSP_CONTINUE)        /* Continue */
#define BT_MAP_RSP_SUCCESS            (BT_OBEX_RSP_SUCCESS)         /* OK, Success */
#define BT_MAP_RSP_BAD_REQ            (BT_OBEX_RSP_BAD_REQ)         /* Bad Request - server couldn't understand request */
#define BT_MAP_RSP_NOT_IMPLEMENTED    (BT_OBEX_RSP_NOT_IMPLEMENTED) /* Not implemented */
#define BT_MAP_RSP_UNAUTH             (BT_OBEX_RSP_UNAUTH)          /* Unauthorized */
#define BT_MAP_RSP_PRECOND_FAILED     (BT_OBEX_RSP_PRECOND_FAILED)  /* Precondition Failed */
#define BT_MAP_RSP_NOT_FOUND          (BT_OBEX_RSP_NOT_FOUND)       /* Not Found */
#define BT_MAP_RSP_NOT_ACCEPTABLE     (BT_OBEX_RSP_NOT_ACCEPTABLE)  /* Not Acceptable */
#define BT_MAP_RSP_SERVICE_UNAVBL     (BT_OBEX_RSP_SERVICE_UNAVBL)  /* Service Unavailable */
#define BT_MAP_RSP_FORBIDDEN          (BT_OBEX_RSP_FORBIDDEN)       /* Forbidden - operation is understood but refused */
#define BT_MAP_RSP_INT_SERVER_ERR     (BT_OBEX_RSP_INT_SERVER_ERR)  /* Internal Server Error */

/**
 * @name MAP Message Type
 *
 * Constant Definitions for MAP Message Type
 */
#define BT_MAP_TYPE_SEND_EVENT                     "x-bt/MAP-event-report"
#define BT_MAP_TYPE_SET_NTF_REG                    "x-bt/MAP-NotificationRegistration"
#define BT_MAP_TYPE_GET_FOLDER_LISTING             "x-obex/folder-listing"
#define BT_MAP_TYPE_GET_MSG_LISTING                "x-bt/MAP-msg-listing"
#define BT_MAP_TYPE_GET_MSG                        "x-bt/message"
#define BT_MAP_TYPE_SET_MSG_STATUS                 "x-bt/messageStatus"
#define BT_MAP_TYPE_PUSH_MSG                       "x-bt/message"
#define BT_MAP_TYPE_UPDATE_INBOX                   "x-bt/MAP-messageUpdate"
#define BT_MAP_TYPE_GET_MAS_INST_INFO              "x-bt/MASInstanceInformation"
#define BT_MAP_TYPE_SET_OWNER_STATUS               "x-bt/ownerStatus"
#define BT_MAP_TYPE_GET_OWNER_STATUS               "x-bt/ownerStatus"
#define BT_MAP_TYPE_GET_CONVO_LISTING              "x-bt/MAP-convo-listing"
#define BT_MAP_TYPE_SET_NTF_FILTER                 "x-bt/MAP-notification-filter"

/**
 * @name MAP Supported Features
 *
 * Constant Definitions for MAP Supported Features
 */
#define BT_MAP_NTF_REG_FEATURE                     (1UL << 0U)
#define BT_MAP_NTF_FEATURE                         (1UL << 1U)
#define BT_MAP_BROWSING_FEATURE                    (1UL << 2U)
#define BT_MAP_UPLOADING_FEATURE                   (1UL << 3U)
#define BT_MAP_DELETE_FEATURE                      (1UL << 4U)
#define BT_MAP_INST_INFO_FEATURE                   (1UL << 5U)
#define BT_MAP_EXT_EVENT_REPORT_1_1                (1UL << 6U)
#define BT_MAP_EXT_EVENT_VERSION_1_2               (1UL << 7U)
#define BT_MAP_MSG_FORMAT_VERSION_1_1              (1UL << 8U)
#define BT_MAP_MSG_LISTING_FORMAT_VERSION_1_1      (1UL << 9U)
#define BT_MAP_PERSISTENT_MSG_HANDLE               (1UL << 10U)
#define BT_MAP_DATABASE_ID                         (1UL << 11U)
#define BT_MAP_FOLDER_VERSION_CNTR                 (1UL << 12U)
#define BT_MAP_CONVO_VERSION_CNTR                  (1UL << 13U)
#define BT_MAP_PARTICIPANT_PRESENCE_CHANGE_NTF     (1UL << 14U)
#define BT_MAP_PARTICIPANT_CHAT_STATE_CHANGE_NTF   (1UL << 15U)
#define BT_MAP_PBAP_CONTACT_CROSS_REF              (1UL << 16U)
#define BT_MAP_NTF_FILTERING                       (1UL << 17U)
#define BT_MAP_UTC_OFFSET_TIMESTAMP_FORMAT         (1UL << 18U)
#define BT_MAP_SUPPORTED_FEATURES_CONNECT_REQ      (1UL << 19U)
#define BT_MAP_CONVO_LISTING                       (1UL << 20U)
#define BT_MAP_OWNER_STATUS                        (1UL << 21U)
#define BT_MAP_MSG_FORWARDING                      (1UL << 22U)

/**
 * @name MAP Application Parameter Tag
 *
 * Constant Definitions for MAP Application Parameter Tag ID
 */
#define BT_MAP_TAG_ID_MAX_LIST_COUNT                (0x01U)  /* Length : 2 bytes */
#define BT_MAP_TAG_ID_LIST_START_OFFSET             (0x02U)  /* Length : 2 bytes */
#define BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE           (0x03U)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN           (0x04U)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_PERIOD_END             (0x05U)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_READ_STATUS            (0x06U)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_FILTER_RECIPIENT              (0x07U)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_ORIGINATOR             (0x08U)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_PRIORITY               (0x09U)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_ATTACHMENT                    (0x0AU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_TRANSPARENT                   (0x0BU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_RETRY                         (0x0CU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_NEW_MESSAGE                   (0x0DU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_NOTIFICATION_STATUS           (0x0EU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_MAS_INSTANCE_ID               (0x0FU)  /* Length : 1 byte */
#define BT_MAP_TAG_ID_PARAMETER_MASK                (0x10U)  /* Length : 4 bytes */
#define BT_MAP_TAG_ID_FOLDER_LISTING_SIZE           (0x11U)  /* Length : 2 bytes */
#define BT_MAP_TAG_ID_LISTING_SIZE                  (0x12U)  /* Length : 2 bytes */
#define BT_MAP_TAG_ID_SUBJECT_LENGTH                (0x13U)  /* Length : variable (max 255 bytes) */
#define BT_MAP_TAG_ID_CHARSET                       (0x14U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_FRACTION_REQUEST              (0x15U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_FRACTION_DELIVER              (0x16U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_STATUS_INDICATOR              (0x17U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_STATUS_VALUE                  (0x18U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_MSE_TIME                      (0x19U)  /* Length : variable */
#define BT_MAP_TAG_ID_DATABASE_IDENTIFIER           (0x1AU)  /* Length : variable (max 32 bytes) */
#define BT_MAP_TAG_ID_CONV_LIST_VER_CNTR            (0x1BU)  /* Length : variable (max 32 bytes) */
#define BT_MAP_TAG_ID_PRESENCE_AVAILABILITY         (0x1CU)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_PRESENCE_TEXT                 (0x1DU)  /* Length : variable */
#define BT_MAP_TAG_ID_LAST_ACTIVITY                 (0x1EU)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN    (0x1FU)  /* Length : variable */
#define BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END      (0x20U)  /* Length : variable */
#define BT_MAP_TAG_ID_CHAT_STATE                    (0x21U)  /* Length : 1 bytes */
#define BT_MAP_TAG_ID_CONVERSATION_ID               (0x22U)  /* Length : variable (max 32 bytes) */
#define BT_MAP_TAG_ID_FOLDER_VER_CNTR               (0x23U)  /* Length : variable (max 32 bytes) */
#define BT_MAP_TAG_ID_FILTER_MSG_HANDLE             (0x24U)  /* Length : variable (max 16 bytes) */
#define BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK      (0x25U)  /* Length : 4 bytes */
#define BT_MAP_TAG_ID_CONV_PARAMETER_MASK           (0x26U)  /* Length : 4 bytes */
#define BT_MAP_TAG_ID_OWNER_UCI                     (0x27U)  /* Length : variable */
#define BT_MAP_TAG_ID_EXTENDED_DATA                 (0x28U)  /* Length : variable */
#define BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES        (0x29U)  /* Length : 4 bytes */
#define BT_MAP_TAG_ID_MESSAGE_HANDLE                (0x2AU)  /* Length : 8 bytes */
#define BT_MAP_TAG_ID_MODIFY_TEXT                   (0x2BU)  /* Length : 1 byte */

struct bt_map_ops_get_cont_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srmp;
} __packed;

struct bt_map_ops_put_cont_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
} __packed;

struct map_mce_connect_hdr
{
    uint8_t opcode;
    uint16_t length;
    uint8_t version;
    uint8_t flags;
    uint16_t max_pkt_len;
    uint8_t uuid[16];
} __packed;

struct bt_map_get_folder_listing_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_FOLDER_LISTING)];
    } __packed type;
} __packed;

struct bt_map_set_folder_hdr
{
    uint8_t opcode;
    uint16_t length;
    uint8_t flags;
    uint8_t constant;
    struct bt_obex_hdr_u32 conn_id;
} __packed;

struct bt_map_get_msg_listing_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_MSG_LISTING)];
    } __packed type;
} __packed;

struct bt_map_get_msg_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[BT_MAP_MSG_HANDLE_SIZE];
    } __packed name;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_MSG)];
    } __packed type;
} __packed;

struct bt_map_set_msg_status_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[BT_MAP_MSG_HANDLE_SIZE];
    } __packed name;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_SET_MSG_STATUS)];
    } __packed type;
} __packed;

struct bt_map_push_msg_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_PUSH_MSG)];
    } __packed type;
} __packed;

struct bt_map_set_ntf_reg_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_SET_NTF_REG)];
    } __packed type;
} __packed;

struct bt_map_update_inbox_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_UPDATE_INBOX)];
    } __packed type;
} __packed;

struct bt_map_get_mas_inst_info_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_MAS_INST_INFO)];
    } __packed type;
} __packed;

struct bt_map_set_owner_status_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_SET_OWNER_STATUS)];
    } __packed type;
} __packed;

struct bt_map_get_owner_status_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_OWNER_STATUS)];
    } __packed type;
} __packed;

struct bt_map_get_convo_listing_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_GET_CONVO_LISTING)];
    } __packed type;
} __packed;

struct bt_map_set_ntf_filter_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_SET_NTF_FILTER)];
    } __packed type;
} __packed;

struct bt_map_send_event_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_MAP_TYPE_SEND_EVENT)];
    } __packed type;
} __packed;

struct bt_map_resp_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
} __packed;

/**
 * @brief Add MAP application parameters into net buffer.
 *
 * @note Calling these functions should follow a call to net_buf_reserve.
 *
 * @param buf net buffer for TX.
 * @param val value to be written into buffer.
 * @param len the length of value.
 */
#define BT_MAP_ADD_MAX_LIST_COUNT(buf, val)                   BT_OBEX_ADD_APP_PARAM_U16((buf), BT_MAP_TAG_ID_MAX_LIST_COUNT, (val))
#define BT_MAP_ADD_LIST_START_OFFSET(buf, val)                BT_OBEX_ADD_APP_PARAM_U16((buf), BT_MAP_TAG_ID_LIST_START_OFFSET, (val))
#define BT_MAP_ADD_FILTER_MESSAGE_TYPE(buf, val)              BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE, (val))
#define BT_MAP_ADD_FILTER_PERIOD_BEGIN(buf, val, len)         bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN, (val), (len))
#define BT_MAP_ADD_FILTER_PERIOD_END(buf, val, len)           bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_PERIOD_END, (val), (len))
#define BT_MAP_ADD_FILTER_READ_STATUS(buf, val)               BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_FILTER_READ_STATUS, (val))
#define BT_MAP_ADD_FILTER_RECIPIENT(buf, val, len)            bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_RECIPIENT, (val), (len))
#define BT_MAP_ADD_FILTER_ORIGINATOR(buf, val, len)           bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_ORIGINATOR, (val), (len))
#define BT_MAP_ADD_FILTER_PRIORITY(buf, val)                  BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_FILTER_PRIORITY, (val))
#define BT_MAP_ADD_ATTACHMENT(buf, val)                       BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_ATTACHMENT, (val))
#define BT_MAP_ADD_TRANSPARENT(buf, val)                      BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_TRANSPARENT, (val))
#define BT_MAP_ADD_RETRY(buf, val)                            BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_RETRY, (val))
#define BT_MAP_ADD_NEW_MESSAGE(buf, val)                      BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_NEW_MESSAGE, (val))
#define BT_MAP_ADD_NOTIFICATION_STATUS(buf, val)              BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_NOTIFICATION_STATUS, (val))
#define BT_MAP_ADD_MAS_INSTANCE_ID(buf, val)                  BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_MAS_INSTANCE_ID, (val))
#define BT_MAP_ADD_PARAMETER_MASK(buf, val)                   BT_OBEX_ADD_APP_PARAM_U32((buf), BT_MAP_TAG_ID_PARAMETER_MASK, (val))
#define BT_MAP_ADD_FOLDER_LISTING_SIZE(buf, val)              BT_OBEX_ADD_APP_PARAM_U16((buf), BT_MAP_TAG_ID_FOLDER_LISTING_SIZE, (val))
#define BT_MAP_ADD_LISTING_SIZE(buf, val)                     BT_OBEX_ADD_APP_PARAM_U16((buf), BT_MAP_TAG_ID_LISTING_SIZE, (val))
#define BT_MAP_ADD_SUBJECT_LENGTH(buf, val)                   BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_SUBJECT_LENGTH, (val))
#define BT_MAP_ADD_CHARSET(buf, val)                          BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_CHARSET, (val))
#define BT_MAP_ADD_FRACTION_REQUEST(buf, val)                 BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_FRACTION_REQUEST, (val))
#define BT_MAP_ADD_FRACTION_DELIVER(buf, val)                 BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_FRACTION_DELIVER, (val))
#define BT_MAP_ADD_STATUS_INDICATOR(buf, val)                 BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_STATUS_INDICATOR, (val))
#define BT_MAP_ADD_STATUS_VALUE(buf, val)                     BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_STATUS_VALUE, (val))
#define BT_MAP_ADD_MSE_TIME(buf, val, len)                    bt_obex_add_app_param((buf), BT_MAP_TAG_ID_MSE_TIME, (val), (len))
#define BT_MAP_ADD_DATABASE_IDENTIFIER(buf, val, len)         bt_obex_add_app_param((buf), BT_MAP_TAG_ID_DATABASE_IDENTIFIER, (val), (len))
#define BT_MAP_ADD_CONV_LIST_VER_CNTR(buf, val, len)          bt_obex_add_app_param((buf), BT_MAP_TAG_ID_CONV_LIST_VER_CNTR, (val), (len))
#define BT_MAP_ADD_PRESENCE_AVAILABILITY(buf, val)            BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_PRESENCE_AVAILABILITY, (val))
#define BT_MAP_ADD_PRESENCE_TEXT(buf, val, len)               bt_obex_add_app_param((buf), BT_MAP_TAG_ID_PRESENCE_TEXT, (val), (len))
#define BT_MAP_ADD_LAST_ACTIVITY(buf, val, len)               bt_obex_add_app_param((buf), BT_MAP_TAG_ID_LAST_ACTIVITY, (val), (len))
#define BT_MAP_ADD_FILTER_LAST_ACTIVITY_BEGIN(buf, val, len)  bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN, (val), (len))
#define BT_MAP_ADD_FILTER_LAST_ACTIVITY_END(buf, val, len)    bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END, (val), (len))
#define BT_MAP_ADD_CHAT_STATE(buf, val)                       BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_CHAT_STATE, (val))
#define BT_MAP_ADD_CONVERSATION_ID(buf, val, len)             bt_obex_add_app_param((buf), BT_MAP_TAG_ID_CONVERSATION_ID, (val), (len))
#define BT_MAP_ADD_FOLDER_VER_CNTR(buf, val, len)             bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FOLDER_VER_CNTR, (val), (len))
#define BT_MAP_ADD_FILTER_MSG_HANDLE(buf, val, len)           bt_obex_add_app_param((buf), BT_MAP_TAG_ID_FILTER_MSG_HANDLE, (val), (len))
#define BT_MAP_ADD_NOTIFICATION_FILTER_MASK(buf, val)         BT_OBEX_ADD_APP_PARAM_U32((buf), BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK, (val))
#define BT_MAP_ADD_CONV_PARAMETER_MASK(buf, val)              BT_OBEX_ADD_APP_PARAM_U32((buf), BT_MAP_TAG_ID_CONV_PARAMETER_MASK, (val))
#define BT_MAP_ADD_OWNER_UCI(buf, val, len)                   bt_obex_add_app_param((buf), BT_MAP_TAG_ID_OWNER_UCI, (val), (len))
#define BT_MAP_ADD_EXTENDED_DATA(buf, val, len)               bt_obex_add_app_param((buf), BT_MAP_TAG_ID_EXTENDED_DATA, (val), (len))
#define BT_MAP_ADD_MAP_SUPPORTED_FEATURES(buf, val)           BT_OBEX_ADD_APP_PARAM_U32((buf), BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES, (val))
#define BT_MAP_ADD_MESSAGE_HANDLE(buf, val, len)              bt_obex_add_app_param((buf), BT_MAP_TAG_ID_MESSAGE_HANDLE, (val), (len))
#define BT_MAP_ADD_MODIFY_TEXT(buf, val)                      BT_OBEX_ADD_APP_PARAM_U8((buf), BT_MAP_TAG_ID_MODIFY_TEXT, (val))
/**
 * @brief Add MAP headers into net buffer.
 *
 * @param buf net buffer for TX.
 * @param val value to be written into buffer.
 * @param len the length of value.
 */
#define BT_MAP_ADD_BODY(buf, val, len)             bt_obex_add_hdr((buf), BT_MAP_HDR_BODY, (val), (len))
#define BT_MAP_ADD_END_OF_BODY(buf, val, len)      bt_obex_add_hdr((buf), BT_MAP_HDR_END_OF_BODY, (val), (len))

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_MAP_TYPES_H_ */
