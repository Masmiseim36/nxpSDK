/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))

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
#include <bluetooth/avrcp.h>
#include <bluetooth/sdp.h>
#include "bt_pal_conn_internal.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "bt_pal_avrcp_al_api.h"
#ifdef AVRCP_COVER_ART
#include "BT_avrcp_ca_api.h"
#endif /* AVRCP_COVER_ART */

#if (defined(CONFIG_BT_AVRCP) && ((CONFIG_BT_AVRCP) > 0U))

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
static int bt_avrcp_parse_vendor_cmd_internal(struct bt_avrcp_vendor *vendor_cmd);
#endif
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
static int bt_avrcp_parse_vendor_rsp_internal(struct bt_avrcp_vendor *vendor_rsp,
    struct bt_avrcp_vendor *vendor_parse,
    uint8_t *replaced);
#endif

struct bt_avrcp_instance
{
    AVRCP_HANDLE ethermind_avrcp;
    struct bt_conn *conn;
    uint8_t connect_initiator : 1;
    uint8_t control_connected : 1;
    uint8_t browsing_connected : 1;
};

struct bt_avrcp_cb avrcp_cbs;
struct bt_avrcp_instance avrcp_instances[CONFIG_BT_AVRCP_MAX_CONN];
static OSA_MUTEX_HANDLE_DEFINE(avrcp_lock_mutex);
static osa_mutex_handle_t avrcp_lock;

#define AVRCP_LOCK   OSA_MutexLock(avrcp_lock, osaWaitForever_c)
#define AVRCP_UNLOCK OSA_MutexUnlock(avrcp_lock)

#define LOG_ENABLE      IS_ENABLED(CONFIG_BT_DEBUG_AVRCP)
#define LOG_MODULE_NAME bt_avrcp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

/* the control channel's max transfer data length is 512 bytes */
#define TRANSFER_DATA_BUFF_SIZE (512u)
#define PARSE_DATA_BUFF_SIZE    (1024u)
uint8_t send_data[TRANSFER_DATA_BUFF_SIZE];
#if (PARSE_DATA_BUFF_SIZE < ((AVCTP_DEFAULT_BROW_CH_L2CAP_MTU - 16) * 2u))
#error "please increase PARSE_DATA_BUFF_SIZE"
#endif
uint32_t parse_data[PARSE_DATA_BUFF_SIZE];

#define AVRCP_PLAYER_ITEM_LEN (31u)
#define AVRCP_FOLDER_ITEM_LEN (17u)
#define AVRCP_MEDIA_ITEM_LEN  (16u)

struct bt_avrcp_browsing_cmd *bt_avrcp_parse_browsing_cmd_data(uint8_t *data, uint32_t len);
struct bt_avrcp_browsing_rsp *bt_avrcp_parse_browsing_rsp_data(uint8_t *data, uint32_t len);

static struct bt_conn *get_conn;
static void avrcp_get_conn(struct bt_conn *conn, void *data)
{
    uint8_t *addr = (uint8_t *)data;

    if (memcmp(&conn->br.dst, addr, 6u) == 0u)
    {
        get_conn = conn;
    }
}

static struct bt_avrcp_instance *bt_avrcp_get_instance(struct bt_conn *conn)
{
    for (uint8_t index = 0; index < (sizeof(avrcp_instances) / sizeof(avrcp_instances[0])); index++)
    {
        if (avrcp_instances[index].conn == conn)
        {
            return &avrcp_instances[index];
        }
    }

    return NULL;
}

static struct bt_avrcp_instance *bt_avrcp_alloc_instance(struct bt_conn *conn)
{
    for (uint8_t index = 0; index < (sizeof(avrcp_instances) / sizeof(avrcp_instances[0])); index++)
    {
        if (avrcp_instances[index].conn == NULL)
        {
            memset(&avrcp_instances[index], 0, sizeof(avrcp_instances[0]));
            avrcp_instances[index].conn = conn;
            return &avrcp_instances[index];
        }
    }

    return NULL;
}

static void bt_avrcp_free_instance(struct bt_avrcp_instance *avrcp)
{
    avrcp->conn = NULL;
}

static struct bt_avrcp_instance *bt_avrcp_cb_get_instance(AVRCP_HANDLE *avrcp_handle)
{
    for (uint8_t index = 0; index < (sizeof(avrcp_instances) / sizeof(avrcp_instances[0])); index++)
    {
        if (avrcp_instances[index].ethermind_avrcp.profile_inst == avrcp_handle->profile_inst)
        {
            return &avrcp_instances[index];
        }
    }

    return NULL;
}

static API_RESULT ethermind_avrcp_cb(
    /* IN */ UCHAR event_type,
    /* IN */ UINT16 event_result,
    /* IN */ AVRCP_HANDLE *avrcp_handle,
    /* IN */ void *event_data,
    /* IN */ UINT16 event_datalen)
{
    API_RESULT ret = API_SUCCESS;
    struct bt_avrcp_instance *avrcp;

    if (AVRCP_CONNECT_IND != event_type) {
        avrcp = bt_avrcp_cb_get_instance(avrcp_handle);
    } else {
        get_conn = NULL;
        bt_conn_foreach(BT_CONN_TYPE_BR, avrcp_get_conn, &avrcp_handle->bd_addr[0]);
        if (get_conn == NULL) {
            return ret;
        }
        avrcp = bt_avrcp_alloc_instance(get_conn);
    }

    if (avrcp == NULL)
    {
        return ret;
    }

    AVRCP_LOCK;

    switch (event_type)
    {
        case AVRCP_CONNECT_CNF:
            if (event_result == API_SUCCESS)
            {
                if (avrcp_cbs.control_connected != NULL) {
                    avrcp_cbs.control_connected(avrcp->conn, 0);
                }
            }
            else
            {
                if (avrcp->connect_initiator)
                {
                    bt_avrcp_free_instance(avrcp);
                }

                if (avrcp_cbs.control_connected != NULL) {
                    avrcp_cbs.control_connected(avrcp->conn, -1);
                }
            }
            break;

        case AVRCP_CONNECT_IND:
            if (event_result == API_SUCCESS)
            {
                avrcp->ethermind_avrcp = *avrcp_handle;
                avrcp->conn            = NULL;
                get_conn = NULL;
                bt_conn_foreach(BT_CONN_TYPE_BR, avrcp_get_conn, &avrcp->ethermind_avrcp.bd_addr[0]);
                avrcp->conn = get_conn;

                if ((avrcp->conn != NULL) && (avrcp_cbs.control_connected != NULL))
                {
                    avrcp_cbs.control_connected(avrcp->conn, 0);
                }
            }
            break;

        case AVRCP_DISCONNECT_CNF:
        case AVRCP_DISCONNECT_IND:
            AVRCP_RESET_HANDLE_BD_ADDR(avrcp->ethermind_avrcp);
            if (avrcp_cbs.control_disconnected != NULL) {
                avrcp_cbs.control_disconnected(avrcp->conn, 0);
            }
            avrcp->conn = NULL;
            break;

        case AVRCP_BOW_CONNECT_IND:
            if (event_result == API_SUCCESS)
            {
                avrcp->browsing_connected = 1u;
                if (avrcp_cbs.browsing_connected) {
                    avrcp_cbs.browsing_connected(avrcp->conn, 0);
                }
            }
            break;

        case AVRCP_BOW_CONNECT_CNF:
            if (event_result == API_SUCCESS)
            {
                avrcp->browsing_connected = 1u;
                if (avrcp_cbs.browsing_connected) {
                    avrcp_cbs.browsing_connected(avrcp->conn, 0);
                }
            }
            else
            {
                if (avrcp_cbs.browsing_connected) {
                    avrcp_cbs.browsing_connected(avrcp->conn, -1);
                }
            }
            break;

        case AVRCP_BOW_DISCONNECT_IND:
        case AVRCP_BOW_DISCONNECT_CNF:
            AVRCP_RESET_HANDLE_BD_ADDR(avrcp->ethermind_avrcp);
            if (avrcp_cbs.browsing_disconnected) {
                avrcp_cbs.browsing_disconnected(avrcp->conn, 0);
            }
            avrcp->conn = NULL;
            break;

        case AVRCP_MESSAGE_SEND_CNF:
            /* Free Allocated Memory */
            if (event_data != NULL)
            {
                OSA_MemoryFree(event_data);
            }

            if (avrcp_cbs.send_result != NULL)
            {
                if (event_result == API_SUCCESS)
                {
                    avrcp_cbs.send_result(avrcp->conn, 0);
                }
                else
                {
                    avrcp_cbs.send_result(avrcp->conn, -1);
                }
            }
            break;

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
        case AVRCP_METADATA_CMD:
        case AVRCP_MESSAGE_IND:
        {
            struct bt_avrcp_control_msg *msg = (struct bt_avrcp_control_msg *)(event_data);

            BT_DBG("AVRCP_MESSAGE_IND");
            // todo: check event_datalen is right or not..

            if (msg->header.op_code == BT_AVRCP_OPCODE_VENDOR_DEPENDENT)
            {
                msg->vendor.parameter_len = sys_get_be16((uint8_t *)&msg->vendor.parameter_len);
                bt_avrcp_parse_vendor_cmd_internal(&msg->vendor);
            }
            if (avrcp_cbs.control_received != NULL) {
                avrcp_cbs.control_received(avrcp->conn, msg, event_result);
            }
            break;
        }
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
        case AVRCP_MESSAGE_CNF:
        case AVRCP_METADATA_INTERIM_RSP:
        case AVRCP_METADATA_RSP:
        {
            if (event_result == API_SUCCESS)
            {
                uint8_t replaced = 0;
                struct bt_avrcp_control_msg *msg = (struct bt_avrcp_control_msg *)event_data;

                BT_DBG("AVRCP_MESSAGE_CNF");
                // todo: check event_datalen is right or not..

                if (msg->header.op_code == BT_AVRCP_OPCODE_VENDOR_DEPENDENT)
                {
                    msg->vendor.parameter_len = sys_get_be16((uint8_t *)&msg->vendor.parameter_len);

                    struct bt_avrcp_control_msg *msg_parse = (struct bt_avrcp_control_msg *)&parse_data[0];
                    memcpy(msg_parse, msg, sizeof(struct bt_avrcp_header) + 7u);
                    bt_avrcp_parse_vendor_rsp_internal(&msg->vendor, &msg_parse->vendor, &replaced);
                    if (replaced)
                    {
                        msg = msg_parse;
                    }
                }
                if (avrcp_cbs.control_rsp_received != NULL) {
                    avrcp_cbs.control_rsp_received(avrcp->conn, msg, 0);
                }
            }
            else
            {
                if (avrcp_cbs.control_rsp_received != NULL) {
                    avrcp_cbs.control_rsp_received(avrcp->conn, NULL, -1);
                }
            }
            break;
        }
#endif

        case AVRCP_BOW_MESSAGE_IND:
        {
            if (avrcp_cbs.browsing_received != NULL)
            {
                BT_DBG("AVRCP_BOW_MESSAGE_IND");
                // todo: check event_datalen is right or not..

                avrcp_cbs.browsing_received(avrcp->conn, bt_avrcp_parse_browsing_cmd_data(event_data, event_datalen),
                                            event_result);
            }
            break;
        }

        case AVRCP_BOW_MESSAGE_RSP:
        {
            if (avrcp_cbs.browsing_rsp_received != NULL)
            {
                BT_DBG("AVRCP_BOW_MESSAGE_RSP");
                // todo: check event_datalen is right or not..

                avrcp_cbs.browsing_rsp_received(
                    avrcp->conn, bt_avrcp_parse_browsing_rsp_data(event_data, event_datalen), event_result);
            }
            break;
        }

        default:
            break;
    }

    AVRCP_UNLOCK;

    return ret;
}

