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


#ifndef PHALMFDFLIGHT_H
#define PHALMFDFLIGHT_H

#include <ph_Status.h>
#include <phTMIUtils.h>
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
#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW

/**
 * \defgroup phalMfdfLight_Sw Component : Software
 * @{
 */

#define PHAL_MFDFLIGHT_SW_ID                                            0x01U   /**< ID for Software MIFARE DESFire Light layer. */

/** \name Sizes */
/** @{ */
#define PHAL_MFDFLIGHT_SIZE_TI                                              4U  /**< Size of Transaction Identifier. */
#define PHAL_MFDFLIGHT_SIZE_MAC                                             16U /**< Size of (untruncated) MAC. */
#define PHAL_MFDFLIGHT_BLOCK_SIZE                                           16U /**< Block Size */
#define PHAL_MFDFLIGHT_MAX_WRITE_SIZE                                   0xFFFDU /**< Max size in a Write function. */
/** @} */

/** \brief MIFARE DESfire Light Software parameter structure  */
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
    uint16_t wCmdCtr ;                                                          /**< Command count within transaction. */
    uint8_t bTi[PHAL_MFDFLIGHT_SIZE_TI];                                        /**< Transaction Identifier. */
    uint8_t bSesAuthMACKey[16];                                                 /**< Authentication MAC key for the session. */
    uint8_t bKeySessionAuthMaster[16];                                          /**< Session Auth master key. */
    uint8_t pUnprocByteBuff[PHAL_MFDFLIGHT_SIZE_MAC];                           /**< Buffer containing unprocessed bytes for read mac answer stream. */
    uint8_t bNoUnprocBytes;                                                     /**< Amount of data in the pUnprocByteBuff. */
    uint8_t bLastBlockBuffer[16];                                               /**< Buffer to store last Block of encrypted data in case of chaining. */
    uint8_t bLastBlockIndex;                                                    /**< Last Block Buffer Index. */
    void * pTMIDataParams;                                                      /**< Pointer to the parameter structure for collecting TMI. */
    uint8_t bShortLenApdu;                                                      /**< Parameter for force set Short Length APDU in case of BIG ISO read. */
} phalMfdfLight_Sw_DataParams_t;

/**
 * \brief Initialise this layer.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phalMfdfLight_Sw_Init(
        phalMfdfLight_Sw_DataParams_t * pDataParams,                            /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wSizeOfDataParams,                                             /**< [In] Specifies the size of the data parameter structure */
        void * pPalMifareDataParams,                                            /**< [In] Pointer to a palMifare component context. */
        void * pKeyStoreDataParams,                                             /**< [In] Pointer to Key Store data parameters. */
        void * pCryptoDataParamsEnc,                                            /**< [In] Pointer to a Crypto component context for encryption. */
        void * pCryptoDataParamsMac,                                            /**< [In] Pointer to a CryptoMAC component context. */
        void * pCryptoRngDataParams,                                            /**< [In] Pointer to a CryptoRng component context. */
        void * pTMIDataParams,                                                  /**< [In] Pointer to a TMI component. */
        void * pHalDataParams                                                   /**< [In] Pointer to the HAL parameters structure. */
    );

phStatus_t phalMfdfLight_Sw_SetVCAParams(
        phalMfdfLight_Sw_DataParams_t * pDataParams,                            /**< [In] Pointer to this layer's parameter structure. */
        void * pAlVCADataParams                                                 /**< [In] Pointer to VCA dataparams stucture. */
    );

/**
 * end of group phalMfdfLight_Sw
 * @}
 */
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */


/***************************************************************************************************************************************/
/* MIFARE DESFire Light Generic command declarations.                                                                                  */
/***************************************************************************************************************************************/
#ifdef NXPBUILD__PHAL_MFDFLIGHT

/** \defgroup phalMfdfLight MIFARE DESFire Light
 * \brief These Functions implement the MIFARE DESFire Light commands.
 * @{
 */

/** \name Other Options for various Functions */
/** @{ */
#define PHAL_MFDFLIGHT_ORIGINALITY_KEY_FIRST                                0x01U   /**< Originality key for AES */
#define PHAL_MFDFLIGHT_ORIGINALITY_KEY_LAST                                 0x04U   /**< Originality key for AES  */
#define PHAL_MFDFLIGHT_APP_KEY_LAST                                         0x04U   /**< Numebr of application keys. */
#define PHAL_MFDFLIGHT_MAC_DATA_INCOMPLETE                                  0x01U   /**< Option for indicating more data to come in next call for MAC calculation */
#define PHAL_MFDFLIGHT_COMMUNICATION_MAC_ON_RC                              0x02U   /**< MAC is appended over response  */
#define PHAL_MFDFLIGHT_AUTHENTICATE_RESET                                   0x08U   /**< authentication state shall be reset */
/** @} */

/** \name The communication mode to be used. */
/** @{ */
#define PHAL_MFDFLIGHT_COMMUNICATION_PLAIN                                  0x00U   /**< Plain mode of communication. */
#define PHAL_MFDFLIGHT_COMMUNICATION_PLAIN_1                                0x20U   /**< Plain mode of communication. */
#define PHAL_MFDFLIGHT_COMMUNICATION_MACD                                   0x10U   /**< MAC mode of communication. */
#define PHAL_MFDFLIGHT_COMMUNICATION_ENC                                    0x30U   /**< Enciphered mode of communication. */
/** @} */

