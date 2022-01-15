/*
* Copyright 2020-2021 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/


/****************************************************************************************/
/*                                                                                      */
/*  User Selections : to be adapted per platform integration                            */
/*                                                                                      */
/****************************************************************************************/

// Select core targeted
//#define PLATFORM_RT600
#define PLATFORM_RT500
//#define PLATFORM_RT1060
//#define PLATFORM_RT1170
//#define PLATFORM_WINDOWS


// Way to reserve memory, if defined then Malloc() allocation else pre-reserved buffer (static allocation) 
//#define MEMORY_MALLOC


// PCM input file
// The data format should be aligned with VIT interface (16kHz, 16-bit, number of channel) 
#define INPUT_FILE                   "../../../_INPUT/HEYNXP_WW_CMD.pcm"

#ifndef MEMORY_MALLOC
// STATIC ALLOCATION 
// Memory region size for VIT must be filled by user :
//     The different size below are provided as example and do not correspond to the optimal size for the current model
//     Each VIT_Model will require different memory sizes (dependent on WakeWord, number of Voice CMDs, Model running from ROM or RAM....) 
//     The different memory region size can be gathered from VIT_GetMemoryTable() (1st run with putting breakpoint after VIT_GetMemoryTable)

#define MEMREGION_SLOW_DATA_SIZE   2*172230
#define MEMREGION_FAST_DATA_SIZE   2*428662
#define MEMREGION_COEF_DATA_SIZE   2*8822
#define MEMREGION_TEMP_DATA_SIZE   2*12967

#endif


// Configurations below are provided as examples of possible VIT configurations
// Configuration should be adapted to number of MICs supported and VIT features targeted.
// If AFE is enable  : only 2 or 3 channels input is supported
// If AFE is disable : only 1 channel input is supported
// see VIT.h for further information on VIT configurations : section "Valid VIT Configurations"
#ifdef PLATFORM_RT1060
    #include "PL_platformTypes_CortexM7.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT1060
    // Configuration : VIT lib integrating AFE - AFE selected - 2 Mics enabled
    #define VIT_OPERATING_MODE          VIT_ALL_MODULE_ENABLE    // LPVAD + AFE + WakeWord + Voice Commands
    #define NUMBER_OF_CHANNELS          _2CHAN
    #define VIT_MIC1_MIC2_DISTANCE      63                       // Distance between MIC2 and the reference MIC in mm
    #define VIT_MIC1_MIC3_DISTANCE      0                        // Distance between MIC3 and the reference MIC in mm
#elif defined PLATFORM_RT1170
    #include "PL_platformTypes_CortexM7.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT1170
    // Configuration : VIT lib integrating AFE - AFE selected  - 3Mics enabled
    #define VIT_OPERATING_MODE          VIT_ALL_MODULE_ENABLE    // LPVAD + AFE + WakeWord + Voice Commands
    #define NUMBER_OF_CHANNELS          _3CHAN
    #define VIT_MIC1_MIC2_DISTANCE      63                       // Distance between MIC2 and the reference MIC in mm
    #define VIT_MIC1_MIC3_DISTANCE      63                       // Distance between MIC3 and the reference MIC in mm
#elif defined  PLATFORM_RT600
    #include "PL_platformTypes_HIFI4_FUSIONF1.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_RAM
    #define DEVICE_ID                   VIT_IMXRT600
    // Configuration : VIT lib integrating AFE - AFE selected - 2 enabled
    #define VIT_OPERATING_MODE          VIT_ALL_MODULE_ENABLE    // LPVAD + AFE + WakeWord + Voice Commands
    #define NUMBER_OF_CHANNELS          _2CHAN
    #define VIT_MIC1_MIC2_DISTANCE      95                       // Distance between MIC2 and the reference MIC in mm
    #define VIT_MIC1_MIC3_DISTANCE      0                        // Distance between MIC3 and the reference MIC in mm
