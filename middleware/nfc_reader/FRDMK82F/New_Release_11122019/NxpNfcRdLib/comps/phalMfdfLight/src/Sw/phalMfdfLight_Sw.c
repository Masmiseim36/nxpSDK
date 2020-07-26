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

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <string.h>
#include <phTools.h>
#include <phKeyStore.h>
#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */
#ifdef NXPBUILD__PH_CRYPTORNG
#include <phCryptoRng.h>
#endif /* NXPBUILD__PH_CRYPTORNG */
#ifdef NXPBUILD__PH_TMIUTILS
#include <phTMIUtils.h>
#endif /* NXPBUILD__PH_TMIUTILS */
#ifdef NXPBUILD__PHAL_VCA
#include <phalVca.h>
#endif /* NXPBUILD__PHAL_VCA */

#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW

#include "../phalMfdfLight_Int.h"
#include "phalMfdfLight_Sw.h"
#include "phalMfdfLight_Sw_Int.h"

phStatus_t phalMfdfLight_Sw_Init(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wSizeOfDataParams, void * pPalMifareDataParams, void * pKeyStoreDataParams,
    void * pCryptoDataParamsEnc, void * pCryptoDataParamsMac, void * pCryptoRngDataParams, void * pTMIDataParams, void * pHalDataParams)
{
    /* data param check */
    if (sizeof(phalMfdfLight_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDFLIGHT);
    }

    PH_ASSERT_NULL_DATA_PARAM(pDataParams,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pPalMifareDataParams,PH_COMP_AL_MFDFLIGHT);
#ifdef NXPBUILD__PH_CRYPTOSYM
    PH_ASSERT_NULL_PARAM (pKeyStoreDataParams,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pCryptoDataParamsEnc,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pCryptoDataParamsMac,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pCryptoRngDataParams,PH_COMP_AL_MFDFLIGHT);
#endif /* NXPBUILD__PH_CRYPTOSYM */
    PH_ASSERT_NULL_PARAM (pTMIDataParams,PH_COMP_AL_MFDFLIGHT);
    PH_ASSERT_NULL_PARAM (pHalDataParams,PH_COMP_AL_MFDFLIGHT);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFDFLIGHT | PHAL_MFDFLIGHT_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;
    pDataParams->pCryptoDataParamsEnc   = pCryptoDataParamsEnc;
    pDataParams->pCryptoDataParamsMac   = pCryptoDataParamsMac;
    pDataParams->pCryptoRngDataParams   = pCryptoRngDataParams;
    pDataParams->pTMIDataParams         = pTMIDataParams;
    pDataParams->pHalDataParams         = pHalDataParams;
    /* 2 Byte CRC initial value in Authenticate mode. */
    pDataParams->wCrc = PH_TOOLS_CRC16_PRESET_ISO14443A;

    /* 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    pDataParams->dwCrc = PH_TOOLS_CRC32_PRESET_DF8;

    memset(pDataParams->bSesAuthENCKey, 0x00, 24);  /* PRQA S 3200 */
    pDataParams->bKeyNo = 0xFF; /* Set to invalid */
    memset(pDataParams->bIv, 0x00, 16); /* PRQA S 3200 */
    memset(pDataParams->pAid, 0x00, 3);  /* PRQA S 3200 */
    pDataParams->bAuthMode = PHAL_MFDFLIGHT_NOT_AUTHENTICATED; /* Set to invalid */
    pDataParams->bWrappedMode = 0x01; /* Set to true */
    pDataParams->bCryptoMethod = 0xFF; /* No crypto just after init */
    pDataParams->wAdditionalInfo = 0x0000;
    pDataParams->bShortLenApdu = 0x01; /* Since Prime variant supports only Short Length APDU, setting this to 0x01 always */
    pDataParams->dwPayLoadLen = 0;
    pDataParams->wCmdCtr = 0;
    memset(pDataParams->bTi, 0x00, PHAL_MFDFLIGHT_SIZE_TI);  /* PRQA S 3200 */
    memset(pDataParams->bSesAuthMACKey, 0x00, 16);  /* PRQA S 3200 */
    memset(pDataParams->pUnprocByteBuff, 0x00, PHAL_MFDFLIGHT_SIZE_MAC);  /* PRQA S 3200 */
    pDataParams->bNoUnprocBytes = 0;
    memset(pDataParams->bLastBlockBuffer, 0x00, 16);  /* PRQA S 3200 */
    pDataParams->bLastBlockIndex = 0;

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_AuthenticateEv2(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bAuthOption, uint16_t wOption, uint16_t wKeyNo, uint16_t wKeyVer,
    uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn, uint8_t *pPcdCapsOut, uint8_t *pPdCapsOut)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Decide whether the Auth option is Non LRP or Regular EV2 */
    if((bAuthOption == PHAL_MFDFLIGHT_AUTHFIRST_NON_LRP) || (bAuthOption == PHAL_MFDFLIGHT_AUTHNONFIRST_NON_LRP))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_AuthenticateEv2(
            pDataParams,
            bAuthOption,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen,
            bLenPcdCapsIn,
            pPcdCapsIn,
            pPcdCapsOut,
            pPdCapsOut
            ));
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_SetConfiguration(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * pData, uint8_t bDataLen)
{
    uint8_t  PH_MEMLOC_REM bCmdBuff[8];
    uint16_t PH_MEMLOC_REM wCmdLen = 0;
    uint8_t  PH_MEMLOC_REM bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_1;
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM bCryptoMethodBkup =  0xFF;
    uint8_t PH_MEMLOC_REM bEnableLrp = 0x00;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_SET_CONFIG;
    bCmdBuff[wCmdLen++] = bOption;
    switch(bOption)
    {
        /* Data = 1B configuration data */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION0:
        /* Data =  KEY || 1BYTE KEY VERSION    Key data is 25 bytes */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION1:
        /* User defined SAK */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION3:
        /* Secure Messaging Configuration */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION4:
        /* Capability data, consisting of VCTID Override, PDCap1 and PDCap2 */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION5:
        /* Virtual Card Installation Identifier(VCIID) or application ISODFName */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION6:
        /*File Renaming*/
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION8:
        /*File configuration for File type ValueFile*/
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION9:
        break;
        /*Failed Authentication configuration */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION10:
        break;
        /*Hardware configuration*/
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION11:
        break;

        /* User defined ATS */
    case PHAL_MFDFLIGHT_SET_CONFIG_OPTION2:
        bPaddingMethod = PH_CRYPTOSYM_PADDING_MODE_2;
        break;

    default:
        /* Do not check for Invalid parameter here. */
        break;
    }

    /* In case of Option-5, where LRP to be enabled(i.e pData[4] = 01), Data is Sent in FULL and the response is expected in PLAIN.
    * Hence the followiung interface should know that it should not process the Response data in FULL mode. Hence
    * using the below flag variable */
    if((bOption == PHAL_MFDFLIGHT_SET_CONFIG_OPTION5) && (pData[4] == 0x02))
    {
        bCryptoMethodBkup = pDataParams->bCryptoMethod;
        pDataParams->bCryptoMethod = PH_CRYPTOSYM_CONFIG_LRP; /*  This is just a value. Same to be checked inside phalMfdfLight_Sw_Int_Write_Enc API */
        bEnableLrp = 0x01;
    }

    statusTmp =  phalMfdfLight_Sw_Int_Write_Enc(
        pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        wCmdLen,
        bPaddingMethod,
        0x00,
        pData,
        (uint16_t)bDataLen
        );

    /* Store back the original Crypto method the pDataParams->bCryptoMethod once the data is complete */
    if(bEnableLrp)
    {
        pDataParams->bCryptoMethod = bCryptoMethodBkup;
    }

    if((statusTmp == PH_ERR_SUCCESS) && (bEnableLrp))
    {
        phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
        bEnableLrp = 0x00;
    }

    return statusTmp;
}

phStatus_t phalMfdfLight_Sw_GetCardUID(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pUid, uint8_t * pUidLength)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_GET_CARD_UID;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Enc(
        pDataParams,
        PHAL_MFDFLIGHT_COMMUNICATION_ENC,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /* Response will be received as
     * 1. 7 byte UID
     * 2. [1 Byte UID Format] + [1 byte UID Length(0x04)] + 4 byte UID
     * 3. [1 Byte UID Format] + [1 byte UID Length(0x0A)] + 10 byte UID
     */
    if ((wRxlen != PHAL_MFDFLIGHT_DEFAULT_UID_LENGTH) &&
        (wRxlen != (PHAL_MFDFLIGHT_10B_UID_LENGTH) &&
        (wRxlen != PHAL_MFDFLIGHT_4B_UID_LENGTH)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    if((wRxlen == PHAL_MFDFLIGHT_10B_UID_LENGTH) ||
        (wRxlen ==  PHAL_MFDFLIGHT_4B_UID_LENGTH ))
    {
        /* In case of 4B/10B UID, strip out first 2 bytes as it contains UID format and UID length */
        *pUidLength = (uint8_t)(wRxlen - 2);
        memcpy(pUid, &pRecv[2], *pUidLength);
    }
    else
    {
        *pUidLength = (uint8_t)wRxlen;
        memcpy(pUid, pRecv, *pUidLength);
    }

    /* Set the card Length in wAdditionalInfo. This is done to assist C# wrapper as it will not be able
    to recognize the card UID Legth */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_SetConfig(pDataParams, PHAL_MFDFLIGHT_ADDITIONAL_INFO, (uint16_t)(*pUidLength)));

    return PH_ERR_SUCCESS;
}
#endif /*NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_Sw_GetVersion(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pVerInfo, uint8_t * pVerLength)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_GET_VERSION;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        1,
        &pRecv,
        &wRxlen
        ));