static API_RESULT ethermind_avrcp_cai_cb(AVRCP_CA_HANDLE *avrcp_ca_handle,
                                         UINT8 event_type,
                                         UINT16 event_result,
                                         AVRCP_CA_HEADERS *avrcp_ca_headers,
                                         UINT16 num_headers);
API_RESULT ethermind_avrcp_car_cb(AVRCP_CA_HANDLE *avrcp_ca_handle,
                                  UINT8 event_type,
                                  UINT16 event_result,
                                  AVRCP_CA_HEADERS *ca_headers,
                                  UINT16 num_headers);

int bt_avrcp_init(void)
{
    API_RESULT ret;

    if (NULL == avrcp_lock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)avrcp_lock_mutex))
        {
            avrcp_lock = (osa_mutex_handle_t)avrcp_lock_mutex;
        }
        else
        {
            BT_ERR("fail to create mutex");
            return -EIO;
        }
    }

    BT_DBG("initlaize avrcp");
    AVRCP_LOCK;
    ret = BT_avrcp_al_init();
    if (ret != API_SUCCESS)
    {
        AVRCP_UNLOCK;
        BT_ERR("fail to initialize avrcp");
        return -EIO;
    }

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))
    BT_DBG("initlaize cover art initiator");
    ret = BT_avrcp_cai_init();
    if (ret != API_SUCCESS)
    {
        AVRCP_UNLOCK;
        BT_ERR("fail to initialize cover art initiator");
        return -EIO;
    }
#endif
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))
    BT_DBG("initlaize cover art responder");
    ret = BT_avrcp_car_init();
    if (ret != API_SUCCESS)
    {
        AVRCP_UNLOCK;
        BT_ERR("fail to initialize cover art responder");
        return -EIO;
    }
#endif
#endif

    AVRCP_UNLOCK;
    return 0;
}

int bt_avrcp_register_callback(struct bt_avrcp_cb *cb)
{
    API_RESULT ret;

    AVRCP_LOCK;
    avrcp_cbs = *cb;

    for (uint8_t index = 0; index < CONFIG_BT_AVRCP_MAX_CONN; index++)
    {
        ret = BT_avrcp_start(&avrcp_instances[index].ethermind_avrcp, ethermind_avrcp_cb);
        if (ret != API_SUCCESS)
        {
            AVRCP_UNLOCK;
            BT_ERR("fail to start avrcp");
            return -EIO;
        }
    }

    AVRCP_UNLOCK;
    return 0;
}

int bt_avrcp_control_connect(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_conn_info info;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    (void)memset(&info, 0, sizeof(info));
    bt_conn_get_info(conn, &info);
    if (info.type == BT_CONN_TYPE_LE)
    {
        return -EINVAL;
    }

    if (avrcp == NULL)
    {
        avrcp = bt_avrcp_alloc_instance(conn);
        if (avrcp == NULL)
        {
            return -EINVAL;
        }
        avrcp->connect_initiator = 1;
    }

    ret = BT_avrcp_connect_req(&avrcp->ethermind_avrcp, (uint8_t *)info.br.dst);
    if (ret != API_SUCCESS)
    {
        if (avrcp->connect_initiator != 0)
        {
            bt_avrcp_free_instance(avrcp);
        }
        BT_ERR("fail to connect");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_control_disconnect(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_disconnect_req(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to disconnect control channel")
        return -EINVAL;
    }

    return 0;
}

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
int bt_avrcp_response_info(struct bt_conn *conn, uint8_t subunit, uint8_t subunit_type, uint8_t tl)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);
    uint8_t tl_msg_type             = (tl << 4u) | AVCTP_CTR_MSG_RESPONSE;

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_info_rsp(&avrcp->ethermind_avrcp, tl_msg_type, subunit, subunit_type);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_info_rsp");
        return -EIO;
    }

    return 0;
}

/* send or response */
int bt_avrcp_response_passthrough(struct bt_conn *conn, uint8_t rsp_type, struct bt_avrcp_control_msg *cmd)
{
    API_RESULT ret;
    uint16_t vendor_op_id;
    uint8_t tl_msg_type             = (cmd->header.tl << 4u) | AVCTP_CTR_MSG_RESPONSE;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    vendor_op_id = 0;
    if (cmd->pass_th.op_id == BT_AVRCP_OP_ID_VENDOR_UNIQUE)
    {
        vendor_op_id = sys_get_be16(&cmd->pass_th.op_data[3]);
    }

    ret = BT_avrcp_al_send_passthrough_cmd_rsp(&avrcp->ethermind_avrcp, tl_msg_type, rsp_type, cmd->pass_th.op_id,
                                               cmd->pass_th.state_flag, vendor_op_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_passthrough_cmd_rsp");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_response_vendor_dependent(
    struct bt_conn *conn, uint8_t pdu_id, uint8_t tl, uint8_t response_type, void *rsp_param, uint16_t param_len)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);
    AVRCP_AL_VD_CMD_INFO vd_rsp_info;
    struct net_buf data_buf;
    struct net_buf *buf;

    data_buf.__buf = &send_data[0];
    data_buf.data  = &send_data[0];
    data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
    data_buf.len   = 0;
    buf            = &data_buf;

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    vd_rsp_info.tl             = (tl << 4);
    vd_rsp_info.pdu_id         = pdu_id;
    vd_rsp_info.packet_type    = BT_AVRCP_PACKET_TYPE_SINGLE;
    vd_rsp_info.cmd_type       = response_type;
    vd_rsp_info.vd_cmd_data    = rsp_param;
    vd_rsp_info.vd_cmd_datalen = param_len;

    if ((response_type == BT_AVRCP_RESPONSE_TYPE_REJECTED) || (response_type == BT_AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED))
    {
        vd_rsp_info.vd_cmd_data    = rsp_param;
        vd_rsp_info.vd_cmd_datalen = param_len;

        ret = BT_avrcp_al_send_metadata_pdu(&avrcp->ethermind_avrcp, &vd_rsp_info, 0x01);
        if (ret != API_SUCCESS)
        {
            BT_ERR("fail to call BT_avrcp_al_send_metadata_pdu");
            return -EIO;
        }
        return 0;
    }

    switch (pdu_id)
    {
        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
        {
            /* the data is byte mapped */
            vd_rsp_info.vd_cmd_data    = rsp_param;
            vd_rsp_info.vd_cmd_datalen = param_len;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_player_get_txt_rsp *rsp = (struct bt_avrcp_player_get_txt_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->num_of_id);
            for (uint8_t index = 0; index < rsp->num_of_id; index++)
            {
                net_buf_add_u8(buf, rsp->texts[index].attr_value_id);
                net_buf_add_be16(buf, rsp->texts[index].char_set);
                net_buf_add_u8(buf, rsp->texts[index].string_len);
                net_buf_add_mem(buf, rsp->texts[index].string, rsp->texts[index].string_len);
            }

            vd_rsp_info.vd_cmd_data    = buf->data;
            vd_rsp_info.vd_cmd_datalen = buf->len;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct bt_avrcp_player_get_element_attr_rsp *rsp = (struct bt_avrcp_player_get_element_attr_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->num_of_attr);
            for (uint8_t index = 0; index < rsp->num_of_attr; index++)
            {
                net_buf_add_be32(buf, rsp->attrs[index].attr_id);
                net_buf_add_be16(buf, rsp->attrs[index].char_set);
                net_buf_add_be16(buf, rsp->attrs[index].string_len);
                net_buf_add_mem(buf, rsp->attrs[index].string, rsp->attrs[index].string_len);
            }
            vd_rsp_info.vd_cmd_data    = buf->data;
            vd_rsp_info.vd_cmd_datalen = buf->len;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        {
            struct bt_avrcp_play_status_rsp *rsp = (struct bt_avrcp_play_status_rsp *)rsp_param;
            ;

            net_buf_add_be32(buf, rsp->song_length);
            net_buf_add_be32(buf, rsp->song_pos);
            net_buf_add_u8(buf, rsp->play_status);
            vd_rsp_info.vd_cmd_data    = buf->data;
            vd_rsp_info.vd_cmd_datalen = buf->len;
            break;
        }
        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
        {
            struct bt_avrcp_event_rsp *rsp = (struct bt_avrcp_event_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->event_id);
            switch (rsp->event_id)
            {
                case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
                    net_buf_add_u8(buf, rsp->play_status);
                    break;

                case BT_AVRCP_EVENT_TRACK_CHANGED:
                    net_buf_add_mem(buf, rsp->identifier, 8u);
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_END:
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_START:
                    break;

                case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
                    net_buf_add_be32(buf, rsp->playback_pos);
                    break;

                case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
                    net_buf_add_u8(buf, rsp->battery_status);
                    break;

                case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
                    net_buf_add_u8(buf, rsp->system_status);
                    break;

                case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
                    net_buf_add_u8(buf, rsp->setting_changed.num_of_attr);
                    for (uint8_t index = 0; index < rsp->setting_changed.num_of_attr; index++)
                    {
                        net_buf_add_u8(buf, rsp->setting_changed.attr_vals[index].attr_id);
                        net_buf_add_u8(buf, rsp->setting_changed.attr_vals[index].value_id);
                    }
                    break;

#ifdef AVRCP_1_4
                case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
                    break;

                case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
                    break;

                case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
                    net_buf_add_be16(buf, rsp->addressed_player_changed.player_id);
                    net_buf_add_be16(buf, rsp->addressed_player_changed.uid_counter);
                    break;

                case BT_AVRCP_EVENT_UIDS_CHANGED:
                    net_buf_add_be16(buf, rsp->uid_counter);
                    break;

                case BT_AVRCP_EVENT_VOLUME_CHANGED:
                    net_buf_add_u8(buf, rsp->absolute_volume);
                    break;

#endif /* AVRCP_1_4 */
                default:
                    break;
            }
            vd_rsp_info.vd_cmd_data    = buf->data;
            vd_rsp_info.vd_cmd_datalen = buf->len;
            break;
        }
        default:
            break;
    }

    ret = BT_avrcp_al_send_metadata_pdu(&avrcp->ethermind_avrcp, &vd_rsp_info, 0x01);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_metadata_pdu");
        return -EIO;
    }

    return 0;
}

