/*
 * Copyright (c) 2016, SDK_Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nand.h"
#include "fsl_nand_flash.h"
#include "dhara_nand_config.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#if defined EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
#include "hamming.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DHARA_NAND_BAD_BLOCK_MARK      (0U)
#define DHARA_NAND_SDK_Free_MARK       (0xFFU)
#define DHARA_HANDLE_COVERT_TO_NAND(n) ((nand_handle_t *)((uint32_t)n + sizeof(struct dhara_nand)))
#if defined EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
#define DHARA_HAMMING_CHUNK_SIZE         (512U)
#define DHARA_HAMMING_ECC_SIZE_PER_CHUNK (3U)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t bno, dhara_error_t *err)
{
    if (Nand_Flash_Erase_Block(DHARA_HANDLE_COVERT_TO_NAND(n), bno) != kStatus_Success)
    {
        return -1;
    }

    return 0;
}

int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
    int status = 0;

#if defined EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
    uint8_t *dataBuffer = NULL, *tempBuffer = NULL;
    uint32_t i         = 0U;
    uint32_t pageSize  = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea;
    uint32_t spareSize = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea;

    assert(pageSize % DHARA_HAMMING_CHUNK_SIZE == 0U);

    dataBuffer = SDK_Malloc(pageSize + spareSize, 1U);
    assert(dataBuffer != NULL);
    memset(dataBuffer, 0xFFU, pageSize + spareSize);

    for (i = 0; i < pageSize / DHARA_HAMMING_CHUNK_SIZE; i++)
    {
        tempBuffer = dataBuffer + i * (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK);
        memcpy(tempBuffer, data + i * DHARA_HAMMING_CHUNK_SIZE, DHARA_HAMMING_CHUNK_SIZE);
        hamming_generate(tempBuffer, DHARA_HAMMING_CHUNK_SIZE, tempBuffer + DHARA_HAMMING_CHUNK_SIZE);
    }

    if (Nand_Flash_Page_Program(DHARA_HANDLE_COVERT_TO_NAND(n), p, dataBuffer, pageSize + spareSize) != kStatus_Success)
    {
        status = -1;
        /* If page program failed, mark this specific block as bad block. */
        dhara_set_error(err, DHARA_E_BAD_BLOCK);
    }

    SDK_Free(dataBuffer);
#else
    if (Nand_Flash_Page_Program(DHARA_HANDLE_COVERT_TO_NAND(n), p, data,
                                DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea) != kStatus_Success)
    {
        status = -1;
        /* If page program failed, mark this specific block as bad block. */
        dhara_set_error(err, DHARA_E_BAD_BLOCK);
    }
#endif

    return status;
}

int dhara_nand_read(
    const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err)
{
    int status = 0;

#if !defined EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
    if (Nand_Flash_Read_Page_Partial(DHARA_HANDLE_COVERT_TO_NAND(n), p, offset, data, length) != kStatus_Success)
    {
        status = -1;
    }
#else
    uint8_t *dataBuffer = NULL, *tempBuffer = NULL;
    uint32_t i = 0U;
    hamming_ecc_t e;
    uint32_t copyLength  = length;
    uint32_t eccTrunkNum = length > DHARA_HAMMING_CHUNK_SIZE ?
                               ((length / DHARA_HAMMING_CHUNK_SIZE) + (length % DHARA_HAMMING_CHUNK_SIZE != 0U)) :
                               1U;
    bool dataSeparate   = false;
    uint32_t tempOffset = 0U;
    uint8_t *tempData   = data;

    offset += (offset / DHARA_HAMMING_CHUNK_SIZE) * DHARA_HAMMING_ECC_SIZE_PER_CHUNK;

    tempOffset = (offset / (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK)) *
                 (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK);

    if (((offset + length) > (tempOffset + DHARA_HAMMING_CHUNK_SIZE)) &&
        (length != DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea))
    {
        eccTrunkNum += 1U;
        dataSeparate = true;
    }

    dataBuffer = SDK_Malloc((DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK) * eccTrunkNum, 1U);
    assert(dataBuffer != NULL);
    memset(dataBuffer, 0U, (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK) * eccTrunkNum);

    if (Nand_Flash_Read_Page_Partial(DHARA_HANDLE_COVERT_TO_NAND(n), p, tempOffset, dataBuffer,
                                     (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK) * eccTrunkNum) !=
        kStatus_Success)
    {
        status = -1;
    }
    else
    {
        for (i = 0; i < eccTrunkNum; i++)
        {
            tempBuffer = dataBuffer + i * (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK);
            e          = hamming_syndrome(tempBuffer, DHARA_HAMMING_CHUNK_SIZE, tempBuffer + DHARA_HAMMING_CHUNK_SIZE);
            if (e)
            {
                hamming_repair(tempBuffer, DHARA_HAMMING_CHUNK_SIZE, e);
            }
            if (dataSeparate)
            {
                if (i == 0U)
                {
                    copyLength = tempOffset + DHARA_HAMMING_CHUNK_SIZE - offset;
                    tempOffset = offset - tempOffset;
                }
                else
                {
                    copyLength = length - copyLength;
                    tempOffset = 0U;
                }
            }
            else
            {
                tempOffset = offset - tempOffset;
            }

            memcpy(tempData, tempBuffer + tempOffset,
                   copyLength > DHARA_HAMMING_CHUNK_SIZE ? DHARA_HAMMING_CHUNK_SIZE : copyLength);
            tempData = (uint8_t *)((uint32_t)tempData +
                                   ((copyLength > DHARA_HAMMING_CHUNK_SIZE) ? DHARA_HAMMING_CHUNK_SIZE : copyLength));
            if (copyLength > DHARA_HAMMING_CHUNK_SIZE)
            {
                copyLength -= DHARA_HAMMING_CHUNK_SIZE;
            }
        }
    }

    SDK_Free(dataBuffer);
#endif

    return status;
}

