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
* PN5190 HAL Register definition header file.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5190_REG_H_
#define PHHALHW_PN5190_REG_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MASK_TX_LASTBITS                                                                                 0x07U    /**< The bits that are valid for TX_LATS_BITS field */
#define MASK_RX_LASTBITS                                                                                 0x07U    /**< The bits that are valid for RX_LATS_BITS field */
#define MASK_RXWAIT                                                                                      0xFFFFFU/**< Bitmask for RxWait bits. */


#define SYSTEM_CONFIG                                                                                     0x0
#define SYSTEM_CONFIG_15693_CHANGE_DATARATE_POS                                                          (5UL)
#define SYSTEM_CONFIG_15693_CHANGE_DATARATE_MASK                                                         (0x00000060UL)  /**< 0-0 - RFU
1 - Change Data Rate to 53kB/sec
2 -  Change Data Rate to 106kB/sec
3 -  Change Data Rate to 212kB/sec*/
#define SYSTEM_CONFIG_AUTO_RF_ON_OFF_POS                                                                 (4UL)
#define SYSTEM_CONFIG_AUTO_RF_ON_OFF_MASK                                                                (0x00000010UL)  /**< 0-*/
#define SYSTEM_CONFIG_AUTOCOLL_STATE_A_POS                                                               (3UL)
#define SYSTEM_CONFIG_AUTOCOLL_STATE_A_MASK                                                              (0x00000008UL)  /**< 0-*/
#define SYSTEM_CONFIG_SOFT_RESET_POS                                                                     (2UL)
#define SYSTEM_CONFIG_SOFT_RESET_MASK                                                                    (0x00000004UL)  /**< 0-*/
#define SYSTEM_CONFIG_MFC_CRYPTO_ON_POS                                                                  (1UL)
#define SYSTEM_CONFIG_MFC_CRYPTO_ON_MASK                                                                 (0x00000002UL)  /**< 8-If set to 1 the mfc-crypto bit is generated for end-/de-cryption*/
#define SYSTEM_CONFIG_PRBS_TYPE_POS                                                                      (0UL)
#define SYSTEM_CONFIG_PRBS_TYPE_MASK                                                                     (0x00000001UL)  /**< 20-Defines the PRBS type. If set to 1 PRBS15 is selected default value 0 selects PRBS9*/

#define EVENT_ENABLE                                                                                      0x1
#define EVENT_ENABLE_RFU_POS                                                                             (12UL)
#define EVENT_ENABLE_RFU_MASK                                                                            (0xFFFFF000UL)  /**< 12-Enable the corresponding event*/
#define EVENT_ENABLE_CTS_EVENT_ENABLE_POS                                                                (11UL)
#define EVENT_ENABLE_CTS_EVENT_ENABLE_MASK                                                               (0x00000800UL)  /**< 11-Enable the corresponding event*/
#define EVENT_ENABLE_IDLE_EVENT_ENABLE_POS                                                               (10UL)
#define EVENT_ENABLE_IDLE_EVENT_ENABLE_MASK                                                              (0x00000400UL)  /**< 10-Enable the corresponding event*/
#define EVENT_ENABLE_LP_CALIBRATION_EVENT_ENABLE_POS                                                     (9UL)
#define EVENT_ENABLE_LP_CALIBRATION_EVENT_ENABLE_MASK                                                    (0x00000200UL)  /**< 9-Enable the corresponding event*/
#define EVENT_ENABLE_LPCD_EVENT_ENABLE_POS                                                               (8UL)
#define EVENT_ENABLE_LPCD_EVENT_ENABLE_MASK                                                              (0x00000100UL)  /**< 8-Enable the corresponding event*/
#define EVENT_ENABLE_AUTOCOLL_EVENT_ENABLE_POS                                                           (7UL)
#define EVENT_ENABLE_AUTOCOLL_EVENT_ENABLE_MASK                                                          (0x00000080UL)  /**< 7-Enable the corresponding event*/
#define EVENT_ENABLE_TIMER0_EVENT_ENABLE_POS                                                             (6UL)
#define EVENT_ENABLE_TIMER0_EVENT_ENABLE_MASK                                                            (0x00000040UL)  /**< 6-Enable the corresponding event*/
#define EVENT_ENABLE_TEMP_ERROR_EVENT_ENABLE_POS                                                         (5UL)
#define EVENT_ENABLE_TEMP_ERROR_EVENT_ENABLE_MASK                                                        (0x00000020UL)  /**< 5-Enable the corresponding event*/
#define EVENT_ENABLE_RFON_DET_EVENT_ENABLE_POS                                                           (4UL)
#define EVENT_ENABLE_RFON_DET_EVENT_ENABLE_MASK                                                          (0x00000010UL)  /**< 4-Enable the corresponding event*/
#define EVENT_ENABLE_RFOFF_DET_EVENT_ENABLE_POS                                                          (3UL)
#define EVENT_ENABLE_RFOFF_DET_EVENT_ENABLE_MASK                                                         (0x00000008UL)  /**< 3-Enable the corresponding event*/
#define EVENT_ENABLE_STANDBY_PREV_EVENT_ENABLE_POS                                                       (2UL)
#define EVENT_ENABLE_STANDBY_PREV_EVENT_ENABLE_MASK                                                      (0x00000004UL)  /**< 2-Enable the corresponding event*/
#define EVENT_ENABLE_GENERAL_ERROR_EVENT_ENABLE_POS                                                      (1UL)
#define EVENT_ENABLE_GENERAL_ERROR_EVENT_ENABLE_MASK                                                     (0x00000002UL)  /**< 1-Enable the corresponding event*/
#define EVENT_ENABLE_BOOT_EVENT_ENABLE_POS                                                               (0UL)
#define EVENT_ENABLE_BOOT_EVENT_ENABLE_MASK                                                              (0x00000001UL)  /**< 0-Enable the corresponding event*/

#define EVENT_STATUS                                                                                      0x2
#define EVENT_STATUS_RFU_POS                                                                             (12UL)
#define EVENT_STATUS_RFU_MASK                                                                            (0xFFFFF000UL)  /**< 12-Reserved */
#define EVENT_STATUS_CTS_EVENT_POS                                                                       (11UL)
#define EVENT_STATUS_CTS_EVENT_MASK                                                                      (0x00000800UL)  /**< 11-Indicated the availablilty of CTS Event*/
#define EVENT_STATUS_IDLE_EVENT_POS                                                                      (10UL)
#define EVENT_STATUS_IDLE_EVENT_MASK                                                                     (0x00000400UL)  /**< 10-Indicated the availability of IDLE event.*/
#define EVENT_STATUS_LP_CALIBRATION_EVENT_POS                                                            (9UL)
#define EVENT_STATUS_LP_CALIBRATION_EVENT_MASK                                                           (0x00000200UL)  /**< 9-Indicated the availability of LP Calibration event*/
#define EVENT_STATUS_LPCD_EVENT_POS                                                                      (8UL)
#define EVENT_STATUS_LPCD_EVENT_MASK                                                                     (0x00000100UL)  /**< 8-Indicated the availability of LPCD event.*/
#define EVENT_STATUS_AUTOCOLL_EVENT_POS                                                                  (7UL)
#define EVENT_STATUS_AUTOCOLL_EVENT_MASK                                                                 (0x00000080UL)  /**< 7-Indicated the availability of Autocoll event*/
#define EVENT_STATUS_TIMER0_EVENT_POS                                                                    (6UL)
#define EVENT_STATUS_TIMER0_EVENT_MASK                                                                   (0x00000040UL)  /**< 6-Indicated the availability of Timer0 event*/
#define EVENT_STATUS_TEMP_ERROR_EVENT_POS                                                                (5UL)
#define EVENT_STATUS_TEMP_ERROR_EVENT_MASK                                                               (0x00000020UL)  /**< 5-Indicated the availability of Temp error*/
#define EVENT_STATUS_RFON_DET_EVENT_POS                                                                  (4UL)
#define EVENT_STATUS_RFON_DET_EVENT_MASK                                                                 (0x00000010UL)  /**< 4-Indicated the availability of RF ON detected*/
#define EVENT_STATUS_RFOFF_DET_EVENT_POS                                                                 (3UL)
#define EVENT_STATUS_RFOFF_DET_EVENT_MASK                                                                (0x00000008UL)  /**< 3-Indicated the availability of Boot event.RF OFF detected*/
#define EVENT_STATUS_STANDBY_PREV_EVENT_POS                                                              (2UL)
#define EVENT_STATUS_STANDBY_PREV_EVENT_MASK                                                             (0x00000004UL)  /**< 2-Indicated the availability of Standby Prevention reason.*/
#define EVENT_STATUS_GENERAL_ERROR_EVENT_POS                                                             (1UL)
#define EVENT_STATUS_GENERAL_ERROR_EVENT_MASK                                                            (0x00000002UL)  /**< 1-Indicated the availability of General Error event.*/
#define EVENT_STATUS_BOOT_EVENT_POS                                                                      (0UL)
#define EVENT_STATUS_BOOT_EVENT_MASK                                                                     (0x00000001UL)  /**< 0-Indicated the availability of Boot event.*/

#define EMD_CONTROL                                                                                       0x3
#define EMD_CONTROL_EMD_TRANSMISSION_TIMER_USED_POS                                                      (8UL)
#define EMD_CONTROL_EMD_TRANSMISSION_TIMER_USED_MASK                                                     (0x00000300UL)  /**< 8-Timer used for RF communication. */
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_B_POS                                         (7UL)
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_B_MASK                                        (0x00000080UL)  /**< 7-Missing CRC treated as protocol error in case of Type B based communication*/
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_A_POS                                         (6UL)
#define EMD_CONTROL_EMD_MISSING_CRC_IS_PROTOCOL_ERROR_TYPE_A_MASK                                        (0x00000040UL)  /**< 6-Missing CRC treated as protocol error in case of Type A based communication*/
#define EMD_CONTROL_EMD_NOISE_BYTES_THRESHOLD_POS                                                        (2UL)
#define EMD_CONTROL_EMD_NOISE_BYTES_THRESHOLD_MASK                                                       (0x0000003CUL)  /**< 2-Defines the threshold under which transmission errors are treated as noise. Note: CRC bytes are NOT included/counted!*/
#define EMD_CONTROL_EMD_TRANSMISSION_ERROR_ABOVE_NOISE_THRESHOLD_IS_NO_EMD_POS                           (1UL)
#define EMD_CONTROL_EMD_TRANSMISSION_ERROR_ABOVE_NOISE_THRESHOLD_IS_NO_EMD_MASK                          (0x00000002UL)  /**< 1-Transmission errors with received byte length >= EMD_NOISE_BYTES_THRESHOLD is never treated as EMD (EMVCo 2.5 standard)*/
#define EMD_CONTROL_EMD_ENABLE_POS                                                                       (0UL)
#define EMD_CONTROL_EMD_ENABLE_MASK                                                                      (0x00000001UL)  /**< 0-EMD handling enabled*/

