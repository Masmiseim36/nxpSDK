/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright 2019-2020 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "tfm_spm_hal.h"
#include "tfm_platform_core_api.h"
#include "target_cfg.h"
#include "utilities.h"
#include "exception_info.h"

#include "Driver_Common.h"

/* Get address of memory regions to configure MPU */
extern const struct memory_region_limits memory_regions;


void C_SEC_VIO_IRQHandler(void)
{
	/* Clear interrupt flag and pending IRQ */
    NVIC_ClearPendingIRQ(SEC_VIO_IRQn);

    /* Print fault message and block execution */
    ERROR_MSG("Platform Exception: MPC fault!!!");

    /* Inform TF-M core that isolation boundary has been violated */
    tfm_access_violation_handler();
}

__attribute__((naked)) void SEC_VIO_IRQHandler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_PLATFORM);

    __ASM volatile(
        "BL        C_SEC_VIO_IRQHandler    \n"
        "B         .                       \n"
    );
}

uint32_t tfm_spm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_spm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_spm_hal_get_ns_entry_point(void)
{
    return *((uint32_t *)(memory_regions.non_secure_code_start+ 4));
}

enum tfm_plat_err_t tfm_spm_hal_set_secure_irq_priority(IRQn_Type irq_line)
{
    NVIC_SetPriority(irq_line, DEFAULT_IRQ_PRIORITY);
    return TFM_PLAT_ERR_SUCCESS;
}

void tfm_spm_hal_clear_pending_irq(IRQn_Type irq_line)
{
    NVIC_ClearPendingIRQ(irq_line);
}

void tfm_spm_hal_enable_irq(IRQn_Type irq_line)
{
    NVIC_EnableIRQ(irq_line);
}

void tfm_spm_hal_disable_irq(IRQn_Type irq_line)
{
    NVIC_DisableIRQ(irq_line);
}

enum irq_target_state_t tfm_spm_hal_set_irq_target_state(
                                           IRQn_Type irq_line,
                                           enum irq_target_state_t target_state)
{
    uint32_t result;

    if (target_state == TFM_IRQ_TARGET_STATE_SECURE) {
        result = NVIC_ClearTargetState(irq_line);
    } else {
        result = NVIC_SetTargetState(irq_line);
    }

    if (result) {
        return TFM_IRQ_TARGET_STATE_NON_SECURE;
    } else {
        return TFM_IRQ_TARGET_STATE_SECURE;
    }
}

#ifndef TFM_PSA_API
enum tfm_plat_err_t tfm_spm_hal_configure_default_isolation(
                  bool privileged,
                  const struct platform_data_t *platform_data)
{
    if (!platform_data) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }
    ppc_configure_to_secure(platform_data->periph_ppc_bank,
                                platform_data->periph_ppc_loc, privileged);
    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* TFM_PSA_API */