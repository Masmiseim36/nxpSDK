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
* PN5190 HAL Instruction layer header file.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5190_INSTR_H
#define PHHALHW_PN5190_INSTR_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NXPBUILD__PHHAL_HW_PN5190

/** \defgroup phhalHw_PN5190_Instr Instruction
* \brief Pn5190 Instruction functions
* @{
*/

/**
* \name PN5190 Instruction Codes.
*
*/
/*@{*/
#define PHHAL_HW_PN5190_INSTR_WRITE_REGISTER                                0x00U  /**< Write 32 bit value to a register. */
#define PHHAL_HW_PN5190_INSTR_WRITE_REGISTER_OR_MASK                        0x01U  /**< Perform a logical OR operation on a register using provided mask. */
#define PHHAL_HW_PN5190_INSTR_WRITE_REGISTER_AND_MASK                       0x02U  /**< Perform a logical AND operation on a register using provided mask. */
#define PHHAL_HW_PN5190_INSTR_WRITE_REGISTER_MULTIPLE                       0x03U  /**< Perform a write register operation using a set of Register-Value pairs. */
#define PHHAL_HW_PN5190_INSTR_READ_REGISTER                                 0x04U  /**< Read back content of a register. */
#define PHHAL_HW_PN5190_INSTR_READ_REGISTER_MULTIPLE                        0x05U  /**< Perform a read register operation on a set of registers. */
#define PHHAL_HW_PN5190_INSTR_WRITE_E2PROM                                  0x06U  /**< Write given values sequentially to E2PROM. */
#define PHHAL_HW_PN5190_INSTR_READ_E2PROM                                   0x07U  /**< Read out values from E2PROM sequentially. */
#define PHHAL_HW_PN5190_INSTR_TRANSMIT_RF_DATA                              0x08U  /**< Write TX data to internal CLIF transmission buffer. */
#define PHHAL_HW_PN5190_INSTR_RETRIEVE_RF_DATA                              0x09U  /**< Read RX data from internal RF reception buffer. */
#define PHHAL_HW_PN5190_INSTR_EXCHANGE_RF_DATA                              0x0AU  /**< Read RX data from internal RF reception buffer. */
#define PHHAL_HW_PN5190_INSTR_MFC_AUTHENTICATE                              0x0BU  /**< Perform authentication on an activated Mifare Classic card. */
#define PHHAL_HW_PN5190_INSTR_EPC_GEN2_INVENTORY                            0x0CU  /**< Perform an ISO 18000-3M3 Inventory. */
#define PHHAL_HW_PN5190_INSTR_LOAD_RF_CONFIGURATION                         0x0DU  /**< Load RF Tx and Rx settings from E2PROM. */
#define PHHAL_HW_PN5190_INSTR_UPDATE_RF_CONFIGURATION                       0x0EU  /**< Update the RF configuration. */
#define PHHAL_HW_PN5190_INSTR_GET_RF_CONFIGURATION                          0x0FU  /**< Retrieve the RF configuration. */
#define PHHAL_HW_PN5190_INSTR_FIELD_ON                                      0x10U  /**< RF Field ON. */
#define PHHAL_HW_PN5190_INSTR_FIELD_OFF                                     0x11U  /**< RF field OFF.*/
#define PHHAL_HW_PN5190_INSTR_CONFIGURE_TESTBUS_DIGITAL                     0x12U  /**< Configure digital test bus. */
#define PHHAL_HW_PN5190_INSTR_CONFIGURE_TESTBUS_ANALOG                      0x13U  /**< Configure Analog test bus. */
#define PHHAL_HW_PN5190_INSTR_CTS_ENABLE                                    0x14U  /**< Enable/disable the CTS logging feature. */
#define PHHAL_HW_PN5190_INSTR_CTS_CONFIGURE                                 0x15U  /**< Configure the all the required CTS registers. */
#define PHHAL_HW_PN5190_INSTR_CTS_RETRIEVE_LOG                              0x16U  /**< Retrieves the log of the captured data samples stored in the memory buffer. */

#define PHHAL_HW_PN5190_INSTR_RECEIVE_RF_DATA                               0x1AU  /**< Perform RF RX operation. */

#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_NORMAL                            0x20U  /**< Switch operation mode. Possible modes are: Normal */
#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_AUTOCOLL                          0x21U  /**< Switch operation mode. - Autocoll. */
#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_STANDBY                           0x22U  /**< Switch operation mode. - Standby. */
#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_LPCD                              0x23U  /**< Switch operation mode. - LPCD. */
#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_SUSPEND                           0x24U  /**< Switch operation mode. -SUSPEND. */
#define PHHAL_HW_PN5190_INSTR_SWITCH_MODE_DOWNLOAD                          0x25U  /**< Switch operation mode. - DOWNLOAD. */
#define PHHAL_HW_PN5190_INSTR_GET_DIE_ID                                    0x26U  /**< Read DIE-ID of Front End. */
#define PHHAL_HW_PN5190_INSTR_GET_VERSION                                   0x27U  /**< Read Version of Front End. */

#define PHHAL_HW_PN5190_INSTR_ANTENNA_SELF_TEST                             0x40U  /**< Check if Antenna is connected and components are populated. */
#define PHHAL_HW_PN5190_INSTR_PRBS_TEST                                     0x41U  /**< Command to generate Pseudo-random Binary Sequence. */
#define PHHAL_HW_PN5190_INSTR_MULTIPLE_PKTS                                 0x50U  /**< Special TLV type, denoting concatenation of multiple TLVs. */
/*@}*/

/**
* \name PN5190 Register and EEPROM Variable Addresses.
*
*/
/*@{*/
#define PHHAL_HW_PN5190_REG_START_ADDRESS           0U
#define PHHAL_HW_PN5190_REG_END_ADDRESS             0x4FU

#define PHHAL_HW_PN5190_EEPROM_START_ADDRS          0U         /** First writable adderss in eeprom */
#define PHHAL_HW_PN5190_EEPROM_END_ADDRS            5120U      /** Last user writable adderss in eeprom */

#define PHHAL_HW_PN5190_SET_LISTEN_EEPROM_ADDR      0x771U
#define PHHAL_HW_PN5190_DYNAMIC_UID_CONFG_ADDR      0x789U
#define PHHAL_HW_PN5190_LPCD_CONFIG_ADDR            0xAC8U
#define PHHAL_HW_PN5190_LPCD_RSSI_TARGET_ADDR       0xAC1U
#define PHHAL_HW_PN5190_LPCD_RSSI_HYST_ADDR         0xAC3U

#define PHHAL_HW_PN5190_REG_START_ADDRESS_EXPERT    0x80U
#define PHHAL_HW_PN5190_REG_END_ADDRESS_EXPERT      0x8FU

/*@}*/

/**
* \name PN5190 LPCD configuration .
*
*/
/*@{*/
#define PHHAL_HW_PN5190_LPCD_CONFIG_MASK     0x80U

/*@}*/

/**
* \name PN5190 EPC GEN2 custom definitions.
* To be used with
* #phhalHw_Pn5190_Instr_EpcGen2Inventory,
* #phhalHw_Pn5190_Instr_EpcGen2ResumeInventory.
*/
/*@{*/
#define PHHAL_HW_PN5190_MIN_SELECT_COMMAND_LENGTH                          1U    /**< Minimum length of Select command. */
#define PHHAL_HW_PN5190_MAX_SELECT_COMMAND_LENGTH                          39U   /**< Maximum length of Select command. */
#define PHHAL_HW_PN5190_BEGIN_COMMAND_LENGTH                               03U   /**< length of Begin command. */

