/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
static void _nt_keydetector_safa_measure(struct nt_electrode_data *electrode, uint32_t signal);
static void _nt_keydetector_safa_process(struct nt_electrode_data *electrode);
static void _nt_keydetector_safa_enable(struct nt_electrode_data *electrode, uint32_t touch);
static void _nt_keydetector_safa_reset(struct nt_electrode_data *electrode);

const struct nt_keydetector_safa nt_keydetector_safa_default = {
    .signal_filter         = {.coef1 = 2},
    .base_avrg             = {.n2_order = 9},
    .non_activity_avrg     = {.n2_order = (int32_t)NT_FILTER_MOVING_AVERAGE_MAX_ORDER},
    .entry_event_cnt       = 4,
    .signal_to_noise_ratio = 8,
    .deadband_cnt          = 5,
    .min_noise_limit       = 20,
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
    return (int32_t)NT_SUCCESS;
}

static int32_t _nt_keydetector_safa_init(struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;

    if (_nt_keydetector_safa_rom_check(rom) != (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    electrode->keydetector_data.safa =
        (struct nt_keydetector_safa_data *)_nt_mem_alloc(sizeof(struct nt_keydetector_safa_data));

    if (electrode->keydetector_data.safa == NULL)
    {
        return (int32_t)NT_OUT_OF_MEMORY;
    }
    electrode->keydetector_data.safa->recovery_cnt = 0;

    electrode->keydetector_data.safa->base_avrg_init  = rom->base_avrg;
    electrode->keydetector_data.safa->noise_avrg_init = rom->base_avrg;
    electrode->keydetector_data.safa->noise_avrg_init.n2_order += 2;

    if (electrode->keydetector_data.safa->noise_avrg_init.n2_order > (int32_t)NT_FILTER_MOVING_AVERAGE_MAX_ORDER)
    {
        electrode->keydetector_data.safa->noise_avrg_init.n2_order = (int32_t)NT_FILTER_MOVING_AVERAGE_MAX_ORDER;
    }

    /* fast noise filter */
    electrode->keydetector_data.safa->f_noise_avrg_init = rom->base_avrg;
    electrode->keydetector_data.safa->f_noise_avrg_init.n2_order =
        electrode->keydetector_data.safa->noise_avrg_init.n2_order - 4;
    if (electrode->keydetector_data.safa->f_noise_avrg_init.n2_order < 6)
    {
        electrode->keydetector_data.safa->f_noise_avrg_init.n2_order = 6;
    }

    electrode->keydetector_data.safa->predicted_signal_avrg_init = rom->base_avrg;
    if (electrode->keydetector_data.safa->predicted_signal_avrg_init.n2_order > 2)
    {
        electrode->keydetector_data.safa->predicted_signal_avrg_init.n2_order -= 2;
    }
    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_INIT);

    return (int32_t)NT_SUCCESS;
}

static void _nt_keydetector_safa_enable(struct nt_electrode_data *electrode, uint32_t touch)
{
    struct nt_keydetector_safa_data *ram = electrode->keydetector_data.safa;

    if ((bool)touch)
    {
        _nt_electrode_set_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
    else
    {
        _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
    ram->filter_state = NT_FILTER_STATE_INIT;
    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_INIT);
}

static void _nt_keydetector_safa_measure(struct nt_electrode_data *electrode, uint32_t signal)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram  = electrode->keydetector_data.safa;

    signal = _nt_electrode_normalization_process(electrode, signal);

    if (ram->filter_state == NT_FILTER_STATE_INIT)
    {
        _nt_reset_keydetector_safa_reset(
            electrode, signal, _nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG));
        ram->filter_state = NT_FILTER_STATE_RUN;
    }
    else
    {
        signal               = _nt_electrode_shielding_process(electrode, signal);
        uint32_t prev_signal = (uint32_t)_nt_electrode_get_signal(electrode);
        uint32_t iir_signal  = _nt_filter_iir_process(&rom->signal_filter, signal, prev_signal);
        _nt_electrode_set_signal(electrode, iir_signal);

        _nt_keydetector_safa_signal_track(electrode, rom, ram, (uint16_t)iir_signal);
    }
}

