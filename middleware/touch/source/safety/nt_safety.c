/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "../source/system/nt_system_frmstr_prv.h"
#include "nt_modules.h"
#include "../source/modules/nt_modules_prv.h"
#include "nt_keydetectors.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_electrodes.h"
#include "nt_module_gpio.h"

#include "../source/safety/nt_safety_prv.h"
#include "../source/safety/nt_safety_gpio.h"
#include "nt_safety.h"
#include "nt_filters.h"

#include "../source/drivers/tsi/nt_drv_tsi_driver.h"

#include "nt_system.h"

/* Local functions */
static int32_t nt_module_safety_gpio_check_interface(const struct nt_module_data *module);
static int32_t _nt_module_safety_switch_electrodes_gpio(struct nt_module_data *module);
static int32_t _nt_module_safety_switch_electrodes_tsi(struct nt_module_data *module);
int32_t nt_module_safety_check_electrode_short_read(const struct nt_electrode *elec, bool value);
int32_t nt_module_safety_check_electrode_short_gnd_adj_set(const struct nt_electrode *elec);

static int8_t _nt_module_safety_delay(struct nt_module_data *module);
static int32_t nt_module_safety_check_cfg_crc(void);

/** interface module safety */
const struct nt_module_safety_interface nt_safety_interface = {
    .init        = _nt_module_safety_init,
    .process     = _nt_module_safety_process,
    .name        = NT_MODULE_SAFETY_NAME,
    .params_size = sizeof(struct nt_module_safety_gpio_params),
};

/* Init the safety functions for the module. */
int32_t _nt_module_safety_init(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    NT_ASSERT(module->rom->safety_interface != NULL);
    NT_ASSERT(module->rom->safety_interface->process != NULL);
    NT_ASSERT(module->rom->electrodes != NULL);

    if (nt_module_safety_gpio_check_data(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    /* Enable all ports clock for gpios to use safety pin check (only with TSI) */
#if (FSL_FEATURE_TSI_VERSION > 0U)
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
#endif

    if ((module->rom->safety_interface != NULL))
    {
        uint16_t crc_calc = 0;

        struct nt_kernel *system = _nt_system_get();

        (void)(int32_t) nt_safety_calc_cfg_crc(system->rom, &crc_calc);
        system->safety_crc = crc_calc;
    }

    if ((bool)(module->rom->safety_params.gpio->delay_interval))
    {
        module->safety_delay_last_time = nt_system_get_time_counter();
        module->safety_last_test       = 0;
        module->safety_last_adj_test   = 0;
    }
    return (int32_t)NT_SUCCESS;
}

/* Process the safety on the module. */
int32_t _nt_module_safety_process(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);

    /* Check electrodes again short-cut to VDD, GND or adjacent */
    int32_t result_temp = nt_module_safety_check_electrodes(module); /* delay to apply physical changes on pin */

    /* Check if data in flash did not change */
    result_temp = nt_module_safety_check_cfg_crc();

    result_temp = (int32_t)NT_SUCCESS;
    return (int32_t)result_temp;
}

/* Check the safety interface. */
static int32_t nt_module_safety_gpio_check_interface(const struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    NT_ASSERT(module->rom->safety_params.gpio->user_interface != NULL);
    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;
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

/* Check the safety data.*/
int32_t nt_module_safety_gpio_check_data(const struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    if (module->rom->safety_params.gpio->user_interface == NULL)
    {
        return (int32_t)NT_FAILURE; /* Is user interface ? */
    }
    if (nt_module_safety_gpio_check_interface(module) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE; /* Does the user interface support all functions? */
    }
    return (int32_t)NT_SUCCESS;
}

/* Single electrode short to Vdd */
int32_t nt_module_safety_check_electrode_short_vdd_set(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);
    struct nt_module_data *module = nt_electrode_get_module_data(elec);

    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;

    uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;
    uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;

    /* Sets GPIO mode input with pull-Down enabled */
    interface->set_pin_gpio_mode_ptr(port, pin); /* GPIO mode */
    interface->set_pin_input_ptr(port, pin);     /* GPIO input */
    interface->set_pin_pull_down_ptr(port, pin); /* pull-down */

    if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
    {
        pin  = elec->gpio_input.gpio2 & 0xFFU;
        port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT;

        interface->set_pin_gpio_mode_ptr(port, pin); /* GPIO mode */
        interface->set_pin_input_ptr(port, pin);     /* GPIO input */
        interface->set_pin_pull_down_ptr(port, pin); /* pull-up */
    }

    return (int32_t)NT_SUCCESS;
}

/* Single electrode short to GND */
int32_t nt_module_safety_check_electrode_short_gnd_adj_set(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);
    struct nt_module_data *module = nt_electrode_get_module_data(elec);

    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;

    uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;
    uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;

    /* Sets GPIO mode input with pull-UP enabled */
    interface->set_pin_gpio_mode_ptr(port, pin); /* GPIO mode */
    interface->set_pin_input_ptr(port, pin);     /* GPIO input */
    interface->set_pin_pull_up_ptr(port, pin);   /* pull-up */

    if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
    {
        pin  = elec->gpio_input.gpio2 & 0xFFU;
        port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT;

        interface->set_pin_gpio_mode_ptr(port, pin); /* GPIO mode */
        interface->set_pin_input_ptr(port, pin);     /* GPIO input */
        interface->set_pin_pull_up_ptr(port, pin);   /* pull-up */
    }

    return (int32_t)NT_SUCCESS;
}

