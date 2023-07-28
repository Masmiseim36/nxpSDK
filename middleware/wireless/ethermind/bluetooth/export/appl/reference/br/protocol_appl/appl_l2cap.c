
/**
 *  \file appl_l2cap.c
 *
 *  Source File for l2cap Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* #define L2CAP_TESTER_MUTE_IUT */
/* #define L2CAP_ASSYMETRIC_PSM */

/* ---------------------------------------- Header File Inclusion */
#include "appl_l2cap.h"
#include "appl_utils.h"

#ifdef BR_EDR_L2CAP
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */
#if 1
DECL_STATIC BT_thread_mutex_type l2cap_dt_mutex;
DECL_STATIC BT_thread_cond_type l2cap_dt_cond;
#endif /* 0 */

/* TBD: Get remote BD_ADDR */
DECL_STATIC UCHAR l2cap_dt_remote_bd_addr[6U] = { 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U };

/* L2CAP PSM */
DECL_STATIC UINT16 l2cap_dt_local_psm  = 0x1231U;
DECL_STATIC UINT16 l2cap_dt_remote_psm = 0x1231U;

/* L2CAP DT Entities */
DECL_STATIC L2CAP_DT_ENTITY l2cap_dt_entity [ L2CAP_DT_MAX_ENTITY ];

/*
 *  InMTU & InFEC Configuration Parameters for L2CAP Channels
 *
 *  Index 0: default Config Options for Basic Mode
 *  Index 1: default Config Options for Retransmission Mode
 *  Index 2: default Config Options for Flow Control Mode
 *  Index 3: default Config Options for Enhanced Retransmission Mode
 *  Index 4: default Config Options for Streaming Mode
 */
DECL_STATIC L2CAP_DT_CONFIG_OPTION l2cap_dt_config_option [ 5U ];
DECL_STATIC UCHAR l2cap_dt_config_index = 0U;

/**
 * FCS configuration for a few L2CAP PTS Test Cases
 *
 * 1 : Use FCS
 * 0 : Do not use FCS
 */
DECL_STATIC UCHAR appl_l2cap_fcs = L2CAP_FCS_OPTION_DEFAULT;

/**
 * Basic Only Mode configuration for a few L2CAP PTS Test Cases
 *
 * 1 : Basic Only Mode (using while responding to connection request)
 * 0 : Not Basic Only Mode
 */
DECL_STATIC UCHAR appl_l2cap_basic_only_mode = 0U;

/**
 * SM Only Mode configuration for a few L2CAP PTS Test Cases
 *
 * 1 : SM Only Mode (using while responding to connection request)
 * 0 : Not SM Only Mode
 */
DECL_STATIC UCHAR appl_l2cap_sm_only_mode = 0U;

/*
 *  Extended Flow Specification Configuration
 *
 *  Index 0: Best Effort
 *  Index 1: Guaranteed
 */
#ifdef L2CAP_EXTENDED_FLOW_SPEC
DECL_STATIC L2CAP_EXT_FLOW_SPEC l2cap_dt_ext_flow_spec_option[2U];
DECL_STATIC UCHAR l2cap_dt_ext_flow_spec_index = 0U;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

/* To test Partial Config, enable this global */
DECL_STATIC UCHAR l2cap_dt_partial_config = 0x0U;

#ifdef L2CAP_TEST_PACKET_DROP
/* Packet Drop Configuration */
DECL_STATIC INT32 l2cap_dt_drop_percentage = 0x0U;
DECL_STATIC UCHAR l2cap_dt_drop_flag = 0x0U;
#endif /* L2CAP_TEST_PACKET_DROP */

/* ----------------------------------------- Static Global Variables */
static const char main_l2cap_options[] = " \n\
========= M A I N   M E N U ========= \n\
   0.  Exit. \n\
   1.  Refresh. \n\
\n\
   2.  Get Peer Address \n\
   3.  Register with L2CAP \n\
   4.  Create L2CAP Channel. \n\
   5.  Accept L2CAP Channel. \n\
   6.  Reconfigure L2CAP Channel. \n\
   7.  Destroy L2CAP Channel. \n\
   8.  Show L2CAP Channels.\n\
\n\
   10. Send L2CAP Data. \n\
   11. Send L2CAP Data (Random Size). \n\
   12. Send L2CAP Data (Unicast Connectionless). \n\
\n\
   15. Cancel Packet Drops \n\
   16. Config Packet Drop for all S-frames \n\
   17. Config Random Packet Drop of I & S-frames \n\
\n\
   20. Disable Rx Flow \n\
   21. Enable Rx Flow \n\
\n\
   22. Set Basic Mode Config Options \n\
   23. Set Retransmission Mode Config Options \n\
   24. Set Flow Control Mode Config Options \n\
   25. Set Enhanced Retransmission Mode Config Options \n\
   26. Set Streaming Mode Config Options \n\
\n\
   30. Set 'Best Effort' Extended Flow Specificiation Options \n\
   31. Set 'Guaranteed' Extended Flow Specificiation Options \n\
\n\
   40. Enable FCS\n\
   41. Disable FCS\n\
\n\
   50. Enable Basic Only Mode\n\
   51. Disable Basic Only Mode\n\
   52. Enable ERTM Only Mode\n\
   53. Disable ERTM Only Mode\n\
   54. Enable Streaming Only Mode\n\
   55. Disable Streaming Only Mode\n\
\n\
  100. Send Echo Request \n\
  101. Send Info Request \n\
Your Option ? ";



/* ----------------------------------------- Functions */

void l2cap_dt_config_option_init (void)
{
    /*
     *  Index 0: Basic Mode
     *
     *  InMTU = 672 (L2CAP's Default)
     *  InFEC->Mode = 0x00 (Basic Mode)
     */
    l2cap_dt_config_option[0U].in_mtu = L2CAP_MTU_DEFAULT;
    l2cap_dt_config_option[0U].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;
    l2cap_init_flow_spec_default (&l2cap_dt_config_option[0U].out_flow);

    l2cap_init_fec_option_default (&l2cap_dt_config_option[0U].in_fec);
    l2cap_dt_config_option[0U].in_fec.mode = L2CAP_MODE_BASIC;

    l2cap_dt_config_option[0U].out_fcs = appl_l2cap_fcs;

#ifndef BT_ENH_L2CAP
    /*
     *  Index 1: Retransmission Mode
     *
     *  InMTU = 672 (L2CAP's Default)
     *  InFEC->Mode = 0x01 (Retransmission Mode)
     *  InFEC->MPS  = 100 Bytes (Must be less than InMTU)
     *  InFEC->MaxTransmit = 5
     *  InFEC->TxWindow = 7 (Range is 1-32)
     *  InFEC->RetransmissionTimeout = 2 Secs (Value is in millisecs)
     *  InFEC->MonitorTimeout = 2 Secs (Value is in millisecs)
     */
    l2cap_dt_config_option[1U].in_mtu = L2CAP_MTU_DEFAULT;
    l2cap_dt_config_option[1U].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;
    l2cap_init_flow_spec_default (&l2cap_dt_config_option[1U].out_flow);

    if (l2cap_dt_partial_config == 0x1U)
    {
        l2cap_dt_config_option[1U].in_mtu = 800U;
        l2cap_dt_config_option[1U].out_flush_to = 0xABCDU;
        l2cap_init_flow_spec_default (&l2cap_dt_config_option[1U].out_flow);
        l2cap_dt_config_option[1U].out_flow.service_type = 0x02U;
    }

    l2cap_init_fec_option_default (&l2cap_dt_config_option[1U].in_fec);
    l2cap_dt_config_option[1U].in_fec.mode = L2CAP_MODE_RTM;
    l2cap_dt_config_option[1U].in_fec.mps  = 100U;
    l2cap_dt_config_option[1U].in_fec.max_transmit = 5U;
    l2cap_dt_config_option[1U].in_fec.tx_window = 7U;
    l2cap_dt_config_option[1U].in_fec.retx_timeout = 10000U;
    l2cap_dt_config_option[1U].in_fec.monitor_timeout = 2000U;

    l2cap_dt_config_option[1U].out_fcs = appl_l2cap_fcs;


    /*
     *  Index 2: Flow Control Mode
     *
     *  InMTU = 672 (L2CAP's Default)
     *  InFEC->Mode = 0x01 (Flow Control Mode)
     *  InFEC->MPS  = 100 Bytes (Must be less than InMTU)
     *  InFEC->MaxTransmit = 5
     *  InFEC->TxWindow = 7 (Range is 1-32)
     *  InFEC->RetransmissionTimeout = 2 Secs (Value is in millisecs)
     *  InFEC->MonitorTimeout = 2 Secs (Value is in millisecs)
     */
    l2cap_dt_config_option[2U].in_mtu = L2CAP_MTU_DEFAULT;
    l2cap_dt_config_option[2U].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;
    l2cap_init_flow_spec_default (&l2cap_dt_config_option[2U].out_flow);

    l2cap_init_fec_option_default (&l2cap_dt_config_option[2U].in_fec);
    l2cap_dt_config_option[2U].in_fec.mode = L2CAP_MODE_FC;
    l2cap_dt_config_option[2U].in_fec.mps  = 100U;
    l2cap_dt_config_option[2U].in_fec.max_transmit = 5U;
    l2cap_dt_config_option[2U].in_fec.tx_window = 7U;
    l2cap_dt_config_option[2U].in_fec.retx_timeout = 10000U;
    l2cap_dt_config_option[2U].in_fec.monitor_timeout = 2000U;

    l2cap_dt_config_option[2U].out_fcs = appl_l2cap_fcs;

#endif  /* !BT_ENH_L2CAP */


    /*
     *  Index 3: Enhanced Retransmission Mode
     *
     *  InMTU = 672 (L2CAP's Default)
     *  InFEC->Mode = 0x01 (Retransmission Mode)
     *  InFEC->MPS  = 100 Bytes (Must be less than InMTU)
     *  InFEC->MaxTransmit = 5
     *  InFEC->TxWindow = 7 (Range is 1-32)
     *  InFEC->RetransmissionTimeout = 2 Secs (Value is in millisecs)
     *  InFEC->MonitorTimeout = 2 Secs (Value is in millisecs)
     */
    l2cap_dt_config_option[3U].in_mtu = L2CAP_MTU_DEFAULT;
    l2cap_dt_config_option[3U].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;
    l2cap_init_flow_spec_default (&l2cap_dt_config_option[3U].out_flow);

    if (l2cap_dt_partial_config == 0x1U)
    {
        l2cap_dt_config_option[3U].in_mtu = 800U;
        l2cap_dt_config_option[3U].out_flush_to = 0xABCDU;
        l2cap_init_flow_spec_default (&l2cap_dt_config_option[3U].out_flow);
        l2cap_dt_config_option[3U].out_flow.service_type = 0x02U;
    }

    l2cap_init_fec_option_default (&l2cap_dt_config_option[3U].in_fec);
    l2cap_dt_config_option[3U].in_fec.mode = L2CAP_MODE_ERTM;
    l2cap_dt_config_option[3U].in_fec.mps  = 200U;
    l2cap_dt_config_option[3U].in_fec.max_transmit = 20U;
    l2cap_dt_config_option[3U].in_fec.tx_window = 10U;
    l2cap_dt_config_option[3U].in_fec.retx_timeout = 10000U;
    l2cap_dt_config_option[3U].in_fec.monitor_timeout = 2000U;

    l2cap_dt_config_option[3U].out_fcs = appl_l2cap_fcs;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    l2cap_dt_config_option[3U].out_ext_flow_spec = NULL;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

    /*
     *  Index 4: Streaming Mode
     *
     *  InMTU = 672 (L2CAP's Default)
     *  InFEC->Mode = 0x01 (Retransmission Mode)
     *  InFEC->MPS  = 100 Bytes (Must be less than InMTU)
     *  InFEC->MaxTransmit = 5
     *  InFEC->TxWindow = 7 (Range is 1-32)
     *  InFEC->RetransmissionTimeout = 2 Secs (Value is in millisecs)
     *  InFEC->MonitorTimeout = 2 Secs (Value is in millisecs)
     */
    l2cap_dt_config_option[4U].in_mtu = L2CAP_MTU_DEFAULT;
    l2cap_dt_config_option[4U].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;
    l2cap_init_flow_spec_default (&l2cap_dt_config_option[4U].out_flow);

    if (l2cap_dt_partial_config == 0x1U)
    {
        l2cap_dt_config_option[4U].in_mtu = 800U;
        l2cap_dt_config_option[4U].out_flush_to = 0xABCDU;
        l2cap_init_flow_spec_default (&l2cap_dt_config_option[4U].out_flow);
        l2cap_dt_config_option[4U].out_flow.service_type = 0x02U;
    }

    l2cap_init_fec_option_default (&l2cap_dt_config_option[4U].in_fec);
    l2cap_dt_config_option[4U].in_fec.mode = L2CAP_MODE_SM;
    l2cap_dt_config_option[4U].in_fec.mps  = 100U;
    l2cap_dt_config_option[4U].in_fec.max_transmit = 5U;
    l2cap_dt_config_option[4U].in_fec.tx_window = 7U;
    l2cap_dt_config_option[4U].in_fec.retx_timeout = 10000U;
    l2cap_dt_config_option[4U].in_fec.monitor_timeout = 2000U;

    l2cap_dt_config_option[4U].out_fcs = appl_l2cap_fcs;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    l2cap_dt_config_option[3U].out_ext_flow_spec = NULL;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */
}

