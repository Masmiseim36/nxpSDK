/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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

#ifndef APP_NTAG_H_
#define APP_NTAG_H_

/*!
\file       app_ntag.h
\brief      Light Bulb application layer for NTAG
*/
     
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <ntag.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define APP_NTAG_ADDRESS          0xFFU  /* I2C Address (0xFF for automatic detection) */

#if (defined OM15045) || (defined OM15053)
#define APP_NTAG_I2C_LOCATION     TRUE   /* DIO4 and DIO5 */
#else
#define APP_NTAG_I2C_LOCATION     FALSE  /* TRUE uses alternate I2C lines (DIO16, 17) instead of (DIO14, 15) */
#endif

#define APP_NTAG_I2C_FREQUENCY_HZ 100000 /* I2C prescale value */
#define APP_NTAG_TICK_MS          5      /* Interval of tick timer in ms */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t APP_bNtagPdmLoad(void);
PUBLIC void APP_vNtagStart(uint8 u8NscType);
PUBLIC void APP_vNtagStop(void);
PUBLIC void APP_cbNtagTimer(void *pvParams);
PUBLIC void APP_cbNtagEvent(            /* Called when an event takes place */
       teNtagEvent  eNtagEvent,         /* Event raised */
       uint32       u32Address,
       uint32       u32Length,
       uint8        *pu8Data);          /* Event data (NULL if no data) */

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_NTAG_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
