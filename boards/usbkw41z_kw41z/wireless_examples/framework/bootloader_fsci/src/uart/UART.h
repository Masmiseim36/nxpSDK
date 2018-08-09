/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __UART_H__
#define __UART_H__


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "main.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#ifndef gBoot_UseUart_d
#define gBoot_UseUart_d 1
#endif

#ifndef gBoot_UseUartCalibration_d
#define gBoot_UseUartCalibration_d 0
#endif


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
void uart_init(void (*calback)(uint32_t), uint32_t param);
uint16_t uart_read(uint8_t* pData, uint16_t count);
void uart_sync_write(uint8_t* pData, uint16_t count);

#endif
