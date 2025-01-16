/* btp_pbap.c - Bluetooth PBAP Tester */

/*
 * Copyright (c) 2022 Codecoup
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_BT_PBAP) && (CONFIG_BT_PBAP > 0)

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include <porting.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include "bt_pal_conn_internal.h"
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include "BT_common.h"
#include "BT_pbap_api.h"
#include "BT_hci_api.h"
#include "BT_sm_api.h"
#include "BT_sdp_api.h"
#include "db_gen.h"
#include "btp.h"
#define PBAP_CLASS_OF_DEVICE (0x10020CU) /* Object Transfer, Phone, Smartphone */

static uint8_t conn_model = BTP_PBAP_PCE_L2CAP_PSM_CONN;
static char userid[16];
static char password[16];
#define SDP_CLIENT_USER_BUF_LEN 512U
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
/* PBAP appl parameters count*/
#define BT_PBAP_APPL_PARAM_HDR_COUNT 16U
struct pbap_hdr
{
    uint8_t *value;
    uint16_t length;
};

#if defined(CONFIG_BT_PBAP_PCE) && (CONFIG_BT_PBAP_PCE > 0)
#include <bluetooth/pbap_pce.h>
NET_BUF_POOL_FIXED_DEFINE(pbap_pce_appl_pool, CONFIG_BT_MAX_CONN, CONFIG_BT_PBAP_PCE_MAX_PKT_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

struct param
{
    uint8_t id;
    uint8_t length;
    uint8_t value[0];
};

uint8_t name[84];

static struct bt_sdp_attribute pbap_pce_attrs[] = {
    BT_SDP_NEW_SERVICE,
        BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        /* ServiceClassIDList */
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_PBAP_PCE_SVCLASS) //11 2E
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PBAP_SVCLASS) //11 30
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0102U) //01 02
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Phonebook Access PCE"),
};

static struct bt_sdp_record pbap_pce_rec = BT_SDP_RECORD(pbap_pce_attrs);

typedef struct app_pbap_pce_
{
    struct bt_pbap_pce *pbap_pceHandle;
    struct bt_conn *conn;
    uint8_t peer_bd_addr[6];
    uint16_t goep_version;
    uint16_t pbap_version;
    uint32_t peer_feature;
    uint32_t loacal_feature;
    uint16_t psm;
    uint16_t scn;
    uint8_t supported_repositories;
    bool lcl_srmp_wait; /* local srmp */
    bool rem_srmp_wait; /* remote srmp */
    int8_t num_srmp_wait;
} app_pbap_pce_t;
static app_pbap_pce_t g_PbapPce;

static uint8_t pce_auth = 0;
static struct bt_pbap_auth g_Pbap_pce_Auth;

static void app_pce_get_auth_info_cb(struct bt_pbap_pce *pbap_pce, struct bt_pbap_auth *pbap_auth_info)
{
    strcpy(userid, "PTS");
    pbap_auth_info->user_id = userid;
    strcpy(password, "0000");
    pbap_auth_info->pin_code = password;
}

static void app_pce_pull_phonebook_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        net_buf_unref(buf);
        return;
    }
  //  bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    net_buf_unref(buf);

    if (result == PBAP_CONTINUE_RSP)
    {
        g_PbapPce.lcl_srmp_wait = (--g_PbapPce.num_srmp_wait) > 0 ? true : false;

        buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
        if (buf == NULL)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));
        revert = bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
        if (revert != 0)
        {
            net_buf_unref(buf);
        }
    }
    return;
}

static void app_pce_pull_vcard_listing_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        net_buf_unref(buf);
        return;
    }
 //   bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    net_buf_unref(buf);

    if (result == PBAP_CONTINUE_RSP)
    {
        g_PbapPce.lcl_srmp_wait = (--g_PbapPce.num_srmp_wait) > 0 ? true : false;

        buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
        if (buf == NULL)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));
        if (conn_model == BTP_PBAP_PCE_RFCOMM_CONN)
        {
            OSA_TimeDelay(1000);
        }
        revert = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
        if (revert != 0)
        {
            net_buf_unref(buf);
        }
    }
    return;
}

static void app_pce_pull_vcard_entry_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
    int revert;
    if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
    {
        net_buf_unref(buf);
        return;
    }
 //   bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
    net_buf_unref(buf);

    if (result == PBAP_CONTINUE_RSP)
    {
        g_PbapPce.lcl_srmp_wait = (--g_PbapPce.num_srmp_wait) > 0 ? true : false;

        buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
        if (buf == NULL)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

        revert = bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
        if (revert != 0)
        {
            net_buf_unref(buf);
        }
    }
    return;
}

static struct bt_pbap_pce_cb pce_cb = {
    .connected     = NULL,
    .disconnected  = NULL,
    .get_auth_info = app_pce_get_auth_info_cb,
    .pull_phonebook     = app_pce_pull_phonebook_cb,
    .set_phonebook_path = NULL,
    .pull_vcard_listing = app_pce_pull_vcard_listing_cb,
    .pull_vcard_entry   = app_pce_pull_vcard_entry_cb,
};


