/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nt_system.h"
#include "../source/system/nt_system_mem_prv.h"
#include "../source/system/nt_system_frmstr_prv.h"
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_electrodes.h"
#include "../source/electrodes/nt_electrodes_prv.h"

/* internal function */
struct nt_control_data *_nt_control_get_data(const struct nt_control *control)
{
  NT_ASSERT(control != NULL);
  uint32_t i = 0;

  struct nt_control **controls = (struct nt_control **)_nt_system_get()->rom->controls;

  while(*controls != NULL)
  {
    if(*controls == control)
    {
      return _nt_system_get()->controls[i];
    }
    i++;
    controls++;
  }
  return NULL;
}

int32_t _nt_control_check_data(const struct nt_control_data *control)
{
  int32_t result = NT_SUCCESS;

  if (control->rom == NULL)
  {
    result = NT_FAILURE;
  }

  else if (control->rom->interface == NULL)
  {
    result = NT_FAILURE;
  }

  if (control->data.general == NULL)
  {
    result = NT_FAILURE;
  }

  if (control->rom->electrodes == NULL)
  {
    result = NT_FAILURE;
  }
  return result;
}

struct nt_control_data *_nt_control_init(const struct nt_control *control)
{
  NT_ASSERT(control != NULL);

  struct nt_control_data *_this = _nt_mem_alloc(sizeof(struct nt_control_data));

  if(_this == NULL)
  {
      return NULL;
  }

  _this->rom = control;
  _this->electrodes_size = nt_control_count_electrodes(_this->rom);

  _this->electrodes = _nt_mem_alloc(sizeof(struct nt_electrode_data*) * _this->electrodes_size);

  if(_this->electrodes == NULL)
  {
      return NULL;
  }

  uint32_t i;
  for(i=0; i < _this->electrodes_size; i++)
  {
    _this->electrodes[i] = _nt_electrode_get_data(control->electrodes[i]);
  }

  if(control->interface->init != NULL)
  {
      if(control->interface->init(_this) < NT_SUCCESS)
      {
          return NULL; /* failure stops the entire init phase */
      }
  }

  if (_nt_control_check_data(_this) != NT_SUCCESS) {
      return NULL;
  }
  
  if(_nt_freemaster_add_variable(control->interface->name, "nt_control_interface", (void*) control->interface, sizeof(struct nt_control_interface)) != NT_SUCCESS)
  {
    return NULL;
  }

  return _this;
}

void nt_control_enable(const struct nt_control *control)
{
  NT_ASSERT(control != NULL);
  
  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);
  
  _nt_control_set_flag(control_data, NT_CONTROL_EN_FLAG);
}

void nt_control_disable(const struct nt_control *control)
{
  NT_ASSERT(control != NULL);
  
  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);
  
  _nt_control_clear_flag(control_data, NT_CONTROL_EN_FLAG);
}

int32_t nt_control_get_touch_button(const struct nt_control *control,
                                    uint32_t index)
{
  NT_ASSERT(control != NULL);
  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT((control_data != NULL));
  
  uint32_t elec_counter = control_data->electrodes_size;

  for (uint32_t i = index; i < elec_counter; i++) {
      if (_nt_electrode_get_last_status(control_data->electrodes[i]) == NT_ELECTRODE_STATE_TOUCH) {
          return i;
      }
  }
  return NT_FAILURE;
}

uint64_t _nt_control_get_electrodes_state(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    uint32_t elec_counter = control->electrodes_size;
    uint64_t current_state = 0U;

    while (elec_counter--) {
        uint64_t electrode_state = _nt_electrode_is_touched(control->electrodes[elec_counter]);
        /* all elec status in a bit field */
        current_state |= (electrode_state << elec_counter);
    }
    return current_state;
}

uint32_t _nt_control_get_electrodes_digital_state(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    uint32_t elec_counter = control->electrodes_size;
    uint32_t current_state = 0U;

    while (elec_counter--) {
        /* all electrode status in a bit field */
		if(_nt_electrode_get_flag(control->electrodes[elec_counter], NT_ELECTRODE_DIGITAL_RESULT_ONLY_FLAG | NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
        	current_state |= (1 << elec_counter);
    }
    return current_state;
}

uint64_t nt_control_get_electrodes_state(struct nt_control *control)
{
  NT_ASSERT(control != NULL);
  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);
    
  return _nt_control_get_electrodes_state(control_data);
}

/* Internal function */
void _nt_control_set_flag_all_elec(struct nt_control_data *control,
                                  uint32_t flag)
{
    NT_ASSERT(control != NULL);
    uint32_t elec_counter = control->electrodes_size;

    while (elec_counter--) {
        _nt_electrode_set_flag(control->electrodes[elec_counter], flag);
    }
}

/* Internal function */
void _nt_control_clear_flag_all_elec(struct nt_control_data *control, uint32_t flag)
{
    NT_ASSERT(control != NULL);
    uint32_t elec_counter = control->electrodes_size;

    while (elec_counter--) {
        _nt_electrode_clear_flag(control->electrodes[elec_counter], flag);
    }
}

int32_t _nt_control_check_neighbours_electrodes(struct nt_control_data *control,
                                                uint32_t first, 
                                                uint32_t second, 
                                                uint32_t overrun)
{
    uint32_t result;
    uint32_t elec_size = control->electrodes_size;
    
    if (first > second) {
        result = first - second;
    } else {
        result = second - first;
    }
    
    if(overrun)
    {
      if ((result > 1U) && (result == (elec_size - 1U))) 
      {
          result = 1; /* valid for arotary */
      }
    }
    
    return (result > 1U) ? NT_INVALID_RESULT : NT_SUCCESS;
}

int32_t _nt_control_check_edge_electrodes(struct nt_control_data *control,
                                                uint32_t electrode_ix)
{
    
    uint32_t elec_size = control->electrodes_size;
    
    if((electrode_ix == 0) || (electrode_ix == (elec_size-1)))
      return NT_SUCCESS;
    
    return NT_INVALID_RESULT;
}

uint32_t nt_control_count_electrodes(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    uint32_t elec_counter = 0U;

    while (control->electrodes[elec_counter] != NULL) {
        elec_counter++;
    }

    return elec_counter;
}

const struct nt_electrode *nt_control_get_electrode(const struct nt_control *control,
                                                                  uint32_t index)
{
  NT_ASSERT((control != NULL));
  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT((control_data != NULL));
  
  if (control_data->electrodes_size > index) {
      return control->electrodes[index];
  } else {
      return NULL;
  }
}

uint32_t _nt_control_get_first_elec_touched(uint32_t current_state)
{
    NT_ASSERT(current_state != 0U);
    uint32_t first_elec_index = 0U;

    while (((current_state >> first_elec_index) & 0x1U) == 0U) {
        first_elec_index++;
    }
    return first_elec_index;
}

uint32_t _nt_control_get_last_elec_touched(uint32_t current_state)
{
    NT_ASSERT(current_state != 0U);
    uint32_t last_elec_index = 0U;

    while (current_state >> last_elec_index) {
        last_elec_index++;
    }
    return last_elec_index;
}

uint32_t _nt_control_get_touch_count(uint32_t current_state)
{
    uint32_t touch_count = 0U;
    uint32_t i_mask = 1U;
    
    while (current_state) 
    {
      if(current_state & i_mask)
      {
        current_state &= ~i_mask;
        touch_count++;
      }
      i_mask <<= 1U; 
    }
    return touch_count;
}

