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

#ifndef PHALMFNTAG42XDNA_SW_INT_H
#define PHALMFNTAG42XDNA_SW_INT_H

/* Macros for LRP usage */
#define PHAL_MFNTAG42XDNA_AUTHENTICATE_LRP_FIRST_RESP_LEN   17U     /* 16 Bytes of Random number + 1 byte of Auth mode */

phStatus_t phalMfNtag42XDna_Sw_Int_GetData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen,
    uint8_t ** pResponse, uint16_t * pRxlen);

phStatus_t phalMfNtag42XDna_Sw_Int_ISOGetData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen,
    uint8_t ** pResponse, uint16_t * pRxlen);

phStatus_t phalMfNtag42XDna_Sw_Int_ReadData_Plain(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t ** ppRxdata, uint16_t * pRxdataLen);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_ReadData_Enc(phalMfNtag42XDna_Sw_DataParams_t * UNALIGNED pDataParams, uint8_t bPaddingOption,
    uint8_t * bCmdBuff, uint16_t wCmdLen, uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfNtag42XDna_Sw_Int_Write_Enc(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bPaddingMode, uint8_t bCommOption, uint8_t * pData, uint16_t wDataLen);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_Write_Plain(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bCommOption, uint8_t * pData, uint16_t  wDataLen);

void phalMfNtag42XDna_Sw_Int_ResetAuthStatus(phalMfNtag42XDna_Sw_DataParams_t * pDataParams);

phStatus_t phalMfNtag42XDna_Sw_Int_SendDataToPICC(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t bCommOption,
    uint8_t * pCmd, uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * bLastChunk, uint16_t wLastChunkLen, uint8_t * pResp,
    uint16_t * pRespLen);

phStatus_t phalMfNtag42XDna_Sw_Int_SendDataAndAddDataToPICC(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * pCmd,
    uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * pAddData, uint16_t wAddDataLen, uint8_t * pResp, uint16_t * pRespLen);

phStatus_t phalMfNtag42XDna_Sw_Int_IsoRead(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t ** ppRxBuffer, uint32_t * pBytesRead);

void phalMfNtag42XDna_Sw_Int_TruncateMac(uint8_t * pMac);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_ComputeIv(uint8_t bIsResponse, uint8_t * pTi, uint16_t wCmdCtr, uint8_t * pIv);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_GetFrameLength(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t * pFSD, uint16_t * pFSC);

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_AuthenticateEv2(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bAuthOption, uint16_t wOption,
    uint16_t wKeyNo, uint16_t wKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn,
    uint8_t *pPcdCapsOut, uint8_t *pPdCapsOut);

#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_ComputeSDMIV(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t *pSessEncKey, uint8_t *pSDMReadCtr,
    uint8_t *pIV);

phStatus_t phalMfNtag42XDna_Sw_Int_DecryptSDMData(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t *pSessEncKey, uint8_t *pIv,
    uint8_t wIvSize, uint8_t *pInputOutputData, uint16_t wInputDataLen);

phStatus_t phalMfNtag42XDna_Sw_Int_GenerateSDMSessionKeysAES(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t bSdmOption, uint16_t wKeyNo,
    uint16_t wKeyVer, uint8_t *pSDMReadCtr, uint8_t * pVCUID, uint8_t bUidLen, uint8_t *pSessEncKey, uint8_t *pSessMacKey);


#endif /* PHALMFNTAG42XDNA_SW_INT_H */
