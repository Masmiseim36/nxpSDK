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
* PN5180 generic HAL interface.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5180_H
#define PHHALHW_PN5180_H


#include <ph_Status.h>
#ifdef NXPBUILD__PHHAL_HW_PN5180
#include <phhalHw.h>

/*#define PHHAL_HW_PN5180_FIFOSIZE         255U             < Size of PN517 internal FiFo buffer */
#define PHHAL_HW_ERR_EMD_ERROR              0x0011U      /**< An EMD error (noise received). */
#define ACTIVE_MODE_HAL_PN5180              0xFF00U
#define A_MODE                              0x01U
#define B_MODE                              0x02U
#define F_MODE                              0x04U

#define ACTIVE_A_MODE                       0x0100U
#define ACTIVE_B_MODE                       0x0200U
#define ACTIVE_F_MODE                       0x0400U
#define UNSUPPORTED_MODES                   0xF8F8U

#define PHHAL_HW_PN5180_BIT_TYPEB                 0x03U   /**< Activates Type B communication mode. */
#define PHHAL_HW_PN5180_BIT_FELICA                0x02U   /**< Activates FeliCa communication mode. */
#define PHHAL_HW_PN5180_BIT_ISO18092              0x01U   /**< Activates ISO18092(with Sync Byte = 0xF0) communication mode. */
#define PHHAL_HW_PN5180_BIT_MIFARE                0x00U   /**< Activates ISO/IEC 14443 type A communication mode. */
#define PHHAL_HW_PN5180_I18092MT_START_BYTE       0xF0U   /**< Start Byte to be added to the protocol header in case protocol ISO18092. */
#define PHHAL_HW_PN5180_SIGPRO_GENERAL_MAPPING    0x04U
#define PHHAL_HW_PN5180_EXCHANGE_HEADER_SIZE      0x02U    /**< Size Tx exchange header.*/
#define PHHAL_HW_PN5180_ACTIVE_ERR_CAUSE4         0x44U


/**
Tranceive States
*/
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_IDLE                 0U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_WAITTRANSMIT         1U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_TRANSMITTING         2U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_WAITRECEIVE          3U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_WAITFORDATA          4U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_RECEIVING            5U
#define PHHAL_HW_PN5180_TRANCEIVE_STATE_LOOPBACK             6U

/**
System Commands-  hold the command bit
*/
#define PHHAL_HW_PN5180_SYSTEM_IDLE_CMD                 0x00U   /*stops all ongoing communication and set the CLIF to IDLE mode*/
#define PHHAL_HW_PN5180_SYSTEM_TRANSMIT_CMD             0x01U  /*starts a transmission immediately*/
#define PHHAL_HW_PN5180_SYSTEM_RECEIVE_CMD              0x02U  /*enables the receiver. After end of reception the bits are clear and IDLE*/
#define PHHAL_HW_PN5180_SYSTEM_TRANSEIVE_CMD            0x03U  /*Transceive command; initiates a transceive cycle. Note: Depending on the value of the Initiator bit a transmission is started or the receiver is enabled Note: The transceive command does not finish automatically. It stays in the transceive cycle until stopped via the IDLE/StopCom command */
#define PHHAL_HW_PN5180_SYSTEM_KEEP_CMD                 0x04U  /*This command does not change the content of the command register and might be used in case other bits in the register are to be changed*/
#define PHHAL_HW_PN5180_SYSTEM_LOOPBACK_CMD             0x05U  /*This command is for test purposes only. It starts a transmission and at the same enables the receiver*/


#define PHHAL_HW_PN5180_TR_RX_PRESCALAR                 0x7FU  /* Tx and Rx Prescaler to 0x7F with 1 Etu ref*/
/**
Set Listen Parameter Constants
*/
#define PHHAL_HW_PN5180_SET_LISTEN_LEN              24U
#define PHHAL_HW_PN5180_SET_LISTEN_EEPROM_ADDR      0x40U
#define PHHAL_HW_PN5180_SET_LISTEN_SENRES_LEN       2U
#define PHHAL_HW_PN5180_SET_LISTEN_NFCID1_LEN       3U
#define PHHAL_HW_PN5180_SET_LISTEN_SELRES_LEN       1U
#define PHHAL_HW_PN5180_SET_LISTEN_POLLRES_LEN      18U
#define PHHAL_HW_PN5180_SET_LISTEN_NFCID3_LEN       1U

