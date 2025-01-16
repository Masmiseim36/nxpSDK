/* hfp.c - Bluetooth HFP Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#if ((defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0U)))
#include "porting.h"

#if ((defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0U)) || \
     (defined(CONFIG_BT_HFP_AG) && (CONFIG_BT_HFP_AG > 0U)))

#if ((defined(CONFIG_BT_HFP_AG) && (CONFIG_BT_HFP_AG > 0U)))
#include <bluetooth/hfp_ag.h>
#endif
#if ((defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0U)))
#include <bluetooth/hfp_hf.h>
#endif

#include "bt_pal_conn_internal.h"
#include "btp/btp.h"
#include "sco_audio_pl.h"
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_hfp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);
static volatile uint8_t s_call_status = 0;
static volatile bool hf_accept_call;
static volatile uint8_t hf_check_signal_strength; // 0xFF is invalid value for the tester.
static volatile uint8_t hf_check_mic_volume;
static volatile uint8_t hf_check_speaker_volume;
static volatile bool roam_active_state;
static volatile bool battery_charged_state; // 1-full, 0-not full
struct bt_hfp_ag *hfp_ag_handle;
static struct bt_conn *default_conn;
static struct bt_conn *default_sco_conn;
uint8_t call_active = 0;
#define MAX_COPS_NAME_SIZE (16)
static char cops_name[MAX_COPS_NAME_SIZE];
static char voice_tag[MAX_COPS_NAME_SIZE] = "\"1234567\",129"; // "+918067064000";
#if 0
static TimerHandle_t s_xTimers    = 0;
#endif
static uint8_t s_hfp_in_calling_status = 0xff;
static struct k_work s_ataRespWork;
static struct k_work s_brvaRespWork;
static bool inband_ring_tone_set;
static bool ec_nr_disabled = 1;
static bool call_setup_ongoing;
static bool audio_conn_created;
static bool mute_inband_ringtone;
static bool mem_call_list         = 0;
static bool mem_call_out_of_range = 0;
static bool ring_alert;
static uint8_t wait_call           = 0;
static bool ag_slc_connect_enabled = 0;
static struct bt_hfp_ag_cb enable_slc_ag_cb;
static struct bt_conn_cb enable_ag_slc_connect;
static struct bt_conn_cb enable_hf_slc_connect;
static uint32_t conn_count      = 0;
static uint8_t wait_for_sclc    = 0;
static uint8_t s_twc_test_case  = 0;
static uint8_t s_vr_test_case   = 0;
static uint8_t s_rthr_test_case = 0;
static uint8_t server_channel;
static uint32_t supported_codecs = 0;
static uint8_t codecs_negotiate_done = 0;
static uint8_t two_held_calls_joined = 0;
struct k_work_delayable disable_audio_work;
static void disable_audio_work_handle(struct k_work *work);
#define APP_CLASS_OF_DEVICE (0x200000U)

#include "sco_audio_pl.h"

#define APP_HFP_HF_INITIAL_VGS_GAIN 12
#define APP_HFP_HF_INITIAL_VGM_GAIN 12
static struct bt_conn_cb enable_hf_pts_connect;
hfp_hf_get_config hfp_hf_config = {
    .bt_hfp_hf_vgs = APP_HFP_HF_INITIAL_VGS_GAIN,
    .bt_hfp_hf_vgm = APP_HFP_HF_INITIAL_VGM_GAIN,

};

static struct bt_sdp_attribute hfp_hf_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(BT_SDP_ATTR_SVCLASS_ID_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), // 35 06
                BT_SDP_DATA_ELEM_LIST(
                    {
                        BT_SDP_TYPE_SIZE(BT_SDP_UUID16),          // 19
                        BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) // 11 1E
                    },
                    {
                        BT_SDP_TYPE_SIZE(BT_SDP_UUID16),              // 19
                        BT_SDP_ARRAY_16(BT_SDP_GENERIC_AUDIO_SVCLASS) // 12 03
                    }, )),
    BT_SDP_LIST(BT_SDP_ATTR_PROTO_DESC_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 12),                       // 35 10
                BT_SDP_DATA_ELEM_LIST({BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 06
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),    // 19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
                                           }, )},
                                      {BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),       // 35 05
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),     // 19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM) // 00 19
                                           },
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UINT8),        // 08
                                               BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_HF) // channel number
                                           }, )}, )),
    BT_SDP_LIST(BT_SDP_ATTR_PROFILE_DESC_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),                        // 35 08
                BT_SDP_DATA_ELEM_LIST({BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), // 35 06
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),          // 19
                                               BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) // 11 1E
                                           },
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UINT16), // 09
                                               BT_SDP_ARRAY_16(0x0108U)         // 01 08
                                           }, )}, )),
    BT_SDP_SERVICE_NAME("Handsfree"),
    BT_SDP_SUPPORTED_FEATURES(0x003f),
};
static struct bt_sdp_record hfp_hf_rec = BT_SDP_RECORD(hfp_hf_attrs);

static struct bt_sdp_attribute hfp_ag_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(BT_SDP_ATTR_SVCLASS_ID_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), // 35 06
                BT_SDP_DATA_ELEM_LIST(
                    {
                        BT_SDP_TYPE_SIZE(BT_SDP_UUID16),              // 19
                        BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_AGW_SVCLASS) // 11 1F
                    },
                    {
                        BT_SDP_TYPE_SIZE(BT_SDP_UUID16),              // 19
                        BT_SDP_ARRAY_16(BT_SDP_GENERIC_AUDIO_SVCLASS) // 12 03
                    }, )),
    BT_SDP_LIST(BT_SDP_ATTR_PROTO_DESC_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 12),                       // 35 10
                BT_SDP_DATA_ELEM_LIST({BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 06
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),    // 19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
                                           }, )},
                                      {BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),       // 35 05
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),     // 19
                                               BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM) // 00 19
                                           },
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UINT8),        // 08
                                               BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_AG) // channel number
                                           }, )}, )),
    BT_SDP_LIST(BT_SDP_ATTR_PROFILE_DESC_LIST,
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),                        // 35 08
                BT_SDP_DATA_ELEM_LIST({BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), // 35 06
                                       BT_SDP_DATA_ELEM_LIST(
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UUID16),          // 19
                                               BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) // 11 1E
                                           },
                                           {
                                               BT_SDP_TYPE_SIZE(BT_SDP_UINT16), // 09
                                               BT_SDP_ARRAY_16(0x0108U)         // 01 08
                                           }, )}, )),
    BT_SDP_SERVICE_NAME("Handsfree_ag"),
    /* External Network */
    BT_SDP_ATTR_EXTERNAL_NETWORK,
    {BT_SDP_TYPE_SIZE(BT_SDP_UINT8), BT_SDP_ARRAY_8(0x01U)},
    BT_SDP_SUPPORTED_FEATURES(0x003f),
};
static struct bt_sdp_record hfp_ag_rec = BT_SDP_RECORD(hfp_ag_attrs);
static API_RESULT audio_setup_pl_ext(uint8_t isRing, SCO_AUDIO_EP_INFO *ep_info)
{
    return API_SUCCESS;
}

static void app_hfp_hf_get_config(hfp_hf_get_config **config)
{
    *config = &hfp_hf_config;
}
void sco_audio_init_pl_ext(void)
{
    return;
}

void sco_audio_shutdown_pl_ext(void)
{
    return;
}

API_RESULT sco_audio_setup_pl_ext(SCO_AUDIO_EP_INFO *ep_info)
{
    return API_SUCCESS;
}

API_RESULT sco_audio_start_pl_ext(void)
{
    return API_SUCCESS;
}

API_RESULT sco_audio_stop_pl_ext(void)
{
    return API_SUCCESS;
}

#ifdef HCI_SCO
void sco_audio_spkr_play_pl_ext(UCHAR *m_data, UINT16 m_datalen)
{
    /* Write to Codec */
}
#endif /* HCI_SCO */

