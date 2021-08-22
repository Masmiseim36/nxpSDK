/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_control_rotary.h"
#include "nt_control_rotary_prv.h"

/* Invokes a callback depending on the event */
static void _nt_control_rotary_invoke_callback(struct nt_control_data *control,
                                               enum nt_control_rotary_event event,
                                               uint32_t position);
/* Set dynamic and movement flags */
static void _nt_control_rotary_calc_dynamic(struct nt_control_data *control, uint32_t position);
/* Calculates position */
static void _nt_control_rotary_calc_position(struct nt_control_data *control,
                                             uint32_t first_elec,
                                             uint32_t last_elec,
                                             uint32_t touch_count);
/* Set proper direction for rotary */
static void _nt_control_rotary_adjust_direction(struct nt_control_data *control, uint32_t position);
/* The Rotary control initialization function. */
static int32_t _nt_control_rotary_init(struct nt_control_data *control);
/* The Rotary control process function. */
static int32_t _nt_control_rotary_process(struct nt_control_data *control);

/* The Rotary control interface. */
const struct nt_control_interface nt_control_rotary_interface = {
    .init        = _nt_control_rotary_init,
    .process     = _nt_control_rotary_process,
    .name        = NT_CONTROL_ROTARY_NAME,
    .params_size = 0,
};

void nt_control_rotary_register_callback(const struct nt_control *control, nt_control_rotary_callback callback)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_rotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_rotary_data *ram = control_data->data.rotary;
    NT_ASSERT(ram != NULL);

    ram->callback = callback;
}

uint32_t nt_control_rotary_get_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_rotary_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_rotary_data *ram = control_data->data.rotary;
    NT_ASSERT(ram != NULL);

    return (uint32_t)ram->position;
}

