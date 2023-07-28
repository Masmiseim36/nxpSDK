
/**
 *  \file appl_hfp_unit.c
 *
 *  Source file for HFP UNIT Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hfp_unit.h"
#include "appl_utils.h"
#include "sco_audio_pl.h"

#ifdef HFP_UNIT
/* ----------------------------------------- Global Variables */
static const char hfp_unit_options[] = "\n\
-------------- HFP UNIT Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
    2.  Register BD_ADDR of Audio Gateway \n\
    3.  Get Registered BD_ADDR of Audio Gateway \n\
\n\
    4.  Initialize Hands-Free Unit \n\
    5.  Start Hands-Free Unit \n\
    6.  Stop Hands-Free Unit \n\
\n\
    7.  Create ACL connection\n\
    8.  Get AG's SDP record \n\
    9.  Establish SLC connection with AG \n\
   10.  Release SLC with AG \n\
   11.  Release ACL with AG \n\
\n\
   19.  Establish Synchronous Connection with custom eSCO parameter \n\
   20.  Establish Synchronous Connection (HV Packet types only) with AG \n\
   21.  Establish Synchronous Connection (HV+EV Packet types only) with AG \n\
   22.  Establish Synchronous Connection (EV Packet types only) with AG\n\
   23.  Release Synchronous Connection with AG \n\
   24.  Select CVSD SCO parameter set \n\
   25.  Select CVSD eSCO parameter set \n\
   26.  Select mSBC eSCO parameter set \n\
   27.  Set eSCO parameter\n\
\n\
   30.  Dial with number \n\
   31.  Memory Dialing \n\
   32.  Last Number ReDial \n\
\n\
   40.  Accept Incoming Call \n\
   41.  Reject Call \n\
   42.  HangUp ongoing call \n\
   43.  Transfer Call to HFP UNIT \n\
   44.  Transfer Call to AG \n\
   45.  Three Way Call (TWC) Options\n\
\n\
   50.  Enable CLIP Notification \n\
   51.  Disable CLIP Notification \n\
   52.  Enable Call Waiting Notification \n\
   53.  Disable Call Waiting Notification \n\
   54.  Enable Voice Recognition \n\
   55.  Disable Voice Recognition \n\
   56.  Disable ECNR in AG \n\
   57.  Get Voice-tag Phone Number (BINP) \n\
\n\
   60.  Set Speaker Volume \n\
   61.  Set Microphone Gain \n\
   62.  Get Speaker Volume & Microphone Gain \n\
\n\
   70.  Get peer Indicator status\n\
   71.  Get peer Indicator List \n\
   72.  Get peer supported CHLD values \n\
   73.  Send DTMF Code\n\
   74.  Use HF 1.5 Features \n\
   75.  Use HF 1.6 Features \n\
   76.  Send String to AG \n\
   77.  Send custom AT command to AG \n\
\n\
   80.  Abort\n\
\n\
   90.  List all BD_ADDR \n\
   91.  Select HFP Unit Handle. \n\
   92.  Get Current HFP Unit Handle. \n\
\n\
   93.  Select HFP 1.7 Features \n\
\n\
   94.  Select HFP 1.8 Features \n\
\n\
Your Choice : ";

static const char hfp_unit_1_5_options[] = "\n\
-------------- HFP UNIT (1.5) Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\n\
    2.  Request AG Subscriber Number \n\
    3.  Query Response And Hold Status \n\
    4.  Set Network Name Format\n\
    5.  Query Operator Selection \n\
    6.  Query List of Current Calls \n\
    7.  Send Response and Hold to AG \n\
    8.  Send Call Hold with Call ID \n\
    9.  Enable CMEE Error Reporting \n\
    10. Disable CMEE Error Reporting \n\
\n\
Your Choice : ";

#ifdef HFP_UNIT_1_6
static const char hfp_unit_1_6_options[] = "\n\
-------------- HFP UNIT (1.6) Menu ---------------\n\
    0 .  Exit \n\
    1 .  Refresh \n\n\
    2 .  (De)Activate individual indicators \n\
    3 .  (De)Activate individual indicators with activation bitmask \n\
    4 .  (De)Activate individual indicators with new and previous activation bitmask \n\
    5 .  (De)Activate individual indicators with required indicators and activation bitmask \n\
    6 .  Inform AG about available codecs \n\
    7 .  Inform AG about available codecs (numeric) \n\
    8 .  Trigger Codec Connection \n\
    9 .  Confirm Codec Selection \n\
    10.  Confirm Codec Selection (numeric) \n\
\n\
Your Choice : ";
#endif /* HFP_UNIT_1_6 */

#define FSTR(x) STR(x)
#define STR(x) #x

DECL_STATIC UCHAR send_string[HFP_UNIT_MAX_COMMAND_LENGTH + 2U];
DECL_STATIC UCHAR send_string_length;

/* HFP_UNIT Application Configuration parameters */
DECL_STATIC HFP_UNIT_APPL_CONFIG_PARAMS appl_hfp_unit_conf_params;

/* AG's BD_ADDR */
DECL_STATIC UCHAR appl_peer_bd_addr[BT_BD_ADDR_SIZE];

/* peer AG's supported features */
DECL_STATIC UINT16 hfp_unit_peer_supported_features[HFP_UNIT_MAX_CONNECTIONS];

/* Stores BD_ADDR for all instances */
DECL_STATIC UCHAR appl_bd_addr[HFP_UNIT_MAX_CONNECTIONS][BT_BD_ADDR_SIZE];

/* Global handle, used with all HFP Unit API calls.*/
DECL_STATIC HFP_UNIT_HANDLE g_hfp_unit_handle;

/* HFP_UNIT local supported features */
DECL_STATIC UCHAR hfp_unit_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN];

/* HFP_UNIT local supported features in numeric format */
DECL_STATIC UINT32 hfp_unit_local_supported_features_ext;

/* HFP_UNIT peer supported features in numeric format */
DECL_STATIC UINT32 hfp_unit_peer_supported_features_ext;

#ifdef HFP_UNIT_1_6
/* HFP_UNIT local list of available codecs */
DECL_STATIC UCHAR hfp_unit_local_available_codecs[APPL_HFU_MAX_AVL_CODEC_ID_LIST_LEN+1U];
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
/* HFP_UNIT local list of supported HF indicators */
DECL_STATIC UCHAR hfp_unit_local_supported_hf_indicators[APPL_HFP_UNIT_HF_INDICATOR_LIST_LEN+1U];

/* Peer supported HF indicators */
DECL_STATIC UCHAR hfp_unit_peer_hf_ind_count;
DECL_STATIC HFP_UNIT_BIND_READ_RESULT  hfp_unit_peer_hf_ind_list_info[HFP_UNIT_MAX_HF_INDICATOR_COUNT];
DECL_STATIC HFP_UNIT_BIND_READ_RESULT *hfp_unit_peer_hf_ind_read_info;

#endif /* HFP_UNIT_1_7 */

/* HFP_UNIT SDP record handle */
DECL_STATIC UINT32 hfp_hfu_record_handle;

/* SCO-CVSD D0-D1 settings */
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfp_sco_cvsd_params[] =
{
    /* D0 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        LMP_MAX_LATENCY_DONT_CARE,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_HV1 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_NONE
    },

    /* D1 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        LMP_MAX_LATENCY_DONT_CARE,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_HV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_NONE
    }
};

/* eSCO-CVSD S1-S4 settings */
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfp_esco_cvsd_params[] =
{
    /* Default */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        LMP_MAX_LATENCY_DONT_CARE,
        LMP_VOICE_SETTING_DEFAULT,
        LMP_ESCO_PACKET_TYPE_DEFAULT,
        LMP_RETX_EFFORT_DONT_CARE
    },

    /* S1 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x0007U,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_EV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_POWER
    },

    /* S2 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x0007U,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_POWER
    },

    /* S3 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x000AU,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_POWER
    },

    /* S4 */
    {
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x000CU,
        LMP_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_QUALITY
    }
};

/* eSCO-mSBC T1-T2 settings */
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfp_esco_msbc_params[] =
{
    /* Default */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        LMP_MAX_LATENCY_DONT_CARE,
        APPL_WIDEBAND_VOICE_SETTING_DEFAULT,
        LMP_ESCO_PACKET_TYPE_DEFAULT,
        LMP_RETX_EFFORT_DONT_CARE
    },

    /* T1 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x0008U,
        APPL_WIDEBAND_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_EV3 | LMP_ESCO_2_EV3 | LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_QUALITY
    },

    /* T2 */
    {
        /* For Narrow Band Speech */
        LMP_ESCO_TX_BANDWIDTH_DEFAULT,
        LMP_ESCO_RX_BANDWIDTH_DEFAULT,
        0x000DU,
        APPL_WIDEBAND_VOICE_SETTING_DEFAULT,
        (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5),
        LMP_RETX_EFFORT_QUALITY
    }
};

/* HFP eSCO Channel Default parameters */
DECL_STATIC HCI_SCO_IN_PARAMS * appl_hfp_esco_params[2U] =
{
    /* Default Narrowband (CVSD) parameter */
    &appl_hfp_esco_cvsd_params[0U],

    /* Default Wideband (mSBC) parameter */
    &appl_hfp_esco_msbc_params[0U]
};

/* ----------------------------------------- Static Global Variables */

/* SDP Handle */

DECL_STATIC SDP_HANDLE hfp_sdp_handle;

DECL_STATIC UCHAR hfp_remote_server_channel;

/* To store Volume and Gain */
DECL_STATIC UCHAR appl_speaker_volume[HFP_UNIT_MAX_CONNECTIONS][3U];
DECL_STATIC UCHAR appl_microphone_gain[HFP_UNIT_MAX_CONNECTIONS][3U];

/* Mark SLC procedures */
DECL_STATIC UCHAR appl_hfu_slc;

/* ------------------------------------------ Functions */

