/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AUDIO_SAMPLES_AUDIO_SAMPLES_H_
#define AUDIO_SAMPLES_AUDIO_SAMPLES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MED_UI_WAKESOUND_WAV_LEN                        62174
#define MED_SYSTEM_ALERTS_MELODIC_01_MONO_WAV_LEN       582972
#define MED_SYSTEM_ALERTS_MELODIC_01_SHORT_MONO_WAV_LEN 192104
#define MED_SYSTEM_ALERTS_MELODIC_02_MONO_WAV_LEN       582972
#define MED_SYSTEM_ALERTS_MELODIC_02_SHORT_MONO_WAV_LEN 192060
#define MED_ALERTS_NOTIFICATION_01_MONO_WAV_LEN         138300
#define MED_ALERTS_NOTIFICATION_03_MONO_WAV_LEN         170556
#define MED_STATE_PRIVACY_MODE_OFF_MONO_WAV_LEN         43202
#define MED_STATE_PRIVACY_MODE_ON_MONO_WAV_LEN          33600
#define MED_UI_ENDPOINTING_MONO_WAV_LEN                 67200
#define MED_UI_WAKESOUND_MONO_WAV_LEN                   62130

#define AUDIO_SAMPLE_WAKE_WORD_BYTES 124260

/*******************************************************************************
 * Global Vars
 ******************************************************************************/
extern const unsigned char _med_ui_wakesound_wav[MED_UI_WAKESOUND_WAV_LEN];
extern const unsigned char med_system_alerts_melodic_01_mono_wav[MED_SYSTEM_ALERTS_MELODIC_01_MONO_WAV_LEN];
extern const unsigned char med_system_alerts_melodic_01_short_mono_wav[MED_SYSTEM_ALERTS_MELODIC_01_SHORT_MONO_WAV_LEN];
extern const unsigned char med_system_alerts_melodic_02_mono_wav[MED_SYSTEM_ALERTS_MELODIC_02_MONO_WAV_LEN];
extern const unsigned char med_system_alerts_melodic_02_short_mono_wav[MED_SYSTEM_ALERTS_MELODIC_02_SHORT_MONO_WAV_LEN];
extern const unsigned char med_alerts_notification_01_mono_wav[MED_ALERTS_NOTIFICATION_01_MONO_WAV_LEN];
extern const unsigned char med_alerts_notification_03_mono_wav[MED_ALERTS_NOTIFICATION_03_MONO_WAV_LEN];
extern const unsigned char med_state_privacy_mode_off_mono_wav[MED_STATE_PRIVACY_MODE_OFF_MONO_WAV_LEN];
extern const unsigned char med_state_privacy_mode_on_mono_wav[MED_STATE_PRIVACY_MODE_ON_MONO_WAV_LEN];
extern const unsigned char med_ui_endpointing_mono_wav[MED_UI_ENDPOINTING_MONO_WAV_LEN];
extern const unsigned char med_ui_wakesound_mono_wav[MED_UI_WAKESOUND_MONO_WAV_LEN];

#endif /* AUDIO_SAMPLES_AUDIO_SAMPLES_H_ */
