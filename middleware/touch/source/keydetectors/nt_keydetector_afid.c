/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/electrodes/nt_electrodes_prv.h"
#include "../source/system/nt_system_prv.h"
#include "../source/keydetectors/nt_keydetector_afid_prv.h"
#include "nt_keydetectors.h"
#include "../source/system/nt_system_mem_prv.h"

static void _nt_keydetector_afid_asc_init(struct nt_keydetector_afid_data *ram);
static void _nt_keydetector_afid_asc_track(const struct nt_keydetector_afid *rom, struct nt_keydetector_afid_data *ram);
static void _nt_keydetector_afid_asc_process(const struct nt_keydetector_afid *rom,
                                             struct nt_keydetector_afid_data *ram);
static void _nt_keydetector_afid_measure(struct nt_electrode_data *electrode, uint32_t signal);

static int32_t _nt_keydetector_afid_init(struct nt_electrode_data *electrode);
static int32_t _nt_keydetector_afid_rom_check(const struct nt_keydetector_afid *rom);
static void _nt_keydetector_afid_process(struct nt_electrode_data *electrode);
static void _nt_keydetector_afid_enable(struct nt_electrode_data *electrode, uint32_t touch);
static void _nt_keydetector_afid_reset(struct nt_electrode_data *electrode);

const struct nt_keydetector_interface nt_keydetector_afid_interface = {
    .nt_keydetector_init    = _nt_keydetector_afid_init,
    .nt_keydetector_enable  = _nt_keydetector_afid_enable,
    .nt_keydetector_process = _nt_keydetector_afid_process,
    .nt_keydetector_measure = _nt_keydetector_afid_measure,
    .nt_keydetector_reset   = _nt_keydetector_afid_reset,
    .name                   = "nt_keydetector_afid_interface",
    .params_size            = sizeof(struct nt_keydetector_afid),
};

static int32_t _nt_keydetector_afid_rom_check(const struct nt_keydetector_afid *rom)
{
    if (rom->fast_signal_filter.cutoff == 0)
    {
        return (int32_t)NT_FAILURE;
    }
    if (rom->slow_signal_filter.cutoff == 0)
    {
        return (int32_t)NT_FAILURE;
    }
    if (rom->base_avrg.n2_order == 0)
    {
        return (int32_t)NT_FAILURE;
    }
    return (int32_t)NT_SUCCESS;
}

static void _nt_keydetector_afid_asc_init(struct nt_keydetector_afid_data *ram)
{
    ram->asc.max_resets = 0;
}

static void _nt_keydetector_afid_asc_track(const struct nt_keydetector_afid *rom, struct nt_keydetector_afid_data *ram)
{
    if (ram->asc.max_resets < (int16_t)ram->touch_reset_counter)
    {
        ram->asc.max_resets = (int16_t)ram->touch_reset_counter;
    }
}

static void _nt_keydetector_afid_asc_process(const struct nt_keydetector_afid *rom,
                                             struct nt_keydetector_afid_data *ram)
{
    /* if transition from electrode touch state to release happen */
    if (ram->asc.max_resets != 0)
    {
        uint32_t touch_threshold = ram->touch_threshold;
        if (ram->asc.max_resets > rom->asc.resets_for_touch)
        {
            touch_threshold =
                (touch_threshold * (uint32_t)(ram->asc.max_resets)) / ((uint16_t)rom->asc.resets_for_touch * 4U);

            if (touch_threshold <= rom->asc.noise_resets_minimum)
            { /* check if sensitivity is not too low */
                touch_threshold = rom->asc.noise_resets_minimum;
            }
        }
        else
        {
            touch_threshold = rom->asc.noise_resets_minimum;
        }

        ram->touch_threshold = touch_threshold;

        ram->asc.max_resets = 0;
    }
}

