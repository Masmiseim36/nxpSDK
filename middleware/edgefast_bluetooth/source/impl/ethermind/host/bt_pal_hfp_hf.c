/* hfp_hf.c - Hands free Profile - Handsfree side handling */

/*
 * Copyright (C) 2021. NXP Ltd.
 * Copyright (c) 2015-2016 Intel Corporation
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
#include <bluetooth/hfp_hf.h>
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
#include "BT_hfp_unit_api.h"
#include "BT_at_parser_api.h"
#include "db_gen.h"
#include "sco_audio_pl.h"
#include "eBT_os.h"

#if (defined (CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0U))

#define LOG_ENABLE      IS_ENABLED(CONFIG_BT_DEBUG_HFP_HF)
#define LOG_MODULE_NAME bt_hfp_hf
#include "fsl_component_log.h"

LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

struct bt_hfp_hf_em
{
    uint8_t allocated;
    uint8_t actived;
    uint8_t serverChannel;
    uint8_t bt_hfp_hp_hfu_slc;
    uint16_t handle;
    uint8_t peerAddr[BT_BD_ADDR_SIZE];
    uint16_t sco_connection_handle;
    struct bt_conn *bt_so_conn;
    struct bt_conn *bt_conn;
    struct bt_hfp_hf_cb *bt_hf_cb;
    uint8_t bt_hfp_hp_speaker_volume[3];
    uint8_t bt_hfp_hp_microphone_gain[3];
    uint32_t hf_features;
    uint32_t ag_features;
    int8_t ind_table[HF_MAX_AG_INDICATORS];
};
static OSA_MUTEX_HANDLE_DEFINE(s_HfpHfLockMutex);
static osa_mutex_handle_t s_HfpHfLock;
static struct bt_hfp_hf_em s_HfpHfInstances[HFP_UNIT_MAX_CONNECTIONS];
#define EDGEFAST_HFP_HF_LOCK   OSA_MutexLock(s_HfpHfLock, osaWaitForever_c)
#define EDGEFAST_HFP_HF_UNLOCK OSA_MutexUnlock(s_HfpHfLock)

/* ----------------------------------------- Structures/ Data Types */
/* Data Structures to display received responses from AG */
typedef struct
{
    uint8_t number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    uint8_t type;
    uint8_t voice_class;
    uint8_t alpha[HFP_UNIT_MAX_OPERATOR_NAME_LEN];

} bt_hfp_hf_parser_ccwa_result;

typedef struct
{
    uint8_t mode;
    uint8_t format;
    uint8_t op_name[HFP_UNIT_MAX_OPERATOR_NAME_LEN];
} bt_hfp_hf_parser_cops_result;

typedef struct
{
    uint8_t number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    uint8_t type;
    uint8_t service;
} bt_hfp_hf_parser_cnum_result;

typedef struct
{
    uint8_t id;
    uint8_t dir;
    uint8_t status;
    uint8_t mode;
    uint8_t mprty;
    uint8_t number[HFP_UNIT_DIGIT_ARRAY_SIZE];
    uint8_t type;
    uint8_t alpha[HFP_UNIT_MAX_OPERATOR_NAME_LEN];
    uint8_t dummy;
} bt_hfp_hf_parser_clcc_result;

typedef struct
{
    uint8_t digits[HFP_UNIT_DIGIT_ARRAY_SIZE];
    uint8_t name[HFP_UNIT_DIGIT_ARRAY_SIZE];

} bt_hfp_hf_parser_cli_result;

typedef struct _bt_hfp_hf_parser_bvra_text
{
    uint32_t text_id;
    uint8_t text_type;
    uint8_t text_op;
    uint8_t str[HFP_UNIT_MAX_ENH_VREC_TEXT_LEN];
} bt_hfp_hf_parser_bvra_text;

typedef struct
{
    uint8_t vrect;
    uint8_t vrecstate;
    bt_hfp_hf_parser_bvra_text text_rep;
} bt_hfp_hf_parser_bvra_result;

typedef uint8_t bt_hfp_hf_parser_token_type;

typedef union
{
    bt_hfp_hf_parser_ccwa_result ccwa_result;
    bt_hfp_hf_parser_cops_result cops_resp_result;
    bt_hfp_hf_parser_cnum_result cnum_resp_result;
    bt_hfp_hf_parser_clcc_result clcc_resp_result;
    bt_hfp_hf_parser_cli_result cli_info;
    bt_hfp_hf_parser_bvra_result bvra_result;
    uint8_t uchar_result;
    uint8_t digits[HFP_UNIT_DIGIT_ARRAY_SIZE];
} bt_hfp_hf_parser_response;

typedef struct
{
    bt_hfp_hf_parser_token_type token;
    bt_hfp_hf_parser_response result_param;

} bt_hfp_hf_parser_result;

#define BT_HFP_HF_INITIAL_GAIN "12"

#define BT_HFP_HF_MAX_CLCC_PARAMS 8
#define BT_HFP_HF_MAX_CCWA_PARAMS 4
#define BT_HFP_HF_MAX_COPS_PARAMS 3
#define BT_HFP_HF_INVALID_VALUE   0xFF

/* HFP v1.8 realated */
#define BT_HFP_HF_BRSF_CMD_FEATURE_BITMASK_ENH_VR_STATUS 0x0400
#define BT_HFP_HF_BRSF_CMD_FEATURE_BITMASK_VR_TEXT       0x0800

#define BT_HFP_HF_BRSF_RSP_FEATURE_BITMASK_ENH_VR_STATUS 0x1000
#define BT_HFP_HF_BRSF_RSP_FEATURE_BITMASK_VR_TEXT       0x2000

#define BT_HFU_MAX_DIGITS_CODEC_ID (3u)

#define BT_HFU_MAX_AVL_CODEC_ID_LIST_LEN (HFP_UNIT_MAX_NUM_AVL_CODECS * (BT_HFU_MAX_DIGITS_CODEC_ID + 1))

#define BT_HFP_HF_MAX_DIGITS_HF_INDICATOR_ID (5u)

#define BT_HFP_HF_HF_INDICATOR_LIST_LEN (HFP_UNIT_MAX_HF_INDICATOR_COUNT * (BT_HFP_HF_MAX_DIGITS_HF_INDICATOR_ID + 1))

#define BT_HFP_WIDEBAND_VOICE_SETTING_DEFAULT                                                               \
    (LMP_VOICE_INPUT_CODING_LINEAR | LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT | LMP_VOICE_INPUT_SAMPLE_16_BIT | \
     LMP_VOICE_LINEAR_BIT_POS_0 | LMP_VOICE_AIR_CODING_TRANSPARENT)

static uint8_t send_string[HFP_UNIT_MAX_COMMAND_LENGTH];
static uint8_t send_string_length;

/* HFP_UNIT Application Configuration parameters */
static HFP_UNIT_APPL_CONFIG_PARAMS bt_hfp_hf_conf_params;

/* HFP_UNIT local supported features */
static uint8_t bt_hfp_hf_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN];

/* HFP_UNIT local supported features in numeric format */
static uint32_t bt_hfp_hf_local_supported_features_ext;

/* HFP_UNIT peer supported features in numeric format */
static uint32_t bt_hfp_hf_peer_supported_features_ext;

#ifdef HFP_UNIT_1_6
/* HFP_UNIT local list of available codecs */
static uint8_t bt_hfp_hf_local_available_codecs[BT_HFU_MAX_AVL_CODEC_ID_LIST_LEN + 1];
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
/* HFP_UNIT local list of supported HF indicators */
static uint8_t bt_hfp_hf_local_supported_hf_indicators[BT_HFP_HF_HF_INDICATOR_LIST_LEN + 1];

/* Peer supported HF indicators */
static uint8_t bt_hfp_hf_peer_hf_ind_count;
static HFP_UNIT_BIND_READ_RESULT bt_bt_hfp_hf_peer_hf_ind_list_info[HFP_UNIT_MAX_HF_INDICATOR_COUNT];
static HFP_UNIT_BIND_READ_RESULT *bt_bt_hfp_hf_peer_hf_ind_read_info;

#endif /* HFP_UNIT_1_7 */

/* HFP_UNIT SDP record handle */
static uint32_t hfp_hfu_record_handle;

