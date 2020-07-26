/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "crc.h"
#include "fsl_crc.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t HAL_CrcCompute(hal_crc_config_t *crcConfig, uint8_t *dataIn, uint32_t length)
{
    CRC_Type *const s_CrcList[] = CRC_BASE_PTRS;
    crc_config_t config;

    config.seed          = crcConfig->crcSeed;
    config.reverseIn     = crcConfig->crcRefIn;
    config.complementIn  = false;
    config.complementOut = crcConfig->complementChecksum;
    config.reverseOut    = crcConfig->crcRefOut;

    if (crcConfig->crcSize == 2)
    {
        config.polynomial = kCRC_Polynomial_CRC_CCITT;
    }
    else if (crcConfig->crcSize == 4)
    {
        config.polynomial = kCRC_Polynomial_CRC_32;
    }

    CRC_Init(s_CrcList[0], &config);
    CRC_WriteData(s_CrcList[0], dataIn, length);
    if (crcConfig->crcSize == 2)
        return CRC_Get16bitResult(s_CrcList[0]);
    else if (crcConfig->crcSize == 4)
        return CRC_Get32bitResult(s_CrcList[0]);
    return 0;
}