#ifdef L2CAP_EXTENDED_FLOW_SPEC
void l2cap_dt_ext_flow_spec_option_init(void)
{
    /*
     *  Index 0: Best Effort
     *
     *  Identifier = 0x01 (L2CAP's Default)
     *  Service Type =  0x01(Best Effort)
     *  Max SDU Size  = 0xFFFF
     *  SDU Inter-arrival Time = 0xFFFFFFFF
     *  Access Latency = 0xFFFFFFFF
     *  Flush Timeout = 0xFFFFFFFF
     */
    l2cap_init_ext_flow_spec_default(&l2cap_dt_ext_flow_spec_option[0U]);

    /*
     *  Index 1: Guaranteed
     *
     *  Identifier = 0x01 (L2CAP's Default)
     *  Service Type =  0x02
     *  Max SDU Size  = 672
     *  SDU Inter-arrival Time = 625 us
     *  Access Latency = 0xFFFFFFFF
     *  Flush Timeout = 0xFFFFFFFF
     */
    l2cap_init_ext_flow_spec_default(&l2cap_dt_ext_flow_spec_option[1U]);
    l2cap_dt_ext_flow_spec_option[1U].service_type = L2CAP_SERVICE_TYPE_GUARANTEED;
    l2cap_dt_ext_flow_spec_option[1U].id = 0x02U;
    l2cap_dt_ext_flow_spec_option[1U].max_sdu_size = L2CAP_MTU_DEFAULT;
    l2cap_dt_ext_flow_spec_option[1U].sdu_ia_time = 625U;

}
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

/* !! NOT USED !! */
#if 0

API_RESULT l2cap_upf_i_frame_rx (UCHAR tx_seq)
{
    /* Enable the Code Segment for the Testcase you are running */

#if 0
    /*
     *  FEC Test 01
     *  Flow Control Mode. Sequence Number higher than TxWindow,
     *  no acknowledgement received
     */

    UCHAR tx_window;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     *
     *  TxWindow of the Peer would be that what we specified in
     *  our InFEC.
     */
    tx_window = l2cap_dt_entity[0].in_fec.tx_window;

    /*
     *  Disable S-Frame Transmission when the first I-frame
     *  with TxSeq 0 is received.
     */
    if (tx_seq == 0)
    {
        /* Disable S-frame transmission */
        l2cap_test_drop_config (0x01);
    }
#endif

#if 0
    /*
     *  FEC Test 02
     *  Flow Control Mode. Sequence Number higher than TxWindow,
     *  acknowledgement received in RR frame
     */

    UCHAR tx_window;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     *
     *  TxWindow of the Peer would be that what we specified in
     *  our InFEC.
     */
    tx_window = l2cap_dt_entity[0].in_fec.tx_window;

    /*
     *  TxSeq = 0 -> Handle the I-frame => Return Success.
     *  TxSeq = 1 onwards -> Drop I-frame => Return Failure.
     */
    if (tx_seq == 0)
    {
        /* Disable S-frame transmission */
        l2cap_test_drop_config (0x01);

        /* And, handle the I-frame, so that ExpectedTxSeq moves up by 1 */
        return API_SUCCESS;
    }

    if (tx_seq < (tx_window - 1))
    {
        /* Drop these I-frames => Return Failure */
        return API_FAILURE;
    }

    if (tx_seq == (tx_window - 1))
    {
        /* Now, enable S-frame transmission */
        l2cap_test_drop_config (0x00);

        /* But drop the I-frame anyway */
        return API_FAILURE;
    }

    /* Otherwise, Frames are accepted/handled */

#endif

#if 0
    /*
     *  FEC Test 03
     *  Flow Control Mode. Sequence Number higher than TxWindow,
     *  acknowledgement received in an I frame
     */

    UCHAR tx_window;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     *
     *  TxWindow of the Peer would be that what we specified in
     *  our InFEC.
     */
    tx_window = l2cap_dt_entity[0].in_fec.tx_window;

    /*
     *  TxSeq = 0 -> Handle the I-frame => Return Success.
     *  TxSeq = 1 onwards -> Drop I-frame => Return Failure.
     */
    if (tx_seq == 0)
    {
        /* Disable S-frame transmission */
        l2cap_test_drop_config (0x01);

        /* And, handle the I-frame, so that ExpectedTxSeq moves up by 1 */
        return API_SUCCESS;
    }
    else
    {
        /* Drop these I-frames => Return Failure */
        return API_FAILURE;
    }

    /* Otherwise, Frames are accepted/handled */

#endif

#if 0
    /*
     *  FEC Test 05
     *  The purpose of this testcase is to verify that when retransmission
     *  flag is set to R=1 in a received RR frame the DUT does not retransmit
     *  packets
     */

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     */

    /*
     *  TxSeq = 0 -> Handle the I-frame => Return Success.
     *  And also, disable Rx Flow => R-bit will be set
     */
    if (tx_seq == 0)
    {
        /* Disable Rx Flow */
        l2ca_rx_flow_req (l2cap_dt_entity[0].lcid, L2CAP_FEC_RX_FLOW_OFF);

        /*
         *  And, handle the I-frame, so that ExpectedTxSeq moves up by 1
         *  But since Rx Flow is now Off, updated ExpectedTxSeq will not
         *  figure in the transmitted S-frames
         */
        return API_SUCCESS;
    }
#endif

#if 0
    /*
     *  FEC Test 06
     *  The purpose of this testcase is to verify that retransmission
     *  occurs after Retransmission timeout when flag is set to R=0 in a
     *  received RR frame
     */

    static UCHAR case_6_i_frame_count = 0;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     */

    /*
     *  TxSeq = 0 -> Drop the I-frame => Return Failure.
     *  As a result, ExpectedTxSeq will not be updated, and Monitor
     *  Timer will cause transmission of S-frame with ReqSeq = 0.
     *
     *  When peer retransmit the I-frame, again with TxSeq = 0, it
     *  will be accepeted/handled => Return Success => ExpectedTxSeq
     *  gets updated now => new S-frame will carry ReqSeq as 1.
     */
    if (tx_seq == 0)
    {
        case_6_i_frame_count ++;
        if (case_6_i_frame_count == 1)
        {
            /* First Time => Drop I-frame */
            return API_FAILURE;
        }

        /* Not first time => Accept/Handle */
        return API_SUCCESS;
    }
#endif

#if 0
    /*
     *  FEC Test 07
     *  The purpose of this testcase is to verify that retransmission
     *  occurs after Retransmission timeout when flag is set to R=0 in a
     *  received I frame
     */

    static UCHAR case_7_i_frame_count = 0;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     */

    /*
     *  First TxSeq = 0 -> Drop the I-frame => Return Failure.
     *  And also, S-frame Transmission is switched off.
     *
     *  As a result, ExpectedTxSeq will not be updated, and no
     *  acknowledgement will go to the peer in S-frames
     *
     *  Rather, user should write an SDU which will cause an
     *  I-frame Transmission with ReqSeq = 0, and R = 0.
     *
     *  When peer retransmit the I-frame, again with TxSeq = 0, it
     *  will be accepeted/handled => Return Success => ExpectedTxSeq
     *  gets updated now => new S-frame will carry ReqSeq as 1.
     *  Also, S-frame transmission to be enabled, so that acknowledgements
     *  are sent.
     */
    if (tx_seq == 0)
    {
        case_7_i_frame_count ++;

        /* First I-frame with TxSeq = 0 */
        if (case_7_i_frame_count == 1)
        {
            /* Disable S-frame Transmission */
            l2cap_test_drop_config (0x01);

            /* First Time => Drop I-frame */
            return API_FAILURE;
        }

        /* Retransmitted I-frame with TxSeq = 0 */
        if (case_7_i_frame_count == 2)
        {
            /* Enable S-frame Transmission */
            l2cap_test_drop_config (0x00);

            /* Accept/Handle */
            return API_SUCCESS;
        }

        /* Not first time => Accept/Handle */
        return API_SUCCESS;
    }
#endif

#if 0
    /*
     *  FEC Test 08
     *  The purpose of this test case is to verify that the initiator
     *  retransmits I frame (Segmented) with sequence number that was
     *  indicated by received REJ frame
     */

    UCHAR tx_window;
    static UCHAR case_8_i_frame_count = 0;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     *
     *  TxWindow of the Peer would be that what we specified in
     *  our InFEC.
     */
    tx_window = l2cap_dt_entity[0].in_fec.tx_window;

    /*
     *  TxSeq = 0 -> Drop the I-frame => Return Failure.
     */
    if (tx_seq == 1)
    {
        case_8_i_frame_count ++;
        if (case_8_i_frame_count == 1)
        {
            /* First Time => Drop I-frame */
            return API_FAILURE;
        }

        /* Not first time => Accept/Handle */
        return API_SUCCESS;
    }
#endif

#if 0
    /*
     *  FEC Test 09
     *  The purpose of this test case is to verify that the initiator
     *  retransmits I frame (Unsegmented) with sequence number that was
     *  indicated by received REJ frame
     */

    UCHAR tx_window;
    static UCHAR case_9_i_frame_count = 0;

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     *
     *  TxWindow of the Peer would be that what we specified in
     *  our InFEC.
     */
    tx_window = l2cap_dt_entity[0].in_fec.tx_window;

    /*
     *  TxSeq = 0 -> Drop the I-frame => Return Failure.
     */
    if (tx_seq == 1)
    {
        case_9_i_frame_count ++;
        if (case_9_i_frame_count == 1)
        {
            /* First Time => Drop I-frame */
            return API_FAILURE;
        }

        /* Not first time => Accept/Handle */
        return API_SUCCESS;
    }
#endif

#if 0
    /*
     *  FEC Test 18
     *  The purpose of this test case is to verify that the initiator
     *  retransmits I frames the number of times configured as Max Transmit
     */

    /*
     *  Assumption:
     *  L2CAP DT Entity is the one at 0th location.
     */

    /*
     *  Disable S-Frame Transmission when the first I-frame
     *  with TxSeq 0 is received.
     *
     *  Also, drop all I-frames with TxSeq = 0
     */
    if (tx_seq == 0)
    {
        /* Disable S-frame transmission */
        l2cap_test_drop_config (0x01);

        return API_FAILURE;
    }
#endif

    return API_SUCCESS;
}

