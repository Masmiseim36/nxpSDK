/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U))

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
#include <bluetooth/map_mce.h>
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "BT_features.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_rfcomm_internal.h"
#include "bt_pal_at.h"
#include "bt_pal_map_internal.h"
#include "fsl_os_abstraction.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_avdtp_api.h"
#include "BT_a2dp_api.h"
#include "BT_config.h"
#include "BT_jpl_api.h"
#include "BT_fops.h"
#include "BT_sdp_api.h"
#include "BT_map_api.h"
#include "BT_at_parser_api.h"
#include "db_gen.h"
#include "fsl_component_log.h"

#if ((defined(CONFIG_BT_MAP_MCE)) && (CONFIG_BT_MAP_MCE > 0U))

#if (CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES > MAP_MNS_NUM_ENTITIES)
#error "CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES should be less than or equal to MAP_MNS_NUM_ENTITIES"
#endif

/* MAP MCE MAS RX buffer count */
#define BT_MAP_MCE_MAS_RX_NET_BUF_COUNT (1U)
/* MAP MCE MAS RX buffer size */
#define BT_MAP_MCE_MAS_RX_NET_BUF_SIZE (CONFIG_BT_MAP_MCE_MAS_MAX_PKT_LEN)
/* MAP MCE MNS RX buffer count */
#define BT_MAP_MCE_MNS_RX_NET_BUF_COUNT (1U)
/* MAP MCE MNS RX buffer size */
#define BT_MAP_MCE_MNS_RX_NET_BUF_SIZE (CONFIG_BT_MAP_MCE_MNS_MAX_PKT_LEN)

#define EDGEFAST_MAP_MCE_LOCK   OSA_MutexLock(s_MapMceLock, osaWaitForever_c)
#define EDGEFAST_MAP_MCE_UNLOCK OSA_MutexUnlock(s_MapMceLock)

/** @brief MAP MCE MAS structure */
struct bt_map_mce_mas
{
    struct bt_conn *acl_conn;
    uint32_t supported_features;
    uint16_t max_pkt_len;
    uint8_t handle;
};

/** @brief MAP MCE MNS structure */
struct bt_map_mce_mns
{
    struct bt_conn *acl_conn;
    uint16_t max_pkt_len;
    uint8_t handle;
    enum bt_obex_req_flags flag;
};

static API_RESULT map_mce_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

static OSA_MUTEX_HANDLE_DEFINE(s_MapMceLockMutex);
static osa_mutex_handle_t s_MapMceLock;

static struct bt_map_mce_mas map_mce_mas_instances[CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES];
static struct bt_map_mce_mns map_mce_mns_instances[CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES];
static struct bt_map_mce_mas_cb *mce_mas_cb;
static struct bt_map_mce_mns_cb *mce_mns_cb;
static uint8_t mce_init = 0;