#define PHHAL_HW_PN5190_MAX_NUM_OF_BITS_FOR_LAST_BYTE                      7U    /**< Maximum number of bits to be transmitted in the last byte of \p pSelectCommand. 0 in case all bits shall be transmitted, otherwise 1-7. */
#define PHHAL_HW_PN5190_MAX_EPC_GEN2_TIMESLOT                              (PHHAL_HW_PN5190_EPC_GEN2_TIMESLOT_SINGLE_SELECT_TAG) /**< Maximum EPC GEN2 TIMESLOT parameter. */
/*@}*/


/**
* \name PN5190 RF configuration custom definitions.
* To be used with
* #phhalHw_Pn5190_Instr_LoadRfConfiguration,
* #phhalHw_Pn5190_Instr_UpdateRfConfiguration,
* #phhalHw_Pn5190_Instr_RetrieveRfConfiguration.
*/
/*@{*/
#define PHHAL_HW_PN5190_MIN_RF_TX_CONFIGURATION_INDEX                      0x00U  /**< Minimum RF configuration index for TX. */
#define PHHAL_HW_PN5190_MAX_RF_TX_CONFIGURATION_INDEX                      0x2BU  /**< Maximum RF configuration index for TX. */
#define PHHAL_HW_PN5190_MIN_RF_RX_CONFIGURATION_INDEX                      0x80U  /**< Minimum RF configuration index for RX. */
#define PHHAL_HW_PN5190_MAX_RF_RX_CONFIGURATION_INDEX                      0xABU  /**< Maximum RF configuration index for RX. */
#define PHHAL_HW_PN5190_MAX_RF_CONFIGURATION_INDEX                         PHHAL_HW_PN5190_MAX_RF_RX_CONFIGURATION_INDEX   /**< Maximum RF configuration index. */
#define PHHAL_HW_PN5190_CURRENT_RF_CONFIGURATION_INDEX                     0xFFU  /**< Current RF configuration index for TX/RX. */
#define PHHAL_HW_PN5190_RF_CONFIGURATION_SIZE                              6U    /**< Size of a single RF configuration
                                                                                  1 Byte Rx or Tx configuration index
                                                                                  1 Byte Register address within the RF configuration
                                                                                  4 Bytes value to be written into the register. */
#define PHHAL_HW_PN5190_RETRIEVE_RF_EACH_CONFIGURATION                     5U    /**< Size of a single RF configuration
                                                                                  1 Byte Register address within the RF configuration
                                                                                  4 Bytes value of the register. */

#define PHHAL_HW_PN5190_RETRIEVE_RF_CONFIGURATION_SIZE_RESPONSE             1U
#define PHHAL_HW_PN5190_MAX_RF_CONFIGURATION_SIZE                          90U  /**< Maximum size of the RF configurations. It must be a multiple of #PHHAL_HW_PN5190_RF_CONFIGURATION_SIZE. */
#define PHHAL_HW_PN5190_MIN_RF_CONFIGURATION_BUFFER_SIZE                   100U /**< Minimum buffer size that needs to be passed to get RF register configuration. */
/*@}*/

/**
 * \name PN5190 Write multiple register operation type.
 * To be used with #phhalHw_Pn5190_Instr_WriteRegisterMultiple.
 */
 /*@{*/
#define PHHAL_HW_PN5190_WRITE_MULTIPLE_TYPE_WRITE                             0x01U  /**< Write register operation requested. */
#define PHHAL_HW_PN5190_WRITE_MULTIPLE_TYPE_WRITE_OR_MASK                     0x02U  /**< Write register OR mask operation requested. */
#define PHHAL_HW_PN5190_WRITE_MULTIPLE_TYPE_WRITE_AND_MASK                    0x03U  /**< Write register AND mask operation requested. */
 /*@}*/


/**
* \name PN5190 register, EEPROM & TX/RX buffer custom definitions.
* To be used with
* #phhalHw_Pn5190_Instr_WriteRegister,
* #phhalHw_Pn5190_Instr_WriteRegisterOrMask,
* #phhalHw_Pn5190_Instr_WriteRegisterAndMask,
* #phhalHw_Pn5190_Instr_WriteRegisterMultiple,
* #phhalHw_Pn5190_Instr_ReadRegister,
* #phhalHw_Pn5190_Instr_ReadRegisterMultiple,
* #phhalHw_Pn5190_Instr_WriteE2Prom,
* #phhalHw_Pn5190_Instr_ReadE2Prom,
* #phhalHw_Pn5190_Instr_RetrieveRFData.
 */
 /*@{*/
#define PHHAL_HW_PN5190_MAX_REGISTER_TYPE_VALUE_SET                        (43U * 0x06U) /**< Size of the buffer holding the register-type-value sets. */
#define PHHAL_HW_PN5190_MIN_REGISTER_TYPE_VALUE_SET                        0x06U /**< Size of the buffer holding the register-type-value sets. Minimum size for one Register operation. */


#define PHHAL_HW_PN5190_TX_DATA_MAX_LENGTH                                 1024U/**< The Transmission maximum payload length is 1024 byte. */
#define PHHAL_HW_PN5190_TX_DATA_MIN_LENGTH                                 1U   /**< The Transmission minimum payload length is 1 byte. */

#define PHHAL_HW_PN5190_RX_DATA_MAX_LENGTH                                 0x200U/**< The Retrieved Maximum data length is 512 byte. */
#define PHHAL_HW_PN5190_RX_DATA_MIN_LENGTH                                 0x01U /**< The Retrieved Minimum data length is 1 byte. */

#define PHHAL_HW_PN5190_MAX_NUM_OF_BITS_FOR_LAST_BYTE                      7U    /**< Maximum number of bits to be transmitted in the last byte of \p pSelectCommand. 0 in case all bits shall be transmitted, otherwise 1-7. */

#define PHHAL_HW_PN5190_MAX_REGISTERS_READ_MULTIPLE                        18U /**< TODO : May need to be removed. */
#define PHHAL_HW_PN5190_BYTES_PER_REGISTER                                 4U


