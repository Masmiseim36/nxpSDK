/*
*         Copyright (c), NXP Semiconductors
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
* PN5180 Register map.
** $Author:nxp62726 $
* $Revision$ (v06.01.00)
* $Date$
*/
#ifndef PHHALHW_PN5180_REG_H
#define PHHALHW_PN5180_REG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define MASK_TX_LASTBITS                                                                                 0x07U    /**< The bits that are valid for TX_LATS_BITS field */
#define MASK_RX_LASTBITS                                                                                 0x07U    /**< The bits that are valid for RX_LATS_BITS field */
#define MASK_RXWAIT                                                                                      0xFFFFFU/**< Bitmask for RxWait bits. */

#define SYSTEM_CONFIG                                                                                    0x00U
#define SYSTEM_CONFIG_ACTIVE_MODE_TX_RF_ENABLE_POS                                                       (10U)
#define SYSTEM_CONFIG_ACTIVE_MODE_TX_RF_ENABLE_MASK                                                      (0x00000400U)  /**< rw-Set to 1; turning on the driver in reader or active mode is requested. Note: According to the setting of InitialRFOn; AutoRFOn and CAOn the driver is turned after a defined time and depended on the presence of an external RF field. Note: In case of an RFActiveError this bit is cleared by HW */
#define SYSTEM_CONFIG_AUTOCOLL_STATE_A_POS                                                               (9U)
#define SYSTEM_CONFIG_AUTOCOLL_STATE_A_MASK                                                              (0x00000200U)  /**< rw-*/
#define SYSTEM_CONFIG_SOFT_RESET_POS                                                                     (8U)
#define SYSTEM_CONFIG_SOFT_RESET_MASK                                                                    (0x00000100U)  /**< w--*/
#define SYSTEM_CONFIG_RESERVED_POS                                                                       (7U)
#define SYSTEM_CONFIG_RESERVED_MASK                                                                      (0x00000080U)  /**< r--*/
#define SYSTEM_CONFIG_MFC_CRYPTO_ON_POS                                                                  (6U)
#define SYSTEM_CONFIG_MFC_CRYPTO_ON_MASK                                                                 (0x00000040U)  /**< rw-If set to 1, the mfc-crypto bit is generated for end-/de-cryption*/
#define SYSTEM_CONFIG_PRBS_TYPE_POS                                                                      (5U)
#define SYSTEM_CONFIG_PRBS_TYPE_MASK                                                                     (0x00000020U)  /**< rw-Defines the PRBS type; If set to 1, PRBS15 is selected, default value 0 selects PRBS9*/
#define SYSTEM_CONFIG_RESERVED1_POS                                                                       (4U)
#define SYSTEM_CONFIG_RESERVED1_MASK                                                                      (0x00000010U)  /**< r--*/
#define SYSTEM_CONFIG_START_SEND_POS                                                                     (3U)
#define SYSTEM_CONFIG_START_SEND_MASK                                                                    (0x00000008U)  /**< rw-Set to logic 1; the data transmission is started. Note: This bit is only valid in combination with the Transceive command Note: If TxWait is set to a value other than zero the TxWait period configured must be expired as well that the transmission starts Note: As soon as the transmission started this bit is cleared by HW. */
#define SYSTEM_CONFIG_COMMAND_POS                                                                        (0U)
#define SYSTEM_CONFIG_COMMAND_MASK                                                                       (0x00000007U)  /**< rw-This registers hold the command bits 0* IDLE/StopCom Command; stops all ongoing communication and set the CLIF to IDLE mode; reset value 1 Transmit command; starts a transmission immediately 2 Receive command; enables the receiver. After end of reception the bits are clear and IDLE 3 Transceive command; initiates a transceive cycle. Note: Depending on the value of the Initiator bit a transmission is started or the receiver is enabled Note: The transceive command does not finish automatically. It stays in the transceive cycle until stopped via the IDLE/StopCom command 4 KeepCommand command; This command does not change the content of the command register and might be used in case other bits in the register are to be changed 5 LoopBack command; This command is for test purposes only. It starts a transmission and at the same enables the receiver. 6 - 7 Reserved: Do not use this settings  1*/

#define IRQ_ENABLE                                                                                        0x01U
#define IRQ_ENABLE_TEMPSENS_ERROR_IRQ_ENABLE_POS                                                         (16U)
#define IRQ_ENABLE_TEMPSENS_ERROR_IRQ_ENABLE_MASK                                                        (0x00010000U)  /**< -x-*/
#define IRQ_ENABLE_RX_SC_DET_IRQ_SET_ENABLE_POS                                                          (15U)
#define IRQ_ENABLE_RX_SC_DET_IRQ_SET_ENABLE_MASK                                                         (0x00008000U)  /**< -x-*/
#define IRQ_ENABLE_RX_SOF_DET_IRQ_SET_ENABLE_POS                                                         (14U)
#define IRQ_ENABLE_RX_SOF_DET_IRQ_SET_ENABLE_MASK                                                        (0x00004000U)  /**< -x-*/
#define IRQ_ENABLE_TIMER2_IRQ_SET_ENABLE_POS                                                             (13U)
#define IRQ_ENABLE_TIMER2_IRQ_SET_ENABLE_MASK                                                            (0x00002000U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_TIMER1_IRQ_SET_ENABLE_POS                                                             (12U)
#define IRQ_ENABLE_TIMER1_IRQ_SET_ENABLE_MASK                                                            (0x00001000U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_TIMER0_IRQ_SET_ENABLE_POS                                                             (11U)
#define IRQ_ENABLE_TIMER0_IRQ_SET_ENABLE_MASK                                                            (0x00000800U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_RF_ACTIVE_ERROR_IRQ_SET_ENABLE_POS                                                    (10U)
#define IRQ_ENABLE_RF_ACTIVE_ERROR_IRQ_SET_ENABLE_MASK                                                   (0x00000400U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_TX_RFON_IRQ_SET_ENABLE_POS                                                            (9U)
#define IRQ_ENABLE_TX_RFON_IRQ_SET_ENABLE_MASK                                                           (0x00000200U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_TX_RFOFF_IRQ_SET_ENABLE_POS                                                           (8U)
#define IRQ_ENABLE_TX_RFOFF_IRQ_SET_ENABLE_MASK                                                          (0x00000100U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_RFON_DET_IRQ_SET_ENABLE_POS                                                           (7U)
#define IRQ_ENABLE_RFON_DET_IRQ_SET_ENABLE_MASK                                                          (0x00000080U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_RFOFF_DET_IRQ_SET_ENABLE_POS                                                          (6U)
#define IRQ_ENABLE_RFOFF_DET_IRQ_SET_ENABLE_MASK                                                         (0x00000040U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_STATE_CHANGE_IRQ_SET_ENABLE_POS                                                       (5U)
#define IRQ_ENABLE_STATE_CHANGE_IRQ_SET_ENABLE_MASK                                                      (0x00000020U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_CARD_ACTIVATED_IRQ_SET_ENABLE_POS                                                     (4U)
#define IRQ_ENABLE_CARD_ACTIVATED_IRQ_SET_ENABLE_MASK                                                    (0x00000010U)  /**< -x-*/
#define IRQ_ENABLE_MODE_DETECTED_IRQ_SET_ENABLE_POS                                                      (3U)
#define IRQ_ENABLE_MODE_DETECTED_IRQ_SET_ENABLE_MASK                                                     (0x00000008U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_IDLE_IRQ_SET_ENABLE_POS                                                               (2U)
#define IRQ_ENABLE_IDLE_IRQ_SET_ENABLE_MASK                                                              (0x00000004U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_TX_IRQ_SET_ENABLE_POS                                                                 (1U)
#define IRQ_ENABLE_TX_IRQ_SET_ENABLE_MASK                                                                (0x00000002U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag */
#define IRQ_ENABLE_RX_IRQ_SET_ENABLE_POS                                                                 (0U)
#define IRQ_ENABLE_RX_IRQ_SET_ENABLE_MASK                                                                (0x00000001U)  /**< -x-Writing 1 to this register does set the corresponding IRQ ENABLE flag*/

#define IRQ_STATUS                                                                                        0x02U
#define IRQ_STATUS_LPCD_IRQ_POS                                                                          (19U)
#define IRQ_STATUS_LPCD_IRQ_MASK                                                                         (0x00080000U)  /**< r--*/
#define IRQ_STATUS_HV_ERROR_IRQ_POS                                                                      (18U)
#define IRQ_STATUS_HV_ERROR_IRQ_MASK                                                                     (0x00040000U)  /**< r--*/
#define IRQ_STATUS_GENERAL_ERROR_IRQ_POS                                                                 (17U)
#define IRQ_STATUS_GENERAL_ERROR_IRQ_MASK                                                                (0x00020000U)  /**< r--General ERROR IRQ*/
#define IRQ_STATUS_TEMPSENS_ERROR_IRQ_STATUS_POS                                                         (16U)
#define IRQ_STATUS_TEMPSENS_ERROR_IRQ_STATUS_MASK                                                        (0x00010000U)  /**< r--*/
#define IRQ_STATUS_RX_SC_DET_IRQ_POS                                                                     (15U)
#define IRQ_STATUS_RX_SC_DET_IRQ_MASK                                                                    (0x00008000U)  /**< r--*/
#define IRQ_STATUS_RX_SOF_DET_IRQ_POS                                                                    (14U)
#define IRQ_STATUS_RX_SOF_DET_IRQ_MASK                                                                   (0x00004000U)  /**< r--*/
#define IRQ_STATUS_TIMER2_IRQ_POS                                                                        (13U)
#define IRQ_STATUS_TIMER2_IRQ_MASK                                                                       (0x00002000U)  /**< r--*/
#define IRQ_STATUS_TIMER1_IRQ_POS                                                                        (12U)
#define IRQ_STATUS_TIMER1_IRQ_MASK                                                                       (0x00001000U)  /**< r--*/
#define IRQ_STATUS_TIMER0_IRQ_POS                                                                        (11U)
#define IRQ_STATUS_TIMER0_IRQ_MASK                                                                       (0x00000800U)  /**< r--*/
#define IRQ_STATUS_RF_ACTIVE_ERROR_IRQ_POS                                                               (10U)
#define IRQ_STATUS_RF_ACTIVE_ERROR_IRQ_MASK                                                              (0x00000400U)  /**< r--*/
#define IRQ_STATUS_TX_RFON_IRQ_POS                                                                       (9U)
#define IRQ_STATUS_TX_RFON_IRQ_MASK                                                                      (0x00000200U)  /**< r--*/
#define IRQ_STATUS_TX_RFOFF_IRQ_POS                                                                      (8U)
#define IRQ_STATUS_TX_RFOFF_IRQ_MASK                                                                     (0x00000100U)  /**< r--*/
#define IRQ_STATUS_RFON_DET_IRQ_POS                                                                      (7U)
#define IRQ_STATUS_RFON_DET_IRQ_MASK                                                                     (0x00000080U)  /**< r--*/
#define IRQ_STATUS_RFOFF_DET_IRQ_POS                                                                     (6U)
#define IRQ_STATUS_RFOFF_DET_IRQ_MASK                                                                    (0x00000040U)  /**< r--*/
#define IRQ_STATUS_STATE_CHANGE_IRQ_POS                                                                  (5U)
#define IRQ_STATUS_STATE_CHANGE_IRQ_MASK                                                                 (0x00000020U)  /**< r--*/
#define IRQ_STATUS_CARD_ACTIVATED_IRQ_POS                                                                (4U)
#define IRQ_STATUS_CARD_ACTIVATED_IRQ_MASK                                                               (0x00000010U)  /**< r--*/
#define IRQ_STATUS_MODE_DETECTED_IRQ_POS                                                                 (3U)
#define IRQ_STATUS_MODE_DETECTED_IRQ_MASK                                                                (0x00000008U)  /**< r--Set to 1 by HW; when the card mode has been detected by the ModeDetector Note: While the TypeA activation FSM is active no IRQ is issued any more */
#define IRQ_STATUS_IDLE_IRQ_POS                                                                          (2U)
#define IRQ_STATUS_IDLE_IRQ_MASK                                                                         (0x00000004U)  /**< r--Set to 1 by HW; when the IDLE state is entered */
#define IRQ_STATUS_TX_IRQ_POS                                                                            (1U)
#define IRQ_STATUS_TX_IRQ_MASK                                                                           (0x00000002U)  /**< r--Set to 1 by HW; when an ongoing transmission is finished. */
#define IRQ_STATUS_RX_IRQ_POS                                                                            (0U)
#define IRQ_STATUS_RX_IRQ_MASK                                                                           (0x00000001U)  /**< r--Set to 1 by HW, when an ongoing reception is finished*/

#define IRQ_SET_CLEAR                                                                                     0x03U
#define IRQ_SET_CLEAR_LPCD_IRQ_CLR_POS                                                                   (19U)
#define IRQ_SET_CLEAR_LPCD_IRQ_CLR_MASK                                                                  (0x00080000U)  /**< w--*/
#define IRQ_SET_CLEAR_HV_ERROR_IRQ_CLR_POS                                                               (18U)
#define IRQ_SET_CLEAR_HV_ERROR_IRQ_CLR_MASK                                                              (0x00040000U)  /**< w--*/
#define IRQ_SET_CLEAR_GENERAL_ERROR_IRQ_CLR_POS                                                          (17U)
#define IRQ_SET_CLEAR_GENERAL_ERROR_IRQ_CLR_MASK                                                         (0x00020000U)  /**< w--*/
#define IRQ_SET_CLEAR_TEMPSENS_ERROR_IRQ_CLR_POS                                                         (16U)
#define IRQ_SET_CLEAR_TEMPSENS_ERROR_IRQ_CLR_MASK                                                        (0x00010000U)  /**< w--*/
#define IRQ_SET_CLEAR_RX_SC_DET_IRQ_CLR_POS                                                              (15U)
#define IRQ_SET_CLEAR_RX_SC_DET_IRQ_CLR_MASK                                                             (0x00008000U)  /**< w--*/
#define IRQ_SET_CLEAR_RX_SOF_DET_IRQ_CLR_POS                                                             (14U)
#define IRQ_SET_CLEAR_RX_SOF_DET_IRQ_CLR_MASK                                                            (0x00004000U)  /**< w--*/
#define IRQ_SET_CLEAR_TIMER2_IRQ_CLR_POS                                                                 (13U)
#define IRQ_SET_CLEAR_TIMER2_IRQ_CLR_MASK                                                                (0x00002000U)  /**< w--*/
#define IRQ_SET_CLEAR_TIMER1_IRQ_CLR_POS                                                                 (12U)
#define IRQ_SET_CLEAR_TIMER1_IRQ_CLR_MASK                                                                (0x00001000U)  /**< w--*/
#define IRQ_SET_CLEAR_TIMER0_IRQ_CLR_POS                                                                 (11U)
#define IRQ_SET_CLEAR_TIMER0_IRQ_CLR_MASK                                                                (0x00000800U)  /**< w--*/
#define IRQ_SET_CLEAR_RF_ACTIVE_ERROR_IRQ_CLR_POS                                                        (10U)
#define IRQ_SET_CLEAR_RF_ACTIVE_ERROR_IRQ_CLR_MASK                                                       (0x00000400U)  /**< w--*/
#define IRQ_SET_CLEAR_TX_RFON_IRQ_CLR_POS                                                                (9U)
#define IRQ_SET_CLEAR_TX_RFON_IRQ_CLR_MASK                                                               (0x00000200U)  /**< w--*/
#define IRQ_SET_CLEAR_TX_RFOFF_IRQ_CLR_POS                                                               (8U)
#define IRQ_SET_CLEAR_TX_RFOFF_IRQ_CLR_MASK                                                              (0x00000100U)  /**< w--*/
#define IRQ_SET_CLEAR_RFON_DET_IRQ_CLR_POS                                                               (7U)
#define IRQ_SET_CLEAR_RFON_DET_IRQ_CLR_MASK                                                              (0x00000080U)  /**< w--*/
#define IRQ_SET_CLEAR_RFOFF_DET_IRQ_CLR_POS                                                              (6U)
#define IRQ_SET_CLEAR_RFOFF_DET_IRQ_CLR_MASK                                                             (0x00000040U)  /**< w--*/
#define IRQ_SET_CLEAR_STATE_CHANGE_IRQ_CLR_POS                                                           (5U)
#define IRQ_SET_CLEAR_STATE_CHANGE_IRQ_CLR_MASK                                                          (0x00000020U)  /**< w--*/
#define IRQ_SET_CLEAR_CARD_ACTIVATED_IRQ_CLR_POS                                                         (4U)
#define IRQ_SET_CLEAR_CARD_ACTIVATED_IRQ_CLR_MASK                                                        (0x00000010U)  /**< w--*/
#define IRQ_SET_CLEAR_MODE_DETECTED_IRQ_CLR_POS                                                          (3U)
#define IRQ_SET_CLEAR_MODE_DETECTED_IRQ_CLR_MASK                                                         (0x00000008U)  /**< w--*/
#define IRQ_SET_CLEAR_IDLE_IRQ_CLR_POS                                                                   (2U)
#define IRQ_SET_CLEAR_IDLE_IRQ_CLR_MASK                                                                  (0x00000004U)  /**< w--*/
#define IRQ_SET_CLEAR_TX_IRQ_CLR_POS                                                                     (1U)
#define IRQ_SET_CLEAR_TX_IRQ_CLR_MASK                                                                    (0x00000002U)  /**< w--*/
#define IRQ_SET_CLEAR_RX_IRQ_CLR_POS                                                                     (0U)
#define IRQ_SET_CLEAR_RX_IRQ_CLR_MASK                                                                    (0x00000001U)  /**< w--*/

