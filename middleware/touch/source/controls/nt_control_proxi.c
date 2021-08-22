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
#include "nt_control_proxi.h"
#include "nt_control_proxi_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "../source/filters/nt_filters_prv.h"

/* Invoke callback based on the even has occurred */
static void _nt_control_proxi_invoke_callback(struct nt_control_data *control,
                                              enum nt_control_proxi_event event,
                                              uint32_t index_or_gesture,
                                              uint32_t proximity);

/* The proximity control initialization function. */
static int32_t _nt_control_proxi_init(struct nt_control_data *control);
/* The proximity control process function. */
static int32_t _nt_control_proxi_process(struct nt_control_data *control);

/* The proximity control interface. */
const struct nt_control_interface nt_control_proxi_interface = {
    .init        = _nt_control_proxi_init,
    .process     = _nt_control_proxi_process,
    .name        = NT_CONTROL_PROXI_NAME,
    .params_size = sizeof(struct nt_control_proxi),
};

void nt_control_proxi_register_callback(const struct nt_control *control, nt_control_proxi_callback callback)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_proxi_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_proxi_data *ram = control_data->data.proxi;
    NT_ASSERT(ram != NULL);

    ram->callback = callback;
}

static void _nt_control_proxi_invoke_callback(struct nt_control_data *control,
                                              enum nt_control_proxi_event event,
                                              uint32_t index_or_gesture,
                                              uint32_t proximity)
{
    struct nt_control_proxi_data *ram = control->data.proxi;
    NT_ASSERT(ram != NULL);

    if (ram->callback != NULL)
    {
        ram->callback(control->rom, event, index_or_gesture, proximity);
    }
}

int32_t nt_control_proxi_set_gesture(const struct nt_control *control, uint8_t gesture)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_proxi_interface);

    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);

    struct nt_control_proxi_data *ram = control_data->data.proxi;
    NT_ASSERT(ram != NULL);

    if (gesture < (uint16_t)NT_PROXI_GESTURES)
    {
        ram->select_gesture = (uint8_t)(gesture + 1U);
        if (gesture >= ram->gesture_number)
        {
            ram->gesture_number = gesture + 1U;
        }
        return (int32_t)NT_SUCCESS;
    }
    return (int32_t)NT_FAILURE;
}

