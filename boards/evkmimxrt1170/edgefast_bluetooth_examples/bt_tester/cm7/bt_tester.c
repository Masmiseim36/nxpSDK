/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_component_serial_manager.h"
#include "fsl_debug_console.h"
#include "fsl_adapter_reset.h"

#include "bt_tester.h"
#include "ble_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Tester application local information */
typedef struct bt_tester_info_tag
{
    SERIAL_MANAGER_WRITE_HANDLE_DEFINE(serialWriteHandleBuffer);
    uint32_t serialReadHandleBuffer[((SERIAL_MANAGER_READ_HANDLE_SIZE - 1) >> 2) + 1];
    serial_read_handle_t  serialReadHandle;
    serial_write_handle_t serialWriteHandle;
    uint8_t recvBuffer[BTP_MTU];
    uint32_t recvLength;
    uint8_t *dataToSend;
    uint32_t dataToSendLength;
    uint8_t  flagPachetReceived;
} bt_tester_info_t;

#define DATA_CLEANUP_PERIOD_MS    5000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void bt_tester_rx_callback
(
    void *callbackParam,
    serial_manager_callback_message_t *message,
    serial_manager_status_t status
);

static void bt_tester_tx_callback
(
    void *callbackParam,
    serial_manager_callback_message_t *message,
    serial_manager_status_t status
);

static void cmd_handler(void* param);

static void handle_core
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
);

static void supported_commands(uint8_t *data, uint16_t len);
static void supported_services(uint8_t *data, uint16_t len);
static void register_service(uint8_t *data, uint16_t len);
static void unregister_service(uint8_t *data, uint16_t len);

static void data_cleanup_timeout_handler(TimerHandle_t timer_id);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bt_tester_info_t bt_tester_info;

/* Command queue */
OSA_MSGQ_HANDLE_DEFINE(cmds_queue, CMD_QUEUED, sizeof(void *));

static QueueHandle_t signal_packet_received;

NET_BUF_POOL_FIXED_DEFINE(buf_pool, CMD_QUEUED, sizeof(btp_hdr_t), NULL);

static TimerHandle_t data_cleanup_timer = NULL;
static bool timerStarted = false;
static long xHigherPriorityTaskWoken;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * @brief   Application entry point.
 */
void bt_tester_task(void *argument)
{
    status_t status;
    osa_status_t ret;
    extern serial_handle_t g_serialHandle;
    assert(NULL != g_serialHandle);

    struct net_buf *buf;

    OSA_SR_ALLOC();

    /* Initialize local data */
    bt_tester_info.dataToSend = NULL;
    bt_tester_info.recvLength = 0;
    bt_tester_info.dataToSendLength = 0;
    for (uint32_t idx = 0; idx < BTP_MTU; idx++)
    {
        bt_tester_info.recvBuffer[idx] = 0;
    }

    /* Register serial handle for reading */
    bt_tester_info.serialReadHandle = (serial_read_handle_t)&bt_tester_info.serialReadHandleBuffer[0];
    status = SerialManager_OpenReadHandle(g_serialHandle, bt_tester_info.serialReadHandle);
    assert(kStatus_SerialManager_Success == status);
    (void)status;
    SerialManager_InstallRxCallback(bt_tester_info.serialReadHandle, bt_tester_rx_callback, &bt_tester_info);

    signal_packet_received = xSemaphoreCreateCounting(0xFFu, 0u);
    if (NULL == signal_packet_received)
    {
        PRINTF("faile to create signal_packet_received\n");
    }

    /* Create command queue */
    ret = OSA_MsgQCreate(cmds_queue, CMD_QUEUED, sizeof(void *));
    if (ret)
    {
        PRINTF("Failed to create app msg queue (err %d)\n", ret);
        return;
    }

    for (int i = 0; i < CMD_QUEUED; i++) {
        buf = net_buf_alloc(&buf_pool, osaWaitForever_c);
        net_buf_put(cmds_queue, buf);
	}

    uint16_t packetLength = 0;
    /* Open serial handle for writes */
    status = SerialManager_OpenWriteHandle(g_serialHandle,
                                           ((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]));
    assert(kStatus_SerialManager_Success == status);
    (void)status;
    SerialManager_InstallTxCallback(((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]),
                                    bt_tester_tx_callback, &bt_tester_info);

    /* Crete data cleanup timer */
    if (data_cleanup_timer == NULL)
    {
        data_cleanup_timer = xTimerCreate("data cleanup", (DATA_CLEANUP_PERIOD_MS / portTICK_PERIOD_MS),
                                      pdFALSE, NULL, data_cleanup_timeout_handler);
    }

    /* Inform IUT that the device is ready */
    tester_send(BTP_SERVICE_ID_CORE, CORE_EV_IUT_READY, BTP_INDEX_NONE,
		        NULL, 0);

    if (NULL != signal_packet_received)
    {
        while(1)
        {
            /* Wait for packet from the serial */
            if (pdTRUE == xSemaphoreTake(signal_packet_received, portMAX_DELAY))
            {
                /* Packet received get one available buffer */
                buf = net_buf_get(cmds_queue, osaWaitForever_c);
                if(buf != NULL)
                {
                    /* Calculate the packet length */
                    packetLength = bt_tester_info.recvBuffer[3] + (bt_tester_info.recvBuffer[4] << 0x08U);
                    /* Put the received data in the queue for the command handling */
                    net_buf_add_mem(buf, bt_tester_info.recvBuffer, (packetLength + CMD_BTP_HDR_SIZE));

                    OSA_ENTER_CRITICAL();
                    bt_tester_info.flagPachetReceived = 0;
                    if((packetLength + CMD_BTP_HDR_SIZE) == bt_tester_info.recvLength)
                    {
                        bt_tester_info.recvLength = 0;
                    }
                    else if((packetLength + CMD_BTP_HDR_SIZE) < bt_tester_info.recvLength)
                    {
                        memcpy(&bt_tester_info.recvBuffer[0], &bt_tester_info.recvBuffer[(packetLength + CMD_BTP_HDR_SIZE)], bt_tester_info.recvLength - (packetLength + CMD_BTP_HDR_SIZE));
                        bt_tester_info.recvLength = bt_tester_info.recvLength - (packetLength + CMD_BTP_HDR_SIZE);
                    }

                    OSA_EXIT_CRITICAL();
                    cmd_handler((btp_hdr_t *)(buf->data));
                    net_buf_reset(buf);
                    net_buf_put(cmds_queue, buf);
                }

            }
        }
    }
}