/** \name Error Codes mapping to custom values. */
/** @{ */
#define PHAL_MFDFLIGHT_ERR_FORMAT                       (PH_ERR_CUSTOM_BEGIN + 0)   /**< MF DF Light Response - No changes done to backup files. */
#define PHAL_MFDFLIGHT_ERR_OUT_OF_EEPROM_ERROR          (PH_ERR_CUSTOM_BEGIN + 1)   /**< MF DF Light Response - Insufficient NV-Memory. */
#define PHAL_MFDFLIGHT_ERR_NO_SUCH_KEY                  (PH_ERR_CUSTOM_BEGIN + 2)   /**< MF DF Light Invalid key number specified. */
#define PHAL_MFDFLIGHT_ERR_PERMISSION_DENIED            (PH_ERR_CUSTOM_BEGIN + 3)   /**< MF DF Light Current configuration/status does not allow the requested command. */
#define PHAL_MFDFLIGHT_ERR_APPLICATION_NOT_FOUND        (PH_ERR_CUSTOM_BEGIN + 4)   /**< MF DF Light Requested AID not found on PICC. */
#define PHAL_MFDFLIGHT_ERR_BOUNDARY_ERROR               (PH_ERR_CUSTOM_BEGIN + 5)   /**< MF DF Light Attempt to read/write data from/to beyond the files/record's limits. */
#define PHAL_MFDFLIGHT_ERR_COMMAND_ABORTED              (PH_ERR_CUSTOM_BEGIN + 6)   /**< MF DF Light Previous cmd not fully completed. Not all frames were requested or provided by the PCD. */
#define PHAL_MFDFLIGHT_ERR_COUNT                        (PH_ERR_CUSTOM_BEGIN + 7)   /**< MF DF Light Num. of applns limited to 28. No additional applications possible. */
#define PHAL_MFDFLIGHT_ERR_DUPLICATE                    (PH_ERR_CUSTOM_BEGIN + 8)   /**< MF DF Light File/Application with same number already exists. */
#define PHAL_MFDFLIGHT_ERR_FILE_NOT_FOUND               (PH_ERR_CUSTOM_BEGIN + 9)   /**< MF DF Light Specified file number does not exist. */
#define PHAL_MFDFLIGHT_ERR_PICC_CRYPTO                  (PH_ERR_CUSTOM_BEGIN + 10)  /**< MF DF Light Crypto error returned by PICC. */
#define PHAL_MFDFLIGHT_ERR_PARAMETER_ERROR              (PH_ERR_CUSTOM_BEGIN + 11)  /**< MF DF Light Parameter value error returned by PICC. */
#define PHAL_MFDFLIGHT_ERR_DF_GEN_ERROR                 (PH_ERR_CUSTOM_BEGIN + 12)  /**< MF DF Light DesFire Generic error. Check additional Info. */
#define PHAL_MFDFLIGHT_ERR_DF_7816_GEN_ERROR            (PH_ERR_CUSTOM_BEGIN + 13)  /**< MF DF Light ISO 7816 Generic error. Check Additional Info. */
#define PHAL_MFDFLIGHT_ERR_CMD_INVALID                  (PH_ERR_CUSTOM_BEGIN + 14)  /**< MF DF Light ISO 7816 Generic error. Check Additional Info. */
/** @} */

