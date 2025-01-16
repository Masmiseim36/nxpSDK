/* map.c - Bluetooth L2CAP Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if ((defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)) || \
     (defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)))

#include <porting.h>

#include "bt_pal_conn_internal.h"

#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/map_mce.h>
#include <bluetooth/map_mse.h>
#include <bluetooth/sdp.h>

#include "board.h"

#include "fsl_adapter_audio.h"
#include "fsl_codec_common.h"

#include "btp/btp.h"

#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_map
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define SDP_CLIENT_USER_BUF_LEN 512U

NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
#define MAP_MSE_CLASS_OF_DEVICE (0x10020CU) /* Object Transfer, Phone, Smartphone */

static struct bt_sdp_attribute map_mse_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MSE_SVCLASS) //11 32
        },
        )
    ),

    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MSE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MAS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16), 
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MSE_1)
    },
    /* MASInstanceID */
    BT_SDP_ATTR_MAS_INSTANCE_ID,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x0)
    },
    /* SupportedMessageTypes */
    BT_SDP_ATTR_SUPPORTED_MESSAGE_TYPES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x1F)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32), 
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MSE_MNS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record map_mse_rec = BT_SDP_RECORD(map_mse_attrs);

struct bt_map_mse_mas_cb mse_mas_cb = {
};

struct bt_map_mse_mns_cb mse_mns_cb = {
};
#endif /* CONFIG_BT_MAP_MSE */

#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
#define MAP_MCE_CLASS_OF_DEVICE (0x10010CU) /* Object Transfer, Computer, Laptop */

#define MAP_MCE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MCE_MAS_TX_NET_BUF_SIZE    (1024U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

struct app_map_mce_instance
{
    struct bt_conn *acl_conn;
    struct bt_map_mce_mas *mce_mas;
    struct bt_map_mce_mns *mce_mns;
    uint32_t supported_features;
    uint16_t max_pkt_len; /* range from opcode to the end of packet */
    uint16_t mns_max_pkt_len; /* range from opcode to the end of packet */
    uint16_t tx_cnt;
    uint16_t goep_version;
    uint16_t map_version;
    uint16_t psm;
    uint8_t scn;
    uint8_t mas_instance_id;
    uint8_t state;
    uint8_t num_srmp_wait;
};

static uint8_t sdp_map_mse_user(struct bt_conn *conn, struct bt_sdp_client_result *result);
static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns);
static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result);
static void app_mce_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag);
static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas);
static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_abort_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name);
static void app_mce_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_get_convo_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);

static struct bt_sdp_attribute map_mce_attrs[] = 
{
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MCE_SVCLASS) //11 33
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MCE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MNS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16), 
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MCE)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32), 
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MCE_MAS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record map_mce_rec = BT_SDP_RECORD(map_mce_attrs);
static struct app_map_mce_instance app_mce_instance;
NET_BUF_POOL_FIXED_DEFINE(mce_mas_tx_pool, MAP_MCE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MCE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);


struct bt_map_mce_mas_cb mce_mas_cb = {
    .connected = map_mce_mas_connected,
    .disconnected = map_mce_mas_disconnected,
    .set_ntf_reg = app_mce_set_ntf_reg_cb,
};

static struct bt_map_mce_mns_cb mce_mns_cb = {
    .connected = map_mce_mns_connected,
    .disconnected = map_mce_mns_disconnected,
    .send_event = NULL,
};

static struct bt_sdp_discover_params discov_map_mse = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_MAP_MSE_SVCLASS),
    .func = sdp_map_mse_user,
    .pool = &sdp_client_pool,
};

