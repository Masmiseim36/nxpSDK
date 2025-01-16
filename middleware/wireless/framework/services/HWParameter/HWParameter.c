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
#include "fwk_config.h"
#include "fwk_platform_definitions.h"

/*
 * If gHwParamsProdDataPlacement_c  is Ifr (gHwParamsProdDataIfrMode_c) or transitioning from MainFlash to Ifr
 * (gHwParamsProdDataMainFlash2IfrMode_c), if present (i.e. IFR_RSVD_SZ > 0), the reserved area at the start of IFR1
 * sector must be preserved.
 */
#if (defined IFR_RSVD_SZ) && (IFR_RSVD_SZ > 0)
#define gHwParamsPreserveReservedArea_c (gHwParamsProdDataPlacement_c != gHwParamsProdDataMainFlashMode_c)
#else
#define gHwParamsPreserveReservedArea_c 0
#endif

#define gHwParamRequireTemporaryCopies_c \
    (gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d || gHwParamsPreserveReservedArea_c)

#if gHwParamRequireTemporaryCopies_c
#include "fsl_component_mem_manager.h"
#endif

/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/

/* Generator for CRC calculations: CCITT CRC16 */
#define POLGEN 0x1021U

#define HW_PARAM_LEGACY_CRC_OFFSET (offsetof(hardwareParameters_t, reserved) + LEGACY_PROD_DATA_PADDING_SZ)

#if (gHwParamsProdDataPlacement_c == gHwParamsProdDataMainFlashMode_c)
#define HW_PARAM_CRC_OFFSET HW_PARAM_LEGACY_CRC_OFFSET
#else
#define HW_PARAM_CRC_OFFSET (offsetof(hardwareParameters_t, reserved) + PROD_DATA_PADDING_SZ)
#endif
/*
 * Compute CRC from first field following the PROD_DATA string till start of CRC at end of
 * reserved field.
 */
#define CRC_PROD_DATA_LEN (HW_PARAM_CRC_OFFSET - PROD_DATA_ID_STRING_SZ)

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
static uint16_t NV_ComputeCrc(uint8_t *ptr, uint32_t len);
static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t *pHwParams);
#if (gHwParamsProdDataPlacement_c == gHwParamsProdDataMainFlash2IfrMode_c)
/* Doomed to be removed after interim phase to convert legacy position and size to new one */
static uint16_t NV_ComputeCrcOverHWParametersLegacy(hardwareParameters_t *pHwParams);
static uint8_t  NV_VerifyCrcOverHWParametersLegacy(hardwareParameters_t *pHwParams);
#endif
static uint8_t NV_VerifyCrcOverHWParameters(hardwareParameters_t *pHwParams);
static uint8_t NvWriteProdData(void *src_data, uint32_t size);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/* Hardware parameters */
static uint8_t               gHardwareParameters[PROD_DATA_LEN] = {[0 ... PROD_DATA_LEN - 1] = 0xFF};
static hardwareParameters_t *gHardwareParameters_p              = NULL;

static const uint8_t mProdDataIdentifier[PROD_DATA_ID_STRING_SZ] = {"PROD_DATA:"};
#if gHwParamsAppFactoryDataExtension_d
static const uint8_t mAppFactoryDataIdentifier[APP_FACTORY_DATA_ID_STRING_SZ] = {"APP_FACT_DATA:  "};
#endif

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
static uint16_t NV_ComputeCrc(uint8_t *ptr, uint32_t len)
{
    uint16_t computedCRC = 0U;

    for (uint32_t i = 0U; i < len; i++)
    {
        uint8_t byte;
        byte = ptr[i];
        computedCRC ^= ((uint16_t)byte << 8U);
        for (uint8_t crcA = 8U; crcA != 0U; crcA--)
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
    }
    return computedCRC;
}

static uint16_t NV_ComputeCrcOverHWParameters(hardwareParameters_t *pHwParams)
{
    uint8_t *ptr = ((uint8_t *)pHwParams) + PROD_DATA_ID_STRING_SZ;
    return NV_ComputeCrc(ptr, CRC_PROD_DATA_LEN);
}

