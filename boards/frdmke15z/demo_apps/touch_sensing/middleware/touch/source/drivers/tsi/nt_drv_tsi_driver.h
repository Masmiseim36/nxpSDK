/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_TSI_DRIVER_H_
#define _FSL_TSI_DRIVER_H_

/**
 * \defgroup tsi_drivers TSI Drivers
 * \ingroup ntapi
 *
 * Drivers represent the lowest level of abstraction in the TSI peripheral control;
 *
 * Drivers contain common and TSI version-specific files. There are functions like
 * \ref NT_TSI_DRV_Init, \ref NT_TSI_DRV_EnableElectrode, \ref NT_TSI_DRV_Measure, and so on.
 *
 * \{
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsl_device_registers.h"
#include "../source/system/nt_system_prv.h"
#if (FSL_FEATURE_TSI_VERSION == 2)
#include "fsl_tsi_v2.h" 
#elif (FSL_FEATURE_TSI_VERSION == 4)
#include "fsl_tsi_v4.h"
#elif (FSL_FEATURE_TSI_VERSION == 5)
#include "fsl_tsi_v5.h"
#include "nt_drv_tsi_v5_driver_specific.h"
#endif

#if FSL_FEATURE_SOC_TSI_COUNT || defined(NT_DOXYGEN)

/**
 * The total number of TSI channels corresponds to FSL_FEATURE_TSI_CHANNEL_COUNT
 * if not defined elsewhere.
 */
#ifndef TF_TSI_TOTAL_CHANNEL_COUNT
#define TF_TSI_TOTAL_CHANNEL_COUNT FSL_FEATURE_TSI_CHANNEL_COUNT
#endif

/**
 * \brief Callback routine of the TSI driver.
 *
 * The function is called at the end of the measure of the TSI driver. The function
 * can be called from an interrupt, so the code inside the callback should be short
 * and fast.
 * \param instance - instance of the TSI peripheral
 * \param usrData - user data (type is void*), the user data are specified by function \ref NT_TSI_DRV_SetCallBackFunc
 * \return - none
 */
typedef void (*tsi_callback_t)(uint32_t instance, void* usrData);

/** Error codes for the TSI driver. */
typedef enum _tsi_status
{
  kStatus_TSI_Success = 0,
  kStatus_TSI_Busy,                     /*!< TSI still in progress */
  kStatus_TSI_Overflow,                 /*!< TSI counter out of range */
  kStatus_TSI_Overrun,                  /*!< TSI measurement overrun  */
  kStatus_TSI_LowPower,                 /*!< TSI is in low-power mode */
  kStatus_TSI_Recalibration,            /*!< TSI is under recalibration process */
  kStatus_TSI_InvalidChannel,           /*!< Invalid TSI channel */
  kStatus_TSI_InvalidMode,              /*!< Invalid TSI mode */
  kStatus_TSI_Initialized,              /*!< The driver is initialized and ready to measure */
  kStatus_TSI_Error,                    /*!< The general driver error */
  kStatus_TSI_SelfUntouchRecalibError,  /*!< The recalibration self-cap untouched calibration error */   
  kStatus_TSI_SelfSensitivRecalibError, /*!< The recalibration self-cap sensitivity calibration error */ 
  kStatus_TSI_MutUntouchRecalibError,   /*!< The recalibration mutual-cap untouched calibration error */   
  kStatus_TSI_MutSensitivRecalibError   /*!< The recalibration mutual-cap sensitivity calibration error */
} tsi_status_t;

/**
* User configuration structure for the TSI driver.
*
* Use an instance of this structure with NT_TSI_DRV_Init(). This enables you to configure the
* most common settings of the TSI peripheral with a single function call. Settings include:
*
*/
typedef struct _nt_tsi_user_config {
    tsi_config_t                        *config;        /*!< A pointer to the hardware configuration. Can't be NULL. */
    tsi_callback_t                      pCallBackFunc;  /*!< A pointer to the callback function at the end of the measurement. */
    void                                * usrData;      /*!< The user data of the callback function. */
} nt_tsi_user_config_t;

