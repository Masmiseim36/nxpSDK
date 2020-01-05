/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../source/system/nt_system_prv.h"
#include "../source/modules/nt_module_tsi_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_modules.h"
#include "nt_module_tsi.h"
#include "../source/filters/nt_filters_prv.h"
#include "../source/modules/nt_modules_prv.h"
#include "../source/safety/nt_safety_prv.h"
#include "nt_safety.h"
#include "../source/safety/nt_safety_gpio.h"   
#include "nt_gpio.h"

/* Call back function of MCUXpresso SDK TSI driver */
static void _nt_module_tsi_driver_callback(uint32_t instance, void* usrData);

/* Local functions */
static void _nt_module_tsi_measure(struct nt_module_data *module);
static int32_t _nt_module_tsi_init(struct nt_module_data *module);
static int32_t _nt_module_tsi_trigger(struct nt_module_data *module);
static int32_t _nt_module_tsi_process(struct nt_module_data *module);
static int32_t _nt_module_tsi_module_recalibrate(struct nt_module_data *module, void *configuration);
static int32_t _nt_module_tsi_electrode_enable(struct nt_module_data *module,
                                   uint32_t elec_index);
static int32_t _nt_module_tsi_electrode_disable(struct nt_module_data *module,
                                    uint32_t elec_index);
static int32_t _nt_module_tsi_change_mode(struct nt_module_data *module, 
                                         const enum nt_module_mode mode, 
                                         const struct nt_electrode * electrode);
/* Local functions for noise mode implementation */
static int32_t _nt_module_tsi_noise_rom_check(const struct nt_module_tsi_noise *rom);
static int32_t _nt_module_tsi_noise_init(struct nt_electrode_data *electrode);
static void _nt_module_tsi_noise_measure(struct nt_electrode_data *electrode, 
                                        uint32_t signal);
static int32_t _nt_module_tsi_noise_process(struct nt_electrode_data *electrode, 
                                           uint32_t maximum_noise);
static int32_t _nt_module_tsi_noise_get_max(struct nt_module_data *module);
                                            
static int32_t _nt_module_tsi_load_configuration(struct nt_module_data *module, const enum nt_module_mode mode, const void* config); /*!<  Load the configuration for select mode. */
static int32_t _nt_module_tsi_save_configuration(struct nt_module_data *module, const enum nt_module_mode mode, void* config); /*!<  Save the configuration of select mode. */

static int32_t _nt_get_tsi_mode(const enum nt_module_mode mode);
static int32_t _nt_update_electrodes_for_noise(struct nt_module_data *module);

/** interface tsi module */
const struct nt_module_interface nt_module_tsi_interface = {
  .init              = _nt_module_tsi_init,
  .trigger           = _nt_module_tsi_trigger,
  .process           = _nt_module_tsi_process,
  .recalibrate       = _nt_module_tsi_module_recalibrate,
  .electrode_enable  = _nt_module_tsi_electrode_enable,
  .electrode_disable = _nt_module_tsi_electrode_disable,
  .change_mode       = _nt_module_tsi_change_mode,
  .load_configuration = _nt_module_tsi_load_configuration,
  .save_configuration = _nt_module_tsi_save_configuration,
  .name              = NT_MODULE_TSI_NAME,
  .params_size       = sizeof(struct nt_module_tsi_params),
};

