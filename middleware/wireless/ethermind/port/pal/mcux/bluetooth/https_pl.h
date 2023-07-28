
/**
 *  \file https_pl.h
 *
 *  This File contains the platform abstraction routines of LE HPS Proxy
 *  Serviceto Transmit and Receive Data over TCP/IP protocol.
 */


#ifndef _H_HTTPS_PL_
#define _H_HTTPS_PL_

/* -------------------------------------------- Header File Inclusion */
#include "BT_common.h"

#include "lwip/opt.h"
#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/netdb.h"

/* -------------------------------------------- Global Definitions */
#ifndef HTTPS_PL_NO_DEBUG
#define HTTPS_PL_ERR(...)         BT_debug_error(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else  /* HTTPS_PL_NO_DEBUG */
#define HTTPS_PL_ERR              BT_debug_null
#endif /* HTTPS_PL_NO_DEBUG */

#ifdef HTTPS_PL_DEBUG
#define HTTPS_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#define HTTPS_PL_INF(...)         BT_debug_info (BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else /* HTTPS_PL_DEBUG */
#define HTTPS_PL_TRC              BT_debug_null
#define HTTPS_PL_INF              BT_debug_null
#endif /* HTTPS_PL_DEBUG */

/** HTTPS related error codes */
#define HTTPS_PL_ERR_ID                              0xF100U
#define HTTPS_PL_MUTEX_INIT_FAILED                   \
        (0x0001U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_COND_INIT_FAILED                    \
        (0x0002U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_MUTEX_LOCK_FAILED                   \
        (0x0003U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_MUTEX_UNLOCK_FAILED                 \
        (0x0004U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_MEMORY_ALLOCATION_FAILED            \
        (0x0005U | HTTPS_PL_ERR_ID)

#define HTTPS_PL_HTTPS_TRANSPORT_EST_ERR             (0x0006U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_NETWORK_NOT_AVAILABLE               (0x0007U | HTTPS_PL_ERR_ID)
#define HTTPS_PL_HTTPS_CONN_NOT_IMPLEMENTED          (0x0008U | HTTPS_PL_ERR_ID)

/* HTTPS - HTTP Client Application Callback */
typedef API_RESULT (* HTTPS_TRANSPORT_CB_PL)
                   (
                       UCHAR  * https_http_rsp,
                       UINT16 https_http_rsp_len
                   );

/* -------------------------------------------- Function Declarations */
/**
 * HTTPS PL interface for transport Initialization.
 * This interface also enables Application to register RX Data Callback
 * via TCP-IP.
 */
API_RESULT https_transport_init_pl
           (
               HTTPS_TRANSPORT_CB_PL cb
           );

/**
 * HTTPS PL interface for transport Deinitialization.
 */
API_RESULT https_transport_deinit_pl
           (
               void
           );

/** HTTPS PL interface to send data over transport */
API_RESULT https_transport_write_data_pl
           (
               UCHAR  * data,
               UINT16 data_len
           );

/** HTTPS PL interface to Open TCP-IP Transport */
API_RESULT https_transport_open_pl (UCHAR * ip_addr);

/** HTTPS PL interface to Close TCP-IP Transport */
void https_transport_close_pl (void);

#endif /* _H_HTTPS_PL_ */

