/*
 * Copyright 2021, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#include <bluetooth/hfp_ag.h>
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_rfcomm_internal.h"
#include "bt_pal_at.h"
#include "bt_pal_hfp_internal.h"
#include "fsl_os_abstraction.h"
#include "BT_features.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_avdtp_api.h"
#include "BT_a2dp_api.h"
#include "BT_config.h"
#include "BT_jpl_api.h"
#include "BT_fops.h"
#include "BT_features.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_hfp_ag_api.h"
#include "BT_at_parser_api.h"
#include "db_gen.h"
#include "sco_audio_pl.h"
#include "bt_pal_sco_internal.h"

#if (defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U))
#if (defined (CONFIG_BT_HFP_AG) && (CONFIG_BT_HFP_AG > 0U))

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_HFP_AG)
#define LOG_MODULE_NAME bt_hfp_ag
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

typedef struct _hfp_ag_clcc_t
{
    int index;
    uint8_t dir;
    uint8_t current_call_state;
    uint8_t mode;
    uint8_t mpty;
    char *number;
    uint8_t type;
} hfp_ag_clcc_t;

struct bt_hfp_ag
{
    struct bt_sco_chan sco_chan;
    /* Flag - Whether allocated or free */
    uint8_t allocated;
    uint8_t serverChannel;
    uint8_t peerAddr[BT_BD_ADDR_SIZE];
    uint16_t sco_connection_handle;
    struct bt_conn *acl_so_conn;
    hfp_ag_get_config *bt_hfp_ag_config;
    uint8_t bt_hfp_ag_bind[2];
    char ag_str_cops[16];
    hfp_ag_cind_t bt_cind_setting;
    hfp_ag_cind_t cind;
    uint32_t hf_features;
    uint32_t ag_features;
};

static struct bt_hfp_ag_cb *bt_hfp_ag_cb;
static OSA_MUTEX_HANDLE_DEFINE(s_HfpAgLockMutex);
static osa_mutex_handle_t s_HfpAgLock;
static struct bt_hfp_ag s_HfpAgInstances[CONFIG_BT_MAX_CONN];
#define EDGEFAST_HFP_AG_LOCK   OSA_MutexLock(s_HfpAgLock, osaWaitForever_c)
#define EDGEFAST_HFP_AG_UNLOCK OSA_MutexUnlock(s_HfpAgLock)

#define SDP_CLIENT_USER_BUF_LEN 512U
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
struct bt_hfp_sdp
{
    struct bt_conn *bt_conn;
    bt_hfp_ag_discover_callback discoverCallback;
    uint8_t allocated;
};
static struct bt_hfp_sdp s_hfp_sdp[CONFIG_BT_MAX_CONN];
static struct bt_hfp_ag *s_actived_bt_hfp_ag = NULL;
/* Other definitions */
#define BT_HFP_AG_STATE_DISCONNECTED 0x00
#define BT_HFP_AG_STATE_CONNECTED    0x01

static uint8_t bt_hfp_agag_state;
#ifndef SDP_DYNAMIC_DB
static uint8_t hfp_ag_local_supported_features[6];
static uint16_t hfp_ag_local_supported_features_ext;
#endif
static void bt_hfp_ag_send_at_rsp(uint8_t rsp_code, void *value);
static void bt_hfp_ag_dump_bytes(uint8_t *buffer, uint16_t length);
extern void bt_sco_cleanup(struct bt_conn *sco_conn);

/* ---------------------------------------- Global Definitions */
/* Response to SLC AT Commands */
#define HFAG_OK_STR        (CHAR *)"\r\nOK\r\n"
#define HFAG_ERROR_STR     (CHAR *)"\r\nERROR\r\n"
#define HFAG_BRSF_STR      (CHAR *)"\r\n+BRSF: "
#define HFAG_BCS_STR       (CHAR *)"\r\n+BCS: "
#define HFAG_BIND_TEST_STR (CHAR *)"\r\n+BIND: (1,2)\r\n"
#define HFAG_BIND_READ_STR (CHAR *)"\r\n+BIND: "
#define HFAG_BVRA_STR      (CHAR *)"\r\n+BVRA: "
#define HFAG_BINP_STR      (CHAR *)"\r\n+BINP: "
#define HFAG_BSIR_STR      (CHAR *)"\r\n+BSIR: "
#define HFAG_VGS_STR       (CHAR *)"\r\n+VGS: "
#define HFAG_VGM_STR       (CHAR *)"\r\n+VGM: "

#define HFP_CR_LF (CHAR *)"\r\n"

#define HFP_AG_ENABLE_VOICE_REG  (CHAR *)"\r\n+BVRA: 1\r\n"
#define HFP_AG_DISABLE_VOICE_REG (CHAR *)"\r\n+BVRA: 0\r\n"

/* + BVRA: 1, 1 */
#define HFP_AG_ENH_VR_MSG (CHAR *)"\r\n+BVRA: 1,"

/* + BVRA: 1, 1, 12AB, 0, 1, “Message to Melissa?*/
#define HFP_AG_ENH_VR_AND_VR_TXT_MSG (CHAR *)"\r\n+BVRA: 1,5,12AB,0,1,\"Message to Melissa\"\r\n"
#define HFAG_ENHVR_TXTREP            (CHAR *)"12AB,0,1,\"Message to Melissa\""

#define HFAG_NOCARRIER_STR (CHAR *)"\r\nNO CARRIER\r\n"
#define HFAG_BUSY_STR      (CHAR *)"\r\nBUSY\r\n"
#define HFAG_NOANSWER_STR  (CHAR *)"\r\nNO ANSWER\r\n"
#define HFAG_RING_STR      (CHAR *)"\r\nRING\r\n"
#define HFAG_CIND_TEST_STR                                                                            \
    (CHAR *)"\r\n+CIND: (\"service\",(0-1)), (\"call\",(0-1)), (\"callsetup\",(0-3)), (\"callheld\",(0-2)), " \
    "(\"signal\",(0-5)), (\"roam\",(0-1)), (\"battchg\",(0-5))\r\n"
#define HFAG_CHLD_TEST_STR (CHAR *)"\r\n+CHLD: (0,1,2,3,4)\r\n"

#define HFAG_COPS_READ_STR (CHAR *)"\r\n+COPS: 0,0\r\n"
#define HFAG_CNUM_STR      (CHAR *)"\r\n+CNUM: ,\"918067064000\",143,,4\r\n"

/* Variable Response Strings */
#define HFAG_CLIP_STR      (CHAR *)"\r\n+CLIP: "
#define HFAG_CCWA_STR      (CHAR *)"\r\n+CCWA: "
#define HFAG_CIND_READ_STR (CHAR *)"\r\n+CIND: "
#define HFAG_SERVICE_STR   (CHAR *)"\r\n+CIEV: 1,"
#define HFAG_CALL_STR      (CHAR *)"\r\n+CIEV: 2,"
#define HFAG_CALLSETUP_STR (CHAR *)"\r\n+CIEV: 3,"
#define HFAG_CALLHELD_STR  (CHAR *)"\r\n+CIEV: 4,"
#define HFAG_SIGNAL_STR    (CHAR *)"\r\n+CIEV: 5,"
#define HFAG_ROAMING_STR   (CHAR *)"\r\n+CIEV: 6,"
#define HFAG_BATTERY_STR   (CHAR *)"\r\n+CIEV: 7,"
#define HFAG_CME_ERROR_STR (CHAR *)"\r\n+CME ERROR: "
#define HFAG_CLCC_STR      (CHAR *)"\r\n+CLCC: "

#define HFAG_BTRH_STR (CHAR *)"\r\n+BTRH: "