#ifdef RDR_LIB_PARAM_CHECK
    /* If received Data length is not equal to 28B,29B(In case of 7BUID) or 30B(In case of 10B UID), 27B(In case of 4B UID)
     * then its a Protocol Error
     */
    if (((wRxlen != PHAL_MFDFLIGHT_DEF_VERSION_LENGTH) && (wRxlen != PHAL_MFDFLIGHT_VERSION_LENGTH_WITH_FABKEY_ID)) &&
        (wRxlen != PHAL_MFDFLIGHT_10B_VERSION_LENGTH) &&
        (wRxlen  != PHAL_MFDFLIGHT_4B_VERSION_LENGTH))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    if( wRxlen > 0)
        memcpy(pVerInfo, pRecv, wRxlen); /* PRQA S 3200 */

    *pVerLength = (uint8_t)(wRxlen);

    /* Do a Set Config of ADDITIONAL_INFO to set  the length(wLength) of the Version string */
    phalMfdfLight_Sw_SetConfig((phalMfdfLight_Sw_DataParams_t *)pDataParams, PHAL_MFDFLIGHT_ADDITIONAL_INFO, wRxlen);

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_ChangeKey(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wOption, uint16_t wOldKeyNo, uint16_t wOldKeyVer, uint16_t wNewKeyNo,
    uint16_t wNewKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[42];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[42];
    uint8_t     PH_MEMLOC_REM bOldKey[32];
    uint8_t     PH_MEMLOC_REM bNewKey[32];
    uint8_t     PH_MEMLOC_REM bNewKeyLen= 0;
    uint8_t     PH_MEMLOC_REM bIndex;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wOldKeyType;
    uint16_t    PH_MEMLOC_REM wNewKeyType;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bMacLen;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint32_t    PH_MEMLOC_REM dwCrc;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint16_t    PH_MEMLOC_REM wTmpOption = 0x0000;
    uint8_t     PH_MEMLOC_REM bAppId[3] = { 0x00, 0x00, 0x00 };
    uint8_t *   PH_MEMLOC_REM pRecv;
    uint16_t    PH_MEMLOC_REM wWorkBufferLen = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if ((wOption == 0x0000) || (bDivLen > 31))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    /* Validate Card Key Number */
    if(memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
    {
        /* At PICC level, no changeKey can be performed */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    else
    {
        /* At App level, 0,1,2,3,4 are valid ones. */
        if((bKeyNoCard > PHAL_MFDFLIGHT_ORIGINALITY_KEY_LAST))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
        }
    }
    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */

    memset(bWorkBuffer, 0x00, 42); /* PRQA S 3200 */
    memset(bCmdBuff, 0x00, 42); /* PRQA S 3200 */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_CHANGE_KEY;
    bCmdBuff[wCmdLen++] = bKeyNoCard;

    /* The IV will be different if AuthMode is AV2. Here the
    * ENC IV has to be computed and used for encryption.
    * The MAC IV is required to generate the MAC and append this to
    * the command before sending to the card.
    */

    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;

        /* the IV is constructed by encrypting with KeyID.SesAuthENCKey according to the ECB mode
        * As ECB encription doesnot use IV during the encription so we need not backup/ update with zero IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ComputeIv(
            PH_OFF,
            pDataParams->bTi,
            pDataParams->wCmdCtr,
            pDataParams->bIv
            ));

        /* Encrypt IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_ECB,
            pDataParams->bIv,
            bIvLen,
            pDataParams->bIv
            ));
    }
    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            bIvLen
            ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wOldKeyNo,
        wOldKeyVer,
        sizeof(bOldKey),
        bOldKey,
        &wOldKeyType
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wNewKeyNo,
        wNewKeyVer,
        sizeof(bNewKey),
        bNewKey,
        &wNewKeyType
        ));

    if (wOldKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wOldKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bOldKey[8], bOldKey, 8); /* PRQA S 3200 */
    }
    if (wNewKeyType == PH_KEYSTORE_KEY_TYPE_DES)
    {
        wNewKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        memcpy(&bNewKey[8], bNewKey, 8); /* PRQA S 3200 */
    }
    /*
    It is allowed to change a key type for PICC master key.
    Old key may not be diversified but new key can be.
    Old key may be diversified with one round but new key can
    be diversified with two rounds.

    Key diversification method (DESFire or MFPlus) cannot be changed
    between old and new key.

    It is assumed that the diversification input specified is the same
    for both old key and new key
    */

    if ((wOption != PHAL_MFDFLIGHT_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY )
        {
            if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC)
            {
                wTmpOption = PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS;
            }
            else
            {
                wTmpOption = PH_CRYPTOSYM_DIV_MODE_DESFIRE;
                if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY_ONERND)
                {
                    wTmpOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF;
                }
            }
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wTmpOption,
                bNewKey,
                wNewKeyType,
                pDivInput,
                bDivLen,
                bNewKey
                ));
        }
        if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY )
        {
            if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC)
            {
                wTmpOption |= PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS;
            }
            else
            {
                wTmpOption |= PH_CRYPTOSYM_DIV_MODE_DESFIRE;
                if (wOption & PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY_ONERND)
                {
                    wTmpOption |= PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF;
                }
            }
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wTmpOption,
                bOldKey,
                wOldKeyType,
                pDivInput,
                bDivLen,
                bOldKey
                ));
        }

        /* Reload the IV and key since the diversify function has invalidated the key */
        /* Load the Session key which is valid for this authentication */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bSesAuthENCKey,
            pDataParams->bCryptoMethod
            ));
        /* The IV will be different if AuthMode is AV2. Here the
        * ENC IV has to be computed and used for encryption.
        * The MAC IV is required to generate the MAC and append this to
        * the command before sending to the card.
        */
        if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
        {
            /* the IV is constructed by encrypting with KeyID.SesAuthENCKey according to the ECB mode
             * As ECB encription doesnot use IV during the encription so we need not backup/ update with zero IV*/
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ComputeIv(
                PH_OFF,
                pDataParams->bTi,
                pDataParams->wCmdCtr,
                pDataParams->bIv
                ));

            /* Encrypt IV */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_ECB,
                pDataParams->bIv,
                bIvLen,
                pDataParams->bIv
                ));
        }
        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            bIvLen
            ));

        /* Need to check whether this is required for 0x0A mode also*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_SetConfig(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CONFIG_KEEP_IV,
            PH_CRYPTOSYM_VALUE_KEEP_IV_ON
            ));
    }

    switch(wNewKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        bNewKeyLen = PH_CRYPTOSYM_AES128_KEY_SIZE;
        break;

    case PH_CRYPTOSYM_KEY_TYPE_2K3DES:
        bNewKeyLen = PH_CRYPTOSYM_2K3DES_KEY_SIZE;
        break;

    case PH_CRYPTOSYM_KEY_TYPE_3K3DES:
        bNewKeyLen = PH_CRYPTOSYM_3K3DES_KEY_SIZE;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFDFLIGHT);
    }

    /* Need new else if statement for AuthEV2
    * In AuthEV2, it is sent in FULL mode, meaning the Data is encrypted
    * and a MAC is calculated and attached at the end.
    * ENC Session key is used for encryption and MAC session key is
    * used for MAC calculation
    */
    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
    {
        if ((pDataParams->bKeyNo & 0x3FU) != (bKeyNoCard & 0x3FU))
        {
            /* Copy bNewKey to the bWorkBuffer */
            for (bIndex = 0; bIndex < bNewKeyLen; bIndex++)
            {
                bWorkBuffer[bIndex] = bNewKey[bIndex];
            }

            /* Append Key Version */
            bWorkBuffer[bIndex++] = (uint8_t)wNewKeyVer;

            /* Calculate CRC32 over the new key  ie CRC32NK shall be the 4-byte CRC value taken over NewKey */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC32_PRESET_DF8,
                PH_TOOLS_CRC32_POLY_DF8,
                bWorkBuffer,
                (uint16_t)bIndex - 1,
                &dwCrc
                ));

            /* Key number authenticated with is not the key to be
            * changed
            * xored_Data = pNewKey ^ wKey
            * bWorkBuffer contains pNewKey
            */
            for (bIndex = 0; bIndex < bNewKeyLen; bIndex++)
            {
                bWorkBuffer[bIndex] = bOldKey[bIndex] ^ bWorkBuffer[bIndex];
            }
            /* xored_Data+ [AES key version] + CRC32 (all prev. data) + CRC32(new key)+padding */
            /* Adding key version should always be true because we are only dealing with
            * AES128 keys here
            */
            bIndex++;   /* Just increment bIndex because it already contains wNewKeyVer */
            memcpy(&bCmdBuff[2], bWorkBuffer, bIndex); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bIndex;

            memcpy(&bCmdBuff[wCmdLen], &dwCrc, 4); /* PRQA S 3200 */
            wCmdLen += 4;

            wTmp = wCmdLen - 2;

            if (wTmp % bIvLen)
            {
                /* Apply padding */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                    PH_CRYPTOSYM_PADDING_MODE_2,
                    &bCmdBuff[2],
                    wTmp,
                    bIvLen,
                    sizeof(bCmdBuff)-2,
                    &bCmdBuff[2],
                    &wTmp
                    ));
            }

            /* Encrypt */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                &bCmdBuff[2],
                wTmp,
                &bCmdBuff[2]
            ));
            wCmdLen = wTmp + 2;

        }
        else
        {
            memcpy(&bCmdBuff[wCmdLen], bNewKey, bNewKeyLen); /* PRQA S 3200 */
            wCmdLen = wCmdLen + bNewKeyLen;

            /* Also check if it is PICC master key and is an AES key is to be written.
            If so then key version also needs to be appended to the new key before
            calculating CRC */
            if (memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
            {
                if ((bKeyNoCard & 0xC0) == 0x80)
                {
                    /* PICC master key is being changed to AES key. Version is relevant */
                    bCmdBuff[wCmdLen++] = (uint8_t)wNewKeyVer;
                }
            }
            else
            {
                bCmdBuff[wCmdLen++] = (uint8_t)wNewKeyVer;
            }

            wTmp = wCmdLen-2;
            /* Apply padding */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                PH_CRYPTOSYM_PADDING_MODE_2,
                &bCmdBuff[2],
                wTmp,
                bIvLen,
                sizeof(bCmdBuff) - 2,
                &bCmdBuff[2],
                &wTmp
                ));

            /* Encrypt */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
                &bCmdBuff[2],
                wTmp,
                &bCmdBuff[2]
            ));
            /* Update Cmd len */
            wCmdLen = wTmp + 2;
        }

        memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */
        wWorkBufferLen = 0;
        bWorkBuffer[wWorkBufferLen++] = bCmdBuff[0];
        /* Add CmdCtr and TI for MAC calculation */
        bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr);
        bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
        memcpy(&bWorkBuffer[wWorkBufferLen], pDataParams->bTi, PHAL_MFDFLIGHT_SIZE_TI); /* PRQA S 3200 */
        wWorkBufferLen += PHAL_MFDFLIGHT_SIZE_TI;

        memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (wCmdLen - 1)); /* PRQA S 3200 */
        wWorkBufferLen += (wCmdLen - 1);

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->bIv,
            bIvLen
            ));

        /* Append MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            (PH_CRYPTOSYM_MAC_MODE_CMAC),
            bWorkBuffer,
            wWorkBufferLen,
            bCMAC,
            &bMacLen
            ));

        /* Truncate the MAC generated */
        phalMfdfLight_Sw_Int_TruncateMac(bCMAC);

        memcpy(&bCmdBuff[wCmdLen], bCMAC, 8); /* PRQA S 3200 */
        wCmdLen += 8;
    }
    else
    {
        /* ERROR: NOT_AUTHENTICATED */
        return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    /* Send the command */
    statusTmp = phalMfdfLight_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        );
    if (statusTmp != PH_ERR_SUCCESS)
    {
        if (pDataParams->bAuthMode != PHAL_MFDFLIGHT_AUTHENTICATE)
        {
            phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
        }
        return statusTmp;
    }

    /* Max 8 byte CMAC is expected nothing more. */
    if (wRxlen > 8)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    memcpy(bWorkBuffer, pRecv, wRxlen);  /* PRQA S 3200 */

    /* Verification of MAC also required for AuthEV2
    */
    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
    {
        /* reset authentication status if the key authenticated with is changed in
        * this case the card does not return a MAC because authentication is lost
        */
        if ((pDataParams->bKeyNo & 0x3FU) == (bKeyNoCard & 0x3FU))
        {
            phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
        }
        else
        {
            if (wRxlen < 8 ) /* If no CMAC received */
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
            }

            /* Increment the command counter. */
            pDataParams->wCmdCtr++;

            /* Load IV */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bIv,
                bIvLen
                ));

            /* copy CMAC received from card*/
            memcpy(bCMacCard, &bWorkBuffer[wRxlen - 8], 8);  /* PRQA S 3200 */
            wRxlen -= 8;

            /*
            * Calculate MAC on RC || wCmdCtr || TI || RespData
            * bWorkBuffer is used as receive buffer so pDataParams->pUnprocByteBuff is used
            */
            pDataParams->bNoUnprocBytes = 0x00;
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x00;
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr);
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pDataParams->bTi, PHAL_MFDFLIGHT_SIZE_TI); /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += PHAL_MFDFLIGHT_SIZE_TI;

            /* verify the MAC */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
                pDataParams->pUnprocByteBuff,
                pDataParams->bNoUnprocBytes,
                bCMAC,
                &bMacLen
                ));

            /* Truncate the MAC generated */
            phalMfdfLight_Sw_Int_TruncateMac(bCMAC);

            /* Compare the CMAC from card and CMAC calculated */
            if (memcmp(bCMacCard, bCMAC, 8) != 0)
            {
                phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFDFLIGHT);
            }
        }
    }
    else
    {
        /*Do Nothing. This is for PRQA compliance */
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_GetKeyVersion(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bKeyNo, uint8_t bKeySetNo, uint8_t * pKeyVersion, uint8_t * bRxLen)
{
    /**
    * This command can be issued without valid authentication
    */
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

#ifdef RDR_LIB_PARAM_CHECK
    uint8_t     PH_MEMLOC_REM bAppId[3] = {0x00, 0x00, 0x00};
    if(memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
    {
        /* At PICC level, 1,2,3,4 are valid ones. 0 is excluded */
        if((bKeyNo > PHAL_MFDFLIGHT_ORIGINALITY_KEY_LAST ) || (bKeyNo < PHAL_MFDFLIGHT_ORIGINALITY_KEY_FIRST))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
        }
    }
    else
    {
        /* At App level, 0,1,2,3,4 are valid ones. */
        if((bKeyNo > PHAL_MFDFLIGHT_ORIGINALITY_KEY_LAST))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
        }
    }
#endif /* RDR_LIB_PARAM_CHECK */

    bKeySetNo =  0x00 ; /* To avoid warnings */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_GET_KEY_VERSION;
    bCmdBuff[wCmdLen++] = bKeyNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /*
    * If length of return bytes is not 1B
    */
    if (wRxlen != 0x01)
    {
         return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    if(memcpy(pKeyVersion, pRecv, wRxlen) == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    *bRxLen = (uint8_t)wRxlen;

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_CreateTransactionMacFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo, uint8_t bCommMode, uint8_t * pAccessRights,
    uint8_t bKeyType, uint8_t * bTMKey, uint8_t bTMKeyVer)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

#ifdef RDR_LIB_PARAM_CHECK
    /* Check for valid file no. and KeyType == AES128 */
    if (bKeyType != PHAL_MFDFLIGHT_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if ((bCommMode != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommMode != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN_1) &&
        (bCommMode != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommMode != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_CREATE_TRANSACTION_MACFILE;
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy communication settings. communication settings in the first nibble so right shifting */
    bCmdBuff[wCmdLen++] = bCommMode >> 4;

    /* Copy Access rights */
    /* Consider checking KEY TYPE passed here and accordingly
    * update the bits and store in cmd buff. */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];
    bCmdBuff[wCmdLen++] = bKeyType;

    memcpy(&bCmdBuff[wCmdLen], bTMKey, 16); /* PRQA S 3200 */
    wCmdLen += 16;

    bCmdBuff[wCmdLen++] = bTMKeyVer;

    /* When authenticated, CommMode.Full is applied (this is different compared to
     * the other filetypes which require CommMode.MAC as no data needs to be encrypted).
     * If not authenticated, the command is send in plain of course. */

    if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            wCmdLen,
            PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
            NULL,
            0x0000
            );
    }
    else
    {
        return phalMfdfLight_Sw_Int_Write_Enc(
            pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            6,
            PH_CRYPTOSYM_PADDING_MODE_1,
            0x00,
            &bCmdBuff[6],
            (wCmdLen - 6)
            );
    }
}

phStatus_t phalMfdfLight_Sw_ChangeFileSettings(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommMode, uint8_t bFileNo, uint8_t bFileOption, uint8_t *pAccessRights,
    uint8_t bTmcLimitLen, uint8_t * pTmcLimit)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[24];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_CHANGE_FILE_SETTINGS;
    bCmdBuff[wCmdLen++] = bFileNo;

    if(bCommMode & PHAL_MFDFLIGHT_MFM_SPECIFICS_ENABLED)
    {
        /* MFM Specific Case, it is clear that the bFileOption parameter sent by the user is prepared as per
         * reference architecture.
         */

        /* Clear the bCommMode[bit-0] */
        bCommMode = bCommMode & 0xFE;

        /* Copy File Option byte */
        bCmdBuff[wCmdLen++] = bFileOption;

        /* Copy Access rights */
        bCmdBuff[wCmdLen++] = pAccessRights[0];
        bCmdBuff[wCmdLen++] = pAccessRights[1];

        /* Copy optional Data If targetted TMAC File and TMC Limit flag is enabled, bAdditionalInfo contains the TMCLImit */
        if(bFileOption & PHAL_MFDFLIGHT_TMCLIMITCONFIG)
        {
            memcpy(&bCmdBuff[wCmdLen], pTmcLimit, bTmcLimitLen);
            wCmdLen +=  bTmcLimitLen;
        }
    }
    else
    {
        /* In this Case, here the bFileOption parameter is prepared as per Desfire Ev2 Convention.
         * Note that in this case, bit-4 and bit-5 is not enabled.
         */

         /* Copy communication settings. Communication settings in the first nibble so right shifting */
        if (bFileOption & 0x80)
        {
            bCmdBuff[wCmdLen++] = (uint8_t)(((bFileOption >> 4) & 0x03) | 0x80);
        }
        else
        {
            bCmdBuff[wCmdLen++] = (uint8_t)(bFileOption >> 4);
        }

        /* Copy Access rights */
        bCmdBuff[wCmdLen++] = pAccessRights[0];
        bCmdBuff[wCmdLen++] = pAccessRights[1];
    }

    /* Based on the Mode of communication selected by the caller/Application, transmit the data */
    if(((bCommMode & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN))
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
    }
    #ifdef NXPBUILD__PH_CRYPTOSYM
    else if((bCommMode & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD)
    {
        /* COMMUNICATION IS Mac */
        return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDFLIGHT_COMMUNICATION_MACD,
        NULL,
        0x0000
        );
    }
    else if((bCommMode & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
        return phalMfdfLight_Sw_Int_Write_Enc(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            0x00,
            &bCmdBuff[2],
            wCmdLen - 2
            );
    }
    #endif /*NXPBUILD__PH_CRYPTOSYM */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
}
#endif /*NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfdfLight_Sw_DeleteFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_DELETE_FILE;
    bCmdBuff[1] = bFileNo;

    /* COMMUNICATION IS PLAIN */
    return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        2,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdfLight_Sw_GetFileIDs(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pFid, uint8_t * bNumFID)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_GET_FILE_IDS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        1,
        &pRecv,
        &wRxlen
        ));

    memcpy(pFid, pRecv, wRxlen); /* PRQA S 3200 */
    *bNumFID = (uint8_t)wRxlen;

    return PH_ERR_SUCCESS;
}

