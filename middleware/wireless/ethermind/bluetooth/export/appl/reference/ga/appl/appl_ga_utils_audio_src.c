/**
 *  \file appl_ga_utils_audio_src.c
 *
 *  Provides the GA Audio Data Path - Includes Audio Source Interactions.
 *  1. ISO Module.
 *  2. Audio PL Module.
 *  3. LC3 Module.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ga_utils_audio_src.h"
#ifdef AUDIO_SRC_PL_SUPPORT
#include "audio_pl.h"
#endif /* AUDIO_SRC_PL_SUPPORT */

#ifdef GA_BAP

/* --------------------------------------------- Global Definitions */

#ifdef AUDIO_SRC_ISO_SUPPORT
AUDIO_ISO_INFO audio_iso_src[AUDIO_SRC_ISO_MAX_ENTRIES];
#endif /* AUDIO_SRC_ISO_SUPPORT */

#ifdef AUDIO_SRC_PL_SUPPORT
AUDIO_PL_INFO audio_pl_src;

/* Queue: Synchronization for thread */
BT_DEFINE_MUTEX(audio_pl_src_th_mutex)
BT_DEFINE_COND(audio_pl_src_th_cond)

#define AUDIO_PL_SRC_WR_TH_INIT                 0x01U
#define AUDIO_PL_SRC_WR_TH_STOP                 0x02U
#define AUDIO_PL_SRC_WR_TH_PLAYING              0x03U

/**
 * Maximum Bytes to Encode will always be not more than MAX SDU.
 * Depends on Bitrate in BPS, Sampling Frequency, Frame Duration.
 * Setting a random value now.
 *
 * Currently assuming max of 2 channel audio possibility. This
 * needs to be dynamic based on the audio source, and the application
 * capability to process N channels.
 */
#define AUDIO_PL_MAX_BYTES_TO_LC3_ENCODE        (960U * 2U)

/**
 * Src PCM Queue Size.
 * Having 4 buffers of above size which should be able to
 * serve buffering with a max interval of 10ms
 */
#define AUDIO_PL_MAX_BUFFER_SIZE                (AUDIO_PL_MAX_BYTES_TO_LC3_ENCODE << 2U)


/* Write thread state */
static UINT8 audio_pl_src_wr_th_state;

/* Queue - Buffers and Rd/Wr indices */
static INT32 audio_pl_src_buffer_rd_ptr;
static INT32 audio_pl_src_buffer_wr_ptr;
static INT32 audio_pl_src_buffer_size;
static UCHAR audio_pl_src_buffer[AUDIO_PL_MAX_BUFFER_SIZE];

/* PCM Buffer for Encoding */
static UCHAR audio_pl_src_pcm_to_send[AUDIO_PL_MAX_BYTES_TO_LC3_ENCODE];
#endif /* AUDIO_SRC_PL_SUPPORT */

#ifdef AUDIO_SRC_LC3_SUPPORT
AUDIO_LC3_SRC_INFO audio_lc3_src;
#endif /* AUDIO_SRC_LC3_SUPPORT */

#ifdef SRC_DUMP_AUDIO_PREENCODED
FILE* src_dump_audio_pl_preencoded_fd;
FILE* src_dump_audio_preencoded_fd;
#endif /* SRC_DUMP_AUDIO_PREENCODED */

#ifdef SRC_DUMP_AUDIO_ENCODED
FILE* src_dump_audio_encoded_fd;
#endif /* SRC_DUMP_AUDIO_ENCODED */


INT32     pcm_in_16_from_8[1000] = { 0 };
UINT8     enc_interleaved_data[1000] = { 0 };

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Functions */
#ifdef AUDIO_SRC_PL_SUPPORT

/*
 * This callback is hit from Audio PL whenever there is audio generated for
 * the requested configuration during Audio PL Setup.
 */
static void audio_pl_src_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            );

/*
 * Task that is waiting for PCM data from Audio PL Layer in the
 * queue. This task picks the data as per number of bytes to be fed to
 * LC3 Encoder. The number of bytes is based on the codec configuration
 * to achieve the desired Bitrate. Once the Task is ready with Number of Bytes
 * to be fed to the LC3 Encoder module, call is made to perform LC3 Encoding.
 */
DECL_STATIC BT_THREAD_RETURN_TYPE audio_pl_src_task_for_handling_queue
                                  (
                                      BT_THREAD_ARGS args
                                  );

/*
 * This is the feeder to the Queue, this feeds the PCM Data from Audio PL
 * generated.
 */
GA_RESULT audio_pl_src_enqueue
          (
              /* IN */  const UCHAR * data,
              /* IN */  UINT16        datalen
          );

GA_RESULT audio_src_iso_write
          (
              UINT16   conn_handle,
              UCHAR    ts_flag,
              UINT32   ts,
              UINT16   seq_num,
              UCHAR  * sdu,
              UINT16   sdu_len
          );

GA_RESULT appl_ga_utils_is_audio_pl_generation_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          );

UINT16 audio_src_get_byte_size_to_encode_for_lc3
       (
           UINT16  sf_in_Hz,
           UINT16  fd_in_us,
           UINT8   channel_count
       );

#endif /* AUDIO_SRC_PL_SUPPORT */

#ifdef AUDIO_SRC_LC3_SUPPORT
GA_RESULT audio_src_lc3_encode_n_send
          (
              UCHAR* data,
              UINT16 datalen
          );

INT32 audio_src_get_size_of_lc3_encoded_bytes
      (
          INT32  bitrate_in_bps,
          INT32  sf_in_Hz,
          INT32  fd_as_per_lc3_format
      );

#endif /* AUDIO_SRC_LC3_SUPPORT */

void appl_ga_utils_audio_src_init(void)
{
    UINT8  index;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_init\n"
    );

#ifdef AUDIO_SRC_ISO_SUPPORT
    /* Initialize all fields to 0U */
    for (index = 0U; index < AUDIO_SRC_ISO_MAX_ENTRIES; index++)
    {
        GA_mem_set
        (
            &audio_iso_src[index],
            0U,
            (sizeof(AUDIO_ISO_INFO))
        );
        audio_iso_src[index].state = AUDIO_ISO_IDLE;
        audio_iso_src[index].conn_handle =
            AUDIO_CIS_CONN_HANDLE_INVALID;
    }
#endif /* AUDIO_SRC_ISO_SUPPORT */

