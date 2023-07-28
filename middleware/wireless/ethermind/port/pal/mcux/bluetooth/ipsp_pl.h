
/**
 *  \file ipsp_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_IPSP_PL_
#define _H_IPSP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "niface.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifndef IPSP_PL_NO_DEBUG
#define IPSP_PL_ERR(...)             BT_debug_error(BT_MODULE_ID_IPSP, __VA_ARGS__)
#else  /* IPSP_PL_NO_DEBUG */
#define IPSP_PL_ERR                  BT_debug_null
#endif /* IPSP_PL_NO_DEBUG */

#ifdef IPSP_PL_DEBUG
    #define IPSP_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_IPSP, __VA_ARGS__)
    #define IPSP_PL_INF(...)         BT_debug_info(BT_MODULE_ID_IPSP, __VA_ARGS__)
#else /* IPSP_PL_DEBUG */
    #define IPSP_PL_TRC              BT_debug_null
    #define IPSP_PL_INF              BT_debug_null
#endif /* IPSP_PL_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef IPSP_HAVE_6LO_NIFACE
void ipsp_init_pl
     (
         BT_DEVICE_ADDR * local_mac,
         void (* read_cb)(UCHAR *data, UINT16 datalen),
         UCHAR is_router
     );
#endif
void ipsp_start_pl (BT_DEVICE_ADDR * remote_mac);
void ipsp_stop_pl(void);
void ipsp_write_pl (UCHAR * eth_data, UINT16 eth_datalen);

#endif /* _H_IPSP_PL_*/
