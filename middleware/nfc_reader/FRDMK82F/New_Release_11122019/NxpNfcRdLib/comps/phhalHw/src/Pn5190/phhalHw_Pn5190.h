/*
*                     Copyright 2019, NXP
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
* PN5190 generic HAL interface.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5190_H
#define PHHALHW_PN5190_H

#include <ph_Status.h>

#ifdef NXPBUILD__PHHAL_HW_PN5190
#include <phhalHw.h>


#define PHHAL_HW_Pn5190_TR_RX_PRESCALAR                 0x7FU  /* Tx and Rx Prescaler to 0x7F with 1 Etu ref*/

/**
Set EMD Modes
*/
#define PHHAL_HW_PN5190_EMVCO_EMD_NO_BYTES (1 << EMD_CONTROL_EMD_NOISE_BYTES_THRESHOLD_POS)
#define PHHAL_HW_PN5190_EMVCO_EMD_TIMER    (1 << EMD_CONTROL_EMD_TRANSMISSION_TIMER_USED_POS)

#define PHHAL_HW_PN5190_EMD_3P0     (EMD_CONTROL_EMD_ENABLE_MASK | \
                                     EMD_CONTROL_EMD_TRANSMISSION_ERROR_ABOVE_NOISE_THRESHOLD_IS_NO_EMD_MASK | \
                                     PHHAL_HW_PN5190_EMVCO_EMD_NO_BYTES | PHHAL_HW_PN5190_EMVCO_EMD_TIMER)   /* EMVCO 3.0 */


#define ACTIVE_MODE_HAL_PN5180              0xFF00U
#define A_MODE                              0x01U
#define B_MODE                              0x02U
#define F_MODE                              0x04U

#define ACTIVE_A_MODE                       0x0100U
#define ACTIVE_B_MODE                       0x0200U
#define ACTIVE_F_MODE                       0x0400U
#define UNSUPPORTED_MODES                   0xF8F8U

#define PHHAL_HW_PN5190_BIT_TYPEB                 0x03U   /**< Activates Type B communication mode. */
#define PHHAL_HW_PN5190_BIT_FELICA                0x02U   /**< Activates FeliCa communication mode. */
#define PHHAL_HW_PN5190_BIT_ISO18092              0x01U   /**< Activates ISO18092(with Sync Byte = 0xF0) communication mode. */
#define PHHAL_HW_PN5190_BIT_MIFARE                0x00U   /**< Activates ISO/IEC 14443 type A communication mode. */
#define PHHAL_HW_PN5190_I18092MT_START_BYTE       0xF0U   /**< Start Byte to be added to the protocol header in case protocol ISO18092. */
#define PHHAL_HW_PN5190_SIGPRO_GENERAL_MAPPING    0x03U

#define PH_SIGPRO_FRAMING_NFC               0x01U
#define PH_NFC_SYNC_BYTE_DEF                0x1FU
#define PH_NINE_BIT_LEN_MASK                0x09U
#define PH_CRC_PRESET_SEL_NFC               0x02U


/**
Set Listen Parameter Constants
*/
#define PHHAL_HW_PN5190_SET_LISTEN_LEN              24U
#define PHHAL_HW_PN5190_SET_LISTEN_SENRES_LEN       2U
#define PHHAL_HW_PN5190_SET_LISTEN_NFCID1_LEN       3U
#define PHHAL_HW_PN5190_SET_LISTEN_SELRES_LEN       1U
#define PHHAL_HW_PN5190_SET_LISTEN_POLLRES_LEN      18U
#define PHHAL_HW_PN5190_SET_LISTEN_NFCID3_LEN       1U

/*
 * If 1-> Enabled SW SYNC byte
 * If 0-> Enabled HW SYNC byte
 * */
#define PHHAL_HW_PN5190_USE_SW_SYNC_BYTE             1

/**
* \brief PN5190 implementation of write register
* This function writes given 32-bit value to the register address.
*/
phStatus_t phhalHw_Pn5190_WriteRegister(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t  bAddress,                                /**< [In] Address to which the data has to be written. */
    uint32_t dwValue                                  /**< [In] The 32-bit value that has to be written to the register at address \a bAddress. */
    );

