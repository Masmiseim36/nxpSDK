/* -------------------------------------------------------------------------- */
/*                           Copyright 2020-2023 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_SFC_H_
#define _FWK_SFC_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                                Public types                                */
/* -------------------------------------------------------------------------- */

/*!
 * \struct sfc_config_t
 * \brief SFC configuration structure passed in SFC_Init or SFC_UpdateConfig
 * \var sfc_config_t::filterSize
 * Defines number of samples used in the filter
 * Note: it will be converted to power of two to allow for bit-shift operations
 * \var sfc_config_t::ppmTarget
 * Defines the ppm target
 */
typedef struct
{
    uint32_t filterSize;
    uint32_t ppmTarget;
} sfc_config_t;

/*!
 * \struct sfc_log_t
 * \brief SFC log structure stored in shared memory at 0x489C0078 (see m_sfc_log_start linker symbol)
 * \var sfc_log_t::lastFreqMeasured
 * Last frequency measurement result
 * \var sfc_log_t::averagePpm
 * Current averaged ppm value (output of the filter)
 */
typedef struct
{
    uint16_t lastFreqMeasured;
    int16_t  averagePpm;
} sfc_log_t;

#endif /* _FWK_SFC_H_ */
