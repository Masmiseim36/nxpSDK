/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 * Copyright 2020-2022 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include <stddef.h>
#include <stdint.h>
#include "array.h"
#include "cmsis.h"
#include "Driver_Common.h"
#include "mmio_defs.h"
#include "mpu_armv8m_drv.h"
#include "region.h"
#include "target_cfg.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_isolation.h"
#include "region_defs.h" //NXP
#include "tfm_peripherals_def.h"
#include "fih.h"
#if TFM_PSA_API
#include "load/partition_defs.h"
#include "load/asset_defs.h"
#include "load/spm_load_api.h"
#endif /* TFM_PSA_API */

/* It can be retrieved from the MPU_TYPE register. */
#define MPU_REGION_NUM                  8

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
static uint32_t n_configured_regions = 0;
struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
#if TFM_LVL == 3
static uint32_t idx_boundary_handle = 0;
REGION_DECLARE(Image$$, PT_RO_START, $$Base);
REGION_DECLARE(Image$$, PT_RO_END, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_END, $$Base);

static struct mpu_armv8m_region_cfg_t isolation_regions[] = {
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_RO_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_RO_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE,
    },
    /* For isolation Level 3, set up static isolation for privileged data.
     * Unprivileged data is dynamically set during Partition scheduling.
     */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_ONLY,
        MPU_ARMV8M_SH_NONE,
    },
};
#else /* TFM_LVL == 3 */

REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
#ifdef CONFIG_TFM_PARTITION_META
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
#endif

#endif /* TFM_LVL == 3 */
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

