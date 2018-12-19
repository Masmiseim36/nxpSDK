/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf.h
 *
 * SIGFOX driver based on AML layer supporting boards based on following NXP
 * parts: OL2361, OL2385.
 *
 * This module is common for all supported models.
 */

#ifndef SOURCE_SF_SF_H_
#define SOURCE_SF_SF_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sf_model.h"
#include "sf_setup.h"
#include "sf_ol23xx.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
/*!
 * @brief This macro returns true when an acknowledgement is expected for a given
 * SPI command. Otherwise it returns false.
 *
 * @param cmd Command number (see sf_spi_cmd_t enumeration).
 */
#define SF_HAS_CMD_ACK(cmd) \
    (((cmd) != sfSpiCmdWakeup) && ((cmd) != sfSpiCmdSleep) && \
     ((cmd) != sfSpiCmdContWave) && ((cmd) != sfSpiCmdTriggerWd) && \
     ((cmd) != sfSpiCmdSwitchToPublicKey) && ((cmd) != sfSpiCmdPskAskZeroSeq) && \
     ((cmd) != sfSpiCmdPskAskAAAASeq))

/*!
 * @addtogroup sf_enum_group
 * Enumerations common for all models.
 * @{
 */
/*! @brief Specific error codes for this driver. */
enum _sf_status_t
{
    /*! Received an error from the OL23xx device via SPI. Use the
     * SF_GetErrorCode function to obtain the error code from the last
     * ACK frame received. */
    kStatus_SF_SpiAckError = MAKE_STATUS(kStatusGroup_SF, 0U),
    /*! Size of the receive buffer is smaller than data to be received
     * (indicated by the first byte of a received acknowledgement). */
    kStatus_SF_SpiAckLength = MAKE_STATUS(kStatusGroup_SF, 1U),
    /*! SPI communication timeout. Note that some commands take extra time
     * to send data to SIGFOX network (for example Send Payload command). */
    kStatus_SF_SpiTimeout = MAKE_STATUS(kStatusGroup_SF, 2U),
    /*! A SPI communication check performed in the SF_Init function failed. */
    kStatus_SF_SpiTestFail = MAKE_STATUS(kStatusGroup_SF, 3U),
};

/*! @brief SPI commands of the device. */
typedef enum
{
    sfSpiCmdSendEcho = SF_CMD_SEND_ECHO_ID,         /*!< Echo command. */
    sfSpiCmdWakeup = SF_CMD_SEND_WAKEUP_ID,         /*!< Send Wakeup command. */
    sfSpiCmdSleep = SF_CMD_SEND_TO_SLEEP_ID,        /*!< Send to sleep command. */
    sfSpiCmdSendPayload = SF_CMD_SEND_PAYLOAD_ID,   /*!< Send payload command. */
    sfSpiCmdSendBit = SF_CMD_SEND_BIT_ID,           /*!< Send bit command. */
    sfSpiCmdOutOfBand = SF_CMD_SEND_OUT_BAND_ID,    /*!< Send Out of Band command. */
    sfSpiCmdReceive = SF_CMD_RECEIVE_FR_ID,         /*!< Receive Frame command
                                                         (available on OL2385). */
    sfSpiCmdGetInfo = SF_CMD_GET_INFO_ID,           /*!< Get Info command. */
    sfSpiCmdGetUlFreq = SF_CMD_GET_UL_FREQ_ID,      /*!< Set UL frequency command. */
    sfSpiCmdSetUlFreq = SF_CMD_SET_UL_FREQ_ID,      /*!< Get UL frequency command. */
    sfSpiCmdContWave = SF_CMD_CONT_WAVE_ID,         /*!< Continuous Wave command. */
    sfSpiCmdGetDevVer = SF_CMD_GET_DEV_VER_ID,      /*!< Get Device Version command. */
    sfSpiCmdTriggerWd = SF_CMD_TRIGGER_WD_ID,       /*!< Trigger Watchdog command,
                                                         which resets the device
                                                         through SW (no WD handling). */
    sfSpiCmdSendTestMode = SF_CMD_SEND_TEST_ID,     /*!< Send Test Mode command. */
    sfSpiCmdChangeToRCZ1 = SF_CMD_CHANGE_TO_RCZ1_ID, /*!< Change to RCZ1 command
                                                     (European ETSI standard). */
    sfSpiCmdChangeToRCZ2 = SF_CMD_CHANGE_TO_RCZ2_ID, /*!< Change to RCZ2 command
                                                     (USA FCC standard). */
    sfSpiCmdChangeToRCZ3 = SF_CMD_CHANGE_TO_RCZ3_ID, /*!< Change to RCZ3 command
                                                     (Japan/Korean ARIB standard). */
    sfSpiCmdChangeToRCZ4 = SF_CMD_CHANGE_TO_RCZ4_ID, /*!< Change to RCZ4 command
                                                     (South American FCC standard). */
    sfSpiCmdSwitchToPrivateKey = SF_CMD_SWITCH_PRIVATE_ID, /*!< Switch to private
                                                         key command. */
    sfSpiCmdSwitchToPublicKey = SF_CMD_SWITCH_PUBLIC_ID, /*!< Switch to public
                                                         key command. */
    sfSpiCmdPskAskZeroSeq = SF_CMD_PSK_ASK_ZERO_ID, /*!< PSK ASK zero sequence
                                                         command. */
    sfSpiCmdPskAskAAAASeq = SF_CMD_PSK_ASK_AAAA_ID, /*!< PSK ASK AAAA sequence
                                                         command. */
    sfSpiCmdSetDlFreq = SF_CMD_SET_DL_FREQ_ID,      /*!< Set DL frequency command. */
    sfSpiCmdGetDlFreq = SF_CMD_GET_DL_FREQ_ID,      /*!< Get DL frequency command. */
    sfSpiCmdSetFCCMacroCh = SF_CMD_SET_FCC_CH_ID,   /*!< Set FCC macro channel
                                                         command. */
    sfSpiCmdGetFCCMacroChUsage = SF_CMD_GET_FCC_CH_ID, /*!< Get FCC macro channel
                                                         usage command. */
    sfSpiCmdResetFCCMacroCh = SF_CMD_RESET_FCC_CH_ID, /*!< Reset FCC macro channel
                                                         command. */
    sfSpiCmdStaticFreqCal = SF_CMD_STATIC_FREQ_CAL_ID, /*!< Static frequency
                                                         calibration command. */
    sfSpiCmdTempFreqCal = SF_CMD_TEMP_FREQ_CAL_ID,  /*!< Temperature frequency
                                                         calibration command. */
    sfSpiCmdGetLastSentRSSI = SF_CMD_GET_LAST_SENT_RSSI_ID, /*!< Get last sent
                                                         RSSI command. */
    sfSpiCmdUpdatePACurve = SF_CMD_UPDATE_PA_CURVE_ID, /*!< Update PA curve command. */
    sfSpiCmdInitRSSICal = SF_CMD_INIT_RSSI_CALIB_ID,   /*!< Initial RSSI calibration
                                                         command. */
} sf_spi_cmd_t;

