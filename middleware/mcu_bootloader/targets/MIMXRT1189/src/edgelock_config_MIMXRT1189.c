/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fsl_edgelock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _fuse_region_
{
    uint32_t startIndex; /*!< Start index for a fuse region.(closed interval) */
    uint32_t endIndex;   /*!< End index for a memory region.(open interval, better for shift operation) */
} fuse_region_t;

typedef struct _fuse_words_
{
    uint32_t index; /*!< Index of a fuse word. */
    uint32_t words; /*!< Returned words for the index, 0 for invalid. */
} fuse_words_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Check ReadCommonFuse command is supported to read the fuse word.
 *
 * @param fuseId Fuse word ID.
 * @return true for supported, false for not.
 */
static inline bool is_read_common_fuse_supported(uint32_t fuseId);

/*!
 * @brief Get the fuse words returned by ReadCommonFuse command.
 *
 * @param fuseId Fuse word ID.
 * @return Fuse word count.
 */
static inline uint32_t get_read_common_fuse_words(uint32_t fuseId);

/*!
 * @brief Read fuse words via OCOTP FSB.
 *
 * @param base OCOTP FSB base.
 * @param fuseId Fuse word ID.
 * @param pFuseValue Fuse value pointer.
 * @param fuseWords Fuse value word count.
 * @return 0 for success, otherwise return error code.
 */
static inline status_t FSB_ReadCommonFuse(OCOTP_FSB_Type *base,
                                          uint32_t fuseId,
                                          uint32_t *pFuseValue,
                                          uint32_t *pFuseWords);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief The fuse region supported by ReadCommonFuse command. */
static const fuse_region_t s_readCommonFuseRegionTable[] = {
    { 63, 64 },
    { 128, 144 },
    { 182, 183 },
    { 188, 189 },
};

/*! @brief The fuse words returned by ReadCommonFuse command. */
static const fuse_words_t s_readCommonFuseWordsTable[] = {
    { 0 }, /* All indexs return one word response. */
};

/*! @brief The fuse region allocated to OCOTP FSB shallows. */
static const fuse_region_t s_fsbAllocatedFuseRegionTable[] = {
    { 0, 52 },
    { 312, 512 },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline bool is_read_common_fuse_supported(uint32_t fuseId)
{
    for (int i = 0; i < ARRAY_SIZE(s_readCommonFuseRegionTable); i++)
    {
        if ((fuseId >= s_readCommonFuseRegionTable[i].startIndex) && (fuseId < s_readCommonFuseRegionTable[i].endIndex))
        {
            return true;
        }
    }
    return false;
}

static inline bool is_allocated_to_fsb(uint32_t fuseId)
{
    for (int i = 0; i < ARRAY_SIZE(s_fsbAllocatedFuseRegionTable); i++)
    {
        if ((fuseId >= s_fsbAllocatedFuseRegionTable[i].startIndex) &&
            (fuseId < s_fsbAllocatedFuseRegionTable[i].endIndex))
        {
            return true;
        }
    }
    return false;
}

static inline uint32_t get_read_common_fuse_words(uint32_t fuseId)
{
    for (int i = 0; i < ARRAY_SIZE(s_readCommonFuseWordsTable); i++)
    {
        if ((fuseId == s_readCommonFuseWordsTable[i].index) && (s_readCommonFuseWordsTable[i].words != 0))
        {
            return s_readCommonFuseWordsTable[i].words;
        }
    }

    return 1 /* Default is one word. */;
}

static inline status_t FSB_ReadCommonFuse(OCOTP_FSB_Type *base,
                                          uint32_t fuseId,
                                          uint32_t *pFuseValue,
                                          uint32_t *pFuseWords)
{
    for (int i = 0; i < *pFuseWords; i++)
    {
        uint32_t portAIndexStart = (uint32_t) & ((OCOTP_FSB_Type *)NULL)->OTP_SHADOW_PARTA / 4;
        uint32_t portAIndexEnd = portAIndexStart + sizeof(base->OTP_SHADOW_PARTA) / 4;
        uint32_t portBIndexStart = (uint32_t) & ((OCOTP_FSB_Type *)NULL)->OTP_SHADOW_PARTB / 4;
        uint32_t portBIndexEnd = portBIndexStart + sizeof(base->OTP_SHADOW_PARTB) / 4;

        if (((fuseId + i) >= portAIndexStart) && ((fuseId + i) < portAIndexEnd))
        {
            pFuseValue[i] = base->OTP_SHADOW_PARTA[fuseId + i - portAIndexStart];
        }
        else if (((fuseId + i) >= portBIndexStart) && ((fuseId + i) < portBIndexEnd))
        {
            pFuseValue[i] = base->OTP_SHADOW_PARTB[fuseId + i - portBIndexStart];
        }
        else
        {
            return kStatus_InvalidArgument;
        }
    }
    return kStatus_Success;
}

/*!
 * @brief Read fuse words.
 *
 * @param base Edgelock Message Unit base.
 * @param fuseId Fuse word ID.
 * @param pFuseValue Fuse value pointer.
 * @param fuseWords Fuse value word count.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_ReadFuse(SxMU_Type *base, uint32_t fuseId, uint32_t *pFuseValue, uint32_t fuseWords)
{
    status_t status;

    for (int index = 0, readWords = 0; readWords < fuseWords; index++)
    {
        bool isEdgelockRead = is_read_common_fuse_supported(fuseId + index);
        uint32_t words = 0;
        if (isEdgelockRead)
        {
            words = get_read_common_fuse_words(fuseId + index);
            if ((readWords + words) > fuseWords)
            {
                return kStatus_OutOfRange;
            }
            status = EDGELOCK_ReadCommonFuse(base, fuseId + index, pFuseValue + readWords, &words);
        }
        else
        {
            if (!is_allocated_to_fsb(fuseId))
            {
                return kStatus_OutOfRange;
            }
            words = 1; /* Always read one word each calling. */
            status = FSB_ReadCommonFuse(OCOTP_FSB, fuseId + index, pFuseValue + readWords, &words);
        }

        readWords += words;

        if (status != kStatus_Success)
        {
            break;
        }
    }

    return status;
}
