/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright 2020-2022 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "tfm_plat_defs.h"
#include "uart_stdout.h"
#include "fih.h"
#include "tfm_spm_log.h"

extern const struct memory_region_limits memory_regions;

#ifdef PLAT_HAS_ITRC /* Intrusion and Tamper Response Controller (ITRC) */
#include "fsl_itrc.h"
static enum tfm_plat_err_t init_itrc(void)
{
    /* Clear all possible pending Event/Action statuses */
    ITRC_ClearAllStatus(ITRC);

    /* Lock all events in ITRC (except Sw Events). */
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_CssGlitch, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_RtcTamper, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_Cdog, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_BodVbat, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_BodVdd, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_Watchdog, kITRC_Lock, kITRC_Enable);
#if 0 /* Disabled, to avoid HW reset on debug mail box start, reading erased memory. */
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_FlashEcc, kITRC_Lock, kITRC_Enable);
#endif
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_Ahb, kITRC_Lock, kITRC_Enable);
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_CssErr, kITRC_Lock, kITRC_Enable);
#if defined(FSL_FEATURE_ITRC_HAS_SYSCON_GLITCH) && (FSL_FEATURE_ITRC_HAS_SYSCON_GLITCH > 0)
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_SysconGlitch, kITRC_Lock, kITRC_Enable);
#endif
    ITRC_SetActionToEvent(ITRC, kITRC_ChipReset, kITRC_Pkc, kITRC_Lock, kITRC_Enable);

    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* PLAT_HAS_ITRC */

#ifdef TFM_FIH_PROFILE_ON
fih_int tfm_hal_platform_init(void)
#else
enum tfm_hal_status_t tfm_hal_platform_init(void)
#endif
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;

    stdio_init();

#ifdef PLAT_HAS_ITRC /* Intrusion and Tamper Response Controller (ITRC) */
    plat_err = init_itrc();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
#endif

    plat_err = enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = init_debug();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    __enable_irq();

    plat_err = nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

uint32_t tfm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_hal_get_ns_entry_point(void)
{
    return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}

#ifdef FIH_ENABLE_DELAY  

/* This implementation is based on MCUBoot */
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

static mbedtls_entropy_context fih_entropy_ctx;
static mbedtls_ctr_drbg_context fih_drbg_ctx;

fih_int tfm_fih_random_init(void)
{
    mbedtls_entropy_init(&fih_entropy_ctx);
    mbedtls_ctr_drbg_init(&fih_drbg_ctx);
    mbedtls_ctr_drbg_seed(&fih_drbg_ctx , mbedtls_entropy_func,
                          &fih_entropy_ctx, NULL, 0);

    return FIH_SUCCESS;
}

void tfm_fih_random_generate(uint8_t *rand)
{
    mbedtls_ctr_drbg_random(&fih_drbg_ctx, (unsigned char*) rand, sizeof(uint8_t));
}

#endif /* FIH_ENABLE_DELAY */

/* Alternative Control Flow Integrity, using  Code Watchdog Timer */
#if defined(FIH_ENABLE_CFI) & defined(FIH_CFI_ALT) & defined(CDOG)

#include "fsl_cdog.h"

static volatile bool cdog_init_is_done = false;
static void fih_cdog_init(void)
{
    status_t result = kStatus_Fail;
    cdog_config_t conf;
    
    /* Initialize HASHCRYPT */
    CDOG_GetDefaultConfig(&conf);

    conf.timeout    = kCDOG_FaultCtrl_EnableInterrupt;
    conf.miscompare = kCDOG_FaultCtrl_EnableInterrupt;
    conf.sequence   = kCDOG_FaultCtrl_EnableInterrupt;
    conf.control    = kCDOG_FaultCtrl_EnableReset; /* Note: Control can generate only reset */
    conf.state      = kCDOG_FaultCtrl_EnableInterrupt;
    conf.address    = kCDOG_FaultCtrl_EnableInterrupt;
    conf.irq_pause  = kCDOG_IrqPauseCtrl_Pause;
    conf.debug_halt = kCDOG_DebugHaltCtrl_Pause;
    conf.lock       = kCDOG_LockCtrl_Lock; /* Lock */ //kCDOG_LockCtrl_Unlock;

    /* Clears pending FLAGS and sets CONTROL register */
    result = CDOG_Init(CDOG, &conf);
    if (result != kStatus_Success)
    {
        SPMLOG_ERRMSG("[CDOG] Init error.\r\n");
        FIH_PANIC;
    }
   
    cdog_init_is_done = true;
}

void CDOG_DriverIRQHandler(void)
{
    NVIC_ClearPendingIRQ(CDOG_IRQn);

    SPMLOG_ERRMSG("[CDOG IRQ] ");

    if ((CDOG->FLAGS & CDOG_FLAGS_TO_FLAG_MASK))
    {
        SPMLOG_ERRMSG("Timeout ");
    }
    if ((CDOG->FLAGS & CDOG_FLAGS_MISCOM_FLAG_MASK))
    {
        SPMLOG_ERRMSG("Miscompare ");
    }
    if ((CDOG->FLAGS & CDOG_FLAGS_SEQ_FLAG_MASK))
    {
        SPMLOG_ERRMSG("Sequence ");
    }
    if ((CDOG->FLAGS & CDOG_FLAGS_CNT_FLAG_MASK))
    {
        SPMLOG_ERRMSG("Control ");
    }
    if ((CDOG->FLAGS & CDOG_FLAGS_STATE_FLAG_MASK))
    {
        SPMLOG_ERRMSG("State ");
    }
    if ((CDOG->FLAGS & CDOG_FLAGS_ADDR_FLAG_MASK))
    {
        SPMLOG_ERRMSG("Address ");
    }
    SPMLOG_ERRMSG("fault occured\r\n");

    FIH_PANIC;
}

fih_int fih_cfi_get_and_increment(uint8_t cnt)
{
    fih_int saved_ctr = _fih_cfi_ctr;

    /* HW */   
    if(cdog_init_is_done == false)
    {
        fih_cdog_init();
    }
    
    /* Start if in the IDLE state */
    if((CDOG->STATUS & CDOG_STATUS_CURST_MASK) == CDOG_STATUS_CURST(0x5)) {
        CDOG_Start(CDOG, 0xFFFFFFFF, fih_int_decode(saved_ctr));
    }

    CDOG_Add(CDOG, cnt);
    
    /* SW */
    _fih_cfi_ctr = fih_int_encode(fih_int_decode(_fih_cfi_ctr) + cnt);
    
    return saved_ctr;
}

void fih_cfi_validate(fih_int saved)
{
    /* HW */
    CDOG_Stop(CDOG, fih_int_decode(saved));
}

void fih_cfi_decrement(void)
{
    /* HW */
    /* Start if in the IDLE state */
    if((CDOG->STATUS & 0xF0000000) == 0x50000000) {
        CDOG_Start(CDOG, 0xFFFFFFFF, (fih_int_decode(_fih_cfi_ctr)));
    }    
    CDOG_Sub1(CDOG);
    
    /* SW */
    _fih_cfi_ctr = fih_int_encode(fih_int_decode(_fih_cfi_ctr) - 1);
}
#endif /* defined(FIH_ENABLE_CFI) & defined(FIH_CFI_ALT) & defined(CDOG) */

