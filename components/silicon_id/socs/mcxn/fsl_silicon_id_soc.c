/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_silicon_id.h"

status_t SILICONID_ReadUniqueID(uint8_t *siliconId, uint32_t *idLen)
{
#define MCXN_UUID_ADDR (0x01100000)
#define MCXN_UUID_SIZE (16U)

    *idLen = (MCXN_UUID_SIZE <= *idLen) ? MCXN_UUID_SIZE : *idLen;

    uint8_t *p = (uint8_t *)MCXN_UUID_ADDR;
    for (uint8_t i = 0; i < *idLen; i++)
    {
        *siliconId = *p;
        siliconId++;
        p++;
    }

    return kStatus_Success;
}
