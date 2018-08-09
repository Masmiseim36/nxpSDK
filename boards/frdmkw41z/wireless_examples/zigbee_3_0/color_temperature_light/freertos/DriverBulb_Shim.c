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

/*!=============================================================================
\file       DriverBulb_Shim.c
\brief      Shim functions to provide generic interface to upper layer
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/* SDK includes */
#include <jendefs.h>


/* Device includes */
#include "DriverBulb_Shim.h"
#include "DriverBulb.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Global Variables                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       		vBULB_Init,vBULB_SetOnOff,vBULB_SetLevel,vBULB_SetColour
 *
 * DESCRIPTION:		Shim functions to provide generic interface to upper layer
 *
 ****************************************************************************/
PUBLIC void vBULB_Init(void)
{
	DriverBulb_vInit();
}

PUBLIC void vBULB_SetOnOff(bool_t bOn)
{
	DriverBulb_vSetOnOff(bOn);
}

PUBLIC void vBULB_SetLevel(uint32 u32Level)
{
	DriverBulb_vSetLevel(u32Level);
}

PUBLIC void vBULB_SetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)
{
	if (DriverBulb_vSetColour)
	{
		DriverBulb_vSetColour(u32Red, u32Green, u32Blue);
	}
}

PUBLIC void vBULB_SetColourTemperature(uint32 u32ColTemp)
{
	if (DriverBulb_vSetTunableWhiteColourTemperature)
	{
		DriverBulb_vSetTunableWhiteColourTemperature(u32ColTemp);
	}
}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