static void _nt_reset_keydetector_safa_reset(struct nt_electrode_data *electrode, uint32_t signal, uint32_t touch)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram  = electrode->keydetector_data.safa;

    _nt_electrode_clear_flag(
        electrode, (uint32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG | (uint32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG |
                       (uint32_t)NT_ELECTRODE_DIGITAL_RESULT_ONLY_FLAG | (uint32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);

    if (!(bool)touch)
    {
        _nt_electrode_set_signal(electrode, signal);

        ram->base_avrg_init.n2_order = rom->base_avrg.n2_order;
        (void)(int32_t) _nt_filter_moving_average_init(&ram->base_avrg_init, &ram->base_avrg, (uint16_t)signal);
        electrode->baseline = electrode->signal;
        (void)(int32_t)
            _nt_filter_moving_average_init(&ram->noise_avrg_init, &ram->noise_avrg, (uint16_t)rom->min_noise_limit);
        /* fast noise filter */
        (void)(int32_t) _nt_filter_moving_average_init(&ram->f_noise_avrg_init, &ram->f_noise_avrg,
                                                       (uint16_t)(rom->min_noise_limit));

        ram->predicted_signal = signal + rom->min_noise_limit * rom->signal_to_noise_ratio;
        (void)(int32_t) _nt_filter_moving_average_init(&rom->non_activity_avrg, &ram->predicted_signal_avrg,
                                                       (uint16_t)ram->predicted_signal);

        ram->noise   = rom->min_noise_limit;
        ram->f_noise = rom->min_noise_limit;

        _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG);
    }
    else
    {
        _nt_electrode_set_signal(electrode, signal);

        uint32_t temp       = signal >> 2;
        int32_t temp1       = (int32_t)signal - (int32_t)(temp);
        electrode->baseline = _nt_filter_limit_u(temp1, 0U, 50000U);

        ram->base_avrg_init = rom->non_activity_avrg;
        (void)(int32_t) _nt_filter_moving_average_init(&ram->base_avrg_init, &ram->base_avrg, electrode->baseline);

        (void)(void)(int32_t)_nt_filter_moving_average_init(&ram->noise_avrg_init, &ram->noise_avrg,
                                                            (uint16_t)rom->min_noise_limit);
        /* fast noise filter */
        (void)(int32_t)
            _nt_filter_moving_average_init(&ram->f_noise_avrg_init, &ram->f_noise_avrg, (uint16_t)rom->min_noise_limit);

        ram->predicted_signal = signal;
        (void)(int32_t) _nt_filter_moving_average_init(&ram->predicted_signal_avrg_init, &ram->predicted_signal_avrg,
                                                       (uint16_t)ram->predicted_signal);

        ram->noise   = rom->min_noise_limit;
        ram->f_noise = rom->min_noise_limit;

        _nt_keydetector_safa_lock_baseline(electrode, rom, ram, (uint16_t)signal);
        _nt_electrode_set_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
}

static void _nt_keydetector_safa_process(struct nt_electrode_data *electrode)
{
    NT_ASSERT(electrode->rom->keydetector_interface == &nt_keydetector_safa_interface);

    const struct nt_keydetector_safa *rom = electrode->rom->keydetector_params.safa;
    struct nt_keydetector_safa_data *ram  = electrode->keydetector_data.safa;
    struct nt_kernel *system              = _nt_system_get();

    uint16_t signal = (uint16_t)_nt_electrode_get_signal(electrode);
    int32_t delta   = (int32_t)_nt_filter_abs((int16_t)((int16_t)electrode->baseline - (int16_t)signal));

    uint16_t sig_filter = signal;

    switch (_nt_electrode_get_last_status(electrode))
    {
        case (int32_t)NT_ELECTRODE_STATE_INIT:
            /* manage switch from electrode init to run phase */
            if (_nt_electrode_get_time_offset(electrode) >= system->rom->init_time)
            {
                ram->entry_event_cnt = 0;
                ram->deadband_cnt    = (int32_t)rom->deadband_cnt;

                if ((bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
                {
                    if (sig_filter < (ram->noise * rom->signal_to_noise_ratio))
                    {
                        sig_filter = (uint16_t)(ram->noise) * (uint16_t)(rom->signal_to_noise_ratio);
                    }
                    (void)(int32_t) _nt_filter_moving_average_init(&ram->predicted_signal_avrg_init,
                                                                   &ram->predicted_signal_avrg, sig_filter);
                    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_TOUCH);
                    _nt_keydetector_safa_lock_baseline(electrode, rom, ram, (uint16_t)signal);
                }
                else
                {
                    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_RELEASE);
                    (void)(int32_t) _nt_filter_moving_average_init(&rom->non_activity_avrg, &ram->predicted_signal_avrg,
                                                                   (uint16_t)(ram->predicted_signal));
                    _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, (uint16_t)signal);
                }
            }
            break;
        case (int32_t)NT_ELECTRODE_STATE_TOUCH:
            if (delta < ((int32_t)_nt_filter_abs(
                            (int16_t)((int16_t)ram->predicted_signal - (int16_t)electrode->baseline) * 4 / 5)) &&
                (ram->deadband_cnt == 0)) /* 80% release thresh */
            {
                ram->entry_event_cnt = 0;
                ram->deadband_cnt    = (int16_t)(rom->deadband_cnt);
                _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_RELEASE);
                (void)(int32_t) _nt_filter_moving_average_init(&rom->non_activity_avrg, &ram->predicted_signal_avrg,
                                                               (uint16_t)(ram->predicted_signal));

                /* is init touch */
                if ((bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
                {
                    if (!(bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG))
                    {
                        _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, (uint16_t)signal);
                    }
                }
            }
            else
            {
                ram->entry_event_cnt = 0;
                if ((bool)(ram->deadband_cnt))
                {
                    ram->deadband_cnt--;
                }
            }

            break;
        case (int32_t)NT_ELECTRODE_STATE_RELEASE:
            if ((((uint16_t)delta > _nt_filter_abs((int16_t)ram->predicted_signal - (int16_t)electrode->baseline) >>
                  2)) &&
                (ram->deadband_cnt == 0) &&
                (!(bool)_nt_filter_is_deadrange_u(signal, electrode->baseline,
                                                  (uint16_t)(ram->noise * rom->signal_to_noise_ratio))))
            {
                ram->entry_event_cnt++;

                if (ram->entry_event_cnt > (int32_t)(rom->entry_event_cnt)) /* debouncing */
                {
                    ram->entry_event_cnt = 0;
                    ram->deadband_cnt    = (int32_t)(rom->deadband_cnt);

                    if (!(bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG))
                    {
                        _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
                    }

                    (void)(int32_t) _nt_filter_moving_average_init(
                        &ram->predicted_signal_avrg_init, &ram->predicted_signal_avrg,
                        _nt_filter_deadrange_u(signal, electrode->baseline,
                                               (uint16_t)(ram->noise * rom->signal_to_noise_ratio)));
                    _nt_electrode_set_status(electrode, (int32_t)NT_ELECTRODE_STATE_TOUCH);
                }
            }

            else
            {
                ram->entry_event_cnt = 0;

                if ((bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG))
                {
                    if ((bool)ram->deadband_cnt)
                    {
                        ram->deadband_cnt--;
                        break;
                    }
                }
                else
                {
                    if ((bool)(ram->recovery_cnt))
                    {
                        ram->recovery_cnt--;
                        if (ram->recovery_cnt == 0)
                        {
                            ram->base_avrg_init = rom->base_avrg;
                            (void)(int32_t) _nt_filter_moving_average_init(&ram->base_avrg_init, &ram->base_avrg,
                                                                           electrode->baseline);
                        }
                        break;
                    }
                }

                ram->deadband_cnt = 0;
                ram->recovery_cnt = 0;

                if (_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG))
                {
                    _nt_keydetector_safa_lock_baseline(electrode, rom, ram, signal);
                }
                if ((_nt_electrode_get_flag(electrode, (uint32_t)NT_ELECTRODE_LOCK_BASELINE_REQ_FLAG |
                                                                 (uint32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG) ==
                           (uint32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG))
                {
                    _nt_keydetector_safa_unlock_baseline(electrode, rom, ram, signal);
                }
            }
            break;
        default:
            /*MISRA rule 16.4*/
            break;
    }
}

static void _nt_keydetector_safa_lock_baseline(struct nt_electrode_data *electrode,
                                               const struct nt_keydetector_safa *rom,
                                               struct nt_keydetector_safa_data *ram,
                                               uint16_t signal)
{
    _nt_electrode_set_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG);

    ram->base_avrg_init = rom->non_activity_avrg;
    (void)(int32_t) _nt_filter_moving_average_init(&ram->base_avrg_init, &ram->base_avrg, electrode->baseline);
}

static void _nt_keydetector_safa_unlock_baseline(struct nt_electrode_data *electrode,
                                                 const struct nt_keydetector_safa *rom,
                                                 struct nt_keydetector_safa_data *ram,
                                                 uint16_t signal)
{
    if ((bool)_nt_electrode_get_flag(electrode, (uint32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG))
    {
        uint32_t temp                = rom->deadband_cnt << 3U;
        ram->recovery_cnt            = (int32_t)temp;
        temp                         = (uint32_t)rom->base_avrg.n2_order >> 2U;
        ram->base_avrg_init.n2_order = (int32_t)temp;
        _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_AFTER_INIT_TOUCH_FLAG);
    }
    else
    {
        uint32_t temp                = rom->deadband_cnt << 1;
        ram->recovery_cnt            = (int32_t)temp;
        ram->base_avrg_init.n2_order = rom->base_avrg.n2_order / 2;
    }

    (void)(int32_t) _nt_filter_moving_average_init(&ram->base_avrg_init, &ram->base_avrg, electrode->baseline);

    _nt_electrode_clear_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG);
}

