/** @file
 *  @brief Bluetooth PBAP types.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_PBAP_TYPES_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_PBAP_TYPES_H_

/**
 * @brief Phone Book Access Profile (PBAP)
 * @defgroup bt_pbap Phone Book Access Profile (PBAP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/obex.h>

/* PBAP Version Counter size */
#define BT_PBAP_FLDR_VER_CNTR_SIZE       0x10U /* 16 */
/*PBAP Database Identifiers size */
#define BT_PBAP_DATABASE_IDENTIFIER_SIZE 0x10U /* 16 */
/* PBAP Supported Feature*/
#define BT_PBAP_SUPPORTED_FEATURES_V11 0x00000003U

/**
 *  @name PBAP OBEX Headers
 *
 * Constant Definitions for PBAP OBEX Headers
 */
#define BT_PBAP_HDR_NAME           BT_OBEX_HDR_NAME
#define BT_PBAP_HDR_TYPE           BT_OBEX_HDR_TYPE
#define BT_PBAP_HDR_TARGET         BT_OBEX_HDR_TARGET
#define BT_PBAP_HDR_BODY           BT_OBEX_HDR_BODY
#define BT_PBAP_HDR_END_OF_BODY    BT_OBEX_HDR_END_OF_BODY
#define BT_PBAP_HDR_WHO            BT_OBEX_HDR_WHO
#define BT_PBAP_HDR_CONNECTION_ID  BT_OBEX_HDR_CONNECTION_ID
#define BT_PBAP_HDR_APP_PARAM      BT_OBEX_HDR_APP_PARAM
#define BT_PBAP_HDR_AUTH_CHALLENGE BT_OBEX_HDR_AUTH_CHALLENGE
#define BT_PBAP_HDR_AUTH_RESPONSE  BT_OBEX_HDR_AUTH_RESPONSE
#define BT_PBAP_HDR_SRM            BT_OBEX_HDR_SRM
#define BT_PBAP_HDR_SRMP           BT_OBEX_HDR_SRMP

/**
 * @name PBAP Event Result Types
 *
 * Constant Definitions for PBAP Event RESULT Types
 */
#define BT_PBAP_CONTINUE_RSP        BT_OBEX_RSP_CONTINUE        /* Continue */
#define BT_PBAP_SUCCESS_RSP         BT_OBEX_RSP_SUCCESS         /* OK, Success */
#define BT_PBAP_BAD_REQ_RSP         BT_OBEX_RSP_BAD_REQ         /* Bad Request - server couldn’t understand request */
#define BT_PBAP_UNAUTH_RSP          BT_OBEX_RSP_UNAUTH          /* Unauthorized */
#define BT_PBAP_FORBIDDEN_RSP       BT_OBEX_RSP_FORBIDDEN       /* Forbidden - operation is understood but refused */
#define BT_PBAP_NOT_FOUND_RSP       BT_OBEX_RSP_NOT_FOUND       /* Not Found */
#define BT_PBAP_NOT_ACCEPTABLE_RSP  BT_OBEX_RSP_NOT_ACCEPTABLE  /* Not Acceptable */
#define BT_PBAP_PRECOND_FAILED_RSP  BT_OBEX_RSP_PRECOND_FAILED  /* Precondition Failed */
#define BT_PBAP_NOT_IMPLEMENTED_RSP BT_OBEX_RSP_NOT_IMPLEMENTED /* Not Implemented */
#define BT_PBAP_NO_SERVICE_RSP      BT_OBEX_RSP_SERVICE_UNAVBL  /* Service Unavailable */

/* PBAP function tpye */
#define BT_PBAP_TYPE_HDR_PHONEBOOK  "x-bt/phonebook"
#define BT_PBAP_TYPE_HDR_VCARD_LIST "x-bt/vcard-listing"
#define BT_PBAP_TYPE_HDR_VCARD      "x-bt/vcard"

/**
 * @name PBAP profile supported feature
 *
 * Constant Definitions for PBAP profile supported featur
 */
