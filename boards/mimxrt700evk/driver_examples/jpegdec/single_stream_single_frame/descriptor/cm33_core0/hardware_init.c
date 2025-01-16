/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
#include "board.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/
#if ((DEMO_PANEL_RM67162 == DEMO_PANEL) && (!RM67162_USE_LCDIF))
void BOARD_MIPI_TE_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;

    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN);

    GPIO_GpioClearInterruptFlags(BOARD_MIPI_TE_GPIO, 1U << BOARD_MIPI_TE_PIN);

    if (intStat & (1U << BOARD_MIPI_TE_PIN))
    {
        BOARD_DisplayTEPinHandler();
    }
}
#endif

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitPsRamPins_Xspi2();

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
#if (SSD1963_DRIVEN_BY == SSD1963_DRIVEN_BY_FLEXIO)
    BOARD_InitFlexIOPanelPins();
#else /* SSD1963_DRIVEN_BY_LCDIF */
    BOARD_InitLcdDBIPanelPins();
#endif

    CLOCK_EnableClock(kCLOCK_Gpio2);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
#else
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    BOARD_InitI2cPins();
#endif
    BOARD_InitMipiPanelPinsEvk();

    CLOCK_EnableClock(kCLOCK_Gpio1);
    CLOCK_EnableClock(kCLOCK_Gpio3);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);
#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_Init16bitsPsRam(XSPI2);

    /* Disable JPEG decoder power down. */
    POWER_DisablePD(kPDRUNCFG_PPD_JPEGDEC);
    POWER_DisablePD(kPDRUNCFG_APD_JPEGDEC);
    POWER_ApplyPD();
}
/*${function:end}*/
