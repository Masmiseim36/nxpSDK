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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#if 0
/* Standard Include */
#include <jendefs.h>

/* On Chip Peripherals include */
#include <AppHardwareApi.h>

/*application Include */
#include "low_bat_indicator.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define DELAY_COUNT        (200000)
#define VBO_MASK            0x0000000eUL
#define GPIO_BASE_ADDR      0x02002000UL
#define SYSCON_SYS_IM_ADDR  0x0200000cUL
#define SYSCON_VBOCTRL_ADDR 0x02000044UL

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct
{
	uint32 u32Dir;
	uint32 u32Dout;
	/* Din is Hw writable, SW read only */
	const volatile uint32 u32Din;
}tsGPIO;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

static uint32 u32LocalLedMask;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vLowBatIndicationInit(uint32 u32LedMask,teBrownOutTripVoltage eBOTripValue)
{

	/* WORKAROUND for [lpsw4269]Set trip voltage, enable SVM & disable reset on BO */
	*(uint32 *)SYSCON_VBOCTRL_ADDR = eBOTripValue |1;

#ifndef RTOS_PRESENT
	/* Hook in the brownout interrupt (will get callback straight away if VDD < VBO) */
	/* NOTE: Don't need to do this in RTOS based systems as can be spoofed from the  */
	/* System Controller ISR on the OS Config diagram                                */

	vAHI_SysCtrlRegisterCallback(vCbSystemController);
#endif

	/* Note the bits used to drive any leds for use by ISR call-back*/
	u32LocalLedMask = u32LedMask;

	/* check we're OK on startup of SVM (also adds delay for config change*/
	if (bAHI_BrownOutStatus() == TRUE)
	{
		vCbSystemController(E_AHI_DEVICE_SYSCTRL,E_AHI_SYSCTRL_VREM_MASK);
	}

    /* Enable Rising Event mask (VDD below BO trip voltage) */
	*(uint32 *)SYSCON_SYS_IM_ADDR |= E_AHI_SYSCTRL_VREM_MASK;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

PUBLIC void vCbSystemController(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
	volatile int i;

	/* Validate interrupt source when brown out state entered */
	if ((E_AHI_DEVICE_SYSCTRL == u32DeviceId) && (E_AHI_SYSCTRL_VREM_MASK == u32ItemBitmap))
	{
		tsGPIO * const psGPIO = (tsGPIO *)GPIO_BASE_ADDR;

		psGPIO->u32Dir  |= u32LocalLedMask;  /* Ensure led DIO are configured as outputs      */
		psGPIO->u32Dout |= u32LocalLedMask;  /* Set them to same state for low bat indication */

		while(1)  /* Loop forever winking the two leds */
		{
			psGPIO->u32Dout = psGPIO->u32Din ^ u32LocalLedMask;
			for(i=0; i<DELAY_COUNT; i++);
		}
	}
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
#endif