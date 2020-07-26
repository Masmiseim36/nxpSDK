/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic Tools Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <phTools.h>
#include <ph_RefDefs.h>
#include <NxpNfcRdLib_Ver.h>

#ifdef _WIN32
#pragma warning(push)           /* PRQA S 3116 */
#pragma warning(disable:4001)   /* PRQA S 3116 */
#include <stdlib.h>
#include <string.h> /* memcpy */
#pragma warning(pop)                /* PRQA S 3116 */
#else
#include <string.h> /* memcpy */
#endif

static uint8_t phTools_CalcParity(uint8_t bDataByte, uint8_t bOption);

phStatus_t phTools_EncodeParity(
                                uint8_t bOption,
                                uint8_t * pInBuffer,
                                uint16_t wInBufferLength,
                                uint8_t bInBufferBits,
                                uint16_t wOutBufferSize,
                                uint8_t * pOutBuffer,
                                uint16_t * pOutBufferLength,
                                uint8_t * pOutBufferBits
                                )
{
    uint16_t    PH_MEMLOC_REM wByteIndexIn;
    uint16_t    PH_MEMLOC_REM wByteIndexOut;
    uint8_t     PH_MEMLOC_REM bBitPosition;
    uint16_t    PH_MEMLOC_REM wInByteCount;

    /* Parameter check */
    if (((bOption != PH_TOOLS_PARITY_OPTION_EVEN) && (bOption != PH_TOOLS_PARITY_OPTION_ODD)) || (bInBufferBits > 7U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    /* Retrieve full input byte count */
    if (bInBufferBits == 0U)
    {
        wInByteCount = wInBufferLength;
    }
    else
    {
        wInByteCount = wInBufferLength - 1u;
    }

    /* Retrieve number of (additional) full bytes */
    (*pOutBufferLength) = (uint16_t)((uint16_t)(wInByteCount + bInBufferBits) >> 3U);

    /* Retrieve output bits */
    *pOutBufferBits = (uint8_t)((uint16_t)(wInByteCount + bInBufferBits) % 8U);

    /* Increment output length in case of incomplete byte */
    if (*pOutBufferBits > 0U)
    {
        ++(*pOutBufferLength);
    }

    /* Overflow check */
    if ((*pOutBufferLength) > (0xFFFFU - wInByteCount))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    /* Calculate number of output bytes */
    (*pOutBufferLength) = wInByteCount + (*pOutBufferLength);

    /* Buffer overflow check*/
    if (wOutBufferSize < (*pOutBufferLength))
    {
        (*pOutBufferLength) = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_TOOLS);
    }

    /* Prepare output buffer */
    pOutBuffer[0] = 0x00;

    /* Prepare loop vars */
    wByteIndexIn = 0;
    wByteIndexOut = 0;
    bBitPosition = 7;

    /* Do for each byte */
    for (; wByteIndexIn < wInBufferLength; ++wByteIndexIn )
    {
        /* Append source bits to output */
        pOutBuffer[wByteIndexOut] |= (uint8_t)(pInBuffer[wByteIndexIn] << (7U - bBitPosition));

        /* If there is more data bits in the sourcebyte append it to next data byte */
        if ((wByteIndexOut + 1U) < (*pOutBufferLength))
        {
            if(bBitPosition == 7U)
            {
            pOutBuffer[wByteIndexOut + 1U]  = 0;
            }
            else
            {
            pOutBuffer[wByteIndexOut + 1U]  = (uint8_t)(pInBuffer[wByteIndexIn] >> (1u + bBitPosition));
            }

            /* Perform parity appending if this isn't an incomplete byte */
            if ((bInBufferBits == 0U) || ((wByteIndexIn + 1U) < wInBufferLength))
            {
                pOutBuffer[wByteIndexOut + 1U] |= (uint8_t)(phTools_CalcParity(pInBuffer[wByteIndexIn], bOption) << (7U - bBitPosition));
            }
        }

        /* We have reached the 8th parity bit, the output buffer index is now one ahead */
        if (bBitPosition == 0U)
        {
            if ((wByteIndexOut + 2U) < (*pOutBufferLength))
            {
                bBitPosition = 8;
                pOutBuffer[++wByteIndexOut + 1U] = 0x00;
            }
        }

        ++wByteIndexOut;
        --bBitPosition;
    }

    /* Mask out invalid bits of last byte */
    if (*pOutBufferBits > 0U)
    {
        pOutBuffer[(*pOutBufferLength) - 1U] &= (uint8_t)(0xFFU >> (8U - *pOutBufferBits));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_DecodeParity(
                                uint8_t bOption,
                                uint8_t * pInBuffer,
                                uint16_t wInBufferLength,
                                uint8_t bInBufferBits,
                                uint16_t wOutBufferSize,
                                uint8_t * pOutBuffer,
                                uint16_t * pOutBufferLength,
                                uint8_t * pOutBufferBits
                                )
{
    uint16_t    PH_MEMLOC_REM wByteIndexIn;
    uint16_t    PH_MEMLOC_REM wByteIndexOut;
    uint8_t     PH_MEMLOC_REM bBitPosition;
    uint16_t    PH_MEMLOC_REM wDiv;
    uint8_t     PH_MEMLOC_REM bMod;
    uint8_t     PH_MEMLOC_REM bParity;

    /* Parameter check */
    if (((bOption != PH_TOOLS_PARITY_OPTION_EVEN) && (bOption != PH_TOOLS_PARITY_OPTION_ODD)) || (bInBufferBits > 7U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    /* Parameter check */
    if (wInBufferLength == 0U)
    {
        /* Zero input length is simply passed through */
        if (bInBufferBits == 0U)
        {
            (*pOutBufferLength) = 0;
            *pOutBufferBits = 0;
            return PH_ERR_SUCCESS;
        }
        /* Invalid parameter */
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
        }
    }

    /* Retrieve DIV and MOD */
    if (bInBufferBits == 0U)
    {
        wDiv = (uint16_t)(wInBufferLength / 9U);
        bMod = (uint8_t)(wInBufferLength % 9U);
    }
    else
    {
        wDiv = (uint16_t)((wInBufferLength - 1U) / 9U);
        bMod = (uint8_t)((wInBufferLength - 1U) % 9U);
    }

    /* Calculate number of output bytes */
    (*pOutBufferLength) = (uint16_t)((wDiv << 3U) + bMod);
    if (bMod > bInBufferBits)
    {
        --(*pOutBufferLength);
    }

    /* Calculate number of rest-bits of output */
    *pOutBufferBits = (uint8_t)((8U - (((8U + ((*pOutBufferLength) % 8U)) - bInBufferBits) % 8U)) % 8U);

    /* Increment output length in case of incomplete byte */
    if (*pOutBufferBits > 0U)
    {
        ++(*pOutBufferLength);
    }

    /* Buffer overflow check*/
    if (wOutBufferSize < (*pOutBufferLength))
    {
        (*pOutBufferLength) = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_TOOLS);
    }

    /* Prepare loop vars */
    wByteIndexIn = 0U;
    wByteIndexOut = 0U;
    bBitPosition = 7U;

    /* Do for each byte */
    for (; wByteIndexOut < (*pOutBufferLength); ++wByteIndexOut, ++wByteIndexIn, --bBitPosition)
    {
        /* Append source bits to output */
        pOutBuffer[wByteIndexOut] = (uint8_t)(pInBuffer[wByteIndexIn] >> (7U - bBitPosition));

        /* If there is more data bits in the sourcebyte append it to next data byte */
        if ((wByteIndexIn + /* */ 1U) < wInBufferLength)
        {
            /* Append remaining bits to output */
            pOutBuffer[wByteIndexOut] |= (uint8_t)(pInBuffer[wByteIndexIn + 1U] << (1U + bBitPosition));

            /* Perform parity checking if this isn't an incomplete byte */
            if ((*pOutBufferBits == 0U)
                || ((wByteIndexOut + /* */ 1U) < (*pOutBufferLength)))
            {
                bParity = phTools_CalcParity(pOutBuffer[wByteIndexOut], bOption);
                if ((pInBuffer[wByteIndexIn + 1U] & (uint8_t)(1U << (7U - bBitPosition))) != (bParity << (7U - bBitPosition)))
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_TOOLS);
                }
            }
        }

        /* We have reached the 8th parity bit, the input buffer index is now one ahead */
        if (bBitPosition == 0U)
        {
            bBitPosition = 8;
            ++wByteIndexIn;
        }
    }

    /* Mask out invalid bits of last byte */
    if (*pOutBufferBits > 0U)
    {
        pOutBuffer[(*pOutBufferLength) - 1U] &= (uint8_t)(0xFFU >> (8U - *pOutBufferBits));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_CalculateCrc5(
                                 uint8_t bOption,
                                 uint8_t bPreset,
                                 uint8_t bPolynom,
                                 uint8_t * pData,
                                 uint16_t wDataLength,
                                 uint8_t * pCrc
                                 )
{
    uint16_t PH_MEMLOC_REM wDataIndex = 0;
    uint8_t  PH_MEMLOC_REM bBitIndex;
    uint8_t  PH_MEMLOC_REM bBitMax;

    if (0u != (bOption & (uint8_t)~(uint8_t)PH_TOOLS_CRC_OPTION_MASK))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    *pCrc = bPreset;

    if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
    {
        /* Shift 5bit preset to 8bit (data) alignment */
        *pCrc <<= 3U;

        /* Shift 5bit polinom to 8bit (data) alignment */
        bPolynom <<= 3U;
    }

    /* Loop through all data bytes */
    while (0U != wDataLength)
    {
        /* XOR input data */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_BITWISE))
        {
            if (wDataLength < 8U)
            {
                bBitMax = (uint8_t)wDataLength;
                wDataLength = 0;
            }
            else
            {
                bBitMax = 8;
                wDataLength -= 8u;
            }
        }
        else
        {
            bBitMax = 8;
            /* Decrement DataLen */
            --wDataLength;
        }

        /* CRC polynom (MSB first) */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
        {
            *pCrc ^= pData[wDataIndex++] & (0xFFU << (8U - bBitMax));

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x80U))
                {
                    *pCrc = (uint8_t)(((*pCrc) << 1U) ^ bPolynom);
                }
                else
                {
                    *pCrc = (*pCrc) << 1U;
                }
            }
        }
        /* CRC polynom (LSB first) */
        else
        {
            *pCrc ^= pData[wDataIndex++] & (0xFFU >> (8U - bBitMax));

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x01U))
                {
                    *pCrc = (uint8_t)(((*pCrc) >> 1U) ^ bPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) >> 1U);
                }
            }
        }
    }

    if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
    {
        /* Shift back for 5bit alignment */
        *pCrc >>= 3U;
    }

    /* Invert CRC if requested */
    if (0U != (bOption & PH_TOOLS_CRC_OPTION_OUPUT_INVERTED))
    {
        *pCrc ^= 0x1FU;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_CalculateCrc8(
                                 uint8_t bOption,
                                 uint8_t bPreset,
                                 uint8_t bPolynom,
                                 uint8_t * pData,
                                 uint16_t wDataLength,
                                 uint8_t * pCrc
                                 )
{
    uint16_t PH_MEMLOC_REM wDataIndex = 0;
    uint8_t  PH_MEMLOC_REM bBitIndex;
    uint8_t  PH_MEMLOC_REM bBitMax;

    if (0u != (bOption & (uint8_t)~(uint8_t)PH_TOOLS_CRC_OPTION_MASK))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    *pCrc = bPreset;

    /* Loop through all data bytes */
    while (0U != wDataLength)
    {
        /* XOR input data */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_BITWISE))
        {
            if (wDataLength < 8U)
            {
                bBitMax = (uint8_t)wDataLength;
                wDataLength = 0;
            }
            else
            {
                bBitMax = 8;
                wDataLength -= 8u;
            }
        }
        else
        {
            bBitMax = 8;
            /* Decrement DataLen */
            --wDataLength;
        }

        /* CRC polynom (MSB first) */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
        {
            *pCrc ^= pData[wDataIndex++] & (0xFFU << (8U - bBitMax));

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0U != ((*pCrc) & 0x80U))
                {
                    *pCrc = (uint8_t)(((*pCrc) << 1U) ^ bPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) << 1U);
                }
            }
        }
        /* CRC polynom (LSB first) */
        else
        {
            *pCrc ^= pData[wDataIndex++] & (0xFFU >> (8U - bBitMax));

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x01U))
                {
                    *pCrc = (uint8_t)(((*pCrc) >> 1U) ^ bPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) >> 1U);
                }
            }
        }
    }

    /* Invert CRC if requested */
    if (0U != (bOption & PH_TOOLS_CRC_OPTION_OUPUT_INVERTED))
    {
        *pCrc ^= 0xFFU;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_CalculateCrc16(
                                  uint8_t bOption,
                                  uint16_t wPreset,
                                  uint16_t wPolynom,
                                  uint8_t * pData,
                                  uint16_t wDataLength,
                                  uint16_t * pCrc
                                  )
{
    uint16_t PH_MEMLOC_REM wDataIndex = 0;
    uint8_t  PH_MEMLOC_REM bBitIndex;
    uint8_t  PH_MEMLOC_REM bBitMax;

    if (0u != (bOption & (uint8_t)~(uint8_t)PH_TOOLS_CRC_OPTION_MASK))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    *pCrc = wPreset;

    /* Loop through all data bytes */
    while (0U != wDataLength)
    {
        /* XOR input data */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_BITWISE))
        {
            if (wDataLength < 8U)
            {
                bBitMax = (uint8_t)wDataLength;
                wDataLength = 0;
            }
            else
            {
                bBitMax = 8;
                wDataLength -= 8u;
            }
        }
        else
        {
            bBitMax = 8;
            /* Decrement DataLen */
            --wDataLength;
        }

        /* CRC polynom (MSB first) */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
        {
            *pCrc ^= (uint16_t)pData[wDataIndex++] << 8U;

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x8000U))
                {
                    *pCrc = (uint16_t)(((*pCrc) << 1U) ^ wPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) << 1U);
                }
            }
        }
        /* CRC polynom (LSB first) */
        else
        {
            *pCrc ^= pData[wDataIndex++];

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0U != ((*pCrc) & 0x0001U))
                {
                    *pCrc = (uint16_t)(((*pCrc) >> 1U) ^ wPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) >> 1U);
                }
            }
        }
    }

    /* Invert CRC if requested */
    if (0U != (bOption & PH_TOOLS_CRC_OPTION_OUPUT_INVERTED))
    {
        *pCrc ^= 0xFFFFU;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_CalculateCrc32(
                                  uint8_t bOption,
                                  uint32_t dwPreset,
                                  uint32_t dwPolynom,
                                  uint8_t * pData,
                                  uint16_t wDataLength,
                                  uint32_t * pCrc
                                  )
{
    uint16_t PH_MEMLOC_REM wDataIndex = 0;
    uint8_t  PH_MEMLOC_REM bBitIndex;
    uint8_t  PH_MEMLOC_REM bBitMax;

    if (0u != (bOption & (uint8_t)~(uint8_t)PH_TOOLS_CRC_OPTION_MASK))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TOOLS);
    }

    *pCrc = dwPreset;

    /* Loop through all data bytes */
    while (0U != wDataLength)
    {
        /* XOR input data */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_BITWISE))
        {
            if (wDataLength < 8U)
            {
                bBitMax = (uint8_t)wDataLength;
                wDataLength = 0;
            }
            else
            {
                bBitMax = 8;
                wDataLength -= 8u;
            }
        }
        else
        {
            bBitMax = 8;
            /* Decrement DataLen */
            --wDataLength;
        }

        /* CRC polynom (MSB first) */
        if (0U != (bOption & PH_TOOLS_CRC_OPTION_MSB_FIRST))
        {
            *pCrc ^= (uint32_t)pData[wDataIndex++] << 24U;

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x80000000U))
                {
                    *pCrc = (uint32_t)(((*pCrc) << 1U) ^ dwPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) << 1U);
                }
            }
        }
        /* CRC polynom (LSB first) */
        else
        {
            *pCrc ^= pData[wDataIndex++];

            for (bBitIndex = 0; bBitIndex < bBitMax; ++bBitIndex)
            {
                if (0u != ((*pCrc) & 0x00000001U))
                {
                    *pCrc = (uint32_t)(((*pCrc) >> 1U) ^ dwPolynom);
                }
                else
                {
                    *pCrc = ((*pCrc) >> 1U);
                }
            }
        }
    }

    /* Invert CRC if requested */
    if (0U != (bOption & PH_TOOLS_CRC_OPTION_OUPUT_INVERTED))
    {
        *pCrc ^= 0xFFFFFFFFU;
    }

    return PH_ERR_SUCCESS;
}