typedef enum _hfp_ag_index_t
{
    HFAG_OK = 0,
    HFAG_ERROR,
    HFAG_BRSF,
    HFAG_BCS,
    HFAG_BINP,
    HFAG_VGS,
    HFAG_VGM,
    HFAG_BSIR,
    HFAG_BIND_TEST,
    HFAG_BIND_READ,
    HFAG_BVRA,
    HFAG_NOCARRIER,
    HFAG_BUSY,
    HFAG_NOANSWER,
    HFAG_RING,
    HFAG_CIND_TEST,
    HFAG_CHLD_TEST,
    HFAG_COPS_READ,
    HFAG_CLIP,
    HFAG_CCWA,
    HFAG_CIND_READ,
    HFAG_CALL,
    HFAG_CALLSETUP,
    HFAG_SERVICE,
    HFAG_SIGNAL,
    HFAG_ROAMING,
    HFAG_BATTERY,
    HFAG_CALLHELD,
    HFAG_CNUM,
    HFAG_CME_ERROR,
    HFAG_CLCC,
    HFAG_BTRH,
} hfp_ag_index_t;

/* Response lookup table */
static CHAR *rsp_table[] = {
    HFAG_OK_STR,     HFAG_ERROR_STR,    HFAG_BRSF_STR,      HFAG_BCS_STR,       HFAG_BINP_STR,      HFAG_VGS_STR,
    HFAG_VGM_STR,    HFAG_BSIR_STR,     HFAG_BIND_TEST_STR, HFAG_BIND_READ_STR, HFAG_BVRA_STR,      HFAG_NOCARRIER_STR,
    HFAG_BUSY_STR,   HFAG_NOANSWER_STR, HFAG_RING_STR,      HFAG_CIND_TEST_STR, HFAG_CHLD_TEST_STR, HFAG_COPS_READ_STR,
    HFAG_CLIP_STR,   HFAG_CCWA_STR,     HFAG_CIND_READ_STR, HFAG_CALL_STR,      HFAG_CALLSETUP_STR, HFAG_SERVICE_STR,
    HFAG_SIGNAL_STR, HFAG_ROAMING_STR,  HFAG_BATTERY_STR,   HFAG_CALLHELD_STR,  HFAG_CNUM_STR,      HFAG_CME_ERROR_STR,
    HFAG_CLCC_STR,   HFAG_BTRH_STR};

static void bt_hfp_ag_send_rsp(uint8_t *rsp, uint16_t rsplen);
/*
 *  Default Voice Settings to be used for Wideband Speech Synchronous Connection:
 *  Input Coding: A-Law
 *  Input Format: 2's Compliment
 *  Input Sample Size: 16-bit
 *  Linear PCM Bit Position: 3
 *  Air Coding Format: TRANSPARENT
 */
#define APPL_WIDEBAND_VOICE_SETTING_DEFAULT                                                                 \
    (LMP_VOICE_INPUT_CODING_LINEAR | LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT | LMP_VOICE_INPUT_SAMPLE_16_BIT | \
     LMP_VOICE_LINEAR_BIT_POS_0 | LMP_VOICE_AIR_CODING_TRANSPARENT)

/* eSCO-CVSD S1-S4 settings */
static HCI_SCO_IN_PARAMS bt_hfp_ag_esco_cvsd_params[] = {
    /* Default */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, LMP_MAX_LATENCY_DONT_CARE, LMP_VOICE_SETTING_DEFAULT,
     LMP_ESCO_PACKET_TYPE_DEFAULT, LMP_RETX_EFFORT_DONT_CARE},

    /* S1 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x0007, LMP_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_EV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_POWER},

    /* S2 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x0007, LMP_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_POWER},

    /* S3 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x000A, LMP_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_POWER},

    /* S4 */
    {LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x000C, LMP_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_QUALITY}};

/* eSCO-mSBC T1-T2 settings */
static HCI_SCO_IN_PARAMS bt_hfp_ag_esco_msbc_params[] = {
    /* Default */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, LMP_MAX_LATENCY_DONT_CARE,
     APPL_WIDEBAND_VOICE_SETTING_DEFAULT, LMP_ESCO_PACKET_TYPE_DEFAULT, LMP_RETX_EFFORT_DONT_CARE},

    /* T1 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x0008, APPL_WIDEBAND_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_EV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_QUALITY},

    /* T2 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x000D, APPL_WIDEBAND_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_QUALITY}};

/* HFP eSCO Channel Default parameters */
static HCI_SCO_IN_PARAMS *bt_hfp_ag_esco_params[] = {
    /* Default Narrowband (CVSD) parameter */
    &bt_hfp_ag_esco_cvsd_params[0],

    /* Default Wideband (mSBC) parameter */
    &bt_hfp_ag_esco_msbc_params[0]};

typedef struct _bt_hfp_ag_sendelt
{
    uint8_t *buf;
    uint16_t len;

} bt_hfp_ag_sendelt;

#define BT_HFP_AG_SEND_QUEUE_SIZE 8

static bt_hfp_ag_sendelt bt_hfp_ag_send[BT_HFP_AG_SEND_QUEUE_SIZE];
static uint8_t bt_hfp_ag_sendwr;
static uint8_t bt_hfp_ag_sendrd;
static struct bt_hfp_ag *hfp_ag_GetInstance(void);
static void hfp_ag_FreeInstance(struct bt_hfp_ag *hfp_ag);

static void bt_hfp_ag_dump_bytes(uint8_t *buffer, uint16_t length)
{
#ifndef APPL_LIMIT_LOGS
    char hex_stream[49];
    char char_stream[17];
    UINT32 i;
    uint16_t offset, count;
    uint8_t c;

    printf("\n");
    printf("-- Dumping %d Bytes --\n", (int)length);

    printf("-------------------------------------------------------------------\n");

    count  = 0;
    offset = 0;
    for (i = 0; i < length; i++)
    {
        c = buffer[i];
        sprintf(hex_stream + offset, "%02X ", c);

        if ((c >= 0x20) && (c <= 0x7E))
        {
            char_stream[count] = c;
        }
        else
        {
            char_stream[count] = '.';
        }

        count++;
        offset += 3;

        if (16 == count)
        {
            char_stream[count] = '\0';
            count              = 0;
            offset             = 0;

            printf("%s   %s\n", hex_stream, char_stream);

            BT_mem_set(hex_stream, 0, 49);
            BT_mem_set(char_stream, 0, 17);
        }
    }

    if (offset != 0)
    {
        char_stream[count] = '\0';

        /* Maintain the alignment */
        printf("%-48s   %s\n", hex_stream, char_stream);
    }

    printf("-------------------------------------------------------------------\n");

    printf("\n");
#endif /* APPL_LIMIT_LOGS */

    return;
}
static API_RESULT bt_hfp_ag_set_esco_channel_parameters(uint8_t set_sco_param, HCI_SCO_IN_PARAMS *sco_params)
{
    uint8_t enable;
    HCI_SCO_IN_PARAMS esco_params;

    memset(&esco_params, 0x0, sizeof(HCI_SCO_IN_PARAMS));
    /* Null Check for eSCO parameters */
    if ((BT_TRUE == set_sco_param) && (NULL == sco_params))
    {
        LOG_ERR("Unable to set NULL as eSCO parameters\n");
        return API_FAILURE;
    }

    if (BT_TRUE == set_sco_param)
    {
        esco_params = *(sco_params);
    }

    /* Update the audio config */
    enable = (LMP_VOICE_AIR_CODING_TRANSPARENT == (esco_params.voice_setting & LMP_VOICE_AIR_CODING_TRANSPARENT)) ?
                 BT_TRUE :
                 BT_FALSE;
    sco_audio_set_wideband_pl(enable);

    return API_SUCCESS;
}
/* Function to handle API return values from HFP AG */
static void bt_hfp_ag_handle_retval_from_hfag(uint16_t retval)
{
    switch (retval)
    {
        case API_SUCCESS:
            LOG_DBG("BT_HFP_AG API_SUCCESS\n");
            break;
        case HFP_AG_ERR_NULL_PARAMETER:
            LOG_ERR("BT_HFP_AG HFP_AG_ERR_NULL_PARAMETER\n");
            break;
        case HFP_AG_MUTEX_INIT_FAILED:
            LOG_ERR("BT_HFP_AG HFP_AG_MUTEX_INIT_FAILED \n");
            break;
        case HFP_AG_ERR_INVALID_PARAMETER:
            LOG_ERR("BT_HFP_AG HFP_AG_ERR_INVALID_PARAMETER\n");
            break;
        case HFP_AG_ERR_INVALID_STATE:
            LOG_ERR("BT_HFP_AG HFP_AG_ERR_INVALID_STATE\n");
            break;
        case HFP_AG_ERR_STOP_PENDING:
            LOG_ERR("BT_HFP_AG HFP_AG_ERR_STOP_PENDING\n");
            break;
        case HFP_AG_MUTEX_LOCK_FAILED:
            LOG_ERR("BT_HFP_AG HFP_AG_MUTEX_LOCK_FAILED\n");
            break;
        case HFP_AG_MUTEX_UNLOCK_FAILED:
            LOG_ERR("BT_HFP_AG HFP_AG_MUTEX_UNLOCK_FAILED\n");
            break;

        default:
            LOG_DBG("BT_HFP_AG Result: 0x%04X\n", retval);
    } /* End Switch */

    return;
}

