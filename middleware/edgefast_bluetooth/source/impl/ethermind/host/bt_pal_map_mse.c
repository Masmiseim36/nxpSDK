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
#include <bluetooth/map_mse.h>
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

#if ((defined(CONFIG_BT_MAP_MSE)) && (CONFIG_BT_MAP_MSE > 0U))

#if (CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES > MAP_MAS_NUM_ENTITIES)
#error "CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES should be less than or equal to MAP_MAS_NUM_ENTITIES"
#endif

/* MAP MSE MAS RX buffer count */
#define BT_MAP_MSE_MAS_RX_NET_BUF_COUNT (1U)
/* MAP MSE MAS RX buffer size */
#define BT_MAP_MSE_MAS_RX_NET_BUF_SIZE (CONFIG_BT_MAP_MSE_MAS_MAX_PKT_LEN)
/* MAP MSE MNS RX buffer count */
#define BT_MAP_MSE_MNS_RX_NET_BUF_COUNT (1U)
/* MAP MSE MNS RX buffer size */
#define BT_MAP_MSE_MNS_RX_NET_BUF_SIZE (CONFIG_BT_MAP_MSE_MNS_MAX_PKT_LEN)

#define EDGEFAST_MAP_MSE_LOCK   OSA_MutexLock(s_MapMseLock, osaWaitForever_c)
#define EDGEFAST_MAP_MSE_UNLOCK OSA_MutexUnlock(s_MapMseLock)

/** @brief MAP MSE MAS structure */
struct bt_map_mse_mas
{
    struct bt_conn *acl_conn;
    uint16_t max_pkt_len;
    uint8_t handle;
    enum bt_obex_req_flags flag;
};

/** @brief MAP MSE MNS structure */
struct bt_map_mse_mns
{
    struct bt_conn *acl_conn;
    uint32_t supported_features;
    uint16_t max_pkt_len;
    uint8_t handle;
};

static API_RESULT map_mse_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

static OSA_MUTEX_HANDLE_DEFINE(s_MapMseLockMutex);
static osa_mutex_handle_t s_MapMseLock;

static struct bt_map_mse_mas map_mse_mas_instances[CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES];
static struct bt_map_mse_mns map_mse_mns_instances[CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES];
static struct bt_map_mse_mas_cb *mse_mas_cb;
static struct bt_map_mse_mns_cb *mse_mns_cb;
static uint8_t mse_init = 0;