#define PH_ERR_PN5190_SUCCESS                                              PH_ERR_SUCCESS
#define PH_ERR_PN5190_TIMEOUT                                              PH_ERR_IO_TIMEOUT
#define PH_ERR_PN5190_INTEGRITY_ERROR                                      PH_ERR_INTEGRITY_ERROR
#define PH_ERR_PN5190_COLLISION_ERROR                                      PH_ERR_COLLISION_ERROR
#define PH_ERR_PN5190_INVALID_CMD                                          PH_ERR_UNSUPPORTED_COMMAND
#define PH_ERR_PN5190_AUTH_ERROR                                           PH_ERR_AUTH_ERROR
#define PH_ERR_PN5190_MEMORY_ERROR                                         PH_ERR_READ_WRITE_ERROR
#define PH_ERR_PN5190_NO_RF_FIELD                                          PH_ERR_RF_ERROR
#define PH_ERR_PN5190_SYNTAX_ERROR                                         PH_ERR_LENGTH_ERROR
#define PH_ERR_PN5190_RESOURCE_ERROR                                       PH_ERR_RESOURCE_ERROR
#define PH_ERR_PN5190_EXTERNAL_FIELD_ERROR                                 PH_ERR_EXT_RF_ERROR
#define PH_ERR_PN5190_USER_CALNCELLED                                      PH_ERR_ABORTED
#define PH_ERR_PN5190_PREVENT_STANDBY                                      0x13U
#define PH_ERR_PN5190_PARAMETER_ERROR                                      0x14U
#define PH_ERR_PN5190_PLL_ERROR                                            0x15U
#define PH_ERR_PN5190_SEMANTIC_ERROR                                       0x16U
#define PH_ERR_PN5190_PRBS_ERROR                                           0x17U
#define PH_ERR_PN5190_INSTR_ERROR                                          0x18U
#define PH_ERR_PN5190_ACCESS_DENIED                                        0x19U
#define PH_ERR_PN5190_TX_FAILURE                                           0x1AU
#define PH_ERR_PN5190_NO_ANTENNA                                           0x1BU
#define PH_ERR_PN5190_INTERNAL_ERROR                                       PH_ERR_INTERNAL_ERROR
#define PH_ERR_PN5190_SUCCESS_CHAINING                                     0xAFU

/*@}*/


/**
 * \name PN5190 MIFARE Classic authentication key type definition, length and status defines
 */
 /*@{*/

#define PHHAL_HW_PN5190_INSTR_MFC_AUTHENTICATE_KEY_TYPE_A                   0x60U  /**< MIFARE Classic Key Type A. */
#define PHHAL_HW_PN5190_INSTR_MFC_AUTHENTICATE_KEY_TYPE_B                   0x61U  /**< MIFARE Classic Key Type B. */
#define PHHAL_HW_PN5190_MFC_UID_LEN                                         0x04U  /**< MIFARE Classic Key Key Len. */

 /*@}*/

/**
* \name PN5190 Timeslot processing behavior.
*/
/*@{*/
#define PHHAL_HW_PN5190_EPC_GEN2_TIMESLOT_FILL_BUFFER                     0x00U  /**< EPC GEN2 inventory processes timeslots until buffer is full. */
#define PHHAL_HW_PN5190_EPC_GEN2_TIMESLOT_SINGLE                          0x01U  /**< EPC GEN2 inventory processes one timeslot only. */
#define PHHAL_HW_PN5190_EPC_GEN2_TIMESLOT_SINGLE_SELECT_TAG               0x02U  /**< EPC GEN2 inventory processes one timeslot only. If timeslot contains a valid tag response, the tag is also selected. */


/*@}*/

/**
 * \name PN5190 autocoll RF technology bitmask definition
 */
 /*@{*/
#define PHHAL_HW_PN5190_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A                    0x01U  /**< Autocoll for NFC-A (ISO14443 mode A) enabled. */
#define PHHAL_HW_PN5190_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F                    0x02U  /**< Autocoll for NFC-F (FeliCa) enabled. */
#define PHHAL_HW_PN5190_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A_ACTIVE             0x04U  /**< Autocoll for NFC_A target mode active enabled. */
#define PHHAL_HW_PN5190_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F_ACTIVE             0x08U  /**< Autocoll for NFC_F target mode active enabled. */
/*@}*/

/**
 * \name PN5190 autocoll operation mode
 */
 /*@{*/
#define PHHAL_HW_PN5190_AUTOCOLL_MODE_TERMINATE_ON_RF_OFF                 0x00U  /**< Autocoll terminates when external field vanishes.  */
#define PHHAL_HW_PN5190_AUTOCOLL_MODE_AUTONOMOUS_WITH_STANDBY             0x01U  /**< Autocoll terminates only upon successful activation. When external field vanishes, PN5180 enters standby. PN5180 wakes up again once external field is detected again. */
#define PHHAL_HW_PN5190_AUTOCOLL_MODE_AUTONOMOUS_WITHOUT_STANDBY          0x02U  /**< Autocoll terminates only upon successful activation. PN5180 does use standby mode. */
 /*@}*/

/**
 * \name PN5190 autocoll Evt Response
 */
 /*@{*/
#define PHHAL_HW_PN5190_AUTOCOLL_ACTIVATED                                0x00  /* PN5190 has been activated as a card / p2p device */
#define PHHAL_HW_PN5190_AUTOCOLL_EXTRF_OFF                                0x10  /* No Ext RF field or Ext RF field has vanished */
/*@}*/

/**
 * \name PN5190 autocoll Evt Payload defines
 */
 /*@{*/
#define PHHAL_HW_PN5190_AUTOCOLL_STATUS_INDEX                             0x00U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_ACT_INDEX                           0x01U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_STATUS_INDEX                        0x02U

#define PHHAL_HW_PN5190_AUTOCOLL_ACTIVATION_MODE_MASK                     0xF0U
#define PHHAL_HW_PN5190_AUTOCOLL_ACTIVATION_SPEED_MASK                    0x0FU

#define PHHAL_HW_PN5190_AUTOCOLL_ACTIVATION_PASSIVE_MODE                  0x10U
#define PHHAL_HW_PN5190_AUTOCOLL_ACTIVATION_ACTIVE_MODE                   0x20U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_PASSIVE_A_106                       0x10U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_PASSIVE_F_212                       0x11U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_PASSIVE_F_424                       0x12U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_ACTIVE_106                          0x20U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_ACTIVE_212                          0x21U
#define PHHAL_HW_PN5190_AUTOCOLL_TECH_ACTIVE_424                          0x22U
/*@}*/

/**
* \name LPCD modes
* Both the modes shall power down the IC based on the \ref PHHAL_HW_CONFIG_SET_LPCD_WAKEUPTIME_MS configuration
* time and performs LPCD after wakeup time.
* If no card is found the IC is powered down again and the procedure is restarted.
* If a card is found the LPCD function returns with LPCD event and the IC remains powered up.
*/
/*@{*/
#define PHHAL_HW_PN5190_LPCD_MODE_HW                 0x00U  /**< LPCD using HW configuration. Should be used for ULPCD Support of HW. NOTE : This functionality is not yet available. */
#define PHHAL_HW_PN5190_LPCD_MODE_SW                 0x01U  /**< LPCD using SW configuration. */
/*@}*/

/**
* \name LPCD modes
*/
/*@{*/
#define PHHAL_HW_PN5190_LPCD_MODE_DEFAULT            0x00U  /**< Default mode. Used to retrieve and configure the LPCD Calibration values and check if LPCD is already successful or not. */
#define PHHAL_HW_PN5190_LPCD_MODE_POWERDOWN          0x01U  /**< Powers down the IC for a certain amount of time and performs LPC after wakeup.
                                                                 If no card is found the IC is powered down again and the procedure is restarted.
                                                                 If a card is found the function returns and the IC remains powered up. */
/*@}*/

/**
* \name LPCD configuration
*/
/*@{*/

