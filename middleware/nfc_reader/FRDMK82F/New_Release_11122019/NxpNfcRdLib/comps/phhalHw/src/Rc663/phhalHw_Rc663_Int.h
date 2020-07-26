/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
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
* Internal definitions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $ (v06.01.00)
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_RC663_INT_H
#define PHHALHW_RC663_INT_H

#include <ph_Status.h>
#include <phhalHw.h>
#include "phhalHw_Rc663_Config.h"

/** \defgroup phhalHw_Rc663_Int Internal
* \brief Internal definitions.
* @{
*/

/** \name Timing related constants
*/
/*@{*/
#define PHHAL_HW_RC663_TIMER_RELOAD_MAX                 0xFFFFU     /**< Maximum reload value of internal timers. */
#define PHHAL_HW_RC663_TIMER0_FREQ                      13.56f      /**< Internal timer frequency of Timer0. */
#define PHHAL_HW_RC663_ETU_I14443                       9.44f       /**< Duration of one ETU for ISO14443 at 106 kBit/s in [us]. */
#define PHHAL_HW_RC663_ETU_212                          4.7f        /**< Duration of one ETU at 212 kBit/s in [us]. */
#define PHHAL_HW_RC663_ETU_424                          2.4f        /**< Duration of one ETU at 424 kBit/s in [us]. */
#define PHHAL_HW_RC663_ETU_848                          1.2f        /**< Duration of one ETU at 848 kBit/s in [us]. */
#define PHHAL_HW_RC663_ETU_I15693_HIGH                  18.88f      /**< Duration of one ETU for ISO15693 in [us]. */
#define PHHAL_HW_RC663_ETU_I15693                       37.76f      /**< Duration of one ETU for ISO15693 in [us]. */
#define PHHAL_HW_RC663_ETU_EPCUID                       37.76f      /**< Duration of one ETU for EPC/UID in [us]. */
#define PHHAL_HW_RC663_ETU_I18000                       9.44f       /**< Duration of one ETU for 18000p3m3 in [us]. */
#define PHHAL_HW_RC663_I14443_ADD_DELAY_US              15U         /**< Additional digital timeout delay for ISO14443. */
#define PHHAL_HW_RC663_I15693_ADD_DELAY_US              120U        /**< Additional digital timeout delay for ISO15693. */
#define PHHAL_HW_RC663_I18000P3M3_CRC_TYPE_PRESET       0x7AU       /**< CRC type and preset register value for I18000p3m3. */
#define PHHAL_HW_RC663_RXTX_TIMER_START                 0x0010U     /**< Start timer manually. */
/*@}*/

/** \name Memory related constants
*/
/*@{*/
#define PHHAL_HW_RC663_MAX_NUM_REGS             0x80U   /**< Maximum number of registers. */
#define PHHAL_HW_RC663_MAX_NUM_KEYS             0x100U  /**< Maximum possible number of keys in the EEPROM. */
#define PHHAL_HW_RC663_MAX_TX_SETTINGS          32U     /**< Maximum possible number of Tx-Settings in the EEPROM. */
#define PHHAL_HW_RC663_MAX_RX_SETTINGS          64U     /**< Maximum possible number of Rx-Settings in the EEPROM. */
#define PHHAL_HW_RC663_EEPROM_SIZE              0x2000U /**< Size of the EEPROM. */
#define PHHAL_HW_RC663_EEPROM_SECTION2_BEGIN    0xC0U   /**< Beginning of Section2 in the EEPROM. */
#define PHHAL_HW_RC663_EEPROM_SECTION2_END      0x17FFU /**< End of Section2 in the EEPROM. */
#define PHHAL_HW_RC663_EEPROM_NUM_PAGES         0x80U   /**< Number of pages within EEPROM. */
#define PHHAL_HW_RC663_EEPROM_PAGE_SIZE         0x40U   /**< Size of a page in EEPROM. */
/*@}*/

/** \name RXTX settings for #PHHAL_HW_RC663_CMD_LOADPROTOCOL
*/
/*@{*/
#define PHHAL_HW_RC663_RXTX_I14443A_106     0x00U   /**< ISO14443A Operating mode at 106kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443A_212     0x01U   /**< ISO14443A Operating mode at 212kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443A_424     0x02U   /**< ISO14443A Operating mode at 414kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443A_848     0x03U   /**< ISO14443A Operating mode at 848kbit/s. */

