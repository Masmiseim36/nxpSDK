#ifndef RT1050_LCD_H
#define RT1050_LCD_H

#include "board.h"

// Display.
#define LCD_DISP_GPIO GPIO1
#define LCD_DISP_GPIO_PIN (2)

// Back light.
#define LCD_BL_GPIO GPIO2
#define LCD_BL_GPIO_PIN (31)

// Macros for the touch touch controller.
#define BOARD_TOUCH_I2C LPI2C1

// Select USB1 PLL (480 MHz) as master lpi2c clock source
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
// Clock divider for master lpi2c clock source
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define BOARD_TOUCH_I2C_CLOCK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define BOARD_TOUCH_I2C_BAUDRATE (100000U)

#define LPI2C_DATA_LENGTH (32U)

#define LPI2C_MASTER_IRQ LPI2C1_IRQn
#define LPI2C_MASTER_IRQHandler LPI2C1_IRQHandler
#define BOARD_TOUCH_INT_PORT PORTA
#define BOARD_TOUCH_INT_PIN (2)

#define BOARD_LCD_READABLE (1)

#define LCD_WIDTH (480)
#define LCD_HEIGHT (272)
#define LCD_BITS_PER_PIXEL (32)

void BOARD_EnableLcdInterrupt(void);
void BOARD_InitLcd(void);
void BOARD_InitLcdifPixelClock(void);

#endif // RT1050_LCD_H