/*! @brief The state of the device in OL23xx software state machine. */
typedef enum
{
    sfStateIdle = 0U,               /*!< Idle state. */
    sfStateInit,                    /*!< Init state (state after initialization
                                         and in sleep mode). */
    sfStateWaitForCmd               /*!< Wait for command state (state after
                                         wake-up). */
} sf_intern_state_t;

/*! @brief Error codes of the SIGFOX device included in SPI
 * acknowledgement frames. */
typedef enum
{
    sfErrNone = 0x00U,  /*!< No error */

    sfErrOpenMalloc = 0x10U,    /*!< Error on MANUF_API_malloc or buffer pointer NULL */
    sfErrOpenIdPtr = 0x11U,     /*!< ID pointer NULL */
    sfErrOpenGetSeq = 0x12U,    /*!< Error on MANUF_API_get_nv_mem w/ SFX_NVMEM_SEQ_CPT */
    sfErrOpenGetPn = 0x13U,     /*!< Error on MANUF_API_get_nv_mem w/ SFX_NVMEM_PN */
    sfErrOpenState = 0x14U,     /*!< State is not idle, library should be closed before */
    sfErrOpenGetFh = 0x15U,     /*!< Error on MANUF_API_get_nv_mem w/ SFX_NVMEM_FH */
    sfErrOpenMacroChannelWidth = 0x16U, /*!< Macro channel width not allowed */
    sfErrOpenRczPtr = 0x17U,    /*!< RCZ pointer is NULL */

    sfErrCloseFree = 0x20U,     /*!< Error on MANUF_API_free */
    sfErrCloseRfStop = 0x21U,   /*!< Error on MANUF_API_rf_stop */

    sfErrSendFrameDataLength = 0x30U, /*!< Customer data length > 12 Bytes */
    sfErrSendFrameState = 0x31U, /*!< State != READY, must close and reopen library */
    sfErrSendFrameResponsePtr = 0x32U, /*!< Response data pointer NULL in case of downlink */
    sfErrSendFrameBuildUplink = 0x33U, /*!< Build uplink frame failed */
    sfErrSendFrameSendUplink = 0x34U, /*!< Send uplink frame failed */
    sfErrSendFrameReceive = 0x35U, /*!< Receive downlink frame failed or timeout */
    sfErrSendFrameDelayOobAck = 0x36U, /*!< Error on MANUF_API_delay w/ SFX_DLY_OOB_ACK (Downlink) */
    sfErrSendFrameBuildOobAck = 0x37U, /*!< Build out of band frame failed (Downlink) */
    sfErrSendFrameSendOobAck = 0x38U, /*!< Send out of band frame failed (Downlink) */
    sfErrSendFrameDataPtr = 0x39U, /*!< Customer data pointer NULL */
    sfErrSendFrameCarrierSenseConfig = 0x3AU, /*!< Carrier Sense configuration need to be initialized */
    sfErrSendFrameCarrierSenseTimeout = 0x3BU, /*!< Wait for clear channel has returned time out */
    sfErrSendFrameWaitTimeout = 0x3EU, /*!< Wait frame has returned time out */
    sfErrSendFrameInvalidFhChan = 0x3FU, /*!< FH invalid channel, must call SIGFOX_API_reset */

    sfErrSendBitState = 0x41U,  /*!< State != READY, must close and reopen library */
    sfErrSendBitResponsePtr = 0x42U, /*!< Response data pointer NULL in case of downlink */
    sfErrSendBitBuildUplink = 0x43U, /*!< Build uplink frame failed */
    sfErrSendBitSendUplink = 0x44U, /*!< Send uplink frame failed */
    sfErrSendBitReceive = 0x45U, /*!< Receive downlink frame failed or timeout */
    sfErrSendBitDelayOobAck = 0x46U, /*!< Error on MANUF_API_delay w/ SFX_DLY_OOB_ACK (Downlink) */
    sfErrSendBitBuildOobAck = 0x47U, /*!< Build out of band frame failed (Downlink) */
    sfErrSendBitSendOobAck = 0x48U, /*!< Send out of band frame failed (Downlink) */
    sfErrSendBitDataPtr = 0x49U, /*!< Customer data pointer NULL */
    sfErrSendBitCarrierSenseConfig = 0x4AU, /*!< Carrier Sense configuration need to be initialized */
    sfErrSendBitCarrierSenseTimeout = 0x4BU, /*!< Wait for clear channel has returned time out */
    sfErrSendBitWaitTimeout = 0x4EU, /*!< Wait frame has returned time out */
    sfErrSendBitInvalidFhChan = 0x4FU, /*!< FH invalid channel, must call SIGFOX_API_reset */

    sfErrSendOobState = 0x51U,  /*!< State != READY, must close and reopen library */
    sfErrSendOobBuildUplink = 0x53U, /*!< Build uplink frame failed */
    sfErrSendOobSendUplink = 0x54U, /*!< Send uplink frame failed */
    sfErrSendOobInvalidFhChan = 0x5FU, /*!< Send out of band frame failed (Downlink) */

    sfErrSetStdConfigSigfoxChan = 0x90U, /*!< Default SIGFOX channel out of range */
    sfErrSetStdConfigSet = 0x91U, /*!< Unable to set configuration */

    sfErrTestMode0RfInit = 0xA0U, /*!< Error on MANUF_API_rf_init */
    sfErrTestMode0ChangeFreq = 0xA1U, /*!< Error on MANUF_API_change_frequency */
    sfErrTestMode0RfSend = 0xA2U, /*!< Error on MANUF_API_rf_send */
    sfErrTestMode0Delay = 0xA3U, /*!< Error on MANUF_API_delay */
    sfErrTestMode0RfStop = 0xA4U, /*!< Error on MANUF_API_rf_stop */

    sfErrTestModeState = 0xB1U, /*!< State != READY, must close and reopen library */

    sfErrTestMode2ReportTest = 0xC0U, /*!< Error on MANUF_API_report_test_result */

    sfErrTestMode3RfInit = 0xD0U, /*!< Error on MANUF_API_rf_init */
    sfErrTestMode3ChangeFreq = 0xD1U, /*!< Error on MANUF_API_change_frequency */
    sfErrTestMode3TimerStart = 0xD2U, /*!< Error on MANUF_API_timer_start */
    sfErrTestMode3ReportTest = 0xD3U, /*!< Error on MANUF_API_report_test_result */
    sfErrTestMode3TimerStop = 0xD4U, /*!< Error on MANUF_API_timer_stop */
    sfErrTestMode3RfStop = 0xD5U, /*!< Error on MANUF_API_rf_stop */

    sfErrTestMode4BuildUplink = 0xE0U, /*!< Build uplink frame failed */
    sfErrTestMode4SendUplink = 0xE1U, /*!< Send uplink frame failed */
    sfErrTestMode4ReportTest = 0xE2U, /*!< Error on MANUF_API_report_test_result */
    sfErrTestMode4GetRssi = 0xE3U, /*!< Error on MANUF_API_get_rssi */
    sfErrTestMode4Delay = 0xE4U, /*!< Error on MANUF_API_delay */

    sfErrTestMode5RfInit = 0xF0U, /*!< Error on MANUF_API_rf_init */
    sfErrTestMode5ChangeFreq = 0xF1U, /*!< Error on MANUF_API_change_frequency */
    sfErrTestMode5BuildUplink = 0xF2U, /*!< Build uplink frame failed */
    sfErrTestMode5SendUplink = 0xF3U, /*!< Send uplink frame failed */
    sfErrTestMode5RfStop = 0xF4U, /*!< Error on MANUF_API_rf_stop */

    sfErrUnknownSpiCommand = 0x01U, /*!< Unknown SPI command received. */
    sfErrManufNone = 0x00U,     /*!< No error */
    sfErrManufMalloc = 0x08U,   /*!< Error on MANUF_API_malloc */
    sfErrManufInit = 0x10U,     /*!< Error on MANUF_API_rf_init */
    sfErrManufSend = 0x18U,     /*!< Error on MANUF_API_rf_send */
    sfErrManufFree = 0x20U,     /*!< Error on MANUF_API_free */
    sfErrManufVoltTemp = 0x28U, /*!< Error on MANUF_API_get_voltage_temperature */
    sfErrManufRfStop = 0x30U,   /*!< Error on MANUF_API_rf_stop */
    sfErrManufDly = 0x38U,      /*!< Error on MANUF_API_delay */
    sfErrManufChFreq = 0x40U,   /*!< Error on MANUF_API_change_frequency */
    sfErrManufAes = 0x48U,      /*!< Error on MANUF_API_aes_128_cbc_encrypt */
    sfErrManufGetnvmem = 0x50U, /*!< Error on MANUF_API_get_nv_mem */
    sfErrManufSetnvmem = 0x58U, /*!< Error on MANUF_API_set_nv_mem */
    sfErrManufGetRssi = 0x60U,  /*!< Error on MANUF_API_get_rssi */
    sfErrManufWaitFrameTimeout = 0x68U, /*!< Error on MANUF_API_wait_frame */
    sfErrManufWaitCsTimeout = 0x69U, /*!< Error on MANUF_API_wait_for_clear_channel */
    sfErrManufTimerStart = 0x70U, /*!< Error on MANUF_API_timer_start */
    sfErrManufTimerStartCs = 0x71U, /*!< Error on MANUF_API_timer_start_carrier_sense */
    sfErrManufTimerStopCs = 0x72U, /*!< Error on MANUF_API_timer_stop_carrier_sense */
    sfErrManufTimerStop = 0x78U, /*!< Error on MANUF_API_timer_stop */
    sfErrManufTimerEnd = 0x80U, /*!< Error on MANUF_API_timer_wait_for_end */
    sfErrManufTestReport = 0x88U, /*!< Error on MANUF_API_report_test_result */
} sf_intern_error_t;

