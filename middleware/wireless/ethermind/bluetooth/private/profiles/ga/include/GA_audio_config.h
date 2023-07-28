/**
 *  \file GA_audio_config.h
 *
 *  \brief This file contains the various audio configurations
 *         defined in the BAP profile specification.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_AUDIO_CONFIG_
#define _H_GA_AUDIO_CONFIG_


/* --------------------------------------------- Header File Inclusion */
#include "GA_assigned_numbers.h"

/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \addtogroup ga_bap_constants
 * \{
 */

/* --------------------------------------------- Global Definitions */
/**
 * \defgroup ga_ac_sets GA Audio Config Sets
 * \{
 * \brief This section describes the Audio Config sets as defined in
 * BAP Specification.
 */

/**
 * \defgroup ga_codec_spec_cap GA Codec Specific Capabilities Sets
 * \{
 * \brief Codec Specific Capabilities Sets.
 * As defined in BAP Specification.
 */

/**
 * \name GA Codec Specific Capabilities Set 8_1
 * \{
 * \brief Codec Specific Capabilities: Set 8_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_8_1_ID             GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 8 */
#define GA_CODEC_CAP_8_1_SF             GA_LTV_CODEC_SPEC_CAP_SSF_VAL_8000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_8_1_FD             GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 26 */
#define GA_CODEC_CAP_8_1_OF             26

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 8_2
 * \{
 * \brief Codec Specific Capabilities: Set 8_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_8_2_ID             GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 8 */
#define GA_CODEC_CAP_8_2_SF             GA_LTV_CODEC_SPEC_CAP_SSF_VAL_8000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_8_2_FD             GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 30 */
#define GA_CODEC_CAP_8_2_OF             30

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 16_1
 * \{
 * \brief Codec Specific Capabilities: Set 16_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_16_1_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 16 */
#define GA_CODEC_CAP_16_1_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_16000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_16_1_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 30 */
#define GA_CODEC_CAP_16_1_OF            30

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 16_2
 * \{
 * \brief Codec Specific Capabilities: Set 16_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_16_2_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 16 */
#define GA_CODEC_CAP_16_2_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_16000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_16_2_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 40 */
#define GA_CODEC_CAP_16_2_OF            40

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 24_1
 * \{
 * \brief Codec Specific Capabilities: Set 24_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_24_1_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 24 */
#define GA_CODEC_CAP_24_1_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_24000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_24_1_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 45 */
#define GA_CODEC_CAP_24_1_OF            45

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 24_2
 * \{
 * \brief Codec Specific Capabilities: Set 24_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_24_2_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 24 */
#define GA_CODEC_CAP_24_2_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_24000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_24_2_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 60 */
#define GA_CODEC_CAP_24_2_OF            60

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 32_1
 * \{
 * \brief Codec Specific Capabilities: Set 32_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_32_1_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 32 */
#define GA_CODEC_CAP_32_1_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_32000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_32_1_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 60 */
#define GA_CODEC_CAP_32_1_OF            60

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 32_2
 * \{
 * \brief Codec Specific Capabilities: Set 32_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_32_2_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 32 */
#define GA_CODEC_CAP_32_2_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_32000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_32_2_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 80 */
#define GA_CODEC_CAP_32_2_OF            80

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 441_1
 * \{
 * \brief Codec Specific Capabilities: Set 441_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_441_1_ID           GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 44.1 */
#define GA_CODEC_CAP_441_1_SF           GA_LTV_CODEC_SPEC_CAP_SSF_VAL_44100
/**
 * Supported_Frame_Durations (ms): 7.5 \n
 * For 44.1 kHz/7.5ms, the actual SDU_Interval is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_CODEC_CAP_441_1_FD           GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 97 */
#define GA_CODEC_CAP_441_1_OF           97

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 441_2
 * \{
 * \brief Codec Specific Capabilities: Set 441_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_441_2_ID           GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 44.1 */
#define GA_CODEC_CAP_441_2_SF           GA_LTV_CODEC_SPEC_CAP_SSF_VAL_44100
/**
 * Supported_Frame_Durations (ms): 10 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_CODEC_CAP_441_2_FD           GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 130 */
#define GA_CODEC_CAP_441_2_OF           130

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_1
 * \{
 * \brief Codec Specific Capabilities: Set 48_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_1_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_1_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_48_1_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 75 */
#define GA_CODEC_CAP_48_1_OF            75

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_2
 * \{
 * \brief Codec Specific Capabilities: Set 48_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_2_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_2_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_48_2_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 100 */
#define GA_CODEC_CAP_48_2_OF            100

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_3
 * \{
 * \brief Codec Specific Capabilities: Set 48_3
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_3_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_3_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_48_3_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 90 */
#define GA_CODEC_CAP_48_3_OF            90

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_4
 * \{
 * \brief Codec Specific Capabilities: Set 48_4
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_4_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_4_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_48_4_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 120 */
#define GA_CODEC_CAP_48_4_OF            120

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_5
 * \{
 * \brief Codec Specific Capabilities: Set 48_5
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_5_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_5_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 7.5 */
#define GA_CODEC_CAP_48_5_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_7_5_MS
/** Supported_Octets_per_Codec_Frame(Octets): 117 */
#define GA_CODEC_CAP_48_5_OF            117

/** \} */

/**
 * \name GA Codec Specific Capabilities Set 48_6
 * \{
 * \brief Codec Specific Capabilities: Set 48_6
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CAP_48_6_ID            GA_AUDIO_CODING_LC3
/** Supported_Sampling_Frequencies(KHz): 48 */
#define GA_CODEC_CAP_48_6_SF            GA_LTV_CODEC_SPEC_CAP_SSF_VAL_48000
/** Supported_Frame_Durations (ms): 10 */
#define GA_CODEC_CAP_48_6_FD            GA_LTV_CODEC_SPEC_CAP_SFD_VAL_10_MS
/** Supported_Octets_per_Codec_Frame(Octets): 155 */
#define GA_CODEC_CAP_48_6_OF            155

/** \} */

/** \} */

/**
 * \defgroup ga_codec_spec_conf_sets GA Codec Specific Configuration Sets
 * \{
 * \brief Codec Specific Configuration Sets.
 * As defined in BAP Specification.
 * Note: The Bitrate as per BAP Spec, Codec-Specific Configuration for Unicast
 * and Broadcast (Table 3.11 and/or Table 3.12) is defined in kbps.
 * However there are some values which are decimal.
 * Hence to avoid the decimal part, we have them
 * defined in bps by * by 1000(k).
 * Eg: For Set 8_1, Bitrate: 27.734 Kbps.
 * Bitrate in Bps: (27.734 * 1000) = 27734 Bps which is
 * GA_CODEC_CONF_8_1_BITRATE_IN_BPS.
 */

/**
 * \name GA Codec Specific Configuration Set 8_1
 * \{
 * \brief Codec Specific Configuration: Set 8_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_8_1_ID                     GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 8 */
#define GA_CODEC_CONF_8_1_SF                     GA_LTV_CODEC_SPEC_CONF_SF_VAL_8000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_8_1_FD                     GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 26 */
#define GA_CODEC_CONF_8_1_OF                     26
/** Bitrate (Kbps): 27.734 */
#define GA_CODEC_CONF_8_1_BITRATE_IN_BPS         27734

/** \} */

/**
 * \name GA Codec Specific Configuration Set 8_2
 * \{
 * \brief Codec Specific Configuration: Set 8_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_8_2_ID                     GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 8 */
#define GA_CODEC_CONF_8_2_SF                     GA_LTV_CODEC_SPEC_CONF_SF_VAL_8000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_8_2_FD                     GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 30 */
#define GA_CODEC_CONF_8_2_OF                     30
/** Bitrate (Kbps): 24 */
#define GA_CODEC_CONF_8_2_BITRATE_IN_BPS         24000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 16_1
 * \{
 * \brief Codec Specific Configuration: Set 16_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_16_1_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 16 */
#define GA_CODEC_CONF_16_1_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_16000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_16_1_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 30 */
#define GA_CODEC_CONF_16_1_OF                    30
/** Bitrate (Kbps): 32 */
#define GA_CODEC_CONF_16_1_BITRATE_IN_BPS        32000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 16_2
 * \{
 * \brief Codec Specific Configuration: Set 16_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_16_2_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 16 */
#define GA_CODEC_CONF_16_2_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_16000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_16_2_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 40 */
#define GA_CODEC_CONF_16_2_OF                    40
/** Bitrate (Kbps): 32 */
#define GA_CODEC_CONF_16_2_BITRATE_IN_BPS        32000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 24_1
 * \{
 * \brief Codec Specific Configuration: Set 24_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_24_1_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 24 */
#define GA_CODEC_CONF_24_1_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_24000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_24_1_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 45 */
#define GA_CODEC_CONF_24_1_OF                    45
/** Bitrate (Kbps): 48 */
#define GA_CODEC_CONF_24_1_BITRATE_IN_BPS        48000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 24_2
 * \{
 * \brief Codec Specific Configuration: Set 24_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_24_2_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 24 */
#define GA_CODEC_CONF_24_2_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_24000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_24_2_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 60 */
#define GA_CODEC_CONF_24_2_OF                    60
/** Bitrate (Kbps): 48 */
#define GA_CODEC_CONF_24_2_BITRATE_IN_BPS        48000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 32_1
 * \{
 * \brief Codec Specific Configuration: Set 32_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_32_1_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 32 */
#define GA_CODEC_CONF_32_1_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_32000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_32_1_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 60 */
#define GA_CODEC_CONF_32_1_OF                    60
/** Bitrate (Kbps): 64 */
#define GA_CODEC_CONF_32_1_BITRATE_IN_BPS        64000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 32_2
 * \{
 * \brief Codec Specific Configuration: Set 32_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_32_2_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 32 */
#define GA_CODEC_CONF_32_2_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_32000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_32_2_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 80 */
#define GA_CODEC_CONF_32_2_OF                    80
/** Bitrate (Kbps): 64 */
#define GA_CODEC_CONF_32_2_BITRATE_IN_BPS        64000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 441_1
 * \{
 * \brief Codec Specific Configuration: Set 441_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_441_1_ID                   GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 44.1 */
#define GA_CODEC_CONF_441_1_SF                   GA_LTV_CODEC_SPEC_CONF_SF_VAL_44100
/**
 * Frame_Duration (ms): 7.5 \n
 * For 44.1 kHz/7.5ms the actual frame duration is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_CODEC_CONF_441_1_FD                   GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 97 */
#define GA_CODEC_CONF_441_1_OF                   97
/** Bitrate (Kbps): 95.06 */
#define GA_CODEC_CONF_441_1_BITRATE_IN_BPS       95060

/** \} */

/**
 * \name GA Codec Specific Configuration Set 441_2
 * \{
 * \brief Codec Specific Configuration: Set 441_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_441_2_ID                   GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 44.1 */
#define GA_CODEC_CONF_441_2_SF                   GA_LTV_CODEC_SPEC_CONF_SF_VAL_44100
/**
 * Frame_Duration (ms): 10 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_CODEC_CONF_441_2_FD                   GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 130 */
#define GA_CODEC_CONF_441_2_OF                   130
/** Bitrate (Kbps): 95.55 */
#define GA_CODEC_CONF_441_2_BITRATE_IN_BPS       95550

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_1
 * \{
 * \brief Codec Specific Configuration: Set 48_1
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_1_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_1_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_48_1_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 75 */
#define GA_CODEC_CONF_48_1_OF                    75
/** Bitrate (Kbps): 80 */
#define GA_CODEC_CONF_48_1_BITRATE_IN_BPS        80000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_2
 * \{
 * \brief Codec Specific Configuration: Set 48_2
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_2_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_2_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_48_2_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 100 */
#define GA_CODEC_CONF_48_2_OF                    100
/** Bitrate (Kbps): 80 */
#define GA_CODEC_CONF_48_2_BITRATE_IN_BPS        80000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_3
 * \{
 * \brief Codec Specific Configuration: Set 48_3
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_3_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_3_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_48_3_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 90 */
#define GA_CODEC_CONF_48_3_OF                    90
/** Bitrate (Kbps): 96 */
#define GA_CODEC_CONF_48_3_BITRATE_IN_BPS        96000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_4
 * \{
 * \brief Codec Specific Configuration: Set 48_4
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_4_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_4_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_48_4_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 120 */
#define GA_CODEC_CONF_48_4_OF                    120
/** Bitrate (Kbps): 96 */
#define GA_CODEC_CONF_48_4_BITRATE_IN_BPS        96000

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_5
 * \{
 * \brief Codec Specific Configuration: Set 48_5
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_5_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_5_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 7.5 */
#define GA_CODEC_CONF_48_5_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_7_5_MS
/** Octets per_Codec_Frame (Octets): 117 */
#define GA_CODEC_CONF_48_5_OF                    117
/** Bitrate (Kbps): 124.8 */
#define GA_CODEC_CONF_48_5_BITRATE_IN_BPS        124800

/** \} */

/**
 * \name GA Codec Specific Configuration Set 48_6
 * \{
 * \brief Codec Specific Configuration: Set 48_6
 * As defined in BAP Specification.
 */

/** Codec_ID: LC3 */
#define GA_CODEC_CONF_48_6_ID                    GA_AUDIO_CODING_LC3
/** Sampling_Frequency (kHz): 48 */
#define GA_CODEC_CONF_48_6_SF                    GA_LTV_CODEC_SPEC_CONF_SF_VAL_48000
/** Frame_Duration (ms): 10 */
#define GA_CODEC_CONF_48_6_FD                    GA_LTV_CODEC_SPEC_CONF_FD_VAL_10_MS
/** Octets per_Codec_Frame (Octets): 155 */
#define GA_CODEC_CONF_48_6_OF                    155
/** Bitrate (Kbps): 124 */
#define GA_CODEC_CONF_48_6_BITRATE_IN_BPS        124000

/** \} */

/** \} */

/** \} */

/**
 * \defgroup ga_qos_conf_setting_sets GA QOS Configuration Settings Sets
 * \{
 * \brief QOS Configuration Settings Sets.
 * As defined in BAP Specification.
 */

/**
 * \name QOS Configuration Settings Set 8_1_1
 * \{
 * \brief QoS Configuration Settings: Set 8_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_8_1_1_SDUINTERVAL_uS         7500
/** Framing: Unframed */
#define GA_8_1_1_FRAMING                GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 26 */
#define GA_8_1_1_MAXSDUSIZE_BYTES       26
/** Retransmission_Number: 2 */
#define GA_8_1_1_RTN                    2
/** Max_Transport_Latency (ms): 8 */
#define GA_8_1_1_MAXTLATENCY_mS         8
/** Presentation_Delay (us): 40000 */
#define GA_8_1_1_PRESENTDELAY_uS        40000

/** \} */

/**
 * \name QOS Configuration Settings Set 8_2_1
 * \{
 * \brief QoS Configuration Settings: Set 8_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_8_2_1_SDUINTERVAL_uS         10000
/** Framing: Unframed */
#define GA_8_2_1_FRAMING                GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 30 */
#define GA_8_2_1_MAXSDUSIZE_BYTES       30
/** Retransmission_Number: 2 */
#define GA_8_2_1_RTN                    2
/** Max_Transport_Latency (ms): 10 */
#define GA_8_2_1_MAXTLATENCY_mS         10
/** Presentation_Delay (us): 40000 */
#define GA_8_2_1_PRESENTDELAY_uS        40000

/** \} */

/**
 * \name QOS Configuration Settings Set 16_1_1
 * \{
 * \brief QoS Configuration Settings: Set 16_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_16_1_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_16_1_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 30 */
#define GA_16_1_1_MAXSDUSIZE_BYTES      30
/** Retransmission_Number: 2 */
#define GA_16_1_1_RTN                   2
/** Max_Transport_Latency (ms): 8 */
#define GA_16_1_1_MAXTLATENCY_mS        8
/** Presentation_Delay (us): 40000 */
#define GA_16_1_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 16_2_1
 * \{
 * \brief QoS Configuration Settings: Set 16_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_16_2_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_16_2_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 40 */
#define GA_16_2_1_MAXSDUSIZE_BYTES      40
/** Retransmission_Number: 2 */
#define GA_16_2_1_RTN                   2
/** Max_Transport_Latency (ms): 10 */
#define GA_16_2_1_MAXTLATENCY_mS        10
/** Presentation_Delay (us): 40000 */
#define GA_16_2_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 24_1_1
 * \{
 * \brief QoS Configuration Settings: Set 24_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_24_1_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_24_1_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 45 */
#define GA_24_1_1_MAXSDUSIZE_BYTES      45
/** Retransmission_Number: 2 */
#define GA_24_1_1_RTN                   2
/** Max_Transport_Latency (ms): 8 */
#define GA_24_1_1_MAXTLATENCY_mS        8
/** Presentation_Delay (us): 40000 */
#define GA_24_1_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 24_2_1
 * \{
 * \brief QoS Configuration Settings: Set 24_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_24_2_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_24_2_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 60 */
#define GA_24_2_1_MAXSDUSIZE_BYTES      60
/** Retransmission_Number: 2 */
#define GA_24_2_1_RTN                   2
/** Max_Transport_Latency (ms): 10 */
#define GA_24_2_1_MAXTLATENCY_mS        10
/** Presentation_Delay (us): 40000 */
#define GA_24_2_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 32_1_1
 * \{
 * \brief QoS Configuration Settings: Set 32_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_32_1_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_32_1_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 60 */
#define GA_32_1_1_MAXSDUSIZE_BYTES      60
/** Retransmission_Number: 2 */
#define GA_32_1_1_RTN                   2
/** Max_Transport_Latency (ms): 8 */
#define GA_32_1_1_MAXTLATENCY_mS        8
/** Presentation_Delay (us): 40000 */
#define GA_32_1_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 32_2_1
 * \{
 * \brief QoS Configuration Settings: Set 32_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_32_2_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_32_2_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 80 */
#define GA_32_2_1_MAXSDUSIZE_BYTES      80
/** Retransmission_Number: 2 */
#define GA_32_2_1_RTN                   2
/** Max_Transport_Latency (ms): 10 */
#define GA_32_2_1_MAXTLATENCY_mS        10
/** Presentation_Delay (us): 40000 */
#define GA_32_2_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 441_1_1
 * \{
 * \brief QoS Configuration Settings: Set 441_1_1
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 7500 \n
 * For 44.1 kHz/7.5ms, the actual SDU_Interval is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_441_1_1_SDUINTERVAL_uS       7500
/** Framing: Framed */
#define GA_441_1_1_FRAMING              GA_FRAMING_FRAMED
/** Maximum_SDU_Size(Octets): 97 */
#define GA_441_1_1_MAXSDUSIZE_BYTES     97
/** Retransmission_Number: 5 */
#define GA_441_1_1_RTN                  5
/** Max_Transport_Latency (ms): 24 */
#define GA_441_1_1_MAXTLATENCY_mS       24
/** Presentation_Delay (us): 40000 */
#define GA_441_1_1_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name QOS Configuration Settings Set 441_2_1
 * \{
 * \brief QoS Configuration Settings: Set 441_2_1
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 10000 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_441_2_1_SDUINTERVAL_uS       10000
/** Framing: Framed */
#define GA_441_2_1_FRAMING              GA_FRAMING_FRAMED
/** Maximum_SDU_Size(Octets): 130 */
#define GA_441_2_1_MAXSDUSIZE_BYTES     130
/** Retransmission_Number: 5 */
#define GA_441_2_1_RTN                  5
/** Max_Transport_Latency (ms): 31 */
#define GA_441_2_1_MAXTLATENCY_mS       31
/** Presentation_Delay (us): 40000 */
#define GA_441_2_1_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_1_1
 * \{
 * \brief QoS Configuration Settings: Set 48_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_1_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_1_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 75 */
#define GA_48_1_1_MAXSDUSIZE_BYTES      75
/** Retransmission_Number: 5 */
#define GA_48_1_1_RTN                   5
/** Max_Transport_Latency (ms): 15 */
#define GA_48_1_1_MAXTLATENCY_mS        15
/** Presentation_Delay (us): 40000 */
#define GA_48_1_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_2_1
 * \{
 * \brief QoS Configuration Settings: Set 48_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_2_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_2_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 100 */
#define GA_48_2_1_MAXSDUSIZE_BYTES      100
/** Retransmission_Number: 5 */
#define GA_48_2_1_RTN                   5
/** Max_Transport_Latency (ms): 20 */
#define GA_48_2_1_MAXTLATENCY_mS        20
/** Presentation_Delay (us): 40000 */
#define GA_48_2_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_3_1
 * \{
 * \brief QoS Configuration Settings: Set 48_3_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_3_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_3_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 90 */
#define GA_48_3_1_MAXSDUSIZE_BYTES      90
/** Retransmission_Number: 5 */
#define GA_48_3_1_RTN                   5
/** Max_Transport_Latency (ms): 15 */
#define GA_48_3_1_MAXTLATENCY_mS        15
/** Presentation_Delay (us): 40000 */
#define GA_48_3_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_4_1
 * \{
 * \brief QoS Configuration Settings: Set 48_4_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_4_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_4_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 120 */
#define GA_48_4_1_MAXSDUSIZE_BYTES      120
/** Retransmission_Number: 5 */
#define GA_48_4_1_RTN                   5
/** Max_Transport_Latency (ms): 20 */
#define GA_48_4_1_MAXTLATENCY_mS        20
/** Presentation_Delay (us): 40000 */
#define GA_48_4_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_5_1
 * \{
 * \brief QoS Configuration Settings: Set 48_5_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_5_1_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_5_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 117 */
#define GA_48_5_1_MAXSDUSIZE_BYTES      117
/** Retransmission_Number: 5 */
#define GA_48_5_1_RTN                   5
/** Max_Transport_Latency (ms): 15 */
#define GA_48_5_1_MAXTLATENCY_mS        15
/** Presentation_Delay (us): 40000 */
#define GA_48_5_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_6_1
 * \{
 * \brief QoS Configuration Settings: Set 48_6_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_6_1_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_6_1_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 155 */
#define GA_48_6_1_MAXSDUSIZE_BYTES      155
/** Retransmission_Number: 5 */
#define GA_48_6_1_RTN                   5
/** Max_Transport_Latency (ms): 20 */
#define GA_48_6_1_MAXTLATENCY_mS        20
/** Presentation_Delay (us): 40000 */
#define GA_48_6_1_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 8_1_2
 * \{
 * \brief QoS Configuration Settings: Set 8_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_8_1_2_SDUINTERVAL_uS         7500
/** Framing: Unframed */
#define GA_8_1_2_FRAMING                GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 26 */
#define GA_8_1_2_MAXSDUSIZE_BYTES       26
/** Retransmission_Number: 13 */
#define GA_8_1_2_RTN                    13
/** Max_Transport_Latency (ms): 75 */
#define GA_8_1_2_MAXTLATENCY_mS         75
/** Presentation_Delay (us): 40000 */
#define GA_8_1_2_PRESENTDELAY_uS        40000

/** \} */

/**
 * \name QOS Configuration Settings Set 8_2_2
 * \{
 * \brief QoS Configuration Settings: Set 8_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_8_2_2_SDUINTERVAL_uS         10000
/** Framing: Unframed */
#define GA_8_2_2_FRAMING                GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 30 */
#define GA_8_2_2_MAXSDUSIZE_BYTES       30
/** Retransmission_Number: 13 */
#define GA_8_2_2_RTN                    13
/** Max_Transport_Latency (ms): 95 */
#define GA_8_2_2_MAXTLATENCY_mS         95
/** Presentation_Delay (us): 40000 */
#define GA_8_2_2_PRESENTDELAY_uS        40000

/** \} */

/**
 * \name QOS Configuration Settings Set 16_1_2
 * \{
 * \brief QoS Configuration Settings: Set 16_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_16_1_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_16_1_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 30 */
#define GA_16_1_2_MAXSDUSIZE_BYTES      30
/** Retransmission_Number: 13 */
#define GA_16_1_2_RTN                   13
/** Max_Transport_Latency (ms): 75 */
#define GA_16_1_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_16_1_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 16_2_2
 * \{
 * \brief QoS Configuration Settings: Set 16_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_16_2_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_16_2_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 40 */
#define GA_16_2_2_MAXSDUSIZE_BYTES      40
/** Retransmission_Number: 13 */
#define GA_16_2_2_RTN                   13
/** Max_Transport_Latency (ms): 95 */
#define GA_16_2_2_MAXTLATENCY_mS        95
/** Presentation_Delay (us): 40000 */
#define GA_16_2_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 24_1_2
 * \{
 * \brief QoS Configuration Settings: Set 24_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_24_1_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_24_1_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 45 */
#define GA_24_1_2_MAXSDUSIZE_BYTES      45
/** Retransmission_Number: 13 */
#define GA_24_1_2_RTN                   13
/** Max_Transport_Latency (ms): 75 */
#define GA_24_1_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_24_1_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 24_2_2
 * \{
 * \brief QoS Configuration Settings: Set 24_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_24_2_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_24_2_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 60 */
#define GA_24_2_2_MAXSDUSIZE_BYTES      60
/** Retransmission_Number: 13 */
#define GA_24_2_2_RTN                   13
/** Max_Transport_Latency (ms): 95 */
#define GA_24_2_2_MAXTLATENCY_mS        95
/** Presentation_Delay (us): 40000 */
#define GA_24_2_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 32_1_2
 * \{
 * \brief QoS Configuration Settings: Set 32_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_32_1_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_32_1_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 60 */
#define GA_32_1_2_MAXSDUSIZE_BYTES      60
/** Retransmission_Number: 13 */
#define GA_32_1_2_RTN                   13
/** Max_Transport_Latency (ms): 8 */
#define GA_32_1_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_32_1_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 32_2_2
 * \{
 * \brief QoS Configuration Settings: Set 32_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_32_2_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_32_2_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 80 */
#define GA_32_2_2_MAXSDUSIZE_BYTES      80
/** Retransmission_Number: 13 */
#define GA_32_2_2_RTN                   13
/** Max_Transport_Latency (ms): 95 */
#define GA_32_2_2_MAXTLATENCY_mS        95
/** Presentation_Delay (us): 40000 */
#define GA_32_2_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 441_1_2
 * \{
 * \brief QoS Configuration Settings: Set 441_1_2
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 8163 \n
 * For 44.1 kHz/7.5ms, the actual SDU_Interval is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_441_1_2_SDUINTERVAL_uS       7500
/** Framing: Framed */
#define GA_441_1_2_FRAMING              GA_FRAMING_FRAMED
/** Maximum_SDU_Size(Octets): 97 */
#define GA_441_1_2_MAXSDUSIZE_BYTES     97
/** Retransmission_Number: 13 */
#define GA_441_1_2_RTN                  13
/** Max_Transport_Latency (ms): 80 */
#define GA_441_1_2_MAXTLATENCY_mS       80
/** Presentation_Delay (us): 40000 */
#define GA_441_1_2_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name QOS Configuration Settings Set 441_2_2
 * \{
 * \brief QoS Configuration Settings: Set 441_2_2
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 10884 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_441_2_2_SDUINTERVAL_uS       10000
/** Framing: Framed */
#define GA_441_2_2_FRAMING              GA_FRAMING_FRAMED
/** Maximum_SDU_Size(Octets): 130 */
#define GA_441_2_2_MAXSDUSIZE_BYTES     130
/** Retransmission_Number: 13 */
#define GA_441_2_2_RTN                  13
/** Max_Transport_Latency (ms): 85 */
#define GA_441_2_2_MAXTLATENCY_mS       85
/** Presentation_Delay (us): 40000 */
#define GA_441_2_2_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_1_2
 * \{
 * \brief QoS Configuration Settings: Set 48_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_1_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_1_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 75 */
#define GA_48_1_2_MAXSDUSIZE_BYTES      75
/** Retransmission_Number: 13 */
#define GA_48_1_2_RTN                   13
/** Max_Transport_Latency (ms): 75 */
#define GA_48_1_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_48_1_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_2_2
 * \{
 * \brief QoS Configuration Settings: Set 48_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_2_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_2_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 100 */
#define GA_48_2_2_MAXSDUSIZE_BYTES      100
/** Retransmission_Number: 13 */
#define GA_48_2_2_RTN                   13
/** Max_Transport_Latency (ms): 95 */
#define GA_48_2_2_MAXTLATENCY_mS        95
/** Presentation_Delay (us): 40000 */
#define GA_48_2_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_3_2
 * \{
 * \brief QoS Configuration Settings: Set 48_3_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_3_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_3_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 90 */
#define GA_48_3_2_MAXSDUSIZE_BYTES      90
/** Retransmission_Number: 13 */
#define GA_48_3_2_RTN                   13
/** Max_Transport_Latency (ms): 75 */
#define GA_48_3_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_48_3_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_4_2
 * \{
 * \brief QoS Configuration Settings: Set 48_4_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_4_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_4_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 120 */
#define GA_48_4_2_MAXSDUSIZE_BYTES      120
/** Retransmission_Number: 13 */
#define GA_48_4_2_RTN                   13
/** Max_Transport_Latency (ms): 100 */
#define GA_48_4_2_MAXTLATENCY_mS        100
/** Presentation_Delay (us): 40000 */
#define GA_48_4_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_5_2
 * \{
 * \brief QoS Configuration Settings: Set 48_5_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_48_5_2_SDUINTERVAL_uS        7500
/** Framing: Unframed */
#define GA_48_5_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 117 */
#define GA_48_5_2_MAXSDUSIZE_BYTES      117
/** Retransmission_Number: 13 */
#define GA_48_5_2_RTN                   13
/** Max_Transport_Latency (ms): 75 */
#define GA_48_5_2_MAXTLATENCY_mS        75
/** Presentation_Delay (us): 40000 */
#define GA_48_5_2_PRESENTDELAY_uS       40000

/** \} */

/**
 * \name QOS Configuration Settings Set 48_6_2
 * \{
 * \brief QoS Configuration Settings: Set 48_6_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_48_6_2_SDUINTERVAL_uS        10000
/** Framing: Unframed */
#define GA_48_6_2_FRAMING               GA_FRAMING_UNFRAMED
/** Maximum_SDU_Size(Octets): 155 */
#define GA_48_6_2_MAXSDUSIZE_BYTES      155
/** Retransmission_Number: 13 */
#define GA_48_6_2_RTN                   13
/** Max_Transport_Latency (ms): 100 */
#define GA_48_6_2_MAXTLATENCY_mS        100
/** Presentation_Delay (us): 40000 */
#define GA_48_6_2_PRESENTDELAY_uS       40000

/** \} */
/** \} */

/**
 * \defgroup ga_bc_conf_setting_sets GA Broadcast Configuration Settings Sets
 * \{
 * \brief Broadcast Audio Stream configuration settings Sets.
 * As defined in BAP Specification.
 */

/**
 * \name Broadcast Configuration Settings Set 8_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 8_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_8_1_1_SDUINTERVAL_uS       7500
/** Framing: Unframed */
#define GA_BC_8_1_1_FRAMING              GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 26 */
#define GA_BC_8_1_1_MAXSDUSIZE_BYTES     26
/** RTN: 2 */
#define GA_BC_8_1_1_RTN                  2
/** Max_Transport_Latency (ms): 8 */
#define GA_BC_8_1_1_MAXTLATENCY_mS       8
/** Presentation_Delay (us): 40000 */
#define GA_BC_8_1_1_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 8_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 8_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_8_2_1_SDUINTERVAL_uS       10000
/** Framing: Unframed */
#define GA_BC_8_2_1_FRAMING              GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 30 */
#define GA_BC_8_2_1_MAXSDUSIZE_BYTES     30
/** RTN: 2 */
#define GA_BC_8_2_1_RTN                  2
/** Max_Transport_Latency (ms): 10 */
#define GA_BC_8_2_1_MAXTLATENCY_mS       10
/** Presentation_Delay (us): 40000 */
#define GA_BC_8_2_1_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 16_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 16_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_16_1_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_16_1_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 30 */
#define GA_BC_16_1_1_MAXSDUSIZE_BYTES    30
/** RTN: 2 */
#define GA_BC_16_1_1_RTN                 2
/** Max_Transport_Latency (ms): 8 */
#define GA_BC_16_1_1_MAXTLATENCY_mS      8
/** Presentation_Delay (us): 40000 */
#define GA_BC_16_1_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 16_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 16_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_16_2_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_16_2_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 40 */
#define GA_BC_16_2_1_MAXSDUSIZE_BYTES    40
/** RTN: 2 */
#define GA_BC_16_2_1_RTN                 2
/** Max_Transport_Latency (ms): 10 */
#define GA_BC_16_2_1_MAXTLATENCY_mS      10
/** Presentation_Delay (us): 40000 */
#define GA_BC_16_2_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 24_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 24_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_24_1_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_24_1_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 45 */
#define GA_BC_24_1_1_MAXSDUSIZE_BYTES    45
/** RTN: 2 */
#define GA_BC_24_1_1_RTN                 2
/** Max_Transport_Latency (ms): 8 */
#define GA_BC_24_1_1_MAXTLATENCY_mS      8
/** Presentation_Delay (us): 40000 */
#define GA_BC_24_1_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 24_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 24_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_24_2_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_24_2_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 60 */
#define GA_BC_24_2_1_MAXSDUSIZE_BYTES    60
/** RTN: 2 */
#define GA_BC_24_2_1_RTN                 2
/** Max_Transport_Latency (ms): 10 */
#define GA_BC_24_2_1_MAXTLATENCY_mS      10
/** Presentation_Delay (us): 40000 */
#define GA_BC_24_2_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 32_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 32_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_32_1_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_32_1_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 60 */
#define GA_BC_32_1_1_MAXSDUSIZE_BYTES    60
/** RTN: 2 */
#define GA_BC_32_1_1_RTN                 2
/** Max_Transport_Latency (ms): 8 */
#define GA_BC_32_1_1_MAXTLATENCY_mS      8
/** Presentation_Delay (us): 40000 */
#define GA_BC_32_1_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 32_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 32_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_32_2_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_32_2_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 80 */
#define GA_BC_32_2_1_MAXSDUSIZE_BYTES    80
/** RTN: 2 */
#define GA_BC_32_2_1_RTN                 2
/** Max_Transport_Latency (ms): 10 */
#define GA_BC_32_2_1_MAXTLATENCY_mS      10
/** Presentation_Delay (us): 40000 */
#define GA_BC_32_2_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 441_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 441_1_1
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 7500 \n
 * For 44.1 kHz/7.5ms, the actual SDU_Interval is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_BC_441_1_1_SDUINTERVAL_uS     7500
/** Framing: Framed */
#define GA_BC_441_1_1_FRAMING            GA_FRAMING_FRAMED
/** Max_SDU (Octets): 97 */
#define GA_BC_441_1_1_MAXSDUSIZE_BYTES   97
/** RTN: 4 */
#define GA_BC_441_1_1_RTN                4
/** Max_Transport_Latency (ms): 24 */
#define GA_BC_441_1_1_MAXTLATENCY_mS     24
/** Presentation_Delay (us): 40000 */
#define GA_BC_441_1_1_PRESENTDELAY_uS    40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 441_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 441_2_1
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 10000 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_BC_441_2_1_SDUINTERVAL_uS     10000
/** Framing: Framed */
#define GA_BC_441_2_1_FRAMING            GA_FRAMING_FRAMED
/** Max_SDU (Octets): 130 */
#define GA_BC_441_2_1_MAXSDUSIZE_BYTES   130
/** RTN: 4 */
#define GA_BC_441_2_1_RTN                4
/** Max_Transport_Latency (ms): 31 */
#define GA_BC_441_2_1_MAXTLATENCY_mS     31
/** Presentation_Delay (us): 40000 */
#define GA_BC_441_2_1_PRESENTDELAY_uS    40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_1_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_1_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_1_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_1_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 75 */
#define GA_BC_48_1_1_MAXSDUSIZE_BYTES    75
/** RTN: 4 */
#define GA_BC_48_1_1_RTN                 4
/** Max_Transport_Latency (ms): 15 */
#define GA_BC_48_1_1_MAXTLATENCY_mS      15
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_1_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_2_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_2_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_2_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_2_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 100 */
#define GA_BC_48_2_1_MAXSDUSIZE_BYTES    100
/** RTN: 4 */
#define GA_BC_48_2_1_RTN                 4
/** Max_Transport_Latency (ms): 20 */
#define GA_BC_48_2_1_MAXTLATENCY_mS      20
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_2_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_3_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_3_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_3_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_3_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 90 */
#define GA_BC_48_3_1_MAXSDUSIZE_BYTES    90
/** RTN: 4 */
#define GA_BC_48_3_1_RTN                 4
/** Max_Transport_Latency (ms): 15 */
#define GA_BC_48_3_1_MAXTLATENCY_mS      15
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_3_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_4_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_4_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_4_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_4_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 120 */
#define GA_BC_48_4_1_MAXSDUSIZE_BYTES    120
/** RTN: 4 */
#define GA_BC_48_4_1_RTN                 4
/** Max_Transport_Latency (ms): 20 */
#define GA_BC_48_4_1_MAXTLATENCY_mS      20
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_4_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_5_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_5_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_5_1_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_5_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 117 */
#define GA_BC_48_5_1_MAXSDUSIZE_BYTES    117
/** RTN: 4 */
#define GA_BC_48_5_1_RTN                 4
/** Max_Transport_Latency (ms): 15 */
#define GA_BC_48_5_1_MAXTLATENCY_mS      15
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_5_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_6_1
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_6_1
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_6_1_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_6_1_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 155 */
#define GA_BC_48_6_1_MAXSDUSIZE_BYTES    155
/** RTN: 4 */
#define GA_BC_48_6_1_RTN                 4
/** Max_Transport_Latency (ms): 20 */
#define GA_BC_48_6_1_MAXTLATENCY_mS      20
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_6_1_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 8_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 8_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_8_1_2_SDUINTERVAL_uS       7500
/** Framing: Unframed */
#define GA_BC_8_1_2_FRAMING              GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 26 */
#define GA_BC_8_1_2_MAXSDUSIZE_BYTES     26
/** RTN: 4 */
#define GA_BC_8_1_2_RTN                  4
/** Max_Transport_Latency (ms): 45 */
#define GA_BC_8_1_2_MAXTLATENCY_mS       45
/** Presentation_Delay (us): 40000 */
#define GA_BC_8_1_2_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 8_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 8_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_8_2_2_SDUINTERVAL_uS       10000
/** Framing: Unframed */
#define GA_BC_8_2_2_FRAMING              GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 30 */
#define GA_BC_8_2_2_MAXSDUSIZE_BYTES     30
/** RTN: 4 */
#define GA_BC_8_2_2_RTN                  4
/** Max_Transport_Latency (ms): 60 */
#define GA_BC_8_2_2_MAXTLATENCY_mS       60
/** Presentation_Delay (us): 40000 */
#define GA_BC_8_2_2_PRESENTDELAY_uS      40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 16_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 16_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_16_1_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_16_1_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 30 */
#define GA_BC_16_1_2_MAXSDUSIZE_BYTES    30
/** RTN: 4 */
#define GA_BC_16_1_2_RTN                 4
/** Max_Transport_Latency (ms): 45 */
#define GA_BC_16_1_2_MAXTLATENCY_mS      45
/** Presentation_Delay (us): 40000 */
#define GA_BC_16_1_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 16_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 16_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_16_2_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_16_2_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 40 */
#define GA_BC_16_2_2_MAXSDUSIZE_BYTES    40
/** RTN: 4 */
#define GA_BC_16_2_2_RTN                 4
/** Max_Transport_Latency (ms): 60 */
#define GA_BC_16_2_2_MAXTLATENCY_mS      60
/** Presentation_Delay (us): 40000 */
#define GA_BC_16_2_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 24_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 24_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_24_1_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_24_1_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 45 */
#define GA_BC_24_1_2_MAXSDUSIZE_BYTES    45
/** RTN: 4 */
#define GA_BC_24_1_2_RTN                 4
/** Max_Transport_Latency (ms): 45 */
#define GA_BC_24_1_2_MAXTLATENCY_mS      45
/** Presentation_Delay (us): 40000 */
#define GA_BC_24_1_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 24_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 24_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_24_2_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_24_2_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 60 */
#define GA_BC_24_2_2_MAXSDUSIZE_BYTES    60
/** RTN: 4 */
#define GA_BC_24_2_2_RTN                 4
/** Max_Transport_Latency (ms): 60 */
#define GA_BC_24_2_2_MAXTLATENCY_mS      60
/** Presentation_Delay (us): 40000 */
#define GA_BC_24_2_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 32_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 32_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_32_1_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_32_1_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 60 */
#define GA_BC_32_1_2_MAXSDUSIZE_BYTES    60
/** RTN: 4 */
#define GA_BC_32_1_2_RTN                 4
/** Max_Transport_Latency (ms): 45 */
#define GA_BC_32_1_2_MAXTLATENCY_mS      45
/** Presentation_Delay (us): 40000 */
#define GA_BC_32_1_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 32_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 32_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_32_2_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_32_2_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 80 */
#define GA_BC_32_2_2_MAXSDUSIZE_BYTES    80
/** RTN: 4 */
#define GA_BC_32_2_2_RTN                 4
/** Max_Transport_Latency (ms): 60 */
#define GA_BC_32_2_2_MAXTLATENCY_mS      60
/** Presentation_Delay (us): 40000 */
#define GA_BC_32_2_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 441_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 441_1_2
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 8163 \n
 * For 44.1 kHz/7.5ms, the actual SDU_Interval is equivalent to
 * 360 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 8.16327 ms
 */
#define GA_BC_441_1_2_SDUINTERVAL_uS     7500
/** Framing: Framed */
#define GA_BC_441_1_2_FRAMING            GA_FRAMING_FRAMED
/** Max_SDU (Octets): 97 */
#define GA_BC_441_1_2_MAXSDUSIZE_BYTES   97
/** RTN: 4 */
#define GA_BC_441_1_2_RTN                4
/** Max_Transport_Latency (ms): 54 */
#define GA_BC_441_1_2_MAXTLATENCY_mS     54
/** Presentation_Delay (us): 40000 */
#define GA_BC_441_1_2_PRESENTDELAY_uS    40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 441_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 441_2_2
 * As defined in BAP Specification.
 */

/**
 * SDU_Interval (us): 10884 \n
 * For 44.1 kHz/10 ms the actual frame duration is equal to
 * 480 (samples per second) divided by 44100 (Sampling Frequency), which
 * equals 10.88435 ms
 */
#define GA_BC_441_2_2_SDUINTERVAL_uS     10000
/** Framing: Framed */
#define GA_BC_441_2_2_FRAMING            GA_FRAMING_FRAMED
/** Max_SDU (Octets): 130 */
#define GA_BC_441_2_2_MAXSDUSIZE_BYTES   130
/** RTN: 4 */
#define GA_BC_441_2_2_RTN                4
/** Max_Transport_Latency (ms): 60 */
#define GA_BC_441_2_2_MAXTLATENCY_mS     60
/** Presentation_Delay (us): 40000 */
#define GA_BC_441_2_2_PRESENTDELAY_uS    40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_1_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_1_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_1_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_1_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 75 */
#define GA_BC_48_1_2_MAXSDUSIZE_BYTES    75
/** RTN: 4 */
#define GA_BC_48_1_2_RTN                 4
/** Max_Transport_Latency (ms): 50 */
#define GA_BC_48_1_2_MAXTLATENCY_mS      50
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_1_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_2_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_2_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_2_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_2_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 100 */
#define GA_BC_48_2_2_MAXSDUSIZE_BYTES    100
/** RTN: 4 */
#define GA_BC_48_2_2_RTN                 4
/** Max_Transport_Latency (ms): 65 */
#define GA_BC_48_2_2_MAXTLATENCY_mS      65
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_2_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_3_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_3_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_3_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_3_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 90 */
#define GA_BC_48_3_2_MAXSDUSIZE_BYTES    90
/** RTN: 4 */
#define GA_BC_48_3_2_RTN                 4
/** Max_Transport_Latency (ms): 50 */
#define GA_BC_48_3_2_MAXTLATENCY_mS      50
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_3_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_4_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_4_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_4_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_4_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 120 */
#define GA_BC_48_4_2_MAXSDUSIZE_BYTES    120
/** RTN: 4 */
#define GA_BC_48_4_2_RTN                 4
/** Max_Transport_Latency (ms): 65 */
#define GA_BC_48_4_2_MAXTLATENCY_mS      65
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_4_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_5_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_5_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 7500 */
#define GA_BC_48_5_2_SDUINTERVAL_uS      7500
/** Framing: Unframed */
#define GA_BC_48_5_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 117 */
#define GA_BC_48_5_2_MAXSDUSIZE_BYTES    117
/** RTN: 4 */
#define GA_BC_48_5_2_RTN                 4
/** Max_Transport_Latency (ms): 50 */
#define GA_BC_48_5_2_MAXTLATENCY_mS      50
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_5_2_PRESENTDELAY_uS     40000

/** \} */

/**
 * \name Broadcast Configuration Settings Set 48_6_2
 * \{
 * \brief Broadcast Audio Stream Configuration Settings: Set 48_6_2
 * As defined in BAP Specification.
 */

/** SDU_Interval (us): 10000 */
#define GA_BC_48_6_2_SDUINTERVAL_uS      10000
/** Framing: Unframed */
#define GA_BC_48_6_2_FRAMING             GA_FRAMING_UNFRAMED
/** Max_SDU (Octets): 155 */
#define GA_BC_48_6_2_MAXSDUSIZE_BYTES    155
/** RTN: 4 */
#define GA_BC_48_6_2_RTN                 4
/** Max_Transport_Latency (ms): 65 */
#define GA_BC_48_6_2_MAXTLATENCY_mS      65
/** Presentation_Delay (us): 40000 */
#define GA_BC_48_6_2_PRESENTDELAY_uS     40000

/** \} */
/** \} */

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

#endif /* _H_GA_AUDIO_CONFIG_ */