/* Single electrode short read */
int32_t nt_module_safety_check_electrode_short_read(const struct nt_electrode *elec, bool value)
{
    NT_ASSERT(elec != NULL);
    uint32_t res;
    struct nt_module_data *module = nt_electrode_get_module_data(elec);

    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;

    uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;
    uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT;

    res = interface->get_pin_value_ptr(port, pin); /* Read value back from the port/pin */

    if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
    {
        pin  = elec->gpio_input.gpio2 & 0xFFU;
        port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT;

        interface->set_pin_gpio_mode_ptr(port, pin); /* GPIO mode */
        interface->set_pin_input_ptr(port, pin);     /* GPIO input */
        interface->set_pin_pull_up_ptr(port, pin);   /* pull-up */

        res = interface->get_pin_value_ptr(port, pin); /* Read value back from the port/pin */
    }

    if ((bool)res == value) /* Detect failure if defined value is red */
    {
        return (int32_t)NT_FAILURE;
    }

    return (int32_t)NT_SUCCESS;
}

/* Check electrode baseline too low level */
int32_t nt_module_safety_check_baseline_low(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);
    if (elec->baseline_level == 0U) /* Baseline test only if baseline level was assigned */
    {
        return (int32_t)NT_NOT_SUPPORTED;
    }

    struct nt_electrode_data *electrode_data = nt_electrode_get_data(elec);
    int32_t s_delta                          = ((int32_t)electrode_data->baseline - (int32_t)elec->baseline_level);

    if (s_delta < 0) /* for too low baseline level must be negative delta */
    {
        s_delta = -s_delta; /* make positive value */
        if (((uint32_t)s_delta << 3U) >
            (uint32_t)elec->baseline_level) /* baseline neg. change is more than 12.5% of normal baseline level */
        {
            return (int32_t)NT_FAILURE;
        }
    }
    return (int32_t)NT_SUCCESS;
}

/* Check electrode baseline too high level */
int32_t nt_module_safety_check_baseline_high(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);
    if (elec->baseline_level == 0U) /* Baseline test only if baseline level was assigned */
    {
        return (int32_t)NT_NOT_SUPPORTED;
    }

    struct nt_electrode_data *electrode_data = nt_electrode_get_data(elec);
    int32_t s_delta                          = ((int32_t)electrode_data->baseline - (int32_t)elec->baseline_level);

    if (s_delta > 0) /* for too high baseline level must be positive delta */
    {
        if (((uint32_t)s_delta << 2U) >
            (uint32_t)elec->baseline_level) /* baseline positive change is more than 25% of normal baseline level */
        {
            return (int32_t)NT_FAILURE;
        }
    }
    return (int32_t)NT_SUCCESS;
}