/** \name Key Types */
/** @{ */
#define PHAL_MFDFLIGHT_KEY_TYPE_2K3DES                                      0x00U   /**< 2 Key Triple Des */
#define PHAL_MFDFLIGHT_KEY_TYPE_3K3DES                                      0x01U   /**< 3 Key Triple Des */
#define PHAL_MFDFLIGHT_KEY_TYPE_AES128                                      0x02U   /**< AES 128 Key */
/** @} */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE DESFire Light secure messaging related commands. --------------------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_SecureMessaging Commands_SecureMessaging
 * \brief These Components implement the MIFARE DESFire Light Secure Messaging related commands.
 * @{
 */

/** \name Authentication Modes */
/** @{ */
#define PHAL_MFDFLIGHT_NOT_AUTHENTICATED                                    0xFFU   /**< No authentication. */
#define PHAL_MFDFLIGHT_AUTHENTICATE                                         0x0AU   /**< D40 Authentication; 0x0A. */
#define PHAL_MFDFLIGHT_AUTHENTICATEISO                                      0x1AU   /**< ISO Authentication; 0x1A. */
#define PHAL_MFDFLIGHT_AUTHENTICATEAES                                      0xAAU   /**< AES Authentication; 0xAA. */
#define PHAL_MFDFLIGHT_AUTHENTICATEEV2                                      0x71U   /**< EV2 First Authentication; 0x71. */
#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE_PART2                               0x77U   /**< EV2 NonFirst Authenticate; 0x77. */
#define PHAL_MFDFLIGHT_AUTH_TYPE_EV2                                        0x01U   /**< Auth Type as EV2. */
#define PHAL_MFDFLIGHT_AUTH_TYPE_LRP                                        0x02U   /**< Auth Type as LRP. */
#define PHAL_MFDFLIGHT_AUTHNONFIRST_NON_LRP                                 0x0000U /**< Non First Auth in regular EV2 auth Mode. */
#define PHAL_MFDFLIGHT_AUTHFIRST_NON_LRP                                    0x0001U /**< First Auth in regular EV2 auth Mode. */
#define PHAL_MFDFLIGHT_AUTHNONFIRST_LRP                                     0x0002U /**< Non First Auth in LRP mode. */
#define PHAL_MFDFLIGHT_AUTHFIRST_LRP                                        0x0003U /**< First Auth in LRP mode. */
/** @} */

/** \name Diversification options to be used with Authenticate command. */
/** @{ */
#define PHAL_MFDFLIGHT_NO_DIVERSIFICATION                                   0xFFFFU /**< No diversification. */
#define PHAL_MFDFLIGHT_DIV_METHOD_ENCR              PH_CRYPTOSYM_DIV_MODE_DESFIRE   /**< Encryption based method of diversification. */
#define PHAL_MFDFLIGHT_DIV_METHOD_CMAC          PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS   /**< CMAC based method of diversification. */
/** @} */

/**
 * \brief Performs an Light First or Light Non First Authentication depending upon bFirstAuth Parameter. This will be using the AES128 keys and will
 * generate and verify the contents based on generic AES algorithm.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_AuthenticateEv2(
        void *pDataParams,                                                      /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFirstAuth,                                                     /**< [In] One of the below options.
                                                                                 *          \arg #PHAL_MFDFLIGHT_AUTHNONFIRST_NON_LRP
                                                                                 *          \arg #PHAL_MFDFLIGHT_AUTHFIRST_NON_LRP
                                                                                 *          \arg #PHAL_MFDFLIGHT_AUTHNONFIRST_LRP
                                                                                 *          \arg #PHAL_MFDFLIGHT_AUTHFIRST_LRP
                                                                                 */
        uint16_t wOption,                                                       /**< [In] Diversification option can be one of
                                                                                 *          \arg #PHAL_MFDFLIGHT_NO_DIVERSIFICATION
                                                                                 *          \arg #PHAL_MFDFLIGHT_DIV_METHOD_ENCR
                                                                                 *          \arg #PHAL_MFDFLIGHT_DIV_METHOD_CMAC
                                                                                 */
        uint16_t wKeyNo,                                                        /**< [In] Key number in keystore of software or SAM. */
        uint16_t wKeyVer,                                                       /**< [In] Key version in keystore of software or SAM. */
        uint8_t bKeyNoCard,                                                     /**< [In] Key number on card. */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input. Can be NULL. */
        uint8_t bDivLen,                                                        /**< [In] Length of diversification input max 31B. */
        uint8_t bLenPcdCapsIn,                                                  /**< [In] Length of PcdCapsIn. Always zero for following authentication */
        uint8_t *bPcdCapsIn,                                                    /**< [In] PCD Capabilities. */
        uint8_t *bPcdCapsOut,                                                   /**< [Out] PCD Capabilities. */
        uint8_t *bPdCapsOut                                                     /**< [Out] PD Capabilities. */
    );

/**
 * end of group phalMfdfLight_SecureMessaging
 * @}
 */




/* MIFARE DESFire Light Memory and Configuration mamangement commands. --------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_MemoryConfiguration Commands_MemoryConfiguration
 * \brief These Components implement the MIFARE DESFire Light Memory and Configuration Management related commands.
 * @{
 */

/** \name Options for SetConfiguration command. */
/** @{ */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION0                                   0x00U   /**< Option 0 for PICC Configuration. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION1                                   0x01U   /**< Option 1 Reserved. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION2                                   0x02U   /**< Option 2 for ATS Update. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION3                                   0x03U   /**< Option 3 for SAK Update. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION4                                   0x04U   /**< Option 4 for Secure Messaging Configuration. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION5                                   0x05U   /**< Option 5 for Capability Data. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION6                                   0x06U   /**< Option 6 for Application Renaming. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION8                                   0x08U   /**< Option 8 for File Renaming. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION9                                   0x09U   /**< Option 9 for Value file type configuration. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION10                                  0x0AU   /**< Option 10 for Failed Authentication Counter Configuration. */
#define PHAL_MFDFLIGHT_SET_CONFIG_OPTION11                                  0x0BU   /**< Option 11 for Hardware Configuration. */
/** @} */

/**
 * \brief Configures the card and pre personalizes the card with a key, defines if the UID or the random ID is sent back during communication setup and
 * configures the ATS string.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_SetConfiguration(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Configuration Option. Define length and content of the Data parameter.
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION0
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION1
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION2
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION3
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION4
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION5
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION6
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION8
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION9
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION10
                                                                                 *          \arg #PHAL_MFDFLIGHT_SET_CONFIG_OPTION11
                                                                                 */
        uint8_t * pData,                                                        /**< [In] Data for the option specified.*/
        uint8_t bDataLen                                                        /**< [In] Length of bytes available in Data buffer. */
    );