#endif /* if 0 */


void main_l2cap_operations (void)
{
    int choice, menu_choice;
    static UINT8 appl_l2cap_initialized = 0x00U;

#ifdef BT_UCD
    L2CAP_UCD_STRUCT appl_ucd;
#endif /* BT_UCD */

#if 1
    if (0x00U == appl_l2cap_initialized)
    {
#if 0
        /* Enable this code for true randomness of rand() ! */
        cur_time = time(NULL);
        srand(cur_time);
        printf("Initializing Random: %d\n", cur_time);
#endif /* 0 */

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_init(&l2cap_dt_mutex, NULL);
        (BT_IGNORE_RETURN_VALUE) BT_thread_cond_init(&l2cap_dt_cond, NULL);

#ifdef BT_UCD
        appl_ucd.l2ca_ucd_data_cb = appl_l2cap_ucd_data_cb;
        (BT_IGNORE_RETURN_VALUE) l2cap_register_ucd_callback(&appl_ucd);
#endif /* BT_UCD */

        appl_l2cap_initialized = 0x01U;
    }
#endif /* 0 */

    /* Main Menu */
    BT_LOOP_FOREVER()
    {
        printf("%s", main_l2cap_options);
        scanf("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            printf("\nReturning from L2CAP application ... \n");
            break; /* return; */

        case 1:
            break;

        case 2:
            /* Get Remote Address */
            l2cap_dt_get_remote_address();
            break;

        case 3:
            /* Register with L2CAP */
            l2cap_dt_register();
            break;

        case 4:
            /* Create L2CAP Channel */
            l2cap_dt_create_channels();
            break;

        case 5:
            /* Accept L2CAP Channel */
            l2cap_dt_accept_channels();
            break;

        case 6:
            /* Reconfigure L2CAP Channel */
            l2cap_dt_config_channels();
            break;

        case 7:
            /* Destroy L2CAP Channel */
            l2cap_dt_destroy_channels();
            break;

        case 8:
            /* Show L2CAP Channels */
            l2cap_dt_show_channels();
            break;

        case 10:
            /* Send L2CAP Data */
            l2cap_dt_send_data();
            break;

        case 11:
            /* Send L2CAP Data */
            l2cap_dt_send_data_random();
            break;

        case 12:
#ifdef BT_UCD
            /* Send L2CAP Data (Connectionless Unicast) */
            l2cap_dt_send_data_ucd();
#else
             printf("Define BT_UCD\n");
#endif /* BT_UCD */
            break;

        case 15:
#ifdef L2CAP_TEST_PACKET_DROP
            /* Cancel Packet Drops */
            l2cap_dt_drop_flag = 0x00U;
#endif /*L2CAP_TEST_PACKET_DROP*/
            break;

        case 16:
#ifdef L2CAP_TEST_PACKET_DROP
            /* Config Packet Drop for all S-frames */
            l2cap_dt_drop_flag = 0x01U;
#endif /*L2CAP_TEST_PACKET_DROP*/
            break;

        case 17:
#ifdef L2CAP_TEST_PACKET_DROP
            /* Config Random Packet Drop of I & S-frames */
            l2cap_dt_drop_percentage = 5U;
            l2cap_dt_drop_flag = 0x02U;
#endif /*L2CAP_TEST_PACKET_DROP*/

            break;

        case 20:
            /* Disable Rx Flow */
            /* NOTE: Flow is Set Off ONLY for FIRST L2CAP channel - Index 0 */
            (BT_IGNORE_RETURN_VALUE) l2ca_rx_flow_req (l2cap_dt_entity[0U].lcid, L2CAP_FEC_RX_FLOW_OFF);
            break;

        case 21:
            /* Enable Rx Flow */
            /* NOTE: Flow is Set On ONLY for FIRST L2CAP channel - Index 0 */
            (BT_IGNORE_RETURN_VALUE) l2ca_rx_flow_req (l2cap_dt_entity[0U].lcid, L2CAP_FEC_RX_FLOW_ON);
            break;

        case 22:
            /* Set Basic Mode Config Options */
            l2cap_dt_config_index = 0U;
            break;

        case 23:
            /* Set Retransmission Mode Config Options */
            l2cap_dt_config_index = 1U;
            break;

        case 24:
            /* Set Flow Control Mode Config Options */
            l2cap_dt_config_index = 2U;
            break;

        case 25:
            /* Set Enhanced Retransmission Mode Config Options */
            l2cap_dt_config_index = 3U;
            break;

        case 26:
            /* Set Streaming Mode Config Options */
            l2cap_dt_config_index = 4U;
            break;

        case 30:
#ifdef L2CAP_EXTENDED_FLOW_SPEC
            /* Set 'Best Effort' Extended Flow Specification Options */
            l2cap_dt_ext_flow_spec_index = 0U;
#else
            printf("Define L2CAP_EXTENDED_FLOW_SPEC\n");
#endif /* L2CAP_EXTENDED_FLOW_SPEC */
            break;

        case 31:
#ifdef L2CAP_EXTENDED_FLOW_SPEC
            /* Set 'Guaranteed' Extended Flow Specification Options */
            l2cap_dt_ext_flow_spec_index = 1U;
#else
            printf("Define L2CAP_EXTENDED_FLOW_SPEC\n");
#endif /* L2CAP_EXTENDED_FLOW_SPEC */
            break;

        case 40:
            appl_l2cap_fcs = L2CAP_FCS_OPTION_DEFAULT;
            break;

        case 41:
            appl_l2cap_fcs = L2CAP_FCS_OPTION_NO_FCS;
            break;

        case 50:
            appl_l2cap_basic_only_mode = 0x01U;
            break;

        case 51:
            appl_l2cap_basic_only_mode = 0x00U;
            break;

        case 52:	 /* Fall Through */
        case 53:
            break;

        case 54:
            appl_l2cap_sm_only_mode = 0x01U;
            break;

        case 55:
            appl_l2cap_sm_only_mode = 0x00U;
            break;

        case 100:
            /* Send Echo Request */
            {
                UCHAR echo_data[] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U};

                (BT_IGNORE_RETURN_VALUE) l2ca_ping_req
                (
                    l2cap_dt_remote_bd_addr,
                    echo_data,
                    sizeof(echo_data)
                );
            }
            break;

        case 101:
            /* Send Info Request */
            {
                UINT16 info_type;

                printf("Select Info Type:\n");
                printf("\tCONNECTIONLESS_MTU : 1\n");
                printf("\tEXTENDED_FEATURES  : 2\n");
                printf("\tFIXED_CHANNELS     : 3\n");

                scanf("%d", &choice);
                info_type = (UINT16)choice;

                (BT_IGNORE_RETURN_VALUE) l2ca_getinfo_req
                (
                    l2cap_dt_remote_bd_addr,
                    info_type
                );
            }
            break;

        default:
            printf("Invalid Option : %d. Try again.\n", choice);
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



API_RESULT l2cap_dt_connect_ind
           (
               /* IN */  DEVICE_HANDLE *    handle,
               /* IN */  UINT16             lcid,
               /* IN */  UINT16             psm
           )
{
    UCHAR index;
    UINT16 result;
    API_RESULT retval;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    printf("\n");
    printf("<- L2CA_ConnectIND. LCID = 0x%04X\n", lcid);

    /* Init */
    retval = API_SUCCESS;

    retval = device_queue_get_remote_addr_only(handle, bd_addr);

    if (API_SUCCESS != retval)
    {
        printf("Failed to find Device Handle\n");
        /* return API_SUCCESS; */
    }
    else
    {
        printf("   BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);

        if (0 != BT_mem_cmp(bd_addr, l2cap_dt_remote_bd_addr, BT_BD_ADDR_SIZE))
        {
            printf("*** L2CA_ConnectIND from Unknown BD_ADDR\n");
            /* return API_SUCCESS; */
        }
        else
        {
            /* Search for L2CAP DT Entity */
            for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
            {
                if ((l2cap_dt_entity[index].valid == 0x1U) &&
                    (l2cap_dt_entity[index].state == L2CAP_DT_W4_CONNECT_IND))
                {
                    break;
                }
            }

            if (index == L2CAP_DT_MAX_ENTITY)
            {
                printf("*** FAILED to Find L2CAP DT Entity in W4_CONNECT_IND\n");
                result = L2CAP_CONNECTION_REFUSED_NO_RESOURCE;
            }
            else
            {
                result = L2CAP_CONNECTION_SUCCESSFUL;

                l2cap_dt_entity[index].lcid = lcid;
                l2cap_dt_entity[index].state = L2CAP_DT_W4_CONFIG;
            }

            /**
             * Set configuration preference based on the current mode configuration.
             */
            if (4U == l2cap_dt_config_index)
            {
                retval = l2ca_connect_rsp_ex(lcid, result, 0x0000U, L2CAP_CONFIG_PREF_SM_OPTIONAL);
            }
            else if ((0U == l2cap_dt_config_index) && (0x01U == appl_l2cap_basic_only_mode))
            {
                retval = l2ca_connect_rsp_ex(lcid, result, 0x0000U, L2CAP_CONFIG_PREF_BASIC);
            }
            else
            {
                retval = l2ca_connect_rsp_ex(lcid, result, 0x0000U, L2CAP_CONFIG_PREF_ERTM_OPTIONAL);
            }

            if (retval != API_SUCCESS)
            {
                printf("*** L2CA_ConnectRSP: 0x%04X\n", retval);
                /* return retval; */
            }
        }

        printf("-> L2CA_ConnectRSP\n");
    }

    BT_IGNORE_UNUSED_PARAM(psm);

    return retval;
}


API_RESULT l2cap_dt_connect_cnf
           (
               /* IN */  DEVICE_HANDLE *    handle,
               /* IN */  UINT16             lcid,
               /* IN */  UINT16             result,
               /* IN */  UINT16             status
           )
{
    UCHAR index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Ignore Connection Pending from Peer */
    if (result == 0x0001U)
    {
        printf("<- L2CA_ConnectPND. LCID = 0x%04X, Status = 0x%04X\n",
        lcid, status);

        /* return API_SUCCESS; */
    }
    else
    {
        if ((result != 0x0000U) || (status != 0x0000U))
        {
            printf("*** L2CA_ConnectCNF: Result - 0x%04X, "\
            "Status - 0x%04X\n", result, status);

            /* return API_SUCCESS; */
        }
        else
        {
            printf("<- L2CA_ConnectCNF. LCID = 0x%04X\n", lcid);

            /* Search for L2CAP DT Entity */
            for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
            {
                if ((l2cap_dt_entity[index].valid == 0x1U) &&
                    (l2cap_dt_entity[index].state == L2CAP_DT_W4_CONNECT_CNF))
                {
                    break;
                }
            }

            if (index == L2CAP_DT_MAX_ENTITY)
            {
                printf("*** FAILED to Find L2CAP DT Entity in W4_CONNECT_CNF\n");
                /* return API_SUCCESS; */
            }
            else
            {
                /* Save LCID */
                l2cap_dt_entity[index].lcid = lcid;
                l2cap_dt_entity[index].state = L2CAP_DT_W4_CONFIG;

                /* Configure L2CAP Channel */
                l2cap_dt_config_channels_index(index);
            }
        }
    }

    BT_IGNORE_UNUSED_PARAM(handle);

    return retval;
}


API_RESULT l2cap_dt_config_ind
           (
              /* IN */  UINT16                 lcid,
              /* IN */  L2CAP_CONFIG_OPTION *  config_option
           )
{
    UCHAR index;
    API_RESULT retval;
    L2CAP_CONFIG_OPTION config;

    /* Init */
    retval = API_SUCCESS;

    printf("<- L2CA_ConfigIND. LCID = 0x%04X\n", lcid);

    /* Search for L2CAP DT Entity */
    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if ((l2cap_dt_entity[index].valid == 0x1U) &&
            (l2cap_dt_entity[index].lcid == lcid))
        {
            break;
        }
    }

    if (index == L2CAP_DT_MAX_ENTITY)
    {
        printf("*** FAILED to Find L2CAP DT Entity for LCID\n");
        /* return API_SUCCESS; */
    }
    else
    {
        if (l2cap_dt_entity[index].state == L2CAP_DT_OPEN)
        {
            printf("Reconfiguring L2CAP Channel ...\n");
            l2cap_dt_entity[index].state = L2CAP_DT_W4_CONFIG;

            /* Get InMTU & InFEC Configuration Parameters */
            l2cap_dt_entity[index].in_mtu =
            l2cap_dt_config_option[l2cap_dt_config_index].in_mtu;
            l2cap_dt_entity[index].out_flush_to =
            l2cap_dt_config_option[l2cap_dt_config_index].out_flush_to;
            l2cap_dt_entity[index].out_flow =
            l2cap_dt_config_option[l2cap_dt_config_index].out_flow;
            l2cap_dt_entity[index].in_fec =
            l2cap_dt_config_option[l2cap_dt_config_index].in_fec;
            l2cap_dt_entity[index].out_fcs =
            l2cap_dt_config_option[l2cap_dt_config_index].out_fcs;
        }

        if (l2cap_dt_entity[index].state != L2CAP_DT_W4_CONFIG)
        {
            printf("*** FAILED to Find L2CAP DT Entity in W4_CONFIG\n");
            /* return API_SUCCESS; */
        }
        else
        {
            config.mtu = ((L2CAP_MIN_MTU < config_option->mtu)? config_option->mtu: L2CAP_MIN_MTU);
            config.flush_to = config_option->flush_to;
            config.flow_spec = config_option->flow_spec;
            config.fec = config_option->fec;
            config.fcs = config_option->fcs;

            retval = l2ca_config_rsp (lcid, &config);
            if (retval != API_SUCCESS)
            {
                printf("*** L2CA_ConfigRSP: 0x%04X\n", retval);
                /* return retval; */
            }
            else
            {
                printf("-> L2CA_ConfigRSP. LCID = 0x%04X\n", lcid);

                /* Save Remote Options */
                l2cap_dt_entity[index].out_mtu = config.mtu;
                if (config.fec != NULL)
                {
                    l2cap_dt_entity[index].out_fec = *(config.fec);
                }
                l2cap_dt_entity[index].in_fcs = config.fcs;

                l2cap_dt_entity[index].remote_config_done = 0x01U;
                if (l2cap_dt_entity[index].local_config_done == 0x01U)
                {
                    printf("L2CAP Channel Created & Configured\n");

                    l2cap_dt_entity[index].local_config_done = 0x0U;
                    l2cap_dt_entity[index].remote_config_done = 0x0U;
                    l2cap_dt_entity[index].config_req_sent = 0x0U;
                    l2cap_dt_entity[index].state = L2CAP_DT_OPEN;

                    l2cap_dt_show_channels();

                    /* Signal */
                    l2cap_dt_signal();
                }
                else if (l2cap_dt_entity[index].local_config_done == 0x00U)
                {
                    l2cap_dt_config_channels_index (index);
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
        }
    }

    return retval;
}


API_RESULT l2cap_dt_config_cnf
           (
              /* IN */  UINT16                 lcid,
              /* IN */  UINT16                 result,
              /* IN */  L2CAP_CONFIG_OPTION *  config_option
           )
{
    UCHAR index;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    if (result != 0x0000U)
    {
        printf("*** L2CA_ConfigCNF: Result - 0x%04X\n", result);
        /* return API_SUCCESS; */
    }
    else
    {

        printf("<- L2CA_ConfigCNF. LCID = 0x%04X\n", lcid);

        /* Search for L2CAP DT Entity */
        for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index++)
        {
            if ((l2cap_dt_entity[index].valid == 0x1U) &&
                (l2cap_dt_entity[index].lcid == lcid) &&
                (l2cap_dt_entity[index].state == L2CAP_DT_W4_CONFIG))
            {
                break;
            }
        }

        if (index == L2CAP_DT_MAX_ENTITY)
        {
            printf("*** FAILED to Find L2CAP DT Entity in W4_CONFIG\n");
            /* return API_SUCCESS; */
        }
        else
        {
            l2cap_dt_entity[index].local_config_done = 0x01U;
            if (l2cap_dt_entity[index].remote_config_done == 0x01U)
            {
                printf("L2CAP Channel Created & Configured\n");

                l2cap_dt_entity[index].local_config_done = 0x0U;
                l2cap_dt_entity[index].remote_config_done = 0x0U;
                l2cap_dt_entity[index].config_req_sent = 0x0U;
                l2cap_dt_entity[index].state = L2CAP_DT_OPEN;

                l2cap_dt_show_channels();

                /* Signal */
                l2cap_dt_signal();
            }
        }
    }

    BT_IGNORE_UNUSED_PARAM(config_option);

    return retval;
}


API_RESULT l2cap_dt_open_ind
           (
              /* IN */  UINT16    lcid
           )
{
    UCHAR index;

    /* Search for L2CAP DT Entity */
    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if ((l2cap_dt_entity[index].valid == 0x1U) &&
            (l2cap_dt_entity[index].lcid == lcid) &&
            (l2cap_dt_entity[index].state == L2CAP_DT_W4_CONFIG))
        {
            break;
        }
    }

    if (index == L2CAP_DT_MAX_ENTITY)
    {
        printf("*** FAILED to Find L2CAP DT Entity in W4_CONFIG\n");
        /* return API_SUCCESS; */
    }
    else
    {
        if ((l2cap_dt_entity[index].local_config_done == 0x01U) &&
            (l2cap_dt_entity[index].remote_config_done == 0x01U))
        {
            printf("L2CAP Channel Created & Configured\n");

            l2cap_dt_entity[index].local_config_done = 0x0U;
            l2cap_dt_entity[index].remote_config_done = 0x0U;
            l2cap_dt_entity[index].config_req_sent = 0x0U;
            l2cap_dt_entity[index].state = L2CAP_DT_OPEN;

            l2cap_dt_show_channels();

            /* Signal */
            l2cap_dt_signal();
        }
        else
        {
            printf("*** Unexpected L2CA_OpenIND !!!\n");
        }
    }

    return API_SUCCESS;
}


API_RESULT l2cap_dt_disconnect_ind
           (
              /* IN */  UINT16  lcid
           )
{
    UCHAR index;

    printf("\n");
    printf("<- L2CA_DisconnectIND. LCID = 0x%04X\n", lcid);

    /* Search for L2CAP DT Entity */
    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if ((l2cap_dt_entity[index].valid == 0x1U) &&
            (l2cap_dt_entity[index].lcid == lcid))
        {
            break;
        }
    }

    if (index == L2CAP_DT_MAX_ENTITY)
    {
        printf("*** FAILED to Find L2CAP DT Entity for L2CA_DisconnectIND\n");
        /* return API_SUCCESS; */
    }
    else
    {
        /* Signal if waiting */
        if (BT_TRUE == l2cap_dt_entity[index].waiting_for_signal)
        {
            l2cap_dt_signal_tx_win_free();
        }

        /* Free L2CAP DT Entity */
        l2cap_dt_entity_free(index);
    }

    return API_SUCCESS;
}


API_RESULT l2cap_dt_disconnect_cnf
           (
              /* IN */  UINT16  lcid,
              /* IN */  UINT16  result
           )
{
    UCHAR index;

    printf("<- L2CA_DisconnectCNF. LCID = 0x%04X\n", lcid);

    /* Search for L2CAP DT Entity */
    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if ((l2cap_dt_entity[index].valid == 0x1U) &&
            (l2cap_dt_entity[index].lcid == lcid) &&
            (l2cap_dt_entity[index].state == L2CAP_DT_W4_DISCONNECT_CNF))
        {
            break;
        }
    }

    if (index == L2CAP_DT_MAX_ENTITY)
    {
        printf("*** FAILED to Find L2CAP DT Entity in W4_DISCONNECT_CNF\n");
        /* return API_SUCCESS; */
    }
    else
    {
        /* Free L2CAP DT Entity */
        l2cap_dt_entity_free(index);

        /* Signal if waiting */
        if (BT_TRUE == l2cap_dt_entity[index].waiting_for_signal)
        {
            l2cap_dt_signal_tx_win_free();
        }
    }

    BT_IGNORE_UNUSED_PARAM(result);

    return API_SUCCESS;
}

