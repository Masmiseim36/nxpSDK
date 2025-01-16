/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CPU_NAME "iMXRT1176"

#define APP_WAKEUP_BUTTON_GPIO        BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_SNVS_IRQ         SNVS_HP_NON_TZ_IRQn
#define APP_WAKEUP_SNVS_IRQ_HANDLER SNVS_HP_NON_TZ_IRQHandler

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x20200000

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif defined(__GNUC__)
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__ICCARM__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#elif defined(__GNUC__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#endif

#if (defined(BOARD_USE_EXT_PMIC) && BOARD_USE_EXT_PMIC)
#define DEMO_PF5020_LPI2C             LPI2C6
#define DEMO_PF5020_LPI2C_CLKSRC_FREQ (CLOCK_GetFreq(kCLOCK_OscRc16M))
#define DEMO_PF5020_LPI2C_BAUDRATE    1000000U
#endif /* (defined(BOARD_USE_EXT_PMIC) && BOARD_USE_EXT_PMIC) */

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_BootCore1(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
