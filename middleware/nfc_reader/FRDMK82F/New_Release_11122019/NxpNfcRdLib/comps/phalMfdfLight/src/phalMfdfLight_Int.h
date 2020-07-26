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
* Internal functions of Software implementation of MIFARE DESFire Light application layer.
* $Author: nxp79567 $
* $Revision: 1546 $ (v06.01.00)
* $Date: 2016-02-03 10:32:15 +0530 (Wed, 03 Feb 2016) $
*/

#ifndef PHALMFDFLIGHT_INT_H
#define PHALMFDFLIGHT_INT_H

#include <ph_Status.h>
#include <phalMfdfLight.h>

/** \addtogroup ph_Private
 * @{
 */

/** \name MIFARE DESFire Light Native Commands */
/** @{ */

#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE                                 0x0A    /**< MFDFLIGHT Authenticate command. */
#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE_ISO                             0x1A    /**< MFDFLIGHT Authenticate ISO command. */
#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE_AES                             0xAA    /**< MFDFLIGHT Authenticate AES command. */
#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE_EV2_FIRST                       0x71    /**< MFDFLIGHT Authenticate Ev2 First command. */
#define PHAL_MFDFLIGHT_CMD_AUTHENTICATE_EV2_NON_FIRST                   0x77    /**< MFDFLIGHT Authenticate Ev2 Non First command. */
#define PHAL_MFDFLIGHT_CMD_CHANGE_KEY_SETTINGS                          0x54    /**< MFDFLIGHT Change key settings cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_KEY_SETTINGS                             0x45    /**< MFDFLIGHT Get Key Settings Cmd.  */
#define PHAL_MFDFLIGHT_CMD_CHANGE_KEY                                   0xC4    /**< MFDFLIGHT Change key cmd. */
#define PHAL_MFDFLIGHT_CMD_CHANGE_KEY_EV2                               0xC6    /**< MFDFLIGHT Change key stored on PICC cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_KEY_VERSION                              0x64    /**< MFDFLIGHT Get Key Version. */
#define PHAL_MFDFLIGHT_CMD_INITIALIZE_KEY_SET                           0x56    /**< MFDFLIGHT Initialize a key set cmd. */
#define PHAL_MFDFLIGHT_CMD_FINALIZE_KEY_SET                             0x57    /**< MFDFLIGHT Finalize a key set cmd. */
#define PHAL_MFDFLIGHT_CMD_ROLL_KEY_SET                                 0x55    /**< MFDFLIGHT Roll to a new key set cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_APPLN                                 0xCA    /**< MFDFLIGHT Create Application cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_DELEGATED_APPLN                       0xC9    /**< MFDFLIGHT Create Delegated Application cmd. */
#define PHAL_MFDFLIGHT_CMD_DELETE_APPLN                                 0xDA    /**< MFDFLIGHT Delete Application cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_APPLN_IDS                                0x6A    /**< MFDFLIGHT Get Application Ids cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_DF_NAMES                                 0x6D    /**< MFDFLIGHT Get Dedicated Fine names cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_DELEGATED_INFO                           0x69    /**< MFDFLIGHT Get Delegated info cmd. */
#define PHAL_MFDFLIGHT_CMD_SELECT_APPLN                                 0x5A    /**< MFDFLIGHT Select Application Cmd. */
#define PHAL_MFDFLIGHT_CMD_FORMAT                                       0xFC    /**< MFDFLIGHT Format PICC Cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_VERSION                                  0x60    /**< MFDFLIGHT Get Version cmd. */
#define PHAL_MFDFLIGHT_CMD_FREE_MEM                                     0x6E    /**< MFDFLIGHT Free Memory cmd. */
#define PHAL_MFDFLIGHT_CMD_SET_CONFIG                                   0x5C    /**< MFDFLIGHT Set Configuration Cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_CARD_UID                                 0x51    /**< MFDFLIGHT Get Card UID cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_FILE_IDS                                 0x6F    /**< MFDFLIGHT Get File IDs cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_ISO_FILE_IDS                             0x61    /**< MFDFLIGHT Get ISO File IDs cmd. */
#define PHAL_MFDFLIGHT_CMD_GET_FILE_SETTINGS                            0xF5    /**< MFDFLIGHT Get File settings cmd. */
#define PHAL_MFDFLIGHT_CMD_CHANGE_FILE_SETTINGS                         0x5F    /**< MFDFLIGHT Change file settings cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_TRANSACTION_MACFILE                   0xCE    /**< MFDFLIGHT Create Transaction MAC file cmd. */
#define PHAL_MFDFLIGHT_CMD_DELETE_FILE                                  0xDF    /**< MFDFLIGHT Delete File cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_STD_DATAFILE                          0xCD    /**< MFDFLIGHT Create Standard data file cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_BKUP_DATAFILE                         0xCB    /**< MFDFLIGHT Create Backup data file cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_VALUE_FILE                            0xCC    /**< MFDFLIGHT Create Value File cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_LINEAR_RECFILE                        0xC1    /**< MFDFLIGHT Create Linear record file cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_CYCLIC_RECFILE                        0xC0    /**< MFDFLIGHT Create Cyclic record file cmd. */
#define PHAL_MFDFLIGHT_CMD_CREATE_TRANSTN_MACFILE                       0xCE    /**< MFDFLIGHT Create Transaction MAC file cmd. */
#define PHAL_MFDFLIGHT_CMD_DELETE_FILE                                  0xDF    /**< MFDFLIGHT Delete File cmd. */
#define PHAL_MFDFLIGHT_CMD_READ_DATA                                    0xBD    /**< MFDFLIGHT Read Data cmd. */
#define PHAL_MFDFLIGHT_CMD_READ_DATA_ISO                                0xAD    /**< MFDFLIGHT Read Data cmd using ISO chaining. */
#define PHAL_MFDFLIGHT_CMD_WRITE_DATA                                   0x3D    /**< MFDFLIGHT Write data cmd. */
#define PHAL_MFDFLIGHT_CMD_WRITE_DATA_ISO                               0x8D    /**< MFDFLIGHT Write data cmd using ISO chaining. */
#define PHAL_MFDFLIGHT_CMD_GET_VALUE                                    0x6C    /**< MFDFLIGHT Get Value cmd. */
#define PHAL_MFDFLIGHT_CMD_CREDIT                                       0x0C    /**< MFDFLIGHT Credit cmd. */
#define PHAL_MFDFLIGHT_CMD_DEBIT                                        0xDC    /**< MFDFLIGHT Debit cmd. */
#define PHAL_MFDFLIGHT_CMD_LIMITED_CREDIT                               0x1C    /**< MFDFLIGHT Limited Credit cmd. */
#define PHAL_MFDFLIGHT_CMD_WRITE_RECORD                                 0x3B    /**< MFDFLIGHT Write Record cmd. */
#define PHAL_MFDFLIGHT_CMD_WRITE_RECORD_ISO                             0x8B    /**< MFDFLIGHT Write Record cmd using ISO chaining. */
#define PHAL_MFDFLIGHT_CMD_UPDATE_RECORD                                0xDB    /**< MFDFLIGHT Update Record cmd. */
#define PHAL_MFDFLIGHT_CMD_UPDATE_RECORD_ISO                            0xBA    /**< MFDFLIGHT Update Record cmd using ISO chaining. */
#define PHAL_MFDFLIGHT_CMD_READ_RECORDS                                 0xBB    /**< MFDFLIGHT Read Records cmd. */
#define PHAL_MFDFLIGHT_CMD_READ_RECORDS_ISO                             0xAB    /**< MFDFLIGHT Read Records cmd using ISO chaining. */
#define PHAL_MFDFLIGHT_CMD_UPDATE_RECORDS                               0xF0    /**< MFDFLIGHT Update Records cmd. */
#define PHAL_MFDFLIGHT_CMD_CLEAR_RECORD_FILE                            0xEB    /**< MFDFLIGHT Clear records file cmd. */
#define PHAL_MFDFLIGHT_CMD_COMMIT_TXN                                   0xC7    /**< MFDFLIGHT Commit transaction cmd. */
#define PHAL_MFDFLIGHT_CMD_ABORT_TXN                                    0xA7    /**< MFDFLIGHT Abort transaction cmd. */
#define PHAL_MFDFLIGHT_CMD_COMMIT_READER_ID                             0xC8    /**< MFDFLIGHT Commit Reader ID cmd. */
#define PHAL_MFDFLIGHT_CMD_READ_SIG                                     0x3C    /**< MFDFLIGHT Verify read signature command. */
#define PHAL_MFDFLIGHT_CMD_FORMAT_PICC                                  0xFC    /**< MFDFLIGHT Format PICC Cmd. */
#define PHAL_MFDFLIGHT_CMD_CLEAR_RECORDS_FILE                           0xEB    /**< MFDFLIGHT Clear records file cmd. */
#define PHAL_MFDFLIGHT_CMD_APPLY_SM                                     0xAE    /**< MFDFLIGHT Apply Secure Messaging cmd. */
#define PHAL_MFDFLIGHT_CMD_UNDO_SM                                      0xAD    /**< MFDFLIGHT Undo Secure Messaging cmd. */
/** @}*/

