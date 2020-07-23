/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "Flash_Adapter.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#if gNvStorageIncluded_d
#include "NVM_Interface.h"
#endif
#include "Panic.h"
#if (defined(CPU_K32W042S1M2CAx_cm4) || defined(CPU_K32W042S1M2VPJ_cm4))
#include "fsl_mu.h"
#endif

/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/
#if (PGM_SIZE_BYTE == 4)
#define mProgBuffSizeInPgmWrUnits_c  16U
#elif (PGM_SIZE_BYTE == 8)
#define mProgBuffSizeInPgmWrUnits_c  8U
#else
#define mProgBuffSizeInPgmWrUnits_c  4U
#endif

/* Generator for CRC calculations. */
#define POLGEN  0x1021U

#if gNvPauseOtherCoreInRAM_c
#define THREAD_MU_CHANNEL           2U
#define CORE_B_IN_RAM               1U
#define CORE_B_OUT_OF_RAM           2U
#endif /* gNvPauseOtherCoreInRAM_c */

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
#if gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE
/*
 * Structure holding the assembly code that is executed from RAM. See
 * mWaitInRamFunctionCode below for the number of lines of code and number of
 * registers.
*/
typedef struct waitInRamStruct_tag{
    uint16_t aInstructions[20]; /*< number of lines of code */
    uint32_t aDict[3];          /*< number of register values needed */
} waitInRamStruct_t;
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static uint32_t NV_FlashProgramAdaptation(uint32_t dest, uint32_t size, uint8_t* pData);
#ifndef CPU_QN908X
static uint8_t  NV_VerifyCrcOverHWParameters(hardwareParameters_t* pHwParams);
static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t* pHwParams);
#endif
static void NV_Flash_WaitForCSEndAndDisableInterrupts(void);
#ifdef CPU_QN908X
static uint32_t SwFlashVerifyErase (uint32_t start, uint32_t lengthInBytes);
#endif

#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
static void NV_WaitForBlackboxRam();
static void NV_BlackboxExitRam();
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/* Hardware parameters */
hardwareParameters_t gHardwareParameters;
flash_config_t gFlashConfig;
#if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
flexnvm_config_t gFlexNvmDriver;
static uint32_t mDflashBlockBase = 0U;
#endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#ifndef CPU_QN908X
static const uint8_t mProdDataIdentifier[10] = {"PROD_DATA:"};
#endif
#if (USE_RTOS)
static osaSemaphoreId_t       mFlashAdapterSemaphoreId;
#endif
static volatile uint8_t mFA_CSFlag = 0U;
static volatile uint8_t mFA_SemWaitCount = 0U;

#if gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE
/*
    Dissasembly code was generated with IAR 8.32.1 for ARMv6. Make sure that the values
    CORE_B_IN_RAM, CORE_B_OUT_OF_RAM and THREAD_MU_CHANNEL are the ones defined in C.
    C code:
    asm ("CPSID I");
    MU_SendMsg(MUB, THREAD_MU_CHANNEL, CORE_B_IN_RAM);
    while (!(MUB->SR & (kMU_Tx0EmptyFlag >> THREAD_MU_CHANNEL)));
    MUB->TR[THREAD_MU_CHANNEL] = CORE_B_IN_RAM;

    while (MU_ReceiveMsg(MUB, THREAD_MU_CHANNEL) != CORE_B_OUT_OF_RAM);
     Wait TX register to be empty.
    while (!(MUB->SR & (kMU_Tx0EmptyFlag >> THREAD_MU_CHANNEL)));
    MUB->TR[THREAD_MU_CHANNEL] = CORE_B_IN_RAM;
    do
    {
         Wait RX register to be full.
        while (!(MUB->SR & (kMU_Rx0FullFlag >> THREAD_MU_CHANNEL)));
    } while (MUB->RR[THREAD_MU_CHANNEL] != CORE_B_OUT_OF_RAM);

    asm ("CPSIE I");
*/
static waitInRamStruct_t mWaitInRamFunctionCode = {
  // code
  {
    0xB500,     // PUSH         {LR}
    0xB672,     // CPSID        I
    0x4808,     // LDR.N        R0, [PC, #32]
    0x6801,     // LDR          R1, [R0]
    0x0289,     // LSLS         R1, R1, #10
    0xD5FB,     // BPL.N        3 instruction back

    0x2101,     // MOVS         R1, 1                   ; CORE_B_IN_RAM = 1
    0x4a07,     // LDR.N        R2, [PC, #]
    0x6011,     // STR          R1, [R2]
    0x6801,     // LDR          R1, [R0]
    0x0189,     // LSL          R1, R1, #6
    0xd5fc,     // BPL.N        2 instructions back
    0x4905,     // LDR.N        R1, [PC, #]
    0x6809,     // LDR          R1, [R1]
    0x2902,     // CMP          R1, #2                  ; CORE_B_OUT_OF_RAM = 2
    0xd1f8,     // BNE.N        6 instructions back

    0xb662,     // CPSIE        I
    0xbd00,     // POP          {PC}
    0x0000,     // NOP          ; align
    0x0000,     // NOP          ; align
  },
  // register addresses
  {
    0x41024060,  // MU_SR
    0x41024028,  // MU_TR[2]                            ; THREAD_MU_CHANNEL = 2
    0x41024048,  // MU_RR[2]                            ; THREAD_MU_CHANNEL = 2
  }
};
static void (*mpWaitInRamFunction)() = (void (*)(void))((uint32_t)&mWaitInRamFunctionCode + 1);
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE */

/*****************************************************************************
 *****************************************************************************
 * Private functions
 *****************************************************************************
 *****************************************************************************/
/*! *********************************************************************************
 * \brief This function ensures that there are no critical sections on and disables interrupts.
 *
 * \param[in] none
 * \return nothing
 *
********************************************************************************** */
static void NV_Flash_WaitForCSEndAndDisableInterrupts(void)
{
#if (USE_RTOS)
#if (gNvStorageIncluded_d)
    /*on freeRTOS NvIdle runs on a freeRTOS idle task hook which must never block. In this RTOS at least one task must be ready */
    /*since NvIdle runs on the task which has the least priority in the system we can wait in a loop for the end of the critical section */
    osaTaskId_t currentTaskId = OSA_TaskGetId();
    if(currentTaskId == (osaTaskId_t)NvGetNvIdleTaskId())
    {
        while(TRUE)
        {
            while (mFA_CSFlag);
            OSA_InterruptDisable();
            if(mFA_CSFlag == 0U)
            {
                break;
            }
            OSA_InterruptEnable();
        }
    }
    else
#endif
    {
        while(TRUE)
        {
            OSA_InterruptDisable();
            if(mFA_CSFlag == 0U)
            {
                break;
            }
            mFA_SemWaitCount++;
            OSA_InterruptEnable();
            OSA_SemaphoreWait(mFlashAdapterSemaphoreId, osaWaitForever_c);
        }
    }
 #else
    OSA_InterruptDisable();
#endif
}
/*! *********************************************************************************
 * \brief  Write aligned data to FLASH
 *
 * \param[in] dest        The address of the Flash location
 * \param[in] size        The number of bytes to be programed
 * \param[in] pData       Pointer to the data to be programmed to Flash
 *
 * \return error code
 *
********************************************************************************** */
static uint32_t NV_FlashProgramAdaptation(uint32_t dest, uint32_t size, uint8_t* pData)
{
  uint32_t progBuf[PGM_SIZE_BYTE/sizeof(uint32_t)];
  uint32_t status = (uint32_t)kStatus_FLASH_Success;
  uint32_t pFlashEndadd = 0U;

  if( (size & ((uint32_t)PGM_SIZE_BYTE - 0x01U)) != 0U )
  {
    status = (uint32_t)kStatus_FTFx_AlignmentError;
  }
  else
  {
    while(size != 0U)
    {
      FLib_MemCpy(progBuf, pData, PGM_SIZE_BYTE);

  #if gNvDisableIntCmdSeq_c
      NV_Flash_WaitForCSEndAndDisableInterrupts();
  #endif
  #if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
      NV_BlackboxEnterRam();
      NV_WaitForBlackboxRam();
  #endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */
      pFlashEndadd = FSL_FEATURE_FLASH_PFLASH_START_ADDRESS + FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT*FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
      if (dest < pFlashEndadd)
      {
          status = (uint32_t)FLASH_Program(&gFlashConfig, dest, (uint8_t*)progBuf, PGM_SIZE_BYTE);
      }
  #if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
      else if (dest < (pFlashEndadd + (uint32_t)((uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT*(uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE)))
      {
           status = (uint32_t)FLEXNVM_DflashProgram(&gFlexNvmDriver, mDflashBlockBase + dest, (uint8_t*)progBuf, PGM_SIZE_BYTE);
      }
      else
      {
           status = (uint32_t)kStatus_FTFx_AddressError;
          /*MISRA rule 15.7*/
      }
  #endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
  #if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
      NV_BlackboxExitRam();
  #endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */
  #if gNvDisableIntCmdSeq_c
      OSA_InterruptEnable();
  #endif

      if(status != (uint32_t)kStatus_FLASH_Success)
      {
        break;
      }

      pData += PGM_SIZE_BYTE;
      dest += (uint32_t)PGM_SIZE_BYTE;
      size -= (uint32_t)PGM_SIZE_BYTE;
    }
  }
  return status;
}

#ifndef CPU_QN908X
/*! *********************************************************************************
 * \brief  Verifies if the CRC field matches computed CRC over stored values
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return 1 on success, 0 otherwise
 *
********************************************************************************** */
static uint8_t NV_VerifyCrcOverHWParameters(hardwareParameters_t* pHwParams)
{
    uint8_t status = 0U;

    if(NV_ComputeCrcOverHWParameters(pHwParams) == pHwParams->hardwareParamsCrc)
    {
        status = 1U;
    }
    else
    {
        status = 0U;
    }
    return status;
}

/*! *********************************************************************************
 * \brief  Computes the CRC for the hardware parameters and stores it
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return Computed CRC value.
 *
********************************************************************************** */
static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t* pHwParams)
{
    uint16_t  computedCRC = 0U;
    uint8_t crcA;
    uint8_t byte = 0U;

    uint8_t *ptr = (uint8_t *)(&pHwParams->reserved);
    uint16_t len = (uint16_t)((uint32_t)(uint8_t *)(&pHwParams->hardwareParamsCrc) -
                       (uint32_t)(uint8_t *)(&pHwParams->reserved));
    while(len != 0U)
    {
        byte = *ptr;
        computedCRC ^= ((uint16_t)byte << 8U);
        for(crcA = 8U; crcA != 0U; crcA--)
        {
            if((computedCRC & 0x8000U) != 0U)
            {
                computedCRC <<= 1U;
                computedCRC ^= POLGEN;
            }
            else
            {
                computedCRC <<= 1U;
            }
        }
        --len;
        ++ptr;
    }
    return computedCRC;
}
#endif

#ifdef CPU_QN908X
/*! *********************************************************************************
 * \brief  Flash verify erase software implementation
 *
 * \param[in] start - start address of the memory block to be verified
 * \param[in] lengthInBytes - memory block length
 *
 * \return TRUE if the flash memory area is blank (erased), FALSE otherwise
 *
********************************************************************************** */
uint32_t SwFlashVerifyErase (uint32_t start, uint32_t lengthInBytes)
{
    uint8_t* pAddress = (uint8_t*)start;
    flash_status_t status = kStatus_FLASH_Success;

    do
    {
        if(*pAddress++ != 0xffU)
        {
            status = kStatus_FLASH_EraseError;
            break;
        }
    } while(--lengthInBytes);

    return (uint32_t)status;
}
#endif

#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
/*! *********************************************************************************
 * \brief  Instructs other core to jump in a RAM loop with interrupts disabled
********************************************************************************** */
static void NV_WaitForBlackboxRam()
{
    while (MU_ReceiveMsg(MUA, THREAD_MU_CHANNEL) != CORE_B_IN_RAM);
}

/*! *********************************************************************************
 * \brief  Instructs other core to exit the RAM loop and re-enable interrupts
********************************************************************************** */
static void NV_BlackboxExitRam()
{
    MU_SendMsg(MUA, THREAD_MU_CHANNEL, CORE_B_OUT_OF_RAM);
}
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Initialize the FLASH driver
 *
********************************************************************************** */
void NV_Init(void)
{
    static bool_t nvmInit = FALSE;
    if(!nvmInit)
    {
#ifdef CPU_QN908X
        /* Configure Flash  */
        FLASH_GetDefaultConfig(&gFlashConfig);
        gFlashConfig.blockBase = 0x0U;
#endif /* CPU_QN908X */
#ifdef gNVM_MULTICORE_SUPPORT_d
        FLASH_SetProperty(&gFlashConfig, kFLASH_PropertyFlashMemoryIndex, 1);
#endif
        /* Init Flash */
        (void)FLASH_Init(&gFlashConfig);

#if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
        OSA_InterruptDisable();
        (void)FLEXNVM_Init(&gFlexNvmDriver);
        OSA_InterruptEnable();
        (void)FLEXNVM_GetProperty(&gFlexNvmDriver, kFLEXNVM_PropertyDflashBlockBaseAddr, &mDflashBlockBase);
        mDflashBlockBase = mDflashBlockBase - (uint32_t)((uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT*(uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE);
#endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#if (USE_RTOS)
        if( (mFlashAdapterSemaphoreId = OSA_SemaphoreCreate(0)) == NULL )
        {
            panic( ID_PANIC(0,0), (uint32_t)NV_Init, 0, 0 );
        }
#endif
        nvmInit = TRUE;
    }
}
/******************************************************************************
 * Name: NvSetCriticalSection
 * Description: enter critical section
 * Parameters: -
 * Return: -
 ******************************************************************************/
/*! *********************************************************************************
 * \brief  Start a critical section during which flash operations are not allowed
 *
 * \param[in] none
 * \return nothing
 *
********************************************************************************** */
void NV_Flash_SetCriticalSection
(
void
)
{
#if (USE_RTOS)
    OSA_InterruptDisable();
    ++mFA_CSFlag;
    OSA_InterruptEnable();
#endif
}

/*! *********************************************************************************
 * \brief  Start a critical section during which flash operations are not allowed
 *
 * \param[in] none
 * \return nothing
 *
********************************************************************************** */
void NV_Flash_ClearCriticalSection
(
void
)
{
#if (USE_RTOS)
    OSA_InterruptDisable();
    if(mFA_CSFlag)
    {
        mFA_CSFlag--;
    }
    OSA_InterruptEnable();
    while(TRUE)
    {
        OSA_InterruptDisable();

        if(mFA_CSFlag)
        {
            break;
        }
        if(mFA_SemWaitCount == 0U)
        {
            break;
        }
        mFA_SemWaitCount--;
        OSA_InterruptEnable();
        OSA_SemaphorePost(mFlashAdapterSemaphoreId);
    }
    OSA_InterruptEnable();
#endif
}

/*! *********************************************************************************
 * \brief  Write aligned data to FLASH
 *
 * \param[in] start                The address of the Flash location
 * \param[in] lengthInBytes        The number of bytes to be programed
 * \param[in] margin
  * \return error code
 *
********************************************************************************** */
uint32_t NV_FlashVerifyErase ( uint32_t start, uint32_t lengthInBytes
#ifndef CPU_QN908X
, ftfx_margin_value_t margin
#endif
)
{
    uint32_t status = (uint32_t)kStatus_FLASH_Success;
#ifndef CPU_QN908X
    uint32_t pFlashEndadd = 0U;
#endif /* CPU_QN908X */
#if gNvDisableIntCmdSeq_c
    NV_Flash_WaitForCSEndAndDisableInterrupts();
#endif
#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
    NV_BlackboxEnterRam();
    NV_WaitForBlackboxRam();
#endif /* MULTICORE_APPLICATION_CORE */
#ifdef CPU_QN908X
    status = SwFlashVerifyErase (start, lengthInBytes);
#else
    pFlashEndadd = FSL_FEATURE_FLASH_PFLASH_START_ADDRESS + FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT*FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
    if (start < pFlashEndadd)
    {
         status = (uint32_t)FLASH_VerifyErase(&gFlashConfig, start, lengthInBytes, margin);
    }
#if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
    else if (start < (pFlashEndadd + (uint32_t)((uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT*(uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE)))
    {
         status = (uint32_t)FLEXNVM_DflashVerifyErase(&gFlexNvmDriver, mDflashBlockBase + start, lengthInBytes, margin);
    }
    else
    {
         status = (uint32_t)kStatus_FTFx_AddressError;
        /*MISRA rule 15.7*/
    }
#endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#endif
#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
    NV_BlackboxExitRam();
#endif /* MULTICORE_APPLICATION_CORE */
#if gNvDisableIntCmdSeq_c
    OSA_InterruptEnable();
#endif
  return status;
}
/*! *********************************************************************************
 * \brief  Write aligned data to FLASH
 *
 * \param[in] pSSDConfig  Pointer to a flash config structure
 * \param[in] dest        The address of the Flash location
 * \param[in] size        The number of bytes to be programed
 * \param[in] pData       Pointer to the data to be programmed to Flash
 * \param[in] pFlashCommandSequence  Pointer to the Flash RAM function
 *
 * \return error code
 *
********************************************************************************** */
uint32_t NV_FlashProgram (         uint32_t dest,
                                   uint32_t size,
                                   uint8_t* pData)
{
    return NV_FlashProgramAdaptation(dest, size, pData);
}

/*! *********************************************************************************
 * \brief  Write data to FLASH
 *
 * \param[in] pSSDConfig  Pointer to a flash config structure
 * \param[in] dest        The address of the Flash location
 * \param[in] size        The number of bytes to be programed
 * \param[in] pData       Pointer to the data to be programmed to Flash
 * \param[in] pFlashCommandSequence  Pointer to the Flash RAM function
 *
 * \return error code
 *
********************************************************************************** */
uint32_t NV_FlashProgramUnaligned(  uint32_t dest,
                                    uint32_t size,
                                    uint8_t* pData)
{
    uint8_t  buffer[PGM_SIZE_BYTE];
    uint32_t bytes = dest & ((uint32_t)PGM_SIZE_BYTE - 1U);
    uint32_t nLeftBytes;
    uint32_t status = (uint32_t)kStatus_FLASH_Success;

    if( bytes != 0U )
    {
        uint32_t unalignedBytes = (uint32_t)PGM_SIZE_BYTE - bytes;

        if( unalignedBytes > size )
        {
            unalignedBytes = size;
        }

        FLib_MemCpy(buffer, (uint8_t*)(dest - bytes), PGM_SIZE_BYTE);
        FLib_MemCpy(&buffer[bytes], pData, unalignedBytes);

        status = NV_FlashProgramAdaptation(dest - bytes, PGM_SIZE_BYTE, buffer);
        if( (uint32_t)kStatus_FLASH_Success == status )
        {
            dest += (uint32_t)PGM_SIZE_BYTE - bytes;
            pData += unalignedBytes;
            size -= unalignedBytes;
        }
    }
    nLeftBytes = size & ((uint32_t)PGM_SIZE_BYTE - 1U);
    if( (uint32_t)kStatus_FLASH_Success == status )
    {
        bytes = size & ~((uint32_t)PGM_SIZE_BYTE - 1U);

        if( bytes != 0U )
        {
            status = NV_FlashProgramAdaptation(dest, bytes, pData);
            if( (uint32_t)kStatus_FLASH_Success == status )
            {
                dest  += bytes;
                pData += bytes;
                nLeftBytes = size -bytes;
            }
        }

        if( (uint32_t)kStatus_FLASH_Success == status )
        {
            if( nLeftBytes != 0U )
            {
                FLib_MemCpy(buffer, (uint8_t*)dest, PGM_SIZE_BYTE);
                FLib_MemCpy(buffer, pData, nLeftBytes);
                status = NV_FlashProgramAdaptation(dest, PGM_SIZE_BYTE, buffer);
            }
        }
    }
    return status;
}

/*! *********************************************************************************
 * \brief  Erase to 0xFF one ore more FLASH sectors.
 *
 * \param[in] pSSDConfig  Pointer to a flash config structure
 * \param[in] dest        The start address of the first sector to be erased
 * \param[in] size        The amount of flash to be erased (multiple of sector size)
 * \param[in] pFlashCommandSequence  Pointer to the Flash RAM function
 *
 * \return error code
 *
********************************************************************************** */
uint32_t NV_FlashEraseSector(uint32_t dest, uint32_t size)
{
    uint32_t status = (uint32_t)kStatus_FLASH_Success;
#ifndef CPU_QN908X
    uint32_t pFlashEndadd = 0U;
#endif /* CPU_QN908X */

#ifdef CPU_QN908X
    uint32_t status_flags;
#endif

#if gNvDisableIntCmdSeq_c
    NV_Flash_WaitForCSEndAndDisableInterrupts();
#endif
#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
    NV_BlackboxEnterRam();
    NV_WaitForBlackboxRam();
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */
#ifdef CPU_QN908X
    status_flags = FLASH_GetStatusFlags();
    if(status_flags & FLASH_INT_STAT_AHBL_INT_MASK)
    {
      FLASH_ClearStatusFlags(FLASH_INTCLR_AHBL_INTCLR_MASK);
    }
    if(status_flags & FLASH_INT_STAT_AHBH_INT_MASK)
    {
      FLASH_ClearStatusFlags(FLASH_INTCLR_AHBH_INTCLR_MASK);
    }
    status = FLASH_Erase(&gFlashConfig, dest, size);
#else
    pFlashEndadd = FSL_FEATURE_FLASH_PFLASH_START_ADDRESS + FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT*FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
    if (dest < pFlashEndadd)
    {
         status = (uint32_t)FLASH_Erase(&gFlashConfig, dest, size, (uint32_t)kFLASH_ApiEraseKey);
    }
#if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
    else if (dest < (pFlashEndadd + (uint32_t)((uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT*(uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE)))
    {
         status = (uint32_t)FLEXNVM_DflashErase(&gFlexNvmDriver, mDflashBlockBase + dest, size, (uint32_t)kFTFx_ApiEraseKey);
    }
    else
    {
        status = (uint32_t)kStatus_FTFx_AddressError;
        /*MISRA rule 15.7*/
    }
#endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#endif
#if gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE
    NV_BlackboxExitRam();
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_APPLICATION_CORE */
#if gNvDisableIntCmdSeq_c
    OSA_InterruptEnable();
#endif
    return status;
}

#ifndef CPU_QN908X
/*! *********************************************************************************
 * \brief  Load the HW parameters from Flash to RAM
 *
 * \param[in] pHwParams  pointer to a structure where HW parameters will be stored
 *
 * \return error code
 *
********************************************************************************** */
uint32_t NV_ReadHWParameters(hardwareParameters_t *pHwParams)
{
    hardwareParameters_t* pLocalParams = (hardwareParameters_t*)(uint32_t)&FREESCALE_PROD_DATA_BASE_ADDR[0];
    uint32_t status = 0;

    assert(FREESCALE_PROD_DATA_BASE_ADDR);
    assert(pHwParams);

    if( FLib_MemCmp(FREESCALE_PROD_DATA_BASE_ADDR, pHwParams, sizeof(hardwareParameters_t)) )
    {
        /* pHwParams already copied from FREESCALE_PROD_DATA_BASE_ADDR , nothing to do */
    }
    else if(FLib_MemCmp(FREESCALE_PROD_DATA_BASE_ADDR, (const void*)mProdDataIdentifier, sizeof(mProdDataIdentifier)) &&
       (NV_VerifyCrcOverHWParameters(pLocalParams) != 0U))
    {
        FLib_MemCpy(pHwParams, FREESCALE_PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t));
    }
    else
    {
        FLib_MemSet(pHwParams, 0xFF, sizeof(hardwareParameters_t));
        status = 1;
    }

    return status;
}

/*! *********************************************************************************
 * \brief  Store the HW parameters to Flash
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return error code of the Flash erase/write functions
 *
********************************************************************************** */
uint32_t NV_WriteHWParameters(hardwareParameters_t *pHwParams)
{
    uint32_t status = 0;
    assert(pHwParams);

    assert(FREESCALE_PROD_DATA_BASE_ADDR);

    NV_Init();

    if(!FLib_MemCmp(pHwParams, (void*)FREESCALE_PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t)))
    {
        pHwParams->hardwareParamsCrc = NV_ComputeCrcOverHWParameters(pHwParams);
        FLib_MemCpy(pHwParams->identificationWord, (const void*)mProdDataIdentifier, sizeof(mProdDataIdentifier));

#if defined(FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)
        status = NV_FlashEraseSector((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR, FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE);
#elif defined(FSL_FEATURE_FLASH_PAGE_SIZE_BYTES)
        status = NV_FlashEraseSector((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR, FSL_FEATURE_FLASH_PAGE_SIZE_BYTES);
#endif

        if( 0U == status )
        {
            status = NV_FlashProgramUnaligned((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR,
                                              sizeof(hardwareParameters_t),
                                              (uint8_t*)pHwParams);
        }
    }
    return status;
}

#if gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE
/*! *********************************************************************************
 * \brief  Enter a loop with interrupts disabled, executed from RAM
********************************************************************************** */
void NV_BlackboxEnterRam()
{
    mpWaitInRamFunction();
}
#endif /* gNvPauseOtherCoreInRAM_c && MULTICORE_CONNECTIVITY_CORE */

#endif
