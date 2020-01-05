/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULES_PRV_H_
#define _NT_MODULES_PRV_H_

#include "nt_types.h"
#include "nt_filters.h"

/**
 * \defgroup modules_private Modules
 * \ingroup ntapi_private
 * Modules represent the data-acquisition layer in the NXP Touch system,
 * it is the layer that is tightly coupled to hardware module available on
 * the NXP MCU device.
 *
 * Each Module implements a set of private functions contained in the nt_modules_prv.h
 * file.
 *
 * \{
 */

/**
 * \defgroup gmodules_private General API
 * \ingroup modules_private
 * General API and definition over all modules.
 *
 * \{
 */
   
/* Forward declaration */
struct nt_module;
struct nt_module_gpio_data;
struct nt_module_gpioint_data;
struct nt_module_tsi_data;
struct nt_module_cs_data;

/**
 * The module optional run-time data.
 *
 */
union nt_module_special_data
{
  struct nt_module_gpio_data    * gpio;          /*!< GPIO module run-time data */
  struct nt_module_gpioint_data * gpioint;       /*!< GPIO interrupt module run-time data */
  struct nt_module_tsi_data     * tsi;           /*!< TSI module run-time data */
  struct nt_module_cs_data      * cs;            /*!< CS module run-time data */
};

/**
 *  Module RAM structure used to store volatile parameters, flags, and other data to enable
 *  a generic behaviour of the Module. This is the main internal structure for a module in
 *  the NT library. A list of pointers to the electrode RAM data structure is created.
 */
struct nt_module_data {
  const struct nt_module        * rom;                  /*!< Pointer to the module parameters defined by the user. */
  struct nt_electrode_data      ** electrodes;          /*!< Pointer to the list of electrodes. Can't be NULL. */
  enum nt_module_mode           active_mode;            /*!< Active mode of the module. */
  uint32_t                      flags;                  /*!< Module's symptoms. */
  uint32_t                      safety_last_time;       /*!< System time of last safety process function execution */ 
  uint32_t                      safety_delay_last_time; /*!< System time of last safety delay function execution */ 
  uint8_t                       safety_last_test;       /*!< Numer of the last executed safety test */
  uint8_t                       safety_last_adj_test;   /*!< Numer of the last executed adj safety test */
  uint8_t                       electrodes_cnt;         /*!< Electrode's count. */  
  union nt_module_special_data  special_data;           /*!< Pointer to the special data (for example run-time data for the GPIO). */
};

/**
 *  Module interface structure; each module uses this structure to register the entry points
 *  to its algorithms. This approach enables a kind-of polymorphism in the touch System.
 *  All modules are processed the same way from the System layer, regardless of the specific
 *  implementation. Each module type defines one static constant structure of this type to
 *  register its own initialization, triggering, processing functions, and functions for enabling
 *  or disabling of electrodes, low power, and proximity.
 */
struct nt_module_interface
{
  int32_t (* init)(struct nt_module_data *module);              /*!< The initialization of the module. */
  int32_t (* trigger)(struct nt_module_data *module);           /*!< Send a trigger event into the module to perform hardware reading of the touches. */
  int32_t (* process)(struct nt_module_data *module);           /*!< Process the read data from the trigger event. */
  int32_t (* recalibrate)(struct nt_module_data *module, void *configuration);       /*!< Force recalibration of the module in the current mode. */
  int32_t (* electrode_enable)(struct nt_module_data *module, const uint32_t elec_index);       /*!< Enable the module electrode in hardware. */
  int32_t (* electrode_disable)(struct nt_module_data *module, const uint32_t elec_index);      /*!< Disable the module electrode in hardware. */
  int32_t (* change_mode)(struct nt_module_data *module, const enum nt_module_mode mode, const struct nt_electrode * electrode); /*!< Change the the mode of the module. */
  int32_t (* load_configuration)(struct nt_module_data *module, const enum nt_module_mode mode, const void* config); /*!<  Load the configuration for the selected mode. */
  int32_t (* save_configuration)(struct nt_module_data *module, const enum nt_module_mode mode, void* config); /*!<  Save the configuration of the selected mode. */
  const char* name;                                             /*!< A name of the variable of this type, used for FreeMASTER support purposes. */
  const uint32_t params_size;                                   /*!< Structure size */ 
};

