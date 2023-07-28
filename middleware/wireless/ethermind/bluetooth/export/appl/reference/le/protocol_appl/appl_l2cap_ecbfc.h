
/**
 *  \file appl_l2cap_ecbfc.h
 *
 *  Header File for L2CAP Enhanced Credit Based Flow Control Mode Command Line Application
 */

/*
 *  Copyright (C) 2019. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_L2CAP_ECBFC_
#define _H_APPL_L2CAP_ECBFC_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "l2cap.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */


/* ----------------------------------------- Function Declarations */

void main_l2cap_ecbfc_operations(void);

/* No definition found */
#if 0
void appl_l2cap_ecbfc_register(void);
void appl_l2cap_ecbfc_callback
     (
         DEVICE_HANDLE * handle,
         UCHAR           event_code,
         UCHAR         * data,
         UINT16          packet_len
     );
#endif /*0 */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
API_RESULT appl_l2ca_ecbfc_connect_ind_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      psm,
               L2CAP_ECBFC_CONNECT_PARAM * param
           );

API_RESULT appl_l2ca_ecbfc_connect_cnf_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      response,
               L2CAP_ECBFC_CONNECT_PARAM * param
           );

API_RESULT appl_l2ca_ecbfc_reconfig_ind_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                   * lcid,
               L2CAP_ECBFC_RECONFIG_PARAM * param
           );

API_RESULT appl_l2ca_ecbfc_reconfig_cnf_cb(DEVICE_HANDLE *handle);
API_RESULT appl_l2ca_ecbfc_disconnect_ind_cb(UINT16 lcid);
API_RESULT appl_l2ca_ecbfc_disconnect_cnf_cb(UINT16 lcid, UINT16 reason);
API_RESULT appl_l2ca_ecbfc_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);
API_RESULT appl_l2ca_ecbfc_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit);
API_RESULT appl_l2ca_ecbfc_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit);
API_RESULT appl_l2ca_ecbfc_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len);

API_RESULT appl_l2cap_ecbfc_send_data
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UINT16          cid,
               /* IN */ UINT16          buffer_len,
               /* IN */ UINT32          count
           );

/* No definition found */
#if 0
API_RESULT appl_l2ca_ecbfc_channel_data_write
           (
                /* IN */ UINT16             cid,
                /* IN */ UCHAR *            buffer,
                /* IN */ UINT16             buffer_len
           );
#endif /* 0 */

/* File Send and Receive related */
#ifdef APPL_L2CAP_SUPPORT_FILE_OP
void appl_l2cap_ecbfc_recv_file(void);
void appl_l2cap_ecbfc_send_file(void);
void appl_l2cap_ecbfc_stop_recv_file(void);
void appl_l2cap_print_file_transfer_stat (void);
#else
#define appl_l2cap_ecbfc_recv_file()
#define appl_l2cap_ecbfc_send_file()
#define appl_l2cap_ecbfc_stop_recv_file()
#define appl_l2cap_print_file_transfer_stat()
#endif /* APPL_L2CAP_SUPPORT_FILE_OP */

#endif /* L2CAP_SUPPORT_ECBFC_MODE */

#endif /* _H_APPL_L2CAP_ECBFC_ */

