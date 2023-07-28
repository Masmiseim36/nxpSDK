
/**
 *  \file a2dp_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "a2dp_pl.h"

#ifdef LEAUDIO
#define A2DP_PL_SOURCE_FS_MEDIA
#endif /* LEAUDIO */

#ifdef A2DP_SOURCE

/* For A2DP Source Streaming */
#ifndef A2DP_PL_SOURCE_FS_MEDIA
#ifndef A2DP_PL_SOURCE_16KHz_SAMPLE
#include "a2dp_pl_media_48KHz.h"
#else /* A2DP_PL_SOURCE_16KHz_SAMPLE */
#include "a2dp_pl_media_16KHz.h"
#endif /* A2DP_PL_SOURCE_16KHz_SAMPLE */
#else /* A2DP_PL_SOURCE_FS_MEDIA */
#include "BT_fops.h"
#endif /* A2DP_PL_SOURCE_FS_MEDIA */
#endif /* A2DP_SOURCE */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#ifdef A2DP_SOURCE
UINT32 a2dp_src_sf;
UCHAR a2dp_src_bps;
UCHAR a2dp_src_nc;
UINT16 a2dp_src_size;
UINT16 a2dp_src_num_samples;
void (* a2dp_src_cb)(const UCHAR *data, UINT16 datalen);

UCHAR a2dp_src_playback;
BT_timer_handle a2dp_src_timer;
void a2dp_pl_start_playback_timer(void);

static int tone_index;
static INT32 a2dp_src_sent_ms;
static UINT32 a2dp_src_missed_count;

static BT_thread_mutex_type a2dp_src_media_mutex;
static BT_thread_cond_type a2dp_src_media_cond;

#ifdef A2DP_PL_SOURCE_FS_MEDIA
#define A2DP_SRC_MEDIA \
     BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "a2dp" BT_FOPS_PATH_SEP "sample.wav")

static BT_fops_file_handle a2dp_src_media_fd;
#endif /* A2DP_PL_SOURCE_FS_MEDIA */

#define A2DP_SRC_PERIOD_MS    10U
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
UINT32 a2dp_snk_sf;
UCHAR a2dp_snk_bps;
UCHAR a2dp_snk_nc;
UINT16 a2dp_snk_size;
void (* a2dp_snk_cb)(const UCHAR *data, UINT16 datlen);

AT_NONCACHEABLE_SECTION_INIT(UCHAR a2dp_sbc_buffer[JPL_SBC_FRAME_SIZE * JPL_SBC_NUM_FRAMES]);
AT_NONCACHEABLE_SECTION_INIT(UCHAR a2dp_pcm_buffer[JPL_PCM_BLOCK_SIZE * JPL_PCM_NUM_BLOCKS]);
AT_NONCACHEABLE_SECTION_INIT(UCHAR a2dp_silence_buffer[JPL_PCM_BLOCK_SIZE]);
#endif /* A2DP_SINK */

/* --------------------------------------------- Functions */
__attribute__((weak)) void a2dp_init_pl_ext (UCHAR role)
{
}

__attribute__((weak)) void a2dp_shutdown_pl_ext (void)
{
}

__attribute__((weak)) void a2dp_setup_pl_ext
     (
         UCHAR ep,
         void (* ep_cb)(const UCHAR *data, UINT16 datalen),
         UINT16 sf,
         UCHAR bps,
         UCHAR nc,
         UINT16 size
     )
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        a2dp_src_cb = ep_cb;
        a2dp_src_sf = sf;
        a2dp_src_bps = bps;
        a2dp_src_nc = nc;
        a2dp_src_size = size;
    }

    BT_IGNORE_UNUSED_PARAM(a2dp_src_bps);
    BT_IGNORE_UNUSED_PARAM(a2dp_src_nc);
    BT_IGNORE_UNUSED_PARAM(a2dp_src_size);
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        a2dp_snk_cb = ep_cb;
        a2dp_snk_sf = sf;
        a2dp_snk_bps = bps;
        a2dp_snk_nc = nc;
        a2dp_snk_size = size;
    }

    BT_IGNORE_UNUSED_PARAM(a2dp_snk_sf);
    BT_IGNORE_UNUSED_PARAM(a2dp_snk_bps);
    BT_IGNORE_UNUSED_PARAM(a2dp_snk_nc);
    BT_IGNORE_UNUSED_PARAM(a2dp_snk_size);
