/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IPC_H_
#define _IPC_H_

/*! *********************************************************************************
 * Include
 ********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_component_serial_manager.h"

/*! *********************************************************************************
 * Public definitions
 ********************************************************************************** */
#ifndef IPC_BUFFER_SIZE
#define IPC_BUFFER_SIZE 10U
#endif

/*! *********************************************************************************
 * Public type definitions
 ********************************************************************************** */
typedef void (*ipcTransportInterface_t)(void *data, uint32_t size);

/*! *********************************************************************************
 * Public memory declarations
 ********************************************************************************** */

/*! *********************************************************************************
 * Public functions
 ********************************************************************************** */
void Ipc_Init(serial_handle_t                g_IpcSerialHandle,
              const serial_manager_config_t *s_ipcSerialManagerConfig,
              ipcTransportInterface_t        interface);
int  Ipc_SendPacket(void *pPacket, uint16_t packetSize);

#endif /* _IPC_H_ */
