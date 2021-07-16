/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "EAP_Parameter_AllEffectOff.h"
#include "EAP_Parameter_AutoVolumeLeveler.h"
#if (ALGORITHM_CS == 1)
#include "EAP_Parameter_ConcertSound.h"
#endif
#if (ALGORITHM_XO == 1)
#include "EAP_Parameter_Crossover2waySpeakers.h"
#include "EAP_Parameter_CrossoverForSubwoofer.h"
#endif
#ifndef CPU_LPC55S69JBD100_cm33_core0
#include "EAP_Parameter_Custom.h"
#endif
#include "EAP_Parameter_LoudnessMaximiser.h"
#ifdef CPU_LPC55S69JBD100_cm33_core0
#include "EAP_Parameter_MusicEnhancerRMSLimiter_lowMIPS.h"
#else
#include "EAP_Parameter_MusicEnhancerRMSLimiter.h"
#endif
#include "EAP_Parameter_ToneGenerator.h"
#ifdef CPU_LPC55S69JBD100_cm33_core0
#include "EAP_Parameter_VoiceEnhancer_lowMIPS.h"
#else
#include "EAP_Parameter_VoiceEnhancer.h"
#endif

#include "osa_types.h"
#include "osa_memory.h"
#include "fsl_debug_console.h"

#include "streamer_element_properties.h"

#include "eap_proc.h"

// EAP Lib
static LVM_Handle_t EAP_hInstance;              /* Instance handle */
static LVM_MemTab_t EAP_MemTab;                 /* Memory allocation table */
static LVM_VersionInfo_st EAP_VersionInfo;      /* Version info structure */
static LVM_ControlParams_t *pEAP_ControlParams; /* control Parameters */
static LVM_InstParams_t *pEAP_InstParams;       /* EAP_InstParams */
#if (ALGORITHM_XO == 1)
static LVM_INT16 *eap_xo_out_buffer[2];
#endif
static bool first_exec;

#ifdef ALGORITHM_EQNB
static LVM_HeadroomParams_t *pEAP_HeadroomParams; /* Headroom parameters */
#endif

int EAP_Init(void *arg);
int EAP_Execute(void *arg, LVM_INT16 *inputBuffer, int size);
int EAP_Deinit();
LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(ext_proc_args *args);

static LVM_UINT32 EAP_AudioTime = 0;

// malloc memory
static LVM_INT16 MallocAlign = 4; /* 4 byte Malloc alignment */

