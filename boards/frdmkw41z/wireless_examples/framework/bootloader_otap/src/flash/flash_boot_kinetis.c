/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "flash_boot_kinetis.h"


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */
uint8_t pCommandArray[FCCOB_REGS];
static FCC0B_STR CommandObj;
void (*ExecuteOnStack)(void) = NULL;
uint8_t buffer[128];


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
//  internal driver function
static uint8_t FLASH_Boot_FlashCommandSequence(uint8_t index);
static void ExecuteOnStackStart(void);


/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/********************************************************
* Init Function
*
********************************************************/
void FlashInitialization(void)
{
    uint16_t i, size;

    //  RAM function should have the same alignment as one from ROM
    if( ((uint32_t)ExecuteOnStackStart - 1) & 0x03 )
        i = 2;
    else
        i = 0;

    size = sizeof(buffer) - i;

    //  initialize pointer to ram function
    ExecuteOnStack = (void(*)(void))&buffer[i+1];

    //  copy function from ROM to RAM
    for( ; i<size; i++ )
        buffer[i] = ((uint8_t*)ExecuteOnStackStart)[i-1];

    //  inititalization of flash clock module
    FLASH_INIT_FLASH_CLOCK;
}

/********************************************************
* Generic FLASH programing function
*
********************************************************/
uint8_t FLASH_Boot_Program(uint32_t dest, uint32_t source, uint32_t size)
{
    uint32_t i;
    uint8_t* pSource = (uint8_t*)source;
    uint8_t temp[FLASH_PGM_SIZE];

    /* Check for unaligned destination address */
    if (dest & (FLASH_PGM_SIZE - 0x01U))
    {
       return FLASH_FAIL;
    }

    while( size )
    {
        /* data must be multiple of FLASH_PGM_SIZE */
        if( size > 0 && size < FLASH_PGM_SIZE  )
        {
            for( i=0; i<size; i++ )
            {
                temp[i] = *pSource++;
            }

            for( i=size; i<FLASH_PGM_SIZE; i++ )
            {
                temp[i] = 0xFF;
            }

            pSource = temp;
            size = FLASH_PGM_SIZE;
        }

        /* preparing passing parameter to program the flash block */
        CommandObj.regsLong.fccob3210 = dest;
#if (FLASH_PGM_SIZE==8)
        CommandObj.regs.fccob0 = FLASH_PROGRAM_PHRASE;
#else
        CommandObj.regs.fccob0 = FLASH_PROGRAM_LONGWORD;
#endif
        for( i=4; i<FLASH_PGM_SIZE+4; i++ )
        {
            CommandObj.all[i] = *pSource++;
        }

        /* calling flash command sequence function to execute the command */
        if( FLASH_OK != FLASH_Boot_FlashCommandSequence(i) )
            return FLASH_FAIL;

        /* update destination address for next iteration */
        dest += FLASH_PGM_SIZE;
        /* update size for next iteration */
        size -= FLASH_PGM_SIZE;
    }

    return FLASH_OK;
}

/********************************************************
* Function for Programming multiple Long Word
*
********************************************************/
uint8_t FLASH_Boot_ProgramLongWords(uint32_t destination, uint32_t source, uint32_t size)
{
    uint8_t ret = FLASH_OK;
    uint8_t bytes;
    uint32_t alignedWord;
    uint8_t* pbSource = (uint8_t*)source;

    bytes = destination & 0x03;
    destination &= ~((uint32_t)0x03);
    if( bytes )
    {
        /* Go to previous aligned address */
        alignedWord = *((uint32_t*)destination);

    }
    while(size)
    {
        ((uint8_t*)&alignedWord)[bytes] = *pbSource++;
        size--;
        if((bytes == 3)||(size == 0))
        {
            ret = FLASH_Boot_ProgramLongWord(destination, alignedWord);
            if(FLASH_OK != ret )
            {
                return ret;
            }
            if(size)
            {
                destination += 4;
                if(size < 4)
                {
                    alignedWord = *((uint32_t*)destination);
                }
            }
        }
        bytes = (bytes+1) & 0x3;
    }


    return FLASH_OK;
}
/********************************************************
* Function for Programming of one Long Word
*
********************************************************/
uint8_t FLASH_Boot_ProgramLongWord(uint32_t destination, uint32_t data32b)
{
    /* preparing passing parameter to program the flash block */

    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_PROGRAM_LONGWORD;
    CommandObj.regsLong.fccob7654 = data32b;

    return FLASH_Boot_FlashCommandSequence(PROGRAM_LONGWORD_INDEX);
}