void sco_audio_play_ringtone_pl_ext(void)
{
}

void sco_audio_play_ringtone_exit_pl_ext(void)
{
}

#define APP_INQUIRY_LENGTH        (10) /* 10 * 1.28 Sec */
#define APP_INQUIRY_NUM_RESPONSES (20)

static uint32_t br_discover_result_count;
static struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];
static void br_device_found(
    size_t index, const bt_addr_t *addr, int8_t rssi, const uint8_t cod[3], const uint8_t eir[240])
{
    char br_addr[BT_ADDR_STR_LEN];
    char name[239];
    int len = 240;

    (void)memset(name, 0, sizeof(name));

    while (len)
    {
        if (len < 2)
        {
            break;
        }

        /* Look for early termination */
        if (!eir[0])
        {
            break;
        }

        /* check if field length is correct */
        if (eir[0] > len - 1)
        {
            break;
        }

        switch (eir[1])
        {
            case BT_DATA_NAME_SHORTENED:
            case BT_DATA_NAME_COMPLETE:
                memcpy(name, &eir[2], (eir[0] - 1) > (sizeof(name) - 1) ? (sizeof(name) - 1) : (eir[0] - 1));
                break;
            default:
                break;
        }

        /* Parse next AD Structure */
        len -= (eir[0] + 1);
        eir += (eir[0] + 1);
    }

    bt_addr_to_str(addr, br_addr, sizeof(br_addr));
    PRINTF("[%d]: %s, RSSI %i %s\r\n", index + 1, br_addr, rssi, name);
}
static void br_discovery_complete(struct bt_br_discovery_result *results, size_t count)
{
    size_t index;

    br_discover_result_count = count;
    PRINTF("BR/EDR discovery complete\r\n");
    for (index = 0; index < count; ++index)
    {
        br_device_found(index, &results[index].addr, results[index].rssi, results[index].cod, results[index].eir);
    }
}

static void app_discover(void)
{
    int err;
    struct bt_br_discovery_param param;

    param.length  = APP_INQUIRY_LENGTH;
    param.limited = 0U;
    err = bt_br_discovery_start(&param, br_discovery_results, APP_INQUIRY_NUM_RESPONSES, br_discovery_complete);
    if (err != 0)
    {
        PRINTF("Failed to start discovery\r\n");
    }
    else
    {
        PRINTF("Discovery started. Please wait ...\r\n");
    }
}
static uint8_t supported_commands(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    struct btp_hfp_read_supported_commands_rp *rp = rsp;

    /* octet 0 */
    tester_set_bit(rp->data, BTP_HFP_READ_SUPPORTED_COMMANDS);
    tester_set_bit(rp->data, BTP_HFP_MAKE_DISCOVERABLE);

    *rsp_len = sizeof(*rp) + 2;

    return BTP_STATUS_SUCCESS;
}

static void ring_cb(struct bt_conn *conn)
{
    ring_alert = true;
    if (hf_accept_call)
    {
        bt_hfp_hf_send_cmd(conn, BT_HFP_HF_ATA);
        hf_accept_call = 0;
    }
}

static void signal(struct bt_conn *conn, uint32_t value)
{
    hf_check_signal_strength = value;
}

static void roam(struct bt_conn *conn, uint32_t value)
{
    roam_active_state = value ? true : false;
}

static void battery(struct bt_conn *conn, uint32_t value)
{
    if (value == 5)
    {
        battery_charged_state = true;
    }
    else
    {
        battery_charged_state = false;
    }
}

static void connected(struct bt_conn *conn)
{
    default_conn = conn;
    conn_count++;
}

static void disconnected(struct bt_conn *conn)
{
    if (default_conn)
    {
        bt_hfp_hf_disconnect(default_conn);
        //  bt_conn_unref(default_conn);
        //  default_conn = NULL;
    }
}

static void cops_query_result(struct bt_conn *conn, uint8_t *result, uint16_t len)
{
    memset(cops_name, 0, MAX_COPS_NAME_SIZE);
    strncpy(&cops_name[0], (char const *)result, MAX_COPS_NAME_SIZE > len ? len : MAX_COPS_NAME_SIZE);
}

void hfp_hf_sco_connected(struct bt_conn *conn, struct bt_conn *sco_conn)
{
    audio_conn_created = true;
    default_sco_conn   = sco_conn;
    tester_event(BTP_SERVICE_ID_HFP, BTP_HFP_EV_SCO_CONNECTED, NULL, 0);
}

void hfp_hf_sco_disconnected(struct bt_conn *sco_conn, uint8_t reason)
{
    audio_conn_created = false;
    tester_event(BTP_SERVICE_ID_HFP, BTP_HFP_EV_SCO_DISCONNECTED, NULL, 0);
}

static void call_setup(struct bt_conn *conn, uint32_t value)
{
    if (value == 1)
    {
        call_setup_ongoing = true;
    }
    else
    {
        call_setup_ongoing = false;
    }
}

void ringtone_inband_set(struct bt_conn *conn, uint8_t set)
{
    inband_ring_tone_set = set;
}

void volume_update(struct bt_conn *conn, hf_volume_type_t type, int volume)
{
    if (type == hf_volume_type_speaker)
    {
        hf_check_speaker_volume = volume;
    }
    else if (type == hf_volume_type_mic)
    {
        hf_check_mic_volume = volume;
    }
}
static void call(struct bt_conn *conn, uint32_t value)
{
    if (value == 1)
    {
        s_call_status = 2;
    }
    else
    {
        s_call_status = 0;
        //    bt_hfp_hf_close_audio(default_sco_conn);
    }
    printf("Call indicator value: %u\n", value);
}
static struct bt_hfp_hf_cb hf_cb = {
    .connected           = connected,
    .disconnected        = disconnected,
    .sco_connected       = hfp_hf_sco_connected,
    .sco_disconnected    = hfp_hf_sco_disconnected,
    .service             = NULL,
    .call                = call,
    .call_setup          = call_setup,
    .call_held           = NULL,
    .signal              = signal,
    .roam                = roam,
    .battery             = battery,
    .ring_indication     = ring_cb,
    .call_phnum          = NULL,
    .voicetag_phnum      = NULL,
    .waiting_call        = NULL,
    .ringtone_inband_set = ringtone_inband_set,
    .volume_update = volume_update,
    .get_config    = app_hfp_hf_get_config,
};

static uint8_t make_discoverable(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (bt_hfp_hf_register(&hf_cb))
    {
        return BTP_STATUS_SUCCESS;
    }
    bt_sdp_register_service(&hfp_hf_rec);
    return BTP_STATUS_SUCCESS;
}
static uint8_t register_connect_cb(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    bt_conn_cb_register(&enable_hf_pts_connect);
    return BTP_STATUS_SUCCESS;
}
static uint8_t make_ag_discoverable(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err;

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(APP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
    }
    err = bt_hfp_ag_init();
    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    if (bt_hfp_ag_register_cb(&enable_slc_ag_cb))
    {
        return BTP_STATUS_SUCCESS;
    }
    bt_sdp_register_service(&hfp_ag_rec);
    bt_conn_cb_register(&enable_ag_slc_connect);
    ag_slc_connect_enabled = 1;

    return BTP_STATUS_SUCCESS;
}
static uint8_t hf_answer_call(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    hf_accept_call = true;

    return BTP_STATUS_SUCCESS;
}

static hfp_ag_get_config enable_slc_ag_config = {
    .bt_hfp_ag_vgs             = 15,
    .bt_hfp_ag_vgm             = 15,
    .bt_hfp_ag_codec           = 1,
    .bt_hfp_ag_nrec            = 1,
    .bt_hfp_ag_inband          = 0,
    .bt_hfp_ag_codec_negotiate = 0,
    .bt_hfp_ag_dial            = 0,
};

