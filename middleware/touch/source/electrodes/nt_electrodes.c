/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_electrodes.h"
#include "../source/modules/nt_modules_prv.h"
#include "../include/nt_system.h"

/* internal function */
struct nt_module_data *nt_electrode_get_module_data(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);

  struct nt_module_data **modules = _nt_system_get()->modules;
  /* try to find in each module */
  while(*modules != NULL)
  {
    struct nt_module *module = (struct nt_module *)(*modules)->rom;
    struct nt_electrode **electrodes = (struct nt_electrode **)(module)->electrodes;

    while(*electrodes != NULL)
    {
      if(electrode == *electrodes)
      {
        return *modules;
      }
      electrodes++;
    }
    modules++;
  }
  return NULL;
}

/* internal function */
int32_t _nt_electrode_get_index_from_module(const struct nt_module * module, const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  uint32_t e = 0;

  struct nt_electrode **electrodes = (struct nt_electrode **)module->electrodes;
  e = 0;
  while(*electrodes != NULL)
  {
    if(electrode == *electrodes)
    {
      return e;
    }
    e++;
    electrodes++;
  }

  return -1;
}

/* internal function */
struct nt_electrode_data *_nt_electrode_get_data(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  uint32_t m = 0;
  int32_t e = 0;

  struct nt_module **modules = (struct nt_module **)_nt_system_get()->rom->modules;
  /* try to find in each module */
  while(*modules != NULL)
  {
    e = _nt_electrode_get_index_from_module(*modules, electrode);
    
    if(e >= 0)
    {
      return _nt_system_get()->modules[m]->electrodes[e];
    }
    
    m++;
    modules++;
  }

  return NULL;
}

/* Public function */
struct nt_electrode_data *nt_electrode_get_data(const struct nt_electrode *electrode)
{
  return _nt_electrode_get_data(electrode);  
}

/* internal function */
uint32_t _nt_electrode_shielding_process(struct nt_electrode_data *electrode,
                                         uint32_t signal)
{
  NT_ASSERT(electrode != NULL);
  struct nt_electrode_data *shielding_electrode = electrode->shielding_electrode;
  int32_t delta_signal;
  int32_t delta_signal_sh;
  int32_t tmp_signal;
  uint8_t sh_threshold;
  uint8_t sh_gain;
  uint8_t sh_attn;
  uint16_t sh_sens;
  
  if (shielding_electrode != NULL) 
  {        
    sh_threshold = electrode->rom->shield_threshold;
    sh_gain =      electrode->rom->shield_gain;        
    sh_sens =      electrode->rom->shield_sens;
    sh_attn =      electrode->rom->shield_attn;
    
    if((sh_threshold == 0U))
    {
      sh_threshold = NT_ELECTRODE_SHIELD_THRESHOLD;
    }
    
    if(sh_gain == 0U)
    {
      sh_gain = NT_ELECTRODE_SHIELD_GAIN;
    }
    
    if(sh_sens == 0U)
    {
      sh_sens = NT_ELECTRODE_SHIELD_SENS;
    }
    
    if(sh_attn == 0U)
    {
      sh_attn = NT_ELECTRODE_SHIELD_ATTN;
    }
    uint32_t shielding_signal = (uint16_t)shielding_electrode->signal;
    
    delta_signal_sh  = (shielding_signal - shielding_electrode->baseline) * sh_gain; 
    delta_signal = signal - electrode->baseline; 
    
    if((delta_signal > sh_threshold) && (delta_signal_sh > sh_threshold))
    {
      /* Signal delta normalization on 16-bit range */
      tmp_signal = (delta_signal_sh * 0xFFFFU) / delta_signal;  
      tmp_signal =  ((tmp_signal * delta_signal_sh)/0xFFFFU);
      
      tmp_signal = tmp_signal / sh_attn; /*attenuate the shielding effect*/  
      /* limiting the maximum compensation value */
      if(tmp_signal > sh_sens)
      {
        tmp_signal = sh_sens;
      }
      
      tmp_signal =  signal - tmp_signal;
        
      /* avoid the negative baseline drop */
      if(tmp_signal > ((electrode->baseline) + sh_threshold)) 
      { 
        signal = tmp_signal; 
      }
      else
      {
        signal = ((electrode->baseline) + sh_threshold);
      }
    }
  }
  return signal;
}

