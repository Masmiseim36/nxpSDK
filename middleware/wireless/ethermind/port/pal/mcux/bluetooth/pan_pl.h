
/**
 *  \file pan_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_PAN_PL_
#define _H_PAN_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "niface.h"

/* --------------------------------------------- Global Definitions */
/* Base folder for PAN objects */
#define PAN_ROOT_FOLDER_BASE \
    BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "pan")

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifndef PAN_PL_NO_DEBUG
#define PAN_PL_ERR(...)             BT_debug_error(BT_MODULE_ID_PAN, __VA_ARGS__)
#else  /* PAN_PL_NO_DEBUG */
#define PAN_PL_ERR                  BT_debug_null
#endif /* PAN_PL_NO_DEBUG */

#ifdef PAN_PL_DEBUG
    #define PAN_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_PAN, __VA_ARGS__)
    #define PAN_PL_INF(...)         BT_debug_info(BT_MODULE_ID_PAN, __VA_ARGS__)
#else /* PAN_PL_DEBUG */
    #define PAN_PL_TRC              BT_debug_null
    #define PAN_PL_INF              BT_debug_null
#endif /* PAN_PL_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void pan_init_pl
     (
         BT_DEVICE_ADDR * mac,
         void (* read_cb)(UCHAR *data, UINT16 datalen),
         UCHAR is_nap
     );
void pan_start_pl (UCHAR dhcp);
void pan_stop_pl(void);
void pan_write_pl (UCHAR * eth_header, UCHAR * eth_data, UINT16 eth_datalen);

#endif /* _H_PAN_PL_*/
