/*
* Copyright (c), NXP Semiconductors Bangalore / India
*
*               (C)NXP Semiconductors
* All rights are reserved. Reproduction in whole or in part is
* prohibited without the written consent of the copyright owner.
* NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
* arising from its use.
*/

/** \file
* Generic MIFARE DESFire EV1 contactless IC Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  Santosh Araballi: Generated 31. August 2010
*
*/

#ifndef PHALMFDF_H
#define PHALMFDF_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>
#include <ph_TypeDefs.h>
#include <ph_RefDefs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NXPBUILD__PHAL_MFDF

/** \defgroup phalMfdf MIFARE DESFire
* \brief These Functions implement the MIFARE DESFire EV1 commands.
* @{
*/


/**
* \name Other Options for various Functions
*/
/** @{ */
#define PHAL_MFDF_NOT_AUTHENTICATED     0xFFU   /**< No authentication. */


#define PHAL_MFDF_COMMUNICATION_PLAIN   0x00U   /**< Plain mode of communication. */
#define PHAL_MFDF_COMMUNICATION_MACD    0x10U   /**< MAC mode of communication. */
#define PHAL_MFDF_COMMUNICATION_ENC     0x30U   /**< Enciphered mode of communication. */


#define PHAL_MFDF_ADDITIONAL_INFO       0x00A1U  /**< Option for getconfig to get additional info of a generic error. */
/** @} */

/** \name phalMfdf Custom Error Codes
*/
/** @{ */
#define PHAL_MFDF_NO_CHANGES                    ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U)     /**< MF DF Response - No changes done to backup files. */
#define PHAL_MFDF_ERR_OUT_OF_EEPROM_ERROR       ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 1U)     /**< MF DF Response - Insufficient NV-Memory. */
#define PHAL_MFDF_ERR_NO_SUCH_KEY               ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 2U)     /**< MF DF Invalid key number specified. */
#define PHAL_MFDF_ERR_PERMISSION_DENIED         ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 3U)     /**< MF DF Current configuration/status does not allow the requested command. */
#define PHAL_MFDF_ERR_APPLICATION_NOT_FOUND     ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 4U)     /**< MF DF Requested AID not found on PICC. */
#define PHAL_MFDF_ERR_BOUNDARY_ERROR            ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 5U)     /**< MF DF Attempt to read/write data from/to beyond the files/record's limits. */
#define PHAL_MFDF_ERR_COMMAND_ABORTED           ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 6U)     /**< MF DF Previous cmd not fully completed. Not all frames were requested or provided by the PCD. */
#define PHAL_MFDF_ERR_COUNT                     ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 7U)     /**< MF DF Num. of applns limited to 28. No additional applications possible. */
#define PHAL_MFDF_ERR_DUPLICATE                 ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 8U)     /**< MF DF File/Application with same number already exists. */
#define PHAL_MFDF_ERR_FILE_NOT_FOUND            ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 9U)     /**< MF DF Specified file number does not exist. */
#define PHAL_MFDF_ERR_PICC_CRYPTO               ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 10U)    /**< MF DF Crypto error returned by PICC. */
#define PHAL_MFDF_ERR_PARAMETER_ERROR           ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 11U)    /**< MF DF Parameter value error returned by PICC. */
#define PHAL_MFDF_ERR_DF_GEN_ERROR              ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 12U)    /**< MF DF DesFire Generic error. Check additional Info. */
#define PHAL_MFDF_ERR_DF_7816_GEN_ERROR         ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 13U)    /**< MF DF ISO 7816 Generic error. Check Additional Info. */
/** @} */

/** @} */

#endif /* NXPBUILD__PHAL_MFDF */

#ifdef NXPBUILD__PHAL_MFDF_SW

/**
* \defgroup phalMfdf_Sw Component : Software
*/
/*@{*/

#define PHAL_MFDF_SW_ID 0x01U   /**< ID for Software MIFARE DESFire layer. */