/**
 * \brief Returns the Unique ID of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetCardUID(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pUid                                                          /**< [Out] The complete UID of the PICC. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Returns manufacturing related data of the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetVersion(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pVerInfo                                                      /**< [Out] The version information of the product. */
    );

/**
 * end of group phalMfdfLight_MemoryConfiguration
 * @}
 */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE DESFire Light Key mamangement commands. ------------------------------------------------------------------------------------ */
/**
 * \defgroup phalMfdfLight_KeyManagement Commands_KeyManagement
 * \brief These Components implement the MIFARE DESFire Light Key Management related commands.
 * @{
 */

/** \name Diversification options to be used with ChangeKey command. */
/** @{ */
#define PHAL_MFDFLIGHT_CHGKEY_NO_DIVERSIFICATION                            0xFFFFU /**< No diversification. */
#define PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY                                   0x0002U /**< Bit 1. Indicating diversification of new key requred. */
#define PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY                                   0x0004U /**< Bit 2 indicating old key was diversified. */
#define PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY_ONERND                            0x0008U /**< Bit 3 indicating new key diversification using one rnd. Default is two rounds. */
#define PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY_ONERND                            0x0010U /**< Bit 4 indicating old key diversification using one rnd. Default is two rounds. */
#define PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC                               0x0020U /**< Bit 5 indicating key diversification method based on CMAC. Default is Encryption method */
/** @} */

/**
 * \brief Changes any key on the PICC
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ChangeKey(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                       /**< [In] One of the below options.
                                                                                 *          \arg #PHAL_MFDFLIGHT_NO_DIVERSIFICATION
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY_ONERND
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY_ONERND
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY |
                                                                                 *               #PHAL_MFDFLIGHT_CHGKEY_DIV_METHOD_CMAC
                                                                                 *          \arg #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY | #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY |
                                                                                 *               #PHAL_MFDFLIGHT_CHGKEY_DIV_NEW_KEY_ONERND | #PHAL_MFDFLIGHT_CHGKEY_DIV_OLD_KEY_ONERND
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
phStatus_t phalMfdfLight_GetKeyVersion(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bKeyNo,                                                         /**< [In] Key number on card. */
        uint8_t bKeySetNo,                                                      /**< [In] 1 byte Key set number. Optional as it is passed only when bit[6] of bKeyNo is set. */
        uint8_t * pKeyVersion,                                                  /**< [Out] The version of the specified key. */
        uint8_t * bRxLen                                                        /**< [Out] Length of bytes available in KeyVersion buffer. */
    );

/**
 * end of group phalMfdfLight_KeyManagement
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */




#ifdef NXPBUILD__PH_CRYPTOSYM
/* MIFARE DESFire Light File mamangement commands. ------------------------------------------------------------------------------------ */
/**
 * \defgroup phalMfdfLight_FileManagement Commands_FileManagement
 * \brief These Components implement the MIFARE DESFire Light File Management related commands.
 * @{
 */

/** \name Diversification options to be used with CreateTransactionMacFile command.
 * These macros are valid only if the layer is initialized for SAM.
 */
/** @{ */
#define PHAL_MFDFLIGHT_CRTMACFILE_DIVERSIFICATION_OFF                       0x00U   /**< No diversification. */
#define PHAL_MFDFLIGHT_CRTMACFILE_DIVERSIFICATION_ON                        0x01U   /**< Diversification is enabled.*/
/** @} */

/**
 * \brief Creates a Transaction MAC file. An application can have only one Transaction MAC File.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_CreateTransactionMacFile(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo,                                                        /**< [In] File number of the file to be created. */
        uint8_t bCommMode,                                                      /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN_1
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t * pAccessRights,                                                /**< [In] The access right to be applied for the file. Should be 2 byte. */
        uint8_t bKeyType,                                                       /**< [In] Bit0-1 indicates key type and it should be always #PHAL_MFDFLIGHT_KEY_TYPE_AES128 */
        uint8_t * bTMKey,                                                       /**< [In] The 16 byte key value to be used as TMKey. */
        uint16_t wKeyNo,                                                        /**< [In] The reference key number in SAM keystore. Only valid if initialized for SAM. */
        uint8_t bTMKeyVer,                                                      /**< [In] The version of the TMKey. */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input to diversify the TMKey. */
        uint8_t bDivInputLen                                                    /**< [In] Length of bytes available in DivInput buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Permanently deactivates a file within the file directory of the currently selected application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_DeleteFile(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo                                                         /**< [In] File number of the file to be deleted. */
    );

/**
 * \brief Returns the file IDs of all active files within the currently selected application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetFileIDs(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pFid,                                                         /**< [Out] The buffer containing the available file ID(s). Buffer should be 32 bytes. */
        uint8_t * bNumFid                                                       /**< [Out] The length of bytes available in Fid buffer. */
    );

/**
 * \brief Get the ISO File IDs.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetISOFileIDs(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pFidBuffer,                                                   /**< [Out] The buffer containing the available ISO file ID(s). Buffer should be 64 bytes. */
        uint8_t * bNumFid                                                       /**< [Out] The number of ISO File ID's read. */
    );

