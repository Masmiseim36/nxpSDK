/*
* Copyright 2020 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

/**
@file
Header file for the application layer interface of the VIT module.
This files includes all definitions, types, structures and function prototypes
required by the calling layer. All other types, structures and functions are
private.
*/

#ifndef __VIT_H__
#define __VIT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
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
//     - a SAMPLE can have 1, 2 or N channels (N defined by VIT_MAX_CHANNEL_NUMBER here after)
//     - a FRAME is composed of multi-channel samples : 160 multi-channel samples in the VIT input buffer
//       meaning that VIT_SAMPLES_PER_FRAME should be always 160 whatever the number of channels 
#define VIT_SAMPLES_PER_FRAME             160                        
#define VIT_SAMPLE_RATE                   16000               // sample rate in Hz
#define VIT_MAX_NUMBER_OF_CHANNEL         _1CHAN              // Max number of Channels per Sample supported by the VIT Lib


// Error type
typedef enum
{
    VIT_SUCCESS                         = 0,                  ////< Successful return from a routine
    VIT_INVALID_BUFFER_MEMORY_ALIGNMENT = 1,                  ///< Memory alignment error
    VIT_INVALID_NULLADDRESS             = 2,                  ///< NULL allocation address
    VIT_INVALID_ARGUMENT                = 3,                  ///< Erro in function arguments
    VIT_INVALID_PARAMETER_OUTOFRANGE    = 4,                  ///< Out of range parameter
    VIT_INVALID_SAMPLE_RATE             = 5,                  ///< Sample rate not supported
    VIT_INVALID_FRAME_SIZE              = 6,                  ///< Frame size not supported
    VIT_INVALID_MODEL                   = 7,                  ///< Model not supported
    VIT_INVALID_STATE                   = 8,                  ///< State machine error
    VIT_INVALID_DEVICE                  = 9,                  ///< VIT not running on expected Device 
    VIT_SYSTEM_ERROR                    = 10,                 ///< System error
    VIT_ERROR_UNDEFINED                 = 11,                 ///< unknow error
    VIT_DUMMY_ERROR                     = PL_MAXENUM
}VIT_ReturnStatus_en;

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
 *    Goal is mainly to control enablement of Wake Word and Voice Command modules
  */
typedef enum
{
    VIT_ALL_MODULE_DISABLE        = 0,     // None module activated
    VIT_SPECIFIC_MODE_ENABLE      = 1,     // Reserved for specific VIT validation : not to be used
    VIT_WAKEWORD_MODULE_ENABLE    = 2,     // Wake Word module activated
    VIT_VOICECMD_MODULE_ENABLE    = 4,     // Voice Command module activated
    VIT_ALL_MODULE_ENABLE         = (VIT_WAKEWORD_MODULE_ENABLE | VIT_VOICECMD_MODULE_ENABLE),     // Wake word and Voice command activated
    VIT_DUMMY_OPERATINGMODE       = PL_MAXENUM
}VIT_OperatingMode_en;


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
    PL_UINT16                    SamplesPerFrame;             // number of input samples per frame
} VIT_InstanceParams_st;

/* Input Buffer structure */
typedef struct
{
    const PL_INT16               *pBuffer_Chan1;
    const PL_INT16               *pBuffer_Chan2;
    const PL_INT16               *pBuffer_Chan3;
} VIT_DataIn_st;

/* Voice Command structure */
typedef struct
{
    PL_UINT16                    Cmd_Id;
    const char                   *Cmd_Name;
} VIT_VoiceCommand_t;

/* Control Parameter structure */
typedef struct
{
    VIT_OperatingMode_en         OperatingMode;
} VIT_ControlParams_st;

/* VIT_Model_Info structure */
typedef struct
{
    PL_UINT32                    VIT_Model_Release;
    PL_BOOL                      WW_VoiceCmds_Strings;        // Inform whether the model is integrating WakeWord and Commands strings
    const char                   *WakeWord;
    PL_UINT16                    NbOfVoiceCmds;
    const char                   *VoiceCmds_List;
}
VIT_Model_Info_t;

/* Status Parameter structure */
typedef struct
{
    PL_UINT32                     VIT_MODEL_Release;
    PL_UINT32                     VIT_LIB_Release;
    PL_UINT8                      reserved[20];
} VIT_StatusParams_st;



/****************************************************************************************/
/*                                                                                      */
/*  Function                                                                            */
/*                                                                                      */
/****************************************************************************************/

/**
* @brief Set the VIT model address.
*
* This function is used to pass the VIT Model address to the VIT Lib
*
* @param pVITModelGroup     Pointer to the VIT Model
*
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_MODEL            Wrong Model provided
* @return VIT_INVALID_NULLADDRESS      pVITModel is NULL
* @return VIT_INVALID_ARGUMENT         VITModel address is not well aligned
* @return VIT_INVALID_DEVICE           VIT can not run on this device
*
* @note The VIT_SetModel function shall be called first.
*/
VIT_ReturnStatus_en VIT_SetModel (const PL_UINT8* pVITModel);



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
VIT_ReturnStatus_en VIT_GetMemoryTable(VIT_Handle_t           phInstance,
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
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When pphInstance or pMemoryTable or pInstanceParams is NULL
* @return VIT_INVALID_NULLADDRESS      When one of the memory regions has a NULL pointer
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
* @return VIT_SUCCESS                  Succeeded
* @return VIT_INVALID_NULLADDRESS      When phInstance or pNewParams is NULL
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
* @pre  pVIT_InputBuffers is filled with the input samples to process.
* @pre  pVIT_DetectionResults should be allocated by caller.
* @post pVIT_DetectionResults will return detection status of VIT.
*
* @return VIT_SUCCESS                  Succeeded
* @return others VIT_ReturnStatus_en   Depending the error
*
*/
VIT_ReturnStatus_en VIT_Process(  VIT_Handle_t            phInstance,
                                  VIT_DataIn_st           *pVIT_InputBuffers,
                                  VIT_DetectionStatus_en  *pVIT_DetectionResults
                                );

/**
* @brief Retrieve the current VIT status parameters.
*
* This function copies internal status variables into the supplied status
* parameter structures.
*
* @param phInstance      Instance handle
* @param pStatusParams  Pointer to the status parameters
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
                                               VIT_VoiceCommand_t  *pVoiceCommand
                                             );


/**
* @brief Retrieve information of the VIT model.
*
* This function returns different information of the VIT model regitered within VIT lib (registration done via VIT_SetModel()).
* The function shall be called only when VIT_SetModel() is informing that the model is correct.
* The function will return VIT_INVALID_STATE if the calling sequence is not followed (i.e VIT_SetModel() to be called first). 
*
* @param phInstance                   Instance handle
*
* @pre   phInstance should be valid handle.
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
* @note The function shall be called only when VIT_SetModel() is informing that teh model is correct.
*
*/
VIT_ReturnStatus_en VIT_GetModelInfo ( VIT_Model_Info_t *pModel_Info);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __VIT_H__ */

/* End of file */
