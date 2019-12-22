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
*   This template is responsible to initialize the system clock of the hardware,
*   to provide a timer tick for the Embedded Wizard UI application and to get
*   access to the realtime clock (RTC). Additionally, this file contains some
*   performance measurements to analyse the CPU usage of the UI application.
*
*******************************************************************************/

#ifndef EW_BSP_CLOCK_H
#define EW_BSP_CLOCK_H


#ifdef __cplusplus
  extern "C"
  {
#endif


#if EW_CPU_LOAD_MEASURING == 1
  #define CPU_LOAD_SET_IDLE()           EwBspCpuLoadSetIdle()
  #define CPU_LOAD_SET_ACTIVE()         EwBspCpuLoadSetActive()
#else
  #define CPU_LOAD_SET_IDLE()
  #define CPU_LOAD_SET_ACTIVE()
#endif


/*******************************************************************************
* FUNCTION:
*   EwBspConfigSystemTick
*
* DESCRIPTION:
*   Configure the system tick counter.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigSystemTick
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspSystemTickIncrement
*
* DESCRIPTION:
*   The function EwBspSystemTickIncrement increments the millisecond counter,
*   which is used by the Runtime Environmet (RTE) to trigger timer events.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspSystemTickIncrement
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspConfigRealTimeClock
*
* DESCRIPTION:
*   Configures the Real Time Clock.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigRealTimeClock
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspGetTime
*
* DESCRIPTION:
*   Returns the current time in seconds since 01.01.1970.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current time in seconds since 01.01.1970.
*
*******************************************************************************/
unsigned long EwBspGetTime
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspSetTime
*
* DESCRIPTION:
*   Sets the given time in seconds since 01.01.1970 at real time clock.
*
*******************************************************************************/
void EwBspSetTime
(
  unsigned long aTime
);


/*******************************************************************************
* FUNCTION:
*   EwBspGetCpuLoad
*
* DESCRIPTION:
*   Returns the current CPU load as percent value.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current CPU load.
*
*******************************************************************************/
int EwBspGetCpuLoad
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspCpuLoadSetActive
*
* DESCRIPTION:
*   Starts the CPU load counting. Call this function whenever CPU processing
*   is needed.
*
* ARGUMENTS:
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspCpuLoadSetActive
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspCpuLoadSetIdle
*
* DESCRIPTION:
*   Stops the CPU load counting. Call this function whenever CPU processing is
*   currently not needed since the program execution is waiting for some system
*   event.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspCpuLoadSetIdle
(
  void
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_CLOCK_H */


/* msy */
