/**
 *  \file appl_ga_utils_audio_snk.c
 *
 *  Provides the GA Audio Data Path - Includes Audio Sink Interactions.
 *  1. ISO Module.
 *  2. Audio PL Module.
 *  3. LC3 Module.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ga_utils_audio_snk.h"
#ifdef AUDIO_SNK_PL_SUPPORT
#include "audio_pl.h"
#endif /* AUDIO_SNK_PL_SUPPORT */

#ifdef GA_BAP

/* --------------------------------------------- Global Definitions */

#ifdef AUDIO_SNK_ISO_SUPPORT
AUDIO_ISO_INFO audio_iso_snk[AUDIO_SNK_ISO_MAX_ENTRIES];
#endif /* AUDIO_SNK_ISO_SUPPORT */

#ifdef AUDIO_SNK_PL_SUPPORT

AUDIO_PL_INFO audio_pl_snk;
#endif /* AUDIO_SNK_PL_SUPPORT */

#ifdef AUDIO_SNK_LC3_SUPPORT

#define AUDIO_SNK_LC3_INPUT_FRAME_SIZE_MAX LC3_INPUT_FRAME_SIZE_MAX
#define LC3_SILENCE_BUFFER_COUNT 4U

AUDIO_LC3_SNK_INFO audio_lc3_snk;
#endif /* AUDIO_SNK_LC3_SUPPORT */

#ifdef SNK_DUMP_AUDIO_PREDECODED
FILE* snk_dump_audio_predecoded_fd;
#endif /* SRC_DUMP_AUDIO_PREDECODED */

#ifdef SNK_DUMP_AUDIO_DECODED
FILE* snk_dump_audio_decoded_fd;
#endif /* SNK_DUMP_AUDIO_DECODED */
/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Functions */
#ifdef AUDIO_SNK_PL_SUPPORT
/* This function is empty, No action required */
static void audio_pl_snk_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            );
#endif /* AUDIO_SNK_PL_SUPPORT */

#ifdef AUDIO_SNK_LC3_SUPPORT
GA_RESULT audio_snk_lc3_decode_n_play
          (
              UCHAR* data,
              UINT16 datalen
          );

void audio_snk_write_pl(UCHAR * data, UINT16 datalen);
#endif /* AUDIO_SNK_LC3_SUPPORT */

void appl_ga_utils_audio_snk_init(void)
{
    UINT8  index;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_init\n"
    );

#ifdef AUDIO_SNK_ISO_SUPPORT
    /* Initialize all fields to 0U */
    for (index = 0U; index < AUDIO_SNK_ISO_MAX_ENTRIES; index++)
    {
        GA_mem_set
        (
            &audio_iso_snk[index],
            0U,
            (sizeof(AUDIO_ISO_INFO))
        );
        audio_iso_snk[index].state = AUDIO_ISO_IDLE;
        audio_iso_snk[index].conn_handle =
            AUDIO_CIS_CONN_HANDLE_INVALID;
    }

    (GA_IGNORE_RETURN_VALUE)appl_ga_utils_audio_snk_register_iso_data_handler();
#endif /* AUDIO_SNK_ISO_SUPPORT */

#ifdef AUDIO_SNK_PL_SUPPORT
    /* Initialize Audio PL */
    audio_init_pl(AUDIO_EP_SINK);

    audio_pl_snk.state = AUDIO_PL_IDLE;
#endif /* AUDIO_SNK_PL_SUPPORT */

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_init\n"
    );
    return;
}

void appl_ga_utils_audio_snk_deinit(void)
{
    APPL_DBG
    (
        "[APPL][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_deinit\n"
    );

#ifdef AUDIO_SNK_PL_SUPPORT
    /* Shutdown Audio PL */
    audio_shutdown_pl();
#endif /* AUDIO_SNK_PL_SUPPORT */

    APPL_DBG
    (
        "[APPL][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_deinit\n"
    );
    return;
}

