/**************************************************************************
PROJECT:   Generic UART Driver
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#include <stdint.h>
#include "mcop_inc.h"

/**************************************************************************
 LOCAL DEFINITIONS
***************************************************************************/

/**************************************************************************
 EXTERNAL REFERENCES
***************************************************************************/

/**************************************************************************
 PUBLIC DEFINITIONS
***************************************************************************/

/**************************************************************************
 LOCAL FUNCTION PROTOTYPES
***************************************************************************/

/**************************************************************************
 LOCAL FUNCTIONS
***************************************************************************/

/**************************************************************************
 PUBLIC FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Initialization of COM interface
RETURNS: -
**************************************************************************/
void COM_Init(void)
{
}

/**************************************************************************
DOES:    Waits for all transmits to be completed
RETURNS: Nothing
**************************************************************************/
void COM_Flush(void)
{
    // all transmits are immediate, so nothing to do here
}

/**************************************************************************
DOES:    Reads next received byte from the receive buffer, if not empty
         Does not block
RETURNS: TRUE, if byte was read, FALSE otherwise
**************************************************************************/
uint8_t COM_GetByte(uint8_t *byt // Pointer to return the received character
)
{
    uint8_t return_val = FALSE;
#if NON_BLOCKING_GETCHAR_AVAILABLE
    int ch;

    ch = getchar();
    if (ch != EOF)
    {
        &byt       = (uint8_t)ch;
        return_val = TRUE;
    }
#endif

    return (return_val);
}

/**************************************************************************
DOES:    Writes a byte to the transmit buffer
RETURNS: TRUE, if byte was sent, FALSE otherwise (buffer full)
**************************************************************************/
uint8_t COM_SendByte(uint8_t byt // Character to send
)
{
    uint8_t return_val = FALSE;
    int ch;

    ch = putchar((int)byt);
    if (ch != EOF)
    {
        return_val = TRUE;
    }

    return (return_val);
}

/**************************************************************************
DOES:    Reads the error status of the communication port and clears status
RETURNS: Error status
**************************************************************************/
uint8_t COM_GetErrorStatus(void)
{
    uint8_t return_val = 0u;

    return (return_val);
}

/**************************************************************************
DOES:    Checks if the transmit buffer is empty
RETURNS: TRUE if transmit buffer is empty
**************************************************************************/
uint8_t COM_TxEmpty(void)
{
    // all transmits are immediate, so always empty
    return TRUE;
}

/***************************************************************************
END OF FILE
***************************************************************************/
