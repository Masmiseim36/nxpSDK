/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "EAP_Parameter_AutoVolumeLeveler.h"
#include "EAP_Parameter_ConcertSound.h"
#include "EAP_Parameter_LoudnessMaximiser.h"
#include "EAP_Parameter_MusicEnhancerRMSLimiter.h"
#include "EAP_Parameter_VoiceEnhancer.h"
#include "EAP_Parameter_AllEffectOff.h"
#include "EAP_Parameter_Custom.h"
#include "EAP_Parameter_ToneGenerator.h"

#include "osa_types.h"
#include "osa_memory.h"
#include "fsl_debug_console.h"

#include "streamer_element_properties.h"

#include "eap_proc.h"

// EAP Lib
static LVM_Handle_t EAP_hInstance;              /* Instance handle */
static LVM_MemTab_t EAP_MemTab;                 /* Memory allocation table */
static LVM_VersionInfo_st EAP_VersionInfo;      /* Version info structure */
static LVM_UINT16 bEventPending = LVM_TRUE;     /* Event Pending flag  to highlight new EAP configuration is present */
static LVM_ControlParams_t *pEAP_ControlParams; /* control Parameters */
static LVM_InstParams_t *pEAP_InstParams;       /* EAP_InstParams */
static LVM_ControlParams_t EAP_ControlParams;   /* control Parameters */
static LVM_ReturnStatus_en LVM_Status;          /* Function call status */

#ifdef ALGORITHM_EQNB
static LVM_HeadroomParams_t *pEAP_HeadroomParams; /* Headroom parameters */
#endif

int EAP_Init(void *arg);
int EAP_Execute(void *arg, LVM_INT16 *inputBuffer, int size);
int EAP_Deinit();
LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(post_proc_args *args);

static LVM_UINT32 EAP_AudioTime = 0;

// malloc memory
static LVM_INT16 MallocAlign = 4; /* 4 byte Malloc alignment */

int EAP_Init(void *arg)
{
    LVM_ReturnStatus_en LVM_Status; /* Function call status */
    LVM_UINT16 i;                   /* loop index */
    LVM_INT32 temp32;               /* temporary address */

    // scratch
    LVM_INT16 *pScratchBase = LVM_NULL;
    LVM_UINT32 ScratchSize  = 0;

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
    post_proc_args *args = arg;
    char num             = args->preset_num;

    switch (num)
    {
        case '1':
            pEAP_ControlParams = &ControlParamSet_allEffectOff; /* Control Parameters */
            pEAP_InstParams    = &InstParams_allEffectOff;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_allEffectOff; /* Headroom parameters */
#endif
            break;
        case '2':
            pEAP_ControlParams = &ControlParamSet_voiceEnhancer; /* Control Parameters */
            pEAP_InstParams    = &InstParams_voiceEnhancer;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_voiceEnhancer; /* Headroom parameters */
#endif
            break;
        case '3':
            pEAP_ControlParams = &ControlParamSet_musicEnhancerRmsLimiter; /* Control Parameters */
            pEAP_InstParams    = &InstParams_musicEnhancerRmsLimiter;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_musicEnhancerRmsLimiter; /* Headroom parameters */
#endif
            break;
        case '4':
            pEAP_ControlParams = &ControlParamSet_autoVolumeLeveler; /* Control Parameters */
            pEAP_InstParams    = &InstParams_autoVolumeLeveler;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_autoVolumeLeveler; /* Headroom parameters */
#endif
            break;
        case '5':
            pEAP_ControlParams = &ControlParamSet_loudnessMaximiser; /* Control Parameters */
            pEAP_InstParams    = &InstParams_loudnessMaximiser;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_loudnessMaximiser; /* Headroom parameters */
#endif
            break;
        case '6':
            pEAP_ControlParams = &ControlParamSet_concertSound; /* Control Parameters */
            pEAP_InstParams    = &InstParams_concertSound;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_concertSound; /* Headroom parameters */
#endif
            break;
        case '7':
            pEAP_ControlParams = &ControlParamSet_custom; /* Control Parameters */
            pEAP_InstParams    = &InstParams_custom;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_custom; /* Headroom parameters */
#endif
            break;
        case '8':
            pEAP_ControlParams = &ControlParamSet_toneGenerator; /* Control Parameters */
            pEAP_InstParams    = &InstParams_toneGenerator;      /* Instance parameters */
#ifdef ALGORITHM_EQNB
            pEAP_HeadroomParams = &HeadroomParams_toneGenerator; /* Headroom parameters */
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
            EAP_MemTab.Region[i].pBaseAddress = (LVM_INT8 *)(temp32 + MallocAlign);
        }
        if (i == LVM_MEMREGION_TEMPORARY_FAST)
        {
            pScratchBase = EAP_MemTab.Region[i].pBaseAddress;
            ScratchSize  = EAP_MemTab.Region[i].Size;
        }
    }

    /******************************************************************************
    Initialize
    *******************************************************************************/
    /*
     * Get an EAP Instance
     */
    EAP_hInstance = LVM_NULL;                             /* Initialise to NULL */
    LVM_Status    = LVM_GetInstanceHandle(&EAP_hInstance, /* Init sets the instance handle */
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
    return LVM_Status;
}

int EAP_Execute(void *arg, LVM_INT16 *inputBuffer, int size)
{
    /* Function call status */
    LVM_ReturnStatus_en LVM_Status;

    int num_channel = *(int *)arg;
    if (num_channel < 1)
    {
        return LVM_OUTOFRANGE;
    }

    /* size of each sample is 2 bytes */
    size = size >> 1;

    EAP_AudioTime += LVM_FRAME_SIZE_MS;

    LVM_Status = LVM_Process(EAP_hInstance,      /* Instance handle */
                             inputBuffer,        /* Input buffer */
                             inputBuffer,        /* Output buffer - in-place processing uses the same buffer*/
                             size / num_channel, /* Number of samples to process */
                             EAP_AudioTime);     /* Audio Time*/

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

LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(post_proc_args *args)
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
    EAP_hInstance = LVM_NULL;
    return LVM_Status;
}

EAP_Initialize_T EAP_Initialize_func = EAP_Init;
EAP_Execute_T EAP_Execute_func       = EAP_Execute;
EAP_Deinit_T EAP_Deinit_func         = EAP_Deinit;