NET_BUF_POOL_FIXED_DEFINE(mse_mas_rx_pool, BT_MAP_MSE_MAS_RX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(BT_MAP_MSE_MAS_RX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static struct bt_map_mse_mas *map_mse_mas_get_instance(struct bt_conn * conn)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; ++index)
    {
        if (map_mse_mas_instances[index].acl_conn == NULL)
        {
            memset(&map_mse_mas_instances[index], 0U, sizeof(map_mse_mas_instances[index]));
            map_mse_mas_instances[index].acl_conn = conn;
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static struct bt_map_mse_mns *map_mse_mns_get_instance(struct bt_conn * conn)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; ++index)
    {
        if (map_mse_mns_instances[index].acl_conn == NULL)
        {
            memset(&map_mse_mns_instances[index], 0U, sizeof(map_mse_mns_instances[index]));
            map_mse_mns_instances[index].acl_conn = conn;
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static void map_mse_mas_free_instance(struct bt_map_mse_mas *map_mse_mas)
{
    if (NULL != map_mse_mas)
    {
        EDGEFAST_MAP_MSE_LOCK;
        map_mse_mas->acl_conn = NULL;
        EDGEFAST_MAP_MSE_UNLOCK;
    }
}

static void map_mse_mns_free_instance(struct bt_map_mse_mns *map_mse_mns)
{
    if (NULL != map_mse_mns)
    {
        EDGEFAST_MAP_MSE_LOCK;
        map_mse_mns->acl_conn = NULL;
        EDGEFAST_MAP_MSE_UNLOCK;
    }
}

static struct bt_map_mse_mas *map_mse_mas_lookup_instance(MAP_HANDLE handle)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; ++index)
    {
        if ((map_mse_mas_instances[index].acl_conn != NULL) && (map_mse_mas_instances[index].handle == handle))
        {
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static struct bt_map_mse_mns *map_mse_mns_lookup_instance(MAP_HANDLE handle)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; ++index)
    {
        if ((map_mse_mns_instances[index].acl_conn != NULL) && (map_mse_mns_instances[index].handle == handle))
        {
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static struct bt_map_mse_mas *map_mse_mas_lookup_instance_by_addr(uint8_t *bd_addr)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; ++index)
    {
        if ((map_mse_mas_instances[index].acl_conn != NULL) &&
            (memcmp(&map_mse_mas_instances[index].acl_conn->br.dst, bd_addr, BT_BD_ADDR_SIZE) == 0))
        {
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mas_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static struct bt_map_mse_mns *map_mse_mns_lookup_instance_by_addr(uint8_t *bd_addr)
{
    EDGEFAST_MAP_MSE_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; ++index)
    {
        if ((map_mse_mns_instances[index].acl_conn != NULL) &&
            (memcmp(&map_mse_mns_instances[index].acl_conn->br.dst, bd_addr, BT_BD_ADDR_SIZE) == 0))
        {
            EDGEFAST_MAP_MSE_UNLOCK;
            return &map_mse_mns_instances[index];
        }
    }

    EDGEFAST_MAP_MSE_UNLOCK;
    return NULL;
}

static int bt_map_copy_tag_from_buf_to_stack(struct bt_obex_tag_bytes *tag, MAP_APPL_PARAMS *appl_param)
{
    switch (tag->id)
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
            appl_param->new_message = *tag->value;
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            return -EINVAL;
        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            appl_param->mas_instance_id = *tag->value;
            break;
        case BT_MAP_TAG_ID_PARAMETER_MASK:
            return -EINVAL;
        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
            appl_param->folder_listing_size = sys_get_be16(tag->value);
            break;
        case BT_MAP_TAG_ID_LISTING_SIZE:
            appl_param->messages_listing_size = sys_get_be16(tag->value);
            break;
        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
        case BT_MAP_TAG_ID_CHARSET:
        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            return -EINVAL;
        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            appl_param->fraction_deliver = *tag->value;
            break;
        case BT_MAP_TAG_ID_STATUS_INDICATOR:
        case BT_MAP_TAG_ID_STATUS_VALUE:
            return -EINVAL;
        case BT_MAP_TAG_ID_MSE_TIME:
            appl_param->mse_time.length = tag->length;
            appl_param->mse_time.value = tag->value;
            break;
#ifdef MAP_1_3
        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
            appl_param->database_identifier.length = tag->length;
            appl_param->database_identifier.value = tag->value;
            break;
        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            appl_param->conv_listing_ver_cntr.length = tag->length;
            appl_param->conv_listing_ver_cntr.value = tag->value;
            break;
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
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            return -EINVAL;
        case BT_MAP_TAG_ID_CHAT_STATE:
            appl_param->chat_state = *tag->value;
            break;
        case BT_MAP_TAG_ID_CONVERSATION_ID:
            return -EINVAL;
        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            appl_param->folder_ver_cntr.length = tag->length;
            appl_param->folder_ver_cntr.value = tag->value;
            break;
        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            return -EINVAL;
        case BT_MAP_TAG_ID_OWNER_UCI:
            appl_param->owner_uci.length = tag->length;
            appl_param->owner_uci.value = tag->value;
            break;
        case BT_MAP_TAG_ID_EXTENDED_DATA:
            return -EINVAL;
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            appl_param->supported_features = sys_get_be32(tag->value);
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
            BT_MAP_ADD_MAX_LIST_COUNT(buf, appl_param->max_list_count);
            break;
        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            BT_MAP_ADD_LIST_START_OFFSET(buf, appl_param->start_offset);
            break;
        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
            BT_MAP_ADD_FILTER_MESSAGE_TYPE(buf, appl_param->filter_message_type);
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
            BT_MAP_ADD_FILTER_PERIOD_BEGIN(buf, appl_param->filter_period_begin.value, appl_param->filter_period_begin.length);
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
            BT_MAP_ADD_FILTER_PERIOD_END(buf, appl_param->filter_period_end.value, appl_param->filter_period_end.length);
            break;
        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
            BT_MAP_ADD_FILTER_READ_STATUS(buf, appl_param->filter_read_status);
            break;
        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
            BT_MAP_ADD_FILTER_RECIPIENT(buf, appl_param->filter_recipient.value, appl_param->filter_recipient.length);
            break;
        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
            BT_MAP_ADD_FILTER_ORIGINATOR(buf, appl_param->filter_originator.value, appl_param->filter_originator.length);
            break;
        case BT_MAP_TAG_ID_FILTER_PRIORITY:
            BT_MAP_ADD_FILTER_PRIORITY(buf, appl_param->filter_priority);
            break;
        case BT_MAP_TAG_ID_ATTACHMENT:
            BT_MAP_ADD_ATTACHMENT(buf, appl_param->attachment);
            break;
        case BT_MAP_TAG_ID_TRANSPARENT:
            BT_MAP_ADD_TRANSPARENT(buf, appl_param->transparent);
            break;
        case BT_MAP_TAG_ID_RETRY:
            BT_MAP_ADD_RETRY(buf, appl_param->retry);
            break;
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            return -EINVAL;
        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            BT_MAP_ADD_NOTIFICATION_STATUS(buf, appl_param->notification_status);
            break;
        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            BT_MAP_ADD_MAS_INSTANCE_ID(buf, appl_param->mas_instance_id);
            break;
        case BT_MAP_TAG_ID_PARAMETER_MASK:
            BT_MAP_ADD_PARAMETER_MASK(buf, appl_param->parameter_mask);
            break;
        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
        case BT_MAP_TAG_ID_LISTING_SIZE:
            return -EINVAL;
        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
            BT_MAP_ADD_SUBJECT_LENGTH(buf, appl_param->subject_length);
            break;
        case BT_MAP_TAG_ID_CHARSET:
            BT_MAP_ADD_CHARSET(buf, appl_param->charset);
            break;
        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            BT_MAP_ADD_FRACTION_REQUEST(buf, appl_param->fraction_request);
            break;
        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            return -EINVAL;
        case BT_MAP_TAG_ID_STATUS_INDICATOR:
            BT_MAP_ADD_STATUS_INDICATOR(buf, appl_param->status_indicator);
            break;
        case BT_MAP_TAG_ID_STATUS_VALUE:
            BT_MAP_ADD_STATUS_VALUE(buf, appl_param->status_value);
            break;
        case BT_MAP_TAG_ID_MSE_TIME:
            return -EINVAL;
#ifdef MAP_1_3
        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            return -EINVAL;
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
            BT_MAP_ADD_FILTER_LAST_ACTIVITY_BEGIN(buf, appl_param->filter_last_activity_begin.value, appl_param->filter_last_activity_begin.length);
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            BT_MAP_ADD_FILTER_LAST_ACTIVITY_END(buf, appl_param->filter_last_activity_end.value, appl_param->filter_last_activity_end.length);
            break;
        case BT_MAP_TAG_ID_CHAT_STATE:
            BT_MAP_ADD_CHAT_STATE(buf, appl_param->chat_state);
            break;
        case BT_MAP_TAG_ID_CONVERSATION_ID:
            BT_MAP_ADD_CONVERSATION_ID(buf, appl_param->conversation_id.value, appl_param->conversation_id.length);
            break;
        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            return -EINVAL;
        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
            BT_MAP_ADD_FILTER_MSG_HANDLE(buf, appl_param->filter_msg_handle.value, appl_param->filter_msg_handle.length);
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
            BT_MAP_ADD_NOTIFICATION_FILTER_MASK(buf, appl_param->ntf_filter_mask);
            break;
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            BT_MAP_ADD_CONV_PARAMETER_MASK(buf, appl_param->conv_parameter_mask);
            break;
        case BT_MAP_TAG_ID_OWNER_UCI:
            return -EINVAL;
        case BT_MAP_TAG_ID_EXTENDED_DATA:
            BT_MAP_ADD_EXTENDED_DATA(buf, appl_param->extended_data.value, appl_param->extended_data.length);
            break;
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            BT_MAP_ADD_MAP_SUPPORTED_FEATURES(buf, appl_param->supported_features);
            break;
#endif /* MAP_1_3 */
#ifdef MAP_1_4
        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
            BT_MAP_ADD_MESSAGE_HANDLE(buf, appl_param->msg_handle.value, appl_param->msg_handle.length);
            break;
        case BT_MAP_TAG_ID_MODIFY_TEXT:
            BT_MAP_ADD_MODIFY_TEXT(buf, appl_param->modify_text);
            break;
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

static int map_mse_init(void)
{
    UCHAR handle;
    if (mse_init == 0)
    {
        BT_map_mse_init();
        for (uint8_t index = 0U; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; index++)
        {
            map_mse_mas_instances[index].acl_conn = NULL;
        }
        for (uint8_t index = 0U; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; index++)
        {
            map_mse_mns_instances[index].acl_conn = NULL;
        }
        for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; index++)
        {
            BT_map_mse_start(MAP_ACCESS_SERVICE, map_mse_callback, &handle);
        }
        if (NULL == s_MapMseLock)
        {
            if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_MapMseLockMutex))
            {
                s_MapMseLock = (osa_mutex_handle_t)s_MapMseLockMutex;
            }
            else
            {
                return -EIO;
            }
        }
    }
    mse_init++;

    return 0;
}

static int map_mse_deinit(void)
{
    mse_init--;

    if (mse_init == 0)
    {
        for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; index++)
        {
            BT_map_mse_stop(MAP_ACCESS_SERVICE, &index);
        }
        BT_map_mse_shutdown();
        if (NULL != s_MapMseLock)
        {
            OSA_MutexDestroy((osa_mutex_handle_t)s_MapMseLockMutex);
            s_MapMseLock = NULL;
        }
    }

    return 0;
}

int bt_map_mse_mas_register(struct bt_map_mse_mas_cb *cb)
{
    int err;

	if (!cb) {
		return -EINVAL;
	}

	if (mse_mas_cb) {
		return -EALREADY;
	}

    err = map_mse_init();
    if (err != 0)
    {
        return err;
    }

    mse_mas_cb = cb;
    return 0;
}

int bt_map_mse_mns_register(struct bt_map_mse_mns_cb *cb)
{
    int err;

	if (!cb) {
		return -EINVAL;
	}

	if (mse_mns_cb) {
		return -EALREADY;
	}

    err = map_mse_init();
    if (err != 0)
    {
        return err;
    }

    mse_mns_cb = cb;
    return 0;
}

int bt_map_mse_mas_unregister(void)
{
    if (mse_mas_cb != NULL)
    {
        map_mse_deinit();
        mse_mas_cb = NULL;
    }
    return 0;
}

int bt_map_mse_mns_unregister(void)
{
    if (mse_mns_cb != NULL)
    {
        map_mse_deinit();
        mse_mns_cb = NULL;
    }
    return 0;
}

/** @brief Create MSE MNS connection
 *
 *  This function is to be called after MCE sets SetNotificationRegistration to ON.
 *  The API is to be used to establish MNS OBEX connection between devices.
 *  This function establishes L2CAP or RFCOM connection.
 *  This function should not directly used by application. Instead, application
 *  should use the macro of bt_map_mse_psm_connect or bt_map_mse_scn_connect to
 *  create L2CAP or RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_map_mse_mns_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param psm  GoepL2capPsm, returned in SDP record.
 *  @param scn  RFCOM server channel number, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *  @param mse_mns  MSE MNS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
static int bt_map_mse_mns_connect(struct bt_conn *conn, uint16_t psm, uint8_t scn, uint32_t supported_features, struct bt_map_mse_mns **mse_mns)
{
    MAP_CONNECT_STRUCT connect_info;
    struct bt_conn_info info;
    struct bt_map_mse_mns *_mse_mns;
#ifdef MAP_USE_NET_BUF
    uint16_t reserve = sizeof(struct map_mse_connect_hdr);
    uint16_t pkt_len = sizeof(struct map_mse_connect_hdr);
    uint8_t uuid[] = MAP_MNS_TARGET_UUID;
    struct net_buf *buf;
#endif

    if ((conn == NULL) || (mse_mns == NULL))
    {
        return -EINVAL;
    }

    *mse_mns = NULL;

    /* Create MNS instance */
    _mse_mns = map_mse_mns_get_instance(conn);
    if (_mse_mns == NULL)
    {
        return -EAGAIN;
    }
    if (BT_map_mse_start(MAP_NTF_SERVICE, map_mse_callback, &_mse_mns->handle) != API_SUCCESS)
    {
        map_mse_mns_free_instance(_mse_mns);
        return -EAGAIN;
    }

    *mse_mns = _mse_mns;

    (void)memset(&info, 0, sizeof(info));
    (void)memset(&connect_info, 0, sizeof(connect_info));

    bt_conn_get_info(conn, &info);

    connect_info.bd_addr = (UCHAR *)&info.br.dst->val[0];
    connect_info.psm = psm;
    connect_info.server_channel = scn;
    connect_info.max_recv_size = CONFIG_BT_MAP_MSE_MNS_MAX_PKT_LEN;

#ifdef MAP_USE_NET_BUF
    reserve += sizeof(struct bt_l2cap_hdr) + 2U; /* L2CAP I-frame Enhanced Control Field(2-byte) */
    buf = bt_conn_create_pdu(NULL, reserve);
    bt_map_push_hdr(buf, BT_MAP_HDR_TARGET, uuid, sizeof(uuid));
    net_buf_push_be16(buf, CONFIG_BT_MAP_MSE_MNS_MAX_PKT_LEN);
    net_buf_push_u8(buf, 0x00); /* flags */
    net_buf_push_u8(buf, 0x00); /* TODO: fill OBEX Version Number */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, OBEX_CONNECT_OP);
    net_buf_unref(buf);
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mse_ns_connect(&_mse_mns->handle, &connect_info) != API_SUCCESS)
    {
        map_mse_mns_free_instance(_mse_mns);
        return -EIO;
    }
    _mse_mns->supported_features = supported_features;

    return 0;
}

