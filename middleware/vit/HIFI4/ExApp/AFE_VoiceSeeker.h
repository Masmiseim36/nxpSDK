/*
* Copyright 2022 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/


#ifndef AFE_VOICESEEKER_H_
#define AFE_VOICESEEKER_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/****************************************************************************************/
/*                                                                                      */
/*  AFE VoiceSeeker                                                                     */
/*                                                                                      */
/****************************************************************************************/

#include "libVoiceSeekerLight.h"
#include "RdspTypes.h"


typedef struct {
    uint16_t VoiceSeekerEnable;
    rdsp_voiceseekerlight_config_t vsl_config;
    RETUNE_VOICESEEKERLIGHT_plugin_t vsl; // = { 0 };

    uint32_t framesize_in;          // number of input samples per channel supported by VoiceSeeker
    void* heap_memory;
    float** mic_in;
} AFE_VoiceSeeker_st;


int16_t AFE_VoiceSeeker_Init(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, uint16_t NumberOfSample,
                             uint16_t NumberOfChannel, uint16_t VoiceSeekerEnable);
int16_t AFE_VoiceSeeker_Process(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, float* VIT_InputOuputBuffer, PL_UINT32 BufferSize);
int16_t AFE_VoiceSeeker_TriggerFound(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, uint16_t Keyword_StartOffset_Samples);
int16_t AFE_VoiceSeeker_Delete(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig);

enum AFE_VOICESEEKER_RETURN_STATUS { AFE_VOICESEEKER_ERROR = -1, AFE_VOICESEEKER_EOF, AFE_VOICESEEKER_NULLADDRESS, AFE_VOICESEEKER_SUCCESS };



/****************************************************************************************/
/*                                                                                      */
/*                          VoiceSeeker Processes                                       */
/*                                                                                      */
/****************************************************************************************/

#define RDSP_ENABLE_VAD 0
#define RDSP_ENABLE_AEC 0
#define RDSP_ENABLE_DOA 0

#define RDSP_BUFFER_LENGTH_SEC 1.5f
#define RDSP_AEC_FILTER_LENGTH_MS 150
#define VAD_MEM_RESOURCE 500           //in bytes

