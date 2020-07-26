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
* Log Component of Reader Library Framework.
* $Author: nxp79920 $
* $Revision: 673 $ (v06.01.00)
* $Date: 2014-07-28 17:56:08 +0530 (Mon, 28 Jul 2014) $
*
* History:
*  CHu: Generated 09. October 2009
*
*/

#include <phTMIUtils.h>

#ifdef NXPBUILD__PH_TMIUTILS

#include <ph_Status.h>
#include <ph_RefDefs.h>

phStatus_t phTMIUtils_Init(
                           phTMIUtils_t * pDataParams,
                           uint8_t * pTMIBuffer,
                           uint32_t dwBufLen
                           )
{
    PH_ASSERT_NULL (pDataParams);
    /* Check parameters */
    if ((pTMIBuffer == NULL) || (dwBufLen == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);
    }
    pDataParams->pTMIBuffer = pTMIBuffer;
    pDataParams->dwTMIBufLen = dwBufLen;
    pDataParams->dwTMIbufIndex = 0;
    pDataParams->bTMIStatus = PH_OFF;
    pDataParams->dwOffsetInTMI = 0;

    return PH_ERR_SUCCESS;
}

phStatus_t phTMIUtils_ActivateTMICollection(
    phTMIUtils_t *pDataParams,
    uint8_t bOption
    )
{
    if ((pDataParams->dwTMIBufLen == 0U) || (pDataParams == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);
    }

    switch (bOption)
    {
    case PH_TMIUTILS_DEACTIVATE_TMI:

        /* Deactivate TMI Collection */
        pDataParams->dwTMIBufLen = 0;
        pDataParams->dwOffsetInTMI = 0;
        pDataParams->bTMIStatus = PH_OFF;
        break;

    case PH_TMIUTILS_ACTIVATE_TMI:
    case PH_TMIUTILS_RESUME_TMI:

        /* Activate/Resume TMI collection */
        pDataParams->bTMIStatus = PH_ON;
        break;

    case PH_TMIUTILS_PAUSE_TMI:

        /* Pause TMI collection */
        pDataParams->bTMIStatus = PH_OFF;
        break;

    case PH_TMIUTILS_RESET_TMI:

        /* Reset TMI collection buffer index to 0 */
        pDataParams->dwTMIbufIndex = 0;
        pDataParams->dwOffsetInTMI = 0;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);

    }
    return PH_ERR_SUCCESS;
}

phStatus_t phTMIUtils_GetTMI(
                             phTMIUtils_t * pDataParams,
                             uint8_t ** ppTMIBuffer,
                             uint32_t * dwTMILen
                             )
{
    PH_ASSERT_NULL (pDataParams);

    *ppTMIBuffer = pDataParams->pTMIBuffer;
    *dwTMILen = pDataParams->dwTMIbufIndex;

    return PH_ERR_SUCCESS;
}

phStatus_t phTMIUtils_CollectTMI(
                                 phTMIUtils_t * pDataParams,
                                 uint8_t bOption,
                                 uint8_t * pCmdBuff,
                                 uint16_t wCmdLen,
                                 uint8_t * pData,
                                 uint32_t dwDataLen,
                                 uint16_t wBlockSize
                                 )
{
    uint32_t    PH_MEMLOC_REM dwTmp = 0;

    PH_ASSERT_NULL (pDataParams);

    /* Parameter risk */
    if ((pCmdBuff == NULL) && (pData == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);
    }
    if ((pDataParams->dwTMIbufIndex + wCmdLen + (wBlockSize - (wCmdLen % wBlockSize)) + dwDataLen + (wBlockSize - (dwDataLen % wBlockSize))) > pDataParams->dwTMIBufLen)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_TMIUTILS);
    }

    if (0U != (wCmdLen))
    {
        /* Copy the command buffer */
        (void)memcpy(&pDataParams->pTMIBuffer[pDataParams->dwTMIbufIndex], pCmdBuff, wCmdLen);
        pDataParams->dwTMIbufIndex += wCmdLen;

        if ((0U != ((bOption & PH_TMIUTILS_ZEROPAD_CMDBUFF))) && ((pDataParams->dwTMIbufIndex % wBlockSize) != 0U))
        {
            /* Zero padding */
            (void)memset(&pDataParams->pTMIBuffer[pDataParams->dwTMIbufIndex], 0x00, (wBlockSize - (pDataParams->dwTMIbufIndex % wBlockSize)));
            pDataParams->dwTMIbufIndex += (wBlockSize - (pDataParams->dwTMIbufIndex % wBlockSize));
        }
    }
    if (0U != (bOption & PH_TMIUTILS_READ_INS))
    {
        if (0U != (pDataParams->dwOffsetInTMI))
        {
            /* Update the Length field Offset in pDataParams */
            dwTmp = pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI + 1U];
            dwTmp <<= 8U;
            dwTmp |= pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI];
            dwTmp += dwDataLen;
            pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI] = (uint8_t)dwTmp;
            pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI + 1U] = (uint8_t)(dwTmp >> 8U);
        }
        else
        {
            /* Store the Length field Offset in pDataParams */
            pDataParams->dwOffsetInTMI = (pDataParams->dwTMIbufIndex - 11u);
        }
    }

    if (0U != (dwDataLen))
    {
        /* Copy the command buffer */
        (void)memcpy(&pDataParams->pTMIBuffer[pDataParams->dwTMIbufIndex], pData, dwDataLen);
        pDataParams->dwTMIbufIndex += dwDataLen;

        if ((0U != ((bOption & PH_TMIUTILS_ZEROPAD_DATABUFF))) && ((pDataParams->dwTMIbufIndex % wBlockSize) != 0U))
        {
            /* Zero padding */
            (void)memset(&pDataParams->pTMIBuffer[pDataParams->dwTMIbufIndex], 0x00, (wBlockSize - (pDataParams->dwTMIbufIndex % wBlockSize)));
            pDataParams->dwTMIbufIndex += (wBlockSize - (pDataParams->dwTMIbufIndex % wBlockSize));
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTMIUtils_SetConfig(
                                phTMIUtils_t * pDataParams,
                                uint16_t wConfig,
                                uint32_t dwValue
                                )
{
    switch (wConfig)
    {
    case PH_TMIUTILS_TMI_OFFSET_LENGTH:
        pDataParams->dwOffsetInTMI  = dwValue;
        break;
    case PH_TMIUTILS_TMI_OFFSET_VALUE:
        pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI] = (uint8_t)dwValue;
        pDataParams->pTMIBuffer[pDataParams->dwOffsetInTMI + 1U] = (uint8_t)(dwValue >> 8U);
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTMIUtils_GetConfig(
                                phTMIUtils_t * pDataParams,
                                uint16_t wConfig,
                                uint32_t *pValue
                                )
{
    switch (wConfig)
    {
    case PH_TMIUTILS_TMI_STATUS:
        *pValue = pDataParams->bTMIStatus;
        break;
    case PH_TMIUTILS_TMI_OFFSET_LENGTH:
        *pValue = pDataParams->dwOffsetInTMI;
        break;
    case PH_TMIUTILS_TMI_BUFFER_INDEX:
        *pValue = pDataParams->dwTMIbufIndex;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_TMIUTILS);
    }

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PH_TMIUTILS */
