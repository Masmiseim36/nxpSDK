
/**
 *  \file appl_hci_api.c
 *
 *  Source File for HCI Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hci.h"
#include "appl_utils.h"

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
/*
 *  Default Voice Settings to be used for Wideband Speech Synchronous Connection:
 *  Input Coding: A-Law
 *  Input Format: 2's Compliment
 *  Input Sample Size: 16-bit
 *  Linear PCM Bit Position: 3
 *  Air Coding Format: TRANSPARENT
 */
#define APPL_WIDEBAND_VOICE_SETTING_DEFAULT \
    (LMP_VOICE_INPUT_CODING_LINEAR | \
    LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT | \
    LMP_VOICE_INPUT_SAMPLE_16_BIT | \
    LMP_VOICE_LINEAR_BIT_POS_0 | \
    LMP_VOICE_AIR_CODING_TRANSPARENT)


/* ----------------------------------------- Functions */

void appl_hci_inquiry ( void )
{
#ifdef BR_EDR_HCI
#ifndef HCI_LITE

    UINT32 lap;
    UCHAR  inq_length;
    UCHAR  inq_resp;
    unsigned int read_val;
    API_RESULT retval;

    printf("HCI Inquiry. Enter Required Information ..\n");

    printf("LAP (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    lap = (UINT32)read_val;

    printf("Inquiry Length (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    inq_length = (UCHAR)read_val;

    printf("Num Responses (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    inq_resp = (UCHAR)read_val;

    printf("\n");
    printf("Initiating Bluetooth Inquiry ... "); fflush(stdout);

    retval = BT_hci_inquiry ( lap,
                              inq_length,
                              inq_resp );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Inquiry Results & Inquiry Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_create_connection ( void )
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    UINT16 clock_offset, packet_type;
    API_RESULT retval;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR role, page_scan_rep_mode, default_params;

    printf("Use default parameters? (1/0): ");
    scanf("%d", &read_val);
    default_params = (UCHAR)read_val;

    printf("Create ACL Connection. Enter Required Information ..\n");

    printf("Remote BD_ADDR = "); fflush(stdout);
    retval = appl_get_bd_addr(bd_addr);
    if(API_SUCCESS != retval)
    {
        printf("Invalid Remote BD_ADDR Address Entered\n");
        /* return; */
    }
    else
    {
        printf("Enter Clock Offset (in Hex) = "); fflush(stdout);
        scanf("%x", &read_val);
        clock_offset = (UINT16)read_val;

        printf("Allow Role Switch (0/1) = "); fflush(stdout);
        scanf("%x", &read_val);
        role = (UCHAR)read_val;

        if (1U == default_params)
        {
            packet_type = LMP_ACL_DH5;
            page_scan_rep_mode = 0x00U;
        }
        else
        {
            printf("Enter packet_type (in Hex)) = "); fflush(stdout);
            scanf("%x", &read_val);
            packet_type = (UINT16)read_val;

            printf("Enter page_scan_repetition_mode (in Hex)) = "); fflush(stdout);
            scanf("%x", &read_val);
            page_scan_rep_mode = (UCHAR)read_val;
        }

        printf("\n");
        printf("Starting ACL Connection ... "); fflush(stdout);

        retval = BT_hci_create_connection ( bd_addr,
                                            packet_type,
                                            page_scan_rep_mode,
                                            0x0U,
                                            clock_offset,
                                            role );

        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Successfully started.\n");
            printf("Please wait for Connect Complete.\n");
        }
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_disconnect ( void )
{
    unsigned int read_val;
    API_RESULT retval;
    UCHAR      reason;
    UINT16     handle;

    printf("Disconnect ACL/SCO Connection. Enter Required Information ...\n");

    printf("ACL/SCO Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    handle = (UINT16)read_val;

    printf("Reason for Disconnection (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    reason = (UCHAR)read_val;

    printf("\n");
    printf("Starting ACL/SCO Disconnection ... "); fflush(stdout);
    retval = BT_hci_disconnect ( handle, reason );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Disconnect Complete.\n");
    }
}


void appl_hci_add_sco_connection ( void )
{
#ifdef BR_EDR_HCI
    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE];
    HCI_SCO_IN_PARAMS esco_params;

    printf("Add SCO Connection. Enter Required Information ...\n");

    printf("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    retval = appl_get_bd_addr(bd_addr);
    if(API_SUCCESS != retval)
    {
        printf("Invalid BD_ADDR Address Entered\n");
        /* return; */
    }
    else
    {
        printf("\n");
        appl_hci_get_esco_channel_parameters(&esco_params);

        printf("Starting Add SCO Connection ... "); fflush(stdout);
        retval = BT_hci_add_sco_connection ( bd_addr,
                                             esco_params.packet_type );

        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Successfully started.\n");
            printf("Please wait for Connect Complete.\n");
        }
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_setup_synchronous_connection ( void )
{
#ifdef BR_EDR_HCI
    int choice;
    API_RESULT retval;
    UINT16 handle;
    HCI_SCO_IN_PARAMS esco_params;

    printf("Setup Synchronous Connection. Enter Required Information ...\n");

    printf("ACL Handle of the Remote Device (in Hex) = ");
    scanf ("%x", &choice);
    handle = (UINT16)choice;

    printf("CVSD or mSBC (1/2)?: ");
    scanf("%d", &choice);

    if (1U == choice)
    {
        esco_params.tx_bandwidth  = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth  = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = 0x000CU;
        esco_params.voice_setting = LMP_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_RETX_EFFORT_QUALITY;
        esco_params.packet_type = (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5);
    }
    else if (2U == choice)
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = 0x000DU;
        esco_params.voice_setting = APPL_WIDEBAND_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_RETX_EFFORT_QUALITY;
        esco_params.packet_type = (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5);
    }
    else if (3U == choice)
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        esco_params.voice_setting = APPL_WIDEBAND_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;
        esco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
    }
    else
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        esco_params.voice_setting = LMP_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;
        esco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
    }

    printf("\n");
    printf("Starting Setup Synchronous Connection ... ");
    retval = BT_hci_setup_synchronous_connection (handle, &esco_params);

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Connect Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_setup_synchronous_connection_with_codec_type (UINT8 type)
{
#ifdef BR_EDR_HCI
    int read_val;
    API_RESULT retval;
    UINT16 handle;
    HCI_SCO_IN_PARAMS esco_params;

    printf("Setup Synchronous Connection. Enter Required Information ...\n");

    printf("ACL Handle of the Remote Device (in Hex) = ");
    scanf("%x", &read_val);
    handle = (UINT16)read_val;

    if (2U == type)
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        esco_params.voice_setting = APPL_WIDEBAND_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;
        esco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
    }
    else
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        esco_params.voice_setting = LMP_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;
        esco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
    }

    printf("\n");
    printf("Starting Setup Synchronous Connection ... ");
    retval = BT_hci_setup_synchronous_connection (handle, &esco_params);

    if (retval != API_SUCCESS)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Connect Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_setup_synchronous_connection_response ( void )
{
#ifdef HCI_NO_ESCO_AUTO_ACCEPT
    int choice;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR reject;

    printf("Synchronous Connection Response. Enter Required Information ...\n");

    printf("BD_ADDR of the Remote Device (in Hex) = ");
    appl_get_bd_addr(bd_addr);

    printf("Accept/Reject (0 - Accept, <ErrorCode> - Reject) (in HEX) = ");
    scanf ("%x", &choice);
    reject = (UCHAR)choice;

    printf("\n");
    printf("Starting Synchronous Connection Response ... ");
    BT_hci_esco_connection_response ( bd_addr, reject );

    printf("Successfully started.\n");
    printf("Please wait for Connect Complete.\n");
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
}


#ifdef HCI_ENH_SCO
void appl_hci_read_local_supported_codecs ( void )
{
    API_RESULT retval;

    printf ("Read local supported codecs ...");
    retval = BT_hci_read_local_supported_codecs ();

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


void appl_hci_enhanced_setup_synchronous_connection ( void )
{
    API_RESULT retval;
    UINT16 handle;
    HCI_ENH_SCO_PARAMS esco_params;
    int read_input;

    printf("Enhanced Setup Synchronous Connection. Enter Required Information ...\n");

    printf("ACL/eSCO Handle with the Remote Device (in Hex) = ");
    scanf("%x", &read_input);

    handle = (UINT16)read_input;

    /* Set parameters for mSBC */
    esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
    esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
    esco_params.max_latency  = LMP_ESCO_MAX_LATENCY_DEFAULT;
    esco_params.packet_type  = LMP_ESCO_PACKET_TYPE_DEFAULT;
    esco_params.retransmission_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;

    /**
     *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
     *  Company ID and Vendor Specific Codec ID shall be ignored.
     */
    esco_params.tx_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
    esco_params.tx_coding_frmt.company_id      = 0x02U; /* dummy */
    esco_params.tx_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

    esco_params.rx_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
    esco_params.rx_coding_frmt.company_id      = 0x02U; /* dummy */
    esco_params.rx_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

    /* Configuring for mSBC codec */
    esco_params.tx_codec_frame_size = MSBC_FRAME_SIZE;
    esco_params.rx_codec_frame_size = MSBC_FRAME_SIZE;

    esco_params.input_bandwidth  = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
    esco_params.output_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;

    /**
     *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
     *  Company ID and Vendor Specific Codec ID shall be ignored.
     */
    esco_params.input_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
    esco_params.input_coding_frmt.company_id      = 0x02U; /* dummy */
    esco_params.input_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

    esco_params.output_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
    esco_params.output_coding_frmt.company_id      = 0x02U; /* dummy */
    esco_params.output_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

    /**
     * mSBC encoded data is a byte stream. so coded unit size is 8 bits
     */
    esco_params.input_coded_data_size  = 8U;
    esco_params.output_coded_data_size = 8U;

    /**
     * Below parameters are valide when
     * coding_format equal to LMP_CODING_FRMT_LINEAR_PCM
     */
    esco_params.input_pcm_data_frmt  = 0xFFU; /* dummy */
    esco_params.output_pcm_data_frmt = 0xFFU; /* dummy */
    esco_params.input_pcm_smpl_payload_msb_pos  = 0xFFU; /* dummy */
    esco_params.output_pcm_smpl_payload_msb_pos = 0xFFU; /* dummy */

    /* Exchange SCO packets over HCI transport */
    esco_params.input_data_path  = ENH_SCO_DATA_PATH_HCI;
    esco_params.output_data_path = ENH_SCO_DATA_PATH_HCI;

    /**
     * As per Spec., if SCO packet are exchanged over HCI transport, then
     * below parameters shall be zero
     */
    esco_params.input_transport_unit_size  = 0x00U;
    esco_params.output_transport_unit_size = 0x00U;

    printf("\n");
    printf("Starting Enhanced Setup Synchronous Connection ... ");
    retval = BT_hci_setup_enh_synchronous_connection (handle, &esco_params);

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Connect Complete.\n");
    }
}


void appl_hci_enhanced_setup_synchronous_connection_response ( void )
{
#ifdef HCI_NO_ESCO_AUTO_ACCEPT
    int choice;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR reject;

    printf("Enhanced Setup Synchronous Connection Response. Enter Required Information ...\n");

    printf("BD_ADDR of the Remote Device (in Hex) = ");
    appl_get_bd_addr(bd_addr);

    printf("Accept/Reject (0 - Accept, <ErrorCode> - Reject) (in HEX) = ");
    scanf ("%x", &choice);
    reject = (UCHAR)choice;

    printf("\n");
    printf("Starting Enhanced Synchronous Connection Response ... ");
    BT_hci_enh_sco_connection_response ( bd_addr, reject );

    printf("Successfully started.\n");
    printf("Please wait for Connect Complete.\n");
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
}
#endif /* HCI_ENH_SCO */


void appl_hci_remote_name_request ( void )
{
#ifdef BR_EDR_HCI
#ifndef HCI_LITE

    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE];
    UCHAR      name[249U];

    BT_mem_set(name, 0x0, 249U);

    printf("Remote Name Request. Enter Required Information ...\n");

    printf("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    retval = appl_get_bd_addr(bd_addr);
    if(API_SUCCESS != retval)
    {
        printf("Invalid BD_ADDR Enterd\n");
        /* return; */
    }
    else
    {
        printf("\n");
        printf("Starting Remote Name Request ... "); fflush(stdout);
        retval = BT_hci_remote_name_request ( bd_addr,
                                              0x00U,
                                              0x00U,
                                              0x0000U );
        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Successfully started.\n");
            printf("Please wait for Command Complete.\n");
        }
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_role_discovery ( void )
{
#ifdef BR_EDR_HCI
#ifndef HCI_LITE

    unsigned int read_val;
    API_RESULT retval;
    UINT16     handle;

    printf("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    handle = (UINT16)read_val;

    printf("\n");
    printf("Starting Role Discovery ... "); fflush(stdout);
    retval = BT_hci_role_discovery ( handle );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_switch_role ( void )
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE], role;

    printf("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    retval = appl_get_bd_addr(bd_addr);
    if (API_SUCCESS != retval)
    {
        printf("Invalid BD_ADDR Entered\n");
        /* return; */
    }
    else
    {
        printf("Enter Role (in Hex) = "); fflush(stdout);
        scanf("%x", &read_val);
        role = (UCHAR)read_val;

        printf("\n");
        printf("Starting Switch Role ... "); fflush(stdout);
        retval = BT_hci_switch_role ( bd_addr, role );
        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Successfully started.\n");
            printf("Please wait for Command Complete.\n");
        }
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_sniff_mode(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  sniff_max_interval;
    UINT16  sniff_min_interval;
    UINT16  sniff_attempt;
    UINT16  sniff_timeout;

    printf("Sniff Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("Sniff Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    sniff_max_interval = (UINT16)read_val;

    printf("Sniff Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    sniff_min_interval = (UINT16)read_val;

    printf("Sniff Attempt (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    sniff_attempt = (UINT16)read_val;

    printf("Sniff Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    sniff_timeout = (UINT16)read_val;

    retval = BT_hci_sniff_mode
             (
                 connection_handle,
                 sniff_max_interval,
                 sniff_min_interval,
                 sniff_attempt,
                 sniff_timeout
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_sniff_subrating(void)
{
#ifdef BT_SSR
    unsigned int read_val;
    API_RESULT retval;

    UINT16 connection_handle;
    UINT16 maximum_latency;
    UINT16 minimum_remote_timeout;
    UINT16 minimum_local_timeout;

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("maximum_latency 0x0000 to 0xFFFE (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    maximum_latency = (UINT16)read_val;

    printf("minimum_remote_timeout 0x0000 to 0xFFFE (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    minimum_remote_timeout = (UINT16)read_val;

    printf("minimum_local_timeout 0x0000 to 0xFFFE (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    minimum_local_timeout = (UINT16)read_val;

    retval = BT_hci_sniff_subrating
             (
                 connection_handle,
                 maximum_latency,
                 minimum_remote_timeout,
                 minimum_local_timeout
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#endif /* BT_SSR */
}

void appl_hci_qos_setup(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    HCI_QOS qos;

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("Token rate (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    qos.token_rate = (UINT32)read_val;

    printf("Peak bandwidth (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    qos.peak_bandwidth = (UINT32)read_val;

    printf("latency (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    qos.latency = (UINT32)read_val;

    printf("delay variation (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    qos.delay_variation = (UINT32)read_val;

    printf("service type(in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    qos.service_type = (UCHAR)read_val;

    retval = BT_hci_qos_setup
             (
                 connection_handle,
                 &qos
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("QOS setup command sent\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_change_esco_packet_type(void)
{
#ifdef BR_EDR_HCI
    int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    HCI_SCO_IN_PARAMS esco_params;
    UINT16 packet_type;

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("packet type (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    packet_type = (UINT16)read_val;

    appl_hci_get_esco_channel_parameters(&esco_params);
    esco_params.packet_type = packet_type;

    retval = BT_hci_setup_synchronous_connection
             (
                 connection_handle,
                 &esco_params
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("change packet type command sent\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_change_packet_type(void)
{
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16 packet_type;

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("packet type (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    packet_type = (UINT16)read_val;

    retval = BT_hci_change_connection_packet_type
             (
                 connection_handle,
                 packet_type
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("change packet type command sent\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_park_mode(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  beacon_max_interval;
    UINT16  beacon_min_interval;

    printf("Park Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("Beacon Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    beacon_max_interval = (UINT16)read_val;

    printf("Beacon Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    beacon_min_interval = (UINT16)read_val;

    retval = BT_hci_park_mode
             (
                 connection_handle,
                 beacon_max_interval,
                 beacon_min_interval
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_hold_mode(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  hold_max_interval;
    UINT16  hold_min_interval;

    printf("Hold Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("Hold Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    hold_max_interval = (UINT16)read_val;

    printf("Hold Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    hold_min_interval = (UINT16)read_val;

    retval = BT_hci_hold_mode
             (
                 connection_handle,
                 hold_max_interval,
                 hold_min_interval
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_exit_sniff_mode(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;

    printf("Exit Sniff Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    retval = BT_hci_exit_sniff_mode
             (
                 connection_handle
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_exit_park_mode(void)
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;

    printf("Exit Park Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    retval = BT_hci_exit_park_mode
             (
                 connection_handle
             );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_read_link_policy ( void )
{
#ifdef BR_EDR_HCI
    API_RESULT retval;
    unsigned int read_val;

    UINT16  connection_handle;

    printf("\n");
    printf("Initiating HCI Read link policy Activity ... ");
    fflush(stdout);

    printf("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    retval = BT_hci_read_link_policy_settings(connection_handle);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_write_link_policy(void)
{
    unsigned int read_val;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  link_policy_settings;

    printf("Link Policy Settings. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("Link Policy Settings (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    link_policy_settings = (UINT16)read_val;

    retval = BT_hci_write_link_policy_settings
        (
        connection_handle,
        link_policy_settings
        );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_default_link_policy_settings ( void )
{
    API_RESULT retval;

    printf("\n");
    printf ("Read Default Link Policy Settings\n");

    retval = BT_hci_read_default_link_policy_settings();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_write_default_link_policy_settings ( void )
{
	UINT16  input_option;

    UINT32 read_val;
    API_RESULT retval;

    read_val = 0x05U;

    printf("Select Default Link Policy Setting(0/1)\n");
    scanf("%d", &input_option);

    if (0 != input_option)
    {
        printf("Enter the Default Link Policy Setting Value(in Hex)\n");
        scanf("%x", &read_val);
    }

    printf("\n");
    printf ("Write Default Link Policy Settings\n");

    retval = BT_hci_write_default_link_policy_settings((UINT16)read_val);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_connection_accept_timeout ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Connection Accept Timeout ... ");
    fflush(stdout);

    retval = BT_hci_read_connection_accept_timeout();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}

void appl_hci_write_connection_accept_timeout ( void )
{
#ifdef BR_EDR_HCI
#ifndef HCI_LITE

    unsigned int read_val;
    UINT16 timeout;
    API_RESULT retval;

    printf("Enter Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    timeout = (UINT16)read_val;

    printf("\n");
    printf("Initiating HCI Write Connection Accept Timeout ... ");
    fflush(stdout);

    retval = BT_hci_write_connection_accept_timeout ( timeout );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_read_page_timeout ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Page Timeout ... ");
    fflush(stdout);

    retval = BT_hci_read_page_timeout();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_timeout ( void )
{
    unsigned int read_val;
    UINT16 timeout;
    API_RESULT retval;

    printf("Enter Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    timeout = (UINT16)read_val;

    printf("\n");
    printf("Initiating HCI Write Page Timeout ... ");
    fflush(stdout);

    retval = BT_hci_write_page_timeout ( timeout );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_page_scan_activity ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Page Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_read_page_scan_activity();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_scan_activity ( void )
{
    unsigned int read_val;
    UINT16 interval, window;
    API_RESULT retval;

    printf("Enter Page Scan Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    interval = (UINT16)read_val;

    printf("Enter Page Scan Window (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    window = (UINT16)read_val;

    printf("\n");
    printf("Initiating HCI Write Page Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_write_page_scan_activity ( interval, window );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_inquiry_scan_activity ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Inquiry Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_read_inquiry_scan_activity();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_scan_activity ( void )
{
    unsigned int read_val;
    UINT16 interval, window;
    API_RESULT retval;

    printf("Enter Inquiry Scan Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    interval = (UINT16)read_val;

    printf("Enter Inquiry Scan Window (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    window = (UINT16)read_val;

    printf("\n");
    printf("Initiating HCI Write Inquiry Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_write_inquiry_scan_activity ( interval, window );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_class_of_device ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read CoD ... "); fflush(stdout);
    retval = BT_hci_read_class_of_device();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


API_RESULT appl_hci_write_class_of_device ( void )
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;
    UINT32     class_of_device;
    UCHAR      cod_str[10];
    int        i,cod_length;

    /* Init */
    retval = API_SUCCESS;

    printf("Enter CoD (in Hex) = "); fflush(stdout);
    scanf ("%s", cod_str);
    cod_length = BT_str_len (cod_str);
    for (i = 0; i < cod_length; i++)
    {
        if (!((cod_str[i] >= '0' && cod_str[i] <= '9') || (cod_str[i] >= 'A' && cod_str[i] <= 'F') || \
            (cod_str[i] >= 'a' && cod_str[i] <= 'f')) || (cod_length != 6))
        {
            printf("Invalid CoD \n");
            retval = API_FAILURE; break; /* return API_FAILURE; */
        }
    }

    if (API_SUCCESS == retval)
    {
        read_val = appl_str_to_hexnum(cod_str, (UINT16)cod_length);
        printf("choice is 0x%04X\n",read_val);
        class_of_device = (UINT32)read_val;

        printf("\n");
        printf("Initiating HCI Write CoD ... "); fflush(stdout);
        retval = BT_hci_write_class_of_device ( class_of_device );
        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Successfully started.\n");
            printf("Please wait for Command Complete.\n");
        }
    }

    return retval;
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
    return API_SUCCESS;
#endif /* BR_EDR_HCI */
}


void appl_hci_read_scan_enable ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Scan Enable ... "); fflush(stdout);
    retval = BT_hci_read_scan_enable();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_scan_enable ( void )
{
    unsigned int read_val;
    API_RESULT retval;
    UCHAR      scan_enable;

    printf("Enter Scan Enable (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    scan_enable = (UCHAR)read_val;

    printf("\n");
    printf("Initiating HCI Write Scan Enable ... "); fflush(stdout);
    retval = BT_hci_write_scan_enable ( scan_enable );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_local_name ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Local Name ... "); fflush(stdout);
    retval = BT_hci_read_local_name();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_change_local_name ( void )
{
#ifdef BR_EDR_HCI
    API_RESULT retval;
    CHAR       name[249U];

    BT_mem_set(name, 0x0, 249U);
    printf("Enter Local Name (No Space allowed) = ");
    scanf("%s", name);

    printf("\n");
    printf("Initiating HCI Change Local Name ... "); fflush(stdout);
    retval = BT_hci_change_local_name ( (UCHAR *)name, (UINT16)BT_str_len((CHAR *)name) );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_get_connection_details ( void )
{
    API_RESULT retval;
    UCHAR i, j, num_list;
    HCI_CONNECTION_LIST hci_conn_list[BT_MAX_REMOTE_DEVICES];

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    num_list = 0U;
    BT_mem_set(hci_conn_list, 0, sizeof(HCI_CONNECTION_LIST)*BT_MAX_REMOTE_DEVICES);

    printf("Getting Connection Details from HCI ... "); fflush(stdout);
    retval = BT_hci_get_connection_details
             (hci_conn_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         printf("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        printf("Succeeded.\n");
        for (i = 0U; i < num_list; i ++)
        {
            printf("Connection Detail %d:\n", i);
            printf("  BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
            hci_conn_list[i].bd_addr[0U], hci_conn_list[i].bd_addr[1U],
            hci_conn_list[i].bd_addr[2U], hci_conn_list[i].bd_addr[3U],
            hci_conn_list[i].bd_addr[4U], hci_conn_list[i].bd_addr[5U]);
            printf("  ACL Handle = 0x%04X\n", hci_conn_list[i].acl_handle);
            printf("  SCO Handles = ");
            for (j = 0U; j < HCI_MAX_SCO_CHANNELS; j ++)
            {
                printf("0x%04X ", hci_conn_list[i].sco_handle[j]);
            }
            printf("\n");
            printf("  Current Mode = 0x%02X\n", hci_conn_list[i].current_mode);
        }
    }
}

#ifdef HCI_READ_BUFFER_SIZE_SUPPORT

void appl_hci_read_buffersize ( void )
{
    API_RESULT retval;

    printf("\n");
    printf("Initiating HCI Read Buffer Size ... "); fflush(stdout);
    retval = BT_hci_read_buffer_size();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}
#endif /* HCI_READ_BUFFER_SIZE_SUPPORT */

#ifdef BT_HCI_1_2

void appl_hci_read_inquiry_mode ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Starting Read Inquiry Mode ... "); fflush(stdout);
    retval = BT_hci_read_inquiry_mode();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_mode ( void )
{
#ifdef BR_EDR_HCI
#ifndef HCI_LITE

    unsigned int read_val;
    API_RESULT retval;
    UCHAR mode;

    printf("Inquiry Mode (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    mode = (UCHAR)read_val;

    printf("\n");
    printf("Starting Write Inquiry Mode ... "); fflush(stdout);
    retval = BT_hci_write_inquiry_mode ( mode );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}


void appl_hci_read_inquiry_scan_type ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Starting Read Inquiry Scan Type ... "); fflush(stdout);
    retval = BT_hci_read_inquiry_scan_type();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_scan_type ( void )
{
    unsigned int read_val;
    API_RESULT retval;
    UCHAR scan_type;

    printf("Inquiry Scan Type (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    scan_type = (UCHAR)read_val;

    printf("\n");
    printf("Starting Write Inquiry Scan Type ... "); fflush(stdout);
    retval = BT_hci_write_inquiry_scan_type ( scan_type );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_cancel_inquiry_scan ( void )
{
#ifdef BR_EDR_HCI
    API_RESULT retval;

    printf("canceling Inquiry Scan...\n"); fflush(stdout);
    retval = BT_hci_inquiry_cancel();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully stopped inquiry.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

void appl_hci_read_page_scan_type ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Starting Read Page Scan Type ... "); fflush(stdout);
    retval = BT_hci_read_page_scan_type();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_scan_type ( void )
{
    unsigned int read_val;
    API_RESULT retval;
    UCHAR scan_type;

    printf("Page Scan Type (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    scan_type = (UCHAR)read_val;

    printf("\n");
    printf("Starting Write Page Scan Type ... "); fflush(stdout);
    retval = BT_hci_write_page_scan_type ( scan_type );
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_current_iaclap ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    printf("\n");
    printf("Starting Read Current IAC LAP ... "); fflush(stdout);
    retval = BT_hci_read_current_iac_lap();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    printf("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}

void appl_hci_write_current_iaclap ( void )
{
#ifdef BR_EDR_HCI
    unsigned int read_val;
    API_RESULT retval;
    UINT32 iac_lap[4U];
    UCHAR num_iac, index;

    printf("Number of Current IAC LAP (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    num_iac = (UCHAR)read_val;

    if (4U < num_iac)
    {
        num_iac = 4U;
    }

    for (index = 0U; index < num_iac; index++)
    {
        printf("Current IAC LAP (in Hex) = "); fflush(stdout);
        scanf("%x", &read_val);
        iac_lap[index] = (UINT32)read_val;
    }

    printf("\n");
    printf("Starting Write Current IAC LAP ... "); fflush(stdout);
    retval = BT_hci_write_current_iac_lap (num_iac, iac_lap);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
#else
    printf("Feature flag BR_EDR_HCI is not defined\n");
#endif /* BR_EDR_HCI */
}

#endif /* BT_HCI_1_2 */

#ifdef BT_3_0

void appl_hci_read_enhanced_transmit_power_level (void)
{
    unsigned int read_val;
    API_RESULT retval;
    UINT16     handle;
    UCHAR      power_type;

    printf("Read Enhanced Transmit Power Level. Enter Required Information ...\n");

    printf("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    handle = (UINT16)read_val;
    printf("\n");

    printf("Power Type (in Hex) [0x00: Current, 0x01: Max] = "); fflush(stdout);
    scanf("%x", &read_val);
    power_type = (UCHAR)read_val;
    printf("\n");

    printf("Reading Enhanced Transmit Power Level ... "); fflush(stdout);
    retval = BT_hci_read_enhanced_transmit_power_level (handle, power_type);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}


#ifndef HCI_LITE
void appl_hci_read_encryption_key_size (void)
{
    unsigned int read_val;
    API_RESULT retval;
    UINT16     handle;

    printf("Read Encryption Key Size. Enter Required Information ...\n");

    printf("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    handle = (UINT16)read_val;

    printf("\n");
    printf("Reading Encryption Key Size ... "); fflush(stdout);
    retval = BT_hci_read_encryption_key_size (handle);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}
#endif /* HCI_LITE */

#endif /* BT_3_0 */

#ifdef BT_BRSC
void appl_hci_write_secure_connections_host_support (void)
{
    unsigned int read_val;
    API_RESULT retval;
    UCHAR enable;

    printf("Write Secure Connections Host Support. Enter Required Information ...\n");

    printf("Support Enable (0-Disable, 1-Enable) = "); fflush(stdout);
    scanf("%d", &read_val);
    enable = (UCHAR)read_val;

    printf("Writing Secure Connections Host Support ... "); fflush(stdout);
    retval = BT_hci_write_secure_connections_host_support (enable);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_secure_connections_host_support (void)
{
    API_RESULT retval;

    printf("Reading Secure Connections Host Support ... "); fflush(stdout);
    retval = BT_hci_read_secure_connections_host_support ();
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}
#endif /* BT_BRSC */

#ifdef BT_BRSC_TEST
void appl_hci_write_secure_connections_test_mode (void)
{
    unsigned int read_val;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR dm1_acl_u_mode;
    UCHAR esco_loopback_mode;

    printf("Write Secure Connections Test Mode. Enter Required Information ...\n");

    printf("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read_val);
    connection_handle = (UINT16)read_val;

    printf("DM1 ACL-U Mode (0-disable, 1- enable) = "); fflush(stdout);
    scanf("%x", &read_val);
    dm1_acl_u_mode = (UCHAR)read_val;

    printf("ESCO Loopback Mode (0-Disable, 1-Enable) = "); fflush(stdout);
    scanf("%x", &read_val);
    esco_loopback_mode = (UCHAR)read_val;

    printf("Writing Secure Connections Test Mode ... "); fflush(stdout);

    retval = BT_hci_write_secure_connections_test_mode
            (
                connection_handle,
                dm1_acl_u_mode,
                esco_loopback_mode
            );

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Successfully started.\n");
        printf("Please wait for Command Complete.\n");
    }
}
#endif /* BT_BRSC_TEST */

