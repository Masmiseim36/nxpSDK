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
* Software ISO18000-3 Mode3 Application Layer (Internals).
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 20. July 2010
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_I18000P3M3_SW

#include <phalI18000p3m3.h>
#include "phalI18000p3m3_Sw_Int.h"

phStatus_t phalI18000p3m3_Sw_ParseError(
                                        phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint8_t * pRxData,
                                        uint16_t wRxDataLength,
                                        uint8_t bRxLastBits
                                        )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bError;
    uint8_t     PH_MEMLOC_REM bHandle[2];

    /* Parameter check */
    if (pRxData == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Check received length */
    if ((wRxDataLength != 4U) || (bRxLastBits != 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Check for Error-Bit */
    if (0U == (pRxData[0] & 0x80U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Check Handle */
    bHandle[0] = (uint8_t)((pRxData[1] << 1U) | (pRxData[2] >> 7U));
    bHandle[1] = (uint8_t)((pRxData[2] << 1U) | (pRxData[3] >> 7U));
    if ((bHandle[0] != pDataParams->abHandle[0]) ||
        (bHandle[1] != pDataParams->abHandle[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
    }

    /* Retrieve and map error code */
    bError = (uint8_t)((pRxData[0] << 1U) | (pRxData[1] >> 7U));
    switch (bError)
    {
    case PHAL_I18000P3M3_ERRCODE_OTHER:
        status = PH_ADD_COMPCODE_FIXED(PHAL_I18000P3M3_ERR_OTHER, PH_COMP_AL_I18000P3M3);
        break;
    case PHAL_I18000P3M3_ERRCODE_MEMORY_OVERRUN:
        status = PH_ADD_COMPCODE_FIXED(PHAL_I18000P3M3_ERR_MEMORY_OVERRUN, PH_COMP_AL_I18000P3M3);
        break;
    case PHAL_I18000P3M3_ERRCODE_MEMORY_LOCKED:
        status = PH_ADD_COMPCODE_FIXED(PHAL_I18000P3M3_ERR_MEMORY_LOCKED, PH_COMP_AL_I18000P3M3);
        break;
    case PHAL_I18000P3M3_ERRCODE_INSUFFICIENT_POWER:
        status = PH_ADD_COMPCODE_FIXED(PHAL_I18000P3M3_ERR_INSUFFICIENT_POWER, PH_COMP_AL_I18000P3M3);
        break;
    case PHAL_I18000P3M3_ERRCODE_NON_SPECIFIC:
        status = PH_ADD_COMPCODE_FIXED(PHAL_I18000P3M3_ERR_NON_SPECIFIC, PH_COMP_AL_I18000P3M3);
        break;
    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I18000P3M3);
        break;
    }

    return status;
}

#endif /* NXPBUILD__PHAL_I18000P3M3_SW */
