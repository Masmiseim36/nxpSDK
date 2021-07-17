
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

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_HCI_API" )
#include "iot_logging_setup.h"

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
#ifndef HCI_LITE

    UINT32 lap;
    UCHAR  inq_length;
    UCHAR  inq_resp;
    unsigned int read;
    API_RESULT retval;

    IotLogDebug("HCI Inquiry. Enter Required Information ..\n");

    IotLogDebug("LAP (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    lap = (UINT32)read;

    IotLogDebug("Inquiry Length (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    inq_length = (UCHAR)read;

    IotLogDebug("Num Responses (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    inq_resp = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating Bluetooth Inquiry ... "); fflush(stdout);

    retval = BT_hci_inquiry ( lap,
                              inq_length,
                              inq_resp );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Inquiry Results & Inquiry Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_create_connection ( void )
{
    unsigned int read;
    UINT16 clock_offset;
    API_RESULT retval;
    UCHAR bd_addr[BT_BD_ADDR_SIZE], role;

    IotLogDebug("Create ACL Connection. Enter Required Information ..\n");

    IotLogDebug("Remote BD_ADDR = "); fflush(stdout);
    appl_get_bd_addr(bd_addr);

    IotLogDebug("Enter Clock Offset (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    clock_offset = (UINT16)read;

    IotLogDebug("Allow Role Switch (0/1) = "); fflush(stdout);
    scanf("%x", &read);
    role = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting ACL Connection ... "); fflush(stdout);

    retval = BT_hci_create_connection ( bd_addr,
                                        LMP_ACL_DH5,
                                        0x0,
                                        0x0,
                                        clock_offset,
                                        role );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Connect Complete.\n");
    }
}


void appl_hci_disconnect ( void )
{
    unsigned int read;
    API_RESULT retval;
    UCHAR      reason;
    UINT16     handle;

    IotLogDebug("Disconnect ACL/SCO Connection. Enter Required Information ...\n");

    IotLogDebug("ACL/SCO Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("Reason for Disconnection (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    reason = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting ACL/SCO Disconnection ... "); fflush(stdout);
    retval = BT_hci_disconnect ( handle, reason );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Disconnect Complete.\n");
    }
}


void appl_hci_add_sco_connection ( void )
{
    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE];

    IotLogDebug("Add SCO Connection. Enter Required Information ...\n");

    IotLogDebug("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    appl_get_bd_addr(bd_addr);

    IotLogDebug("\n");
    IotLogDebug("Starting Add SCO Connection ... "); fflush(stdout);
    retval = BT_hci_add_sco_connection ( bd_addr,
                                         LMP_SCO_HV1 );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Connect Complete.\n");
    }
}


void appl_hci_setup_synchronous_connection ( void )
{
    int choice;
    API_RESULT retval;
    UINT16 handle;
    HCI_SCO_IN_PARAMS esco_params;

    IotLogDebug("Setup Synchronous Connection. Enter Required Information ...\n");

    IotLogDebug("ACL Handle of the Remote Device (in Hex) = ");
    scanf ("%x", &choice);
    handle = (UINT16)choice;

    IotLogDebug("CVSD or mSBC (1/2)?: ");
    scanf("%d", &choice);

    if (1 == choice)
    {
        esco_params.tx_bandwidth  = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth  = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = 0x000C;
        esco_params.voice_setting = LMP_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_RETX_EFFORT_QUALITY;
        esco_params.packet_type = (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5);
    }
    else if (2 == choice)
    {
        esco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        esco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        esco_params.max_latency = 0x000D;
        esco_params.voice_setting = APPL_WIDEBAND_VOICE_SETTING_DEFAULT;
        esco_params.rtx_effort = LMP_RETX_EFFORT_QUALITY;
        esco_params.packet_type = (LMP_ESCO_3_EV3 | LMP_ESCO_2_EV5 | LMP_ESCO_3_EV5);
    }
    else if (3 == choice)
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

    IotLogDebug("\n");
    IotLogDebug("Starting Setup Synchronous Connection ... ");
    retval = BT_hci_setup_synchronous_connection (handle, &esco_params);

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Connect Complete.\n");
    }
}

void appl_hci_setup_synchronous_connection_with_codec_type (UINT8 type)
{
    int read;
    API_RESULT retval;
    UINT16 handle;
    HCI_SCO_IN_PARAMS esco_params;

    IotLogDebug("Setup Synchronous Connection. Enter Required Information ...\n");

    IotLogDebug("ACL Handle of the Remote Device (in Hex) = ");
    scanf("%x", &read);
    handle = (UINT16)read;

    if (2 == type)
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

    IotLogDebug("\n");
    IotLogDebug("Starting Setup Synchronous Connection ... ");
    retval = BT_hci_setup_synchronous_connection (handle, &esco_params);

    if (retval != API_SUCCESS)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Connect Complete.\n");
    }
}