int EAP_Init(void *arg)
{
    LVM_ReturnStatus_en LVM_Status; /* Function call status */
    LVM_UINT16 i;                   /* loop index */
    LVM_INT32 temp32;               /* temporary address */

    /******************************************************************************
    GET VERSION INFORMATION
    *******************************************************************************/
    LVM_Status = LVM_GetVersionInfo(&EAP_VersionInfo);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    /******************************************************************************
    SET THE INPUT PARAMETERS
    *******************************************************************************/
    /*
     * Select parameter configuration
     */
    ext_proc_args *args = arg;
    switch (args->preset_num)
    {
        case 1:
            pEAP_ControlParams = &ControlParamSet_allEffectOff; /* Control Parameters */
            pEAP_InstParams    = &InstParams_allEffectOff;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_allEffectOff; /* Headroom parameters */
#endif
            break;
        case 2:
#ifdef EAP_PARAMETER_VOICE_ENHANCER_LOWMIPS
            pEAP_ControlParams = &ControlParamSet_voiceEnhancer_lowMIPS; /* Control Parameters */
            pEAP_InstParams    = &InstParams_voiceEnhancer_lowMIPS;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_voiceEnhancer_lowMIPS; /* Headroom parameters */
#endif
#else
            pEAP_ControlParams  = &ControlParamSet_voiceEnhancer;           /* Control Parameters */
            pEAP_InstParams     = &InstParams_voiceEnhancer;                /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_voiceEnhancer;            /* Headroom parameters */
#endif
#endif
            break;
        case 3:
#ifdef EAP_PARAMETER_MUSIC_ENHANCER_LOWMIPS
            pEAP_ControlParams = &ControlParamSet_musicEnhancerRmsLimiter_lowMIPS; /* Control Parameters */
            pEAP_InstParams    = &InstParams_musicEnhancerRmsLimiter_lowMIPS;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_musicEnhancerRmsLimiter_lowMIPS; /* Headroom parameters */
#endif
#else
            pEAP_ControlParams  = &ControlParamSet_musicEnhancerRmsLimiter; /* Control Parameters */
            pEAP_InstParams     = &InstParams_musicEnhancerRmsLimiter;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_musicEnhancerRmsLimiter;  /* Headroom parameters */
#endif
#endif
            break;
        case 4:
            pEAP_ControlParams = &ControlParamSet_autoVolumeLeveler; /* Control Parameters */
            pEAP_InstParams    = &InstParams_autoVolumeLeveler;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_autoVolumeLeveler; /* Headroom parameters */
#endif
            break;
        case 5:
            pEAP_ControlParams = &ControlParamSet_loudnessMaximiser; /* Control Parameters */
            pEAP_InstParams    = &InstParams_loudnessMaximiser;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_loudnessMaximiser; /* Headroom parameters */
#endif
            break;
#if (ALGORITHM_CS == 1)
        case 6:
            pEAP_ControlParams = &ControlParamSet_concertSound; /* Control Parameters */
            pEAP_InstParams    = &InstParams_concertSound;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_concertSound; /* Headroom parameters */
#endif
            break;
#endif
#ifndef CPU_LPC55S69JBD100_cm33_core0
        case 7:
            pEAP_ControlParams = &ControlParamSet_custom; /* Control Parameters */
            pEAP_InstParams    = &InstParams_custom;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_custom; /* Headroom parameters */
#endif
            break;
#endif
        case 8:
            pEAP_ControlParams = &ControlParamSet_toneGenerator; /* Control Parameters */
            pEAP_InstParams    = &InstParams_toneGenerator;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_toneGenerator; /* Headroom parameters */
#endif
            break;
#if (ALGORITHM_XO == 1)
        case 9:
            pEAP_ControlParams = &ControlParamSet_Crossover2WaySpeaker; /* Control Parameters */
            pEAP_InstParams    = &InstParams_Crossover2WaySpeaker;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_Crossover2WaySpeaker; /* Headroom parameters */
#endif
            break;
        case 10:
            pEAP_ControlParams = &ControlParamSet_CrossoverForSubwoofer; /* Control Parameters */
            pEAP_InstParams    = &InstParams_CrossoverForSubwoofer;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_CrossoverForSubwoofer; /* Headroom parameters */
#endif
#endif
            break;
        default:
            pEAP_ControlParams = &ControlParamSet_allEffectOff; /* Control Parameters */
            pEAP_InstParams    = &InstParams_allEffectOff;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_allEffectOff; /* Headroom parameters */
#endif
            break;
    }

    LVM_Status = EAP_SetSampleRateAndNumOfChannels(args);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    /* EAP max block size needs to be adjusted to the real obtained buffer size */
    pEAP_InstParams->MaxBlockSize = MAX_SAMPLE_SIZE;

    /******************************************************************************
    Allocate memory
    Force alignment by allocating extra memory
    *******************************************************************************/
    LVM_Status = LVM_GetMemoryTable(LVM_NULL, &EAP_MemTab, pEAP_InstParams);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    for (i = 0; i < LVM_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        if (EAP_MemTab.Region[i].Size != 0)
        {
            temp32 = (LVM_INT32)osa_malloc(EAP_MemTab.Region[i].Size + (LVM_UINT32)MallocAlign);
            if (!temp32)
            {
                return LVM_NULLADDRESS;
            }
            EAP_MemTab.Region[i].pBaseAddress = (LVM_INT8 *)(temp32 + MallocAlign);
        }
    }

    /******************************************************************************
    Initialize
    *******************************************************************************/
    /*
     * Get an EAP Instance
     */
    pEAP_InstParams->Platform = PLATFORM_ID;
    EAP_hInstance             = LVM_NULL;                             /* Initialise to NULL */
    LVM_Status                = LVM_GetInstanceHandle(&EAP_hInstance, /* Init sets the instance handle */
                                       &EAP_MemTab, pEAP_InstParams);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

#ifdef ALGORITHM_EQNB
    {
        // set headroom param config
        LVM_Status = LVM_SetHeadroomParams(EAP_hInstance, pEAP_HeadroomParams);
    }
#endif

    /******************************************************************************
    Call set control parameters
     - propagate the configuration to EAP
    *******************************************************************************/
    LVM_Status = LVM_SetControlParameters(EAP_hInstance, pEAP_ControlParams);

    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }
    else
    {
        /* ready for first EAP execute */
        first_exec = true;
    }
    return LVM_Status;
}

