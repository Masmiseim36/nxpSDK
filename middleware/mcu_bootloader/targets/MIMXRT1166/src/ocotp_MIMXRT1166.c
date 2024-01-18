/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_ocotp.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variable
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void ocotp_get_locked_status(OCOTP_Type *base, uint32_t **pStatusWords, uint32_t *lengthInBytes)
{
    static uint32_t s_fuseLockedStatus[2] = {0};

    s_fuseLockedStatus[0] = base->LOCKED0;
    s_fuseLockedStatus[1] = base->LOCKED1;

    *pStatusWords = s_fuseLockedStatus;
    *lengthInBytes = sizeof(s_fuseLockedStatus);
}