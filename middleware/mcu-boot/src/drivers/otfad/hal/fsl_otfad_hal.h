/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_OTFAD_HAL_H_
#define _FSL_OTFAD_HAL_H_

#include "bootloader_common.h"

//! @addtogroup otfad_hal
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constants
enum _hal_constants
{
    kOtfadNumKeyWords = 4, //!< Number of key words
    kOtfadNumCtrWords = 2, //!< Number of counter words
    kOtfadNumRgnWords = 2  //!< Number of region words
};

//! @brief Format of context info.
typedef struct OtfadContextInfo
{
    uint32_t keyInfo[kOtfadNumKeyWords];    //!< Key words
    uint32_t ctrInfo[kOtfadNumCtrWords];    //!< Counter words
    uint32_t regionInfo[kOtfadNumRgnWords]; //!< Region words
} otfad_context_info_t;

//! @brief Operational modes.
enum _otfad_modes
{
    kOtfadMode_Normal = 0x00,            //!< Normal mode (NRM)
    kOtfadMode_SecurityViolation = 0x02, //!< Security Violation Mode (SVM)
    kOtfadMode_LogiallyDisabled = 0x03   //!< Logically Disabled Mode (LDM)
};

//! @brief Enable flags.
enum _otfad_enables
{
    kOtfadDisable = 0x00, //!< Disable
    kOtfadEnable = 0x01   //!< Enable
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//! @brief Get the current mode.
//!
//! @param baseAddr Module base address
//! @return Current mode in status register
uint32_t otfad_hal_get_mode(OTFAD_Type *baseAddr);

//! @brief Enable the module.
//!
//! Set the global enable flag in the control register to '1'.
//! A power-on reset is required to clear this flag.
//!
//! @param baseAddr Module base address
void otfad_hal_global_enable(OTFAD_Type *baseAddr);

//! @brief Disable the module.
//!
//! Clear the global enable flag in the control register to '0'.
//! A power-on reset is required to clear this flag.
//!
//! @param baseAddr Module base address
void otfad_hal_global_disable(OTFAD_Type *baseAddr);

//! @brief Get enabled state.
//!
//! @param baseAddr Module base address
//! @return Global enabled flag in status register
bool otfad_hal_is_enabled(OTFAD_Type *baseAddr);

//! @brief Enable restricted register status.
//!
//! Set the restricted register access enable flag in the control register to '1'.
//! A power-on reset is required to clear this flag.
//!
//! @param baseAddr Module base address
void otfad_hal_restricted_register_access_enable(OTFAD_Type *baseAddr);

//! @brief Get restricted register access state.
//!
//! @param baseAddr Module base address
//! @return Restricted register access flag in status register
bool otfad_hal_is_register_access_restricted(OTFAD_Type *baseAddr);

//! @brief Get hardware revision level.
//!
//! @param baseAddr Module base address
//! @return Hardware revision level from status register
uint32_t otfad_hal_get_hardware_revision_level(OTFAD_Type *baseAddr);

//! @brief Get number of contexts.
//!
//! @param baseAddr Module base address
//! @return Number of contexts from status register
uint32_t otfad_hal_get_number_of_contexts(OTFAD_Type *baseAddr);

//! @brief Set context info.
//!
//! @param baseAddr Module base address
//! @param contextNum Context number
//! @param contextInfo Ponter to context info struct
void otfad_hal_set_context(OTFAD_Type *baseAddr, uint32_t contextNum, const otfad_context_info_t *contextInfo);

#if defined(__cplusplus)
}
#endif // __cplusplus

//! @}

#endif // _FSL_OTFAD_HAL_H_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
