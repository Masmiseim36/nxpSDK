/*
 * Copyright (c) 2013-2022 ARM Limited. All rights reserved.
 * Copyright 2019-2022 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "target_cfg.h"
#include "Driver_Flash.h"
#include "platform_base_address.h"
#include "flash_layout.h"
#include "fsl_iap.h"
#include "fsl_cache.h"
#include "tfm_spm_log.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/**
 * \brief Arm Flash device structure.
 */
struct arm_flash_dev_t {
    ARM_FLASH_INFO       *data;         /*!< FLASH data */
    flexspi_nor_config_t flash_config;  /*!< FLASH config*/
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/**
 * \brief Flash driver capability macro definitions \ref ARM_FLASH_CAPABILITIES
 */
/* Flash Ready event generation capability values */
#define EVENT_READY_NOT_AVAILABLE   (0u)
#define EVENT_READY_AVAILABLE       (1u)

/* Data access size values */
enum {
    DATA_WIDTH_8BIT   = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

/* Chip erase capability values */
#define CHIP_ERASE_NOT_SUPPORTED    (0u)
#define CHIP_ERASE_SUPPORTED        (1u)

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    EVENT_READY_NOT_AVAILABLE,
    DATA_WIDTH_32BIT,
    CHIP_ERASE_NOT_SUPPORTED
};

static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
    .sector_size  = FLASH0_SECTOR_SIZE,
    .page_size    = FLASH0_PAGE_SIZE,
    .program_unit = FLASH0_PROGRAM_UNIT,
    .erased_value = 0xFF};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
    .data        = &(ARM_FLASH0_DEV_DATA)};

static struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/* Prototypes */
static bool is_range_valid(struct arm_flash_dev_t *flash_dev,
                           uint32_t offset);
static bool is_write_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param);

/* Functions */

/**
 * \brief      Check if the Flash memory boundaries are not violated.
 * \param[in]  flash_dev  Flash device structure \ref arm_flash_dev_t
 * \param[in]  offset     Highest Flash memory address which would be accessed.
 * \return     Returns true if Flash memory boundaries are not violated, false
 *             otherwise.
 */
static bool is_range_valid(struct arm_flash_dev_t *flash_dev,
                           uint32_t offset)
{
    uint32_t flash_limit = 0;

    /* Calculating the highest address of the Flash memory address range */
    flash_limit = FLASH0_SIZE - 1;

    return (offset > flash_limit) ? (false) : (true) ;
}

/**
 * \brief        Check if the parameter is aligned to program_unit.
 * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
 * \param[in]    param      Any number that can be checked against the
 *                          program_unit, e.g. Flash memory address or
 *                          data length in bytes.
 * \return       Returns true if param is aligned to program_unit, false
 *               otherwise.
 */
static bool is_write_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
    return ((param % flash_dev->data->program_unit) != 0) ? (false) : (true);
}

/**
  * \brief        Check if the parameter is aligned to page_unit.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          sector_size, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to sector_size, false
  *               otherwise.
  */
static bool is_erase_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
  return ((param % (flash_dev->data->sector_size)) != 0) ? (false) : (true);
}

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

