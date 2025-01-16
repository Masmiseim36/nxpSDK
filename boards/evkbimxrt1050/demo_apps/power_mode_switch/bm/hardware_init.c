/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void SetLowPowerClockGate(void)
{
    CLOCK_ControlGate(kCLOCK_Aips_tz1, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Aips_tz2, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Mqs, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_FlexSpiExsc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Sim_M_Main, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Dcp, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Lpuart3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Can1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Can1S, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Can2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Can2S, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Trace, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Gpt2, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Gpt2S, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpuart2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Gpio2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpspi1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpspi2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpspi3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpspi4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Adc2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Enet, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Pit, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Aoi2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Adc1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_SemcExsc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Gpt1, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Gpt1S, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpuart4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Gpio1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Csu, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Gpio5, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_OcramExsc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Csi, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_IomuxcSnvs, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Lpi2c1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpi2c2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpi2c3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Ocotp, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Xbar3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Ipmux1, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Ipmux2, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Ipmux3, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Xbar1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Xbar2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Gpio3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lcd, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Pxp, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Flexio2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpuart5, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Semc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Lpuart6, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Aoi1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_LcdPixel, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Gpio4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Ewm0, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Wdog1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_FlexRam, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Acmp1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Acmp2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Acmp3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Acmp4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Ocram, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_IomuxcSnvsGpr, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Sim_m7_clk_r, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Iomuxc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_IomuxcGpr, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Bee, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_SimM7, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Tsc, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_SimM, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_SimEms, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Pwm1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Pwm2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Pwm3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Pwm4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Enc1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Enc2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Enc3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Enc4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Rom, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Flexio1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Wdog3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Dma, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Kpp, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Wdog2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Aips_tz4, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Spdif, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_SimMain, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Sai1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Sai2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Sai3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpuart1, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Lpuart7, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_SnvsHp, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_SnvsLp, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_UsbOh3, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Usdhc1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Usdhc2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Dcdc, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Ipmux4, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_FlexSpi, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Trng, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Lpuart8, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Timer4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Aips_tz3, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_SimPer, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Anadig, kCLOCK_ClockNeededRun);
    CLOCK_ControlGate(kCLOCK_Lpi2c4, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Timer1, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Timer2, kCLOCK_ClockNotNeeded);
    CLOCK_ControlGate(kCLOCK_Timer3, kCLOCK_ClockNotNeeded);
}

void PowerDownUSBPHY(void)
{
    USBPHY1->CTRL = 0xFFFFFFFFU;
    USBPHY2->CTRL = 0xFFFFFFFFU;
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* When wakeup from suspend, peripheral's doze & stop requests won't be cleared, need to clear them manually */
    IOMUXC_GPR->GPR4  = 0;
    IOMUXC_GPR->GPR7  = 0;
    IOMUXC_GPR->GPR8  = 0;
    IOMUXC_GPR->GPR12 = 0;

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    BOARD_InitDebugConsole();

    /* Since SNVS_PMIC_STBY_REQ_GPIO5_IO02 will output a high-level signal under Stop Mode(Suspend Mode) and this pin is
     * connected to LCD power switch circuit. So it needs to be configured as a low-level output GPIO to reduce the
     * current. */
    BOARD_Init_PMIC_STBY_REQ();
    BOARD_InitBootPeripherals();
    /* Disable clock gates which are not used in this application. User should modify this function based on application
     * requirement. */
    SetLowPowerClockGate();
    /* USBPHY is not used in this application. */
    CLOCK_DisableUsbhs0PhyPllClock();
    CLOCK_DisableUsbhs1PhyPllClock();
    PowerDownUSBPHY();
}
/*${function:end}*/
