
/**
 *  \file appl_hfp_ag.c
 *
 *  This file contains the source code of HFP AG command line application.
 *  This application uses the standard I/O for user interface (MMI).
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#define HFP_AG_1_5_RSP

/* ------------------------------------------- Header File Inclusion */
#include "appl_hfp_ag.h"
#include "appl_hfp_ag_atrsp.h"
#include "appl_utils.h"

#include "appl_hci.h"
#include "hfp_ag_pl.h"

#ifdef HFP_AG

/* ------------------------------------------- External Global Variables */

/* ------------------------------------------- Exported Global Variables */

/* ------------------------------------------- Static Global Variables */
/* AG data*/
DECL_STATIC CHAR appl_hfag_phnum[16U];
DECL_STATIC CHAR appl_hfag_phnum_tag[16U];
DECL_STATIC CHAR appl_hfag_brsf[8U];
DECL_STATIC UCHAR appl_hfag_inband;
DECL_STATIC UCHAR appl_hfag_vgs;
DECL_STATIC UCHAR appl_hfag_vgm;
DECL_STATIC UCHAR appl_hfag_codec;
DECL_STATIC UCHAR appl_hfag_bind[2U];
DECL_STATIC UCHAR appl_hfag_codec_negotiate;
DECL_STATIC UCHAR appl_hfag_nrec;
DECL_STATIC UCHAR appl_hfag_dial;

DECL_STATIC UINT32 appl_hfu_brsf;

DECL_STATIC UCHAR appl_handle_ret = 1;

/* Response lookup table */
DECL_STATIC CHAR * appl_rsp_table[] = {
    HFAG_OK_STR,
    HFAG_ERROR_STR,
    HFAG_BRSF_STR,
    HFAG_BCS_STR,
    HFAG_BINP_STR,
    HFAG_VGS_STR,
    HFAG_VGM_STR,
    HFAG_BSIR_STR,
    HFAG_BIND_TEST_STR,
    HFAG_BIND_READ_STR,
    HFAG_BVRA_STR
};

void appl_hfag_read_pl(UCHAR * rsp, UINT16 rsplen);
void appl_hfag_call_status_pl(UCHAR status);

static const char hfp_ag_menu[] = "\n\
-------- HFP AG Menu ------- \n\
    0.  Exit \n\
    1.  Refresh menu \n\n\
    2.  Initialize HF AG \n\
    3.  Start HF AG \n\
    4.  Connect to Hands-Free Unit \n\
    5.  Disconnect From Hands-Free Unit \n\
    6.  Register Supported Features in SDP \n\
    7.  Stop HF Audio Gateway \n\
\n\
    10. Register BD_ADDR of peer(HFU) \n\
    11. Get HF-Unit SDP record \n\
    12. Create ACL Connection\n\
    13. Release ACL Connection\n\
\n\
    14. Establish Synchronous Connection with Unit \n\
    15. Release Synchronous Connection with Unit \n\
    16. Select CVSD SCO parameter set \n\
    17. Select CVSD eSCO parameter set \n\
    18. Select mSBC eSCO parameter set \n\
    19. Set eSCO parameter\n\
\n\
    20. Send AG string data\n\
    21. Send result codes to HFU \n\
\n\
    41. MemDial/Redial Support\n\
    42. ECNR Support\n\
    43. Send AG Enable Voice Recognition\n\
    44. Send AG Disable Voice Recognition\n\
    45. Send AG Enhanced Voice Recognition Msg.\n\
\n\
    60. Set InBand Ring\n\
    61. Set Default Codec\n\
    62. Set Bluetooth Indicator\n\
    63. Set Speaker Gain\n\
    64. Set Microphone Gain\n\
\n\
    65. Establish Synchronous Connection with Unit for custom eSCO parameter\n\
\n\
   100. Network Simulator \n\
\n\
Your Option -> \0";

static HFP_AG_EVENT_NOTIFY_CB appl_cb_ptr;
static UCHAR hfu_bd_addr[BT_BD_ADDR_SIZE];
static UCHAR appl_hfp_ag_state;
static UCHAR hfp_ag_local_supported_features[6U];
static UINT16 hfp_ag_local_supported_features_ext;
static UINT8 hfp_ag_server_channel;

/* HFU server channel */
static UINT8 hfp_ag_peer_server_channel;

/* SDP Handle */
DECL_STATIC SDP_HANDLE sdp_handle;

/* For PTS testing */
CHAR     appl_hfp_ag_data[128U];
UINT16   appl_hfp_ag_datalen;

/*
*  Default Voice Settings to be used for Wideband Speech Synchronous Connection:
*  Input Coding: A-Law
*  Input Format: 2's Compliment
*  Input Sample Size: 16-bit
*  Linear PCM Bit Position: 3
*  Air Coding Format: TRANSPARENT
*/
#define APPL_WIDEBAND_VOICE_SETTING_DEFAULT \
        (LMP_VOICE_INPUT_CODING_LINEAR|\
         LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT|\
         LMP_VOICE_INPUT_SAMPLE_16_BIT|\
         LMP_VOICE_LINEAR_BIT_POS_0|\
         LMP_VOICE_AIR_CODING_TRANSPARENT)

/* SCO-CVSD D0-D1 settings */
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfag_sco_cvsd_params[] =
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
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfag_esco_cvsd_params[] =
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
DECL_STATIC HCI_SCO_IN_PARAMS appl_hfag_esco_msbc_params[] =
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
DECL_STATIC HCI_SCO_IN_PARAMS * appl_hfag_esco_params[] =
{
    /* Default Narrowband (CVSD) parameter */
    &appl_hfag_esco_cvsd_params[0U],

    /* Default Wideband (mSBC) parameter */
    &appl_hfag_esco_msbc_params[0U]
};

void appl_send_rsp(UCHAR rsp_code, void * value);

/* Send queue for HFP AG */
DECL_STATIC APPL_HFAG_SENDELT appl_hfag_send[APPL_HFAG_SEND_QUEUE_SIZE];
DECL_STATIC UCHAR appl_hfag_sendwr;
DECL_STATIC UCHAR appl_hfag_sendrd;