#if (gHwParamsProdDataPlacement_c == gHwParamsProdDataMainFlash2IfrMode_c)
static uint16_t NV_ComputeCrcOverHWParametersLegacy(hardwareParameters_t *pHwParams)
{
    uint8_t *ptr = ((uint8_t *)pHwParams) + PROD_DATA_ID_STRING_SZ;
    uint32_t len = HW_PARAM_LEGACY_CRC_OFFSET - PROD_DATA_ID_STRING_SZ;

    return NV_ComputeCrc(ptr, len);
}

static uint8_t NV_VerifyCrcOverHWParametersLegacy(hardwareParameters_t *pHwParams)
{
    uint16_t hw_param_crc;
    uint8_t  status = gHWParameterSuccess_c;
    uint8_t *p_crc  = (uint8_t *)pHwParams + HW_PARAM_LEGACY_CRC_OFFSET;

    hw_param_crc = ((uint16_t)p_crc[1] << 8u) + (uint16_t)p_crc[0];

    if (NV_ComputeCrcOverHWParametersLegacy(pHwParams) != hw_param_crc)
    {
        status = gHWParameterCrcError_c;
    }

    return status;
}
#endif

/*! *********************************************************************************
 * \brief  Verifies if the CRC field matches computed CRC over stored values
 *
 * \param[in] pHwParams  pointer to a structure containing HW parameters
 *
 * \return     gHWParameterSuccess_c (0)  on success, gHWParameterError_c (1)  otherwise
 *
 ********************************************************************************** */
static uint8_t NV_VerifyCrcOverHWParameters(hardwareParameters_t *pHwParams)
{
    uint8_t  status = gHWParameterSuccess_c;
    uint16_t hw_param_crc;

    uint8_t *p_crc = (uint8_t *)pHwParams + HW_PARAM_CRC_OFFSET;
    hw_param_crc   = ((uint16_t)p_crc[1] << 8u) + (uint16_t)p_crc[0];
    if (NV_ComputeCrcOverHWParameters(pHwParams) != hw_param_crc)
    {
        status = gHWParameterCrcError_c;
    }
    return status;
}

#if gHwParamsAppFactoryDataExtension_d
static uint8_t NvExtendedFactoryDataValid(extendedAppFactoryData_t *p_ext_app_factory_data, uint32_t *p_total_length)
{
    uint8_t st      = gHWParameterSuccess_c;
    *p_total_length = 0U;

    uint32_t len         = p_ext_app_factory_data->extendedDataLength;
    uint32_t data_offset = offsetof(extendedAppFactoryData_t, app_factory_data);
    uint32_t total_len   = len + data_offset + sizeof(uint16_t); /* CRC size included */
    do
    {
        uint8_t *p_crc     = NULL;
        uint16_t flash_crc = 0U;
        uint16_t computed_crc;

        if (!FLib_MemCmp(p_ext_app_factory_data->identificationWord, (const void *)mAppFactoryDataIdentifier,
                         sizeof(mProdDataIdentifier)))
        {
            /* Unexpected data */
            st = gHWParameterError_c;
            break;
        }

        if ((len == 0U) || (total_len > APP_FACTORY_DATA_MAX_LEN))
        {
            /* length must be non 0 and smaller than FACTORY_DATA_LEN maximum size */
            st = gHWParameterLenError_c;
            break;
        }
        p_crc        = &p_ext_app_factory_data->app_factory_data[len];
        flash_crc    = ((uint16_t)p_crc[1] << 8u) + (uint16_t)p_crc[0];
        computed_crc = NV_ComputeCrc(&p_ext_app_factory_data->app_factory_data[0], len);
        if (computed_crc != flash_crc)
        {
            /* Factory Data was corrupted CRCs do not match */
            st = gHWParameterCrcError_c;
            break;
        }
        st              = gHWParameterSuccess_c;
        *p_total_length = total_len;
    } while (false);
    return st;
}
#endif

