/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AUDIO_SAMPLES_AUDIO_SAMPLES_H_
#define AUDIO_SAMPLES_AUDIO_SAMPLES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MED_UI_WAKESOUND_WAV_LEN 62174
#define MED_SYSTEM_ALERTS_MELODIC_01_MONO_WAV_LEN 582972
#define MED_SYSTEM_ALERTS_MELODIC_01_SHORT_MONO_WAV_LEN 192104
#define MED_SYSTEM_ALERTS_MELODIC_02_MONO_WAV_LEN 582972
#define MED_SYSTEM_ALERTS_MELODIC_02_SHORT_MONO_WAV_LEN 192060
#define MED_ALERTS_NOTIFICATION_01_MONO_WAV_LEN 138300
#define MED_ALERTS_NOTIFICATION_03_MONO_WAV_LEN 170556
#define MED_STATE_PRIVACY_MODE_OFF_MONO_WAV_LEN 43202
#define MED_STATE_PRIVACY_MODE_ON_MONO_WAV_LEN 33600
#define MED_UI_ENDPOINTING_MONO_WAV_LEN 67200
#define MED_UI_WAKESOUND_MONO_WAV_LEN 62130

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
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Functions
 ******************************************************************************/

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
/*******************************************************************************
 * End
 ******************************************************************************/

#endif /* AUDIO_SAMPLES_AUDIO_SAMPLES_H_ */
