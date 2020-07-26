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
* $Revision: 1505 $ (v06.01.00)
* $Date: 2016-01-11 14:51:50 +0530 (Mon, 11 Jan 2016) $
*
*/

#ifndef PHALMFNTAG42XDNA_H
#define PHALMFNTAG42XDNA_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>
#include <ph_TypeDefs.h>
#include <ph_RefDefs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************************************************************************************************************/
/* Software Dataparams and Initialization Interface.                                                                                   */
/***************************************************************************************************************************************/
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW

/**
 * \defgroup phalMfNtag42xDna_Sw Component : Software
 * @{
 */

#define PHAL_MFNTAG42XDNA_SW_ID                                         0x01U   /**< ID for Software MIFARE NTAG 42XDNA layer. */

/** \name Sizes */
/** @{ */
#define PHAL_MFNTAG42XDNA_SIZE_TI                                       4U      /**< Size of Transaction Identifier. */
#define PHAL_MFNTAG42XDNA_SIZE_MAC                                      16U     /**< Size of (untruncated) MAC. */
#define PHAL_MFNTAG42XDNA_BLOCK_SIZE                                    16U     /**< Block Size */
#define PHAL_MFNTAG42XDNA_MAX_WRITE_SIZE                                0xFFFDU /**< Max size in a Write function. */
/** @} */

/** \brief MIFARE NTAG 42xDNA Software parameter structure  */
typedef struct
{
    uint16_t wId;                                                               /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                                                /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                                                 /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParamsEnc;                                                /**< Pointer to the parameter structure of the Crypto layer for encryption. */
    void * pCryptoDataParamsMac;                                                /**< Pointer to the parameter structure of the CryptoMAC. */
    void * pCryptoRngDataParams;                                                /**< Pointer to the parameter structure of the CryptoRng layer. */
    void * pHalDataParams;                                                      /**< Pointer to the HAL parameters structure. */
    uint8_t bSesAuthENCKey[24];                                                 /**< Session key for this authentication */
    uint8_t bKeyNo;                                                             /**< key number against which this authentication is done */
    uint8_t bIv[16];                                                            /**< Max size of IV can be 16 bytes */
    uint8_t bAuthMode;                                                          /**< Authenticate (0x0A), AuthISO (0x1A), AuthAES (0xAA) */
    uint8_t pAid[3];                                                            /**< Aid of the currently selected application */
    uint8_t bCryptoMethod;                                                      /**< DES,3DES, 3K3DES or AES, LRP  */
    uint8_t bWrappedMode;                                                       /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 APDUs. */
    uint16_t wCrc;                                                              /**< 2 Byte CRC initial value in Authenticate mode. */
    uint32_t dwCrc;                                                             /**< 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    uint16_t wAdditionalInfo;                                                   /**< Specific error codes for Desfire generic errors. */
    uint32_t dwPayLoadLen;                                                      /**< Amount of data to be read. Required for Enc read to verify CRC. */
    uint16_t wCmdCtr;                                                           /**< Command count within transaction. */
    uint8_t bTi[PHAL_MFNTAG42XDNA_SIZE_TI];                                     /**< Transaction Identifier. */
    uint8_t bSesAuthMACKey[16];                                                 /**< Authentication MAC key for the session. */
    uint8_t bKeySessionAuthMaster[16];                                          /**< Session Auth master key. */
    uint8_t pUnprocByteBuff[PHAL_MFNTAG42XDNA_SIZE_MAC];                        /**< Buffer containing unprocessed bytes for read mac answer stream. */
    uint8_t bNoUnprocBytes;                                                     /**< Amount of data in the pUnprocByteBuff. */
    uint8_t bLastBlockBuffer[16];                                               /**< Buffer to store last Block of encrypted data in case of chaining. */
    uint8_t bLastBlockIndex;                                                    /**< Last Block Buffer Index. */
    uint8_t bShortLenApdu;                                                      /**< Parameter for force set Short Length APDU in case of BIG ISO read. */
    uint8_t bKeyType;                                                           /**< Distinguish between the LRP_AES or AES key. */
} phalMfNtag42XDna_Sw_DataParams_t;

/**
 * \brief Initialise this layer.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phalMfNtag42XDna_Sw_Init (
        phalMfNtag42XDna_Sw_DataParams_t * pDataParams,                         /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wSizeOfDataParams,                                             /**< [In] Specifies the size of the data parameter structure */
        void * pPalMifareDataParams,                                            /**< [In] Pointer to a palMifare component context. */
        void * pKeyStoreDataParams,                                             /**< [In] Pointer to Key Store data parameters. */
        void * pCryptoDataParamsEnc,                                            /**< [In] Pointer to a Crypto component context for encryption. */
        void * pCryptoDataParamsMac,                                            /**< [In] Pointer to a CryptoMAC component context. */
        void * pCryptoRngDataParams,                                            /**< [In] Pointer to a CryptoRng component context. */
        void * pHalDataParams                                                   /**< [In] Pointer to the HAL parameters structure. */
    );