static API_RESULT hfp_ag_callback(HFP_AG_EVENTS hfp_ag_event, API_RESULT result, void *data, uint16_t data_length)
{
    uint8_t recvd_bd_addr[BT_BD_ADDR_SIZE];
    uint16_t i;
    uint8_t *recvd_data;
    uint8_t option, index;
    struct bt_hfp_ag *hfp_ag;
    uint32_t codecs = 0;

    switch (hfp_ag_event)
    {
        case HFP_AG_CONNECT_IND:
        {
            struct bt_conn *conn;

            LOG_DBG("BT_HFP_AG HFP_AG_CONNECT_IND \n");
            bt_hfp_agag_state = BT_HFP_AG_STATE_CONNECTED;

            hfp_ag = hfp_ag_GetInstance();
            if (NULL == hfp_ag)
            {
                break;
            }

            s_actived_bt_hfp_ag = hfp_ag;
            memset(s_actived_bt_hfp_ag->bt_hfp_ag_bind, 0x0, 2U);
            s_actived_bt_hfp_ag->bt_cind_setting.server           = 1U;
            s_actived_bt_hfp_ag->bt_cind_setting.call_state       = 0U;
            s_actived_bt_hfp_ag->bt_cind_setting.call_setup_state = HFP_AG_CALL_SETUP_STATUS_IDLE;
            s_actived_bt_hfp_ag->bt_cind_setting.call_held_state  = 0U;
            s_actived_bt_hfp_ag->bt_cind_setting.signal           = 4U;
            s_actived_bt_hfp_ag->bt_cind_setting.roam             = 0U;
            s_actived_bt_hfp_ag->bt_cind_setting.batt_lev         = 2U;

            hfp_ag->ag_features = BT_HFP_AG_SUPPORTED_FEATURES;

            if (data_length >= BT_BD_ADDR_SIZE)
            {
                BT_mem_copy(hfp_ag->peerAddr, data, BT_BD_ADDR_SIZE);
                conn = bt_conn_lookup_addr_br((const bt_addr_t *)hfp_ag->peerAddr);
                if (!conn)
                {
                    break;
                }
                else
                {
                    bt_conn_unref(conn);
                }

                hfp_ag->acl_so_conn = conn;
            }

            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->connected))
            {
                bt_hfp_ag_cb->connected(s_actived_bt_hfp_ag);
            }

            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->get_config))
            {
                bt_hfp_ag_cb->get_config(s_actived_bt_hfp_ag, &s_actived_bt_hfp_ag->bt_hfp_ag_config);
            }

            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband = BT_TRUE;
                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_dial   = BT_TRUE;
            }

            bt_hfp_ag_set_esco_channel_parameters(BT_TRUE, bt_hfp_ag_esco_params[0]);
            break;
        }

        case HFP_AG_CONNECT_CNF:
            LOG_DBG("BT_HFP_AG HFP_AG_CONNECT_CNF \n");
            if (API_SUCCESS == result)
            {
                bt_hfp_agag_state = BT_HFP_AG_STATE_CONNECTED;

                if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->connected))
                {
                    bt_hfp_ag_cb->connected(s_actived_bt_hfp_ag);
                }

                if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->get_config))
                {
                    bt_hfp_ag_cb->get_config(s_actived_bt_hfp_ag, &s_actived_bt_hfp_ag->bt_hfp_ag_config);
                }
                if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                {
                    s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband = BT_TRUE;
                    s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_dial   = BT_TRUE;
                }
            }
            bt_hfp_ag_handle_retval_from_hfag(result);
            bt_hfp_ag_set_esco_channel_parameters(BT_TRUE, bt_hfp_ag_esco_params[0]);
            break;
        case HFP_AG_DISCONNECT_IND:
            BT_mem_copy(recvd_bd_addr, (uint8_t *)data, data_length);
            LOG_DBG("BT_HFP_AG HFP_AG_DISCONNECT_IND \n");

            LOG_DBG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER, BT_DEVICE_ADDR_ONLY_PRINT_STR(recvd_bd_addr));

            bt_hfp_agag_state = BT_HFP_AG_STATE_DISCONNECTED;

            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->disconnected))
            {
                bt_hfp_ag_cb->disconnected(s_actived_bt_hfp_ag);
            }
            hfp_ag_FreeInstance(s_actived_bt_hfp_ag);
            break;

        case HFP_AG_DISCONNECT_CNF:
            BT_mem_copy(recvd_bd_addr, (uint8_t *)data, data_length);
            LOG_DBG("BT_HFP_AG HFP_AG_DISCONNECT_CNF \n");

            LOG_DBG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER, BT_DEVICE_ADDR_ONLY_PRINT_STR(recvd_bd_addr));

            bt_hfp_agag_state = BT_HFP_AG_STATE_DISCONNECTED;

            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->disconnected))
            {
                bt_hfp_ag_cb->disconnected(s_actived_bt_hfp_ag);
            }
            hfp_ag_FreeInstance(s_actived_bt_hfp_ag);
            break;

        case HFP_AG_STOP_CNF:
            LOG_DBG("BT_HFP_AG HFP_AG_STOP_CNF \n");
            break;

        case HFP_AG_SEND_DATA_CNF:
            if (API_SUCCESS != result)
            {
                LOG_DBG("BT_HFP_AG HFP_AG_SEND_DATA_CNF \n");
                bt_hfp_ag_handle_retval_from_hfag(result);
            }

            if (bt_hfp_ag_send[bt_hfp_ag_sendrd].buf == data)
            {
                API_RESULT retval;

                BT_free_mem(bt_hfp_ag_send[bt_hfp_ag_sendrd].buf);
                bt_hfp_ag_send[bt_hfp_ag_sendrd].buf = NULL;

                if (API_SUCCESS != result)
                {
                    break;
                }

                bt_hfp_ag_sendrd++;
                bt_hfp_ag_sendrd &= (BT_HFP_AG_SEND_QUEUE_SIZE - 1);

                if ((bt_hfp_ag_sendrd != bt_hfp_ag_sendwr) && (NULL != bt_hfp_ag_send[bt_hfp_ag_sendrd].buf))
                {
                    retval =
                        BT_hfp_ag_send_data(bt_hfp_ag_send[bt_hfp_ag_sendrd].buf, bt_hfp_ag_send[bt_hfp_ag_sendrd].len);

                    if (API_SUCCESS != retval)
                    {
                        LOG_ERR("BT_hfp_ag_send_data Failed - 0x%04X\n", retval);
                        BT_free_mem(bt_hfp_ag_send[bt_hfp_ag_sendrd].buf);
                    }
                }
            }
            else
            {
                LOG_DBG("*** Spurious Send Data Cnf ***\n");
            }
            break;

        case HFP_AG_RECVD_DATA_IND:
            LOG_DBG("BT_HFP_AG HFP_AG_RECVD_ATCMD \n");
            LOG_DBG("BT_HFP_AG CMD LEN : %d \n", (uint16_t)data_length);

            recvd_data = (uint8_t *)data;
            for (i = 0; i < data_length; i++)
            {
                LOG_DBG("%c", recvd_data[i]);
            }
            LOG_DBG("\n");