/**
* \brief Command to Read the contents of a given register.
*/
phStatus_t phhalHw_Pn5190_ReadRegister(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bAddress,                                 /**< [In] Address from where the data is to be read. */
    uint32_t * pValue                                 /**< [Out] the read value. */
    );

/**
* \brief PN5190 implementation of Exchange.
*
* \sa phhalHw_Exchange
*/
phStatus_t phhalHw_Pn5190_Exchange(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                                 /**< [In] Option parameter. */
    uint8_t * pTxBuffer,                              /**< [In] Data to transmit. */
    uint16_t wTxLength,                               /**< [In] Number of bytes to transmit. */
    uint8_t ** ppRxBuffer,                            /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                              /**< [Out] Number of received data bytes. */
    );

/**
* \brief PN5190 implementation of SetConfig.
*
* \sa phhalHw_SetConfig
*/
phStatus_t phhalHw_Pn5190_SetConfig(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,                                 /**< [In] Configuration Identifier. */
    uint16_t wValue                                   /**< [In] Configuration Value. */
    );

/**
* \brief PN5190 implementation of GetConfig.
*
* \sa phhalHw_GetConfig
*/
phStatus_t phhalHw_Pn5190_GetConfig(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,                                 /**< [In] Configuration Identifier. */
    uint16_t * pValue                                 /**< [Out] Configuration Value. */
    );

/**
* \brief PN5190 implementation of MfcAuthenticateKeyNo.
*
* \sa phhalHw_MfcAuthenticateKeyNo
*/
phStatus_t phhalHw_Pn5190_MfcAuthenticateKeyNo(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bBlockNo,                                 /**< [In] Block-Number on card to authenticate to. */
    uint8_t bKeyType,                                 /**< [In] Either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
    uint16_t wKeyNo,                                  /**< [In] Key number to be used in authentication. */
    uint16_t wKeyVersion,                             /**< [In] Key version to be used in authentication. */
    uint8_t * pUid                                    /**< [In] Serial number of current cascade level; uint8_t[4]. */
    );

/**
* \brief PN5190 implementation of MfcAuthenticate.
*
* \sa phhalHw_MfcAuthenticate
*/
phStatus_t phhalHw_Pn5190_MfcAuthenticate(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bBlockNo,                                 /**< [In] Block number. */
    uint8_t bKeyType,                                 /**< [In] Key type to be used KEY_A or KEY_B. */
    uint8_t * pKey,                                   /**< [In] Key to be used for authentication. */
    uint8_t * pUid                                    /**< [In] UID of the tag to be authenticated. */
    );

/**
* \brief PN5190 implementation of ApplyProtocolSettings.
*
* \sa phhalHw_ApplyProtocolSettings
*/
phStatus_t phhalHw_Pn5190_ApplyProtocolSettings(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bCardType                                 /**< [In] Type of card for which the HAL should be configured for. */
    );

