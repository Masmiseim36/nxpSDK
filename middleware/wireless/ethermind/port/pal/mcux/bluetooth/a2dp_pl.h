
/**
 *  \file a2dp_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_A2DP_PL_
#define _H_A2DP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* --------------------------------------------- Global Definitions */
/* Enable 16KHz streaming configuration */
/* #define A2DP_PL_SOURCE_16KHz_SAMPLE */

/* Enable to play media from filesystem */
/* #define A2DP_PL_SOURCE_FS_MEDIA */

/* Stream end points */
#define A2DP_EP_SOURCE          0x00U
#define A2DP_EP_SINK            0x01U

/* Buffer types for platform allocation */
#define A2DP_SBC_BUFFER         0x00U
#define A2DP_PCM_BUFFER         0x01U
#define A2DP_SILENCE_BUFFER     0x02U

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifndef A2DP_PL_NO_DEBUG
#define A2DP_PL_ERR(...)             BT_debug_error(BT_MODULE_ID_A2DP_PL, __VA_ARGS__)
#else  /* A2DP_PL_NO_DEBUG */
#define A2DP_PL_ERR                  BT_debug_null
#endif /* A2DP_PL_NO_DEBUG */

#ifdef A2DP_PL_DEBUG
    #define A2DP_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_A2DP_PL, __VA_ARGS__)
    #define A2DP_PL_INF(...)         BT_debug_info(BT_MODULE_ID_A2DP_PL, __VA_ARGS__)
#else /* A2DP_PL_DEBUG */
    #define A2DP_PL_TRC              BT_debug_null
    #define A2DP_PL_INF              BT_debug_null
#endif /* A2DP_PL_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
API_RESULT a2dp_create_task_pl(BT_THREAD_START_ROUTINE routine);
void a2dp_init_pl (UCHAR role);
void a2dp_shutdown_pl (void);
UCHAR * a2dp_alloc_buffer_pl(UCHAR type);
void a2dp_setup_pl
     (
         UCHAR ep,
         void (* ep_cb)(const UCHAR *data, UINT16 datalen),
         UINT16 sf,
         UCHAR bps,
         UCHAR nc,
         UINT16 size
     );
void a2dp_start_pl (UCHAR ep);
void a2dp_stop_pl (UCHAR ep);
void a2dp_write_pl (UCHAR ep, UCHAR * m_data, UINT16 m_datalen);

void a2dp_init_pl_ext (UCHAR role);
void a2dp_shutdown_pl_ext (void);
void a2dp_setup_pl_ext
     (
         UCHAR ep,
         void (* ep_cb)(const UCHAR *data, UINT16 datalen),
         UINT16 sf,
         UCHAR bps,
         UCHAR nc,
         UINT16 size
     );
void a2dp_start_pl_ext (UCHAR ep);
void a2dp_stop_pl_ext (UCHAR ep);
void a2dp_write_pl_ext (UCHAR ep, UCHAR * m_data, UINT16 m_datalen);

#endif /* _H_A2DP_PL_*/
