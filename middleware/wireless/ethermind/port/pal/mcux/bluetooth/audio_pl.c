
/**
 *  \file audio_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "audio_pl.h"
#include "a2dp_pl.h"

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */
void audio_init_pl (UCHAR role)
{
    a2dp_init_pl(role);
}

void audio_shutdown_pl (void)
{
    a2dp_shutdown_pl();
}

UCHAR * audio_alloc_buffer_pl(UCHAR type)
{
    return a2dp_alloc_buffer_pl(type);
}

API_RESULT audio_create_task_pl(BT_THREAD_START_ROUTINE routine)
{
    return a2dp_create_task_pl(routine);
}

API_RESULT audio_setup_pl
           (
               UCHAR ep,
               void (* ep_cb)(const UCHAR *data, UINT16 datalen),
               UINT16 sf,
               UCHAR bps,
               UCHAR nc,
               UINT16 size
           )
{
    a2dp_setup_pl(ep, ep_cb, sf, bps, nc, size);

    return API_SUCCESS;
}

API_RESULT audio_start_pl (UCHAR ep)
{
    a2dp_start_pl(ep);

    return API_SUCCESS;
}

API_RESULT audio_stop_pl (UCHAR ep)
{
    a2dp_stop_pl(ep);

    return API_SUCCESS;
}

void audio_write_pl (UCHAR ep, UCHAR * m_data, UINT16 m_datalen)
{
    a2dp_write_pl(ep, m_data, m_datalen);
}

