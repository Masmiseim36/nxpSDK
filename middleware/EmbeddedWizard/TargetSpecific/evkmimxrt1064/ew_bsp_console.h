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
*   This template is responsible to establish a console connection in order
*   to send debug messages to a PC terminal tool, or to receive key events
*   for the UI application.
*
*******************************************************************************/

#ifndef EW_BSP_CONSOLE_H
#define EW_BSP_CONSOLE_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleInit
*
* DESCRIPTION:
*   The function EwBspConsoleInit initializes a console connection used to print
*   error and trace messages from an Embedded Wizard GUI application.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsoleInit
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleDone
*
* DESCRIPTION:
*   The function EwBspConsoleDone terminates a console connection.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsoleDone
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspConsolePutCharacter
*
* DESCRIPTION:
*   The function EwBspConsolePutCharacter sends the given character to the console
*   interface.
*
* ARGUMENTS:
*   aCharacter - The character to be send via the console interface.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsolePutCharacter
(
  unsigned char               aCharacter
);


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleGetCharacter
*
* DESCRIPTION:
*   The function EwBspConsoleGetCharacter returns the current character from the
*   console interface. If no character is available within the input buffer, 0 is
*   returned.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Current character from console input buffer or 0.
*
*******************************************************************************/
unsigned char EwBspConsoleGetCharacter
(
  void
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_CONSOLE_H */


/* msy */