#ifndef HFP_AG_NO_AUTOMATED_SLC_RESPONSE
            {
                AT_PARSER_RESPONSE at_response;
                uint16_t start_index, buffer_size, length, retval;

                /* Call the AT parser to parse the AT command sent from unit */
                length      = data_length;
                buffer_size = data_length;
                start_index = 0;

                bt_hfp_ag_dump_bytes(recvd_data, buffer_size);

                retval = BT_ag_at_parser(recvd_data, start_index, buffer_size, &length, &at_response);

                /* Handle the AT commands */
                if (API_SUCCESS == retval)
                {
                    switch (at_response.keyword_type)
                    {
                        case ATA:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->ata_response))
                            {
                                bt_hfp_ag_cb->ata_response(s_actived_bt_hfp_ag);
                            }
                            break;
                        case AT_CHUP:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->chup_response))
                            {
                                bt_hfp_ag_cb->chup_response(s_actived_bt_hfp_ag);
                            }
                            break;

                        case AT_BRSF:
                            s_actived_bt_hfp_ag->hf_features =
                                atoi((char const *)&at_response.global_at_str[at_response.param->start_of_value_index]);
                            LOG_DBG("Get peer brsf: %x\n", s_actived_bt_hfp_ag->hf_features);
                            bt_hfp_ag_send_at_rsp(HFAG_BRSF, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);

                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->hfu_brsf))
                            {
                                bt_hfp_ag_cb->hfu_brsf(s_actived_bt_hfp_ag,
                                                                            s_actived_bt_hfp_ag->hf_features);
                            }
                            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                            {
                                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec_negotiate = 0;
                            }
                            break;

                        case AT_CMER:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        case AT_CIND_TEST:
                            bt_hfp_ag_send_at_rsp(HFAG_CIND_TEST, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        case AT_CIND_READ:
                            bt_hfp_ag_send_at_rsp(HFAG_CIND_READ, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        case AT_CHLD_TEST:
                            bt_hfp_ag_send_at_rsp(HFAG_CHLD_TEST, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        case AT_NREC:
                            if ((s_actived_bt_hfp_ag->bt_hfp_ag_config) && (!s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_nrec))
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                            }
                            else
                            {
                                switch (at_response.global_at_str[at_response.param->start_of_value_index])
                                {
                                    case '1':
                                        LOG_DBG("NREC Enabled\n");
                                        bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                                        break;

                                    case '0':
                                        LOG_DBG("NREC Disbled\n");
                                        if (s_actived_bt_hfp_ag->ag_features | BT_HFP_AG_FEATURE_ECNR )
                                        {
                                            bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                                        }
                                        else
                                        {
                                             bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                                        }
                                        break;

                                    default:
                                        bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                                        break;
                                }
                            }
                            break;

                        case AT_VTS:
                            LOG_DBG("DTMF codes received \n");
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->recv_dtmf_codes))
                            {
                                bt_hfp_ag_cb->recv_dtmf_codes(
                                    s_actived_bt_hfp_ag,
                                    at_response.global_at_str[at_response.param->start_of_value_index]);
                            }
                            break;

                        case AT_BVRA:
                            switch (at_response.global_at_str[at_response.param->start_of_value_index])
                            {
                                case '1':
                                    LOG_DBG("VR Enabled\n");
                                    bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);

                                     if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->brva))
                                      {
                                          bt_hfp_ag_cb->brva(
                                              s_actived_bt_hfp_ag,
                                              1);
                                      }
                                    break;

                                case '0':
                                    LOG_DBG("VR Disbled\n");
                                    bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                                     if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->brva))
                                     {
                                          bt_hfp_ag_cb->brva(
                                              s_actived_bt_hfp_ag,
                                              0);
                                      }
                                    break;

                                case '2':
                                    LOG_DBG("EVR Enabled\n");
                                    bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                                    break;

                                default:
                                    bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                                    break;
                            }
                            break;

                        case AT_VGS:
                        {
                            uint8_t bt_hfp_ag_vgs;

                            bt_hfp_ag_vgs = (uint8_t)atoi(
                                    (char const *)&at_response.global_at_str[at_response.param->start_of_value_index]);
                            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                            {
                                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgs = bt_hfp_ag_vgs;
                            }
                            LOG_DBG("Speaker Gain Updated - %d\n", bt_hfp_ag_vgs);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->volume_control))
                            {
                                bt_hfp_ag_cb->volume_control(s_actived_bt_hfp_ag, hf_ag_volume_type_speaker,
                                                             bt_hfp_ag_vgs);
                            }
                            break;
                        }

                        case AT_VGM:
                        {
                            uint8_t bt_hfp_ag_vgm = (uint8_t)atoi(
                                    (char const *)&at_response.global_at_str[at_response.param->start_of_value_index]);

                            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                            {
                                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgm = bt_hfp_ag_vgm;
                            }
                            LOG_DBG("Microphone Gain Updated - %d\n", bt_hfp_ag_vgm);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->volume_control))
                            {
                                bt_hfp_ag_cb->volume_control(s_actived_bt_hfp_ag, hf_ag_volume_type_mic, bt_hfp_ag_vgm);
                            }
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        }

                        case AT_BINP:
                            bt_hfp_ag_send_at_rsp(HFAG_BINP, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;

                        case AT_BCC:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->codec_connect_req))
                            {
                                bt_hfp_ag_cb->codec_connect_req(s_actived_bt_hfp_ag);
                            }
                            break;

                        case AT_CLCC:
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->clcc))
                            {
                                bt_hfp_ag_cb->clcc(s_actived_bt_hfp_ag);
                            }
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;
                        case AT_BCS:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->codec_negotiate))
                            {
                                bt_hfp_ag_cb->codec_negotiate(
                                    s_actived_bt_hfp_ag,
                                    at_response.global_at_str[at_response.param->start_of_value_index] - '0' );
                            }

                            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                            {
                                s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec_negotiate = 0;
                                /* Trigger codec connection */
                                if (at_response.global_at_str[at_response.param->start_of_value_index] == s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec)
                                {
                                    bt_hfp_ag_open_audio(
                                        s_actived_bt_hfp_ag,
                                        (at_response.global_at_str[at_response.param->start_of_value_index] - '0') - 1);
                                }
                            }
                            break;

                        case AT_BAC:
                            for (i = 0; i < at_response.number_of_params; i++)
                            {
                                codecs |= at_response.global_at_str[at_response.param[i].start_of_value_index] - '0';
                            }

                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->codec))
                            {
                                bt_hfp_ag_cb->codec(s_actived_bt_hfp_ag, codecs);
                            }
                            break;

                        case AT_BIND_TEST:
                            bt_hfp_ag_send_at_rsp(HFAG_BIND_TEST, NULL);
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;

                        case AT_BIND_READ:
                            for (i = 0; i < sizeof(s_actived_bt_hfp_ag->bt_hfp_ag_bind); i++)
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_BIND_READ, &i);
                            }
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;

                        case AT_BIND:
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            break;

                        case AT_BIEV:
                            if (2 < (at_response.global_at_str[at_response.param->start_of_value_index] - '0'))
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                            }
                            else
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            }
                            break;

                        case ATD:
                            /* Hold the phone number */
                            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
                            {
                                BT_mem_set(s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum, 0x0, sizeof(s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum));
                                BT_str_copy(s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum,
                                            &at_response.global_at_str[at_response.param->start_of_value_index]);
                            }

                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->dial))
                            {
                                bt_hfp_ag_cb->dial(
                                    s_actived_bt_hfp_ag,
                                    (char*)&at_response.global_at_str[at_response.param->start_of_value_index]);
                            }

                            break;

                        case ATDM:
                            if ((!s_actived_bt_hfp_ag->bt_hfp_ag_config) || (!s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_dial))
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                                break;
                            }

                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->memory_dial))
                            {
                                bt_hfp_ag_cb->memory_dial(
                                    s_actived_bt_hfp_ag, at_response.global_at_str[at_response.param->start_of_value_index] - '0');
                            }
                            break;
                        case AT_BLDN:
                            if ((!s_actived_bt_hfp_ag->bt_hfp_ag_config) || (!s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_dial))
                            {
                                bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                                break;
                            }
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->last_dial))
                            {
                                bt_hfp_ag_cb->last_dial(
                                    s_actived_bt_hfp_ag);
                            }
                            break;
                        case AT_BTRH_READ:
                            /* todo , will impletment on full feature release*/
                            bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                            break;

                         case AT_CHLD:
                            option = at_response.global_at_str[at_response.param->start_of_value_index] - '0';
                            index = at_response.global_at_str[at_response.param->start_of_value_index + 1U];
                            bt_hfp_ag_send_at_rsp(HFAG_OK, NULL);
                            index = (index == '\r') ? 0U : index - '0';

                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->chld))
                            {
                                bt_hfp_ag_cb->chld(
                                    s_actived_bt_hfp_ag,
                                    option, index);
                            }
                            break;

                        default:
                            if ((bt_hfp_ag_cb) && (bt_hfp_ag_cb->unkown_at))
                            {
                                bt_hfp_ag_cb->unkown_at(s_actived_bt_hfp_ag, (char *)recvd_data,
                                                                             buffer_size);
                            }
                            else
                                bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                            break;
                    }
                }
                else
                {
                    bt_hfp_ag_send_at_rsp(HFAG_ERROR, NULL);
                }
            }