NET_BUF_POOL_FIXED_DEFINE(mce_mas_rx_pool, BT_MAP_MCE_MAS_RX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(BT_MAP_MCE_MAS_RX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(mce_mns_rx_pool, BT_MAP_MCE_MNS_RX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(BT_MAP_MCE_MNS_RX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static struct bt_map_mce_mas *map_mce_mas_get_instance(struct bt_conn * conn)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; ++index)
    {
        if (map_mce_mas_instances[index].acl_conn == NULL)
        {
            memset(&map_mce_mas_instances[index], 0U, sizeof(map_mce_mas_instances[index]));
            map_mce_mas_instances[index].acl_conn = conn;
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static struct bt_map_mce_mns *map_mce_mns_get_instance(struct bt_conn * conn)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; ++index)
    {
        if (map_mce_mns_instances[index].acl_conn == NULL)
        {
            memset(&map_mce_mns_instances[index], 0U, sizeof(map_mce_mns_instances[index]));
            map_mce_mns_instances[index].acl_conn = conn;
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static void map_mce_mas_free_instance(struct bt_map_mce_mas *map_mce_mas)
{
    if (NULL != map_mce_mas)
    {
        EDGEFAST_MAP_MCE_LOCK;
        map_mce_mas->acl_conn = NULL;
        EDGEFAST_MAP_MCE_UNLOCK;
    }
}

static void map_mce_mns_free_instance(struct bt_map_mce_mns *map_mce_mns)
{
    if (NULL != map_mce_mns)
    {
        EDGEFAST_MAP_MCE_LOCK;
        map_mce_mns->acl_conn = NULL;
        EDGEFAST_MAP_MCE_UNLOCK;
    }
}

static struct bt_map_mce_mas *map_mce_mas_lookup_instance(MAP_HANDLE handle)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; ++index)
    {
        if ((map_mce_mas_instances[index].acl_conn != NULL) && (map_mce_mas_instances[index].handle == handle))
        {
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static struct bt_map_mce_mns *map_mce_mns_lookup_instance(MAP_HANDLE handle)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; ++index)
    {
        if ((map_mce_mns_instances[index].acl_conn != NULL) && (map_mce_mns_instances[index].handle == handle))
        {
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static struct bt_map_mce_mas *map_mce_mas_lookup_instance_by_addr(uint8_t *bd_addr)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; ++index)
    {
        if ((map_mce_mas_instances[index].acl_conn != NULL) &&
            (memcmp(&map_mce_mas_instances[index].acl_conn->br.dst, bd_addr, BT_BD_ADDR_SIZE) == 0))
        {
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static struct bt_map_mce_mns *map_mce_mns_lookup_instance_by_addr(uint8_t *bd_addr)
{
    EDGEFAST_MAP_MCE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; ++index)
    {
        if ((map_mce_mns_instances[index].acl_conn != NULL) &&
            (memcmp(&map_mce_mns_instances[index].acl_conn->br.dst, bd_addr, BT_BD_ADDR_SIZE) == 0))
        {
            EDGEFAST_MAP_MCE_UNLOCK;
            return &map_mce_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MCE_UNLOCK;
    return NULL;
}

static int bt_map_copy_tag_from_buf_to_stack(struct bt_obex_tag_bytes *tag, MAP_APPL_PARAMS *appl_param)
{
    switch (tag->id)
    {
        case BT_MAP_TAG_ID_MAX_LIST_COUNT:
            appl_param->max_list_count = sys_get_be16(tag->value);
            break;
        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            appl_param->start_offset = sys_get_be16(tag->value);
            break;
        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
            appl_param->filter_message_type = *tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
            appl_param->filter_period_begin.length = tag->length;
            appl_param->filter_period_begin.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
            appl_param->filter_period_end.length = tag->length;
            appl_param->filter_period_end.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
            appl_param->filter_read_status = *tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
            appl_param->filter_recipient.length = tag->length;
            appl_param->filter_recipient.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
            appl_param->filter_originator.length = tag->length;
            appl_param->filter_originator.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_PRIORITY:
            appl_param->filter_priority = *tag->value;
            break;
        case BT_MAP_TAG_ID_ATTACHMENT:
            appl_param->attachment = *tag->value;
            break;
        case BT_MAP_TAG_ID_TRANSPARENT:
            appl_param->transparent = *tag->value;
            break;
        case BT_MAP_TAG_ID_RETRY:
            appl_param->retry = *tag->value;
            break;
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            return -EINVAL;
        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            appl_param->notification_status = *tag->value;
            break;
        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            appl_param->mas_instance_id = *tag->value;
            break;
        case BT_MAP_TAG_ID_PARAMETER_MASK:
            appl_param->parameter_mask = sys_get_be32(tag->value);
            break;
        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
        case BT_MAP_TAG_ID_LISTING_SIZE:
            return -EINVAL;
        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
            appl_param->subject_length = *tag->value;
            break;
        case BT_MAP_TAG_ID_CHARSET:
            appl_param->charset = *tag->value;
            break;
        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            appl_param->fraction_request = *tag->value;
            break;
        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            return -EINVAL;
        case BT_MAP_TAG_ID_STATUS_INDICATOR:
            appl_param->status_indicator = *tag->value;
            break;
        case BT_MAP_TAG_ID_STATUS_VALUE:
            appl_param->status_value = *tag->value;
            break;
        case BT_MAP_TAG_ID_MSE_TIME:
            return -EINVAL;
#ifdef MAP_1_3
        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            return -EINVAL;
        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            appl_param->presence_availability = *tag->value;
            break;
        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            appl_param->presence_text.length = tag->length;
            appl_param->presence_text.value = tag->value;
            break;
        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            appl_param->last_activity.length = tag->length;
            appl_param->last_activity.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN:
            appl_param->filter_last_activity_begin.length = tag->length;
            appl_param->filter_last_activity_begin.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            appl_param->filter_last_activity_end.length = tag->length;
            appl_param->filter_last_activity_end.value = tag->value;
            break;
        case BT_MAP_TAG_ID_CHAT_STATE:
            appl_param->chat_state = *tag->value;
            break;
        case BT_MAP_TAG_ID_CONVERSATION_ID:
            appl_param->conversation_id.length = tag->length;
            appl_param->conversation_id.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            return -EINVAL;
        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
            appl_param->filter_msg_handle.length = tag->length;
            appl_param->filter_msg_handle.value = tag->value;
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
            appl_param->ntf_filter_mask = sys_get_be32(tag->value);
            break;
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            appl_param->conv_parameter_mask = sys_get_be32(tag->value);
            break;
        case BT_MAP_TAG_ID_OWNER_UCI:
            return -EINVAL;
        case BT_MAP_TAG_ID_EXTENDED_DATA:
            appl_param->extended_data.length = tag->length;
            appl_param->extended_data.value = tag->value;
            break;
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            appl_param->supported_features = sys_get_be32(tag->value);
            break;
#endif /* MAP_1_3 */
#ifdef MAP_1_4
        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
            appl_param->msg_handle.length = tag->length;
            appl_param->msg_handle.value = tag->value;
            break;
        case BT_MAP_TAG_ID_MODIFY_TEXT:
            appl_param->modify_text = *tag->value;
            break;
#endif /* MAP_1_4 */
        default:
            return -EINVAL;
    }

    return 0;
}

static int bt_map_copy_appl_param_from_buf_to_stack(struct net_buf *buf, MAP_APPL_PARAMS *appl_param, uint16_t *pkt_len)
{
    int err;
    struct bt_obex_tag_bytes tag;
    uint16_t hdr_length;
    uint8_t *hdr_value;

    err = bt_obex_get_hdr(buf, BT_MAP_HDR_APP_PARAM, &hdr_value, &hdr_length);
    if (err < 0)
    {
        return err;
    }

    *pkt_len += hdr_length + sizeof(struct bt_obex_hdr_bytes);

    while (hdr_length)
    {
        err = bt_map_copy_tag_from_buf_to_stack((struct bt_obex_tag_bytes *)hdr_value, appl_param);
        if (err < 0)
        {
            return err;
        }
        tag.id = ((struct bt_obex_tag_bytes *)hdr_value)->id - 1;
#ifndef MAP_1_3
        MAP_SET_APPL_PARAM_FLAG
        (
            appl_param->appl_param_flag,
            (uint32_t)1U << tag.id % (sizeof(appl_param->appl_param_flag[0]) * 8U),
        );
#else
        MAP_SET_APPL_PARAM_FLAG_EXT
        (
            appl_param->appl_param_flag,
            (uint32_t)1U << tag.id % (sizeof(appl_param->appl_param_flag[0]) * 8U),
            tag.id / (sizeof(appl_param->appl_param_flag[0]) * 8U)
        );
#endif /* MAP_1_3 */
        tag.length = ((struct bt_obex_tag_bytes *)hdr_value)->length + sizeof(struct bt_obex_tag_bytes);
        if (hdr_length < tag.length)
        {
            return -EINVAL;
        }
        hdr_length -= tag.length;
        hdr_value  += tag.length;
    }

    return 0;
}

static int bt_map_copy_tag_from_stack_to_buf(uint8_t tag_id, MAP_APPL_PARAMS *appl_param, struct net_buf *buf)
{
    switch (tag_id)
    {
        case BT_MAP_TAG_ID_MAX_LIST_COUNT:
        case BT_MAP_TAG_ID_LIST_START_OFFSET:
        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
        case BT_MAP_TAG_ID_FILTER_PRIORITY:
        case BT_MAP_TAG_ID_ATTACHMENT:
        case BT_MAP_TAG_ID_TRANSPARENT:
        case BT_MAP_TAG_ID_RETRY:
            return -EINVAL;
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            BT_MAP_ADD_NEW_MESSAGE(buf, appl_param->new_message);
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            return -EINVAL;
        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            BT_MAP_ADD_MAS_INSTANCE_ID(buf, appl_param->mas_instance_id);
            break;
        case BT_MAP_TAG_ID_PARAMETER_MASK:
            return -EINVAL;
        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
            BT_MAP_ADD_FOLDER_LISTING_SIZE(buf, appl_param->folder_listing_size);
            break;
        case BT_MAP_TAG_ID_LISTING_SIZE:
            BT_MAP_ADD_LISTING_SIZE(buf, appl_param->messages_listing_size);
            break;
        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
        case BT_MAP_TAG_ID_CHARSET:
        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            return -EINVAL;
        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            BT_MAP_ADD_FRACTION_DELIVER(buf, appl_param->fraction_deliver);
            break;
        case BT_MAP_TAG_ID_STATUS_INDICATOR:
        case BT_MAP_TAG_ID_STATUS_VALUE:
            return -EINVAL;
        case BT_MAP_TAG_ID_MSE_TIME:
            BT_MAP_ADD_MSE_TIME(buf, appl_param->mse_time.value, appl_param->mse_time.length);
            break;
#ifdef MAP_1_3
        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
            BT_MAP_ADD_DATABASE_IDENTIFIER(buf, appl_param->database_identifier.value, appl_param->database_identifier.length);
            break;
        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            BT_MAP_ADD_CONV_LIST_VER_CNTR(buf, appl_param->conv_listing_ver_cntr.value, appl_param->conv_listing_ver_cntr.length);
            break;
        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            BT_MAP_ADD_PRESENCE_AVAILABILITY(buf, appl_param->presence_availability);
            break;
        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            BT_MAP_ADD_PRESENCE_TEXT(buf, appl_param->presence_text.value, appl_param->presence_text.length);
            break;
        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            BT_MAP_ADD_LAST_ACTIVITY(buf, appl_param->last_activity.value, appl_param->last_activity.length);
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN:
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            return -EINVAL;
        case BT_MAP_TAG_ID_CHAT_STATE:
            BT_MAP_ADD_CHAT_STATE(buf, appl_param->chat_state);
            break;
        case BT_MAP_TAG_ID_CONVERSATION_ID:
            return -EINVAL;
        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            BT_MAP_ADD_FOLDER_VER_CNTR(buf, appl_param->folder_ver_cntr.value, appl_param->folder_ver_cntr.length);
            break;
        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            return -EINVAL;
        case BT_MAP_TAG_ID_OWNER_UCI:
            BT_MAP_ADD_OWNER_UCI(buf, appl_param->owner_uci.value, appl_param->owner_uci.length);
            break;
        case BT_MAP_TAG_ID_EXTENDED_DATA:
            return -EINVAL;
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            BT_MAP_ADD_MAP_SUPPORTED_FEATURES(buf, appl_param->supported_features);
            break;
#endif /* MAP_1_3 */
#ifdef MAP_1_4
        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
        case BT_MAP_TAG_ID_MODIFY_TEXT:
#endif /* MAP_1_4 */
        default:
            return -EINVAL;
    }

    return 0;
}

static void bt_map_copy_appl_param_from_stack_to_buf(struct net_buf *buf, MAP_APPL_PARAMS *appl_param)
{
    if (appl_param == NULL)
    {
        return;
    }

    for(uint8_t index = 0U; index < MAP_NUM_APPL_HEADERS; index++)
    {
#ifndef MAP_1_3
        if (MAP_GET_APPL_PARAM_FLAG
        (
            appl_param->appl_param_flag,
            (uint32_t)1U << index % (sizeof(appl_param->appl_param_flag[0]) * 8U),
        ) != 0U)
#else
        if (MAP_GET_APPL_PARAM_FLAG_EXT
        (
            appl_param->appl_param_flag,
            (uint32_t)1U << index % (sizeof(appl_param->appl_param_flag[0]) * 8U),
            index / (sizeof(appl_param->appl_param_flag[0]) * 8U)
        ) != 0U)
#endif /* MAP_1_3 */
        {
            bt_map_copy_tag_from_stack_to_buf(index + 1, appl_param, buf);
        }
    }
}

#ifdef MAP_USE_NET_BUF
static void bt_map_push_hdr(struct net_buf *buf, uint8_t hi, uint8_t *value, uint16_t length)
{
    net_buf_push_mem(buf, value, length);
    net_buf_push_be16(buf, length + sizeof(struct bt_obex_hdr_bytes));
    net_buf_push_u8(buf, hi);
}

static void bt_map_push_hdr_u8(struct net_buf *buf, uint8_t hi, uint8_t value)
{
    net_buf_push_u8(buf, value);
    net_buf_push_u8(buf, hi);
}

static void bt_map_push_hdr_u32(struct net_buf *buf, uint8_t hi, uint32_t value)
{
    net_buf_push_be32(buf, value);
    net_buf_push_u8(buf, hi);
}
#endif /* MAP_USE_NET_BUF */

static int map_mce_init(void)
{
    UCHAR handle;
    if (mce_init == 0)
    {
        BT_map_mce_init();
        for (uint8_t index = 0U; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; index++)
        {
            map_mce_mas_instances[index].acl_conn = NULL;
        }
        for (uint8_t index = 0U; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; index++)
        {
            map_mce_mns_instances[index].acl_conn = NULL;
        }
        for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; index++)
        {
            BT_map_mce_start(MAP_NTF_SERVICE, map_mce_callback, &handle);
        }
        if (NULL == s_MapMceLock)
        {
            if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_MapMceLockMutex))
            {
                s_MapMceLock = (osa_mutex_handle_t)s_MapMceLockMutex;
            }
            else
            {
                return -EIO;
            }
        }
    }
    mce_init++;

    return 0;
}

static int map_mce_deinit(void)
{
    mce_init--;

    if (mce_init == 0)
    {
        for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; index++)
        {
            BT_map_mce_stop(MAP_NTF_SERVICE, &index);
        }
        BT_map_mce_shutdown();
        if (NULL != s_MapMceLock)
        {
            OSA_MutexDestroy((osa_mutex_handle_t)s_MapMceLockMutex);
            s_MapMceLock = NULL;
        }
    }

    return 0;
}

int bt_map_mce_mas_register(struct bt_map_mce_mas_cb *cb)
{
    int err;

	if (!cb) {
		return -EINVAL;
	}

	if (mce_mas_cb) {
		return -EALREADY;
	}

    err = map_mce_init();
    if (err != 0)
    {
        return err;
    }

    mce_mas_cb = cb;
    return 0;
}

int bt_map_mce_mns_register(struct bt_map_mce_mns_cb *cb)
{
    int err;

	if (!cb) {
		return -EINVAL;
	}

	if (mce_mns_cb) {
		return -EALREADY;
	}

    err = map_mce_init();
    if (err != 0)
    {
        return err;
    }

    mce_mns_cb = cb;
    return 0;
}

int bt_map_mce_mas_unregister(void)
{
    if (mce_mas_cb != NULL)
    {
        map_mce_deinit();
        mce_mas_cb = NULL;
    }
    return 0;
}

int bt_map_mce_mns_unregister(void)
{
    if (mce_mns_cb != NULL)
    {
        map_mce_deinit();
        mce_mns_cb = NULL;
    }
    return 0;
}

/** @brief Create MCE MAS connection
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish MAS OBEX
 *  connection between devices.
 *  This function establishes L2CAP or RFCOM connection.
 *  This function should not directly used by application. Instead, application
 *  should use the macro of bt_map_mce_psm_connect or bt_map_mce_scn_connect to
 *  create L2CAP or RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param mce_mas  MCE MAS object.
 *  @param psm  GoepL2capPsm, returned in SDP record.
 *  @param scn  RFCOM server channel number, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
static int bt_map_mce_connect(struct bt_conn *conn, uint16_t psm, uint8_t scn, uint32_t supported_features, struct bt_map_mce_mas **mce_mas)
{
    MAP_CONNECT_STRUCT connect_info;
    struct bt_conn_info info;
    struct bt_map_mce_mas *_mce_mas;
#ifdef MAP_USE_NET_BUF
    uint16_t reserve = sizeof(struct map_mce_connect_hdr);
    uint16_t pkt_len = sizeof(struct map_mce_connect_hdr);
    uint8_t uuid[] = MAP_MAS_TARGET_UUID;
    struct net_buf *buf;
#endif

    if ((conn == NULL) || (mce_mas == NULL))
    {
        return -EINVAL;
    }

    *mce_mas = NULL;

    /* Create MAS instance */
    _mce_mas = map_mce_mas_get_instance(conn);
    if (_mce_mas == NULL)
    {
        return -EAGAIN;
    }
    if (BT_map_mce_start(MAP_ACCESS_SERVICE, map_mce_callback, &_mce_mas->handle) != API_SUCCESS)
    {
        map_mce_mas_free_instance(_mce_mas);
        return -EAGAIN;
    }

    *mce_mas = _mce_mas;

    (void)memset(&info, 0, sizeof(info));
    (void)memset(&connect_info, 0, sizeof(connect_info));

    bt_conn_get_info(conn, &info);

    connect_info.bd_addr = (UCHAR *)&info.br.dst->val[0];
    connect_info.psm = psm;
    connect_info.server_channel = scn;
    connect_info.max_recv_size = CONFIG_BT_MAP_MCE_MAS_MAX_PKT_LEN;
    if (supported_features & 1UL << 19U)
    {
        connect_info.map_supported_features = CONFIG_BT_MAP_MCE_MAS_SUPPORTED_FEATURES;
    }

#ifdef MAP_USE_NET_BUF
    reserve += sizeof(struct bt_l2cap_hdr) + 2U; /* L2CAP I-frame Enhanced Control Field(2-byte) */
    buf = bt_conn_create_pdu(NULL, reserve);
#ifdef MAP_1_3
    if (supported_features & 1UL << 19U)
    {
        BT_MAP_ADD_MAP_SUPPORTED_FEATURES(buf, CONFIG_BT_MAP_MCE_MAS_SUPPORTED_FEATURES);
        pkt_len += buf->len;
    }
#endif
    bt_map_push_hdr(buf, BT_MAP_HDR_TARGET, uuid, sizeof(uuid));
    net_buf_push_be16(buf, CONFIG_BT_MAP_MCE_MAS_MAX_PKT_LEN);
    net_buf_push_u8(buf, 0x00); /* flags */
    net_buf_push_u8(buf, 0x00); /* TODO: fill OBEX Version Number */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_CONNECT_OP);
    net_buf_unref(buf);
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_connect(&_mce_mas->handle, &connect_info) != API_SUCCESS)
    {
        map_mce_mas_free_instance(_mce_mas);
        return -EIO;
    }
    _mce_mas->supported_features = supported_features;

    return 0;
}

int bt_map_mce_psm_connect(struct bt_conn *conn, uint16_t psm, uint32_t supported_features, struct bt_map_mce_mas **mce_mas)
{
    return bt_map_mce_connect(conn, psm, 0, supported_features, mce_mas);
}

int bt_map_mce_scn_connect(struct bt_conn *conn, uint8_t scn, uint32_t supported_features, struct bt_map_mce_mas **mce_mas)
{
    return bt_map_mce_connect(conn, 0, scn, supported_features, mce_mas);
}

int bt_map_mce_disconnect(struct bt_map_mce_mas *mce_mas)
{
    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    if (BT_map_mce_disconnect(&mce_mas->handle) != API_SUCCESS)
    {
        return -EIO;
    }
    return 0;
}

int bt_map_mce_abort(struct bt_map_mce_mas *mce_mas)
{
    API_RESULT retval;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    retval = BT_map_mce_abort(&mce_mas->handle);
    if (retval == API_SUCCESS)
    {
        return 0;
    }
    else if (retval == MAP_INVALID_PARAMETERS)
    {
        return -ESRCH;
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

int bt_map_mce_mns_disconnect(struct bt_map_mce_mns *mce_mns)
{
    if (mce_mns == NULL)
    {
        return -EINVAL;
    }

    if (BT_map_mce_ns_transport_close(&mce_mns->handle) != API_SUCCESS)
    {
        return -EIO;
    }
    return 0;
}

int bt_map_mce_get_folder_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT get_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));
    get_info.wait = wait;
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_FOLDER_LISTING, sizeof(BT_MAP_TYPE_GET_FOLDER_LISTING));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_FOLDER_LISTING);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_get_folder_listing(&mce_mas->handle, &get_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &get_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_GET_FOLDER_LISTING_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                0,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_set_folder(struct bt_map_mce_mas *mce_mas, char *name)
{
    MAP_REQUEST_STRUCT set_info;
    MAP_HEADER_STRUCT name_req;
    uint8_t setpath_flag;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_FLAGS_SIZE + BT_MAP_CONSTANT_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if ((mce_mas == NULL) || (name == NULL))
    {
        return -EINVAL;
    }

    if (strcmp(name, "/") == 0)
    {
        setpath_flag = MAP_SET_ROOT_FOLDER;
        name_req.value = NULL;
        name_req.length = 0;
    }
    else if (strncmp(name, "..", 2U) == 0)
    {
        setpath_flag = MAP_SET_PARENT_FOLDER;
        if (name[2] == '/')
        {
            name_req.value = (uint8_t *)&name[3];
            name_req.length = (strlen(&name[3]) == 0) ? 0 : strlen(&name[3]) + 1U;
        }
        else if (name[2] != '\0')
        {
            name_req.value = (uint8_t *)&name[2];
            name_req.length = strlen(&name[2]) + 1U;
        }
        else
        {
            name_req.value = NULL;
            name_req.length = 0;
        }
    }
    else
    {
        setpath_flag = MAP_SET_CHILD_FOLDER;
        if (strncmp(name, "./", 2U) == 0)
        {
            if (name[2] == '\0')
            {
                return -EINVAL;
            }
            else
            {
                name_req.value = (uint8_t *)&name[2];
                name_req.length = strlen(&name[2]) + 1U;
            }
        }
        else if (name[0] != '\0')
        {
            name_req.value = (uint8_t *)name;
            name_req.length = strlen(name) + 1U;
        }
        else
        {
            return -EINVAL;
        }
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    set_info.name = &name_req;
    set_info.setpath_flag = setpath_flag;

#ifdef MAP_USE_NET_BUF
    if (setpath_flag == 0x03U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_NAME, NULL, 0);
        pkt_len += BT_MAP_HEADER_SIZE;
    }
    else
    {
        if (name != NULL)
        {
            bt_map_push_hdr(buf, BT_MAP_HDR_NAME, name, name_req.length);
            pkt_len += BT_MAP_HEADER_SIZE + name_req.length;
        }
    }
    if (setpath_flag == 0x02U)
    {
        setpath_flag = 0x03;
    }
    else
    {
        setpath_flag = 0x02;
    }
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_u8(buf, 0U);
    net_buf_push_u8(buf, setpath_flag);
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_SETPATH_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_set_folder(&mce_mas->handle, &set_info) != API_SUCCESS)
    {
        return -EIO;
    }

    return 0;
}

int bt_map_mce_get_msg_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT get_info;
    MAP_HEADER_STRUCT name_req;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(get_info));
    get_info.wait = wait;

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (name != NULL)
        {
            name_req.value = (uint8_t *)name;
            name_req.length = (strlen(name) == 0) ? 0U : strlen(name) + 1U;
        }
        else
        {
            name_req.length = 0;
            name_req.value = NULL;
        }
        get_info.name = &name_req;
    }

    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (name != NULL)
        {
            bt_map_push_hdr(buf, BT_MAP_HDR_NAME, name, name_req.length);
            pkt_len += BT_MAP_HEADER_SIZE + name_req.length;
        }
        else
        {
            bt_map_push_hdr(buf, BT_MAP_HDR_NAME, NULL, 0U);
            pkt_len += BT_MAP_HEADER_SIZE;
        }
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_MSG_LISTING, sizeof(BT_MAP_TYPE_GET_MSG_LISTING));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_MSG_LISTING);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_GET_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_get_messages_listing(&mce_mas->handle, &get_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &get_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_GET_MESSAGES_LISTING_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                0,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_get_msg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT get_info;
    MAP_HEADER_STRUCT name_req;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));
    get_info.wait = wait;
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (name == NULL)
        {
            LOG_ERR("The name is not present or empty.");
            return -EINVAL;
        }
        name_req.length = (strlen(name) == 0) ? 0U : strlen(name) + 1U;
        name_req.value = (uint8_t *)name;
        get_info.name = &name_req;
        if (name_req.length > BT_MAP_MSG_HANDLE_SIZE / 2U)
        {
            LOG_ERR("The name length is greater than BT_MAP_MSG_HANDLE_SIZE.");
            return -EINVAL;
        }
    }

    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_MSG, sizeof(BT_MAP_TYPE_GET_MSG));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_MSG);
        bt_map_push_hdr(buf, BT_MAP_HDR_NAME, name, name_req.length);
        pkt_len += BT_MAP_HEADER_SIZE + name_req.length;

    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_GET_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_get_message(&mce_mas->handle, &get_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &get_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_GET_MESSAGE_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                0,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_set_msg_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT set_info;
    MAP_HEADER_STRUCT name_req;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (name == NULL)
        {
            LOG_ERR("The name is not present or empty.");
            return -EINVAL;
        }
        name_req.value = (uint8_t *)name;
        name_req.length = (strlen(name) == 0) ? 0U : strlen(name) + 1U;
        set_info.name = &name_req;
        if (name_req.length > BT_MAP_MSG_HANDLE_SIZE / 2U)
        {
            LOG_ERR("The name length is greater than BT_MAP_MSG_HANDLE_SIZE.");
            return -EINVAL;
        }
    }
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        set_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        BT_MAP_ADD_END_OF_BODY(buf, MAP_FILLER_BYTE, sizeof(MAP_FILLER_BYTE) - 1U);
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(MAP_FILLER_BYTE) -  1U;
    }
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_SET_MSG_STATUS, sizeof(BT_MAP_TYPE_SET_MSG_STATUS));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_SET_MSG_STATUS);
        bt_map_push_hdr(buf, OBEX_HDR_ANME, name, name_req.length);
        pkt_len += BT_MAP_HEADER_SIZE + name_req.length;
    }
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_PUT_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_set_message_status(&mce_mas->handle, &set_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &set_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_SET_MESSAGE_STATUS_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                ((flags & BT_OBEX_REQ_END) != 0U) ? 0 : 1,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_push_msg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT set_info;
    MAP_HEADER_STRUCT name_req;
    MAP_HEADER_STRUCT body_req;
    MAP_APPL_PARAMS appl_param;
    uint8_t more;
    uint16_t actual;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    if (bt_obex_get_body(buf, &body_req.value, &body_req.length) < 0)
    {
        body_req.value = NULL;
        body_req.length = 0;
    }
    set_info.body = &body_req;
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (name == NULL)
        {
            name_req.length = 0;
            name_req.value = NULL;
        }
        else
        {
            name_req.length = (strlen(name) == 0) ? 0U : strlen(name) + 1U;
            name_req.value = (uint8_t *)name;
        }
        set_info.name = &name_req;
    }

    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        set_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

    more = ((flags & BT_OBEX_REQ_END) != 0U) ? 0 : 1;

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_NAME, name, name_req.length);
        pkt_len += BT_MAP_HEADER_SIZE + name_req.length;
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_PUSH_MSG, sizeof(BT_MAP_TYPE_PUSH_MSG));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_PUSH_MSG);
#ifdef MAP_1_2
        if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