#ifdef AUDIO_SRC_PL_SUPPORT
    /* Init all the Audio PL related info */
    (void)BT_thread_mutex_init(&audio_pl_src_th_mutex, NULL);
    (void)BT_thread_cond_init(&audio_pl_src_th_cond, NULL);

    /* Initialize the task variables and create the task */
    audio_pl_src_wr_th_state = AUDIO_PL_SRC_WR_TH_INIT;

    audio_pl_src_buffer_size = AUDIO_PL_MAX_BUFFER_SIZE;
    audio_pl_src_buffer_rd_ptr = 0U;
    audio_pl_src_buffer_wr_ptr = 0U;
    audio_pl_src.state = AUDIO_PL_IDLE;

    /* Initialize Audio PL */
    audio_init_pl(AUDIO_EP_SOURCE);

    /* Create the Write task which will trigger audio produce on audio start */
    audio_create_task_pl(audio_pl_src_task_for_handling_queue);
#endif /* AUDIO_SRC_PL_SUPPORT */

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_init\n"
    );
}
void appl_ga_utils_audio_src_deinit(void)
{
    APPL_DBG
    (
        "[APPL][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_deinit\n"
    );

#ifdef AUDIO_SRC_PL_SUPPORT
    /* Shutdown Audio PL */
    audio_shutdown_pl();
#endif /* AUDIO_SRC_PL_SUPPORT */

    APPL_DBG
    (
        "[APPL][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_deinit\n"
    );
    return;
}

#ifdef AUDIO_SRC_ISO_SUPPORT
GA_RESULT appl_ga_utils_audio_src_set_entry
          (
              UINT16                     conn_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          )
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_set_entry\n"
    );

    /*
     * Validate if there is an entry with this conn_handle already,
     * If yes, Update the other fields.
     * If no, Create a new entry.
     */
    if ((GA_SUCCESS == appl_ga_utils_audio_src_get_entry(conn_handle, &index)) ||
        (GA_SUCCESS == appl_ga_utils_audio_src_get_free_entry(&index)))
    {
        audio_iso_src[index].conn_handle = conn_handle;
        audio_iso_src[index].cs_conf = cs_conf;
        retval = GA_SUCCESS;
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SRC][ISO]: ISO Entry not Found !\n");
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_set_entry\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_audio_src_remove_entry(UINT16 conn_handle)
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_remove_entry\n"
    );

    if (GA_SUCCESS == appl_ga_utils_audio_src_get_entry(conn_handle, &index))
    {
        GA_mem_set
        (
            &audio_iso_src[index],
            0U,
            sizeof(AUDIO_ISO_INFO)
        );
        audio_iso_src[index].state = AUDIO_ISO_IDLE;
        audio_iso_src[index].conn_handle =
            AUDIO_CIS_CONN_HANDLE_INVALID;
        retval = GA_SUCCESS;
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SRC][ISO]: ISO Entry not Found !\n");
    }

    APPL_DBG
    (
        "[APPL][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_remove_entry\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_audio_src_get_free_entry(UINT8 * index)
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_get_free_entry\n"
    );

    for (i = 0U; i < AUDIO_SRC_ISO_MAX_ENTRIES; i++)
    {
        if ((AUDIO_ISO_IDLE == audio_iso_src[i].state) &&
            (AUDIO_CIS_CONN_HANDLE_INVALID == audio_iso_src[i].conn_handle))
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_get_free_entry\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_src_get_entry
          (
              UINT16   conn_handle,
              UINT8  * index
          )
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;
    i = 0U;

    APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_get_entry\n");

    for (i = 0U; i < AUDIO_SRC_ISO_MAX_ENTRIES; i++)
    {
        if (conn_handle == audio_iso_src[i].conn_handle)
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_get_entry\n");
    return retval;
}

GA_RESULT appl_ga_utils_audio_src_update_entry
          (
              UINT16 conn_handle,
              UINT8  state,
              UINT8  status,
              UINT8  streaming_state
          )
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_SUCCESS;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_update_entry\n"
    );

    if (GA_SUCCESS == appl_ga_utils_audio_src_get_entry(conn_handle, &index))
    {
        if ((GA_TRUE == streaming_state) || (GA_FALSE == streaming_state))
        {
            audio_iso_src[index].streaming_state = streaming_state;
        }

        if (0x00 == status)
        {
            /* ISO Setup is Complete */
            if ((AUDIO_ISO_DATA_SETUP == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state))
            {
                APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Setup Status: Complete\n");
                audio_iso_src[index].state = AUDIO_ISO_SETUP_COMPLETE;
            }
            else if ((AUDIO_ISO_DATA_REMOVE == state)
                     &&
                     (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state))
            {
                APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: Complete\n");
                audio_iso_src[index].state = AUDIO_ISO_IDLE;
            }
            else
            {
                /* Unknown State */
            }
        }
        else if (AUDIO_IGNORE_VALUE != status)
        {
            /* ISO Setup has Failed */
            if ((AUDIO_ISO_DATA_SETUP == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state))
            {
                APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Setup Status: Failed !\n");
                audio_iso_src[index].state = AUDIO_ISO_IDLE;
            }
            else if ((AUDIO_ISO_DATA_REMOVE == state)
                     &&
                     (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state))
            {
                APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: Failed !\n");
                audio_iso_src[index].state = AUDIO_ISO_SETUP_COMPLETE;
            }
            else
            {
                /* Unknown State */
            }
        }
        else
        {
            /* No action required */
        }
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SRC][ISO]: ISO Entry not Found !\n");
        retval = GA_FAILURE;
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_update_entry\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_src_le_setup_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          )
{
    GA_RESULT  retval;
    UINT8      index;
    UINT8      codec_id[5U] = { 0U };

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_le_setup_iso_data_path\n"
    );

    if (GA_SUCCESS != appl_ga_utils_audio_src_get_entry(conn_handle, &index))
    {
        APPL_DBG("[APPL][AUDIO_SRC][ISO]: ISO Entry not Found !\n");
        return retval;
    }

    if (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state)
    {
        /* Do not allow again since already in progress */
        APPL_DBG("ISO Data Path Setup Status: Already In-progress !\n");
    }
    else if (AUDIO_ISO_IDLE == audio_iso_src[index].state)
    {
        /* The ISO Data Path is not setup yet, Set it up */
        /*
         * NOTE: The if the codec in use resides in the Bluetooth Host of the
         * device using the LE Setup ISO Data Path command, the Unicast Client
         * and/or Unicast Server shall:
         * Write the LE Setup ISO Data Path command Codec_Configuration_Length
         * parameter with the value 0x00.
         * Write octet 0 (Coding_Format) of the LE Setup ISO Data Path command
         * Codec_ID parameter with the value 0x03 (Transparent).
         * Refer BAP_v1.0.1, Page 95.
         */
        codec_id[0U] = AUDIO_CODING_FORMAT_TRANSPARENT;

        if (GA_SUCCESS == BT_hci_le_setup_iso_data_path
                          (
                              audio_iso_src[index].conn_handle,
                              AUDIO_ISO_INPUT_H_TO_C,
                              AUDIO_DATA_PATH_ID_HCI,
                              &codec_id[0U],
                              AUDIO_ISO_CONTROLLER_DELAY,
                              0U,
                              NULL
                          ))
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Setup Status: In-progress\n");
            retval = GA_CONTINUE;
            audio_iso_src[index].state = AUDIO_ISO_UPDATE_IN_PROGRESS;
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Setup Status: Failed !\n");
        }
    }
    else if (AUDIO_ISO_SETUP_COMPLETE == audio_iso_src[index].state)
    {
        /* ISO Data Path is already ready */
        APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Setup Status: Already Complete\n");
        retval = GA_SUCCESS;
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_le_setup_iso_data_path\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_src_le_remove_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          )
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: >> appl_ga_utils_audio_src_le_remove_iso_data_path\n"
    );

    if (GA_SUCCESS != appl_ga_utils_audio_src_get_entry(conn_handle, &index))
    {
        APPL_DBG("[APPL][AUDIO_SRC][ISO]: ISO Entry not Found !\n");
        return retval;
    }

    if (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_src[index].state)
    {
        /* Do not allow now since already in progress */
        APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: Already In-progress !\n");
    }
    else if (AUDIO_ISO_IDLE == audio_iso_src[index].state)
    {
        /* The ISO Data Path is already removed */
        APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: Already Removed !\n");
        retval = GA_SUCCESS;
    }
    else if (AUDIO_ISO_SETUP_COMPLETE == audio_iso_src[index].state)
    {
        /* The ISO Data Path is setup, Remove it */
        if (GA_SUCCESS == BT_hci_le_remove_iso_data_path
                          (
                              conn_handle,
                              AUDIO_ISO_INPUT_DATA_PATH
                          ))
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: In-progress\n");
            retval = GA_CONTINUE;
            audio_iso_src[index].state = AUDIO_ISO_UPDATE_IN_PROGRESS;
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][ISO]: ISO Data Path Removal Status: Failed !\n");
        }
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SRC][ISO]: << appl_ga_utils_audio_src_le_remove_iso_data_path\n"
    );
    return retval;
}

