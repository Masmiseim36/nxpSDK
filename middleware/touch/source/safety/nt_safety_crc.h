/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _NT_SAFETY_CRC_H_
#define _NT_SAFETY_CRC_H_

/**
 * \defgroup gsafety_crc General API
 * \ingroup safety_crc
 * General Function definition of the safety_crc.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Calculate CRC checksum on the System configuration data
 * \return the status of the calculation
 *   - NT_SUCCESS if the calculation was properly done
 *   - NT_FAILURE if the calculation cannot be finished
 *
 * Calculate the CRC on the System configuration data and returns the pointer to the CRC checksum
  \code
    // Calc CRC on system configuration data
    int32_t testResult;
    testResult = nt_safety_calc_cfg_crc(const struct nt_system *system, uint16_t *crc_val);
  \endcode
 */
int32_t nt_safety_calc_cfg_crc(const struct nt_system *system, uint16_t *crc_val);

/**
 * \brief Check the System configuration data.
 * \return the status of the operation
 *   - NT_SUCCESS if the check was properly done
 *   - NT_FAILURE if the check cannot be finished
 * Check the system data .
  \code
    // Check the system configuration data, calculate CRC and compare with the  pre-calculated CRC
    int32_t testResult;
    testResult = nt_safety_check_cfg_data(const struct nt_system *system, uint16_t crc_val);
  \endcode
 */
int32_t nt_safety_check_cfg_data(const struct nt_system *system, uint16_t crc_val);

#ifdef __cplusplus
}
#endif

/** \} end of gsafety_crc_api group */
/** \} end of safety_crc group */

#endif
