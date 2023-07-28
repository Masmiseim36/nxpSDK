/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _NAND_ECC_H_
#define _NAND_ECC_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief NAND ECC Status Group
enum
{
    kStatusGroup_NandECC = 213,
};

//! @brief Status for NAND ECC driver
enum _nand_ecc_status
{
    kStatus_NandECC_NoBitErrorDetected    = MAKE_STATUS(kStatusGroup_NandECC, 0),
    kStatus_NandECC_SingleBitErrorCorrected    = MAKE_STATUS(kStatusGroup_NandECC, 1),
    kStatus_NandECC_NoLessThanDoubleBitError    = MAKE_STATUS(kStatusGroup_NandECC, 2),
    kStatus_NandECC_EccDataErrorInOOB    = MAKE_STATUS(kStatusGroup_NandECC, 3),
};

#define NAND_ECC_MAX_BLOCK_SIZE (512U)

//!@brief NAND ECC block size
typedef enum _nand_ecc_block_size
{
    kNandEccBlockSize_256B = 256U,
    kNandEccBlockSize_512B = 512U,
} nand_ecc_block_size_t;

//!@brief NAND ECC SEC detect error info
enum
{
    kNandEccSecResultBit1Count_NoError = 0U,
    kNandEccSecResultBit1Count_OobError = 1U,
    kNandEccSecResultBit1Count_SingleError256B = 11U,
    kNandEccSecResultBit1Count_SingleError512B = 12U,
};

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/* Calculate 3-byte ECC for 256/512 byte block */
void nand_ecc_sec_calculate_encode(nand_ecc_block_size_t blockSize, uint8_t *nandData, uint8_t *calculatedEccCode);

/* Detect and correct a 1 bit error for 256/512 byte block */
status_t nand_ecc_sec_verify_correct(nand_ecc_block_size_t blockSize, uint8_t *nandData, uint8_t *readoutEccCode);

#ifdef __cplusplus
}
#endif

#endif // _NAND_ECC_H_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