static char network_operator_name[] = "\"ZephyrSIM\"";
static void unkown_at(struct bt_hfp_ag *hfp_ag, char *value, uint32_t length)
{
    if (strncmp(value, "AT+COPS?", 8) == 0)
    {
        int err = bt_hfp_ag_set_cops(hfp_ag_handle, network_operator_name);
        if (!err)
        {
            memcpy(&cops_name[0], &network_operator_name[0], sizeof(network_operator_name));
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
        }
        else
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nERROR\r\n", 9u);
        }
    }
    else
    {
        bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
    }
}
static uint8_t s_brva     = 0;
static uint8_t s_btrh_ata = 0;
static void k_work_brva_response(struct k_work *work)
{
    if (s_brva == 1)
    {
        //   bt_hfp_ag_send_enable_voice_recognition(hfp_ag_handle);
        bt_hfp_ag_open_audio(hfp_ag_handle, 0);
    }
    else
    {
        //  bt_hfp_ag_send_disable_voice_recognition(hfp_ag_handle);
        bt_hfp_ag_close_audio(hfp_ag_handle);
    }
}
static void k_work_ata_response(struct k_work *work)
{
    s_hfp_in_calling_status = 3;
    if (s_btrh_ata == 0)
    {
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 1);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
    }
#if 0
    if (s_xTimers != 0)
    {
        xTimerStop(s_xTimers, 0);
        xTimerDelete(s_xTimers, 0);
        s_xTimers = 0;
    }
#endif
    bt_hfp_ag_call_status_pl(hfp_ag_handle, hfp_ag_call_call_incoming);
}

static void ag_connected(struct bt_hfp_ag *hfp_ag)
{
    hfp_ag_handle = hfp_ag;
    if (call_active)
    {
        s_hfp_in_calling_status = 3;
        hfp_ag_cind_t cind_setting;
        bt_hfp_ag_get_cind_setting(hfp_ag_handle, &cind_setting);
        cind_setting.call_state = 1;
        bt_hfp_ag_set_cind_setting(hfp_ag_handle, &cind_setting);
    }
    else
    {
        s_hfp_in_calling_status = 1;
    }
    k_work_init(&s_ataRespWork, k_work_ata_response);
    k_work_init(&s_brvaRespWork, k_work_brva_response);
}

static void ag_disconnected(struct bt_hfp_ag *hfp_ag)
{
    // hfp_ag_handle = NULL;
    if (default_conn)
    {
        bt_conn_unref(default_conn);
    }
    //    default_conn = NULL;
}

static void ata_response(struct bt_hfp_ag *hfp_ag)
{
    k_work_submit(&s_ataRespWork);
}

void hfp_ag_sco_connected(struct bt_hfp_ag *ag, struct bt_conn *sco_conn)
{
    default_sco_conn   = sco_conn;
    audio_conn_created = true;

    tester_event(BTP_SERVICE_ID_HFP, BTP_HFP_EV_SCO_CONNECTED, NULL, 0);
}

void hfp_ag_sco_disconnected(struct bt_hfp_ag *ag)
{
    audio_conn_created = false;

    tester_event(BTP_SERVICE_ID_HFP, BTP_HFP_EV_SCO_DISCONNECTED, NULL, 0);
}

void chup_response(struct bt_hfp_ag *hfp_ag)
{
    if ((wait_call == 1) && (s_hfp_in_calling_status == 1))
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        wait_call = 0;
    }
    if (s_hfp_in_calling_status == 2)
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        s_hfp_in_calling_status = 1;
        return;
    }
    if (s_hfp_in_calling_status != 1)
    {
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
        s_hfp_in_calling_status = 1;
        bt_hfp_ag_call_status_pl(hfp_ag_handle, hfp_ag_call_call_end);
    }
}

void last_dial(struct bt_hfp_ag *hfp_ag)
{
    if (s_hfp_in_calling_status == 3)
    {
        bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        s_hfp_in_calling_status = 4;
    }
    else
    {
        if (mem_call_list == 0)
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        }
        else
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nERROR\r\n", 9u);
        }
        s_hfp_in_calling_status = 3;
    }
}

void memory_dial(struct bt_hfp_ag *ag, uint32_t location)
{
    if (s_hfp_in_calling_status == 3)
    {
        bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        s_hfp_in_calling_status = 4;
    }
    else
    {
        if (mem_call_list == 0)
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        }
        else
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nERROR\r\n", 9u);
        }
        s_hfp_in_calling_status = 3;
    }
}
void dial(struct bt_hfp_ag *hfp_ag, char *number)
{
    if (s_hfp_in_calling_status == 3)
    {
        bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        s_hfp_in_calling_status = 4;
    }
    else
    {
        if (mem_call_list == 0)
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
            s_hfp_in_calling_status = 2;
        }
        else
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nERROR\r\n", 9u);
        }
    }
}
static void brva(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    k_work_submit(&s_brvaRespWork);
    s_brva = value;
}

static void btrh(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    if ((value == 1) || (value == 0))
    {
        k_work_submit(&s_ataRespWork);
    }
    if (value == 2)
    {
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
    }
}
static uint8_t call_held = 0;
static void chld(struct bt_hfp_ag *hfp_ag, uint8_t option, uint8_t index)
{
    printf("AT_CHLD mutlipcall option  index :%d %d\n", option, index);
    if (option == 0)
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
    }
    else if (option == 1)
    {
        printf("  Release Active Calls and accept held/waiting call\n");

        if (call_held == 1)
        {
            bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 0);
        }
        else
        {
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        }
    }
    else if (option == 2)
    {
        printf(
            "  Hold Active Call and accept already "
            "held/new waiting call\n");
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 1);
        call_held = 1;
    }
    else if (option == 3)
    {
        printf(" bt multipcall 3. Conference all calls\n");
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 0);
        two_held_calls_joined = 1;
    }
    else if (option == 4)
    {
        printf(" bt multipcall 4. Connect other calls and disconnect self from TWC\n");
    }
}

static void binp(struct bt_hfp_ag *hfp_ag)
{
    bt_hfp_ag_set_phnum_tag(hfp_ag_handle, voice_tag);
}

static uint8_t s_outing_call = 0;
static void clcc(struct bt_hfp_ag *hfp_ag)
{
    // bt_hfp_ag_set_clcc(hfp_ag_handle, "1,1,1,0,0, 1234567,129");
    if (two_held_calls_joined == 1)
    {
        bt_hfp_ag_set_clcc(hfp_ag_handle, "1,1,0,0,1,\"1234567\",129");
        bt_hfp_ag_set_clcc(hfp_ag_handle, "2,1,0,0,1,\"7654321\",129");
        return;
    }
    if (s_twc_test_case == 3)
    {
        bt_hfp_ag_set_clcc(hfp_ag_handle, "1,1,0,0,0,\"1234567\",129");
        bt_hfp_ag_set_clcc(hfp_ag_handle, "2,0,0,0,0,\"7654321\",129");
        return;
    }
    if (s_outing_call)
    {
        bt_hfp_ag_set_clcc(hfp_ag_handle, "1,0,0,0,0,\"1234567\",129");
    }
    else
    {
        bt_hfp_ag_set_clcc(hfp_ag_handle, "1,1,4,0,0,\"1234567\",129");
    }
}
static char hf_dtmf_code;
static void recv_dtmf_codes(struct bt_hfp_ag *hfp_ag, char dtmf_code)
{
    hf_dtmf_code = dtmf_code;
}
void volume_control(struct bt_hfp_ag *hfp_ag, hf_ag_volume_type_t type, int volume)
{
    if (type == hf_ag_volume_type_speaker)
    {
        hf_check_speaker_volume = volume;
    }
    else if (type == hf_ag_volume_type_mic)
    {
        hf_check_mic_volume = volume;
    }
}

struct k_work_delayable enable_ag_bcc_work;
static void enable_ag_bcc_work_handle(struct k_work *work)
{
    bt_hfp_ag_codec_selector(hfp_ag_handle, 2);
}