#endif /* A2DP_SINK */

    BT_IGNORE_UNUSED_PARAM(ep);
}

__attribute__((weak)) void a2dp_start_pl_ext (UCHAR ep)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        /* Start Audio Source */
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        /* Start Audio Player */
    }
#endif /* A2DP_SINK */
}

__attribute__((weak)) void a2dp_stop_pl_ext (UCHAR ep)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        /* Stop Audio Source */
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        /* Stop Audio Player */
    }
#endif /* A2DP_SINK */
}

__attribute__((weak)) void a2dp_write_pl_ext (UCHAR ep, UCHAR * m_data, UINT16 m_datalen)
{
#ifdef A2DP_SINK
    if (A2DP_EP_SINK != ep)
    {
        return;
    }

    /* Write to player */
#endif /* A2DP_SINK */
}

#ifdef A2DP_SOURCE
void a2dp_pl_produce_media(void)
{
    UCHAR * media;
    UINT16  medialen;

#ifndef A2DP_PL_SOURCE_FS_MEDIA

    /* Music Audio is Stereo */
    medialen = (a2dp_src_num_samples << a2dp_src_nc);

    /* For mono or dual configuration, skip alternative samples */
    if (1U == a2dp_src_nc)
    {
        UINT16 index;

        /* Allocate Memory */
        media = (UCHAR*)BT_alloc_mem(medialen);

        if (NULL == media)
        {
            A2DP_PL_ERR("Memory Allocation failed in Produce Media\n");
            return;
        }

        for (index = 0U; index < a2dp_src_num_samples; index++)
        {
            media[(2U * index)] = *((UCHAR*)beethoven + tone_index);
            media[(2U * index) + 1U] = *((UCHAR*)beethoven + tone_index + 1U);

            /* Update the tone index */
            tone_index += 4U;
            if (tone_index >= sizeof(beethoven))
            {
                tone_index = 0U;
            }
        }
    }
    else
    {
        if ((tone_index + (a2dp_src_num_samples << 2U)) > sizeof(beethoven))
        {
            media = (UCHAR*)BT_alloc_mem(medialen);
            if (NULL == media)
            {
                A2DP_PL_ERR("Memory Allocation failed in Produce Media\n");
                return;
            }
            memcpy(media, ((UCHAR*)beethoven + tone_index), sizeof(beethoven) - tone_index);
            memcpy(&media[sizeof(beethoven) - tone_index],
                   ((UCHAR*)beethoven),
                   ((a2dp_src_num_samples << 2U) - (sizeof(beethoven) - tone_index)));
            /* Update the tone index */
            tone_index = ((a2dp_src_num_samples << 2U) - (sizeof(beethoven) - tone_index));
        }
        else
        {
            media = ((UCHAR*)beethoven + tone_index);
            /* Update the tone index */
            tone_index += (a2dp_src_num_samples << 2U);
            if (tone_index >= sizeof(beethoven))
            {
                tone_index = 0U;
            }
        }
    }
#else /* A2DP_PL_SOURCE_FS_MEDIA */
    API_RESULT retval;
    UINT16 readlen;

    if (NULL == a2dp_src_media_fd)
    {
        A2DP_PL_ERR("Media not ready\n");
        return;
    }

    medialen = (a2dp_src_num_samples << a2dp_src_nc);
    media = BT_alloc_mem(medialen);

    if (NULL == media)
    {
        A2DP_PL_ERR("Memory Allocation failed in Produce Media\n");
        return;
    }

    retval = BT_fops_file_read(media, medialen, a2dp_src_media_fd, &readlen);
    if (API_SUCCESS != retval)
    {
        BT_fops_file_seek(a2dp_src_media_fd, 0U, BT_FOPS_SEEK_SET);
        retval = BT_fops_file_read(media, medialen, a2dp_src_media_fd, &readlen);
        if (API_SUCCESS != retval)
        {
            A2DP_PL_ERR("Media Read Failed\n");
            return;
        }
    }
    medialen = readlen;
#endif /* A2DP_PL_SOURCE_FS_MEDIA */

    /* Give data to callback */
    a2dp_src_cb(media, medialen);

#ifdef A2DP_PL_SOURCE_FS_MEDIA
    BT_free_mem(media);
#else /* A2DP_PL_SOURCE_FS_MEDIA */
    if (1U == a2dp_src_nc)
    {
        BT_free_mem(media);
    }
#endif /* A2DP_PL_SOURCE_FS_MEDIA */
}