#define PHHAL_HW_RC663_RXTX_I14443B_106     0x04U   /**< ISO14443B Operating mode at 106kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443B_212     0x05U   /**< ISO14443B Operating mode at 212kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443B_424     0x06U   /**< ISO14443B Operating mode at 414kbit/s. */
#define PHHAL_HW_RC663_RXTX_I14443B_848     0x07U   /**< ISO14443B Operating mode at 848kbit/s. */

#define PHHAL_HW_RC663_RXTX_FELICA_212      0x08U   /**< FeliCa Operating mode at 212kbit/s. */
#define PHHAL_HW_RC663_RXTX_FELICA_424      0x09U   /**< FeliCa Operating mode at 424kbit/s. */

#define PHHAL_HW_RC663_RXTX_I15693_1OO4     0x0AU   /**< ISO15693 One-Out-Of-Four Operating mode. */
#define PHHAL_HW_RC663_RXTX_I15693_1OO256   0x0CU   /**< ISO15693 One-Out-Of-256 Operating mode. */
#define PHHAL_HW_RC663_RXTX_I15693_HIGH_SSC 0x0AU   /**< ISO15693 High-Speed (RX) Operating mode (Single Subcarrier). */
#define PHHAL_HW_RC663_RXTX_I15693_FAST     0x0BU   /**< ISO15693 Fast-Speed (RX) Operating mode (Single Subcarrier). */
#define PHHAL_HW_RC663_RXTX_I15693_HIGH_DSC 0x0CU   /**< ISO15693 High-Speed (RX) Operating mode (Dual Subcarrier). */

#define PHHAL_HW_RC663_RXTX_EPC_UID 0x0DU   /**< ICode EPC/UID. */

#define PHHAL_HW_RC663_RXTX_I18000P3M3_DS_M2    0x0EU   /**< ICode ISO18000-3 Mode3 424 kBit/s (M=2). */
#define PHHAL_HW_RC663_RXTX_I18000P3M3_DS_M4    0x0FU   /**< ICode ISO18000-3 Mode3 424 kBit/s (M=4). */
#define PHHAL_HW_RC663_RXTX_I18000P3M3_QS_M2    0x10U   /**< ICode ISO18000-3 Mode3 848 kBit/s (M=2). */
#define PHHAL_HW_RC663_RXTX_I18000P3M3_QS_M4    0x11U   /**< ICode ISO18000-3 Mode3 848 kBit/s (M=4). */
/*@}*/

/** \name RS232 speed settings
*/
/*@{*/
#define PHHAL_HW_RC663_SERIALSPEED_7200     0xFAU   /**< 7.200 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_9600     0xEBU   /**< 9.600 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_14400    0xDAU   /**< 14.400 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_19200    0xCBU   /**< 19.200 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_38400    0xABU   /**< 38.400 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_57600    0x9AU   /**< 57.500 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_115200   0x7AU   /**< 115.200 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_128000   0x74U   /**< 128.000 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_230400   0x5AU   /**< 230.400 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_460800   0x3AU   /**< 460.800 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_921600   0x1CU   /**< 921.600 kBit/s. */
#define PHHAL_HW_RC663_SERIALSPEED_1228800  0x15U   /**< 1.228.800 kBit/s. */
/*@}*/

/** \name MIFARE(R) Classic contactless IC stuff
*/
/*@{*/
#define PHHAL_HW_RC663_MFC_AUTHA_CMD        0x60U   /**< MIFARE(R) Classic AUTHA command code. */
#define PHHAL_HW_RC663_MFC_AUTHB_CMD        0x61U   /**< MIFARE(R) Classic AUTHB command code. */
#define PHHAL_HW_RC663_MFC_KEY_LENGTH       0x06U   /**< Lengh of a MIFARE(R) Classic key. */
/*@}*/

/** \name Custom command option
*/
/*@{*/
#define PHHAL_HW_RC663_OPTION_FIFO_NOCLEAR  0x0080U /**< Do not clear FIFO prior to command execution. */
/*@}*/

/** \name RC663 Derivatives IC ID
*/
/*@{*/
#define PHHAL_HW_CLRC663_PRODUCT_ID        0x01U   /**< Product ID for CLRC663. */
#define PHHAL_HW_MFRC631_PRODUCT_ID        0xC0U   /**< Product ID for MFRC631. */
#define PHHAL_HW_CLRC661_PRODUCT_ID        0xE0U   /**< Product ID for CLRC661. */
#define PHHAL_HW_MFRC630_PRODUCT_ID        0x80U   /**< Product ID for MFRC630. */
#define PHHAL_HW_SLRC610_PRODUCT_ID        0x20U   /**< Product ID for SLRC610. */
#define PHHAL_HW_CTN53003_PRODUCT_ID       0xB0U   /**< Product ID for CTN53003. */
/*@}*/

