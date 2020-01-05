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
/**
* @file host_interface_service.c
* @brief The dhost_interface_servicec file implements host interface functions
*/
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "host_interface_service.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern comm_interface_t commUART;

/*******************************************************************************
 * Functions
 *****************************************************************************/
int32_t HOST_Initialize(host_interface_handle_t *pHandle,
                        comm_type_t type,
                        void *pCommInstance,
                        comm_instance_type_t inType,
                        Host_Event_t event,
                        void *pInData)
{
    int32_t status;

    if (type == COMM_UART)
    {
        pHandle->pCommInterface = &commUART;
    }
    comm_interface_t *pCommInt = pHandle->pCommInterface;
    pHandle->event = event;
    pHandle->pInData = pInData;
    pHandle->instanceType = inType;
    // call interface function with appropriate comm instance.
    status = pCommInt->Init(&pHandle->commHandle, (void *)pCommInstance, pHandle->event, pHandle->pInData);

    return status;
}

int32_t HOST_Configure(host_interface_handle_t *pHandle, void *pConfigData)
{
    int32_t status;

    comm_interface_t *pCommInt = pHandle->pCommInterface;
    status = pCommInt->Configure(&pHandle->commHandle, pConfigData);
    return status;
}

int32_t HOST_Send(host_interface_handle_t *pHandle, uint8_t *pData, uint32_t size)
{
    int32_t status;

    comm_interface_t *pCommInt = pHandle->pCommInterface;
    status = pCommInt->Send(&pHandle->commHandle, pData, size);
    return status;
}

int32_t HOST_Receive(
    host_interface_handle_t *pHandle, uint8_t *pData, uint32_t *pRecvSize, uint32_t size, BlockRead_t process)
{
    int32_t status;

    comm_interface_t *pCommInt = pHandle->pCommInterface;
    // Call Host Data format specified callback for block read data
    if ((process != NULL) && (pHandle->instanceType == COMM_BLOCKING))
    {
        process(pHandle, pData);
        status = HOST_INTERFACE_OK;
    }
    else
    {
        status = pCommInt->Receive(&pHandle->commHandle, pData, size);
    }
    return status;
}
