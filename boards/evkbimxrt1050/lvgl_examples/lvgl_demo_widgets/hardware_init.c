/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
AT_QUICKACCESS_SECTION_CODE(void BOARD_ReconfigFlexSpiRxBuffer(void));

/*
 * When PXP fetch images from FlexSPI flash, the default FlexSPI RX buffer
 * configuration does not meet the PXP bandwidth requirement. Reconfigure
 * here.
 */
void BOARD_ReconfigFlexSpiRxBuffer(void)
{
    uint32_t ahbcr;

    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }

    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    ahbcr = FLEXSPI->AHBCR;

    /* Temporarily disable prefetching while changing the buffer settings */
    FLEXSPI->AHBCR = ahbcr & ~(FLEXSPI_AHBCR_CACHABLEEN_MASK | FLEXSPI_AHBCR_PREFETCHEN_MASK);

    /* Wait for FlexSPI idle to make sure no flash data transfer. */
    while ((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) == 0U)
    {
    }

    /* Allocate half of the prefetch buffer to the core */
    FLEXSPI->AHBRXBUFCR0[0] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID(0) | FLEXSPI_AHBRXBUFCR0_BUFSZ(0x40);

    /* Disable dedicate prefetch buffer for DMA. */
    FLEXSPI->AHBRXBUFCR0[1] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID(1) | FLEXSPI_AHBRXBUFCR0_BUFSZ(0x00);

    /* Disable dedicate prefetch buffer for DCP. */
    FLEXSPI->AHBRXBUFCR0[2] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID(2) | FLEXSPI_AHBRXBUFCR0_BUFSZ(0x00);

    /* Other half of the buffer for other masters incl. PXP */
    FLEXSPI->AHBRXBUFCR0[3] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID(3) | FLEXSPI_AHBRXBUFCR0_BUFSZ(0x40);

    FLEXSPI->AHBCR = ahbcr; /* Set AHBCR back to the original value */

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

void BOARD_InitHardware(void)
{
    /* Set the eLCDIF read_qos priority high, to make sure eLCDIF
     * can fetch data in time when PXP is used.
     */
    *((volatile uint32_t *)0x41044100) = 5;

    BOARD_ConfigMPU();
    BOARD_ReconfigFlexSpiRxBuffer();
    BOARD_InitPins();
    BOARD_InitI2C1Pins();
    BOARD_InitSemcPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