int16_t AFE_VoiceSeeker_Init(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, uint16_t NumberOfSample,
                             uint16_t NumberOfChannel, uint16_t AFE_VoiceSeekerEnable)
{
    /*
     * Query VoiceSeeker library for the supported configuration
    */
    
    if(AFE_VoiceSeekerConfig == NULL)
        return AFE_VOICESEEKER_ERROR;

    AFE_VoiceSeekerConfig->VoiceSeekerEnable = AFE_VoiceSeekerEnable;

    if (!AFE_VoiceSeekerEnable)
        return AFE_VOICESEEKER_SUCCESS;


    rdsp_voiceseekerlight_constants_t vsl_constants;
    VoiceSeekerLight_GetConstants(&vsl_constants);


    printf("\nVoiceSeeker constants:\n");
    printf("\tmax_num_mics = %i\n", vsl_constants.max_num_mics);
    printf("\tmax_num_spks = %i\n", vsl_constants.max_num_spks);
    printf("\tsamplerate = %0.0f\n", vsl_constants.samplerate);
    printf("\tframesize_in = %i\n", vsl_constants.framesize_in);

    if (vsl_constants.max_num_mics < NumberOfChannel)
    {
        printf("Error VoiceSeeker is supporting only %d mics", vsl_constants.max_num_mics);
        return AFE_VOICESEEKER_ERROR;
    }

    /*
     * VoiceSeekerLight plugin configuration
     */
    AFE_VoiceSeekerConfig->vsl_config.num_mics = NumberOfChannel;
    AFE_VoiceSeekerConfig->vsl_config.num_spks = 0;
    AFE_VoiceSeekerConfig->vsl_config.framesize_out = NumberOfSample;
    AFE_VoiceSeekerConfig->vsl_config.create_aec = RDSP_ENABLE_AEC;
    AFE_VoiceSeekerConfig->vsl_config.create_doa = RDSP_ENABLE_DOA;
    AFE_VoiceSeekerConfig->vsl_config.buffer_length_sec = RDSP_BUFFER_LENGTH_SEC;
    AFE_VoiceSeekerConfig->vsl_config.aec_filter_length_ms = RDSP_AEC_FILTER_LENGTH_MS;

    // Specify mic geometry
    AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm = (rdsp_xyz_t*)malloc(sizeof(rdsp_xyz_t) * AFE_VoiceSeekerConfig->vsl_config.num_mics);
    if (AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm == NULL) {
        return AFE_VOICESEEKER_ERROR;
    }

    // mic0 xyz
    if (AFE_VoiceSeekerConfig->vsl_config.num_mics > 0) {
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[0][0] = 0.0f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[0][1] = 53.0f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[0][2] = 0.0f;
    }
    // mic1 xyz
    if (AFE_VoiceSeekerConfig->vsl_config.num_mics > 1) {
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[1][0] = -30.5f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[1][1] = 0.0f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[1][2] = 0.0f;
    }
    // mic2 xyz
    if (AFE_VoiceSeekerConfig->vsl_config.num_mics > 2) {
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[2][0] = 30.5f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[2][1] = 0.0f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[2][2] = 0.0f;
    }
    // mic3 xyz
    if (AFE_VoiceSeekerConfig->vsl_config.num_mics > 3) {
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[3][0] = 0.0f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[3][1] = -22.5f;
        AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm[3][2] = 0.0f;
    }


    /*
     * Query how much heap memory is required for the configuration
     */
    uint32_t heap_req_bytes = VoiceSeekerLight_GetRequiredHeapMemoryBytes(&AFE_VoiceSeekerConfig->vsl, &AFE_VoiceSeekerConfig->vsl_config);
    heap_req_bytes += VAD_MEM_RESOURCE;
    printf("VoiceSeekerLight_GetRequiredHeapMemoryBytes: %i bytes\n", heap_req_bytes);

    // Allocate needed memory
    AFE_VoiceSeekerConfig->heap_memory = malloc(heap_req_bytes);
    AFE_VoiceSeekerConfig->vsl.mem.pPrivateDataBase = AFE_VoiceSeekerConfig->heap_memory;
    AFE_VoiceSeekerConfig->vsl.mem.pPrivateDataNext = AFE_VoiceSeekerConfig->heap_memory;
    AFE_VoiceSeekerConfig->vsl.mem.FreePrivateDataSize = heap_req_bytes;

    /*
     * Create VoiceSeekerLight
     */
    RdspStatus voiceseeker_status = VoiceSeekerLight_Create(&AFE_VoiceSeekerConfig->vsl, &AFE_VoiceSeekerConfig->vsl_config);
    printf("VoiceSeekerLight_Create: voiceseeker_status = %d\n", voiceseeker_status);
    if (voiceseeker_status != OK) {
        printf("error during VoiceSeeker creation");
        return AFE_VOICESEEKER_ERROR;
    }


     /*
      * Initialize VoiceSeekerLight
      */
    VoiceSeekerLight_Init(&AFE_VoiceSeekerConfig->vsl);

    /*
     * VoiceSeekerLight Version and configuration
     */
    rdsp_voiceseekerlight_ver_struct_t vsl_version;
    VoiceSeekerLight_GetLibVersion(&AFE_VoiceSeekerConfig->vsl, &vsl_version.major, &vsl_version.minor, &vsl_version.patch);
    printf("VoiceSeekerLight_GetLibVersion: v%i.%i.%i\n", vsl_version.major, vsl_version.minor, vsl_version.patch);

    VoiceSeekerLight_PrintConfig(&AFE_VoiceSeekerConfig->vsl);

    // Unpack configuration
    AFE_VoiceSeekerConfig->framesize_in = vsl_constants.framesize_in;
    uint32_t framesize_out = AFE_VoiceSeekerConfig->vsl_config.framesize_out;
    uint32_t num_mics = AFE_VoiceSeekerConfig->vsl_config.num_mics;

#if RDSP_ENABLE_VAD==1
    /*
     * Create and initialize VAD
     */
    voiceseeker_status = VoiceSeekerLight_Vad_Create(&AFE_VoiceSeekerConfig->vsl);
    printf("VoiceSeekerLight_Vad_Create: voiceseeker_status = %d\n", voiceseeker_status);
    if (voiceseeker_status != OK) {
        printf("error during VoiceSeeker VAD creation");
        return voiceseeker_status;
    }
    VoiceSeekerLight_Vad_Init(&AFE_VoiceSeekerConfig->vsl);
#endif


    /*
     * Map mic_in pointers to mic buffer
     */
     // Check right config of VoiceSeeker in and out buffers
    if ((framesize_out % AFE_VoiceSeekerConfig->framesize_in) != 0)
        printf("VoiceSeeker in = %d not a modulo of VoiceSeeker out = %d", AFE_VoiceSeekerConfig->framesize_in, framesize_out);

    // allocate with the Number of channels of the file
    AFE_VoiceSeekerConfig->mic_in = (float**)malloc(sizeof(float*) * AFE_VoiceSeekerConfig->vsl_config.num_mics);

    /*
     * Print VoiceSeekerLight memory overview
     */
    VoiceSeekerLight_PrintMemOverview(&AFE_VoiceSeekerConfig->vsl);

    return AFE_VOICESEEKER_SUCCESS;
}



