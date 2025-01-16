/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <string.h>
#include <stdio.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/byteorder.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/pbap_pse.h>
#include "bt_pal_conn_internal.h"
#include "bt_pal_pbap_internal.h"
#include "BT_features.h"
#include "BT_config.h"
#include "BT_pbap_api.h"
#include "db_gen.h"
#include "eBT_os.h"
#include "bt_pal_l2cap_internal.h"

#if (defined(CONFIG_BT_PBAP_PSE) && ((CONFIG_BT_PBAP_PSE) > 0U))

#define LOG_ENABLE      IS_ENABLED(CONFIG_BT_DEBUG_PBAP_PSE)
#define LOG_MODULE_NAME bt_pbap_pse
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

/* Blocking code prepared for zephyr */
#define CONFIG_BT_ZEPHYR_BUF 0U
/* PBAP appl parameters count*/
#define BT_PBAP_APPL_PARAM_HDR_COUNT 16U

#define PBAP_PSE_MAX_PKT_SIZE OBEX_MAX_PACKET_LENGTH

NET_BUF_POOL_FIXED_DEFINE(pbap_pool, PBAP_PSE_MAX_ENTITY, PBAP_PSE_MAX_PKT_SIZE, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static struct bt_pbap_pse_cb *pbap_pse_cb;
static struct bt_pbap_auth auth_info[PBAP_PSE_MAX_ENTITY];

/** @brief PBAP PCE structure */
struct bt_pbap_pse
{
    uint8_t pbap_handle;
    struct bt_conn *acl_conn;
    uint16_t goep_version;
    uint16_t max_pkt_len;
    struct bt_pbap_auth *auth;
    uint32_t peer_feature;
    uint32_t local_feature;
    bool lcl_wait;
    bool rem_wait;
    bool local_auth;
    bool remote_auth;
    enum bt_obex_req_flags flag;
    enum bt_obex_req_flags cb_flag;
};

struct pbap_hdr
{
    uint8_t *value;
    uint16_t length;
};

static OSA_MUTEX_HANDLE_DEFINE(s_PbapPseLockMutex);
static osa_mutex_handle_t s_PbapPseLock;
static struct bt_pbap_pse s_PbapPseInstances[PBAP_PSE_MAX_ENTITY];
#define EDGEFAST_PBAP_PSE_LOCK   OSA_MutexLock(s_PbapPseLock, osaWaitForever_c)
#define EDGEFAST_PBAP_PSE_UNLOCK OSA_MutexUnlock(s_PbapPseLock)

/**
 * @brief Helper for getting body.
 *
 * A helper for getting Body header. The most
 * common scenario is to call this helper on the body received in
 * the callback that was given to bt_pbap_register.
 *
 * @param buf       net buffer returned in the callback registered by bt_pbap_register.
 * @param body      pointer used for holding body data.
 * @param length    the length of body data.
 */
#define bt_pbap_pse_get_body(buf, body, length) bt_obex_get_body(buf, body, length)

#define child_floader_count   7
static char *child_floader_name[child_floader_count] = {"pb", "ich", "och", "mch", "cch", "spd", "fav"};
static int startwith(char *str, char *prefix)
{
    uint8_t str_len    = strlen(str);
    uint8_t prefix_len = strlen(prefix);
    if (prefix_len > str_len)
    {
        return 0;
    }
    return strncmp(str, prefix, prefix_len) == 0;
}
static int endwith(char *str, char *suffix)
{
    uint8_t str_len    = strlen(str);
    uint8_t suffix_len = strlen(suffix);
    if (str_len < suffix_len)
    {
        return 0;
    }
    char *endpart = str + str_len - suffix_len;
    return strcmp(endpart, suffix) == 0;
}

static struct bt_pbap_pse *pbap_pse_get_instance(struct bt_conn *conn)
{
    EDGEFAST_PBAP_PSE_LOCK;
    for (uint8_t index = 0; index < PBAP_PSE_MAX_ENTITY; ++index)
    {
        if (s_PbapPseInstances[index].acl_conn == NULL)
        {
            memset(&s_PbapPseInstances[index], 0U, sizeof(s_PbapPseInstances[index]));
            s_PbapPseInstances[index].acl_conn = conn;
            EDGEFAST_PBAP_PSE_UNLOCK;
            return &s_PbapPseInstances[index];
        }
    }
    EDGEFAST_PBAP_PSE_UNLOCK;
    return NULL;
}

static void pbap_pse_free_instance(struct bt_pbap_pse *pbap_pse)
{
    if (NULL != pbap_pse)
    {
        EDGEFAST_PBAP_PSE_LOCK;
        pbap_pse->acl_conn = NULL;
        EDGEFAST_PBAP_PSE_UNLOCK;
    }
}

static struct bt_pbap_pse *bt_pabp_pse_lookup_bt_handle(uint8_t handle)
{
    EDGEFAST_PBAP_PSE_LOCK;
    for (uint8_t index = 0; index < PBAP_PSE_MAX_ENTITY; ++index)
    {
        if (handle == s_PbapPseInstances[index].pbap_handle)
        {
            EDGEFAST_PBAP_PSE_UNLOCK;
            return &s_PbapPseInstances[index];
        }
    }
    EDGEFAST_PBAP_PSE_UNLOCK;
    return NULL;
}

static struct bt_pbap_pse *bt_pbap_pse_lookup_bt_conn(struct bt_conn *conn)
{
    EDGEFAST_PBAP_PSE_LOCK;
    for (uint8_t index = 0; index < PBAP_PSE_MAX_ENTITY; ++index)
    {
        if (conn == s_PbapPseInstances[index].acl_conn)
        {
            EDGEFAST_PBAP_PSE_UNLOCK;
            return &s_PbapPseInstances[index];
        }
    }
    EDGEFAST_PBAP_PSE_UNLOCK;
    return NULL;
}

static int bt_pal_pbap_pse_stop_instance(struct bt_pbap_pse *pbap_pse)
{
    API_RESULT retval = 0;

    if (pbap_pse == NULL)
    {
        return -EINVAL;
    }

    retval = BT_pbap_pse_stop_instance(pbap_pse->pbap_handle);

    if (API_SUCCESS != retval)
    {
        LOG_ERR("Stop instance Failed. Reason 0x%04X \n", retval);
    }
    else
    {
        LOG_INF("Successfully stopped PBAP PCE Entity %d \n", pbap_pce->pbap_handle);
    }

    return retval;
}

static void bt_pabp_set_appl_params_hdr_value(uint8_t tag_id, PBAP_APPL_PARAMS *appl_params, struct net_buf *buf)
{
    switch (tag_id)
    {
        case BT_PBAP_TAG_ID_ORDER:
            BT_PBAP_ADD_PARAMS_ORDER(buf, appl_params->order);
            break;
        case BT_PBAP_TAG_ID_SEARCH_VALUE:
            BT_PBAP_ADD_PARAMS_SEARCH_VALUE(buf, appl_params->search_value.value,
                                            (uint8_t)strlen((char *)appl_params->search_value.value));
            break;
        case BT_PBAP_TAG_ID_SEARCH_PROPERTY:
            BT_PBAP_ADD_PARAMS_SEARCH_PROPERTY(buf, appl_params->search_attr);
            break;
        case BT_PBAP_TAG_ID_MAX_LIST_COUNT:
            BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, appl_params->max_list_count);
            break;
        case BT_PBAP_TAG_ID_LIST_START_OFFSET:
            BT_PBAP_ADD_PARAMS_LIST_START_OFFSET(buf, appl_params->list_start_offset);
            break;
        case BT_PBAP_TAG_ID_PROPERTY_SELECTOR:
            BT_PBAP_ADD_PARAMS_PROPERTY_SELECTOR(buf, sys_get_le64((uint8_t *)appl_params->filter));
            break;
        case BT_PBAP_TAG_ID_FORMAT:
            BT_PBAP_ADD_PARAMS_FORMAT(buf, appl_params->format);
            break;
        case BT_PBAP_TAG_ID_PHONE_BOOK_SIZE:
            BT_PBAP_ADD_PARAMS_PHONE_BOOK_SIZE(buf, appl_params->phonebook_size);
            break;
        case BT_PBAP_TAG_ID_NEW_MISSED_CALLS:
            BT_PBAP_ADD_PARAMS_NEW_MISSED_CALLS(buf, appl_params->new_missed_calls);
            break;
        case BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION:
            BT_PBAP_ADD_PARAMS_PRIMARY_FOLDER_VERSION(buf, (uint8_t *)appl_params->primary_folder_ver);
            break;
        case BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION:
            BT_PBAP_ADD_PARAMS_SECONDARY_FOLDER_VERSION(buf, (uint8_t *)appl_params->secondary_folder_ver);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR:
            BT_PBAP_ADD_PARAMS_VCARD_SELECTOR(buf, sys_get_le64((uint8_t *)appl_params->vcard_selector));
            break;
        case BT_PBAP_TAG_ID_DATABASE_IDENTIFIER:
            BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, (uint8_t *)appl_params->database_identifier);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR:
            BT_PBAP_ADD_PARAMS_VCARD_SELECTOR_OPERATOR(buf, appl_params->vcard_selector_operator);
            break;
        case BT_PBAP_TAG_ID_RESET_NEW_MISSED_CALLS:
            BT_PBAP_ADD_PARAMS_RESET_NEW_MISSED_CALLS(buf, appl_params->reset_new_missed_calls);
            break;
        case BT_PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES:
            BT_PBAP_ADD_PARAMS_PBAP_SUPPORTED_FEATURES(buf, appl_params->supported_features);
            break;
        default:
            break;
    }
}