#endif /* MAP_1_2 */
    }

    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_PUT_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_push_message(&mce_mas->handle, &set_info, more, &actual) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &set_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_PUSH_MESSAGE_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                more,
                &actual
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_set_ntf_reg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf)
{
    int err;
    MAP_REQUEST_STRUCT set_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        set_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    BT_MAP_ADD_END_OF_BODY(buf, MAP_FILLER_BYTE, sizeof(MAP_FILLER_BYTE) - 1U);
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(MAP_FILLER_BYTE) -  1U;
    bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_SET_NTF_REG, sizeof(BT_MAP_TYPE_SET_NTF_REG));
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_SET_NTF_REG);
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_set_ntf_registration(&mce_mas->handle, &set_info) != API_SUCCESS)
    {
        return -EIO;
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_update_inbox(struct bt_map_mce_mas *mce_mas)
{
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

#ifdef MAP_USE_NET_BUF
    BT_MAP_ADD_END_OF_BODY(buf, MAP_FILLER_BYTE, sizeof(MAP_FILLER_BYTE) - 1U);
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(MAP_FILLER_BYTE) -  1U;
    bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_UPDATE_INBOX, sizeof(BT_MAP_TYPE_UPDATE_INBOX));
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_UPDATE_INBOX);
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_update_inbox(&mce_mas->handle) != API_SUCCESS)
    {
        return -EIO;
    }

    return 0;
}

