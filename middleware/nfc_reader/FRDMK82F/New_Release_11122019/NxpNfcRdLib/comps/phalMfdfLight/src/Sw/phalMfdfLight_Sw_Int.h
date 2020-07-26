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
* Internal header file used by software MIFARE DESFire Light Application Component
* of Reader Library Framework.
* $Author: nxp79567 $
* $Revision: 1546 $ (v06.01.00)
* $Date: 2016-02-03 10:32:15 +0530 (Wed, 03 Feb 2016) $
*/

#ifndef PHALMFDFLIGHT_SW_INT_H
#define PHALMFDFLIGHT_SW_INT_H

/* Macros for LRP usage */
#define PHAL_MFDFLIGHT_AUTHENTICATE_LRP_FIRST_RESP_LEN  17U     /* 16 Bytes of Random number + 1 byte of Auth mode */

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_Int_AuthenticateEv2(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bAuthOption, uint16_t wOption, uint16_t wKeyNo,
    uint16_t wKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn, uint8_t *pPcdCapsOut,
    uint8_t *pPdCapsOut);


phStatus_t phalMfdfLight_Sw_Int_ReadData_Enc(phalMfdfLight_Sw_DataParams_t * UNALIGNED pDataParams, uint8_t bPaddingOption,  /*  padding method 1 or 2 */
    uint8_t * bCmdBuff, uint16_t wCmdLen, uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfdfLight_Sw_Int_Write_Enc(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bIns, /* ISO 14443-4 Chaining */ uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bPaddingMode,  /*  Either PH_CRYPTOSYM_PADDING_MODE_1 or PH_CRYPTOSYM_PADDING_MODE_2 */ uint8_t bCommOption,
    uint8_t * pData, uint16_t wDataLen);

phStatus_t phalMfdfLight_Sw_Int_ComputeIv(uint8_t bIsResponse, uint8_t * pTi, uint16_t wCmdCtr, uint8_t * pIv);
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_Sw_Int_GetData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen, uint8_t ** pResponse,
    uint16_t * pRxlen);

phStatus_t phalMfdfLight_Sw_Int_ISOGetData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen, uint8_t ** pResponse,
    uint16_t * pRxlen);

phStatus_t phalMfdfLight_Sw_Int_ReadData_Plain(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * bCmdBuff, uint16_t wCmdLen,
    uint8_t ** ppRxdata, uint16_t * pRxdataLen);

phStatus_t phalMfdfLight_Sw_Int_Write_Plain(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bIns, /* ISO 14443-4 Chaining */ uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bCommOption, uint8_t * pData, uint16_t  wDataLen);

void phalMfdfLight_Sw_Int_ResetAuthStatus(phalMfdfLight_Sw_DataParams_t * pDataParams);

phStatus_t phalMfdfLight_Sw_Int_SendDataToPICC(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bIns, /* ISO 14443-4 Chaining */ uint8_t bCommOption,
    uint8_t * pCmd, uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * bLastChunk, uint16_t wLastChunkLen,  uint8_t * pResp,
    uint16_t * pRespLen);

phStatus_t phalMfdfLight_Sw_Int_SendDataAndAddDataToPICC(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * pCmd, uint16_t wCmdLen,
    uint8_t * pData, uint16_t wDataLen, uint8_t * pAddData, uint16_t wAddDataLen, uint8_t * pResp, uint16_t * pRespLen);

phStatus_t phalMfdfLight_Sw_Int_IsoRead(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t * bCmdBuff, uint16_t wCmdLen,
    uint8_t ** ppRxBuffer, uint32_t * pBytesRead);

void phalMfdfLight_Sw_Int_TruncateMac(uint8_t * pMac);

phStatus_t phalMfdfLight_Sw_Int_GetFrameLength(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t * pFSD, uint16_t * pFSC);

phStatus_t phalMfdfLight_Sw_Int_buildLrpSV(uint8_t *pRndA, uint8_t *pRndB, uint8_t *pSV);

#endif /* PHALMFDFLIGHT_SW_INT_H */