static void header_from_stack_to_buf(struct net_buf *buf, PBAP_REQUEST_STRUCT *pbap_req_info)
{
    if (pbap_req_info == NULL || buf == NULL)
    {
        return;
    }

    /* appl params*/
    if (pbap_req_info->appl_params)
    {
        PBAP_APPL_PARAMS *appl_param = pbap_req_info->appl_params;
        for (uint8_t index = 0; index < BT_PBAP_APPL_PARAM_HDR_COUNT; ++index)
        {
            if (PBAP_GET_APPL_PARAM_FLAG(appl_param->appl_param_flag,
                                         (uint16_t)1U << index % BT_PBAP_APPL_PARAM_HDR_COUNT) != 0)
            {
                bt_pabp_set_appl_params_hdr_value(index + 1, appl_param, buf);
            }
        }
    }
}

static int bt_pbap_get_appl_param_hdr_value(struct bt_obex_tag_bytes *tag, PBAP_APPL_PARAMS *appl_params)
{
    uint64_t value;
    switch (tag->id)
    {
        case BT_PBAP_TAG_ID_ORDER:
            appl_params->order = *tag->value;
            break;
        case BT_PBAP_TAG_ID_SEARCH_VALUE:
            appl_params->search_value.value  = tag->value;
            appl_params->search_value.length = tag->length;
            break;
        case BT_PBAP_TAG_ID_SEARCH_PROPERTY:
            appl_params->search_attr = *tag->value;
            break;
        case BT_PBAP_TAG_ID_MAX_LIST_COUNT:
            appl_params->max_list_count = sys_get_be16(tag->value);
            break;
        case BT_PBAP_TAG_ID_LIST_START_OFFSET:
            appl_params->list_start_offset = sys_get_be16(tag->value);
            break;
        case BT_PBAP_TAG_ID_PROPERTY_SELECTOR:
            memcpy(appl_params->filter, tag->value, sizeof(appl_params->filter));
            value = sys_get_be64((uint8_t *)appl_params->filter);
            memcpy(appl_params->filter, &value, sizeof(appl_params->filter));
            break;
        case BT_PBAP_TAG_ID_FORMAT:
            appl_params->format = *tag->value;
            break;
        case BT_PBAP_TAG_ID_PHONE_BOOK_SIZE:
            appl_params->phonebook_size = sys_get_be16(tag->value);
            break;
        case BT_PBAP_TAG_ID_NEW_MISSED_CALLS:
            appl_params->new_missed_calls = *tag->value;
            break;
        case BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION:
            memcpy(appl_params->primary_folder_ver, tag->value, BT_PBAP_FLDR_VER_CNTR_SIZE);
            break;
        case BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION:
            memcpy(appl_params->secondary_folder_ver, tag->value, BT_PBAP_FLDR_VER_CNTR_SIZE);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR:
            memcpy(appl_params->vcard_selector, tag->value, sizeof(appl_params->vcard_selector));
            value = sys_get_be64((uint8_t *)appl_params->vcard_selector);
            memcpy(appl_params->vcard_selector, &value, sizeof(appl_params->vcard_selector));
            break;
        case BT_PBAP_TAG_ID_DATABASE_IDENTIFIER:
            memcpy(appl_params->database_identifier, tag->value, BT_PBAP_DATABASE_IDENTIFIER_SIZE);
            break;
        case BT_PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR:
            appl_params->vcard_selector_operator = *tag->value;
            break;
        case BT_PBAP_TAG_ID_RESET_NEW_MISSED_CALLS:
            appl_params->reset_new_missed_calls = *tag->value;
            break;
        case BT_PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES:
            appl_params->supported_features = sys_get_be32(tag->value);
            break;
        default:
            break;
    }
    return 0;
}