#if defined(FLASH_NEED_RESET) && FLASH_NEED_RESET
AT_QUICKACCESS_SECTION_CODE(static status_t BOARD_FlexspiInit(uint32_t instance,
                                                       flexspi_nor_config_t *config,
                                                       serial_nor_config_option_t *option))
{
    /* Reset external flash */
    GPIO->CLR[2] = 1 << 12;
    for (uint32_t i = 0; i < 6000; i++)
        __NOP();
    GPIO->SET[2] = 1 << 12;

    /* Clear FLEXSPI NOR flash configure context */
    SYSCTL0->FLEXSPI_BOOTROM_SCRATCH0 = 0;

    status_t status = IAP_FlexspiNorAutoConfig(instance, config, option);

    if ((CACHE64->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0)
    {
        /* Invalidate all lines in both ways and initiate the cache command. */
        CACHE64->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

        /* Wait until the cache command completes. */
        while ((CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
        {
        }

        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        CACHE64->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);
    }

    return status;
}
#endif

static bool flash_init_is_done = false;
static int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    status_t                    status;
    serial_nor_config_option_t  option;

    if (flash_init_is_done == false)
    {
        option.option0.U = NOR_FLASH_OPTION0;
        option.option1.U = NOR_FLASH_OPTION1;

    #if defined(FLASH_NEED_RESET) && FLASH_NEED_RESET
            status = BOARD_FlexspiInit(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &option);
    #else
            status = IAP_FlexspiNorAutoConfig(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &option);
    #endif
       

        if (status != kStatus_Success)
        {
        #if TARGET_DEBUG_LOG
            SPMLOG_DBGMSG("\r\n***NOR Flash Initialization Failed!***\r\n");
        #endif
            return ARM_DRIVER_ERROR;
        }

        CACHE64_InvalidateCache(CACHE_BASE); /* Without this MIMXRT685-AUD-EVK is not stable */

    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Initialization Success!***\r\n");
    #endif

        flash_init_is_done = true;
    }

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    flash_init_is_done = false;
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state) {
    case ARM_POWER_FULL:
        /* Nothing to be done */
        return ARM_DRIVER_OK;

    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

static int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    status_t status;
    bool is_valid = true;
    int32_t result = cnt;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check Flash memory boundaries */
    is_valid = is_range_valid(FLASH0_DEV, addr + cnt);
    if(is_valid != true) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Read error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Read Data */
    if(cnt)
    {
        
    #if TARGET_DEBUG_LOG
        //TBD LOG_MSG("[OK] 0x%X => 0x%X (%d)\r\n", addr, data, cnt );
    #endif

#if 0  /* Failed on RT595 as "addr" must be aligned to 4 bytes, RT685 was OK*/
        status = IAP_FlexspiNorRead(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, data, addr, cnt);
        if (status != kStatus_Success) {
        #if TARGET_DEBUG_LOG
          //TBD  LOG_MSG("\r\n***NOR Flash Read Failed [0x%X]  0x%X => 0x%X (%d)!***\r\n", status, addr, data, cnt);
        #endif
            return ARM_DRIVER_ERROR;
        }
#else
        uint32_t cnt_diff;
        uint32_t addr_diff = addr%4; /* Flash read address must be aligned to 4 bytes, othewuse ROM IAP will read from a wrong address. */
        uint32_t tmp;                /* Write address must be aligned to 4 bytes, otherwise ROM IAP returns error. */
        
        if(addr_diff != 0)
        {
            #if TARGET_DEBUG_LOG
             //TBD   LOG_MSG("[] 0x%X => 0x%X (%d)\r\n", addr, data, cnt );
            #endif
            
            if(cnt > (4-addr_diff)) 
            {
                cnt_diff = (4-addr_diff);
            }
            else
            {
                cnt_diff = cnt;
            }
            
            status = IAP_FlexspiNorRead(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &tmp, (addr-addr_diff), cnt_diff+addr_diff);
            if (status != kStatus_Success) {
            #if TARGET_DEBUG_LOG
             //TBD   LOG_MSG("\r\n***NOR Flash Read Failed [0x%X]  0x%X => 0x%X (%d, %d)!***\r\n", status, addr, data, cnt, addr_diff);
            #endif
                return ARM_DRIVER_ERROR;
            }
            
            (void)memcpy(data, (((uint8_t *)&tmp) + addr_diff), cnt_diff);
            cnt -= cnt_diff;
            addr += cnt_diff; 
            data =  (uint8_t*)data + cnt_diff;
            
            /* Addr is aligned to 4 bytes now */
        }

        while(cnt)
        {
            if(cnt > 4) 
            {
                cnt_diff = 4;
            }
            else
            {
                cnt_diff = cnt;
            }
            
            status = IAP_FlexspiNorRead(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &tmp, addr, cnt_diff);
            if (status != kStatus_Success) {
            #if TARGET_DEBUG_LOG
              //TBD  LOG_MSG("\r\n***NOR Flash Read Failed [0x%X]  0x%X => 0x%X (%d, %d)!***\r\n", status, addr, data, cnt, cnt_diff);
            #endif
                return ARM_DRIVER_ERROR;
            }
            
            (void)memcpy(data, (uint8_t *)&tmp, cnt_diff);
            cnt -= cnt_diff;
            addr += cnt_diff; 
            data = (uint8_t*)data + cnt_diff;
        }
#endif        
    }

    return result;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    static uint32_t status;
    uint32_t        pages;
    uint32_t        i;
    const uint32_t  *src;
    static uint32_t write_buffer[FLASH0_PAGE_SIZE/sizeof(uint32_t)]; /* Aligned temporary buffer. IAP requires that the source address must be aligned to 4 bytes.*/
    bool            data_is_aligned;
    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check Flash memory boundaries and alignment with minimum write size
    * (program_unit), data size also needs to be a multiple of program_unit. */
    if(!(is_range_valid(FLASH0_DEV, addr + cnt) &&
         is_write_aligned(FLASH0_DEV, addr)     &&
         is_write_aligned(FLASH0_DEV, cnt)      )) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Program error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    pages = cnt / FLASH0_DEV->data->page_size;
    data_is_aligned = ((uint32_t)data % (sizeof(uint32_t)) != 0) ? (false) : (true);
    
    for (i = 0; i < pages; i++, data = (uint8_t*)data + FLASH0_PAGE_SIZE, addr += FLASH0_PAGE_SIZE)
    {
        if(data_is_aligned)
        {
            src = data;
        }
        else
        {
            src = memcpy(write_buffer, data, FLASH0_PAGE_SIZE);
        }

        status = IAP_FlexspiNorPageProgram(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, addr, src);

        if (status != kStatus_Success)
        {
        #if TARGET_DEBUG_LOG
          //TBD  LOG_MSG("\r\n***NOR Flash Page %d Program Failed!*** Status = %d Dest = 0x%x Src = 0x%x \r\n", i, status, addr, data);
        #endif
            return ARM_DRIVER_ERROR;
        }
    }

    CACHE64_InvalidateCache(CACHE_BASE);

#if 0 /* Check result */
    {
        int i;

        for (i = 0; i < (cnt/4); i++)
        {
            uint32_t test_data;
            status = IAP_FlexspiNorRead(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &test_data, addr + i*4, 4);
            if (status != kStatus_Success)
            {
            #if TARGET_DEBUG_LOG
                SPMLOG_DBGMSG("\r\n***NOR Flash Read Failed (after progtram)!***\r\n");
            #endif
                break;
            }
            
            if ( test_data != *((uint32_t *)((uint8_t*)data + i*4)) )
            {
            #if TARGET_DEBUG_LOG
                SPMLOG_DBGMSG("\r\n***NOR Flash program Check Failed!***\r\n");
            #endif
                break;
            }
        }
    }
#endif
    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    status_t status;

    if(!(is_range_valid(FLASH0_DEV, addr) &&
         is_erase_aligned(FLASH0_DEV, addr) )) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Erase error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }
         
    /* Erase sector */
    status = IAP_FlexspiNorEraseSector(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, addr);
    if (status != kStatus_Success)
    {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Erase Failed!***\r\n");
    #endif
        return ARM_DRIVER_ERROR;
    }

    CACHE64_InvalidateCache(CACHE_BASE);

#if 1 /* Check result */ /* Also, without this check an application is not stable on RT685 */
    {
        int i;

        for (i = 0; i < (FLASH0_DEV->data->sector_size/4); i++)
        {
            uint32_t data;
            status = IAP_FlexspiNorRead(NOR_FLASH_INSTANCE, &FLASH0_DEV->flash_config, &data, addr + i*4, 4);
            if (status != kStatus_Success)
            {
            #if TARGET_DEBUG_LOG
                SPMLOG_DBGMSG("\r\n***NOR Flash Read Failed (after erase)!***\r\n");
            #endif
                return ARM_DRIVER_ERROR;
            }
            
            if (data != 0xFFFFFFFF)
            {
            #if TARGET_DEBUG_LOG
                SPMLOG_DBGMSG("\r\n***NOR Flash Erase Check Failed!***\r\n");
            #endif
                return ARM_DRIVER_ERROR;
            }
        }
    }
#endif

    return ARM_DRIVER_OK;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    .GetVersion = ARM_Flash_GetVersion,
    .GetCapabilities = ARM_Flash_GetCapabilities,
    .Initialize = ARM_Flash_Initialize,
    .Uninitialize = ARM_Flash_Uninitialize,
    .PowerControl = ARM_Flash_PowerControl,
    .ReadData = ARM_Flash_ReadData,
    .ProgramData = ARM_Flash_ProgramData,
    .EraseSector = ARM_Flash_EraseSector,
    .GetStatus = ARM_Flash_GetStatus,
    .GetInfo = ARM_Flash_GetInfo
};