#ifdef L2CAP_CHANNEL_DATAWRITE_CALLBACK
API_RESULT l2cap_dt_data_write_cnf
           (
               UINT16 lcid,
               UCHAR *data,
               UINT16 datalen
           )
{
    printf("\n");
    printf("<- L2CA_DataWriteCNF. LCID = 0x%04X, Data Length = %d\n",
        lcid, datalen);

    if (BT_STATIC_DATA_SIZE < datalen)
    {
        BT_assert(NULL != data);
        BT_free_mem(data);
    }

    return API_SUCCESS;
}
#endif /* L2CAP_CHANNEL_DATAWRITE_CALLBACK */

API_RESULT l2cap_dt_data_read
           (
              /* IN */  UINT16   lcid,
              /* IN */  UCHAR *  data,
              /* IN */  UINT16   datalen
           )
{
    int i;

    printf("\n");
    printf("<- L2CA_DataReadIND. LCID = 0x%04X, Data Length = %d\n",
    lcid, datalen);

    for (i = 0U; i < datalen; i ++)
    {
        if (data[i] != (UCHAR)(L2CAP_HDR_LEN + i))
        {
            printf("\n");
            printf("**** Data does not match\n");

            printf("Data = ");
            for (i = 0U; i < datalen; i ++)
            {
                printf("%02X ", data[i]);
            }
            printf("\n");
        }
    }

    return API_SUCCESS;
}


