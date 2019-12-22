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

#if EW_USE_FREE_RTOS == 1

  #include "FreeRTOS.h"
  #include "task.h"

  #define OS_SIGNAL_WAKEUP_UI  0x00000001

  static TaskHandle_t ThreadId = 0;

#endif

#include "ew_bsp_clock.h"
#include "ew_bsp_event.h"

void EwBspDelay( unsigned long milliseconds );

/*******************************************************************************
* FUNCTION:
*   EwBspWaitForSystemEvent
*
* DESCRIPTION:
*   The function EwBspWaitForSystemEvent should be called from the Embedded
*   Wizard main loop in case there are no pending events, signals or timers that
*   have to be processed by the UI application.
*   The function EwBspWaitForSystemEvent is used to sleep the given time span or
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
void EwBspWaitForSystemEvent( int aTimeout )
{
  if ( aTimeout > 0 )
  {
    CPU_LOAD_SET_IDLE();

    #if EW_USE_FREE_RTOS == 1

      /* save the thread ID for signaling */
      if ( ThreadId == 0 )
        ThreadId = xTaskGetCurrentTaskHandle();

      /* Important note: Within this template, the timeout value is limited to
         10 ms in order to ensure that the UI application is continuously working,
         e.g. to get new touch values or to receive data from a device driver.
         This makes the usage of this template very simple.
         If you want to implement a completely event driven system, you can use
         the given timeout without limiting it. In this case you have to ensure
         that the touch driver or your device drivers are resuming the UI task
         by calling TriggerSystemEvent().
      */
      if ( aTimeout > 10 )
        aTimeout = 10;

      xTaskNotifyWait( 0, OS_SIGNAL_WAKEUP_UI, NULL, aTimeout / portTICK_PERIOD_MS );

    #else

      /* in bare metal maximum wait time is 10 ms to keep main loop running */
      if ( aTimeout > 10 )
        aTimeout = 10;

      EwBspDelay( aTimeout );

    #endif

    CPU_LOAD_SET_ACTIVE();
  }
}


/*******************************************************************************
* FUNCTION:
*   EwBspTriggerSystemEvent
*
* DESCRIPTION:
*   The function EwBspTriggerSystemEvent is used in combination with an
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
void EwBspTriggerSystemEvent( void )
{
  #if EW_USE_FREE_RTOS == 1

  xTaskNotify( ThreadId, OS_SIGNAL_WAKEUP_UI, eSetBits );

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspDelayMs
*
* DESCRIPTION:
*   Delay in milliseconds.
*
* ARGUMENTS:
*   milliseconds - Delay in milliseconds.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDelay( unsigned long milliseconds )
{
    unsigned long i;
    unsigned long j;

    for (i = 0; i < milliseconds; i++)
    {
        for (j = 0; j < 23600U; j++)
        {
            __asm("NOP");
        }
    }
}


/* mli */