static void codec_connect_req(struct bt_hfp_ag *hfp_age)
{
    k_work_init_delayable(&enable_ag_bcc_work, enable_ag_bcc_work_handle);
    k_work_schedule(&enable_ag_bcc_work, K_MSEC(2000));
}
hfp_ag_get_config hfp_ag_config = {
    .bt_hfp_ag_vgs             = 15,
    .bt_hfp_ag_vgm             = 15,
    .bt_hfp_ag_codec           = 1,
    .bt_hfp_ag_nrec            = 1,
    .bt_hfp_ag_inband          = 0,
    .bt_hfp_ag_codec_negotiate = 1,
    .bt_hfp_ag_dial            = 0,
};
void get_config(struct bt_hfp_ag *hfp_ag, hfp_ag_get_config **config)
{
    *config = &hfp_ag_config;
}

void codec(struct bt_hfp_ag *ag, uint32_t ids)
{
    supported_codecs = ids;
}

void codec_negotiate(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    hfp_ag_config.bt_hfp_ag_codec = value;
    codecs_negotiate_done = 1;
}

static struct bt_hfp_ag_cb enable_slc_ag_cb = {
    .connected        = ag_connected,
    .disconnected     = ag_disconnected,
    .sco_connected    = hfp_ag_sco_connected,
    .sco_disconnected = hfp_ag_sco_disconnected,
    .ata_response     = ata_response,
    .chup_response    = chup_response,
    .volume_control   = volume_control,
    .dial             = dial,
    .last_dial        = last_dial,
    .memory_dial      = memory_dial,
    .brva             = brva,
    .chld             = chld,
    .clcc             = clcc,
    .recv_dtmf_codes  = recv_dtmf_codes,
    .codec_negotiate  = codec_negotiate,
    .codec_connect_req = codec_connect_req,
    .unkown_at        = unkown_at,
    .get_config       = get_config,
    .codec            = codec,
};

int enable_slc_ag_discover(struct bt_conn *conn, uint8_t channel)
{
    int err;

    enable_slc_ag_config.server_channel = channel;
    err                     = bt_hfp_ag_connect(conn, &enable_slc_ag_config, &enable_slc_ag_cb, &hfp_ag_handle);
    hf_check_mic_volume     = enable_slc_ag_config.bt_hfp_ag_vgm;
    hf_check_speaker_volume = enable_slc_ag_config.bt_hfp_ag_vgs;
    return err;
}

static void enable_ag_slc_connected(struct bt_conn *conn, uint8_t err)
{
    int stat;

    if (!err)
    {
        bt_conn_cb_unregister(&enable_ag_slc_connect);
        bt_hfp_ag_discover(conn, &enable_slc_ag_discover);
    }
}

static void enable_ag_slc_disconnected(struct bt_conn *conn, uint8_t err)
{
}
int enable_slc_hf_discover(struct bt_conn *conn, uint8_t channel)
{
    int err = 0;

    server_channel = channel;
    default_conn   = conn;
    int api_retval = bt_hfp_hf_connect(default_conn, server_channel);
    if (api_retval)
    {
        return -EIO;
    }
    return err;
}

static void enable_hf_slc_connected(struct bt_conn *conn, uint8_t err)
{
    int stat;

    if (!err)
    {
        default_conn = conn;
        bt_conn_cb_unregister(&enable_hf_slc_connect);
        bt_hfp_hf_discover(conn, &enable_slc_hf_discover);
    }
}
static struct bt_conn_cb enable_ag_slc_connect = {
    .connected    = enable_ag_slc_connected,
    .disconnected = enable_ag_slc_disconnected,
};
static struct bt_conn_cb enable_hf_slc_connect = {
    .connected = enable_hf_slc_connected,
};
/* ACL created by PTS */
static void enable_hf_pts_connected(struct bt_conn *conn, uint8_t err);
static void enable_hf_pts_disconnected(struct bt_conn *conn, uint8_t err);
static struct bt_conn_cb enable_hf_pts_connect = {
    .connected    = enable_hf_pts_connected,
    .disconnected = enable_hf_pts_disconnected,
};
static void enable_hf_pts_connected(struct bt_conn *conn, uint8_t err)
{
    if (!err)
    {
        default_conn = conn;
        bt_conn_cb_unregister(&enable_hf_slc_connect);
        bt_hfp_hf_discover(conn, &enable_slc_hf_discover);
    }
}
static void enable_hf_pts_disconnected(struct bt_conn *conn, uint8_t err)
{
    bt_conn_cb_unregister(&enable_hf_pts_connect);
}
static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Pairing cancelled: %s\n", addr);
}

static void passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    PRINTF("Passkey %06u\n", passkey);
}

static struct bt_conn_auth_cb auth_cb_display = {
    .cancel = auth_cancel, .passkey_display = passkey_display, /* Passkey display callback */
                                                               //  .passkey_confirm = passkey_confirm,
};
static uint8_t enable_slc(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_enable_slc_cmd *cp = cmd;
    struct bt_conn *conn;
    int err;

    if (cp->is_ag == 1)
    {
        if (default_conn)
        {
            if (default_conn->state == BT_CONN_CONNECTED)
            {
                bt_hfp_ag_connect(default_conn, &enable_slc_ag_config, &enable_slc_ag_cb, &hfp_ag_handle);
                return BTP_STATUS_SUCCESS;
            }
            else
            {
                default_conn = NULL;
            }
        }
        err = bt_hfp_ag_init();
        bt_hfp_ag_register_cb(&enable_slc_ag_cb);
        bt_sdp_register_service(&hfp_ag_rec);
        if (err != 0)
        {
            return BTP_STATUS_FAILED;
        }
        if (ag_slc_connect_enabled == 0)
        {
            bt_conn_cb_register(&enable_ag_slc_connect);
        }

        conn = bt_conn_create_br(&cp->address, BT_BR_CONN_PARAM_DEFAULT);
        if (conn == NULL)
        {
            return BTP_STATUS_FAILED;
        }
        default_conn = conn;
    }
    else
    {
        struct bt_conn *conn;
        if (default_conn)
        {
            if (default_conn->state == BT_CONN_CONNECTED)
            {
                bt_hfp_hf_connect(default_conn, server_channel);
                return BTP_STATUS_SUCCESS;
            }
            else
            {
                default_conn = NULL;
            }
        }

        if (bt_hfp_hf_register(&hf_cb))
        {
        }
        //    bt_conn_cb_register(&enable_hf_slc_connect);
        conn = bt_conn_lookup_addr_br(&cp->address);
        if (conn != NULL)
        {
            bt_conn_unref(conn);
        }

        conn = bt_conn_create_br(&cp->address, BT_BR_CONN_PARAM_DEFAULT);
        if (conn == NULL)
        {
            return BTP_STATUS_FAILED;
        }
        default_conn = conn;
        bt_conn_auth_cb_register(&auth_cb_display);
    }

    return BTP_STATUS_SUCCESS;
}

struct k_work_delayable disable_slc_work;
static void disable_slc_work_handle(struct k_work *work)
{
    uint8_t count = 0;
    if (hfp_ag_handle)
    {
        bt_hfp_ag_disconnect(hfp_ag_handle);
    }
    else
    {
        while (conn_count == 0)
        {
            count++;
            OSA_TimeDelay(500);
            if (count > 100)
                break;
        }
        bt_hfp_hf_disconnect(default_conn);
    }
}
static uint8_t disable_slc(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&disable_slc_work, disable_slc_work_handle);
    k_work_schedule(&disable_slc_work, K_MSEC(500));

    return BTP_STATUS_SUCCESS;
}

static uint8_t disable_network(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_disable_network_cmd *cp = cmd;
    int err;

    err = bt_hfp_ag_send_service_indicator(hfp_ag_handle, 0);
    if (err != 0)
    {
        return BTP_STATUS_FAILED;
    }
    bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
    bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nNO CARRIER\r\n", 14);
    return BTP_STATUS_SUCCESS;
}