/**
 * end of group phalMfNtag42xDna_Sw
 * @}
 */

#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */


/***************************************************************************************************************************************/
/* MIFARE NTAG 42xDNA Generic command declarations.                                                                                    */
/***************************************************************************************************************************************/
#ifdef NXPBUILD__PHAL_MFNTAG42XDNA

/** \defgroup phalMfNtag42xDna NTAG 42x DNA / TT
 * \brief These Functions implement the NTAG 42x DNA / TT commands.
 * @{
 */
/** \defgroup phalMfNtag42xDna_Cust_Err_Codes Error Codes
 * @{
 */

/** \name Error Codes mapping to custom values. */
/* @{ */
#define PHAL_MFNTAG42XDNA_ERR_FORMAT                (PH_ERR_CUSTOM_BEGIN + 0)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_FORMAT. */
#define PHAL_MFNTAG42XDNA_ERR_OUT_OF_EEPROM_ERROR   (PH_ERR_CUSTOM_BEGIN + 1)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_OUT_OF_EEPROM_ERROR. */
#define PHAL_MFNTAG42XDNA_ERR_NO_SUCH_KEY           (PH_ERR_CUSTOM_BEGIN + 2)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_NO_SUCH_KEY. */
#define PHAL_MFNTAG42XDNA_ERR_PERMISSION_DENIED     (PH_ERR_CUSTOM_BEGIN + 3)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_PERMISSION_DENIED. */
#define PHAL_MFNTAG42XDNA_ERR_APPLICATION_NOT_FOUND (PH_ERR_CUSTOM_BEGIN + 4)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_APPLICATION_NOT_FOUND. */
#define PHAL_MFNTAG42XDNA_ERR_BOUNDARY_ERROR        (PH_ERR_CUSTOM_BEGIN + 5)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_BOUNDARY_ERROR. */
#define PHAL_MFNTAG42XDNA_ERR_COMMAND_ABORTED       (PH_ERR_CUSTOM_BEGIN + 6)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_COMMAND_ABORTED. */
#define PHAL_MFNTAG42XDNA_ERR_COUNT                 (PH_ERR_CUSTOM_BEGIN + 7)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_COUNT. */
#define PHAL_MFNTAG42XDNA_ERR_DUPLICATE             (PH_ERR_CUSTOM_BEGIN + 8)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_DUPLICATE. */
#define PHAL_MFNTAG42XDNA_ERR_FILE_NOT_FOUND        (PH_ERR_CUSTOM_BEGIN + 9)   /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_FILE_NOT_FOUND. */
#define PHAL_MFNTAG42XDNA_ERR_PICC_CRYPTO           (PH_ERR_CUSTOM_BEGIN + 10)  /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_INTEGRITY_ERROR. */
#define PHAL_MFNTAG42XDNA_ERR_PARAMETER_ERROR       (PH_ERR_CUSTOM_BEGIN + 11)  /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_PARAMETER_ERROR. */
#define PHAL_MFNTAG42XDNA_ERR_DF_GEN_ERROR          (PH_ERR_CUSTOM_BEGIN + 12)  /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_APPL_INTEGRITY_ERROR,
                                                                                 *   #PHAL_MFNTAG42XDNA_RESP_ERR_PICC_INTEGRITY, #PHAL_MFNTAG42XDNA_RESP_ERR_EEPROM,
                                                                                 *   #PHAL_MFNTAG42XDNA_RESP_ERR_FILE_INTEGRITY, #PHAL_MFNTAG42XDNA_RESP_ERR_PIC_DISABLED,
                                                                                 *   #PHAL_MFNTAG42XDNA_RESP_ILLEGAL_COMMAND_CODE
                                                                                 */
#define PHAL_MFNTAG42XDNA_ERR_DF_7816_GEN_ERROR     (PH_ERR_CUSTOM_BEGIN + 13)  /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LENGTH,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_PARAMS, #PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LC,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LE, #PHAL_MFNTAG42XDNA_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_EOF_REACHED, #PHAL_MFNTAG42XDNA_ISO7816_ERR_LIMITED_FUNCTIONALITY_INS,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_ACCESS, #PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_EMPTY,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_NOT_FOUND, #PHAL_MFNTAG42XDNA_ISO7816_ERR_MEMORY_FAILURE,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_INCORRECT_PARAMS, #PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_CLA,
                                                                                 *   #PHAL_MFNTAG42XDNA_ISO7816_ERR_UNSUPPORTED_INS
                                                                                 */
#define PHAL_MFNTAG42XDNA_ERR_CMD_INVALID           (PH_ERR_CUSTOM_BEGIN + 14)  /**< Custom error code for PICC's #PHAL_MFNTAG42XDNA_RESP_ERR_CMD_INVALID. */
/* @} */
/** @} */

