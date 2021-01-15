
/**
 *  \file device_queue_pl.h
 *
 *  Platform specific Header File for Device Queue module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_DEVICE_QUEUE_PL_
#define _H_DEVICE_QUEUE_PL_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"


/* ----------------------------------------------- Global Definitions */
#ifndef DQ_PL_NO_DEBUG
#define DQ_PL_ERR(...)               BT_debug_error(BT_MODULE_ID_DQ,__VA_ARGS__)
#else  /* DQ_PL_NO_DEBUG */
#ifdef VAR_ARG_IN_MACRO_NOT_SUPPORTED
#define DQ_PL_ERR
#else
#define DQ_PL_ERR(...)               BT_debug_null(BT_MODULE_ID_DQ,__VA_ARGS__)
#endif  /*  VAR_ARG_IN_MACRO_NOT_SUPPORTED */
#endif /* DQ_PL_NO_DEBUG */

#ifdef DQ_PL_DEBUG

#define DQ_PL_TRC(...)               BT_debug_trace(BT_MODULE_ID_DQ,__VA_ARGS__)
#define DQ_PL_INF(...)               BT_debug_info(BT_MODULE_ID_DQ,__VA_ARGS__)

#else /* DQ_PL_DEBUG */

#ifdef VAR_ARG_IN_MACRO_NOT_SUPPORTED
#define DQ_PL_TRC
#define DQ_PL_INF
#else
#define DQ_PL_TRC(...)               BT_debug_null(BT_MODULE_ID_DQ,__VA_ARGS__)
#define DQ_PL_INF(...)               BT_debug_null(BT_MODULE_ID_DQ,__VA_ARGS__)
#endif  /*  VAR_ARG_IN_MACRO_NOT_SUPPORTED */

#endif /* DQ_PL_DEBUG */

/* ----------------------------------------------- Structures/Data Types */


/* ----------------------------------------------- Function Declarations */
void device_queue_pl_init ( void );

void device_queue_pl_shutdown ( void );

void device_queue_free_pl (DEVICE_HANDLE    * hndl);

void device_queue_full_pl(DEVICE_LINK_TYPE    link_type);

void device_queue_disconnect_on_full_pl
     (
         /* IN */ BT_DEVICE_ADDR    * bd_addr,
         /* IN */ DEVICE_LINK_TYPE    link_type
     );

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
void device_queue_cleanup_pl
     (
         /* IN */ UINT32 dq_valid_entries
     );
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#endif /* _H_DEVICE_QUEUE_PL_ */


