/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_edma.h"
#include "fsl_xspi_edma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*psram enable x16 mode address will offset 8 byte*/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_hyper_ram_write_buffer[256], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_hyper_ram_read_buffer[256], 4);
extern void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData);
extern void xspi_hyper_ram_init(XSPI_Type *base);
extern void xspi_hyper_ram_ahbcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern void xspi_hyper_ram_ahbcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t xspi_hyper_ram_ipcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t xspi_hyper_ram_ipcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t xspi_hyper_ram_dmacommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
xspi_edma_handle_t xspiHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i  = 0;
    status_t result = kStatus_Success;
    edma_config_t userConfig;

    BOARD_InitHardware();
    /* XSPI init */
    xspi_hyper_ram_init(EXAMPLE_XSPI);

    PRINTF("XSPI edma example started!\r\n");


    /* EDMA init */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    userConfig.enableMasterIdReplication = true;
    EDMA_Init(EXAMPLE_XSPI_DMA, &userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    EDMA_EnableChannelMasterIDReplication(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, true);

    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL);


    /* Create handle for xspi. */
    XSPI_TransferCreateHandleEDMA(EXAMPLE_XSPI, &xspiHandle, xspi_callback, NULL, &dmaTxHandle,
                                    &dmaRxHandle);

    for (i = 0; i < sizeof(s_hyper_ram_write_buffer); i++)
    {
        s_hyper_ram_write_buffer[i] = i;
    }

    /* IP command write/read, should notice that the start address should be even address and the write address/size
     * should be 1024 aligned.*/
    for (i = 0; i < DRAM_SIZE; i += 1024)
    {
        result = xspi_hyper_ram_dmacommand_write_data(EXAMPLE_XSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                                         sizeof(s_hyper_ram_write_buffer));
        if (result != kStatus_Success)
        {
            PRINTF("EDMA Command Write Failure 0x%x - 0x%x, Failure code : 0x%x!\r\n", i, i + 1023, result);
            return -1;
        }

        xspi_hyper_ram_ahbcommand_read_data(EXAMPLE_XSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                                        sizeof(s_hyper_ram_read_buffer));

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("EDMA Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }

    PRINTF("EDMA Command Read/Write data successfully at all address range !\r\n");

    while (1)
    {
    }

}
