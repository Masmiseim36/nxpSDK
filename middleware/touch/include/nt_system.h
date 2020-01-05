/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_SYSTEM_H_
#define _NT_SYSTEM_H_

#include "nt_electrodes.h"
#include "nt_modules.h"
#include "nt_controls.h"

/**
 * \defgroup system System
 * \ingroup ntapi
 * The System structure represents the NXP Touch Library in the user application;
 * it is represented by the \ref nt_system structure, which contains further references to
 * all other application objects like: \ref electrodes, \ref detectors, \ref modules, and
 * \ref controls.
 *
 * The nt_system structure is allocated and initialized by the user, in order to define
 * library configuration, including low-level electrode hardware channels and high-level
 * control parameters. Just like all other structure types, it is up to the user, whether
 * an instance of this structure is allocated statically in compile-time or dynamically.
 * The examples provided with the NXP Touch library show the static allocation
 * and initialization of nt_system along with all other related data structures.
 *
 * \{
 */

/** System callbacks events.
 */
enum nt_system_event {                                                                               
    NT_SYSTEM_EVENT_OVERRUN           = 0, /*!< Data has been overrun                                */
    NT_SYSTEM_EVENT_DATA_READY        = 1, /*!< New data are available                               */
    NT_SYSTEM_EVENT_MODULE_DATA_READY = 2, /*!< New data measured                                    */
    NT_SYSTEM_EVENT_DATA_OVERFLOW     = 3, /*!< Measured data overflow (HW out of range)             */
    NT_SYSTEM_EVENT_SIGNAL_LOW        = 4, /*!< Measured data overflow (HW out of range)             */
    NT_SYSTEM_EVENT_SIGNAL_HIGH       = 5, /*!< Measured data overflow (HW out of range)             */
    NT_SYSTEM_EVENT_ELEC_SHORT_VDD    = 6, /*!< Possible Short to VDD on electrode pin detected      */
    NT_SYSTEM_EVENT_ELEC_SHORT_GND    = 7, /*!< Possible Short to GND on electrode pin detected      */
    NT_SYSTEM_EVENT_ELEC_SHORT_ADJ    = 8, /*!< Possible Short to Adjacent on electrode pin detected */
    NT_SYSTEM_EVENT_CRC_FAILED        = 9, /*!< CRC failure on the system cfg data detected          */
};

union nt_system_event_context {
  struct 
  {
    struct nt_electrode * electrode;
  }sig_out_of_limit;
  
  struct nt_module_data *module_data;
};

/* forward declaration */
struct nt_module;
struct nt_control;

/** System event callback function pointer type.
 * \param event  Event type \ref nt_system_event that caused the callback function call.
 * \return      None.
 */
typedef void (* nt_system_callback)(uint32_t event, union nt_system_event_context * context);

/** Error callback function pointer type.
 * \param file  The name of the file where the error occurs.
 * \param line  The line index in the file where the error occurs.
 * \return      None.
 */
typedef void (* nt_error_callback)(char *file_name, uint32_t line);

/**
 * The main structure representing the NXP Touch library;
 * The structure contains pointer lists referring to all other objects used in the
 * application, such as \ref electrodes, \ref detectors, \ref modules, and \ref controls.
 *
 * The nt_system structure and all referred structures are allocated and initialized by
 * the user code, in order to define the library configuration. This configuration affects
 * all library layers from the low-level electrode parameters (for example hardware pins and channels)
 * up to the high-level control parameters (for example slider range or keypad multiplexing).
 *
 * Just like with all other structure types, it is up to the user, whether the instance of
 * this structure is allocated statically in the compile-time, or dynamically. Examples
 * provided with the NXP Touch library show the static allocation and initialization
 * of the nt_system, along with all other related data structures.
 *
 * This structure can be allocated in ROM.
 */
struct nt_system {
    const struct nt_control * const * controls;    /*!< A pointer to the list of controls. Can't be NULL. */
    const struct nt_module  * const * modules;     /*!< A pointer to the list of modules. Can't be NULL. */
    char                    * gui_path;            /*!< GUI path to read xml configuration */    
    uint16_t                time_period;           /*!< Defined time period (triggering period). Can't be 0.*/
    uint16_t                init_time;             /*!< Initialization time for the system. */
    uint16_t                safety_period_multiple;/*!< Multiplier of the time period Time period for safety process 
                                                        function execution. If same as time period the safety process
                                                        will be executed each NT system period. To avoid safety process 
                                                        execution set zero */   
    uint8_t                 safety_crc_hw;         /*!< true if the crc hw module present on chip will be used */

};