/********************************************************
* Function for Programming of one section (maximum is FLASH_ERASE_PAGE )
*
********************************************************/
uint8_t FLASH_Boot_ProgramSection(uint32_t destination, uint32_t* pSource, uint32_t size)
{
    uint8_t* pProgBuff = (uint8_t*)(0x14000000);
    uint8_t* pSrc = (uint8_t*)pSource;
    uint32_t phases = size / FLASH_PHASE_SIZE;
    uint8_t  r = size % FLASH_PHASE_SIZE;

    // check the size of memory
    if((size > FLASH_ERASE_PAGE) || (size == 0))
        return FLASH_FAIL;

    if(destination & 0x00000003)
        return FLASH_FAIL;

    if (r > 0)
    {
        phases++;
        r = FLASH_PHASE_SIZE - r;
    }

    /* preparing passing parameter to program the flash block */
    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_PROGRAM_SECTION;
    CommandObj.regs.fccob4 = (uint8_t)(phases >> 8);
    CommandObj.regs.fccob5 = (uint8_t)(phases & 0xff);

    //  copy source data to program buffer
    while(size--)
        *pProgBuff++ = *pSrc++;

    while(r--)
        *pProgBuff++ = 0xFF;

    //  call flash sequence
    return FLASH_Boot_FlashCommandSequence(PROGRAM_SECTION_INDEX);
}

/********************************************************
* Function for Programming of one section (maximum is FLASH_ERASE_PAGE )
* The data must be preloaded into the program acceleration RAM
********************************************************/
uint8_t FLASH_Boot_ProgramPreloadedSection(uint32_t destination, uint32_t size)
{
    uint8_t* pProgBuff = (uint8_t*)(0x14000000) + size;
    uint32_t phases = size / FLASH_PHASE_SIZE;
    uint8_t  r = size % FLASH_PHASE_SIZE;

    /* check the size of memory */
    if((size > FLASH_ERASE_PAGE) || (size == 0))
        return FLASH_FAIL;

    if(destination & 0x00000003)
        return FLASH_FAIL;

    if (r > 0)
    {
        phases++;
        r = FLASH_PHASE_SIZE - r;
    }

    /* preparing passing parameter to program the flash block */
    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_PROGRAM_SECTION;
    CommandObj.regs.fccob4 = (uint8_t)(phases >> 8);
    CommandObj.regs.fccob5 = (uint8_t)(phases & 0xff);

    while(r--)
        *pProgBuff++ = 0xFF;

    /*  call flash sequence */
    return FLASH_Boot_FlashCommandSequence(PROGRAM_SECTION_INDEX);
}


/********************************************************
* Function for Programming of one section (maximum is 2048 Bytes)
*
********************************************************/
uint8_t FLASH_Boot_ProgramSectionPhrases(uint32_t destination, uint32_t* pSource, uint32_t size)
{
    uint8_t* pProgBuff = (uint8_t*)(0x14000000);
    uint8_t* pSrc = (uint8_t*)pSource;
    uint32_t phases = size / FLASH_PHASE_SIZE;
    uint8_t  r = size % FLASH_PHASE_SIZE;

    // check the size of memory
    if((size > FLASH_ERASE_PAGE) || (size == 0))
        return FLASH_FAIL;

    if(destination & 0x00000003)
    {
        if(size & 0x00000001)
            if(!FLASH_Boot_ProgramLongWord(destination, *pSource))
                return FLASH_FAIL;
    }

    if(destination & 0x00000004)
    {
        if(!FLASH_Boot_ProgramLongWord(destination, *pSource++))
            return FLASH_FAIL;
        size-=4;
        destination += 4;
    }

    if (r)
    {
        phases++;
        r = FLASH_PHASE_SIZE - r;
    }

    /* preparing passing parameter to program the flash block */
    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_PROGRAM_SECTION;
    CommandObj.regs.fccob4 = (uint8_t)(phases >> 8);
    CommandObj.regs.fccob5 = (uint8_t)(phases & 0xff);

    //  copy source data to program buffer
    while(size--)
        *pProgBuff++ = *pSrc++;

    while(r--)
        *pProgBuff++ = 0xFF;

    //  call flash sequence
    return FLASH_Boot_FlashCommandSequence(PROGRAM_SECTION_INDEX);
}

