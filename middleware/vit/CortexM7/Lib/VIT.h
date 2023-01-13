/*
* Copyright 2020-2022 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

/**
@file
Header file for the application layer interface of the VIT module.
This file includes all definitions, types, structures and function prototypes
required by the calling layer. All other types, structures and functions are
private.
*/

#ifndef VIT_H_
#define VIT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  Valid VIT Configurations                                                            */
/*                                                                                      */
/****************************************************************************************/


/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
typedef enum { _1CHAN = 1, _2CHAN, _3CHAN} NumberOfChannel_en;

/*****  Input audio (size, sampling rate, channel number)  *****/

// Clarifications : 
//     - a SAMPLE can have 1, 2 or N channels (N defined by VIT_MAX_NUMBER_OF_CHANNEL here after)

#define VIT_SAMPLES_PER_10MS_FRAME        160                 // corresponds to 10ms @16kHz
#define VIT_SAMPLES_PER_30MS_FRAME        480                 // corresponds to 30ms @16kHz
#define VIT_SAMPLE_RATE                   16000               // sample rate in Hz
#define VIT_MAX_NUMBER_OF_CHANNEL         _1CHAN

// Error type
typedef enum
{
    VIT_SUCCESS                         = 0,                  ///< Successful return from a routine
    VIT_INVALID_BUFFER_MEMORY_ALIGNMENT = 1,                  ///< Memory alignment error
    VIT_INVALID_NULLADDRESS             = 2,                  ///< NULL allocation address
    VIT_INVALID_ARGUMENT                = 3,                  ///< Error in function arguments
    VIT_INVALID_PARAMETER_OUTOFRANGE    = 4,                  ///< Out of range parameter
    VIT_INVALID_SAMPLE_RATE             = 5,                  ///< Sample rate not supported
    VIT_INVALID_FRAME_SIZE              = 6,                  ///< Frame size not supported
    VIT_INVALID_MODEL                   = 7,                  ///< Model not supported
    VIT_INVALID_API_VERSION             = 8,                  ///< wrong API version
    VIT_INVALID_STATE                   = 9,                  ///< State machine error
    VIT_INVALID_DEVICE                  = 10,                 ///< VIT not running on expected Device 
    VIT_SYSTEM_ERROR                    = 11,                 ///< System error
    VIT_ERROR_UNDEFINED                 = 12,                 ///< Unknow error
    VIT_DUMMY_ERROR                     = PL_MAXENUM
}VIT_ReturnStatus_en;


#define VIT_API_VERSION_MAJOR 2
#define VIT_API_VERSION_MINOR 0
#define VIT_API_VERSION       ((VIT_API_VERSION_MAJOR<<16) | (VIT_API_VERSION_MINOR<<8))

/****************************************************************************************/
/*                                                                                      */
/*  Global                                                                              */
/*                                                                                      */
/****************************************************************************************/


/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/

/*
 * VIT Instance Handle
 * This handle is used by most of the VIT APIs
 * @see VIT_GetInstanceHandle
 * @ingroup VIT
*/
typedef void *VIT_Handle_t;   ///< VIT handle

/* VIT Operating Mode
 *    Goal is mainly to control enablement of the different VIT features :
 *      Low power VAD, WakeWord and Voice commands
 */
typedef enum
{
    VIT_ALL_MODULE_DISABLE        = 0,     // None module activated
    VIT_SPECIFIC_MODE_ENABLE      = 1,     // Reserved for specific VIT validation : not to be used
    VIT_LPVAD_ENABLE              = 2,     // Low power VAD module activated
    VIT_WAKEWORD_ENABLE           = 4,     // Wake Word module activated
    VIT_VOICECMD_ENABLE           = 8,     // Voice Commands module activated
    VIT_ALL_MODULE_ENABLE         = VIT_LPVAD_ENABLE | VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE, // LPVAD, Wake word and Voice commands activated
    VIT_DUMMY_OPERATINGMODE       = PL_MAXENUM
}VIT_OperatingMode_en;


/* VIT Model location
*    Indicates memory location of the VIT Model
*/
typedef enum
{
    VIT_MODEL_IN_ROM   = 0,                // Flash memory
    VIT_MODEL_IN_RAM,                      // can be on chip RAM or external RAM
    VIT_DUMMY_LOCATION = PL_MAXENUM
}VIT_Model_Location_en;