#define FELICA_EMD_CONTROL                                                                                0x4
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_BYTE_VALUE_POS                                                  (24UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_BYTE_VALUE_MASK                                                 (0xFF000000UL)  /**< 24-RC byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MAX_POS                                                (16UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MAX_MASK                                               (0x00FF0000UL)  /**< 16-Maximum Legth byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MIN_POS                                                (8UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LENGTH_BYTE_MIN_MASK                                               (0x0000FF00UL)  /**< 8-Minimum Legth byte value that needs to be received to not treat the frame as EMD*/
#define FELICA_EMD_CONTROL_RESERVED_POS                                                                  (5UL)
#define FELICA_EMD_CONTROL_RESERVED_MASK                                                                 (0x000000E0UL)  /**< 5-Reserved */
#define FELICA_EMD_CONTROL_FELICA_EMD_INTEGRITY_ERR_CHECK_ENABLE_POS                                     (4UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_INTEGRITY_ERR_CHECK_ENABLE_MASK                                    (0x00000010UL)  /**< 4-FeliCa EMD handling enabled when integrity error is set*/
#define FELICA_EMD_CONTROL_FELICA_EMD_PROTOCOL_ERR_CHECK_ENABLE_POS                                      (3UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_PROTOCOL_ERR_CHECK_ENABLE_MASK                                     (0x00000008UL)  /**< 3-FeliCa EMD handling enabled when protocol error is set*/
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_CHECK_ENABLE_POS                                                (2UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_RC_CHECK_ENABLE_MASK                                               (0x00000004UL)  /**< 2-RC byte check enabled for FeliCa EMD handling */
#define FELICA_EMD_CONTROL_FELICA_EMD_LEN_CHECK_ENABLE_POS                                               (1UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_LEN_CHECK_ENABLE_MASK                                              (0x00000002UL)  /**< 1-Length byte check enabled for FeliCa EMD handling*/
#define FELICA_EMD_CONTROL_FELICA_EMD_ENABLE_POS                                                         (0UL)
#define FELICA_EMD_CONTROL_FELICA_EMD_ENABLE_MASK                                                        (0x00000001UL)  /**< 0-FeliCa EMD handling enabled*/

#define CLIF_RX_STATUS                                                                                    0x5
#define CLIF_RX_STATUS_RESERVED_POS                                                                      (27UL)
#define CLIF_RX_STATUS_RESERVED_MASK                                                                     (0xF8000000UL)  /**< 27-Reserved*/
#define CLIF_RX_STATUS_RX_COLL_POS_POS                                                                   (20UL)
#define CLIF_RX_STATUS_RX_COLL_POS_MASK                                                                  (0x07F00000UL)  /**< 20-Status indicating the bit position of the first collision detected in the data bit. The value is valid only when RX_COLLISION_DETECTED==1. The value of the RX_BIT_ALIGN is also taken intoaccount (RX_COLL_POS = physical bit position in the flow + RX_BIT_ALIGN value).Indicates the collision position in the first 8 bytes only. Can be used during the TypeA/ICODE/EPC anticollision procedure.0x00 - 1st bit 0x01 - 2nd bit...0x7F - 128th bit.The status register is not updated by the collision detected on stop or parity bit.*/
#define CLIF_RX_STATUS_RX_NUM_LAST_BITS_POS                                                              (17UL)
#define CLIF_RX_STATUS_RX_NUM_LAST_BITS_MASK                                                             (0x000E0000UL)  /**< 17-Status indicating the number of valid bits in the last byte written to the System RAM.0x0 - all bits are valid 0x1 - 1 bit is valid...0x7 - 7 bits are valid*/
#define CLIF_RX_STATUS_RX_NUM_FRAMES_RECEIVED_POS                                                        (13UL)
#define CLIF_RX_STATUS_RX_NUM_FRAMES_RECEIVED_MASK                                                       (0x0001E000UL)  /**< 13-Indicates the number of frames received. The value is updated after every normal frame reception in RX_MULTIPLE mode.The value is valid only if the bit RX_MULTIPLE_ENABLE==1.*/
#define CLIF_RX_STATUS_RX_NUM_BYTES_RECEIVED_POS                                                         (0UL)
#define CLIF_RX_STATUS_RX_NUM_BYTES_RECEIVED_MASK                                                        (0x00001FFFUL)  /**< 0-Status indicating the number of bytes written to the System RAM.0x0000 - no data0x0001 - 1 byte...0x1000 - 4K bytes.0x1001 - 0x1FFF - not supported*/

#define CLIF_RX_STATUS_ERROR                                                                              0x6
#define CLIF_RX_STATUS_ERROR_RESERVED_POS                                                                (30UL)
#define CLIF_RX_STATUS_ERROR_RESERVED_MASK                                                               (0xC0000000UL)  /**< 30-Reserved*/
#define CLIF_RX_STATUS_ERROR_EMD_DETECTED_IN_RXDEC_POS                                                   (29UL)
#define CLIF_RX_STATUS_ERROR_EMD_DETECTED_IN_RXDEC_MASK                                                  (0x20000000UL)  /**< 29-The high level indicates that the EMD was detected (in the SigPro or in the RxDecoder or in both) during the reception.*/
#define CLIF_RX_STATUS_ERROR_EMD_DETECTED_IN_SIGPRO_POS                                                  (28UL)
#define CLIF_RX_STATUS_ERROR_EMD_DETECTED_IN_SIGPRO_MASK                                                 (0x10000000UL)  /**< 28-The high level indicates that the EMD was detected on the Physical layer (in the SigPro) during the reception.*/
#define CLIF_RX_STATUS_ERROR_EXT_RFOFF_DETECTED_POS                                                      (27UL)
#define CLIF_RX_STATUS_ERROR_EXT_RFOFF_DETECTED_MASK                                                     (0x08000000UL)  /**< 27-The high level indicates that the external RF-field vanished during the reception.*/
#define CLIF_RX_STATUS_ERROR_RX_FRAME_MAXLEN_VIOL_POS                                                    (26UL)
#define CLIF_RX_STATUS_ERROR_RX_FRAME_MAXLEN_VIOL_MASK                                                   (0x04000000UL)  /**< 26-The high level indicates that the received frame length violated the configured maximum limit.*/
#define CLIF_RX_STATUS_ERROR_RX_FRAME_MINLEN_VIOL_POS                                                    (25UL)
#define CLIF_RX_STATUS_ERROR_RX_FRAME_MINLEN_VIOL_MASK                                                   (0x02000000UL)  /**< 25-The high level indicates that the received frame length violated the configured minimum limit.*/
#define CLIF_RX_STATUS_ERROR_RX_FRAME_LE_CRC_POS                                                         (24UL)
#define CLIF_RX_STATUS_ERROR_RX_FRAME_LE_CRC_MASK                                                        (0x01000000UL)  /**< 24-The high level indicates that the received frame length is less or equal to the expected CRC field length.*/
#define CLIF_RX_STATUS_ERROR_RX_NOT_FULL_BYTE_POS                                                        (23UL)
#define CLIF_RX_STATUS_ERROR_RX_NOT_FULL_BYTE_MASK                                                       (0x00800000UL)  /**< 23-The high level indicates that the last received character in the frame has less than 8 bits.*/
#define CLIF_RX_STATUS_ERROR_RX_MISSING_PARBIT_DETECTED_POS                                              (22UL)
#define CLIF_RX_STATUS_ERROR_RX_MISSING_PARBIT_DETECTED_MASK                                             (0x00400000UL)  /**< 22-The high level indicates that the last received character in the frame has 8 data bits but the expected parity bit is absent.*/
#define CLIF_RX_STATUS_ERROR_RX_MISSING_STOPBIT_DETECTED_POS                                             (21UL)
#define CLIF_RX_STATUS_ERROR_RX_MISSING_STOPBIT_DETECTED_MASK                                            (0x00200000UL)  /**< 21-The high level indicates that the last received character in the frame has 8 data bits but the expected stop bit is absent.*/
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_PARBIT_DETECTED_POS                                            (20UL)
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_PARBIT_DETECTED_MASK                                           (0x00100000UL)  /**< 20-The high level indicates that the collision was detected on the parity bit position.*/
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_STOPBIT_DETECTED_POS                                           (19UL)
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_STOPBIT_DETECTED_MASK                                          (0x00080000UL)  /**< 19- The high level indicates that the collision was detected on the stop bit position.*/
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_DETECTED_POS                                                   (18UL)
#define CLIF_RX_STATUS_ERROR_RX_COLLISION_DETECTED_MASK                                                  (0x00040000UL)  /**< 18-The high level indicates that the collision was detected during the frame reception.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_RXOVER_POS                                                       (17UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_RXOVER_MASK                                                      (0x00020000UL)  /**< 17-The high level indicates that the frame reception was stopped by SGP_MSG_RXOVER_* message reception.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_RFOFF_POS                                                        (16UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_RFOFF_MASK                                                       (0x00010000UL)  /**< 16-The high level indicates that the frame reception was interrupted by external RF-field vanishing event.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_ERR_POS                                                          (15UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_ERR_MASK                                                         (0x00008000UL)  /**< 15-The high level indicates that the frame reception was stopped by detected communication error event.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_LEN_POS                                                          (14UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_LEN_MASK                                                         (0x00004000UL)  /**< 14-The high level indicates that the frame reception was normally stopped by byte counter expiration event. Relates to the protocols where the LEN field is used in the frame format (Felica RM/CM, FWEC RM/CM).*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_INVPAR_POS                                                       (13UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_INVPAR_MASK                                                      (0x00002000UL)  /**< 13-The high level indicates that the frame reception was normally stopped by the inverted parity detection event. Relates to the TypeA RM 212-848 kbps modes. 12 RX_STOP_ON_PATTERN R 0h The high level indicates that the frame reception was normally stopped by EOF pattern detection event. Relates to the TypeB RM/CM, B prime RM/CM modes.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_PATTERN_POS                                                      (12UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_PATTERN_MASK                                                     (0x00001000UL)  /**< 12-The high level indicates that the frame reception was normally stopped by EOF pattern detection event. Relates to the TypeB RM/CM, B prime RM/CM modes.*/
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_ANTICOLL_POS                                                     (11UL)
#define CLIF_RX_STATUS_ERROR_RX_STOP_ON_ANTICOLL_MASK                                                    (0x00000800UL)  /**< 11-The high level indicates that the frame reception was normally stopped by collision detected on data bit position. Relates to the bit-oriented frame reception in TypeA RM 106 kbps mode during the anticollision procedure.*/
#define CLIF_RX_STATUS_ERROR_RX_CRC_ERROR_POS                                                            (10UL)
#define CLIF_RX_STATUS_ERROR_RX_CRC_ERROR_MASK                                                           (0x00000400UL)  /**< 10-The high level indicates that the CRC error is detected in the received frame.*/
#define CLIF_RX_STATUS_ERROR_RX_LEN_ERROR_POS                                                            (9UL)
#define CLIF_RX_STATUS_ERROR_RX_LEN_ERROR_MASK                                                           (0x00000200UL)  /**< 9-The high level is set if the received frame is shorter than the length stated in the received frame LEN field OR if the LEN parameter in the received frame violates the configured [RX_FRAME_MINLEN:RX_FRAME_MAX LEN] limits. Can assert only in the mode swhere the LEN field is used in the frame format (Felica RM/CM, FWEC RM/CM).*/
#define CLIF_RX_STATUS_ERROR_RX_SIGPRO_ERROR_POS                                                         (8UL)
#define CLIF_RX_STATUS_ERROR_RX_SIGPRO_ERROR_MASK                                                        (0x00000100UL)  /**< 8-The high level indicates that the communication error/errors were detected during the frame reception on physical layer(in the SigPro).*/
#define CLIF_RX_STATUS_ERROR_RX_PARITY_ERROR_POS                                                         (7UL)
#define CLIF_RX_STATUS_ERROR_RX_PARITY_ERROR_MASK                                                        (0x00000080UL)  /**< 7-The high level indicates that the parity error was detected during the frame reception.*/
#define CLIF_RX_STATUS_ERROR_RX_STOPBIT_ERROR_POS                                                        (6UL)
#define CLIF_RX_STATUS_ERROR_RX_STOPBIT_ERROR_MASK                                                       (0x00000040UL)  /**< 6-The high level indicates that the stop bit error (0 level instead of 1 on the stop bit position) was detected during the frame reception.*/
#define CLIF_RX_STATUS_ERROR_RX_WRITE_ERROR_POS                                                          (5UL)
#define CLIF_RX_STATUS_ERROR_RX_WRITE_ERROR_MASK                                                         (0x00000020UL)  /**< 5-The high level indicates that the error acknowledge status was received on the CLIF-system interface during the received frame transmission to the System RAM.*/
#define CLIF_RX_STATUS_ERROR_RX_BUFFER_OVFL_ERROR_POS                                                    (4UL)
#define CLIF_RX_STATUS_ERROR_RX_BUFFER_OVFL_ERROR_MASK                                                   (0x00000010UL)  /**< 4-The high level indicates that the data payload length in the received frame exceeds the 28 bytes limit. Relates to the PollReq procedure in the Felica RM mode only.*/
#define CLIF_RX_STATUS_ERROR_RX_LATENCY_ERROR_POS                                                        (3UL)
#define CLIF_RX_STATUS_ERROR_RX_LATENCY_ERROR_MASK                                                       (0x00000008UL)  /**< 3-The high level indicates that the write request flow was corrupted due to traffic congestion on the system interface during the received frame transmission to the System RAM.*/
#define CLIF_RX_STATUS_ERROR_RX_DATA_INTEGRITY_ERROR_POS                                                 (2UL)
#define CLIF_RX_STATUS_ERROR_RX_DATA_INTEGRITY_ERROR_MASK                                                (0x00000004UL)  /**< 2-The high level indicates that the data integrity corruption (parity/CRC/etc error)was detected in the received frame.*/
#define CLIF_RX_STATUS_ERROR_RX_PROTOCOL_ERROR_POS                                                       (1UL)
#define CLIF_RX_STATUS_ERROR_RX_PROTOCOL_ERROR_MASK                                                      (0x00000002UL)  /**< 1-The high level indicates that the protocol requirements violation (stop bit error,missing parity bit, not full byte received, etc) was detected in the received frame.*/
#define CLIF_RX_STATUS_ERROR_RX_CL_ERROR_POS                                                             (0UL)
#define CLIF_RX_STATUS_ERROR_RX_CL_ERROR_MASK                                                            (0x00000001UL)  /**< 0-The high level indicates that some protocol/data integrity erorr/errors were detected during the frame reception*/

#define CLIF_STATUS                                                                                       0x7
#define CLIF_STATUS_RESERVED_POS                                                                         (30UL)
#define CLIF_STATUS_RESERVED_MASK                                                                        (0xC0000000UL)  /**< 30-Reserved */
#define CLIF_STATUS_CRC_OK_POS                                                                           (29UL)
#define CLIF_STATUS_CRC_OK_MASK                                                                          (0x20000000UL)  /**< 29-This bit indicates the status of the actual CRC calculation. If 1 the CRC is correct. meaning the CRC register has the value 0 or the residue value if inverted CRC is used. Note: This flag should only by evaluated at the end of a communication */
#define CLIF_STATUS_RX_SC_DETECTED_POS                                                                   (28UL)
#define CLIF_STATUS_RX_SC_DETECTED_MASK                                                                  (0x10000000UL)  /**< 28-Status signal indicating that a sub-carrier is detected.*/
#define CLIF_STATUS_RX_SOF_DETECTED_POS                                                                  (27UL)
#define CLIF_STATUS_RX_SOF_DETECTED_MASK                                                                 (0x08000000UL)  /**< 27-Status signal indicating that a SOF has been detected.*/
#define CLIF_STATUS_TX_RF_STATUS_POS                                                                     (26UL)
#define CLIF_STATUS_TX_RF_STATUS_MASK                                                                    (0x04000000UL)  /**< 26-If set to 1 this bit indicates that the drivers are turned on. meaning an RF-Field is created by the device itself. */
#define CLIF_STATUS_RF_DET_STATUS_POS                                                                    (25UL)
#define CLIF_STATUS_RF_DET_STATUS_MASK                                                                   (0x02000000UL)  /**< 25-If set to 1 this bit indicates that an external RF-Field is detected by the rf level detectors (after digital filtering) */
#define CLIF_STATUS_ADC_Q_CLIPPING_POS                                                                   (24UL)
#define CLIF_STATUS_ADC_Q_CLIPPING_MASK                                                                  (0x01000000UL)  /**< 24-Indicates that the Q-Channel ADC has clipped (value 0 or 63), This bit is reset with Rx-reset (enabling of receiver).*/
#define CLIF_STATUS_ADC_I_CLIPPING_POS                                                                   (23UL)
#define CLIF_STATUS_ADC_I_CLIPPING_MASK                                                                  (0x00800000UL)  /**< 23-Indicates that the I-Channel ADC has clipped (value 0 or 63), This bit is reset with Rx-reset (enabling of receiver).*/
#define CLIF_STATUS_DPLL_SATURATED_LIMIT_POS                                                             (22UL)
#define CLIF_STATUS_DPLL_SATURATED_LIMIT_MASK                                                            (0x00400000UL)  /**< 22-Status signal indicating that the DPLL has reached its locking limits (integrator is at 0 or maximum)*/
#define CLIF_STATUS_DPLL_SATURATED_LOCK_RANGE_POS                                                        (21UL)
#define CLIF_STATUS_DPLL_SATURATED_LOCK_RANGE_MASK                                                       (0x00200000UL)  /**< 21-Status signal indicating that the DPLL has reached its locking limits. (Saturation range configured via DPLL_SATURATION_VAL)*/
#define CLIF_STATUS_DPLL_FREQ_LOCK_SUPER_FINE_POS                                                        (20UL)
#define CLIF_STATUS_DPLL_FREQ_LOCK_SUPER_FINE_MASK                                                       (0x00100000UL)  /**< 20-Status signal indicating that the DPLL has reached frequency-lock with ~1.4Hz accuracy */
#define CLIF_STATUS_DPLL_FREQ_LOCK_FINE_POS                                                              (19UL)
#define CLIF_STATUS_DPLL_FREQ_LOCK_FINE_MASK                                                             (0x00080000UL)  /**< 19-Status signal indicating that the DPLL has reached frequency-lock with ~5Hz accuracy */
#define CLIF_STATUS_DPLL_FREQ_LOCK_COARSE_POS                                                            (18UL)
#define CLIF_STATUS_DPLL_FREQ_LOCK_COARSE_MASK                                                           (0x00040000UL)  /**< 18-Status signal indicating that the DPLL has reached frequency-lock with ~50Hz accuracy*/
#define CLIF_STATUS_DPLL_PHASE_LOCK_POS                                                                  (17UL)
#define CLIF_STATUS_DPLL_PHASE_LOCK_MASK                                                                 (0x00020000UL)  /**< 17-Status signal indicating that the DPLL has reached phase-lock (typically happens before DPLL_FREQUENCY_LOCK_COARSE is set).*/
#define CLIF_STATUS_DPLL_ENABLE_POS                                                                      (16UL)
#define CLIF_STATUS_DPLL_ENABLE_MASK                                                                     (0x00010000UL)  /**< 16-This bit indicates that the DPLL Controler has enabled the DPLL (RF on RF frequency ok PLL locked)*/
#define CLIF_STATUS_RESERVED1_POS                                                                         (15UL)
#define CLIF_STATUS_RESERVED1_MASK                                                                        (0x00008000UL)  /**< 15-Reserved */
#define CLIF_STATUS_BMA_TRANSFER_ONGOING_POS                                                             (14UL)
#define CLIF_STATUS_BMA_TRANSFER_ONGOING_MASK                                                            (0x00004000UL)  /**< 14-Status signal from Buffer Manager to indicate that a transfer is actually ongoing.*/
#define CLIF_STATUS_TX_READ_ERROR_POS                                                                    (13UL)
#define CLIF_STATUS_TX_READ_ERROR_MASK                                                                   (0x00002000UL)  /**< 13-This error flag is set to 1 if for an ongoing transmission data is not copied from RAM in time (BMA encountered read error) and therefor the transmission is aborted            Note: This case should not happen in normal operation*/
#define CLIF_STATUS_TX_LATENCY_ERROR_POS                                                                 (12UL)
#define CLIF_STATUS_TX_LATENCY_ERROR_MASK                                                                (0x00001000UL)  /**< 12-This error flag is set to 1. if for an ongoing transmission data is not available in time (BMA latency to big) and therefor the transmission is aborted. Note: This case should not happen in normal operation */
#define CLIF_STATUS_TX_NO_DATA_ERROR_POS                                                                 (11UL)
#define CLIF_STATUS_TX_NO_DATA_ERROR_MASK                                                                (0x00000800UL)  /**< 11-This error flag is set to 1. in case a transmission is started but no data is available (register NumBytesToSend == 0). */
#define CLIF_STATUS_RF_ACTIVE_ERROR_CAUSE_POS                                                            (8UL)
#define CLIF_STATUS_RF_ACTIVE_ERROR_CAUSE_MASK                                                           (0x00000700UL)  /**< 8-This status flag indicates the cause of an NFC-Active error. Note: This bits are only valid when the RF_ACTIVE_ERROR_IRQ is raised and will be cleared as soon as the bit TX_RF_ENABLE is set to 1. 0* No Error. reset value 1 External field was detected on within TIDT timing 2 External field was detected on within TADT timing 3 No external field was detected within TADT timings 4 Peer did switch off RFField without but no Rx event was raised (no data received) 5 - 7 Reserved.*/
#define CLIF_STATUS_RESERVED2_POS                                                                         (6UL)
#define CLIF_STATUS_RESERVED2_MASK                                                                        (0x000000C0UL)  /**< 6-Reserved */
#define CLIF_STATUS_RX_ENABLE_POS                                                                        (5UL)
#define CLIF_STATUS_RX_ENABLE_MASK                                                                       (0x00000020UL)  /**< 5-This bit indicates if the RxDecoder is enalbed. If 1 the RxDecoder was enabled by the Transceive Unit and is now ready for data reception */
#define CLIF_STATUS_TX_ACTIVE_POS                                                                        (4UL)
#define CLIF_STATUS_TX_ACTIVE_MASK                                                                       (0x00000010UL)  /**< 4-This bit indicates activity of the TxEncoder. If 1 a transmission is ongoing otherwise the TxEncoder is in idle state.*/
#define CLIF_STATUS_RX_ACTIVE_POS                                                                        (3UL)
#define CLIF_STATUS_RX_ACTIVE_MASK                                                                       (0x00000008UL)  /**< 3-This bit indicates activity of the RxDecoder. If 1 a data reception is ongoing. otherwise the RxDecoder is in idle state. */
#define CLIF_STATUS_TRANSCEIVE_STATE_POS                                                                 (0UL)
#define CLIF_STATUS_TRANSCEIVE_STATE_MASK                                                                (0x00000007UL)  /**< 0-This registers hold the command bits 0* IDLE state 1 WaitTransmit state 2 Transmitting state 3 WaitReceive state 4 WaitForData state 5 Receiving state 6 LoopBack state 7 reserved*/

#define CLIF_TRANSCEIVE_CONTROL                                                                           0x8
#define CLIF_TRANSCEIVE_CONTROL_RESERVED_POS                                                             (30UL)
#define CLIF_TRANSCEIVE_CONTROL_RESERVED_MASK                                                            (0xC0000000UL)  /**< 30-Reserved */
#define CLIF_TRANSCEIVE_CONTROL_STATE_TRIGGER_SELECT_POS                                                 (24UL)
#define CLIF_TRANSCEIVE_CONTROL_STATE_TRIGGER_SELECT_MASK                                                (0x3F000000UL)  /**< 24-Register to select the state to trigger the STATE_CHANGE_IRQ flag. Each bit of the bit field enables one state - several states are possible. Note: If all bits are 0 no IRQ is triggered. xxxxx1 IDLE state enabled to trigger IRQ xxxx1x WaitTransmit state enabled to trigger IRQ  xxx1xx Transmitting state enabled to trigger IRQ xx1xxx WaitReceive state enabled to trigger IRQ x1xxxx WaitForData state enabled to trigger IRQ 1xxxxx Receiving state enabled to trigger IRQ */
#define CLIF_TRANSCEIVE_CONTROL_RESERVED1_POS                                                             (18UL)
#define CLIF_TRANSCEIVE_CONTROL_RESERVED1_MASK                                                            (0x00FC0000UL)  /**< 18-Reserved */
#define CLIF_TRANSCEIVE_CONTROL_TX_WAIT_RFON_ENABLE_POS                                                  (17UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_WAIT_RFON_ENABLE_MASK                                                 (0x00020000UL)  /**< 17-If set to 1 the TxWait guard time will be started when the own RF-Field is switched on. Note: In default configuration the TxWait guard time is started at the end of reception. This feature is intended to be used for NFC-Active communication */
#define CLIF_TRANSCEIVE_CONTROL_RX_WAIT_RFON_ENABLE_POS                                                  (16UL)
#define CLIF_TRANSCEIVE_CONTROL_RX_WAIT_RFON_ENABLE_MASK                                                 (0x00010000UL)  /**< 16-If set to 1 the RxWait guard time will be started when the own RF-Field is switched off and an external RF-Field was detected. Note: In default configuration the RxWait guard time is started at the end of transmission. This feature is intended to be used for NFC-Active communication */
#define CLIF_TRANSCEIVE_CONTROL_TX_BITPHASE_POS                                                          (8UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_BITPHASE_MASK                                                         (0x0000FF00UL)  /**< 8-Defines the number of 13.56MHz cycles used for adjustment of tx_wait to meet the FDT. */
#define CLIF_TRANSCEIVE_CONTROL_RESERVED2_POS                                                             (7UL)
#define CLIF_TRANSCEIVE_CONTROL_RESERVED2_MASK                                                            (0x00000080UL)  /**< 7-Reserved */
#define CLIF_TRANSCEIVE_CONTROL_TX_MILLER_SYNC_ENABLE_POS                                                (6UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_MILLER_SYNC_ENABLE_MASK                                               (0x00000040UL)  /**< 6-If set to 1 guard time is synchronized to miller envelope for transmission. Used for AGC.*/
#define CLIF_TRANSCEIVE_CONTROL_TX_SKIP_SEND_ENABLE_POS                                                  (5UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_SKIP_SEND_ENABLE_MASK                                                 (0x00000020UL)  /**< 5-If set to 1 not transmission is started after tx_wait is expired and START_SEND was set Note: The bit is cleared by HW when the WaitReceive state is entered.*/
#define CLIF_TRANSCEIVE_CONTROL_TX_PICC2_TIMESLOT_ENABLE_POS                                             (4UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_PICC2_TIMESLOT_ENABLE_MASK                                            (0x00000010UL)  /**< 4-If set to 1 two PICC responses are transmitted in different timeslots. Note: Only valid when command Transceive is active. */
#define CLIF_TRANSCEIVE_CONTROL_TX_FRAMESTEP_ENABLE_POS                                                  (3UL)
#define CLIF_TRANSCEIVE_CONTROL_TX_FRAMESTEP_ENABLE_MASK                                                 (0x00000008UL)  /**< 3-If set to 1. at every start of transmission. each byte of data is sent in a seperate frame. SOF and EOF is appended to the data byte according to the framing settings. After one byte is transmitted. the TxEncoder waits for a new start trigger to continue with the next byte. */
#define CLIF_TRANSCEIVE_CONTROL_RX_MULTIPLE_ENABLE_POS                                                   (2UL)
#define CLIF_TRANSCEIVE_CONTROL_RX_MULTIPLE_ENABLE_MASK                                                  (0x00000004UL)  /**< 2-If this bit is set to 1. the receiver is re-activated after the end of a reception. A statusbyte is written to the RAM containing all relevant status information of the frame. Note: Data in RAM is word aligned therefore empty bytes of a data Word in RAM are padded with 0x00 bytes. SW has to calculate the correct address for the following frame. */
#define CLIF_TRANSCEIVE_CONTROL_MILLER_SYNC_ENABLE_POS                                                   (1UL)
#define CLIF_TRANSCEIVE_CONTROL_MILLER_SYNC_ENABLE_MASK                                                  (0x00000002UL)  /**< 1-If set to one miller pulse synchronisation is enabled. Note: TX_BITPHASE is used to adjust the prescaler of the guard timer. Note: This bit is set by HW when TypeA protocol is detected in automatic mode detection. */
#define CLIF_TRANSCEIVE_CONTROL_INITIATOR_POS                                                            (0UL)
#define CLIF_TRANSCEIVE_CONTROL_INITIATOR_MASK                                                           (0x00000001UL)  /**< 0-Set to 1. the CLIF is configured for initiator mode. Depending on this setting the behavior of the transceive command is different*/

#define CLIF_TX_SYMBOL01_MOD                                                                              0x9
#define CLIF_TX_SYMBOL01_MOD_RESERVED_POS                                                                (24UL)
#define CLIF_TX_SYMBOL01_MOD_RESERVED_MASK                                                               (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_TX_SYMBOL01_MOD_TX_S01_MODWIDTH_POS                                                         (16UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_MODWIDTH_MASK                                                        (0x00FF0000UL)  /**< 16-Specifies the length of a pulse for sending data of symbol 0/1. The length is given by the number of carrier clocks + 1. */
#define CLIF_TX_SYMBOL01_MOD_RESERVED1_POS                                                                (9UL)
#define CLIF_TX_SYMBOL01_MOD_RESERVED1_MASK                                                               (0x0000FE00UL)  /**< 9-Reserved */
#define CLIF_TX_SYMBOL01_MOD_TX_S01_MILLER_ENABLE_POS                                                    (8UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_MILLER_ENABLE_MASK                                                   (0x00000100UL)  /**< 8-If set to 1. pulse modulation is applied according to modified miller coding. */
#define CLIF_TX_SYMBOL01_MOD_TX_S01_INV_ENV_POS                                                          (7UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_INV_ENV_MASK                                                         (0x00000080UL)  /**< 7-If set to 1. the output envelope is inverted. */
#define CLIF_TX_SYMBOL01_MOD_TX_S01_ENV_TYPE_POS                                                         (4UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_ENV_TYPE_MASK                                                        (0x00000070UL)  /**< 4-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU*/
#define CLIF_TX_SYMBOL01_MOD_TX_S01_SC_FREQ_POS                                                          (3UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_SC_FREQ_MASK                                                         (0x00000008UL)  /**< 3-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz */
#define CLIF_TX_SYMBOL01_MOD_TX_S01_BIT_FREQ_POS                                                         (0UL)
#define CLIF_TX_SYMBOL01_MOD_TX_S01_BIT_FREQ_MASK                                                        (0x00000007UL)  /**< 0-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.*/

#define CLIF_TX_SYMBOL1_DEF                                                                               0xA
#define CLIF_TX_SYMBOL1_DEF_TX_SYMBOL1_DEF_POS                                                           (0UL)
#define CLIF_TX_SYMBOL1_DEF_TX_SYMBOL1_DEF_MASK                                                          (0xFFFFFFFFUL)  /**< 0-Pattern definition for Symbol1 */

#define CLIF_TX_SYMBOL0_DEF                                                                               0xB
#define CLIF_TX_SYMBOL0_DEF_TX_SYMBOL0_DEF_POS                                                           (0UL)
#define CLIF_TX_SYMBOL0_DEF_TX_SYMBOL0_DEF_MASK                                                          (0xFFFFFFFFUL)  /**< 0-Pattern definition for Symbol0*/

#define CLIF_TX_SYMBOL23_MOD                                                                              0xC
#define CLIF_TX_SYMBOL23_MOD_RESERVED_POS                                                                (24UL)
#define CLIF_TX_SYMBOL23_MOD_RESERVED_MASK                                                               (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_TX_SYMBOL23_MOD_TX_S23_MODWIDTH_POS                                                         (16UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_MODWIDTH_MASK                                                        (0x00FF0000UL)  /**< 16-Specifies the length of a pulse for sending data of symbol 2/3. The length is given by the number of carrier clocks + 1.*/
#define CLIF_TX_SYMBOL23_MOD_RESERVED1_POS                                                                (9UL)
#define CLIF_TX_SYMBOL23_MOD_RESERVED1_MASK                                                               (0x0000FE00UL)  /**< 9-Reserved */
#define CLIF_TX_SYMBOL23_MOD_TX_S23_MILLER_ENABLE_POS                                                    (8UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_MILLER_ENABLE_MASK                                                   (0x00000100UL)  /**< 8-If set to 1 pulse modulation is applied according to modified miller coding.*/
#define CLIF_TX_SYMBOL23_MOD_TX_S23_INV_ENV_POS                                                          (7UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_INV_ENV_MASK                                                         (0x00000080UL)  /**< 7-If set to 1 the output envelope is inverted.*/
#define CLIF_TX_SYMBOL23_MOD_TX_S23_ENV_TYPE_POS                                                         (4UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_ENV_TYPE_MASK                                                        (0x00000070UL)  /**< 4-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU*/
#define CLIF_TX_SYMBOL23_MOD_TX_S23_SC_FREQ_POS                                                          (3UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_SC_FREQ_MASK                                                         (0x00000008UL)  /**< 3-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz*/
#define CLIF_TX_SYMBOL23_MOD_TX_S23_BIT_FREQ_POS                                                         (0UL)
#define CLIF_TX_SYMBOL23_MOD_TX_S23_BIT_FREQ_MASK                                                        (0x00000007UL)  /**< 0-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.*/

#define CLIF_TX_SYMBOL23_DEF                                                                              0xD
#define CLIF_TX_SYMBOL23_DEF_RESERVED_POS                                                                (24UL)
#define CLIF_TX_SYMBOL23_DEF_RESERVED_MASK                                                               (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_TX_SYMBOL23_DEF_TX_SYMBOL3_DEF_POS                                                          (16UL)
#define CLIF_TX_SYMBOL23_DEF_TX_SYMBOL3_DEF_MASK                                                         (0x00FF0000UL)  /**< 16-Pattern definition for Symbol3 */
#define CLIF_TX_SYMBOL23_DEF_RESERVED1_POS                                                                (8UL)
#define CLIF_TX_SYMBOL23_DEF_RESERVED1_MASK                                                               (0x0000FF00UL)  /**< 8-Reserved */
#define CLIF_TX_SYMBOL23_DEF_TX_SYMBOL2_DEF_POS                                                          (0UL)
#define CLIF_TX_SYMBOL23_DEF_TX_SYMBOL2_DEF_MASK                                                         (0x000000FFUL)  /**< 0-Pattern definition for Symbol2*/

#define CLIF_TX_SYMBOL_CONFIG                                                                             0xE
#define CLIF_TX_SYMBOL_CONFIG_RESERVED_POS                                                               (31UL)
#define CLIF_TX_SYMBOL_CONFIG_RESERVED_MASK                                                              (0x80000000UL)  /**< 31-Reserved */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_LEN_POS                                                   (27UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_LEN_MASK                                                  (0x78000000UL)  /**< 27-Specifies the number of bits issued for symbol 1 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit*/
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_TYPE_POS                                                  (26UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_TYPE_MASK                                                 (0x04000000UL)  /**< 26-Specifies the type of the burst of Symbol1 (logical zero / logical one) */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_ONLY_POS                                                  (25UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_ONLY_MASK                                                 (0x02000000UL)  /**< 25-If set to 1. Symbol1 consists only of a burst and no symbol pattern */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_ENABLE_POS                                                (24UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_BURST_ENABLE_MASK                                               (0x01000000UL)  /**< 24-If set to 1. the burst of Symbol0 of the length defined in bit field SYMBOL1_BURST_LEN is enabled         */
#define CLIF_TX_SYMBOL_CONFIG_RESERVED1_POS                                                               (23UL)
#define CLIF_TX_SYMBOL_CONFIG_RESERVED1_MASK                                                              (0x00800000UL)  /**< 23-Reserved */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_LEN_POS                                                   (19UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_LEN_MASK                                                  (0x00780000UL)  /**< 19-Specifies the number of bits issued for symbol 0 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit*/
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_TYPE_POS                                                  (18UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_TYPE_MASK                                                 (0x00040000UL)  /**< 18-Specifies the type of the burst of Symbol0 (logical zero / logical one) */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_ONLY_POS                                                  (17UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_ONLY_MASK                                                 (0x00020000UL)  /**< 17-If set to 1. Symbol0 consists only of a burst and no symbol pattern */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_ENABLE_POS                                                (16UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_BURST_ENABLE_MASK                                               (0x00010000UL)  /**< 16-If set to 1. the burst of Symbol0 of the length defined in bit field SYMBOL0_BURST_LEN is enabled */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL3_LEN_POS                                                         (13UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL3_LEN_MASK                                                        (0x0000E000UL)  /**< 13-Specifies the number of valid bits of the symbol definition of Symbol3. The range is from 1 bit (value 0000) to 8 bit (value 111) */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL2_LEN_POS                                                         (10UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL2_LEN_MASK                                                        (0x00001C00UL)  /**< 10-Specifies the number of valid bits of the symbol definition of Symbol2. The range is from 1 bit (value 0000) to 8bit (value 111) */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_LEN_POS                                                         (5UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL1_LEN_MASK                                                        (0x000003E0UL)  /**< 5-Specifies the number of valid bits of the symbol definition of Symbol1. The range is from 1 bit (value 0000) to 31 bits (value 11110) */
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_LEN_POS                                                         (0UL)
#define CLIF_TX_SYMBOL_CONFIG_TX_SYMBOL0_LEN_MASK                                                        (0x0000001FUL)  /**< 0-Specifies the number of valid bits of the symbol definition of Symbol0. The range is from 1 bit (value 0000) to 31 bits (value 11110)*/

#define CLIF_TX_FRAME_CONFIG                                                                              0xF
#define CLIF_TX_FRAME_CONFIG_RESERVED_POS                                                                (19UL)
#define CLIF_TX_FRAME_CONFIG_RESERVED_MASK                                                               (0xFFF80000UL)  /**< 19-Reserved */
#define CLIF_TX_FRAME_CONFIG_TX_DATA_CODE_TYPE_POS                                                       (16UL)
#define CLIF_TX_FRAME_CONFIG_TX_DATA_CODE_TYPE_MASK                                                      (0x00070000UL)  /**< 16-Specifies the type of encoding of data to be used 000b No special code 001b 1 out of 4 code [I-Code SLI] 010b 1 out of 256 code [I-Code SLI] 011b Pulse interval encoding (PIE) [I-Code EPC-V2] 100b 2bit tuple code (intended only for test purpose) 101-111b Reserved */
#define CLIF_TX_FRAME_CONFIG_TX_STOPBIT_TYPE_POS                                                         (13UL)
#define CLIF_TX_FRAME_CONFIG_TX_STOPBIT_TYPE_MASK                                                        (0x0000E000UL)  /**< 13-Enables the stop bit (logic 1) and extra guard time (logic 1). The value 0 disables transmission of stop-bits. 000b no stop-bit. no EGT 001b stop-bit. no EGT 010b stop-bit + 1 EGT 011b stop-bit + 2 EGT 100b stop-bit + 3 EGT 101b stop-bit + 4 EGT 110b stop-bit + 5 EGT 111b stop-bit + 6 EGT */
#define CLIF_TX_FRAME_CONFIG_TX_STARTBIT_ENABLE_POS                                                      (12UL)
#define CLIF_TX_FRAME_CONFIG_TX_STARTBIT_ENABLE_MASK                                                     (0x00001000UL)  /**< 12-If set to 1. a start-bit (logic 0) will be send */
#define CLIF_TX_FRAME_CONFIG_TX_MSB_FIRST_POS                                                            (11UL)
#define CLIF_TX_FRAME_CONFIG_TX_MSB_FIRST_MASK                                                           (0x00000800UL)  /**< 11-If set to 1. data bytes are interpreted MSB first for data transmission */
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_LAST_INV_ENABLE_POS                                               (10UL)
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_LAST_INV_ENABLE_MASK                                              (0x00000400UL)  /**< 10-If set to 1. the parity bit of last sent data byte is inverted */
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_TYPE_POS                                                          (9UL)
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_TYPE_MASK                                                         (0x00000200UL)  /**< 9-Defines the type of the parity bit 0 Even Parity is calculated 1 Odd parity is calculated       */
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_ENABLE_POS                                                        (8UL)
#define CLIF_TX_FRAME_CONFIG_TX_PARITY_ENABLE_MASK                                                       (0x00000100UL)  /**< 8-If set to 1. a parity bit is calculated and appended to each byte transmitted. If the Transmission Of Data Is Enabled and TX_NUM_BYTES_2_SEND is zero. then a NO_DATA_ERROR occurs. */
#define CLIF_TX_FRAME_CONFIG_RESERVED1_POS                                                                (5UL)
#define CLIF_TX_FRAME_CONFIG_RESERVED1_MASK                                                               (0x000000E0UL)  /**< 5-Reserved */
#define CLIF_TX_FRAME_CONFIG_TX_DATA_ENABLE_POS                                                          (4UL)
#define CLIF_TX_FRAME_CONFIG_TX_DATA_ENABLE_MASK                                                         (0x00000010UL)  /**< 4-If set to 1. transmission of data is enabled otherwise only symbols are tranmitted. */
#define CLIF_TX_FRAME_CONFIG_TX_STOP_SYMBOL_POS                                                          (2UL)
#define CLIF_TX_FRAME_CONFIG_TX_STOP_SYMBOL_MASK                                                         (0x0000000CUL)  /**< 2-Defines which pattern symbol is sent as frame stop-symbol 00b No symbol is sent 01b Symbol1 is sent 10b Symbol2 is sent 11b Symbol3 is sent */
#define CLIF_TX_FRAME_CONFIG_TX_START_SYMBOL_POS                                                         (0UL)
#define CLIF_TX_FRAME_CONFIG_TX_START_SYMBOL_MASK                                                        (0x00000003UL)  /**< 0-Defines which symbol pattern is sent as frame start-symbol 00b No symbol pattern is sent 01b Symbol0 is sent 10b Symbol1 is sent 11b Symbol2 is sent*/

#define CLIF_TX_DATA_MOD                                                                                  0x10
#define CLIF_TX_DATA_MOD_RESERVED_POS                                                                    (25UL)
#define CLIF_TX_DATA_MOD_RESERVED_MASK                                                                   (0xFE000000UL)  /**< 25-Reserved*/
#define CLIF_TX_DATA_MOD_TX_ICODE_DATA_MODWIDTH_ENABLE_POS                                               (24UL)
#define CLIF_TX_DATA_MOD_TX_ICODE_DATA_MODWIDTH_ENABLE_MASK                                              (0x01000000UL)  /**< 24-Enables modulation width of icode data. Width of modulation is defined by the TX_DATA_MODWIDTH field. When 1, we should have TX_DATA_ENV_TYPE=0 and TX_DATA_INV_ENV=0*/
#define CLIF_TX_DATA_MOD_TX_DATA_MODWIDTH_POS                                                            (16UL)
#define CLIF_TX_DATA_MOD_TX_DATA_MODWIDTH_MASK                                                           (0x00FF0000UL)  /**< 16-Specifies the length of a pulse for sending data with miller pulse modulation enabled. The length is given by the number of carrier clocks + 1.*/
#define CLIF_TX_DATA_MOD_RESERVED1_POS                                                                    (9UL)
#define CLIF_TX_DATA_MOD_RESERVED1_MASK                                                                   (0x0000FE00UL)  /**< 9-Reserved*/
#define CLIF_TX_DATA_MOD_TX_DATA_MILLER_ENABLE_POS                                                       (8UL)
#define CLIF_TX_DATA_MOD_TX_DATA_MILLER_ENABLE_MASK                                                      (0x00000100UL)  /**< 8-If set to 1 pulse modulation is applied according to modified miller coding*/
#define CLIF_TX_DATA_MOD_TX_DATA_INV_ENV_POS                                                             (7UL)
#define CLIF_TX_DATA_MOD_TX_DATA_INV_ENV_MASK                                                            (0x00000080UL)  /**< 7-If set to 1 the output envelope is inverted*/
#define CLIF_TX_DATA_MOD_TX_DATA_ENV_TYPE_POS                                                            (4UL)
#define CLIF_TX_DATA_MOD_TX_DATA_ENV_TYPE_MASK                                                           (0x00000070UL)  /**< 4-Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple coding 111b RFU*/
#define CLIF_TX_DATA_MOD_TX_DATA_SC_FREQ_POS                                                             (3UL)
#define CLIF_TX_DATA_MOD_TX_DATA_SC_FREQ_MASK                                                            (0x00000008UL)  /**< 3-Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz*/
#define CLIF_TX_DATA_MOD_TX_DATA_BIT_FREQ_POS                                                            (0UL)
#define CLIF_TX_DATA_MOD_TX_DATA_BIT_FREQ_MASK                                                           (0x00000007UL)  /**< 0-Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.*/

#define CLIF_TX_WAIT                                                                                      0x11
#define CLIF_TX_WAIT_RESERVED_POS                                                                        (28UL)
#define CLIF_TX_WAIT_RESERVED_MASK                                                                       (0xF0000000UL)  /**< 28-Reserved */
#define CLIF_TX_WAIT_TX_WAIT_VALUE_POS                                                                   (8UL)
#define CLIF_TX_WAIT_TX_WAIT_VALUE_MASK                                                                  (0x0FFFFF00UL)  /**< 8-Defines the tx_wait timer reload value. Note: If set to 00000h the tx_wait guard time is disabled Note: This bit is set by HW a protocol is detected in automatic mode detection*/
#define CLIF_TX_WAIT_TX_WAIT_PRESCALER_POS                                                               (0UL)
#define CLIF_TX_WAIT_TX_WAIT_PRESCALER_MASK                                                              (0x000000FFUL)  /**< 0-Defines the prescaler reload value for the tx_wait timer. Note: This bit is set by HW a protocol is  detected in automatic mode detection*/

#define CLIF_CRC_TX_CONFIG                                                                                0x12
#define CLIF_CRC_TX_CONFIG_TX_CRC_PRESET_VALUE_POS                                                       (16UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_PRESET_VALUE_MASK                                                      (0xFFFF0000UL)  /**< 16-Arbitrary preset value for the Tx-Encoder CRC calculation. */
#define CLIF_CRC_TX_CONFIG_RESERVED_POS                                                                  (7UL)
#define CLIF_CRC_TX_CONFIG_RESERVED_MASK                                                                 (0x0000FF80UL)  /**< 7-Reserved */
#define CLIF_CRC_TX_CONFIG_TX_CRC_BYTE2_ENABLE_POS                                                       (6UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_BYTE2_ENABLE_MASK                                                      (0x00000040UL)  /**< 6-If set. the CRC is calculated from the 2nd byte onwards (intended for HID). Note that this option is used in the Tx-Encoder.           */
#define CLIF_CRC_TX_CONFIG_TX_CRC_PRESET_SEL_POS                                                         (3UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_PRESET_SEL_MASK                                                        (0x00000038UL)  /**< 3-Preset value of the CRC register for the Tx-Encoder. For a CRC calculation using 5bits. only the LSByte is used. 000b* 0000h. reset value 001b  6363h 010b A671h 011b  FFFFh 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value TX_CRC_PRESET_VALUE*/
#define CLIF_CRC_TX_CONFIG_TX_CRC_TYPE_POS                                                               (2UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_TYPE_MASK                                                              (0x00000004UL)  /**< 2-Controls the type of CRC calulation for the Tx-Encoder 0* 16bit CRC calculation. reset value 1 5bit CRC calculation */
#define CLIF_CRC_TX_CONFIG_TX_CRC_INV_POS                                                                (1UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_INV_MASK                                                               (0x00000002UL)  /**< 1-Controls the sending of an inverted CRC value by the Tx-Encoder 0* Not inverted CRC checksum. reset value 1 Inverted CRC checksum */
#define CLIF_CRC_TX_CONFIG_TX_CRC_ENABLE_POS                                                             (0UL)
#define CLIF_CRC_TX_CONFIG_TX_CRC_ENABLE_MASK                                                            (0x00000001UL)  /**< 0-If set to one. the Tx-Encoder will compute and transmit a CRC.*/

#define CLIF_TX_UNDERSHOOT_CONFIG                                                                         0x13
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_POS                                              (16UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_MASK                                             (0xFFFF0000UL)  /**< 16-Undershoot pattern which is transmitted after each falling egde. */
#define CLIF_TX_UNDERSHOOT_CONFIG_RESERVED_POS                                                           (8UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_RESERVED_MASK                                                          (0x0000FF00UL)  /**< 8-Reserved*/
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_EXTENDED_TRANSMISSION_POS                                           (6UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_EXTENDED_TRANSMISSION_MASK                                          (0x000000C0UL)  /**< 6-Extends the transmission with envelope of continuously 1 for SL-ALM transmission in BPSK mode. Refer to (PN552-ECO-RF-02-DIG4)*/
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_LAST_SC_ENABLE_POS                                  (5UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_LAST_SC_ENABLE_MASK                                 (0x00000020UL)  /**< 5-This mode activates the undershoot prevention circuit only for the last sub-carrier cycle for card-mode transmission - Note: The bit TX_UNDERSHOOT_PROT_ENABLE must not be set in this mode. */
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_LEN_POS                                          (1UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PATTERN_LEN_MASK                                         (0x0000001EUL)  /**< 1-Defines length of the undershoot prevention pattern (value +1). The pattern is applied starting from the LSB of the defined pattern. all other bits are ignored. */
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_ENABLE_POS                                          (0UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_TX_UNDERSHOOT_PROT_ENABLE_MASK                                         (0x00000001UL)  /**< 0-If set to 1. the undershoot protection is enabled*/

#define CLIF_TX_OVERSHOOT_CONFIG                                                                          0x14
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_POS                                                (16UL)
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_MASK                                               (0xFFFF0000UL)  /**< 16-Overshoot pattern which is transmitted after each rising egde. */
#define CLIF_TX_OVERSHOOT_CONFIG_RESERVED_POS                                                            (5UL)
#define CLIF_TX_OVERSHOOT_CONFIG_RESERVED_MASK                                                           (0x0000FFE0UL)  /**< 5-Reserved */
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_LEN_POS                                            (1UL)
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PATTERN_LEN_MASK                                           (0x0000001EUL)  /**< 1-Defines length of the overshoot prevention pattern (value +1). The pattern is applied starting from the LSB of the defined pattern. all other bits are ignored. */
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PROT_ENABLE_POS                                            (0UL)
#define CLIF_TX_OVERSHOOT_CONFIG_TX_OVERSHOOT_PROT_ENABLE_MASK                                           (0x00000001UL)  /**< 0-If set to 1. the overshoot protection is enabled.*/

#define CLIF_SS_TX_CFG                                                                                    0x15
#define CLIF_SS_TX_CFG_RESERVED_POS                                                                      (14UL)
#define CLIF_SS_TX_CFG_RESERVED_MASK                                                                     (0xFFFFC000UL)  /**< 14-Reserved*/
#define CLIF_SS_TX_CFG_TX2_USE_TX1_CONF_POS                                                              (13UL)
#define CLIF_SS_TX_CFG_TX2_USE_TX1_CONF_MASK                                                             (0x00002000UL)  /**< 13-When 1, the tx1 configuration is used also for tx2: all CLIF_SS_TX2_* registers are discarded and configurations from corresponding CLIF_SS_TX1 register is used.*/
#define CLIF_SS_TX_CFG_TX_FRCZERO_THR_POS                                                                (6UL)
#define CLIF_SS_TX_CFG_TX_FRCZERO_THR_MASK                                                               (0x00001FC0UL)  /**< 6-angle below which output is set to 0. Values lower than 10 (0xa) are forbidden; they would lead to too short transmitters activations.*/
#define CLIF_SS_TX_CFG_TX2_CLK_MODE_DEFAULT_POS                                                          (3UL)
#define CLIF_SS_TX_CFG_TX2_CLK_MODE_DEFAULT_MASK                                                         (0x00000038UL)  /**< 3-TX2 clk mode without field (RM and CM)*/
#define CLIF_SS_TX_CFG_TX1_CLK_MODE_DEFAULT_POS                                                          (0UL)
#define CLIF_SS_TX_CFG_TX1_CLK_MODE_DEFAULT_MASK                                                         (0x00000007UL)  /**< 0-TX1 clk mode without field (RM and CM)*/

#define CLIF_SS_TX1_RMCFG                                                                                 0x16
#define CLIF_SS_TX1_RMCFG_RESERVED_POS                                                                   (25UL)
#define CLIF_SS_TX1_RMCFG_RESERVED_MASK                                                                  (0xFE000000UL)  /**< 25-Reserved*/
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_TRANS_RM_POS                                                      (22UL)
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_TRANS_RM_MASK                                                     (0x01C00000UL)  /**< 22-TX1 clock mode in RM during transition*/
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_MOD_RM_POS                                                        (19UL)
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_MOD_RM_MASK                                                       (0x00380000UL)  /**< 19-TX1 clock mode of modulated wave in RM*/
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_CW_RM_POS                                                         (16UL)
#define CLIF_SS_TX1_RMCFG_TX1_CLK_MODE_CW_RM_MASK                                                        (0x00070000UL)  /**< 16-TX1 clock mode of unmodulated wave in RM*/
#define CLIF_SS_TX1_RMCFG_TX1_AMP_MOD_RM_POS                                                             (8UL)
#define CLIF_SS_TX1_RMCFG_TX1_AMP_MOD_RM_MASK                                                            (0x0000FF00UL)  /**< 8-TX1 amplitude of modulated wave in RM*/
#define CLIF_SS_TX1_RMCFG_TX1_AMP_CW_RM_POS                                                              (0UL)
#define CLIF_SS_TX1_RMCFG_TX1_AMP_CW_RM_MASK                                                             (0x000000FFUL)  /**< 0-TX1 amplitude of unmodulated wave in RM*/

#define CLIF_SS_TX2_RMCFG                                                                                 0x17
#define CLIF_SS_TX2_RMCFG_RESERVED_POS                                                                   (25UL)
#define CLIF_SS_TX2_RMCFG_RESERVED_MASK                                                                  (0xFE000000UL)  /**< 25-Reserved*/
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_TRANS_RM_POS                                                      (22UL)
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_TRANS_RM_MASK                                                     (0x01C00000UL)  /**< 22-TX2 clock mode in RM during transition*/
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_MOD_RM_POS                                                        (19UL)
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_MOD_RM_MASK                                                       (0x00380000UL)  /**< 19-TX2 clock mode of modulated wave in RM*/
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_CW_RM_POS                                                         (16UL)
#define CLIF_SS_TX2_RMCFG_TX2_CLK_MODE_CW_RM_MASK                                                        (0x00070000UL)  /**< 16-TX2 clock mode of unmodulated wave in RM*/
#define CLIF_SS_TX2_RMCFG_TX2_AMP_MOD_RM_POS                                                             (8UL)
#define CLIF_SS_TX2_RMCFG_TX2_AMP_MOD_RM_MASK                                                            (0x0000FF00UL)  /**< 8-TX2 amplitude of modulated wave in RM*/
#define CLIF_SS_TX2_RMCFG_TX2_AMP_CW_RM_POS                                                              (0UL)
#define CLIF_SS_TX2_RMCFG_TX2_AMP_CW_RM_MASK                                                             (0x000000FFUL)  /**< 0-TX2 amplitude of unmodulated wave in RM*/

#define CLIF_SS_TX_SCALE_CFG                                                                              0x18
#define CLIF_SS_TX_SCALE_CFG_TX2_SS_SCALE_UPD_TIME_POS                                                   (24UL)
#define CLIF_SS_TX_SCALE_CFG_TX2_SS_SCALE_UPD_TIME_MASK                                                  (0xFF000000UL)  /**< 24-time between scaling factor inc/decrement to reach TX2_SS_TARGET_SCALE in 8/13.56MHz multiples (0: immediate update with target value)*/
#define CLIF_SS_TX_SCALE_CFG_TX1_SS_SCALE_UPD_TIME_POS                                                   (16UL)
#define CLIF_SS_TX_SCALE_CFG_TX1_SS_SCALE_UPD_TIME_MASK                                                  (0x00FF0000UL)  /**< 16-time between scaling factor inc/decrement to reach TX1_SS_TARGET_SCALE in 8/13.56MHz multiples (0: immediate update with target value)*/
#define CLIF_SS_TX_SCALE_CFG_TX2_SS_TARGET_SCALE_POS                                                     (8UL)
#define CLIF_SS_TX_SCALE_CFG_TX2_SS_TARGET_SCALE_MASK                                                    (0x0000FF00UL)  /**< 8-TX2 scaling factor*/
#define CLIF_SS_TX_SCALE_CFG_TX1_SS_TARGET_SCALE_POS                                                     (0UL)
#define CLIF_SS_TX_SCALE_CFG_TX1_SS_TARGET_SCALE_MASK                                                    (0x000000FFUL)  /**< 0-TX1 scaling factor*/

#define CLIF_SS_TX_TRANS_CFG                                                                              0x19
#define CLIF_SS_TX_TRANS_CFG_RESERVED_POS                                                                (12UL)
#define CLIF_SS_TX_TRANS_CFG_RESERVED_MASK                                                               (0xFFFFF000UL)  /**< 12-Reserved*/
#define CLIF_SS_TX_TRANS_CFG_TX2_SS_TRANS_RATE_POS                                                       (11UL)
#define CLIF_SS_TX_TRANS_CFG_TX2_SS_TRANS_RATE_MASK                                                      (0x00000800UL)  /**< 11-TX2 shaping edge rate: 0: 1/fc, 1: 2/fc. 1/fc should be selected for CM.*/
#define CLIF_SS_TX_TRANS_CFG_TX1_SS_TRANS_RATE_POS                                                       (10UL)
#define CLIF_SS_TX_TRANS_CFG_TX1_SS_TRANS_RATE_MASK                                                      (0x00000400UL)  /**< 10-TX2 shaping edge rate: 0: 1/fc, 1: 2/fc. 1/fc should be selected for CM*/
#define CLIF_SS_TX_TRANS_CFG_TX2_SS_TRANS_LENGTH_POS                                                     (5UL)
#define CLIF_SS_TX_TRANS_CFG_TX2_SS_TRANS_LENGTH_MASK                                                    (0x000003E0UL)  /**< 5-TX2 shaping edge length: from 0 (disable) to 16. for CM, only 0 or 4 values are valid*/
#define CLIF_SS_TX_TRANS_CFG_TX1_SS_TRANS_LENGTH_POS                                                     (0UL)
#define CLIF_SS_TX_TRANS_CFG_TX1_SS_TRANS_LENGTH_MASK                                                    (0x0000001FUL)  /**< 0-TX1 shaping edge length: from 0 (disable) to 16. for CM, only 0 or 4 values are valid*/

#define CLIF_ANACTRL_TX1_GSN                                                                              0x1A
#define CLIF_ANACTRL_TX1_GSN_RESERVED_POS                                                                (25UL)
#define CLIF_ANACTRL_TX1_GSN_RESERVED_MASK                                                               (0xFE000000UL)  /**< 25-Reserved */
#define CLIF_ANACTRL_TX1_GSN_GSN_DEFAULT_TX1_POS                                                         (20UL)
#define CLIF_ANACTRL_TX1_GSN_GSN_DEFAULT_TX1_MASK                                                        (0x01F00000UL)  /**< 20-low side transistor default conductance for tx1*/
#define CLIF_ANACTRL_TX1_GSN_GSN_MOD_CM_TX1_POS                                                          (15UL)
#define CLIF_ANACTRL_TX1_GSN_GSN_MOD_CM_TX1_MASK                                                         (0x000F8000UL)  /**< 15-low side transistor conductance in card mode, mod, for tx1*/
#define CLIF_ANACTRL_TX1_GSN_GSN_CW_CM_TX1_POS                                                           (10UL)
#define CLIF_ANACTRL_TX1_GSN_GSN_CW_CM_TX1_MASK                                                          (0x00007C00UL)  /**< 10-low side transistor conductance in card mode, cw, for tx1*/
#define CLIF_ANACTRL_TX1_GSN_GSN_MOD_RM_TX1_POS                                                          (5UL)
#define CLIF_ANACTRL_TX1_GSN_GSN_MOD_RM_TX1_MASK                                                         (0x000003E0UL)  /**< 5-low side transistor conductance in reader mode, mod, for tx1*/
#define CLIF_ANACTRL_TX1_GSN_GSN_CW_RM_TX1_POS                                                           (0UL)
#define CLIF_ANACTRL_TX1_GSN_GSN_CW_RM_TX1_MASK                                                          (0x0000001FUL)  /**< 0-low side transistor conductance in reader mode, cw, for tx1*/

#define CLIF_ANACTRL_TX2_GSN                                                                              0x1B
#define CLIF_ANACTRL_TX2_GSN_RESERVED_POS                                                                (25UL)
#define CLIF_ANACTRL_TX2_GSN_RESERVED_MASK                                                               (0xFE000000UL)  /**< 25-Reserved */
#define CLIF_ANACTRL_TX2_GSN_GSN_DEFAULT_TX2_POS                                                         (20UL)
#define CLIF_ANACTRL_TX2_GSN_GSN_DEFAULT_TX2_MASK                                                        (0x01F00000UL)  /**< 20-low side transistor default conductance for tx2*/
#define CLIF_ANACTRL_TX2_GSN_GSN_MOD_CM_TX2_POS                                                          (15UL)
#define CLIF_ANACTRL_TX2_GSN_GSN_MOD_CM_TX2_MASK                                                         (0x000F8000UL)  /**< 15-low side transistor conductance in card mode, mod, for tx2*/
#define CLIF_ANACTRL_TX2_GSN_GSN_CW_CM_TX2_POS                                                           (10UL)
#define CLIF_ANACTRL_TX2_GSN_GSN_CW_CM_TX2_MASK                                                          (0x00007C00UL)  /**< 10-low side transistor conductance in card mode, cw, for tx2*/
#define CLIF_ANACTRL_TX2_GSN_GSN_MOD_RM_TX2_POS                                                          (5UL)
#define CLIF_ANACTRL_TX2_GSN_GSN_MOD_RM_TX2_MASK                                                         (0x000003E0UL)  /**< 5-low side transistor conductance in reader mode, mod, for tx2*/
#define CLIF_ANACTRL_TX2_GSN_GSN_CW_RM_TX2_POS                                                           (0UL)
#define CLIF_ANACTRL_TX2_GSN_GSN_CW_RM_TX2_MASK                                                          (0x0000001FUL)  /**< 0-low side transistor conductance in reader mode, cw, for tx2*/

#define CLIF_ANACTRL_TX_GSP                                                                               0x1C
#define CLIF_ANACTRL_TX_GSP_RESERVED_POS                                                                 (30UL)
#define CLIF_ANACTRL_TX_GSP_RESERVED_MASK                                                                (0xC0000000UL)  /**< 30-Reserved */
#define CLIF_ANACTRL_TX_GSP_GSP_DEFAULT_TX2_POS                                                          (25UL)
#define CLIF_ANACTRL_TX_GSP_GSP_DEFAULT_TX2_MASK                                                         (0x3E000000UL)  /**< 25-high side transistor default conductance for tx2*/
#define CLIF_ANACTRL_TX_GSP_GSP_DEFAULT_TX1_POS                                                          (20UL)
#define CLIF_ANACTRL_TX_GSP_GSP_DEFAULT_TX1_MASK                                                         (0x01F00000UL)  /**< 20-high side transistor default conductance for tx1*/
#define CLIF_ANACTRL_TX_GSP_GSP_CM_TX2_POS                                                               (15UL)
#define CLIF_ANACTRL_TX_GSP_GSP_CM_TX2_MASK                                                              (0x000F8000UL)  /**< 15-high side transistor conductance in card mode for tx2*/
#define CLIF_ANACTRL_TX_GSP_GSP_CM_TX1_POS                                                               (10UL)
#define CLIF_ANACTRL_TX_GSP_GSP_CM_TX1_MASK                                                              (0x00007C00UL)  /**< 10-high side transistor conductance in card mode for tx1*/
#define CLIF_ANACTRL_TX_GSP_GSP_RM_TX2_POS                                                               (5UL)
#define CLIF_ANACTRL_TX_GSP_GSP_RM_TX2_MASK                                                              (0x000003E0UL)  /**< 5-high side transistor conductance in reader mode for tx2*/
#define CLIF_ANACTRL_TX_GSP_GSP_RM_TX1_POS                                                               (0UL)
#define CLIF_ANACTRL_TX_GSP_GSP_RM_TX1_MASK                                                              (0x0000001FUL)  /**< 0-high side transistor conductance in reader mode for tx1*/

#define CLIF_SIGPRO_CONFIG                                                                                0x1D
#define CLIF_SIGPRO_CONFIG_DGRM_SOFT_RESET_POS                                                           (31UL)
#define CLIF_SIGPRO_CONFIG_DGRM_SOFT_RESET_MASK                                                          (0x80000000UL)  /**< 31-When high (1) resets the DGRM block.*/
#define CLIF_SIGPRO_CONFIG_DGRM_RESET_ON_FALSE_ALARM_POS                                                 (30UL)
#define CLIF_SIGPRO_CONFIG_DGRM_RESET_ON_FALSE_ALARM_MASK                                                (0x40000000UL)  /**< 30-When high (1) resets the DGRM state on false alarm.*/
#define CLIF_SIGPRO_CONFIG_DGRM_MINMAX_AVG_SEL_POS                                                       (28UL)
#define CLIF_SIGPRO_CONFIG_DGRM_MINMAX_AVG_SEL_MASK                                                      (0x30000000UL)  /**< 28-Defines the number of samples to average ADC I/Q min/max value: averages over 2^(DRGM_MINMAX_AVG_SEL+1) samples.*/
#define CLIF_SIGPRO_CONFIG_SIGPRO_SPARE_CTRL_POS                                                         (26UL)
#define CLIF_SIGPRO_CONFIG_SIGPRO_SPARE_CTRL_MASK                                                        (0x0C000000UL)  /**< 26-Spare Control Bits reserved for ECO and Last Minutes fixes.*/
#define CLIF_SIGPRO_CONFIG_DGRM_SPARE_CTRL_POS                                                           (24UL)
#define CLIF_SIGPRO_CONFIG_DGRM_SPARE_CTRL_MASK                                                          (0x03000000UL)  /**< 24-Spare Control Bits reserved for ECO and Last Minutes fixes.*/
#define CLIF_SIGPRO_CONFIG_CM_DISABLE_BVSFELICA_PROT_POS                                                 (23UL)
#define CLIF_SIGPRO_CONFIG_CM_DISABLE_BVSFELICA_PROT_MASK                                                (0x00800000UL)  /**< 23-Chicken bit to disable the protection preventing typeB detection while Felica decoder is running in GTM*/
#define CLIF_SIGPRO_CONFIG_CM_GTM_TYPEA_LGTH_POS                                                         (21UL)
#define CLIF_SIGPRO_CONFIG_CM_GTM_TYPEA_LGTH_MASK                                                        (0x00600000UL)  /**< 21-Number of typeA bits required to validate typeA*/
#define CLIF_SIGPRO_CONFIG_RM_DCO_FILT_SEL_POS                                                           (19UL)
#define CLIF_SIGPRO_CONFIG_RM_DCO_FILT_SEL_MASK                                                          (0x00180000UL)  /**< 19-*/
#define CLIF_SIGPRO_CONFIG_RESERVED_POS                                                                  (8UL)
#define CLIF_SIGPRO_CONFIG_RESERVED_MASK                                                                 (0x0007FF00UL)  /**< 8-Reserved */
#define CLIF_SIGPRO_CONFIG_RXCTRL_ENABLE_POS                                                             (7UL)
#define CLIF_SIGPRO_CONFIG_RXCTRL_ENABLE_MASK                                                            (0x00000080UL)  /**< 7-Enables the RXCTRL module*/
#define CLIF_SIGPRO_CONFIG_SIGPRO_BAUDRATE_POS                                                           (4UL)
#define CLIF_SIGPRO_CONFIG_SIGPRO_BAUDRATE_MASK                                                          (0x00000070UL)  /**< 4-Baud-Rates: 0x00 .. 6.6kBd; 0x01 .. 26kBd; 0x02 .. 53kBd; 0x03 .. 106kBd; 0x04 .. 212kBd; 0x05 .. 424kBd; 0x06 .. 848kBd; 0x07 .. RFU.*/
#define CLIF_SIGPRO_CONFIG_SIGPRO_FRAMING_POS                                                            (1UL)
#define CLIF_SIGPRO_CONFIG_SIGPRO_FRAMING_MASK                                                           (0x0000000EUL)  /**< 1-Transfer Types::: Card mode: 000: TypeA; 001: NFC; 010: Felica; 011: TypeB; 1xx: RFU. Reader mode: 000: TypeA; 001: NFC; 010: Felica; 011: TypeB; 100: ICode; 101: EPC-V2 4Manchester; 110: EPC-V2 2Manchester; 111: RFU.*/
#define CLIF_SIGPRO_CONFIG_SIGPRO_MODE_SEL_POS                                                           (0UL)
#define CLIF_SIGPRO_CONFIG_SIGPRO_MODE_SEL_MASK                                                          (0x00000001UL)  /**< 0-Selects between Card (1) or Reader (0) mode.*/

#define CLIF_SIGPRO_RM_WAIT                                                                               0x1E
#define CLIF_SIGPRO_RM_WAIT_RM_NOISE_COLL_TH_POS                                                         (25UL)
#define CLIF_SIGPRO_RM_WAIT_RM_NOISE_COLL_TH_MASK                                                        (0xFE000000UL)  /**< 25-Noise threshold used to declare collision.*/
#define CLIF_SIGPRO_RM_WAIT_RM_MF_SC_TH_POS                                                              (20UL)
#define CLIF_SIGPRO_RM_WAIT_RM_MF_SC_TH_MASK                                                             (0x01F00000UL)  /**< 20-Threshold to compare MF output inorder to detect sub-carriers in A106*/
#define CLIF_SIGPRO_RM_WAIT_RM_SC_CNT_RECEIVE_TH_POS                                                     (16UL)
#define CLIF_SIGPRO_RM_WAIT_RM_SC_CNT_RECEIVE_TH_MASK                                                    (0x000F0000UL)  /**< 16-Number of sub-carriers to be received before transitioning to RECEIVE state in Type A106.*/
#define CLIF_SIGPRO_RM_WAIT_RM_BBA_DECR_WAIT_POS                                                         (8UL)
#define CLIF_SIGPRO_RM_WAIT_RM_BBA_DECR_WAIT_MASK                                                        (0x0000FF00UL)  /**< 8-Number of (13.56MHz/2) cycles to supperss A106 re-sync mechanism after reduction in BBA gain*/
#define CLIF_SIGPRO_RM_WAIT_RM_HF_ATT_UPDATE_WAIT_POS                                                    (0UL)
#define CLIF_SIGPRO_RM_WAIT_RM_HF_ATT_UPDATE_WAIT_MASK                                                   (0x000000FFUL)  /**< 0-Number of (13.56MHz/2) cycles to supperss A106 SoF detection after change in HF-attenuator*/

#define CLIF_SIGPRO_RM_CONFIG                                                                             0x1F
#define CLIF_SIGPRO_RM_CONFIG_RESERVED_POS                                                               (30UL)
#define CLIF_SIGPRO_RM_CONFIG_RESERVED_MASK                                                              (0xC0000000UL)  /**< 30-RESERVED*/
#define CLIF_SIGPRO_RM_CONFIG_RM_PREAMBLE_SC_TRIGGER_POS                                                 (25UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_PREAMBLE_SC_TRIGGER_MASK                                                (0x3E000000UL)  /**< 25-Number of sub-carrier cycles required for response detection in BPSK modulated schemes*/
#define CLIF_SIGPRO_RM_CONFIG_RM_OOK_COL_IS_NOISE_POS                                                    (24UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_OOK_COL_IS_NOISE_MASK                                                   (0x01000000UL)  /**< 24-When set, treats collision as noise*/
#define CLIF_SIGPRO_RM_CONFIG_RM_EOFMF_COND_ENABLE_POS                                                   (23UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_EOFMF_COND_ENABLE_MASK                                                  (0x00800000UL)  /**< 23-Enables the IS_eofmf condition to detect EOF. Used in Type A HDR decoder and Felica*/
#define CLIF_SIGPRO_RM_CONFIG_RM_SILENT_COND_ENABLE_POS                                                  (22UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_SILENT_COND_ENABLE_MASK                                                 (0x00400000UL)  /**< 22-Enables the IS_silent condition to detect EOF in Type A HDR and Felica*/
#define CLIF_SIGPRO_RM_CONFIG_RM_BIT2BIT_VALUE_COND_ENABLE_POS                                           (21UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_BIT2BIT_VALUE_COND_ENABLE_MASK                                          (0x00200000UL)  /**< 21-Enables the bit-2-bit similarity figure condition to detect silence. Used for data rates higher than 106kBd.*/
#define CLIF_SIGPRO_RM_CONFIG_RM_BIT2BIT_RATIO_COND_ENABLE_POS                                           (20UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_BIT2BIT_RATIO_COND_ENABLE_MASK                                          (0x00100000UL)  /**< 20-Enables the bit-2-bit similarity figure ratio condition to detect silence. Used for data rates higher than 106kBd.*/
#define CLIF_SIGPRO_RM_CONFIG_RM_DOWNMIX_MODE_POS                                                        (19UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_DOWNMIX_MODE_MASK                                                       (0x00080000UL)  /**< 19-Defines the down sampling mode*/
#define CLIF_SIGPRO_RM_CONFIG_RM_EOF_2_FALSE_ALARM_POS                                                   (15UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_EOF_2_FALSE_ALARM_MASK                                                  (0x00078000UL)  /**< 15-Defines the number of bits to differentiate false alarm from EOF. Used only in Type A 106kBd and I-Code.*/
#define CLIF_SIGPRO_RM_CONFIG_RM_OOK_ERROR_MODE_POS                                                      (14UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_OOK_ERROR_MODE_MASK                                                     (0x00004000UL)  /**< 14-When set, data_invalid will not occur for OOK modulations*/
#define CLIF_SIGPRO_RM_CONFIG_RM_CH_COMB_MODE_SEL_POS                                                    (12UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_CH_COMB_MODE_SEL_MASK                                                   (0x00003000UL)  /**< 12-Selects channel combiner mode.*/
#define CLIF_SIGPRO_RM_CONFIG_RM_EMD_RESTART_MODE_POS                                                    (11UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_EMD_RESTART_MODE_MASK                                                   (0x00000800UL)  /**< 11-Selects emd restart mode (default 0 for direct restart)*/
#define CLIF_SIGPRO_RM_CONFIG_RM_SILENCE_FACTOR_POS                                                      (8UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_SILENCE_FACTOR_MASK                                                     (0x00000700UL)  /**< 8-Defines the threshold for comparing consecutive bit-to-bit similarity figures. Used in detection of silence for data rates 106kBd and lower.*/
#define CLIF_SIGPRO_RM_CONFIG_RM_CORR_DISTANCE_POS                                                       (5UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_CORR_DISTANCE_MASK                                                      (0x000000E0UL)  /**< 5-Defines the threshold for comparing the distance between the two correlator outputs. Used for data rates higher than 106kBd*/
#define CLIF_SIGPRO_RM_CONFIG_RM_CORR_FACTOR_POS                                                         (2UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_CORR_FACTOR_MASK                                                        (0x0000001CUL)  /**< 2-Defines the threshold for comparing the matched filter. Used for data rates higher than 106kBd*/
#define CLIF_SIGPRO_RM_CONFIG_RM_EMD_SENSITIVITY_POS                                                     (0UL)
#define CLIF_SIGPRO_RM_CONFIG_RM_EMD_SENSITIVITY_MASK                                                    (0x00000003UL)  /**< 0-Defines the threshold for EMD.*/

#define CLIF_SIGPRO_RM_PATTERN                                                                            0x20
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_POS                                                       (16UL)
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_MASK                                                      (0xFFFF0000UL)  /**< 16-Sync pattern for Felica. LSB transmitted last*/
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_EXT4_POS                                                  (15UL)
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_EXT4_MASK                                                 (0x00008000UL)  /**< 15-extend Felica sync pattern with 16 leading 0s*/
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_EXT2_POS                                                  (14UL)
#define CLIF_SIGPRO_RM_PATTERN_RM_SYNC_PATTERN_EXT2_MASK                                                 (0x00004000UL)  /**< 14-extend Felica sync pattern with 8 leading 0s*/
#define CLIF_SIGPRO_RM_PATTERN_RM_RECEIVE_TILL_END_POS                                                   (13UL)
#define CLIF_SIGPRO_RM_PATTERN_RM_RECEIVE_TILL_END_MASK                                                  (0x00002000UL)  /**< 13-do not stop the reception before RxDecoder sends a stop command*/
#define CLIF_SIGPRO_RM_PATTERN_RESERVED_POS                                                              (12UL)
#define CLIF_SIGPRO_RM_PATTERN_RESERVED_MASK                                                             (0x00001000UL)  /**< 12-Reserved*/
#define CLIF_SIGPRO_RM_PATTERN_RM_SOF_PATTERN_POS                                                        (0UL)
#define CLIF_SIGPRO_RM_PATTERN_RM_SOF_PATTERN_MASK                                                       (0x00000FFFUL)  /**< 0-SOF pattern for Type B. LSB transmitted last or Start Byte pattern for NFC passive.*/

#define CLIF_SIGPRO_RM_ENABLES                                                                            0x21
#define CLIF_SIGPRO_RM_ENABLES_RM_STOP_ON_EGT_ERROR_POS                                                  (31UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_STOP_ON_EGT_ERROR_MASK                                                 (0x80000000UL)  /**< 31-When enabled (A0 mode), Type B FSM stops on EGT error. When disabled, FSM continues reception after asserting EGT error.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_STOP_AFTER_2_ATTEMPTS_POS                                              (30UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_STOP_AFTER_2_ATTEMPTS_MASK                                             (0x40000000UL)  /**< 30-When enabled (A0 mode), A106 FSM stops after the 2nd EMD restart if the 1st EMD restart was followed by 4 or more valid bits. When disabled, A106 FSM continues to restart on EMD and false alarms.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_EOF_2_VALID_DELAY_POS                                                  (26UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_EOF_2_VALID_DELAY_MASK                                                 (0x3C000000UL)  /**< 26-Defines the #of ETU cycles after EOF detection when valid is released. Used only in Type A 106kBd I-Code and EPC.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_SOF_COLL_DET_ENABLE_POS                                                (25UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_SOF_COLL_DET_ENABLE_MASK                                               (0x02000000UL)  /**< 25-Enables collision detection at SOF to declare false alarm*/
#define CLIF_SIGPRO_RM_ENABLES_RM_UPDT_SYNCREF_ON_CLIPPING_POS                                           (24UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_UPDT_SYNCREF_ON_CLIPPING_MASK                                          (0x01000000UL)  /**< 24-Update sync_ref_level when ADC data is clipped*/
#define CLIF_SIGPRO_RM_ENABLES_RM_RESET_ONLY_ON_FALSE_ALARM_POS                                          (23UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_RESET_ONLY_ON_FALSE_ALARM_MASK                                         (0x00800000UL)  /**< 23-Enables reset only on false alarm and not for correct frame or re-sync*/
#define CLIF_SIGPRO_RM_ENABLES_RM_DELAY_ONLY_WAITRES_POS                                                 (22UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_DELAY_ONLY_WAITRES_MASK                                                (0x00400000UL)  /**< 22-In TypeB mode, when set, transition from WAITRESDELAY to WAITSYNC is only based on delay counter. Else, the transition will be based on the MF outputs as well.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_EMD_TH_BASED_ON_SIGNAL_POS                                             (21UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_EMD_TH_BASED_ON_SIGNAL_MASK                                            (0x00200000UL)  /**< 21-When high, EMD threshold = max(signal_level)/2. Else, EMD threshold is based on max_noise*/
#define CLIF_SIGPRO_RM_ENABLES_RM_COMM_TH_TRACK_ENABLE_POS                                               (20UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_COMM_TH_TRACK_ENABLE_MASK                                              (0x00100000UL)  /**< 20-Enables tracking of comm threshold for A106 based on signal level*/
#define CLIF_SIGPRO_RM_ENABLES_RM_SUP_COLL_DET_ON_GAIN_CHG_POS                                           (19UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_SUP_COLL_DET_ON_GAIN_CHG_MASK                                          (0x00080000UL)  /**< 19-When high, suppress collision detection when BBA gain is reduced*/
#define CLIF_SIGPRO_RM_ENABLES_RM_OOK_COL_HIGH_SLOPE_POS                                                 (17UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_OOK_COL_HIGH_SLOPE_MASK                                                (0x00060000UL)  /**< 17-EMD upper slope for the collision region 0: Threshold of 1; 1: Threshold of 1.125; 2: Threshold of 1.25; 3: Threshold of 1.5*/
#define CLIF_SIGPRO_RM_ENABLES_RM_OOK_COL_LOW_SLOPE_POS                                                  (15UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_OOK_COL_LOW_SLOPE_MASK                                                 (0x00018000UL)  /**< 15-EMD lower slope for the collision region 0: Threshold of 1; 1: Threshold of 0.875; 2: Threshold of 0.75; 3: Threshold of 0.5*/
#define CLIF_SIGPRO_RM_ENABLES_RM_NOISE_INVALID_TH_POS                                                   (8UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_NOISE_INVALID_TH_MASK                                                  (0x00007F00UL)  /**< 8-Noise threshold used to declare invalid data.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_LMA_READ_ENABLE_POS                                                    (7UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_LMA_READ_ENABLE_MASK                                                   (0x00000080UL)  /**< 7-When set, HW updates RM_LOAD_MOD_AMPLITUDE.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_LMA_TRACKING_ENABLE_POS                                                (6UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_LMA_TRACKING_ENABLE_MASK                                               (0x00000040UL)  /**< 6-Enables load modulation amplitude tracking through the frame.*/
#define CLIF_SIGPRO_RM_ENABLES_RM_EMD_RESET_ENABLE_POS                                                   (5UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_EMD_RESET_ENABLE_MASK                                                  (0x00000020UL)  /**< 5-Enables EMD reset handling Type A 106kBd, EPC and I-Code*/
#define CLIF_SIGPRO_RM_ENABLES_RM_SILENT_RESET_ENABLE_POS                                                (4UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_SILENT_RESET_ENABLE_MASK                                               (0x00000010UL)  /**< 4-Enables receiver reset due to silence detection*/
#define CLIF_SIGPRO_RM_ENABLES_RM_COLLISION_DET_ENABLE_POS                                               (3UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_COLLISION_DET_ENABLE_MASK                                              (0x00000008UL)  /**< 3-Enables receiver reset due to collision detection*/
#define CLIF_SIGPRO_RM_ENABLES_RM_FALSE_ALARM_SOF_RESET_ENABLE_POS                                       (2UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_FALSE_ALARM_SOF_RESET_ENABLE_MASK                                      (0x00000004UL)  /**< 2-Enables receiver reset due to false alarm*/
#define CLIF_SIGPRO_RM_ENABLES_RM_RESYNC_RESET_ENABLE_POS                                                (1UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_RESYNC_RESET_ENABLE_MASK                                               (0x00000002UL)  /**< 1-Enables receiver reset due to re-sync*/
#define CLIF_SIGPRO_RM_ENABLES_RM_WDT_RESET_ENABLE_POS                                                   (0UL)
#define CLIF_SIGPRO_RM_ENABLES_RM_WDT_RESET_ENABLE_MASK                                                  (0x00000001UL)  /**< 0-Enables receiver reset when watch dog timer expires*/

#define CLIF_SIGPRO_RM_TECH                                                                               0x22
#define CLIF_SIGPRO_RM_TECH_RM_NCO_PERIOD_SEL_POS                                                        (30UL)
#define CLIF_SIGPRO_RM_TECH_RM_NCO_PERIOD_SEL_MASK                                                       (0xC0000000UL)  /**< 30-Defines the reset value for the NCO counter*/
#define CLIF_SIGPRO_RM_TECH_RM_WAIT_RES_PERIOD_SEL_POS                                                   (27UL)
#define CLIF_SIGPRO_RM_TECH_RM_WAIT_RES_PERIOD_SEL_MASK                                                  (0x38000000UL)  /**< 27-Defines the reset value for the Delay counter*/
#define CLIF_SIGPRO_RM_TECH_RM_EGT_WINDOW_TH_SEL_POS                                                     (26UL)
#define CLIF_SIGPRO_RM_TECH_RM_EGT_WINDOW_TH_SEL_MASK                                                    (0x04000000UL)  /**< 26-Defines the EGT window threshold for Type B*/
#define CLIF_SIGPRO_RM_TECH_RM_DC_REMOVAL_ENABLE_POS                                                     (25UL)
#define CLIF_SIGPRO_RM_TECH_RM_DC_REMOVAL_ENABLE_MASK                                                    (0x02000000UL)  /**< 25-Reserved*/
#define CLIF_SIGPRO_RM_TECH_RM_DOWNSAMPLE_RATE_SEL_POS                                                   (23UL)
#define CLIF_SIGPRO_RM_TECH_RM_DOWNSAMPLE_RATE_SEL_MASK                                                  (0x01800000UL)  /**< 23-Defines the  down sample rate for the reader demod.*/
#define CLIF_SIGPRO_RM_TECH_RM_SOF_NUM_CYCLES_SEL_POS                                                    (20UL)
#define CLIF_SIGPRO_RM_TECH_RM_SOF_NUM_CYCLES_SEL_MASK                                                   (0x00700000UL)  /**< 20-Defines the number of samples in I-Code SOF.*/
#define CLIF_SIGPRO_RM_TECH_RM_MF_SEL_POS                                                                (17UL)
#define CLIF_SIGPRO_RM_TECH_RM_MF_SEL_MASK                                                               (0x000E0000UL)  /**< 17-Defines the selection for the Matched-Filters*/
#define CLIF_SIGPRO_RM_TECH_RM_MF_GAIN_POS                                                               (15UL)
#define CLIF_SIGPRO_RM_TECH_RM_MF_GAIN_MASK                                                              (0x00018000UL)  /**< 15-Defines the gain of the Matched-Filters*/
#define CLIF_SIGPRO_RM_TECH_RM_MRC_WEIGHT_SEL_POS                                                        (13UL)
#define CLIF_SIGPRO_RM_TECH_RM_MRC_WEIGHT_SEL_MASK                                                       (0x00006000UL)  /**< 13-Defines the channel combiner weight on the lower channel*/
#define CLIF_SIGPRO_RM_TECH_RM_AVG_FILT_GAIN_POS                                                         (12UL)
#define CLIF_SIGPRO_RM_TECH_RM_AVG_FILT_GAIN_MASK                                                        (0x00001000UL)  /**< 12-Defines the averaging filter gain*/
#define CLIF_SIGPRO_RM_TECH_RM_AVG_FILT_SEL_POS                                                          (10UL)
#define CLIF_SIGPRO_RM_TECH_RM_AVG_FILT_SEL_MASK                                                         (0x00000C00UL)  /**< 10-Defines the averaging filter selection*/
#define CLIF_SIGPRO_RM_TECH_RM_SYNC_FILT_IN_SEL_POS                                                      (8UL)
#define CLIF_SIGPRO_RM_TECH_RM_SYNC_FILT_IN_SEL_MASK                                                     (0x00000300UL)  /**< 8-Defines the input selection for the sync filter.*/
#define CLIF_SIGPRO_RM_TECH_RM_SYNC_FILT_SEL_POS                                                         (6UL)
#define CLIF_SIGPRO_RM_TECH_RM_SYNC_FILT_SEL_MASK                                                        (0x000000C0UL)  /**< 6-Defines the synchronization filter selection*/
#define CLIF_SIGPRO_RM_TECH_RM_WATCH_DOG_PERIOD_SEL_POS                                                  (3UL)
#define CLIF_SIGPRO_RM_TECH_RM_WATCH_DOG_PERIOD_SEL_MASK                                                 (0x00000038UL)  /**< 3-Defines the reset value for the watch-dog counter*/
#define CLIF_SIGPRO_RM_TECH_RM_EST_RESTART_ENABLE_POS                                                    (2UL)
#define CLIF_SIGPRO_RM_TECH_RM_EST_RESTART_ENABLE_MASK                                                   (0x00000004UL)  /**< 2-Reserved*/
#define CLIF_SIGPRO_RM_TECH_RM_OOK_STAT_LEN_POS                                                          (0UL)
#define CLIF_SIGPRO_RM_TECH_RM_OOK_STAT_LEN_MASK                                                         (0x00000003UL)  /**< 0-Defines the number of samples used to check for invalid at the beginning of a reception in A106 and Icode. Value 0x0 = 2 samples, value 0x1 = 4 samples, value 0x2 = 8 samples, value 0x3 = 16 samples */

#define CLIF_SIGPRO_NOISE_CONFIG1                                                                         0x23
#define CLIF_SIGPRO_NOISE_CONFIG1_RESERVED_POS                                                           (28UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_RESERVED_MASK                                                          (0xF0000000UL)  /**< 28-Reserved */
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_MRC_SCALE_VALUE_POS                                         (25UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_MRC_SCALE_VALUE_MASK                                        (0x0E000000UL)  /**< 25-starts noise power estimation sent by FW*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_START_POS                                                   (24UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_START_MASK                                                  (0x01000000UL)  /**< 24-starts noise power estimation sent by FW*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_FAST_MODE_POS                                               (23UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_FAST_MODE_MASK                                              (0x00800000UL)  /**< 23-enables/disable fast mode*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_INPUT_SEL_POS                                               (22UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_INPUT_SEL_MASK                                              (0x00400000UL)  /**< 22-selects noise power detector input*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_ROUGH_WIN_SIZE_POS                                          (16UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_ROUGH_WIN_SIZE_MASK                                         (0x003F0000UL)  /**< 16-defines noise power detector rough window length*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_FINE_WIN_SIZE_POS                                           (10UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_FINE_WIN_SIZE_MASK                                          (0x0000FC00UL)  /**< 10-defines noise power detector fine window length*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_FACTOR_POS                                             (7UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_FACTOR_MASK                                            (0x00000380UL)  /**< 7-defines noise power factor to apply on the peak noise*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_ABS_TH_POS                                             (5UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_ABS_TH_MASK                                            (0x00000060UL)  /**< 5-defines absolute threshold used to detect peaks*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_REL_TH_POS                                             (3UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_PEAK_REL_TH_MASK                                            (0x00000018UL)  /**< 3-defines relative threshold used to detect peaks*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_AVG_LGTH_POS                                                (1UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_AVG_LGTH_MASK                                               (0x00000006UL)  /**< 1-defines the averaging window length used to calculate noise*/
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_ENABLE_POS                                                  (0UL)
#define CLIF_SIGPRO_NOISE_CONFIG1_SIGPRO_NPD_ENABLE_MASK                                                 (0x00000001UL)  /**< 0-enables noise power detector*/

#define CLIF_SIGPRO_NOISE_CONFIG2                                                                         0x24
#define CLIF_SIGPRO_NOISE_CONFIG2_RESERVED_POS                                                           (24UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_RESERVED_MASK                                                          (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_Q_OVR_ENABLE_POS                                            (23UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_Q_OVR_ENABLE_MASK                                           (0x00800000UL)  /**< 23-enables the overwritting of Q channel noise power value by FW*/
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_I_OVR_ENABLE_POS                                            (22UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_I_OVR_ENABLE_MASK                                           (0x00400000UL)  /**< 22-enables the overwritting of I channel noise power value by FW*/
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_UPDT_COND_POS                                               (20UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_UPDT_COND_MASK                                              (0x00300000UL)  /**< 20-update condition for the noise status register*/
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_Q_OVR_VALUE_POS                                             (10UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_Q_OVR_VALUE_MASK                                            (0x000FFC00UL)  /**< 10-Noise value for Q channel that the FW can force*/
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_I_OVR_VALUE_POS                                             (0UL)
#define CLIF_SIGPRO_NOISE_CONFIG2_SIGPRO_NPD_I_OVR_VALUE_MASK                                            (0x000003FFUL)  /**< 0-Noise value for I channel that the FW can force*/

#define CLIF_RX_PROTOCOL_CONFIG                                                                           0x25
#define CLIF_RX_PROTOCOL_CONFIG_RESERVED_POS                                                             (23UL)
#define CLIF_RX_PROTOCOL_CONFIG_RESERVED_MASK                                                            (0xFF800000UL)  /**< 23-Reserved*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_LEN_SIZE_POS                                                          (22UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_LEN_SIZE_MASK                                                         (0x00400000UL)  /**< 22-If the LEN field is a part of the frame format - the parameter defines the LEN field size in the frame.0: the LEN field consists of 1 byte (E.g. Felica frames).1: the LEN field consists of 2 bytes (E.g. FWEC frames).*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_EOFDET_MASK_POS                                                       (19UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_EOFDET_MASK_MASK                                                      (0x00380000UL)  /**< 19-Bit scale which defines the permitted position of the EOF pattern in the frame. If the corresponding bit of the scale is set to 1, the EOF pattern in the corresponding position is considered. Otherwise its ignored.bit[0] - Next to stop bit position.bit[1] - Next to parity bit position.bit[2] - Next to any data bit position.*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_EOF_LENGTH_POS                                                        (15UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_EOF_LENGTH_MASK                                                       (0x00078000UL)  /**< 15-The length of the EOF pattern set in the CLIF_RX_EOF_PATTERN_REG register in 2-bits symbols units.0 - 1 symbols...15 - 16 symbols.If set to n, only N=n+1 least significant 2-bit symbols are taken from the CLIF_RX_EOF_PATTERN_REG vector for the EOF pattern detection.*/
#define CLIF_RX_PROTOCOL_CONFIG_EOC_COND_SCALE_POS                                                       (13UL)
#define CLIF_RX_PROTOCOL_CONFIG_EOC_COND_SCALE_MASK                                                      (0x00006000UL)  /**< 13-End of Communication (EOC) condition bit scale. If the corresponding bit of the scale is set to 1 - the condition is activated.bit[0] - EOC is detected EOF defined by the EOF_COND_SCALE register value.bit[1] - EOC is External RF-Off event. Any combination of set bits is allowed. The SGP_MSG_RXOVER_* message is hardcoded EOC condition and cant be disabled.*/
#define CLIF_RX_PROTOCOL_CONFIG_EOF_COND_SCALE_POS                                                       (8UL)
#define CLIF_RX_PROTOCOL_CONFIG_EOF_COND_SCALE_MASK                                                      (0x00001F00UL)  /**< 8-End of Frame (EOF) condition bit scale. If the corresponding bit of the scale is set to 1 - the condition is activated.bit[0] - EOF is Data symbol pattern.bit[1] - EOF is Inverted parity.bit[2] - EOF is defined by LEN frame parameter.bit[3] - EOF is TypeA Anticollision pattern. bit[4] - EOF is communication error event. Any combination of set bits is allowed. The SGP_MSG_RXOVER_* message is hardcoded EOF condition and cant be disabled.*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_MSB_FIRST_POS                                                         (7UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_MSB_FIRST_MASK                                                        (0x00000080UL)  /**< 7-If set to 1, the first received bit is MSB in the frame character. Otherwise its LSB.*/
#define CLIF_RX_PROTOCOL_CONFIG_VALUES_AFTER_COLLISION_POS                                               (6UL)
#define CLIF_RX_PROTOCOL_CONFIG_VALUES_AFTER_COLLISION_MASK                                              (0x00000040UL)  /**< 6-If set to 0, every received bit after a collision is replaced by a zero (needed for TypeA Anticollision procedure).if set to 1 - the bits keep their values. The collision bit is always replaced by 1 for both settings.*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_BIT_ALIGN_POS                                                         (3UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_BIT_ALIGN_MASK                                                        (0x00000038UL)  /**< 3-Defines the position of the 1st received data bit in the formed data byte.000b - 0 position...111b - 7th position*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_PARITY_TYPE_POS                                                       (2UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_PARITY_TYPE_MASK                                                      (0x00000004UL)  /**< 2-0 - data + parity bits contain even number of 1. 1 - data + parity bits contain odd number of 1. Valid if cfg_rx_parity_enable_i ==1.*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_PARITY_ENABLE_POS                                                     (1UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_PARITY_ENABLE_MASK                                                    (0x00000002UL)  /**< 1-If set to 1, the bit following last data bit in the frame character is considered as parity bit.*/
#define CLIF_RX_PROTOCOL_CONFIG_RX_STOPBIT_ENABLE_POS                                                    (0UL)
#define CLIF_RX_PROTOCOL_CONFIG_RX_STOPBIT_ENABLE_MASK                                                   (0x00000001UL)  /**< 0-if set to 1, a stopbit is a part of the input frame character.*/

#define CLIF_RX_FRAME_LENGTH                                                                              0x26
#define CLIF_RX_FRAME_LENGTH_RESERVED_POS                                                                (31UL)
#define CLIF_RX_FRAME_LENGTH_RESERVED_MASK                                                               (0x80000000UL)  /**< 31-Reserved*/
#define CLIF_RX_FRAME_LENGTH_RX_FRAME_MAXLEN_POS                                                         (16UL)
#define CLIF_RX_FRAME_LENGTH_RX_FRAME_MAXLEN_MASK                                                        (0x7FFF0000UL)  /**< 16-Maximal number of received [DATA + CRC] bits in theframe. The violation of the maximum length limit can be also configured as an Error/EMD condition. If the max length violation is configured as error - the frame reception is stopped in case of maximum length limit exceeding. Otherwise the reception is continued.0x0000 - 1 bit...0x7FFF - 32K bits*/
#define CLIF_RX_FRAME_LENGTH_RESERVED1_POS                                                                (15UL)
#define CLIF_RX_FRAME_LENGTH_RESERVED1_MASK                                                               (0x00008000UL)  /**< 15-Reserved*/
#define CLIF_RX_FRAME_LENGTH_RX_FRAME_MINLEN_POS                                                         (0UL)
#define CLIF_RX_FRAME_LENGTH_RX_FRAME_MINLEN_MASK                                                        (0x00007FFFUL)  /**< 0-Minimal number of received [DATA + CRC] bits in the frame. The violation of the minimum length limit can be also configured as an Error/EMD condition.The parameter also defines the number of received[DATA + CRC] bits before which any of the EOF pattern or INVPAR stop condition events are ignored.0x0000 - 1 bit...0x7FFF - 32K bits*/

#define CLIF_RX_ERROR_CONFIG                                                                              0x27
#define CLIF_RX_ERROR_CONFIG_RESERVED_POS                                                                (14UL)
#define CLIF_RX_ERROR_CONFIG_RESERVED_MASK                                                               (0xFFFFC000UL)  /**< 14-Reserved*/
#define CLIF_RX_ERROR_CONFIG_RX_NO_STOP_COND_IS_ERROR_POS                                                (13UL)
#define CLIF_RX_ERROR_CONFIG_RX_NO_STOP_COND_IS_ERROR_MASK                                               (0x00002000UL)  /**< 13-If asserted and:1) The frame reception is over and was not interrupted by any detected protocol/data integrity error or external rf-off event AND 2) No any stop conditions, configured via EOF_COND_SCALE register, were detected. The scenario is considered as communication error. If the frame reception was interrupted by Rxdecoder or if any of the configured stop conditions is detected - the no stop conditions error is not asserted.*/
#define CLIF_RX_ERROR_CONFIG_RX_CRC_ERROR_IS_ERROR_POS                                                   (12UL)
#define CLIF_RX_ERROR_CONFIG_RX_CRC_ERROR_IS_ERROR_MASK                                                  (0x00001000UL)  /**< 12-If asserted, the detection of the CRC error is considered as communication error. Otherwise its ignored and just reported to the FW.*/
#define CLIF_RX_ERROR_CONFIG_RX_NOT_FULL_BYTE_IS_ERROR_POS                                               (11UL)
#define CLIF_RX_ERROR_CONFIG_RX_NOT_FULL_BYTE_IS_ERROR_MASK                                              (0x00000800UL)  /**< 11-If asserted, the reception of last byte with size <8 data bits is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_MISSING_PARITY_IS_ERROR_POS                                              (10UL)
#define CLIF_RX_ERROR_CONFIG_RX_MISSING_PARITY_IS_ERROR_MASK                                             (0x00000400UL)  /**< 10- If asserted and the parity bit is a part of the frame character, the reception of the last character with 8 data bits but without parity bit is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_MISSING_STOPBIT_IS_ERROR_POS                                             (9UL)
#define CLIF_RX_ERROR_CONFIG_RX_MISSING_STOPBIT_IS_ERROR_MASK                                            (0x00000200UL)  /**< 9-If asserted and the stop bit is a part of the frame character, the reception of the last character with 8 data bits but without stop bit is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_LEN_ERROR_IS_ERROR_POS                                                   (8UL)
#define CLIF_RX_ERROR_CONFIG_RX_LEN_ERROR_IS_ERROR_MASK                                                  (0x00000100UL)  /**< 8-If asserted and the LEN field is a part of the frame, the wrong received value in the field is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_MINLEN_FRAME_VIOL_IS_ERROR_POS                                           (7UL)
#define CLIF_RX_ERROR_CONFIG_RX_MINLEN_FRAME_VIOL_IS_ERROR_MASK                                          (0x00000080UL)  /**< 7-If asserted and the received frame length violates the min frame length configured via the RX_RX_FRAME_MINLEN register - the event is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_MAXLEN_FRAME_VIOL_IS_ERROR_POS                                           (6UL)
#define CLIF_RX_ERROR_CONFIG_RX_MAXLEN_FRAME_VIOL_IS_ERROR_MASK                                          (0x00000040UL)  /**< 6-If asserted and the received frame length violates the max frame length configured via the RX_RX_FRAME_MAXLEN register - the event is considered as communication error. The Rxdecoder is always stopped on max length violation.*/
#define CLIF_RX_ERROR_CONFIG_RX_SIGPRO_ERR_IS_ERROR_POS                                                  (5UL)
#define CLIF_RX_ERROR_CONFIG_RX_SIGPRO_ERR_IS_ERROR_MASK                                                 (0x00000020UL)  /**< 5-If asserted and either SGP_MSG_RXOVER_ERROR or SGP_MSG_ERROR message is received - the event is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_PARBIT_ERR_IS_ERROR_POS                                                  (4UL)
#define CLIF_RX_ERROR_CONFIG_RX_PARBIT_ERR_IS_ERROR_MASK                                                 (0x00000010UL)  /**< 4-If asserted, the detected parity error is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_STOPBIT_ERR_IS_ERROR_POS                                                 (3UL)
#define CLIF_RX_ERROR_CONFIG_RX_STOPBIT_ERR_IS_ERROR_MASK                                                (0x00000008UL)  /**< 3-If asserted, the detected stop bit error (0 level instead of 1) is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_COLL_PARBIT_IS_ERROR_POS                                                 (2UL)
#define CLIF_RX_ERROR_CONFIG_RX_COLL_PARBIT_IS_ERROR_MASK                                                (0x00000004UL)  /**< 2-If asserted, the collision detected on parity bit position is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_COLL_STOPBIT_IS_ERROR_POS                                                (1UL)
#define CLIF_RX_ERROR_CONFIG_RX_COLL_STOPBIT_IS_ERROR_MASK                                               (0x00000002UL)  /**< 1-If asserted, the collision detected on stop bit position is considered as communication error.*/
#define CLIF_RX_ERROR_CONFIG_RX_COLL_DATA_IS_ERROR_POS                                                   (0UL)
#define CLIF_RX_ERROR_CONFIG_RX_COLL_DATA_IS_ERROR_MASK                                                  (0x00000001UL)  /**< 0-If asserted, the collision detected on any data bit position is considered as communication error.*/

#define CLIF_SPARE1_REG                                                                                   0x28
#define CLIF_SPARE1_REG_RFU_POS                                                                          (0UL)
#define CLIF_SPARE1_REG_RFU_MASK                                                                         (0xFFFFFFFFUL)  /**< 0-RFU*/

#define CLIF_SIGPRO_IIR_CONFIG1                                                                           0x29
#define CLIF_SIGPRO_IIR_CONFIG1_RESERVED_POS                                                             (28UL)
#define CLIF_SIGPRO_IIR_CONFIG1_RESERVED_MASK                                                            (0xF0000000UL)  /**< 28-Reserved */
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B3_POS                                                          (27UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B3_MASK                                                         (0x08000000UL)  /**< 27-IIR B3 sign*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B3_POS                                                          (21UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B3_MASK                                                         (0x07E00000UL)  /**< 21-IIR B3 coef (unsigned, MSB unused) Value is coded value/32*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B2_POS                                                          (20UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B2_MASK                                                         (0x00100000UL)  /**< 20-IIR B2 sign*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B2_POS                                                          (14UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B2_MASK                                                         (0x000FC000UL)  /**< 14-IIR B2 coef (unsigned, MSB unused) Value is coded value/32*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B1_POS                                                          (13UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B1_MASK                                                         (0x00002000UL)  /**< 13-IIR B1 sign*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B1_POS                                                          (7UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B1_MASK                                                         (0x00001F80UL)  /**< 7-IIR B1 coef (unsigned, MSB unused)  Value is coded value/32*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B0_POS                                                          (6UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_SIGN_B0_MASK                                                         (0x00000040UL)  /**< 6-IIR B0 sign*/
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B0_POS                                                          (0UL)
#define CLIF_SIGPRO_IIR_CONFIG1_IIR_COEF_B0_MASK                                                         (0x0000003FUL)  /**< 0-IIR B0 coef (unsigned, MSB unused)  Value is coded value/32*/

#define CLIF_SIGPRO_IIR_CONFIG0                                                                           0x2A
#define CLIF_SIGPRO_IIR_CONFIG0_RESERVED_POS                                                             (20UL)
#define CLIF_SIGPRO_IIR_CONFIG0_RESERVED_MASK                                                            (0xFFF00000UL)  /**< 20-Reserved */
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_SIGN_A2_POS                                                          (19UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_SIGN_A2_MASK                                                         (0x00080000UL)  /**< 19-IIR A1 sign*/
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_COEF_A2_POS                                                          (12UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_COEF_A2_MASK                                                         (0x0007F000UL)  /**< 12-IIR A1 coef (unsigned, MSB unused)  Value is coded value/64*/
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_SIGN_A1_POS                                                          (11UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_SIGN_A1_MASK                                                         (0x00000800UL)  /**< 11-IIR A0 sign*/
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_COEF_A1_POS                                                          (4UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_COEF_A1_MASK                                                         (0x000007F0UL)  /**< 4-IIR A0 coef (unsigned, MSB unused)  Value is coded value/64*/
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_GAIN_POS                                                             (1UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_GAIN_MASK                                                            (0x0000000EUL)  /**< 1-IIR filter gain*/
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_ENABLE_POS                                                           (0UL)
#define CLIF_SIGPRO_IIR_CONFIG0_IIR_ENABLE_MASK                                                          (0x00000001UL)  /**< 0-Enable the IIR filter*/

#define CLIF_DGRM_DAC_FILTER                                                                              0x2B
#define CLIF_DGRM_DAC_FILTER_RESERVED_POS                                                                (24UL)
#define CLIF_DGRM_DAC_FILTER_RESERVED_MASK                                                               (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_AMP_TH_POS                                                        (22UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_AMP_TH_MASK                                                       (0x00C00000UL)  /**< 22-defines the clipping level*/
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_DET_TH_POS                                                        (15UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_DET_TH_MASK                                                       (0x003F8000UL)  /**< 15-defines the number of clipping signal required to generate clip_det_o*/
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_WIN_SIZE_POS                                                      (14UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_CLIP_WIN_SIZE_MASK                                                     (0x00004000UL)  /**< 14-defines the monitoring window length*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DOUBLE_MEAN_LGTH_WINDOW_POS                                            (13UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DOUBLE_MEAN_LGTH_WINDOW_MASK                                           (0x00002000UL)  /**< 13-Doubles the window length used to calculate the mean value. This must never be set for Icode 6kBd.*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_OFF_POS                                                 (12UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_OFF_MASK                                                (0x00001000UL)  /**< 12-disable the dc step cancelation mechanism. To be set if IIR filter is used*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_RATE_POS                                                (11UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_RATE_MASK                                               (0x00000800UL)  /**< 11-0: DC cancel step is reduced by 1 each clock cycle; 1: DC cancel step is reduced by 2 each clock cycle*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_MODE_POS                                                (10UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DC_STEP_CANCEL_MODE_MASK                                               (0x00000400UL)  /**< 10-0: DC cancel step is initialized to step in ADC data; 1: DC cancel step is initialized to half of step in ADC data*/
#define CLIF_DGRM_DAC_FILTER_DGRM_HF_ATT_SLOW_ADJ_SEL_POS                                                (8UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_HF_ATT_SLOW_ADJ_SEL_MASK                                               (0x00000300UL)  /**< 8-Defines HF-ATT index increment during slow mode*/
#define CLIF_DGRM_DAC_FILTER_DGRM_HT_ATT_QUICK_ADJ_ENABLE_POS                                            (7UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_HT_ATT_QUICK_ADJ_ENABLE_MASK                                           (0x00000080UL)  /**< 7-Enables setting of HF-attenuator to its max. value when RSSI is above a maximum threshold*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DCO_TRACK_MAX_ITER_POS                                                 (4UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DCO_TRACK_MAX_ITER_MASK                                                (0x00000070UL)  /**< 4-Defines the maximum number of iterations in the SET_DCO_DAC state in TRACK MODE*/
#define CLIF_DGRM_DAC_FILTER_DGRM_DCO_TRACK_MAX_ITER_SEL_POS                                             (3UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_DCO_TRACK_MAX_ITER_SEL_MASK                                            (0x00000008UL)  /**< 3-When high, DGRM_DCO_TRACK_MAX_ITER gives the maximum number of iterations in the SET_DCO_DAC state in TRACK mode. When low, DGRM_DCO_MAX_ITER_SEL is used*/
#define CLIF_DGRM_DAC_FILTER_DGRM_FINE_DAC_FILTER_K2_POS                                                 (2UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_FINE_DAC_FILTER_K2_MASK                                                (0x00000004UL)  /**< 2-Defines K2 of the frequency response of the filter for the Fine DAC control.*/
#define CLIF_DGRM_DAC_FILTER_DGRM_FINE_DAC_FILTER_K1_POS                                                 (0UL)
#define CLIF_DGRM_DAC_FILTER_DGRM_FINE_DAC_FILTER_K1_MASK                                                (0x00000003UL)  /**< 0-Defines K1 of the frequency response of the filter for the Fine DAC control. 0: K1=2; 1: K1=4; 2: K1=8; 3: Reserved.*/

#define CLIF_DGRM_CONFIG                                                                                  0x2C
#define CLIF_DGRM_CONFIG_DGRM_STATISTICS_READ_ENABLE_POS                                                 (31UL)
#define CLIF_DGRM_CONFIG_DGRM_STATISTICS_READ_ENABLE_MASK                                                (0x80000000UL)  /**< 31-When set, HW updates DGRM_MEAN_I, DGRM_SWING_I, DGRM_MEAN_Q and DGRM_SWING_Q.*/
#define CLIF_DGRM_CONFIG_DGRM_MEASURE_ENABLE_POS                                                         (30UL)
#define CLIF_DGRM_CONFIG_DGRM_MEASURE_ENABLE_MASK                                                        (0x40000000UL)  /**< 30-Enables measurement of statistics (swing, mean) of both channels*/
#define CLIF_DGRM_CONFIG_DGRM_GAIN_SHIFT_OVR_POS                                                         (29UL)
#define CLIF_DGRM_CONFIG_DGRM_GAIN_SHIFT_OVR_MASK                                                        (0x20000000UL)  /**< 29-Enables the override of digital gain shift. Override value is set based on DGRM_GAIN_SHIFT_OVR_VAL*/
#define CLIF_DGRM_CONFIG_DGRM_GAIN_SHIFT_OVR_VAL_POS                                                     (26UL)
#define CLIF_DGRM_CONFIG_DGRM_GAIN_SHIFT_OVR_VAL_MASK                                                    (0x1C000000UL)  /**< 26-Defines the override value for digital gain shift when DGRM_GAIN_SHIFT_OVR is set*/
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_OVR_POS                                                           (25UL)
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_OVR_MASK                                                          (0x02000000UL)  /**< 25-Enables override of DC step cancellation signal. Override value is set based on DGRM_DCO_STEP_OVR_VAL*/
#define CLIF_DGRM_CONFIG_DGRM_TRACK_AFTER_FRAME_ENABLE_POS                                               (24UL)
#define CLIF_DGRM_CONFIG_DGRM_TRACK_AFTER_FRAME_ENABLE_MASK                                              (0x01000000UL)  /**< 24-Sets the HF-attenuator after frame*/
#define CLIF_DGRM_CONFIG_DGRM_TRACK_DURING_FRAME_ENABLE_POS                                              (23UL)
#define CLIF_DGRM_CONFIG_DGRM_TRACK_DURING_FRAME_ENABLE_MASK                                             (0x00800000UL)  /**< 23-Enables the BBA gain and DCO tracking during frame*/
#define CLIF_DGRM_CONFIG_DGRM_TRACK_BEFORE_FRAME_ENABLE_POS                                              (22UL)
#define CLIF_DGRM_CONFIG_DGRM_TRACK_BEFORE_FRAME_ENABLE_MASK                                             (0x00400000UL)  /**< 22-Enables the HF-attenuator and DCO tracking before frame*/
#define CLIF_DGRM_CONFIG_DGRM_SNR_LIM_DB_POS                                                             (18UL)
#define CLIF_DGRM_CONFIG_DGRM_SNR_LIM_DB_MASK                                                            (0x003C0000UL)  /**< 18-Scales noise power estimate*/
#define CLIF_DGRM_CONFIG_DGRM_SNR_LIM_INF_POS                                                            (15UL)
#define CLIF_DGRM_CONFIG_DGRM_SNR_LIM_INF_MASK                                                           (0x00038000UL)  /**< 15-Used to set signal threshold when noise power estimate is zero*/
#define CLIF_DGRM_CONFIG_DGRM_DCO_TRACK_TH_SEL_POS                                                       (13UL)
#define CLIF_DGRM_CONFIG_DGRM_DCO_TRACK_TH_SEL_MASK                                                      (0x00006000UL)  /**< 13-Defines the DCO threshold for tracking during card response*/
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_DELAY_POS                                                         (10UL)
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_DELAY_MASK                                                        (0x00001C00UL)  /**< 10-Defines the delay for the DC step cancellation*/
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_OVR_VAL_POS                                                       (0UL)
#define CLIF_DGRM_CONFIG_DGRM_DCO_STEP_OVR_VAL_MASK                                                      (0x000003FFUL)  /**< 0-Defines the override value for the digital DC step when DGRM_DCO_STEP_OVR is set*/

#define CLIF_DGRM_BBA                                                                                     0x2D
#define CLIF_DGRM_BBA_DGRM_FALSE_ALARM_WAIT_POS                                                          (30UL)
#define CLIF_DGRM_BBA_DGRM_FALSE_ALARM_WAIT_MASK                                                         (0xC0000000UL)  /**< 30-False alarm wait in multiples of 256 cycles */
#define CLIF_DGRM_BBA_DGRM_BBA_FAST_MODE_ENABLE_POS                                                      (29UL)
#define CLIF_DGRM_BBA_DGRM_BBA_FAST_MODE_ENABLE_MASK                                                     (0x20000000UL)  /**< 29-Enables the BBA fast mode*/
#define CLIF_DGRM_BBA_DGRM_GAIN_SHIFT_DELAY_POS                                                          (26UL)
#define CLIF_DGRM_BBA_DGRM_GAIN_SHIFT_DELAY_MASK                                                         (0x1C000000UL)  /**< 26-Defines the delay for digital gain compensation to match the latency from BBA gain*/
#define CLIF_DGRM_BBA_DGRM_BBA_TH_SEL_POS                                                                (24UL)
#define CLIF_DGRM_BBA_DGRM_BBA_TH_SEL_MASK                                                               (0x03000000UL)  /**< 24-Defines the threshold for the max absolute ADC value */
#define CLIF_DGRM_BBA_DGRM_MAX_SWING_TH_SEL_POS                                                          (22UL)
#define CLIF_DGRM_BBA_DGRM_MAX_SWING_TH_SEL_MASK                                                         (0x00C00000UL)  /**< 22-Defines the maximum swing threshold for decreasing BBA gain*/
#define CLIF_DGRM_BBA_DGRM_MIN_SWING_TH_SEL_POS                                                          (20UL)
#define CLIF_DGRM_BBA_DGRM_MIN_SWING_TH_SEL_MASK                                                         (0x00300000UL)  /**< 20-Defines the minimum swing threshold for increasing BBA gain*/
#define CLIF_DGRM_BBA_DGRM_WATER_LEVEL_TH_SEL_POS                                                        (18UL)
#define CLIF_DGRM_BBA_DGRM_WATER_LEVEL_TH_SEL_MASK                                                       (0x000C0000UL)  /**< 18-Defines the water level threshold*/
#define CLIF_DGRM_BBA_RESERVED_POS                                                                       (17UL)
#define CLIF_DGRM_BBA_RESERVED_MASK                                                                      (0x00020000UL)  /**< 17-RESERVED*/
#define CLIF_DGRM_BBA_DGRM_BBA_MIN_VAL_POS                                                               (14UL)
#define CLIF_DGRM_BBA_DGRM_BBA_MIN_VAL_MASK                                                              (0x0001C000UL)  /**< 14-Defines the minimum value of BBA gain. */
#define CLIF_DGRM_BBA_DGRM_BBA_MAX_VAL_POS                                                               (11UL)
#define CLIF_DGRM_BBA_DGRM_BBA_MAX_VAL_MASK                                                              (0x00003800UL)  /**< 11-Defines the maximum value of BBA gain.*/
#define CLIF_DGRM_BBA_DGRM_BBA_INIT_VAL_POS                                                              (8UL)
#define CLIF_DGRM_BBA_DGRM_BBA_INIT_VAL_MASK                                                             (0x00000700UL)  /**< 8-Defines initial value of BBA gain. If BBA fast and slow modes are disabled, this value defines the forced value.*/
#define CLIF_DGRM_BBA_DGRM_GUESS_BBA_GAIN_ENABLE_POS                                                     (7UL)
#define CLIF_DGRM_BBA_DGRM_GUESS_BBA_GAIN_ENABLE_MASK                                                    (0x00000080UL)  /**< 7-Enables the initial guess of BBA gain based on noise power estimate*/
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_EST_READ_ENABLE_POS                                               (6UL)
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_EST_READ_ENABLE_MASK                                              (0x00000040UL)  /**< 6-When set, HW updates DGRM_SIGNAL_DETECT_TH, DGRM_NOISE_POWER_EST_Q and DGRM_NOISE_POWER_EST_I.*/
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_EST_ENABLE_POS                                                    (5UL)
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_EST_ENABLE_MASK                                                   (0x00000020UL)  /**< 5-Enables the noise power estimator in free running mode*/
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_SHIFT_VAL_POS                                                     (2UL)
#define CLIF_DGRM_BBA_DGRM_NOISE_POWER_SHIFT_VAL_MASK                                                    (0x0000001CUL)  /**< 2-Defines the number of bits to shift right in order to scale the noise power. This is in addition to the scaling due to accumulation*/
#define CLIF_DGRM_BBA_RESERVED1_POS                                                                       (0UL)
#define CLIF_DGRM_BBA_RESERVED1_MASK                                                                      (0x00000003UL)  /**< 0-Reserved */

#define CLIF_DGRM_DCO                                                                                     0x2E
#define CLIF_DGRM_DCO_DGRM_DCO_TRACK_AVG_LEN_SEL_POS                                                     (30UL)
#define CLIF_DGRM_DCO_DGRM_DCO_TRACK_AVG_LEN_SEL_MASK                                                    (0xC0000000UL)  /**< 30-Defines the number of ADC samples to average during TRACK mode*/
#define CLIF_DGRM_DCO_DGRM_DCO_INIT_AVG_LEN_SEL_POS                                                      (28UL)
#define CLIF_DGRM_DCO_DGRM_DCO_INIT_AVG_LEN_SEL_MASK                                                     (0x30000000UL)  /**< 28-Defines the number of ADC samples to average during INIT mode*/
#define CLIF_DGRM_DCO_DGRM_DCO_WAIT_PERIOD_SEL_POS                                                       (26UL)
#define CLIF_DGRM_DCO_DGRM_DCO_WAIT_PERIOD_SEL_MASK                                                      (0x0C000000UL)  /**< 26-Defines the wait period after DCO DAC update and before estimating DCO*/
#define CLIF_DGRM_DCO_DGRM_DCO_TH_SEL_POS                                                                (24UL)
#define CLIF_DGRM_DCO_DGRM_DCO_TH_SEL_MASK                                                               (0x03000000UL)  /**< 24-Defines the DCO DAC threshold*/
#define CLIF_DGRM_DCO_DGRM_DCO_MAX_ITER_SEL_POS                                                          (22UL)
#define CLIF_DGRM_DCO_DGRM_DCO_MAX_ITER_SEL_MASK                                                         (0x00C00000UL)  /**< 22-Defines the maximum number of iterations in DCO DAC fast mode*/
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_SLOW_MODE_ENABLE_POS                                                  (21UL)
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_SLOW_MODE_ENABLE_MASK                                                 (0x00200000UL)  /**< 21-Enables DCO DAC slow mode before card response.*/
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_FAST_MODE_ENABLE_POS                                                  (20UL)
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_FAST_MODE_ENABLE_MASK                                                 (0x00100000UL)  /**< 20-Enables DCO DAC fast mode*/
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_Q_INIT_VAL_POS                                                        (10UL)
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_Q_INIT_VAL_MASK                                                       (0x000FFC00UL)  /**< 10-Defines the initial value of the composite DAC for the Q channel. This is also the forced value if DCO DAC fast and slow modes are disabled. The 5 MSBs control the coarse DAC and the 5 MSBs control the fine DAC.*/
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_I_INIT_VAL_POS                                                        (0UL)
#define CLIF_DGRM_DCO_DGRM_DCO_DAC_I_INIT_VAL_MASK                                                       (0x000003FFUL)  /**< 0-Defines the initial value of the composite DAC for the I channel. This is also the forced value if DCO DAC fast and slow modes are disabled. The 5 MSBs control the coarse DAC and the 5 MSBs control the fine DAC.*/

#define CLIF_DGRM_HF_ATT                                                                                  0x2F
#define CLIF_DGRM_HF_ATT_DGRM_MINMAX_AVG_COUNT_FORCE_POS                                                 (31UL)
#define CLIF_DGRM_HF_ATT_DGRM_MINMAX_AVG_COUNT_FORCE_MASK                                                (0x80000000UL)  /**< 31-Forces number of min or max samples to be averaged to 16. */
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_FREEZE_ENABLE_POS                                                   (30UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_FREEZE_ENABLE_MASK                                                  (0x40000000UL)  /**< 30-Enable HF attenuator freeze based on tx_active, miller bit-grid and signal_detection.*/
#define CLIF_DGRM_HF_ATT_DGRM_MILLER_BG_HIGH_FREEZE_TH_POS                                               (23UL)
#define CLIF_DGRM_HF_ATT_DGRM_MILLER_BG_HIGH_FREEZE_TH_MASK                                              (0x3F800000UL)  /**< 23-Defines the upper threshold of Miller bit-grid value to freeze HF-attenuator.*/
#define CLIF_DGRM_HF_ATT_DGRM_MILLER_BG_LOW_FREEZE_TH_POS                                                (16UL)
#define CLIF_DGRM_HF_ATT_DGRM_MILLER_BG_LOW_FREEZE_TH_MASK                                               (0x007F0000UL)  /**< 16-Defines the lower threshold of Miller bit-grid value to freeze HF-attenuator.*/
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_MAX_ITER_SEL_POS                                                    (14UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_MAX_ITER_SEL_MASK                                                   (0x0000C000UL)  /**< 14-Defines the maximum number of iterations in HF-attenuator fast mode*/
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_MAX_VAL_POS                                                         (8UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_MAX_VAL_MASK                                                        (0x00003F00UL)  /**< 8-Maximum value of HF-attenuator. */
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_INIT_VAL_POS                                                        (2UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_INIT_VAL_MASK                                                       (0x000000FCUL)  /**< 2-Initial value of HF-attenuator. This value will also be used as the forced value when fast mode is disabled.*/
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_SLOW_MODE_ENABLE_POS                                                (1UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_SLOW_MODE_ENABLE_MASK                                               (0x00000002UL)  /**< 1-Enables HF-attenuator slow regulation mode*/
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_FAST_MODE_ENABLE_POS                                                (0UL)
#define CLIF_DGRM_HF_ATT_DGRM_HF_ATT_FAST_MODE_ENABLE_MASK                                               (0x00000001UL)  /**< 0-Enables HF-attenuator fast settling mode*/

#define CLIF_DGRM_RSSI                                                                                    0x30
#define CLIF_DGRM_RSSI_DGRM_DEMOD_EN_FORCE_POS                                                           (31UL)
#define CLIF_DGRM_RSSI_DGRM_DEMOD_EN_FORCE_MASK                                                          (0x80000000UL)  /**< 31-When set, forces demod_enable high*/
#define CLIF_DGRM_RSSI_DGRM_NUM_GAIN_UPDT_FORCE_POS                                                      (30UL)
#define CLIF_DGRM_RSSI_DGRM_NUM_GAIN_UPDT_FORCE_MASK                                                     (0x40000000UL)  /**< 30-When set, forces dgrm_num_gain_updt to be zero*/
#define CLIF_DGRM_RSSI_DGRM_SIGNAL_DETECT_TH_OVR_POS                                                     (29UL)
#define CLIF_DGRM_RSSI_DGRM_SIGNAL_DETECT_TH_OVR_MASK                                                    (0x20000000UL)  /**< 29-Enables the override of signal detect threshold. Override value is set based on DGRM_SIGNAL_DETECT_TH_OVR_VAL.*/
#define CLIF_DGRM_RSSI_DGRM_RSSI_TRACK_AVG_LEN_SEL_POS                                                   (27UL)
#define CLIF_DGRM_RSSI_DGRM_RSSI_TRACK_AVG_LEN_SEL_MASK                                                  (0x18000000UL)  /**< 27-Defines the number of RSSI samples to average during track mode*/
#define CLIF_DGRM_RSSI_DGRM_RSSI_INIT_AVG_LEN_SEL_POS                                                    (25UL)
#define CLIF_DGRM_RSSI_DGRM_RSSI_INIT_AVG_LEN_SEL_MASK                                                   (0x06000000UL)  /**< 25-Defines the number of RSSI samples to average during INIT mode*/
#define CLIF_DGRM_RSSI_DGRM_RSSI_WAIT_PERIOD_POS                                                         (23UL)
#define CLIF_DGRM_RSSI_DGRM_RSSI_WAIT_PERIOD_MASK                                                        (0x01800000UL)  /**< 23-Defines the number of RSSI samples to discard before averaging*/
#define CLIF_DGRM_RSSI_DGRM_RSSI_HYST_POS                                                                (17UL)
#define CLIF_DGRM_RSSI_DGRM_RSSI_HYST_MASK                                                               (0x007E0000UL)  /**< 17-Hysteresis value for RSSI target*/
#define CLIF_DGRM_RSSI_DGRM_RSSI_TARGET_POS                                                              (7UL)
#define CLIF_DGRM_RSSI_DGRM_RSSI_TARGET_MASK                                                             (0x0001FF80UL)  /**< 7-RSSI target value*/
#define CLIF_DGRM_RSSI_DGRM_SIGNAL_DETECT_TH_OVR_VAL_POS                                                 (0UL)
#define CLIF_DGRM_RSSI_DGRM_SIGNAL_DETECT_TH_OVR_VAL_MASK                                                (0x0000007FUL)  /**< 0-Defines the override value for signal detect threshold when DGRM_SIGNAL_DETECT_TH_OVR is set.*/

#define CLIF_CRC_RX_CONFIG                                                                                0x31
#define CLIF_CRC_RX_CONFIG_RX_CRC_PRESET_VALUE_POS                                                       (16UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_PRESET_VALUE_MASK                                                      (0xFFFF0000UL)  /**< 16-Arbitrary preset value for the Rx-Decoder CRC calculation. */
#define CLIF_CRC_RX_CONFIG_RESERVED_POS                                                                  (8UL)
#define CLIF_CRC_RX_CONFIG_RESERVED_MASK                                                                 (0x0000FF00UL)  /**< 8-Reserved */
#define CLIF_CRC_RX_CONFIG_RX_FORCE_CRC_WRITE_POS                                                        (7UL)
#define CLIF_CRC_RX_CONFIG_RX_FORCE_CRC_WRITE_MASK                                                       (0x00000080UL)  /**< 7-If set. the Rx-Decoder will send to the RAM the CRC bits as well.*/
#define CLIF_CRC_RX_CONFIG_RX_CRC_ALLOW_BITS_POS                                                         (6UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_ALLOW_BITS_MASK                                                        (0x00000040UL)  /**< 6-If activated the frame with length =< CRC_length will be always sent to the System RAM as is, without CRC bits removal.*/
#define CLIF_CRC_RX_CONFIG_RX_CRC_PRESET_SEL_POS                                                         (3UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_PRESET_SEL_MASK                                                        (0x00000038UL)  /**< 3-Preset value of the CRC register for the Rx-Decoder. For a CRC calculation using 5bits only the LSByte is used. 000b* 0000h reset value Note that this configuration is set by the Mode detector for FeliCa. 001b  6363h Note that this configuration is set by the Mode detector for ISO14443 type A. 010b  A671h 011b FFFFh Note that this configuration is set by the Mode detector for ISO14443 type B. 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value RX_CRC_PRESET_VALUE*/
#define CLIF_CRC_RX_CONFIG_RX_CRC_TYPE_POS                                                               (2UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_TYPE_MASK                                                              (0x00000004UL)  /**< 2-Controls the type of CRC calulation for the Rx-Decoder 0* 16bit CRC calculation. reset value 1 5bit CRC calculation */
#define CLIF_CRC_RX_CONFIG_RX_CRC_INV_POS                                                                (1UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_INV_MASK                                                               (0x00000002UL)  /**< 1-Controls the comparison of the CRC checksum for the Rx-Decoder 0* Not inverted CRC value: 0000h reset value Note that this nit is cleared by the Mode detector for ISO14443 type A and FeliCa. 1 Inverted CRC value: F0B8h Note that this bit is set by the Mode detector for ISO14443 type B.*/
#define CLIF_CRC_RX_CONFIG_RX_CRC_ENABLE_POS                                                             (0UL)
#define CLIF_CRC_RX_CONFIG_RX_CRC_ENABLE_MASK                                                            (0x00000001UL)  /**< 0-If set. the Rx-Decoder will check the CRC for correctness. Note that this bit is set by the Mode Detector. when ISO14443 type B. or FeliCa (212 or 424kBd) is detected.*/

#define CLIF_RX_WAIT                                                                                      0x32
#define CLIF_RX_WAIT_RESERVED_POS                                                                        (28UL)
#define CLIF_RX_WAIT_RESERVED_MASK                                                                       (0xF0000000UL)  /**< 28-Reserved */
#define CLIF_RX_WAIT_RX_WAIT_VALUE_POS                                                                   (8UL)
#define CLIF_RX_WAIT_RX_WAIT_VALUE_MASK                                                                  (0x0FFFFF00UL)  /**< 8-Defines the rx_wait timer reload value. Note: If set to 00000h the rx_wait guard time is disabled */
#define CLIF_RX_WAIT_RX_WAIT_PRESCALER_POS                                                               (0UL)
#define CLIF_RX_WAIT_RX_WAIT_PRESCALER_MASK                                                              (0x000000FFUL)  /**< 0-Defines the prescaler reload value for the rx_wait timer.*/

#define CLIF_DCOC_CONFIG                                                                                  0x33
#define CLIF_DCOC_CONFIG_RESERVED_POS                                                                    (22UL)
#define CLIF_DCOC_CONFIG_RESERVED_MASK                                                                   (0xFFC00000UL)  /**< 22-RESERVED*/
#define CLIF_DCOC_CONFIG_DCOC_CAL_DONE_FORCE_POS                                                         (21UL)
#define CLIF_DCOC_CONFIG_DCOC_CAL_DONE_FORCE_MASK                                                        (0x00200000UL)  /**< 21-forces the signal dcoc_cal_done to 1*/
#define CLIF_DCOC_CONFIG_DCOC_AVERAGE_EN_POS                                                             (20UL)
#define CLIF_DCOC_CONFIG_DCOC_AVERAGE_EN_MASK                                                            (0x00100000UL)  /**< 20-enables the averaging of the adc_data in the dcoc module*/
#define CLIF_DCOC_CONFIG_DCOC_ADD_OFFSET_Q_POS                                                           (14UL)
#define CLIF_DCOC_CONFIG_DCOC_ADD_OFFSET_Q_MASK                                                          (0x000FC000UL)  /**< 14-additional offset added on Q channel (signed)*/
#define CLIF_DCOC_CONFIG_DCOC_ADD_OFFSET_I_POS                                                           (8UL)
#define CLIF_DCOC_CONFIG_DCOC_ADD_OFFSET_I_MASK                                                          (0x00003F00UL)  /**< 8-additional offset added on I channel (signed)*/
#define CLIF_DCOC_CONFIG_DCOC_TIME_DELAY_POS                                                             (4UL)
#define CLIF_DCOC_CONFIG_DCOC_TIME_DELAY_MASK                                                            (0x000000F0UL)  /**< 4-Number of clock cycles given to the analog to settle after a dco calibration process*/
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_RX_WAIT_POS                                                      (3UL)
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_RX_WAIT_MASK                                                     (0x00000008UL)  /**< 3-signal to start the calibration when the transceiver enters WAIT_RECEIVE state.*/
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_WAIT_DATA_POS                                                    (2UL)
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_WAIT_DATA_MASK                                                   (0x00000004UL)  /**< 2-signal to start the calibration when the transceiver enters WAIT_FOR_DATA state*/
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_NOW_POS                                                          (1UL)
#define CLIF_DCOC_CONFIG_DCOC_START_CAL_NOW_MASK                                                         (0x00000002UL)  /**< 1-signal to start the calibration, can be cleared by HW.*/
#define CLIF_DCOC_CONFIG_DCOC_FSM_BYP_POS                                                                (0UL)
#define CLIF_DCOC_CONFIG_DCOC_FSM_BYP_MASK                                                               (0x00000001UL)  /**< 0-Bypass the DCO FSM*/

#define CLIF_RXM_NFCLD_LVL                                                                                0x34
#define CLIF_RXM_NFCLD_LVL_RESERVED_POS                                                                  (30UL)
#define CLIF_RXM_NFCLD_LVL_RESERVED_MASK                                                                 (0xC0000000UL)  /**< 30-reserved*/
#define CLIF_RXM_NFCLD_LVL_RXM_NFCLD_OFF_TH_POS                                                          (16UL)
#define CLIF_RXM_NFCLD_LVL_RXM_NFCLD_OFF_TH_MASK                                                         (0x3FFF0000UL)  /**< 16-RSSI OFF threshold*/
#define CLIF_RXM_NFCLD_LVL_RESERVED1_POS                                                                  (14UL)
#define CLIF_RXM_NFCLD_LVL_RESERVED1_MASK                                                                 (0x0000C000UL)  /**< 14-reserved*/
#define CLIF_RXM_NFCLD_LVL_RXM_NFCLD_ON_TH_POS                                                           (0UL)
#define CLIF_RXM_NFCLD_LVL_RXM_NFCLD_ON_TH_MASK                                                          (0x00003FFFUL)  /**< 0-RSSI ON threshold*/

#define CLIF_RXM_CTRL                                                                                     0x35
#define CLIF_RXM_CTRL_RESERVED_POS                                                                       (26UL)
#define CLIF_RXM_CTRL_RESERVED_MASK                                                                      (0xFC000000UL)  /**< 26-reserved*/
#define CLIF_RXM_CTRL_RXM_AVRG_REQ_POS                                                                   (25UL)
#define CLIF_RXM_CTRL_RXM_AVRG_REQ_MASK                                                                  (0x02000000UL)  /**< 25-1: run averaging of I and Q. Is reset by HW when averaging is complete. Resulting data are stored in RXM_AVRG register.*/
#define CLIF_RXM_CTRL_RXM_FORCE_NFCLDDET_POS                                                             (24UL)
#define CLIF_RXM_CTRL_RXM_FORCE_NFCLDDET_MASK                                                            (0x01000000UL)  /**< 24-Forces nfcld_det_o to RXM_FORCE_NFCLDDET_VALUE.*/
#define CLIF_RXM_CTRL_RXM_FORCE_NFCLDDET_VALUE_POS                                                       (23UL)
#define CLIF_RXM_CTRL_RXM_FORCE_NFCLDDET_VALUE_MASK                                                      (0x00800000UL)  /**< 23-Value to force nfcld_det_o to when RXM_FORCE_NFCLDDET=1.*/
#define CLIF_RXM_CTRL_RXM_HFATT_ENABLE_POS                                                               (22UL)
#define CLIF_RXM_CTRL_RXM_HFATT_ENABLE_MASK                                                              (0x00400000UL)  /**< 22-When 0, HF attenuation not considered for field detection When 1, consider field ON when HF attenuation is not 0*/
#define CLIF_RXM_CTRL_RXM_MAX_PHASE_DIFF_POS                                                             (17UL)
#define CLIF_RXM_CTRL_RXM_MAX_PHASE_DIFF_MASK                                                            (0x003E0000UL)  /**< 17-maximum difference between 2 consecutive phase (bit) mesured at 1.7MHz rate. In 13.56MHz/512 multiples.
when exeeded, frequency is concidered as not valid.*/
#define CLIF_RXM_CTRL_RXM_GAIN_MASK_TIME_POS                                                             (14UL)
#define CLIF_RXM_CTRL_RXM_GAIN_MASK_TIME_MASK                                                            (0x0001C000UL)  /**< 14-RSSI masking time after BBA gain or HFAtt changes  (in 8/13,56MHz multiples)*/
#define CLIF_RXM_CTRL_RXM_RFOFF_MASK_TIME_POS                                                            (11UL)
#define CLIF_RXM_CTRL_RXM_RFOFF_MASK_TIME_MASK                                                           (0x00003800UL)  /**< 11-RSSI masking time after RF switched off (in 256/13,56MHz multiples)*/
#define CLIF_RXM_CTRL_RXM_RXOFF_MASK_TIME_POS                                                            (8UL)
#define CLIF_RXM_CTRL_RXM_RXOFF_MASK_TIME_MASK                                                           (0x00000700UL)  /**< 8-RSSI masking time after RX (in 256/13,56MHz multiples)*/
#define CLIF_RXM_CTRL_RXM_TXOFF_MASK_TIME_POS                                                            (5UL)
#define CLIF_RXM_CTRL_RXM_TXOFF_MASK_TIME_MASK                                                           (0x000000E0UL)  /**< 5-RSSI masking time after TX (in 256/13,56MHz multiples)*/
#define CLIF_RXM_CTRL_RXM_TX_MASK_ENABLE_POS                                                             (4UL)
#define CLIF_RXM_CTRL_RXM_TX_MASK_ENABLE_MASK                                                            (0x00000010UL)  /**< 4-enable frequency masking during transmission*/
#define CLIF_RXM_CTRL_RXM_RX_MASK_ENABLE_POS                                                             (3UL)
#define CLIF_RXM_CTRL_RXM_RX_MASK_ENABLE_MASK                                                            (0x00000008UL)  /**< 3-enable frequency masking during reception*/
#define CLIF_RXM_CTRL_RXM_FRQ_CHECK_PCRM_ENABLE_POS                                                      (2UL)
#define CLIF_RXM_CTRL_RXM_FRQ_CHECK_PCRM_ENABLE_MASK                                                     (0x00000004UL)  /**< 2-enable frequency check from pcrm*/
#define CLIF_RXM_CTRL_RXM_FRQ_CHECK_CORDIC_ENABLE_POS                                                    (1UL)
#define CLIF_RXM_CTRL_RXM_FRQ_CHECK_CORDIC_ENABLE_MASK                                                   (0x00000002UL)  /**< 1-enable precise frequency check from cordic phase (+/- 1.7MHz multiples)*/
#define CLIF_RXM_CTRL_RXM_ENABLE_POS                                                                     (0UL)
#define CLIF_RXM_CTRL_RXM_ENABLE_MASK                                                                    (0x00000001UL)  /**< 0-enable the all RxMeasure module. */

#define CLIF_ANA_AGC_DCO_CTRL                                                                             0x36
#define CLIF_ANA_AGC_DCO_CTRL_RESERVED_POS                                                               (26UL)
#define CLIF_ANA_AGC_DCO_CTRL_RESERVED_MASK                                                              (0xFC000000UL)  /**< 26-reserved*/
#define CLIF_ANA_AGC_DCO_CTRL_RX_DC_CAL_EN_POS                                                           (25UL)
#define CLIF_ANA_AGC_DCO_CTRL_RX_DC_CAL_EN_MASK                                                          (0x02000000UL)  /**< 25-Enable signal for RX DC calibration */
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_C_EN_POS                                                            (24UL)
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_C_EN_MASK                                                           (0x01000000UL)  /**< 24-Enable signal for the DCO coarse DAC*/
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_DAC_Q_POS                                                           (14UL)
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_DAC_Q_MASK                                                          (0x00FFC000UL)  /**< 14-DCO DAC input control*/
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_DAC_I_POS                                                           (4UL)
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_DAC_I_MASK                                                          (0x00003FF0UL)  /**< 4-DCO DAC input control*/
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_F_EN_POS                                                            (3UL)
#define CLIF_ANA_AGC_DCO_CTRL_RX_DCO_F_EN_MASK                                                           (0x00000008UL)  /**< 3-enable signal for the DCO fine DAC*/
#define CLIF_ANA_AGC_DCO_CTRL_RESERVED1_POS                                                               (0UL)
#define CLIF_ANA_AGC_DCO_CTRL_RESERVED1_MASK                                                              (0x00000007UL)  /**< 0-reserved*/

#define CLIF_SIGPRO_CM_CONFIG                                                                             0x37
#define CLIF_SIGPRO_CM_CONFIG_CM_LONG_BPRIME_ENABLE_POS                                                  (31UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_LONG_BPRIME_ENABLE_MASK                                                 (0x80000000UL)  /**< 31-0 (default) bprime sof window has max value. 1: no max values */
#define CLIF_SIGPRO_CM_CONFIG_CM_SINGLE_THLD_ENABLE_POS                                                  (30UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_SINGLE_THLD_ENABLE_MASK                                                 (0x40000000UL)  /**< 30-0 (default) to use 2 thresholds in the filters. 1 to only use 1 threshold */
#define CLIF_SIGPRO_CM_CONFIG_CM_INT_FILT_RESET_ENABLE_POS                                               (29UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_INT_FILT_RESET_ENABLE_MASK                                              (0x20000000UL)  /**< 29-0 (default) disables internal filter reset after error detection. 1 to enable it.*/
#define CLIF_SIGPRO_CM_CONFIG_CM_EGT_CHECK_POS                                                           (28UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_EGT_CHECK_MASK                                                          (0x10000000UL)  /**< 28-enable check and error generation for EGT in TypeB*/
#define CLIF_SIGPRO_CM_CONFIG_CM_GTM_ON_POS                                                              (27UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_GTM_ON_MASK                                                             (0x08000000UL)  /**< 27-enable general target mode detection*/
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_A_POS                                                        (26UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_A_MASK                                                       (0x04000000UL)  /**< 26-Enables TypeA detection in General Target Mode*/
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_B_POS                                                        (25UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_B_MASK                                                       (0x02000000UL)  /**< 25-Enables TypeB detection in General Target Mode*/
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_F2_POS                                                       (24UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_F2_MASK                                                      (0x01000000UL)  /**< 24-Enables Felica212 detection in General Target Mode*/
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_F4_POS                                                       (23UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_ENABLE_GTM_F4_MASK                                                      (0x00800000UL)  /**< 23-Enables Felica424 detection in General Target Mode*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_EXT_SYNC_POS                                                     (22UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_EXT_SYNC_MASK                                                    (0x00400000UL)  /**< 22-*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_EXT_SYNC_POS                                                     (21UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_EXT_SYNC_MASK                                                    (0x00200000UL)  /**< 21-change sync pattern from b24d to 0000b24d*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_WIN_LGTH_POS                                                     (19UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_WIN_LGTH_MASK                                                    (0x00180000UL)  /**< 19-Controls the length of the bitgrid window in the f212 decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_WIN_LGTH_POS                                                     (17UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_WIN_LGTH_MASK                                                    (0x00060000UL)  /**< 17-Controls the length of the bitgrid window in the f424 decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEA_GCHANGE_DLY_POS                                                   (15UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEA_GCHANGE_DLY_MASK                                                  (0x00018000UL)  /**< 15-Programs the delay to authorize gain change during reception in the typeA decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEB_GCHANGE_DLY_POS                                                   (13UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEB_GCHANGE_DLY_MASK                                                  (0x00006000UL)  /**< 13-Programs the delay to authorize gain change during reception in the typeB decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_GCHANGE_DLY_POS                                                  (11UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF2_GCHANGE_DLY_MASK                                                 (0x00001800UL)  /**< 11-Programs the delay to authorize gain change during reception in the f212 decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_GCHANGE_DLY_POS                                                  (9UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_TYPEF4_GCHANGE_DLY_MASK                                                 (0x00000600UL)  /**< 9-Programs the delay to authorize gain change during reception in the f2424 decoder*/
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_THLD_POS                                                            (5UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_THLD_MASK                                                           (0x000001E0UL)  /**< 5-frame delay time fix threshold value*/
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_AUTO_INIT_THLD_POS                                                  (1UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_AUTO_INIT_THLD_MASK                                                 (0x0000001EUL)  /**< 1-frame delay time initial threshold value for automatic mode*/
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_AUTO_ENABLE_POS                                                     (0UL)
#define CLIF_SIGPRO_CM_CONFIG_CM_FDT_AUTO_ENABLE_MASK                                                    (0x00000001UL)  /**< 0-enable automatic threshold mode for frame delay time*/

#define CLIF_GCM_CONFIG2                                                                                  0x38
#define CLIF_GCM_CONFIG2_RESERVED_POS                                                                    (24UL)
#define CLIF_GCM_CONFIG2_RESERVED_MASK                                                                   (0xFF000000UL)  /**< 24-Reserved */
#define CLIF_GCM_CONFIG2_GCM_TX_RELEASE_DLY_POS                                                          (21UL)
#define CLIF_GCM_CONFIG2_GCM_TX_RELEASE_DLY_MASK                                                         (0x00E00000UL)  /**< 21-Extends the tx_active signal received at the end of a transmission with DECIMAL(GCM_TX_RELEASE_DLY)*16 RF cycles.*/
#define CLIF_GCM_CONFIG2_GCM_TX_ENV_DLY_POS                                                              (18UL)
#define CLIF_GCM_CONFIG2_GCM_TX_ENV_DLY_MASK                                                             (0x001C0000UL)  /**< 18-*/
#define CLIF_GCM_CONFIG2_GCM_BBA_TX_VAL_POS                                                              (15UL)
#define CLIF_GCM_CONFIG2_GCM_BBA_TX_VAL_MASK                                                             (0x00038000UL)  /**< 15-BBA value during transmit. Only values in the range [0:4] are allowed. Values in the range [5:7] are RFU.*/
#define CLIF_GCM_CONFIG2_GCM_BBA_INIT_EXT_VAL_POS                                                        (12UL)
#define CLIF_GCM_CONFIG2_GCM_BBA_INIT_EXT_VAL_MASK                                                       (0x00007000UL)  /**< 12-BBA start-up value on external field. Only values in the range [0:4] are allowed. Values in the range [5:7] are RFU.*/
#define CLIF_GCM_CONFIG2_GCM_BBA_INIT_INT_VAL_POS                                                        (9UL)
#define CLIF_GCM_CONFIG2_GCM_BBA_INIT_INT_VAL_MASK                                                       (0x00000E00UL)  /**< 9-BBA start-up value on internal field. Only values in the range [0:4] are allowed. Values in the range [5:7] are RFU.*/
#define CLIF_GCM_CONFIG2_GCM_BBA_MIN_VAL_POS                                                             (6UL)
#define CLIF_GCM_CONFIG2_GCM_BBA_MIN_VAL_MASK                                                            (0x000001C0UL)  /**< 6-BBA minimal value. Only values in the range [0:4] are allowed. Values in the range [5:7] are RFU.*/
#define CLIF_GCM_CONFIG2_GCM_BBA_MAX_VAL_POS                                                             (3UL)
#define CLIF_GCM_CONFIG2_GCM_BBA_MAX_VAL_MASK                                                            (0x00000038UL)  /**< 3-BBA maximal value. Only values in the range [0:4] are allowed. Values in the range [5:7] are RFU.*/
#define CLIF_GCM_CONFIG2_Reserved_POS                                                                    (0UL)
#define CLIF_GCM_CONFIG2_Reserved_MASK                                                                   (0x00000007UL)  /**< 0-0*/

#define CLIF_GCM_CONFIG1                                                                                  0x39
#define CLIF_GCM_CONFIG1_RESERVED_POS                                                                    (31UL)
#define CLIF_GCM_CONFIG1_RESERVED_MASK                                                                   (0x80000000UL)  /**< 31-Reserved */
#define CLIF_GCM_CONFIG1_GCM_RF_OFF_ENABLE_POS                                                           (30UL)
#define CLIF_GCM_CONFIG1_GCM_RF_OFF_ENABLE_MASK                                                          (0x40000000UL)  /**< 30-allows regulation in IDLE state*/
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_TX_VAL_POS                                                           (24UL)
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_TX_VAL_MASK                                                          (0x3F000000UL)  /**< 24-HF_ATT value on Transmission*/
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_INIT_EXT_VAL_POS                                                     (18UL)
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_INIT_EXT_VAL_MASK                                                    (0x00FC0000UL)  /**< 18-HF_ATT startup value*/
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_INIT_INT_VAL_POS                                                     (12UL)
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_INIT_INT_VAL_MASK                                                    (0x0003F000UL)  /**< 12-HF_ATT startup value*/
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_MIN_VAL_POS                                                          (6UL)
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_MIN_VAL_MASK                                                         (0x00000FC0UL)  /**< 6-HF_ATT minimal value*/
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_MAX_VAL_POS                                                          (0UL)
#define CLIF_GCM_CONFIG1_GCM_HF_ATT_MAX_VAL_MASK                                                         (0x0000003FUL)  /**< 0-HF_ATT maximal value*/

#define CLIF_GCM_CONFIG0                                                                                  0x3A
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_WAIT_27_DLY_POS                                                   (31UL)
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_WAIT_27_DLY_MASK                                                  (0x80000000UL)  /**< 31-Adds a clk27 delay before taking tx_env = 0 into account.*/
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_WAIT_POS                                                          (24UL)
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_WAIT_MASK                                                         (0x7F000000UL)  /**< 24-Number of clk13 cycles before taking tx_env = 0 into account during transmission*/
#define CLIF_GCM_CONFIG0_GCM_MAG_HIGH_THLD_POS                                                           (21UL)
#define CLIF_GCM_CONFIG0_GCM_MAG_HIGH_THLD_MASK                                                          (0x00E00000UL)  /**< 21-High threshold value. Threshold value is (MAG_HIGH_THLD+1)*64*/
#define CLIF_GCM_CONFIG0_GCM_MAG_LOW_THLD_POS                                                            (18UL)
#define CLIF_GCM_CONFIG0_GCM_MAG_LOW_THLD_MASK                                                           (0x001C0000UL)  /**< 18-Low threshold value. Threshold value is (MAG_LOW_THLD+1)*32*/
#define CLIF_GCM_CONFIG0_GCM_MAG_OFF_THLD_POS                                                            (15UL)
#define CLIF_GCM_CONFIG0_GCM_MAG_OFF_THLD_MASK                                                           (0x00038000UL)  /**< 15-Off threshold value. Threshold value is MAG_OFF_THLD*4. A value of 0 disables the off level detection.*/
#define CLIF_GCM_CONFIG0_GCM_MAG_OFF_FAST_POS                                                            (14UL)
#define CLIF_GCM_CONFIG0_GCM_MAG_OFF_FAST_MASK                                                           (0x00004000UL)  /**< 14-enables immediate reset of the gains upon field off detection*/
#define CLIF_GCM_CONFIG0_GCM_AVG_LENGTH_POS                                                              (12UL)
#define CLIF_GCM_CONFIG0_GCM_AVG_LENGTH_MASK                                                             (0x00003000UL)  /**< 12-length of the average window. length is 2^(AVG_LENGTH+2)*/
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_ENABLE_POS                                                        (11UL)
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_ENABLE_MASK                                                       (0x00000800UL)  /**< 11-enable/disable fast mode*/
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_RFON_INT_POS                                                      (10UL)
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_RFON_INT_MASK                                                     (0x00000400UL)  /**< 10-enable/disable forced fast mode at startup on RFON internal event*/
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_RFON_EXT_POS                                                      (9UL)
#define CLIF_GCM_CONFIG0_GCM_FAST_MODE_RFON_EXT_MASK                                                     (0x00000200UL)  /**< 9-enable/disable forced fast mode at startup on RFON external event*/
#define CLIF_GCM_CONFIG0_GCM_RX_CHANGE_ENABLE_POS                                                        (8UL)
#define CLIF_GCM_CONFIG0_GCM_RX_CHANGE_ENABLE_MASK                                                       (0x00000100UL)  /**< 8-enable gain change during reception*/
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_MODE_POS                                                          (6UL)
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_MODE_MASK                                                         (0x000000C0UL)  /**< 6-controls the switching value used during transmission envelop pattern*/
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_SWITCH_VAL_POS                                                    (5UL)
#define CLIF_GCM_CONFIG0_GCM_FREEZE_TX_SWITCH_VAL_MASK                                                   (0x00000020UL)  /**< 5-freeze gain during transmission*/
#define CLIF_GCM_CONFIG0_GCM_MAX_TXRFOFF_ENABLE_POS                                                      (4UL)
#define CLIF_GCM_CONFIG0_GCM_MAX_TXRFOFF_ENABLE_MASK                                                     (0x00000010UL)  /**< 4-force max gain upon rfoff_event_i reception*/
#define CLIF_GCM_CONFIG0_GCM_RESET_TXRFOFF_ENABLE_POS                                                    (3UL)
#define CLIF_GCM_CONFIG0_GCM_RESET_TXRFOFF_ENABLE_MASK                                                   (0x00000008UL)  /**< 3-reset gain upon rfoff_event_i reception*/
#define CLIF_GCM_CONFIG0_GCM_HF_ATT_FORCE_POS                                                            (2UL)
#define CLIF_GCM_CONFIG0_GCM_HF_ATT_FORCE_MASK                                                           (0x00000004UL)  /**< 2-Enable / disable forcing HF_ATT value with GCM_HF_ATT_INIT_EXT_VAL*/
#define CLIF_GCM_CONFIG0_GCM_BBA_FORCE_POS                                                               (1UL)
#define CLIF_GCM_CONFIG0_GCM_BBA_FORCE_MASK                                                              (0x00000002UL)  /**< 1-Enable / disable forcing BBA value with BBA_VAL*/
#define CLIF_GCM_CONFIG0_GCM_ENABLE_POS                                                                  (0UL)
#define CLIF_GCM_CONFIG0_GCM_ENABLE_MASK                                                                 (0x00000001UL)  /**< 0-module enable signal*/

#define CLIF_SS_TX1_CMCFG                                                                                 0x3B
#define CLIF_SS_TX1_CMCFG_RESERVED_POS                                                                   (22UL)
#define CLIF_SS_TX1_CMCFG_RESERVED_MASK                                                                  (0xFFC00000UL)  /**< 22-Reserved*/
#define CLIF_SS_TX1_CMCFG_TX1_CLK_MODE_MOD_CM_POS                                                        (19UL)
#define CLIF_SS_TX1_CMCFG_TX1_CLK_MODE_MOD_CM_MASK                                                       (0x00380000UL)  /**< 19-TX1 clock mode of modulated wave in CM*/
#define CLIF_SS_TX1_CMCFG_TX1_CLK_MODE_CW_CM_POS                                                         (16UL)
#define CLIF_SS_TX1_CMCFG_TX1_CLK_MODE_CW_CM_MASK                                                        (0x00070000UL)  /**< 16-TX1 clock mode of unmodulated wave in CM*/
#define CLIF_SS_TX1_CMCFG_TX1_AMP_MOD_CM_POS                                                             (8UL)
#define CLIF_SS_TX1_CMCFG_TX1_AMP_MOD_CM_MASK                                                            (0x0000FF00UL)  /**< 8-TX1 amplitude of modulated wave in CM*/
#define CLIF_SS_TX1_CMCFG_TX1_AMP_CW_CM_POS                                                              (0UL)
#define CLIF_SS_TX1_CMCFG_TX1_AMP_CW_CM_MASK                                                             (0x000000FFUL)  /**< 0-TX1 amplitude of unmodulated wave in CM*/

#define CLIF_SS_TX2_CMCFG                                                                                 0x3C
#define CLIF_SS_TX2_CMCFG_RESERVED_POS                                                                   (22UL)
#define CLIF_SS_TX2_CMCFG_RESERVED_MASK                                                                  (0xFFC00000UL)  /**< 22-Reserved*/
#define CLIF_SS_TX2_CMCFG_TX2_CLK_MODE_MOD_CM_POS                                                        (19UL)
#define CLIF_SS_TX2_CMCFG_TX2_CLK_MODE_MOD_CM_MASK                                                       (0x00380000UL)  /**< 19-TX2 clock mode of modulated wave in CM*/
#define CLIF_SS_TX2_CMCFG_TX2_CLK_MODE_CW_CM_POS                                                         (16UL)
#define CLIF_SS_TX2_CMCFG_TX2_CLK_MODE_CW_CM_MASK                                                        (0x00070000UL)  /**< 16-TX2 clock mode of unmodulated wave in CM*/
#define CLIF_SS_TX2_CMCFG_TX2_AMP_MOD_CM_POS                                                             (8UL)
#define CLIF_SS_TX2_CMCFG_TX2_AMP_MOD_CM_MASK                                                            (0x0000FF00UL)  /**< 8-TX2 amplitude of modulated wave in CM*/
#define CLIF_SS_TX2_CMCFG_TX2_AMP_CW_CM_POS                                                              (0UL)
#define CLIF_SS_TX2_CMCFG_TX2_AMP_CW_CM_MASK                                                             (0x000000FFUL)  /**< 0-TX2 amplitude of unmodulated wave in CM*/

#define CLIF_TIMER0_CONFIG                                                                                0x3D
#define CLIF_TIMER0_CONFIG_T0_START_NOW_POS                                                              (8UL)
#define CLIF_TIMER0_CONFIG_T0_START_NOW_MASK                                                             (0x00000100UL)  /**< 8-T0_START_EVENT: If set. the timer T0 is started immediatly. */
#define CLIF_TIMER0_CONFIG_RESERVED_POS                                                                  (7UL)
#define CLIF_TIMER0_CONFIG_RESERVED_MASK                                                                 (0x00000080UL)  /**< 7-Reserved */
#define CLIF_TIMER0_CONFIG_T0_ONE_SHOT_MODE_POS                                                          (6UL)
#define CLIF_TIMER0_CONFIG_T0_ONE_SHOT_MODE_MASK                                                         (0x00000040UL)  /**< 6-When set to 1 the counter value does not reload again until the counter value has reached zero*/
#define CLIF_TIMER0_CONFIG_T0_PRESCALE_SEL_POS                                                           (3UL)
#define CLIF_TIMER0_CONFIG_T0_PRESCALE_SEL_MASK                                                          (0x00000038UL)  /**< 3-Controls frequency/period of the timer T0 when the prescaler is activated in T0_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter   */
#define CLIF_TIMER0_CONFIG_T0_MODE_SEL_POS                                                               (2UL)
#define CLIF_TIMER0_CONFIG_T0_MODE_SEL_MASK                                                              (0x00000004UL)  /**< 2-Configuration of the timer T0 clock. 0b* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1b Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T0_PRESCALE_SEL).*/
#define CLIF_TIMER0_CONFIG_T0_RELOAD_ENABLE_POS                                                          (1UL)
#define CLIF_TIMER0_CONFIG_T0_RELOAD_ENABLE_MASK                                                         (0x00000002UL)  /**< 1-If set to 0.the timer T0 will stop on expiration. 0* After expiration the timer T0 will stop counting. i.e.. remain zero. reset value. 1 After expiration the timer T0 will reload its preset value and continue counting down. */
#define CLIF_TIMER0_CONFIG_T0_ENABLE_POS                                                                 (0UL)
#define CLIF_TIMER0_CONFIG_T0_ENABLE_MASK                                                                (0x00000001UL)  /**< 0-Enables the timer T0*/

#define CLIF_TIMER0_RELOAD                                                                                0x3E
#define CLIF_TIMER0_RELOAD_RESERVED_POS                                                                  (20UL)
#define CLIF_TIMER0_RELOAD_RESERVED_MASK                                                                 (0xFFF00000UL)  /**< 20-Reserved */
#define CLIF_TIMER0_RELOAD_T0_RELOAD_VALUE_POS                                                           (0UL)
#define CLIF_TIMER0_RELOAD_T0_RELOAD_VALUE_MASK                                                          (0x000FFFFFUL)  /**< 0-Reload value of the timer T0. */

#define CLIF_TIMER1_CONFIG                                                                                0x3F
#define CLIF_TIMER1_CONFIG_RESERVED_POS                                                                  (31UL)
#define CLIF_TIMER1_CONFIG_RESERVED_MASK                                                                 (0x80000000UL)  /**< 31-Reserved */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RX_STARTED_POS                                                     (30UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RX_STARTED_MASK                                                    (0x40000000UL)  /**< 30-T1_STOP_EVENT: If set. the timer T1 is stopped when a data reception begins (1st bit is received). */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_TX_STARTED_POS                                                     (29UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_TX_STARTED_MASK                                                    (0x20000000UL)  /**< 29-T1_STOP_EVENT: If set. the timer T1 is stopped when a data transmission begins. */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_ON_EXT_POS                                                      (28UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_ON_EXT_MASK                                                     (0x10000000UL)  /**< 28-T1_STOP_EVENT: If set. the timer T1 is stopped when the external RF field is detected. */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_OFF_EXT_POS                                                     (27UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_OFF_EXT_MASK                                                    (0x08000000UL)  /**< 27-T1_STOP_EVENT: If set. the timer T1 is stopped when the external RF field vanishes. */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_ON_INT_POS                                                      (26UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_ON_INT_MASK                                                     (0x04000000UL)  /**< 26-T1_STOP_EVENT: If set. the timer T1 is stopped when the internal RF field is turned on. */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_OFF_INT_POS                                                     (25UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RF_OFF_INT_MASK                                                    (0x02000000UL)  /**< 25-T1_STOP_EVENT: If set. the timer T1 is stopped when the internal RF field is turned off. */
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RX_ENDED_POS                                                       (24UL)
#define CLIF_TIMER1_CONFIG_T1_STOP_ON_RX_ENDED_MASK                                                      (0x01000000UL)  /**< 24-T1_STOP_EVENT: If set the timer T1 is stopped when an activity on RX is detected.*/
#define CLIF_TIMER1_CONFIG_RESERVED1_POS                                                                  (18UL)
#define CLIF_TIMER1_CONFIG_RESERVED1_MASK                                                                 (0x00FC0000UL)  /**< 18-Reserved */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RX_STARTED_POS                                                    (17UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RX_STARTED_MASK                                                   (0x00020000UL)  /**< 17-T1_START_EVENT: If set. the timer T1 is started when a data reception begins (1st bit is received). */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RX_ENDED_POS                                                      (16UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RX_ENDED_MASK                                                     (0x00010000UL)  /**< 16-T1_START_EVENT: If set. the timer T1 is started when a data reception ends. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_STARTED_POS                                                    (15UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_STARTED_MASK                                                   (0x00008000UL)  /**< 15-T1_START_EVENT: If set. the timer T1 is started when a data transmission begins.   */
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_ENDED_POS                                                      (14UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_ENDED_MASK                                                     (0x00004000UL)  /**< 14-T1_START_EVENT: If set. the timer T1 is started when a data transmission ends. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_ON_EXT_POS                                                     (13UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_ON_EXT_MASK                                                    (0x00002000UL)  /**< 13-T1_START_EVENT: If set. the timer T1 is startedwhen the external RF field is detected. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_OFF_EXT_POS                                                    (12UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_OFF_EXT_MASK                                                   (0x00001000UL)  /**< 12-T1_START_EVENT: If set. the timer T1 is started when the external RF field is not detected any more. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_ON_INT_POS                                                     (11UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_ON_INT_MASK                                                    (0x00000800UL)  /**< 11-T1_START_EVENT: If set. the timer T1 is started when an internal RF field is turned on. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_OFF_INT_POS                                                    (10UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_RF_OFF_INT_MASK                                                   (0x00000400UL)  /**< 10-T1_START_EVENT: If set. the timer T1 is started when an internal RF field is turned off. */
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_FRAMESTEP_POS                                                  (9UL)
#define CLIF_TIMER1_CONFIG_T1_START_ON_TX_FRAMESTEP_MASK                                                 (0x00000200UL)  /**< 9-T1_START_EVENT: If set. the timer T1 is started when an activity on Framestep is detected.*/
#define CLIF_TIMER1_CONFIG_T1_START_NOW_POS                                                              (8UL)
#define CLIF_TIMER1_CONFIG_T1_START_NOW_MASK                                                             (0x00000100UL)  /**< 8-T1_START_EVENT: If set. the timer T1 is started immediatly. */
#define CLIF_TIMER1_CONFIG_RESERVED2_POS                                                                  (7UL)
#define CLIF_TIMER1_CONFIG_RESERVED2_MASK                                                                 (0x00000080UL)  /**< 7-Reserved */
#define CLIF_TIMER1_CONFIG_T1_ONE_SHOT_MODE_POS                                                          (6UL)
#define CLIF_TIMER1_CONFIG_T1_ONE_SHOT_MODE_MASK                                                         (0x00000040UL)  /**< 6-When set to 1 the counter value does not reload again until the counter value has reached zero*/
#define CLIF_TIMER1_CONFIG_T1_PRESCALE_SEL_POS                                                           (3UL)
#define CLIF_TIMER1_CONFIG_T1_PRESCALE_SEL_MASK                                                          (0x00000038UL)  /**< 3-Controls frequency/period of the timer T1 when the prescaler is activated by T1_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter */
#define CLIF_TIMER1_CONFIG_T1_MODE_SEL_POS                                                               (2UL)
#define CLIF_TIMER1_CONFIG_T1_MODE_SEL_MASK                                                              (0x00000004UL)  /**< 2-If set. the timer T1 is started the prescaler for the timer T1 is enabled. 0* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1 Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T1_PRESCALE_SEL). */
#define CLIF_TIMER1_CONFIG_T1_RELOAD_ENABLE_POS                                                          (1UL)
#define CLIF_TIMER1_CONFIG_T1_RELOAD_ENABLE_MASK                                                         (0x00000002UL)  /**< 1-If set to 0.the timer T1 will stop on expiration. 0* After expiration the timer T1 will stop counting. i.e.. remain zero. reset value. 1 After expiration the timer T1 will reload its preset value and continue counting down. */
#define CLIF_TIMER1_CONFIG_T1_ENABLE_POS                                                                 (0UL)
#define CLIF_TIMER1_CONFIG_T1_ENABLE_MASK                                                                (0x00000001UL)  /**< 0-Enables the timer T1*/

#define CLIF_TIMER1_RELOAD                                                                                0x40
#define CLIF_TIMER1_RELOAD_RESERVED_POS                                                                  (20UL)
#define CLIF_TIMER1_RELOAD_RESERVED_MASK                                                                 (0xFFF00000UL)  /**< 20-Reserved */
#define CLIF_TIMER1_RELOAD_T1_RELOAD_VALUE_POS                                                           (0UL)
#define CLIF_TIMER1_RELOAD_T1_RELOAD_VALUE_MASK                                                          (0x000FFFFFUL)  /**< 0-Reload value of the timer T1.*/

#define CLIF_SPARE2_REG                                                                                   0x41
#define CLIF_SPARE2_REG_RFU_POS                                                                          (0UL)
#define CLIF_SPARE2_REG_RFU_MASK                                                                         (0xFFFFFFFFUL)  /**< 0-RFU*/

#define CLIF_SPARE3_REG                                                                                   0x42
#define CLIF_SPARE3_REG_RFU_POS                                                                          (0UL)
#define CLIF_SPARE3_REG_RFU_MASK                                                                         (0xFFFFFFFFUL)  /**< 0-RFU*/

#define CLIF_ANA_RX_CTRL                                                                                  0x43
#define CLIF_ANA_RX_CTRL_RESERVED_POS                                                                    (27UL)
#define CLIF_ANA_RX_CTRL_RESERVED_MASK                                                                   (0xF8000000UL)  /**< 27-reserved*/
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_HP_EN_POS                                                            (26UL)
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_HP_EN_MASK                                                           (0x04000000UL)  /**< 26-*/
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_MON_EN_POS                                                           (25UL)
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_MON_EN_MASK                                                          (0x02000000UL)  /**< 25-enable signal for the monitoring buffer*/
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_LATCH_EN_POS                                                         (24UL)
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_LATCH_EN_MASK                                                        (0x01000000UL)  /**< 24-enable signal for the DPLL latch*/
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_BUFFER_EN_POS                                                        (23UL)
#define CLIF_ANA_RX_CTRL_RX_CLK_BUF_BUFFER_EN_MASK                                                       (0x00800000UL)  /**< 23-enable signal for the ADPLL buffer*/
#define CLIF_ANA_RX_CTRL_RX_MIXER_SS_MODE_EN_POS                                                         (22UL)
#define CLIF_ANA_RX_CTRL_RX_MIXER_SS_MODE_EN_MASK                                                        (0x00400000UL)  /**< 22-13M56 sampling rate, to be used for test purpose only, makes the mixer sensitive to DC*/
#define CLIF_ANA_RX_CTRL_RX_ATB_MON_SEL_POS                                                              (19UL)
#define CLIF_ANA_RX_CTRL_RX_ATB_MON_SEL_MASK                                                             (0x00380000UL)  /**< 19-configuration bits for ATB signal monitoring, validation/test purpose*/
#define CLIF_ANA_RX_CTRL_RX_CLKGEN_PH_CTRL_POS                                                           (16UL)
#define CLIF_ANA_RX_CTRL_RX_CLKGEN_PH_CTRL_MASK                                                          (0x00070000UL)  /**< 16-used to add delay to the I/Q RX mixer CLK - step is 1 / 108M*/
#define CLIF_ANA_RX_CTRL_RX_CLKGEN_EN_POS                                                                (15UL)
#define CLIF_ANA_RX_CTRL_RX_CLKGEN_EN_MASK                                                               (0x00008000UL)  /**< 15-Signal enable for RX_CLKGEN synchro*/
#define CLIF_ANA_RX_CTRL_RX_BIAS_EN_POS                                                                  (14UL)
#define CLIF_ANA_RX_CTRL_RX_BIAS_EN_MASK                                                                 (0x00004000UL)  /**< 14-enable signal for RX bias circuit */
#define CLIF_ANA_RX_CTRL_RX_RSSI_EN_POS                                                                  (13UL)
#define CLIF_ANA_RX_CTRL_RX_RSSI_EN_MASK                                                                 (0x00002000UL)  /**< 13-enable signal for RSSI*/
#define CLIF_ANA_RX_CTRL_RFU_POS                                                                         (12UL)
#define CLIF_ANA_RX_CTRL_RFU_MASK                                                                        (0x00001000UL)  /**< 12-Reserved*/
#define CLIF_ANA_RX_CTRL_RX_RSSI_HIGAIN_POS                                                              (11UL)
#define CLIF_ANA_RX_CTRL_RX_RSSI_HIGAIN_MASK                                                             (0x00000800UL)  /**< 11-*/
#define CLIF_ANA_RX_CTRL_RX_RSSI_DC_TRIM_POS                                                             (7UL)
#define CLIF_ANA_RX_CTRL_RX_RSSI_DC_TRIM_MASK                                                            (0x00000780UL)  /**< 7-used for adjusting RSSI DC Offset*/
#define CLIF_ANA_RX_CTRL_RX_MIXER_SE_MODE_EN_POS                                                         (6UL)
#define CLIF_ANA_RX_CTRL_RX_MIXER_SE_MODE_EN_MASK                                                        (0x00000040UL)  /**< 6-enable signal for selecting single ended mode*/
#define CLIF_ANA_RX_CTRL_RX_BBA_FTRIM_SEL_POS                                                            (4UL)
#define CLIF_ANA_RX_CTRL_RX_BBA_FTRIM_SEL_MASK                                                           (0x00000030UL)  /**< 4-used to adjust RX BBA filter cut off frequency*/
#define CLIF_ANA_RX_CTRL_RX_BBA_MON_EN_POS                                                               (3UL)
#define CLIF_ANA_RX_CTRL_RX_BBA_MON_EN_MASK                                                              (0x00000008UL)  /**< 3-enable signal for diff to single monitoring buffers*/
#define CLIF_ANA_RX_CTRL_RX_ADC_EN_POS                                                                   (2UL)
#define CLIF_ANA_RX_CTRL_RX_ADC_EN_MASK                                                                  (0x00000004UL)  /**< 2-enable signal for RX ADCs*/
#define CLIF_ANA_RX_CTRL_RX_MIXER_EN_POS                                                                 (1UL)
#define CLIF_ANA_RX_CTRL_RX_MIXER_EN_MASK                                                                (0x00000002UL)  /**< 1-enable signal for  RX mixers*/
#define CLIF_ANA_RX_CTRL_RX_BBA_EN_POS                                                                   (0UL)
#define CLIF_ANA_RX_CTRL_RX_BBA_EN_MASK                                                                  (0x00000001UL)  /**< 0-enable signal for the baseband amplifiers*/

#define CLIF_ANACTRL_TX_CONFIG                                                                            0x44
#define CLIF_ANACTRL_TX_CONFIG_RESERVED_POS                                                              (22UL)
#define CLIF_ANACTRL_TX_CONFIG_RESERVED_MASK                                                             (0xFFC00000UL)  /**< 22-Reserved */
#define CLIF_ANACTRL_TX_CONFIG_TX_PH_SHIFT_MOD10_POS                                                     (18UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_PH_SHIFT_MOD10_MASK                                                    (0x003C0000UL)  /**< 18-Applies a tx_ph_shift_mod10x0.9 degree on ck13m56_tx wrt ck13m56. Value is unsigned, ranging from 0 to 9. Final tx phase shift is the following: (tx_ph_shift_div10x10+tx_ph_shift_mod10)x0.9 degree*/
#define CLIF_ANACTRL_TX_CONFIG_TX_PH_SHIFT_DIV10_POS                                                     (13UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_PH_SHIFT_DIV10_MASK                                                    (0x0003E000UL)  /**< 13-Applies a tx_ph_shift_div10x10x0.9 degree on ck13m56_tx wrt ck13m56. Value is signed 2s complement, ranging from -16 to 15, corresponding to -144 degree to 135 degree, with steps of 9 degree*/
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_CW_CM_POS                                                         (11UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_CW_CM_MASK                                                        (0x00001800UL)  /**< 11-Shift driver waves of 180 degrees in CM Continuous Wave. Index 0: TX1; index 1: TX2*/
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_MOD_CM_POS                                                        (9UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_MOD_CM_MASK                                                       (0x00000600UL)  /**< 9-Shift driver waves of 180 degrees in CM Modulated Wave. Index 0: TX1; index 1: TX2*/
#define CLIF_ANACTRL_TX_CONFIG_TX_INVL_CM_POS                                                            (7UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_INVL_CM_MASK                                                           (0x00000180UL)  /**< 7-inverse polarity of driver waves in CM. Index 0: TX1; index 1: TX2*/
#define CLIF_ANACTRL_TX_CONFIG_TX_PWM_MODE_CM_POS                                                        (6UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_PWM_MODE_CM_MASK                                                       (0x00000040UL)  /**< 6-PWM scheme for CM: 0: TX1/2 3-levels (If at least either clk_mode_tx1 or clk_mode_tx2 is set on a clocked mode) 1: TX1/2 2-levels with differentiated pulses*/
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_RM_POS                                                            (4UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_INVP_RM_MASK                                                           (0x00000030UL)  /**< 4-shift driver waves of 180 in RM. Index 0: TX1; index 1: TX2*/
#define CLIF_ANACTRL_TX_CONFIG_TX_INVL_POS                                                               (2UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_INVL_MASK                                                              (0x0000000CUL)  /**< 2-inverse polarity of driver waves in RM. Index 0: TX1; index 1: TX2*/
#define CLIF_ANACTRL_TX_CONFIG_TX_PWM_MODE_RM_POS                                                        (1UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_PWM_MODE_RM_MASK                                                       (0x00000002UL)  /**< 1-PWM scheme for RM: 0: TX1/2 3-levels (If at least either clk_mode_tx1 or clk_mode_tx2 is set on a clocked mode) 1: TX1/2 2-levels with differentiated pulses*/
#define CLIF_ANACTRL_TX_CONFIG_TX_EN_POS                                                                 (0UL)
#define CLIF_ANACTRL_TX_CONFIG_TX_EN_MASK                                                                (0x00000001UL)  /**< 0-Enables transmitter*/

#define CLIF_ANACTRL_TX_NOV                                                                               0x45
#define CLIF_ANACTRL_TX_NOV_RESERVED_POS                                                                 (27UL)
#define CLIF_ANACTRL_TX_NOV_RESERVED_MASK                                                                (0xF8000000UL)  /**< 27-Reserved */
#define CLIF_ANACTRL_TX_NOV_TX_NOV_OFFSET_IN_POS                                                         (22UL)
#define CLIF_ANACTRL_TX_NOV_TX_NOV_OFFSET_IN_MASK                                                        (0x07C00000UL)  /**< 22-Offset value to be loaded based on SEL_OFFSET_CNT*/
#define CLIF_ANACTRL_TX_NOV_TX_SEL_OFFSET_CNT_POS                                                        (19UL)
#define CLIF_ANACTRL_TX_NOV_TX_SEL_OFFSET_CNT_MASK                                                       (0x00380000UL)  /**< 19-Non-overlap regulation counter selection*/
#define CLIF_ANACTRL_TX_NOV_TX_OFFSET_LOAD_POS                                                           (18UL)
#define CLIF_ANACTRL_TX_NOV_TX_OFFSET_LOAD_MASK                                                          (0x00040000UL)  /**< 18-If set to 1, overrides the non-overlap regulation counter selected by SEL_OFFSET_CNT with NOV_OFFSET_IN*/
#define CLIF_ANACTRL_TX_NOV_TX_SET_NOV_TIME_FW_POS                                                       (15UL)
#define CLIF_ANACTRL_TX_NOV_TX_SET_NOV_TIME_FW_MASK                                                      (0x00038000UL)  /**< 15-Non-overlap window target setting for the PMOS/NMOS/free-wheel in the 3-levels mode (400ps*(set_nov_time+1))*/
#define CLIF_ANACTRL_TX_NOV_TX_SET_NOV_TIME_FP_POS                                                       (12UL)
#define CLIF_ANACTRL_TX_NOV_TX_SET_NOV_TIME_FP_MASK                                                      (0x00007000UL)  /**< 12-Non-overlap window target setting for the PMOS/NMOS in 2-levels modes (400ps*(set_nov_time+1))*/
#define CLIF_ANACTRL_TX_NOV_TX_FRWH_THRESH_POS                                                           (5UL)
#define CLIF_ANACTRL_TX_NOV_TX_FRWH_THRESH_MASK                                                          (0x00000FE0UL)  /**< 5-Digital threshold below which the freewheel / 3 levels operation is fully activated */
#define CLIF_ANACTRL_TX_NOV_TX_AUTO_NOV_POS                                                              (1UL)
#define CLIF_ANACTRL_TX_NOV_TX_AUTO_NOV_MASK                                                             (0x0000001EUL)  /**< 1-Automatic triggering of non overlap regulation loop at RF. 0 : disable, /=0 :  enable regulation during 128/13.56MHz*AUTO_NOV after Rfon*/
#define CLIF_ANACTRL_TX_NOV_TX_FREEZE_NOV_POS                                                            (0UL)
#define CLIF_ANACTRL_TX_NOV_TX_FREEZE_NOV_MASK                                                           (0x00000001UL)  /**< 0-Freezes the automatic non-overlap regulation mechanism*/

#define CLIF_RX_EOF_PATTERN                                                                               0x46
#define CLIF_RX_EOF_PATTERN_RX_EOF_PATTERN_POS                                                           (0UL)
#define CLIF_RX_EOF_PATTERN_RX_EOF_PATTERN_MASK                                                          (0xFFFFFFFFUL)  /**< 0-*/

#define CLIF_RX_EMD_1_CONFIG                                                                              0x47
#define CLIF_RX_EMD_1_CONFIG_RESERVED_POS                                                                (17UL)
#define CLIF_RX_EMD_1_CONFIG_RESERVED_MASK                                                               (0xFFFE0000UL)  /**< 17-Reserved*/
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_IRQ_MASK_POS                                                         (16UL)
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_IRQ_MASK_MASK                                                        (0x00010000UL)  /**< 16-If asserted, the detected EMD is not reported via EMD IRQ. Otherwise every detected EMD causes the EMD IRQ assertion.*/
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_LEVEL_THRESH_1_POS                                                   (8UL)
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_LEVEL_THRESH_1_MASK                                                  (0x0000FF00UL)  /**< 8-Defines the EMD detection threshold when the number of received bits >RX_EMD_LENGTH_THRESH_0 If currently calculated EMD level more than RX_EMD_LEVEL_THRESH_1 - the frame is classified as EMD, otherwise its normal Frame.*/
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_LEVEL_THRESH_0_POS                                                   (0UL)
#define CLIF_RX_EMD_1_CONFIG_RX_EMD_LEVEL_THRESH_0_MASK                                                  (0x000000FFUL)  /**< 0-Defines the EMD detection threshold when the number of received bits =< RX_EMD_LENGTH_THRESH_0 If currently calculated EMD level more than RX_EMD_LEVEL_THRESH_0 - the frame is classified as EMD, otherwise its normal Frame.*/

#define CLIF_RX_EMD_0_CONFIG                                                                              0x48
#define CLIF_RX_EMD_0_CONFIG_RX_EMD_LENGTH_THRESH_0_POS                                                  (24UL)
#define CLIF_RX_EMD_0_CONFIG_RX_EMD_LENGTH_THRESH_0_MASK                                                 (0xFF000000UL)  /**< 24-Threshold in number of received data bits which defines which group of EMD config parameters must be used for the EMD detection.If the number of received bits >RX_EMD_LENGTH_THRESH_0  the *_IS_EMD_1 group of parameters must be used for the EMD classification, otherwise the*_IS_EMD_0 group.*/
#define CLIF_RX_EMD_0_CONFIG_RESERVED_POS                                                                (23UL)
#define CLIF_RX_EMD_0_CONFIG_RESERVED_MASK                                                               (0x00800000UL)  /**< 23-Reserved*/
#define CLIF_RX_EMD_0_CONFIG_RX_MAXLEN_FRAME_IS_EMD_POS                                                  (22UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MAXLEN_FRAME_IS_EMD_MASK                                                 (0x00400000UL)  /**< 22-If asserted and the received frame length violates the max frame length configured via the RX_FRAME_MAXLEN register - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_MINLEN_FRAME_IS_EMD_POS                                                  (21UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MINLEN_FRAME_IS_EMD_MASK                                                 (0x00200000UL)  /**< 21-If asserted and the received frame length violates the min frame length configured via the RX_FRAME_MINLEN register - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_LEN_ERR_IS_EMD_POS                                                       (20UL)
#define CLIF_RX_EMD_0_CONFIG_RX_LEN_ERR_IS_EMD_MASK                                                      (0x00100000UL)  /**< 20-If asserted and the LEN field is a part of the frame, the wrong received value in the field is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_TERM_IS_EMD_1_POS                                                 (19UL)
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_TERM_IS_EMD_1_MASK                                                (0x00080000UL)  /**< 19-If asserted, the number of received bits>RX_EMD_LENGTH_THRESH_0 and the SGP_MSG_RXOVER_TERM message is received - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_ERR_IS_EMD_1_POS                                                  (18UL)
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_ERR_IS_EMD_1_MASK                                                 (0x00040000UL)  /**< 18-If asserted, the number of received bits >RX_EMD_LENGTH_THRESH_0 and either SGP_MSG_RXOVER_ERROR or SGP_MSG_ERROR message is received - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_NOSTOP_DET_IS_EMD_1_POS                                                  (17UL)
#define CLIF_RX_EMD_0_CONFIG_RX_NOSTOP_DET_IS_EMD_1_MASK                                                 (0x00020000UL)  /**< 17-If asserted, the number of received bits >RX_EMD_LENGTH_THRESH_0 and the frame reception is over being not interrupted by any detected error or external rf-off event but no any stop conditions, configured via EOF_COND_SCALE register, were detected -the scenario is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_CRC_ERR_IS_EMD_1_POS                                                     (16UL)
#define CLIF_RX_EMD_0_CONFIG_RX_CRC_ERR_IS_EMD_1_MASK                                                    (0x00010000UL)  /**< 16-If asserted and the number of received bits>RX_EMD_LENGTH_THRESH_0 - the detection of the CRC error is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_NOT_FULL_BYTE_IS_EMD_1_POS                                               (15UL)
#define CLIF_RX_EMD_0_CONFIG_RX_NOT_FULL_BYTE_IS_EMD_1_MASK                                              (0x00008000UL)  /**< 15-If asserted and the number of received bits>RX_EMD_LENGTH_THRESH_0 - the reception of last byte with size < 8 data bits is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_STOPBIT_IS_EMD_1_POS                                             (14UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_STOPBIT_IS_EMD_1_MASK                                            (0x00004000UL)  /**< 14-If asserted, the number of received bits >RX_EMD_LENGTH_THRESH_0 and the stop bit is a part of the frame character, the reception of the last character with 8 data bits but without stop bit is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_PARITY_IS_EMD_1_POS                                              (13UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_PARITY_IS_EMD_1_MASK                                             (0x00002000UL)  /**< 13-If asserted, the number of received bits >RX_EMD_LENGTH_THRESH_0 and the parity bit is a part of the frame character, the reception of the last character with 8 data bits but without parity bit is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_PARBIT_ERR_IS_EMD_1_POS                                                  (12UL)
#define CLIF_RX_EMD_0_CONFIG_RX_PARBIT_ERR_IS_EMD_1_MASK                                                 (0x00001000UL)  /**< 12-If asserted and the number of received bits >RX_EMD_LENGTH_THRESH_0 - the detected parity error is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_STOPBIT_ERR_IS_EMD_1_POS                                                 (11UL)
#define CLIF_RX_EMD_0_CONFIG_RX_STOPBIT_ERR_IS_EMD_1_MASK                                                (0x00000800UL)  /**< 11-If asserted and the number of received bits >RX_EMD_LENGTH_THRESH_0 - the detected stop bit error (0 level instead of 1 on the stop bit position) is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_COLL_IS_EMD_1_POS                                                        (10UL)
#define CLIF_RX_EMD_0_CONFIG_RX_COLL_IS_EMD_1_MASK                                                       (0x00000400UL)  /**< 10-If asserted and the number of received bits >RX_EMD_LENGTH_THRESH_0 - the collision is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_TERM_IS_EMD_0_POS                                                 (9UL)
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_TERM_IS_EMD_0_MASK                                                (0x00000200UL)  /**< 9-If asserted, the number of received bits =<RX_EMD_LENGTH_THRESH_0 and the SGP_MSG_RXOVER_TERM message is received - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_ERR_IS_EMD_0_POS                                                  (8UL)
#define CLIF_RX_EMD_0_CONFIG_RX_SIGPRO_ERR_IS_EMD_0_MASK                                                 (0x00000100UL)  /**< 8- If asserted, the number of received bits =<RX_EMD_LENGTH_THRESH_0 and either SGP_MSG_RXOVER_ERROR or SGP_MSG_ERROR message is received - the event is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_NOSTOP_DET_IS_EMD_0_POS                                                  (7UL)
#define CLIF_RX_EMD_0_CONFIG_RX_NOSTOP_DET_IS_EMD_0_MASK                                                 (0x00000080UL)  /**< 7-If asserted, the number of received bits =<RX_EMD_LENGTH_THRESH_0 and the frame reception is over being not interrupted by any detected error or external rf-off event but no any stop conditions, configured via EOF_COND_SCALE register, were detected  -the scenario is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_CRC_ERR_IS_EMD_0_POS                                                     (6UL)
#define CLIF_RX_EMD_0_CONFIG_RX_CRC_ERR_IS_EMD_0_MASK                                                    (0x00000040UL)  /**< 6-If asserted and the number of received bits =<RX_EMD_LENGTH_THRESH_0 - the detection of the CRC error is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_NOT_FULL_BYTE_IS_EMD_0_POS                                               (5UL)
#define CLIF_RX_EMD_0_CONFIG_RX_NOT_FULL_BYTE_IS_EMD_0_MASK                                              (0x00000020UL)  /**< 5-If asserted and the number of received bits =<RX_EMD_LENGTH_THRESH_0 - the reception of last byte with size < 8 data bits is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_STOPBIT_IS_EMD_0_POS                                             (4UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_STOPBIT_IS_EMD_0_MASK                                            (0x00000010UL)  /**< 4-If asserted, the number of received bits =<RX_EMD_LENGTH_THRESH_0 and the stop bit is a part of the frame character, the reception of the last character with 8 data bits but without stop bit is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_PARITY_IS_EMD_0_POS                                              (3UL)
#define CLIF_RX_EMD_0_CONFIG_RX_MISSING_PARITY_IS_EMD_0_MASK                                             (0x00000008UL)  /**< 3-If asserted, the number of received bits =<RX_EMD_LENGTH_THRESH_0 and the parity bit is a part of the frame character, the reception of the last character with 8 data bits but without parity bit is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_PARBIT_ERR_IS_EMD_0_POS                                                  (2UL)
#define CLIF_RX_EMD_0_CONFIG_RX_PARBIT_ERR_IS_EMD_0_MASK                                                 (0x00000004UL)  /**< 2-If asserted and the number of received bits =<RX_EMD_LENGTH_THRESH_0 - the detected parity error is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_STOPBIT_ERR_IS_EMD_0_POS                                                 (1UL)
#define CLIF_RX_EMD_0_CONFIG_RX_STOPBIT_ERR_IS_EMD_0_MASK                                                (0x00000002UL)  /**< 1-If asserted and the number of received bits =<RX_EMD_LENGTH_THRESH_0 - the detected stop bit error (0 level instead of 1 on the stop bit position) is considered as EMD symptom.*/
#define CLIF_RX_EMD_0_CONFIG_RX_COLL_IS_EMD_0_POS                                                        (0UL)
#define CLIF_RX_EMD_0_CONFIG_RX_COLL_IS_EMD_0_MASK                                                       (0x00000001UL)  /**< 0-If asserted and the number of received bits=<RX_EMD_LENGTH_THRESH_0 - the collision is considered as EMD symptom.*/

#define LPCD_CALIBRATE_CTRL                                                                               0x49
#define LPCD_CALIBRATE_CTRL_FREEZE_VALUES_POS                                                            (30UL)
#define LPCD_CALIBRATE_CTRL_FREEZE_VALUES_MASK                                                           (0x40000000UL)  /**< 30-Freeze the RSSI Target and hysteresis values into UserArea*/
#define LPCD_CALIBRATE_CTRL_RFU_POS                                                                      (24UL)
#define LPCD_CALIBRATE_CTRL_RFU_MASK                                                                     (0x3F000000UL)  /**< 24-Reserved*/
#define LPCD_CALIBRATE_CTRL_RSSI_HYSTERESIS_POS                                                          (16UL)
#define LPCD_CALIBRATE_CTRL_RSSI_HYSTERESIS_MASK                                                         (0x00FF0000UL)  /**< 16-[REG] Value to be set in CLIF_DGRM_RSSI_REG_DGRM_RSSI_HYST used for calibration*/
#define LPCD_CALIBRATE_CTRL_RSSI_TARGET_POS                                                              (0UL)
#define LPCD_CALIBRATE_CTRL_RSSI_TARGET_MASK                                                             (0x0000FFFFUL)  /**< 0-[REG] Value to be set in CLIF_DGRM_RSSI_REG_DGRM_RSSI_TARGET used for calibration*/

#define IQ_CHANNEL_VALS                                                                                   0x4A
#define IQ_CHANNEL_VALS_Q_CHANNEL_VAL_POS                                                                (16UL)
#define IQ_CHANNEL_VALS_Q_CHANNEL_VAL_MASK                                                               (0xFFFF0000UL)  /**< 16-Q Channel value*/
#define IQ_CHANNEL_VALS_I_CHANNEL_VAL_POS                                                                (0UL)
#define IQ_CHANNEL_VALS_I_CHANNEL_VAL_MASK                                                               (0x0000FFFFUL)  /**< 0-I Channel value*/

#define PAD_CONFIG                                                                                        0x4B
#define PAD_CONFIG_AUX3_PUPD_VAL_POS                                                                     (28UL)
#define PAD_CONFIG_AUX3_PUPD_VAL_MASK                                                                    (0x30000000UL)  /**< 28-Enables PullUp/Down for AUX_3 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up) */
#define PAD_CONFIG_AUX2_PUPD_VAL_POS                                                                     (26UL)
#define PAD_CONFIG_AUX2_PUPD_VAL_MASK                                                                    (0x0C000000UL)  /**< 26-Enables PullUp/Down for AUX_2 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up)*/
#define PAD_CONFIG_AUX1_PUPD_VAL_POS                                                                     (24UL)
#define PAD_CONFIG_AUX1_PUPD_VAL_MASK                                                                    (0x03000000UL)  /**< 24-Enables PullUp/Down for AUX_1 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up) */
#define PAD_CONFIG_GPIO3_PUPD_VAL_POS                                                                    (22UL)
#define PAD_CONFIG_GPIO3_PUPD_VAL_MASK                                                                   (0x00C00000UL)  /**< 22-Enable PullUp/Down for GPIO3 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up)*/
#define PAD_CONFIG_GPIO2_PUPD_VAL_POS                                                                    (20UL)
#define PAD_CONFIG_GPIO2_PUPD_VAL_MASK                                                                   (0x00300000UL)  /**< 20-Enable PullUp/Down for GPIO2 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up)*/
#define PAD_CONFIG_GPIO1_PUPD_VAL_POS                                                                    (18UL)
#define PAD_CONFIG_GPIO1_PUPD_VAL_MASK                                                                   (0x000C0000UL)  /**< 18-Enables PullUp/Down for GPIO1 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up) */
#define PAD_CONFIG_GPIO0_PUPD_VAL_POS                                                                    (16UL)
#define PAD_CONFIG_GPIO0_PUPD_VAL_MASK                                                                   (0x00030000UL)  /**< 16-Enables PullUp/Down for GPIO0 (00: Weak Pull-Down; 01: No Pull; 10: Bus Keeper; 11: Weak Pull-Up)*/
#define PAD_CONFIG_RESERVED_POS                                                                          (7UL)
#define PAD_CONFIG_RESERVED_MASK                                                                         (0x0000FF80UL)  /**< 7-RESERVED*/
#define PAD_CONFIG_AUX3_OUTPUT_VAL_POS                                                                   (6UL)
#define PAD_CONFIG_AUX3_OUTPUT_VAL_MASK                                                                  (0x00000040UL)  /**< 6-Output value for AUX3*/
#define PAD_CONFIG_AUX2_OUTPUT_VAL_POS                                                                   (5UL)
#define PAD_CONFIG_AUX2_OUTPUT_VAL_MASK                                                                  (0x00000020UL)  /**< 5-Output value for AUX2*/
#define PAD_CONFIG_AUX1_OUTPUT_VAL_POS                                                                   (4UL)
#define PAD_CONFIG_AUX1_OUTPUT_VAL_MASK                                                                  (0x00000010UL)  /**< 4-Output value for AUX1*/
#define PAD_CONFIG_GPIO3_OUTPUT_VAL_POS                                                                  (3UL)
#define PAD_CONFIG_GPIO3_OUTPUT_VAL_MASK                                                                 (0x00000008UL)  /**< 3-Output value for GPIO3*/
#define PAD_CONFIG_GPIO2_OUTPUT_VAL_POS                                                                  (2UL)
#define PAD_CONFIG_GPIO2_OUTPUT_VAL_MASK                                                                 (0x00000004UL)  /**< 2-Output value for GPIO2*/
#define PAD_CONFIG_GPIO1_OUTPUT_VAL_POS                                                                  (1UL)
#define PAD_CONFIG_GPIO1_OUTPUT_VAL_MASK                                                                 (0x00000002UL)  /**< 1-Output value for GPIO1*/
#define PAD_CONFIG_GPIO0_OUTPUT_VAL_POS                                                                  (0UL)
#define PAD_CONFIG_GPIO0_OUTPUT_VAL_MASK                                                                 (0x00000001UL)  /**< 0-Output value for GPIO0*/

#define LPCD_CALIBRATE_STATUS                                                                             0x4C
#define LPCD_CALIBRATE_STATUS_CALIBRATION_STATUS_POS                                                     (31UL)
#define LPCD_CALIBRATE_STATUS_CALIBRATION_STATUS_MASK                                                    (0x80000000UL)  /**< 31-Calibration Status 0 - Calibration Not Done, 1- Calibration Done*/

#define TXLDO_VDDPA_CONFIG                                                                                0x4D
#define TXLDO_VDDPA_CONFIG_RESERVED_POS                                                                  (8UL)
#define TXLDO_VDDPA_CONFIG_RESERVED_MASK                                                                 (0xFFFFFF00UL)  /**< 8-Reserved*/
#define TXLDO_VDDPA_CONFIG_VDDPA_CONFIG_POS                                                              (0UL)
#define TXLDO_VDDPA_CONFIG_VDDPA_CONFIG_MASK                                                             (0x000000FFUL)  /**< 0-Configures the VDDPA to the value set*/

#define GENERAL_ERROR_STATUS                                                                              0x4E
#define GENERAL_ERROR_STATUS_RFU_POS                                                                     (3UL)
#define GENERAL_ERROR_STATUS_RFU_MASK                                                                    (0xFFFFFFF8UL)  /**< 3-*/
#define GENERAL_ERROR_STATUS_TXLDO_ERROR_POS                                                             (2UL)
#define GENERAL_ERROR_STATUS_TXLDO_ERROR_MASK                                                            (0x00000004UL)  /**< 2-*/
#define GENERAL_ERROR_STATUS_CLOCK_ERROR_POS                                                             (1UL)
#define GENERAL_ERROR_STATUS_CLOCK_ERROR_MASK                                                            (0x00000002UL)  /**< 1-*/
#define GENERAL_ERROR_STATUS_GPADC_ERROR_POS                                                             (0UL)
#define GENERAL_ERROR_STATUS_GPADC_ERROR_MASK                                                            (0x00000001UL)  /**< 0-*/

#define TXLDO_VOUT_CURR                                                                                   0x4F
#define TXLDO_VOUT_CURR_TXLDO_CURRENT_POS                                                                (8UL)
#define TXLDO_VOUT_CURR_TXLDO_CURRENT_MASK                                                               (0x00FFFF00UL)  /**< 8-Incdicates the TXLDO Current*/
#define TXLDO_VOUT_CURR_VDDPA_VOUT_POS                                                                   (0UL)
#define TXLDO_VOUT_CURR_VDDPA_VOUT_MASK                                                                  (0x000000FFUL)  /**< 0-Indicates the current VDDPA Voltage configured*/


#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHHALHW_PN5190_REG_H_ */