static uint8_t sdp_map_mse_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features;
    uint16_t map_version;
    const char *service_name;
    uint8_t mas_instance_id;
    uint8_t supported_msg_type;

    if ((result) && (result->resp_buf))
    {
        PRINTF("sdp success callback\r\n");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn);
        if (res < 0)
        {
            PRINTF("REFCOMM channel number is not found\r\n");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        PRINTF("REFCOMM channel number %d\r\n", scn);
        res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        if (res < 0)
        {
            app_mce_instance.goep_version = BT_GOEP_VERSION_1_1;
            PRINTF("L2CAP PSM is not found\r\n");
        }
        else
        {
            app_mce_instance.goep_version = BT_GOEP_VERSION_2_0;
            PRINTF("L2CAP PSM  0x%04X\r\n", psm);
        }
        res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        if (res < 0)
        {
            PRINTF("MAP version is not found\r\n");
        }
        else
        {
            PRINTF("MAP version 0x%04X\r\n", map_version);
            app_mce_instance.map_version = map_version;
        }
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        if (res < 0)
        {
            switch (app_mce_instance.map_version)
            {
                case BT_MAP_VERSION_1_1:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11;
                    break;
                case BT_MAP_VERSION_1_2:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V12;
                    break;
                case BT_MAP_VERSION_1_3:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V13;
                    break;
                case BT_MAP_VERSION_1_4:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V14;
                    break;
                default:
                    app_mce_instance.supported_features = 0;
                    break;
            }
            PRINTF("Supported features is not found."
            "Use the default supported features 0x%08Xr\n", app_mce_instance.supported_features);
        }
        else
        {
            PRINTF("MAP supported features 0x%08X\r\n", supported_features);
            app_mce_instance.supported_features = supported_features;
        }
        res = bt_sdp_get_instance_id(result->resp_buf, &mas_instance_id);
        if (res < 0)
        {
            PRINTF("MAS instance ID is not found\r\n");
        }
        else
        {
            PRINTF("MAS instance ID %d\r\n", mas_instance_id);
            app_mce_instance.mas_instance_id = mas_instance_id;
        }
        res = bt_sdp_get_supported_msg_type(result->resp_buf, &supported_msg_type);
        if (res < 0)
        {
            PRINTF("Supported message type is not found\r\n");
        }
        else
        {
            PRINTF("Supported message type 0x%02X\r\n", supported_msg_type);
        }
        res = bt_sdp_get_service_name(result->resp_buf, &service_name);
        if (res < 0)
        {
            PRINTF("Service name is not found\r\n");
        }
        else
        {
            PRINTF("Service name %s\r\n", service_name);
        }
        if ((scn != 0U) || (psm != 0U))
        {
            PRINTF("Message Access Server found. Connecting ...\r\n");
            app_mce_instance.psm = psm;
            app_mce_instance.scn = scn;
            if (app_mce_instance.goep_version >= BT_GOEP_VERSION_2_0)
            {
                res = bt_map_mce_psm_connect(conn, psm, supported_features, &app_mce_instance.mce_mas);
            }
            else
            {
                res = bt_map_mce_scn_connect(conn, (uint8_t)scn, supported_features, &app_mce_instance.mce_mas);
            }
            if (0 != res)
            {
                PRINTF("fail to connect MSE (err: %d)\r\n", res);
            }
        }
        return BT_SDP_DISCOVER_UUID_STOP;
    }
    else
    {
        PRINTF("sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns)
{
    app_mce_instance.mce_mns = mce_mns;
    PRINTF("MCE MNS connection");
    if (bt_map_mce_mns_get_max_pkt_len(mce_mns, &app_mce_instance.mns_max_pkt_len) == 0)
    {
        PRINTF("MAX Packet Length - %d\r\n", app_mce_instance.mns_max_pkt_len);
    }
    else
    {
        PRINTF("MAX Packet Length is invalid\r\n");
    }
}

static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result)
{
    app_mce_instance.mce_mns = NULL;
    PRINTF("MCE MNS disconnection - 0x%02X\r\n", result);
}

static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas)
{
    app_mce_instance.mce_mas = mce_mas;
    PRINTF("MCE MAS connection\r\n");
    if (bt_map_mce_get_max_pkt_len(mce_mas, &app_mce_instance.max_pkt_len) == 0)
    {
        PRINTF("MAX Packet Length - %d\r\n", app_mce_instance.max_pkt_len);
    }
    else
    {
        PRINTF("MAX Packet Length is invalid\r\n");
    }
}

static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    app_mce_instance.mce_mas = NULL;
    PRINTF("MCE MAS disconnection - 0x%02X\r\n", result);
}

static void app_mce_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF("MAP Set Notification Registration CNF - 0x%02X\r\n", result);
}

static uint8_t map_mce_disconnect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
    int err = bt_map_mce_disconnect(app_mce_instance.mce_mas);

    if (err < 0)
    {
        PRINTF("disconnect failed Reason %d", err);
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t map_mce_connect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_map_mce_connect_cmd *cp = cmd;
    struct bt_conn *conn = bt_conn_lookup_addr_br(&cp->address.a);

	if (!conn) {
		return BTP_STATUS_FAILED;
	} else {
		bt_sdp_discover(conn, &discov_map_mse);
        return BTP_STATUS_SUCCESS;
	}
}

static uint8_t map_mce_set_ntf_reg(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    int err;
    struct net_buf *buf;
    const struct btp_map_mce_set_ntf_reg_cmd *cp = cmd;

    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_REG(app_mce_instance.mce_mas));
    BT_MAP_ADD_NOTIFICATION_STATUS(buf, cp->ntf_status);
    err = bt_map_mce_set_ntf_reg(app_mce_instance.mce_mas, buf);
    if (err != 0)
    {
        net_buf_unref(buf);
        PRINTF("set notification registration failed Reason %d\r\n", err);
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_MAP_MCE */

static const struct btp_handler handlers[] = {
#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
 	{
		.opcode = BTP_MAP_MCE_DISCONNECT,
		.expect_len = 0,
		.func = map_mce_disconnect,
	},
 	{
		.opcode = BTP_MAP_MCE_CONNECT,
		.expect_len = sizeof(struct btp_map_mce_connect_cmd),
		.func = map_mce_connect,
	},
 	{
		.opcode = BTP_MAP_MCE_SET_NTF_REG,
		.expect_len = sizeof(struct btp_map_mce_set_ntf_reg_cmd),
		.func = map_mce_set_ntf_reg,
	},
#endif /* CONFIG_BT_MAP_MCE */
};

uint8_t tester_init_map(void)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;

#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
    /* send hci write device class cmd */
    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MCE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        (void)bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return  BTP_STATUS_FAILED;
    }

    /* reigster SDP records */
    bt_sdp_register_service(&map_mce_rec);

    /* register mce callback */
    bt_map_mce_mas_register(&mce_mas_cb);
    bt_map_mce_mns_register(&mce_mns_cb);
#endif /* CONFIG_BT_MAP_MCE */

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
    /* send hci write device class cmd */
    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MSE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        (void)bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return  BTP_STATUS_FAILED;
    }

    /* reigster SDP records */
    bt_sdp_register_service(&map_mse_rec);

    /* register mse callback */
    bt_map_mse_mas_register(&mse_mas_cb);
    bt_map_mse_mns_register(&mse_mns_cb);
#endif /* CONFIG_BT_MAP_MSE */

    tester_register_command_handlers(BTP_SERVICE_ID_MAP, handlers,
                                    ARRAY_SIZE(handlers));

    return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_map(void)
{
    return BTP_STATUS_SUCCESS;
}
#endif /* CONFIG_BT_MAP_MCE || CONFIG_BT_MAP_MSE > 0 */