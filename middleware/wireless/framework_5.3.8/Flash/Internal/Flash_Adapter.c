/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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
/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/
#if (PGM_SIZE_BYTE == 4)
#define mProgBuffSizeInPgmWrUnits_c  16
#elif (PGM_SIZE_BYTE == 8)
#define mProgBuffSizeInPgmWrUnits_c  8
#else
#define mProgBuffSizeInPgmWrUnits_c  4
#endif

/* Generator for CRC calculations. */
#define POLGEN  0x1021
/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static uint32_t NV_FlashProgramAdaptation(uint32_t dest, uint32_t size, uint8_t* pData);
static uint8_t  NV_VerifyCrcOverHWParameters(hardwareParameters_t* pHwParams);
static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t* pHwParams);
static void NV_Flash_WaitForCSEndAndDisableInterrupts(void);
/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
#if defined(__CC_ARM)
extern uint32_t Image$$PROD_DATA$$Base[];
#define FREESCALE_PROD_DATA_BASE_ADDR Image$$PROD_DATA$$Base
#else
extern uint32_t FREESCALE_PROD_DATA_BASE_ADDR[];
#endif /* defined(__CC_ARM) */

/* Hardware parameters */
hardwareParameters_t gHardwareParameters;
flash_config_t gFlashConfig;
static const uint8_t mProdDataIdentifier[10] = {"PROD_DATA:"};
#if (USE_RTOS)
static osaSemaphoreId_t       mFlashAdapterSemaphoreId;
#endif
static volatile uint8_t mFA_CSFlag = 0;
static volatile uint8_t mFA_SemWaitCount = 0;
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
        while(1)
        {
            while (mFA_CSFlag);
            OSA_InterruptDisable();
            if(mFA_CSFlag == 0)
            {
                break;
            }
            OSA_InterruptEnable();
        }
    }
    else
