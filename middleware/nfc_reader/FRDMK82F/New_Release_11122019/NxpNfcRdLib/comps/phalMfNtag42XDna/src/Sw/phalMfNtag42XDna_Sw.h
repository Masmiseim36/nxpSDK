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

#ifndef PHALMFNTAG42XDNA_SW_H
#define PHALMFNTAG42XDNA_SW_H

#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA secure messaging related commands. ----------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_AuthenticateEv2(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bAuthOption, uint16_t wOption,
    uint16_t wKeyNo, uint16_t wKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn,
    uint8_t *pPcdCapsOut, uint8_t *pPdCapsOut);
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Memory and Configuration mamangement commands. ----------------------------------------------------------------- */
#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_SetConfiguration(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * pData,
    uint8_t bDataLen);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_GetVersion(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pVerInfo, uint8_t * pVerLength);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_GetCardUID(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pUid, uint8_t * pUidLength);
#endif /* NXPBUILD__PH_CRYPTOSYM */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Key mamangement commands. -------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_ChangeKey(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wOption, uint16_t wOldKeyNo,
    uint16_t wOldKeyVer, uint16_t wNewKeyNo, uint16_t wNewKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen);

phStatus_t phalMfNtag42XDna_Sw_GetKeyVersion(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bKeyNo, uint8_t bKeySetNo,
    uint8_t * pKeyVersion, uint8_t * bRxLen);
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA File mamangement commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_GetFileCounters(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bFileNo,
    uint8_t * pFileCounters, uint8_t * pRxLen);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_GetFileSettings(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bFileNo, uint8_t * pFSBuffer,
    uint8_t * bBufferLen);

phStatus_t phalMfNtag42XDna_Sw_ChangeFileSettings(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bCommMode, uint8_t bFileNo,
    uint8_t bFileOption, uint8_t * pAccessRights, uint8_t bAdditionalInfoLen, uint8_t * bAdditionalInfo);

phStatus_t phalMfNtag42XDna_Sw_ChangeFileSettingsSDM(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bFileNo,
    uint8_t bFileOption, uint8_t *pAccessRights, uint8_t bSdmOptions, uint8_t *pSdmAccessRights, uint8_t *pVCUIDOffset,
    uint8_t *pSDMReadCtrOffset, uint8_t *pPICCDataOffset, uint8_t *pTTPermStatusOffset, uint8_t *pSDMMACInputOffset,
    uint8_t *pSDMENCOffset, uint8_t *pSDMENCLen, uint8_t *pSDMMACOffset, uint8_t *pSDMReadCtrLimit);
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Data mamangement commands. ------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_ReadData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo,
    uint8_t * pOffset, uint8_t * pLength, uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfNtag42XDna_Sw_WriteData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo,
    uint8_t * pOffset, uint8_t * pTxData, uint8_t * pTxDataLen);




/* MIFARE NTAG 42xDNA ISO7816-4 commands. -------------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_IsoSelectFile(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bSelector, uint8_t * pFid,
    uint8_t * pDFname, uint8_t bDFnameLen, uint8_t  bExtendedLenApdu, uint8_t ** ppFCI, uint16_t * pwFCILen);

phStatus_t phalMfNtag42XDna_Sw_IsoReadBinary(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t bOffset, uint8_t bSfid,
    uint32_t dwBytesToRead, uint8_t bExtendedLenApdu, uint8_t ** ppRxBuffer, uint32_t * pBytesRead);

phStatus_t phalMfNtag42XDna_Sw_IsoUpdateBinary(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOffset, uint8_t bSfid, uint8_t bLcLen,
    uint8_t * pData, uint32_t dwDataLen);




/* MIFARE NTAG 42xDNA Originality Check functions. ----------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_ReadSign(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bAddr, uint8_t ** pSignature);




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Tag Tamper Protection functions. ------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_GetTagTamperStatus(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pRxBuffer, uint8_t * pRxLen);
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Miscellaneous functions. --------------------------------------------------------------------------------------- */
phStatus_t phalMfNtag42XDna_Sw_GetConfig(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wConfig, uint16_t * pValue);

phStatus_t phalMfNtag42XDna_Sw_SetConfig(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint16_t wConfig, uint16_t wValue);

phStatus_t phalMfNtag42XDna_Sw_ResetAuthentication(phalMfNtag42XDna_Sw_DataParams_t * pDataParams);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_CalculateMACSDM(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bSdmOption, uint16_t wSDMMacKeyNo,
    uint16_t wSDMMacKeyVer, uint8_t * pUid, uint8_t bUidLen, uint8_t * SDMReadCtr, uint8_t * pInData, uint16_t wInDataLen, uint8_t * pRespMac);

phStatus_t phalMfNtag42XDna_Sw_DecryptSDMENCFileData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bSdmOption, uint16_t wEncKeyNo,
    uint16_t wEncKeyVer, uint8_t * pUid, uint8_t bUidLen, uint8_t * pSDMReadCtr, uint8_t * pEncdata, uint16_t wEncDataLen, uint8_t * pPlainData);

phStatus_t phalMfNtag42XDna_Sw_DecryptSDMPICCData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wKeyNo, uint16_t wKeyVer,
    uint8_t * pIndata, uint16_t wInDataLen, uint8_t * pPlainData);
#endif /* NXPBUILD__PH_CRYPTOSYM */

#endif /* PHALMFNTAG42XDNA_SW_H */