int EAP_Execute(void *arg, LVM_INT16 *inputBuffer, int size)
{
    /* Function call status */
    LVM_ReturnStatus_en LVM_Status;
    LVM_INT16 *outBuffer[2] = {inputBuffer, NULL};

    int num_channel = *(int *)arg;
    if (num_channel < 1)
    {
        return LVM_OUTOFRANGE;
    }
#if (ALGORITHM_XO == 1)
    if (pEAP_ControlParams->XO_OperatingMode == LVM_MODE_ON && first_exec)
    {
        for (int i = 0; i < 2; i++)
        {
            eap_xo_out_buffer[i] = osa_malloc(size);
            if (eap_xo_out_buffer[i] != NULL)
            {
                outBuffer[i] = eap_xo_out_buffer[i];
                first_exec   = false;
            }
            else
            {
                return LVM_NULLADDRESS;
            }
        }
    }
    if (pEAP_ControlParams->XO_OperatingMode == LVM_MODE_ON)
    {
        outBuffer[0] = eap_xo_out_buffer[0];
        outBuffer[1] = eap_xo_out_buffer[1];
    }
#endif
    /* size of each sample is 2 bytes */
    size = size >> 1;

    EAP_AudioTime += LVM_FRAME_SIZE_MS;

    LVM_Status = LVM_Process(EAP_hInstance,      /* Instance handle */
                             inputBuffer,        /* Input buffer */
                             outBuffer,          /* Output buffer */
                             size / num_channel, /* Number of samples to process */
                             EAP_AudioTime);     /* Audio Time*/

#if (ALGORITHM_XO == 1)
    if (pEAP_ControlParams->XO_OperatingMode)
    {
        /* Interleave data - mix low and high band output of crossover (in case of stereo input take just right
         * channel)*/
        for (int i = 0; i < size / num_channel; i++)
        {
            *(inputBuffer + 2 * i)     = *(outBuffer[0] + num_channel * i);
            *(inputBuffer + 2 * i + 1) = *(outBuffer[1] + num_channel * i);
        }
    }
#endif
    /* Check for error and stop if needed */
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    return LVM_Status;
}

LVM_ReturnStatus_en EAP_SetConfig(LVM_ControlParams_t *pEAP_ControlParamsSet)
{
    /* Function call status */
    LVM_ReturnStatus_en LVM_Status;

    /******************************************************************************
    Call set control parameters
     - propagate the configuration to EAP
    *******************************************************************************/
    LVM_Status = LVM_SetControlParameters(EAP_hInstance, pEAP_ControlParamsSet);
    if (LVM_Status == LVM_NULLADDRESS)
    {
        return LVM_Status;
    }
    if (LVM_Status == LVM_OUTOFRANGE)
    {
        return LVM_Status;
    }

    return LVM_Status;
}

LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(ext_proc_args *args)
{
    LVM_ReturnStatus_en LVM_Status = LVM_SUCCESS;
    switch (args->sample_rate)
    {
        case 4000:
            LVM_Status = LVM_OUTOFRANGE;
            break;
        case 8000:
            pEAP_ControlParams->SampleRate = LVM_FS_8000;
            break;
        case 11025:
            pEAP_ControlParams->SampleRate = LVM_FS_11025;
            break;
        case 12000:
            pEAP_ControlParams->SampleRate = LVM_FS_12000;
            break;
        case 16000:
            pEAP_ControlParams->SampleRate = LVM_FS_16000;
            break;
        case 22050:
            pEAP_ControlParams->SampleRate = LVM_FS_22050;
            break;
        case 24000:
            pEAP_ControlParams->SampleRate = LVM_FS_24000;
            break;
        case 32000:
            pEAP_ControlParams->SampleRate = LVM_FS_32000;
            break;
        case 44100:
            pEAP_ControlParams->SampleRate = LVM_FS_44100;
            break;
        case 48000:
            pEAP_ControlParams->SampleRate = LVM_FS_48000;
            break;
        case 64000:
        case 88200:
        case 96000:
        case 128000:
        case 176400:
        case 192000:
            LVM_Status = LVM_OUTOFRANGE;
            break;
    }
    switch (args->num_channels)
    {
        case 1:
            pEAP_ControlParams->SourceFormat = LVM_MONO;
            break;
        case 2:
            pEAP_ControlParams->SourceFormat = LVM_STEREO;
            break;
    }
    if (LVM_Status == LVM_OUTOFRANGE)
    {
        return LVM_Status;
    }
    else
    {
        return LVM_SUCCESS;
    }
}

int EAP_Deinit(void)
{
    LVM_ReturnStatus_en LVM_Status; /* Function call status */
    LVM_UINT16 i;                   /* loop index */
    LVM_INT32 temp32;               /* temporary address */

    /*
     * Free memory
     */
    LVM_Status = LVM_GetMemoryTable(EAP_hInstance, &EAP_MemTab, LVM_NULL);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    for (i = 0; i < LVM_NR_MEMORY_REGIONS; i++)
    {
        if (EAP_MemTab.Region[i].Size != 0)
        {
            temp32 = (LVM_INT32)EAP_MemTab.Region[i].pBaseAddress - MallocAlign;
            osa_free((LVM_INT8 *)temp32);
        }
    }
#if (ALGORITHM_XO == 1)
    if (pEAP_ControlParams->XO_OperatingMode)
    {
        for (int i = 0; i < 2; i++)
        {
            osa_free(eap_xo_out_buffer[i]);
        }
    }
#endif

    EAP_hInstance = LVM_NULL;
    return LVM_Status;
}

EAP_Initialize_T EAP_Initialize_func = EAP_Init;
EAP_Execute_T EAP_Execute_func       = EAP_Execute;
EAP_Deinit_T EAP_Deinit_func         = EAP_Deinit;
