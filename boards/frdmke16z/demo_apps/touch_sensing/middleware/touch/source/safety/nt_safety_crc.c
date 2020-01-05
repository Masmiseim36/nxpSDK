/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_system.h"

#include "nt_control_arotary.h"
#include "nt_control_aslider.h"
#include "nt_control_keypad.h"
#include "nt_control_proxi.h"

#include "nt_module_tsi.h"
#include "nt_module_gpio.h"
#include "nt_module_gpioint.h"

#include "nt_modules.h"
#include "../source/modules/nt_modules_prv.h"
#include "../source/safety/nt_safety_prv.h"
#include "../source/safety/nt_safety_gpio.h"
#include "nt_safety.h"

#include "string.h"
#include "fsl_crc.h"

static uint16_t nt_safety_crc16(const uint8_t *data_p, uint8_t length, uint16_t seed, uint32_t hw_acc);
static uint16_t nt_safety_crc16_calc_sw(const uint8_t *data_p, uint8_t length, uint16_t seed);
static uint16_t nt_safety_crc16_calc_hw(const uint8_t *data_p, uint8_t length, uint16_t seed);

static void init_crc16_hw_ccit(uint16_t seed);


static uint16_t nt_safety_crc16_calc_hw(const uint8_t *data_p, uint8_t length, uint16_t seed)
{
  uint16_t crc = seed;
  CRC_Type *base = CRC0;

  CRC_WriteData(base, data_p, length); /* write new data every time */
  crc = CRC_Get16bitResult(base);
  return crc;
}

static uint16_t nt_safety_crc16_calc_sw(const uint8_t *data_p, uint8_t length, uint16_t seed)
{
    uint8_t x=0;
    uint16_t crc = seed;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
    }
    crc &= 0xFFFF;
    return crc;
}

/* Initialization the CRC HW module, polynomial = 0x1021, seed = 0xFFFF */
static void init_crc16_hw_ccit(uint16_t seed)
{
    crc_config_t config;
    CRC_Type *base = CRC0;
    CRC_GetDefaultConfig(&config);
    config.seed = seed;
    CRC_Init(base, &config);
}

static uint16_t nt_safety_crc16(const uint8_t *data_p, uint8_t length, uint16_t seed, uint32_t hw_acc)
{
  uint16_t crc;

  if(hw_acc)
  {
    crc = nt_safety_crc16_calc_hw(data_p, length, seed); /* perform CRC HW data calculation */
  }

  else
  {
    crc = nt_safety_crc16_calc_sw(data_p, length, seed); /* perform CRC SW data calculation */
  }

  return crc;
}