/* internal function */
uint32_t _nt_electrode_normalization_process(const struct nt_electrode_data *electrode,
                                            uint32_t signal)
{
    NT_ASSERT(electrode != NULL);
    /* signal here is unsigned - division */
    if (electrode->rom->multiplier > 0U) {
        if (electrode->rom->multiplier < (0xFFFFU / signal)) {
            signal *=  electrode->rom->multiplier;
        } else {
            signal = 0xFFFFU;
        }
    }
    if (electrode->rom->divider > 0U) {
        signal /= electrode->rom->divider;
    }
    return signal;
}

struct nt_electrode_data *_nt_electrode_init(struct nt_module_data * module, const struct nt_electrode *electrode)
{
    NT_ASSERT(electrode != NULL);
         
    struct nt_electrode_data *elec = _nt_mem_alloc(sizeof(struct nt_electrode_data));
    if (elec == NULL)
    {
      return NULL;
    }

    elec->rom = electrode;

    _nt_electrode_set_status(elec, NT_ELECTRODE_STATE_INIT);
    elec->status_index = 0U; 
    if(electrode->shielding_electrode != NULL)
    {
      int32_t index = _nt_electrode_get_index_from_module(module->rom, electrode->shielding_electrode); /* get the shielding electrode number */
      if(index < 0)
      {
        return NULL;
      }
      elec->shielding_electrode = module->electrodes[index];
    }else
    {
      elec->shielding_electrode = NULL;
    }
    return elec;
}


int32_t nt_electrode_enable(const struct nt_electrode *electrode, uint32_t touch)
{
  NT_ASSERT(electrode != NULL);
  int32_t res;
  struct nt_module_data *module_data = nt_electrode_get_module_data(electrode);
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  NT_ASSERT(module_data != NULL);
  NT_ASSERT(module_data->rom->interface->electrode_enable != NULL);

  res = module_data->rom->interface->electrode_enable(module_data, electrode->pin_input);
  
  if(res != NT_SUCCESS)
      return res;
	  
  electrode_data->signal = 0;
  
  electrode->keydetector_interface->nt_keydetector_enable(electrode_data, touch);
  
  _nt_electrode_set_flag(electrode_data, NT_ELECTRODE_ENABLED);
  
  return NT_SUCCESS; 
}

int32_t nt_electrode_resume(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  int32_t res;
  struct nt_module_data *module_data = nt_electrode_get_module_data(electrode);
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  NT_ASSERT(module_data != NULL);
  NT_ASSERT(module_data->rom->interface->electrode_enable != NULL);

  res = module_data->rom->interface->electrode_enable(module_data, electrode->pin_input);
  
  if(res != NT_SUCCESS)
      return res;
	    
  _nt_electrode_set_flag(electrode_data, NT_ELECTRODE_ENABLED);
  
  return NT_SUCCESS; 
}
int32_t nt_electrode_disable(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);

  struct nt_module_data *module_data = nt_electrode_get_module_data(electrode);
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);

  NT_ASSERT(module_data != NULL);
  NT_ASSERT(module_data->rom->interface->electrode_disable != NULL);
  
  _nt_electrode_clear_flag(electrode_data, NT_ELECTRODE_ENABLED);
  
  return module_data->rom->interface->electrode_disable(module_data, electrode->pin_input);
}

int32_t nt_electrode_suspend(const struct nt_electrode *electrode)
{
  return nt_electrode_disable(electrode);
}

int32_t nt_electrode_reset(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
    
  electrode->keydetector_interface->nt_keydetector_reset(electrode_data); 
  return NT_SUCCESS; 
}

uint32_t nt_electrode_is_enabled(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);

  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  
  return _nt_electrode_get_flag(electrode_data, NT_ELECTRODE_ENABLED);
}

uint32_t nt_electrode_get_signal(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  return _nt_electrode_get_signal(electrode_data);
}