/**
* \brief PN5190 implementation of FieldOn.
*
* \sa phhalHw_FieldOn
*/
phStatus_t phhalHw_Pn5190_FieldOn(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of FieldOff.
*
* \sa phhalHw_FieldOff
*/
phStatus_t phhalHw_Pn5190_FieldOff(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of FieldReset.
*
* \sa phhalHw_FieldReset
*/
phStatus_t phhalHw_Pn5190_FieldReset(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of Wait.
*
* \sa phhalHw_Wait
*/
phStatus_t phhalHw_Pn5190_Wait(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bUnit,                                    /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
    uint16_t wTimeout                                 /**< [In] Timeout value. */
    );

/**
* \brief PN5190 implementation of Listen.
*
* \sa phhalHw_Listen
*/
phStatus_t phhalHw_Pn5190_Listen(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                            /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                              /**< [Out] Number of received data bytes. */
    );

/**
* \brief PN5190 implementation of Transmit.
*
* \sa phhalHw_Transmit
*/
phStatus_t phhalHw_Pn5190_Transmit(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                                 /**< [In] Option parameter. */
    uint8_t * pTxBuffer,                              /**< [In] Data to transmit. */
    uint16_t wTxLength                                /**< [In] Number of bytes to transmit. */
    );

/**
* \brief PN5190 implementation of Receive.
*
* \sa phhalHw_Receive
*/
phStatus_t phhalHw_Pn5190_Receive(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                                 /**< [In] Option parameter. */
    uint8_t ** ppRxBuffer,                            /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                              /**< [Out] Number of received data bytes. */
    );

/**
* \brief PN5190 implementation of Autocoll.
*
* \sa phhalHw_Autocoll
*/
phStatus_t phhalHw_Pn5190_Autocoll(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wMode,                                   /**< [In] Autocoll Mode. */
    uint8_t ** ppRxBuffer,                            /**< [Out] Pointer to received data. */
    uint16_t * pRxLength,                             /**< [Out] Number of received data bytes. */
    uint16_t * pProtParams
    );


#ifdef _WIN32
/**
* \brief API used by external applications to retrieve the
* size of #phhalHw_Pn5190_DataParams_t
*/
phStatus_t phhalHw_Pn5190_GetDataParamsSize(
    uint16_t * pDataParamsSize                        /**< [Out] Size of #phhalHw_Pn5190_DataParams_t. */
    );
#endif
/**
* \brief PN5190 implementation of SetMinFDT.
*
* \sa phhalHw_SetMinFDT
*/
phStatus_t phhalHw_Pn5190_SetMinFDT(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wValue                                   /**< [In] Frame Delay Time Value. */
    );

/**
* \brief PN5190 implementation LPCD.
*
* \sa phhalHw_Lpcd
*/
phStatus_t phhalHw_Pn5190_Lpcd(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of abort command.
*
* \sa phhalHw_AsyncAbort
*/
phStatus_t phhalHw_Pn5190_AsyncAbort(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of I18000p3m3Inventory command.
*
* \sa phhalHw_I18000p3m3Inventory
*/
phStatus_t phhalHw_Pn5190_I18000p3m3Inventory(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pSelCmd,                                /**< [In] ISO 18000p3m3 Select command frame. */
    uint8_t bSelCmdLen,                               /**< [In] Select command length in bytes- 1 to 39 bytes. */
    uint8_t bNumValidBitsinLastByte,                  /**< [In] Number of valid bits in last byte of pSelCmd. */
    uint8_t * pBeginRndCmd,                           /**< [In] ISO 18000p3m3 BeginRound command frame. This is 17bits i.e., 3 bytes are expected. CRC5 should not be provided. */
    uint8_t bTSprocessing,                            /**< [In] TimeSlot processing behavior. */
    uint8_t ** ppRxBuffer,                            /**< [In] Pointer to HAL RX Buffer containing response from single/multiple tags. */
    uint16_t * wRxBufferLen                           /**< [In] Length of response in HAL RX Buffer. */
    );

/**
* \brief PN5190 implementation of I18000p3m3ResumeInventory command.
*
* \sa phhalHw_I18000p3m3ResumeInventory
*/
phStatus_t phhalHw_Pn5190_18000p3m3ResumeInventory(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                            /**< [In] Pointer to HAL RX Buffer containing response from single/multiple tags. */
    uint16_t * wRxBufferLen                           /**< [In] Length of response in HAL RX Buffer. */
    );

/**
* \brief PN5190 implementation of EventWait command.
*
* \sa phhalHw_EventWait
*/
phStatus_t phhalHw_Pn5190_EventWait(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint32_t dwEventTimeout                           /**< [In] Event time out. */
    );

/**
* \brief PN5190 implementation of EventConsume command.
*
* \sa phhalHw_EventConsume
*/
phStatus_t phhalHw_Pn5190_EventConsume(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief PN5190 implementation of DeInit command.
*
* \sa phhalHw_DeInit
*/
phStatus_t phhalHw_Pn5190_DeInit(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

#endif /*NXPBUILD__PHHAL_HW_PN5190*/
#endif  /* PHHALHW_PN5190_H */