int bt_map_mce_get_mas_inst_info(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_REQUEST_STRUCT get_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));
    get_info.wait = wait;
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_MAS_INST_INFO, sizeof(BT_MAP_TYPE_GET_MAS_INST_INFO));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_MAS_INST_INFO);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_get_mas_instance_info(&mce_mas->handle, &get_info) != API_SUCCESS)
    {
        return -EIO;
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_set_owner_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT set_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        set_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        BT_MAP_ADD_END_OF_BODY(buf, MAP_FILLER_BYTE, sizeof(MAP_FILLER_BYTE) - 1U);
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(MAP_FILLER_BYTE) -  1U;
    }
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_SET_OWNER_STATUS, sizeof(BT_MAP_TYPE_SET_OWNER_STATUS));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_SET_OWNER_STATUS);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_PUT_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_set_owner_status(&mce_mas->handle, &set_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &set_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_SET_OWNER_STATUS_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                ((flags & BT_OBEX_REQ_END) != 0U) ? 0 : 1,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_get_owner_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT get_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));
    get_info.wait = wait;
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_OWNER_STATUS, sizeof(BT_MAP_TYPE_GET_OWNER_STATUS));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_OWNER_STATUS);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_get_owner_status(&mce_mas->handle, &get_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &get_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_GET_OWNER_STATUS_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                0,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_get_convo_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT get_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));
    get_info.wait = wait;
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        get_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_GET_CONVO_LISTING, sizeof(BT_MAP_TYPE_GET_CONVO_LISTING));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_GET_CONVO_LISTING);
    }
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((flags & BT_OBEX_REQ_START) != 0U)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    if ((flags & BT_OBEX_REQ_END) != 0U)
    {
        net_buf_push_u8(buf, OBEX_GET_FINAL_OP);
    }
    else
    {
        net_buf_push_u8(buf, OBEX_GET_OP);
    }
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if ((flags & BT_OBEX_REQ_START) != 0U)
    {
        if (BT_map_mce_get_conversation_listing(&mce_mas->handle, &get_info) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &get_info;
        if (BT_map_mce_send_request
            (
                &mce_mas->handle,
                MAP_MCE_GET_CONVERSATION_LIST_CNF,
                BT_MAP_RSP_CONTINUE,
                &map_header,
                ((flags & BT_OBEX_REQ_END) != 0U) ? 0 : 1,
                &pkt_len
            ))
        {
            return -EIO;
        }
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_set_ntf_filter(struct bt_map_mce_mas *mce_mas, struct net_buf *buf)
{
    int err;
    MAP_REQUEST_STRUCT set_info;
    MAP_APPL_PARAMS appl_param;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mce_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
    err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
    if (err == 0)
    {
        set_info.appl_params = &appl_param;
    }
    else
    {
        if (err != -EAGAIN)
        {
            return err;
        }
    }

#ifdef MAP_USE_NET_BUF
    BT_MAP_ADD_END_OF_BODY(buf, MAP_FILLER_BYTE, sizeof(MAP_FILLER_BYTE) - 1U);
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(MAP_FILLER_BYTE) -  1U;
    bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_SET_NTF_FILTER, sizeof(BT_MAP_TYPE_SET_NTF_FILTER));
    pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_SET_NTF_FILTER);
#ifdef MAP_1_2
    if (mce_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
        pkt_len += BT_MAP_SRM_SIZE;
    }
#endif /* MAP_1_2 */
    bt_map_push_hdr_u32(buf, BT_MAP_HDR_CONNECTION_ID, 0U); /* TODO: fill connection ID */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_PUT_FINAL_OP);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mce_set_notification_filter(&mce_mas->handle, &set_info) != API_SUCCESS)
    {
        return -EIO;
    }

    net_buf_unref(buf);

    return 0;
}

