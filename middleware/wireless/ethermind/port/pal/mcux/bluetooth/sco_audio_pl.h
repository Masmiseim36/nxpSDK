
/**
 *  \file sco_audio_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2016. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_SCO_AUDIO_PL_
#define _H_SCO_AUDIO_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_hci_api.h"

/* --------------------------------------------- Global Definitions */
/* mSBC defines */
#define MSBC_ENCODER_INPUT_FRAME_LEN     240U
#define MSBC_ENCODER_OUTPUT_FRAME_LEN    57U

#define MSBC_DECODER_INPUT_FRAME_LEN     MSBC_ENCODER_OUTPUT_FRAME_LEN
#define MSBC_DECODER_OUTPUT_FRAME_LEN    MSBC_ENCODER_INPUT_FRAME_LEN

/* SCO H2 SYNC Header */
#define SCO_H2_SYNC_HDR                  0x0801U

/* --------------------------------------------- Structures/Data Types */
/* SCO Audio EndPoint information information structure */
typedef struct _SCO_AUDIO_EP_INFO
{
    /**
     * No. of channels
     */
    UCHAR   num_ch;

    /**
     * Sample Lenght(in bits)
     */
    UCHAR   sample_len;

    /**
     * Sampling Frequency
     */
    UINT16  sampl_freq;

    /**
     * Data size
     */
    UINT16  data_size;

    /**
     * EndPoint callback for read
     */
    void (* sco_in_complete_cb)(UCHAR *data, UINT16 datalen);

    /**
     * EndPoint callback for write complete
     */
    void (* sco_out_complete_cb)(UCHAR *data, UINT16 datalen);
}SCO_AUDIO_EP_INFO;

/* --------------------------------------------- Macros */
#ifndef SCO_PL_NO_DEBUG
#define SCO_PL_ERR(...)             BT_debug_error(0, __VA_ARGS__)
#else  /* SCO_PL_NO_DEBUG */
#define SCO_PL_ERR                  BT_debug_null
#endif /* SCO_PL_NO_DEBUG */

#ifdef SCO_PL_DEBUG
#define SCO_PL_TRC(...)             BT_debug_trace(0, __VA_ARGS__)
#define SCO_PL_INF(...)             BT_debug_info(0, __VA_ARGS__)
#else /* SCO_PL_DEBUG */
#define SCO_PL_TRC                  BT_debug_null
#define SCO_PL_INF                  BT_debug_null
#endif /* SCO_PL_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void sco_audio_init_pl (void);
void sco_audio_shutdown_pl (void);
API_RESULT sco_audio_setup_pl (SCO_AUDIO_EP_INFO *ep_info);
API_RESULT sco_audio_start_pl (void);
API_RESULT sco_audio_stop_pl (void);
void sco_audio_set_wideband_pl (UCHAR enable);

void sco_audio_play_ringtone_pl (void);
void sco_audio_play_inband_ringtone_pl (void);
void sco_audio_set_speaker_volume_pl (UCHAR value);
void sco_audio_set_microphone_gain_pl (UCHAR value);
void sco_audio_play_ringtone_exit_pl (void);

#ifdef HCI_SCO
void sco_audio_spkr_play_pl (UCHAR * m_data, UINT16 m_datalen);
#endif /* HCI_SCO */

#endif /* _H_SCO_AUDIOS_PL_*/