#define PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_ADDR   0xE8U
#define PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_LEN    0x01U
#define PHHAL_HW_PN5180_DIGITAL_DELAY_ENABLE        0x01U

#define PHHAL_HW_PN5180_FDT_TIMER_USED              0x01U     /* As Timer 1 is used for FWT/FDT in HAL 5180, configure timer 1 for digital delay calculation in FW. */
#define PHHAL_HW_PN5180_FDT_TIMER_USED_MASK         0x06U

#define PHHAL_HW_PN5180_DELAY_TO_CHECK_TESTBUS      2U
#define PHHAL_HW_PN5180_RESET_DELAY_MILLI_SECS      2U

/**
Set EMD Modes
*/
#define PHHAL_HW_PN5180_NO_EMD           0U       /* No EMD */
#define PHHAL_HW_PN5180_EMD_2P3          0x185U   /* EMVCO 2.3.1 EMD */
#define PHHAL_HW_PN5180_EMVCO_NFC_EMD    0x107U   /* EMVCO 2.5 or 2.6 or 3.0 or NFC Mode EMD */
#define PHHAL_HW_PN5180_ISO_EMD          0x103U   /* ISO Mode EMD */

/**
FIRMWARE_VERSION address in E2PROM
*/
#define PHHAL_HW_PN5180_FIRMWARE_VERSION_ADDR         0x12U

/**
TESTBUS_MODE address in E2PROM
*/
#define PHHAL_HW_PN5180_TESTBUS_ENABLE_ADDR           0x17U

/**
LPCD_REFERENCE_VALUE address in E2PROM
*/
#define PHHAL_HW_PN5180_LPCD_REFERENCE_VALUE_ADDR     0x34U

/**
OCPROT_CONTROL address in E2PROM
*/
#define PHHAL_HW_PN5180_OCPROT_CONTROL_ADDR           0x59U

/**
LPCD_GPO_REFVAL_CONTROL address in E2PROM
*/
#define PHHAL_HW_PN5180_LPCD_GPO_REFVAL_CONTROL_ADDR  0x38U

/**
LPCD_REFERENCE_VALUE Mask to get the mode
*/
#define     LPCD_REFVAL_CONTROL_MASK                  0x03U

/**
Clear all INTERUPT MASK
*/
#define PHHAL_HW_PN5180_IRQ_SET_CLEAR_ALL_MASK        0xFFFFFU

/**
Prescalar for Millisec
*/
#define PHHAL_HW_PN5180_MS_TIMEOUT_PRESCALAR          0x3CU

/** \name Modulation indices
*/
/*@{*/
#define PHHAL_HW_PN5180_MODINDEX_I14443B     0xCCU   /**< Default modulation index for ISO14443B. */
#define PHHAL_HW_PN5180_MODINDEX_FELICA      0x17U   /**< Default modulation index for FeliCa. */
#define PHHAL_HW_PN5180_MODINDEX_I15693      0x10U   /**< Default modulation index for ISO15693.  */
#define PHHAL_HW_PN5180_MODINDEX_EPCUID      0x10U   /**< Default modulation index for EPC/UID. */
#define PHHAL_HW_PN5180_MODINDEX_I18000P3M3  0x10U   /**< Default modulation index for ISO18000-3 Mode3. */
/*@}*/

/**
* \brief PN5180 implementation of write register
* This function writes given 32-bit value to the given address.
* This function makes use of the SetRegister command and Pn5180 expects the
* command and the data in the following format.
*
*  +-------+-------+-------+-------+-------+-------+
*  |       |       |       |       |       |       |
*  | 0x00  |  Addr | LSB1  | LSB2  | MSB1  | MSB2  |
*  +-------+-------+-------+-------+-------+-------+
*     0        1       2      3       4       5
*
*  In response to this command, we are going to receive a three byte response
*
*   +-------+
*   |       |
*   | Status|
*   +-------+
*      0        1       2
*
*   status byte indicates the error condition or absence of it.
*
*/

