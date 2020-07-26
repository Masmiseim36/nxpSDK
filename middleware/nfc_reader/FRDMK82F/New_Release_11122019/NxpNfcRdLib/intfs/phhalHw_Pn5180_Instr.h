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
* PN5180 HAL Instruction Layer.
** $Author:nxp62726 $
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5180_INSTR_H
#define PHHALHW_PN5180_INSTR_H

#include <ph_Status.h>


#if defined( NXPBUILD__PHHAL_HW_PN5180 )

/** \defgroup phhalHw_PN5180_Instr Instruction
* \brief Pn5180 Instruction functions
* @{
*/


  /**
    * \name Major Version
    */
    /*@{*/
#define HAL_MAJOR_VER   ((uint16_t)(3))

  /**
    * \name Minor Version
    */
    /*@{*/
#define HAL_MINOR_VER   ((uint16_t)(4))

   /**
    * \name PN5180 Instruction Codes.
    * To be used with #phhalHw_Pn5180_Instr_Execute
    */
    /*@{*/
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER                               0x00U  /**< Write 32 bit value to a register. */
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER_OR_MASK                       0x01U  /**< Perform a logical OR operation on a register using provided mask. */
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER_AND_MASK                      0x02U  /**< Perform a logical AND operation on a register using provided mask. */
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER_MULTIPLE                      0x03U  /**< Perform a write register operation using a set of Register-Value pairs. */
#define PHHAL_HW_PN5180_GET_INSTR_READ_REGISTER                                0x04U  /**< Read back content of a register. */
#define PHHAL_HW_PN5180_GET_INSTR_READ_REGISTER_MULTIPLE                       0x05U  /**< Perform a read register operation on a set of registers. */
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_E2PROM                                 0x06U  /**< Write given values sequentially to E2PROM. */
#define PHHAL_HW_PN5180_GET_INSTR_READ_E2PROM                                  0x07U  /**< Read out values from E2PROM sequentially. */
#define PHHAL_HW_PN5180_SET_INSTR_WRITE_TX_DATA                                0x08U  /**< Write Tx data to internal CLIF transmission buffer. */
#define PHHAL_HW_PN5180_SET_INSTR_SEND_DATA                                    0x09U  /**< Write Tx data to internal RF transmission buffer and starts transmission using transceive command. */
#define PHHAL_HW_PN5180_GET_INSTR_RETRIEVE_RX_DATA                             0x0AU  /**< Read Rx data from internal RF reception buffer. */
#define PHHAL_HW_PN5180_SET_INSTR_SWITCH_MODE                                  0x0BU  /**< Switch operation mode. Possible modes are: Normal, LPCD, Autocoll, Standby. */
#define PHHAL_HW_PN5180_GET_INSTR_MFC_AUTHENTICATE                             0x0CU  /**< Perform authentication on an activated MIFARE Classic contactless IC card. */
#define PHHAL_HW_PN5180_SET_INSTR_EPC_GEN2_INVENTORY                           0x0DU  /**< Perform an ISO 18000-3M3 Inventory. */
#define PHHAL_HW_PN5180_SET_INSTR_EPC_GEN2_RESUME_INVENTORY                    0x0EU  /**< Resume ISO18000-3M3 Inventory. */
#define PHHAL_HW_PN5180_GET_INSTR_EPC_GEN2_RETRIEVE_INVENTORY_RESULT_SIZE      0x0FU  /**< Retrieve the size of a pending EPC GEN2 inventory result. */
#define PHHAL_HW_PN5180_GET_INSTR_EPC_GEN2_RETRIEVE_INVENTORY_RESULT           0x10U  /**< Return the result of inventory. */
#define PHHAL_HW_PN5180_SET_INSTR_LOAD_RF_CONFIGURATION                        0x11U  /**< Load RF Tx and Rx settings from E2PROM. */
#define PHHAL_HW_PN5180_SET_INSTR_UPDATE_RF_CONFIGURATION                      0x12U  /**< Update the RF configuration. */
#define PHHAL_HW_PN5180_GET_INSTR_RETRIEVE_RF_CONFIGURATION_SIZE               0x13U  /**< Retrieve the number of register-value pairs of the RF configuration. */
#define PHHAL_HW_PN5180_GET_INSTR_RETRIEVE_RF_CONFIGURATION                    0x14U  /**< Retrieve the RF configuration. */
#define PHHAL_HW_PN5180_SET_INSTR_CONFIGURE_TESTBUS_RFU                        0x15U  /**< Retrieve the RF configuration. */
#define PHHAL_HW_PN5180_GET_INSTR_FIELD_ON                                     0x16U  /**< RF Field ON. */
#define PHHAL_HW_PN5180_GET_INSTR_FIELD_OFF                                    0x17U  /**< RF field OFF.*/
#define PHHAL_HW_PN5180_SET_INSTR_CONFIGURE_TESTBUS_DIGITAL                    0x18U  /**< Digital Testbus.*/
#define PHHAL_HW_PN5180_SET_INSTR_CONFIGURE_TESTBUS_ANALOG                     0x19U  /**< Analog Testbus*/


/*@}*/


   /**
    * \name PN5180 Write multiple register operation type.
    * To be used with #phhalHw_Pn5180_Instr_WriteRegisterMultiple.
    */
    /*@{*/
#define PHHAL_HW_PN5180_WRITE_MULTIPLE_TYPE_WRITE                             0x01U  /**< Write register operation requested. */
#define PHHAL_HW_PN5180_WRITE_MULTIPLE_TYPE_WRITE_OR_MASK                     0x02U  /**< Write register OR mask operation requested. */
#define PHHAL_HW_PN5180_WRITE_MULTIPLE_TYPE_WRITE_AND_MASK                    0x03U  /**< Write register AND mask operation requested. */
    /*@}*/

   /**
    * \name PN5180 wakeup control bitmask defintion
    */
    /*@{*/
#define PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_WUP_ON_COUNTER                    0x01U  /**< Wake up from standby on wakeup counter expire. */
#define PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_WUP_ON_EXT_FIELD                  0x02U  /**< Wake up from standby due to external RF field. */
#define PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_ASSERT_GPO                        0x04U  /**< Assert GPO when leaving standby. */
    /*@}*/

   /**
    * \name PN5180 autocoll RF technology bitmask definition
    */
    /*@{*/
#define PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A                    0x01U  /**< Autocoll for NFC-A (ISO14443 mode A) enabled. */
#define PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F                    0x02U  /**< Autocoll for NFC-F (FeliCa) enabled. */
#define PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_ISO18092_ACTIVE          0x04U  /**< Autocoll for ISO18092 target mode active enabled. */
#define PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F_ACTIVE             0x08U  /**< Autocoll for NFC_F target mode active enabled. */
/*@}*/

   /**
    * \name PN5180 autocoll operation mode
    */
    /*@{*/