/* ------------------------------------------- Functions */
/* main function for HFP AG operations */
void main_hfp_ag_operations (void)
{
    int read_val;
    int choice, menu_choice;
    API_RESULT retval;
    UCHAR supported_features;
    UINT16 hci_handle;
    UCHAR  str[5] = "";
    UINT16 length,i;

    BT_LOOP_FOREVER()
    {
        retval = API_SUCCESS;

        printf("%s",hfp_ag_menu);
        scanf ("%s", str);

        length = (UINT16)BT_str_len (str);
        for (i = 0; i < length; i++)
        {
            if (!((str[i]) >= '0' && (str[i]) <= '9'))
            {
                LOG_DEBUG ("Invalid option\n");
                retval = API_FAILURE; break; /* return; */
            }
        }

        if (API_SUCCESS == retval)
        {
            choice = appl_str_to_num(str,length);
            LOG_DEBUG("choice is %d\n",choice);
            menu_choice = choice;

            switch(choice)
            {
            case 0: /* exit */
                break; /* return; */

            case 1: /* Refresh */
                break;

            case 2: /* INIT HF AG */
                appl_cb_ptr = hfp_ag_appl_callback;
                retval = BT_hfp_ag_init(appl_cb_ptr);
                appl_handle_retval_from_hfag(retval);

                BT_mem_set(appl_hfag_phnum, 0, sizeof(appl_hfag_phnum));
                BT_mem_set(appl_hfag_brsf, 0, sizeof(appl_hfag_brsf));
                appl_hfag_vgs = 15U;
                appl_hfag_vgm = 15U;
                appl_hfag_codec = 1U;
                appl_hfag_nrec = 1U;
                BT_str_print(appl_hfag_phnum_tag, "%s", "+918067064000");

                /* Init the network simulator interface */
                hfp_ag_init_pl(appl_hfag_read_pl, appl_hfag_call_status_pl);

                break;

            case 3: /* Start HF AG */
                appl_start_hfp_ag();
                break;

            case 4: /* Connect to HFU */
                retval = BT_hfp_ag_connect
                         (
                             hfp_ag_peer_server_channel,
                             hfu_bd_addr
                         );
                appl_handle_retval_from_hfag(retval);

                break;

            case 5: /* Disconnect from HFU */
                retval = BT_hfp_ag_disconnect(hfu_bd_addr);
                appl_handle_retval_from_hfag(retval);
                break;

            case 6: /* Register Supported features */
                BT_mem_set(&supported_features, 0, sizeof(supported_features));
                LOG_DEBUG("Select Supported Features \n");
                register_features(supported_features);
                break;

            case 7: /* Stop HF AG */
                retval = BT_hfp_ag_stop();
                appl_handle_retval_from_hfag(retval);
                break;

            case 10: /* Register BD_ADDR of HFU */
                LOG_DEBUG ("Enter HFP Unit Bluetooth Device Address: ");
                appl_get_bd_addr(hfu_bd_addr);
                appl_handle_retval_from_hfag(retval);
                break;

            case 11: /* Get HFU's SDP record */
                /* Set the SDP Handle */
                SDP_SET_HANDLE(sdp_handle,hfu_bd_addr,appl_hfag_sdp_cb);

                /* Call SDP open */
                retval = BT_sdp_open(&sdp_handle);

                LOG_DEBUG("> API RETVAL BT_sdp_open : 0x%04X\n",retval);
                break;

            case 12:/*create ACL Connection*/
                retval =  BT_hci_create_connection
                          (
                              hfu_bd_addr,
                              LMP_ACL_DM1|LMP_ACL_DM3|LMP_ACL_DM5|
                              LMP_ACL_DH1|LMP_ACL_DH3|LMP_ACL_DH5,
                              0U, 0U, 0U,
                              1U
                          );

                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("> HCI Create Connection Successful\n");
                }
                else
                {
                    LOG_DEBUG("> - Error creating ACL , API RETVAL 0x%04X\n",
                        retval);
                }
                break;

            case 13:/*release ACL connection*/
                retval = BT_hci_get_acl_connection_handle
                         (
                             hfu_bd_addr,
                             &hci_handle
                         );

                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                    hci_handle);
                }
                else
                {
                    LOG_DEBUG ("ACL Connection for HSP-AG not found\n");
                    LOG_DEBUG("Enter ACL Handle\n");
                    scanf("%x",&choice);
                    hci_handle = (UINT16)choice;
                }

                retval = BT_hci_disconnect
                         (
                             hci_handle,
                             0x13U
                         );

                LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
                break;

            case 14:
                if (0U != appl_hfag_codec_negotiate)
                {
                    appl_send_rsp(HFAG_BCS, NULL);
                }
                else
                {
                    appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
                }
                break;

            case 15: /* Release Synchronous Connection with AG */
                appl_hfag_close_voice_channel();
                break;

            case 16:
                LOG_DEBUG("Select CVSD SCO parameter set (0-D0, 1-D1): ");
                scanf("%d", &choice);
                appl_hfag_esco_params[0U] = &appl_hfag_sco_cvsd_params[choice];

                /* Update the SCO channel paramters for Default CVSD Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfag_esco_params[0U]
                );

                break;

            case 17:
                LOG_DEBUG("Select CVSD eSCO parameter set (0-Default, 1-S1, 2-S2, 3-S3, 4-S4): ");
                scanf("%d", &choice);
                appl_hfag_esco_params[0U] = &appl_hfag_esco_cvsd_params[choice];

                /* Update the eSCO channel paramters for Default CVSD Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfag_esco_params[0U]
                );
                break;

            case 18:
                LOG_DEBUG("Select mSBC eSCO parameter set (0-Default, 1-T1, 2-T2): ");
                scanf("%d", &choice);
                appl_hfag_esco_params[1U] = &appl_hfag_esco_msbc_params[choice];

                /* Update the eSCO channel paramters for Default mSBC Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfag_esco_params[1U]
                );
                break;

            case 19U:
                appl_hfpag_change_esco_config();
                break;

            case 20U:
                appl_hfp_ag_send_data();
                break;

            case 21: /* Send result code */
                LOG_DEBUG("%s", AT_cmd_menu);
                scanf("%d", &read_val);
                if (read_val > 2U)
                {
                    appl_hfag_read_pl
                    (
                        (UCHAR*)at_cmd_option[read_val],
                        (UCHAR)BT_str_len(at_cmd_option[read_val])
                    );
                }
                else
                {
                    LOG_DEBUG(" User Choice is not allowed. Please \n");
                    LOG_DEBUG(" enter a choice greater than 2 \n");
                }
                break;

            case 41:
                LOG_DEBUG("Enter Memdial/Redial support (1-Yes, 0-No): ");
                scanf("%d", &choice);
                appl_hfag_dial = (UCHAR)choice;
                break;

            case 42:
                LOG_DEBUG("Enter ECNR support (1-Yes, 0-No): ");
                scanf("%d", &choice);
                appl_hfag_nrec = (UCHAR)choice;
                break;

            case 43:
                appl_hfp_ag_send_enable_voice_recognition ();
                break;

            case 44:
                appl_hfp_ag_send_disable_voice_recognition ();
                break;

            case 45:
#ifdef HFP_AG_1_8
                appl_hfp_ag_send_enh_voice_recognition ();
#else
                LOG_DEBUG ("Please enable HFP_AG_1_8 Flag \n");
#endif /* HFP_AG_1_8 */
                break;

            case 60:
                LOG_DEBUG("Setup Inband ring (1/0): ");
                scanf("%d", &choice);
                if ((choice < 0) || (choice > 1))
                {
                    appl_handle_ret = 0;
                    LOG_DEBUG("Invalid choice \n");
                    break;
                }

                appl_hfag_inband = (UCHAR)choice;
                appl_hfag_read_pl
                (
                    (UCHAR*)at_cmd_option[7U - choice],
                    (UCHAR)BT_str_len(at_cmd_option[7U - choice])
                );

                break;

            case 61:
                LOG_DEBUG("Enter Default Codec (1 - CVSD, 2 - mSBC):");
                scanf("%d", &choice);

                if (appl_hfag_codec != choice)
                {
                    appl_hfag_codec = (UCHAR)choice;
                    appl_hfag_codec_negotiate = 1U;

                    /* Update the eSCO channel paramters for the Codec */
                    (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                    (
                        BT_TRUE,
                        appl_hfag_esco_params[choice - 1U]
                    );
                }
                break;

            case 62:
                LOG_DEBUG("Bluetooth Indicators:\n");
                LOG_DEBUG("1. Enhanced Safety\n");
                LOG_DEBUG("2. Battery Level\n");
                LOG_DEBUG("Enter option: ");
                scanf("%d", &choice);
                choice--;

                LOG_DEBUG("Enter Activation (1-Enable, 0-Disable): ");
                scanf("%d", &read_val);
                appl_hfag_bind[choice] = (UCHAR)read_val;

                appl_send_rsp(HFAG_BIND_READ, &choice);

                break;

            case 63:
                if (0U == ((appl_hfu_brsf & 0x0010U)))
                {
                    LOG_DEBUG("Peer does not support Volume Control\n");
                    break;
                }

                LOG_DEBUG("Enter Speaker Gain (0-15): ");
                scanf("%d", &read_val);
                appl_hfag_vgs = (UCHAR)read_val;

                appl_send_rsp(HFAG_VGS, NULL);
                break;

            case 64:
                if (0U == ((appl_hfu_brsf & 0x0010U)))
                {
                    LOG_DEBUG("Peer does not support Volume Control\n");
                    break;
                }

                LOG_DEBUG("Enter Speaker Gain (0-15): ");
                scanf("%d", &read_val);
                appl_hfag_vgm = (UCHAR)read_val;

                appl_send_rsp(HFAG_VGM, NULL);
                break;

            case 65:
                appl_hfpag_create_eSCO_connection();
                break;

            case 100:
                hfp_ag_access_pl();
                break;

            default:
                LOG_DEBUG("Invalid Choice: %d\n", choice);
                appl_handle_ret = 0;
                break;
            }/* End Switch */

            if (0 == menu_choice)
            {
                /* return */
                break;
            }

            if (appl_handle_ret == 1)
            {
                appl_handle_retval_from_hfag (retval);
            }
            appl_handle_ret = 1;
        } /* if (API_SUCCESS == retval) */
    } /* End while */
}

