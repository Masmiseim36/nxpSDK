/* btp_hfp.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/* HFP Service */
/* commands */
#define BTP_HFP_READ_SUPPORTED_COMMANDS 0x01

struct btp_hfp_read_supported_commands_rp
{
    uint8_t data[1];
} __packed;

#define BTP_HFP_MAKE_DISCOVERABLE    0x02
#define BTP_HFP_AG_MAKE_DISCOVERABLE 0xf2
#define BTP_HFP_HF_ANSWER_CALL       0x03
#define BTP_HFP_ENABLE_SLC           0x04

struct btp_hfp_enable_slc_cmd
{
    bt_addr_t address;
    uint8_t is_ag;
} __packed;
#define BTP_HFP_DISABLE_SLC 0x05

struct btp_hfp_disable_slc_cmd
{
    bt_addr_t address;
} __packed;

#define BTP_HFP_DISABLE_NETWORK 0x06

struct btp_hfp_disable_network_cmd
{
    bt_addr_t address;
} __packed;

#define BTP_HFP_ENABLE_NETWORK 0x07

struct btp_hfp_enable_network_cmd
{
    bt_addr_t address;

} __packed;

#define BTP_HFP_SIGNAL_STRENGTH_VERIFY 0x08

struct btp_hfp_signal_strength_verify_cmd
{
    uint8_t signal_strength;
} __packed;

#define BTP_HFP_SIGNAL_STRENGTH_SEND 0x09

struct btp_hfp_signal_strength_send_cmd
{
    uint8_t signal_strength;
} __packed;

#define BTP_HFP_SPERKER_MIC_VOLUME_SEND 0x1B

struct btp_hfp_speaker_mic_volume_send_cmd
{
    uint8_t speaker_mic;
    uint8_t speaker_mic_volume;
} __packed;

#define BTP_HFP_SPERKER_MIC_VOLUME_VERIFY 0x1C

struct btp_hfp_speaker_mic_volume_verify_cmd
{
    uint8_t speaker_mic;
    uint8_t speaker_mic_volume;
} __packed;
#define BTP_HFP_DTMF_CODE_VERIFY 0x1D

struct btp_hfp_dtmf_code_verify_cmd
{
    uint8_t dtmf_code;
} __packed;

#define BTP_HFP_DTMF_CODE_SEND 0x1E

struct btp_hfp_dtmf_code_send_cmd
{
    uint8_t dtmf_code;
} __packed;
#define BTP_HFP_VERIFY_ROAM_ACTIVE                      0x0A
#define BTP_HFP_VERIFY_ROAM_INACTIVE                    0x0B
#define BTP_HFP_MAKE_ROAM_ACTIVE                        0x0C
#define BTP_HFP_MAKE_ROAM_INACTIVE                      0x0D
#define BTP_HFP_VERIFY_BATTERY_DISCHARGED               0x0E
#define BTP_HFP_VERIFY_BATTERY_CHARGED                  0x0F
#define BTP_HFP_MAKE_BATTERY_CHARGED                    0x10
#define BTP_HFP_MAKE_BATTERY_DISCHARGED                 0x11
#define BTP_HFP_QUERY_NETWORK_OPERATOR                  0x12
#define BTP_HFP_VERIFY_NETWORK_OPERATOR                 0x13
#define BTP_HFP_MAKE_SURE_AG_REGISTERED_ON_HOME_NETWORK 0x14
#define BTP_HFP_ENABLE_AUDIO                            0x15
#define BTP_HFP_DISABLE_AUDIO                           0x16
#define BTP_HFP_AG_ENABLE_CALL                          0x17
#define BTP_HFP_AG_DISABLE_CALL_EXTERNAL                0x18
#define BTP_HFP_VERIFY                                  0x19
#define BTP_HFP_VERIFY_VOICE_TAG                        0x1f
#define BTP_HFP_REGISTER_CONNECT_CB                     0x20
struct btp_hfp_verify_cmd
{
    uint8_t verify_type;
} __packed;

enum btp_hfp_verify_type
{
    HFP_VERIFY_INBAND_RING        = 0x01,
    HFP_VERIFY_INBAND_RING_MUTING = 0x02,
    HFP_VERIFY_IUT_ALERTING       = 0x03,
    HFP_VERIFY_IUT_NOT_ALERTING   = 0x04,
    HFP_VERIFY_EC_NR_DISABLED     = 0x05,
};

#define BTP_HFP_CONTROL 0x1A

struct btp_hfp_control_cmd
{
    uint8_t control_type;
} __packed;

enum btp_hfp_control_type
{
    HFP_ENABLE_INBAND_RING        = 0x01,
    HFP_DISABLE_CALL              = 0x02,
    HFP_MUTE_INBAND_RING          = 0x03,
    HFP_AG_ANSWER_CALL            = 0x04,
    HFP_REJECT_CALL               = 0x05,
    HFP_OUT_CALL                  = 0x06,
    HFP_CLS_MEM_CALL_LIST         = 0x07,
    HFP_OUT_MEM_CALL              = 0x08,
    HFP_OUT_MEM_OUTOFRANGE_CALL   = 0x9,
    HFP_OUT_LAST_CALL             = 0xa,
    HFP_TWC_CALL                  = 0xb,
    HFP_END_SECOND_CALL           = 0xc,
    HFP_DISABLE_ACTIVE_CALL       = 0xd,
    HFP_HELD_ACTIVE_CALL          = 0xe,
    HFP_EC_NR_DISABLE             = 0xf,
    HFP_ENABLE_BINP               = 0x10,
    HFP_ENABLE_SUB_NUMBER         = 0x11,
    HFP_ENABLE_VR                 = 0x12,
    HFP_DISABLE_VR                = 0x13,
    HFP_QUERY_LIST_CALL           = 0x14,
    HFP_REJECT_HELD_CALL          = 0x15,
    HFP_ACCEPT_HELD_CALL          = 0x16,
    HFP_ACCEPT_INCOMING_HELD_CALL = 0x17,
    HFP_SEND_IIA                  = 0x18,
    HFP_SEND_BCC                  = 0x19,
    HFP_DIS_CTR_CHN               = 0x1a,
    HFP_IMPAIR_SIGNAL             = 0x1b,
    HFP_JOIN_CONVERSATION_CALL    = 0x1c,
    HFP_EXPLICIT_TRANSFER_CALL    = 0x1d,
    HFP_ENABLE_CLIP               = 0x1e,
    HFP_DISABLE_IN_BAND           = 0x1f,
    HFP_DISCOVER_START            = 0x20,
    HFP_INTG_HELD_CALL            = 0x21,
    HFP_SEND_BCC_WBS              = 0x22,
};

#define BTP_HFP_EV_SCO_CONNECTED    0x80
#define BTP_HFP_EV_SCO_DISCONNECTED 0x81

#ifdef __cplusplus
}
#endif