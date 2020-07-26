/*
 *                     Copyright 2016-2018, NXP
 *
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
* Generic MIFARE DESFire Light Application Component of Reader Library Framework.
* $Author: nxp79567 $
* $Revision: 1791 $ (v06.01.00)
* $Date: 2016-05-10 16:07:13 +0530 (Tue, 10 May 2016) $
*
*/

#include <phalMfdfLight.h>
#include <ph_Status.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
#include "Sw/phalMfdfLight_Sw.h"
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



#ifdef NXPBUILD__PHAL_MFDFLIGHT

#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE DESFire Light Secure Messaging commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_AuthenticateEv2(void *pDataParams, uint8_t bFirstAuth, uint16_t wOption, uint16_t wKeyNo, uint16_t wKeyVer, uint8_t bKeyNoCard,
    uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *bPcdCapsIn, uint8_t *bPcdCapsOut, uint8_t *bPdCapsOut)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_AuthenticatEv2");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFirstAuth);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPcdCapsIn);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPcdCapsOut);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPdCapsOut);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFirstAuth_log, &bFirstAuth);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVer_log, &wKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPcdCapsIn_log, bPcdCapsIn, bLenPcdCapsIn);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams,PH_COMP_AL_MFDFLIGHT);
    if (bDivLen) PH_ASSERT_NULL_PARAM(pDivInput,PH_COMP_AL_MFDFLIGHT);
    if(bLenPcdCapsIn)  PH_ASSERT_NULL_PARAM(bPcdCapsIn,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_AuthenticateEv2((phalMfdfLight_Sw_DataParams_t *)pDataParams, bFirstAuth, wOption, wKeyNo, wKeyVer, bKeyNoCard,
            pDivInput, bDivLen, bLenPcdCapsIn, bPcdCapsIn, bPcdCapsOut, bPdCapsOut);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if (bFirstAuth)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPcdCapsOut_log, bPcdCapsOut, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPdCapsOut_log, bPdCapsOut, 6);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light Memory and Configuration commands. ----------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_SetConfiguration(void * pDataParams, uint8_t bOption, uint8_t * pData, uint8_t bDataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_SetConfiguration");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDataLen_log, &bDataLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, bDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pData,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {

#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_SetConfiguration((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, pData, bDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_GetCardUID(void * pDataParams, uint8_t * pUid)
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t PH_MEMLOC_REM bCardUidLength = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetCardUID");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCardUidLength);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pUid,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetCardUID((phalMfdfLight_Sw_DataParams_t *)pDataParams, pUid, &bCardUidLength);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bCardUidLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCardUidLength_log, &bCardUidLength);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_GetVersion(void * pDataParams, uint8_t * pVerInfo)
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t PH_MEMLOC_REM bVersionLength = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVerInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVersionLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pVerInfo,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetVersion((phalMfdfLight_Sw_DataParams_t *)pDataParams, pVerInfo,&bVersionLength);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVerInfo_log, pVerInfo, bVersionLength);
     PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pVerInfo_log, &bVersionLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pVersionLength_log, &bVersionLength);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE DESFire Light Key Management commands. --------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_ChangeKey(void * pDataParams, uint16_t wOption, uint16_t wOldKeyNo, uint16_t wOldKeyVer, uint16_t wNewKeyNo, uint16_t wNewKeyVer,
    uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen)
{
    /**
    * This  function will handle all the three authentication modes: 0x0A, 1A and AA.
    * and all crypto modes i.e., DES, 3DES, 3K3DES, AES
    * The previous authentication status including key number and session key is
    * present in the params  structure.
    * Successful auth. with PICC master key is required if AID = 0x00 else
    * an auth. with the application master key is required.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ChangeKey");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOldKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNewKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOldKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNewKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyVer_log, &wOldKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyVer_log, &wNewKeyVer);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyNo_log, &wOldKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyNo_log, &wNewKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivLen_log, &bDivLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    if (bDivLen > 0)PH_ASSERT_NULL_PARAM (pDivInput,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ChangeKey((phalMfdfLight_Sw_DataParams_t *)pDataParams, wOption, wOldKeyNo, wOldKeyVer, wNewKeyNo, wNewKeyVer,
            bKeyNoCard, pDivInput, bDivLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalMfdfLight_GetKeyVersion(void * pDataParams, uint8_t bKeyNo, uint8_t bKeySetNo, uint8_t * pKeyVersion, uint8_t * bRxLen)
{
    /**
    * This command can be issued without valid authentication
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetKeyVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySetNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRxLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNo_log, &bKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySetNo_log, &bKeySetNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pKeyVersion,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetKeyVersion((phalMfdfLight_Sw_DataParams_t *)pDataParams, bKeyNo, bKeySetNo, pKeyVersion, bRxLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKeyVersion_log, pKeyVersion, *bRxLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRxLen_log, bRxLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light File Management commands. -------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_CreateTransactionMacFile(void * pDataParams, uint8_t bFileNo, uint8_t bCommMode, uint8_t * pAccessRights, uint8_t bKeyType,
    uint8_t * bTMKey, uint16_t wKeyNo, uint8_t bTMKeyVer, uint8_t * pDivInput, uint8_t bDivInputLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_CreateTransactionMacFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTMKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTMKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivInputLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommMode_log, &bCommMode);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTMKeyVer_log, &bTMKeyVer);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bTMKey_log, bTMKey, 16);
    if(pDivInput != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivInputLen_log, &bDivInputLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pAccessRights, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (bTMKey, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_CreateTransactionMacFile((phalMfdfLight_Sw_DataParams_t *)pDataParams, bFileNo, bCommMode, pAccessRights, bKeyType,
            bTMKey, bTMKeyVer);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_ChangeFileSettings(void * pDataParams, uint8_t bCommMode, uint8_t bFileNo, uint8_t bFileOption, uint8_t *pAccessRights,
    uint8_t bTmcLimitLen, uint8_t * pTmcLimit)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ChangeFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTmcLimitLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTmcLimit);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommMode_log, &bCommMode);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileOption_log, &bFileOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTmcLimitLen_log, &bTmcLimitLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTmcLimit_log, pTmcLimit, bTmcLimitLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pAccessRights, PH_COMP_AL_MFDFLIGHT);
    if(bTmcLimitLen) PH_ASSERT_NULL_PARAM (pTmcLimit, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ChangeFileSettings((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommMode, bFileNo, bFileOption, pAccessRights, bTmcLimitLen,
            pTmcLimit);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_DeleteFile(void * pDataParams, uint8_t bFileNo)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_DeleteFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_DeleteFile((phalMfdfLight_Sw_DataParams_t *)pDataParams, bFileNo);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_GetFileIDs(void * pDataParams, uint8_t * pFid, uint8_t * bNumFID)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetFileIDs");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumFID);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pFid,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (bNumFID,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetFileIDs((phalMfdfLight_Sw_DataParams_t *)pDataParams, pFid, bNumFID);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid, (*bNumFID));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bNumFID_log, bNumFID, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_GetISOFileIDs(void * pDataParams, uint8_t * pFidBuffer, uint8_t * pNumFID)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetISOFileIDs");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFidBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumFID);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pFidBuffer,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pNumFID,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetISOFileIDs((phalMfdfLight_Sw_DataParams_t *)pDataParams, pFidBuffer, pNumFID);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFidBuffer_log, pFidBuffer, (*pNumFID) * 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pNumFID_log, pNumFID, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_GetFileSettings(void * pDataParams, uint8_t bFileNo, uint8_t * pFSBuffer, uint8_t * bBufferLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFSBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBufferLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pFSBuffer,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (bBufferLen,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetFileSettings((phalMfdfLight_Sw_DataParams_t *)pDataParams, bFileNo, pFSBuffer, bBufferLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,pFSBuffer_log, pFSBuffer, (*bBufferLen));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bBufferLen_log, bBufferLen, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light Data Management commands. -------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_ReadData(void * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pLength, uint8_t ** ppRxdata,
    uint16_t * pRxdataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ReadData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIns);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxdataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIns_log, &bIns);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset, 3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pLength_log, pLength, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pOffset,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pLength,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (ppRxdata,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRxdataLen,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ReadData((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, bIns, bFileNo, pOffset, pLength, ppRxdata, pRxdataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if ((status == PH_ERR_SUCCESS) && ((*pRxdataLen) != 0))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxdata_log, *ppRxdata, (*pRxdataLen));
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pRxdataLen_log, pRxdataLen);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_WriteData(void * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pTxData, uint8_t * pTxDataLen)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_WriteData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIns);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIns_log, &bIns);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset, 3);

    if(pTxDataLen != NULL)
    {
        wDataLen = (uint16_t)pTxDataLen[1];
        wDataLen = wDataLen << 8;
        wDataLen |= pTxDataLen[0];
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxData_log, pTxData, wDataLen);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxDataLen_log, pTxDataLen, 3);
    }
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pOffset,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pTxData,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pTxDataLen,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_WriteData((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, bIns, bFileNo, pOffset, pTxData, pTxDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_GetValue(void * pDataParams, uint8_t bOption, uint8_t bFileNo, uint8_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetValue");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pValue,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetValue((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, bFileNo, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_Credit(void * pDataParams, uint8_t bOption, uint8_t bFileNo, uint8_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_Credit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pValue,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_Credit((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, bFileNo, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;

}

phStatus_t phalMfdfLight_Debit(void * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_Debit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pValue,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_Debit((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommOption, bFileNo, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_LimitedCredit(void * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_LimitedCredit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pValue,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_LimitedCredit((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommOption, bFileNo, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_ReadRecords(void * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo, uint8_t * pRecCount,
    uint8_t * pRecSize, uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ReadRecords");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIns);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecCount);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxdataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIns_log, &bIns);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecNo_log, pRecNo, 3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecCount_log, pRecCount,3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecSize_log, pRecSize,3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRecNo,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRecCount,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (ppRxdata,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRxdataLen,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRecSize,PH_COMP_AL_MFDFLIGHT);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ReadRecords((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommOption, bIns, bFileNo, pRecNo, pRecCount, pRecSize,
            ppRxdata, pRxdataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if ((status == PH_ERR_SUCCESS) && ((*pRxdataLen) != 0))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,ppRxdata_log, *ppRxdata, (*pRxdataLen));
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pRxdataLen_log, pRxdataLen);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_WriteRecord(void * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pData,
    uint8_t * pDataLen)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_WriteRecord");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIns);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIns_log, &bIns);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset,3);

    if(pDataLen != NULL)
    {
         wDataLen = (uint16_t)pDataLen[1];
        wDataLen = wDataLen << 8;
        wDataLen |= pDataLen[0];

        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, wDataLen);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDataLen_log, pDataLen, 3);
    }
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pOffset,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pData,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pDataLen,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_WriteRecord((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommOption, bIns, bFileNo, pOffset, pData, pDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_UpdateRecord(void * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo, uint8_t * pOffset,
    uint8_t * pData, uint8_t * pDataLen)
{
    phStatus_t PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_UpdateRecord");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIns);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIns_log, &bIns);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    if(pRecNo != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecNo_log, pRecNo,3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset,3);

    if(pDataLen != NULL)
    {
        wDataLen = (uint16_t)pDataLen[1];
        wDataLen = wDataLen << 8;
        wDataLen |= pDataLen[0];

        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, wDataLen);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDataLen_log, pDataLen, 3);
    }
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pOffset,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pData,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pDataLen,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pRecNo,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pOffset,PH_COMP_AL_MFDFLIGHT);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_UpdateRecord((phalMfdfLight_Sw_DataParams_t *)pDataParams, bCommOption, bIns, bFileNo, pRecNo, pOffset,
            pData, pDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_ClearRecordFile(void * pDataParams, uint8_t bFileNo)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ClearRecordFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ClearRecordFile((phalMfdfLight_Sw_DataParams_t *)pDataParams, bFileNo);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light Transaction Management commands. ------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_CommitTransaction(void * pDataParams, uint8_t bOption, uint8_t * pTMC, uint8_t * pTMV)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_CommitTransaction");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMV);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    if(bOption & 0x01)
    {
        PH_ASSERT_NULL_PARAM (pTMC, PH_COMP_AL_MFDFLIGHT);
        PH_ASSERT_NULL_PARAM (pTMV, PH_COMP_AL_MFDFLIGHT);
    }

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_CommitTransaction((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, pTMC, pTMV);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if (bOption & 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMC_log, pTMC, 4);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMV_log, pTMV, 8);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_AbortTransaction(void * pDataParams)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_AbortTransaction");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_AbortTransaction((phalMfdfLight_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_CommitReaderID(void * pDataParams, uint8_t * pTMRI, uint8_t * pEncTMRI)

{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_CommitReaderID");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMRI);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncTMRI);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMRI_log, pTMRI, 16);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pTMRI,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_CommitReaderID((phalMfdfLight_Sw_DataParams_t *)pDataParams, pTMRI, pEncTMRI);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if(pEncTMRI != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncTMRI_log, pEncTMRI, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light ISO7816-4 commands. -------------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_IsoSelectFile(void * pDataParams, uint8_t bOption, uint8_t bSelector, uint8_t * pFid, uint8_t * pDFname, uint8_t bDFnameLen,
    uint8_t bExtendedLenApdu, uint8_t **ppFCI, uint16_t *pwFCILen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_IsoSelectFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSelector);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFname);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDFnameLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSelector_log, &bSelector);
    if(pFid != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid,2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDFnameLen_log, &bDFnameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFname_log, pDFname,(uint16_t)bDFnameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_IsoSelectFile((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOption, bSelector, pFid, pDFname, bDFnameLen, bExtendedLenApdu,
            ppFCI, pwFCILen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_IsoReadBinary(void * pDataParams, uint16_t wOption, uint8_t bOffset, uint8_t bSfid, uint32_t dwBytesToRead, uint8_t bExtendedLenApdu,
    uint8_t ** ppRxBuffer, uint32_t * pBytesRead)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_IsoReadBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwBytesToRead);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBytesRead);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, dwBytesToRead_log, &dwBytesToRead);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (ppRxBuffer,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pBytesRead,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_IsoReadBinary((phalMfdfLight_Sw_DataParams_t *)pDataParams, wOption, bOffset, bSfid, dwBytesToRead, bExtendedLenApdu,
            ppRxBuffer, pBytesRead);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, (uint16_t)(*pBytesRead));
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_INFO, pBytesRead_log, pBytesRead);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_IsoUpdateBinary(void * pDataParams, uint8_t bOffset, uint8_t bSfid, uint8_t bExtendedLenApdu, uint8_t * pData, uint32_t dwDataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_IsoUpdateBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData,(uint16_t)dwDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_INFO, dwDataLen_log, &dwDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pData,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_IsoUpdateBinary((phalMfdfLight_Sw_DataParams_t *)pDataParams, bOffset, bSfid, bExtendedLenApdu, pData, dwDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light OriginalityCheck commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_ReadSign(void * pDataParams, uint8_t bAddr, uint8_t ** pSignature)
{
    phStatus_t PH_MEMLOC_REM status = 0;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ReadSign");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSignature);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddr_log, &bAddr);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pSignature,PH_COMP_AL_MFDFLIGHT);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return  PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ReadSign((phalMfdfLight_Sw_DataParams_t *)pDataParams, bAddr, pSignature);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSignature_log, *pSignature, 56);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/* MIFARE DESFire Light Miscellaneous commands. ---------------------------------------------------------------------------------------- */
phStatus_t phalMfdfLight_GetConfig(void * pDataParams, uint16_t wConfig, uint16_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pValue,PH_COMP_AL_MFDFLIGHT);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_GetConfig((phalMfdfLight_Sw_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
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

phStatus_t phalMfdfLight_SetConfig(void * pDataParams, uint16_t wConfig, uint16_t wValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_SetConfig((phalMfdfLight_Sw_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_ResetAuthentication(void * pDataParams)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_ResetAuthentication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_ResetAuthentication((phalMfdfLight_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_CalculateTMV(void * pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint16_t wRamKeyNo, uint16_t wRamKeyVer, uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid,
    uint8_t bUidLen, uint8_t * pTMI,  uint32_t dwTMILen, uint8_t * pTMV)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_CalculateTMV");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNoTMACKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVerTMACKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivInputLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUidLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMI);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wTMILen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMV);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNoTMACKey_log, &wKeyNoTMACKey);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVerTMACKey_log, &wKeyVerTMACKey);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyNo_log, &wRamKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyVer_log, &wRamKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivInputLen_log, &bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUidLen_log, &bUidLen);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, wTMILen_log, &dwTMILen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMC_log, pTMC, 4);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bUidLen);
   PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMI_log, pTMI, (uint16_t)dwTMILen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    if (bDivInputLen) PH_ASSERT_NULL_PARAM(pDivInput,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM(pTMC,PH_COMP_AL_MFDFLIGHT);
    if (bUidLen) PH_ASSERT_NULL_PARAM(pUid,PH_COMP_AL_MFDFLIGHT);
    if (dwTMILen) PH_ASSERT_NULL_PARAM(pTMI,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM(pTMI,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM(pUid,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_CalculateTMV((phalMfdfLight_Sw_DataParams_t *)pDataParams, wOption, wKeyNoTMACKey, wKeyVerTMACKey, pDivInput, bDivInputLen,
            pTMC, pUid, bUidLen, pTMI, dwTMILen, pTMV);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMV_log, pTMV, 8);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdfLight_DecryptReaderID(void * pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint16_t wRamKeyNo, uint16_t wRamKeyVer, uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid,
    uint8_t bUidLen, uint8_t * pEncTMRI, uint8_t * pTMRIPrev)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdfLight_DecryptReaderID");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNoTMACKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVerTMACKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivInputLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUidLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncTMRI);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTMRIPrev);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNoTMACKey_log, &wKeyNoTMACKey);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVerTMACKey_log, &wKeyVerTMACKey);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyNo_log, &wRamKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyVer_log, &wRamKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivInputLen_log, &bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUidLen_log, &bUidLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivInputLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMC_log, pTMC, 4);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bUidLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncTMRI_log, pEncTMRI, 16);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM (pDataParams, PH_COMP_AL_MFDFLIGHT);
    if (bDivInputLen) PH_ASSERT_NULL_PARAM(pDivInput,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM(pTMC,PH_COMP_AL_MFDFLIGHT);
    if (bUidLen) PH_ASSERT_NULL_PARAM(pUid,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM(pEncTMRI,PH_COMP_AL_MFDFLIGHT);

    /* Parameter Validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDFLIGHT)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    /* Perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
    case PHAL_MFDFLIGHT_SW_ID:
        status = phalMfdfLight_Sw_DecryptReaderID((phalMfdfLight_Sw_DataParams_t *)pDataParams, wOption, wKeyNoTMACKey, wKeyVerTMACKey, pDivInput,
            bDivInputLen, pTMC, pUid, bUidLen, pEncTMRI, pTMRIPrev);
        break;
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */



    default:
        status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTMRIPrev_log, pTMRIPrev, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

#endif /* NXPBUILD__PHAL_MFDFLIGHT */

