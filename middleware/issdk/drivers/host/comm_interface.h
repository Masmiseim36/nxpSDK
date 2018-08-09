/*
 * The Clear BSD License
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
* @file comm_interface.h
* @brief The comm_interface.h file describes the interface definition for the
 communication interface. Each commaunication instance needs to implement comm interface.
*/

#ifndef COMM_INTERFACE_H_
#define COMM_INTERFACE_H_
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define COMM_INTERFACE_OK 0 ///< Operation succeeded

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @brief This defines the communication interface handle
 */
typedef struct _comm_handle_
{
    void *pComm;     /*!< pointer to a specific communication channel.*/
    uint32_t status; /*!< Current Comm status.*/
} comm_handle_t;

/* @brief This defines the communication interface control config
 */
typedef struct _comm_control_
{
    uint32_t control; /*!< The Control TAG.*/
    uint32_t arg;     /*!< The Control Arguement value.*/
} comm_control_t;

/* @brief This defines the different commmunication types
 */
typedef enum _comm_type_
{
    COMM_UART,
    COMM_BLUETOOTH,
    COMM_I2C,
    COMM_SOCKET,
    COMM_NFC,
} comm_type_t;

/* @brief This defines the different commmunication types
 */
typedef enum _comm_instance_type_
{
    COMM_BLOCKING,    // Blocking Read and Write Calls
    COMM_NONBLOCKING, // NonBlocking Read and Write Calls
} comm_instance_type_t;

/*  Interface functions Signature.*/
typedef void (*COMM_Event_t)(uint32_t event); /*< Pointer to Comm Event.*/
/*! @brief       The interface function to get the capability of the communication interface.*/
typedef int32_t(COMM_GetCapabilities_t)(comm_handle_t *pHandle);
/*! @brief       The interface function to initialize the communication interface.*/
typedef int32_t(COMM_Init_t)(comm_handle_t *pHandle, void *pCommInstance, COMM_Event_t event, void *pInitializeData);
/*! @brief       The interface function to configure the communication interface.*/
typedef int32_t(COMM_Config_t)(comm_handle_t *pHandle, void *pConfigData);
/*! @brief       The interface function to send data through the communication interface.*/
typedef int32_t(COMM_Send_t)(comm_handle_t *pHandle, void *pData, uint32_t size);
/*! @brief       The interface function to receive data through the communication interface.*/
typedef int32_t(COMM_Receive_t)(comm_handle_t *pHandle, void *pData, uint32_t size);
/*! @brief       The interface function to get the status of the comm.*/
typedef int32_t(COMM_GetStatus_t)(comm_handle_t *pHandle);

/* @brief This defines the function pointers for the comm interface.
 */
typedef struct _comm_interface_
{
    COMM_GetCapabilities_t *GetCapabilities; /*!< Pointer to the communication GetCapabilities() function. */
    COMM_Init_t *Init;                       /*!< Pointer to the communication Initialize() function. */
    COMM_Config_t *Configure;                /*!< Pointer to the communication Configure() function. */
    COMM_Send_t *Send;                       /*!< Pointer to the communication send() function. */
    COMM_Receive_t *Receive;                 /*!< Pointer to the communication receive() function. */
    COMM_GetStatus_t *GetStatus;             /*!< Pointer to the adapter GetStatus() function. */
} comm_interface_t;
#endif // COMM_INTERFACE_H_
