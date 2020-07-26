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
* Command functions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. March 2010
*
*/

#ifndef PHHALHW_RC663_CMD_H
#define PHHALHW_RC663_CMD_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHHAL_HW_RC663

/** \defgroup phhalHw_Rc663_Cmd Commands
* \brief Supported ReaderIC commands.
* @{
*/

/**
* \name Low-Power-Card-Detection Modes
*/
/*@{*/

/**
Default mode. Try LPCD until timeout is reached.
This mode is used during LPCD calibration. This mode sets standby mode and uses Timer3 to timeout
from LPCD calibration cycle. Uses Timer 3 and Timer 4 values set by the application using
\ref phhalHw_Rc663_Cmd_Lpcd_SetConfig function before calling \ref phhalHw_Rc663_Cmd_Lpcd_GetConfig function.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT            0x00U

/**
Powers down the IC for a certain amount of time and performs LPC after wakeup.
If no card is found the IC is powered down again and the procedure is restarted.
If a card is found the function returns and the IC remains powered up.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN          0x01U

/**
Same as #PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN, but uses the timeout set with
either #PHHAL_HW_CONFIG_TIMING_US or #PHHAL_HW_CONFIG_TIMING_MS as abort criteria.
Be advised that the guard-timer in this case is only running during the power-up phases,
so the timeout has to be adjusted properly.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN_GUARDED  0x02U

/**
Or this bit to the desired mode to perform LPO trimming together with the command.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_TRIMM_LPO   0x80U

/**
Or this bit to the desired mode to prevent the function to set I and Q channel values.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_IGNORE_IQ   0x40U

/**
Mask for option bits within Mode byte.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_MASK        0xF0U

/**
Mask for Mode configuration.
*/
#define PHHAL_HW_RC663_CMD_LPCD_MODE_MASK               0x0FU
/*@}*/

/**
* \brief Perform Low-Power-Card-Detection.
*
* In default-mode the \c bPowerDownTimeMs and \c bDetectionTimeUs parameters are not used.\n
* The timeout set with #PHHAL_HW_CONFIG_TIMEOUT_VALUE_US or #PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS is always used as stop-condition.
* \return Status code
* \retval #PH_ERR_SUCCESS Card present.
* \retval #PH_ERR_IO_TIMEOUT No card found.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_Lpcd(
                                  phhalHw_Rc663_DataParams_t * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                                  );

/**
* \brief Perform Low-Power-Card-Detection Configuration Procedure.
*
* Returns the I and Q channel values for the current antenna status.
* This is inteded to be used to retrieve the I and Q channel values in case of no card on antenna.
* The values can directly be used as parameters for the \ref phhalHw_Rc663_Cmd_Lpcd function.
* \return Status code
* \retval #PH_ERR_SUCCESS Trimming successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_Lpcd_GetConfig(
                                        phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                        uint8_t * pI,                               /**< [Out] I-Channel value in case of no card on antenna. */
                                        uint8_t * pQ                                /**< [Out] Q-Channel value in case of no card on antenna. */
                                        );

/**
* \brief Performs a SetConfig.
*
* Sets the bMode,I, Q, wPowerDownTimeMs, and wDetectionTimeUs channel values for the current antenna status.
* This is intended to be used to set the input parameters for LPCD.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Rc663_Cmd_Lpcd_SetConfig(
                                            phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                            uint8_t bMode,                               /**< [In] One of the above LPCD modes. */
                                            uint8_t bI,                                  /**< [In] I-Channel value in case of no card on antenna. */
                                            uint8_t bQ,                                  /**< [In] Q-Channel value in case of no card on antenna. */
                                            uint16_t wPowerDownTimeMs,                   /**< [In] Power-Down time in milliseconds if power-down mode is used. */
                                            uint16_t wDetectionTimeUs                    /**< [In] Detection time in microseconds if power-down mode is used. */
                                            );

/**
* \brief Load a MIFARE product key into the key-buffer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_LoadKey(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t * pKey                             /**< [In] 6 byte MIFARE product key. */
                                     );