#define TRANSCEIVER_CONFIG                                                                                0x04U
#define TRANSCEIVER_CONFIG_STATE_TRIGGER_SELECT_POS                                                      (4U)
#define TRANSCEIVER_CONFIG_STATE_TRIGGER_SELECT_MASK                                                     (0x000003F0U)  /**< rw-Register to select the state to trigger the STATE_CHANGE_IRQ flag. Each bit of the bit field enables one state - several states are possible. Note: If all bits are 0 no IRQ is triggered. xxxxx1 IDLE state enabled to trigger IRQ xxxx1x WaitTransmit state enabled to trigger IRQ  xxx1xx Transmitting state enabled to trigger IRQ xx1xxx WaitReceive state enabled to trigger IRQ x1xxxx WaitForData state enabled to trigger IRQ 1xxxxx Receiving state enabled to trigger IRQ .*/
#define TRANSCEIVER_CONFIG_TX_SKIP_SEND_ENABLE_POS                                                       (3U)
#define TRANSCEIVER_CONFIG_TX_SKIP_SEND_ENABLE_MASK                                                      (0x00000008U)  /**< rw-If set to 1 not transmission is started after tx_wait is expired and START_SEND was set Note: The bit is cleared by HW when the WaitReceive state is entered.*/
#define TRANSCEIVER_CONFIG_TX_FRAMESTEP_ENABLE_POS                                                       (2U)
#define TRANSCEIVER_CONFIG_TX_FRAMESTEP_ENABLE_MASK                                                      (0x00000004U)  /**< rw-If set to 1; at every start of transmission; each byte of data is sent in a seperate frame. SOF and EOF is appended to the data byte according to the framing settings. After one byte is transmitted; the TxEncoder waits for a new start trigger to continue with the next byte. */
#define TRANSCEIVER_CONFIG_RX_MULTIPLE_ENABLE_POS                                                        (1U)
#define TRANSCEIVER_CONFIG_RX_MULTIPLE_ENABLE_MASK                                                       (0x00000002U)  /**< rw-If this bit is set to 1; the receiver is re-activated after the end of a reception. A statusbyte is written to the RAM containing all relevant status information of the frame. Note: Data in RAM is word aligned therefore empty bytes of a data Word in RAM are padded with 0x00 bytes. SW has to calculate the correct address for the following frame. */
#define TRANSCEIVER_CONFIG_INITIATOR_POS                                                                 (0U)
#define TRANSCEIVER_CONFIG_INITIATOR_MASK                                                                (0x00000001U)  /**< rw-Set to 1; the CLIF is configured for initiator mode. Depending on this setting the behavior of the transceive command is different*/

#define PADCONFIG_REG                                                                                     0x05U
#define PADCONFIG_REG_EN_SLEW_RATE_CONTROL_POS                                                           (7U)
#define PADCONFIG_REG_EN_SLEW_RATE_CONTROL_MASK                                                          (0x00000080U)  /**< rw-Enables slew rate control of digital pads*/
#define PADCONFIG_REG_GPIO7_DIR_POS                                                                      (6U)
#define PADCONFIG_REG_GPIO7_DIR_MASK                                                                     (0x00000040U)  /**< rw-Enables the input driver of GPIO7*/
#define PADCONFIG_REG_GPIO6_DIR_POS                                                                      (5U)
#define PADCONFIG_REG_GPIO6_DIR_MASK                                                                     (0x00000020U)  /**< rw-Enables the input driver of GPIO6*/
#define PADCONFIG_REG_GPIO5_DIR_POS                                                                      (4U)
#define PADCONFIG_REG_GPIO5_DIR_MASK                                                                     (0x00000010U)  /**< rw-Enables the input driver of GPIO5*/
#define PADCONFIG_REG_GPIO4_DIR_POS                                                                      (3U)
#define PADCONFIG_REG_GPIO4_DIR_MASK                                                                     (0x00000008U)  /**< rw-Enables the input driver of GPIO4*/
#define PADCONFIG_REG_GPIO3_DIR_POS                                                                      (2U)
#define PADCONFIG_REG_GPIO3_DIR_MASK                                                                     (0x00000004U)  /**< rw-Enables the input driver of GPIO3*/
#define PADCONFIG_REG_GPIO2_DIR_POS                                                                      (1U)
#define PADCONFIG_REG_GPIO2_DIR_MASK                                                                     (0x00000002U)  /**< rw-Enables the input driver of GPIO2*/
#define PADCONFIG_REG_GPIO1_DIR_POS                                                                      (0U)
#define PADCONFIG_REG_GPIO1_DIR_MASK                                                                     (0x00000001U)  /**< rw-Enables the input driver of GPIO1*/

#define PADIN_REG                                                                                         0x06U
#define PADIN_REG_GPIO7_IN_VAL_POS                                                                       (6U)
#define PADIN_REG_GPIO7_IN_VAL_MASK                                                                      (0x00000040U)  /**< -*/
#define PADIN_REG_GPIO6_IN_VAL_POS                                                                       (5U)
#define PADIN_REG_GPIO6_IN_VAL_MASK                                                                      (0x00000020U)  /**< -*/
#define PADIN_REG_GPIO5_IN_VAL_POS                                                                       (4U)
#define PADIN_REG_GPIO5_IN_VAL_MASK                                                                      (0x00000010U)  /**< -*/
#define PADIN_REG_GPIO4_IN_VAL_POS                                                                       (3U)
#define PADIN_REG_GPIO4_IN_VAL_MASK                                                                      (0x00000008U)  /**< -*/
#define PADIN_REG_GPIO3_IN_VAL_POS                                                                       (2U)
#define PADIN_REG_GPIO3_IN_VAL_MASK                                                                      (0x00000004U)  /**< -*/
#define PADIN_REG_GPIO2_IN_VAL_POS                                                                       (1U)
#define PADIN_REG_GPIO2_IN_VAL_MASK                                                                      (0x00000002U)  /**< -*/
#define PADIN_REG_GPIO1_IN_VAL_POS                                                                       (0U)
#define PADIN_REG_GPIO1_IN_VAL_MASK                                                                      (0x00000001U)  /**< -*/

#define PADOUT_REG                                                                                        0x07U
#define PADOUT_REG_GPIO7_OUT_VAL_POS                                                                     (6U)
#define PADOUT_REG_GPIO7_OUT_VAL_MASK                                                                    (0x00000040U)  /**< rw-*/
#define PADOUT_REG_GPIO6_OUT_VAL_POS                                                                     (5U)
#define PADOUT_REG_GPIO6_OUT_VAL_MASK                                                                    (0x00000020U)  /**< rw-*/
#define PADOUT_REG_GPIO5_OUT_VAL_POS                                                                     (4U)
#define PADOUT_REG_GPIO5_OUT_VAL_MASK                                                                    (0x00000010U)  /**< rw-*/
#define PADOUT_REG_GPIO4_OUT_VAL_POS                                                                     (3U)
#define PADOUT_REG_GPIO4_OUT_VAL_MASK                                                                    (0x00000008U)  /**< rw-*/
#define PADOUT_REG_GPIO3_OUT_VAL_POS                                                                     (2U)
#define PADOUT_REG_GPIO3_OUT_VAL_MASK                                                                    (0x00000004U)  /**< rw-*/
#define PADOUT_REG_GPIO2_OUT_VAL_POS                                                                     (1U)
#define PADOUT_REG_GPIO2_OUT_VAL_MASK                                                                    (0x00000002U)  /**< rw-*/
#define PADOUT_REG_GPIO1_OUT_VAL_POS                                                                     (0U)
#define PADOUT_REG_GPIO1_OUT_VAL_MASK                                                                    (0x00000001U)  /**< rw-*/

#define TIMER0_STATUS                                                                                     0x08U
#define TIMER0_STATUS_T0_RUNNING_POS                                                                     (20U)
#define TIMER0_STATUS_T0_RUNNING_MASK                                                                    (0x00100000U)  /**< r--Indicates that timer T0 is running (busy) */
#define TIMER0_STATUS_T0_VALUE_POS                                                                       (0U)
#define TIMER0_STATUS_T0_VALUE_MASK                                                                      (0x000FFFFFU)  /**< r--Value of 20bit counter in timer T0 */

#define TIMER1_STATUS                                                                                     0x09U
#define TIMER1_STATUS_T1_RUNNING_POS                                                                     (20U)
#define TIMER1_STATUS_T1_RUNNING_MASK                                                                    (0x00100000U)  /**< r--Indicates that timer T1 is running (busy) */
#define TIMER1_STATUS_T1_VALUE_POS                                                                       (0U)
#define TIMER1_STATUS_T1_VALUE_MASK                                                                      (0x000FFFFFU)  /**< r--Value of 20bit counter in timer T1 */

#define TIMER2_STATUS                                                                                     0x0AU
#define TIMER2_STATUS_T2_RUNNING_POS                                                                     (20U)
#define TIMER2_STATUS_T2_RUNNING_MASK                                                                    (0x00100000U)  /**< r--Indicates that timer T2 is running (busy) */
#define TIMER2_STATUS_T2_VALUE_POS                                                                       (0U)
#define TIMER2_STATUS_T2_VALUE_MASK                                                                      (0x000FFFFFU)  /**< r--Value of 20bit counter in timer T2 */

#define TIMER0_RELOAD                                                                                     0x0BU
#define TIMER0_RELOAD_T0_RELOAD_VALUE_POS                                                                (0U)
#define TIMER0_RELOAD_T0_RELOAD_VALUE_MASK                                                               (0x000FFFFFU)  /**< rw-Reload value of the timer T0. */

#define TIMER1_RELOAD                                                                                     0x0CU
#define TIMER1_RELOAD_T1_RELOAD_VALUE_POS                                                                (0U)
#define TIMER1_RELOAD_T1_RELOAD_VALUE_MASK                                                               (0x000FFFFFU)  /**< rw-Reload value of the timer T1.*/

#define TIMER2_RELOAD                                                                                     0x0DU
#define TIMER2_RELOAD_T2_RELOAD_VALUE_POS                                                                (0U)
#define TIMER2_RELOAD_T2_RELOAD_VALUE_MASK                                                               (0x000FFFFFU)  /**< rw-Reload value of the timer T2. */

#define TIMER0_CONFIG                                                                                     0x0EU
#define TIMER0_CONFIG_T0_STOP_ON_RX_STARTED_POS                                                          (20U)
#define TIMER0_CONFIG_T0_STOP_ON_RX_STARTED_MASK                                                         (0x00100000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when a data reception begins (1st bit is received). */
#define TIMER0_CONFIG_T0_STOP_ON_TX_STARTED_POS                                                          (19U)
#define TIMER0_CONFIG_T0_STOP_ON_TX_STARTED_MASK                                                         (0x00080000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when a data transmission begins. */
#define TIMER0_CONFIG_T0_STOP_ON_RF_ON_EXT_POS                                                           (18U)
#define TIMER0_CONFIG_T0_STOP_ON_RF_ON_EXT_MASK                                                          (0x00040000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when the external RF field is detected.*/
#define TIMER0_CONFIG_T0_STOP_ON_RF_OFF_EXT_POS                                                          (17U)
#define TIMER0_CONFIG_T0_STOP_ON_RF_OFF_EXT_MASK                                                         (0x00020000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when the external RF field vanishes. */
#define TIMER0_CONFIG_T0_STOP_ON_RF_ON_INT_POS                                                           (16U)
#define TIMER0_CONFIG_T0_STOP_ON_RF_ON_INT_MASK                                                          (0x00010000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when the internal RF field is turned on. */
#define TIMER0_CONFIG_T0_STOP_ON_RF_OFF_INT_POS                                                          (15U)
#define TIMER0_CONFIG_T0_STOP_ON_RF_OFF_INT_MASK                                                         (0x00008000U)  /**< rw-T0_STOP_EVENT: If set; the timer T0 is stopped when the internal RF field is turned off. */
#define TIMER0_CONFIG_T0_START_ON_RX_STARTED_POS                                                         (14U)
#define TIMER0_CONFIG_T0_START_ON_RX_STARTED_MASK                                                        (0x00004000U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when a data reception begins (1st bit is received). */
#define TIMER0_CONFIG_T0_START_ON_RX_ENDED_POS                                                           (13U)
#define TIMER0_CONFIG_T0_START_ON_RX_ENDED_MASK                                                          (0x00002000U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when a data reception ends. */
#define TIMER0_CONFIG_T0_START_ON_TX_STARTED_POS                                                         (12U)
#define TIMER0_CONFIG_T0_START_ON_TX_STARTED_MASK                                                        (0x00001000U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when a data transmission begins. */
#define TIMER0_CONFIG_T0_START_ON_TX_ENDED_POS                                                           (11U)
#define TIMER0_CONFIG_T0_START_ON_TX_ENDED_MASK                                                          (0x00000800U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when a data transmission ends. */
#define TIMER0_CONFIG_T0_START_ON_RF_ON_EXT_POS                                                          (10U)
#define TIMER0_CONFIG_T0_START_ON_RF_ON_EXT_MASK                                                         (0x00000400U)  /**< rw-T0_START_EVENT: If set; the timer T0 is startedwhen the external RF field is detected. */
#define TIMER0_CONFIG_T0_START_ON_RF_OFF_EXT_POS                                                         (9U)
#define TIMER0_CONFIG_T0_START_ON_RF_OFF_EXT_MASK                                                        (0x00000200U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when the external RF field is not detected any more. */
#define TIMER0_CONFIG_T0_START_ON_RF_ON_INT_POS                                                          (8U)
#define TIMER0_CONFIG_T0_START_ON_RF_ON_INT_MASK                                                         (0x00000100U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started when an internal RF field is turned on. */
#define TIMER0_CONFIG_T0_START_ON_RF_OFF_INT_POS                                                         (7U)
#define TIMER0_CONFIG_T0_START_ON_RF_OFF_INT_MASK                                                        (0x00000080U)  /**< rw-T0_START_EVENT:If set; the timer T0 is started when an internal RF field is turned off. */
#define TIMER0_CONFIG_T0_START_NOW_POS                                                                   (6U)
#define TIMER0_CONFIG_T0_START_NOW_MASK                                                                  (0x00000040U)  /**< rw-T0_START_EVENT: If set; the timer T0 is started immediatly. */
#define TIMER0_CONFIG_T0_PRESCALE_SEL_POS                                                                (3U)
#define TIMER0_CONFIG_T0_PRESCALE_SEL_MASK                                                               (0x00000038U)  /**< rw-Controls frequency/period of the timer T0 when the prescaler is activated in T0_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter   */
#define TIMER0_CONFIG_T0_MODE_SEL_POS                                                                    (2U)
#define TIMER0_CONFIG_T0_MODE_SEL_MASK                                                                   (0x00000004U)  /**< rw-Configuration of the timer T0 clock. 0b* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1b Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T0_PRESCALE_SEL).*/
#define TIMER0_CONFIG_T0_RELOAD_ENABLE_POS                                                               (1U)
#define TIMER0_CONFIG_T0_RELOAD_ENABLE_MASK                                                              (0x00000002U)  /**< rw-If set to 0;the timer T0 will stop on expiration. 0* After expiration the timer T0 will stop counting; i.e.; remain zero; reset value. 1 After expiration the timer T0 will reload its preset value and continue counting down. */
#define TIMER0_CONFIG_T0_ENABLE_POS                                                                      (0U)
#define TIMER0_CONFIG_T0_ENABLE_MASK                                                                     (0x00000001U)  /**< rw-Enables the timer T0*/

#define TIMER1_CONFIG                                                                                     0x0FU
#define TIMER1_CONFIG_T1_STOP_ON_RX_STARTED_POS                                                          (20U)
#define TIMER1_CONFIG_T1_STOP_ON_RX_STARTED_MASK                                                         (0x00100000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when a data reception begins (1st bit is received). */
#define TIMER1_CONFIG_T1_STOP_ON_TX_STARTED_POS                                                          (19U)
#define TIMER1_CONFIG_T1_STOP_ON_TX_STARTED_MASK                                                         (0x00080000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when a data transmission begins. */
#define TIMER1_CONFIG_T1_STOP_ON_RF_ON_EXT_POS                                                           (18U)
#define TIMER1_CONFIG_T1_STOP_ON_RF_ON_EXT_MASK                                                          (0x00040000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when the external RF field is detected. */
#define TIMER1_CONFIG_T1_STOP_ON_RF_OFF_EXT_POS                                                          (17U)
#define TIMER1_CONFIG_T1_STOP_ON_RF_OFF_EXT_MASK                                                         (0x00020000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when the external RF field vanishes. */
#define TIMER1_CONFIG_T1_STOP_ON_RF_ON_INT_POS                                                           (16U)
#define TIMER1_CONFIG_T1_STOP_ON_RF_ON_INT_MASK                                                          (0x00010000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when the internal RF field is turned on. */
#define TIMER1_CONFIG_T1_STOP_ON_RF_OFF_INT_POS                                                          (15U)
#define TIMER1_CONFIG_T1_STOP_ON_RF_OFF_INT_MASK                                                         (0x00008000U)  /**< rw-T1_STOP_EVENT: If set; the timer T1 is stopped when the internal RF field is turned off. */
#define TIMER1_CONFIG_T1_START_ON_RX_STARTED_POS                                                         (14U)
#define TIMER1_CONFIG_T1_START_ON_RX_STARTED_MASK                                                        (0x00004000U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when a data reception begins (1st bit is received). */
#define TIMER1_CONFIG_T1_START_ON_RX_ENDED_POS                                                           (13U)
#define TIMER1_CONFIG_T1_START_ON_RX_ENDED_MASK                                                          (0x00002000U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when a data reception ends. */
#define TIMER1_CONFIG_T1_START_ON_TX_STARTED_POS                                                         (12U)
#define TIMER1_CONFIG_T1_START_ON_TX_STARTED_MASK                                                        (0x00001000U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when a data transmission begins.   */
#define TIMER1_CONFIG_T1_START_ON_TX_ENDED_POS                                                           (11U)
#define TIMER1_CONFIG_T1_START_ON_TX_ENDED_MASK                                                          (0x00000800U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when a data transmission ends. */
#define TIMER1_CONFIG_T1_START_ON_RF_ON_EXT_POS                                                          (10U)
#define TIMER1_CONFIG_T1_START_ON_RF_ON_EXT_MASK                                                         (0x00000400U)  /**< rw-T1_START_EVENT: If set; the timer T1 is startedwhen the external RF field is detected. */
#define TIMER1_CONFIG_T1_START_ON_RF_OFF_EXT_POS                                                         (9U)
#define TIMER1_CONFIG_T1_START_ON_RF_OFF_EXT_MASK                                                        (0x00000200U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when the external RF field is not detected any more. */
#define TIMER1_CONFIG_T1_START_ON_RF_ON_INT_POS                                                          (8U)
#define TIMER1_CONFIG_T1_START_ON_RF_ON_INT_MASK                                                         (0x00000100U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started when an internal RF field is turned on. */
#define TIMER1_CONFIG_T1_START_ON_RF_OFF_INT_POS                                                         (7U)
#define TIMER1_CONFIG_T1_START_ON_RF_OFF_INT_MASK                                                        (0x00000080U)  /**< rw-T1_START_EVENT:If set; the timer T1 is started when an internal RF field is turned off. */
#define TIMER1_CONFIG_T1_START_NOW_POS                                                                   (6U)
#define TIMER1_CONFIG_T1_START_NOW_MASK                                                                  (0x00000040U)  /**< rw-T1_START_EVENT: If set; the timer T1 is started immediatly. */
#define TIMER1_CONFIG_T1_PRESCALE_SEL_POS                                                                (3U)
#define TIMER1_CONFIG_T1_PRESCALE_SEL_MASK                                                               (0x00000038U)  /**< rw-Controls frequency/period of the timer T1 when the prescaler is activated by T1_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter */
#define TIMER1_CONFIG_T1_MODE_SEL_POS                                                                    (2U)
#define TIMER1_CONFIG_T1_MODE_SEL_MASK                                                                   (0x00000004U)  /**< rw-If set; the timer T1 is started the prescaler for the timer T1 is enabled. 0* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1 Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T1_PRESCALE_SEL). */
#define TIMER1_CONFIG_T1_RELOAD_ENABLE_POS                                                               (1U)
#define TIMER1_CONFIG_T1_RELOAD_ENABLE_MASK                                                              (0x00000002U)  /**< rw-If set to 0;the timer T1 will stop on expiration. 0* After expiration the timer T1 will stop counting; i.e.; remain zero; reset value. 1 After expiration the timer T1 will reload its preset value and continue counting down. */
#define TIMER1_CONFIG_T1_ENABLE_POS                                                                      (0U)
#define TIMER1_CONFIG_T1_ENABLE_MASK                                                                     (0x00000001U)  /**< rw-Enables the timer T1*/

