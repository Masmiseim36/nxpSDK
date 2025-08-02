/* bttester.c - Bluetooth Tester */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright (c) 2022 Codecoup
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <porting.h>

#include "fsl_component_serial_manager.h"
#include "fsl_debug_console.h"
#include "fsl_adapter_reset.h"

#if (defined(RW610_SERIES) || defined(RW612_SERIES))
#include "fsl_loader.h"
#endif /* RW610_SERIES & RW612_SERIES */

#include "btp/btp.h"
#include "ble_utils.h"

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

#define CMD_QUEUED 2

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

static uint8_t cmd_handler(void* param);

struct btp_buf {
    /* intptr_t _reserved; */
	union {
		uint8_t data[BTP_MTU];
		struct btp_hdr hdr;
	};
	uint8_t rsp[BTP_MTU];
};

static void data_cleanup_timeout_handler(TimerHandle_t timer_id);

static void tester_send_with_index(uint8_t service, uint8_t opcode, uint8_t index,
				   const uint8_t *data, size_t len);
static void tester_rsp_with_index(uint8_t service, uint8_t opcode, uint8_t index,
				  uint8_t status);

static bt_tester_info_t bt_tester_info;

/* Command queue */
static K_FIFO_DEFINE(cmds_queue);

static QueueHandle_t signal_packet_received;