/** \name Other Options for various Functions */
/** @{ */
#define PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_FIRST                         0x01U   /**< Originality key for AES */
#define PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_LAST                          0x04U   /**< Originality key for AES  */
#define PHAL_MFNTAG42XDNA_APP_KEY_LAST                                  0x04U   /**< Numebr of application keys. */
#define PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE                           0x01U   /**< Option for indicating more data to come in next call for MAC calculation */
#define PHAL_MFNTAG42XDNA_COMMUNICATION_MAC_ON_RC                       0x02U   /**< MAC is appended over response  */
#define PHAL_MFNTAG42XDNA_AUTHENTICATE_RESET                            0x08U   /**< authentication state shall be reset */
/** @} */

/** \name The communication mode to be used. */
/** @{ */
#define PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN                           0x00U   /**< Plain mode of communication. */
#define PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN_1                         0x20U   /**< Plain mode of communication. */
#define PHAL_MFNTAG42XDNA_COMMUNICATION_MACD                            0x10U   /**< MAC mode of communication. */
#define PHAL_MFNTAG42XDNA_COMMUNICATION_ENC                             0x30U   /**< Enciphered mode of communication. */
/** @} */

/** \name Key Types */
/** @{ */
#define PHAL_MFNTAG42XDNA_KEY_TYPE_2K3DES                               0x00U   /**< 2 Key Triple Des */
#define PHAL_MFNTAG42XDNA_KEY_TYPE_3K3DES                               0x01U   /**< 3 Key Triple Des */
#define PHAL_MFNTAG42XDNA_KEY_TYPE_AES128                               0x02U   /**< AES 128 Key */
/** @} */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA secure messaging related commands. ----------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_SecureMessaging Commands_SecureMessaging
 * \brief These Components implement the MIFARE MFNTAG42XDNA Secure Messaging related commands.
 * @{
 */

/** \name Authentication Modes */
/** @{ */
#define PHAL_MFNTAG42XDNA_NOT_AUTHENTICATED                             0xFFU   /**< No authentication. */
#define PHAL_MFNTAG42XDNA_AUTHENTICATE                                  0x0AU   /**< D40 Authentication; 0x0A. */
#define PHAL_MFNTAG42XDNA_AUTHENTICATEISO                               0x1AU   /**< ISO Authentication; 0x1A. */
#define PHAL_MFNTAG42XDNA_AUTHENTICATEAES                               0xAAU   /**< AES Authentication; 0xAA. */
#define PHAL_MFNTAG42XDNA_AUTHENTICATEEV2                               0x71U   /**< EV2 First Authentication; 0x71. */
#define PHAL_MFNTAG42XDNA_AUTH_TYPE_EV2                                 0x01U   /**< Auth Type as EV2. */
#define PHAL_MFNTAG42XDNA_AUTH_TYPE_LRP                                 0x02U   /**< Auth Type as LRP. */
#define PHAL_MFNTAG42XDNA_AUTH_TYPE_UNKNOWN                             0xFFU   /**< Auth Type as Unknown. */
#define PHAL_MFNTAG42XDNA_CMD_AUTHENTICATE_PART2                        0x77U   /**< EV2 NonFirst Authenticate; 0x77. */
#define PHAL_MFNTAG42XDNA_AUTHNONFIRST_NON_LRP                          0x0000U /**< Non First Auth in regular EV2 auth Mode. */
#define PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP                             0x0001U /**< First Auth in regular EV2 auth Mode. */
#define PHAL_MFNTAG42XDNA_AUTHNONFIRST_LRP                              0x0002U /**< Non First Auth in LRP mode. */
#define PHAL_MFNTAG42XDNA_AUTHFIRST_LRP                                 0x0003U /**< First Auth in LRP mode. */
/** @} */

/** \name Diversification options to be used with Authenticate command. */
/** @{ */
#define PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION                                0xFFFFU /**< No diversification. */
#define PHAL_MFNTAG42XDNA_DIV_METHOD_ENCR           PH_CRYPTOSYM_DIV_MODE_DESFIRE   /**< Encryption based method of diversification. */
#define PHAL_MFNTAG42XDNA_DIV_METHOD_CMAC       PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS   /**< CMAC based method of diversification. */
/** @} */

/**
 * \brief Performs an First or Non First Authentication depending upon bFirstAuth Parameter. This will be using the AES128 keys and will
 * generate and verify the contents based on generic AES algorithm.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_AuthenticateEv2 (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFirstAuth,                                                     /**< [In] One of the below options.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_AUTHNONFIRST_NON_LRP
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_AUTHNONFIRST_LRP
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_AUTHFIRST_LRP
                                                                                 */
        uint16_t wOption,                                                       /**< [In] Diversification option can be one of
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_DIV_METHOD_ENCR
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_DIV_METHOD_CMAC
                                                                                 */
        uint16_t wKeyNo,                                                        /**< [In] Key number in keystore of software or SAM. */
        uint16_t wKeyVer,                                                       /**< [In] Key version in keystore of software or SAM. */
        uint8_t bKeyNoCard,                                                     /**< [In] Key number on card. */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input. Can be NULL. */
        uint8_t bDivLen,                                                        /**< [In] Length of diversification input max 31B. */
        uint8_t bLenPcdCapsIn,                                                  /**< [In] Length of PcdCapsIn. Always zero for following authentication */
        uint8_t * bPcdCapsIn,                                                   /**< [In] PCD Capabilities. */
        uint8_t * bPcdCapsOut,                                                  /**< [Out] PCD Capabilities. */
        uint8_t * bPdCapsOut                                                    /**< [Out] PD Capabilities. */
    );

