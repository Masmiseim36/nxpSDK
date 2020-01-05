/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/electrodes/nt_electrodes_prv.h"
#include "../source/system/nt_system_prv.h"
#include "nt_keydetector_safa_prv.h"
#include "nt_keydetectors.h"
#include "../source/system/nt_system_mem_prv.h"

static int32_t _nt_keydetector_safa_rom_check(const struct nt_keydetector_safa *rom);
static void _nt_keydetector_safa_signal_track(struct nt_electrode_data *electrode,
                                                const struct nt_keydetector_safa *rom,
                                                struct nt_keydetector_safa_data *ram,
                                                uint16_t signal);
static void _nt_reset_keydetector_safa_reset(struct nt_electrode_data *electrode, uint32_t signal, uint32_t touch);
static void _nt_keydetector_safa_lock_baseline(struct nt_electrode_data *electrode,
                                                const struct nt_keydetector_safa *rom, 
                                                struct nt_keydetector_safa_data *ram, 
                                                uint16_t signal);

static void _nt_keydetector_safa_unlock_baseline(struct nt_electrode_data *electrode,
                                                const struct nt_keydetector_safa *rom, 
                                                struct nt_keydetector_safa_data *ram, 
                                                uint16_t signal);

static int32_t _nt_keydetector_safa_init(struct nt_electrode_data *electrode);
static void _nt_keydetector_safa_measure(struct nt_electrode_data *electrode,
                      uint32_t signal);
static void _nt_keydetector_safa_process(struct nt_electrode_data *electrode);
static void _nt_keydetector_safa_enable(struct nt_electrode_data *electrode, uint32_t touch);

static void _nt_keydetector_safa_reset(struct nt_electrode_data *electrode);

const struct nt_keydetector_safa nt_keydetector_safa_default = 
{
    .signal_filter = {.coef1 = 2},
    .base_avrg = {.n2_order = 9},
    .non_activity_avrg =  {.n2_order = NT_FILTER_MOVING_AVERAGE_MAX_ORDER},
    .entry_event_cnt = 4,
    .signal_to_noise_ratio = 8,
    .deadband_cnt = 5,
    .min_noise_limit = 20,
};

const struct nt_keydetector_interface nt_keydetector_safa_interface = {
  .nt_keydetector_init    = _nt_keydetector_safa_init,
  .nt_keydetector_process = _nt_keydetector_safa_process,
  .nt_keydetector_measure = _nt_keydetector_safa_measure,
  .nt_keydetector_enable  = _nt_keydetector_safa_enable,
  .nt_keydetector_reset   = _nt_keydetector_safa_reset,
  .name                   = "nt_keydetector_safa_interface",
  .params_size            = sizeof(struct nt_keydetector_safa),
};

static int32_t _nt_keydetector_safa_rom_check(const struct nt_keydetector_safa *rom)
{
    
    return NT_SUCCESS;
}

static int32_t _nt_keydetector_safa_init(struct nt_electrode_data *electrode)
{
  NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

  const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;

  if(_nt_keydetector_safa_rom_check(rom) != NT_SUCCESS)
  {
    return NT_FAILURE;
  }

  electrode->keydetector_data.safa = (struct nt_keydetector_safa_data *) _nt_mem_alloc(sizeof(struct nt_keydetector_safa_data));

  if(electrode->keydetector_data.safa == NULL)
  {
      return NT_OUT_OF_MEMORY;
  }
  electrode->keydetector_data.safa->recovery_cnt = 0;
  
  electrode->keydetector_data.safa->base_avrg_init = rom->base_avrg;
  electrode->keydetector_data.safa->noise_avrg_init = rom->base_avrg;
  electrode->keydetector_data.safa->noise_avrg_init.n2_order += 2;

  if(electrode->keydetector_data.safa->noise_avrg_init.n2_order > NT_FILTER_MOVING_AVERAGE_MAX_ORDER)
      electrode->keydetector_data.safa->noise_avrg_init.n2_order = NT_FILTER_MOVING_AVERAGE_MAX_ORDER;
  
  /* fast noise filter */
  electrode->keydetector_data.safa->f_noise_avrg_init = rom->base_avrg;
  electrode->keydetector_data.safa->f_noise_avrg_init.n2_order = electrode->keydetector_data.safa->noise_avrg_init.n2_order - 4;
  if(electrode->keydetector_data.safa->f_noise_avrg_init.n2_order < 6)
    electrode->keydetector_data.safa->f_noise_avrg_init.n2_order = 6;
  
  electrode->keydetector_data.safa->predicted_signal_avrg_init = rom->base_avrg;
  if(electrode->keydetector_data.safa->predicted_signal_avrg_init.n2_order > 2)
    electrode->keydetector_data.safa->predicted_signal_avrg_init.n2_order -= 2;
  
  _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_INIT);
  
  return NT_SUCCESS;
}