static uint8_t phTools_CalcParity(
                                  uint8_t bDataByte,
                                  uint8_t bOption
                                  )
{
    uint8_t PH_MEMLOC_REM bBit;
    uint8_t PH_MEMLOC_REM bParity;

    if (bOption == PH_TOOLS_PARITY_OPTION_EVEN)
    {
        bParity = 0x00;
    }
    else
    {
        bParity = 0x01;
    }

    for (bBit = 0; bBit < 8U; ++bBit)
    {
        if (0U != (bDataByte & (uint8_t)(1U << bBit)))
        {
            ++bParity;
        }
    }
    return bParity & 0x01U;
}

static void phTools_UpdateCrc_B(uint8_t bCh, uint16_t *pLpwCrc)
{
    bCh = (bCh^(uint8_t)((*pLpwCrc)&0x00FFU));
    bCh = (bCh ^ (bCh<<4U));
    *pLpwCrc = (*pLpwCrc >> 8U) ^ ((uint16_t)bCh << 8U) ^ ((uint16_t)bCh << 3U) ^ ((uint16_t)bCh>>4U);
}

phStatus_t phTools_ComputeCrc_B(
                                uint8_t *pData,
                                uint32_t dwLength,
                                uint8_t *pCrc)
{
    uint8_t PH_MEMLOC_REM bChBlock = 0;
    uint16_t PH_MEMLOC_REM wCrc = 0xFFFF;

    do
    {
        bChBlock = *pData++;
        phTools_UpdateCrc_B(bChBlock, &wCrc);
    } while (0u != (--dwLength));

    wCrc = ~wCrc;

    pCrc[0] = (uint8_t) (wCrc & 0xFFU);
    pCrc[1] = (uint8_t) ( (wCrc>>8U) & 0xFFU);

    return PH_ERR_SUCCESS;
}