static int bt_avrcp_parse_vendor_cmd_internal(struct bt_avrcp_vendor *vendor_cmd)
{
    if (vendor_cmd->pdu_id == BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET)
    {
        struct net_buf data_buf;
        struct net_buf *buf;
        struct bt_avrcp_inform_displayable_char_set *cmd = &vendor_cmd->inform_char_set;

        data_buf.__buf = &vendor_cmd->parameter;
        data_buf.data  = &vendor_cmd->parameter;
        data_buf.size  = vendor_cmd->parameter_len;
        data_buf.len   = vendor_cmd->parameter_len;
        buf            = &data_buf;

        cmd->num_of_char = net_buf_pull_u8(buf);
        for (uint8_t index = 0; index < cmd->num_of_char; index++)
        {
            cmd->char_sets[index] = net_buf_pull_be16(buf);
        }
    }
    else if (vendor_cmd->pdu_id == BT_AVRCP_PDU_ID_PLAY_ITEMS)
    {
        struct bt_avrcp_play_item *play = &vendor_cmd->play_item;
        play->uid_counter = sys_get_be16((uint8_t *)&play->uid_counter);
    }
    else if (vendor_cmd->pdu_id == BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING)
    {
        struct bt_avrcp_add_to_now_playing *play = &vendor_cmd->add_now_play;
        play->uid_counter                        = sys_get_be16((uint8_t *)&play->uid_counter);
    }
    else if (vendor_cmd->pdu_id == BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION)
    {
        struct bt_avrcp_register_ntfy *reg = &vendor_cmd->register_notify;
        reg->playback_interval = sys_get_be32((uint8_t *)&reg->playback_interval);
    }

    return 0;
}