#endif /* !HFP_AG_NO_AUTOMATED_SLC_RESPONSE */
    }

    return API_SUCCESS;
}

static void hfp_ag_sco_connected(struct bt_sco_chan *chan)
{
    if ((bt_hfp_ag_cb != NULL) && (bt_hfp_ag_cb->sco_connected))
    {
        bt_hfp_ag_cb->sco_connected(s_actived_bt_hfp_ag, chan->sco);
    }
}

static void hfp_ag_sco_disconnected(struct bt_sco_chan *chan, uint8_t reason)
{
    if ((bt_hfp_ag_cb != NULL) && (bt_hfp_ag_cb->sco_disconnected))
    {
        bt_hfp_ag_cb->sco_disconnected(s_actived_bt_hfp_ag);
    }
}

static struct bt_sco_chan_ops ag_sco_chan_ops = {
    .connected    = hfp_ag_sco_connected,
    .disconnected = hfp_ag_sco_disconnected,
};

static int bt_hfp_ag_sco_accept(const struct bt_sco_accept_info *info, struct bt_sco_chan **chan)
{
    LOG_DBG("conn %p", info->acl);

    if (s_actived_bt_hfp_ag)
    {
        s_actived_bt_hfp_ag->sco_chan.ops = &ag_sco_chan_ops;
        *chan                             = &s_actived_bt_hfp_ag->sco_chan;
        return 0;
    }

    LOG_ERR("Unable to establish HF connection (%p)", info->acl);

    return -ENOMEM;
}

static int hfp_ag_start_pre(void)
{
    API_RESULT api_retval;
    UINT8 hfp_ag_server_channel;
#ifdef SDP_DYNAMIC_DB
    hfp_ag_server_channel = BT_RFCOMM_CHAN_HFP_AG;
#else
    UINT32 hfp_ag_record_handle;


    /* local varibale to extract the supported features */
    uint8_t attr_value[] = {0x09, 0x00, 0x00};

    LOG_DBG("Selected the Local HFP_AG Supported Features(as Decimal value)\n: %x", BT_HFP_AG_SUPPORTED_FEATURES);

    sprintf((char *)hfp_ag_local_supported_features, "%u", BT_HFP_AG_SUPPORTED_FEATURES);

    hfp_ag_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN - 1] = '\0';

    /* Local Supported Features in Numeric Format */
    hfp_ag_local_supported_features_ext = BT_HFP_AG_SUPPORTED_FEATURES;
    (void)hfp_ag_local_supported_features_ext;

    attr_value[1] = hfp_ag_local_supported_features[0];
    attr_value[2] = hfp_ag_local_supported_features[1];

    /* Get HFP_UNIT SDP record handle */
    BT_dbase_get_record_handle(DB_RECORD_HFAG, 0, &hfp_ag_record_handle);

    api_retval = BT_dbase_update_attr_value(hfp_ag_record_handle, 0x0311, attr_value, 0x03);
    bt_hfp_ag_handle_retval_from_hfag(api_retval);

    api_retval = BT_dbase_get_server_channel
                     (
                         hfp_ag_record_handle,
                         PROTOCOL_DESC_LIST,
                         &hfp_ag_server_channel
                     );
    if(API_SUCCESS != api_retval)
    {
        return -EIO;
    }
#endif
    api_retval = BT_hfp_ag_start(hfp_ag_server_channel);
    if(API_SUCCESS != api_retval)
    {
        return -EIO;
    }
#ifndef SDP_DYNAMIC_DB
    (void)BT_dbase_activate_record(hfp_ag_record_handle);
#endif
    return 0;
}

static struct bt_hfp_ag *hfp_ag_GetInstance(void)
{
    EDGEFAST_HFP_AG_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAX_CONN; ++index)
    {
        if (s_HfpAgInstances[index].allocated == 0U)
        {
            memset(&s_HfpAgInstances[index], 0U, sizeof(s_HfpAgInstances[index]));
            s_HfpAgInstances[index].allocated = 1U;
            EDGEFAST_HFP_AG_UNLOCK;
            return &s_HfpAgInstances[index];
        }
    }

    EDGEFAST_HFP_AG_UNLOCK;
    return NULL;
}

static void hfp_ag_FreeInstance(struct bt_hfp_ag *hfp_ag)
{
    if (NULL != hfp_ag)
    {
        EDGEFAST_HFP_AG_LOCK;
        hfp_ag->allocated = 0U;
        EDGEFAST_HFP_AG_UNLOCK;
    }
}

