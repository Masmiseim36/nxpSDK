/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_UTILITY_APP_
#define _SLN_UTILITY_APP_

/**********************************************/
/* Generated File Do Not Edit!                */
/**********************************************/

#include <stdint.h>

/**********************************************/
/* Thumb compatibility macro                  */
/**********************************************/

#define SET_THUMB_ADDRESS(x)  (x | 0x1)

/**********************************************/
/* Memory Allocation and De-allocation Types  */
/**********************************************/

typedef void *(*malloc_func_t)(size_t size);
typedef void (*free_func_t)(void* ptr);

/**********************************************/
/* SLN_DSP status codes                       */
/**********************************************/

typedef enum _sln_dsp_states
{
    kDspOutOfMemory        = -7,
    kDspStreamIDInUse      = -6,
    kDspNoStreamsAvailable = -5,
    kDspBitGrowthOverflow  = -4,
    kDspInvalidSteamID     = -3,
    kDspInvalidParam       = -2,
    kDspNullPointer        = -1,
    kDspSuccess            = 0
}sln_dsp_states_t;

/**********************************************/
/* SLN_DSP function pointer types             */
/**********************************************/

typedef int32_t (*SLN_DSP_Init_t)(uint8_t **memPool, malloc_func_t malloc_func);

typedef int32_t (*SLN_DSP_Deinit_t)(uint8_t **memPool, free_func_t free_func);

typedef int32_t (*SLN_DSP_SetCaptureLength_t)(uint8_t **memPool, uint32_t lenBytes);

typedef int32_t (*SLN_DSP_GetConversionLength_t)(uint8_t **memPool, uint32_t *lenBytes);

typedef int32_t (*SLN_DSP_SetGainFactor_t)(uint8_t **memPool, int16_t gainFactor);

typedef int32_t (*SLN_DSP_pdm_to_pcm_t)(uint8_t **memPool, uint32_t streamID, uint8_t *in, int16_t *out);

typedef int32_t (*SLN_DSP_create_scratch_buf_t)(uint8_t **memPool, uint32_t **scratch, malloc_func_t malloc_func);

typedef int32_t (*SLN_DSP_pdm_to_pcm_multi_ch_t)(uint8_t **memPool, uint32_t firstStreamID, uint32_t numChannels, uint32_t *in, int16_t *out, uint32_t *scratch);

typedef int32_t (*SLN_DSP_downsample_by_3_t)(uint8_t **memPool, uint32_t streamID, int16_t *in, uint32_t inSampleCount, int16_t *out);


/**********************************************/
/* SLN_DSP function addresses                 */
/**********************************************/

#ifdef SLN_DSP
SLN_DSP_Init_t                           SLN_DSP_Init                             = (SLN_DSP_Init_t) SET_THUMB_ADDRESS(0x60200094);
SLN_DSP_Deinit_t                         SLN_DSP_Deinit                           = (SLN_DSP_Deinit_t) SET_THUMB_ADDRESS(0x60200214);
SLN_DSP_SetCaptureLength_t               SLN_DSP_SetCaptureLength                 = (SLN_DSP_SetCaptureLength_t) SET_THUMB_ADDRESS(0x60200244);
SLN_DSP_GetConversionLength_t            SLN_DSP_GetConversionLength              = (SLN_DSP_GetConversionLength_t) SET_THUMB_ADDRESS(0x60200268);
SLN_DSP_SetGainFactor_t                  SLN_DSP_SetGainFactor                    = (SLN_DSP_SetGainFactor_t) SET_THUMB_ADDRESS(0x60200280);
SLN_DSP_pdm_to_pcm_t                     SLN_DSP_pdm_to_pcm                       = (SLN_DSP_pdm_to_pcm_t) SET_THUMB_ADDRESS(0x60200294);
SLN_DSP_create_scratch_buf_t             SLN_DSP_create_scratch_buf               = (SLN_DSP_create_scratch_buf_t) SET_THUMB_ADDRESS(0x60200424);
SLN_DSP_pdm_to_pcm_multi_ch_t            SLN_DSP_pdm_to_pcm_multi_ch              = (SLN_DSP_pdm_to_pcm_multi_ch_t) SET_THUMB_ADDRESS(0x60200448);
SLN_DSP_downsample_by_3_t                SLN_DSP_downsample_by_3                  = (SLN_DSP_downsample_by_3_t) SET_THUMB_ADDRESS(0x60200690);
#endif /* SLN_DSP */

/**********************************************/
/* SLN_Voice status codes                     */
/**********************************************/

typedef enum _sln_voice_states
{
    kVoiceFail               = 0,
    kVoiceOkay               = 1,
    kVoiceUnsupported        = 2,
    kVoiceMemError           = 3,
    kVoiceNoAudioProcessing  = 4,
    kVoiceNoWakeWord         = 5,
}sln_voice_states_t;

typedef struct _sln_afe_configuration_params
{
    uint16_t u16PostProcessedGain; 	// The amount of dynamic gain after processing. This will not causing clipping */
    uint8_t u8NumberOfMics; 		// The number of microphones. Only 2/3 microphones are supported */
}sln_afe_configuration_params_t;

/**********************************************/
/* SLN_Voice function pointer types           */
/**********************************************/

typedef uint32_t (*SLN_Voice_Req_Mem_Size_t)(void);

typedef uint32_t (*SLN_Voice_Init_t)(uint8_t *memPool, sln_afe_configuration_params_t *afeConfig);

typedef uint32_t (*SLN_Voice_Process_Audio_t)(uint8_t *memPool, int16_t *pi16AudioBuff, int16_t *pi16RefSignal, uint8_t **ppu16ProcessedAudio, uint8_t *pu8WakeWordDetected, uint16_t *pu16WakeWordLength);

typedef uint32_t (*SLN_Voice_Process_Wake_Word_t)(uint8_t *memPool, int16_t *pi16AudioBuff, uint8_t *pu8WakeWordDetected, uint16_t *pu16WakeWordLength);


/**********************************************/
/* SLN_Voice function addresses               */
/**********************************************/

#ifdef SLN_Voice
SLN_Voice_Req_Mem_Size_t                 SLN_Voice_Req_Mem_Size                   = (SLN_Voice_Req_Mem_Size_t) SET_THUMB_ADDRESS(0x60200BB8);
SLN_Voice_Init_t                         SLN_Voice_Init                           = (SLN_Voice_Init_t) SET_THUMB_ADDRESS(0x60208EA6);
SLN_Voice_Process_Audio_t                SLN_Voice_Process_Audio                  = (SLN_Voice_Process_Audio_t) SET_THUMB_ADDRESS(0x60208EC8);
SLN_Voice_Process_Wake_Word_t            SLN_Voice_Process_Wake_Word              = (SLN_Voice_Process_Wake_Word_t) SET_THUMB_ADDRESS(0x60208F0A);
#endif /* SLN_Voice */

#endif // _SLN_UTILITY_APP_