#define PHHAL_HW_CONFIG_SET_LPCD_WAKEUPTIME_MS       0x0070U     /**< Used value for wakeup counter in msecs, i.e. after this amount of time IC will wakes up from standby. */
#define PHHAL_HW_CONFIG_LPCD_MODE                    0x0071U     /**< Used to set options  PHHAL_HW_PN5190_LPCD_MODE_DEFAULT or PHHAL_HW_PN5190_LPCD_MODE_POWERDOWN */
#define PHHAL_HW_CONFIG_LPCD_REF                     0x0072U     /**< Used to set or get LPCD Ref */
#define PHHAL_HW_CONFIG_LPCD_CONFIG                  0x0073U     /**< Used to set options  PHHAL_HW_PN5190_LPCD_MODE_HW or PHHAL_HW_PN5190_LPCD_MODE_SW */


#define PHHAL_HW_CONFIG_LPCD_REF                     0x0072U     /**< Used to set or get LPCD Ref */
#define PHHAL_HW_LPCD_CALIBRATE_MASK                 0x10U       /**< Mask for LPCD Calibration. */
/*@}*/

/**
 * \name PN5190 PRBS parameter definition and status defines
 */
 /*@{*/
#define PHHAL_HW_PN5190_PRBS_TYPE_PRBS9                                     0x00U
#define PHHAL_HW_PN5190_PRBS_TYPE_PRBS15                                    0x01U

#define PHHAL_HW_PN5190_PRBS_TECHNO_ISO14443_A                              0x00U
#define PHHAL_HW_PN5190_PRBS_TECHNO_ISO14443_B                              0x01U
#define PHHAL_HW_PN5190_PRBS_TECHNO_FELICA                                  0x02U
#define PHHAL_HW_PN5190_PRBS_TECHNO_ISO15693                                0x03U

#define PHHAL_HW_PN5190_PRBS_BAUD_106                                       0x00U
#define PHHAL_HW_PN5190_PRBS_BAUD_212                                       0x01U
#define PHHAL_HW_PN5190_PRBS_BAUD_424                                       0x02U
#define PHHAL_HW_PN5190_PRBS_BAUD_848                                       0x03U
#define PHHAL_HW_PN5190_PRBS_BAUD_26                                        0x04U

#define PHHAL_HW_PN5190_PRBS_MIN_LENGTH                                     0x01U
#define PHHAL_HW_PN5190_PRBS_MAX_LENGTH                                     0x200U

/*@}*/


/**
* \name PN5190 Load Configuration for Tx and Rx baudrates/reg values.
* To be used with
* #phhalHw_ApplyProtocolSettings.
* Used by internal API phhalHw_Pn5190_SetCardMode.
*/
 /*@{*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_106_MILLER                      0U      /**< Type A baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_212_MILLER                      1U      /**< Type A baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_424_MILLER                      2U      /**< Type A baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_848_MILLER                      3U      /**< Type A baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443B_106_NRZ                         4U      /**< Type B baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443B_212_NRZ                         5U      /**< Type B baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443B_424_NRZ                         6U      /**< Type B baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443B_848_NRZ                         7U      /**< Type B baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_FELICA_212                                8U      /**< Type F baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_FELICA_424                                9U      /**< Type F baud rate @ 484 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AI_106_106                            0x00U   /**< Active Initiator baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AI_212_212                            0x08U   /**< Active Initiator baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AI_424_424                            0x09U   /**< Active Initiator baud rate @ 424 load configurations TX Value.*/

#define PHHAL_HW_PN5190_RF_TX_ISO15693_26_1OF4_ASK100                   0x0AU   /**< Type V (ISO 15693 ) load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO15693_26_1OF4_ASK10                    0x0BU   /**< Type V (ISO 15693 ) load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO180003M3_TARI_18_88_ASK                0x0FU   /**< ISO 18000P3M3 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO180003M3_TARI_9_44_ASK                 0x10U   /**< ISO 18000P3M3 load configurations TX Value.*/

#define PHHAL_HW_PN5190_RF_TX_ISO14443A_PICC_106_MANCH_SUBC             0x13U   /**< Type A Card mode baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_PICC_212_BPSK                   0x14U   /**< Type A Card mode baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_PICC_424_BPSK                   0x15U   /**< Type A Card mode baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_ISO14443A_PICC_848_BPSK                   0x16U   /**< Type A Card mode baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_PT_212                                0x17U   /**< Passive Target baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_PT_424                                0x18U   /**< Passive Target baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AT_106                                0x19U   /**< Active Target baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AT_212                                0x1AU   /**< Active Target baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_NFC_AT_424                                0x1BU   /**< Active Target baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5190_RF_TX_GTM_All_Al                                0x1CU   /**< General Target Mode configuration.*/

#define PHHAL_HW_PN5190_TX_NO_CHANGE                                    0xFFU

#define PHHAL_HW_PN5190_RF_RX_ISO14443A_106_MANCH_SUBC                  0x80U   /**< Type A baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_212_BPSK                        0x81U   /**< Type A baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_424_BPSK                        0x82U   /**< Type A baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_848_BPSK                        0x83U   /**< Type A baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443B_106_BPSK                        0x84U   /**< Type B baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443B_212_BPSK                        0x85U   /**< Type B baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443B_424_BPSK                        0x86U   /**< Type B baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443B_848_BPSK                        0x87U   /**< Type B baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_FELICA_212                                0x88U   /**< Type F baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_FELICA_424                                0x89U   /**< Type F baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_AI_106                                0x93U   /**< Active Initiator @ baud rate 106 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_AI_212                                0x97U   /**< Active Initiator @ baud rate 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_AI_424                                0x98U   /**< Active Initiator @ baud rate 424 load configurations RX Value.*/

#define PHHAL_HW_PN5190_RF_RX_ISO15693_26_1OF4_SC                       0x8BU   /**< Type V (ISO 15693 ) load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO15693_53_1OF4_SC                       0x8CU   /**< Type V (ISO 15693 ) load configurations RX Value.*/

#define PHHAL_HW_PN5190_RF_RX_ISO15693_106                              0x8DU   /**< Type (ISO 15693 106kbps) load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO15693_212                              0x8EU   /**< Type (ISO 15693 212kbps) load configurations RX Value.*/

#define PHHAL_HW_PN5190_RF_RX_ISO180003M3_MANCH424_4_PERIOD             0x8FU   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO180003M3_MANCH424_2_PERIOD             0x90U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO180003M3_MANCH848_4_PERIOD             0x91U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO180003M3_MANCH848_2_PERIOD             0x92U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_PICC_106_MILLER                 0x93U   /**< Type A Card mode baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_PICC_212_MILLER                 0x94U   /**< Type A Card mode baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_PICC_424_MILLER                 0x95U   /**< Type A Card mode baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_ISO14443A_PICC_848_MILLER                 0x96U   /**< Type A Card mode baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_PT_212                                0x97U   /**< Passive Target baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_PT_424                                0x98U   /**< Passive Target baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_AT_106                                0x99U   /**< Active Initiator baud rate @ 106 load configurations RX Value. */
#define PHHAL_HW_PN5190_RF_RX_NFC_AT_212                                0x9AU   /**< Active Initiator baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_NFC_AT_424                                0x9BU   /**< Active Initiator baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5190_RF_RX_GTM_All_All                               0x9CU   /**< General Target Mode configuration.*/

 /*@}*/


