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
* @file data_format_service.h
* @brief The format_service.h file describes the  interface and structures for the data-format standards.
*/

#ifndef DATA_FORMAT_SERVICE_H_
#define DATA_FORMAT_SERVICE_H_
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define DATA_FORMAT_OK 0 ///< Operation succeeded

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @brief This defines the communication handle.
 */
typedef struct _comm_handle_
{
    void *pComm;     /*!< pointer to a specific communication channel.*/
    uint32_t status; /*!< Current Comm status.*/
} comm_handle_t;

/*  Event function signature.*/
typedef void (*DATA_FORMAT_Event_t)(void *pData); /*< Pointer to data format Event.*/

/*! @brief       The interface function to initialize the data-format service.*/
typedef int32_t(DATA_FORMAT_Init_t)(DATA_FORMAT_Event_t event, void *pInitializeData);
/*! @brief       The interface function to serialize the data*/
typedef int32_t(DATA_FORMAT_Serialize_t)(void *pData, uint32_t size, void *pDataElement);
/*! @brief       The interface function to deserialize the data.*/
typedef int32_t(DATA_FORMAT_Deserialize_t)(void *pData, uint32_t size, void *pDataElement);
/*! @brief       The interface function to append the data on the formated stream*/
typedef int32_t (DATA_FORMAT_Append_t))        (void * pData, uint32_t size, void* pDataElement));

/* @brief This defines the function pointers for the data format interface.
 */
typedef struct _data_format_interface_
{
    DATA_FORMAT_Init_t *Init;               /*!< Pointer to the data-format Initialize() function. */
    DATA_FORMAT_Serialize_t *Serialize;     /*!< Pointer to the data-format Serialize() function. */
    DATA_FORMAT_Deserialize_t *Deserialize; /*!< Pointer to the data-format Deserialize() function. */
    DATA_FORMAT_Append_t *Append;           /*!< Pointer to the data-format Append() function. */
} data_format_interface_t;
#endif // DATA_FORMAT_SERVICE_H_