phStatus_t phhalHw_Pn5180_WriteRegister(
    phhalHw_Pn5180_DataParams_t * pDataParams,         /**<[In] DataParams representing this layer. */
    uint8_t  bAddress,                                /**<[In] Address to which the dat has to be written. */
    uint32_t dwValue                                  /**<[In] The 32-bit value that has to be written to the register at address \a bAddress. */
    );



/**
* \brief PN5180 implementation of Reads the contents of a given register.
*/

phStatus_t phhalHw_Pn5180_ReadRegister(
    phhalHw_Pn5180_DataParams_t * pDataParams,          /**<[In] DataParams representing this layer. */
    uint8_t bAddress,                                   /**<{In] Address from where the data is to be read. */
    uint32_t * pValue                                   /**<[Out] the read value. */
    );

/**
* \brief Perform data exchange on the bus.
* This API first performs BUSY check to exchange command between Pn5180 FE and HOST.
* If response is expected out of this exchange then BUSY check is once again performed and
* BAL exchange is once again performed to receive response from Pn5180 FE.
* SPI : This API will also perform required flow control for particular front-end.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
* \retval #PH_ERR_IO_TIMEOUT No response received within given time frame.
* \retval #PH_ERR_BUFFER_OVERFLOW Response is too big for either given receive buffer or internal buffer.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_Pn5180_BalExchange(
                             phhalHw_Pn5180_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t * pTxBuffer,                          /**< [In] Data to transmit. */
                             uint16_t wTxLength,                           /**< [In] Number of bytes to transmit. */
                             uint16_t wRxBufSize,                          /**< [In] Size of receive buffer / Number of bytes to receive (depending on implementation). */
                             uint8_t * pRxBuffer,                          /**< [Out] Received data. */
                             uint16_t * pRxLength                          /**< [Out] Number of received data bytes. */
                             );


/**
* \brief PN5180 implementation of phhalHw_Exchange
*
* \sa phhalHw_Exchange
*/
phStatus_t phhalHw_Pn5180_Exchange(
    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams representing this layer. */
    uint16_t wOption,       /**< [In] Option parameter. */
    uint8_t * pTxBuffer,    /**< [In] Data to transmit. */
    uint16_t wTxLength,     /**< [In] Number of bytes to transmit. */
    uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data. */
    uint16_t * pRxLength    /**< [Out] Number of received data bytes. */
    );


/**
* \brief PN5180 implementation of phhalHw_SetConfig
*
* \sa phhalHw_SetConfig
*/
phStatus_t phhalHw_Pn5180_SetConfig(
    phhalHw_Pn5180_DataParams_t * pDataParams,             /**<[In] DataParams representing this layer. */
    uint16_t wConfig,                                     /**< [In] Configuration Identifier. */
    uint16_t wValue                                       /**< [In] Configuration Value. */
    );

/**
* \brief PN5180 implementation of phhalHw_GetConfig
*
* \sa phhalHw_GetConfig
*/
phStatus_t phhalHw_Pn5180_GetConfig(
    phhalHw_Pn5180_DataParams_t * pDataParams,              /**<[In] DataParams representing this layer.  */
    uint16_t wConfig,                                      /**< [In] Configuration Identifier. */
    uint16_t * pValue                                      /**< [Out] Configuration Value. */
    );

/**
* \brief PN5180 implementation of phhalHw_Pn5180_MfcAuthenticateKeyNo
*
* \sa phhalHw_Pn5180_MfcAuthenticateKeyNo
*/
phStatus_t phhalHw_Pn5180_MfcAuthenticateKeyNo(
    phhalHw_Pn5180_DataParams_t * pDataParams,      /**<[In] DataParams representing this layer.  */
    uint8_t bBlockNo,                               /**< [In] Block-Number on card to authenticate to. */
    uint8_t bKeyType,                               /**< [In] Either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
    uint16_t wKeyNo,                                /**< [In] Key number to be used in authentication. */
    uint16_t wKeyVersion,                           /**< [In] Key version to be used in authentication. */
    uint8_t * pUid                                  /**< [In] Serial number of current cascade level; uint8_t[4]. */
    );