static int32_t _nt_control_proxi_init(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_proxi_interface);
    NT_ASSERT(control->rom->control_params.proxi != NULL);

    const struct nt_control_proxi *proxi = (const struct nt_control_proxi *)control->rom->control_params.proxi;

    if (proxi->range == 0U)
    {
        return (int32_t)NT_FAILURE;
    }

    if (proxi->threshold == 0U)
    {
        return (int32_t)NT_FAILURE;
    }

    control->data.proxi = _nt_mem_alloc(sizeof(struct nt_control_proxi_data));

    if (control->data.proxi == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }

    if ((bool)_nt_control_check_data(control) != (bool)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    NT_ASSERT(control->data.proxi != NULL);
    struct nt_control_proxi_data *ram = control->data.proxi;
    /* Initialization of maximum proximity for all proximity electrodes from proxi->scale */
    uint32_t elec_counter = control->electrodes_size;
    while ((bool)(elec_counter--))
    { /* Assign some proxi_max value to make proximity working when max proximity value is not obtained. */
        ram->proxi_max[elec_counter] = proxi->scale;
        /* proxi_movement initialization to be same as threshold value */
        ram->proxi_movement[elec_counter] = proxi->threshold;
    }
#if NT_PROXI_ELECTRODES == 1 /* if one electrode defined initialize index to zero. */
    ram->index = 0;          /* Set key index to zero because only one electrode assigned */
#endif                       /* NT_PROXI_ELECTRODES < 2  */

#if NT_PROXI_GESTURES > 0
    ram->recognized_gesture = 0; /* Initialization recognized gesture variable */
    ram->select_gesture     = 0; /* Initialization selected gesture to store variable */
    ram->buffer             = 0; /* Initialization buffer variable */

    /* Check if gestures are defined */
    if (*(uint32_t *)proxi->gesture == 0U)
    {
        return (int32_t)NT_FAILURE;
    }
    /* Evaluate how many gestures was defined */
    ram->gesture_number = 0;
    uint32_t *gesture   = proxi->gesture;
    while (*(uint32_t *)gesture++ != 0U)
    {
        ram->gesture_number++;
    }

    if (control->electrodes_size > 8U)
    {
        return (int32_t)NT_FAILURE;
    }

    /* Set buffer mask constant */
    uint32_t buffer_mask = 0;
    elec_counter         = control->electrodes_size;
    while ((bool)(elec_counter--))
    {
        buffer_mask |= (0xfUL << (elec_counter << 2U));
    }
    ram->buffer_mask = buffer_mask;

#endif /* NT_PROXI_GESTURES > 0 */
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_control_proxi_process(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_proxi_interface);

    uint32_t elec_counter = control->electrodes_size;
    uint32_t current_prox = 0;

    struct nt_control_proxi_data *ram  = control->data.proxi;
    const struct nt_control_proxi *rom = control->rom->control_params.proxi;

    NT_ASSERT(ram != NULL);
    NT_ASSERT(rom != NULL);

    if (!(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_EN_FLAG) ||
        !(bool)_nt_control_get_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG))
    {
        return (int32_t)NT_FAILURE; /* control disabled or data not ready */
    }

    /* Proximity process */
    while ((bool)(elec_counter--))
    {
        /* Get electrodes delta values */
        current_prox = (uint16_t)_nt_abs_int32(_nt_electrode_get_delta(control->electrodes[elec_counter]));

        /* Finding maximum delta value to calculate proximity */
        if ((uint32_t)ram->proxi_max[elec_counter] < current_prox)
        {
            ram->proxi_max[elec_counter] = (uint16_t)current_prox;
        }

        /* denominator in not zero (checked in init) so calculate scaled proximity */
        current_prox = (uint32_t)((uint32_t)current_prox * (uint32_t)rom->range);
        current_prox = (uint32_t)((uint32_t)current_prox / (uint16_t)ram->proxi_max[elec_counter]);

        /* Store proximity to array for gesture processing */
        ram->proxi_curr[elec_counter] = (uint8_t)current_prox;

        /* If touch, check change and direction */
        if ((bool)_nt_control_proxi_get_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_TOUCH_FLAG))
        { /* Check insensitivity for movement neglecting. */
            if (current_prox >= ((uint32_t)ram->proxi_movement[elec_counter] + (uint32_t)rom->insensitivity))
            {
                _nt_control_proxi_set_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_DIRECTION_FLAG);
                _nt_control_set_flag(control, (int32_t)NT_PROXI_DIRECTION_FLAG);
                if ((uint16_t)ram->proxi_movement[elec_counter] + (uint16_t)rom->insensitivity < 255U)
                {
                    ram->proxi_movement[elec_counter] += rom->insensitivity;
                }
                /* flags and callbacks when moving (proximity changing) */
                _nt_control_proxi_set_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_MOVEMENT_FLAG);
                _nt_control_set_flag(control, (int32_t)NT_PROXI_MOVEMENT_FLAG);
                _nt_control_proxi_invoke_callback(control, NT_PROXI_MOVEMENT, elec_counter, current_prox);
            }
            else if (current_prox < ram->proxi_movement[elec_counter])
            {
                _nt_control_proxi_clear_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_DIRECTION_FLAG);
                _nt_control_clear_flag(control, (int32_t)NT_PROXI_DIRECTION_FLAG);
                if ((uint16_t)ram->proxi_movement[elec_counter] - (uint16_t)rom->insensitivity > 0U)
                {
                    ram->proxi_movement[elec_counter] -= rom->insensitivity;
                }
                /* flags and callbacks when moving (proximity changing) */
                _nt_control_proxi_set_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_MOVEMENT_FLAG);
                _nt_control_set_flag(control, (int32_t)NT_PROXI_MOVEMENT_FLAG);
                _nt_control_proxi_invoke_callback(control, NT_PROXI_MOVEMENT, elec_counter, current_prox);
            }
            else /* Too small proximity change - will be neglected */
            {
                _nt_control_proxi_clear_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_MOVEMENT_FLAG);
                _nt_control_clear_flag(control, (int32_t)NT_PROXI_MOVEMENT_FLAG);
            }
        }
        ram->proxi_curr[elec_counter] = (uint8_t)current_prox; /* store proximity value for next step */

        /* Touch recognition */
        if (current_prox > rom->threshold) /* Check Touch threshold */
        {                                  /* Skip if touch was detected previously, only one touch event call */
            if (!(bool)_nt_control_proxi_get_flag((&ram->proxi_flags[elec_counter]), (int32_t)NT_PROXI_TOUCH_FLAG))
            {
                _nt_control_set_flag(control, (int32_t)NT_PROXI_TOUCH_FLAG);
                _nt_control_proxi_set_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_TOUCH_FLAG);
                _nt_control_proxi_clear_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_RELEASE_FLAG);
                _nt_control_proxi_invoke_callback(control, NT_PROXI_TOUCH, elec_counter, current_prox);
            }
        }
        else /* Release event */
        {
            _nt_control_clear_flag(control, (int32_t)NT_PROXI_TOUCH_FLAG);    /* released */
            _nt_control_clear_flag(control, (int32_t)NT_PROXI_MOVEMENT_FLAG); /* clear flag */
            _nt_control_proxi_clear_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_TOUCH_FLAG); /* released */
            _nt_control_proxi_clear_flag(&ram->proxi_flags[elec_counter],
                                         (int32_t)NT_PROXI_MOVEMENT_FLAG); /* clear flag */
            ram->proxi_movement[elec_counter] = rom->threshold;
            /* Call just once, if already released, no more calls*/
            if (!(bool)_nt_control_proxi_get_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_RELEASE_FLAG))
            {
                _nt_control_proxi_invoke_callback(control, NT_PROXI_RELEASE, elec_counter, current_prox);
                _nt_control_proxi_set_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_RELEASE_FLAG);
                _nt_control_set_flag(control, (int32_t)NT_PROXI_RELEASE_FLAG);
            }
        }
    }