#endif
    {
        while(1)
        {
            OSA_InterruptDisable();
            if(mFA_CSFlag == 0)
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
 * \brief  Write alligned data to FLASH
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
  uint32_t status = kStatus_FLASH_Success;

  if( (size & (PGM_SIZE_BYTE - 0x01U)) != 0 )
  {
    return kStatus_FLASH_AlignmentError;
  }

  while(size)
  {
    FLib_MemCpy(progBuf, pData, PGM_SIZE_BYTE);

#if gNvDisableIntCmdSeq_c
    NV_Flash_WaitForCSEndAndDisableInterrupts();
#endif
    status = FLASH_Program(&gFlashConfig, dest, (uint32_t*)progBuf, PGM_SIZE_BYTE);
#if gNvDisableIntCmdSeq_c
    OSA_InterruptEnable();
#endif

    if(status != kStatus_FLASH_Success)
    {
      break;
    }

    pData += PGM_SIZE_BYTE;
    dest += PGM_SIZE_BYTE;
    size -= PGM_SIZE_BYTE;
  }

  return status;
}

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
    uint8_t status = 0;
    
    if(NV_ComputeCrcOverHWParameters(pHwParams) == pHwParams->hardwareParamsCrc)
    {
        status = 1;
    }
    else
    {
        status = 0;
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
    uint16_t  computedCRC = 0;
    uint8_t crcA;
    uint8_t byte = 0;
    if(NULL != pHwParams)
    {
        uint8_t *ptr = (uint8_t *)(&pHwParams->reserved);
        uint16_t len = (uint8_t *)(&pHwParams->hardwareParamsCrc) -
                           (uint8_t *)(&pHwParams->reserved);
        while(len)
        {
            byte = *ptr;
            computedCRC ^= ((uint16_t)byte << 8);
            for(crcA = 8; crcA; crcA--)
            {
                if(computedCRC & 0x8000) {
                    computedCRC <<= 1;
                    computedCRC ^= POLGEN;
                }
                else computedCRC <<= 1;
            }
            --len;
            ++ptr;
        }
    }
    return computedCRC;
}
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
    static bool_t nvmInit = 0;
    if(nvmInit == 0)
    {
        FLASH_Init(&gFlashConfig);
#if (USE_RTOS)
        if( (mFlashAdapterSemaphoreId = OSA_SemaphoreCreate(0)) == NULL )
        {
            panic( ID_PANIC(0,0), (uint32_t)NV_Init, 0, 0 );
        }
#endif
        nvmInit = 1;
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
    while(1)
    {
        OSA_InterruptDisable();

        if(mFA_CSFlag)
        {
            break;
        }
        if(mFA_SemWaitCount == 0)
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
 * \brief  Write alligned data to FLASH
 *
 * \param[in] start                The address of the Flash location
 * \param[in] lengthInBytes        The number of bytes to be programed
 * \param[in] margin
  * \return error code
 *
********************************************************************************** */
uint32_t NV_FlashVerifyErase ( uint32_t start, uint32_t lengthInBytes, flash_margin_value_t margin)
{
  uint32_t status;
#if gNvDisableIntCmdSeq_c
    NV_Flash_WaitForCSEndAndDisableInterrupts();
#endif
    status = FLASH_VerifyErase(&gFlashConfig, start, lengthInBytes, margin);
#if gNvDisableIntCmdSeq_c
    OSA_InterruptEnable();
#endif
  return status;
}
/*! *********************************************************************************
 * \brief  Write alligned data to FLASH
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
    uint16_t bytes = dest & (PGM_SIZE_BYTE-1);
    uint32_t status;

    if( bytes )
    {
        uint16_t unalignedBytes = PGM_SIZE_BYTE - bytes;

        if( unalignedBytes > size )
        {
            unalignedBytes = size;
        }

        FLib_MemCpy(buffer, (void*)(dest - bytes), PGM_SIZE_BYTE);
        FLib_MemCpy(&buffer[bytes], pData, unalignedBytes);

        if((status = NV_FlashProgramAdaptation(dest - bytes, PGM_SIZE_BYTE, buffer)) != kStatus_FLASH_Success)
        {
            return status;
        }

        dest += PGM_SIZE_BYTE - bytes;
        pData += unalignedBytes;
        size -= unalignedBytes;
    }

    bytes = size & ~(PGM_SIZE_BYTE - 1U);

    if( bytes )
    {
        if((status = NV_FlashProgramAdaptation(dest, bytes, pData)) != kStatus_FLASH_Success)
        {
            return status;
        }

        dest  += bytes;
        pData += bytes;
        size  -= bytes;
    }
    
    if( size )
    {
        FLib_MemCpy(buffer, (void*)dest, PGM_SIZE_BYTE);
        FLib_MemCpy(buffer, pData, size);
        if((status = NV_FlashProgramAdaptation(dest, PGM_SIZE_BYTE, buffer)) != kStatus_FLASH_Success)
        {
            return status;
        }
    }
    
    return kStatus_FLASH_Success;
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
    uint32_t status;
#if gNvDisableIntCmdSeq_c
    NV_Flash_WaitForCSEndAndDisableInterrupts();
#endif
    status = FLASH_Erase(&gFlashConfig, dest, size, kFLASH_ApiEraseKey);
#if gNvDisableIntCmdSeq_c
    OSA_InterruptEnable();
#endif
    return status;
}

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
    hardwareParameters_t* pLocalParams = (hardwareParameters_t*)FREESCALE_PROD_DATA_BASE_ADDR;
    uint32_t status = 0;
    if(NULL != pHwParams)
    {
        if(FLib_MemCmp(FREESCALE_PROD_DATA_BASE_ADDR, (void*)mProdDataIdentifier, sizeof(mProdDataIdentifier)) &&
           NV_VerifyCrcOverHWParameters(pLocalParams))
        {
            FLib_MemCpy(pHwParams, FREESCALE_PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t));
        }
        else
        {
            FLib_MemSet(pHwParams, 0xFF, sizeof(hardwareParameters_t));
            status = 1;
        }
    }
    else
    {
        status = 2;
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
    NV_Init();
    
    if(!FLib_MemCmp(pHwParams, (void*)FREESCALE_PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t)))
    {
        pHwParams->hardwareParamsCrc = NV_ComputeCrcOverHWParameters(pHwParams);
        FLib_MemCpy(pHwParams->identificationWord, (void*)mProdDataIdentifier, sizeof(mProdDataIdentifier));

        status = NV_FlashEraseSector((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR, FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE);
        if( 0 == status )
        {
            status = NV_FlashProgramUnaligned((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR,
                                              sizeof(hardwareParameters_t),
                                              (uint8_t*)pHwParams);
        }
    }
    return status;
}