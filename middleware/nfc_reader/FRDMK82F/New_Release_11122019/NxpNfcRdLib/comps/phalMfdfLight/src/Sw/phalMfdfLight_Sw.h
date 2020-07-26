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
* Software MIFARE DESFire Light Application Component of Reader Library Framework.
* $Author: nxp79567 $
* $Revision: 1546 $ (v06.01.00)
* $Date: 2016-02-03 10:32:15 +0530 (Wed, 03 Feb 2016) $
*/

#ifndef PHALMFDFLIGHT_SW_H
#define PHALMFDFLIGHT_SW_H

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_AuthenticateEv2(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFirstAuth, uint16_t wOption, uint16_t wKeyNo, uint16_t wKeyVer,
    uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn, uint8_t *pPcdCapsOut, uint8_t *pPdCapsOut);




phStatus_t phalMfdfLight_Sw_SetConfiguration(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * pData, uint8_t bDataLen);

phStatus_t phalMfdfLight_Sw_GetCardUID(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pUid, uint8_t * pUidLength);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_Sw_GetVersion(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pVerInfo, uint8_t * pVerLength);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_ChangeKey(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wOption, uint16_t wOldKeyNo, uint16_t wOldKeyVer, uint16_t wNewKeyNo,
    uint16_t wNewKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen);

phStatus_t phalMfdfLight_Sw_GetKeyVersion(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bKeyNo, uint8_t bKeySetNo, uint8_t * pKeyVersion, uint8_t * bRxLen);




phStatus_t phalMfdfLight_Sw_CreateTransactionMacFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo, uint8_t bCommMode, uint8_t * pAccessRights,
    uint8_t bKeyType, uint8_t * bTMKey, uint8_t bTMKeyVer);

phStatus_t phalMfdfLight_Sw_ChangeFileSettings(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommMode, uint8_t bFileNo, uint8_t bFileOption,
    uint8_t * pAccessRights, uint8_t bAdditionalInfoLen, uint8_t * bAdditionalInfo);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_Sw_DeleteFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo);

phStatus_t phalMfdfLight_Sw_GetFileIDs(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pFid, uint8_t * bNumFid);

phStatus_t phalMfdfLight_Sw_GetISOFileIDs(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pFidBuffer, uint8_t * bNumFid);

phStatus_t phalMfdfLight_Sw_GetFileSettings(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo, uint8_t * pFSBuffer, uint8_t * bBufferLen);

phStatus_t phalMfdfLight_Sw_ReadData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pLength,
    uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfdfLight_Sw_WriteData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset,
    uint8_t * pTxData, uint8_t * pTxDataLen);

phStatus_t phalMfdfLight_Sw_GetValue(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bFileNo, uint8_t * pValue);

phStatus_t phalMfdfLight_Sw_Credit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue);

phStatus_t phalMfdfLight_Sw_Debit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue);

phStatus_t phalMfdfLight_Sw_LimitedCredit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue);

phStatus_t phalMfdfLight_Sw_ReadRecords(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo,
    uint8_t * pRecCount, uint8_t * pRecSize, uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfdfLight_Sw_WriteRecord(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset,
    uint8_t * pData, uint8_t * pDataLen);

phStatus_t phalMfdfLight_Sw_UpdateRecord(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo,
    uint8_t * pOffset, uint8_t * pData, uint8_t * pDataLen);

phStatus_t phalMfdfLight_Sw_ClearRecordFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo);

phStatus_t phalMfdfLight_Sw_CommitTransaction(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * pTMC, uint8_t * pTMAC);

phStatus_t phalMfdfLight_Sw_AbortTransaction(phalMfdfLight_Sw_DataParams_t * pDataParams);

phStatus_t phalMfdfLight_Sw_CommitReaderID(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pTMRI, uint8_t * pEncTMRI);

phStatus_t phalMfdfLight_Sw_IsoSelectFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bSelector, uint8_t * pFid, uint8_t * pDFname,
    uint8_t bDFnameLen, uint8_t bExtendedLenApdu, uint8_t ** ppFCI, uint16_t * pwFCILen);

phStatus_t phalMfdfLight_Sw_IsoReadBinary(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t bOffset, uint8_t bSfid, uint32_t dwBytesToRead,
    uint8_t bExtendedLenApdu, uint8_t ** ppRxBuffer, uint32_t * pBytesRead);

phStatus_t phalMfdfLight_Sw_IsoUpdateBinary(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOffset, uint8_t bSfid, uint8_t bLcLen, uint8_t * pData,
    uint32_t dwDataLen);

phStatus_t phalMfdfLight_Sw_ReadSign(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bAddr, uint8_t ** pSignature);

phStatus_t phalMfdfLight_Sw_GetConfig(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wConfig, uint16_t * pValue);

phStatus_t phalMfdfLight_Sw_SetConfig(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wConfig, uint16_t wValue);

phStatus_t phalMfdfLight_Sw_ResetAuthentication(phalMfdfLight_Sw_DataParams_t * pDataParams);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_CalculateTMV(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid, uint8_t bUidLen, uint8_t * pTMI, uint32_t dwTMILen, uint8_t * pTMV);

phStatus_t phalMfdfLight_Sw_DecryptReaderID(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid, uint8_t bUidLen, uint8_t * pEncTMRI, uint8_t * pTMRIPrev);
#endif /* NXPBUILD__PH_CRYPTOSYM */

#endif /* PHALMFDFLIGHT_SW_H */
