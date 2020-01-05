/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_H_
#define _NT_H_

/* The main NXP Touch header. */
#include "nt_types.h"
#include "nt_system.h"
#include "nt_electrodes.h"

/* Controls */
#include "nt_controls.h"
#include "nt_control_keypad.h"
#include "nt_control_aslider.h"
#include "nt_control_arotary.h"
#include "nt_control_rotary.h"
#include "nt_control_slider.h"
#include "nt_control_proxi.h"
#include "nt_control_matrix.h"

/* Modules */
#include "nt_modules.h"
#if defined(CPU_QN908X) || defined(CPU_QN9080C)
  #include "nt_module_cs.h"
#else
  #include "nt_module_tsi.h"
#endif
#include "nt_module_gpioint.h"
#include "nt_module_gpio.h"
#if (NT_SAFETY_SUPPORT == 1)
  #include "nt_safety.h"
#endif /* NT_SAFETY_SUPPORT */
/* Key Detectors */
#include "nt_keydetector_safa.h"
#include "nt_keydetector_afid.h"
#include "nt_keydetector_usafa.h"

/**
 * \defgroup ntapi NXP Touch User API
 *
 * The functions documented in this module are the primary functions used in the user
 * application that uses the NXP Touch library. The user calls the API
 * functions to give run-time for the NXP Touch measurement and data-processing
 * algorithms. All library callbacks are executed in a context of one of
 * these API calls.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup system_api
 * \{
 */
  
/**
 * \brief NXP Touch Library initialization.
 * \param system Pointer to the NT system parameters structure.
 * \param pool  Pointer to the memory pool what will be used for internal NT data.
 * \param size  Size of the memory pool handled by the parameter pool (needed size depends
 *              on the number of components used in the NT - electrodes, modules, controls, and so on).
 * \return
 *   - NT_SUCCESS if library was properly initialized,
 *   - NT_FAILURE if initialization failed (one of the reasons is not enough memory in the memory pool).
 *
 * This function validates the NXP Touch configuration passed within the
 * nt_system structure. After this call, the system structure becomes the main
 * data provider for the application. There are also created and filled-up internal 
 * volatile data structures used by the driver. It is the user's responsibility to prepare
 * the configuration of all electrodes, modules, and controls in the system structure
 * before calling this function. The application should not execute any other
 * NT library calls if this function returns NT_FAILURE.
 * This is an example of the NT library initialization:
   \code
    uint8_t nt_memory_pool[512];
    
    if(nt_init(&my_nt_system_params, nt_memory_pool, sizeof(nt_memory_pool)) == NT_FAILURE)
    {
      printf("Initialization of NT failed. There can be a problem with the memory size
      or invalid parameters in component parameter structures.");
    }
    // The NT is successfully initialized 
  
   \endcode
 */
int32_t nt_init(const struct nt_system *system, uint8_t *pool, const uint32_t size);

/**
 * \brief NXP Touch Main processing entry point.
 * \return
 *   - NT_SUCCESS when data acquired during the last trigger are now processed
 *   - NT_FAILURE when no new data are ready
 *
 * This function should be called by the application as often as possible, in
 * order to process the data acquired during the data trigger. This function
 * should be called at least once per trigger time.
 *
 * Internally, this function passes the NT_SYSTEM_MODULE_PROCESS and
 * NT_SYSTEM_CONTROL_PROCESS command calls to each object configured in \ref modules and
 * \ref controls.
 * This is an example of running a task of the NT library:
   \code
    uint8_t nt_memory_pool[512];
    
    if(nt_init(&my_nt_system_params, nt_memory_pool, sizeof(nt_memory_pool)) == NT_FAILURE)
    {
      printf("Initialization of NT failed. There can be problem with memory size
      or invalid parameters in component parameter structures.");
    }
    // The NT is successfully initialized
  
    // Main never-ending loop of the application
    while(1)
    {
      if(nt_task() == NT_SUCCESS)
      {
        // New data has been processed
      }
    }

   \endcode
 */
int32_t nt_task(void);

/**
 * \brief Main Trigger function to acquire the touch-sensing data.
 * \return
 *   - NT_SUCCESS when the trigger was performed without any errors or warnings.
 *   - NT_FAILURE when a problem is detected, such as module not ready, overrun
 *               (data loss) error, and so on. Regardless of the error, the trigger
 *               is always initiated.
 *
 * This function should be called by the application periodically in a timer
 * interrupt, or in a task to trigger new data measurement. Depending on the
 * \ref modules implementation, this function may take the data immediately, or
 * may only start the hardware sampling with interrupt enabled.
 * This is an example of the NT library triggering:
   \code
    //For example, there is a callback routine from any periodical source (for example 5 ms)   
    static void Timer_5msCallBack(void)
    {
     if(nt_trigger() != NT_SUCCESS)
      {
        // Trigger error
      }
    }
   \endcode
 */
int32_t nt_trigger(void);

#ifdef __cplusplus
}
#endif

/** \} end of system group */

#endif