/*
 * @brief   Serial Rx Callback
 */
static void bt_tester_rx_callback
(
    void *callbackParam,
    serial_manager_callback_message_t *message,
    serial_manager_status_t status
)
{
    uint32_t             count = 0;
    uint8_t              recvChar[BTP_MTU];
    uint8_t              going = 0;
    uint16_t             packetLength = 0;
    OSA_SR_ALLOC();

    do
    {
        /* Read the data from the serial and copy it to the local buffer */
        if ((kStatus_SerialManager_Success != SerialManager_TryRead(bt_tester_info.serialReadHandle, &recvChar[0], sizeof(recvChar), &count)) || (0 == count))
        {
            break;
        }

        /* Copy locally the received data */
        OSA_ENTER_CRITICAL();
        for (int i = 0; i < count; i++)
        {
            if (bt_tester_info.recvLength < BTP_MTU)
            {
                bt_tester_info.recvBuffer[bt_tester_info.recvLength] = recvChar[i];
                bt_tester_info.recvLength ++;
                going = 1;
            }
            else
            {
                break;
            }
        }
        OSA_EXIT_CRITICAL();
    } while (going);
    /* Verify if a packet is in process */
    if(bt_tester_info.flagPachetReceived == 0x0)
    {
        /* Verify if in the buffer are minimum size for a packet */
        if(bt_tester_info.recvLength >= 0x05)
        {
            /* Calculate the packet length */
            packetLength = bt_tester_info.recvBuffer[3] + (bt_tester_info.recvBuffer[4] << 0x08U);
            /* Verify if the packet length is bigger then the maximum size of a packet */
            if (packetLength > sizeof(btp_hdr_t))
            {
                /* Junk data reset teh receive buffer */
                OSA_ENTER_CRITICAL();
                bt_tester_info.recvLength = 0;
                OSA_EXIT_CRITICAL();
                return;
            }

            if (packetLength != 0x0U)
            {
                /* Packet contains header and data. Start data cleanup timer in case of errors */
                if (timerStarted == false)
                {
                    xTimerStartFromISR(data_cleanup_timer, &xHigherPriorityTaskWoken);
                    timerStarted = true;
                }

                /* Verify if all the data from the packet has been received */
                if (bt_tester_info.recvLength >= (packetLength + CMD_BTP_HDR_SIZE))
                {
                    bt_tester_info.flagPachetReceived = 1;
                    timerStarted = false;
                    xTimerStopFromISR(data_cleanup_timer, &xHigherPriorityTaskWoken);
                }
            }
            else
            {
                /* Packet contains only header - no more data to be received */
                bt_tester_info.flagPachetReceived = 1;
            }
        }

        if (bt_tester_info.flagPachetReceived > 0)
        {
            if (NULL != signal_packet_received)
            {
                xSemaphoreGiveFromISR(signal_packet_received, &xReturn);
            }
        }
    }
}