/*! @brief Selection of a test mode used to verify the protocol, RF and
 * sensitivity. Detailed description of the modes is in the device datasheet. */
typedef enum
{
    sfTestModeTxBpsk = 0U,      /*!< Sending PRBS data in a 26 Bytes frame
                                     at constant frequency. */
    sfTestModeTxProtocol,       /*!< Test of the complete protocol in up-link. */
    sfTestModeRxProtocol,       /*!< Test of the complete protocol in down-link. */
    sfTestModeRxGfsk,           /*!< Receiving constant GFSK frames at constant
                                     frequency. */
    sfTestModeRxSensi,          /*!< Measurement of the real sensitivity of
                                     device. */
    sfTestModTxSynth            /*!< Sending SIGFOX frames with 4 Bytes payload
                                     at forced frequency. */
} sf_test_mode_t;

/*! @brief Selection of XTAL type used in the device. */
typedef enum
{
    sfXtal552 = 0U,     /*!< 55.2 MHz Normal crystal. */
    sfXtal276 = 1U      /*!< 27.6 MHz External TCXO. */
} sf_xtal_type_t;

/*! @brief Amount of transmissions that will happen on
 *  one time invoking of SF_SendPayload function. */
typedef enum
{
    sfTxRepeat1 = 0U,     /*!< One TX transmission. */
    sfTxRepeat3 = 1U      /*!< Three TX transmissions. */
} sf_tx_repeat_t;

