/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_electrodes.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_control_arotary.h"
#include "nt_control_arotary_prv.h"
#include "nt_keydetector_safa.h"

/* Invoke callback based on the even has occurred */
static void _nt_control_arotary_invoke_callback(const struct nt_control_data *control,
                                                enum nt_control_arotary_event event,
                                                uint32_t position);
/* Find maximum delta electrode */
static int32_t _nt_control_arotary_get_base_data(struct nt_control_data *control,
                                                 struct nt_control_arotary_temp_data *temp_data);
/* Calculate position */
static uint32_t _nt_control_arotary_calculate_position(const struct nt_control_data *control,
                                                       struct nt_control_arotary_temp_data *temp_data);

/* Provide event when all electrodes are released. */
static void __nt_control_arotary_process_all_released(struct nt_control_data *control);
/* The Analog Rotary initialization function. */
static int32_t _nt_control_arotary_init(struct nt_control_data *control);
/* The Analog Rotary process function. */
static int32_t _nt_control_arotary_process(struct nt_control_data *control);

/* The Analog Rotary control interface. */
const struct nt_control_interface nt_control_arotary_interface = {
    .init        = _nt_control_arotary_init,
    .process     = _nt_control_arotary_process,
    .name        = NT_CONTROL_AROTARY_NAME,
    .params_size = sizeof(struct nt_control_arotary),
};

void nt_control_arotary_register_callback(const struct nt_control *control, nt_control_arotary_callback callback)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_arotary_data *ram = control_data->data.arotary;
    NT_ASSERT(ram != NULL);

    ram->callback = callback;
}

uint32_t nt_control_arotary_get_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_arotary_data *ram = control_data->data.arotary;
    NT_ASSERT(ram != NULL);

    return (uint32_t)ram->position;
}

