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
    crc_config_t config;

    config.polynomial = crcConfig->crcPoly;
    config.seed       = crcConfig->crcSeed;
    config.reflectIn  = crcConfig->crcRefIn;
    config.reflectOut = crcConfig->crcRefOut;
    ;
    config.complementChecksum = crcConfig->complementChecksum;
    if (crcConfig->crcSize == 2)
        config.crcBits = kCrcBits16;
    else if (crcConfig->crcSize == 4)
        config.crcBits = kCrcBits32;
    config.crcResult = kCrcFinalChecksum;

    CRC_Init(CRC0, &config);
    CRC_WriteData(CRC0, dataIn, length);
    if (crcConfig->crcSize == 2)
        return CRC_Get16bitResult(CRC0);
    else if (crcConfig->crcSize == 4)
        return CRC_Get32bitResult(CRC0);
    return 0;
}
