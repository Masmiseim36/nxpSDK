/**
 *  \file appl_ga_utils_audio_src.h
 *
 *  Provides the GA Audio Data Path - Includes Audio Source Interactions.
 *  1. ISO Module.
 *  2. Audio PL Module.
 *  3. LC3 Module.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UTILS_AUDIO_SRC_
#define _H_APPL_GA_UTILS_AUDIO_SRC_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_hci.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
#define AUDIO_SRC_PL_BUFFER_SIZE    1536U
/* --------------------------------------------- APIs */
#ifdef AUDIO_SRC_ISO_SUPPORT
void appl_ga_utils_audio_src_init(void);
void appl_ga_utils_audio_src_deinit(void);
GA_RESULT appl_ga_utils_audio_src_set_entry
          (
              UINT16                     conn_handle,
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          );
GA_RESULT appl_ga_utils_audio_src_remove_entry(UINT16 conn_handle);
GA_RESULT appl_ga_utils_audio_src_get_free_entry(UINT8 * index);
GA_RESULT appl_ga_utils_audio_src_get_entry
          (
              UINT16   conn_handle,
              UINT8  * index
          );
GA_RESULT appl_ga_utils_audio_src_update_entry
          (
              UINT16 conn_handle,
              UINT8  state,
              UINT8  status,
              UINT8  streaming_state
          );

GA_RESULT appl_ga_utils_audio_src_le_setup_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          );

GA_RESULT appl_ga_utils_audio_src_le_remove_iso_data_path
          (
              /* IN */ UINT16 conn_handle
          );

GA_RESULT appl_ga_utils_audio_src_audio_lc3_enc_allowed
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf
          );

#endif /* AUDIO_SRC_ISO_SUPPORT */

#ifdef AUDIO_SRC_PL_SUPPORT
GA_RESULT appl_ga_utils_audio_src_setup_generator_pl
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UCHAR                      bps,
              UINT16                     size
          );

GA_RESULT appl_ga_utils_audio_src_start_generator_pl(void);

UINT8 appl_ga_utils_audio_src_is_generator_pl_running(void);
GA_RESULT appl_ga_utils_audio_src_stop_generator_pl(void);
#endif /* AUDIO_SRC_PL_SUPPORT */

#ifdef AUDIO_SRC_LC3_SUPPORT
GA_RESULT appl_ga_utils_audio_src_create_lc3_encoder
          (
              GA_LC3_CODEC_SPECIFIC_CONF cs_conf,
              UINT8                      cs_conf_set_id,
              UCHAR                      bps
          );
UINT8 appl_ga_utils_audio_src_is_lc3_encoder_running(void);
GA_RESULT appl_ga_utils_audio_src_delete_lc3_encoder(void);

#endif /* AUDIO_SRC_LC3_SUPPORT */

GA_RESULT appl_ga_utils_audio_src_config_audio_dump_files(UINT8 op);

#endif /* _H_APPL_GA_UTILS_AUDIO_SRC_ */