NET_BUF_POOL_FIXED_DEFINE(buf_pool, CMD_QUEUED, sizeof(struct btp_buf), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static struct net_buf *delayed_cmd;

static struct {
	const struct btp_handler *handlers;
	size_t num;
} service_handler[BTP_SERVICE_ID_MAX + 1];

static struct net_buf_simple *rsp_buf = NET_BUF_SIMPLE(BTP_MTU);
static OSA_MUTEX_HANDLE_DEFINE(rsp_buf_mutex);

static TimerHandle_t data_cleanup_timer = NULL;
static bool timerStarted = false;
static long xHigherPriorityTaskWoken;

void tester_register_command_handlers(uint8_t service,
				      const struct btp_handler *handlers,
				      size_t num)
{
	__ASSERT_NO_MSG(service <= BTP_SERVICE_ID_MAX);
	__ASSERT_NO_MSG(service_handler[service].handlers == NULL);

	service_handler[service].handlers = handlers;
	service_handler[service].num = num;
}

static const struct btp_handler *find_btp_handler(uint8_t service, uint8_t opcode)
{
	if ((service > BTP_SERVICE_ID_MAX) ||
	    (service_handler[service].handlers == NULL)) {
		return NULL;
	}

	for (uint8_t i = 0; i < service_handler[service].num; i++) {
		if (service_handler[service].handlers[i].opcode == opcode) {
			return &service_handler[service].handlers[i];
		}
	}

	return NULL;
}

/*
 * @brief   Processes requests received over uart
 */
static uint8_t cmd_handler(void* param)
{
    const struct btp_handler *btp;
    struct btp_buf *cmd = (struct btp_buf *)param;
    uint8_t status;
    uint16_t rsp_len = 0;
    uint16_t len;

    len = sys_le16_to_cpu(cmd->hdr.len);

    btp = find_btp_handler(cmd->hdr.service, cmd->hdr.opcode);
    if (btp)
    {
        if (btp->index != cmd->hdr.index)
        {
            status = BTP_STATUS_FAILED;
        }
        else if ((btp->expect_len >= 0) && (btp->expect_len != len))
        {
            status = BTP_STATUS_FAILED;
        }
        else
        {
            status = btp->func(cmd->hdr.data, len,
						   cmd->rsp, &rsp_len);
        }

        __ASSERT_NO_MSG((rsp_len + sizeof(struct btp_hdr)) <= BTP_MTU);
    }
    else
    {
        status = BTP_STATUS_UNKNOWN_CMD;
    }
    if (status != BTP_STATUS_DELAY_REPLY)
    {
        if ((status == BTP_STATUS_SUCCESS) && rsp_len > 0)
        {
            tester_send_with_index(cmd->hdr.service, cmd->hdr.opcode,
                           cmd->hdr.index, cmd->rsp, rsp_len);
        }
        else
        {
            tester_rsp_with_index(cmd->hdr.service, cmd->hdr.opcode,
                          cmd->hdr.index, status);
        }
    }

    return status;
}

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

	status = SerialManager_InstallRxCallback(bt_tester_info.serialReadHandle, bt_tester_rx_callback, &bt_tester_info);
	assert(kStatus_SerialManager_Success == status);
	(void)status;

	signal_packet_received = xSemaphoreCreateCounting(0xFFu, 0u);
	if (NULL == signal_packet_received)
	{
		PRINTF("faile to create signal_packet_received\n");
	}

	/* Create rsp buf mutex */
	ret = OSA_MutexCreate((osa_mutex_handle_t)rsp_buf_mutex);
	if (ret)
	{
		PRINTF("Failed to create rsp buf mutex (err %d)\n", ret);
		return;
	}

	/* Create command queue */
	k_fifo_init(&cmds_queue);

	for (int i = 0; i < CMD_QUEUED; i++) {
		buf = net_buf_alloc(&buf_pool, osaWaitForever_c);
		if (buf != NULL)
		{
			net_buf_put(&cmds_queue, buf);
		}
	}

	uint16_t packetLength = 0;
	/* Open serial handle for writes */
	status = SerialManager_OpenWriteHandle(g_serialHandle,
						((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]));
	assert(kStatus_SerialManager_Success == status);
	(void)status;

	status = SerialManager_InstallTxCallback(((serial_write_handle_t)&bt_tester_info.serialWriteHandleBuffer[0]),
					bt_tester_tx_callback, &bt_tester_info);
	assert(kStatus_SerialManager_Success == status);
	(void)status;

	/* Crete data cleanup timer */
	if (data_cleanup_timer == NULL)
	{
		data_cleanup_timer = xTimerCreate("data cleanup", (DATA_CLEANUP_PERIOD_MS / portTICK_PERIOD_MS),
					pdFALSE, NULL, data_cleanup_timeout_handler);
	}

	/* core service is always available */
	tester_init_core();

	/* Inform IUT that the device is ready */
	tester_send_with_index(BTP_SERVICE_ID_CORE, BTP_CORE_EV_IUT_READY,
				BTP_INDEX_NONE, NULL, 0);

	if (NULL != signal_packet_received)
	{
		while(1)
		{
			/* Wait for packet from the serial */
			if (pdTRUE == xSemaphoreTake(signal_packet_received, portMAX_DELAY))
			{
				/* Packet received get one available buffer */
				buf = net_buf_get(&cmds_queue, K_FOREVER);
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
					if(BTP_STATUS_DELAY_REPLY == cmd_handler((struct btp_buf *)(buf->data)))
					{
						/* Allow to delay only 1 command. This is for convenience only
						* of using cmd data without need of copying those in async
						* functions. Should be not needed eventually.
						*/
						__ASSERT_NO_MSG(delayed_cmd == NULL);
						delayed_cmd = buf;
					}
					else
					{
						net_buf_reset(buf);
						net_buf_put(&cmds_queue, buf);
					}
				}
			}
		}
	}
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
    static uint8_t       recvChar[BTP_MTU];
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
            if (packetLength > BTP_MTU)
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
                    (void)xTimerStartFromISR(data_cleanup_timer, &xHigherPriorityTaskWoken);
                    timerStarted = true;
                }

                /* Verify if all the data from the packet has been received */
                if (bt_tester_info.recvLength >= (packetLength + CMD_BTP_HDR_SIZE))
                {
                    bt_tester_info.flagPachetReceived = 1;
                    timerStarted = false;
                    (void)xTimerStopFromISR(data_cleanup_timer, &xHigherPriorityTaskWoken);
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
                (void)xSemaphoreGiveFromISR(signal_packet_received, &xReturn);
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

int tester_rsp_buffer_lock(void)
{
	if (OSA_MutexLock((osa_mutex_handle_t)rsp_buf_mutex, osaWaitForever_c) != 0) {
		LOG_ERR("Cannot lock rsp_ring_buf");

		return -EACCES;
	}

	return 0;
}

void tester_rsp_buffer_unlock(void)
{
	(void)OSA_MutexUnlock((osa_mutex_handle_t)rsp_buf_mutex);
}

void tester_rsp_buffer_free(void)
{
	net_buf_simple_init(rsp_buf, 0);
}

void tester_rsp_buffer_allocate(size_t len, uint8_t **data)
{
	tester_rsp_buffer_free();

	*data = net_buf_simple_add(rsp_buf, len);
}

/*
 * @brief    Send information to the upper tester
 */
static void tester_send_with_index(uint8_t service, uint8_t opcode, uint8_t index,
				   const uint8_t *data, size_t len)
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
static void tester_rsp_with_index(uint8_t service, uint8_t opcode, uint8_t index,
				  uint8_t status)
{
	struct btp_status s;

	if (status == BTP_STATUS_SUCCESS) {
		tester_send_with_index(service, opcode, index, NULL, 0);
    } else {
        s.code = status;
        tester_send_with_index(service, BTP_STATUS, index, (uint8_t *) &s, sizeof(s));
    }
}

void tester_event(uint8_t service, uint8_t opcode, const void *data, size_t len)
{
	__ASSERT_NO_MSG(opcode >= 0x80);
	tester_send_with_index(service, opcode, BTP_INDEX, data, len);
}

void tester_rsp_full(uint8_t service, uint8_t opcode, const void *rsp, size_t len)
{
    struct net_buf *buf;

	__ASSERT_NO_MSG(opcode < 0x80);
	__ASSERT_NO_MSG(delayed_cmd != NULL);

	tester_send_with_index(service, opcode, BTP_INDEX, rsp, len);

    buf = delayed_cmd;
	delayed_cmd = NULL;

	net_buf_reset(buf);
	net_buf_put(&cmds_queue, buf);
}

void tester_rsp(uint8_t service, uint8_t opcode, uint8_t status)
{
    struct net_buf *buf;

	__ASSERT_NO_MSG(opcode < 0x80);
	__ASSERT_NO_MSG(delayed_cmd != NULL);

	tester_rsp_with_index(service, opcode, BTP_INDEX, status);

	buf = delayed_cmd;
	delayed_cmd = NULL;

    net_buf_reset(buf);
    net_buf_put(&cmds_queue, buf);
}