/* eSCO-CVSD S1-S4 settings */
static HCI_SCO_IN_PARAMS bt_hfp_esco_cvsd_params[] = {
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
static HCI_SCO_IN_PARAMS bt_hfp_esco_msbc_params[] = {
    /* Default */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, LMP_MAX_LATENCY_DONT_CARE,
     BT_HFP_WIDEBAND_VOICE_SETTING_DEFAULT, LMP_ESCO_PACKET_TYPE_DEFAULT, LMP_RETX_EFFORT_DONT_CARE},

    /* T1 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x0008, BT_HFP_WIDEBAND_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_EV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_QUALITY},

    /* T2 */
    {/* For Narrow Band Speech */
     LMP_ESCO_TX_BANDWIDTH_DEFAULT, LMP_ESCO_RX_BANDWIDTH_DEFAULT, 0x000D, BT_HFP_WIDEBAND_VOICE_SETTING_DEFAULT,
     (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5), LMP_RETX_EFFORT_QUALITY}};

/* HFP eSCO Channel Default parameters */
static HCI_SCO_IN_PARAMS *bt_hfp_esco_params[2] = {
    /* Default Narrowband (CVSD) parameter */
    &bt_hfp_esco_cvsd_params[0],

    /* Default Wideband (mSBC) parameter */
    &bt_hfp_esco_msbc_params[0]};

static struct bt_hfp_hf_em *hfp_hf_GetInstance(void)
{
    EDGEFAST_HFP_HF_LOCK;
    for (uint8_t index = 0; index < HFP_UNIT_MAX_CONNECTIONS; ++index)
    {
        if (s_HfpHfInstances[index].allocated == 0U)
        {
            memset(&s_HfpHfInstances[index], 0U, sizeof(s_HfpHfInstances[index]));
            s_HfpHfInstances[index].allocated = 1U;
            EDGEFAST_HFP_HF_UNLOCK;
            return &s_HfpHfInstances[index];
        }
    }

    EDGEFAST_HFP_HF_UNLOCK;
    return NULL;
}
static void hfp_hf_FreeInstance(struct bt_hfp_hf_em *hfp_hf)
{
    if (NULL != hfp_hf)
    {
        EDGEFAST_HFP_HF_LOCK;
        hfp_hf->allocated = 0U;
        EDGEFAST_HFP_HF_UNLOCK;
    }
}
static void hfp_hf_DeActiveInstance(struct bt_hfp_hf_em *hfp_hf)
{
    if (NULL != hfp_hf)
    {
        EDGEFAST_HFP_HF_LOCK;
        hfp_hf->actived = 0U;
        EDGEFAST_HFP_HF_UNLOCK;
    }
}
static struct bt_hfp_hf_em *hfp_hf_GetNoneActiveInstance(void)
{
    EDGEFAST_HFP_HF_LOCK;
    for (uint8_t index = 0; index < HFP_UNIT_MAX_CONNECTIONS; ++index)
    {
        if ((s_HfpHfInstances[index].allocated == 1U) && (s_HfpHfInstances[index].actived == 0U))
        {
            EDGEFAST_HFP_HF_UNLOCK;
            return &s_HfpHfInstances[index];
        }
    }

    EDGEFAST_HFP_HF_UNLOCK;
    return NULL;
}

static struct bt_hfp_hf_em *bt_hfp_hf_lookup_bt_handle(uint16_t handle)
{
    EDGEFAST_HFP_HF_LOCK;
    for (uint8_t index = 0; index < HFP_UNIT_MAX_CONNECTIONS; ++index)
    {
        if ((s_HfpHfInstances[index].allocated == 1U) && (s_HfpHfInstances[index].actived == 1U))
        {
            if (handle == s_HfpHfInstances[index].handle)
            {
                EDGEFAST_HFP_HF_UNLOCK;
                return &s_HfpHfInstances[index];
            }
        }
    }

    EDGEFAST_HFP_HF_UNLOCK;
    return NULL;
}
static struct bt_hfp_hf_em *bt_hfp_hf_lookup_bt_addr(uint8_t *peerAddr)
{
    EDGEFAST_HFP_HF_LOCK;
    for (uint8_t index = 0; index < HFP_UNIT_MAX_CONNECTIONS; ++index)
    {
        if ((s_HfpHfInstances[index].allocated == 1U) && (s_HfpHfInstances[index].actived == 1U))
        {
            if (0 == BT_mem_cmp(peerAddr, s_HfpHfInstances[index].peerAddr, BT_BD_ADDR_SIZE))
            {
                EDGEFAST_HFP_HF_UNLOCK;
                return &s_HfpHfInstances[index];
            }
        }
    }

    EDGEFAST_HFP_HF_UNLOCK;
    return NULL;
}

static struct bt_hfp_hf_em *bt_hfp_hf_lookup_bt_conn(struct bt_conn *conn)
{
    EDGEFAST_HFP_HF_LOCK;
    for (uint8_t index = 0; index < HFP_UNIT_MAX_CONNECTIONS; ++index)
    {
        if ((s_HfpHfInstances[index].allocated == 1U) && (s_HfpHfInstances[index].actived == 1U))
        {
            if (conn == s_HfpHfInstances[index].bt_conn)
            {
                EDGEFAST_HFP_HF_UNLOCK;
                return &s_HfpHfInstances[index];
            }
        }
    }

    EDGEFAST_HFP_HF_UNLOCK;
    return NULL;
}

static uint32_t appl_str_to_num_in_hex_format(uint8_t *str, uint16_t len)
{
    uint32_t num = 0;
    uint8_t index;
    uint8_t val;

    if (len > 8)
    {
        BT_DBG("Invalid string length for str to hex num conversion \n");
        return 0;
    }

    val = 0;
    for (index = 0; index < len; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            val = (str[index] - '0');
        }
        else if ((str[index] == 'A') || (str[index] == 'a'))
        {
            val = 0xA;
        }
        else if ((str[index] == 'B') || (str[index] == 'b'))
        {
            val = 0xB;
        }
        else if ((str[index] == 'C') || (str[index] == 'c'))
        {
            val = 0xC;
        }
        else if ((str[index] == 'D') || (str[index] == 'd'))
        {
            val = 0xD;
        }
        else if ((str[index] == 'E') || (str[index] == 'e'))
        {
            val = 0xE;
        }
        else if ((str[index] == 'F') || (str[index] == 'f'))
        {
            val = 0xF;
        }
        else
        {
            /* Should not reach here */
            BT_ERR("Invalid char in string for str to hex num conversion \n");
        }

        num = (num << 4) | val;
    }

    return num;
}

