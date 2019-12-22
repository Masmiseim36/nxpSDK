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
*   This template is responsible to establish a serial connection in order
*   to send debug messages to a PC terminal tool, or to receive key events
*   for the UI application.
*
*******************************************************************************/

#include "fsl_debug_console.h"
#include "ew_bsp_serial.h"


/*******************************************************************************
* FUNCTION:
*   EwBspConfigSerial
*
* DESCRIPTION:
*   The function EwBspConfigSerial initializes a serial connection via UART/USART
*   interface used to print error and trace messages from an Embedded Wizard
*   GUI application.
*   This implementation uses the following configuration:
*   - BaudRate = 115200 baud
*   - Word Length = 8 Bits
*   - One Stop Bit
*   - No parity
*   - Hardware flow control disabled (RTS and CTS signals)
*   - Receive and transmit enabled
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigSerial( void )
{
}


/*******************************************************************************
* FUNCTION:
*   EwBspPutCharacter
*
* DESCRIPTION:
*   The function EwBspPutCharacter sends the given character to the serial
*   interface.
*
* ARGUMENTS:
*   aCharacter - The character to be send via the serial interface.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspPutCharacter( unsigned char aCharacter )
{
  DbgConsole_Putchar( aCharacter );
}

/*******************************************************************************
* FUNCTION:
*   EwBspGetCharacter
*
* DESCRIPTION:
*   The function EwBspGetCharacter returns the current character from the serial
*   interface. If no character is available within the input buffer, 0 is
*   returned.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Current character from serial input buffer or 0.
*
*******************************************************************************/
unsigned char EwBspGetCharacter( void )
{
  return 0;
}

/* msy */
