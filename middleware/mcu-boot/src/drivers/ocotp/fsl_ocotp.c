/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "bootloader_common.h"
#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"
#include "fsl_clock.h"
#include "fsl_ocotp.h"
#include "microseconds/microseconds.h"

////////////////////////////////////////////////////////////////////////////////
// Feature
////////////////////////////////////////////////////////////////////////////////
#define OCOTP_FEATURE_AUTO_RELOAD_SHADOWS 1

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define OCOTP_OPT_TIMEOUT_MS 100

#define OCOTP_PROGRAM_UNLOCK_KEY 0x3e77

#define OCOTP_TIMING_RELEX_NS (uint64_t)17
#define OCOTP_TIMING_PROGRAM_NS (uint64_t)10000

#if defined(BL_TARGET_FPGA)
#define OCOTP_TIMING_READ_NS (uint64_t)1000
#else
#define OCOTP_TIMING_READ_NS (uint64_t)37
#endif

typedef struct _ocotp_timing
{
    uint32_t relax;
    uint32_t strobe_prog;
    uint32_t strobe_read;
} ocotp_timing_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static status_t ocotp_wait_for_busy(OCOTP_Type *base, uint32_t timeoutInMs);

static inline status_t ocotp_reload_shadow(OCOTP_Type *base);

static inline void ocotp_clear_error(OCOTP_Type *base);

static inline void ocotp_set_program_timing(OCOTP_Type *base);

static inline void ocotp_set_read_timing(OCOTP_Type *base);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static bool s_ocotpIntialized = { false };
static ocotp_timing_t s_ocotpTiming = { 0 };

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_ocotp.h for documentation on this function.
status_t ocotp_init(OCOTP_Type *base)
{
    // Open CLK GT.
    // No clock GT.

    // Calculate the timing parameter.
    uint32_t speedInHz = get_ocotp_clock();

    // tRelax shoule be higher than OCOTP_TIMING_RELEX_NS.
    s_ocotpTiming.relax = (OCOTP_TIMING_RELEX_NS * speedInHz + 1000000000) / 1000000000 - 1;

    // tStrobe_prog should be close to OCOTP_TIMING_PROGRAM_NS, only add half of 1000000000.
    s_ocotpTiming.strobe_prog =
        (OCOTP_TIMING_PROGRAM_NS * speedInHz + 500000000) / 1000000000 + 2 * (s_ocotpTiming.relax + 1) - 1;

    // tStrobe_read should be higher than OCOTP_TIMING_READ_NS.
    s_ocotpTiming.strobe_read =
        (OCOTP_TIMING_READ_NS * speedInHz + 1000000000) / 1000000000 + 2 * (s_ocotpTiming.relax + 1) - 1;

    return kStatus_Success;
}

// See fsl_ocotp.h for documentation on this function.
status_t ocotp_program_once(OCOTP_Type *base, uint32_t index, uint32_t *src, uint32_t lengthInBytes)
{
    assert(base);
    assert(src);
    assert(lengthInBytes);

    // If length to read is 0, then return success.
    if (!lengthInBytes)
    {
        return kStatus_Success;
    }

    // Length to read must be 1 word (4 bytes)
    // Index should be in the bit mask range.
    if ((lengthInBytes != 4) || ((index & (~OCOTP_CTRL_ADDR_MASK)) != 0))
    {
        return kStatus_InvalidArgument;
    }

    status_t status;
    // If OCOTP driver is not initialized, then do initialization first.
    if (!s_ocotpIntialized)
    {
        status = ocotp_init(base);
        // Failed to init, return the failure status;
        if (status != kStatus_Success)
        {
            return status;
        }
        // Initialized successfully. Update init status.
        s_ocotpIntialized = true;
    }

    // Before access OCOTP, must wait for the previous operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return status;
    }
    // Clear the error status caused by previous operation.
    ocotp_clear_error(base);

    // Set the timing parameter.
    ocotp_set_program_timing(base);

// Unlock the write operation.
// Write the index to the address bits.
#ifdef K32H844P_SERIES
    base->CTRL.CLR = OCOTP_CTRL_WR_UNLOCK_MASK | OCOTP_CTRL_ADDR_MASK;
    base->CTRL.SET = OCOTP_CTRL_ADDR(index) | OCOTP_CTRL_WR_UNLOCK(OCOTP_PROGRAM_UNLOCK_KEY);
#else
    base->CTRL_CLR = OCOTP_CTRL_WR_UNLOCK_MASK | OCOTP_CTRL_ADDR_MASK;
    base->CTRL_SET = OCOTP_CTRL_ADDR(index) | OCOTP_CTRL_WR_UNLOCK(OCOTP_PROGRAM_UNLOCK_KEY);
#endif
    // Write the data.
    base->DATA = *src;

    // Wait the operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return status;
    }

// Check the error status;
#ifdef K32H844P_SERIES
    if (base->CTRL.RW & OCOTP_CTRL_ERROR_MASK)
#else
    if (base->CTRL & OCOTP_CTRL_ERROR_MASK)
#endif
    {
        return kStatus_OCOTP_ProgramFailure;
    }
#if OCOTP_FEATURE_AUTO_RELOAD_SHADOWS
    // Reload the shadow registers
    status = ocotp_reload_shadow(base);
#endif // #if OCOTP_FEATURE_AUTO_RELOAD_SHADOWS

    return status;
}

