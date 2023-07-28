
/**
 *  \file hps_pl.h
 *
 *  This File contains the platform abstraction routines of LE HTTP Proxy
 *  Serviceto Transmit and Receive Data over TCP/IP protocol.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HPS_PL_
#define _H_HPS_PL_

/* -------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "niface.h"

/* -------------------------------------------- Global Definitions */
#ifndef HPS_PL_NO_DEBUG
#define HPS_PL_ERR(...)         BT_debug_error(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else  /* HPS_PL_NO_DEBUG */
#define HPS_PL_ERR              BT_debug_null
#endif /* HPS_PL_NO_DEBUG */

#ifdef HPS_PL_DEBUG
#define HPS_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#define HPS_PL_INF(...)         BT_debug_info (BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else /* HPS_PL_DEBUG */
#define HPS_PL_TRC              BT_debug_null
#define HPS_PL_INF              BT_debug_null
#endif /* HPS_PL_DEBUG */

/** HPS related error codes */
#define HPS_PL_ERR_ID                              0xF000U
#define HPS_PL_MUTEX_INIT_FAILED                   \
        (0x0001U | HPS_PL_ERR_ID)
#define HPS_PL_COND_INIT_FAILED                    \
        (0x0002U | HPS_PL_ERR_ID)
#define HPS_PL_MUTEX_LOCK_FAILED                   \
        (0x0003U | HPS_PL_ERR_ID)
#define HPS_PL_MUTEX_UNLOCK_FAILED                 \
        (0x0004U | HPS_PL_ERR_ID)
#define HPS_PL_MEMORY_ALLOCATION_FAILED            \
        (0x0005U | HPS_PL_ERR_ID)

/* HPS - HTTP Client Application Callback */
typedef API_RESULT (* HPS_TRANSPORT_CB_PL)
                   (
                       UCHAR  * hps_http_rsp,
                       UINT16 hps_http_rsp_len
                   );

/* -------------------------------------------- Function Declarations */
/**
 * HPS PL interface for transport Initialization.
 * This interface also enables Application to register RX Data Callback
 * via TCP-IP.
 */
API_RESULT hps_transport_init_pl
           (
               HPS_TRANSPORT_CB_PL cb
           );

/** HPS PL interface to send data over transport */
API_RESULT hps_transport_write_data_pl
           (
               UCHAR  * data,
               UINT16 data_len
           );

/** HPS PL interface to Open TCP-IP Transport */
API_RESULT hps_transport_open_pl (UCHAR * ip_addr);

/** HPS PL interface to Close TCP-IP Transport */
void hps_transport_close_pl (void);

/* HPS PL Internal Functions to Send Data */
void hps_socket_write_data_pl (UCHAR * buf, UINT16 length);

/* HPS PL Fetch IP Address from Host-Name */
API_RESULT hps_fetch_ip_from_hostname_pl
           (
               UCHAR * hostname,
               UCHAR * ip
           );

#endif /* _H_HPS_PL_ */