#define PHHAL_HW_PN5180_AUTOCOLL_MODE_TERMINATE_ON_RF_OFF                 0x00U  /**< Autocoll terminates when external field vanishes. */
#define PHHAL_HW_PN5180_AUTOCOLL_MODE_AUTONOMOUS_WITH_STANDBY             0x01U  /**< Autocoll terminates only upon successful activation. When external field vanishes, PN5180 enters standby. PN5180 wakes up again once external field is detected again. */
#define PHHAL_HW_PN5180_AUTOCOLL_MODE_AUTONOMOUS_WITHOUT_STANDBY          0x02U  /**< Autocoll terminates only upon successful activation. PN5180 does use standby mode. */
    /*@}*/

   /**
    * \name PN5180 MIFARE Classic authentication key type definition and length defines
    */
    /*@{*/

#define PHHAL_HW_PN5180_INSTR_MFC_AUTHENTICATE_KEY_TYPE_A                   0x60U  /**< MIFARE Classic Key Type A. */
#define PHHAL_HW_PN5180_INSTR_MFC_AUTHENTICATE_KEY_TYPE_B                   0x61U  /**< MIFARE Classic Key Type B. */
#define PHHAL_HW_PN5180_MFC_UID_LEN                                         0x04U  /**< MIFARE Classic Key Key Len. */
    /*@}*/

   /**
    * \name PN5180 Timeslot processing behavior.
    */
    /*@{*/
#define PHHAL_HW_PN5180_EPC_GEN2_TIMESLOT_FILL_BUFFER                     0x00U  /**< EPC GEN2 inventory processes timeslots until buffer is full. */
#define PHHAL_HW_PN5180_EPC_GEN2_TIMESLOT_SINGLE                          0x01U  /**< EPC GEN2 inventory processes one timeslot only. */
#define PHHAL_HW_PN5180_EPC_GEN2_TIMESLOT_SINGLE_SELECT_TAG               0x02U  /**< EPC GEN2 inventory processes one timeslot only. If timeslot contains a valid tag response, the tag is also selected. */
    /*@}*/


 /**
    * \name PN5180 Switch Mode Type type.
    */
    /*@{*/
#define PHHAL_HW_PN5180_SWITCH_MODE_STANDBY                               0x00U  /**< PN5180 is in standby/sleep state to save power. */
#define PHHAL_HW_PN5180_SWITCH_MODE_LPCD                                  0x01U  /**< PN5180 is in low power card detection mode. */
#define PHHAL_HW_PN5180_SWITCH_MODE_AUTOCOLL                              0x02U  /**< PN5180 is acting as RF listener, performing target mode activation autonomously. */
#define PHHAL_HW_PN5180_SWITCH_MODE_NORMAL                                0x03U  /**< PN5180 is in Normal modes. */

    /*@}*/


   /**
   * \name PN5180 register, EEPROM & TX/RX buffer custom definitions.
   * To be used with
   * #phhalHw_Pn5180_Instr_WriteRegister,
   * #phhalHw_Pn5180_Instr_WriteRegisterOrMask,
   * #phhalHw_Pn5180_Instr_WriteRegisterAndMask,
   * #phhalHw_Pn5180_Instr_WriteRegisterMultiple,
   * #phhalHw_Pn5180_Instr_ReadRegister,
   * #phhalHw_Pn5180_Instr_ReadRegisterMultiple,
   * #phhalHw_Pn5180_Instr_WriteE2Prom,
   * #phhalHw_Pn5180_Instr_ReadE2Prom,
   * #phhalHw_Pn5180_Instr_WriteTxData,
   * #phhalHw_Pn5180_Instr_SendData,
   * #phhalHw_Pn5180_Instr_RetrieveRxData.
    */
    /*@{*/
#define PHHAL_HW_PN5180_MAX_REGISTER_TYPE_VALUE_SET                       0x102U /**< Size of the buffer holding the register-type-value sets. Maximum size supported is 258 = 43 sets. */
#define PHHAL_HW_PN5180_MIN_REGISTER_TYPE_VALUE_SET                        0x06U /**< Size of the buffer holding the register-type-value sets. Minimum size supported is 6 = 1 set. */

#define PHHAL_HW_PN5180_E2PROM_MAX_DATA_LENGTH                             0x41U /**< The E2PROM maximum payload length is 254 byte. */
#define PHHAL_HW_PN5180_E2PROM_MIN_DATA_LENGTH                             0x01U /**< The E2PROM minimum payload length is 1 byte. */
#define PHHAL_HW_PN5180_E2PROM_MAX_ADDESS                                  0xFDU /**< The E2PROM maximum Address is 0x93. */
#define PHHAL_HW_PN5180_E2PROM_MIN_ADDESS                                  0x16U  /**< The E2PROM minimum Address 0. */

#define PHHAL_HW_PN5180_TX_DATA_MAX_LENGTH                                 0x104U/**< The Transmission maximum payload length is 260 byte. */
#define PHHAL_HW_PN5180_TX_DATA_MIN_LENGTH                                 0x01U /**< The Transmission minimum payload length is 1 byte. */

#define PHHAL_HW_PN5180_RX_DATA_MAX_LENGTH                                 0x200U/**< The Retrieved Maximum data length is 512 byte. */
#define PHHAL_HW_PN5180_RX_DATA_MIN_LENGTH                                 0x01U /**< The Retrieved Minimum data length is 1 byte. */

#define PHHAL_HW_PN5180_INSTR_DATA_MIN_VALUE                               0x01U /**< The Minimum data value 01. */

#define PHHAL_HW_PN5180_INVALID_REG_START_ADDRESS                          0x44U /**< Regs from 0x45 to 0x7F are not implemented Update everytime regmap changes due to reg addition.*/
#define PHHAL_HW_PN5180_INVALID_REG_END_ADDRESS                            0x7FU /**< Regs from 0x45 to 0x7F are not implemented Update everytime regmap changes due to reg addition.*/



#define PHHAL_HW_PN5180_MAX_REGISTERS_READ_MULTIPLE                        18U /**< Maximum registers for multiple Read*/
#define PHHAL_HW_PN5180_BYTES_PER_REGISTER                                 4U
#define PHHAL_HW_PN5180_SEND_DATA_FIXED_CMD_BYTES                          2U
#define PHHAL_HW_PN5180_WRITE_TX_DATA_FIXED_CMD_BYTES                      1U
/*@}*/

   /**
    * \name PN5180 Execute instruction custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_Execute.
    */
    /*@{*/
