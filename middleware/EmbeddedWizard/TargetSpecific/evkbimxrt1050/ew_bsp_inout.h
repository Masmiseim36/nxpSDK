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
*   This template provides access to some LEDs and buttons of the board.
*
*******************************************************************************/

#ifndef EW_BSP_INOUT_H
#define EW_BSP_INOUT_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* CALLBACK:
*   TButtonCallback
*
* DESCRIPTION:
*   A function of this type has to be set with EwBspButtonConfig() to get
*   notified, everytime the hardware button is pressed or released.
*
* ARGUMENTS:
*   aButtonPresssed
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void ( * TButtonCallback ) ( int aButtonPresssed );


/*******************************************************************************
* FUNCTION:
*   EwBspConfigButton
*
* DESCRIPTION:
*   Configures one hardware button of the board used for demo applications.
*
* ARGUMENTS:
*   aButtonCallback - The button callback.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigButton
(
  TButtonCallback             aButtonCallback
);


/*******************************************************************************
* FUNCTION:
*   EwBspConfigLed
*
* DESCRIPTION:
*   Configures one LED of the board used for demo applications.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigLed
( 
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspLedOn
*
* DESCRIPTION:
*   Switch LED on (used for demo applications).
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspLedOn
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwBspLedOff
*
* DESCRIPTION:
*   Switch LED off (used for demo applications).
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspLedOff
( 
  void
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_INOUT_H */


/* msy */