int bt_map_mse_mns_psm_connect(struct bt_conn *conn, uint16_t psm, uint32_t supported_features, struct bt_map_mse_mns **mse_mns)
{
    return bt_map_mse_mns_connect(conn, psm, 0, supported_features, mse_mns);
}

int bt_map_mse_mns_scn_connect(struct bt_conn *conn, uint8_t scn, uint32_t supported_features, struct bt_map_mse_mns **mse_mns)
{
    return bt_map_mse_mns_connect(conn, 0, scn, supported_features, mse_mns);
}

int bt_map_mse_mns_disconnect(struct bt_map_mse_mns *mse_mns)
{
    if (mse_mns == NULL)
    {
        return -EINVAL;
    }

    if (BT_map_mse_ns_disconnect(&mse_mns->handle) != API_SUCCESS)
    {
        return -EIO;
    }
    return 0;
}


int bt_map_mse_disconnect(struct bt_map_mse_mas *mse_mas)
{
    if (mse_mas == NULL)
    {
        return -EINVAL;
    }

    if (BT_map_mse_transport_close(&mse_mas->handle) != API_SUCCESS)
    {
        return -EIO;
    }
    return 0;
}

/** @brief Send response when receiving request from MCE.
 *
 *  This function is to send response when receiving request from MCE.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param result BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
 *  @param event_type the last event type returned by map_mse_callback.
 *  @param name  return message handle to MCE
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
static int bt_map_mse_send_response(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, uint8_t result, uint8_t event_type, char *name, bool wait)
{
    int err;
    MAP_HEADERS map_header;
    MAP_RESPONSE_STRUCT rsp_info;
    MAP_APPL_PARAMS appl_param;
    MAP_HEADER_STRUCT body_req;
    MAP_HEADER_STRUCT name_req;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE;

    if (mse_mas == NULL)
    {
        return -EINVAL;
    }

    BT_mem_set(&rsp_info, 0, sizeof(MAP_REQUEST_STRUCT));
    BT_mem_set(&map_header, 0, sizeof(map_header));
    if (buf != NULL)
    {
        MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
        err = bt_map_copy_appl_param_from_buf_to_stack(buf, &appl_param, &pkt_len);
        if (err == 0)
        {
            rsp_info.appl_params = &appl_param;
        }
        else
        {
            if (err != -EAGAIN)
            {
                return err;
            }
        }
    }
    if (name != NULL)
    {
        name_req.value = (uint8_t *)name;
        name_req.length = strlen(name) + 1U;
        rsp_info.name = &name_req;
    }
    if (bt_map_mse_get_body(buf, &body_req.value, &body_req.length) == 0)
    {
        rsp_info.body = &body_req;
    }
    rsp_info.wait = wait;
    map_header.map_resp_info = &rsp_info;

#ifdef MAP_USE_NET_BUF
#ifdef MAP_1_2
    if (mse_mas->goep_version >= BT_GOEP_VERSION_2_0)
    {
        if (wait)
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRMP, 1U);
            pkt_len += BT_MAP_SRMP_SIZE;
        }
        if ((!mse_mas->lcl_srm) && (result == BT_MAP_RSP_CONTINUE))
        {
            bt_map_push_hdr_u8(buf, OBEX_HDR_SRM, 1U);
            pkt_len += BT_MAP_SRM_SIZE;
        }
    }
#endif /* MAP_1_2 */
    net_buf_push_be16(buf, pkt_len);
    net_buf_push_u8(buf, result);