/*******************************************************************************
*                       TSI MODULE functions
*******************************************************************************/
static int32_t _nt_module_tsi_init(struct nt_module_data *module)
{
  
  module->special_data.tsi = _nt_mem_alloc(sizeof(struct nt_module_tsi_data));
  
  if(module->special_data.tsi == NULL)
  {
    return NT_OUT_OF_MEMORY;
  }
  
  nt_tsi_user_config_t tsi_config;

  //Create the TSI structure
  tsi_config.config = module->rom->config;
  tsi_config.pCallBackFunc = _nt_module_tsi_driver_callback;
  tsi_config.usrData = (void*)_nt_system_get();

  if(NT_TSI_DRV_Init(module->rom->instance, &module->special_data.tsi->tsi_state, &tsi_config) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  if(module->rom->module_params.tsi) /* noise mode parameters detected */
  {
#if (FSL_FEATURE_TSI_VERSION == 4)
        nt_tsi_operation_mode_t tsi_op_mode;
#endif
        
    if(_nt_module_tsi_noise_rom_check(&module->rom->module_params.tsi->noise) == NT_FAILURE)
    {
      return NT_FAILURE;
    }
    
    _nt_module_set_flag(module, NT_MODULE_HAS_NOISE_MODE_FLAG);
    
    for(uint32_t i=0; i<module->electrodes_cnt; i++)
    {
      struct nt_electrode_data *elec = module->electrodes[i];

      if(_nt_module_tsi_noise_init(elec) != NT_SUCCESS)
      {
         return NT_OUT_OF_MEMORY;
      } /* init noise keydetectors data */
    }
#if (FSL_FEATURE_TSI_VERSION == 4)
    
    /* Load settings for the noise mode from noise setup */   
    tsi_op_mode.config.prescaler = module->rom->module_params.tsi->config->prescaler; 
    tsi_op_mode.config.nscn = module->rom->module_params.tsi->config->nscn; 
    tsi_op_mode.config.refchrg = module->rom->module_params.tsi->config->refchrg;   
    tsi_op_mode.config.mode = module->rom->module_params.tsi->config->mode;   
    tsi_op_mode.config.dvolt = module->rom->module_params.tsi->config->dvolt;    
    tsi_op_mode.config.resistor = module->rom->module_params.tsi->config->resistor;   
    tsi_op_mode.config.filter = module->rom->module_params.tsi->config->filter; 
      
    
    if(NT_TSI_DRV_LoadConfiguration(module->rom->instance, tsi_OpModeNoise, &tsi_op_mode) != kStatus_TSI_Success)
    {
      return NT_FAILURE;
    }
        
#endif  
  }
  
  return NT_SUCCESS;
}

static int32_t _nt_module_tsi_trigger(struct nt_module_data *module)
{
  tsi_status_t result = kStatus_TSI_Success;
  
  /* Handle noise mode if possible */
  if(module->rom->module_params.tsi)
  {
    if(_nt_module_get_flag(module, NT_MODULE_HAS_NOISE_MODE_FLAG))
    {
      if((!_nt_module_get_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG)) && ((!_nt_system_get_time_offset_from_period((module->rom->module_params.tsi->noise.update_rate) /_nt_system_get_time_period())) || (_nt_module_get_flag(module, NT_MODULE_NOISE_MODE_REQ_FLAG))))
      {
        /* when no touch on the module electrodes */
        if(!_nt_module_get_electrodes_state(module))
        { 
          /* Switch to noise mode */
          result = NT_TSI_DRV_ChangeMode(module->rom->instance, tsi_OpModeNoise);
          
          if(result == kStatus_TSI_Success)
          {
            _nt_module_set_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG);
            _nt_module_clear_flag(module, NT_MODULE_NOISE_MODE_REQ_FLAG);
          }
          else if(result == kStatus_TSI_InvalidMode)
          {
            _nt_module_clear_flag(module, NT_MODULE_HAS_NOISE_MODE_FLAG | NT_MODULE_NOISE_MODE_REQ_FLAG);
          }else
          {
            _nt_module_set_flag(module, NT_MODULE_NOISE_MODE_REQ_FLAG);
          }
        }
                
      }
      else
      {
        if ((_nt_module_get_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG)) && (!_nt_module_get_flag(module, NT_MODULE_DIGITAL_RESULTS_FLAG)))
        {
          
          if( nt_system_get_time_offset(module->special_data.tsi->noise_timestamp) > module->rom->module_params.tsi->noise.noise_mode_timeout)
          {           
            /* switch back to CAP (no noise result measured in the last process and time out occurred) */
            result = NT_TSI_DRV_ChangeMode(module->rom->instance, tsi_OpModeNormal);
			_nt_module_clear_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG);
          }
        }
      }
    }
  }
  
  if(result == kStatus_TSI_Success)
  {
    result = NT_TSI_DRV_Measure(module->rom->instance);
  }

  switch(result)
  {
    case kStatus_TSI_Success:
    return NT_SUCCESS;

    case kStatus_TSI_Busy:
    return NT_SCAN_IN_PROGRESS;

    default:
    return NT_FAILURE;
  }
}