API_RESULT l2cap_dt_qos_violation_ind
           (
              /* IN */  UCHAR *  bd_addr
           )
{
    BT_IGNORE_UNUSED_PARAM(bd_addr);

    return API_SUCCESS;
}


API_RESULT l2cap_dt_data_inconsistent
           (
              /* IN */  UINT16   lcid
           )
{
    BT_IGNORE_UNUSED_PARAM(lcid);

    return API_SUCCESS;
}


API_RESULT l2cap_dt_ping_cnf
           (
               /* IN */ DEVICE_HANDLE *  handle,
               /* IN */ UINT16           result,
               /* IN */ UCHAR         *  echo_data,
               /* IN */ UINT16           length
           )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(result);
    BT_IGNORE_UNUSED_PARAM(echo_data);
    BT_IGNORE_UNUSED_PARAM(length);

    return API_SUCCESS;
}


API_RESULT l2cap_dt_getinfo_cnf
           (
              /* IN */ DEVICE_HANDLE  * handle,
              /* IN */ UINT16           result,
              /* IN */ UCHAR         *  info_data,
              /* IN */ UINT16           info_datalen
           )
{
    int i;

    printf("\n");
    printf("<- L2CA_GetInfoCNF\n");
    printf("   Device Handle: 0x%02X\n", (*handle));
    printf("   Result  = 0x%04X\n",result);
    printf("   Info Data = ");
    for(i = 0U; i < info_datalen; i++)
    {
        printf("%x ", info_data[i]);
    }
    printf("\n");

    return API_SUCCESS;
}


#ifdef BT_ENH_L2CAP
API_RESULT l2cap_dt_tx_flow_ind
           (
              /* IN */  UINT16   lcid,
              /* IN */  UCHAR    flow
           )
{
    if (flow == L2CAP_FEC_TX_FLOW_ON)
    {
        printf("<- L2CA_TxFlowIND. LCID = 0x%04X, Flow = On<0x%02x>\n",
        lcid, flow);

        l2cap_dt_signal_tx_win_free();
    }
    else if (flow == L2CAP_FEC_TX_FLOW_OFF)
    {
        printf("<- L2CA_TxFlowIND. LCID = 0x%04X, Flow = Off<0x%02x>\n",
        lcid, flow);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return API_SUCCESS;
}

API_RESULT l2cap_dt_get_fec_params
           (
               /* IN */ UINT16 lcid,
               /* IN */ L2CAP_FEC_OPTION * fec_ptr
           )
{
    /* Check the Suggested Mode */
    if (L2CAP_MODE_ERTM == fec_ptr->mode)
    {
        *fec_ptr = l2cap_dt_config_option[3U].in_fec;
    }
    else if (L2CAP_MODE_SM == fec_ptr->mode)
    {
        *fec_ptr = l2cap_dt_config_option[4U].in_fec;

#if 0
        fec_ptr->tx_window = 0x00;
        fec_ptr->max_transmit = 0x00;
        fec_ptr->retx_timeout = 0;
        fec_ptr->monitor_timeout = 0;
        fec_ptr->mps = 100;
#endif /* 0 */
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    BT_IGNORE_UNUSED_PARAM(lcid);

    return API_SUCCESS;
}
#endif /* BT_ENH_L2CAP */


void l2cap_dt_entity_init (void)
{
    UCHAR index;

    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        l2cap_dt_entity[index].valid = 0x0U;
    }
}


void l2cap_dt_entity_alloc (UCHAR *entity_index)
{
    UCHAR index;

    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if (l2cap_dt_entity[index].valid == 0x0U)
        {
            l2cap_dt_entity[index].valid = 0x1U;

            l2cap_dt_entity[index].config_req_sent = 0x0U;
            l2cap_dt_entity[index].local_config_done = 0x0U;
            l2cap_dt_entity[index].remote_config_done = 0x0U;

            l2cap_dt_entity[index].waiting_for_signal = BT_FALSE;

            l2cap_dt_entity[index].state = 0x0U;
            l2cap_dt_entity[index].lcid = 0x0000U;

            l2cap_dt_entity[index].in_mtu = L2CAP_MTU_DEFAULT;
            l2cap_dt_entity[index].out_mtu = L2CAP_MTU_DEFAULT;

            l2cap_dt_entity[index].in_flush_to = L2CAP_FLUSH_TO_DEFAULT;
            l2cap_dt_entity[index].out_flush_to = L2CAP_FLUSH_TO_DEFAULT;

            l2cap_init_flow_spec_default (&l2cap_dt_entity[index].in_flow);
            l2cap_init_flow_spec_default (&l2cap_dt_entity[index].out_flow);

            l2cap_init_fec_option_default (&l2cap_dt_entity[index].in_fec);
            l2cap_init_fec_option_default (&l2cap_dt_entity[index].out_fec);

            *entity_index = index;
            break; /* return; */
        }
    }

    if (L2CAP_DT_MAX_ENTITY == index)
    {
        *entity_index = 0xFFU;
    }

    return;
}


void l2cap_dt_entity_free (UCHAR index)
{
    l2cap_dt_entity[index].valid = 0x0U;
}