#define BT_PBAP_FEATURE_DOWNLOAD                     (1UL << 0U)
#define BT_PBAP_FEATURE_BROWSING                     (1UL << 1U)
#define BT_PBAP_FEATURE_DATABASE_IDENTIFIER          (1UL << 2U)
#define BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS      (1UL << 3U)
#define BT_PBAP_FEATURE_VCARD_SELECTING              (1UL << 4U)
#define BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS        (1UL << 5U)
#define BT_PBAP_FEATURE_X_BT_UCL_VCARD_PROPERTY      (1UL << 6U)
#define BT_PBAP_FEATURE_X_BT_UID_VCARD_PROPERTY      (1UL << 7U)
#define BT_PBAP_FEATURE_CONTACT_REFERENCING          (1UL << 8U)
#define BT_PBAP_FEATURE_DEFAULT_CONTACT_IMAGE_FORMAT (1UL << 9U)

/**
 * @name PBAP Application Parameter Tag
 *
 * Constant Definitions for PBAP Application Parameter Tag ID
 */
#define BT_PBAP_TAG_ID_ORDER                    0x01U /* 1  Byte Length  */
#define BT_PBAP_TAG_ID_SEARCH_VALUE             0x02U /* Variable Length */
#define BT_PBAP_TAG_ID_SEARCH_PROPERTY          0x03U /* 1  Byte Length  */
#define BT_PBAP_TAG_ID_MAX_LIST_COUNT           0x04U /* 2       "       */
#define BT_PBAP_TAG_ID_LIST_START_OFFSET        0x05U /* 2       "       */
#define BT_PBAP_TAG_ID_PROPERTY_SELECTOR        0x06U /* 8       "       */
#define BT_PBAP_TAG_ID_FORMAT                   0x07U /* 1       "       */
#define BT_PBAP_TAG_ID_PHONE_BOOK_SIZE          0x08U /* 2       "       */
#define BT_PBAP_TAG_ID_NEW_MISSED_CALLS         0x09U /* 1       "       */
#define BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION   0x0AU /* 16      "       */
#define BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION 0x0BU /* 16      "       */
#define BT_PBAP_TAG_ID_VCARD_SELECTOR           0x0CU /* 8       "       */
#define BT_PBAP_TAG_ID_DATABASE_IDENTIFIER      0x0DU /* 16      "       */
#define BT_PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR  0x0EU /* 1       "       */
#define BT_PBAP_TAG_ID_RESET_NEW_MISSED_CALLS   0x0FU /* 1       "       */
#define BT_PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES  0x10U /* 4       "       */

/** @brief PBAP Authentication structure.
 *  if auth is be provided, its memory cannot be released during the entire connection hold process
 *  Authentication of user_id is not supported now, support pin_code only now.
 */
struct bt_pbap_auth
{
    char *user_id;
    char *pin_code;
};

struct bt_pbap_pull_phonebook_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_PBAP_TYPE_HDR_PHONEBOOK)];
    } __packed type;
} __packed;

struct bt_pbap_set_path_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    uint8_t Flags;
    uint8_t constant;
    struct bt_obex_hdr_u32 conn_id;
} __packed;

struct bt_pbap_pull_vcard_listing_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_PBAP_TYPE_HDR_VCARD_LIST)];
    } __packed type;
} __packed;

struct bt_pbap_pull_vcard_entry_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
    struct
    {
        uint8_t hi;
        uint16_t length;
        uint8_t hv[sizeof(BT_PBAP_TYPE_HDR_VCARD)];
    } __packed type;
} __packed;

struct bt_pbap_ops_get_cont_hdr
{
    uint8_t opcode;
    uint16_t length;
    struct bt_obex_hdr_u32 conn_id;
    struct bt_obex_hdr_u8 srmp;
} __packed;

struct bt_pbap_connection_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    uint8_t obex_version;
    uint8_t flags;
    uint16_t maximum_packet_length;
    uint8_t target[16];
} __packed;

struct bt_pbap_push_response_hdr
{
    uint8_t opcode;
    uint16_t packet_length;
    struct bt_obex_hdr_u8 srm;
    struct bt_obex_hdr_u8 srmp;
} __packed;