void appl_hci_setup_synchronous_connection_response ( void )
{
#ifdef HCI_NO_ESCO_AUTO_ACCEPT
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR reject;

    IotLogDebug("Synchronous Connection Response. Enter Required Information ...\n");

    IotLogDebug("BD_ADDR of the Remote Device (in Hex) = ");
    appl_get_bd_addr(bd_addr);

    IotLogDebug("Accept/Reject (0 - Accept, <ErrorCode> - Reject) (in HEX) = ");
    scanf ("%x", &reject);

    IotLogDebug("\n");
    IotLogDebug("Starting Synchronous Connection Response ... ");
    BT_hci_esco_connection_response ( bd_addr, reject );

    IotLogDebug("Successfully started.\n");
    IotLogDebug("Please wait for Connect Complete.\n");
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
}


#ifdef HCI_ENH_SCO
void appl_hci_read_local_supported_codecs ( void )
{
    API_RESULT retval;

    IotLogDebug ("Read local supported codecs ...");
    retval = BT_hci_read_local_supported_codecs ();

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_enhanced_setup_synchronous_connection ( void )
{
    API_RESULT retval;
    UINT16 handle;
    HCI_ENH_SCO_PARAMS esco_params;
    int read_input;

    IotLogDebug("Enhanced Setup Synchronous Connection. Enter Required Information ...\n");

    IotLogDebug("ACL/eSCO Handle with the Remote Device (in Hex) = ");
    scanf("%x", &read_input);

    handle = (UINT16)read_input;

    /* Set parameters for mSBC */
    esco_params.tx_bandwidth = LMP_ENH_SCO_TX_BANDWIDTH_DEFAULT;
    esco_params.rx_bandwidth = LMP_ENH_SCO_RX_BANDWIDTH_DEFAULT;
    esco_params.max_latency  = LMP_ESCO_MAX_LATENCY_DEFAULT;
    esco_params.packet_type  = LMP_ESCO_PACKET_TYPE_DEFAULT;
    esco_params.retransmission_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;

    /**
     *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
     *  Company ID and Vendor Specific Codec ID shall be ignored.
     */
    esco_params.tx_coding_frmt.coding_format   = LMP_CODING_FRMT_TRANSPARENT;
    esco_params.tx_coding_frmt.company_id      = 0x02; /* dummy */
    esco_params.tx_coding_frmt.vendor_codec_id = 0x01; /* dummy */

    esco_params.rx_coding_frmt.coding_format   = LMP_CODING_FRMT_TRANSPARENT;
    esco_params.rx_coding_frmt.company_id      = 0x02; /* dummy */
    esco_params.rx_coding_frmt.vendor_codec_id = 0x01; /* dummy */

    /* Configuring for mSBC codec */
    esco_params.tx_codec_frame_size = MSBC_FRAME_SIZE;
    esco_params.rx_codec_frame_size = MSBC_FRAME_SIZE;

    esco_params.input_bandwidth  = LMP_ENH_SCO_RX_BANDWIDTH_DEFAULT;
    esco_params.output_bandwidth = LMP_ENH_SCO_TX_BANDWIDTH_DEFAULT;

    /**
     *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
     *  Company ID and Vendor Specific Codec ID shall be ignored.
     */
    esco_params.input_coding_frmt.coding_format   = LMP_CODING_FRMT_MSBC;
    esco_params.input_coding_frmt.company_id      = 0x02; /* dummy */
    esco_params.input_coding_frmt.vendor_codec_id = 0x01; /* dummy */

    esco_params.output_coding_frmt.coding_format   = LMP_CODING_FRMT_MSBC;
    esco_params.output_coding_frmt.company_id      = 0x02; /* dummy */
    esco_params.output_coding_frmt.vendor_codec_id = 0x01; /* dummy */

    /**
     * mSBC encoded data is a byte stream. so coded unit size is 8 bits
     */
    esco_params.input_coded_data_size  = 8;
    esco_params.output_coded_data_size = 8;

    /**
     * Below parameters are valide when
     * coding_format equal to LMP_CODING_FRMT_LINEAR_PCM
     */
    esco_params.input_pcm_data_frmt  = 0xFF; /* dummy */
    esco_params.output_pcm_data_frmt = 0xFF; /* dummy */
    esco_params.input_pcm_smpl_payload_msb_pos  = 0x00; /* dummy */
    esco_params.output_pcm_smpl_payload_msb_pos = 0x00; /* dummy */

    /* Exchange SCO packets over HCI transport */
    esco_params.input_data_path  = ENH_SCO_DATA_PATH_HCI;
    esco_params.output_data_path = ENH_SCO_DATA_PATH_HCI;

    /**
     * As per Spec., if SCO packet are exchanged over HCI transport, then
     * below parameters shall be zero
     */
    esco_params.input_transport_unit_size  = 0x00;
    esco_params.output_transport_unit_size = 0x00;

    IotLogDebug("\n");
    IotLogDebug("Starting Enhanced Setup Synchronous Connection ... ");
    retval = BT_hci_setup_enh_synchronous_connection (handle, &esco_params);

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Connect Complete.\n");
    }
}


