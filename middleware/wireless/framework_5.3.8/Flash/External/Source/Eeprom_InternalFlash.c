/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the Source file for the EEPROM emulated inside the MCU's FLASH
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "Eeprom.h"

#if gEepromType_d == gEepromDevice_InternalFlash_c

#include "Flash_Adapter.h"
#include "FunctionLib.h"

/******************************************************************************
*******************************************************************************
* Private Macros
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr);

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/
static uint8_t mEepromEraseBitmap[32];
#if (gEepromParams_WriteAlignment_c > 1)
static uint8_t  mWriteBuff[gEepromParams_WriteAlignment_c];
static uint8_t  mWriteBuffLevel=0;
static uint32_t mWriteBuffAddr=0;
#endif

/******************************************************************************
*******************************************************************************
* Public Memory
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public Functions
*******************************************************************************
******************************************************************************/

/*****************************************************************************
*  EEPROM_Init
*
*  Initializes the EEPROM peripheral
*
*****************************************************************************/
ee_err_t EEPROM_Init(void)
{
    FLib_MemSet (mEepromEraseBitmap, 0x00, 32);
    
    NV_Init();
    
    return ee_ok;
}

/*****************************************************************************
*  EEPROM_ChipErase
*
*  Erase all memory to 0xFF
*
*****************************************************************************/
ee_err_t EEPROM_ChipErase(void)
{
    uint32_t i, endAddr;
    
    endAddr = gEepromParams_StartOffset_c + gEepromParams_TotalSize_c;

    for( i=gEepromParams_StartOffset_c; i<endAddr; i+=gEepromParams_SectorSize_c )
    {
        if( ee_ok != EEPROM_EraseBlock(i, gEepromParams_SectorSize_c) )
        {
            return ee_error;
        }
    }

    return ee_ok;
}

/*****************************************************************************
*  EEPROM_EraseSector4K
*
*  Erase 4K of memory to 0xFF
*
*****************************************************************************/
ee_err_t EEPROM_EraseBlock(uint32_t Addr, uint32_t size)
{
    ee_err_t status;

    if( size != gEepromParams_SectorSize_c )
    {
        status = ee_error;
    }
    else if( NV_FlashEraseSector(gEepromParams_StartOffset_c + Addr, size) )
    {
        status = ee_error;
    }
    else
    {
        status = ee_ok;
    }
    
    return status;
}

/*****************************************************************************
*  EEPROM_WriteData
*
*  Writes a data buffer into EEPROM, at a given address
*
*****************************************************************************/
ee_err_t EEPROM_WriteData(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{
    if( ee_ok != EEPROM_PrepareForWrite(NoOfBytes, Addr) )
    {
        return ee_error;
    }

#if (gEepromParams_WriteAlignment_c > 1)
    if( mWriteBuffLevel )
    {
        if( Addr >= mWriteBuffAddr &&
            Addr <  mWriteBuffAddr + gEepromParams_WriteAlignment_c )
        {
            uint32_t offset = Addr - mWriteBuffAddr;
            uint32_t size = gEepromParams_WriteAlignment_c - offset;

            FLib_MemCpy(&mWriteBuff[offset], Outbuf, size);
            Addr += size;
            Outbuf += size;
            NoOfBytes -= size;
        }
        else
        {
            FLib_MemSet(&mWriteBuff[mWriteBuffLevel], 0xFF, gEepromParams_WriteAlignment_c - mWriteBuffLevel);
        }
        
        NV_FlashProgramUnaligned(gEepromParams_StartOffset_c + mWriteBuffAddr, sizeof(mWriteBuff), mWriteBuff);
        mWriteBuffLevel = 0;
    }

    if( Addr & (gEepromParams_WriteAlignment_c-1) )
        return ee_not_aligned;

    /* Store unaligned bytes for later processing */
    mWriteBuffLevel = NoOfBytes % gEepromParams_WriteAlignment_c;
    NoOfBytes -= mWriteBuffLevel;
    mWriteBuffAddr = Addr + NoOfBytes;
    FLib_MemCpy(mWriteBuff, &Outbuf[NoOfBytes], mWriteBuffLevel);
#endif

    /* Write data to FLASH */
    if( NV_FlashProgramUnaligned(gEepromParams_StartOffset_c + Addr, NoOfBytes, Outbuf) )
    {
        return ee_error;
    }
    
    return ee_ok;    
}

/*****************************************************************************
*  EEPROM_ReadData
*
*  Reads a data buffer from EEPROM, from a given address
*
*****************************************************************************/
ee_err_t EEPROM_ReadData(uint16_t NoOfBytes, uint32_t Addr, uint8_t *inbuf)
{
  FLib_MemCpy(inbuf, (void*)(gEepromParams_StartOffset_c + Addr), NoOfBytes);

#if (gEepromParams_WriteAlignment_c > 1)
  uint32_t i;

  /* Copy data from write buffer if needed */
  for( i=0; i<mWriteBuffLevel; i++ )
  {
      if( (mWriteBuffAddr + i) >= Addr &&
          (mWriteBuffAddr + i) <  (Addr + NoOfBytes) )
      {
          inbuf[mWriteBuffAddr - Addr + i] = mWriteBuff[i];
      }
  }
#endif
  
  return ee_ok;
}

/*****************************************************************************
*  EEPROM_ReadStatusReq
*
*
*****************************************************************************/
uint8_t EEPROM_isBusy(void)
{
  return FALSE;
}

/******************************************************************************
*******************************************************************************
* Private Functions
*******************************************************************************
******************************************************************************/


/*****************************************************************************
*  EEPROM_WriteData
*
*  Writes a data buffer into the External Memory, at a given address
*
*****************************************************************************/
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr)
{
    uint32_t i;
    uint32_t startBlk, endBlk;
    
    /* Obtain the first and last block that need to be erased */
    startBlk = Addr / gEepromParams_SectorSize_c;
    endBlk   = (Addr + NoOfBytes) / gEepromParams_SectorSize_c;
    
    if( (Addr + NoOfBytes) & gEepromParams_SectorSize_c )
    {
        endBlk++;
    }
    
    /* Check if the block was previousley erased */
    for(i = startBlk; i <= endBlk; i++)
    {
        if ( (mEepromEraseBitmap[i/8] & (1U << (i%8) ) ) == 0)
        {
            if (EEPROM_EraseBlock(i * gEepromParams_SectorSize_c, gEepromParams_SectorSize_c) != ee_ok)
            {
                return ee_error;
            }
            mEepromEraseBitmap[i/8] |= 1U << (i%8);
        }
    }
    
    return ee_ok;
}

#endif /* gEepromDevice_InternalFlash_c */