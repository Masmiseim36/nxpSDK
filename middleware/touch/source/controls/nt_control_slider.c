/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "nt_control_slider.h"
#include "nt_control_slider_prv.h"
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"

/* Invokes a callback depending on the event */
static void _nt_control_slider_invoke_callback(const struct nt_control_data *control,
                                               enum nt_control_slider_event event,
                                               uint32_t position);
/* Invokes a callback depending on the event */
static void _nt_control_slider_calc_dynamic(struct nt_control_data *control, uint32_t position);
/* Calculates position */
static void _nt_control_slider_calc_position(struct nt_control_data *control,
                                             uint32_t first_elec,
                                             uint32_t last_elec,
                                             uint32_t touch_count);
/* The Slider control initialization function. */
static int32_t _nt_control_slider_init(struct nt_control_data *control);
/* The Slider control process function. */
static int32_t _nt_control_slider_process(struct nt_control_data *control);

/* The Slider control interface. */
const struct nt_control_interface nt_control_slider_interface = {
    .init        = _nt_control_slider_init,
    .process     = _nt_control_slider_process,
    .name        = NT_CONTROL_SLIDER_NAME,
    .params_size = 0,
};

void nt_control_slider_register_callback(const struct nt_control *control, nt_control_slider_callback callback)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_slider_data *ram = control_data->data.slider;
    NT_ASSERT(ram != NULL);

    ram->callback = callback;
}

uint32_t nt_control_slider_get_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_slider_data *ram = control_data->data.slider;
    NT_ASSERT(ram != NULL);

    return (uint32_t)ram->position;
}

uint32_t nt_control_slider_is_touched(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_SLIDER_TOUCH_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_slider_movement_detected(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_SLIDER_MOVEMENT_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_slider_get_direction(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_SLIDER_DIRECTION_FLAG);
    return (bool)flag ? 1U : 0U;
}

uint32_t nt_control_slider_get_invalid_position(const struct nt_control *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_slider_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    uint32_t flag = _nt_control_get_flag(control_data, (int32_t)NT_SLIDER_INVALID_POSITION_FLAG);
    return (bool)flag ? 1U : 0U;
}

static void _nt_control_slider_invoke_callback(const struct nt_control_data *control,
                                               enum nt_control_slider_event event,
                                               uint32_t position)
{
    struct nt_control_slider_data *ram = control->data.slider;
    NT_ASSERT(ram != NULL);

    if (ram->callback != NULL)
    {
        ram->callback(control->rom, event, position);
    }
}

static void _nt_control_slider_calc_dynamic(struct nt_control_data *control, uint32_t position)
{
    struct nt_control_slider_data *ram = control->data.slider;
    NT_ASSERT(ram != NULL);

    if (position != ram->position)
    {
        if (position > ram->position)
        {
            _nt_control_set_flag(control, (int32_t)NT_SLIDER_DIRECTION_FLAG);
        }
        else
        {
            _nt_control_clear_flag(control, (int32_t)NT_SLIDER_DIRECTION_FLAG);
        }
        ram->position = (uint8_t)position;
        _nt_control_set_flag(control, (int32_t)NT_SLIDER_MOVEMENT_FLAG);
        _nt_control_slider_invoke_callback(control, NT_SLIDER_MOVEMENT, position);
    }
    else
    {
        _nt_control_clear_flag(control, (int32_t)NT_SLIDER_MOVEMENT_FLAG);
    }
}

static void _nt_control_slider_calc_position(struct nt_control_data *control,
                                             uint32_t first_elec,
                                             uint32_t last_elec,
                                             uint32_t touch_count)
{
    if (((last_elec - first_elec) == touch_count) && (touch_count <= 2U))
    {
        /* valid position */
        _nt_control_clear_flag(control, (int32_t)NT_SLIDER_INVALID_POSITION_FLAG);
        uint32_t position = ((first_elec << 1U) + touch_count) - 1U;
        _nt_control_slider_calc_dynamic(control, position);
    }
    else
    {
        _nt_control_set_flag(control, (int32_t)NT_SLIDER_INVALID_POSITION_FLAG);
    }
}

static int32_t _nt_control_slider_init(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_slider_interface);

    control->data.slider = _nt_mem_alloc(sizeof(struct nt_control_slider_data));

    if (control->data.slider == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if ((bool)_nt_control_check_data(control) != (bool)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_control_slider_process(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_slider_interface);

    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_EN_FLAG) ||
        !(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG))
    {
        return (int32_t)NT_FAILURE; /* control disabled or data not ready */
    }

    struct nt_control_slider_data *ram = control->data.slider;
    uint64_t elec_state                = _nt_control_get_electrodes_state(control);
    if (!(bool)elec_state)
    {
        /* all released */
        if ((bool)_nt_control_get_flag(control, (int32_t)NT_SLIDER_TOUCH_FLAG))
        {
            /* if none is touched & touch was reported, all released event */
            _nt_control_clear_flag(control, (int32_t)NT_SLIDER_TOUCH_FLAG);
            _nt_control_clear_flag(control, (int32_t)NT_SLIDER_MOVEMENT_FLAG);
            _nt_control_slider_invoke_callback(control, NT_SLIDER_ALL_RELEASE, (uint32_t)ram->position);
        }
        _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
        return (int32_t)NT_SUCCESS;                                         /* no touch on the control's electrodes */
    }

    uint32_t last_elec   = _nt_control_get_last_elec_touched(elec_state);
    uint32_t first_elec  = _nt_control_get_first_elec_touched(elec_state);
    uint32_t touch_count = _nt_control_get_touch_count(elec_state);
    _nt_control_slider_calc_position(control, first_elec, last_elec, touch_count);

    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_SLIDER_TOUCH_FLAG))
    {
        _nt_control_slider_invoke_callback(control, NT_SLIDER_INITIAL_TOUCH, (uint32_t)ram->position);
    }
    _nt_control_set_flag(control, (int32_t)NT_SLIDER_TOUCH_FLAG);

    _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */
    return (int32_t)NT_SUCCESS;
}
