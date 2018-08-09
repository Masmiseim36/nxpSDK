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

#if 0
#ifndef  CAPTOUCH_H_INCLUDED
#define  CAPTOUCH_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/** Enumerated type for the status of the touch interface */
typedef enum
{
    TOUCH_STATUS_OK,                /**< Touch interface ok */
    TOUCH_STATUS_ERROR,             /**< Touch interface error   */
    TOUCH_STATUS_DONT_SLEEP,        /**< Touch interface needs to sample again */
} teTouchStatus;

/** Enumerated type of possible button events */
typedef enum
{
    TOUCH_BUTTON_EVENT_RELEASED,    /**< Touch button was released  */
    TOUCH_BUTTON_EVENT_PRESSED,     /**< Touch button was pressed   */
    TOUCH_BUTTON_EVENT_ALL_RELEASED,/**< No touch buttons are currently pressed */
} eTouchButtonEvent;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/** Initialise the touch interface of the platform
 *  \return TOUCH_STATUS_OK if all set up ok
 */
PUBLIC teTouchStatus eTouchInit(void);


/** Inform the touch driver that the device is going to sleep
 *  \return TOUCH_STATUS_OK if ok,
 *  TOUCH_STATUS_DONT_SLEEP if the interface needs to sample again.
 */
PUBLIC teTouchStatus eTouchSleep(void);


/** Inform the touch driver that the device has just woken up.
 *  Combined with the sleep function, this can be used to detect
 *  changes in the baseline capacitance associated with the device
 *  being picked up.
 *  \return TOUCH_STATUS_OK if ok,
 *  TOUCH_STATUS_DONT_SLEEP if the interface needs to sample again.
 */
PUBLIC teTouchStatus eTouchWake(void);

/** Function that the application must call regularly to process the
 *  touch interface.
 *  The touch interface will acquire data from the connected touch interface.
 *  The callback functions will be called as appropriate for any events that
 *  may occur during this call.
 *  \return TOUCH_STATUS_OK if ok,
 *  TOUCH_STATUS_DONT_SLEEP if the interface needs to sample again.
 */
PUBLIC teTouchStatus eTouchProcess(void);

/** Callback prototype function for the touch driver to call when
 *  a button event is detected.
 *  Weak symbol which can be overridden in user code but does not have to be defined.
 *  \param   eEvent          - The event that has been detected
 *  \param   u8ButtonNumber  - The button number which experienced the event
 */
PUBLIC void vCbTouchEventButton(eTouchButtonEvent eEvent, uint8 u8ButtonNumber) __attribute__((weak));

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* CAPTOUCH_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
#endif