#elif defined  PLATFORM_RT500
    #include "PL_platformTypes_HIFI4_FUSIONF1.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_RAM
    #define DEVICE_ID                   VIT_IMXRT500
    // Configuration : VIT lib not integrating AFE - only 1Mic supported
    #define VIT_OPERATING_MODE          VIT_LPVAD_ENABLE | VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
    #define NUMBER_OF_CHANNELS          _1CHAN
    #define VIT_MIC1_MIC2_DISTANCE      0 
    #define VIT_MIC1_MIC3_DISTANCE      0
#elif defined  PLATFORM_WINDOWS
#include "PL_platformTypes_windows.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define VIT_OPERATING_MODE          VIT_LPVAD_ENABLE | VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
    #define NUMBER_OF_CHANNELS          _1CHAN
    #define VIT_MIC1_MIC2_DISTANCE      0
    #define VIT_MIC1_MIC3_DISTANCE      0

#else
    #error "No platform selected"
#endif


/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VIT.h"
#include "VIT_Model_en.h"


/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
/*
*   Useful Definitions : not to be changed
*/
// MEMORY
#define MEMORY_ALIGNMENT             8     // in bytes


/****************************************************************************************/
/*                                                                                      */
/*  Local variables                                                                     */
/*                                                                                      */
/****************************************************************************************/

// To manage the input data from the file
PL_UINT16  InputRawData[VIT_SAMPLES_PER_FRAME*NUMBER_OF_CHANNELS];           // data from file
PL_INT16   DeInterleavedBuffer[VIT_SAMPLES_PER_FRAME*NUMBER_OF_CHANNELS];    // data from file de-Interleaved


// VIT Buffers declaration in case of static allocation
#ifndef MEMORY_MALLOC
// To be allocated in the fastest memory of the platform (when possible)
PL_MEM_ALIGN(PL_UINT8 VIT_FastData[MEMREGION_FAST_DATA_SIZE], MEMORY_ALIGNMENT);

// To be allocated in the standard/slow memory of the platform
PL_MEM_ALIGN(PL_UINT8 VIT_SlowData[MEMREGION_SLOW_DATA_SIZE], MEMORY_ALIGNMENT);
PL_MEM_ALIGN(PL_UINT8 VIT_CoefData[MEMREGION_COEF_DATA_SIZE], MEMORY_ALIGNMENT);
PL_MEM_ALIGN(PL_UINT8 VIT_TempData[MEMREGION_TEMP_DATA_SIZE], MEMORY_ALIGNMENT);
#endif

/****************************************************************************************/
/*                                                                                      */
/*  Functions                                                                            */
/*                                                                                      */
/****************************************************************************************/
void DeInterleave(const PL_INT16 *pDataInput, PL_INT16 *pDataOutput, PL_UINT16 FrameSize, PL_UINT16 ChannelNumber);