/**
* \brief struct phalMfdf_Sw_DataParams_t
*
*/
typedef struct
{
    uint16_t wId;                                               /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                                /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                                 /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParamsEnc;                                /**< Pointer to the parameter structure of the Crypto layer for encryption. */
    void * pCryptoRngDataParams;                                /**< Pointer to the parameter structure of the CryptoRng layer. */
    void * pHalDataParams;                                      /**< Pointer to the HAL parameters structure. */
    uint8_t bSessionKey[24];                                    /**< Session key for this authentication */
    uint8_t bKeyNo;                                             /**< key number against which this authentication is done */
    uint8_t bIv[16];                                            /**< Max size of IV can be 16 bytes */
    uint8_t bAuthMode;                                          /**< Authenticate (0x0A), AuthISO (0x1A), AuthAES (0xAA) */
    uint8_t pAid[3];                                            /**< Aid of the currently selected application */
    uint8_t bCryptoMethod;                                      /**< DES,3DES, 3K3DES or AES */
    uint8_t bWrappedMode;                                       /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 APDUs. */
    uint16_t wCrc;                                              /**< 2 Byte CRC initial value in Authenticate mode. */
    uint32_t dwCrc;                                             /**< 4 Byte CRC initial value in 0x1A, 0xAA mode. */
    uint16_t wAdditionalInfo;                                   /**< Specific error codes for MIFARE DESFire generic errors. */
    uint16_t wPayLoadLen;                                       /**< Amount of data to be read. Required for Enc read to verify CRC. */
    uint8_t bLastBlockBuffer[16];                               /**< Buffer to store last Block of encrypted data in case of chaining. */
    uint8_t bLastBlockIndex;                                    /**< Last Block Buffer Index. */
} phalMfdf_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfdf_Sw_Init(
                            phalMfdf_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t wSizeOfDataParams,             /**< [In] Specifies the size of the data parameter structure */
                            void * pPalMifareDataParams,            /**< [In] Pointer to a palMifare component context. */
                            void * pKeyStoreDataParams,             /**< [In] Pointer to Key Store data parameters. */
                            void * pCryptoDataParamsEnc,            /**< [In] Pointer to a Crypto component context for encryption. */
                            void * pCryptoRngDataParams,            /**< [In] Pointer to a CryptoRng component context. */
                            void * pHalDataParams                   /**< [In] Pointer to the HAL parameters structure. */
                            );

/** @} */

#endif /* NXPBUILD__PHAL_MFDF_SW */



#ifdef NXPBUILD__PHAL_MFDF

/** \addtogroup phalMfdf
* @{
*/


/**
* \name PICC level Commands
*/
/** @{ */


#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalMfdf/src/Sw/phalMfdf_Sw.h"

#define phalMfdf_CreateApplication(pDataParams,bOption,pAid,bKeySettings1,bKeySettings2,pISOFileId,pISODFName,bISODFNameLen) \
        phalMfdf_Sw_CreateApplication((phalMfdf_Sw_DataParams_t *)pDataParams,bOption,pAid,bKeySettings1,bKeySettings2,pISOFileId,pISODFName,bISODFNameLen)

#define phalMfdf_SelectApplication( pDataParams, pAppId) \
        phalMfdf_Sw_SelectApplication( (phalMfdf_Sw_DataParams_t *)pDataParams, pAppId)

#define phalMfdf_GetVersion(pDataParams, pVerInfo) \
        phalMfdf_Sw_GetVersion((phalMfdf_Sw_DataParams_t *)pDataParams, pVerInfo)

#define phalMfdf_CreateStdDataFile(pDataParams,bOption,bFileNo,pISOFileId,bCommSett,pAccessRights,pFileSize) \
        phalMfdf_Sw_CreateStdDataFile((phalMfdf_Sw_DataParams_t *)pDataParams,bOption,bFileNo,pISOFileId,bCommSett,pAccessRights,pFileSize)

#define phalMfdf_WriteData(pDataParams,bCommOption,bFileNo,pOffset,pTxData,pTxDataLen) \
        phalMfdf_Sw_WriteData((phalMfdf_Sw_DataParams_t *)pDataParams,bCommOption,bFileNo,pOffset,pTxData,pTxDataLen)