static int bt_pbap_form_stack_param(struct net_buf *buf, PBAP_APPL_PARAMS *appl_params)
{
    memset(appl_params, 0, sizeof(PBAP_APPL_PARAMS));

    uint16_t appl_param_len = 0;
    uint16_t hdr_length;
    uint8_t *hdr_value;
    struct bt_obex_tag_bytes tag;

    if (bt_obex_get_hdr(buf, OBEX_HDR_APP_PARAM, &hdr_value, &hdr_length) == 0)
    {
        appl_param_len = sizeof(struct bt_obex_hdr_bytes) + hdr_length;
        while (hdr_length)
        {
            bt_pbap_get_appl_param_hdr_value((struct bt_obex_tag_bytes *)hdr_value, appl_params);
            tag.id = ((struct bt_obex_tag_bytes *)hdr_value)->id - 1;
            PBAP_SET_APPL_PARAM_FLAG(appl_params->appl_param_flag,
                                     (uint16_t)(1U << tag.id % BT_PBAP_APPL_PARAM_HDR_COUNT));
            tag.length = ((struct bt_obex_tag_bytes *)hdr_value)->length + sizeof(struct bt_obex_tag_bytes);
            if (hdr_length < tag.length)
            {
                return -EINVAL;
            }
            hdr_length -= tag.length;
            hdr_value += tag.length;
        }
    }
    return appl_param_len;
}