#define TIMER2_CONFIG                                                                                     0x10U
#define TIMER2_CONFIG_T2_STOP_ON_RX_STARTED_POS                                                          (20U)
#define TIMER2_CONFIG_T2_STOP_ON_RX_STARTED_MASK                                                         (0x00100000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when a data reception begins (1st bit is received). */
#define TIMER2_CONFIG_T2_STOP_ON_TX_STARTED_POS                                                          (19U)
#define TIMER2_CONFIG_T2_STOP_ON_TX_STARTED_MASK                                                         (0x00080000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when a data transmission begins. */
#define TIMER2_CONFIG_T2_STOP_ON_RF_ON_EXT_POS                                                           (18U)
#define TIMER2_CONFIG_T2_STOP_ON_RF_ON_EXT_MASK                                                          (0x00040000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when the external RF field is detected. */
#define TIMER2_CONFIG_T2_STOP_ON_RF_OFF_EXT_POS                                                          (17U)
#define TIMER2_CONFIG_T2_STOP_ON_RF_OFF_EXT_MASK                                                         (0x00020000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when the external RF field vanishes. */
#define TIMER2_CONFIG_T2_STOP_ON_RF_ON_INT_POS                                                           (16U)
#define TIMER2_CONFIG_T2_STOP_ON_RF_ON_INT_MASK                                                          (0x00010000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when the internal RF field is turned on. */
#define TIMER2_CONFIG_T2_STOP_ON_RF_OFF_INT_POS                                                          (15U)
#define TIMER2_CONFIG_T2_STOP_ON_RF_OFF_INT_MASK                                                         (0x00008000U)  /**< rw-T2_STOP_EVENT: If set; the timer T2 is stopped when the internal RF field is turned off. */
#define TIMER2_CONFIG_T2_START_ON_RX_STARTED_POS                                                         (14U)
#define TIMER2_CONFIG_T2_START_ON_RX_STARTED_MASK                                                        (0x00004000U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when a data reception begins (1st bit is received). */
#define TIMER2_CONFIG_T2_START_ON_RX_ENDED_POS                                                           (13U)
#define TIMER2_CONFIG_T2_START_ON_RX_ENDED_MASK                                                          (0x00002000U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when a data reception ends. */
#define TIMER2_CONFIG_T2_START_ON_TX_STARTED_POS                                                         (12U)
#define TIMER2_CONFIG_T2_START_ON_TX_STARTED_MASK                                                        (0x00001000U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when a data transmission begins. */
#define TIMER2_CONFIG_T2_START_ON_TX_ENDED_POS                                                           (11U)
#define TIMER2_CONFIG_T2_START_ON_TX_ENDED_MASK                                                          (0x00000800U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when a data transmission ends. */
#define TIMER2_CONFIG_T2_START_ON_RF_ON_EXT_POS                                                          (10U)
#define TIMER2_CONFIG_T2_START_ON_RF_ON_EXT_MASK                                                         (0x00000400U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when the external RF field is detected. */
#define TIMER2_CONFIG_T2_START_ON_RF_OFF_EXT_POS                                                         (9U)
#define TIMER2_CONFIG_T2_START_ON_RF_OFF_EXT_MASK                                                        (0x00000200U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when the external RF field is not detected any more. */
#define TIMER2_CONFIG_T2_START_ON_RF_ON_INT_POS                                                          (8U)
#define TIMER2_CONFIG_T2_START_ON_RF_ON_INT_MASK                                                         (0x00000100U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when an internal RF field is turned on. */
#define TIMER2_CONFIG_T2_START_ON_RF_OFF_INT_POS                                                         (7U)
#define TIMER2_CONFIG_T2_START_ON_RF_OFF_INT_MASK                                                        (0x00000080U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started when an internal RF field is turned off. */
#define TIMER2_CONFIG_T2_START_NOW_POS                                                                   (6U)
#define TIMER2_CONFIG_T2_START_NOW_MASK                                                                  (0x00000040U)  /**< rw-T2_START_EVENT: If set; the timer T2 is started immediatly. */
#define TIMER2_CONFIG_T2_PRESCALE_SEL_POS                                                                (3U)
#define TIMER2_CONFIG_T2_PRESCALE_SEL_MASK                                                               (0x00000038U)  /**< rw-Controls frequency/period of the timer T2 when the prescaler is activated by T2_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter */
#define TIMER2_CONFIG_T2_MODE_SEL_POS                                                                    (2U)
#define TIMER2_CONFIG_T2_MODE_SEL_MASK                                                                   (0x00000004U)  /**< rw-If set; the timer T2 is started the prescaler for the timer T2 is enabled. 0* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1 Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T2_PRESCALE_SEL). */
#define TIMER2_CONFIG_T2_RELOAD_ENABLE_POS                                                               (1U)
#define TIMER2_CONFIG_T2_RELOAD_ENABLE_MASK                                                              (0x00000002U)  /**< rw-If set to 0;the timer T2 will stop on expiration. 0* After expiration the timer T2 will stop counting; i.e.; remain zero; reset value. 1 After expiration the timer T2 will reload its preset value and continue counting down. */
#define TIMER2_CONFIG_T2_ENABLE_POS                                                                      (0U)
#define TIMER2_CONFIG_T2_ENABLE_MASK                                                                     (0x00000001U)  /**< rw-Enables the timer T2 */

#define RX_WAIT_CONFIG                                                                                    0x11U
#define RX_WAIT_CONFIG_RX_WAIT_VALUE_POS                                                                 (8U)
#define RX_WAIT_CONFIG_RX_WAIT_VALUE_MASK                                                                (0x0FFFFF00U)  /**< rw-Defines the rx_wait timer reload value. Note: If set to 00000h the rx_wait guard time is disabled */
#define RX_WAIT_CONFIG_RX_WAIT_PRESCALER_POS                                                             (0U)
#define RX_WAIT_CONFIG_RX_WAIT_PRESCALER_MASK                                                            (0x000000FFU)  /**< rw-Defines the prescaler reload value for the rx_wait timer.*/

#define CRC_RX_CONFIG                                                                                     0x12U
#define CRC_RX_CONFIG_RX_CRC_PRESET_VALUE_POS                                                            (16U)
#define CRC_RX_CONFIG_RX_CRC_PRESET_VALUE_MASK                                                           (0xFFFF0000U)  /**< rw-Arbitrary preset value for the Rx-Decoder CRC calculation. */
#define CRC_RX_CONFIG_RESERVED_POS                                                                       (12U)
#define CRC_RX_CONFIG_RESERVED_MASK                                                                      (0x0000F000U)  /**< r--*/
#define CRC_RX_CONFIG_RX_PARITY_TYPE_POS                                                                 (11U)
#define CRC_RX_CONFIG_RX_PARITY_TYPE_MASK                                                                (0x00000800U)  /**< rw-Defines which type of the parity-bit is used Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443A communication is detected. 0 Even parity calculation is used 1 Odd parity calculation is used */
#define CRC_RX_CONFIG_RX_PARITY_ENABLE_POS                                                               (10U)
#define CRC_RX_CONFIG_RX_PARITY_ENABLE_MASK                                                              (0x00000400U)  /**< rw-If set to 1; a parity-bit for each byte is expected; will be extracted from data stream and checked for correctness. In case the parity-bit is incorrect; the RX_DATA_INTEGRITY_ERROR flag is set. Nevertheless the reception is continued. Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443A communication is detected.*/
#define CRC_RX_CONFIG_VALUES_AFTER_COLLISION_POS                                                         (9U)
#define CRC_RX_CONFIG_VALUES_AFTER_COLLISION_MASK                                                        (0x00000200U)  /**< rw-This bit defined the value of bits received after a collision occured. 0* All received bits after a collision will be cleared. 1 All received bits after a collision keep their value. */
#define CRC_RX_CONFIG_RX_BIT_ALIGN_POS                                                                   (6U)
#define CRC_RX_CONFIG_RX_BIT_ALIGN_MASK                                                                  (0x000001C0U)  /**< rw-RxAlign defines the bit position within the byte for the first bit received. Further received bits are stored at the following bit positions.    */
#define CRC_RX_CONFIG_RX_CRC_PRESET_SEL_POS                                                              (3U)
#define CRC_RX_CONFIG_RX_CRC_PRESET_SEL_MASK                                                             (0x00000038U)  /**< rw-Preset value of the CRC register for the Rx-Decoder. For a CRC calculation using 5bits, only the LSByte is used. 000b* 0000h, reset value Note that this configuration is set by the Mode detector for FeliCa. 001b  6363h Note that this configuration is set by the Mode detector for ISO14443 type A. 010b  A671h 011b FFFFh Note that this configuration is set by the Mode detector for ISO14443 type B. 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value RX_CRC_PRESET_VALUE*/
#define CRC_RX_CONFIG_RX_CRC_TYPE_POS                                                                    (2U)
#define CRC_RX_CONFIG_RX_CRC_TYPE_MASK                                                                   (0x00000004U)  /**< rw-Controls the type of CRC calulation for the Rx-Decoder 0* 16bit CRC calculation; reset value 1 5bit CRC calculation */
#define CRC_RX_CONFIG_RX_CRC_INV_POS                                                                     (1U)
#define CRC_RX_CONFIG_RX_CRC_INV_MASK                                                                    (0x00000002U)  /**< rw-Controls the comparison of the CRC checksum for the Rx-Decoder 0* Not inverted CRC value: 0000h, reset value Note that this nit is cleared by the Mode detector for ISO14443 type A and FeliCa. 1 Inverted CRC value: F0B8h Note that this bit is set by the Mode detector for ISO14443 type B.*/
#define CRC_RX_CONFIG_RX_CRC_ENABLE_POS                                                                  (0U)
#define CRC_RX_CONFIG_RX_CRC_ENABLE_MASK                                                                 (0x00000001U)  /**< rw-If set; the Rx-Decoder will check the CRC for correctness. Note that this bit is set by the Mode Detector; when ISO14443 type B; or FeliCa (212 or 424kBd) is detected.*/

#define RX_STATUS                                                                                         0x13U
#define RX_STATUS_RX_COLL_POS_POS                                                                        (19U)
#define RX_STATUS_RX_COLL_POS_MASK                                                                       (0x03F80000U)  /**< r--These bits show the bit position of the first detected collision in a received frame (only databits are interpreted). Note: These bits shall only be interpreted in passive communication mode at 106 kbit/s or ISO14443-3 TypeA reader / writer or ICode SLI reader / writer mode if bit CollPosValid is set to 1. Note: If RX_ALIGN is set to a value different to 0;this value is included in the RX_COLL_POS. */
#define RX_STATUS_RX_COLLISION_DETECTED_POS                                                              (18U)
#define RX_STATUS_RX_COLLISION_DETECTED_MASK                                                             (0x00040000U)  /**< r--This flag is set to 1; when a collision has accured. The position of the first collision is shown in the register RX_COLLPOS*/
#define RX_STATUS_RX_PROTOCOL_ERROR_POS                                                                  (17U)
#define RX_STATUS_RX_PROTOCOL_ERROR_MASK                                                                 (0x00020000U)  /**< r--This flag is set to 1; when a protocol error has accured. A protocol error can be a wrong stopbit; a missing or wrong ISO14443B EOF or SOF or a wrong number of received data bytes. Note: When a protocol error is detected; data reception is stopped. Note: The flag is automatically cleared at start of next reception. */
#define RX_STATUS_RX_DATA_INTEGRITY_ERROR_POS                                                            (16U)
#define RX_STATUS_RX_DATA_INTEGRITY_ERROR_MASK                                                           (0x00010000U)  /**< r--This flag is set to 1; if a data integrity error has been detected. Possible caused can be a wrong parity or a wrong CRC. Note: On a data integrity error the reception is continued Note: The flag is automatically cleared at start of next reception. Note: If a reversed parity bit is a stop criteria; the flag is not set to 1 in case of a wrong parity. */
#define RX_STATUS_RX_NUM_LAST_BITS_POS                                                                   (13U)
#define RX_STATUS_RX_NUM_LAST_BITS_MASK                                                                  (0x0000E000U)  /**< r--Defines the number of valid bits of the last data byte received in bit-oriented communications. If zero the whole byte is valid. */
#define RX_STATUS_RX_NUM_FRAMES_RECEIVED_POS                                                             (9U)
#define RX_STATUS_RX_NUM_FRAMES_RECEIVED_MASK                                                            (0x00001E00U)  /**< r--Indicates the number of frames received. The value is updated when the RxIRQ is raised. Note: This bit field is only valid when the RxMultiple is active (bit RX_MULTIPLE_ENABLE set) */
#define RX_STATUS_RX_NUM_BYTES_RECEIVED_POS                                                              (0U)
#define RX_STATUS_RX_NUM_BYTES_RECEIVED_MASK                                                             (0x000001FFU)  /**< r--the number of bytes received. The value is valid when the RxIRQ is raised until the receiver is enabled again.*/

#define TX_UNDERSHOOT_CONFIG                                                                              0x14U
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_POS                                                   (16U)
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_MASK                                                  (0xFFFF0000U)  /**< rw-Undershoot pattern which is transmitted after each falling egde. */
#define TX_UNDERSHOOT_CONFIG_RESERVED_POS                                                                (5U)
#define TX_UNDERSHOOT_CONFIG_RESERVED_MASK                                                               (0x0000FFE0U)  /**< r--Reserved */
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_LEN_POS                                               (1U)
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_LEN_MASK                                              (0x0000001EU)  /**< rw-Defines length of the undershoot prevention pattern (value +1U). The pattern is applied starting from the LSB of the defined pattern; all other bits are ignored. */
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_ENABLE_POS                                               (0U)
#define TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_ENABLE_MASK                                              (0x00000001U)  /**< rw-If set to 1; the undershoot protection is enabled*/

#define TX_OVERSHOOT_CONFIG                                                                               0x15U
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_POS                                                     (16U)
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_MASK                                                    (0xFFFF0000U)  /**< rw-Overshoot pattern which is transmitted after each rising egde. */
#define TX_OVERSHOOT_CONFIG_RESERVED_POS                                                                 (5U)
#define TX_OVERSHOOT_CONFIG_RESERVED_MASK                                                                (0x0000FFE0U)  /**< r--Reserved */
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_LEN_POS                                                 (1U)
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_LEN_MASK                                                (0x0000001EU)  /**< rw-Defines length of the overshoot prevention pattern (value +1U). The pattern is applied starting from the LSB of the defined pattern; all other bits are ignored. */
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PROT_ENABLE_POS                                                 (0U)
#define TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PROT_ENABLE_MASK                                                (0x00000001U)  /**< rw-If set to 1; the overshoot protection is enabled.*/

#define TX_DATA_MOD                                                                                       0x16U
#define TX_DATA_MOD_TX_DATA_MOD_WIDTH_POS                                                                (8U)
#define TX_DATA_MOD_TX_DATA_MOD_WIDTH_MASK                                                               (0x0000FF00U)  /**< rw-*/
#define TX_DATA_MOD_TX_BITPHASE_POS                                                                      (0U)
#define TX_DATA_MOD_TX_BITPHASE_MASK                                                                     (0x000000FFU)  /**< rw-Defines the number of 13.56MHz cycles used for adjustment of tx_wait to meet the FDT. */

#define TX_WAIT_CONFIG                                                                                    0x17U
#define TX_WAIT_CONFIG_TX_WAIT_VALUE_POS                                                                 (8U)
#define TX_WAIT_CONFIG_TX_WAIT_VALUE_MASK                                                                (0x0FFFFF00U)  /**< rw-Defines the tx_wait timer reload value. Note: If set to 00000h the tx_wait guard time is disabled Note: This bit is set by HW a protocol is detected in automatic mode detection*/
#define TX_WAIT_CONFIG_TX_WAIT_PRESCALER_POS                                                             (0U)
#define TX_WAIT_CONFIG_TX_WAIT_PRESCALER_MASK                                                            (0x000000FFU)  /**< rw-Defines the prescaler reload value for the tx_wait timer. Note: This bit is set by HW a protocol is  detected in automatic mode detection*/