/*
 * Should check the chaining case here. Maximum length returnable from card is 32 * 2 = 64 bytes.
 * But this cannot be returned in one exchange. Should check the possibility of returning pointer to
 * RxBuffer instead of doing a memcpy of the RxBuffer to user buffer.
 */
phStatus_t phalMfdfLight_Sw_GetISOFileIDs(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pFidBuffer, uint8_t * pNumFID)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* Initialization */
    *pNumFID = 0;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_GET_ISO_FILE_IDS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        1,
        &pRecv,
        &wRxlen
        ));

    /* Length should be multiple of 2 */
    if ((wRxlen != 0) && (wRxlen % 2))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
    memcpy(pFidBuffer, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pNumAids and return  */
    *pNumFID = (uint8_t)(wRxlen / 2);

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_GetFileSettings(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo, uint8_t * pFSBuffer, uint8_t * pBufferLen)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_GET_FILE_SETTINGS;
    bCmdBuff[1] = bFileNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        2,
        &pRecv,
        &wRxlen
        ));

    /*
    * File type can be obtained by reading the zeroth index of the receive buffer
    * For Std data file, pRecv[0] = 0x00
    * For Backup data file, pRecv[0] = 0x01
    * For Value file, pRecv[0] = 0x02
    * For Linear Record file, pRecv[0] = 0x03
    * For Cyclic Record file, pRecv[0] = 0x04
    * For Transaction MAC file, pRecv[0] = 0x05
    * For TransactionMAC file wRxLen = 6
    * For Standard file or Backup file mandatory 7 bytes and optional No. of AAR with AAR are received
    * For Value file mandatory 17 bytes and optional No. of AAR with AAR are received
    * For Linear/Cyclic Record file mandatory 13 bytes and optional No. of AAR with AAR are received
    */

    if ((pRecv[0] == 0x00) || (pRecv[0] == 0x01))
    {
        if ((wRxlen < 7) || ((wRxlen > 7) && (wRxlen != (8 + (pRecv[7] * 2)))))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }
    else if (pRecv[0] == 0x02)
    {
        if ((wRxlen < 17) || ((wRxlen > 17) && (wRxlen != (18 + (pRecv[17] * 2)))))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }
    else if ((pRecv[0] == 0x03) || (pRecv[0] == 0x04))
    {
        if ((wRxlen < 13) || ((wRxlen > 13) && (wRxlen != (14 + (pRecv[13] * 2)))))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }
    else if (pRecv[0] == 0x05)
    {
        if (wRxlen < 0x06)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    memcpy(pFSBuffer, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pBufferLen and return  */
    *pBufferLen = (uint8_t)wRxlen;

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_ReadData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pLength,
    uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    /* The signature of this is changed. We include
    * the bIns as third parameter that will differentiate
    * between application chaining and ISO chaining modes
    */
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint8_t     PH_MEMLOC_REM bTmp = 0;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    uint32_t    PH_MEMLOC_REM dwDataLen = 0;

#ifdef RDR_LIB_PARAM_CHECK
    /* bit[1] of bIns will also be used. Hence bIns should be checked for above 0x03*/
    if (bIns > 0x03)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    if ((bOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_RESP_ADDITIONAL_FRAME;
    }
    else if ((bOption & 0x0FU) == PH_EXCHANGE_DEFAULT)
    {
        /* copy data length */
        dwDataLen = pLength[2];
        dwDataLen = dwDataLen << 8 | pLength[1];
        dwDataLen = dwDataLen << 8 | pLength[0];

        if (bIns == 0x00)
        {
            bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_READ_DATA;
        }
        else
        {
            bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_READ_DATA_ISO;
        }

        bCmdBuff[wCmdLen++] = bFileNo;
        memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
        wCmdLen += 3;
        memcpy(&bCmdBuff[wCmdLen], pLength, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_TMI_STATUS,
            &dwTMIStatus));

        /* Check TMI Collection Status */
        if (dwTMIStatus == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
                (uint8_t)((dwDataLen == 0) ? (PH_TMIUTILS_READ_INS | PH_TMIUTILS_ZEROPAD_CMDBUFF) : PH_TMIUTILS_ZEROPAD_CMDBUFF),
                bCmdBuff,
                wCmdLen,
                NULL,
                0,
                PHAL_MFDFLIGHT_BLOCK_SIZE
                ));
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }

    if ((bOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Upload Payload size for proper CRC calculation */
        if ((bOption & 0x0FU) !=  PH_EXCHANGE_RXCHAINING)
        {
            pDataParams->dwPayLoadLen = dwDataLen;
        }

        status = phalMfdfLight_Sw_Int_ReadData_Enc(
            pDataParams,
            bOption | ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
#endif /* NXPBUILD__PH_CRYPTOSYM */
    }
    else if (((bOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD) ||
        ((bOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN))
    {
        status = phalMfdfLight_Sw_Int_ReadData_Plain(
            pDataParams,
            bOption | ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }

    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDFLIGHT_RESP_ADDITIONAL_FRAME))
    {
        if ((dwDataLen != *pRxdataLen) && (dwDataLen != 0))
        {
            /* Reset authentication status */
            if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
            {
                phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }
    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        if (dwDataLen == 0)
        {
            bTmp = PH_TMIUTILS_READ_INS;
        }
        if (status == PH_ERR_SUCCESS)
        {
            bTmp |= PH_TMIUTILS_ZEROPAD_DATABUFF;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            bTmp,
            NULL,
            0,
            *ppRxdata,
            *pRxdataLen,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));

        if ((status == PH_ERR_SUCCESS) && (dwDataLen == 0))
        {
            /* Reset wOffsetInTMI to 0 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_SetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_TMI_OFFSET_LENGTH,
                0
                ));
        }
    }
    return status;
}

phStatus_t phalMfdfLight_Sw_WriteData(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset, uint8_t * pData,
    uint8_t * pDataLen)
{
    /* The signature of this is changed. We include
    * the bIns as third parameter that will differentiate
    * between application chaining and ISO chaining modes
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommOptionTemp = bCommOption;
    uint8_t     PH_MEMLOC_REM bLoopData = 1;
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t     PH_MEMLOC_REM bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
#endif /*NXPBUILD__PH_CRYPTOSYM */
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wDataLenTemp;
    uint32_t    PH_MEMLOC_REM dwDataLen;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    uint32_t    PH_MEMLOC_REM dwDataWritten = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if (bIns > 0x01)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command depending on bIns */
    if (bIns == 0x00)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_WRITE_DATA;
    }
    else
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_WRITE_DATA_ISO;
    }
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* copy data length */
    dwDataLen = pDataLen[2];
    dwDataLen = dwDataLen << 8 | pDataLen[1];
    dwDataLen = dwDataLen << 8 | pDataLen[0];

    /* to handle 2 MB of data update maximum of data bytes that can be sent in a single loop */
    if(dwDataLen > PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
    {
        wDataLenTemp =(uint16_t) PHAL_MFDFLIGHT_MAX_WRITE_SIZE;
        bLoopData = (uint8_t)(dwDataLen/PHAL_MFDFLIGHT_MAX_WRITE_SIZE);
        if(dwDataLen % PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
        {
            bLoopData++;
        }
        bCommOptionTemp = PHAL_MFDFLIGHT_MAC_DATA_INCOMPLETE |bCommOption;
    }
    else
    {
        wDataLenTemp = (uint16_t)dwDataLen;
    }

    /* update the total number of data to be written
     * when INS and wrapped mode is enable, we need to send the total number of bytes with Le */
    pDataParams->dwPayLoadLen = dwDataLen;

    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDFLIGHT_COMMUNICATION_PLAIN;
    }
    #ifdef NXPBUILD__PH_CRYPTOSYM
    else
    {
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD)
        {
            pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x04 : 0x08);
        }
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
        {
            if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
            {
                pDataParams->dwPayLoadLen = 8 + pDataParams->dwPayLoadLen + ((pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)?
                                                                        (PH_CRYPTOSYM_AES_BLOCK_SIZE - (pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)) : PH_CRYPTOSYM_AES_BLOCK_SIZE) ;
            }
            else
            {
                pDataParams->dwPayLoadLen  = pDataParams->dwPayLoadLen  + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x02 : 0x04);
                bIvLen = (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEAES) ? PH_CRYPTOSYM_AES_BLOCK_SIZE : PH_CRYPTOSYM_DES_BLOCK_SIZE;

                if(pDataParams->dwPayLoadLen % bIvLen)
                {
                    pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen +  (bIvLen - (pDataParams->dwPayLoadLen % bIvLen));
                }
            }
        }
    }
    #endif /*NXPBUILD__PH_CRYPTOSYM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            (PH_TMIUTILS_ZEROPAD_CMDBUFF | PH_TMIUTILS_ZEROPAD_DATABUFF),
            bCmdBuff,
            wCmdLen,
            pData,
            dwDataLen,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    do
    {
        if (bLoopData == 1)
        {
            bCommOptionTemp = bCommOption;
            wDataLenTemp = (uint16_t)(dwDataLen - dwDataWritten);
        }
        if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
        {
#ifdef NXPBUILD__PH_CRYPTOSYM
            statusTmp =  phalMfdfLight_Sw_Int_Write_Enc(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                PH_CRYPTOSYM_PADDING_MODE_1,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );
#endif /*NXPBUILD__PH_CRYPTOSYM */
        }
        else
        {
            statusTmp =  phalMfdfLight_Sw_Int_Write_Plain(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );
        }

        if((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            return statusTmp;
        }

        bLoopData--;
        dwDataWritten +=wDataLenTemp;

        bCmdBuff[0] = 0xAF;
        wCmdLen = 1;

    }while(bLoopData > 0);

    return statusTmp;

}

phStatus_t phalMfdfLight_Sw_GetValue(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{

    /* If not authenticated, send the data and get the value in plain.
    Else use the mode dictated by the caller of this API
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
#ifdef RDR_LIB_PARAM_CHECK
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_GET_VALUE;
    bCmdBuff[1] = bFileNo;

    if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Upload Payload size for proper CRC calculation */
        pDataParams->dwPayLoadLen = 4;

        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Enc(
            pDataParams,
            bCommOption,
            bCmdBuff,
            2,
            &pRecv,
            &wRxlen
            ));
#endif /* NXPBUILD__PH_CRYPTOSYM */
    }
    else
    {
        /* For EV2, bCommOption == MACD means the cmd+MAC is sent to card
        * and a MAC on response is received.
        * Hope this is taken care of in readdata_plain
        */

        if((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) && ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD))
        {
            bCommOption = bCommOption | PHAL_MFDFLIGHT_COMMUNICATION_MAC_ON_RC;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
            pDataParams,
            bCommOption,
            bCmdBuff,
            2,
            &pRecv,
            &wRxlen
            ));
    }

    if (wRxlen != 4)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
    memcpy(pValue, pRecv, wRxlen); /* PRQA S 3200 */

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_ZEROPAD_DATABUFF,
            bCmdBuff,
            2,
            pValue,
            4,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_Credit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t status = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_ZEROPAD_CMDBUFF,
            bCmdBuff,
            wCmdLen,
            NULL,
            0,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        status = phalMfdfLight_Sw_Int_Write_Enc(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            0x00,
            &bCmdBuff[2],
            0x0004
            );