static void data_cleanup_timeout_handler(TimerHandle_t timer_id)
{
    OSA_SR_ALLOC();

    /* Junk data reset the receive buffer */
    OSA_ENTER_CRITICAL();
    bt_tester_info.recvLength = 0;
    timerStarted = false;
    OSA_EXIT_CRITICAL();
}

/*
 * @brief   Serial Tx Callback
 */
static void bt_tester_tx_callback
(
    void *callbackParam,
    serial_manager_callback_message_t *message,
    serial_manager_status_t status
)
{
    /* If there was event data to send to the upper tester do it now */
    if (bt_tester_info.dataToSend != NULL)
    {
        OSA_SR_ALLOC();
        OSA_ENTER_CRITICAL();
        Utils_RevertByteArray(bt_tester_info.dataToSend, bt_tester_info.dataToSendLength);
        SerialManager_WriteBlocking(((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]), bt_tester_info.dataToSend, bt_tester_info.dataToSendLength);
        bt_tester_info.dataToSend = NULL;
        bt_tester_info.dataToSendLength = 0;
        OSA_EXIT_CRITICAL();
    }
}

/*
 * @brief    Send information to the upper tester
 */
void tester_send
(
    uint8_t service,
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    size_t len
)
{
    uint8_t *dataToSend = malloc(CMD_BTP_HDR_SIZE + len);
    uint8_t *buff = dataToSend;
    OSA_SR_ALLOC();

    if (dataToSend != NULL)
    {
        /* Copy the data to be sent to the upper tester in little endian orer */
        Utils_Copy8(buff, service);
        Utils_Copy8(buff, opcode);
        Utils_Copy8(buff, index);
        Utils_Copy16(buff, len);

        OSA_ENTER_CRITICAL();

        if (data != NULL)
        {
            memcpy(buff, data, len);
        }
        OSA_EXIT_CRITICAL();

        /* Send response to upper tester */
        SerialManager_WriteBlocking(((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]), dataToSend, CMD_BTP_HDR_SIZE + len);
    }

    free(dataToSend);
}

/*
 * @brief    Send response to upper tester
 */
void tester_rsp(uint8_t service, uint8_t opcode, uint8_t index, uint8_t status)
{
    btp_status_t s;

    if (status == BTP_STATUS_SUCCESS) {
        tester_send(service, opcode, index, NULL, 0);
    }
    else
    {
        s.code = status;
        tester_send(service, BTP_STATUS, index, (uint8_t *) &s, sizeof(s));
    }
}

/*
 * @brief   Processes requests received over uart
 */
static void cmd_handler(void* param)
{
    btp_hdr_t *cmd = (btp_hdr_t *)param;

    switch (cmd->service) {
    case BTP_SERVICE_ID_CORE:
        handle_core(cmd->opcode, cmd->index, cmd->data, cmd->len);
        break;
    case BTP_SERVICE_ID_GAP:
        tester_handle_gap(cmd->opcode, cmd->index, cmd->data, cmd->len);
        break;
    case BTP_SERVICE_ID_GATT:
        tester_handle_gatt(cmd->opcode, cmd->index, cmd->data, cmd->len);
        break;
    case BTP_SERVICE_ID_L2CAP:
        tester_handle_l2cap(cmd->opcode, cmd->index, cmd->data, cmd->len);
        break;

    default:
        PRINTF("Unknown service: 0x%x", cmd->service);
        tester_rsp(cmd->service, cmd->opcode, cmd->index, BTP_STATUS_FAILED);
        break;
    }
}

/*
 * @brief   Core command handler
 */
