/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <string.h>
#include <math.h>

#include "fsl_debug_console.h"

/* VIT Library, configuration */
#include "vit_proc.h"
#include "VIT_Model.h"

#include "osa_memory.h"

// VIT Lib
#define VIT_OPERATING_MODE VIT_ALL_MODULE_ENABLE // Wake Word and Voice Commands detection

#define MEMORY_ALIGNMENT 4 // in bytes
// Current VIT lib is supporting only one 1 input channel
#define NUMBER_OF_CHANNELS _1CHAN

static VIT_Handle_t VITHandle = PL_NULL;      // VIT handle pointer
static VIT_InstanceParams_st VITInstParams;   // VIT instance parameters structure
static VIT_ControlParams_st VITControlParams; // VIT control parameters structure
static PL_MemoryTable_st VITMemoryTable;      // VIT memory table descriptor
static PL_BOOL InitPhase_Error        = PL_FALSE;
static VIT_DataIn_st VIT_InputBuffers = {PL_NULL, PL_NULL,
                                         PL_NULL}; // Resetting Input Buffer addresses provided to VIT_process() API
static PL_INT8 *pMemory[PL_NR_MEMORY_REGIONS];

VIT_ReturnStatus_en VIT_ModelInfo(void)
{
    VIT_ReturnStatus_en VIT_Status; /* Function call status */
    /*
     *   VIT Get Model Info (OPTIONAL)
     *       To retrieve information on the VIT_Model registered in VIT:
     *               - Model Release Number, number of commands supported
     *               - WakeWord supported (when info is present)
     *               - list of commands (when info is present)
     *
     */
    VIT_Model_Info_t Model_Info;
    VIT_Status = VIT_GetModelInfo(&Model_Info);
    if (VIT_Status != VIT_SUCCESS)
    {
        // PRINTF("VIT_GetModelInfo error : %d\n", VIT_Status);
        return VIT_INVALID_MODEL; // We can exit from here since memory is not allocated yet
    }

    // printf("VIT Model info \n");
    // printf("  VIT Model Release = 0x%04x\n", Model_Info.VIT_Model_Release);
    // printf("  Number of Commands supported : %d \n", Model_Info.NbOfVoiceCmds);

    if (!Model_Info.WW_VoiceCmds_Strings) // Check here if Model is containing WW and CMDs strings
    {
        // printf("  VIT_Model integrating WakeWord and Voice Commands strings : NO\n");
    }
    else
    {
        const char *ptr;

        // printf("  VIT_Model integrating WakeWord and Voice Commands strings : YES\n");
        ptr = Model_Info.WakeWord;
        if (ptr != PL_NULL)
        {
            // printf("  WakeWord supported : %s \n", ptr);
        }
        // printf("  Voice commands supported : \n");
        ptr = Model_Info.VoiceCmds_List;
        if (ptr != PL_NULL)
        {
            for (PL_UINT16 i = 0; i < Model_Info.NbOfVoiceCmds; i++)
            {
                // printf("   '%s' \n", ptr);
                ptr += strlen(ptr) + 1; // to consider NULL char
            }
        }
    }
    return VIT_SUCCESS;
}

int VIT_Initialize(void *arg)
{
    VIT_ReturnStatus_en VIT_Status; /* Function call status */

    VIT_Status = VIT_SetModel(VIT_Model);
    if (VIT_Status != VIT_SUCCESS)
    {
        return VIT_Status; // We can exit from here since memory is not allocated yet
    }

    VIT_Status = VIT_ModelInfo();
    if (VIT_Status != VIT_SUCCESS)
    {
        return VIT_Status; // We can exit from here since memory is not allocated yet
    }
    /*
     *   Configure VIT Instance Parameters
     */
    VITInstParams.SampleRate_Hz   = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame = VIT_SAMPLES_PER_FRAME;
    VITInstParams.NumberOfChannel = NUMBER_OF_CHANNELS;

    /*
     *   VIT get memory table : Get size info per memory type
     */
    VIT_Status = VIT_GetMemoryTable(PL_NULL, // VITHandle param should be NULL
                                    &VITMemoryTable, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        PRINTF("VIT_GetMemoryTable error : %d\r\n", VIT_Status);
        return VIT_Status; // We can exit from here since memory is not allocated yet
    }

    /*
     *   Reserve memory space : Malloc for each memory type
     */
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        if (VITMemoryTable.Region[i].Size != 0)
        {
            // reserve memory space
            // NB : VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be alloacted
            //      in the fatest memory of the platform (when possible) - this is not the case in this example.
            pMemory[i]                            = osa_malloc(VITMemoryTable.Region[i].Size + MEMORY_ALIGNMENT);
            VITMemoryTable.Region[i].pBaseAddress = (void *)pMemory[i];
        }
    }

    /*
     *    Create VIT Instance
     */
    VITHandle  = PL_NULL; // force to null address for correct memory initialization
    VIT_Status = VIT_GetInstanceHandle(&VITHandle, &VITMemoryTable, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        InitPhase_Error = PL_TRUE;
        PRINTF("VIT_GetInstanceHandle error : %d\r\n", VIT_Status);
    }

    /*
     *    Test the reset (OPTIONAL)
     */
    if (!InitPhase_Error)
    {
        VIT_Status = VIT_ResetInstance(VITHandle);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            PRINTF("VIT_ResetInstance error : %d\r\n", VIT_Status);
        }
    }

    /*
     *   Set and Apply VIT control parameters
     */
    VITControlParams.OperatingMode = VIT_OPERATING_MODE;

    if (!InitPhase_Error)
    {
        VIT_Status = VIT_SetControlParameters(VITHandle, &VITControlParams);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            PRINTF("VIT_SetControlParameters error : %d\r\n", VIT_Status);
        }
    }

    return VIT_Status;
}