static uint8_t enable_network(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_enable_network_cmd *cp = cmd;
    int err;

    err = bt_hfp_ag_send_service_indicator(hfp_ag_handle, 1);
    if (err != 0)
    {
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

static uint8_t signal_strength_verify(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_signal_strength_verify_cmd *cp = cmd;

    if (hf_check_signal_strength == cp->signal_strength)
    {
        return BTP_STATUS_SUCCESS;
    }
    else
    {
        return BTP_STATUS_FAILED;
    }
}
static uint8_t signal_strength_send(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_signal_strength_send_cmd *cp = cmd;
    int err;

    err = bt_hfp_ag_send_signal_indicator(hfp_ag_handle, cp->signal_strength);
    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    hf_check_signal_strength = cp->signal_strength;
    return BTP_STATUS_SUCCESS;
}

static uint8_t speaker_mic_volume_send(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_speaker_mic_volume_send_cmd *cp = cmd;

    int err;
    if (cp->speaker_mic == 0x0)
    {
        if (hfp_ag_handle != NULL)
        {
            err = bt_hfp_ag_set_volume_control(hfp_ag_handle, hf_ag_volume_type_speaker, cp->speaker_mic_volume);
            if (err)
            {
                return BTP_STATUS_FAILED;
            }
        }
        else if (default_conn != NULL)
        {
            err = bt_hfp_hf_volume_update(default_conn, hf_volume_type_speaker, cp->speaker_mic_volume);
            if (err)
            {
                return BTP_STATUS_FAILED;
            }
        }
        hf_check_speaker_volume = cp->speaker_mic_volume;
    }
    else if (cp->speaker_mic == 0x1)
    {
        if (hfp_ag_handle != NULL)
        {
            err = bt_hfp_ag_set_volume_control(hfp_ag_handle, hf_ag_volume_type_mic, cp->speaker_mic_volume);
            if (err)
            {
                return BTP_STATUS_FAILED;
            }
        }
        else if (default_conn != NULL)
        {
            err = bt_hfp_hf_volume_update(default_conn, hf_volume_type_mic, cp->speaker_mic_volume);
            if (err)
            {
                return BTP_STATUS_FAILED;
            }
        }

        hf_check_mic_volume = cp->speaker_mic_volume;
    }
    else
    {
        return BTP_STATUS_UNKNOWN_CMD;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t speaker_mic_volume_verify(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_speaker_mic_volume_verify_cmd *cp = cmd;
    if (cp->speaker_mic == 0x1)
    {
        if (hf_check_mic_volume == cp->speaker_mic_volume)
        {
            return BTP_STATUS_SUCCESS;
        }
        else
        {
            return BTP_STATUS_FAILED;
        }
    }
    else if (cp->speaker_mic == 0x0)
    {
        if (hf_check_speaker_volume == cp->speaker_mic_volume)
        {
            return BTP_STATUS_SUCCESS;
        }
        else
        {
            return BTP_STATUS_FAILED;
        }
    }
    else
    {
        return BTP_STATUS_UNKNOWN_CMD;
    }
}
static uint8_t dtmf_code_send(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
#if 0
    const struct btp_hfp_dtmf_code_send_cmd *cp = cmd;
    bt_hfp_hf_send_dtmf_code(default_conn, cp->dtmf_code);
#endif
    return BTP_STATUS_SUCCESS;
}
static uint8_t dtmf_code_verify(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_dtmf_code_verify_cmd *cp = cmd;
    if (hf_dtmf_code == cp->dtmf_code)
    {
        return BTP_STATUS_SUCCESS;
    }
    else
    {
        return BTP_STATUS_FAILED;
    }
}
static uint8_t verify_roam_active(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (roam_active_state)
    {
        return BTP_STATUS_SUCCESS;
    }
    return BTP_STATUS_FAILED;
}

static uint8_t verify_roam_inactive(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (!roam_active_state)
    {
        return BTP_STATUS_SUCCESS;
    }
    return BTP_STATUS_FAILED;
}

static uint8_t make_roam_active(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    int err = bt_hfp_ag_send_roaming_indicator(hfp_ag_handle, 1);

    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t make_roam_inactive(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    int err = bt_hfp_ag_send_roaming_indicator(hfp_ag_handle, 0);

    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t verify_battery_discharged(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (!battery_charged_state)
    {
        return BTP_STATUS_SUCCESS;
    }

    return BTP_STATUS_FAILED;
}

static uint8_t verify_battery_charged(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (battery_charged_state)
    {
        return BTP_STATUS_SUCCESS;
    }

    return BTP_STATUS_FAILED;
}

static uint8_t make_battery_discharged(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (hfp_ag_handle != NULL)
    {
        int err = bt_hfp_ag_send_battery_indicator(hfp_ag_handle, 3);

        if (err)
        {
            return BTP_STATUS_FAILED;
        }
    }
    else
    {
    }
    return BTP_STATUS_SUCCESS;
}

static uint8_t make_battery_charged(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    if (hfp_ag_handle != NULL)
    {
        int err = bt_hfp_ag_send_battery_indicator(hfp_ag_handle, 5);

        if (err)
        {
            return BTP_STATUS_FAILED;
        }
    }
    else
    {
    }
    return BTP_STATUS_SUCCESS;
}
struct k_work_delayable hfp_query_network_operator_work;
static void hfp_query_network_operator_handle(struct k_work *work)
{
#if 0
    int err;

    err = bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_SET_NW_NAME_FORMAT);
    if (err == -EBUSY)
    {
        OSA_TimeDelay(500);
        err = bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_SET_NW_NAME_FORMAT);
        if (err)
        {
            return;
        }
    }
    OSA_TimeDelay(1000);
    err = bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_AT_COPS_Q);
    if (err == -EBUSY)
    {
        OSA_TimeDelay(500);
        err = bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_AT_COPS_Q);
        if (err)
        {
            return;
        }
    }
#endif
    return;
}
static uint8_t query_network_operator(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&hfp_query_network_operator_work, hfp_query_network_operator_handle);
    k_work_schedule(&hfp_query_network_operator_work, K_MSEC(2000));
    return 0;
}

static uint8_t verify_network_operator(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const char *cp = (const char *)cmd;

    if (strncmp(cp, cops_name, MAX_COPS_NAME_SIZE) == 0)
    {
        return BTP_STATUS_SUCCESS;
    }

    return BTP_STATUS_FAILED;
}
static uint8_t verify_voice_tag(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const char *cp = (const char *)cmd;

    if (strncmp(cp, voice_tag, MAX_COPS_NAME_SIZE) == 0)
    {
        return BTP_STATUS_SUCCESS;
    }

    return BTP_STATUS_FAILED;
}

static uint8_t make_sure_ag_registered_on_home_network(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    int err;

    err = bt_hfp_ag_init();
    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    err = bt_hfp_ag_register_cb(&enable_slc_ag_cb);
    if (err)
    {
        return BTP_STATUS_FAILED;
    }
    bt_sdp_register_service(&hfp_ag_rec);
    return BTP_STATUS_SUCCESS;
}

struct k_work_delayable enable_audio_work;
static void enable_audio_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_open_audio(hfp_ag_handle, 0);
    }
    else
    {
        bt_hfp_hf_open_audio(default_conn, 0);
    }
}

static uint8_t enable_audio(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&enable_audio_work, enable_audio_work_handle);
    k_work_schedule(&enable_audio_work, K_MSEC(10));
    return BTP_STATUS_SUCCESS;
}

struct k_work_delayable disable_audio_work;
static void disable_audio_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_close_audio(hfp_ag_handle);
    }
    else
    {
        bt_hfp_hf_close_audio(default_conn);
    }
}

static uint8_t disable_audio(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&disable_audio_work, disable_audio_work_handle);
    k_work_schedule(&disable_audio_work, K_MSEC(1000));

    return BTP_STATUS_SUCCESS;
}
static uint8_t rec_clcc = 0;
#if 0
static void vTimerRingCallback(TimerHandle_t xTimer)
{
    bt_hfp_ag_send_callring(hfp_ag_handle);
}
#endif