/** \name RC663 Derivative and Version values and address
*/
/*@{*/
#define PHHAL_HW_CLRC663_PRODUCT_ID_ADD     0x1U    /**< EEPROM Address of CLRC663 derivative Product ID. */
#define PHHAL_HW_CLRC663_PRODUCT_ID_LEN     0x1U    /**< Length of CLRC663 derivative Product ID to be read from EEPROM. */
#define PHHAL_HW_CLRC663PLUS_VERSION        0x1AU   /**< Version of RC66303 (RC663 Plus). Refer VERSION Register (Address : 0x7F) for more information. */
/*@}*/

/** \name Position and Mask of Version and Derivative in variable wDerivative of structure phhalHw_Rc663_DataParams_t.
 */
/*@{*/
#define PHHAL_HW_VERSION_POS                8U
#define PHHAL_HW_VERSION_MASK               0xFF00U
#define PHHAL_HW_DERIVATIVE_POS             0U
#define PHHAL_HW_DERIVATIVE_MASK            0x00FFU
/*@}*/

/* Emvco: Macros for EMVCO EMD Noise Handling */
#define PHHAL_HW_EMD_NOISE_ERROR             (PHHAL_HW_RC663_BIT_COLLDET | PHHAL_HW_RC663_BIT_INTEGERR | PHHAL_HW_RC663_BIT_PROTERR)

#define PHHAL_HW_EMD_NOISE_CHECK(dwReg)                      \
    (                                               \
    (((dwReg) & PHHAL_HW_EMD_NOISE_ERROR) != 0U)              \
    ? 1U : 0U                                         \
    )

#define PHHAL_HW_RC663_EMD_NOISE_MAXLEN             0x02U /**< EMVCo or NFC Mode EMD. */
#define PHHAL_HW_RC663_EMD_NOISE_MAXLEN_ISO_MODE    0x01U /**< ISO Mode EMD. */

/**
* ISO 18000p3m3 Sub-carrier type
*/
#define PHHAL_HW_RC663_I18000P3M3_M_MANCHESTER_2     0x02U
#define PHHAL_HW_RC663_I18000P3M3_M_MANCHESTER_4     0x03U

/**
* ISO 18000p3m3 Timeout constants
*/
#define PHHAL_HW_RC663_I18000P3M3_SW_T1_MAX_US      78U
#define PHHAL_HW_RC663_I18000P3M3_SW_T2_MIN_US      151U
#define PHHAL_HW_RC663_I18000P3M3_SW_T3_MIN_US      25U
#define PHHAL_HW_RC663_I18000P3M3_SW_T4_MIN_US      100U
#define PHHAL_HW_RC663_I18000P3M3_SW_DELTA_MIN_US   10U

/**
* ISO 18000p3m3 Command Codes
*/
#define PHHAL_HW_RC663_I18000P3M3_CMD_NEXT_SLOT      0x00U
#define PHHAL_HW_RC663_I18000P3M3_CMD_ACK            0x01U
#define PHHAL_HW_RC663_I18000P3M3_CMD_REQ_RN         0xC1U

/**
 * \name Index for ERPROM address in the array \ref gkphhalHw_Rc663_LoadConfig
 */
/*@{*/
#define PHHAL_HW_RC663_ARR_INDEX_I14443A_106          0x00U
#define PHHAL_HW_RC663_ARR_INDEX_I14443A_212          0x01U
#define PHHAL_HW_RC663_ARR_INDEX_I14443A_424          0x02U
#define PHHAL_HW_RC663_ARR_INDEX_I14443A_848          0x03U
#define PHHAL_HW_RC663_ARR_INDEX_I14443B_106          0x04U
#define PHHAL_HW_RC663_ARR_INDEX_I14443B_EMVCO_106    0x05U
#define PHHAL_HW_RC663_ARR_INDEX_I14443B_212          0x06U
#define PHHAL_HW_RC663_ARR_INDEX_I14443B_424          0x07U
#define PHHAL_HW_RC663_ARR_INDEX_I14443B_848          0x08U
#define PHHAL_HW_RC663_ARR_INDEX_FELICA_212           0x09U
#define PHHAL_HW_RC663_ARR_INDEX_FELICA_424           0x0AU
#define PHHAL_HW_RC663_ARR_INDEX_I15693_HIGH          0x0BU
#define PHHAL_HW_RC663_ARR_INDEX_I15693_FAST_HIGH     0x0CU
#define PHHAL_HW_RC663_ARR_INDEX_EPC_UID              0x0DU
#define PHHAL_HW_RC663_ARR_INDEX_I18000P3M3_DSM2      0x0EU
#define PHHAL_HW_RC663_ARR_INDEX_I18000P3M3_DSM4      0x0FU
#define PHHAL_HW_RC663_ARR_INDEX_I18000P3M3_QSM2      0x10U
#define PHHAL_HW_RC663_ARR_INDEX_I18000P3M3_QSM4      0x11U
/*@}*/