/**
 * \brief Get informtion on the properties of a specific file
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetFileSettings(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo,                                                        /**< [In] The file number for which the setting to be retrieved. */
        uint8_t * pFSBuffer,                                                    /**< [Out] The buffer containing the settings. The buffer should be 17 bytes. */
        uint8_t * bBufferLen                                                    /**< [Out] The length of bytes available in FSBuffer buffer. */
    );

/** \name The File option and other flags for ChangeFileSettings command. */
/** @{ */
#define PHAL_MFDFLIGHT_FILE_OPTION_PLAIN                                0x00U   /**< Plain mode of communication. */
#define PHAL_MFDFLIGHT_FILE_OPTION_PLAIN_1                              0x02U   /**< Plain mode of communication. */
#define PHAL_MFDFLIGHT_FILE_OPTION_MACD                                 0x01U   /**< MAC mode of communication. */
#define PHAL_MFDFLIGHT_FILE_OPTION_ENC                                  0x03U   /**< Enciphered mode of communication. */
#define PHAL_MFDFLIGHT_MFM_SPECIFICS_ENABLED                            0x01U   /**< MFM Specific feature(TMC Limit) to be realized */

#define PHAL_MFDFLIGHT_TMCLIMITCONFIG                                   0x20U   /**< 5th Bit of FileOption indicating TMC limit config. */
#define PHAL_MFDFLIGHT_EXCLUNAUTHCONFIG                                 0x10U   /**< 4th Bit of FileOption indicating Excude Unauthentication config. */
/** @} */

/**
 * \brief Changes the access parameters of an existing file
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ChangeFileSettings(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Indicates the mode of communication to be used while exchanging the data to PICC.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 *        Ored with the below option. Valid for Software mode only.
                                                                                 *          \arg #PHAL_MFDFLIGHT_MFM_SPECIFICS_ENABLED
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] File number for which the setting need to be updated. */
        uint8_t bFileOption,                                                    /**< [In] New communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_FILE_OPTION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_FILE_OPTION_PLAIN_1
                                                                                 *          \arg #PHAL_MFDFLIGHT_FILE_OPTION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_FILE_OPTION_ENC
                                                                                 *        \n
                                                                                 *        Ored with one of the below flags if required.
                                                                                 *          \arg #PHAL_MFDFLIGHT_TMCLIMITCONFIG
                                                                                 *          \arg #PHAL_MFDFLIGHT_EXCLUNAUTHCONFIG
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
 * end of group phalMfdfLight_FileManagement
 * @}
 */




/* MIFARE DESFire Light Data mamangement commands. ----------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_DataManagement Commands_DataManagement
 * \brief These Components implement the MIFARE DESFire Light Data Management related commands.
 * @{
 */

/**
 * \brief Reads data from standard data files or backup data files
 *
 * \remarks
 * Chaining upto the size of the HAL Rx buffer is handled within this function. If more data is to be read, the user has to call
 * this function again with bOption = #PH_EXCHANGE_RXCHAINING | [one of the communication options]
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_SUCCESS_CHAINING indicating more data to be read.
 * \retval Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ReadData(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 *
                                                                                 *          \c #PH_EXCHANGE_RXCHAINING: To be Or'd with the above option
                                                                                 *              flag if Chaining status is returned.
                                                                                 */
        uint8_t bIns,                                                           /**< [In] Uses ISO 14443-4 chaining instead of DESFire application chaining. This should be always Set. */
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
phStatus_t phalMfdfLight_WriteData(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bIns,                                                           /**< [In] Uses ISO 14443-4 chaining instead of DESFire application chaining. This should be always Set. */
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
 * \brief Reads the currently stored value from value files.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetValue(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] The file number for which the value to the retrieved. */
        uint8_t * pValue                                                        /**< [Out] The value returned by the PICC. The buffer should be 4 bytes. */
    );

/**
 * \brief Increases a value stored in a Value File
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_Credit(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] The file number to which the value should be credited. */
        uint8_t * pValue                                                        /**< [Out] The value to be credited. Will be of 4 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be credited then it will be 10 00 00 00.
                                                                                 */
    );

/**
 * \brief Decreases a value stored in a Value File
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_Debit(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] The file number to which the value should be debited. */
        uint8_t * pValue                                                        /**< [Out] The value to be debited. Will be of 4 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be debited then it will be 10 00 00 00.
                                                                                 */
    );

/**
 * \brief Allows a limited increase of a value stored in a Value File without having full credit permissions to the file.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_LimitedCredit(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bFileNo,                                                        /**< [In] The file number to which the value should be credited. */
        uint8_t * pValue                                                        /**< [Out] The value to be credited. Will be of 4 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be credited then it will be 10 00 00 00.
                                                                                 */
    );

