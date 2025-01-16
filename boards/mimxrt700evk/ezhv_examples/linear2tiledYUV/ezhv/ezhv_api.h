/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EZHV_API_
#define _EZHV_API_

#include "ezhv_para.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if (EZHV_LINEAR_TO_TILE_EN)
void EZHV_Linear2Tile(uint32_t paraAddr);
#endif

#if (EZHV_DVP_EN)
void EZHV_FlexioCameraDvp(uint32_t paraAddr);
#endif

#if (EZHV_MIP_EN)
void EZHV_MiP(uint32_t paraAddr);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