static void _nt_module_tsi_measure(struct nt_module_data *module)
{
  uint32_t elec_counter = module->electrodes_cnt;
  while (elec_counter--) 
  {
    struct nt_electrode_data *elec = module->electrodes[elec_counter];
    uint32_t raw_signal = _nt_electrode_get_raw_signal(elec);
    
    if(_nt_module_get_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG | NT_MODULE_HAS_NOISE_MODE_FLAG) == (NT_MODULE_IN_NOISE_MODE_FLAG | NT_MODULE_HAS_NOISE_MODE_FLAG)) 
    {
        _nt_module_tsi_noise_measure(elec, raw_signal);
    } else 
    {
        elec->rom->keydetector_interface->nt_keydetector_measure(elec, raw_signal);
    }
  }
}

static int32_t _nt_module_tsi_noise_get_max(struct nt_module_data *module)
{
  uint32_t maximum_noise = 0U;
  struct nt_electrode_data *elec;
  struct nt_module_tsi_noise_data *ram;

  uint32_t elec_counter = module->electrodes_cnt;

  while (elec_counter--) {
      elec = module->electrodes[elec_counter];
      ram = elec->special_data.tsi_noise;
      /* check electrode noise level */
      if (_nt_module_tsi_noise_process(elec, 0U) == NT_SUCCESS) 
      {
          if (maximum_noise < ram->noise) 
          {
              maximum_noise = ram->noise;
          }
      }
  }
  return maximum_noise;
}

static int32_t _nt_module_tsi_process(struct nt_module_data *module)
{
  uint32_t elec_counter;
  
  _nt_module_tsi_measure(module);
  uint32_t cap_process = 1U;

  if(_nt_module_get_flag(module, NT_MODULE_IN_NOISE_MODE_FLAG | NT_MODULE_HAS_NOISE_MODE_FLAG) == (NT_MODULE_IN_NOISE_MODE_FLAG | NT_MODULE_HAS_NOISE_MODE_FLAG)) 
  {
    /* process noise keydetector */
    uint32_t noise_result = _nt_module_tsi_noise_get_max(module);
    elec_counter = module->electrodes_cnt;
    
    if (noise_result > 0U) 
    {
      /* store noise timestamp */
      module->special_data.tsi->noise_timestamp = nt_system_get_time_counter(); 
      while (elec_counter--) 
      {
        struct nt_electrode_data *elec = module->electrodes[elec_counter];
        /* evaluate touch/release */
        (void)_nt_module_tsi_noise_process(elec, noise_result);
        
        _nt_electrode_set_flag(elec, NT_ELECTRODE_DIGITAL_RESULT_ONLY_FLAG);
      }
      _nt_module_set_flag(module, NT_MODULE_DIGITAL_RESULTS_FLAG);
      
      cap_process = 0U;
    }
  }

  if (cap_process) 
  {
      _nt_module_clear_flag(module, NT_MODULE_DIGITAL_RESULTS_FLAG);
      elec_counter = module->electrodes_cnt;
      
      while (elec_counter--) 
      {
          struct nt_electrode_data *elec = module->electrodes[elec_counter];
          elec->rom->keydetector_interface->nt_keydetector_process(elec);
          _nt_electrode_clear_flag(elec, NT_ELECTRODE_DIGITAL_RESULT_ONLY_FLAG);
      }
  }
  return NT_SUCCESS;
}