/********************************************************
*
*
********************************************************/
uint8_t FLASH_Boot_ProgramByte(uint32_t destination, uint8_t data8b)
{
    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_PROGRAM_BYTE;
    CommandObj.regs.fccob4 = data8b;

    return FLASH_Boot_FlashCommandSequence(PROGRAM_BYTE_INDEX);
}

/********************************************************
* Function for erasing of flash memory sector (0x800)
*
********************************************************/
uint8_t FLASH_Boot_EraseSector(uint32_t destination)
{
    CommandObj.regsLong.fccob3210 = destination;
    CommandObj.regs.fccob0 = FLASH_ERASE_SECTOR;

    return FLASH_Boot_FlashCommandSequence(ERASE_BLOCK_INDEX);
}


/********************************************************
* Function for erasing of all block of memory
*
********************************************************/
uint8_t FLASH_Boot_MassErase(void)
{
    CommandObj.regs.fccob0 = FLASH_ERASE_ALL_BLOCK;
    return FLASH_Boot_FlashCommandSequence(ERASE_MASS_INDEX);
}

/********************************************************
* Function for enabling the EERAM
*
********************************************************/
uint8_t FLASH_Boot_SetEEEEnable(uint8_t ctrl_code)
{
    CommandObj.regs.fccob0 = FLASH_SET_EERAM;
    CommandObj.regs.fccob1 = ctrl_code;
    return FLASH_Boot_FlashCommandSequence(SET_EERAM_INDEX);
}

/********************************************************
* Function for Verifying the Flash Backdoor Key
*
********************************************************/
uint8_t FLASH_Boot_VerifyBackdoorKey(uint64_t key)
{
    uint8_t *pKey = (uint8_t*)&key;

    CommandObj.regs.fccob0 = FLASH_VERIFY_BACKDOOR_KEY;
    CommandObj.regs.fccob4 = pKey[3];
    CommandObj.regs.fccob5 = pKey[2];
    CommandObj.regs.fccob6 = pKey[1];
    CommandObj.regs.fccob7 = pKey[0];
    CommandObj.regs.fccob8 = pKey[7];
    CommandObj.regs.fccob9 = pKey[6];
    CommandObj.regs.fccobA = pKey[5];
    CommandObj.regs.fccobB = pKey[4];
    
    return FLASH_Boot_FlashCommandSequence(VERIFY_KEY_INDEX);
}


/*! *********************************************************************************
*************************************************************************************
* Private Functions
*************************************************************************************
********************************************************************************** */

/********************************************************
* Function for executing of the flash command
*
********************************************************/
static uint8_t FLASH_Boot_FlashCommandSequence(uint8_t index)
{
    uint8_t* ptrFccobReg = (uint8_t*)&FLASH_BASE_PTR->FCCOB3;
    uint8_t* ptrCommandObj = (uint8_t*)&CommandObj;


    /* wait till CCIF bit is set */
    while((FLASH_FSTAT&FLASH_FSTAT_CCIF_MASK) != FLASH_FSTAT_CCIF_MASK){};

    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FLASH_FSTAT = FLASH_FSTAT_ACCERR_MASK|FLASH_FSTAT_FPVIOL_MASK|FLASH_FSTAT_RDCOLERR_MASK;

    /* load FCCOB registers */
    while(index--)
        *ptrFccobReg++ = *ptrCommandObj++;

    /* jump to RAM function */
    ExecuteOnStack();

    /* Check error bits */
    /* Get flash status register value */
    if(FLASH_FSTAT & (FLASH_FSTAT_ACCERR_MASK|FLASH_FSTAT_FPVIOL_MASK|FLASH_FSTAT_MGSTAT0_MASK))
        return FLASH_FAIL;

    return FLASH_OK;
}

/********************************************************
* Function for lounching a FLASH command: will run from RAM
*
********************************************************/
static void ExecuteOnStackStart(void)
{
    __asm(" cpsid i ");
    /*  launch a command */
    FLASH_FSTAT |= FLASH_FSTAT_CCIF_MASK;

    /*  waiting for the finishing of the command */
    while((FLASH_FSTAT&FLASH_FSTAT_CCIF_MASK) != FLASH_FSTAT_CCIF_MASK){};
    __asm(" cpsie i ");
}