/****************************************************************************************/
/*                                                                                      */
/*  Application code                                                                    */
/*                                                                                      */
/****************************************************************************************/
PL_INT32 main(void)
{
    // Audio File management
    FILE                      *INFILE = NULL;

    // VIT
    VIT_Handle_t              VITHandle = PL_NULL;                      // VIT handle pointer
    VIT_InstanceParams_st     VITInstParams;                            // VIT instance parameters structure
    VIT_ControlParams_st      VITControlParams;                         // VIT control parameters structure
    PL_MemoryTable_st         VITMemoryTable;                           // VIT memory table descriptor
    VIT_ReturnStatus_en       Status;                                   // Status of the function
    VIT_VoiceCommand_st        VoiceCommand;                             // Voice Command id
    VIT_DetectionStatus_en    VIT_DetectionResults = VIT_NO_DETECTION;  // VIT detection result
    VIT_DataIn_st             VIT_InputBuffers     = { PL_NULL, PL_NULL, PL_NULL };  // Resetting Input Buffer addresses provided to VIT_process() API
    PL_INT16                  *VIT_InputData;    

    // General
    PL_UINT32                 i, frameCnt;
    PL_UINT16                 InputRawData_Read_Number;                 // number of raw data sample read in the input file
    PL_UINT8                  MainLoop_Flag = 1;                        // main loop flag. Loop enable until value = 1
    PL_UINT16                 FrameSize = 0;
    PL_BOOL                   InitPhase_Error = PL_FALSE;
    VIT_StatusParams_st       VIT_StatusParams_Buffer;


    // Memory management
#ifdef MEMORY_MALLOC
    PL_INT8                   *pMemory[PL_NR_MEMORY_REGIONS];
#endif


    /*
     *   Open Input PCM File
     */

    INFILE = fopen(INPUT_FILE, "rb");

    if (INFILE == NULL)
    {
        printf("Can't open Input file : %s\n", INPUT_FILE);
        scanf("%d", &Status);
        exit(-1);
    }
    else 
    {
        printf("Input file : %s\n", INPUT_FILE);
    }



    /* 
     *   VIT Set Model : register the Model in VIT
     */
    Status = VIT_SetModel(VIT_Model_en, MODEL_LOCATION);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_SetModel error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }


    /* 
    *   VIT Get Model Info (OPTIONAL)
    *       To retrieve information on the VIT_Model registered in VIT:
    *               - Model Release Number, number of commands supported
    *               - WakeWord supported (when info is present)
    *               - list of commands (when info is present) 
    *
    */
    VIT_ModelInfo_st Model_Info;
    Status = VIT_GetModelInfo(&Model_Info);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetModelInfo error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }

    printf("VIT Model info \n");
    printf("  VIT Model Release = 0x%04x\n", Model_Info.VIT_Model_Release);
    if (Model_Info.pLanguage != PL_NULL)
    {
        printf("  Language supported : %s \n", Model_Info.pLanguage);
    }

    printf("  Number of Commands supported : %d \n", Model_Info.NbOfVoiceCmds);

    if (!Model_Info.WW_VoiceCmds_Strings)               // Check here if Model is containing WW and CMDs strings
    {
        printf("  VIT_Model integrating WakeWord and Voice Commands strings : NO\n");
    }
    else
    {
        const char* ptr;

        printf("  VIT_Model integrating WakeWord and Voice Commands strings : YES\n");
        ptr = Model_Info.pWakeWord;
        if (ptr != PL_NULL)
        {
            printf("  WakeWord supported : %s \n", ptr);
        }
        printf("  Voice commands supported : \n");
        ptr = Model_Info.pVoiceCmds_List;
        if (ptr != PL_NULL)
        {
            for (PL_UINT16 i = 0; i < Model_Info.NbOfVoiceCmds; i++)
            {
                printf("   '%s' \n", ptr);
                ptr += strlen(ptr) + 1;                 // to consider NULL char
            }
        }
    }


    /*
     *   VIT Get Library information
     */
    VIT_LibInfo_st Lib_Info;
    Status = VIT_GetLibInfo(&Lib_Info);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetLibInfo error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }
    printf("\nVIT Lib Info\n");
    printf(" VIT LIB Release   = 0x%04x\n", Lib_Info.VIT_LIB_Release);
    printf(" VIT Features supported by the lib       = 0x%04x\n", Lib_Info.VIT_Features_Supported);
    printf(" Number of channels supported by VIT lib = %d\n", Lib_Info.NumberOfChannels_Supported);
    if (Lib_Info.WakeWord_In_Text2Model)
    {
        printf(" VIT WakeWord in Text2Model\n\n ");
    }
    else
    {
        printf(" VIT WakeWord in Audio2Model\n\n ");
    }


    /*
     *   Configure VIT Instance Parameters
     */
    // Check that NUMBER_OF_CHANNELS is supported by VIT
    // Retrieve from VIT_GetLibInfo API the number of channel supported by the VIT lib
    PL_UINT16 max_nb_of_Channels = Lib_Info.NumberOfChannels_Supported ;
    if (NUMBER_OF_CHANNELS > max_nb_of_Channels)
    {
        printf("VIT lib is supporting only : %d channels\n", max_nb_of_Channels);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }
    VITInstParams.SampleRate_Hz            = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame          = VIT_SAMPLES_PER_FRAME;
    VITInstParams.NumberOfChannel          = NUMBER_OF_CHANNELS;
    VITInstParams.DeviceId                 = DEVICE_ID;


    /* 
     *   VIT get memory table : Get size info per memory type
     */
    Status = VIT_GetMemoryTable( PL_NULL,                // VITHandle param should be NULL 
                                 &VITMemoryTable,
                                 &VITInstParams);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetMemoryTable error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }


    /* 
     *   Reserve memory space : Malloc for each memory type
     */ 