#define TX_CONFIG                                                                                         0x18U
#define TX_CONFIG_TX_PARITY_LAST_INV_ENABLE_POS                                                          (13U)
#define TX_CONFIG_TX_PARITY_LAST_INV_ENABLE_MASK                                                         (0x00002000U)  /**< rw-If set to 1; the parity bit of last sent data byte is inverted */
#define TX_CONFIG_TX_PARITY_TYPE_POS                                                                     (12U)
#define TX_CONFIG_TX_PARITY_TYPE_MASK                                                                    (0x00001000U)  /**< rw-Defines the type of the parity bit 0 Even Parity is calculated 1 Odd parity is calculated       */
#define TX_CONFIG_TX_PARITY_ENABLE_POS                                                                   (11U)
#define TX_CONFIG_TX_PARITY_ENABLE_MASK                                                                  (0x00000800U)  /**< rw-If set to 1; a parity bit is calculated and appended to each byte transmitted. If the Transmission Of Data Is Enabled and TX_NUM_BYTES_2_SEND is zero; then a NO_DATA_ERROR occurs. */
#define TX_CONFIG_TX_DATA_ENABLE_POS                                                                     (10U)
#define TX_CONFIG_TX_DATA_ENABLE_MASK                                                                    (0x00000400U)  /**< rw-If set to 1; transmission of data is enabled otherwise only symbols are tranmitted. */
#define TX_CONFIG_TX_STOP_SYMBOL_POS                                                                     (8U)
#define TX_CONFIG_TX_STOP_SYMBOL_MASK                                                                    (0x00000300U)  /**< rw-Defines which pattern symbol is sent as frame stop-symbol 00b No symbol is sent 01b Symbol1 is sent 10b Symbol2 is sent 11b Symbol3 is sent */
#define TX_CONFIG_TX_START_SYMBOL_POS                                                                    (6U)
#define TX_CONFIG_TX_START_SYMBOL_MASK                                                                   (0x000000C0U)  /**< rw-Defines which symbol pattern is sent as frame start-symbol 00b No symbol pattern is sent 01b Symbol0 is sent 10b Symbol1 is sent 11b Symbol2 is sent*/
#define TX_CONFIG_TX_LAST_BITS_POS                                                                       (3U)
#define TX_CONFIG_TX_LAST_BITS_MASK                                                                      (0x00000038U)  /**< rw-Defines how many bits of the last data byte to be sent. If set to 000b all bits of the last data byte are sent. Note: Bits are skipped at the end of the byte */
#define TX_CONFIG_TX_FIRST_BITS_POS                                                                      (0U)
#define TX_CONFIG_TX_FIRST_BITS_MASK                                                                     (0x00000007U)  /**< rw-Defines how many bits of the first data byte to be sent. If set to 000b all bits of the last data byte are sent. Note: Bits are skipped at the beginning of the byte */

#define CRC_TX_CONFIG                                                                                     0x19U
#define CRC_TX_CONFIG_TX_CRC_PRESET_VALUE_POS                                                            (16U)
#define CRC_TX_CONFIG_TX_CRC_PRESET_VALUE_MASK                                                           (0xFFFF0000U)  /**< rw-Arbitrary preset value for the Tx-Encoder CRC calculation. */
#define CRC_TX_CONFIG_RESERVED_POS                                                                       (7U)
#define CRC_TX_CONFIG_RESERVED_MASK                                                                      (0x0000FF80U)  /**< r--Reserved */
#define CRC_TX_CONFIG_TX_CRC_BYTE2_ENABLE_POS                                                            (6U)
#define CRC_TX_CONFIG_TX_CRC_BYTE2_ENABLE_MASK                                                           (0x00000040U)  /**< rw-If set; the CRC is calculated from the 2nd byte onwards (intended for HID). Note that this option is used in the Tx-Encoder.           */
#define CRC_TX_CONFIG_TX_CRC_PRESET_SEL_POS                                                              (3U)
#define CRC_TX_CONFIG_TX_CRC_PRESET_SEL_MASK                                                             (0x00000038U)  /**< rw-Preset value of the CRC register for the Tx-Encoder. For a CRC calculation using 5bits; only the LSByte is used. 000b* 0000h; reset value 001b  6363h 010b A671h 011b  FFFFh 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value TX_CRC_PRESET_VALUE*/
#define CRC_TX_CONFIG_TX_CRC_TYPE_POS                                                                    (2U)
#define CRC_TX_CONFIG_TX_CRC_TYPE_MASK                                                                   (0x00000004U)  /**< rw-Controls the type of CRC calulation for the Tx-Encoder 0* 16bit CRC calculation; reset value 1 5bit CRC calculation */
#define CRC_TX_CONFIG_TX_CRC_INV_POS                                                                     (1U)
#define CRC_TX_CONFIG_TX_CRC_INV_MASK                                                                    (0x00000002U)  /**< rw-Controls the sending of an inverted CRC value by the Tx-Encoder 0* Not inverted CRC checksum; reset value 1 Inverted CRC checksum */
#define CRC_TX_CONFIG_TX_CRC_ENABLE_POS                                                                  (0U)
#define CRC_TX_CONFIG_TX_CRC_ENABLE_MASK                                                                 (0x00000001U)  /**< rw-If set to one; the Tx-Encoder will compute and transmit a CRC.*/

#define SIGPRO_CONFIG                                                                                     0x1AU
#define SIGPRO_CONFIG_BAUDRATE_POS                                                                       (0U)
#define SIGPRO_CONFIG_BAUDRATE_MASK                                                                      (0x00000007U)  /**< rw-Defines the baudrate of the receiving signal. The MSB is only relevent for reader mode. Note: These bits are set by the mode-detector if automatic mode detection is enabled and the communication mode is detected. 000* RFU 001 RFU 010 26kBd (only for reader mode) 011 53kBd (only for reader mode) 100 106kBd Note that this configuration is set by the Mode detector for ISO14443 type A and B. 101 212kBd Note that this configuration is set by the Mode detector for FeliCa 212kBd. 110 424kBd Note that this configuration is set by the Mode detector for FeliCa 424kBd. 111 848kBd*/

#define SIGPRO_CM_CONFIG                                                                                  0x1BU
#define SIGPRO_CM_CONFIG_RX_FRAMING_POS                                                                  (29U)
#define SIGPRO_CM_CONFIG_RX_FRAMING_MASK                                                                 (0x60000000U)  /**< rw-Defines the framing in card mode. Note that these bits are set by the mod-detector if automatic mode detection is enabled and the communication mode is detected. 00* ISO14443-3 TypeA 01 ISO18092 (NFC - with Syncbyte 0xF0) 10 FeliCa 11 ISO14443B*/
#define SIGPRO_CM_CONFIG_EDGE_DETECT_TAP_SEL_POS                                                         (26U)
#define SIGPRO_CM_CONFIG_EDGE_DETECT_TAP_SEL_MASK                                                        (0x1C000000U)  /**< rw-Selects the number of taps of the edge-detector filter. 000* Edge detector filter with 4 taps 001 Edge detector filter with 6 taps 010 Edge detector filter with 8 taps  4             011 Edge detector filter with 12 taps 100 Edge detector filter with 16 taps 101 Edge detector filter with 18 taps 110 Edge detector filter with 24 taps 111 Edge detector filter with 32 taps */
#define SIGPRO_CM_CONFIG_EDGE_DETECT_TH_POS                                                              (13U)
#define SIGPRO_CM_CONFIG_EDGE_DETECT_TH_MASK                                                             (0x03FFE000U)  /**< rw-Threshold for the edge decision block of the ADCBCM. */
#define SIGPRO_CM_CONFIG_BIT_DETECT_TH_POS                                                               (0U)
#define SIGPRO_CM_CONFIG_BIT_DETECT_TH_MASK                                                              (0x00001FFFU)  /**< rw-Threshold for the "bit" decision block of the ADCBCM.*/

#define SIGPRO_RM_CONFIG                                                                                  0x1CU
#define SIGPRO_RM_CONFIG_BPSK_IQ_MODE_POS                                                                (21U)
#define SIGPRO_RM_CONFIG_BPSK_IQ_MODE_MASK                                                               (0x00E00000U)  /**< rw-Defines strategy of signal processing regarding I and Q channel    000* Both channels (I and Q) are used for signal processing  001 Use only I channel   010 Use only Q channel   011 RFU   100 Use the strongest channel    101 Use the first channel   110-111 RFU*/
#define SIGPRO_RM_CONFIG_BPSK_FILT6_POS                                                                  (20U)
#define SIGPRO_RM_CONFIG_BPSK_FILT6_MASK                                                                 (0x00100000U)  /**< rw-This bit changes the Spike filter for the Phaseshift detection from order 5 to order 6 (default = order 6).*/
#define SIGPRO_RM_CONFIG_RESYNC_EQ_ON_POS                                                                (19U)
#define SIGPRO_RM_CONFIG_RESYNC_EQ_ON_MASK                                                               (0x00080000U)  /**< rw-Resynchronisaton during the SOF for an equal correlation value is done (default = activated).*/
#define SIGPRO_RM_CONFIG_CORR_RESET_ON_POS                                                               (18U)
#define SIGPRO_RM_CONFIG_CORR_RESET_ON_MASK                                                              (0x00040000U)  /**< rw-The correclator is reset at a reset (default = activated).*/
#define SIGPRO_RM_CONFIG_VALID_FILT_OFF_POS                                                              (17U)
#define SIGPRO_RM_CONFIG_VALID_FILT_OFF_MASK                                                             (0x00020000U)  /**< rw-Disables a special filter in BPSK mode. If set to 0, the correlation of 0110 is filtered with the correlation of 1110 and 0111. Otherwise the demodulation is done using the correlation with 0110*/
#define SIGPRO_RM_CONFIG_DATA_BEFORE_MIN_POS                                                             (16U)
#define SIGPRO_RM_CONFIG_DATA_BEFORE_MIN_MASK                                                            (0x00010000U)  /**< rw-Data is received even before the first minimum at the SOF (default: = deactivated).*/
#define SIGPRO_RM_CONFIG_MIN_LEVEL_POS                                                                   (12U)
#define SIGPRO_RM_CONFIG_MIN_LEVEL_MASK                                                                  (0x0000F000U)  /**< rw-Defines the MinLevel of the reception Note: The MinLevel should be higher than the noise level in the system Note: Used for BPSK and Manchester with Subcarrier communication types as MinLevel! */
#define SIGPRO_RM_CONFIG_MIN_LEVEL_P_POS                                                                 (8U)
#define SIGPRO_RM_CONFIG_MIN_LEVEL_P_MASK                                                                (0x00000F00U)  /**< rw-Defines the minlevel for the phaseshift detector unit */
#define SIGPRO_RM_CONFIG_USE_SMALL_EVAL_POS                                                              (7U)
#define SIGPRO_RM_CONFIG_USE_SMALL_EVAL_MASK                                                             (0x00000080U)  /**< rw-Defines the length of the eval periode for the correlator for Manchester subcarrier communication types. */
#define SIGPRO_RM_CONFIG_COLL_LEVEL_POS                                                                  (5U)
#define SIGPRO_RM_CONFIG_COLL_LEVEL_MASK                                                                 (0x00000060U)  /**< rw-Defines how strong a signal must be to be interpreded as a collision for Manchester subcarrier communication types. 00* >12U;5% 01 >25% 10 >50% No Collison */
#define SIGPRO_RM_CONFIG_PRE_FILTER_POS                                                                  (4U)
#define SIGPRO_RM_CONFIG_PRE_FILTER_MASK                                                                 (0x00000010U)  /**< rw-If set to 1 four samples are combined to one data. (average) */
#define SIGPRO_RM_CONFIG_RECT_FILTER_POS                                                                 (3U)
#define SIGPRO_RM_CONFIG_RECT_FILTER_MASK                                                                (0x00000008U)  /**< rw-If set to one; the ADC-values are changed to a more rectangular waveshape. */
#define SIGPRO_RM_CONFIG_SYNC_HIGH_POS                                                                   (2U)
#define SIGPRO_RM_CONFIG_SYNC_HIGH_MASK                                                                  (0x00000004U)  /**< rw-Defines if the bitgrid is fixed at maximum (1) or at a minimum(0) value of the correlation. */
#define SIGPRO_RM_CONFIG_FSK_POS                                                                         (1U)
#define SIGPRO_RM_CONFIG_FSK_MASK                                                                        (0x00000002U)  /**< rw-If set to 1; the demodulation scheme is FSK. */
#define SIGPRO_RM_CONFIG_BPSK_POS                                                                        (0U)
#define SIGPRO_RM_CONFIG_BPSK_MASK                                                                       (0x00000001U)  /**< rw-If set to 1, the demodulation scheme is BPSK.*/

#define RF_STATUS                                                                                         0x1DU
#define RF_STATUS_TRANSCEIVE_STATE_POS                                                                   (24U)
#define RF_STATUS_TRANSCEIVE_STATE_MASK                                                                  (0x07000000U)  /**< r--This registers hold the command bits 0* IDLE state 1 WaitTransmit state 2 Transmitting state 3 WaitReceive state 4 WaitForData state 5 Receiving state 6 LoopBack state 7 reserved*/
#define RF_STATUS_OCPROT_CURRENT_GEAR_POS                                                                (20U)
#define RF_STATUS_OCPROT_CURRENT_GEAR_MASK                                                               (0x00F00000U)  /**< r--OcProt status, current gear*/
#define RF_STATUS_DPLL_ENABLE_POS                                                                        (19U)
#define RF_STATUS_DPLL_ENABLE_MASK                                                                       (0x00080000U)  /**< r--This bit indicates that the DPLL Controler has enabled the DPLL (RF on, RF frequency ok, PLL locked)*/
#define RF_STATUS_CRC_OK_POS                                                                             (18U)
#define RF_STATUS_CRC_OK_MASK                                                                            (0x00040000U)  /**< r--This bit indicates the status of the actual CRC calculation. If 1 the CRC is correct; meaning the CRC register has the value 0 or the residue value if inverted CRC is used. Note: This flag should only by evaluated at the end of a communication */
#define RF_STATUS_TX_RF_STATUS_POS                                                                       (17U)
#define RF_STATUS_TX_RF_STATUS_MASK                                                                      (0x00020000U)  /**< r--If set to 1 this bit indicates that the drivers are turned on; meaning an RF-Field is created by the device itself. */
#define RF_STATUS_RF_DET_STATUS_POS                                                                      (16U)
#define RF_STATUS_RF_DET_STATUS_MASK                                                                     (0x00010000U)  /**< r--If set to 1 this bit indicates that an external RF-Field is detected by the rf level detectors (after digital filtering) */
#define RF_STATUS_RF_ACTIVE_ERROR_CAUSE_POS                                                              (13U)
#define RF_STATUS_RF_ACTIVE_ERROR_CAUSE_MASK                                                             (0x0000E000U)  /**< r--This status flag indicates the cause of an NFC-Active error. Note: This bits are only valid when the RF_ACTIVE_ERROR_IRQ is raised and will be cleared as soon as the bit TX_RF_ENABLE is set to 1. 0* No Error; reset value 1 External field was detected on within TIDT timing 2 External field was detected on within TADT timing 3 No external field was detected within TADT timings 4 Peer did switch off RFField without but no Rx event was raised (no data received) 5 - 7 Reserved */
#define RF_STATUS_RX_ENABLE_POS                                                                          (12U)
#define RF_STATUS_RX_ENABLE_MASK                                                                         (0x00001000U)  /**< r--This bit indicates if the RxDecoder is enalbed. If 1 the RxDecoder was enabled by the Transceive Unit and is now ready for data reception */
#define RF_STATUS_TX_ACTIVE_POS                                                                          (11U)
#define RF_STATUS_TX_ACTIVE_MASK                                                                         (0x00000800U)  /**< r--This bit indicates activity of the TxEncoder. If 1 a transmission is ongoing, otherwise the TxEncoder is in idle state.*/
#define RF_STATUS_RX_ACTIVE_POS                                                                          (10U)
#define RF_STATUS_RX_ACTIVE_MASK                                                                         (0x00000400U)  /**< r--This bit indicates activity of the RxDecoder. If 1 a data reception is ongoing; otherwise the RxDecoder is in idle state. */
#define RF_STATUS_AGC_VALUE_POS                                                                          (0U)
#define RF_STATUS_AGC_VALUE_MASK                                                                         (0x000003FFU)  /**< r--Current value of the AGC*/

#define AGC_CONFIG                                                                                        0x1EU
#define AGC_CONFIG_AGC_VREF_SEL_POS                                                                      (14U)
#define AGC_CONFIG_AGC_VREF_SEL_MASK                                                                     (0x0000C000U)  /**< rw-Select the comparison reference voltage*/
#define AGC_CONFIG_AGC_TIME_CONSTANT_POS                                                                 (4U)
#define AGC_CONFIG_AGC_TIME_CONSTANT_MASK                                                                (0x00003FF0U)  /**< rw-Time constant for the AGC update. An AGC period is given by (AGC_TIME_CONSTANT+1U ) * 13.56MHz*/
#define AGC_CONFIG_AGC_INPUT_SEL_POS                                                                     (3U)
#define AGC_CONFIG_AGC_INPUT_SEL_MASK                                                                    (0x00000008U)  /**< rw-Selects the AGC value to be loaded into the AGC and the scource for manual mode: 0* CLIF_AGC_INPUT_REG.AGC_CM_VALUE 1 CLIF_AGC_INPUT_REG.AGC_RM_VALUE*/
#define AGC_CONFIG_AGC_LOAD_POS                                                                          (2U)
#define AGC_CONFIG_AGC_LOAD_MASK                                                                         (0x00000004U)  /**< -x-If set; one AGC control value is loaded from CLIF_AGC_VALUE_REG .into the internal AGC register (depending on AGC_INPUT_SEL). */
#define AGC_CONFIG_AGC_MODE_SEL_POS                                                                      (1U)
#define AGC_CONFIG_AGC_MODE_SEL_MASK                                                                     (0x00000002U)  /**< rw-Selects the operation mode of the AGC: 0* Rx-Divider is controlled by the register CLIF_AGC_INPUT_REG.AGC_CM_VALUE or CLIF_AGC_INPUT_REG.AGC_RM_VALUE (dependent on AGC_INPUT_SEL) 1 Rx-Divider value is controlled by the AGC.*/
#define AGC_CONFIG_AGC_MODE_ENABLE_POS                                                                   (0U)
#define AGC_CONFIG_AGC_MODE_ENABLE_MASK                                                                  (0x00000001U)  /**< rw-If set, the AGC is enabled. If not set, the Rx-Divider is controlled by either the internal AGC register or a register value (dependent on AGC_MODE_SEL).*/

#define AGC_VALUE                                                                                         0x1FU
#define AGC_VALUE_AGC_RM_VALUE_POS                                                                       (10U)
#define AGC_VALUE_AGC_RM_VALUE_MASK                                                                      (0x000FFC00U)  /**< rw-Static AGC value used for reader mode */
#define AGC_VALUE_AGC_CM_VALUE_POS                                                                       (0U)
#define AGC_VALUE_AGC_CM_VALUE_MASK                                                                      (0x000003FFU)  /**< rw-Static AGC value used for card mode*/

