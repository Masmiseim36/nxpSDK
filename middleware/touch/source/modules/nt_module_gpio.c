/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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

static int32_t _nt_module_gpio_init(struct nt_module_data *module);
static int32_t _nt_module_gpio_process(struct nt_module_data *module);
static int32_t _nt_module_gpio_trigger(struct nt_module_data *module);
static int32_t _nt_module_gpio_electrode_enable(struct nt_module_data *module, uint32_t elec_index);
static int32_t _nt_module_gpio_electrode_disable(struct nt_module_data *module, uint32_t elec_index);
static int32_t _nt_module_gpio_change_mode(struct nt_module_data *module,
                                           const enum nt_module_mode mode,
                                           const struct nt_electrode *electrode);

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

    if (module->special_data.gpio == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if (nt_gpio_data_check(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    const struct nt_module_gpio_user_interface *interface = module->rom->module_params.gpio->user_interface;

    interface->init_timer_ptr();

    uint32_t electrode_counter = module->electrodes_cnt;

    while ((bool)(electrode_counter--))
    {
        /* Get the pointer to electrode. */
        const struct nt_electrode *elec = module->electrodes[electrode_counter]->rom;

        /* default state for all pins. */
        interface->set_pin_default_state_ptr(elec->pin_input >> NT_GPIO_PORT_SHIFT, elec->pin_input & 0xFFU);
    }

    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpio_trigger(struct nt_module_data *module)
{
    _nt_module_set_flag(module, (uint32_t)NT_MODULE_NEW_DATA_FLAG);
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpio_process(struct nt_module_data *module)
{
    uint32_t electrode_counter                            = module->electrodes_cnt;
    const struct nt_module_gpio_user_interface *interface = module->rom->module_params.gpio->user_interface;

    while ((bool)(electrode_counter--))
    {
        struct nt_electrode_data *elec = module->electrodes[electrode_counter];
        uint32_t port                  = elec->rom->pin_input >> NT_GPIO_PORT_SHIFT;
        uint32_t pin                   = elec->rom->pin_input & 0xFFU;
        uint32_t overrun               = 0;
        uint32_t disabledInterrupts;
        interface->set_pin_output_ptr(port, pin);

        disabledInterrupts = __get_PRIMASK();
        /* all interrupts MUST be disabled to achieve valuable measurement */
        if (!(bool)disabledInterrupts)
        {
            (void)(uint32_t) DisableGlobalIRQ();
        }

        interface->set_pin_low_ptr(port, pin);
        interface->timer_reset_counter_ptr();
        interface->start_timer_ptr();
        interface->set_pin_input_ptr(port, pin);
        while (!(bool)interface->get_pin_value_ptr(port, pin))
        {
            if ((bool)(interface->timer_get_overrun_ptr()))
            {
                overrun = 1;
                break;
            }
        };
        uint32_t signal = interface->timer_get_counter_ptr();

        if (!(bool)disabledInterrupts)
        {
            EnableGlobalIRQ(disabledInterrupts);
        }

        interface->set_pin_default_state_ptr(port, pin);

        if (!(bool)overrun)
        {
            _nt_electrode_set_raw_signal(elec, signal);

            /* call general keydetector */
            elec->rom->keydetector_interface->nt_keydetector_measure(elec, signal);
            elec->rom->keydetector_interface->nt_keydetector_process(elec);
        }
    }

    _nt_module_set_flag(module, (uint32_t)NT_MODULE_NEW_DATA_FLAG);
    _nt_system_modules_data_ready(module);

    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpio_electrode_enable(struct nt_module_data *module, uint32_t elec_index)
{
    if (elec_index < module->electrodes_cnt)
    {
        module->special_data.gpio->pen |= (uint32_t)1 << elec_index;
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpio_electrode_disable(struct nt_module_data *module, uint32_t elec_index)
{
    if (elec_index < module->electrodes_cnt)
    {
        module->special_data.gpio->pen &= ~(uint32_t)((uint32_t)1 << elec_index);
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpio_change_mode(struct nt_module_data *module,
                                           const enum nt_module_mode mode,
                                           const struct nt_electrode *electrode)
{
    NT_ASSERT(module != NULL);
    NT_ASSERT(electrode != NULL);

    if (mode == NT_MODULE_MODE_NORMAL)
    {
        return (int32_t)NT_SUCCESS;
    }
    else
    {
        return (int32_t)NT_NOT_SUPPORTED;
    }
}

static int32_t nt_gpio_check_interface(const struct nt_module_data *module)
{
    const struct nt_module_gpio_user_interface *interface = module->rom->module_params.gpio->user_interface;
    if (interface->set_pin_output_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->set_pin_input_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->set_pin_low_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->set_pin_high_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->init_timer_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->start_timer_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->stop_timer_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->timer_reset_counter_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->timer_get_counter_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->timer_get_overrun_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->set_pin_default_state_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else if (interface->get_pin_value_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else
    {
        return (int32_t)NT_SUCCESS;
    }
}

int32_t nt_gpio_data_check(const struct nt_module_data *module)
{
    if (module->rom->module_params.gpio->user_interface == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    if (nt_gpio_check_interface(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }
    return (int32_t)NT_SUCCESS;
}