void l2cap_dt_register(void)
{
#ifdef L2CAP_ASSYMETRIC_PSM
    int choice;
#endif /* L2CAP_ASSYMETRIC_PSM */

    L2CAP_PSM l2cap_psm;
    L2CAP_COMMON_CB common_cb;

#ifdef L2CAP_ASSYMETRIC_PSM
    printf("Enter Local PSM = "); fflush(stdout);
    scanf("%x", &choice);
    l2cap_dt_local_psm = (UINT16)choice;

    printf("Enter Remote PSM = "); fflush(stdout);
    scanf("%x", &choice);
    l2cap_dt_remote_psm = (UINT16)choice;
#endif /* L2CAP_ASSYMETRIC_PSM */

    /* Initialize PSM */
    l2cap_init_psm(l2cap_dt_local_psm, &l2cap_psm);

    /* Register PSM */
    l2cap_psm.l2ca_connect_ind_cb = l2cap_dt_connect_ind;
    l2cap_psm.l2ca_connect_cnf_cb = l2cap_dt_connect_cnf;
    l2cap_psm.l2ca_config_ind_cb = l2cap_dt_config_ind;
    l2cap_psm.l2ca_config_cnf_cb = l2cap_dt_config_cnf;
    l2cap_psm.l2ca_disconnect_ind_cb = l2cap_dt_disconnect_ind;
    l2cap_psm.l2ca_disconnect_cnf_cb = l2cap_dt_disconnect_cnf;
    l2cap_psm.l2ca_data_read_cb = l2cap_dt_data_read;

#ifdef L2CAP_CHANNEL_DATAWRITE_CALLBACK
    l2cap_psm.l2ca_data_write_cb = l2cap_dt_data_write_cnf;
#endif /* L2CAP_CHANNEL_DATAWRITE_CALLBACK */

#ifdef BT_ENH_L2CAP
    l2cap_psm.l2ca_tx_flow_ind_cb = l2cap_dt_tx_flow_ind;

    /** Callback for l2ca_get_fec_params (CID, FEC) */
    l2cap_psm.l2ca_get_fec_params_cb = l2cap_dt_get_fec_params;
#endif /* BT_ENH_L2CAP */

    /* Update Feature Mask */
    l2cap_psm.feature_mask |= L2CAP_FEATURE_ERTM |
                              L2CAP_FEATURE_SM |
                              L2CAP_FEATURE_FCS;

    printf("Registering PSM 0x%04X with L2CAP ...\n", l2cap_psm.psm);
    (BT_IGNORE_RETURN_VALUE) l2cap_register_psm(&l2cap_psm);

    /* Register Common Callbacks */
    common_cb.l2ca_ping_cnf = l2cap_dt_ping_cnf;
    common_cb.l2ca_getinfo_cnf = l2cap_dt_getinfo_cnf;

    printf("Registering Common Callbacks to L2CAP ...\n");
    (BT_IGNORE_RETURN_VALUE) l2cap_register_common_cb(&common_cb);

    /* Initialize L2CAP DT Entities */
    l2cap_dt_entity_init();
    l2cap_dt_config_option_init();
    l2cap_dt_ext_flow_spec_option_init();

    return;
}

void l2cap_dt_get_remote_address(void)
{
    int i, choice;

    /* Get Remote Bluetooth Device Address */
    printf("Enter Remote BD Address = ");
    for (i = 0U; i < 6U; i++)
    {
        scanf("%x", &choice);
        l2cap_dt_remote_bd_addr[i] = (UCHAR)choice;
    }

    printf("Configured Remote BD Address = ");
    printf("%02X %02X %02X %02X %02X %02X\n",
    l2cap_dt_remote_bd_addr[0U], l2cap_dt_remote_bd_addr[1U],
    l2cap_dt_remote_bd_addr[2U], l2cap_dt_remote_bd_addr[3U],
    l2cap_dt_remote_bd_addr[4U], l2cap_dt_remote_bd_addr[5U]);


    return;
}

void l2cap_dt_create_channels (void)
{
    UCHAR index;
    DEVICE_HANDLE handle;
    BT_DEVICE_ADDR bd_addr;
    API_RESULT retval;
    UCHAR      config_preference;

    printf("\n");
    printf("Initiating L2CAP Channel Establishment ...\n");

    index = 0xFFU;

    /* Create L2CAP DT Entity */
    l2cap_dt_entity_alloc (&index);
    if (index == 0xFFU)
    {
        printf("*** FAILED to Alloc L2CAP DT Entity\n");
        /* return; */
    }
    else
    {
        bd_addr.type = DQ_LINK_NONE;
        BT_COPY_BD_ADDR(bd_addr.addr, l2cap_dt_remote_bd_addr);

        /* retval = device_queue_search_br_edr_remote_addr(&handle, &bd_addr); */
        retval = device_queue_search_any_remote_addr(&handle, &bd_addr);

        if (API_SUCCESS != retval)
        {
            printf("Failed to find remote device in Device Queue\n");
            /* return; */
        }
        else
        {
            /* Send L2CA_ConnectREQ */
            /**
             * Set configuration preference based on the current mode configuration.
             */
            if (4U == l2cap_dt_config_index)
            {
                if (0x01 == appl_l2cap_sm_only_mode)
                {
                    config_preference = L2CAP_CONFIG_PREF_SM_ONLY;
                }
                else
                {
                    config_preference = L2CAP_CONFIG_PREF_SM_OPTIONAL;
                }
                retval = l2ca_connect_req_ex
                         (
                             l2cap_dt_local_psm,
                             l2cap_dt_remote_psm,
                             &handle,
                             config_preference
                         );
            }
            else
            {
                if (0x01 == appl_l2cap_sm_only_mode)
                {
                    config_preference = L2CAP_CONFIG_PREF_ERTM_ONLY;
                }
                else
                {
                    config_preference = L2CAP_CONFIG_PREF_ERTM_OPTIONAL;
                }
                retval = l2ca_connect_req_ex
                         (
                             l2cap_dt_local_psm,
                             l2cap_dt_remote_psm,
                             &handle,
                             config_preference
                         );
            }

            if (retval != API_SUCCESS)
            {
                printf("*** L2CA_ConnectREQ: 0x%04X\n", retval);
                /* return; */
            }
            else
            {
                printf("-> L2CA_ConnectREQ\n");

                /* Get InMTU & InFEC Configuration Parameters */
                l2cap_dt_entity[index].in_mtu =
                l2cap_dt_config_option[l2cap_dt_config_index].in_mtu;
                l2cap_dt_entity[index].out_flush_to =
                l2cap_dt_config_option[l2cap_dt_config_index].out_flush_to;
                l2cap_dt_entity[index].out_flow =
                l2cap_dt_config_option[l2cap_dt_config_index].out_flow;
                l2cap_dt_entity[index].in_fec =
                l2cap_dt_config_option[l2cap_dt_config_index].in_fec;
                l2cap_dt_entity[index].out_fcs =
                l2cap_dt_config_option[l2cap_dt_config_index].out_fcs;

                l2cap_dt_entity[index].state = L2CAP_DT_W4_CONNECT_CNF;

                /* Wait for Completion */
                l2cap_dt_wait();
            }
        }
    }

    return;
}


void l2cap_dt_accept_channels (void)
{
    UCHAR index;

    printf("\n");
    printf("Waiting for L2CAP Channel Establishment ...\n");

    index = 0xFFU;

    /* Create L2CAP DT Entity */
    l2cap_dt_entity_alloc (&index);
    if (index == 0xFFU)
    {
        printf("*** FAILED to Alloc L2CAP DT Entity\n");
        /* return; */
    }
    else
    {
        /* Get InMTU & InFEC Configuration Parameters */
        l2cap_dt_entity[index].in_mtu =
            l2cap_dt_config_option[l2cap_dt_config_index].in_mtu;
        l2cap_dt_entity[index].out_flush_to =
            l2cap_dt_config_option[l2cap_dt_config_index].out_flush_to;
        l2cap_dt_entity[index].out_flow =
            l2cap_dt_config_option[l2cap_dt_config_index].out_flow;
        l2cap_dt_entity[index].in_fec =
            l2cap_dt_config_option[l2cap_dt_config_index].in_fec;
        l2cap_dt_entity[index].out_fcs =
            l2cap_dt_config_option[l2cap_dt_config_index].out_fcs;

        l2cap_dt_entity[index].state = L2CAP_DT_W4_CONNECT_IND;

        /* Wait for Completion */
        l2cap_dt_wait();
    }

    return;
}


void l2cap_dt_config_channels (void)
{
    UCHAR index;
    int choice;

    printf("\n");
    printf("Reconfiguring L2CAP Channel ...\n");

    /* Show L2CAP Channels */
    l2cap_dt_show_channels ();
    printf("Enter Index to Reconfigure = "); fflush(stdout);
    scanf("%d", &choice);
    index = (UCHAR)choice;

    /* Get InMTU & InFEC Configuration Parameters */
    l2cap_dt_entity[index].in_mtu =
    l2cap_dt_config_option[l2cap_dt_config_index].in_mtu;
    l2cap_dt_entity[index].out_flush_to =
    l2cap_dt_config_option[l2cap_dt_config_index].out_flush_to;
    l2cap_dt_entity[index].out_flow =
    l2cap_dt_config_option[l2cap_dt_config_index].out_flow;
    l2cap_dt_entity[index].in_fec =
    l2cap_dt_config_option[l2cap_dt_config_index].in_fec;
    l2cap_dt_entity[index].out_fcs =
    l2cap_dt_config_option[l2cap_dt_config_index].out_fcs;

    l2cap_dt_entity[index].state = L2CAP_DT_W4_CONFIG;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    BT_mem_copy
    (
        &l2cap_dt_entity[index].ext_flow_spec,
        &l2cap_dt_ext_flow_spec_option[l2cap_dt_ext_flow_spec_index],
        sizeof(l2cap_dt_ext_flow_spec_option[l2cap_dt_ext_flow_spec_index])
    );
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

    /* Initiate Reconfiguration */
    l2cap_dt_config_channels_index (index);

    /* Wait for Completion */
    l2cap_dt_wait();

    return;
}


void l2cap_dt_config_channels_index (UCHAR index)
{
    L2CAP_CONFIG_OPTION config;
    API_RESULT retval;

    printf("\n");
    printf("Configuring L2CAP Channel ...\n");

    if (l2cap_dt_entity[index].local_config_done == 0x0U)
    {
        if (l2cap_dt_entity[index].config_req_sent == 0x0U)
        {
            l2cap_init_config_option (&config);

            config.mtu = l2cap_dt_entity[index].in_mtu;
            config.flush_to = l2cap_dt_entity[index].out_flush_to;
            config.flow_spec = &l2cap_dt_entity[index].out_flow;
            config.fec = &l2cap_dt_entity[index].in_fec;
            config.fcs = l2cap_dt_entity[index].out_fcs;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
            config.ext_flow_spec = &l2cap_dt_entity[index].ext_flow_spec;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

            /* Start Config */
            retval = l2ca_config_req (l2cap_dt_entity[index].lcid, &config);

            if (retval != API_SUCCESS)
            {
                printf("*** L2CA_ConfigREQ: 0x%04X\n", retval);
                /* return; */
            }
            else
            {
                printf("-> L2CA_ConfigREQ. LCID = 0x%04X\n",
                    l2cap_dt_entity[index].lcid);

                l2cap_dt_entity[index].config_req_sent = 0x1U;
            }
        }
    }
}


void l2cap_dt_destroy_channels (void)
{
    UCHAR index;
    int choice;
    API_RESULT retval;

    printf("\n");
    printf("Disconnecting L2CAP Channel ...\n");

    /* Show L2CAP Channels */
    l2cap_dt_show_channels ();
    printf("Enter Index to Disconnect = "); fflush(stdout);
    scanf("%d", &choice);
    index = (UCHAR)choice;

    /* Start Disconnect */
    retval = l2ca_disconnect_req (l2cap_dt_entity[index].lcid);
    if (retval != API_SUCCESS)
    {
        printf("*** L2CA_DisconnectREQ: 0x%04X\n", retval);
        /* return; */
    }
    else
    {
        printf("-> L2CA_DisconnectREQ. LCID = 0x%04X\n",
        l2cap_dt_entity[index].lcid);

        l2cap_dt_entity[index].state = L2CAP_DT_W4_DISCONNECT_CNF;

        /* Wait for Completion */
        l2cap_dt_wait();
    }

    return;
}


void l2cap_dt_send_data (void)
{
    int choice;
    UCHAR index;
    UCHAR *sdu_data;
    UINT16 i, j, sdu_size, sdu_num, actual;
    API_RESULT retval;

    printf("\n");
    printf("Sending Data over L2CAP Channel ...\n");

    /* Show L2CAP Channels */
    l2cap_dt_show_channels ();
    printf("Enter Index of L2CAP Channel = "); fflush(stdout);
    scanf("%d", &choice);
    index = (UCHAR)choice;

    if (l2cap_dt_entity[index].state != L2CAP_DT_OPEN)
    {
        printf("*** LCID is NOT in OPEN State. Cannot Send Data\n");
        /* return; */
    }
    else
    {
        printf("Enter L2CAP SDU Size = "); fflush(stdout);
        scanf("%d", &choice);
        sdu_size = (UINT16) choice;

        printf("Enter Number of L2CAP SDU = "); fflush(stdout);
        scanf("%d", &choice);
        sdu_num = (UINT16) choice;

        if (sdu_size > l2cap_dt_entity[index].out_mtu)
        {
            printf("*** SDU Size is MORE than OutMTU. Cannot Send Data\n");
            /* return; */
        }
        else
        {
            for (j = 0U; j < sdu_num; j ++)
            {
                sdu_data = (UCHAR *) BT_alloc_mem (sdu_size + L2CAP_HDR_LEN);
                if (sdu_data == NULL)
                {
                    printf("*** FAILED to Allocate Memory for SDU Payload\n");
                    break; /* return; */
                }

                /* Form SDU Data Pattern */
                BT_mem_set (sdu_data, 0x0, L2CAP_HDR_LEN);
                for (i = L2CAP_HDR_LEN; i < (sdu_size + L2CAP_HDR_LEN); i ++)
                {
                    sdu_data[i] = (UCHAR) i;
                }

                /* MISRA C-2012 Rule 9.1 */
                actual = 0U;

                /* Send Data */
                retval = l2ca_data_write
                         (
                             l2cap_dt_entity[index].lcid,
                             (UINT16)(sdu_size + L2CAP_HDR_LEN),
                             sdu_data,
                             &actual
                         );

                if ((API_SUCCESS != L2CA_IS_DATA_WRITE_SUCCESS(retval)) ||
                    (BT_STATIC_DATA_SIZE >= (sdu_size + L2CAP_HDR_LEN)))
                {
                    /* Free the allocated SDU */
                    BT_free_mem(sdu_data);
                }

                printf("-> L2CA_DataWriteREQ. SDU Size: %d Bytes\n", sdu_size);

                if (actual != sdu_size)
                {
                    printf("*** Actual (%d) does NOT match Requested SDU Size (%d)\n",
                    actual, sdu_size);

                    break; /* return; */
                }

                if (retval == L2CAP_FEC_TX_WINDOW_FULL)
                {
                    printf("Waiting for TxWindow to become Free ...\n");
                    l2cap_dt_entity[index].waiting_for_signal = BT_TRUE;
                    l2cap_dt_wait_for_tx_win_free();
                    l2cap_dt_entity[index].waiting_for_signal = BT_FALSE;
                }
                else if (retval != API_SUCCESS)
                {
                    printf("*** L2CA_DataWriteREQ. Reason = 0x%04X\n", retval);
                    break; /* return; */
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
        }
    }
}


void l2cap_dt_send_data_random (void)
{
    int choice;
    UCHAR index;
    UCHAR *sdu_data;
    UINT16 i, j, sdu_size, sdu_num, actual;
    API_RESULT retval;
    UINT16 out_mtu;

    printf("\n");
    printf("Sending Data (Random) over L2CAP Channel ...\n");

    /* Show L2CAP Channels */
    l2cap_dt_show_channels ();
    printf("Enter Index of L2CAP Channel = "); fflush(stdout);
    scanf("%d", &choice);
    index = (UCHAR)choice;

    if (l2cap_dt_entity[index].state != L2CAP_DT_OPEN)
    {
        printf("*** LCID is NOT in OPEN State. Cannot Send Data\n");
        /* return; */
    }
    else
    {
        /* Get OutMTU */
        out_mtu = l2cap_dt_entity[index].out_mtu;

        printf("Enter Number of L2CAP SDUs = "); fflush(stdout);
        scanf("%d", &choice);
        sdu_num = (UINT16) choice;

        for (j = 0U; j < sdu_num; j ++)
        {
            /* Select SDU Size Randomly */
            BT_LOOP_FOREVER()
            {
                sdu_size = rand() % (out_mtu + 1U);

                if ((sdu_size > out_mtu) ||
                    (sdu_size <= BT_STATIC_DATA_SIZE))
                {
                    continue;
                }
                else
                {
                    break;
                }
            }

            sdu_data = (UCHAR *) BT_alloc_mem (sdu_size + L2CAP_HDR_LEN);
            if (sdu_data == NULL)
            {
                printf("*** FAILED to Allocate Memory for SDU Payload\n");
                break; /* return; */
            }

            /* Form SDU Data Pattern */
            BT_mem_set (sdu_data, 0x0, L2CAP_HDR_LEN);
            for (i = L2CAP_HDR_LEN; i < (sdu_size + L2CAP_HDR_LEN); i ++)
            {
                sdu_data[i] = (UCHAR) i;
            }

            /* MISRA C-2012 Rule 9.1 */
            actual = 0U;

            /* Send Data */
            retval = l2ca_data_write
                     (
                         l2cap_dt_entity[index].lcid,
                         (UINT16)(sdu_size + L2CAP_HDR_LEN),
                         sdu_data,
                         &actual
                     );

            printf("-> L2CA_DataWriteREQ. SDU Size: %d Bytes\n",
            sdu_size);

            if (actual != sdu_size)
            {
                printf("*** Actual (%d) does NOT match Requested SDU Size (%d)\n",
                actual, sdu_size);

                break; /* return; */
            }

            if (retval == L2CAP_FEC_TX_WINDOW_FULL)
            {
                printf("Waiting for TxWindow to become Free ...\n");
                l2cap_dt_entity[index].waiting_for_signal = BT_TRUE;
                l2cap_dt_wait_for_tx_win_free();
                l2cap_dt_entity[index].waiting_for_signal = BT_FALSE;
            }
            else if (retval != API_SUCCESS)
            {
                printf("*** L2CA_DataWriteREQ. Reason = 0x%04X\n", retval);
                break; /* return; */
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        } /* for */
    }
}


void l2cap_dt_show_channels (void)
{
    UCHAR index;

    for (index = 0U; index < L2CAP_DT_MAX_ENTITY; index ++)
    {
        if (l2cap_dt_entity[index].valid == 0x1U)
        {
            l2cap_dt_show_channels_index (index);
        }
    }
}


void l2cap_dt_show_channels_index (UCHAR index)
{
    printf("[%d] LCID 0x%04X\n",
    index, l2cap_dt_entity[index].lcid);

    printf("     InMTU  : %d\n", l2cap_dt_entity[index].in_mtu);
    printf("     OutMTU : %d\n", l2cap_dt_entity[index].out_mtu);

    printf("     InFEC Parameters\n");
    printf("         Mode = 0x%02X, MPS = 0x%04X (%d)\n",
    l2cap_dt_entity[index].in_fec.mode,
    l2cap_dt_entity[index].in_fec.mps,
    l2cap_dt_entity[index].in_fec.mps);
    printf("         TxWindow = 0x%02X (%d), MaxTransmit = 0x%02X (%d)\n",
    l2cap_dt_entity[index].in_fec.tx_window,
    l2cap_dt_entity[index].in_fec.tx_window,
    l2cap_dt_entity[index].in_fec.max_transmit,
    l2cap_dt_entity[index].in_fec.max_transmit);
    printf("         RetxTO = 0x%04X (%d), MonitorTO = 0x%04X (%d)\n",
    l2cap_dt_entity[index].in_fec.retx_timeout,
    l2cap_dt_entity[index].in_fec.retx_timeout,
    l2cap_dt_entity[index].in_fec.monitor_timeout,
    l2cap_dt_entity[index].in_fec.monitor_timeout);

    printf("     OutFEC Parameters\n");
    printf("         Mode = 0x%02X, MPS = 0x%04X (%d)\n",
    l2cap_dt_entity[index].out_fec.mode,
    l2cap_dt_entity[index].out_fec.mps,
    l2cap_dt_entity[index].out_fec.mps);
    printf("         TxWindow = 0x%02X (%d), MaxTransmit = 0x%02X (%d)\n",
    l2cap_dt_entity[index].out_fec.tx_window,
    l2cap_dt_entity[index].out_fec.tx_window,
    l2cap_dt_entity[index].out_fec.max_transmit,
    l2cap_dt_entity[index].out_fec.max_transmit);
    printf("         RetxTO = 0x%04X (%d), MonitorTO = 0x%04X (%d)\n",
    l2cap_dt_entity[index].out_fec.retx_timeout,
    l2cap_dt_entity[index].out_fec.retx_timeout,
    l2cap_dt_entity[index].out_fec.monitor_timeout,
    l2cap_dt_entity[index].out_fec.monitor_timeout);
}


void l2cap_dt_wait (void)
{
#if 0
    BT_thread_mutex_lock (&l2cap_dt_mutex);
    BT_thread_cond_wait (&l2cap_dt_cond, &l2cap_dt_mutex);
    BT_thread_mutex_unlock (&l2cap_dt_mutex);
#else
    printf ("Wait for the completion ....\n");
#endif /* 0 */
}


void l2cap_dt_signal (void)
{
#if 0
    BT_thread_mutex_lock (&l2cap_dt_mutex);
    BT_thread_cond_signal (&l2cap_dt_cond);
    BT_thread_mutex_unlock (&l2cap_dt_mutex);
#else
    printf ("Proceed ...\n");
#endif /* 0 */
}

void l2cap_dt_wait_for_tx_win_free(void)
{
#if 1
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&l2cap_dt_mutex);
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait(&l2cap_dt_cond, &l2cap_dt_mutex);
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&l2cap_dt_mutex);
#else
    printf("Wait for the completion ....\n");
#endif /* 0 */
}

void l2cap_dt_signal_tx_win_free(void)
{
#if 1
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&l2cap_dt_mutex);
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal(&l2cap_dt_cond);
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&l2cap_dt_mutex);
#else
    printf("Proceed ...\n");
#endif /* 0 */
}


#ifdef L2CAP_TEST_PACKET_DROP
UCHAR l2cap_test_packet_drop_tx (UCHAR *header, UCHAR header_len)
{
    INT32 num;
    UCHAR tx_seq, req_seq;
    UCHAR ret_val;

    /* Get ReqSeq */
    req_seq = header[5U] & 0x3FU;
    ret_val = 0U;

    if ((header[4U] & 0x01U) == 0x01U)
    {
        /* S-Frame */
        switch (header[4U] & 0x0CU)
        {
        case 0x00U:
            /* RR */
            printf("-> RR: ReqSeq %d", req_seq);
            break;

        case 0x04U:
            /* REJ */
            printf("-> REJ: ReqSeq %d", req_seq);
            break;

        case 0x08U:
            /* RNR */
            printf("-> RNR: ReqSeq %d", req_seq);
            break;

        case 0x0CU:
            /* SREJ */
            printf("-> SREJ: ReqSeq %d", req_seq);
            break;

        default:
            printf("Invalid S-Frame type: 0x%02X\n", (header[4U] & 0x0CU));
            break;
        }

        if ((header[4U] & 0x10U) == 0x00U)
        {
            printf(" P=0");
        }
        else
        {
            printf(" P=1");
        }
        if ((header[4U] & 0x80U) == 0x00U)
        {
            printf(" F=0");
        }
        else
        {
            printf(" F=1");
        }
    }
    else
    {
        /* Get TxSeq */
        tx_seq = (header[4U] & 0x7EU) >> 1U;

        /* I-Frame */
        printf("-> I: TxSeq %d, ReqSeq %d", tx_seq, req_seq);

        if ((header[4U] & 0x80U) == 0x00U)
        {
            printf(" F=0");
        }
        else
        {
            printf(" F=1");
        }
    }

    switch (l2cap_dt_drop_flag)
    {
    case 0x01U:
        /* Drop all S-frame(s) Acknowledgements */
        if ((header_len > 4U) &&
            ((header[4U] & 0x01U) == 0x01U))
        {
            printf(" *\n");
            ret_val = 1U; /* return 1U; */
        }

        break;

    case 0x02U:
        num = rand() % 100U;
        if (num < l2cap_dt_drop_percentage)
        {
            printf(" *\n");
            ret_val = 1U; /* return 1U; */
        }
        break;

    default:
        printf("Invalid Drop Flag: %d\n", l2cap_dt_drop_flag);
        break;
    }

    printf("\n");
    return ret_val;
}


UCHAR l2cap_test_packet_drop_rx (UCHAR *header, UCHAR header_len)
{
    UCHAR tx_seq, req_seq;

    /* Get ReqSeq */
    req_seq = header[5U] & 0x3FU;

    if ((header[4U] & 0x01U) == 0x01U)
    {
        /* S-Frame */
        switch (header[4U] & 0x0CU)
        {
        case 0x00U:
            /* RR */
            printf("<- RR: ReqSeq %d", req_seq);
            break;

        case 0x04U:
            /* REJ */
            printf("<- REJ: ReqSeq %d", req_seq);
            break;

        case 0x08U:
            /* RNR */
            printf("<- RNR: ReqSeq %d", req_seq);
            break;

        case 0x0CU:
            /* SREJ */
            printf("<- SREJ: ReqSeq %d", req_seq);
            break;

        default:
            printf("Invalid S-Frame type: 0x%02X\n", (header[4U] & 0x0CU));
            break;
        }

        if ((header[4U] & 0x10U) == 0x00U)
        {
            printf(" P=0");
        }
        else
        {
            printf(" P=1");
        }

        if ((header[4U] & 0x80U) == 0x00U)
        {
            printf(" F=0");
        }
        else
        {
            printf(" F=1");
        }
    }
    else
    {
        /* Get TxSeq */
        tx_seq = (header[4U] & 0x7EU) >> 1U;

        /* I-Frame */
        printf("<- I: TxSeq %d, ReqSeq %d", tx_seq, req_seq);

        if ((header[4U] & 0x80U) == 0x00U)
        {
            printf(" F=0");
        }
        else
        {
            printf(" F=1");
        }
    }

    printf("\n");

    BT_IGNORE_UNUSED_PARAM(header_len);

    return 0U;
}
#endif /* L2CAP_TEST_PACKET_DROP */

#ifdef BT_UCD

void l2cap_dt_send_data_ucd (void)
{
    int choice;
    UCHAR *sdu_data;
    UINT16 i, j, sdu_size, sdu_num, actual;
    API_RESULT retval;
    DEVICE_HANDLE handle;
    BT_DEVICE_ADDR bd_addr;

    printf("\n");
    printf("Sending Data over L2CAP Channel (Connectionless Unicast) ...\n");

    bd_addr.type = DQ_LINK_NONE;
    BT_COPY_BD_ADDR(bd_addr.addr, l2cap_dt_remote_bd_addr);

    /* retval = device_queue_search_br_edr_remote_addr(&handle, &bd_addr); */
    retval = device_queue_search_any_remote_addr(&handle, &bd_addr);

    if (API_SUCCESS != retval)
    {
        printf("Failed to find remote device in Device Queue\n");
        /* return; */
    }
    else
    {
        printf("Enter L2CAP SDU Size = "); fflush(stdout);
        scanf("%d", &choice);
        sdu_size = (UINT16) choice;

        printf("Enter Number of L2CAP SDU = "); fflush(stdout);
        scanf("%d", &choice);
        sdu_num = (UINT16) choice;

        for (j = 0U; j < sdu_num; j ++)
        {
            sdu_data = (UCHAR *) BT_alloc_mem (sdu_size + L2CAP_HDR_LEN);
            if (sdu_data == NULL)
            {
                printf("*** FAILED to Allocate Memory for SDU Payload\n");
                break; /* return; */
            }

            /* Form SDU Data Pattern */
            BT_mem_set (sdu_data, 0x0, L2CAP_HDR_LEN);
            for (i = L2CAP_HDR_LEN; i < (sdu_size + L2CAP_HDR_LEN); i ++)
            {
                sdu_data[i] = (UCHAR) i;
            }

            /* MISRA C-2012 Rule 9.1 */
            actual = 0U;

            /* Send Unicast Connectionless Data */
            retval = l2ca_ucd_write
                     (
                         &handle,
                         (UINT16)(sdu_size + L2CAP_HDR_LEN),
                         sdu_data,
                         &actual
                     );

            printf("-> L2CA_DataWriteREQ. SDU Size: %d Bytes.\n", sdu_size);

            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);

                break;
            }
            else
            {
                printf("Succeeded !!\n");

                if (actual != sdu_size)
                {
                    printf("*** Actual (%d) does NOT match Requested SDU Size (%d)\n",
                    actual, sdu_size);

                    break; /* return; */
                }
            }
        }
    }
}

API_RESULT appl_l2cap_ucd_data_cb
           (
               DEVICE_HANDLE * handle,
               UCHAR         * data,
               UINT16          datalen
           )
{
    UINT16 psm;

    printf("Received Unicast Connectionless Data (%d bytes) from Device Handle: 0x%02X\n", datalen, (*handle));

    /* Extract PSM */
    if (2U > datalen)
    {
        printf("Invalid G-Frame. PSM field missing\n");

        return API_FAILURE;
    }

    BT_UNPACK_LE_2_BYTE(&psm, data);
    printf("[G-frame] PSM 0x%04X\n", psm);

    /* Dump Information Payload */
    printf("[G-frame] Info Payload of Length %d\n", (datalen - 2U));
    appl_dump_bytes(&data[2U], (datalen - 2U));

    return API_SUCCESS;
}
#endif /* BT_UCD */
#endif /* BR_EDR_L2CAP */