int bt_map_mce_send_event_response(struct bt_map_mce_mns *mce_mns, uint8_t result, bool wait)
{
    MAP_HEADERS map_header;
    MAP_RESPONSE_STRUCT rsp_info;

    if (mce_mns == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&rsp_info, 0, sizeof(MAP_REQUEST_STRUCT));
    BT_mem_set(&map_header, 0, sizeof(map_header));
    rsp_info.wait = wait;
    map_header.map_resp_info = &rsp_info;
    if (BT_map_mce_ns_send_response(&mce_mns->handle, MAP_MCE_NS_EVENT_REPORT_IND, result, &map_header) != API_SUCCESS)
    {
        return -EIO;
    }

    if (result != BT_MAP_RSP_CONTINUE)
    {
        mce_mns->flag = BT_OBEX_REQ_UNSEG;
    }

    return 0;
}

int bt_map_mce_get_max_pkt_len(struct bt_map_mce_mas *mce_mas, uint16_t *max_pkt_len)
{
    if ((mce_mas == NULL) || (mce_mas->acl_conn == NULL))
    {
        return -EINVAL;
    }
    *max_pkt_len = mce_mas->max_pkt_len;

    return 0;
}

int bt_map_mce_mns_get_max_pkt_len(struct bt_map_mce_mns *mce_mns, uint16_t *max_pkt_len)
{
    if ((mce_mns == NULL) || (mce_mns->acl_conn == NULL))
    {
        return -EINVAL;
    }
    *max_pkt_len = mce_mns->max_pkt_len;

    return 0;
}

