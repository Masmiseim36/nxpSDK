/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fwk_config.h"
#include "fwk_platform_extflash.h"
#include "fsl_os_abstraction.h"
#include "mflash_drv.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/* _SECTOR_ADDR returns the address of sector containing address */
#define _SECTOR_ADDR(addr) (((addr) & ~(PLATFORM_EXTFLASH_SECTOR_SIZE - 1U)))
/* _PAGE_ADDR returns the address of page containing address */
#define _PAGE_ADDR(addr) (((addr) & ~(PLATFORM_EXTFLASH_PAGE_SIZE - 1U)))

#if !defined PLATFORM_ACCESS_ALIGNMENT_CONSTRAINT_LOG
#define PLATFORM_ACCESS_ALIGNMENT_CONSTRAINT_LOG 0u
#endif

#define PLATFORM_ACCESS_UNIT_SZ (1u<<PLATFORM_ACCESS_ALIGNMENT_CONSTRAINT_LOG)
#define PLATFORM_ACCESS_UNIT_MSK (PLATFORM_ACCESS_UNIT_SZ - 1u)

#define _SZ_MULTIPLE_OF_ACCESS_UNIT(sz) (((sz) & PLATFORM_ACCESS_UNIT_MSK) == 0u)
#define _ROUND_UP_MULTIPLE_OF_ACCESS_UNIT(length) ((length+PLATFORM_ACCESS_UNIT_MSK) & ~PLATFORM_ACCESS_UNIT_MSK)

/*******************************************************************************
 * Public functions
 ******************************************************************************/

int PLATFORM_InitExternalFlash(void)
{
    status_t    st        = kStatus_Success;
    static bool init_done = false;
    if (!init_done)
    {
        st = mflash_drv_init();
        if (st == kStatus_Success)
        {
            init_done = true;
        }
    }
    return (int)st;
}

int PLATFORM_EraseExternalFlash(uint32_t address, uint32_t size)
{
    status_t st      = kStatus_Success;
    uint32_t endAddr = address + size;
    uint32_t startBlock, endBlock;
    uint32_t index;
    if ((endAddr & (PLATFORM_EXTFLASH_SECTOR_SIZE - 1U)) != 0U)
    {
        /* If the address is in the middle of a block, round up to the next block
         * This gives the upper block limit, every blocks before this one will be erased */
        endAddr = ((endAddr / PLATFORM_EXTFLASH_SECTOR_SIZE) + 1U) * PLATFORM_EXTFLASH_SECTOR_SIZE;
    }

    startBlock = address / PLATFORM_EXTFLASH_SECTOR_SIZE;
    endBlock   = endAddr / PLATFORM_EXTFLASH_SECTOR_SIZE;
    index      = startBlock;
    for (index = startBlock; (index < endBlock); index++)
    {
        uint32_t addr;

        /* The external flash driver is MFLASH, which has no Block Erase API, so limit to sector erase use only.
         * This is less optimal and operations take arounf 15% longer when erasing sectors one by one */
        addr = index * PLATFORM_EXTFLASH_SECTOR_SIZE;
        /* No address conversion to physical address */
        st = mflash_drv_sector_erase(addr);
        if (st != kStatus_Success)
        {
            break;
        }
    }
    return (int)st;
}

int PLATFORM_ReadExternalFlash(uint8_t *dest, uint32_t length, uint32_t address, bool requestFastRead)
{
    (void)(requestFastRead);
    status_t st;
    uint32_t read_bytes = length;
    if (!_SZ_MULTIPLE_OF_ACCESS_UNIT(length))
    {
        read_bytes = _ROUND_UP_MULTIPLE_OF_ACCESS_UNIT(length);
    }
    /* mflash_drv_read already does the logical to physical address conversion */
    st = mflash_drv_read(address, (uint32_t *)dest, read_bytes);

    return (int)st;
}

int PLATFORM_WriteExternalFlash(uint8_t *data, uint32_t length, uint32_t address)
{
    status_t st = kStatus_Success;
    //    uint32_t phys_addr;
    //    phys_addr = mflash_drv_log2phys((void *)address, length);
    uint32_t nb_pages = length / PLATFORM_EXTFLASH_PAGE_SIZE;
    for (uint32_t i = 0U; i < nb_pages; i++)
    {
        st = mflash_drv_page_program(address, (uint32_t *)data);
        if (st != kStatus_Success)
        {
            assert(st == kStatus_Success);
            break;
        }

        if (length >= PLATFORM_EXTFLASH_PAGE_SIZE)
        {
            data += PLATFORM_EXTFLASH_PAGE_SIZE;
            address += PLATFORM_EXTFLASH_PAGE_SIZE;
        }
    }

    return (int)st;
}

int PLATFORM_IsExternalFlashBusy(bool *isBusy)
{
    uint32_t  dummy;
    status_t st;
    st      = mflash_drv_read(0U, (void *)&dummy, sizeof(uint32_t));
    *isBusy = (kStatus_Busy == st) ? 1U : 0U;
    return (int)st;
}

bool PLATFORM_IsExternalFlashPageBlank(uint32_t address)
{
    bool ret;

    uint32_t addr = _PAGE_ADDR(address);
    /* Start from address of page containing argument address */

    uint32_t read_buf[PLATFORM_EXTFLASH_PAGE_SIZE / sizeof(uint32_t)];

    if (kStatus_Success != mflash_drv_read(addr, (void *)read_buf, PLATFORM_EXTFLASH_PAGE_SIZE))
    {
        ret = false;
    }
    else
    {
        ret = true;
        for (uint8_t i = 0U; i < (PLATFORM_EXTFLASH_PAGE_SIZE / sizeof(uint32_t)); i++)
        {
            /* After erase, NOR flash presumably has all bits to 1 */
            /* All bytes being equal to 0xff denote a blank state and the page may be written */
            if (read_buf[i] != 0xffffffffU)
            {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

bool PLATFORM_IsExternalFlashSectorBlank(uint32_t address)
{
    bool ret = true;
    /* Start from address of sector containing argument address */
    uint32_t addr = _SECTOR_ADDR(address);
    /* Loop through all pages of a sector and check their state */
    for (uint8_t i = 0U; i < (PLATFORM_EXTFLASH_SECTOR_SIZE / PLATFORM_EXTFLASH_PAGE_SIZE); i++)
    {
        ret = PLATFORM_IsExternalFlashPageBlank(addr);
        if (!ret)
        {
            /* The first non-blank page in a sector denotes a non blank state */
            break;
        }
        addr += PLATFORM_EXTFLASH_PAGE_SIZE;
    }
    /* next page within sector  */
    return ret;
}