/** \name MIFARE DESFire Light Response Codes */
/** @{ */

#define PHAL_MFDFLIGHT_RESP_OPERATION_OK                                0x00U   /**< MFDFLIGHT Response - Successful operation. */
#define PHAL_MFDFLIGHT_RESP_OK                                          0x90U   /**< MFDFLIGHT Response - Successful operation. */
#define PHAL_MFDFLIGHT_RESP_NO_CHANGES                                  0x0CU   /**< MFDFLIGHT Response - No changes done to backup files. */
#define PHAL_MFDFLIGHT_RESP_ERR_OUT_OF_EEPROM_ERROR                     0x0EU   /**< MFDFLIGHT Response - Insufficient NV-Memory. */
#define PHAL_MFDFLIGHT_RESP_ILLEGAL_COMMAND_CODE                        0x1CU   /**< MFDFLIGHT command code not supported. */
#define PHAL_MFDFLIGHT_RESP_ERR_INTEGRITY_ERROR                         0x1EU   /**< MFDFLIGHT CRC or MAC does not match data padding bytes not valid. */
#define PHAL_MFDFLIGHT_RESP_NO_SUCH_KEY                                 0x40U   /**< MFDFLIGHT Invalid key number specified. */
#define PHAL_MFDFLIGHT_RESP_CHAINING                                    0x71U   /**< MFDFLIGHT ISO Chaining Status. */
#define PHAL_MFDFLIGHT_RESP_ERR_LENGTH_ERROR                            0x7EU   /**< MFDFLIGHT Length of command string invalid. */
#define PHAL_MFDFLIGHT_RESP_PERMISSION_DENIED                           0x9DU   /**< MFDFLIGHT Current configuration/status does not allow the requested command. */
#define PHAL_MFDFLIGHT_RESP_ERR_PARAMETER_ERROR                         0x9EU   /**< MFDFLIGHT Value of params invalid. */
#define PHAL_MFDFLIGHT_RESP_APPLICATION_NOT_FOUND                       0xA0U   /**< MFDFLIGHT Requested AID not found on PICC. */
#define PHAL_MFDFLIGHT_RESP_ERR_APPL_INTEGRITY_ERROR                    0xA1U   /**< MFDFLIGHT Unrecoverable error within application, appln will be disabled. */
#define PHAL_MFDFLIGHT_RESP_ERR_AUTHENTICATION_ERROR                    0xAEU   /**< MFDFLIGHT Current authentication status does not allow the requested cmd. */
#define PHAL_MFDFLIGHT_RESP_ERR_AUTHENTICATION_DELAY                    0xADU   /**< Keep Waiting till Delay is met. */
#define PHAL_MFDFLIGHT_RESP_ADDITIONAL_FRAME                            0xAFU   /**< MFDFLIGHT Additional data frame is expected to be sent. */
#define PHAL_MFDFLIGHT_RESP_ERR_BOUNDARY_ERROR                          0xBEU   /**< MFDFLIGHT Attempt to read/write data from/to beyond the files/record's limits. */
#define PHAL_MFDFLIGHT_RESP_ERR_PICC_INTEGRITY                          0xC1U   /**< MFDFLIGHT Unrecoverable error within PICC. PICC will be disabled. */
#define PHAL_MFDFLIGHT_RESP_ERR_COMMAND_ABORTED                         0xCAU   /**< MFDFLIGHT Previous cmd not fully completed. Not all frames were requested or provided by the PCD. */
#define PHAL_MFDFLIGHT_RESP_ERR_PIC_DISABLED                            0xCDU   /**< MFDFLIGHT PICC was disabled by an unrecoverable error. */
#define PHAL_MFDFLIGHT_RESP_ERR_COUNT                                   0xCEU   /**< MFDFLIGHT Num. of applns limited to 28. No additional applications possible. */
#define PHAL_MFDFLIGHT_RESP_ERR_DUPLICATE                               0xDEU   /**< MFDFLIGHT File/Application with same number already exists. */
#define PHAL_MFDFLIGHT_RESP_ERR_EEPROM                                  0xEEU   /**< MFDFLIGHT Could not complete NV-Write operation due to loss of power. */
#define PHAL_MFDFLIGHT_RESP_ERR_FILE_NOT_FOUND                          0xF0U   /**< MFDFLIGHT Specified file number does not exist. */
#define PHAL_MFDFLIGHT_RESP_ERR_FILE_INTEGRITY                          0xF1U   /**< MFDFLIGHT Unrecoverable error within file. File will be disabled. */
/** @} */

