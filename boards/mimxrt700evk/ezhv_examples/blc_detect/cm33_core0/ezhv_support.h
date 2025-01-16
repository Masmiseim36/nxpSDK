/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EZHV_SUPPORT_H_
#define EZHV_SUPPORT_H_

/*******************************************************************************
 * DEFINITION
 ******************************************************************************/

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$EZHV_REGION$$Base;
extern uint32_t Image$$EZHV_REGION$$Length;
#define EZHV_IMAGE_START ((uintptr_t)&Image$$EZHV_REGION$$Base)
#define EZHV_IMAGE_SIZE  ((uintptr_t)&Image$$EZHV_REGION$$Length)
#elif defined(__ICCARM__)
#pragma section = "__ezhv_image"
#define EZHV_IMAGE_START ((uintptr_t)__section_begin("__ezhv_image"))
#define EZHV_IMAGE_SIZE  ((uintptr_t)__section_end("__ezhv_image") - EZHV_IMAGE_START)
#elif defined(__GNUC__)
extern const char ezhv_image_start[];
extern const char ezhv_image_end[];
#define EZHV_IMAGE_START ((uintptr_t)ezhv_image_start)
#define EZHV_IMAGE_SIZE  ((uintptr_t)ezhv_image_end - EZHV_IMAGE_START)
#else
#error "Not supported compiler type"
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void BOARD_EZHV_Init(void);
/*
 * Set EZHV master to secure privileged mode, this is used when EZHV wants to
 * configure some registers which must be accessed in secure privileged mode,
 * such as GPIO registers.
 * Another way is, the primary core configures all registers need secure
 * privileged accessing, and EZHV only run in non-secure and unprivileged mode.
 */
void BOARD_EZHV_SetSecurePriv(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _EZHV_SUPPORT_H_ */