/*! @brief Selection of internal PA type used in the device. */
typedef enum
{
    sfPa14 = 0U,     /*!< 14 dBm PA. */
    sfPa0  = 1U      /*!< 0 dBm PA. */
} sf_pa_type_t;

/*! @brief List of standards defining network communication parameters. */
typedef enum
{
    sfNetStandardETSI = 0U,         /*!< European standard ETSI. Up-link frequency
                                         is 868 130 000 Hz, down-link frequency
                                         is 869 525 000 Hz, bit rate 100 bps. */
    sfNetStandardFCC_USA,           /*!< USA standard FCC. Up-link frequency
                                         is 902 200 000 Hz, down-link frequency
                                         is 905 200 000 Hz, bit rate 600 bps. */
    sfNetStandardARIB,              /*!< Japan/Korean standard ARIB. Up-link
                                         frequency is 923 200 000 Hz, down-link
                                         frequency is 922 200 000 Hz, bit rate
                                         600 bps with listen before talk. */
    sfNetStandardFCC_SouthAmerica,  /*!< South American standard FCC. Up-link
                                         frequency is 902 200 000 Hz, down-link
                                         frequency is 922 300 000 Hz, bit rate 600 bps. */
} sf_net_standard_t;
/*! @} */

/*!
 * @addtogroup sf_struct_group
 * Structures common for all models.
 * @{
 */
/*! @brief This structure is used by the user to initialize the SIGFOX device.
 * It contains a configuration of the SIGFOX device only (no SPI,
 * etc. configuration).  */
