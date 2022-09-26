
/**
 *  \file appl_obex_server.h
 *
 *  Header File for OBEX Server Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_OBEX_SERVER_
#define _H_APPL_OBEX_SERVER_

/* ----------------------------------------- Header File Inclusion */
#include "appl_obex_common.h"
#include "BT_fops.h"

/* ----------------------------------------- Global Definitions */

/**
 * OBEX server rfcomm server ch. & L2CAP psm for
 * OBEX transport connection.
 */
#define       APPL_OBEX_SER_RFCOMM_CH         0x07U
#define       APPL_OBEX_SER_L2CAP_PSM         0x1011U

/* ----------------------------------------- Structures/Data Types */

/* ----------------------------------------- Macros */

/**
 * OBEX Server Application States
 */
#define OBEX_SERVER_STATE_IDLE                       0x01U
#define OBEX_SERVER_STATE_INITIALIZED                0x02U
#define OBEX_SERVER_STATE_IN_TRANS_CONNECT           0x03U
#define OBEX_SERVER_STATE_TRANS_CONNECTED            0x04U
#define OBEX_SERVER_STATE_IN_CONNECT                 0x05U
#define OBEX_SERVER_STATE_CONNECTED                  0x06U
#define OBEX_SERVER_STATE_W4_REQ                     0x07U
#define OBEX_SERVER_STATE_W4_RSP                     0x08U
#define OBEX_SERVER_STATE_IN_GET                     0x09U
#define OBEX_SERVER_STATE_IN_PUT                     0x0AU
#define OBEX_SERVER_STATE_IN_DISCONNECT              0x0BU
#define OBEX_SERVER_STATE_IN_TRANS_DISCONNECT        0x0CU
#define OBEX_SERVER_STATE_FOR_ABORT                  0x0DU
#define OBEX_SERVER_STATE_IN_ABORT                   0x0EU
#define OBEX_SERVER_STATE_STOP_PENDING               0x0FU

#define OBEX_SERVER_STATE_INVALID                    0xFFU

/* ----------------------------------------- Function Declarations */

void main_obex_server_operations (void);

void appl_obex_server_print_instances (void);

API_RESULT appl_obex_server_init (void);

API_RESULT appl_obex_server_shutdown (void);

API_RESULT appl_obex_server_start
           (
               APPL_OBEX_HANDLE *obex_appl_handle,
               OBEX_NOTIFY_CB notify_cb
           );

API_RESULT appl_obex_server_stop
           (
               APPL_OBEX_HANDLE *obex_appl_handle
           );

API_RESULT appl_obex_server_callback
           (
               OBEX_HANDLE * obex_handle,
               UINT16        event_result,
               UCHAR         event_type,
               void        * event_data,
               UINT16        event_datalen
           );

API_RESULT appl_obex_server_send_rsp
           (
               UCHAR                    *obex_app_handle,
               UCHAR                     event_type,
               UCHAR                     event_result,
               APPL_OBEX_RSP_STRUCT     *app_rsq_hdrs
           );

API_RESULT appl_obex_server_transport_disconnect
           (
               APPL_OBEX_HANDLE *obex_app_handle
           );

#endif /* _H_APPL_OBEX_SERVER_ */