#define PHHAL_HW_PN5180_MAX_INSTRUCTION_CODE                               PHHAL_HW_PN5180_GET_INSTR_RETRIEVE_RF_CONFIGURATION            /**< Maximum value of the instruction code. */
#define PHHAL_HW_PN5180_MIN_PAYLOAD_LENGTH                                 1U                                                             /**< Minimum payload length for the instruction. */
#define PHHAL_HW_PN5180_MAX_PAYLOAD_LENGTH                                 261U                                                           /**< Maximum payload length for the instruction. */
#define PHHAL_HW_PN5180_MAX_INSTRUCTION_LENGTH                             (1U + PHHAL_HW_PN5180_MAX_PAYLOAD_LENGTH)                      /**< Maximum length for the instruction. 1 byte is for the instruction code */
#define PHHAL_HW_PN5180_MAX_INST_RESPONSE_LENGTH                           (0x200U)                                                       /**< Maximum length for the instruction response */


/*@}*/

   /**
    * \name PN5180 standby custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_SwitchModeStandby.
    */
    /*@{*/
#define PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK                                (PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_WUP_ON_COUNTER \
                                                                           | PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_WUP_ON_EXT_FIELD \
                                                                           | PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_ASSERT_GPO)                /**< Mask value for wakeup control. */

#define PHHAL_HW_PN5180_MAX_WAKEUP_COUNTER                                 (2690U)                                                            /**< Maximum value for wakeup counter in msecs. */

    /*@}*/

   /**
    * \name PN5180 Autocoll custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_SwitchModeAutocoll.
    */
    /*@{*/
#define PHHAL_HW_PN5180_MAX_AUTOCOLL_MODE                                  PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_ASSERT_GPO                 /**< Maximum value for Autocoll mode. */
#define PHHAL_HW_PN5180_AUTOCOLL_MODE_MASK                                 (PHHAL_HW_PN5180_AUTOCOLL_MODE_TERMINATE_ON_RF_OFF  \
                                                                           | PHHAL_HW_PN5180_AUTOCOLL_MODE_AUTONOMOUS_WITH_STANDBY \
                                                                           | PHHAL_HW_PN5180_AUTOCOLL_MODE_AUTONOMOUS_WITHOUT_STANDBY)      /**< Mask value for RF technology. */

#define PHHAL_HW_PN5180_MAX_AUTOCOLL_RF_TECHNOLOGY                         PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_ISO18092_ACTIVE    /**< Maximum value for RF technology. */

#define PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK                        (PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A \
                                                                           | PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F \
                                                                           | PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_ISO18092_ACTIVE \
                                                                           | PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_F_ACTIVE)   /**< Mask value for RF technology. */
    /*@}*/

    /**
    * \name PN5180 EPC GEN2 custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_EpcGen2Inventory,
    * #phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize,
    * #phhalHw_Pn5180_Instr_EpcGen2RetrieveResult,
    * #phhalHw_Pn5180_Instr_EpcGen2ResumeInventory.
    */
    /*@{*/
#define PHHAL_HW_PN5180_MIN_SELECT_COMMAND_LENGTH                          1U    /**< Minimum length of Select command. */
#define PHHAL_HW_PN5180_MAX_SELECT_COMMAND_LENGTH                          39U   /**< Maximum length of Select command. */
#define PHHAL_HW_PN5180_BEGIN_COMMAND_LENGTH                               03U   /**< length of Begin command. */

#define PHHAL_HW_PN5180_MAX_NUM_OF_BITS_FOR_LAST_BYTE                      7U    /**< Maximum number of bits to be transmitted in the last byte of \p pSelectCommand. 0 in case all bits shall be transmitted, otherwise 1-7. */
#define PHHAL_HW_PN5180_MAX_EPC_GEN2_TIMESLOT                              (PHHAL_HW_PN5180_EPC_GEN2_TIMESLOT_SINGLE_SELECT_TAG) /**< Maximum EPC GEN2 TIMESLOT parameter. */
    /*@}*/

   /**
    * \name PN5180 RF configuration custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_LoadRfConfiguration,
    * #phhalHw_Pn5180_Instr_UpdateRfConfiguration,
    * #phhalHw_Pn5180_Instr_RetrieveRfConfigurationSize,
    * #phhalHw_Pn5180_Instr_RetrieveRfConfiguration.
    */
    /*@{*/
#define PHHAL_HW_PN5180_MIN_RF_TX_CONFIGURATION_INDEX                      0x00U  /**< Minimum RF configuration index for TX. */
#define PHHAL_HW_PN5180_MAX_RF_TX_CONFIGURATION_INDEX                      0x1CU  /**< Maximum RF configuration index for TX. */
#define PHHAL_HW_PN5180_MIN_RF_RX_CONFIGURATION_INDEX                      0x80U  /**< Minimum RF configuration index for RX. */
#define PHHAL_HW_PN5180_MAX_RF_RX_CONFIGURATION_INDEX                      0x9DU  /**< Maximum RF configuration index for RX. */
#define PHHAL_HW_PN5180_MAX_RF_CONFIGURATION_INDEX                         PHHAL_HW_PN5180_MAX_RF_RX_CONFIGURATION_INDEX   /**< Maximum RF configuration index. */
#define PHHAL_HW_PN5180_CURRENT_RF_CONFIGURATION_INDEX                     0xFFU  /**< Current RF configuration index for TX/RX. */
#define PHHAL_HW_PN5180_RF_CONFIGURATION_SIZE                              6U    /**< Size of a single RF configuration
                                                                                      1 Byte Rx or Tx configuration index
                                                                                      1 Byte Register address within the RF configuration
                                                                                      4 Bytes value to be written into the register. */
#define PHHAL_HW_PN5180_RETRIEVE_RF_EACH_CONFIGURATION                     5U    /**< Size of a single RF configuration
                                                                                      1 Byte Register address within the RF configuration
                                                                                      4 Bytes value of the register. */

#define PHHAL_HW_PN5180_RETRIEVE_RF_CONFIGURATION_SIZE_RESPONSE             1U
#define PHHAL_HW_PN5180_MAX_RF_CONFIGURATION_SIZE                          252U  /**< Maximum size of the RF configurations. It must be a multiple of #PHHAL_HW_PN5180_RF_CONFIGURATION_SIZE. */
#define PHHAL_HW_PN5180_MAX_RF_REGS                                        0x43U  /**< Max RF config regs- Not in Spec-Discussed with JP. */
#define PHHAL_HW_PN5180_UPDATE_RF_CONFIG_FIXED_CMD_BYTES                   2U