#define phalMfdf_IsoSelectFile(pDataParams,bOption,bSelector,pFid,pDFname,bDFnameLen,ppFCI,pwFCILen) \
        phalMfdf_Sw_IsoSelectFile((phalMfdf_Sw_DataParams_t *)pDataParams,bOption,bSelector,pFid,pDFname,bDFnameLen,ppFCI,pwFCILen)

#define phalMfdf_IsoReadBinary(pDataParams,wOption,bOffset,bSfid,bBytesToRead,ppRxBuffer,pBytesRead) \
        phalMfdf_Sw_IsoReadBinary((phalMfdf_Sw_DataParams_t *)pDataParams,wOption,bOffset,bSfid,bBytesToRead,ppRxBuffer,pBytesRead)

#define phalMfdf_IsoUpdateBinary(pDataParams,bOffset,bSfid,pData,bDataLen) \
        phalMfdf_Sw_IsoUpdateBinary((phalMfdf_Sw_DataParams_t *)pDataParams,bOffset,bSfid,pData,bDataLen)

#define phalMfdf_GetConfig(pDataParams, wConfig, pValue) \
        phalMfdf_Sw_GetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phalMfdf_SetConfig(pDataParams, wConfig, wValue) \
        phalMfdf_Sw_SetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, wValue)


#endif /* NXPRDLIB_REM_GEN_INTFS */

#ifndef NXPRDLIB_REM_GEN_INTFS /* Without optimization */

/**
* \brief Creates new applications on the PICC
*
* bOption value can be \n
* \li 01 meaning wISOFileId is supplied \n
* \li 02 meaning pISODFName is present \n
* \li 03 meaning both wISOFileId and pISODFName are present \n
* \li 00 meaning both not present
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateApplication(
                                      void * pDataParams,       /**< [In] Pointer to this layers param structure */
                                      uint8_t bOption,          /**< [In] Option parameter. */
                                      uint8_t * pAid,           /**< [In] array of 3 bytes. */
                                      uint8_t bKeySettings1,    /**< [In] 1 byte. */
                                      uint8_t bKeySettings2,    /**< [In] 1 byte. */
                                      uint8_t * pISOFileId,     /**< [In] 2 btyes ISO File ID. */
                                      uint8_t * pISODFName,     /**< [In] 1 to 16 Bytes. Can also be NULL. */
                                      uint8_t bISODFNameLen     /**< [In] Size of pISODFName if that is present. */
                                      );


/**
* \brief Selects one particular application on the PICC for
* further access
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_SelectApplication(
                                      void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                      uint8_t * pAid        /**< [In] 3 byte AID. LSB First. */
                                      );


/**
* \brief Returns manufacturing related data of the PICC
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_GetVersion(
                               void * pDataParams,  /**< [In] Pointer to this layers param structure. */
                               uint8_t * pVerInfo   /**< [Out] 28bytes of version info. User has to parse this. */
                               );


/** @} */

/**
* \name Application level Commands
*/
/** @{ */


/**
* \brief Creates files for storage of plain unformatted user data within
* an existing application on the PICC
*
* \remarks
* If bOption==1U, it means pIsoFileId is present and is valid.
* If bOption=0, it means pIsoFileId is not present. \n
*
* Communication option (bCommSett) Possible Values are:\n
* \li #PHAL_MFDF_COMMUNICATION_PLAIN
* \li #PHAL_MFDF_COMMUNICATION_ENC
* \li #PHAL_MFDF_COMMUNICATION_MACD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_CreateStdDataFile(
                                      void * pDataParams,       /**< [In] Pointer to this layers param structure. */
                                      uint8_t bOption,          /**< [In] option parameter. 0x00 means wISOFileId is not provided. 0x01 means wISOFileId is provided and is valid */
                                      uint8_t bFileNo,          /**< [In] file number. */
                                      uint8_t * pISOFileId,     /**< [In] ISO File ID. */
                                      uint8_t bCommSett,        /**< [In] communication settings. */
                                      uint8_t * pAccessRights,  /**< [In] 2 byte access rights. Sent LSB first to PICC. */
                                      uint8_t * pFileSize       /**< [In] 3bytes. Sent LSB first to PICC. */
                                      );


/** @} */

/**
* \name Data Manipulation Commands
*/
/** @{ */