static void _nt_keydetector_safa_enable(struct nt_electrode_data *electrode, uint32_t touch)
{
  struct nt_keydetector_safa_data *ram = electrode->keydetector_data.safa;
  
  if(touch)
  {
      _nt_electrode_set_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
  }
  else
  {
      _nt_electrode_clear_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
  }
  ram->filter_state = NT_FILTER_STATE_INIT;
  _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_INIT);
}

static void _nt_keydetector_safa_measure(struct nt_electrode_data *electrode, uint32_t signal)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram = electrode->keydetector_data.safa;

    
    signal = _nt_electrode_normalization_process(electrode, signal);

    if (ram->filter_state == NT_FILTER_STATE_INIT) {
        _nt_reset_keydetector_safa_reset(electrode, signal, _nt_electrode_get_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG));       
        ram->filter_state = NT_FILTER_STATE_RUN;
    } else {
        signal = _nt_electrode_shielding_process(electrode, signal);
        uint32_t prev_signal = (uint32_t)_nt_electrode_get_signal(electrode);
        uint32_t iir_signal = _nt_filter_iir_process(&rom->signal_filter, signal, prev_signal);
        _nt_electrode_set_signal(electrode, iir_signal);
        
        _nt_keydetector_safa_signal_track(electrode, rom, ram, iir_signal);
    }
    
}


static void _nt_reset_keydetector_safa_reset(struct nt_electrode_data *electrode, uint32_t signal, uint32_t touch)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram = electrode->keydetector_data.safa;

    _nt_electrode_clear_flag(electrode, 
                             NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG | 
                             NT_ELECTRODE_LOCK_BASELINE_FLAG | 
                             NT_ELECTRODE_DIGITAL_RESULT_ONLY_FLAG | 
                             NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG
                             );
    
    if(!touch)
    {    
      _nt_electrode_set_signal(electrode, signal);
      
      ram->base_avrg_init.n2_order = rom->base_avrg.n2_order;
      _nt_filter_moving_average_init(&ram->base_avrg_init, 
                                     &ram->base_avrg,
                                     signal);
      electrode->baseline = electrode->signal;
      _nt_filter_moving_average_init(&ram->noise_avrg_init, 
                                     &ram->noise_avrg,
                                     rom->min_noise_limit); 
      /* fast noise filter */
      _nt_filter_moving_average_init(&ram->f_noise_avrg_init,  
                                     &ram->f_noise_avrg,
                                     rom->min_noise_limit);

      ram->predicted_signal = signal + rom->min_noise_limit * rom->signal_to_noise_ratio;
      _nt_filter_moving_average_init(&rom->non_activity_avrg, 
                                     &ram->predicted_signal_avrg,
                                     ram->predicted_signal);

      ram->noise = rom->min_noise_limit;
      ram->f_noise = rom->min_noise_limit;
      
      _nt_electrode_clear_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG);
    }
    else
    {
      _nt_electrode_set_signal(electrode, signal);
      
      electrode->baseline = _nt_filter_limit_u(((signal / 4) * 3), 0, 50000);

      ram->base_avrg_init = rom->non_activity_avrg;
      _nt_filter_moving_average_init(&ram->base_avrg_init,
                                     &ram->base_avrg,
                                     electrode->baseline);
      
      _nt_filter_moving_average_init(&ram->noise_avrg_init,
                                     &ram->noise_avrg,
                                     rom->min_noise_limit); 
      /* fast noise filter */
      _nt_filter_moving_average_init(&ram->f_noise_avrg_init, 
                                     &ram->f_noise_avrg,
                                     rom->min_noise_limit);

      ram->predicted_signal = signal;
      _nt_filter_moving_average_init(&ram->predicted_signal_avrg_init,
                                     &ram->predicted_signal_avrg,
                                     ram->predicted_signal);

      ram->noise = rom->min_noise_limit;
      ram->f_noise = rom->min_noise_limit;
      
      _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
      _nt_electrode_set_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
}