/*@}*/

   /**
   * \name PN5180 Load Configuration for Tx and Rx baudrates/reg values.
   * To be used with
   * #phhalHw_ApplyProtocolSettings.
   * #phhalHw_Pn5180_SetCardMode.
   */
    /*@{*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_106_MILLER                      0U      /**< Type A baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_212_MILLER                      1U      /**< Type A baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_424_MILLER                      2U      /**< Type A baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_848_MILLER                      3U      /**< Type A baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443B_106_NRZ                         4U      /**< Type B baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443B_212_NRZ                         5U      /**< Type B baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443B_424_NRZ                         6U      /**< Type B baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443B_848_NRZ                         7U      /**< Type B baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_FELICA_212                                8U      /**< Type F baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_FELICA_424                                9U      /**< Type F baud rate @ 484 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AI_106_106                            0x0AU   /**< Active Initiator baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AI_212_212                            0x0BU   /**< Active Initiator baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AI_424_424                            0x0CU   /**< Active Initiator baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO15693_26_1OF4_ASK100                   0x0DU   /**< Type V (ISO 15693 ) load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO15693_26_1OF4_ASK10                    0x0EU   /**< Type V (ISO 15693 ) load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO180003M3_TARI_18_88_ASK                0x0FU   /**< ISO 18000P3M3 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO180003M3_TARI_9_44_ASK                 0x10U   /**< ISO 18000P3M3 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_PICC_106_MANCH_SUBC             0x13U   /**< Type A Card mode baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_PICC_212_BPSK                   0x14U   /**< Type A Card mode baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_PICC_424_BPSK                   0x15U   /**< Type A Card mode baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_ISO14443A_PICC_848_BPSK                   0x16U   /**< Type A Card mode baud rate @ 848 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_PT_212                                0x17U   /**< Passive Target baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_PT_424                                0x18U   /**< Passive Target baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AT_106                                0x19U   /**< Active Target baud rate @ 106 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AT_212                                0x1AU   /**< Active Target baud rate @ 212 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_NFC_AT_424                                0x1BU   /**< Active Target baud rate @ 424 load configurations TX Value.*/
#define PHHAL_HW_PN5180_RF_TX_GTM_All_Al                                0x1CU   /**< General Target Mode configuration.*/


#define PHHAL_HW_PN5180_RF_RX_ISO14443A_106_MANCH_SUBC                  0x80U   /**< Type A baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_212_BPSK                        0x81U   /**< Type A baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_424_BPSK                        0x82U   /**< Type A baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_848_BPSK                        0x83U   /**< Type A baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443B_106_BPSK                        0x84U   /**< Type B baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443B_212_BPSK                        0x85U   /**< Type B baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443B_424_BPSK                        0x86U   /**< Type B baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443B_848_BPSK                        0x87U   /**< Type B baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_FELICA_212                                0x88U   /**< Type F baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_FELICA_424                                0x89U   /**< Type F baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_AI_106                                0x8AU   /**< Active Initiator @ baud rate 106 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_AI_212                                0x8BU   /**< Active Initiator @ baud rate 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_AI_424                                0x8CU   /**< Active Initiator @ baud rate 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO15693_26_1OF4_SC                       0x8DU   /**< Type V (ISO 15693 ) load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO15693_53_1OF4_SC                       0x8EU   /**< Type V (ISO 15693 ) load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO180003M3_MANCH424_4_PERIOD             0x8FU   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO180003M3_MANCH424_2_PERIOD             0x90U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO180003M3_MANCH848_4_PERIOD             0x91U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO180003M3_MANCH848_2_PERIOD             0x92U   /**< ISO 18000P3M3 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_PICC_106_MILLER                 0x93U   /**< Type A Card mode baud rate @ 106 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_PICC_212_MILLER                 0x94U   /**< Type A Card mode baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_PICC_424_MILLER                 0x95U   /**< Type A Card mode baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_ISO14443A_PICC_848_MILLER                 0x96U   /**< Type A Card mode baud rate @ 848 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_PT_212                                0x97U   /**< Passive Target baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_PT_424                                0x98U   /**< Passive Target baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_AT_106                                0x99U   /**< Active Initiator baud rate @ 106 load configurations RX Value. */
#define PHHAL_HW_PN5180_RF_RX_NFC_AT_212                                0x9AU   /**< Active Initiator baud rate @ 212 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_NFC_AT_424                                0x9BU   /**< Active Initiator baud rate @ 424 load configurations RX Value.*/
#define PHHAL_HW_PN5180_RF_RX_GTM_All_All                               0x9CU   /**< General Target Mode configuration.*/
    /*@}*/


  /**
    * \name PN5180 EEPROM custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_WriteE2Prom.
    */
    /*@{*/
#define PHHAL_HW_PN5180_WRITE_E2PROM_FIXED_CMD_BYTES                       3U
    /*@}*/

/**
    * \name PN5180 ConfigureTestBus custom definitions.
    * To be used with
    * #phhalHw_Pn5180_Instr_ConfigureTestBusDigital.
    */
    /*@{*/
#define PHHAL_HW_PN5180_CONFIGURE_TESTBUS_DIGITAL_FIXED_CMD_BYTES                       2U
#define PHHAL_HW_PN5180_CONFIGURE_TESTBUS_ANALOG_CMD_BYTES                              3U
    /*@}*/


    /**
* \name LPCD configuration
*/
/*@{*/

#define PHHAL_HW_CONFIG_SET_LPCD_WAKEUPTIME_MS  0x0070U     /**< Used value for wakeup counter in msecs, i.e. after this amount of time IC will wakes up from standby. */
#define PHHAL_HW_CONFIG_LPCD_MODE               0x0071U     /**< Used to set options  PHHAL_HW_PN5180_LPCD_MODE_DEFAULT or PHHAL_HW_PN5180_LPCD_MODE_POWERDOWN */
#define PHHAL_HW_CONFIG_LPCD_REF                0x0072U     /**< Used to set or get LPCD Ref */

/*@}*/

/**
* \name LPCD modes
*/
/*@{*/
#define PHHAL_HW_PN5180_LPCD_MODE_DEFAULT            0x00U  /**< Default mode. Try LPCD until timeout is reached. */
#define PHHAL_HW_PN5180_LPCD_MODE_POWERDOWN          0x01U  /**< Powers down the IC for a certain amount of time and performs LPC after wakeup.
                                                                 If no card is found the IC is powered down again and the procedure is restarted.
                                                                 If a card is found the function returns and the IC remains powered up. */