static void _nt_keydetector_safa_signal_track(struct nt_electrode_data *electrode,
                                              const struct nt_keydetector_safa *rom,
                                              struct nt_keydetector_safa_data *ram,
                                              uint16_t signal)
{
    if (ram->entry_event_cnt == 0)
    {
        /* Just use the direction of change and limit it as minimal noise is defined */
        int32_t delta =
            _nt_filter_range_s(((int32_t)signal - (int32_t)electrode->baseline), (rom->min_noise_limit / 2U));
        uint16_t loc_signal = electrode->baseline + (uint16_t)delta;

        electrode->baseline =
            (uint16_t)_nt_filter_moving_average_process(&ram->base_avrg_init, &ram->base_avrg, loc_signal);
    }

    if (!(bool)_nt_electrode_get_flag(electrode, (int32_t)NT_ELECTRODE_LOCK_BASELINE_FLAG))
    {
        if (_nt_filter_abs((int16_t)electrode->baseline - (int16_t)signal) < (ram->noise * rom->signal_to_noise_ratio))
        {
            ram->noise = _nt_filter_moving_average_process(
                &ram->noise_avrg_init, &ram->noise_avrg,
                _nt_filter_limit_u((int32_t)_nt_filter_abs((int16_t)electrode->baseline - (int16_t)signal),
                                   (uint16_t)rom->min_noise_limit,
                                   (uint16_t)(((uint32_t)rom->min_noise_limit * (uint32_t)rom->signal_to_noise_ratio)
                                              << 1U))); /* max noise level step adaptation factor */

            ram->f_noise = _nt_filter_moving_average_process(
                &ram->f_noise_avrg_init, &ram->f_noise_avrg,
                _nt_filter_limit_u(
                    (int32_t)_nt_filter_pos((int32_t)signal - (int32_t)electrode->baseline),
                    (uint16_t)rom->min_noise_limit,
                    (uint16_t)((uint32_t)rom->min_noise_limit * (uint32_t)rom->signal_to_noise_ratio * 4U)));

            /* Noise level recovery */
            if ((ram->noise > (rom->min_noise_limit)) && (ram->noise > (ram->f_noise)))
            {
                /* if fast noise decreases 2x time faster than slowly accumulated value, then recovery slow noise level
                 */
                if ((ram->noise - rom->min_noise_limit) > 2U * (ram->f_noise - rom->min_noise_limit))

                {
                    (void)(int32_t)
                        _nt_filter_moving_average_init(&ram->noise_avrg_init, &ram->noise_avrg, (uint16_t)ram->f_noise);
                }
            }
        }
        /* Deadband auxiliary calculation */
        ram->deadband_h = (electrode->baseline + (ram->noise * rom->signal_to_noise_ratio));
        ram->deadband_l = (electrode->baseline - (ram->noise * rom->signal_to_noise_ratio));
    }

    if (_nt_electrode_get_last_status(electrode) != (int32_t)NT_ELECTRODE_STATE_TOUCH)
    {
        uint16_t sig_filter = signal;

        sig_filter = _nt_filter_abs((int16_t)signal - (int16_t)electrode->baseline);

        if (sig_filter < (ram->noise * rom->signal_to_noise_ratio))
        {
            sig_filter = (uint16_t)(ram->noise * rom->signal_to_noise_ratio);
        }

        sig_filter += electrode->baseline;

        sig_filter = (uint16_t)_nt_filter_limit_u((int32_t)sig_filter, 0, 65535);
        ram->predicted_signal =
            _nt_filter_moving_average_process(&rom->non_activity_avrg, &ram->predicted_signal_avrg, sig_filter);
    }
    else
    {
        ram->predicted_signal =
            _nt_filter_deadrange_u((uint16_t)_nt_filter_moving_average_process(&ram->predicted_signal_avrg_init,
                                                                               &ram->predicted_signal_avrg, signal),
                                   electrode->baseline, (uint16_t)(ram->noise * rom->signal_to_noise_ratio));
    }
}

static void _nt_keydetector_safa_reset(struct nt_electrode_data *electrode)
{
    uint32_t signal = _nt_electrode_get_raw_signal(electrode);
    signal          = _nt_electrode_normalization_process(electrode, signal);

    _nt_reset_keydetector_safa_reset(electrode, signal, 0);
}