#endif /*NXPBUILD__PH_CRYPTOSYM */
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        /* Need to differentiate between plain and MACD mode
        * for AuthEV2 mode
        */
        status = phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
    return status;
}

phStatus_t phalMfdfLight_Sw_Debit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t status = 0;
#ifdef RDR_LIB_PARAM_CHECK
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_DEBIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_ZEROPAD_CMDBUFF,
            bCmdBuff,
            wCmdLen,
            NULL,
            0,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        status = phalMfdfLight_Sw_Int_Write_Enc(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            0x00,
            &bCmdBuff[2],
            0x0004
            );
#endif /*NXPBUILD__PH_CRYPTOSYM */
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        status = phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
    return status;
}

phStatus_t phalMfdfLight_Sw_LimitedCredit(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bFileNo, uint8_t * pValue)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t status = 0;
#ifdef RDR_LIB_PARAM_CHECK
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_LIMITED_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_ZEROPAD_CMDBUFF,
            bCmdBuff,
            wCmdLen,
            NULL,
            0,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        status = phalMfdfLight_Sw_Int_Write_Enc(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            PH_CRYPTOSYM_PADDING_MODE_1,
            0x00,
            &bCmdBuff[2],
            0x0004
            );
#endif /*NXPBUILD__PH_CRYPTOSYM */
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        status = phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
            PHAL_MFDFLIGHT_DEFAULT_MODE,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
    return status;
}

