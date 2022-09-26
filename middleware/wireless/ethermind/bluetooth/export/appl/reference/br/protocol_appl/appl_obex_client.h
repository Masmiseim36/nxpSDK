/**
 *  \file appl_obex_client.h
 *
 *  Header File for OBEX Client Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_OBEX_CLIENT_
#define _H_APPL_OBEX_CLIENT_

/* ----------------------------------------- Header File Inclusion */
#include "appl_obex_common.h"
#include "BT_hci_api.h"
#include "BT_fops.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Structures/Data Types */

/* ----------------------------------------- Macros */

/**
 * OBEX Client Application States 
 */
#define OBEX_CLIENT_STATE_IDLE                       0x01U
#define OBEX_CLIENT_STATE_INITIALIZED                0x02U
#define OBEX_CLIENT_STATE_IN_TRANS_CONNECT           0x03U
#define OBEX_CLIENT_STATE_TRANS_CONNECTED            0x04U
#define OBEX_CLIENT_STATE_IN_CONNECT                 0x05U
#define OBEX_CLIENT_STATE_CONNECTED                  0x06U
#define OBEX_CLIENT_STATE_W4_REQ                     0x07U
#define OBEX_CLIENT_STATE_W4_RSP                     0x08U
#define OBEX_CLIENT_STATE_IN_GET                     0x09U
#define OBEX_CLIENT_STATE_IN_PUT                     0x0AU
#define OBEX_CLIENT_STATE_IN_DISCONNECT              0x0BU
#define OBEX_CLIENT_STATE_IN_TRANS_DISCONNECT        0x0CU
#define OBEX_CLIENT_STATE_FOR_ABORT                  0x0DU
#define OBEX_CLIENT_STATE_IN_ABORT                   0x0EU
#define OBEX_CLIENT_STATE_STOP_PENDING               0x0FU

#define OBEX_CLIENT_STATE_INVALID                    0xFFU

/* ----------------------------------------- Function Declarations */
void main_obex_client_operations (void);

void appl_obex_client_print_instances(void);

API_RESULT appl_obex_client_init (void);

API_RESULT appl_obex_client_shutdown(void);

API_RESULT appl_obex_client_callback
           (
               OBEX_HANDLE * obex_handle,
               UINT16        event_result,
               UCHAR         event_type,
               void        * event_data,
               UINT16        event_datalen
           );

API_RESULT appl_obex_client_start
           (
               APPL_OBEX_HANDLE *obex_app_handle,
               OBEX_NOTIFY_CB obex_notify_cb
           );

API_RESULT appl_obex_client_stop
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

API_RESULT appl_obex_client_connect 
           ( 
                APPL_OBEX_HANDLE *obex_app_handle,
                APPL_OBEX_CONNECT_STRUCT * obex_connect_info
           );

API_RESULT appl_obex_client_transport_disconnect 
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

API_RESULT appl_obex_client_disconnect 
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

API_RESULT appl_obex_client_send_req
           (
               APPL_OBEX_HANDLE        *obex_app_handle,  
               UINT8                    event_type,
               UINT16                   event_result,
               APPL_OBEX_REQ_STRUCT    *app_req_hdrs,
               UCHAR                    more
           );

API_RESULT appl_obex_client_push_file 
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

API_RESULT appl_obex_client_pull_file
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

API_RESULT appl_obex_client_setpath
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

#ifdef OBEX_ENH_SUPPORT
API_RESULT appl_obex_client_manipulate_obj
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );
#endif /* OBEX_ENH_SUPPORT */

API_RESULT appl_obex_client_abort
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

#endif /* _H_APPL_OBEX_CLIENT_ */