/**
 * \defgroup system_api API Functions
 * \ingroup system
 * General Function definition of the system.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif   
  
/**
 * \brief Register the system callback function.
 * \param callback Pointer to the callback function, which will receive the system event notifications.
 * \return none
 * This is an example of installing and using the parameters of the NT library system events handler:
 * \code
 *  static void my_nt_system_callback(uint32_t event);
 *  
 *  // To catch the system events, install the system handler
 *  nt_system_register_callback(my_nt_system_callback)
 *
 *  // The NT system events handling routine
 *  static void my_nt_system_callback(uint32_t event)
 *  {
 *    if(event == NT_SYSTEM_EVENT_OVERRUN)
 *    {  
 *      printf("\nThe measured data has been overrun. Call more frequently nt_task();");
 *    }
 *    else if(event == NT_SYSTEM_EVENT_DATA_READY)
 *    {  
 *      printf("\nThere is new data in the NT library.);
 *    }
 *  }
 *
 * \endcode
 */
void nt_system_register_callback(nt_system_callback callback);

/**
 * \brief Register the system error callback function.
 * \param callback Pointer to the callback function, which will receive the error event notifications.
 * \return none
 * After this callback finishes, the driver falls to a never ending loop.
 * This is an example of installing and using the parameters of the NT library error handler:
 * \code
 *  static void my_nt_error_callback(char *file_name, uint32_t line);
 *  
 *  // For library debugging only, install the error handler
 *  nt_error_register_callback(my_nt_error_callback)
 *
 *  // The NT error-handling routine
 *  static void my_nt_error_callback(char *file_name, uint32_t line)
 *  {
 *    printf("\nError occurred in the NT library. File: %s, Line: %d.\n", file_name, line);
 *  }
 *
 * \endcode
 */
void nt_error_register_callback(nt_error_callback callback);
/**
 * \brief Returns the system time counter.
 * \return Time counter value.
 * This is an example of getting the current time of the NT library:
 * \code
 *  // Printing the current NXP Touch library time
 *  printf("The current NT library time is: &d ms since start.\n", nt_system_get_time_counter());
 *
 * \endcode
 */
uint32_t nt_system_get_time_counter(void);

/**
 * \brief Returns the safety period.
 * \return Returns the safety period calculated as system time period and safety period multioplier.
 * This is an example of getting the current time of the NT library:
 * \code
 *  // Printing the current NXP Touch library safety period 
 *  printf("The NT library safety period is: &d ms since start.\n", _nt_system_get_safety_period());
 *
 * \endcode
 */
uint32_t _nt_system_get_safety_period(void);


/**
 * \brief Returns the system time counter offset.
 * \return Time counter offset value.
 * This is an example of getting the current time of the NT library:
 * \code
 *  // Printing the current NXP Touch library time offset
 *  printf("The NT library time offset is: &d ms since start.\n", nt_system_get_time_offset());
 *
 * \endcode
 */
uint32_t nt_system_get_time_offset(uint32_t event_stamp);

/**
 * \brief Returns the free memory size in the NT memory pool
 * \return size of unused memory in the NT memory pool
 *
 * This can be used in debugging of the driver to specify the exact size of the 
 * NXP Touch memory pool needed.
 * This is an example of initializing the NT library and checking the final size:
 * \code
 *  uint8_t nt_memory_pool[512];
 *  
 *  if(nt_init(&my_nt_system_params, nt_memory_pool, sizeof(nt_memory_pool)) == NT_FAILURE)
 *  {
 *    printf("Initialization of the NT failed. There may be a problem with the memory size,
 *    or invalid parameters in the complemented parameter structures.");
 *  }
 *  // The NT is successfully initialized 
 *
 *  printf("The unused memory size is: &d Bytes. The memory pool can be reduced 
 *  by this size.", nt_mem_get_free_size());
 *
 * \endcode
 */
uint32_t nt_mem_get_free_size(void);

#ifdef __cplusplus
}
#endif

/** \} */ /* end of system_api group */
/** \} */ /* end of system group */

#endif /* _NT_SYSTEM_H_ */
