/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_ezhv.h"
#include "ezhv_support.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EZHV_ITCM_ADDRESSESS 0x24100000UL

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_EZHV_Init(void)
{
    ezhv_copy_image_t ezhv_image;

    ezhv_image.srcAddr = EZHV_IMAGE_START;
    ezhv_image.size= EZHV_IMAGE_SIZE;
    ezhv_image.destAddr = EZHV_ITCM_ADDRESSESS;

    EZHV_Init(&ezhv_image);
    XCACHE_CleanInvalidateCacheByRange(ezhv_image.destAddr, ezhv_image.size);

    EZHV_Boot(EZHV_ITCM_ADDRESSESS);
}

void BOARD_EZHV_SetSecurePriv(void)
{
    GlikeyWriteEnable(GLIKEY2, 3U);
    AHBSC4->MASTER_SEC_LEVEL |= AHBSC4_MASTER_SEC_LEVEL_EZHV_MASK;
    AHBSC4->MASTER_SEC_ANTI_POL_REG &= ~AHBSC4_MASTER_SEC_LEVEL_EZHV_MASK;

    GlikeyWriteEnable(GLIKEY0, 6U);
    AHBSC0->MASTER_SEC_LEVEL |= AHBSC0_MASTER_SEC_LEVEL_MEDIA_MASK;
    AHBSC0->MASTER_SEC_ANTI_POL_REG &= ~AHBSC0_MASTER_SEC_LEVEL_MEDIA_MASK;

    GlikeyClearConfig(GLIKEY0);
    GlikeyClearConfig(GLIKEY2);
}