static int bt_pbap_pse_send_reponse(
    struct bt_pbap_pse *pbap_pse, struct net_buf *buf, uint8_t result, bool wait, uint8_t command)
{
    PBAP_HEADER_STRUCT body;
    PBAP_HEADERS pbap_rsp_hdrs;
    PBAP_RESPONSE_STRUCT pbap_resp_info;
    PBAP_APPL_PARAMS pbap_appl_params;
    API_RESULT retval = 0;
    memset(&pbap_rsp_hdrs, 0, sizeof(pbap_rsp_hdrs));
    memset(&pbap_resp_info, 0, sizeof(pbap_resp_info));
    memset(&pbap_appl_params, 0, sizeof(pbap_appl_params));
    pbap_resp_info.appl_params   = &pbap_appl_params;
    pbap_rsp_hdrs.pbap_resp_info = &pbap_resp_info;

    if (pbap_pse == NULL || buf == NULL)
    {
        return -EINVAL;
    }
#if CONFIG_BT_ZEPHYR_BUF
    if (pbap_pse->flag & BT_OBEX_REQ_END)
    {
        if (result == BT_PBAP_CONTINUE_RSP) /* the first but not last package*/
        {
            pbap_pse->flag = BT_OBEX_REQ_CONTINUE;
        }
        else /* The first and last package */
        {
            pbap_pse->flag = BT_OBEX_REQ_UNSEG;
        }
        /* to do : net_buf_push opcode length ...*/
    }
    else if (pbap_pse->flag & BT_OBEX_REQ_CONTINUE)
    {
        if (result != BT_PBAP_CONTINUE_RSP) /* The last package of subsequent packages */
        {
            pbap_pse->flag = BT_OBEX_REQ_END;
        }
        /* to do : net_buf_add body if exists*/
    }
#endif
    /* form stack variable*/
    bt_pbap_form_stack_param(buf, &pbap_appl_params);
    if (bt_pbap_pse_get_body(buf, &body.value, &body.length) == 0)
    {
        pbap_resp_info.body = &body;
    }
    else
    {
        body.value          = NULL;
        body.length         = 0;
        pbap_resp_info.body = &body;
    }
    if (wait)
    {
        pbap_resp_info.wait = 1;
    }
    retval = BT_pbap_pse_send_response(pbap_pse->pbap_handle, command, result, &pbap_rsp_hdrs);
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Response send failed. Reason 0x%04X \n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successfully send response\r\n");
    }
    net_buf_unref(buf);
    return 0;
}

int bt_pbap_pse_pull_phonebook_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_pbap_pse_send_reponse(pbap_pse, buf, result, wait, PBAP_PSE_GET_PHONEBOOK_IND);
}

int bt_pbap_pse_pull_vcard_listing_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_pbap_pse_send_reponse(pbap_pse, buf, result, wait, PBAP_PSE_GET_VCARD_LIST_IND);
}

