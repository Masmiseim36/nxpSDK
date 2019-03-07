/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_ecspi.h"
#include "fsl_ecspi_freertos.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ECSPI_MASTER_BASE ECSPI4
#define EXAMPLE_ECSPI_MASTER_IRQN eCSPI4_IRQn
#define EXAMPLE_ECSPI_MASTER_CLK_FREQ (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 8U)

#define EXAMPLE_ECSPI_SLAVE_BASE ECSPI4
#define EXAMPLE_ECSPI_SLAVE_IRQN eCSPI4_IRQn
#define EXAMPLE_ECSPI_SLAVE_CLK_FREQ (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 8U)

#define SINGLE_BOARD 0
#define BOARD_TO_BOARD 1

#define EXAMPLE_CONNECT_ECSPI BOARD_TO_BOARD
#if (EXAMPLE_CONNECT_ECSPI == BOARD_TO_BOARD)
#define isMASTER 0
#define isSLAVE 1
#define ECSPI_MASTER_SLAVE isMASTER
#endif
#define EXAMPLE_ECSPI_MASTER_BASEADDR ((ECSPI_Type *)EXAMPLE_ECSPI_MASTER_BASE)
#define EXAMPLE_ECSPI_SLAVE_BASEADDR ((ECSPI_Type *)EXAMPLE_ECSPI_SLAVE_BASE)

#define TRANSFER_SIZE (256)         /*! Transfer size */
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t masterReceiveBuffer[TRANSFER_SIZE] = {0};
uint32_t masterSendBuffer[TRANSFER_SIZE] = {0};
uint32_t slaveReceiveBuffer[TRANSFER_SIZE] = {0};
uint32_t slaveSendBuffer[TRANSFER_SIZE] = {0};

ecspi_slave_handle_t g_s_handle;
SemaphoreHandle_t ecspi_sem;
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define slave_task_PRIORITY (configMAX_PRIORITIES - 2)
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)
/* Interrupt priorities. */
#ifdef __GIC_PRIO_BITS
#define ECSPI_GIC_PRIO 25
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void slave_task(void *pvParameters);
#if ((ECSPI_MASTER_SLAVE == isMaster) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
static void master_task(void *pvParameters);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();

    CLOCK_SetMux(kCLOCK_EcspiMux, 0); /* Set ECSPI source to PLL3 60M */
    CLOCK_SetDiv(kCLOCK_EcspiDiv, 0); /* Set ECSPI divider to 1 */

    PRINTF("FreeRTOS ECSPI example start.\r\n");
#if (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD)
    PRINTF("This example use one ecspi instance as master and another as slave on one board.\r\n");
#elif(EXAMPLE_CONNECT_ECSPI == BOARD_TO_BOARD)
    PRINTF("This example use two boards to connect with one as master and anohter as slave.\r\n");