static void bt_hfp_ag_send_rsp(uint8_t *rsp, uint16_t rsplen)
{
    API_RESULT retval;
    uint8_t *buf;

    if (BT_HFP_AG_STATE_CONNECTED != bt_hfp_agag_state)
    {
        return;
    }

    buf = (uint8_t *)BT_alloc_mem(rsplen);
    if (NULL == buf)
    {
        LOG_ERR("Failed to allocate buffer\n");
        return;
    }

    BT_mem_copy(buf, rsp, rsplen);

    bt_hfp_ag_send[bt_hfp_ag_sendwr].buf = buf;
    bt_hfp_ag_send[bt_hfp_ag_sendwr].len = rsplen;

    retval = API_SUCCESS;

    if (bt_hfp_ag_sendwr == bt_hfp_ag_sendrd)
    {
        retval = BT_hfp_ag_send_data(buf, rsplen);

        if (API_SUCCESS != retval)
        {
            LOG_ERR("BT_hfp_ag_send_data Failed - 0x%04X\n", retval);
            BT_free_mem(buf);
        }
    }

    if (API_SUCCESS == retval)
    {
        bt_hfp_ag_sendwr++;
        bt_hfp_ag_sendwr &= (BT_HFP_AG_SEND_QUEUE_SIZE - 1);
    }
}
static void bt_hfp_ag_send_at_rsp(uint8_t rsp_code, void *value)
{
    CHAR response[256];
    uint8_t length;

    BT_mem_set(response, 0, sizeof(response));

    BT_str_copy(response, rsp_table[rsp_code]);
    length = (uint8_t)BT_str_len(rsp_table[rsp_code]);

    switch (rsp_code)
    {
        case HFAG_OK:
        case HFAG_ERROR:
        case HFAG_NOCARRIER:
        case HFAG_BUSY:
        case HFAG_NOANSWER:
        case HFAG_RING:
        case HFAG_CIND_TEST:
        case HFAG_CHLD_TEST:
        case HFAG_CNUM:
            break;

        case HFAG_COPS_READ:
            sprintf((response + length), "%s\r\n", s_actived_bt_hfp_ag->ag_str_cops);
            break;

        case HFAG_CIND_READ:
            sprintf((response + length), "%d,%d,%d,%d,%d,%d,%d\r\n", s_actived_bt_hfp_ag->bt_cind_setting.server,
                    s_actived_bt_hfp_ag->bt_cind_setting.call_state,
                    s_actived_bt_hfp_ag->bt_cind_setting.call_setup_state,
                    s_actived_bt_hfp_ag->bt_cind_setting.call_held_state, s_actived_bt_hfp_ag->bt_cind_setting.signal,
                    s_actived_bt_hfp_ag->bt_cind_setting.roam, s_actived_bt_hfp_ag->bt_cind_setting.batt_lev);
            break;

        case HFAG_BRSF:
            sprintf((response + length), "%d\r\n", (int)s_actived_bt_hfp_ag->ag_features);
            break;

        case HFAG_BCS:
            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                sprintf((response + length), "%d\r\n", s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec);
            }
            break;

        case HFAG_BINP:
            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                sprintf((response + length), "%s\r\n", s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum_tag);
            }
            break;

        case HFAG_VGS:
            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                sprintf((response + length), "%d\r\n", s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgs);
            }
            break;

        case HFAG_VGM:
            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                sprintf((response + length), "%d\r\n", s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgm);
            }
            break;

        case HFAG_BSIR:
            if (s_actived_bt_hfp_ag->bt_hfp_ag_config)
            {
                sprintf((response + length), "%d\r\n", s_actived_bt_hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband);
            }
            break;

        case HFAG_BIND_TEST:
            break;

        case HFAG_CLIP:
        case HFAG_CCWA:
            sprintf((response + length), "\"%s\",129\r\n", (CHAR *)value);
            break;
        case HFAG_CLCC:
            sprintf((response + length), "%s\r\n", (CHAR *)value);
            break;

        case HFAG_BTRH:
        case HFAG_CALL:
        case HFAG_CALLSETUP:
        case HFAG_SERVICE:
        case HFAG_SIGNAL:
        case HFAG_ROAMING:
        case HFAG_CALLHELD:
        case HFAG_BATTERY:
            sprintf((response + length), "%d\r\n", *((uint8_t *)value));
            break;
        case HFAG_BIND_READ:
            sprintf((response + length), "%d, %d\r\n", (*(uint8_t *)value + 1),
                    s_actived_bt_hfp_ag->bt_hfp_ag_bind[*(uint8_t *)value]);
            break;

        default:
            /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
            break;
    }

    /* Update length and send response to interface */
    length = (uint8_t)BT_str_len(response);
    bt_hfp_ag_send_rsp((uint8_t *)response, length);
}

static void bt_hfp_ag_callend_timeout_handler(void *args, uint16_t size)
{
    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(size);

    bt_hfp_ag_close_audio(s_actived_bt_hfp_ag);
}

int bt_hfp_ag_init(void)
{
    static int ag_init = 0;

    if (ag_init == 0)
    {
        int err;

        BT_hfp_ag_init(hfp_ag_callback);
        ag_init = 1U;
        err = hfp_ag_start_pre();
        if (err) {
            return err;
        }
    }
    if (NULL == s_HfpAgLock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_HfpAgLockMutex))
        {
            s_HfpAgLock = (osa_mutex_handle_t)s_HfpAgLockMutex;
        }
        else
        {
            return -EIO;
        }
    }
    for (int i = 0; i < CONFIG_BT_MAX_CONN; i++)
    {
        s_hfp_sdp[i].allocated = 0;
    }

    static struct bt_sco_server sco_server = {
        .sec_level = BT_SECURITY_L0,
        .accept    = bt_hfp_ag_sco_accept,
    };

    (void)bt_sco_server_register(&sco_server);

    return 0;
}

int bt_hfp_ag_deinit(void)
{
    /* TODO */
    return 0;
}

int bt_hfp_ag_connect(struct bt_conn *conn,
                      hfp_ag_get_config *config,
                      struct bt_hfp_ag_cb *cb,
                      struct bt_hfp_ag **phfp_ag)
{
    struct bt_hfp_ag *hfp_ag;

    hfp_ag = hfp_ag_GetInstance();
    if (NULL == hfp_ag)
    {
        return -EAGAIN;
    }

    if ((!cb) && bt_hfp_ag_cb == NULL)
    {
        return -EINVAL;
    }
    if (cb)
    {
        bt_hfp_ag_cb = cb;
    }

    if (!config)
    {
        return -EINVAL;
    }

    hfp_ag->bt_hfp_ag_config = config;
    hfp_ag->serverChannel = config->server_channel;

    memcpy(hfp_ag->peerAddr, conn->br.dst.val, BT_BD_ADDR_SIZE);
    BT_hfp_ag_connect(config->server_channel, hfp_ag->peerAddr);
    s_actived_bt_hfp_ag = hfp_ag;
    hfp_ag->acl_so_conn = conn;
    memset(s_actived_bt_hfp_ag->bt_hfp_ag_bind, 0x0, 2U);
    s_actived_bt_hfp_ag->bt_cind_setting.server           = 1U;
    s_actived_bt_hfp_ag->bt_cind_setting.call_state       = 0U;
    s_actived_bt_hfp_ag->bt_cind_setting.call_setup_state = HFP_AG_CALL_SETUP_STATUS_IDLE;
    s_actived_bt_hfp_ag->bt_cind_setting.call_held_state  = 0U;
    s_actived_bt_hfp_ag->bt_cind_setting.signal           = 4U;
    s_actived_bt_hfp_ag->bt_cind_setting.roam             = 0U;
    s_actived_bt_hfp_ag->bt_cind_setting.batt_lev         = 2U;