static API_RESULT bt_hfp_hf_set_esco_channel_parameters(uint8_t set_sco_param, HCI_SCO_IN_PARAMS *sco_params)
{
    uint8_t enable;
    HCI_SCO_IN_PARAMS esco_params;

    memset(&esco_params, 0x0, sizeof(HCI_SCO_IN_PARAMS));
    /* Null Check for eSCO parameters */
    if ((BT_TRUE == set_sco_param) && (NULL == sco_params))
    {
        BT_ERR("Unable to set NULL as eSCO parameters\n");
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

static API_RESULT bt_hfp_hp_extract_result(AT_PARSER_RESPONSE *parser_response,
                                           uint8_t *buffer,
                                           bt_hfp_hf_parser_result *parse_result)
{
    uint8_t index;
    uint32_t temp_val;
    API_RESULT retval;

    index               = 0;
    parse_result->token = parser_response->keyword_type;
    retval              = API_SUCCESS;

    switch (parser_response->keyword_type)
    {
        case AT_PL_CLIP_CL_: /* <CRLF>+CLIP: */
        {
            BT_mem_copy(parse_result->result_param.cli_info.digits,
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length);
            parse_result->result_param.cli_info.digits[parser_response->param[index++].value_length] = '\0';
            /* Name from PB */
            if (5 == parser_response->number_of_params)
            {
                /* Store Name */
                index = 4;
                BT_mem_copy(parse_result->result_param.cli_info.name,
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length);
                parse_result->result_param.cli_info.name[parser_response->param[index++].value_length] = '\0';
            }
            else
            {
                parse_result->result_param.cli_info.name[0] = '\0';
            }
        }
        break;

        case AT_PL_CCWA_CL_: /* <CRLF>+CCWA: */
        {
            /* +CCWA: <number>, <type>, <class>[, <alpha>] */

            BT_mem_copy(parse_result->result_param.ccwa_result.number,
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length);
            parse_result->result_param.ccwa_result.number[parser_response->param[index++].value_length] = '\0';

            if (parser_response->number_of_params == index)
            {
                break;
            }

            parse_result->result_param.ccwa_result.type =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            if (parser_response->number_of_params == index)
            {
                parse_result->result_param.ccwa_result.voice_class = 255;
                parse_result->result_param.ccwa_result.alpha[0]    = '\0';
                break;
            }

            parse_result->result_param.ccwa_result.voice_class =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            /* Optional Param */
            if (BT_HFP_HF_MAX_CCWA_PARAMS == parser_response->number_of_params)
            {
                BT_mem_copy(parse_result->result_param.ccwa_result.alpha,
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length);
                parse_result->result_param.ccwa_result.alpha[parser_response->param[index++].value_length] = '\0';
            }
            else
            {
                parse_result->result_param.ccwa_result.alpha[0] = '\0';
            }
        }
        break;

        case AT_PL_BINP_CL_: /* <CRLF>+BINP: */

            BT_mem_copy(parse_result->result_param.digits, &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length);
            parse_result->result_param.digits[parser_response->param[index++].value_length] = '\0';

            break;

        case AT_PL_COPS_CL_: /* <CRLF>+COPS: */
        {
            /* +COPS:<mode>,[0,<operator>] */
            parse_result->result_param.cops_resp_result.mode =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            /* Optional Param */
            if (BT_HFP_HF_MAX_COPS_PARAMS == parser_response->number_of_params)
            {
                parse_result->result_param.cops_resp_result.format =
                    (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

                index++;

                BT_mem_copy(parse_result->result_param.cops_resp_result.op_name,
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length);

                parse_result->result_param.cops_resp_result.op_name[parser_response->param[index++].value_length] =
                    '\0';
            }
            else
            {
                parse_result->result_param.cops_resp_result.format     = BT_HFP_HF_INVALID_VALUE;
                parse_result->result_param.cops_resp_result.op_name[0] = '\0';
            }
        }
        break;

        case AT_PL_CNUM_CL_: /* <CRLF>+CNUM: */
        {
            index = 0;
            BT_mem_copy(parse_result->result_param.cnum_resp_result.number,
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length);
            parse_result->result_param.cnum_resp_result.number[parser_response->param[index++].value_length] = '\0';

            if (parser_response->number_of_params == index)
            {
                break;
            }

            parse_result->result_param.cnum_resp_result.type =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            if (parser_response->number_of_params <= index)
            {
                break;
            }
            parse_result->result_param.cnum_resp_result.service =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;
        }
        break;

        case AT_PL_CLCC_CL_: /* <CRLF>+CLCC: */
        {
            /*
             * +CLCC:<idx>,<dir>,<status>,<mode>,
             * <mprty>,<number>,<type>[,<alpha>]
             */
            parse_result->result_param.clcc_resp_result.id =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            parse_result->result_param.clcc_resp_result.dir =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            parse_result->result_param.clcc_resp_result.status =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            parse_result->result_param.clcc_resp_result.mode =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            parse_result->result_param.clcc_resp_result.mprty =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            if (parser_response->number_of_params == index)
            {
                parse_result->result_param.clcc_resp_result.number[0] = '\0';

                parse_result->result_param.clcc_resp_result.alpha[0] = '\0';

                break;
            }

            BT_mem_copy(parse_result->result_param.clcc_resp_result.number,
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length);

            parse_result->result_param.clcc_resp_result.number[parser_response->param[index++].value_length] = '\0';

            parse_result->result_param.clcc_resp_result.type =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            /* Optional Param */
            if (BT_HFP_HF_MAX_CLCC_PARAMS == parser_response->number_of_params)
            {
                BT_mem_copy(parse_result->result_param.clcc_resp_result.alpha,
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length);
                parse_result->result_param.clcc_resp_result.alpha[parser_response->param[index++].value_length] = '\0';
            }
            else
            {
                parse_result->result_param.clcc_resp_result.alpha[0] = '\0';
            }
        }
        break;

#ifdef HFP_UNIT_1_8
        case AT_PL_BVRA_CL_: /* <CRLF>+BVRA: */ /* Fall Through */

            parse_result->result_param.bvra_result.vrect =
                (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            index++;

            if ((0 != (bt_hfp_hf_local_supported_features_ext & 0x0400)) &&
                (0 != (bt_hfp_hf_peer_supported_features_ext & 0x1000)) && (parser_response->number_of_params > index))
            {
                parse_result->result_param.bvra_result.vrecstate =
                    (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

                index++;
            }

            if ((0 != (bt_hfp_hf_local_supported_features_ext & 0x0800)) &&
                (0 != (bt_hfp_hf_peer_supported_features_ext & 0x2000)) && (parser_response->number_of_params > index))
            {
                parse_result->result_param.bvra_result.text_rep.text_id =
                    (uint16_t)appl_str_to_num_in_hex_format(&buffer[parser_response->param[index].start_of_value_index],
                                                            parser_response->param[index].value_length);

                index++;

                parse_result->result_param.bvra_result.text_rep.text_type =
                    (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

                index++;

                parse_result->result_param.bvra_result.text_rep.text_op =
                    (uint8_t)atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

                index++;

                BT_mem_copy(parse_result->result_param.bvra_result.text_rep.str,
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length);

                parse_result->result_param.bvra_result.text_rep.str[parser_response->param[index++].value_length] =
                    '\0';
            }

            break;
#endif /* HFP_UNIT_1_8 */

        case AT_PL_VGS_CL_: /* <CRLF>+VGS: */   /* Fall Through */
        case AT_PL_VGM_CL_: /* <CRLF>+VGM: */   /* Fall Through */
        case AT_PL_VGS_EQ: /* <CRLF>+VGS= */    /* Fall Through */
        case AT_PL_VGM_EQ: /* <CRLF>+VGM= */    /* Fall Through */
        case AT_PL_BTRH_CL_: /* <CRLF>+BTRH: */ /* Fall Through */
        case AT_PL_BSIR_CL_: /* <CRLF>+BSIR: */ /* Fall Through */
#ifndef HFP_UNIT_1_8
        case AT_PL_BVRA_CL_: /* <CRLF>+BVRA: */ /* Fall Through */
#endif                                          /* HFP_UNIT_1_8 */
#ifdef HFP_UNIT_1_6
        case AT_PL_BCS_CL_: /* <CRLF>+BCS: */ /* Fall Through */
#endif                                        /* HFP_UNIT_1_6 */
        case AT_PL_CME_ERROR_CL_:             /* <CRLF>+CME ERROR: */

            temp_val = atoi((char const *)&buffer[parser_response->param[index].start_of_value_index]);

            parse_result->result_param.uchar_result = (uint8_t)((temp_val > 0xFF) ? (0xFF) : (temp_val));

            index++;
            break;

#if 0
        case AT_PL_BIND_READ_ :  /* <CRLF>+BIND: <ind_id>, <ind_val>*/

            /* Indicator ID */
            parse_result->result_param.bind_info.hf_ind = atoi((char const*)&buffer[parser_response->param[index].start_of_value_index]);


            index ++;

            /* Indicator Value */
            parse_result->result_param.bind_info.hf_ind_val = atoi((char const*)&buffer[parser_response->param[index].start_of_value_index]);


        break;
#endif /* 0 */

        default:
            /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
            break;
    } /* switch (keyword type) */

    return retval;
}
/* Callback registered with HFP - profile */
static API_RESULT bt_hfp_hf_callback_registered_with_hfu(HFP_UNIT_HANDLE handle, /* Connection Instance */
                                                  HFP_UNIT_EVENTS event,  /* HFP Events          */
                                                  API_RESULT result,      /* Result of operation */
                                                  void *data,             /* Data for each event */
                                                  uint16_t data_len       /* Length of Data      */
)
{
    uint8_t *bd_addr;
    uint8_t index;
    bt_hfp_hf_parser_result app_parser_result;

    HFP_UNIT_CIND_READ_RESULT *cind_result;

    uint8_t *data_recvd;
    HFP_UNIT_APP_DATA *data_to_app;
    /* AG's BD_ADDR */
    uint8_t bt_hfp_hp_peer_bd_addr[BT_BD_ADDR_SIZE];

    // struct bt_conn *hfp_hf_bt_conn = bt_conn_lookup_handle((uint16_t)handle);
    struct bt_hfp_hf_em *hfp_hf = bt_hfp_hf_lookup_bt_handle((uint16_t)handle);

#ifdef HFP_UNIT_1_7
    API_RESULT retval;
#endif /* HFP_UNIT_1_7 */

#ifdef HFP_UNIT_1_6
    uint8_t esco_codec_id;
#endif /* HFP_UNIT_1_6 */

    data_to_app = data;

    bd_addr = NULL;

    /* Reset */
    BT_mem_set(&app_parser_result, 0, sizeof(bt_hfp_hf_parser_result));

    switch (event)
    {
        case HFP_UNIT_CLI_DIGITS:            /* Fall Through */
        case HFP_UNIT_CALL_WAITING_IND:      /* Fall Through */
        case HFP_UNIT_VOICE_RECOG_IND:       /* Fall Through */
        case HFP_UNIT_BSIR_IND:              /* Fall Through */
        case HFP_UNIT_VGM_IND:               /* Fall Through */
        case HFP_UNIT_VGS_IND:               /* Fall Through */
        case HFP_UNIT_VOICETAG_PHNUM_IND:    /* Fall Through */
        case HFP_UNIT_RECVD_BTRH_IND:        /* Fall Through */
        case HFP_UNIT_REQ_SUB_NUM_IND:       /* Fall Through */
        case HFP_UNIT_COPS_QUERY_RESULT_IND: /* Fall Through */
        case HFP_UNIT_CURRENT_CALL_LIST_IND: /* Fall Through */
        case HFP_UNIT_CMEE_IND:              /* Fall Through */
#ifdef HFP_UNIT_1_6
        case HFP_UNIT_BCS_IND: /* Fall Through */
#endif                         /* HFP_UNIT_1_6 */

            /**
             * These events require the utility function to extract data
             * from them. The rest of the functions have data in an easily
             * recognisable format. For the complete list of events and the
             * data associated with hem, please refer the HFP Unit API Document.
             */
            bt_hfp_hp_extract_result(data_to_app->parser_resp, data_to_app->buffer, &app_parser_result);

            break;

        default:
            /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
            break;
    }

    switch (event)
    {
        /*
         * Profile Level Connection and Disconnection related Events.
         */
        case HFP_UNIT_CONNECT_CNF:
            BT_DBG("\n> Event          : HFP_UNIT_CONNECT_CNF\n");
            BT_DBG("> Instance       : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result   : 0x%04X\n", result);
            if (API_SUCCESS == result)
            {
                BT_mem_copy(bt_hfp_hp_peer_bd_addr, data, BT_BD_ADDR_SIZE);
                BT_DBG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n", bt_hfp_hp_peer_bd_addr[0],
                       bt_hfp_hp_peer_bd_addr[1], bt_hfp_hp_peer_bd_addr[2], bt_hfp_hp_peer_bd_addr[3],
                       bt_hfp_hp_peer_bd_addr[4], bt_hfp_hp_peer_bd_addr[5]);

                hfp_hf         = bt_hfp_hf_lookup_bt_addr(bt_hfp_hp_peer_bd_addr);
                hfp_hf->handle = handle;
                /* Update the eSCO channel paramters for Default CVSD Codec */
                bt_hfp_hf_set_esco_channel_parameters(BT_TRUE, bt_hfp_esco_params[0]);

                /* Get peer supported features */
                BT_hfp_unit_get_peer_supported_features(handle, &bt_hfp_hf_peer_supported_features_ext);

                hfp_hf->bt_hfp_hp_hfu_slc = 1;

                BT_hfp_unit_feature_control(handle, HFP_UNIT_FEATURE_CLIP, HFP_UNIT_ACTION_ENABLE);
            }
            break;

        case HFP_UNIT_CONNECT_IND:
            BT_DBG("\n> Event          : HFP_UNIT_CONNECT_IND\n");
            BT_DBG("> Instance       : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result   : 0x%04X\n", result);
            if (API_SUCCESS == result)
            {
                BT_mem_copy(bt_hfp_hp_peer_bd_addr, data, BT_BD_ADDR_SIZE);
                BT_DBG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n", bt_hfp_hp_peer_bd_addr[0],
                       bt_hfp_hp_peer_bd_addr[1], bt_hfp_hp_peer_bd_addr[2], bt_hfp_hp_peer_bd_addr[3],
                       bt_hfp_hp_peer_bd_addr[4], bt_hfp_hp_peer_bd_addr[5]);

                hfp_hf         = bt_hfp_hf_lookup_bt_addr(bt_hfp_hp_peer_bd_addr);
                hfp_hf->handle = handle;
                /* Update the eSCO channel paramters for Default CVSD Codec */
                bt_hfp_hf_set_esco_channel_parameters(BT_TRUE, bt_hfp_esco_params[0]);

                /* Get peer supported features */
                BT_hfp_unit_get_peer_supported_features(handle, &bt_hfp_hf_peer_supported_features_ext);

                hfp_hf->bt_hfp_hp_hfu_slc = 1;

                BT_hfp_unit_feature_control(handle, HFP_UNIT_FEATURE_CLIP, HFP_UNIT_ACTION_ENABLE);
            }

            break;

        case HFP_UNIT_DISCONNECT_CNF:
            BT_DBG("\n> Event          : HFP_UNIT_DISCONNECT_CNF\n");
            BT_DBG("> Instance       : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result   : 0x%04X\n", result);
            bd_addr = (uint8_t *)data;
            BT_DBG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3],
                   bd_addr[4], bd_addr[5]);
            (void)bd_addr;
            break;

        case HFP_UNIT_DISCONNECT_IND:
            BT_DBG("\n> Event          : HFP_UNIT_DISCONNECT_IND\n");
            BT_DBG("> Instance       : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result   : 0x%04X\n", result);
            bd_addr = (uint8_t *)data;
            BT_DBG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3],
                   bd_addr[4], bd_addr[5]);          
            break;

        case HFP_UNIT_CALL_ACTIVE:
            BT_DBG("\n> Event    : HFP_UNIT_CALL_ACTIVE\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            if (hfp_hf->bt_hf_cb->call)
            {
                hfp_hf->bt_hf_cb->call(hfp_hf->bt_conn, 1U);
            }

            break;
        case HFP_UNIT_NO_CALL:
            BT_DBG("\n> Event    : HFP_UNIT_NO_CALL\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            if (hfp_hf->bt_hf_cb->call)
            {
                hfp_hf->bt_hf_cb->call(hfp_hf->bt_conn, 0U);
            }
            break;

        case HFP_UNIT_CALLSETUP:
            BT_DBG("\n> Event            : HFP_UNIT_CALLSETUP\n");
            BT_DBG("> Instance         : 0x%02X\n", (unsigned int)handle);
            if (data != NULL)
            {
                BT_DBG("> Call Setup Value : %d\n", *(uint8_t *)data);
            }

            if (hfp_hf->bt_hf_cb->call_setup)
            {
                hfp_hf->bt_hf_cb->call_setup(hfp_hf->bt_conn, *(uint8_t *)data);
            }
            sco_audio_play_ringtone_exit_pl();
            BT_hfp_unit_set_gain
            (
                hfp_hf->handle,
                hf_volume_type_speaker,
                hfp_hf->bt_hfp_hp_speaker_volume,
                (UCHAR)BT_str_len(hfp_hf->bt_hfp_hp_speaker_volume)
            );
            sco_audio_set_speaker_volume_pl(atoi((char const *)(const char *)hfp_hf->bt_hfp_hp_speaker_volume));

            break;

        case HFP_UNIT_AG_SERVICE_IND:
            BT_DBG("\n> Event         : HFP_UNIT_AG_SERVICE_IND.\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Data Received : %d\n", *((uint8_t *)data));

            if (hfp_hf->bt_hf_cb->service)
            {
                hfp_hf->bt_hf_cb->service(hfp_hf->bt_conn, *((uint8_t *)data));
            }
            break;

        case HFP_UNIT_CIEV_SIGNAL_IND:
            BT_DBG("\n> Event    : HFP_UNIT_CIEV_SIGNAL_IND.\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Value    : %d\n", *((uint8_t *)data));

            if (hfp_hf->bt_hf_cb->signal)
            {
                hfp_hf->bt_hf_cb->signal(hfp_hf->bt_conn, *((uint8_t *)data));
            }
            break;

        case HFP_UNIT_CIEV_ROAM_IND:
            BT_DBG("\n> Event    : HFP_UNIT_CIEV_ROAM_IND.\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Value    : %d\n", *((uint8_t *)data));

            if (hfp_hf->bt_hf_cb->roam)
            {
                hfp_hf->bt_hf_cb->roam(hfp_hf->bt_conn, *((uint8_t *)data));
            }
            break;

        case HFP_UNIT_CIEV_BATTCHG_IND:
            BT_DBG("\n> Event    : HFP_UNIT_CIEV_BATTCHG_IND.\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Value    : %d\n", *((uint8_t *)data));
            if (hfp_hf->bt_hf_cb->battery)
            {
                hfp_hf->bt_hf_cb->battery(hfp_hf->bt_conn, *((uint8_t *)data));
            }
            break;

        case HFP_UNIT_CALL_HELD_IND:
            BT_DBG("\n> Event         : HFP_UNIT_CALL_HELD_IND\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);
            data_recvd = (uint8_t *)data;
            BT_DBG("> Data Received : %d\n", (*data_recvd));
            if (hfp_hf->bt_hf_cb->call)
            {
                hfp_hf->bt_hf_cb->call(hfp_hf->bt_conn, (*data_recvd));
            }
            break;

        case HFP_UNIT_INCALL_ALERT:
            BT_DBG("\n> Event     : HFP_UNIT_INCALL_ALERT\n");
            BT_DBG("> Instance  : 0x%02X\n", (unsigned int)handle);

            if (hfp_hf->bt_hf_cb->ring_indication)
            {
                hfp_hf->bt_hf_cb->ring_indication(hfp_hf->bt_conn);
            }
            /* Indicate platform of ring */
           sco_audio_play_ringtone_pl();

            break;

        case HFP_UNIT_CLI_DIGITS:
            BT_DBG("\n> Event    : HFP_UNIT_CLI_DIGITS\n");
            BT_DBG("> Number   : %s\n", app_parser_result.result_param.cli_info.digits);
            if (BT_str_len(app_parser_result.result_param.cli_info.name))
            {
                BT_DBG("> Name : %s\n", app_parser_result.result_param.cli_info.name);
            }
            if (hfp_hf->bt_hf_cb->call_phnum)
            {
                hfp_hf->bt_hf_cb->call_phnum(hfp_hf->bt_conn, (char *)app_parser_result.result_param.cli_info.digits);
            }
            break;

        case HFP_UNIT_CALL_WAITING_IND:
            BT_DBG("\n> Event           : HFP_UNIT_CALL_WAITING_IND\n");
            BT_DBG("> Instance        : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Received Number : %s\n", app_parser_result.result_param.ccwa_result.number);
            BT_DBG("> Number Type     : %d\n", app_parser_result.result_param.ccwa_result.type);
            BT_DBG("> Voice Class     : %d\n", app_parser_result.result_param.ccwa_result.voice_class);
            BT_DBG("> Operator Name   : %s\n", app_parser_result.result_param.ccwa_result.alpha);
            if (hfp_hf->bt_hf_cb->waiting_call)
            {
                hfp_hf->bt_hf_cb->waiting_call(hfp_hf->bt_conn, (hf_waiting_call_state_t *)&app_parser_result.result_param.ccwa_result);
            }

            break;

        case HFP_UNIT_VOICE_RECOG_IND:
            BT_DBG("\n> Event         : HFP_UNIT_VOICE_RECOG_IND\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);

#ifdef HFP_UNIT_1_8
            BT_DBG("> vrect : %d\n", app_parser_result.result_param.bvra_result.vrect);

            if ((0 != (bt_hfp_hf_local_supported_features_ext & BT_HFP_HF_BRSF_CMD_FEATURE_BITMASK_ENH_VR_STATUS)) &&
                (0 != (bt_hfp_hf_peer_supported_features_ext & BT_HFP_HF_BRSF_RSP_FEATURE_BITMASK_ENH_VR_STATUS)))
            {
                /* Enhanced Voice Recognition related field */

                BT_DBG("> vrecstate : 0x%02x\n", app_parser_result.result_param.bvra_result.vrecstate);
            }

            if ((0 != (bt_hfp_hf_local_supported_features_ext & BT_HFP_HF_BRSF_CMD_FEATURE_BITMASK_VR_TEXT)) &&
                (0 != (bt_hfp_hf_peer_supported_features_ext & BT_HFP_HF_BRSF_RSP_FEATURE_BITMASK_VR_TEXT)))
            {
                /* Voice Recognition TextFeature related fields */

                BT_DBG("> text_id : 0x%04X\n", app_parser_result.result_param.bvra_result.text_rep.text_id);

                BT_DBG("> text_type : %d\n", app_parser_result.result_param.bvra_result.text_rep.text_type);

                BT_DBG("> text_operation : %d\n", app_parser_result.result_param.bvra_result.text_rep.text_op);

                BT_DBG("> String : %s\n", app_parser_result.result_param.bvra_result.text_rep.str);
            }
#else
            BT_DBG("> Data Received : %d\n", app_parser_result.result_param.uchar_result);
#endif /* HFP_UNIT_1_8 */
            break;

        case HFP_UNIT_BSIR_IND:
            BT_DBG("\n> Event         : HFP_UNIT_BSIR_IND\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Data Received : %d\n", app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_VGM_IND:
            BT_DBG("\n> Event        : HFP_UNIT_VGM_IND\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);

            /* Store the value of VGM */
            BT_mem_copy(hfp_hf->bt_hfp_hp_microphone_gain,
                        &data_to_app->buffer[data_to_app->parser_resp->param[0].start_of_value_index],
                        data_to_app->parser_resp->param[0].value_length);
            /* NULL terminate the str */
            hfp_hf->bt_hfp_hp_microphone_gain[data_to_app->parser_resp->param[0].value_length] = '\0';

            sco_audio_set_microphone_gain_pl(atoi((char const *)(const char *)hfp_hf->bt_hfp_hp_microphone_gain));
            BT_DBG("> Data Received : %s\n", hfp_hf->bt_hfp_hp_microphone_gain);
            break;

        case HFP_UNIT_VGS_IND:
            BT_DBG("\n> Event        : HFP_UNIT_VGS_IND\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            /* Store the value of VGS */
            BT_mem_copy(hfp_hf->bt_hfp_hp_speaker_volume,
                        &data_to_app->buffer[data_to_app->parser_resp->param[0].start_of_value_index],
                        data_to_app->parser_resp->param[0].value_length);
            /* NULL terminate the str */
            hfp_hf->bt_hfp_hp_speaker_volume[data_to_app->parser_resp->param[0].value_length] = '\0';
            sco_audio_set_speaker_volume_pl(atoi((char const *)(const char *)hfp_hf->bt_hfp_hp_speaker_volume));
            BT_DBG("> Data Received : %s\n", hfp_hf->bt_hfp_hp_speaker_volume);
            break;

        case HFP_UNIT_VOICETAG_PHNUM_IND:
            BT_DBG("\n> Event           : HFP_UNIT_VOICETAG_PHNUM_IND\n");
            BT_DBG("> Instance        : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Received Number : %s\n", app_parser_result.result_param.digits);
            if (hfp_hf->bt_hf_cb->voicetag_phnum)
            {
                hfp_hf->bt_hf_cb->voicetag_phnum(hfp_hf->bt_conn, (char *)app_parser_result.result_param.digits);
            }            
            break;

        case HFP_UNIT_RECVD_BTRH_IND:
            BT_DBG("\n> Event         : HFP_UNIT_RECVD_BTRH_IND.\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Data Received : %d\n", app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_REQ_SUB_NUM_IND:
            BT_DBG("\n> Event             : HFP_UNIT_REQ_SUB_NUM_IND\n");
            BT_DBG("> Instance          : 0x%02X\n", (unsigned int)handle);

            BT_DBG("> Subscriber Number : %s\n", app_parser_result.result_param.cnum_resp_result.number);
            BT_DBG("> Number Type       : %d\n", app_parser_result.result_param.cnum_resp_result.type);
            BT_DBG("> Service Class     : %d\n", app_parser_result.result_param.cnum_resp_result.service);
            break;

        case HFP_UNIT_COPS_QUERY_RESULT_IND:
            BT_DBG("\n> Event         : HFP_UNIT_COPS_QUERY_RESULT_IND\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);

            BT_DBG("> Mode          : %d\n", app_parser_result.result_param.cops_resp_result.mode);
            BT_DBG("> Format        : %d\n", app_parser_result.result_param.cops_resp_result.format);
            BT_DBG("> Operator Name : %s\n", app_parser_result.result_param.cops_resp_result.op_name);

            break;

        case HFP_UNIT_CURRENT_CALL_LIST_IND:
            BT_DBG("\n> Event         : HFP_UNIT_CURRENT_CALL_LIST_IND\n");
            BT_DBG("> Instance      : 0x%02X\n", (unsigned int)handle);

            BT_DBG("> ID            : %d\n", app_parser_result.result_param.clcc_resp_result.id);
            BT_DBG("> DIR           : %d\n", app_parser_result.result_param.clcc_resp_result.dir);
            BT_DBG("> Status        : %d\n", app_parser_result.result_param.clcc_resp_result.status);
            BT_DBG("> Mode          : %d\n", app_parser_result.result_param.clcc_resp_result.mode);
            BT_DBG("> Mprty         : %d\n", app_parser_result.result_param.clcc_resp_result.mprty);
            BT_DBG("> Number        : %s\n", app_parser_result.result_param.clcc_resp_result.number);
            BT_DBG("> Type          : %d\n", app_parser_result.result_param.clcc_resp_result.type);
            BT_DBG("> Operator Name : %s\n", app_parser_result.result_param.clcc_resp_result.alpha);

            break;

        case HFP_UNIT_PEER_IND_STATUS_IND:
            BT_DBG("\n> Event    : HFP_UNIT_PEER_INDICATOR_STATUS_IND\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);

            cind_result = (HFP_UNIT_CIND_READ_RESULT *)data;
            (void)cind_result;

            BT_DBG("> ID : Battchg %d\n", cind_result->battchg);
            BT_DBG("> ID : Call %d\n", cind_result->call);
            BT_DBG("> ID : Call_Setup %d\n", cind_result->call_setup);
            BT_DBG("> ID : CallHeld %d\n", cind_result->callheld);
            BT_DBG("> ID : Callsetup %d\n", cind_result->callsetup);
            BT_DBG("> ID : Roam %d\n", cind_result->roam);
            BT_DBG("> ID : Service %d\n", cind_result->service);
            BT_DBG("> ID : Signal %d\n", cind_result->signal);
            break;

        case HFP_UNIT_AG_ERROR_IND:
            BT_DBG("\n> Event     : HFP_UNIT_AG_ERROR_IND\n");
            BT_DBG("> Instance  : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Error     : 0x%04X\n", result);
            break;

        case HFP_UNIT_CMEE_IND:
            BT_DBG("\n> Event          : HFP_UNIT_CMEE_IND\n");
            BT_DBG("> Instance       : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> CMEE ERR Value : %d\n", app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_RECVD_DATA_IND:
            BT_DBG("\n> Event    : HFP_UNIT_RECVD_DATA_IND\n");
            BT_DBG("> Instance : 0x%02X\n", (unsigned int)handle);
            if (data != NULL)
            {
                data_recvd = (uint8_t *)data;

                BT_DBG("\n----------------CHAR DUMP-----------------------\n");
                for (index = 0; index < data_len; index++)
                {
                    BT_DBG("%c ", data_recvd[index]);
                }
                BT_DBG("\n------------------------------------------------\n");
                BT_DBG("\n----------------HEX DUMP------------------------\n");
                for (index = 0; index < data_len; index++)
                {
                    BT_DBG("%X ", data_recvd[index]);
                }
                BT_DBG("\n------------------------------------------------\n");
            }
            break;

        case HFP_UNIT_SEND_DATA_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SEND_DATA_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_PEER_IND_STATUS_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_PEER_INDICATOR_STATUS_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_VOICETAG_PHNUM_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_VOICETAG_PHNUM_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_INCALL_ACCEPT_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_INCALL_ACCEPT_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_OUTCALL_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_OUTCALL_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_CALLHANGUP_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_CALLHANGUP_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_TWC_CALL_CTRL_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_TWC_CALL_CTRL_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_SET_VGM_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SET_VGM_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_SET_VGS_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SET_VGS_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_CCWA_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_NOTIFICATION_CCWA_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_CLIP_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_NOTIFICATION_CLIP_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);

            if (hfp_hf->bt_hfp_hp_hfu_slc)
            {
                /* Enable Call waiting */
                BT_hfp_unit_feature_control(handle, HFP_UNIT_FEATURE_CCWA, HFP_UNIT_ACTION_ENABLE);
            }
            break;

        case HFP_UNIT_ECNR_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_ECNR_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_VREC_ENABLE_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_VREC_ENABLE_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

#ifdef HFP_UNIT_1_8
        case HFP_UNIT_ENH_VREC_ENABLE_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_ENH_VREC_ENABLE_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;
#endif /* HFP_UNIT_1_8 */

        case HFP_UNIT_VREC_DISABLE_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_VREC_DISABLE_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_SEND_DTMF_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SEND_DTMF_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_STOP_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_STOP_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            BT_dbase_inactivate_record(hfp_hfu_record_handle);
            break;

        case HFP_UNIT_CMEE_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_CMEE_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_REQ_SUB_NUM_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_REQ_SUB_NUM_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_SET_NW_NAME_FORMAT_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SET_NW_NAME_FORMAT_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_SEND_BTRH_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_SEND_BTRH_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_ADV_CALL_HOLD_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_ADV_CALL_HOLD_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_COPS_QUERY_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_COPS_QUERY_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_CURRENT_CALL_LIST_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_CURRENT_CALL_LIST_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

#ifdef HFP_UNIT_1_6
        case HFP_UNIT_BIA_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_BIA_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_BAC_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_BAC_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_BCC_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_BCC_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_BCS_CNF:
            BT_DBG("\n> Event        : HFP_UNIT_BCS_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);
            break;

        case HFP_UNIT_BCS_IND:
            BT_DBG("\n> Event        : HFP_UNIT_BCS_IND\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);

            /* Get Codec ID from event buffer */
            esco_codec_id = data_to_app->buffer[data_to_app->parser_resp->param->start_of_value_index] - '0';

            /* Based on Codec ID, update eSCO parameters */
            if ((HFP_UNIT_CODEC_ID_CVSD == esco_codec_id) || (HFP_UNIT_CODEC_ID_MSBC == esco_codec_id))
            {
                /* Update the eSCO channel parameters for mSBC Codec */
                bt_hfp_hf_set_esco_channel_parameters(BT_TRUE, bt_hfp_esco_params[esco_codec_id - 1]);
            }
            else
            {
                BT_ERR("Codec Selection: ???\n");
            }

            /* Send codec confirmation */
            BT_hfp_unit_codec_confirmation_num(handle, esco_codec_id);

            break;

#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7

        case HFP_UNIT_BIEV_CNF:

            BT_DBG("\n> Event        : HFP_UNIT_BIEV_CNF\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);

            break;

        case HFP_UNIT_BIND_IND:

            BT_DBG("\n> Event      : HFP_UNIT_BIND_IND\n");
            BT_DBG("> Instance     : 0x%02X\n", (unsigned int)handle);
            BT_DBG("> Event result : 0x%04X\n", result);

            bt_bt_hfp_hf_peer_hf_ind_read_info     = (HFP_UNIT_BIND_READ_RESULT *)data;

            BT_DBG("Updated HF Indicator Info:\n");
            BT_DBG("    ID    : 0x%04x\n", bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_id);
            BT_DBG("    Value : 0x%02x\n", bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_val);

            /* Update Peer HF indicator status */
            for (index = 0; index < bt_hfp_hf_peer_hf_ind_count; index++)
            {
                if (bt_bt_hfp_hf_peer_hf_ind_list_info[index].hf_ind_id ==
                    bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_id)
                {
                    bt_bt_hfp_hf_peer_hf_ind_list_info[index].hf_ind_val =
                        bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_val;
                }
            }

            /**
             * According to section 4.35.1.4 in HFPv1.7 specification:
             * To change the state, the AG shall send an unsolicited +BIND: anum,state response code.
             * Whenever the HF receives an unsolicited +BIND indication from the AG that changes the
             * state of a particular HF indicator from disabled to enabled, the HF should send the
             * current state of that indicator to the AG using the +BIEV command
             *
             * But Battery Level Indicator value can have 0 to 100, in this case how to decide
             * the disabled to enabled condition?
             *
             * For Now sending +BIEV command only when the received Ehanced Safety indicator value = 1
             */
            if ((HFP_UNIT_HF_IND_ID_ENHANCED_SAFETY == bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_id) &&
                (0x01 == bt_bt_hfp_hf_peer_hf_ind_read_info->hf_ind_val))
            {
                BT_str_copy(send_string, "1,1");
            }
            else
            {
                /* Not sending AT+BIEV Command to synchronize HF Unit & AG */
                break;
            }

            BT_DBG("Sending AT+BIEV Command to Synchronize HF Unit & AG\n");

            send_string_length = (uint8_t)strnlen((char const *)send_string, HFP_UNIT_MAX_COMMAND_LENGTH);

            retval = BT_hfp_unit_update_hf_ind_value(handle, send_string, send_string_length);
            if (API_SUCCESS != retval)
            {
                BT_ERR("Failed: BT_hfp_unit_update_hf_ind_value 0x%04X\n", retval);
            }

            break;
#endif /* HFP_UNIT_1_7 */

        default:
            BT_DBG("\n> UNKNOWN EVENT RECVD FROM PROFILE :0x%04X\n", event);
            return API_SUCCESS;
    }

#ifdef HFP_UNIT_1_7
    if (((HFP_UNIT_CONNECT_IND == event) || (HFP_UNIT_CONNECT_CNF == event)) && (API_SUCCESS == result))
    {
        /**
         * After SLC procedur getting the peer supported HF indicators
         * and corresponding status
         */
        retval = BT_hfp_unit_get_peer_hf_indicators(handle, bt_bt_hfp_hf_peer_hf_ind_list_info,
                                                    &bt_hfp_hf_peer_hf_ind_count);
        if (API_SUCCESS == retval)
        {
            BT_DBG("=== Peer HF indicators info. ===\n");
            BT_DBG("No. of Indicators: 0x%02x\n", bt_hfp_hf_peer_hf_ind_count);
            BT_DBG("HF indicators status:\n");
            for (index = 0; index < bt_hfp_hf_peer_hf_ind_count; index++)
            {
                BT_DBG("    ID    : 0x%08x\n", bt_bt_hfp_hf_peer_hf_ind_list_info[index].hf_ind_id);
                BT_DBG("    Value : 0x%02x\n", bt_bt_hfp_hf_peer_hf_ind_list_info[index].hf_ind_val);
            }
        }
    }
#endif /* HFP_UNIT_1_7 */

    return API_SUCCESS;
}

static API_RESULT bt_hfp_hf_start(HFP_UNIT_APPL_CONFIG_PARAMS *p_bt_hfp_hf_appl_conf_params)
{
    API_RESULT api_retval;

    /* local variable to extract the supported features */
    uint8_t attr_value[] = {0x09, 0x00, 0x00};

    /* local supported features */
    uint32_t supported_features;

    supported_features = atoi((char const *)p_bt_hfp_hf_appl_conf_params->supp_features);

    attr_value[2] = (uint8_t)(supported_features & 0x1F);

    if (0x80 & supported_features)
    {
        /* Yes. Enable wideband speech bit (6th bit)*/
        attr_value[2] |= 0x20;
    }

    /* Get HFP_UNIT SDP record handle */
    BT_dbase_get_record_handle(DB_RECORD_HFU, 0, &hfp_hfu_record_handle);

    api_retval = BT_dbase_update_attr_value(hfp_hfu_record_handle, 0x0311, attr_value, 0x03);

    BT_dbase_get_server_channel(hfp_hfu_record_handle, PROTOCOL_DESC_LIST,
                                &p_bt_hfp_hf_appl_conf_params->server_channel);

    api_retval = BT_hfp_unit_start(p_bt_hfp_hf_appl_conf_params);
    BT_INFO("> API RETVAL BT_hfp_unit_start : 0x%04X\n", api_retval);

    if (API_SUCCESS == api_retval)
    {
        BT_dbase_activate_record(hfp_hfu_record_handle);

        BT_INFO("> HF Profile Started Successfully\n");
    }

    return api_retval;
}

static void hfp_hf_init(void)
{
    API_RESULT api_retval;
    api_retval = BT_hfp_unit_init(bt_hfp_hf_callback_registered_with_hfu);

    BT_INFO("> API RETVAL BT_hfp_unit_init : 0x%04X\n", api_retval);

    if (API_SUCCESS == api_retval)
    {
        BT_INFO("> BT HFP HF Init Successful\n");
    }

    sprintf((char *)bt_hfp_hf_local_supported_features, "%u", BT_HFP_HF_SUPPORTED_FEATURES);

    bt_hfp_hf_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN - 1] = '\0';

    bt_hfp_hf_local_supported_features_ext = BT_HFP_HF_SUPPORTED_FEATURES;

#ifdef HFP_UNIT_1_6

    bt_hfp_hf_conf_params.supp_features = bt_hfp_hf_local_supported_features;
    bt_hfp_hf_conf_params.supp_features_len =
        (uint16_t)strnlen((char const *)bt_hfp_hf_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN);
    if (bt_hfp_hf_local_supported_features_ext & BT_HFP_HF_FEATURE_CODEC_NEG)
    {
        BT_str_n_copy(bt_hfp_hf_local_available_codecs, "1,2", 4);
    }
    else
    {
        BT_str_n_copy(bt_hfp_hf_local_available_codecs, "1", 2);
    }
    bt_hfp_hf_conf_params.available_codecs = bt_hfp_hf_local_available_codecs;
    bt_hfp_hf_conf_params.available_codecs_len =
        (uint16_t)strnlen((char const *)bt_hfp_hf_local_available_codecs, (BT_HFU_MAX_AVL_CODEC_ID_LIST_LEN + 1));

#ifdef HFP_UNIT_1_7
    if (bt_hfp_hf_local_supported_features_ext & BT_HFP_HF_FEATURE_CODEC_NEG)
    {
        BT_str_n_copy(bt_hfp_hf_local_supported_hf_indicators, "1,2", 4);
    }
    bt_hfp_hf_conf_params.hf_indicators = bt_hfp_hf_local_supported_hf_indicators;
    bt_hfp_hf_conf_params.hf_indicators_len =
        (uint8_t)strnlen((char const *)bt_hfp_hf_local_supported_hf_indicators, (BT_HFP_HF_HF_INDICATOR_LIST_LEN + 1));
    /* Set the default */
    bt_hfp_hf_peer_hf_ind_count = 0;
#endif /* HFP_UNIT_1_7 */

    bt_hfp_hf_start(&bt_hfp_hf_conf_params);
#else  /* HFP_UNIT_1_6 */
    bt_hfp_hf_start(bt_hfp_hf_local_supported_features,
                    (uint8_t)strnlen(bt_hfp_hf_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN));
#endif /* HFP_UNIT_1_6 */
}

static void bt_disconnected(struct bt_conn *conn, uint8_t reason)
{
    struct bt_hfp_hf_em *hfp_hf;
    BT_INFO("Dis Connection failed (reason 0x%02x)\n", reason);
    hfp_hf = bt_hfp_hf_lookup_bt_conn(conn);
       
    if (hfp_hf->bt_hf_cb->disconnected)
    {
        hfp_hf->bt_hf_cb->disconnected(conn);
    }
    BT_mem_set(hfp_hf->peerAddr, 0, BT_BD_ADDR_SIZE);
    hfp_hf_DeActiveInstance(hfp_hf);
}
static void bt_connected(struct bt_conn *conn, uint8_t err)
{
    struct bt_hfp_hf_em *hfp_hf;
    struct bt_conn_info info;
    if (err)
    {
        BT_ERR("Connection failed (err 0x%02x)\n", err);
    }
    else
    {
        bt_conn_ref(conn);
        BT_INFO("bt_connected\n");

        hfp_hf = hfp_hf_GetNoneActiveInstance();
        (void)memset(&info, 0, sizeof(info));
        bt_conn_get_info(conn, &info);
        if (info.type == BT_CONN_TYPE_LE)
        {
            hfp_hf_FreeInstance(hfp_hf);
            return;
        }

        memcpy(hfp_hf->peerAddr, info.br.dst, BT_BD_ADDR_SIZE);
        if (hfp_hf->bt_hf_cb->connected)
        {
            hfp_hf->bt_hf_cb->connected(conn);
        }
        hfp_hf->actived     = 1U;
        hfp_hf->bt_conn     = conn;
        hfp_hf->hf_features = BT_HFP_HF_SUPPORTED_FEATURES;

    }
}

static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_to_str(bt_conn_get_dst_br(conn), addr, sizeof(addr));

    if (!err)
    {
        BT_INFO("Security changed: %s level %u\n", addr, level);
    }
    else
    {
        BT_ERR("Security failed: %s level %u err %d\n", addr, level, err);
    }
}

static struct bt_conn_cb conn_callbacks = {
    .connected        = bt_connected,
    .disconnected     =  bt_disconnected,
    .security_changed = security_changed,
};

/** @brief Register HFP HF profile
 *
 *  Register Handsfree profile callbacks to monitor the state and get the
 *  required HFP details to display.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_hfp_hf_register(struct bt_hfp_hf_cb *cb)
{
    if (!cb)
    {
        return -EINVAL;
    }

    struct bt_hfp_hf_em *hfp_hf;
    if (NULL == s_HfpHfLock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)s_HfpHfLockMutex))
        {
            s_HfpHfLock = (osa_mutex_handle_t)s_HfpHfLockMutex;
        }
        else
        {
            return -EIO;
        }
    }
    hfp_hf = hfp_hf_GetNoneActiveInstance();
    if (NULL == hfp_hf)
    {
        hfp_hf = hfp_hf_GetInstance();
        if (NULL == hfp_hf)
        {
            return -EIO;
        }
    }

    hfp_hf->bt_hf_cb = cb;
    bt_conn_cb_register(&conn_callbacks);
    hfp_hf_init();
    hfp_hf->actived = 0;
    BT_str_n_copy(&hfp_hf->bt_hfp_hp_speaker_volume[0], BT_HFP_HF_INITIAL_GAIN, 3);
    BT_str_n_copy(&hfp_hf->bt_hfp_hp_microphone_gain[0], BT_HFP_HF_INITIAL_GAIN, 3);

    return 0;
}

/** @brief Handsfree client Send AT
 *
 *  Send specific AT commands to handsfree client profile.
 *
 *  @param conn Connection object.
 *  @param cmd AT command to be sent.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_hfp_hf_send_cmd(struct bt_conn *conn, enum bt_hfp_hf_at_cmd cmd)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;
    struct bt_hfp_hf_cmd_complete cmd_complete = {0};

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    switch (cmd)
    {
        case BT_HFP_HF_ATA:
            api_retval = BT_hfp_unit_callaccept(hf->handle);

            if (api_retval < 0)
            {
                BT_ERR("Failed ATA");
                status = api_retval;
            }
            break;
        case BT_HFP_HF_AT_CHUP:
            api_retval = BT_hfp_unit_callhangup(hf->handle);
            if (api_retval < 0)
            {
                BT_ERR("Failed AT+CHUP");
                status = api_retval;
            }
            break;
        default:
            BT_ERR("Invalid AT Command");
            status = -EINVAL;
            break;
    }
    if (status < 0)
    {
        cmd_complete.type = HFP_HF_CMD_ERROR;
    }
    else
    {
        cmd_complete.type = HFP_HF_CMD_OK;
    }

    if (hf->bt_hf_cb->cmd_complete_cb)
    {
        hf->bt_hf_cb->cmd_complete_cb(conn, &cmd_complete);
    }
    return status;
}
int bt_hfp_hf_start_voice_recognition(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        
        return -ENOTCONN;
    }
    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_BVRA,
                     HFP_UNIT_ACTION_ENABLE
                 );

    if (api_retval < 0)
    {
        BT_ERR("Failed start voice recognition");
        status = api_retval;
    }

    return status;  
}

int bt_hfp_hf_stop_voice_recognition(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }
    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_BVRA,
                     HFP_UNIT_ACTION_DISABLE
                 );

    if (api_retval < 0)
    {
        BT_ERR("Failed stop voice recognition");
        status = api_retval;
    }
    return status;
}
int bt_hfp_hf_dial(struct bt_conn *conn, const char *number)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_dial
               (
                   hf->handle,
                   (void *)number,
                   (UCHAR)BT_str_len(number)
               );

    if (api_retval < 0)
    {
        BT_ERR("Failed to dial number");
        status = api_retval;
    }
    return status;  
  
}
int bt_hfp_hf_dial_memory(struct bt_conn *conn, int location)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;
    char                     memid[4U];

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    sprintf(memid,"%d", location);

    /* Remove the terminal '\n', if any */
    if ('\n' == memid[sizeof(memid) - 1U])
    {
        memid[sizeof(memid) - 1U] = '\0';
    }
    else if ('\n' == memid[BT_str_len(memid) - 1U])
    {
        memid[BT_str_len(memid) - 1U] = '\0';
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    api_retval = BT_hfp_unit_memdial
                 (
                     hf->handle,
                     memid,
                     (UCHAR)BT_str_len(memid)
                 );


    if (api_retval < 0)
    {
        BT_ERR("Failed to dial number");
        status = api_retval;
    }
    return status;  
    
}
int bt_hfp_hf_last_dial(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_redial
                 (
                     hf->handle
                 );


    if (api_retval < 0)
    {
        BT_ERR("Failed to dial number");
        status = api_retval;
    }
    return status; 
}
int bt_hfp_hf_multiparty_call_option(struct bt_conn *conn, hf_multiparty_call_option_t option)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_control_multiparty_call
                 (
                     hf->handle,
                     option
                 );
    if (api_retval < 0)
    {
        BT_ERR("Failed to control multiparty call");
        status = api_retval;
    }
    return status;   
}
int bt_hfp_hf_enable_clip_notification(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_CLIP,
                     HFP_UNIT_ACTION_ENABLE
                 );
    if (api_retval < 0)
    {
        BT_ERR("Failed to control multiparty call");
        status = api_retval;
    }
    return status;    
}

int bt_hfp_hf_disable_clip_notification(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_CLIP,
                     HFP_UNIT_ACTION_DISABLE
                 );
    if (api_retval < 0)
    {
        BT_ERR("Failed to control multiparty call");
        status = api_retval;
    }
    return status;    
}
int bt_hfp_hf_enable_call_waiting_notification(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_CCWA,
                     HFP_UNIT_ACTION_ENABLE
                 );
    if (api_retval < 0)
    {
        BT_ERR("Failed to control multiparty call");
        status = api_retval;
    }
    return status;    
}
int bt_hfp_hf_disable_call_waiting_notification(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    api_retval = BT_hfp_unit_feature_control
                 (
                     hf->handle,
                     HFP_UNIT_FEATURE_CCWA,
                     HFP_UNIT_ACTION_DISABLE
                 );
    if (api_retval < 0)
    {
        BT_ERR("Failed to control multiparty call");
        status = api_retval;
    }
    return status;    
}
int bt_hfp_hf_volume_update(struct bt_conn *conn, hf_volume_type_t type, int volume)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;
    UCHAR volumeStr[3U];

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }

    sprintf((char *)&volumeStr[0U],"%d", volume);
    
    api_retval = BT_hfp_unit_set_gain
                 (
                     hf->handle,
                     type,
                     volumeStr,
                     (UCHAR)BT_str_len(volumeStr)
                 );

    printf("> API RETVAL Set Volume 0x%04X\n",api_retval);
    if (type == hf_volume_type_speaker )
    {
        BT_str_n_copy(&hf->bt_hfp_hp_speaker_volume[0], volume, 3);
        sco_audio_set_speaker_volume_pl(volume);
    }
    else
    {
        BT_str_n_copy(&hf->bt_hfp_hp_microphone_gain[0], volume, 3);
    }
    return status;    
}
int bt_hfp_hf_get_last_voice_tag_number(struct bt_conn *conn)
{
    struct bt_hfp_hf_em *hf;
    int api_retval;
    int status                                 = 0;

    if (!conn)
    {
        BT_ERR("Invalid connection");
        return -ENOTCONN;
    }

    hf = bt_hfp_hf_lookup_bt_conn(conn);
    if (!hf)
    {
        BT_ERR("No HF connection found");
        return -ENOTCONN;
    }
    api_retval = BT_hfp_unit_feature_control
             (
                 hf->handle,
                 HFP_UNIT_FEATURE_BINP,
                 HFP_UNIT_ACTION_ENABLE
             );
    printf("> API RETVAL Set Volume 0x%04X\n",api_retval);

    return status;    
}

/**
 * @}
 */
#endif /* CONFIG_BT_HFP_HF */
