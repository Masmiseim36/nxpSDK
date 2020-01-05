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
* @file comm_if_uart.c
* @brief The comm_if_uart.c file implements comm interface for the UART.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "Driver_USART.h"
#include "comm_interface.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t COMM_UART_GetCapabilities(comm_handle_t *pHandle)
{
    // @todo the implementation.
    return COMM_INTERFACE_OK;
}

int32_t COMM_UART_Init(comm_handle_t *pHandle, void *pCommInstance, COMM_Event_t event, void *pInitializeData)
{
    // @todo handle the pointers and validation properly.
    pHandle->pComm = pCommInstance;
    //@todo use the rest of argument properly.
    //SDK CMSIS driver adoption; init call will be done by the user application.
    //((ARM_DRIVER_USART *)(pHandle->pComm))->Initialize(event);
    return COMM_INTERFACE_OK;
}
int32_t COMM_UART_Config(comm_handle_t *pHandle, void *pConfigData)
{
    // @todo the implementation.
    comm_control_t *pControl = (comm_control_t*)pConfigData;
    return ((ARM_DRIVER_USART *)(pHandle->pComm))->Control(pControl->control, pControl->arg);
}
int32_t COMM_UART_Send(comm_handle_t *pHandle, void *pData, uint32_t size)
{
    //@todo the validation of the pointer
    return ((ARM_DRIVER_USART *)(pHandle->pComm))->Send(pData, size);
}
int32_t COMM_UART_Receive(comm_handle_t *pHandle, void *pData, uint32_t size)
{
    //@todo the validation of the pointer
    return ((ARM_DRIVER_USART *)(pHandle->pComm))->Receive(pData, size);
}
int32_t COMM_UART_GetStatus(comm_handle_t *pHandle)
{
    //@todo the validation of the pointer
    return pHandle->status;
}
// End USART Interface

comm_interface_t commUART = {
    COMM_UART_GetCapabilities, COMM_UART_Init, COMM_UART_Config, COMM_UART_Send, COMM_UART_Receive, COMM_UART_GetStatus,
};
