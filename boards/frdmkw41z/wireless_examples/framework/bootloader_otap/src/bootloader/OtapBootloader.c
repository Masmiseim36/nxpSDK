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
#include "main.h"
#include "Eeprom_Boot.h"
#include "flash_boot_kinetis.h"
#include "OtapBootloader.h"


/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".BOOT_MODULE_INFO"
__root const moduleInfo_t mBootloaderInfo =
#elif defined(__GNUC__)
const moduleInfo_t mBootloaderInfo __attribute__ ((section(".BOOT_MODULE_INFO"), used)) =
#endif
{
  {'B', 'O', 'O', 'T'},
  gBootloaderVerString_c,
  gBootloaderModuleId_c,
  {gBootloaderVerMajor_c, gBootloaderVerMinor_c, gBootloaderVerPatch_c},
  gBootloaderBuildNo_c
};

/* Variables used by the Bootloader */
volatile bootInfo_t *gpBootInfo;
volatile uint32_t gBootStorageStartAddress;

uint32_t Boot_GetInternalStorageStartAddr(void);
uint8_t Boot_InitExternalStorage(void);
uint8_t Boot_ReadExternalStorage(uint16_t NoOfBytes, uint32_t Addr, uint8_t *outbuf);
void Boot_LoadImage (void);


/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   This function determines the start address of the internal sorage.
*          If value is 0xFFFFFFFF then external storage should be used!
*
* \return the start address of the internal storage
*
********************************************************************************** */
uint32_t Boot_GetInternalStorageStartAddr(void)
{
    return *((uint32_t*)(gBootProductInfoAddress_c + gInternalStorageStartAddressOffset_c));
}

/*! *********************************************************************************
* \brief   Initialize the external storage
*
* \return status
*
********************************************************************************** */
uint8_t Boot_InitExternalStorage(void)
{
    if (gBootInvalidAddress_c == gBootStorageStartAddress)
        return EEPROM_Init();

    return 0;
}


/*! *********************************************************************************
* \brief   Read data from the external stoage
*
* \param[in]  NoOfBytes  number of byter to read
* \param[in]  Addr       read memory address
* \param[out] outbuf     location where read data is stored
*
* \return error code. 0 if success
*
********************************************************************************** */
uint8_t Boot_ReadExternalStorage(uint16_t NoOfBytes, uint32_t Addr, uint8_t *outbuf)
{
    if (gBootInvalidAddress_c == gBootStorageStartAddress)
    {
        return EEPROM_ReadData(NoOfBytes, Addr, outbuf);
    }
    else
    {
        Addr += gBootStorageStartAddress;
        Boot_MemCpy(outbuf, (void*)Addr, NoOfBytes);

        return 0;
    }
}