int bt_pbap_pse_pull_vcard_entry_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_pbap_pse_send_reponse(pbap_pse, buf, result, wait, PBAP_PSE_GET_VCARD_IND);
}

int bt_pbap_pse_set_phonebook_path_response(struct bt_pbap_pse *pbap_pse, uint8_t result)
{
    API_RESULT retval = 0;
    PBAP_HEADERS pbap_rsp_hdrs;
    pbap_rsp_hdrs.pbap_resp_info = NULL;
    retval = BT_pbap_pse_send_response(pbap_pse->pbap_handle, PBAP_PSE_SET_PHONEBOOK_IND, result, &pbap_rsp_hdrs);
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Response send failed. Reason 0x%04X \n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successfully send response\r\n");
    }
    return 0;
}

static int8_t bt_pal_pull_phonebook_param(PBAP_HEADERS *pbap_headers)
{
    /* Basic Name Requirements :  (xxx/) child_floader_name.vcf */
    int8_t index = 0;
    uint8_t suffix_index = 0;
    char *name;
    char phonebook_name[10] = {0};
    if ((char *)pbap_headers->pbap_req_info->name == NULL || pbap_headers->pbap_req_info->name->value == NULL)
    {
        return -EINVAL;
    }
    name = (char *)pbap_headers->pbap_req_info->name->value;
    index = pbap_headers->pbap_req_info->name->length -1;
    if (endwith(name, ".vcf") == 0)
    {
        return -EINVAL;
    }
    while(index >= 0)
    {
        if (name[index] == '.')
        {
            suffix_index = index;
        }
        else if (name[index] == '/')
        {
            break;
        }
        index--;
    }
    memcpy(phonebook_name, name + index + 1, suffix_index - index -1);
    phonebook_name[suffix_index - index -1] = 0;
    for (index = 0; index < child_floader_count; index++)
    {
        if (strcmp((char *)phonebook_name, child_floader_name[index]) == 0)
        {
            break;
        }
    }
    if (index == child_floader_count)
    {
        return -EINVAL;
    }
    return 0;
}

static int8_t bt_pal_pull_vcard_listing_param(PBAP_HEADERS *pbap_headers)
{
    /* Basic Name Requirements :   child_floader_name */
    uint8_t index = 0;
    char *name;
    if ((char *)pbap_headers->pbap_req_info->name == NULL || pbap_headers->pbap_req_info->name->value == NULL)
    {
        return -EINVAL;
    }
    name = (char *)pbap_headers->pbap_req_info->name->value;
    for (index = 0; index < child_floader_count; index++)
    {
        if (strcmp(name, child_floader_name[index]) == 0)
        {
            break;
        }
    }
    if (index == child_floader_count)
    {
        return -EINVAL;
    }
    return 0;

}