typedef struct
{
    sf_net_standard_t netStandard;  /*!< Selection of a standard defining network
                                         communication parameters. */
    uint8_t txAttSteps;             /*!< Amount of 0.25 dB steps that needs to be
                                         subtracted from TX power. */
    sf_xtal_type_t xtal;            /*!< Selection of XTAL type used in the device. */
    sf_tx_repeat_t txRepeat;        /*!< Amount of transmissions that will happen on
                                         one time invoking of SF_SendPayload function. */
    sf_pa_type_t paType;            /*!< Selection of internal PA type used in the
                                         device. */
} sf_user_config_t;

/*! @brief This data structure is used by the SIGFOX driver (it is the first
 * parameter of most SIGFOX functions). */
typedef struct
{
    sf_spi_config_t spiConfig;      /*!< SPI configuration. */
    sf_gpio_config_t gpioConfig;    /*!< GPIO configuration. */
    sf_intern_state_t devState;     /*!< The state of the SIGFOX device in
                                         software state machine received in
                                         the last acknowledgement. */
    sf_intern_error_t errorCode;    /*!< The error code received in the last
                                         acknowledgement. */
} sf_drv_data_t;

/*! @brief This structure represents the payload of SPI frames. */
typedef struct
{
    uint8_t payloadLen;             /*!< The size of the payload in bytes. */
    uint8_t *payload;               /*!< Pointer to the frame payload. */
} sf_msg_payload_t;