#define ANA_RF_CONTROL_TX                                                                                 0x20U
#define ANA_RF_CONTROL_TX_TX_ALM_TYPE_SELECT_POS                                                         (26U)
#define ANA_RF_CONTROL_TX_TX_ALM_TYPE_SELECT_MASK                                                        (0x04000000U)  /**< rw-Defines which ALM type is used. If set to 1 Dual-Loop-ALM is used, default (0) is Single-Loop-ALM.*/
#define ANA_RF_CONTROL_TX_TX_CW_AMPLITUDE_ALM_CM_POS                                                     (24U)
#define ANA_RF_CONTROL_TX_TX_CW_AMPLITUDE_ALM_CM_MASK                                                    (0x03000000U)  /**< rw-set amplitude of unmodulated carrier at card mode*/
#define ANA_RF_CONTROL_TX_TX_RESIDUAL_CARRIER_OV_PREV_POS                                                (19U)
#define ANA_RF_CONTROL_TX_TX_RESIDUAL_CARRIER_OV_PREV_MASK                                               (0x00F80000U)  /**< rw-Defines the value for the residual carrier for the period the overshoot prevention pattern is active.*/
#define ANA_RF_CONTROL_TX_TX_CW_TO_MAX_ALM_CM_POS                                                        (18U)
#define ANA_RF_CONTROL_TX_TX_CW_TO_MAX_ALM_CM_MASK                                                       (0x00040000U)  /**< rw-TX HI output is the maximum voltage obtainable from charge pump (CM setting); if set to 1 -> TX_CW_AMPLITUDE_CM is overruled.*/
#define ANA_RF_CONTROL_TX_TX_RESIDUAL_CARRIER_POS                                                        (13U)
#define ANA_RF_CONTROL_TX_TX_RESIDUAL_CARRIER_MASK                                                       (0x0003E000U)  /**< rw-set residual carrier (0=100%, 1F = 0%)*/
#define ANA_RF_CONTROL_TX_TX_SET_BYPASS_SC_SHAPING_POS                                                   (12U)
#define ANA_RF_CONTROL_TX_TX_SET_BYPASS_SC_SHAPING_MASK                                                  (0x00001000U)  /**< rw-Bypasses switched capacitor shaping of the Transmitter Signal*/
#define ANA_RF_CONTROL_TX_TX_SET_SLEW_SHUNTREG_POS                                                       (8U)
#define ANA_RF_CONTROL_TX_TX_SET_SLEW_SHUNTREG_MASK                                                      (0x00000F00U)  /**< rw-Set slew rate for shunt regulator*/
#define ANA_RF_CONTROL_TX_TX_SET_TAU_MOD_FALLING_POS                                                     (4U)
#define ANA_RF_CONTROL_TX_TX_SET_TAU_MOD_FALLING_MASK                                                    (0x000000F0U)  /**< rw-Transmitter TAU setting for falling edge of modulation shape. In AnalogControl module the output signal is switched with the tx_envelope. Only valid is TX_SET_SINGLE_CP_MODE is set*/
#define ANA_RF_CONTROL_TX_TX_SET_TAU_MOD_RISING_POS                                                      (0U)
#define ANA_RF_CONTROL_TX_TX_SET_TAU_MOD_RISING_MASK                                                     (0x0000000FU)  /**< rw-Transmitter TAU setting for rising edge of modulation shape. In AnalogControl module the output signal is switched with the tx_envelope. Only valid is TX_SET_SINGLE_CP_MODE is set*/

#define ANA_RF_CONTROL_TX_CLK                                                                             0x21U
#define ANA_RF_CONTROL_TX_CLK_TX_ALM_ENABLE_POS                                                          (18U)
#define ANA_RF_CONTROL_TX_CLK_TX_ALM_ENABLE_MASK                                                         (0x00040000U)  /**< rw-If set to 1 ALM is used for transmission in card mode*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_TX_SEL_SLALM_POS                                                     (17U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_TX_SEL_SLALM_MASK                                                    (0x00020000U)  /**< rw-Selects DPLL*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_PD_RF_27M12_POS                                                      (16U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_PD_RF_27M12_MASK                                                     (0x00010000U)  /**< rw-Power down RF clock generation*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_ADC_SEL_RF_POS                                                       (15U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_ADC_SEL_RF_MASK                                                      (0x00008000U)  /**< rw-Select RF clock for ADC from DLL or PLL*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_TX_SEL_RF_POS                                                        (14U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_TX_SEL_RF_MASK                                                       (0x00004000U)  /**< rw-Select DLL clock for TX (instead of PLL clock)*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_CONFIG_DLL_ALM_POS                                                   (11U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_CONFIG_DLL_ALM_MASK                                                  (0x00003800U)  /**< rw-Select DLL clock phase*/
#define ANA_RF_CONTROL_TX_CLK_TX_CLK_MODE_OVUN_PREV_POS                                                  (8U)
#define ANA_RF_CONTROL_TX_CLK_TX_CLK_MODE_OVUN_PREV_MASK                                                 (0x00000700U)  /**< rw-Defines the TX clockmode for the period the overshoot/undershoot prevention is active*/
#define ANA_RF_CONTROL_TX_CLK_TX2_INV_RM_POS                                                             (7U)
#define ANA_RF_CONTROL_TX_CLK_TX2_INV_RM_MASK                                                            (0x00000080U)  /**< rw-If 1 -> TX output is inverted (clk_13m56_n is used); 0 -> clk_13m56 is used*/
#define ANA_RF_CONTROL_TX_CLK_TX2_INV_CM_POS                                                             (6U)
#define ANA_RF_CONTROL_TX_CLK_TX2_INV_CM_MASK                                                            (0x00000040U)  /**< rw-If 1 -> TX output is inverted (clk_13m56_n is used); 0 -> clk_13m56 is used*/
#define ANA_RF_CONTROL_TX_CLK_TX1_INV_RM_POS                                                             (5U)
#define ANA_RF_CONTROL_TX_CLK_TX1_INV_RM_MASK                                                            (0x00000020U)  /**< rw-If 1 -> TX output is inverted (clk_13m56_n is used); 0 -> clk_13m56 is used*/
#define ANA_RF_CONTROL_TX_CLK_TX1_INV_CM_POS                                                             (4U)
#define ANA_RF_CONTROL_TX_CLK_TX1_INV_CM_MASK                                                            (0x00000010U)  /**< rw-If 1 -> TX output is inverted (clk_13m56_n is used); 0 -> clk_13m56 is used*/
#define ANA_RF_CONTROL_TX_CLK_TX_CLK_MODE_RM_POS                                                         (1U)
#define ANA_RF_CONTROL_TX_CLK_TX_CLK_MODE_RM_MASK                                                        (0x0000000EU)  /**< rw-TX clockmode*/
#define ANA_RF_CONTROL_TX_CLK_CLOCK_ENABLE_DPLL_POS                                                      (0U)
#define ANA_RF_CONTROL_TX_CLK_CLOCK_ENABLE_DPLL_MASK                                                     (0x00000001U)  /**< rw-Enables the DPLL*/

#define ANA_RF_CONTROL_RX                                                                                 0x22U
#define ANA_RF_CONTROL_RX_CM_MILLER_SENS_POS                                                             (6U)
#define ANA_RF_CONTROL_RX_CM_MILLER_SENS_MASK                                                            (0x000000C0U)  /**< rw-Configuration bits for reference level of Miller demodulator*/
#define ANA_RF_CONTROL_RX_RX_MIXER_CONTROL_POS                                                           (4U)#define ANA_RF_CONTROL_RX_RX_MIXER_CONTROL_MASK                                                          (0x00000030U)  /**< rw-00,11 ... power down both mixer 01... reader mode mixer 10... card mode mixer, */
#define ANA_RF_CONTROL_RX_RX_HPCF_POS                                                                    (2U)
#define ANA_RF_CONTROL_RX_RX_HPCF_MASK                                                                   (0x0000000CU)  /**< rw-Lower Corner Frequency: 00->45kHz, 01->85kHz, 10->150kHz, 11->250kHz*/
#define ANA_RF_CONTROL_RX_RX_GAIN_POS                                                                    (0U)
#define ANA_RF_CONTROL_RX_RX_GAIN_MASK                                                                   (0x00000003U)  /**< rw-Gain Adjustment BBA: 00->33dBU, 01->40dBU, 10->50dBU, 11->57dB*/

#define LD_CONTROL                                                                                        0x23U
#define LD_CONTROL_CM_PD_NFC_DET_POS                                                                     (14U)
#define LD_CONTROL_CM_PD_NFC_DET_MASK                                                                    (0x00004000U)  /**< rw-Power Down NFC level detector*/
#define LD_CONTROL_RFDET_SOURCE_SEL_POS                                                                  (12U)
#define LD_CONTROL_RFDET_SOURCE_SEL_MASK                                                                 (0x00003000U)  /**< rw-Select the source for RF-Field detection; 0* -> NFC-Level detector indication signal is used; 1 -> RF-Level detector indication signal is used 2; -> NFC- and RF-Level detector indication signal is used 3; -> Override - RF-Field detected is emulated*/
#define LD_CONTROL_CM_RFL_NFC_POS                                                                        (8U)
#define LD_CONTROL_CM_RFL_NFC_MASK                                                                       (0x00000F00U)  /**< rw-Programming of detection level*/
#define LD_CONTROL_RFLD_REF_LO_POS                                                                       (4U)
#define LD_CONTROL_RFLD_REF_LO_MASK                                                                      (0x000000F0U)  /**< rw-Higher Reference Value for RF Level Detector*/
#define LD_CONTROL_RFLD_REF_HI_POS                                                                       (0U)
#define LD_CONTROL_RFLD_REF_HI_MASK                                                                      (0x0000000FU)  /**< rw-Lower Reference Value for RF Level Detector*/

#define SYSTEM_STATUS                                                                                     0x24U
#define SYSTEM_STATUS_PARAMETER_ERROR_POS                                                                (8U)
#define SYSTEM_STATUS_PARAMETER_ERROR_MASK                                                               (0x00000100U)  /**< r--*/
#define SYSTEM_STATUS_SYNTAX_ERROR_POS                                                                   (7U)
#define SYSTEM_STATUS_SYNTAX_ERROR_MASK                                                                  (0x00000080U)  /**< r--*/
#define SYSTEM_STATUS_SEMANTIC_ERROR_POS                                                                 (6U)
#define SYSTEM_STATUS_SEMANTIC_ERROR_MASK                                                                (0x00000040U)  /**< r--*/
#define SYSTEM_STATUS_STBY_PREVENT_RFLD_POS                                                              (5U)
#define SYSTEM_STATUS_STBY_PREVENT_RFLD_MASK                                                             (0x00000020U)  /**< r--*/
#define SYSTEM_STATUS_BOOT_TEMP_POS                                                                      (4U)
#define SYSTEM_STATUS_BOOT_TEMP_MASK                                                                     (0x00000010U)  /**< r--*/
#define SYSTEM_STATUS_BOOT_SOFT_RESET_POS                                                                (3U)
#define SYSTEM_STATUS_BOOT_SOFT_RESET_MASK                                                               (0x00000008U)  /**< r--*/
#define SYSTEM_STATUS_BOOT_WUC_POS                                                                       (2U)
#define SYSTEM_STATUS_BOOT_WUC_MASK                                                                      (0x00000004U)  /**< r--*/
#define SYSTEM_STATUS_BOOT_RFLD_POS                                                                      (1U)
#define SYSTEM_STATUS_BOOT_RFLD_MASK                                                                     (0x00000002U)  /**< r--*/
#define SYSTEM_STATUS_BOOT_POR_POS                                                                       (0U)
#define SYSTEM_STATUS_BOOT_POR_MASK                                                                      (0x00000001U)  /**< r--*/

#define TEMP_CONTROL                                                                                      0x25U
#define TEMP_CONTROL_TEMP_ENABLE_HYST_POS                                                                (3U)
#define TEMP_CONTROL_TEMP_ENABLE_HYST_MASK                                                               (0x00000008U)  /**< rw-Enable Hystere of Temperature Sensor*/
#define TEMP_CONTROL_TEMP_ENABLE_POS                                                                     (2U)
#define TEMP_CONTROL_TEMP_ENABLE_MASK                                                                    (0x00000004U)  /**< rw-Enable Temp Sensor*/
#define TEMP_CONTROL_TEMP_DELTA_POS                                                                      (0U)
#define TEMP_CONTROL_TEMP_DELTA_MASK                                                                     (0x00000003U)  /**< rw-selects temperatur value*/

#define AGC_REF_CONFIG                                                                                    0x26U
#define AGC_REF_CONFIG_AGC_GEAR_POS                                                                      (10U)
#define AGC_REF_CONFIG_AGC_GEAR_MASK                                                                     (0x00003C00U)  /**< rw-*/
#define AGC_REF_CONFIG_AGC_VALUE_POS                                                                     (0U)
#define AGC_REF_CONFIG_AGC_VALUE_MASK                                                                    (0x000003FFU)  /**< rw-*/

#define TXOCP_CONTROL                                                                                     0x27U
#define TXOCP_CONTROL_TX_GSN_CW_CM_POS                                                                   (16U)
#define TXOCP_CONTROL_TX_GSN_CW_CM_MASK                                                                  (0x000F0000U)  /**< rw-gsn setting at continuous wave in card mode*/
#define TXOCP_CONTROL_TX_GSN_MOD_CM_POS                                                                  (12U)
#define TXOCP_CONTROL_TX_GSN_MOD_CM_MASK                                                                 (0x0000F000U)  /**< rw-gsn setting at continuous wave in card mode*/
#define TXOCP_CONTROL_TX_GSN_MOD_RM_POS                                                                  (8U)
#define TXOCP_CONTROL_TX_GSN_MOD_RM_MASK                                                                 (0x00000F00U)  /**< rw-gsn setting at continuous wave in reader mode*/
#define TXOCP_CONTROL_TX_GSN_CW_RM_POS                                                                   (4U)
#define TXOCP_CONTROL_TX_GSN_CW_RM_MASK                                                                  (0x000000F0U)  /**< rw-gsn setting at continuous wave in reader mode*/
#define TXOCP_CONTROL_TX_CW_TO_MAX_RM_POS                                                                (3U)
#define TXOCP_CONTROL_TX_CW_TO_MAX_RM_MASK                                                               (0x00000008U)  /**< rw-TX HI output is the maximum voltage obtainable from charge pump (RM setting); if set to 1 -> TX_CW_AMPLITUDE_RM is overruled.*/
#define TXOCP_CONTROL_TX_CW_AMPLITUDE_RM_POS                                                             (1U)
#define TXOCP_CONTROL_TX_CW_AMPLITUDE_RM_MASK                                                            (0x00000006U)  /**< rw-set amplitude of unmodulated carrier at reader mode*/
#define TXOCP_CONTROL_TX_CW_AMP_REF2TVDD_POS                                                             (0U)
#define TXOCP_CONTROL_TX_CW_AMP_REF2TVDD_MASK                                                            (0x00000001U)  /**< rw-If set to 1 the reference of the unmodulated carrier is defined relative to TVDDNote: Do not change in normal operation*/

#define EMD_CONTROL                                                                                       0x28U
#define EMD_CONTROL_EMD_TRANSMISSION_TIMER_USED_POS                                                      (8U)
#define EMD_CONTROL_EMD_TRANSMISSION_TIMER_USED_MASK                                                     (0x00000300U)  /**< rw-Timer used for RF communication.
00 ... Timer0
01 ... Timer1
10 ... Timer 2
11 ... RFU*/
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_B_POS                                         (7U)
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_B_MASK                                        (0x00000080U)  /**< rw-Missing CRC treated as protocol error in case of Type B based communication*/
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_A_POS                                         (6U)
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_A_MASK                                        (0x00000040U)  /**< rw-Missing CRC treated as protocol error in case of Type A based communication*/
#define EMD_CONTROL_EMD_NOISE_BYTES_THRESHOLD_POS                                                        (2U)
#define EMD_CONTROL_EMD_NOISE_BYTES_THRESHOLD_MASK                                                       (0x0000003CU)  /**< rw-Defines the threshold under which transmission errors are treated as noise. Note: CRC bytes are NOT included/counted!*/
#define EMD_CONTROL_EMD_TRANSMISSION_ERROR_ABOVE_NOISE_THRESHOLD_IS_NO_EMD_POS                           (1U)
#define EMD_CONTROL_EMD_TRANSMISSION_ERROR_ABOVE_NOISE_THRESHOLD_IS_NO_EMD_MASK                          (0x00000002U)  /**< rw-Transmission errors with received byte length >= EMD_NOISE_BYTES_THRESHOLD is never treated as EMD (EMVCo 2.5 standard)*/
#define EMD_CONTROL_EMD_ENABLE_POS                                                                       (0U)
#define EMD_CONTROL_EMD_ENABLE_MASK                                                                      (0x00000001U)  /**< rw-EMD handling enabled*/