#endif /* CONFIG_BT_AVRCP_TG */

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
static int bt_avrcp_parse_vendor_rsp_internal(struct bt_avrcp_vendor *vendor_rsp,
    struct bt_avrcp_vendor *vendor_parse,
    uint8_t *replaced)
{
    struct net_buf data_buf;
    struct net_buf *buf;

    data_buf.__buf = &vendor_rsp->parameter;
    data_buf.data  = &vendor_rsp->parameter;
    data_buf.size  = vendor_rsp->parameter_len;
    data_buf.len   = vendor_rsp->parameter_len;
    buf            = &data_buf;

    switch (vendor_rsp->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        {
            /* the data is byte mapped */
            *replaced = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_player_get_txt_rsp *rsp = &vendor_parse->get_txt_rsp;

            *replaced = 1;
            rsp->num_of_id                          = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < rsp->num_of_id; index++)
            {
                rsp->texts[index].attr_value_id = net_buf_pull_u8(buf);
                rsp->texts[index].char_set      = net_buf_pull_be16(buf);
                rsp->texts[index].string_len    = net_buf_pull_u8(buf);
                rsp->texts[index].string        = net_buf_pull_mem(buf, rsp->texts[index].string_len);
            }
            break;
        }
        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct bt_avrcp_player_get_element_attr_rsp *rsp =
                &vendor_parse->element_attr_rsp;

            *replaced = 1;
            rsp->num_of_attr = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < rsp->num_of_attr; index++)
            {
                rsp->attrs[index].attr_id    = net_buf_pull_be32(buf);
                rsp->attrs[index].char_set   = net_buf_pull_be16(buf);
                rsp->attrs[index].string_len = net_buf_pull_be16(buf);
                rsp->attrs[index].string     = net_buf_pull_mem(buf, rsp->attrs[index].string_len);
            }
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        {
            struct bt_avrcp_play_status_rsp *rsp = &vendor_parse->play_status_rsp;

            *replaced = 1;
            rsp->song_length                     = net_buf_pull_be32(buf);
            rsp->song_pos                        = net_buf_pull_be32(buf);
            rsp->play_status                     = net_buf_pull_u8(buf);
            break;
        }
        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
        {
            struct bt_avrcp_event_rsp *rsp = &vendor_parse->event_rsp;

            *replaced = 1;
            rsp->event_id                            = net_buf_pull_u8(buf);
            switch (rsp->event_id)
            {
                case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
                    rsp->play_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_TRACK_CHANGED:
                    memcpy(rsp->identifier, buf->data, 8u);
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_END:
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_START:
                    break;

                case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
                    break;

                case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
                    rsp->battery_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
                    rsp->system_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
                    rsp->setting_changed.num_of_attr = net_buf_pull_u8(buf);
                    for (uint8_t index = 0; index < rsp->setting_changed.num_of_attr; index++)
                    {
                        rsp->setting_changed.attr_vals[index].attr_id  = net_buf_pull_u8(buf);
                        rsp->setting_changed.attr_vals[index].value_id = net_buf_pull_u8(buf);
                    }
                    break;

#ifdef AVRCP_1_4
                case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
                    break;

                case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
                    break;

                case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
                    rsp->addressed_player_changed.player_id   = net_buf_pull_be16(buf);
                    rsp->addressed_player_changed.uid_counter = net_buf_pull_be16(buf);
                    break;

                case BT_AVRCP_EVENT_UIDS_CHANGED:
                    rsp->uid_counter = net_buf_pull_be16(buf);
                    break;

                case BT_AVRCP_EVENT_VOLUME_CHANGED:
                    rsp->absolute_volume = net_buf_pull_u8(buf);
                    break;

#endif /* AVRCP_1_4 */
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    return 0;
}

void *bt_avrcp_parse_vendor_rsp(struct bt_avrcp_vendor *vendor_rsp)
{
    void *ret = NULL;
    struct net_buf data_buf;
    struct net_buf *buf;

    data_buf.__buf = &vendor_rsp->parameter;
    data_buf.data  = &vendor_rsp->parameter;
    data_buf.size  = vendor_rsp->parameter_len;
    data_buf.len   = vendor_rsp->parameter_len;
    buf            = &data_buf;

    switch (vendor_rsp->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        {
            /* the data is byte mapped */
            ret = &vendor_rsp->parameter;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_player_get_txt_rsp *rsp = (struct bt_avrcp_player_get_txt_rsp *)&parse_data[0];
            ret                                     = rsp;
            rsp->num_of_id                          = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < rsp->num_of_id; index++)
            {
                rsp->texts[index].attr_value_id = net_buf_pull_u8(buf);
                rsp->texts[index].char_set      = net_buf_pull_be16(buf);
                rsp->texts[index].string_len    = net_buf_pull_u8(buf);
                rsp->texts[index].string        = net_buf_pull_mem(buf, rsp->texts[index].string_len);
            }
            break;
        }
        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct bt_avrcp_player_get_element_attr_rsp *rsp =
                (struct bt_avrcp_player_get_element_attr_rsp *)&parse_data[0];
            ret              = rsp;
            rsp->num_of_attr = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < rsp->num_of_attr; index++)
            {
                rsp->attrs[index].attr_id    = net_buf_pull_be32(buf);
                rsp->attrs[index].char_set   = net_buf_pull_be16(buf);
                rsp->attrs[index].string_len = net_buf_pull_be16(buf);
                rsp->attrs[index].string     = net_buf_pull_mem(buf, rsp->attrs[index].string_len);
            }
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        {
            struct bt_avrcp_play_status_rsp *rsp = (struct bt_avrcp_play_status_rsp *)parse_data;
            ret                                  = rsp;
            rsp->song_length                     = net_buf_pull_be32(buf);
            rsp->song_pos                        = net_buf_pull_be32(buf);
            rsp->play_status                     = net_buf_pull_u8(buf);
            break;
        }
        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
        {
            struct bt_avrcp_event_rsp *rsp = (struct bt_avrcp_event_rsp *)parse_data;
            ret                                      = rsp;
            rsp->event_id                            = net_buf_pull_u8(buf);
            switch (rsp->event_id)
            {
                case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
                    rsp->play_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_TRACK_CHANGED:
                    memcpy(rsp->identifier, buf->data, 8u);
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_END:
                    break;

                case BT_AVRCP_EVENT_TRACK_REACHED_START:
                    break;

                case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
                    break;

                case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
                    rsp->battery_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
                    rsp->system_status = net_buf_pull_u8(buf);
                    break;

                case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
                    rsp->setting_changed.num_of_attr = net_buf_pull_u8(buf);
                    for (uint8_t index = 0; index < rsp->setting_changed.num_of_attr; index++)
                    {
                        rsp->setting_changed.attr_vals[index].attr_id  = net_buf_pull_u8(buf);
                        rsp->setting_changed.attr_vals[index].value_id = net_buf_pull_u8(buf);
                    }
                    break;

#ifdef AVRCP_1_4
                case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
                    break;

                case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
                    break;

                case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
                    rsp->addressed_player_changed.player_id   = net_buf_pull_be16(buf);
                    rsp->addressed_player_changed.uid_counter = net_buf_pull_be16(buf);
                    break;

                case BT_AVRCP_EVENT_UIDS_CHANGED:
                    rsp->uid_counter = net_buf_pull_be16(buf);
                    break;

                case BT_AVRCP_EVENT_VOLUME_CHANGED:
                    rsp->absolute_volume = net_buf_pull_u8(buf);
                    break;

#endif /* AVRCP_1_4 */
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    return ret;
}

int bt_avrcp_send_unit_info(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_unit_info(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_unit_info");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_send_subunit_info(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_subunit_info(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_subunit_info");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_send_passthrough(struct bt_conn *conn, uint8_t op_id, uint8_t vendor_op_id, uint8_t state_flag)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_passthrough_cmd_rsp(&avrcp->ethermind_avrcp, 0x00, BT_AVRCP_COMMAND_TYPE_CONTROL, op_id,
                                               state_flag, vendor_op_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_AVRCP_COMMAND_TYPE_CONTROL");
        return -EIO;
    }

    return 0;
}

#if 0
int bt_avrcp_get_company_id_supported(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_get_companyid_supported(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_AVRCP_COMMAND_TYPE_CONTROL");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_event_supported(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_get_event_supported(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_AVRCP_COMMAND_TYPE_CONTROL");
        return -EIO;
    }

    return 0;
}
#endif

int bt_avrcp_send_vendor_dependent(struct bt_conn *conn, uint8_t pdu_id, void *parameter)
{
    API_RESULT ret;
    AVRCP_AL_VD_CMD_INFO pdu_info;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);
    uint8_t data[5];

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    pdu_info.tl          = 0x00;
    pdu_info.packet_type = BT_AVRCP_PACKET_TYPE_SINGLE;
    pdu_info.pdu_id      = pdu_id;
    pdu_info.cmd_type    = BT_AVRCP_COMMAND_TYPE_STATUS;
    ret                  = API_SUCCESS;

    switch (pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        {
            pdu_info.vd_cmd_data    = parameter;
            pdu_info.vd_cmd_datalen = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
        {
            struct bt_avrcp_register_ntfy *reg =
                (struct bt_avrcp_register_ntfy *)parameter;
            struct net_buf data_buf;
            struct net_buf *buf;

            data_buf.__buf = &send_data[0];
            data_buf.data  = &send_data[0];
            data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
            data_buf.len   = 0;
            buf            = &data_buf;

            net_buf_add_u8(buf, reg->event_id);
            net_buf_add_be32(buf, reg->playback_interval);
            pdu_info.cmd_type       = BT_AVRCP_COMMAND_TYPE_NOTIFY;
            pdu_info.vd_cmd_data    = buf->data;
            pdu_info.vd_cmd_datalen = buf->len;
            break;
        }
        case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
        case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
            pdu_info.cmd_type = BT_AVRCP_COMMAND_TYPE_CONTROL;
            pdu_info.vd_cmd_data = parameter;
            pdu_info.vd_cmd_datalen = 1;
            break;
        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
            pdu_info.cmd_type = BT_AVRCP_COMMAND_TYPE_CONTROL;
            sys_put_be16(*((uint16_t*)parameter), data);
            pdu_info.vd_cmd_data = data;
            pdu_info.vd_cmd_datalen = 2;
            break;
        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
            pdu_info.vd_cmd_data    = NULL;
            pdu_info.vd_cmd_datalen = 0;
            break;

        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
            pdu_info.vd_cmd_data    = parameter;
            pdu_info.vd_cmd_datalen = 1;
            break;

        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        {
            struct bt_avrcp_player_app_setting_attr_ids *attr_ids =
                (struct bt_avrcp_player_app_setting_attr_ids *)parameter;

            pdu_info.vd_cmd_data    = attr_ids;
            pdu_info.vd_cmd_datalen = 1 + attr_ids->num_of_attr;
            break;
        }

        case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_attr_values *attr_values = (struct bt_avrcp_player_app_attr_values *)parameter;

            pdu_info.cmd_type       = BT_AVRCP_COMMAND_TYPE_CONTROL;
            pdu_info.vd_cmd_data    = attr_values;
            pdu_info.vd_cmd_datalen = 1 + 2 * attr_values->num_of_attr;
            break;
        }

        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_get_player_app_setting_value_text *value_text =
                (struct bt_avrcp_get_player_app_setting_value_text *)parameter;

            pdu_info.vd_cmd_data    = value_text;
            pdu_info.vd_cmd_datalen = 1 + 1 + value_text->num_of_value;
            break;
        }

        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
        {
            struct bt_avrcp_inform_displayable_char_set *char_set =
                (struct bt_avrcp_inform_displayable_char_set *)parameter;
            struct net_buf data_buf;
            struct net_buf *buf;

            data_buf.__buf = &send_data[0];
            data_buf.data  = &send_data[0];
            data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
            data_buf.len   = 0;
            buf            = &data_buf;

            net_buf_add_u8(buf, char_set->num_of_char);
            for (uint8_t index = 0; index < char_set->num_of_char; index++)
            {
                net_buf_add_be16(buf, char_set->char_sets[index]);
            }

            pdu_info.cmd_type       = BT_AVRCP_COMMAND_TYPE_CONTROL;
            pdu_info.vd_cmd_data    = buf->data;
            pdu_info.vd_cmd_datalen = buf->len;
            break;
        }

        case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
        {
            pdu_info.cmd_type       = BT_AVRCP_COMMAND_TYPE_CONTROL;
            pdu_info.vd_cmd_data    = parameter;
            pdu_info.vd_cmd_datalen = 1;
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct net_buf data_buf;
            struct net_buf *buf;
            struct bt_avrcp_get_element_attrs *elem = (struct bt_avrcp_get_element_attrs *)parameter;

            data_buf.__buf = &send_data[0];
            data_buf.data  = &send_data[0];
            data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
            data_buf.len   = 0;
            buf            = &data_buf;

            net_buf_add_mem(buf, &elem->identifier[0], 8u);
            net_buf_add_u8(buf, elem->num_of_attr);
            for (uint8_t index = 0; index < elem->num_of_attr; index++)
            {
                net_buf_add_be32(buf, elem->attr_ids[index]);
            }

            pdu_info.pdu_id         = BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE;
            pdu_info.vd_cmd_data    = buf->data;
            pdu_info.vd_cmd_datalen = buf->len;
            break;
        }

        case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
        {
            struct net_buf data_buf;
            struct net_buf *buf;
            struct bt_avrcp_add_to_now_playing *add = (struct bt_avrcp_add_to_now_playing *)parameter;

            data_buf.__buf = &send_data[0];
            data_buf.data  = &send_data[0];
            data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
            data_buf.len   = 0;
            buf            = &data_buf;

            net_buf_add_u8(buf, add->scope);
            net_buf_add_mem(buf, add->uid, 8u);
            net_buf_add_be16(buf, add->uid_counter);

            pdu_info.pdu_id         = BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING;
            pdu_info.vd_cmd_data    = buf->data;
            pdu_info.vd_cmd_datalen = buf->len;
            break;
        }

        case BT_AVRCP_PDU_ID_PLAY_ITEMS:
        {
            struct net_buf data_buf;
            struct net_buf *buf;
            struct bt_avrcp_play_item *play = (struct bt_avrcp_play_item *)parameter;

            data_buf.__buf = &send_data[0];
            data_buf.data  = &send_data[0];
            data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
            data_buf.len   = 0;
            buf            = &data_buf;

            net_buf_add_u8(buf, play->scope);
            net_buf_add_mem(buf, play->uid, 8u);
            net_buf_add_be16(buf, play->uid_counter);

            pdu_info.pdu_id         = BT_AVRCP_PDU_ID_PLAY_ITEMS;
            pdu_info.vd_cmd_data    = buf->data;
            pdu_info.vd_cmd_datalen = buf->len;
            break;
        }

        default:
            ret = API_FAILURE;
            break;
    }

    if (ret != API_SUCCESS)
    {
        return -EIO;
    }

    ret = BT_avrcp_al_send_metadata_pdu(&avrcp->ethermind_avrcp, &pdu_info, 0x00);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_metadata_pdu");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_play_status(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_get_play_status(&avrcp->ethermind_avrcp);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_get_play_status");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_register_notification(struct bt_conn *conn, uint8_t event, uint32_t playback_interval)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_register_notification(&avrcp->ethermind_avrcp, event, playback_interval);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_register_notification");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_request_continuing_rsp(struct bt_conn *conn, uint8_t pdu_id)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_request_continuing_rsp(&avrcp->ethermind_avrcp, pdu_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_request_continuing_rsp");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_abort_continuing_rsp(struct bt_conn *conn, uint8_t pdu_id)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_abort_continuing_rsp(&avrcp->ethermind_avrcp, pdu_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_abort_continuing_rsp");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_set_volume(struct bt_conn *conn, uint8_t volume)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_set_absolute_volume(&avrcp->ethermind_avrcp, volume);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_set_absolute_volume");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_set_addressed_player(struct bt_conn *conn, uint16_t player_id)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_send_set_addressed_player_cmd(&avrcp->ethermind_avrcp, player_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_send_set_addressed_player_cmd");
        return -EIO;
    }

    return 0;
}

