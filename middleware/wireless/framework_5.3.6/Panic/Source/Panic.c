/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the Panic module.
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

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "Panic.h"
#include "fsl_os_abstraction.h"

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
#if gUsePanic_c
panicData_t panic_data;
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  This function will halt the system
*
* \param[in]  id Description of the param2 in parameter
* \param[in]  location address where the Panic occured
* \param[in]  extra1 parameter to be stored in Panic structure
* \param[in]  extra2 parameter to be stored in Panic structure
*
********************************************************************************** */

void panic( panicId_t id, uint32_t location, uint32_t extra1, uint32_t extra2 )
{
#if gUsePanic_c
    /* Save the Link Register */
    volatile uint32_t savedLR = 0;
    __asm("push {r2}  ");
    __asm("push {LR} ");
    __asm("pop  {r2} ");
    __asm("str  r2, [SP, #4]");
    __asm("pop {r2}");

    panic_data.id = id;
    panic_data.location = location;
    panic_data.extra1 = extra1;
    panic_data.extra2 = extra2;
    panic_data.linkRegister = savedLR;
    panic_data.cpsr_contents   = 0;

    OSA_InterruptDisable(); /* disable interrupts */

    /* infinite loop just to ensure this routine never returns */
    for(;;)
    {
        __asm("NOP");
    }
#endif
}

#if defined(__GNUC__)
void __attribute__((weak)) __assertion_failed(char* s) {}
#endif