/**
* \brief PN5180 implementation of phhalHw_MfcAuthenticate
*
* \sa phhalHw_MfcAuthenticate
*/
phStatus_t phhalHw_Pn5180_MfcAuthenticate(
    phhalHw_Pn5180_DataParams_t * pDataParams,/**<[In] Pointer to this layer's parameter structure. */
    uint8_t bBlockNo,                        /**<[In] Block number. */
    uint8_t bKeyType,                        /**<[In] Key type to be used KEY_A or KEY_B. */
    uint8_t * pKey,                          /**<[In] Key to be used for auth. */
    uint8_t * pUid                           /**<[In] Uid of the tag to be authenticated. */
    );

/**
* \brief PN5180 implementation of phhalHw_ApplyProtocolSettings
*
* \sa phhalHw_ApplyProtocolSettings
*/
phStatus_t phhalHw_Pn5180_ApplyProtocolSettings(
    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams representing this layer. */
    uint8_t bCardType                          /**< [In] Type of card for which the HAL should be configured for. */
    );

/**
* \brief PN5180 implementation of phhalHw_FieldOn
*
* \sa phhalHw_FieldOn
*/
phStatus_t phhalHw_Pn5180_FieldOn(
    phhalHw_Pn5180_DataParams_t * pDataParams               /**<[In] DataParams representing this layer. */
    );

/**
* \brief PN5180 implementation of phhalHw_FieldOff
*
* \sa phhalHw_FieldOff
*/
phStatus_t phhalHw_Pn5180_FieldOff(
    phhalHw_Pn5180_DataParams_t * pDataParams             /**<[In] DataParams representing this layer. */
    );

/**
* \brief PN5180 implementation of phhalHw_FieldReset
*
* \sa phhalHw_FieldReset
*/
phStatus_t phhalHw_Pn5180_FieldReset(
    phhalHw_Pn5180_DataParams_t * pDataParams           /**<[In] DataParams representing this layer. */
    );

/**
* \brief PN5180 implementation of phhalHw_Wait
*
* \sa phhalHw_Wait
*/
phStatus_t phhalHw_Pn5180_Wait(
    phhalHw_Pn5180_DataParams_t * pDataParams,                /**<[In] DataParams representing this layer. */
    uint8_t bUnit,                                            /**< [In] Unit of given timeout value (either #PHHAL_HW_TIME_MICROSECONDS or #PHHAL_HW_TIME_MILLISECONDS). */
    uint16_t wTimeout                                         /**< [In] Timeout value. */
    );

/**
* \brief PN5180 implementation of phhalHw_Listen
*
* \sa phhalHw_Listen
*
*/
phStatus_t phhalHw_Pn5180_Listen(
    phhalHw_Pn5180_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                     /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                       /**< [Out] Number of received data bytes. */
    );

/**
* \brief PN5180 implementation of phhalHw_Transmit
*
* \sa phhalHw_Transmit
*/
phStatus_t phhalHw_Pn5180_Transmit(
    phhalHw_Pn5180_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                            /**< [In] Option parameter. */
    uint8_t * pTxBuffer,                         /**< [In] Data to transmit. */
    uint16_t wTxLength                           /**< [In] Number of bytes to transmit. */
    );

/**
* \brief PN5180 implementation of phhalHw_Receive
*
* \sa phhalHw_Receive
*/
phStatus_t phhalHw_Pn5180_Receive(
    phhalHw_Pn5180_DataParams_t * pDataParams,     /**< [In]  Pointer to this layer's parameter structure. */
    uint16_t wOption,                              /**< [In]  Option parameter. */
    uint8_t ** ppRxBuffer,                         /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                           /**< [Out] Number of received data bytes. */
    );


