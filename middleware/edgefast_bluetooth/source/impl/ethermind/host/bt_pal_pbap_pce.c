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
#include <bluetooth/pbap_pce.h>
#include "bt_pal_conn_internal.h"
#include "bt_pal_pbap_internal.h"
#include "BT_features.h"
#include "BT_config.h"
#include "BT_pbap_api.h"
#include "db_gen.h"
#include "eBT_os.h"
#include "bt_pal_l2cap_internal.h"

#if (defined(CONFIG_BT_PBAP_PCE) && ((CONFIG_BT_PBAP_PCE) > 0U))

#define LOG_ENABLE      IS_ENABLED(CONFIG_BT_DEBUG_PBAP_PCE)
#define LOG_MODULE_NAME bt_pbap_pce
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

/* Blocking code prepared for zephyr */
#define CONFIG_BT_ZEPHYR_BUF 0U
/* PBAP appl parameters count*/
#define BT_PBAP_APPL_PARAM_HDR_COUNT 16U

NET_BUF_POOL_FIXED_DEFINE(pbap_pool, PBAP_PCE_MAX_ENTITY, CONFIG_BT_PBAP_PCE_MAX_PKT_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
/** @brief pbap request command. */
typedef enum _pbap_pce_command
{
    BT_PBAP_PCE_CONNECT            = 0x0U,
    BT_PBAP_PCE_PULL_PHONEBOOK     = 0x1U,
    BT_PBAP_PCE_SET_PHONEBOOK_PATH = 0x2U,
    BT_PBAP_PCE_PULL_VCARD_LISTING = 0x3U,
    BT_PBAP_PCE_PULL_VCARD_ENTRY   = 0x4U,
    BT_PBAP_PCE_DISCONNECT         = 0x5u,
} pbap_pce_command;

static struct bt_pbap_pce_cb *pbap_pce_cb;
static struct bt_pbap_auth auth_info[PBAP_PCE_MAX_ENTITY];

/* Target ID of PBAP */
const UCHAR pbap_target_id[] = {0x79U, 0x61U, 0x35U, 0xf0U, 0xf0U, 0xc5U, 0x11U, 0xd8U,
                                0x09U, 0x66U, 0x08U, 0x00U, 0x20U, 0x0cU, 0x9aU, 0x66U};
/** @brief PBAP PCE structure */
struct bt_pbap_pce
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
};
static OSA_MUTEX_HANDLE_DEFINE(s_PbapPceLockMutex);
static osa_mutex_handle_t s_PbapPceLock;
static struct bt_pbap_pce s_PbapPceInstances[PBAP_PCE_MAX_ENTITY];
#define EDGEFAST_PBAP_PCE_LOCK   OSA_MutexLock(s_PbapPceLock, osaWaitForever_c)
#define EDGEFAST_PBAP_PCE_UNLOCK OSA_MutexUnlock(s_PbapPceLock)

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
static struct bt_pbap_pce *pbap_pce_get_instance(struct bt_conn *conn)
{
    EDGEFAST_PBAP_PCE_LOCK;
    for (uint8_t index = 0; index < PBAP_PCE_MAX_ENTITY; ++index)
    {
        if (s_PbapPceInstances[index].acl_conn == NULL)
        {
            memset(&s_PbapPceInstances[index], 0U, sizeof(s_PbapPceInstances[index]));
            s_PbapPceInstances[index].acl_conn = conn;
            s_PbapPceInstances[index].auth     = &auth_info[index];
            EDGEFAST_PBAP_PCE_UNLOCK;
            return &s_PbapPceInstances[index];
        }
    }
    EDGEFAST_PBAP_PCE_UNLOCK;
    return NULL;
}

static void pbap_pce_free_instance(struct bt_pbap_pce *pbap_pce)
{
    if (NULL != pbap_pce)
    {
        EDGEFAST_PBAP_PCE_LOCK;
        pbap_pce->acl_conn = NULL;
        EDGEFAST_PBAP_PCE_UNLOCK;
    }
}

static struct bt_pbap_pce *bt_pabp_pce_lookup_bt_handle(uint8_t handle)
{
    EDGEFAST_PBAP_PCE_LOCK;
    for (uint8_t index = 0; index < PBAP_PCE_MAX_ENTITY; ++index)
    {
        if (handle == s_PbapPceInstances[index].pbap_handle)
        {
            EDGEFAST_PBAP_PCE_UNLOCK;
            return &s_PbapPceInstances[index];
        }
    }
    EDGEFAST_PBAP_PCE_UNLOCK;
    return NULL;
}

static struct bt_pbap_pce *bt_pbap_pce_lookup_bt_conn(struct bt_conn *conn)
{
    EDGEFAST_PBAP_PCE_LOCK;
    for (uint8_t index = 0; index < PBAP_PCE_MAX_ENTITY; ++index)
    {
        if (conn == s_PbapPceInstances[index].acl_conn)
        {
            EDGEFAST_PBAP_PCE_UNLOCK;
            return &s_PbapPceInstances[index];
        }
    }
    EDGEFAST_PBAP_PCE_UNLOCK;
    return NULL;
}