#ifndef __min
#   define __min(X,Y) (((X) < (Y)) ? (X) : (Y))
#endif

phStatus_t phTools_GetVersion(
    uint8_t * pProductVer,
    uint16_t * pMajor,
    uint8_t * pMinor,
    uint16_t * pPatch_Dev,
    uint8_t * pVersionString,
    uint16_t * pVersionStringLen)
{
    uint16_t PH_MEMLOC_REM wCopyLen = 0;
    const char szNxpNfcRdLib_VER_FILEDESCRIPTION[] = NxpNfcRdLib_VER_FILEDESCRIPTION;

    PH_UNUSED_VARIABLE(pProductVer);

    /* *pProductVer = ; ankur update it when there ir product version */
    *pMajor = NxpNfcRdLib_VER_MAJOR;
    *pMinor = NxpNfcRdLib_VER_MINOR;
    *pPatch_Dev = NxpNfcRdLib_VER_DEV;

    wCopyLen = (uint16_t)(sizeof(szNxpNfcRdLib_VER_FILEDESCRIPTION)/sizeof(szNxpNfcRdLib_VER_FILEDESCRIPTION[0]));

    if(*pVersionStringLen < (wCopyLen - 1u))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_LENGTH_ERROR, PH_COMP_TOOLS);
    }
    else
    {
        wCopyLen = __min(wCopyLen,(*pVersionStringLen));

        (void)memcpy(pVersionString, szNxpNfcRdLib_VER_FILEDESCRIPTION, wCopyLen);

        *pVersionStringLen = wCopyLen - 1u;
    }

    return PH_ERR_SUCCESS;
}