void appl_send_rsp(UCHAR rsp_code, void * value)
{
    CHAR response[256U];
    UCHAR length;

    /* Initialize the response */
    BT_mem_set(response, 0, sizeof(response));

    /* Copy the base response and length from the response table */
    BT_str_n_copy(response, appl_rsp_table[rsp_code], (sizeof(response) - 1));
    length = (UCHAR)BT_str_len(appl_rsp_table[rsp_code]);

    switch (rsp_code)
    {
        case HFAG_OK:
            break;

        case HFAG_ERROR:
            break;

        case HFAG_BRSF:
            BT_str_print((&response[length]), "%s\r\n", appl_hfag_brsf);
            break;

        case HFAG_BCS:
            BT_str_print((&response[length]), "%d\r\n", appl_hfag_codec);
            break;

        case HFAG_BINP:
            BT_str_print((&response[length]), "%s\r\n", appl_hfag_phnum_tag);
            break;

        case HFAG_VGS:
            BT_str_print((&response[length]), "%d\r\n", appl_hfag_vgs);
            break;

        case HFAG_VGM:
            BT_str_print((&response[length]), "%d\r\n", appl_hfag_vgm);
            break;

        case HFAG_BSIR:
            BT_str_print((&response[length]), "%d\r\n", appl_hfag_inband);
            break;

        case HFAG_BIND_TEST:
            break;

        case HFAG_BIND_READ:
            BT_str_print((&response[length]), "%d, %d\r\n", (*(UCHAR *)value + 1U), appl_hfag_bind[*(UCHAR *)value]);
            break;

        default:
            LOG_DEBUG("Invalid Response Code: 0x%02X\n", rsp_code);
            break;
    }

    /* Update length and send response to interface */
    length = (UCHAR)BT_str_len(response);
    appl_hfag_read_pl((UCHAR *)response, length);
}


