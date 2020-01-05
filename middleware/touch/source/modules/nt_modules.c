/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "../source/system/nt_system_frmstr_prv.h"
#include "nt_modules.h"
#include "../source/modules/nt_modules_prv.h"
#include "nt_keydetectors.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_electrodes.h"
#include "nt_module_gpio.h"

#include "nt_safety.h"
#include "../source/safety/nt_safety_prv.h"


uint32_t _nt_module_count_electrodes(const struct nt_module *module);
static int32_t _nt_module_data_check(const struct nt_module_data *module);
static int32_t _nt_module_init_electrode(struct nt_module_data *module, uint32_t index);

int32_t _nt_module_check_electrodes(struct nt_module_data *module);



static int32_t _nt_module_data_check(const struct nt_module_data *module)
{
  if (module->rom == NULL)
  {
      return NT_FAILURE;
  }
  if (module->rom->electrodes == NULL)
  {
      return NT_FAILURE;
  }

  return NT_SUCCESS;
}

uint32_t _nt_module_count_electrodes(const struct nt_module *module)
{
  uint32_t elec_counter = 0U;

  while (module->electrodes[elec_counter] != NULL)
  {
    elec_counter++;
  }
  return elec_counter;
}

uint32_t _nt_module_get_electrodes_state(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    uint32_t elec_counter = module->electrodes_cnt;
    uint32_t current_state = 0U;

    while (elec_counter--) {
        uint32_t electrode_state = _nt_electrode_is_touched(module->electrodes[elec_counter]);
        /* all elec status in a bit field */
        current_state |= (electrode_state << elec_counter);
    }
    return current_state;
}

/* internal function */
struct nt_module_data *_nt_module_get_data(const struct nt_module *module)
{
  NT_ASSERT(module != NULL);
  uint32_t i = 0;

  struct nt_module **modules = (struct nt_module **)_nt_system_get()->rom->modules;

  while(*modules != NULL)
  {
    if(*modules == module)
    {
      return _nt_system_get()->modules[i];
    }
    i++;
    modules++;
  }

  return NULL;
}

static int32_t _nt_module_init_electrode(struct nt_module_data *module, uint32_t index)
{
  struct nt_electrode_data *elec;

  elec = _nt_electrode_init(module, module->rom->electrodes[index]);

  if(elec == NULL)
  {
    return NT_OUT_OF_MEMORY;
  }

  module->electrodes[index] = elec;
  elec->module_data = module;

  NT_ASSERT(elec->rom->keydetector_interface->nt_keydetector_init != NULL);

  if(elec->rom->keydetector_interface->nt_keydetector_init(elec) != NT_SUCCESS)
  {
     return NT_OUT_OF_MEMORY;
  } /* Initialization keydetectors data */

  if(_nt_freemaster_add_variable(elec->rom->keydetector_interface->name, "nt_keydetector_interface", (void*) elec->rom->keydetector_interface, sizeof(struct nt_keydetector_interface)) != NT_SUCCESS)
  {
    return NT_OUT_OF_MEMORY;
  }

  return NT_SUCCESS;
}

struct nt_module_data *_nt_module_init(const struct nt_module *module)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(module->interface != NULL);
  NT_ASSERT(module->electrodes != NULL);

  struct nt_module_data *_this = _nt_mem_alloc(sizeof(struct nt_module_data));


  if(_this == NULL)
  {
    return NULL;
  }

  _this->electrodes_cnt = (uint8_t)_nt_module_count_electrodes(module);
  _nt_module_set_mode(_this, NT_MODULE_MODE_NORMAL);

  _this->electrodes = _nt_mem_alloc(sizeof(struct nt_electrode_data*) * _this->electrodes_cnt);

  if(_this->electrodes == NULL)
  {
    return NULL;
  }

  _this->rom = module;

  if (_nt_module_data_check(_this) < NT_SUCCESS)
  {
    return NULL;
  }

  uint32_t i;

  for(i=0; i<_this->electrodes_cnt; i++)
  {
    _this->electrodes[i] = NULL;
  }

  for(i=0; i<_this->electrodes_cnt; i++)
  {
    struct nt_electrode const * shield_electrode;

    if(_this->electrodes[i] != NULL)
    {
      continue;
    }

    shield_electrode = _nt_electrode_get_shield(_this->rom->electrodes[i]);

    if(shield_electrode != NULL)
    {
      /* Initialization SH electrode before the electrodes which use it as the shield */
      int32_t  j = _nt_electrode_get_index_from_module(_this->rom, shield_electrode);
      if(j >= 0)
      { /* hot fix of multiple sh electrode initialization */
        if(_this->electrodes[j]->status_index == 0) /* Electrode was not initialized yet */
        {
          if(_nt_module_init_electrode(_this, j) != NT_SUCCESS)
          {
            return NULL;
          }
        }
      }
    }

    if(_nt_module_init_electrode(_this, i) != NT_SUCCESS)
    {
      return NULL;
    }
  }

  if(module->interface->init)
  {
    if(module->interface->init(_this) != NT_SUCCESS)
    {
      return NULL;
    }
  }

  if(_nt_freemaster_add_variable(module->interface->name, "nt_module_interface", (void*) module->interface, sizeof(struct nt_module_interface)) != NT_SUCCESS)
  {
    return NULL;
  }

  /* Initialization the safety functions */
  if(module->safety_interface)
  {
    NT_ASSERT(module->safety_interface->init != NULL);
    if(module->safety_interface->init(_this) != NT_SUCCESS)
    {
      return NULL;
    }
  }

  return _this;
}