/* VIT Device Id
*    To specify on which device VIT is running
*/
typedef enum
{
    VIT_IMXRT1040 = 1,                         // I.MXRT1040 : VIT running on Cortex-M7
    VIT_IMXRT1050 ,                            // I.MXRT1050 : VIT running on Cortex-M7
    VIT_IMXRT1060 ,                            // I.MXRT1060 : VIT running on Cortex-M7
    VIT_IMXRT1160 ,                            // I.MXRT1160 : VIT running on Cortex-M7
    VIT_IMXRT1170,                             // I.MXRT1170 : VIT running on Cortex-M7
    VIT_IMXRT500,                              // I.MXRT500  : VIT running on FusionF1
    VIT_IMXRT600,                              // I.MXRT600  : VIT running on HIFI4
    VIT_IMX8MMINIM4,                           // I.MX8MINI  : VIT running on Cortex-M4
    VIT_IMX8MPLUSM7,                           // I.MX8PLUS  : VIT running on Cortex-M7
    VIT_IMX8MA53,                              // I.MX8MA53  : VIT running on Cortex-A (i.MX8MPlus and i.MX8MMini)
    VIT_IMX9XA55,                              // I.MX9XA55  : VIT running on Cortex-A (i.MX9X)

    VIT_NB_OF_DEVICES = VIT_IMX9XA55,
    VIT_DUMMY_DEVICE  = PL_MAXENUM
}VIT_DeviceId_en;

/* VIT Detection Status
 *    Status returned by VIT_Process() API.
 *    Indicates if the Wake Word or a Voice Command has been detected on the frame processed 
 */
typedef enum
{
    VIT_NO_DETECTION    = 0,               // Nothing detected
    VIT_WW_DETECTED     = 1,               // WakeWord Detected
    VIT_VC_DETECTED     = 2,               // a Voice Command Detected
    VIT_DUMMY_DETECTION = PL_MAXENUM
}VIT_DetectionStatus_en;


/****************************************************************************************/
/*                                                                                      */
/*  Structures                                                                          */
/*                                                                                      */
/****************************************************************************************/
typedef struct
{
    PL_UINT16                    SampleRate_Hz;               // Sampling Rate value in Hz
    NumberOfChannel_en           NumberOfChannel;             // Number of Channels
    PL_UINT16                    SamplesPerFrame;             // Number of input samples per frame
    VIT_DeviceId_en              DeviceId;                    // To specify on which device VIT is running
    PL_UINT32                    APIVersion;
} VIT_InstanceParams_st;

/* Wakeword structure */
typedef struct
{
    PL_UINT16                    Id;
    const char                   *pName;
    PL_UINT32                    StartOffset;                 // in samples
} VIT_WakeWord_st;

/* Voice Command structure */
typedef struct
{
    PL_UINT16                    Id;
    const char                   *pName;
} VIT_VoiceCommand_st;

/* Control Parameter structure */
typedef struct
{
    VIT_OperatingMode_en         OperatingMode;
    PL_FLOAT                     Command_Time_Span;    // Corresponding to the detection period in second for each command (Max allowed 8.0 seconds)
                                                       // VIT will return UNKNOWN if no command is recognized during this time span.
    PL_BOOL                      Feature_LowRes;       // Compute features in low resolution - considered only if OperatingMode equals to VIT_ALL_MODULE_DISABLE
    PL_UINT32                    Reserved;
} VIT_ControlParams_st;

/* VIT_Model_Info structure */
typedef struct
{
    PL_UINT32                    VIT_Model_Release;
    const char                   *pLanguage;
    PL_BOOL                      WW_VoiceCmds_Strings;        // Inform whether the model is integrating WakeWord and Commands strings
    PL_UINT16                    NbOfWakeWords;
    const char                   *pWakeWord_List;
    PL_UINT16                    NbOfVoiceCmds;
    const char                   *pVoiceCmds_List;
}
VIT_ModelInfo_st;

/* VIT_Lib_Info_st structure */
typedef struct
{
    PL_UINT32                     VIT_LIB_Release;
    VIT_OperatingMode_en          VIT_Features_Supported;            // List of features supported by VIT
    PL_UINT16                     NumberOfChannels_Supported;        // Number of Channels supported by the VIT lib 
} VIT_LibInfo_st;


