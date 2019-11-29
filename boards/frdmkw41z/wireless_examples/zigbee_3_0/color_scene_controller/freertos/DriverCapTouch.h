/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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