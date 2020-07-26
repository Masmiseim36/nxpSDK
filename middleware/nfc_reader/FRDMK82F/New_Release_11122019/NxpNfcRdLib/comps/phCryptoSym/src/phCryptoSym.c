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
* Generic Generic HostComm Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phCryptoSym.h>
#include <ph_RefDefs.h>
#include "phCryptoSym_Int.h"

#ifdef NXPBUILD__PH_CRYPTOSYM_SW
#include "Sw/phCryptoSym_Sw.h"
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


#ifdef NXPBUILD__PH_CRYPTOSYM

phStatus_t phCryptoSym_InvalidateKey(
                                     void * pDataParams
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_InvalidateKey");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_InvalidateKey((phCryptoSym_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phCryptoSym_Encrypt(
                               void * pDataParams,
                               uint16_t wOption,
                               const uint8_t * pPlainBuffer,
                               uint16_t wBufferLength,
                               uint8_t * pEncryptedBuffer
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_Encrypt");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPlainBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncryptedBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPlainBuffer_log, pPlainBuffer, wBufferLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPlainBuffer);
    PH_ASSERT_NULL (pEncryptedBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_Encrypt((phCryptoSym_Sw_DataParams_t *)pDataParams, wOption, pPlainBuffer,wBufferLength,pEncryptedBuffer);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncryptedBuffer_log, pEncryptedBuffer, wBufferLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phCryptoSym_Decrypt(
                               void * pDataParams,
                               uint16_t wOption,
                               const uint8_t * pEncryptedBuffer,
                               uint16_t wBufferLength,
                               uint8_t * pPlainBuffer
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_Decrypt");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncryptedBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPlainBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncryptedBuffer_log, pEncryptedBuffer, wBufferLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pEncryptedBuffer);
    PH_ASSERT_NULL (pPlainBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_Decrypt((phCryptoSym_Sw_DataParams_t *)pDataParams,wOption,pEncryptedBuffer,wBufferLength,pPlainBuffer);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPlainBuffer_log, pPlainBuffer, wBufferLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}



phStatus_t phCryptoSym_CalculateMac(
                                    void * pDataParams,
                                    uint16_t wOption,
                                    const uint8_t* pData,
                                    uint16_t    wDataLength,
                                    uint8_t*    pMac,
                                    uint8_t*    pMacLength
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_CalculateMac");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMac);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, wDataLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pMac);
    PH_ASSERT_NULL (pMacLength);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_CalculateMac((phCryptoSym_Sw_DataParams_t *)pDataParams, wOption, pData,wDataLength,pMac,pMacLength);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMac_log, pMac, *pMacLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phCryptoSym_LoadIv(
                              void * pDataParams,
                              const uint8_t * pIV,
                              uint8_t bIVLength
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_LoadIv");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIV);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIV_log, pIV, bIVLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pIV);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_LoadIv((phCryptoSym_Sw_DataParams_t *)pDataParams, pIV, bIVLength);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phCryptoSym_LoadKey(
                               void * pDataParams,
                               uint16_t    wKeyNo,
                               uint16_t    wKeyVersion,
                               uint16_t    wKeyType
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_LoadKey");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyType_log, &wKeyType);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_LoadKey((phCryptoSym_Sw_DataParams_t *)pDataParams,wKeyNo,wKeyVersion,wKeyType);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phCryptoSym_LoadKeyDirect(
                                     void * pDataParams,
                                     const uint8_t*    pKey,
                                     uint16_t    wKeyType
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_LoadKeyDirect");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKey_log, pKey, phCryptoSym_GetKeySize(wKeyType));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyType_log, &wKeyType);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pKey);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_LoadKeyDirect((phCryptoSym_Sw_DataParams_t *)pDataParams,pKey,wKeyType);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phCryptoSym_DiversifyKey(
                                    void * pDataParams,
                                    uint16_t wOption,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVersion,
                                    uint8_t * pDivInput,
                                    uint8_t  bLenDivInput,
                                    uint8_t * pDiversifiedKey
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_DiversifyKey");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDiversifiedKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (0U != bLenDivInput) PH_ASSERT_NULL (pDivInput);
    PH_ASSERT_NULL (pDiversifiedKey);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_DiversifyKey((phCryptoSym_Sw_DataParams_t *)pDataParams, wOption, wKeyNo, wKeyVersion, pDivInput, bLenDivInput, pDiversifiedKey);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDiversifiedKey_log, pDiversifiedKey, bLenDivInput);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phCryptoSym_DiversifyDirectKey(
    void * pDataParams,
    uint16_t wOption,
    uint8_t * pKey,
    uint16_t wKeyType,
    uint8_t * pDivInput,
    uint8_t bLenDivInput,
    uint8_t * pDiversifiedKey
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_DiversifyDirectKey");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDiversifiedKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKey_log, pKey, phCryptoSym_GetKeySize(wKeyType));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wKeyType_log, &wKeyType);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pKey);
    if (0U != (bLenDivInput)) PH_ASSERT_NULL (pDivInput);
    PH_ASSERT_NULL (pDiversifiedKey);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_DiversifyDirectKey((phCryptoSym_Sw_DataParams_t *)pDataParams, wOption, pKey, wKeyType, pDivInput, bLenDivInput, pDiversifiedKey);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDiversifiedKey_log, pDiversifiedKey, bLenDivInput);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
phStatus_t phCryptoSym_SetConfig(
                                 void * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t wValue
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_SetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_SetConfig((phCryptoSym_Sw_DataParams_t *)pDataParams,wConfig,wValue);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}



phStatus_t phCryptoSym_GetConfig(
                                 void * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t * pValue
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phCryptoSym_GetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CRYPTOSYM)
    {
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM_SW
    case PH_CRYPTOSYM_SW_ID:
        status = phCryptoSym_Sw_GetConfig((phCryptoSym_Sw_DataParams_t *)pDataParams,wConfig,pValue);
        break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */


    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CRYPTOSYM);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phCryptoSym_ApplyPadding(
                                    uint8_t         bOption,
                                    const uint8_t * pDataIn,
                                    uint16_t        wDataInLength,
                                    uint8_t         bBlockSize,
                                    uint16_t        wDataOutBufSize,
                                    uint8_t *       pDataOut,
                                    uint16_t *      pDataOutLength
                                    )
{
    uint8_t bPaddingLength;
    (*pDataOutLength) = 0;

    /* How many bytes we have to pad? */
    bPaddingLength = (uint8_t)(wDataInLength % bBlockSize);
    if ((bPaddingLength == 0x00U) &&(bOption != PH_CRYPTOSYM_PADDING_MODE_1))
    {
        bPaddingLength = (uint8_t)bBlockSize;
    }else
    {
        bPaddingLength = (uint8_t)(bBlockSize - bPaddingLength);
    }

    /* Do we have sufficient space in the output buffer? */
    if (wDataOutBufSize < (wDataInLength + bPaddingLength))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_CRYPTOSYM);
    }

    /* Recopy the array in case pDataIn != pDataOut */
    if (pDataIn != pDataOut)
    {
        (void)memcpy(pDataOut, pDataIn, wDataInLength);
    }

    /* Add the padding constant */
    switch(bOption)
    {
    case PH_CRYPTOSYM_PADDING_MODE_1:
        pDataOut[wDataInLength++] = 0x00;
        break;
    case PH_CRYPTOSYM_PADDING_MODE_2:
        pDataOut[wDataInLength++] = 0x80;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }
    bPaddingLength--;
    while(0U != (bPaddingLength--))
    {
        /* Add zeros */
        pDataOut[wDataInLength++] = 0x00;
    }

    (*pDataOutLength) = wDataInLength;
    return PH_ERR_SUCCESS;

}

phStatus_t phCryptoSym_RemovePadding(
                                     uint8_t            bOption,
                                     const uint8_t *    pDataIn,
                                     uint16_t           wDataInLength,
                                     uint8_t            bBlockSize,
                                     uint16_t           wDataOutBufSize,
                                     uint8_t *          pDataOut,
                                     uint16_t *         pDataOutLength
                                     )
{
    uint8_t bFinished = 0;
    (*pDataOutLength) = 0;

    /* Is the data in length a multiple of the blocksize?? */
    if (0U != (wDataInLength % bBlockSize))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
    }

    /* find the padding constant */
    while((wDataInLength > 0U)&&(bFinished == 0U))
    {
        wDataInLength--;
        switch(bOption)
        {
        case PH_CRYPTOSYM_PADDING_MODE_1:
            if (pDataIn[wDataInLength] != 0x00U)
            {
                bFinished = 1;
                wDataInLength++;
                break;
            }else
            {
                /* Continue processing */
            }
            break;
        case PH_CRYPTOSYM_PADDING_MODE_2:
            if (pDataIn[wDataInLength] == 0x80U)
            {
                bFinished = 1;
                break;
            }else if (pDataIn[wDataInLength] != 0x00U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_FRAMING_ERROR, PH_COMP_CRYPTOSYM);
            }else
            {
                ;/* Continue processing */
            }
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_CRYPTOSYM);
        }
    }

    /* In case of it is not the last byte, we can set the correct new data out length, otherwise we have a out data length of 0. */
    (*pDataOutLength) = wDataInLength;

    /* do we have a buffer overflow? */
    if (wDataOutBufSize < (*pDataOutLength))
    {
        (*pDataOutLength) = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_CRYPTOSYM);
    }

    /* Recopy the array in case pDataIn != pDataOut */
    if (pDataIn != pDataOut)
    {
        if((*pDataOutLength) != 0U)
        {
            (void)memcpy(pDataOut, pDataIn, (size_t)(*pDataOutLength));
        }
    }

    return PH_ERR_SUCCESS;
}

uint16_t phCryptoSym_GetKeySize(
                                uint16_t wKeyType
                                )
{
    switch (wKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_DES:
        return 8;
    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        return 16;
    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        return 24;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        return 32;
    default:
        return 0;
    }
}

#endif /* NXPBUILD__PH_CRYPTOSYM */
