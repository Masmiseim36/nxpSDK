/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AUDIO_CFG_H_
#define _AUDIO_CFG_H_

/*
 * Metadata Configuration
 */
#define CASCFG_META_DEFAULT_ARTIST_NAME "Unknown Artist"
#define CASCFG_META_DEFAULT_ALBUM_NAME  "Unknown Album"
#define CASCFG_META_DEFAULT_GENRE_NAME  "Unknown Genre"
#define CASCFG_TRACK_TITLE_LEN          64
#define CASCFG_TRACK_ARTIST_LEN         64
#define CASCFG_TRACK_ALBUM_LEN          64
#define CASCFG_TRACK_GENRE_LEN          34
#define CASCFG_TRACK_YEAR_LEN           5

/*
 * Log Level - Message levels to always display
 */
#define CASCFG_LOGLVL_DEFAULT_CATASTROPHIC 1
#define CASCFG_LOGLVL_DEFAULT_ERROR        1
#define CASCFG_LOGLVL_DEFAULT_WARNING      1
#define CASCFG_LOGLVL_DEFAULT_INFO         1
/* CASCFG_LOGLVL_DEFAULT_FUNCTIONS is not set */
/* CASCFG_LOGLVL_DEFAULT_DEBUG is not set */

/*
 * Log Output - Initial setting for message output
 */
/* CASCFG_LOG_DEFAULT_OUTPUT_OFF is not set */
#define CASCFG_LOG_DEFAULT_OUTPUT_CONSOLE 1
/* CASCFG_LOG_DEFAULT_OUTPUT_FILE is not set */
/* CASCFG_LOG_DEFAULT_OUTPUT_BOTH is not set */

#endif