static int8_t bt_pal_pull_vcard_entry_param(PBAP_HEADERS *pbap_headers)
{
    /* Basic Name Requirements :  xxx.vcf or X-BT-UID:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
    char *name;
    // check name
    if ((char *)pbap_headers->pbap_req_info->name == NULL || pbap_headers->pbap_req_info->name->value == NULL)
    {
        return -EINVAL;
    }
    name = (char *)pbap_headers->pbap_req_info->name->value;
    if (endwith(name, ".vcf") == 0)
    {
        if(startwith(name, "X-BT-UID:") == 0)
        {
            return -EINVAL;
        }
    }
    return 0;
}
static API_RESULT ethermind_pbap_pse_event_callback(
    /* IN */ uint8_t event_type,
    /* IN */ uint16_t event_result,
    /* IN */ PBAP_HANDLE pbap_handle,
    /* IN */ PBAP_HEADERS *pbap_headers,
    /* IN */ uint8_t num_headers)
{
    API_RESULT retval;
    int8_t result;
    PBAP_HEADERS pbap_resp_header;
    PBAP_CONNECT_STRUCT connect_req_send;
    PBAP_HEADER_STRUCT password;
    PBAP_HEADER_STRUCT uid;
    struct net_buf *buf;
    char path_name[10]      = {0};
    uint16_t disconn_reason = 0;
    memset(&pbap_resp_header, 0, sizeof(pbap_resp_header));
    struct bt_pbap_pse *pbap_pse = bt_pabp_pse_lookup_bt_handle((uint8_t)pbap_handle);
    if (pbap_pse == NULL)
    {
        return API_SUCCESS;
    }
    if (pbap_pse->cb_flag & BT_OBEX_REQ_END)
    {
        if (event_result == BT_PBAP_CONTINUE_RSP) /* the first but not last package*/
        {
            pbap_pse->cb_flag = BT_OBEX_REQ_CONTINUE;
        }
        else /* The first and last package */
        {
            pbap_pse->cb_flag = BT_OBEX_REQ_UNSEG;
        }
    }
    else if (pbap_pse->cb_flag & BT_OBEX_REQ_CONTINUE)
    {
        if (event_result != BT_PBAP_CONTINUE_RSP) /* The last package of subsequent packages */
        {
            pbap_pse->cb_flag = BT_OBEX_REQ_END;
        }
    }
    switch (event_type)
    {
        case PBAP_PSE_CONNECT_IND:
            pbap_pse->max_pkt_len = pbap_headers->pbap_connect_info->max_recv_size +
                                    (3U + 17U); /* Subtract 20 in stack and add 20 back here. */
            pbap_pse->peer_feature  = pbap_headers->pbap_connect_info->pbap_supported_features == 0x0U ?
                                          BT_PBAP_SUPPORTED_FEATURES_V11 :
                                          pbap_headers->pbap_connect_info->pbap_supported_features;
            pbap_pse->local_feature = CONFIG_BT_PBAP_PSE_SUPPORTED_FEATURES;
            memset(&connect_req_send, 0, sizeof(connect_req_send));
            password.value            = NULL;
            password.length           = 0;
            uid.value                 = NULL;
            uid.length                = 0;
            connect_req_send.pin_info = &password;
            connect_req_send.user_id  = &uid;
            if (((PBAP_SUCCESS_RSP != event_result) && (PBAP_NOT_AUTHENTICATED != event_result) &&
                 (PBAP_UNAUTH_RSP != event_result)) ||
                (NULL == pbap_headers->pbap_connect_info))
            {
                if (PBAP_AUTHENTICATION_FAILED == event_result)
                {
                    event_result = PBAP_NOT_AUTHENTICATED;
                }
                else
                {
                    event_result = PBAP_NOT_ACCEPTABLE_RSP;
                }
            }
            if (event_result == PBAP_NOT_AUTHENTICATED)
            {
                if (pbap_pse->auth == NULL || pbap_pse->auth->pin_code == NULL)
                {
                    if (pbap_pse_cb->get_auth_info)
                    {
                        pbap_pse_cb->get_auth_info(pbap_pse, pbap_pse->auth, &pbap_pse->local_auth);
                    }
                }
                if (pbap_pse->auth != NULL && pbap_pse->auth->pin_code != NULL &&
                    strlen(pbap_pse->auth->pin_code) > 0)
                {
                    password.value  = (UCHAR *)pbap_pse->auth->pin_code;
                    password.length = strlen(pbap_pse->auth->pin_code);
                    uid.value       = (UCHAR *)pbap_pse->auth->user_id;
                    uid.length      = (uint16_t)strlen(pbap_pse->auth->user_id);
                    event_result    = PBAP_SUCCESS_RSP;
                    if (pbap_pse->local_auth)
                    {
                        event_result         = PBAP_UNAUTH_RSP;
                        pbap_pse->local_auth = 1;
                    }
                }
                else
                {
                    event_result = PBAP_SUCCESS_RSP;
                }
            }
            else if (PBAP_UNAUTH_RSP == event_result)
            {
                if (pbap_pse->auth == NULL || pbap_pse->auth->pin_code == NULL)
                {
                    if (pbap_pse_cb->get_auth_info)
                    {
                        pbap_pse_cb->get_auth_info(pbap_pse, pbap_pse->auth, &pbap_pse->local_auth);
                    }
                }
                if (pbap_pse->auth != NULL && pbap_pse->auth->pin_code != NULL &&
                    strlen(pbap_pse->auth->pin_code) > 0)
                {
                    password.value            = (UCHAR *)pbap_pse->auth->pin_code;
                    password.length           = strlen(pbap_pse->auth->pin_code);
                    uid.value                 = (UCHAR *)pbap_pse->auth->user_id;
                    uid.length                = (uint16_t)strlen(pbap_pse->auth->user_id);
                    event_result              = PBAP_SUCCESS_RSP;
                    connect_req_send.pin_info = &password;
                }
            }
            else
            {
                event_result = PBAP_SUCCESS_RSP;
            }
            connect_req_send.max_recv_size     = pbap_pse->max_pkt_len;
            pbap_resp_header.pbap_connect_info = &connect_req_send;
            retval = BT_pbap_pse_send_response(pbap_handle, PBAP_PSE_CONNECT_IND, event_result, &pbap_resp_header);
            if (retval != API_SUCCESS)
            {
                pbap_pse_free_instance(pbap_pse);
            }
            else
            {
                if (event_result == PBAP_SUCCESS_RSP && pbap_pse_cb->connected != NULL && pbap_pse != NULL)
                {
                    pbap_pse_cb->connected(pbap_pse);
                }
            }
            break;

        case PBAP_PSE_DISCONNECT_IND:
            /* Transport close has been sent in the underlying layer so don't need call BT_pbap_pse_trans_disconnect. */
            /* if (pbap_pse != NULL)
            {
                BT_pbap_pse_trans_disconnect(pbap_pse->handle);
            } */
            break;

        case PBAP_PSE_ABORT_IND:
            if (pbap_pse_cb != NULL && pbap_pse_cb->abort != NULL && pbap_pse != NULL)
            {
                pbap_pse_cb->abort(pbap_pse, event_result);
            }
            break;

        case PBAP_PSE_GET_PHONEBOOK_IND:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL)
            {
                break;
            }
            header_from_stack_to_buf(buf, pbap_headers->pbap_req_info);
            if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value)
            {
                result = bt_pal_pull_phonebook_param(pbap_headers);
                if(result < 0)
                {
                    BT_pbap_pse_send_response(pbap_pse->pbap_handle, PBAP_PSE_GET_PHONEBOOK_IND, BT_PBAP_NOT_ACCEPTABLE_RSP, &pbap_resp_header);
                    net_buf_unref(buf);
                    break;
                }
            }
            if (pbap_pse_cb != NULL && pbap_pse_cb->pull_phonebook != NULL && pbap_pse != NULL)
            {
                if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value)
                {
                    pbap_pse_cb->pull_phonebook(pbap_pse, buf, (char *)pbap_headers->pbap_req_info->name->value,
                                                pbap_pse->cb_flag);
                }
                else
                {
                    pbap_pse_cb->pull_phonebook(pbap_pse, buf, NULL, pbap_pse->cb_flag);
                }
            }
            break;

        case PBAP_PSE_GET_VCARD_LIST_IND:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL)
            {
                break;
            }
            header_from_stack_to_buf(buf, pbap_headers->pbap_req_info);
            if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value && pbap_headers->pbap_req_info->name->length >= 2)
            {
                result = bt_pal_pull_vcard_listing_param(pbap_headers);
                if(result < 0)
                {
                    BT_pbap_pse_send_response(pbap_pse->pbap_handle, PBAP_PSE_GET_VCARD_LIST_IND, BT_PBAP_NOT_FOUND_RSP, &pbap_resp_header);
                    net_buf_unref(buf);
                    break;
                }
            }

            if (pbap_pse_cb != NULL && pbap_pse_cb->pull_vcard_listing != NULL && pbap_pse != NULL)
            {
                if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value && pbap_headers->pbap_req_info->name->length >= 2)
                {
                    pbap_pse_cb->pull_vcard_listing(pbap_pse, buf, (char *)pbap_headers->pbap_req_info->name->value,
                                                    pbap_pse->cb_flag);
                }
                else
                {
                    pbap_pse_cb->pull_vcard_listing(pbap_pse, buf, NULL, pbap_pse->cb_flag);
                }
            }
            break;

        case PBAP_PSE_GET_VCARD_IND:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL)
            {
                break;
            }
            header_from_stack_to_buf(buf, pbap_headers->pbap_req_info);
            if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value)
            {
                result = bt_pal_pull_vcard_entry_param(pbap_headers);
                if(result < 0)
                {
                    BT_pbap_pse_send_response(pbap_pse->pbap_handle, PBAP_PSE_GET_VCARD_IND, BT_PBAP_NOT_FOUND_RSP, &pbap_resp_header);
                    net_buf_unref(buf);
                    break;
                }
            }
            if (pbap_pse_cb != NULL && pbap_pse_cb->pull_vcard_entry != NULL && pbap_pse != NULL)
            {
                if (pbap_headers->pbap_req_info->name && pbap_headers->pbap_req_info->name->value)
                {
                    pbap_pse_cb->pull_vcard_entry(pbap_pse, buf, (char *)pbap_headers->pbap_req_info->name->value,
                                                  pbap_pse->cb_flag);
                }
                else
                {
                    pbap_pse_cb->pull_vcard_entry(pbap_pse, buf, NULL, pbap_pse->cb_flag);
                }
            }
            break;

        case PBAP_PSE_SET_PHONEBOOK_IND:
            switch (pbap_headers->pbap_req_info->setpath_flag)
            {
                case 0x02U: /* root */
                    if ((NULL == pbap_headers->pbap_req_info->name) ||
                        (0x00U == pbap_headers->pbap_req_info->name->length))
                    {
                        strcat(path_name, "/");
                    }
                    else /* child */
                    {
                        strcat(path_name, "./");
                        strncat(path_name, (char *)pbap_headers->pbap_req_info->name->value,
                                pbap_headers->pbap_req_info->name->length);
                    }
                    break;

                case 0x03U: /* parents*/
                    strcat(path_name, "..");
                    break;

                default:
                    break;
            }
            if (pbap_pse_cb != NULL && pbap_pse_cb->set_phonebook_path != NULL && pbap_pse != NULL)
            {
                pbap_pse_cb->set_phonebook_path(pbap_pse, path_name);
            }
            break;

        case PBAP_PSE_TRANSPORT_CLOSE_CFM:
        case PBAP_PSE_TRANSPORT_CLOSE_IND:
            pbap_pse_free_instance(pbap_pse);
            if (pbap_pse_cb != NULL && pbap_pse_cb->disconnected != NULL && pbap_pse != NULL)
            {
                if (disconn_reason == BT_PBAP_FORBIDDEN_RSP)
                {
                    event_result = BT_PBAP_FORBIDDEN_RSP;
                }
                pbap_pse_cb->disconnected(pbap_pse, event_result);
            }
            break;

        default:
            break;
    }
    return API_SUCCESS;
}