/* This function is the callback registered with HFP AG profile */
API_RESULT hfp_ag_appl_callback
           (
               /* IN */ HFP_AG_EVENTS    hfp_ag_event,
               /* IN */ API_RESULT       result,
               /* IN */ void *           data,
               /* IN */ UINT16           data_length
           )
{
    UCHAR recvd_bd_addr[BT_BD_ADDR_SIZE];
    UINT16 i;
    UCHAR* recvd_data;
    UCHAR cmd[16U];

    switch(hfp_ag_event)
    {
        case HFP_AG_CONNECT_IND:
            LOG_DEBUG("[Appl] HFP_AG_CONNECT_IND \n");
            BT_mem_copy(hfu_bd_addr, (UCHAR*)data,data_length);
            appl_hfp_ag_state = APPL_HF_STATE_CONNECTED;

            LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
            BT_DEVICE_ADDR_ONLY_PRINT_STR (hfu_bd_addr));

            /* Update the eSCO channel paramters for Default CVSD Codec */
            (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
            (
                BT_TRUE,
                appl_hfag_esco_params[0U]
            );

            appl_hfag_inband = BT_FALSE;
            appl_hfag_dial = BT_TRUE;
            hfp_ag_start_pl(hfp_ag_local_supported_features_ext);

            break;

        case HFP_AG_CONNECT_CNF:
            LOG_DEBUG("[Appl] HFP_AG_CONNECT_CNF \n");
            if(API_SUCCESS == result)
            {
                appl_hfp_ag_state = APPL_HF_STATE_CONNECTED;
                BT_mem_copy(hfu_bd_addr, (UCHAR*)data,data_length);
                LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
                BT_DEVICE_ADDR_ONLY_PRINT_STR (hfu_bd_addr));

                /* Update the eSCO channel paramters for Default CVSD Codec */
                (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
                (
                    BT_TRUE,
                    appl_hfag_esco_params[0U]
                );

                appl_hfag_inband = BT_FALSE;
                appl_hfag_dial = BT_TRUE;
                hfp_ag_start_pl(hfp_ag_local_supported_features_ext);
            }
            appl_handle_retval_from_hfag(result);
            break;
        case HFP_AG_DISCONNECT_IND:
            BT_mem_copy(recvd_bd_addr, (UCHAR*)data,data_length);
            LOG_DEBUG("[Appl] HFP_AG_DISCONNECT_IND \n");

            LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
            BT_DEVICE_ADDR_ONLY_PRINT_STR (recvd_bd_addr));

            appl_hfp_ag_state = APPL_HF_STATE_DISCONNECTED;
            hfp_ag_stop_pl();

            break;

        case HFP_AG_DISCONNECT_CNF:
            BT_mem_copy(recvd_bd_addr, (UCHAR*)data,data_length);
            LOG_DEBUG("[Appl] HFP_AG_DISCONNECT_CNF \n");

            LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
            BT_DEVICE_ADDR_ONLY_PRINT_STR (recvd_bd_addr));

            appl_hfp_ag_state = APPL_HF_STATE_DISCONNECTED;
            hfp_ag_stop_pl();

            break;

        case HFP_AG_STOP_CNF:
            LOG_DEBUG("[Appl] HFP_AG_STOP_CNF \n");
            break;

        case HFP_AG_SEND_DATA_CNF:
            if (API_SUCCESS != result)
            {
                LOG_DEBUG("[Appl] HFP_AG_SEND_DATA_CNF \n");
                appl_handle_retval_from_hfag(result);
            }

            if (appl_hfag_send[appl_hfag_sendrd].buf == data)
            {
                API_RESULT retval;

                BT_free_mem(appl_hfag_send[appl_hfag_sendrd].buf);
                appl_hfag_send[appl_hfag_sendrd].buf = NULL;

                if (API_SUCCESS != result)
                {
                    break;
                }

                appl_hfag_sendrd++;
                appl_hfag_sendrd &= (APPL_HFAG_SEND_QUEUE_SIZE - 1U);

                if ((appl_hfag_sendrd != appl_hfag_sendwr) &&
                    (NULL != appl_hfag_send[appl_hfag_sendrd].buf))
                {
                    retval = BT_hfp_ag_send_data
                             (
                                 appl_hfag_send[appl_hfag_sendrd].buf,
                                 appl_hfag_send[appl_hfag_sendrd].len
                             );

                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG("BT_hfp_ag_send_data Failed - 0x%04X\n", retval);
                        BT_free_mem(appl_hfag_send[appl_hfag_sendrd].buf);
                    }
                }
            }
            else
            {
                LOG_DEBUG("*** Spurious Send Data Cnf ***\n");
            }
            break;

        case HFP_AG_RECVD_DATA_IND:
            LOG_DEBUG("[Appl] HFP_AG_RECVD_ATCMD \n");
            LOG_DEBUG("[Appl] CMD LEN : %d \n",(UINT16) data_length);

            recvd_data = (UCHAR*)data;
            for(i=0U; i<data_length;i++)
            {
                LOG_DEBUG("%c",recvd_data[i]);
            }
            LOG_DEBUG("\n");

#ifndef HFP_AG_NO_AUTOMATED_SLC_RESPONSE
        {
            AT_PARSER_RESPONSE at_response;
            UINT16 start_index, buffer_size, length, retval;

            /* Call the AT parser to parse the AT command sent from unit */
            length = data_length;
            buffer_size = data_length;
            start_index = 0U;

            appl_dump_bytes (recvd_data, buffer_size);

            retval = BT_ag_at_parser
                     (
                         recvd_data,
                         start_index,
                         buffer_size,
                         &length,
                         &at_response
                     );

            /* Handle the AT commands */
            if (API_SUCCESS == retval)
            {
                switch (at_response.keyword_type)
                {
                    case AT_BRSF:
                        appl_hfu_brsf = appl_str_to_num
                                        (
                                            &at_response.global_at_str[at_response.param->start_of_value_index],
                                            at_response.param->value_length
                                        );

                        appl_send_rsp(HFAG_BRSF, NULL);
                        appl_send_rsp(HFAG_OK, NULL);
                        appl_hfag_codec_negotiate = 0U;
                        break;

                    case AT_NREC:
                        if (!appl_hfag_nrec)
                        {
                            appl_send_rsp(HFAG_ERROR, NULL);
                        }
                        else
                        {
                            switch (at_response.global_at_str[at_response.param->start_of_value_index])
                            {
                            case '1':
                                LOG_DEBUG("NREC Enabled\n");
                                appl_send_rsp(HFAG_OK, NULL);
                                break;

                            case '0':
                                LOG_DEBUG("NREC Disbled\n");
                                appl_send_rsp(HFAG_OK, NULL);
                                break;

                            default:
                                appl_send_rsp(HFAG_ERROR, NULL);
                                break;
                            }
                        }
                        break;

                    case AT_BVRA:
                        switch (at_response.global_at_str[at_response.param->start_of_value_index])
                        {
                        case '1':
                            LOG_DEBUG("VR Enabled\n");
                            appl_send_rsp(HFAG_OK, NULL);
                            appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
                            break;

                        case '0':
                            LOG_DEBUG("VR Disbled\n");
                            appl_send_rsp(HFAG_OK, NULL);
                            appl_hfag_close_voice_channel();
                            break;

                        case '2':
                            LOG_DEBUG("EVR Enabled\n");
                            appl_send_rsp(HFAG_OK, NULL);
                            break;

                        default:
                            appl_send_rsp(HFAG_ERROR, NULL);
                            break;
                        }
                        break;

                    case AT_VGS:
                        appl_hfag_vgs = (UCHAR)appl_str_to_num
                                               (
                                                   &at_response.global_at_str[at_response.param->start_of_value_index],
                                                   at_response.param->value_length
                                               );
                        LOG_DEBUG("Speaker Gain Updated - %d\n", appl_hfag_vgs);
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_VGM:
                        appl_hfag_vgm = (UCHAR)appl_str_to_num
                                               (
                                                   &at_response.global_at_str[at_response.param->start_of_value_index],
                                                   at_response.param->value_length
                                               );
                        LOG_DEBUG("Microphone Gain Updated - %d\n", appl_hfag_vgm);
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_BINP:
                        appl_send_rsp(HFAG_BINP, NULL);
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_BCC:
                        appl_send_rsp(HFAG_OK, NULL);
                        appl_send_rsp(HFAG_BCS, NULL);
                        break;

                    case AT_BCS:
                        appl_send_rsp(HFAG_OK, NULL);

                        appl_hfag_codec_negotiate = 0U;

                        /* Trigger codec connection */
                        appl_hfag_open_voice_channel
                        ((at_response.global_at_str[at_response.param->start_of_value_index] - '0') - 1U);
                        break;

                    case AT_BAC:
                        appl_send_rsp(HFAG_OK, NULL);
                        appl_hfag_codec_negotiate = 1U;
                        appl_hfag_codec = 1U;
                        break;

                    case AT_BIND_TEST:
                        appl_send_rsp(HFAG_BIND_TEST, NULL);
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_BIND_READ:
                        for (i = 0U; i < sizeof(appl_hfag_bind); i++)
                        {
                            appl_send_rsp(HFAG_BIND_READ, &i);
                        }
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_BIND:
                        appl_send_rsp(HFAG_OK, NULL);
                        break;

                    case AT_BIEV:
                        if (2U < (at_response.global_at_str[at_response.param->start_of_value_index] - '0'))
                        {
                            appl_send_rsp(HFAG_ERROR, NULL);
                        }
                        else
                        {
                            appl_send_rsp(HFAG_OK, NULL);
                        }
                        break;

                    case ATD:
                        /* Hold the phone number */
                        BT_str_n_copy
                        (
                            appl_hfag_phnum,
                            &at_response.global_at_str[at_response.param->start_of_value_index],
                            (sizeof(appl_hfag_phnum) - 1)
                        );
                        hfp_ag_write_pl(recvd_data, buffer_size);
                        break;

                    case ATDM:    /* Fall Through */
                    case AT_BLDN:
                        if (0U == appl_hfag_dial)
                        {
                            appl_send_rsp(HFAG_ERROR, NULL);
                            break;
                        }

                        BT_mem_set(cmd, 0, sizeof(cmd));
                        BT_str_print((CHAR *)cmd, "ATD7654321;\r");
                        hfp_ag_write_pl(cmd, (UINT16)BT_str_len(cmd));
                        break;

                    default:
                        hfp_ag_write_pl(recvd_data, buffer_size);
                        break;
                }
            }
            else
            {
                appl_send_rsp(HFAG_ERROR, NULL);
            }
        }
