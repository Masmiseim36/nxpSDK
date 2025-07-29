/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x20600000
#define APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY

#define BUTTON_1_INIT()                                    \
    gpio_pin_config_t sw_config = {kGPIO_DigitalInput, 0}; \
    GPIO_PinInit(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN, &sw_config)
#define IS_BUTTON_1_PRESSED() (0U == GPIO_PinRead(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN))
#define BUTTON_1_NAME         BOARD_SW5_NAME

#define BUTTON_2_INIT()       GPIO_PinInit(BOARD_SW7_GPIO, BOARD_SW7_GPIO_PIN, &sw_config)
#define IS_BUTTON_2_PRESSED() (0U == GPIO_PinRead(BOARD_SW7_GPIO, BOARD_SW7_GPIO_PIN))
#define BUTTON_2_NAME         "SW7"

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
extern uint32_t core1_image_size;
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
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
/*${prototype:end}*/

#endif /* _APP_H_ */