/* Status Parameter structure */
typedef struct
{
    PL_UINT32                     VIT_MODEL_Release;
    PL_UINT32                     VIT_LIB_Release;
    VIT_OperatingMode_en          VIT_Features_Supported;            // List of features supported by VIT
    VIT_OperatingMode_en          VIT_Features_Selected;             // List of features enabled
    PL_UINT16                     NumberOfChannels_Supported;        // Number of Channels supported by the VIT lib 
    VIT_DeviceId_en               Device_Selected;
    PL_UINT16                     VIT_Sequencer_Slot;
    PL_BOOL                       LPVAD_EventDetected;
    PL_UINT8                      Reserved[20];
} VIT_StatusParams_st;



/****************************************************************************************/
/*                                                                                      */
/*  Functions                                                                            */
/*                                                                                      */
/****************************************************************************************/

/**
* @brief Set the VIT model address.
*
* This function is used to pass the VIT Model address to the VIT Lib
*
* @param pVITModelGroup     Pointer to the VIT Model
* @param Location           Memory location of the VIT Model (ROM or RAM)
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_MODEL            Wrong Model provided
* @return VIT_INVALID_NULLADDRESS      pVITModel is NULL
* @return VIT_INVALID_ARGUMENT         VITModel address is not well aligned or location is not correct
* @return VIT_INVALID_DEVICE           VIT can not run on this device
*
* @note The VIT_SetModel function shall be called first.
*/
VIT_ReturnStatus_en VIT_SetModel (const PL_UINT8* pVITModel, VIT_Model_Location_en Location);



/**
* @brief Retrieve the memory requirements of the VIT module.
*
* This function is used to obtain the VIT module memory requirements to support
* memory allocation. It can also be used to return the memory base address provided
* during memory allocation to support freeing of memory when the VIT module is no
* longer required.
*
* @param phInstance              Instance Handle
* @param pMemoryTable            Pointer to an empty memory table
* @param pInstanceParams         Pointer to the instance parameters
*
* This function can be used in two ways:
* @li phInstance = NULL                Returns the memory requirements
* @li phInstance = Instance handle     Returns the memory requirements and allocated
*                                      base addresses.
*
* @post When this function is called with phInstance = NULL the memory base address pointers
* will be NULL on return.
* @post When the function is called for freeing memory, phInstance = Instance Handle the
* memory table returns the allocated memory and base addresses used during initialisation.
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When pMemoryTable is NULL
* @return VIT_INVALID_NULLADDRESS      When requesting memory requirements or pInstanceParams
*                                      is NULL
*
*/
VIT_ReturnStatus_en VIT_GetMemoryTable(VIT_Handle_t           hInstance,
                                       PL_MemoryTable_st      *pMemoryTable,
                                       VIT_InstanceParams_st  *pInstanceParams);


/**
* @brief Retrieve the handle to the instance of the VIT module
*
* This function is used to create a VIT module instance. It returns the created
* instance handle through pphInstance. All parameters are set to invalid values, the
* VIT_SetControlParameters function must be called with a set of valid control
* parameters before the VIT_Process function is called.
*
* @param pphInstance             Pointer to the instance handle
* @param pMemoryTable            Pointer to the memory definition table
* @param pInstanceParams         Pointer to the instance parameters
*
* @pre pphInstance should point to PL_NULL.
* @pre The memory allocation must be provided by the application by filling
* in the memory region base addresses in the memory table before calling this function.
* @pre The pMemoryTable should be filled with the same values provided to
* VIT_GetMemoryTable.
*
* @post The module handle will be initialized. All parameters are set to invalid values,
* the VIT_SetControlParameters function must be called with a set of valid control
* parameters before the VIT Process function can be called.
*
* @return VIT_SUCCESS                        Succeeded
* @return VIT_INVALID_NULLADDRESS            When phInstance or pMemoryTable or pInstanceParams is NULL
* @return VIT_INVALID_NULLADDRESS            When one of the memory regions has a NULL pointer
* @return VIT_INVALID_PARAMETER_OUTOFRANGE   When one of the InstansParams is wrong
* @return VIT_INVALID_STATE                  When a wrong VIT APIs call sequence is observed
*/
VIT_ReturnStatus_en VIT_GetInstanceHandle(VIT_Handle_t           *phInstance,
                                          PL_MemoryTable_st      *pMemoryTable,
                                          VIT_InstanceParams_st  *pInstanceParams
                                         );