#endif /* !HFP_AG_NO_AUTOMATED_SLC_RESPONSE */
        break;

        default:
        LOG_DEBUG("Invalid Event: 0x%02X\n", hfp_ag_event);
        break;
    }

    return API_SUCCESS;
}

/* Function to handle API return values from HFP AG */
void appl_handle_retval_from_hfag ( /* IN */ UINT16 retval)
{
    switch(retval)
    {
    case API_SUCCESS:
        LOG_DEBUG("[Appl] API_SUCCESS\n");
        break;
    case HFP_AG_ERR_NULL_PARAMETER:
        LOG_DEBUG("[Appl] HFP_AG_ERR_NULL_PARAMETER\n");
        break;
    case HFP_AG_MUTEX_INIT_FAILED:
        LOG_DEBUG("[Appl] HFP_AG_MUTEX_INIT_FAILED \n");
        break;
    case HFP_AG_ERR_INVALID_PARAMETER:
        LOG_DEBUG("[Appl] HFP_AG_ERR_INVALID_PARAMETER\n");
        break;
    case HFP_AG_ERR_INVALID_STATE:
        LOG_DEBUG("[Appl] HFP_AG_ERR_INVALID_STATE\n");
        break;
    case HFP_AG_ERR_STOP_PENDING:
        LOG_DEBUG("[Appl] HFP_AG_ERR_STOP_PENDING\n");
        break;
    case HFP_AG_MUTEX_LOCK_FAILED:
        LOG_DEBUG("[Appl] HFP_AG_MUTEX_LOCK_FAILED\n");
        break;
    case HFP_AG_MUTEX_UNLOCK_FAILED:
        LOG_DEBUG("[Appl] HFP_AG_MUTEX_UNLOCK_FAILED\n");
        break;

    default:
        LOG_DEBUG("[Appl] Result: 0x%04X\n", retval);
        break;
    } /* End Switch */

    return;
}


/* Function to process user choice for supported features */
void register_features( /* IN */ UCHAR supported_features)
{
    int choice;
    int feature_choice;

    do
    {
        LOG_DEBUG("\n");
        LOG_DEBUG("    0. exit without register\n");
        LOG_DEBUG("    1. Three Way Call Support\n");
        LOG_DEBUG("    2. EC and/or NR function\n");
        LOG_DEBUG("    3. Voice Recognition function\n");
        LOG_DEBUG("    4. InBand-Ring Tone Capability\n");
        LOG_DEBUG("    5. Attach a phone number to voice tag\n");
        LOG_DEBUG("    6. Network Call-Reject Ability\n");
        LOG_DEBUG("    7. Enable all supported features\n");
        LOG_DEBUG("    8. Register with SDP\n");

        LOG_DEBUG("\n  Your choice :");

        scanf("%d",&choice);

        switch(choice)
        {
        case 0:
           break;

        case 1:
           LOG_DEBUG("    Enter 1-Yes 0-No :");
           scanf("%d", &feature_choice);
           if(0 != feature_choice)
           {
               supported_features |= SET_AG_CCWA;
           }
           else
           {
               supported_features &= RESET_AG_CCWA;
           }
           break;

        case 2:
            LOG_DEBUG("    Enter 1-Yes 0-No :");
            scanf("%d", &feature_choice);
            if(0 != feature_choice)
            {
                supported_features |= SET_AG_ECNR;
            }
            else
            {
                supported_features &= RESET_AG_ECNR;
            }
            break;

        case 3:
            LOG_DEBUG("    Enter 1-Yes 0-No :");
            scanf("%d", &feature_choice);
            if(0 != feature_choice)
            {
                supported_features |= SET_AG_BVRA;
            }
            else
            {
                supported_features &= RESET_AG_BVRA;
            }
            break;

        case 4:
            LOG_DEBUG("    Enter 1-Yes 0-No :");
            scanf("%d", &feature_choice);
            if(0 != feature_choice)
            {
                supported_features |= SET_AG_BSIR;
            }
            else
            {
                supported_features &= RESET_AG_BSIR;
            }
            break;

        case 5:
            LOG_DEBUG("    Enter 1-Yes 0-No :");
            scanf("%d", &feature_choice);
            if(0 != feature_choice)
            {
               supported_features |= SET_AG_BINP;
            }
            else
            {
                supported_features &= RESET_AG_BINP;
            }
            break;

        case 6:
            LOG_DEBUG("    Enter 1-Ability to reject a call \n");
            LOG_DEBUG("    Enter 0-No ability to reject a call \n");
            scanf("%d", &feature_choice);
            break;

        case 7:
            /* Enable all features here */
            supported_features |= SET_ALL_FEATURES;
            LOG_DEBUG(" Enabled all supported features \n");

            break;

        case 8:
            hfp_ag_register_supp_feat(supported_features);
            break;

        default:
            LOG_DEBUG("Invalid Choice \n");
            break;
         }
    } while(choice != 0U && choice != 8U);

    return;
}