    hfp_ag->ag_features = BT_HFP_AG_SUPPORTED_FEATURES;
    *phfp_ag = hfp_ag;
    return 0;
}

int bt_hfp_ag_register_cb(struct bt_hfp_ag_cb *cb)
{
    bt_hfp_ag_cb = cb;
    return 0;
}

int bt_hfp_ag_disconnect(struct bt_hfp_ag *hfp_ag)
{
    bt_hfp_ag_close_audio(hfp_ag);
    BT_hfp_ag_disconnect(hfp_ag->peerAddr);
    return 0;
}

int bt_hfp_ag_get_cind_setting(struct bt_hfp_ag *hfp_ag, hfp_ag_cind_t *cind_setting)
{
    if ( (!hfp_ag) || (!cind_setting))
    {
        return -EINVAL;
    }
    memcpy(cind_setting, &s_actived_bt_hfp_ag->bt_cind_setting, sizeof(hfp_ag_cind_t));
    return 0;
}
int bt_hfp_ag_set_cind_setting(struct bt_hfp_ag *hfp_ag, hfp_ag_cind_t *cind_setting)
{
    if ( (!hfp_ag) || (!cind_setting))
    {
        return -EINVAL;
    }
    memcpy(&s_actived_bt_hfp_ag->bt_cind_setting, cind_setting, sizeof(hfp_ag_cind_t));

    return 0;
}
int bt_hfp_ag_send_enable_voice_recognition(struct bt_hfp_ag *hfp_ag)
{
    assert(hfp_ag);
    if (!(hfp_ag->hf_features & 0x0008))
    {
        LOG_DBG("HFU does not support VR feature\n");
        return -EIO;
    }

    bt_hfp_ag_send_rsp((uint8_t *)HFP_AG_ENABLE_VOICE_REG, (uint8_t)BT_str_len(HFP_AG_ENABLE_VOICE_REG));

    bt_hfp_ag_open_audio(hfp_ag, hfp_ag->bt_hfp_ag_config ? hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec - 1 : 0);

    return 0;
}

int bt_hfp_ag_send_disable_voice_recognition(struct bt_hfp_ag *hfp_ag)
{
    assert(hfp_ag);
    if (!(hfp_ag->hf_features & 0x0008))
    {
        LOG_DBG("HFU does not support VR feature\n");
        return -EIO;
    }

    bt_hfp_ag_send_rsp((uint8_t *)HFP_AG_DISABLE_VOICE_REG, (uint8_t)BT_str_len(HFP_AG_DISABLE_VOICE_REG));

    bt_hfp_ag_close_audio(hfp_ag);

    return 0;
}

int bt_hfp_ag_send_disable_voice_ecnr(struct bt_hfp_ag *hfp_ag)
{
    /* TODO*/
    return 0;
}
int bt_hfp_ag_send_enable_voice_ecnr(struct bt_hfp_ag *hfp_ag)
{
    /* TODO*/
    return 0;
}
int bt_hfp_ag_unknown_at_response(struct bt_hfp_ag *hfp_ag, uint8_t *unknow_at_rsp, uint16_t unknow_at_rsplen)
{
    assert(hfp_ag);
    bt_hfp_ag_send_rsp(unknow_at_rsp, unknow_at_rsplen);
    return 0;
}

void bt_hfp_ag_open_audio(struct bt_hfp_ag *hfp_ag, uint8_t codec)
{
    struct bt_conn *bt_so_conn;
    /* Update the eSCO channel paramters for Codec */
    bt_hfp_ag_set_esco_channel_parameters(BT_TRUE, bt_hfp_ag_esco_params[codec]);
    LOG_DBG("> bt_hfp_ag_set_esco_channel_parameters \n");
    if (hfp_ag->sco_chan.sco == NULL)
    {
        hfp_ag->sco_chan.ops = &ag_sco_chan_ops;
        bt_so_conn = bt_conn_create_sco((const bt_addr_t *)hfp_ag->peerAddr, &hfp_ag->sco_chan);
        if (NULL == bt_so_conn)
        {
            LOG_ERR("FAILED !! bt_conn_create_sco \n");
        }
        else
        {
            bt_conn_unref(bt_so_conn);
        }
        LOG_DBG(" bt_conn_create_sco  Sucuss\n");
        hfp_ag->sco_chan.sco = bt_so_conn;
    }
}

void bt_hfp_ag_close_audio(struct bt_hfp_ag *hfp_ag)
{
    int retval;
    if ( hfp_ag->sco_chan.sco != NULL)
    {
        struct bt_conn *sco;

        sco                  = hfp_ag->sco_chan.sco;
        hfp_ag->sco_chan.sco = NULL;
        retval = bt_conn_disconnect(sco, 0x13U);
        if (0 == retval)
        {
            LOG_DBG("Disconnected SCO Connection 0x%04X\n", sco);
        }
        else
        {
            LOG_ERR("SCO Connection for HFP-Unit not found\n");
        }
    }
}

int bt_hfp_ag_register_supp_features(struct bt_hfp_ag *hfp_ag, uint32_t supported_features)
{
    assert(hfp_ag);
    if (~BT_HFP_AG_SUPPORTED_FEATURES & supported_features)
    {
        return -EINVAL;
    }

    hfp_ag->ag_features = supported_features;

    bt_hfp_ag_send_at_rsp(HFAG_BRSF, NULL);
    return 0;
}

int bt_hfp_ag_set_phnum_tag(struct bt_hfp_ag *hfp_ag, char *name)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    if ((!name) || (!hfp_ag->bt_hfp_ag_config))
    {
        return -EINVAL;
    }
    BT_str_n_copy(hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum_tag, name, 16);
    hfp_ag->bt_hfp_ag_config->bt_hfp_ag_phnum_tag[15] = '\0';
    bt_hfp_ag_send_at_rsp(HFAG_BINP, NULL);
    return 0;
}
int bt_hfp_ag_set_clcc(struct bt_hfp_ag *hfp_ag, char *call_list)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    if ((!call_list) || (!hfp_ag->bt_hfp_ag_config))
    {
        return -EINVAL;
    }

    bt_hfp_ag_send_at_rsp(HFAG_CLCC, call_list);
    return 0;
}
int bt_hfp_ag_set_cops(struct bt_hfp_ag *hfp_ag, char *name)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    if (!name)
    {
        return -EINVAL;
    }
    BT_str_n_copy(hfp_ag->ag_str_cops, name, 16);
    hfp_ag->ag_str_cops[15] = '\0';
    bt_hfp_ag_send_at_rsp(HFAG_COPS_READ, NULL);
    return 0;
}

uint32_t bt_hfp_ag_get_peer_supp_features(struct bt_hfp_ag *hfp_ag)
{
    assert(hfp_ag);
    if (!hfp_ag)
    {
        return 0;
    }
    return hfp_ag->hf_features;
}

int bt_hfp_ag_register_cind_features(struct bt_hfp_ag *hfp_ag, char *cind)
{
    /* TODO*/
    return 0;
}

int bt_hfp_ag_send_ccwa_indicator(struct bt_hfp_ag *hfp_ag, char *number)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    PRINTF("bt_hfp_ag_send_ccwa_indicator %s\r\n", number);
    if (NULL != number)
    {
          bt_hfp_ag_send_at_rsp(HFAG_CCWA, number);
    }

    return 0;
}
int bt_hfp_ag_set_volume_control(struct bt_hfp_ag *hfp_ag, hf_ag_volume_type_t type, int value)
{
    if ((!hfp_ag) || (!hfp_ag->bt_hfp_ag_config))
    {
        return -EINVAL;
    }
    if (type == hf_ag_volume_type_speaker)
    {
        hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgs = value;
        bt_hfp_ag_send_at_rsp(HFAG_VGS, NULL);
    }
    else if (type == hf_ag_volume_type_mic)
    {
        hfp_ag->bt_hfp_ag_config->bt_hfp_ag_vgm = value;
        bt_hfp_ag_send_at_rsp(HFAG_VGM, NULL);
    }
    return 0;
}