int32_t nt_safety_calc_cfg_crc(const struct nt_system *system, uint16_t *crc_val)
{
  NT_ASSERT(system != NULL);

  struct nt_control * const * loc_controls;
  struct nt_module * const * loc_modules;
  struct nt_electrode * const * loc_electrodes;

  uint8_t *pdata;
  uint16_t crc = 0xffff;
  uint32_t hw = 0;

  if (_nt_system_check_data(system) < NT_SUCCESS) {
      return NT_FAILURE;
  }

  if(system->safety_crc_hw)
  {
    init_crc16_hw_ccit(crc); /* initialization and run HW CRC, wait for new data */
    hw = 1;
  }

  pdata = (uint8_t *) system;
  /* calculates CRC of nt_system struct */
  crc = nt_safety_crc16(pdata , sizeof(struct nt_system) , crc, hw);
  loc_controls = (struct nt_control * const *) system->controls;

  pdata = (uint8_t *) loc_controls; /* pass the all controls list pointers to calculate CRC */
  crc = nt_safety_crc16(pdata , sizeof(struct nt_control *) * _nt_system_count_pointer_array((const void**)loc_controls), crc, hw);

  /* do with every control */
  while(loc_controls && *loc_controls)
  {
    struct nt_control *ctrl = *loc_controls;
    pdata = (uint8_t *) ctrl; /* calculate CRC of individual control structures */
    crc = nt_safety_crc16(pdata , sizeof(struct nt_control) , crc, hw);

    /*  check interface.params_size */
    if(ctrl->interface->params_size != 0)
    {
      pdata = (uint8_t *) ctrl->control_params.general;
      crc = nt_safety_crc16(pdata , ctrl->interface->params_size, crc, hw);

      /*  check control name, calculate CRC if Keypad groups defined */
      if(!strcmp(ctrl->interface->name, NT_CONTROL_KEYPAD_NAME))
      {
        if(ctrl->control_params.keypad->groups != NULL)
        {
          pdata = (uint8_t *) ctrl->control_params.keypad->groups;
          crc = nt_safety_crc16(pdata , (ctrl->control_params.keypad->groups_size) * sizeof(*ctrl->control_params.keypad->groups) , crc, hw);
        }
      }
    }
    /* find electrode pointers assigned to control */
    loc_electrodes = (struct nt_electrode * const *) ctrl->electrodes;
    pdata = (uint8_t *) loc_electrodes; /* calculate CRC of electrode pointers */
    crc = nt_safety_crc16(pdata , sizeof(struct nt_electrode *) * nt_control_count_electrodes(ctrl), crc, hw);

    while(loc_electrodes && *loc_electrodes)
    {
      struct nt_electrode *elec = *loc_electrodes;
      pdata = (uint8_t *) elec; /* calculate CRC of electrode structures */
      crc = nt_safety_crc16(pdata , sizeof(struct nt_electrode), crc, hw);

      loc_electrodes++;
    }
    loc_controls++;

  }/* do with every control */

  /* do with modules */
  loc_modules = (struct nt_module * const *) system->modules;
  pdata = (uint8_t *) loc_modules; /* calculate CRC of list of pointers */
  crc = nt_safety_crc16(pdata , sizeof(struct nt_module *) * _nt_system_count_pointer_array((const void**)loc_modules), crc, hw);

  while(loc_modules && *loc_modules)
  {
    struct nt_module *module = *loc_modules;
    pdata = (uint8_t *) module; /* calculate CRC of individual module structures */
    crc = nt_safety_crc16(pdata , sizeof(struct nt_module), crc, hw);

    /* Do with module params if exist */
    if(module->interface->params_size != 0)
    {
      pdata = (uint8_t *) module->module_params.general;
      crc = nt_safety_crc16(pdata , module->interface->params_size, crc, hw);

      if(module->interface->params_size == sizeof(struct nt_module_tsi_params))
      {
        /*  Do with TSI module params if exist */
        if(!strcmp(module->interface->name, NT_MODULE_TSI_NAME))
        {
          if(module->module_params.tsi != NULL)
          {
            if(module->module_params.tsi->config != NULL) /* if noise_config defined */
            {
              pdata = (uint8_t *) module->module_params.tsi->config;
              crc = nt_safety_crc16(pdata , sizeof(tsi_config_t), crc, hw); /* calculate CRC for key groups array */
            }
          }
        }
      }

      /* Do with: GPIO module params */
      if(module->interface->params_size == sizeof(struct nt_module_gpio_params))
      {
        /* Do with TSI module params if exist */
        if(!strcmp(module->interface->name, NT_MODULE_GPIO_NAME))
        {
          if(module->module_params.gpio != NULL)
          {
            if(module->module_params.gpio->user_interface != NULL) /* if Gpio user_interface defined */
            {
              pdata = (uint8_t *) module->module_params.gpio->user_interface;
              crc = nt_safety_crc16(pdata , sizeof(struct nt_module_gpio_user_interface), crc, hw); /* calculate CRC for gpio user_interface */
            }
          }
        }
      }

      /*  Do with GPIOINT module params*/
      if(module->interface->params_size == sizeof(struct nt_module_gpioint_params))
      {
        /* Do with TSI module params if exist */
        if(!strcmp(module->interface->name, NT_MODULE_GPIOINT_NAME))
        {
          if(module->module_params.gpioint != NULL)
          {
            if(module->module_params.gpioint->user_interface != NULL) /* if gpioint user_interface defined */
            {
              pdata = (uint8_t *) module->module_params.gpioint->user_interface;  /* TO DO check if OK */
              crc = nt_safety_crc16(pdata , sizeof(struct nt_module_gpioint_user_interface), crc, hw); /* calculate CRC for gpioint user_interface */
            }
          }
        }
      }
    }/* If module params exist */

    /* Do with safety params(GPIO params only) */
    if(module->safety_interface->params_size != 0)
    {
      if(module->safety_interface->params_size == sizeof(struct nt_module_safety_gpio_params))
      {
        /* Do with Safety params and user defined GPIO function interface */
        if(0 == strcmp(module->safety_interface->name, NT_MODULE_SAFETY_NAME))
        {
          if(module->safety_params.gpio != NULL)
          {
            if(module->safety_params.gpio->user_interface != NULL) /* if gpioint user_interface defined */
            {
              pdata = (uint8_t *) module->safety_params.gpio->user_interface;
              crc = nt_safety_crc16(pdata , sizeof(struct nt_module_gpio_user_interface), crc, hw); /* calculate CRC for gpioint user_interface */
            }
          }
        }
      }
    }

    /*  Do with TSI hw_config */
    if(module->config !=NULL)
    {
      pdata = (uint8_t *) module->config;
      crc = nt_safety_crc16(pdata , sizeof(tsi_config_t) , crc, hw); /*  calculate CRC for gpioint user_interface */
    }

    /* Do with electrodes */
    loc_electrodes = (struct nt_electrode * const *) (*loc_modules)->electrodes;
    pdata = (uint8_t *) loc_electrodes; /* electrode list pointers to calculate CRC */
    crc = nt_safety_crc16(pdata , sizeof(struct nt_electrode *) * _nt_module_count_electrodes(module), crc, hw);

    while(loc_electrodes && *loc_electrodes) /* if electrode pointers exists */
    {
      struct nt_electrode *elec = *loc_electrodes;
      pdata = (uint8_t *) elec; /* calculate CRC of individual control structures */
      crc = nt_safety_crc16(pdata , sizeof(struct nt_electrode), crc, hw); /* calculate CRC of electrode configuration struct data */

      /* Do with Keydetector params */
      if(elec->keydetector_interface->params_size != 0)
      {
        pdata = (uint8_t *) elec->keydetector_params.general;
        crc = nt_safety_crc16(pdata , elec->keydetector_interface->params_size, crc, hw);
      }

      loc_electrodes++;
    } /* do with every electrode */

    loc_modules++;

  }/* do with every module (mostly just one defined) */

  /* return CRC inside parameter */
  *crc_val = crc;

  return NT_SUCCESS;
}

/* calculates CRC on the system data */
int32_t nt_safety_check_cfg_data(const struct nt_system *system, uint16_t crc_val)
{
  uint16_t crc_calc;

  nt_safety_calc_cfg_crc(system, &crc_calc);
  if(crc_calc != crc_val)
  {
    return NT_FAILURE;
  }

  return NT_SUCCESS;
}