/* Function to register Supported features with local SDP Database */
void hfp_ag_register_supp_feat ( /* IN */ UCHAR supported_features)
{
    UCHAR  attr_value[] = {0x09U, 0x00U, 0x00U};
    UINT32 hfp_ag_record_handle;
    UINT16 hfp_ag_supp_features;

    /* Get HF AG SDP record handle */
    if (API_SUCCESS == BT_dbase_get_record_handle(DB_RECORD_HFAG, 1U, &hfp_ag_record_handle))
    {
        hfp_ag_supp_features = supported_features;

        attr_value[1U] = (UCHAR)(hfp_ag_supp_features >> 8U);
        attr_value[2U] = (UCHAR)(hfp_ag_supp_features);

        if(API_SUCCESS != BT_dbase_update_attr_value
                          (
                              hfp_ag_record_handle,
                              HFP_AG_SUPP_FEATURES_ATTR_ID,
                              attr_value,
                              0x03U
                          ))
        {
            LOG_DEBUG("HFP AG SUPP_FEAT_REG_FAILED \n");
        }
    }

    return;
}


/* Function to activate HFP AG SDP record and Start HFAG */
void appl_start_hfp_ag (void)
{
    API_RESULT api_retval;

    UINT32 hfp_ag_record_handle;

    /* local varibale to extract the supported features */
    UCHAR  attr_value[] = {0x09U, 0x00U, 0x00U};

    UINT16 str_len, hfp_ag_feature, i;
    UCHAR  hfp_ag_str[6U];

    LOG_DEBUG("HF supported feature bitmap\n");
    LOG_DEBUG("\tBit\tFeature\n");
    LOG_DEBUG("\t---\t-------\n");
    LOG_DEBUG("\t 0  \t3-way calling\n");
    LOG_DEBUG("\t 1  \tEC and/or NR function\n");
    LOG_DEBUG("\t 2  \tVoice recognition function\n");
    LOG_DEBUG("\t 3  \tIn-band ringtone capability\n");
    LOG_DEBUG("\t 4  \tAttach a number to a voice tag\n");
    LOG_DEBUG("\t 5  \tAbility to reject a call\n");
    LOG_DEBUG("\t 6  \tEnhanced call status\n");
    LOG_DEBUG("\t 7  \tEnhanced call control\n");
    LOG_DEBUG("\t 8  \tExtended Error result codes\n");

#ifdef HFP_UNIT_1_6
    LOG_DEBUG("\t 9  \tCodec Negotiation\n");
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
    LOG_DEBUG("\t 10  \tHF Indicators\n");
    LOG_DEBUG("\t 11  \teSCO S4 Settings Supported\n");
#endif /* HFP_UNIT_1_7 */

#ifdef HFP_UNIT_1_8
    LOG_DEBUG("\t 12 \tEnhanced Voice Recognition Status\n");
    LOG_DEBUG("\t 13 \tVoice Recognition Text\n");
#endif /* HFP_UNIT_1_8 */

    LOG_DEBUG("Enter Local HFP_AG Supported Features(as Decimal value)\n:");
    scanf("%s", hfp_ag_local_supported_features);

    BT_mem_set(hfp_ag_str, 0x00, sizeof(hfp_ag_str));
    BT_str_n_copy(hfp_ag_str ,hfp_ag_local_supported_features, (sizeof(hfp_ag_str) - 1));
    str_len = (UINT16)BT_str_len(hfp_ag_str);

    hfp_ag_feature  = (UINT16)appl_str_to_num(hfp_ag_local_supported_features, str_len);

    /* Init */
    api_retval = API_SUCCESS;

    for (i = 0; i < str_len; i++)
    {
        if ((!((hfp_ag_str[i]) >= '0' && (hfp_ag_str[i]) <= '9')) || ( (hfp_ag_feature < HFP_AG_MIN_SUPP_FEATURE_RANGE) || (hfp_ag_feature > HFP_AG_MAX_SUPP_FEATURE_RANGE) ) )
        {
            api_retval =  HFP_AG_ERR_INVALID_PARAMETER;
            appl_handle_ret = 0;
            appl_handle_retval_from_hfag(api_retval);
            break; /* return; */
        }
    }

    if (API_SUCCESS == api_retval)
    {
        appl_handle_ret = 1;

        hfp_ag_local_supported_features[HFP_UNIT_MAX_SUPP_FEATURE_LEN - 1U] = '\0';
        BT_str_n_copy(appl_hfag_brsf, hfp_ag_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN);

        /* Local Supported Features in Numeric Format */
        hfp_ag_local_supported_features_ext = (UINT16)appl_str_to_num
                                                      (
                                                          hfp_ag_local_supported_features,
                                                          (UINT16)BT_str_n_len(hfp_ag_local_supported_features, HFP_UNIT_MAX_SUPP_FEATURE_LEN)
                                                      );

        attr_value[1U] = hfp_ag_local_supported_features[0U];
        attr_value[2U] = hfp_ag_local_supported_features[1U];

        /* Get HFP_UNIT SDP record handle */
        (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle
        (
            DB_RECORD_HFAG,
            0U,
            &hfp_ag_record_handle
        );

        api_retval = BT_dbase_update_attr_value
                     (
                         hfp_ag_record_handle,
                         0x0311U,
                         attr_value,
                         0x03U
                     );

        LOG_DEBUG("[Appl] HFP AG Get Record Handle %02x\n",api_retval);

        api_retval = BT_dbase_get_server_channel
                     (
                         hfp_ag_record_handle,
                         PROTOCOL_DESC_LIST,
                         &hfp_ag_server_channel
                     );
        LOG_DEBUG("[Appl] HFP AG Get Server Channel %02x\n",api_retval);

        api_retval = BT_hfp_ag_start(hfp_ag_server_channel);

        if(API_SUCCESS == api_retval)
        {
            appl_hfp_ag_state = APPL_HF_STATE_DISCONNECTED;

            (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record(hfp_ag_record_handle);

            LOG_DEBUG("\n[Appl] HFP AG Profile Started Successfully\n");
        }
        else
        {
            appl_handle_retval_from_hfag(api_retval);
        }
    }

    return;
}


/* SDP Callback for HFP */
void appl_hfag_sdp_cb
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
    API_RESULT retval = API_SUCCESS;

    UINT16 len_attrib_data;

    /* Length of supported features */
    /* UINT16 support_len = 2; */

    BT_IGNORE_UNUSED_PARAM(length);

    /* Set AG related UUID variables */
    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = HFP_HFU_HANDS_FREE_UUID;
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
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
            retval = API_FAILURE; /*  return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        switch(command)
        {
        case SDP_Open : /* SDP open callback */
        {
            attribute_data = BT_alloc_mem (100U);

            if(NULL == attribute_data)
            {
                LOG_DEBUG("> ** FAILED to Allocate memory for SDP Query\n");

                /* Close SDP */
                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
                /* return; */
            }
            else
            {
                len_attrib_data = 100U;

                /* Do Service Search Request */
                retval = BT_sdp_servicesearchattributerequest
                         (
                             &sdp_handle,
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

                    BT_free_mem (attribute_data);

                    LOG_DEBUG("> Closing SDP Connection\n");

                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);

                    /* return; */
                }
            }
            /* SDP operation to continue */

            break;
        }

        case SDP_Close: /* SDP Close callback */
        {
               break;
        }

        /* Service Search Attribute callback */
        case SDP_ServiceSearchAttributeResponse:
            /* From the response extract the server channel */
            retval = BT_sdp_get_channel_number
                     (
                         data,
                         &hfp_ag_peer_server_channel
                     );

            LOG_DEBUG("> Remote SERVER CHANNEL 0x%02X\n",\
            hfp_ag_peer_server_channel);

            if(API_SUCCESS != retval)
            {
                BT_free_mem(data);

                LOG_DEBUG("> ** FAILED to get Remote SERVER CHANNEL\n");

                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
                /* return; */
            }
            else
            {
#if 0
                /* From the response extract the remote supported features */
                if( BT_sdp_get_attr_value
                    (
                        attrib_id[1],
                        data,
                        (UCHAR *)&hfp_unit_peer_supported_features[0],
                        &support_len
                    ) != API_SUCCESS )
                {
                    LOG_DEBUG("> ** FAILED to extract the remote supported"\
                    "features\n");
                    sdp_close_due2error = 2;

                }
                else
                {
                    LOG_DEBUG("> AG Supported features %X\n", \
                    hfp_unit_peer_supported_features[0]);

                }
#endif /* 0 */

                /* Free allocated memory for attribute data */
                BT_free_mem(data);

                /* Close SDP */
                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
            }

            break;


        case SDP_ErrorResponse:

            LOG_DEBUG("> ** ERROR occoured in SDP Query\n");

            /* Free allocated memory for attribute data */
            BT_free_mem(data);

            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);

            break;

        default: /* Invalid: Nothing to do */
            LOG_DEBUG("Invalid Command: 0x%02X\n", command);
            break;

        }/* switch */
    }

    return;
}

