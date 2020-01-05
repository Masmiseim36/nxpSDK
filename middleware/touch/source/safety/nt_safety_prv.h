 /*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_SAFETY_PRV_H_
#define _NT_SAFETY_PRV_H_

#include "nt_types.h"

/**
 * \defgroup safety_private safety
 * \ingroup ntapi_private
 * Safety represent the safety check layer in the NXP Touch system,
 * it is the layer that is tightly coupled to hardware available on
 * the NXP MCU device.
 *
 * Each safety implements a set of private functions contained in the nt_safety_prv.h
 * file.
 *
 * \{
 */
/**
 * \defgroup gsafety_private General API
 * \ingroup safety_private
 * General API and definition over all safety.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 *  safety interface structure; safety uses this structure to register the entry points
 *  to its algorithms. This approach enables a kind-of polymorphism in the touch System.
 *  Safety functions are processed the same way from the System layer, regardless of the specific
 *  implementation. 
 */
struct nt_module_safety_interface
{
  int32_t (* init)(struct nt_module_data *module);    /*!< The initialization of the safety functions */
  int32_t (* process)(struct nt_module_data *module); /*!< Process the safety functions */
  const char* name;                                   /*!< A name of the variable of this type, used for FreeMASTER support purposes. */
  const uint32_t params_size;                         /*!< Structure size */ 
};

/**
 *  The matrix RAM structure is used to store volatile parameters of the safety interface.
 *
 *  Allocate this structure and put a pointer into the nt_safety_data
 *  structure when it is being registered in the system.
 */
struct nt_safety_data {
    uint32_t                   safety_delay_next_time;                   /*!< Time instant for the delay interval end */
};


/**
 * \brief Init the safety functions for the module.
 * \param module Pointer to the where the module safety to be initialized.
 * \return The result of the operation.
 */
int32_t _nt_module_safety_init(struct nt_module_data *module);
  
/**
 * \brief Process the safety on the module.
 * \param module Pointer to the where the module safety to be processed.
 * \return The result of the operation.
 */
int32_t _nt_module_safety_process(struct nt_module_data *module);  

  
#ifdef __cplusplus
}
#endif

/** \} */ /* end of gsafety_private group */
/** \} */ /* end of safety_private group */

#endif    /* _NT_SAFETY_PRV_H_ */