static uint8_t NvWriteProdData(void *src_data, uint32_t size)
{
    uint8_t st = gHWParameterError_c;
#if gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d
    uint32_t prgm_app_factory_data_addr = APP_FACTORY_DATA_FLASH_ADDR;
    uint8_t *save_app_factory_data      = NULL;
    uint32_t total_length               = 0;
#endif
#if gHwParamsPreserveReservedArea_c
    uint8_t *save_rsvd_area = NULL;
#endif

    do
    {
        hal_flash_status_t status;

        status = HAL_FlashInit();
        if (kStatus_HAL_Flash_Success != status)
        {
            break;
        }
        if (size != (HW_PARAM_CRC_OFFSET + sizeof(uint16_t)))
        {
            break;
        }
        if (FLib_MemCmp((uint8_t *)PROD_DATA_FLASH_ADDR, src_data, PROD_DATA_LEN))
        {
            /* Return without error if there are no differences between data to program and PROD DATA in flash*/
            st = gHWParameterSuccess_c;
            break;
        }

        /* If area to be programmed is still blank no erase will be required */
        status = HAL_FlashVerifyErase(PROD_DATA_FLASH_ADDR, size, kHAL_Flash_MarginValueNormal);
        if (kStatus_HAL_Flash_Success != status)
        {
#if gHwParamsPreserveReservedArea_c
            /* If something is programmed in the reserved area, need to spare it before erase */
            status = HAL_FlashVerifyErase(IFR_USER_ADDR, IFR_RSVD_SZ, kHAL_Flash_MarginValueNormal);
            if (kStatus_HAL_Flash_Success != status)
            {
                save_rsvd_area = MEM_BufferAlloc(IFR_RSVD_SZ);
                if (save_rsvd_area == NULL)
                {
                    /* Allocation failure */
                    break;
                }
                FLib_MemCpy(save_rsvd_area, (uint8_t *)IFR_USER_ADDR, IFR_RSVD_SZ);
            }
#endif
#if gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d
            status = HAL_FlashVerifyErase(prgm_app_factory_data_addr, APP_FACTORY_DATA_MAX_LEN,
                                          kHAL_Flash_MarginValueNormal);
            if (kStatus_HAL_Flash_Success != status)
            {
                extendedAppFactoryData_t *p_ext_app_factory_data =
                    (extendedAppFactoryData_t *)prgm_app_factory_data_addr;
                if (NvExtendedFactoryDataValid(p_ext_app_factory_data, &total_length) == 0)
                {
                    /* If not blank, an erase will be required so stash away factory data before erase */
                    save_app_factory_data = MEM_BufferAlloc(total_length);
                    if (save_app_factory_data == NULL)
                    {
                        /* Allocation failure */
                        break;
                    }
                    FLib_MemCpy(save_app_factory_data, (uint8_t *)prgm_app_factory_data_addr, total_length);
                }
            }
#endif
            /* now can erase */
            status = HAL_FlashEraseSector((uint32_t)USER_DATA_SECTOR, PLATFORM_INTFLASH_SECTOR_SIZE);
            if (kStatus_HAL_Flash_Success != status)
            {
                break;
            }
        }

        status = HAL_FlashProgramUnaligned(PROD_DATA_FLASH_ADDR, size, (uint8_t *)src_data);
        if (kStatus_HAL_Flash_Success != status)
        {
            break;
        }

        st = gHWParameterSuccess_c;

    } while (false);
#if gHwParamsPreserveReservedArea_c
    if (save_rsvd_area != NULL)
    {
        if (kStatus_HAL_Flash_Success != HAL_FlashProgram(IFR_USER_ADDR, IFR_RSVD_SZ, (uint8_t *)save_rsvd_area))
        {
            st = gHWParameterError_c;
        }
        (void)MEM_BufferFree(save_rsvd_area);
    }
#endif
#if gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d
    if (save_app_factory_data != NULL)
    {
        /* If fdp is not NULL it denotes that sector got erased */
        /* There might have been an error while programming PROD_DATA, yet attempt to reprogram FDP otherwise it would
         * be lost  */
        if (kStatus_HAL_Flash_Success !=
            HAL_FlashProgram(APP_FACTORY_DATA_FLASH_ADDR, total_length, (uint8_t *)save_app_factory_data))
        {
            st = gHWParameterError_c;
        }
        (void)MEM_BufferFree(save_app_factory_data);
    }
#endif
    return st;
}
static int SearchEccFaultsInHWParam(uint32_t hw_param_sz)
{
    int status = gHWParameterSuccess_c;

    for (uint32_t i = 0U; i < hw_param_sz;)
    {
        uint8_t  read_buf[32];
        uint16_t sz = (hw_param_sz - i);
        if (sz > 32)
        {
            sz = 32;
        }
        /* Dummy read so as to detect potential ECC errors */
        if (HAL_FlashReadCheckEccFaults(PROD_DATA_FLASH_ADDR + i, sz, read_buf) == kStatus_HAL_Flash_EccError)
        {
            status = gHWParameterError_c;
            break;
        }
        i += sz;
    }
    return status;
}
/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Set a pointer to point on start of HW parameters structure.
 *         Let it NULL if structure not correct
 *
 * \param[in] pHwParams  pointer of pointer to a structure where HW parameters will
 * be stored
 *
 * \return error code
 *
 ********************************************************************************** */