/**
 * \defgroup modules_api_private API functions
 * \ingroup gmodules_private
 * General Private Function definition of the modules.
 *
 * \{
 */ 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Get the module data structure pointer.
 * \param module Pointer to the module user parameter structure.
 * \return Pointer to the data module structure that is represented by the handled user parameter structure pointer.        
 */
struct nt_module_data *_nt_module_get_data(const struct nt_module *module);

/**
 * \brief Init module.
 * \param module Pointer to the module to be initialized.
 * \return The result of the operation.
 */
struct nt_module_data *_nt_module_init(const struct nt_module *module);

/**
 * \brief Trigger the start of measure event of the module.
 * \param module Pointer to the module to be triggered.
 * \return The result of the operation.
 */
int32_t _nt_module_trigger(struct nt_module_data *module);

/**
 * \brief Process the module.
 * \param module Pointer to the module to be processed.
 * \return The result of the operation.
 */
int32_t _nt_module_process(struct nt_module_data *module);

/**
 * \brief Process module Safety tests
 * \param module Pointer to the module under test.
 * \return The result of the operation.
 */
int32_t _nt_module_process_safety(struct nt_module_data *module);

/**
 * \brief Get the module electrodes state
 * \param module Pointer to the NT module_data.
 * \return mode.
 */
uint32_t _nt_module_get_electrodes_state(struct nt_module_data *module);

/**
 * \brief Check the module electrodes signal levels
 * \param module Pointer to the NT module_data.
 * \return mode.
 */
int32_t _nt_module_check_signal_levels(struct nt_module_data *module);

uint32_t _nt_module_count_electrodes(const struct nt_module *module);

/**
 * \brief Set the flag of the module.
 * \param module Pointer to the NT module.
 * \param flags The flags to be set.
 * \return void
 */
static inline void _nt_module_set_flag(struct nt_module_data *module,
                                      uint32_t flags)
{
    NT_ASSERT(module != NULL);
    module->flags |= flags;
}

/**
 * \brief Reset the flag of the module.
 * \param module Pointer to the NT module.
 * \param flags The flags to be cleared.
 * \return void
 */
static inline void _nt_module_clear_flag(struct nt_module_data *module,
                                        uint32_t flags)
{
    module->flags &= (~flags);
}

/**
 * \brief Return the flag of the module.
 * \param module Pointer to the NT module.
 * \param flags The flags to be tested
 * \return Non-zero if any of the tested flags are set. This is bit-wise AND of
 *     the control flags and the flags parameter.
 */
static inline uint32_t _nt_module_get_flag(struct nt_module_data *module,
                                          uint32_t flags)
{
    return (module->flags & flags);
}

/**
 * \brief Return the instance of the module.
 * \param module Pointer to the NT module.
 * \return instance
 */
static inline uint32_t _nt_module_get_instance(const struct nt_module_data *module)
{
    return module->rom->instance;
}

/**
 * \brief Set the module's mode.
 * \param module Pointer to the NT module.
 * \param mode
 * \return None.
 */
static inline void _nt_module_set_mode(struct nt_module_data *module,
                                      uint32_t mode)
{
    module->active_mode = (enum nt_module_mode)mode;
}

/**
 * \brief Get the module's mode.
 * \param module Pointer to the NT module_data.
 * \return mode.
 */
static inline uint32_t _nt_module_get_mode(struct nt_module_data *module)
{
    return (uint32_t)module->active_mode;
}

#ifdef __cplusplus
}
#endif

/** \} end of modules_api_private group */
/** \} end of gmodules_private group */
/** \} end of modules_private group */
                                        
#endif /* NT_MODULES_PRV_H_ */
