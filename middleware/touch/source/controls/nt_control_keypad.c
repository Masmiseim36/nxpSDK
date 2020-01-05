/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_control_keypad.h"
#include "nt_control_keypad_prv.h"


/* Check changes on keypad's electrode (group electrodes) */
static void _nt_control_keypad_decode_groups(struct nt_control_data *control,
                                    uint32_t changes, uint32_t current_state);
/* Check changes on keypad's electrode (1 electrode - 1 pad) */
static void _nt_control_keypad_decode_simple(struct nt_control_data *control,
                                    uint32_t changes, uint32_t current_state);
/* Invoke callback based on the even has occurred */
static void _nt_control_keypad_invoke_callback(struct nt_control_data *control,
                                      enum nt_control_keypad_event event,
                                      uint32_t index);
/* Handles all things when the key is pressed */
static void _nt_control_keypad_key_press(struct nt_control_data *control, int32_t ix);
/* Handles all things when the key is released */
static void _nt_control_keypad_key_release(struct nt_control_data *control, int32_t ix);

/* Invoke autorepeat callback if time period runs out and an electrode has been touched */
static void _nt_control_keypad_autorepeat(struct nt_control_data *control);
/* The KeyPad control initialization function. */
static int32_t _nt_control_keypad_init(struct nt_control_data *control);
/* The KeyPad control process function. */
static int32_t _nt_control_keypad_process(struct nt_control_data *control);

/* The KeyPad control interface. */
const struct nt_control_interface nt_control_keypad_interface = {
  .init    = _nt_control_keypad_init,
  .process = _nt_control_keypad_process,
  .name    = NT_CONTROL_KEYPAD_NAME,
  .params_size = sizeof(struct nt_control_keypad),
};

void nt_control_keypad_only_one_key_valid(const struct nt_control *control,
                                       uint32_t enable)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->interface == &nt_control_keypad_interface);

  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);

  struct nt_control_keypad_data *ram = control_data->data.keypad;
  NT_ASSERT(ram != NULL);

  if(ram->last_key_state && enable)
  {
    /* Handle cases when the keys are already touched */
    uint32_t elec_counter = control_data->electrodes_size;
    while (elec_counter--) 
    {
      uint32_t el_group = (1U << elec_counter);
      if (el_group & ram->last_key_state) 
      {
        if(elec_counter == ram->last_electrode)
        {
          continue;
        }
        _nt_control_keypad_key_release(control_data, elec_counter);                        
      }
    }
  }
  
  if(enable)
  {
    _nt_control_set_flag(control_data, NT_KEYPAD_ONLY_ONE_KEY_FLAG);
  }
  else
  {
    _nt_control_clear_flag(control_data, NT_KEYPAD_ONLY_ONE_KEY_FLAG);
    ram->last_state = 0;
  }
}

void nt_control_keypad_register_callback(const struct nt_control *control,
                                       nt_control_keypad_callback callback)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->interface == &nt_control_keypad_interface);

  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);

  struct nt_control_keypad_data *ram = control_data->data.keypad;
  NT_ASSERT(ram != NULL);

  ram->callback = callback;
}

void nt_control_keypad_set_autorepeat_rate(const struct nt_control *control,
                                   uint32_t value, uint32_t start_value)
{
  NT_ASSERT(value < 65535); 
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->interface == &nt_control_keypad_interface);

  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);

  struct nt_control_keypad_data *ram = control_data->data.keypad;
  NT_ASSERT(ram != NULL);

  ram->autorepeat_rate = value;
  ram->start_autorepeat_rate = start_value;
}

uint32_t nt_control_keypad_get_autorepeat_rate(const struct nt_control *control)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->interface == &nt_control_keypad_interface);

  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);

  struct nt_control_keypad_data *ram = control_data->data.keypad;
  NT_ASSERT(ram != NULL);

  return ram->autorepeat_rate;
}

uint32_t nt_control_keypad_is_button_touched(const struct nt_control *control,
                                     uint32_t index)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->interface == &nt_control_keypad_interface);

  struct nt_control_data *control_data = _nt_control_get_data(control);
  NT_ASSERT(control_data != NULL);

  struct nt_control_keypad_data *ram = control_data->data.keypad;
  NT_ASSERT(ram != NULL);
  
  return (ram->last_key_state & (1 << index));
}

static void _nt_control_keypad_invoke_callback(struct nt_control_data *control,
                                      enum nt_control_keypad_event event, uint32_t index)
{
  struct nt_control_keypad_data *ram = control->data.keypad;
  NT_ASSERT(ram != NULL);

  if (ram->callback != NULL) 
  {
    ram->callback(control->rom, event, index);
  }
}

static void _nt_control_keypad_key_press(struct nt_control_data *control, int32_t ix)
{
  NT_ASSERT(control != NULL);
  struct nt_control_keypad_data *ram = control->data.keypad;
  NT_ASSERT(ram != NULL);
  
  if(_nt_control_get_flag(control, NT_KEYPAD_ONLY_ONE_KEY_FLAG) && ram->last_key_state)
  {
    return;
  }
  
  /* Add the flags to real keys pressed */
  ram->last_key_state |= 1<<ix;
  ram->last_electrode = ix;
  ram->repeat_time = nt_system_get_time_counter() + 
  ((ram->start_autorepeat_rate != 0)? ram->start_autorepeat_rate : ram->autorepeat_rate);
  
  _nt_control_keypad_invoke_callback(control, NT_KEYPAD_TOUCH, ix);
}