/** \name ISO 7816 Instructions */
/** @{ */
#define PHAL_MFDFLIGHT_CMD_ISO7816_SELECT_FILE                          0xA4    /**< ISO Select File. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_READ_RECORDS                         0xB2    /**< ISO Read records. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_READ_BINARY                          0xB0    /**< ISO Read Binary. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_UPDATE_BINARY                        0xD6    /**< ISO UPDATE Binary. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_APPEND_RECORD                        0xE2    /**< ISO Append record. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_UPDATE_RECORD                        0xD2    /**< ISO Update record. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_GET_CHALLENGE                        0x84    /**< ISO Get challenge. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_EXT_AUTHENTICATE                     0x82    /**< ISO Ext. Authenticate. */
#define PHAL_MFDFLIGHT_CMD_ISO7816_INT_AUTHENTICATE                     0x88    /**< ISO Int. Authenticate. */
/** @} */

/** \name MIFARE DESFire Light specific defines */
/** @{ */
#define PHAL_MFDFLIGHT_MAXWRAPPEDAPDU_SIZE                              0x37u   /**< Maximum size of wrapped APDU 55 bytes. */
#define PHAL_MFDFLIGHT_MAXDFAPDU_SIZE                                   0x3Cu   /**< Maximum size of DESFire APDU 60 bytes. */
#define PHAL_MFDFLIGHT_DFAPPID_SIZE                                     0x03u   /**< Size of MFDFLIGHT application Id. */
#define PHAL_MFDFLIGHT_DATA_BLOCK_SIZE                                  0x10u   /**< Data block size need for internal purposes. */
#define PHAL_MFDFLIGHT_MAX_FRAME_SIZE                                   0x40u   /**< Max size in a ISO 14443-4 frame. */
/** @} */