/*@}*/

/**
 * \brief Execute a PN5180 Instruction.
 *
 * This function can be used the execute any PN5180 instruction.
 * The instruction is composed of \p bInstructionCode and \p pInstrPayload.
 * If the instruction also results in a response (in case of GET_* instructions) a pointer to a buffer containing the response (\p ppInstrResponse)
 * is returned. The caller has to provide the length of the response using \p wExpectedResponseLength.
 * Note: GET_* instructions always deliver a response, also in error cases. It's the responsibility of the caller
 * to read out the status register to see if the response is valid (no exception occurred).
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Execution of instruction successful.
 */
phStatus_t phhalHw_Pn5180_Instr_Execute(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bInstructionCode,                       /**<[In] Instruction code to use. For instance \c #PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER */
                                    uint8_t * pInstrPayload,                        /**<[In] Payload to be used along with the instruction. */
                                    uint16_t wInstrPayloadLength,                   /**<[In] Length of the payload. */
                                    uint16_t wExpectedResponseLength,               /**<[In] Number of bytes to be received as a response to the instruction. Only valid for GET_* instructions. 0 in case of SET_* instructions. */
                                    uint8_t ** ppInstrResponse                      /**<[Out] Pointer to the received response. */
                                    );

/**
 * \brief Write a 32 bit value (little endian) to a logical register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_READ_WRITE_ERROR Register is read-only.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteRegister(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bRegister,                              /**<[In] Address of the register. */
                                    uint32_t dwValue                                /**<[In] 32 Bit register value which has to be written. */
                                    );

/**
 * \brief Perform a logical OR operation on a register using provided mask.
 *
 * This function is used to modify the content of register using a logical OR operation.
 * The content of the register is read and a logical OR operation is performed with the provided mask.
 * The modified content is written back to the register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_READ_WRITE_ERROR Register is read-only.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteRegisterOrMask(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bRegister,                              /**<[In] Address of the register. */
                                    uint32_t dwMask                                 /**<[In] Bitmask used as operand for logical OR operation. */
                                    );

/**
 * \brief Perform a logical AND operation on a register using provided mask.
 *
 * This function is used to modify the content of register using a logical AND operation.
 * The content of the register is read and a logical AND operation is performed with the provided mask.
 * The modified content is written back to the register.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully written.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address.
 * \retval #PH_ERR_READ_WRITE_ERROR Register is read-only.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteRegisterAndMask(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bRegister,                              /**<[In] Address of the register. */
                                    uint32_t dwMask                                 /**<[In] Bitmask used as operand for logical AND operation. */
                                    );

/**
 * \brief Perform a write register operation using a set of Register-Value pairs.
 *
 * This function behaves like #phhalHw_Pn5180_Instr_WriteRegister, #phhalHw_Pn5180_Instr_WriteRegisterOrMask and
 * #phhalHw_Pn5180_Instr_WriteRegisterAndMask with the possibility to combine them.
 * In fact it takes an array of register-type-value set and performs appropriate action.
 * The type reflects the action which is either write register, logical OR operation on a register or logical AND operation on a register.
 * The array is passed in as a buffer (\p pRegTypeValueSets) which contains one or more sets. One set consists of 1 byte register address,
 * 1 byte type (see \c #PHHAL_HW_PN5180_WRITE_MULTIPLE_TYPE_WRITE) and 4 bytes value/mask.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All registers successfully modified.
 * \retval #PH_ERR_INVALID_PARAMETER At least one unknown register address provided, or general parameter error (e.g. wSizeOfRegTypeValueSets is higher than 258)
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is read-only.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteRegisterMultiple(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t * pRegTypeValueSets,                    /**<[In] A buffer containing one or more sets Register-Type-Value sets.  */
                                    uint16_t wSizeOfRegTypeValueSets                /**<[In] Size of the buffer holding the register-type-value sets. Max. size supported is 258. */
                                    );

/**
 * \brief Read back content of a register.
 *
 * This function is used to read back the content of a logical register whose address is given by \p bRegister.
 * The content is available in output parameter \p pValue.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Register successfully read out.
 * \retval #PH_ERR_INVALID_PARAMETER Unknown register address provided or pValue set to NULL.
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is write-only.
 */
phStatus_t phhalHw_Pn5180_Instr_ReadRegister(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bRegister,                              /**<[In] Address of the register. */
                                    uint32_t * pValue                               /**<[Out] Pointer to a uint32_t variable, where the result (value of the read out register) shall be written to. */
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
 * \retval #PH_ERR_INVALID_PARAMETER At least one unknown register address provided, or general parameter error (e.g. \p bNumOfRegisters is higher than 16)
 * \retval #PH_ERR_READ_WRITE_ERROR At least one register is write-only.
 */
phStatus_t phhalHw_Pn5180_Instr_ReadRegisterMultiple(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t * pRegisters,                           /**<[In] A buffer containing list of registers (one byte each) to be read out. */
                                    uint8_t bNumOfRegisters,                        /**<[In] Number of registers to be read out (=size of \p pRegisters). Maximum size is 16. */
                                    uint8_t ** ppValues                             /**<[Out] A pointer to a buffer which contains the result, in fact the register values read out. Size of the buffer is 4 * \p bNumOfRegisters. */
                                    );

/**
 * \brief Write values sequentially to E2PROM.
 *
 * This function is used to write one or more values to E2PROM.
 * \p pDataToWrite shall contain the data to be written to E2PROM starting at the address \p bE2PromAddress.
 * The data is written in sequential order.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data written.
 * \retval #PH_ERR_INVALID_PARAMETER \p pDataToWrite or \p pDataParams is NULL.
 * \retval #PH_ERR_PARAMETER_OVERFLOW Write operation attempts to write beyond end of E2PROM.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteE2Prom(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bE2PromAddress,                         /**<[In] Start address within E2PROM. */
                                    uint8_t * pDataToWrite,                         /**<[In] Data to be written. */
                                    uint8_t bDataLength                             /**<[In] Number of bytes to be written. */
                                    );

/**
 * \brief Read values sequentially to E2PROM.
 *
 * This function is used to write one or more values to E2PROM.
 * \p pDataToWrite shall contain the data to be written to E2PROM starting at the address \p bE2PromAddress.
 * The data is written in sequential order.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data read.
 * \retval #PH_ERR_INVALID_PARAMETER \p pReadData or \p pDataParams is NULL.
 * \retval #PH_ERR_PARAMETER_OVERFLOW Read operation attempts to read beyond end of E2PROM.
 */