phStatus_t phalMfdfLight_Sw_ReadRecords(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo,
    uint8_t * pRecCount, uint8_t * pRecSize, uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwRecLen = 0;
    uint32_t    PH_MEMLOC_REM dwNumRec = 0;
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bOption = 0;
    uint32_t    PH_MEMLOC_REM dwDataLen = 0;
    uint32_t    PH_MEMLOC_REM dwOffsetTMI = 0;
    uint32_t    PH_MEMLOC_REM dwTMIBufInd = 0;
    uint32_t    PH_MEMLOC_REM dwTotalRecLen = 0;
    uint32_t    PH_MEMLOC_REM dwNumRecCal = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if (bIns > 0x03)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* copy size of each record */
    dwRecLen = pRecSize[2];/* MSB */
    dwRecLen = dwRecLen << 8 | pRecSize[1];
    dwRecLen = dwRecLen << 8 | pRecSize[0];/* LSB */

    /* copy number of records to be read */
    dwNumRec = pRecCount[2];/* MSB */
    dwNumRec = dwNumRec << 8 | pRecCount[1];
    dwNumRec = dwNumRec << 8 | pRecCount[0];/* LSB */

    if ((bCommOption & 0x0FU) ==  PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_RESP_ADDITIONAL_FRAME;
    }
    else if ((bCommOption & 0x0FU) ==  PH_EXCHANGE_DEFAULT)
    {
        /* form the command depending upon bIns */
        if (bIns == 0x00)
        {
            bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_READ_RECORDS;
        }
        else
        {
            bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_READ_RECORDS_ISO;
        }
        bCmdBuff[wCmdLen++] = bFileNo;

        /* Record No */
        memcpy(&bCmdBuff[wCmdLen], pRecNo, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* Record Count */
        memcpy(&bCmdBuff[wCmdLen], pRecCount, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* Total number of bytes to read */
        dwTotalRecLen = (uint32_t)dwRecLen * dwNumRec;

        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_TMI_STATUS,
            &dwTMIStatus));

        /* Check TMI Collection Status */
        if (dwTMIStatus == PH_ON)
        {
            /* Should should provide atleast wRecLen / wNumRec to update in TIM collection */
            if((0 == dwRecLen) && (0 == dwNumRec))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
            }
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
                (uint8_t)((dwNumRec == 0) ? (PH_TMIUTILS_READ_INS | PH_TMIUTILS_ZEROPAD_CMDBUFF) : PH_TMIUTILS_ZEROPAD_CMDBUFF),
                bCmdBuff,
                wCmdLen,
                NULL,
                0,
                PHAL_MFDFLIGHT_BLOCK_SIZE
                ));
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Upload Payload size for proper CRC calculation */
        if ((bCommOption & 0x0FU) !=  PH_EXCHANGE_RXCHAINING)
        {
            pDataParams->dwPayLoadLen = dwTotalRecLen;
        }

        status = phalMfdfLight_Sw_Int_ReadData_Enc(
            pDataParams,
            bCommOption | ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
#endif /* NXPBUILD__PH_CRYPTOSYM */
    }
    else if (((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) ||
        ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {

        if((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) && ((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD))
        {
            bCommOption = bCommOption | PHAL_MFDFLIGHT_COMMUNICATION_MAC_ON_RC;
        }

        status = phalMfdfLight_Sw_Int_ReadData_Plain(
            pDataParams,
            bCommOption | ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        return status;
    }

    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDFLIGHT_RESP_ADDITIONAL_FRAME))
    {
        /* Can check this case if user has not given num of records
        as 0x000000. If 0x000000, then all records are read */
        if ((dwTotalRecLen != *pRxdataLen) && (dwTotalRecLen != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEAES))
            {
                phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
        }
    }

    /* if function called with PH_EXCHANGE_RXCHAINING */
    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        if ((dwNumRec == 0) && (status == PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_TMI_OFFSET_LENGTH,
                &dwOffsetTMI
                ));
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_TMI_BUFFER_INDEX,
                &dwTMIBufInd
                ));

            /* calculate Rx length in case of chaining */
            dwDataLen = *pRxdataLen + dwTMIBufInd -(dwOffsetTMI + 11);

            /* for Qmore compliance below code is added check is done before itself  */
            if(dwRecLen == 0)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
            }

            /* if user update worng RecSize, we cant calculate recCnt */
            if(dwDataLen % dwRecLen)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
            }

            /* calculate number of records */
            dwNumRecCal = dwDataLen / dwRecLen ;

            /* update record count */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_SetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_TMI_OFFSET_VALUE,
                dwNumRecCal
                ));

        }
        if (status == PH_ERR_SUCCESS)
        {
            bOption = PH_TMIUTILS_ZEROPAD_DATABUFF;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            bOption,
            NULL,
            0,
            *ppRxdata,
            *pRxdataLen,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));

        if ((status == PH_ERR_SUCCESS) && (dwNumRec == 0))
        {
            /* Reset wOffsetInTMI to 0 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_SetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_TMI_OFFSET_LENGTH,
                0
                ));
        }
    }
    return status;
}

phStatus_t phalMfdfLight_Sw_WriteRecord(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pOffset,
    uint8_t * pData, uint8_t * pDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint8_t     PH_MEMLOC_REM bCommOptionTemp = bCommOption;
    uint8_t     PH_MEMLOC_REM bLoopData = 1;
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t     PH_MEMLOC_REM bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
#endif /*NXPBUILD__PH_CRYPTOSYM */
    uint16_t    PH_MEMLOC_REM wDataLenTemp;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint32_t    PH_MEMLOC_REM dwDataLen;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    uint32_t    PH_MEMLOC_REM dwDataWritten = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if (bIns > 0x01)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command depending on bIns */
    if (bIns == 0x00)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_WRITE_RECORD;
    }
    else
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_WRITE_RECORD_ISO;
    }
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* Assuming here that the size can never go beyond FFFF. */
    dwDataLen = pDataLen[2];
    dwDataLen = dwDataLen << 8 | pDataLen[1];
    dwDataLen = dwDataLen << 8 | pDataLen[0];

    /* to handle 2 MB of data update maximum of data bytes that can be sent in a single loop */
    if(dwDataLen > PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
    {
        wDataLenTemp = (uint16_t)PHAL_MFDFLIGHT_MAX_WRITE_SIZE;
        bLoopData = (uint8_t)(dwDataLen/PHAL_MFDFLIGHT_MAX_WRITE_SIZE);
        if(dwDataLen % PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
        {
            bLoopData++;
        }
        bCommOptionTemp = PHAL_MFDFLIGHT_MAC_DATA_INCOMPLETE |bCommOption;
    }
    else
    {
        wDataLenTemp = (uint16_t)dwDataLen;
    }


    /* update the total number of data to be written
     * when INS and wrapped mode is enable, we need to send the total number of bytes with Le */
    pDataParams->dwPayLoadLen = dwDataLen;

    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDFLIGHT_COMMUNICATION_PLAIN;
    }
#ifdef NXPBUILD__PH_CRYPTOSYM
    else
    {
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD)
        {
            pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x04 : 0x08);
        }
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
        {
            if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
            {
                pDataParams->dwPayLoadLen = 8 + pDataParams->dwPayLoadLen + ((pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)?
                                                                        (PH_CRYPTOSYM_AES_BLOCK_SIZE - (pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)) : PH_CRYPTOSYM_AES_BLOCK_SIZE) ;
            }
            else
            {
                pDataParams->dwPayLoadLen  = pDataParams->dwPayLoadLen  + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x02 : 0x04);
                bIvLen = (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEAES) ? PH_CRYPTOSYM_AES_BLOCK_SIZE : PH_CRYPTOSYM_DES_BLOCK_SIZE;

                if(pDataParams->dwPayLoadLen % bIvLen)
                {
                    pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen +  (bIvLen - (pDataParams->dwPayLoadLen % bIvLen));
                }
            }
        }
    }
#endif /*NXPBUILD__PH_CRYPTOSYM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            (PH_TMIUTILS_ZEROPAD_CMDBUFF | PH_TMIUTILS_ZEROPAD_DATABUFF),
            bCmdBuff,
            wCmdLen,
            pData,
            dwDataLen,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    do
    {
        if (bLoopData == 1)
        {
            bCommOptionTemp = bCommOption;
            wDataLenTemp = (uint16_t)(dwDataLen - dwDataWritten);
        }

        if ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)||
            (bCommOption == PHAL_MFDFLIGHT_COMMUNICATION_MACD) ||
            (bCommOption == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN))
        {
            statusTmp =  phalMfdfLight_Sw_Int_Write_Plain(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );

        }
        #ifdef NXPBUILD__PH_CRYPTOSYM
        else
        {
            statusTmp =  phalMfdfLight_Sw_Int_Write_Enc(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                PH_CRYPTOSYM_PADDING_MODE_1,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );
        }
        #endif /*NXPBUILD__PH_CRYPTOSYM */
        if((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            return statusTmp;
        }

        bLoopData--;
        dwDataWritten +=wDataLenTemp;

        bCmdBuff[0] = 0xAF;
        wCmdLen = 1;

    }while(bLoopData > 0);

    return statusTmp;
}

