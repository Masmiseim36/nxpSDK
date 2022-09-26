
/**
 *  \file appl_l2cap.h
 *
 *  Header File for L2CAP Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_L2CAP_
#define _H_APPL_L2CAP_

/* ---------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "l2cap.h"

#ifdef BR_EDR_L2CAP
/* ---------------------------------------- Global Definitions */
#define L2CAP_DT_DEVICE_A                       0x1U
#define L2CAP_DT_DEVICE_B                       0x2U

#define L2CAP_DT_MAX_ENTITY                     5U

#define L2CAP_DT_W4_CONNECT_IND                 0x01U
#define L2CAP_DT_W4_CONNECT_CNF                 0x02U
#define L2CAP_DT_W4_CONFIG                      0x03U
#define L2CAP_DT_OPEN                           0x04U
#define L2CAP_DT_W4_DISCONNECT_IND              0x05U
#define L2CAP_DT_W4_DISCONNECT_CNF              0x06U


/* ---------------------------------------- Structures/Data Types */
typedef struct
{
    /* Channel ID */
    UINT16 lcid;

    /* Configured Parameters */
    UINT16 in_mtu, out_mtu;
    UINT16 in_flush_to, out_flush_to;
    FLOW_SPEC in_flow, out_flow;
    L2CAP_FEC_OPTION in_fec, out_fec;
    UCHAR in_fcs, out_fcs;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    L2CAP_EXT_FLOW_SPEC ext_flow_spec;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

    /* Flags */
    UCHAR config_req_sent;
    UCHAR local_config_done;
    UCHAR remote_config_done;

    /* State */
    UCHAR state;
    UCHAR valid;

} L2CAP_DT_ENTITY;


typedef struct
{
    UINT16 in_mtu;
    UINT16 out_flush_to;
    FLOW_SPEC out_flow;
    L2CAP_FEC_OPTION in_fec;
    UCHAR out_fcs;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    L2CAP_EXT_FLOW_SPEC * out_ext_flow_spec;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

} L2CAP_DT_CONFIG_OPTION;


/* ---------------------------------------- Function Declarations */
API_RESULT l2cap_dt_connect_ind
           (
               /* IN */  DEVICE_HANDLE *    handle,
               /* IN */  UINT16             lcid,
               /* IN */  UINT16             psm
           );

API_RESULT l2cap_dt_connect_cnf
           (
               /* IN */  DEVICE_HANDLE *    handle,
               /* IN */  UINT16             lcid,
               /* IN */  UINT16             result,
               /* IN */  UINT16             status
           );

API_RESULT l2cap_dt_config_ind
           (
               /* IN */ UINT16                 lcid,
               /* IN */ L2CAP_CONFIG_OPTION *  config_option
           );

API_RESULT l2cap_dt_config_cnf
           (
               /* IN */ UINT16                 lcid,
               /* IN */ UINT16                 result,
               /* IN */ L2CAP_CONFIG_OPTION *  config_option
           );

API_RESULT l2cap_dt_open_ind
           (
               /* IN */ UINT16    lcid
           );

API_RESULT l2cap_dt_disconnect_ind
           (
               /* IN */ UINT16  lcid
           );

API_RESULT l2cap_dt_disconnect_cnf
           (
               /* IN */ UINT16  lcid,
               /* IN */ UINT16  result
           );

API_RESULT l2cap_dt_data_write_cnf
           (
               /* IN */ UINT16   lcid,
               /* IN */ UCHAR *  data,
               /* IN */ UINT16   datalen
           );

API_RESULT l2cap_dt_data_read
           (
               /* IN */ UINT16   lcid,
               /* IN */ UCHAR *  data,
               /* IN */ UINT16   datalen
           );

API_RESULT l2cap_dt_qos_violation_ind
           (
               /* IN */ UCHAR *  bd_addr
           );

API_RESULT l2cap_dt_data_inconsistent
           (
               /* IN */ UINT16   lcid
           );

API_RESULT l2cap_dt_ping_cnf
           (
               /* IN */ DEVICE_HANDLE *  handle,
               /* IN */ UINT16           result,
               /* IN */ UCHAR         *  echo_data,
               /* IN */ UINT16           length
           );

API_RESULT l2cap_dt_getinfo_cnf
           (
               /* IN */ DEVICE_HANDLE *  handle,
               /* IN */ UINT16           result,
               /* IN */ UCHAR         *  info_data,
               /* IN */ UINT16           info_datalen
           );

API_RESULT l2cap_dt_tx_flow_ind
           (
               /* IN */ UINT16   lcid,
               /* IN */ UCHAR    flow
           );

API_RESULT l2cap_dt_get_fec_params
           (
               /* IN */ UINT16 lcid,
               /* IN */ L2CAP_FEC_OPTION * fec_ptr
           );

/* API_RESULT l2cap_dt_bluetooth_on_complete (void); */

#if 0
API_RESULT l2cap_dt_hci_event_ind
           (
               /* IN */ UCHAR    event_code,
               /* IN */ UCHAR *  event_data,
               /* IN */ UCHAR    event_datalen
           );

API_RESULT l2cap_dt_sm_notify_cb
           (
               /* IN */ UCHAR      event,
               /* IN */ UCHAR *    bd_addr,
               /* IN */ UCHAR *    data
           );
#endif /* 0 */

void l2cap_dt_entity_init (void);
void l2cap_dt_entity_alloc (UCHAR *entity_index);
void l2cap_dt_entity_free (UCHAR index);

void l2cap_dt_get_remote_address(void);
void l2cap_dt_register(void);
void l2cap_dt_create_channels (void);
void l2cap_dt_accept_channels (void);
void l2cap_dt_config_channels (void);
void l2cap_dt_config_channels_index (UCHAR index);
void l2cap_dt_destroy_channels (void);
void l2cap_dt_show_channels (void);
void l2cap_dt_show_channels_index (UCHAR index);

void l2cap_dt_send_data (void);
void l2cap_dt_send_data_random (void);
void l2cap_dt_send_data_ucd(void);
void l2cap_dt_config_option_init (void);

#ifdef BT_UCD
API_RESULT appl_l2cap_ucd_data_cb
           (
               DEVICE_HANDLE * handle,
               UCHAR         * data,
               UINT16          datalen
           );
#endif /* BT_UCD */

#ifdef L2CAP_EXTENDED_FLOW_SPEC
void l2cap_dt_ext_flow_spec_option_init(void);
#else
#define l2cap_dt_ext_flow_spec_option_init()
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

void l2cap_dt_wait (void);
void l2cap_dt_signal (void);

void l2cap_dt_wait_for_tx_win_free(void);
void l2cap_dt_signal_tx_win_free(void);

void l2cap_packet_drop_set_percentage
     (
         /* IN */ INT32  percentage
     );

void main_l2cap_operations (void);

#endif /* BR_EDR_L2CAP */

#endif /* _H_APPL_L2CAP_ */