phStatus_t phhalHw_Pn5180_Instr_ReadE2Prom(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bE2PromAddress,                         /**<[In] Start address within E2PROM. */
                                    uint8_t * pReadData,                            /**<[Out] Read out data. */
                                    uint8_t bDataLength                             /**<[In] Number of bytes to be read. */
                                    );


/**
 * \brief Write Tx data to internal CLIF transmission buffer.
 *
 * This function is used to write data into the internal CLIF transmission buffer.
 * The size of this buffer is limited to 260 bytes.
 * After this instruction has been executed, an RF transmission can be started by setting corresponding registers.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data written to internal Tx buffer.
 * \retval #PH_ERR_INVALID_PARAMETER \p pTxBuffer or \p pDataParams is NULL, or \p wTxBufferLength > 260.
 */
phStatus_t phhalHw_Pn5180_Instr_WriteTxData(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t * pTxBuffer,                            /**<[In] Buffer containing the data to be used for RF transmission. */
                                    uint16_t wTxBufferLength                        /**<[In] Size of the buffer \p pTxBuffer. */
                                    );

/**
 * \brief Write Tx data to internal RF transmission buffer and start transmission using transceive command.
 *
 * This function is used to write data to internal CLIF transmission buffer
 * (see #phhalHw_Pn5180_Instr_WriteTxData) and, additionally,
 * start the RF transmission.
 * \p wTxBufferLength and \p bNumberOfValidBits indicate the exact data length to be transmitted.
 * For actual transmission, it is assumed that host has executed the transceiver command
 * (by setting corresponding register).
 * Note: When the instruction returns, transmission might still be ongoing,
 * i.e. the instruction just starts the transmission but does not wait for end of transmission.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS All data written to internal Tx buffer and transmission started.
 * \retval #PH_ERR_INVALID_PARAMETER \p pTxBuffer or \p pDataParams is NULL, \p bTxBufferLength > 260 or \p bNumberOfValidBits > 7.
 */
phStatus_t phhalHw_Pn5180_Instr_SendData(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t * pTxBuffer,                            /**<[In] Buffer containing the data to be used for RF transmission. */
                                    uint16_t wTxBufferLength,                       /**<[In] Size of the buffer \p pTxBuffer. */
                                    uint8_t bNumberOfValidBits                      /**<[In] Number of valid bits in last byte. 0 means all bits are transferred */
                                    );

/**
 * \brief Read data back from internal CLIF Rx buffer.
 *
 * This function is used to retrieve data from internal CLIF receive buffer after
 * successful reception.
 * The size of this buffer is limited to 260 bytes.
 * Note: This instruction does not check if reception was successful.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Data successfully read.
 * \retval #PH_ERR_INVALID_PARAMETER \p wExpectedRxDataLength is > 260.
 */
phStatus_t phhalHw_Pn5180_Instr_RetrieveRxData(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t ** pRxBuffer,                           /**<[In] Pointer to the buffer holding the data. */
                                    uint16_t wExpectedRxDataLength                  /**<[In] Number of byte to be retrieved. */
                                    );


/**
 * \brief Switch to standby mode.
 *
 * This function is used to put PN5180 into standby mode. The wake up conditions are defined using
 * \p bWakeupControlMask parameter. The instruction does not check if system actually entered standby
 * or standby was prevented. An interrupt is raised once PN5180 wakes up or in case standby was prevented.
 * Optionally, a GPO is set on wake up.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER No wake up condition defined or \p wWakeupCounterInMs is out of range.
 */
phStatus_t phhalHw_Pn5180_Instr_SwitchModeStandby(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bWakeupControlMask,                     /**<[In] Bitmask controlling the standby wakeup. For example see: #PHHAL_HW_PN5180_WAKEUP_CONTROL_MASK_WUP_ON_COUNTER.*/
                                    uint16_t wWakeupCounterInMs                     /**<[In] Used value for wakeup counter in msecs, i.e. after this amount of time PN5180 wakes up from standby. Maximum value is 2690. */
                                    );
/**
 * \brief Switch to LPCD mode.
 *
 * This function is used to put PN5180 into LPCD mode. The wait period in msecs between two LPCD cycles
 * is defined by \p wWakeupCounterValue. During wait period PN5180 is in standby.
 * The function returns immediately, i.e. it does not wait until a card has been detected. On detection of a card,
 * PN5180 raises an interrupt and normal mode is entered again.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER \p wWakeupCounterInMs is out of range.
 */
phStatus_t phhalHw_Pn5180_Instr_SwitchModeLpcd(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint16_t wWakeupCounterInMs                     /**<[In] Used value for wakeup counter in msecs, i.e. after this amount of time PN5180 wakes up from standby. Maximum value is 2690. */
                                    );

/**
 * \brief Switch to Autocoll mode.
 *
 * This function is used to put PN5180 into Autocoll mode in order to handle target mode activation.
 * The termination condition depends on \p bAutocollMode.
 * The function returns immediately, i.e. it does not wait until autocoll terminates. Once terminated, autocoll raises
 * an interrupt and normal mode is entered again.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 * \retval #PH_ERR_INVALID_PARAMETER \p wWakeupCounterInMs is 0.
 */
phStatus_t phhalHw_Pn5180_Instr_SwitchModeAutocoll(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DataParams to this layer. */
                                    uint8_t bRfTechnologyMask,                      /**<[In] Bitmask indicating the RF technology to listen for during Autocoll. For example see #PHHAL_HW_PN5180_AUTOCOLL_RF_TECHNOLOGY_MASK_NFC_A. */
                                    uint8_t bAutocollMode                           /**<[In] Autocoll operation mode to be used. See for example #PHHAL_HW_PN5180_AUTOCOLL_MODE_TERMINATE_ON_RF_OFF. */
                                    );

/**
 * \brief Perform authentication on an activated MIFARE Classic contactless IC.
 *
 * This function is used to perform a MIFARE Classic Authentication on an activated card.
 * It takes the key, card UID and the key type to authenticate at given block address.
 *
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Authentication successful.
 * \retval #PH_ERR_INVALID_PARAMETER \p bKeyType wrong, \p pUid or \p pKey is NULL.
 * \retval #PH_ERR_IO_TIMEOUT Timeout during authentication.
 * \retval #PH_ERR_AUTH_ERROR Authentication failed.
 */