static int bt_pal_pbap_pse_init(void)
{
    API_RESULT retval;
    retval = BT_pbap_pse_init();
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Initialization Failed. Reason 0x%04X \n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successfully Initialized PBAP PCE\n");
    }

    if (NULL == s_PbapPseLock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_PbapPseLockMutex))
        {
            s_PbapPseLock = (osa_mutex_handle_t)s_PbapPseLockMutex;
        }
        else
        {
            return -EIO;
        }
    }

    for (uint8_t index = 0; index < PBAP_PSE_MAX_ENTITY; ++index)
    {
        s_PbapPseInstances[index].acl_conn = NULL;
    }
    for (uint8_t index = 0; index < PBAP_PSE_MAX_ENTITY; ++index)
    {
        s_PbapPseInstances[index].flag    = BT_OBEX_REQ_END;
        s_PbapPseInstances[index].cb_flag = BT_OBEX_REQ_END;
        s_PbapPseInstances[index].auth    = &auth_info[index];
        retval = BT_pbap_pse_start_instance(&s_PbapPseInstances[index].pbap_handle, ethermind_pbap_pse_event_callback);
        if (API_SUCCESS != retval)
        {
            LOG_ERR("Start instance fail 0x%04X\r\n", retval);
        }
    }
    return retval;
}

