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
\file       app_buttons.c
\brief      ZLO Demo: DK button handler -Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "dbg.h"

#include "app_events.h"

#include "pwrm.h"
#include "app_buttons.h"
#include "app_main.h"
#include "ZTimer.h"
//#include "fsl_gint.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef TRACE_APP_BUTTON
#define TRACE_APP_BUTTON               FALSE
#endif




/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
#if (defined OM15081)
void gint_callback(void);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
#if (defined BUTTON_MAP_OM15081)

	PRIVATE uint8 s_u8ButtonDebounce[APP_BUTTONS_NUM] = { 0xff };
	PRIVATE uint8 s_u8ButtonState[APP_BUTTONS_NUM] = { 0 };
	PRIVATE const uint8 s_u8ButtonDIOLine[APP_BUTTONS_NUM] =
	{
			APP_BOARD_SW0_PIN
	};
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: APP_bButtonInitialise
 *
 * DESCRIPTION:
 * Button Initialization
 *
 * PARAMETER: void
 *
 * RETURNS: bool
 *
 ****************************************************************************/
PUBLIC bool_t APP_bButtonInitialise(void)
{

#if (defined OM15081)
    gpio_pin_config_t switch_config = {
        kGPIO_DigitalInput,
    };

    GPIO_PinInit(GPIO, APP_BOARD_GPIO_PORT, APP_BOARD_SW0_PIN, &switch_config);

    /* Initialize GINT0 */
    GINT_Init(GINT0);

    /* Setup GINT0 for edge trigger, "OR" mode */
    GINT_SetCtrl(GINT0, kGINT_CombineOr, kGINT_TrigEdge, gint_callback);

    /* Select pins & polarity for GINT0 */
    GINT_ConfigPins(GINT0, DEMO_GINT0_PORT, DEMO_GINT0_POL_MASK, DEMO_GINT0_ENA_MASK);

    /* Enable callback(s) */
    GINT_EnableCallback(GINT0);

/* If we came out of deep sleep; perform appropriate action as well based
	   on button press.*/
	APP_cbTimerButtonScan(NULL);

	uint32 u32Buttons = APP_u32GetSwitchIOState();

	if (u32Buttons != APP_BUTTONS_DIO_MASK)
	{
		return TRUE;
	}
	return FALSE;

#else
    return FALSE;
#endif
}


/****************************************************************************
 *
 * NAME: APP_u32GetSwitchIOState
 *
 * DESCRIPTION:
 * reads the IO state of any switch inputs, flips the bits of any switches that are
 * active high
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
PUBLIC uint32 APP_u32GetSwitchIOState(void)
{
#if (defined OM15081)
    return (GPIO_ReadPinsInput(GPIO, APP_BOARD_GPIO_PORT) & APP_BUTTONS_DIO_MASK) ^ APP_BUTTON_INVERT_MASK;
#else
    return -0xffffffff;
#endif
}

/****************************************************************************
 *
 * NAME: gint_callback
 *
 * DESCRIPTION:
 *callback from the group io interrupt
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
#if (defined OM15081)
void gint_callback(void)
{
    /* Take action for gint event */;
    uint32 u32IOStatus = (GPIO_ReadPinsInput(GPIO, APP_BOARD_GPIO_PORT) & APP_BUTTONS_DIO_MASK) ^ APP_BUTTON_INVERT_MASK;
    DBG_vPrintf(TRACE_APP_BUTTON, "GINT 0 Pin Interrupt event detected %x\r\r\n", u32IOStatus);

    if (u32IOStatus != APP_BUTTONS_DIO_MASK)
    {
        /* disable edge detection until scan complete */
        GINT_DisableCallback(GINT0);
        ZTIMER_eStop(u8TimerButtonScan);
        ZTIMER_eStart(u8TimerButtonScan, ZTIMER_TIME_MSEC(10));
    }
}
#endif

/****************************************************************************
 *
 * NAME: APP_cbTimerButtonScan
 *
 * DESCRIPTION:
 * Timer callback to debounce the button presses
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerButtonScan(void *pvParam)
{

#if 0   
    uint8 u8AllReleased = 0xff;
    unsigned int i;


    uint32 u32DIOState = APP_u32GetSwitchIOState();

    for (i = 0; i < APP_BUTTONS_NUM; i++)
    {
        uint8 u8Button = (uint8) ((u32DIOState >> s_u8ButtonDIOLine[i]) & 1);

        s_u8ButtonDebounce[i] <<= 1;
        s_u8ButtonDebounce[i] |= u8Button;
        u8AllReleased &= s_u8ButtonDebounce[i];

        if (0 == s_u8ButtonDebounce[i] && !s_u8ButtonState[i])
        {
            s_u8ButtonState[i] = TRUE;

            /*
             * button consistently depressed for 8 scan periods
             * so post message to application task to indicate
             * a button down event
             */
            APP_tsLightEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;
            sButtonEvent.uEvent.sButton.u8Button = i;

            DBG_vPrintf(TRACE_APP_BUTTON, "Button DN=%d\r\n", i);

            ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent);
        }
        else if (0xff == s_u8ButtonDebounce[i] && s_u8ButtonState[i] != FALSE)
        {
            s_u8ButtonState[i] = FALSE;

            /*
             * button consistently released for 8 scan periods
             * so post message to application task to indicate
             * a button up event
             */
            APP_tsLightEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_UP;
            sButtonEvent.uEvent.sButton.u8Button = i;

            DBG_vPrintf(TRACE_APP_BUTTON, "Button UP=%i\r\n", i);

            ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent);
        }
    }

    if (0xff == u8AllReleased)
    {
        /*
         * all buttons high so set dio to interrupt on change
         */
        DBG_vPrintf(TRACE_APP_BUTTON, "ALL UP\r\n", i);
   // 	GINT_EnableCallback(GINT0);
 
    }
    else
    {
        /*
         * one or more buttons is still depressed so continue scanning
         */
        ZTIMER_eStop(u8TimerButtonScan);
        ZTIMER_eStart(u8TimerButtonScan, ZTIMER_TIME_MSEC(10));
    }
#endif    
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
