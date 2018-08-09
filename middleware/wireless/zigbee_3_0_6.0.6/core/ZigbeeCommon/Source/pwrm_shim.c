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
\file       pwrm_shim.c
\brief      provides the wrapper functions for power manager
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "PWR_Configuration.h"
#include "PWR_Interface.h"
#include "pwrm.h"
#include "dbg.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Imported Function Prototypes                                  ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE pwrm_tsCallbackDescriptor *pwrm_psWakeupCallbacks = NULL;
PRIVATE pwrm_tsCallbackDescriptor *s_psPreSleepCallbacks = NULL;
PRIVATE volatile bool_t s_bWakeTimerActive = FALSE;
void (*prWakeupCallbackfn)(void) = NULL;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: PWRM_vInit
 *
 * DESCRIPTION:
 * Initialise the PWRM manager
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/

PUBLIC void PWRM_vInit(PWRM_tePowerMode ePowerMode)
{
    static bool_t initialized = FALSE;

    if(FALSE == initialized)
    {
        initialized = TRUE;
        PWR_Init();
    }
    PWR_ChangeDeepSleepMode(ePowerMode);
    s_bWakeTimerActive = FALSE;
}

/****************************************************************************
 *
 * NAME: PWRM_vManagePower
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:      Name            RW  Usage
 * None
 *
 * RETURNS:
 * None
 *
 * NOTES:
 *
 ****************************************************************************/

PUBLIC void PWRM_vManagePower(void)
{

    if(PWR_CheckIfDeviceCanGoToSleep() && ((TRUE == s_bWakeTimerActive)||(3 != PWR_GetDeepSleepMode())))
    {
        PWRLib_WakeupReason_t wakeupReason;
        wakeupReason = PWR_EnterLowPower();
        if( wakeupReason.Bits.FromKeyBoard )
        {
            /* Protection to the LLWD pin enabled on both edges */
            static bool_t  wakeUpFlag = FALSE;
            if(TRUE == wakeUpFlag)
            {
                wakeUpFlag = FALSE;
                #if  (defined CPU_MKW41Z512VHT4) && (defined ZBPRO_DEVICE_TYPE_ZED)
                extern PUBLIC void App_SedWakeUpFromKeyBoard(void);
                App_SedWakeUpFromKeyBoard();
                #endif
            }
            else
            {
                wakeUpFlag = TRUE;
            }
        }
        else if( wakeupReason.Bits.DeepSleepTimeout )
        {
            //DBG_vPrintf(TRUE, "wakeupReason DeepSleepTimeout\r\n");
            if(NULL != prWakeupCallbackfn)
            {
                prWakeupCallbackfn();
            }
        }
    }
}

/****************************************************************************
 *
 * NAME: pwrm_vRegisterWakeupCallback
 *
 * DESCRIPTION:
 * Sets the pointer to the post sleep callbacks
 *
 * PARAMETERS:      Name            RW  Usage
 * A pointer to the structure holding the post sleep callbacks
 *
 * RETURNS:
 * None
 *
 * NOTES:
 *
 ****************************************************************************/

PUBLIC void pwrm_vRegisterWakeupCallback
            (pwrm_tsCallbackDescriptor *psCBDesc ) {

    /* [I SP001262_fr 145] begin */
    if (pwrm_psWakeupCallbacks == NULL) {
        pwrm_psWakeupCallbacks = psCBDesc;
    } else {
        pwrm_tsCallbackDescriptor *psCB = pwrm_psWakeupCallbacks;
        while (psCB->psNext != NULL) {
            psCB = psCB->psNext;
        }
        psCB->psNext = psCBDesc;
    }
}

/****************************************************************************
 *
 * NAME: pwrm_vRegisterPreSleepCallback
 *
 * DESCRIPTION:
 * Sets the pointer to the pre sleep callbacks
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psCBDesc        pointer to callback descriptor
 *
 * RETURNS:
 * None
 *
 * NOTES:
 *
 ****************************************************************************/

PUBLIC void pwrm_vRegisterPreSleepCallback
    ( pwrm_tsCallbackDescriptor *psCBDesc ){

    /* [I SP001262_fr 143] begin */
    if (s_psPreSleepCallbacks == NULL) {
        s_psPreSleepCallbacks = psCBDesc;
    }
    else {
        pwrm_tsCallbackDescriptor *psCB = s_psPreSleepCallbacks;
        while (psCB->psNext != NULL) {psCB = psCB->psNext;}
        psCB->psNext = psCBDesc;
    }
}

/****************************************************************************
 *
 * NAME: PWRM_eScheduleActivity
 *
 * DESCRIPTION:
 * If timed sleep is being used, start the wake timer with desired interval
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u32Ticks        number of miliseconds to sleep
 *
 * RETURNS:
 * None
 *
 * NOTES:
 *
 ****************************************************************************/

PUBLIC PWRM_teStatus PWRM_eScheduleActivity(
    pwrm_tsWakeTimerEvent * psWake,
    uint32 u32Ticks,
    void (*prCallbackfn)(void))
{

    if(3 == PWR_GetDeepSleepMode())
    {
        s_bWakeTimerActive = TRUE;
        PWR_SetDeepSleepTimeInMs(u32Ticks);
        prWakeupCallbackfn = prCallbackfn;
        return PWRM_E_OK;
    }

    return PWRM_E_TIMER_INVALID;
}


void PWRM_vPreSleepCallback(void)
{
    pwrm_tsCallbackDescriptor *psCb;
    for (psCb = s_psPreSleepCallbacks; psCb != NULL; psCb = psCb->psNext)
    {
        psCb->prCallback();
    }
}

void PWRM_vWakeupCallback(void)
{
    pwrm_tsCallbackDescriptor *psCb;
    s_bWakeTimerActive = FALSE;
    for (psCb = pwrm_psWakeupCallbacks; psCb != NULL; psCb = psCb->psNext)
    {
        psCb->prCallback();
    }
}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