int32_t nt_electrode_get_last_status(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  return _nt_electrode_get_last_status(electrode_data);
}

uint32_t nt_electrode_get_time_offset(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  return _nt_electrode_get_time_offset(electrode_data);
}
  
uint32_t nt_electrode_get_last_time_stamp(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
    
  NT_ASSERT(electrode_data != NULL);
  
  return _nt_electrode_get_last_time_stamp(electrode_data);
}

uint32_t nt_electrode_get_raw_signal(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  struct nt_electrode_data * electrode_data = _nt_electrode_get_data(electrode);
  NT_ASSERT(electrode_data != NULL);
  
  return _nt_electrode_get_raw_signal(electrode_data);
}
  
uint32_t _nt_electrode_get_signal(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return electrode->signal;
}

const struct nt_electrode * _nt_electrode_get_shield(const struct nt_electrode *electrode)
{
  NT_ASSERT(electrode != NULL);
  
  return electrode->shielding_electrode;
}

/* internal function */
void _nt_electrode_set_signal(struct nt_electrode_data *electrode,
                             uint32_t signal)
{
    NT_ASSERT(electrode != NULL);
    electrode->signal = (uint16_t)signal;
}

/* internal function */
void _nt_electrode_set_raw_signal(struct nt_electrode_data *electrode,
                                 uint32_t signal)
{
    NT_ASSERT(electrode != NULL);
    electrode->raw_signal = (uint16_t)signal;
}

uint32_t _nt_electrode_get_raw_signal(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return (int32_t)electrode->raw_signal;
}

/* internal function */
void _nt_electrode_set_status(struct nt_electrode_data *electrode,
                             int32_t state)
{
    NT_ASSERT(electrode != NULL);
    uint32_t index = (uint32_t)electrode->status_index;

    if (++index >= NT_ELECTRODE_STATUS_HISTORY_COUNT) {
        index = 0U;
    }
    electrode->status_index = (uint8_t)index;
    electrode->status[index].state = (uint8_t)state;
    uint32_t time_stamp = nt_system_get_time_counter();
    electrode->status[index].time_stamp = time_stamp;
}

int32_t _nt_electrode_get_status(const struct nt_electrode_data *electrode,
                                uint32_t index)
{
    NT_ASSERT(electrode != NULL);
    if (index < NT_ELECTRODE_STATUS_HISTORY_COUNT) {
        return (int32_t)electrode->status[index].state;
    } else {
        return NT_FAILURE;
    }
}

int32_t _nt_electrode_get_last_status(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return _nt_electrode_get_status(electrode, electrode->status_index);
}

int32_t _nt_electrode_get_delta(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return (_nt_electrode_get_signal(electrode) - (uint32_t)electrode->baseline);
}

uint32_t _nt_electrode_get_time_stamp(const struct nt_electrode_data *electrode,
                                     uint32_t index)
{
    NT_ASSERT(electrode != NULL);
    if (index < NT_ELECTRODE_STATUS_HISTORY_COUNT) {
        return electrode->status[index].time_stamp;
    } else {
        return 0U;
    }
}

uint32_t _nt_electrode_get_last_time_stamp(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return _nt_electrode_get_time_stamp(electrode, electrode->status_index);
}

uint32_t _nt_electrode_get_time_offset(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    return nt_system_get_time_offset(_nt_electrode_get_last_time_stamp(electrode));
}

uint32_t _nt_electrode_get_time_offset_period(const struct nt_electrode_data *electrode,
                                             uint32_t event_period)
{
    NT_ASSERT(electrode != NULL);
    if (event_period) {
        uint32_t time_offset = _nt_electrode_get_time_offset(electrode);
        uint32_t time_period = (uint32_t)_nt_system_get_time_period();
        return ((time_offset / time_period) % event_period);
    } else {
        return 1U; /* does not perform function if period is 0 */
    }
}

uint32_t _nt_electrode_is_touched(const struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode != NULL);
    uint32_t touched = 0;

    if (_nt_electrode_get_status(electrode, electrode->status_index) ==
        NT_ELECTRODE_STATE_TOUCH) {
        touched = 1;
    }
    return touched;
}