int32_t _nt_module_trigger(struct nt_module_data *module)
{
  NT_ASSERT(module != NULL);

  int32_t result;

  if (_nt_module_get_flag(module, NT_MODULE_TRIGGER_DISABLED_FLAG))
  {
    return NT_FAILURE;
  }

  _nt_module_clear_flag(module, NT_MODULE_NEW_DATA_FLAG);

  if(module->rom->interface->trigger)
  {
    _nt_module_set_flag(module, NT_MODULE_BUSY_FLAG); 

    if((result = module->rom->interface->trigger(module)) != NT_SUCCESS)
    {
      return result;
    }
  }

  return NT_SUCCESS;
}
#if (NT_SAFETY_SUPPORT == 1)
int32_t _nt_module_process_safety(struct nt_module_data *module)
{
  NT_ASSERT(module != NULL);
  /* Process safety routines */
  if(!_nt_module_get_flag(module, NT_MODULE_BUSY_FLAG))
  {
    if(module->rom->safety_interface != NULL)
    {
      /* Safety process triggering based on NT system triggering.
      If safety period == 0 safety process function will be executed */
      uint32_t safety_period = _nt_system_get_safety_period();
      uint32_t syst_time = nt_system_get_time_counter();
      if ((syst_time > (module->safety_last_time + safety_period)) && safety_period)
      {
        module->safety_last_time = syst_time;

        /* disable triggering, while safety test in progress */
        _nt_module_set_flag(module, NT_MODULE_TRIGGER_DISABLED_FLAG);

        if (module->rom->safety_interface->process(module) != NT_SUCCESS)
        {
          _nt_module_clear_flag(module, NT_MODULE_TRIGGER_DISABLED_FLAG);
          return NT_FAILURE; /* safety failure */
        }
        _nt_module_clear_flag(module, NT_MODULE_TRIGGER_DISABLED_FLAG); /* re-enable trigger */
      }
    }
  }
  return NT_SUCCESS;
}
#endif /* NT_SAFETY_SUPPORT */

int32_t _nt_module_process(struct nt_module_data *module)
{
  NT_ASSERT(module != NULL);

  if (!_nt_module_get_flag(module, NT_MODULE_NEW_DATA_FLAG))
  {
    return NT_FAILURE; /* no data to be processed */
  }

  /* Give chance to any specific process inside of used module. */
  if(module->rom->interface->process)
  {
    if(module->rom->interface->process(module) != NT_SUCCESS)
    {
      return NT_FAILURE;
    }
  }
  else
  {
    uint32_t el_counter = module->electrodes_cnt;

    while (el_counter--)
    {
      struct nt_electrode_data *elec = module->electrodes[el_counter];
      {
        /* call general key-detector */
        elec->rom->keydetector_interface->nt_keydetector_measure(elec,
                                               _nt_electrode_get_raw_signal(elec));

        elec->rom->keydetector_interface->nt_keydetector_process(elec);
      }
    }
  }

  _nt_module_clear_flag(module, NT_MODULE_NEW_DATA_FLAG);
  return NT_SUCCESS;
}