int bt_hfp_ag_set_inband_ring_tone(struct bt_hfp_ag *hfp_ag, int value)
{
    if ((!hfp_ag) || (!hfp_ag->bt_hfp_ag_config))
    {
        return -EINVAL;
    }
    hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband = value;
    bt_hfp_ag_send_at_rsp(HFAG_BSIR, NULL);
    return 0;
}

void bt_hfp_ag_call_status_pl(struct bt_hfp_ag *hfp_ag, hfp_ag_call_status_t status)
{
    BT_timer_handle tmr_handle;

    switch (status)
    {
        case hfp_ag_call_call_end: /* Call End */
            BT_start_timer(&tmr_handle, 1, bt_hfp_ag_callend_timeout_handler, NULL, 0);
            break;

        case hfp_ag_call_call_active: /* Call Active */
            if ((hfp_ag->bt_hfp_ag_config) && (hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec_negotiate))
            {
                bt_hfp_ag_send_at_rsp(HFAG_BCS, NULL);
            }
            else
            {
                bt_hfp_ag_open_audio(s_actived_bt_hfp_ag,
                                     hfp_ag->bt_hfp_ag_config ? hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec - 1 : 0);
            }
            break;

        case hfp_ag_call_call_incoming: /* Call Incoming */
            if ((hfp_ag->bt_hfp_ag_config) && (hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband))
            {
                if ((hfp_ag->bt_hfp_ag_config) && (hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec_negotiate))
                {
                    bt_hfp_ag_send_at_rsp(HFAG_BCS, NULL);
                    LOG_DBG("hfp_ag_call_call_incoming HFAG_BCS \n");
                }
                else
                {
                    if ((!hfp_ag->bt_hfp_ag_config) || (hfp_ag->bt_hfp_ag_config->bt_hfp_ag_inband))
                    {
                        bt_hfp_ag_open_audio(s_actived_bt_hfp_ag,
                                             hfp_ag->bt_hfp_ag_config ? hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec - 1 : 0);
                    }
                }
            }

            break;

        case hfp_ag_call_call_outgoing: /* Call Outgoing */
            if ((hfp_ag->bt_hfp_ag_config) && (hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec_negotiate))
            {
                bt_hfp_ag_send_at_rsp(HFAG_BCS, NULL);
            }
            else
            {
                bt_hfp_ag_open_audio(s_actived_bt_hfp_ag,
                                     hfp_ag->bt_hfp_ag_config ? hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec - 1 : 0);
            }
            break;
    }
}

int bt_hfp_ag_handle_btrh(struct bt_hfp_ag *hfp_ag, uint8_t option)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    bt_hfp_ag_send_at_rsp(HFAG_BTRH, &option);
    return 0;
}
void bt_hfp_ag_send_callring(struct bt_hfp_ag *hfp_ag)
{
    /* Ring */
    bt_hfp_ag_send_at_rsp(HFAG_RING, NULL);
}

int bt_hfp_ag_send_call_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.call_state = value;
    bt_hfp_ag_send_at_rsp(HFAG_CALL, &value);
    return 0;
}
int bt_hfp_ag_send_callheld_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.call_state = value;
    bt_hfp_ag_send_at_rsp(HFAG_CALLHELD, &value);
    return 0;
}
int bt_hfp_ag_send_callsetup_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.call_setup_state = (hfp_ag_call_setup_status_t)value;
    bt_hfp_ag_send_at_rsp(HFAG_CALLSETUP, &value);
    return 0;
}
int bt_hfp_ag_send_service_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.server = value;
    bt_hfp_ag_send_at_rsp(HFAG_SERVICE, &value);
    return 0;
}
int bt_hfp_ag_send_signal_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.signal = value;
    bt_hfp_ag_send_at_rsp(HFAG_SIGNAL, &value);
    return 0;
}
int bt_hfp_ag_send_roaming_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.roam = value;
    bt_hfp_ag_send_at_rsp(HFAG_ROAMING, &value);
    return 0;
}
int bt_hfp_ag_send_battery_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    hfp_ag->cind.batt_lev = value;
    bt_hfp_ag_send_at_rsp(HFAG_BATTERY, &value);
    return 0;
}

int bt_hfp_ag_codec_selector(struct bt_hfp_ag *hfp_ag, uint8_t value)
{
    if ((!hfp_ag) || (!hfp_ag->bt_hfp_ag_config))
    {
        return -EINVAL;
    }
    if (!(hfp_ag->hf_features & BT_HFP_HF_FEATURE_CODEC_NEG))
    {
        return -ENOTSUP;
    }
    if (!(hfp_ag->ag_features & BT_HFP_AG_FEATURE_CODEC_NEG))
    {
        return -ENOTSUP;
    }

    hfp_ag->bt_hfp_ag_config->bt_hfp_ag_codec = value;
    bt_hfp_ag_send_at_rsp(HFAG_BCS, NULL);
    return 0;
}
int  bt_hfp_ag_send_clip(struct bt_hfp_ag *hfp_ag, uint8_t *clip_result)
{
    if (!hfp_ag)
    {
        return -EINVAL;
    }
    bt_hfp_ag_send_at_rsp(HFAG_CLIP, clip_result);
    return 0;
}
static uint8_t bt_hfp_ag_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    uint16_t param;
    int res;

    if ((result) && (result->resp_buf))
    {
        LOG_INF("sdp success callback\r\n");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &param);
        if (res < 0)
        {
            LOG_ERR("PSM is not found\r\n");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        LOG_INF("param %x\r\n", param);
        if (param != 0)
        {
            LOG_INF("HFP  Service found. Connecting ...\n");
            for (int i = 0; i < CONFIG_BT_MAX_CONN; i++)
            {
                if (s_hfp_sdp[i].bt_conn == conn)
                {
                    s_hfp_sdp[i].discoverCallback(conn, param);
                    s_hfp_sdp[i].allocated = 0U;
                    s_hfp_sdp[i].bt_conn   = NULL;
                    break;
                }
            }

            return BT_SDP_DISCOVER_UUID_STOP;
        }
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
    else
    {
        LOG_ERR("sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}
static struct bt_sdp_discover_params discov_hfp_hf = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_HANDSFREE_SVCLASS),
    .func = bt_hfp_ag_sdp_user,
    .pool = &sdp_client_pool,
};

int bt_hfp_ag_discover(struct bt_conn *conn, bt_hfp_ag_discover_callback discoverCallback)
{
    int res;
    for (int i = 0; i < CONFIG_BT_MAX_CONN; i++)
    {
        if (s_hfp_sdp[i].allocated == 0)
        {
            s_hfp_sdp[i].allocated        = 1U;
            s_hfp_sdp[i].bt_conn          = conn;
            s_hfp_sdp[i].discoverCallback = discoverCallback;
            res                           = bt_sdp_discover(conn, &discov_hfp_hf);
            if (res)
            {
                LOG_ERR("SDP discovery failed: result\r\n");
            }
            else
            {
                LOG_INF("SDP discovery started\r\n");
            }
            return res;
        }
    }
    return -ENOSPC;
}

#endif /* CONFIG_BT_HFP_AG */
#endif /* CONFIG_BT_CLASSIC */