/**
 * end of group phalMfNtag42xDna_SecureMessaging
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA Memory and Configuration mamangement commands. ----------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_MemoryConfiguration Commands_MemoryConfiguration
 * \brief These Components implement the MIFARE NTAG 42xDNA Memory and Configuration Management related commands.
 * @{
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/** \name Options for SetConfiguration command. */
/** @{ */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION0                            0x00U   /**< Option 0 for PICC configuration. */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION4                            0x04U   /**< Option 4 for Secure Messaging Configuration. */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION5                            0x05U   /**< Option 5 for Capibility Data. */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION7                            0x07U   /**< Option 7 for Tag Tamper configuration. */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION10                           0x0AU   /**< Option 10 for Failed Authentication Counter Configuration. */
#define PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION11                           0x0BU   /**< Option 11 for Hardware Configuration. */
/** @} */

/**
 * \brief Configures the card and pre personalizes the card with a key, defines if the UID or the random ID is sent back during communication setup and
 * configures the ATS string.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_SetConfiguration (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Configuration Option. Define length and content of the Data parameter.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION0
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION4
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION5
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION7
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION10
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SET_CONFIG_OPTION11
                                                                                 */
        uint8_t * pData,                                                        /**< [In] Data for the option specified.*/
        uint8_t bDataLen                                                        /**< [In] Length of bytes available in Data buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Returns manufacturing related data of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetVersion (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pVerInfo                                                      /**< [Out] The version information of the product. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Returns the Unique ID of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetCardUID (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pUid                                                          /**< [Out] The complete UID of the PICC. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalMfNtag42xDna_MemoryConfiguration
 * @}
 */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Key mamangement commands. -------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_KeyManagement Commands_KeyManagement
 * \brief These Components implement the MIFARE NTAG 42xDNA Key Management related commands.
 * @{
 */

/** \name Diversification options to be used with ChangeKey command. */
/** @{ */
#define PHAL_MFNTAG42XDNA_CHGKEY_NO_DIVERSIFICATION                     0xFFFFU /**< No diversification. */
#define PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY                            0x0002U /**< Bit 1. Indicating diversification of new key requred. */
#define PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY                            0x0004U /**< Bit 2 indicating old key was diversified. */
#define PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY_ONERND                     0x0008U /**< Bit 3 indicating new key diversification using one rnd. Default is two rounds. */
#define PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY_ONERND                     0x0010U /**< Bit 4 indicating old key diversification using one rnd. Default is two rounds. */
#define PHAL_MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC                        0x0020U /**< Bit 5 indicating key diversification method based on CMAC. Default is Encryption method */
/** @} */

/**
 * \brief Changes any key on the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_ChangeKey (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                       /**< [In] One of the below options.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY_ONERND
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY_ONERND
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY |
                                                                                 *               #PHAL_MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY |
                                                                                 *               #PHAL_MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY_ONERND | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY_ONERND
                                                                                 */
        uint16_t wOldKeyNo,                                                     /**< [In] Old key number in keystore of software or SAM. */
        uint16_t wOldKeyVer,                                                    /**< [In] Old key number in keystore of software or SAM. */
        uint16_t wNewKeyNo,                                                     /**< [In] New key number in keystore of software or SAM. */
        uint16_t wNewKeyVer,                                                    /**< [In] New key number in keystore of software or SAM. */
        uint8_t bKeyNoCard,                                                     /**< [In] Key number on card. */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input. Can be NULL. */
        uint8_t bDivLen                                                         /**< [In] Length of diversification input max 31B. */
    );

/**
 * \brief Reads out the current key version of any key stored on the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetKeyVersion (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bKeyNo,                                                         /**< [In] Key number on card. */
        uint8_t bKeySetNo,                                                      /**< [In] 1 byte Key set number. Optional as it is passed only when bit[6] of bKeyNo is set. */
        uint8_t * pKeyVersion,                                                  /**< [Out] The version of the specified key. */
        uint8_t * bRxLen                                                        /**< [Out] Length of bytes available in KeyVersion buffer. */
    );

/**
 * end of group phalMfNtag42xDna_KeyManagement
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */




/* MIFARE NTAG 42xDNA File mamangement commands. ------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_FileManagement Commands_FileManagement
 * \brief These Components implement the MIFARE NTAG 42xDNA File Management related commands.
 * @{
 */