uint32_t nt_control_arotary_is_touched(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_AROTARY_TOUCH_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_arotary_movement_detected(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_AROTARY_MOVEMENT_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_arotary_get_direction(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_AROTARY_DIRECTION_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_arotary_get_invalid_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_arotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_AROTARY_INVALID_POSITION_FLAG);
    return (bool)flag ? 1U : 0U;
}

static void _nt_control_arotary_invoke_callback(const struct nt_control_data *control,
                                                enum nt_control_arotary_event event,
                                                uint32_t position)
{
    struct nt_control_arotary_data *ram = control->data.arotary;
    NT_ASSERT(ram != NULL);

    if (ram->callback != NULL)
    {
        ram->callback(control->rom, event, position);
    }
}

static int32_t _nt_control_arotary_get_base_data(struct nt_control_data *control,
                                                 struct nt_control_arotary_temp_data *temp_data)
{
    uint32_t elec_counter = control->electrodes_size;
    int32_t delta;
    int32_t delta1 = 0, delta2 = 0;
    uint32_t max_first = 0, max_second = 0;

    /* finding the maximum 2 deltas in the control */
    while ((bool)(elec_counter--))
    {
        delta = _nt_electrode_get_delta(control->electrodes[elec_counter]);

        if (delta < 0)
        {
            return (int32_t)NT_INVALID_RESULT;
        }

        if (delta > delta1)
        {
            max_second = max_first;
            max_first  = elec_counter;
            delta2     = delta1;
            delta1     = delta;
        }
        else if (delta > delta2)
        {
            max_second = elec_counter;
            delta2     = delta;
        }
        else
        { /* no command to avoid Misra issue */
        }
    }

    if ((bool)_nt_control_check_neighbours_electrodes(control, max_first, max_second, 1) != (bool)NT_SUCCESS)
    {
        _nt_control_set_flag(control, (int32_t)NT_AROTARY_INVALID_POSITION_FLAG);
        return (int32_t)NT_INVALID_RESULT;
    }

    if (((bool)_nt_control_check_edge_electrodes(control, max_first) == (bool)NT_SUCCESS) &&
        ((bool)_nt_control_check_edge_electrodes(control, max_second) == (bool)NT_SUCCESS))
    {
        temp_data->active_el_ix = control->electrodes_size - 1UL;
        if (max_first == 0U)
        {
            temp_data->first_delta = (uint32_t)delta1;
        }
        else
        {
            temp_data->first_delta = (uint32_t)delta2;
        }
    }
    else if (max_first < max_second)
    {
        temp_data->active_el_ix = max_first;
        temp_data->first_delta  = (uint32_t)delta2;
    }
    else
    {
        temp_data->active_el_ix = max_second;
        temp_data->first_delta  = (uint32_t)delta1;
    }

    /* This constant ensure the rotary linearization and has been obtain from experiences and measurement*/
    temp_data->range = ((uint32_t)delta1 + (uint32_t)delta2) * 7UL / 12UL;

    _nt_control_clear_flag(control, (int32_t)NT_AROTARY_INVALID_POSITION_FLAG);

    return (int32_t)NT_SUCCESS;
}

static uint32_t _nt_control_arotary_calculate_position(const struct nt_control_data *control,
                                                       struct nt_control_arotary_temp_data *temp_data)
{
    NT_ASSERT(control->rom->interface == &nt_control_arotary_interface);

    uint32_t temp_range = 0xffffU / (uint32_t)control->electrodes_size;

    uint32_t real_position = temp_range * temp_data->active_el_ix;

    const struct nt_control_arotary *arotary_desc = control->rom->control_params.arotary;

    real_position += (temp_range * temp_data->first_delta) / temp_data->range;
    /* This constant ensure the rotary linearization and has been obtain from experiences and measurement*/
    if (real_position >= (uint32_t)6553)
    {
        real_position -= (uint32_t)6553;
    }
    else
    {
        real_position = (uint32_t)0;
    }
    return ((real_position * arotary_desc->range) / 0xffffU);
}

static void __nt_control_arotary_process_all_released(struct nt_control_data *control)
{
    /* all released */
    if ((bool)_nt_control_get_flag(control, (int32_t)NT_AROTARY_TOUCH_FLAG))
    {
        struct nt_control_arotary_data *ram = control->data.arotary;
        NT_ASSERT(ram != NULL);
        /* if none is touched & touch was reported, all released event */
        _nt_control_clear_flag(control, (uint32_t)NT_AROTARY_TOUCH_FLAG | (uint32_t)NT_AROTARY_INVALID_POSITION_FLAG |
                                            (uint32_t)NT_AROTARY_MOVEMENT_FLAG);
        _nt_control_arotary_invoke_callback(control, NT_AROTARY_ALL_RELEASE, (uint32_t)ram->position);
        _nt_control_clear_flag_all_elec(control, (int32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG);
    }
    _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
}

static int32_t _nt_control_arotary_init(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_arotary_interface);
    NT_ASSERT(control->rom->control_params.arotary != NULL);

#if (NT_DEBUG == 0)
    uint32_t elec_cnt = control->electrodes_size;
    while ((bool)(elec_cnt--))
    {
        NT_ASSERT(control->electrodes[elec_cnt]->rom->keydetector_interface == &nt_keydetector_safa_interface);
    }
#endif

    const struct nt_control_arotary *arotary = control->rom->control_params.arotary;

    if (arotary->range == 0U)
    {
        return (int32_t)NT_FAILURE;
    }

    control->data.arotary = _nt_mem_alloc(sizeof(struct nt_control_arotary_data));

    if (control->data.arotary == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if ((bool)_nt_control_check_data(control) != (bool)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_control_arotary_process(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_arotary_interface);

    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_EN_FLAG) ||
        !(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG))
    {
        return (int32_t)NT_FAILURE; /* control disabled or data not ready */
    }

    if (!(bool)_nt_control_get_electrodes_state(control))
    {
        __nt_control_arotary_process_all_released(control);
        return (int32_t)NT_SUCCESS; /* no touch on the control's electrodes */
    }

    if ((bool)_nt_control_get_electrodes_digital_state(control))
    {
        return (int32_t)NT_SUCCESS; /* no touch on the control's electrodes */
    }

    struct nt_control_arotary_data *ram      = control->data.arotary;
    const struct nt_control_arotary *arotary = control->rom->control_params.arotary;

    struct nt_control_arotary_temp_data computing_data;

    /* at least one is touched, validate position */
    if ((bool)_nt_control_arotary_get_base_data(control, &computing_data) == (bool)NT_SUCCESS)
    {
        uint32_t current_position = _nt_control_arotary_calculate_position(control, &computing_data);

        uint8_t previous_pos = ram->position;

        uint8_t subrange = (arotary->range - 1U) / control->electrodes_size;
        if (previous_pos != current_position)
        {
            /* handle edge cases on rotary */
            if ((previous_pos > ((arotary->range - 1U) - subrange)) && (previous_pos < subrange))
            {
                _nt_control_set_flag(control, (int32_t)NT_AROTARY_DIRECTION_FLAG);
            }
            else if (((uint8_t)current_position > ((arotary->range - 1U) - subrange)) &&
                     ((uint8_t)current_position < subrange))
            {
                _nt_control_clear_flag(control, (int32_t)NT_AROTARY_DIRECTION_FLAG);
            }
            else
            {
                /* no edge case here */
                if (previous_pos < current_position)
                {
                    _nt_control_set_flag(control, (int32_t)NT_AROTARY_DIRECTION_FLAG);
                }
                else
                {
                    _nt_control_clear_flag(control, (int32_t)NT_AROTARY_DIRECTION_FLAG);
                }
            }
            ram->position = (uint8_t)current_position;

            _nt_control_set_flag(control, (int32_t)NT_AROTARY_MOVEMENT_FLAG);
            _nt_control_arotary_invoke_callback(control, NT_AROTARY_MOVEMENT, (uint32_t)ram->position);
        }
        else
        {
            _nt_control_clear_flag(control, (int32_t)NT_AROTARY_MOVEMENT_FLAG);
        }
    }
    /* end new code*/
    /* if arotary is touched for the first time */
    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_AROTARY_TOUCH_FLAG))
    {
        _nt_control_arotary_invoke_callback(control, NT_AROTARY_INITIAL_TOUCH, (uint32_t)ram->position);
    }
    /* lock baseline for all electrode in the control */
    _nt_control_set_flag_all_elec(control, (int32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG);
    _nt_control_set_flag(control, (int32_t)NT_AROTARY_TOUCH_FLAG);
    _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
    return (int32_t)NT_SUCCESS;
}