/**
 * @brief Add pbap application parameters into net buffer.
 *
 * @note Calling these functions should follow a call to net_buf_reserve.
 *
 * @param buf net buffer for TX.
 *
 * @param val value to be written into buffer.
 *
 * @param len the length of value.
 */
#define BT_PBAP_ADD_PARAMS_ORDER(buf, value)                    BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_ORDER, (value))
#define BT_PBAP_ADD_PARAMS_SEARCH_VALUE(buf, value, len)        bt_obex_add_app_param((buf), BT_PBAP_TAG_ID_SEARCH_VALUE, (value), (len))
#define BT_PBAP_ADD_PARAMS_SEARCH_PROPERTY(buf, value)          BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_SEARCH_PROPERTY, (value))
#define BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, value)           BT_OBEX_ADD_APP_PARAM_U16((buf), BT_PBAP_TAG_ID_MAX_LIST_COUNT, (value))
#define BT_PBAP_ADD_PARAMS_LIST_START_OFFSET(buf, value)        BT_OBEX_ADD_APP_PARAM_U16((buf), BT_PBAP_TAG_ID_LIST_START_OFFSET, (value))
#define BT_PBAP_ADD_PARAMS_PROPERTY_SELECTOR(buf, value)        BT_OBEX_ADD_APP_PARAM_U64((buf), BT_PBAP_TAG_ID_PROPERTY_SELECTOR, (value))
#define BT_PBAP_ADD_PARAMS_FORMAT(buf, value)                   BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_FORMAT, (value))
#define BT_PBAP_ADD_PARAMS_PHONE_BOOK_SIZE(buf, value)         BT_OBEX_ADD_APP_PARAM_U16((buf), BT_PBAP_TAG_ID_PHONE_BOOK_SIZE, (value))
#define BT_PBAP_ADD_PARAMS_NEW_MISSED_CALLS(buf, value)         BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_NEW_MISSED_CALLS, (value))
#define BT_PBAP_ADD_PARAMS_PRIMARY_FOLDER_VERSION(buf, value)   bt_obex_add_app_param((buf), BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION, (value), BT_PBAP_FLDR_VER_CNTR_SIZE)
#define BT_PBAP_ADD_PARAMS_SECONDARY_FOLDER_VERSION(buf, value) bt_obex_add_app_param((buf), BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION, (value), BT_PBAP_FLDR_VER_CNTR_SIZE)
#define BT_PBAP_ADD_PARAMS_VCARD_SELECTOR(buf, value)           BT_OBEX_ADD_APP_PARAM_U64((buf), BT_PBAP_TAG_ID_VCARD_SELECTOR, (value))
#define BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, value)      bt_obex_add_app_param((buf), BT_PBAP_TAG_ID_DATABASE_IDENTIFIER, (value), BT_PBAP_DATABASE_IDENTIFIER_SIZE)
#define BT_PBAP_ADD_PARAMS_VCARD_SELECTOR_OPERATOR(buf, value)  BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR, (value))
#define BT_PBAP_ADD_PARAMS_RESET_NEW_MISSED_CALLS(buf, value)   BT_OBEX_ADD_APP_PARAM_U8((buf), BT_PBAP_TAG_ID_RESET_NEW_MISSED_CALLS, (value))
#define BT_PBAP_ADD_PARAMS_PBAP_SUPPORTED_FEATURES(buf, value)  BT_OBEX_ADD_APP_PARAM_U32((buf), BT_PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES, (value))

/**
 * @brief Add PBAP headers into net buffer.
 *
 * @param buf net buffer for TX.
 * @param val value to be written into buffer.
 * @param len the length of value.
 */
#define BT_PBAP_ADD_BODY(buf, val, len)        bt_obex_add_hdr(buf, BT_PBAP_HDR_BODY, val, len)
#define BT_PBAP_ADD_END_OF_BODY(buf, val, len) bt_obex_add_hdr(buf, BT_PBAP_HDR_END_OF_BODY, val, len)

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_PBAP_TYPES_H_ */
