/*! *********************************************************************************
 * \defgroup SHELL GAP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the interface file for the GAP Shell module
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

#ifndef _SHELL_GAP_H_
#define _SHELL_GAP_H_

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
#include "l2ca_interface.h"

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int8_t  ShellGap_Command(uint8_t argc, char * argv[]);
void    ShellGap_GenericCallback (gapGenericEvent_t* pGenericEvent);

void ShellGap_L2caControlCallback
(
    l2capControlMessageType_t  messageType,
    void* pMessage
);

#ifdef __cplusplus
}
#endif 


#endif /* _SHELL_GAP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