/**
 * \brief Reads out a set of complete records from a Cyclic or Linear Record File.
 *
 * \remarks
 * The readrecords command reads and stores data in the rxbuffer upto the rxbuffer size before returning
 * to the user. The rxbuffer is configured during the HAL init and this is specified by the user.
 *
 * Chaining upto the size of the HAL Rx buffer is handled within this function.
 * If more data is to be read, the user has to call this function again with
 * bCommOption = PH_EXCHANGE_RXCHAINING | [one of the communication options]
 *
 * If TMI collection is ON, if pRecCount is zero then pRecSize is madatory parameter.
 * If pRecSize and RecCount are zero and TMI collection is ON, then  PH_ERR_INVALID_PARAMETER error returned.
 * If TMI collection is ON; and if wrong pRecSize is provided, then wrong RecCount value will be calculated and updated for TMI collection.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_SUCCESS_CHAINING indicating more data to be read.
 * \retval Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ReadRecords(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 *
                                                                                 *          \c #PH_EXCHANGE_RXCHAINING: To be Or'd with the above option
                                                                                 *              flag if Chaining status is returned.
                                                                                 */
        uint8_t bIns,                                                           /**< [In] Uses ISO 14443-4 chaining instead of DESFire application chaining. This should be always Set. */
        uint8_t bFileNo,                                                        /**< [In] The file number from where the data to be read. */
        uint8_t * pRecNo,                                                       /**< [In] 3 bytes LSB first. Record number of the newest record targeted, starting to count
                                                                                 *        from the latest record written.
                                                                                 */
        uint8_t * pRecCount,                                                    /**< [In] 3 bytes LSB first. Number of records to be read. If 0x00 00 00, then all the records are read. */
        uint8_t * pRecSize,                                                     /**< [In] The number of bytes to be read. Will be of 3 bytes with LSB first. */
        uint8_t ** ppRxdata,                                                    /**< [Out] The data retuned by the PICC. */
        uint16_t * pRxdataLen                                                   /**< [Out] Length of bytes available in RxData buffer. */
    );

/**
 * \brief Writes data to a record in a Cyclic or Linear Record File.
 *
 * \remarks
 * Implements chaining to the card.
 * The data provided on pData will be chained to the card by sending data upto the frame size of the DESFire PICC, at a time.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_WriteRecord(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bIns,                                                           /**< [In] Uses ISO 14443-4 chaining instead of DESFire application chaining. This should be always Set. */
        uint8_t bFileNo,                                                        /**< [In] The file number to which the data should be written. */
        uint8_t * pOffset,                                                      /**< [In] The offset from where the data should be written. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 need to be offset then it will be 10 00 00.
                                                                                 */
        uint8_t * pData,                                                        /**< [In] The data to be written to the PICC. */
        uint8_t * pDataLen                                                      /**< [In] The number of bytes to be written. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be written then it will be 10 00 00.
                                                                                 */
    );

/**
 * \brief Updates data to a record in a Cyclic or Linear Record File.
 *
 * \remarks
 * Implements chaining to the card.
 * The data provided on pData will be chained to the card by sending data upto the frame size of the DESFire PICC, at a time.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_UpdateRecord(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommOption,                                                    /**< [In] Communication settings for the file.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_PLAIN
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_MACD
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMUNICATION_ENC
                                                                                 */
        uint8_t bIns,                                                           /**< [In] Uses ISO 14443-4 chaining instead of DESFire application chaining. This should be always Set. */
        uint8_t bFileNo,                                                        /**< [In] The file number to which the data should be written. */
        uint8_t * pRecNo,                                                       /**< [In] Record number. Records are numbered starting with the latest record written. */
        uint8_t * pOffset,                                                      /**< [In] Starting position for the update operationwithin the targeted record. */
        uint8_t * pData,                                                        /**< [In] The data to be written to the PICC. */
        uint8_t * pDataLen                                                      /**< [In] The number of bytes to be written. Will be of 3 bytes with LSB first.
                                                                                 *          \c If 0x10 bytes need to be written then it will be 10 00 00.
                                                                                 */
    );

/**
 * \brief Resets a Cyclic or Linear Record File.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ClearRecordFile(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo                                                         /**< [In] The file number to be cleared. */
    );

/**
 * end of group phalMfdfLight_DataManagement
 * @}
 */




/* MIFARE DESFire Light Transaction mamangement commands. ---------------------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_TransactionnManagement Commands_TransactionManagement
 * \brief These Components implement the MIFARE DESFire Light Transaction Management related commands.
 * @{
 */

/** \name Options for CommitTransaction command. */
/** @{ */
#define PHAL_MFDFLIGHT_COMMIT_TXN_NO_TMC_TMV_RETURNED                       0x00    /**< Option byte is not exchanged to the PICC. */
#define PHAL_MFDFLIGHT_COMMIT_TXN_TMC_TMV_RETURNED                          0x01    /**< Option byte exchanged to PICC and represent return of TMC and TMV. */
#define PHAL_MFDFLIGHT_COMMIT_TXN_INCLUDE_OPTION                            0x80    /**< Option byte exchanged to PICC. This is to support the exchange of Option byte to PICC. */
/** @} */