/* GESTURE RECOGNITION */
/* index (position) recognition */
#if NT_PROXI_ELECTRODES > 1 /* must be more then 2 electrodes to find proxi max. */
    elec_counter = control->electrodes_size;
    if ((bool)_nt_control_get_flag(control, (int32_t)NT_PROXI_TOUCH_FLAG))
    {
        ram->proximity = 0;            /* Clear proximity max. */
        while ((bool)(elec_counter--)) /* Find proximity max and its electrode */
        {
            if (ram->proxi_curr[elec_counter] > ram->proximity) /* Compare electrode proximity with max values */
            {
                ram->proximity = ram->proxi_curr[elec_counter]; /* Store current in case of higher value was found */
                if ((bool)_nt_control_proxi_get_flag(&ram->proxi_flags[elec_counter], (int32_t)NT_PROXI_TOUCH_FLAG))
                {
                    ram->index = elec_counter; /* Store index of the touched electrode with max proximity */
                }
            }
        }
#if NT_PROXI_GESTURES > 0                      /* Avoid the code execution if PROXI_GESTURES not required. */
        if (ram->index != ram->index_k_1)      /* Max proximity electrode was changed */
        {                                      /* Storing max electrode to buffer from right to left */
            ram->buffer = ram->buffer << 4;    /* Shift whole buffer left to prepare for new step */
            ram->buffer &= (ram->buffer_mask); /* Clean longer history then electrode number */
            ram->buffer |= ram->index;         /* Store new point on the right */
            ram->index_k_1 = ram->index;       /* Store electrode index for next step */

            uint8_t n;
            /* guesture recognition */
            if (ram->select_gesture > 0) /* Store gesture captured in buffer (gesture 1 has array index 0)*/
            {
                if (ram->select_gesture > ram->gesture_number) /* Ensure to rewrite defines gesture number only */
                    ram->select_gesture = 0;
                rom->gesture[ram->select_gesture - 1] = ram->buffer; /* Store buffered value to gesture array */
                _nt_control_proxi_invoke_callback(control, (int32_t)NT_PROXI_SET_GESTURE, ram->select_gesture,
                                                  current_prox);
                ram->select_gesture = 0; /* Clean selected gesture quantity for next gesture storing */
            }
            else
            {
                ram->recognized_gesture = 0; /* Clear recognized gesture value */
                uint32_t temp_gesture;
                for (n = 0; n < ram->gesture_number; n++)
                {
                    temp_gesture = rom->gesture[n];  /* Read from Rom */
                    if (temp_gesture == ram->buffer) /* Compare buffered gesture with stored one */
                    {
                        ram->recognized_gesture = n + 1; /* In case of sameness store the gesture number */
                    }
                }
                if (ram->recognized_gesture != 0)
                    _nt_control_proxi_invoke_callback(control, (int32_t)NT_PROXI_GET_GESTURE, ram->recognized_gesture,
                                                      current_prox);
            }
        }
#endif /* #if NT_PROXI_GESTURES > 0 */
    }
#else                                                                   /* #if NT_PROXI_ELECTRODES > 0 */
    ram->proximity = (uint8_t)current_prox; /* Store current proxi if only one electrode assigned */
#endif                                                                  /* #if NT_PROXI_ELECTRODES > 0 */
    _nt_control_clear_flag(control, (int32_t)NT_CONTROL_NEW_DATA_FLAG); /* data processed */

    return (int32_t)NT_SUCCESS;
}