static int32_t _nt_keydetector_afid_init(struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_afid_interface);

    const struct nt_keydetector_afid *rom = electrode->rom->keydetector_params.afid;

    if (_nt_keydetector_afid_rom_check(rom) != (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    electrode->keydetector_data.afid =
        (struct nt_keydetector_afid_data *)_nt_mem_alloc(sizeof(struct nt_keydetector_afid_data));

    if (electrode->keydetector_data.afid == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }
    return (int32_t)NT_SUCCESS;
}

static void _nt_keydetector_afid_enable(struct nt_electrode_data *electrode, uint32_t touch)
{
    const struct nt_keydetector_afid *rom = electrode->rom->keydetector_params.afid;
    struct nt_keydetector_afid_data *ram  = electrode->keydetector_data.afid;

    ram->integration_value     = NT_KEYDETECTOR_AFID_INITIAL_INTEGRATOR_VALUE;
    ram->touch_threshold       = rom->asc.noise_resets_minimum;
    ram->touch_reset_counter   = NT_KEYDETECTOR_AFID_INITIAL_RESET_TOUCH_COUNTER_VALUE;
    ram->release_reset_counter = NT_KEYDETECTOR_AFID_INITIAL_RESET_RELEASE_COUNTER_VALUE;
    _nt_keydetector_afid_asc_init(ram);

    if ((bool)touch)
    {
        _nt_electrode_set_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
    else
    {
        _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
    ram->filter_state = (int32_t)NT_FILTER_STATE_INIT;
    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_INIT);
}

static void _nt_keydetector_afid_measure(struct nt_electrode_data *electrode, uint32_t signal)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_afid_interface);

    struct nt_keydetector_afid_data *ram  = electrode->keydetector_data.afid;
    const struct nt_keydetector_afid *rom = electrode->rom->keydetector_params.afid;

    signal = _nt_electrode_normalization_process(electrode, signal);

    if (ram->filter_state == (uint8_t)NT_FILTER_STATE_INIT)
    {
        _nt_filter_fbutt_init(&rom->fast_signal_filter, &ram->fast_signal_filter, signal);
        _nt_filter_fbutt_init(&rom->slow_signal_filter, &ram->slow_signal_filter, signal);
        (void)(int32_t) _nt_filter_moving_average_init(&rom->base_avrg, &ram->base_avrg, (uint16_t)signal);
        electrode->baseline = (uint16_t)signal;
        ram->slow_signal    = (uint16_t)signal;
        _nt_electrode_set_signal(electrode, signal);
        ram->filter_state = (uint8_t)NT_FILTER_STATE_RUN;
    }
    else
    {
        signal               = _nt_electrode_shielding_process(electrode, signal);
        uint32_t prev_signal = (uint32_t)_nt_electrode_get_signal(electrode);
        signal               = _nt_filter_iir_process(&rom->signal_filter, signal, prev_signal);

        ram->slow_signal = (uint16_t)_nt_filter_fbutt_process(&ram->slow_signal_filter, (uint16_t)signal);
        signal           = (uint16_t)_nt_filter_fbutt_process(&ram->fast_signal_filter, (uint16_t)signal);
        _nt_electrode_set_signal(electrode, signal);
    }
    /* calculate delta (subtraction of fast and slow filter) */
    int32_t integration_value = (int32_t)((int32_t)signal - (int32_t)ram->slow_signal);
    /* signal drift suppression - caused by not ideal filters */
    if (_nt_abs_int32(integration_value) <= 8U)
    {
        integration_value = 0;
    }
    /* process integration */
    integration_value += (int32_t)ram->integration_value;
    /* process resetting */
    uint32_t touch_reset_counter   = ram->touch_reset_counter;
    uint32_t release_reset_counter = ram->release_reset_counter;
    /* perform reset function and touch detection */
    if (_nt_abs_int32(integration_value) > ram->touch_threshold)
    {
        if (integration_value < 0)
        {
            /* negative saturation reset - release */
            /* report release state after so many resets happen as many touch resets happen */
            if (touch_reset_counter > 4U)
            {
                touch_reset_counter -= 4U;
            }
            else
            {
                release_reset_counter++;
                touch_reset_counter = 0U;
            }
        }
        else
        {
            /* positive saturation reset - touch */
            /* report touch status */
            touch_reset_counter++;
            release_reset_counter = 0U;
        }
        integration_value = 0;
    }

    ram->touch_reset_counter   = touch_reset_counter;
    ram->release_reset_counter = release_reset_counter;
    ram->integration_value     = (int16_t)integration_value;

    /* perform time dependent operations if release state */
    if (release_reset_counter > 0U)
    {
        /* slow touch threshold fall if touch threshold > destination threshold */
        if (ram->touch_threshold > rom->asc.noise_resets_minimum)
        {
            if (!(bool)(_nt_electrode_get_time_offset_period(electrode, (uint32_t)rom->asc.touch_threshold_fall_rate)))
            {
                ram->touch_threshold -= (ram->touch_threshold / rom->asc.noise_resets_minimum);
                if (ram->touch_threshold < rom->asc.noise_resets_minimum)
                {
                    ram->touch_threshold = rom->asc.noise_resets_minimum;
                }
            }
        }
        /* integration reset - for suppression of long-term temperature drift */
        if (!(bool)(_nt_electrode_get_time_offset_period(electrode, rom->reset_rate)))
        {
            ram->integration_value = NT_KEYDETECTOR_AFID_INITIAL_INTEGRATOR_VALUE;
        }

        /* Update the baseline */
        electrode->baseline =
            (uint16_t)_nt_filter_moving_average_process(&rom->base_avrg, &ram->base_avrg, (uint16_t)signal);
    }
}