static uint8_t bt_map_convert_result(uint16_t event_result)
{
    uint8_t result = (uint8_t)event_result;

    switch (event_result)
    {
        case BT_MAP_RSP_CONTINUE:
        case BT_MAP_RSP_SUCCESS:
        case BT_MAP_RSP_BAD_REQ:
        case BT_MAP_RSP_NOT_IMPLEMENTED:
        case BT_MAP_RSP_UNAUTH:
        case BT_MAP_RSP_PRECOND_FAILED:
        case BT_MAP_RSP_NOT_FOUND:
        case BT_MAP_RSP_NOT_ACCEPTABLE:
        case BT_MAP_RSP_SERVICE_UNAVBL:
        case BT_MAP_RSP_FORBIDDEN:
        case BT_MAP_RSP_INT_SERVER_ERR:
            break;
        case API_SUCCESS:
            result = BT_MAP_RSP_SUCCESS;
            break;
        default:
            result = BT_MAP_RSP_BAD_REQ;
            break;
    }

    return result;
}

static API_RESULT map_mce_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{

    API_RESULT retval = API_SUCCESS;;
    struct bt_map_mce_mas *mce_mas;
    struct bt_map_mce_mns *mce_mns;
    MAP_HANDLE handle;
    struct net_buf *buf;
    struct bt_conn *conn;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    LOG_DBG ("\n\nMAP MCE Event: 0x%02X Result: 0x%04X\n\n", event_type, event_result);
    handle = map_handle->entity_id;

    if (event_result == MAP_OPERATION_ABORTED)
    {
        if (event_type == MAP_MCE_NS_EVENT_REPORT_IND)
        {
            mce_mns = map_mce_mns_lookup_instance(handle);
            if (mce_mns != NULL)
            {
                mce_mns->flag = BT_OBEX_REQ_UNSEG;
            }
            return retval;
        }
        else
        {
            if (event_type != MAP_ABORT_IND)
            {
                mce_mas = map_mce_mas_lookup_instance(handle);
                if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->abort != NULL))
                {
                    mce_mas_cb->abort(mce_mas, BT_MAP_RSP_SUCCESS);
                }
                return retval;
            }
        }
    }

    event_result = bt_map_convert_result(event_result);

    switch (event_type)
    {
    case MAP_MCE_GET_FOLDER_LISTING_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_FOLDER_LISTING_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_folder_listing != NULL))
        {
            mce_mas_cb->get_folder_listing(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MCE_SET_FOLDER_CNF:
        LOG_DBG ("Recvd MAP_MCE_SET_FOLDER_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->set_folder != NULL))
        {
            mce_mas_cb->set_folder(mce_mas, event_result);
        }
        break;

    case MAP_MCE_GET_MESSAGES_LISTING_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_MESSAGES_LISTING_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_msg_listing != NULL))
        {
            mce_mas_cb->get_msg_listing(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MCE_GET_MESSAGE_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_MESSAGE_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_msg != NULL))
        {
            mce_mas_cb->get_msg(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MCE_SET_MESSAGE_STATUS_CNF:
        LOG_DBG ("Recvd MAP_MCE_SET_MESSAGE_STATUS_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->set_msg_status != NULL))
        {
            mce_mas_cb->set_msg_status(mce_mas, event_result);
        }
        break;

    case MAP_MCE_PUSH_MESSAGE_CNF:
        LOG_DBG ("Recvd MAP_MCE_PUSH_MESSAGE_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->push_msg != NULL))
        {
            char *name;
            if (event_header->map_req_info->name->length == 0U)
            {
                name = NULL;
            }
            else
            {
                name = (char *)event_header->map_req_info->name->value;
                name[event_header->map_req_info->name->length - 1U] = '\0';
            }
            mce_mas_cb->push_msg(mce_mas, event_result, name);
        }
        break;

    case MAP_MCE_SET_NTF_REGISTRATION_CNF:
        LOG_DBG ("Recvd MAP_MCE_SET_NTF_REGISTRATION_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->set_ntf_reg != NULL))
        {
            mce_mas_cb->set_ntf_reg(mce_mas, event_result);
        }
        break;

    case MAP_MCE_UPDATE_INBOX_CNF:
        LOG_DBG ("Recvd MAP_MCE_UPDATE_INBOX_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->update_inbox != NULL))
        {
            mce_mas_cb->update_inbox(mce_mas, event_result);
        }
        break;

#ifdef MAP_1_2
    case MAP_MCE_GET_MASINSTINFO_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_MASINSTINFO_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_mas_inst_info != NULL))
        {
            mce_mas_cb->get_mas_inst_info(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;
#endif /* MAP_1_2 */

    case MAP_MCE_SET_OWNER_STATUS_CNF:
        LOG_DBG ("Recvd MAP_MCE_SET_OWNER_STATUS_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->set_owner_status != NULL))
        {
            mce_mas_cb->set_owner_status(mce_mas, event_result);
        }
        break;

    case MAP_MCE_GET_OWNER_STATUS_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_OWNER_STATUS_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_owner_status != NULL))
        {
            mce_mas_cb->get_owner_status(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

#ifdef MAP_1_3
    case MAP_MCE_GET_CONVERSATION_LIST_CNF:
        LOG_DBG ("Recvd MAP_MCE_GET_CONVERSATION_LIST_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mas_cb != NULL) && (mce_mas_cb->get_convo_listing != NULL))
        {
            mce_mas_cb->get_convo_listing(mce_mas, event_result, buf);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MCE_SET_NOTIFICATION_FILTER_CNF:
        LOG_DBG ("Recvd MAP_MCE_SET_NOTIFICATION_FILTER_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->set_ntf_filter != NULL))
        {
            mce_mas_cb->set_ntf_filter(mce_mas, event_result);
        }
        break;
#endif /* MAP_1_3 */

    case MAP_MCE_CONNECT_CNF:
        LOG_DBG ("Recvd MAP_MCE_CONNECT_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas != NULL)
        {
            if (event_result != BT_MAP_RSP_SUCCESS)
            {
                if ((mce_mas_cb != NULL) && (mce_mas_cb->disconnected != NULL))
                {
                    mce_mas_cb->disconnected(mce_mas, event_result);
                }
                BT_map_mce_stop(MAP_ACCESS_SERVICE, &mce_mas->handle);
                map_mce_mas_free_instance(mce_mas);
            }
            else
            {
                mce_mas->max_pkt_len = event_header->map_connect_info->max_recv_size + 3U + 5U + 4U; /* Subtract 12 in stack and add 12 back here. */
                if ((mce_mas_cb != NULL) && (mce_mas_cb->connected != NULL))
                {
                    mce_mas_cb->connected(mce_mas);
                }
            }
        }
        break;

    case MAP_MCE_DISCONNECT_CNF:
        LOG_DBG ("Recvd MAP_MCE_DISCONNECT_CNF - 0x%04X\n", event_result);
        /* Transport close has been sent in the underlying layer so don't need call BT_map_mce_transport_close. */
        /* mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas != NULL)
        {
            BT_map_mce_transport_close(&mce_mas->handle);
        } */
        break;

    case MAP_MCE_TRANSPORT_CLOSE_CNF:
        LOG_DBG ("Recvd MAP_MCE_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas != NULL)
        {
            if ((mce_mas_cb != NULL) && (mce_mas_cb->disconnected != NULL))
            {
                mce_mas_cb->disconnected(mce_mas, event_result);
            }
            BT_map_mce_stop(MAP_ACCESS_SERVICE, &mce_mas->handle);
            map_mce_mas_free_instance(mce_mas);
        }
        break;

    case MAP_MCE_TRANSPORT_CLOSE_IND:
        LOG_DBG ("Recvd MAP_MCE_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if (mce_mas != NULL)
        {
            if ((mce_mas_cb != NULL) && (mce_mas_cb->disconnected != NULL))
            {
                mce_mas_cb->disconnected(mce_mas, event_result);
            }
            BT_map_mce_stop(MAP_ACCESS_SERVICE, &mce_mas->handle);
            map_mce_mas_free_instance(mce_mas);
        }
        break;

    case MAP_ABORT_CNF:
        LOG_DBG ("Recvd MAP_ABORT_CNF - 0x%04X\n", event_result);
        mce_mas = map_mce_mas_lookup_instance(handle);
        if ((mce_mas != NULL) && (mce_mas_cb != NULL) && (mce_mas_cb->abort != NULL))
        {
            mce_mas_cb->abort(mce_mas, event_result);
        }
        break;

#ifdef MAP_SUPPORT_NOTIFICATION
    case MAP_MCE_NS_TRANSPORT_CLOSE_CNF:
        LOG_DBG ("Recvd MAP_MCE_MNS_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        mce_mns = map_mce_mns_lookup_instance(handle);
        if (mce_mns != NULL)
        {
            if ((mce_mns_cb != NULL) && (mce_mns_cb->disconnected != NULL))
            {
                mce_mns_cb->disconnected(mce_mns, event_result);
            }
            map_mce_mns_free_instance(mce_mns);
        }
        break;

    case MAP_MCE_NS_TRANSPORT_CLOSE_IND:
        LOG_DBG ("Recvd MAP_MCE_MNS_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        mce_mns = map_mce_mns_lookup_instance(handle);
        if (mce_mns != NULL)
        {
            if ((mce_mns_cb != NULL) && (mce_mns_cb->disconnected != NULL))
            {
                mce_mns_cb->disconnected(mce_mns, event_result);
            }
            map_mce_mns_free_instance(mce_mns);
        }
        break;

    case MAP_MCE_NS_DISCONNECT_IND:
        LOG_DBG ("Recvd MAP_MCE_MNS_DISCONNECT_IND - 0x%04X\n", event_result);
        mce_mns = map_mce_mns_lookup_instance(handle);
        if (mce_mns != NULL)
        {
            /* Response has been sent in the underlying layer so don't need call BT_map_mce_ns_send_response. */
            /* BT_map_mce_ns_send_response(&mce_mns->handle, event_type, (UCHAR)event_result, NULL); */
            event_result = BT_map_mce_ns_transport_close(&mce_mns->handle);
            if ((event_result != API_SUCCESS) && (mce_mns_cb != NULL) && (mce_mns_cb->disconnected != NULL))
            {
                mce_mns_cb->disconnected(mce_mns, BT_MAP_RSP_SUCCESS);
                map_mce_mns_free_instance(mce_mns);
            }
        }
        break;

    case MAP_MCE_NS_CONNECT_IND:
        LOG_DBG ("Recvd MAP_MCE_MNS_CONNECT_IND - %04X\n", event_result);
        mce_mns = map_mce_mns_lookup_instance(handle);
        if (mce_mns == NULL)
        {
            if (event_header->map_connect_info == NULL)
            {
                break;
            }
            conn = bt_conn_lookup_addr_br((const bt_addr_t *)event_header->map_connect_info->bd_addr);
            if (conn == NULL)
            {
                break;
            }
            mce_mns = map_mce_mns_get_instance(conn);
            if (mce_mns == NULL)
            {
                break;
            }

            event_result = (event_result != BT_MAP_RSP_SUCCESS) ? BT_MAP_RSP_NOT_ACCEPTABLE : event_result;

            if ((BT_map_mce_ns_send_response(&handle, event_type, (UCHAR)event_result, NULL) == API_SUCCESS) &&
                (event_result == BT_MAP_RSP_SUCCESS))
            {
                mce_mns->handle = handle;
                mce_mns->max_pkt_len = event_header->map_connect_info->max_recv_size + 3U + 17U; /* Subtract 20 in stack and add 20 back here. */
                mce_mns->flag = BT_OBEX_REQ_UNSEG;
                if ((mce_mns_cb != NULL) && (mce_mns_cb->connected != NULL))
                {
                    mce_mns_cb->connected(mce_mns);
                }
            }
            else
            {
                map_mce_mns_free_instance(mce_mns);
            }
        }
        break;

    case MAP_MCE_NS_EVENT_REPORT_IND:
        LOG_DBG ("Recvd MAP_MCE_MNS_EVENT_REPORT_IND - 0x%04X\n", event_result);
        mce_mns = map_mce_mns_lookup_instance(handle);
        if (mce_mns == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mce_mns_rx_pool, osaWaitForever_c);
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
            if (mce_mns->flag & BT_OBEX_REQ_END)
            {
                mce_mns->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mce_mns->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_resp_info->body->value != NULL) && (event_header->map_resp_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_resp_info->body->value, event_header->map_resp_info->body->length);
            }
            if (mce_mns->flag & BT_OBEX_REQ_END)
            {
                mce_mns->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mce_mns->flag = BT_OBEX_REQ_END;
            }
        }
        else
        {
            /* no action */
        }
        if ((mce_mns_cb != NULL) && (mce_mns_cb->send_event != NULL))
        {
            mce_mns_cb->send_event(mce_mns, buf, mce_mns->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;
#endif /* MAP_SUPPORT_NOTIFICATION */

    default:
        LOG_DBG ("Unknown MAP MCE Event\n");
        break;
    }

    return retval;
}

#endif /* CONFIG_BT_MAP_MCE */
#endif /* CONFIG_BT_CLASSIC */