#endif
    PRINTF("Master and slave uses interrupt way. Slave should start first. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("ECSPI_master -- ECSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   MOSI     --    MOSI \r\n");
    PRINTF("   MISO     --    MISO \r\n");
#if (EXAMPLE_CONNECT_ECSPI == BOARD_TO_BOARD)
    PRINTF("   GND      --    GND \r\n");
#endif

    if (xTaskCreate(slave_task, "Slave_task", configMINIMAL_STACK_SIZE + 64, NULL, slave_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Failed to create slave task");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Data structure and callback function for slave SPI communication.
 */

typedef struct _callback_message_t
{
    status_t async_status;
    SemaphoreHandle_t sem;
} callback_message_t;

void ECSPI_SlaveUserCallback(ECSPI_Type *base, ecspi_slave_handle_t *handle, status_t status, void *userData)
{
    callback_message_t *cb_msg = (callback_message_t *)userData;
    BaseType_t reschedule;

    cb_msg->async_status = status;
    xSemaphoreGiveFromISR(cb_msg->sem, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/*!
 * @brief Task responsible for slave SPI communication.
 */
static void slave_task(void *pvParameters)
{
#if ((ECSPI_MASTER_SLAVE == isSLAVE) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
    ecspi_slave_config_t slaveConfig;
    ecspi_transfer_t slaveXfer;
#endif
    uint32_t errorCount;
    uint32_t i;
    callback_message_t cb_msg;

    cb_msg.sem = xSemaphoreCreateBinary();
    ecspi_sem = cb_msg.sem;
    if (cb_msg.sem == NULL)
    {
        PRINTF("ECSPI slave: Error creating semaphore\r\n");
        vTaskSuspend(NULL);
    }
    /*Set up the transfer data*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterSendBuffer[i] = i % 256;
        masterReceiveBuffer[i] = 0;

        slaveSendBuffer[i] = masterSendBuffer[i];
        slaveReceiveBuffer[i] = 0;
    }

#if ((ECSPI_MASTER_SLAVE == isSLAVE) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
    /* Slave config:
     * slaveConfig.channel = kECSPI_Channel0;
     * slaveConfig.burstLength = 8;
     * slaveConfig.txFifoThreshold = 1;
     * slaveConfig.rxFifoThreshold = 0;
     */
    ECSPI_SlaveGetDefaultConfig(&slaveConfig);

    /*  Set ecspi slave interrupt priority higher. */
    GIC_SetPriority(EXAMPLE_ECSPI_SLAVE_IRQN, ECSPI_GIC_PRIO);

    ECSPI_SlaveInit(EXAMPLE_ECSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up slave first */
    ECSPI_SlaveTransferCreateHandle(EXAMPLE_ECSPI_SLAVE_BASEADDR, &g_s_handle, ECSPI_SlaveUserCallback, &cb_msg);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData = slaveSendBuffer;
    slaveXfer.rxData = slaveReceiveBuffer;
    slaveXfer.dataSize = TRANSFER_SIZE;
    slaveXfer.channel = kECSPI_Channel0;

    ECSPI_SlaveTransferNonBlocking(EXAMPLE_ECSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);
#endif /* ((ECSPI_MASTER_SLAVE == isSLAVE) ||  (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD)) */

#if ((ECSPI_MASTER_SLAVE == isMASTER) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
    if (xTaskCreate(master_task, "Master_task", configMINIMAL_STACK_SIZE + 64, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Failed to create master task");
        vTaskSuspend(NULL);
    }
#endif /* ((ECSPI_MASTER_SLAVE == isMASTER) ||  (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD)) */

    /* Wait for transfer to finish */
    xSemaphoreTake(cb_msg.sem, portMAX_DELAY);
#if ((ECSPI_MASTER_SLAVE == isSLAVE) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
    if (cb_msg.async_status == kStatus_Success)
    {
        PRINTF("ECSPI slave transfer completed successfully. \r\n\r\n");
    }
    else
    {
        PRINTF("ECSPI slave transfer completed with error. \r\n\r\n");
    }
#endif
    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
#if (EXAMPLE_CONNECT_ECSPI == BOARD_TO_BOARD)
#if (ECSPI_MASTER_SLAVE == isMASTER)
        if (slaveSendBuffer[i] != masterReceiveBuffer[i])
        {
            errorCount++;
        }
#elif(ECSPI_MASTER_SLAVE == isSLAVE)
        if (masterSendBuffer[i] != slaveReceiveBuffer[i])
        {
            errorCount++;
        }
#endif
#elif(EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD)
        if (masterSendBuffer[i] != slaveReceiveBuffer[i])
        {
            errorCount++;
        }

        if (slaveSendBuffer[i] != masterReceiveBuffer[i])
        {
            errorCount++;
        }
#endif
    }
    if (errorCount == 0)
    {
        PRINTF("ECSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("Error occured in ECSPI transfer ! \r\n");
    }

    vTaskSuspend(NULL);
}

/*!
* @brief Task responsible for master SPI communication.
*/
#if ((ECSPI_MASTER_SLAVE == isMaster) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
static void master_task(void *pvParameters)
{
    ecspi_transfer_t masterXfer;
    ecspi_rtos_handle_t master_rtos_handle;
    ecspi_master_config_t masterConfig;
    uint32_t sourceClock;
    status_t status;

    /* Master config:
     * masterConfig.channel = kECSPI_Channel0;
     * masterConfig.burstLength = 8;
     * masterConfig.samplePeriodClock = kECSPI_spiClock;
     * masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;
     * masterConfig.chipSelectDelay = 0;
     * masterConfig.samplePeriod = 0;
     * masterConfig.txFifoThreshold = 1;
     * masterConfig.rxFifoThreshold = 0;
     */
    ECSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;

#if ((defined FSL_FEATURE_SOC_ECSPI_COUNT) && (FSL_FEATURE_SOC_ECSPI_COUNT))
    GIC_SetPriority(EXAMPLE_ECSPI_MASTER_IRQN, ECSPI_GIC_PRIO + 1);
#endif

    sourceClock = EXAMPLE_ECSPI_MASTER_CLK_FREQ;
    status = ECSPI_RTOS_Init(&master_rtos_handle, EXAMPLE_ECSPI_MASTER_BASEADDR, &masterConfig, sourceClock);

    if (status != kStatus_Success)
    {
        PRINTF("ECSPI master: error during initialization. \r\n");
        vTaskSuspend(NULL);
    }
    /*Start master transfer, send data to slave */
    masterXfer.txData = masterSendBuffer;
    masterXfer.rxData = masterReceiveBuffer;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.channel = kECSPI_Channel0;

    status = ECSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);
    if (status == kStatus_Success)
    {
#if (EXAMPLE_CONNECT_ECSPI == BOARD_TO_BOARD)
        xSemaphoreGive(ecspi_sem);
#endif
        PRINTF("ECSPI master transfer completed successfully. \r\n\r\n");
    }
    else
    {
        PRINTF("ECSPI master transfer completed with error. \r\n\r\n");
    }

    vTaskSuspend(NULL);
}
#endif //((SPI_MASTER_SLAVE == isMaster) || (EXAMPLE_CONNECT_ECSPI == SINGLE_BOARD))