uint32_t NV_ReadHWParameters(hardwareParameters_t **pHwParams)
{
    uint32_t status = gHWParameterSuccess_c;
    assert(*pHwParams == NULL);
    uint32_t hw_param_sz;
#if ((gHwParamsProdDataPlacement_c == gHwParamsProdDataIfrMode_c) || \
     (gHwParamsProdDataPlacement_c == gHwParamsProdDataMainFlash2IfrMode_c))
    hw_param_sz = PROD_DATA_LEN;
#else
    hw_param_sz = LEGACY_PROD_DATA_LEN;
#endif
    do
    {
        hal_flash_status_t st;

        if (gHardwareParameters_p != NULL)
        {
            status = gHWParameterSuccess_c;
            break;
        }

        st = HAL_FlashInit();
        if (kStatus_HAL_Flash_Success != st)
        {
            break;
        }
        st = HAL_FlashVerifyErase(PROD_DATA_FLASH_ADDR, hw_param_sz, kHAL_Flash_MarginValueNormal);
        if (kStatus_HAL_Flash_Success == st)
        {
#if (gHwParamsProdDataPlacement_c == gHwParamsProdDataMainFlash2IfrMode_c)
            /* Check if there are prod data at legacy position. The IFR sector was not programmed
             * with HWParams yet. Attempt read from legacy position and transfer to IFR.
             * In the interim period while migration phase is not complete need to keep the sector at
             * top of flash.
             */
            uint32_t              mainflash_prod_data_location = (uint32_t)MAIN_FLASH_PROD_DATA_ADDR;
            hardwareParameters_t *pParamsToCopy                = (hardwareParameters_t *)mainflash_prod_data_location;
            if ((FLib_MemCmp(pParamsToCopy->identificationWord, (const void *)mProdDataIdentifier,
                             sizeof(mProdDataIdentifier)) == TRUE) &&
                (NV_VerifyCrcOverHWParametersLegacy(pParamsToCopy) == gHWParameterSuccess_c))
            {
                uint8_t *p_crc;
                uint16_t crc;
                /* There is a valid version of PROD DATA at the Main Flash location: allocate a RAM buffer to copy it to
                 * IFR */
                FLib_MemCpy(&gHardwareParameters[0], (uint8_t *)pParamsToCopy, sizeof(hardwareParameters_t) - 1u);
                FLib_MemSet(&gHardwareParameters[offsetof(hardwareParameters_t, reserved)], 0xffU,
                            PROD_DATA_PADDING_SZ);
                crc      = NV_ComputeCrcOverHWParameters((hardwareParameters_t *)(void *)&gHardwareParameters[0]);
                p_crc    = &gHardwareParameters[HW_PARAM_CRC_OFFSET];
                p_crc[0] = (uint8_t)(crc & 0x00ffU);
                p_crc[1] = (uint8_t)((crc >> 8U) & 0x00ffU);
                /* The size to be written ios 2 octets after the CRC offset : this works for both legacy and new
                 * structure */
                status = NvWriteProdData(&gHardwareParameters[0], HW_PARAM_CRC_OFFSET + sizeof(uint16_t));

                if (status != gHWParameterSuccess_c)
                {
                    gHardwareParameters_p = NULL;
                    break;
                }
                gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
                status                = gHWParameterSuccess_c;
                break;
            }
            else
#endif
            {
                status = gHWParameterBlank_c;
                FLib_MemSet(&gHardwareParameters[0], 0xffu, hw_param_sz);
            }
            gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
        }
        else
        {
            /* First do an ECC error check over the HWParam location */
            status = SearchEccFaultsInHWParam(hw_param_sz);

            /* If ECC error detected, need to erase  */
            if (status == gHWParameterError_c)
            {
                if (HAL_FlashEraseSector(MAIN_FLASH_PROD_DATA_ADDR, PLATFORM_INTFLASH_SECTOR_SIZE) !=
                    kStatus_HAL_Flash_Success)
                {
                    status = gHWParameterError_c;
                }
                else
                {
                    status = gHWParameterBlank_c;
                }
                gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
                FLib_MemSet(&gHardwareParameters[0], 0xffu, hw_param_sz);
            }

            /* if we reach this point the HwPAram area is exempt of ECC errors */
            /*Load the hardware parameters in Flash to RAM */
            hardwareParameters_t *pLocalParams = (hardwareParameters_t *)PROD_DATA_FLASH_ADDR;
            if (FLib_MemCmp(pLocalParams->identificationWord, (const void *)mProdDataIdentifier,
                            sizeof(mProdDataIdentifier)) &&
                (NV_VerifyCrcOverHWParameters(pLocalParams) == gHWParameterSuccess_c))
            {
                /*Copy the local value in our static structure*/
                FLib_MemCpy(&gHardwareParameters[0], (void *)pLocalParams, hw_param_sz);
                gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
                status                = gHWParameterSuccess_c;
            }
            else
            {
                gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
                status                = gHWParameterError_c;
                break;
            }
        }
    } while (false);

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
    uint32_t status            = gHWParameterSuccess_c;
    uint32_t prod_data_address = PROD_DATA_FLASH_ADDR;
    uint16_t crc;
    uint8_t *p_crc;
    assert(gHardwareParameters_p);
    do
    {
        uint32_t regPrimask;

        if (kStatus_HAL_Flash_Success != HAL_FlashInit())
        {
            status = gHWParameterError_c;
            break;
        }

        if (FLib_MemCmp(gHardwareParameters_p, (uint8_t *)prod_data_address, HW_PARAM_CRC_OFFSET))
        {
            /* The CRC has not been recalculated yet, so compare the RAM
             * values to check whether it is worth reprogramming , if equal do nothing  */
            break;
        }

        regPrimask = DisableGlobalIRQ();
        /*Re-calculate the Crc*/
        crc      = NV_ComputeCrcOverHWParameters((hardwareParameters_t *)(void *)&gHardwareParameters[0]);
        p_crc    = &gHardwareParameters[HW_PARAM_CRC_OFFSET];
        p_crc[0] = (uint8_t)(crc & 0x00ffU);
        p_crc[1] = (uint8_t)((crc >> 8U) & 0x00ffU);

        FLib_MemCpy(gHardwareParameters_p->identificationWord, (const void *)mProdDataIdentifier,
                    sizeof(mProdDataIdentifier));

        /*Re-writing the hardware parameters in Flash*/
        status = NvWriteProdData(gHardwareParameters_p, HW_PARAM_CRC_OFFSET + sizeof(uint16_t));

        EnableGlobalIRQ(regPrimask);

    } while (false);
    return status;
}