phStatus_t phalMfdfLight_Sw_UpdateRecord(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bCommOption, uint8_t bIns, uint8_t bFileNo, uint8_t * pRecNo,
    uint8_t * pOffset, uint8_t * pData, uint8_t * pDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommOptionTemp = bCommOption;
    uint8_t     PH_MEMLOC_REM bLoopData = 1;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t     PH_MEMLOC_REM bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;
#endif /*NXPBUILD__PH_CRYPTOSYM */
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wDataLenTemp;
    uint32_t    PH_MEMLOC_REM dwDataLen;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    uint32_t    PH_MEMLOC_REM dwDataWritten = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if (bIns > 0x01)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if ((bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDFLIGHT_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command depending on bIns */
    if (bIns == 0x00)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_UPDATE_RECORD;
    }
    else
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_UPDATE_RECORD_ISO;
    }
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pRecNo, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* copy data length */
    dwDataLen = pDataLen[2];
    dwDataLen = dwDataLen << 8 | pDataLen[1];
    dwDataLen = dwDataLen << 8 | pDataLen[0];

    /* to handle 2 MB of data update maximum of data bytes that can be sent in a single loop */
    if(dwDataLen > PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
    {
        wDataLenTemp = (uint16_t)PHAL_MFDFLIGHT_MAX_WRITE_SIZE;
        bLoopData = (uint8_t)(dwDataLen/PHAL_MFDFLIGHT_MAX_WRITE_SIZE);
        if(dwDataLen % PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
        {
            bLoopData++;
        }
        bCommOptionTemp = PHAL_MFDFLIGHT_MAC_DATA_INCOMPLETE |bCommOption;
    }
    else
    {
        wDataLenTemp = (uint16_t)dwDataLen;
    }

    /* update the total number of data to be written
     * when INS and wrapped mode is enable, we need to send the total number of bytes with Le */
    pDataParams->dwPayLoadLen = dwDataLen;

    if (pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDFLIGHT_COMMUNICATION_PLAIN;
    }
#ifdef NXPBUILD__PH_CRYPTOSYM
    else
    {
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_MACD)
        {
            pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x04 : 0x08);
        }
        if((bCommOption & 0xF0U) == PHAL_MFDFLIGHT_COMMUNICATION_ENC)
        {
            if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
            {
                pDataParams->dwPayLoadLen = 8 + pDataParams->dwPayLoadLen + ((pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)?
                                                                        (PH_CRYPTOSYM_AES_BLOCK_SIZE - (pDataParams->dwPayLoadLen % PH_CRYPTOSYM_AES_BLOCK_SIZE)) : PH_CRYPTOSYM_AES_BLOCK_SIZE) ;
            }
            else
            {
                pDataParams->dwPayLoadLen  = pDataParams->dwPayLoadLen  + ((pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATE) ? 0x02 : 0x04);
                bIvLen = (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEAES) ? PH_CRYPTOSYM_AES_BLOCK_SIZE : PH_CRYPTOSYM_DES_BLOCK_SIZE;

                if(pDataParams->dwPayLoadLen % bIvLen)
                {
                    pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen +  (bIvLen - (pDataParams->dwPayLoadLen % bIvLen));
                }
            }
        }
    }
#endif /*NXPBUILD__PH_CRYPTOSYM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            (PH_TMIUTILS_ZEROPAD_CMDBUFF | PH_TMIUTILS_ZEROPAD_DATABUFF),
            bCmdBuff,
            wCmdLen,
            pData,
            dwDataLen,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    do
    {
        if (bLoopData == 1)
        {
            bCommOptionTemp = bCommOption;
            wDataLenTemp = (uint16_t)(dwDataLen - dwDataWritten);
        }

        if ( (pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)||
            (bCommOption == PHAL_MFDFLIGHT_COMMUNICATION_MACD) ||
            (bCommOption == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN))
        {
            statusTmp = phalMfdfLight_Sw_Int_Write_Plain(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );
        }
#ifdef NXPBUILD__PH_CRYPTOSYM
        else
        {
            statusTmp = phalMfdfLight_Sw_Int_Write_Enc(
                pDataParams,
                ((bIns == 0x00) ? PHAL_MFDFLIGHT_DEFAULT_MODE : PHAL_MFDFLIGHT_ISO_CHAINING_MODE),
                bCmdBuff,
                wCmdLen,
                PH_CRYPTOSYM_PADDING_MODE_1,
                bCommOptionTemp,
                &pData[dwDataWritten],
                wDataLenTemp
                );
        }
#endif /*NXPBUILD__PH_CRYPTOSYM */
        /* If chaining send data remaining data */
        if((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            return statusTmp;
        }

        bLoopData--;
        dwDataWritten +=wDataLenTemp;

        bCmdBuff[0] = 0xAF;
        wCmdLen = 1;

    }while(bLoopData > 0);

    return statusTmp;
}

phStatus_t phalMfdfLight_Sw_ClearRecordFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bFileNo)
{
    uint8_t PH_MEMLOC_REM bCmdBuff[8];
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_CLEAR_RECORD_FILE;
    bCmdBuff[1] = bFileNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
            PH_TMIUTILS_ZEROPAD_CMDBUFF,
            bCmdBuff,
            2,
            NULL,
            0,
            PHAL_MFDFLIGHT_BLOCK_SIZE
            ));
    }

    return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        2,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdfLight_Sw_CommitTransaction(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * pTMC, uint8_t * pTMAC)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;

#ifdef RDR_LIB_PARAM_CHECK
    /* As per ref_arch 0.04 for Cmd.CommitTransaction: simplified to always
     * use CommMode.MAC, so communication mode of response does not depend
     * on File-Type.TransactionMAC anymore.
     */
    if ((bOption & 0x0F) > 0x01)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_COMMIT_TXN;

    /*
     * 0x01 and 0x00 are values to support the legacy implementation.
     *      Here if it 0x00 the Option is not exchanged to the PICC. Its only exchanged for 0x01
     *
     * 0x80 and 0x81 are the values to exchange the Option by to PICC.
     *      If its 0x80, Option byte will be exchanged by the Option information will be zero.
     *      If its 0x81, Option byte will be exchanged by the Option information will be one.
     */
    if((bOption & 0x80) || (bOption & 0x0F))
    {
        bCmdBuff[wCmdLen++] = (bOption & 0x0F);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));


    if (((bOption & 0x0F) == 0x01) && (wRxlen != 0x0C))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
    else
    {
        if(NULL != pRecv)
        {
            memcpy(pTMC, pRecv, 4); /* PRQA S 3200 */
            memcpy(pTMAC, &pRecv[4],8); /* PRQA S 3200 */
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_AbortTransaction(phalMfdfLight_Sw_DataParams_t * pDataParams)
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_ABORT_TXN;

    /* COMMUNICATION IS PLAIN */
    return phalMfdfLight_Sw_Int_Write_Plain(pDataParams,
        PHAL_MFDFLIGHT_DEFAULT_MODE,
        bCmdBuff,
        1,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdfLight_Sw_CommitReaderID(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t * pTMRI, uint8_t * pEncTMRI)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[24];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint32_t    PH_MEMLOC_REM dwTMIStatus = 0;
    uint8_t     PH_MEMLOC_REM bOption = PHAL_MFDFLIGHT_COMMUNICATION_PLAIN;
    uint8_t     PH_MEMLOC_REM bEncTMRILocal[16];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDFLIGHT_CMD_COMMIT_READER_ID;
    memcpy(&bCmdBuff[1], pTMRI, 16);

    if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
    {
        bOption = PHAL_MFDFLIGHT_COMMUNICATION_MACD;
    }

    /*
    * For Ev2 SM: PCD->PICC: cmd + cmdData ( CMAC (Kses, Cmd||cmdData) Mac is used as IV for next operation)
    *              PICC->PCD: RC + respData + CMAC (Kses, cmdData||RC)
    */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
        pDataParams,
        (pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)? PHAL_MFDFLIGHT_COMMUNICATION_MACD : PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
        bCmdBuff,
        17,
        &pRecv,
        &wRxlen
        ));

    if ((bOption == PHAL_MFDFLIGHT_COMMUNICATION_PLAIN) && (wRxlen != 0))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }
    else if ((bOption == PHAL_MFDFLIGHT_COMMUNICATION_MACD) && (wRxlen != 16))      /* Response should be 16 bytes long */
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    if(wRxlen > 0)
    {
        memcpy(bEncTMRILocal, pRecv, wRxlen);
        memcpy(pEncTMRI, pRecv, wRxlen);
    }
    else
    {
        pEncTMRI = '\0';
    }

    /* Do a Set Config of ADDITIONAL_INFO to set  the length(wLength) of the recieved TMRI */
    phalMfdfLight_Sw_SetConfig((phalMfdfLight_Sw_DataParams_t *)pDataParams, PHAL_MFDFLIGHT_ADDITIONAL_INFO, wRxlen);

    PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_GetConfig((phTMIUtils_t *)pDataParams->pTMIDataParams,
        PH_TMIUTILS_TMI_STATUS,
        &dwTMIStatus));

    /* Check TMI Collection Status */
    if (dwTMIStatus == PH_ON)
    {
        /* As per Ref arch(0.13),
            * If authenticated, Cmd.CommitReaderID shall update the Transaction MAC Input TMI as follows:
            * TMI = TMI || Cmd || TMRICur||EncTMRI||ZeroPadding
            */
        if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_AUTHENTICATEEV2)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_ZEROPAD_DATABUFF,
                bCmdBuff,
                17,
                bEncTMRILocal,
                16,
                PHAL_MFDFLIGHT_BLOCK_SIZE
                ));
            memcpy(pEncTMRI,bEncTMRILocal,16);
        }
        /* As per Ref arch(0.13),
            * If Not authenticated, Cmd.CommitReaderID shall update the Transaction MAC Input TMI as follows:
            * TMI = TMI || Cmd || TMRICur||ZeroPadding
            */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phTMIUtils_CollectTMI((phTMIUtils_t *)pDataParams->pTMIDataParams,
                PH_TMIUTILS_ZEROPAD_CMDBUFF,
                bCmdBuff,
                17,
                NULL,
                0x00,
                PHAL_MFDFLIGHT_BLOCK_SIZE
                ));
            pEncTMRI = '\0';
        }
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_IsoSelectFile(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t bSelector, uint8_t * pFid, uint8_t * pDFname,
    uint8_t bDFnameLen, uint8_t bExtendedLenApdu, uint8_t ** ppFCI, uint16_t * pwFCILen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bData[24];
    uint32_t     PH_MEMLOC_REM bLc = 0;
    uint32_t     PH_MEMLOC_REM bLe = 0;
    uint8_t     PH_MEMLOC_REM bFileId[3] = {'\0'};
    uint8_t     PH_MEMLOC_REM bPICCDefaultDFName[8] = {0xD2,0x76,0x00,0x00,0x85,0x01,0x00}; /*D2760000850100 PICC DFNAME*/
    uint16_t    wVal = 0;

#ifdef RDR_LIB_PARAM_CHECK
    if (bDFnameLen > 16)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    /* If User intends to select through DFName, check for validity of DFName passed */
    if((bSelector ==  0x04) && (bDFnameLen == 0x00))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if ((bOption != 0x00) && (bOption != 0x0C))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    if((bSelector !=  0x04) && (bSelector !=  0x03) && (pFid == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
#endif /* RDR_LIB_PARAM_CHECK */
    if((bSelector ==  0x00) || (bSelector == 0x01) || (bSelector == 0x02))
    {
        /* Selection by EF Id*/
        /* Send MSB first to card */
        if(pFid != NULL)
        {
             bFileId[1] = bData[0] = pFid[1];
             bFileId[0] = bData[1] = pFid[0];
             bFileId[2] = 0x00;
             bLc = 2;
        }
    }
    else if((bSelector ==  0x04))
    {
        /* Selection by DF Name */
        memcpy(bData, pDFname, bDFnameLen); /* PRQA S 3200 */
        bLc = bDFnameLen;
    }
    else
    {
        /* Do Nothing, Pass it to PICC. Let Card throw error */
    }
    statusTmp = phalMfdfLight_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDFLIGHT_CMD_ISO7816_SELECT_FILE,
        bSelector,
        bOption,
        bLc,
        bExtendedLenApdu,
        bData,
        bLe,
        ppFCI,
        pwFCILen);

    if((statusTmp & PH_ERR_MASK) == PHAL_MFDFLIGHT_ERR_DF_7816_GEN_ERROR)
    {
        (void)phalMfdfLight_Sw_GetConfig(pDataParams, PHAL_MFDFLIGHT_ADDITIONAL_INFO, &wVal);
    }

    if((statusTmp == PH_ERR_SUCCESS) || (wVal == PHAL_MFDFLIGHT_ISO7816_ERR_LIMITED_FUNCTIONALITY_INS))
    {
        /* Reset Authentication should not be targeted for elementary file selection using file ID */
        if(bSelector !=  0x02)
        {
            /* Reset Authentication Status here */
            phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
        }

        /* ISO wrapped mode is on */
        pDataParams->bWrappedMode = 1;

        /* once the selection Success, update the File Id to master data structure if the selection is done through AID*/
        if((bSelector ==  0x00) || (bSelector == 0x01) || (bSelector == 0x02))
        {
            memcpy(pDataParams->pAid, bFileId, sizeof(bFileId));
        }
        else if((bSelector ==  0x04))
        {
            /* Check for PICC Level default DF Name */
            if (memcmp(bPICCDefaultDFName, pDFname, bDFnameLen) == 0)
            {
                bFileId[0] = 0x00;
                bFileId[1] = 0x00;
                bFileId[2] = 0x00;
            }
            else
            {
                /* APP Level ISOSelect Done. This is done to avoid session key generation at the time of authenticate API */
                bFileId[0] = 0xff;
                bFileId[1] = 0xff;
                bFileId[2] = 0xff;
            }
            memcpy(pDataParams->pAid, bFileId, sizeof(bFileId));
        }
    }
    return PH_ADD_COMPCODE(statusTmp, PH_COMP_AL_MFDFLIGHT);
}

phStatus_t phalMfdfLight_Sw_IsoReadBinary(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t bOffset, uint8_t bSfid, uint32_t dwBytesToRead,
    uint8_t bExtendedLenApdu, uint8_t ** ppRxBuffer, uint32_t * pBytesRead)
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wOffset;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pLePtr=NULL;

    if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_DEFAULT)
    {
        if (bSfid & 0x80)
        {
#ifdef RDR_LIB_PARAM_CHECK
            /* Short file id is supplied */
            if ((bSfid & 0x7FU) > 0x1F)
            {
                /* Error condition */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
            }
#endif /* RDR_LIB_PARAM_CHECK */
            bP1 = bSfid;
            bP2 = bOffset;
        }
        else
        {
            /* P1 and P2 code the offset */
            wOffset = bP1 = bSfid;
            wOffset <<= 8; /* Left shift */
            wOffset |= bOffset;
            bP2 = bOffset;
        }
        pLePtr = (uint8_t *) &dwBytesToRead;
        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_ISO7816_READ_BINARY; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        /* Check whether Length Le should be represented in Short APDU or extended length APDU */
        if(bExtendedLenApdu == 0x01)
        {
            /*
             * An extended Le field consists of either three bytes (one * byte set to '00' followed by two bytes with any
             * value) if the Lc field is absent, or two bytes (with any * value) if an extended Lc field is present.
             * From '0001' to 'FFFF', the two bytes encode Ne from one
             * to 65 535.
             * If the two bytes are set to '0000', then Ne is 65 536.
             */
             bCmdBuff[wCmdLen++] = 0x00;
             bCmdBuff[wCmdLen++] = *(pLePtr + 1);
             bCmdBuff[wCmdLen++] = *(pLePtr);
             /* Need to handle the case where the expected data to be read is more than 0xFFFF */
        }
        else
        {
            /* Short APDU */
            bCmdBuff[wCmdLen++] = *(pLePtr);
        }
    }
    else if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }

    return phalMfdfLight_Sw_Int_IsoRead(
        pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );
}