extern void BOARD_InitHardware(void); //NXP

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_set_up_static_boundaries(void)
{

    BOARD_InitHardware(); //NXP

#ifdef NS_QSPI_FLASH /* For LPC55s36-EVK */
	#include "Driver_Flash.h"
    extern ARM_DRIVER_FLASH Driver_QSPI_FLASH0;
    Driver_QSPI_FLASH0.Initialize(NULL);
#endif

#if TARGET_DEBUG_LOG //NXP
    stdio_init(); /* To enable USART */
#endif

    /* Set up isolation boundaries between SPE and NSPE */
    sau_and_idau_cfg();

    if (mpc_init_cfg() != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    if (ppc_init_cfg() != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Set up static isolation boundaries inside SPE */
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

    mpu_armv8m_clean(&dev_mpu_s);
#if TFM_LVL == 3
    int32_t i;
    /*
     * Update MPU region numbers. The numbers start from 0 and are continuous.
     * Under isolation level3, at lease one MPU region is reserved for private
     * data asset.
     */
    if (ARRAY_SIZE(isolation_regions) >= MPU_REGION_NUM) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    for (i = 0; i < ARRAY_SIZE(isolation_regions); i++) {
        /* Update region number */
        isolation_regions[i].region_nr = i;
        /* Enable regions */
        if (mpu_armv8m_region_enable(&dev_mpu_s, &isolation_regions[i])
                                                             != MPU_ARMV8M_OK) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
    n_configured_regions = i;

#else /* TFM_LVL == 3 */
    struct mpu_armv8m_region_cfg_t region_cfg;

    /* Veneer region */
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base = (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#if TARGET_DEBUG_LOG //NXP
    SPMLOG_DBGMSGVAL("Veneers starts from : ", region_cfg.region_base);
    SPMLOG_DBGMSGVAL("Veneers ends at : ", region_cfg.region_base +
                                           region_cfg.region_limit);
#endif

    /* TFM Core unprivileged code region */
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#if TARGET_DEBUG_LOG //NXP
    SPMLOG_DBGMSGVAL("Code section starts from : ", region_cfg.region_base);
    SPMLOG_DBGMSGVAL("Code section ends at : ", region_cfg.region_base +
                                                region_cfg.region_limit);
#endif

    /* RO region */
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#if TARGET_DEBUG_LOG //NXP
    SPMLOG_DBGMSGVAL("RO APP CODE starts from : ", region_cfg.region_base);
    SPMLOG_DBGMSGVAL("RO APP CODE ends at : ", region_cfg.region_base +
                                               region_cfg.region_limit);
#endif

    /* RW, ZI and stack as one region */
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base);
    region_cfg.region_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#if TARGET_DEBUG_LOG //NXP
    SPMLOG_DBGMSGVAL("RW, ZI APP starts from : ", region_cfg.region_base);
    SPMLOG_DBGMSGVAL("RW, ZI APP ends at : ", region_cfg.region_base +
                                              region_cfg.region_limit);
#endif

    /* NS Data, mark as nonpriviladged */ //NXP
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base = NS_DATA_START;
    region_cfg.region_limit = NS_DATA_LIMIT;
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSGVAL("NS Data starts from : ", region_cfg.region_base);
    SPMLOG_DBGMSGVAL("NS Data ends at : ", region_cfg.region_base +
                                           region_cfg.region_limit);
#endif

#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    region_cfg.region_nr = n_configured_regions;
    region_cfg.region_base =
     (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
    region_cfg.region_limit =
     (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
    region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    n_configured_regions++;

#endif /* CONFIG_TFM_PARTITION_META */
#endif /* TFM_LVL == 3 */

    /* Enable MPU */
    if (mpu_armv8m_enable(&dev_mpu_s,
                          PRIVILEGED_DEFAULT_ENABLE,
                          HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
#endif

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

#ifdef TFM_PSA_API
/*
 * Implementation of tfm_hal_bind_boundary():
 *
 * The API encodes some attributes into a handle and returns it to SPM.
 * The attributes include isolation boundaries, privilege, and MMIO information.
 * When scheduler switches running partitions, SPM compares the handle between
 * partitions to know if boundary update is necessary. If update is required,
 * SPM passes the handle to platform to do platform settings and update
 * isolation boundaries.
 *
 * The handle should be unique under isolation level 3. The implementation
 * encodes an index at the highest 8 bits to assure handle uniqueness. While
 * under isolation level 1/2, handles may not be unique.
 *
 * The encoding format assignment:
 * - For isolation level 3
 *      BIT | 31        24 | 23         20 | ... | 7           4 | 3       0 |
 *          | Unique Index | Region Attr 5 | ... | Region Attr 1 | Base Attr |
 *
 *      In which the "Region Attr i" is:
 *      BIT |       3      | 2        0 |
 *          | 1: RW, 0: RO | MMIO Index |
 *
 *      In which the "Base Attr" is:
 *      BIT |               1                |                           0                     |
 *          | 1: privileged, 0: unprivileged | 1: Trustzone-specific NSPE, 0: Secure partition |
 *
 * - For isolation level 1/2
 *      BIT | 31     2 |              1                |                           0                     |
 *          | Reserved |1: privileged, 0: unprivileged | 1: Trustzone-specific NSPE, 0: Secure partition |
 *
 * This is a reference implementation, and may have some
 * limitations.
 * 1. The maximum number of allowed MMIO regions is 5.
 * 2. Highest 8 bits are for index. It supports 256 unique handles at most.
 */
FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_bind_boundary(
                                    const struct partition_load_info_t *p_ldinf,
                                    uintptr_t *p_boundary)
{
    uint32_t i, j;
    bool privileged;
    bool ns_agent;
    uint32_t partition_attrs = 0;
    const struct asset_desc_t *p_asset;
    struct platform_data_t *plat_data_ptr;
#if TFM_LVL == 2
    struct mpu_armv8m_region_cfg_t localcfg;
#endif

    if (!p_ldinf || !p_boundary) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    ns_agent = IS_PARTITION_NS_AGENT(p_ldinf);
    p_asset = (const struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);
    /*
     * Validate if the named MMIO of partition is allowed by the platform.
     * Otherwise, skip validation.
     *
     * NOTE: Need to add validation of numbered MMIO if platform requires.
     */
    for (i = 0; i < p_ldinf->nassets; i++) {
        if (!(p_asset[i].attr & ASSET_ATTR_NAMED_MMIO)) {
            continue;
        }
        for (j = 0; j < ARRAY_SIZE(partition_named_mmio_list); j++) {
            if (p_asset[i].dev.dev_ref == partition_named_mmio_list[j]) {
                break;
            }
        }
        if (j == ARRAY_SIZE(partition_named_mmio_list)) {
            /* The MMIO asset is not in the allowed list of platform. */
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
        /* Assume PPC & MPC settings are required even under level 1 */
        plat_data_ptr = REFERENCE_TO_PTR(p_asset[i].dev.dev_ref,
                                         struct platform_data_t *);
        ppc_configure_to_secure(plat_data_ptr->periph_ppc_bank,
                                plat_data_ptr->periph_ppc_loc, privileged);
#if TFM_LVL == 2
        /*
            * Static boundaries are set. Set up MPU region for MMIO.
            * Setup regions for unprivileged assets only.
            */
        if (!privileged) {
            localcfg.region_base = plat_data_ptr->periph_start;
            localcfg.region_limit = plat_data_ptr->periph_limit;
            localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
            localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
            localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
            localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
            localcfg.region_nr = n_configured_regions++;
            if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg)
                != MPU_ARMV8M_OK) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
        }
#elif TFM_LVL == 3
        /* Encode MMIO attributes into the "partition_attrs". */
        partition_attrs <<= HANDLE_PER_ATTR_BITS;
        partition_attrs |= ((j + 1) & HANDLE_ATTR_INDEX_MASK);
        if (p_asset[i].attr & ASSET_ATTR_READ_WRITE) {
            partition_attrs |= HANDLE_ATTR_RW_POS;
        }
#endif
    }

#if TFM_LVL == 3
    partition_attrs <<= HANDLE_PER_ATTR_BITS;
    /*
     * Highest 8 bits are reserved for index, if they are non-zero, MMIO numbers
     * must have exceeded the limit of 5.
     */
    if (partition_attrs & HANDLE_INDEX_MASK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    HANDLE_ENCODE_INDEX(partition_attrs, idx_boundary_handle);
#endif

    partition_attrs |= ((uint32_t)privileged << HANDLE_ATTR_PRIV_POS) &
                        HANDLE_ATTR_PRIV_MASK;
    partition_attrs |= ((uint32_t)ns_agent << HANDLE_ATTR_NS_POS) &
                        HANDLE_ATTR_NS_MASK;
    *p_boundary = (uintptr_t)partition_attrs;

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_activate_boundary(
                             const struct partition_load_info_t *p_ldinf,
                             uintptr_t boundary)
{
    CONTROL_Type ctrl;
    uint32_t local_handle = (uint32_t)boundary;
    bool privileged = !!(local_handle & HANDLE_ATTR_PRIV_MASK);
#if TFM_LVL == 3
    struct mpu_armv8m_region_cfg_t localcfg;
    uint32_t i, mmio_index;
    struct platform_data_t *plat_data_ptr;
    struct asset_desc_t *rt_mem;
#endif
    /* Privileged level is required to be set always */
    ctrl.w = __get_CONTROL();
    ctrl.b.nPRIV = privileged ? 0 : 1;
    __set_CONTROL(ctrl.w);
#if TFM_LVL == 3
    if (!p_ldinf) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    /* Update regions, for unprivileged partitions only */
    if (privileged) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    }
    /* Setup runtime memory first */
    localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    rt_mem = (struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);
    /*
     * NXP shortcut: The first item is the only runtime memory asset.
     * Platforms with many memory assets please check this part.
     */
    for (i = 0;
         i < p_ldinf->nassets && !(rt_mem[i].attr & ASSET_ATTR_MMIO);
         i++) {
        localcfg.region_nr = n_configured_regions + i;
        localcfg.region_base = rt_mem[i].mem.start;
        localcfg.region_limit = rt_mem[i].mem.limit;
        if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg) != MPU_ARMV8M_OK) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }

    /* Named MMIO part */
    local_handle = local_handle & (~HANDLE_INDEX_MASK);
    local_handle >>= HANDLE_PER_ATTR_BITS;
    mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;
    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
    i = n_configured_regions + i;
    while (mmio_index && i < MPU_REGION_NUM) {
        plat_data_ptr =
          (struct platform_data_t *)partition_named_mmio_list[mmio_index - 1];
        localcfg.region_nr = i++;
        localcfg.attr_access = (local_handle & HANDLE_ATTR_RW_POS)?
                            MPU_ARMV8M_AP_RW_PRIV_UNPRIV :
                            MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
        localcfg.region_base = plat_data_ptr->periph_start;
        localcfg.region_limit = plat_data_ptr->periph_limit;
        if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg) != MPU_ARMV8M_OK) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
        local_handle >>= HANDLE_PER_ATTR_BITS;
        mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;
    }
    /* Disable unused regions */
    while (i < MPU_REGION_NUM) {
        if (mpu_armv8m_region_disable(&dev_mpu_s, i++)!= MPU_ARMV8M_OK) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
#endif
    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif /* TFM_PSA_API */

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_memory_check(
                                           uintptr_t boundary, uintptr_t base,
                                           size_t size, uint32_t access_type)
{
    int flags = 0;

    /* If size is zero, this indicates an empty buffer and base is ignored */
    if (size == 0) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    }

    if (!base) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    if ((access_type & TFM_HAL_ACCESS_READWRITE) == TFM_HAL_ACCESS_READWRITE) {
        flags |= CMSE_MPU_READWRITE;
    } else if (access_type & TFM_HAL_ACCESS_READABLE) {
        flags |= CMSE_MPU_READ;
    } else {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    if (!((uint32_t)boundary & HANDLE_ATTR_PRIV_MASK)) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if ((uint32_t)boundary & HANDLE_ATTR_NS_MASK) {
        CONTROL_Type ctrl;
        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            flags |= CMSE_MPU_UNPRIV;
        } else {
            flags &= ~CMSE_MPU_UNPRIV;
        }
        flags |= CMSE_NONSECURE;
    }

    if (cmse_check_address_range((void *)base, size, flags) != NULL) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    } else {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
}

#ifdef TFM_FIH_PROFILE_ON
/* This function is responsible for checking all critical isolation configurations. */
fih_int tfm_hal_verify_static_boundaries(void)
{
    int32_t result = TFM_HAL_ERROR_GENERIC;
    
        /* Check if SAU is enabled */
    if(((SAU->CTRL & SAU_CTRL_ENABLE_Msk) == SAU_CTRL_ENABLE_Msk) &&
        /* Check if AHB secure controller check is enabled */
        (AHB_SECURE_CTRL->MISC_CTRL_DP_REG == AHB_SECURE_CTRL->MISC_CTRL_REG) &&
    #ifdef SECTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING /* Different definition name for LPC55S36 */
        ((AHB_SECURE_CTRL->MISC_CTRL_REG & SECTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U)) == SECTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U)) 
    #else
        ((AHB_SECURE_CTRL->MISC_CTRL_REG & AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U)) == AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U)) 
    #endif 
    )
    {
       result = TFM_HAL_SUCCESS;
    }

    FIH_RET(fih_int_encode(result));
}
#endif /* TFM_FIH_PROFILE_ON */