static int32_t _nt_module_tsi_module_recalibrate(struct nt_module_data *module, void *configuration)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(configuration != NULL);

  tsi_status_t result = kStatus_TSI_Success;

#if (NT_SAFETY_SUPPORT == 1)
  nt_tsi_modes_t curr_mode;
  const nt_tsi_modes_t tsi_opModes[] = {tsi_OpModeNormal, tsi_OpModeProximity, tsi_OpModeLowPower, tsi_OpModeNoise};
  uint32_t mode;
  
  /* Save the current operational mode */
  curr_mode = NT_TSI_DRV_GetMode(module->rom->instance);

  for(mode = 0; mode < sizeof(tsi_opModes); mode++)
  {    
    /* Change modes*/
    if((result = NT_TSI_DRV_ChangeMode(module->rom->instance, tsi_opModes[mode])) == kStatus_TSI_Success)
    {

      /* recalibrate */
      result = NT_TSI_DRV_Recalibrate(module->rom->instance, configuration);
    }              
     
    if(result != kStatus_TSI_Success)
    {
      /* skip to end if failed*/
      break;
    }
  }
  
  /* Return back to the original mode */
  NT_TSI_DRV_ChangeMode(module->rom->instance, curr_mode);
#endif /* NT_SAFETY_SUPPORT */

  return result;
}