phStatus_t phalMfdfLight_Sw_IsoUpdateBinary(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bOffset, uint8_t bSfid, uint8_t bExtendedLenApdu, uint8_t * pData,
    uint32_t dwDataLen)

{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint32_t    PH_MEMLOC_REM bLc = 0;
    uint16_t    PH_MEMLOC_REM wOffset;
    phStatus_t  PH_MEMLOC_REM status;

    if (bSfid & 0x80)
    {
#ifdef RDR_LIB_PARAM_CHECK
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1F)
        {
            /* Error condition */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
        }
#endif /* RDR_LIB_PARAM_CHECK */
        bP1 = bSfid;
        bP2 = bOffset;
    }
    else
    {
        /* P1 and P2 code the offset */
        wOffset = bP1 = bSfid;
        wOffset <<= 8; /* Left shift */
        wOffset |= bOffset;
        bP2 = bOffset;
    }

    bLc = dwDataLen;
    status = phalMfdfLight_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDFLIGHT_CMD_ISO7816_UPDATE_BINARY,
        bP1,
        bP2,
        bLc,
        bExtendedLenApdu,
        pData,
        0x00,
        NULL,
        NULL
        );

    if (status != PH_ERR_SUCCESS)
    {
        /* Reset authentication status */
        phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);
    }

    return status;
}

phStatus_t phalMfdfLight_Sw_ReadSign(phalMfdfLight_Sw_DataParams_t * pDataParams, uint8_t bAddr, uint8_t ** pSignature)
{

    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[2];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* build command frame */
    bCmdBuff[wCmdLen++] = PHAL_MFDFLIGHT_CMD_READ_SIG;
    bCmdBuff[wCmdLen++] = bAddr;

    /* Req spec(ver 0.14 says),
    * 1. Cmd.Read_Sig shall return the NXPOriginalitySignature as written during wafer test in plain if not authenticated
    * 2. Cmd.Read_Sig shall require MACed command if authenticated.
    */
    if(pDataParams->bAuthMode == PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Plain(
            pDataParams,
            PHAL_MFDFLIGHT_COMMUNICATION_PLAIN,
            bCmdBuff,
            wCmdLen,
            pSignature,
            &wRxLength
            ));
    }
#ifdef NXPBUILD__PH_CRYPTOSYM
    else
    {
        /* Set the expected data length as 56 bytes */
        pDataParams->dwPayLoadLen = PHAL_MFDFLIGHT_SIG_LENGTH;

        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdfLight_Sw_Int_ReadData_Enc(
            pDataParams,
            PHAL_MFDFLIGHT_COMMUNICATION_ENC,
            bCmdBuff,
            wCmdLen,
            pSignature,
            &wRxLength
            ));
    }
#endif /* NXPBUILD__PH_CRYPTOSYM */
    /* check received length :- 56 byte signature */
    if (wRxLength != PHAL_MFDFLIGHT_SIG_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDFLIGHT);
    }

    return PH_ERR_SUCCESS;

}

