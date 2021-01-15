/**************************************************************************
PROJECT:   Communication interface for Remote Access, here UART
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020.
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

#ifndef _MCOHW_COM_H
#define _MCOHW_COM_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
 DEFINITIONS AND TYPES
***************************************************************************/

#define COM_COMMON_ERR 0x40 // Common error of receiver
#define COM_FULL_RX    0x10 // Full receive buffer
#define COM_FULL_TX    0x20 // Full transmit buffer

/**************************************************************************
DOES:    Initialization of COM interface
RETURNS: -
**************************************************************************/
void COM_Init(void);

/**************************************************************************
DOES:    Waits for all transmits to be completed
RETURNS: Nothing
**************************************************************************/
void COM_Flush(void);

/**************************************************************************
DOES:    Reads next received byte from the receive buffer, if not empty
RETURNS: TRUE, if byte was read, FALSE otherwise
**************************************************************************/
uint8_t COM_GetByte(uint8_t *byt // Pointer to return the received character
);

/**************************************************************************
DOES:    Writes a byte to the transmit buffer
RETURNS: TRUE, if byte was sent, FALSE otherwise (buffer full)
**************************************************************************/
uint8_t COM_SendByte(uint8_t byt // Character to send
);

/**************************************************************************
DOES:    Reads the error status of the communication port and clears status
RETURNS: Error status
**************************************************************************/
uint8_t COM_GetErrorStatus(void);

/**************************************************************************
DOES:    Checks if the transmit buffer is empty
RETURNS: TRUE if transmit buffer is empty
**************************************************************************/
uint8_t COM_TxEmpty(void);

#ifdef __cplusplus
}
#endif

#endif // ifndef _MCOHW_COM_H
/***************************************************************************
END OF FILE
***************************************************************************/