/**  \name ISO 7816 MIFARE DESFire Light return Codes */
/** @{ */
#define PHAL_MFDFLIGHT_ISO7816_SUCCESS                                  0x9000U /**< Correct execution. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_WRONG_LENGTH                         0x6700U /**< Wrong length. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_INVALID_APPLN                        0x6A82U /**< Application / file not found. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_WRONG_PARAMS                         0x6A86U /**< Wrong parameters P1 and/or P2. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_WRONG_LC                             0x6A87U /**< Lc inconsistent with P1/p2. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_WRONG_LE                             0x6C00U /**< Wrong Le. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS               0x6F00U /**< No precise diagnostics. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_EOF_REACHED                          0x6282U /**< End of File reached. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_LIMITED_FUNCTIONALITY_INS            0x6283U /**< Limited Functionality. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_FILE_ACCESS                          0x6982U /**< File access not allowed. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_FILE_EMPTY                           0x6985U /**< File empty or access conditions not satisfied. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_FILE_NOT_FOUND                       0x6A82U /**< File not found. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_MEMORY_FAILURE                       0x6581U /**< Memory failure (unsuccessful update). */
#define PHAL_MFDFLIGHT_ISO7816_ERR_INCORRECT_PARAMS                     0x6B00U /**< Wrong parameter p1 or p2. READ RECORDS. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_WRONG_CLA                            0x6E00U /**< Wrong Class byte. */
#define PHAL_MFDFLIGHT_ISO7816_ERR_UNSUPPORTED_INS                      0x6D00U /**< Instruction not supported. */
/*@}*/