/**
* \brief Validate the FIFO Buffer space in Rc663 and perform \ref phhalHw_Rc663_WriteData.
*
* Note that in opposite to \ref phhalHw_Rc663_ReadFifo, this command only supports up to 255 bytes.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_WriteFifo(
                                   phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pData,                             /**< [In] input data. */
                                   uint16_t wLength,                            /**< [In] length of input data. */
                                   uint16_t * pBytesWritten                     /**< [Out] Number of written bytes. */
                                   );

/**
* \brief Read Data from Fifo buffer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_ReadFifo(
                                  phhalHw_Rc663_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wDataSize,                       /**< [In] size of data buffer. */
                                  uint8_t * pData,                          /**< [Out] output data. */
                                  uint16_t * pLength                        /**< [Out] number of output data bytes. */
                                  );

/**
* \brief Clear Fifo buffer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_FlushFifo(
                                   phhalHw_Rc663_DataParams_t * pDataParams /**< [In] Pointer to this layer's parameter structure. */
                                   );

/**
* \brief Perform actual Write to Rc663 FIFO with the data passed.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_WriteData(
                                   phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bAddress,                            /**< [In] Start-Register Address. */
                                   uint8_t * pData,                             /**< [In] Register Values; uint8_t[wLength]. */
                                   uint16_t wLength                             /**< [In] Number of Values. */
                                   );

/**
* \brief Perform actual read from Rc663 FIFO for the address passed.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_ReadData(
                                  phhalHw_Rc663_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bAddress,                         /**< [In] Register Address. */
                                  uint16_t wLength,                         /**< [In] Number of Values. */
                                  uint8_t * pData                           /**< [Out] Register Values; uint8_t[wLength]. */
                                  );

/**
* \brief Apply card mode register settings (Tx and Rx Data Rate) according to given parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_SetCardMode(
    phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wTxDataRate,                       /**< [In] TxDataRate HAL config. */
    uint16_t wRxDataRate,                       /**< [In] RxDataRate HAL config. */
    uint16_t wSubcarrier                        /**< [In] SubCarrier HAL config. */
    );

/**
* \brief Set configuration parameter (Internal).
*
* This is needed since \ref phhalHw_Rc663_SetCardMode would cause a recursive
* call to \ref phhalHw_SetConfig which is bad for 805x based systems.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_PARAMETER_OVERFLOW Setting the parameter value would lead to an overflow.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_Rc663_SetConfig_Int(
                                       phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                       uint16_t wConfig,                            /**< [In] Configuration Identifier. */
                                       uint16_t wValue                              /**< [In] Configuration Value. */
                                       );

/**
* \brief Sets the RxWait time.
*
* \b Note: RxWait is defined between the last transmitted bit and the activation of the receiver.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_SetRxWait(
                                   phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wTimeEtu                            /**< [In] RxWait time in ETUs. */
                                   );

/**
* \brief Sets the TxWait time in microseconds.
*
* \b Note: TxWait is defined between the last received bit and the next transmitted bit.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_SetTxWait(
                                   phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wTimeUs                             /**< [In] TxWait time in microseconds. */
                                   );

/**
* \brief Sets the Frame Delay Time (Timeout).
*
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_SetFdt(
                                phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                uint8_t bUnit,                              /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
                                uint16_t wTimeout                           /**< [In] Timeout value. */
                                );

/**
* \brief Retrieves the Frame Delay Time of the last command.
*
* \b Note: Frame Delay Time is defined between the last transmitted bit and the first received bit.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_GetFdt(
                                phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                phStatus_t wExchangeStatus,                 /**< [In] Status code returned by exchange function. */
                                uint32_t * pTime                            /**< [Out] Calculated time in microseconds from timer contents. */
                                );