/* Basic HFP Operations */
void main_hfp_unit_operations (void)
{
    unsigned int              choice, menu_choice;
    int                       read_val;
    UINT32                    input_gain;
    UCHAR                     i;
    API_RESULT                api_retval;
    UCHAR                     dialeddigits[33U];
    UCHAR                     memid[4U];
    UCHAR                     gain[3U];
    UINT16                    hci_handle;
    HFP_UNIT_CHLD_RESP_RESULT ag_chld_values;
    HFP_UNIT_TWC_CONTROL_TYPE option;
    HFP_UNIT_CIND_TEST_RESULT cind_resp_result;
    HCI_SCO_IN_PARAMS         sco_params_in;
    UINT16                    hci_sco_handles[HCI_MAX_SCO_CHANNELS];
    UCHAR                     num_of_sco_handles;
    UINT16                    str_len,hfp_unit_feature;
    UCHAR                     hfp_unit_str[6];

    printf("%s\n", hfp_unit_options);


    BT_LOOP_FOREVER()
    {
        api_retval = API_SUCCESS;
        LOG_DEBUG("\n< ");
        scanf("%u", &choice);
        LOG_DEBUG("\n");
        fflush(stdin);

        menu_choice = choice;

        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            printf("%s\n", hfp_unit_options);
            break;

        case 2: /* Register BD_ADDR of peer device */
            LOG_DEBUG("Please enter BD ADDR of Audio Gateway\n");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_peer_bd_addr);
            break;

        case 3:/* Get Registered BD_ADDR of peer device */
            LOG_DEBUG("> Registered Peer BD_ADDR "\
            "%02X:%02X:%02X:%02X:%02X:%02X\n",
            appl_peer_bd_addr[0U],
            appl_peer_bd_addr[1U],
            appl_peer_bd_addr[2U],
            appl_peer_bd_addr[3U],
            appl_peer_bd_addr[4U],
            appl_peer_bd_addr[5U]);
            break;

        case 4: /* Initialise Hands-Free Unit */
            api_retval = BT_hfp_unit_init(appl_callback_registered_with_hfu);

            LOG_DEBUG("> API RETVAL BT_hfp_unit_init : 0x%04X\n",api_retval);

            if (API_SUCCESS != api_retval)
            {
                printf("> FAILED. Reason = 0x%04X\n", api_retval);
            }
            else
            {
                LOG_DEBUG("> HFP_UNIT Init Successful\n");

                BT_mem_set(&appl_speaker_volume[0], 0, sizeof(appl_speaker_volume[0]));
                BT_mem_set(&appl_microphone_gain[0], 0, sizeof(appl_microphone_gain[0]));

                for (i = 0U; i < HFP_UNIT_MAX_CONNECTIONS; i++)
                {
                    BT_str_n_copy(&appl_speaker_volume[i][0U], APPL_HFU_INITIAL_GAIN, (sizeof(appl_speaker_volume[i]) - 1));
                    BT_str_n_copy(&appl_microphone_gain[i][0U], APPL_HFU_INITIAL_GAIN, (sizeof(appl_microphone_gain[i]) - 1));
                }
            }
            break;

        case 5: /* Start Hands-Free Unit */
            LOG_DEBUG ("HF supported feature bitmap\n");
            LOG_DEBUG ("\tBit\tFeature\n");
            LOG_DEBUG ("\t---\t-------\n");
            LOG_DEBUG ("\t 0  \tEC and/or NR function\n");
            LOG_DEBUG ("\t 1  \tCall waiting or 3-way calling\n");
            LOG_DEBUG ("\t 2  \tCLI presentation capability\n");
            LOG_DEBUG ("\t 3  \tVoice recognition activation\n");
            LOG_DEBUG ("\t 4  \tRemote volume control\n");
            LOG_DEBUG ("\t 5  \tEnhanced call status\n");
            LOG_DEBUG ("\t 6  \tEnhanced call control\n");
            LOG_DEBUG ("\t 7  \tCodec negotiation\n");

#ifdef HFP_UNIT_1_7
            LOG_DEBUG ("\t 8  \tHF Indicators\n");
            LOG_DEBUG ("\t 9  \teSCO S4(T2) Settings Support\n");
#endif /* HFP_UNIT_1_7 */

#ifdef HFP_UNIT_1_8
            LOG_DEBUG ("\t 10 \tEnhanced Voice Recognition Status\n");
            LOG_DEBUG ("\t 11 \tVoice Recognition Text\n");
#endif /* HFP_UNIT_1_8 */

            LOG_DEBUG("Enter Local HFP_UNIT Supported Features(as Decimal value)\n:");
            scanf("%s", hfp_unit_local_supported_features);

            BT_mem_set(hfp_unit_str, 0x00, sizeof(hfp_unit_str));
            BT_str_n_copy(hfp_unit_str ,hfp_unit_local_supported_features, (sizeof(hfp_unit_str) - 1));
            str_len = (UINT16)BT_str_len(hfp_unit_str);

            hfp_unit_feature =  (UINT16)appl_str_to_num(hfp_unit_local_supported_features,str_len);

            for (i= 0; i< str_len; i++)
            {
                if ((!((hfp_unit_str[i]) >= '0' && (hfp_unit_str[i]) <= '9'))|| ( (hfp_unit_feature < HFP_UNIT_MIN_SUPP_FEATURE_RANGE ) || (hfp_unit_feature > HFP_UNIT_MAX_SUPP_FEATURE_RANGE) ) )
                {
                    LOG_DEBUG("Invalid Supported Features\n");
                    api_retval = API_FAILURE; break; /*  return; */
                }
            }

            if (API_SUCCESS == api_retval)
            {
                hfp_unit_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN - 1U] = '\0';

                /* Local Supported Features in Numeric Format */
                hfp_unit_local_supported_features_ext = appl_str_to_num
                                                        (
                                                            hfp_unit_local_supported_features,
                                                            (UINT16)BT_str_n_len(hfp_unit_local_supported_features, sizeof(hfp_unit_local_supported_features))
                                                        );

#ifdef HFP_UNIT_1_6
                LOG_DEBUG("Enter Local HFP_UNIT list of available Codecs\n");
                LOG_DEBUG("\t1   -> for CVSD\n");
                LOG_DEBUG("\t1,2 -> for CVSD, mSBC\n:");
                scanf("%s", hfp_unit_local_available_codecs);

                int var1 = BT_str_n_len(hfp_unit_local_available_codecs, sizeof(hfp_unit_local_available_codecs));
                if (var1 > 3)
                {
                    LOG_DEBUG("invalid Codecs parameters entered \n");
                    break;
                }
                else if (((hfp_unit_local_available_codecs[0] == '1') && (1 == var1)) ||
                        ((hfp_unit_local_available_codecs[0] == '1') && (hfp_unit_local_available_codecs[1] == ',') \
                          && (hfp_unit_local_available_codecs[2] == '2') && (3 == var1)))
                {
                    appl_hfp_unit_conf_params.supp_features = hfp_unit_local_supported_features;
                    appl_hfp_unit_conf_params.supp_features_len = (UINT16)BT_str_n_len(hfp_unit_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN);
                    appl_hfp_unit_conf_params.available_codecs = hfp_unit_local_available_codecs;
                    appl_hfp_unit_conf_params.available_codecs_len = (UINT16)BT_str_n_len(hfp_unit_local_available_codecs, (APPL_HFU_MAX_AVL_CODEC_ID_LIST_LEN + 1U));
                }
                else if ((hfp_unit_local_available_codecs[0] == '2') && (1 == var1))
                {
                    LOG_DEBUG("CVSD Codec selection is mandatory\n");
                    break;
                }
                else
                {
                    LOG_DEBUG("invalid Codecs Selected\n");
                    break;
                }

#ifdef HFP_UNIT_1_7
                LOG_DEBUG("Enter Local HFP_UNIT list of available HF Indicators \n");
                LOG_DEBUG("\t1    -> for Enhanced Safety \n");
                LOG_DEBUG("\t2    -> for Battery Level \n");
                LOG_DEBUG("\t1,2  -> for both Enhanced Safety and Battery Level\n");
                scanf("%s", hfp_unit_local_supported_hf_indicators);

                int var2 = BT_str_n_len(hfp_unit_local_supported_hf_indicators, sizeof(hfp_unit_local_supported_hf_indicators));
                if (var2 > 3)
                {
                    LOG_DEBUG("invalid indicator \n");
                    break;
                }
                else if (((hfp_unit_local_supported_hf_indicators[0] == '1') && (1 == var2)) ||
                        ((hfp_unit_local_supported_hf_indicators[0] == '1') && (hfp_unit_local_supported_hf_indicators[1] == ',') \
                           && (hfp_unit_local_supported_hf_indicators[2] == '2') && (3 == var2)) ||
                          ((hfp_unit_local_supported_hf_indicators[0] == '2') && (1 == var2)))
                {
                    appl_hfp_unit_conf_params.hf_indicators = hfp_unit_local_supported_hf_indicators;
                    appl_hfp_unit_conf_params.hf_indicators_len = (UCHAR)BT_str_n_len(hfp_unit_local_supported_hf_indicators, (APPL_HFP_UNIT_HF_INDICATOR_LIST_LEN + 1));
                }
                else
                {
                    LOG_DEBUG("invalid indicator Selected\n");
                    break;
                }

                /* Set the default */
                hfp_unit_peer_hf_ind_count = 0U;
#endif /* HFP_UNIT_1_7 */

                (BT_IGNORE_RETURN_VALUE)appl_start_hfu
                (
                    &appl_hfp_unit_conf_params
                );
#else /* HFP_UNIT_1_6 */
                appl_start_hfu
                (
                    hfp_unit_local_supported_features,
                    (UCHAR)BT_str_n_len(hfp_unit_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN)
                );
#endif /* HFP_UNIT_1_6 */
            }
            break;

        case 6: /* Stop Hands-Free Unit */
            api_retval = BT_hfp_unit_stop();

            if (HFP_UNIT_ERR_STOP_PENDING == api_retval)
            {
                LOG_DEBUG("> STOP_PENDING.........\n");
                LOG_DEBUG("> Wait for Confirmation..\n");
            }
            else
            {
                LOG_DEBUG("> API RETVAL Profile Stop 0x%04X\n", api_retval);
                (BT_IGNORE_RETURN_VALUE) BT_dbase_inactivate_record(hfp_hfu_record_handle);
            }
            break;

        case 7: /* Create ACL connection */
            api_retval =  BT_hci_create_connection
                          (
                              appl_peer_bd_addr,
                              LMP_ACL_DM1|LMP_ACL_DM3|LMP_ACL_DM5|
                              LMP_ACL_DH1|LMP_ACL_DH3|LMP_ACL_DH5,
                              0U, 0U, 0U,
                              0U
                          );

            if (API_SUCCESS == api_retval)
            {
                LOG_DEBUG("> HCI Create Connection Successful\n");
            }
            else
            {
                LOG_DEBUG("> - Error creating ACL , API RETVAL 0x%04X\n",
                api_retval);
            }
            break;

        case 8: /* Get AG's SDP record */
            /* Set the SDP Handle */
            SDP_SET_HANDLE(hfp_sdp_handle,appl_peer_bd_addr,appl_hfu_sdp_cb);

            /* Call SDP open */
            api_retval = BT_sdp_open(&hfp_sdp_handle);

            LOG_DEBUG("> API RETVAL BT_sdp_open : 0x%04X\n",api_retval);
            break;

        case 9: /* Establish SLC connection with AG */
            LOG_DEBUG("> Registered Peer BD_ADDR "\
            "%02X:%02X:%02X:%02X:%02X:%02X\n",
            appl_peer_bd_addr[0U],
            appl_peer_bd_addr[1U],
            appl_peer_bd_addr[2U],
            appl_peer_bd_addr[3U],
            appl_peer_bd_addr[4U],
            appl_peer_bd_addr[5U]);
            api_retval = BT_hfp_unit_connect
                         (
                             appl_peer_bd_addr,
                             hfp_remote_server_channel
                         );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_connect: 0x%04X\n",
            api_retval);
            break;

        case 10: /* Release SLC with AG */
            api_retval = BT_hfp_unit_disconnect(g_hfp_unit_handle);

            LOG_DEBUG("> API RETVAL BT_hfp_unit_disconnect 0x%04X\n",
            api_retval);

            break;

        case 11:/* Release ACL with AG */
            api_retval = BT_hci_get_acl_connection_handle
                         (
                             appl_peer_bd_addr,
                             &hci_handle
                         );

            if (API_SUCCESS == api_retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for HSP-Unit not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&choice);
                hci_handle = (UINT16)choice;
            }

            api_retval = BT_hci_disconnect
                         (
                             hci_handle,
                             0x13U
                         );

            LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", api_retval);
            break;

        case 19:
            /*create sco connection with custom eSCO parameter set using command 27*/
            appl_hfpunit_create_eSCO_connection();
            break;
        case 20: /*
                  * Establish Synchronous Connection (HV Packet types only)
                  * with AG
                  */
                 /* Fall Through */
        case 43: /* Transfer Call to HFP_UNIT */
            /* Synchronous Connection Input Parameters */
            api_retval = BT_hci_get_acl_connection_handle
                         (
                             appl_peer_bd_addr,
                             &hci_handle
                         );

            if (API_SUCCESS == api_retval)
            {
                sco_params_in.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
                sco_params_in.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
                sco_params_in.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
                sco_params_in.rtx_effort = LMP_RETX_EFFORT_DONT_CARE;
                sco_params_in.packet_type =
                                      (LMP_ESCO_HV1|LMP_ESCO_HV2|LMP_ESCO_HV3);

                /* Voice Setting for Silicon Wave Baseband */
                sco_params_in.voice_setting = LMP_VOICE_SETTING_DEFAULT;

                api_retval = BT_hci_setup_synchronous_connection
                             (
                                 hci_handle,
                                 &sco_params_in
                             );
            }

            if (API_SUCCESS != api_retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", api_retval);
            }

            LOG_DEBUG("> API RETVAL BT_hci_setup_synchronous_connection :"\
            "0x%04X\n", api_retval);
            break;

        case 21:
            /*
             * Establish Synchronous Connection (HV+EV Packet types only)
             * with AG
             */
            /* Synchronous Connection Input Parameters */
            api_retval = BT_hci_get_acl_connection_handle
                         (
                             appl_peer_bd_addr,
                             &hci_handle
                         );
            if (API_SUCCESS == api_retval)
            {
                sco_params_in.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
                sco_params_in.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
                sco_params_in.max_latency  = LMP_ESCO_MAX_LATENCY_DEFAULT;
                sco_params_in.rtx_effort   = LMP_RETX_EFFORT_DONT_CARE;
                sco_params_in.packet_type  = LMP_ESCO_PACKET_TYPE_DEFAULT;

                /* Voice Setting for Silicon Wave Baseband */
                sco_params_in.voice_setting = LMP_VOICE_SETTING_DEFAULT;

                api_retval = BT_hci_setup_synchronous_connection
                             (
                                 hci_handle,
                                 &sco_params_in
                             );
            }
            if (API_SUCCESS != api_retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", api_retval);
            }
            else
            {
                LOG_DEBUG("API RETVAL Establish Synchronous Connection"\
                "(HV+EV Packet types only) 0x%04X\n",api_retval);
            }
            break;

        case 22:
            /*
             * Establish Synchronous Connection (EV Packet types only)
             * with AG
             */
            /* Synchronous Connection Input Parameters */

            api_retval = BT_hci_get_acl_connection_handle
                         (
                             appl_peer_bd_addr,
                             &hci_handle
                         );

            if (API_SUCCESS == api_retval)
            {
                sco_params_in.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
                sco_params_in.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
                sco_params_in.max_latency  = LMP_ESCO_MAX_LATENCY_DEFAULT;
                sco_params_in.rtx_effort   = LMP_RETX_EFFORT_DONT_CARE;
                sco_params_in.packet_type  =
                (LMP_ESCO_EV3|LMP_ESCO_EV4|LMP_ESCO_EV5);

                /* Voice Setting for Silicon Wave Baseband */
                sco_params_in.voice_setting = LMP_VOICE_SETTING_DEFAULT;

                api_retval = BT_hci_setup_synchronous_connection
                             (
                                 hci_handle,
                                 &sco_params_in
                             );
            }

            if (API_SUCCESS != api_retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", api_retval);
            }
            else
            {
                LOG_DEBUG("API RETVAL Establish Synchronous Connection"\
                "(EV Packet types only) 0x%04X\n",api_retval);
            }
            break;

        case 23: /* Release Synchronous Connection with AG */
                 /* Fall Through */
        case 44: /* Transfer Call to AG */
            api_retval = BT_hci_get_sco_connection_handle
                         (
                             appl_peer_bd_addr,
                             hci_sco_handles,
                             &num_of_sco_handles
                         );

            /* If SCO/eSCO connection present, disconect the first one */
            if (API_SUCCESS == api_retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_sco_handles[0U]);
            }
            else
            {
                LOG_DEBUG ("SCO Connection for HFP-Unit not found\n");
                LOG_DEBUG("Enter SCO Handle\n");
                scanf("%x",&choice);
                hci_sco_handles[0U] = (UINT16)choice;
            }

            api_retval = BT_hci_disconnect
                         (
                             hci_sco_handles[0U],
                             0x13U
                         );

            LOG_DEBUG("> API RETVAL Transfer Call to AG : 0x%04X\n", api_retval);
            break;

        case 24:
            LOG_DEBUG("Select CVSD SCO parameter set (0-D0, 1-D1): ");
            scanf("%d", &choice);
            appl_hfp_esco_params[0U] = &appl_hfp_sco_cvsd_params[choice];
            (BT_IGNORE_RETURN_VALUE)appl_hci_set_esco_channel_parameters
            (
                BT_TRUE,
                appl_hfp_esco_params[0U]
            );
            break;

        case 25:
            LOG_DEBUG("Select CVSD eSCO parameter set (1-S1, 2-S2, 3-S3, 4-S4): ");
            scanf("%d", &choice);
            appl_hfp_esco_params[0U] = &appl_hfp_esco_cvsd_params[choice];
            (BT_IGNORE_RETURN_VALUE)appl_hci_set_esco_channel_parameters
            (
                BT_TRUE,
                appl_hfp_esco_params[0U]
            );
            break;

        case 26:
            LOG_DEBUG("Select mSBC eSCO parameter set (1-T1, 2-T2): ");
            scanf("%d", &choice);
            appl_hfp_esco_params[1U] = &appl_hfp_esco_msbc_params[choice];
            (BT_IGNORE_RETURN_VALUE)appl_hci_set_esco_channel_parameters
            (
                BT_TRUE,
                appl_hfp_esco_params[1U]
            );
            break;

        case 27:
            appl_hfpunit_change_esco_config();
            break;

        case 30:/* Dial with number */
            LOG_DEBUG("Enter the number to dial : ");
            scanf("%s",dialeddigits);

            for (i = 0U; i < BT_str_len(dialeddigits); i++)
            {
                if ((11U < BT_str_len(dialeddigits)) ||
                    !((dialeddigits[i] >= '0') && (dialeddigits[i] <= '9')))
                {
                    printf ("Invalid Dial Number Entered\n");
                    break;
                }
                else
                {
                    api_retval = BT_hfp_unit_dial
                                 (
                                     g_hfp_unit_handle,
                                     dialeddigits,
                                     (UCHAR)BT_str_len(dialeddigits)
                                 );
                     LOG_DEBUG("> API RETVAL BT_hfp_unit_dial: 0x%04X\n", api_retval);
                }
            }
            break;

        case 31: /* Memory Dialing */
            LOG_DEBUG("Input the Memory Location : ");
            scanf("%s", (char *)memid);

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
                             g_hfp_unit_handle,
                             memid,
                             (UCHAR)BT_str_len(memid)
                         );

            LOG_DEBUG("> API RETVAL BT_hfp_unit_memdial 0x%04X\n",
            api_retval);
            break;

        case 32: /* Last Number ReDial */
            api_retval = BT_hfp_unit_redial
                         (
                             g_hfp_unit_handle
                         );

            LOG_DEBUG("> API RETVAL BT_hfp_unit_redial : 0x%04X\n",
            api_retval);
            break;

        case 40: /* Accept Incoming Call */
            api_retval = BT_hfp_unit_callaccept
                         (
                             g_hfp_unit_handle
                         );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_callaccept : 0x%04X\n",
            api_retval);
            break;

        case 41: /* Reject Call */ /* Fall Through */
        case 42: /* Hang Up ongoing call */
            api_retval = BT_hfp_unit_callhangup
                         (
                             g_hfp_unit_handle
                         );

            LOG_DEBUG("> API RETVAL BT_hfp_unit_callhangup : 0x%04X\n",
            api_retval);
            break;

        case 45:/* Three Way Call (TWC) Options */
            /*
             * Take user input for the following
             * 0. Release All Call and set UDUB
             * 1. Release All Active Call and accepts held/waiting Call
             * 2. Place Active Call(if any) on hold and accepts
             *    held/waiting Call
             * 3. Add Call to the Conefrencing
             * 4. Connect two Call and disconnect
             * Send event  HFP_HFU_HOLD_CALL (with input value)
             */
            LOG_DEBUG(" 0. Release all Held Calls and set UUDB tone " \
                   "(Reject new incoming waiting call)\n");
            LOG_DEBUG(" 1. Release Active Calls and accept held/waiting call\n");
            LOG_DEBUG(" 2. Hold Active Call and accept already " \
                   "held/new waiting call\n");
            LOG_DEBUG(" 3. Conference all calls\n");
            LOG_DEBUG(" 4. Connect other calls and disconnect self from TWC\n");

            LOG_DEBUG("Your choice : ");
            scanf("%u", &choice);
            option = (UCHAR)choice;
            api_retval = BT_hfp_unit_control_multiparty_call
                         (
                             g_hfp_unit_handle,
                             option
                         );

            LOG_DEBUG("> API RETVAL BT_hfp_unit_control_multi_party_call"\
            "0x%04X\n", api_retval);
            break;

        case 50: /* Enable CLIP Notification */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_CLIP,
                             HFP_UNIT_ACTION_ENABLE
                         );
            LOG_DEBUG("> API RETVAL Enable CLIP 0x%04X\n",api_retval);
            break;

        case 51: /* Disable CLIP Notification */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_CLIP,
                             HFP_UNIT_ACTION_DISABLE
                         );
            LOG_DEBUG("> API RETVAL Disable CLIP 0x%04X\n",api_retval);
            break;

        case 52: /* Enable Call Waiting Notification */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_CCWA,
                             HFP_UNIT_ACTION_ENABLE
                         );
            LOG_DEBUG("> API RETVAL Enable CCWA 0x%04X\n",api_retval);
            break;

        case 53: /* Disable Call Waiting Notification */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_CCWA,
                             HFP_UNIT_ACTION_DISABLE
                         );
            LOG_DEBUG("> API RETVAL Disable CCWA 0x%04X\n",api_retval);
            break;

        case 54: /* Enable Voice Recognition */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_BVRA,
                             HFP_UNIT_ACTION_ENABLE
                         );
            LOG_DEBUG("> API RETVAL Enable BVRA 0x%04X\n",api_retval);
            break;

        case 55: /* Disable Voice Recognition */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_BVRA,
                             HFP_UNIT_ACTION_DISABLE
                         );
            LOG_DEBUG("> API RETVAL Disable BVRA 0x%04X\n",api_retval);
            break;

        case 56: /* Disable ECNR in AG */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_ECNR,
                             HFP_UNIT_ACTION_DISABLE
                         );
            LOG_DEBUG("> API RETVAL Disable ECNR 0x%04X\n",api_retval);
            break;

        case 57: /* Get Voice-tag Phone Number */
            api_retval = BT_hfp_unit_feature_control
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_FEATURE_BINP,
                             HFP_UNIT_ACTION_ENABLE
                         );
            LOG_DEBUG("> API RETVAL Get BINP 0x%04X\n",api_retval);
            break;

        case 60: /* Set Speaker Volume */
            LOG_DEBUG("Enter Speaker Volume\n");
            LOG_DEBUG("NOTE: Speaker Volume min value is 0, max value is 15\n");
            fflush(stdin);
            scanf("%s", (char *)gain);

            /* Remove the terminal '\n', if any */
            if ('\n' == gain[sizeof(gain) - 1U])
            {
                gain[sizeof(gain) - 1U] = '\0';
            }
            else if ('\n' == gain[BT_str_len(gain) - 1U])
            {
                gain[BT_str_len(gain) - 1U] = '\0';
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            input_gain = appl_str_to_num
                         (
                             gain,
                             (UINT16)BT_str_len(gain)
                         );

            if (15U < input_gain)
            {
                LOG_DEBUG("> Invalid value of Speaker Volume \n");
                break;
            }

            BT_str_copy(&appl_speaker_volume[g_hfp_unit_handle][0U], gain);

            api_retval = BT_hfp_unit_set_gain
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_DEVICE_SPEAKER,
                             appl_speaker_volume[g_hfp_unit_handle],
                             (UCHAR)BT_str_len(gain)
                         );
            LOG_DEBUG("> API RETVAL Set Volume 0x%04X\n",api_retval);
            sco_audio_set_speaker_volume_pl((UCHAR)(appl_str_to_num
                                                   (
                                                       appl_speaker_volume[g_hfp_unit_handle],
                                                       (UINT16)BT_str_len(appl_speaker_volume[g_hfp_unit_handle])
                                                   )));

            break;

        case 61: /* Set Microphone Gain */
            LOG_DEBUG("NOTE: Microphone Gain min value is 0, max value is 15\n");
            LOG_DEBUG("Enter Microphone Gain\n");
            fflush(stdin);
            scanf("%s", (char *)gain);

            /* Remove the terminal '\n', if any */
            if ('\n' == gain[sizeof(gain) - 1U])
            {
                gain[sizeof(gain) - 1U] = '\0';
            }
            else if ('\n' == gain[BT_str_len(gain) - 1U])
            {
                gain[BT_str_len(gain) - 1U] = '\0';
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            input_gain = appl_str_to_num
                         (
                             gain,
                             (UINT16)BT_str_len(gain)
                         );

            if (15U < input_gain)
            {
                LOG_DEBUG("> Invalid value of Microphone Gain \n");
                break;
            }

            BT_str_copy(&appl_microphone_gain[g_hfp_unit_handle][0U], gain);

            api_retval = BT_hfp_unit_set_gain
                         (
                             g_hfp_unit_handle,
                             HFP_UNIT_DEVICE_MICROPHONE,
                             appl_microphone_gain[g_hfp_unit_handle],
                             (UCHAR)BT_str_len(gain)
                         );
            LOG_DEBUG("> API RETVAL Set Gain 0x%04X\n",api_retval);
            sco_audio_set_microphone_gain_pl((UCHAR)(appl_str_to_num
                                                    (
                                                        appl_microphone_gain[g_hfp_unit_handle],
                                                        (UINT16)BT_str_len(appl_microphone_gain[g_hfp_unit_handle])
                                                    )));
            break;

        case 62:/* Get Speaker Volume & Microphone Gain */
            LOG_DEBUG("> Speaker Volume  : %s\n",
            appl_speaker_volume[g_hfp_unit_handle]);

            LOG_DEBUG("> Microphone Gain : %s\n",
            appl_microphone_gain[g_hfp_unit_handle]);

            break;

        case 70: /* Get peer Indicator status */
            api_retval = BT_hfp_unit_get_peer_indicator_status
                         (
                             g_hfp_unit_handle
                         );
            LOG_DEBUG("> API RETVAL get_peer_indicator_status: 0x%04X\n",
            api_retval);
            break;

        case 71:/* Get peer Indicator List */
            BT_mem_set(&cind_resp_result, 0x00, sizeof(AT_PARSER_CIND_TEST_RESULT));
            api_retval = BT_hfp_unit_get_peer_indicator_list
                         (
                             g_hfp_unit_handle,
                             &cind_resp_result
                         );
            LOG_DEBUG("> API RETVAL get_peer_indicator_list: 0x%04X\n",
            api_retval);
            LOG_DEBUG("> Indicator List:\n");
            LOG_DEBUG("    Service Max Value : %d\n"  \
                   "    Service Min Value : %d\n"  \
                   "    Call Max Value : %d\n"     \
                   "    Call Min Value : %d\n"     \
                   "    Callsetup Max Value : %d\n"\
                   "    Callsetup Min Value : %d\n"\
                   "    Signal Max Value : %d\n"   \
                   "    Signal Min Value : %d\n"   \
                   "    Roam Max Value : %d\n"     \
                   "    Roam Min Value : %d\n"     \
                   "    Battchg Max Value : %d\n"  \
                   "    Battchg Min Value : %d\n"  \
                   "    CallHeld Max Value : %d\n" \
                   "    CallHeld Min Value : %d\n",
                    cind_resp_result.indicator_value.service_max_value,
                    cind_resp_result.indicator_value.service_min_value,
                    cind_resp_result.indicator_value.call_max_value,
                    cind_resp_result.indicator_value.call_min_value,
                    cind_resp_result.indicator_value.callsetup_max_value,
                    cind_resp_result.indicator_value.callsetup_min_value,
                    cind_resp_result.indicator_value.signal_max_value,
                    cind_resp_result.indicator_value.signal_min_value,
                    cind_resp_result.indicator_value.roam_max_value,
                    cind_resp_result.indicator_value.roam_min_value,
                    cind_resp_result.indicator_value.battchg_max_value,
                    cind_resp_result.indicator_value.battchg_min_value,
                    cind_resp_result.indicator_value.callheld_max_value,
                    cind_resp_result.indicator_value.callheld_min_value);
            break;

        case 72: /* Get peer supported CHLD values */
            api_retval = BT_hfp_unit_get_peer_chld_values
                         (
                             g_hfp_unit_handle,
                             &ag_chld_values
                         );
            LOG_DEBUG("> API RETVAL get_peer_indicator_list: 0x%04X\n",
            api_retval);

            if (API_SUCCESS == api_retval)
            {
                if(0U == ag_chld_values.hold_count)
                {
                    LOG_DEBUG("> CHLD Not Supported by AG\n");
                }
                else
                {
                    LOG_DEBUG("> chld_value %s\n",
                    ag_chld_values.peer_hold_value);
                    LOG_DEBUG("> hold count %d\n",
                    ag_chld_values.hold_count);
                }
            }
            break;

         case 73: /* Send DTMF Code */
             LOG_DEBUG("Enter the DTMF Code : ");
             scanf("%s", dialeddigits);
             api_retval = BT_hfp_unit_send_dtmf
                          (
                              g_hfp_unit_handle,
                              dialeddigits,
                              (UCHAR)BT_str_len(dialeddigits)
                          );
             LOG_DEBUG("> API RETVAL BT_hfp_unit_send_dtmf: 0x%04X\n",
             api_retval);
             break;

        case 74: /* Use HF 1.5 Features */
        #ifdef HFP_UNIT_1_5
            appl_hfp_unit_1_5_choice_hdlr();
            printf("%s\n", hfp_unit_options);
        #else  /* HFP_UNIT_1_5 */
            LOG_DEBUG("HFP UNIT 1.5 Feature not enabled\n");
        #endif /* HFP_UNIT_1_5 */
            break;

        case 75: /* Use HF 1.6 Features */
        #ifdef HFP_UNIT_1_6
            appl_hfp_unit_1_6_choice_hdlr();
            printf("%s\n", hfp_unit_options);
        #else  /* HFP_UNIT_1_6 */
            LOG_DEBUG("HFP UNIT 1.6 Feature not enabled\n");
        #endif /* HFP_UNIT_1_6 */
            break;

        case 76: /* Send String to AG */

            BT_mem_set(send_string, 0, sizeof(send_string));
            LOG_DEBUG("Enter the string\n");
            scanf("%"FSTR(HFP_UNIT_MAX_COMMAND_LENGTH)"s",send_string);
            LOG_DEBUG("Enter string length\n");
            scanf("%d",&read_val);
            send_string_length = (UCHAR) read_val;

            api_retval = BT_hfp_unit_send_string
                         (
                              g_hfp_unit_handle,
                              send_string,
                              send_string_length
                         );

            LOG_DEBUG("> API RETVAL - BT_hfp_unit_send_string 0x%04X\n",
            api_retval);

            break;

        case 77: /* Send custom AT command to AG */
            {
            BT_mem_set(send_string, 0, sizeof(send_string));
            LOG_DEBUG("Enter the AT command\n");
            scanf("%"FSTR(HFP_UNIT_MAX_COMMAND_LENGTH)"s",send_string);
            BT_str_n_cat (send_string, "\r", 1U);
            send_string_length = (UCHAR)BT_str_n_len (send_string, sizeof(send_string));

            api_retval = BT_hfp_unit_send_string
                         (
                              g_hfp_unit_handle,
                              send_string,
                              send_string_length
                         );

            LOG_DEBUG("> API RETVAL - BT_hfp_unit_send_string 0x%04X\n",
            api_retval);
            }
            break;

        case 80: /* Abort HF Procedure */
            api_retval = BT_hfp_unit_abort
                         (
                             g_hfp_unit_handle
                         );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_abort 0x%04X \n",api_retval);
            break;

        case 90: /* List all BD_ADDR */
            for (i = 0U;i < HFP_UNIT_MAX_CONNECTIONS ; i++)
            {
                LOG_DEBUG("> Instance: 0x%02X "
                "BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n\n",
                i,
                appl_bd_addr[i][0U],
                appl_bd_addr[i][1U],
                appl_bd_addr[i][2U],
                appl_bd_addr[i][3U],
                appl_bd_addr[i][4U],
                appl_bd_addr[i][5U]);
            }
            break;

        case 91U :
            LOG_DEBUG("> Select HFP Unit Handle\n");
            scanf("%d", &read_val);

            /* Check Validity of HFP Unit Handle */
            if (read_val >= HFP_UNIT_MAX_CONNECTIONS)
            {
                LOG_DEBUG("Handle %d out of Range (0 - %d)\n",
                read_val, HFP_UNIT_MAX_CONNECTIONS);
                break;
            }

            g_hfp_unit_handle = (HFP_UNIT_HANDLE)read_val;
            break;

        case 92U :
            LOG_DEBUG("> Current HFP Unit Handle : %d\n",
            (unsigned int) g_hfp_unit_handle);
            break;

        case 93: /* Update HF indicator value */

#ifdef HFP_UNIT_1_7
            LOG_DEBUG (" 1. Send BIEV Command\n");
            LOG_DEBUG (" 2. Display Peer Supported HF indicators Status\n");
            LOG_DEBUG (" Select Option: ");
            scanf("%d",&read_val);

            if (1U == read_val)
            {
                int biev_option;

                LOG_DEBUG ("Select options below:\n");
                LOG_DEBUG("    0 -> To Disable Enhanced Safety. \n");
                LOG_DEBUG("    1 -> To Enable Enhanced Safety. \n");
                LOG_DEBUG("    2 -> Send Battery Level. \n");
                scanf("%d",&biev_option);

                if (0x00U == biev_option)
                {
                    BT_str_copy
                    (
                        send_string,
                        "1,0"
                    );
                }
                else if (0x01U == biev_option)
                {
                    BT_str_copy
                    (
                        send_string,
                        "1,1"
                    );
                }
                else if (0x02U == biev_option)
                {
                    UCHAR  batter_level[4U];
                    INT32 batter_level_val;

                    LOG_DEBUG ("Enter the battery level <= 100(in decimal format):");
                    scanf ("%s", batter_level);

                    BT_str_copy
                    (
                        send_string,
                        "2,"
                    );

                    BT_str_n_cat
                    (
                        send_string,
                        batter_level,
                        sizeof(batter_level)
                    );

                    batter_level_val = appl_str_to_num
                                       (
                                           batter_level,
                                           (UINT16)BT_str_n_len(batter_level, sizeof(batter_level))
                                       );

                    for (i = 0U; i < hfp_unit_peer_hf_ind_count; i++)
                    {
                        if (HFP_UNIT_HF_IND_ID_BATTERY_LEVEL == hfp_unit_peer_hf_ind_list_info[i].hf_ind_id)
                        {
                            hfp_unit_peer_hf_ind_list_info[i].hf_ind_val = (UCHAR )batter_level_val;
                        }
                    }
                }
                else
                {
                    LOG_DEBUG ("Invalide options\n");
                    break;
                }

                if ((0x00U == biev_option) || (0x01U == biev_option))
                {
                    for (i = 0U; i < hfp_unit_peer_hf_ind_count; i++)
                    {
                        if (HFP_UNIT_HF_IND_ID_ENHANCED_SAFETY == hfp_unit_peer_hf_ind_list_info[i].hf_ind_id)
                        {
                            hfp_unit_peer_hf_ind_list_info[i].hf_ind_val = (UCHAR )biev_option;
                        }
                    }
                }

                send_string_length = (UCHAR)BT_str_n_len (send_string, HFP_UNIT_MAX_COMMAND_LENGTH);

                api_retval =  BT_hfp_unit_update_hf_ind_value
                              (
                                  g_hfp_unit_handle,
                                  send_string,
                                  send_string_length
                              );

                LOG_DEBUG("BT_hfp_unit_update_hf_ind_value returned 0x%04X\n", api_retval);
            }
            else if (2U == read_val)
            {
                LOG_DEBUG ("No. of Indicators: 0x%02x\n", hfp_unit_peer_hf_ind_count);
                LOG_DEBUG ("HF indicators Info.\n");
                for (i = 0U; i < hfp_unit_peer_hf_ind_count; i++)
                {
                    LOG_DEBUG ("    ID    : 0x%08x\n", hfp_unit_peer_hf_ind_list_info[i].hf_ind_id);
                    LOG_DEBUG ("    Value : 0x%02x\n", hfp_unit_peer_hf_ind_list_info[i].hf_ind_val);
                }
            }
            else
            {
                LOG_DEBUG ("Invalid choice\n");
            }
#else
            LOG_DEBUG ("Please Enable HFP_UNIT_1_7 flag \n");
#endif /* HFP_UNIT_1_7 */

            break;

        case 94:

#ifdef HFP_UNIT_1_8
            LOG_DEBUG (" 1. Send AT+BRVA=2 Command\n");
            LOG_DEBUG (" Select Option: ");
            scanf ("%d", &read_val);

            if (1U == read_val)
            {
                api_retval = BT_hfp_unit_enable_enh_voice_recognition
                             (
                                 g_hfp_unit_handle
                             );
                LOG_DEBUG ("> API RETVAL Enable Enhanced Voice Recognition 0x%04X\n", api_retval);
            }
#else
            LOG_DEBUG ("Please Enable HFP_UNIT_1_8 flag \n");
#endif /* HFP_UNIT_1_8 */

        break;

        default:
            LOG_DEBUG("> Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

/* Create sco connection with custom eSCO parameter set using command 27 */
void appl_hfpunit_create_eSCO_connection(void)
{
    API_RESULT retval;
    UINT16 hci_handle;
    HCI_SCO_IN_PARAMS esco_params;

    /**
     * Establish Synchronous Connection with selected setting
     * with AG
     */
    retval = BT_hci_get_acl_connection_handle
                 (
                     appl_peer_bd_addr,
                     &hci_handle
                 );

        if (API_SUCCESS == retval)
        {
            appl_hci_get_esco_channel_parameters(&esco_params);

            LOG_DEBUG("> API RETVAL BT_hci_setup_synchronous_connection\n");

            retval = BT_hci_setup_synchronous_connection
                     (
                         hci_handle,
                         &esco_params
                     );
        }

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
        }
}

void appl_hfpunit_change_esco_config(void)
{
#ifdef BR_EDR_HCI
    int read_val;

    HCI_SCO_IN_PARAMS esco_params;
    UINT16  max_latency;
    UINT16  packet_type;
    UCHAR   rtx_effort;

    /*packet type input*/
    LOG_DEBUG("packet type (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    packet_type = (UINT16)read_val;

    /*latency input*/
    LOG_DEBUG("latency (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    max_latency = (UINT16)read_val;

    /*retransmission effort*/
    LOG_DEBUG("retransmission effort (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    rtx_effort = (UCHAR)read_val;

    appl_hci_get_esco_channel_parameters(&esco_params);
    esco_params.packet_type = packet_type;
    esco_params.max_latency = max_latency;
    esco_params.rtx_effort = rtx_effort;

    /* Update the eSCO channel paramters for Default mSBC Codec */
    (BT_IGNORE_RETURN_VALUE)appl_hci_set_esco_channel_parameters
    (
        BT_TRUE,
        &esco_params
    );

#else
    LOG_DEBUG("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

/* HFP 1.5 Operations */
#ifdef HFP_UNIT_1_5
void appl_hfp_unit_1_5_choice_hdlr( void )
{
    API_RESULT                   retval;
    HFP_UNIT_HELD_INCALL_OPTIONS option;
    HFP_UNIT_ADV_CALL_HOLD       option1;
    int                          choice, menu_choice;

    option1.call_id = 0U;

    LOG_DEBUG("%s\n", hfp_unit_1_5_options);

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("\n< ");
        scanf("%d", &choice);
        menu_choice = choice;
        LOG_DEBUG("\n");
        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            LOG_DEBUG("%s\n", hfp_unit_1_5_options);
            break;

        case 2:/* Request AG Subscriber Number */
            retval = BT_hfp_unit_request_subscriber_num
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_request_subscriber_num "\
            "0x%04X\n",retval);
            break;

        case 3:/* Query Response And Hold Status */
            retval = BT_hfp_unit_query_resp_hold_status
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_query_resp_hold_status 0x%04X\n",
            retval);
            break;

        case 4:/* Set Network Name Format */
            retval = BT_hfp_unit_set_network_name_format\
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> BT_hfp_unit_set_network_name_format 0x%04X\n",retval);
            break;

        case 5:/* Query Operator Selection */
            retval = BT_hfp_unit_query_operator_selection
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_query_operator_selection "\
            "0x%04X\n",retval);
            break;

        case 6:/* Query List of Current Calls */
            retval = BT_hfp_unit_query_list_current_calls
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_get_list_current_calls"\
            " 0x%04X\n", retval);
            break;

        case 7:/* Send Resposne and Hold to AG */
            LOG_DEBUG("Enter Response and Hold Option\n");
            LOG_DEBUG("0. INCALL HOLD\n");
            LOG_DEBUG("1. INCALL ACCEPT\n");
            LOG_DEBUG("2. INCALL REJECT\n");
            scanf("%d", &choice);
            option = (UCHAR)choice;

            retval = BT_hfp_unit_send_resp_hold_values
                     (
                         g_hfp_unit_handle,
                         option
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_send_resp_hold_values 0x%04X\n",
            retval);
            break;

        case 8:/* Send Call Hold with Call ID */
            LOG_DEBUG("Enter Call ID\n");
            fflush(stdin);
            scanf("%c", &option1.call_id);

            /*
             * Take user input for the following
             * 0. Release All Call and set UDUB
             * 1. Release All Active Call and accepts held/waiting Call
             * 2. Place Active Call(if any) on hold and accepts
             *    held/waiting Call
             * 3. Add Call to the Conefrencing
             * 4. Connect two Call and disconnect
             * Send event  HFP_HFP_UNIT_HOLD_CALL (with input value)
             */
            /*
             * At the moment the Specification allows for only choices
             * 1 and 2
             */
            LOG_DEBUG(" 1. Release Active Calls and accept held/waiting call\n");
            LOG_DEBUG(" 2. Hold Active Call and accept already " \
                   "held/new waiting call\n");

            LOG_DEBUG("Your choice : ");
            fflush(stdin);
            scanf("%c", &option1.option);

            if ( (option1.option < '0') || (option1.option > '3') )
            {
                LOG_DEBUG(" Invalid Choice\n");
                break;
            }

            retval = BT_hfp_unit_send_adv_call_hold
                     (
                         g_hfp_unit_handle,
                         &option1
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_send_adv_call_hold 0x%04X\n",
            retval);
            break;

        case 9: /* Enable CMEE Error Reporting */
            retval = BT_hfp_unit_enable_cmee_reporting
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL CMEE Enable 0x%04X\n",retval);
            break;

        case 10: /* Disable CMEE Error Reporting */
            retval = BT_hfp_unit_disable_cmee_reporting
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> API RETVAL CMEE Disable 0x%04X\n",retval);
            break;

        default:
            LOG_DEBUG("Invalid Option: %d\n", choice);
            break;
        } /* switch */

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    } /* BT_LOOP_FOREVER() */

    return;
}
#endif /* HFP_UNIT_1_5 */

/* HFP 1.6 Operations */
#ifdef HFP_UNIT_1_6
void appl_hfp_unit_1_6_choice_hdlr( void )
{
    API_RESULT                   retval;
    int                          choice, menu_choice;

    LOG_DEBUG("%s\n", hfp_unit_1_6_options);

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("\n< ");
        scanf("%d", &choice);
        menu_choice = choice;
        LOG_DEBUG("\n");
        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            LOG_DEBUG("%s\n", hfp_unit_1_6_options);
            break;

        case 2:/* (De)Activate individual indicators */
            {
            /* Length = number of indicators * 2 i.e. one space for indicator,
             * one for comma. The terminating character is a \0 and not a comma.
             */
            UCHAR iia_string[HFP_UNIT_MAX_INDICATOR_COUNT<<1U] = "";
            LOG_DEBUG("\nEnter the indicator activation string sequence - ");
            scanf("%s", iia_string);

            retval = BT_hfp_unit_send_iia
                     (
                         g_hfp_unit_handle,
                         iia_string,
                         (UCHAR)BT_str_len(iia_string)
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_send_iia "\
            "0x%04X\n",retval);
            }
            break;

        case 3:/* (De)Activate individual indicators with activation bitmask */
            {
            UINT8 iia_bit_mask[((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U)] = {0U};
            UINT32 iia_bit_mask_length = 0U;
            UINT8 i;
            UINT32 temp;

            LOG_DEBUG("\nEnter IIA bit mask length (in number of bits) - ");
            scanf("%u", &iia_bit_mask_length);

            if (((iia_bit_mask_length+1U+7U)>>3U) > ((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U))
            {
                LOG_DEBUG("\nInvalid value entered.");
                break;
            }

            for(i = 0U; i < (iia_bit_mask_length+1U+7U)>>3U; i++)
            {
                LOG_DEBUG("\nEnter byte %d of the IIA bit mask (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                iia_bit_mask[i] = (UINT8)temp;
            }

            retval = BT_hfp_unit_iia_simple_bitmask
                     (
                         g_hfp_unit_handle,
                         iia_bit_mask,
                         (UINT8)iia_bit_mask_length
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_iia_simple_bitmask "\
            "0x%04X\n",retval);
            }
            break;

        case 4:/* (De)Activate individual indicators with new and previous activation bitmask */
            {
            UINT8 new_iia_bit_mask[((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U)]  = {0U};
            UINT8 curr_iia_bit_mask[((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U)] = {0U};
            UINT32 iia_bit_mask_length = 0U;
            UINT8 i;
            UINT32 temp;

            LOG_DEBUG("\nEnter IIA bit mask length (in number of bits) - ");
            scanf("%u", &iia_bit_mask_length);

            if (((iia_bit_mask_length+1U+7U)>>3U) > ((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U))
            {
                LOG_DEBUG("\nInvalid value entered.");
                break;
            }

            for(i = 0U; i < (iia_bit_mask_length+1U+7U)>>3U; i++)
            {
                LOG_DEBUG("\nEnter byte %d of the new IIA bit mask (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                new_iia_bit_mask[i] = (UINT8)temp;
            }

            for(i = 0U; i < (iia_bit_mask_length+1U+7U)>>3U; i++)
            {
                LOG_DEBUG("\nEnter byte %d of the current IIA bit mask (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                curr_iia_bit_mask[i] = (UINT8)temp;
            }

            retval = BT_hfp_unit_iia_new_and_curr_bitmask
                     (
                         g_hfp_unit_handle,
                         new_iia_bit_mask,
                         curr_iia_bit_mask,
                         (UINT8)iia_bit_mask_length
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_iia_new_and_curr_bitmask "\
            "0x%04X\n",retval);
            }
            break;

        case 5:/* (De)Activate individual indicators with required indicators and activation bitmask */
            {
            UINT8 iia_bit_mask[((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U)]     = {0U};
            UINT8 req_ind_bit_mask[((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U)] = {0U};
            UINT32 iia_bit_mask_length = 0U;
            UINT8 i;
            UINT32 temp;

            LOG_DEBUG("\nEnter IIA bit mask length (in number of bits) - ");
            scanf("%u", &iia_bit_mask_length);

            if (((iia_bit_mask_length+1U+7U)>>3U) > ((HFP_UNIT_MAX_INDICATOR_COUNT+7U)>>3U))
            {
                LOG_DEBUG("\nInvalid value entered.");
                break;
            }

            for(i = 0U; i < (iia_bit_mask_length+1U+7U)>>3U; i++)
            {
                LOG_DEBUG("\nEnter byte %d of the IIA bit mask (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                iia_bit_mask[i] = (UINT8)temp;
            }

            for(i = 0U; i < (iia_bit_mask_length+1U+7U)>>3U; i++)
            {
                LOG_DEBUG("\nEnter byte %d of the required indicators bit mask (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                req_ind_bit_mask[i] = (UINT8)temp;
            }

            retval = BT_hfp_unit_iia_actvn_and_req_ind_bitmask
                     (
                         g_hfp_unit_handle,
                         iia_bit_mask,
                         req_ind_bit_mask,
                         (UINT8)iia_bit_mask_length
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_iia_actvn_and_req_ind_bitmask "\
            "0x%04X\n",retval);
            }
            break;

        case 6:/* Inform AG about available codecs */
            {
            UCHAR avl_cdc_list[APPL_HFU_MAX_AVL_CODEC_ID_LIST_LEN] = "";
            LOG_DEBUG("Enter Local HFP_UNIT list of available Codecs\n");
            LOG_DEBUG("\t1   -> for CVSD\n");
            LOG_DEBUG("\t1,2 -> for CVSD, mSBC\n:");
            scanf("%s", avl_cdc_list);

            retval = BT_hfp_unit_send_avl_cdc_list
                     (
                         g_hfp_unit_handle,
                         avl_cdc_list,
                         (UCHAR)BT_str_len(avl_cdc_list)
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_send_avl_cdc_list 0x%04X\n",
            retval);
            }
            break;

        case 7:/* Inform AG about available codecs (numeric) */
            {
            UINT8 i;
            HFP_UNIT_CODEC_ID avl_cdc_list[HFP_UNIT_MAX_NUM_AVL_CODECS] = {0U};
            UINT32 avl_cdc_len = 0U;
            UINT32 temp;

            LOG_DEBUG("\nEnter the length of the available codecs list - ");
            scanf("%u", &avl_cdc_len);

            if (HFP_UNIT_MAX_NUM_AVL_CODECS < avl_cdc_len)
            {
                LOG_DEBUG("\nInvalid value entered.");
                break;
            }

            for (i = 0U; i < avl_cdc_len; i++)
            {
                LOG_DEBUG("\nEnter the Id of Codec %d (in hex) - 0x", i+1U);
                scanf("%x", &temp);
                avl_cdc_list[i] = (UINT8)temp;
            }

            retval = BT_hfp_unit_avl_cdc_list_num
                     (
                         g_hfp_unit_handle,
                         avl_cdc_list,
                         (UINT8)avl_cdc_len
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_avl_cdc_list_num 0x%04X\n",
            retval);
            }
            break;

        case 8:/* Trigger Codec Connection */
            retval = BT_hfp_unit_trigger_codec_connection
                     (
                         g_hfp_unit_handle
                     );
            LOG_DEBUG("> BT_hfp_unit_trigger_codec_connection 0x%04X\n",retval);
            break;

        case 9:/* Confirm Codec Selection */
            {
            UCHAR codec_id_str[APPL_HFU_MAX_DIGITS_CODEC_ID+1U] = "";

            LOG_DEBUG("\nEnter the Codec Id (in decimal, maximum permissable value is 255) - ");
            scanf("%s", codec_id_str);
            retval = BT_hfp_unit_send_codec_confirmation
                     (
                         g_hfp_unit_handle,
                         codec_id_str,
                         (UCHAR)BT_str_len(codec_id_str)
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_send_codec_confirmation "\
            "0x%04X\n",retval);
            }
            break;

        case 10:/* Confirm Codec Selection (numeric) */
            {
            UINT32 codec_id = 0U;

            LOG_DEBUG("\nEnter the Codec Id (in hex, maximum permissable value is 0xFF) - 0x");
            scanf("%x", &codec_id);
            retval = BT_hfp_unit_codec_confirmation_num
                     (
                         g_hfp_unit_handle,
                         (HFP_UNIT_CODEC_ID)codec_id
                     );
            LOG_DEBUG("> API RETVAL BT_hfp_unit_codec_confirmation_num "\
            "0x%04X\n",retval);
            }
            break;

        default:
            LOG_DEBUG("Invalid Option: %d\n", choice);
            break;
        } /* switch */

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    } /* BT_LOOP_FOREVER() */

    return;
}
#endif /* HFP_UNIT_1_6 */


/* Callback registered with HFP - profile */
API_RESULT appl_callback_registered_with_hfu
           (
               HFP_UNIT_HANDLE handle,   /* Connection Instance */
               HFP_UNIT_EVENTS event,    /* HFP Events          */
               API_RESULT      result,   /* Result of operation */
               void *          data,     /* Data for each event */
               UINT16          data_len  /* Length of Data      */
           )
{
    UCHAR                     * bd_addr;
    UCHAR                       index;
    APPL_PARSER_RESULT          app_parser_result;

    HFP_UNIT_CIND_READ_RESULT * cind_result;

    UCHAR                     * data_recvd;
    HFP_UNIT_APP_DATA         * data_to_app;

    API_RESULT                  retval;

#ifdef HFP_UNIT_1_6
    UCHAR esco_codec_id;
#endif /* HFP_UNIT_1_6 */

    data_to_app = data;

    bd_addr = NULL;
    retval = API_SUCCESS;

    /* Validate the connection instance */
    if (HFP_UNIT_MAX_CONNECTIONS <= handle)
    {
        LOG_DEBUG("Invalid Handle \n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Reset */
        BT_mem_set(&app_parser_result, 0, sizeof(APPL_PARSER_RESULT));

        switch( event )
        {
        case HFP_UNIT_CLI_DIGITS:           /* Fall Through */
        case HFP_UNIT_CALL_WAITING_IND:     /* Fall Through */
        case HFP_UNIT_VOICE_RECOG_IND:      /* Fall Through */
        case HFP_UNIT_BSIR_IND:             /* Fall Through */
        case HFP_UNIT_VGM_IND:              /* Fall Through */
        case HFP_UNIT_VGS_IND:              /* Fall Through */
        case HFP_UNIT_VOICETAG_PHNUM_IND:   /* Fall Through */
        case HFP_UNIT_RECVD_BTRH_IND:       /* Fall Through */
        case HFP_UNIT_REQ_SUB_NUM_IND:      /* Fall Through */
        case HFP_UNIT_COPS_QUERY_RESULT_IND:/* Fall Through */
        case HFP_UNIT_CURRENT_CALL_LIST_IND:/* Fall Through */
        case HFP_UNIT_CMEE_IND:             /* Fall Through */
    #ifdef HFP_UNIT_1_6
        case HFP_UNIT_BCS_IND:              /* Fall Through */
    #endif /* HFP_UNIT_1_6 */

            /**
             * These events require the utility function to extract data
             * from them. The rest of the functions have data in an easily
             * recognisable format. For the complete list of events and the
             * data associated with hem, please refer the HFP Unit API Document.
             */
            (BT_IGNORE_RETURN_VALUE) appl_extract_result
            (
                data_to_app->parser_resp,
                data_to_app->buffer,
                &app_parser_result
            );

            break;

        default:
            break;

        }

        switch(event)
        {

        /*
         * Profile Level Connection and Disconnection related Events.
         */
        case HFP_UNIT_CONNECT_CNF:
            LOG_DEBUG("\n> Event          : HFP_UNIT_CONNECT_CNF\n");
            LOG_DEBUG("> Instance       : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result   : 0x%04X\n",result);
            if (API_SUCCESS == result)
            {
                BT_mem_copy
                (
                    appl_bd_addr[handle],
                    data,
                    BT_BD_ADDR_SIZE
                );
                LOG_DEBUG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n",
                appl_bd_addr[handle][0U],
                appl_bd_addr[handle][1U],
                appl_bd_addr[handle][2U],
                appl_bd_addr[handle][3U],
                appl_bd_addr[handle][4U],
                appl_bd_addr[handle][5U]);

                BT_mem_copy
                (
                    appl_peer_bd_addr,
                    data,
                    BT_BD_ADDR_SIZE
                );

                /* Update the eSCO channel paramters for Default CVSD Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfp_esco_params[0U]
                );

                /* Set the handle to recently connected device */
                g_hfp_unit_handle = handle;

                /* Get peer supported features */
                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_get_peer_supported_features
                (
                    g_hfp_unit_handle,
                    &hfp_unit_peer_supported_features_ext
                );

                appl_hfu_slc = 1U;

                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_feature_control
                (
                    g_hfp_unit_handle,
                    HFP_UNIT_FEATURE_CLIP,
                    HFP_UNIT_ACTION_ENABLE
                );
            }
            break;

        case HFP_UNIT_CONNECT_IND:
            LOG_DEBUG("\n> Event          : HFP_UNIT_CONNECT_IND\n");
            LOG_DEBUG("> Instance       : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result   : 0x%04X\n",result);
            if (API_SUCCESS == result)
            {
                BT_mem_copy
                (
                    appl_bd_addr[handle],
                    data,
                    BT_BD_ADDR_SIZE
                );
                LOG_DEBUG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n",
                appl_bd_addr[handle][0U],
                appl_bd_addr[handle][1U],
                appl_bd_addr[handle][2U],
                appl_bd_addr[handle][3U],
                appl_bd_addr[handle][4U],
                appl_bd_addr[handle][5U]);

                BT_mem_copy
                (
                    appl_peer_bd_addr,
                    data,
                    BT_BD_ADDR_SIZE
                );

                /* Update the eSCO channel paramters for Default CVSD Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfp_esco_params[0U]
                );

                /* Set the handle to recently connected device */
                g_hfp_unit_handle = handle;

                /* Get peer supported features */
                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_get_peer_supported_features
                (
                    g_hfp_unit_handle,
                    &hfp_unit_peer_supported_features_ext
                );

                appl_hfu_slc = 1U;

                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_feature_control
                (
                    g_hfp_unit_handle,
                    HFP_UNIT_FEATURE_CLIP,
                    HFP_UNIT_ACTION_ENABLE
                );
            }
            break;

        case HFP_UNIT_DISCONNECT_CNF:
            LOG_DEBUG("\n> Event          : HFP_UNIT_DISCONNECT_CNF\n");
            LOG_DEBUG("> Instance       : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result   : 0x%04X\n",result);
            bd_addr = (UCHAR*)data;
            LOG_DEBUG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
            if (API_SUCCESS == result)
            {
                BT_mem_set(appl_bd_addr[handle], 0, BT_BD_ADDR_SIZE);
            }
            break;

       case HFP_UNIT_DISCONNECT_IND:
            LOG_DEBUG("\n> Event          : HFP_UNIT_DISCONNECT_IND\n");
            LOG_DEBUG("> Instance       : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result   : 0x%04X\n",result);
            bd_addr = (UCHAR*)data;
            LOG_DEBUG("> BD_ADDR of peer %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
            if (API_SUCCESS == result)
            {
                BT_mem_set(appl_bd_addr[handle], 0, BT_BD_ADDR_SIZE);
            }
            break;

        /*
         * Profile Indicators received.
         */

        case HFP_UNIT_CALL_ACTIVE:
            LOG_DEBUG("\n> Event    : HFP_UNIT_CALL_ACTIVE\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            break;
        case HFP_UNIT_NO_CALL:
            LOG_DEBUG("\n> Event    : HFP_UNIT_NO_CALL\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            break;

        case HFP_UNIT_CALLSETUP:
            LOG_DEBUG("\n> Event            : HFP_UNIT_CALLSETUP\n");
            LOG_DEBUG("> Instance         : 0x%02X\n", (unsigned int) handle);
            if (data != NULL)
            {
                LOG_DEBUG("> Call Setup Value : %d\n",*(UCHAR *)data);
            }
            sco_audio_play_ringtone_exit_pl();
            break;

        case HFP_UNIT_AG_SERVICE_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_AG_SERVICE_IND.\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Data Received : %d\n",*((UCHAR*)data));
            break;

        case HFP_UNIT_CIEV_SIGNAL_IND:
            LOG_DEBUG("\n> Event    : HFP_UNIT_CIEV_SIGNAL_IND.\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Value    : %d\n", *((UCHAR*)data));
            break;

        case HFP_UNIT_CIEV_ROAM_IND:
            LOG_DEBUG("\n> Event    : HFP_UNIT_CIEV_ROAM_IND.\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Value    : %d\n", *((UCHAR*)data));
            break;

        case HFP_UNIT_CIEV_BATTCHG_IND:
            LOG_DEBUG("\n> Event    : HFP_UNIT_CIEV_BATTCHG_IND.\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Value    : %d\n", *((UCHAR*)data));
            break;

        case HFP_UNIT_CALL_HELD_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_CALL_HELD_IND\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);
            data_recvd = (UCHAR*)data;
            LOG_DEBUG("> Data Received : %d\n", (*data_recvd));
            break;

        /*
         * Profile IND Events are handled below
         */
        case HFP_UNIT_INCALL_ALERT:
            LOG_DEBUG("\n> Event     : HFP_UNIT_INCALL_ALERT\n");
            LOG_DEBUG("> Instance  : 0x%02X\n", (unsigned int) handle);

            /* Indicate platform of ring */
            sco_audio_play_ringtone_pl();

            break;

        case HFP_UNIT_CLI_DIGITS:
            LOG_DEBUG("\n> Event    : HFP_UNIT_CLI_DIGITS\n");
            LOG_DEBUG("> Number   : %s\n",
            app_parser_result.result_param.cli_info.digits);
            if (0U != (BT_str_len(app_parser_result.result_param.cli_info.name)))
            {
                LOG_DEBUG("> Name : %s\n",
                app_parser_result.result_param.cli_info.name);
            }
            break;

        case HFP_UNIT_CALL_WAITING_IND:
            LOG_DEBUG("\n> Event           : HFP_UNIT_CALL_WAITING_IND\n");
            LOG_DEBUG("> Instance        : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Received Number : %s\n",\
                      app_parser_result.result_param.ccwa_result.number);
            LOG_DEBUG("> Number Type     : %d\n",\
                      app_parser_result.result_param.ccwa_result.type);
            LOG_DEBUG("> Voice Class     : %d\n",\
                      app_parser_result.result_param.ccwa_result.voice_class);
            LOG_DEBUG("> Operator Name   : %s\n",\
                      app_parser_result.result_param.ccwa_result.alpha);

            break;

        case HFP_UNIT_VOICE_RECOG_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_VOICE_RECOG_IND\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);

    #ifdef HFP_UNIT_1_8
            LOG_DEBUG ("> vrect : %d\n", \
                app_parser_result.result_param.bvra_result.vrect);

            if ((0U != (hfp_unit_local_supported_features_ext & APPL_HFP_UNIT_BRSF_CMD_FEATURE_BITMASK_ENH_VR_STATUS)) &&
                (0U != (hfp_unit_peer_supported_features_ext & APPL_HFP_UNIT_BRSF_RSP_FEATURE_BITMASK_ENH_VR_STATUS)))
            {
                /* Enhanced Voice Recognition related field */

                LOG_DEBUG ("> vrecstate : 0x%02x\n", \
                    app_parser_result.result_param.bvra_result.vrecstate);
            }

            if ((0U != (hfp_unit_local_supported_features_ext & APPL_HFP_UNIT_BRSF_CMD_FEATURE_BITMASK_VR_TEXT)) &&
                (0U != (hfp_unit_peer_supported_features_ext & APPL_HFP_UNIT_BRSF_RSP_FEATURE_BITMASK_VR_TEXT)))
            {
                /* Voice Recognition TextFeature related fields */

                LOG_DEBUG ("> text_id : 0x%04X\n", \
                    app_parser_result.result_param.bvra_result.text_rep.text_id);

                LOG_DEBUG ("> text_type : %d\n", \
                    app_parser_result.result_param.bvra_result.text_rep.text_type);

                LOG_DEBUG ("> text_operation : %d\n", \
                    app_parser_result.result_param.bvra_result.text_rep.text_op);

                LOG_DEBUG ("> String : %s\n", \
                    app_parser_result.result_param.bvra_result.text_rep.str);
            }
    #else
            LOG_DEBUG("> Data Received : %d\n",
            app_parser_result.result_param.uchar_result);
    #endif /* HFP_UNIT_1_8 */
            break;

        case HFP_UNIT_BSIR_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_BSIR_IND\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Data Received : %d\n",
            app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_VGM_IND:
            LOG_DEBUG("\n> Event        : HFP_UNIT_VGM_IND\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);

            /* Store the value of VGM */
            BT_mem_copy
            (
                appl_microphone_gain[ handle ],
                &data_to_app->buffer[ data_to_app->parser_resp->
                                    param[ 0U ].start_of_value_index],
                data_to_app->parser_resp->param[ 0U ].value_length
            );
            /* NULL terminate the str */
            appl_microphone_gain[ handle ][ data_to_app->
                                parser_resp->param[ 0U ].value_length] = '\0';

            sco_audio_set_microphone_gain_pl((UCHAR)(appl_str_to_num
                                                    (
                                                        appl_microphone_gain[g_hfp_unit_handle],
                                                        (UINT16)BT_str_len(appl_microphone_gain[g_hfp_unit_handle])
                                                    )));

            LOG_DEBUG("> Data Received : %s\n",appl_microphone_gain[ handle ]);
            break;

        case HFP_UNIT_VGS_IND:
            LOG_DEBUG("\n> Event        : HFP_UNIT_VGS_IND\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            /* Store the value of VGS */
            BT_mem_copy
            (
                appl_speaker_volume[ handle ],
                &data_to_app->buffer[ data_to_app->parser_resp->
                                    param[ 0U ].start_of_value_index],
                data_to_app->parser_resp->param[ 0U ].value_length
            );
            /* NULL terminate the str */
            appl_speaker_volume[ handle ][ data_to_app->
                               parser_resp->param[ 0U ].value_length] = '\0';

            sco_audio_set_speaker_volume_pl((UCHAR)(appl_str_to_num
                                                   (
                                                       appl_speaker_volume[g_hfp_unit_handle],
                                                       (UINT16)BT_str_len(appl_speaker_volume[g_hfp_unit_handle])
                                                   )));

            LOG_DEBUG("> Data Received : %s\n",appl_speaker_volume[ handle ]);
            break;

        case HFP_UNIT_VOICETAG_PHNUM_IND:
            LOG_DEBUG("\n> Event           : HFP_UNIT_VOICETAG_PHNUM_IND\n");
            LOG_DEBUG("> Instance        : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Received Number : %s\n",
            app_parser_result.result_param.digits);
            break;

        case HFP_UNIT_RECVD_BTRH_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_RECVD_BTRH_IND.\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Data Received : %d\n",
            app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_REQ_SUB_NUM_IND:
            LOG_DEBUG("\n> Event             : HFP_UNIT_REQ_SUB_NUM_IND\n");
            LOG_DEBUG("> Instance          : 0x%02X\n", (unsigned int) handle);

            LOG_DEBUG("> Subscriber Number : %s\n", \
                   app_parser_result.result_param.cnum_resp_result.number);
            LOG_DEBUG("> Number Type       : %d\n",\
                    app_parser_result.result_param.cnum_resp_result.type);
            LOG_DEBUG("> Service Class     : %d\n",\
                    app_parser_result.result_param.cnum_resp_result.service);
            break;

        case HFP_UNIT_COPS_QUERY_RESULT_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_COPS_QUERY_RESULT_IND\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);

            LOG_DEBUG("> Mode          : %d\n",\
                   app_parser_result.result_param.cops_resp_result.mode);
            LOG_DEBUG("> Format        : %d\n",\
                    app_parser_result.result_param.cops_resp_result.format);
            LOG_DEBUG("> Operator Name : %s\n",\
                    app_parser_result.result_param.cops_resp_result.op_name);

            break;

        case HFP_UNIT_CURRENT_CALL_LIST_IND:
            LOG_DEBUG("\n> Event         : HFP_UNIT_CURRENT_CALL_LIST_IND\n");
            LOG_DEBUG("> Instance      : 0x%02X\n", (unsigned int) handle);

            LOG_DEBUG("> ID            : %d\n",\
                      app_parser_result.result_param.clcc_resp_result.id);
            LOG_DEBUG("> DIR           : %d\n",\
                      app_parser_result.result_param.clcc_resp_result.dir);
            LOG_DEBUG("> Status        : %d\n",\
                      app_parser_result.result_param.clcc_resp_result.status);
            LOG_DEBUG("> Mode          : %d\n",\
                      app_parser_result.result_param.clcc_resp_result.mode);
            LOG_DEBUG("> Mprty         : %d\n",\
                      app_parser_result.result_param.clcc_resp_result.mprty);
            LOG_DEBUG("> Number        : %s\n",\
                      app_parser_result.result_param.clcc_resp_result.number);
            LOG_DEBUG("> Type          : %d\n",
                      app_parser_result.result_param.clcc_resp_result.type);
            LOG_DEBUG("> Operator Name : %s\n",\
                      app_parser_result.result_param.clcc_resp_result.alpha);

            break;

        case HFP_UNIT_PEER_IND_STATUS_IND:
            LOG_DEBUG("\n> Event    : HFP_UNIT_PEER_INDICATOR_STATUS_IND\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);

            cind_result = (HFP_UNIT_CIND_READ_RESULT*)data;

            LOG_DEBUG("> ID : Battchg %d\n",cind_result->battchg);
            LOG_DEBUG("> ID : Call %d\n",cind_result->call);
            LOG_DEBUG("> ID : Call_Setup %d\n",cind_result->call_setup);
            LOG_DEBUG("> ID : CallHeld %d\n", cind_result->callheld);
            LOG_DEBUG("> ID : Callsetup %d\n", cind_result->callsetup);
            LOG_DEBUG("> ID : Roam %d\n", cind_result->roam);
            LOG_DEBUG("> ID : Service %d\n", cind_result->service);
            LOG_DEBUG("> ID : Signal %d\n", cind_result->signal);
            break;

        case HFP_UNIT_AG_ERROR_IND:
            LOG_DEBUG("\n> Event     : HFP_UNIT_AG_ERROR_IND\n");
            LOG_DEBUG("> Instance  : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Error     : 0x%04X\n", result);
            break;

        case HFP_UNIT_CMEE_IND:
            LOG_DEBUG("\n> Event          : HFP_UNIT_CMEE_IND\n");
            LOG_DEBUG("> Instance       : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> CMEE ERR Value : %d\n",
            app_parser_result.result_param.uchar_result);
            break;

        case HFP_UNIT_RECVD_DATA_IND:
            LOG_DEBUG("\n> Event    : HFP_UNIT_RECVD_DATA_IND\n");
            LOG_DEBUG("> Instance : 0x%02X\n", (unsigned int) handle);
            if(data != NULL)
            {
                data_recvd = (UCHAR*)data;

                LOG_DEBUG("\n----------------CHAR DUMP-----------------------\n");
                for(index = 0U; index < data_len; index++ )
                {
                    LOG_DEBUG("%c ",data_recvd[index]);
                }
                LOG_DEBUG("\n------------------------------------------------\n");
                LOG_DEBUG("\n----------------HEX DUMP------------------------\n");
                for(index = 0U; index <data_len; index++ )
                {
                    LOG_DEBUG("%X ",data_recvd[index]);
                }
                LOG_DEBUG("\n------------------------------------------------\n");
            }
            break;

        /*
         * CNF Events are handled below.
         */
        case HFP_UNIT_SEND_DATA_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SEND_DATA_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_PEER_IND_STATUS_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_PEER_INDICATOR_STATUS_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_VOICETAG_PHNUM_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_VOICETAG_PHNUM_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_INCALL_ACCEPT_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_INCALL_ACCEPT_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_OUTCALL_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_OUTCALL_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_CALLHANGUP_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_CALLHANGUP_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_TWC_CALL_CTRL_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_TWC_CALL_CTRL_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_SET_VGM_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SET_VGM_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_SET_VGS_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SET_VGS_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            if (0U != appl_hfu_slc)
            {
                /* Update the microphone gain */
                BT_str_copy(&appl_microphone_gain[g_hfp_unit_handle][0U], APPL_HFU_INITIAL_GAIN);
                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_set_gain
                (
                    g_hfp_unit_handle,
                    HFP_UNIT_DEVICE_MICROPHONE,
                    appl_microphone_gain[g_hfp_unit_handle],
                    (UCHAR)BT_str_len(APPL_HFU_INITIAL_GAIN)
                );

                appl_hfu_slc = 0U;
            }
            break;

        case HFP_UNIT_CCWA_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_NOTIFICATION_CCWA_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            if (0U != appl_hfu_slc)
            {
                /* Update the speaker gain */
                BT_str_copy(&appl_speaker_volume[g_hfp_unit_handle][0U], APPL_HFU_INITIAL_GAIN);
                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_set_gain
                (
                    g_hfp_unit_handle,
                    HFP_UNIT_DEVICE_SPEAKER,
                    appl_speaker_volume[g_hfp_unit_handle],
                    (UCHAR)BT_str_len(APPL_HFU_INITIAL_GAIN)
                );
            }
            break;

        case HFP_UNIT_CLIP_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_NOTIFICATION_CLIP_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            if (0U != appl_hfu_slc)
            {
                /* Enable Call waiting */
                (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_feature_control
                (
                    g_hfp_unit_handle,
                    HFP_UNIT_FEATURE_CCWA,
                    HFP_UNIT_ACTION_ENABLE
                );
            }
            break;

        case HFP_UNIT_ECNR_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_ECNR_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_VREC_ENABLE_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_VREC_ENABLE_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

    #ifdef HFP_UNIT_1_8
        case HFP_UNIT_ENH_VREC_ENABLE_CNF:
            LOG_DEBUG ("\n> Event        : HFP_UNIT_ENH_VREC_ENABLE_CNF\n");
            LOG_DEBUG ("> Instance     : 0x%02X\n", (unsigned int)handle);
            LOG_DEBUG ("> Event result : 0x%04X\n", result);
            break;
    #endif /* HFP_UNIT_1_8 */

        case HFP_UNIT_VREC_DISABLE_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_VREC_DISABLE_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_SEND_DTMF_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SEND_DTMF_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_STOP_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_STOP_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            (BT_IGNORE_RETURN_VALUE) BT_dbase_inactivate_record(hfp_hfu_record_handle);
            break;

        case HFP_UNIT_CMEE_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_CMEE_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_REQ_SUB_NUM_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_REQ_SUB_NUM_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_SET_NW_NAME_FORMAT_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SET_NW_NAME_FORMAT_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_SEND_BTRH_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_SEND_BTRH_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_ADV_CALL_HOLD_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_ADV_CALL_HOLD_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_COPS_QUERY_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_COPS_QUERY_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_CURRENT_CALL_LIST_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_CURRENT_CALL_LIST_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

    #ifdef HFP_UNIT_1_6

        case HFP_UNIT_BIA_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_BIA_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_BAC_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_BAC_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_BCC_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_BCC_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_BCS_CNF:
            LOG_DEBUG("\n> Event        : HFP_UNIT_BCS_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);
            break;

        case HFP_UNIT_BCS_IND:
        {
            HCI_SCO_IN_PARAMS hfp_unit_esco_param;
#ifdef HCI_ENH_SCO
            HCI_ENH_SCO_PARAMS hfp_unit_enh_esco_param;
#endif /* HCI_ENH_SCO */
            LOG_DEBUG("\n> Event        : HFP_UNIT_BCS_IND\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            BT_mem_set(&hfp_unit_esco_param, 0, sizeof(HCI_SCO_IN_PARAMS));
#ifdef HCI_ENH_SCO
            BT_mem_set(&hfp_unit_enh_esco_param, 0, sizeof(HCI_ENH_SCO_PARAMS));
#endif /* HCI_ENH_SCO */

            /* Get Codec ID from event buffer */
            esco_codec_id = data_to_app->buffer[data_to_app->parser_resp->param->start_of_value_index] - '0';

            /*get default eSCO parameter set */
            appl_hci_get_esco_channel_parameters(&hfp_unit_esco_param);

            /* Based on Codec ID, update eSCO parameters */
            if (HFP_UNIT_CODEC_ID_CVSD == esco_codec_id)
            {
                LOG_DEBUG ("Codec Selection: CVSD\n");

                hfp_unit_esco_param.voice_setting = appl_hfp_esco_params[0U]->voice_setting;
                /* Update the eSCO channel parameters for CVSD Codec  */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    &hfp_unit_esco_param
                );
            }
            else if (HFP_UNIT_CODEC_ID_MSBC == esco_codec_id)
            {
                LOG_DEBUG ("Codec Selection: mSBC\n");
#ifdef HCI_ENH_SCO
                appl_hci_get_enh_esco_channel_parameters(&hfp_unit_enh_esco_param);
                hfp_unit_enh_esco_param.tx_coding_frmt.coding_format     = LMP_CODING_FRMT_MSBC;
                hfp_unit_enh_esco_param.rx_coding_frmt.coding_format     = LMP_CODING_FRMT_MSBC;
                hfp_unit_enh_esco_param.input_coding_frmt.coding_format  = LMP_CODING_FRMT_MSBC;
                hfp_unit_enh_esco_param.output_coding_frmt.coding_format = LMP_CODING_FRMT_MSBC;
                (BT_IGNORE_RETURN_VALUE)appl_hci_set_enh_esco_channel_parameters(&hfp_unit_enh_esco_param);
#endif /* HCI_ENH_SCO */
                hfp_unit_esco_param.voice_setting = appl_hfp_esco_params[1U]->voice_setting;
                /* Update the eSCO channel parameters for mSBC Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    &hfp_unit_esco_param
                );
            }
            else
            {
                LOG_DEBUG ("Codec Selection: ???\n");
            }

            /* Send codec confirmation */
            (BT_IGNORE_RETURN_VALUE) BT_hfp_unit_codec_confirmation_num(handle, esco_codec_id);
        }
        break;

    #endif /* HFP_UNIT_1_6 */

    #ifdef HFP_UNIT_1_7

        case HFP_UNIT_BIEV_CNF:

            LOG_DEBUG("\n> Event        : HFP_UNIT_BIEV_CNF\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            break;

        case HFP_UNIT_BIND_IND:

            LOG_DEBUG("\n> Event      : HFP_UNIT_BIND_IND\n");
            LOG_DEBUG("> Instance     : 0x%02X\n", (unsigned int) handle);
            LOG_DEBUG("> Event result : 0x%04X\n",result);

            hfp_unit_peer_hf_ind_read_info = (HFP_UNIT_BIND_READ_RESULT *)data;

            LOG_DEBUG ("Updated HF Indicator Info:\n");
            LOG_DEBUG ("    ID    : 0x%04x\n", hfp_unit_peer_hf_ind_read_info->hf_ind_id);
            LOG_DEBUG ("    Value : 0x%02x\n", hfp_unit_peer_hf_ind_read_info->hf_ind_val);

            /* Update Peer HF indicator status */
            for (index = 0U; index < hfp_unit_peer_hf_ind_count; index++)
            {
                if (hfp_unit_peer_hf_ind_list_info[index].hf_ind_id == hfp_unit_peer_hf_ind_read_info->hf_ind_id)
                {
                    hfp_unit_peer_hf_ind_list_info[index].hf_ind_val = hfp_unit_peer_hf_ind_read_info->hf_ind_val;
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
            if((HFP_UNIT_HF_IND_ID_ENHANCED_SAFETY == hfp_unit_peer_hf_ind_read_info->hf_ind_id) &&
               (0x01U == hfp_unit_peer_hf_ind_read_info->hf_ind_val))
            {
                BT_str_copy
                (
                    send_string,
                    "1,1"
                );
            }
#if 0
            else if((HFP_UNIT_HF_IND_ID_BATTERY_LEVEL == hfp_unit_peer_hf_ind_read_info->hf_ind_id) &&
                    ((hfp_unit_peer_hf_ind_read_info->hf_ind_val <= 100) &&
                     (hfp_unit_peer_hf_ind_read_info->hf_ind_val != 0)))
            {
                UCHAR  batt_level[4], length;

                appl_num_to_str
                (
                    hfp_unit_peer_hf_ind_read_info->hf_ind_val,
                    batt_level,
                    &length
                );

                BT_str_n_copy
                (
                    send_string,
                    "2,",
                    sizeof(send_string)
                );

                BT_str_n_cat
                (
                    send_string,
                    batt_level,
                    (sizeof(send_string) - BT_str_len(send_string))
                );
            }
#endif /* 0 */
            else
            {
                /* Not sending AT+BIEV Command to synchronize HF Unit & AG */
                break;
            }

            LOG_DEBUG ("Sending AT+BIEV Command to Synchronize HF Unit & AG\n");

            send_string_length = (UCHAR)BT_str_n_len(send_string, HFP_UNIT_MAX_COMMAND_LENGTH);

            retval =  BT_hfp_unit_update_hf_ind_value
                      (
                          g_hfp_unit_handle,
                          send_string,
                          send_string_length
                      );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed: BT_hfp_unit_update_hf_ind_value 0x%04X\n", retval);
            }

            break;
    #endif /* HFP_UNIT_1_7 */

        default:
            LOG_DEBUG("\n> UNKNOWN EVENT RECVD FROM PROFILE :0x%04X\n", event);
            retval = API_SUCCESS; /* return API_SUCCESS; */
        }

#ifdef HFP_UNIT_1_7
        if (((HFP_UNIT_CONNECT_IND == event) ||
             (HFP_UNIT_CONNECT_CNF == event)) &&
             (API_SUCCESS == result))
        {
            /**
             * After SLC procedur getting the peer supported HF indicators
             * and corresponding status
             */
            retval =  BT_hfp_unit_get_peer_hf_indicators
                      (
                          g_hfp_unit_handle,
                          hfp_unit_peer_hf_ind_list_info,
                          &hfp_unit_peer_hf_ind_count
                      );
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("=== Peer HF indicators info. ===\n");
                LOG_DEBUG ("No. of Indicators: 0x%02x\n", hfp_unit_peer_hf_ind_count);
                LOG_DEBUG ("HF indicators status:\n");
                for (index = 0U; index < hfp_unit_peer_hf_ind_count; index++)
                {
                    LOG_DEBUG ("    ID    : 0x%08x\n", hfp_unit_peer_hf_ind_list_info[index].hf_ind_id);
                    LOG_DEBUG ("    Value : 0x%02x\n", hfp_unit_peer_hf_ind_list_info[index].hf_ind_val);
                }
            }
        }
#endif /* HFP_UNIT_1_7 */
    }

    return retval;
}


/* SDP Callback for HFP */
void appl_hfu_sdp_cb
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    S_UUID  uuid;
    UINT16 num_uuids;
    UINT16  attrib_id[2U];
    UINT16 num_attribute_ids = 2U ;
    UCHAR *attribute_data = NULL;
    API_RESULT retval;

    UINT16 len_attrib_data;

    /* Init */
    retval = API_SUCCESS;

    /* Length of supported features */
    UINT16 support_len = 2U;

    BT_IGNORE_UNUSED_PARAM(length);

    /* Set AG related UUID variables */
    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = HFP_AG_AUDIO_GATEWAY_UUID;
    num_uuids = 0x01U;

    attrib_id[0U] = PROTOCOL_DESC_LIST;
    attrib_id[1U] = SUPPORTED_FEATURES_ATTR_ID;

    /* It can come only while creating connection. verify */
    if (API_SUCCESS != status)
    {
        LOG_DEBUG("> ** FAILED performing SDP Operation: %02X\n", command);
        LOG_DEBUG("> Return Value : 0x%04X\n", status);

        /* Implies that SDP open wa successful but service search failed */
        if(command == SDP_ServiceSearchAttributeResponse)
        {
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        switch(command)
        {
        case SDP_Open : /* SDP open callback */
        {
            attribute_data = BT_alloc_mem(100U);

            if(NULL == attribute_data)
            {
                LOG_DEBUG("> ** FAILED to Allocate memory for SDP Query\n");

                /* Close SDP */
                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);
            }
            else
            {
                len_attrib_data = 100U;

                /* Do Service Search Request */
                retval = BT_sdp_servicesearchattributerequest
                         (
                             &hfp_sdp_handle,
                             &uuid,
                             num_uuids,
                             attrib_id,
                             num_attribute_ids,
                             NULL,
                             0x00U,
                             attribute_data,
                             &len_attrib_data
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("> ** "\
                           "BT_sdp_servicesearchattributerequest Failed\n");

                    BT_free_mem(attribute_data);

                    LOG_DEBUG("> Closing SDP Connection\n");

                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);
                }
                /* SDP operation to continue */
            }

            break;
        }

        case SDP_Close: /* SDP Close callback */
        {
               break;
        }

        /* Service Search Attribute callback */
        case SDP_ServiceSearchAttributeResponse:
    #ifdef APPL_HFHS_DUMP
            BT_debug_dump_bytes_stdout(data, length);
    #endif /* APPL_HFHS_DUMP */

            /* From the response extract the server channel */
            retval = BT_sdp_get_channel_number
                     (
                         data,
                         &hfp_remote_server_channel
                     );

            LOG_DEBUG("> Remote SERVER CHANNEL 0x%02X\n",\
            hfp_remote_server_channel);

            if(API_SUCCESS != retval)
            {
                BT_free_mem(data);

                LOG_DEBUG("> ** FAILED to get Remote SERVER CHANNEL\n");

                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);
            }
            else
            {
                /* From the response extract the remote supported features */
                if( BT_sdp_get_attr_value
                    (
                        attrib_id[1U],
                        data,
                        (UCHAR *)&hfp_unit_peer_supported_features[0U],
                        &support_len
                    ) != API_SUCCESS )
                {
                    LOG_DEBUG("> ** FAILED to extract the remote supported"\
                    "features\n");
                }
                else
                {
                    LOG_DEBUG("> AG Supported features %X\n", \
                    hfp_unit_peer_supported_features[0U]);

                }

                /* Free allocated memory for attribute data */
                BT_free_mem(data);

                /* Close SDP */
                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);
            }

            break;


        case SDP_ErrorResponse:

            LOG_DEBUG("> ** ERROR occoured in SDP Query\n");

            /* Free allocated memory for attribute data */
            BT_free_mem(data);

            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&hfp_sdp_handle);

            break;

        default: /* Invalid: Nothing to do */
            LOG_DEBUG("Invalid Command Type: 0x%02X\n", command);
            break;

        }/* switch */
    }

    return;
}


/*
 * Utility function to extract values from parser results
 */

API_RESULT appl_extract_result
           (
               /* IN */  AT_PARSER_RESPONSE * parser_response,
               /* IN */  UCHAR              * buffer,
               /* OUT */ APPL_PARSER_RESULT * parse_result
           )
{
    UINT8      index;
    UINT32     temp_val;
    API_RESULT retval;

    index               = 0U;
    parse_result->token = parser_response->keyword_type;
    retval              = API_SUCCESS;

    switch(parser_response->keyword_type)
    {
        case AT_PL_CLIP_CL_:    /* <CRLF>+CLIP: */
        {
            BT_mem_copy
            (
                parse_result->result_param.cli_info.digits,
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );
            parse_result->result_param
            .cli_info.digits[parser_response->param[index++].value_length] =
                                                                     '\0';
            /* Name from PB */
            if (5U == parser_response->number_of_params)
            {
                /* Store Name */
                index = 4U;
                BT_mem_copy
                (
                    parse_result->result_param.cli_info.name,
                    &buffer[parser_response->param[index].
                                                start_of_value_index],
                    parser_response->param[index].value_length
                );
                parse_result->result_param
                .cli_info.name[parser_response->param[index++].value_length] =
                                                                         '\0';
            }
            else
            {
                parse_result->result_param.cli_info.name[0U] = '\0';
            }
        }
        break;

        case AT_PL_CCWA_CL_:    /* <CRLF>+CCWA: */
        {
            /* +CCWA: <number>, <type>, <class>[, <alpha>] */

            BT_mem_copy
            (
                parse_result->result_param.ccwa_result.number,
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );
            parse_result->result_param.ccwa_result
            .number[parser_response->param[index++].value_length] = '\0';

            if (parser_response->number_of_params == index)
            {
                break;
            }

            parse_result->result_param.ccwa_result.type =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            if (parser_response->number_of_params == index)
            {
                parse_result->result_param.ccwa_result.voice_class = 255U;
                parse_result->result_param.ccwa_result.alpha[0U] = '\0';
                break;
            }

            parse_result->result_param.ccwa_result.voice_class =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            /* Optional Param */
            if (MAX_CCWA_PARAMS == parser_response->number_of_params)
            {
                BT_mem_copy
                (
                    parse_result->result_param.ccwa_result.alpha,
                    &buffer[parser_response->param[index]
                                                    .start_of_value_index],
                    parser_response->param[index].value_length
                );
                parse_result->result_param.ccwa_result
                .alpha[parser_response->param[index++].value_length] = '\0';
            }
            else
            {
                parse_result->result_param.ccwa_result.alpha[0U] = '\0';
            }
        }
        break;

        case AT_PL_BINP_CL_:    /* <CRLF>+BINP: */

            BT_mem_copy
            (
                parse_result->result_param.digits,
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );
            parse_result->result_param
            .digits[parser_response->param[index++].value_length] = '\0';

        break;

        case AT_PL_COPS_CL_:    /* <CRLF>+COPS: */
        {
            /* +COPS:<mode>,[0,<operator>] */
            parse_result->result_param.cops_resp_result.mode =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            /* Optional Param */
            if (MAX_COPS_PARAMS == parser_response->number_of_params)
            {
                parse_result->result_param.cops_resp_result.format =
                (UCHAR )appl_str_to_num
                        (
                            &buffer[parser_response->param[index].start_of_value_index],
                            parser_response->param[index].value_length
                        );

                index ++;

                BT_mem_copy
                (
                    parse_result->result_param.cops_resp_result.op_name,
                    &buffer[parser_response->param[index].start_of_value_index],
                    parser_response->param[index].value_length
                );

                parse_result->result_param.cops_resp_result
                .op_name[parser_response->param[index++].value_length] = '\0';
            }
            else
            {
                parse_result->result_param.cops_resp_result.format =
                                                       INVALID_VALUE;
                parse_result->result_param.cops_resp_result.op_name[0U] = '\0';
            }
        }
        break;

        case AT_PL_CNUM_CL_:    /* <CRLF>+CNUM: */
        {
            index = 0U;
            BT_mem_copy
            (
                parse_result->result_param.cnum_resp_result.number,
                &buffer[parser_response->param[index]
                                                .start_of_value_index],
                parser_response->param[index].value_length
            );
            parse_result->result_param.cnum_resp_result
            .number[parser_response->param[index++].value_length] = '\0';

            if (parser_response->number_of_params == index)
            {
                break;
            }

            parse_result->result_param.cnum_resp_result.type =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            if (parser_response->number_of_params <= index)
            {
                break;
            }
            parse_result->result_param.cnum_resp_result.service =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            /* MISRA C - 2012 Rule 2.2 */
            /* index ++; */

        }
        break;

        case AT_PL_CLCC_CL_:    /* <CRLF>+CLCC: */
        {
            /*
             * +CLCC:<idx>,<dir>,<status>,<mode>,
             * <mprty>,<number>,<type>[,<alpha>]
             */
            parse_result->result_param.clcc_resp_result.id =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            parse_result->result_param.clcc_resp_result.dir =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            parse_result->result_param.clcc_resp_result.status =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            parse_result->result_param.clcc_resp_result.mode =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            parse_result->result_param.clcc_resp_result.mprty =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            if (parser_response->number_of_params == index)
            {
                parse_result->result_param.clcc_resp_result.number[0U] = '\0';

                parse_result->result_param.clcc_resp_result.alpha[0U] = '\0';

                break;
            }

            BT_mem_copy
            (
                parse_result->result_param.clcc_resp_result.number,
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );

            parse_result->result_param.clcc_resp_result
            .number[parser_response->param[index++].value_length] = '\0';

            parse_result->result_param.clcc_resp_result.type =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index ++;

            /* Optional Param */
            if (MAX_CLCC_PARAMS == parser_response->number_of_params)
            {
                BT_mem_copy
                (
                    parse_result->result_param.clcc_resp_result.alpha,
                    &buffer[parser_response->param[index]
                                                    .start_of_value_index],
                    parser_response->param[index].value_length
                );
                parse_result->result_param.clcc_resp_result
                .alpha[parser_response->param[index].value_length]='\0';
            }
            else
            {
                parse_result->result_param.clcc_resp_result.alpha[0U]='\0';
            }
        }
        break;

#ifdef HFP_UNIT_1_8
        case AT_PL_BVRA_CL_:      /* <CRLF>+BVRA: */      /* Fall Through */

            parse_result->result_param.bvra_result.vrect =
            (UCHAR )appl_str_to_num
                    (
                        &buffer[parser_response->param[index].start_of_value_index],
                        parser_response->param[index].value_length
                    );

            index++;

            if ((0U != (hfp_unit_local_supported_features_ext & 0x0400U)) &&
                (0U != (hfp_unit_peer_supported_features_ext & 0x1000U)) &&
                (parser_response->number_of_params > index))
            {
                parse_result->result_param.bvra_result.vrecstate =
                    (UCHAR)appl_str_to_num
                           (
                               &buffer[parser_response->param[index].start_of_value_index],
                               parser_response->param[index].value_length
                           );

                index++;
            }

            if ((0U != (hfp_unit_local_supported_features_ext & 0x0800U)) &&
                (0U != (hfp_unit_peer_supported_features_ext & 0x2000U)) &&
                (parser_response->number_of_params > index))
            {
                parse_result->result_param.bvra_result.text_rep.text_id =
                    (UINT16)appl_str_to_hexnum
                            (
                                &buffer[parser_response->param[index].start_of_value_index],
                                parser_response->param[index].value_length
                            );

                index++;

                parse_result->result_param.bvra_result.text_rep.text_type =
                    (UCHAR)appl_str_to_num
                           (
                               &buffer[parser_response->param[index].start_of_value_index],
                               parser_response->param[index].value_length
                           );

                index++;

                parse_result->result_param.bvra_result.text_rep.text_op =
                    (UCHAR)appl_str_to_num
                           (
                               &buffer[parser_response->param[index].start_of_value_index],
                               parser_response->param[index].value_length
                           );

                index++;

                BT_mem_copy
                (
                    parse_result->result_param.bvra_result.text_rep.str,
                    &buffer[parser_response->param[index].start_of_value_index],
                    parser_response->param[index].value_length
                );

                parse_result->result_param.bvra_result.text_rep
                    .str[parser_response->param[index].value_length] = '\0';
            }

        break;
#endif /* HFP_UNIT_1_8 */

        case AT_PL_VGS_CL_:       /* <CRLF>+VGS: */       /* Fall Through */
        case AT_PL_VGM_CL_:       /* <CRLF>+VGM: */       /* Fall Through */
        case AT_PL_VGS_EQ:        /* <CRLF>+VGS= */       /* Fall Through */
        case AT_PL_VGM_EQ:        /* <CRLF>+VGM= */       /* Fall Through */
        case AT_PL_BTRH_CL_:      /* <CRLF>+BTRH: */      /* Fall Through */
        case AT_PL_BSIR_CL_:      /* <CRLF>+BSIR: */      /* Fall Through */
#ifndef HFP_UNIT_1_8
        case AT_PL_BVRA_CL_:      /* <CRLF>+BVRA: */      /* Fall Through */
#endif /* HFP_UNIT_1_8 */
#ifdef HFP_UNIT_1_6
        case AT_PL_BCS_CL_:       /* <CRLF>+BCS: */       /* Fall Through */
#endif /* HFP_UNIT_1_6 */
        case AT_PL_CME_ERROR_CL_: /* <CRLF>+CME ERROR: */

            temp_val =
            appl_str_to_num
            (
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );
            parse_result->result_param.uchar_result =
                (UCHAR )((temp_val> 0xFFU) ? (0xFFU) : (temp_val));

            /* MISRA C - 2012 Rule 2.2 */
            /* index ++; */
        break;

#if 0
        case AT_PL_BIND_READ_ :  /* <CRLF>+BIND: <ind_id>, <ind_val>*/

            /* Indicator ID */
            parse_result->result_param.bind_info.hf_ind =
            appl_str_to_num
            (
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );

            index ++;

            /* Indicator Value */
            parse_result->result_param.bind_info.hf_ind_val =
            appl_str_to_num
            (
                &buffer[parser_response->param[index].start_of_value_index],
                parser_response->param[index].value_length
            );

        break;
#endif /* 0 */

    default:
        LOG_DEBUG("Invalid keyword type: %d\n", parser_response->keyword_type);
        break;
    } /* switch (keyword type) */

    return retval;
}


#ifndef HFP_UNIT_1_6
/* Start HFP UNIT */
API_RESULT appl_start_hfu
           (
               UCHAR  * hfp_unit_local_supported_features,
               UINT16 data_length
           )
{
    API_RESULT api_retval;

    /* local varibale to extract the supported features */
    UCHAR      attr_value[] = {0x09U, 0x00U, 0x00U};

     /* local supported features */
    UCHAR      supported_features;

    supported_features = 0x1FU;

    attr_value[1U] = hfp_unit_local_supported_features[0U];
    attr_value[2U] = hfp_unit_local_supported_features[1U];

    /* Get HFP_UNIT SDP record handle */
    BT_dbase_get_record_handle
    (
        DB_RECORD_HFU,
        0U,
        &hfp_hfu_record_handle
    );

    api_retval = BT_dbase_update_attr_value
                 (
                     hfp_hfu_record_handle,
                     0x0311U,
                     attr_value,
                     0x03U
                 );

    BT_dbase_get_server_channel
    (
        hfp_hfu_record_handle,
        PROTOCOL_DESC_LIST,
        &appl_hfp_unit_conf_params.server_channel
    );

    api_retval = BT_hfp_unit_start
                 (
                     appl_hfp_unit_conf_params.server_channel,
                     hfp_unit_local_supported_features,
                     data_length
                 );

    LOG_DEBUG("> API RETVAL BT_hfp_unit_start : 0x%04X\n", api_retval);

    if (API_SUCCESS == api_retval)
    {

        BT_dbase_activate_record(hfp_hfu_record_handle);

        LOG_DEBUG("> HF Profile Started Successfully\n");
    }

    return api_retval;
}
#else /* HFP_UNIT_1_6 */
API_RESULT appl_start_hfu
           (
               /* IN */  HFP_UNIT_APPL_CONFIG_PARAMS *p_hfp_unit_appl_conf_params
           )
{
    API_RESULT api_retval;

    /* local variable to extract the supported features */
    UCHAR      attr_value[] = {0x09U, 0x00U, 0x00U};

    /* local supported features */
    UINT32     supported_features;

    /**
     *  Map AT+BRSF supported features bits to SDP supported features bits.
     */
    supported_features = appl_str_to_num
                         (
                             p_hfp_unit_appl_conf_params->supp_features,
                             p_hfp_unit_appl_conf_params->supp_features_len
                         );

    /**
     * First 5 bit positions of AT+BRSF command value and
     * SDP supported feature value bits are same.
     */
    attr_value[2U] = (UCHAR)(supported_features & 0x1FU);

    /* Check if Codec Negotiation is supported */
    if (0U != (0x80U & supported_features))
    {
        /* Yes. Enable wideband speech bit (6th bit)*/
        attr_value[2U] |= 0x20U;
    }

    /* Get HFP_UNIT SDP record handle */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle
    (
        DB_RECORD_HFU,
        0U,
        &hfp_hfu_record_handle
    );

    api_retval = BT_dbase_update_attr_value
                 (
                     hfp_hfu_record_handle,
                     0x0311U,
                     attr_value,
                     0x03U
                 );

    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_server_channel
    (
        hfp_hfu_record_handle,
        PROTOCOL_DESC_LIST,
        &p_hfp_unit_appl_conf_params->server_channel
    );

    api_retval = BT_hfp_unit_start(p_hfp_unit_appl_conf_params);
    LOG_DEBUG("> API RETVAL BT_hfp_unit_start : 0x%04X\n", api_retval);

    if (API_SUCCESS == api_retval)
    {
        (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record(hfp_hfu_record_handle);

        LOG_DEBUG("> HF Profile Started Successfully\n");
    }

    return api_retval;
}
#endif /* HFP_UNIT_1_6 */

#endif /* HFP_UNIT */
