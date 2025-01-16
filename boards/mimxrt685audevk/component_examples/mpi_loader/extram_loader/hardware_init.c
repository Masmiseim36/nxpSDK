/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_mpi_loader.h"
/*${header:end}*/

/*${function:start}*/
extern const clock_sys_pll_config_t g_sysPllConfig_BOARD_BootClockRUN;
extern const clock_audio_pll_config_t g_audioPllConfig_BOARD_BootClockRUN;

void BOARD_InitHardware(void)
{
    /* Need to set global variables. */
    POWER_UpdateOscSettlingTime(BOARD_SYSOSC_SETTLING_US); /* Updated XTAL oscillator settling time */
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ);              /* Sets external XTAL OSC freq */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;

    BOARD_InitDebugConsole();
}

static void APP_BootClockRUN()
{
    /* Configure LPOSC clock*/
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC); /* Power on LPOSC (1MHz) */
    /* Configure FFRO clock */
    POWER_DisablePD(kPDRUNCFG_PD_FFRO);  /* Power on FFRO (48/60MHz) */
    CLOCK_EnableFfroClk(kCLOCK_Ffro48M); /* Enable FFRO clock*/
    /* Configure SFRO clock */
    POWER_DisablePD(kPDRUNCFG_PD_SFRO); /* Power on SFRO (16MHz) */
    CLOCK_EnableSfroClk();              /* Wait until SFRO stable */

    /* Let CPU run on ffro for safe switching */
    CLOCK_AttachClk(kFFRO_to_MAIN_CLK);

    /* Configure SYSOSC clock source */
    POWER_DisablePD(kPDRUNCFG_PD_SYSXTAL);                       /* Power on SYSXTAL */
    CLOCK_EnableSysOscClk(true, true, BOARD_SYSOSC_SETTLING_US); /* Enable system OSC */

    /* Configure SysPLL0 clock source */
    CLOCK_InitSysPll(&g_sysPllConfig_BOARD_BootClockRUN);
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 19); /* Enable MAIN PLL clock */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24); /* Enable AUX0 PLL clock */

    /* Configure Audio PLL clock source */
    CLOCK_InitAudioPll(&g_audioPllConfig_BOARD_BootClockRUN);
    CLOCK_InitAudioPfd(kCLOCK_Pfd0, 26);         /* Enable Audio PLL clock */

    CLOCK_SetClkDiv(kCLOCK_DivSysCpuAhbClk, 2U); /* Set SYSCPUAHBCLKDIV divider to value 2 */

    /* Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kMAIN_PLL_to_MAIN_CLK);        /* Switch MAIN_CLK to MAIN_PLL */
    CLOCK_AttachClk(kMAIN_CLK_DIV_to_SYSTICK_CLK); /* Switch SYSTICK_CLK to MAIN_CLK_DIV */

    /* Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAudioPllClk, 15U); /* Set AUDIOPLLCLKDIV divider to value 15 */
    CLOCK_SetClkDiv(kCLOCK_DivPllFrgClk, 12U);   /* Set FRGPLLCLKDIV divider to value 12 */
}

/* Enable multicore packed image loader for DSP context copy. */
void SystemInitHook(void)
{
    status_t status;

    BOARD_InitBootPins();
    BOARD_InitPsRamPins();
    APP_BootClockRUN();

    status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

    MPI_LoadMultiImages();
}
/*${function:end}*/