int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t bno)
{
    uint8_t *spareBuffer = NULL;
    int isBad            = 0U;

    spareBuffer = SDK_Malloc(DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea, 1U);
    assert(spareBuffer != NULL);
    memset(spareBuffer, 0U, DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea);

    if (Nand_Flash_Read_Page_Partial(DHARA_HANDLE_COVERT_TO_NAND(n), bno * DHARA_HANDLE_COVERT_TO_NAND(n)->pagesInBlock,
                                     DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea, spareBuffer,
                                     DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea) != kStatus_Success)
    {
        isBad = -1;
    }
    else
    {
        /* check the first page last byte of spare area */
        if (spareBuffer[DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea - 1U] == DHARA_NAND_BAD_BLOCK_MARK)
        {
            isBad = 1;
        }
    }

    SDK_Free(spareBuffer);

    return isBad;
}

void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t bno)
{
    uint8_t *dataBuffer = NULL;
    uint32_t pageSize   = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea;
    uint32_t spareSize  = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea;

    dataBuffer = SDK_Malloc(pageSize + spareSize, 1U);
    assert(dataBuffer != NULL);

    if (Nand_Flash_Read_Page(DHARA_HANDLE_COVERT_TO_NAND(n), bno * DHARA_HANDLE_COVERT_TO_NAND(n)->pagesInBlock,
                             dataBuffer, pageSize + spareSize) == kStatus_Success)
    {
        /* use the first page last byte of spare area as bad block mark for the target block */
        dataBuffer[pageSize + spareSize - 1U] = DHARA_NAND_BAD_BLOCK_MARK;
        Nand_Flash_Page_Program(DHARA_HANDLE_COVERT_TO_NAND(n), bno * DHARA_HANDLE_COVERT_TO_NAND(n)->pagesInBlock,
                                dataBuffer, pageSize + spareSize);
    }

    SDK_Free(dataBuffer);
}

int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p)
{
    uint8_t *dataBuffer = NULL;
    uint32_t i          = 0U;
    int isFree          = 1;

    dataBuffer = SDK_Malloc(DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea, 1U);
    assert(dataBuffer != NULL);

    memset(dataBuffer, 0U, DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea);

    if (Nand_Flash_Read_Page_Partial(DHARA_HANDLE_COVERT_TO_NAND(n), p, 0U, dataBuffer,
                                     DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea) != kStatus_Success)
    {
        isFree = -1;
    }
    else
    {
        for (i = 0U; i < DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea; i++)
        {
            if (dataBuffer[i] != DHARA_NAND_SDK_Free_MARK)
            {
                isFree = 0;
            }
        }
    }

    SDK_Free(dataBuffer);

    return isFree;
}

int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err)
{
    uint8_t *dataBuffer = NULL;
    uint32_t pageSize   = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageDataArea;
    uint32_t spareSize  = DHARA_HANDLE_COVERT_TO_NAND(n)->bytesInPageSpareArea;
    int isDone          = 0;

    dataBuffer = SDK_Malloc(pageSize + spareSize, 1U);
    assert(dataBuffer != NULL);
    memset(dataBuffer, 0U, pageSize + spareSize);

    if (Nand_Flash_Read_Page(DHARA_HANDLE_COVERT_TO_NAND(n), src, dataBuffer, pageSize + spareSize) != kStatus_Success)
    {
        isDone = -1;
    }
    else
    {
#if defined EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
        uint32_t i = 0U;
        hamming_ecc_t e;
        uint8_t *eccBuffer   = NULL;
        uint32_t eccTrunkNum = (spareSize + pageSize) / (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK);

        for (i = 0; i < eccTrunkNum; i++)
        {
            eccBuffer = i * (DHARA_HAMMING_CHUNK_SIZE + DHARA_HAMMING_ECC_SIZE_PER_CHUNK) + dataBuffer;
            e         = hamming_syndrome(eccBuffer, DHARA_HAMMING_CHUNK_SIZE, eccBuffer + DHARA_HAMMING_CHUNK_SIZE);

            if (e)
            {
                hamming_repair(eccBuffer, DHARA_HAMMING_CHUNK_SIZE, e);
            }
        }

#endif

        if (Nand_Flash_Page_Program(DHARA_HANDLE_COVERT_TO_NAND(n), dst, dataBuffer, pageSize + spareSize) !=
            kStatus_Success)
        {
            isDone = -1;
            /* If page program failed, mark this specific block as bad block. */
            dhara_set_error(err, DHARA_E_BAD_BLOCK);
        }
    }

    SDK_Free(dataBuffer);

    return isDone;
}