static void _nt_keydetector_safa_process(struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram = electrode->keydetector_data.safa;
    struct nt_kernel *system = _nt_system_get();

    int32_t signal = (int32_t)_nt_electrode_get_signal(electrode);
    int32_t delta = _nt_filter_abs(electrode->baseline - signal);
    
    uint16_t sig_filter = signal;
    
      switch (_nt_electrode_get_last_status(electrode))
      {
        case NT_ELECTRODE_STATE_INIT:
            /* manage switch from electrode init to run phase */
            if (_nt_electrode_get_time_offset(electrode) >= system->rom->init_time)
            {
              ram->entry_event_cnt = 0;
              ram->deadband_cnt = rom->deadband_cnt;
                            
              if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
              {  
                if(sig_filter < (ram->noise * rom->signal_to_noise_ratio))
                {
                  sig_filter = ram->noise * rom->signal_to_noise_ratio;
                }
                _nt_filter_moving_average_init(&ram->predicted_signal_avrg_init, 
                   &ram->predicted_signal_avrg, sig_filter);
                _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_TOUCH);
                _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
              }
              else
              { 
                _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_RELEASE);
                _nt_filter_moving_average_init(&rom->non_activity_avrg, 
                               &ram->predicted_signal_avrg,
                               ram->predicted_signal);
                _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, signal);
              }              
            }
          break;
        case NT_ELECTRODE_STATE_TOUCH:
            if((delta < (_nt_filter_abs(ram->predicted_signal - electrode->baseline) * 4/5)) && (ram->deadband_cnt == 0)) /* 80% release thresh */             
            {
              ram->entry_event_cnt = 0;
              ram->deadband_cnt = rom->deadband_cnt;
              _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_RELEASE);
              _nt_filter_moving_average_init(&rom->non_activity_avrg, 
                               &ram->predicted_signal_avrg,
                               ram->predicted_signal);
              
              /* is init touch */
              if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
              {
                if(!_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG))
                  _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, signal);
              }
            }else
            {
                ram->entry_event_cnt = 0;
                if(ram->deadband_cnt)
                {
                    ram->deadband_cnt--;
                }
            }
            

          break;
        case NT_ELECTRODE_STATE_RELEASE:
              if(((delta > (_nt_filter_abs(ram->predicted_signal - electrode->baseline) / 4))) &&
                 (ram->deadband_cnt == 0) &&
                 (!_nt_filter_is_deadrange_u(signal, electrode->baseline, (ram->noise * rom->signal_to_noise_ratio))))
              {
                ram->entry_event_cnt++;
                    
                if(ram->entry_event_cnt > rom->entry_event_cnt) /* debouncing */
                {                                
                    ram->entry_event_cnt = 0;
                    ram->deadband_cnt = rom->deadband_cnt;
                  
                  if(!_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG))
                    _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
                  
                  _nt_filter_moving_average_init(&ram->predicted_signal_avrg_init, 
                     &ram->predicted_signal_avrg,
                     _nt_filter_deadrange_u(signal, electrode->baseline, 
                                            (ram->noise * rom->signal_to_noise_ratio)));
                    _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_TOUCH);

                }
                
              }
              
              else
              {
                ram->entry_event_cnt = 0;
                
                if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG))
                {
                  if(ram->deadband_cnt)
                  {
                    ram->deadband_cnt--;
                    break;
                  }
                }
                else
                {
                  if(ram->recovery_cnt)
                  {
                      ram->recovery_cnt--;
                      if(ram->recovery_cnt == 0)
                      {
                        ram->base_avrg_init = rom->base_avrg;
                        _nt_filter_moving_average_init(&ram->base_avrg_init, 
                                 &ram->base_avrg,
                                 electrode->baseline);
                      }
                      break;
                  }
                }  

                ram->deadband_cnt = 0;
                ram->recovery_cnt = 0;
                  
                if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG))
                    _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
                
                if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG | NT_ELECTRODE_LOCK_BASELINE_FLAG) == NT_ELECTRODE_LOCK_BASELINE_FLAG)
                    _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, signal);
                
              }
            break;
        default:
            break;
      }
}

static void _nt_keydetector_safa_lock_baseline(struct nt_electrode_data *electrode, const struct nt_keydetector_safa *rom, struct nt_keydetector_safa_data *ram, uint16_t signal)
{
  _nt_electrode_set_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG);
                  
  ram->base_avrg_init = rom->non_activity_avrg;
  _nt_filter_moving_average_init(&ram->base_avrg_init, 
                               &ram->base_avrg,
                               electrode->baseline);
}