#if (FSL_FEATURE_TSI_VERSION == 5)
/**
* User recalibration configuration structure for the TSI_v5 driver.
*
* Use an instance of this structure with a second parameter of the NT_TSI_DRV_Recalibrate function to transmit recalibration 
* parameters. The structure consists of the minimum and maximum counter and the delta (sensitivity) values for both 
* the mutual-capacitance and self-capacitance modes. 
*/
typedef struct nt_tsi_recalib_config {
    uint32_t SelfRangeMin     ;       /*!< minimum counter value for self-cap mode */
    uint32_t SelfRangeMax     ;       /*!< maximum counter value for self-cap mode */
    uint32_t SelfRangeDeltaMin;       /*!< minimum delta value for self-cap mode */
    uint32_t SelfRangeDeltaMax;       /*!< maximum delta value for self-cap mode */
    uint32_t MutRangeMin      ;       /*!< minimum counter value for mutual-cap mode */
    uint32_t MutRangeMax      ;       /*!< maximum counter value for mutual-cap mode */
    uint32_t MutRangeDeltaMin ;       /*!< minimum delta value for mutual-cap mode */
    uint32_t MutRangeDeltaMax ;       /*!< maximum delta value for mutual-cap mode */
} nt_tsi_recalib_config_t;
#endif

/**
* Driver operation mode definition.
*
* The operation name definition used for the TSI driver.
*
*/
typedef enum _nt_tsi_modes
{
  tsi_OpModeNormal = 0,        /*!< The normal mode of TSI. */
  tsi_OpModeProximity,         /*!< The proximity-sensing mode of TSI. */
  tsi_OpModeLowPower,          /*!< The low-power mode of TSI. */
  tsi_OpModeNoise,             /*!< The noise mode of TSI. This mode is not valid with TSI HW, valid only for the TSIL HW. */
  tsi_OpModeCnt,               /*!< The count of TSI modes - for internal use. */
  tsi_OpModeNoChange           /*!< The special value of operation mode that allows calling for example the \ref NT_TSI_DRV_DisableLowPower function without changing the operation mode. */
}nt_tsi_modes_t;

/**
* Driver operation status definition.
*
* The operation status definition used for the TSI driver.
*
*/
typedef enum _nt_tsi_op_status
{
  tsi_OpStatusNormal = 0,        /*!< The normal mode of TSI. */
  tsi_OpStatusSuspend,           /*!< TSI mesurement suspended */
}nt_tsi_op_status_t;


/**
* Driver operation mode data hold structure.
*
* This is the operation mode data hold structure. The structure is keeping all data needed
* for the driver to be able to switch the operation modes and properly set up the HW peripheral.
*
*/
typedef struct _nt_tsi_operation_mode
{
  uint64_t                              enabledElectrodes;  /*!< The backup of enabled electrodes for the operation mode */
  tsi_config_t                          config;             /*!< Hardware configuration */
}nt_tsi_operation_mode_t;

/**
* Driver data storage place.
*
* It must be created by the application code and the pointer is handled by the \ref NT_TSI_DRV_Init function
* to the driver. The driver keeps all context data for itself running. The settings include:
*
*/
typedef struct _nt_tsi_state {
  tsi_status_t                          status;             /*!< Current status of the driver. */
  nt_tsi_op_status_t                    opSatus;            /*!< Suspended or Normal operation */
  tsi_callback_t                        pCallBackFunc;      /*!< Pointer to the callback function at the end of the measurement. */
  void                                  *usrData;           /*!< User data pointer handled by the callback function. */
  nt_mutex_t                            lock;               /*!< Used by the whole driver to secure the context data integrity. */
  nt_mutex_t                            lockChangeMode;     /*!< Used by the change mode function to secure the context data integrity. */ 
  nt_tsi_modes_t                        opMode;             /*!< Storage of current operation mode. */
  nt_tsi_operation_mode_t               opModesData[tsi_OpModeCnt]; /*!< Data storage of individual operational modes. */
  uint16_t                              counters[TF_TSI_TOTAL_CHANNEL_COUNT]; /*!< The mirror of the last state of the counter registers. */
}nt_tsi_state_t;