#ifdef MEMORY_MALLOC // way to manage memory is with malloc
    for (i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        printf("Memory region %d, size %d in Bytes\n", (int)i, (int)VITMemoryTable.Region[i].Size);
        if (VITMemoryTable.Region[i].Size != 0)
        {
            // reserve memory space 
            // NB : VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be allocated 
            //      in the fatest memory of the platform (when possible) - this is not the case in this example.
            pMemory[i] = malloc(VITMemoryTable.Region[i].Size + MEMORY_ALIGNMENT);
            VITMemoryTable.Region[i].pBaseAddress = (void *)pMemory[i];

            printf(" Memory region address %p\n", VITMemoryTable.Region[i].pBaseAddress);
        }
    }

#else  // way to manage memory is with pre-reserved buffer
    
    // PL_MEMREGION_PERSISTENT_SLOW_DATA memory allocation
    if (VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_SLOW_DATA].Size <= sizeof(VIT_SlowData))                 // if required size is < than pre reserved size
    {
        VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_SLOW_DATA].pBaseAddress = (void *)&VIT_SlowData[0];      // memory allocation to pre reserved buffer
        printf("Memory region PERSISTENT_SLOW_DATA, size %d in Bytes\n", (int)VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_SLOW_DATA].Size);
    }
    else
    {
        printf("Memory region error, not enough place in reserved buffer\n");
    }

    // PL_MEMREGION_PERSISTENT_FAST_DATA memory allocation
    if (VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA].Size <= sizeof(VIT_FastData))                 // if required size is < than pre reserved size
    {
        VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA].pBaseAddress = (void *)&VIT_FastData[0];      // memory allocation to pre reserved buffer
        printf("Memory region PERSISTENT_FAST_DATA, size %d in Bytes\n", (int)VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA].Size);
    }
    else
    {
        printf("Memory region error, not enough place in reserved buffer\n");
    }

    // PL_MEMREGION_PERSISTENT_COEF memory allocation
    if (VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_COEF].Size <= sizeof(VIT_CoefData))                      // if required size is < than pre reserved size
    {
        VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_COEF].pBaseAddress = (void *)&VIT_CoefData[0];           // memory allocation to pre reserved buffer
        printf("Memory region PERSISTENT_FAST_COEF, size %d in Bytes\n", (int)VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_COEF].Size);
    }
    else
    {
        printf("Memory region error, not enough place in reserved buffer\n");
    }

    // PL_MEMREGION_TEMPORARY memory allocation
    if (VITMemoryTable.Region[PL_MEMREGION_TEMPORARY].Size <= sizeof(VIT_TempData))                            // if required size is < than pre reserved size
    {
        VITMemoryTable.Region[PL_MEMREGION_TEMPORARY].pBaseAddress = (void *)&VIT_TempData[0];                 // memory allocation to pre reserved buffer
        printf("Memory region TEMPORARY_FAST, size %d in Bytes\n", (int)VITMemoryTable.Region[PL_MEMREGION_TEMPORARY].Size);
    }
    else
    {
        printf("Memory region error, not enough place in reserved buffer\n");
    }
#endif


    /* 
    *    Create VIT Instance
    */
    VITHandle = PL_NULL;                            // force to null address for correct memory initialization
    Status    = VIT_GetInstanceHandle(  &VITHandle,
                                        &VITMemoryTable,
                                        &VITInstParams);
    if (Status != VIT_SUCCESS)
    {
        InitPhase_Error = PL_TRUE;
        printf("VIT_GetInstanceHandle error : %d\n", Status);
    }



    /* 
    *    Test the reset (OPTIONAL)
    */
    if (!InitPhase_Error)
    {
        Status = VIT_ResetInstance(VITHandle);
        if (Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            printf("VIT_ResetInstance error : %d\n", Status);
        }
    }
    

    /* 
    *   Set and Apply VIT control parameters
    */
    VITControlParams.OperatingMode      = VIT_OPERATING_MODE;
    VITControlParams.MIC1_MIC2_Distance = VIT_MIC1_MIC2_DISTANCE;
    VITControlParams.MIC1_MIC3_Distance = VIT_MIC1_MIC3_DISTANCE;

    if (!InitPhase_Error)
    {
        Status = VIT_SetControlParameters(  VITHandle,
                                            &VITControlParams);
        if (Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            printf("VIT_SetControlParameters error : %d\n", Status);
        }
    }

    {
            //Public call to VIT_GetStatusParameters
            VIT_StatusParams_st*       pVIT_StatusParam_Buffer = (VIT_StatusParams_st*)&VIT_StatusParams_Buffer;

            VIT_GetStatusParameters(VITHandle, pVIT_StatusParam_Buffer, sizeof(VIT_StatusParams_Buffer));
            printf("\nVIT Status Params\n");
            printf(" VIT LIB Release   = 0x%04x\n", pVIT_StatusParam_Buffer->VIT_LIB_Release);
            printf(" VIT Model Release = 0x%04x\n", pVIT_StatusParam_Buffer->VIT_MODEL_Release);
            printf(" VIT Features supported by the lib = 0x%04x\n", pVIT_StatusParam_Buffer->VIT_Features_Supported);
            printf(" VIT Features Selected             = 0x%04x\n", pVIT_StatusParam_Buffer->VIT_Features_Selected);
            printf(" Number of channels supported by VIT lib = %d\n", pVIT_StatusParam_Buffer->NumberOfChannels_Supported);
            printf(" Number of channels selected             = %d\n", pVIT_StatusParam_Buffer->NumberOfChannels_Selected);
            printf(" Device Selected : device id = %d\n", pVIT_StatusParam_Buffer->Device_Selected);
            if (pVIT_StatusParam_Buffer->WakeWord_In_Text2Model)
            {
                printf(" VIT WakeWord in Text2Model\n ");
            }
            else
            {
                printf(" VIT WakeWord in Audio2Model\n ");
            }
        }


    /*
    * Loop until whole file is processed
    */
    frameCnt = 1;
    printf("VIT Main Loop Start\n");

    if (InitPhase_Error)
        MainLoop_Flag = 0;             // will not process VIT but go directly to MEM free

    while (MainLoop_Flag == 1)
    {
        printf("\r%d", frameCnt);     // Animation during processing
        
        FrameSize = VIT_SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS;
        InputRawData_Read_Number = (short) fread(   &InputRawData,              // read data 1 full frame
                                                    sizeof(short),
                                                    (size_t)FrameSize,
                                                    INFILE);

        if (InputRawData_Read_Number < FrameSize)                               // if frame is shorter than the frame size
        {
            for (i = InputRawData_Read_Number; i<FrameSize; i++)                // set missing samples to 0
            {
                InputRawData[i] = 0;
            }
            MainLoop_Flag = 0;                                                  // Last frame to be read, stop read loop
        }


        /*
        *   VIT Process
        */
        // Current VIT library is supporting from 1 to 3 channels
        // VIT_InputBuffers.pBuffer_Chan1/2/3 should be set to the input buffer addresses
        // VIT_InputBuffers.pBuffer_Chan1/2/3 setting can be done out of the while loop
        // Application should take care of the ping pong buffers (when present) handling - no pingpong buffer in this example app.   

        // Convert from float interleaved to float non-interleaved (Multichannel case)
        if (VITInstParams.NumberOfChannel > _1CHAN)
        {
            // De-Interleaved input streams to VIT_LargeInputBuffer
            // In place processing not supported 
            DeInterleave(InputRawData, DeInterleavedBuffer, VITInstParams.SamplesPerFrame, VITInstParams.NumberOfChannel);
            VIT_InputData = DeInterleavedBuffer;      //Configure address of VIT_InputBuffer
        }
        else {
            VIT_InputData = InputRawData;             //Configure address of VIT_InputBuffer
        }


        switch (VITInstParams.NumberOfChannel)
        {
        case _1CHAN:
                VIT_InputBuffers.pBuffer_Chan1 = &VIT_InputData[0];
                VIT_InputBuffers.pBuffer_Chan2 = PL_NULL;
                VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
                break;

        case _2CHAN:
                VIT_InputBuffers.pBuffer_Chan1 = &VIT_InputData[0];
                VIT_InputBuffers.pBuffer_Chan2 = &VIT_InputData[VIT_SAMPLES_PER_FRAME];
                VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
                break;

        case _3CHAN:
                VIT_InputBuffers.pBuffer_Chan1 = &VIT_InputData[0];
                VIT_InputBuffers.pBuffer_Chan2 = &VIT_InputData[VIT_SAMPLES_PER_FRAME];
                VIT_InputBuffers.pBuffer_Chan3 = &VIT_InputData[VIT_SAMPLES_PER_FRAME*2];
        }
        

        Status = VIT_Process ( VITHandle,
                               &VIT_InputBuffers,                               // temporal audio input data
                               &VIT_DetectionResults
                              );

        if (Status != VIT_SUCCESS)
        {
            printf("VIT_Process error : %d\n", Status);
            MainLoop_Flag = 0;                                                  // will stop processing VIT and go directly to MEM free
        }


        if (VIT_DetectionResults == VIT_WW_DETECTED)
        {
            printf(" - WakeWord detected \n");
        }
        else if (VIT_DetectionResults == VIT_VC_DETECTED)
        {
            // Retrieve id of the Voice Command detected
            // String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model)
            Status = VIT_GetVoiceCommandFound( VITHandle,
                                               &VoiceCommand
                                             );
            if (Status != VIT_SUCCESS)
            {
                printf("VIT_GetVoiceCommandFound error : %d\n", Status);
                MainLoop_Flag = 0;                                              // will stop processing VIT and go directly to MEM free
            }
            else
            {
                printf(" - Voice Command detected %d", VoiceCommand.Cmd_Id);

                // Retrieve CMD Name : OPTIONAL
                // Check first if CMD string is present
                if (VoiceCommand.pCmd_Name != PL_NULL)
                {
                    printf(" %s\n", VoiceCommand.pCmd_Name);
                }
            }
        }
        
        frameCnt++;
    } // end while MainLoop_Flag == 1

    printf(" frames processesd \n"); // Animation during processing end


    /*  
     *   Close files
     */
     if (INFILE != PL_NULL)
        fclose(INFILE);

 
    /*
     *   Free memory for each region
     */
