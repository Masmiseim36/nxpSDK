/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 * Copyright 2019-2020 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "cmsis.h"
#include "tfm_spm_hal.h"
#include "tfm_platform_core_api.h"
#include "target_cfg.h"
#include "mpu_armv8m_drv.h"
#include "region_defs.h"
#include "utilities.h"
#include "region.h"

#include "Driver_Common.h"

#include "log/tfm_log.h"

/* Get address of memory regions to configure MPU */
extern const struct memory_region_limits memory_regions;

struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
#define MPU_REGION_VENEERS              0
#define MPU_REGION_TFM_UNPRIV_CODE      1
#define MPU_REGION_TFM_UNPRIV_DATA      2
#define MPU_REGION_NS_STACK             3
#define PARTITION_REGION_RO             4
#define PARTITION_REGION_RW_STACK       5
#define PARTITION_REGION_PERIPH_START   6
#define PARTITION_REGION_PERIPH_MAX_NUM 2

#define PARTITION_NS_DATA               7   //NXP //DM

uint32_t periph_num_count = 0;
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

enum tfm_plat_err_t tfm_spm_hal_init_isolation_hw(void)
{
    int32_t ret = ARM_DRIVER_OK;
    /* Configures non-secure memory spaces in the target */
    sau_and_idau_cfg();
    ret = mpc_init_cfg();
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    ret = ppc_init_cfg();
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_spm_hal_configure_default_isolation(
                  uint32_t partition_idx,
                  const struct tfm_spm_partition_platform_data_t *platform_data)
{
    bool privileged = tfm_is_partition_privileged(partition_idx);
#if defined(CONFIG_TFM_ENABLE_MEMORY_PROTECT) && (TFM_LVL != 1)
    struct mpu_armv8m_region_cfg_t region_cfg;
#endif

    if (!platform_data) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

#if defined(CONFIG_TFM_ENABLE_MEMORY_PROTECT) && (TFM_LVL != 1)
    if (!privileged) {
        region_cfg.region_nr = PARTITION_REGION_PERIPH_START +
                                periph_num_count;
        periph_num_count++;
        if (periph_num_count >= PARTITION_REGION_PERIPH_MAX_NUM) {
            return TFM_PLAT_ERR_MAX_VALUE;
        }
        region_cfg.region_base = platform_data->periph_start;
        region_cfg.region_limit = platform_data->periph_limit;
        region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
        region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
        region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
        region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;

        mpu_armv8m_disable(&dev_mpu_s);

        if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg)
            != MPU_ARMV8M_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
        mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                          HARDFAULT_NMI_ENABLE);
    }
#endif /* defined(CONFIG_TFM_ENABLE_MEMORY_PROTECT) && (TFM_LVL != 1) */
    if (platform_data->periph_ppc_bank != 0 /* PPC_SP_DO_NOT_CONFIGURE */) {
        ppc_configure_to_secure(platform_data->periph_ppc_bank,
                                platform_data->periph_ppc_loc, privileged);

    }
    return TFM_PLAT_ERR_SUCCESS;
}

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_DATA, $$RW$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_DATA, $$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);

static enum tfm_plat_err_t tfm_spm_mpu_init(void)
{
    struct mpu_armv8m_region_cfg_t region_cfg;

    mpu_armv8m_clean(&dev_mpu_s);

    /* Veneer region */
    region_cfg.region_nr = MPU_REGION_VENEERS;
    region_cfg.region_base = memory_regions.veneer_base;
    region_cfg.region_limit = memory_regions.veneer_limit;
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("=== [MPU Unpriviladged] =======\r\n");
    LOG_MSG("Veneers = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit);
#endif

    /* TFM Core unprivileged code region */
    region_cfg.region_nr = MPU_REGION_TFM_UNPRIV_CODE;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("Code = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit);    
#endif

    /* TFM Core unprivileged data region */
    region_cfg.region_nr = MPU_REGION_TFM_UNPRIV_DATA;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_DATA, $$RW$$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_DATA, $$ZI$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("Data = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit); 
#endif
    
    /* NSPM PSP */
    region_cfg.region_nr = MPU_REGION_NS_STACK;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("NS STACK = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit); 
#endif

    /* RO region */
    region_cfg.region_nr = PARTITION_REGION_RO;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("RO APP CODE = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit); 
#endif

    /* RW, ZI and stack as one region */
    region_cfg.region_nr = PARTITION_REGION_RW_STACK;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("RW,ZI APP = [0x%x, 0x%x] \r\n", region_cfg.region_base, region_cfg.region_limit); 
    LOG_MSG("===============================\r\n");
#endif

    /* NS Data, mark as nonpriviladged */
    region_cfg.region_nr = PARTITION_NS_DATA;
    region_cfg.region_base = NS_DATA_START;
    region_cfg.region_limit = NS_DATA_LIMIT;
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#if TARGET_DEBUG_LOG
    LOG_MSG("NS Data = [0x%x, 0x%x] \r\n", NS_DATA_START, NS_DATA_LIMIT); 
    LOG_MSG("===============================\r\n");
#endif
    
    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                      HARDFAULT_NMI_ENABLE);

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_spm_hal_setup_isolation_hw(void)
{
    if (tfm_spm_mpu_init() != TFM_PLAT_ERR_SUCCESS) {
        ERROR_MSG("Failed to set up initial MPU configuration! Halting.");
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

void SEC_VIO_IRQHandler(void)
{
	  /* Clear interrupt flag and pending IRQ */
    NVIC_ClearPendingIRQ(SEC_VIO_IRQn);

    /* Print fault message and block execution */
    ERROR_MSG("Oops... MPC fault!!!");

    /* Inform TF-M core that isolation boundary has been violated */
    tfm_access_violation_handler();
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

enum tfm_plat_err_t tfm_spm_hal_set_secure_irq_priority(IRQn_Type irq_line,
                                                        uint32_t priority)
{
    uint32_t quantized_priority = priority >> (8U - __NVIC_PRIO_BITS);
    NVIC_SetPriority(irq_line, quantized_priority);
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

enum tfm_plat_err_t tfm_spm_hal_enable_fault_handlers(void)
{
    return enable_fault_handlers();
}

enum tfm_plat_err_t tfm_spm_hal_system_reset_cfg(void)
{
    return system_reset_cfg();
}

enum tfm_plat_err_t tfm_spm_hal_init_debug(void)
{
    return init_debug();
}

enum tfm_plat_err_t tfm_spm_hal_nvic_interrupt_target_state_cfg(void)
{
    return nvic_interrupt_target_state_cfg();
}

enum tfm_plat_err_t tfm_spm_hal_nvic_interrupt_enable(void)
{
    return nvic_interrupt_enable();
}