/*
 * This function will just call the Data Write assuming the Channel Frames
 * based on Channel Count is taken care before calling this function.
 */
GA_RESULT audio_src_iso_write
          (
              UINT16   conn_handle,
              UCHAR    ts_flag,
              UINT32   ts,
              UINT16   seq_num,
              UCHAR  * sdu,
              UINT16   sdu_len
          )
{
    /*
     * NOTE:
     * No logs are added here since it may cause delay in Data Write which may
     * affect the Audio Quality.
     */
    return (BT_hci_iso_write(
               conn_handle,
               ts_flag,
               ts,
               seq_num,
               sdu,
               sdu_len
           ));
}

UINT8 appl_ga_utils_audio_src_is_remove_allowed(void)
{
    UINT8 index = 0U;
    UINT8 streaming_count = 0U;
    UINT8 retval;

    for (index = 0U; index < AUDIO_SRC_ISO_MAX_ENTRIES; index++)
    {
        if (GA_TRUE == audio_iso_src[index].streaming_state)
        {
            streaming_count++;
        }
    }

    /* If there is atleast one stream active, disallow */
    if (streaming_count >= 1U)
    {
        retval = GA_FALSE;
    }
    else
    {
        retval = GA_TRUE;
    }

    return retval;
}
#endif /* AUDIO_SRC_ISO_SUPPORT */

#ifdef AUDIO_SRC_PL_SUPPORT

GA_RESULT audio_pl_src_enqueue
          (
              /* IN */  const UCHAR * data,
              /* IN */  UINT16        datalen
          )
{
    INT32  n_free;
    UINT32 count;

    BT_thread_mutex_lock(&audio_pl_src_th_mutex);

    /*
     *  Get amount of free buffer space.
     */
    if (audio_pl_src_buffer_wr_ptr >= audio_pl_src_buffer_rd_ptr)
    {
        /*
         *    ----------DDDDDDDDDDDDDDD--------------X
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       rd_ptr         wr_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = (audio_pl_src_buffer_size - audio_pl_src_buffer_wr_ptr)
            + audio_pl_src_buffer_rd_ptr;
    }
    else
    {
        /*
         *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       wr_ptr         rd_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = audio_pl_src_buffer_rd_ptr - audio_pl_src_buffer_wr_ptr;
    }

    /*
     *  Do we have enough space to accomodate new data ?
     *  Buffer Empty Condition: when rd_ptr == wr_ptr
     *  Buffer Full  Condition: when diff(rd_ptr, wr_ptr) == 1
     */
    if ((n_free < 1) || (n_free < datalen))
    {
        /* Overflow ! */
        APPL_ERR ("+ %d %d + ", n_free, datalen);
        BT_thread_mutex_unlock(&audio_pl_src_th_mutex);
        return GA_FAILURE;
    }

#ifdef SRC_DUMP_AUDIO_PREENCODED
    if (NULL != src_dump_audio_pl_preencoded_fd)
    {
        fwrite(data, sizeof(UCHAR), datalen, src_dump_audio_pl_preencoded_fd);
    }
#endif /* SRC_DUMP_AUDIO_PREENCODED */

    /* Store new data into Buffer */
    for (count = 0U; count < datalen; count++)
    {
        audio_pl_src_buffer[audio_pl_src_buffer_wr_ptr] = data[count];
        audio_pl_src_buffer_wr_ptr++;

        if (audio_pl_src_buffer_wr_ptr == audio_pl_src_buffer_size)
        {
            audio_pl_src_buffer_wr_ptr = 0;
        }
    }

    if (AUDIO_PL_SRC_WR_TH_INIT == audio_pl_src_wr_th_state)
    {
        /* Signal the waiting thread */
        audio_pl_src_wr_th_state = AUDIO_PL_SRC_WR_TH_PLAYING;
        BT_thread_cond_signal(&audio_pl_src_th_cond);
    }

    BT_thread_mutex_unlock(&audio_pl_src_th_mutex);

    return GA_SUCCESS;
}

/*
 * This task is created by Audio PL, but this is used by us to handle the
 * Audio produced by the Audio PL when we are Source.
 */
DECL_STATIC BT_THREAD_RETURN_TYPE audio_pl_src_task_for_handling_queue
                                  (
                                      BT_THREAD_ARGS args
                                  )
{
    INT32  rd_ptr, index, remaining;
    UINT16 bytes_to_send, buf_index, encode_len;

    GA_IGNORE_UNUSED_PARAM(args);

    buf_index = 0U;

    BT_LOOP_FOREVER()
    {
        BT_thread_mutex_lock(&audio_pl_src_th_mutex);

        if (AUDIO_PL_SRC_WR_TH_INIT == audio_pl_src_wr_th_state)
        {
            /* Wait for data in buffer */
#ifndef HAVE_CONDWAIT_WO_MUTEX
            BT_thread_cond_wait(&audio_pl_src_th_cond, &audio_pl_src_th_mutex);
#else /* HAVE_CONDWAIT_WO_MUTEX */
            BT_thread_mutex_unlock(&audio_pl_src_th_mutex);
            BT_thread_cond_wait(&audio_pl_src_th_cond, &audio_pl_src_th_mutex);
            BT_thread_mutex_lock(&audio_pl_src_th_mutex);
#endif /* HAVE_CONDWAIT_WO_MUTEX */
        }
        else if (AUDIO_PL_SRC_WR_TH_STOP == audio_pl_src_wr_th_state)
        {
            /* Exit from task */
            BT_thread_mutex_unlock(&audio_pl_src_th_mutex);
            return BT_THREAD_RETURN_VAL_SUCCESS;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (audio_pl_src_buffer_wr_ptr >= audio_pl_src_buffer_rd_ptr)
        {
            /*
                *    ----------DDDDDDDDDDDDDDD--------------X
                *    ^         ^              ^             ^
                *    |         |              |             |
                *    0       rd_ptr         wr_ptr         max
                *
                *  Free Space => '-----------'
                */
            remaining = audio_pl_src_buffer_wr_ptr - audio_pl_src_buffer_rd_ptr;
        }
        else
        {
            /*
                *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
                *    ^         ^              ^             ^
                *    |         |              |             |
                *    0       wr_ptr         rd_ptr         max
                *
                *  Free Space => '-----------'
                */
            remaining = audio_pl_src_buffer_size -
                (audio_pl_src_buffer_rd_ptr - audio_pl_src_buffer_wr_ptr);
        }

        /* Do we really have anything to read ? */
        if (remaining < audio_pl_src.bytes_to_lc3_encode)
        {
#ifndef HAVE_CONDWAIT_WO_MUTEX
            /* Wait for data in buffer */
            audio_pl_src_wr_th_state = AUDIO_PL_SRC_WR_TH_INIT;
            BT_thread_cond_wait(&audio_pl_src_th_cond, &audio_pl_src_th_mutex);
            BT_thread_mutex_unlock(&audio_pl_src_th_mutex);
#else /* HAVE_CONDWAIT_WO_MUTEX */
            audio_pl_src_wr_th_state = AUDIO_PL_SRC_WR_TH_INIT;
            BT_thread_mutex_unlock(&audio_pl_src_th_mutex);
            BT_thread_cond_wait(&audio_pl_src_th_cond, &audio_pl_src_th_mutex);
#endif /* HAVE_CONDWAIT_WO_MUTEX */

            /* APPL_INF (bt_debug_fd, "remaining < 1\n"); */
            continue;
        }

        buf_index = 0U;

        rd_ptr = audio_pl_src_buffer_rd_ptr;

        bytes_to_send = (remaining >= audio_pl_src.bytes_to_lc3_encode) ?
            audio_pl_src.bytes_to_lc3_encode : (UINT16)remaining;

        for (index = 0; index < bytes_to_send; index++)
        {
            audio_pl_src_pcm_to_send[index] = audio_pl_src_buffer[rd_ptr];

            rd_ptr = rd_ptr + 1;
            if (rd_ptr == audio_pl_src_buffer_size)
            {
                rd_ptr = 0;
            }
        }

        /* Update the read pointer */
        audio_pl_src_buffer_rd_ptr = rd_ptr;

        BT_thread_mutex_unlock(&audio_pl_src_th_mutex);

        while (buf_index < bytes_to_send)
        {
            if ((bytes_to_send - buf_index) <= audio_pl_src.bytes_to_lc3_encode)
            {
                encode_len = bytes_to_send - buf_index;
            }
            else
            {
                encode_len = audio_pl_src.bytes_to_lc3_encode;
            }

#ifdef AUDIO_SRC_LC3_SUPPORT
            audio_src_lc3_encode_n_send
            (
                &audio_pl_src_pcm_to_send[buf_index],
                encode_len
            );
#endif /* AUDIO_SRC_LC3_SUPPORT */

            buf_index += encode_len;
        }
    }
}

/*
 * Callback that is triggered from Audio Pl callback.
 * Call the Queue Module to stagger data flow to achieve synchronization.
 */
static void audio_pl_src_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            )
{
    /* APPL_TRC(". "); */
    /* APPL_TRC("%lld ", timestamp); */
    /* APPL_TRC("%d ", p_datalen); */

    if (AUDIO_PL_STREAM_GENERATING == audio_pl_src.state)
    {
#ifdef SRC_DUMP_AUDIO_ENCODED
        if (NULL != src_dump_audio_encoded_fd)
        {
            fwrite(p_data, sizeof(UCHAR), p_datalen, src_dump_audio_encoded_fd);
        }
#endif /* SRC_DUMP_AUDIO_ENCODED */

#ifdef AUDIO_SRC_LC3_SUPPORT
        audio_pl_src_enqueue(p_data, p_datalen);
#endif /* AUDIO_SRC_LC3_SUPPORT */
    }
    else
    {
        APPL_TRC("ISO: Data generated at wrong ASE state\n");
    }
    return;
}

GA_RESULT appl_ga_utils_audio_src_setup_generator_pl
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps,
              UINT16                     size
          )
{
    GA_RESULT  retval;
    UINT32     sf_hz;
    UINT8      cc;
    UINT16     fd_in_us;

    /* Initialize */
    sf_hz = 0U;
    cc = 0U;

    /* Fetch Sampling Freq in Hz */
    sf_hz = appl_ga_utils_bap_fetch_sf(cs_conf.sf);

    /* Get the Frame Duration in us */
    fd_in_us = appl_ga_utils_get_fd_in_us(cs_conf.fd);

    /* Fetch Channel Count from Audio Channel Allocation */
    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    retval = audio_setup_pl
             (
                 AUDIO_EP_SOURCE,
                 audio_pl_src_callback,
                 (UINT16 )sf_hz,
                 bps,
                 cc,
                 size
             );

    if ((GA_SUCCESS == retval) && (AUDIO_PL_STREAM_GENERATING != audio_pl_src.state))
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Setup Status: Success\n");
        /*
         * Save/Update the Settings that the Audio PL is configured currently.
         */
        audio_pl_src.sf_hz = (UINT16 )sf_hz;
        audio_pl_src.bps = bps;
        audio_pl_src.cc = cc;
        audio_pl_src.size = size;
        audio_pl_src.bytes_to_lc3_encode = audio_src_get_byte_size_to_encode_for_lc3
                                           (
                                               (UINT16 )sf_hz,
                                               fd_in_us,
                                               cc
                                           );

        audio_pl_src.state = AUDIO_PL_SETUP_COMPLETE;
    }
    else
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Setup Status: Failed !\n");
    }

    return retval;
}

GA_RESULT appl_ga_utils_audio_src_start_generator_pl(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (AUDIO_PL_SETUP_COMPLETE == audio_pl_src.state)
    {
        retval = audio_start_pl(AUDIO_EP_SOURCE);

        if (GA_SUCCESS == retval)
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Start Process Status: Success\n");
            audio_pl_src.state = AUDIO_PL_STREAM_GENERATING;
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Start Process Status: Failed !\n");
        }
    }
    else
    {
        /* Already Audio Generation in Progress, Do nothing */
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Start Process Status: Already generating !\n");
    }

    return retval;
}

UINT8 appl_ga_utils_audio_src_is_generator_pl_running(void)
{
    UINT8 isRunning;

    /* Initialize */
    isRunning = GA_FALSE;

    if ((AUDIO_PL_SETUP_COMPLETE == audio_pl_src.state)
        || (AUDIO_PL_IDLE == audio_pl_src.state))
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Status: Already Stopped !\n");
    }
    else
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Status: Generating !\n");
        isRunning = GA_TRUE;
    }

    return isRunning;
}

GA_RESULT appl_ga_utils_audio_src_stop_generator_pl(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (GA_TRUE == appl_ga_utils_audio_src_is_remove_allowed())
    {
        retval = audio_stop_pl(AUDIO_EP_SOURCE);

        if (GA_SUCCESS == retval)
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Stop Process Status: Success\n");
            audio_pl_src.state = AUDIO_PL_SETUP_COMPLETE;
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Stop Process Status: Failed !\n");
        }
    }
    else
    {
        /* Already Audio Generation is Stopped, Do nothing */
        APPL_DBG("[APPL][GA][AUDIO_SRC][PL]: Audio PL Generator Stop Process Status: Already stopped !\n");
    }

    return retval;
}

UINT16 audio_src_get_byte_size_to_encode_for_lc3
       (
           UINT16  sf_in_Hz,
           UINT16  fd_in_us,
           UINT8   channel_count
       )
{
    UINT16 bytes_to_lc3_encode;

    /* Initialize */
    bytes_to_lc3_encode = 0U;

    /*
     * E.g.,
     * sf_in_Hz = 48000
     * fd_in_us = 10000 in micro seconds
     * channel_count = 1
     * No of samples per frame: 2
     * bytes_to_lc3_encode = (48000 * 10000 * 1 * 2)/(1000000) = 960
     * We are dividing by 10^6 as fd will be in micro seconds.
     */

    /* TODO: Why * 2? */
    bytes_to_lc3_encode = (UINT16)((sf_in_Hz * fd_in_us * channel_count * 2U)
                                   / (1000U * 1000U));

    return bytes_to_lc3_encode;
}

#endif /* AUDIO_SRC_PL_SUPPORT */

GA_RESULT appl_ga_utils_is_audio_pl_generation_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          )
{
    GA_RESULT retval;

#ifdef AUDIO_SRC_PL_SUPPORT
    UINT8 cc;

    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    /*
     * First Validate if the Audio PL is already in generation progress,
     * If yes, Validate and Allow.
     * Else. Allow immediately.
     */
    if (AUDIO_PL_STREAM_GENERATING == audio_pl_src.state)
    {
        /* TODO:
         * Validate if requested Configuration is a Subset of ongoing.
         */

        /*
         * Validate if the CC is less or same as already configured.
         * Also validate if the values of Audio Channel Allocation are the same
         * or is a subset.
         */
        if (cc > audio_pl_src.cc)
        {
            retval = GA_FAILURE;
        }

        /* TODO: Check if ACA is same or a subset */

        retval = GA_SUCCESS;
    }
    else
    {
        retval = GA_SUCCESS;
    }

#else /* AUDIO_SRC_PL_SUPPORT */
    retval = GA_SUCCESS;
#endif /* AUDIO_SRC_PL_SUPPORT */

    return retval;
}

GA_RESULT appl_ga_utils_audio_src_audio_lc3_enc_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          )
{
    GA_RESULT retval;

#ifdef AUDIO_SRC_LC3_SUPPORT
    UINT8 cc;

    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    if (AUDIO_SRC_LC3_CHANNELS_MAX < cc)
    {
        retval = GA_FAILURE;
    }

    /*
     * First Validate if the LC3 Encoder is already in generation progress,
     * If yes, Validate and Allow.
     * Else. Allow immediately.
     */
    if (AUDIO_LC3_IN_PROGRESS == audio_lc3_src.state)
    {
        /* TODO:
         * Validate if requested Configuration is a Subset of ongoing.
         */

        /*
         * Validate if the CC is less or same as already configured.
         * Also validate if the values of Audio Channel Allocation are the same
         * or is a subset.
         */
        if (cc > audio_lc3_src.cc)
        {
            retval = GA_FAILURE;
        }

        /* TODO: Check if ACA is a subset */

        /* TODO: Update this ISO Handle with the ACA and Priority */
        retval = GA_SUCCESS;
    }
    else
    {
        retval = GA_SUCCESS;
    }

#else /* AUDIO_SRC_LC3_SUPPORT */
    retval = GA_SUCCESS;
#endif /* AUDIO_SRC_LC3_SUPPORT */

    return retval;
}

#ifdef AUDIO_SRC_LC3_SUPPORT

GA_RESULT appl_ga_utils_audio_src_create_lc3_encoder
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8                      cs_conf_set_id,
              UCHAR                      bps
          )
{
    GA_RESULT  retval;
    INT32      lc3_retval;
    UINT8      index;
    INT32      lc3_encoded_bytes;
    INT32      size_of_lc3_enc_bytes;
    UINT16     fd_in_us;
    UINT16     fd_as_per_lc3_format;
    UINT32     sf_hz;
    UINT8      cc;
    UINT32     bitrate_in_bps;

    /* Initialize */
    retval = GA_SUCCESS;
    lc3_retval = 0U;
    index = 0U;
    lc3_encoded_bytes = 0;
    size_of_lc3_enc_bytes = 0;
    fd_in_us = 0U;
    sf_hz = 0U;
    cc = 0U;
    fd_as_per_lc3_format = 0U;
    bitrate_in_bps = 0U;

    /* Fetch Sampling Freq in Hz */
    sf_hz = appl_ga_utils_bap_fetch_sf(cs_conf.sf);

    /* Get the Frame Duration in us */
    fd_in_us = appl_ga_utils_get_fd_in_us(cs_conf.fd);

    /* Fetch Channel Count from Audio Channel Allocation */
    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    /* Fetch the Frame duration value as per LC3 format */
    GET_SUPP_FRAME_DURATION_FOR_LC3(fd_in_us, fd_as_per_lc3_format);

    /* Fetch Bitrate from cs_conf set ID */
    bitrate_in_bps = appl_ga_utils_bap_fetch_bitrate_from_qos_conf_set_id
                     (
                         cs_conf_set_id
                     );

    /*
     * By default, we assume that the LC3 Encoder is called only after
     * validating appl_ga_utils_audio_src_set_entry(), hence return GA_SUCCESS
     * by default.
     */

    if (AUDIO_LC3_IN_PROGRESS != audio_lc3_src.state)
    {
        size_of_lc3_enc_bytes = audio_src_get_size_of_lc3_encoded_bytes
                                (
                                    bitrate_in_bps,
                                    sf_hz,
                                    fd_as_per_lc3_format
                                );

        GA_mem_set
        (
            audio_lc3_src.ctx.src_pcm_buffer, 0, sizeof(audio_lc3_src.ctx.src_pcm_buffer)
        );
        GA_mem_set
        (
            audio_lc3_src.ctx.src_enc_buffer, 0U, sizeof(audio_lc3_src.ctx.src_enc_buffer)
        );

        GA_mem_set(audio_lc3_src.ctx.target_enc_bytes, 0x00, AUDIO_SRC_LC3_CHANNELS_MAX);

        for (index = 0U; index < cc; index++)
        {
            /*
             * Assign Appl Buffer for pcm_buf - Source -> PCM Input,
             *                        enc_buf - Resultant -> LC3 Enc Output
             */


            audio_lc3_src.ctx.src_pcm_buf[index] = &audio_lc3_src.ctx.src_pcm_buffer[index][0U];
            audio_lc3_src.ctx.src_enc_buf[index] = &audio_lc3_src.ctx.src_enc_buffer[index][0U];
            audio_lc3_src.ctx.target_enc_bytes[index] = size_of_lc3_enc_bytes;
        }

        /* Init LC3 Encode Module */
        lc3_retval = LC3_encoder_create
                     (
                         &audio_lc3_src.ctx.enc_ctx,
                         sf_hz,
                         bps,
                         cc,
                         fd_as_per_lc3_format,
                         audio_lc3_src.ctx.target_enc_bytes,
                         audio_lc3_src.ctx.enc_core_buffer,
                         audio_lc3_src.ctx.enc_work_buffer,
                         audio_lc3_src.ctx.src_pcm_buf,
                         audio_lc3_src.ctx.src_enc_buf
                     );

        if (LC3_ENCODER_SUCCESS == lc3_retval)
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Created\n");
            audio_lc3_src.sf_hz = (UINT16 )sf_hz;
            audio_lc3_src.bps = bps;
            audio_lc3_src.cc = cc;
            audio_lc3_src.fd_in_us = fd_in_us;
            audio_lc3_src.location = cs_conf.aca;
            audio_lc3_src.aca_info.cc = cc;
            appl_ga_utils_fetch_priority_from_aca
            (
                audio_lc3_src.location,
                &audio_lc3_src.aca_info
            );
            audio_lc3_src.state = AUDIO_LC3_CREATED;
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Could not create !\n");
            GA_mem_set
            (
                &audio_lc3_src.ctx.enc_ctx,
                0U,
                sizeof(LC3_ENCODER_CNTX)
            );
            retval = GA_FAILURE;
        }
    }

    return retval;
}

static void audio_src_lc3_interleave_samples(UINT8** in, UINT8* out, INT32 n, INT32 channels)
{
    UINT8 ch, i;
    for (ch = 0; ch < channels; ch++)
    {
        for (i = 0; i < n; i++)
        {
            out[(i * channels) + ch] = in[ch][i];
        }
    }
}

static void audio_src_lc3_deinterleave_samples(INT32* in, INT32** out, INT32 n, INT32 channels)
{
    INT32 ch, i;
    for (ch = 0; ch < channels; ch++)
    {
        for (i = 0; i < n; i++)
        {
            out[ch][i] = in[(i * channels) + ch];
        }
    }
}