static void _nt_keydetector_afid_process(struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_afid_interface);

    struct nt_keydetector_afid_data *ram  = electrode->keydetector_data.afid;
    const struct nt_keydetector_afid *rom = electrode->rom->keydetector_params.afid;
    struct nt_kernel *system              = _nt_system_get();

    switch (_nt_electrode_get_last_status(electrode))
    {
        case (int32_t)NT_ELECTRODE_STATE_INIT:
            /* manage switch from init to run phase */
            if (_nt_electrode_get_time_offset(electrode) >= system->rom->init_time)
            {
                _nt_keydetector_afid_asc_track(rom, ram);
                _nt_keydetector_afid_asc_process(rom, ram);

                if ((bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
                {
                    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_TOUCH);
                }
                else
                {
                    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_RELEASE);
                }

                _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
            }
            break;
        case (int32_t)NT_ELECTRODE_STATE_TOUCH:
            if (ram->release_reset_counter > 0U)
            {
                _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_RELEASE);
            }
            else
            {
                _nt_keydetector_afid_asc_track(rom, ram);

                if (ram->touch_reset_counter < ((uint32_t)rom->asc.resets_for_touch >> 1))
                {
                    ram->touch_reset_counter = 0;
                }
            }
            break;

        case (int32_t)NT_ELECTRODE_STATE_RELEASE:
            if (ram->touch_reset_counter > (uint32_t)rom->asc.resets_for_touch)
            {
                _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_TOUCH);
            }
            else
            {
                _nt_keydetector_afid_asc_process(rom, ram);
            }
            break;

        default:
            /*MISRA rule 16.4*/
            break;
    }
}

static void _nt_keydetector_afid_reset(struct nt_electrode_data *electrode)
{
    uint32_t signal                       = _nt_electrode_get_raw_signal(electrode);
    struct nt_keydetector_afid_data *ram  = electrode->keydetector_data.afid;
    const struct nt_keydetector_afid *rom = electrode->rom->keydetector_params.afid;

    signal = _nt_electrode_normalization_process(electrode, signal);

    _nt_filter_fbutt_init(&rom->fast_signal_filter, &ram->fast_signal_filter, signal);
    _nt_filter_fbutt_init(&rom->slow_signal_filter, &ram->slow_signal_filter, signal);
    (void)(int32_t) _nt_filter_moving_average_init(&rom->base_avrg, &ram->base_avrg, (uint16_t)signal);
    electrode->baseline = (uint16_t)signal;
    ram->slow_signal    = (uint16_t)signal;
    _nt_electrode_set_signal(electrode, signal);
}