static int32_t _nt_module_tsi_electrode_enable(struct nt_module_data *module,
                            const uint32_t elec_index)
{
  if(NT_TSI_DRV_EnableElectrode(module->rom->instance, elec_index, true) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  if(module->rom->module_params.tsi)
  {
    if(_nt_update_electrodes_for_noise(module) != NT_SUCCESS)
    {
      return NT_FAILURE;
    }
  }

  return NT_SUCCESS;
}

static int32_t _nt_module_tsi_electrode_disable(struct nt_module_data *module,
                             const uint32_t elec_index)
{
  if(NT_TSI_DRV_EnableElectrode(module->rom->instance, elec_index, false) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  if(module->rom->module_params.tsi)
  {
    if(_nt_update_electrodes_for_noise(module) != NT_SUCCESS)
    {
      return NT_FAILURE;
    }
  }

  return NT_SUCCESS;
}

static int32_t _nt_update_electrodes_for_noise(struct nt_module_data *module)
{
  nt_tsi_operation_mode_t tsi_op_mode;
  nt_tsi_operation_mode_t tsi_op_mode_2;

  nt_tsi_modes_t mode = NT_TSI_DRV_GetMode(module->rom->instance);
  nt_tsi_modes_t mode_2 = (mode == tsi_OpModeNoise)? tsi_OpModeNormal : tsi_OpModeNoise;

  /* Duplicate settings for the noise mode from normal setup */
  if(NT_TSI_DRV_SaveConfiguration(module->rom->instance, mode, &tsi_op_mode) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }

  if(NT_TSI_DRV_SaveConfiguration(module->rom->instance, mode_2, &tsi_op_mode_2) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }

  tsi_op_mode_2.enabledElectrodes = tsi_op_mode.enabledElectrodes;

  if(NT_TSI_DRV_LoadConfiguration(module->rom->instance, mode_2, &tsi_op_mode_2) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  return NT_SUCCESS;  
}

static int32_t _nt_get_tsi_mode(const enum nt_module_mode mode)
{
  struct modes_cross_table
  {
    enum nt_module_mode nt_modes;
    nt_tsi_modes_t         tsi_modes;
  };

  const struct modes_cross_table modes_table[3] =
  {
    { NT_MODULE_MODE_NORMAL, tsi_OpModeNormal},
    { NT_MODULE_MODE_PROXIMITY, tsi_OpModeProximity},
    { NT_MODULE_MODE_LOW_POWER, tsi_OpModeLowPower},
  };
  
  for(int32_t mode_ix = 0; mode_ix < 3; mode_ix++)
  {
    if(modes_table[mode_ix].nt_modes == mode)
    {
      return modes_table[mode_ix].tsi_modes;
    }
  }
  
  return NT_FAILURE;
}

static int32_t _nt_module_tsi_change_mode(struct nt_module_data *module, const enum nt_module_mode mode, const struct nt_electrode * electrode)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(electrode != NULL);

  int32_t tsi_mode = _nt_get_tsi_mode(mode);

  if(tsi_mode == NT_FAILURE)
  {
    return NT_FAILURE;
  }

  if(NT_TSI_DRV_GetMode(module->rom->instance) == tsi_OpModeLowPower)
  {
    /* Disable the low power mode and switch back driver to normal mode */
    if(NT_TSI_DRV_DisableLowPower(module->rom->instance, tsi_OpModeNoChange) != kStatus_TSI_Success)
    {
      return NT_FAILURE;
    }
  }

    /* Change mode of TSI driver */
  if(NT_TSI_DRV_ChangeMode(module->rom->instance, (nt_tsi_modes_t)tsi_mode) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }

  if((tsi_mode == tsi_OpModeLowPower) || (tsi_mode == tsi_OpModeProximity))
  {
    /* Enable the right electrode for Low Power and Proximity */
    if(NT_TSI_DRV_EnableElectrode(module->rom->instance, electrode->pin_input, true) != kStatus_TSI_Success)
    {
      return NT_FAILURE;
    }
  }

  /* It should be in separate API to allow setup/recalibrate/manage low power */
  if(tsi_mode == tsi_OpModeLowPower)
  {
    /* Enable the low power functionality */
    if(NT_TSI_DRV_EnableLowPower(module->rom->instance) != kStatus_TSI_Success)
    {
      return NT_FAILURE;
    }
  }

  return NT_SUCCESS;
}

static int32_t _nt_module_tsi_load_configuration(struct nt_module_data *module, const enum nt_module_mode mode, const void* config)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(config != NULL);
  
  int32_t tsi_mode = _nt_get_tsi_mode(mode);

  if(tsi_mode == NT_FAILURE)
  {
    return NT_FAILURE;
  }
  
  nt_tsi_operation_mode_t tsi_op_mode;
  
  tsi_op_mode.enabledElectrodes = NT_TSI_DRV_GetEnabledElectrodes(module->rom->instance);
  tsi_op_mode.config = *((tsi_config_t*)config);

  if(NT_TSI_DRV_LoadConfiguration(module->rom->instance, (nt_tsi_modes_t)tsi_mode, &tsi_op_mode) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  return NT_SUCCESS;
}

static int32_t _nt_module_tsi_save_configuration(struct nt_module_data *module, const enum nt_module_mode mode, void* config)
{
  NT_ASSERT(module != NULL);
  NT_ASSERT(config != NULL);
  
  int32_t tsi_mode = _nt_get_tsi_mode(mode);

  if(tsi_mode == NT_FAILURE)
  {
    return NT_FAILURE;
  }
  
  nt_tsi_operation_mode_t tsi_op_mode;
  
  if(NT_TSI_DRV_SaveConfiguration(module->rom->instance, (nt_tsi_modes_t)tsi_mode, &tsi_op_mode) != kStatus_TSI_Success)
  {
    return NT_FAILURE;
  }
  
  *((tsi_config_t*)config) = tsi_op_mode.config;
  
  return NT_SUCCESS;
}

/*******************************************************************************
*                       TSI MODULE NOISE mode functions
*******************************************************************************/
static int32_t _nt_module_tsi_noise_rom_check(const struct nt_module_tsi_noise *rom)
{
    if (rom->noise_filter.coef1 == 0) {
        return NT_FAILURE;
    }
    if (rom->update_rate == 0) {
        return NT_FAILURE;
    }

    return NT_SUCCESS;
}

static int32_t _nt_module_tsi_noise_init(struct nt_electrode_data *electrode)
{
  NT_ASSERT(electrode != NULL);

  struct nt_module_tsi_noise_data *ram = (struct nt_module_tsi_noise_data *)
                        _nt_mem_alloc(sizeof(struct nt_module_tsi_noise_data));

  if(ram == NULL)
  {
      return NT_OUT_OF_MEMORY;
  }

  electrode->special_data.tsi_noise = ram;

  ram->touch_threshold = NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD;
  ram->filter_state = NT_FILTER_STATE_INIT;

  return NT_SUCCESS;
}

static void _nt_module_tsi_noise_measure(struct nt_electrode_data *electrode,
                             uint32_t signal)
{
  NT_ASSERT(electrode != NULL);
  struct nt_module_tsi_noise_data *ram = electrode->special_data.tsi_noise;
  NT_ASSERT(ram != NULL);
  const struct nt_module_tsi_noise *rom = &electrode->module_data->rom->module_params.tsi->noise;
  NT_ASSERT(rom != NULL);

  if (ram->filter_state == NT_FILTER_STATE_INIT)
  {
    ram->noise = (uint8_t)signal;
    ram->filter_state = NT_FILTER_STATE_RUN;
  } else 
  {
    
    ram->noise = (uint8_t)_nt_filter_iir_process(&rom->noise_filter, signal,
                                                ram->noise);  
  }
}


static int32_t _nt_module_tsi_noise_process(struct nt_electrode_data *electrode, uint32_t maximum_noise)
{
  NT_ASSERT(electrode != NULL);
  struct nt_module_tsi_noise_data *ram = electrode->special_data.tsi_noise;
  NT_ASSERT(ram != NULL);

  int32_t result = NT_FAILURE;
  
  
  if (maximum_noise == 0u)
  {
    if (_nt_electrode_get_last_status(electrode) == NT_ELECTRODE_STATE_INIT) {
      /* noise touch threshold calibration (find maximum) */
      if (ram->touch_threshold < ram->noise)
      {
        ram->touch_threshold = ram->noise;
      }
    } else
    {
      /* check if noise is presented */
      if (ram->noise > ram->touch_threshold)
      {
        result = NT_SUCCESS;
      }
    }
  } 
  else /* max_noise */
  {
    
    /* evaluate touch/release */
    if (maximum_noise <= NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD)
    {
      maximum_noise = NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD + 1u;
    }
    
    if (maximum_noise > NT_TSI_NOISE_TOUCH_RANGE)
    {
      maximum_noise = NT_TSI_NOISE_TOUCH_RANGE;           
    }
      
    switch (_nt_electrode_get_last_status(electrode)) {
      case NT_ELECTRODE_STATE_TOUCH:
        if (ram->noise < NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD)
        {
          _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_RELEASE);
        }
        break;
      case NT_ELECTRODE_STATE_RELEASE:
        if (ram->noise >= NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD)
        {
          _nt_electrode_set_status(electrode, NT_ELECTRODE_STATE_TOUCH);
        }
        break;
      default:
        break;
    }
    result = NT_SUCCESS;
  }
  return result;
}

static void _nt_module_tsi_driver_callback(uint32_t instance, void* usrData)
{
  struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
  NT_ASSERT(module != NULL);
  struct nt_electrode_data *elec;
  const uint64_t enabled_electrodes = NT_TSI_DRV_GetEnabledElectrodes(instance);
  uint32_t elec_size  = module->electrodes_cnt;

  while (elec_size--)
  {
    elec = module->electrodes[elec_size];

    if(enabled_electrodes & (1ULL<<(elec->rom->pin_input)))
    {
      uint16_t data;
      NT_TSI_DRV_GetCounter(instance, elec->rom->pin_input, &data);
      _nt_electrode_set_raw_signal(elec, data);
    }
  }
  
  if(module->special_data.tsi->tsi_state.status == kStatus_TSI_Overflow)
  {
    _nt_module_set_flag(module, NT_MODULE_OVERFLOW_FLAG);
    _nt_system_invoke_callback(NT_SYSTEM_EVENT_DATA_OVERFLOW, NULL);
  }
       
  _nt_module_set_flag(module, NT_MODULE_NEW_DATA_FLAG);
  _nt_system_modules_data_ready(module);
}

/*FUNCTION**********************************************************************
*
* Function Name : nt_module_TSI_update
* Description   : Automatically update TSI HW configuration structure used by touch library from pointed structure.
*
*END**************************************************************************/
#if (NT_SAFETY_SUPPORT == 1)
void nt_module_TSI_update (struct nt_module const* module)
{
    struct nt_module_data *module_data;
    module_data = _nt_module_get_data(module);
    
    tsi_config_t * tsi_config;
    tsi_config = module_data->rom->config;  
    nt_tsi_state_t * tsiState = g_tsiStatePtr[0];    
    
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.mainClock   = tsi_config->configSelfCap.commonConfig.mainClock ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.mode        = tsi_config->configSelfCap.commonConfig.mode        ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.dvolt       = tsi_config->configSelfCap.commonConfig.dvolt       ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.cutoff      = tsi_config->configSelfCap.commonConfig.cutoff      ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.decimation  = tsi_config->configSelfCap.commonConfig.decimation  ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.order       = tsi_config->configSelfCap.commonConfig.order       ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.chargeNum   = tsi_config->configSelfCap.commonConfig.chargeNum   ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.noChargeNum = tsi_config->configSelfCap.commonConfig.noChargeNum ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.commonConfig.prbsOutsel  = tsi_config->configSelfCap.commonConfig.prbsOutsel  ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.enableSensitivity        = tsi_config->configSelfCap.enableSensitivity        ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.enableShield             = tsi_config->configSelfCap.enableShield             ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.inputCurrent             = tsi_config->configSelfCap.inputCurrent             ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.xdn                      = tsi_config->configSelfCap.xdn                      ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.chargeCurrent            = tsi_config->configSelfCap.chargeCurrent            ;
    tsiState->opModesData[tsiState->opMode].config.configSelfCap.ctrim                    = tsi_config->configSelfCap.ctrim                    ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.mainClock    = tsi_config->configMutual.commonConfig.mainClock    ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.mode         = tsi_config->configMutual.commonConfig.mode         ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.dvolt        = tsi_config->configMutual.commonConfig.dvolt        ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.cutoff       = tsi_config->configMutual.commonConfig.cutoff       ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.order        = tsi_config->configMutual.commonConfig.order        ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.decimation   = tsi_config->configMutual.commonConfig.decimation   ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.chargeNum    = tsi_config->configMutual.commonConfig.chargeNum    ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.commonConfig.noChargeNum  = tsi_config->configMutual.commonConfig.noChargeNum  ;
    tsiState->opModesData[tsiState->opMode].config.configMutual.preCurrent                = tsi_config->configMutual.preCurrent                ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.preResistor               = tsi_config->configMutual.preResistor               ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.senseResistor             = tsi_config->configMutual.senseResistor             ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.boostCurrent              = tsi_config->configMutual.boostCurrent              ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.txDriveMode               = tsi_config->configMutual.txDriveMode               ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.pmosLeftCurrent           = tsi_config->configMutual.pmosLeftCurrent           ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.pmosRightCurrent          = tsi_config->configMutual.pmosRightCurrent          ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.enableNmosMirror          = tsi_config->configMutual.enableNmosMirror          ;   
    tsiState->opModesData[tsiState->opMode].config.configMutual.nmosCurrent               = tsi_config->configMutual.nmosCurrent               ;   
}

int32_t nt_module_TSI_TouchOneElect (struct nt_electrode* electrode)
{
    NT_ASSERT(electrode != NULL); 

    uint32_t pin = electrode->gpio_input.gpio1 & 0xFFU;
    uint32_t port= electrode->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;  
    set_pin_gpio_mode(port, pin); 
    set_pin_input(port, pin); 
    if(electrode->gpio_input.gpio2 != 0) 
        set_pin_pull_down(port, pin);      
    else 
        set_pin_pull_up(port, pin); 

    return NT_SUCCESS;
}  

int32_t nt_module_TSI_ReleaseOneElect (struct nt_electrode* electrode)
{
    NT_ASSERT(electrode != NULL); 
 
    uint32_t pin = electrode->gpio_input.gpio1 & 0xFFU;                /* Select pin  */
    uint32_t port = electrode->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
    set_pin_tsi_mode(port, pin);   /* Set pinmux to TSI peripheral mode */

    return NT_SUCCESS;
} 

int32_t nt_module_TSI_TouchAll (uint32_t instance)
{
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL); 
    uint32_t elec_counter = module->electrodes_cnt;

    while (elec_counter--) 
    {    
        struct nt_electrode const*electrode = module->rom->electrodes[elec_counter];
        NT_ASSERT(electrode != NULL); 
        uint32_t pin = electrode->gpio_input.gpio1 & 0xFFU;
        uint32_t port= electrode->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;  
        set_pin_gpio_mode(port, pin); 
        set_pin_input(port, pin); 
        if(electrode->gpio_input.gpio2 != 0) 
            set_pin_pull_down(port, pin);      
        else 
            set_pin_pull_up(port, pin); 
    }
    return NT_SUCCESS;
}  