void a2dp_pl_playback(void)
{
    INT32 now_ms, period_ms;

    /* Get the current time */
    now_ms = BT_get_time_ms();
    period_ms = A2DP_SRC_PERIOD_MS;

    /* Adjust the period */
    if (0U < a2dp_src_sent_ms)
    {
        period_ms = (now_ms > a2dp_src_sent_ms)?
            (now_ms - a2dp_src_sent_ms): A2DP_SRC_PERIOD_MS;
    }

    /* Get the number of samples */
    a2dp_src_num_samples = (UINT16)((period_ms * a2dp_src_sf) / 1000U);
    a2dp_src_missed_count += (period_ms * a2dp_src_sf) % 1000U;

    /* Raw adjust for the drift */
    while (a2dp_src_missed_count >= 1000U)
    {
        a2dp_src_num_samples++;
        a2dp_src_missed_count -= 1000U;
    }

    /* Update the sent timestamp */
    a2dp_src_sent_ms = now_ms;

    a2dp_pl_produce_media();
}

void a2dp_pl_playback_timeout_handler(void * args, UINT16 size)
{
    /* If stopped then return */
    if (BT_FALSE == a2dp_src_playback)
    {
        return;
    }

    /* Restart the timer */
    a2dp_src_timer = BT_TIMER_HANDLE_INIT_VAL;
    a2dp_pl_start_playback_timer();

    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal(&a2dp_src_media_cond);
}

void a2dp_pl_start_playback_timer(void)
{
    (BT_IGNORE_RETURN_VALUE) BT_start_timer
    (
        &a2dp_src_timer,
        (BT_TIMEOUT_MILLISEC | A2DP_SRC_PERIOD_MS),
        a2dp_pl_playback_timeout_handler,
        NULL, 0U
    );
}

DECL_STATIC BT_THREAD_RETURN_TYPE a2dp_src_media_task (BT_THREAD_ARGS args)
{
    BT_LOOP_FOREVER()
    {
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&a2dp_src_media_mutex);
        (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait(&a2dp_src_media_cond, &a2dp_src_media_mutex);
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&a2dp_src_media_mutex);

        if ((BT_TRUE != a2dp_src_playback) || (NULL == a2dp_src_cb))
        {
            continue;
        }

        a2dp_pl_playback();
    }
}
#endif /* A2DP_SOURCE */

API_RESULT a2dp_create_task_pl(BT_THREAD_START_ROUTINE routine)
{
#ifdef A2DP_SOURCE
    BT_thread_type tid;

    PL_TRC(
    "[A2DP] Creating Task ...\n");

    /* Create Write Task */
    if (0 != BT_thread_create(&tid,
        NULL,
        routine,
        NULL))
    {
        PL_ERR(
        "[A2DP] FATAL ERROR: FAILED to Create Task\n");

        return API_FAILURE;
    }
#endif /* A2DP_SOURCE */

    return API_SUCCESS;
}

void a2dp_init_default_src_pl (void)
{
#ifdef A2DP_SOURCE
    static UCHAR task_init = 0x00U;

    BT_thread_type tid;
    BT_thread_attr_type task_attr;

    /* Initialize the Write Task Attributes */
    task_attr.thread_name       = (DECL_CONST CHAR  *)"EtherMind PCM Task";
    task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    task_attr.thread_priority   = BT_TASK_PRIORITY;

    if (0x00U != task_init)
    {
        return;
    }

    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void) BT_thread_mutex_init (&a2dp_src_media_mutex, NULL);
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_init (&a2dp_src_media_cond, NULL);

    task_init = 0x01U;

    if (0 != BT_thread_create(&tid, &task_attr, a2dp_src_media_task, NULL))
    {
        A2DP_PL_ERR("Failed to create A2DP Media Task\n");
        return;
    }