#else
    (void)pkt_len;
#endif /* MAP_USE_NET_BUF */

    if (BT_map_mse_send_response(&mse_mas->handle, event_type, result, &map_header) != API_SUCCESS)
    {
        return -EIO;
    }

    if (result != BT_MAP_RSP_CONTINUE)
    {
        mse_mas->flag = BT_OBEX_REQ_UNSEG;
    }

    if (buf != NULL)
    {
        net_buf_unref(buf);
    }

    return 0;
}

int bt_map_mse_get_folder_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_FOLDER_LISTING_IND, NULL, wait);
}

int bt_map_mse_set_folder_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_SET_FOLDER_IND, NULL, false);
}

int bt_map_mse_get_msg_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_MESSAGES_LISTING_IND, NULL, wait);
}

int bt_map_mse_get_msg_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_MESSAGE_IND, NULL, wait);
}

int bt_map_mse_set_msg_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_SET_MESSAGE_STATUS_IND, NULL, false);
}

int bt_map_mse_push_msg_response(struct bt_map_mse_mas *mse_mas, uint8_t result, char *name, bool wait)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_PUSH_MESSAGE_IND, name, wait);
}

int bt_map_mse_set_ntf_reg_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_SET_NTF_REGISTRATION_IND, NULL, false);
}