static int hfp_ag_start_incoming_call()
{
    if (s_hfp_in_calling_status == 1)
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 1);
#if 0
        s_xTimers = xTimerCreate("RingTimer", (2000) + 10, pdTRUE, 0, vTimerRingCallback);
        xTimerStart(s_xTimers, 0);
#endif
        OSA_TimeDelay(500);
        bt_hfp_ag_send_callring(hfp_ag_handle);
        bt_hfp_ag_send_clip(hfp_ag_handle, "1234567");

        s_hfp_in_calling_status = 2;
        ;
        return 0;
    }
    return -1;
}

struct k_work_delayable ag_enable_call_work;
static void ag_enable_call_work_handle(struct k_work *work)
{
    hfp_ag_start_incoming_call();
}

static uint8_t ag_enable_call(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&ag_enable_call_work, ag_enable_call_work_handle);
    if (s_hfp_in_calling_status != 0xff)
    {
        k_work_schedule(&ag_enable_call_work, K_MSEC(20));
    }
    else
    {
        call_active = 1;
    }

    return BTP_STATUS_SUCCESS;
}

static int hfp_ag_stop_incoming_call()
{
    if (s_hfp_in_calling_status >= 2)
    {
        bt_hfp_ag_call_status_pl(hfp_ag_handle, hfp_ag_call_call_end);

#if 0
        if (s_xTimers != 0)
        {
            xTimerStop(s_xTimers, 0);
            xTimerDelete(s_xTimers, 0);
            s_xTimers = 0;
        }
#endif
        if (s_hfp_in_calling_status > 2)
        {
            bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
        }
        else
        {
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        }
        s_hfp_in_calling_status = 1;
        return 0;
    }
    return -1;
}

struct k_work_delayable ag_disable_call_external_work;
static void ag_disable_call_external_work_handle(struct k_work *work)
{
    hfp_ag_stop_incoming_call();
}
static uint8_t ag_disable_call_external(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    k_work_init_delayable(&ag_disable_call_external_work, ag_disable_call_external_work_handle);
    k_work_schedule(&ag_disable_call_external_work, K_MSEC(500));

    return BTP_STATUS_SUCCESS;
}

static uint8_t verify(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_verify_cmd *cp = cmd;

    switch (cp->verify_type)
    {
        case HFP_VERIFY_INBAND_RING:
            if (inband_ring_tone_set && call_setup_ongoing && audio_conn_created)
            {
                return BTP_STATUS_SUCCESS;
            }
            else
            {
                uint16_t delay = 12;

                while (delay--)
                {
                    OSA_TimeDelay(500);
                    if (inband_ring_tone_set && call_setup_ongoing && audio_conn_created)
                    {
                        return BTP_STATUS_SUCCESS;
                    }
                }
            }
            break;
        case HFP_VERIFY_INBAND_RING_MUTING:
            if (inband_ring_tone_set && (!mute_inband_ringtone))
            {
                return BTP_STATUS_FAILED;
            }
            else
            {
                return BTP_STATUS_SUCCESS;
            }
            break;
        case HFP_VERIFY_IUT_ALERTING:
            if (ring_alert && call_setup_ongoing)
            {
                return BTP_STATUS_SUCCESS;
            }
            break;
        case HFP_VERIFY_IUT_NOT_ALERTING:
            if (!(ring_alert && call_setup_ongoing))
            {
                return BTP_STATUS_SUCCESS;
            }
            break;
        case HFP_VERIFY_EC_NR_DISABLED:
            if (ec_nr_disabled)
            {
                return BTP_STATUS_SUCCESS;
            }
            else
            {
                return BTP_STATUS_FAILED;
            }
            break;
        default:
            break;
    }
    return BTP_STATUS_FAILED;
}

struct k_work_delayable ag_answer_call_work;
static void ag_answer_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 1);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        bt_hfp_ag_call_status_pl(hfp_ag_handle, hfp_ag_call_call_incoming);
        s_hfp_in_calling_status = 3;
    }
    else
    {
        bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_ATA);
    }
}

struct k_work_delayable hfp_reject_call_work;
static void hfp_reject_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
    }
    else
    {
        bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_AT_CHUP);
    }
}
struct k_work_delayable hfp_out_call_work;
static void hfp_out_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 2);
        bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 3);
        bt_hfp_ag_send_call_indicator(hfp_ag_handle, 1);
        s_outing_call = 1;
    }
    else
    {
        bt_hfp_hf_dial(default_conn, "1234567");
    }
}
struct k_work_delayable hfp_out_mem_call_work;
static void hfp_out_mem_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
    }
    else
    {
        if (mem_call_out_of_range == 0)
        {
            bt_hfp_hf_dial_memory(default_conn, 1);
        }
        else
        {
            bt_hfp_hf_dial_memory(default_conn, 9);
        }
    }
}
struct k_work_delayable hfp_out_last_call_work;
static void hfp_out_last_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
    }
    else
    {
        bt_hfp_hf_last_dial(default_conn);
    }
}
struct k_work_delayable hfp_twc_call_work;
static void hfp_twc_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        if ((s_hfp_in_calling_status == 4) || (wait_call == 1))
        {
            bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 1);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
            call_held = 1;
            bt_hfp_ag_close_audio(hfp_ag_handle);
        }
        else
        {
            bt_hfp_ag_send_ccwa_indicator(hfp_ag_handle, "7654321");
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 1);
            wait_call = 1;
        }
    }
    else
    {
    }
}

struct k_work_delayable hfp_twc_test_call_work;
static void hfp_twc_test_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        if (s_twc_test_case == 0)
        {
        }
        else if (s_twc_test_case == 4)
        {
            bt_hfp_ag_unknown_at_response(hfp_ag_handle, "\r\nOK\r\n", 6u);
            bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 0);
        }
    }
    else
    {
        if (s_twc_test_case == 0)
        {
            bt_hfp_hf_multiparty_call_option(default_conn, hf_multiparty_call_option_one);
        }
        else if (s_twc_test_case == 1)
        {
            bt_hfp_hf_multiparty_call_option(default_conn, hf_multiparty_call_option_two);
        }
        else if (s_twc_test_case == 2)
        {
            bt_hfp_hf_multiparty_call_option(default_conn, hf_multiparty_call_option_three);
        }
        else if (s_twc_test_case == 3)
        {
            bt_hfp_hf_multiparty_call_option(default_conn, hf_multiparty_call_option_four);
        }
        else if (s_twc_test_case == 4)
        {
            bt_hfp_hf_multiparty_call_option(default_conn, hf_multiparty_call_option_five);
        }
    }
}

struct k_work_delayable hfp_vr_test_call_work;
static void hfp_vr_test_call_work_handle(struct k_work *work)
{
    if (s_vr_test_case == 1)
    {
        if (hfp_ag_handle)
        {
            bt_hfp_ag_send_enable_voice_recognition(hfp_ag_handle);
        }
        else
        {
            bt_hfp_hf_start_voice_recognition(default_conn);
        }
    }
    else if (s_vr_test_case == 0)
    {
        if (hfp_ag_handle)
        {
            bt_hfp_ag_send_disable_voice_recognition(hfp_ag_handle);
        }
        else
        {
            bt_hfp_hf_stop_voice_recognition(default_conn);
        }
    }
}