/**
 * \brief Validates all previous write access' on Backup Data files, value files and record files within one application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_CommitTransaction(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] One of the below options.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMIT_TXN_NO_TMC_TMV_RETURNED
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMIT_TXN_TMC_TMV_RETURNED
                                                                                 *        \n
                                                                                 *        Can be ored with one of the above values.
                                                                                 *          \arg #PHAL_MFDFLIGHT_COMMIT_TXN_INCLUDE_OPTION
                                                                                 */
        uint8_t * pTMC,                                                         /**< [Out] The increased value as stored in FileType.TransactionMAC. \n
                                                                                 *          \c If Standard AES: 4 byte Transaction MAC counter value. \n
                                                                                 *          \c If LRP         : 2 byte of Actual TMC followed by 2 bytes of Session TMC
                                                                                 */
        uint8_t * pTMV                                                          /**< [Out] 8 bytes Transaction MAC value. */
    );

/**
 * \brief Invalidates all previous write access' on Backup Data files, value files and record files within one application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_AbortTransaction(
        void * pDataParams                                                      /**< [In] Pointer to this layer's parameter structure. */
    );

/**
 * \brief Secures the transaction by commiting the application to ReaderID specified
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_CommitReaderID(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pTMRI,                                                        /**< [In] 16 bytes of Transaction MAC reader ID. */
        uint8_t * pEncTMRI                                                      /**< [Out] 16 bytes of Encrypted Transaction MAC ReaderID of the latest successful transaction. */
    );

/**
 * end of group phalMfdfLight_TransactionnManagement
 * @}
 */




/* MIFARE DESFire Light ISO7816-4 commands. ------------------------------------------------------------------------------------------ */
/**
 * \defgroup phalMfdfLight_ISO7816 Commands_ISO7816
 * \brief These Components implement the MIFARE DESFire Light ISO/IEC 7816-4 Basic commands.
 * @{
 */

/** \name Options for ISOSelectFile command. */
/** @{ */
#define PHAL_MFDFLIGHT_FCI_RETURNED                                     0x00U   /**< Option to indicate the return of FCI. */
#define PHAL_MFDFLIGHT_FCI_NOT_RETURNED                                 0x0CU   /**< Option to indicate the no return of FCI. */
#define PHAL_MFDFLIGHT_SELECTOR_0                                       0x00U   /**< Option to indicate Selection by 2 byte file Id. */
#define PHAL_MFDFLIGHT_SELECTOR_1                                       0x01U   /**< Option to indicate Selection by child DF. */
#define PHAL_MFDFLIGHT_SELECTOR_2                                       0x02U   /**< Option to indicate Select EF under current DF. Fid = EF id. */
#define PHAL_MFDFLIGHT_SELECTOR_3                                       0x03U   /**< Option to indicate Select parent DF of the current DF. */
#define PHAL_MFDFLIGHT_SELECTOR_4                                       0x04U   /**< Option to indicate Selection by DF Name. DFName and len is then valid. */
/** @} */

/**
 * \brief ISO Select. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_IsoSelectFile(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                        /**< [In] Option for return / no return of FCI.
                                                                                 *          \arg #PHAL_MFDFLIGHT_FCI_RETURNED
                                                                                 *          \arg #PHAL_MFDFLIGHT_FCI_NOT_RETURNED
                                                                                 */
        uint8_t bSelector,                                                      /**< [In] The selector to be used.
                                                                                 *          \arg #PHAL_MFDFLIGHT_SELECTOR_0
                                                                                 *          \arg #PHAL_MFDFLIGHT_SELECTOR_1
                                                                                 *          \arg #PHAL_MFDFLIGHT_SELECTOR_2
                                                                                 *          \arg #PHAL_MFDFLIGHT_SELECTOR_3
                                                                                 *          \arg #PHAL_MFDFLIGHT_SELECTOR_4
                                                                                 */
        uint8_t * pFid,                                                         /**< [In] The ISO File number to be selected. */
        uint8_t * pDFname,                                                      /**< [In] The ISO DFName to be selected. Valid only when bOption = 0x04. */
        uint8_t bDFnameLen,                                                     /**< [In] Length of bytes available in DFname buffer. */
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared. */
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
phStatus_t phalMfdfLight_IsoReadBinary(
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
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared. */
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
phStatus_t phalMfdfLight_IsoUpdateBinary(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOffset,                                                        /**< [In] The offset from where the data should be updated. */
        uint8_t bSfid,                                                          /**< [In] Short ISO File Id. Bit 7 should be 1 to indicate Sfid is supplied.
                                                                                 *        Else it is treated as MSB of 2Byte offset.
                                                                                 */
        uint8_t bExtendedLenApdu,                                               /**< [In] Flag for Extended Length APDU. This should be always cleared. */
        uint8_t * pData,                                                        /**< [In] Data to be updated. */
        uint32_t dwDataLen                                                      /**< [In] Length of bytes available in Data buffer. */
    );

/**
 * end of group phalMfdfLight_ISO7816
 * @}
 */




/* MIFARE DESFire Light Originality Check functions. --------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_OriginalityCheck Commands_OriginalityCheck
 * \brief These Components implement the MIFARE DESFire Light Originality Check functions.
 * @{
 */

/**
 * \brief Performs the originality check to verify the genuineness of chip
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_ReadSign(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bAddr,                                                          /**< [In] Value is always 00. Present for forward compatibility reasons */
        uint8_t ** pSignature                                                   /**< [Out] The plain 56 bytes originality signature of the PICC. */
    );

