/**************************************************************************
MODULE:    MCOHW_NVOL_SIM
CONTAINS:  Simulation of non-volatile memory, here in regular RAM
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

#include "mcohw.h"

#if USE_STORE_PARAMETERS

// starting address of nvol storage in RAM
#define NVOL_ADDRESS 0x2000A000

// Simulated non-volatile memory, here: regular RAM
uint8_t mNVOL[NVOL_STORE_SIZE];
// static uint8_t *mNVOL; // DEBUG/TEST: here grabbing some free RAM

/**************************************************************************
DOES:    Initializes access to non-volatile memory.
**************************************************************************/
void NVOL_Init(void)
{
    uint8_t *pRAM;
    uint32_t lp;

    // here: use reserved bytes in RAM
    pRAM = (uint8_t *)NVOL_ADDRESS;
    for (lp = 0; lp < NVOL_STORE_SIZE; lp++)
    {
        mNVOL[lp] = *pRAM;
        pRAM++;
    }
}

/**************************************************************************
DOES:    Reads a data byte from non-volatile memory
NOTE:    The address is relative, an offset to NVOL_STORE_START
RETURNS: The data read from memory
**************************************************************************/
uint8_t NVOL_ReadByte(uint16_t address // location of byte in NVOL memory
)
{
    // Protect from illegal address access
    if (address >= NVOL_STORE_SIZE)
        return 0;
    return mNVOL[address];
}

/**************************************************************************
DOES:    Writes a data byte to non-volatile memory
NOTE:    The address is relative, an offset to NVOL_STORE_START
RETURNS: nothing
**************************************************************************/
void NVOL_WriteByte(uint16_t address, // location of byte in NVOL memory
                    uint8_t data)
{
    // Protect from illegal address access
    if (address >= NVOL_STORE_SIZE)
        return;
    mNVOL[address] = data;
}

/**************************************************************************
DOES:    Is called when a block of write cycles is complete. The driver
         may buffer the data from calls to NVOL_WriteByte in RAM and then
         write the entire buffer to non-volatile memory upon a call to
         this function.
**************************************************************************/
void NVOL_WriteComplete(void)
{
    uint8_t *pRAM;
    uint32_t lp;

    // here: use reserved bytes in RAM
    pRAM = (uint8_t *)NVOL_ADDRESS;
    for (lp = 0; lp < NVOL_STORE_SIZE; lp++)
    {
        *pRAM = mNVOL[lp];
        pRAM++;
    }
}
#endif // USE_STORE_PARAMETERS

/*----------------------- END OF FILE ----------------------------------*/