int32_t nt_module_safety_check_electrodes(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);

    uint32_t el_counter       = module->electrodes_cnt;
    static uint32_t fault_cnt = 0;
    union nt_system_event_context context;

    /* Check if the GPIO methods are available, if unavailable skip and go out */
    if (nt_module_safety_gpio_check_data(module) != (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }

    /*** Execute baseline level tests ***/
    el_counter = module->electrodes_cnt; /* Get number of electrodes */
    while ((bool)(el_counter--))         /* The main test loop */
    {
        const struct nt_electrode *elec = module->electrodes[el_counter]->rom;
        /* Check baseline is too low */
        if (nt_module_safety_check_baseline_low(elec) ==
            (int32_t)NT_FAILURE) /* Test each defined electrode to low limit baseline level */
        {
            fault_cnt++;                                                                /* Increment fault counter */
            context.sig_out_of_limit.electrode = elec;                                  /* Store electrode not passed */
            _nt_system_invoke_callback((uint32_t)NT_SYSTEM_EVENT_SIGNAL_LOW, &context); /* Call callback */
        }

        /* Check baseline is too high */
        if (nt_module_safety_check_baseline_high(elec) ==
            (int32_t)NT_FAILURE) /* Test each defined electrode to high limit baseline level */
        {
            fault_cnt++;                               /* Increment fault counter */
            context.sig_out_of_limit.electrode = elec; /* Store electrode not passed */
            _nt_system_invoke_callback((uint32_t)NT_SYSTEM_EVENT_SIGNAL_HIGH, &context);
        }
    }

    /*** Execute short to GND tests ***/
    if (module->safety_last_test == 0U) /* Set short to GND tests */
    {
        (void)(int32_t)
            _nt_module_safety_switch_electrodes_gpio(module); /* Switch all pins to GPIO because GPIO tests start  */
        el_counter = module->electrodes_cnt;                  /* Get number of electrodes */
        while ((bool)(el_counter--))                          /* The main test loop */
        {
            const struct nt_electrode *elec = module->electrodes[el_counter]->rom;
            (void)(int32_t) nt_module_safety_check_electrode_short_gnd_adj_set(
                elec); /* Set each assigned electrode to GPIO input with pull-up */
        }
        module->safety_last_test++; /* Increment test counter to recognize that GND tests is activated */
    }
    if (module->safety_last_test == 1U) /* If GND tests is running check the safety delay overflow */
    {
        if (_nt_module_safety_delay(module) ==
            (int32_t)NT_SUCCESS) /* After applying delay the assigned electrode will be sensed */
        {
            el_counter = module->electrodes_cnt; /* Get number of electrodes */
            while ((bool)(el_counter--))         /* The main test loop */
            {
                const struct nt_electrode *elec = module->electrodes[el_counter]->rom; /* Get electrode to test */
                if (nt_module_safety_check_electrode_short_read(elec, false) ==
                    (int32_t)NT_FAILURE) /* Test each assigned electrode to GPIO input with pull-up */
                {
                    fault_cnt++;                               /* Increment fault counter */
                    context.sig_out_of_limit.electrode = elec; /* Store electrode not passed */
                    _nt_system_invoke_callback((int32_t)NT_SYSTEM_EVENT_ELEC_SHORT_GND, &context); /* Call callback */
                }
            }
            module->safety_last_test++; /* Increment test counter to recognize that GND tests was executed */
        }
        else
        {
            return (int32_t)NT_SCAN_IN_PROGRESS; /* Delay interval was not achieved - go out */
        }
    }

    /*** Execute short to VDD tests ***/
    if (module->safety_last_test == 2U) /* Set short to VDD tests */
    {
        el_counter = module->electrodes_cnt; /* Get number of electrodes */
        while ((bool)(el_counter--))         /* The main test loop */
        {
            const struct nt_electrode *elec = module->electrodes[el_counter]->rom;
            (void)(int32_t) nt_module_safety_check_electrode_short_vdd_set(
                elec); /* Set each assigned electrode to GPIO input with pull-down */
        }
        module->safety_last_test++; /* Increment test counter to recognize that VDD tests is activated */
    }
    if (module->safety_last_test == 3U) /* If VDD tests is running check the safety delay overflow */
    {
        if (_nt_module_safety_delay(module) ==
            (int32_t)NT_SUCCESS) /* After applying delay the assigned electrode will be sensed */
        {
            el_counter = module->electrodes_cnt; /* Get number of electrodes */
            while ((bool)(el_counter--))         /* The main test loop */
            {
                const struct nt_electrode *elec = module->electrodes[el_counter]->rom; /* Get electrode to test */
                if (nt_module_safety_check_electrode_short_read(elec, true) ==
                    (int32_t)NT_FAILURE) /* Test each assigned electrode to GPIO input */
                {
                    fault_cnt++;                               /* Increment fault counter */
                    context.sig_out_of_limit.electrode = elec; /* Store electrode not passed */
                    _nt_system_invoke_callback((int32_t)NT_SYSTEM_EVENT_ELEC_SHORT_VDD, &context); /* Call callback */
                }
            }
            module->safety_last_test++; /* Increment test counter to recognize that VDD tests was executed */
        }
        else
        {
            return (int32_t)NT_SCAN_IN_PROGRESS; /* Delay interval was not achieved - go out */
        }
    }

    /*** Execute short to ADJ tests ***/
    if (module->safety_last_test >= 4U) /* Set short to ADJ tests */
    {
        if (module->safety_last_adj_test ==
            module->safety_last_test - 4U) /* Set short to ADJ test for assigned electrode */
        {
            /* Set the first electrode for ADJ test */
            const struct nt_electrode *elec = module->electrodes[module->safety_last_adj_test]->rom;
            do /* main set test ADJ electrode loop */
            {
                elec = module->electrodes[module->safety_last_adj_test]->rom; /* Select elect to ADJ test set */
                if (nt_module_safety_check_electrode_short_gnd_adj_set(elec) ==
                    (int32_t)NT_NOT_SUPPORTED) /* Set electrode for ADJ test */
                {
                    module->safety_last_adj_test++; /* Avoid waiting for reading electrode if not assigned */
                }
                module->safety_last_test++; /* Increment test counter to recognize that ADJ test is activated */
                if (module->safety_last_test >=
                    module->electrodes_cnt + 4U) /* Reactivate pin test if last electrode achieved */
                {
                    module->safety_last_adj_test = 0; /* Reactivate test counter  */
                    module->safety_last_test     = 0; /* Reactivate auxiliary ADJ counter */
                    (void)(int32_t) _nt_module_safety_switch_electrodes_tsi(
                        module); /* Switch all pins to TSI if no next electrode to test */
                    return (int32_t)NT_SUCCESS;
                }
            } while (nt_module_safety_check_electrode_short_gnd_adj_set(elec) ==
                     (int32_t)NT_NOT_SUPPORTED); /* repeat main ADJ loop if assigned electrode exists */
        }
        if (module->safety_last_adj_test ==
            module->safety_last_test - 5U) /* If ADJ test is running check the safety delay overflow */
        {
            if (_nt_module_safety_delay(module) ==
                (int32_t)NT_SUCCESS) /* After applying delay the assigned electrode will be sensed */
            {
                el_counter = module->electrodes_cnt; /* Get number of electrodes */
                while ((bool)(el_counter--))         /* The main test loop */
                {
                    const struct nt_electrode *elec = module->electrodes[el_counter]->rom; /* Get electrode to test */
                    if (nt_module_safety_check_electrode_short_read(elec, true) ==
                        (int32_t)NT_FAILURE) /* Test each assigned electrode to GPIO input */
                    {
                        /* Skipped hypothetical faults when same electrode is set and read, or swapped short electrode
                         * combination */
                        if (el_counter > (module->safety_last_adj_test))
                        {
                            fault_cnt++;                               /* Increment fault counter */
                            context.sig_out_of_limit.electrode = elec; /* Store electrode not passed */
                            _nt_system_invoke_callback((int32_t)NT_SYSTEM_EVENT_ELEC_SHORT_ADJ,
                                                       &context); /* Call callback */
                        }
                    }
                }
                module->safety_last_adj_test++;
                /* restart test counter for next measurement */
                if (module->safety_last_test >=
                    module->electrodes_cnt + 4U) /* Reactivate pin test if last electrode achieved */
                {
                    module->safety_last_adj_test = 0U; /* Reactivate test counter  */
                    module->safety_last_test     = 0U; /* Reactivate auxiliary ADJ counter */
                    (void)(int32_t) _nt_module_safety_switch_electrodes_tsi(
                        module); /* Switch all pins to TSI if no next electrode to test */
                    return (int32_t)NT_SUCCESS;
                }
            }
            else
            {
                return (int32_t)NT_SCAN_IN_PROGRESS; /* Delay interval was not achieved - go out */
            }
        }
    }

    /* Checked detected faults and return the test result */
    if (fault_cnt == 0U)
    {
        return (int32_t)NT_SUCCESS;
    }
    else
    {
        return (int32_t)NT_FAILURE;
    }
}

