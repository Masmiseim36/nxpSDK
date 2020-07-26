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
* Generic Crypto RNG-Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/
#include <phCryptoRng.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PH_CRYPTORNG_SW
#include "Sw/phCryptoRng_Sw.h"
#endif /* NXPBUILD__PH_CRYPTORNG_SW */


#ifdef NXPBUILD__PH_CRYPTORNG

phStatus_t phCryptoRng_Seed(
                                    void * pDataParams,
                                    uint8_t * pSeed,
                                    uint8_t bSeedLength
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_ASSERT_NULL (pDataParams);
    if (0U != bSeedLength) PH_ASSERT_NULL (pSeed);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTORNG)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTORNG);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTORNG_SW
    case PH_CRYPTORNG_SW_ID:
        status = phCryptoRng_Sw_Seed((phCryptoRng_Sw_DataParams_t *)pDataParams,pSeed,bSeedLength);
        break;
#endif /* NXPBUILD__PH_CRYPTORNG_SW */

    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTORNG);
        break;
    }

    return status;
}

phStatus_t phCryptoRng_Rnd(
                            void * pDataParams,
                            uint16_t  wNoOfRndBytes,
                            uint8_t * pRnd
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoRng_Rnd");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNoOfRndBytes);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRnd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wNoOfRndBytes_log, &wNoOfRndBytes);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pRnd);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTORNG)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTORNG);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTORNG_SW
    case PH_CRYPTORNG_SW_ID:
        status = phCryptoRng_Sw_Rnd((phCryptoRng_Sw_DataParams_t *)pDataParams,wNoOfRndBytes,pRnd);
        break;
#endif /* NXPBUILD__PH_CRYPTORNG_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTORNG);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRnd_log, pRnd, wNoOfRndBytes);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPBUILD__PH_CRYPTORNG */
