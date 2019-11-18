/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi_cmsis.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA__EDMA0
#define DRIVER_MASTER_SPI Driver_SPI2
#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffffU
#define TRANSFER_SIZE 64U         /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user SignalEvent */
void LPSPI_MasterSignalEvent_t(uint32_t event);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterRxData[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterTxData[TRANSFER_SIZE]) = {0};

volatile bool isTransferCompleted = false;
volatile bool isMasterOnTransmit  = false;
volatile bool isMasterOnReceive   = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void LPSPI2_InitPins(void)
{
    sc_ipc_t ipc = SystemGetScfwIpcHandle();

    LPSPI2_InitPinsEx(ipc);
}

void LPSPI2_DeinitPins(void)
{
    sc_ipc_t ipc = SystemGetScfwIpcHandle();

    LPSPI2_DeinitPinsEx(ipc);
}

uint32_t LPSPI0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi0);
}

uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi1);
}

uint32_t LPSPI2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi2);
}

uint32_t LPSPI3_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi3);
}
void LPSPI_MasterSignalEvent_t(uint32_t event)
{
    if (true == isMasterOnReceive)
    {
        PRINTF("This is LPSPI_MasterSignalEvent_t\r\n");
        PRINTF("Master receive data from slave has completed!\r\n");
        isMasterOnReceive = false;
    }
    if (true == isMasterOnTransmit)
    {
        PRINTF("This is LPSPI_MasterSignalEvent_t\r\n");
        PRINTF("Master transmit data to slave has completed!\r\n");
        isMasterOnTransmit = false;
    }
    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sc_ipc_t ipc;
    uint32_t freq;

    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power on SPI. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_SPI_2, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on LPSPI\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_DMA_0_CH4, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on DMA Channel\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_DMA_0_CH5, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on DMA Channel\r\n");
    }

    /* Setup LPSPI clock */
    freq = CLOCK_SetIpFreq(kCLOCK_DMA_Lpspi2, SC_66MHZ);
    if (freq == 0)
    {
        PRINTF("Error: Failed to set LPSPI clock frequency\r\n");
    }

    /* Enable LPSPI interrupt in IRQSTEER */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_SPI2_DMA_RX_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_SPI2_DMA_TX_INT_IRQn);

    PRINTF("LPSPI CMSIS driver board to board edma example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master and slave uses EDMA way. Slave should start first. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK       --    CLK  \r\n");
    PRINTF("   PCS       --    PCS \r\n");
    PRINTF("   SOUT      --    SIN  \r\n");
    PRINTF("   SIN       --    SOUT \r\n");
    PRINTF("   GND       --    GND \r\n");

    /* DMA Mux init and EDMA init */
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR, &edmaConfig);
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR);
#endif
    uint32_t errorCount;
    uint32_t loopCount = 1U;
    uint32_t i;

    /*LPSPI master init*/
    DRIVER_MASTER_SPI.Initialize(LPSPI_MasterSignalEvent_t);
    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER, TRANSFER_BAUDRATE);

    while (1)
    {
        /* Set up the transfer data */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            masterTxData[i] = (i + loopCount) % 256U;
            masterRxData[i] = 0U;
        }

        /* Print out transmit buffer */
        PRINTF("\r\n Master transmit:\r\n");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", masterTxData[i]);
        }
        PRINTF("\r\n");

        isTransferCompleted = false;
        isMasterOnTransmit  = true;
        /* Start master transfer, send data to slave */
        DRIVER_MASTER_SPI.Send(masterTxData, TRANSFER_SIZE);
        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        }

        /* Delay to wait slave is ready */
        for (i = 0U; i < EXAMPLE_LPSPI_DEALY_COUNT; i++)
        {
            __NOP();
        }

        isTransferCompleted = false;
        isMasterOnReceive   = true;
        /* Start master transfer, receive data from slave */
        DRIVER_MASTER_SPI.Receive(masterRxData, TRANSFER_SIZE);
        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        }

        errorCount = 0;
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            if (masterTxData[i] != masterRxData[i])
            {
                errorCount++;
            }
        }
        if (errorCount == 0)
        {
            PRINTF(" \r\nLPSPI transfer all data matched! \r\n");
            /* Print out receive buffer */
            PRINTF("\r\n Master received:\r\n");
            for (i = 0; i < TRANSFER_SIZE; i++)
            {
                /* Print 16 numbers in a line */
                if ((i & 0x0FU) == 0U)
                {
                    PRINTF("\r\n");
                }
                PRINTF(" %02X", masterRxData[i]);
            }
            PRINTF("\r\n");
        }
        else
        {
            PRINTF(" \r\nError occurred in LPSPI transfer ! \r\n");
        }

        /* Wait for input any key */
        PRINTF("\r\n Input any char to run again\r\n");
        GETCHAR();
        /* Increase loop count to change transmit buffer */
        loopCount++;
    }
}
