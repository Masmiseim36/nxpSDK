/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nt_modules.h"
#include "nt_module_gpioint.h"
#include "../source/system/nt_system_prv.h"
#include "../source/modules/nt_module_gpioint_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "../source/filters/nt_filters_prv.h"
#include "../source/modules/nt_modules_prv.h"

/* Local functions */
static int32_t nt_module_gpioint_check_interface(const struct nt_module_data *module);
static int32_t nt_module_gpioint_data_check(const struct nt_module_data *module);

static int32_t _nt_module_gpioint_init(struct nt_module_data *module);
static int32_t _nt_module_gpioint_process(struct nt_module_data *module);
static int32_t _nt_module_gpioint_trigger(struct nt_module_data *module);
static int32_t _nt_module_gpioint_electrode_enable(struct nt_module_data *module, uint32_t elec_index);
static int32_t _nt_module_gpioint_electrode_disable(struct nt_module_data *module, uint32_t elec_index);
static int32_t _nt_module_gpioint_change_mode(struct nt_module_data *module,
                                              const enum nt_module_mode mode,
                                              const struct nt_electrode *electrode);

static void nt_module_gpioint_start_channel(struct nt_module_data *module,
                                            struct nt_electrode_data *electrode,
                                            uint32_t elec_index);
static void nt_module_gpioint_stop_channel(struct nt_module_data *module, struct nt_electrode_data *electrode);

/** interface gpio module */
const struct nt_module_interface nt_module_gpioint_interface = {
    .init              = _nt_module_gpioint_init,
    .trigger           = _nt_module_gpioint_trigger,
    .process           = _nt_module_gpioint_process,
    .recalibrate       = NULL,
    .electrode_enable  = _nt_module_gpioint_electrode_enable,
    .electrode_disable = _nt_module_gpioint_electrode_disable,
    .change_mode       = _nt_module_gpioint_change_mode,
    .name              = NT_MODULE_GPIOINT_NAME,
    .params_size       = sizeof(struct nt_module_gpioint_params),
};

/*******************************************************************************
 *                       GPIO MODULE functions
 *******************************************************************************/