int bt_map_mse_update_inbox_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_UPDATE_INBOX_IND, NULL, false);
}

int bt_map_mse_get_mas_inst_info_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_MASINSTINFO_IND, NULL, wait);
}

int bt_map_mse_set_owner_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_SET_OWNER_STATUS_IND, NULL, false);
}

int bt_map_mse_get_owner_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_OWNER_STATUS_IND, NULL, wait);
}

int bt_map_mse_get_convo_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait)
{
    return bt_map_mse_send_response(mse_mas, buf, result, MAP_MSE_GET_CONVERSATION_LIST_IND, NULL, wait);
}

int bt_map_mse_set_ntf_filter_response(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    return bt_map_mse_send_response(mse_mas, NULL, result, MAP_MSE_SET_NOTIFICATION_FILTER_IND, NULL, false);
}

int bt_map_mse_send_event(struct bt_map_mse_mns *mse_mns, struct net_buf *buf, enum bt_obex_req_flags flags)
{
    int err;
    MAP_HEADERS map_header;
    MAP_REQUEST_STRUCT set_info;
    MAP_HEADER_STRUCT body_req;
    MAP_APPL_PARAMS appl_param;
    uint8_t more;
    uint16_t actual;
    uint16_t pkt_len = BT_MAP_OPCODE_SIZE + BT_MAP_PACKET_LENGTH_SIZE + BT_MAP_CONNECTION_ID_SIZE;

    if (mse_mns == NULL)
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
        bt_map_push_hdr(buf, BT_MAP_HDR_TYPE, BT_MAP_TYPE_SEND_EVENT, sizeof(BT_MAP_TYPE_SEND_EVENT));
        pkt_len += BT_MAP_HEADER_SIZE + sizeof(BT_MAP_TYPE_SEND_EVENT);
#ifdef MAP_1_2
        if (mse_mas->goep_version >= BT_GOEP_VERSION_2_0)
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
        if (BT_map_mse_ns_send_event(&mse_mns->handle, &set_info, more) != API_SUCCESS)
        {
            return -EIO;
        }
    }
    else
    {
        BT_mem_set(&map_header, 0, sizeof(map_header));
        map_header.map_req_info = &set_info;
        if (BT_map_mse_ns_send_request
            (
                &mse_mns->handle,
                MAP_MSE_NS_SEND_EVENT_CNF,
                BT_MAP_RSP_SUCCESS,
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

int bt_map_mse_get_max_pkt_len(struct bt_map_mse_mas *mse_mas, uint16_t *max_pkt_len)
{
    if ((mse_mas == NULL) || (mse_mas->acl_conn == NULL))
    {
        return -EINVAL;
    }
    *max_pkt_len = mse_mas->max_pkt_len;

    return 0;
}

int bt_map_mse_mns_get_max_pkt_len(struct bt_map_mse_mns *mse_mns, uint16_t *max_pkt_len)
{
    if ((mse_mns == NULL) || (mse_mns->acl_conn == NULL))
    {
        return -EINVAL;
    }
    *max_pkt_len = mse_mns->max_pkt_len;

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

static API_RESULT map_mse_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{

    API_RESULT retval = API_SUCCESS;;
    struct bt_map_mse_mas *mse_mas;
    struct bt_map_mse_mns *mse_mns;
    MAP_HANDLE handle;
    struct net_buf *buf;
    struct bt_conn *conn;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    LOG_DBG ("\n\nMAP MSE Event: 0x%02X Result: 0x%04X\n\n", event_type, event_result);
    handle = map_handle->entity_id;

    if (event_result == MAP_OPERATION_ABORTED)
    {
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas != NULL)
        {
            mse_mas->flag = BT_OBEX_REQ_UNSEG;
            if ((mse_mas_cb != NULL) && (mse_mas_cb->abort != NULL))
            {
                mse_mas_cb->abort(mse_mas);
            }
        }
        return retval;
    }

    event_result = bt_map_convert_result(event_result);

    switch (event_type)
    {
    case MAP_MSE_GET_FOLDER_LISTING_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_FOLDER_LISTING_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_resp_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_folder_listing != NULL))
        {
            mse_mas_cb->get_folder_listing(mse_mas, buf, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_SET_FOLDER_IND:
        LOG_DBG ("Recvd MAP_MSE_SET_FOLDER_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        switch(event_header->map_req_info->setpath_flag)
        {
            case 0x02U:
                if ((event_header->map_req_info->name == NULL) ||
                    (event_header->map_req_info->name->length == 0U))
                {
                    net_buf_add_mem(buf, "/", sizeof("/"));
                }
                else if((event_header->map_req_info->name != NULL) &&
                        (event_header->map_req_info->name->value != NULL) &&
                        (event_header->map_req_info->name->length > 0U))
                {
                    if (net_buf_tailroom(buf) >= event_header->map_req_info->name->length)
                    {
                        net_buf_add_mem(buf, event_header->map_req_info->name->value, event_header->map_req_info->name->length - 1U);
                        net_buf_add_u8(buf, (uint8_t)'\0');
                    }
                    else
                    {
                        event_result = BT_MAP_RSP_NOT_ACCEPTABLE;
                    }
                }
                else
                {
                    event_result = BT_MAP_RSP_BAD_REQ;
                }
                break;

            case 0x03U:
                if ((event_header->map_req_info->name == NULL) ||
                    (event_header->map_req_info->name->length == 0U))
                {
                    net_buf_add_mem(buf, "../", sizeof("../"));
                }
                else if((event_header->map_req_info->name != NULL) &&
                        (event_header->map_req_info->name->value != NULL) &&
                        (event_header->map_req_info->name->length > 0U))
                {
                    net_buf_add_mem(buf, "../", sizeof("../") - 1U);
                    if (net_buf_tailroom(buf) >= event_header->map_req_info->name->length)
                    {
                        net_buf_add_mem(buf, event_header->map_req_info->name->value, event_header->map_req_info->name->length - 1U);
                        net_buf_add_u8(buf, (uint8_t)'\0');
                    }
                    else
                    {
                        event_result = BT_MAP_RSP_NOT_ACCEPTABLE;
                    }
                }
                else
                {
                    event_result = BT_MAP_RSP_BAD_REQ;
                }
                break;

            default:;
                event_result = BT_MAP_RSP_BAD_REQ;
                break;
        }
        if (event_result != BT_MAP_RSP_SUCCESS)
        {
            BT_map_mse_send_response(&handle, event_type, (UCHAR)event_result, NULL);
        }
        else
        {
            if ((mse_mas_cb != NULL) && (mse_mas_cb->set_folder != NULL))
            {
                mse_mas_cb->set_folder(mse_mas, (char *)buf->data);
            }
        }
        net_buf_unref(buf);
        break;

    case MAP_MSE_GET_MESSAGES_LISTING_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_MESSAGES_LISTING_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_msg_listing != NULL))
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
            mse_mas_cb->get_msg_listing(mse_mas, buf, name, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_GET_MESSAGE_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_MESSAGE_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_msg != NULL))
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
            mse_mas_cb->get_msg(mse_mas, buf, name, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_SET_MESSAGE_STATUS_IND:
        LOG_DBG ("Recvd MAP_MSE_SET_MESSAGE_STATUS_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if (MAP_CONTINUE_RSP == event_result)
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_END;
            }
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->set_msg_status != NULL))
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
            mse_mas_cb->set_msg_status(mse_mas, buf, name, mse_mas->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_PUSH_MESSAGE_IND:
        LOG_DBG ("Recvd MAP_MSE_PUSH_MESSAGE_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if (event_result == BT_MAP_RSP_CONTINUE)
        {
            if ((event_header->map_req_info->body->value != NULL) && (event_header->map_req_info->body->length != 0U))
            {
                BT_MAP_ADD_BODY(buf, event_header->map_req_info->body->value, event_header->map_req_info->body->length);
            }
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else if (event_result == BT_MAP_RSP_SUCCESS)
        {
            if ((event_header->map_req_info->body->value != NULL) && (event_header->map_req_info->body->length != 0U))
            {
                BT_MAP_ADD_END_OF_BODY(buf, event_header->map_req_info->body->value, event_header->map_req_info->body->length);
            }
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_END;
            }
        }
        else
        {
            /* no action */
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->push_msg != NULL))
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
            mse_mas_cb->push_msg(mse_mas, buf, name, mse_mas->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_SET_NTF_REGISTRATION_IND:
        LOG_DBG ("Recvd MAP_MSE_SET_NTF_REGISTRATION_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if (MAP_CONTINUE_RSP == event_result)
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_END;
            }
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->set_ntf_reg != NULL))
        {
            mse_mas_cb->set_ntf_reg(mse_mas, buf, mse_mas->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_UPDATE_INBOX_IND:
        LOG_DBG ("Recvd MAP_MSE_UPDATE_INBOX_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->update_inbox != NULL))
        {
            mse_mas_cb->update_inbox(mse_mas);
        }
        break;

#ifdef MAP_1_2
    case MAP_MSE_GET_MASINSTINFO_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_MASINSTINFO_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_mas_inst_info != NULL))
        {
            mse_mas_cb->get_mas_inst_info(mse_mas, buf, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;
#endif /* MAP_1_2 */

    case MAP_MSE_SET_OWNER_STATUS_IND:
        LOG_DBG ("Recvd MAP_MSE_SET_OWNER_STATUS_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if (MAP_CONTINUE_RSP == event_result)
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_END;
            }
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->set_owner_status != NULL))
        {
            mse_mas_cb->set_owner_status(mse_mas, buf, mse_mas->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_GET_OWNER_STATUS_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_OWNER_STATUS_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_owner_status != NULL))
        {
            mse_mas_cb->get_owner_status(mse_mas, buf, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

#ifdef MAP_1_3
    case MAP_MSE_GET_CONVERSATION_LIST_IND:
        LOG_DBG ("Recvd MAP_MSE_GET_CONVERSATION_LIST_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if ((mse_mas_cb != NULL) && (mse_mas_cb->get_convo_listing != NULL))
        {
            mse_mas_cb->get_convo_listing(mse_mas, buf, BT_OBEX_REQ_UNSEG);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;

    case MAP_MSE_SET_NOTIFICATION_FILTER_IND:
        LOG_DBG ("Recvd MAP_MSE_SET_NOTIFICATION_FILTER_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
        {
            break;
        }
        buf = net_buf_alloc(&mse_mas_rx_pool, osaWaitForever_c);
        if (buf == NULL)
        {
            break;
        }
        bt_map_copy_appl_param_from_stack_to_buf(buf, event_header->map_req_info->appl_params);
        if (MAP_CONTINUE_RSP == event_result)
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_START;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_CONTINUE;
            }
        }
        else
        {
            if (mse_mas->flag & BT_OBEX_REQ_END)
            {
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
            }
            else
            {
                mse_mas->flag = BT_OBEX_REQ_END;
            }
        }
        if ((mse_mas_cb != NULL) && (mse_mas_cb->set_ntf_filter != NULL))
        {
            mse_mas_cb->set_ntf_filter(mse_mas, buf, mse_mas->flag);
        }
        else
        {
            net_buf_unref(buf);
        }
        break;
#endif /* MAP_1_3 */

    case MAP_MSE_CONNECT_IND:
        LOG_DBG ("Recvd MAP_MSE_CONNECT_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas == NULL)
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
            mse_mas = map_mse_mas_get_instance(conn);
            if (mse_mas == NULL)
            {
                break;
            }

            event_result = (event_result != BT_MAP_RSP_SUCCESS) ? BT_MAP_RSP_NOT_ACCEPTABLE : event_result;

            if ((BT_map_mse_send_response(&handle, event_type, (UCHAR)event_result, NULL) == API_SUCCESS) &&
                (event_result == BT_MAP_RSP_SUCCESS))
            {
                UINT32 sdp_record_handle;
                uint16_t psm = 0;
                uint8_t scn = 0;

                mse_mas->handle = handle;
                mse_mas->max_pkt_len = event_header->map_connect_info->max_recv_size + 3U + 17U; /* Subtract 20 in stack and add 20 back here. */
                mse_mas->flag = BT_OBEX_REQ_UNSEG;
                BT_dbase_get_record_handle(DB_RECORD_MAP_MSE, handle, &sdp_record_handle);
                BT_dbase_get_server_channel(sdp_record_handle, PROTOCOL_DESC_LIST, &scn);
                BT_dbase_get_obex_psm(sdp_record_handle, GOEP_L2CAP_PSM_ATTR_ID, &psm);
                if ((mse_mas_cb != NULL) && (mse_mas_cb->connected != NULL))
                {
                    mse_mas_cb->connected(mse_mas, psm, scn);
                }
            }
            else
            {
                map_mse_mas_free_instance(mse_mas);
            }
        }
        break;

    case MAP_MSE_DISCONNECT_IND:
        LOG_DBG ("Recvd MAP_MSE_DISCONNECT_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas != NULL)
        {
            event_result = BT_map_mse_transport_close(&mse_mas->handle);
            if ((event_result != API_SUCCESS) && (mse_mas_cb != NULL) && (mse_mas_cb->disconnected != NULL))
            {
                mse_mas_cb->disconnected(mse_mas, BT_MAP_RSP_SUCCESS);
                map_mse_mas_free_instance(mse_mas);
            }
        }
        break;

    case MAP_MSE_TRANSPORT_CLOSE_CNF:
        LOG_DBG ("Recvd MAP_MSE_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas != NULL)
        {
            if ((mse_mas_cb != NULL) && (mse_mas_cb->disconnected != NULL))
            {
                mse_mas_cb->disconnected(mse_mas, event_result);
            }
            map_mse_mas_free_instance(mse_mas);
        }
        break;

    case MAP_MSE_TRANSPORT_CLOSE_IND:
        LOG_DBG ("Recvd MAP_MSE_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if (mse_mas != NULL)
        {
            if ((mse_mas_cb != NULL) && (mse_mas_cb->disconnected != NULL))
            {
                mse_mas_cb->disconnected(mse_mas, event_result);
            }
            map_mse_mas_free_instance(mse_mas);
        }
        break;

    case MAP_ABORT_IND:
        LOG_DBG ("Recvd MAP_ABORT_IND - 0x%04X\n", event_result);
        mse_mas = map_mse_mas_lookup_instance(handle);
        if ((mse_mas != NULL) && (mse_mas_cb != NULL) && (mse_mas_cb->abort != NULL))
        {
            mse_mas_cb->abort(mse_mas);
        }
        break;

#ifdef MAP_SUPPORT_NOTIFICATION
    case MAP_MSE_NS_TRANSPORT_CLOSE_CNF:
        LOG_DBG ("Recvd MAP_MSE_MNS_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        mse_mns = map_mse_mns_lookup_instance(handle);
        if (mse_mns != NULL)
        {
            if ((mse_mns_cb != NULL) && (mse_mns_cb->disconnected != NULL))
            {
                mse_mns_cb->disconnected(mse_mns, event_result);
            }
            BT_map_mse_stop(MAP_NTF_SERVICE, &mse_mns->handle);
            map_mse_mns_free_instance(mse_mns);
        }
        break;

    case MAP_MSE_NS_TRANSPORT_CLOSE_IND:
        LOG_DBG ("Recvd MAP_MSE_MNS_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        mse_mns = map_mse_mns_lookup_instance(handle);
        if (mse_mns != NULL)
        {
            if ((mse_mns_cb != NULL) && (mse_mns_cb->disconnected != NULL))
            {
                mse_mns_cb->disconnected(mse_mns, event_result);
            }
            BT_map_mse_stop(MAP_NTF_SERVICE, &mse_mns->handle);
            map_mse_mns_free_instance(mse_mns);
        }
        break;

    case MAP_MSE_NS_DISCONNECT_CNF:
        LOG_DBG ("Recvd MAP_MSE_MNS_DISCONNECT_CNF - 0x%04X\n", event_result);
        /* Transport close has been sent in the underlying layer so don't need call BT_map_mse_ns_transport_close. */
        /* mse_mns = map_mse_mns_lookup_instance(handle);
        if (mse_mns != NULL)
        {
            BT_map_mse_ns_transport_close(&mse_mns->handle);
        } */
        break;

    case MAP_MSE_NS_CONNECT_CNF:
        LOG_DBG ("Recvd MAP_MSE_MNS_CONNECT_CNF - 0x%04X\n", event_result);
        mse_mns = map_mse_mns_lookup_instance(handle);
        if (mse_mns != NULL)
        {
            if (event_result != BT_MAP_RSP_SUCCESS)
            {
                if ((mse_mns_cb != NULL) && (mse_mns_cb->disconnected != NULL))
                {
                    mse_mns_cb->disconnected(mse_mns, event_result);
                }
                BT_map_mse_stop(MAP_NTF_SERVICE, &mse_mns->handle);
                map_mse_mns_free_instance(mse_mns);
            }
            else
            {
                mse_mns->max_pkt_len = event_header->map_connect_info->max_recv_size + 3U + 5U + 4U; /* Subtract 12 in stack and add 12 back here. */
                if ((mse_mns_cb != NULL) && (mse_mns_cb->connected != NULL))
                {
                    mse_mns_cb->connected(mse_mns);
                }
            }
        }
        break;

    case MAP_MSE_NS_SEND_EVENT_CNF:
        LOG_DBG ("Recvd MAP_MSE_MNS_SEND_EVENT_CNF - 0x%04X\n", event_result);
        mse_mns = map_mse_mns_lookup_instance(handle);
        if (mse_mns == NULL)
        {
            break;
        }
        if ((mse_mns_cb != NULL) && (mse_mns_cb->send_event != NULL))
        {
            mse_mns_cb->send_event(mse_mns, event_result);
        }
        break;
#endif /* MAP_SUPPORT_NOTIFICATION */

    default:
        LOG_DBG ("Unknown MAP MSE Event\n");
        break;
    }

    return retval;
}

#endif /* CONFIG_BT_MAP_MSE */
#endif /* CONFIG_BT_CLASSIC */
