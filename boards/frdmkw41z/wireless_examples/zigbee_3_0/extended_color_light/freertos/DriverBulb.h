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

#ifndef  DRIVERBULB_H_INCLUDED
#define  DRIVERBULB_H_INCLUDED

/*!
\file       DriverBulb.h
\brief      Driver Bulb generic - Interface
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/* SDK includes */
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

enum {E_RED_PWM,E_GREEN_PWM,E_BLUE_PWM,E_WHITE_PWM};
/****************************************************************************/
/***        Public Function Prototypes                                    ***/
/****************************************************************************/

/* Mandatory Interface functions */
PUBLIC void 		DriverBulb_vInit(void);
PUBLIC void 		DriverBulb_vOn(void);
PUBLIC void 		DriverBulb_vOff(void);
PUBLIC bool_t		DriverBulb_bOn(void);

PUBLIC void 	 	DriverBulb_vSetLevel(uint32 u32Level);
PUBLIC void         DriverBulb_vSetOnOff(bool_t bOn);
#if defined(__IAR_SYSTEMS_ICC__)
PUBLIC __weak void         DriverBulb_vSetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue);
PUBLIC __weak void         DriverBulb_vSetRGBWColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32White);

/* No need to stub these out -only DR1192/DR1221 build variants support these */
PUBLIC __weak void         DriverBulb_vSetBalance(uint8 u8Balance);
PUBLIC __weak void         DriverBulb_vSetTunableWhiteColourTemperature(int32 i32ColourTemperature);
PUBLIC __weak void         DriverBulb_vGetColourTempPhyMinMax(uint16 *pu16PhyMin, uint16 *pu16PhyMax);

/* Auxillary interface for DR1223 (AAL RGB) */
PUBLIC __weak uint16 DriverBulb_u16GetAdcValue(uint32 u32ChannelId);
PUBLIC __weak void DriverBulb_vSet12BitColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue);

#elif defined(__GNUC__)
PUBLIC void         DriverBulb_vSetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)__attribute__((weak));
PUBLIC void         DriverBulb_vSetRGBWColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32White)__attribute__((weak));

/* No need to stub these out -only DR1192/DR1221 build variants support these */
PUBLIC void         DriverBulb_vSetBalance(uint8 u8Balance)__attribute__((weak));
PUBLIC void         DriverBulb_vSetTunableWhiteColourTemperature(int32 i32ColourTemperature)__attribute__((weak));
PUBLIC void         DriverBulb_vGetColourTempPhyMinMax(uint16 *pu16PhyMin, uint16 *pu16PhyMax)__attribute__((weak));

/* Auxillary interface for DR1223 (AAL RGB) */
PUBLIC uint16 DriverBulb_u16GetAdcValue(uint32 u32ChannelId)__attribute__((weak));
PUBLIC void DriverBulb_vSet12BitColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)__attribute__((weak));
#endif

/* Optional Interface Functions                        */
/* Stub out in implementation if no behaviour required */
PUBLIC bool_t 	 	DriverBulb_bReady(void);
PUBLIC bool_t       DriverBulb_bFailed(void);
PUBLIC int16 		DriverBulb_i16Analogue(uint8 u8Adc, uint16 u16AdcRead);
PUBLIC void      	DriverBulb_vTick(void);
PUBLIC void			DriverBulb_vCbSetLevel(uint8 u8Level);




/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* DRIVERBULB_H_INCLUDED */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
