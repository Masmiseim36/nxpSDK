
/**
 *  \file appl_l2cap_le.h
 *
 *  Header File for LE L2CAP Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_L2CAP_LE_
#define _H_APPL_L2CAP_LE_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "l2cap.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */


/* ----------------------------------------- Function Declarations */

void main_l2cap_le_operations(void);
void appl_l2cap_le_register(void);

void appl_l2cap_le_callback
     (
         DEVICE_HANDLE * handle,
         UCHAR                 event_code,
         UCHAR               * data,
         UINT16                packet_len
     );

#ifdef L2CAP_SUPPORT_CBFC_MODE
API_RESULT appl_l2ca_connect_ind_cb (DEVICE_HANDLE *, UINT16, UINT16, L2CAP_CBFC_CONNECT_PARAM *);
API_RESULT appl_l2ca_connect_cnf_cb (DEVICE_HANDLE *, UINT16, UINT16, L2CAP_CBFC_CONNECT_PARAM *);
API_RESULT appl_l2ca_disconnect_ind_cb(UINT16);
API_RESULT appl_l2ca_disconnect_cnf_cb(UINT16, UINT16);
API_RESULT appl_l2ca_data_read_cb (UINT16, UINT16, UCHAR *, UINT16);
API_RESULT appl_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit);
API_RESULT appl_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit);
API_RESULT appl_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len);

API_RESULT appl_le_l2cap_co_send_data
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UINT16          cid,
               /* IN */ UINT16          buffer_len,
               /* IN */ UINT32          count
           );
API_RESULT appl_l2ca_channel_data_write
           (
                /* IN */ UINT16             cid,
                /* IN */ UCHAR *            buffer,
                /* IN */ UINT16             buffer_len
            );

/* File Send and Receive related */
#ifdef APPL_L2CAP_SUPPORT_FILE_OP
void appl_l2cap_cbfc_recv_file(void);
void appl_l2cap_cbfc_send_file(void);
void appl_l2cap_cbfc_stop_recv_file(void);
void appl_l2cap_print_file_transfer_stat (void);
#else
#define appl_l2cap_cbfc_recv_file()
#define appl_l2cap_cbfc_send_file()
#define appl_l2cap_cbfc_stop_recv_file()
#define appl_l2cap_print_file_transfer_stat()
#endif /* APPL_L2CAP_SUPPORT_FILE_OP */

#endif /* L2CAP_SUPPORT_CBFC_MODE */

#endif /* _H_APPL_L2CAP_LE_ */