/**
* \brief Returns the delay of the digital circuitry for the current protocol.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Rc663_GetDigiDelay(
                                      phhalHw_Rc663_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t bIsTimeout,                       /**< [In] Wheter the returned delay is added to a timeout (#PH_ON) or is used for measurement (#PH_OFF). */
                                      uint16_t * pDelayUs                       /**< [Out] The digital delay in microseconds. */
                                      );

/**
* \brief Returns the TxBuffer pointer, length and size.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Rc663_GetTxBuffer(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bIsExchange,                       /**< [In] Whether this is for Exchange (#PH_ON) or not (#PH_OFF). */
                                     uint8_t ** pTxBuffer,                      /**< [Out] Pointer to the TxBuffer. */
                                     uint16_t * pTxBufferLen,                   /**< [Out] Number of bytes already in the TxBuffer. */
                                     uint16_t * pTxBufferSize                   /**< [Out] Size of the TxBuffer. */
                                     );

/**
* \brief Returns the RxBuffer pointer, length and size.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Rc663_GetRxBuffer(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bIsExchange,                       /**< [In] Whether this is for Exchange (#PH_ON) or not (#PH_OFF). */
                                     uint8_t ** pRxBuffer,                      /**< [Out] Pointer to the RxBuffer. */
                                     uint16_t * pRxBufferLen,                   /**< [Out] Number of bytes already in the RxBuffer. */
                                     uint16_t * pRxBufferSize                   /**< [Out] Size of the RxBuffer. */
                                     );

/**
* \brief Build and Execute MIFARE(R) Classic Authentication command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_Rc663_MfcAuthenticate_Int(
    phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bBlockNo,                           /**< [In] Block-number on card to authenticate to. */
    uint8_t bKeyType,                           /**< [In] Either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
    uint8_t * pUid                              /**< [In] Serial number of current cascade level; uint8_t[4]. */
    );

/**
* \brief Execute a Reader-specific command.
*
* Internal function for executing all commands except Transceive/Transmit/Receive.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_Rc663_Command_Int(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bCmd,                              /**< [In] Command code. */
                                     uint16_t wOption,                          /**< [In] Option parameter. */
                                     uint8_t bIrq0WaitFor,                      /**< [In] Bitmask of interrupts in Irq0 register to wait for. */
                                     uint8_t bIrq1WaitFor,                      /**< [In] Bitmask of interrupts in Irq1 register to wait for. */
                                     uint8_t * pTxBuffer,                       /**< [In] Data to transmit. */
                                     uint16_t wTxLength,                        /**< [In] Length of input data. */
                                     uint16_t wRxBufferSize,                    /**< [In] Size of receive buffer. */
                                     uint8_t * pRxBuffer,                       /**< [Out] Pointer to received data. */
                                     uint16_t * pRxLength                       /**< [Out] Number of received data bytes. */
                                     );

/**
* \brief Wait until one of the given interrupts occurs.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_Rc663_WaitIrq(
                                 phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t bEnableIrqs,                       /**< [In] Whether to enable (#PH_ON) Irqs to wait for or not (#PH_OFF). */
                                 uint8_t bWaitUntilPowerUp,                 /**< [In] Whether to wait (#PH_ON) until the StandBy bit in command register is cleared or not (#PH_OFF). */
                                 uint8_t bIrq0WaitFor,                      /**< [In] Bitmask of interrupts in Irq0 register to wait for. */
                                 uint8_t bIrq1WaitFor,                      /**< [In] Bitmask of interrupts in Irq1 register to wait for. */
                                 uint8_t * pIrq0Reg,                        /**< [Out] Contents of Irq0 register. */
                                 uint8_t * pIrq1Reg                         /**< [Out] Contents of Irq1 register. */
                                 );



/**
* \brief Initialize Rc663 registers for 18000p3m3 protocol.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_Rc663_EPCV2_Init(
                                    phhalHw_Rc663_DataParams_t * pDataParams /**< [In] Pointer to this layer's parameter structure. */
                                    );


/**
* \brief Read different errors present./w
*
* \retval Error code
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_Rc663_GetErrorStatus(
                                        phhalHw_Rc663_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                        uint16_t *pwBufferLen,                     /**< [Out] Number of bytes already in the TxBuffer. */
                                        uint16_t *pwBufferSize                     /**< [Out] Size of the TxBuffer. */
                                        );
/**
* \brief Function will read Multiple Registers and return of data read from multiple registers. Specific for EMVCo
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_GetMultiReg(
                                     phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t* pData                               /**< [Out] Received data. */
                                     );