/*! @brief This structure contains information about the SIGFOX device. */
typedef struct
{
    uint32_t devId;                 /*!< Device ID pre-flashed on the device
                                         (result of the "Get info" command). */
    uint8_t devPac[SF_DEV_PAC_B];   /*!< PAC code pre-flashed on the device
                                         (result of the "Get info" command). */
    uint8_t devVersion[SF_DEV_VER_B]; /*!< Device version (result of the "Get
                                         Device Version" command). */
    uint8_t libVersion[SF_LIB_VER_B]; /*!< SIGFOX library version (result of the
                                         "Get info" command). */
} sf_device_info_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup sf_functions_group
 * @{
 */

/*!
 * @brief Initializes the SIGFOX driver based on user configuration.
 *
 * @param drvData    Driver run-time data.
 * @param userConfig User-provided configuration of the SIGFOX driver.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_Init(sf_drv_data_t *drvData, const sf_user_config_t *userConfig);

/*!
 * @brief Fills user configuration structure with default values,
 * i.e. ETSI standard, 55.2 MHz XTAL, 3 TX during SendPayload, 14 dBm PA.
 *
 * @param userConfig Pointer to a structure where the default SIGFOX
 *                   configuration is stored.
 */
void SF_GetDefaultConfig(sf_user_config_t *userConfig);

/*!
 * @brief This function sends a command to the device. This function waits for
 * an acknowledgement (if any).
 *
 * @param drvData      Driver run-time data.
 * @param cmd          The frame command number.
 * @param sendPayload  Pointer to a payload to be sent. The user can set payload
 *                     data (sendPayload->payload) to NULL when the size is zero.
 * @param recvPayload  Pointer where a resulting payload is stored. It can be
 *                     NULL if the payload is not desired or present in the
 *                     ACK frame. The user can use SF_HAS_CMD_ACK to check if
 *                     a command has an acknowledgement. Size of the data variable
 *                     (recvPayload->payload) must be sufficient
 *                     (see SF_ACK_PAYLOAD_MAX_B macro). The size variable
 *                     (recvPayload->payloadLen) will be set by this function
 *                     and indicates the number of actually received bytes.
 * @param recvBufferSize Size of the receive buffer in bytes.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SendCommand(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        const sf_msg_payload_t *sendPayload, sf_msg_payload_t *recvPayload,
        uint8_t recvBufferSize);

/*!
 * @brief This function sends the "Send Wakeup" command to wake up the device
 * from power down mode.
 *
 * Note that this command does not have an acknowledgement.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_WakeUp(sf_drv_data_t *drvData);

/*!
 * @brief This function puts the device into power off mode with use of the
 * "Send To Sleep" command. In this mode the device settings are reset to
 * default values by the SIGFOX device.
 *
 * Note: to lower power consumption of the SIGFOX device the user should set
 * MCU pins connected to the device to low except the CS pin which should be
 * asserted high.
 * Note that this command does not have an acknowledgement.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_Sleep(sf_drv_data_t *drvData);

/*!
 * @brief This function sets up-link frequency of the SIGFOX device.
 *
 * @param drvData Driver run-time data.
 * @param freqHz  Up-link frequency in Hz.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SetUlFrequency(sf_drv_data_t *drvData, uint32_t freqHz);

/*!
 * @brief This function gets up-link frequency of the SIGFOX device.
 *
 * @param drvData Driver run-time data.
 * @param freqHz  Pointer where the up-link frequency is stored in Hz.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetUlFrequency(sf_drv_data_t *drvData, uint32_t *freqHz);

/*!
 * @brief This function sends the "Send Payload" command to the device. It sends
 * user data to SIGFOX network.
 *
 * @param drvData     Driver run-time data.
 * @param sendPayload Pointer to a frame payload to be sent.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SendPayload(sf_drv_data_t *drvData, const sf_msg_payload_t *sendPayload);

/*!
 * @brief This function sends the "Send Bit" command, which transmits just one
 * bit to the SIGFOX network. This is the shortest frame that SIGFOX library
 * generates.
 *
 * Note: the Send bit SPI command has an additional parameter which initiates
 * receiving from Sigfox network. This parameter is not supported currently
 * (i.e. no receiving).
 *
 * @param drvData Driver run-time data.
 * @param bitVal  Value of the bit.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SendBit(sf_drv_data_t *drvData, uint8_t bitVal);

/*!
 * @brief This function sends the "Send Out of band" command, which transmits
 * data to the SIGFOX network. Data is composed of information about the chip
 * itself (Voltage, Temperature). This function should be called every 24 hours
 * at least or never if an application has some energy critical constraints.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SendOutOfBand(sf_drv_data_t *drvData);

#if (SF_MODEL == SF_MODEL_OL2385)
/*!
 * @brief This function sends and receives a data from SIGFOX network with use of the
 * "Receive Frame" command. It is available for OL2385 device only. Max. size
 * of received data is defined by #SF_RECEIVE_FR_ACK_PLD_B (SIGFOX limitation).
 *
 * @param drvData     Driver run-time data.
 * @param sendPayload Pointer to a frame payload to be sent.
 * @param recvPayload Pointer where the paylaod from a received ACK frame is
 *                    stored.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ReceiveMessage(sf_drv_data_t *drvData, const sf_msg_payload_t *sendPayload,
        sf_msg_payload_t *recvPayload);
#endif

/*!
 * @brief This function resets the SIGFOX device with use of the
 * "Trigger Watchdog" command. It does not handle watchdog.
 *
 * Note that this command does not have an acknowledgement.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_TriggerWatchdog(sf_drv_data_t *drvData);

/*!
 * @brief This function reads device ID, PAC, SIGFOX library version
 * and device version. It uses "Get info" and "Get Device Version" commands.
 *
 * @param drvData Driver run-time data.
 * @param devInfo Pointer where resulting device information is stored.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetDeviceInfo(sf_drv_data_t *drvData, sf_device_info_t *devInfo);

/*!
 * @brief This function returns the current state of the SIGFOX device
 * software state machine. The state is updated every time an ACK
 * SPI frame is received.
 *
 * @param drvData Driver run-time data.
 *
 * @return SIGFOX device state received in the last SPI frame.
 */
static inline sf_intern_state_t SF_GetState(sf_drv_data_t *drvData)
{
    return drvData->devState;
}

/*!
 * @brief This function returns an error code received in a SPI frame from
 * the SIGFOX device. The error code is updated every time an ACK SPI
 * frame is received.
 *
 * It clears the the error code (set to 0).
 *
 * @param drvData Driver run-time data.
 *
 * @return An error code received in the last SPI frame.
 */
sf_intern_error_t SF_GetErrorCode(sf_drv_data_t *drvData);

/*!
 * @brief This function tests if the SPI bus is working. It uses the "Echo"
 * command to send data (see #SF_ECHO_PLD_ARRAY macro in sf.c) and checks
 * if the device replies with the inverted payload.
 *
 * @param drvData Driver run-time data.
 * @param success Pointer to the check result (true - OK, false - error).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_TestSpiCon(sf_drv_data_t *drvData, bool *success);

/*!
 * @brief This function executes a test of up-link and down-link connectivity
 * using the "Send Test Mode" command. Returned status represents result
 * of the test (success or fail), the user can obtain details using the
 * SF_GetErrorCode function.
 *
 * @param drvData    Driver run-time data.
 * @param testMode   Test mode selection.
 * @param testConfig Test mode configuration.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_TestDevice(sf_drv_data_t *drvData, sf_test_mode_t testMode,
        uint8_t modeConfig);

/*!
 * @brief This function sends the "Continuous Wave" command, which can be used
 * for testing. It generates a continuous transmission at last set frequency.
 * The signal can be then checked using spectrum analyzer.
 *
 * Note that this command does not have an acknowledgement.
 *
 * @param drvData Driver run-time data.
 * @param enable  Enables or disables the continuous wave (true - enabled,
 * false - disabled).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GenerateContWave(sf_drv_data_t *drvData, bool enable);

/*!
 * @brief This function changes Sigfox networking parameters including up-link
 * and down-link frequency and bit rate.
 *
 * It enables to set European ETSI standard (default setting), USA FCC standard,
 * Japanese/Korean ARIB standard and South American FCC standard. It uses Change
 * To RCZ1, Change To RCZ2, Change To RCZ3 and Change To RCZ4 commands to set
 * an appropriate standard.
 *
 * @param drvData     Driver run-time data.
 * @param standardSel Selection of a standard defining network communication
 * parameters.
 * @param txAttSteps  Amount of 0.25dB steps that needs to be subtracted from
 * TX power (default value is 0).
 * @param xtal        Type of used XTAL.
 * @param txRepeat    Amount of transmissions that will happen on one time
 *                    invoking of SF_SendPayload function.
 * @param paType      Type of internal used PA.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ChangeNetworkStandard(sf_drv_data_t *drvData,
        sf_net_standard_t standardSel, uint8_t txAttSteps, sf_xtal_type_t xtal,
        sf_tx_repeat_t txRepeat, sf_pa_type_t paType);
/*!
 * @brief Switches to the private key provided by SIGFOX, the key is used
 * for normal RF.
 *
 * @param drvData      Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SwitchToPrivateKey(sf_drv_data_t *drvData);

/*!
 * @brief Switches to the public key provided by SIGFOX, it is used for protocol
 * tests.
 *
 * @param drvData      Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SwitchToPublicKey(sf_drv_data_t *drvData);

/*!
 * @brief Sends modulated zeros at currently set frequency (PSK ASK zero sequence
 * command).
 *
 * Note: since this command has no ACK, it is not very clear how long will it
 * take to send all frames. The user should wait after execution of this
 * function for a while and then can send other SPI commands.
 * Usually, this command is only for RF test.
 *
 * @param drvData      Driver run-time data.
 * @param repetitions  Number of transmission repetitions (0 - 255).
 *                     Transmission infinite loop is enabled when the value
 *                     is negative.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_TransmitZeroSeq(sf_drv_data_t *drvData, int16_t repetitions);

/*!
 * @brief Sends modulated AA at currently set frequency (PSK ASK AAAA sequence
 * command).
 *
 * Note: since this command has no ACK, it is not very clear how long will it
 * take to send all frames. The user should wait after execution of this
 * function for a while and then can send other SPI commands.
 * Usually, this command is only for RF test.
 *
 * @param drvData      Driver run-time data.
 * @param repetitions  Number of transmission repetitions (0 - 255).
 *                     Transmission infinite loop is enabled when the value
 *                     is negative.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_TransmitAAAASeq(sf_drv_data_t *drvData, int16_t repetitions);

/*!
 * @brief This function sets down-link frequency of the SIGFOX device.
 *
 * @param drvData Driver run-time data.
 * @param freqHz  Down-link frequency in Hz.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SetDlFrequency(sf_drv_data_t *drvData, uint32_t freqHz);

/*!
 * @brief This function gets down-link frequency of the SIGFOX device.
 *
 * @param drvData Driver run-time data.
 * @param freqHz  Pointer where the down-link frequency is stored in Hz.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetDlFrequency(sf_drv_data_t *drvData, uint32_t *freqHz);

/*!
 * @brief Sets the FCC Macro Channel.
 *
 * @param drvData Driver run-time data.
 * @param fhMask  Frequency hopping mask. Size of the array is 3 (number of bytes
 * is determined by #SF_MACRO_CH_CFG_B).
 * @param defaultChNo Default channel.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SetFCCMacroChannel(sf_drv_data_t *drvData, const uint32_t fhMask[],
        uint8_t defaultChNo);

/*!
 * @brief Gets the FCC Macro channel usage.
 *
 * Note that the command returns different data based on currently used
 * networking standard (ETSI,FCC,ARIB,...). Data interpretation is
 * user responsibility, for more information see manual.
 *
 * @param drvData Driver run-time data.
 * @param channelUsage Pointer to resulting data containing channel usage.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetFCCMacroChUsage(sf_drv_data_t *drvData, uint8_t *channelUsage);

/*!
 * @brief Restores the default macro channel settings.
 *
 * Before sending any frame over SPI for RCZ2/4,
 * MCU must sent "Get_FCC_macro_channel" to check the free macro channels first
 * and then call "Reset_FCC_Macro_Channel", if required.
 * This command is necessary for RCZ2 and RCZ4 devices only, because these
 * regions use frequency hopping.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ResetFCCMacroChannel(sf_drv_data_t *drvData);

/*!
 * @brief Calibrates TX frequency. The passed drift offset measured in
 * Hz is added or subtracted to the LO when doing RF TX.
 *
 * @param drvData  Driver run-time data.
 * @param signPlus Determines whether the offset is added or subtracted from
 * LO (true - add, false - subtract).
 * @param driftHz  Drift offset in Hz.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_CalibrateFreqStatic(sf_drv_data_t *drvData, bool signPlus,
        uint32_t driftHz);

/*!
 * @brief Updates temperature frequency calibration table.
 *
 * @param drvData  Driver run-time data.
 * @param driftTable Drift table containing signed integer values (see
 * #SF_TEMP_FREQ_CAL_TBL_SIZE).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_UpdateTempFreqCalTable(sf_drv_data_t *drvData, const uint8_t driftTable[]);

/*!
 * @brief Reads temperature frequency calibration table.
 *
 * @param drvData  Driver run-time data.
 * @param driftTableRes Pointer where the resulting drift table is stored.
 * The array must have sufficient size (see #SF_TEMP_FREQ_CAL_TBL_SIZE).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ReadTempFreqCalTable(sf_drv_data_t *drvData, uint8_t driftTableRes[]);

/*!
 * @brief Sets default temperature frequency calibration table.
 *
 * @param drvData  Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SetDefaultTempFreqCalTable(sf_drv_data_t *drvData);

/*!
 * @brief Gets the recently computed RSSI measure.
 *
 * @param drvData  Driver run-time data.
 * @param rssiRes  Pointer where the measured RSSI is stored.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetLastSentRSSI(sf_drv_data_t *drvData, uint8_t *rssiRes);

/*!
 * @brief Gets the recently computed RSSI measure for 30 frames
 * during RX GFSK test. This is the receiver sensitivity test.
 *
 * @param drvData  Driver run-time data.
 * @param rssiRes  Pointer where the measured RSSI are stored.
 * The array must have sufficient size (see #SF_SENT_RSSI_TBL_SIZE).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_GetLast30SentRSSI(sf_drv_data_t *drvData, uint8_t *rssiRes);

/*!
 * @brief Resets buffer which holds the recently computed RSSI for 30
 * frames during RX GFSK test. This is the receiver sensitivity test.
 *
 * @param drvData  Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ResetLastSentRSSIBuffer(sf_drv_data_t *drvData);

/*!
 * @brief Reads current ETSI/FCC PA table.
 *
 * @param drvData     Driver run-time data.
 * @param netStd      Network standard.
 * @param paTable     Pointer where the resulting PA table is stored.
 * The array must have sufficient size (see #SF_PA_TBL_SIZE).
 * Note that when using FCC, only first 86 values are used in OL2385.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ReadCurrentPATable(sf_drv_data_t *drvData, sf_net_standard_t netStd, uint8_t *paTable);

/*!
 * @brief Updates current ETSI/FCC PA table.
 *
 * @param drvData     Driver run-time data.
 * @param netStd      Network standard.
 * @param paTable     Pointer where the PA table is stored.
 * The array must have sufficient size (see #SF_PA_TBL_SIZE).
 * Note that when using FCC, only first 86 values are used in OL2385.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_UpdatePATable(sf_drv_data_t *drvData, sf_net_standard_t netStd, uint8_t *paTable);

/*!
 * @brief Sets default ETSI/FCC PA table.
 *
 * @param drvData     Driver run-time data.
 * @param netStd      Network standard.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SetDefaultPATable(sf_drv_data_t *drvData, sf_net_standard_t netStd);

/*!
 * @brief Sets the initial RSSI drift offset.
 *
 * @param drvData     Driver run-time data.
 * @param signPlus    Determines whether the offset is added or subtracted
   (true - add, false - subtract).
 * @param offset      RSSI drift offset.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_CalibrateInitialRSSI(sf_drv_data_t *drvData, bool signPlus, uint16_t offset);

/*!
 * @brief This function sends a command to the device. It does not wait for
 * an acknowledgement.
 *
 * The user can use SF_HAS_CMD_ACK macro to figure out if a command
 * has ACK. Then he can utilize SF_IsAckFrameReady and SF_ReadAckFrameNonBlock
 * functions to receive the ACK.
 *
 * @param drvData     Driver run-time data.
 * @param cmd         The frame command number.
 * @param sendPayload Pointer to a frame payload to be sent. The user can set
 * payload data (sendPayload->payload) to NULL when the size is zero.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_SendCommandNonBlock(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        const sf_msg_payload_t *sendPayload);

/*!
 * @brief This function checks if the SIGFOX device is ready to send an
 * acknowledgement. It checks ACK pin value, which is active low.
 *
 * The user can use SF_HAS_CMD_ACK macro to check if a command has an
 * acknowledgement.
 *
 * @param drvData     Driver run-time data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
bool SF_IsAckFrameReady(sf_drv_data_t *drvData);

/*!
 * @brief This function receives an acknowledge frame.
 *
 * It should be used in conjunction with SF_SendCommandNonBlock and
 * SF_IsAckFrameReady functions. The user can use SF_HAS_CMD_ACK macro to check
 * if a command has an acknowledgement.
 *
 * @param drvData      Driver run-time data.
 * @param recvPayload  Pointer where a resulting payload is stored. Size of the
 *                     data variable (recvPayload->payload) must be sufficient
 *                     (see SF_ACK_PAYLOAD_MAX_B macro). The size variable
 *                     (recvPayload->payloadLen) will be set by this function
 *                     and indicates the number of actually received bytes.
 * @param recvBufferSize Size of the receive buffer in bytes.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t SF_ReadAckFrameNonBlock(sf_drv_data_t *drvData,
        sf_msg_payload_t *recvPayload, uint8_t recvBufferSize);
/*! @} */

#endif /* SOURCE_SF_SF_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