/**
 * \brief Get informtion on the properties of a specific file
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetFileSettings (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo,                                                        /**< [In] The file number for which the setting to be retrieved. */
        uint8_t * pFSBuffer,                                                    /**< [Out] The buffer containing the settings. The buffer should be 17 bytes. */
        uint8_t * bBufferLen                                                    /**< [Out] The length of bytes available in FSBuffer buffer. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Returns manufacturing related data of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetFileCounters (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Indicates the mode of communication to be used while exchanging the data to PICC.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] File number for which the Counter information need to be received. */
        uint8_t * pFileCounters,                                                /**< [Out] The SDMReadCounter information returned by the PICC. */
        uint8_t * pRxLen                                                        /**< [Out] Length of bytes available in FileCounters buffer. */
    );

/** \name The File option and other flags for ChangeFileSettings command. */
/** @{ */
#define PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN                             0x00U   /**< Plain mode of communication. */
#define PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN_1                           0x02U   /**< Plain mode of communication. */
#define PHAL_MFNTAG42XDNA_FILE_OPTION_MACD                              0x01U   /**< MAC mode of communication. */
#define PHAL_MFNTAG42XDNA_FILE_OPTION_ENC                               0x03U   /**< Enciphered mode of communication. */
#define PHAL_MFNTAG42XDNA_SDM_ENABLED                                   0x40u   /**< Secure Dynamic Messaging and Mirroring is enabled. */

#define PHAL_MFNTAG42XDNA_VCUID_PRESENT                                 0x80U   /**< Only VCUID is considred for SDM MAC calculation */
#define PHAL_MFNTAG42XDNA_RDCTR_PRESENT                                 0x40U   /**< Only RDCTR  is considred for SDM MAC calculation */
#define PHAL_MFNTAG42XDNA_RDCTR_LIMIT_PRESENT                           0x20U   /**< Indicates the presence of SDM Read Counter Limit. */
#define PHAL_MFNTAG42XDNA_SDM_ENC_FILE_DATA_PRESENT                     0x10U   /**< Indicates the presence of SDM ENC File data. */
#define PHAL_MFNTAG42XDNA_SDM_TT_STATUS_PRESENT                         0x08U   /**< Indicates the presence of SDM TT Status. */
#define PHAL_MFNTAG42XDNA_SDM_ENCODING_MODE_ASCII                       0x01U   /**< Indicates the encoding as ASCII. */
#define PHAL_MFNTAG42XDNA_SPECIFICS_ENABLED                             0x01u   /**< MF NTAG 42xDNA Specific feature to  be used. */
/** @} */

/**
 * \brief Changes the access parameters of an existing file.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t  phalMfNtag42XDna_ChangeFileSettings (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommMode,                                                      /**< [In] Indicates the mode of communication to be used while exchanging the data to PICC.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
                                                                                 *        Ored with the below option
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SPECIFICS_ENABLED
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] File number for which the setting need to be updated. */
        uint8_t bFileOption,                                                    /**< [In] New communication settings for the file.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN_1
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_ENC
                                                                                 *        \n
                                                                                 *        Ored with one of the below flags if required.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_ENABLED
                                                                                 */
        uint8_t * pAccessRights,                                                /**< [In] The new access right to be applied for the file. Should be 2 byte. */
        uint8_t bAdditionalInfoLen,                                             /**< [In] Length of the Additional Info to be sent (2 Bytes or 4 Bytes).
                                                                                 *        This is used to set TMC Limit in case of TMAC File
                                                                                 */
        uint8_t * bAdditionalInfo                                               /**< [In] One of the below inforamtion. \n
                                                                                 *          \c If Standard AES: 4 byte TMC Limit value. \n
                                                                                 *          \c If LRP         : 2 byte TMC Limit value.
                                                                                 */
    );

