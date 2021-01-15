/*
* Copyright 2020 NXP
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
//#define PLATFORM_HIFI4
//#define PLATFORM_CORTEXM7
//#define PLATFORM_WINDOWS


// Way to reserve memory, if defined then Malloc() allocation else pre-reserved buffer (static allocation) 
//#define MEMORY_MALLOC

// Control enablement of the VIT Modules :
//    VIT Lib can detect wakeword only or wakeword + Voice commands
#define VIT_OPERATING_MODE           VIT_ALL_MODULE_ENABLE          //Wake Word and Voice Commands detection
//#define VIT_OPERATING_MODE           VIT_WAKEWORD_MODULE_ENABLE     //WakeWord detection only 

// PCM input file
// The data format should be aligned with VIT interface (16kHz, 16-bit, number of channel) 
#define INPUT_FILE                   "../../../_INPUT/HEYNXP_WW_CMD.pcm"



#ifndef MEMORY_MALLOC
// STATIC ALLOCATION 
// Memory region size for VIT must be filled by user :
//     The different size below are provided as example and do not correspond to the optimal size for the current model
//     Each VIT_Model will require different memory sizes (dependent on WakeWord, number of Voice CMDs, Model running from ROM or RAM....) 
//     The different memory region size can be gathered from VIT_GetMemoryTable() (1st run with putting breakpoint after VIT_GetMemoryTable)

#define MEMREGION_SLOW_DATA_SIZE   171100
#define MEMREGION_FAST_DATA_SIZE   421662
#define MEMREGION_COEF_DATA_SIZE   8822
#define MEMREGION_TEMP_DATA_SIZE   12967

#endif

/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef PLATFORM_CORTEXM7
    #include "PL_platformTypes_CortexM7.h"
#elif defined  PLATFORM_HIFI4
    #include "PL_platformTypes_HIFI4.h"
#elif defined  PLATFORM_WINDOWS
#include "PL_platformTypes_windows.h"
#else
    #error "No platform selected"
#endif


#include "VIT.h"
#include "VIT_Model.h"



/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
/*
*   Useful Definitions : not to be changed
*/
// MEMORY
#define MEMORY_ALIGNMENT             4     // in bytes

// Current VIT lib is supporting only one input channel
#define NUMBER_OF_CHANNELS          _1CHAN


/****************************************************************************************/
/*                                                                                      */
/*  Local variables                                                                     */
/*                                                                                      */
/****************************************************************************************/

// To manage the input data from the file
PL_UINT16  InputRawData[VIT_SAMPLES_PER_FRAME];                         // data from file

// VIT Buffers declaration in case of static allocation
#ifndef MEMORY_MALLOC
// To be allocated in the fatest memory of the platform (when possible)
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
    VIT_VoiceCommand_t        VoiceCommand;                             // Voice Command id
    VIT_DetectionStatus_en    VIT_DetectionResults = VIT_NO_DETECTION;  // VIT detection result
    VIT_DataIn_st             VIT_InputBuffers     = { PL_NULL, PL_NULL, PL_NULL };  // Resetting Input Buffer addresses provided to VIT_process() API

    // General
    PL_UINT32                 i, frameCnt;
    PL_UINT16                 InputRawData_Read_Number;                 // number of raw data sample read in the input file
    PL_UINT8                  MainLoop_Flag = 1;                        // main loop flag. Loop enable until value = 1
    PL_UINT16                 FrameSize = 0;
    PL_BOOL                   InitPhase_Error = PL_FALSE;

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
    Status = VIT_SetModel(VIT_Model);
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
    VIT_Model_Info_t Model_Info;
    Status = VIT_GetModelInfo(&Model_Info);
    if (Status != VIT_SUCCESS)
    {
        printf("VIT_GetModelInfo error : %d\n", Status);
        scanf("%d", &Status);
        exit(-1);                                        // We can exit from here since memory is not allocated yet
    }

    printf("VIT Model info \n");
    printf("  VIT Model Release = 0x%04x\n", Model_Info.VIT_Model_Release);
    printf("  Number of Commands supported : %d \n", Model_Info.NbOfVoiceCmds);

    if (!Model_Info.WW_VoiceCmds_Strings)               // Check here if Model is containing WW and CMDs strings
    {
        printf("  VIT_Model integrating WakeWord and Voice Commands strings : NO\n");
    }
    else
    {
        const char* ptr;

        printf("  VIT_Model integrating WakeWord and Voice Commands strings : YES\n");
        ptr = Model_Info.WakeWord;
        if (ptr != PL_NULL)
        {
            printf("  WakeWord supported : %s \n", ptr);
        }
        printf("  Voice commands supported : \n");
        ptr = Model_Info.VoiceCmds_List;
        if (ptr != PL_NULL)
        {
            for (PL_UINT16 i = 0; i < Model_Info.NbOfVoiceCmds; i++)
            {
                printf("   '%s' \n", ptr);
                ptr += strlen(ptr) + 1;                 // to consider NULL char
            }
        }
    }

    printf("\n");


    /* 
     *   Configure VIT Instance Parameters   
     */
    VITInstParams.SampleRate_Hz            = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame          = VIT_SAMPLES_PER_FRAME;
    VITInstParams.NumberOfChannel          = NUMBER_OF_CHANNELS;


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
    VITControlParams.OperatingMode = VIT_OPERATING_MODE;

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
        // Current VIT library is supporting only one channel
        // VIT_InputBuffers.pBuffer_Chan1 should be set to the input buffer address
        // VIT_InputBuffers.pBuffer_Chan1 setting can be done out of the while loop 
        // Application should take care of the ping pong buffers (when present) handling - no pingpong buffer in this example app.   
        if (VITInstParams.NumberOfChannel == _1CHAN)
        {
            VIT_InputBuffers.pBuffer_Chan1 = InputRawData;                      // PCM buffer : 16-bit - 16kHz - mono 
            VIT_InputBuffers.pBuffer_Chan2 = PL_NULL;
            VIT_InputBuffers.pBuffer_Chan3 = PL_NULL;
        }
        else
        {
            printf("Input Buffer format issue \n");
            MainLoop_Flag = 0;
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
                if (VoiceCommand.Cmd_Name != PL_NULL)
                {
                    printf(" %s\n", VoiceCommand.Cmd_Name);
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