/**
* \name LPCD modes
*/
/*@{*/
#define PHHAL_HW_PN5190_LPCD_HW                                         0x00U  /**< HW LPCD, use LPCD HW block of Pn5190 */
#define PHHAL_HW_PN5190_LPCD_FW                                         0x01U  /**< Pn5190 FW based LPCD. */
#define PHHAL_HW_PN5190_LPCD_RESP_CARD_DETECTED                         0x00U  /**< LPCD finished with CARD detection. */

/*@}*/

/**
 * PN5190 CTS configuration parameter structure.
 * User shall fill all the configuration parameters that are required
 * for \ref phhalHw_Pn5190_Instr_CtsConfig in this data structure.
 * operations.
 */
#ifdef _WIN32
typedef struct
#else
typedef struct __attribute__((__packed__))
#endif /* _WIN32 */
{
    uint8_t bPreTriggerShift;                         /**< Defines values to be programmed in CTS_PRE_TRIGGER_SHIFT field in CLIF_CTS_MAIN_CTRL_REG */
    uint8_t bTriggerMode;                             /**< Specifies Acquisition mode to be used. 0x00 - POST mode. 0x01 - COMB Mode. 0x02 - PRE-Mode. 0x03 - 0xFF - Invalid */
    uint8_t bRamPageWidth;                            /**< Defines value to be programmed in CTS_RAM_PAGE_WIDTH field in CLIF_CTS_SAMPLER_REGISTER. Note: FW might end up using a smaller value if the defined value is not feasible. */
    uint8_t bSampleClkDiv;                            /**< Defines Value to be programmed in CTS_SAMPLE_CLK_DIV field in CLIF_CTS_SAMPLER_REGISTER. Values > 0x7 are invalid. */
    uint8_t bSampleByteSel;                           /**< Defines Value to be programmed in CTS_SAMPLE_BYTE_SEL field in CLIF_CTS_SAMPLER_REGISTER. Values > 0x0F are invalid. */
    uint8_t bSampleModeSel;                           /**< Defines Value to be programmed in CTS_SAMPLE_MODE_SEL field in CLIF_CTS_SAMPLER_REGISTER. Values > 0x2 are invalid. */
    uint8_t bTB0;                                     /**< Selects which Test bus to be connected to TB0. */
    uint8_t bTB1;                                     /**< Selects which Test bus to be connected to TB1. */
    uint8_t bTB2;                                     /**< Selects which Test bus to be connected to TB2. */
    uint8_t bTB3;                                     /**< Selects which Test bus to be connected to TB3. */
    uint8_t bTTBSelect;                               /**< Selects which CLIF Test bus to be connected to the trigger sources. */
    uint32_t dwClifCtsTbCtrl;                         /**< Value to be programmed in CLIF_CTS_TBCTRL_REG. */
    uint32_t dwHw0Trigger0;                           /**< Value to be programmed in CLIF_CTS_HW0_TRIGGER0_REG. */
    uint32_t dwHw0Trigger1;                           /**< Value to be programmed in CLIF_CTS_HW0_TRIGGER1_REG. */
    uint32_t dwHw0Trigger2;                           /**< Value to be programmed in CLIF_CTS_HW0_TRIGGER2_REG. */
    uint32_t dwHw1Trigger0;                           /**< Value to be programmed in CLIF_CTS_HW1_TRIGGER0_REG. */
    uint32_t dwHw1Trigger1;                           /**< Value to be programmed in CLIF_CTS_HW1_TRIGGER1_REG. */
    uint32_t dwHw1Trigger2;                           /**< Value to be programmed in CLIF_CTS_HW1_TRIGGER2_REG. */
} phhalHw_Pn5190_Instr_CtsConfig_t;


/**
 * PN5190 Version Information structure
 *
 */
typedef struct
{
    uint8_t bHw_Version;                              /**< Hardware version of PN5190. */
    uint8_t bROM_Version;                             /**< ROM version of PN5190. */
    uint16_t wFW_Version;                             /**< Firmware version of PN51909. */
}phhalHw_Pn5190_Version_t;

/**
 * \brief Write a 32 bit value (little endian) to a logical register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_INVALID_DATA_PARAMS wrong hal layer parameter.
 * \retval #PH_ERR_INTERFACE_ERROR front-end returned error.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_WriteRegister(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRegister,                                /**< [In] Address of the register. */
    uint32_t dwValue                                  /**< [In] 32 Bit register value which has to be written. */
    );

/**
 * \brief Perform a logical OR operation on a register using provided mask.
 *
 * This function is used to modify the content of register using a logical OR
 * operation. The content of the register is read and a logical OR operation
 * is performed with the provided mask. The modified content is written back
 * to the register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_READ_WRITE_ERROR Register is read-only.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_WriteRegisterOrMask(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRegister,                                /**< [In] Address of the register. */
    uint32_t dwMask                                   /**< [In] Bit-mask used as operand for logical OR operation. */
    );

/**
 * \brief Perform a logical AND operation on a register using provided mask.
 *
 * This function is used to modify the content of register using a logical
 * AND operation. The content of the register is read and a logical AND
 * operation is performed with the provided mask. The modified content is
 * written back to the register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_READ_WRITE_ERROR Register is read-only.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_WriteRegisterAndMask(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRegister,                                /**< [In] Address of the register. */
    uint32_t dwMask                                   /**< [In] Bit-mask used as operand for logical AND operation. */
    );

/**
 * \brief Perform a write register operation using a set of Register-Value pairs.
 *
 * This function behaves like #phhalHw_Pn5190_Instr_WriteRegister,
 * #phhalHw_Pn5190_Instr_WriteRegisterOrMask and
 * #phhalHw_Pn5190_Instr_WriteRegisterAndMask with the possibility to combine
 * them. In fact it takes an array of register-type-value set and performs
 * appropriate action. The type reflects the action which is either write
 * register, logical OR operation on a register or logical AND operation on
 * a register. The array is passed in as a buffer (\p pRegTypeValueSets) which
 * contains one or more sets. One set consists of 1 byte register address, 1
 * byte type (see \c #PHHAL_HW_PN5190_WRITE_MULTIPLE_TYPE_WRITE) and 4 bytes
 * value/mask.
 *
 * NOTE: \p wSizeOfRegTypeValueSets size shall be less than \c PHHAL_HW_PN5190_REG_END_ADDRESS * 6.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All registers successfully modified.
 * \retval #PH_ERR_INVALID_PARAMETER At least one unknown register address
 *         provided, or general parameter error (e.g. wSizeOfRegTypeValueSets
 *         is higher than 258)
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is read-only.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_WriteRegisterMultiple(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pRegTypeValueSets,                      /**< [In] A buffer containing one or more sets Register-Type-Value sets.  */
    uint16_t wSizeOfRegTypeValueSets                  /**< [In] Size of the buffer holding the register-type-value sets. */
    );

/**
 * \brief Read back content of a register.
 *
 * This function is used to read back the content of a logical register whose
 * address is given by \p bRegister.
 * The content is available in output parameter \p pValue.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully read out.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address provided or
 *         pValue set to NULL.
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is write-only.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_ReadRegister(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRegister,                                /**< [In] Address of the register. */
    uint32_t * pValue                                 /**< [Out] Pointer to a uint32_t variable, where the result (value of the read out register) shall be written to. */
    );

