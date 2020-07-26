/*
 *                     Copyright 2016-2019, NXP
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
* Generic MIFARE Ntag42XDna Application Component of Reader Library Framework.
* $Author: nxf18805 $
* $Revision: 1791 $ (v06.01.00)
* $Date: 2016-05-10 16:07:13 +0530 (Tue, 10 May 2016) $
*
*/

#include <phalMfNtag42XDna.h>
#include <ph_Status.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
#include "Sw/phalMfNtag42XDna_Sw.h"
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



#ifdef NXPBUILD__PHAL_MFNTAG42XDNA

#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA secure messaging related commands. ----------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_AuthenticateEv2(void *pDataParams, uint8_t bAuthOption, uint16_t wOption, uint16_t wKeyNo, uint16_t wKeyVer, uint8_t bKeyNoCard,
    uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *bPcdCapsIn, uint8_t *bPcdCapsOut, uint8_t *bPdCapsOut)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_AuthenticatEv2");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAuthOption);
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
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAuthOption_log, &bAuthOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVer_log, &wKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPcdCapsIn_log, bPcdCapsIn, bLenPcdCapsIn);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    if(bDivLen) PH_ASSERT_NULL_PARAM(pDivInput, PH_COMP_AL_MFNTAG42XDNA);
    if(bLenPcdCapsIn)  PH_ASSERT_NULL_PARAM(bPcdCapsIn, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_AuthenticateEv2((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bAuthOption, wOption, wKeyNo, wKeyVer,
                bKeyNoCard, pDivInput, bDivLen, bLenPcdCapsIn, bPcdCapsIn, bPcdCapsOut, bPdCapsOut);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if(bAuthOption)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPcdCapsOut_log, bPcdCapsOut, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bPdCapsOut_log, bPdCapsOut, 6);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Memory and Configuration mamangement commands. ----------------------------------------------------------------- */
#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_SetConfiguration(void * pDataParams, uint8_t bOption, uint8_t * pData, uint8_t bDataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_SetConfiguration");
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
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pData, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_SetConfiguration((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, pData, bDataLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_GetVersion(void * pDataParams, uint8_t * pVerInfo)
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t PH_MEMLOC_REM bVersionLength = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVerInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bVersionLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pVerInfo, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetVersion((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, pVerInfo, &bVersionLength);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVerInfo_log, pVerInfo, bVersionLength);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bVersionLength_log, &bVersionLength);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_GetCardUID(void * pDataParams, uint8_t * pUid)
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t PH_MEMLOC_REM bCardUidLength = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetCardUID");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCardUidLength);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pUid, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetCardUID((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, pUid, &bCardUidLength);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
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




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Key mamangement commands. -------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_ChangeKey(void * pDataParams, uint16_t wOption, uint16_t wOldKeyNo, uint16_t wOldKeyVer, uint16_t wNewKeyNo, uint16_t wNewKeyVer,
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

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ChangeKey");
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
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyNo_log, &wOldKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyNo_log, &wNewKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyVer_log, &wOldKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyVer_log, &wNewKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivLen_log, &bDivLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    if(bDivLen) PH_ASSERT_NULL_PARAM(pDivInput, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ChangeKey((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, wOption, wOldKeyNo, wOldKeyVer, wNewKeyNo,
                wNewKeyVer, bKeyNoCard, pDivInput, bDivLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalMfNtag42XDna_GetKeyVersion(void * pDataParams, uint8_t bKeyNo, uint8_t bKeySetNo, uint8_t * pKeyVersion, uint8_t * bRxLen)
{
    /**
    * This command can be issued without valid authentication
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetKeyVersion");
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
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pKeyVersion, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetKeyVersion((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bKeyNo, bKeySetNo, pKeyVersion, bRxLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKeyVersion_log, pKeyVersion, *bRxLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRxLen_log, bRxLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA File mamangement commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_GetFileSettings(void * pDataParams, uint8_t bFileNo, uint8_t * pFSBuffer, uint8_t * bBufferLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFSBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBufferLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pFSBuffer, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(bBufferLen, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetFileSettings((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bFileNo, pFSBuffer, bBufferLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFSBuffer_log, pFSBuffer, (*bBufferLen));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bBufferLen_log, bBufferLen, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_GetFileCounters(void * pDataParams, uint8_t bOption, uint8_t bFileNo, uint8_t * pFileCounters, uint8_t * pRxLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetFileCounters");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFileCounters);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pFileCounters, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pRxLen, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetFileCounters((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, bFileNo, pFileCounters, pRxLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFileCounters_log, pFileCounters, (*pRxLen));
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxLen_log, pRxLen, 1);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_ChangeFileSettings(void * pDataParams, uint8_t bCommMode, uint8_t bFileNo, uint8_t bFileOption, uint8_t *pAccessRights,
    uint8_t bAdditionalInfoLen, uint8_t * bAdditionalInfo)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ChangeFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommMode);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAdditionalInfoLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAdditionalInfo);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommMode_log, &bCommMode);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileOption_log, &bFileOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAdditionalInfoLen_log, &bAdditionalInfoLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bAdditionalInfo_log, bAdditionalInfo, bAdditionalInfoLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pAccessRights, PH_COMP_AL_MFNTAG42XDNA);
    if(bAdditionalInfoLen) PH_ASSERT_NULL_PARAM(bAdditionalInfo, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ChangeFileSettings((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bCommMode, bFileNo, bFileOption, pAccessRights,
                bAdditionalInfoLen, bAdditionalInfo);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_ChangeFileSettingsSDM(void * pDataParams, uint8_t bOption, uint8_t bFileNo, uint8_t bFileOption, uint8_t * pAccessRights,
    uint8_t bSdmOptions, uint8_t * pSdmAccessRights, uint8_t * pVCUIDOffset, uint8_t * pSDMReadCtrOffset, uint8_t * pPICCDataOffset,
    uint8_t * pTTPermStatusOffset, uint8_t * pSDMMACInputOffset, uint8_t * pSDMENCOffset, uint8_t * pSDMENCLen, uint8_t * pSDMMACOffset,
    uint8_t * pSDMReadCtrLimit)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ChangeFileSettingsSDM");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSdmOptions);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSdmAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVCUIDOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMReadCtrOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPICCDataOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTTPermStatusOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMMACInputOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMENCOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMENCLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMMACOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMReadCtrLimit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileOption_log, &bFileOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    if(pAccessRights != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSdmOptions_log, &bSdmOptions);
    if(pSdmAccessRights != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSdmAccessRights_log, pSdmAccessRights, 2);
    if(pVCUIDOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVCUIDOffset_log, pVCUIDOffset, 3);
    if(pSDMReadCtrOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMReadCtrOffset_log, pSDMReadCtrOffset, 3);
    if(pPICCDataOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPICCDataOffset_log, pPICCDataOffset, 3);
    if(pTTPermStatusOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTTPermStatusOffset_log, pTTPermStatusOffset, 3);
    if(pSDMMACInputOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMMACInputOffset_log, pSDMMACInputOffset, 3);
    if(pSDMENCOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMENCOffset_log, pSDMENCOffset, 3);
    if(pSDMENCLen != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMENCLen_log, pSDMENCLen, 3);
    if(pSDMMACOffset != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMMACOffset_log, pSDMMACOffset, 3);
    if(pSDMReadCtrLimit != NULL)
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMReadCtrLimit_log, pSDMReadCtrLimit, 3);

    /* Verify the parameters */
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ChangeFileSettingsSDM((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, bFileNo, bFileOption, pAccessRights,
                bSdmOptions, pSdmAccessRights, pVCUIDOffset, pSDMReadCtrOffset, pPICCDataOffset, pTTPermStatusOffset, pSDMMACInputOffset, pSDMENCOffset,
                pSDMENCLen, pSDMMACOffset, pSDMReadCtrLimit);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Data mamangement commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_ReadData(void * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pLength,
    uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ReadData");
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
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pOffset, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pLength, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(ppRxdata, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pRxdataLen, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ReadData((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, bIns, bFileNo, pOffset, pLength,
                ppRxdata, pRxdataLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if((status == PH_ERR_SUCCESS) && ((*pRxdataLen) != 0))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxdata_log, *ppRxdata, (*pRxdataLen));
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pRxdataLen_log, pRxdataLen);
    }
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_WriteData(void * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pTxData,
    uint8_t * pTxDataLen)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_WriteData");
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
        wDataLen = (uint16_t) pTxDataLen[1];
        wDataLen = wDataLen << 8;
        wDataLen |= pTxDataLen[0];

        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxData_log, pTxData, wDataLen);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxDataLen_log, pTxDataLen, 3);
    }
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pOffset, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pTxData, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pTxDataLen, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_WriteData((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, bIns, bFileNo, pOffset,
                pTxData, pTxDataLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}




/* MIFARE NTAG 42xDNA ISO7816-4 commands. -------------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_IsoSelectFile(void * pDataParams, uint8_t bOption, uint8_t bSelector, uint8_t * pFid, uint8_t * pDFname,
    uint8_t bDFnameLen, uint8_t bExtendedLenApdu, uint8_t **ppFCI, uint16_t *pwFCILen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_IsoSelectFile");
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
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid, 2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDFnameLen_log, &bDFnameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFname_log, pDFname, (uint16_t) bDFnameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_IsoSelectFile((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOption, bSelector, pFid, pDFname, bDFnameLen,
                bExtendedLenApdu, ppFCI, pwFCILen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_IsoReadBinary(void * pDataParams, uint16_t wOption, uint8_t bOffset, uint8_t bSfid, uint32_t dwBytesToRead,
    uint8_t bExtendedLenApdu, uint8_t ** ppRxBuffer, uint32_t * pBytesRead)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_IsoReadBinary");
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
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(ppRxBuffer, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pBytesRead, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_IsoReadBinary((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, wOption, bOffset, bSfid, dwBytesToRead,
                bExtendedLenApdu, ppRxBuffer, pBytesRead);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, (uint16_t) (*pBytesRead));
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_INFO, pBytesRead_log, pBytesRead);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_IsoUpdateBinary(void * pDataParams, uint8_t bOffset, uint8_t bSfid, uint8_t bExtendedLenApdu, uint8_t * pData,
    uint32_t dwDataLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_IsoUpdateBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, (uint16_t) dwDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_INFO, dwDataLen_log, &dwDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pData, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_IsoUpdateBinary((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bOffset, bSfid, bExtendedLenApdu, pData,
                dwDataLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}




/* MIFARE NTAG 42xDNA Originality Check functions. ----------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_ReadSign(void * pDataParams, uint8_t bAddr, uint8_t ** pSignature)
{
    phStatus_t PH_MEMLOC_REM status = 0;
    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ReadSign");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSignature);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddr_log, &bAddr);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pSignature, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return  PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ReadSign((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bAddr, pSignature);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSignature_log, *pSignature, 56);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Tag Tamper Protection functions. ------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_GetTagTamperStatus(void * pDataParams, uint8_t * pRxBuffer, uint8_t * pRxLen)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetTagTamperStatus");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetTagTamperStatus((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, pRxBuffer, pRxLen);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxBuffer_log, pRxBuffer, (*pRxLen));
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxLen_log, pRxLen, 1);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Miscellaneous functions. --------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_GetConfig(void * pDataParams, uint16_t wConfig, uint16_t * pValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pValue, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_GetConfig((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, wConfig, pValue);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_SetConfig(void * pDataParams, uint16_t wConfig, uint16_t wValue)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_SetConfig((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, wConfig, wValue);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_ResetAuthentication(void * pDataParams)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_ResetAuthentication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_ResetAuthentication((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_CalculateMACSDM(void * pDataParams, uint8_t bSdmOption, uint16_t wSDMMacKeyNo, uint16_t wSDMMacKeyVer,
    uint16_t wRamKeyNo, uint16_t wRamKeyVer, uint8_t * pUid, uint8_t bUidLen, uint8_t * pSDMReadCtr, uint8_t * pInData,
    uint16_t wInDataLen, uint8_t * pRespMac)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_CalculateMACSDM");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSdmOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wSDMMacKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wSDMMacKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUidLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMReadCtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pInData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wInDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRespMac);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSdmOption_log, &bSdmOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wSDMMacKeyNo_log, &wSDMMacKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wSDMMacKeyVer_log, &wSDMMacKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyNo_log, &wRamKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyVer_log, &wRamKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUidLen_log, &bUidLen);
    if(pSDMReadCtr != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMReadCtr_log, pSDMReadCtr, 3);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wInDataLen_log, &wInDataLen);
    if(pInData != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pInData_log, pInData, wInDataLen);
    if(pUid != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bUidLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pInData, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_CalculateMACSDM((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bSdmOption, wSDMMacKeyNo, wSDMMacKeyVer, pUid,
                bUidLen, pSDMReadCtr, pInData, wInDataLen, pRespMac);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRespMac_log, pRespMac, 8);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_DecryptSDMENCFileData(void * pDataParams, uint8_t bSdmOption, uint16_t wEncKeyNo, uint16_t wEncKeyVer,
    uint16_t wRamKeyNo, uint16_t wRamKeyVer, uint8_t * pUid, uint8_t bUidLen, uint8_t * pSDMReadCtr, uint8_t * pEncdata,
    uint16_t wEncDataLen, uint8_t * pPlainData)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_DecryptSDMENCFileData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSdmOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wEncKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wEncKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRamKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUidLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSDMReadCtr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wEncDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPlainData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSdmOption_log, &bSdmOption);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wEncKeyNo_log, &wEncKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wEncKeyVer_log, &wEncKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyNo_log, &wRamKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRamKeyVer_log, &wRamKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUidLen_log, &bUidLen);
    if(pUid != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bUidLen);
    if(pSDMReadCtr != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSDMReadCtr_log, pSDMReadCtr, 3);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wEncDataLen_log, &wEncDataLen);
    if(pEncdata != NULL) PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncdata_log, pEncdata, wEncDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Parameter Validation */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pEncdata, PH_COMP_AL_MFNTAG42XDNA);

    /* Perform operation on active layer */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_DecryptSDMENCFileData((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, bSdmOption, wEncKeyNo, wEncKeyVer, pUid,
                bUidLen, pSDMReadCtr, pEncdata, wEncDataLen, pPlainData);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPlainData_log, pPlainData, wEncDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfNtag42XDna_DecryptSDMPICCData(void * pDataParams, uint16_t wKeyNo, uint16_t wKeyVer, uint8_t * pEncdata, uint16_t wEncDataLen,
    uint8_t * pPlainData)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfNtag42XDna_DecryptSDMPICCData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pEncdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wEncDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPlainData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVer_log, &wKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wEncDataLen_log, &wEncDataLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pEncdata_log, pEncdata, wEncDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    /* Verify the parameters */
    PH_ASSERT_NULL_DATA_PARAM(pDataParams, PH_COMP_AL_MFNTAG42XDNA);
    PH_ASSERT_NULL_PARAM(pEncdata, PH_COMP_AL_MFNTAG42XDNA);

    /* Parameter Validation */
    if(PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFNTAG42XDNA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Perform operation on active layer */
    switch(PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
        case PHAL_MFNTAG42XDNA_SW_ID:
            status = phalMfNtag42XDna_Sw_DecryptSDMPICCData((phalMfNtag42XDna_Sw_DataParams_t *) pDataParams, wKeyNo, wKeyVer, pEncdata, wEncDataLen,
                pPlainData);
            break;
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */



        default:
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPlainData_log, pPlainData, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

#endif /* NXPBUILD__PHAL_MFNTAG42XDNA */