/**
 * end of group phalMfdfLight_OriginalityCheck
 * @}
 */




/* MIFARE DESFire Light Miscellaneous functions. ------------------------------------------------------------------------------------- */
/**
 * \defgroup phalMfdfLight_Miscellaneous Commands_Miscellaneous
 * \brief These Components implement the MIFARE DESFire Light Miscellaneous functions. These are not part of actual DESFire Light data sheet
 * rather its for internal purpose.
 * @{
 */

/** \name The configuration to be used for SetConfig / GetConfig. */
/** @{ */
#define PHAL_MFDFLIGHT_ADDITIONAL_INFO                                      0x00A1U  /**< Option for getconfig to get additional info of a generic error. */
#define PHAL_MFDFLIGHT_WRAPPED_MODE                                         0x00A2U  /**< Option to get / set current status of command wrapping in ISO 7816-4 APDUs. */
#define PHAL_MFDFLIGHT_SHORT_LENGTH_APDU                                    0x00A3U  /**< Option to get / set Short Length APDU wrapping in ISO 7816-4 APDUs. */
/** @} */

/**
 * \brief Perform a GetConfig command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_GetConfig(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                       /**< [In] Configuration to read. Will be one of the below values.
                                                                                 *          \arg #PHAL_MFDFLIGHT_ADDITIONAL_INFO
                                                                                 *        \n
                                                                                 *        Support for Software mode only including the above ones.
                                                                                 *          \arg #PHAL_MFDFLIGHT_WRAPPED_MODE
                                                                                 *          \arg #PHAL_MFDFLIGHT_SHORT_LENGTH_APDU
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
phStatus_t phalMfdfLight_SetConfig(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                       /**< [In] Configuration to set. Will be one of the below values.
                                                                                 *          \arg #PHAL_MFDFLIGHT_ADDITIONAL_INFO
                                                                                 *        \n
                                                                                 *        Support for Software mode only including the above ones.
                                                                                 *          \arg #PHAL_MFDFLIGHT_WRAPPED_MODE
                                                                                 *          \arg #PHAL_MFDFLIGHT_SHORT_LENGTH_APDU
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
phStatus_t phalMfdfLight_ResetAuthentication(
        void * pDataParams                                                      /**< [In] Pointer to this layer's parameter structure. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Computed the Transaction MAC input as performed on the PICC.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_CalculateTMV(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                       /**< [In] Diversification option. 0xFFFF is for No Diversification. */
        uint16_t wKeyNoTMACKey,                                                 /**< [In] Key number in key store of Software. */
        uint16_t wKeyVerTMACKey,                                                /**< [In] Key version in key store of Software. */
        uint16_t wRamKeyNo,                                                     /**< [In] Key number of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint16_t wRamKeyVer,                                                    /**< [In] Key version of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input to diversify TMACKey. */
        uint8_t bDivInputLen,                                                   /**< [In] Length of byte available in DivInput buffer. */
        uint8_t * pTMC,                                                         /**< [In] 4 bytes Transaction MAC Counter. It should be 1 time subtracted from
                                                                                 *        the actual value and shold be LSB first.
                                                                                 */
        uint8_t * pUid,                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                        /**< [In] Length of UID supplied. */
        uint8_t * pTMI,                                                         /**< [In] Transaction MAC Input. */
        uint32_t wTMILen,                                                       /**< [In] Length of bytes available in TMI buffer. */
        uint8_t * pTMV                                                          /**< [Out] The computed Transaction MAC Value. */
    );

/**
 * \brief Decrypted the reader ID as performed on the PICC.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalMfdfLight_DecryptReaderID(
        void * pDataParams,                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                       /**< [In] Diversification option. 0xFFFF is for No Diversification. */
        uint16_t wKeyNoTMACKey,                                                 /**< [In] Key number in key store of Software. */
        uint16_t wKeyVerTMACKey,                                                /**< [In] Key version in key store of Software. */
        uint16_t wRamKeyNo,                                                     /**< [In] Key number of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint16_t wRamKeyVer,                                                    /**< [In] Key version of Destination Key where the computed session TMAC key will be stored.
                                                                                 *        To be used for SAM AV3 only.
                                                                                 */
        uint8_t * pDivInput,                                                    /**< [In] Diversification input to diversify TMACKey. */
        uint8_t bDivInputLen,                                                   /**< [In] Length of byte available in DivInput buffer. */
        uint8_t * pTMC,                                                         /**< [In] 4 bytes Transaction MAC Counter. It should be 1 time subtracted from
                                                                                 *        the actual value and shold be LSB first.
                                                                                 */
        uint8_t * pUid,                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                        /**< [In] Length of UID supplied. */
        uint8_t * pEncTMRI,                                                     /**< [In] Encrypted Transaction MAC ReaderID of the latest successful transaction. */
        uint8_t * pTMRIPrev                                                     /**< [Out] Decrypted Reader ID of the last successful transaction. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalMfdfLight_Miscellaneous
 * @}
 */

/**
 * end of group phalMfdfLight
 * @}
 */
#endif /* NXPBUILD__PHAL_MFDFLIGHT */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALMFDFLIGHT_H */
