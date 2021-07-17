/*
 * Automatically generated C config: don't edit
 * Freescale Audio Solutions Platform
 * Sun Aug  5 10:01:54 2018
 */
#define CASCFG_PLATFORM_VERSION "1.0.0-ga"

/*
 * Target Setup
 */
#define CASCFG_CPU_IMXRT1050 1
#define CASCFG_ARCH_CORTEX_M 1
/* CASCFG_BOARD_IMX8M_EVK is not set */
#define CASCFG_BOARD_IMXRT1050_EVK 1
/* CASCFG_BOARD_IMX6_SABRE_AUTO is not set */
/* CASCFG_BOARD_TWR_K70120F is not set */
/* CASCFG_BOARD_TWR_K64120F is not set */
/* CASCFG_BOARD_FRDM_K64120F is not set */
#define CASCFG_PLATFORM_FREERTOS 1

/*
 * Platform Configuration
 */

/*
 * Audio Sources
 */
/* CASCFG_AUD_ENABLE_FS_USB is not set */
/* CASCFG_AUD_ENABLE_FS_SD is not set */
/* CASCFG_AUD_ENABLE_ETHERNET is not set */
/* CASCFG_AUD_ENABLE_ANALOG is not set */

/*
 * Apple Device Support
 */
/* CASCFG_AUD_ENABLE_APPLE is not set */
/* CASCFG_AUD_ENABLE_AOAP is not set */

/*
 * Device Manager Configuration
 */
/* CASCFG_DEVMGR is not set */

/*
 * Input Manager Configuration
 */
/* CASCFG_INPUTMGR is not set */

/*
 * Persistent Storage Configuration
 */
/* CASCFG_ENABLE_PSM is not set */

/*
 * Vector Container Configuration
 */
/* CASCFG_VECTOR_SHRINK_AFTER_DATA_REMOVE is not set */
#define CASCFG_VECTOR_SIZE_MIN    10
#define CASCFG_VECTOR_EXPAND_STEP 5
#define CASCFG_VECTOR_EXPAND_FACT 1

/*
 * Freescale Audio Framework Configuration
 */

/*
 * Player Module Configuration
 */

/*
 * Audio Decoders
 */
#define CASCFG_USE_CCI                 1
#define CASCFG_NUM_OF_PING_PONG_BUFFER 2
#define CASCFG_ENABLE_WAV_CODEC        1
#define CASCFG_ENABLE_MP3_CODEC        1
/* CASCFG_ENABLE_WMA_CODEC is not set */
/* CASCFG_ENABLE_AAC_CODEC is not set */
/* CASCFG_ENABLE_FLAC_CODEC is not set */
/* CASCFG_ENABLE_OPUS_CODEC is not set */
/* CASCFG_ENABLE_OGG_CODEC is not set */
/* CASCFG_ENABLE_ALAC_CODEC is not set */
/* CASCFG_ENABLE_AC3_CODEC is not set */

/*
 * Thread Element
 */
/* CASCFG_ENABLE_THREAD_ELEMENT is not set */

/*
 * Audio Playlists
 */
/* CASCFG_ENABLE_PLAYLIST_SUPPORT is not set */

/*
 * Audio Trick Modes
 */
/* CASCFG_PLAYER_MUTE_ON_SEEK is not set */
/* CASCFG_PLAYER_FFREW_ACROSS_TRACK is not set */
#define CASCFG_PLAYER_DURATION_REWIND       -5000
#define CASCFG_PLAYER_DURATION_FAST_FORWARD 5000
#define CASCFG_PLAYER_MAX_REW_SEEK_TIME     -32000
#define CASCFG_PLAYER_MAX_FFW_SEEK_TIME     32000

/*
 * General Player Settings
 */
#define CASCFG_PLAYER_VOL_DEFAULT 80

/*
 * Browse Module Configuration
 */
#define CASCFG_RECURSIVE_FOLDER_MAX_DEPTH 6

/*
 * PlayQueue Module Configuration
 */
#define CASCFG_PLAYQUEUE_MAX_SIZE   1000
#define CASCFG_PLAYQUEUE_REPEAT_OFF 1
/* CASCFG_PLAYQUEUE_REPEAT_SINGLE is not set */
/* CASCFG_PLAYQUEUE_REPEAT_ALL is not set */
#define CASCFG_PLAYQUEUE_SHUFFLE_OFF 1
/* CASCFG_PLAYQUEUE_SHUFFLE_RANDOM is not set */

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
 * Debug and Logging Configuration
 */
#define CASCFG_LOG_ENABLED 1

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

/*
 * Log File Settings
 */
#define CASCFG_LOG_FILE_NAME "CR_logging_output.log"
/* CASCFG_LOG_STORAGE_USB is not set */
#define CASCFG_LOG_STORAGE_SD    1
#define CASCFG_SHELL_HISTORY     1
#define CASCFG_SHELL_HISTORY_NUM 20
