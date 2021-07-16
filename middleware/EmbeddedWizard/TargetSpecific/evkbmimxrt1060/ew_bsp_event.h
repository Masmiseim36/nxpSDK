/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   This template provides a system event mechanism, that can be used in 
*   combination with an operating system to sleep and to continue (resume) the 
*   operation of the UI main loop.
*
*******************************************************************************/

#ifndef EW_BSP_EVENT_H
#define EW_BSP_EVENT_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* FUNCTION:
*   EwBspEventWait
*
* DESCRIPTION:
*   The function EwBspEventWait should be called from the Embedded 
*   Wizard main loop in case there are no pending events, signals or timers that 
*   have to be processed by the UI application.
*   The function EwBspEventWait is used to sleep the given time span or
*   to suspend the UI task. The function returns as soon as a new system event
*   occurs or when the given timeout value is expired.
*   Typically, a system event is a touch event or a key event or any event
*   from your device driver.
*
* ARGUMENTS:
*   aTimeout - timeout value in milliseconds.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspEventWait
( 
  int                         aTimeout
);


/*******************************************************************************
* FUNCTION:
*   EwBspEventTrigger
*
* DESCRIPTION:
*   The function EwBspEventTrigger is used in combination with an 
*   operating system to continue (resume) the operation of the UI main loop.
*   Typically, a system event is a touch event or a key event or any event
*   from your device driver.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspEventTrigger
( 
  void
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_EVENT_H */


/* msy */