static uint8_t bt_pbap_pce_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint32_t peer_feature = 0;
    uint16_t rfommchannel = 0;
    uint16_t l2cappsm     = 0;
    if ((result) && (result->resp_buf))
    {
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &peer_feature);
        if (res < 0)
        {
            g_PbapPce.peer_feature = BT_PBAP_SUPPORTED_FEATURES_V11;
        }
        else
        {
            g_PbapPce.peer_feature = peer_feature;
        }
        g_PbapPce.loacal_feature = CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE;
        bt_sdp_get_goep_l2cap_psm(result->resp_buf, &l2cappsm);
        bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &rfommchannel);
        if(conn_model == BTP_PBAP_PCE_L2CAP_PSM_CONN && l2cappsm > 0)
        {
            if(pce_auth)
            {
                bt_pbap_pce_psm_connect(conn, l2cappsm, &g_Pbap_pce_Auth, peer_feature, &g_PbapPce.pbap_pceHandle);
            }
            else
            {
                bt_pbap_pce_psm_connect(conn, l2cappsm, NULL, peer_feature, &g_PbapPce.pbap_pceHandle);
            }
            g_PbapPce.goep_version = BT_GOEP_VERSION_2_0;
            return BT_SDP_DISCOVER_UUID_STOP;
        }
        else
        {
            if(pce_auth)
            {
                bt_pbap_pce_scn_connect(conn, rfommchannel, &g_Pbap_pce_Auth, peer_feature, &g_PbapPce.pbap_pceHandle);
            }
            else
            {
                bt_pbap_pce_scn_connect(conn, rfommchannel, NULL, peer_feature, &g_PbapPce.pbap_pceHandle);
            }
            
            g_PbapPce.goep_version = BT_GOEP_VERSION_1_1;
        }
    }
    return BT_SDP_DISCOVER_UUID_STOP;
}

static struct bt_sdp_discover_params discov_pbap_pce = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_PBAP_PSE_SVCLASS),
    .func = bt_pbap_pce_sdp_user,
    .pool = &sdp_client_pool,
};

static uint8_t create_connect(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    int retval = 0;
    const struct btp_pbap_pce_connect_cmd *cp = cmd;
    struct bt_conn *conn = bt_conn_lookup_addr_br(&cp->address.a);
    conn_model = cp->model;

    if (!conn) {
		return BTP_STATUS_FAILED;
	} else {
		bt_sdp_discover(conn, &discov_pbap_pce);
        return BTP_STATUS_SUCCESS;
	}
}

static uint8_t pce_disconnect(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    API_RESULT retval = 0;
    retval            = bt_pbap_pce_disconnect(g_PbapPce.pbap_pceHandle);

    if (API_SUCCESS != retval)
    {
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t pce_connect_auth(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    pce_auth = 1;
    strcpy(userid, "PTS");
    g_Pbap_pce_Auth.user_id = userid;
    strcpy(password, "0000");
    g_Pbap_pce_Auth.pin_code = password;
    return BTP_STATUS_SUCCESS;
}

static void pce_add_params(uint8_t *cmd, uint16_t len, struct net_buf *buf)
{
    uint8_t *req_param = cmd;
    uint16_t req_len = len;
    uint8_t data_8 = 0;
    uint16_t data_16 = 0;
    uint64_t data_64 = 0;
    if(cmd == NULL || len == 0 || buf == NULL)
    {
        return;
    }
    while(req_len > 0)
    {
        struct param *pce_param = (struct param *)req_param;
        switch (pce_param->id)
        {
            case BTP_PBAP_PCE_NAME:
                memcpy(name, pce_param->value, pce_param->length);
                name[pce_param->length] = 0;
                break;
            
            case BTP_PBAP_PCE_APPL_MLC:
                data_16 = sys_get_be16(pce_param->value);
                BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, data_16);
                break;
            
            case BTP_PBAP_PCE_APPL_PROPERTY_SELECTOR:
                data_64 = sys_get_be64(pce_param->value);
                BT_PBAP_ADD_PARAMS_PROPERTY_SELECTOR(buf, data_64);
                break;
            
            case BTP_PBAP_PCE_APPL_RESET_NEW_MISSED_CALLS:
                data_8 = pce_param->value[0];
                BT_PBAP_ADD_PARAMS_RESET_NEW_MISSED_CALLS(buf, data_8);
                break;

            case BTP_PBAP_PCE_APPL_VCAED_SELECTOR:
                data_64 = sys_get_be64(pce_param->value);
                BT_PBAP_ADD_PARAMS_VCARD_SELECTOR(buf, data_64);
                break;

            case BTP_PBAP_PCE_APPL_SEARCH_PROPERTY:
                data_8 = pce_param->value[0];
                BT_PBAP_ADD_PARAMS_SEARCH_PROPERTY(buf, data_8);
                break;

            case BTP_PBAP_PCE_APPL_SEARCH_VALUE:
                BT_PBAP_ADD_PARAMS_SEARCH_VALUE(buf, pce_param->value, pce_param->length);
                break;
            
            case BTP_PBAP_PCE_APPL_VCARD_SELELCTOP_OPERATOR:
                data_8 = pce_param->value[0];
                BT_PBAP_ADD_PARAMS_VCARD_SELECTOR_OPERATOR(buf, data_8);
                break;

        }
        req_len -= pce_param->length;
        req_len -= sizeof(struct param);
        req_param += sizeof(struct param);
        req_param += pce_param->length;
    }
}

static uint8_t pce_pull_phonebook(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{

    API_RESULT retval = 0;
    struct net_buf *buf;
    uint8_t *req_param = (uint8_t *)cmd;
    uint16_t req_len = cmd_len;

    buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
    if (!buf)
    {
         return BTP_STATUS_FAILED;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));
    
    BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 65535);
    pce_add_params(req_param, req_len, buf);

    g_PbapPce.num_srmp_wait = 0;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

    retval = bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, (char *)name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    if (API_SUCCESS != retval)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
    
}