uint32_t nt_module_recalibrate(const struct nt_module *module)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(module->recalib_config != NULL);  
  
  if(module->interface->recalibrate)
  {
    return (uint32_t)module->interface->recalibrate(_nt_module_get_data(module),(void*)module->recalib_config);
  }

  return NT_SUCCESS;
}

int32_t nt_module_change_mode(struct nt_module *module, const enum nt_module_mode mode, const struct nt_electrode *electrode)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(electrode != NULL);

  struct nt_module_mode_flags
  {
    enum nt_module_mode mode;
    uint32_t         flags_set;
    uint32_t         flags_clear;
  };

  const struct nt_module_mode_flags mode_flags[3] =
  {
    {
      NT_MODULE_MODE_NORMAL,
      0,
      NT_MODULE_TRIGGER_DISABLED_FLAG
    },
    {
      NT_MODULE_MODE_PROXIMITY,
      0,
      NT_MODULE_TRIGGER_DISABLED_FLAG
    },
    {
      NT_MODULE_MODE_LOW_POWER,
      NT_MODULE_TRIGGER_DISABLED_FLAG,
      0
    },
  };

  uint32_t mode_ix;

  struct nt_module_data *module_data = _nt_module_get_data(module);
  NT_ASSERT(module_data != NULL);

  if (module->interface->change_mode == NULL)
  {
    return NT_FAILURE;
  }

  for(mode_ix = 0; mode_ix < 3; mode_ix++)
  {
    if(mode_flags[mode_ix].mode == mode)
      break;
  }

  if(mode_ix >= 3)
  {
    return NT_FAILURE;
  }

  _nt_module_set_flag(module_data, mode_flags[mode_ix].flags_set);


  int32_t result;
  if((result = (module->interface->change_mode(module_data, mode, electrode))) != NT_SUCCESS)
  {
    _nt_module_clear_flag(module_data, mode_flags[mode_ix].flags_set);
    return result;
  }


  _nt_module_clear_flag(module_data, mode_flags[mode_ix].flags_clear);

  if(mode == NT_MODULE_MODE_LOW_POWER)
  {
    _nt_system_increment_time_counter(); /* inc counter for the wake up interrupt */
  }

  _nt_module_set_mode(module_data, mode);

  return NT_SUCCESS;
}

int32_t nt_module_load_configuration(struct nt_module *module, const enum nt_module_mode mode, const void *config)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(config != NULL);

  struct nt_module_data *module_data = _nt_module_get_data(module);

  NT_ASSERT(module_data != NULL);
  int32_t result;

  if(module->interface->load_configuration)
  {
    result = module->interface->load_configuration(module_data, mode, config);

    if(result != NT_SUCCESS)
    {
      return result;
    }
  }

  return NT_SUCCESS;
}

int32_t nt_module_save_configuration(struct nt_module *module, const enum nt_module_mode mode, void *config)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(config != NULL);

  struct nt_module_data *module_data = _nt_module_get_data(module);

  NT_ASSERT(module_data != NULL);
  int32_t result;

  if(module->interface->save_configuration)
  {
    result = module->interface->save_configuration(module_data, mode, config);

    if(result != NT_SUCCESS)
    {
      return result;
    }
  }

  return NT_SUCCESS;
}

int32_t _nt_module_check_signal_levels(struct nt_module_data *module)
{
  uint32_t el_counter = module->electrodes_cnt;
  uint16_t wtrmark_high = module->rom->wtrmark_hi;
  uint16_t wtrmark_low = module->rom->wtrmark_lo;

  union nt_system_event_context context;

  if(wtrmark_low == 0U)
  {
    wtrmark_low = NT_MODULE_SIGNAL_WTRMARK_LOW;
  }
  if(wtrmark_high == 0U)
  {
    wtrmark_high = NT_MODULE_SIGNAL_WTRMARK_HIGH;
  }

    while (el_counter--)
    {
      struct nt_electrode_data *elec = module->electrodes[el_counter];
      if(elec->signal < wtrmark_low)
      {
        context.sig_out_of_limit.electrode = (struct nt_electrode *)elec->rom;
       _nt_system_invoke_callback(NT_SYSTEM_EVENT_SIGNAL_LOW, &context);
      }
      if(elec->signal > wtrmark_high)
      {
        context.sig_out_of_limit.electrode = (struct nt_electrode *)elec->rom;
        _nt_system_invoke_callback(NT_SYSTEM_EVENT_SIGNAL_HIGH, &context);
      }
    }

  return NT_SUCCESS;
}