/* Change all pins to GPIO mode */
static int32_t _nt_module_safety_switch_electrodes_gpio(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    uint32_t el_counter = module->electrodes_cnt;

    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;

    while ((bool)(el_counter--))
    {
        struct nt_electrode_data *electrode_data = module->electrodes[el_counter];
        const struct nt_electrode *elec          = electrode_data->rom;

        uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;               /* Select pin  */
        uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
        interface->set_pin_gpio_mode_ptr(port, pin);                  /* Set pinmux to GPIO mode */

        if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
        {
            pin  = elec->gpio_input.gpio2 & 0xFFU;               /* Select pin  */
            port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT; /* Select port */
            interface->set_pin_gpio_mode_ptr(port, pin);         /* Set pinmux to GPIO mode */
        }
    }

    return (int32_t)NT_SUCCESS;
}

int32_t _nt_module_safety_switch_electrode_gpio(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);

    uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;               /* Select pin  */
    uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
    set_pin_gpio_mode(port, pin);                                 /* Set pinmux to GPIO mode */

    if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
    {
        pin  = elec->gpio_input.gpio2 & 0xFFU;               /* Select pin  */
        port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT; /* Select port */
        set_pin_gpio_mode(port, pin);                        /* Set pinmux to GPIO mode */
    }

    return (int32_t)NT_SUCCESS;
}