struct k_work_delayable hfp_query_list_call_work;
static void hfp_query_list_call_work_handle(struct k_work *work)
{
    //   bt_hfp_hf_query_list_current_calls(default_conn);
}
struct k_work_delayable hfp_send_iia_work;
static void hfp_send_iia_work_handle(struct k_work *work)
{
    //   bt_hfp_hf_send_iia(default_conn, "1,0,1");
}
struct k_work_delayable hfp_openaudio_call_work;
static void hfp_openaudio_call_work_handle(struct k_work *work)
{
    bt_hfp_ag_call_status_pl(hfp_ag_handle, hfp_ag_call_call_incoming);
}
struct k_work_delayable hfp_send_bcc_work;
static void hfp_send_bcc_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
         if (codecs_negotiate_done == 0)
         {
              bt_hfp_ag_codec_selector(hfp_ag_handle, 1);
         }
         k_work_init_delayable(&hfp_openaudio_call_work, hfp_openaudio_call_work_handle);
         k_work_schedule(&hfp_openaudio_call_work, K_MSEC(300));

        s_hfp_in_calling_status = 3;
    }
    else
    {
        bt_hfp_hf_trigger_codec_connection(default_conn);
    }
}
struct k_work_delayable hfp_send_bcc_wbs_work;
static void hfp_send_bcc_wbs_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        if (codecs_negotiate_done == 0)
         {
              bt_hfp_ag_codec_selector(hfp_ag_handle, 2 );
         }
         k_work_init_delayable(&hfp_openaudio_call_work, hfp_openaudio_call_work_handle);
         k_work_schedule(&hfp_openaudio_call_work, K_MSEC(300));

        s_hfp_in_calling_status = 3;
    }
    else
    {
        bt_hfp_hf_trigger_codec_connection(default_conn);
    }
}
struct k_work_delayable hfp_dis_contorl_channel_work;

static void hfp_dis_contorl_channel_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_send_service_indicator(hfp_ag_handle, 0);
    }
    else
    {
    }
}

struct k_work_delayable hfp_enable_clip_work;
static void hfp_enable_clip_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
    }
    else
    {
        bt_hfp_hf_enable_clip_notification(default_conn);
    }
}
struct k_work_delayable hfp_disable_in_band_work;
static void hfp_disable_in_band_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_set_inband_ring_tone(hfp_ag_handle, 0);
    }
    else
    {
    }
}

struct k_work_delayable hfp_rthr_call_work;
uint8_t btrh_call_active = 1;
static void hfp_rthr_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle != NULL)
    {
        if (s_rthr_test_case == 0)
        {
            bt_hfp_ag_handle_btrh(hfp_ag_handle, 0);
            bt_hfp_ag_send_call_indicator(hfp_ag_handle, 1);
            bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
            bt_hfp_ag_open_audio(hfp_ag_handle, 0);
            btrh_call_active = 1;
        }
        else if (s_rthr_test_case == 1)
        {
            bt_hfp_ag_handle_btrh(hfp_ag_handle, 1);
            bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
        }
        if (s_rthr_test_case == 2)
        {
            bt_hfp_ag_handle_btrh(hfp_ag_handle, 2);
            if (btrh_call_active == 0)
            {
                bt_hfp_ag_send_callsetup_indicator(hfp_ag_handle, 0);
            }
            else
            {
                bt_hfp_ag_send_call_indicator(hfp_ag_handle, 0);
            }
        }
    }
    else
    {
        //     bt_hfp_hf_send_resp_hold_values(default_conn, s_rthr_test_case);
    }
}