uint32_t nt_control_rotary_is_touched(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    uint32_t flag = _nt_control_get_flag(_nt_control_get_data(control), (int32_t)NT_ROTARY_TOUCH_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_rotary_movement_detected(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    uint32_t flag = _nt_control_get_flag(_nt_control_get_data(control), (int32_t)NT_ROTARY_MOVEMENT_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_rotary_get_direction(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    uint32_t flag = _nt_control_get_flag(_nt_control_get_data(control), (int32_t)NT_ROTARY_DIRECTION_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_rotary_get_invalid_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    uint32_t flag = _nt_control_get_flag(_nt_control_get_data(control), (int32_t)NT_ROTARY_INVALID_POSITION_FLAG);
    return (bool)flag ? 1U : 0U;
}

static void _nt_control_rotary_invoke_callback(struct nt_control_data *control,
                                               enum nt_control_rotary_event event,
                                               uint32_t position)
{
    struct nt_control_rotary_data *ram = control->data.rotary;
    NT_ASSERT(ram != NULL);

    if (ram->callback != NULL)
    {
        ram->callback(control->rom, event, position);
    }
}

static void _nt_control_rotary_calc_dynamic(struct nt_control_data *control, uint32_t position)
{
    struct nt_control_rotary_data *ram = control->data.rotary;
    NT_ASSERT(ram != NULL);

    if (position != ram->position)
    {
        /* movement detected */
        if (position > ram->position)
        {
            _nt_control_set_flag(control, (int32_t)NT_ROTARY_DIRECTION_FLAG);
        }
        else
        {
            _nt_control_clear_flag(control, (int32_t)NT_ROTARY_DIRECTION_FLAG);
        }
        _nt_control_set_flag(control, (int32_t)NT_ROTARY_MOVEMENT_FLAG);
    }
    else
    {
        _nt_control_clear_flag(control, (int32_t)NT_ROTARY_MOVEMENT_FLAG);
    }
}

static void _nt_control_rotary_adjust_direction(struct nt_control_data *control, uint32_t position)
{
    struct nt_control_rotary_data *ram = control->data.rotary;
    NT_ASSERT(ram != NULL);

    uint32_t prev_pos  = ram->position;
    uint32_t elec_size = control->electrodes_size;

    if (((prev_pos >= elec_size) && (position == 0U)) || ((prev_pos == 0U) && (position >= elec_size)))
    {
        /* negate the direction flag if the rotary was crossed through 0 */
        if ((bool)_nt_control_get_flag(control, (int32_t)NT_ROTARY_DIRECTION_FLAG))
        {
            _nt_control_clear_flag(control, (int32_t)NT_ROTARY_DIRECTION_FLAG);
        }
        else
        {
            _nt_control_set_flag(control, (int32_t)NT_ROTARY_DIRECTION_FLAG);
        }
    }
}

static void _nt_control_rotary_calc_position(struct nt_control_data *control,
                                             uint32_t first_elec,
                                             uint32_t last_elec,
                                             uint32_t touch_count)
{
    struct nt_control_rotary_data *ram = control->data.rotary;
    NT_ASSERT(ram != NULL);

    if ((last_elec - first_elec) == touch_count)
    {
        /* valid position (not on the edges) */
        _nt_control_clear_flag(control, (int32_t)NT_ROTARY_INVALID_POSITION_FLAG);
        uint32_t position = ((first_elec << 1U) + touch_count) - 1U;
        _nt_control_rotary_calc_dynamic(control, position);
        ram->position = (uint8_t)position;
    }
    else
    {
        /* first position or the last one */
        if ((ram->position == 0U) || (ram->position == ((control->electrodes_size - 1U) << 1U)))
        {
            /* first or the last touched */
            if ((first_elec == 0U) && (last_elec == control->electrodes_size))
            {
                uint32_t position = (uint8_t)((control->electrodes_size << 1U) - 1U);
                _nt_control_clear_flag(control, (int32_t)NT_ROTARY_INVALID_POSITION_FLAG);
                _nt_control_rotary_calc_dynamic(control, position);
                _nt_control_rotary_adjust_direction(control, position);
                ram->position = (uint8_t)position;
            }
            else
            {
                _nt_control_set_flag(control, (int32_t)NT_ROTARY_INVALID_POSITION_FLAG);
            }
        }
    }
    /* processed all use cases, invoke movement callback if movement is set */
    if ((bool)_nt_control_get_flag(control, (int32_t)NT_ROTARY_MOVEMENT_FLAG))
    {
        _nt_control_rotary_invoke_callback(control, NT_ROTARY_MOVEMENT, ram->position);
    }
}

static int32_t _nt_control_rotary_init(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_rotary_interface);

    control->data.rotary = _nt_mem_alloc(sizeof(struct nt_control_rotary_data));

    if (control->data.rotary == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if ((bool)_nt_control_check_data(control) != (bool)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_control_rotary_process(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_rotary_interface);

    struct nt_control_rotary_data *ram = control->data.rotary;
    NT_ASSERT(ram != NULL);

    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_EN_FLAG) ||
        !(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG))
    {
        return (int32_t)NT_FAILURE; /* control disabled or data not ready */
    }

    uint64_t elec_state = _nt_control_get_electrodes_state(control);
    if (!(bool)elec_state)
    {
        /* all released */
        if ((bool)_nt_control_get_flag(control, (int32_t)NT_ROTARY_TOUCH_FLAG))
        {
            /* if none is touched & touch was reported, all released event */
            _nt_control_clear_flag(control, (uint32_t)NT_ROTARY_TOUCH_FLAG | (uint32_t)NT_ROTARY_MOVEMENT_FLAG |
                                                (uint32_t)NT_ROTARY_INVALID_POSITION_FLAG);
            _nt_control_rotary_invoke_callback(control, NT_ROTARY_ALL_RELEASE, (uint32_t)ram->position);
        }
        _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
        return (int32_t)NT_SUCCESS;                                         /* no touch on the control's electrodes */
    }

    uint32_t touch_count = _nt_control_get_touch_count(elec_state);
    if (touch_count <= 2U)
    {
        uint32_t last_elec  = _nt_control_get_last_elec_touched(elec_state);
        uint32_t first_elec = _nt_control_get_first_elec_touched(elec_state);

        _nt_control_rotary_calc_position(control, first_elec, last_elec, touch_count);

        if (!(bool)_nt_control_get_flag(control, (int32_t)NT_ROTARY_TOUCH_FLAG))
        {
            _nt_control_rotary_invoke_callback(control, NT_ROTARY_INITIAL_TOUCH, (uint32_t)ram->position);
        }
        _nt_control_set_flag(control, (int32_t)NT_ROTARY_TOUCH_FLAG);

        _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
    }
    else
    {
        _nt_control_set_flag(control, (int32_t)NT_ROTARY_INVALID_POSITION_FLAG);
    }
    return (int32_t)NT_SUCCESS;
}