/* Change all pins to peripheral mode as TSI electrode */
static int32_t _nt_module_safety_switch_electrodes_tsi(struct nt_module_data *module)
{
    NT_ASSERT(module != NULL);
    uint32_t el_counter = module->electrodes_cnt;

    const struct nt_module_gpio_user_interface *interface = module->rom->safety_params.gpio->user_interface;

    while ((bool)(el_counter--))
    {
        struct nt_electrode_data *electrode_data = module->electrodes[el_counter];
        const struct nt_electrode *elec          = electrode_data->rom;

        if (elec->gpio_input.gpio1 == 0U)
        {
            continue; /* if electrode is not defined set next defined electrode to TSI mode */
        }
        uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;               /* Select pin  */
        uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
        interface->set_pin_tsi_mode_ptr(port, pin);                   /* Set pinmux to TSI peripheral mode */

        if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
        {
            pin  = elec->gpio_input.gpio2 & 0xFFU;               /* Select pin  */
            port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT; /* Select port */
            interface->set_pin_tsi_mode_ptr(port, pin);          /* Set pinmux to TSI peripheral mode */
        }
    }
    return (int32_t)NT_SUCCESS;
}

int32_t _nt_module_safety_switch_electrode_tsi(const struct nt_electrode *elec)
{
    NT_ASSERT(elec != NULL);

    uint32_t pin  = elec->gpio_input.gpio1 & 0xFFU;               /* Select pin  */
    uint32_t port = elec->gpio_input.gpio1 >> NT_GPIO_PORT_SHIFT; /* Select port */
    set_pin_tsi_mode(port, pin);                                  /* Set pinmux to TSI peripheral mode */

    if (elec->gpio_input.gpio2 != 0U) /* For 2nd GPIO if defined (mutual mode) */
    {
        pin  = elec->gpio_input.gpio2 & 0xFFU;               /* Select pin  */
        port = elec->gpio_input.gpio2 >> NT_GPIO_PORT_SHIFT; /* Select port */
        set_pin_tsi_mode(port, pin);                         /* Set pinmux to TSI peripheral mode */
    }

    return (int32_t)NT_SUCCESS;
}

static int8_t _nt_module_safety_delay(struct nt_module_data *module)
{
    /* Safety process triggering based on NT system triggering.
    If safety period equal to 0 safety process function will be executed */
    uint32_t safety_delay_period = module->rom->safety_params.gpio->delay_interval;
    uint32_t syst_time           = nt_system_get_time_counter();

    /* safety delay time detection by compare system time with calculated trigger */
    if (syst_time > (module->safety_delay_last_time + safety_delay_period))
    {
        module->safety_delay_last_time = syst_time;
        return (int8_t)NT_SUCCESS;
    }
    else
    {
        return (int8_t)NT_SCAN_IN_PROGRESS;
    }
}

/* Check CRC data */
static int32_t nt_module_safety_check_cfg_crc(void)
{
    /* Get main system structure */
    struct nt_kernel *system = _nt_system_get();

    /* CRC compare of the structure store in ROM with stored CRC value to detect data changed failure */
    if (nt_safety_check_cfg_data(system->rom, system->safety_crc) != (int32_t)NT_SUCCESS)
    {
        _nt_system_invoke_callback((uint32_t)NT_SYSTEM_EVENT_CRC_FAILED, NULL); /* Call callback */
        return (int32_t)NT_FAILURE;
    }
    return (int32_t)NT_SUCCESS;
}