// See fsl_ocotp.h for documentation on this function.
status_t ocotp_read_once(OCOTP_Type *base, uint32_t index, uint32_t *dst, uint32_t lengthInBytes)
{
    assert(base);
    assert(dst);
    assert(lengthInBytes);

    // If length to read is 0, then return success.
    if (!lengthInBytes)
    {
        return kStatus_Success;
    }

    // Length to read must be 1 word (4 bytes)
    // Index should be in the bit mask range.
    if ((lengthInBytes != 4) || ((index & (~OCOTP_CTRL_ADDR_MASK)) != 0))
    {
        return kStatus_InvalidArgument;
    }

    status_t status;
    // If OCOTP driver is not initialized, then do initialization first.
    if (!s_ocotpIntialized)
    {
        status = ocotp_init(base);
        // Failed to init, return the failure status;
        if (status != kStatus_Success)
        {
            return status;
        }
        // Initialized successfully. Update init status.
        s_ocotpIntialized = true;
    }

    // Before access OCOTP, must wait for the previous operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return status;
    }
    // Clear the error status caused by previous operation.
    ocotp_clear_error(base);

    // Set the timing parameter.
    ocotp_set_read_timing(base);

// Note: Shouldn't read the efuse data from the physical address.
// Reading physical address returns the value in shadow register,
// which won't be updated by OCOTP controller, before next reset,
// or executing reload shadows operation successfully.

// Write the index to the address bits.
#ifdef K32H844P_SERIES
    base->CTRL.CLR = OCOTP_CTRL_ADDR_MASK;
    base->CTRL.SET = OCOTP_CTRL_ADDR(index);
#else
    base->CTRL_CLR = OCOTP_CTRL_ADDR_MASK;
    base->CTRL_SET = OCOTP_CTRL_ADDR(index);
#endif
    // Start read operation.
    base->READ_CTRL = OCOTP_READ_CTRL_READ_FUSE(1);

    // Wait the operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return status;
    }

// Check the error status;
#ifdef K32H844P_SERIES
    if (base->CTRL.RW & OCOTP_CTRL_ERROR_MASK)
#else
    if (base->CTRL & OCOTP_CTRL_ERROR_MASK)
#endif
    {
        return kStatus_OCOTP_ReadFailure;
    }

    // Read the result data.
    *dst = base->READ_FUSE_DATA;

    return kStatus_Success;
}

static status_t ocotp_wait_for_busy(OCOTP_Type *base, uint32_t timeoutInMs)
{
    status_t status = kStatus_Success;

    uint64_t timeoutTicks = timeoutInMs * microseconds_convert_to_ticks(1000);
    uint64_t lastTicks = microseconds_get_ticks();

// Check the busy bit. 1 means busy.
#ifdef K32H844P_SERIES
    while (base->CTRL.RW & OCOTP_CTRL_BUSY_MASK)
#else
    while (base->CTRL & OCOTP_CTRL_BUSY_MASK)
#endif
    {
        // timeoutInMs is zero means no timeout and always wait in this loop.
        if (timeoutInMs)
        {
            uint64_t elapsedTicks = microseconds_get_ticks() - lastTicks;
            if (elapsedTicks >= timeoutTicks)
            {
                // Elapsed, return timeout error.
                status = kStatus_OCOTP_WaitTimeout;
                break;
            }
        }
    }
    return status;
}

#if OCOTP_FEATURE_AUTO_RELOAD_SHADOWS
static inline status_t ocotp_reload_shadow(OCOTP_Type *base)
{
    status_t status;
    // Before access OCOTP, must wait for the previous operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return kStatus_OCOTP_ReloadFailure;
    }
    // Clear the error status caused by previous operation.
    ocotp_clear_error(base);

    // Set the timing parameter.
    // Reload shadow operation uses the read timing.
    ocotp_set_read_timing(base);
#ifdef K32H844P_SERIES
    base->CTRL.SET = OCOTP_CTRL_RELOAD_SHADOWS(1);
#else
    base->CTRL_SET = OCOTP_CTRL_RELOAD_SHADOWS(1);
#endif

    // Wait for the reload shadow operation completed.
    status = ocotp_wait_for_busy(base, OCOTP_OPT_TIMEOUT_MS);
    if (status != kStatus_Success)
    {
        return kStatus_OCOTP_ReloadFailure;
    }
    return status;
}
#endif // #if OCOTP_FEATURE_AUTO_RELOAD_SHADOWS

static inline void ocotp_clear_error(OCOTP_Type *base)
{
#ifdef K32H844P_SERIES
    base->CTRL.CLR = OCOTP_CTRL_ERROR_MASK;
#else
    base->CTRL_CLR = OCOTP_CTRL_ERROR_MASK;
#endif
}

static inline void ocotp_set_program_timing(OCOTP_Type *base)
{
    base->TIMING &= ~(OCOTP_TIMING_RELAX_MASK | OCOTP_TIMING_STROBE_PROG_MASK);
    base->TIMING |= OCOTP_TIMING_RELAX(s_ocotpTiming.relax) | OCOTP_TIMING_STROBE_PROG(s_ocotpTiming.strobe_prog);
}

static inline void ocotp_set_read_timing(OCOTP_Type *base)
{
    base->TIMING &= ~(OCOTP_TIMING_RELAX_MASK | OCOTP_TIMING_STROBE_READ_MASK);
    base->TIMING |= OCOTP_TIMING_RELAX(s_ocotpTiming.relax) | OCOTP_TIMING_STROBE_READ(s_ocotpTiming.strobe_read);
}
