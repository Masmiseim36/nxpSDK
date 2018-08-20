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

#ifndef __PWR_LIB_PLATFORM_H__
#define __PWR_LIB_PLATFORM_H__

/*****************************************************************************
 *                             PUBLIC MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define NO_WAKE_UP    (0)
#define LPTMR_WAKE_UP (1)
#define UART_WAKE_UP  (2)
#define GPIO_WAKE_UP  (3)
#define XCVR_IRQ_WAKE_UP (4)
#define UNKNOWN_WAKE_UP  (5)
#define PWRP_LLWU_WAKE_UP_PIN   (BOARD_LLWU_PIN_ENABLE_BITMAP)

/*****************************************************************************
 *                        PUBLIC TYPE DEFINITIONS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Return codes from the custom platform power modes */
typedef enum pwrp_status_tag
{
    PWRP_OK = 0,
    PWRP_ERR,
    PWRP_MAX
}pwrp_status_t;

/*****************************************************************************
 *                               PUBLIC VARIABLES(External)                  *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have global      *
 * (project) scope.                                                          *
 * These variables / constants can be accessed outside this module.          *
 * These variables / constants shall be preceded by the 'extern' keyword in  *
 * the interface header.                                                     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded (as a     *
 * good practice) by the keyword 'extern'                                    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
extern void PWRP_UpdateLPTMRCount(uint16_t u16TimeInMs);
extern void PWRP_Init(void);
extern pwrp_status_t PWRP_SetWakeUpSource(uint8_t u8WUSource);
extern pwrp_status_t Enter_Wait(void);
extern pwrp_status_t Enter_Stop(void);
extern pwrp_status_t Enter_RUN (void);
extern pwrp_status_t Enter_VLPR(void);
extern pwrp_status_t Enter_VLPW(void);
extern pwrp_status_t Enter_VLPS(void);
extern pwrp_status_t Enter_LLS (void);
extern pwrp_status_t Enter_VLLS(uint8_t u8SubMode);
extern uint8_t PWRP_GetWakeUpReason(void);

#endif /* __PWR_LIB_PLATFORM_H__ */