int VIT_Execute(void *arg, PL_INT16 *inputBuffer, int size)
{
    VIT_ReturnStatus_en VIT_Status;
    VIT_VoiceCommand_t VoiceCommand;                                // Voice Command id
    VIT_DetectionStatus_en VIT_DetectionResults = VIT_NO_DETECTION; // VIT detection result

    if (size != VIT_SAMPLES_PER_FRAME)
    {
        PRINTF("Input buffer format issue \r\n");
        return VIT_INVALID_FRAME_SIZE;
    }
    /*
     *   VIT Process
     */
    // Current VIT library is supporting only one channel
    // VIT_InputBuffers.pBuffer_Chan1 should be set to the input buffer address
    // VIT_InputBuffers.pBuffer_Chan1 setting can be done out of the while loop
    // Application should take care of the ping pong buffers (when present) handling - no pingpong buffer in this
    // example app.
    if (VITInstParams.NumberOfChannel == _1CHAN)
    {
        VIT_InputBuffers.pBuffer_Chan1 = inputBuffer; // PCM buffer : 16-bit - 16kHz - mono
        VIT_InputBuffers.pBuffer_Chan2 = PL_NULL;
        VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
    }
    else
    {
        PRINTF("Input buffer format issue \r\n");
        return VIT_INVALID_PARAMETER_OUTOFRANGE;
    }

    VIT_Status = VIT_Process(VITHandle,
                             &VIT_InputBuffers, // temporal audio input data
                             &VIT_DetectionResults);

    if (VIT_Status != VIT_SUCCESS)
    {
        PRINTF("VIT_Process error : %d\n", VIT_Status);
        return VIT_Status; // will stop processing VIT and go directly to MEM free
    }

    if (VIT_DetectionResults == VIT_WW_DETECTED)
    {
        PRINTF(" - WakeWord detected \r\n");
    }
    else if (VIT_DetectionResults == VIT_VC_DETECTED)
    {
        // Retrieve id of the Voice Command detected
        // String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model)
        VIT_Status = VIT_GetVoiceCommandFound(VITHandle, &VoiceCommand);
        if (VIT_Status != VIT_SUCCESS)
        {
            PRINTF("VIT_GetVoiceCommandFound error : %d\r\n", VIT_Status);
            return VIT_Status; // will stop processing VIT and go directly to MEM free
        }
        else
        {
            PRINTF(" - Voice Command detected %d", VoiceCommand.Cmd_Id);

            // Retrieve CMD Name : OPTIONAL
            // Check first if CMD string is present
            if (VoiceCommand.Cmd_Name != PL_NULL)
            {
                PRINTF(" %s\r\n", VoiceCommand.Cmd_Name);
            }
        }
    }
    return VIT_Status;
}

int VIT_Deinit(void)
{
    VIT_ReturnStatus_en VIT_Status; /* Function call status */
                                    // retrieve size of the different MEM tables allocated
    VIT_Status =
        VIT_GetMemoryTable(VITHandle, // Should provide VIT_Handle to retrieve the size of the different MemTabs
                           &VITMemoryTable, &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        PRINTF("VIT_GetMemoryTable error : %d\r\n", VIT_Status);
        return VIT_Status;
    }

    // Free the MEM tables
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        if (VITMemoryTable.Region[i].Size != 0)
        {
            osa_free((PL_INT8 *)pMemory[i]);
            pMemory[i] = NULL;
        }
    }
    return VIT_Status;
}

VIT_Initialize_T VIT_Initialize_func = VIT_Initialize;
VIT_Execute_T VIT_Execute_func       = VIT_Execute;
VIT_Deinit_T VIT_Deinit_func         = VIT_Deinit;