static int bt_pal_pbap_pce_init(void)
{
    API_RESULT retval;
    /* PBAP PCE SDP record handle */
    static uint32_t pbap_pce_record_handle;
    /* Get PBAP_PCE SDP record handle */
    BT_dbase_get_record_handle(DB_RECORD_PBAP_PCE, 0, &pbap_pce_record_handle);

    retval = BT_pbap_pce_init();
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Initialization Failed. Reason 0x%04X \n", retval);
    }
    else
    {
        BT_dbase_activate_record(pbap_pce_record_handle);
        LOG_INF("Successfully Initialized PBAP PCE\n");
    }

    if (NULL == s_PbapPceLock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_PbapPceLockMutex))
        {
            s_PbapPceLock = (osa_mutex_handle_t)s_PbapPceLockMutex;
        }
        else
        {
            return -EIO;
        }
    }

    for (uint8_t index = 0; index < PBAP_PCE_MAX_ENTITY; ++index)
    {
        s_PbapPceInstances[index].acl_conn = NULL;
    }
    return retval;
}

int bt_pbap_pce_register(struct bt_pbap_pce_cb *cb)
{
    int res;
    if (!cb)
    {
        return -EINVAL;
    }

    pbap_pce_cb = cb;

    res = bt_pal_pbap_pce_init();
    if (res != 0)
    {
        LOG_ERR("pbap pce init failed\r\n", retval);
    }

    return res;
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

int bt_pbap_pce_disconnect(struct bt_pbap_pce *pbap_pce)
{
    API_RESULT retval = 0;
    if (pbap_pce == NULL)
    {
        return -EAGAIN;
    }
    if (NULL == pbap_pce)
    {
        return -EAGAIN;
    }

    retval = BT_pbap_pce_disconnect(pbap_pce->pbap_handle);

    if (API_SUCCESS != retval)
    {
        LOG_ERR("Stop Failed. Reason 0x%04X \n", retval);
    }
    return retval;
}

static int bt_pal_pbap_pce_stop_instance(struct bt_pbap_pce *pbap_pce)
{
    API_RESULT retval = 0;

    if (pbap_pce == NULL)
    {
        return -EINVAL;
    }

    retval = BT_pbap_pce_stop_instance(pbap_pce->pbap_handle);

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

static void pbap_ascii_to_unicode(uint8_t *des, const uint8_t *src)
{
    uint32_t i = 0;

    if ((src == NULL) || (des == NULL))
    {
        return;
    }
    while (src[i] != 0x00U)
    {
        des[(i << 1U)]      = src[i];
        des[(i << 1U) + 1U] = 0x00U;
        i++;
    }

    des[(i << 1U)]      = 0x00U;
    des[(i << 1U) + 1U] = 0x00U; /* terminate with 0x00, 0x00 */
}

static void pbap_unicode_to_ascii(uint8_t *des, const uint8_t *src)
{
    uint32_t i = 0;

    if ((src == NULL) || (des == NULL))
    {
        return;
    }

    while ((src[i] != 0U) || (src[i + 1U] != 0U))
    {
        des[i >> 1U] = src[i];
        i += 2U;
    }
    des[i >> 1U] = 0x00U; /* terminate with  0x00 */
}

void bt_pbap_add_nonce(struct net_buf *buf, uint8_t tag_id, char *value)
{
    char nonce[16] = {0};
    memcpy(nonce, value, 16);
    bt_obex_add_app_param(buf, tag_id, (uint8_t *)nonce, 16);
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
            PBAP_SET_APPL_PARAM_FLAG(appl_params->appl_param_flag, (uint16_t)(1U << tag.id % BT_PBAP_APPL_PARAM_HDR_COUNT));
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

static void bt_pbap_add_pbap_hdr(struct bt_pbap_pce *pbap_pce,
                                 struct net_buf *buf,
                                 char *name,
                                 bool wait,
                                 uint16_t appl_len,
                                 pbap_pce_command command,
                                 enum bt_obex_req_flags flag)
{
    uint8_t opcode;
    uint16_t packet_length = appl_len;
    uint8_t hi;
    uint8_t str_len;
    struct bt_obex_hdr_u8 bt_srm;
    struct bt_obex_hdr_u8 bt_srmp;
    struct bt_obex_hdr_u32 bt_conn_id;

    if (NULL == name)
    {
        return;
    }
    if (flag & BT_OBEX_REQ_START)
    {
        /* name*/
        hi      = OBEX_HDR_NAME;
        str_len = (uint16_t)strlen(name);
        net_buf_push_mem(buf, name, str_len);
        net_buf_push_be16(buf, str_len + 3);
        net_buf_push_u8(buf, hi);
        packet_length += str_len + 3;

        /* type */
        hi = OBEX_HDR_TYPE;
        switch (command)
        {
            case BT_PBAP_PCE_PULL_PHONEBOOK:
                str_len = (uint16_t)strlen(BT_PBAP_TYPE_HDR_PHONEBOOK);
                net_buf_push_mem(buf, BT_PBAP_TYPE_HDR_PHONEBOOK, str_len);
                net_buf_push_be16(buf, str_len + 3);
                break;
            case BT_PBAP_PCE_PULL_VCARD_LISTING:
                str_len = (uint16_t)strlen(BT_PBAP_TYPE_HDR_VCARD_LIST);
                net_buf_push_mem(buf, BT_PBAP_TYPE_HDR_VCARD_LIST, str_len);
                net_buf_push_be16(buf, str_len + 3);
                break;
            case BT_PBAP_PCE_PULL_VCARD_ENTRY:
                str_len = (uint16_t)strlen(BT_PBAP_TYPE_HDR_VCARD);
                net_buf_push_mem(buf, BT_PBAP_TYPE_HDR_VCARD, str_len);
                net_buf_push_be16(buf, str_len + 3);
                break;
            default:
                break;
        }
        net_buf_push_u8(buf, hi);
        packet_length += str_len + 3;
    }
    /* SRM && SRMP */
    if (pbap_pce->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (flag & BT_OBEX_REQ_START)
        {
            bt_srm.hi = OBEX_HDR_SRM;
            bt_srm.hv = OBEX_SRM_ENABLE;
            packet_length += sizeof(struct bt_obex_hdr_u8);
            net_buf_push_mem(buf, &bt_srm, sizeof(bt_srm));
        }
        if (wait)
        {
            bt_srmp.hi = OBEX_HDR_SRMP;
            bt_srm.hv  = OBEX_SRMP_WAIT;
            packet_length += sizeof(bt_srmp);
            net_buf_push_mem(buf, &bt_srmp, sizeof(bt_srmp));
        }
    }

    /* connection id */
    bt_conn_id.hi = OBEX_HDR_CONNECTION_ID;
    bt_conn_id.hv = 0;
    packet_length += sizeof(bt_conn_id);
    net_buf_push_mem(buf, &bt_conn_id, sizeof(bt_conn_id));

    /* packet_length */
    packet_length += 3;
    net_buf_push_be16(buf, packet_length);

    /* Opcode */
    opcode = OBEX_GET_FINAL_OP;
    net_buf_push_u8(buf, opcode);
}

static int8_t bt_pal_pull_phonebook_param(char *name)
{
    /* Basic Name Requirements :  (xxx/) child_floader_name.vcf  */
    int8_t index = 0;
    uint8_t suffix_index = 0;
    char phonebook_name[10] = {0};
    if (name == NULL)
    {
        return -EINVAL;
    }
    index = strlen(name) -1;
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

int bt_pbap_pce_pull_phonebook(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag)
{
    int retval = 0;
    PBAP_HEADER_STRUCT name_header;
    PBAP_APPL_PARAMS appl_params;
    PBAP_REQUEST_STRUCT req_info;
    PBAP_HEADERS pbap_header;
    memset(&req_info, 0, sizeof(req_info));
    if (NULL == pbap_pce || NULL == buf)
    {
        return -EAGAIN;
    }

    /* Forming the stack parameter */
    name_header.value  = NULL;
    name_header.length = 0;
    req_info.name      = &name_header;
    if ((flag & BT_OBEX_REQ_START))
    {
        if (bt_pal_pull_phonebook_param(name) < 0)
        {
            return -EINVAL;
        }
        name_header.value  = (UCHAR *)name;
        name_header.length = (uint16_t)strlen(name);
    }

    pbap_pce->lcl_wait = 0;
    if (wait)
    {
        req_info.wait      = true;
        pbap_pce->lcl_wait = wait;
    }

    retval = bt_pbap_form_stack_param(buf, &appl_params);
    if (retval < 0)
    {
        PRINTF("INVAILD PARAMS\r\n");
        return -EINVAL;
    }

    req_info.appl_params = &appl_params;

#if CONFIG_BT_ZEPHYR_BUF
    /* Fill net_buf */
    char unicode_name[BT_FUNC_PULL_PHONEBOOK_NAME_MAX_LEN];
    pbap_ascii_to_unicode((uint8_t *)unicode_name, (uint8_t *)name);
    bt_pbap_add_pbap_hdr(pbap_pce, buf, unicode_name, wait, retval, BT_PBAP_PCE_PULL_PHONEBOOK, flag);
#endif

    if (flag & BT_OBEX_REQ_START)
    {
        retval = BT_pbap_pce_get_phonebook(pbap_pce->pbap_handle, &req_info);
    }
    else
    {
        memset(&pbap_header, 0, sizeof(pbap_header));
        pbap_header.pbap_req_info = &req_info;
        retval =BT_pbap_pce_send_request(pbap_pce->pbap_handle, PBAP_PCE_GET_PHONEBOOK_CFM, PBAP_SUCCESS_RSP, &pbap_header);
    }

    if (API_SUCCESS != retval)
    {
        LOG_ERR("Failed. Reason 0x%04X\r\n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successful\r\n");
    }
    net_buf_unref(buf);

    return 0;
}

static int bt_pbap_pce_set_book_path_stack_param(PBAP_REQUEST_STRUCT *req_info, char *name)
{
    if (name == NULL || req_info == NULL)
    {
        return -EINVAL;
    }
    if (strcmp(name, "/") == 0)
    {
        req_info->setpath_flag = PBAP_SET_ROOT_FOLDER;
        req_info->name->value  = NULL;
        req_info->name->length = 0;
        return 0;
    }
    else if (strncmp(name, "..", 2U) == 0)
    {
        req_info->setpath_flag = PBAP_SET_PARENT_FOLDER;
        req_info->name->value  = NULL;
        req_info->name->length = 0;
        return 0;
    }
    else if (strncmp(name, "./", 2U) == 0)
    {
        req_info->setpath_flag = PBAP_SET_CHILD_FOLDER;
        if (name[2] == '\0')
        {
            return -EINVAL;
        }
        else
        {
            req_info->name->value  = (uint8_t *)&name[2];
            req_info->name->length = strlen(&name[2]);
            return 0;
        }
    }
    else if (name[0] != '\0')
    {
        req_info->setpath_flag = PBAP_SET_CHILD_FOLDER;
        req_info->name->value  = (uint8_t *)name;
        req_info->name->length = strlen(name);
        return 0;
    }
    return -EINVAL;
}

static void bt_pbap_add_pbap_setPhonebookPath_hdr(struct net_buf *buf, char *name)
{
    uint8_t hi;
    struct bt_pbap_set_path_hdr bt_set_path_hdr;
    uint16_t length;
    uint16_t packet_len;
    length = (uint16_t)strlen(name);
    bt_set_path_hdr.opcode = OBEX_SETPATH_OP;
    bt_set_path_hdr.packet_length = 3;
    /*FLAG*/
    if (length == 1 && strcmp(name, "/") == 0)
    {
        bt_set_path_hdr.Flags = 0x2U;
    }
    else if (length == 2 && strcmp(name, "..") == 0)
    {
        bt_set_path_hdr.Flags = 0x3U;
    }
    else
    {
        if (name != NULL && name[0] == '.' && name[1] == '/')
        {
            bt_set_path_hdr.Flags = 0x2U;
        }
    }
    bt_set_path_hdr.packet_length += 1;
    /*constant*/
    bt_set_path_hdr.constant = 0;
    bt_set_path_hdr.packet_length += 1;
    /*connextion_id*/
    bt_set_path_hdr.conn_id.hi = OBEX_HDR_CONNECTION_ID;
    bt_set_path_hdr.conn_id.hv = 0;
    bt_set_path_hdr.packet_length += sizeof(struct bt_obex_hdr_u32);

    if (bt_set_path_hdr.Flags == 0x2U)
    {
        /* name */
        hi     = OBEX_HDR_NAME;
        length = (uint16_t)strlen(name) + 3;
        net_buf_push_mem(buf, name, length - 3);
        net_buf_push_be16(buf, length);
        net_buf_push_u8(buf, hi);
    }
    packet_len = bt_set_path_hdr.packet_length;
    bt_set_path_hdr.packet_length = sys_cpu_to_be16(packet_len);
    net_buf_push_mem(buf, &bt_set_path_hdr, sizeof(bt_set_path_hdr));
}

/*
   root :    /
   parent    ..
   child     ./name or name
   For multilevel jumps, need to do it on a level-by-level basis
*/
int bt_pbap_pce_set_phonebook_path(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name)
{
    int retval = 0;
    PBAP_HEADER_STRUCT name_header;
    PBAP_REQUEST_STRUCT req_info;
    if (pbap_pce == NULL || buf == NULL)
    {
        return -EAGAIN;
    }
    memset(&req_info, 0, sizeof(PBAP_REQUEST_STRUCT));
    req_info.name = &name_header;

    retval = bt_pbap_pce_set_book_path_stack_param(&req_info, name);
    if (retval != 0)
    {
        return retval;
    }

#if CONFIG_BT_ZEPHYR_BUF
    bt_pbap_add_pbap_setPhonebookPath_hdr(buf, name);
#endif

    retval = BT_pbap_pce_set_phonebook(pbap_pce->pbap_handle, &req_info);
    if (API_SUCCESS != retval)
    {
        LOG_ERR(" Failed. Reason 0x%04X \n", retval);
        return retval;
    }
    else
    {
        LOG_INF(" Successful. Wait for SETPATH Response... \n");
    }
    net_buf_unref(buf);
    return 0;
}

static int8_t bt_pal_pull_vcard_listing_param(char *name)
{
    uint8_t index = 0;
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
int bt_pbap_pce_pull_vcard_listing(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag)
{
    int retval = 0;
    PBAP_HEADER_STRUCT name_header;
    PBAP_APPL_PARAMS appl_params;
    PBAP_REQUEST_STRUCT req_info;
    PBAP_HEADERS pbap_header;
    if (NULL == pbap_pce || NULL == buf)
    {
        return -EAGAIN;
    }
    /* Forming the stack parameter */
    name_header.value  = NULL;
    name_header.length = 0;
    req_info.name      = &name_header;
    if ((flag & BT_OBEX_REQ_START))
    {
        if (name != NULL && strlen(name) > 0)
        {
            if(bt_pal_pull_vcard_listing_param(name) < 0)
            {
                return -EAGAIN;
            }
            name_header.value  = (UCHAR *)name;
            name_header.length = (uint16_t)strlen(name);
        }
    }

    pbap_pce->lcl_wait = 0;
    if (wait)
    {
        req_info.wait      = true;
        pbap_pce->lcl_wait = wait;
    }

    retval = bt_pbap_form_stack_param(buf, &appl_params);
    if (retval < 0)
    {
        LOG_ERR("INVAILD PARAMS\r\n");
        return -EINVAL;
    }
    req_info.appl_params = &appl_params;

#if CONFIG_BT_ZEPHYR_BUF
    /* Fill net_buf */
    bt_pbap_add_pbap_hdr(pbap_pce, buf, name, wait, retval, BT_PBAP_PCE_PULL_VCARD_LISTING, flag);
#endif

    if (flag & BT_OBEX_REQ_START)
    {
        retval = BT_pbap_pce_get_vcard_list(pbap_pce->pbap_handle, &req_info);
    }
    else
    {
        memset(&pbap_header, 0, sizeof(pbap_header));
        pbap_header.pbap_req_info = &req_info;
        retval = BT_pbap_pce_send_request(pbap_pce->pbap_handle, PBAP_PCE_GET_VCARD_LIST_CFM, BT_PBAP_SUCCESS_RSP, &pbap_header);
    }
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Failed. Reason 0x%04X\r\n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successful\r\n");
    }

    net_buf_unref(buf);

    return 0;
}

static int8_t bt_pal_pull_vcard_entry_param(char *name)
{
    /* Basic Name Requirements :  xxx.vcf or X-BT-UID:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   */
    if (name == NULL)
    {
        return -EINVAL;
    }
    if (endwith(name, ".vcf") == 0)
    {
        if(startwith(name, "X-BT-UID:") == 0)
        {
            return -EINVAL;
        }
    }
    return 0;
}

int bt_pbap_pce_pull_vcard_entry(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag)
{
    int retval = 0;
    PBAP_HEADER_STRUCT name_header;
    PBAP_APPL_PARAMS appl_params;
    PBAP_REQUEST_STRUCT req_info;
    PBAP_HEADERS pbap_header;
    if (NULL == pbap_pce || NULL == buf)
    {
        return -EAGAIN;
    }
    /* Forming the stack parameter */
    name_header.value  = NULL;
    name_header.length = 0;
    req_info.name      = &name_header;
    if ((flag & BT_OBEX_REQ_START))
    {
        if (name == NULL || bt_pal_pull_vcard_entry_param(name) < 0)
        {
            return -EINVAL;
        }
        name_header.value  = (UCHAR *)name;
        name_header.length = (uint16_t)strlen(name);
    }

    pbap_pce->lcl_wait = 0;
    if (wait)
    {
        req_info.wait      = true;
        pbap_pce->lcl_wait = wait;
    }

    retval = bt_pbap_form_stack_param(buf, &appl_params);
    if (retval < 0)
    {
        LOG_ERR("INVAILD PARAMS\r\n");
        return -EINVAL;
    }
    req_info.appl_params = &appl_params;

#if CONFIG_BT_ZEPHYR_BUF
    /* Fill net_buf */
    bt_pbap_add_pbap_hdr(pbap_pce, buf, name, wait, retval, BT_PBAP_PCE_PULL_VCARD_ENTRY, flag);
#endif

    if (flag & BT_OBEX_REQ_START)
    {
        retval = BT_pbap_pce_get_vcard(pbap_pce->pbap_handle, &req_info);
    }
    else
    {
        memset(&pbap_header, 0, sizeof(pbap_header));
        pbap_header.pbap_req_info = &req_info;
        retval = BT_pbap_pce_send_request(pbap_pce->pbap_handle, PBAP_PCE_GET_VCARD_CFM, BT_PBAP_SUCCESS_RSP, &pbap_header);
    }
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Failed. Reason 0x%04X\r\n", retval);
        return retval;
    }
    else
    {
        LOG_INF("Successful\r\n");
    }

    net_buf_unref(buf);

    return 0;
}

int bt_pbap_pce_abort(struct bt_pbap_pce *pbap_pce)
{
    API_RESULT retval;

    if (pbap_pce == NULL)
    {
        return -EINVAL;
    }

    retval = BT_pbap_pce_abort(pbap_pce->pbap_handle);
    if (retval == API_SUCCESS)
    {
        return 0;
    }
    else if (retval == API_FAILURE)
    {
        return -EINPROGRESS;
    }
    else
    {
        return -EIO;
    }
}
static void appl_params_from_stack_to_buf(struct net_buf *buf, PBAP_APPL_PARAMS *appl_param)
{
    if (appl_param == NULL || buf == NULL)
    {
        return;
    }
    for (uint8_t index = 0; index < BT_PBAP_APPL_PARAM_HDR_COUNT; ++index)
    {
        if (PBAP_GET_APPL_PARAM_FLAG(appl_param->appl_param_flag,
                                     (uint16_t)1U << index % BT_PBAP_APPL_PARAM_HDR_COUNT) != 0)
        {
            bt_pabp_set_appl_params_hdr_value(index + 1, appl_param, buf);
        }
    }
}
static API_RESULT ethermind_pbap_pce_event_callback(
    /* IN */ uint8_t event_type,
    /* IN */ uint16_t event_result,
    /* IN */ PBAP_HANDLE pbap_handle,
    /* IN */ PBAP_HEADERS *pbap_headers,
    /* IN */ uint8_t num_headers)
{
    API_RESULT retval;
    PBAP_HEADERS pbap_req_header;
    PBAP_CONNECT_STRUCT connect_req_send;
    PBAP_HEADER_STRUCT password;
    PBAP_HEADER_STRUCT uid;
    struct net_buf *buf;
    PBAP_APPL_PARAMS *appl_param;
    uint16_t disconn_reason = 0;

    struct bt_pbap_pce *pbap_pce = bt_pabp_pce_lookup_bt_handle((uint8_t)pbap_handle);
    switch (event_type)
    {
        case PBAP_PCE_CONNECT_CFM:
            if (pbap_pce == NULL)
            {
                break;
            }
#if CONFIG_BT_ZEPHYR_BUF
            if (bt_obex_get_hdr(BUF, OBEX_HDR_AUTH_RESPONSE, VALUE, LENGTH) == 0)
            {
                if (pbap_pce->auth != NULL && pbap_pce->auth->password != NULL)
                {
                    buf = bt_conn_create_pdu(NULL, BT_L2CAP_BUF_SIZE(2U));
                    if (!buf)
                    {
                        return -EAGAIN;
                    }
                    bt_pbap_add_connect_hdr(buf, password, peer_feature);
                }
                else
                {
                    bt_pal_pbap_pce_stop_instance(pbap_pce);
                    pbap_pce_free_instance(pbap_pce);
                }
            }
#endif
            if (BT_PBAP_UNAUTH_RSP != event_result && PBAP_SUCCESS_RSP != event_result)
            {
                /* Transport close has been sent in the underlying layer so don't need call
                 * BT_pbap_pce_trans_disconnect. */
                disconn_reason = BT_PBAP_FORBIDDEN_RSP;
                break;
            }

            if (BT_PBAP_UNAUTH_RSP == event_result)
            {
                if (pbap_pce->auth == NULL || pbap_pce->auth->pin_code == NULL)
                {
                    LOG_INF("pse want to authicate pce, please offer user_id and password if exit\r\n \n");
                    if (pbap_pce_cb->get_auth_info)
                    {
                        pbap_pce_cb->get_auth_info(pbap_pce, pbap_pce->auth);
                    }
                }
                if (pbap_pce->auth != NULL && pbap_pce->auth->pin_code != NULL &&
                    strlen(pbap_pce->auth->pin_code) > 0)
                {
                    memset(&pbap_req_header, 0, sizeof(PBAP_HEADERS));
                    memset(&connect_req_send, 0, sizeof(PBAP_HEADERS));
                    password.value                    = (UCHAR *)pbap_pce->auth->pin_code;
                    password.length                   = (uint16_t)strlen(pbap_pce->auth->pin_code);
                    uid.value                         = (UCHAR *)pbap_pce->auth->user_id;
                    uid.length                        = (uint16_t)strlen(pbap_pce->auth->user_id);
                    connect_req_send.pin_info         = &password;
                    connect_req_send.user_id          = &uid;
                    connect_req_send.auth_flag        = 0U;
                    pbap_req_header.pbap_connect_info = &connect_req_send;
                    retval = BT_pbap_pce_send_request(pbap_pce->pbap_handle, PBAP_PCE_CONNECT_CFM, PBAP_UNAUTH_RSP,
                                                      &pbap_req_header);
                    if (API_SUCCESS != retval)
                    {
                        LOG_ERR(" Failed to send the Connect Request \n");
                    }
                    else
                    {
                        LOG_INF(" Succuessfully sent the Connect Request \n");
                    }
                    break;
                }
                else
                {
                    disconn_reason = BT_PBAP_FORBIDDEN_RSP;
                    bt_pal_pbap_pce_stop_instance(pbap_pce);
                    pbap_pce_free_instance(pbap_pce);
                    break;
                }
            }
            pbap_pce->max_pkt_len = pbap_headers->pbap_connect_info->max_recv_size + (3U + 5U + 3U); /* Subtract 11 in stack and add 11 back here. */
            if (pbap_pce_cb->connected != NULL && pbap_pce != NULL)
            {
                pbap_pce_cb->connected(pbap_pce);
            }
            break;
        case PBAP_PCE_DISCONNECT_CFM:
            /* Transport close has been sent in the underlying layer so don't need call BT_pbap_pce_trans_disconnect. */
            /* if (pbap_pce != NULL)
            {
                BT_pbap_pce_trans_disconnect(pbap_pce->handle);
            } */
            break;

        case PBAP_PCE_GET_PHONEBOOK_CFM:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL || pbap_pce == NULL)
            {
                break;
            }
            appl_param = pbap_headers->pbap_resp_info->appl_params;
            appl_params_from_stack_to_buf(buf, appl_param);
            if (event_result == BT_PBAP_CONTINUE_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else if (event_result == BT_PBAP_SUCCESS_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_END_OF_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else
            {
                /* no action*/
            }
            if (pbap_pce_cb != NULL && pbap_pce_cb->pull_phonebook)
            {
                pbap_pce_cb->pull_phonebook(pbap_pce, event_result, buf);
            }
            else
            {
                net_buf_unref(buf);
            }
            break;

        case PBAP_PCE_GET_VCARD_LIST_CFM:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL || pbap_pce == NULL)
            {
                break;
            }
            appl_param = pbap_headers->pbap_resp_info->appl_params;
            appl_params_from_stack_to_buf(buf, appl_param);
            if (event_result == BT_PBAP_CONTINUE_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else if (event_result == BT_PBAP_SUCCESS_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_END_OF_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else
            {
                /* no action*/
            }
            if (pbap_pce_cb != NULL && pbap_pce_cb->pull_vcard_listing)
            {
                pbap_pce_cb->pull_vcard_listing(pbap_pce, event_result, buf);
            }
            else
            {
                net_buf_unref(buf);
            }
            break;

        case PBAP_PCE_GET_VCARD_CFM:
            buf = net_buf_alloc(&pbap_pool, osaWaitForever_c);
            if (buf == NULL || pbap_pce == NULL)
            {
                break;
            }
            appl_param = pbap_headers->pbap_resp_info->appl_params;
            appl_params_from_stack_to_buf(buf, appl_param);
            if (event_result == BT_PBAP_CONTINUE_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else if (event_result == BT_PBAP_SUCCESS_RSP)
            {
                if (pbap_headers->pbap_resp_info->body != NULL)
                {
                    BT_PBAP_ADD_END_OF_BODY(buf, pbap_headers->pbap_resp_info->body->value, pbap_headers->pbap_resp_info->body->length);
                }
            }
            else
            {
                /* no action*/
            }
            if (pbap_pce_cb != NULL && pbap_pce_cb->pull_vcard_entry != NULL && pbap_pce != NULL)
            {
                pbap_pce_cb->pull_vcard_entry(pbap_pce, event_result, buf);
            }
            else
            {
                net_buf_unref(buf);
            }
            break;

        case PBAP_PCE_SET_PHONEBOOK_CFM:
            if (pbap_pce_cb != NULL && pbap_pce_cb->set_phonebook_path != NULL && pbap_pce != NULL)
            {
                pbap_pce_cb->set_phonebook_path(pbap_pce, event_result);
            }
            break;

        case PBAP_PCE_ABORT_CFM:
            if (pbap_pce_cb != NULL && pbap_pce_cb->abort != NULL && pbap_pce != NULL)
            {
                pbap_pce_cb->abort(pbap_pce, event_result);
            }
            break;
        case PBAP_PCE_TRANSPORT_CLOSE_CFM:
        case PBAP_PCE_TRANSPORT_CLOSE_IND:
            bt_pal_pbap_pce_stop_instance(pbap_pce);
            pbap_pce_free_instance(pbap_pce);
            if (pbap_pce_cb != NULL && pbap_pce_cb->disconnected != NULL && pbap_pce != NULL)
            {
                if (disconn_reason == BT_PBAP_FORBIDDEN_RSP)
                {
                    event_result = BT_PBAP_FORBIDDEN_RSP;
                }
                pbap_pce_cb->disconnected(pbap_pce, event_result);
            }
            break;
        default:
            break;
    }
    return API_SUCCESS;
}

static void bt_pbap_add_connect_hdr(struct net_buf *buf, char *password, uint32_t peer_feature)
{
    uint16_t packet_length = 1 + 2 + 1 + 1 + 2 + 19;

    /*  opcode */
    net_buf_add_u8(buf, OBEX_CONNECT_OP);
    /* packet_length */
    if (password)
    {
        packet_length += 21;
    }
    packet_length += 9;
    net_buf_add_be16(buf, packet_length);

    /*version num*/
    net_buf_add_u8(buf, 0x10);

    /* flags*/
    net_buf_add_u8(buf, 0x0U);

    /* maximum packet length*/
    net_buf_add_be32(buf, CONFIG_BT_PBAP_PCE_MAX_PKT_LEN);

    /*target*/
    net_buf_add_u8(buf, OBEX_HDR_TARGET);
    net_buf_add_be16(buf, 16 + 3);
    net_buf_add_mem(buf, pbap_target_id, 16);

    /* auth challenge*/
    if (password)
    {
        // BT_PBAP_ADD_AUTH_CHALLENGE_NONCE(buf, password);
    }
    if (peer_feature)
    {
        /* supported feature*/
        BT_PBAP_ADD_PARAMS_PBAP_SUPPORTED_FEATURES(buf, CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE);
    }
}

static int bt_pbap_pce_connect(struct bt_conn *conn,
                               uint8_t channel,
                               uint16_t psm,
                               struct bt_pbap_auth *auth,
                               uint32_t peer_feature,
                               struct bt_pbap_pce **pbap_pce)
{
    API_RESULT retval = 0;
    struct bt_conn_info info;
    struct bt_pbap_pce *_pbap_pce;
    PBAP_CONNECT_STRUCT connect_info;
    PBAP_HEADER_STRUCT pass_word;
    memset(&connect_info, 0, sizeof(connect_info));
#if CONFIG_BT_ZEPHYR_BUF
    struct net_buf *buf;
    buf = bt_conn_create_pdu(NULL, BT_L2CAP_BUF_SIZE(2U));
    if (!buf)
    {
        return -EAGAIN;
    }
    bt_pbap_add_connect_hdr(buf, password, peer_feature);
#endif

    if (conn == NULL)
    {
        return -EAGAIN;
    }
    _pbap_pce = pbap_pce_get_instance(conn);

    if (NULL == _pbap_pce)
    {
        return -EAGAIN;
    }
    _pbap_pce->goep_version  = BT_GOEP_VERSION_1_1;
    _pbap_pce->local_feature = CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE;
    _pbap_pce->peer_feature  = BT_PBAP_SUPPORTED_FEATURES_V11;

    bt_conn_get_info(conn, &info);
    memcpy(connect_info.bd_addr, info.br.dst, BT_BD_ADDR_SIZE);
    /* no Authenticate*/
    connect_info.auth_flag = 0;
    if (peer_feature)
    {
        /* add pbap supported feature*/
        _pbap_pce->peer_feature              = peer_feature;
        connect_info.pbap_supported_features = CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE;
    }
    connect_info.psm          = psm;
    connect_info.server_ch_no = channel;
    if (psm)
    {
        _pbap_pce->goep_version = BT_GOEP_VERSION_2_0;
    }
    connect_info.max_recv_size = CONFIG_BT_PBAP_PCE_MAX_PKT_LEN;

    if (auth != NULL)
    {
        if (auth->pin_code != NULL && strlen(auth->pin_code) > 0)
        {
            pass_word.value           = (UCHAR *)auth->pin_code;
            pass_word.length          = (uint16_t)strlen(auth->pin_code);
            connect_info.pin_info     = &pass_word;
            connect_info.auth_flag    = 1;
            _pbap_pce->auth->pin_code = auth->pin_code;
        }
    }

    retval = BT_pbap_pce_start_instance(&(_pbap_pce->pbap_handle), ethermind_pbap_pce_event_callback);
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Initialization Failed. Reason 0x%04X \n", retval);
        pbap_pce_free_instance(_pbap_pce);
        return retval;
    }
    else
    {
        PRINTF("Successfully START PBAP PCE entities \n");
    }

    retval = BT_pbap_pce_connect(_pbap_pce->pbap_handle, &connect_info);
    if (API_SUCCESS != retval)
    {
        LOG_ERR("Failed. Reason 0x%04X\r\n", retval);
        return retval;
    }
    else
    {
        *pbap_pce = _pbap_pce;
        LOG_INF("Successful\r\n");
    }

#if CONFIG_BT_ZEPHYR_BUF
    net_buf_unref(buf);
#endif

    return 0;
}

int bt_pbap_pce_scn_connect(struct bt_conn *conn, uint8_t channel, struct bt_pbap_auth *auth, uint32_t peer_feature, struct bt_pbap_pce **pbap_pce)
{
    return bt_pbap_pce_connect(conn, channel, 0, auth, peer_feature, pbap_pce);
}

int bt_pbap_pce_psm_connect(struct bt_conn *conn, uint16_t psm, struct bt_pbap_auth *auth, uint32_t peer_feature, struct bt_pbap_pce **pbap_pce)
{
    return bt_pbap_pce_connect(conn, 0, psm, auth, peer_feature, pbap_pce);
}

int bt_pbap_pce_get_max_pkt_len(struct bt_pbap_pce *pbap_pce, uint16_t *max_pkt_len)
{
    if ((pbap_pce == NULL) || (pbap_pce->acl_conn == NULL))
    {
        return -EINVAL;
    }
    *max_pkt_len = pbap_pce->max_pkt_len;

    return 0;
}


#endif /*CONFIG_BT_PBAP_PCE */