static uint8_t pce_set_path(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    API_RESULT retval = 0;
    struct net_buf *buf;
    uint8_t *req_cmd = (uint8_t *)cmd;
    uint8_t length = req_cmd[0];
    memset(name, 0, sizeof(name));
    buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
    if (!buf)
    {
        return BTP_STATUS_FAILED;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_SET_PATH(g_PbapPce.pbap_pceHandle));

    memcpy(name, req_cmd + 1, length);
    retval = bt_pbap_pce_set_phonebook_path(g_PbapPce.pbap_pceHandle, buf, (char *)name);
    if (retval != API_SUCCESS)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t pce_pull_vcard_listing(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    API_RESULT retval = 0;
    struct net_buf *buf;
    uint8_t *req_param = (uint8_t *)cmd;
    uint16_t req_len = cmd_len;

    buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
    if (!buf)
    {
         return BTP_STATUS_FAILED;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));
    
    BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 65535);

    pce_add_params(req_param, req_len, buf);

    g_PbapPce.num_srmp_wait = 0;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

    retval = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, (char *)name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    if (API_SUCCESS != retval)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

static uint8_t pce_pull_vcard_entry(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    API_RESULT retval = 0;
    struct net_buf *buf;
    uint8_t *req_param = (uint8_t *)cmd;
    uint16_t req_len = cmd_len;

    buf = net_buf_alloc(&pbap_pce_appl_pool, osaWaitNone_c);
    if (!buf)
    {
         return BTP_STATUS_FAILED;
    }
    net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));
    
    pce_add_params(req_param, req_len, buf);

    g_PbapPce.num_srmp_wait = 0;
    g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

    retval = bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, (char *)name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
    if (API_SUCCESS != retval)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