phStatus_t phalMfdf_WriteData(
                              void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                              uint8_t bCommOption,  /**< [In] Communication Mode. Plain, Mac'd or encrypted. */
                              uint8_t bFileNo,      /**< [In] 1 byte file number. */
                              uint8_t * pOffset,    /**< [In] 3 bytes offset. LSB First. */
                              uint8_t * pTxData,    /**< [in] Data to be written. */
                              uint8_t * pTxDataLen  /**< [in] 3 bytes. length of data to be written. */
                              );


/** @} */

/**
* \name ISO 7816 COMMANDS
*/
/** @{ */

/**
* \brief ISO Select
*
* \remarks
* bSelector = 0x00 => Selection by 2 byte file Id. \n
* bSelector = 0x02 => Select EF under current DF. Fid = EF id \n
* bSelector = 0x04 => Selection by DF Name. DFName and len is then valid \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phalMfdf_IsoSelectFile(
                                  void * pDataParams,   /**< [In] Pointer to this layers param structure. */
                                  uint8_t bOption,      /**< [In] If bOption == 00 FCI is returned. If 0x0C no FCI returned. */
                                  uint8_t bSelector,    /**< [In] bSelector equals either 0x00 or 0x02 or 0x04. */
                                  uint8_t * pFid,       /**< [In] two byte file id. Send LSB first. */
                                  uint8_t * pDFname,    /**< [In] DFName upto 16 bytes. valid only when bOption = 0x04. */
                                  uint8_t bDFnameLen,   /**< [In] Length of DFName string provided by the user. */
                                  uint8_t ** ppFCI,     /**< [Out] File control information. */
                                  uint16_t * pwFCILen   /**< [Out] Length of FCI returned. */
                                  );

/**
* \brief ISO Read Binary
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_RXCHAINING
*
* If status of #PH_ERR_SUCCESS_CHAINING is returned
* Recall this function with wOption PH_EXCHANGE_RXCHAINING to
* get remaining data.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING operation success with chaining.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoReadBinary(
                                  void * pDataParams,       /**< [In] Pointer to this layers param structure. */
                                  uint16_t wOption,         /**< [In] #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_RXCHAINING. */
                                  uint8_t bOffset,          /**< [In] Offset from where to read. */
                                  uint8_t bSfid,            /**< [In] Short ISO File Id.
                                                                      Bit 7 should be 1 to indicate Sfid is supplied.
                                                                      Else it is treated as MSB of 2Byte offset. */
                                  uint8_t bBytesToRead,     /**< [In] number of bytes to read. If 0, then entire file to be read. */
                                  uint8_t ** ppRxBuffer,    /**< [Out] buffer where the read bytes will be stored. */
                                  uint16_t * pBytesRead     /**< [Out] number of bytes read. */
                                  );

/**
* \brief Iso Update Binary
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfdf_IsoUpdateBinary(
                                    void * pDataParams, /**< [In] Pointer to this layers param structure. */
                                    uint8_t bOffset,    /**< [In] Offset from where to write. */
                                    uint8_t bSfid,      /**< [In] Short ISO File Id.
                                                        Bit 7 should be 1 to indicate Sfid is supplied
                                                        Else it is treated as MSB of 2Byte offset. */
                                    uint8_t * pData,    /**< [In] data to be written. */
                                    uint8_t bDataLen    /**< [In] number of bytes to write. */
                                    );


/** @} */

/**
* \name Miscellaneous functions
*/
/** @{ */

/**
* \brief Perform a GetConfig command.
*
* \c wConfig can be one of:\n
* \li #PHAL_MFDF_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalMfdf_GetConfig(
                              void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wConfig,    /**< [In] Item to read. */
                              uint16_t * pValue    /**< [Out] Read value. */
                              );

/**
* \brief Perform a SetConfig command.
*
* \c wConfig can:\n
* \li #PHAL_MFDF_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalMfdf_SetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wConfig,     /**< [In] Item to set. */
                              uint16_t wValue       /**< [In] Value to set. */
                              );


/** @} */

/** @} */

#endif /* Without optimization */

#endif /* NXPBUILD__PHAL_MFDF */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALMFDF_H */
