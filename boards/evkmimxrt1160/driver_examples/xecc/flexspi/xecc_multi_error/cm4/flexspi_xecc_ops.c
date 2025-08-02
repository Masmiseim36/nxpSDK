/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "fsl_xecc.h"

extern status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);
extern status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);

void flexspi_nor_AHB_write_4bytes(FLEXSPI_Type *base, uint32_t address, uint8_t *buffer)
{
    /* Write enable */
    flexspi_nor_write_enable(base, 0);

    /* AHB write access to external flash memory */
    (*((volatile uint32_t *)(address))) =
        (uint32_t)buffer[0] << 0 | (uint32_t)buffer[1] << 8 | (uint32_t)buffer[2] << 16 | (uint32_t)buffer[3] << 24;

    flexspi_nor_wait_bus_busy(base);
}

void flexspi_nor_AHB_read_4bytes(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer)
{
    *buffer = (*((volatile uint32_t *)(address)));
}

void flexspi_nor_xecc_init(FLEXSPI_Type *flexspiBase, XECC_Type *xeccBase, const xecc_config_t *config)
{
    /* Waiting for bus idle only when FLEXSPI enabled. */
    if ((flexspiBase->MCR0 & FLEXSPI_MCR0_MDIS_MASK) != FLEXSPI_MCR0_MDIS_MASK)
    {
        /* Make sure flexspi bus idle before change its clock setting. */
        while (!FLEXSPI_GetBusIdleStatus(flexspiBase))
        {
        }
    }

    /* Clear AHB TX and RX buffer, both of them are reserved field now, so use software reset to clear instead.*/
    FLEXSPI_SoftwareReset(flexspiBase);

    XECC_Init(xeccBase, config);
}