/**
 * \brief Changes the access parameters of an existing file.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t  phalMfNtag42XDna_ChangeFileSettingsSDM (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Indicates the mode of communication to be used while exchanging the data to PICC.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] File number for which the setting need to be updated. */
        uint8_t bFileOption,                                                    /**< [In] New communication settings for the file.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_PLAIN_1
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FILE_OPTION_ENC
                                                                                 *        \n
                                                                                 *        Ored with \arg #PHAL_MFNTAG42XDNA_SDM_ENABLED
                                                                                 */
        uint8_t * pAccessRights,                                                /**< [In] The new access right to be applied for the file. Should be 2 byte. */
        uint8_t bSdmOptions,                                                    /**< [In] One of the below values to be used. Can be ORed.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_VCUID_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_RDCTR_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_RDCTR_LIMIT_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_ENC_FILE_DATA_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_TT_STATUS_PRESENT
                                                                                 *        \n
                                                                                 *        Must be ored with the above values.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_ENCODING_MODE_ASCII
                                                                                 */
        uint8_t * pSdmAccessRights,                                             /**< [In] The SDM access rights to be applied. Should be 2 bytes. */
        uint8_t * pVCUIDOffset,                                                 /**< [In] VCUID Offset information. Should be 3 bytes. */
        uint8_t * pSDMReadCtrOffset,                                            /**< [In] SDMReadCtr value (LSB first). Should be 3 bytes. */
        uint8_t * pPICCDataOffset,                                              /**< [In] Mirror position (LSB first) for encrypted PICCData. Should be 3 bytes. */
        uint8_t * pTTPermStatusOffset,                                          /**< [In] Tag Tamper Permanent Status Offset value. Should be 3 bytes. */
        uint8_t * pSDMMACInputOffset,                                           /**< [In] Offset in the file where the SDM MAC computation starts (LSB first). Should be 3 bytes. */
        uint8_t * pSDMENCOffset,                                                /**< [In] SDMENCFileData mirror position (LSB first). Should be 3 bytes. */
        uint8_t * pSDMENCLen,                                                   /**< [In] Length of the SDMENCFileData (LSB first). Should be 3 bytes. */
        uint8_t * pSDMMACOffset,                                                /**< [In] SDMMAC mirror position (LSB first). Should be 3 bytes. */
        uint8_t * pSDMReadCtrLimit                                              /**< [In] SDM Read Counter Limit value. Should be 3 bytes. */

    );
#endif /* NXPBUILD__PH_CRYPTOSYM */
/**
 * end of group  phalMfNtag42xDna_FileManagement
 * @}
 */




/* MIFARE NTAG 42xDNA Data mamangement commands. ------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_DataManagement Commands_DataManagement
 * \brief These Components implement the MIFARE NTAG 42xDNA Data Management related commands.
 * @{
 */

/**
 * \brief Reads data from standard data files or backup data files
 *
 * \remarks
 *
 * Chaining upto the size of the HAL Rx buffer is handled within this function. If more data is to be read, the user has to call
 * this function again with bOption = #PH_EXCHANGE_RXCHAINING | [one of the communication options]
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_SUCCESS_CHAINING indicating more data to be read.
 * \retval Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_ReadData (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
                                                                                 *
                                                                                 *          \c #PH_EXCHANGE_RXCHAINING: To be Or'd with the above option
                                                                                 *              flag if Chaining status is returned.
                                                                                 */
        uint8_t bIns,                                                           /**< [In] If set, uses ISO 14443-4 chaining instead of DESFire application chaining. */
        uint8_t bFileNo,                                                        /**< [In] The file number from where the data to be read. */
        uint8_t * pOffset,                                                      /**< [In] The offset from where the data should be read. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 need to be offset then it will be 10 00 00.
                                                                                 */
        uint8_t * pLength,                                                      /**< [In] The number of bytes to be read. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be read then it will be 10 00 00. \n
                                                                                 *          \c If complete file need to be read then it will be 00 00 00.
                                                                                 */
        uint8_t ** ppRxdata,                                                    /**< [Out] The data retuned by the PICC. */
        uint16_t * pRxdataLen                                                   /**< [Out] Length of bytes available in RxData buffer. */
    );

/**
 * \brief Writes data to standard data files or backup data files
 *
 * \remarks
 * Implements chaining to the card.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_WriteData (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
                                                                                 */
        uint8_t bIns,                                                           /**< [In] If set, uses ISO 14443-4 chaining instead of DESFire application chaining. */
        uint8_t bFileNo,                                                        /**< [In] The file number from where the data to be read. */
        uint8_t * pOffset,                                                      /**< [In] The offset from where the data should be written. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 need to be offset then it will be 10 00 00.
                                                                                 */
        uint8_t * pTxData,                                                      /**< [In] The data to be written to the PICC. */
        uint8_t * pTxDataLen                                                    /**< [In] The number of bytes to be written. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be written then it will be 10 00 00.
                                                                                 */
    );

/**
 * end of group phalMfNtag42xDna_DataManagement
 * @}
 */




/* MIFARE NTAG 42xDNA ISO7816-4 commands. -------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_ISO7816 Commands_ISO7816
 * \brief These Components implement the MIFARE NTAG 42xDNA ISO/IEC 7816-4 Basic commands.
 * @{
 */

/** \name Options for ISOSelectFile command. */
/** @{ */
#define PHAL_MFNTAG42XDNA_FCI_RETURNED                                  0x00U   /**< Option to indicate the return of FCI. */
#define PHAL_MFNTAG42XDNA_FCI_NOT_RETURNED                              0x0CU   /**< Option to indicate the no return of FCI. */
#define PHAL_MFNTAG42XDNA_SELECTOR_0                                    0x00U   /**< Option to indicate Selection by 2 byte file Id. */
#define PHAL_MFNTAG42XDNA_SELECTOR_1                                    0x01U   /**< Option to indicate Selection by child DF. */
#define PHAL_MFNTAG42XDNA_SELECTOR_2                                    0x02U   /**< Option to indicate Select EF under current DF. Fid = EF id. */
#define PHAL_MFNTAG42XDNA_SELECTOR_3                                    0x03U   /**< Option to indicate Select parent DF of the current DF. */
#define PHAL_MFNTAG42XDNA_SELECTOR_4                                    0x04U   /**< Option to indicate Selection by DF Name. DFName and len is then valid. */
/** @} */