GA_RESULT audio_src_lc3_encode_n_send
          (
              UCHAR  * pcm_data,
              UINT16   pcm_datalen
          )
{
    GA_RESULT retval;
    INT32     lc3_encoded_bytes;
    UINT16    index;
    UINT16    seq_num;
    UCHAR     ts_flag;
    UINT32    ts;
    UINT16    handle[3U] = { 0 }; /* TODO */
    UINT32    aca[3U] = { 0 }; /* TODO */
    UINT8     cc[3U] = { 0 }; /* TODO */
    UINT8     active_handle_count;

    /* Initialize */
    retval = GA_FAILURE;
    lc3_encoded_bytes = 0;
    index = 0U;
    seq_num = 0U;
    ts_flag = 0U;
    ts = 0x000000U;
    active_handle_count = 0U;

    /*
     * As per BAP Spec: 4.3.4 Example usage
     * Figure 4.3: Example LC3 Codec_Specific_Capabilities and Codec_Specific_Configuration parameter usage
     * The �Front Left� location has the lower bit index and is encoded and transported first.
     * The �Front Right� location has the higher bit index and is encoded and transported second.
     */

#ifdef SRC_DUMP_AUDIO_PREENCODED
    if (NULL != src_dump_audio_preencoded_fd)
    {
        fwrite(pcm_data, sizeof(UCHAR), pcm_datalen, src_dump_audio_preencoded_fd);
    }
#endif /* SRC_DUMP_AUDIO_PREENCODED */

    /*
     * Validate the ISO State and extract the ISO handle and ACA.
     */
    for (index = 0U; index < AUDIO_SRC_ISO_MAX_ENTRIES; index++)
    {
        if (GA_TRUE == audio_iso_src[index].streaming_state)
        {
            handle[active_handle_count] = audio_iso_src[index].conn_handle;
            aca[active_handle_count] = audio_iso_src[index].cs_conf.aca;
            cc[active_handle_count] = appl_ga_utils_fetch_sacc_from_aca
                                      (
                                          audio_iso_src[index].cs_conf.aca
                                      );
            /* TODO: Based on the ACA, Assign the Right Packet Index */
            active_handle_count++;
        }
    }

    memset(pcm_in_16_from_8, 0, sizeof(pcm_in_16_from_8));
    memset(enc_interleaved_data, 0, sizeof(enc_interleaved_data));


    /**
     * Copy the local buffer contents here to the LC3 Module buffer.
     * Convert UCHAR LE16 PCM Data to UINT16 LE16.
     * De-Interleave samples to fetch the samples per channel.
     *
     * UINT8 to UINT16:
     *  1. Fetch the pcm_data[0] and pcm_data[1]
     *  2. Pack it into UINT16 pcm_in_16_from_8[0].
     *  3. Run this loop for pcm_datalen/2 - As, we are working on 2 samples at a time.
     */
    for (index = 0; index < (pcm_datalen >> 1U); index++)
    {
        pcm_in_16_from_8[index] =
            (INT16)((pcm_data[2U * index + 1U] << 8U) | (pcm_data[2U * index]));
    }

    /*
     * De-Interleave Per Channel:
     *  The Length per channel will be pcm_datalen/4.
     *  1. pcm_in_16_from_8 is fed to deinterleave:
     *     Feeds the 1, 3, 5, 7... samples on Channel 1
     *     Feeds the 2, 4, 6, 8... samples on Channel 2
     */

    audio_src_lc3_deinterleave_samples
    (
        pcm_in_16_from_8,
        audio_lc3_src.ctx.src_pcm_buf,
        (pcm_datalen >> (audio_lc3_src.cc)),
        audio_lc3_src.cc
    );

    /* APPL_TRC("ISO: LC3 Module Bytes for encoding: %d \n", pcm_datalen); */

    if (AUDIO_LC3_CREATED == audio_lc3_src.state)
    {
        /* Update it for the first time here */
        audio_lc3_src.state = AUDIO_LC3_IN_PROGRESS;
    }

    if (AUDIO_LC3_IN_PROGRESS == audio_lc3_src.state)
    {
        lc3_encoded_bytes = LC3_encoder_process(&audio_lc3_src.ctx.enc_ctx);
        /* APPL_TRC("ISO: LC3 Module Encoded Bytes: %d \n", lc3_encoded_bytes); */

        if (lc3_encoded_bytes > 0)
        {
            seq_num++;
            for (index = 0U; index < active_handle_count; index++)
            {

#ifdef SRC_DUMP_AUDIO_ENCODED
                if (NULL != src_dump_audio_encoded_fd)
                {
                    fwrite
                    (
                        audio_lc3_src.ctx.src_enc_buf[0U],
                        sizeof(UCHAR),
                        lc3_encoded_bytes,
                        src_dump_audio_encoded_fd
                    );
                }
#endif /* SRC_DUMP_AUDIO_ENCODED */

                /*
                 * Based on the ACA and CC, Send the right data to the right
                 * ISO Handle.
                 */
                if (1U < audio_lc3_src.cc)
                {
                    /* LC3 Encoder has encoded for samples > 1 Channel */
                    if (cc[index] != 1U)
                    {
                        /* This is a Handle that is supporting more than 1 CC */
                        /* Interleave the samples */

                        audio_src_lc3_interleave_samples
                        (
                            audio_lc3_src.ctx.src_enc_buf,
                            &enc_interleaved_data[0U],
                            lc3_encoded_bytes,
                            audio_lc3_src.cc
                        );

                        retval = audio_src_iso_write
                                 (
                                     handle[index],
                                     ts_flag,
                                     ts,
                                     (UINT16)seq_num,
                                     &enc_interleaved_data[0U],
                                     (UINT16)lc3_encoded_bytes
                                 );

                        /* APPL_TRC("%lld ", timestamp); */
                        if (GA_SUCCESS != retval)
                        {
                            /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Failed !\n"); */
                            APPL_TRC("! ");
                            /* APPL_TRC("ISO Data Write: FAILED !!! Error code = %04X\n", retval); */
                        }
                        else
                        {
                            /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Success\n"); */
                            APPL_TRC("* ");
                        }

                    }
                    else
                    {
                        /* This is a Handle that is supporting 1 CC */
                        /* TODO: Based on the ACA, feed only that Channel Samples */

                        retval = audio_src_iso_write
                                 (
                                     handle[index],
                                     ts_flag,
                                     ts,
                                     (UINT16)seq_num,
                                     audio_lc3_src.ctx.src_enc_buf[0U],
                                     (UINT16)lc3_encoded_bytes
                                 );

                        /* APPL_TRC("%lld ", timestamp); */
                        if (GA_SUCCESS != retval)
                        {
                            /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Failed !\n"); */
                            APPL_TRC("! ");
                            /* APPL_TRC("ISO Data Write: FAILED !!! Error code = %04X\n", retval); */
                        }
                        else
                        {
                            /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Success\n"); */
                            APPL_TRC("* ");
                        }
                    }
                }
                else
                {
                    /* It is supporting 1 CC */
                    retval = audio_src_iso_write
                             (
                                 handle[index],
                                 ts_flag,
                                 ts,
                                 (UINT16)seq_num,
                                 audio_lc3_src.ctx.src_enc_buf[0U],
                                 (UINT16)lc3_encoded_bytes
                             );

                    /* APPL_TRC("%lld ", timestamp); */
                    if (GA_SUCCESS != retval)
                    {
                        /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Failed !\n"); */
                        APPL_TRC("! ");
                        /* APPL_TRC("ISO Data Write: FAILED !!! Error code = %04X\n", retval); */
                    }
                    else
                    {
                        /* APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Process Start Status: Success\n"); */
                        APPL_TRC("* ");
                    }
                }
            }
        }
    }

    return GA_SUCCESS;
}

