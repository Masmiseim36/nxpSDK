/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "display_support.h"
#include "lvgl_support.h"
/*${header:end}*/

/*${function:start}*/
#if (DEMO_PANEL_RM67162 == DEMO_PANEL) || (DEMO_PANEL_CO5300 == DEMO_PANEL)

void  BOARD_MIPI_TE_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;

    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN);

    if (intStat != 0U)
    {
        GPIO_GpioClearInterruptFlags(BOARD_MIPI_TE_GPIO, 1U << BOARD_MIPI_TE_PIN);
        BOARD_DisplayTEPinHandler();
    }
}
void  BOARD_MIPI_TOUCH_INT_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;
    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN);

    if (intStat != 0U)
    {
        GPIO_GpioClearInterruptFlags(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, 1U << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
        BOARD_TouchIntHandler();
    }
}
#endif


void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitPsRamPins_Xspi2();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_Init16bitsPsRam(XSPI2);

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
#if (SSD1963_DRIVEN_BY == SSD1963_DRIVEN_BY_FLEXIO)
    BOARD_InitFlexIOPanelPins();
#else
    BOARD_InitLcdDBIPanelPins();
#endif
    CLOCK_EnableClock(kCLOCK_Gpio2);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    BOARD_InitI2c8();
#else
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    BOARD_InitI2c8();
#endif
    BOARD_InitMipiPanelPins();
    CLOCK_EnableClock(kCLOCK_Gpio1);
    CLOCK_EnableClock(kCLOCK_Gpio3);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);

    BOARD_InitMipiTouchPanelPins();

#endif

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RM67162 == DEMO_PANEL) || \
     (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL) || (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL) || \
     (DEMO_PANEL_CO5300 == DEMO_PANEL))
    BOARD_MIPIPanelTouch_I2C_Init();
#else
#endif
}
/*${function:end}*/