/** Table of the base addresses for TSI instances. */
extern TSI_Type * const g_tsiBase[];

/** Table to save the TSI IRQ enumeration numbers defined in the CMSIS header file. */
extern const IRQn_Type g_tsiIrqId[FSL_FEATURE_SOC_TSI_COUNT];

/** Table to save the pointers to the context data. */
extern nt_tsi_state_t * g_tsiStatePtr[FSL_FEATURE_SOC_TSI_COUNT];

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * \defgroup tsi_drivers_api API Functions
 * \ingroup tsi_drivers
 * General Function definition of the drivers.
 *
 * \{ */

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \brief Initializes a TSI instance for operation.
*
* This function initializes the run-time state structure and prepares the
* entire peripheral to measure the capacitances on the electrodes.
 \code

   static nt_tsi_state_t myTsiDriverStateStructure;

   nt_tsi_user_config_t myTsiDriveruserConfig =
   {
    .config =
       {
          ...
       },
     .pCallBackFunc = APP_myTsiCallBackFunc,
     .usrData = myData,
   };

   if(NT_TSI_DRV_Init(0, &myTsiDriverStateStructure, &myTsiDriveruserConfig) != kStatus_TSI_Success)
   {
      // Error, the TSI is not initialized
   }
  \endcode
*
* \param instance The TSI module instance.
* \param tsiState A pointer to the TSI driver state structure memory. The user is only
*  responsible to pass in the memory for this run-time state structure where the TSI driver
*  takes care of filling out the members. This run-time state structure keeps track of the
*  current TSI peripheral and driver state.
* \param tsiUserConfig The user configuration structure of the nt_tsi_user_config_t type. The user
*   populates the members of this structure and passes the pointer of this structure
*  into the function.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_Init(uint32_t instance, nt_tsi_state_t * tsiState, const nt_tsi_user_config_t * tsiUserConfig);

/**
* \brief Initializes TSI v5 in both modes (self cap, mutual).
*
*
 \code
   NT_TSI_DRV_InitSpecific(0, &myTsiDriveruserConfig) 
  \endcode
* \param base The TSI module instance.
* \param config The user configuration structure of the tsi_config_t type. The user
*   populates the members of this structure and passes the pointer of this structure
*  into the function.
* \return none.
*/
void NT_TSI_DRV_InitSpecific(TSI_Type *base, const tsi_config_t *config);

/**
* \brief Shuts down the TSI by disabling the interrupts and the peripheral.
*
* This function disables the TSI interrupts and the peripheral.
*
 \code
   if(NT_TSI_DRV_DeInit(0) != kStatus_TSI_Success)
   {
      // Error, the TSI is not de-initialized
   }
  \endcode
* \param instance The TSI module instance.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_DeInit(uint32_t instance);

/**
* \brief Enables/disables one electrode of the TSI module.
*
* The function must be called for each used electrode after the initialization of the TSI driver.
*
  \code
        // On the TSI instance 0, enable electrode with index 5
    if(NT_TSI_DRV_EnableElectrode(0, 5, TRUE) != kStatus_TSI_Success)
    {
        // Error, the TSI 5'th electrode is not enabled
    }
  \endcode
* \param instance   TSI module instance.
* \param channel    Index of the TSI channel.
* \param enable     TRUE - for channel enable, FALSE for disable.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_EnableElectrode(uint32_t instance, const uint32_t channel, const bool enable);

/**
* \brief Returns a mask of the enabled electrodes of the TSI module.
*
* The function returns the mask of the enabled electrodes of the current mode.
*
  \code
    uint32_t enabledElectrodeMask;
    enabledElectrodeMask = NT_TSI_DRV_GetEnabledElectrodes(0);
  \endcode
* \param instance TSI module instance.
* \return Mask of enabled electrodes for the current mode.
*/
uint64_t NT_TSI_DRV_GetEnabledElectrodes(uint32_t instance);

/**
* \brief Starts the measure cycle of the enabled electrodes.
*
* The function is non-blocking. Therefore, the results can be obtained after the driver completes the measure cycle.
*         The end of the measure cycle can be checked by pooling the \ref NT_TSI_DRV_GetStatus function or waiting for the registered callback function by using
*         \ref NT_TSI_DRV_SetCallBackFunc or \ref NT_TSI_DRV_Init.
*
  \code
    // Example of the pooling style of the use of the NT_TSI_DRV_Measure() function
    if(NT_TSI_DRV_Measure(0) != kStatus_TSI_Success)
    {
        // Error, the TSI 5'th electrode is not enabled
    }

    while(NT_TSI_DRV_GetStatus(0) != kStatus_TSI_Initialized)
    {
        // Do something useful - don't waste the CPU cycle time
    }

  \endcode
* \param instance The TSI module instance.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_Measure(uint32_t instance);

/**
* \brief Returns the last measured value.
*
* This function returns the last measured value in the previous measure cycle.
*           The data is buffered inside the driver.
*
  \code
    // Get the counter value from the TSI instance 0 and 5th channel

    uint32_t result;

    if(NT_TSI_DRV_GetCounter(0, 5, &result) != kStatus_TSI_Success)
    {
        // Error, the TSI 5'th electrode is not read
    }

  \endcode
* \param instance The TSI module instance.
* \param channel The TSI electrode index.
* \param counter The pointer to the 16-bit value where the channel counter value is stored.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_GetCounter(uint32_t instance, const uint32_t channel, uint16_t * counter);

/**
* \brief Returns the current status of the driver.
*
* This function returns the current working status of the driver.
*
  \code
    // Get the current status of the TSI driver

    tsi_status_t status;

    status = NT_TSI_DRV_GetStatus(0);


  \endcode
* \param instance The TSI module instance.
* \return The current status of the driver.
*/
tsi_status_t NT_TSI_DRV_GetStatus(uint32_t instance);

/**
* \brief Enters the low-power mode of the TSI driver.
*
* This function switches the driver to the low-power mode and immediately enables the
*            low-power functionality of the TSI peripheral. Before calling this
*           function, the low-power mode must be configured - Enable the right electrode
*           and recalibrate the low-power mode to get the best performance for this mode.
*
  \code
    // Switch the driver to the low-power mode
    uint16_t signal;

    // The first time is needed to configure the low-power mode configuration

    (void)NT_TSI_DRV_ChangeMode(0, tsi_OpModeLowPower); // I don't check the result because I believe in.
    // Enable the right electrode for the low-power AKE up operation
    (void)NT_TSI_DRV_EnableElectrode(0, 5, true);
    // Recalibrate the mode to get the best performance for this electrode
    (void)NT_TSI_DRV_Recalibrate(0);

    if(NT_TSI_DRV_EnableLowPower(0) != kStatus_TSI_Success)
    {
        // Error, the TSI driver can't go to the low-power mode
    }


  \endcode
* \param instance The TSI module instance.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_EnableLowPower(uint32_t instance);

/**
* \brief This function returns back the TSI driver from the low-power to the standard operation
*
* Function switches the driver back from the low-power mode and it can immediately change
*           the operation mode to any other or keep the driver in the low-power
*           configuration to be able go back to the low-power state.
*
  \code
    // Switch the driver from the low-power mode

    if(NT_TSI_DRV_DisableLowPower(0, tsi_OpModeNormal) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot go from the low-power mode
    }


  \endcode
* \param instance   The TSI module instance.
* \param mode       The new operation mode request.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_DisableLowPower(uint32_t instance, const nt_tsi_modes_t mode);

/**
* \brief Automatically measures one electrode.
*
* This function executes one measurement of the specified electrode (self or mutual capacitance)
*           The electrode to be measured is specified by the number in the second function parameter.
*           The result (counter value) is returned by this function.           
*
  \code
    uint32_t result;

    // Measure one electrode at one time 
    result= NT_TSI_DRV_MeasureOneElect(0, 0);


  \endcode
* \param instance   The TSI module instance.
* \param electrode  The electrode number to be measured. 
* \return           The measured electrode counter value. 
*/

int32_t NT_TSI_DRV_MeasureOneElect(uint32_t instance, uint32_t electrode);

/**
* \brief Automatically measures all used TSI electrodes.
*
* This function executes one measurement of all used electrodes.
*           The results (counter values) are stored in te array pointed to by 
*           the pointer in the function parameters.           
*
  \code
     uint16_t curr_counters[TF_TSI_TOTAL_CHANNEL_COUNT]; 

    // Measure all electrodes at one time 
    if(NT_TSI_DRV_MeasureAllElect(0, &curr_counters) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot measure the electrodes in this mode
    }

  \endcode
* \param instance   The TSI module instance.
* \param counters  The pointer to the array where all used TSI counters are stored. 
* \return An error code or kStatus_TSI_Success.
*/

tsi_status_t NT_TSI_DRV_MeasureAllElect(uint32_t instance, uint16_t (*counters)[]);
/**
* \brief Automatically measures selected TSI electrodes.
*
* This function executes one measurement of more selected electrodes.
*           The results (counter values) are stored in te array pointed to by 
*           the pointer in the function parameters.           
*
  \code
     uint16_t curr_counters[TF_TSI_TOTAL_CHANNEL_COUNT]; 
     uint64_t enabledElectrodes = 0x01FFFFFFU;

    // Measure all electrodes at one time 
    if(result = NT_TSI_DRV_MeasureMoreElect(instance, &curr_counters, enabledElectrodes);) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot measure the electrodes in this mode
    }

  \endcode
* \param instance           The TSI module instance.
* \param counters           The pointer to the array where all used TSI counters are stored. 
* \param measuredElectrodes The electrodes to be measured. 
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_MeasureMoreElect(uint32_t instance, uint16_t (*counters)[], uint64_t measuredElectrodes);

/**
* \brief Automatically recalibrates all important TSI settings.
*
*  The function sets the TSI register to the default configuration and adjusts the configuration to reach
*  the selected counter and sensitivity ranges (defined by user). Firstly, the self-capacitance TSI channels
*  are adjusted, and then the mutual-capacitance TSI channels are adjusted. When any channel type is adjusted, the function firstly 
*  measures all TSI channels (self or mutual) and calculates the average counter value. If the average counter value is
*  outside the selected range, the function adjusts the TSI parameter to get inside the range. The function repeats 
*  the previous steps until the average counter value is within the selected range. If all combinations are reached, 
*  the function returns an error. Then the function calibrates the sensitivity in a similar way, but the user must touch 
*  the self-cap or mutual-cap electrodes before each calibration cycle. The function waits for one or more touch event from the user,
*  depending on how many tuning cycles are required to reach the selected sensitivity ranges.
*
* \warning In some cases, the function does not finish the calibration (returns an error).
*
  \code
    // define auto-calibration constant in nt_setup.c file
    tsi_status_t recalib_status
    struct nt_module tsi_module
    struct nt_tsi_recalib_config recalib_config = { 
    .SelfRangeMin     = 50000,
    .SelfRangeMax     = 60000,
    .SelfRangeDeltaMin= 400,
    .SelfRangeDeltaMax= 1300,
    .MutRangeMin      = 12000,
    .MutRangeMax      = 16000,
    .MutRangeDeltaMin = 300,
    .MutRangeDeltaMax = 600, 
    };

    // assign auto-calibration structure to tsi module in nt_setup.c file 
    const struct nt_module tsi_module =
    {
        ...
        .recalib_config = (void*)&recalib_config,
    };

    // call the nt_module_recalibrate function
    recalib_status = (tsi_status_t) nt_module_recalibrate(&tsi_module);

    if (recalib_status != kStatus_TSI_Success)
    {   
        // Error, TSI calibration  was not successful (recalib_config parameters are too strict)  
    }

  \endcode
* \param module         The pointer to the TSI module structure.
* \param configuration  The pointer to the user recalibration configuration structure.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_Recalibrate(uint32_t instance, void *configuration);

/**
* \brief Sets the callback function that is called when the measure cycle ends.
*
* This function sets up or clears (parameter pFuncCallBack  = NULL) the callback function pointer
*           which is called after each measure cycle ends. The user can also set the custom user data
*           which is handled by the parameter to a callback function. One function can be called by more sources.
*
  \code
    // Clear the previous callback function

    if(NT_TSI_DRV_SetCallBackFunc(0, NULL, NULL) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot set up the callback function at the moment
    }

    // Set new callback function

    if(NT_TSI_DRV_SetCallBackFunc(0, myFunction, (void*)0x12345678) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot set up the callback function at the moment
    }


  \endcode
* \param instance       The TSI module instance.
* \param pFuncCallBack  The pointer to the application callback function.
* \param usrData        The user data pointer.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_SetCallBackFunc(uint32_t instance, const tsi_callback_t pFuncCallBack, void * usrData);

/**
* \brief Changes the current working operation mode.
*
* This function changes the working operation mode of the driver.
*
  \code
    // Change the operation mode to low-power

    if(NT_TSI_DRV_ChangeMode(0, tsi_OpModeLowPower) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot change the operation mode into low-power
    }

  \endcode
* \param instance       The TSI module instance.
* \param mode           The requested new operation mode.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_ChangeMode(uint32_t instance, const nt_tsi_modes_t mode);

/**
* \brief Returns the current working operation mode.
*
* This function returns the current working operation mode of the driver.
*
  \code
    // Gets the current operation mode of the TSI driver
    nt_tsi_modes_t mode;

    mode = NT_TSI_DRV_GetMode(0);

  \endcode
* \param instance       The TSI module instance.
* \return The current operation mode of the TSI driver.
*/
nt_tsi_modes_t NT_TSI_DRV_GetMode(uint32_t instance);

/**
* \brief Loads the new configuration into a specific mode.
*
* This function loads the new configuration into a specific mode.
*           This can be used when the calibrated data are stored in any NVM
*           to load after the startup of the MCU to avoid the run recalibration that takes
*           more time.
*
  \code
    // Load operation mode configuration

    extern const nt_tsi_operation_mode_t * myTsiNvmLowPowerConfiguration;

    if(NT_TSI_DRV_LoadConfiguration(0, tsi_OpModeLowPower, myTsiNvmLowPowerConfiguration) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot load the configuration
    }

  \endcode
* \param instance       The TSI module instance.
* \param mode           The requested new operation mode.
* \param operationMode  The pointer to the storage place of the configuration that should be loaded.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_LoadConfiguration(uint32_t instance, const nt_tsi_modes_t mode, const nt_tsi_operation_mode_t * operationMode);

/**
* \brief Saves the TSI driver configuration for a specific mode.
*
* This function saves the configuration of a specific mode.
*           This can be used when the calibrated data should be stored in any backup memory
*           to load after the start of the MCU to avoid running the recalibration that takes 
*           more time.
*
  \code
    // Save operation mode configuration

    extern nt_tsi_operation_mode_t  myTsiNvmLowPowerConfiguration;

    if(NT_TSI_DRV_SaveConfiguration(0, tsi_OpModeLowPower, &myTsiNvmLowPowerConfiguration) != kStatus_TSI_Success)
    {
        // Error, the TSI driver cannot save the configuration.
    }

  \endcode
* \param instance       The TSI module instance.
* \param mode           The requested new operation mode.
* \param operationMode  The pointer to the storage place of the configuration that should be saved.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_SaveConfiguration(uint32_t instance, const nt_tsi_modes_t mode, nt_tsi_operation_mode_t * operationMode);

/**
* \brief Temporary block of TSI measurement
*
*
 \code
   NT_TSI_DRV_Suspend(0) 
  \endcode
* \param instance       The TSI module instance.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_Suspend(uint32_t instance);

/**
* \brief Resumes measurement after Suspend
*
*
 \code
   NT_TSI_DRV_Resume(0) 
  \endcode
* \param instance       The TSI module instance.
* \return An error code or kStatus_TSI_Success.
*/
tsi_status_t NT_TSI_DRV_Resume(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/** \} */ /* end of tsi_drivers_api group */ 
/** \} */ /* end of tsi_drivers group */

#endif
#endif /* _FSL_TSI_DRIVER_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