/**
* Used to configure the IC to handle FeliCa polling or Type A anti-collision in Target/Card Mode.
* Parameter wMode option is irrelevant in case of Pn5180 as only NFC Target mode is supported and cannot be
* individually configured to either Type A or Type F target.
*
* Note: Ensure that the correct configuration has been loaded using the phhalHw_Pn5180_SetListenParameters command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_IO_TIMEOUT No response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Autocoll(
    phhalHw_Pn5180_DataParams_t * pDataParams,    /**< [In]  Pointer to this layer's parameter structure. */
    uint16_t wMode,                               /**< [In]  Autocall Mode. */
    uint8_t ** ppRxBuffer,                        /**< [Out] Pointer to received data. */
    uint16_t * pRxLength,                         /**< [Out] Number of received data bytes. */
    uint16_t * pProtParams
    );

/**
* \brief API used by external applications to retrieve the size of #phhalHw_Pn5180_DataParams_t
*
*/
phStatus_t phhalHw_Pn5180_GetDataParamsSize(
    uint16_t * pDataParamsSize /**< [Out]  Size of #phhalHw_Pn5180_DataParams_t. */
    );


/**
* \brief PN5180 implementation of setting of FDT
*
* \sa phhalHw_Pn5180_SetMinFDT
*
*/
phStatus_t phhalHw_Pn5180_SetMinFDT(
    phhalHw_Pn5180_DataParams_t * pDataParams,     /**< [In]  Pointer to this layer's parameter structure. */
    uint16_t wValue                                /**< [In]  Frame Dalay Time Value. */
    );

/**
* \brief PN5180 implementation Lpcd
*
* \sa phhalHw_Pn5180_Lpcd
*
*/
phStatus_t phhalHw_Pn5180_Lpcd(
    phhalHw_Pn5180_DataParams_t * pDataParams);


/**
* \brief PN5180 implementation of shutdown command
*
* \sa phhalHw_Pn5180_AsyncAbort
*
*/
phStatus_t phhalHw_Pn5180_AsyncAbort(
    phhalHw_Pn5180_DataParams_t * pDataParams
    );

/**
* \brief PN5180 implementation of I18000p3m3Inventory command
*
* \sa phhalHw_Pn5180_I18000p3m3Inventory
*
*/
phStatus_t phhalHw_Pn5180_I18000p3m3Inventory(phhalHw_Pn5180_DataParams_t * pDataParams,
    uint8_t * pSelCmd,                  /**< [In] ISO 18000p3m3 Select command frame. */
    uint8_t bSelCmdLen,                 /**< [In] Select command length in bytes- 1 to 39 bytes. */
    uint8_t bNumValidBitsinLastByte,    /**< [In] Number of valid bits in last byte of pSelCmd. */
    uint8_t * pBeginRndCmd,             /**< [In] ISO 18000p3m3 BeginRound command frame. This is 17bits i.e., 3 bytes are expected. CRC5 should not be provided. */
    uint8_t bTSprocessing,              /**< [In] TimeSlot processing behavior. */
    uint8_t ** ppRxBuffer,              /**< [In] Pointer to HAL Rx Buffer containing response from single/multiple tags. */
    uint16_t * wRxBufferLen             /**< [In] Length of response in hal Rx Buffer. */
    );

/**
* \brief PN5180 implementation of I18000p3m3ResumeInventory command
*
* \sa phhalHw_Pn5180_I18000p3m3ResumeInventory
*
*/
phStatus_t phhalHw_Pn5180_18000p3m3ResumeInventory(
    phhalHw_Pn5180_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                      /**< [In] Pointer to HAL Rx Buffer containing response from single/multiple tags. */
    uint16_t * wRxBufferLen                     /**< [In] Length of response in hal Rx Buffer. */
    );

phStatus_t phhalHw_Pn5180_EventWait(
    phhalHw_Pn5180_DataParams_t * pDataParams,
    uint32_t dwEventTimeout
    );

phStatus_t phhalHw_Pn5180_EventConsume(
    phhalHw_Pn5180_DataParams_t * pDataParams
    );

/**
* \brief PN5180 implementation of DeInit functionality command
*
* \sa phhalHw_Pn5180_DeInit
*
*/
phStatus_t phhalHw_Pn5180_DeInit(
    phhalHw_Pn5180_DataParams_t * pDataParams
    );


#endif /*NXPBUILD__PHHAL_HW_PN5180*/
#endif  /* PHHALHW_PN5180_H */