void appl_hci_enhanced_setup_synchronous_connection_response ( void )
{
#ifdef HCI_NO_ESCO_AUTO_ACCEPT
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR reject;

    IotLogDebug("Enhanced Setup Synchronous Connection Response. Enter Required Information ...\n");

    IotLogDebug("BD_ADDR of the Remote Device (in Hex) = ");
    appl_get_bd_addr(bd_addr);

    IotLogDebug("Accept/Reject (0 - Accept, <ErrorCode> - Reject) (in HEX) = ");
    scanf ("%x", &reject);

    IotLogDebug("\n");
    IotLogDebug("Starting Enhanced Synchronous Connection Response ... ");
    BT_hci_enh_sco_connection_response ( bd_addr, reject );

    IotLogDebug("Successfully started.\n");
    IotLogDebug("Please wait for Connect Complete.\n");
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
}
#endif /* HCI_ENH_SCO */


void appl_hci_remote_name_request ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE];
    UCHAR      name[249];

    BT_mem_set(name, 0x0, 249);

    IotLogDebug("Remote Name Request. Enter Required Information ...\n");

    IotLogDebug("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    appl_get_bd_addr(bd_addr);

    IotLogDebug("\n");
    IotLogDebug("Starting Remote Name Request ... "); fflush(stdout);
    retval = BT_hci_remote_name_request ( bd_addr,
                                          0x00,
                                          0x00,
                                          0x0000 );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_role_discovery ( void )
{
#ifndef HCI_LITE

    unsigned int read;
    API_RESULT retval;
    UINT16     handle;

    IotLogDebug("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Role Discovery ... "); fflush(stdout);
    retval = BT_hci_role_discovery ( handle );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_switch_role ( void )
{
    unsigned int read;
    API_RESULT retval;
    UCHAR      bd_addr[BT_BD_ADDR_SIZE], role;

    IotLogDebug("BD_ADDR of the Remote Device (in Hex) = "); fflush(stdout);
    appl_get_bd_addr(bd_addr);

    IotLogDebug("Enter Role (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    role = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Switch Role ... "); fflush(stdout);
    retval = BT_hci_switch_role ( bd_addr, role );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_sniff_mode(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  sniff_max_interval;
    UINT16  sniff_min_interval;
    UINT16  sniff_attempt;
    UINT16  sniff_timeout;

    IotLogDebug("Sniff Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Sniff Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    sniff_max_interval = (UINT16)read;

    IotLogDebug("Sniff Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    sniff_min_interval = (UINT16)read;

    IotLogDebug("Sniff Attempt (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    sniff_attempt = (UINT16)read;

    IotLogDebug("Sniff Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    sniff_timeout = (UINT16)read;

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
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_park_mode(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  beacon_max_interval;
    UINT16  beacon_min_interval;

    IotLogDebug("Park Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Beacon Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    beacon_max_interval = (UINT16)read;

    IotLogDebug("Beacon Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    beacon_min_interval = (UINT16)read;

    retval = BT_hci_park_mode
             (
                 connection_handle,
                 beacon_max_interval,
                 beacon_min_interval
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_hold_mode(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  hold_max_interval;
    UINT16  hold_min_interval;

    IotLogDebug("Hold Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Hold Max Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    hold_max_interval = (UINT16)read;

    IotLogDebug("Hold Min Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    hold_min_interval = (UINT16)read;

    retval = BT_hci_hold_mode
             (
                 connection_handle,
                 hold_max_interval,
                 hold_min_interval
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_exit_sniff_mode(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;

    IotLogDebug("Exit Sniff Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    retval = BT_hci_exit_sniff_mode
             (
                 connection_handle
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_exit_park_mode(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;

    IotLogDebug("Exit Park Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    retval = BT_hci_exit_park_mode
             (
                 connection_handle
             );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_write_link_policy(void)
{
    unsigned int read;
    API_RESULT retval;

    UINT16  connection_handle;
    UINT16  link_policy_settings;

    IotLogDebug("Link Policy Settings. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("Link Policy Settings (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    link_policy_settings = (UINT16)read;

    retval = BT_hci_write_link_policy_settings
        (
        connection_handle,
        link_policy_settings
        );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_connection_accept_timeout ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Connection Accept Timeout ... ");
    fflush(stdout);

    retval = BT_hci_read_connection_accept_timeout();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}

void appl_hci_write_connection_accept_timeout ( void )
{
#ifndef HCI_LITE

    unsigned int read;
    UINT16 timeout;
    API_RESULT retval;

    IotLogDebug("Enter Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    timeout = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write Connection Accept Timeout ... ");
    fflush(stdout);

    retval = BT_hci_write_connection_accept_timeout ( timeout );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_read_page_timeout ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Page Timeout ... ");
    fflush(stdout);

    retval = BT_hci_read_page_timeout();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_timeout ( void )
{
    unsigned int read;
    UINT16 timeout;
    API_RESULT retval;

    IotLogDebug("Enter Timeout (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    timeout = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write Page Timeout ... ");
    fflush(stdout);

    retval = BT_hci_write_page_timeout ( timeout );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_page_scan_activity ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Page Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_read_page_scan_activity();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_scan_activity ( void )
{
    unsigned int read;
    UINT16 interval, window;
    API_RESULT retval;

    IotLogDebug("Enter Page Scan Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    interval = (UINT16)read;

    IotLogDebug("Enter Page Scan Window (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    window = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write Page Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_write_page_scan_activity ( interval, window );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_inquiry_scan_activity ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Inquiry Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_read_inquiry_scan_activity();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_scan_activity ( void )
{
    unsigned int read;
    UINT16 interval, window;
    API_RESULT retval;

    IotLogDebug("Enter Inquiry Scan Interval (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    interval = (UINT16)read;

    IotLogDebug("Enter Inquiry Scan Window (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    window = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write Inquiry Scan Activity ... ");
    fflush(stdout);

    retval = BT_hci_write_inquiry_scan_activity ( interval, window );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_class_of_device ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read CoD ... "); fflush(stdout);
    retval = BT_hci_read_class_of_device();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_class_of_device ( void )
{
    unsigned int read;
    API_RESULT retval;
    UINT32      class_of_device;

    IotLogDebug("Enter CoD (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    class_of_device = (UINT32)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write CoD ... "); fflush(stdout);
    retval = BT_hci_write_class_of_device ( class_of_device );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_scan_enable ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Scan Enable ... "); fflush(stdout);
    retval = BT_hci_read_scan_enable();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_scan_enable ( void )
{
    unsigned int read;
    API_RESULT retval;
    UCHAR      scan_enable;

    IotLogDebug("Enter Scan Enable (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    scan_enable = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Write Scan Enable ... "); fflush(stdout);
    retval = BT_hci_write_scan_enable ( scan_enable );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_local_name ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Read Local Name ... "); fflush(stdout);
    retval = BT_hci_read_local_name();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_change_local_name ( void )
{
    API_RESULT retval;
    CHAR       name[249];

    BT_mem_set(name, 0x0, 249);
    IotLogDebug("Enter Local Name (No Space allowed) = ");
    scanf("%s", name);

    IotLogDebug("\n");
    IotLogDebug("Initiating HCI Change Local Name ... "); fflush(stdout);
    retval = BT_hci_change_local_name ( (UCHAR *)name, (UINT16)BT_str_len((CHAR *)name) );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_get_connection_details ( void )
{
    API_RESULT retval;
    UCHAR i, j, num_list;
    HCI_CONNECTION_LIST hci_conn_list[BT_MAX_REMOTE_DEVICES];

    IotLogDebug("Getting Connection Details from HCI ... "); fflush(stdout);
    retval = BT_hci_get_connection_details
             (hci_conn_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         IotLogDebug("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        IotLogDebug("Succeeded.\n");
        for (i = 0; i < num_list; i ++)
        {
            IotLogDebug("Connection Detail %d:\n", i);
            IotLogDebug("  BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
            hci_conn_list[i].bd_addr[0], hci_conn_list[i].bd_addr[1],
            hci_conn_list[i].bd_addr[2], hci_conn_list[i].bd_addr[3],
            hci_conn_list[i].bd_addr[4], hci_conn_list[i].bd_addr[5]);
            IotLogDebug("  ACL Handle = 0x%04X\n", hci_conn_list[i].acl_handle);
            IotLogDebug("  SCO Handles = ");
            for (j = 0; j < HCI_MAX_SCO_CHANNELS; j ++)
            {
                IotLogDebug("0x%04X ", hci_conn_list[i].sco_handle[j]);
            }
            IotLogDebug("\n");
            IotLogDebug("  Current Mode = 0x%02X\n", hci_conn_list[i].current_mode);
        }
    }
}


#ifdef BT_HCI_1_2

void appl_hci_read_inquiry_mode ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Starting Read Inquiry Mode ... "); fflush(stdout);
    retval = BT_hci_read_inquiry_mode();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_mode ( void )
{
#ifndef HCI_LITE

    unsigned int read;
    API_RESULT retval;
    UCHAR mode;

    IotLogDebug("Inquiry Mode (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    mode = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Write Inquiry Mode ... "); fflush(stdout);
    retval = BT_hci_write_inquiry_mode ( mode );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_read_inquiry_scan_type ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Starting Read Inquiry Scan Type ... "); fflush(stdout);
    retval = BT_hci_read_inquiry_scan_type();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_inquiry_scan_type ( void )
{
    unsigned int read;
    API_RESULT retval;
    UCHAR scan_type;

    IotLogDebug("Inquiry Scan Type (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    scan_type = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Write Inquiry Scan Type ... "); fflush(stdout);
    retval = BT_hci_write_inquiry_scan_type ( scan_type );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


void appl_hci_read_page_scan_type ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Starting Read Page Scan Type ... "); fflush(stdout);
    retval = BT_hci_read_page_scan_type();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}


void appl_hci_write_page_scan_type ( void )
{
    unsigned int read;
    API_RESULT retval;
    UCHAR scan_type;

    IotLogDebug("Page Scan Type (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    scan_type = (UCHAR)read;

    IotLogDebug("\n");
    IotLogDebug("Starting Write Page Scan Type ... "); fflush(stdout);
    retval = BT_hci_write_page_scan_type ( scan_type );
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_current_iaclap ( void )
{
#ifndef HCI_LITE

    API_RESULT retval;

    IotLogDebug("\n");
    IotLogDebug("Starting Read Current IAC LAP ... "); fflush(stdout);
    retval = BT_hci_read_current_iac_lap();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }

#else  /* HCI_LITE */

    IotLogDebug("HCI_LITE flag is defined\n");

#endif /* HCI_LITE */
}

void appl_hci_write_current_iaclap ( void )
{
    unsigned int read;
    API_RESULT retval;
    UINT32 iac_lap;

    IotLogDebug("Current IAC LAP (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    iac_lap = read;

    IotLogDebug("\n");
    IotLogDebug("Starting Write Current IAC LAP ... "); fflush(stdout);
    retval = BT_hci_write_current_iac_lap (1, &iac_lap);
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

#endif /* BT_HCI_1_2 */

#ifdef BT_3_0

void appl_hci_read_enhanced_transmit_power_level (void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16     handle;
    UCHAR      power_type;

    IotLogDebug("Read Enhanced Transmit Power Level. Enter Required Information ...\n");

    IotLogDebug("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;
    IotLogDebug("\n");

    IotLogDebug("Power Type (in Hex) [0x00: Current, 0x01: Max] = "); fflush(stdout);
    scanf("%x", &read);
    power_type = (UCHAR)read;
    IotLogDebug("\n");

    IotLogDebug("Reading Enhanced Transmit Power Level ... "); fflush(stdout);
    retval = BT_hci_read_enhanced_transmit_power_level (handle, power_type);
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}


#ifndef HCI_LITE
void appl_hci_read_encryption_key_size (void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16     handle;

    IotLogDebug("Read Encryption Key Size. Enter Required Information ...\n");

    IotLogDebug("ACL Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    handle = (UINT16)read;

    IotLogDebug("\n");
    IotLogDebug("Reading Encryption Key Size ... "); fflush(stdout);
    retval = BT_hci_read_encryption_key_size (handle);
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}
#endif /* HCI_LITE */

#endif /* BT_3_0 */

#ifdef BT_BRSC
void appl_hci_write_secure_connections_host_support (void)
{
    unsigned int read;
    API_RESULT retval;
    UCHAR enable;

    IotLogDebug("Write Secure Connections Host Support. Enter Required Information ...\n");

    IotLogDebug("Support Enable (0-Disable, 1-Enable) = "); fflush(stdout);
    scanf("%d", &read);
    enable = (UCHAR)read;

    IotLogDebug("Writing Secure Connections Host Support ... "); fflush(stdout);
    retval = BT_hci_write_secure_connections_host_support (enable);
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}

void appl_hci_read_secure_connections_host_support (void)
{
    API_RESULT retval;

    IotLogDebug("Reading Secure Connections Host Support ... "); fflush(stdout);
    retval = BT_hci_read_secure_connections_host_support ();
    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}
#endif /* BT_BRSC */

#ifdef BT_BRSC_TEST
void appl_hci_write_secure_connections_test_mode (void)
{
    unsigned int read;
    API_RESULT retval;
    UINT16 connection_handle;
    UCHAR dm1_acl_u_mode;
    UCHAR esco_loopback_mode;

    IotLogDebug("Write Secure Connections Test Mode. Enter Required Information ...\n");

    IotLogDebug("Connection Handle (in Hex) = "); fflush(stdout);
    scanf("%x", &read);
    connection_handle = (UINT16)read;

    IotLogDebug("DM1 ACL-U Mode (0-disable, 1- enable) = "); fflush(stdout);
    scanf("%x", &read);
    dm1_acl_u_mode = (UCHAR)read;

    IotLogDebug("ESCO Loopback Mode (0-Disable, 1-Enable) = "); fflush(stdout);
    scanf("%x", &read);
    esco_loopback_mode = (UCHAR)read;

    IotLogDebug("Writing Secure Connections Test Mode ... "); fflush(stdout);

    retval = BT_hci_write_secure_connections_test_mode
            (
                connection_handle,
                dm1_acl_u_mode,
                esco_loopback_mode
            );

    if (API_SUCCESS != retval)
    {
        IotLogDebug("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        IotLogDebug("Successfully started.\n");
        IotLogDebug("Please wait for Command Complete.\n");
    }
}
#endif /* BT_BRSC_TEST */