/**
 * \brief Perform a read register operation on a set of registers.
 *
 * This function is used to read multiple logical register at once.
 * The order of the register contents/values within the response (\p ppValues)
 * corresponds to the order of the register addresses within \p pRegisters.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All requested registers successfully read out.
 * \retval #PH_ERR_INVALID_PARAMETER At least one unknown register address
 *         provided, or general parameter error (e.g. \p bNumOfRegisters is
 *         higher than 16)
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is write-only.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_ReadRegisterMultiple(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pRegisters,                             /**< [In] A buffer containing list of registers (one byte each) to be read out. */
    uint8_t bNumOfRegisters,                          /**< [In] Number of registers to be read out (=size of \p pRegisters). Maximum size is 16. */
    uint8_t ** ppValues                               /**< [Out] A pointer to a buffer which contains the result, in fact the register values read out. Size of the buffer is 4 * \p bNumOfRegisters. */
    );

/**
 * \brief Write values sequentially to E2PROM.
 *
 * This function is used to write one or more values to E2PROM.
 * \p pDataToWrite shall contain the data to be written to E2PROM starting at
 * the address \p bE2PromAddress.
 * The data is written in sequential order.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data written.
 * \retval #PH_ERR_INVALID_PARAMETER \p pDataToWrite or \p pDataParams is NULL.
 * \retval #PH_ERR_PARAMETER_OVERFLOW Write operation attempts to write beyond end of E2PROM.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_WriteE2Prom(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wE2PromAddress,                          /**< [In] Start address within E2PROM. */
    uint8_t * pDataToWrite,                           /**< [In] Data to be written. */
    uint16_t wDataLength                              /**< [In] Number of bytes to be written. */
    );

/**
 * \brief Read values sequentially to E2PROM.
 *
 * This function is used to write one or more values to E2PROM.
 * \p pDataToWrite shall contain the data to be written to E2PROM starting at
 * the address \p bE2PromAddress.
 * The data is written in sequential order.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data read.
 * \retval #PH_ERR_INVALID_PARAMETER \p pReadData or \p pDataParams is NULL.
 * \retval #PH_ERR_PARAMETER_OVERFLOW Read operation attempts to read beyond
 *         end of E2PROM.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_ReadE2Prom(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t bE2PromAddress,                          /**< [In] Start address within E2PROM. */
    uint8_t * pReadData,                              /**< [Out] Read out data. */
    uint16_t wDataLength                              /**< [In] Number of bytes to be read. */
    );

/**
 * \brief Perform RF data transmit with initiator/target/PICC in initiator or
 * target modes.
 *
 * This instruction is used to write TX data to internal RF transmission buffer
 * and start transmission using Transceive command. The command returns
 * immediately after Transmission is complete.
 *
 * Note: If already a Trasceive command is started, only Data is transmitted by
 * setting Start_Send bit, otherwise a new Transceive command is started.
 *
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data written to internal Tx buffer and
 *         transmission started.
 * \retval #PH_ERR_INVALID_PARAMETER \p pTxBuffer or \p pDataParams is NULL,
 *         \p bTxBufferLength > 260 or \p bNumberOfValidBits > 7.
 * \retval #PH_ERR_ABORTED Instruction aborted.
*/

phStatus_t phhalHw_Pn5190_Instr_TransmitRFData(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                                 /**< [In] Option parameter. */
    uint8_t bTxLastBits,                              /**< [In] Number of valid bits in last byte. 0 means all bits are transferred */
    uint8_t * pTxBuffer,                              /**< [In] Buffer containing the data to be used for RF transmission. */
    uint16_t wTxBufferLength                          /**< [In] Size of the buffer \p pTxBuffer. */
    );

/**
 * \brief Used to receive data. Can be used in initiator/PCD and
 * target/PICC Modes.
 *
 * This instruction is used to read data from the internal CLIF RX buffer,
 * if already a reception is complete.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_RetrieveRFData(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** pRxBuffer,                             /**< [Out] Pointer to the buffer holding the data. */
    uint16_t * pRxDataLength                          /**< [Out] Number of received data bytes. Range 1 - 1024. */
    );


/**
 * \brief Used to receive data. Can be used in initiator/PCD and
 * target/PICC Modes.
 *
 * This instruction is a blocking call, returns when probable/expected Event has occurred. If any
 * of the config mask bit is set, then corresponding data is included as part of response.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_ReceiveRFData(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRespConfigMask,                          /**< [In] Mask to indicate whether to include RX Data OR status registers in response. */
    uint8_t ** ppRxBuffer,                            /**< [Out] Pointer to the buffer holding the data. */
    uint16_t * pRxDataLength                          /**< [Out] Number of received data bytes. Range 1 - 1024. */
    );


/**
 * \brief Switch to standby mode.
 *
 * This function is used to put PN5190 into standby mode. The wake up
 * conditions are defined using \p bWakeupControlMask parameter. The
 * instruction does not check if system actually entered standby or standby
 * was prevented. An Event will generated if standby is prevented Or when
 * Pn5190 wakes-up due to one of the reason mentioned in bWakeupControlMask.
 *
 * A typical way of using this API is given below\n
 * \li phhalHw_Pn5190_Instr_SwitchModeStandby()
 * \li phhalHw_EventWait() this will return in following scenarios\n
 *       a) #PH_ERR_IO_TIMEOUT EventWait is timed out\n
 *       b) #PH_ERR_ABORTED EventWait is aborted\n
 *       c) #PH_PN5190_EVT_BOOT Pn5190 wake-up due one of the standby wake-up control reason\n
 *       d) #PH_PN5190_EVT_STANDBY_PREV Pn5190 could not enter standby\n
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed
 * \retval #PH_ERR_INVALID_PARAMETER No wake up condition defined or
 *         \p wWakeupCounterInMs is out of range.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_SwitchModeStandby(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bWakeupControlMask,                       /**< [In] Bit-mask controlling the standby wake-up.*/
    uint16_t wWakeupCounterInMs                       /**< [In] Used value for wake-up counter in milliseconds, i.e. after this amount of time PN5190 wakes up from standby. Maximum value is 2690. */
    );


/**
 * \brief Set LPCD Configuration parameter.
 *
 * This instruction is used to configure LPCD parameters.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER \p wValue not is valid for the wConfig.
 * \retval #PH_ERR_UNSUPPORTED_PARAMETER \p wConfig is not supported one.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_LPCD_SetConfig(
                                               phhalHw_Pn5190_DataParams_t * pDataParams,
                                               uint16_t wConfig,
                                               uint16_t wValue
                                               );

/**
 * \brief Get LPCD Configuration parameter.
 *
 * This instruction is used to read back the value of a give LPCD parameter.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_UNSUPPORTED_PARAMETER \p wConfig is not supported one.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_LPCD_GetConfig(
                                               phhalHw_Pn5190_DataParams_t * pDataParams,
                                               uint16_t wConfig,
                                               uint16_t * pValue
                                               );

/**
 * \brief Switch to LPCD mode.
 *
 * This function is used to put PN5190 into LPCD mode. The wait period in
 * msecs between two LPCD cycles is defined by \p wWakeupCounterValue. During
 * wait period PN5190 is in standby. The function returns immediately, i.e.
 * it does not wait until a card has been detected. On detection of a card,
 * PN5190 raises an interrupt and normal mode is entered again.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER \p wWakeupCounterInMs is out of range.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_SwitchModeLpcd(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t  bLpcdMode,                               /**< [In] Lpcd Mode; 1) 0x00 Hw LPCD, 2) 0x01 FW Lpcd. */
    uint16_t wWakeupCounterInMs                       /**< [In] Used value for wake-up counter in milliseconds, i.e. after this amount of time PN5190 wakes up from standby. Maximum value is 2690. */
    );