static void handle_core
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
)
{
    if (index != BTP_INDEX_NONE) {
        PRINTF("index != BTP_INDEX_NONE: 0x%x", index);
        tester_rsp(BTP_SERVICE_ID_CORE, opcode, index, BTP_STATUS_FAILED);
        return;
    }

    switch (opcode)
    {
    case CORE_READ_SUPPORTED_COMMANDS:
        supported_commands(data, len);
        return;

    case CORE_READ_SUPPORTED_SERVICES:
        supported_services(data, len);
        return;

    case CORE_REGISTER_SERVICE:
        register_service(data, len);
        return;

    case CORE_UNREGISTER_SERVICE:
        unregister_service(data, len);
        return;

    case CORE_RESET_BOARD:
        HAL_ResetMCU();
        break;

    default:
        PRINTF("unknown opcode: 0x%x", opcode);
        tester_rsp(BTP_SERVICE_ID_CORE, opcode, BTP_INDEX_NONE,
               BTP_STATUS_UNKNOWN_CMD);
        return;
    }
}

/*
 * @brief   Read Supported Commands handler
 *          Each bit in response is a flag indicating if command with
 *          opcode matching bit number is supported.
 */
static void supported_commands(uint8_t *data, uint16_t len)
{
    uint8_t buf[1];
    core_read_supported_commands_rp_t *rp = (void *) buf;

    (void)memset(buf, 0, sizeof(buf));

    tester_set_bit(buf, CORE_READ_SUPPORTED_COMMANDS);
    tester_set_bit(buf, CORE_READ_SUPPORTED_SERVICES);
    tester_set_bit(buf, CORE_REGISTER_SERVICE);
    tester_set_bit(buf, CORE_UNREGISTER_SERVICE);

    tester_send(BTP_SERVICE_ID_CORE, CORE_READ_SUPPORTED_COMMANDS,
            BTP_INDEX_NONE, (uint8_t *) rp, sizeof(buf));
}

/*
 * @brief   Read Supported Services handler
 *          Each bit in response is a flag indicating if service with
 *          ID matching bit number is supported.
 */
static void supported_services(uint8_t *data, uint16_t len)
{
    uint8_t buf[1];
    core_read_supported_services_rp_t *rp = (void *) buf;

    (void)memset(buf, 0, sizeof(buf));

    tester_set_bit(buf, BTP_SERVICE_ID_CORE);
    tester_set_bit(buf, BTP_SERVICE_ID_GAP);
    tester_set_bit(buf, BTP_SERVICE_ID_GATT);
    tester_set_bit(buf, BTP_SERVICE_ID_L2CAP);

    tester_send(BTP_SERVICE_ID_CORE, CORE_READ_SUPPORTED_SERVICES,
            BTP_INDEX_NONE, (uint8_t *) rp, sizeof(buf));
}

/*
 * @brief   Register Service command handler
 *          Reject command for an undeclared service ID.
 *          There will be no events for undeclaredservice ID.
 */
static void register_service(uint8_t *data, uint16_t len)
{
    core_register_service_cmd_t *cmd = (void *) data;
    uint8_t status;

    switch (cmd->id) {
    case BTP_SERVICE_ID_GAP:
        status = tester_init_gap();
        /* Rsp with success status will be handled by bt enable cb */
        if (status == BTP_STATUS_FAILED) {
                goto rsp;
        }
        return;

    case BTP_SERVICE_ID_GATT:
        status = tester_init_gatt();
        break;

    case BTP_SERVICE_ID_L2CAP:
        status = tester_init_l2cap();
        break;

    default:
        PRINTF("unknown id: 0x%02x", cmd->id);
        status = BTP_STATUS_FAILED;
        break;
    }

rsp:
    tester_rsp(BTP_SERVICE_ID_CORE, CORE_REGISTER_SERVICE, BTP_INDEX_NONE,
           status);
}

/*
 * @brief   Unregister  Service command handler
 *
 */
static void unregister_service(uint8_t *data, uint16_t len)
{
    core_unregister_service_cmd_t *cmd = (void *) data;
    uint8_t status;

    switch (cmd->id) {
    case BTP_SERVICE_ID_GAP:
        status = tester_unregister_gap();
        break;

    case BTP_SERVICE_ID_GATT:
        status = tester_unregister_gatt();
        break;

    case BTP_SERVICE_ID_L2CAP:
        status = tester_unregister_l2cap();
        break;

    default:
        PRINTF("unknown id: 0x%x", cmd->id);
        status = BTP_STATUS_FAILED;
        break;
    }

     if (status == BTP_STATUS_FAILED)
     {
         tester_rsp(BTP_SERVICE_ID_CORE, CORE_UNREGISTER_SERVICE, BTP_INDEX_NONE,
               status);
     }
}