static uint8_t pce_abort(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
{
    int result = 0;
    result = bt_pbap_pce_abort(g_PbapPce.pbap_pceHandle);
    if (result < 0)
    {
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_PBAP_PCE */



#if defined(CONFIG_BT_PBAP_PSE) && (CONFIG_BT_PBAP_PSE > 0)
#include <bluetooth/pbap_pse.h>
struct appl_params
{
    /**
     * Attribute Mask
     */
    uint64_t property_selector;

    /**
     * Maximum entries in the list
     */
    uint16_t max_list_count;

    /**
     * Offset of the first entry
     */
    uint16_t list_start_offset;

    /**
     * Number of indexes in phonebook
     */
    uint16_t phonebook_size;

    /**
     * Application Parameter Bit field
     */

    /**
     *  16 Bit Flag used as below
     *
     *  Usage:
     *       15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
     *      [  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ]
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |  |____Order
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |_______Search Value
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |__________Search Attribute
     *        |  |  |  |  |  | |  |   |  |  |  |  |_____________Max List Count
     *        |  |  |  |  |  | |  |   |  |  |  |________________List Start Offset
     *        |  |  |  |  |  | |  |   |  |  |___________________Property Selector
     *        |  |  |  |  |  | |  |   |  |______________________Format
     *        |  |  |  |  |  | |  |   |_________________________Phone book size
     *        |  |  |  |  |  | |  |_____________________________New Missed Calls
     *        |  |  |  |  |  | |________________________________PrimaryVersionCounter
     *        |  |  |  |  |  |__________________________________SecondaryVersionCounter
     *        |  |  |  |  |_____________________________________vCardSelector
     *        |  |  |  |________________________________________DatabaseIdentifier
     *        |  |  |___________________________________________vCardSelectorOperator
     *        |  |______________________________________________ResetNewMissedCalls
     *        |_________________________________________________PbapSupportedFeatures
     *
     *
     */
    uint16_t appl_param_flag;

    /**
     * Sorting Order
     */
    uint8_t order;

    /**
     * Attribute to be searched
     */
    uint8_t search_attr;

    /**
     * vCard Format 2.1 or 3.0
     */
    uint8_t format;

    /**
     * Number of new missed calls
     */
    uint8_t new_missed_calls;
    /**
     * PSE primary folder version
     */
    uint8_t primary_folder_ver[16];

    /**
     * PSE secodary folder version
     */
    uint8_t secondary_folder_ver[16];

    /**
     * PSE database identifier
     */
    uint8_t database_identifier[16];

    /**
     * vCard Selector, Attribute mask
     */
    uint64_t vcard_selector;

    /**
     * vCard selector operator when multiple
     * attributes are slector
     */
    uint8_t vcard_selector_operator;

    /**
     * To reset newmissed calls
     */
    uint8_t reset_new_missed_calls;

    /**
     * PBAP supported features
     */
    uint32_t supported_features;

    /**
     * Value of the Search attribute
     */
    struct pbap_hdr search_value;
};
#define CURRENT_PATH_MAX_LEN 30
typedef struct app_pbap_pse_
{
    struct bt_pbap_pse *pbap_pseHandle;
    struct bt_conn *conn;
    uint8_t peer_bd_addr[6];
    char currentpath[CURRENT_PATH_MAX_LEN];
    uint32_t remaining_rsp;
    struct appl_params req_appl_params;
    char name[42];
    uint32_t send_rsp;
    uint32_t lcl_supported_features;
    uint32_t rem_supported_features;
} app_pbap_pse_t;

app_pbap_pse_t g_PbapPse;

enum pbap_pse_command
{
    PBAP_PSE_PULL_PHONE_BOOK = 0x1,
    PBAP_PSE_PULL_VCARD_LISTING = 0x2,
};
#define PBAP_PSE_MAX_PKT_SIZE OBEX_MAX_PACKET_LENGTH

NET_BUF_POOL_FIXED_DEFINE(pbap_appl_pse_pool, CONFIG_BT_MAX_CONN, PBAP_PSE_MAX_PKT_SIZE, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

char *child_floader_name[8] = {"telecom", "pb", "ich", "och", "mch", "cch", "spd", "fav"};
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
    uint8_t str_len    = BT_str_len(str);
    uint8_t suffix_len = BT_str_len(suffix);
    if (str_len < suffix_len)
    {
        return 0;
    }
    char *endpart = str + str_len - suffix_len;
    return strcmp(endpart, suffix) == 0;
}

uint8_t feature = 0;

static uint8_t sample_primay_folder_version[16] =
       {
          0x05U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U
       };

static uint8_t sample_secondary_folder_version[16] =
       {
          0x06U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U
       };

static uint8_t sample_database_identifier[16] =
       {
          0x07U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x03U
       };
static char pbap_pse_phonebook_example[] =
    "BEGIN:VCARD\n\
VERSION:2.1\n\
FN;CHARSET=UTF-8:descvs\n\
N;CHARSET=UTF-8:descvs\n\
END:VCARD\n\
BEGIN:VCARD\n\
VERSION:2.1\n\
N:;cc;;;\n\
FN:cc\n\
TEL;CELL:154555845\n\
END:VCARD";

static const char pbap_pse_vcard_listing[] =
    "<?xml version=\"1.0\"?><!DOCTYPE vcard-listing SYSTEM \"vcard-listing.dtd\"><vCard-listing version=\"1.0\">\
<card handle=\"1.vcf\" name=\"qwe\"/><card handle=\"2.vcf\" name=\"qwe\"/><card handle=\"3.vcf\" name=\"qwe\"/>\
<card handle=\"4.vcf\" name=\"1155\"/><card handle=\"5.vcf\" name=\"051295205593\"/><card handle=\"6.vcf\" name=\"130\"/>/vCard-listing>";

static const char pbap_pse_vcard_entry[] =
    "BEGIN:VCARD\n\
VERSION:2.1\n\
FN:\n\
N:\n\
TEL;X-0:1155\n\
X-IRMC-CALL-DATETIME;DIALED:20220913T110607\n\
END:VCARD";

static struct bt_sdp_attribute pbap_pse_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_PBAP_PSE_SVCLASS) //11 2F
        },
        )
    ),
    /* ProtocolDescriptorList */
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
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_PBAP_PSE) //RFCOMM CHANNEL
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
                BT_SDP_ARRAY_16(BT_SDP_PBAP_SVCLASS) //11 30
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0101U) //01 01
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Phonebook Access PSE"),
     /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16), 
        BT_SDP_ARRAY_16(BT_BR_PSM_PBAP_PSE)
    },

    /* SupportedRepositories */
    BT_SDP_ATTR_SUPPORTED_REPOSITORIES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), 
        BT_SDP_ARRAY_8(CONFIG_BT_PBAP_PSE_SUPPORTED_REPOSITORIES)
    },

    /* PBAP_PSE SupportedFeatures */
    BT_SDP_ATTR_PBAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32), 
        BT_SDP_ARRAY_32(CONFIG_BT_PBAP_PSE_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record pbap_pse_rec = BT_SDP_RECORD(pbap_pse_attrs);

static uint8_t pse_auth = 0;

static void app_pse_get_auth_info_cb(struct bt_pbap_pse *pbap_pse, struct bt_pbap_auth *pbap_auth_info, bool *active_auth)
{       
    strcpy(userid, "PTS");
    pbap_auth_info->user_id = userid;
    strcpy(password, "0000");
    pbap_auth_info->pin_code = password;
    *active_auth = pse_auth;
}

static int app_check_pull_phonebook_path(char *name)
{
    uint8_t length = BT_str_len(name);
    int8_t index  = 0;
    uint8_t book_name[10] = {0};
    uint8_t end = 0; 
    if (name == NULL)
    {
        return BT_PBAP_BAD_REQ_RSP; 
    }
    if (endwith((char *)name, ".vcf") == 0)
    {
        return BT_PBAP_BAD_REQ_RSP;
    }
    index = BT_str_len(name) -1;
    while(index >= 0)
    {
        if (name[index] == '.')
        {
            end = index;
        }
        else if (name[index] == '/')
        {
            strncpy((char *)book_name, (char *)name + index + 1, end - index -1);
            break;
        }
        index--;
    }

    for (index = 1; index < 8; index++)
    {
        if (strcmp((char *)book_name, child_floader_name[index]) == 0)
        {
            break;
        }
    }
    if (index == 8)
    {
        return BT_PBAP_BAD_REQ_RSP;
    }
    index = 0;
    /* Check if requested phonebook file has 'SIMI' */
    char *pb_ptr = (char *)BT_str_str(name, "SIM1");
    if (NULL == pb_ptr)
    {
        /* pb_file_name does not have 'SIM1', check for 'telecom' */
        pb_ptr = (char *)BT_str_str(name, "telecom");
        if (NULL != pb_ptr)
        {
            
        }
        else
        {
            /* Requested pb_file_name is not valid, Send Error */
            return BT_PBAP_NOT_FOUND_RSP;
        }
    }
    else
    {
        index += 5U;
        pb_ptr = (char *)BT_str_str(&name[index], (char *)"telecom");
        if (NULL != pb_ptr)
        {
            
        }
        else
        {
            /* Requested pb_file_name is not valid, Send Error */
            return BT_PBAP_NOT_FOUND_RSP;
        }
    }
    return 0;
}

static bool app_pse_param_cb(struct bt_data *data, void *user_data)
{
    switch (data->type)
    {
        case BT_PBAP_TAG_ID_MAX_LIST_COUNT:
            g_PbapPse.req_appl_params.max_list_count = sys_get_be16(data->data);
            PBAP_SET_APPL_PARAM_FLAG(g_PbapPse.req_appl_params.appl_param_flag, (uint16_t)1U << (data->type -1) % BT_PBAP_APPL_PARAM_HDR_COUNT);
            break;
        default:
            break;
    }
    
    return 1;
}

static uint8_t app_pse_appl_param_response(struct net_buf *buf, app_pbap_pse_t *g_PbapPse, enum pbap_pse_command command)
{
    uint8_t send_body = 1;
    for (uint8_t index = 0; index < BT_PBAP_APPL_PARAM_HDR_COUNT; ++index)
    {
        if (PBAP_GET_APPL_PARAM_FLAG(g_PbapPse->req_appl_params.appl_param_flag, (uint16_t)1U << index % BT_PBAP_APPL_PARAM_HDR_COUNT) != 0)
        {
            switch (index + 1)
            {
                case  BT_PBAP_TAG_ID_MAX_LIST_COUNT:
                    if (g_PbapPse->req_appl_params.max_list_count == 0)
                    {
                        BT_PBAP_ADD_PARAMS_PHONE_BOOK_SIZE(buf, 2);
                        send_body = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if (g_PbapPse->rem_supported_features & BT_PBAP_FEATURE_ENHANCED_MISSED_CALLS || strstr(g_PbapPse->name, "mch") != NULL)
    {
        BT_PBAP_ADD_PARAMS_NEW_MISSED_CALLS(buf, 1);
    }
    if(g_PbapPse->rem_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER)
    {
        BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, sample_database_identifier);
    }
    if (g_PbapPse->rem_supported_features & BT_PBAP_FEATURE_FOLDER_VERSION_COUNTERS)
    {
        BT_PBAP_ADD_PARAMS_PRIMARY_FOLDER_VERSION(buf, sample_primay_folder_version);
        BT_PBAP_ADD_PARAMS_SECONDARY_FOLDER_VERSION(buf, sample_secondary_folder_version);
    }
    if(send_body)
    {
        if (command == PBAP_PSE_PULL_PHONE_BOOK)
        {
            g_PbapPse->remaining_rsp = BT_str_len(pbap_pse_phonebook_example);
        }
        else if(command == PBAP_PSE_PULL_VCARD_LISTING)
        {
            g_PbapPse->remaining_rsp = BT_str_len(pbap_pse_vcard_listing);
        }

        if (g_PbapPse->remaining_rsp == 0)
        {
            return BT_PBAP_NOT_FOUND_RSP;
        }
        else
        {
            g_PbapPse->send_rsp = 0;
        }
    }
    return BT_PBAP_SUCCESS_RSP; 

};

static void app_pse_pull_phonebook_cb(struct bt_pbap_pse *pbap_pse, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    memset(&g_PbapPse.req_appl_params, 0, sizeof(g_PbapPse.req_appl_params));
    bt_pbap_pse_app_param_parse(buf, app_pse_param_cb, NULL);
    net_buf_unref(buf);
    if (name != NULL)
    {
        memcpy(g_PbapPse.name, name, BT_str_len(name) + 1);
    }
    buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
    if (!buf)
    {
        return;
    }
    net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {
        if (g_PbapPse.remaining_rsp == 0)
        {
            result = app_check_pull_phonebook_path(g_PbapPse.name);
            if (result == 0)
            {
                memcpy(g_PbapPse.currentpath, "root\0", BT_str_len("root\0"));
                result = app_pse_appl_param_response(buf, &g_PbapPse, PBAP_PSE_PULL_PHONE_BOOK);
            }
            else
            {
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {
        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= sizeof(struct bt_pbap_push_response_hdr);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_phonebook_example[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp  = 0;
            result                   = BT_PBAP_SUCCESS_RSP;
            g_PbapPse.currentpath[4] = 0;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_phonebook_example[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }
    revert = bt_pbap_pse_pull_phonebook_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
    }
}

static void app_pse_set_phonebook_path_cb(struct bt_pbap_pse *pbap_pse, char *name)
{
    int revert      = 0;
    uint8_t result  = 0;
    uint8_t index   = 0;
    char *path_name = NULL;
    if (name == NULL)
    {
        return;
    }
    if (strcmp(name, "/") == 0)
    {
        memset(g_PbapPse.currentpath, 0, sizeof(g_PbapPse.currentpath));
        memcpy(g_PbapPse.currentpath, "root\0", 5);
        result = BT_PBAP_SUCCESS_RSP;
    }
    else if (strcmp(name, "..") == 0)
    {
        if (strcmp(g_PbapPse.currentpath, "root") == 0)
        {
            result = BT_PBAP_NOT_FOUND_RSP;
        }
        else
        {
            for (int8_t index = BT_str_len(g_PbapPse.currentpath); index >= 0; index--)
            {
                if (g_PbapPse.currentpath[index] == '/')
                {
                    g_PbapPse.currentpath[index] = 0;
                    break;
                }
            }
            result = BT_PBAP_SUCCESS_RSP;
        }
    }
    else if (name[0] == '.' && name[1] == '/' && BT_str_len(name) > 2)
    {
        path_name = &name[2];
        if (strcmp(path_name, "SIM1") == 0)
        {
            if (strcmp(g_PbapPse.currentpath, "root") == 0)
            {
                strcat(g_PbapPse.currentpath, "/");
                strcat(g_PbapPse.currentpath, path_name);
                result = BT_PBAP_SUCCESS_RSP;
            }
            else
            {
                result = BT_PBAP_NOT_FOUND_RSP;
            }
        }
        for (index = 0; index < 8; index++)
        {
            if (strcmp(path_name, child_floader_name[index]) == 0)
            {
                if (index == 0)
                {
                    if (endwith(g_PbapPse.currentpath, "root") || endwith(g_PbapPse.currentpath, "SIM1"))
                    {
                        strcat(g_PbapPse.currentpath, "/");
                        strcat(g_PbapPse.currentpath, path_name);
                        result = BT_PBAP_SUCCESS_RSP;
                        break;
                    }
                    else
                    {
                        result = BT_PBAP_NOT_FOUND_RSP;
                        break;
                    }
                }
                else
                {
                    if (endwith(g_PbapPse.currentpath, "telecom"))
                    {
                        strcat((char *)g_PbapPse.currentpath, "/");
                        strcat((char *)g_PbapPse.currentpath, path_name);
                        result = BT_PBAP_SUCCESS_RSP;
                        break;
                    }
                    else
                    {
                        result = BT_PBAP_NOT_FOUND_RSP;
                        break;
                    }
                }
            }
        }
        if (index >= 8)
        {
            result = BT_PBAP_NOT_FOUND_RSP;
        }
    }
    revert = bt_pbap_pse_set_phonebook_path_response(pbap_pse, result);
    if (API_SUCCESS != revert)
    {
        return ;
    }
}

static int app_check_pull_vcard_listing_path(char *name)
{
    uint8_t index = 0;
    for (index = 0; index < 8; index++)
    {
        if (strcmp(name, child_floader_name[index]) == 0)
        {
            break;
        }
    }
    if (index == 8)
    {
        return BT_PBAP_NOT_FOUND_RSP;
    }
    if (endwith(g_PbapPse.currentpath, "telecom"))
    {
        return BT_PBAP_SUCCESS_RSP;
    }
    else
    {
        return BT_PBAP_NOT_FOUND_RSP;
    }
}

static void app_pse_pull_vcard_listing_cb(struct bt_pbap_pse *pbap_pse, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    bt_pbap_pse_app_param_parse(buf, app_pse_param_cb, NULL);
    net_buf_unref(buf);
    buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
    if (!buf)
    {
        return;
    }
    net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
    if (name != NULL)
    {
        memcpy(g_PbapPse.name, name, BT_str_len(name) + 1);
    }
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {

        if (g_PbapPse.remaining_rsp == 0)
        {
            if (name != NULL)
            {
                result = app_check_pull_vcard_listing_path(g_PbapPse.name);
            }
            if (result == BT_PBAP_SUCCESS_RSP)
            {
                result = app_pse_appl_param_response(buf, &g_PbapPse, PBAP_PSE_PULL_VCARD_LISTING);
            }
            else
            {
                result                  = BT_PBAP_NOT_FOUND_RSP;
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {

        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= sizeof(struct bt_pbap_push_response_hdr);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_vcard_listing[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp = 0;
            result                  = BT_PBAP_SUCCESS_RSP;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_vcard_listing[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }
    revert = bt_pbap_pse_pull_vcard_listing_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
    }
}

static int app_check_pull_vcard_entry_path(char *name)
{
    uint8_t index = 1;
    if (name == NULL)
    {
        return -EINVAL;
    }
    if (endwith(name, ".vcf") == 0)
    {
        if (startwith(name, "X-BT-UID") == 0)
        {
            return -EINVAL;
        }
    }
    for (index = 1; index < 8; index++)
    {
        if (endwith(g_PbapPse.currentpath, child_floader_name[index]))
        {
            break;
        }
    }
    if (index >= 8)
    {
        return -EINVAL;
    }
    return 0;
}

static void app_pse_pull_vcard_entry_cb(struct bt_pbap_pse *pbap_pse,
                                        struct net_buf *buf,
                                        char *name,
                                        enum bt_obex_req_flags flag)
{
    struct pbap_hdr body;
    int revert           = 0;
    uint8_t result       = BT_PBAP_SUCCESS_RSP;
    uint16_t max_pkt_len = 0;
    if (pbap_pse == NULL || buf == NULL)
    {
        return;
    }
    bt_pbap_pse_app_param_parse(buf, app_pse_param_cb, NULL);
    net_buf_unref(buf);
    if (name != NULL)
    {
        memcpy(g_PbapPse.name, name, BT_str_len(name) + 1);
    }
    if (!(flag & BT_OBEX_REQ_UNSEG)) /* stack do not support now */
    {
        g_PbapPse.remaining_rsp = 0;
        result                  = BT_PBAP_NOT_IMPLEMENTED_RSP;
    }
    else
    {
        if (g_PbapPse.remaining_rsp == 0)
        {
            if (app_check_pull_vcard_entry_path(g_PbapPse.name) == 0)
            {
                g_PbapPse.remaining_rsp = BT_str_len(pbap_pse_vcard_entry);
                if (g_PbapPse.remaining_rsp == 0)
                {
                    result = BT_PBAP_NOT_FOUND_RSP;
                }
                else
                {
                    g_PbapPse.send_rsp = 0;
                }
            }
            else
            {
                buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
                if (!buf)
                {
                    return;
                }
                net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
                result                  = BT_PBAP_NOT_FOUND_RSP;
                g_PbapPse.remaining_rsp = 0;
            }
        }
    }
    if (g_PbapPse.remaining_rsp != 0)
    {
        buf = net_buf_alloc(&pbap_appl_pse_pool, osaWaitNone_c);
        if (!buf)
        {
            return;
        }
        net_buf_reserve(buf, BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse));
        if ((g_PbapPse.lcl_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER) &&
            (g_PbapPse.rem_supported_features & BT_PBAP_FEATURE_DATABASE_IDENTIFIER))
        {
            BT_PBAP_ADD_PARAMS_DATABASE_IDENTIFIER(buf, sample_database_identifier);
        }
        bt_pbap_pse_get_max_pkt_len(pbap_pse, &max_pkt_len);
        max_pkt_len -= sizeof(struct bt_pbap_push_response_hdr);
        max_pkt_len -= buf->len;
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes);
        if (g_PbapPse.remaining_rsp <= max_pkt_len)
        {
            body.value  = (uint8_t *)(uint8_t *)&pbap_pse_vcard_entry[g_PbapPse.send_rsp];
            body.length = g_PbapPse.remaining_rsp;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp = 0;
            result                  = BT_PBAP_SUCCESS_RSP;
        }
        else
        {
            body.value  = (uint8_t *)&pbap_pse_vcard_entry[g_PbapPse.send_rsp];
            body.length = max_pkt_len;
            bt_obex_add_hdr(buf, BT_OBEX_HDR_BODY, body.value, body.length);
            g_PbapPse.remaining_rsp -= body.length;
            g_PbapPse.send_rsp += body.length;
            result = BT_PBAP_CONTINUE_RSP;
        }
    }
    revert = bt_pbap_pse_pull_vcard_entry_response(pbap_pse, result, buf, 0);
    if (revert != 0)
    {
        net_buf_unref(buf);
    }
}

static void app_pse_connected(struct bt_pbap_pse *pbap_pse)
{
    memcpy(g_PbapPse.currentpath, "root", 4);
    g_PbapPse.pbap_pseHandle         = pbap_pse;
    g_PbapPse.lcl_supported_features = CONFIG_BT_PBAP_PSE_SUPPORTED_FEATURES;
    bt_pbap_pse_get_peer_supported_features(pbap_pse, &g_PbapPse.rem_supported_features);
    g_PbapPse.send_rsp      = 0;
    g_PbapPse.remaining_rsp = 0;
}

static void app_pse_disconnected(struct bt_pbap_pse *pbap_pse, uint8_t result)
{
    g_PbapPse.send_rsp      = 0;
    g_PbapPse.remaining_rsp = 0;
    memcpy(g_PbapPse.currentpath, "root", 4);
}

static struct bt_pbap_pse_cb pse_cb = {
    .connected          = app_pse_connected,
    .disconnected       = app_pse_disconnected,
    .get_auth_info      = app_pse_get_auth_info_cb,
    .pull_phonebook     = app_pse_pull_phonebook_cb,
    .set_phonebook_path = app_pse_set_phonebook_path_cb,
    .pull_vcard_listing = app_pse_pull_vcard_listing_cb,
    .pull_vcard_entry   = app_pse_pull_vcard_entry_cb,
};

static uint8_t pse_connect_auth(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
    pse_auth = 1;
    return BTP_STATUS_SUCCESS;
}

static uint8_t pse_reset_dbi(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
    sample_database_identifier[0] = 0xFF;
    return BTP_STATUS_SUCCESS;
}

static uint8_t pse_modify_contact(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
    
    memcpy(pbap_pse_phonebook_example, (char *)cmd, cmd_len);
    sample_primay_folder_version[0] = 0xA;
    if (strstr(pbap_pse_phonebook_example, "VERSION:2.1") == 0)
    {
        sample_secondary_folder_version[0] = 0xB;
    }
    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_PBAP_PSE */


static uint8_t supported_commands(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_pbap_read_supported_commands_rp *rp = rsp;

	/* octet 0 */
	tester_set_bit(rp->data, BTP_PBAP_READ_SUPPORTED_COMMANDS);
        tester_set_bit(rp->data, BTP_PBAP_MODIFY_VERSION);

	*rsp_len = sizeof(*rp) + 1;

	return BTP_STATUS_SUCCESS;
}

 static uint8_t modify_version(const void *cmd, uint16_t cmd_len,
 				  void *rsp, uint16_t *rsp_len)
 {
    int err;
    struct btp_pbap_modify_version_req *buf = (struct btp_pbap_modify_version_req *)cmd;
    uint8_t role = buf->data[0];
    /* PBAP_PSE SDP record handle */
    static uint32_t pbap_pse_record_handle;
    /* PBAP_PSE SDP record handle */
    static uint32_t pbap_pce_record_handle;
    uint8_t attr_value[10] = {0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x30, 0x09, 0x01, 0x02};
    if (role == 0x01)
    {
        BT_dbase_get_record_handle(DB_RECORD_PBAP_PSE, 1, &pbap_pse_record_handle);
        BT_dbase_inactivate_record(pbap_pse_record_handle);
        BT_dbase_update_attr_value(pbap_pse_record_handle, 0x0009, attr_value, 10);
        BT_dbase_activate_record(pbap_pse_record_handle);
    }
    else
    {
        BT_dbase_get_record_handle(DB_RECORD_PBAP_PCE, 1, &pbap_pce_record_handle);
        BT_dbase_inactivate_record(pbap_pce_record_handle);
        BT_dbase_update_attr_value(pbap_pce_record_handle, 0x0009, attr_value, 10);
        BT_dbase_activate_record(pbap_pce_record_handle);
    }

    return BTP_STATUS_SUCCESS;
 }





static const struct btp_handler handlers[] = {
    {
        .opcode = BTP_PBAP_READ_SUPPORTED_COMMANDS,
        .index = BTP_INDEX_NONE,
        .expect_len = 0,
        .func = supported_commands,
    },
    {
        .opcode = BTP_PBAP_MODIFY_VERSION,
        .index = BTP_INDEX,
        .expect_len = 1,
        .func = modify_version,
    },
#if defined(CONFIG_BT_PBAP_PCE) && (CONFIG_BT_PBAP_PCE > 0)
    {
        .opcode = BTP_PBAP_PCE_CONNECT,
        .index = BTP_INDEX,
        .expect_len = sizeof(struct btp_pbap_pce_connect_cmd),
        .func = create_connect,
    },
    {
        .opcode = BTP_PBAP_PCE_DISCONNECT,
        .index = BTP_INDEX,
        .expect_len = 0,
        .func = pce_disconnect,
    },
    {
        .opcode = BTP_PBAP_PCE_CONNECT_AUTH,
        .index = BTP_INDEX,
        .expect_len = 0,
        .func = pce_connect_auth,
    },
    {
        .opcode = BTP_PBAP_PULL_PHONEBOOK,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pce_pull_phonebook,
    },
    {
        .opcode = BTP_PBAP_PCE_SET_PATH,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pce_set_path,
    },
    {
        .opcode = BTP_PBAP_PCE_PULL_VCARD_LISTING,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pce_pull_vcard_listing,
    },
    {
        .opcode = BTP_PBAP_PCE_PULL_VCARD_ENTRY,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pce_pull_vcard_entry,
    },
    {
        .opcode = BTP_PBAP_PCE_ABORT,
        .index = BTP_INDEX,
        .expect_len = -1, 
        .func = pce_abort,
    },
#endif 
#if defined(CONFIG_BT_PBAP_PSE) && (CONFIG_BT_PBAP_PSE > 0)
    {
        .opcode = BTP_PBAP_PSE_CONNECT_AUTH,
        .index = BTP_INDEX,
        .expect_len = 0,
        .func = pse_connect_auth,
    },
    {
        .opcode = BTP_PBAP_PSE_RESET_DBI,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pse_reset_dbi,
    },
    {
        .opcode = BTP_PBAP_PSE_MODIFY_CONTACT,
        .index = BTP_INDEX,
        .expect_len = -1,
        .func = pse_modify_contact,
    },
#endif 
};


uint8_t tester_init_pbap(void)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err = 0;
    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(PBAP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return BTP_STATUS_FAILED;
    }
    if (err < 0)
    {
        return BTP_STATUS_FAILED;
    }
#if defined(CONFIG_BT_PBAP_PCE) && (CONFIG_BT_PBAP_PCE > 0)
    /* reigster PBAP PCE SDP records */
    bt_sdp_register_service(&pbap_pce_rec);
    err = bt_pbap_pce_register(&pce_cb);
    if (err < 0)
    {
        return BTP_STATUS_FAILED;
    }
#endif /* CONFIG_BT_PBAP_PCE */

#if defined(CONFIG_BT_PBAP_PSE) && (CONFIG_BT_PBAP_PSE > 0)
    bt_sdp_register_service(&pbap_pse_rec);
    err = bt_pbap_pse_register(&pse_cb);
        
    if (err < 0)
    {
        return BTP_STATUS_FAILED;
    }
#endif /* CONFIG_BT_PBAP_PSE*/

    tester_register_command_handlers(BTP_SERVICE_ID_PBAP, handlers,
					 ARRAY_SIZE(handlers));
    
    
    return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_pbap(void)
{
    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_PBAP */