static int32_t _nt_module_gpioint_init(struct nt_module_data *module)
{
    module->special_data.gpioint = _nt_mem_alloc(sizeof(struct nt_module_gpioint_data));

    if (module->special_data.gpioint == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if (nt_module_gpioint_data_check(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    const struct nt_module_gpioint_user_interface *interface = module->rom->module_params.gpioint->user_interface;

    interface->init_timer_ptr();

    uint32_t electrode_counter = module->electrodes_cnt;

    while ((bool)(electrode_counter--))
    {
        /* Get the pointer to electrode. */
        const struct nt_electrode *elec = module->electrodes[electrode_counter]->rom;

        /* default state for all pins. */
        interface->init_pin_ptr(elec->pin_input >> NT_GPIO_PORT_SHIFT, elec->pin_input & 0xFFU);
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpioint_trigger(struct nt_module_data *module)
{
    if (module->special_data.gpioint->scan_status == (uint8_t)NT_GPIO_IN_PROGRESS)
    {
        return (int32_t)NT_SCAN_IN_PROGRESS;
    }
    else
    {
        struct nt_electrode_data *elec = module->electrodes[0];
        nt_module_gpioint_start_channel(module, elec, 0U);
        module->special_data.gpioint->scan_status = (uint8_t)NT_GPIO_IN_PROGRESS;
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpioint_process(struct nt_module_data *module)
{
    uint32_t electrode_counter = module->electrodes_cnt;

    while ((bool)(electrode_counter--))
    {
        struct nt_electrode_data *elec = module->electrodes[electrode_counter];
        elec->rom->keydetector_interface->nt_keydetector_measure(elec, _nt_electrode_get_raw_signal(elec));
        elec->rom->keydetector_interface->nt_keydetector_process(elec);
    }

    _nt_module_clear_flag(module, (uint32_t)NT_MODULE_NEW_DATA_FLAG);
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpioint_electrode_enable(struct nt_module_data *module, uint32_t elec_index)
{
    if (elec_index < module->electrodes_cnt)
    {
        module->special_data.gpioint->pen |= (uint32_t)1 << elec_index;
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpioint_electrode_disable(struct nt_module_data *module, uint32_t elec_index)
{
    if (elec_index < module->electrodes_cnt)
    {
        module->special_data.gpioint->pen &= ~((uint32_t)1 << elec_index);
    }
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_module_gpioint_change_mode(struct nt_module_data *module,
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

static int32_t nt_module_gpioint_check_interface(const struct nt_module_data *module)
{
    const struct nt_module_gpioint_user_interface *interface = module->rom->module_params.gpioint->user_interface;
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
    else if (interface->init_pin_ptr == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    else
    {
        return (int32_t)NT_SUCCESS;
    }
}

static int32_t nt_module_gpioint_data_check(const struct nt_module_data *module)
{
    if (module->rom->module_params.gpioint->user_interface == NULL)
    {
        return (int32_t)NT_FAILURE;
    }
    if (nt_module_gpioint_check_interface(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }
    return (int32_t)NT_SUCCESS;
}

static void nt_module_gpioint_start_channel(struct nt_module_data *module,
                                            struct nt_electrode_data *electrode,
                                            uint32_t elec_index)
{
    uint32_t port = electrode->rom->pin_input >> NT_GPIO_PORT_SHIFT;
    uint32_t pin  = electrode->rom->pin_input & 0xFFU;

    const struct nt_module_gpioint_user_interface *interface = module->rom->module_params.gpioint->user_interface;

    module->special_data.gpioint->measured_pin = (uint8_t)elec_index;
    interface->set_pin_output_ptr(port, pin);
    interface->set_pin_low_ptr(port, pin);
    interface->clear_pin_interrupt_ptr(port, pin);
    interface->set_pin_interrupt_ptr(port, pin);
    interface->timer_reset_counter_ptr();
    interface->start_timer_ptr();
    interface->set_pin_input_ptr(port, pin);
}

static void nt_module_gpioint_stop_channel(struct nt_module_data *module, struct nt_electrode_data *electrode)
{
    uint32_t port = electrode->rom->pin_input >> NT_GPIO_PORT_SHIFT;
    uint32_t pin  = electrode->rom->pin_input & 0xFFU;

    const struct nt_module_gpioint_user_interface *interface = module->rom->module_params.gpioint->user_interface;
    interface->clear_pin_interrupt_ptr(port, pin);
    interface->stop_timer_ptr();
    interface->set_pin_high_ptr(port, pin);
    interface->set_pin_output_ptr(port, pin);
}

void nt_module_gpioint_isr(const struct nt_module *module)
{
    NT_ASSERT(module != NULL);

    struct nt_module_data *module_data = _nt_module_get_data(module);
    NT_ASSERT(module_data != NULL);

    uint32_t measured_pin                                    = module_data->special_data.gpioint->measured_pin;
    struct nt_electrode_data *elec                           = module_data->electrodes[measured_pin];
    const struct nt_module_gpioint_user_interface *interface = module->module_params.gpioint->user_interface;

    uint32_t signal = interface->timer_get_counter_ptr();
    _nt_electrode_set_raw_signal(elec, signal);

    nt_module_gpioint_stop_channel(module_data, elec);

    if (measured_pin < (uint32_t)module_data->electrodes_cnt - 1UL)
    {
        measured_pin++;
        elec = module_data->electrodes[measured_pin];
        nt_module_gpioint_start_channel(module_data, elec, measured_pin);
    }
    else
    {
        /* all pins measured */
        _nt_module_set_flag(module_data, (uint32_t)NT_MODULE_NEW_DATA_FLAG);
        _nt_system_modules_data_ready(module_data);
        module_data->special_data.gpioint->scan_status = (uint8_t)NT_GPIO_MEASURED;
    }
}

void nt_module_gpioint_overflow_isr(const struct nt_module *module)
{
    NT_ASSERT(module != NULL);

    struct nt_module_data *module_data = _nt_module_get_data(module);
    NT_ASSERT(module_data != NULL);

    uint32_t measured_pin          = module_data->special_data.gpioint->measured_pin;
    struct nt_electrode_data *elec = module_data->electrodes[measured_pin];

    nt_module_gpioint_stop_channel(module_data, elec);

    if (measured_pin < ((uint32_t)module_data->electrodes_cnt - 1UL))
    {
        measured_pin++;
        elec = module_data->electrodes[measured_pin];
        nt_module_gpioint_start_channel(module_data, elec, measured_pin);
    }
    else
    {
        /* all pins measured */
        module_data->special_data.gpioint->scan_status = (uint8_t)NT_GPIO_MEASURED;
    }
}
