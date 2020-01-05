/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nt_modules.h"
#include "nt_module_gpio.h"
#include "../source/system/nt_system_prv.h"
#include "../source/modules/nt_module_gpio_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "../source/filters/nt_filters_prv.h"
#include "../source/modules/nt_modules_prv.h"

#include "fsl_common.h"


/* Local functions */
static int32_t nt_gpio_check_interface(const struct nt_module_data *module);
int32_t nt_gpio_data_check(const struct nt_module_data *module);

static int32_t _nt_module_gpio_init(struct nt_module_data *module);
static int32_t _nt_module_gpio_process(struct nt_module_data *module);
static int32_t _nt_module_gpio_trigger(struct nt_module_data *module);
static int32_t _nt_module_gpio_electrode_enable(struct nt_module_data *module,
                                   uint32_t elec_index);
static int32_t _nt_module_gpio_electrode_disable(struct nt_module_data *module,
                                    uint32_t elec_index);
static int32_t _nt_module_gpio_change_mode(struct nt_module_data *module, 
                                         const enum nt_module_mode mode, 
                                         const struct nt_electrode * electrode);

/** interface gpio module */
const struct nt_module_interface nt_module_gpio_interface = {
  .init              = _nt_module_gpio_init,
  .trigger           = _nt_module_gpio_trigger,
  .process           = _nt_module_gpio_process,
  .recalibrate       = NULL,
  .electrode_enable  = _nt_module_gpio_electrode_enable,
  .electrode_disable = _nt_module_gpio_electrode_disable,
  .change_mode       = _nt_module_gpio_change_mode,
  .name              = NT_MODULE_GPIO_NAME,
  .params_size       = sizeof(struct nt_module_gpio_params),
};

/*******************************************************************************
*                       GPIO MODULE functions
*******************************************************************************/
static int32_t _nt_module_gpio_init(struct nt_module_data *module)
{
  module->special_data.gpio = _nt_mem_alloc(sizeof(struct nt_module_gpio_data));
  
  if(module->special_data.gpio == NULL)
  {
    return NT_OUT_OF_MEMORY;
  }

  if (nt_gpio_data_check(module) < NT_SUCCESS) 
  {
      return NT_FAILURE;
  }
  
  const struct nt_module_gpio_user_interface *interface = 
    module->rom->module_params.gpio->user_interface;

  interface->init_timer();
  
  uint32_t electrode_counter = module->electrodes_cnt;
  
  while (electrode_counter--) 
  {
    /* Get the pointer to electrode. */
    const struct nt_electrode *elec = module->electrodes[electrode_counter]->rom;
    
    /* default state for all pins. */
    interface->set_pin_default_state(elec->pin_input >> NT_GPIO_PORT_SHIFT,
                                                 elec->pin_input & 0xFFU);
  }
 
  return NT_SUCCESS;
}

static int32_t _nt_module_gpio_trigger(struct nt_module_data *module)
{
  _nt_module_set_flag(module, NT_MODULE_NEW_DATA_FLAG);
  return NT_SUCCESS;
}

static int32_t _nt_module_gpio_process(struct nt_module_data *module)
{
  uint32_t electrode_counter = module->electrodes_cnt;
  const struct nt_module_gpio_user_interface *interface = 
    module->rom->module_params.gpio->user_interface;
  
  while (electrode_counter--) 
  {
    struct nt_electrode_data *elec = module->electrodes[electrode_counter];
    uint32_t port = elec->rom->pin_input >> NT_GPIO_PORT_SHIFT;
    uint32_t pin = elec->rom->pin_input & 0xFFU;
    uint32_t overrun = 0;
    uint32_t disabledInterrupts;
    interface->set_pin_output(port, pin);
    
    disabledInterrupts = __get_PRIMASK();
    /* all interrupts MUST be disabled to achieve valuable measurement */
    if(!disabledInterrupts)
    {
      DisableGlobalIRQ();
    }
    
    interface->set_pin_low(port, pin);
    interface->timer_reset_counter();
    interface->start_timer();
    interface->set_pin_input(port, pin);
    while (!interface->get_pin_value(port, pin))
    {
      if(interface->timer_get_overrun())
      {
        overrun = 1;
        break;
      }
    };
    uint32_t signal = interface->timer_get_counter();

    if(!disabledInterrupts)
    {
      EnableGlobalIRQ(disabledInterrupts);
    }
    
    interface->set_pin_default_state(port, pin);
    
    if(!overrun)
    {
      _nt_electrode_set_raw_signal(elec, signal);
      
      /* call general keydetector */
      elec->rom->keydetector_interface->nt_keydetector_measure(elec, signal);
      elec->rom->keydetector_interface->nt_keydetector_process(elec); 
    }
  }
  
  _nt_module_set_flag(module, NT_MODULE_NEW_DATA_FLAG);
  _nt_system_modules_data_ready(module);
  
  return NT_SUCCESS;
}

static int32_t _nt_module_gpio_electrode_enable(struct nt_module_data *module,
                            const uint32_t elec_index)
{
  if (elec_index < module->electrodes_cnt) 
  {
    module->special_data.gpio->pen |= (uint32_t)(1U << elec_index);
  }
  return NT_SUCCESS;
}

static int32_t _nt_module_gpio_electrode_disable(struct nt_module_data *module,
                             const uint32_t elec_index)
{
  if (elec_index < module->electrodes_cnt) 
  {
    module->special_data.gpio->pen &= ~(uint32_t)(1U << elec_index);
  }
  return NT_SUCCESS;
}

static int32_t _nt_module_gpio_change_mode(struct nt_module_data *module, const enum nt_module_mode mode, const struct nt_electrode * electrode)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(electrode != NULL);
  
  if(mode == NT_MODULE_MODE_NORMAL)
  {
    return NT_SUCCESS;
  }else
  {
    return NT_NOT_SUPPORTED;
  }
}

static int32_t nt_gpio_check_interface(const struct nt_module_data *module)
{
  const struct nt_module_gpio_user_interface *interface = 
    module->rom->module_params.gpio->user_interface;
  if (interface->set_pin_output == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->set_pin_input == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->set_pin_low == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->set_pin_high == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->init_timer == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->start_timer == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->stop_timer == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->timer_reset_counter == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->timer_get_counter == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->timer_get_overrun == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->set_pin_default_state == NULL) 
  {
    return NT_FAILURE;
  } else if (interface->get_pin_value == NULL) 
  {
    return NT_FAILURE;
  }
  return NT_SUCCESS;
}

int32_t nt_gpio_data_check(const struct nt_module_data *module)
{
  if (module->rom->module_params.gpio->user_interface == NULL) 
  {
      return NT_FAILURE;
  }
  if (nt_gpio_check_interface(module) < NT_SUCCESS) 
  {
      return NT_FAILURE;
  }
  return NT_SUCCESS;
}
