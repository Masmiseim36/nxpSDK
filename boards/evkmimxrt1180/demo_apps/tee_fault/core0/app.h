/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DEMO_MU                     MU1_MUA
#define APP_MU_IRQn                 MU1_IRQn
/* In TEE of the config tools, has set the code TCM region (0x0ffff000 - 0x0fffffff)
 * as secure for Domain 5 (non-secure domain)
 */
#define DEMO_INVALID_DATA_ADDR      (0x0ffff000UL)  /* in M7 core ITCM */

/* TRDC related definitions. */
#define EXAMPLE_TRDC_INSTANCE       TRDC1

#define EXAMPLE_TRDC_SECURE_DOMAIN           2
#define EXAMPLE_TRDC_NONSECURE_DOMAIN        5

#define EXAMPLE_SECURE_PID          (1U)
#define EXAMPLE_NONSECURE_PID       (10U)

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS          (0x303C0000UL)
#define CORE1_KICKOFF_ADDRESS       (0x0U)

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base

#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")

#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/

void BOARD_InitHardware(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
void DEMO_SwitchToUntrustedDomain(void);
void DEMO_BootCore1(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
