
/**
 *  \file hfp_ag_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_HFP_AG_PL_
#define _H_HFP_AG_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifndef HFP_AG_PL_NO_DEBUG
#define HFP_AG_PL_ERR(...)             BT_debug_error(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else  /* HFP_AG_PL_NO_DEBUG */
#define HFP_AG_PL_ERR                  BT_debug_null
#endif /* HFP_AG_PL_NO_DEBUG */

#ifdef HFP_AG_PL_DEBUG
    #define HFP_AG_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
    #define HFP_AG_PL_INF(...)         BT_debug_info (BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else /* HFP_AG_PL_DEBUG */
    #define HFP_AG_PL_TRC              BT_debug_null
    #define HFP_AG_PL_INF              BT_debug_null
#endif /* HFP_AG_PL_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void hfp_ag_init_pl
     (
         void (* read_cb)(UCHAR *rsp, UINT16 rsplen),
         void (* call_cb)(UCHAR status),
         UCHAR(* inbandring_status_cb)(void)
     );
void hfp_ag_start_pl (UINT16 sf);
void hfp_ag_stop_pl(void);
void hfp_ag_write_pl (UCHAR * cmd, UINT16 cmdlen);
void hfp_ag_access_pl(void);

#endif /* _H_HFP_AG_PL_*/