int16_t AFE_VoiceSeeker_Process(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, float* InputOuputBuffer, PL_UINT32 BufferSize)
{
    /*
     * Run VoiceSeeker Light process loop
     */
    if ((AFE_VoiceSeekerConfig == NULL) || (InputOuputBuffer == NULL))
        return AFE_VOICESEEKER_ERROR;

    if (!AFE_VoiceSeekerConfig->VoiceSeekerEnable)
        return AFE_VOICESEEKER_SUCCESS;


    /*
     * VoiceSeeker Light process
     */
    uint32_t framesize_out = AFE_VoiceSeekerConfig->vsl_config.framesize_out;       // VoiceSeeker output 
    uint32_t framesize_in_mic = AFE_VoiceSeekerConfig->vsl_config.framesize_out;    // Number of samples received from the serial port
    float VoiceSeekerIteration = (float)framesize_out / (float)AFE_VoiceSeekerConfig->framesize_in;
    float* vsl_out = NULL;

    if (BufferSize < (framesize_in_mic * AFE_VoiceSeekerConfig->vsl_config.num_mics * sizeof(float)))
        return AFE_VOICESEEKER_ERROR;

    // Initialize multichannel input pointers
    for (uint32_t imic = 0; imic < AFE_VoiceSeekerConfig->vsl_config.num_mics; imic++) {
        AFE_VoiceSeekerConfig->mic_in[imic] = &InputOuputBuffer[imic* framesize_in_mic];  
    }

#if RDSP_ENABLE_VAD==1
    /*
        * VAD process
        */
    int32_t vad_result = VoiceSeekerLight_Vad_Process(&AFE_VoiceSeekerConfig->vsl, AFE_VoiceSeekerConfig->mic_in[0]);
    if (vad_result == 1) {
        printf("VAD detection\n");
    }
#endif

    RdspStatus voiceseeker_status;
    while (VoiceSeekerIteration)
    {

        voiceseeker_status = VoiceSeekerLight_Process(&AFE_VoiceSeekerConfig->vsl, AFE_VoiceSeekerConfig->mic_in, NULL, &vsl_out);
        if (voiceseeker_status != OK) {
            printf("VoiceSeekerLight_Process: voiceseeker_status = %d\n", (int)voiceseeker_status);
            return AFE_VOICESEEKER_ERROR;
        }
        // Update multichannel input pointers
        // input pointers incremented by Voiceseeker framesize_in (i.e +32 float*)
        for (uint32_t imic = 0; imic < AFE_VoiceSeekerConfig->vsl_config.num_mics; imic++) {
            AFE_VoiceSeekerConfig->mic_in[imic] += AFE_VoiceSeekerConfig->framesize_in;      // increase address by VoiceSeeker input size i.e 32 samples
        }
        VoiceSeekerIteration--;
    }

    if (vsl_out != NULL)
    {      
        // Copy VoiceSeeker output to VIT Input buffer
        memcpy(InputOuputBuffer, vsl_out, framesize_out*sizeof(float));
    }
    else 
        printf("wrong config between VoiceSeeker input = %zd and out = %d samples.", AFE_VoiceSeekerConfig->framesize_in, framesize_out);



    return AFE_VOICESEEKER_SUCCESS;
}


int16_t AFE_VoiceSeeker_TriggerFound(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig, uint16_t Keyword_StartOffset_Samples)
{

    /*
     * Inform VoiceSeekerLight upon a trigger event
     */
    if (AFE_VoiceSeekerConfig == NULL)
        return AFE_VOICESEEKER_ERROR;

    if (!AFE_VoiceSeekerConfig->VoiceSeekerEnable)
        return AFE_VOICESEEKER_SUCCESS;

    VoiceSeekerLight_TriggerFound(&AFE_VoiceSeekerConfig->vsl, Keyword_StartOffset_Samples);

    return AFE_VOICESEEKER_SUCCESS;
}



int16_t AFE_VoiceSeeker_Delete(AFE_VoiceSeeker_st* AFE_VoiceSeekerConfig)
{
    
    if (AFE_VoiceSeekerConfig == NULL)
        return AFE_VOICESEEKER_ERROR;

    if (!AFE_VoiceSeekerConfig->VoiceSeekerEnable)
        return AFE_VOICESEEKER_SUCCESS;

    VoiceSeekerLight_Destroy(&AFE_VoiceSeekerConfig->vsl);

    if (AFE_VoiceSeekerConfig->mic_in != NULL)
        free(AFE_VoiceSeekerConfig->mic_in);


#if RDSP_ENABLE_VAD==1
    /*
     * Destroy VAD
     */
    VoiceSeekerLight_Vad_Destroy(&AFE_VoiceSeekerConfig->vsl);
#endif

    // Free microphone geometry
    free(AFE_VoiceSeekerConfig->vsl_config.mic_xyz_mm);

    // Free VoiceSeeker Light App memory
    free(AFE_VoiceSeekerConfig->heap_memory);

    return AFE_VOICESEEKER_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* AFE_VOICESEEKER_H_ */


