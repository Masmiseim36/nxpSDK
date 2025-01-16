/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_BOARD_H
#define LVGL_SUPPORT_BOARD_H

#include <stdint.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_TOUCH_I2C           Driver_I2C2
#define BOARD_TOUCH_I2C_IRQ       FLEXCOMM2_IRQn
#define BOARD_TOUCH_I2C_FREQ_FUNC I2C2_GetFreq
#define BOARD_TOUCH_I2C_FREQ      CLOCK_GetFlexCommClkFreq(2U)

#define BOARD_LCD_SPI           Driver_SPI5
#define BOARD_LCD_SPI_BAUDRATE  10000000U
#define BOARD_LCD_SPI_IRQ       FLEXCOMM5_IRQn
#define BOARD_LCD_SPI_FREQ_FUNC SPI5_GetFreq
#define BOARD_LCD_SPI_FREQ      CLOCK_GetFlexCommClkFreq(5U)
#define BOARD_LCD_SPI_DMA_IRQ   DMA0_IRQn

#define BOARD_INIT_TOUCH_I2C_CLOCK() do {  \
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);   \
} while (false)

#define BOARD_INIT_LCD_SPI_CLOCK() do {  \
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM5); \
} while (false)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_BOARD_H */