#endif /* CONFIG_BT_AVRCP_CT */

#if (defined(CONFIG_BT_AVRCP_BROWSING) && ((CONFIG_BT_AVRCP_BROWSING) > 0U))
int bt_avrcp_browsing_connect(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_channel_connect_req(&avrcp->ethermind_avrcp, conn->br.dst.val, AVCTP_CHANNEL_TYPE_BROWSING);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to connect");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_browsing_disconnect(struct bt_conn *conn)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_channel_disconnect_req(&avrcp->ethermind_avrcp, AVCTP_CHANNEL_TYPE_BROWSING);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to disconnect browsing channel")
        return -EINVAL;
    }

    return 0;
}

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
int bt_avrcp_get_folder_items(struct bt_conn *conn, struct bt_avrcp_get_folder_items_cmd *param)
{
    API_RESULT ret;
    uint8_t index;
    AVRCP_AL_GET_FOLDER_ITEMS_PARAM get_param;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    get_param.scope      = param->scope;
    get_param.start_item = param->start_item;
    get_param.end_item   = param->end_item;
    get_param.attr_count = param->attr_count;

    if ((0x00 != param->attr_count) && (0xFF != param->attr_count))
    {
        for (index = 0; index < param->attr_count; index++)
        {
            get_param.attr_list[index] = param->attr_list[index];
        }
    }

    ret = BT_avrcp_al_get_folder_items(&avrcp->ethermind_avrcp, &get_param);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_get_folder_items");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_change_path(struct bt_conn *conn, struct bt_avrcp_change_path_cmd *param)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_change_path(&avrcp->ethermind_avrcp, param->uid_counter, param->direction, param->folder_uid);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_change_path");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_set_borwsed_player(struct bt_conn *conn, uint16_t player_id)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_set_browsed_player(&avrcp->ethermind_avrcp, player_id);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_get_folder_items");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_items_attribute(struct bt_conn *conn, struct bt_avrcp_get_item_attrs_cmd *param)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);
    AVRCP_AL_GET_ITEM_ATTRIBUTE_PARAMS attr_param;

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    attr_param.scope       = param->scope;
    attr_param.uid_msb     = sys_get_be32(&param->uid[4]);
    attr_param.uid_lsb     = sys_get_be32(&param->uid[0]);
    attr_param.uid_counter = param->uid_counter;
    if (param->num_of_attr > 8)
    {
        param->num_of_attr = 8;
    }
    attr_param.attr_count = param->num_of_attr;
    for (uint8_t index = 0; index < param->num_of_attr; index++)
    {
        attr_param.attr_list[index] = param->attr_list[index];
    }

    ret = BT_avrcp_al_get_items_attribute(&avrcp->ethermind_avrcp, &attr_param);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_get_items_attribute");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_search(struct bt_conn *conn, struct bt_avrcp_search_cmd *param)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_search(&avrcp->ethermind_avrcp, param->char_set, param->str, param->length);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_search");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_total_num_of_items(struct bt_conn *conn, uint8_t scope)
{
    API_RESULT ret;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    ret = BT_avrcp_al_get_total_number_of_items(&avrcp->ethermind_avrcp, scope);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_al_get_total_number_of_items");
        return -EIO;
    }

    return 0;
}
#endif /* CONFIG_BT_AVRCP_CT */

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
static uint16_t get_item_len(struct bt_avrcp_item *item)
{
    uint16_t len = 0;

    switch (item->item_type)
    {
        case AVRCP_ITEM_MEDIA_PLAYER:
            len = AVRCP_PLAYER_ITEM_LEN + item->player_item.name_len;
            break;
        case AVRCP_ITEM_FOLDER:
            len = AVRCP_FOLDER_ITEM_LEN + item->folder_item.name_len;
            break;
        case AVRCP_ITEM_MEDIA_ELEMENT:
            len = AVRCP_MEDIA_ITEM_LEN + item->media_item.name_len;
            len += 1;
            for (uint8_t index = 0; index < item->media_item.num_of_attr; index++)
            {
                len += 8;
                len += item->media_item.attrs[index].value_len;
            }
            break;
        default:
            break;
    }

    return len;
}

struct bt_avrcp_browsing_cmd *bt_avrcp_parse_browsing_cmd_data(uint8_t *data, uint32_t len)
{
    struct net_buf data_buf;
    struct net_buf *buf;
    struct bt_avrcp_browsing_cmd *browsing_cmd = (struct bt_avrcp_browsing_cmd *)&parse_data[0];

    data_buf.__buf = data;
    data_buf.data  = data;
    data_buf.size  = len;
    data_buf.len   = len;
    buf            = &data_buf;

    browsing_cmd->header.tl_pt_cr_ipid = net_buf_pull_u8(buf);
    // browsing_cmd->header.pid = net_buf_pull_be16(buf);
    browsing_cmd->header.pdu_id        = net_buf_pull_u8(buf);
    browsing_cmd->header.parameter_len = net_buf_pull_be16(buf);
    switch (browsing_cmd->header.pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
        {
            struct bt_avrcp_get_folder_items_cmd *cmd = &browsing_cmd->folder_items;

            cmd->scope      = net_buf_pull_u8(buf);
            cmd->start_item = net_buf_pull_be32(buf);
            cmd->end_item   = net_buf_pull_be32(buf);
            cmd->attr_count = net_buf_pull_u8(buf);

            for (uint8_t index = 0; index < cmd->attr_count; index++)
            {
                cmd->attr_list[index] = net_buf_pull_be32(buf);
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
            struct bt_avrcp_set_browsed_player_cmd *cmd = &browsing_cmd->set_browsed_player;

            cmd->player_id = net_buf_pull_be16(buf);
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            uint8_t *data;
            struct bt_avrcp_change_path_cmd *cmd = &browsing_cmd->change_path;

            cmd->uid_counter = net_buf_pull_be16(buf);
            cmd->direction   = net_buf_pull_u8(buf);
            data             = net_buf_pull_mem(buf, 8u);
            memcpy(cmd->folder_uid, data, 8u);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            uint8_t *data;
            struct bt_avrcp_get_item_attrs_cmd *cmd = &browsing_cmd->get_item_attrs;

            cmd->scope = net_buf_pull_u8(buf);
            data       = net_buf_pull_mem(buf, 8u);
            memcpy(cmd->uid, data, 8u);
            cmd->uid_counter = net_buf_pull_be16(buf);
            cmd->num_of_attr = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < cmd->num_of_attr; index++)
            {
                cmd->attr_list[index] = net_buf_pull_be32(buf);
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            struct bt_avrcp_search_cmd *cmd = &browsing_cmd->search;

            cmd->char_set = net_buf_pull_be16(buf);
            cmd->length   = net_buf_pull_be16(buf);
            cmd->str      = net_buf_pull_mem(buf, cmd->length);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            struct bt_avrcp_get_total_num_of_items_cmd *cmd = &browsing_cmd->get_total_num_of_items;

            cmd->scope = net_buf_pull_u8(buf);
            break;
        }

        default:
            browsing_cmd = NULL;
            break;
    }

    return browsing_cmd;
}

struct bt_avrcp_browsing_rsp *bt_avrcp_parse_browsing_rsp_data(uint8_t *data, uint32_t len)
{
    struct bt_avrcp_browsing_rsp *browsing_rsp = (struct bt_avrcp_browsing_rsp *)&parse_data[0];
    struct net_buf data_buf;
    struct net_buf *buf;

    data_buf.__buf = data;
    data_buf.data  = data;
    data_buf.size  = len;
    data_buf.len   = len;
    buf            = &data_buf;

    browsing_rsp->header.tl_pt_cr_ipid = net_buf_pull_u8(buf);
    // browsing_rsp->header.pid = net_buf_pull_be16(buf);
    browsing_rsp->header.pdu_id        = net_buf_pull_u8(buf);
    browsing_rsp->header.parameter_len = net_buf_pull_be16(buf);

