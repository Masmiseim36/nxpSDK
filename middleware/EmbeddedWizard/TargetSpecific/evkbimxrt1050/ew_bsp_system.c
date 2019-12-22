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
*   This template is responsible to configurate the entire system (CPU clock,
*   memory, qspi, etc).
*
*******************************************************************************/

#include "board.h"
#include "pin_mux.h"

#include "ew_bsp_system.h"
#include "ew_bsp_clock.h"
#include "fsl_debug_console.h"


/*******************************************************************************
* FUNCTION:
*   EwBspConfigSystem
*
* DESCRIPTION:
*   Configuration of system components (CPU clock, memory, qspi, ...)
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigSystem( void )
{
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_InitI2C1Pins();
  BOARD_InitSemcPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

#if 0
  EwBspConfigRealTimeClock();
  setDefaultTime();
#endif
}

/* msy, mli */
