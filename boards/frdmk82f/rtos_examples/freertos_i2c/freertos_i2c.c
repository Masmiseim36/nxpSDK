/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
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
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

/*  Standard C Included Files */
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_i2c_freertos.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE I2C0_BASE
#define EXAMPLE_I2C_SLAVE_BASE I2C1_BASE

#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define EXAMPLE_I2C_SLAVE ((I2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#if (EXAMPLE_I2C_MASTER_BASE == I2C0_BASE)
#define I2C_MASTER_CLK_SRC (I2C0_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C1_BASE)
#define I2C_MASTER_CLK_SRC (I2C1_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C2_BASE)
#define I2C_MASTER_CLK_SRC (I2C2_CLK_SRC)
#else
#error Should define the I2C_MASTER_CLK_SRC!
#endif

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE (100000) /* 100K */
#define I2C_DATA_LENGTH (32)  /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

i2c_master_handle_t g_m_handle;
i2c_slave_handle_t g_s_handle;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define slave_task_PRIORITY (configMAX_PRIORITIES - 1)
#define master_task_PRIORITY (configMAX_PRIORITIES - 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void slave_task(void *pvParameters);
static void master_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    PRINTF("\r\n==I2C example -- MasterInterrupt_SlaveInterrupt.==\r\n");

    if (xTaskCreate(slave_task, "Slave_task", configMINIMAL_STACK_SIZE + 60, NULL, slave_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Failed to create slave task");
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Data structure and callback function for slave I2C communication.
 */

typedef struct _callback_message_t
{
    status_t async_status;
    SemaphoreHandle_t sem;
} callback_message_t;

static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
    callback_message_t *cb_msg = (callback_message_t *)userData;
    BaseType_t reschedule;

    switch (xfer->event)
    {
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            xfer->data = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            cb_msg->async_status = xfer->completionStatus;
            xSemaphoreGiveFromISR(cb_msg->sem, &reschedule);
            portYIELD_FROM_ISR(reschedule);
            break;

        default:
            break;
    }
}

/*!
 * @brief Task responsible for slave I2C communication.
 */

static void slave_task(void *pvParameters)
{
    i2c_slave_config_t slaveConfig;
    callback_message_t cb_msg;

    cb_msg.sem = xSemaphoreCreateBinary();
    if (cb_msg.sem == NULL)
    {
        PRINTF("I2C slave: Error creating semaphore\r\n");
        vTaskSuspend(NULL);
    }

/*  Set i2c slave interrupt priority higher. */
#if (EXAMPLE_I2C_SLAVE_BASE == LPI2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C0_IRQn, 5);
    else
        NVIC_SetPriority(LPI2C0_IRQn, 2);
#elif(EXAMPLE_I2C_SLAVE_BASE == LPI2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C1_IRQn, 5);
    else
        NVIC_SetPriority(LPI2C1_IRQn, 2);
#elif(EXAMPLE_I2C_SLAVE_BASE == LPI2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C2_IRQn, 5);
    else
        NVIC_SetPriority(LPI2C2_IRQn, 2);
#elif(EXAMPLE_I2C_SLAVE_BASE == I2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C0_IRQn, 5);
    else
        NVIC_SetPriority(I2C0_IRQn, 2);
#elif(EXAMPLE_I2C_SLAVE_BASE == I2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C1_IRQn, 5);
    else
        NVIC_SetPriority(I2C1_IRQn, 2);
#elif(EXAMPLE_I2C_SLAVE_BASE == I2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C2_IRQn, 5);
    else
        NVIC_SetPriority(I2C2_IRQn, 2);
#endif

    /* Set up I2C slave */
    /*
     * slaveConfig.addressingMode = kI2C_Address7bit;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.enableWakeUp = false;
     * slaveConfig.enableHighDrive = false;
     * slaveConfig.enableBaudRateCtl = false;
     * slaveConfig.enableSlave = true;
     */
    I2C_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.addressingMode = kI2C_Address7bit;
    slaveConfig.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    slaveConfig.upperAddress = 0; /*  not used for this example */

    I2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig);

    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = 0;
    }

    memset(&g_s_handle, 0, sizeof(g_s_handle));

    I2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, i2c_slave_callback, &cb_msg);
    I2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle, kI2C_SlaveCompletionEvent);

    if (xTaskCreate(master_task, "Master_task", configMINIMAL_STACK_SIZE + 124, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Failed to create master task");
        vTaskSuspend(NULL);
    }

    /* Wait for transfer to finish */
    xSemaphoreTake(cb_msg.sem, portMAX_DELAY);

    if (cb_msg.async_status == kStatus_Success)
    {
        PRINTF("I2C slave transfer completed successfully. \r\n\r\n");
    }
    else
    {
        PRINTF("I2C slave transfer completed with error. \r\n\r\n");
    }

    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occured in this transfer ! \r\n");
            break;
        }
    }

    PRINTF("Slave received data :");
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Set up slave ready to send data to master. */
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = ~g_slave_buff[i];
    }

    PRINTF("This time , slave will send data: :");
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Wait for transfer to finish */
    xSemaphoreTake(cb_msg.sem, portMAX_DELAY);

    vTaskSuspend(NULL);
}

static void master_task(void *pvParameters)
{
    i2c_rtos_handle_t master_rtos_handle;
    i2c_master_config_t masterConfig;
    i2c_master_transfer_t masterXfer;
    uint32_t sourceClock;
    status_t status;

#if (EXAMPLE_I2C_MASTER_BASE == LPI2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C0_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C0_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C1_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C1_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C2_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C2_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C0_IRQn, 6);
    else
        NVIC_SetPriority(I2C0_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C1_IRQn, 6);
    else
        NVIC_SetPriority(I2C1_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C2_IRQn, 6);
    else
        NVIC_SetPriority(I2C2_IRQn, 3);
#endif

    /* Set up i2c master to send data to slave */
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /*
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableHighDrive = false;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    sourceClock = CLOCK_GetFreq(I2C_MASTER_CLK_SRC);

    status = I2C_RTOS_Init(&master_rtos_handle, EXAMPLE_I2C_MASTER, &masterConfig, sourceClock);
    if (status != kStatus_Success)
    {
        PRINTF("I2C master: error during init, %d", status);
    }

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = g_master_buff;
    masterXfer.dataSize = I2C_DATA_LENGTH;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_RTOS_Transfer(&master_rtos_handle, &masterXfer);
    if (status != kStatus_Success)
    {
        PRINTF("I2C master: error during write transaction, %d", status);
    }

    /* Set up master to receive data from slave. */

    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = 0;
    }

    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = g_master_buff;
    masterXfer.dataSize = I2C_DATA_LENGTH;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_RTOS_Transfer(&master_rtos_handle, &masterXfer);
    if (status != kStatus_Success)
    {
        PRINTF("I2C master: error during read transaction, %d", status);
    }

    /* Transfer completed. Check the data. */
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occured in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("Master received data :");
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    vTaskSuspend(NULL);
}
