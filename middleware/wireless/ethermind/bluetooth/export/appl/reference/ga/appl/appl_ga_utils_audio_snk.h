/**
 *  \file appl_ga_utils_audio_snk.h
 *
 *  Provides the GA Audio Data Path - Includes Audio Sink Interactions.
 *  1. ISO Module.
 *  2. Audio PL Module.
 *  3. LC3 Module.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UTILS_AUDIO_SNK_
#define _H_APPL_GA_UTILS_AUDIO_SNK_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "GA_bearer_api.h"
#include "appl_hci.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

#define AUDIO_SNK_MAX_ISO_ENTRIES                      5U

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
#define AUDIO_SNK_PL_BUFFER_SIZE    1536U
/* --------------------------------------------- APIs */

#ifdef AUDIO_SNK_ISO_SUPPORT
void appl_ga_utils_audio_snk_init(void);
void appl_ga_utils_audio_snk_deinit(void);
GA_RESULT appl_ga_utils_audio_snk_set_entry
          (
              UINT16                     conn_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          );
GA_RESULT appl_ga_utils_audio_snk_remove_entry(UINT16 conn_handle);
GA_RESULT appl_ga_utils_audio_snk_get_free_entry(UINT8 * index);
GA_RESULT appl_ga_utils_audio_snk_get_entry
          (
              UINT16   conn_handle,
              UINT8  * index
          );
GA_RESULT appl_ga_utils_audio_snk_update_entry
          (
              UINT16 conn_handle,
              UINT8  state,
              UINT8  status,
              UINT8  streaming_state
          );

GA_RESULT appl_ga_utils_audio_snk_le_setup_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          );

GA_RESULT appl_ga_utils_audio_snk_le_remove_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          );

GA_RESULT appl_ga_utils_audio_snk_register_iso_data_handler(void);

GA_RESULT appl_ga_utils_audio_snk_hci_iso_data_ind_cb
          (
              UCHAR  * header,
              UCHAR  * data,
              UINT16   datalen
          );

GA_RESULT appl_ga_utils_audio_snk_audio_lc3_dec_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          );
#endif /* AUDIO_SNK_ISO_SUPPORT */

#ifdef AUDIO_SNK_PL_SUPPORT
GA_RESULT appl_ga_utils_audio_snk_setup_playback_pl
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps,
              UINT16                     size
          );

GA_RESULT appl_ga_utils_audio_snk_start_playback_pl(void);

UINT8 appl_ga_utils_audio_snk_is_playback_pl_running(void);
GA_RESULT appl_ga_utils_audio_snk_stop_playback_pl(void);
#endif /* AUDIO_SNK_PL_SUPPORT */

#ifdef AUDIO_SNK_LC3_SUPPORT
GA_RESULT appl_ga_utils_audio_snk_create_lc3_decoder
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps
          );

UINT8 appl_ga_utils_audio_snk_is_lc3_decoder_running(void);
GA_RESULT appl_ga_utils_audio_snk_delete_lc3_decoder(void);
#endif /* AUDIO_SNK_LC3_SUPPORT */

GA_RESULT appl_ga_utils_audio_snk_config_audio_dump_files(UINT8 op);

#endif /* _H_APPL_GA_UTILS_AUDIO_SNK_ */