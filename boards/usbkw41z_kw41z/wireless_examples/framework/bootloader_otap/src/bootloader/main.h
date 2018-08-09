/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Bootloader interface file
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

#ifndef __MAIN_H__
#define __MAIN_H__


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"

#if defined(FRDM_KW24)
  #include "frdmkw24_cfg.h"
  
#elif defined(TWR_KW24D512)
  #include "twrkw24d512_cfg.h" 

#elif defined(MCU_MK21DX256)
  #include "MK21D256_cfg.h" 

#elif defined(FRDM_KL46Z)
  #include "frdmkl46z_cfg.h"

#elif defined(FRDM_KW40Z)
  #include "frdmkw40z_cfg.h"

#elif defined(FRDM_KW41Z)
  #include "frdmkw41z_cfg.h"

#elif defined(USB_KW41)
  #include "usbkw41z_kw41z_cfg.h"

#elif defined(FRDM_K64F)
  #include "frdmk64f_cfg.h"

#else
  #error Board not supported.
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*
 * Name: gMcuFlashBlocks_c
 * Description: The number of the internal flash blocks
 */

#define gMcuFlashBlocks_c FLASH_BLOCK_CNT
/*
 * Name: gMcuFlashSize_c
 * Description: The size of the internal FLASH
 */

#define gMcuFlashSize_c ((uint32_t)__ROM_SIZE__)
/*
 * Name: gFlashErasePage_c
 * Description: The minimum amount of FLASH that can be erased
 */

#define gFlashErasePage_c  FLASH_ERASE_PAGE

/*
 * Name: gUserFlashStart_d
 * Description: Starting point of the application
 */
#define gUserFlashStart_d               ((uint32_t)(__region_BOOT_ROM_end__) + 1)

#if gMcuFlashBlocks_c == 2
/*
 * Name: gUserFlash2Start_d
 * Description: Start address of the second Flash block
*/
#define gUserFlash2Start_d     USER_FLASH_START_2
/*
 * Name: gMcuFlash2End_c
 * Description: End address of the second Flash block
*/
#define gMcuFlash2End_c        USER_FLASH_END_2
#endif

/*
 * Name: gFlashProtection_c
 * Description: The value for FPROT register. By default the Flash is not Protected
 */
#ifndef gFlashProtection_c
#define gFlashProtection_c 0xFFFFFFFF /* Flash is not write protected */
// #define gFlashProtection_c 0xFFFFFFFE /* The first Flash region is write protected */
#endif

#ifndef gFlashBackDoorKey1_d
#define gFlashBackDoorKey1_d 0xFFFFFFFF
#endif

#ifndef gFlashBackDoorKey2_d
#define gFlashBackDoorKey2_d 0xFFFFFFFF
#endif

#ifndef gFlashConfigOptions_d 
#define gFlashConfigOptions_d  0xFFFFFFFE
#endif

/* Defines how the bootloader should handle errors */
#ifdef gBootLoaderDebug_c
  #define gHandleBootError_d() while(1);
#else
  #define gHandleBootError_d() Boot_ResetMCU();
#endif

#define gBootFlagInvalid_d (0xFFFFFFFFFFFFFFFF)

/* Defines the bootloader sector bitmap length */
#define gBootData_SectorsBitmap_Size_c (32)

/* Init value for the mask used to look through each byte in the bitmap that
indicates which Flash sectors are write protected and should not be updated */
#define gBitMaskInit_c (0x01)
        
#define NoOfElements(x) (sizeof(x)/sizeof(x[0]))


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/*
 * Name: FlashConfig_t
 * Description: Structure used to access Flash Configuration
 */
typedef union _FlashConfig_t
{
  uint32_t Data[4];
  struct {
    uint32_t BackDoorKey[2];
    uint32_t Protection;
    uint32_t Config;
  };
} FlashConfig_t;

typedef void (*vector_entry)(void);
typedef void pointer(void);

typedef struct bootFlags_tag{
    uint64_t newImageFlag;
}bootFlags_t;


/******************************************************************************
 *******************************************************************************
 * Public memory declarations
 *******************************************************************************
 ******************************************************************************/
extern volatile const bootFlags_t gBootFlags;

#if defined(__IAR_SYSTEMS_ICC__)
    extern uint32_t __BOOT_STACK_ADDRESS[];
#elif defined(__GNUC__)
    extern uint32_t __SP_INIT[];
#endif

extern uint32_t __region_BOOT_ROM_start__[];
extern uint32_t __region_BOOT_ROM_end__[];
extern uint32_t __BOOT_Flags_start__[];
extern uint32_t __ROM_SIZE__[];

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void defaultISR(void);
void NMI_handler(void);
void __thumb_startup(void);
int main(int argc, char **argv);
void Boot_ResetMCU(void);
void EnableRamVectorTable(void);
void InstallIrqHandler(IRQn_Type irqType, void (*handler)(void));
void Boot_MemCpy (void* pDst, void* pSrc, uint32_t cBytes);
void BOOT_ClockInit(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
