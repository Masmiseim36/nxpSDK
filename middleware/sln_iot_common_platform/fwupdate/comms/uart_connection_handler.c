/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "common_connection_handler.h"

#include "task.h"
#include "fsl_lpuart.h"
#include "sln_RT10xx_RGB_LED_driver.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
TaskHandle_t uartCommsTaskHandle        = NULL;
static sln_common_connection_desc *desc = NULL;

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
static void SLN_UART_COMMS_ServerTask(void *args);
static sln_common_connection_message_status SLN_UART_COMMS_Write(sln_common_connection_write_context *writeContext);
static sln_common_connection_message_status SLN_UART_COMMS_Read(uint32_t fd);

static int32_t SLN_UART_COMMS_WriteData(uint32_t fd, uint8_t *buf, uint16_t size);
static int32_t SLN_UART_COMMS_ReadData(uint32_t fd, uint8_t *buf, uint16_t size);

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
sln_common_connection_message_status SLN_UART_COMMS_Init(sln_common_connection_desc *descriptor)
{
    sln_common_connection_message_status status = kCommon_Success;
    BaseType_t xReturned;
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = true;
     * config.enableRx = true;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = descriptor->context.sUartContext.speed_bps;
    config.enableTx     = true;
    config.enableRx     = true;

    if (kStatus_Success != LPUART_Init((LPUART_Type *)descriptor->context.sUartContext.portbase, &config,
                                       descriptor->context.sUartContext.clksrc_hz))
    {
        status = kCommon_Failed;
    }

    if (status == kCommon_Success)
    {
        desc        = descriptor;
        desc->write = &SLN_UART_COMMS_Write;

        xReturned = xTaskCreate(SLN_UART_COMMS_ServerTask, "UART_Comms_Server", 1024, descriptor,
                                configMAX_PRIORITIES - 3, &uartCommsTaskHandle);
    }

    return xReturned;
}

/**
 * @brief The UART handler Task
 *
 * @param args: Task arguments
 *
 */
static void SLN_UART_COMMS_ServerTask(void *args)
{
    sln_common_connection_desc *descriptor = args;

    assert(NULL != descriptor);
    configPRINTF(("[%s] Starting UART Connection handler processor\r\n", __FUNCTION__));

    for (;;)
    {
        /* Let the user know we are in OTW mode */
        RGB_LED_SetColor(LED_COLOR_BLUE);
        SLN_UART_COMMS_Read(descriptor->context.sUartContext.portbase);

        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}

/**
 * @brief Write to the UART port
 *
 * @param sln_common_connection_write_context: Data and client context
 *
 * @return          sln_comms_message_status
 */

static sln_common_connection_message_status SLN_UART_COMMS_Write(sln_common_connection_write_context *writeContext)
{
    sln_common_connection_message_status status = kCommon_Success;
    int32_t bytes_sent                          = 0;

    /* Send the length of the message */
    bytes_sent = SLN_UART_COMMS_WriteData(writeContext->connContext.sUartContext.fd, (uint8_t *)&writeContext->len, 4);

    /* If four bytes weren't sent, abort */
    if (bytes_sent != 4)
    {
        status = kCommon_Failed;
    }

    /* Send the data */
    if (status == kCommon_Success)
    {
        bytes_sent =
            SLN_UART_COMMS_WriteData(writeContext->connContext.sUartContext.fd, writeContext->data, writeContext->len);
    }

    /* TODO: Could possibly loop as the message could end up being large */
    if (bytes_sent != writeContext->len)
    {
        status = kCommon_Failed;
    }

    return status;
}

/**
 * @brief The function to read from the UART port
 *
 */
static sln_common_connection_message_status SLN_UART_COMMS_Read(uint32_t fd)
{
    sln_common_connection_message_status status = kCommon_Success;
    uint8_t *buff                               = NULL;

    configPRINTF(("[%s] Received Data from connected device\r\n", __FUNCTION__));
    int32_t packet_size = 0;

    /* Receive the header length information */
    int32_t len = SLN_UART_COMMS_ReadData(fd, (uint8_t *)&packet_size, 4);

    if (len == 0)
    {
        /* Could be connection issue, will read again later to check */
        configPRINTF(("[%s] Received no bytes\r\n", __FUNCTION__));
        status = kCommon_NoDataRead;
    }

    if (packet_size > desc->context.sUartContext.max_rx_buff_size)
    {
        /* Bounds checking to ensure we didn't get a bad length */
        configPRINTF(("[%s] Data too large\r\n", __FUNCTION__));
        status = kCommon_ToManyBytes;
    }

    if (kCommon_ToManyBytes == status)
    {
        /* TBD */
    }

    if (kCommon_Success == status)
    {
        /* Malloc the size of the expected message */
        buff = (uint8_t *)pvPortMalloc(packet_size);
        if (buff == NULL)
        {
            configPRINTF(("[%s] Failed to malloc\r\n", __FUNCTION__));
            status = kCommon_Failed;
        }
        else
        {
            /* Set the data to 0 to ensure potentially weird things don't occur */
            memset(buff, 0, packet_size);
        }
    }

    if (kCommon_Success == status)
    {
        configPRINTF(("[%s] Receiving data\r\n", __FUNCTION__));

        if (packet_size == SLN_UART_COMMS_ReadData(fd, buff, packet_size))
        {
            if (desc->recv_cb)
            {
                sln_common_connection_recv_context rxContext;

                rxContext.data                        = buff;
                rxContext.packet_size                 = packet_size;
                rxContext.connContext.sUartContext.fd = fd;

                desc->recv_cb(&rxContext);
            }
        }
        vPortFree(buff);
    }

    return status;
}

static int32_t SLN_UART_COMMS_WriteData(uint32_t fd, uint8_t *buf, uint16_t size)
{
    LPUART_WriteBlocking((LPUART_Type *)fd, buf, size);
    return size;
}

static int32_t SLN_UART_COMMS_ReadData(uint32_t fd, uint8_t *buf, uint16_t size)
{
    if (kStatus_Success == LPUART_ReadBlocking((LPUART_Type *)fd, buf, size))
    {
        return size;
    }
    return 0;
}