#endif /* A2DP_SOURCE */
}

void a2dp_init_pl (UCHAR role)
{
#ifdef A2DP_SOURCE
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
#endif /* A2DP_SINK */
    a2dp_init_pl_ext(role);
}

void a2dp_shutdown_pl (void)
{
#ifdef A2DP_SOURCE
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
#endif /* A2DP_SINK */
    a2dp_shutdown_pl_ext();
}

UCHAR * a2dp_alloc_buffer_pl(UCHAR type)
{
#ifdef A2DP_SINK
    UCHAR * ptr;

    switch (type)
    {
    case A2DP_SBC_BUFFER:
        ptr = a2dp_sbc_buffer;
        break;

    case A2DP_PCM_BUFFER:
        ptr = a2dp_pcm_buffer;
        break;

    case A2DP_SILENCE_BUFFER:
        ptr = a2dp_silence_buffer;
        break;

    default:
        ptr = NULL;
    }

    return ptr;
#else /* A2DP_SINK */

    return NULL;
#endif /* A2DP_SINK */
}

void a2dp_setup_pl
     (
         UCHAR ep,
         void (* ep_cb)(const UCHAR *data, UINT16 datalen),
         UINT16 sf,
         UCHAR bps,
         UCHAR nc,
         UINT16 size
     )
{
    a2dp_setup_pl_ext(ep, ep_cb, sf, bps, nc, size);
}

void a2dp_start_pl (UCHAR ep)
{
#ifdef NVRAM_WORKAROUND
    /* Disable storage update */
    BT_storage_disable_store();
#endif /* NVRAM_WORKAROUND */

#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
#ifdef A2DP_PL_SOURCE_FS_MEDIA
        UCHAR wav_header[44U];
        API_RESULT retval;
        UINT16 headerlen;

        a2dp_src_media_fd = NULL;
        retval = BT_fops_file_open(A2DP_SRC_MEDIA, "rb", &a2dp_src_media_fd);

        if (API_SUCCESS != retval)
        {
            A2DP_PL_ERR("Failed to open media for playback\n");
            return;
        }

        /* Skip the initial 44 bytes of Wav header */
        BT_fops_file_read(wav_header, sizeof(wav_header), a2dp_src_media_fd, &headerlen);
#endif /* A2DP_PL_SOURCE_FS_MEDIA */

        /* Reset indices */
        tone_index = 0U;
        a2dp_src_sent_ms = 0U;
        a2dp_src_num_samples = 0U;
        a2dp_src_missed_count = 0U;

        /* Start Audio Source */
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        /* Start Audio Player */
    }
#endif /* A2DP_SINK */
    a2dp_start_pl_ext(ep);
}

void a2dp_stop_pl (UCHAR ep)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        /* Stop Audio Source */

#ifdef A2DP_PL_SOURCE_FS_MEDIA
        BT_fops_file_close(a2dp_src_media_fd);
        a2dp_src_media_fd = NULL;
#endif /* A2DP_PL_SOURCE_FS_MEDIA */
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        /* Stop Audio Player */
    }
#endif /* A2DP_SINK */
    a2dp_stop_pl_ext(ep);

#ifdef NVRAM_WORKAROUND
    /* Sync the storage */
    BT_storage_sync_db(STORAGE_TYPE_PERSISTENT);
#endif /* NVRAM_WORKAROUND */
}

void a2dp_write_pl (UCHAR ep, UCHAR * m_data, UINT16 m_datalen)
{
#ifdef A2DP_SINK
    if (A2DP_EP_SINK != ep)
    {
        return;
    }

    /* Write to player */
#endif /* A2DP_SINK */
    a2dp_write_pl_ext(ep, m_data, m_datalen);
}