/**
* @brief Retrieve the current VIT control parameters.
*
* This function copies the control parameters from the VIT lib into the supplied
* parameter structure. The values returned are the values given in the last successful
* call to the VIT_SetControlParameters function.
*
* @param phInstance             Instance handle
* @param pControlParams         Pointer to the control parameters
*
* @pre   phInstance should be valid handle.
* @pre   pControlParams should be allocated by caller.
* @post  pControlParams will be filled with the values given in the last successful call to
*        the VIT_SetControlParameters function. 
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pControlParams is NULL
*
* @note The VIT_GetControlParameters function can be called at any time during processing.
* @see VIT_SetControlParameters
*/
VIT_ReturnStatus_en VIT_GetControlParameters(VIT_Handle_t           phInstance,
                                             VIT_ControlParams_st   *pControlParams
                                            );


/**
* @brief Sets or changes the VIT module parameters.
*
* This function takes the new set of parameters and makes a local copy within
* VIT but the parameters are only applied on the next call of the VIT process function
* When a parameter is unchanged no action is taken. This function can be called at any time during the processing.
*
* @param phInstance             Instance Handle
* @param pNewParams             Pointer to a parameter structure
*
* @pre phInstance should be valid handle.
*
* @return VIT_SUCCESS                        Succeeded
* @return VIT_INVALID_NULLADDRESS            When phInstance or pNewParams is NULL
* @return VIT_INVALID_PARAMETER_OUTOFRANGE   When one parameter is not in the right range
* @return VIT_INVALID_AFE_CONFIG             when AFE configuration is wrong
*
*/
VIT_ReturnStatus_en VIT_SetControlParameters(VIT_Handle_t                 phInstance,
                                             const VIT_ControlParams_st   *const pNewParams
                                            );

/**
* @brief Resets the VIT module.
*
* The VIT module instance memory contains data which depend on the input
* samples that have been processed previously.  These data are adaptive buffers of the
* algorithm. The VIT_ResetInstance function resets these buffers.
*
* The VIT_ResetInstance function should be called whenever there is a
* discontinuity in the input audio stream.  A discontinuity means that the
* current block of samples is not contiguous with the previous block of samples.
* Examples are
* @li Calling the VIT process function after a period of inactivity
* @li Buffer underrun or overflow in the audio driver
*
* @param phInstance               Instance Handle
*
* @pre phInstance should be valid handle.
*
* @post full reset to the latest applied control parameters. All
* internal audio buffers and full adaptive behavior is reset. This should be only
* done after a long period of inactivity.
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pNewParams is NULL
*
*/
VIT_ReturnStatus_en VIT_ResetInstance(VIT_Handle_t phInstance);


/**
* @brief Analyzes a block of samples by the VIT module.
*
* This function processes the block of samples as configured with the current
* parameter settings. If new parameters have been given since the last call to
* VIT_Process then these will be applied at the beginning of this process call.
*
* @param phInstance  Instance Handle
* @param pInData     Pointer to the input sample buffer
*
* @pre phInstance should be valid handle.
* @pre VIT_SetControlParameters should be called successfully once before
* the first call to VIT_Process
* @pre  pVIT_InputBuffer is filled with the input samples to process.
* @pre  pVIT_DetectionResults should be allocated by caller.
* @post pVIT_DetectionResults will return detection status of VIT.
*
* @return VIT_SUCCESS                  Succeeded
* @return others VIT_ReturnStatus_en   Depending the error
*
*/
VIT_ReturnStatus_en VIT_Process(  VIT_Handle_t            phInstance,
                                  void                    *pVIT_InputBuffer,
                                  VIT_DetectionStatus_en  *pVIT_DetectionResults
                               );



/**
* @brief Retrieve the current VIT status parameters.
*
* This function copies internal status variables into the supplied status
* parameter structures.
*
* @param phInstance      Instance handle
* @param pStatusParams   Pointer to the status parameters
*
* @pre   phInstance should be valid handle.
* @pre   pStatusParams should be allocated by caller.
* @post  pStatusParams will be filled with the latest status values of VIT.
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pStatusParams is NULL
* @return VIT_INVALID_ARGUMENT         When StatusParamsSize is less than the expected size of status params buffer
*
* @note The VIT_GetStatusParameters function can be called at any time during processing.
*/
VIT_ReturnStatus_en VIT_GetStatusParameters( VIT_Handle_t           phInstance,
                                             VIT_StatusParams_st    *pStatusParams,
                                             PL_UINT16              StatusParamsSize
                                            );


