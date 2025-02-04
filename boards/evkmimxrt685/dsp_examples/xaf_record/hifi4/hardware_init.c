/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "board_hifi4.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "fsl_i2s.h"
#include "pin_mux.h"
#include "srtm_utils.h"
#include "fsl_sema42.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
extern int NonCacheable_start, NonCacheable_end;
extern int NonCacheable_init_start, NonCacheable_init_end;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${function:start}*/
static void BOARD_InitClock(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* sets external XTAL OSC freq */

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* DSP_INT0_SEL18 = DMA1 */
    INPUTMUX_AttachSignal(INPUTMUX, 18U, kINPUTMUX_Dmac1ToDspInterrupt);
    /* MUB interrupt signal is selected for DSP interrupt input 1 */
    INPUTMUX_AttachSignal(INPUTMUX, 1U, kINPUTMUX_MuBToDspInterrupt);

    /* I2S clocks */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM4);
    /* reset FLEXCOMM for I2S */
    RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);

    /* DMIC uses 24.576MHz AUDIO_PLL clock */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC_CLK);

    /* 24.576MHz divided by 12 = 2048 KHz PDM clock --> gives 16kHz sample rate */
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 12);
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}

void BOARD_InitHardware(void)
{
    XOS_Init();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitClock();

    /* Iniitalize DMA1 which will be shared by capturer and renderer. */
    DMA_Init(DMA1);

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
}

/*${function:end}*/