/**
* \brief Function is used to detect and Suppress EMVCo EMD Noise during reception
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_CheckForEmdError(
                                          phhalHw_Rc663_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                          uint8_t bIrq0WaitFor,                     /**< [In] Bitmask of interrupts in Irq0 register to wait for. */
                                          uint8_t bIrq1WaitFor,                     /**< [In] Bitmask of interrupts in Irq1 register to wait for. */
                                          uint32_t * pdwMultiReg,                   /**< [Out] Contains Multiple Registry Read Value. */
                                          uint32_t * pdwPrevReg                     /**< [Out] Contains previous operation Multiple Registry Read value. */
                                          );

/**
* \brief This function will  return the answer to ReqC in 32 byte multi slot frame format.
* Size of output frame is #PHHAL_HW_RESP_FRAME_SIZE byte.
*
* \b Note: This function is only valid when multiple reception #PHHAL_HW_CONFIG_RXMULTIPLE is enabled.
*/
void phhalHw_Rc663_CreateRespFrame(
                                   uint8_t* bInBuff,  /**< [In]  Pointer to Rx buffer. */
                                   uint8_t bDataPos,  /**< [In]  position of card response in Rxbuffer. */
                                   uint8_t * bOutBuff /**< [Out] Pointer to buffer which store frame of #PHHAL_HW_RESP_FRAME_SIZE size.  */
                                   );

/**
* \brief Form packet which contains a series of ReqC responses(if multiple response received) in multi-slot response format and update Hal Rx buffer.
* This function will use function \ref phhalHw_Rc663_CreateRespFrame to frame each card responses multi-slot frame format.
*
* \b Note: This function is only valid when multiple reception #PHHAL_HW_CONFIG_RXMULTIPLE is enabled.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and under-laying component.
*/
phStatus_t phhalHw_Rc663_FrameRxMultiplePkt(
                                            phhalHw_Rc663_DataParams_t * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                            );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_Exchange(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t bTxLastBits,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength,
    uint8_t * pRxLastBits
    );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_Select(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t * pSelCmd,
    uint8_t bSelCmdLen,
    uint8_t bNumValidBitsinLastByte
    );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_BeginRound(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t * pBeginRndCmd
    );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_NextSlot(
    phhalHw_Rc663_DataParams_t * pDataParams
    );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_Ack(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t *pRxLength,
    uint8_t *pRxLastBits
    );

phStatus_t phhalHw_Rc663_18000p3m3_Sw_ReqRn(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer
    );

/**
* \brief This function read the derivative from the EEPROM and Version Register(PHHAL_HW_RC663_REG_VERSION (0x7F))
* of the Front End IC attached and update pDerivate with the value.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and under-laying component.
*/
phStatus_t phhalHw_Rc663_Int_ReadDerivative(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t * pFEDerivate,
    uint8_t * pVersion
    );

/**
* \brief This function return success if the RC663 derivative supports the protocol that the user is interested
* to apply.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION If protocol is not supported by the RC663 derivative.
* \retval #PH_ERR_INVALID_PARAMETER \b bMode is invalid or not supported.
*/
phStatus_t phhalHw_Rc663_Int_ProtocolSupport(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t bMode);

void phhalHw_Rc663_WriteSSEL(
    void *pBalDataParams,
    uint8_t bValue
    );


#ifdef NXPBUILD__PHHAL_HW_RC663
#if !defined(NXPBUILD__PH_PLATFORM_DELEGATE)

#if (PIN_IRQ_TRIGGER_TYPE == PH_DRIVER_INTERRUPT_RISINGEDGE)
#       define phhalHw_Rc663_GetIrqTrigger() PH_HALHW_RC663_RAISING_EDGE_TRIGGER
#else
#   if (PIN_IRQ_TRIGGER_TYPE == PH_DRIVER_INTERRUPT_FALLINGEDGE)
#       define phhalHw_Rc663_GetIrqTrigger() PH_HALHW_RC663_FALLING_EDGE_TRIGGER
#   else
#       error "Invalid combination"
#   endif
#endif /* PIN_IRQ_TRIGGER_TYPE */
#endif /* !defined(NXPBUILD__PH_PLATFORM_DELEGATE) */

/** @}
* end of phhalHw_Rc663_Internal group
*/

#endif /* NXPBUILD__PHHAL_HW_RC663 */

#endif /* PHHALHW_RC663_INT_H */
