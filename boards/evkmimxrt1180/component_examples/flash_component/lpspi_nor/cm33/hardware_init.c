/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_lpspi_mem_adapter.h"
#include "fsl_nor_flash.h"
#include "fsl_lpspi_nor_flash.h"
/*${header:end}*/

/*${function:start}*/
lpspi_memory_config_t mem_Config = {
    .bytesInPageSize   = 256UL,    /* 256Byte */
    .bytesInSectorSize = 4096UL,   /* 4KByte */
    .bytesInMemorySize = 0x80000, /*512KByte, 4MBit*/
};

nor_config_t norConfig = {
    .memControlConfig = &mem_Config,
    .driverBaseAddr   = LPSPI_BASE,
};

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

}

uint32_t BOARD_GetLpspiClock(void)
{
    /* Configure LPSPI0102 using SYS_PLL3_PFD1_CLK */
    clock_root_config_t rootCfg = {0};
    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd1, 25);
    rootCfg.mux = kCLOCK_LPSPI0102_ClockRoot_MuxSysPll3Pfd1;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi0102, &rootCfg);

    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi0102);
}

LPSPI_Type *BOARD_GetLpspiForNorFlash(void)
{
    return LPSPI_BASE;
}

void BOARD_LpspiPcsPinControl(bool isSelected)
{
    RGPIO_WritePinOutput(RGPIO1, 5,
                  isSelected ? 0U : 1U);
}

void BOARD_LpspiIomuxConfig(spi_pin_mode_t pinMode)
{

}

uint32_t BOARD_GetNorFlashBaudrate(void)
{
    return FLASH_BAUDRATE;
}
/*${function:end}*/
