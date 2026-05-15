/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_silicon_id.h"

#define MCXE31X_UUID_ADDR (0x1B000040U)
#define MCXE31X_UUID_SIZE (8U)

status_t SILICONID_ReadUniqueID(uint8_t *siliconId, uint32_t *idLen)
{
    *idLen     = (MCXE31X_UUID_SIZE <= *idLen) ? MCXE31X_UUID_SIZE : *idLen;
    uint8_t *p = (uint8_t *)MCXE31X_UUID_ADDR;

    for (uint8_t i = 0U; i < *idLen; i++)
    {
        *siliconId = *p;
        siliconId++;
        p++;
    }

    return kStatus_Success;
}