/**
 * \brief Switch to Autocoll mode.
 *
 * This function is used to put PN5190 into Autocoll mode in order to handle
 * target mode activation.
 * The termination condition depends on \p bAutocollMode.
 * The function returns immediately, i.e. it does not wait until autocoll
 * terminates. Once terminated, autocoll raises an interrupt and normal mode
 * is entered again.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER \p wWakeupCounterInMs is 0.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_SwitchModeAutocoll(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRfTechnologyMask,                        /**< [In] Bit-mask indicating the RF technology to listen for during Autocoll. For example see #PHHAL_HW_PN5190_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A. */
    uint8_t bAutocollMode                             /**< [In] Autocoll operation mode to be used. See for example #PHHAL_HW_PN5190_AUTOCOLL_MODE_TERMINATE_ON_RF_OFF. */
    );

/**
 * \brief Perform authentication on an activated Mifare Classic card.
 *
 * This function is used to perform a Mifare Classic Authentication on an
 * activated card. It takes the key, card UID and the key type to authenticate
 * at given block address.
 *
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Authentication successful.
 * \retval #PH_ERR_INVALID_PARAMETER \p bKeyType wrong, \p pUid or \p pKey
 *         is NULL.
 * \retval #PH_ERR_IO_TIMEOUT Timeout during authentication.
 * \retval #PH_ERR_AUTH_ERROR Authentication failed.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_MifareAuthenticate(
    phhalHw_Pn5190_DataParams_t * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pKey,                                    /**< [In] Key to be used for authentication. uint8_t[6]. */
    uint8_t bKeyType,                                  /**< [In] Key type. Either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
    uint8_t bBlockNo,                                  /**< [In] Block-Number on card to authenticate to. */
    uint8_t * pUid                                     /**< [In] UID of the activated card. uint8_t[4]. */
    );

/**
 * \brief Perform an ISO 18000-3M3 Inventory.
 *
 * This function is used to perform an inventory of ISO18000-3M3 tags.
 * It implements an autonomous execution of several commands according to
 * ISO18000-3M3 in order to guarantee the timings specified by that standard.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Inventory executed, internal response buffer filled
 *         according to the \p bTimeslotProcessingBehavior policy.
 * \retval #PH_ERR_INVALID_PARAMETER \p pBeginRoundCommand is NULL or wrong
 *         value for one of \p bTimeslotProcessingBehavior,
 *         \p bSelectCommandBitsInLastByte or \p bSelectCommandLength.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_EpcGen2Inventory(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pSelectCommand,                         /**< [In] If present (not NULL) it contains the Select command (according to ISO18000-3, Table 47) which is sent prior to BeginRound command. CRC-16c shall not be included. Size of the buffer is between 1-39 bytes, inclusive. */
    uint8_t bSelectCommandLength,                     /**< [In] Size of \p pSelectCommand buffer. 0 in case no Select Command shall be sent, otherwise 1-39. */
    uint8_t bSelectCommandBitsInLastByte,             /**< [In] Number of bits to be transmitted in the last byte of \p pSelectCommand. 0 in case all bits shall be transmitted, otherwise 1-7. */
    uint8_t * pBeginRoundCmmand,                      /**< [In] Buffer containing BeginRound command (according to ISO18000-3, Table 49). CRC-5 shall not be included. uint8_t[3]. */
    uint8_t bTimeslotProcessingBehavior,              /**< [In] Time slot processing behavior. Indicates how many time slots shall be processed before returning response to the host. For example see: PHHAL_HW_PN5190_EPC_GEN2_TIMESLOT_FILL_BUFFER. */
    uint8_t ** ppRxBuffer,                            /**< [In] Pointer to HAL Rx Buffer containing response from single/multiple tags. */
    uint16_t * pRxDataLength                          /**< [In] Length of response in hal Rx Buffer. */
    );

/**
 * \brief Resume an ISO 18000-3M3 Inventory.
 *
 * This function is used to resume the inventory algorithm in case it is paused.
 * More specifically, this instruction has to be repeatedly called,
 * as long as \p pResultSize of #phhalHw_Pn5190_Instr_EpcGen2RetrieveResultSize
 * is greater than 0.
 * Typical sequence for a complete EPC GEN2 inventory is
 * 1.) #phhalHw_Pn5190_Instr_EpcGen2Inventory
 * 2.) if \p pResultSize is 0, inventory has finished.
 * 3.) #phhalHw_Pn5190_Instr_EpcGen2RetrieveResult
 * 4.) #phhalHw_Pn5190_Instr_EpcGen2ResumeInventory
 * 5.) Go to step 2.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Inventory resumed, internal response buffer filled
 *         according to the \p bTimeslotProcessingBehavior policy set during
 *         execution of #phhalHw_Pn5190_Instr_EpcGen2Inventory.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_EpcGen2ResumeInventory(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                            /**< [In] Pointer to HAL Rx Buffer containing response from single/multiple tags. */
    uint16_t * pRxDataLength                          /**< [In] Length of response in hal Rx Buffer. */
    );

/**
 * \brief Load RF Tx and Rx settings from E2PROM.
 *
 * This function is used to load the RF configuration from E2PROM into
 * internal CLIF registers. RF configuration refers to a unique combination of
 * RF Technology, mode (target/initiator) and baud rate. RF configuration can
 * be loaded separately for the CLIF receiver (Rx configuration) and
 * transmitter (Tx configuration) path.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully loaded.
 * \retval #PH_ERR_INVALID_PARAMETER \p bTxConfiguration or \p bRxConfiguration
 *         out of range.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_LoadRfConfiguration(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRfTxConfiguration,                       /**< [In] TX configuration index to be used. 0xff means TX Configuration is not modified. */
    uint8_t bRfRxConfiguration                        /**< [In] RX configuration index to be used. 0xff means RX Configuration is not modified. */
    );

/**
 * \brief Update the RF configuration
 *
 * This function is used to update the RF configuration within E2PROM.
 * Only registers available within the respective RF configuration can be
 * updated, registers can't be added or removed.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully updated.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfConfigurationSize not a multiple of
 *         6 or out of range. \p pRfConfiguration is NULL.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_UpdateRfConfiguration(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pRfConfiguration,                       /**< [In] Array containing one or more configurations to be written. One configuration is a set of
                                                           1 Byte RX or TX configuration index
                                                           1 Byte Register address within the RF configuration
                                                           4 Bytes value to be written into the register. */
    uint8_t  bRfConfigurationSize                     /**< [In] Size of \p pRfConfiguration. Must be a multiple of 6, maximum value is 252. */
    );


/**
 * \brief Retrieve the RF configuration.
 *
 * This function is used to read out an Protocol RF configuration of PN5190.
 * User needs to provide \p pRfConfBuf buffer of minimum length #PHHAL_HW_PN5190_MIN_RF_CONFIGURATION_BUFFER_SIZE
 * to get all the register address-value-pairs of a particular protocol which can be
 * sent using \p bRfConfiguration parameter.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully retrieved.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfConfiguration out of range.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_RetrieveRfConfiguration(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRfConfiguration,                         /**< [In] RF TX or RX configuration index to be used. */
    uint8_t * pRfConfBuf,                             /**< [Out] Pointer to a buffer containing \p bNumOfRegisters configurations. One configuration is a set of
                                                           1 Byte Register address within the RF configuration
                                                           4 Bytes value to be written into the register. */
    uint16_t * pRfConfBufSize                         /**< [InOut] Input the application buffer size and output with valid RF configuration data to application. */
    );