struct k_work_delayable hfp_intg_held_call;
static void hfp_intg_held_call_work_handle(struct k_work *work)
{
    if (hfp_ag_handle)
    {
        bt_hfp_ag_send_callheld_indicator(hfp_ag_handle, 0);
        two_held_calls_joined = 1;
    }

}
static uint8_t control(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
    const struct btp_hfp_control_cmd *cp = cmd;
    int err;
    hfp_ag_cind_t cind_setting;

    switch (cp->control_type)
    {
        case HFP_ENABLE_INBAND_RING:
            err = bt_hfp_ag_set_inband_ring_tone(hfp_ag_handle, 1);
            if (!err)
            {
                return BTP_STATUS_SUCCESS;
            }
            break;
        case HFP_DISABLE_CALL:
            err = bt_hfp_hf_send_cmd(default_conn, BT_HFP_HF_AT_CHUP);
            if (!err)
            {
                return BTP_STATUS_SUCCESS;
            }
            break;
        case HFP_MUTE_INBAND_RING:
            mute_inband_ringtone = true;
            return BTP_STATUS_SUCCESS;
        case HFP_AG_ANSWER_CALL:
            k_work_init_delayable(&ag_answer_call_work, ag_answer_call_work_handle);
            k_work_schedule(&ag_answer_call_work, K_MSEC(4000));
            return BTP_STATUS_SUCCESS;
        case HFP_OUT_CALL:
            k_work_init_delayable(&hfp_out_call_work, hfp_out_call_work_handle);
            k_work_schedule(&hfp_out_call_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_CLS_MEM_CALL_LIST:
            mem_call_list = 1;
            return BTP_STATUS_SUCCESS;
        case HFP_OUT_MEM_CALL:
            mem_call_out_of_range = 0;
            k_work_init_delayable(&hfp_out_mem_call_work, hfp_out_mem_call_work_handle);
            k_work_schedule(&hfp_out_mem_call_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_OUT_MEM_OUTOFRANGE_CALL:
            mem_call_out_of_range = 1;
            k_work_init_delayable(&hfp_out_mem_call_work, hfp_out_mem_call_work_handle);
            k_work_schedule(&hfp_out_mem_call_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_OUT_LAST_CALL:
            mem_call_out_of_range = 0;
            k_work_init_delayable(&hfp_out_last_call_work, hfp_out_last_call_work_handle);
            k_work_schedule(&hfp_out_last_call_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_REJECT_CALL:
            k_work_init_delayable(&hfp_reject_call_work, hfp_reject_call_work_handle);
            k_work_schedule(&hfp_reject_call_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;

        case HFP_TWC_CALL:
            k_work_init_delayable(&hfp_twc_call_work, hfp_twc_call_work_handle);
            k_work_schedule(&hfp_twc_call_work, K_MSEC(11000));
            return BTP_STATUS_SUCCESS;
        case HFP_END_SECOND_CALL:
            s_twc_test_case = 0;
            k_work_init_delayable(&hfp_twc_test_call_work, hfp_twc_test_call_work_handle);
            k_work_schedule(&hfp_twc_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_DISABLE_ACTIVE_CALL:
            s_twc_test_case = 1;
            k_work_init_delayable(&hfp_twc_test_call_work, hfp_twc_test_call_work_handle);
            k_work_schedule(&hfp_twc_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_HELD_ACTIVE_CALL:
            s_twc_test_case = 2;
            k_work_init_delayable(&hfp_twc_test_call_work, hfp_twc_test_call_work_handle);
            k_work_schedule(&hfp_twc_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_JOIN_CONVERSATION_CALL:
            s_twc_test_case = 3;
            k_work_init_delayable(&hfp_twc_test_call_work, hfp_twc_test_call_work_handle);
            k_work_schedule(&hfp_twc_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_EXPLICIT_TRANSFER_CALL:
            s_twc_test_case = 4;
            k_work_init_delayable(&hfp_twc_test_call_work, hfp_twc_test_call_work_handle);
            k_work_schedule(&hfp_twc_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_EC_NR_DISABLE:
            //    bt_hfp_hf_control_ec_nr(default_conn, 0);
            return BTP_STATUS_SUCCESS;
        case HFP_ENABLE_BINP:
            //        bt_hfp_hf_control_voice_tag(default_conn, 1);
            return BTP_STATUS_SUCCESS;
        case HFP_ENABLE_SUB_NUMBER:
            //     bt_hfp_hf_request_subscriber_num(default_conn);
            return BTP_STATUS_SUCCESS;
        case HFP_ENABLE_VR:

            s_vr_test_case = 1;
            k_work_init_delayable(&hfp_vr_test_call_work, hfp_vr_test_call_work_handle);
            k_work_schedule(&hfp_vr_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_DISABLE_VR:

            s_vr_test_case = 0;
            k_work_init_delayable(&hfp_vr_test_call_work, hfp_vr_test_call_work_handle);
            k_work_schedule(&hfp_vr_test_call_work, K_MSEC(3000));
            return BTP_STATUS_SUCCESS;
        case HFP_QUERY_LIST_CALL:

            k_work_init_delayable(&hfp_query_list_call_work, hfp_query_list_call_work_handle);
            k_work_schedule(&hfp_query_list_call_work, K_MSEC(10000));
            return BTP_STATUS_SUCCESS;
        case HFP_REJECT_HELD_CALL:
            s_rthr_test_case = 2;
            k_work_init_delayable(&hfp_rthr_call_work, hfp_rthr_call_work_handle);
            k_work_schedule(&hfp_rthr_call_work, K_MSEC(5000));
            return BTP_STATUS_SUCCESS;

        case HFP_ACCEPT_HELD_CALL:
            s_rthr_test_case = 0;
            k_work_init_delayable(&hfp_rthr_call_work, hfp_rthr_call_work_handle);
            k_work_schedule(&hfp_rthr_call_work, K_MSEC(2000));
            return BTP_STATUS_SUCCESS;
        case HFP_ACCEPT_INCOMING_HELD_CALL:
            s_rthr_test_case = 1;
            k_work_init_delayable(&hfp_rthr_call_work, hfp_rthr_call_work_handle);
            k_work_schedule(&hfp_rthr_call_work, K_MSEC(2000));
            return BTP_STATUS_SUCCESS;
        case HFP_SEND_IIA:
            k_work_init_delayable(&hfp_send_iia_work, hfp_send_iia_work_handle);
            k_work_schedule(&hfp_send_iia_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_SEND_BCC:
            k_work_init_delayable(&hfp_send_bcc_work, hfp_send_bcc_work_handle);
            k_work_schedule(&hfp_send_bcc_work, K_MSEC(100));
            return BTP_STATUS_SUCCESS;
        case HFP_SEND_BCC_WBS:
            k_work_init_delayable(&hfp_send_bcc_wbs_work, hfp_send_bcc_wbs_work_handle);
            k_work_schedule(&hfp_send_bcc_wbs_work, K_MSEC(100));
            return BTP_STATUS_SUCCESS;
        case HFP_DIS_CTR_CHN:
            k_work_init_delayable(&hfp_dis_contorl_channel_work, hfp_dis_contorl_channel_work_handle);
            k_work_schedule(&hfp_dis_contorl_channel_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_IMPAIR_SIGNAL:
            bt_hfp_ag_get_cind_setting(hfp_ag_handle, &cind_setting);
            cind_setting.server = 0;
            cind_setting.signal--;
            bt_hfp_ag_set_cind_setting(hfp_ag_handle, &cind_setting);
            bt_hfp_ag_send_signal_indicator(hfp_ag_handle, cind_setting.signal);
            return BTP_STATUS_SUCCESS;
        case HFP_ENABLE_CLIP:
            k_work_init_delayable(&hfp_enable_clip_work, hfp_enable_clip_work_handle);
            k_work_schedule(&hfp_enable_clip_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_DISABLE_IN_BAND:
            k_work_init_delayable(&hfp_disable_in_band_work, hfp_disable_in_band_work_handle);
            k_work_schedule(&hfp_disable_in_band_work, K_MSEC(1000));
            return BTP_STATUS_SUCCESS;
        case HFP_DISCOVER_START:
            app_discover();
            return BTP_STATUS_SUCCESS;
        case HFP_INTG_HELD_CALL:
            k_work_init_delayable(&hfp_intg_held_call, hfp_intg_held_call_work_handle);
            k_work_schedule(&hfp_intg_held_call, K_MSEC(100));
            return BTP_STATUS_SUCCESS;
        default:
            break;
    }
    return BTP_STATUS_FAILED;
}

static const struct btp_handler handlers[] = {
    {
        .opcode     = BTP_HFP_READ_SUPPORTED_COMMANDS,
        .index      = BTP_INDEX_NONE,
        .expect_len = 0,
        .func       = supported_commands,
    },
    {
        .opcode     = BTP_HFP_MAKE_DISCOVERABLE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_discoverable,
    },
    {
        .opcode     = BTP_HFP_AG_MAKE_DISCOVERABLE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_ag_discoverable,
    },
    {
        .opcode     = BTP_HFP_HF_ANSWER_CALL,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = hf_answer_call,
    },
    {
        .opcode     = BTP_HFP_ENABLE_SLC,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = enable_slc,
    },
    {
        .opcode     = BTP_HFP_DISABLE_SLC,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = disable_slc,
    },
    {
        .opcode     = BTP_HFP_DISABLE_NETWORK,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = disable_network,
    },
    {
        .opcode     = BTP_HFP_ENABLE_NETWORK,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = enable_network,
    },
    {
        .opcode     = BTP_HFP_SIGNAL_STRENGTH_VERIFY,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = signal_strength_verify,
    },
    {
        .opcode     = BTP_HFP_SIGNAL_STRENGTH_SEND,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = signal_strength_send,
    },
    {
        .opcode     = BTP_HFP_VERIFY_ROAM_ACTIVE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_roam_active,
    },
    {
        .opcode     = BTP_HFP_VERIFY_ROAM_INACTIVE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_roam_inactive,
    },
    {
        .opcode     = BTP_HFP_MAKE_ROAM_ACTIVE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_roam_active,
    },
    {
        .opcode     = BTP_HFP_MAKE_ROAM_INACTIVE,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_roam_inactive,
    },
    {
        .opcode     = BTP_HFP_VERIFY_BATTERY_DISCHARGED,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_battery_discharged,
    },
    {
        .opcode     = BTP_HFP_VERIFY_BATTERY_CHARGED,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_battery_charged,
    },
    {
        .opcode     = BTP_HFP_MAKE_BATTERY_DISCHARGED,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_battery_discharged,
    },
    {
        .opcode     = BTP_HFP_MAKE_BATTERY_CHARGED,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_battery_charged,
    },
    {
        .opcode     = BTP_HFP_QUERY_NETWORK_OPERATOR,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = query_network_operator,
    },
    {
        .opcode     = BTP_HFP_VERIFY_NETWORK_OPERATOR,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_network_operator,
    },
    {
        .opcode     = BTP_HFP_VERIFY_VOICE_TAG,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify_voice_tag,
    },

    {
        .opcode     = BTP_HFP_MAKE_SURE_AG_REGISTERED_ON_HOME_NETWORK,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = make_sure_ag_registered_on_home_network,
    },
    {
        .opcode     = BTP_HFP_ENABLE_AUDIO,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = enable_audio,
    },
    {
        .opcode     = BTP_HFP_DISABLE_AUDIO,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = disable_audio,
    },
    {
        .opcode     = BTP_HFP_AG_ENABLE_CALL,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = ag_enable_call,
    },
    {
        .opcode     = BTP_HFP_AG_DISABLE_CALL_EXTERNAL,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = ag_disable_call_external,
    },
    {
        .opcode     = BTP_HFP_VERIFY,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = verify,
    },
    {
        .opcode     = BTP_HFP_CONTROL,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = control,
    },
    {
        .opcode     = BTP_HFP_SPERKER_MIC_VOLUME_SEND,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = speaker_mic_volume_send,
    },
    {
        .opcode     = BTP_HFP_DTMF_CODE_SEND,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = dtmf_code_send,
    },

    {
        .opcode     = BTP_HFP_SPERKER_MIC_VOLUME_VERIFY,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = speaker_mic_volume_verify,
    },
    {
        .opcode     = BTP_HFP_DTMF_CODE_VERIFY,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = dtmf_code_verify,
    },
    {
        .opcode     = BTP_HFP_REGISTER_CONNECT_CB,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func       = register_connect_cb,
    },
};

uint8_t tester_init_hfp(void)
{
    tester_register_command_handlers(BTP_SERVICE_ID_HFP, handlers, ARRAY_SIZE(handlers));
    hf_accept_call           = false;
    hf_check_signal_strength = 0xFFu;

    return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_hfp(void)
{
    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_HFP_HF && CONFIG_BT_HFP_AG */
#endif /* CONFIG_BT_BREDR */