uint8_t Nv_WriteAppFactoryData(extendedAppFactoryData_t *src_data, uint32_t extended_data_len)
{
    uint8_t st = gHWParameterError_c;

#if gHwParamsAppFactoryDataExtension_d
    const uint32_t prgm_prod_data_addr        = (uint32_t)PROD_DATA_FLASH_ADDR;
    const uint32_t prgm_app_factory_data_addr = (uint32_t)(APP_FACTORY_DATA_FLASH_ADDR);
    uint8_t       *p_prod_data                = NULL;
    uint32_t       input_total_len =
        extended_data_len + offsetof(extendedAppFactoryData_t, app_factory_data) + sizeof(uint16_t);
    uint32_t regPrimask = 0U;
    bool     is_masked  = false;

    do
    {
        hal_flash_status_t status;
        uint32_t           total_length;
        uint16_t           crc;
        status = HAL_FlashInit();
        if (kStatus_HAL_Flash_Success != status)
        {
            break;
        }
        status = HAL_FlashVerifyErase(prgm_app_factory_data_addr, input_total_len, kHAL_Flash_MarginValueNormal);
        if (kStatus_HAL_Flash_Success != status)
        {
            extendedAppFactoryData_t *p_ext_app_factory_data = (extendedAppFactoryData_t *)prgm_app_factory_data_addr;
            if ((NvExtendedFactoryDataValid(p_ext_app_factory_data, &total_length) == 0) &&
                (p_ext_app_factory_data->extendedDataLength == extended_data_len) &&
                (FLib_MemCmp(&p_ext_app_factory_data->app_factory_data[0], &src_data->app_factory_data[0],
                             extended_data_len)))
            {
                /* The CRC has not been recalculated yet, so compare the RAM up to the CRC
                 * values to check whether it is worth reprogramming , if equal do nothing  */
                st = gHWParameterSuccess_c;
                break;
            }
            /* If we reached here a sector erase is required */
            /* If the PROD DATA field was programmed priorly, need to stash it away so that we can reprogram it */
            status = HAL_FlashVerifyErase(prgm_prod_data_addr, PROD_DATA_LEN, kHAL_Flash_MarginValueNormal);
            if (kStatus_HAL_Flash_Success != status)
            {
                /* If not blank, an erase will be required so stash away factory data before erase */
                /*Preserve in RAM the hardware parameters in Flash to conserve them while erasing  */
                hardwareParameters_t *pLocalParams = (hardwareParameters_t *)prgm_prod_data_addr;
                if (FLib_MemCmp(pLocalParams->identificationWord, (const void *)mProdDataIdentifier,
                                sizeof(mProdDataIdentifier)) &&
                    (NV_VerifyCrcOverHWParameters(pLocalParams) == gHWParameterSuccess_c))
                {
                    /*Copy the local value in our static structure*/
                    FLib_MemCpy(&gHardwareParameters, (void *)pLocalParams, sizeof(hardwareParameters_t));
                    gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];
                    p_prod_data           = gHardwareParameters;
                }
            }
            /* now can erase */
            status = HAL_FlashEraseSector(prgm_prod_data_addr, PLATFORM_INTFLASH_SECTOR_SIZE);
            if (kStatus_HAL_Flash_Success != status)
            {
                st = gHWParameterError_c;
                break;
            }
        }

        regPrimask = DisableGlobalIRQ();
        is_masked  = true;
        /*Re-calculate the Crc*/
        crc            = NV_ComputeCrc((uint8_t *)&src_data->app_factory_data, extended_data_len);
        uint8_t *p_crc = &src_data->app_factory_data[extended_data_len];
        p_crc[0]       = (uint8_t)(crc & 0x00ffU);
        p_crc[1]       = (uint8_t)((crc >> 8U) & 0x00ffU);

        FLib_MemCpy(src_data->identificationWord, (const void *)mAppFactoryDataIdentifier,
                    sizeof(mAppFactoryDataIdentifier));
        src_data->extendedDataLength = extended_data_len;

        status = HAL_FlashProgram(prgm_app_factory_data_addr, input_total_len, (uint8_t *)src_data);
        if (kStatus_HAL_Flash_Success != status)
        {
            break;
        }

        st = gHWParameterSuccess_c;
    } while (false);

    if (p_prod_data != NULL)
    {
        /* If p_prod_data is not NULL it denotes that sector got erased */
        /* There might have been an error while programming FACTORY_DATA, yet attempt to reprogram PROD_DATA otherwise
         * it would be lost  */
        if (kStatus_HAL_Flash_Success != HAL_FlashProgram(prgm_prod_data_addr, PROD_DATA_LEN, (uint8_t *)p_prod_data))
        {
            st = gHWParameterError_c;
        }
    }
    if (is_masked)
    {
        EnableGlobalIRQ(regPrimask);
    }
#endif
    return st;
}

extendedAppFactoryData_t *Nv_GetAppFactoryData(void)
{
#if gHwParamsAppFactoryDataExtension_d

    extendedAppFactoryData_t *p_extended_data = (extendedAppFactoryData_t *)APP_FACTORY_DATA_FLASH_ADDR;
    uint32_t                  total_len;
    if (NvExtendedFactoryDataValid(p_extended_data, &total_len) != gHWParameterSuccess_c)
    {
        p_extended_data = NULL;
    }
    return p_extended_data;
#else
    return NULL;
#endif
}

void HWParametersReset(void)
{
    gHardwareParameters_p = NULL;
    FLib_MemSet(gHardwareParameters, 0xffU, sizeof(gHardwareParameters));
}

void HWParametersErase(void)
{
    FLib_MemSet(gHardwareParameters, 0xffU, sizeof(gHardwareParameters));
    gHardwareParameters_p = (hardwareParameters_t *)(void *)&gHardwareParameters[0];

    /*Re-writing the hardware parameters in Flash*/
    (void)NvWriteProdData(gHardwareParameters_p, HW_PARAM_CRC_OFFSET + sizeof(uint16_t));
}
