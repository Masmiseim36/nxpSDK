/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
 * Include
 ********************************************************************************** */
#include "board.h"
#include "fsl_component_mem_manager.h"
#include "fsl_component_serial_manager.h"
#include "fsl_os_abstraction.h"
#include "FunctionLib.h"
#include "ipc.h"

/*! *********************************************************************************
 * Private definitions
 ********************************************************************************** */

/*! *********************************************************************************
 * Private memory declarations
 ********************************************************************************** */
OSA_SEMAPHORE_HANDLE_DEFINE(gIpcDataBufferingSem);
static SERIAL_MANAGER_READ_HANDLE_DEFINE(s_ipcReadHandle);
uint8_t                 rx_buffer[IPC_BUFFER_SIZE];
ipcTransportInterface_t mIpcInterface     = NULL;
static serial_handle_t  s_ipcSerialHandle = NULL;

/*! *********************************************************************************
 * Private functions prototypes
 ********************************************************************************** */
static void Ipc_SerialFreePacket(void *                             pPacket,
                                 serial_manager_callback_message_t *message,
                                 serial_manager_status_t            status);
static void Ipc_RxCallBack(void *                             callbackParam,
                           serial_manager_callback_message_t *message,
                           serial_manager_status_t            status);
static void Ipc_FreePacket(void *pPacket);

/*! *********************************************************************************
 * Public functions
 ********************************************************************************** */
void Ipc_Init(serial_handle_t                g_IpcSerialHandle,
              const serial_manager_config_t *s_ipcSerialManagerConfig,
              ipcTransportInterface_t        interface)
{
    osa_status_t            osa_status;
    serial_manager_status_t serial_status;

    osa_status = OSA_SemaphoreCreate(gIpcDataBufferingSem, 0);
    assert(osa_status == KOSA_StatusSuccess);

    s_ipcSerialHandle = g_IpcSerialHandle;

    /* Init Serial Manager */
    serial_status = SerialManager_Init((serial_handle_t)s_ipcSerialHandle, s_ipcSerialManagerConfig);
    assert(serial_status == kStatus_SerialManager_Success);

    serial_status =
        SerialManager_OpenReadHandle((serial_handle_t)s_ipcSerialHandle, (serial_read_handle_t)s_ipcReadHandle);
    assert(serial_status == kStatus_SerialManager_Success);

    (void)SerialManager_InstallRxCallback((serial_read_handle_t)s_ipcReadHandle, Ipc_RxCallBack, NULL);

    mIpcInterface = interface;

#ifndef KW45B41Z83_NBU_SERIES
    assert(mIpcInterface != NULL);
#endif
}

int Ipc_SendPacket(void *pPacket, uint16_t packetSize)
{
    uint8_t *pSerialPacket = NULL;
    int      result        = 0;

    uint8_t *               pPacketBuffer;
    uint8_t *               ipcWriteHandle;
    serial_manager_status_t status = kStatus_SerialManager_Success;

    pPacketBuffer = MEM_BufferAlloc(SERIAL_MANAGER_WRITE_HANDLE_SIZE + (uint32_t)packetSize);
    if (pPacketBuffer == NULL)
    {
        result = -2;
    }
    ipcWriteHandle = pPacketBuffer;
    pSerialPacket  = pPacketBuffer + SERIAL_MANAGER_WRITE_HANDLE_SIZE;

    if ((result == 0) && (NULL != pSerialPacket))
    {
        FLib_MemCpy(pSerialPacket, (uint8_t *)pPacket, packetSize);

        if (kStatus_SerialManager_Success !=
            SerialManager_OpenWriteHandle((serial_handle_t)s_ipcSerialHandle, (serial_write_handle_t)ipcWriteHandle))
        {
            Ipc_FreePacket(pPacketBuffer);
            return -1;
        }

        (void)SerialManager_InstallTxCallback((serial_write_handle_t)ipcWriteHandle, Ipc_SerialFreePacket,
                                              pPacketBuffer);
        status = SerialManager_WriteNonBlocking((serial_write_handle_t)ipcWriteHandle, pSerialPacket, packetSize);

        if (kStatus_SerialManager_Success != status)
        {
            Ipc_FreePacket(pPacketBuffer);
            result = -1;
        }
    }
    else
    {
        result = -2;
    }
    assert(result == 0);

    return result;
}

/*! *********************************************************************************
 * Private functions
 ********************************************************************************** */
static void Ipc_RxCallBack(void *                             callbackParam,
                           serial_manager_callback_message_t *message,
                           serial_manager_status_t            status)
{
    serial_manager_status_t serial_status;
    uint32_t                count = 0U;

    assert(message->length <= IPC_BUFFER_SIZE);

    serial_status = SerialManager_TryRead((serial_read_handle_t)s_ipcReadHandle, rx_buffer, message->length, &count);
    assert(serial_status == kStatus_SerialManager_Success);
    assert(count == message->length);

    /* Send packet to registered interface */
    mIpcInterface(rx_buffer, message->length);
}

static void Ipc_SerialFreePacket(void *                             pPacket,
                                 serial_manager_callback_message_t *message,
                                 serial_manager_status_t            status)
{
    SerialManager_CloseWriteHandle((serial_write_handle_t)pPacket);
    (void)MEM_BufferFree(pPacket);
}

static void Ipc_FreePacket(void *pPacket)
{
    (void)MEM_BufferFree(pPacket);
}