static void _nt_keydetector_safa_unlock_baseline(struct nt_electrode_data *electrode, const struct nt_keydetector_safa *rom, struct nt_keydetector_safa_data *ram, uint16_t signal)
{
  if(_nt_electrode_get_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
  {
      ram->recovery_cnt = rom->deadband_cnt * 8;
      ram->base_avrg_init.n2_order = rom->base_avrg.n2_order / 4;
      _nt_electrode_clear_flag(electrode, NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
      
  }else
  {
      ram->recovery_cnt = rom->deadband_cnt * 2;
      ram->base_avrg_init.n2_order = rom->base_avrg.n2_order / 2;
  }
  
  _nt_filter_moving_average_init(&ram->base_avrg_init, 
                               &ram->base_avrg,
                               electrode->baseline);
    
  _nt_electrode_clear_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG);
}

static void _nt_keydetector_safa_signal_track(struct nt_electrode_data *electrode, const struct nt_keydetector_safa *rom, struct nt_keydetector_safa_data *ram, uint16_t signal)
{
  if(ram->entry_event_cnt == 0)
  {
    /* Just use the direction of change and limit it as minimal noise is defined */
    int32_t delta = _nt_filter_range_s(signal - electrode->baseline, (rom->min_noise_limit / 2));
    uint16_t loc_signal = electrode->baseline + delta;
    
    electrode->baseline = _nt_filter_moving_average_process(&ram->base_avrg_init, 
                                           &ram->base_avrg,
                                           loc_signal);
  }
    
  if(!_nt_electrode_get_flag(electrode, NT_ELECTRODE_LOCK_BASELINE_FLAG))
  {
    if(_nt_filter_abs(electrode->baseline - signal) < (ram->noise * rom->signal_to_noise_ratio))
    {
        ram->noise = _nt_filter_moving_average_process(&ram->noise_avrg_init, 
                                               &ram->noise_avrg,
                                              _nt_filter_limit_u(_nt_filter_abs(electrode->baseline - signal), rom->min_noise_limit, (rom->min_noise_limit * rom->signal_to_noise_ratio * 2))); /* max noise level step adaptation factor */
        
        ram->f_noise = _nt_filter_moving_average_process(&ram->f_noise_avrg_init, 
                                               &ram->f_noise_avrg,
                                              _nt_filter_limit_u(_nt_filter_abs(electrode->baseline - signal), rom->min_noise_limit, (rom->min_noise_limit * rom->signal_to_noise_ratio * 2))); /* Fast Noise */
        
        /* Noise level recovery */
        if((ram->noise > (rom->min_noise_limit )) && (ram->noise > (ram->f_noise) )) 
        {
          /* if fast noise decreases 2x time faster than slowly accumulated value, then recovery slow noise level */
          if((ram->noise - rom->min_noise_limit) > 2*(ram->f_noise -  rom->min_noise_limit)) 
          
          {
            _nt_filter_moving_average_init(&ram->noise_avrg_init, 
                                           &ram->noise_avrg,
                                           ram->f_noise);  
          }
        }

    }
    /* Deadband auxiliary calculation */
    ram->deadband_h = (electrode->baseline + (ram->noise * rom->signal_to_noise_ratio));
    ram->deadband_l = (electrode->baseline - (ram->noise * rom->signal_to_noise_ratio));
    
  }
  
  if(_nt_electrode_get_last_status(electrode) != NT_ELECTRODE_STATE_TOUCH)
  {
	    uint16_t sig_filter = signal;
    
            sig_filter = _nt_filter_abs(signal - electrode->baseline);
            
	    if(sig_filter < (ram->noise * rom->signal_to_noise_ratio))
              sig_filter = ram->noise * rom->signal_to_noise_ratio;
                  
            sig_filter += electrode->baseline;
            
            sig_filter = _nt_filter_limit_u(sig_filter, 0, 65535);    
            ram->predicted_signal = _nt_filter_moving_average_process(&rom->non_activity_avrg, 
	                                           &ram->predicted_signal_avrg,
	                                           sig_filter);
  }else
  {
    ram->predicted_signal = _nt_filter_deadrange_u(_nt_filter_moving_average_process(&ram->predicted_signal_avrg_init, 
                                               &ram->predicted_signal_avrg,
                                               signal), electrode->baseline, (ram->noise * rom->signal_to_noise_ratio));
  }
  
}


static void _nt_keydetector_safa_reset(struct nt_electrode_data *electrode)
{
  uint32_t signal = _nt_electrode_get_raw_signal(electrode);
  signal = _nt_electrode_normalization_process(electrode, signal);
  
  _nt_reset_keydetector_safa_reset(electrode, signal, 0);

}