/*! *********************************************************************************
 * Copyright 2019-2022 NXP
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

#include "fwk_config.h"

#include "FunctionLib.h"
#include "fsl_common.h"
#include "fsl_adapter_flash.h"
#include "HWParameter.h"

/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/

/* Generator for CRC calculations. */
#define POLGEN 0x1021U

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/* Hardware parameters */
static hardwareParameters_t  gHardwareParameters;
static hardwareParameters_t *gHardwareParameters_p = NULL;

static const uint8_t mProdDataIdentifier[10] = {"PROD_DATA:"};

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$PROD_DATA_REGION$$Base;
#define PROD_DATA_BASE_ADDR &Image$$PROD_DATA_REGION$$Base

#else
extern uint32_t PROD_DATA_BASE_ADDR[];
#endif /* defined(__CC_ARM) */

/*****************************************************************************
 *****************************************************************************
 * Private functions
 *****************************************************************************
 *****************************************************************************/

/*! *********************************************************************************
 * \brief  Computes the CRC for the hardware parameters and stores it
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return Computed CRC value.
 *
 ********************************************************************************** */
static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t *pHwParams)
{
    uint16_t computedCRC = 0U;
    uint8_t  crcA;
    uint8_t  byte = 0U;

    uint8_t *ptr = (uint8_t *)(&pHwParams->bluetooth_address[0]);
    uint16_t len = (uint16_t)((uint32_t)(uint8_t *)(&pHwParams->hardwareParamsCrc) -
                              (uint32_t)(uint8_t *)(&pHwParams->bluetooth_address[0]));
    while (len != 0U)
    {
        byte = *ptr;
        computedCRC ^= ((uint16_t)byte << 8U);
        for (crcA = 8U; crcA != 0U; crcA--)
        {
            if ((computedCRC & 0x8000U) != 0U)
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

/*! *********************************************************************************
 * \brief  Verifies if the CRC field matches computed CRC over stored values
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return 1 on success, 0 otherwise
 *
 ********************************************************************************** */
static uint8_t NV_VerifyCrcOverHWParameters(hardwareParameters_t *pHwParams)
{
    uint8_t status = 0U;

    if (NV_ComputeCrcOverHWParameters(pHwParams) == pHwParams->hardwareParamsCrc)
    {
        status = 1U;
    }
    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Load the HW parameters from Flash to RAM on first call, give the pointer
 *  on it if HW parameters are already in RAM
 *
 * \param[in] pHwParams  pointer of pointer to a structure where HW parameters will
 * be stored
 *
 * \return error code
 *
 ********************************************************************************** */
uint32_t NV_ReadHWParameters(hardwareParameters_t **pHwParams)
{
    uint32_t status = 0U;
    assert(PROD_DATA_BASE_ADDR);
    assert(*pHwParams == NULL);

    if (gHardwareParameters_p == NULL)
    {
        /*Load in RAM the hardware parameters in Flash*/
        hardwareParameters_t *pLocalParams = (hardwareParameters_t *)(uint32_t)PROD_DATA_BASE_ADDR;

        if (FLib_MemCmp(PROD_DATA_BASE_ADDR, (const void *)mProdDataIdentifier, sizeof(mProdDataIdentifier)) &&
            (NV_VerifyCrcOverHWParameters(pLocalParams) != 0U))
        {
            /*Copy the local value in our static structure*/
            FLib_MemCpy(&gHardwareParameters, PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t));
        }
        else
        {
            FLib_MemSet(&gHardwareParameters, 0xFF, sizeof(hardwareParameters_t));
            status = 1U;
        }
        /*Assign a static pointer on our static structure*/
        gHardwareParameters_p = &gHardwareParameters;
    }
    *pHwParams = gHardwareParameters_p;
    return status;
}

/*! *********************************************************************************
 * \brief  Store the HW parameters to Flash
 *
 * \param[in] void
 *
 * \return error code of the Flash erase/write functions
 *
 ********************************************************************************** */
uint32_t NV_WriteHWParameters(void)
{
    uint32_t status = 0U, regPrimask;

    assert(gHardwareParameters_p);
    assert(PROD_DATA_BASE_ADDR);
    status = (uint32_t)HAL_FlashInit();
    if (0U != status)
    {
        return status;
    }

    if (!FLib_MemCmp(gHardwareParameters_p, (void *)PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t)))
    {
        regPrimask = DisableGlobalIRQ();
        /*Re-calculate the Crc*/
        gHardwareParameters_p->hardwareParamsCrc = NV_ComputeCrcOverHWParameters(gHardwareParameters_p);

        FLib_MemCpy(gHardwareParameters_p->identificationWord, (const void *)mProdDataIdentifier,
                    sizeof(mProdDataIdentifier));

        /*Erase sectors before re-writting in Flash*/
#if defined(FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE)
        status = (uint32_t)HAL_FlashEraseSector((uint32_t)PROD_DATA_BASE_ADDR, FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE);
#elif defined(FSL_FEATURE_FLASH_PAGE_SIZE_BYTES)
        status = (uint32_t)HAL_FlashEraseSector((uint32_t)PROD_DATA_BASE_ADDR, FSL_FEATURE_FLASH_PAGE_SIZE_BYTES);
#elif defined(FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES)
        status =
            (uint32_t)HAL_FlashEraseSector((uint32_t)PROD_DATA_BASE_ADDR, FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES);
#endif
        if (0U == status)
        {
            /*Re-writting the hardware parameters in Flash*/
            status = (uint32_t)HAL_FlashProgramUnaligned((uint32_t)PROD_DATA_BASE_ADDR, sizeof(hardwareParameters_t),
                                                         (uint8_t *)gHardwareParameters_p);
        }
        EnableGlobalIRQ(regPrimask);
    }
    return status;
}
