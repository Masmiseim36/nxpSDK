/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "fsl_otfad_hal.h"

#if BL_FEATURE_OTFAD_MODULE

//! @addtogroup otfad_hal
//! @{

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_otfad_hal.h for documentation on this function.
uint32_t otfad_hal_get_mode(OTFAD_Type *baseAddr)
{
    return ((baseAddr->SR & OTFAD_SR_MODE_MASK) >> OTFAD_SR_MODE_SHIFT);
}

// See fsl_otfad_hal.h for documentation on this function.
void otfad_hal_global_enable(OTFAD_Type *baseAddr)
{
    baseAddr->CR |= OTFAD_CR_GE_MASK;
}

// See fsl_otfad_hal.h for documentation on this function.
void otfad_hal_global_disable(OTFAD_Type *baseAddr)
{
    baseAddr->CR &= ~OTFAD_CR_GE_MASK;
}

// See fsl_otfad_hal.h for documentation on this function.
bool otfad_hal_is_enabled(OTFAD_Type *baseAddr)
{
    return (baseAddr->CR & OTFAD_CR_GE_MASK) ? true : false;
}

// See fsl_otfad_hal.h for documentation on this function.
void otfad_hal_restricted_register_access_enable(OTFAD_Type *baseAddr)
{
    baseAddr->CR |= OTFAD_CR_RRAE_MASK;
}

// See fsl_otfad_hal.h for documentation on this function.
bool otfad_hal_is_register_access_restricted(OTFAD_Type *baseAddr)
{
    return (baseAddr->CR & OTFAD_CR_RRAE_MASK) ? true : false;
}

// See fsl_otfad_hal.h for documentation on this function.
uint32_t otfad_hal_get_hardware_revision_level(OTFAD_Type *baseAddr)
{
    return ((baseAddr->SR & OTFAD_SR_HRL_MASK) >> OTFAD_SR_HRL_SHIFT);
}

// See fsl_otfad_hal.h for documentation on this function.
uint32_t otfad_hal_get_number_of_contexts(OTFAD_Type *baseAddr)
{
    return ((baseAddr->SR & OTFAD_SR_NCTX_MASK) >> OTFAD_SR_NCTX_SHIFT);
}

// See fsl_otfad_hal.h for documentation on this function.
void otfad_hal_set_context(OTFAD_Type *baseAddr, uint32_t contextNum, const otfad_context_info_t *contextInfo)
{
    // Key words
    baseAddr->CTX[contextNum].CTX_KEY[0] = contextInfo->keyInfo[0];
    baseAddr->CTX[contextNum].CTX_KEY[1] = contextInfo->keyInfo[1];
    baseAddr->CTX[contextNum].CTX_KEY[2] = contextInfo->keyInfo[2];
    baseAddr->CTX[contextNum].CTX_KEY[3] = contextInfo->keyInfo[3];

    // Counter words
    baseAddr->CTX[contextNum].CTX_CTR[0] = contextInfo->ctrInfo[0];
    baseAddr->CTX[contextNum].CTX_CTR[1] = contextInfo->ctrInfo[1];

    // Region words
    baseAddr->CTX[contextNum].CTX_RGD[0] = contextInfo->regionInfo[0];
    baseAddr->CTX[contextNum].CTX_RGD[1] = contextInfo->regionInfo[1];
}

//! @}

#endif // BL_FEATURE_OTFAD_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