/**
 * \brief ISO Select. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */

phStatus_t phalMfNtag42XDna_IsoSelectFile (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Option for return / no return of FCI.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FCI_RETURNED
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_FCI_NOT_RETURNED
                                                                                 */
        uint8_t bSelector,                                                      /**< [In] The selector to be used.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SELECTOR_0
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SELECTOR_1
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SELECTOR_2
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SELECTOR_3
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SELECTOR_4
                                                                                 */
        uint8_t * pFid,                                                         /**< [In] The ISO File number to be selected. */
        uint8_t * pDFname,                                                      /**< [In] The ISO DFName to be selected. Valid only when bOption = 0x04. */
        uint8_t bDFnameLen,                                                     /**< [In] Length of bytes available in DFname buffer. */
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared */
        uint8_t ** ppFCI,                                                       /**< [Out] The FCI information returned by the PICC. */
        uint16_t * pwFCILen                                                     /**< [Out] Length of bytes available in FCI buffer. */
    );

/**
 * \brief ISO Read Binary. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_SUCCESS_CHAINING operation success with chaining.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_IsoReadBinary (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                       /**< [In] One of the below options.
                                                                                 *          \arg #PH_EXCHANGE_DEFAULT   : To exchange the initial command to the PICC.
                                                                                 *          \arg #PH_EXCHANGE_RXCHAINING: To exchange the chaining command to PICC in
                                                                                 *                                        case PICC returns AF as the status.
                                                                                 */
        uint8_t bOffset,                                                        /**< [In] The offset from where the data should be read. */
        uint8_t bSfid,                                                          /**< [In] Short ISO File Id. Bit 7 should be 1 to indicate Sfid is supplied.
                                                                                 *        Else it is treated as MSB of 2Byte offset.
                                                                                 */
        uint32_t dwBytesToRead,                                                 /**< [In] Number of bytes to read. If 0, then entire file to be read. */
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared */
        uint8_t ** ppRxBuffer,                                                  /**< [Out] The data retuned by the PICC. */
        uint32_t * pBytesRead                                                   /**< [Out] Length of bytes available in RxData buffer. */
    );

/**
 * \brief Iso Update Binary. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_IsoUpdateBinary (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOffset,                                                        /**< [In] The offset from where the data should be updated. */
        uint8_t bSfid,                                                          /**< [In] Short ISO File Id. Bit 7 should be 1 to indicate Sfid is supplied.
                                                                                 *        Else it is treated as MSB of 2Byte offset.
                                                                                 */
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared */
        uint8_t * pData,                                                        /**< [In] Data to be updated. */
        uint32_t dwDataLen                                                      /**< [In] Length of bytes available in Data buffer. */
    );

/**
 * end of group phalMfNtag42xDna_ISO7816
 * @}
 */




/* MIFARE NTAG 42xDNA Originality Check functions. ----------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_OriginalityCheck Commands_OriginalityCheck
 * \brief These Components implement the MIFARE NTAG 42xDNA Originality Check functions.
 * @{
 */

/**
 * \brief Performs the originality check to verify the genuineness of chip
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_ReadSign (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bAddr,                                                          /**< [In] Value is always 00. Present for forward compatibility reasons */
        uint8_t ** pSignature                                                   /**< [Out] The plain 56 bytes originality signature of the PICC. */
    );

/**
 * end of group phalMfNtag42xDna_OriginalityCheck
 * @}
 */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE NTAG 42xDNA Tag Tamper Protection functions. ------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_TagTamper Commands_TagTamper
 * \brief These Components implement the MIFARE NTAG 42xDNA Tag Tamper Protection functions.
 * @{
 */

/**
 * \brief Returns Tag Tamper Status data of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetTagTamperStatus(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pRxBuffer,                                                    /**< [Out] Read Tag Tamper Protection status */
        uint8_t * pRxLen                                                        /**< [Out] Recieved Length */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalMfNtag42xDna_TagTamper
 * @}
 */




/* MIFARE NTAG 42xDNA Miscellaneous functions. --------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfNtag42xDna_Miscellaneous Commands_Miscellaneous
 * \brief These Components implement the MIFARE DESFire Light Miscellaneous functions. These are not part of actual Identity data sheet
 * rather its for internal purpose.
 * @{
 */

