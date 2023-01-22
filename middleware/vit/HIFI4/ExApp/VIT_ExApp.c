/*
* Copyright 2020-2022 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/


/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/****************************************************************************************/
/*                                                                                      */
/*  User Selections : to be adapted per platform integration                            */
/*                                                                                      */
/****************************************************************************************/

/*    VIT Configuration      */

// Select core targeted
//#define PLATFORM_RT600
//#define PLATFORM_RT500
//#define PLATFORM_RT1040
//#define PLATFORM_RT1060
//#define PLATFORM_RT1170
//#define PLATFORM_WINDOWS

// Way to reserve memory, if defined then Malloc() allocation else pre-reserved buffer (static allocation) 
#define MEMORY_MALLOC


// PCM input file
// The data format should be aligned with VIT interface (16kHz, 16-bit, number of channel) 
#define INPUT_FILE                   "../../../../_INPUT/HEYNXP_WW_CMD.pcm" 


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
// see VIT.h for further information on VIT configurations
#ifdef PLATFORM_RT1040
    #include "PL_platformTypes_CortexM.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT1040
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif PLATFORM_RT1060
    #include "PL_platformTypes_CortexM.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT1060
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif defined PLATFORM_RT1170
    #include "PL_platformTypes_CortexM.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT1170
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif defined  PLATFORM_RT600
    #include "PL_platformTypes_HIFI4_FUSIONF1.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_RAM
    #define DEVICE_ID                   VIT_IMXRT600
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif defined  PLATFORM_RT500
    #include "PL_platformTypes_HIFI4_FUSIONF1.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_RAM
    #define DEVICE_ID                   VIT_IMXRT500
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif defined  PLATFORM_WINDOWS
    #include "PL_platformTypes_windows.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT500   //Dummy
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#elif defined  PLATFORM_LINUX
    #include "PL_platformTypes_linux.h"
    #define MODEL_LOCATION              VIT_MODEL_IN_ROM
    #define DEVICE_ID                   VIT_IMXRT500   //Dummy
    #define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE
#else
    #error "No platform selected"
#endif

// Configure the detection period in second for each command
// VIT will return UNKNOWN if no command is recognized during this time span.
#define VIT_COMMAND_TIME_SPAN           3.0 // in second


// Configure number of samples per frame read from the serial interface (MIC, File,...)
// This will correspond to the number of samples at VoiceSeeker Output and VIT Input
#define SAMPLES_PER_FRAME               VIT_SAMPLES_PER_30MS_FRAME



#include "VIT.h"
#include "VIT_Model_en.h"



/*    VoiceSeeker Configuration      */

// Enable or not VoiceSeeker Processing

#define AFE_VOICESEEKER