#define PHAL_MFDFLIGHT_WRAP_HDR_LEN                                     0x05U   /* Wrapped APDU header length */
#define PHAL_MFDFLIGHT_WRAPPEDAPDU_CLA                                  0x90U   /* Wrapped APDU default class. */
#define PHAL_MFDFLIGHT_WRAPPEDAPDU_P1                                   0x00U   /* Wrapped APDU default P1. */
#define PHAL_MFDFLIGHT_WRAPPEDAPDU_P2                                   0x00U   /* Wrapped APDU default P2. */
#define PHAL_MFDFLIGHT_WRAPPEDAPDU_LE                                   0x00U   /* Wrapped APDU default LE. */

#define PHAL_MFDFLIGHT_TRUNCATED_MAC_SIZE                                   8   /**< Size of the truncated MAC. */
#define PHAL_MFDFLIGHT_KEYSETVERSIONS                                   0x40U   /**< 6th bit KeyNo used to retrieve all keyset versions. */
#define PHAL_MFDFLIGHT_ISO_CHAINING_MODE                                0x04    /**< Option to enable ISO chaining. */
#define PHAL_MFDFLIGHT_DEFAULT_MODE                                     0x00    /**< Native Chaining. */
#define PHAL_MFDFLIGHT_PC_RND_LEN                                           7   /**< Size of the Proximity Check Random numbers. */
#define PHAL_MFDFLIGHT_SIG_LENGTH                                       0x38    /**< NXP Originality Signature length */
#define PHAL_MFDFLIGHT_DEFAULT_UID_LENGTH                               0x07    /**< By default EV2 card is configures as 7 byte UID */
#define PHAL_MFDFLIGHT_10B_UID_LENGTH                                   0x09    /**< EV2 can also be configured as 10 byte UID. */
#define PHAL_MFDFLIGHT_4B_UID_LENGTH                                    0x06    /**< EV2 can also be configured as 4 byte UID. */
#define PHAL_MFDFLIGHT_DEF_VERSION_LENGTH                               0x1C    /**< Version String is of 28 bytes by default(If its 7 Byte UID) */
#define PHAL_MFDFLIGHT_VERSION_LENGTH_WITH_FABKEY_ID                    0x1D    /**< Version String is of 29 bytes by default(If its 7 Byte UID) */
#define PHAL_MFDFLIGHT_10B_VERSION_LENGTH                               0x1E    /**< Version String is of 30 bytes If its 10B Byte UID */
#define PHAL_MFDFLIGHT_4B_VERSION_LENGTH                                0x1B    /**< Version String is of 27 bytes If its 4B Byte UID */
#define PHAL_MFDFLIGHT_ISO_CHAINING_MODE_MAPPED (PHAL_MFDFLIGHT_ISO_CHAINING_MODE << 2) /**< Option to enable ISO chaining mapped for internal use. */