/**
* @brief Retrieve the Wakeword Detected.
*
* This function returns the Wakeword Id and string (when present in model) detected by the VIT instance.
* The function shall be called only when VIT_Process() is informing that a Wakeword is detected (*pVIT_DetectionResults==VIT_WW_DETECTED)
* The function will return VIT_INVALID_STATE if the calling sequence is not followed (i.e VIT_GetWakeWordFound() to be called after VIT_Process()
* only if a Wakeword has been detected). 
*
* @param phInstance                   Instance handle
*
* @pre   phInstance should be valid handle.
*
* @pre   pWakeWord should be allocated by caller.
* @post  pWakeWord will be filled with the Id and string of the Wakeword detected.
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pWakeWord is NULL
*
* @note The function shall be called only when VIT_Process() is informing that a Wakeword is detected (*pVIT_DetectionResults==VIT_WW_DETECTED)
*
*/
VIT_ReturnStatus_en VIT_GetWakeWordFound     ( VIT_Handle_t         pVIT_Instance,
                                               VIT_WakeWord_st      *pWakeWord
                                             );


/**
* @brief Retrieve the Voice Command Detected.
*
* This function returns the Voice Command Id and string (when present in model) detected by the VIT instance.
* The function shall be called only when VIT_Process() is informing that a Voice Command is detected (*pVIT_DetectionResults==VIT_VC_DETECTED)
* The function will return VIT_INVALID_STATE if the calling sequence is not followed (i.e VIT_GetVoiceCommandFound() to be called after VIT_Process()
* only if a command has been detected). 
*
* @param phInstance                   Instance handle
*
* @pre   phInstance should be valid handle.
*
* @pre   pVoiceCommand should be allocated by caller.
* @post  pVoiceCommand will be filled with the Id and string of the command detected.
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pVoiceCommand is NULL
*
* @note The function shall be called only when VIT_Process() is informing that a Voice Command is detected (*pVIT_DetectionResults==VIT_VC_DETECTED)
*
*/
VIT_ReturnStatus_en VIT_GetVoiceCommandFound ( VIT_Handle_t         pVIT_Instance,
                                               VIT_VoiceCommand_st  *pVoiceCommand
                                             );


/**
* @brief Retrieve information of the VIT model.
*
* This function returns different information of the VIT model regitered within VIT lib (registration done via VIT_SetModel()).
* The function shall be called only when VIT_SetModel() is informing that the model is correct.
* The function will return VIT_INVALID_STATE if the calling sequence is not followed (i.e VIT_SetModel() to be called first). 
*
* @pre   pModel_Info should be allocated by caller.
* @post  pModel_Info will be filled with
*              - VIT_Model release number
*              - If model integrates WWW and Voice CMDs strings (WW_VoiceCmds_Strings)
*              - WAKEWORD string    (if WW_VoiceCmds_Strings == TRUE)
*              - Number Of Voice Commands supported
*              - Voice CMDs ID
*              - Voice CMDs Names strings  (if WW_VoiceCmds_Strings == TRUE) : The CMDs are listed following CMD ID order
*                         - First CMD name is corresponding to the voice command corresponding to CMD_ID 1
*                         - Second CMD name is corresponding to the voice command corresponding to CMD_ID 2
*                         - ...
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When pModel_Info is NULL
* @return VIT_INVALID_STATE            When VIT_SetModel() has not been called before
*
* @note The function shall be called only when VIT_SetModel() is informing that the model is correct.
*
*/
VIT_ReturnStatus_en VIT_GetModelInfo ( VIT_ModelInfo_st *pModel_Info);


/**
* @brief Retrieve information of the VIT library.
*
* This function copies internal LibInfo variables into the supplied LibInfo buffer
* parameter structures.
*
* @pre   pLib_Info should be allocated by caller.
* @post  pLib_Info will be filled with
*              - VIT lib release number
*              - Features integrated in the library
*              - Number of channels supported by the library
*              - If WakeWord is in Text2Model or not
*
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When pModel_Info is NULL
*
* @note The VIT_GetLibInfo function can be called at any time during processing.
*/
VIT_ReturnStatus_en VIT_GetLibInfo(VIT_LibInfo_st    *pLib_Info);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* VIT_H_ */

/* End of file */