#ifdef MEMORY_MALLOC
     // retrieve size of the different MEM tables allocated
    Status = VIT_GetMemoryTable(   VITHandle,                // Should provide VIT_Handle to retrieve the size of the different MemTabs
                                   &VITMemoryTable,
                                   &VITInstParams);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetMemoryTable error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);
    }

    // Free the MEM tables
    for (i = 0; i<PL_NR_MEMORY_REGIONS; i++)
    {
        if (VITMemoryTable.Region[i].Size != 0)
        {
            free((PL_INT8 *)pMemory[i]);
        }
    }
#endif
    
    /* end */
    printf("Press a key + ENTER to close: \n\n");
    scanf("%d", &Status);

    return 1;
}


/****************************************************************************************/
/*                                                                                      */
/*                          Functions                                                   */
/*                                                                                      */
/****************************************************************************************/
//  de-Interleave Multichannel signal
//   example:  A1.B1.C1.A2.B2.C2.A3.B3.C3....An.Bn.Cn   (3 Channels case : A, B, C)
//             will become
//             A1.A2.A3....An.B1.B2.B3....Bn.C1.C2.C3....Cn

// Simple helper function for de-interleaving Multichannel stream
// The caller function shall ensure that all arguments are correct.
// In place processing not supported
void DeInterleave(const PL_INT16 *pDataInput, PL_INT16 *pDataOutput, PL_UINT16 FrameSize, PL_UINT16 ChannelNumber)
{

    for (PL_UINT16 ichan = 0; ichan < ChannelNumber; ichan++)
    {
        for (PL_UINT16 i = 0; i < FrameSize; i++)
        {
            pDataOutput[i+(ichan*FrameSize)] = pDataInput[(i*ChannelNumber)+ichan];
        }
    }
    return;
}