#define PHAL_MFDFLIGHT_ISO_7816_NO_LC_LE                                0x00    /**< Check if Lc=0 and Le=0 */
#define PHAL_MFDFLIGHT_ISO_7816_LC_SHORT_APDU                           0x01    /**< Indicates Lc=1 byte */
#define PHAL_MFDFLIGHT_ISO_7816_LC_EXT_APDU_3B                          0x03    /**< Indicates Lc=3 bytes */
#define PHAL_MFDFLIGHT_ISO_7816_LE_SHORT_APDU    PHAL_MFDFLIGHT_ISO_7816_LC_SHORT_APDU  /**< Indicates Le=1 byte */
#define PHAL_MFDFLIGHT_ISO_7816_LE_EXT_APDU_2B                          0x02    /**< Indicates Le=2 bytes */
#define PHAL_MFDFLIGHT_ISO_7816_LE_EXT_APDU_3B PHAL_MFDFLIGHT_ISO_7816_LC_EXT_APDU_3B   /**< Indicates Le=3 bytes */
/**
* \name Proximity Check return Codes
*/
/*@{*/
#define PHAL_MFDFLIGHT_RESP_NACK0                                       0x00U   /*< MFDFLIGHT NACK 0 (in ISO14443-3 mode). */
#define PHAL_MFDFLIGHT_RESP_NACK1                                       0x01U   /*< MFDFLIGHT NACK 1 (in ISO14443-3 mode). */
#define PHAL_MFDFLIGHT_RESP_NACK4                                       0x04U   /*< MFDFLIGHT NACK 4 (in ISO14443-3 mode). */
#define PHAL_MFDFLIGHT_RESP_NACK5                                       0x05U   /*< MFDFLIGHT NACK 5 (in ISO14443-3 mode). */
#define PHAL_MFDFLIGHT_RESP_ACK_ISO3                                    0x0AU   /*< MFDFLIGHT ACK (in ISO14443-3 mode). */
#define PHAL_MFDFLIGHT_RESP_ACK_ISO4                                    0x90U   /*< MFDFLIGHT ACK (in ISO14443-4 mode). */
#define PHAL_MFDFLIGHT_RESP_ERR_AUTH                                    0x06U   /*< MFDFLIGHT Authentication Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_CMD_OVERFLOW                            0x07U   /*< MFDFLIGHT Command Overflow Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_MAC_PCD                                 0x08U   /*< MFDFLIGHT MAC Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_BNR                                     0x09U   /*< MFDFLIGHT Blocknumber Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_EXT                                     0x0AU   /*< MFDFLIGHT Extension Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_CMD_INVALID                             0x0BU   /*< MFDFLIGHT Invalid Command Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_FORMAT                                  0x0CU   /*< MFDFLIGHT Format Error. */
#define PHAL_MFDFLIGHT_RESP_ERR_GEN_FAILURE                             0x0FU   /*< MFDFLIGHT Generic Error. */
/*@}*/

/* To avoid unused variable warnings. */
#define PHAL_MFDFLIGHT_UNUSED_VARIABLE(x)   {for( ( x ) = ( x ) ; ( x ) != ( x ) ; );}
#define PHAL_MFDFLIGHT_UNUSED_ARRAY(x)      {for( ( (x)[0] ) = ( (x)[0] ) ; ( (x)[0] ) != ( (x)[0] ) ; );}

phStatus_t phalMfdfLight_Int_ComputeErrorResponse(void * pDataParams, uint16_t wStatus);

phStatus_t phalMfdfLight_ExchangeCmd(void * pDataParams, void * pPalMifareDataParams, uint8_t bWrappedMode, uint8_t * pCmd, uint16_t wCmdLen,
    uint8_t ** ppResponse, uint16_t * wRxlen);

phStatus_t phalMfdfLight_Int_Send7816Apdu(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t bIns, uint8_t p1, uint8_t p2,
    uint32_t Lc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint32_t bLe, uint8_t ** pDataOut, uint16_t *pDataLen);

phStatus_t phalMfdfLight_Int_ISOSelectFile(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t * bCmdBuff, uint16_t wCmdLen,
    uint8_t bLc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint8_t bLe, uint8_t ** ppDataOut, uint16_t *pDataLen);

phStatus_t phalMfdfLight_Int_ISOUpdateBinary(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t * bCmdBuff, uint16_t wCmdLen,
    uint32_t dwLc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint8_t ** ppDataOut, uint16_t *pDataLen);

/** @} */

#endif /* PHALMFDFLIGHT_INT_H */