/** \name The configuration to be used for SetConfig / GetConfig. */
/** @{ */
#define PHAL_MFNTAG42XDNA_ADDITIONAL_INFO                               0x00A1U /**< Option for getconfig to get additional info of a generic error. */
#define PHAL_MFNTAG42XDNA_WRAPPED_MODE                                  0x00A2U /**< Option to get / set current status of command wrapping in ISO 7816-4 APDUs. */
#define PHAL_MFNTAG42XDNA_SHORT_LENGTH_APDU                             0x00A3U /**< Option to get / set Short Length APDU wrapping in ISO 7816-4 APDUs. */
#define PHAL_MFNTAG42XDNA_SDM_KEY_TYPE                                  0x00A4U /**< Distinguish between the LRP_AES or AES key. The values to be used for this configuration are
                                                                                 *      \c AES128       : 0x00
                                                                                 *      \c LRP_AES128   : 0x01
                                                                                 */
/** @} */

/**
 * \brief Perform a GetConfig command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_GetConfig (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                       /**< [In] Configuration to read. Will be one of the below values.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_ADDITIONAL_INFO
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_KEY_TYPE
                                                                                 *        \n
                                                                                 *        Support for Software mode only including the above ones.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_WRAPPED_MODE
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SHORT_LENGTH_APDU
                                                                                 */
        uint16_t * pValue                                                       /**< [Out] The value for the mentioned configuration. */
    );

/**
 * \brief Perform a SetConfig command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_SetConfig (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                       /**< [In] Configuration to set. Will be one of the below values.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_ADDITIONAL_INFO
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SDM_KEY_TYPE
                                                                                 *        \n
                                                                                 *        Support for Software mode only including the above ones.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_WRAPPED_MODE
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_SHORT_LENGTH_APDU
                                                                                 */
        uint16_t wValue                                                         /**< [In] The value for the mentioned configuration. */
    );

/**
 * \brief Reset the authentication
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_ResetAuthentication (
        void * pDataParams                                                      /**< [In] Pointer to this layer's parameter structure. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Calculate MAC for SDM information.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_CalculateMACSDM (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bSdmOption,                                                     /**< [In] SDM Option to indicate which parameters to be considered. Can be ORed.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_VCUID_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_RDCTR_PRESENT
                                                                                 */
        uint16_t wSDMMacKeyNo,                                                  /**< [In] Key number in Software or SAM keystore. */
        uint16_t wSDMMacKeyVer,                                                 /**< [In] Key version in Software or SAM keystore. */
        uint16_t wRamKeyNo,                                                     /**< [In] Key number of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint16_t wRamKeyVer,                                                    /**< [In] Key version of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint8_t * pUid,                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                        /**< [In] Length of bytes available in UID buffer. */
        uint8_t * pSDMReadCtr,                                                  /**< [In] SDM Read Counter Input. */
        uint8_t * pInData,                                                      /**< [In] Data read out of PICC from SDMMacInputoffset for ReadLength bytes */
        uint16_t wInDataLen,                                                    /**< [In] Length of bytes available in InData buffer. */
        uint8_t * pRespMac                                                      /**< [Out] The computed MAC information. */
    );

/**
 * \brief Decrypt the SDM File data information.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_DecryptSDMENCFileData (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bSdmOption,                                                     /**< [In] SDM Option to indicate which parameters to be considered. Can be ORed.
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_VCUID_PRESENT
                                                                                 *          \arg #PHAL_MFNTAG42XDNA_RDCTR_PRESENT
                                                                                 */
        uint16_t wEncKeyNo,                                                     /**< [In] Key number in keystore of Software or SAM. */
        uint16_t wEncKeyVer,                                                    /**< [In] Key version in keystore of Software or SAM. */
        uint16_t wRamKeyNo,                                                     /**< [In] Key number of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint16_t wRamKeyVer,                                                    /**< [In] Key version of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint8_t * pUid,                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                        /**< [In] Length of bytes available in UID buffer. */
        uint8_t * pSDMReadCtr,                                                  /**< [In] SDM Read Counter Input. */
        uint8_t * pEncdata,                                                     /**< [In] Input Enciphered data. Caller has to pass only the Enciphered data which is between
                                                                                 *        SDMEncOffset for SDMEncLength bytes.
                                                                                 */
        uint16_t wInDataLen,                                                    /**< [In] Length of bytes available in Encdata buffer */
        uint8_t * pPlainData                                                    /**< [Out] The decrypted SDM ENC file data. */
    );

/**
 * \brief Decrypt SDM PICC Data
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfNtag42XDna_DecryptSDMPICCData (
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wKeyNo,                                                        /**< [In] Software or SAM keystore. */
        uint16_t wKeyVer,                                                       /**< [In] Software or SAM keystore. */
        uint8_t * pEncdata,                                                     /**< [In] The Encrypted PICC data. */
        uint16_t wEncDataLen,                                                   /**< [In] Length of bytes available in EncData buffer. */
        uint8_t * pPlainData                                                    /**< [Out] The decrypted SDM PICC data. */
);
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
* end of group phalMfNtag42xDna_Miscellaneous
* @}
*/

/**
 * end of froup phalMfNtag42xDna
 * @}
 */
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALMFNTAG42XDNA_H */