static void _nt_control_keypad_key_release(struct nt_control_data *control, int32_t ix)
{
  NT_ASSERT(control != NULL);
  struct nt_control_keypad_data *ram = control->data.keypad;
  NT_ASSERT(ram != NULL);
  
  if(_nt_control_get_flag(control, NT_KEYPAD_ONLY_ONE_KEY_FLAG))
  {
    if(ram->last_electrode != ix)
    {
      return;
    }else
    {
      ram->last_state = 0; /* Force check all changes */
    }
  }
  
  /* Add the flags to real keys pressed */
  ram->last_key_state &= ~(1<<ix);
  
  if(ram->last_electrode == ix)
  {
    ram->last_electrode = -1;
  }
  
  _nt_control_keypad_invoke_callback(control, NT_KEYPAD_RELEASE, ix);
}

static void _nt_control_keypad_decode_groups(struct nt_control_data *control,
                                    uint32_t changes, uint32_t current_state)
{
  NT_ASSERT(control->rom->interface == &nt_control_keypad_interface);

  const struct nt_control_keypad *keypad = control->rom->control_params.keypad;
  NT_ASSERT(keypad != NULL);

  uint32_t group_counter = keypad->groups_size;

    while (group_counter--) {
        uint32_t el_group = keypad->groups[group_counter];
        if (el_group & changes) {
            if ((el_group & current_state) == el_group) {
                _nt_control_keypad_key_press(control, group_counter);
            } else if (((~current_state) & el_group) == el_group) {
                _nt_control_keypad_key_release(control, group_counter);
            }
        }
    }
}

static void _nt_control_keypad_decode_simple(struct nt_control_data *control,
                                    uint32_t changes, uint32_t current_state)
{
    uint32_t elec_counter = control->electrodes_size;

    while (elec_counter--) {
        uint32_t el_group = (1U << elec_counter);
        if (el_group & changes) {
            /* decode elec state */
            if ((current_state >> elec_counter) & 0x1U) {
                _nt_control_keypad_key_press(control, elec_counter);
            } else {
                _nt_control_keypad_key_release(control, elec_counter);
            }            
        }
    }
}

static void _nt_control_keypad_autorepeat(struct nt_control_data *control)
{
  uint32_t counter = nt_system_get_time_counter();
  
  struct nt_control_keypad_data *ram = control->data.keypad;
  NT_ASSERT(ram != NULL);

  if(ram->last_electrode < 0)
    return;
  
  if (counter >= ram->repeat_time) {
    ram->repeat_time = counter + ram->autorepeat_rate;
    _nt_control_keypad_invoke_callback(control, NT_KEYPAD_AUTOREPEAT, ram->last_electrode);    
  }  
}


static int32_t _nt_control_keypad_init(struct nt_control_data *control)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->rom->interface == &nt_control_keypad_interface);
  NT_ASSERT(control->rom->control_params.keypad != NULL);
  
  const struct nt_control_keypad *keypad = control->rom->control_params.keypad;

  if (keypad->groups != NULL && keypad->groups_size == 0U)
  {
    return NT_FAILURE;
  }
  
  if (keypad->multi_touch != NULL && keypad->multi_touch_size == 0U)
  {
    return NT_FAILURE;
  }

  control->data.keypad = _nt_mem_alloc(sizeof(struct nt_control_keypad_data));

  if(control->data.keypad == NULL)
  {
      return NT_OUT_OF_MEMORY;
  }

  if (_nt_control_check_data(control) != NT_SUCCESS) {
      return NT_FAILURE;
  }
  
  control->data.keypad->last_electrode = -1;
  
  return NT_SUCCESS;
}

static int32_t _nt_control_keypad_process(struct nt_control_data *control)
{
  NT_ASSERT(control != NULL);
  NT_ASSERT(control->rom->interface == &nt_control_keypad_interface);

  const struct nt_control_keypad *keypad = control->rom->control_params.keypad;

  struct nt_control_keypad_data *ram = control->data.keypad;
  NT_ASSERT(ram != NULL);

  if (!_nt_control_get_flag(control, NT_CONTROL_EN_FLAG)) 
  {
    return NT_FAILURE; /* not enabled or data not ready */
  }

  if(_nt_control_get_flag(control, NT_CONTROL_NEW_DATA_FLAG))
  {
    uint64_t current_state = _nt_control_get_electrodes_state(control);
    uint64_t has_changed = current_state ^ ram->last_state;
    
    if (has_changed) 
    {
      ram->last_state = current_state;  
      if (keypad->multi_touch != NULL)
      { uint8_t multi_touch_counter = keypad->multi_touch_size;
        while (multi_touch_counter--) 
        {  
          if (current_state == keypad->multi_touch[multi_touch_counter])
          {
            _nt_control_keypad_invoke_callback(control, NT_KEYPAD_MULTI_TOUCH, multi_touch_counter);       
            _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
            return NT_SUCCESS;
          }
        }
      }

      if (keypad->groups != NULL) 
      {
        _nt_control_keypad_decode_groups(control, has_changed, current_state);
      } else {
          _nt_control_keypad_decode_simple(control, has_changed, current_state);
      }
    }
    _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
  }
      
  if (ram->autorepeat_rate != 0U) {
      _nt_control_keypad_autorepeat(control);
  }
  
  return NT_SUCCESS;
}
