/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${header:start}*/
#include "core1_support.h"
#include "fsl_cache.h"
#include "fsl_mu.h"
/*${header:end}*/

/*${macro:start}*/
#define CORE0_BOOT_CORE1              0U
#define APP_BOARD_HAS_LED             0U
#define CORE0_BOOT_CORE1_SPECIFIC_WAY 1U

#define APP_MU     MU1_MUA
#define APP_SEMA42 SEMA42_0

#define CORE1_IMAGE_FLUSH_CACHE XCACHE_CleanInvalidateCacheByRange
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitCore0Domain(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