#define FELICA_EMD_CONTROL                                                                                0x43U
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_BYTE_VALUE_POS                                                  (24UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_BYTE_VALUE_MASK                                                 (0xFF000000UL)  /**< rw-RC byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MAX_POS                                                (16UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MAX_MASK                                               (0x00FF0000UL)  /**< rw-Maximum Legth byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MIN_POS                                                (8UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MIN_MASK                                               (0x0000FF00UL)  /**< rw-Minimum Legth byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_RESERVED_POS                                                                  (5UL)
#define FELICA_EMD_CONTROL_RESERVED_MASK                                                                 (0x000000E0UL)  /**< r--Reserved */
#define FELICA_EMD_CONTROL_FELICA_EMD_INTEGRITY_ERR_CHECK_ENABLE_POS                                     (4UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_INTEGRITY_ERR_CHECK_ENABLE_MASK                                    (0x00000010UL)  /**< rw-FeliCa EMD handling enabled when integrity error is set*/
#define FELICA_EMD_CONTROL_FELICA_EMD_PROTOCOL_ERR_CHECK_ENABLE_POS                                      (3UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_PROTOCOL_ERR_CHECK_ENABLE_MASK                                     (0x00000008UL)  /**< rw-FeliCa EMD handling enabled when protocol error is set*/
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_CHECK_ENABLE_POS                                                (2UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_CHECK_ENABLE_MASK                                               (0x00000004UL)  /**< rw-RC byte check enabled for FeliCa EMD handling */
#define FELICA_EMD_CONTROL_FELICA_EMD_LEN_CHECK_ENABLE_POS                                               (1UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LEN_CHECK_ENABLE_MASK                                              (0x00000002UL)  /**< rw-Length byte check enabled for FeliCa EMD handling*/
#define FELICA_EMD_CONTROL_FELICA_EMD_ENABLE_POS                                                         (0UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_ENABLE_MASK                                                        (0x00000001UL)  /**< rw-FeliCa EMD handling enabled*/

#define ANA_PBF_CONTROL                                                                                   0x29U
#define ANA_PBF_CONTROL_PBF_INVERT_ON_TXACTIVE_POS                                                       (7U)
#define ANA_PBF_CONTROL_PBF_INVERT_ON_TXACTIVE_MASK                                                      (0x00000080U)  /**< rw-If set to 1, the PbF short interface in card mode is inverted when tx_active is asserted (i.e. while transmission). Note: this bit is only valid in card mode. Note: if it BF_ALM_AUTO_SWITCH_ENABLE is set this setting is ignored*/
#define ANA_PBF_CONTROL_PBF_ALM_AUTO_SWITCH_ENABLE_POS                                                   (6U)
#define ANA_PBF_CONTROL_PBF_ALM_AUTO_SWITCH_ENABLE_MASK                                                  (0x00000040U)  /**< rw-If set to 1, the PBF settting for ALM is switch automatically by HW. By default for ALM the pbf_short and pbf_mod uses the same settings as for PLM.*/
#define ANA_PBF_CONTROL_PBF_ALM_FW_RESET_POS                                                             (5U)
#define ANA_PBF_CONTROL_PBF_ALM_FW_RESET_MASK                                                            (0x00000020U)  /**< -x-If set to 1 the PBF setting for ALM is reset to its initial receive configuration*/
#define ANA_PBF_CONTROL_PBF_SHORT_SELECT_RM_POS                                                          (4U)
#define ANA_PBF_CONTROL_PBF_SHORT_SELECT_RM_MASK                                                         (0x00000010U)  /**< rw-Selects the control of the PbF modulation interface in reader mode*/
#define ANA_PBF_CONTROL_PBF_SHORT_SELECT_POS                                                             (2U)
#define ANA_PBF_CONTROL_PBF_SHORT_SELECT_MASK                                                            (0x0000000CU)  /**< rw-Selects the control of the PbF short interface in cardmode for PLM; in reader mode and ALM the analog control signals are switched by digital logic.   00b Constant 0 (PbF open)    01b Constant 1 (PbF short)   10b TxEnvelope used (idle = 1, modulation = 0)   11b Inverted TxEnvelope used (idle = 0, modulation = 1)*/
#define ANA_PBF_CONTROL_PBF_MOD_SELECT_POS                                                               (0U)
#define ANA_PBF_CONTROL_PBF_MOD_SELECT_MASK                                                              (0x00000003U)  /**< rw-Selects the control of the PbF modulation interface in cardmode for PLM; in reader mode and ALM the analog control signals are switched by digital logic.   00b Constant 0 (No modulation on PbF mod )   01b Constant 1 (modulation on PbF mod)   10b TxEnvelope used (idle = 1, modulation = 0)   11b Inverted TxEnvelope used (idle = 0, modulation = 1)*/

#define TRANSCEIVER_CONFIG_RFU                                                                            0x2AU
#define TRANSCEIVER_CONFIG_RFU_TX_WAIT_RFON_ENABLE_POS                                                   (2U)
#define TRANSCEIVER_CONFIG_RFU_TX_WAIT_RFON_ENABLE_MASK                                                  (0x00000004U)  /**< rw-If set to 1 the TxWait guard time will be started when the own RF-Field is switched on. Note: In default configuration the TxWait guard time is started at the end of reception. This feature is intended to be used for NFC-Active communication */
#define TRANSCEIVER_CONFIG_RFU_RX_WAIT_RFON_ENABLE_POS                                                   (1U)
#define TRANSCEIVER_CONFIG_RFU_RX_WAIT_RFON_ENABLE_MASK                                                  (0x00000002U)  /**< rw-If set to 1 the RxWait guard time will be started when the own RF-Field is switched off and an external RF-Field was detected. Note: In default configuration the RxWait guard time is started at the end of transmission. This feature is intended to be used for NFC-Active communication */
#define TRANSCEIVER_CONFIG_RFU_TX_MILLER_SYNC_ENABLE_POS                                                 (0U)
#define TRANSCEIVER_CONFIG_RFU_TX_MILLER_SYNC_ENABLE_MASK                                                (0x00000001U)  /**< rw-If set to 1 guard time is synchronized to miller envelope for transmission. Used for AGC.*/

#define AGC_CONFIG0_RFU                                                                                   0x2BU
#define AGC_CONFIG0_RFU_AGC_TESTMODE_ENABLE_POS                                                          (17U)
#define AGC_CONFIG0_RFU_AGC_TESTMODE_ENABLE_MASK                                                         (0x00020000U)  /**< rw-If this bit is set, then the AGC value is increased/decreased by signals form the testbus. Note that this bit is shall never be set by FW and is intended only for test-purposes.*/
#define AGC_CONFIG0_RFU_MILLER_BG_FREEZE_VALUE_POS                                                       (10U)
#define AGC_CONFIG0_RFU_MILLER_BG_FREEZE_VALUE_MASK                                                      (0x0001FC00U)  /**< rw-This bit is primarily intended to achieve AGC operation in RM for ISO14443 type A 106kBd (AGC_FREEZE_ON_MILLER_BG). However, this field is also re-used for the back-up modes AGC_FREEZE_ON_RX_WAIT and AGC_FAST_MODE_RX_WAIT.*/
#define AGC_CONFIG0_RFU_AGC_THRESHOLD_POS                                                                (1U)
#define AGC_CONFIG0_RFU_AGC_THRESHOLD_MASK                                                               (0x000003FEU)  /**< rw-Defines the threshold value for the AGC. Value must not be larger than AGC_TIME_CONSTANT>>1 - 1.*/
#define AGC_CONFIG0_RFU_AGC_SYNC_COMP_DISABLE_POS                                                        (0U)
#define AGC_CONFIG0_RFU_AGC_SYNC_COMP_DISABLE_MASK                                                       (0x00000001U)  /**< rw-If set, the synchronization register at the input of the digital controller to synchronize agc_comp_ai is by-passed.*/

#define AGC_CONFIG1_RFU                                                                                   0x2CU
#define AGC_CONFIG1_RFU_AGC_RF_DETECT_SEL_POS                                                            (29U)
#define AGC_CONFIG1_RFU_AGC_RF_DETECT_SEL_MASK                                                           (0x60000000U)  /**< rw-Selects/enables the RF-off detection during transmission (intended for SL-ALM). If enabled, the AGC reference voltage is set to its minimum (0x00)*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_TX_SWITCH_VALUE_POS                                                   (28U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_TX_SWITCH_VALUE_MASK                                                  (0x10000000U)  /**< rw-If set, the internal AGC is frozen during transmission and the output value is switched with the tx-envelope between the CM value (env=0) (CLIF_AGC_INPUT_REG.AGC_CM_VALUE) and the RM value (env=1) (CLIF_AGC_INPUT_REG.AGC_RM_VALUE).*/
#define AGC_CONFIG1_RFU_AGC_MAX_TXRFOFF_ENABLE_POS                                                       (27U)
#define AGC_CONFIG1_RFU_AGC_MAX_TXRFOFF_ENABLE_MASK                                                      (0x08000000U)  /**< rw-If set, the AGC is forced to maximum value for a given time period after the devices' RF-Field was switched off.*/
#define AGC_CONFIG1_RFU_AGC_MASK_DURATION_POS                                                            (21U)
#define AGC_CONFIG1_RFU_AGC_MASK_DURATION_MASK                                                           (0x07E00000U)  /**< rw-Number of RF cycles the AGC active signal is set after each AGC update. In RM the signal masks out the preamble detection in the SigPro RM. In CM the signal masks out the data signals in the SigPro during Fast-Mode. In case AGC_MASK_DURATION is set to zero, the agc_active_o signal is not set.*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_RESET_ON_RF_OFF_POS                                                   (20U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_RESET_ON_RF_OFF_MASK                                                  (0x00100000U)  /**< rw-If set, the AGC is reset to zero when an external field vanishes and frozen (as long as no external field is present).*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_MILLER_BG_POS                                                      (19U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_MILLER_BG_MASK                                                     (0x00080000U)  /**< rw-If set, the AGC is frozen, when the bit-grid value is below the value in MILLER_BG_FREEZE_VALUE.*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_SOF_DETECTED_POS                                                   (18U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_SOF_DETECTED_MASK                                                  (0x00040000U)  /**< rw-If rm_startbit_detected_i is set (SOF detected by the SigPro), the AGC is frozen.*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_DPRESENT_POS                                                       (17U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_DPRESENT_MASK                                                      (0x00020000U)  /**< rw-If rm_dpresent_i is set (preamble detected by the SigPro), the AGC is frozen.*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_DPRESENT_NEXT_POS                                                  (16U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_DPRESENT_NEXT_MASK                                                 (0x00010000U)  /**< rw-If rm_dpresent_next_i is set (preamble detected by the SigPro), the AGC is frozen.*/
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_RXWAIT_OFF_POS                                                     (15U)
#define AGC_CONFIG1_RFU_AGC_FREEZE_ON_RXWAIT_OFF_MASK                                                    (0x00008000U)  /**< rw-If rx_wait_i is not set, the AGC is frozen.*/
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_DURATION_POS                                                       (5U)
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_DURATION_MASK                                                      (0x00007FE0U)  /**< rw-Duration of the Fast-Mode in multiples of RF clock cycles (clk13).*/
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_RFON_EXT_POS                                                       (4U)
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_RFON_EXT_MASK                                                      (0x00000010U)  /**< rw-If set the AGC enters fast mode (HW sets AGC_FAST_MODE_ENABLE) when an external RF field is detected.*/
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_TX_OVER_POS                                                        (3U)
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_TX_OVER_MASK                                                       (0x00000008U)  /**< rw-If set to one, the AGC fast-mode (i.e. time-constant "1" and step size given by AGC_FAST_MODE_STEP) is enabled for AGC_FAST_MODE_DURATION clk13 cycles after transmission*/
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_STEP_POS                                                           (1U)
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_STEP_MASK                                                          (0x00000006U)  /**< rw-Step size of the AGC in fast-mode: 0* Step by one 1 Step by two 2 Step by three 3 Step by four*/
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_ENABLE_POS                                                         (0U)
#define AGC_CONFIG1_RFU_AGC_FAST_MODE_ENABLE_MASK                                                        (0x00000001U)  /**< rw-If set to one, the AGC fast-mode (i.e. time-constant "1" and step size given by AGC_FAST_MODE_STEP) is enabled for AGC_FAST_MODE_DURATION clk13 cycles.*/

#define ANA_RX_RFU                                                                                        0x2DU
#define ANA_RX_RFU_RX_VMID_RSEL_POS                                                                      (2U)
#define ANA_RX_RFU_RX_VMID_RSEL_MASK                                                                     (0x0000000CU)  /**< rw-Selection of resistance between VDD and VSS: 00->1.4k; 01->2.8k, 10->5.7k, 11->11k*/
#define ANA_RX_RFU_RX_SHORT_MIX2ADC_POS                                                                  (1U)
#define ANA_RX_RFU_RX_SHORT_MIX2ADC_MASK                                                                 (0x00000002U)  /**< rw-Short Mixer to ADC, I and Q*/
#define ANA_RX_RFU_RX_OC_ENABLE_POS                                                                      (0U)
#define ANA_RX_RFU_RX_OC_ENABLE_MASK                                                                     (0x00000001U)  /**< rw-Offset Compensation BBA*/

#define ANA_RX_POWER_CONTROL_RFU                                                                          0x2EU
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_MILLER_POS                                                        (9U)
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_MILLER_MASK                                                       (0x00000200U)  /**< rw-Power Down CM reference based miller demodulator*/
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_CLKREC_POS                                                        (8U)
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_CLKREC_MASK                                                       (0x00000100U)  /**< rw-Power Down CM clock recovery*/
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_DLL_POS                                                           (7U)
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_DLL_MASK                                                          (0x00000080U)  /**< rw-Power Down CM delay lock loop (DLL)*/
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_DLL_CP_POS                                                        (6U)
#define ANA_RX_POWER_CONTROL_RFU_CM_PD_DLL_CP_MASK                                                       (0x00000040U)  /**< rw-Power Down CM charge pump (inside DLL)*/
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_ADC_Q_POS                                                         (5U)
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_ADC_Q_MASK                                                        (0x00000020U)  /**< rw-Power Down ADC Q-channel*/
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_ADC_I_POS                                                         (4U)
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_ADC_I_MASK                                                        (0x00000010U)  /**< rw-Power Down ADC I-channel*/
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_BIAS_POS                                                      (3U)
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_BIAS_MASK                                                     (0x00000008U)  /**< rw-Power Down Baseband Amplifier Bias Block*/
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_Q_POS                                                         (2U)
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_Q_MASK                                                        (0x00000004U)  /**< rw-Power Down Baseband Amplifier Q-Channel*/
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_I_POS                                                         (1U)
#define ANA_RX_POWER_CONTROL_RFU_RX_PD_BBA_I_MASK                                                        (0x00000002U)  /**< rw-Power Down Baseband Amplifier I-Channel*/
#define ANA_RX_POWER_CONTROL_RFU_RX_SHORT_MIX2BBA_ST2_POS                                                (0U)
#define ANA_RX_POWER_CONTROL_RFU_RX_SHORT_MIX2BBA_ST2_MASK                                               (0x00000001U)  /**< rw-Short Mixer to BBA Stage 2; I only*/

#define ANA_TX_CONTROL_RFU                                                                                0x2FU
#define ANA_TX_CONTROL_RFU_TX_PD_CLK_GEN_POS                                                             (1U)
#define ANA_TX_CONTROL_RFU_TX_PD_CLK_GEN_MASK                                                            (0x00000002U)  /**< rw-TX clock generation disabled and thus also envelope synchronization to clk is disabled.*/
#define ANA_TX_CONTROL_RFU_TX_ALM_BPSK_ENABLE_POS                                                        (0U)
#define ANA_TX_CONTROL_RFU_TX_ALM_BPSK_ENABLE_MASK                                                       (0x00000001U)  /**< rw-If set to 1, modulation for ALM is done with BPSK scheme on carrier*/

#define DPLL_CONTROL_RFU                                                                                  0x30U
#define DPLL_CONTROL_RFU_SINGLE_LOOP_ALM_ENABLE_POS                                                      (8U)
#define DPLL_CONTROL_RFU_SINGLE_LOOP_ALM_ENABLE_MASK                                                     (0x00000100U)  /**< rw-Enables the DPLL controller*/
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_TX_ENV_POS                                                       (6U)
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_TX_ENV_MASK                                                      (0x000000C0U)  /**< rw-Freeze when activly modulating (env for ALM == 1U); 'b00 = disabled, 'b01 = unfreeze as early as possible after modulation, 'b10 = unfreeze with additional medium delay after modulation (additional 3 cycles), 'b11 = do not unfreeze during subcarrier modulation (except phase  shift)*/
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_FW_POS                                                           (5U)
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_FW_MASK                                                          (0x00000020U)  /**< rw-Freeze initiated by FW, intended to change the gear shift settings*/
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_ADCBCM_ACTIVE_POS                                                (4U)
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_ADCBCM_ACTIVE_MASK                                               (0x00000010U)  /**< rw-Freezes the DPLL frequency already when the ADC based card-mode detects an envelope*/
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_RX_POS                                                           (3U)
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_RX_MASK                                                          (0x00000008U)  /**< rw-Freezes the DPLL frequency in RECEIVE state (as soon as the start of reception is detected)*/
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_WAITDATA_POS                                                     (2U)
#define DPLL_CONTROL_RFU_DPLL_FREEZE_ON_WAITDATA_MASK                                                    (0x00000004U)  /**< rw-Freezes the DPLL frequency in WAIT_FOR_DATA state*/
#define DPLL_CONTROL_RFU_DPLL_RESTART_GEAR_TXWAIT_POS                                                    (1U)
#define DPLL_CONTROL_RFU_DPLL_RESTART_GEAR_TXWAIT_MASK                                                   (0x00000002U)  /**< rw-Command to restart the gear shifting at the beginning of Tx-Wait from gear zero*/
#define DPLL_CONTROL_RFU_DPLL_RESTART_GEAR_POS                                                           (0U)
#define DPLL_CONTROL_RFU_DPLL_RESTART_GEAR_MASK                                                          (0x00000001U)  /**< rw-Command to restart the gear shifting from gear zero with initial settings from CLIF_DPLL_INIT_REG*/

#define DPLL_INIT_RFU                                                                                     0x31U
#define DPLL_INIT_RFU_DPLL_WAKEUP_TIME_POS                                                               (11U)
#define DPLL_INIT_RFU_DPLL_WAKEUP_TIME_MASK                                                              (0x00001800U)  /**< rw-Duration of the Wakeup-Mode*/
#define DPLL_INIT_RFU_DPLL_WAKEUP_INT_ENABLE_POS                                                         (10U)
#define DPLL_INIT_RFU_DPLL_WAKEUP_INT_ENABLE_MASK                                                        (0x00000400U)  /**< rw-Enables the integral path of the loop-filter even in WAKEUP mode*/
#define DPLL_INIT_RFU_DPLL_WAKEUP_LF_SEL_POS                                                             (8U)
#define DPLL_INIT_RFU_DPLL_WAKEUP_LF_SEL_MASK                                                            (0x00000300U)  /**< rw-Proportional parameter of loop filter for WAKEUP mode*/
#define DPLL_INIT_RFU_DPLL_TIME_INIT_SEL_POS                                                             (6U)
#define DPLL_INIT_RFU_DPLL_TIME_INIT_SEL_MASK                                                            (0x000000C0U)  /**< rw-Initial gear duration*/
#define DPLL_INIT_RFU_DPLL_INT_INIT_SEL_POS                                                              (3U)
#define DPLL_INIT_RFU_DPLL_INT_INIT_SEL_MASK                                                             (0x00000038U)  /**< rw-Initial gear integral parameter*/
#define DPLL_INIT_RFU_DPLL_PROP_INIT_SEL_POS                                                             (0U)
#define DPLL_INIT_RFU_DPLL_PROP_INIT_SEL_MASK                                                            (0x00000007U)  /**< rw-Initial gear proportional parameter*/

#define DPLL_INIT_FREQ_RFU                                                                                0x32U
#define DPLL_INIT_FREQ_RFU_DPLL_SATURATION_ENABLE_POS                                                    (16U)
#define DPLL_INIT_FREQ_RFU_DPLL_SATURATION_ENABLE_MASK                                                   (0x00010000U)  /**< rw-If set, the integrator is saturated to DPLL_CTRL +/- 0.0546875*/
#define DPLL_INIT_FREQ_RFU_DPLL_CTRL_POS                                                                 (0U)
#define DPLL_INIT_FREQ_RFU_DPLL_CTRL_MASK                                                                (0x0000FFFFU)  /**< rw-The initial DPLL integrator value, i.e., the nominal post-divider value. Note: This value is loaded into the DPLL in the first cycle the DPLL controller enables the DPLL (refer to CLIF_DPLL_INIT_REG.SINGLE_LOOP_ALM_ENABLE). Note: It is not allowed to change this value during DPLL operation, because the limits (convergence range) is derived from it*/

#define MISC_CONTROL_RFU                                                                                  0x33U
#define MISC_CONTROL_RFU_TADT_EXTEND_SELECT_POS                                                          (5U)
#define MISC_CONTROL_RFU_TADT_EXTEND_SELECT_MASK                                                         (0x00000060U)  /**< rw-By setting a value != 0 the minimum time for TADT can be increased*/
#define MISC_CONTROL_RFU_LONG_LD_MASK_ENABLE_POS                                                         (4U)
#define MISC_CONTROL_RFU_LONG_LD_MASK_ENABLE_MASK                                                        (0x00000010U)  /**< rw-If set to 1, the masking time for the NFC-LD after TxRFOff is longer.*/
#define MISC_CONTROL_RFU_TIDT_SKIP_ENABLE_POS                                                            (3U)
#define MISC_CONTROL_RFU_TIDT_SKIP_ENABLE_MASK                                                           (0x00000008U)  /**< rw-If set to 1, the TIDT reload value is reduced to 1, resulting in a nearly immediate switch on of the RF-Field.*/
#define MISC_CONTROL_RFU_MILLER_DECODER_SEL_POS                                                          (2U)
#define MISC_CONTROL_RFU_MILLER_DECODER_SEL_MASK                                                         (0x00000004U)  /**< rw-Chooses the Miller decoder 0* Advanced Miller decoder (HFO clock based) 1 RF clock based Miller decoder*/
#define MISC_CONTROL_RFU_TADT_SKIP_CHECK_ENABLE_POS                                                      (1U)
#define MISC_CONTROL_RFU_TADT_SKIP_CHECK_ENABLE_MASK                                                     (0x00000002U)  /**< rw-If set to 1 the timing check for TADT is skipped */
#define MISC_CONTROL_RFU_RF_FREQ_OK_OVERRIDE_ENABLE_POS                                                  (0U)
#define MISC_CONTROL_RFU_RF_FREQ_OK_OVERRIDE_ENABLE_MASK                                                 (0x00000001U)  /**< rw-If set to 1 the frequency check of the RF-Clock is overridden. */

#define PAD_CONFIG_RFU                                                                                    0x34U
#define PAD_CONFIG_RFU_TX_GSN_SRC_SEL_POS                                                                (4U)
#define PAD_CONFIG_RFU_TX_GSN_SRC_SEL_MASK                                                               (0x00000010U)  /**< rw-Source of GSN value (0... SMU, 1... CLIF)*/
#define PAD_CONFIG_RFU_TX_GSN_CW_SB_POS                                                                  (0U)
#define PAD_CONFIG_RFU_TX_GSN_CW_SB_MASK                                                                 (0x0000000FU)  /**< rw-GSN Value for Standby Mode*/

#define ANA_AGC_RFU                                                                                       0x35U
#define ANA_AGC_RFU_AGC_PD_POS                                                                           (0U)
#define ANA_AGC_RFU_AGC_PD_MASK                                                                          (0x00000001U)  /**< rw-AGC power down*/

#define TX_CONTROL_RFU                                                                                    0x36U
#define TX_CONTROL_RFU_TX_CM_GSN_MOD_ON_TXACTIVE_POS                                                     (1U)
#define TX_CONTROL_RFU_TX_CM_GSN_MOD_ON_TXACTIVE_MASK                                                    (0x00000002U)  /**< rw-If set to 1, CM GSN value is switched with tx_active instead of envelope*/
#define TX_CONTROL_RFU_TX_INVERT_POS                                                                     (0U)
#define TX_CONTROL_RFU_TX_INVERT_MASK                                                                    (0x00000001U)  /**< rw-If this bit is set to 1, the resulting signal is inverted*/

#define SIGPRO_CM_CONFIG_RFU                                                                              0x37U
#define SIGPRO_CM_CONFIG_RFU_MILLER_SOURCE_EXT_POS                                                       (30U)
#define SIGPRO_CM_CONFIG_RFU_MILLER_SOURCE_EXT_MASK                                                      (0xC0000000U)  /**< rw-Defines if miller decoder uses the external modulation signal for decoding. Note: This setting (if not 0) overrides other input settings. 0: not used, 1: plain envelope from external device, 2: 100% envelope generated on 13.56MHz clock domain, 3: 100% envelope generated on HFO clock domain*/
#define SIGPRO_CM_CONFIG_RFU_USE_CMPLUS_MILLER_POS                                                       (29U)
#define SIGPRO_CM_CONFIG_RFU_USE_CMPLUS_MILLER_MASK                                                      (0x20000000U)  /**< rw-If set, the Miller decoder uses the CM plus (Manchester demodulator) as preferred analog demodulator. Note that if USE_CMPLUS_MILLER and USE_ADCBCM_MILLER are set, then the ADCBCM is used as input to the Miller decoder.*/
#define SIGPRO_CM_CONFIG_RFU_USE_ADCBCM_MANCHESTER_POS                                                   (28U)
#define SIGPRO_CM_CONFIG_RFU_USE_ADCBCM_MANCHESTER_MASK                                                  (0x10000000U)  /**< rw-If set; the ADC based Card-Mode output is used as input to the Manchester decoder. */
#define SIGPRO_CM_CONFIG_RFU_USE_ADCBCM_MILLER_POS                                                       (27U)
#define SIGPRO_CM_CONFIG_RFU_USE_ADCBCM_MILLER_MASK                                                      (0x08000000U)  /**< rw-If set; the ADC based Card-Mode output is used as input to the Miller decoder. */
#define SIGPRO_CM_CONFIG_RFU_SIGPRO_IN_SEL_POS                                                           (25U)
#define SIGPRO_CM_CONFIG_RFU_SIGPRO_IN_SEL_MASK                                                          (0x06000000U)  /**< rw-Defines the source of SigPro input signals. Note: PN548 analogue means for reader mode the ADC signals and for card mode the signal from the appropriate analog demodulator or the ADC based card mode. 00* TxEnvelope for loopback mode 01 Analog Modulator / ADCBCM 010 SigIn 011 Analog Modulator / ADCBCM + SigIn 100 TxEnvelope for loopback mode*/
#define SIGPRO_CM_CONFIG_RFU_TARGET_POS                                                                  (24U)
#define SIGPRO_CM_CONFIG_RFU_TARGET_MASK                                                                 (0x01000000U)  /**< rw-Defines that the receiver is in target mode. Note: Do not change this bit when the Rx-Decoder is enabled. */
#define SIGPRO_CM_CONFIG_RFU_SYNC_MODE_SEL_POS                                                           (22U)
#define SIGPRO_CM_CONFIG_RFU_SYNC_MODE_SEL_MASK                                                          (0x00C00000U)  /**< rw-Chooses the threshold for interpolation of the zero-crossing detection. 0h 0.25; 1h 0.5; 2h-3h 0.75*/
#define SIGPRO_CM_CONFIG_RFU_EPSILON_TH_POS                                                              (16U)
#define SIGPRO_CM_CONFIG_RFU_EPSILON_TH_MASK                                                             (0x003F0000U)  /**< rw-Threshold for the epsilon environment in the zero-crossing detection block. Note that with value 1h the block is disabled and the value 0h is forbidden. */
#define SIGPRO_CM_CONFIG_RFU_WATCH_DOG_VAL_POS                                                           (8U)
#define SIGPRO_CM_CONFIG_RFU_WATCH_DOG_VAL_MASK                                                          (0x0000FF00U)  /**< rw-Reset value for the Watch-Dog Counter.After this number of 848kHz clock cycles where no edges was detected, the Watch Dog counter resets the edge-locking, the output envelope, and the dynamic threshold.*/
#define SIGPRO_CM_CONFIG_RFU_WATCH_DOG_ENABLE_POS                                                        (7U)
#define SIGPRO_CM_CONFIG_RFU_WATCH_DOG_ENABLE_MASK                                                       (0x00000080U)  /**< rw-Enables the watch-dog timer to release the edge-locking after a certain time. */
#define SIGPRO_CM_CONFIG_RFU_SYNC_MODE_ENABLE_POS                                                        (6U)
#define SIGPRO_CM_CONFIG_RFU_SYNC_MODE_ENABLE_MASK                                                       (0x00000040U)  /**< rw-If set; the sync mode is activated. The sync-mode is used to reduce the jitter in the zero-crossing detection (usage depends on the analog HW configuration). */
#define SIGPRO_CM_CONFIG_RFU_EDGE_DETECT_DYN_ENABLE_POS                                                  (5U)
#define SIGPRO_CM_CONFIG_RFU_EDGE_DETECT_DYN_ENABLE_MASK                                                 (0x00000020U)  /**< rw-If set; the edge detecto threshold is dynamically adapted (1/4th bit energy of the last successful edge decision but not smaller than the threshold in CLIF_SIGPRO_ADCBCM_THRESHOLD_REG .EDGE_D ETECT_TH). */
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_848_POS                                                       (4U)
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_848_MASK                                                      (0x00000010U)  /**< rw-Enables the 848kBd envelope detection of the ADCBCM. */
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_424_POS                                                       (3U)
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_424_MASK                                                      (0x00000008U)  /**< rw-Enables the 424kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. */
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_212_POS                                                       (2U)
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_212_MASK                                                      (0x00000004U)  /**< rw-Enables the 212kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. */
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_106_POS                                                       (1U)
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_106_MASK                                                      (0x00000002U)  /**< rw-Enables the 106kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. */
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_POS                                                           (0U)
#define SIGPRO_CM_CONFIG_RFU_ADCBCM_ENABLE_MASK                                                          (0x00000001U)  /**< rw-Enables the ADCBCM. Note that at least one mode (106; 212; 424; or 848kBd) has to enabled additionally. */

#define SIGPRO_CM_CONFIG2_RFU                                                                             0x38U
#define SIGPRO_CM_CONFIG2_RFU_MILLER_SYNC_ENABLE_POS                                                     (13U)
#define SIGPRO_CM_CONFIG2_RFU_MILLER_SYNC_ENABLE_MASK                                                    (0x00002000U)  /**< -If set to one miller pulse synchronisation is enabled. Note: TX_BITPHASE is used to adjust the prescaler of the guard timer. Note: This bit is set by HW when TypeA protocol is detected in automatic mode detection. */
#define SIGPRO_CM_CONFIG2_RFU_DIG_CLOCK_FAIL_DETECTION_ENABLE_POS                                        (12U)
#define SIGPRO_CM_CONFIG2_RFU_DIG_CLOCK_FAIL_DETECTION_ENABLE_MASK                                       (0x00001000U)  /**< rw-If set, digital clock detection is used instead of analog*/
#define SIGPRO_CM_CONFIG2_RFU_MANCHESTER_SOURCE_EXT_POS                                                  (11U)
#define SIGPRO_CM_CONFIG2_RFU_MANCHESTER_SOURCE_EXT_MASK                                                 (0x00000800U)  /**< rw-If set, the manchester decoder uses the external modulation signal for decoding. Note: This setting (if not 0) overrides other input settings*/
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_USE_RFCOUNTER_POS                                               (10U)
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_USE_RFCOUNTER_MASK                                              (0x00000400U)  /**< rw-If set the Adv. Miller decoder uses the RF counter directly for decoding. This feature is intended for 848kBd to avoid additional errors due to synchronization.*/
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_FILTER_ENABLE_POS                                               (9U)
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_FILTER_ENABLE_MASK                                              (0x00000200U)  /**< rw-If set the Miller input signal is filtered with a 3-out-of-5 median filter (on HFO clock), before decoding. Note: This feature is not available in combination with ADV_MILLER_USE_RFCOUNTER.*/
#define SIGPRO_CM_CONFIG2_RFU_NRZ_DEC_ENABLE_POS                                                         (8U)
#define SIGPRO_CM_CONFIG2_RFU_NRZ_DEC_ENABLE_MASK                                                        (0x00000100U)  /**< rw-If set the NRZ decoder is activated for ISO14443 type B PICC reception. Note that the NRZ decoder must be activated in general target mode to be able to detect ISO14443 type B.*/
#define SIGPRO_CM_CONFIG2_RFU_MANCHESTER_DEC_ENABLE_POS                                                  (7U)
#define SIGPRO_CM_CONFIG2_RFU_MANCHESTER_DEC_ENABLE_MASK                                                 (0x00000080U)  /**< rw-If set the Manchester decoder is activated for FeliCa PICC reception. Note that the Manchester decoder must be activated in general target mode to be able to detect FeliCa.*/
#define SIGPRO_CM_CONFIG2_RFU_MILLER_DEC_ENABLE_POS                                                      (6U)
#define SIGPRO_CM_CONFIG2_RFU_MILLER_DEC_ENABLE_MASK                                                     (0x00000040U)  /**< rw-If set the Miller decoder is activated for ISO14443 type A PICC reception. Note that the Miller decoder must be activated in general target mode to be able to detect ISO14443 type A.*/
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_CALIB_OFF_POS                                                   (5U)
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_CALIB_OFF_MASK                                                  (0x00000020U)  /**< rw-If set, the calibration of the advanced Miller decoder is deactivated and the nominal threshold values are used.*/
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_EDGE_SEL_POS                                                    (4U)
#define SIGPRO_CM_CONFIG2_RFU_ADV_MILLER_EDGE_SEL_MASK                                                   (0x00000010U)  /**< rw-Select the edge for decoding Note: This feature is not available in combination with ADV_MILLER_USE_RFCOUNTER. 0* decoder counts from falling to falling edge: 1 decoder counts from rising to rising edge. */
#define SIGPRO_CM_CONFIG2_RFU_CM_EGT_CHECK_POS                                                           (3U)
#define SIGPRO_CM_CONFIG2_RFU_CM_EGT_CHECK_MASK                                                          (0x00000008U)  /**< rw-If set, the EGT check for ISO14443 type B card-mode is activated. An invalid EGT length causes an protocol error.*/
#define SIGPRO_CM_CONFIG2_RFU_TAU_POS                                                                    (1U)
#define SIGPRO_CM_CONFIG2_RFU_TAU_MASK                                                                   (0x00000006U)  /**< rw-Defines the timing constant of the internal PLL as a negative power of two. */
#define SIGPRO_CM_CONFIG2_RFU_MODE_DETECTOR_OFF_POS                                                      (0U)
#define SIGPRO_CM_CONFIG2_RFU_MODE_DETECTOR_OFF_MASK                                                     (0x00000001U)  /**< rw-If set to 1; the mode detector is deactivated */

#define SIGPRO_RM_CONFIG_RFU                                                                              0x39U
#define SIGPRO_RM_CONFIG_RFU_SYNC_VAL_POS                                                                (16U)
#define SIGPRO_RM_CONFIG_RFU_SYNC_VAL_MASK                                                               (0xFFFF0000U)  /**< rw-Defines the Sync Pattern; which is expected to be sent as preamble before the actual data. */
#define SIGPRO_RM_CONFIG_RFU_SYNC_LEN_POS                                                                (12U)
#define SIGPRO_RM_CONFIG_RFU_SYNC_LEN_MASK                                                               (0x0000F000U)  /**< rw-Defines how many Bits of Sync_Val are valid. Example: 0 .. 1 Bit. */
#define SIGPRO_RM_CONFIG_RFU_SYNC_NEGEDGE_POS                                                            (11U)
#define SIGPRO_RM_CONFIG_RFU_SYNC_NEGEDGE_MASK                                                           (0x00000800U)  /**< rw-Defines a SOF with no min or max in correlation. The bitgrid will be defined by the neg edge (EPC; UID). */
#define SIGPRO_RM_CONFIG_RFU_LAST_SYNC_HALF_POS                                                          (10U)
#define SIGPRO_RM_CONFIG_RFU_LAST_SYNC_HALF_MASK                                                         (0x00000400U)  /**< rw-The last Bit of the Synccode has only half of the length compared to all other bits (EPC V2). */
#define SIGPRO_RM_CONFIG_RFU_SYNC_TYPE_POS                                                               (8U)
#define SIGPRO_RM_CONFIG_RFU_SYNC_TYPE_MASK                                                              (0x00000300U)  /**< rw-Set to 0 all 16 bits of SyncVal are interpreted as bits. Set to 1 a nipple of bits is interpreted as one bit in following way:{data; coll} data=zero or one; coll=1 means a collision on this bit. Note: if Coll=1 the vale of data is ignored.  Set to 2 the synchronisation is done at every startbit of each byte (TypeB)*/
#define SIGPRO_RM_CONFIG_RFU_RESERVED_POS                                                                (7U)
#define SIGPRO_RM_CONFIG_RFU_RESERVED_MASK                                                               (0x00000080U)  /**< r--Reserved */
#define SIGPRO_RM_CONFIG_RFU_EGT_CHECK_POS                                                               (6U)
#define SIGPRO_RM_CONFIG_RFU_EGT_CHECK_MASK                                                              (0x00000040U)  /**< rw-Set to 1; enables the EGT check functionality. An invalid EGT length causes an protocol error */
#define SIGPRO_RM_CONFIG_RFU_CORR_SPEED_POS                                                              (4U)
#define SIGPRO_RM_CONFIG_RFU_CORR_SPEED_MASK                                                             (0x00000030U)  /**< rw-Defines the number of clocks used for one correlation. 00* ISO14443 01 ICODE 53kBaud; FeliCa 424 kBaud 10 ICODE 26kBaud; FeliCa 212 kBaud 11 RFU*/
#define SIGPRO_RM_CONFIG_RFU_CORR_FREQ_POS                                                               (2U)
#define SIGPRO_RM_CONFIG_RFU_CORR_FREQ_MASK                                                              (0x0000000CU)  /**< rw-Sets the correlation frequency 00:212 kHz 01:424 kHz 10:848 kHz 11:848 kHz */
#define SIGPRO_RM_CONFIG_RFU_CORR_LENGTH_POS                                                             (1U)
#define SIGPRO_RM_CONFIG_RFU_CORR_LENGTH_MASK                                                            (0x00000002U)  /**< rw-Reserved */
#define SIGPRO_RM_CONFIG_RFU_CORR_INV_POS                                                                (0U)
#define SIGPRO_RM_CONFIG_RFU_CORR_INV_MASK                                                               (0x00000001U)  /**< rw-Defines a logical one: 0:subcarrier / no subcarrier 1:no subcarrier / subcarrier*/

#define RX_CONFIG_REG_RFU                                                                                 0x3AU
#define RX_CONFIG_REG_RFU_RX_PARITY_EMD_ON_SOVER_POS                                                     (29U)
#define RX_CONFIG_REG_RFU_RX_PARITY_EMD_ON_SOVER_MASK                                                    (0x20000000U)  /**< rw-If set, decision if EMD due to parity error is
taken at sover*/
#define RX_CONFIG_REG_RFU_RX_MISSING_PARITY_IS_EMD_POS                                                   (28U)
#define RX_CONFIG_REG_RFU_RX_MISSING_PARITY_IS_EMD_MASK                                                  (0x10000000U)  /**< rw-If set, a missing parity bit in the 4th byte is treated as
EMD (for EMD option 3/4*/
#define RX_CONFIG_REG_RFU_RX_ADVANCED_EMD_ENABLE_POS                                                     (27U)
#define RX_CONFIG_REG_RFU_RX_ADVANCED_EMD_ENABLE_MASK                                                    (0x08000000U)  /**< rw-If set, new EMD options for PN548 are enabled*/
#define RX_CONFIG_REG_RFU_RX_PARITY_ERROR_IS_EMD_POS                                                     (26U)
#define RX_CONFIG_REG_RFU_RX_PARITY_ERROR_IS_EMD_MASK                                                    (0x04000000U)  /**< rw-If set to 1 a parity error in the 3rd/4th byte (depending on RX_EMD_SUP setting) is interpreted as an EMD error. Otherwise it is interpreted as a parity error.*/
#define RX_CONFIG_REG_RFU_RX_EMD_SUP_POS                                                                 (23U)
#define RX_CONFIG_REG_RFU_RX_EMD_SUP_MASK                                                                (0x03800000U)  /**< rw-Defines EMD suppression mechanism 000 Off 001 EMD suppression according to ISO14443 010 EMD suppression according to NFC Forum (with respect to the first 3 characters) 011 EMD suppression according to NFC Forum (with respect to the first 4characters) 100 EMD suppression according to NFC Forum (with respect to the first 4characters, all valid frames <4 bytes are ignored) 101-111 RFU*/
#define RX_CONFIG_REG_RFU_RX_COLL_IS_DATA_ERROR_POS                                                      (22U)
#define RX_CONFIG_REG_RFU_RX_COLL_IS_DATA_ERROR_MASK                                                     (0x00400000U)  /**< rw-If set to 1; a collision is treated as a data integrity error (especially for ISO14443-4) */
#define RX_CONFIG_REG_RFU_RESERVED_POS                                                                   (21U)
#define RX_CONFIG_REG_RFU_RESERVED_MASK                                                                  (0x00200000U)  /**< r--*/
#define RX_CONFIG_REG_RFU_RX_CRC_ALLOW_BITS_POS                                                          (20U)
#define RX_CONFIG_REG_RFU_RX_CRC_ALLOW_BITS_MASK                                                         (0x00100000U)  /**< rw-Set to 1; a frame with less than one byte length is writtten to ram; when the CRC is enabled otherwise it is discarted. */
#define RX_CONFIG_REG_RFU_RX_FORCE_CRC_WRITE_POS                                                         (19U)
#define RX_CONFIG_REG_RFU_RX_FORCE_CRC_WRITE_MASK                                                        (0x00080000U)  /**< rw-Set to 1; the received CRC byte(s) are written to ram. In normal operation (if this bit is set to 0) CRC bytes are only checked and removed from the data stream. */
#define RX_CONFIG_REG_RFU_RX_PARITY_ERR_8BITS_ENABLE_POS                                                 (18U)
#define RX_CONFIG_REG_RFU_RX_PARITY_ERR_8BITS_ENABLE_MASK                                                (0x00040000U)  /**< rw-If set, parity error is detected when only 8 bits in last byte received*/
#define RX_CONFIG_REG_RFU_RESERVED1_POS                                                                   (15U)
#define RX_CONFIG_REG_RFU_RESERVED1_MASK                                                                  (0x00038000U)  /**< r--*/
#define RX_CONFIG_REG_RFU_RX_EOF_SYMBOL_POS                                                              (7U)
#define RX_CONFIG_REG_RFU_RX_EOF_SYMBOL_MASK                                                             (0x00007F80U)  /**< rw-This value defines the pattern of the EOF symbol. Note: All bits are clear upon enabling of the receiver is the automatic mode detection is enabled. If in such a case an ISO14443A communication is detected bit0 is set by the mod-detector. */
#define RX_CONFIG_REG_RFU_RX_EOF_TYPE_POS                                                                (6U)
#define RX_CONFIG_REG_RFU_RX_EOF_TYPE_MASK                                                               (0x00000040U)  /**< rw-Defines the EOF handling 0 EOF as defined in the register field RX_EOF_SYMBOL is expected. 1 EOF as defined in ISO14443B is expected. */
#define RX_CONFIG_REG_RFU_RX_STOP_CONDITION_POS                                                          (4U)
#define RX_CONFIG_REG_RFU_RX_STOP_CONDITION_MASK                                                         (0x00000030U)  /**< rw-This bit field defines the condition to stop a reception. Note: This bit is set by the mod-detector if automatic mode detection is enabled and the corresponding communication is detected. 00b Reception is stopped only if end of data communication is detected by the signal processing 01b Reception is stopped upon reception of the EOF pattern selected by the register field RX_EOF_TYPE and RX_EOF_SYMBOL Note: This value is set by the mod-detector if a ISO14443A or ISO14443B communication is detected 10b Reception is stopped on detection of an incorrect parity bit. 11b Recption is stopped if the number of received bytes reaches the defined frame length (which extracted from the first data-byte received). Note: This value is set by the mod-detector if a FeliCa communication is detected */
#define RX_CONFIG_REG_RFU_RX_MSB_FIRST_POS                                                               (3U)
#define RX_CONFIG_REG_RFU_RX_MSB_FIRST_MASK                                                              (0x00000008U)  /**< rw-If set to 1; data bytes are interpreted MSB first for reception */
#define RX_CONFIG_REG_RFU_RX_STOPBIT_ENABLE_POS                                                          (2U)
#define RX_CONFIG_REG_RFU_RX_STOPBIT_ENABLE_MASK                                                         (0x00000004U)  /**< rw-Set to 1; a stop-bit is expected and will be checked and extracted from data stream. Note: The stop-bit is not configurable and fixed to a logic 1. Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443B communication is detected.*/
#define RX_CONFIG_REG_RFU_RESERVED2_POS                                                                   (0U)
#define RX_CONFIG_REG_RFU_RESERVED2_MASK                                                                  (0x00000003U)  /**< r--*/

#define TX_DATA_MOD_RFU                                                                                   0x3BU
#define TX_DATA_MOD_RFU_TX_DATA_MILLER_ENABLE_POS                                                        (8U)
#define TX_DATA_MOD_RFU_TX_DATA_MILLER_ENABLE_MASK                                                       (0x00000100U)  /**< rw-If set to 1, pulse modulation is applied according to modified miller coding*/
#define TX_DATA_MOD_RFU_TX_DATA_INV_ENV_POS                                                              (7U)
#define TX_DATA_MOD_RFU_TX_DATA_INV_ENV_MASK                                                             (0x00000080U)  /**< rw-If set to 1, the output envelope is inverted*/
#define TX_DATA_MOD_RFU_TX_DATA_ENV_TYPE_POS                                                             (4U)
#define TX_DATA_MOD_RFU_TX_DATA_ENV_TYPE_MASK                                                            (0x00000070U)  /**< rw-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple coding 111b RFU*/
#define TX_DATA_MOD_RFU_TX_DATA_SC_FREQ_POS                                                              (3U)
#define TX_DATA_MOD_RFU_TX_DATA_SC_FREQ_MASK                                                             (0x00000008U)  /**< rw-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz*/
#define TX_DATA_MOD_RFU_TX_DATA_BIT_FREQ_POS                                                             (0U)
#define TX_DATA_MOD_RFU_TX_DATA_BIT_FREQ_MASK                                                            (0x00000007U)  /**< rw-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz; 001b -> Reserved; 010b -> 26 kHz; 011b -> 53 kHz; 100b -> 106 kHz; 101b -> 212 kHz; 110b -> 424 kHz; 111b -> 848 kHz;*/

#define TX_FRAME_CONFIG_RFU                                                                               0x3CU
#define TX_FRAME_CONFIG_RFU_TX_DATA_CODE_TYPE_POS                                                        (13U)
#define TX_FRAME_CONFIG_RFU_TX_DATA_CODE_TYPE_MASK                                                       (0x0000E000U)  /**< rw-Specifies the type of encoding of data to be used 000b No special code 001b 1 out of 4 code [I-Code SLI] 010b 1 out of 256 code [I-Code SLI] 011b Pulse interval encoding (PIE) [I-Code EPC-V2] 100b 2bit tuple code (intended only for test purpose) 101-111b Reserved */
#define TX_FRAME_CONFIG_RFU_TX_STOPBIT_TYPE_POS                                                          (10U)
#define TX_FRAME_CONFIG_RFU_TX_STOPBIT_TYPE_MASK                                                         (0x00001C00U)  /**< rw-Enables the stop bit (logic "1") and extra guard time (logic "1"). The value 0 disables transmission of stop-bits. 000b no stop-bit; no EGT 001b stop-bit; no EGT 010b stop-bit + 1 EGT 011b stop-bit + 2 EGT 100b stop-bit + 3 EGT 101b stop-bit + 4 EGT 110b stop-bit + 5 EGT 111b stop-bit + 6 EGT */
#define TX_FRAME_CONFIG_RFU_TX_STARTBIT_ENABLE_POS                                                       (9U)
#define TX_FRAME_CONFIG_RFU_TX_STARTBIT_ENABLE_MASK                                                      (0x00000200U)  /**< rw-If set to 1; a start-bit (logic "0") will be send */
#define TX_FRAME_CONFIG_RFU_TX_MSB_FIRST_POS                                                             (8U)
#define TX_FRAME_CONFIG_RFU_TX_MSB_FIRST_MASK                                                            (0x00000100U)  /**< rw-If set to 1; data bytes are interpreted MSB first for data transmission */
#define TX_FRAME_CONFIG_RFU_RESERVED_POS                                                                 (0U)
#define TX_FRAME_CONFIG_RFU_RESERVED_MASK                                                                (0x000000FFU)  /**< r--*/

#define TX_SYMBOL_CONFIG_RFU                                                                              0x3DU
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_LEN_POS                                                    (26U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_LEN_MASK                                                   (0x3C000000U)  /**< rw-Specifies the number of bits issued for symbol 1 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit*/
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_TYPE_POS                                                   (25U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_TYPE_MASK                                                  (0x02000000U)  /**< rw-Specifies the type of the burst of Symbol1 (logical zero / logical one) */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_ONLY_POS                                                   (24U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_ONLY_MASK                                                  (0x01000000U)  /**< rw-If set to 1; Symbol1 consists only of a burst and no symbol pattern */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_ENABLE_POS                                                 (23U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_BURST_ENABLE_MASK                                                (0x00800000U)  /**< rw-If set to 1; the burst of Symbol0 of the length defined in bit field SYMBOL1_BURST_LEN is enabled         */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_LEN_POS                                                    (19U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_LEN_MASK                                                   (0x00780000U)  /**< rw-Specifies the number of bits issued for symbol 0 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit*/
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_TYPE_POS                                                   (18U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_TYPE_MASK                                                  (0x00040000U)  /**< rw-Specifies the type of the burst of Symbol0 (logical zero / logical one) */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_ONLY_POS                                                   (17U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_ONLY_MASK                                                  (0x00020000U)  /**< rw-If set to 1; Symbol0 consists only of a burst and no symbol pattern */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_ENABLE_POS                                                 (16U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_BURST_ENABLE_MASK                                                (0x00010000U)  /**< rw-If set to 1; the burst of Symbol0 of the length defined in bit field SYMBOL0_BURST_LEN is enabled */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL3_LEN_POS                                                          (13U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL3_LEN_MASK                                                         (0x0000E000U)  /**< rw-Specifies the number of valid bits of the symbol definition of Symbol3. The range is from 1 bit (value 0000) to 8 bit (value 111) */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL2_LEN_POS                                                          (10U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL2_LEN_MASK                                                         (0x00001C00U)  /**< rw-Specifies the number of valid bits of the symbol definition of Symbol2. The range is from 1 bit (value 0000) to 8bit (value 111) */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_LEN_POS                                                          (5U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL1_LEN_MASK                                                         (0x000003E0U)  /**< rw-Specifies the number of valid bits of the symbol definition of Symbol1. The range is from 1 bit (value 0000) to 28 bit (value 11011) */
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_LEN_POS                                                          (0U)
#define TX_SYMBOL_CONFIG_RFU_TX_SYMBOL0_LEN_MASK                                                         (0x0000001FU)  /**< rw-Specifies the number of valid bits of the symbol definition of Symbol0. The range is from 1 bit (value 0000) to 28 bit (value 11011)*/

#define TX_SYMBOL0_DEF_RFU                                                                                0x3EU
#define TX_SYMBOL0_DEF_RFU_TX_SYMBOL0_DEF_POS                                                            (0U)
#define TX_SYMBOL0_DEF_RFU_TX_SYMBOL0_DEF_MASK                                                           (0x0FFFFFFFU)  /**< rw-Pattern definition for Symbol0*/

#define TX_SYMBOL1_DEF_RFU                                                                                0x3FU
#define TX_SYMBOL1_DEF_RFU_TX_SYMBOL1_DEF_POS                                                            (0U)
#define TX_SYMBOL1_DEF_RFU_TX_SYMBOL1_DEF_MASK                                                           (0x0FFFFFFFU)  /**< rw-Pattern definition for Symbol1 */

#define TX_SYMBOL23_DEF_RFU                                                                               0x40U
#define TX_SYMBOL23_DEF_RFU_TX_SYMBOL3_DEF_POS                                                           (16U)
#define TX_SYMBOL23_DEF_RFU_TX_SYMBOL3_DEF_MASK                                                          (0x00FF0000U)  /**< rw-Pattern definition for Symbol3 */
#define TX_SYMBOL23_DEF_RFU_RESERVED_POS                                                                 (8U)
#define TX_SYMBOL23_DEF_RFU_RESERVED_MASK                                                                (0x0000FF00U)  /**< r--Reserved */
#define TX_SYMBOL23_DEF_RFU_TX_SYMBOL2_DEF_POS                                                           (0U)
#define TX_SYMBOL23_DEF_RFU_TX_SYMBOL2_DEF_MASK                                                          (0x000000FFU)  /**< rw-Pattern definition for Symbol2*/

#define TX_SYMBOL01_MOD_RFU                                                                               0x41U
#define TX_SYMBOL01_MOD_RFU_TX_S01_MODWIDTH_POS                                                          (16U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_MODWIDTH_MASK                                                         (0x00FF0000U)  /**< rw-Specifies the length of a pulse for sending data of symbol 0/1. The length is given by the number of carrier clocks + 1. */
#define TX_SYMBOL01_MOD_RFU_RESERVED_POS                                                                 (9U)
#define TX_SYMBOL01_MOD_RFU_RESERVED_MASK                                                                (0x0000FE00U)  /**< r--Reserved */
#define TX_SYMBOL01_MOD_RFU_TX_S01_MILLER_ENABLE_POS                                                     (8U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_MILLER_ENABLE_MASK                                                    (0x00000100U)  /**< rw-If set to 1; pulse modulation is applied according to modified miller coding. */
#define TX_SYMBOL01_MOD_RFU_TX_S01_INV_ENV_POS                                                           (7U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_INV_ENV_MASK                                                          (0x00000080U)  /**< rw-If set to 1; the output envelope is inverted. */
#define TX_SYMBOL01_MOD_RFU_TX_S01_ENV_TYPE_POS                                                          (4U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_ENV_TYPE_MASK                                                         (0x00000070U)  /**< rw-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU*/
#define TX_SYMBOL01_MOD_RFU_TX_S01_SC_FREQ_POS                                                           (3U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_SC_FREQ_MASK                                                          (0x00000008U)  /**< rw-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz */
#define TX_SYMBOL01_MOD_RFU_TX_S01_BIT_FREQ_POS                                                          (0U)
#define TX_SYMBOL01_MOD_RFU_TX_S01_BIT_FREQ_MASK                                                         (0x00000007U)  /**< rw-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz; 001b -> Reserved; 010b -> 26 kHz; 011b -> 53 kHz; 100b -> 106 kHz; 101b -> 212 kHz; 110b -> 424 kHz; 111b -> 848 kHz;*/

#define TX_SYMBOL23_MOD_RFU                                                                               0x42U
#define TX_SYMBOL23_MOD_RFU_RESERVED_POS                                                                 (9U)
#define TX_SYMBOL23_MOD_RFU_RESERVED_MASK                                                                (0x0000FE00U)  /**< r--Reserved */
#define TX_SYMBOL23_MOD_RFU_TX_S23_MILLER_ENABLE_POS                                                     (8U)
#define TX_SYMBOL23_MOD_RFU_TX_S23_MILLER_ENABLE_MASK                                                    (0x00000100U)  /**< rw-If set to 1, pulse modulation is applied according to modified miller coding.*/
#define TX_SYMBOL23_MOD_RFU_TX_S23_INV_ENV_POS                                                           (7U)
#define TX_SYMBOL23_MOD_RFU_TX_S23_INV_ENV_MASK                                                          (0x00000080U)  /**< rw-If set to 1, the output envelope is inverted.*/
#define TX_SYMBOL23_MOD_RFU_TX_S23_ENV_TYPE_POS                                                          (4U)
#define TX_SYMBOL23_MOD_RFU_TX_S23_ENV_TYPE_MASK                                                         (0x00000070U)  /**< rw-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU*/
#define TX_SYMBOL23_MOD_RFU_TX_S23_SC_FREQ_POS                                                           (3U)
#define TX_SYMBOL23_MOD_RFU_TX_S23_SC_FREQ_MASK                                                          (0x00000008U)  /**< rw-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz*/
#define TX_SYMBOL23_MOD_RFU_TX_S23_BIT_FREQ_POS                                                          (0U)
#define TX_SYMBOL23_MOD_RFU_TX_S23_BIT_FREQ_MASK                                                         (0x00000007U)  /**< rw-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz; 001b -> Reserved; 010b -> 26 kHz; 011b -> 53 kHz; 100b -> 106 kHz; 101b -> 212 kHz; 110b -> 424 kHz; 111b -> 848 kHz;*/

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHHALHW_PN5180_REG_H */