/**
* \brief Performs a query, an Ack and a Req-Rn as per ISO 18000-3M3 protocol.
*
* The complete query (BeginRound) shall be provided as an input in the
* pCmdBuffer. Refer ISO/IEC FDIS 18000-3:2010(E) section 6.3.3.4.11.2.1 for
* format of BeginRound command. \ref phpalI18000p3m3_CreateBeginRoundCmd can
* also be used for creating the BeginRound command. The content of pCmdBuffer
* is not validated in this API.
*
* All answers to the command are returned back in pRxBuffer.
*
* This implements AckReq command of RC663. See \ref phhalHw_I18000p3m3Inventory
* for an alternative API designed to facilitate detection of ISO 18000-3M3 tags
* with more flexibility.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_AckReq(
                                    phhalHw_Rc663_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                    uint8_t * pCmdBuffer,                          /**< [In] Pointer to query command buffer. */
                                    uint16_t wLen,                                 /**< [In] Length of the pCmdBuffer. */
                                    uint8_t * pRxBuffer,                           /**< [Out]Pointer to receive output of CmdAckReq. */
                                    uint16_t * pwRxBufferLen                        /**< [Out]Length of the Data read. */
                                    );

/**
* \brief Write one byte of data to a given EEPROM address.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_WriteE2(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint16_t wAddress,                         /**< [In] 2 byte Address; Range is 0x0000 - 0x1FFF. */
                                     uint8_t bData                              /**< [In] Data byte to write. */
                                     );

/**
* \brief Write a number of data bytes to a given EEPROM page.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_WriteE2Page(
    phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wAddress,                          /**< [In] 2 byte Address; Range is 0x0000 - 0x007F. */
    uint8_t * pData,                            /**< [In] Pointer to data byte array. */
    uint8_t bDataLen                            /**< [In] Number of data bytes to write; Range is 1 - 64. */
    );

/**
* \brief Read one byte from a given EEPROM address.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_ReadE2(
                                    phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wAddress,                          /**< [In] 2 byte Address; Range is 0x0000 - 0x1FFF. */
                                    uint16_t wNumBytes,                         /**< [In] Number of data bytes to read; Range is 0(256) - 256. */
                                    uint8_t * pData                             /**< [Out] Requested data bytes. */
                                    );

/**
* \brief Copies a defined number of bytes from the EEPROM into the register set, beginning at the given register address RegAdr.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_LoadReg(
                                     phhalHw_Rc663_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                     uint16_t wEEAddress,                       /**< [In] 2 byte EEPROM Address; Range is 0x00C0 - 0x17FF. */
                                     uint8_t bRegAddress,                       /**< [In] Register Address; Range is 0x00 - 0xFF. */
                                     uint8_t bNumBytes                          /**< [In] Number of bytes to copy; Range is 1 - 255. */
                                     );

/**
* \brief Initializes protected registers for a dedicated protocol.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_LoadProtocol(
    phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bTxProtocol,                        /**< [In] Tx-Protocol (see PHHAL_HW_RC663_RXTX_* defines). */
    uint8_t bRxProtocol                         /**< [In] Rx-Protocol (see PHHAL_HW_RC663_RXTX_* defines). */
    );

/**
* \brief Load a MIFARE product key from EEPROM into the key-buffer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_LoadKeyE2(
                                       phhalHw_Rc663_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bKeyNo                               /**< [In] Key number in EEPROM; Range is 0x00 - 0xFF. */
                                       );

/**
* \brief Write one or more MIFARE product key(s) into EEPROM.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_StoreKeyE2(
                                        phhalHw_Rc663_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                        uint8_t bKeyNo,                             /**< [In] Key number in EEPROM; Range is 0x00 - 0xFF. */
                                        uint8_t * pKeys,                            /**< [In] Pointer to MIFARE product key array. */
                                        uint8_t bNumberOfKeys                       /**< [In] Number of keys to write. */
                                        );

/**
* \brief Performs a Soft reset.
*
* \b Note: It is up to the caller to wait until the IC is powered-up and ready again.\n
* In addition to that, the caller should call \ref phhalHw_ApplyProtocolSettings again to re-configure the IC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Rc663_Cmd_SoftReset(
                                       phhalHw_Rc663_DataParams_t * pDataParams     /**< [In] Pointer to this layer's parameter structure. */
                                       );

/**
* \brief Performs transmit of PRBS sequence.
*
* \b This function is a infinite blocking call which will transmit the bytes that is been provided by the application, continuously
* in a loop till an abort event is been received from different execution context by calling \ref phhalHw_AsyncAbort API.\n
*
*
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
* \retval #PH_ERR_INTERFACE_ERROR Hardware problem.
*/
phStatus_t phhalHw_Rc663_Cmd_PRBS(
                                  phhalHw_Rc663_DataParams_t * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t * pBufferStartPos,                        /**< [In] Pointer of the data to transmit. */
                                  uint16_t wBuffLength                              /**< [In] Length of data. */
                                  );

/** @} */
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHHALHW_RC663_CMD_H */
