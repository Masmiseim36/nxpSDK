/*
 * Copyright (c) 2018-2020 Arm Limited
 * Copyright 2019-2023 NXP. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_COMMON_H__
#define __TARGET_CFG_COMMON_H__

#include "uart_stdout.h"
#include "tfm_peripherals_def.h"
#if defined(TFM_LPUART_FEATRUE)
#include "fsl_lpuart.h"
#else
#include "fsl_usart.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TARGET_DEBUG_LOG
#define TARGET_DEBUG_LOG 0
#endif

/**
 * \brief Store the addresses of memory regions
 */
struct memory_region_limits {
    uint32_t non_secure_code_start;
    uint32_t non_secure_partition_base;
    uint32_t non_secure_partition_limit;
    uint32_t veneer_base;
    uint32_t veneer_limit;
};

/**
 * \brief Configures the Memory Protection Controller.
 *
 * \return  Returns error code.
 */
int32_t mpc_init_cfg(void);

/**
 * \brief Configures the Peripheral Protection Controller.
 *
 * \return  Returns error code.
 */
int32_t ppc_init_cfg(void);

/**
 * \brief Restict access to peripheral to secure
 */
void ppc_configure_to_secure(struct platform_data_t *platform_data, bool privileged);

/**
 * \brief Configures SAU and IDAU.
 */
void sau_and_idau_cfg(void);

/**
 * \brief Enables the fault handlers and sets priorities.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t enable_fault_handlers(void);

/**
 * \brief Configures the system reset request properties
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t system_reset_cfg(void);

/**
 * \brief Configures the system debug properties.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t init_debug(void);

/**
 * \brief Configures all external interrupts to target the
 *        NS state, apart for the ones associated to secure
 *        peripherals (plus MPC and PPC)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void);

/**
 * \brief This function enable the interrupts associated
 *        to the secure peripherals (plus the isolation boundary violation
 *        interrupts)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_enable(void);

#ifdef __cplusplus
}
#endif

#endif /* __TARGET_CFG_COMMON_H__ */