#ifdef AFE_VOICESEEKER
#include "AFE_VoiceSeeker.h"
#define VOICESEEKER_ENABLE          1
#define NUMBER_OF_CHANNELS          _3CHAN
#else
#define NUMBER_OF_CHANNELS          _1CHAN
#endif

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
PL_INT16  InputRawData[SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS];            // data from file
PL_INT32  TempData[SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS];                // data in Float32 or int32_t


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
    VIT_VoiceCommand_st       VoiceCommand;                             // Voice Command info
    VIT_WakeWord_st           WakeWord;                                 // Wakeword info
    VIT_DetectionStatus_en    VIT_DetectionResults = VIT_NO_DETECTION;  // VIT detection result
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
        scanf("%d", (int *)&Status);
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
        scanf("%d", (int *)&Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }


    /* 
    *   VIT Get Model Info (OPTIONAL)
    *       To retrieve information on the VIT_Model registered in VIT:
    *               - Model Release Number, number of commands supported
    *               - WakeWords supported (when info is present)
    *               - list of commands (when info is present) 
    *
    */
    VIT_ModelInfo_st Model_Info;
    Status = VIT_GetModelInfo(&Model_Info);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetModelInfo error : %d\n", Status);
        scanf("%d", (int *)&Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }

    printf("VIT Model info \n");
    printf("  VIT Model Release = 0x%04x\n", Model_Info.VIT_Model_Release);
    if (Model_Info.pLanguage != PL_NULL)
    {
        printf("  Language supported : %s \n", Model_Info.pLanguage);
    }

    printf("  Number of WakeWords supported : %d \n", Model_Info.NbOfWakeWords);
    printf("  Number of Commands supported : %d \n",  Model_Info.NbOfVoiceCmds);

    if (!Model_Info.WW_VoiceCmds_Strings)               // Check here if Model is containing WW and CMDs strings
    {
        printf("  VIT_Model integrating WakeWord and Voice Commands strings : NO\n");
    }
    else
    {
        const char* ptr;

        printf("  VIT_Model integrating WakeWord and Voice Commands strings : YES\n");
        printf("  WakeWords supported : \n");
        ptr = Model_Info.pWakeWord_List;
        if (ptr != PL_NULL)
        {
            for (PL_UINT16 i = 0; i < Model_Info.NbOfWakeWords; i++)
            {
                printf("   '%s' \n", ptr);
                ptr += strlen(ptr) + 1;                 // to consider NULL char
            }
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
        scanf("%d", (int *)&Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }
    printf("\nVIT Lib Info\n");
    printf(" VIT LIB Release   = 0x%04x\n", Lib_Info.VIT_LIB_Release);
    printf(" VIT Features supported by the lib       = 0x%04x\n", Lib_Info.VIT_Features_Supported);
    printf(" Number of channels supported by VIT lib = %d\n", Lib_Info.NumberOfChannels_Supported);


    /*
     *   Configure VIT Instance Parameters
     */
    VITInstParams.SampleRate_Hz            = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame          = SAMPLES_PER_FRAME;
    VITInstParams.NumberOfChannel          = _1CHAN;
    VITInstParams.DeviceId                 = DEVICE_ID;
    VITInstParams.APIVersion               = VIT_API_VERSION;


    /* 
     *   VIT get memory table : Get size info per memory type
     */
    Status = VIT_GetMemoryTable( PL_NULL,                // VITHandle param should be NULL 
                                 &VITMemoryTable,
                                 &VITInstParams);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetMemoryTable error : %d\n", Status);
        scanf("%d", (int *)&Status);
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
    VITControlParams.Feature_LowRes     = PL_FALSE;
    VITControlParams.Command_Time_Span  = VIT_COMMAND_TIME_SPAN;


    if (!InitPhase_Error)
    {
        Status = VIT_SetControlParameters(  VITHandle,
                                            &VITControlParams);
        if (Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
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
            printf(" Device Selected : device id = %d\n", pVIT_StatusParam_Buffer->Device_Selected);
        }

#ifdef AFE_VOICESEEKER
    AFE_VoiceSeeker_st AFE_VoiceSeekerConfig;

    if (AFE_VoiceSeeker_Init(&AFE_VoiceSeekerConfig, SAMPLES_PER_FRAME,
                             NUMBER_OF_CHANNELS, VOICESEEKER_ENABLE) != AFE_VOICESEEKER_SUCCESS)
    {
        (void)scanf("%d", (int *)&Status);
        exit(-1);
    }
#endif


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
        
        FrameSize = SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS;
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
        *   AFE VoiceSeeker Process
        */

        // Application should take care of the ping pong buffers (when present) handling - no pingpong buffer in this example app.   

        // Convert from float interleaved to float non-interleaved (Multichannel case)
        if (NUMBER_OF_CHANNELS > _1CHAN)
        {

#ifdef AFE_VOICESEEKER
            // De-Interleaved input streams to TempData
            // In place processing not supported 
            DeInterleave(InputRawData, (PL_INT16*)TempData, SAMPLES_PER_FRAME, NUMBER_OF_CHANNELS);

            PL_FLOAT *InputTempDataFloat = (float*)TempData;
            PL_INT16 *InputTempDataINT16 = (PL_INT16*)TempData;

            // Convert  INT16 Q15 data to Float data
            #define INV_32768   3.0517578125e-05f
            for (PL_UINT16 i = FrameSize; i > 0; i--)
            {
                InputTempDataFloat[i-1] = ((float)InputTempDataINT16[i-1]) * INV_32768;
            }


            // AFE VoiceSeeker Processing : in place processing
            PL_INT16 VoiceSeekerStatus = 0;
            VoiceSeekerStatus = AFE_VoiceSeeker_Process(&AFE_VoiceSeekerConfig, (float*)TempData,
                                                        sizeof(PL_FLOAT)* SAMPLES_PER_FRAME * NUMBER_OF_CHANNELS);


            if (VoiceSeekerStatus == AFE_VOICESEEKER_ERROR)
            {
                printf("VoiceSeeker Error\n");
                break;
            }
            else
            {
                // Convert VoiceSeeker output from Float data to INT16 Q15 data
                VIT_InputData = (PL_INT16*)TempData;
                for (PL_UINT16 i = 0; i < SAMPLES_PER_FRAME; i++)
                {
                    VIT_InputData[i] = (PL_INT16)((float)InputTempDataFloat[i] * 32768);
                }
            }

#else
            printf("wrong AFE VoiceSeeker configuration\n");
            break;
#endif

        }
        else 
        {
            VIT_InputData = InputRawData;             //Configure address of VIT_InputBuffer
        }

      
        /*
        *   VIT Process
        */

        Status = VIT_Process ( VITHandle,
                               (void*)VIT_InputData,                               // temporal audio input data
                               &VIT_DetectionResults
                              );

        if (Status != VIT_SUCCESS)
        {
            printf("VIT_Process error : %d\n", Status);
            break;                                                                 // will stop processing VIT and go directly to MEM free
        }


        if (VIT_DetectionResults == VIT_WW_DETECTED)
        {
            // Retrieve id of the WakeWord detected
            // String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model)
            Status = VIT_GetWakeWordFound( VITHandle,
                                           &WakeWord
                                         );
            if (Status != VIT_SUCCESS)
            {
                printf("VIT_GetWakeWordFound error : %d\n", Status);
                break;                                                             // will stop processing VIT and go directly to MEM free
            }
            else
            {
                printf(" - WakeWord detected %d", WakeWord.Id);

                // Retrieve WakeWord Name : OPTIONAL
                // Check first if WakeWord string is present
                if (WakeWord.pName != PL_NULL)
                {
                    printf(" %s\n", WakeWord.pName);
                }

#ifdef AFE_VOICESEEKER
                AFE_VoiceSeeker_TriggerFound(&AFE_VoiceSeekerConfig, WakeWord.StartOffset);
#endif
            }
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
                break;                                                               // will stop processing VIT and go directly to MEM free
            }
            else
            {
                printf(" - Voice Command detected %d", VoiceCommand.Id);

                // Retrieve CMD Name : OPTIONAL
                // Check first if CMD string is present
                if (VoiceCommand.pName != PL_NULL)
                {
                    printf(" %s\n", VoiceCommand.pName);
                }
            }
        }
        
        frameCnt++;
    } // end while MainLoop_Flag == 1

    printf(" frames processed \n"); // Animation during processing end


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
        scanf("%d", (int *)&Status);
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
    

#ifdef AFE_VOICESEEKER
    AFE_VoiceSeeker_Delete(&AFE_VoiceSeekerConfig);
#endif

    /* end */
    printf("Press a key + ENTER to close: \n\n");
    scanf("%d", (int *)&Status);

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




