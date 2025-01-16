/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* make it easy on the folks that want to compile the libs with a
   different malloc than stdlib */
#define _ogg_malloc  ogg_OSA_malloc
#define _ogg_calloc  ogg_OSA_malloc
#define _ogg_realloc ogg_OSA_realloc
#define _ogg_free    ogg_OSA_free

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void *ogg_OSA_malloc(uint32_t size);
void *ogg_OSA_realloc(void *ptr, uint32_t size);
void ogg_OSA_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* _MEMORY_H */
