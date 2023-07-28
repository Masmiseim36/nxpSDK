
/**
 *  \file audio_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_AUDIO_PL_
#define _H_AUDIO_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */
#define AUDIO_SOURCE
#define AUDIO_SINK

/* Stream end points */
#define AUDIO_EP_SOURCE          0x00U
#define AUDIO_EP_SINK            0x01U

/* Buffer types for platform allocation */
#define AUDIO_SBC_BUFFER         0x00U
#define AUDIO_PCM_BUFFER         0x01U
#define AUDIO_SILENCE_BUFFER     0x02U

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifndef AUDIO_NO_DEBUG
#define AUDIO_PL_ERR(...)             BT_debug_error(0, __VA_ARGS__)
#else  /* AUDIO_NO_DEBUG */
#define AUDIO_PL_ERR                  BT_debug_null
#endif /* AUDIO_NO_DEBUG */

#ifdef AUDIO_DEBUG
    #define AUDIO_PL_TRC(...)         BT_debug_trace(0, __VA_ARGS__)
    #define AUDIO_PL_INF(...)         BT_debug_info(0, __VA_ARGS__)
#else /* AUDIO_DEBUG */
    #define AUDIO_PL_TRC              BT_debug_null
    #define AUDIO_PL_INF              BT_debug_null
#endif /* AUDIO_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
API_RESULT audio_create_task_pl(BT_THREAD_START_ROUTINE routine);
void audio_init_pl (UCHAR role);
void audio_shutdown_pl (void);
UCHAR * audio_alloc_buffer_pl(UCHAR type);
API_RESULT audio_setup_pl
           (
               UCHAR ep,
               void (* ep_cb)(const UCHAR * data, UINT16 datalen),
               UINT16 sf,
               UCHAR bps,
               UCHAR nc,
               UINT16 size
           );
API_RESULT audio_start_pl (UCHAR ep);
API_RESULT audio_stop_pl (UCHAR ep);

void audio_write_pl (UCHAR ep, UCHAR * m_data, UINT16 m_datalen);

#endif /* _H_AUDIO_PL_*/