phStatus_t phhalHw_Pn5180_Instr_MifareAuthenticate(
                                    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams to this layer. */
                                    uint8_t * pKey,                           /**<[In] Key to be used for authentication. uint8_t[6]. */
                                    uint8_t bKeyType,                         /**<[In] Key type. Either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
                                    uint8_t bBlockNo,                         /**<[In] Block-Number on card to authenticate to. */
                                    uint8_t * pUid                            /**<[In] UID of the activated card. uint8_t[4]. */
                                    );


/**
 * \brief Perform an ISO 18000-3M3 Inventory.
 *
 * This function is used to perform an inventory of ISO18000-3M3 tags.
 * It implements an autonomous execution of several commands according to ISO18000-3M3
 * in order to guarantee the timings specified by that standard.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Inventory executed, internal response buffer filled according to the \p bTimeslotProcessingBehavior policy.
 * \retval #PH_ERR_INVALID_PARAMETER \p pBeginRoundCommand is NULL or wrong value for one of \p bTimeslotProcessingBehavior, \p bSelectCommandBitsInLastByte or \p bSelectCommandLength.
 */
phStatus_t phhalHw_Pn5180_Instr_EpcGen2Inventory(
                                    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams to this layer. */
                                    uint8_t * pSelectCommand,                 /**<[In] If present (not NULL) it contains the Select command (according to ISO18000-3, Table 47) which is sent prior to BeginRound command. CRC-16c shall not be included. Size of the buffer is between 1-39 bytes, inclusive. */
                                    uint8_t bSelectCommandLength,             /**<[In] Size of \p pSelectCommand buffer. 0 in case no Select Command shall be sent, otherwise 1-39. */
                                    uint8_t bSelectCommandBitsInLastByte,     /**<[In] Number of bits to be transmitted in the last byte of \p pSelectCommand. 0 in case all bits shall be transmitted, otherwise 1-7. */
                                    uint8_t * pBeginRoundCmmand,              /**<[In] Buffer containing BeginRound command (according to ISO18000-3, Table 49). CRC-5 shall not be included. uint8_t[3]. */
                                    uint8_t bTimeslotProcessingBehavior       /**<[In] Timeslot processing behavior. Indicates how many timeslots shall be processed before returning response to the host. For example see: PHHAL_HW_PN5180_EPC_GEN2_TIMESLOT_FILL_BUFFER. */
                                    );

/**
 * \brief Retrieve the size of a pending EPC GEN2 inventory result.
 *
 * This function is used to retrieve the size of the inventory result.
 * The size reflects the payload size of the response to the next execution #phhalHw_Pn5180_Instr_EpcGen2RetrieveResult.
 * If the size is 0, then no more results are available which means inventory algorithm has finished.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed, result size available in \p pResultSize.
 * \retval #PH_ERR_INVALID_PARAMETER \p wResultSize is NULL.
 */
phStatus_t phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize(
                                    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams to this layer. */
                                    uint16_t * pResultSize                    /**<[Out] Size of the inventory result. */
                                    );

/**
 * \brief Return the result of inventory.
 *
 * This function is used to retrieve the result of a preceding call to #phhalHw_Pn5180_Instr_EpcGen2Inventory or #phhalHw_Pn5180_Instr_EpcGen2ResumeInventory instruction
 * The size of the response is determined by \p pResultSize of #phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize.
 * The result (available in \p ppInventoryResult) actually contains the result for one or more processed timeslots.
 *
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed, result size available in \p wResultSize.
 * \retval #PH_ERR_INVALID_PARAMETER \p wResultSize is NULL.
 */
phStatus_t phhalHw_Pn5180_Instr_EpcGen2RetrieveResult(
                                    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams to this layer. */
                                    uint8_t ** ppInventoryResult,             /**<[Out] Pointer to a buffer containing the result of the inventory. The buffer may contain the result of one or more
                                                                                        timeslots. Each timeslot result has the following structure:
                                                                                        1 Byte Timeslot Status (\n
                                                                                            0 - Tag response available. "Tag Reply Length" field, "Valid bits in last byte" field, and "Tag reply" field present.\n
                                                                                            1 - Tag response available. "Tag Reply Length" field, "Valid bits in last byte" field, "Tag reply" field and "Tag handle" field present.\n
                                                                                            2 - No tag replied in timeslot. "Tag Reply Length" field, "Valid bits in last byte" field, and "Tag reply" field shall not be present.\n
                                                                                            3 - Two or more tags responded in the timeslot. (Collision). "Tag Reply Length" field, "Valid bits in last byte" field, and "Tag reply" field shall not be present.)\n
                                                                                        1 Byte 'Tag Reply' Length (1-66)
                                                                                        1 Byte Valid Bits in last Byte (0-7, 0 means all bits are valid)
                                                                                        0-66 Bytes Tag Reply
                                                                                        0 or 2 Bytes Handle of the tag, in case field "Timeslot Status" is set to "1". Otherwise field not present. */
                                    uint16_t wResultSize                      /**<[In] Size of the inventory result. #phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize has to be called to retrieve the size. */
                                    );

/**
 * \brief Resume an ISO 18000-3M3 Inventory.
 *
 * This function is used to resume the inventory algorithm in case it is paused.
 * More specifically, this instruction has to be repeatedly called,
 * as long as \p pResultSize of #phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize is greater than 0.
 * Typical sequence for a complete EPC GEN2 inventory is
 * 1.) #phhalHw_Pn5180_Instr_EpcGen2Inventory
 * 2.) #phhalHw_Pn5180_Instr_EpcGen2RetrieveResultSize
 * 3.) if \p pResultSize is 0, inventory has finished.
 * 4.) #phhalHw_Pn5180_Instr_EpcGen2RetrieveResult
 * 5.) #phhalHw_Pn5180_Instr_EpcGen2ResumeInventory
 * 6.) Go to step 2.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Inventory resumed, internal response buffer filled according to the \p bTimeslotProcessingBehavior policy set during execution of #phhalHw_Pn5180_Instr_EpcGen2Inventory.
 */
phStatus_t phhalHw_Pn5180_Instr_EpcGen2ResumeInventory(
                                    phhalHw_Pn5180_DataParams_t * pDataParams /**<[In] DataParams to this layer. */
                                    );


/**
 * \brief Load RF Tx and Rx settings from E2PROM.
 *
 * This function is used to load the RF configuration from E2PROM into internal CLIF registers.
 * RF configuration refers to a unique combination of RF Technology, mode (target/initiator) and baud rate.
 * RF configuration can be loaded separately for the CLIF receiver (Rx configuration)
 * and transmitter (Tx configuration) path.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully loaded.
 * \retval #PH_ERR_INVALID_PARAMETER \p bTxConfiguration or \p bRxConfiguration out of range.
 */