#ifdef AUDIO_SNK_ISO_SUPPORT
GA_RESULT appl_ga_utils_audio_snk_set_entry
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
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_set_entry\n"
    );

    /* Validate if there is an entry with this conn_handle already,
     * If yes, Update the other fields.
     * If no, Create a new entry.
     */
    if ((GA_SUCCESS == appl_ga_utils_audio_snk_get_entry(conn_handle, &index)) ||
        (GA_SUCCESS == appl_ga_utils_audio_snk_get_free_entry(&index)))
    {
        audio_iso_snk[index].conn_handle = conn_handle;
        audio_iso_snk[index].cs_conf = cs_conf;
        retval = GA_SUCCESS;
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Entry not Found !\n");
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_set_entry\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_remove_entry(UINT16 conn_handle)
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_remove_entry\n"
    );

    if (GA_SUCCESS == appl_ga_utils_audio_snk_get_entry(conn_handle, &index))
    {
        GA_mem_set
        (
            &audio_iso_snk[index],
            0U,
            sizeof(AUDIO_ISO_INFO)
        );
        audio_iso_snk[index].state = AUDIO_ISO_IDLE;
        audio_iso_snk[index].conn_handle =
            AUDIO_CIS_CONN_HANDLE_INVALID;
        retval = GA_SUCCESS;
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Entry not Found !\n");
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_remove_entry\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_get_free_entry(UINT8 * index)
{
    GA_RESULT  retval;
    UINT8      i;

    /* Initialize */
    retval = GA_FAILURE;

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_get_free_entry\n"
    );

    for (i = 0U; i < AUDIO_SNK_ISO_MAX_ENTRIES; i++)
    {
        if ((AUDIO_ISO_IDLE == audio_iso_snk[i].state) &&
            (AUDIO_CIS_CONN_HANDLE_INVALID == audio_iso_snk[i].conn_handle))
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_get_free_entry\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_get_entry
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

    APPL_DBG("[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_get_entry\n");

    for (i = 0U; i < AUDIO_SNK_ISO_MAX_ENTRIES; i++)
    {
        if (conn_handle == audio_iso_snk[i].conn_handle)
        {
            *index = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    APPL_DBG("[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_get_entry\n");
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_update_entry
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
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_update_entry\n"
    );

    if (GA_SUCCESS == appl_ga_utils_audio_snk_get_entry(conn_handle, &index))
    {
        if ((GA_TRUE == streaming_state) || (GA_FALSE == streaming_state))
        {
            audio_iso_snk[index].streaming_state = streaming_state;
        }

        if (0x00 == status)
        {
            /* ISO Setup is complete */
            if ((AUDIO_ISO_DATA_SETUP == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state))
            {
                APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Setup Status: Complete\n");
                audio_iso_snk[index].state = AUDIO_ISO_SETUP_COMPLETE;
            }
            else if ((AUDIO_ISO_DATA_REMOVE == state)
                     &&
                     (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state))
            {
                APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: Complete\n");
                audio_iso_snk[index].state = AUDIO_ISO_IDLE;
            }
            else
            {
                /* Unknown State */
            }
        }
        else if (AUDIO_IGNORE_VALUE != status)
        {
            /* ISO Setup Failed */
            if ((AUDIO_ISO_DATA_SETUP == state)
                &&
                (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state))
            {
                APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Setup Status: Failed !\n");
                audio_iso_snk[index].state = AUDIO_ISO_IDLE;
            }
            else if ((AUDIO_ISO_DATA_REMOVE == state)
                     &&
                     (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state))
            {
                APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: Failed !\n");
                audio_iso_snk[index].state = AUDIO_ISO_SETUP_COMPLETE;
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
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Entry not Found !\n");
        retval = GA_FAILURE;
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_update_entry\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_le_setup_iso_data_path
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
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_le_setup_iso_data_path\n"
    );

    if (GA_SUCCESS != appl_ga_utils_audio_snk_get_entry(conn_handle, &index))
    {
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Entry not Found !\n");
        return retval;
    }

    if (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state)
    {
        /* Do not allow again since already in progress */
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Setup Status: Already In-progress !\n");
    }
    else if (AUDIO_ISO_IDLE == audio_iso_snk[index].state)
    {
        /* The ISO Data Path is not setup yet, Set it up */

        /*
         * NOTE: The if the codec in use resides in the Bluetooth Host of the
         * device using the LE Setup ISO Data Path command, the Unicast Client
         * and/or Unicast Server shall:
         * Write the LE Setup ISO Data Path command Codec_Configuration_Length
         * parameter with the value 0x00.
         * Write octet 0 (Coding_Format) of the LE Setup ISO Data Path command
         * Codec_ID parameter with the value 0x03 (Transparent)
         */
        codec_id[0U] = AUDIO_CODING_FORMAT_TRANSPARENT;

        if (GA_SUCCESS == BT_hci_le_setup_iso_data_path
                          (
                              audio_iso_snk[index].conn_handle,
                              AUDIO_ISO_OUTPUT_C_TO_H,
                              AUDIO_DATA_PATH_ID_HCI,
                              &codec_id[0U],
                              AUDIO_ISO_CONTROLLER_DELAY,
                              0U,
                              NULL
                          ))
        {
            APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Setup Status: In-progress\n");
            retval = GA_CONTINUE;
            audio_iso_snk[index].state = AUDIO_ISO_UPDATE_IN_PROGRESS;
        }
    }
    else if (AUDIO_ISO_SETUP_COMPLETE == audio_iso_snk[index].state)
    {
        /* ISO Data Path is already ready */
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Setup Status: Already Complete\n");
        retval = GA_SUCCESS;
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_le_setup_iso_data_path\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_le_remove_iso_data_path
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
        "[APPL][GA][AUDIO_SNK][ISO]: >> appl_ga_utils_audio_snk_le_remove_iso_data_path\n"
    );

    if (GA_SUCCESS != appl_ga_utils_audio_snk_get_entry(conn_handle, &index))
    {
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Entry not Found !\n");
        return retval;
    }

    if (AUDIO_ISO_UPDATE_IN_PROGRESS == audio_iso_snk[index].state)
    {
        /* Do not allow now since already in progress */
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: Already In-progress !\n");
    }
    else if (AUDIO_ISO_IDLE == audio_iso_snk[index].state)
    {
        /* The ISO Data Path is already removed */
        APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: Already Removed !\n");
        retval = GA_SUCCESS;
    }
    else if (AUDIO_ISO_SETUP_COMPLETE == audio_iso_snk[index].state)
    {
        /* The ISO Data Path is setup, Remove it */
        if (GA_SUCCESS == BT_hci_le_remove_iso_data_path
                          (
                              conn_handle,
                              AUDIO_ISO_OUTPUT_DATA_PATH
                          ))
        {
            APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: In-progress\n");
            retval = GA_CONTINUE;
            audio_iso_snk[index].state = AUDIO_ISO_UPDATE_IN_PROGRESS;
        }
        else
        {
            APPL_DBG("[APPL][AUDIO_SNK][ISO]: ISO Data Path Removal Status: Failed !\n");
        }
    }

    APPL_DBG
    (
        "[APPL][GA][AUDIO_SNK][ISO]: << appl_ga_utils_audio_snk_le_remove_iso_data_path\n"
    );
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_register_iso_data_handler(void)
{
    GA_RESULT  retval;
    UINT8      index;

    /* Initialize */
    retval = GA_FAILURE;
    index = 0U;

    APPL_DBG
    (
        "[APPL][AUDIO_SNK][ISO]: "
        ">> appl_ga_utils_audio_snk_register_iso_data_handler\n"
    );

    retval = BT_hci_register_iso_data_handler
             (
                 &appl_ga_utils_audio_snk_hci_iso_data_ind_cb
             );

    if (GA_SUCCESS != retval)
    {
        APPL_DBG
        (
            "[APPL][GA][AUDIO_SNK][ISO]: "
            "Register ISO Data Handler Failed 0x%04X !\n",
            retval
        );
    }
    else
    {
        APPL_DBG
        (
            "[APPL][AUDIO_SNK][ISO]: "
            "Register ISO Data Handler Success 0x%04X !\n",
            retval
        );
    }

    APPL_DBG
    (
        "[APPL][AUDIO_SNK][ISO]: "
        "<< appl_ga_utils_audio_snk_register_iso_data_handler\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_hci_iso_data_ind_cb
          (
              UCHAR  * header,
              UCHAR  * data,
              UINT16  datalen
          )
{
    GA_RESULT retval;

    /* TODO: Extract header if required with the ISO utility macros */
    GA_IGNORE_UNUSED_PARAM(header);

#ifndef AUDIO_SNK_PL_SUPPORT
    GA_IGNORE_UNUSED_PARAM(data);
    GA_IGNORE_UNUSED_PARAM(datalen);
#endif /* AUDIO_SNK_PL_SUPPORT */

    retval = GA_FAILURE;

    /* APPL_TRC("ISO: LC3 Encoded Data : %d\n", datalen); */
    /* APPL_TRC("."); */

    if (AUDIO_PL_STREAM_PLAYBACK == audio_pl_snk.state)
    {
#ifdef AUDIO_SNK_LC3_SUPPORT
        if ((0U < datalen) && (NULL != data))
        {
            /* Decode and send the rx packet */
            retval = audio_snk_lc3_decode_n_play(data, datalen);
        }
#endif /* AUDIO_SNK_LC3_SUPPORT */
    }

    return retval;
}
#endif /* AUDIO_SNK_ISO_SUPPORT */

#ifdef AUDIO_SNK_PL_SUPPORT

void audio_snk_write_pl(UCHAR * data, UINT16 datalen)
{
    audio_write_pl
    (
        AUDIO_EP_SINK,
        data,
        datalen
    );

    return;
}

static void audio_pl_snk_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            )
{
    /* No action required */
    GA_IGNORE_UNUSED_PARAM(p_data);
    GA_IGNORE_UNUSED_PARAM(p_datalen);

    return;
}

GA_RESULT appl_ga_utils_audio_snk_setup_playback_pl
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps,
              UINT16                     size
          )
{
    GA_RESULT  retval;
    UINT32     sf_hz;
    UINT8      cc;

    /* Initialize */
    sf_hz = 0U;
    cc = 0U;

    /* Fetch Sampling Freq in Hz */
    sf_hz = appl_ga_utils_bap_fetch_sf(cs_conf.sf);

    /* Fetch Channel Count from Audio Channel Allocation */
    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);
    retval = audio_setup_pl
             (
                 AUDIO_EP_SINK,
                 audio_pl_snk_callback,
                 (UINT16 )sf_hz,
                 bps,
                 cc,
                 size
             );

    if (GA_SUCCESS == retval)
    {
        APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Setup Status: Success\n");
        /*
         * Save/Update the Settings that the Audio PL is configured currently.
         */
        audio_pl_snk.sf_hz = (UINT16 )sf_hz;
        audio_pl_snk.bps = bps;
        audio_pl_snk.cc = cc;
        audio_pl_snk.size = size;
        audio_pl_snk.state = AUDIO_PL_SETUP_COMPLETE;
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Setup Status: Failed !\n");
    }
    return retval;
}

GA_RESULT appl_ga_utils_audio_snk_start_playback_pl(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (AUDIO_PL_SETUP_COMPLETE == audio_pl_snk.state)
    {
        retval = audio_start_pl(AUDIO_EP_SINK);

        if (GA_SUCCESS == retval)
        {
            APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Start Process Status: Success\n");
            audio_pl_snk.state = AUDIO_PL_STREAM_PLAYBACK;
        }
        else
        {
            APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Start Process Status: Failed !\n");
        }
    }
    else
    {
        /* Already Audio Playback in Progress, Do nothing */
        APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Start Process Status: Already playing !\n");
    }
    return retval;
}

UINT8 appl_ga_utils_audio_snk_is_playback_pl_running(void)
{
    UINT8 isRunning;

    /* Initialize */
    isRunning = GA_FALSE;

    if ((AUDIO_PL_SETUP_COMPLETE == audio_pl_snk.state)
        || (AUDIO_PL_IDLE == audio_pl_snk.state))
    {
        APPL_DBG("[APPL][GA][AUDIO_SNK][PL]: Audio PL Playback Status: Already Stopped !\n");
    }
    else
    {
        APPL_DBG("[APPL][GA][AUDIO_SNK][PL]: Audio PL Playback Status: Playing !\n");
        isRunning = GA_TRUE;
    }

    return isRunning;
}

GA_RESULT appl_ga_utils_audio_snk_stop_playback_pl(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (AUDIO_PL_STREAM_PLAYBACK == audio_pl_snk.state)
    {
        retval = audio_stop_pl(AUDIO_EP_SINK);

        if (GA_SUCCESS == retval)
        {
            APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Stop Process Status: Success\n");
            audio_pl_snk.state = AUDIO_PL_SETUP_COMPLETE;
        }
        else
        {
            APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Stop Process Status: Failed !\n");
        }
    }
    else
    {
        /* Already Audio Playback is Stopped, Do nothing */
        APPL_DBG("[APPL][AUDIO_SNK][PL]: Audio PL Playback Stop Process Status: Already stopped !\n");
    }

    return retval;
}

#endif /* AUDIO_SNK_PL_SUPPORT */

GA_RESULT appl_ga_utils_audio_snk_audio_lc3_dec_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          )
{
#ifdef AUDIO_SNK_LC3_SUPPORT

    GA_RESULT retval;
    UINT8 cc;

    /* Initialize */
    retval = GA_FAILURE;

    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    if (AUDIO_SNK_LC3_CHANNELS_MAX < cc)
    {
        /* Not Allowed */
    }

    /*
     * First Validate if the LC3 Decoder is already in progress
     */
    if (AUDIO_LC3_IN_PROGRESS != audio_lc3_snk.state)
    {
        retval = GA_SUCCESS;
    }
    else
    {
        /*
         * Not Allowed, Since the Audio PL has the capability to playback only
         * one stream at a time. The codec conf for the CS and the ACA doesnt
         * matter.
         */
    }

    return retval;
#else /* AUDIO_SNK_LC3_SUPPORT */
    return GA_SUCCESS;
#endif /* AUDIO_SNK_LC3_SUPPORT */
}

#ifdef AUDIO_SNK_LC3_SUPPORT

GA_RESULT appl_ga_utils_audio_snk_create_lc3_decoder
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps
          )
{
    GA_RESULT  retval;
    INT32      lc3_retval;
    UINT8      index;
    UINT16     fd_in_us;
    UINT32     sf_hz;
    UINT8      cc;
    UINT16     fd_as_per_lc3_format;

    /* Initialize */
    retval = GA_SUCCESS;
    fd_in_us = 0U;
    sf_hz = 0U;
    cc = 0U;

    /* Fetch Sampling Freq in Hz */
    sf_hz = appl_ga_utils_bap_fetch_sf(cs_conf.sf);

    /* Get the Frame Duration in us */
    fd_in_us = appl_ga_utils_get_fd_in_us(cs_conf.fd);

    /* Fetch Channel Count from Audio Channel Allocation */
    cc = appl_ga_utils_fetch_sacc_from_aca(cs_conf.aca);

    GET_SUPP_FRAME_DURATION_FOR_LC3(fd_in_us, fd_as_per_lc3_format);
    /*
     * By default, we assume that the LC3 Encoder is called only after
     * validating appl_ga_utils_audio_snk_set_entry(), hence return GA_SUCCESS
     * by default.
     */

    if (AUDIO_LC3_IN_PROGRESS != audio_lc3_snk.state)
    {
        for (index = 0U; index < cc; index++)
        {
            /*
             * Assign Appl Buffer for enc_buf - Source -> LC3 Encoded Input,
             *                        dec_buf - Resultant -> LC3 Decoded Output
             */

            GA_mem_set
            (
                audio_lc3_snk.ctx.snk_enc_buffer, 0, sizeof(audio_lc3_snk.ctx.snk_enc_buffer)
            );
            GA_mem_set
            (
                audio_lc3_snk.ctx.snk_dec_buffer, 0, sizeof(audio_lc3_snk.ctx.snk_dec_buffer)
            );

            audio_lc3_snk.ctx.snk_enc_buf[index] = &audio_lc3_snk.ctx.snk_enc_buffer[index][0U];
            audio_lc3_snk.ctx.snk_dec_buf[index] = &audio_lc3_snk.ctx.snk_dec_buffer[index][0U];
        }

        /* Init LC3 Decode Module */
        lc3_retval = LC3_decoder_create
                     (
                         &audio_lc3_snk.ctx.dec_ctx,
                         sf_hz,
                         bps,
                         cc,
                         fd_as_per_lc3_format,
                         AUDIO_LC3_DEC_PLC_TYPE_DEFAULT,
                         audio_lc3_snk.ctx.dec_core_buffer,
                         audio_lc3_snk.ctx.dec_work_buffer,
                         audio_lc3_snk.ctx.snk_enc_buf,
                         audio_lc3_snk.ctx.snk_dec_buf
                     );

        if (LC3_DECODER_SUCCESS == lc3_retval)
        {
            APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: Created\n");
            audio_lc3_snk.sf_hz = (UINT16 )sf_hz;
            audio_lc3_snk.bps = bps;
            audio_lc3_snk.cc = cc;
            audio_lc3_snk.fd_in_us = fd_in_us;
            audio_lc3_snk.state = AUDIO_LC3_CREATED;
        }
        else
        {
            APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: Could not create !\n");
            GA_mem_set
            (
                &audio_lc3_snk.ctx.dec_ctx,
                0U,
                sizeof(LC3_DECODER_CNTX)
            );
            retval = GA_FAILURE;
        }
    }

    return retval;
}

UINT8 appl_ga_utils_audio_snk_is_lc3_decoder_running(void)
{
    UINT8 isRunning;

    /* Initialize */
    isRunning = GA_FALSE;

    if (AUDIO_LC3_IDLE == audio_lc3_snk.state)
    {
        APPL_DBG("[APPL][GA][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: Already Deleted !\n");
    }
    else
    {
        APPL_DBG("[APPL][GA][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: In Progress !\n");
        isRunning = GA_TRUE;
    }

    return isRunning;
}

GA_RESULT appl_ga_utils_audio_snk_delete_lc3_decoder(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (AUDIO_LC3_IDLE != audio_lc3_snk.state)
    {
        LC3_decoder_delete(&audio_lc3_snk.ctx.dec_ctx);
        GA_mem_set
        (
            &audio_lc3_snk.ctx.dec_ctx,
            0U,
            sizeof(LC3_DECODER_CNTX)
        );
        audio_lc3_snk.sf_hz = 0U;
        audio_lc3_snk.bps = 0U;
        audio_lc3_snk.cc = 0U;
        audio_lc3_snk.fd_in_us = 0U;
        audio_lc3_snk.state = AUDIO_LC3_IDLE;

        retval = GA_SUCCESS;
        APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: Deleted\n");
    }
    else
    {
        APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Setup Status: Already Deleted !\n");
    }

    return retval;
}

GA_RESULT audio_snk_lc3_decode_n_play(UCHAR* data, UINT16 datalen)
{
    INT32         encoded_byte_len[AUDIO_SNK_LC3_CHANNELS_MAX];
    static INT16  dec16_buffer[AUDIO_SNK_LC3_INPUT_FRAME_SIZE_MAX];
    static INT16  dec16_silence[AUDIO_SNK_LC3_INPUT_FRAME_SIZE_MAX];
    INT32         flg_bytes[AUDIO_SNK_LC3_CHANNELS_MAX];
    UINT16        index;
    INT32         samples_per_frame;

    /* Initialize */
    index = 0U;
    samples_per_frame = 0;

#ifdef SNK_DUMP_AUDIO_PREDECODED
    if (NULL != snk_dump_audio_predecoded_fd)
    {
        fwrite(data, sizeof(UINT8), (datalen), snk_dump_audio_predecoded_fd);
    }
#endif /* SNK_DUMP_AUDIO_PREDECODED */

    if (AUDIO_LC3_IDLE == audio_lc3_snk.state)
    {
        return GA_FAILURE;
    }

    BT_mem_copy(audio_lc3_snk.ctx.snk_enc_buffer[0], data, datalen);

    /* TODO */
    encoded_byte_len[0] = 0;
    flg_bytes[0] = 0; /* G192_GOOD_FRAME = 0 */
    encoded_byte_len[0U] = datalen; /* Single Channel Only */

    samples_per_frame = LC3_decoder_get_frame_length(&audio_lc3_snk.ctx.dec_ctx);

    if (LC3_DECODER_SUCCESS == LC3_decoder_process
                               (
                                   &audio_lc3_snk.ctx.dec_ctx,
                                   flg_bytes,
                                   encoded_byte_len
                               ))
    {
        /* APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Process Start Status: Succes\n"); */
        /* APPL_TRC("ISO: LC3 Decoded Data : %d\n", samples_per_frame); */

        for (index = 0U; index < samples_per_frame; index++)
        {
            dec16_buffer[index] = (INT16)(audio_lc3_snk.ctx.snk_dec_buffer[0U][index]);
        }

#ifdef SNK_DUMP_AUDIO_DECODED
        if (NULL != snk_dump_audio_decoded_fd)
        {
            fwrite(dec16_buffer, sizeof(INT16), (samples_per_frame), snk_dump_audio_decoded_fd);
        }
#endif /* SNK_DUMP_AUDIO_DECODED */

#ifdef AUDIO_SNK_PL_SUPPORT
        if (AUDIO_LC3_CREATED == audio_lc3_snk.state)
        {
            GA_mem_set(dec16_silence, 0x00, sizeof(dec16_silence));

            for (index = 0; index < LC3_SILENCE_BUFFER_COUNT; index++)
            {
                audio_snk_write_pl
                (
                    (UCHAR*)dec16_silence,
                    (UINT16)(samples_per_frame << 1) /* Check this */
                );
            }

            audio_lc3_snk.state = AUDIO_LC3_IN_PROGRESS;
        }

        audio_snk_write_pl
        (
            (UCHAR *)dec16_buffer,
            (UINT16)(samples_per_frame << 1) /* Check this */
        );
#endif /* AUDIO_SNK_PL_SUPPORT */
    }
    else
    {
        /* APPL_DBG("[APPL][AUDIO_SNK][LC3]: LC3 Decoder Process Start Status: Failed !\n"); */
    }

    return GA_SUCCESS;
}

#endif /* AUDIO_SNK_LC3_SUPPORT */

GA_RESULT appl_ga_utils_audio_snk_config_audio_dump_files(UINT8 op)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

#ifdef SNK_DUMP_AUDIO_PREDECODED
    /* Enable(Open) the file */
    if (1U == op)
    {
        snk_dump_audio_predecoded_fd = fopen("snk_dump_audio_predecoded", "wb+");
        if (NULL != snk_dump_audio_predecoded_fd)
        {
            APPL_DBG("Sink Audio Pre-Decoded Dump created successfully\n");
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_DBG("Could not create Sink Audio Pre-Decoded Dump\n");
        }
    }
    /* Disable(Close) the file */
    else if (2U == op)
    {
        if(NULL != snk_dump_audio_predecoded_fd)
        {
            fclose(snk_dump_audio_predecoded_fd);
            snk_dump_audio_predecoded_fd = NULL;
            APPL_DBG("Sink Audio Pre-Decoded Dump closed successfully\n");
        }
    }
    else
    {
        APPL_DBG("Invalid Operation\n");
    }
#else /* SNK_DUMP_AUDIO_PREDECODED */
    APPL_ERR("SNK_DUMP_AUDIO_PREDECODED is not enabled !\n");
#endif /* SNK_DUMP_AUDIO_PREDECODED */

#ifdef SNK_DUMP_AUDIO_DECODED
    /* Enable(Open) the file */
    if (1U == op)
    {
        snk_dump_audio_decoded_fd = fopen("snk_dump_audio_decoded", "wb+");
        if (NULL != snk_dump_audio_decoded_fd)
        {
            APPL_DBG("Sink Audio Decoded Dump created successfully\n");
            retval = GA_SUCCESS;
        }
        else
        {
            APPL_DBG("Could not create Sink Audio Decoded Dump\n");
        }
    }
    /* Disable(Close) the file */
    else if (2U == op)
    {
        if(NULL != snk_dump_audio_decoded_fd)
        {
            fclose(snk_dump_audio_decoded_fd);
            snk_dump_audio_decoded_fd = NULL;
            retval = GA_SUCCESS;
            APPL_DBG("Sink Audio Decoded Dump closed successfully\n");
        }
    }
    else
    {
        APPL_DBG("Invalid Operation\n");
    }
#else /* SNK_DUMP_AUDIO_DECODED */
    APPL_ERR("SNK_DUMP_AUDIO_DECODED is not enabled !\n");
#endif /* SNK_DUMP_AUDIO_DECODED */

    return retval;
}

#endif /* GA_BAP */
