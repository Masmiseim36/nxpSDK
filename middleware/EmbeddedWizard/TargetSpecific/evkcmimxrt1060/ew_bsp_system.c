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

#include "ewconfig.h"
#include "ew_bsp_system.h"
#include "ew_bsp_clock.h"
#include "fsl_debug_console.h"


/*******************************************************************************
* FUNCTION:
*   EwBspSystemInit
*
* DESCRIPTION:
*   The function EwBspSystemInit initializes the system components.
*   (CPU clock, memory, qspi, ...)
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspSystemInit( void )
{
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_InitI2C1Pins();
  BOARD_InitSemcPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Workaround for SEMC queue:
     The SEMC queue allows for reordering of SDRAM operations. In some conditions,
     the reodering can cause problems that appear as reading incorrect/stale data
     from the SDRAM. Changing the BMCRn values to increase the WAGE weighting
     prevents the reordering of operations and avoids the issue. */
  {
    uint32_t* SEMC_BMCR0 = (uint32_t*)0x402F0008;
    *SEMC_BMCR0 = 0x80;
    uint32_t* SEMC_BMCR1 = (uint32_t*)0x402F000C;
    *SEMC_BMCR1 = 0x80;
  }

  /* Workaround to support direct access of PXP to flash memory - splitting the
     FlexSPI AHB buffer into two parts to share between core and PXP */
  // FLEXSPI->AHBCR          = 0x0;        /* Temporarily disable prefetching while changing the buffer settings */
  // FLEXSPI->AHBRXBUFCR0[0] = 0x80000040; /* Allocate half of the prefetch buffer to the core */
  // FLEXSPI->AHBRXBUFCR0[3] = 0x80030040; /* Other half of the buffer for other masters incl. PXP */
  // FLEXSPI->AHBCR          = 0x78;       /* Set AHBCR back to the original value */

}


/*******************************************************************************
* FUNCTION:
*   EwBspSystemDone
*
* DESCRIPTION:
*   The function EwBspSystemDone terminates the system components.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspSystemDone( void )
{
}


/* msy, mli */
