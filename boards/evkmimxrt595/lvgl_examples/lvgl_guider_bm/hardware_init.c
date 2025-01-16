/*
 * Copyright 2020 NXP
 * All rights reserved.
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
#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
void GPIO_INTA_IRQHandler(void)
{
    uint32_t intStat;

    intStat = GPIO_PortGetInterruptStatus(GPIO, BOARD_MIPI_TE_PORT, kGPIO_InterruptA);

    if (intStat & (1UL << BOARD_MIPI_TE_PIN))
    {
        GPIO_PortClearInterruptFlags(GPIO, BOARD_MIPI_TE_PORT, kGPIO_InterruptA, (1UL << BOARD_MIPI_TE_PIN));
        BOARD_DisplayTEPinHandler();
    }

    intStat = GPIO_PortGetInterruptStatus(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, kGPIO_InterruptA);

    if (intStat & (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN))
    {
        GPIO_PortClearInterruptFlags(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, kGPIO_InterruptA,
                                     (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN));
        BOARD_TouchIntHandler();
    }
}
#endif

void BOARD_InitHardware(void)
{
    status_t status;

    BOARD_InitUARTPins();
    BOARD_InitPsRamPins();

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
    BOARD_InitFlexIOPanelPins();

    GPIO_PortInit(GPIO, BOARD_SSD1963_RST_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_CS_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_RS_PORT);
#else
    BOARD_InitMipiPanelPins();

    GPIO_PortInit(GPIO, BOARD_MIPI_POWER_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_BL_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_RST_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT);

#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
    GPIO_PortInit(GPIO, BOARD_MIPI_TE_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT);
#endif

#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RM67162 == DEMO_PANEL))
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);

    BOARD_MIPIPanelTouch_I2C_Init();
#else
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);
#endif
}
/*${function:end}*/