phStatus_t phhalHw_Pn5180_Instr_LoadRfConfiguration(
                                    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DataParams to this layer. */
                                    uint8_t bRfTxConfiguration,               /**<[In] Tx configuration index to be used. 0xff means Tx Configuration is not modified. */
                                    uint8_t bRfRxConfiguration                /**<[In] Rx configuration index to be used. 0xff means Rx Configuration is not modified. */
                                    );

/**
 * \brief Update the RF configuration
 *
 * This function is used to update the RF configuration within E2PROM.
 * Only registers available within the respective RF configuration can be updated, registers can't be added or removed.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully updated.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfConfigurationSize not a multiple of 6 or out of range. \p pRfConfiguration is NULL.
 */
phStatus_t phhalHw_Pn5180_Instr_UpdateRfConfiguration(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DataParams to this layer. */
                                    uint8_t * pRfConfiguration,                /**<[In] Array containing one or more configurations to be written. One configuration is a set of
                                                                                       1 Byte Rx or Tx configuration index
                                                                                       1 Byte Register address within the RF configuration
                                                                                       4 Bytes value to be written into the register. */
                                    uint8_t  bRfConfigurationSize              /**[In] Size of \p pRfConfiguration. Must be a multiple of 6, maximum value is 252. */
                                    );

/**
 * \brief Retrieve the number of register-value pairs of the RF configuration.
 *
 * This function is used to retrieve the size (number of registers) of a given RF configuration.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration size retrieved.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfConfiguration out of range or \p pNumOfRegisters is NULL.
 */
phStatus_t phhalHw_Pn5180_Instr_RetrieveRfConfigurationSize(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DataParams to this layer. */
                                    uint8_t bRfConfiguration,                  /**<[In] Rf Tx or Rx configuration index to be used. */
                                    uint8_t * pNumOfRegisters                  /**<[Out] Number of registers (register-value pairs) within given Rf configuration. */
                                    );


/**
 * \brief Retrieve the RF configuration.
 *
 * This function is used to read out an RF configuration.
 * The register address-value-pairs are available in \p ppRfConfiguration.
 * In order to know how many pairs are to be expected, first #phhalHw_Pn5180_Instr_RetrieveRfConfigurationSize has to be executed.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF configuration successfully retrieved.
 * \retval #PH_ERR_INVALID_PARAMETER \p bRfConfiguration out of range.
 */
phStatus_t phhalHw_Pn5180_Instr_RetrieveRfConfiguration(
                                    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DataParams to this layer. */
                                    uint8_t bRfConfiguration,                  /**<[In] Rf Tx or Rx configuration index to be used. */
                                    uint8_t ** ppRfConfiguration,              /**<[Out] Pointer to a buffer containing \p bNumOfRegisters configurations. One configuration is a set of
                                                                                       1 Byte Register address within the RF configuration
                                                                                       4 Bytes value to be written into the register. */
                                    uint8_t   bNumOfRegisters                  /**<[In] Number of registers (register-value pairs) within given Rf configuration. To be retrieved upfront using #phhalHw_Pn5180_Instr_RetrieveRfConfigurationSize. */
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
 */
phStatus_t phhalHw_Pn5180_Instr_RfOn(
    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DataParams to this layer. */
    uint8_t bRfOnConfig          /**<[In] The ON Configuration. */
    );


/**
 * \brief Turns the RF field OFF.
 *
 * This function is used to turn the RF field OFF.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS RF Field turn OFF sent to IC.
 * \retval #PH_ERR_INVALID_PARAMETER \p PData params is null.
 */
phStatus_t phhalHw_Pn5180_Instr_RfOff(
    phhalHw_Pn5180_DataParams_t * pDataParams      /**<[In] DataParams to this layer. */
    );

/**
 * \brief Configures the Digital Test BUS.
 *
 * This function is used to configure the Digital Test Bus.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS If the TestBus is configured successfully.
 * \retval #PH_ERR_INVALID_PARAMETER \p PData params is null.
 */
phStatus_t phhalHw_Pn5180_Instr_ConfigureTestBusDigital(
    phhalHw_Pn5180_DataParams_t * pDataParams,      /**<[In] DataParams to this layer. */
    uint8_t bTBSelect,                              /**<[In] This value is written to the CLIF_TBCONTROL_REG. */
    uint8_t * pTBpos,                               /**<[In] 0xXY (X - Pad Location, Y - Testbus Bit position). */
    uint8_t bTBposSize                              /**<[In] pTBpos size 0 to 6 bytes. */
    );


/**
 * \brief Configures the Analog Test BUS.
 *
 * This function is used to configure the Digital Test Bus.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS If the TestBus is configured successfully.
 * \retval #PH_ERR_INVALID_PARAMETER \p PData params is null.
 */

phStatus_t phhalHw_Pn5180_Instr_ConfigureTestBusAnalog(
    phhalHw_Pn5180_DataParams_t * pDataParams,      /**<[In] DataParams to this layer. */
    uint8_t bDAC1Config,                            /**<[In] define dac1 sources . */
    uint8_t bDAC0Config                             /**<[In] define dac0 sources . */
    );


/**
 * \brief Returns the current version of HAL only for I&V reporting purposes.
 *
 *  This function returns the current version of HAL only for I&V reporting purposes.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS when version number is returned.
  */
phStatus_t phhalHw_Pn5180_Instr_GetHALVersion(
    uint16_t * pValue                           /**<[Out] Version information. */
    );


/**
 * \brief Set config for LFCD which is IC specific.
 *
 *  This function can be used to set configuration parametrs of LPCD.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS when successful.
  */
phStatus_t phhalHw_Pn5180_Int_LPCD_SetConfig(
    phhalHw_Pn5180_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    );


/**
 * \brief Get config for LFCD which is IC specific.
 *
 *  This function can be used to get configuration parameters of LPCD.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS when successful.
  */
phStatus_t phhalHw_Pn5180_Int_LPCD_GetConfig(
    phhalHw_Pn5180_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
    );


/**
 * \brief Switch to Normal mode.
 *
 * This function is used to put PN5180 into Normal mode.
 *
 * \return Status Code
 * \retval #PH_ERR_SUCCESS Instruction successfully executed.
 */
phStatus_t phhalHw_Pn5180_Instr_SwitchModeNormal(phhalHw_Pn5180_DataParams_t * pDataParams);

/** @}
* end of phhalHw_PN5180_Instr group
*/

#endif /* #if defined( NXPBUILD__PHHAL_HW_PN5180 ) */

#endif  /* PHHALHW_PN5180_INSTR_H */