phStatus_t phalMfdfLight_Sw_GetConfig(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wConfig, uint16_t * pValue)
{
    switch (wConfig)
    {
    case PHAL_MFDFLIGHT_ADDITIONAL_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;

    case PHAL_MFDFLIGHT_WRAPPED_MODE:
        *pValue = (uint16_t)pDataParams->bWrappedMode;
        break;

    case PHAL_MFDFLIGHT_SHORT_LENGTH_APDU:
        *pValue = (uint16_t)pDataParams->bShortLenApdu;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_SetConfig(phalMfdfLight_Sw_DataParams_t * pDataParams, uint16_t wConfig, uint16_t wValue)
{
    switch (wConfig)
    {
    case PHAL_MFDFLIGHT_ADDITIONAL_INFO:
        pDataParams->wAdditionalInfo = wValue;
        break;

    case PHAL_MFDFLIGHT_WRAPPED_MODE:
        pDataParams->bWrappedMode = (uint8_t)wValue;
        break;
    case PHAL_MFDFLIGHT_SHORT_LENGTH_APDU:
        pDataParams->bShortLenApdu = (uint8_t)wValue;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDFLIGHT);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_ResetAuthentication(phalMfdfLight_Sw_DataParams_t * pDataParams)
{
    phStatus_t PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM pValue;

    /*Do a Get Config of  ADDITIONAL_INFO to check the value for 0xFFFF*/
    status = phalMfdfLight_Sw_GetConfig((phalMfdfLight_Sw_DataParams_t *)pDataParams, PHAL_MFDFLIGHT_ADDITIONAL_INFO, &pValue);
    if(pValue == 0xFFFF)
        memset(pDataParams->pAid, 0x00, 0x03);
    phalMfdfLight_Sw_Int_ResetAuthStatus(pDataParams);

    return status;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfdfLight_Sw_CalculateTMV(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid, uint8_t bUidLen, uint8_t * pTMI, uint32_t dwTMILen, uint8_t * pTMV)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLen = 0;
    uint8_t     PH_MEMLOC_REM bKeyLen = 0;
    uint8_t     PH_MEMLOC_REM bSVMacLen = 0;
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint16_t    PH_MEMLOC_REM wTmpTMILen;
    uint16_t    PH_MEMLOC_REM bLoopData = 1;
    uint8_t     PH_MEMLOC_REM bTmpIV[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bSV[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint32_t    PH_MEMLOC_REM dwTMC = 0;
    uint32_t    PH_MEMLOC_REM dwTMCtemp = 0;
    uint32_t    PH_MEMLOC_REM dwTMILenWritten = 0;
    uint16_t    PH_MEMLOC_REM wCommMode;

    {
        /*OLD this block is replaced by below for Qmore compliance
        dwTMC |= (uint32_t)pTMC[3];
        dwTMC |= (uint32_t)(pTMC[2] << 8);
        dwTMC |= (uint32_t)(pTMC[1] << 16);
        dwTMC |= (uint32_t)(pTMC[0] << 24); */

        /* Formation of TMC as double word value- TMC shall be communicated LSB first. */
        dwTMC = pTMC[0];
        dwTMCtemp = pTMC[1];
        dwTMC |= (dwTMCtemp << 8);
        dwTMCtemp = pTMC[2];
        dwTMC |= (dwTMCtemp << 16);
        dwTMCtemp = pTMC[3];
        dwTMC |= (dwTMCtemp << 24);

#ifdef RDR_LIB_PARAM_CHECK
        /* If TMC is 0xFFFFFFFF, then return error */
        if (dwTMC == 0xFFFFFFFF)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_AL_MFDFLIGHT);
        }

        if ((wOption != PHAL_MFDFLIGHT_NO_DIVERSIFICATION) && (bDivInputLen > 31))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDFLIGHT);
        }
#endif /* RDR_LIB_PARAM_CHECK */
        /* Get Key out of the key store object */
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
            pDataParams->pKeyStoreDataParams,
            wKeyNoTMACKey,
            wKeyVerTMACKey,
            PH_CRYPTOSYM_AES128_KEY_SIZE,
            bKey,
            &wKeyType
            ));

        /* Invalid key type at wKeyNoTMACKey and wKeyVerTMACKey */
        if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFDFLIGHT);
        }

        if ((wOption != PHAL_MFDFLIGHT_NO_DIVERSIFICATION) && (bDivInputLen != 0x00))
        {
            /* Key is diversified and put back in bKey */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wOption,
                bKey,
                wKeyType,
                pDivInput,
                bDivInputLen,
                bKey
                ));
        }

        /* Increment dwTMC */
        dwTMC++;

        /* Update bSV buffer SV = 0x5A || 0x00 || 0x01 || 0x00 || 0x80 ||(TMC + 1) || UID [||0x00::0x00] */
        bSV[bSVMacLen++] = 0x5A;
        bSV[bSVMacLen++] = 0x00;
        bSV[bSVMacLen++] = 0x01;
        bSV[bSVMacLen++] = 0x00;
        bSV[bSVMacLen++] = 0x80;
        bSV[bSVMacLen++] = (uint8_t)(dwTMC & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 8) & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 16) & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 24) & 0xFF);

        /* Copy UID into SV - UID should not more then 7 byte in
        * this case else bSV array size needs to be changed
        */
        memcpy(&bSV[bSVMacLen], pUid, bUidLen); /* PRQA S 3200 */

        bSVMacLen += bUidLen;

        /* SV padded with the zero bytes up to a length of 16 bytes (if needed)*/
        if(bSVMacLen < PH_CRYPTOSYM_AES128_KEY_SIZE)
        {
            memset(&bSV[bSVMacLen], 0x00, (PH_CRYPTOSYM_AES128_KEY_SIZE - bSVMacLen)); /* PRQA S 3200 */
        }

        /* load key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            bKey,
            wKeyType));

        /* Create a Back up of the current IV */
        memcpy(bTmpIV, pDataParams->bIv, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

        /* Load zero to IV */
        memset(pDataParams->bIv, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE);  /* PRQA S 3200 */

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

        /* Encrypt SV to obtain SesTMMACKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsEnc,
            (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
            bSV,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            bKey,
            &bKeyLen
            ));

        /* Now calculate TMV as TMV = MACtTM(KSesTMMAC; TMI) */
        /* load key -SesTMMACKey*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsMac,
            bKey,
            PH_CRYPTOSYM_KEY_TYPE_AES128));

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

        /* to handle 2 MB of data update maximum of data bytes that can be sent in a single loop */
        if(dwTMILen > PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
        {
            wTmpTMILen = (uint16_t) PHAL_MFDFLIGHT_MAX_WRITE_SIZE;
            bLoopData = (uint8_t)(dwTMILen/PHAL_MFDFLIGHT_MAX_WRITE_SIZE);
            if(dwTMILen % PHAL_MFDFLIGHT_MAX_WRITE_SIZE)
            {
                bLoopData++;
            }
            wCommMode = PH_EXCHANGE_BUFFER_FIRST;
        }
        else
        {
            wTmpTMILen = (uint16_t)dwTMILen;
            wCommMode = PH_EXCHANGE_DEFAULT;
        }

        do{

            if (bLoopData == 1)
            {
                wTmpTMILen = (uint16_t)(dwTMILen - dwTMILenWritten);
            }
            /* Encrypt TMI to obtain TMV */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | wCommMode),
                &pTMI[dwTMILenWritten],
                wTmpTMILen,
                bCMAC,
                &bMacLen
                ));
            bLoopData--;
            wCommMode = (bLoopData == 1)?PH_EXCHANGE_BUFFER_LAST:PH_EXCHANGE_BUFFER_CONT;
            dwTMILenWritten +=wTmpTMILen;
        }while(bLoopData > 0);

        if(pDataParams->bAuthMode != PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
        {
            /* Load the session key */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
                pDataParams->pCryptoDataParamsEnc,
                pDataParams->bSesAuthENCKey,
                pDataParams->bCryptoMethod
                ));

            /* Load the session key */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bSesAuthMACKey,
                pDataParams->bCryptoMethod
                ));
        }
    }
    /* Truncate the Calculated CMAC */
    phalMfdfLight_Sw_Int_TruncateMac(bCMAC);

    /* Copy the Truncated CMAC into the return buffer */
    memcpy(pTMV, bCMAC, PHAL_MFDFLIGHT_TRUNCATED_MAC_SIZE); /* PRQA S 3200 */

    /* Restore back the IV */
    memcpy(pDataParams->bIv, bTmpIV, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfdfLight_Sw_DecryptReaderID(phalMfdfLight_Sw_DataParams_t *pDataParams, uint16_t wOption, uint16_t wKeyNoTMACKey, uint16_t wKeyVerTMACKey,
    uint8_t * pDivInput, uint8_t bDivInputLen, uint8_t * pTMC, uint8_t * pUid, uint8_t bUidLen, uint8_t * pEncTMRI, uint8_t * pTMRIPrev)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bSVMacLen = 0;
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bTmpIV[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bSV[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint32_t    PH_MEMLOC_REM dwTMC = 0;
    uint32_t    PH_MEMLOC_REM dwTMCtemp = 0;

    {

        /* Formation of TMC as double word value- TMC shall be communicated LSB first. */
        dwTMC = pTMC[0];
        dwTMCtemp = pTMC[1];
        dwTMC |= (dwTMCtemp << 8);
        dwTMCtemp = pTMC[2];
        dwTMC |= (dwTMCtemp << 16);
        dwTMCtemp = pTMC[3];
        dwTMC |= (dwTMCtemp << 24);


        /* Get Key out of the key store object */
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
            pDataParams->pKeyStoreDataParams,
            wKeyNoTMACKey,
            wKeyVerTMACKey,
            PH_CRYPTOSYM_AES128_KEY_SIZE,
            bKey,
            &wKeyType
            ));

        /* Invalid key type at wKeyNoTMACKey and wKeyVerTMACKey */
        if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFDFLIGHT);
        }

        if ((wOption != PHAL_MFDFLIGHT_NO_DIVERSIFICATION) && (bDivInputLen != 0x00))
        {
            /* Key is diversified and put back in bKey */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
                pDataParams->pCryptoDataParamsEnc,
                wOption,
                bKey,
                wKeyType,
                pDivInput,
                bDivInputLen,
                bKey
                ));
        }

        /* Increment dwTMC */
        dwTMC++;
        /* Update bSV buffer SV = 0xA5 || 0x00 || 0x01 || 0x00 || 0x80 ||(TMC + 1) || UID [||0x00::0x00] */
        bSV[bSVMacLen++] = 0xA5;
        bSV[bSVMacLen++] = 0x00;
        bSV[bSVMacLen++] = 0x01;
        bSV[bSVMacLen++] = 0x00;
        bSV[bSVMacLen++] = 0x80;
        bSV[bSVMacLen++] = (uint8_t)(dwTMC & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 8) & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 16) & 0xFF);
        bSV[bSVMacLen++] = (uint8_t)((dwTMC >> 24) & 0xFF);

        /* Copy UID into SV - UID should not more then 7 byte in
        * this case else bSV array size needs to be changed
        */
        memcpy(&bSV[bSVMacLen], pUid, bUidLen); /* PRQA S 3200 */

        bSVMacLen += bUidLen;

        /* SV padded with the zero bytes up to a length of 16 bytes (if needed)*/
        if(bSVMacLen < PH_CRYPTOSYM_AES128_KEY_SIZE)
        {
            memset(&bSV[bSVMacLen], 0x00, (PH_CRYPTOSYM_AES128_KEY_SIZE - bSVMacLen)); /* PRQA S 3200 */
        }

        /* load key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            bKey,
            wKeyType));

        /* Create a Back up of the current IV */
        memcpy(bTmpIV, pDataParams->bIv, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

        /* Load zero to IV */
        memset(pDataParams->bIv, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE);  /* PRQA S 3200 */

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

        /* Encrypt SV to obtain SesTMMACKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsEnc,
            (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
            bSV,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            bKey,
            &bSVMacLen
            ));

        /* Now calculate TMV as TMV = MACtTM(KSesTMMAC; TMI) */
        /* load key -SesTMMACKey*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            bKey,
            PH_CRYPTOSYM_KEY_TYPE_AES128));

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_DEFAULT,
            pEncTMRI,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            pTMRIPrev
            ));

        /* Restore back the IV */
        memcpy(pDataParams->bIv, bTmpIV, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

        if(pDataParams->bAuthMode != PHAL_MFDFLIGHT_NOT_AUTHENTICATED)
        {
            /* Load the session key */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
                pDataParams->pCryptoDataParamsEnc,
                pDataParams->bSesAuthENCKey,
                pDataParams->bCryptoMethod
                ));
            /* Load the session key */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bSesAuthMACKey,
                pDataParams->bCryptoMethod
                ));
        }

        return PH_ERR_SUCCESS;
    }
}
#endif /*NXPBUILD__PH_CRYPTOSYM */

#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */

