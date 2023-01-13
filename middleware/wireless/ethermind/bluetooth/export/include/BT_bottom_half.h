
/*
 *  \file BT_bottom_half.h
 *
 *  This file lists all the Write Task Bottom Half function handlers
 *  under EtherMind Protocol Suite.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_BOTTOM_HALF_
#define _H_BT_BOTTOM_HALF_

/*
 *  Theory:
 *  There are three kinds of Bottom Halves, as provided by the EtherMind
 *  Write Task module:
 *    1. User Data Bottom Halves (eg, RFCOMM/BNEP Write)
 *    2. User API Bottom Halves
 *    3. Internal (eg, Timer/Internal Loopback handlers)
 *
 *  As required by the Write Task module, all EtherMind Bottom Halves
 *  have the same format:
 *      void bh_handler_function (void *args, UINT16 length);
 */

/* Macro to declare a Bottom Half handler */
#define BT_BOTTOM_HALF(x) \
        void x (void *args, UINT16 args_len)


/* ============================================= User Data Bottom Halves */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------- Section 'HCI' */
#ifdef HCI_SCO
BT_BOTTOM_HALF ( hci_sco_data_bh );
#endif /* HCI_SCO */

/* --------------------------------------------- Section 'RFCOMM' */
BT_BOTTOM_HALF ( rfcomm_write_bh );

/* --------------------------------------------- Section 'BNEP' */
BT_BOTTOM_HALF ( bnep_write_bh );

/* --------------------------------------------- Section 'AVDTP' */
BT_BOTTOM_HALF ( avdtp_media_write_bh );

/* --------------------------------------------- Section 'AVCTP' */
BT_BOTTOM_HALF ( avctp_send_message_bh );

/* --------------------------------------------- Section 'MCAP' */
BT_BOTTOM_HALF ( mcap_md_write_bh );

/* ===================================================================== */


/* ============================================= User API Bottom Halves  */
/* --------------------------------------------- Section 'HCI' */
BT_BOTTOM_HALF ( hci_common_api_bh );

/* --------------------------------------------- Section 'SM' */
BT_BOTTOM_HALF ( sm_ui_notification_bh );

/* --------------------------------------------- Section 'SDP' */
BT_BOTTOM_HALF ( sdp_request_bh );

/* --------------------------------------------- Section 'RFCOMM' */
BT_BOTTOM_HALF ( rfcomm_open_bh );
BT_BOTTOM_HALF ( rfcomm_close_bh );
BT_BOTTOM_HALF ( rfcomm_send_pn_bh );
BT_BOTTOM_HALF ( rfcomm_send_msc_bh );
BT_BOTTOM_HALF ( rfcomm_send_rpn_bh );
BT_BOTTOM_HALF ( rfcomm_send_rls_bh );
BT_BOTTOM_HALF ( rfcomm_session_fc_bh );

#ifndef RFCOMM_NO_SEND_TEST
BT_BOTTOM_HALF ( rfcomm_session_test_bh );
#endif /* RFCOMM_NO_SEND_TEST */

BT_BOTTOM_HALF ( rfcomm_credit_tx_bh );

#ifdef RFCOMM_ENABLE_SUPPORT_APIS
BT_BOTTOM_HALF ( rfcomm_session_open_bh );
BT_BOTTOM_HALF ( rfcomm_session_close_bh );
BT_BOTTOM_HALF ( rfcomm_dlc_open_bh );
BT_BOTTOM_HALF ( rfcomm_dlc_close_bh );
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */

BT_BOTTOM_HALF ( rfcomm_user_accept_cancel_bh );

/* --------------------------------------------- Section 'BNEP' */
BT_BOTTOM_HALF ( bnep_connect_bh );
BT_BOTTOM_HALF ( bnep_disconnect_bh );
BT_BOTTOM_HALF ( bnep_setup_bh );
BT_BOTTOM_HALF ( bnep_setup_rsp_bh );
BT_BOTTOM_HALF ( bnep_filter_bh );

/* --------------------------------------------- Section 'AVDTP' */
BT_BOTTOM_HALF ( avdtp_connect_req_bh );
BT_BOTTOM_HALF ( avdtp_disconnect_req_bh );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_1 );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_2 );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_3 );

/* --------------------------------------------- Section 'AVCTP' */
BT_BOTTOM_HALF ( avctp_connect_req_bh );
BT_BOTTOM_HALF ( avctp_disconnect_req_bh );

/* --------------------------------------------- Section 'MCAP' */
BT_BOTTOM_HALF ( mcap_md_command_bh );
BT_BOTTOM_HALF ( mcap_mc_bh );
BT_BOTTOM_HALF ( mcap_sync_bh );

/* --------------------------------------------- Section 'OBEX' */
BT_BOTTOM_HALF ( obex_connect_req_bh );
BT_BOTTOM_HALF ( obex_disconnect_req_bh );
BT_BOTTOM_HALF ( obex_send_bh );

/* ===================================================================== */


/* ============================================= Internal Bottom Halves  */
/* --------------------------------------------- Section 'HCI Transport' */
#ifdef BT_TRANSPORT_SINGLE_TASK
BT_BOTTOM_HALF ( hci_transport_data_read_bh );
#endif /* BT_TRANSPORT_SINGLE_TASK */

/* --------------------------------------------- Section 'BCSP' */
#if 0
BT_BOTTOM_HALF ( bcsp_link_establishment_bh );
BT_BOTTOM_HALF ( bcsp_sequence_tx_bh );
BT_BOTTOM_HALF ( bcsp_sequence_ack_bh );
#endif /* 0 */

/* --------------------------------------------- Section 'HCI' */
BT_BOTTOM_HALF ( hci_command_tx_bh );

#ifdef HCI_ISO_DATA
BT_BOTTOM_HALF(hci_iso_data_tx_bh);
#endif /* HCI_ISO_DATA */

/* --------------------------------------------- Section 'L2CAP' */
BT_BOTTOM_HALF ( l2cap_data_transmitter_bh );
BT_BOTTOM_HALF ( l2cap_fec_signal_handler_bh );
BT_BOTTOM_HALF ( l2cap_fec_data_receiver_bh );

#if ((defined L2CAP_SUPPORT_CBFC_MODE) || (defined L2CAP_SUPPORT_ECBFC_MODE))
BT_BOTTOM_HALF ( l2ca_channel_data_write_bh );
#endif /* L2CAP_SUPPORT_CBFC_MODE || L2CAP_SUPPORT_ECBFC_MODE */

/* --------------------------------------------- Section 'SMP' */
BT_BOTTOM_HALF ( smp_auth_complete_bh );
#ifdef SMP_HAVE_TBX_CMD_WT_BH
BT_BOTTOM_HALF ( smp_tbx_commands_bh );
#endif /* SMP_HAVE_TBX_CMD_WT_BH */

/* ===================================================================== */
#ifdef __cplusplus
};
#endif

#endif /* _H_BT_BOTTOM_HALF_ */

