/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_flexspi.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x10000
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 512
#define EXAMPLE_SECTOR 101
#define SECTOR_SIZE 0x40000
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_FlexSpi
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS 2
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR 6
#define HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM 10
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASECHIP 12
#define CUSTOM_LUT_LENGTH 64
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

static inline void flexspi_clock_init(void)
{
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /*For XIP targets, change to use PLL2 PFD2 instead of re-configuring PLL3 PFD0
     * to prevent the potential impact to FLEXSPI2. */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);    /* Set PLL2 PFD2 clock 328MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x2); /* Choose PLL2 PFD2 clock as flexspi source clock.*/
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 82M. */
#else
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 83M, DDR mode, internal clock 42M. */
#endif
}

static inline void flexspi_clock_update(void)
{
    /* Program finished, speed the clock to 166M. */
    FLEXSPI_Enable(EXAMPLE_FLEXSPI, false);
    CLOCK_DisableClock(EXAMPLE_FLEXSPI_CLOCK);
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0); /* flexspi clock 332M, DDR mode, internal clock 166M. */
    CLOCK_EnableClock(EXAMPLE_FLEXSPI_CLOCK);
    FLEXSPI_Enable(EXAMPLE_FLEXSPI, true);
}
/*${prototype:end}*/

#endif /* _APP_H_ */