/*! *********************************************************************************
* \brief   This function will copy the User Application from the external memory
*          into the program Flash
*
********************************************************************************** */
void Boot_LoadImage (void)
{
    static uint8_t bitmapBuffer[gBootData_SectorsBitmap_Size_c];
    bootInfo_t flags;
#ifdef FLASH_PROGRAM_SECTION_SUPPORT
    uint8_t *buffer = (uint8_t*)(0x14000000);
#else
    uint8_t  buffer[gFlashErasePage_c];
#endif
    uint32_t remaingImgSize, len;
    uint32_t flashAddr      = gUserFlashStart_d;
    uint8_t  bitMask;
    uint8_t *pBitmap;

#if defined(MCU_MK21DX256)
    vuint8_t* pFlexRamAddress;
    uint8_t  EEEDataSetSize;
    uint16_t size;
#endif

    /* Check if we have a valid internal storage start address. */
    gBootStorageStartAddress = Boot_GetInternalStorageStartAddr();

    /* Init the flash module */
    FlashInitialization();

    /* Init the external storage */
    Boot_InitExternalStorage();

    /* Read image size */
    if (Boot_ReadExternalStorage(gBootData_ImageLength_Size_c,gBootData_ImageLength_Offset_c, (uint8_t*)(&remaingImgSize)))
    {
        gHandleBootError_d();
    }

    /* Read sector bitmap */
    if (Boot_ReadExternalStorage(gBootData_SectorsBitmap_Size_c, gBootData_SectorsBitmap_Offset_c, bitmapBuffer))
    {
        gHandleBootError_d();
    }

    /* Ignore sector bitmap representing Bootloader Flash */
    len = gUserFlashStart_d / gFlashErasePage_c;
    pBitmap = &bitmapBuffer[len/8];
    bitMask = gBitMaskInit_c << (len%8);

    /* Start writing the image. Do not alter the last sector which contains HW specific data! */
    while (flashAddr < (gMcuFlashSize_c - gFlashErasePage_c))
    {
        if (remaingImgSize > gFlashErasePage_c)
        {
            len = gFlashErasePage_c;
        }
        else
        {
            len = remaingImgSize;
        }

        /* Check if bitmap indicates that this sector is write protected and shouldn't be updated */
        if( *pBitmap & bitMask )
        {
            /* Erase Flash sector */
            if (FLASH_OK != FLASH_Boot_EraseSector(flashAddr))
            {
                gHandleBootError_d();
            }

            if (len)
            {
                /* Read a new image block */
                if (Boot_ReadExternalStorage(len, flashAddr - gUserFlashStart_d + gBootData_Image_Offset_c, buffer))
                {
                    gHandleBootError_d();
                }


                if( (flashAddr <= gBootImageFlagsAddress_c) && (flashAddr + len > gBootImageFlagsAddress_c) )
                {
                    uint32_t i, offset = gBootImageFlagsAddress_c - flashAddr;
                    
                    /* Program the Flash before boot flags */
                    if(FLASH_OK != FLASH_Boot_Program(flashAddr, (uint32_t)buffer, offset))
                    {
                        gHandleBootError_d();
                    }

                    /* Keep the boot flags set  until the all image is downloaded */
                    for( i=0; i<gBootFlagSize_c; i++ )
                    {
                        flags.newBootImageAvailable[i] = gBootValueForFALSE_c;
                        flags.bootProcessCompleted[i] = gBootValueForTRUE_c;
                    }
//                    i = offset + 2 * gBootFlagSize_c;
//                    flags.bootVersion[0] = buffer[i++];
//                    flags.bootVersion[1] = buffer[i];
//                    offset += gEepromAlignAddr_d(sizeof(bootInfo_t));
                    offset += (sizeof(bootInfo_t) + (FLASH_PGM_SIZE - 1)) & (~(FLASH_PGM_SIZE - 1));

                    /* Program the Flash after the boot flags*/
                    if(FLASH_OK != FLASH_Boot_Program(flashAddr + offset, (uint32_t)(&buffer[offset]), len - offset))
                    {
                        gHandleBootError_d();
                    }
                }
                else
                {
                    /* Program the image block to Flash */
#ifdef FLASH_PROGRAM_SECTION_SUPPORT
                    if(FLASH_OK != FLASH_Boot_ProgramPreloadedSection(flashAddr, len))
#else
                    if(FLASH_OK != FLASH_Boot_Program(flashAddr, (uint32_t)buffer, len))
#endif
                    {
                        gHandleBootError_d();
                    }
                }
            }
        }

        /* Update Bitmask */
        bitMask <<= 1;
        if (bitMask == 0)
        {
            /* This was last bit in the current bitmap byte. Move to next bitmap byte */
            bitMask = gBitMaskInit_c;
            pBitmap++;
        }

        /* Update the current flash address */
        flashAddr += gFlashErasePage_c;

        /* Update the remaining bytes*/
        if (remaingImgSize)
            remaingImgSize -= len;
    } /* while */

#if defined(MCU_MK21DX256)

    while((FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK) != FTFL_FSTAT_CCIF_MASK)
    {
        /* wait till CCIF bit is set */
    }

    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FTFL_FSTAT = (FTFL_FSTAT_RDCOLERR_MASK | FTFL_FSTAT_ACCERR_MASK |
                  FTFL_FSTAT_FPVIOL_MASK);

    /* Write Command Code to FCCOB0 */
    FTFL_FCCOB0 = FLASH_READ_RESOURCE;
    /* Write address to FCCOB1/2/3 */
    FTFL_FCCOB1 = ((uint8_t)(FLASH_DFLASH_IFR_READRESOURCE_ADDRESS >> 16));
    FTFL_FCCOB2 = ((uint8_t)((FLASH_DFLASH_IFR_READRESOURCE_ADDRESS >> 8) & 0xFF));
    FTFL_FCCOB3 = ((uint8_t)(FLASH_DFLASH_IFR_READRESOURCE_ADDRESS & 0xFF));

    /* Write Resource Select Code of 0 to FCCOB8 to select IFR. Without this, */
    /* an access error may occur if the register contains data from a previous command. */
    FTFL_FCCOB8 = 0;

    /* clear CCIF bit */
    FTFL_FSTAT |= FTFL_FSTAT_CCIF_MASK;

    /* check CCIF bit */
    while((FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK) != FTFL_FSTAT_CCIF_MASK)
    {
        /* wait till CCIF bit is set */
    }

    /* read the FlexRAM window size */
    EEEDataSetSize = FTFL_FCCOB6;
    EEEDataSetSize &= 0x0F;

    if((EEEDataSetSize == 2) || (EEEDataSetSize == 3)) /* FlexRAM size is 4KB or 2KB */
    {
        /* Enable EEE */
        FLASH_Boot_SetEEEEnable(FLASH_FLEXRAM_FNC_CTRL_CODE);

        pBitmap = &bitmapBuffer[0] + (uint32_t)((gMcuFlashSize_c/gFlashErasePage_c)/8);

        if(EEEDataSetSize == 2) /* 4KB */
        {
            if(*pBitmap & 0x01)
            {
                size = gBootFlexRam_4K_Size_c/2;
                pFlexRamAddress = (vuint8_t*)((uint32_t)gBootFlexRAMBaseAddress_c);
                while(size--)
                {
                    /* wait for EEPROM system to be ready */
                    while(!(FTFL_FCNFG & FTFL_FCNFG_EEERDY_MASK));
                    *pFlexRamAddress = 0xFF;
                    pFlexRamAddress ++;
                }
            }
            if(*pBitmap & 0x02)
            {
                size = gBootFlexRam_4K_Size_c/2;
                pFlexRamAddress = (vuint8_t*)((uint32_t)(gBootFlexRAMBaseAddress_c + size));
                while(size--)
                {
                    /* wait for EEPROM system to be ready */
                    while(!(FTFL_FCNFG & FTFL_FCNFG_EEERDY_MASK));
                    *pFlexRamAddress = 0xFF;
                    pFlexRamAddress ++;
                }
            }
        }
        else if (EEEDataSetSize == 3) /* 2K */
        {
            if(*pBitmap & 0x01)
            {
                size = gBootFlexRam_2K_Size_c/2;
                pFlexRamAddress = (vuint8_t*)((uint32_t)gBootFlexRAMBaseAddress_c);
                while(size--)
                {
                    /* wait for EEPROM system to be ready */
                    while(!(FTFL_FCNFG & FTFL_FCNFG_EEERDY_MASK));
                    *pFlexRamAddress = 0xFF;
                    pFlexRamAddress ++;
                }
            }
            if(*pBitmap & 0x02)
            {
                size = gBootFlexRam_2K_Size_c/2;
                pFlexRamAddress = (vuint8_t*)((uint32_t)(gBootFlexRAMBaseAddress_c + size));
                while(size--)
                {
                    /* wait for EEPROM system to be ready */
                    while(!(FTFL_FCNFG & FTFL_FCNFG_EEERDY_MASK));
                    *pFlexRamAddress = 0xFF;
                    pFlexRamAddress ++;
                }
            }
        }
    }
#endif


    /* Set the bBootProcessCompleted Flag */
    if( FLASH_OK != FLASH_Boot_Program((uint32_t)gBootImageFlagsAddress_c, (uint32_t)&flags, sizeof(flags)) )
        gHandleBootError_d();

    /* Reseting MCU */
    Boot_ResetMCU();
}

void Boot_CheckOtapFlags(void)
{
    /* Check if there is no boot image available in the external EEPROM and if eventually
    the booting of the previous one has been completed. If both conditions are met, start
    running the application in the internal Flash. Else, start the process of booting from
    external EEPROM */
    gpBootInfo = (bootInfo_t*)gBootImageFlagsAddress_c;
    /*
    if( (gpBootInfo->newBootImageAvailable == gBootValueForTRUE_c) &&
        (gpBootInfo->bootProcessCompleted ==  gBootValueForFALSE_c) )
    */
    if ((gpBootInfo->newBootImageAvailable[0] != gBootValueForTRUE_c) &&
        (gpBootInfo->bootProcessCompleted[0] ==  gBootValueForTRUE_c))
    {
        return;
    }
    else
    {
        /* Write the new image */
        BOOT_ClockInit();
        Boot_LoadImage();
    }
}

//-----------------------------------------------------------------------------