UINT8 appl_ga_utils_audio_src_is_lc3_encoder_running(void)
{
    UINT8 isRunning;

    /* Initialize */
    isRunning = GA_FALSE;

    if (AUDIO_LC3_IDLE == audio_lc3_src.state)
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Already Deleted !\n");
    }
    else
    {
        APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: In Progress !\n");
        isRunning = GA_TRUE;
    }

    return isRunning;
}

GA_RESULT appl_ga_utils_audio_src_delete_lc3_encoder(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (AUDIO_LC3_IDLE != audio_lc3_src.state)
    {
        if (GA_TRUE == appl_ga_utils_audio_src_is_remove_allowed())
        {
            LC3_encoder_delete(&audio_lc3_src.ctx.enc_ctx);
            GA_mem_set
            (
                &audio_lc3_src.ctx.enc_ctx,
                0U,
                sizeof(LC3_ENCODER_CNTX)
            );
            audio_lc3_src.sf_hz = 0U;
            audio_lc3_src.bps = 0U;
            audio_lc3_src.cc = 0U;
            audio_lc3_src.fd_in_us = 0U;
            audio_lc3_src.state = AUDIO_LC3_IDLE;

            retval = GA_SUCCESS;
            APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Deleted\n");
        }
        else
        {
            APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Could not delete !\n");
        }
    }
    else
    {
        retval = GA_SUCCESS;
        APPL_DBG("[APPL][GA][AUDIO_SRC][LC3]: LC3 Encoder Setup Status: Already Deleted !\n");
    }

    return retval;
}

INT32 audio_src_get_size_of_lc3_encoded_bytes
      (
          INT32  bitrate_in_bps,
          INT32  sf_in_Hz,
          INT32  fd_as_per_lc3_format
      )
{
    INT32 lc3_encoded_bytes;
    /*
     * E.g.,
     * bitrate_in_bps = 80000
     * sf_in_Hz = 48000
     * fd_as_per_lc3_format = 100
     * Result: 100Bytes.
     */
    lc3_encoded_bytes = LC3_encoder_convert_bitrate_to_bytes
                        (
                            bitrate_in_bps,
                            sf_in_Hz,
                            fd_as_per_lc3_format
                        );

    return lc3_encoded_bytes;
}

#endif /* AUDIO_SRC_LC3_SUPPORT */

GA_RESULT appl_ga_utils_audio_src_config_audio_dump_files(UINT8 op)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

#ifdef SRC_DUMP_AUDIO_PREENCODED
    /* Enable(Open) the file */
    if (1U == op)
    {
        src_dump_audio_pl_preencoded_fd = fopen("src_dump_audio_pl_preencoded", "wb+");
        if (NULL != src_dump_audio_pl_preencoded_fd)
        {
            APPL_DBG("Source Audio_PL Dump created successfully\n");
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_DBG("Could not create Source Audio_PL Dump\n");
        }

        src_dump_audio_preencoded_fd = fopen("src_dump_audio_preencoded", "wb+");
        if (NULL != src_dump_audio_preencoded_fd)
        {
            APPL_DBG("Source Audio Pre-Encoded Dump created successfully\n");
        }
        else
        {
            APPL_DBG("Could not create Source Audio Pre-Encoded Dump\n");
            retval = GA_FAILURE;
        }
    }
    /* Disable(Close) the file */
    else if (2U == op)
    {
        if(NULL != src_dump_audio_pl_preencoded_fd)
        {
            fclose(src_dump_audio_pl_preencoded_fd);
            src_dump_audio_pl_preencoded_fd = NULL;
            APPL_DBG("Source Audio_PL Dump closed successfully\n");
        }

        if(NULL != src_dump_audio_preencoded_fd)
        {
            fclose(src_dump_audio_preencoded_fd);
            src_dump_audio_preencoded_fd = NULL;
            retval = GA_SUCCESS;
            APPL_DBG("Source Audio Pre-Encoded Dump closed successfully\n");
        }
    }
    else
    {
        APPL_DBG("Invalid Operation\n");
    }
#else /* SRC_DUMP_AUDIO_PREENCODED */
    APPL_ERR("SRC_DUMP_AUDIO_PREENCODED is not enabled !\n");
#endif /* SRC_DUMP_AUDIO_PREENCODED */

#ifdef SRC_DUMP_AUDIO_ENCODED
    /* Enable(Open) the file */
    if (1U == op)
    {
        src_dump_audio_encoded_fd = fopen("src_dump_audio_encoded", "wb+");
        if (NULL != src_dump_audio_encoded_fd)
        {
            APPL_DBG("Source Audio Encoded Dump created successfully\n");
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_DBG("Could not create Source Audio Encoded Dump\n");
        }
    }
    /* Disable(Close) the file */
    else if (2U == op)
    {
        if(NULL != src_dump_audio_encoded_fd)
        {
            fclose(src_dump_audio_encoded_fd);
            src_dump_audio_encoded_fd = NULL;
            retval = GA_SUCCESS;
            APPL_DBG("Source Audio Encoded Dump closed successfully\n");
        }
    }
    else
    {
        APPL_DBG("Invalid Operation\n");
    }
#else /* SRC_DUMP_AUDIO_ENCODED */
    APPL_ERR("SRC_DUMP_AUDIO_ENCODED is not enabled !\n");
#endif /* SRC_DUMP_AUDIO_ENCODED */

    return retval;
}

#endif /* GA_BAP */