/**
 * \brief Turns the RF field ON.
 *
 * This function is used to turn the RF field ON.
 * The configurations are available in \p ppRfOnConfig.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF Field turn ON sent to IC.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfOnConfig out of range.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_RfOn(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRfOnConfig                               /**< [In] The ON Configuration.
                                                           Bit 0   0   Use collision avoidance.
                                                                   1   Disable collision avoidance.
                                                           Bit 1   0   No P2P active.
                                                                   1   P2P active.*/
    );

/**
 * \brief Turns the RF field OFF.
 *
 * This function is used to turn the RF field OFF.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF Field turn OFF sent to IC.
 * \retval #PH_ERR_INVALID_PARAMETER \p PData params is null.
 * \retval #PH_ERR_ABORTED Instruction aborted.
 */
phStatus_t phhalHw_Pn5190_Instr_RfOff(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
 * \brief Switch to Normal Boot mode.
 *
 * This function is used, after POR,  to Boot PN5190 into Normal mode.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 */
phStatus_t phhalHw_Pn5190_Instr_SwitchModeBootNormal(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
 * \brief Switch to Normal mode.
 *
 * This function is used when system is already operating in Normal mode and
 * is waiting for some event to happen like after sending Switch_Mode_AutoColl.
 * This command brings PN5190 back to normal mode of operation.
 *
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 */
phStatus_t phhalHw_Pn5190_Instr_SwitchModeNormal(
    phhalHw_Pn5190_DataParams_t * pDataParams         /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Perform RF data exchange for Reader mode P2P” Passive/Active Initiator mode. This
* is a blocking call.
*
* This instruction writes data to internal CLIF transmission buffer and, additionally, starts the RF
* transmission. The instruction returns, after end of transmission and end
* of reception.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_ExchangeRFData(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                                 /**< [In] Option parameter. */
    uint8_t bTxLastBits,                              /**< [In] Number of valid bits in last TX byte. Range 0 – 7. */
    uint8_t bRespControl,                             /**< [In] Control flag to include RX Data and status registers in response. */
    uint8_t * pTxData,                                /**< [In] Data to transmit. */
    uint16_t wTxDataLength,                           /**< [In] Number of bytes to transmit. Range 1 – 261. */
    uint8_t ** ppRxData,                              /**< [Out] Pointer to received data (will also contain receive Rf data, if RX Data bit is set in bRespControl). */
    uint16_t * pRxDataLength                          /**< [Out] Number of received data bytes. Range 1 - 1024. */
    );

/**
* \brief This instruction is used to configure the digital test bus feature.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_ConfigureTestBusDigital(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bSignalIndex,                             /**< [In] Signal Index. */
    uint8_t bBitIndex,                                /**< [In] Bit Index. */
    uint8_t bPadIndex                                 /**< [In] Pad Index. */
    );


/**
* \brief This instruction is used to enable/disable the CTS logging feature.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter is invalid.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_CtsEnable(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption                                   /**< [In] Enable/Disable the CTS logging feature. 0 - Disable. 1 - Enable. */
    );

/**
* \brief This instruction is used to write configuration values to all the
* required CTS Registers such as Triggers, Test Bus registers, Sampling
* configuration etc.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_CtsConfig(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    phhalHw_Pn5190_Instr_CtsConfig_t * pCtsConfig     /**< [In] Pointer to CTS configuration data structure. */
    );

/**
* \brief This instruction retrieves the data log of the captured test bus data
* samples stored in the memory buffer. Buffer size of \b pLogData shall one more then
* the \b bChunkSize provided by the user.
*
* If #PH_ERR_SUCCESS_CHAINING is returned by this API then the user shall call this
* API in a loop until #PH_ERR_SUCCESS is returned by this API to analyze the complete
* CTS Log.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING Complete CTS Log data is not yet been retrieved from PN5190.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_PN5190_ERR_NO_CTS_EVENT Shall be returned if CTS Event is not been received from PN5190.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_CtsRetrieveLog(
    phhalHw_Pn5190_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bChunkSize,                               /**< [In] Expected size of Log data that needs to be retrieved from PN5190. */
    uint8_t * pLogData,                               /**< [Out] The data log of the captured data samples stored in the memory buffer. */
    uint8_t * bLogDataSize                            /**< [Out] Size of the retrieved data log of the captured data samples stored in the memory buffer. */
    );


/**
* \brief This instruction retrieves the DieID of the CHIP
* Buffer size of 16 bytes shall by provided by the user
* to read DieId information.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER Input parameter is invalid.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_GetDieId(
    phhalHw_Pn5190_DataParams_t * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
    uint8_t *pDieIdValue                             /**< [InOut] User to provide buffer of 16 byte to get DieId of the PN5190 chip. */
    );

/**
* \brief This instruction retrieves Hw Version, ROM Version
* and FW version. Buffer of size N + 1 bytes shall be provided
* by the user, where N is the number of byte required for version
* information,
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER Input parameter is invalid.
* \retval #PH_ERR_INTERNAL_ERROR Operation failed.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_GetVersion(
    phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
    phhalHw_Pn5190_Version_t  *pVersion             /**< [InOut] structure pointer for, 4 byte Version Information read from PN5190. */
    );


/**
* \brief This instruction switches operation mode of
* Pn5190 to FIRMWARE_DOWNLOAD mode.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INTERNAL_ERROR Switch Mode has not been Entered.
*/
phStatus_t phhalHw_Pn5190_Instr_SwitchModeDownload(
        phhalHw_Pn5190_DataParams_t *pDataParams        /**< [In] Pointer to this layer's parameter structure. */
        );


/**
* \brief This instruction send pseudo-random binary sequence. Once this instruction is received
* the Pn5190 will continue to send the pseudo-random binary sequence until Switch_Mode_Normal
* command is sent.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INTERNAL_ERROR Switch Mode has not been Entered.
* \retval #PH_ERR_ABORTED Instruction aborted.
*/
phStatus_t phhalHw_Pn5190_Instr_PrbsTest(
        phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bPrbs_type,                             /**< [In] Prbs_type a) 0x00 PRBS9 b) 0x01 PRBS15 */
        uint8_t bPrbs_techno,                           /**< [In] Prbs_techno a) 0x00 ISO14443-A b) 0x01 ISO14443-B C) 0x02 Felica D) 0x03 ISO15693  */
        uint8_t bPrbs_Baud,                             /**< [In] Prbs_baud a) 0x00 106 b) 212 c) 424 d) 848 e) 26 (default for ISO15693). */
        uint16_t wPrbs_Len                              /**< [In] Data Length of the Transmission. Length range 1 byte - 510 byte. */
        );

/** @}
* end of phhalHw_PN5190_Instr group
*/

#endif /* NXPBUILD__PHHAL_HW_PN5190 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#endif  /* PHHALHW_PN5190_INSTR_H */