void appl_hfp_ag_send_data(void)
{
    CHAR     ag_rsp_data[64U];

    BT_mem_set(appl_hfp_ag_data, 0, sizeof(appl_hfp_ag_data));
    BT_mem_set(ag_rsp_data, 0, sizeof(ag_rsp_data));

    LOG_DEBUG ("Enter AT cmd/rsp string\n");
    scanf("%s", ag_rsp_data);

    BT_str_n_copy (appl_hfp_ag_data, HFP_CR_LF, sizeof(appl_hfp_ag_data));
    BT_str_n_cat (appl_hfp_ag_data, ag_rsp_data, (sizeof(appl_hfp_ag_data) - BT_str_len(appl_hfp_ag_data)));
    BT_str_n_cat (appl_hfp_ag_data, HFP_CR_LF, (sizeof(appl_hfp_ag_data) - BT_str_len(appl_hfp_ag_data)));

    appl_hfp_ag_datalen = (UCHAR )BT_str_len(appl_hfp_ag_data);

    LOG_DEBUG ("== HFP AG data ==\n");

#if 0
    LOG_DEBUG ("HFP AG datalen:%d\n", appl_hfp_ag_datalen);
    LOG_DEBUG ("== HFP AG data(in CHAR frmt) ==\n");
    for (i = 0; i < appl_hfp_ag_datalen; i++)
    {
        LOG_DEBUG ("%c", appl_hfp_ag_data[i]);
    }
    LOG_DEBUG ("\n=================\n");

    LOG_DEBUG ("== HFP AG data(in HEX frmt) ==\n");
    for (i = 0; i < appl_hfp_ag_datalen; i++)
    {
        LOG_DEBUG ("%02X", appl_hfp_ag_data[i]);
    }
    LOG_DEBUG ("\n=================\n");

#else
    appl_dump_bytes ((UCHAR *)appl_hfp_ag_data, appl_hfp_ag_datalen);
#endif /* 0 */

    /**
     * Yes, Send data received from HFP Unit RFCOMM Transport to
     * remote HFP Unit through HFP AG
     */
    appl_hfag_read_pl
    (
        (UCHAR *)appl_hfp_ag_data,
        appl_hfp_ag_datalen
    );

    return;
}

void appl_hfp_ag_send_enable_voice_recognition (void)
{
    if (0U == (appl_hfu_brsf & 0x0008U))
    {
        LOG_DEBUG("HFU does not support VR feature\n");
        /* return; */
    }
    else
    {
        appl_hfag_read_pl
        (
            (UCHAR *)HFP_AG_ENABLE_VOICE_REG,
            (UCHAR)BT_str_len(HFP_AG_ENABLE_VOICE_REG)
        );

        appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
    }

    return;
}

void appl_hfp_ag_send_disable_voice_recognition (void)
{
    if (0U == (appl_hfu_brsf & 0x0008U))
    {
        LOG_DEBUG("HFU does not support VR feature\n");
        /* return; */
    }
    else
    {
        appl_hfag_read_pl
        (
            (UCHAR *)HFP_AG_DISABLE_VOICE_REG,
            (UCHAR)BT_str_len(HFP_AG_DISABLE_VOICE_REG)
        );

        appl_hfag_close_voice_channel();
    }

    return;
}