    switch (browsing_rsp->header.pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
        {
            struct bt_avrcp_get_folder_items_rsp *rsp = &browsing_rsp->folder_items;

            rsp->status       = net_buf_pull_u8(buf);
            rsp->uid_counter  = net_buf_pull_be16(buf);
            rsp->num_of_items = net_buf_pull_be16(buf);

            for (uint8_t index = 0; index < rsp->num_of_items; index++)
            {
                uint16_t item_len;
                struct bt_avrcp_item *item = (struct bt_avrcp_item *)&rsp->items[index];

                item->item_type = net_buf_pull_u8(buf);
                item_len        = net_buf_pull_be16(buf);
                if (item_len > buf->len)
                {
                    return NULL;
                }

                switch (item->item_type)
                {
                    case AVRCP_ITEM_MEDIA_PLAYER:
                        item->player_item.player_id      = net_buf_pull_be16(buf);
                        item->player_item.player_type    = net_buf_pull_u8(buf);
                        item->player_item.player_subtype = net_buf_pull_be32(buf);
                        item->player_item.play_status    = net_buf_pull_u8(buf);
                        memcpy(item->player_item.feature_mask, net_buf_pull_mem(buf, 16u), 16u);
                        item->player_item.char_set = net_buf_pull_be16(buf);
                        item->player_item.name_len = net_buf_pull_be16(buf);
                        item->player_item.name     = net_buf_pull_mem(buf, item->player_item.name_len);
                        break;
                    case AVRCP_ITEM_FOLDER:
                        memcpy(item->folder_item.folder_uid, net_buf_pull_mem(buf, 8u), 8u);
                        item->folder_item.folder_type = net_buf_pull_u8(buf);
                        item->folder_item.playable    = net_buf_pull_u8(buf);
                        item->folder_item.char_set    = net_buf_pull_be16(buf);
                        item->folder_item.name_len    = net_buf_pull_be16(buf);
                        item->folder_item.name        = net_buf_pull_mem(buf, item->folder_item.name_len);
                        break;
                    case AVRCP_ITEM_MEDIA_ELEMENT:
                        memcpy(item->media_item.media_uid, net_buf_pull_mem(buf, 8u), 8u);
                        item->media_item.media_type  = net_buf_pull_u8(buf);
                        item->media_item.char_set    = net_buf_pull_be16(buf);
                        item->media_item.name_len    = net_buf_pull_be16(buf);
                        item->media_item.name        = net_buf_pull_mem(buf, item->media_item.name_len);
                        item->media_item.num_of_attr = net_buf_pull_u8(buf);
                        for (uint8_t ind = 0; ind < item->media_item.num_of_attr; ind++)
                        {
                            item->media_item.attrs[ind].attr_id   = net_buf_pull_be32(buf);
                            item->media_item.attrs[ind].char_set  = net_buf_pull_be16(buf);
                            item->media_item.attrs[ind].value_len = net_buf_pull_be16(buf);
                            item->media_item.attrs[ind].value_str =
                                net_buf_pull_mem(buf, item->media_item.attrs[ind].value_len);
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
            struct bt_avrcp_set_browsed_player_rsp *rsp = &browsing_rsp->set_browsed_player;

            rsp->status       = net_buf_pull_u8(buf);
            rsp->uid_counter  = net_buf_pull_be16(buf);
            rsp->num_of_items = net_buf_pull_be32(buf);
            rsp->char_set     = net_buf_pull_be16(buf);
            rsp->folder_depth = net_buf_pull_u8(buf);

            for (uint8_t index = 0; index < rsp->folder_depth; index++)
            {
                rsp->folder_names[index].folder_name_len = net_buf_pull_be16(buf);
                rsp->folder_names[index].folder_name = net_buf_pull_mem(buf, rsp->folder_names[index].folder_name_len);
            }
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            struct bt_avrcp_change_path_rsp *rsp = &browsing_rsp->change_path;

            rsp->status       = net_buf_pull_u8(buf);
            rsp->num_of_items = net_buf_pull_be32(buf);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            struct bt_avrcp_get_item_attrs_rsp *rsp = &browsing_rsp->get_item_attrs;

            rsp->status      = net_buf_pull_u8(buf);
            rsp->num_of_attr = net_buf_pull_u8(buf);
            for (uint8_t index = 0; index < rsp->num_of_attr; index++)
            {
                uint16_t item_len                    = 8;
                struct bt_avrcp_attr_val_entry *item = &rsp->attrs[index];

                item_len += item->value_len;

                if (item_len < net_buf_tailroom(buf))
                {
                    item->attr_id   = net_buf_pull_be32(buf);
                    item->char_set  = net_buf_pull_be16(buf);
                    item->value_len = net_buf_pull_be16(buf);
                    item->value_str = net_buf_pull_mem(buf, item->value_len);
                }
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            struct bt_avrcp_search_rsp *rsp = &browsing_rsp->search;

            rsp->status       = net_buf_pull_u8(buf);
            rsp->uid_counter  = net_buf_pull_be16(buf);
            rsp->num_of_items = net_buf_pull_be32(buf);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            struct bt_avrcp_get_total_num_of_items_rsp *rsp = &browsing_rsp->get_total_num_of_items;

            rsp->status       = net_buf_pull_u8(buf);
            rsp->uid_counter  = net_buf_pull_be16(buf);
            rsp->num_of_items = net_buf_pull_be32(buf);
            break;
        }

        default:
            browsing_rsp = NULL;
            break;
    }

    return browsing_rsp;
}

int bt_avrcp_response_browsing(struct bt_conn *conn,
    uint8_t pdu_id, uint8_t tl, void *rsp_param, uint16_t param_len)
{
    uint8_t send      = 1;
    API_RESULT retval = API_SUCCESS;
    struct net_buf data_buf;
    struct net_buf *buf;
    AVRCP_AL_BROW_RSP_INFO rsp_info;
    struct bt_avrcp_instance *avrcp = bt_avrcp_get_instance(conn);

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    if (avrcp == NULL)
    {
        return -EINVAL;
    }

    if (rsp_param == NULL)
    {
        return -EINVAL;
    }

    data_buf.__buf = &send_data[0];
    data_buf.data  = &send_data[0];
    data_buf.size  = TRANSFER_DATA_BUFF_SIZE;
    data_buf.len   = 0;
    buf            = &data_buf;

    rsp_info.pdu_id = pdu_id;
    rsp_info.tl     = tl << 4u;
    if (*((uint8_t *)rsp_param) != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
    {
        rsp_info.param_info     = rsp_param;
        rsp_info.param_info_len = 1;

        retval = BT_avrcp_al_send_browsing_cmd_rsp(&avrcp->ethermind_avrcp, &rsp_info);
        if (retval != API_SUCCESS)
        {
            BT_ERR("FAILED !! Error Code = 0x%04X\n", retval);
        }
        return 0;
    }

    rsp_info.param_info     = NULL;
    rsp_info.param_info_len = 0;

    switch (pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
        {
            uint8_t index;
            struct bt_avrcp_get_folder_items_rsp *rsp = (struct bt_avrcp_get_folder_items_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_be16(buf, rsp->uid_counter);
            net_buf_add_be16(buf, rsp->num_of_items);

            for (index = 0; index < rsp->num_of_items; index++)
            {
                uint16_t item_len;
                struct bt_avrcp_item *item = (struct bt_avrcp_item *)&rsp->items[index];

                item_len = get_item_len(item);
                if (item_len < net_buf_tailroom(buf))
                {
                    net_buf_add_u8(buf, item->item_type);
                    net_buf_add_be16(buf, (item_len - 3));
                    switch (item->item_type)
                    {
                        case AVRCP_ITEM_MEDIA_PLAYER:
                            net_buf_add_be16(buf, item->player_item.player_id);
                            net_buf_add_u8(buf, item->player_item.player_type);
                            net_buf_add_be32(buf, item->player_item.player_subtype);
                            net_buf_add_u8(buf, item->player_item.play_status);
                            net_buf_add_mem(buf, item->player_item.feature_mask, 16u);
                            net_buf_add_be16(buf, item->player_item.char_set);
                            net_buf_add_be16(buf, item->player_item.name_len);
                            net_buf_add_mem(buf, item->player_item.name, item->player_item.name_len);
                            break;
                        case AVRCP_ITEM_FOLDER:
                            net_buf_add_mem(buf, item->folder_item.folder_uid, 8u);
                            net_buf_add_u8(buf, item->folder_item.folder_type);
                            net_buf_add_u8(buf, item->folder_item.playable);
                            net_buf_add_be16(buf, item->folder_item.char_set);
                            net_buf_add_be16(buf, item->folder_item.name_len);
                            net_buf_add_mem(buf, item->folder_item.name, item->folder_item.name_len);
                            break;
                        case AVRCP_ITEM_MEDIA_ELEMENT:
                            net_buf_add_mem(buf, item->media_item.media_uid, 8u);
                            net_buf_add_u8(buf, item->media_item.media_type);
                            net_buf_add_be16(buf, item->media_item.char_set);
                            net_buf_add_be16(buf, item->media_item.name_len);
                            net_buf_add_mem(buf, item->media_item.name, item->media_item.name_len);
                            net_buf_add_u8(buf, item->media_item.num_of_attr);
                            for (uint8_t ind = 0; ind < item->media_item.num_of_attr; ind++)
                            {
                                net_buf_add_be32(buf, item->media_item.attrs[ind].attr_id);
                                net_buf_add_be16(buf, item->media_item.attrs[ind].char_set);
                                net_buf_add_be16(buf, item->media_item.attrs[ind].value_len);
                                net_buf_add_mem(buf, item->media_item.attrs[ind].value_str,
                                                item->media_item.attrs[ind].value_len);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
            struct bt_avrcp_set_browsed_player_rsp *rsp = (struct bt_avrcp_set_browsed_player_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_be16(buf, rsp->uid_counter);
            net_buf_add_be32(buf, rsp->num_of_items);
            net_buf_add_be16(buf, rsp->char_set);
            net_buf_add_u8(buf, rsp->folder_depth);

            for (uint8_t index = 0; index < rsp->folder_depth; index++)
            {
                net_buf_add_be16(buf, rsp->folder_names[index].folder_name_len);
                net_buf_add_mem(buf, rsp->folder_names[index].folder_name, rsp->folder_names[index].folder_name_len);
            }
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            struct bt_avrcp_change_path_rsp *rsp = (struct bt_avrcp_change_path_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_be32(buf, rsp->num_of_items);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            struct bt_avrcp_get_item_attrs_rsp *rsp = (struct bt_avrcp_get_item_attrs_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_u8(buf, rsp->num_of_attr);
            for (uint8_t index = 0; index < rsp->num_of_attr; index++)
            {
                uint16_t item_len                    = 8;
                struct bt_avrcp_attr_val_entry *item = &rsp->attrs[index];

                item_len += item->value_len;

                if (item_len < net_buf_tailroom(buf))
                {
                    net_buf_add_be32(buf, item->attr_id);
                    net_buf_add_be16(buf, item->char_set);
                    net_buf_add_be16(buf, item->value_len);
                    net_buf_add_mem(buf, item->value_str, item->value_len);
                }
            }
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            struct bt_avrcp_search_rsp *rsp = (struct bt_avrcp_search_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_be16(buf, rsp->uid_counter);
            net_buf_add_be32(buf, rsp->num_of_items);
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            struct bt_avrcp_get_total_num_of_items_rsp *rsp = (struct bt_avrcp_get_total_num_of_items_rsp *)rsp_param;

            net_buf_add_u8(buf, rsp->status);
            net_buf_add_be16(buf, rsp->uid_counter);
            net_buf_add_be32(buf, rsp->num_of_items);
            break;
        }

        default:
            send = 0;
            break;
    }

    if (send == 1)
    {
        rsp_info.param_info     = buf->data;
        rsp_info.param_info_len = buf->len;

        retval = BT_avrcp_al_send_browsing_cmd_rsp(&avrcp->ethermind_avrcp, &rsp_info);
        if (retval != API_SUCCESS)
        {
            BT_ERR("FAILED !! Error Code = 0x%04X\n", retval);
            return -EIO;
        }
    }

    return 0;
}
#endif /* CONFIG_BT_AVRCP_TG */
#endif /* CONFIG_BT_AVRCP_BROWSING */

#if (defined(CONFIG_BT_AVRCP_COVER_ART) && (CONFIG_BT_AVRCP_COVER_ART > 0U))
struct bt_avrcp_cover_art_cb cover_art_cb;
int bt_avrcp_register_cover_art_cb(struct bt_avrcp_cover_art_cb *cb)
{
    cover_art_cb = *cb;
    return 0;
}

static API_RESULT ethermind_avrcp_cai_cb(AVRCP_CA_HANDLE *avrcp_ca_handle,
                                         UINT8 event_type,
                                         UINT16 event_result,
                                         AVRCP_CA_HEADERS *avrcp_ca_headers,
                                         UINT16 num_headers)
{
    uint8_t handle   = *avrcp_ca_handle;
    uint8_t callback = 1;
    struct bt_avrcp_cover_art_rsp rsp;

    switch (event_type)
    {
        case AVRCP_CAI_TRANSPORT_CLOSE_CNF:
        case AVRCP_CAI_TRANSPORT_CLOSE_IND:
            if (cover_art_cb.disconnected != NULL)
            {
                cover_art_cb.disconnected(handle, 0);
            }
            callback = 0;
            break;

        case AVRCP_CAI_DISCONNECT_CNF:
            rsp.cmd = BT_AVRCP_COVER_ART_DISCONNECT;
            break;

        case AVRCP_CAI_CONNECT_CNF:
        {
            struct bt_conn *conn;

            get_conn = NULL;
            bt_conn_foreach(BT_CONN_TYPE_BR, avrcp_get_conn, avrcp_ca_headers->ca_connect_info->bd_addr);
            conn                      = get_conn;
            rsp.cmd                   = BT_AVRCP_COVER_ART_CONNECT;
            rsp.connect.max_recv_size = avrcp_ca_headers->ca_connect_info->max_recv_size;

            if (cover_art_cb.connected != NULL)
            {
                cover_art_cb.connected(handle, conn, 0);
            }
            break;
        }

        case AVRCP_CAI_GET_IMAGE_PROPERTIES_CNF:
            rsp.cmd             = BT_AVRCP_COVER_ART_GET_PROP;
            rsp.response        = event_result;
            rsp.get_prop.data   = avrcp_ca_headers->ca_rsp_info->body->value;
            rsp.get_prop.length = avrcp_ca_headers->ca_rsp_info->body->length;
            break;

        case AVRCP_CAI_GET_IMAGE_CNF:
            rsp.cmd              = BT_AVRCP_COVER_ART_GET_IMAGE;
            rsp.response         = event_result;
            rsp.get_image.data   = avrcp_ca_headers->ca_rsp_info->body->value;
            rsp.get_image.length = avrcp_ca_headers->ca_rsp_info->body->length;
            break;

        case AVRCP_CAI_GET_LINKED_THUMBNAIL_CNF:
            rsp.cmd              = BT_AVRCP_COVER_ART_GET_THUMB;
            rsp.response         = event_result;
            rsp.get_thumb.data   = avrcp_ca_headers->ca_rsp_info->body->value;
            rsp.get_thumb.length = avrcp_ca_headers->ca_rsp_info->body->length;
            break;

        case AVRCP_CAI_GET_ABORT_CNF:
            rsp.cmd = BT_AVRCP_COVER_ART_ABORT;
            break;

        default:
            break;
    }

    if (callback)
    {
        if (cover_art_cb.rsp_received != NULL)
        {
            cover_art_cb.rsp_received(handle, &rsp, 0);
        }
    }
    return API_SUCCESS;
}

API_RESULT ethermind_avrcp_car_cb(AVRCP_CA_HANDLE *avrcp_ca_handle,
                                  UINT8 event_type,
                                  UINT16 event_result,
                                  AVRCP_CA_HEADERS *ca_headers,
                                  UINT16 num_headers)
{
    struct bt_avrcp_cover_art_cmd cmd_msg;
    uint8_t callback = 1;

    switch (event_type)
    {
        case AVRCP_CAR_TRANSPORT_CLOSE_IND:
        case AVRCP_CAR_TRANSPORT_CLOSE_CNF:
            if (cover_art_cb.disconnected != NULL)
            {
                cover_art_cb.disconnected(*avrcp_ca_handle, 0);
            }
            callback = 0;
            break;

        case AVRCP_CAR_DISCONNECT_IND:
            cmd_msg.cmd = BT_AVRCP_COVER_ART_DISCONNECT;
            break;

        case AVRCP_CAR_CONNECT_IND:
        {
            struct bt_conn *conn;
            cmd_msg.cmd = BT_AVRCP_COVER_ART_CONNECT;
            if ((BT_AVRCP_CA_SUCCESS_RSP == event_result) && (NULL == ca_headers->ca_connect_info))
            {
                AVRCP_CA_HEADERS ca_rsp_hdrs;
                AVRCP_CA_RESPONSE_STRUCT rsp_hdrs;
                AVRCP_CA_HEADER_STRUCT ca_body_info;
                AVRCP_CA_HEADER_STRUCT ca_length_info;
                ca_rsp_hdrs.ca_rsp_info = &rsp_hdrs;

                /* Init response headers */
                rsp_hdrs.body   = &ca_body_info;
                rsp_hdrs.length = &ca_length_info;

                CA_INIT_HEADER_STRUCT(ca_body_info);
                CA_INIT_HEADER_STRUCT(ca_length_info);

                API_RESULT retval =
                    BT_avrcp_car_send_response(avrcp_ca_handle, event_type, AVRCP_CA_NOT_ACCEPTABLE_RSP, &ca_rsp_hdrs);
                if (API_SUCCESS != retval)
                {
                    BT_ERR("Failed to send AVRCP_TG Cover Art Response - 0x%05X\n", retval);
                }
                callback = 0;
                break;
            }

            get_conn = NULL;
            bt_conn_foreach(BT_CONN_TYPE_BR, avrcp_get_conn, ca_headers->ca_connect_info->bd_addr);
            conn = get_conn;

            cmd_msg.connect.max_recv_size = ca_headers->ca_connect_info->max_recv_size;
            if (cover_art_cb.connected != NULL)
            {
                cover_art_cb.connected(*avrcp_ca_handle, conn, 0);
            }
            break;
        }

        case AVRCP_CAR_GET_IMAGE_PROPERTIES_IND:
            cmd_msg.cmd = BT_AVRCP_COVER_ART_GET_PROP;
            if (ca_headers->ca_req_info->img_handle != NULL)
            {
                cmd_msg.get_prop.image_handle     = ca_headers->ca_req_info->img_handle->value;
                cmd_msg.get_prop.image_handle_len = ca_headers->ca_req_info->img_handle->length;
            }
            else
            {
                cmd_msg.get_prop.image_handle     = NULL;
                cmd_msg.get_prop.image_handle_len = 0;
            }
            break;

        case AVRCP_CAR_GET_IMAGE_IND:
            cmd_msg.cmd = BT_AVRCP_COVER_ART_GET_IMAGE;
            if (ca_headers->ca_req_info->img_handle != NULL)
            {
                cmd_msg.get_image.image_handle     = ca_headers->ca_req_info->img_handle->value;
                cmd_msg.get_image.image_handle_len = ca_headers->ca_req_info->img_handle->length;
            }
            else
            {
                cmd_msg.get_image.image_handle     = NULL;
                cmd_msg.get_image.image_handle_len = 0;
            }

            if (ca_headers->ca_req_info->img_descriptor != NULL)
            {
                cmd_msg.get_image.image_descriptor_data     = ca_headers->ca_req_info->img_descriptor->value;
                cmd_msg.get_image.image_descriptor_data_len = ca_headers->ca_req_info->img_descriptor->length;
            }
            else
            {
                cmd_msg.get_image.image_descriptor_data     = NULL;
                cmd_msg.get_image.image_descriptor_data_len = 0;
            }
            break;

        case AVRCP_CAR_GET_LINKED_THUMBNAIL_IND:
            cmd_msg.cmd = BT_AVRCP_COVER_ART_GET_THUMB;
            if (ca_headers->ca_req_info->img_handle != NULL)
            {
                cmd_msg.get_thumb.image_handle     = ca_headers->ca_req_info->img_handle->value;
                cmd_msg.get_thumb.image_handle_len = ca_headers->ca_req_info->img_handle->length;
            }
            else
            {
                cmd_msg.get_thumb.image_handle     = NULL;
                cmd_msg.get_thumb.image_handle_len = 0;
            }
            break;

        case AVRCP_CAR_GET_ABORT_IND:
            cmd_msg.cmd = BT_AVRCP_COVER_ART_GET_THUMB;
            break;

        default:
            break;
    }

    if (callback == 1u)
    {
        if (cover_art_cb.cmd_received != NULL)
        {
            cover_art_cb.cmd_received(*avrcp_ca_handle, &cmd_msg, 0);
        }
    }

    return API_SUCCESS;
}

#if (defined(CONFIG_BT_AVRCP_COVER_ART_INITIATOR) && ((CONFIG_BT_AVRCP_COVER_ART_INITIATOR) > 0U))
int bt_avrcp_cover_art_start_initiator(uint8_t *handle)
{
    API_RESULT ret = BT_avrcp_cai_start(handle, ethermind_avrcp_cai_cb);
    if (ret != API_SUCCESS)
    {
        BT_ERR("FAILED !! Error Code = 0x%04X\n", ret);
        return -EIO;
    }
    return 0;
}

int bt_avrcp_cover_art_stop_initiator(uint8_t handle)
{
    API_RESULT ret = BT_avrcp_cai_stop(&handle);
    if (ret != API_SUCCESS)
    {
        BT_ERR("FAILED !! Error Code = 0x%04X\n", ret);
        return -EIO;
    }
    return 0;
}
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_RESPONDER) && ((CONFIG_BT_AVRCP_COVER_ART_RESPONDER) > 0U))
int bt_avrcp_cover_art_start_responder(uint8_t *handle)
{
    API_RESULT ret = BT_avrcp_car_start(handle, ethermind_avrcp_car_cb);
    if (ret != API_SUCCESS)
    {
        BT_ERR("FAILED !! Error Code = 0x%04X\n", ret);
        return -EIO;
    }
    return 0;
}

int bt_avrcp_cover_art_stop_responder(uint8_t handle)
{
    API_RESULT ret = BT_avrcp_car_stop(&handle);
    if (ret != API_SUCCESS)
    {
        BT_ERR("FAILED !! Error Code = 0x%04X\n", ret);
        return -EIO;
    }
    return 0;
}
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_INITIATOR) && ((CONFIG_BT_AVRCP_COVER_ART_INITIATOR) > 0U))
int bt_avrcp_cover_art_connect(uint8_t handle, struct bt_conn *conn, struct bt_avrcp_cover_art_connect *cmd)
{
    API_RESULT ret;
    AVRCP_CA_CONNECT_STRUCT connect_info;

    connect_info.bd_addr       = conn->br.dst.val;
    connect_info.l2cap_rpsm    = cmd->l2cap_rpsm;
    connect_info.max_recv_size = cmd->max_recv_size;

    ret = BT_avrcp_cai_connect(&handle, &connect_info);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to connect");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_cover_art_disconnect(uint8_t handle)
{
    API_RESULT ret = BT_avrcp_cai_disconnect(&handle);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_disconnect");
        return -EIO;
    }
    return 0;
}

int bt_avrcp_get_image_property(uint8_t handle, struct bt_avrcp_get_image_property *param)
{
    API_RESULT ret;
    AVRCP_CA_REQUEST_STRUCT ca_req_info;
    AVRCP_CA_HEADER_STRUCT ca_img_handle_info;

    ca_req_info.wait          = param->wait;
    ca_req_info.img_handle    = &ca_img_handle_info;
    ca_img_handle_info.value  = param->image_handle;
    ca_img_handle_info.length = param->image_handle_len;

    ret = BT_avrcp_cai_get_image_properties(&handle, &ca_req_info);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_get_image_properties");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_image(uint8_t handle, struct bt_avrcp_get_image *param)
{
    API_RESULT ret;
    AVRCP_CA_REQUEST_STRUCT ca_req_info;
    AVRCP_CA_HEADER_STRUCT ca_img_handle_info;
    AVRCP_CA_HEADER_STRUCT ca_img_descriptor_info;

    ca_req_info.wait              = param->wait;
    ca_req_info.img_handle        = &ca_img_handle_info;
    ca_req_info.img_descriptor    = &ca_img_descriptor_info;
    ca_img_handle_info.value      = param->image_handle;
    ca_img_handle_info.length     = param->image_handle_len;
    ca_img_descriptor_info.value  = param->image_descriptor_data;
    ca_img_descriptor_info.length = param->image_descriptor_data_len;

    ret = BT_avrcp_cai_get_image(&handle, &ca_req_info);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_get_image");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_get_linked_thumbnail(uint8_t handle, struct bt_avrcp_get_linked_thumbnail *param)
{
    API_RESULT ret;
    AVRCP_CA_REQUEST_STRUCT ca_req_info;
    AVRCP_CA_HEADER_STRUCT ca_img_handle_info;

    ca_req_info.wait          = param->wait;
    ca_req_info.img_handle    = &ca_img_handle_info;
    ca_img_handle_info.value  = param->image_handle;
    ca_img_handle_info.length = param->image_handle_len;

    ret = BT_avrcp_cai_get_linked_thumbnail(&handle, &ca_req_info);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_get_linked_thumbnail");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_abort(uint8_t handle)
{
    API_RESULT ret;

    ret = BT_avrcp_cai_abort(&handle);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_get_linked_thumbnail");
        return -EIO;
    }

    return 0;
}

int bt_avrcp_send_request(uint8_t handle, uint8_t wait, struct bt_avrcp_cover_art_rsp *rsp)
{
    API_RESULT ret;
    uint8_t event_type;
    AVRCP_CA_HEADERS ca_event_headers;
    AVRCP_CA_REQUEST_STRUCT req_info;

    switch (rsp->cmd)
    {
        case BT_AVRCP_COVER_ART_CONNECT:
        {
            event_type = AVRCP_CAI_CONNECT_CNF;
            break;
        }
        case BT_AVRCP_COVER_ART_GET_PROP:
        {
            event_type = AVRCP_CAI_GET_IMAGE_PROPERTIES_CNF;
            break;
        }

        case BT_AVRCP_COVER_ART_GET_IMAGE:
        {
            event_type = AVRCP_CAI_GET_IMAGE_CNF;
            break;
        }

        case BT_AVRCP_COVER_ART_GET_THUMB:
        {
            event_type = AVRCP_CAI_GET_LINKED_THUMBNAIL_CNF;
            break;
        }
        default:
            event_type = 0;
            break;
    }

    ca_event_headers.ca_req_info = &req_info;
    if (wait != 0)
    {
        req_info.wait = BT_TRUE;
    }
    else
    {
        req_info.wait = BT_FALSE;
    }

    ret = BT_avrcp_cai_send_req(&handle, event_type, rsp->response, &ca_event_headers);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_cai_send_req");
        return -EIO;
    }

    return 0;
}
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_RESPONDER) && ((CONFIG_BT_AVRCP_COVER_ART_RESPONDER) > 0U))
int bt_avrcp_response_cover_art(uint8_t handle, uint8_t cmd, uint8_t response, void *rsp)
{
    API_RESULT ret;
    AVRCP_CA_HEADERS ca_rsp_hdrs;
    AVRCP_CA_RESPONSE_STRUCT rsp_hdrs;
    AVRCP_CA_HEADER_STRUCT ca_body_info;
    AVRCP_CA_HEADER_STRUCT ca_length_info;
    uint8_t event_type;

    ca_rsp_hdrs.ca_rsp_info = &rsp_hdrs;
    /* Init response headers */
    rsp_hdrs.body   = &ca_body_info;
    rsp_hdrs.length = &ca_length_info;

    CA_INIT_HEADER_STRUCT(ca_body_info);
    CA_INIT_HEADER_STRUCT(ca_length_info);

    switch (cmd)
    {
        case BT_AVRCP_COVER_ART_CONNECT:
        {
            event_type = AVRCP_CAR_CONNECT_IND;
            break;
        }
        case BT_AVRCP_COVER_ART_GET_PROP:
        {
            struct bt_avrcp_get_image_properity_rsp *cmd_rsp =
                (struct bt_avrcp_get_image_properity_rsp *)rsp;

            event_type          = AVRCP_CAR_GET_IMAGE_PROPERTIES_IND;
            ca_body_info.length = cmd_rsp->length;
            ca_body_info.value  = cmd_rsp->data;
            break;
        }

        case BT_AVRCP_COVER_ART_GET_IMAGE:
        {
            struct bt_avrcp_get_image_rsp *cmd_rsp =
                (struct bt_avrcp_get_image_rsp *)rsp;
            uint8_t file_size[4];

            event_type            = AVRCP_CAR_GET_IMAGE_IND;
            ca_length_info.length = 4u;
            sys_put_be32(cmd_rsp->image_file_size, file_size);
            ca_length_info.value = file_size;
            ca_body_info.length  = cmd_rsp->length;
            ca_body_info.value   = cmd_rsp->data;
            break;
        }

        case BT_AVRCP_COVER_ART_GET_THUMB:
        {
            struct bt_avrcp_get_linked_thumbnail_rsp *cmd_rsp =
                (struct bt_avrcp_get_linked_thumbnail_rsp *)rsp;

            event_type          = AVRCP_CAR_GET_LINKED_THUMBNAIL_IND;
            ca_body_info.length = cmd_rsp->length;
            ca_body_info.value  = cmd_rsp->data;
            break;
        }

        default:
            event_type = 0U;
            break;
    }

    if (event_type == 0U) {
        return -EINVAL;
    }

    ret = BT_avrcp_car_send_response(&handle, event_type, response, &ca_rsp_hdrs);
    if (ret != API_SUCCESS)
    {
        BT_ERR("fail to call BT_avrcp_car_send_response");
        return -EIO;
    }

    return 0;
}
#endif /* CONFIG_BT_AVRCP_COVER_ART_RESPONDER */
 
#endif  /* CONFIG_BT_AVRCP_COVER_ART */
#endif /* CONFIG_BT_AVRCP */

#endif /* CONFIG_BT_BREDR */