int32_t nt_module_TSI_Touch (uint32_t instance, uint64_t enabledElectrodes)
{
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL); 
    uint32_t elec_counter;
    
    elec_counter = module->electrodes_cnt;
    while (elec_counter--)
    {
        struct nt_electrode const*electrode = module->rom->electrodes[elec_counter];
        NT_ASSERT(electrode != NULL); 
        uint64_t enabledElectrode = 0x1ULL << electrode->pin_input;
        if (enabledElectrodes & enabledElectrode)
        {
            uint32_t pin = electrode->gpio_input.gpio1 & 0xFFU;
            uint32_t port= electrode->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;     
            set_pin_gpio_mode(port, pin); 
            set_pin_input(port, pin); 
            if(electrode->gpio_input.gpio2 != 0) 
                set_pin_pull_down(port, pin);      
            else 
                set_pin_pull_up(port, pin);  
        }
    } 
    return NT_SUCCESS; 
} 

int32_t nt_module_TSI_ReleaseAll (uint32_t instance) 
{
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL);
    
    uint32_t elec_counter = module->electrodes_cnt;

    while (elec_counter--) 
    {    
        struct nt_electrode const* electrode = module->rom->electrodes[elec_counter];
        NT_ASSERT(electrode != NULL); 
        (void)_nt_module_safety_switch_electrode_tsi(electrode);
    }
    return NT_SUCCESS;
}

int32_t nt_module_TSI_Release (uint32_t instance, uint64_t enabledElectrodes) 
{
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL); 
    uint32_t elec_counter;
    
    elec_counter = module->electrodes_cnt;
    while (elec_counter--)
    {
        struct nt_electrode const*electrode = module->rom->electrodes[elec_counter];
        NT_ASSERT(electrode != NULL); 
        uint64_t enabledElectrode = 0x1ULL << electrode->pin_input;
        if (enabledElectrodes & enabledElectrode)
        {
            uint32_t pin = electrode->gpio_input.gpio1 & 0xFFU;                /* Select pin  */
            uint32_t port = electrode->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
            set_pin_tsi_mode(port, pin);   /* Set pinmux to TSI peripheral mode */
        }
    } 
    
    return NT_SUCCESS;
}

void delay(void)
{
   volatile int j=0;
   volatile int k=0;
   
   for (j=0; j<100; j++)
      for (k=0; k<2000; k++)
      {
        CUSTOM_DELAY();
      }
}
#endif /* NT_SAFETY_SUPPORT */