int bt_pbap_pse_register(struct bt_pbap_pse_cb *cb)
{
    int res;
    if (!cb)
    {
        return -EINVAL;
    }

    pbap_pse_cb = cb;

    res = bt_pal_pbap_pse_init();
    if (res != 0)
    {
        LOG_ERR("pbap pse init failed\r\n", retval);
    }

    return res;
}

int bt_pbap_pse_disconnect(struct bt_pbap_pse *pbap_pse)
{
    if (pbap_pse == NULL)
    {
        return -EINVAL;
    }
    if (BT_pbap_pse_trans_disconnect(pbap_pse->pbap_handle) != API_SUCCESS)
    {
        return -EIO;
    }
    return 0;
}

int bt_pbap_pse_get_max_pkt_len(struct bt_pbap_pse *pbap_pse, uint16_t *max_pkt_len)
{
    if (pbap_pse == NULL)
    {
        return -EINVAL;
    }
    *max_pkt_len = pbap_pse->max_pkt_len;

    return 0;
}

int bt_pbap_pse_get_peer_supported_features(struct bt_pbap_pse *pbap_pse, uint32_t *supported_features)
{
    if (pbap_pse == NULL)
    {
        return -EINVAL;
    }
    *supported_features = pbap_pse->peer_feature;

    return 0;
}

#endif /* CONFIG_BT_PBAP_PSE */