#ifdef HFP_AG_1_8
void appl_hfp_ag_send_enh_voice_recognition (void)
{
    CHAR response[128U];
    int choice;
    CHAR *text[] = { "Hi", "Hello", "Hello?", "Hello!" };
    CHAR *str;

    if (0U == (appl_hfu_brsf & 0x0400U))
    {
        LOG_DEBUG("HFU does not support EVR feature\n");
        /* return; */
    }
    else
    {
        BT_mem_set(response, 0, sizeof(response));

        LOG_DEBUG("VRECStates:\n");
        LOG_DEBUG("Bit 0: Ready to Accept Audio Input\n");
        LOG_DEBUG("Bit 1: Sending Audio to HF\n");
        LOG_DEBUG("Bit 2: Processing Audio Input\n");
        LOG_DEBUG("Enter the VRECState: ");
        scanf("%d", &choice);

        BT_str_print(response, "%s%d,", HFP_AG_ENH_VR_MSG, choice);

        if (0U != (appl_hfu_brsf & 0x0800U))
        {
            LOG_DEBUG("Text ID (2 octet Max):\n");
            scanf("%d", &choice);
            BT_str_print((&response[BT_str_len(response)]), "%04X,", choice);

            LOG_DEBUG("Enter Text Type (0: HF IP, 1: AG OP 2: AG OP Question, 3: AG OP Error): ");
            scanf("%d", &choice);
            BT_str_print((&response[BT_str_len(response)]), "%d,", choice);
            str = text[choice];
            LOG_DEBUG("Enter Text Operation (1: New, 2: Replace, 3: Append): ");
            scanf("%d", &choice);
            BT_str_print((&response[BT_str_len(response)]), "%d,", choice);

            BT_str_print((&response[BT_str_len(response)]), "\"%s\"\r\n", str);
        }

        appl_hfag_read_pl
        (
            (UCHAR *)response,
            (UCHAR)BT_str_len(response)
        );
    }

    return;
}
#endif /* HFP_AG_1_8 */

void appl_hfpag_change_esco_config(void)
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
    appl_hci_set_esco_channel_parameters
    (
        BT_TRUE,
        &esco_params
    );

#else
    LOG_DEBUG("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

/*command to create eSCO connection with custom eSCO parameter set using command 19*/
void appl_hfpag_create_eSCO_connection()
{
    API_RESULT retval;
    UINT16 hci_handle;
    HCI_SCO_IN_PARAMS esco_params;
    /*
    * Establish Synchronous Connection with selected setting
    * with UNIT
    */
    retval = BT_hci_get_acl_connection_handle
             (
                 hfu_bd_addr,
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

void appl_hfag_open_voice_channel(UCHAR codec)
{
    API_RESULT retval;
    UINT16 hci_handle;
    HCI_SCO_IN_PARAMS hfp_ag_esco_param;
    /*
    * Establish Synchronous Connection with selected setting
    * with AG
    */
    retval = BT_hci_get_acl_connection_handle
             (
                 hfu_bd_addr,
                 &hci_handle
             );

    if (API_SUCCESS == retval)
    {
        /*get default eSCO parameter set */
        appl_hci_get_esco_channel_parameters(&hfp_ag_esco_param);

        hfp_ag_esco_param.voice_setting = appl_hfag_esco_params[codec]->voice_setting;

        /* Update the eSCO channel paramters for Codec */
        (BT_IGNORE_RETURN_VALUE) appl_hci_set_esco_channel_parameters
        (
            BT_TRUE,
            &hfp_ag_esco_param
        );

        LOG_DEBUG("> API RETVAL BT_hci_setup_synchronous_connection\n");

        retval = BT_hci_setup_synchronous_connection
                 (
                     hci_handle,
                     &hfp_ag_esco_param
                 );
    }

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
}

void appl_hfag_close_voice_channel(void)
{
    API_RESULT retval;
    UINT16 hci_sco_handles[HCI_MAX_SCO_CHANNELS];
    UCHAR num_of_sco_handles;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(hci_sco_handles, 0, sizeof(UINT16)*HCI_MAX_SCO_CHANNELS);

    retval = BT_hci_get_sco_connection_handle
                (
                    hfu_bd_addr,
                    hci_sco_handles,
                    &num_of_sco_handles
                );

    /* If SCO/eSCO connection present, disconect the first one */
    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("Disconnecting SCO/eSCO Connection 0x%04X\n",
        hci_sco_handles[0U]);

        retval = BT_hci_disconnect
                 (
                     hci_sco_handles[0U],
                     0x13U
                 );
    }
    else
    {
        LOG_DEBUG ("SCO Connection for HFP-Unit not found\n");
    }
}


void appl_hfag_read_pl(UCHAR * rsp, UINT16 rsplen)
{
    API_RESULT retval;
    UCHAR * buf;

    if (APPL_HF_STATE_CONNECTED != appl_hfp_ag_state)
    {
        LOG_DEBUG("Invalid State for read operation\n");
        /* return; */
    }
    else
    {
        /* Allocate memory */
        buf = BT_alloc_mem(rsplen);
        if (NULL == buf)
        {
            LOG_DEBUG("Failed to allocate buffer\n");
            /* return; */
        }
        else
        {
            /* Copy the data */
            BT_mem_copy(buf, rsp, rsplen);

            /* Add it to the queue */
            appl_hfag_send[appl_hfag_sendwr].buf = buf;
            appl_hfag_send[appl_hfag_sendwr].len = rsplen;

            retval = API_SUCCESS;

            /* Is Tx ongoing? */
            if (appl_hfag_sendwr == appl_hfag_sendrd)
            {
                retval = BT_hfp_ag_send_data
                         (
                             buf,
                             rsplen
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("BT_hfp_ag_send_data Failed - 0x%04X\n", retval);
                    BT_free_mem(buf);
                }
            }

            if (API_SUCCESS == retval)
            {
                appl_hfag_sendwr++;
                appl_hfag_sendwr &= (APPL_HFAG_SEND_QUEUE_SIZE - 1U);
            }
        }
    }
}

void appl_hfag_callend_timeout_handler(void * args, UINT16 size)
{
    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(size);

    appl_hfag_close_voice_channel();
}

void appl_hfag_call_status_pl(UCHAR status)
{
    BT_timer_handle tmr_handle;

    switch (status)
    {
    case 0x00U: /* Call End */
        (BT_IGNORE_RETURN_VALUE) BT_start_timer(&tmr_handle, 1U, appl_hfag_callend_timeout_handler, NULL, 0U);
        break;

    case 0x01U: /* Call Active */
        /* if (!appl_hfag_inband) */
        {
            if (0U != appl_hfag_codec_negotiate)
            {
                appl_send_rsp(HFAG_BCS, NULL);
            }
            else
            {
                appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
            }
        }
        break;

    case 0x02U: /* Call Incoming */
        if (0U != appl_hfag_inband)
        {
            if (0U != appl_hfag_codec_negotiate)
            {
                appl_send_rsp(HFAG_BCS, NULL);
            }
            else
            {
                appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
            }
        }
        break;

    case 0x03U: /* Call Outgoing */
        if (0U != appl_hfag_codec_negotiate)
        {
            appl_send_rsp(HFAG_BCS, NULL);
        }
        else
        {
            appl_hfag_open_voice_channel(appl_hfag_codec - 1U);
        }
        break;

    default:
        LOG_DEBUG("Invalid Status: 0x%02X\n", status);
        break;
    }
}

#endif /* HFP_AG */
